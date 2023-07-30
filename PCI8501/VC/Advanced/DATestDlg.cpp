// DATestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "sys.h"
#include "DATestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDATestDlg dialog


CDATestDlg::CDATestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDATestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDATestDlg)
	m_nDALsb = 0;
	//}}AFX_DATA_INIT
}


void CDATestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDATestDlg)
	DDX_Text(pDX, IDC_EDIT_DALsb, m_nDALsb);
	DDV_MinMaxInt(pDX, m_nDALsb, 0, 4095);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDATestDlg, CDialog)
	//{{AFX_MSG_MAP(CDATestDlg)
	ON_BN_CLICKED(IDC_BUTTON_DAOut, OnBUTTONDAOut)
	ON_EN_CHANGE(IDC_EDIT_DALsb, OnChangeEDITDALsb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDATestDlg message handlers

void CDATestDlg::OnBUTTONDAOut() 
{
	// TODO: Add your control notification handler code here
	CSysApp* pApp = (CSysApp*)AfxGetApp();
//	PXI8501_WriteDeviceProDA(pApp->m_hDevice, m_nDALsb); // 准备输出的DA数据LSB原码
}

void CDATestDlg::OnChangeEDITDALsb() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
}
