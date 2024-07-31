#if !defined(AFX_CHAN_COMBO_H__08E497EB_35AE_47F1_962E_DA7AB611098E__INCLUDED_)
#define AFX_CHAN_COMBO_H__08E497EB_35AE_47F1_962E_DA7AB611098E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChanCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChanCombo window

class CChanCombo : public CComboBox
{
// Construction
public:
	CChanCombo();
	CFont m_Font;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChanCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChanCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChanCombo)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHAN_COMBO_H__08E497EB_35AE_47F1_962E_DA7AB611098E__INCLUDED_)
