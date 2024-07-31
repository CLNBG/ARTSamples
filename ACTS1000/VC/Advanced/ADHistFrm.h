#if !defined(AFX_HistDataFrm_H__D442B980_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_)
#define AFX_HistDataFrm_H__D442B980_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistDataFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CADHistFrm frame

class CADHistFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CADHistFrm)
protected:
	CADHistFrm();           // protected constructor used by dynamic creation

// Attributes
public:
	CSplitterWnd m_wndSplitter;
	CDialogBar m_wndSetupBar;
public:
	BOOL m_bStart;
	int  m_ShowMode;
	LONG m_PositionAoffset;
public:
	void RedrawDataWindow(void);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADHistFrm)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual CDocument* GetActiveDocument();
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual ~CADHistFrm();

	// Generated message map functions
	//{{AFX_MSG(CADHistFrm)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHistoryStart();
	afx_msg void OnUpdateHistoryStart(CCmdUI* pCmdUI);
	afx_msg void OnHistoryStop();
	afx_msg void OnUpdateHistoryStop(CCmdUI* pCmdUI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCustomdrawSLIDEROffset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCOMBORate();
	afx_msg void OnChangeEDITOffset();
	afx_msg void OnClose();
	afx_msg void OnBUTTONRefrushData();
	afx_msg void OnBUTTONPositionA();
	afx_msg void OnBUTTONPositionB();
	afx_msg void OnBUTTONTriggerPos();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGoto();
	afx_msg void OnEnKillfocusEditOffset();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HistDataFrm_H__D442B980_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_)
