#if !defined(AFX_ParaCfgView_H__6F78B8A1_CC79_11D6_B1D2_CF0B5E784620__INCLUDED_)
#define AFX_ParaCfgView_H__6F78B8A1_CC79_11D6_B1D2_CF0B5E784620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParaCfgView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CParaCfgView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ADFrm.h"
#include "afxwin.h"
#include "AIChanList.h"
#include "ADDoc.h"
class CParaCfgView : public CFormView
{
protected:
	CParaCfgView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CParaCfgView)

public:
	CADFrame* m_pADFrm;
	BOOL m_bErrorDlg;
// Form Data
public:
	//{{AFX_DATA(CParaCfgView)
	enum { IDD = CG_IDD_ADCHANNEL };
	CEdit	m_Edit_TrigInterva;
	CButton	m_Check_TimerTrig;
	CButton	m_Button_SoftTirg;
	CComboBox	m_Combo_SyncTrigSignal;
	CButton	m_Check_bMasterEn;
	CButton	m_Check_bClkOutEn;
	CButton	m_Check_bTrigOutEn;
	CEdit	m_Edit_nSampleLenth;
	//CButton	m_Check_bAvailableTrig;
	CEdit	m_Edit_TrigLevelVolt;
	CEdit   m_Edit_TrigCount;
	CEdit   m_Edit_TrigWindow;
	CEdit   m_Edit_FreqDivision;
	CEdit   m_Edit_Frequency;
	CEdit   m_Edit_TrigOutWidth;
	CEdit	m_Edit_N_Length;
	CEdit	m_Edit_M_Length;
	CComboBox	m_Combo_ReferenceClock;
	CComboBox	m_Combo_TrigOutPolarity;
	CComboBox	m_Combo_ClkOutSel;
	CComboBox	m_ComBo_TriggerMode;

	CComboBox	m_Combo_SampleLenth;
	CComboBox	m_Combo_TriggerDir;
	CComboBox	m_Combo_TriggerSource;
	CStatic		m_Static_Offset;
	CSpinButtonCtrl	m_Spin_BufferID;
	CSliderCtrl	m_Slider_Offset;
	CEdit		m_Edit_BufferID;
	CButton		m_Check_Auto;
	LONG		m_TrigLevelVolt;
	CAIChanList m_List_CHParam;
	//}}AFX_DATA

// Attributes
public:

	long Array[80];
	int CountItem;
	BOOL m_bInitial;
	SCROLLINFO si;
	long cyChar;
	int iVertPos;
	int m_nBits;
	int    m_nChannelDataBit;
	LONG m_lSingleCH;

// Operations
public:
	void EnableVootRange();
	void StartAD();
	void SetInputRangeComboSel(int* pInputRange);
	
	void EnableWindows(BOOL Flag=TRUE);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParaCfgView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CParaCfgView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CParaCfgView)
	afx_msg void OnSelchangeCOMBOTriggerSource();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSelchangeCOMBOOutTriggerDir();
	afx_msg void OnCustomdrawSLIDEROffset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeEDITBufferID();
	afx_msg void OnCHECKAuto();
	afx_msg void OnSelchangeCOMBOVoltRange();
	afx_msg void OnSelchangeCOMBOTriggerMode();
//	afx_msg void OnKillfocusCOMBObAvailableTrig();
	afx_msg void OnSelchangeCOMBOReferenceClock();
//	afx_msg void OnCHECKbAvailableTrig();
	afx_msg void OnChangeEDITTrigLevelVolt();
	afx_msg void OnChangeEDITMLength();
	afx_msg void OnChangeEDITNLength();
	afx_msg void OnCHECKbMasterEn();
	afx_msg void OnCHECKbClkOutEn();
	afx_msg void OnCHECKbTrigOutEn();
	afx_msg void OnSelchangeCOMBOSyncTrigSignal();
	afx_msg void OnBUTTONSoftTirg();
	afx_msg void OnKillfocusEDITTrigInterval();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnKillfocusEditMLength();
public:
	afx_msg void OnEnKillfocusEditNLength();
	afx_msg void OnCheckCh();
	afx_msg void OnEnChangeEditTrigcount();
	afx_msg void OnEnChangeEditTrigWindow();
public:
	afx_msg void OnCbnSelchangeComboClkoutsel();
public:
	afx_msg void OnCbnSelchangeComboTrigoutpolarity();
public:
	afx_msg void OnEnChangeEditTrigoutwidth();
public:
	afx_msg void OnEnKillfocusEditTrigoutwidth();
public:
	afx_msg void OnEnKillfocusEditTrigwindow();
public:
	afx_msg void OnEnKillfocusEditFreqdivision();
public:
	afx_msg void OnEnKillfocusEditFrequency();
public:
	CComboBox m_Combo_TimeBaseClock;
public:
	afx_msg void OnCbnSelchangeComboTimebaseclock();
public:
	CComboBox m_ComBo_SampleMode;
public:
	afx_msg void OnCbnSelchangeComboSamplemode();
public:
	CComboBox m_Combo_PFISel;
public:
	afx_msg void OnCbnSelchangeComboPfisel();
public:
	CButton m_Check_bSaveFile;
public:
	CEdit m_Edit_FilePath;
public:
	afx_msg void OnBnClickedCheckbsavefile();
public:
	afx_msg void OnEnKillfocusEditFilepath();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ParaCfgView_H__6F78B8A1_CC79_11D6_B1D2_CF0B5E784620__INCLUDED_)
