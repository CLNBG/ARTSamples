// ADDoc.cpp : implementation of the CADDoc class
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CADDoc

IMPLEMENT_DYNCREATE(CADDoc, CDocument)

BEGIN_MESSAGE_MAP(CADDoc, CDocument)
//{{AFX_MSG_MAP(CADDoc)
ON_COMMAND(IDM_TileWave, OnTileWave)
ON_UPDATE_COMMAND_UI(IDM_TileWave, OnUpdateTileWave)
ON_COMMAND(IDM_SuperPoseWave, OnSuperPoseWave)
ON_UPDATE_COMMAND_UI(IDM_SuperPoseWave, OnUpdateSuperPoseWave)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
///////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CADDoc construction/destruction

CADDoc::CADDoc()
{	
	// TODO: add one-time construction code here
	gl_bProgress = FALSE;           // 一开始不更新进度条
	gl_nProcMode = PROC_MODE_DIGIT; // 默认方式：数字显示
	gl_TopTriggerVolt = (int)AD_VOLT_RANGE; // 置触发电平的初值为其最大量程
	gl_TopTriggerVolt = 0;
	gl_bCreateFile = TRUE;  // 一开始，为第一次创建文件对象
	gl_bCloseFile = TRUE;   // 没有打开文件时，认为文件被关闭
	gl_nChannelCount = 2;
}

CADDoc::~CADDoc()
{

}

BOOL CADDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CADDoc serialization

void CADDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here 
	}
}

/////////////////////////////////////////////////////////////////////////////
// CADDoc diagnostics

#ifdef _DEBUG
void CADDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CADDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


void CADDoc::StartDeviceAD() 
{
	HANDLE hDevice = theApp.m_hDevice;
	////////////////////////////////////////////////////////////////////////
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		if (hDevice == INVALID_HANDLE_VALUE)
		{
			AfxMessageBox(_T("对不起，无PCI设备存在...") , MB_ICONERROR);
			return;
		}
	}

	
	CADFrm* pADFrm = (CADFrm*)(theApp.m_ADFrm); // 取得子帧窗口指针
	CButton* pStart = (CButton*)((pADFrm->m_wndSTCBar).GetDlgItem(IDM_StartDeviceAD));
	CButton* pStop = (CButton*)((pADFrm->m_wndSTCBar).GetDlgItem(IDM_StopDeviceAD));

	gl_pParaCfgView->EnableControlWindows(FALSE);  // 在设备开始运行时，禁止操作硬件参数
	gl_pADStatusView->EnableControlWindows(FALSE);

	int Channel = 0;
	gl_nChannelCount = 0; 
	for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
	{
		if (ADPara.bChannelArray[Channel])
			gl_nChannelCount++;
	}

	SYSTEMTIME  time;
	int nRandNum;

	if (!gl_bCreateFile) // 如果创建了文件对象，则先写头文件
	{
		::GetSystemTime(&time); 
		srand(time.wMilliseconds); // 种子数
		nRandNum =  rand();        // 随机数
		m_header.HeadSizeBytes = sizeof(FILE_HEADER); // 头信息大小
		m_header.FileType= DEFAULT_FILE_TYPE;         // 文件类型
		m_header.BusType = DEFAULT_BUSTYPE;           // 总线类型
		m_header.DeviceNum = DEFAULT_DEVICE_NUM;	  // 设备ID号
		m_header.HeadVersion = 0x0600;                // 头版本号: 6.0
		m_header.ADPara = ADPara;                     // 硬件参数	
		m_header.nTriggerPos = 0;                     // 触发点位置
		m_header.BatCode = nRandNum<<2;             // 批文件识别码
		m_header.HeadEndFlag = HEAD_END_FLAG;       // 文件头结束符
		
		for (int nChannel=0; nChannel<MAX_AD_CHANNELS; nChannel++)
		{
			if (ADPara.bChannelArray[nChannel] == TRUE)
			{
				switch(ADPara.InputRange[nChannel])         // 电压量程
				{		
				case PCI9757_INPUT_N10000_P10000mV:  // ±10000mV
					m_header.VoltBottomRange[nChannel] = -10000;
					m_header.VoltTopRange[nChannel] = 10000;
					break;
				case PCI9757_INPUT_N5000_P5000mV:  // ±5000mV
					m_header.VoltBottomRange[nChannel] = -5000;
					m_header.VoltTopRange[nChannel] = 5000;
					break;

				case PCI9757_INPUT_N2500_P2500mV:  // ±5000mV
					m_header.VoltBottomRange[nChannel] = -2500;
					m_header.VoltTopRange[nChannel] = 2500;
					break;
					
					
				case PCI9757_INPUT_0_P10000mV:    // 10000mV
					m_header.VoltBottomRange[nChannel] = 0;
					m_header.VoltTopRange[nChannel] = 10000;
					break;		
				
				case PCI9757_INPUT_0_P5000mV:    // 5000mV
					m_header.VoltBottomRange[nChannel] = 0;
					m_header.VoltTopRange[nChannel] = 5000;
					break;
					
				default:
					ASSERT(FALSE);
					break;
				}
			}			
		}
		
		// 写文件头
		ASSERT("wirte file");
		
		g_FileOut.Write(
			(WORD*)&m_header, 
			m_header.HeadSizeBytes);  
	}
	
	gl_FileLenghtWords = 0; // 文件长度初始化
	
//	CADFrm* pADFrm = (CADFrm*)(theApp.m_ADFrm); // 取得子帧窗口指针
	CProgressCtrl* pProgress = (CProgressCtrl*)(pADFrm->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));	
	pProgress->SetPos(0);  // 释放设备时，置进度条为起值
	
	memset(ADBuffer, 0x00, sizeof(ADBuffer)); // 将用户缓冲区复位清零
	
	if (gl_nSampleMode == NPT_MODE) // 非空方式
	{
		m_hReadThread = AfxBeginThread(ReadDataThread_Npt, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	}
	if (gl_nSampleMode == HALF_MODE) // 半满方式
	{
		m_hReadThread = AfxBeginThread(ReadDataThread_Half, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	}
	if (gl_nSampleMode == DMA_MODE) // DMA方式
	{
		m_hReadThread = AfxBeginThread(ReadDataThread_Dma, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	}

	ASSERT(m_hReadThread != NULL);	
	m_hReadThread->m_bAutoDelete = FALSE; // 自动删除线程
	gl_bDeviceADRun = TRUE;             // 告之采集线程可以工作了
	
	// 创建绘制窗口线程
 	m_hProcThread = AfxBeginThread(ProcessDataThread, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED); 
	ASSERT(m_hProcThread != NULL);
	m_hProcThread->m_bAutoDelete = FALSE; // 自动删除 
	gl_bDeviceADRun = TRUE; // 设置设备启动标志

	// 创建数据采集与窗口绘制同步事件对象
	gl_hEvent = PCI9757_CreateSystemEvent();
	::ResetEvent(gl_hEvent);
	gl_hExitEvent = PCI9757_CreateSystemEvent(); 


	
	pStart->EnableWindow(FALSE);	
	pStop->EnableWindow(TRUE);

	CADWaveView* pWaveView = (CADWaveView*)CView::FromHandle(m_hWndWave);
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();

	m_hProcThread->ResumeThread(); // 启动绘制窗口线程
	m_hReadThread->ResumeThread();       // 启动采集线程
	gl_bDataProcessing = FALSE;
}

//###################  处理数据线程函数 #########################
UINT ProcessDataThread(PVOID pThreadPara)  
{
	CADFrm* pADFrm = theApp.m_ADFrm;
	CADDoc* pADDoc = theApp.m_pADDoc;
	int SegmentIndex = 0;
	ULONG WroteMB = 0;  
	int ShowCount = 0;
	gl_nDrawIndex = 0;
	int ProcessCount = 0;
	while (gl_bDeviceADRun)  // 循环处理AD数据
	{ 
		do{
			if (!gl_bDeviceADRun) goto ExitProcDataThread;
		}while (WaitForSingleObject(gl_hEvent, 256) != WAIT_OBJECT_0);  

		SegmentIndex = gl_nReadIndex - 1;
		if(SegmentIndex < 0)
		{
			SegmentIndex = MAX_SEGMENT_COUNT - 1;
			
		}
		switch(gl_nProcMode)  // 判断数据处理方式
		{
		case PROC_MODE_DIGIT: // 数字处理
			if (gl_bDataProcessing == FALSE)
			{
				ProcessCount++;
				gl_nDrawIndex = SegmentIndex; // 如果窗口已完成数据刷新，则置新的缓冲区索引号，使之绘制新缓冲区
				::SendMessage(pADDoc->m_hWndDigit, WM_SHOW_DIGIT, NULL, NULL);
			}
			gl_bProgress = TRUE; // 使OnDraw函数能更新进度条		
			break;
			
		case PROC_MODE_WAVE:  // 波形处理
			if (gl_bDataProcessing == FALSE)
			{
				gl_nDrawIndex = SegmentIndex; // 如果窗口已完成数据刷新，则置新的缓冲区索引号，使之绘制新缓冲区
				::SendMessage(pADDoc->m_hWndWave, WM_SHOW_WAVE, NULL, NULL);
			}
			gl_bProgress = TRUE; // 使OnDraw函数能更新进度条	
			break;
			
		case PROC_MODE_SAVE: // 数据存盘处理	
			if (!gl_bDeviceADRun) goto ExitProcDataThread;
			
			g_FileOut.Write(ADBuffer[SegmentIndex], gl_ReadSizeWords*sizeof(LONG));
			
			CString strFileLenght;
			WroteMB = (WroteMB+16)%1024;
			gl_FileLenghtWords += 16; // 16Kb
			pADFrm->GetProgressCtrl()->SetPos((int)WroteMB);					
			strFileLenght.Format(_T("%d") , gl_FileLenghtWords);
			int iDotPos = 0;
			iDotPos = strFileLenght.GetLength()-3;
			while (iDotPos>0)
			{
				strFileLenght.Insert(iDotPos, ',');
				iDotPos -= 3;
			}
			
			strFileLenght += " Kb";
			pADFrm->GetFileLenghtStatic()->SetWindowText(strFileLenght);
			ShowCount++;
			break;
		} // end switch
		CString str;
		CEdit* pEditOffset = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_Offset);
	} // end while
	
ExitProcDataThread:

	return TRUE;
}
//##########################################################################################

void CADDoc::Public_ReleaseDevice(void) // 供MDI窗口调用
{
	StopDeviceAD();
}

void CADDoc::StopDeviceAD() 
{
	CSysApp* pApp = (CSysApp *)AfxGetApp();
	gl_bDeviceADRun = FALSE; // 停止子线程进行连续数据采集	
	Sleep(20);

	CADFrm* pADFrm = ((CSysApp*)AfxGetApp())->m_ADFrm; // 取得子帧窗口句柄

	// 停止定时器取得触发点位置
	m_hADParaCfgView->KillTimer(1);

	if (/*ADPara.TriggerMode == PCI9757_TRIGMODE_POST &&*/ gl_nSampleMode == NPT_MODE)
	{
		HANDLE hDeviceTTT = PCI9757_CreateDevice(theApp.m_CurrentDeviceID);
		PCI9757_ReleaseDeviceProAD(hDeviceTTT);
		PCI9757_ReleaseDevice(hDeviceTTT);
	}


// 	::WaitForSingleObject(m_hReadThread->m_hThread, INFINITE);
// 	delete m_hReadThread;
// 	m_hReadThread = NULL;	
// 	
// 	::WaitForSingleObject(m_hProcThread->m_hThread, INFINITE);
// 	delete m_hProcThread;
// 	m_hProcThread = NULL;
	

//	::WaitForSingleObject(gl_hExitEvent, INFINITE);



	if (!MyStopDeviceAD(pApp->m_hDevice)) // 关闭AD设备
	{
		AfxMessageBox(_T("关闭AD设备失败!"));
		return;
	}

	gl_bCreateDevice = FALSE;  
	PCI9757_ReleaseSystemEvent(gl_hEvent); // 释放消息
	gl_hEvent = INVALID_HANDLE_VALUE;
	// 终止采集线程
	CProgressCtrl* pProgress = (CProgressCtrl*)(pADFrm->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));
	pProgress->SetPos(0);  // 释放设备时，置进度条为起值
	
	// 在设备对象释放后，允许操作硬件参数
	gl_pParaCfgView->EnableControlWindows(TRUE);	
	gl_pADStatusView->EnableControlWindows(TRUE);  
	gl_bCreateDevice = FALSE;			// 释放设备后，视设备未创建
	if (gl_nProcMode == PROC_MODE_SAVE)  // 如果数据处理方式为存盘
	{		
		g_FileOut.SeekToBegin(); // 返回到文件起始位置
		m_header.StaticOverFlow = ADStatus.bStatic_Overflow;
		m_header.DynamicOverflow = ADStatus.bDynamic_Overflow;
		m_header.nTriggerPos = ADStatus.nTriggerPos;
		g_FileOut.Write((WORD*)&m_header, m_header.HeadSizeBytes);
		OnCloseFile();					// 释放文件对象
		gl_nProcMode = PROC_MODE_DIGIT; // 置成数字显示方式			
		CButton* pDataSave = (CButton*)(gl_pADStatusView->GetDlgItem(IDM_DataSave));	
		pDataSave->EnableWindow(FALSE);  // 使存盘方式单选框有效
	}
}

void CADDoc::OnDataFileNew() 
{
	int Channel = 0;
	CString strNewFileName;
	CString TempFileName;
	HANDLE hDevice = theApp.m_hDevice;

	if (!(theApp.DoPromptFileName(strNewFileName, IDS_NEW_FILE, 
		OFN_HIDEREADONLY | OFN_CREATEPROMPT, TRUE, NULL)))
		return;
 
	if (!gl_bCreateFile)
	{
		g_FileOut.Close();
	}
	
	// 创建新文件对象
	int FileNameLength = strNewFileName.GetLength();
	TempFileName = strNewFileName.Left(FileNameLength-4);

	strNewFileName = TempFileName + ".pci";
	
	if (!g_FileOut.Open(strNewFileName, CFile::modeCreate| CFile::modeWrite))		
	{
		AfxMessageBox(_T("创建文件对象失败...") , MB_ICONSTOP, 0);
		return;
	}	
	g_strFileFullName = strNewFileName;

	CADFrm* pADFrm = theApp.m_ADFrm;	// 取得子帧窗口句柄
	pADFrm->ShowHideDlgSave ();			// 显示存盘栏
	pADFrm->ShowHideStatus(FALSE);		// 隐藏状态栏
	
	CButton* pDataSave = (CButton*)(gl_pADStatusView->GetDlgItem(IDM_DataSave));	
	pDataSave->EnableWindow(TRUE);		// 使存盘方式单选框有效
	
	_ULARGE_INTEGER dlFreeSpace, dlTotalSpace, dlUserSpace;

	GetDiskFreeSpaceEx(strNewFileName.Left(3), &dlUserSpace, &dlUserSpace, &dlFreeSpace);
	(void)dlTotalSpace;
	(void)dlUserSpace;

	ULONGLONG DiskFreeBytes = dlFreeSpace.QuadPart; 

	// 取得文件所在磁盘可用空间
	m_RemainMB = (ULONG)(DiskFreeBytes/(1024*1024));  // 求出有多少M字	
	CString str;
	
	if (m_RemainMB<2) 
	{
		AfxMessageBox(_T("硬盘剩余空间不足2M，存盘操作不能进行...") , MB_ICONSTOP, 0);
		return;
	}

	str.Format(_T("%d Mb") , m_RemainMB);
	pADFrm->GetDiskFreeStatic()->SetWindowText(str); // 置当前硬盘可用空间MB
	
	pADFrm->GetProgressCtrl()->SetRange32(0, 1024); // 设置进程范围
	pADFrm->GetProgressCtrl()->SetPos(0);
	
	/////////////
	pADFrm->GetFileNameStatic()->SetWindowText(strNewFileName); // 显示存盘文件名
	gl_bCreateFile = FALSE;  // 若成功文件对象，则置标志	
	
	gl_bCloseFile = FALSE;	// 有文件没有关闭, 可以操作《关闭当前文件》菜单
	gl_pADStatusView->SetDlgCheck();
	
	gl_nProcMode = PROC_MODE_SAVE; // 当打开新文件时，意味着要进行存盘处理
	gl_pADStatusView->SetDlgCheck();	
}

void CADDoc::OnCloseFile() 
{
	g_FileOut.Close();
	
	this->SetPathName(g_strFileFullName);
	
	SetTitle(_T("AD数采测试"));

	gl_bCloseFile = TRUE;  // 文件被关闭
	gl_nProcMode = PROC_MODE_DIGIT; // 当关闭新文件时，意味着要不能再进行存盘处理，因此置成数字方式
	gl_bCreateFile = TRUE; // 当新文件被关闭后，意味着以后新文件还是第一次被创建 
	///////
	CADFrm* pADFrm = ((CSysApp*)AfxGetApp())->m_ADFrm; // 取得子帧窗口句柄
	CButton* pDataSave = (CButton*)(gl_pADStatusView->GetDlgItem(IDM_DataSave));	
	pDataSave->EnableWindow(TRUE);  // 使存盘方式单选框有效
	
	CString str;
	str.Format(_T("%d") , 0);
	pADFrm->GetDiskFreeStatic()->SetWindowText(str);
	pADFrm->ShowHideDlgSave (FALSE);	// 隐藏存盘栏
	pADFrm->ShowHideStatus();			// 显示状态栏
	pADFrm->GetProgressCtrl()->SetRange32(0, 1024);  // 设置进程范围
	pADFrm->GetProgressCtrl()->SetPos(0);	
	// 一兆进度
	gl_pADStatusView->SetDlgCheck();
	
}

void CADDoc::OnTileWave() 
{
	gl_bTileWave = TRUE;
	CButton* pButtonTile = (CButton*)gl_pADStatusView->GetDlgItem(IDM_TileWave);
	pButtonTile->SetCheck(1);
	CButton* pButtonPose = (CButton*)gl_pADStatusView->GetDlgItem(IDM_SuperPoseWave);
	pButtonPose->SetCheck(0);
    CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
	
}

void CADDoc::OnSuperPoseWave() 
{
	gl_bTileWave = FALSE;
	CButton* pButtonTile = (CButton*)gl_pADStatusView->GetDlgItem(IDM_TileWave);
	pButtonTile->SetCheck(0);
	CButton* pButtonPose = (CButton*)gl_pADStatusView->GetDlgItem(IDM_SuperPoseWave);
	pButtonPose->SetCheck(1);
    CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
	
}

void CADDoc::OnUpdateSuperPoseWave(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(gl_bTileWave == TRUE ? 0:1);
}

void CADDoc::OnUpdateTileWave(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(gl_bTileWave == TRUE ? 1:0);
	
}
