#include "stdAfx.h"
#include "math.h"

CWinThread* m_ReadThread;          // ���߳̾��
ULONGLONG g_TrigPosAD;
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------------
UINT ReadDataThread_Npt(PVOID hWnd);
UINT ReadDataThread_Dma(PVOID hWnd);
UINT ReadDataThread_Half(PVOID hWnd);
//UINT ReadDataThread_INT(PVOID hWnd);

//###############################################################
// �ɼ��̺߳���(����ʽ��ȡ����)
//
UINT ReadDataThread_Npt(PVOID hWnd)  
{
	CADDoc* pDoc = theApp.m_pADDoc; 
	gl_nReadIndex = 0;              // ��λ��������
	ULONG nReadSizeWords = HALF_SIZE_WORDS - HALF_SIZE_WORDS % gl_nChannelCount;       // ��ȡ����

	HANDLE hDevice = theApp.m_hDevice; // �豸���

	if (!PCI9757_InitDeviceProAD(hDevice, &ADPara)) // ��Ӳ��������ʼ���豸
	{
		AfxMessageBox(_T("�����ĳ�ʼ������.....") , MB_ICONERROR);		
		return -1;
	}

	PCI9757_StartDeviceProAD(hDevice);  // ��ʼ���������豸���� 
				

	while (gl_bDeviceADRun)  // ѭ���ɼ�AD����
	{	

		// ������
		if (!PCI9757_ReadDeviceProAD_Npt(hDevice, ADBuffer[gl_nReadIndex], nReadSizeWords, (LONG*)gl_ReadSizeWords))
		{
			AfxMessageBox(_T("Read Data Error....."));
		}	
		if (!gl_bDeviceADRun ) 
		{		
			goto ExitReadThread;
		}
	
		if (!PCI9757_GetDevStatusProAD(hDevice, &ADStatus))
		{
			AfxMessageBox(_T("Get Device Status Error...\n"));	
			goto ExitReadThread;
		}		

		Sleep(1);
		if (gl_nProcMode == PROC_MODE_SAVE)
			g_FileOut.Write(ADBuffer[gl_nReadIndex], nReadSizeWords*sizeof(LONG));
		          
		gl_nReadIndex++;
		if (gl_nReadIndex >= MAX_SEGMENT_COUNT) gl_nReadIndex = 0;

		// �����¼������߻��ƴ����̣߳����������Ѳɼ����
		SetEvent(gl_hEvent); // ������ʾ	
		
		if (gl_FirstScreenStop) // ��Ҫ������ֹͣ����ֻ��һ��ˢ��	
		{
			gl_bDeviceADRun = FALSE; // ��ֹAD�ɼ�
			PCI9757_ReleaseDeviceProAD(hDevice); // �ͷ�AD
			goto ExitReadThread;
		}
	} // �߳�ѭ��ȡ�� 

	
ExitReadThread:
	
/*	if (ADPara.ClockSource == 0)
	{
		PCI9757_StopDeviceProAD(hDevice);

		// ������
		if (!PCI9757_ReadDeviceProAD_Npt(hDevice, ADBuffer[gl_nReadIndex], nReadSizeWords, (LONG*)gl_ReadSizeWords))
		{
			AfxMessageBox(_T("Read Data Error.....");
		}
		
		if (!PCI9757_GetDevStatusProAD(hDevice, &ADStatus))
		{
			AfxMessageBox(_T("Get Device Status Error...\n");	
		}			
		
		if (gl_nProcMode == PROC_MODE_SAVE)
			PCI9757_WriteFile(gl_hFileObject, ADBuffer[gl_nReadIndex], nReadSizeWords*sizeof(LONG));
		
		gl_nReadIndex++;
		if (gl_nReadIndex >= MAX_SEGMENT_COUNT) gl_nReadIndex = 0;
		
		// �����¼������߻��ƴ����̣߳����������Ѳɼ����
		SetEvent(gl_hEvent); // ������ʾ

	}*/

	SetEvent(gl_hExitEvent); // �����豸�ɹ��˳���Ϣ���ùر��豸���̵߳ô���Ϣ
	if (gl_FirstScreenStop)	pDoc->StopDeviceAD();

	return TRUE;  
} // ReadDataThread_Pro

// �ɼ��̺߳���(������ʽ��ȡ����)
UINT ReadDataThread_Half(PVOID hWnd)
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADDoc* pDoc = pApp->m_pADDoc;
	HANDLE hDevice = pApp->m_hDevice; // �豸���
	int iReadSizeWords = HALF_SIZE_WORDS - HALF_SIZE_WORDS % gl_nChannelCount; // ��ȡ���ݵĴ�С(�ڰ�����ʽ��,Ӧ����С�ڰ�������)

	if (!PCI9757_InitDeviceProAD(hDevice, &ADPara)) // ��Ӳ��������ʼ���豸
	{
		AfxMessageBox(_T("�����ĳ�ʼ������.....") , MB_ICONERROR);		
		return -1;
	}

	PCI9757_StartDeviceProAD(hDevice);  // ��ʼ���������豸���� 
	int nTempIndex = 0;
	int gl_TempIndex(0);
	while (gl_bDeviceADRun)  // ѭ���ɼ�AD����
	{	
		while (gl_bDeviceADRun) // ��ѯ��ǰFIFO�洢��״̬�Ƿ��Ѵﵽ����
		{
			if (!PCI9757_GetDevStatusProAD(hDevice, &ADStatus))
			{
				AfxMessageBox(_T("Get Device Status Error...\n"));	
				goto ExitReadThread;
			}
			
			if (ADStatus.bHalf)
			{
				break;
			}		
			else
			{				
				if (!gl_bDeviceADRun)
					goto ExitReadThread;				
			}
		}
			Sleep(1);
		long nRetReadSizeWords;
		if (!PCI9757_ReadDeviceProAD_Half(hDevice, ADBuffer[gl_nReadIndex], iReadSizeWords, (long*)&nRetReadSizeWords))
		{
			AfxMessageBox(_T("Read Data Error....."));
		}
		
		if (gl_nProcMode == PROC_MODE_SAVE)
			g_FileOut.Write(ADBuffer[gl_nReadIndex], iReadSizeWords*sizeof(LONG));
		
		gl_nReadIndex++;
		
		if (gl_nReadIndex >= MAX_SEGMENT_COUNT)	gl_nReadIndex = 0;

		SetEvent(gl_hEvent);// ������ʾ	

		if (gl_FirstScreenStop) // ��Ҫ������ֹͣ����ֻ��һ��ˢ��	
		{
			gl_bDeviceADRun = FALSE;            // ��ֹAD�ɼ�
			PCI9757_ReleaseDeviceProAD( hDevice ); // �ͷ�AD			
			goto ExitReadThread;
		}
	}// �߳�ѭ��ȡ��   
	
ExitReadThread:
	
	/*if (ADPara.ClockSource == 0)
	{
		PCI9757_StopDeviceProAD(hDevice);

		long nRetReadSizeWords;
		if (!PCI9757_ReadDeviceProAD_Half(hDevice, ADBuffer[gl_nReadIndex], iReadSizeWords, (long*)&nRetReadSizeWords))
		{
			AfxMessageBox(_T("Read Data Error.....");
		}

		if (gl_nProcMode == PROC_MODE_SAVE)
			PCI9757_WriteFile(gl_hFileObject, ADBuffer[gl_nReadIndex], iReadSizeWords*sizeof(LONG));
		
		gl_nReadIndex++;
		
		if (gl_nReadIndex >= MAX_SEGMENT_COUNT)	gl_nReadIndex = 0;

		SetEvent(gl_hEvent);// ������ʾ	
	}
*/
	SetEvent(gl_hExitEvent); // �����豸�ɹ��˳���Ϣ���ùر��豸���̵߳ô���Ϣ
	if (gl_FirstScreenStop)	pDoc->StopDeviceAD();
	
	return TRUE;  

} // ReadDataThread_Half

// �ɼ��̺߳���(DMA��ʽ��ȡ����)
UINT ReadDataThread_Dma(PVOID hWnd)  
{
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	CADDoc* pDoc = pApp->m_pADDoc; 

	int iReadSizeWords = HALF_SIZE_WORDS - HALF_SIZE_WORDS % gl_nChannelCount; // ��ȡ���ݵĴ�С

	gl_nReadIndex = 0;            // ADBuffer��ǰд���ݻ���������
	ULONG SegmentID = 0;
	PCI9757_STATUS_DMA DMAStatus;                            // Ӳ��״̬	
	HANDLE hDevice = pApp->m_hDevice;                      // �豸���	
	HANDLE hDmaEvent = PCI9757_CreateSystemEvent();      // �����ж��¼�

	int DMASegment0 = 0;

	if (!PCI9757_InitDeviceDmaAD(hDevice, hDmaEvent, 
								&ADBuffer[0][0], iReadSizeWords,
								MAX_SEGMENT_COUNT, AD_FIFO_HALF_LENGTH, &ADPara)) // ��Ӳ��������ʼ���豸
	{
		AfxMessageBox(_T("�����ĳ�ʼ������.....") , MB_ICONERROR);		
		return -1;
	}
 	PCI9757_StartDeviceDmaAD(hDevice);  // ��ʼ���������豸���� 
	int nTempIndex = 0;
	int IntCount = 0;
	while (gl_bDeviceADRun) // ѭ���ɼ�AD����
	{	
	//	SetEvent(gl_hEvent);
		Sleep(1);
		while (TRUE)
		{
			if (WaitForSingleObject(hDmaEvent, 200) == WAIT_OBJECT_0)
				break;
			else
			{
				if (!gl_bDeviceADRun ) 
					goto ExitReadThread;
			}
		
		}		
		
		if (!PCI9757_GetDevStatusDmaAD(hDevice, &DMAStatus))
		{
			AfxMessageBox(_T("Get Device Status Error...\n"));	
			goto ExitReadThread;
		}
			Sleep(1);

		SegmentID = DMASegment0;

		for (int index = 0; index < MAX_SEGMENT_COUNT; index++)
		{
			if(DMAStatus.bSegmentSts[SegmentID])
			{

				if(gl_nProcMode == PROC_MODE_SAVE)
				{
					g_FileOut.Write(ADBuffer[gl_nReadIndex], iReadSizeWords*sizeof(LONG));
				}
				
				gl_nReadIndex = SegmentID;

				gl_nReadIndex++;				
				if (gl_nReadIndex >= MAX_SEGMENT_COUNT)	gl_nReadIndex = 0;
 				SetEvent(gl_hEvent); // �����¼������߻��ƴ����̣߳����������Ѳɼ����

				if(!PCI9757_SetDevStatusDmaAD(hDevice, SegmentID))
				{
					AfxMessageBox(_T("SetDevStatusDmaAD Error...\n"));
					goto ExitReadThread;
				}	
				DMASegment0 = (SegmentID + 1)%MAX_SEGMENT_COUNT;
			}
			SegmentID ++;
			if (SegmentID >= MAX_SEGMENT_COUNT)
			{
				SegmentID = 0;
			}
		}
		
		if (gl_FirstScreenStop) // ��Ҫ������ֹͣ����ֻ��һ��ˢ��	
		{
			gl_bDeviceADRun = FALSE;            // ��ֹAD�ɼ�			
			goto ExitReadThread;
		}
		
	}// �߳�ѭ��ȡ�� 
	



	
ExitReadThread:
	if (ADPara.ClockSource == 0)
	{
		PCI9757_StopDeviceDmaAD(hDevice);
		//PCI9757_ReleaseDeviceDmaAD( hDevice ); // �ͷ�AD
		
		if (!PCI9757_GetDevStatusDmaAD(hDevice, &DMAStatus))
		{
			AfxMessageBox(_T("Get Device Status Error...\n"));	
		}	
		
		SegmentID = DMASegment0;	
		for (int index = 0; index < MAX_SEGMENT_COUNT; index++)
		{
			if(DMAStatus.bSegmentSts[SegmentID])
			{			
				if(gl_nProcMode == PROC_MODE_SAVE)
				{
					g_FileOut.Write(ADBuffer[gl_nReadIndex], iReadSizeWords*sizeof(LONG));
				}
				
				gl_nReadIndex = SegmentID;			
				gl_nReadIndex++;				
				if (gl_nReadIndex >= MAX_SEGMENT_COUNT)	gl_nReadIndex = 0;
				SetEvent(gl_hEvent); // �����¼������߻��ƴ����̣߳����������Ѳɼ����
				
				if(!PCI9757_SetDevStatusDmaAD(hDevice, SegmentID))
				{
					AfxMessageBox(_T("SetDevStatusDmaAD Error...\n"));				
				}	
				DMASegment0 = (SegmentID + 1)%MAX_SEGMENT_COUNT;
			}
			SegmentID ++;
			if (SegmentID >= MAX_SEGMENT_COUNT)
			{
				SegmentID = 0;
			}
		}
		
	}

	SetEvent(gl_hExitEvent); // �����豸�ɹ��˳���Ϣ���ùر��豸���̵߳ô���Ϣ
	if (gl_FirstScreenStop)	pDoc->StopDeviceAD();  
	return 0;		  
} // ReadDataThread_Dma



BOOL MyStopDeviceAD(HANDLE hDevice)
{
	gl_bDeviceADRun = FALSE;   // ��ֹAD�ɼ�
	::WaitForSingleObject(gl_hExitEvent, INFINITE);

	switch(gl_nSampleMode)
	{
	case NPT_MODE:
		PCI9757_StopDeviceProAD(hDevice);
		PCI9757_ReleaseDeviceProAD( hDevice ); // �ͷ�AD
		break;
	case HALF_MODE:
		PCI9757_StopDeviceProAD(hDevice);
		PCI9757_ReleaseDeviceProAD( hDevice ); // �ͷ�AD
		break;
	case DMA_MODE:
		PCI9757_StopDeviceDmaAD(hDevice);
		PCI9757_ReleaseDeviceDmaAD( hDevice ); // �ͷ�AD
		break;
	/*case INT_MODE:
		PCI9757_StopDeviceIntAD(hDevice);
		PCI9757_ReleaseDeviceIntAD( hDevice ); // �ͷ�AD
		break;*/
	}
	
	gl_bCreateDevice = FALSE;
	return TRUE;
}

