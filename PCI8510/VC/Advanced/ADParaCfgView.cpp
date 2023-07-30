// ParaCfgView.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADParaCfgView
///////////////////////////////

UINT IDC_COMBO_Gains[] =
{
	IDC_COMBO_Gains0, IDC_COMBO_Gains1,
		IDC_COMBO_Gains2, IDC_COMBO_Gains3,
		IDC_COMBO_Gains4, IDC_COMBO_Gains5,
		IDC_COMBO_Gains6, IDC_COMBO_Gains7		
};

#define  m_ComboGains(nNum) ((CComboBox*)GetDlgItem(IDC_COMBO_Gains[nNum]))

///////////////////////////////
IMPLEMENT_DYNCREATE(CADParaCfgView, CFormView)
CADParaCfgView::CADParaCfgView()
: CFormView(CADParaCfgView::IDD)
{
	//{{AFX_DATA_INIT(CADParaCfgView)
	m_bSetTrigLevel = TRUE;
	//}}AFX_DATA_INIT

	m_bInitialUpdate = FALSE;
	m_nDARange = 0;
	m_fTrigVolt = 0;
	m_bEnableClockOutput = TRUE;

}

CADParaCfgView::~CADParaCfgView()
{
}

void CADParaCfgView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADParaCfgView)
	DDX_Control(pDX, IDC_CHECK_OutTrigSourceDir, m_Button_OutTrigSourceDir);
	DDX_Control(pDX, IDC_COMBO_ADINRange, m_Combo_ADINRange);
	DDX_Control(pDX, IDC_EDIT_TrigLevelVolt, m_Edit_TrigLevelVolt);
	DDX_Control(pDX, IDC_COMBO_Frequency, m_Combo_Frequency);
	DDX_Control(pDX, IDC_COMBO_TriggerMode, m_Combo_TriggerMode);
	DDX_Control(pDX, IDC_COMBO_TriggerType, m_Combo_TriggerType);
	DDX_Control(pDX, IDC_COMBO_ClockSource, m_Combo_ClockSource);
	DDX_Control(pDX, IDC_COMBO_OutTrigSource, m_Combo_OutTrigSource);
	DDX_Control(pDX, IDC_CHECK_ClockOutput, m_Button_ClockOutput);
	DDX_Control(pDX, IDC_STATIC_Offset, m_Static_Offset);
	DDX_Control(pDX, IDC_SLIDER_Offset, m_Slider_Offset);
	DDX_Control(pDX, IDC_COMBO_OutTriggerDir, m_Combo_TriggerDir);
	DDX_Control(pDX, IDC_COMBO_TriggerSource, m_Combo_TriggerSource);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CADParaCfgView, CFormView)
//{{AFX_MSG_MAP(CADParaCfgView)
	ON_CBN_SELCHANGE(IDC_COMBO_TriggerSource, OnSelchangeCOMBOTriggerSource)	
	ON_CBN_SELCHANGE(IDC_COMBO_OutTriggerDir, OnSelchangeCOMBOTriggerDir)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Offset, OnCustomdrawSLIDEROffset)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains0, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_ClockSource, OnSelchangeCOMBOClockSource)
	ON_CBN_SELCHANGE(IDC_COMBO_OutTrigSource, OnSelchangeCOMBOOutTrigSource)
	ON_CBN_SELCHANGE(IDC_COMBO_TriggerType, OnSelchangeCOMBOTriggerType)
	ON_CBN_SELCHANGE(IDC_COMBO_TriggerMode, OnSelchangeCOMBOTriggerMode)
	ON_CBN_KILLFOCUS(IDC_COMBO_Frequency, OnKillfocusCOMBOFrequency)
	ON_EN_CHANGE(IDC_EDIT_TrigLevelVolt, OnChangeEDITTrigLevelVolt)
	ON_CBN_SELCHANGE(IDC_COMBO_ADINRange, OnSelchangeCOMBOADINRange)
	ON_BN_CLICKED(IDC_CHECK_ClockOutput, OnCHECKClockOutput)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains1, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains2, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains3, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains4, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains5, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains6, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains7, OnSelchangeCOMBOGains)
	ON_BN_CLICKED(IDC_CHECK_OutTrigSourceDir, OnCHECKOutTrigSourceDir)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADParaCfgView diagnostics

#ifdef _DEBUG
void CADParaCfgView::AssertValid() const
{
	CFormView::AssertValid();
}

void CADParaCfgView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADParaCfgView message handlers

void CADParaCfgView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	CADDoc* pDoc = (CADDoc*)theApp.m_pADDoc;
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 

	pDoc->m_hWndADCfg = this->m_hWnd;
	theApp.m_hParaCfgView = this->m_hWnd;
	// 初始化用户界面视觉参数		
	CString str;

	gl_ADINRange = ADPara.InputRange;
	m_Combo_ADINRange.SetCurSel(gl_ADINRange);

		
	for (int nCH=0; nCH<MAX_AD_CHANNELS; nCH++)
	{
		m_ComboGains(nCH)->SetCurSel(ADPara.Gains[nCH]);
		SetInputRange(ADPara.InputRange, nCH);
		SetInputRangeEx(ADPara.InputRange, nCH);
		m_ComboGains(nCH)->EnableWindow(TRUE);
	}

	m_Combo_TriggerSource.SetCurSel(ADPara.TriggerSource);
	m_Combo_TriggerMode.SetCurSel(ADPara.TriggerMode);	// 触发方式设置
	str.Format(_T("%d"), ADPara.TrigLevelVolt);
	m_Edit_TrigLevelVolt.SetWindowText(str);
	
	m_Button_ClockOutput.SetCheck(ADPara.bClockOutput);
	m_Button_OutTrigSourceDir.SetCheck(ADPara.OutTrigSourceDir);
	UpdateData(FALSE);

	m_Combo_TriggerDir.SetCurSel(ADPara.TriggerDir); // 初始化外触发方向
	m_Combo_TriggerType.SetCurSel(ADPara.TriggerType); // 触发方式(电平|边沿)
	m_Static_Offset.SetWindowText(_T("0"));
	m_Slider_Offset.SetRange(0, HALF_SIZE_WORDS/MAX_AD_CHANNELS-1); // 初始化缓冲段内偏移范围

	// 初始化采样频率-----------------------------------------------------------
 	m_Combo_Frequency.AddString(_T("500000 "));
	m_Combo_Frequency.AddString(_T("250000 "));
	m_Combo_Frequency.AddString(_T("100000 "));
	m_Combo_Frequency.AddString(_T("50000 "));
	m_Combo_Frequency.AddString(_T("40000 "));
	m_Combo_Frequency.AddString(_T("25000 "));
	m_Combo_Frequency.AddString(_T("10000 "));
	m_Combo_Frequency.AddString(_T("1000 "));
	m_nFrequency = ADPara.Frequency; // 取得频率值
	SetFreqComboSel(m_nFrequency);   // 在组合框中选择频率
	m_Combo_OutTrigSource.SetCurSel(ADPara.OutTrigSource);		
	m_Combo_ClockSource.SetCurSel(ADPara.ClockSource);			// 初始化时钟源

	SetStatusBar();

	m_Edit_TrigLevelVolt.EnableWindow(TRUE);
 
	m_bInitialUpdate = TRUE;
	theApp.m_pADDoc->m_hADParaCfgView = this;
	OnSelchangeCOMBOTriggerType();
	// 采样长度设置
}

//###########################################################################
// 设置触发源 (内触发或外触发)
void CADParaCfgView::OnSelchangeCOMBOTriggerMode() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TriggerMode);
	ADPara.TriggerMode = pCombo->GetCurSel();
}

void CADParaCfgView::OnSelchangeCOMBOTriggerSource() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_TriggerSource);
	ADPara.TriggerSource = pCombo->GetCurSel();
}

void CADParaCfgView::EnableControlWindows(BOOL Flag)
{
	
	m_Combo_Frequency.EnableWindow(Flag);
	m_Combo_TriggerMode.EnableWindow(Flag);
	m_Combo_TriggerDir.EnableWindow(Flag);    // 外触发沿	
	m_Combo_TriggerSource.EnableWindow(Flag); // 触发方式
	m_Combo_ClockSource.EnableWindow(Flag);
	m_Combo_OutTrigSource.EnableWindow(Flag);
	m_Button_OutTrigSourceDir.EnableWindow(Flag);
	m_Button_ClockOutput.EnableWindow(Flag);

	for (int nCH=0; nCH<MAX_AD_CHANNELS; nCH++)
	{			
		m_ComboGains(nCH)->EnableWindow(Flag);
	}

	m_Combo_ADINRange.EnableWindow(Flag);

	m_Combo_TriggerDir.EnableWindow(Flag); 
	m_Combo_TriggerType.EnableWindow(Flag);
	m_Edit_TrigLevelVolt.EnableWindow(Flag);
}

//######################################################################################
// 外触发方向设置
void CADParaCfgView::OnSelchangeCOMBOTriggerDir() 
{
	ADPara.TriggerDir = m_Combo_TriggerDir.GetCurSel();
}

void CADParaCfgView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)  
{
	// TODO: Add your specialized code here and/or call the base class
	if ((lHint==1) || (lHint==2)) return;
	CView::OnUpdate(pSender, lHint, pHint);
}

void CADParaCfgView::OnCustomdrawSLIDEROffset(NMHDR*  pNMHDR, LRESULT* pResult) 
{ 
	if (m_bInitialUpdate) // 如果本类对象的OnInitialUpdate函数已调用后
	{ 
		CSliderCtrl* pSliderOffset = (CSliderCtrl*)(GetDlgItem(IDC_SLIDER_Offset));	
		int Pos = pSliderOffset->GetPos();  // 取得滑动条位置
		pSliderOffset->SetPos(Pos);
		
		gl_Offset = Pos;	// gloffset 标志在缓冲区中的偏移
	
		CString str;
		str.Format(_T("%d"), gl_Offset);
		CStatic* pOffsetStatic = (CStatic*)(GetDlgItem(IDC_STATIC_Offset));
		pOffsetStatic->SetWindowText(str);
		CSysApp* pApp = (CSysApp*)AfxGetApp();	
		CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndWave));
		CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndDigit));
		
		pWaveView->OnDrawPolyLine(); // 更新波形视图
		pDigitView->RedrawWindow();  // 更新数字视图
		
		CADFrm* pADFrm = (CADFrm*)pWaveView->GetParentFrame();
		CEdit* pEditOffset = (CEdit*)(pADFrm->m_wndShowStatus).GetDlgItem(IDC_EDIT_Offset); // 取得IDC_STATIC_ShowXY句柄
		str.Format(_T("%d"), gl_Offset);
		pEditOffset->SetWindowText(str);
		*pResult = 0;
	}
}

void CADParaCfgView::SetInputRangeEx(int nInputRange, int nChannel)
{
	CADDoc* pDoc = theApp.m_pADDoc; 
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit));

	int nCH =nChannel;

		
	switch(nInputRange)
		{
		case PCI8510_INPUT_N10000_P10000mV:			
			gl_InputRange[nChannel] = 20000;
			gl_PerLsbVolt[nChannel] = (float)(20000.0 / AD_LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(-10000, 10000, nChannel);
			break;

		case PCI8510_INPUT_N5000_P5000mV:		
			gl_InputRange[nChannel] = 10000;
			gl_PerLsbVolt[nChannel] = (float)(10000.0 / AD_LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(-5000, 5000, nChannel);
			break;

		case PCI8510_INPUT_0_P10000mV:		
			gl_InputRange[nChannel] = 10000;
			gl_PerLsbVolt[nChannel] = (float)(10000.0 / AD_LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(0, 10000, nChannel);
			break;

		case PCI8510_INPUT_0_P5000mV:		
			gl_InputRange[nChannel] = 5000;
			gl_PerLsbVolt[nChannel] = (float)(5000.0 / AD_LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(0, 5000, nChannel);
			break;

		default:
			break;
		}

	if (nChannel == 0)
	{
		gl_ScreenVolume = gl_InputRange[0];
		gl_VoltRange = gl_InputRange[0];
	}

	

}

void CADParaCfgView::SetInputRange(int nInputRange, int nChannel)
{
	CADDoc* pDoc = theApp.m_pADDoc; 
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 

	if (nChannel==0)
	{
		gl_VoltRange = gl_InputRange[nChannel];
		gl_ScreenVolume = (int)gl_VoltRange;
	}
	//ADPara.Gains[nChannel] = nInputRange;

	pDigitView->Invalidate();
}

// 设置通道0的电压量程
void CADParaCfgView::OnSelchangeCOMBOGains() 
{
	int nChannel = 0;
	int nInputRange = 0;
	

	const MSG* pMsg = GetCurrentMessage();
	for (int nCH=0; nCH<MAX_AD_CHANNELS; nCH++)
	{
		if (pMsg->lParam == (LPARAM)m_ComboGains(nCH)->m_hWnd)
		{
			nInputRange = m_ComboGains(nCH)->GetCurSel();			
			ADPara.Gains[nCH] = nInputRange;

			SetInputRange(nInputRange, nCH);
			break;
		}
	}
}

//######################################################################
// 设置外触发类型 (电平触发或沿触发)
void CADParaCfgView::OnSelchangeCOMBOTriggerType() 
{
	// TODO: Add your control notification handler code here
	ADPara.TriggerType = m_Combo_TriggerType.GetCurSel();
	m_Combo_TriggerDir.DeleteString(0);
	m_Combo_TriggerDir.DeleteString(0);
	m_Combo_TriggerDir.DeleteString(0);
	
	if (ADPara.TriggerType == PCI8510_TRIGTYPE_EDGE)	// 边沿触发
	{		
		m_Combo_TriggerDir.AddString(_T("下降沿触发"));
		m_Combo_TriggerDir.AddString(_T("上升沿触发"));
		m_Combo_TriggerDir.AddString(_T("上下沿均触发"));
	}
	else
	{		
		m_Combo_TriggerDir.AddString(_T("低电平触发"));
		m_Combo_TriggerDir.AddString(_T("高电平触发"));
		m_Combo_TriggerDir.AddString(_T("高低电平均触发"));
	}
	
	m_Combo_TriggerDir.SetCurSel(ADPara.TriggerDir);

}


//#########################################################################
// 时钟源选择 (内时钟或外时钟)
void CADParaCfgView::OnSelchangeCOMBOClockSource() 
{
	ADPara.ClockSource = m_Combo_ClockSource.GetCurSel();
	if (ADPara.ClockSource == PCI8510_CLOCKSRC_IN)
		m_Combo_Frequency.EnableWindow(TRUE);
	else
		m_Combo_Frequency.EnableWindow(FALSE);
}
void CADParaCfgView::OnCHECKClockOutput() 
{
	UpdateData();
	if (m_Button_ClockOutput.GetCheck())
	{
		ADPara.bClockOutput = TRUE;
	}
	else
	{
		ADPara.bClockOutput = FALSE;
	}
}

// 通过频率值来设置频率设置项的选择
void CADParaCfgView::SetFreqComboSel(int nFrequecy)
{
	CString strFrequecy;
	strFrequecy.Format(_T("%d "), nFrequecy); // %d后面的空格不能少!
	m_Combo_Frequency.SelectString(-1, strFrequecy);
 	ADPara.Frequency = nFrequecy;
	
}

// 设置底下的状态栏
void CADParaCfgView::SetStatusBar()
{
	CString strTimeDiv, strFrequency;
	float fTimePixel, fTimeDiv;
	m_Combo_Frequency.GetWindowText(strFrequency);
	long nFrequency = (long)_tcstol(strFrequency, NULL, 10);
	ADPara.Frequency = nFrequency;
	fTimePixel = (float)(1000000.0/nFrequency); // 求每两点间的周期(uS)
	fTimeDiv = fTimePixel * 50; // 求每大格的周期(每大格为50个象素)
	if (fTimeDiv < 1000.0)
		strTimeDiv.Format(_T("%7.2f uS/Div"), fTimeDiv);

	if ((fTimeDiv >= 1000.0) && (fTimeDiv < 1000000.0))
		strTimeDiv.Format(_T("%7.2f mS/Div"), fTimeDiv/1000);

	if (fTimeDiv >= 1000000.0)
		strTimeDiv.Format(_T("%7.2f S/Div"), fTimeDiv/1000000);

	CStatic* pStatic = (CStatic*)m_pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv);
    pStatic->SetWindowText(strTimeDiv);
}


void CADParaCfgView::OnKillfocusCOMBOFrequency() 
{
	// TODO: Add your control notification handler code here
	CString strFrequency;
	m_Combo_Frequency.GetWindowText(strFrequency);
	long nFrequency = (long)_tcstol(strFrequency, NULL, 10);

	ADPara.Frequency = nFrequency; // 设置频率值

	SetStatusBar(); // 设置底下的状态栏	
}


BOOL CADParaCfgView::CHECKVoltRange()
{
	for (int Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
	{
		if (Channel == 8) return FALSE;
	}	
	return TRUE;
}



void CADParaCfgView::OnChangeEDITTrigLevelVolt() 
{
	CString strTrigVolt;
	LONG oldTrigLevelVolt, newTrigLevelVolt;
	oldTrigLevelVolt = ADPara.TrigLevelVolt;
	m_Edit_TrigLevelVolt.GetWindowText(strTrigVolt);
	newTrigLevelVolt = _tcstol(strTrigVolt, NULL, 10);
	if (newTrigLevelVolt<-10000 || newTrigLevelVolt>10000)
	{
		AfxMessageBox(_T("输入错误"));
		ADPara.TrigLevelVolt = oldTrigLevelVolt;
		strTrigVolt.Format(_T("%d"), ADPara.TrigLevelVolt);
		m_Edit_TrigLevelVolt.SetWindowText(strTrigVolt);
	}
	else
	{
		ADPara.TrigLevelVolt = newTrigLevelVolt;
	}

}

void CADParaCfgView::OnSelchangeCOMBOADINRange() 
{
	// TODO: Add your control notification handler code here
	
	gl_ADINRange = m_Combo_ADINRange.GetCurSel();
	ADPara.InputRange = gl_ADINRange;
	
	for (int Channel=0; Channel<MAX_AD_CHANNELS; Channel++) // 初始化电压量程
	{
		SetInputRangeEx(gl_ADINRange, Channel);
	}
}

void CADParaCfgView::OnSelchangeCOMBOOutTrigSource() 
{
	// TODO: Add your control notification handler code here
	ADPara.OutTrigSource = m_Combo_OutTrigSource.GetCurSel();
// 	if (ADPara.OutTrigSource > 1)
// 	{
// 		ADPara.OutTrigSource = ADPara.OutTrigSource + 5;
// 	}
}


void CADParaCfgView::OnCHECKOutTrigSourceDir() 
{
	// TODO: Add your control notification handler code here
	ADPara.OutTrigSourceDir = m_Button_OutTrigSourceDir.GetCheck();
}
