// ParaCfgView.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"
#include "ParaCfgView.h"
#include "ADWaveView.h"
#include "ADDigitView.h"
#include "math.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL gl_bRt = FALSE;
/////////////////////////////////////////////////////////////////////////////
// CParaCfgView
///////////////////////////////
int glOffset=0;
int glBufferID=0;
extern int gl_InputRange[MAX_CHANNEL_COUNT];
int gl_MLength = 0;
int gl_NLength = 0;
LONG gl_lFrequency;




#define TIMER_TRIGER	3

extern CSysApp theApp;
///////////////////////////////
IMPLEMENT_DYNCREATE(CParaCfgView, CFormView)
CParaCfgView::CParaCfgView()
: CFormView(CParaCfgView::IDD)
{
	CountItem = 4;
	m_bInitial = FALSE;
	//{{AFX_DATA_INIT(CParaCfgView)
	m_TrigLevelVolt = 0;
	m_nChannelDataBit = 0;
	//}}AFX_DATA_INIT
}

CParaCfgView::~CParaCfgView()
{
}

void CParaCfgView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CParaCfgView)
	DDX_Control(pDX, IDC_EDIT_TrigInterval, m_Edit_TrigInterva);
	DDX_Control(pDX, IDC_CHECK_TimerTrig, m_Check_TimerTrig);
	DDX_Control(pDX, IDC_BUTTON_SoftTirg, m_Button_SoftTirg);
	DDX_Control(pDX, IDC_COMBO_SyncTrigSignal, m_Combo_SyncTrigSignal);
	DDX_Control(pDX, IDC_CHECK_bMasterEn, m_Check_bMasterEn);
	DDX_Control(pDX, IDC_CHECK_bClkOutEn, m_Check_bClkOutEn);
	DDX_Control(pDX, IDC_CHECK_bTrigOutEn, m_Check_bTrigOutEn);
	DDX_Control(pDX, IDC_EDIT_SampleLenth, m_Edit_nSampleLenth);
	//DDX_Control(pDX, IDC_CHECK_bAvailableTrig, m_Check_bAvailableTrig);
	DDX_Control(pDX, IDC_EDIT_TrigLevelVolt, m_Edit_TrigLevelVolt);
	DDX_Control(pDX, IDC_EDIT_TrigCount, m_Edit_TrigCount);
	DDX_Control(pDX, IDC_EDIT_TrigWindow, m_Edit_TrigWindow);
	DDX_Control(pDX, IDC_EDIT_FreqDivision, m_Edit_FreqDivision);
	DDX_Control(pDX, IDC_EDIT_Frequency, m_Edit_Frequency);
	DDX_Control(pDX, IDC_EDIT_TrigOutWidth, m_Edit_TrigOutWidth);
	DDX_Control(pDX, IDC_EDIT_N_Length, m_Edit_N_Length);
	DDX_Control(pDX, IDC_EDIT_M_Length, m_Edit_M_Length);
	DDX_Control(pDX, IDC_COMBO_ReferenceClock, m_Combo_ReferenceClock);
	DDX_Control(pDX, IDC_COMBO_TrigOutPolarity, m_Combo_TrigOutPolarity);
	DDX_Control(pDX, IDC_COMBO_ClkOutSel, m_Combo_ClkOutSel);
	DDX_Control(pDX, IDC_COMBO_TriggerMode, m_ComBo_TriggerMode);
	DDX_Control(pDX, IDC_COMBO_OutTriggerDir, m_Combo_TriggerDir);
	DDX_Control(pDX, IDC_COMBO_TriggerSource, m_Combo_TriggerSource);
	DDX_Control(pDX, IDC_STATIC_Offset, m_Static_Offset);
	DDX_Control(pDX, IDC_SPIN_BufferID, m_Spin_BufferID);
	DDX_Control(pDX, IDC_SLIDER_Offset, m_Slider_Offset);
	DDX_Control(pDX, IDC_EDIT_BufferID, m_Edit_BufferID);
	DDX_Control(pDX, IDC_CHECK_Auto, m_Check_Auto);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_TimeBaseClock, m_Combo_TimeBaseClock);
	DDX_Control(pDX, IDC_COMBO_SampleMode, m_ComBo_SampleMode);
	DDX_Control(pDX, IDC_LIST_CHParam, m_List_CHParam);
	DDX_Control(pDX, IDC_COMBO_PFISel, m_Combo_PFISel);
	DDX_Control(pDX, IDC_CHECK_bSaveFile, m_Check_bSaveFile);
	DDX_Control(pDX, IDC_EDIT_FilePath, m_Edit_FilePath);
}

BEGIN_MESSAGE_MAP(CParaCfgView, CFormView)
//{{AFX_MSG_MAP(CParaCfgView)
	ON_CBN_SELCHANGE(IDC_COMBO_TriggerSource, OnSelchangeCOMBOTriggerSource)	
	ON_WM_KILLFOCUS()
	ON_CBN_SELCHANGE(IDC_COMBO_OutTriggerDir, OnSelchangeCOMBOOutTriggerDir)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Offset, OnCustomdrawSLIDEROffset)
	ON_EN_CHANGE(IDC_EDIT_BufferID, OnChangeEDITBufferID)
	ON_BN_CLICKED(IDC_CHECK_Auto, OnCHECKAuto)
	ON_CBN_SELCHANGE(IDC_COMBO_TriggerMode, OnSelchangeCOMBOTriggerMode)
//	ON_CBN_KILLFOCUS(IDC_COMBO_bAvailableTrig, OnKillfocusCOMBObAvailableTrig)
	ON_CBN_SELCHANGE(IDC_COMBO_ReferenceClock, OnSelchangeCOMBOReferenceClock)
//	ON_BN_CLICKED(IDC_CHECK_bAvailableTrig, OnCHECKbAvailableTrig)
	ON_EN_CHANGE(IDC_EDIT_TrigLevelVolt, OnChangeEDITTrigLevelVolt)
	ON_EN_CHANGE(IDC_EDIT_M_Length, OnChangeEDITMLength)
	ON_EN_CHANGE(IDC_EDIT_N_Length, OnChangeEDITNLength)
	ON_BN_CLICKED(IDC_CHECK_bMasterEn, OnCHECKbMasterEn)
	ON_BN_CLICKED(IDC_CHECK_bClkOutEn, OnCHECKbClkOutEn)
	ON_BN_CLICKED(IDC_CHECK_bTrigOutEn, OnCHECKbTrigOutEn)
	ON_CBN_SELCHANGE(IDC_COMBO_SyncTrigSignal, OnSelchangeCOMBOSyncTrigSignal)
	ON_BN_CLICKED(IDC_BUTTON_SoftTirg, OnBUTTONSoftTirg)
	ON_EN_KILLFOCUS(IDC_EDIT_TrigInterval, OnKillfocusEDITTrigInterval)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_EN_KILLFOCUS(IDC_EDIT_M_Length, &CParaCfgView::OnEnKillfocusEditMLength)
	ON_EN_KILLFOCUS(IDC_EDIT_N_Length, &CParaCfgView::OnEnKillfocusEditNLength)
	ON_EN_CHANGE(IDC_EDIT_TrigCount, &CParaCfgView::OnEnChangeEditTrigcount)
	ON_EN_CHANGE(IDC_EDIT_TrigWindow, &CParaCfgView::OnEnChangeEditTrigWindow)
	ON_CBN_SELCHANGE(IDC_COMBO_ClkOutSel, &CParaCfgView::OnCbnSelchangeComboClkoutsel)
	ON_CBN_SELCHANGE(IDC_COMBO_TrigOutPolarity, &CParaCfgView::OnCbnSelchangeComboTrigoutpolarity)
	ON_EN_CHANGE(IDC_EDIT_TrigOutWidth, &CParaCfgView::OnEnChangeEditTrigoutwidth)
	ON_EN_KILLFOCUS(IDC_EDIT_TrigOutWidth, &CParaCfgView::OnEnKillfocusEditTrigoutwidth)
	ON_EN_KILLFOCUS(IDC_EDIT_TrigWindow, &CParaCfgView::OnEnKillfocusEditTrigwindow)
	ON_EN_KILLFOCUS(IDC_EDIT_FreqDivision, &CParaCfgView::OnEnKillfocusEditFreqdivision)
	ON_EN_KILLFOCUS(IDC_EDIT_Frequency, &CParaCfgView::OnEnKillfocusEditFrequency)
	ON_CBN_SELCHANGE(IDC_COMBO_TimeBaseClock, &CParaCfgView::OnCbnSelchangeComboTimebaseclock)
	ON_CBN_SELCHANGE(IDC_COMBO_SampleMode, &CParaCfgView::OnCbnSelchangeComboSamplemode)
	ON_CBN_SELCHANGE(IDC_COMBO_PFISel, &CParaCfgView::OnCbnSelchangeComboPfisel)
	ON_BN_CLICKED(IDC_CHECK_bSaveFile, &CParaCfgView::OnBnClickedCheckbsavefile)
	ON_EN_KILLFOCUS(IDC_EDIT_FilePath, &CParaCfgView::OnEnKillfocusEditFilepath)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CParaCfgView diagnostics

#ifdef _DEBUG
void CParaCfgView::AssertValid() const
{
	CFormView::AssertValid();
}

void CParaCfgView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CParaCfgView message handlers

void CParaCfgView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	CSysApp *pApp = (CSysApp *)AfxGetApp();
	CADDoc *pDoc = (CADDoc *)pApp->m_pADDoc;
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	pDoc->m_hWndADCfg = this->m_hWnd;
	pApp->m_hParaCfgView = this->m_hWnd;
	m_lSingleCH = 0xFF;

	// ��ʼ���û������Ӿ�����
	CString str;
	OutAnalogValve = 2000;
	m_ComBo_TriggerMode.SetCurSel(ADPara.TriggerMode);
	m_ComBo_SampleMode.SetCurSel(ADPara.SampleMode);

	m_Combo_TriggerDir.SetCurSel(ADPara.TriggerDir);
	m_Static_Offset.SetWindowText(_T("0"));
	m_Spin_BufferID.SetRange(0, MAX_SEGMENT-1);	
	m_Spin_BufferID.SetBuddy(&m_Edit_BufferID);
	m_Spin_BufferID.SetPos(0);

	m_Slider_Offset.SetRange(0, MAX_RAM_SIZE/MAX_CHANNEL_COUNT-1); // ��ʼ������ƫ�Ʒ�Χ
	
	OldADPara = ADPara;

	m_List_CHParam.InsertColumn(0, L"ͨ��", LVCFMT_LEFT, 38, 70);
	m_List_CHParam.InsertColumn(1, L"ʹ��", LVCFMT_LEFT, 38, 70);
	m_List_CHParam.InsertColumn(2, L"��Χ", LVCFMT_LEFT, 45, 70);
	m_List_CHParam.InsertColumn(3, L"���", LVCFMT_LEFT, 38, 70);
	if (gl_ADMainInfo.nSupportImped == 1)
	{
		m_List_CHParam.InsertColumn(4, L"�迹", LVCFMT_LEFT, 38, 70);
	}
	DWORD type = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_INFOTIP |
		LVS_EX_ONECLICKACTIVATE | LVS_EX_SUBITEMIMAGES;

	m_List_CHParam.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(type));


	//CString string;

	str.Format(_T("%d"), ADPara.FreqDivision);
	m_Edit_FreqDivision.SetWindowText(str);

	gl_lFrequency = gl_ADMainInfo.nBaseRate/ADPara.FreqDivision;
	str.Format(_T("%d"), gl_lFrequency);
	m_Edit_Frequency.SetWindowText(str);


	m_Edit_FilePath.SetWindowText(ADPara.chFileName);
	m_Check_bSaveFile.SetCheck(ADPara.bSaveFile);

	m_Edit_FilePath.EnableWindow(ADPara.bSaveFile);
	

	// ��ʼ����������
	gl_nSampleLenth = ADPara.M_Length + ADPara.N_Length;
	str.Format(_T("%u"), gl_nSampleLenth);
	m_Edit_nSampleLenth.SetWindowText(str);


	// ��ʼ��״̬����TimeDiv��(�����Ƶ�����Ӧ)
	float fFrequency, fTimePixel, fTimeDiv;
	
	fFrequency = gl_nSampleLenth/1000000.0;	// ȡ��Ƶ�ʴ�С(MHz)
	fTimePixel = float(1000.0 / fFrequency); // ÿ�������ص�֮���ʱ��(nS)
    fTimeDiv = fTimePixel*50;
	if(fTimeDiv < 1000.0)
		str.Format(_T("%7.2f nS/Div"), fTimeDiv);
	if(fTimeDiv>=1000.0 && fTimeDiv<1000000.0)
		str.Format(_T("%7.2f uS/Div"), fTimeDiv/1000);
	if(fTimeDiv>=1000000.0 && fTimeDiv<1000000000.0)
		str.Format(_T("%7.2f mS/Div"), fTimeDiv/1000000);
	if(fTimeDiv >= 1000000000.0)
		str.Format(_T("%7.2f  S/Div"), fTimeDiv/1000000000);
	CStatic *pStatic = (CStatic *)m_pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv);
    pStatic->SetWindowText(str);

	CString strMsg;
	ULONG nChanIdx = 0;
	
	m_List_CHParam.DeleteAllItems();
	for(nChanIdx=0; nChanIdx<gl_ADMainInfo.nChannelCount; nChanIdx++)
	{
		strMsg.Format(L"%d", nChanIdx);
		m_List_CHParam.InsertItem(nChanIdx, strMsg);

		strMsg.Format(L"AI%d", nChanIdx);
		m_List_CHParam.SetItemText(nChanIdx, LIST_COL_CHANNEL, strMsg);
		
		int bChannelEn = ADPara.bChannelArray[nChanIdx];
		m_List_CHParam.SetItemText(nChanIdx, LIST_COL_EN, g_strChannelEn[bChannelEn]);
		

		
		int nSampleRange = ADPara.InputRange[nChanIdx];
		m_List_CHParam.SetItemText(nChanIdx, LIST_COL_SAMPRANGE, g_strAIRange[nSampleRange]);
		/*if (theApp.m_nRevisionID == PCIE8502D_RID && nSampleRange == ACTS1000_INPUT_N5000_P5000mV)
			m_List_CHParam.SetItemText(nChanIdx, LIST_COL_SAMPRANGE, L"��10V");
		else
			m_List_CHParam.SetItemText(nChanIdx, LIST_COL_SAMPRANGE, g_strAIRange[nSampleRange]);*/
		OnSelchangeCOMBOVoltRange();



		int nCouplingType = ADPara.CouplingType[nChanIdx];
		m_List_CHParam.SetItemText(nChanIdx, LIST_COL_REFGND, g_strRefGround[nCouplingType]);
		


		if (gl_ADMainInfo.nSupportImped == 1)
		{
			int nInputImped = ADPara.InputImped[nChanIdx];
			m_List_CHParam.SetItemText(nChanIdx, LIST_COL_IMPED, g_strInputImped[nInputImped]);
		}
 	}
	m_Combo_TriggerSource.ResetContent();
	if (gl_ADMainInfo.nSupportPFI == 0)
	{
		m_Combo_TriggerSource.AddString(L"�������");
		m_Combo_TriggerSource.AddString(L"DTR");
		m_Combo_TriggerSource.AddString(L"ͬ���źŴ���");
	}
	else
	{
		m_Combo_TriggerSource.AddString(L"�������");
		m_Combo_TriggerSource.AddString(L"���ִ���PFI");
		m_Combo_TriggerSource.AddString(L"ͬ���źŴ���");
	}
	for (int nCH=0; nCH<gl_ADMainInfo.nChannelCount; nCH++)
	{
		strMsg.Format(L"ͨ��%d����", nCH);
		m_Combo_TriggerSource.AddString(strMsg);
	}
	
	m_Combo_ReferenceClock.ResetContent();
	m_Combo_TimeBaseClock.ResetContent();
	m_Combo_TimeBaseClock.AddString(L"�ڲ�ʱ��");
	if (gl_ADMainInfo.nSupportExtClk == 1)
	{
		m_Combo_ReferenceClock.AddString(L"����ʱ��");
		m_Combo_ReferenceClock.AddString(L"�ⲿ10M");
		if (gl_ADMainInfo.nSupportPXIE100M == 1)
		{
			m_Combo_ReferenceClock.AddString(L"PXI_CLK10M");
			m_Combo_ReferenceClock.AddString(L"PXIe_CLK100M");
		}
		else
		{
			m_Combo_ReferenceClock.AddString(L"����10M");
		}

		m_Combo_TimeBaseClock.AddString(L"�ⲿʱ��");
	}
	else
	{
		m_Combo_ReferenceClock.AddString(L"����ʱ��");
		m_Combo_ReferenceClock.AddString(L"�ⲿ10M(��֧��)");
		if (gl_ADMainInfo.nSupportPXIE100M == 1)
		{
			m_Combo_ReferenceClock.AddString(L"PXI_CLK10M");
			m_Combo_ReferenceClock.AddString(L"PXIe_CLK100M");
		}
		else
		{
			m_Combo_ReferenceClock.AddString(L"����10M");
		}
	}

	m_ComBo_TriggerMode.SetCurSel(ADPara.TriggerMode);		// ��ʼ���ⴥ��ģʽ
	m_Combo_TriggerSource.SetCurSel(ADPara.TriggerSource);	// ��ʼ���ⴥԴ
	m_Combo_TriggerDir.SetCurSel(ADPara.TriggerDir);		// ��ʼ����������
//	m_Check_bAvailableTrig.SetCheck(ADPara.bAvailableTrig);	// ��ʼ�������¼���ǰ
	m_TrigLevelVolt = ADPara.TrigLevelVolt;		// ������ƽ
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TrigLevelVolt); // ������ƽ��ֵ
	

	m_Combo_ReferenceClock.SetCurSel(ADPara.ReferenceClock);
	m_Combo_TimeBaseClock.SetCurSel(ADPara.TimeBaseClock);
	m_Combo_PFISel.SetCurSel(ADPara.PFISel);	
	m_Combo_TrigOutPolarity.SetCurSel(ADPara.TrigOutPolarity);
	m_Combo_ClkOutSel.SetCurSel(ADPara.ClkOutSel);
	m_Check_bMasterEn.SetCheck(ADPara.bMasterEn);
	m_Check_bClkOutEn.SetCheck(ADPara.bClkOutEn);
	m_Check_bTrigOutEn.SetCheck(ADPara.bTrigOutEn);
	m_Combo_SyncTrigSignal.SetCurSel(ADPara.SyncTrigSignal);

	if (gl_ADMainInfo.nSupportPFI == 1)
	{
		m_Combo_PFISel.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_PFI)->ShowWindow(SW_SHOW);
	}

	if (gl_ADMainInfo.nSupportClkOut == 1)
	{
		m_Check_bClkOutEn.ShowWindow(SW_SHOW);
		m_Combo_ClkOutSel.ShowWindow(SW_SHOW);
	}



	if (ADPara.TriggerMode == ACTS1000_TRIGMODE_POST || ADPara.TriggerMode == ACTS1000_TRIGMODE_PRE)
	{
		m_Edit_M_Length.EnableWindow(FALSE);
		ADPara.M_Length = 0;
	}

	if (ADPara.TriggerMode == ACTS1000_TRIGMODE_POST || ADPara.TriggerMode == ACTS1000_TRIGMODE_DELAY)
	{
		if (ADPara.SampleMode == ACTS1000_SAMPMODE_CONTINUOUS)
		{
			m_Edit_TrigCount.EnableWindow(FALSE);
		}
		else
			m_Edit_TrigCount.EnableWindow(TRUE);
	}
	else
	{
		m_Edit_TrigCount.EnableWindow(FALSE);
	}

	if (ADPara.M_Length > 645) // ��ʼ��M�κ�N�γ���(645Ϊ������ʾ�Ŀ��)
	{
		ADPara.M_Length = 0;
		ADPara.N_Length = gl_nSampleLenth - 1;
	}
	else
	{
		gl_MLength = ADPara.M_Length;
		gl_NLength = ADPara.N_Length;
	}
	str.Format(_T("%u"), gl_MLength);
	m_Edit_M_Length.SetWindowText(str);
	str.Format(_T("%u"), gl_NLength);
	m_Edit_N_Length.SetWindowText(str);

	for (int nChannel = 0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
	{

		switch(ADPara.InputRange[nChannel])
		{	
		case ACTS1000_INPUT_N1000_P1000mV:
			gl_InputRange[nChannel] = 2000;
			gl_PerLsbVoltM[nChannel] = (float)(2000.0 / gl_fLsbCount);
			pWaveView->m_ADScopeCtrl.SetRange(-1000, 1000, nChannel);
			break;	
		case ACTS1000_INPUT_N10000_P10000mV:
			gl_InputRange[nChannel] = 20000;
			gl_PerLsbVoltM[nChannel] = (float)(20000.0 / gl_fLsbCount);
			pWaveView->m_ADScopeCtrl.SetRange(-10000, 10000, nChannel);
			break;

		case ACTS1000_INPUT_N5000_P5000mV:
				gl_InputRange[nChannel] = 10000;
				gl_PerLsbVoltM[nChannel] = (float)(10000.0 / gl_fLsbCount);
				pWaveView->m_ADScopeCtrl.SetRange(-5000, 5000, nChannel);
			break;	
		default:
			break;
		}
	}

	gl_ScreenVolume = gl_InputRange[0];
	str.Format(_T("%d"), ADPara.TrigLevelVolt);
	m_Edit_TrigLevelVolt.SetWindowText(str);
// 	if (ADPara.TrigCount<1)
// 		ADPara.TrigCount= 1;
	str.Format(_T("%d"), ADPara.TrigCount);
	m_Edit_TrigCount.SetWindowText(str);
	str.Format(_T("%d"), ADPara.TrigWindow);
	m_Edit_TrigWindow.SetWindowText(str);
	str.Format(_T("%d"), ADPara.TrigOutWidth);
	m_Edit_TrigOutWidth.SetWindowText(str);
	m_Check_Auto.SetCheck(TRUE); // �Զ��ɼ�	
	m_Check_TimerTrig.SetCheck(1);
	m_Edit_TrigInterva.SetWindowText(_T("100"));
	OnSelchangeCOMBOTriggerSource();


	gl_bAutoCollect = TRUE;
	m_bInitial = TRUE;
	OnCHECKbClkOutEn();
	OnCHECKbTrigOutEn();
	OnSelchangeCOMBOReferenceClock();
}

void CParaCfgView::OnSelchangeCOMBOTriggerSource() 
{
	// TODO: Add your control notification handler code here
	ADPara.TriggerSource = m_Combo_TriggerSource.GetCurSel();

	if (ADPara.TriggerSource == ACTS1000_TRIGMODE_SOFT)
	{
		m_Button_SoftTirg.EnableWindow(TRUE);
		m_Check_TimerTrig.EnableWindow(TRUE);
		m_Edit_TrigInterva.EnableWindow(TRUE);
	}
	else
	{
		m_Button_SoftTirg.EnableWindow(FALSE);
		m_Check_TimerTrig.EnableWindow(FALSE);
		m_Edit_TrigInterva.EnableWindow(FALSE);
	}
}

void CParaCfgView::EnableWindows(BOOL Flag)
{

	//m_List_CHParam.EnableWindow(Flag);

	m_List_CHParam.EnableWindow(Flag);

	m_Check_TimerTrig.EnableWindow(Flag);
	m_Edit_TrigInterva.EnableWindow(Flag);	
	
	if (ADPara.TriggerSource != ACTS1000_TRIGMODE_SOFT)
	{
		m_Button_SoftTirg.EnableWindow(FALSE);
		m_Check_TimerTrig.EnableWindow(FALSE);
		m_Edit_TrigInterva.EnableWindow(FALSE);
	}
	else
	{
		m_Button_SoftTirg.EnableWindow(TRUE);
	}


	m_Edit_M_Length.EnableWindow(Flag);
	if (ADPara.TriggerMode == ACTS1000_TRIGMODE_POST || ADPara.TriggerMode == ACTS1000_TRIGMODE_PRE)
	{
		m_Edit_M_Length.EnableWindow(FALSE);
		ADPara.M_Length = 0;
		m_Edit_M_Length.SetWindowText(L"0");
	}

	m_Edit_TrigCount.EnableWindow(Flag);
	if (Flag)
	{
		if (ADPara.TriggerMode == ACTS1000_TRIGMODE_POST || ADPara.TriggerMode == ACTS1000_TRIGMODE_DELAY)
		{
			if (ADPara.SampleMode == ACTS1000_SAMPMODE_CONTINUOUS)
			{
				m_Edit_TrigCount.EnableWindow(FALSE);
			}
			else
				m_Edit_TrigCount.EnableWindow(TRUE);
		}
		else
		{
			m_Edit_TrigCount.EnableWindow(FALSE);
		}
	}

// 	if (Flag)
// 	{
// 		if (m_lSingleCH == 0xFF)
// 		{
// 		}
// 		else
// 		{
// 			for (int i=0;i<MAX_CHANNEL_COUNT;i++)
// 			{
// 				if (m_lSingleCH != i)
// 				{
// 					m_pCheck_CH(i)->EnableWindow(FALSE);
// 				}
// 			}
// 		}
// 	}

	m_Edit_N_Length.EnableWindow(Flag);
	m_Edit_TrigWindow.EnableWindow(Flag);
	m_ComBo_TriggerMode.EnableWindow(Flag);
	m_ComBo_SampleMode.EnableWindow(Flag);
	m_Combo_TriggerSource.EnableWindow(Flag);
	m_Combo_TriggerDir.EnableWindow(Flag);


	m_Edit_FreqDivision.EnableWindow(Flag);
	if (ADPara.TimeBaseClock == ACTS1000_TBCLK_IN)
	{
		m_Edit_Frequency.EnableWindow(Flag);
		m_Combo_ReferenceClock.EnableWindow(Flag);
	}
	else
	{
		m_Edit_Frequency.EnableWindow(FALSE);
		m_Combo_ReferenceClock.EnableWindow(FALSE);
	}

//	m_Check_bAvailableTrig.EnableWindow(Flag);
	m_Edit_TrigLevelVolt.EnableWindow(Flag);
	m_Combo_ReferenceClock.EnableWindow(Flag);
	m_Combo_TimeBaseClock.EnableWindow(Flag);
	m_Combo_PFISel.EnableWindow(Flag);
	if (ADPara.bClkOutEn)
	{
		m_Combo_ClkOutSel.EnableWindow(Flag);
	}
	else{
		m_Combo_ClkOutSel.EnableWindow(FALSE);
	}
	if (ADPara.bTrigOutEn)
	{
		m_Combo_TrigOutPolarity.EnableWindow(Flag);
		m_Edit_TrigOutWidth.EnableWindow(Flag);
	}
	else{
		m_Combo_TrigOutPolarity.EnableWindow(FALSE);
		m_Edit_TrigOutWidth.EnableWindow(FALSE);
	}
	m_Check_bMasterEn.EnableWindow(Flag);
	m_Check_bClkOutEn.EnableWindow(Flag);
	m_Check_bTrigOutEn.EnableWindow(Flag);
	m_Combo_SyncTrigSignal.EnableWindow(Flag);
	if(nProcessMode == SAVE_MODE) // ����Ǵ��̲���
	{
		m_Edit_BufferID.EnableWindow(Flag);
		m_Spin_BufferID.EnableWindow(Flag);
	}
	if (Flag)
	{
		OnCbnSelchangeComboPfisel();
		m_Edit_FilePath.EnableWindow(ADPara.bSaveFile);
	}
	else
	{
		m_Edit_FilePath.EnableWindow(FALSE);

	}
	m_Check_bSaveFile.EnableWindow(Flag);
}


void CParaCfgView::OnKillFocus(CWnd* pNewWnd) 
{
	CFormView::OnKillFocus(pNewWnd);
	// TODO: Add your message handler code here	
}

void CParaCfgView::OnSelchangeCOMBOOutTriggerDir() 
{
	// TODO: Add your control notification handler code here
	ADPara.TriggerDir = m_Combo_TriggerDir.GetCurSel();
}


void CParaCfgView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)  
{
	// TODO: Add your specialized code here and/or call the base class
	if(lHint==1 || lHint==2) return;
	CView::OnUpdate(pSender, lHint, pHint);
}

void CParaCfgView::OnCustomdrawSLIDEROffset(NMHDR* pNMHDR, LRESULT* pResult) 
{ 
	// TODO: Add your control notification handler code here
	if(m_bInitial)
	{ 
		CSliderCtrl* pSliderOffset=(CSliderCtrl*)(GetDlgItem(IDC_SLIDER_Offset));	
		int Pos = pSliderOffset->GetPos();  // ȡ�û�����λ��
		pSliderOffset->SetPos(Pos);
		
		glOffset = Pos;	//gloffset ��־�ڻ������е�ƫ��
		CString str;
		str.Format(_T("%d"), glOffset);
		CStatic* pStatic = (CStatic*)(GetDlgItem(IDC_STATIC_Offset));
		pStatic->SetWindowText(str);
		CSysApp* pApp = (CSysApp*)AfxGetApp();	
		CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndWave));
		CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndDigit));
		
		pWaveView->OnDrawPolyLine(); // ���²�����ͼ
		pDigitView->RedrawWindow();  // ����������ͼ
		
//		CADFrame* pADFrame = (CADFrame*)pWaveView->GetParentFrame();
//		CEdit* pEditOffset=(CEdit*)(pADFrame->m_wndShowStatus).GetDlgItem(IDC_EDIT_Offset); 
//		int Count = glOffset + WavePointX;  // ƫ���ܵ���
//		str.Format(_T("%d"), Count);
		
		*pResult = 0;
	}
}

BOOL gl_bCollected = FALSE; //  �Ƿ��Ѿ����й�һ�βɼ�
void CParaCfgView::OnChangeEDITBufferID() 
{
	CString string;
	CEdit* pEditBufferID = (CEdit*)(GetDlgItem(IDC_EDIT_BufferID));	
	pEditBufferID->GetWindowText(string); 
	
	glBufferID = _tcstol(string,NULL,10); // �ı�ADBuffer�Ļ���κ�
	if(glBufferID<0 || glBufferID>2)
	{	
		glBufferID = 0;
		pEditBufferID->SetWindowText(_T("0"));
	}
	CSysApp* pApp=(CSysApp*)AfxGetApp();	
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndWave));
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndDigit));
	CADDoc *pADDoc = (CADDoc *)pApp->m_pADDoc;

	
		pDigitView->RedrawWindow();  // ����������ͼ
		pWaveView->OnDrawPolyLine(); // ���²�����ͼ
}

void CParaCfgView::OnCHECKAuto() // �Զ��ɼ�����
{
	if(m_Check_Auto.GetCheck())	
		gl_bAutoCollect = TRUE;
	else 
		gl_bAutoCollect = FALSE;
}


// ����ͨ��0�ĵ�ѹ����
void CParaCfgView::OnSelchangeCOMBOVoltRange() 
{
	// TODO: Add your control notification handler code here
	CSysApp *pApp=(CSysApp*)AfxGetApp();
	CADDoc* pDoc=pApp->m_pADDoc;
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave));
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit));
//	int nInputRange;
	
	//const MSG* pMsg = GetCurrentMessage();
	for (int nCH=0; nCH<MAX_CHANNEL_COUNT; nCH++)
	{	
		switch(ADPara.InputRange[nCH])
		{				
		case ACTS1000_INPUT_N1000_P1000mV:
			gl_ScreenVolume = 2000;
			gl_InputRange[nCH]= 2000;
			gl_PerLsbVoltM[nCH] = (float)(2000.0 / gl_fLsbCount);
			pWaveView->m_ADScopeCtrl.SetRange(-1000, 1000, nCH);
			break;
		case ACTS1000_INPUT_N10000_P10000mV:
			gl_ScreenVolume = 20000;
			gl_InputRange[nCH] = 20000;
			gl_PerLsbVoltM[nCH] = (float)(20000.0 / gl_fLsbCount);
			pWaveView->m_ADScopeCtrl.SetRange(-10000, 10000, nCH);
			break;

		case ACTS1000_INPUT_N5000_P5000mV:
			gl_ScreenVolume = 10000;
			gl_InputRange[nCH] = 10000;
			gl_PerLsbVoltM[nCH] = (float)(10000.0 / gl_fLsbCount);
			pWaveView->m_ADScopeCtrl.SetRange(-5000, 5000, nCH);
			break;	
		default:
			break;
		}
	}

	if (m_lSingleCH != 0xFF)
	{

		for (int i=0;i<MAX_CHANNEL_COUNT;i++)
		{
			if (m_lSingleCH == i)
			{
				switch(ADPara.InputRange[i])
				{				
				case ACTS1000_INPUT_N1000_P1000mV:
					gl_ScreenVolume = 2000;
					gl_InputRange[i]= 2000;
					gl_PerLsbVoltM[i] = (float)(2000.0 / gl_fLsbCount);
					pWaveView->m_ADScopeCtrl.SetRange(-1000, 1000, i);
					break;
				case ACTS1000_INPUT_N10000_P10000mV:
					
					gl_ScreenVolume = 20000;
					gl_InputRange[i] = 20000;
					gl_PerLsbVoltM[i] = (float)(20000.0 / gl_fLsbCount);
					pWaveView->m_ADScopeCtrl.SetRange(-10000, 10000, i);
					break;
				case ACTS1000_INPUT_N5000_P5000mV:	
					gl_ScreenVolume = 10000;
					gl_InputRange[i] = 10000;
					gl_PerLsbVoltM[i] = (float)(10000.0 / gl_fLsbCount);
					pWaveView->m_ADScopeCtrl.SetRange(-5000, 5000, i);
					break;	
				default:
					break;
				}
				break;
			}
		}
	}

	pDigitView->Invalidate(); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
}

void CParaCfgView::SetInputRangeComboSel(int *pInputRange)
{
//	m_Combo_VoltRange0.SetCurSel(pInputRange[0]);

//	ADPara.InputRange = pInputRange[0];
}

// ���ô���ģʽ
void CParaCfgView::OnSelchangeCOMBOTriggerMode() 
{
	// TODO: Add your control notification handler code here
	
	ADPara.TriggerMode = m_ComBo_TriggerMode.GetCurSel();

	if (ADPara.SampleMode == ACTS1000_SAMPMODE_CONTINUOUS)
	{
		if (ADPara.TriggerMode==ACTS1000_TRIGMODE_MIDL || ADPara.TriggerMode==ACTS1000_TRIGMODE_PRE)
		{
			AfxMessageBox(L"���������� ����ģʽֻ��Ϊ�󴥷���Ӳ����ʱ����!");
			m_ComBo_TriggerMode.SetCurSel(ACTS1000_TRIGMODE_POST);
			ADPara.TriggerMode = ACTS1000_TRIGMODE_POST;
			return;
		}
	}


	if (ADPara.TriggerMode == ACTS1000_TRIGMODE_POST || ADPara.TriggerMode == ACTS1000_TRIGMODE_PRE)
	{
		m_Edit_M_Length.EnableWindow(FALSE);
		m_Edit_M_Length.SetWindowText(_T("0"));
		ADPara.M_Length = 0;
	}
	else
	{
		m_Edit_M_Length.EnableWindow(TRUE);
	}


	if (ADPara.TriggerMode == ACTS1000_TRIGMODE_POST || ADPara.TriggerMode == ACTS1000_TRIGMODE_DELAY)
	{
		if (ADPara.SampleMode == ACTS1000_SAMPMODE_CONTINUOUS)
		{
			m_Edit_TrigCount.EnableWindow(FALSE);
		}
		else
			m_Edit_TrigCount.EnableWindow(TRUE);
	}
	else
	{
		m_Edit_TrigCount.EnableWindow(FALSE);
		m_Edit_TrigCount.EnableWindow(FALSE);
		m_Edit_TrigCount.SetWindowText(_T("1"));
		ADPara.TrigCount = 1;
	}
	OnEnKillfocusEditNLength();
	
}

// void CParaCfgView::OnKillfocusCOMBObAvailableTrig() 
// {
// 	// TODO: Add your control notification handler code here
// //	CComboBox* pTrig = (CComboBox*)GetDlgItem(IDC_COMBO_bAvailableTrig);
// //	ADPara.bAvailableTrig = pTrig->GetCurSel();
// }

void CParaCfgView::OnSelchangeCOMBOReferenceClock() 
{
	// TODO: Add your control notification handler code here
	ADPara.ReferenceClock = m_Combo_ReferenceClock.GetCurSel();
// 	if(ADPara.ReferenceClock != ACTS1000_CLOCKSRC_OUT)
// 	{
// 		m_Edit_Frequency.EnableWindow(TRUE);
// 	}
// 	else
// 	{
// 		m_Edit_Frequency.EnableWindow(FALSE);
// 	}
}



// void CParaCfgView::OnCHECKbAvailableTrig() 
// {
// 	// TODO: Add your control notification handler code here
// //	ADPara.bAvailableTrig = m_Check_bAvailableTrig.GetCheck();
// }

void CParaCfgView::OnChangeEDITTrigLevelVolt() 
{
	// TODO: Add your control notification handler code here
	CString str;
	this->m_Edit_TrigLevelVolt.GetWindowText(str);
	LONG TrigLevelVolt = (LONG)_tcstol(str, NULL, 10);

// 	if(TrigLevelVolt<0 || TrigLevelVolt>10000)
// 	{	
// 		MessageBox(_T("������ƽ�ķ�ΧΪ:(0 ~ 10000)mV"));	
// 		TrigLevelVolt =1000; 	
// 		this->m_Edit_TrigLevelVolt.SetWindowText(_T("1000"));
// 	}
// 	ADPara.TrigLevelVolt = TrigLevelVolt;		
	LONG VoltTopRange, VoltBottomRange;	
	LONG lCH = ADPara.TriggerSource-ACTS1000_TRIGSRC_CH0;
	if (lCH<0 || lCH>(ACTS1000_AD_MAX_CHANNELS-1))
	{
		lCH = 0;
	}

	switch(ADPara.InputRange[lCH])	// ��ѹ����
	{		
	case ACTS1000_INPUT_N1000_P1000mV: // ��1000����
		VoltTopRange = 1000;
		VoltBottomRange = -1000;
		break;
	case ACTS1000_INPUT_N10000_P10000mV: // ��5000����
			VoltTopRange = 10000;
			VoltBottomRange = -10000;
			break;
	case ACTS1000_INPUT_N5000_P5000mV:
			VoltTopRange = 5000;
			VoltBottomRange = -5000;
		break;
	}	


	if(TrigLevelVolt<VoltBottomRange || TrigLevelVolt>VoltTopRange)
	{
		str.Format(L"������ƽ�ķ�ΧΪ��%dmV��%dmV", VoltBottomRange,VoltTopRange);
		MessageBox(str, L"Information", MB_ICONWARNING | MB_OK);
		if(ADPara.TrigLevelVolt<VoltBottomRange || ADPara.TrigLevelVolt>VoltTopRange)
		{
			ADPara.TrigLevelVolt =VoltTopRange; 
		}
		str.Format(L"%ld", ADPara.TrigLevelVolt);
		this->m_Edit_TrigLevelVolt.SetWindowText(str);
	}
	else
	{
		ADPara.TrigLevelVolt = TrigLevelVolt;
	}
}

void CParaCfgView::OnChangeEDITMLength() 
{
	// TODO: Add your control notification handler code here

	
	CSysApp *pApp = (CSysApp *)AfxGetApp();
	CADDoc *pDoc = (CADDoc *)pApp->m_pADDoc;
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 

	CString strMLength;
	CString strNLength;
	ULONG nMLength = 0;
	ULONG nNLength = 0;
	int i = 0;
	
	CEdit* pEditM_Length = (CEdit*)(GetDlgItem(IDC_EDIT_M_Length));	// ȡ��IDC_EDIT_M_Length���
	pEditM_Length->GetWindowText(strMLength);
	CEdit* pEditN_Length = (CEdit*)(GetDlgItem(IDC_EDIT_N_Length));	// ȡ��IDC_EDIT_M_Length���
	pEditN_Length->GetWindowText(strNLength);
	nMLength = _tcstoul(strMLength, NULL, 10);
	nNLength = _tcstoul(strMLength, NULL, 10);

	

	{
		pWaveView->m_ADScopeCtrl.m_OldMoveX1 = pWaveView->m_ADScopeCtrl.m_OldMoveX1 + (nMLength - ADPara.M_Length);		
		gl_MLength = nMLength;
		ADPara.M_Length = nMLength;
	}

	pWaveView->m_ADScopeCtrl.InvalidateCtrl();


	gl_nSampleLenth = gl_NLength/* + gl_MLength*/;
	CString str;
	str.Format(_T("%u"), gl_nSampleLenth);
	m_Edit_nSampleLenth.SetWindowText(str);

}

void CParaCfgView::OnChangeEDITNLength() 
{
	// TODO: Add your control notification handler code here
// 	CString strNLength;
// 	CString strMLength;
// 	ULONG nNLength = 0;	
// 	ULONG nMLength = 0;
// 	int i = 0;
// 
// 	CEdit* pEditN_Length = (CEdit*)(GetDlgItem(IDC_EDIT_N_Length));	// ȡ��IDC_EDIT_M_Length���
// 	pEditN_Length->GetWindowText(strNLength);
// 	CEdit* pEditM_Length = (CEdit*)(GetDlgItem(IDC_EDIT_M_Length));	// ȡ��IDC_EDIT_M_Length���
// 	pEditM_Length->GetWindowText(strMLength);
// 	nNLength = _tcstoul(strNLength, NULL, 10);
// 	nMLength = _tcstol(strNLength, NULL, 10);
// 	
// 	i = 0;
// 	for (int nCH=0; nCH<gl_ADMainInfo.nChannelCount; nCH++)
// 	{
// 		if (ADPara.bChannelArray[nCH])
// 		{
// 			i++;
// 		}
// 	}
// 
// 	ULONGLONG ulLen=nNLength;
// 	ulLen = ulLen*gl_nChannelCount*ADPara.TrigCount;
// 	if (ulLen % 2048 != 0)
// 	{
// 		if (ulLen>2048)
// 		{
// 			ulLen = ulLen-ulLen%2048;
// 			nNLength = ulLen/gl_nChannelCount/ADPara.TrigCount;
// 		}
// 		else
// 		{
// 			nNLength = 2048/gl_nChannelCount/ADPara.TrigCount;
// 			if (nNLength*gl_nChannelCount*ADPara.TrigCount % 2048 != 0)
// 			{
// 				nNLength = 2048/gl_nChannelCount;
// 			}
// 		}
// 		strNLength.Format(_T("%d"), nNLength);
// 		pEditN_Length->SetWindowText(strNLength);
// 		pEditN_Length->SetSel(0, -1, FALSE);
// 	}

//?? 
// 	if (nNLength + gl_MLength > gl_DDR2Lenth/i/2  || nNLength<0)
// 	{
// 		if (gl_MLength > gl_DDR2Lenth/i/2)
// 		{
// 			gl_MLength = 1024;
// 			strMLength.Format(_T("%d"), gl_MLength);
// 			pEditM_Length->SetWindowText(strMLength);
// 			pEditM_Length->SetSel(0, -1, FALSE);
// 		}
// 		strNLength.Format(_T("M�γ���Ϊ%d,N�η�Χ:0~%d"), gl_MLength, gl_DDR2Lenth/i/2 - gl_MLength);
// 		AfxMessageBox(strNLength);
// 		
// 		if (gl_NLength<0)
// 		{
// 			gl_NLength = 1024;
// 		}
// 		else
// 		{
// 			gl_NLength = gl_DDR2Lenth/i/2 - gl_MLength;
// 
// 		}
// 		strNLength.Format(_T("%d"), gl_NLength);
// 		pEditN_Length->SetWindowText(strNLength);
// 		pEditN_Length->SetSel(0, -1, FALSE);
// 	}
// 	else
// 	{
// 		gl_NLength = nNLength;
// 		ADPara.N_Length = nNLength;
// 	}


// 	gl_NLength = nNLength;
// 	ADPara.N_Length = nNLength;
// 
// 	gl_nSampleLenth = gl_MLength + gl_NLength;
// 	CString str;
// 	str.Format(_T("%u"), gl_nSampleLenth);
// 	m_Edit_nSampleLenth.SetWindowText(str);

}

void CParaCfgView::OnCHECKbMasterEn() 
{
	// TODO: Add your control notification handler code here
	ADPara.bMasterEn = m_Check_bMasterEn.GetCheck();
}

void CParaCfgView::OnCHECKbClkOutEn() 
{
	// TODO: Add your control notification handler code here
	ADPara.bClkOutEn = m_Check_bClkOutEn.GetCheck();
	if (ADPara.bClkOutEn)
	{
		m_Combo_ClkOutSel.EnableWindow(TRUE);
	}
	else{
		m_Combo_ClkOutSel.EnableWindow(FALSE);
	}
}

void CParaCfgView::OnCHECKbTrigOutEn() 
{
	// TODO: Add your control notification handler code here
	ADPara.bTrigOutEn = m_Check_bTrigOutEn.GetCheck();
	if (ADPara.bTrigOutEn)
	{
		m_Combo_TrigOutPolarity.EnableWindow(TRUE);
		m_Edit_TrigOutWidth.EnableWindow(TRUE);
	}
	else{
		m_Combo_TrigOutPolarity.EnableWindow(FALSE);
		m_Edit_TrigOutWidth.EnableWindow(FALSE);
	}
}

void CParaCfgView::OnSelchangeCOMBOSyncTrigSignal() 
{
	// TODO: Add your control notification handler code here
	ADPara.SyncTrigSignal = m_Combo_SyncTrigSignal.GetCurSel();
}

void CParaCfgView::OnBUTTONSoftTirg() 
{
	// TODO: Add your control notification handler code here
	
	if (gl_bDeviceADRun)
	{
		ACTS1000_SetDeviceTrigAD(theApp.m_hDevice);
	}
}

void CParaCfgView::OnKillfocusEDITTrigInterval() 
{
	// TODO: Add your control notification handler code here
	CString str;
	m_Edit_TrigInterva.GetWindowText(str);	
	long lTmp = _tcstol(str, NULL, 10);
	if (lTmp<1)
	{
		m_Edit_TrigInterva.SetWindowText(_T("1"));
	}
	

}

void CParaCfgView::StartAD()
{
	
	gl_bAutoTrig = FALSE;
	if (m_Check_TimerTrig.GetCheck() && ADPara.TriggerSource == ACTS1000_TRIGMODE_SOFT)
	{
		gl_bAutoTrig = TRUE;
		CString str;
		m_Edit_TrigInterva.GetWindowText(str);	
		SetTimer(TIMER_TRIGER,_tcstol(str, NULL, 10), NULL);
		
	}
}

void CParaCfgView::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (nIDEvent == TIMER_TRIGER)
	{
		
		if (gl_bDeviceADRun)
		{
			ACTS1000_SetDeviceTrigAD(theApp.m_hDevice);
		}
		else
		{
			KillTimer(TIMER_TRIGER);
		}
		
	}

	CFormView::OnTimer(nIDEvent);
}

void CParaCfgView::OnEnKillfocusEditMLength()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
// 	CEdit* pEditM_Length = (CEdit*)(GetDlgItem(IDC_EDIT_M_Length));	// ȡ��IDC_EDIT_M_Length���
// 	ULONG ulTemp = 16/gl_nChannelCount;
// 	if (gl_nSampleLenth%ulTemp !=0)
// 	{
// 		if (!m_bErrorDlg)
// 		{
// 			CString str;
// 			str.Format(L"M+N�ĳ���ӦΪ%d��������",ulTemp);
// 			AfxMessageBox(str);
// 			pEditM_Length->SetFocus();
// 			pEditM_Length->SetSel(0, -1, FALSE);
// 			m_bErrorDlg = TRUE;
// 		}
// 		else
// 		{
// 			m_bErrorDlg = FALSE;
// 		}
// 
// 	}
	OnEnKillfocusEditNLength();
}

ULONGLONG gcd(ULONGLONG a, ULONGLONG b)//���Լ��
{
	while(b != 0)
	{
		ULONGLONG t = a;
		a = b;
		b = t % b;
	}
	return a;
}
ULONGLONG lcm(ULONGLONG a,ULONGLONG b) //����С������
{
	ULONGLONG x;
	x=gcd(a,b); //����gcd()����
	return a*b/x;
}

void CParaCfgView::OnEnKillfocusEditNLength()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString strNLength;
	ULONG nNLength = 0;	
	CEdit* pEditN_Length = (CEdit*)(GetDlgItem(IDC_EDIT_N_Length));	// ȡ��IDC_EDIT_M_Length���
	CEdit* pEditM_Length = (CEdit*)(GetDlgItem(IDC_EDIT_M_Length));	// ȡ��IDC_EDIT_M_Length���
	pEditN_Length->GetWindowText(strNLength);
	nNLength = _tcstoul(strNLength, NULL, 10);

	if (ADPara.SampleMode == ACTS1000_SAMPMODE_FINITE)
	{
		if (ADPara.TriggerMode==ACTS1000_TRIGMODE_MIDL)
		{
			if (ADPara.M_Length+nNLength>gl_DDR2Lenth/gl_nChannelCount/2)
			{
				nNLength = gl_DDR2Lenth/gl_nChannelCount/2-ADPara.M_Length;
			}
			if (((ADPara.M_Length+nNLength)*gl_nChannelCount)%512 !=0)
			{
				nNLength=((((ADPara.M_Length+nNLength)*gl_nChannelCount)/512+1)*512)/gl_nChannelCount-ADPara.M_Length;
			}
		}
		else if (ADPara.TriggerMode==ACTS1000_TRIGMODE_PRE)
		{
			if (nNLength>gl_DDR2Lenth/gl_nChannelCount/2)
			{
				nNLength = gl_DDR2Lenth/gl_nChannelCount/2;
			}
			if ((nNLength*gl_nChannelCount)%512 !=0)
			{
				nNLength=(((nNLength*gl_nChannelCount)/512+1)*512)/gl_nChannelCount;
			}
		}
		else
		{
			if (ADPara.TrigCount > 0)
			{
				ULONGLONG ulChushu = gl_nChannelCount*(ADPara.TrigCount);
				ULONGLONG ulLen=nNLength;
				ulLen = ulLen*ulChushu;

				if (ulLen%512 !=0)
				{

					ULONG ulCm = lcm(512,ulChushu);
					if (ulLen % ulCm != 0)
					{
						if (ulLen>ulCm)
						{
							ulLen = ulLen-ulLen%ulCm;
							nNLength = ulLen/ulChushu;
						}
						else
						{
							nNLength = ulCm/ulChushu;
						}


					}
				}
			}
		}
	}

	strNLength.Format(_T("%u"), nNLength);
	pEditN_Length->SetWindowText(strNLength);
	pEditN_Length->SetSel(0, -1, FALSE);


	gl_NLength = nNLength;
	ADPara.N_Length = nNLength;

	gl_nSampleLenth = gl_NLength;
	if (ADPara.TriggerMode==ACTS1000_TRIGMODE_MIDL)
	{
		gl_nSampleLenth = gl_NLength + gl_MLength;
	}
	CString str;
	str.Format(_T("%u"), gl_nSampleLenth);
	m_Edit_nSampleLenth.SetWindowText(str);
}

// 
// void CParaCfgView::OnCheckCh0() 
// {
// 	// TODO: Add your control notification handler code here
// 
// 	m_pCheck_CH(0)->SetCheck(1);
// 	m_pCheck_CH(1)->SetCheck(0);
// 	m_pCheck_CH(2)->SetCheck(0);
// 	m_pCheck_CH(3)->SetCheck(0);
// 	EnableVootRange();
// }
// 
// void CParaCfgView::OnCheckCh1() 
// {
// 	// TODO: Add your control notification handler code here
// 	
// 	if(!m_pCheck_CH(1)->GetCheck())
// 	{
// 		m_pCheck_CH(0)->SetCheck(1);
// 		m_pCheck_CH(1)->SetCheck(0);
// 		m_pCheck_CH(2)->SetCheck(0);
// 		m_pCheck_CH(3)->SetCheck(0);
// 	}
// 	else
// 	{
// 		m_pCheck_CH(0)->SetCheck(1);
// 		m_pCheck_CH(1)->SetCheck(1);
// 		m_pCheck_CH(2)->SetCheck(0);
// 		m_pCheck_CH(3)->SetCheck(0);
// 	}
// 	EnableVootRange();
// }
// 
// void CParaCfgView::OnCheckCh2() 
// {
// 	// TODO: Add your control notification handler code here
// 	
// 	if(!m_pCheck_CH(2)->GetCheck())
// 	{
// 		m_pCheck_CH(0)->SetCheck(1);
// 		m_pCheck_CH(1)->SetCheck(1);
// 		m_pCheck_CH(2)->SetCheck(0);
// 		m_pCheck_CH(3)->SetCheck(0);
// 	}
// 	else
// 	{
// 		m_pCheck_CH(0)->SetCheck(1);
// 		m_pCheck_CH(1)->SetCheck(1);
// 		m_pCheck_CH(2)->SetCheck(1);
// 		m_pCheck_CH(3)->SetCheck(1);
// 	}
// 	EnableVootRange();
// }
// 
// void CParaCfgView::OnCheckCh3() 
// {
// 	// TODO: Add your control notification handler code here
// 	
// 	if(!m_pCheck_CH(3)->GetCheck())
// 	{
// 		m_pCheck_CH(0)->SetCheck(1);
// 		m_pCheck_CH(1)->SetCheck(1);
// 		m_pCheck_CH(2)->SetCheck(0);
// 		m_pCheck_CH(3)->SetCheck(0);
// 	}
// 	else
// 	{
// 		m_pCheck_CH(0)->SetCheck(1);
// 		m_pCheck_CH(1)->SetCheck(1);
// 		m_pCheck_CH(2)->SetCheck(1);
// 		m_pCheck_CH(3)->SetCheck(1);
// 	}
// 	EnableVootRange();
// }
void CParaCfgView::EnableVootRange()
{


	CADDoc* pDoc=theApp.m_pADDoc;
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave));
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit));

 	ULONG ulCount = 0;
 	for (INT nCH=0; nCH<MAX_CHANNEL_COUNT; nCH++)
 	{
 		if (ADPara.bChannelArray[nCH] == TRUE)
 		{
 			nChannelArrayClient[ulCount] = nCH;
 			ulCount++;
 		}
 	}
	pDigitView->Invalidate(); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();

	OnSelchangeCOMBOVoltRange();

}

void CParaCfgView::OnEnChangeEditTrigWindow()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CFormView::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	

}


void CParaCfgView::OnEnChangeEditTrigcount()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CFormView::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CString str;
	m_Edit_TrigCount.GetWindowText(str);
	LONG TrigCount = (LONG)_tcstol(str, NULL, 10);


// 	if(TrigCount<1)
// 	{
// 		//MessageBox(str, _T("��������������ڵ���1"), MB_ICONWARNING | MB_OK);
// 		AfxMessageBox(L"��������������ڵ���1");
// 		if(ADPara.TrigCount<1)
// 		{
// 			ADPara.TrigLevelVolt =1; 
// 		}
// 		str.Format(_T("%ld"), ADPara.TrigCount);
// 		m_Edit_TrigCount.SetWindowText(str);
// 	}
	if(TrigCount<0)
	{
		AfxMessageBox(L"��������������ڵ���0");
		ADPara.TrigLevelVolt =1; 
		str.Format(_T("%ld"), ADPara.TrigCount);
		m_Edit_TrigCount.SetWindowText(str);
	}
 	else
	{
		ADPara.TrigCount = TrigCount;
	}

	OnEnKillfocusEditNLength();

}


void CParaCfgView::OnCbnSelchangeComboClkoutsel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	ADPara.ClkOutSel = m_Combo_ClkOutSel.GetCurSel();

}

void CParaCfgView::OnCbnSelchangeComboTrigoutpolarity()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ADPara.TrigOutPolarity = m_Combo_TrigOutPolarity.GetCurSel();
}

void CParaCfgView::OnEnChangeEditTrigoutwidth()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CFormView::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

}

void CParaCfgView::OnEnKillfocusEditTrigoutwidth()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString str;
	m_Edit_TrigOutWidth.GetWindowText(str);
	LONG TrigOutWidth = (LONG)_tcstol(str, NULL, 10);


	if(TrigOutWidth<50 || TrigOutWidth>50000)
	{
		//MessageBox(str, _T("���������ȷ�Χ[50,50000]"), MB_ICONWARNING | MB_OK);
		AfxMessageBox(L"���������ȷ�Χ[50,50000],����50");
		if(ADPara.TrigOutWidth<50 || ADPara.TrigOutWidth>50000)
		{
			ADPara.TrigOutWidth =50; 
		}
		str.Format(_T("%ld"), ADPara.TrigOutWidth);
		m_Edit_TrigOutWidth.SetWindowText(str);
	}
	else
	{
		ADPara.TrigOutWidth = TrigOutWidth;
	}
}

void CParaCfgView::OnEnKillfocusEditTrigwindow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_Edit_TrigWindow.GetWindowText(str);
	LONG TrigWindow = (LONG)_tcstol(str, NULL, 10);


	if(TrigWindow<0)
	{
		//MessageBox(str, _T("����������������ڵ���0"), MB_ICONWARNING | MB_OK);
		AfxMessageBox(L"����������������ڵ���0,����12.5ns,0~12Ϊ0ns,13~24Ϊ12.5ns,25~49Ϊ25ns");
		if(ADPara.TrigWindow<0)
		{
			ADPara.TrigWindow =0; 
		}
		str.Format(_T("%ld"), ADPara.TrigWindow);
		m_Edit_TrigWindow.SetWindowText(str);
	}
	else
	{
		ADPara.TrigWindow = TrigWindow;
	}
}

void CParaCfgView::OnEnKillfocusEditFreqdivision()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	CString str;
	m_Edit_FreqDivision.GetWindowText(str);
	LONG FreqDivision = (LONG)_tcstol(str, NULL, 10);


	if(FreqDivision<gl_ADMainInfo.nMinFreqDivision || FreqDivision>2147483647)
	{
		//MessageBox(str, _T("��Ƶ����Χ[1,2147483647]"), MB_ICONWARNING | MB_OK);
		str.Format(L"��Ƶ����Χ[%d,2147483647]", gl_ADMainInfo.nMinFreqDivision);
		AfxMessageBox(str);
		if(ADPara.FreqDivision<gl_ADMainInfo.nMinFreqDivision || ADPara.FreqDivision>2147483647)
		{
			ADPara.FreqDivision =gl_ADMainInfo.nMinFreqDivision; 
		}
		str.Format(_T("%d"), ADPara.FreqDivision);
		m_Edit_FreqDivision.SetWindowText(str);
	}
	else
	{
		ADPara.FreqDivision = FreqDivision;
	}
	
	gl_lFrequency = gl_ADMainInfo.nBaseRate/ADPara.FreqDivision;

	str.Format(_T("%d"), gl_lFrequency);
	m_Edit_Frequency.SetWindowText(str);



	float fFrequency = gl_lFrequency/1000000.0;  // ȡ��Ƶ��ֵ(MHz)
	float fTimePixel = float(1000.0 / fFrequency); // ��ÿ����������(nS)
	float fTimeDiv = fTimePixel*50; // ��ÿ��������(ÿ���Ϊ50������,��ÿ�����ؼ�����һ��AD��)

	if(fTimeDiv < 1000.0)
		str.Format(_T("%7.2f nS/Div"), fTimeDiv);
	if(fTimeDiv>=1000.0 && fTimeDiv<1000000.0)
		str.Format(_T("%7.2f uS/Div"), fTimeDiv/1000);
	if(fTimeDiv>=1000000.0 && fTimeDiv<1000000000.0)
		str.Format(_T("%7.2f mS/Div"), fTimeDiv/1000000);
	if(fTimeDiv >= 1000000000.0)
		str.Format(_T("%7.2f  S/Div"), fTimeDiv/1000000000);
	// ����״̬���ķ���ʱ��
	CStatic *pStatic = (CStatic *)m_pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv);
	pStatic->SetWindowText(str);	
}

void CParaCfgView::OnEnKillfocusEditFrequency()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	CString str;
	m_Edit_Frequency.GetWindowText(str);
	LONG Frequency = (LONG)_tcstol(str, NULL, 10);


	if(Frequency<10 || Frequency>gl_ADMainInfo.nMaxRate)
	{
		str.Format(L"Ƶ�ʷ�Χ[10,%d]", gl_ADMainInfo.nMaxRate);
		AfxMessageBox(str);
		if(gl_lFrequency<10 || gl_lFrequency>gl_ADMainInfo.nMaxRate)
		{
			gl_lFrequency =10; 
		}
		str.Format(_T("%d"), gl_lFrequency);
		m_Edit_Frequency.SetWindowText(str);
	}
	else
	{
		gl_lFrequency = Frequency;
	}

	ADPara.FreqDivision = gl_ADMainInfo.nBaseRate/gl_lFrequency;
	str.Format(_T("%d"), ADPara.FreqDivision);
	m_Edit_FreqDivision.SetWindowText(str);

	if ((ADPara.FreqDivision*gl_lFrequency) != gl_ADMainInfo.nBaseRate)
	{
		OnEnKillfocusEditFreqdivision();
	}


	float fFrequency = gl_lFrequency/1000000.0;  // ȡ��Ƶ��ֵ(MHz)
	float fTimePixel = float(1000.0 / fFrequency); // ��ÿ����������(nS)
	float fTimeDiv = fTimePixel*50; // ��ÿ��������(ÿ���Ϊ50������,��ÿ�����ؼ�����һ��AD��)

	if(fTimeDiv < 1000.0)
		str.Format(_T("%7.2f nS/Div"), fTimeDiv);
	if(fTimeDiv>=1000.0 && fTimeDiv<1000000.0)
		str.Format(_T("%7.2f uS/Div"), fTimeDiv/1000);
	if(fTimeDiv>=1000000.0 && fTimeDiv<1000000000.0)
		str.Format(_T("%7.2f mS/Div"), fTimeDiv/1000000);
	if(fTimeDiv >= 1000000000.0)
		str.Format(_T("%7.2f  S/Div"), fTimeDiv/1000000000);
	// ����״̬���ķ���ʱ��
	CStatic *pStatic = (CStatic *)m_pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv);
	pStatic->SetWindowText(str);	


}

void CParaCfgView::OnCbnSelchangeComboTimebaseclock()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ADPara.TimeBaseClock = m_Combo_TimeBaseClock.GetCurSel();
	if(ADPara.TimeBaseClock == ACTS1000_TBCLK_IN)
	{
		m_Edit_Frequency.EnableWindow(TRUE);
		m_Combo_ReferenceClock.EnableWindow(TRUE);
	}
	else
	{
		m_Edit_Frequency.EnableWindow(FALSE);
		m_Combo_ReferenceClock.EnableWindow(FALSE);
	}
}

void CParaCfgView::OnCbnSelchangeComboSamplemode()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ADPara.SampleMode = m_ComBo_SampleMode.GetCurSel();

	if (ADPara.SampleMode == ACTS1000_SAMPMODE_CONTINUOUS)
	{
		if (ADPara.TriggerMode==ACTS1000_TRIGMODE_MIDL || ADPara.TriggerMode==ACTS1000_TRIGMODE_PRE)
		{
			AfxMessageBox(L"���������� ����ģʽֻ��Ϊ�󴥷���Ӳ����ʱ����!");
			m_ComBo_TriggerMode.SetCurSel(ACTS1000_TRIGMODE_POST);

			ADPara.TriggerMode = ACTS1000_TRIGMODE_POST;
		}
		ADPara.TrigCount = 1;
		m_Edit_TrigCount.SetWindowText(L"1");
	}
	EnableWindows(TRUE);
	
}

void CParaCfgView::OnCbnSelchangeComboPfisel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	ADPara.PFISel = m_Combo_PFISel.GetCurSel();

	if ((ADPara.PFISel == ACTS1000_PFISEL_TRIG_OUT) || (gl_ADMainInfo.nSupportPFI==0))
	{
		m_Check_bTrigOutEn.EnableWindow(TRUE);
		if (m_Check_bTrigOutEn.GetCheck() == TRUE)
		{
			m_Combo_TrigOutPolarity.EnableWindow(TRUE);
			m_Edit_TrigOutWidth.EnableWindow(TRUE);
		}
		else
		{
			m_Combo_TrigOutPolarity.EnableWindow(FALSE);
			m_Edit_TrigOutWidth.EnableWindow(FALSE);
		}
	}
	else{
		m_Check_bTrigOutEn.EnableWindow(FALSE);
		m_Combo_TrigOutPolarity.EnableWindow(FALSE);
		m_Edit_TrigOutWidth.EnableWindow(FALSE);
	}
}

void CParaCfgView::OnBnClickedCheckbsavefile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	ADPara.bSaveFile = m_Check_bSaveFile.GetCheck();
	m_Edit_FilePath.EnableWindow(ADPara.bSaveFile);
}

void CParaCfgView::OnEnKillfocusEditFilepath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str;
	m_Edit_FilePath.GetWindowText(str);

	_stprintf_s(ADPara.chFileName, 256, _T("%s"), str);

}
