// DlgCalibration.cpp : implementation file
//

#include "stdafx.h"
#include "sys.h"
#include "ADCalibration.h"
#include "ACTS1000/ACTS1000.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADCalibration dialog



CADCalibration::CADCalibration(CWnd* pParent /*=NULL*/)
	: CDialog(CADCalibration::IDD, pParent)
{
	//{{AFX_DATA_INIT(CADCalibration)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CADCalibration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADCalibration)
	DDX_Control(pDX, IDC_PROGRESS_Calibration, m_ProgressCalibration);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CADCalibration, CDialog)
	//{{AFX_MSG_MAP(CADCalibration)
	ON_BN_CLICKED(IDC_BUTTON_Calibration, OnBUTTONCalibration)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADCalibration message handlers
extern CSysApp theApp;
BOOL gl__bCalibration = FALSE;

BOOL CADCalibration::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	m_ProgressCalibration.SetRange(0, 100);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CADCalibration::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	m_lProgress++;
	if (m_lProgress > 100)
		m_lProgress = 0;
	m_ProgressCalibration.SetPos(m_lProgress);

	if (!gl__bCalibration)
		StopCalibration();


	CDialog::OnTimer(nIDEvent);
}

void CADCalibration::OnBUTTONCalibration()
{
	// TODO: Add your control notification handler code here	

	if (theApp.m_hDevice != INVALID_HANDLE_VALUE)
	{
		int iRet = AfxMessageBox(_T("确认没有外部连接信号(注意:仅支持输入阻抗为1M欧校准)!"), MB_OKCANCEL);
		if (iRet == 2)
		{
			return;
		}
		CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_Calibration);
		m_lProgress = 0;
		gl__bCalibration = TRUE;
		SetTimer(1, 100, NULL);
		pStatic->SetWindowText(_T("Calibration over a long time, please be patient"));
		GetDlgItem(IDC_BUTTON_Calibration)->EnableWindow(FALSE);

		AfxBeginThread(ReadDataThread, &m_hWnd, THREAD_PRIORITY_HIGHEST);
	}
	else
	{
		AfxMessageBox(_T("The device does not exist"), MB_OK, MB_ICONWARNING);
	}
}

UINT CADCalibration::ReadDataThread(PVOID hWnd)
{
	ACTS1000_ADCalibration(theApp.m_hDevice);
	gl__bCalibration = FALSE;

	TRACE("END ADCalibrationThread");
	return 1;
}

void CADCalibration::StopCalibration()
{
	GetDlgItem(IDC_BUTTON_Calibration)->EnableWindow(TRUE);
	KillTimer(1);
	m_ProgressCalibration.SetPos(100);
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_Calibration);
	pStatic->SetWindowText(_T("Complete the calibration"));
	AfxMessageBox(_T("Complete the calibration"));

}
