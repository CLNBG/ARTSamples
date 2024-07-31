// ADDoc.cpp : implementation of the CADDoc class
//

#include "stdafx.h"
#include "Sys.h"
#include "ADDoc.h"
#include "ADDigitView.h"
#include "ADWaveView.h"
#include "ADStatusView.h"
#include "ParaCfgView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#ifdef ACTS1000_CreateSystemEvent
#define ACTS1000_CreateSystemEvent ACTS1000_TCreateSystemEvent 
#endif
////////////////////////////////////////////////////////////////////////
WORD* ADBuffer[MAX_SEGMENT]; // 缓冲队列
WORD* ADDrawBuffer; // 波形刷新缓冲

BOOL	m_bDrawComp= FALSE;
BOOL gl_bAutoCollect = FALSE;
int CurrentIndex; // AD数据处理线程当前缓冲区索引号
int ReadIndex;    // AD数据采集线程当前缓冲区索引号
BOOL gl_bDeviceADRun=FALSE;
BOOL gl_bAutoTrig=TRUE;
ACTS1000_PARA_AD ADPara;
ACTS1000_PARA_AD OldADPara;
ACTS1000_STATUS_AD gl_ADStatus;
ACTS1000_AD_MAIN_INFO gl_ADMainInfo;
WORD gl_wMaxLSB = 0x3FFF;
float gl_fLsbCount = 16384.0;
WORD gl_lLsbHalf = 8192;
//BOOL bCheckOverflow=FALSE; // 是否检测FIFO溢出标志
int  gl_nSampleMode = PRO_MODE;            // 采集模式(1,查询， 2, 中断， 3， DMA)
UINT ReadDataThread(PVOID hWnd);//数据线程
UINT ProccessDataThread(PVOID hWnd);  // 绘制数据线程

BOOL m_bFirstCreateDeviceFile;
int  m_ProcessGraphicMode;//为1,波形显示
BOOL m_AnalyzeCheck;//控制精度分析
int  gl_ScreenVolume;//设置屏幕显示的量程
int  m_TriggerVolt;//触发电压
int gl_MiddleLsb[MAX_CHANNEL_COUNT];
float m_AnalyzeAllCount;
UINT m_OverLimitCount;
int m_ProcessMoveVolt;//为1时,平移电压
BOOL bCreateDevice;
BOOL m_bProgress;  // 是否更新进度条
int nProcessMode;  // 数据处理方式 1：数字显示  2：波形显示  3：数据存盘
CEdit* pCountEdit;
CEdit* pRangeEdit;
CEdit* pRatioEdit;
CADDigitView* m_pDigitView;
CADWaveView* m_pWaveView;
CADStatusView* pADStatusView = NULL;
CParaCfgView* pParaCfgView = NULL;
HANDLE hEvent;  // 采集线程与绘制线程的同步信号
int m_DigitalDataShowMode; // 数字窗口显示模式
int DeviceID;   // 该程序使用的设备ID
HANDLE g_ArrFileOut;

int gl_nChannelCount = MAX_CHANNEL_COUNT;
int gl_nSaveFileCount = MAX_CHANNEL_COUNT;

BOOL m_bCloseFile;
HANDLE hExitEvent;
float Center, PerY;
CRect ClientRect;
float middle1;
BOOL bDrawing=FALSE;

ULONGLONG FileLenghtWords;
int nChannelArrayClient[MAX_CHANNEL_COUNT];// 通道号
int gl_CurrentbufNum = 0; // 当前的缓冲索引号
int DisplayChannel[MAX_CHANNEL_COUNT];// 用于标志各通道是否显示
float OutAnalogValve; // 表示模拟外触发的全局变量
ULONG gl_nSampleLenth = MAX_RAM_SIZE;   // 采样长度
int  gl_InputRange[MAX_CHANNEL_COUNT];    // 各通道设置的电压量程范围
float gl_PerLsbVoltM[MAX_CHANNEL_COUNT]; // 单位LSB的电压值
CString g_strFileFullName;
ULONG gl_StartAddr;

TCHAR g_strChannelEn[2][64] = { L"禁止", L"允许" };
TCHAR g_strGain[4][64] = { L"1倍放大", L"2倍放大", L"4倍放大", L"8倍放大" };
TCHAR g_strRefGround[3][64] = { L"直流", L"交流"};
TCHAR g_strAIRange[3][64] = {L"±5V", L"±1V", L"±10V"};
TCHAR g_strInputImped[2][64] = {L"1MΩ", L"50Ω"};

ULONG m_nSampRangeCount;
ULONG m_nSampRangeVal[8];

ULONG gl_CHIdx[MAX_CHANNEL_COUNT] = 
{
	0,1,2,3,4,5,6,7
};

//##############################################
//使用多媒体定时器以每秒50帧或以下的速度刷新屏幕
#include "stdio.h"
#include "MmSystem.h"

void PASCAL DrawWindowProc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);
#define ONE_MILLI_SECOND 1
#define TWO_SECOND 2000
// 定 义 时 钟 分 辨 率， 以 毫 秒 为 单 位 ---- 
#define TIMER_ACCURACY 1
UINT wAccuracy;  // 定 义 分 辨 率
UINT TimerID; // 定 义 定 时 器 句 柄 

////////////////////////////////////////////////////////////////////////////
// CADDoc

IMPLEMENT_DYNCREATE(CADDoc, CDocument)

BEGIN_MESSAGE_MAP(CADDoc, CDocument)
//{{AFX_MSG_MAP(CADDoc)
ON_COMMAND(IDM_TileWave, OnTileWave)
ON_COMMAND(IDM_SuperPoseWave, OnSuperPoseWave)
ON_UPDATE_COMMAND_UI(IDM_SuperPoseWave, OnUpdateSuperPoseWave)
ON_UPDATE_COMMAND_UI(IDM_TileWave, OnUpdateTileWave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
///////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CADDoc construction/destruction

extern CSysApp theApp;
CADDoc::CADDoc()
{
	// TODO: add one-time construction code here
	m_bProgress=FALSE; // 一开始不更新进度条
	nProcessMode = DATA_MODE;    // 默认方式：数字显示
	m_TriggerVolt=(int)VOLT_RANGE;	// 置触发电平的初值为其最大量程
	m_bFirstCreateDeviceFile=TRUE;  // 一开始，为第一次创建文件对象
	m_bCloseFile=TRUE; // 没有打开文件时，认为文件被关闭	
	gl_nChannelCount=0;
	ACTS1000_LoadParaAD(theApp.m_hDevice, &ADPara);  // 读入系统参数

	for (int i=gl_ADMainInfo.nChannelCount; i<ACTS1000_AD_MAX_CHANNELS; i++)
	{
		ADPara.bChannelArray[i] = 0;
	}

	for (int nCH=0; nCH<gl_ADMainInfo.nChannelCount; nCH++)
	{
		if (ADPara.bChannelArray[nCH])
		{
			gl_nChannelCount++;
		}
	}

	//gl_nChannelCount = MAX_CHANNEL_COUNT;

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

BOOL gl_bTerminateDrawThr = TRUE;
void CADDoc::StartDeviceAD() 
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	////////////////////////////////////////////////////////////////////////
	if(pApp->m_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("对不起，无设备存在..."),MB_ICONERROR);
		return;
	}
	if(!gl_bTerminateDrawThr)
	{
		AfxMessageBox(_T(" 线程没有中止"));
		return;
	}
	FileLenghtWords = 0; // 文件长度初始化
	m_DrawWindowThread = AfxBeginThread(ProccessDataThread,NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_DrawWindowThread->m_bAutoDelete = TRUE; // 自动删除    
	
	// 创建数据采集与窗口绘制同步事件对象
	hEvent = ACTS1000_CreateSystemEvent();
	hExitEvent = ACTS1000_CreateSystemEvent();
	bCollectData=TRUE;  // 创建设备对象后，可以进行数据采集
// 	for(int i=0;i<MAX_CHANNEL_COUNT;i++)
// 		gl_MiddleLsb[i]=0;
	
	
	if(!m_bFirstCreateDeviceFile) // 如果创建了文件对象
	{
		header.HeadSizeBytes = sizeof(FILE_HEADER);   // 头信息大小
		header.FileType = DEFAULT_FILE_TYPE;
		header.BusType = gl_ADMainInfo.nDeviceType>>16;        // 该设备总线类型
		header.DeviceNum = DEFAULT_DEVICE_NUM;	 
		header.DeviceType = gl_ADMainInfo.nDeviceType&0xFFFF;	 // 设备ID号
		header.ChannelCount = gl_nChannelCount;                 // 每通道都是独立的，所以为1
		header.DataWidth = gl_ADMainInfo.nSampResolution;   // 12位
		header.bXorHighBit = 0;                  // 高位不用求反
		header.nRevisionID = theApp.m_nRevisionID;

		ULONG nSampRangeCount;
	ULONG nSampRangeVal[8];
		if (!ACTS1000_GetSupportRangeInfo(theApp.m_hDevice, &nSampRangeCount,nSampRangeVal))
		{
			AfxMessageBox(L"获去支持量程信息失败",MB_ICONWARNING,0);

			return;
		}

		// 还没有设置
		switch(ADPara.InputRange[0])	// 电压量程
		{		
		case ACTS1000_INPUT_N1000_P1000mV: // ±1000毫伏
			header.VoltTopRange = 1000;
			header.VoltBottomRange = -1000;
			break;		
		case ACTS1000_INPUT_N10000_P10000mV: // ±5000毫伏
			
			header.VoltTopRange = 10000;
			header.VoltBottomRange = -10000;
			break;
		case ACTS1000_INPUT_N5000_P5000mV:	
			header.VoltTopRange = 5000;
			header.VoltBottomRange = -5000;
			break;		
		}			
		header.ADPara = ADPara;
		header.CrystalFreq = gl_ADMainInfo.nBaseRate; // 晶振频率
		header.HeadEndFlag = HEAD_END_FLAG;
		header.ADStatus = gl_ADStatus;

		time_t nowtime;       
		time(&nowtime);
		header.CreateFTime = (ULONGLONG)nowtime;
		memcpy(&header.ADMainInfo, &gl_ADMainInfo, sizeof(gl_ADMainInfo));

		// 写文件头
		//	g_ArrFileOut[Index].Write((WORD*)&header, header.HeadSizeBytes);
		DWORD dTmp;
		if (!WriteFile(g_ArrFileOut, (WORD*)&header, header.HeadSizeBytes,
			&dTmp, NULL))
		{
			AfxMessageBox(L"error");
		}
	}
	
	// 根据通道数，重新设置视图滚动范围
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(m_hWndDigit)); 
    CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	CADFrame* pShow=((CSysApp*)AfxGetApp())->m_ADFrame ; // 取得子帧窗口句柄
	
	CProgressCtrl* pProgress=(CProgressCtrl*)(pShow->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));	
	pProgress->SetPos(0);  // 释放设备时，置进度条为起值
	

	// 启动设备
	if(!MyStartDeviceAD(pApp->m_hDevice)) // 启动设备,开始采集数据
	{
		AfxMessageBox(_T("设备启动失败"));
		return;
	}

	gl_bDeviceADRun = TRUE;
	if(pParaCfgView != NULL)
	{
		pParaCfgView->EnableWindows(FALSE); // 在设备开始运行时，禁止操作硬件参数
		pParaCfgView->StartAD();
	}
	m_DrawWindowThread->ResumeThread(); // 启动绘制窗口线程
}

BOOL gl_Event = FALSE;
//###################  绘制数据线程 ################################################3### 
UINT ProccessDataThread(PVOID hWnd)  
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADFrame *pFrm=pApp->m_ADFrame ;
	CADDoc* pDoc=pApp->m_pADDoc; 
	// 在线程中取得视图指针
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	CString str;
	BOOL bFirst=TRUE;    
	ULONG WroteMB=0;  
	
	pDoc->m_Wrote8KWCounter=0;
	pDoc->m_WroteMB=0;
	CurrentIndex=0;
	int ShowCount=0,i;
	ULONG ulTime = GetTickCount();
	while(gl_bDeviceADRun)  // 循环处理AD数据
	{ 
		do{
			if(!gl_bDeviceADRun) goto ExitProcDataThread;
		} while(WaitForSingleObject(hEvent, 100) != WAIT_OBJECT_0);
		gl_bTerminateDrawThr = FALSE;
		gl_Event = TRUE;
		switch(nProcessMode)  // 数据处理
		{
		case DATA_MODE: // 数字处理
			if ((GetTickCount()-ulTime)>100)
			{
				::SendMessage(pDoc->m_hWndDigit, WM_SHOWDIGIT, NULL, NULL);
				m_bProgress=TRUE; // 使OnDraw函数能更新进度条
				ulTime = GetTickCount();							
			}
			/*else
			{
				if (gl_bSmlThenSize)
				{
					gl_bSmlThenSize = FALSE;
				}
			}*/
			break; 
		case WAVE_MODE:  // 波形处理
			if ((GetTickCount()-ulTime)>120)
			{
				::SendMessage(pDoc->m_hWndWave,WM_SHOWWAVE, NULL, NULL);
				m_bProgress=TRUE; // 使OnDraw函数能更新进度条
				ulTime = GetTickCount();
				//Sleep(1);
			}
			break;
		case SAVE_MODE: // 数据存盘处理				
			WroteMB=(WroteMB+32)%1024;
			pFrm->m_Progress()->SetPos((int)WroteMB);					
			str.Format(_T("%d"), (FileLenghtWords/512));
			i=str.GetLength()-3;
			while(i>0)
			{
				str.Insert(i, ',');
				i-=3;
			}
			str+=" Kb";
			pFrm->m_FileLenght()->SetWindowText(str);
			ShowCount++;
			break;
		}//end switch  
		
	}//end while
ExitProcDataThread:
	gl_bTerminateDrawThr = TRUE; // 线程中止
	return TRUE;
}
//##########################################################################################

void CADDoc::Public_ReleaseDevice(void) // 供MDI窗口调用
{
	StopDeviceAD();
}

void CADDoc::StopDeviceAD() 
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
//	gl_bDeviceADRun = FALSE; // 停止子线程进行连续数据采集	
	
	CADFrame* pShow=((CSysApp*)AfxGetApp())->m_ADFrame ; // 取得子帧窗口句柄
	


//	bCreateDevice = FALSE;
//	SetEvent(hExitEvent);
//	WaitForSingleObject(hExitEvent, 100);  // 等待采集线程退出
	// 终止采集线程
	CProgressCtrl* pProgress=(CProgressCtrl*)(pShow->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));
	pProgress->SetPos(0);  // 释放设备时，置进度条为起值
	
	// 在设备对象释放后，允许操作硬件参数
	if(pParaCfgView != NULL)
	{
		pParaCfgView->EnableWindows(TRUE);	
	}
	bCreateDevice = FALSE;     // 释放设备后，视设备未创建
	

	::SendMessage(m_hWndDigit, WM_SHOWDIGIT, NULL, NULL);
	::SendMessage(m_hWndWave,WM_SHOWWAVE, NULL, NULL);
	if(!MyStopDeviceAD(pApp->m_hDevice)) // 关闭AD设备
	{
		AfxMessageBox(_T("关闭AD设备失败!"));
		return;
	}
	if(nProcessMode == SAVE_MODE)  // 如果数据处理方式为存盘
	{
		
		OnCloseFile();//释放文件
		nProcessMode = DATA_MODE; // 置成数字方式	
		if(pADStatusView != NULL)
		{
			CButton* pDataSave=(CButton*)(pADStatusView->GetDlgItem(IDM_DataSave));	
			pDataSave->EnableWindow(FALSE);  // 使存盘方式单选框有效

		}
	}
	ACTS1000_ReleaseSystemEvent(hEvent);	   // 释放消息
	ACTS1000_ReleaseSystemEvent(hExitEvent);	   // 释放消息

}


void CADDoc::OnDataFileNew() 
{
	// TODO: Add your command handler code here
	CString strNewFileName;
	CString TempFileName;
	CSysApp* pApp=(CSysApp*)AfxGetApp();
	if (!(pApp->DoPromptFileName(strNewFileName, IDS_NEW_FILE,
		OFN_HIDEREADONLY | OFN_CREATEPROMPT, TRUE, NULL)))
		return;
	if(!m_bFirstCreateDeviceFile)
	{  // 如果不是第一次创建文件对象,则释放老文件对象	
		if ( ! CloseHandle( g_ArrFileOut ) )
		{
			AfxMessageBox(L"释放文件对象失败...",MB_ICONSTOP,0);
		}
	} 
	
	// 创建新文件对象

	int FileNameLength=strNewFileName.GetLength();
	//TempFileName=strNewFileName.Left(FileNameLength-4);


	g_ArrFileOut =CreateFile(strNewFileName, GENERIC_READ|GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (g_ArrFileOut == INVALID_HANDLE_VALUE )		

	{
		AfxMessageBox(_T("初始化文件对象失败..."),MB_ICONSTOP,0);
		return;
	}	
	// 保存打开的文件名称
	g_strFileFullName = strNewFileName;

	gl_nSaveFileCount = gl_nChannelCount;


	
	CADFrame* pShow=((CSysApp*)AfxGetApp())->m_ADFrame ; // 取得子帧窗口句柄
	pShow->ShowHideDlgSave ();	//显示存盘栏
	pShow->ShowHideStatus(FALSE);//隐藏状态栏
	if(pADStatusView != NULL)
	{
		CButton* pDataSave=(CButton*)(pADStatusView->GetDlgItem(IDM_DataSave));	
		pDataSave->EnableWindow(TRUE);  // 使存盘方式单选框有效
	}
	
	_ULARGE_INTEGER dlFreeSpace, dlTotalSpace, dlUserSpace;

	GetDiskFreeSpaceEx(strNewFileName.Left(3), &dlUserSpace, &dlUserSpace, &dlFreeSpace);
	(void)dlTotalSpace;
	(void)dlUserSpace;

	ULONGLONG DiskFreeBytes = dlFreeSpace.QuadPart; 

	// 取得文件所在磁盘可用空间 */
	m_RemainMB=(ULONG)(DiskFreeBytes/(1024*1024));  // 求出有多少M字	
	CString str;
	
	if(m_RemainMB<2) 
	{
		AfxMessageBox(_T("硬盘剩余空间不足2M，存盘操作不能进行..."),MB_ICONSTOP,0);
		return;
	}
	str.Format(_T("%d Mb"),m_RemainMB);
	pShow->m_DiskFree()->SetWindowText (str);// 置当前硬盘可用空间MB
	
	pShow->m_Progress()->SetRange32(0,1024);// 设置进程范围
	pShow->m_Progress()->SetPos(0);
	
	/////////////
	pShow->m_FileName()->SetWindowText(strNewFileName);//显示存盘文件名
	m_bFirstCreateDeviceFile=FALSE;  // 若成功文件对象，则置标志	
	
	m_bCloseFile=FALSE; // 有文件没有关闭,可以操作《关闭当前文件》菜单
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
	nProcessMode = SAVE_MODE; // 当打开新文件时，意味着要进行存盘处理
	
	pADStatusView->SetDlgCheck();
}

void CADDoc::OnCloseFile() 
{
	// TODO: Add your command handler code here
	DWORD dTmp;
	
	header.HeadSizeBytes = sizeof(FILE_HEADER);
	header.ADStatus = gl_ADStatus; // gl_ADStatus.bTrigFlag;
// 		g_ArrFileOut.SeekToBegin();
// 		g_ArrFileOut.Write(&header, sizeof(FILE_HEADER));
// 		g_ArrFileOut.Close();
	SetFilePointer(g_ArrFileOut, 0, 0, FILE_BEGIN);
	//ACTS1000_WriteFile(hFileObject[nChannel], &header, sizeof(FILE_HEADER));
	if (!WriteFile(g_ArrFileOut, &header, sizeof(FILE_HEADER), &dTmp, NULL))
	{
		AfxMessageBox(L"error");
	}
	CloseHandle( g_ArrFileOut);

	CString strTitle;
	strTitle = GetTitle(); // 为防止SetPathName改变文档的Title，则提前保存
	// 添加到 曾打开的文件列表中





	m_bCloseFile=TRUE;  // 文件被关闭
	nProcessMode = DATA_MODE; // 当关闭新文件时，意味着要不能再进行存盘处理，因此置成数字方式
	m_bFirstCreateDeviceFile=TRUE; // 当新文件被关闭后，意味着以后新文件还是第一次被创建 
	///////
	CADFrame* pADFrm = ((CSysApp*)AfxGetApp())->m_ADFrame ; // 取得子帧窗口句柄
	CButton* pDataSave=(CButton*)(pADStatusView->GetDlgItem(IDM_DataSave));	
	pDataSave->EnableWindow(TRUE);  // 使存盘方式单选框有效
	
	CString str;
	str.Format(_T("%d"), 0);
	pADFrm->m_DiskFree()->SetWindowText(str);

	if (gl_bAutoCollect)
	{
//		SetPathName(g_strFileFullName[1]);
		SetPathName(g_strFileFullName); 
		SetTitle(strTitle); // 恢复文档的Title
		pADFrm->ShowHideDlgSave (FALSE); // 隐藏存盘栏
		pADFrm->ShowHideStatus();//显示状态栏
	}
	     

	pADFrm->m_Progress()->SetRange32(0,1024);  // 设置进程范围
	pADFrm->m_Progress()->SetPos(0);	
	// 一兆进度
	pADStatusView->SetDlgCheck();	
}


void CADDoc::OnTileWave() 
{
	// TODO: Add your command handler code here
	gl_bTileWave = TRUE;
	CButton* pButtonTile = (CButton*)pADStatusView->GetDlgItem(IDM_TileWave);
	pButtonTile->SetCheck(1);
	CButton* pButtonPose = (CButton*)pADStatusView->GetDlgItem(IDM_SuperPoseWave);
	pButtonPose->SetCheck(0);
    CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();	
	pWaveView->OnDrawPolyLine();
}

void CADDoc::OnSuperPoseWave() 
{
	// TODO: Add your command handler code here
	gl_bTileWave = FALSE;
	CButton* pButtonTile = (CButton*)pADStatusView->GetDlgItem(IDM_TileWave);
	pButtonTile->SetCheck(0);
	CButton* pButtonPose = (CButton*)pADStatusView->GetDlgItem(IDM_SuperPoseWave);
	pButtonPose->SetCheck(1);
    CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
	pWaveView->OnDrawPolyLine();
	
}

void CADDoc::OnUpdateSuperPoseWave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_bTileWave==TRUE ? 0:1);
}

void CADDoc::OnUpdateTileWave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_bTileWave==TRUE ? 1:0);
	
}

