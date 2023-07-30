#if !defined(AFX_DATESTDLG_H__6BA2901A_25E8_4FC5_B307_436CD217813A__INCLUDED_)
#define AFX_DATESTDLG_H__6BA2901A_25E8_4FC5_B307_436CD217813A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DATestDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDATestDlg dialog

class CDATestDlg : public CDialog
{
// Construction
public:
	CDATestDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDATestDlg)
	enum { IDD = IDD_DA_Test };
	int		m_nDALsb;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDATestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDATestDlg)
	afx_msg void OnBUTTONDAOut();
	afx_msg void OnChangeEDITDALsb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATESTDLG_H__6BA2901A_25E8_4FC5_B307_436CD217813A__INCLUDED_)
