#include "stdAfx.h"
#include "math.h"

CWinThread* m_ReadThread;          // 子线程句柄
ULONGLONG g_TrigPosAD;
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------------
UINT ReadDataThread_Npt(PVOID hWnd);
UINT ReadDataThread_Dma(PVOID hWnd);
UINT ReadDataThread_Half(PVOID hWnd);

//###############################################################
// 采集线程函数(程序方式读取数据)
//
UINT ReadDataThread_Npt(PVOID hWnd)  
{
	CADDoc* pDoc = theApp.m_pADDoc; 
	gl_nReadIndex = 0;              // 复位段索引号
	ULONG nReadSizeWords = HALF_SIZE_WORDS - HALF_SIZE_WORDS % gl_nChannelCount;       // 读取长度

	HANDLE hDevice = theApp.m_hDevice; // 设备句柄

	if (!PCI8510_InitDeviceProAD(hDevice, &ADPara)) // 用硬件参数初始化设备
	{
		AfxMessageBox(_T("不明的初始化错误....."), MB_ICONERROR);		
		return -1;
	}

	PCI8510_StartDeviceProAD(hDevice);  // 初始化后，启动设备工作 

	while (gl_bDeviceADRun)  // 循环采集AD数据
	{	

		// 读数据
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

		// 发送事件，告诉绘制窗口线程，该批数据已采集完毕
		SetEvent(gl_hEvent); // 控制显示	
		
		if (gl_FirstScreenStop) // 若要求首屏停止，则只第一次刷新	
		{
			gl_bDeviceADRun = FALSE; // 中止AD采集
			PCI8510_ReleaseDeviceProAD(hDevice); // 释放AD
			goto ExitReadThread;
		}
	} // 线程循环取样 

	
ExitReadThread:
	SetEvent(gl_hExitEvent); // 发出设备成功退出消息，让关闭设备的线程得此消息
	if (gl_FirstScreenStop)	pDoc->StopDeviceAD();

	return TRUE;  
} // ReadDataThread_Pro

// 采集线程函数(半满方式读取数据)
UINT ReadDataThread_Half(PVOID hWnd)
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADDoc* pDoc = pApp->m_pADDoc;
	HANDLE hDevice = pApp->m_hDevice; // 设备句柄
	int iReadSizeWords = HALF_SIZE_WORDS - HALF_SIZE_WORDS % gl_nChannelCount; // 读取数据的大小(在半满方式下,应等于小于半满长度)

	if (!PCI8510_InitDeviceProAD(hDevice, &ADPara)) // 用硬件参数初始化设备
	{
		AfxMessageBox(_T("不明的初始化错误....."), MB_ICONERROR);		
		return -1;
	}

	PCI8510_StartDeviceProAD(hDevice);  // 初始化后，启动设备工作 
	int nTempIndex = 0;
	int gl_TempIndex(0);
	while (gl_bDeviceADRun)  // 循环采集AD数据
	{	
		while (gl_bDeviceADRun) // 查询当前FIFO存储器状态是否已达到半满
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

		SetEvent(gl_hEvent);// 控制显示	

		if (gl_FirstScreenStop) // 若要求首屏停止，则只第一次刷新	
		{
			gl_bDeviceADRun = FALSE;            // 中止AD采集
			PCI8510_ReleaseDeviceProAD( hDevice ); // 释放AD			
			goto ExitReadThread;
		}
	}// 线程循环取样   
	
ExitReadThread:
	SetEvent(gl_hExitEvent); // 发出设备成功退出消息，让关闭设备的线程得此消息
	if (gl_FirstScreenStop)	pDoc->StopDeviceAD();
	
	return TRUE;  

} // ReadDataThread_Half

// 采集线程函数(DMA方式读取数据)
UINT ReadDataThread_Dma(PVOID hWnd)  
{
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	CADDoc* pDoc = pApp->m_pADDoc; 

	int iReadSizeWords = HALF_SIZE_WORDS - HALF_SIZE_WORDS % gl_nChannelCount; // 读取数据的大小

	gl_nReadIndex = 0;										// ADBuffer当前写数据缓冲区索引
	ULONG SegmentID = 0;
	PCI8510_STATUS_DMA DMAStatus;                           // 硬件状态	
	HANDLE hDevice = pApp->m_hDevice;                       // 设备句柄	
	HANDLE hDmaEvent = PCI8510_CreateSystemEvent();			// 创建中断事件

	int DMASegment0 = 0;

	if (!PCI8510_InitDeviceDmaAD(hDevice, hDmaEvent, 
								&ADBuffer[0][0], iReadSizeWords,
								MAX_SEGMENT_COUNT, AD_FIFO_HALF_LENGTH, &ADPara)) // 用硬件参数初始化设备 AD_FIFO_HALF_LENGTH
	{
		AfxMessageBox(_T("不明的初始化错误....."), MB_ICONERROR);		
		return -1;
	}
	PCI8510_StartDeviceDmaAD(hDevice);  // 初始化后，启动设备工作 
	int nTempIndex = 0;
	int IntCount = 0;
	while (gl_bDeviceADRun)				// 循环采集AD数据
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
				SetEvent(gl_hEvent); // 发送事件，告诉绘制窗口线程，该批数据已采集完毕

				
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
		
		if (gl_FirstScreenStop) // 若要求首屏停止，则只第一次刷新	
		{
			gl_bDeviceADRun = FALSE;            // 中止AD采集			
			goto ExitReadThread;
		}
		
	}// 线程循环取样 
	
ExitReadThread:

	if (ADPara.ClockSource == 0)
	{
		PCI8510_StopDeviceDmaAD(hDevice);
		//PCI8510_ReleaseDeviceDmaAD( hDevice ); // 释放AD
		
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
				SetEvent(gl_hEvent); // 发送事件，告诉绘制窗口线程，该批数据已采集完毕
				
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


	SetEvent(gl_hExitEvent); // 发出设备成功退出消息，让关闭设备的线程得此消息
	if (gl_FirstScreenStop)	pDoc->StopDeviceAD();		  
	return 0;		  
} // ReadDataThread_Dma

BOOL MyStopDeviceAD(HANDLE hDevice)
{
	gl_bDeviceADRun = FALSE;   // 中止AD采集


	if (gl_nSampleMode == NPT_MODE && ADPara.TriggerMode == PCI8510_TRIGMODE_POST)
	{
		HANDLE hTemp = PCI8510_CreateDevice(theApp.m_CurrentDeviceID);		
		PCI8510_ReleaseDeviceProAD(hTemp); // 释放AD	
		PCI8510_ReleaseDevice(hTemp); 
	}
	
	::WaitForSingleObject(gl_hExitEvent, INFINITE);

	switch(gl_nSampleMode)
	{
	case NPT_MODE:
		PCI8510_ReleaseDeviceProAD( hDevice ); // 释放AD
		break;
	case HALF_MODE:
		PCI8510_ReleaseDeviceProAD( hDevice ); // 释放AD
		break;
	case DMA_MODE:
		PCI8510_ReleaseDeviceDmaAD( hDevice ); // 释放AD
		break;
	}
	
	gl_bCreateDevice = FALSE;
	return TRUE;
}

