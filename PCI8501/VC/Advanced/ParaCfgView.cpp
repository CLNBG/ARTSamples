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

BOOL gl_bRt= FALSE;
/////////////////////////////////////////////////////////////////////////////
// CParaCfgView
///////////////////////////////
int glOffset=0;
int glBufferID=0;
extern int gl_InputRange[MAX_CHANNEL_COUNT];
int gl_MLength = 0;
int gl_NLength = 0;

static UINT IDC_COMBO_VoltRange[] ={
		IDC_COMBO_VoltRange0, IDC_COMBO_VoltRange1,
		IDC_COMBO_VoltRange2, IDC_COMBO_VoltRange3,
		IDC_COMBO_VoltRange4, IDC_COMBO_VoltRange5,
		IDC_COMBO_VoltRange6, IDC_COMBO_VoltRange7
};


static UINT IDC_COMBO_Gains[] ={
		IDC_COMBO_Gains0, IDC_COMBO_Gains1,
		IDC_COMBO_Gains2, IDC_COMBO_Gains3,
		IDC_COMBO_Gains4, IDC_COMBO_Gains5,
		IDC_COMBO_Gains6, IDC_COMBO_Gains7
};

#define m_pCOMBO_VoltRange(nNum)	((CComboBox*)GetDlgItem(IDC_COMBO_VoltRange[nNum]))	
#define m_pCOMBO_Gains(nNum)	((CComboBox*)GetDlgItem(IDC_COMBO_Gains[nNum]))	

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
	DDX_Control(pDX, IDC_COMBO_OutClockSource, m_Combo_OutClockSource);
	DDX_Control(pDX, IDC_CHECK_bClockSourceDir, m_Check_bClockSourceDir);
	DDX_Control(pDX, IDC_EDIT_SampleLenth, m_Edit_nSampleLenth);
	DDX_Control(pDX, IDC_EDIT_TrigLevelVolt, m_Edit_TrigLevelVolt);
	DDX_Control(pDX, IDC_EDIT_N_Length, m_Edit_N_Length);
	DDX_Control(pDX, IDC_EDIT_M_Length, m_Edit_M_Length);
	DDX_Control(pDX, IDC_COMBO_ClockSource, m_Combo_ClockSource);
	DDX_Control(pDX, IDC_COMBO_TriggerMode, m_ComBo_TriggerMode);
 	DDX_Control(pDX, IDC_COMBO_Frequence, m_Combo_Frequency);
	DDX_Control(pDX, IDC_COMBO_OutTriggerDir, m_Combo_TriggerDir);
	DDX_Control(pDX, IDC_COMBO_TriggerSource, m_Combo_TriggerSource);
	DDX_Control(pDX, IDC_STATIC_Offset, m_Static_Offset);
	DDX_Control(pDX, IDC_SPIN_BufferID, m_Spin_BufferID);
	DDX_Control(pDX, IDC_SLIDER_Offset, m_Slider_Offset);
	DDX_Control(pDX, IDC_EDIT_BufferID, m_Edit_BufferID);
	DDX_Control(pDX, IDC_CHECK_Auto, m_Check_Auto);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CParaCfgView, CFormView)
//{{AFX_MSG_MAP(CParaCfgView)
ON_CBN_SELCHANGE(IDC_COMBO_TriggerSource, OnSelchangeCOMBOTriggerSource)	
ON_WM_KILLFOCUS()
ON_CBN_SELCHANGE(IDC_COMBO_OutTriggerDir, OnSelchangeCOMBOOutTriggerDir)
ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Offset, OnCustomdrawSLIDEROffset)
ON_EN_CHANGE(IDC_EDIT_BufferID, OnChangeEDITBufferID)
ON_BN_CLICKED(IDC_CHECK_Auto, OnCHECKAuto)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange0, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_TriggerMode, OnSelchangeCOMBOTriggerMode)
	ON_CBN_KILLFOCUS(IDC_COMBO_bAvailableTrig, OnKillfocusCOMBObAvailableTrig)
	ON_CBN_SELCHANGE(IDC_COMBO_ClockSource, OnSelchangeCOMBOClockSource)
	ON_EN_CHANGE(IDC_EDIT_TrigLevelVolt, OnChangeEDITTrigLevelVolt)
	ON_CBN_SELCHANGE(IDC_COMBO_Frequence, OnSelchangeCOMBOFrequence)
	ON_CBN_EDITCHANGE(IDC_COMBO_Frequence, OnEditchangeCOMBOFrequence)
	ON_EN_CHANGE(IDC_EDIT_M_Length, OnChangeEDITMLength)
	ON_EN_CHANGE(IDC_EDIT_N_Length, OnChangeEDITNLength)
	ON_BN_CLICKED(IDC_CHECK_bClockSourceDir, OnCHECKbClockSourceDir)
	ON_CBN_SELCHANGE(IDC_COMBO_OutClockSource, OnSelchangeCOMBOOutClockSource)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains0, OnSelchangeCOMBOGains)
	ON_BN_CLICKED(IDC_BUTTON_SoftTirg, OnBUTTONSoftTirg)
	ON_WM_TIMER()
	ON_EN_KILLFOCUS(IDC_EDIT_TrigInterval, OnKillfocusEDITTrigInterval)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange1, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange2, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange3, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange4, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange5, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange6, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_VoltRange7, OnSelchangeCOMBOVoltRange)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains1, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains2, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains3, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains4, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains5, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains6, OnSelchangeCOMBOGains)
	ON_CBN_SELCHANGE(IDC_COMBO_Gains7, OnSelchangeCOMBOGains)
	//}}AFX_MSG_MAP
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

	// 初始化用户界面视觉参数
	CString str;
	OutAnalogValve = 2000;
	m_ComBo_TriggerMode.SetCurSel(ADPara.TriggerMode);


	m_Combo_TriggerDir.SetCurSel(ADPara.TriggerDir);
	m_Static_Offset.SetWindowText(L"0");
	m_Spin_BufferID.SetRange(0, MAX_SEGMENT-1);	
	m_Spin_BufferID.SetBuddy(&m_Edit_BufferID);
	m_Spin_BufferID.SetPos(0);

	m_Slider_Offset.SetRange(0, MAX_RAM_SIZE/MAX_CHANNEL_COUNT-1); // 初始化缓冲偏移范围
	m_Check_TimerTrig.SetCheck(1);
	OldADPara = ADPara;
	CString string;
	int CurSelIndex = 0;
	string.Format(L"%3.2f MHz", (ADPara.Frequency/1000000.0));
	CurSelIndex = m_Combo_Frequency.FindString(-1, string);
	if(CurSelIndex==-1) CurSelIndex = 1;
	m_Combo_Frequency.SetCurSel(CurSelIndex); 

	// 初始化采样长度
	gl_nSampleLenth = ADPara.M_Length + ADPara.N_Length;
	string.Format(L"%dK", gl_nSampleLenth);
	m_Edit_nSampleLenth.SetWindowText(str);


	// 初始化状态栏的TimeDiv框(与采样频率相对应)
	float fFrequency, fTimePixel, fTimeDiv;
	m_Combo_Frequency.GetWindowText(str);
	
	fFrequency = (float)_wtof(str.Left(5)); // 取得频率大小(MHz)
	fTimePixel = float(1000.0 / fFrequency); // 每两个像素点之间的时间(nS)
    fTimeDiv = fTimePixel*50;
	if(fTimeDiv < 1000.0)
		str.Format(L"%7.2f nS/Div", fTimeDiv);
	if(fTimeDiv>=1000.0 && fTimeDiv<1000000.0)
		str.Format(L"%7.2f uS/Div", fTimeDiv/1000);
	if(fTimeDiv>=1000000.0 && fTimeDiv<1000000000.0)
		str.Format(L"%7.2f mS/Div", fTimeDiv/1000000);
	if(fTimeDiv >= 1000000000.0)
		str.Format(L"%7.2f S/Div", fTimeDiv/1000000000);
	CStatic *pStatic = (CStatic *)m_pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv);
    pStatic->SetWindowText(str);
	
	// 初始化电压量程大小为 5000毫伏
	for (int index=0; index<MAX_CHANNEL_COUNT; index++)
	{
		m_pCOMBO_VoltRange(index)->SetCurSel(ADPara.InputRange[index]);
		m_pCOMBO_Gains(index)->SetCurSel(ADPara.Gains[index]);
	}
	m_ComBo_TriggerMode.SetCurSel(ADPara.TriggerMode);		// 初始化外触发模式
	m_Combo_TriggerSource.SetCurSel(ADPara.TriggerSource);	// 初始化外触源

	m_Combo_TriggerDir.SetCurSel(ADPara.TriggerDir);		// 初始化触发方向

	m_TrigLevelVolt = ADPara.TrigLevelVolt;		// 触发电平
	CEdit *pEdit = (CEdit*)GetDlgItem(IDC_EDIT_TrigLevelVolt); // 触发电平码值

	m_Combo_ClockSource.SetCurSel(ADPara.ClockSource);	// 初始化时钟源
	m_Check_bClockSourceDir.SetCheck(ADPara.bClockSourceDir);
	m_Combo_OutClockSource.SetCurSel(ADPara.OutClockSource);

	if (ADPara.TriggerMode == PCI8501_TRIGMODE_POST || ADPara.TriggerMode == PCI8501_TRIGMODE_PRE)
	{
		m_Edit_M_Length.EnableWindow(FALSE);
		ADPara.M_Length = 0;
	}

	if (ADPara.M_Length > 645) // 初始化M段和N段长度(645为波形显示的宽度)
	{
		ADPara.M_Length = 0;
		ADPara.N_Length = gl_nSampleLenth - 1;
	}
	else
	{
		gl_MLength = ADPara.M_Length;
		gl_NLength = ADPara.N_Length;
	}
	str.Format(L"%d", gl_MLength);
	m_Edit_M_Length.SetWindowText(str);
	str.Format(L"%d", gl_NLength);
	m_Edit_N_Length.SetWindowText(str);

	


	for (int nChannel = 0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
	{
		switch(ADPara.InputRange[nChannel])
		{
		case PCI8501_INPUT_0_P5000mV:
			gl_InputRange[nChannel] = 5000;
			gl_PerLsbVoltM[nChannel] = (float)(5000.0 / LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(0, 5000, nChannel);
			break;
		case PCI8501_INPUT_0_P10000mV:
			gl_InputRange[nChannel] = 10000;
			gl_PerLsbVoltM[nChannel] = (float)(10000.0 / LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(0, 10000, nChannel);
			break;
		case PCI8501_INPUT_N2500_P2500mV:
			gl_InputRange[nChannel] = 5000;
			gl_PerLsbVoltM[nChannel] = (float)(5000.0 / LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(-2500, 2500, nChannel);
			break;
		case PCI8501_INPUT_N5000_P5000mV:
			gl_InputRange[nChannel] = 10000;
			gl_PerLsbVoltM[nChannel] = (float)(10000.0 / LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(-5000, 5000, nChannel);
			break;
		case PCI8501_INPUT_N10000_P10000mV:
			gl_InputRange[nChannel] = 20000;
			gl_PerLsbVoltM[nChannel] = (float)(20000.0 / LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(-10000, 10000, nChannel);
			break;
		default:
			break;
		}
	}
	
	gl_ScreenVolume = gl_InputRange[0];
	str.Format(L"%d", ADPara.TrigLevelVolt);
	m_Edit_TrigLevelVolt.SetWindowText(str);

	m_Check_Auto.SetCheck(TRUE); // 自动采集

	m_Edit_TrigInterva.SetWindowText(L"100");
	OnSelchangeCOMBOTriggerSource();


	gl_bAutoCollect = TRUE;
	m_bInitial = TRUE;
}

void CParaCfgView::OnSelchangeCOMBOTriggerSource() 
{
	// TODO: Add your control notification handler code here
	ADPara.TriggerSource = m_Combo_TriggerSource.GetCurSel();
	if (ADPara.TriggerSource == PCI8501_TRIGMODE_SOFT)
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


// 	if (ADPara.TriggerSource == PCI8501_TRIGMODE_SOFT)
// 	{
// 		m_Edit_M_Length.EnableWindow(FALSE);
// 		m_Edit_M_Length.SetWindowText(L"0");
// 		ADPara.M_Length = 0;
// 	}
// 	else
// 		m_Edit_M_Length.EnableWindow(TRUE);



}

void CParaCfgView::EnableWindows(BOOL Flag)
{

	m_Edit_M_Length.EnableWindow(Flag);	


	m_Check_TimerTrig.EnableWindow(Flag);
	m_Edit_TrigInterva.EnableWindow(Flag);


	if (ADPara.TriggerSource != PCI8501_TRIGMODE_SOFT)
	{
		m_Button_SoftTirg.EnableWindow(FALSE);
		m_Check_TimerTrig.EnableWindow(FALSE);
		m_Edit_TrigInterva.EnableWindow(FALSE);
	}
	else
	{
		m_Button_SoftTirg.EnableWindow(TRUE);
	}

	if (ADPara.TriggerMode == PCI8501_TRIGMODE_POST || ADPara.TriggerMode == PCI8501_TRIGMODE_PRE)
	{
		m_Edit_M_Length.EnableWindow(FALSE);
		ADPara.M_Length = 0;
	}

	m_Combo_Frequency.EnableWindow(Flag);
	m_Edit_N_Length.EnableWindow(Flag);
	m_ComBo_TriggerMode.EnableWindow(Flag);
	m_Combo_TriggerSource.EnableWindow(Flag);
	m_Combo_TriggerDir.EnableWindow(Flag);
	m_Edit_TrigLevelVolt.EnableWindow(Flag);
	m_Combo_ClockSource.EnableWindow(Flag);
	m_Check_bClockSourceDir.EnableWindow(Flag);
	m_Combo_OutClockSource.EnableWindow(Flag);
	for (int index=0; index<MAX_CHANNEL_COUNT; index++)
	{
		m_pCOMBO_VoltRange(index)->EnableWindow(Flag);
		m_pCOMBO_Gains(index)->EnableWindow(Flag);
	}
	if(nProcessMode == SAVE_MODE) // 如果是存盘操作
	{
		m_Edit_BufferID.EnableWindow(Flag);
		m_Spin_BufferID.EnableWindow(Flag);
	}
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
		int Pos = pSliderOffset->GetPos();  // 取得滑动条位置
		pSliderOffset->SetPos(Pos);
		
		glOffset = Pos;	//gloffset 标志在缓冲区中的偏移
		CString str;
		str.Format(L"%d", glOffset);
		CStatic* pStatic = (CStatic*)(GetDlgItem(IDC_STATIC_Offset));
		pStatic->SetWindowText(str);
		CSysApp* pApp = (CSysApp*)AfxGetApp();	
		CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndWave));
		CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndDigit));
		
		pWaveView->OnDrawPolyLine(); // 更新波形视图
		pDigitView->RedrawWindow();  // 更新数字视图
		
//		CADFrame* pADFrame = (CADFrame*)pWaveView->GetParentFrame();
//		CEdit* pEditOffset=(CEdit*)(pADFrame->m_wndShowStatus).GetDlgItem(IDC_EDIT_Offset); 
//		int Count = glOffset + WavePointX;  // 偏移总点数
//		str.Format(L"%d", Count);
		
		*pResult = 0;
	}
}

BOOL gl_bCollected = FALSE; //  是否已经进行过一次采集
void CParaCfgView::OnChangeEDITBufferID() 
{
	CString string;
	CEdit* pEditBufferID = (CEdit*)(GetDlgItem(IDC_EDIT_BufferID));	
	pEditBufferID->GetWindowText(string); 
	
	glBufferID = wcstol(string,NULL,10); // 改变ADBuffer的缓冲段号
	if(glBufferID<0 || glBufferID>1)
	{	
		glBufferID = 0;
		pEditBufferID->SetWindowText(L"0");
	}
	CSysApp* pApp=(CSysApp*)AfxGetApp();	
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndWave));
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pApp->m_pADDoc->m_hWndDigit));
	CADDoc *pADDoc = (CADDoc *)pApp->m_pADDoc;
	int ReadOffsetWords = glBufferID*4096; // RAM偏移位置，以4K为单位
	LONG nRetSizeWords;
	if(m_bInitial)
	{
		if(gl_bDeviceADRun == FALSE)
		{
			if(gl_bCollected)
			{				
				for(int nADChannel=0; nADChannel<MAX_CHANNEL_COUNT; nADChannel++) 
				{   
					PCI8501_ReadDeviceAD(pApp->m_hDevice, ADBuffer[nADChannel][glBufferID], gl_ReadSizeWords, 0,&nRetSizeWords);
				}				
			}	
		}
		pDigitView->RedrawWindow();  // 更新数字视图
		pWaveView->OnDrawPolyLine(); // 更新波形视图
	}
	
}

void CParaCfgView::OnCHECKAuto() // 自动采集设置
{
	if(m_Check_Auto.GetCheck())	
		gl_bAutoCollect = TRUE;
	else 
		gl_bAutoCollect = FALSE;
}


// 设置通道0的电压量程
void CParaCfgView::OnSelchangeCOMBOVoltRange() 
{
	// TODO: Add your control notification handler code here
	CSysApp *pApp=(CSysApp*)AfxGetApp();
	CADDoc* pDoc=pApp->m_pADDoc;
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave));
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit));
    int nChannel;
	
	const MSG* pMsg = GetCurrentMessage();
	for (nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
	{
		
		if (pMsg->lParam == (LPARAM)m_pCOMBO_VoltRange(nChannel)->m_hWnd)
		{
			ADPara.InputRange[nChannel] = m_pCOMBO_VoltRange(nChannel)->GetCurSel();
			break;
		}
	}

	
	for (nChannel = 0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
	{
		switch(ADPara.InputRange[nChannel])
		{
		case PCI8501_INPUT_0_P5000mV:
			gl_InputRange[nChannel] = 5000;
			gl_ScreenVolume = 5000;
			gl_PerLsbVoltM[nChannel] = (float)(5000.0 / LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(0, 5000, nChannel);
			break;
		case PCI8501_INPUT_0_P10000mV:
			gl_InputRange[nChannel]= 10000;
			gl_ScreenVolume = 10000;
			gl_PerLsbVoltM[nChannel] = (float)(10000.0 / LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(0, 10000, nChannel);
			break;
		case PCI8501_INPUT_N2500_P2500mV:
			gl_InputRange[nChannel] = 5000;
			gl_ScreenVolume = 5000;
			gl_PerLsbVoltM[nChannel] = (float)(5000.0 / LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(-2500, 2500, nChannel);
			break;
		case PCI8501_INPUT_N5000_P5000mV:
			gl_InputRange[nChannel] = 10000;
			gl_ScreenVolume = 10000;
			gl_PerLsbVoltM[nChannel] = (float)(10000.0 / LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(-5000, 5000, nChannel);
			break;
		case PCI8501_INPUT_N10000_P10000mV:
			gl_InputRange[nChannel] = 20000;
			gl_ScreenVolume = 20000;
			gl_PerLsbVoltM[nChannel] = (float)(20000.0 / LSB_COUNT);
			pWaveView->m_ADScopeCtrl.SetRange(-10000, 10000, nChannel);
			break;
		default:
			break;
		}
		
		
	}
	nChannel = pWaveView->m_ADScopeCtrl.m_nChannelNum;

	switch(ADPara.InputRange[nChannel])
	{
	case PCI8501_INPUT_0_P5000mV:
		gl_InputRange[nChannel] = 5000;
		gl_ScreenVolume = 5000;

		gl_PerLsbVoltM[nChannel] = (float)(5000.0 / LSB_COUNT);
		pWaveView->m_ADScopeCtrl.SetRange(0, 5000, nChannel);
		break;
	case PCI8501_INPUT_0_P10000mV:
		gl_InputRange[nChannel]= 10000;
		gl_ScreenVolume = 10000;
		gl_PerLsbVoltM[nChannel] = (float)(10000.0 / LSB_COUNT);
		pWaveView->m_ADScopeCtrl.SetRange(0, 10000, nChannel);
		break;
	case PCI8501_INPUT_N2500_P2500mV:
		gl_InputRange[nChannel] = 5000;
		gl_ScreenVolume = 5000;
		gl_PerLsbVoltM[nChannel] = (float)(5000.0 / LSB_COUNT);
		pWaveView->m_ADScopeCtrl.SetRange(-2500, 2500, nChannel);
		break;
	case PCI8501_INPUT_N5000_P5000mV:
		gl_InputRange[nChannel] = 10000;
		gl_ScreenVolume = 10000;
		gl_PerLsbVoltM[nChannel] = (float)(10000.0 / LSB_COUNT);
		pWaveView->m_ADScopeCtrl.SetRange(-5000, 5000, nChannel);
		break;
	case PCI8501_INPUT_N10000_P10000mV:
		gl_InputRange[nChannel] = 20000;
		gl_ScreenVolume = 20000;
		gl_PerLsbVoltM[nChannel] = (float)(20000.0 / LSB_COUNT);
		pWaveView->m_ADScopeCtrl.SetRange(-10000, 10000, nChannel);
		break;
	default:
		break;
	}

	

	pDigitView->Invalidate(); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
}

void CParaCfgView::SetInputRangeComboSel(int *pInputRange)
{
//	m_Combo_VoltRange0.SetCurSel(pInputRange[0]);

//	ADPara.InputRange[0] = pInputRange[0];
}

// 设置触发模式
void CParaCfgView::OnSelchangeCOMBOTriggerMode() 
{
	// TODO: Add your control notification handler code here
	ADPara.TriggerMode = m_ComBo_TriggerMode.GetCurSel();


	if (ADPara.TriggerMode == PCI8501_TRIGMODE_POST || ADPara.TriggerMode == PCI8501_TRIGMODE_PRE)
	{
		m_Edit_M_Length.EnableWindow(FALSE);
		m_Edit_M_Length.SetWindowText(L"0");
		ADPara.M_Length = 0;
	}
	else
	{
		m_Edit_M_Length.EnableWindow(TRUE);
	}

}

void CParaCfgView::OnKillfocusCOMBObAvailableTrig() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pTrig = (CComboBox*)GetDlgItem(IDC_COMBO_bAvailableTrig);
}

void CParaCfgView::OnSelchangeCOMBOClockSource() 
{
	// TODO: Add your control notification handler code here
	ADPara.ClockSource = m_Combo_ClockSource.GetCurSel();
	if(ADPara.ClockSource == PCI8501_CLOCKSRC_IN)
	{
		m_Combo_Frequency.EnableWindow(TRUE);
	}
	else
	{
		m_Combo_Frequency.EnableWindow(FALSE);
	}
}




void CParaCfgView::OnChangeEDITTrigLevelVolt() 
{
	// TODO: Add your control notification handler code here
	CString str;
	this->m_Edit_TrigLevelVolt.GetWindowText(str);
	LONG TrigLevelVolt = (LONG)wcstol(str, NULL, 10);
	if(TrigLevelVolt<-10000 || TrigLevelVolt>10000)
	{
		MessageBox(L"触发电平的范围为:-10000mV～+10000mV", L"Information", MB_ICONWARNING | MB_OK);
		str.Format(L"%ld", ADPara.TrigLevelVolt);
		this->m_Edit_TrigLevelVolt.SetWindowText(str);
	}
	else
	{
		ADPara.TrigLevelVolt = TrigLevelVolt;
	}
		
}

void CParaCfgView::OnSelchangeCOMBOFrequence() 
{
	// TODO: Add your control notification handler code here
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	CString str;
	double FrequencyMHz;
	m_Combo_Frequency.SetCurSel(m_Combo_Frequency.GetCurSel());
	m_Combo_Frequency.GetWindowText(str);
	FrequencyMHz = wcstod(str, NULL);    // 保存分频数
	
	if (FrequencyMHz * 1000 * 1000>800000)
	{
		float fTemp = (float)ADPara.Frequency;
		str.Format(L"%.2f MHz", fTemp/1000/1000);
		m_Combo_Frequency.SetWindowText(str);
		return;
	}

	ADPara.Frequency = (LONG)(FrequencyMHz * 1000 * 1000);

	float fFrequency = (float)_wtof(str.Left(8));  // 取得频率值(MHz)
	float fTimePixel = float(1000.0 / fFrequency); // 求每两点间的周期(nS)
    float fTimeDiv = fTimePixel*50; // 求每大格的周期(每大格为50个象素,且每个象素即代表一个AD点)

	if(fTimeDiv < 1000.0)
		str.Format(L"%7.2f nS/Div", fTimeDiv);
	if(fTimeDiv>=1000.0 && fTimeDiv<1000000.0)
		str.Format(L"%7.2f uS/Div", fTimeDiv/1000);
	if(fTimeDiv>=1000000.0 && fTimeDiv<1000000000.0)
		str.Format(L"%7.2f mS/Div", fTimeDiv/1000000);
	if(fTimeDiv >= 1000000000.0)
		str.Format(L"%7.2f S/Div", fTimeDiv/1000000000);
	// 设置状态栏的方格时间
	CStatic *pStatic = (CStatic *)m_pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv);
    pStatic->SetWindowText(str);	
}

void CParaCfgView::OnEditchangeCOMBOFrequence() 
{
	// TODO: Add your control notification handler code here
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	CString str;
	double FrequencyMHz;
	m_Combo_Frequency.GetWindowText(str);
	FrequencyMHz = _wtof(str);    // 保存分频数
	if (FrequencyMHz * 1000 * 1000>800000)
	{
		float fTemp = (float)ADPara.Frequency;
		str.Format(L"%.2f MHz", fTemp/1000/1000);
		m_Combo_Frequency.SetWindowText(str);
		return;
	}
	ADPara.Frequency = (LONG)(FrequencyMHz * 1000 * 1000);

	float fFrequency = (float)_wtof(str.Left(8));  // 取得频率值(MHz)
	float fTimePixel = float(1000.0 / fFrequency); // 求每两点间的周期(nS)
    float fTimeDiv = fTimePixel*50; // 求每大格的周期(每大格为50个象素,且每个象素即代表一个AD点)

	if(fTimeDiv < 1000.0)
		str.Format(L"%7.2f nS/Div", fTimeDiv);
	if(fTimeDiv>=1000.0 && fTimeDiv<1000000.0)
		str.Format(L"%7.2f uS/Div", fTimeDiv/1000);
	if(fTimeDiv>=1000000.0 && fTimeDiv<1000000000.0)
		str.Format(L"%7.2f mS/Div", fTimeDiv/1000000);
	if(fTimeDiv >= 1000000000.0)
		str.Format(L"%7.2f S/Div", fTimeDiv/1000000000);
	// 设置状态栏的方格时间
	CStatic *pStatic = (CStatic *)m_pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv);
    pStatic->SetWindowText(str);
}

void CParaCfgView::OnChangeEDITMLength() 
{
	// TODO: Add your control notification handler code here

	
	CSysApp *pApp = (CSysApp *)AfxGetApp();
	CADDoc *pDoc = (CADDoc *)pApp->m_pADDoc;
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 

	CString strMLength;
	ULONG nMLength = 0;

	
	CEdit* pEditM_Length = (CEdit*)(GetDlgItem(IDC_EDIT_M_Length));	// 取得IDC_EDIT_M_Length句柄
	pEditM_Length->GetWindowText(strMLength);
	nMLength = wcstoul(strMLength, NULL, 10);

	//gl_DDR2Lenth = gl_DDR2Lenth;
	if (nMLength + gl_NLength > gl_DDR2Lenth/16 || nMLength<0)
	{
		strMLength.Format(L"M段长度为%d,M段范围:0~%d", gl_MLength, gl_DDR2Lenth/16 - gl_NLength);
		AfxMessageBox(strMLength);
		if (gl_MLength<0)
		{
			gl_MLength = 0;
		}
		strMLength.Format(L"%d", gl_MLength);
		pEditM_Length->SetWindowText(strMLength);
		pEditM_Length->SetSel(0, -1, FALSE);
	}
	else
	{
		pWaveView->m_ADScopeCtrl.m_OldMoveX1 = pWaveView->m_ADScopeCtrl.m_OldMoveX1 + (nMLength - ADPara.M_Length);		
		gl_MLength = nMLength;
		ADPara.M_Length = nMLength;
	}

	pWaveView->m_ADScopeCtrl.InvalidateCtrl();


	gl_nSampleLenth = gl_NLength + gl_MLength;
	CString str;
	str.Format(L"%d", gl_nSampleLenth);
	m_Edit_nSampleLenth.SetWindowText(str);

}

void CParaCfgView::OnChangeEDITNLength() 
{
	// TODO: Add your control notification handler code here
	CString strNLength;
	ULONG nNLength = 0;
	
	CEdit* pEditN_Length = (CEdit*)(GetDlgItem(IDC_EDIT_N_Length));	// 取得IDC_EDIT_M_Length句柄
	pEditN_Length->GetWindowText(strNLength);
	nNLength = wcstol(strNLength, NULL, 10);
	if (nNLength + gl_MLength > gl_DDR2Lenth/16  || nNLength<0)
	{
		strNLength.Format(L"N段长度为%d,N段范围:0~%d", gl_NLength, gl_DDR2Lenth/16 - gl_MLength);
		AfxMessageBox(strNLength);
		
		if (gl_NLength<0)
		{
			gl_NLength = 1024;
		}
		strNLength.Format(L"%d", gl_NLength);
		pEditN_Length->SetWindowText(strNLength);
		pEditN_Length->SetSel(0, -1, FALSE);
	}
	else
	{
		gl_NLength = nNLength;
		ADPara.N_Length = nNLength;
	}

	gl_nSampleLenth = gl_MLength + gl_NLength;
	CString str;
	str.Format(L"%d", gl_nSampleLenth);
	m_Edit_nSampleLenth.SetWindowText(str);

}


void CParaCfgView::OnCHECKbClockSourceDir() 
{
	// TODO: Add your control notification handler code here
	ADPara.bClockSourceDir = m_Check_bClockSourceDir.GetCheck();
}

void CParaCfgView::OnSelchangeCOMBOOutClockSource() 
{
	// TODO: Add your control notification handler code here
	ADPara.OutClockSource = m_Combo_OutClockSource.GetCurSel();
}

void CParaCfgView::OnSelchangeCOMBOGains() 
{
	// TODO: Add your control notification handler code here
	

	const MSG* pMsg = GetCurrentMessage();
	for (int nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
	{
		
		if (pMsg->lParam == (LPARAM)m_pCOMBO_Gains(nChannel)->m_hWnd)
		{
			ADPara.Gains[nChannel] = m_pCOMBO_Gains(nChannel)->GetCurSel();
			break;
		}
	}

}

void CParaCfgView::OnBUTTONSoftTirg() 
{
	// TODO: Add your control notification handler code here
	
	if (gl_bDeviceADRun)
	{
		PCI8501_SetDeviceTrigAD(theApp.m_hDevice);
	}

}

void CParaCfgView::StartAD()
{	

	if (m_Check_TimerTrig.GetCheck() && ADPara.TriggerSource == PCI8501_TRIGMODE_SOFT)
	{
		
		CString str;
		m_Edit_TrigInterva.GetWindowText(str);	
		SetTimer(TIMER_TRIGER,wcstol(str, NULL, 10), NULL);
		
	}
}


void CParaCfgView::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == TIMER_TRIGER)
	{
		
		if (gl_bDeviceADRun)
		{
			PCI8501_SetDeviceTrigAD(theApp.m_hDevice);
		}
		else
		{
			KillTimer(TIMER_TRIGER);
		}

	}
	CFormView::OnTimer(nIDEvent);
}

void CParaCfgView::OnKillfocusEDITTrigInterval() 
{
	// TODO: Add your control notification handler code here
	
	CString str;
	m_Edit_TrigInterva.GetWindowText(str);	
	long lTmp = wcstol(str, NULL, 10);
	if (lTmp<1)
	{
		m_Edit_TrigInterva.SetWindowText(L"1");
	}


}
