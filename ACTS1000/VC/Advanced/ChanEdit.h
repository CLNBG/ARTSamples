#if !defined(AFX_CHAN_EDIT_H__C1AFC325_4FA3_421A_A85B_AAFE25FAD9F1__INCLUDED_)
#define AFX_CHAN_EDIT_H__C1AFC325_4FA3_421A_A85B_AAFE25FAD9F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CChanEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChanEdit window

class CChanEdit : public CEdit
{
// Construction
public:
	CChanEdit();

// Attributes
public:

protected:
	CFont m_Font;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChanEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChanEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChanEdit)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHAN_EDIT_H__C1AFC325_4FA3_421A_A85B_AAFE25FAD9F1__INCLUDED_)
