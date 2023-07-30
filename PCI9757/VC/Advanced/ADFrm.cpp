// ADFrm.cpp : implementation of the CADFrm class
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// CADFrm

IMPLEMENT_DYNCREATE(CADFrm, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CADFrm, CMDIChildWnd)
ON_COMMAND_EX(CG_ID_VIEW_DLGBAR, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_DLGBAR, OnUpdateControlBarMenu)
ON_COMMAND_EX(CG_ID_VIEW_SHOWSTATUS1, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_SHOWSTATUS1, OnUpdateControlBarMenu)
ON_COMMAND_EX(CG_ID_VIEW_SHOWSTATUS, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_SHOWSTATUS, OnUpdateControlBarMenu)
ON_WM_CREATE()
ON_COMMAND_EX(CG_ID_VIEW_SETUPDLG, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_SETUPDLG, OnUpdateControlBarMenu)
//{{AFX_MSG_MAP(CADFrm)
ON_WM_CLOSE()
ON_WM_CANCELMODE()
ON_WM_LBUTTONUP()
ON_COMMAND(IDM_MoveVolt, OnMoveVolt)
ON_WM_DESTROY()
ON_WM_CREATE()
ON_UPDATE_COMMAND_UI(IDM_MoveVolt, OnUpdateMoveVolt)
ON_COMMAND(IDM_ShowCfgAnalBar, OnShowCfgAnalBar)
ON_UPDATE_COMMAND_UI(IDM_ShowCfgAnalBar, OnUpdateShowCfgAnalBar)
ON_COMMAND(IDM_ShowStatusBar, OnShowStatusBar)
ON_UPDATE_COMMAND_UI(IDM_ShowStatusBar, OnUpdateShowStatusBar)
	//ON_COMMAND(IDM_AutoZeroValAD, OnAutoZeroValAD)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADFrm construction/destruction

CADFrm::CADFrm()
{
	// TODO: add member initialization code here
	gl_bProcessMoveVolt = TRUE;
	gl_ReadSizeWords = 0;
	gl_ScreenVolume = (int)AD_VOLT_RANGE;	
}

CADFrm::~CADFrm()
{
}

/////////////////////////////////////////////////////////////////////////////
// CADFrm diagnostics

#ifdef _DEBUG
void CADFrm::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CADFrm::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADFrm message handlers

BOOL CADFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndSplitter.CreateStatic(this, 1, 2))  // ����1��2�зָ�
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}
	
	// add the first splitter pane - the default view in column 0
	// ������һ�����ӣ���0����ʹ��Ĭ�ϵ���ͼ�����ĵ�ģ�������
	
	if (!m_wndSplitter.CreateView(0, 0, 
		RUNTIME_CLASS(CADDigitView), CSize(365, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	} 
	
	if (!m_wndSplitter.CreateView(0, 1, 
		RUNTIME_CLASS(CADWaveView), CSize(0, 470), pContext))
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}

	return TRUE;	
}


void CADFrm::ActivateFrame(int nCmdShow) 
{
	nCmdShow = SW_SHOWMAXIMIZED; // ʹ��֡�������
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

int CADFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// �����Ҳ�CControlBar����
	m_wndSTCBar.Create(this, CSize(220, 800), IDW_PROP_BAR);
	m_wndSTCBar.SetBarStyle(CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
	
	m_wndSTCBar.EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndSTCBar, AFX_IDW_DOCKBAR_RIGHT);
	
	gl_pParaCfgView = (CADParaCfgView*)m_wndSTCBar.AddView(_T("��������"), RUNTIME_CLASS(CADParaCfgView)); // CParaCfgView
	gl_pParaCfgView->m_pADFrm = this;
	
	gl_pADStatusView = (CADStatusView*)m_wndSTCBar.AddView(_T("���ܷ���"), RUNTIME_CLASS(CADStatusView)); // CADStatusView

	PCI9757_LoadParaAD(NULL, &ADPara);  // ����ϵͳ����
	
	
	// Initialize dialog bar m_wndShowStatus
	if (!m_wndShowStatus.Create(this, CG_IDD_Status,  
		CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE, 
		CG_IDD_Status))
	{
		TRACE0("Failed to create dialog bar m_wndShowStatus\n");
		return -1;		// fail to create
	}
	
	m_wndShowStatus.EnableDocking(CBRS_ALIGN_BOTTOM | CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndShowStatus);
	
	CProgressCtrl* pProgress = (CProgressCtrl*)m_wndShowStatus.GetDlgItem(IDC_PROGRESS);
	pProgress->SetRange(0, 50);
	
	if (!m_wndDlgSave.Create(this, CG_IDD_DIALOG_Save, 
		CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE, 
		CG_IDD_DIALOG_Save))
	{
		TRACE0("Failed to create dialog bar m_wndShowStatus\n");
		return -1;		// fail to create
	}
	m_wndDlgSave.ShowWindow(SW_HIDE);
	m_wndDlgSave.EnableDocking(CBRS_ALIGN_BOTTOM | CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndDlgSave);
	
	CEdit* pFreqEdit = (CEdit*)(this->m_wndShowStatus).GetDlgItem(IDC_EDIT_MeasureFreq); 
	pFreqEdit->SetWindowText(_T("������Ƶ"));
	
	m_ToolTip.Create(this, TTS_ALWAYSTIP|WS_CHILD|WS_VISIBLE);
	CComboBox* pFrequency = (CComboBox *)gl_pParaCfgView->GetDlgItem(IDC_COMBO_Frequency);
	CComboBox* pTriggerSource = (CComboBox *)gl_pParaCfgView->GetDlgItem(IDC_COMBO_TriggerSource);
	CComboBox* pTriggerDir = (CComboBox *)gl_pParaCfgView->GetDlgItem(IDC_COMBO_OutTriggerDir);
	CSliderCtrl* pSLIDER_Offset = (CSliderCtrl*)gl_pParaCfgView->GetDlgItem(IDC_SLIDER_Offset);
	m_ToolTip.AddTool(pTriggerSource, _T("�ڲ��������ⲿ����"));
	m_ToolTip.AddTool(pTriggerDir, _T("��������"));
	m_ToolTip.AddTool(pSLIDER_Offset, _T("��4K��������ѡ��ƫ��"));
	return 0;
}

void CADFrm::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnClose();
}

void CADFrm::OnCancelMode() 
{
	CMDIChildWnd::OnCancelMode();
}

void CADFrm::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CMDIChildWnd::OnLButtonUp(nFlags, point);
}

void CADFrm::OnMoveVolt() // ƽ�Ƶ�ѹ�ĺ���
{
	WORD m_DataMiddle; // �м���ԭ��
	ULONG nArrayCount = -1;
	ULONG m = 0;
	TCHAR str[256];
	int Channel = 0, TChannel = 0;
	CSysApp* pApp = (CSysApp *)AfxGetApp();
	CADDoc* pDoc = pApp->m_pADDoc; 
	CString strn;
	CADDigitView* pDigitView = (CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	ULONG iDataSize = (HALF_SIZE_WORDS - HALF_SIZE_WORDS%gl_nChannelCount) / gl_nChannelCount;

	if (gl_bProcessMoveVolt) // ��ʼ���м�ֵ
	{
		for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
		{
			if (ADPara.bChannelArray[Channel] == TRUE)
			{
				for (ULONG Index=0; Index<iDataSize; Index++)
				{
					DataAnalyze[Channel][Index] = ((ADBuffer[gl_nDrawIndex][Index * gl_nChannelCount + TChannel])&MASK_MSB); 
				}
				TChannel++;
			}
			
		}
		
		gl_pADStatusView->m_Combo_MoveVolt.DeleteString(0);
		/////////////////////////////////////////////			
		for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++) // ÿͨ�������ݸ��Խ�������
		{
			if (ADPara.bChannelArray[Channel] == TRUE)
			{
				m_QSortData.Sort(DataAnalyze[Channel], 0, iDataSize);
				m_DataMiddle = m_QSortData.GetMidData(); // ȡ���м���		
				// m_DataMiddle�Ƿ�ƫ���룬��ԭ�룬��0-LSB_COUNT�е�����ֵ
				// gl_MiddleLsb ��ƫ����ֵ����ԭ���ȥLSB_HALF���ֵ, ����ԭ�����¸�����ֵ
				gl_MiddleLsb[Channel] = m_DataMiddle;
				
				CString StrMoveVolt;
				switch(gl_DigitShowMode)
				{
				case SHOW_MODE_DEC: // ʮ����ԭ��
					_stprintf_s(str, _T("CH%d %d Dec"), Channel, m_DataMiddle);
					break;
				case SHOW_MODE_HEX: // ʮ������ԭ��
					_stprintf_s(str, _T("CH%d %x Hec"), Channel, m_DataMiddle);
					break;
				case SHOW_MODE_VOLT: // ��ѹֵ
					switch(ADPara.InputRange[Channel])
					{
					case PCI9757_INPUT_N10000_P10000mV:
					case PCI9757_INPUT_N5000_P5000mV:
					case PCI9757_INPUT_N2500_P2500mV:
						_stprintf_s(str, _T("CH%d %5.2fmV"), Channel, (m_DataMiddle - AD_LSB_HALF)*gl_PerLsbVolt[Channel]);
						break;
					case PCI9757_INPUT_0_P10000mV:
					case PCI9757_INPUT_0_P5000mV:
						_stprintf_s(str, _T("CH%d %5.2fmV"), Channel, (m_DataMiddle)*gl_PerLsbVolt[Channel]);
						break;
					default:
						break;
					}			
					
					break;
				}
				gl_pADStatusView->m_Combo_MoveVolt.AddString(str); // ����Ͽ������ƽ�Ƶ�ѹֵ
				
			}
			
			gl_AnalyzeAllCount = 0;
			gl_OverLimitCount = 0;
		}
		gl_pADStatusView->m_Combo_MoveVolt.SetCurSel(0);
		gl_bProcessMoveVolt = false;
	}	
	else
	{
		for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
		{
			gl_MiddleLsb[Channel] = 0; // �����ƫ��, ��ƫ��ֵΪ0
		}
		
		while (gl_pADStatusView->m_Combo_MoveVolt.GetCount()>0)
			gl_pADStatusView->m_Combo_MoveVolt.DeleteString(0);
		
		CString StrMoveVolt;
		StrMoveVolt = _T("û��ƽ��");
		gl_pADStatusView->m_Combo_MoveVolt.AddString (StrMoveVolt);
		gl_pADStatusView->m_Combo_MoveVolt.SetCurSel(0);
		gl_bProcessMoveVolt = true;
	}
	
	if (gl_bDeviceADRun == FALSE) // ����豸û�����У�����̬����
	{
		pDigitView->Invalidate();
		pWaveView->Invalidate();
		return;
	}
	
	if (gl_nProcMode == PROC_MODE_DIGIT)  // ��������ִ���ʽ����ֻ�������ִ���
	{
		pDigitView->Invalidate();
	}
	
	if (gl_nProcMode == PROC_MODE_WAVE)  // ����ǲ��δ���ʽ����ֻ���²��δ���
	{
		pWaveView->Invalidate();
	}
}

void CADFrm::OnUpdateMoveVolt(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(!gl_bProcessMoveVolt);	
}

void CADFrm::OnDestroy() 
{
	// TODO: Add your message handler code here
	CSysApp* pApp = (CSysApp*)AfxGetApp();
    pApp->m_bCreateADFrm = FALSE;
	ADPara.bChannelArray[0] = 1;
	PCI9757_SaveParaAD(NULL, &ADPara);
	
	if (gl_bDeviceADRun) // ����Ƿ��豸
	{
		CADDoc* pDoc = (CADDoc*)GetActiveDocument();
		pDoc->Public_ReleaseDevice();
	}    
	pApp->m_ADFrm = NULL;
	
	CMDIChildWnd::OnDestroy();	
}

void CADFrm::OnShowCfgAnalBar() 
{
	if (m_wndSTCBar.IsVisible())
	{
		ShowControlBar(&m_wndSTCBar, FALSE, FALSE);
	}
	else
	{
		ShowControlBar(&m_wndSTCBar, TRUE, FALSE);
	}
	
}

void CADFrm::OnUpdateShowCfgAnalBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndSTCBar.IsVisible());	
}

void CADFrm::OnShowStatusBar() 
{	
	if (m_wndShowStatus.IsVisible())
	{
		ShowControlBar(&m_wndShowStatus, FALSE, FALSE);
	}
	else
	{
		ShowControlBar(&m_wndShowStatus, TRUE, FALSE);
	}
}

void CADFrm::OnUpdateShowStatusBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndShowStatus.IsVisible());
}

BOOL CADFrm::PreTranslateMessage(MSG* pMsg) 
{
	m_ToolTip.RelayEvent(pMsg);		
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

CString CADFrm::GetAppPath()
{
	CString strPath, str;
	TCHAR cTempPath[256];
	HINSTANCE hinStance = AfxGetInstanceHandle();		// ��þ��
	GetModuleFileName(hinStance, cTempPath, 256);	// ��õ�ǰӦ�ó���·��
	strPath = cTempPath ;//��ǰ·��
	str = AfxGetApp()->m_pszExeName;
	int ProgramNameLength = strPath.GetLength();
	strPath = strPath.Left(ProgramNameLength-7);
//	str = "AutoZeroAD\\Sys.exe";
	strPath = strPath + str;
	return strPath ;
}

/*void CADFrm::OnAutoZeroValAD() 
{
	STARTUPINFO StartupInfo;
	memset(&StartupInfo, 0, sizeof(STARTUPINFO));
	StartupInfo.cb = sizeof(STARTUPINFO);
	CString AutoZeroADPath = GetAppPath();
//	PROCESS_INFORMATION ProcessInfo;

// 	::CreateProcess
// 		(AutoZeroADPath,	// "AutoZeroAD\\Sys.exe",
// 		" Wave",  // ע�⣺��һ���ַ�����Ϊ�ո�
// 		NULL,
// 		NULL,
// 		FALSE,
// 		0,
// 		NULL,
// 		NULL,
// 		&StartupInfo,
// 		&ProcessInfo);	
	
}*/
