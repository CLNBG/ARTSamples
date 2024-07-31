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
		AfxMessageBox(_T("��һ�ε��̻߳�û����ֹ"));
		return FALSE; 
	}

	m_ReadThread = AfxBeginThread(ReadDataThread,NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_ReadThread->m_bAutoDelete = true; // �Զ�ɾ���߳�
	gl_bDeviceADRun = TRUE;				// ��֮�ɼ��߳̿��Թ�����
	m_ReadThread->ResumeThread();		// �����ɼ��߳�ReadDataThread_NotEmpty
	return TRUE;
}

BOOL bWriteCnt = 0;
//###############################################################
ACTS1000_PARA_AD gl_OldADPara = ADPara;
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
BOOL gl_bStoped = FALSE;

// ��ȡ���� �̺߳���
UINT ReadDataThread(PVOID hWnd)  
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADDoc* pDoc=pApp->m_pADDoc; 
	ULONG ReadOffsetWords = 0;
	LONG RetReadSizeWords;
	int nADChannel = 0;
	DWORD dTmp=0;
	HANDLE hDevice = pApp->m_hDevice;
	ULONG SampleCnt = 1;		// �ɼ�����
	ULONG SampleIndex;
	ULONG RemainSizeWords = 0;
	ULONG nRetSizeWords;
	ULONG nAvailSampsPoints;
	ULONG nIndex = 0;
	BOOL bSuccess = FALSE;
	ULONGLONG nReadSize;
	ULONG nRead;
	ULONG lSaveTime;

	ULONG ulReadCount = MAX_RAM_SIZE;
	if(gl_bAutoCollect)			// ���ֶ��ɼ��Ļ�����ʾ���˾�ֹͣ�ɼ���
	{	
		SampleCnt = 0xFFFFFFFF;
	}
	gl_bStoped = FALSE;


	LONG lBufferID = 0;
	ULONG nConstReadCnt = 0;// ������ʽ�����ڻ�ȡ״̬
	ACTS1000_STATUS_AD ADStatus;
	
	if(!ACTS1000_InitDeviceAD(hDevice, &ADPara)) // ��ʼ���豸
	{
		AfxMessageBox(_T("Ӳ����ʼ������,��ϸ��Ϣ��鿴log�ļ�"), MB_ICONERROR);		

		goto ExitReadThread;
	}

	if (ADPara.TriggerMode == ACTS1000_TRIGMODE_MIDL)
	{
		gl_ReadSizeWords = (ADPara.M_Length + ADPara.N_Length)*gl_nChannelCount; 
	}
	else
	{
		gl_ReadSizeWords = ADPara.N_Length;
		gl_ReadSizeWords = gl_ReadSizeWords*gl_nChannelCount; 
	}
	if (ADPara.TriggerMode == ACTS1000_TRIGMODE_POST || ADPara.TriggerMode == ACTS1000_TRIGMODE_DELAY)
	{
		if (ADPara.TrigCount==0)
		{
			gl_ReadSizeWords = 0xFFFFFFFFFFFFFFFF;
		}
		else
		{
			gl_ReadSizeWords = gl_ReadSizeWords*ADPara.TrigCount;
		}
	}



	lSaveTime = GetTickCount();
	gl_bTerminateThr = FALSE ;
	if (ADPara.SampleMode == ACTS1000_SAMPMODE_FINITE)
	{
		for(SampleIndex = 0; SampleIndex<SampleCnt; SampleIndex++)// ѭ���ɼ�AD����
		{
			if(!gl_bDeviceADRun)
				goto ExitReadThread;
			if (!ACTS1000_StartDeviceAD(hDevice))// ��ʼ���������豸���� 	
			{

				MsgBox(_T("����ʧ��!"));
				goto ExitReadThread;
			}
			//ACTS1000_SetDeviceTrigAD(hDevice);

			if (ADPara.bSaveFile)
			{
				while(TRUE) 
				{
					if(!ACTS1000_GetDevStatusAD(hDevice, &gl_ADStatus))
					{
						MsgBox(_T("GetDevStatusAD Error"));
						goto ExitReadThread;
					}
					if(gl_ADStatus.bComplete) // �Զ����̵������޵������Ƿ����
					{
						goto FinComplete;
					}
					else
					{
						if(!gl_bDeviceADRun) goto ExitReadThread;
						Sleep(1);
					}
					if (GetTickCount()-lSaveTime>300)
					{	
						if (gl_ADStatus.lSavePoints>FileLenghtWords)
						{
							FileLenghtWords=gl_ADStatus.lSavePoints;
							SetEvent(hEvent);					// �����¼������߻��ƴ����̣߳����������Ѳɼ����
							lSaveTime = GetTickCount();
						}
					}
				}
			}
			else	// if (ADPara.bSaveFile)
			{


				nReadSize = gl_ReadSizeWords;
				while (nReadSize>0)
				{
					if (nReadSize>=ulReadCount)
					{
						nRead = ulReadCount;
						gl_bSmlThenSize = FALSE;
					}
					else
					{
						gl_bSmlThenSize = TRUE;
						nRead = nReadSize;
					}
NextRead:
					if(!gl_bDeviceADRun) 
						goto ExitReadThread;
					RetReadSizeWords = ACTS1000_ReadDeviceAD(hDevice, ADBuffer[lBufferID], nRead, &nRetSizeWords, &nAvailSampsPoints,1.0);
					if(RetReadSizeWords == 0)
					{
						AfxMessageBox(_T("�����ݳ���!"));
						goto ExitReadThread;
					}
					if (nRetSizeWords==0)
					{
						int a=0;
					}
					if (nRetSizeWords<=0)
					{
						goto NextRead;
					}
					if (nRetSizeWords != 262144)
					{
						int a= 0;
					}
					nReadSize -= nRetSizeWords;	
					bSuccess = TRUE;
					if (nProcessMode == SAVE_MODE)
					{					
						WriteFile(g_ArrFileOut,ADBuffer[lBufferID], nRetSizeWords*sizeof(WORD),&dTmp,NULL);
						FileLenghtWords += nRead;	// �����ļ���С����
					}
					if (!m_bDrawComp)
					{
						glBufferID = lBufferID;
					}

					lBufferID++;
					if (lBufferID >= MAX_SEGMENT)
						lBufferID = 0;

					SetEvent(hEvent);					// �����¼������߻��ƴ����̣߳����������Ѳɼ����
				}		
			}


FinComplete:
			ACTS1000_StopDeviceAD(hDevice);  // ��ʼ���������豸���� 	

		}// �߳�ѭ��ȡ��
	}
	else
	{
		lBufferID = 0;
		if (!ACTS1000_StartDeviceAD(hDevice))// ��ʼ���������豸���� 	
		{
			MsgBox(_T("����ʧ��!"));
			goto ExitReadThread;
		}

		//ACTS1000_SetDeviceTrigAD(hDevice);
		if (ADPara.bSaveFile)
		{
			while(TRUE) 
			{
				if(!ACTS1000_GetDevStatusAD(hDevice, &gl_ADStatus))
				{
					MsgBox(_T("GetDevStatusAD Error"));
					goto ExitReadThread;
				}
				if(!gl_bDeviceADRun)
					goto ExitReadThread;
				Sleep(1);				
				if (GetTickCount()-lSaveTime>300)
				{	
					if (gl_ADStatus.lSavePoints>FileLenghtWords)
					{
						FileLenghtWords=gl_ADStatus.lSavePoints;
						SetEvent(hEvent);					// �����¼������߻��ƴ����̣߳����������Ѳɼ����
						lSaveTime = GetTickCount();
					}
				}
			}
		}
		else
		{
			nRead = ulReadCount;
			while(gl_bDeviceADRun) 
			{
				RetReadSizeWords = ACTS1000_ReadDeviceAD(hDevice, ADBuffer[lBufferID], nRead, &nRetSizeWords, &nAvailSampsPoints,1.0);
				if(RetReadSizeWords == 0)
				{
					AfxMessageBox(_T("�����ݳ���!"));
					goto ExitReadThread;
				}
				if (nRetSizeWords<=0)
				{
					Sleep(1);
					continue;
				}
				bSuccess = TRUE;
				if (nProcessMode == SAVE_MODE)
				{		
					WriteFile(g_ArrFileOut,ADBuffer[lBufferID], nRetSizeWords*sizeof(WORD),&dTmp,NULL);

					FileLenghtWords += nRetSizeWords;	// �����ļ���С����
				}

				if (!m_bDrawComp)
				{
					glBufferID = lBufferID;

				}
				lBufferID++;
				if (lBufferID >= MAX_SEGMENT)
					lBufferID = 0;

				SetEvent(hEvent);					// �����¼������߻��ƴ����̣߳����������Ѳɼ����
				nConstReadCnt++;
				if (nConstReadCnt%4000==0)
				{
					if (ACTS1000_GetDevStatusAD(hDevice,&ADStatus))
					{
						if (ADStatus.bAheadTrig)
						{
							AfxMessageBox(_T("Ӳ�����,1:�뽵�Ͳ����� 2:��߳���Ч�� 3:ʹ��Simple\AD\Continue�µ�exe���Դ����ٶ�!"));
							goto ExitReadThread;
						}
					}
				}
			}
		}
	}	
ExitReadThread:				
	SetEvent(hExitEvent);	// �����豸�ɹ��˳���Ϣ���ùر��豸���̵߳ô���Ϣ
	gl_bTerminateThr = TRUE;
	gl_bCollected = TRUE;	// �����һ�βɼ�

	ACTS1000_StopDeviceAD(hDevice);
	ACTS1000_ReleaseDeviceAD(hDevice);

	if (!gl_bAutoCollect && bSuccess && gl_bStoped==FALSE)
	{
		//pApp->m_pADDoc->StopDeviceAD();
	}
	

	return TRUE;  
}

BOOL MyStopDeviceAD(HANDLE hDevice)
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();

	gl_bDeviceADRun = FALSE;
	if (gl_bAutoTrig)
	{
		ACTS1000_SetDeviceTrigAD(pApp->m_hDevice);
	}
	if (gl_bAutoCollect == FALSE)
	{

		gl_bStoped = TRUE;
		WaitForSingleObject(m_ReadThread->m_hThread, 1000);
	}
	else
	{
		WaitForSingleObject(m_ReadThread->m_hThread, INFINITE);
	}
	bCreateDevice = FALSE;	
	return TRUE;
}
 