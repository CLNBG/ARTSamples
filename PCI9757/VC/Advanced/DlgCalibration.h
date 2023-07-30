#if !defined(AFX_DLGCALIBRATION_H__0795B992_CCBF_42D3_9239_57D2B74EA847__INCLUDED_)
#define AFX_DLGCALIBRATION_H__0795B992_CCBF_42D3_9239_57D2B74EA847__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCalibration.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgCalibration dialog

class CDlgCalibration : public CDialog
{
// Construction
public:
	LONG m_lProgress;
	void StopCalibration();
	CDlgCalibration(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgCalibration)
	enum { IDD = IDD_DIALOG_ADCalibration };
	CProgressCtrl	m_ProgressCalibration;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCalibration)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	static UINT ADCalibration(PVOID hWnd);

	// Generated message map functions
	//{{AFX_MSG(CDlgCalibration)
	afx_msg void OnBUTTONCalibration();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCALIBRATION_H__0795B992_CCBF_42D3_9239_57D2B74EA847__INCLUDED_)
