#if !defined(AFX_ParaCfgView_H__6F78B8A1_CC79_11D6_B1D2_CF0B5E784620__INCLUDED_)
#define AFX_ParaCfgView_H__6F78B8A1_CC79_11D6_B1D2_CF0B5E784620__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ParaCfgView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CADParaCfgView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "ADFrm.h"

class CADParaCfgView : public CFormView
{
protected:
	CADParaCfgView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CADParaCfgView)

public:
	CADFrm* m_pADFrm;
	void SetInputRange(int nInputRange, int nChannel);
	void SetInputRangeEx(int nInputRange, int nChannel);

// Form Data
public:
	//{{AFX_DATA(CADParaCfgView)
	enum { IDD = CG_IDD_AD_CFG };
	CEdit	m_Edit_TrigLevelVolt;
	CEdit	m_Edit_TrigWindow;
	CButton	m_Check_VoltRange0;
	CButton	m_Check_VoltRange1;
	CButton	m_Check_VoltRange10;
	CButton	m_Check_VoltRange11;
	CButton	m_Check_VoltRange12;
	CButton	m_Check_VoltRange13;
	CButton	m_Check_VoltRange14;
	CButton	m_Check_VoltRange15;
	CButton	m_Check_VoltRange2;
	CButton	m_Check_VoltRange3;
	CButton	m_Check_VoltRange4;
	CButton	m_Check_VoltRange5;
	CButton	m_Check_VoltRange6;
	CButton	m_Check_VoltRange7;
	CButton	m_Check_VoltRange8;
	CButton	m_Check_VoltRange9;
	CComboBox	m_Combo_Frequency;
	CComboBox	m_Combo_TriggerMode;
	CComboBox	m_Combo_TriggerType;
	CComboBox	m_Combo_ClockSource;
	CComboBox	m_Combo_VoltRange0;
	CComboBox	m_Combo_VoltRange1;
	CComboBox	m_Combo_VoltRange2;
	CComboBox	m_Combo_VoltRange3;
	CStatic	m_Static_Offset;
	CSliderCtrl	m_Slider_Offset;
	CComboBox	m_Combo_TriggerDir;
	CComboBox	m_Combo_TriggerSource;
	BOOL	m_bSetTrigLevel;
	BOOL	m_bEnableClockOutput;
	//}}AFX_DATA
protected:
	int m_nFrequency; 
	float m_fTrigVolt;
	int m_nDARange;

// Attributes
public:

	BOOL m_bInitialUpdate;

// Operations
public:
	void SetInputRangeComboSel(int* pInputRange);
	void SetSignalSrcComboSel(int* pSignalSrc);
	BOOL CHECKVoltRange();
	void EnableControlWindows(BOOL Flag = TRUE);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADParaCfgView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetStatusBar();
	void SetFreqComboSel(int nFrequecy);
	virtual ~CADParaCfgView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CADParaCfgView)
	afx_msg void OnSelchangeCOMBOTriggerSource();
	afx_msg void OnSelchangeCOMBOTriggerDir();
	afx_msg void OnCustomdrawSLIDEROffset(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCOMBOVoltRange();
	afx_msg void OnSelchangeCOMBOClockSource();
	afx_msg void OnSelchangeCOMBOADMode();
	afx_msg void OnSelchangeCOMBOTriggerType();
	afx_msg void OnSelchangeCOMBOVoltRange1();
	afx_msg void OnSelchangeCOMBOVoltRange2();
	afx_msg void OnSelchangeCOMBOVoltRange3();
	//afx_msg void OnCHECKClockOutput();
	afx_msg void OnSelchangeCOMBOTriggerMode();
	afx_msg void OnKillfocusCOMBOFrequency();
	afx_msg void OnCHECKVoltRange();
	afx_msg void OnChangeEDITTrigWindow();
	afx_msg void OnChangeEDITTrigLevelVolt();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ParaCfgView_H__6F78B8A1_CC79_11D6_B1D2_CF0B5E784620__INCLUDED_)
