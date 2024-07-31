#if !defined(AFX_AIAICHAN_LIST_H__162EB05F_779E_401B_83B8_2BCD82BC8A83__INCLUDED_)
#define AFX_AICHAN_LIST_H__162EB05F_779E_401B_83B8_2BCD82BC8A83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CHListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAIChanList window
#include "ChanCombo.h"
#include "ChanEdit.h"

#define LIST_COL_CHANNEL	0
#define LIST_COL_EN  1	
#define LIST_COL_SAMPRANGE	2
//#define LIST_COL_GAIN		1
#define LIST_COL_REFGND		3
#define LIST_COL_IMPED		4

class CAIChanList : public CListCtrl
{
// Construction
public:
	CAIChanList();

// Attributes
public:
	int m_nChanIdx; // 通道组单元下标
	int m_nType;
	
	CChanEdit	m_wndEdit;				// Build in Edit Control
	CChanCombo	m_wndCombo;				// Build in ComboBox Control
	BOOL RestoreCtrl(CRect rcCtrl);
	BOOL SetCtrlFocus(CWnd* pWnd, BOOL bFocus);
	BOOL m_bEnableEx;
	BOOL EnableWindowEx(BOOL bEnable);
	void SetSameParam(int nCurItem, int nSubItem);
	int m_nCurItem;
	int m_nCurSubItem;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAIChanList)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	LRESULT OnSetCtrlFocus(WPARAM wParam, LPARAM lParam);	

	virtual ~CAIChanList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAIChanList)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnDelNode();
	afx_msg void OnSameParam();
	afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHPARAMLIST_H__062EB05F_779E_401B_83B8_2BCD82BC8A83__INCLUDED_)
