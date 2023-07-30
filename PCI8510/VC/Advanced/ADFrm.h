// ADFrm.h : interface of the CADFrm class
//
/////////////////////////////////////////////////////////////////////////////

#include "MTabWnd.h"
#if !defined(AFX_ADFRM_H__D485DF03_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
#define AFX_ADFRM_H__D485DF03_85AB_11D1_B878_A68EB300DD63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "CfgTabCtrlBar.h"
#include "QSortData.h"	// Added by ClassView
#include "stdafx.h"

class CADFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CADFrm)

public:
	CADFrm();

	inline void ShowHideDlgSave(BOOL Flag = TRUE)
	{ ShowControlBar(&m_wndDlgSave, Flag, FALSE); }

	inline void ShowHideStatus(BOOL Flag = TRUE)
	{ ShowControlBar(&m_wndShowStatus, Flag, FALSE); }

	inline CStatic* GetFileNameStatic()
	{ return ((CStatic*)m_wndDlgSave.GetDlgItem(IDC_FileName)); }

	inline CStatic* GetDiskFreeStatic()
	{ return ((CStatic*)m_wndDlgSave.GetDlgItem(IDC_DiskFree)); }

	inline CStatic* GetFileLenghtStatic()
	{ return ((CStatic*)m_wndDlgSave.GetDlgItem(IDC_FileLenght)); }

	inline CProgressCtrl* GetProgressCtrl()
	{ return ((CProgressCtrl*)m_wndDlgSave.GetDlgItem(IDC_PROGRESS)); }

	inline CStatic* GetTimeDivStatic()
	{ return ((CStatic*)m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv)); }

// Attributes
public:
 	LONG DataAnalyze[MAX_CHANNEL_COUNT][16384];
	CDialogBar	m_wndDlgSave; // 存储状态显示框

public:
	CString GetAppPath();

public:
	CCfgTabCtrlBar m_wndSTCBar;  // 放于右侧的形如VC左侧的控件
	CSplitterWnd m_wndSplitter;  // 分割视图控件
	CDialogBar m_wndShowStatus;  // AD状态显示框

	CToolTipCtrl m_ToolTip;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADFrm)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	CQSortData m_QSortData;
	virtual ~CADFrm();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	CDialogBar m_DevDlgBar;
	//{{AFX_MSG(CADFrm)
	afx_msg void OnClose();
	afx_msg void OnCancelMode();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMoveVolt();
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateMoveVolt(CCmdUI* pCmdUI);
	afx_msg void OnShowCfgAnalBar();
	afx_msg void OnUpdateShowCfgAnalBar(CCmdUI* pCmdUI);
	afx_msg void OnShowStatusBar();
	afx_msg void OnUpdateShowStatusBar(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADFRM_H__D485DF03_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
