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
	m_CurrentDeviceID = 0;   // ָ����ǰ�豸��ID��ʾ��

	int DeviceID = 0;
	while (TRUE)
	{
		swprintf_s(szMutex,_T("PCI8510-%d"), DeviceID);

		// �����������
		m_hMutex=::CreateMutex(NULL, NULL, szMutex);  // m_pszExeNameΪ�������ִ����
		if (GetLastError() == ERROR_ALREADY_EXISTS)      // �ڶ��δ���Ӧ�ó���
		{
			DeviceID++;
			continue;  // ����Ѿ��������������һ���豸��Ӧ�ó��򴴽�
		}
		else
		{ 	
			m_CurrentDeviceID = DeviceID;
			m_hDevice = PCI8510_CreateDevice(m_CurrentDeviceID);
			if (m_hDevice == INVALID_HANDLE_VALUE)
			{			
				AfxMessageBox(_T("�Բ���û���豸�����������豸�ѱ���Ӧ������������ٴ�����ʵ��..."), MB_ICONWARNING, 0);
			}
			break;
		}
	}

	// �ж��û�����ʾ��ģʽ�Ƿ�Ϊ1024*768
	int Len = GetSystemMetrics(SM_CXSCREEN);  // ȡ����Ļ���
	if (Len < 1024) // �����Ļ��ȴ�С1024����
	{
		if (AfxMessageBox(_T("�����ʹ��1024*768�����ϵ���ʾ���ֱ��ʣ�������"), MB_ICONWARNING | MB_YESNO, 0) == IDNO)	
		{
			ExitInstance();
			return FALSE;
		}
	}
	
	// Ӧ�ó��������������룺
	AfxEnableControlContainer();
	
// #ifdef _AFXDLL
// 	Enable3dControls();			// Call this when using MFC in a shared DLL
// #else
// 	Enable3dControlsStatic();	// Call this when linking to MFC statically
// #endif
	
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	
	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)
	
	// ���AD�ɼ�ģ��
	pADTemplate = new CMultiDocTemplate(
		IDR_AD,
		RUNTIME_CLASS(CADDoc),
		RUNTIME_CLASS(CADFrm), // custom MDI child frame
		RUNTIME_CLASS(CADDigitView));
	AddDocTemplate(pADTemplate);
	
	// �����ʷ���ݻط�ģ��
	pHistDataTemplate = new CMultiDocTemplate(
		IDR_ADHist,
		RUNTIME_CLASS(CADHistDoc),
		RUNTIME_CLASS(CADHistFrm), // custom MDI child frame
		RUNTIME_CLASS(CADHistDigitView));
	AddDocTemplate(pHistDataTemplate);

	// ��ӿ�����ģ��
	pDIOTemplate = new CMultiDocTemplate(
		IDR_DIO,
		RUNTIME_CLASS(CDIODoc),
		RUNTIME_CLASS(CDIOFrm), // custom MDI child frame
		RUNTIME_CLASS(CDIOView));
	AddDocTemplate(pDIOTemplate);
	
	// Enable DDE Execute open
	// ��WINDOWS�ǼǸó����ȱʡ�����ļ�����չ��
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
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED); // ʹ���������
	pMainFrame->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles(); // ֧���ϷŹ���
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

	OnOpenAD();		// ��AD�ɼ�ģ��
	m_ADFrm->BringWindowToTop();	// ��ADģ��������Ļ����
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

	// ȡ�ò���ϵͳ�汾
	DWORD	dwVersion = GetVersion();
	// Get the Windows version.
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	// Get the build number.
	gl_nSampleMode = DMA_MODE;		// �ǿշ�ʽ
	
	BeginWaitCursor();  // ��ʼ©�����
	CDocument* pDoc;
	pDoc = pADTemplate->CreateNewDocument();  // �������ĵ�
	m_pADDoc = (CADDoc*)pDoc;  // �����ĵ�����ʵ����App���Ա��

	// �������ĵ��������֡����
	m_ADFrm = (CADFrm*)pADTemplate->CreateNewFrame(pDoc, NULL);
	
	// ��ʼ��FRAME��֡���ڣ��������ĵ�ʵ��, �Ҵ���FRAME��OnUpdate����
  	pADTemplate->InitialUpdateFrame(m_ADFrm, pDoc);
	// ����Ĭ�ϵ��ĵ�����
	pDoc->SetTitle(_T("AD���ɲ���"));

	if (gl_pADStatusView != NULL)
	{
		// ������������ͼ������ĵ��б��Ա��������ͼһ�����ĵ������
		pDoc->AddView((CView*)gl_pADStatusView); 
	}

	if (gl_pParaCfgView != NULL)
	{
		// ������������ͼ������ĵ��б��Ա��������ͼһ�����ĵ������
		pDoc->AddView((CView*)gl_pParaCfgView);
	}

	m_bCreateADFrm = TRUE;
	EndWaitCursor(); // ֹͣ©�����
}

// ����ʷ�ĵ�
void CSysApp::OnOpenADHist() 
{

	BeginWaitCursor();  // ��ʼ©�����
	CFileOpenDlg FileOpenDlg;
	CFile m_File;
	CString strFileName;
	// �����ļ��Ի���
	if (!(DoPromptFileName(strFileName, IDS_ADHist,
		OFN_HIDEREADONLY, TRUE, NULL))) 
		return;	

	TCHAR* pFileName = strFileName.GetBuffer(sizeof(strFileName)); // ����ת��
	if (!m_File.Open(pFileName, PCI8510_modeRead)) // ���ļ�
	{
		AfxMessageBox(_T("�ļ��򿪴�����޴��ļ�!"));
		return;
	}
	m_File.Close();	

	m_strFilePath = strFileName;
	m_pADHistDoc = (CADHistDoc*)pHistDataTemplate->OpenDocumentFile(strFileName);
	EndWaitCursor(); // ֹͣ©�����
}

void CSysApp::OnUpdateADHist(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

// ������򿪵��ļ��д��ļ�
CDocument* CSysApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{	
	BeginWaitCursor();  // ��ʼ©�����
	CFileOpenDlg FileOpenDlg;
	CFile File;
	int nRet = 0;
	BOOL bFileAvailable = TRUE;
	FILE_HEADER FileHeader;
	if (File.Open(lpszFileName, PCI8510_modeRead)) // ����ļ��򿪳ɹ�
	{
		File.Seek(0, CFile::begin);
		File.Read((WORD*)&FileHeader, sizeof(FileHeader)); // ��ȡ�ļ�ͷ
		File.Close();
		if ((FileHeader.DeviceNum) != DEFAULT_DEVICE_NUM)
		{
			memset(&(FileHeader.ADPara), 0x00, sizeof(FileHeader.ADPara));

			if (AfxMessageBox(_T("���ļ�����PCI8510�忨�����ļ�, �Ƿ����?"), MB_OKCANCEL) == IDCANCEL)
				return NULL;
			bFileAvailable = FALSE; // �ļ���Ч
			
		}
		
		m_pADHistDoc = (CADHistDoc*)pHistDataTemplate->OpenDocumentFile(lpszFileName); // ΪNULLʱ������CHistDataFrm::OpenDocumentFile

		return m_pADHistDoc; // �����ĵ�ָ��
	}
	else
	{
		AfxMessageBox(_T("�ļ��򿪴�����޴��ļ�!"));
		return NULL;
	}

	EndWaitCursor(); // ֹͣ©�����
	return NULL;
}

int CSysApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	ReleaseMutex(m_hMutex);

	if(m_hDevice != INVALID_HANDLE_VALUE)
	{
		PCI8510_ReleaseDevice(m_hDevice ); // �ͷ��豸����
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
    // ���߳���ȡ����ͼָ��
    CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	if (gl_nProcMode == PROC_MODE_SAVE) // ���Ϊ���̴���ʽ
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
	
    // ���߳���ȡ����ͼָ��	
    CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	
	if (gl_nProcMode == PROC_MODE_SAVE) // ���Ϊ���̴���ʽ
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
	gl_nSampleMode = DMA_MODE; // DMA��ʽ
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
	gl_nSampleMode = NPT_MODE; // �ǿշ�ʽ
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
	gl_nSampleMode = HALF_MODE; // �ǿշ�ʽ
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
	BeginWaitCursor();  // ��ʼ©�����
	CDocument* pDoc;
	pDoc=pDIOTemplate->CreateNewDocument();  // �������ĵ�
	m_pDIODoc=(CDIODoc*)pDoc;  // �����ĵ�����
	// �������ĵ��������֡����
	m_pDIOFrm=(CDIOFrm*)pDIOTemplate->CreateNewFrame(pDoc, NULL);
	// ����Ĭ�ϵ��ĵ�����
	// �����ĵ�ʵ��
	// ��ʼ��FRAME��֡���ڣ��Ҵ���FRAME��OnUpdate����
	pDIOTemplate->InitialUpdateFrame(m_pDIOFrm, pDoc);
	pDoc->SetTitle(_T("����������"));
	
	EndWaitCursor(); // ֹͣ©�����

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
