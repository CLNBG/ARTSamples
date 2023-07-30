// Sys.cpp : Defines the class behaviors for the application.
//
#include "stdafx.h"
#include "DlgCalibration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CSysApp
BEGIN_MESSAGE_MAP(CSysApp, CWinApp)
//{{AFX_MSG_MAP(CSysApp)
ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
ON_UPDATE_COMMAND_UI(IDM_CloseFile, OnUpdateCloseFile)
ON_UPDATE_COMMAND_UI(IDM_DataFileNew, OnUpdateDataFileNew)
ON_COMMAND(IDM_CloseFile, OnCloseFile)
ON_COMMAND(IDM_DataFileNew, OnDataFileNew)
ON_COMMAND(IDM_StartDeviceAD, OnStartDeviceAD)
ON_COMMAND(IDM_StopDeviceAD, OnStopDeviceAD)
ON_UPDATE_COMMAND_UI(IDM_StopDeviceAD, OnUpdateStopDeviceAD)
ON_COMMAND(IDM_Open_AD, OnOpenAD)
ON_UPDATE_COMMAND_UI(IDM_Open_AD, OnUpdateOpenAD)
ON_COMMAND(IDM_OpenADHist, OnOpenADHist)
ON_UPDATE_COMMAND_UI(IDM_OpenADHist, OnUpdateADHist)
ON_UPDATE_COMMAND_UI(IDM_StartDeviceAD, OnUpdateStartDeviceAD)
ON_UPDATE_COMMAND_UI(IDM_MDataSave, OnUpdateDataSave)
ON_UPDATE_COMMAND_UI(IDM_MDigitalShow, OnUpdateDigitalShow)
ON_UPDATE_COMMAND_UI(IDM_MGraphicShow, OnUpdateGraphicShow)
ON_COMMAND(IDM_MDataSave, OnMDataSave)
ON_COMMAND(IDM_MDigitalShow, OnMDigitalShow)
ON_COMMAND(IDM_MGraphicShow, OnMGraphicShow)
ON_COMMAND(IDM_Dma_Mode, OnDmaMode)
ON_UPDATE_COMMAND_UI(IDM_Dma_Mode, OnUpdateDmaMode)
ON_COMMAND(IDM_Npt_Mode, OnNptMode)
ON_UPDATE_COMMAND_UI(IDM_Npt_Mode, OnUpdateNptMode)
ON_COMMAND(IDM_Half_Mode, OnHalfMode)
ON_UPDATE_COMMAND_UI(IDM_Half_Mode, OnUpdateHalfMode)
	ON_COMMAND(IDM_Open_DIO, OnOpenDIO)
	ON_COMMAND(IDM_ADCalibration, OnADCalibration)
	//}}AFX_MSG_MAP
// Standard file based document commands
ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
// Standard print setup command
ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysApp construction

CSysApp::CSysApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_bCreateADFrm = FALSE;
	m_bCreateDAFrm = FALSE;
	m_hDevice = INVALID_HANDLE_VALUE;
	m_ADFrm = NULL;
	m_pDIOFrm = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSysApp object

CSysApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CSysApp initialization

BOOL CSysApp::InitInstance()
{
	TCHAR szMutex[256];
	m_CurrentDeviceID = 0;   // 指定当前设备的ID标示符

	int DeviceID = 0;
	while (TRUE)
	{
		swprintf_s(szMutex,_T("PCI8510-%d"), DeviceID);

		// 创建互斥对象
		m_hMutex=::CreateMutex(NULL, NULL, szMutex);  // m_pszExeName为本程序的执行名
		if (GetLastError() == ERROR_ALREADY_EXISTS)      // 第二次创建应用程序
		{
			DeviceID++;
			continue;  // 如果已经创建，则继续下一个设备的应用程序创建
		}
		else
		{ 	
			m_CurrentDeviceID = DeviceID;
			m_hDevice = PCI8510_CreateDevice(m_CurrentDeviceID);
			if (m_hDevice == INVALID_HANDLE_VALUE)
			{			
				AfxMessageBox(_T("对不起，没有设备或您的所有设备已被相应程序管理，不能再创建新实例..."), MB_ICONWARNING, 0);
			}
			break;
		}
	}

	// 判断用户的显示器模式是否为1024*768
	int Len = GetSystemMetrics(SM_CXSCREEN);  // 取得屏幕宽度
	if (Len < 1024) // 如果屏幕宽度大小1024，则
	{
		if (AfxMessageBox(_T("请最好使用1024*768或以上的显示器分辨率，继续吗？"), MB_ICONWARNING | MB_YESNO, 0) == IDNO)	
		{
			ExitInstance();
			return FALSE;
		}
	}
	
	// 应用程序正常创建代码：
	AfxEnableControlContainer();
	
// #ifdef _AFXDLL
// 	Enable3dControls();			// Call this when using MFC in a shared DLL
// #else
// 	Enable3dControlsStatic();	// Call this when linking to MFC statically
// #endif
	
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	
	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)
	
	// 添加AD采集模板
	pADTemplate = new CMultiDocTemplate(
		IDR_AD,
		RUNTIME_CLASS(CADDoc),
		RUNTIME_CLASS(CADFrm), // custom MDI child frame
		RUNTIME_CLASS(CADDigitView));
	AddDocTemplate(pADTemplate);
	
	// 添加历史数据回放模板
	pHistDataTemplate = new CMultiDocTemplate(
		IDR_ADHist,
		RUNTIME_CLASS(CADHistDoc),
		RUNTIME_CLASS(CADHistFrm), // custom MDI child frame
		RUNTIME_CLASS(CADHistDigitView));
	AddDocTemplate(pHistDataTemplate);

	// 添加开关量模板
	pDIOTemplate = new CMultiDocTemplate(
		IDR_DIO,
		RUNTIME_CLASS(CDIODoc),
		RUNTIME_CLASS(CDIOFrm), // custom MDI child frame
		RUNTIME_CLASS(CDIOView));
	AddDocTemplate(pDIOTemplate);
	
	// Enable DDE Execute open
	// 让WINDOWS登记该程序的缺省数据文件名扩展名
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	m_pMainWnd = pMainFrame;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED); // 使主窗口最大化
	pMainFrame->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles(); // 支持拖放功能
	::SetProp(m_pMainWnd->GetSafeHwnd(), szMutex, (HANDLE)1);	


	LONG DeviceLgcID, DevicePhysID=0;
	if (m_hDevice != INVALID_HANDLE_VALUE)
	{
		PCI8510_GetDeviceCurrentID(m_hDevice, &DeviceLgcID, &DevicePhysID);
	}

	CString MainFrmName; 
	TCHAR str[100];
	swprintf_s(str,_T("PCI8510-%d-%d "), m_CurrentDeviceID,DevicePhysID);
	MainFrmName = pMainFrame->GetTitle();
	MainFrmName = str + MainFrmName;
	pMainFrame->SetTitle(MainFrmName);	

	OnOpenAD();		// 打开AD采集模板
	m_ADFrm->BringWindowToTop();	// 将AD模板置于屏幕顶端
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CSysApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSysApp message handlers
void CSysApp::OnUpdateOpenAD(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	//	pCmdUI->Enable(this->m_bCreateADFrm==FALSE?1:0);
}

void CSysApp::OnOpenAD(void)
{
	if (m_ADFrm != NULL)
	{
		m_ADFrm->BringWindowToTop();
		return;
	}

	// 取得操作系统版本
	DWORD	dwVersion = GetVersion();
	// Get the Windows version.
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	// Get the build number.
	gl_nSampleMode = DMA_MODE;		// 非空方式
	
	BeginWaitCursor();  // 开始漏斗鼠标
	CDocument* pDoc;
	pDoc = pADTemplate->CreateNewDocument();  // 创建新文档
	m_pADDoc = (CADDoc*)pDoc;  // 保存文档对象实例在App类成员中

	// 创建该文档对象的子帧窗口
	m_ADFrm = (CADFrm*)pADTemplate->CreateNewFrame(pDoc, NULL);
	
	// 初始化FRAME子帧窗口，并创建文档实例, 且触发FRAME的OnUpdate函数
  	pADTemplate->InitialUpdateFrame(m_ADFrm, pDoc);
	// 设置默认的文档标题
	pDoc->SetTitle(_T("AD数采测试"));

	if (gl_pADStatusView != NULL)
	{
		// 将参数配置视图类加入文档列表，以便跟正常视图一样被文档类管理
		pDoc->AddView((CView*)gl_pADStatusView); 
	}

	if (gl_pParaCfgView != NULL)
	{
		// 将参数配置视图类加入文档列表，以便跟正常视图一样被文档类管理
		pDoc->AddView((CView*)gl_pParaCfgView);
	}

	m_bCreateADFrm = TRUE;
	EndWaitCursor(); // 停止漏斗鼠标
}

// 打开历史文档
void CSysApp::OnOpenADHist() 
{

	BeginWaitCursor();  // 开始漏斗鼠标
	CFileOpenDlg FileOpenDlg;
	CFile m_File;
	CString strFileName;
	// 弹出文件对话框
	if (!(DoPromptFileName(strFileName, IDS_ADHist,
		OFN_HIDEREADONLY, TRUE, NULL))) 
		return;	

	TCHAR* pFileName = strFileName.GetBuffer(sizeof(strFileName)); // 类型转换
	if (!m_File.Open(pFileName, PCI8510_modeRead)) // 打开文件
	{
		AfxMessageBox(_T("文件打开错误或无此文件!"));
		return;
	}
	m_File.Close();	

	m_strFilePath = strFileName;
	m_pADHistDoc = (CADHistDoc*)pHistDataTemplate->OpenDocumentFile(strFileName);
	EndWaitCursor(); // 停止漏斗鼠标
}

void CSysApp::OnUpdateADHist(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

// 从最近打开的文件中打开文件
CDocument* CSysApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{	
	BeginWaitCursor();  // 开始漏斗鼠标
	CFileOpenDlg FileOpenDlg;
	CFile File;
	int nRet = 0;
	BOOL bFileAvailable = TRUE;
	FILE_HEADER FileHeader;
	if (File.Open(lpszFileName, PCI8510_modeRead)) // 如果文件打开成功
	{
		File.Seek(0, CFile::begin);
		File.Read((WORD*)&FileHeader, sizeof(FileHeader)); // 读取文件头
		File.Close();
		if ((FileHeader.DeviceNum) != DEFAULT_DEVICE_NUM)
		{
			memset(&(FileHeader.ADPara), 0x00, sizeof(FileHeader.ADPara));

			if (AfxMessageBox(_T("此文件并非PCI8510板卡配套文件, 是否继续?"), MB_OKCANCEL) == IDCANCEL)
				return NULL;
			bFileAvailable = FALSE; // 文件无效
			
		}
		
		m_pADHistDoc = (CADHistDoc*)pHistDataTemplate->OpenDocumentFile(lpszFileName); // 为NULL时不引发CHistDataFrm::OpenDocumentFile

		return m_pADHistDoc; // 返回文档指针
	}
	else
	{
		AfxMessageBox(_T("文件打开错误或无此文件!"));
		return NULL;
	}

	EndWaitCursor(); // 停止漏斗鼠标
	return NULL;
}

int CSysApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	ReleaseMutex(m_hMutex);

	if(m_hDevice != INVALID_HANDLE_VALUE)
	{
		PCI8510_ReleaseDevice(m_hDevice ); // 释放设备对象
		m_hDevice = INVALID_HANDLE_VALUE;
	}

	if(gl_hEvent != INVALID_HANDLE_VALUE)
	{
		PCI8510_ReleaseSystemEvent(gl_hEvent);
		gl_hEvent = INVALID_HANDLE_VALUE;
	}
	return CWinApp::ExitInstance();
}


void CSysApp::OnUpdateDataFileNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((!gl_bDeviceADRun)&&gl_bCreateFile);
	
}

void CSysApp::OnCloseFile() 
{
	// TODO: Add your command handler code here
	m_pADDoc->OnCloseFile();
}

void CSysApp::OnDataFileNew() 
{
	// TODO: Add your command handler code here
	m_pADDoc->OnDataFileNew();
}

void CSysApp::OnStartDeviceAD() 
{
	// TODO: Add your command handler code here
	m_pADDoc->StartDeviceAD();
}

void CSysApp::OnUpdateStartDeviceAD(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(gl_bDeviceADRun==FALSE?1:0);	
}

void CSysApp::OnStopDeviceAD() 
{
	// TODO: Add your command handler code here
	m_pADDoc->StopDeviceAD();
}

void CSysApp::OnUpdateStopDeviceAD(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(gl_bDeviceADRun == TRUE ? 1:0);		
}

void CSysApp::OnUpdateCloseFile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (FALSE); 
	if ((gl_nProcMode == PROC_MODE_SAVE) && (gl_bCreateDevice))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(!gl_bCloseFile);
	
}

void CSysApp::OnUpdateDataSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (gl_pADStatusView != NULL)
		gl_pADStatusView->SetDlgCheck();

	if (gl_bCreateFile)
		pCmdUI->Enable(FALSE);
	
	pCmdUI->SetCheck(gl_nProcMode == PROC_MODE_SAVE ? 1:0);	
}

void CSysApp::OnUpdateDigitalShow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_nProcMode == PROC_MODE_DIGIT ? 1:0);
	gl_pADStatusView->SetDlgCheck();
	
    CADDoc* pDoc = theApp.m_pADDoc; 
    // 在线程中取得视图指针
    CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	if (gl_nProcMode == PROC_MODE_SAVE) // 如果为存盘处理方式
	{
		pDigitView->EnableWindow(FALSE);
	}
	else
	{
		pDigitView->EnableWindow(TRUE);
	}		
}

void CSysApp::OnUpdateGraphicShow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	gl_pADStatusView->SetDlgCheck();
	pCmdUI->SetCheck(gl_nProcMode == PROC_MODE_WAVE ? 1:0);
    CADDoc* pDoc = theApp.m_pADDoc;
	
    // 在线程中取得视图指针	
    CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	
	if (gl_nProcMode == PROC_MODE_SAVE) // 如果为存盘处理方式
	{
		pWaveView->EnableWindow(FALSE);
	}
	else
	{
		pWaveView->EnableWindow(TRUE);
	}	
	
}

void CSysApp::OnMDataSave() 
{
	// TODO: Add your command handler code here
	gl_nProcMode = PROC_MODE_SAVE;
	gl_pADStatusView->SetDlgCheck();
}

void CSysApp::OnMDigitalShow() 
{
	// TODO: Add your command handler code here
	gl_nProcMode = PROC_MODE_DIGIT;
	gl_pADStatusView->SetDlgCheck();
}

void CSysApp::OnMGraphicShow() 
{
	// TODO: Add your command handler code here
	gl_nProcMode = PROC_MODE_WAVE;
	gl_pADStatusView->SetDlgCheck();
}


void CSysApp::OnDmaMode() 
{
	// TODO: Add your command handler code here
	gl_nSampleMode = DMA_MODE; // DMA方式
}

void CSysApp::OnUpdateDmaMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_nSampleMode == DMA_MODE ? 1:0);
	pCmdUI->Enable(!gl_bDeviceADRun);
}

void CSysApp::OnNptMode() 
{
	// TODO: Add your command handler code here
	gl_nSampleMode = NPT_MODE; // 非空方式
}

void CSysApp::OnUpdateNptMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_nSampleMode == NPT_MODE ? 1:0);
	pCmdUI->Enable(!gl_bDeviceADRun);	
}

void CSysApp::OnHalfMode() 
{
	// TODO: Add your command handler code here
	gl_nSampleMode = HALF_MODE; // 非空方式
}

void CSysApp::OnUpdateHalfMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_nSampleMode == HALF_MODE ? 1:0);
	pCmdUI->Enable(!gl_bDeviceADRun);	
}


void CSysApp::OnOpenDIO() 
{
	// TODO: Add your command handler code here
	
	if (m_pDIOFrm != NULL)
	{
		m_pDIOFrm->BringWindowToTop();
		return;
	}
	BeginWaitCursor();  // 开始漏斗鼠标
	CDocument* pDoc;
	pDoc=pDIOTemplate->CreateNewDocument();  // 创建新文档
	m_pDIODoc=(CDIODoc*)pDoc;  // 保存文档对象
	// 创建该文档对象的子帧窗口
	m_pDIOFrm=(CDIOFrm*)pDIOTemplate->CreateNewFrame(pDoc, NULL);
	// 设置默认的文档标题
	// 创建文档实例
	// 初始化FRAME子帧窗口，且触发FRAME的OnUpdate函数
	pDIOTemplate->InitialUpdateFrame(m_pDIOFrm, pDoc);
	pDoc->SetTitle(_T("开关量测试"));
	
	EndWaitCursor(); // 停止漏斗鼠标

}

void CSysApp::OnADCalibration() 
{
	// TODO: Add your command handler code here
	
	if (m_hDevice != INVALID_HANDLE_VALUE)
	{
		CDlgCalibration dlg;
		dlg.DoModal();
	}
}
