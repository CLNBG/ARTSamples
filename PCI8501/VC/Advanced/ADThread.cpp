#include "stdAfx.h"
#include "Sys.h"
#include "ADThread.h"
#include "ADDigitView.h"
#include "math.h"

CWinThread* m_ReadThread;  // ���߳̾��
BOOL        gl_bTerminateThr = TRUE; // �߳��Ƿ�ɾ��
BOOL		gl_bSmlThenSize = FALSE;
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------------
UINT ReadDataThread(PVOID hWnd);
//########################################################################
// �������ݲɼ��߳�
BOOL MyStartDeviceAD(HANDLE hDevice)
{
	if(!gl_bTerminateThr)				// ��һ���̻߳�û��ɾ��
	{
		AfxMessageBox(L"��һ�ε��̻߳�û����ֹ");
		return FALSE; 
	}
	for (int i=0; i<MAX_CHANNEL_COUNT; i++)
	{
		for (int j=0; j<2; j++)
		{
			for (int k=0; k<MAX_RAM_SIZE; k++)
			{
				ADBuffer1[i][j][k]= 0;
			}
		}
	}
	m_ReadThread = AfxBeginThread(ReadDataThread,NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_ReadThread->m_bAutoDelete = true; // �Զ�ɾ���߳�
	gl_bDeviceADRun = TRUE;				// ��֮�ɼ��߳̿��Թ�����
	m_ReadThread->ResumeThread();		// �����ɼ��߳�ReadDataThread_NotEmpty

	return TRUE;
}

//###############################################################
PCI8501_PARA_AD gl_OldADPara = ADPara;
BOOL IsChangePara()
{
	BOOL bRet;
	if(memcmp(&gl_OldADPara, &ADPara, sizeof(ADPara))==0)
		bRet = FALSE;
	else
		bRet = TRUE;
	gl_OldADPara = ADPara;
	return bRet;
}

// ��ȡ���� �̺߳���
UINT ReadDataThread(PVOID hWnd)  
{
	PWORD Buffer;
 	Buffer = new WORD[MAX_RAM_SIZE];
	//WORD Buffer[MAX_RAM_SIZE];
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADDoc* pDoc=pApp->m_pADDoc; 
	ULONG ReadOffsetWords = 0;
	LONG RetReadSizeWords;
	int nADChannel = 0;

	HANDLE hDevice = pApp->m_hDevice;
	ULONG SampleCnt = 1;		// �ɼ�����
	ULONG SampleIndex;
	ULONG RemainSizeWords = 0;
	LONG nRetSizeWords;
	ULONG nIndex = 0;
	if(gl_bAutoCollect)			// ���ֶ��ɼ��Ļ�����ʾ���˾�ֹͣ�ɼ���
	{	
		SampleCnt = 0xFFFFFFFF;
	}
	
	LONG nReadSize;
	ULONG ulStartAddr=0;

	if (ADPara.TriggerMode == PCI8501_TRIGMODE_DELAY)
	{
		gl_ReadSizeWords = ADPara.N_Length*8; // ���ʵ�ʲ�������С��8K������ʵ�ʳ���Ϊ׼
	}
	else
	{
		gl_ReadSizeWords = (ADPara.M_Length + ADPara.N_Length)*8; // ���ʵ�ʲ�������С��8K������ʵ�ʳ���Ϊ׼
	}
	
	
	if(IsChangePara()) // ���Ӳ�����������˱仯�������³�ʼ��Ӳ��
	{
ReInitDevice:
		if(!PCI8501_InitDeviceAD(hDevice, &ADPara)) // ��ʼ���豸
		{
			AfxMessageBox(L"�����ĳ�ʼ������.....", MB_ICONERROR);		
			return FALSE;
		}
	}

	gl_bTerminateThr = FALSE ;
	//ulStartAddr=0;
	DWORD dTmp;
	for(SampleIndex = 0; SampleIndex<SampleCnt; SampleIndex++)// ѭ���ɼ�AD����
	{
		if(!gl_bDeviceADRun)
			goto ExitReadThread;
		PCI8501_StartDeviceAD(hDevice);  // ��ʼ���������豸���� 	
		
	//	PCI8501_SetDeviceTrigAD(hDevice);  // ��ʼ���������豸���� 	

		while(TRUE) 
		{
			if(!PCI8501_GetDevStatusAD(hDevice, &gl_ADStatus))
			{
				MsgBox(L"GetDevStatusAD Error");
				goto ExitReadThread;
			}
			if(gl_ADStatus.bComplete)
			{
				break; // �����ɱ�־��Ч�����˳�״̬��ѯ���������ݶ�ȡ�ʹ���׶�
			}
			else
			{
				if(IsChangePara()) // ���Ӳ�����������˱仯�������³�ʼ��Ӳ��
				{
					goto ReInitDevice;
				}
				if(!gl_bDeviceADRun) goto ExitReadThread;
				Sleep(10);
			}
		}

		ulStartAddr=0;
		nReadSize = gl_ReadSizeWords;
		if(!PCI8501_GetDevStatusAD(hDevice, &gl_ADStatus))
		{
			MsgBox(L"GetDevStatusAD Error");
			goto ExitReadThread;
		}
		
		LONG offsetAddr	= gl_ADStatus.lEndAddr + 4 - gl_ReadSizeWords*sizeof(WORD);
		if (offsetAddr<0)
		{
			ulStartAddr = gl_DDR2Lenth - gl_ReadSizeWords*sizeof(WORD) + gl_ADStatus.lEndAddr + 4;
		}
		else
			ulStartAddr = (ULONG)offsetAddr;

		PCI8501_StopDeviceAD(hDevice);  // ��ʼ���������豸���� 	
		ULONG ulReadCount = 0;
		while (nReadSize>0)
		{
			if (nReadSize > MAX_RAM_SIZE)
			{
				ulReadCount = MAX_RAM_SIZE;
				gl_bSmlThenSize = FALSE;
			}
			else
			{
				gl_bSmlThenSize = TRUE;
				ulReadCount = nReadSize;
			}

			if (ulStartAddr> gl_DDR2Lenth)
			{
				ulStartAddr = ulStartAddr - gl_DDR2Lenth;
			}
// 			if ((ulReadCount%4096) != 0)
// 			{
// 				ulReadCount = ulReadCount + (4096-ulReadCount%4096);
// 			}
			if(!gl_bDeviceADRun)
				goto ExitReadThread;
				RetReadSizeWords = PCI8501_ReadDeviceAD(hDevice, Buffer, ulReadCount, ulStartAddr, &nRetSizeWords);
				if(RetReadSizeWords == -1)
				{
					AfxMessageBox(L"�����ݳ���!");
					goto ExitReadThread;
				}


			ulStartAddr += ulReadCount*sizeof(WORD);
 			nReadSize -= ulReadCount;	
			
			glBufferID++;
			if (glBufferID >= MAX_SEGMENT)
				glBufferID = 0;
			
			int nCount=0;
			for (nIndex=0; nIndex<ulReadCount/gl_nChannelCount; nIndex++)
			{
				nCount=0;
				for (int nCH=0; nCH<MAX_CHANNEL_COUNT; nCH++)
				{					
					ADBuffer1[nCH][glBufferID][nIndex] = Buffer[gl_nChannelCount*nIndex + nCount];
					nCount++;
					
				}
			}			
			
			if (nProcessMode == SAVE_MODE)
			{		
				for (nADChannel = 0; nADChannel<MAX_CHANNEL_COUNT; nADChannel++)
				{

					WriteFile(hFileObject[nADChannel], ADBuffer1[nADChannel][glBufferID], ulReadCount/gl_nChannelCount*sizeof(WORD), &dTmp, NULL);					
					//PCI8501_WriteFile(hFileObject[nADChannel], ADBuffer1[nADChannel][glBufferID], ulReadCount/gl_nChannelCount*sizeof(WORD)); // �����ݴ����Ӳ����
				}		
				FileLenghtWords += (ulReadCount*sizeof(WORD));	// �����ļ���С����

			}

			memcpy(&ADBuffer, &ADBuffer1, sizeof(ADBuffer));
			SetEvent(hEvent);					// �����¼������߻��ƴ����̣߳����������Ѳɼ����
		}		
		
		if(gl_bAutoCollect && nProcessMode == SAVE_MODE)
		{
			
		//	for (nADChannel = 0; nADChannel<MAX_CHANNEL_COUNT; nADChannel++)
			//{						// �������־���ݴ������(1024����)	
				//PCI8501_WriteFile(hFileObject[nADChannel], IsolateData, 1024*2);	
// 				WriteFile(hFileObject[nADChannel], IsolateData, 1024*2,
// 					&dTmp, NULL);
// 				{
// 					AfxMessageBox(_T("WriteFile error"));
// 				}
		//	}
		}
	}// �߳�ѭ��ȡ��
	
ExitReadThread:
				
	SetEvent(hExitEvent);	// �����豸�ɹ��˳���Ϣ���ùر��豸���̵߳ô���Ϣ
	gl_bTerminateThr = TRUE;
	gl_bCollected = TRUE;	// �����һ�βɼ�
	
	if (!gl_bAutoCollect)
	{
		pApp->m_pADDoc->StopDeviceAD();
	}
	
	
 	delete Buffer;

	return TRUE;  
}

BOOL MyStopDeviceAD(HANDLE hDevice)
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	
	gl_bDeviceADRun = FALSE;
	WaitForSingleObject(m_ReadThread->m_hThread, INFINITE);
	bCreateDevice = FALSE;	
	PCI8501_StopDeviceAD(hDevice);  // ��ʼ���������豸����
	PCI8501_ReleaseDeviceAD(hDevice);  // ��ʼ���������豸���� 	

	return TRUE;
}
 