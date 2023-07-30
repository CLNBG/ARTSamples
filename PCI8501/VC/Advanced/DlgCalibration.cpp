// DlgCalibration.cpp : implementation file
//

#include "stdafx.h"
#include "sys.h"
#include "DlgCalibration.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCalibration dialog



CDlgCalibration::CDlgCalibration(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCalibration::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCalibration)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgCalibration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCalibration)
	DDX_Control(pDX, IDC_PROGRESS_Calibration, m_ProgressCalibration);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_VoltRangeD, m_Combo_VoltRangeD);
}


BEGIN_MESSAGE_MAP(CDlgCalibration, CDialog)
	//{{AFX_MSG_MAP(CDlgCalibration)
	ON_BN_CLICKED(IDC_BUTTON_Calibration, OnBUTTONCalibration)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCalibration message handlers
extern CSysApp theApp;
BOOL gl__bCalibration = FALSE;

BOOL CDlgCalibration::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_ProgressCalibration.SetRange(0, 100);
	m_Combo_VoltRangeD.SetCurSel(0);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCalibration::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	m_lProgress++;
	if (m_lProgress>100)
		m_lProgress = 0;
	m_ProgressCalibration.SetPos(m_lProgress);

	if (!gl__bCalibration)
		StopCalibration();
		
		
	CDialog::OnTimer(nIDEvent);
}
LONG gl_CalibrationRange=0;
void CDlgCalibration::OnBUTTONCalibration() 
{
	// TODO: Add your control notification handler code here
	if (theApp.m_hDevice != INVALID_HANDLE_VALUE)
	{
		gl_CalibrationRange = m_Combo_VoltRangeD.GetCurSel();
		int iRet = AfxMessageBox(L"您是否确认板卡没有外部连接",MB_OKCANCEL);
		if (iRet == 2)
		{
			return;
		}
		CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_Calibration);
		m_lProgress =0;		
		gl__bCalibration = TRUE;
		SetTimer(1, 100, NULL);
		pStatic->SetWindowText(L"校准时间较长，请耐心等待");
		GetDlgItem(IDC_BUTTON_Calibration)->EnableWindow(FALSE);		
		
		AfxBeginThread(ADCalibration, &m_hWnd, THREAD_PRIORITY_HIGHEST);		
	}
	else
	{
		AfxMessageBox(L"设备不存在",MB_OK, MB_ICONWARNING);
	}
}

UINT CDlgCalibration::ADCalibration(PVOID hWnd)
{	

	PCI8501_ADCalibration(theApp.m_hDevice, gl_CalibrationRange);
	
	gl__bCalibration = FALSE;

	TRACE("END ADCalibrationThread");
	return 1;
}

void CDlgCalibration::StopCalibration()
{
	GetDlgItem(IDC_BUTTON_Calibration)->EnableWindow(TRUE);
	KillTimer(1);
	m_ProgressCalibration.SetPos(100);
	CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_Calibration);
	pStatic->SetWindowText(L"校准完成");
	
}
