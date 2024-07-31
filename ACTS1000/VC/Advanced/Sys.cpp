// Sys.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Sys.h"
#include "MainFrm.h"
#include "ADFrm.h"
#include "ADDoc.h"
#include "ADDigitView.h"
#include "ADWaveView.h"
#include "ADHistFrm.h"
#include "ADHistDoc.h"
#include "ADHistDigitView.h"
#include "ADCalibration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
#include "ADStatusView.h"
#include "ParaCfgView.h"
extern CADStatusView* pADStatusView;
extern CParaCfgView* pParaCfgView;
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
	ON_COMMAND(IDM_HistoryData, OnHistoryData)
	ON_UPDATE_COMMAND_UI(IDM_HistoryData, OnUpdateHistoryData)
	ON_UPDATE_COMMAND_UI(IDM_Open_DA, OnUpdateOpenDA)
	ON_UPDATE_COMMAND_UI(IDM_StartDeviceAD, OnUpdateStartDeviceAD)
	ON_UPDATE_COMMAND_UI(IDM_MDataSave, OnUpdateDataSave)
	ON_UPDATE_COMMAND_UI(IDM_MDigitalShow, OnUpdateDigitalShow)
	ON_UPDATE_COMMAND_UI(IDM_MGraphicShow, OnUpdateGraphicShow)
	ON_COMMAND(IDM_MDataSave, OnMDataSave)
	ON_COMMAND(IDM_MDigitalShow, OnMDigitalShow)
	ON_COMMAND(IDM_MGraphicShow, OnMGraphicShow)
	ON_COMMAND(IDM_ListDeviceDlg, OnListDeviceDlg)
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
	m_ADFrame = NULL;
	m_bDeviceCount = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSysApp object

CSysApp theApp;
ULONGLONG gl_ReadSizeWords;	// ��������ݳ���
ULONGLONG gl_DDR2Lenth;
ULONGLONG gl_pulDDR2Length;

/////////////////////////////////////////////////////////////////////////////
// CSysApp initialization

BOOL CSysApp::InitInstance()
{

	for(int i=0; i<MAX_SEGMENT; i++)
	{
		ADBuffer[i] = (USHORT*)ACTS1000_AllocateBuffer(MAX_RAM_SIZE*sizeof(USHORT));
	}

	ADDrawBuffer = (USHORT*)ACTS1000_AllocateBuffer(MAX_RAM_SIZE*sizeof(USHORT));

	//CString strMutex;
	TCHAR szMutex[256];
	BOOL bCreate=FALSE;
	//HANDLE hDevice;
	//m_hDevice = INVALID_HANDLE_VALUE; // �ڸ�Ӧ�ó���ʹ�õ�ȫ���豸����δ������ʱ��Ӧ�ó�ֵ����Ϊ�豸��Ч
	m_CurrentDeviceID=0;   // ָ����ǰ�豸��ID��ʾ��
	/////
	int DeviceID = 0;
	while (TRUE)
	{
		swprintf_s(szMutex, L"ACTS1000-%d", DeviceID);

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
			m_hDevice = ACTS1000_CreateDevice(m_CurrentDeviceID);
			if (m_hDevice == INVALID_HANDLE_VALUE)
			{			
				AfxMessageBox(L"�Բ������������豸�ѱ���Ӧ����������������ٴ�����ʵ��,���豸������", MB_ICONWARNING, 0);
			}
			break;
		}
	}

	
	/*int DeviceCount;
	hDevice = ACTS1000_CreateDevice(0); // �����豸���󣬱�����App�У��ɹ������κ��Ӵ�����ӹ���ʹ��
	if(hDevice!=INVALID_HANDLE_VALUE) // ����豸�����Ч
	{
		DeviceCount = ACTS1000_GetDeviceCount(hDevice);
		ACTS1000_ReleaseDevice(hDevice);	
	}
	else
	{
		DeviceCount = 0; // ���û�д����ɹ�������Ϊ�豸����Ϊ0
	}

	for(int i=0; i<DeviceCount; i++)
	{
		strMutex.Format(_T("ACTS1000-%d"), i);
		
		// �����������
		m_hMutex=::CreateMutex(NULL, NULL, strMutex);  // m_pszExeNameΪ�������ִ����
		if(GetLastError()==ERROR_ALREADY_EXISTS)  // �ڶ��δ���Ӧ�ó���
		{
			bCreate=FALSE;
			continue;  // ����Ѿ��������������һ���豸��Ӧ�ó��򴴽�
		}
		else
		{ 	
			bCreate=TRUE;
			m_CurrentDeviceID=i;
			break;
		}
		
	}	
	if(DeviceCount!=0)
	{
		if(bCreate==FALSE)  // ����ʵ�����ܱ�����ʱ
		{
			AfxMessageBox(_T("�Բ������������豸�ѱ���Ӧ����������������ٴ�����ʵ��..."),MB_ICONWARNING,0);
			return FALSE; 
		}
		m_hDevice = ACTS1000_CreateDevice(m_CurrentDeviceID);
	}
	*/
	ACTS1000_GetMainInfo(m_hDevice,&gl_ADMainInfo);			// DDR2�ĳ���(��λ��MB)
	gl_pulDDR2Length = gl_ADMainInfo.nDepthOfMemory;
	gl_DDR2Lenth = gl_pulDDR2Length*1024*1024;


	m_nRevisionID = 0;
	if(!ACTS1000_GetBusInfo(m_hDevice, NULL, NULL, NULL, &m_nRevisionID))
	{
		//AfxMessageBox("����豸������Ϣʧ��");
		//return;
	}
	
	ACTS1000_GetSupportRangeInfo(m_hDevice, &m_nSampRangeCount,m_nSampRangeVal);
	
	
	/*ULONG nInx = 0;
	for (int i=0; i<m_nSampRangeCount; i++)
	{
		m_nSampRangeVal[i]= i;
	}*/

	gl_wMaxLSB = (WORD)(gl_ADMainInfo.nSampCodeCount - 1);
	gl_fLsbCount = (float)gl_ADMainInfo.nSampCodeCount;
	gl_lLsbHalf = (WORD)(gl_ADMainInfo.nSampCodeCount/2);

	///////////////////////////////////////////////
	// �ж��û�����ʾ��ģʽ�Ƿ�Ϊ1024*768
	int Len=GetSystemMetrics(SM_CXSCREEN);  // ȡ����Ļ����
	if(Len<1024) // �����Ļ���ȴ�С1024����
	{
		if(AfxMessageBox(_T("�����ʹ��1024*768�����ϵ���ʾ���ֱ��ʣ�������"),MB_ICONWARNING|MB_YESNO,0)==IDNO)	
		{
			ExitInstance();
			return FALSE;
		}
	}
	
///////////////////////////////////////////////
	
// Ӧ�ó��������������룺
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings(16);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.
	
	// ����AD�ɼ�ģ��
	pADTemplate = new CMultiDocTemplate(
		IDR_AD,
		RUNTIME_CLASS(CADDoc),
		RUNTIME_CLASS(CADFrame), // custom MDI child frame
		RUNTIME_CLASS(CADDigitView));
	AddDocTemplate(pADTemplate);
	
	// ������ʷ���ݻط�ģ��
	pHistDataTemplate = new CMultiDocTemplate(
		IDR_HistoryData,
		RUNTIME_CLASS(CADHistDoc),
		RUNTIME_CLASS(CADHistFrm), // custom MDI child frame
		RUNTIME_CLASS(CADHistDigitView));
	AddDocTemplate(pHistDataTemplate);


	// Enable DDE Execute open
	// ��WINDOWS�ǼǸó����ȱʡ�����ļ�����չ��
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line
	//if (!ProcessShellCommand(cmdInfo))
	//	return FALSE;


	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	
	
	m_pMainWnd = pMainFrame;
	
	
	// The main window has been initialized, so show and update it.
	// pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);  // ʹ���������
	
	pMainFrame->UpdateWindow();
	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();  // ֧���ϷŹ���

	::SetProp(m_pMainWnd->GetSafeHwnd(), m_pszExeName, (HANDLE)1);	

	LONG nPhyID = 0;
	if (m_hDevice != INVALID_HANDLE_VALUE)
	{
		ACTS1000_GetDeviceCurrentID(m_hDevice, NULL, &nPhyID);
	}
	CString MainFrmName; TCHAR str[100];
	switch (gl_ADMainInfo.nDeviceType>>16)
	{
	case PXIE_BUS:
		if (m_nRevisionID >= PCIE8502D_RID && m_nRevisionID <0xFF)
		{
			_stprintf(str, _T("ACTS1000-PXIE%04X(%02X)-%d-%d[%dM] "), gl_ADMainInfo.nDeviceType&0xFFFF, m_nRevisionID, m_CurrentDeviceID,nPhyID, gl_pulDDR2Length);
		}
		else if (m_nRevisionID == 0)
			_stprintf(str, _T("ACTS1000-PXIE%04X-%d-%d[%dM] "), gl_ADMainInfo.nDeviceType&0xFFFF, m_CurrentDeviceID, nPhyID, gl_pulDDR2Length);
		else
		_stprintf(str, _T("ACTS1000-PXIE%04X(%C)-%d-%d[%dM] "), gl_ADMainInfo.nDeviceType&0xFFFF,'A'+ m_nRevisionID-1, m_CurrentDeviceID,nPhyID, gl_pulDDR2Length);



		/*if (m_nRevisionID >= PCIE8502D_RID && m_nRevisionID <0xFF)
		{
			_stprintf(str, _T("ACTS1000-PXIE%04X(%02X)-%d-%d[%dM] "), gl_ADMainInfo.nDeviceType&0xFFFF, m_nRevisionID, m_CurrentDeviceID,nPhyID, gl_pulDDR2Length);
		}
		else if (m_nRevisionID == 0x01)
		{
			_stprintf(str, _T("ACTS1000-PXIE%04X(A)-%d-%d[%dM] "), gl_ADMainInfo.nDeviceType&0xFFFF, m_CurrentDeviceID,nPhyID, gl_pulDDR2Length);
		}
		else
		{
			_stprintf(str, _T("ACTS1000-PXIE%04X-%d-%d[%dM] "), gl_ADMainInfo.nDeviceType&0xFFFF, m_CurrentDeviceID, nPhyID, gl_pulDDR2Length);
		}*/
		break;
	case PCIE_BUS:
		if (m_nRevisionID >= PCIE8502D_RID && m_nRevisionID <0xFF)
		{
			_stprintf(str, _T("ACTS1000-PCIE%04X(%02X)-%d-%d[%dM] "), gl_ADMainInfo.nDeviceType&0xFFFF, m_nRevisionID, m_CurrentDeviceID,nPhyID, gl_pulDDR2Length);
		}
		else if (m_nRevisionID == 0)
				_stprintf(str, _T("ACTS1000-PCIE%04X-%d-%d[%dM] "), gl_ADMainInfo.nDeviceType&0xFFFF, m_CurrentDeviceID, nPhyID, gl_pulDDR2Length);
		else
			_stprintf(str, _T("ACTS1000-PCIE%04X(%C)-%d-%d[%dM] "), gl_ADMainInfo.nDeviceType&0xFFFF,'A'+ m_nRevisionID-1, m_CurrentDeviceID,nPhyID, gl_pulDDR2Length);


		/*if (m_nRevisionID >= PCIE8502D_RID && m_nRevisionID <0xFF)
		{
			_stprintf(str, _T("ACTS1000-PCIE%04X(%02X)-%d-%d[%dM] "), gl_ADMainInfo.nDeviceType&0xFFFF, m_nRevisionID, m_CurrentDeviceID,nPhyID, gl_pulDDR2Length);
		}
		else if (m_nRevisionID == 0x01)
		{
			_stprintf(str, _T("ACTS1000-PCIE%04X(A)-%d-%d[%dM] "), gl_ADMainInfo.nDeviceType&0xFFFF, m_CurrentDeviceID,nPhyID, gl_pulDDR2Length);
		}
		else
		{
			_stprintf(str, _T("ACTS1000-PCIE%04X-%d-%d[%dM] "), gl_ADMainInfo.nDeviceType&0xFFFF, m_CurrentDeviceID,nPhyID, gl_pulDDR2Length);
		}*/
		break;
	default:
		_stprintf(str, _T("ACTS1000-%04X-%d-[%dM] "), gl_ADMainInfo.nDeviceType, m_CurrentDeviceID,gl_pulDDR2Length);
	}


	MainFrmName = pMainFrame->GetTitle();
	MainFrmName = str+MainFrmName;
	pMainFrame->SetTitle(MainFrmName);

	OnOpenAD();		// ��AD�豸
	m_ADFrame->BringWindowToTop();		// ��ADģ��������Ļ����

	// ȡ�ò���ϵͳ�汾
	DWORD	dwVersion = GetVersion();
	// Get the Windows version.
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
	// Get the build number.
	if ((dwVersion < 0x80000000) || (dwWindowsMajorVersion < 4))              // Windows NT
		gl_nSampleMode = DMA_MODE;		// DMA��ʽ
	else                                     // Windows Me/98/95
		gl_nSampleMode = PRO_MODE;		// ��ѯ��ʽ

	
		gl_nSampleMode = PRO_MODE;		// ��ѯ��ʽ
	// CG: This line inserted by 'Tip of the Day' component.
 

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
public:
	afx_msg void OnBnClickedButtonSet();
public:
	virtual BOOL OnInitDialog();
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
	ON_BN_CLICKED(IDC_BUTTON_Set, &CAboutDlg::OnBnClickedButtonSet)
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
	pCmdUI->Enable(this->m_bCreateADFrm==FALSE?1:0);
}

void CSysApp::OnOpenAD(void)
{
	if (m_ADFrame != NULL)
	{
		m_ADFrame->BringWindowToTop();
		return;
	}
	BeginWaitCursor();  // ��ʼ©�����
	CDocument* pDoc;
	pDoc=pADTemplate->CreateNewDocument();  // �������ĵ�
	m_pADDoc=(CADDoc*)pDoc;  // �����ĵ�����
	// �������ĵ��������֡����
	m_ADFrame=(CADFrame*)pADTemplate->CreateNewFrame(pDoc, NULL);
	// ����Ĭ�ϵ��ĵ�����
	// �����ĵ�ʵ��
	// ��ʼ��FRAME��֡���ڣ��Ҵ���FRAME��OnUpdate����
	pADTemplate->InitialUpdateFrame(m_ADFrame, pDoc);
	pDoc->SetTitle(_T("AD���ɲ���"));

	if(pADStatusView != NULL)
	{
		pDoc->AddView ((CView*)pADStatusView);
	}
	if(pParaCfgView != NULL)
	{
		pDoc->AddView((CView*)pParaCfgView);
	}

	EndWaitCursor(); // ֹͣ©�����
}

void CSysApp::OnHistoryData() 
{
	// TODO: Add your command handler code here
	CString strNewFileName;	
	// �����ļ��Ի���
	if (!(DoPromptFileName(strNewFileName, IDS_ProccessHistoryData,
		OFN_HIDEREADONLY , TRUE, NULL))) // OFN_CREATEPROMPT:���ļ�������ʱ���Ƿ�Ҫ���½�
		return;
	
// 	CFile file;
// 	CFileStatus status;
// 	if (!file.GetStatus(strNewFileName, status))
// 	{
// 		strNewFileName=strNewFileName+" ������!";
// 		AfxMessageBox(strNewFileName,MB_ICONSTOP, 0);
// 		return; // ����ļ�����
// 	}
	
	CFile File;
	FILE_HEADER m_Header;

	//File.Open(strNewFileName, GENERIC_READ);
	if (!File.Open(strNewFileName, GENERIC_READ))
	{
		AfxMessageBox(L"���ļ�ʧ��");
		return ;
	}
	File.Seek(0,CFile::begin);
	int a =File.Read((WORD*)&m_Header,sizeof(m_Header)); // �ļ���Ϣ��HistDigitView�ཫ������HistDataFrm�ĶԻ�������OnInitialUpdate
	File.Close();
	if (a == 0)
	{
		AfxMessageBox(L"�ļ���СΪ0");
		return;
	}

	CADHistDoc* pDoc;	
	BeginWaitCursor();  // ��ʼ©�����
	pDoc=(CADHistDoc*)pHistDataTemplate->OpenDocumentFile(strNewFileName); // �ȴ����ļ�����pDoc����Ϣ����OnOpenDocument
	EndWaitCursor();   // ֹͣ©�����

}

void CSysApp::OnUpdateHistoryData(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}



void CSysApp::OnUpdateDataFileNew(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable((!gl_bDeviceADRun)&&m_bFirstCreateDeviceFile);	
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

void CSysApp::OnUpdateOpenDA(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
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
	pCmdUI->Enable(gl_bDeviceADRun==TRUE?1:0);		
}

void CSysApp::OnUpdateCloseFile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetRadio (FALSE); 
	if((nProcessMode == SAVE_MODE) && (bCreateDevice))
		pCmdUI->Enable(FALSE);
	else
		pCmdUI->Enable(!m_bCloseFile);	
}

void CSysApp::OnUpdateDataSave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
	if(m_bFirstCreateDeviceFile)
		pCmdUI->Enable(FALSE);	
	pCmdUI->SetCheck(nProcessMode == SAVE_MODE ? 1:0);	
}

void CSysApp::OnUpdateDigitalShow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(nProcessMode == DATA_MODE ? 1:0);
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
    CSysApp *pApp=(CSysApp *)AfxGetApp();

    CADDoc* pDoc=pApp->m_pADDoc; 
    // ���߳���ȡ����ͼָ��
    CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	if(nProcessMode == SAVE_MODE) // ���Ϊ���̷�ʽ
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
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
	pCmdUI->SetCheck(nProcessMode == WAVE_MODE ? 1:0);
	CSysApp *pApp=(CSysApp *)AfxGetApp();
    CADDoc* pDoc=pApp->m_pADDoc; 
    // ���߳���ȡ����ͼָ��

    CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	
	if(nProcessMode == SAVE_MODE) // ���Ϊ���̷�ʽ
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
	nProcessMode = SAVE_MODE;
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
}

void CSysApp::OnMDigitalShow() 
{
	// TODO: Add your command handler code here
	nProcessMode = DATA_MODE;
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
}

void CSysApp::OnMGraphicShow() 
{
	// TODO: Add your command handler code here
	nProcessMode = WAVE_MODE;
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
}

CDocument* CSysApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString str;
// 	CFile file;
// 	CFileStatus status;
// 	if (!file.GetStatus(lpszFileName, status))
// 	{
// 		str=lpszFileName;
// 		str=str+" ������!";
// 		AfxMessageBox(lpszFileName,MB_ICONSTOP, 0);
// 		return NULL;  // ����ļ�����
// 	}
// 	
	CFile File;
	FILE_HEADER m_Header;
	//File.Open(lpszFileName, GENERIC_READ);
	if (!File.Open(lpszFileName, GENERIC_READ))
	{
		AfxMessageBox(L"���ļ�ʧ��");
		return NULL;
	}
	File.Seek(0, CFile::begin);
	File.Read((WORD*)&m_Header, sizeof(m_Header));  // �ļ���Ϣ��HistDigitView�ཫ������HistDataFrame�ĶԻ�������OnInitialUpdate
	File.Close();
	if(m_Header.DeviceNum != DEFAULT_DEVICE_NUM) 
	{
		if(AfxMessageBox(_T("�Բ��𣬸��ļ����ܽ���, �Ƿ����"),MB_YESNO, 0)==IDNO)
		return NULL;
		//bFile = FALSE;
	}	

	CADHistDoc* pDoc;	
	BeginWaitCursor();  // ��ʼ©�����
	pDoc=(CADHistDoc*)pHistDataTemplate->OpenDocumentFile(lpszFileName);  // �ȴ����ļ�����pDoc����Ϣ����OnOpenDocument
	str = lpszFileName;
	//str="��ʷ���ݣ�"+ str;
	str="��ʷ���ݣ�";
	pDoc->SetTitle(str);  // ���ĵ��±���
	EndWaitCursor(); // ֹͣ©�����
	return pDoc;
	//return CWinApp::OpenDocumentFile(lpszFileName);
}

void CSysApp::OnListDeviceDlg() 
{
	// TODO: Add your command handler code here
	ACTS1000_ListDeviceDlg(m_hDevice); // �б������豸
}

int CSysApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class

	ReleaseMutex(m_hMutex);
 	if(m_hDevice != INVALID_HANDLE_VALUE) 
 	{
		ACTS1000_ReleaseDevice(m_hDevice);
 		m_hDevice = INVALID_HANDLE_VALUE;
 	}
	for(int i=0; i<MAX_SEGMENT; i++)
	{
		ACTS1000_FreeBuffer(ADBuffer[i]);
	}
	ACTS1000_FreeBuffer(ADDrawBuffer);

	return CWinApp::ExitInstance();
}


void CSysApp::OnADCalibration() 
{
	// TODO: Add your command handler code here
	CADCalibration dlg;
	dlg.DoModal();
}

void CAboutDlg::OnBnClickedButtonSet()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������

	if (theApp.m_hDevice != INVALID_HANDLE_VALUE)
	{
		CString str;
		LONG nPhysIdx;
		ULONG nSerialNum, nPassWord;
		CEdit* pEdit;

		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PhysID);
		pEdit->GetWindowText(str);
		nPhysIdx = _tcstoul(str, NULL, 10);
		if (nPhysIdx>255)
		{
			nPhysIdx = 255;
		}


		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SN);
		pEdit->GetWindowText(str);	
		nSerialNum = _tcstoul(str, NULL, 10);	

		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PassWord);
		pEdit->GetWindowText(str);	
		nPassWord = _tcstoul(str, NULL, 10);	



		ACTS1000_SetDevicePhysID(theApp.m_hDevice, nPhysIdx);

		ACTS1000_SetPassword(nPassWord);
		if (!ACTS1000_SetSerialNum(theApp.m_hDevice, nSerialNum))
		{
			AfxMessageBox(L"�������к�ʧ��");
		}
	}
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ����Ӷ���ĳ�ʼ��



	if (theApp.m_hDevice != INVALID_HANDLE_VALUE)
	{
		CString str;
		ULONG nSerialNum;
		LONG nPhysIdx, nDeviceLgcID;
		ACTS1000_GetDeviceCurrentID(theApp.m_hDevice, &nDeviceLgcID, &nPhysIdx);
		ACTS1000_GetSerialNum(theApp.m_hDevice, &nSerialNum);

		CEdit* pEdit;
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_PhysID);
		str.Format(L"%u", nPhysIdx);
		pEdit->SetWindowText(str);

		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SN);
		str.Format(L"%u", nSerialNum);
		pEdit->SetWindowText(str);


		CString strVer;
		ULONG ulFmwVersion, ulDriverVersion;
		if (ACTS1000_GetDevVersion(theApp.m_hDevice, &ulFmwVersion, &ulDriverVersion))
		{
			CStatic* pStatic;

			pStatic = (CStatic*)GetDlgItem(IDC_STATIC_FmwVersion);
			str.Format(_T("%u"),ulFmwVersion);
			pStatic->SetWindowText(str);

			pStatic = (CStatic*)GetDlgItem(IDC_STATIC_DriverVersion);
			str.Format(_T("%u"),ulDriverVersion);
			pStatic->SetWindowText(str);
		}


	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}