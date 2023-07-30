#include "stdAfx.h"
#include "math.h"

CWinThread* m_ReadThread;          // ���߳̾��
ULONGLONG g_TrigPosAD;
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------------
UINT ReadDataThread_Npt(PVOID hWnd);
UINT ReadDataThread_Dma(PVOID hWnd);
UINT ReadDataThread_Half(PVOID hWnd);

//###############################################################
// �ɼ��̺߳���(����ʽ��ȡ����)
//
UINT ReadDataThread_Npt(PVOID hWnd)  
{
	CADDoc* pDoc = theApp.m_pADDoc; 
	gl_nReadIndex = 0;              // ��λ��������
	ULONG nReadSizeWords = HALF_SIZE_WORDS - HALF_SIZE_WORDS % gl_nChannelCount;       // ��ȡ����

	HANDLE hDevice = theApp.m_hDevice; // �豸���

	if (!PCI8510_InitDeviceProAD(hDevice, &ADPara)) // ��Ӳ��������ʼ���豸
	{
		AfxMessageBox(_T("�����ĳ�ʼ������....."), MB_ICONERROR);		
		return -1;
	}

	PCI8510_StartDeviceProAD(hDevice);  // ��ʼ���������豸���� 

	while (gl_bDeviceADRun)  // ѭ���ɼ�AD����
	{	

		// ������
		if (!PCI8510_ReadDeviceProAD_Npt(hDevice, ADBuffer[gl_nReadIndex], nReadSizeWords, (LONG*)gl_ReadSizeWords))
		{
			AfxMessageBox(_T("Read Data Error....."));
		}

		if (!PCI8510_GetDevStatusProAD(hDevice, &ADStatus))
		{
			AfxMessageBox(_T("Get Device Status Error...\n"));	
			goto ExitReadThread;
		}			

		if (gl_nProcMode == PROC_MODE_SAVE)
		{
			DWORD dTmp;
			if (!WriteFile(gl_hFileObject, ADBuffer[gl_nReadIndex], nReadSizeWords*sizeof(LONG),
				&dTmp, NULL))
			{
				AfxMessageBox(L"error");
			}

		}
		          
		gl_nReadIndex++;
		if (gl_nReadIndex >= MAX_SEGMENT_COUNT) gl_nReadIndex = 0;

		// �����¼������߻��ƴ����̣߳����������Ѳɼ����
		SetEvent(gl_hEvent); // ������ʾ	
		
		if (gl_FirstScreenStop) // ��Ҫ������ֹͣ����ֻ��һ��ˢ��	
		{
			gl_bDeviceADRun = FALSE; // ��ֹAD�ɼ�
			PCI8510_ReleaseDeviceProAD(hDevice); // �ͷ�AD
			goto ExitReadThread;
		}
	} // �߳�ѭ��ȡ�� 

	
ExitReadThread:
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

	if (!PCI8510_InitDeviceProAD(hDevice, &ADPara)) // ��Ӳ��������ʼ���豸
	{
		AfxMessageBox(_T("�����ĳ�ʼ������....."), MB_ICONERROR);		
		return -1;
	}

	PCI8510_StartDeviceProAD(hDevice);  // ��ʼ���������豸���� 
	int nTempIndex = 0;
	int gl_TempIndex(0);
	while (gl_bDeviceADRun)  // ѭ���ɼ�AD����
	{	
		while (gl_bDeviceADRun) // ��ѯ��ǰFIFO�洢��״̬�Ƿ��Ѵﵽ����
		{
			if (!PCI8510_GetDevStatusProAD(hDevice, &ADStatus))
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
		long nRetReadSizeWords;
		if (!PCI8510_ReadDeviceProAD_Half(hDevice, ADBuffer[gl_nReadIndex], iReadSizeWords, (long*)&nRetReadSizeWords))
		{
			AfxMessageBox(_T("Read Data Error....."));
		}

		if (gl_nProcMode == PROC_MODE_SAVE)
		{
			DWORD dTmp;
			if (!WriteFile(gl_hFileObject, ADBuffer[gl_nReadIndex], iReadSizeWords*sizeof(LONG),
				&dTmp, NULL))
			{
				AfxMessageBox(L"error");
			}
		}
		
		gl_nReadIndex++;
		
		if (gl_nReadIndex >= MAX_SEGMENT_COUNT)	gl_nReadIndex = 0;

		SetEvent(gl_hEvent);// ������ʾ	

		if (gl_FirstScreenStop) // ��Ҫ������ֹͣ����ֻ��һ��ˢ��	
		{
			gl_bDeviceADRun = FALSE;            // ��ֹAD�ɼ�
			PCI8510_ReleaseDeviceProAD( hDevice ); // �ͷ�AD			
			goto ExitReadThread;
		}
	}// �߳�ѭ��ȡ��   
	
ExitReadThread:
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

	gl_nReadIndex = 0;										// ADBuffer��ǰд���ݻ���������
	ULONG SegmentID = 0;
	PCI8510_STATUS_DMA DMAStatus;                           // Ӳ��״̬	
	HANDLE hDevice = pApp->m_hDevice;                       // �豸���	
	HANDLE hDmaEvent = PCI8510_CreateSystemEvent();			// �����ж��¼�

	int DMASegment0 = 0;

	if (!PCI8510_InitDeviceDmaAD(hDevice, hDmaEvent, 
								&ADBuffer[0][0], iReadSizeWords,
								MAX_SEGMENT_COUNT, AD_FIFO_HALF_LENGTH, &ADPara)) // ��Ӳ��������ʼ���豸 AD_FIFO_HALF_LENGTH
	{
		AfxMessageBox(_T("�����ĳ�ʼ������....."), MB_ICONERROR);		
		return -1;
	}
	PCI8510_StartDeviceDmaAD(hDevice);  // ��ʼ���������豸���� 
	int nTempIndex = 0;
	int IntCount = 0;
	while (gl_bDeviceADRun)				// ѭ���ɼ�AD����
	{		
		while (TRUE)
		{
			if (WaitForSingleObject(hDmaEvent, 100) == WAIT_OBJECT_0)
				break;
			else
			{
				if (!gl_bDeviceADRun) 
					goto ExitReadThread;
			}
		}		
		
		if (!PCI8510_GetDevStatusDmaAD(hDevice, &DMAStatus))
		{
			AfxMessageBox(_T("Get Device Status Error...\n"));	
			goto ExitReadThread;
		}
		
		SegmentID = DMASegment0;

		for (int index = 0; index < MAX_SEGMENT_COUNT; index++)
		{
			if(DMAStatus.bSegmentSts[SegmentID])
			{
				gl_nReadIndex = SegmentID;

				if(gl_nProcMode == PROC_MODE_SAVE)
				{

					DWORD dTmp;
					if (!WriteFile(gl_hFileObject, ADBuffer[gl_nReadIndex], iReadSizeWords*sizeof(LONG),
						&dTmp, NULL))
					{
						AfxMessageBox(L"error");
					}
				}

				
				gl_nReadIndex++;
		
				if (gl_nReadIndex >= MAX_SEGMENT_COUNT)	gl_nReadIndex = 0;
				SetEvent(gl_hEvent); // �����¼������߻��ƴ����̣߳����������Ѳɼ����

				
				if(!PCI8510_SetDevStatusDmaAD(hDevice, SegmentID))
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
		PCI8510_StopDeviceDmaAD(hDevice);
		//PCI8510_ReleaseDeviceDmaAD( hDevice ); // �ͷ�AD
		
		if (!PCI8510_GetDevStatusDmaAD(hDevice, &DMAStatus))
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

					DWORD dTmp;
					if (!WriteFile(gl_hFileObject, ADBuffer[gl_nReadIndex], iReadSizeWords*sizeof(LONG),
						&dTmp, NULL))
					{
						AfxMessageBox(L"error");
					}
				}
				
				gl_nReadIndex = SegmentID;			
				gl_nReadIndex++;				
				if (gl_nReadIndex >= MAX_SEGMENT_COUNT)	gl_nReadIndex = 0;
				SetEvent(gl_hEvent); // �����¼������߻��ƴ����̣߳����������Ѳɼ����
				
				if(!PCI8510_SetDevStatusDmaAD(hDevice, SegmentID))
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


	if (gl_nSampleMode == NPT_MODE && ADPara.TriggerMode == PCI8510_TRIGMODE_POST)
	{
		HANDLE hTemp = PCI8510_CreateDevice(theApp.m_CurrentDeviceID);		
		PCI8510_ReleaseDeviceProAD(hTemp); // �ͷ�AD	
		PCI8510_ReleaseDevice(hTemp); 
	}
	
	::WaitForSingleObject(gl_hExitEvent, INFINITE);

	switch(gl_nSampleMode)
	{
	case NPT_MODE:
		PCI8510_ReleaseDeviceProAD( hDevice ); // �ͷ�AD
		break;
	case HALF_MODE:
		PCI8510_ReleaseDeviceProAD( hDevice ); // �ͷ�AD
		break;
	case DMA_MODE:
		PCI8510_ReleaseDeviceDmaAD( hDevice ); // �ͷ�AD
		break;
	}
	
	gl_bCreateDevice = FALSE;
	return TRUE;
}

