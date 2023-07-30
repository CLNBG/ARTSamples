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
		AfxMessageBox(L"上一次的线程还没有中止");
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
	m_ReadThread->m_bAutoDelete = true; // 自动删除线程
	gl_bDeviceADRun = TRUE;				// 告之采集线程可以工作了
	m_ReadThread->ResumeThread();		// 启动采集线程ReadDataThread_NotEmpty

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

// 读取数据 线程函数
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
	ULONG SampleCnt = 1;		// 采集次数
	ULONG SampleIndex;
	ULONG RemainSizeWords = 0;
	LONG nRetSizeWords;
	ULONG nIndex = 0;
	if(gl_bAutoCollect)			// 是手动采集的话，显示完了就停止采集了
	{	
		SampleCnt = 0xFFFFFFFF;
	}
	
	LONG nReadSize;
	ULONG ulStartAddr=0;

	if (ADPara.TriggerMode == PCI8501_TRIGMODE_DELAY)
	{
		gl_ReadSizeWords = ADPara.N_Length*8; // 如果实际采样长度小于8K，则以实际长度为准
	}
	else
	{
		gl_ReadSizeWords = (ADPara.M_Length + ADPara.N_Length)*8; // 如果实际采样长度小于8K，则以实际长度为准
	}
	
	
	if(IsChangePara()) // 如果硬件参数发生了变化，则重新初始化硬件
	{
ReInitDevice:
		if(!PCI8501_InitDeviceAD(hDevice, &ADPara)) // 初始化设备
		{
			AfxMessageBox(L"不明的初始化错误.....", MB_ICONERROR);		
			return FALSE;
		}
	}

	gl_bTerminateThr = FALSE ;
	//ulStartAddr=0;
	DWORD dTmp;
	for(SampleIndex = 0; SampleIndex<SampleCnt; SampleIndex++)// 循环采集AD数据
	{
		if(!gl_bDeviceADRun)
			goto ExitReadThread;
		PCI8501_StartDeviceAD(hDevice);  // 初始化后，启动设备工作 	
		
	//	PCI8501_SetDeviceTrigAD(hDevice);  // 初始化后，启动设备工作 	

		while(TRUE) 
		{
			if(!PCI8501_GetDevStatusAD(hDevice, &gl_ADStatus))
			{
				MsgBox(L"GetDevStatusAD Error");
				goto ExitReadThread;
			}
			if(gl_ADStatus.bComplete)
			{
				break; // 如果完成标志有效，则退出状态查询，进入数据读取和处理阶段
			}
			else
			{
				if(IsChangePara()) // 如果硬件参数发生了变化，则重新初始化硬件
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

		PCI8501_StopDeviceAD(hDevice);  // 初始化后，启动设备工作 	
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
					AfxMessageBox(L"读数据出错!");
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
					//PCI8501_WriteFile(hFileObject[nADChannel], ADBuffer1[nADChannel][glBufferID], ulReadCount/gl_nChannelCount*sizeof(WORD)); // 将数据存放在硬盘上
				}		
				FileLenghtWords += (ulReadCount*sizeof(WORD));	// 存盘文件大小增加

			}

			memcpy(&ADBuffer, &ADBuffer1, sizeof(ADBuffer));
			SetEvent(hEvent);					// 发送事件，告诉绘制窗口线程，该批数据已采集完毕
		}		
		
		if(gl_bAutoCollect && nProcessMode == SAVE_MODE)
		{
			
		//	for (nADChannel = 0; nADChannel<MAX_CHANNEL_COUNT; nADChannel++)
			//{						// 将隔离标志数据存入磁盘(1024个点)	
				//PCI8501_WriteFile(hFileObject[nADChannel], IsolateData, 1024*2);	
// 				WriteFile(hFileObject[nADChannel], IsolateData, 1024*2,
// 					&dTmp, NULL);
// 				{
// 					AfxMessageBox(_T("WriteFile error"));
// 				}
		//	}
		}
	}// 线程循环取样
	
ExitReadThread:
				
	SetEvent(hExitEvent);	// 发出设备成功退出消息，让关闭设备的线程得此消息
	gl_bTerminateThr = TRUE;
	gl_bCollected = TRUE;	// 完成了一次采集
	
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
	PCI8501_StopDeviceAD(hDevice);  // 初始化后，启动设备工作
	PCI8501_ReleaseDeviceAD(hDevice);  // 初始化后，启动设备工作 	

	return TRUE;
}
 