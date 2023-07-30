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



static UINT IDC_CHECK_VoltRange[] = 
{
	IDC_CHECK_VoltRange0,
	IDC_CHECK_VoltRange1,
	IDC_CHECK_VoltRange2,
	IDC_CHECK_VoltRange3
};

static UINT IDC_COMBO_VoltRange[] = 
{
	IDC_COMBO_VoltRange0,
	IDC_COMBO_VoltRange1,
	IDC_COMBO_VoltRange2,
	IDC_COMBO_VoltRange3
};





#define m_Button_VoltRange(nNum)  ((CButton*)GetDlgItem(IDC_CHECK_VoltRange[nNum]))
#define m_Combo_VoltRange(nNum)  ((CComboBox*)GetDlgItem(IDC_COMBO_VoltRange[nNum]))



///////////////////////////////
IMPLEMENT_DYNCREATE(CADParaCfgView, CFormView)
CADParaCfgView::CADParaCfgView()
: CFormView(CADParaCfgView::IDD)
{
	//{{AFX_DATA_INIT(CADParaCfgView)
	m_bSetTrigLevel = TRUE;
	m_bEnableClockOutput = FALSE;
	//}}AFX_DATA_INIT

	m_bInitialUpdate = FALSE;
	m_nDARange = 0;
	m_fTrigVolt = 0;

}

CADParaCfgView::~CADParaCfgView()
{
}

void CADParaCfgView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CADParaCfgView)
	DDX_Control(pDX, IDC_EDIT_TrigLevelVolt, m_Edit_TrigLevelVolt);
	DDX_Control(pDX, IDC_EDIT_TrigWindow, m_Edit_TrigWindow);
	DDX_Control(pDX, IDC_CHECK_VoltRange0, m_Check_VoltRange0);
	DDX_Control(pDX, IDC_CHECK_VoltRange1, m_Check_VoltRange1);
	DDX_Control(pDX, IDC_CHECK_VoltRange2, m_Check_VoltRange2);
	DDX_Control(pDX, IDC_CHECK_VoltRange3, m_Check_VoltRange3);
	DDX_Control(pDX, IDC_COMBO_Frequency, m_Combo_Frequency);
	DDX_Control(pDX, IDC_COMBO_TriggerMode, m_Combo_TriggerMode);
	DDX_Control(pDX, IDC_COMBO_TriggerType, m_Combo_TriggerType);
	DDX_Control(pDX, IDC_COMBO_ClockSource, m_Combo_ClockSource);
	DDX_Control(pDX, IDC_COMBO_VoltRange0, m_Combo_VoltRange0);
	DDX_Control(pDX, IDC_COMBO_VoltRange1, m_Combo_VoltRange1);
	DDX_Control(pDX, IDC_COMBO_VoltRange2, m_Combo_VoltRange2);
	DDX_Control(pDX, IDC_COMBO_VoltRange3, m_Combo_VoltRange3);
	DDX_Control(pDX, IDC_STATIC_Offset, m_Static_Offset);
	DDX_Control(pDX, IDC_SLIDER_Offset, m_Slider_Offset);
	DDX_Control(pDX, IDC_COMBO_OutTriggerDir, m_Combo_TriggerDir);
	DDX_Control(pDX, IDC_COMBO_TriggerSource, m_Combo_TriggerSource);
	//DDX_Check(pDX, IDC_CHECK_ClockOutput, m_bEnableClockOutput);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CADParaCfgView, CFormView)
//{{AFX_MSG_MAP(CADParaCfgView)
	ON_CBN_SELCHANGE(IDC_COMBO_TriggerSource, OnSelchangeCOMBOTriggerSource)	
	ON_CBN_SELCHANGE(IDC_COMBO_OutTriggerDir, OnSelchangeCOMBOTriggerDir)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Offset, OnCustomdrawSLIDEROffset)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange0, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_ClockSource, OnSelchangeCOMBOClockSource)
	ON_CBN_SELCHANGE(IDC_COMBO_TriggerType, OnSelchangeCOMBOTriggerType)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange1, OnSelchangeCOMBOVoltRange1)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange2, OnSelchangeCOMBOVoltRange2)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange3, OnSelchangeCOMBOVoltRange3)
	//ON_BN_CLICKED(IDC_CHECK_ClockOutput, OnCHECKClockOutput)
	ON_CBN_SELCHANGE(IDC_COMBO_TriggerMode, OnSelchangeCOMBOTriggerMode)
	ON_CBN_KILLFOCUS(IDC_COMBO_Frequency, OnKillfocusCOMBOFrequency)
	ON_BN_CLICKED(IDC_CHECK_VoltRange0, OnCHECKVoltRange)
	ON_EN_CHANGE(IDC_EDIT_TrigWindow, OnChangeEDITTrigWindow)
	ON_EN_CHANGE(IDC_EDIT_TrigLevelVolt, OnChangeEDITTrigLevelVolt)
	ON_BN_CLICKED(IDC_CHECK_VoltRange1, OnCHECKVoltRange)
	ON_BN_CLICKED(IDC_CHECK_VoltRange2, OnCHECKVoltRange)
	ON_BN_CLICKED(IDC_CHECK_VoltRange3, OnCHECKVoltRange)
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
	// ��ʼ���û������Ӿ�����		
	CString str;

	m_Combo_TriggerSource.SetCurSel(ADPara.TriggerSource);
	m_Combo_TriggerMode.SetCurSel(ADPara.TriggerMode);	// ������ʽ����
	str.Format(_T("%d") , ADPara.TrigLevelVolt);
	m_Edit_TrigLevelVolt.SetWindowText(str);
	str.Format(_T("%d") , ADPara.TrigWindow);
	m_Edit_TrigWindow.SetWindowText(str);


	UpdateData(FALSE);

	m_Combo_TriggerDir.SetCurSel(ADPara.TriggerDir); // ��ʼ���ⴥ������
	m_Combo_TriggerType.SetCurSel(ADPara.TriggerType); // ������ʽ(��ƽ|����)
	m_Static_Offset.SetWindowText(_T("0"));
//	m_Slider_Offset.SetRange(0, 4046); // ��ʼ���������ƫ�Ʒ�Χ

	m_Slider_Offset.SetRange(0, 4096/MAX_CHANNEL_COUNT-1); // ��ʼ���������ƫ�Ʒ�Χ
	

	// ��ʼ������Ƶ��-----------------------------------------------------------	

//	m_Combo_Frequency.AddString(_T("1000000 ");
	m_Combo_Frequency.AddString(_T("800000 "));
	m_Combo_Frequency.AddString(_T("400000 "));
	m_Combo_Frequency.AddString(_T("300000 ")); 
	m_Combo_Frequency.AddString(_T("200000 "));
	m_Combo_Frequency.AddString(_T("150000 "));
	m_Combo_Frequency.AddString(_T("100000 "));
	m_Combo_Frequency.AddString(_T("80000 "));
	m_Combo_Frequency.AddString(_T("50000 "));
	m_Combo_Frequency.AddString(_T("40000 "));
	m_Combo_Frequency.AddString(_T("25000 "));
	m_Combo_Frequency.AddString(_T("10000 "));
	m_Combo_Frequency.AddString(_T("1000 "));
	m_nFrequency = ADPara.Frequency; // ȡ��Ƶ��ֵ
	SetFreqComboSel(m_nFrequency);   // ����Ͽ���ѡ��Ƶ��

	m_Combo_ClockSource.SetCurSel(ADPara.ClockSource); // ��ʼ��ʱ��Դ

	SetStatusBar();

	m_Combo_VoltRange0.SetCurSel(ADPara.InputRange[0]);
	m_Combo_VoltRange1.SetCurSel(ADPara.InputRange[1]);
	m_Combo_VoltRange2.SetCurSel(ADPara.InputRange[2]);
	m_Combo_VoltRange3.SetCurSel(ADPara.InputRange[3]);

	m_Combo_VoltRange0.EnableWindow(TRUE);
	m_Combo_VoltRange1.EnableWindow(TRUE);
	m_Combo_VoltRange2.EnableWindow(TRUE);
	m_Combo_VoltRange3.EnableWindow(TRUE);

	m_Edit_TrigLevelVolt.EnableWindow(TRUE);
	
	m_Check_VoltRange0.SetCheck(1);
	m_Check_VoltRange1.SetCheck(1);
	m_Check_VoltRange2.SetCheck(1);
	m_Check_VoltRange3.SetCheck(1);
	ADPara.bChannelArray[0] = TRUE; 
	ADPara.bChannelArray[1] = TRUE;
	ADPara.bChannelArray[2] = TRUE; 
	ADPara.bChannelArray[3] = TRUE;

// 	for(int nInputRange = 0; nInputRange < 4; nInputRange++)
// 	{
// 		for (int Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
// 		{
//  			PCI9757_LoadZeroValAD(theApp.m_hDevice, &ADZeroLsb[nInputRange][Channel], nInputRange, Channel);
// 		}
// 	}
	int Channel;
	for (Channel=4; Channel<MAX_AD_CHANNELS; Channel++)
	{
		ADPara.bChannelArray[Channel] = FALSE;
	}

	for (Channel=0; Channel<MAX_AD_CHANNELS; Channel++) // ��ʼ����ѹ����
	{
		SetInputRangeEx(ADPara.InputRange[Channel], Channel);

	}

	m_bInitialUpdate = TRUE;
	theApp.m_pADDoc->m_hADParaCfgView = this;
	// ������������
}

//###########################################################################
// ���ô���Դ (�ڴ������ⴥ��)
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
	m_Combo_TriggerDir.EnableWindow(Flag);    // �ⴥ����	
	m_Combo_TriggerSource.EnableWindow(Flag); // ������ʽ
	m_Combo_ClockSource.EnableWindow(Flag);
	m_Edit_TrigWindow.EnableWindow(Flag);

	if (!m_Check_VoltRange0.GetCheck()) m_Combo_VoltRange0.EnableWindow(FALSE);
	else m_Combo_VoltRange0.EnableWindow(Flag);

	if (!m_Check_VoltRange1.GetCheck()) m_Combo_VoltRange1.EnableWindow(FALSE);
	else m_Combo_VoltRange1.EnableWindow(Flag);

	if (!m_Check_VoltRange2.GetCheck()) m_Combo_VoltRange2.EnableWindow(FALSE);
	else m_Combo_VoltRange2.EnableWindow(Flag);

	if (!m_Check_VoltRange3.GetCheck()) m_Combo_VoltRange3.EnableWindow(FALSE);
	else m_Combo_VoltRange3.EnableWindow(Flag);



	m_Check_VoltRange0.EnableWindow(Flag);
	m_Check_VoltRange1.EnableWindow(Flag);
	m_Check_VoltRange2.EnableWindow(Flag);
	m_Check_VoltRange3.EnableWindow(Flag);

	m_Combo_TriggerDir.EnableWindow(Flag); 
	m_Combo_TriggerType.EnableWindow(Flag);
	m_Edit_TrigLevelVolt.EnableWindow(Flag);
}

//######################################################################################
// �ⴥ����������
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

void CADParaCfgView::OnCustomdrawSLIDEROffset(NMHDR* pNMHDR, LRESULT* pResult) 
{ 
	if (m_bInitialUpdate) // �����������OnInitialUpdate�����ѵ��ú�
	{ 
		CSliderCtrl* pSliderOffset = (CSliderCtrl*)(GetDlgItem(IDC_SLIDER_Offset));	
		int Pos = pSliderOffset->GetPos();  // ȡ�û�����λ��
		pSliderOffset->SetPos(Pos);
		
		gl_Offset = Pos;	// gloffset ��־�ڻ������е�ƫ��
		CString str;
		str.Format(_T("%d") , gl_Offset);
		CStatic* pOffsetStatic = (CStatic*)(GetDlgItem(IDC_STATIC_Offset));
		pOffsetStatic->SetWindowText(str);
		CSysApp* pApp = (CSysApp*)AfxGetApp();	
		CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndWave));
		CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndDigit));
		
		pWaveView->OnDrawPolyLine(); // ���²�����ͼ
		pDigitView->RedrawWindow();  // ����������ͼ
		
		CADFrm* pADFrm = (CADFrm*)pWaveView->GetParentFrame();
		CEdit* pEditOffset = (CEdit*)(pADFrm->m_wndShowStatus).GetDlgItem(IDC_EDIT_Offset); // ȡ��IDC_STATIC_ShowXY���
		str.Format(_T("%d") , gl_Offset);
		pEditOffset->SetWindowText(str);
		*pResult = 0;
	}
}

void CADParaCfgView::SetInputRangeEx(int nInputRange, int nChannel)
{
	CADDoc* pDoc = theApp.m_pADDoc; 
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit));

	switch(nInputRange)
	{
	case PCI9757_INPUT_N10000_P10000mV:
		gl_InputRange[nChannel] = 20000;
		gl_PerLsbVolt[nChannel] = (float)(20000.0 / AD_LSB_COUNT);
		pWaveView->m_ADScopeCtrl.SetRange(-10000, 10000, nChannel);
		for (nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
		{
			ADCHZeroLsb[nChannel] = (SHORT)ADZeroLsb[0][nChannel];
		}
		break;
	case PCI9757_INPUT_N5000_P5000mV:
		gl_InputRange[nChannel] = 10000;
		gl_PerLsbVolt[nChannel] = (float)(10000.0 / AD_LSB_COUNT);
		pWaveView->m_ADScopeCtrl.SetRange(-5000, 5000, nChannel);
		for (nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
		{
			ADCHZeroLsb[nChannel] = (SHORT)ADZeroLsb[1][nChannel];
		}
		break;

	case PCI9757_INPUT_N2500_P2500mV:
		gl_InputRange[nChannel] = 5000;
		gl_PerLsbVolt[nChannel] = (float)(5000.0 / AD_LSB_COUNT);
		pWaveView->m_ADScopeCtrl.SetRange(-2500, 2500, nChannel);
		for (nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
		{
			ADCHZeroLsb[nChannel] = (SHORT)ADZeroLsb[1][nChannel];
		}
		break;

	case PCI9757_INPUT_0_P10000mV:
		gl_InputRange[nChannel] = 10000;
		gl_PerLsbVolt[nChannel] = (float)(10000.0 / AD_LSB_COUNT);
		pWaveView->m_ADScopeCtrl.SetRange(0, 10000, nChannel);
		for (nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
		{
			ADCHZeroLsb[nChannel] = (SHORT)ADZeroLsb[1][nChannel];
		}
		break;

	case PCI9757_INPUT_0_P5000mV:
		gl_InputRange[nChannel] = 5000;
		gl_PerLsbVolt[nChannel] = (float)(5000.0 / AD_LSB_COUNT);
		pWaveView->m_ADScopeCtrl.SetRange(0, 5000, nChannel);
		for (nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
		{
			ADCHZeroLsb[nChannel] = (SHORT)ADZeroLsb[1][nChannel];
		}
		break;
		

	default:
		break;
	}
}

void CADParaCfgView::SetInputRange(int nInputRange, int nChannel)
{
	CADDoc* pDoc = theApp.m_pADDoc; 
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 

	SetInputRangeEx(nInputRange, nChannel);
	ADPara.InputRange[nChannel] = nInputRange;

	pDigitView->Invalidate();
}

// ����ͨ��0�ĵ�ѹ����
void CADParaCfgView::OnSelchangeCOMBOVoltRange() 
{
	int nChannel = 0;
	int nInputRange = m_Combo_VoltRange0.GetCurSel();
	SetInputRange(nInputRange, nChannel);

}

// ����ͨ��1�ĵ�ѹ����
void CADParaCfgView::OnSelchangeCOMBOVoltRange1() 
{
	int nChannel = 1;
	int nInputRange = m_Combo_VoltRange1.GetCurSel();
	SetInputRange(nInputRange, nChannel);	
}

// ����ͨ��2�ĵ�ѹ����
void CADParaCfgView::OnSelchangeCOMBOVoltRange2() 
{
	int nChannel = 2;
	int nInputRange = m_Combo_VoltRange2.GetCurSel();
	SetInputRange(nInputRange, nChannel);	
}

// ����ͨ��3�ĵ�ѹ����
void CADParaCfgView::OnSelchangeCOMBOVoltRange3() 
{
	int nChannel = 3;
	int nInputRange = m_Combo_VoltRange3.GetCurSel();
	SetInputRange(nInputRange, nChannel);	
}



//######################################################################
// �����ⴥ������ (��ƽ�������ش���)
void CADParaCfgView::OnSelchangeCOMBOTriggerType() 
{
	// TODO: Add your control notification handler code here
	ADPara.TriggerType = m_Combo_TriggerType.GetCurSel();
}

// ����(ѡ��)����ͨ���ĵ�ѹ����
void CADParaCfgView::SetInputRangeComboSel(int* pInputRange)
{
	m_Combo_VoltRange0.SetCurSel(pInputRange[0]);
	m_Combo_VoltRange1.SetCurSel(pInputRange[1]);
	m_Combo_VoltRange2.SetCurSel(pInputRange[2]);
	m_Combo_VoltRange3.SetCurSel(pInputRange[3]);

	ADPara.InputRange[0] = pInputRange[0];
	ADPara.InputRange[1] = pInputRange[1];
	ADPara.InputRange[2] = pInputRange[2];
	ADPara.InputRange[3] = pInputRange[3];

}

//#########################################################################
// ʱ��Դѡ�� (��ʱ�ӻ���ʱ��)
void CADParaCfgView::OnSelchangeCOMBOClockSource() 
{
	ADPara.ClockSource = m_Combo_ClockSource.GetCurSel();
	if (ADPara.ClockSource == PCI9757_CLOCKSRC_IN)
		m_Combo_Frequency.EnableWindow(TRUE);
	else
		m_Combo_Frequency.EnableWindow(FALSE);
}

 
// ͨ��Ƶ��ֵ������Ƶ���������ѡ��
void CADParaCfgView::SetFreqComboSel(int nFrequecy)
{
	CString strFrequecy;
	strFrequecy.Format(_T("%d ") , nFrequecy); // %d����Ŀո�����!
	m_Combo_Frequency.SelectString(-1, strFrequecy);
 	ADPara.Frequency = nFrequecy;
	
}

// ���õ��µ�״̬��
void CADParaCfgView::SetStatusBar()
{
	CString strTimeDiv, strFrequency;
	float fTimePixel, fTimeDiv;
	m_Combo_Frequency.GetWindowText(strFrequency);
	long nFrequency = (long)_tcstol(strFrequency, NULL, 10);
	ADPara.Frequency = nFrequency;
	fTimePixel = (float)(1000000.0/nFrequency); // ��ÿ����������(uS)
	fTimeDiv = fTimePixel * 50; // ��ÿ��������(ÿ���Ϊ50������)
	if (fTimeDiv < 1000.0)
		strTimeDiv.Format(_T("%7.2f uS/Div") , fTimeDiv);

	if ((fTimeDiv >= 1000.0) && (fTimeDiv < 1000000.0))
		strTimeDiv.Format(_T("%7.2f mS/Div") , fTimeDiv/1000);

	if (fTimeDiv >= 1000000.0)
		strTimeDiv.Format(_T("%7.2f S/Div") , fTimeDiv/1000000);

	CStatic* pStatic = (CStatic*)m_pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv);
    pStatic->SetWindowText(strTimeDiv);
}

//void CADParaCfgView::OnCHECKClockOutput() 
//{
	// TODO: Add your control notification handler code here
	//UpdateData();
/*	ADPara.bClockOutput = m_bEnableClockOutput; // �Ƿ�����ʱ�����	*/
//}


void CADParaCfgView::OnKillfocusCOMBOFrequency() 
{
	// TODO: Add your control notification handler code here
	CString strFrequency;
	m_Combo_Frequency.GetWindowText(strFrequency);
	long nFrequency = (long)_tcstol(strFrequency, NULL, 10);
	ADPara.Frequency = nFrequency; // ����Ƶ��ֵ
	SetStatusBar(); // ���õ��µ�״̬��	
}

void CADParaCfgView::OnCHECKVoltRange() 
{
	const MSG* pMsg = GetCurrentMessage();
	for (int nChannel=0; nChannel<MAX_AD_CHANNELS; nChannel++)
	{
		if (pMsg->lParam == (LPARAM)m_Button_VoltRange(nChannel)->m_hWnd)
		{			
			if (m_Button_VoltRange(nChannel)->GetCheck())
			{
				m_Combo_VoltRange(nChannel)->EnableWindow(TRUE);
				ADPara.bChannelArray[nChannel]	= TRUE; // ����ͨ��0����
				gl_nChannelCount++;
			}
			else
			{		
				m_Combo_VoltRange(nChannel)->EnableWindow(FALSE);				
				ADPara.bChannelArray[nChannel]	= FALSE; // ��ֹͨ��0����
				gl_nChannelCount--;
				
				if (!CHECKVoltRange())
				{
					gl_nChannelCount=1;
					MsgBox(_T("������ѡ��1��ͨ��"));					
					m_Combo_VoltRange(nChannel)->EnableWindow(TRUE);
					ADPara.bChannelArray[nChannel]	= TRUE; // ����ͨ��0����
					m_Button_VoltRange(nChannel)->SetCheck(1);
				}		
			}	
			
		}
	}

	m_Slider_Offset.SetRange(0, 4096/gl_nChannelCount-1); // ��ʼ���������ƫ�Ʒ�Χ


	if (m_Check_VoltRange0.GetCheck())
	{
		m_Combo_VoltRange0.EnableWindow(TRUE);
		ADPara.bChannelArray[0]	= TRUE; // ����ͨ��0����
	}
	else
	{		
		m_Combo_VoltRange0.EnableWindow(FALSE);

		ADPara.bChannelArray[0]	= FALSE; // ��ֹͨ��0����

		if (!CHECKVoltRange())
		{
			MsgBox(_T("������ѡ��1��ͨ��"));
			m_Combo_VoltRange0.EnableWindow(TRUE);			
			ADPara.bChannelArray[0]	= TRUE; // ����ͨ��0����				
			m_Check_VoltRange0.SetCheck(1);	
		}		
	}	
}


BOOL CADParaCfgView::CHECKVoltRange()
{
	int Channel;
	for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
	{
		if (ADPara.bChannelArray[Channel] == TRUE) break;
	}
	if (Channel == 4) return FALSE;
	
	return TRUE;
}

void CADParaCfgView::OnChangeEDITTrigWindow() 
{
	CString strTrigWindow;
	m_Edit_TrigWindow.GetWindowText(strTrigWindow);
	long TrigWindow = _tcstol(strTrigWindow, NULL, 10);
	if (TrigWindow > 0 && TrigWindow <255)
	{
		ADPara.TrigWindow = TrigWindow;
	}
	else
	{
		AfxMessageBox(_T("�������"));
		ADPara.TrigWindow = 1;
		m_Edit_TrigWindow.SetWindowText(_T("1"));
	}
	
}

void CADParaCfgView::OnChangeEDITTrigLevelVolt() 
{
	CString strTrigVolt;
	LONG oldTrigLevelVolt, newTrigLevelVolt;
	oldTrigLevelVolt = ADPara.TrigLevelVolt;
	m_Edit_TrigLevelVolt.GetWindowText(strTrigVolt);
	newTrigLevelVolt = _tcstol(strTrigVolt, NULL, 10);
	if (newTrigLevelVolt<0 || newTrigLevelVolt>10000)
	{
		AfxMessageBox(_T("���뷶Χ:0~10000mV") ,MB_ICONINFORMATION);
		ADPara.TrigLevelVolt = oldTrigLevelVolt;
		strTrigVolt.Format(_T("%d") , ADPara.TrigLevelVolt);
		m_Edit_TrigLevelVolt.SetWindowText(strTrigVolt);
	}
	else
	{
		ADPara.TrigLevelVolt = newTrigLevelVolt;
	}

}



