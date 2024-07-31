#include "stdAfx.h"
#include "Sys.h"
#include "ADThread.h"
#include "ADDigitView.h"
#include "math.h"

CWinThread* m_ReadThread;  // 子线程句柄
BOOL        gl_bTerminateThr = TRUE; // 线程是否删除
BOOL		gl_bSmlThenSize = FALSE;
//////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------------------------------------------------------------------
UINT ReadDataThread(PVOID hWnd);
//########################################################################
// 启动数据采集线程
BOOL MyStartDeviceAD(HANDLE hDevice)
{
	if(!gl_bTerminateThr)				// 上一次线程还没有删除
	{
		AfxMessageBox(_T("上一次的线程还没有中止"));
		return FALSE; 
	}

	m_ReadThread = AfxBeginThread(ReadDataThread,NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_ReadThread->m_bAutoDelete = true; // 自动删除线程
	gl_bDeviceADRun = TRUE;				// 告之采集线程可以工作了
	m_ReadThread->ResumeThread();		// 启动采集线程ReadDataThread_NotEmpty
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

// 读取数据 线程函数
UINT ReadDataThread(PVOID hWnd)  
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADDoc* pDoc=pApp->m_pADDoc; 
	ULONG ReadOffsetWords = 0;
	LONG RetReadSizeWords;
	int nADChannel = 0;
	DWORD dTmp=0;
	HANDLE hDevice = pApp->m_hDevice;
	ULONG SampleCnt = 1;		// 采集次数
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
	if(gl_bAutoCollect)			// 是手动采集的话，显示完了就停止采集了
	{	
		SampleCnt = 0xFFFFFFFF;
	}
	gl_bStoped = FALSE;


	LONG lBufferID = 0;
	ULONG nConstReadCnt = 0;// 连续方式下用于获取状态
	ACTS1000_STATUS_AD ADStatus;
	
	if(!ACTS1000_InitDeviceAD(hDevice, &ADPara)) // 初始化设备
	{
		AfxMessageBox(_T("硬件初始化错误,详细信息请查看log文件"), MB_ICONERROR);		

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
		for(SampleIndex = 0; SampleIndex<SampleCnt; SampleIndex++)// 循环采集AD数据
		{
			if(!gl_bDeviceADRun)
				goto ExitReadThread;
			if (!ACTS1000_StartDeviceAD(hDevice))// 初始化后，启动设备工作 	
			{

				MsgBox(_T("启动失败!"));
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
					if(gl_ADStatus.bComplete) // 自动存盘单次有限点任务是否完成
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
							SetEvent(hEvent);					// 发送事件，告诉绘制窗口线程，该批数据已采集完毕
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
						AfxMessageBox(_T("读数据出错!"));
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
						FileLenghtWords += nRead;	// 存盘文件大小增加
					}
					if (!m_bDrawComp)
					{
						glBufferID = lBufferID;
					}

					lBufferID++;
					if (lBufferID >= MAX_SEGMENT)
						lBufferID = 0;

					SetEvent(hEvent);					// 发送事件，告诉绘制窗口线程，该批数据已采集完毕
				}		
			}


FinComplete:
			ACTS1000_StopDeviceAD(hDevice);  // 初始化后，启动设备工作 	

		}// 线程循环取样
	}
	else
	{
		lBufferID = 0;
		if (!ACTS1000_StartDeviceAD(hDevice))// 初始化后，启动设备工作 	
		{
			MsgBox(_T("启动失败!"));
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
						SetEvent(hEvent);					// 发送事件，告诉绘制窗口线程，该批数据已采集完毕
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
					AfxMessageBox(_T("读数据出错!"));
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

					FileLenghtWords += nRetSizeWords;	// 存盘文件大小增加
				}

				if (!m_bDrawComp)
				{
					glBufferID = lBufferID;

				}
				lBufferID++;
				if (lBufferID >= MAX_SEGMENT)
					lBufferID = 0;

				SetEvent(hEvent);					// 发送事件，告诉绘制窗口线程，该批数据已采集完毕
				nConstReadCnt++;
				if (nConstReadCnt%4000==0)
				{
					if (ACTS1000_GetDevStatusAD(hDevice,&ADStatus))
					{
						if (ADStatus.bAheadTrig)
						{
							AfxMessageBox(_T("硬件溢出,1:请降低采样率 2:提高程序效率 3:使用Simple\AD\Continue下的exe测试传输速度!"));
							goto ExitReadThread;
						}
					}
				}
			}
		}
	}	
ExitReadThread:				
	SetEvent(hExitEvent);	// 发出设备成功退出消息，让关闭设备的线程得此消息
	gl_bTerminateThr = TRUE;
	gl_bCollected = TRUE;	// 完成了一次采集

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
 