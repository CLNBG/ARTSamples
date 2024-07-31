// ADFrm.cpp : implementation of the CADFrame class
//

#include "stdafx.h"
#include "Sys.h"

#include "MainFrm.h"
#include "ADFrm.h"
#include "ADDoc.h"
#include "ADDigitView.h"
#include "ADWaveView.h"
#include "QSortData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////
#include "ADStatusView.h"
#include "ParaCfgView.h"
extern CADStatusView* pADStatusView;
extern CParaCfgView* pParaCfgView;
////////////////////////////////////////////////////////////////////////////
// CADFrame

IMPLEMENT_DYNCREATE(CADFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CADFrame, CMDIChildWnd)
ON_COMMAND_EX(CG_ID_VIEW_DLGBAR, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_DLGBAR, OnUpdateControlBarMenu)
ON_COMMAND_EX(CG_ID_VIEW_SHOWSTATUS1, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_SHOWSTATUS1, OnUpdateControlBarMenu)
ON_COMMAND_EX(CG_ID_VIEW_SHOWSTATUS, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_SHOWSTATUS, OnUpdateControlBarMenu)
ON_WM_CREATE()
ON_COMMAND_EX(CG_ID_VIEW_SETUPDLG, OnBarCheck)
ON_UPDATE_COMMAND_UI(CG_ID_VIEW_SETUPDLG, OnUpdateControlBarMenu)
//{{AFX_MSG_MAP(CADFrame)
ON_WM_CLOSE()
ON_WM_CANCELMODE()
ON_WM_LBUTTONUP()
//ON_COMMAND(IDM_History_Digit, OnHistoryDigit)
ON_COMMAND(IDM_History_Wave, OnHistoryWave)
//ON_UPDATE_COMMAND_UI(IDM_History_Digit, OnUpdateHistoryDigit)
ON_UPDATE_COMMAND_UI(IDM_History_Wave, OnUpdateHistoryWave)
ON_COMMAND(IDM_MoveVolt, OnMoveVolt)
ON_WM_DESTROY()
ON_WM_CREATE()
ON_UPDATE_COMMAND_UI(IDM_MoveVolt, OnUpdateMoveVolt)
ON_BN_CLICKED(IDC_CHECK_CheckOverflow, OnCHECKCheckOverflow)
ON_COMMAND(IDM_ShowCfgAnalBar, OnShowCfgAnalBar)
ON_UPDATE_COMMAND_UI(IDM_ShowCfgAnalBar, OnUpdateShowCfgAnalBar)
ON_COMMAND(IDM_ShowStatusBar, OnShowStatusBar)
ON_UPDATE_COMMAND_UI(IDM_ShowStatusBar, OnUpdateShowStatusBar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADFrame construction/destruction

CADFrame::CADFrame()
{
	// TODO: add member initialization code here
	//	m_ProcessDigitalMode=1;
	m_ProcessGraphicMode = 1; 
	m_AnalyzeCheck = false;	
	m_bShowCfgAnalBar = true;
	m_bShowStatusBar = true;
	m_ProcessMoveVolt = 1;
	gl_ReadSizeWords = 0;
	gl_ScreenVolume = (int)VOLT_RANGE;
	
}

CADFrame::~CADFrame()
{
	if(m_pWndSTCBar != NULL)
	{
		delete m_pWndSTCBar;
		m_pWndSTCBar = NULL;
	}
	pParaCfgView = NULL;
	pADStatusView = NULL;

}



/////////////////////////////////////////////////////////////////////////////
// CADFrame diagnostics

#ifdef _DEBUG
void CADFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CADFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADFrame message handlers

BOOL CADFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
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
		RUNTIME_CLASS(CADDigitView), CSize(360, 50), pContext))//CADDigitView
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	} 
	
	if (!m_wndSplitter.CreateView(0, 1,
		RUNTIME_CLASS(CADWaveView), CSize(0, 470), pContext))//
	{
		TRACE0("Failed to create second pane\n");
		return FALSE;
	}
	return TRUE;	
}


void CADFrame::ActivateFrame(int nCmdShow) 
{
	nCmdShow = SW_SHOWMAXIMIZED; // ʹ��֡�������
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

int CADFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// �����Ҳ�CControlBar����
	m_pWndSTCBar = new CCfgTabCtrlBar;
	m_pWndSTCBar->Create(this, CSize(235, 100), IDW_PROP_BAR);
	m_pWndSTCBar->SetBarStyle(CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_FIXED);//CBRS_SIZE_DYNAMIC);
	
	m_pWndSTCBar->EnableDocking(CBRS_ALIGN_LEFT | CBRS_ALIGN_RIGHT);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(m_pWndSTCBar, AFX_IDW_DOCKBAR_RIGHT);
	{ // �����ͼû������
		pParaCfgView=(CParaCfgView*)m_pWndSTCBar->AddView(_T("��������"), RUNTIME_CLASS(CParaCfgView/*CParaCfgView*/));//CParaCfgView
		pParaCfgView->m_pADFrm = this;
	}
	{// �����ͼ��������
		pADStatusView=(CADStatusView*)m_pWndSTCBar->AddView(_T("���ܷ���"), RUNTIME_CLASS(CADStatusView));//CADStatusView
	}	

	OldADPara = ADPara;
	// TODO: Add a menu item that will toggle the visibility of the
	{
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
		
		CProgressCtrl* pT=(CProgressCtrl*)m_wndShowStatus.GetDlgItem(IDC_PROGRESS);
        pT->SetRange(0, 50);
		
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
	}
	CEdit* pFreqEdit=(CEdit*)(this->m_wndShowStatus).GetDlgItem(IDC_EDIT_MeasureFreq); 
	pFreqEdit->SetWindowText(_T("������Ƶ"));
	
	//m_ToolTip.Create(this,TTS_ALWAYSTIP|WS_CHILD|WS_VISIBLE);
	if(pParaCfgView != NULL)
	{
		CComboBox * pTriggerSource =(CComboBox *)pParaCfgView->GetDlgItem(IDC_COMBO_TriggerSource);
		CComboBox * pTriggerDir =(CComboBox *)pParaCfgView->GetDlgItem(IDC_COMBO_OutTriggerDir);
		
		CEdit * pBufferID =(CEdit *)pParaCfgView->GetDlgItem(IDC_EDIT_BufferID);
		CSliderCtrl* pSLIDER_Offset =(CSliderCtrl*)pParaCfgView->GetDlgItem(IDC_SLIDER_Offset);
		/*m_ToolTip.AddTool(pFrequency,_T("�趨Ƶ��"));
		m_ToolTip.AddTool(pTriggerSource,_T("�ڲ��������ⲿ����"));
		m_ToolTip.AddTool(pTriggerDir,_T("��������"));
		m_ToolTip.AddTool(pBufferID,_T("��2����������ѡ�񻺳���"));
		m_ToolTip.AddTool(pSLIDER_Offset,_T("�ڻ�������ѡ��ƫ��"));*/
	}
	return 0;
}

void CADFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnClose();
}

void CADFrame::OnCancelMode() 
{
	CMDIChildWnd::OnCancelMode();
	
	// TODO: Add your message handler code here
	
}

void CADFrame::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	CMDIChildWnd::OnLButtonUp(nFlags, point);
}


//void CADFrame::OnHistoryDigit() 
//{
//	// TODO: Add your command handler code here
//	STARTUPINFO StartupInfo;
//	memset(&StartupInfo,0,sizeof(STARTUPINFO));
//	StartupInfo.cb=sizeof(STARTUPINFO);
//	
//	PROCESS_INFORMATION ProcessInfo;
//	
//	::CreateProcess
//		(_T("\\User\\SysTest\\Release\\SysTest.exe"),
//		_T(" Digit"),  // ע�⣺��һ���ַ�����Ϊ�ո�
//		NULL,
//		NULL,
//		FALSE,
//		0,
//		NULL,
//		NULL,
//		&StartupInfo,
//		&ProcessInfo);
//}

void CADFrame::OnHistoryWave() 
{
	// TODO: Add your command handler code here
	STARTUPINFO StartupInfo;
	memset(&StartupInfo,0,sizeof(STARTUPINFO));
	StartupInfo.cb=sizeof(STARTUPINFO);
	
	PROCESS_INFORMATION ProcessInfo;
	
	::CreateProcess
		(_T("\\User\\SysTest\\Release\\SysTest.exe"),
		_T(" Wave"),  // ע�⣺��һ���ַ�����Ϊ�ո�
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&StartupInfo,
		&ProcessInfo);
	
}

//void CADFrame::OnUpdateHistoryDigit(CCmdUI* pCmdUI) 
//{
//	// TODO: Add your command update UI handler code here
//	pCmdUI->Enable(FALSE);
//}

void CADFrame::OnUpdateHistoryWave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(FALSE);
}

void CADFrame::OnMoveVolt() //ƽ�Ƶ�ѹ�ĺ���
{
	LONG m_DataMiddle; // �м���ԭ��
	ULONG nArrayCount=-1;
	ULONG m=0;
	TCHAR str[100];
	
	//m_ProcessMoveVolt=m_ProcessMoveVolt==0?1:0;
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADDoc* pDoc=pApp->m_pADDoc; 
	CString strn;
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	if(m_ProcessMoveVolt == 1) // ��ʼ���м�ֵ
	{
		m_ProcessMoveVolt = 0;
		if(gl_ReadSizeWords == 0)
		{
// 			AfxMessageBox(_T("��û�п�ʼ�ɼ�����!");
// 			return;
		}

		ULONG lCount = (gl_ReadSizeWords%4096)/gl_nChannelCount;
		if (lCount == 0)
		{
			if (gl_ReadSizeWords < MAX_RAM_SIZE)
			{
				lCount = gl_ReadSizeWords/gl_nChannelCount;
			}
			else
			{
				lCount = (gl_ReadSizeWords%MAX_RAM_SIZE)/gl_nChannelCount;
			}
			if (lCount == 0)
			{
				lCount = MAX_RAM_SIZE/gl_nChannelCount;
			}
		}

		
		//pADStatusView->m_nMoveVolt.ResetContent();

		if (lCount>8192)	// 8192ΪSHORT DataAnalyze[MAX_CHANNEL_COUNT][8192];
		{
			lCount = 8192;
		}
		
		for(int nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
		{
			if (ADPara.bChannelArray[nChannel] == TRUE)
			{
				for(ULONG Index=0; Index<lCount; Index++)
				{
					DataAnalyze[nChannel][Index] = ((ADBuffer[glBufferID][Index*gl_nChannelCount+gl_CHIdx[nChannel]])&gl_wMaxLSB);
				}
			}
		}
		/////////////////////////////////////////////			
		for(int k=0; k<MAX_CHANNEL_COUNT; k++)//ÿͨ�������ݸ��Խ�������
		{
			if (ADPara.bChannelArray[k] == TRUE)
			{

			m_QSortData.Sort(DataAnalyze[k], 0, lCount);
			m_DataMiddle = m_QSortData.GetMidData(); // ȡ���м���		
			// m_DataMiddle�Ƿ�ƫ���룬��ԭ�룬��0-gl_fLsbCount�е�����ֵ
			// gl_MiddleLsb ��ƫ����ֵ����ԭ���ȥgl_lLsbHalf���ֵ,����ԭ�����¸�����ֵ
			// gl_MiddleLsb[k]=(int)((m_DataMiddle)-gl_lLsbHalf);
			gl_MiddleLsb[k]=(int)(m_DataMiddle - gl_lLsbHalf);
//			gl_MiddleLsb[k] = gl_MiddleLsb[k];
			
			CString StrMoveVolt;
			switch(m_DigitalDataShowMode)
			{
			case 0: // ʮ����ԭ��
				_stprintf(str, _T("CH%d %d Dec"),k,m_DataMiddle-gl_lLsbHalf);
				break;
			case 1: // ʮ������ԭ��
				//StrMoveVolt.Format(_T("%x Hec"),m_DataMiddle.value);
				_stprintf(str, _T("CH%d %x Hec"),k,m_DataMiddle);
				break;
			case 2: // ��ѹֵ
				//StrMoveVolt.Format(_T("%5.2fmV"),(m_DataMiddle.value-8192)*VOLT_RANGE/gl_fLsbCount);
				_stprintf(str, _T("CH%d %5.2fmV"),k,(m_DataMiddle-gl_lLsbHalf)*gl_PerLsbVoltM[k]);
				break;
			}
			//pADStatus->m_nMoveVolt.SetWindowText(StrMoveVolt);
			if(pADStatusView != NULL)
				pADStatusView->m_nMoveVolt.AddString (str);
			}
			m_AnalyzeAllCount=0;
			m_OverLimitCount=0;
		}
		if(pADStatusView != NULL)
			pADStatusView->m_nMoveVolt.SetCurSel(0);
	}	
	else
	{
		m_ProcessMoveVolt = 1;
		for(int i=0;i<MAX_CHANNEL_COUNT;i++)
		{
			gl_MiddleLsb[i] = 0;// �����ƫ��, ��ƫ��ֵΪ0
			//gl_MiddleLsb[i] = 0;
		}
		if(pADStatusView != NULL)
		{
			while(pADStatusView->m_nMoveVolt.GetCount()>0)
				pADStatusView->m_nMoveVolt.DeleteString(0);
			CString StrMoveVolt;
			StrMoveVolt=" ";
			pADStatusView->m_nMoveVolt.AddString (StrMoveVolt);
		}
	}
	
	if(gl_bDeviceADRun==FALSE) // ����豸û�����У�����̬����
	{
		pDigitView->RedrawWindow();
		pWaveView->OnDrawPolyLine();
		return;
	}
	if(nProcessMode == DATA_MODE)  // ����ǲ��η�ʽ����ֻ�������ַ�ʽ
	{
		pDigitView->RedrawWindow();
	}
	if(nProcessMode == WAVE_MODE) // ��֮��Ȼ
	{
		pWaveView->OnDrawPolyLine();
	}
}
void CADFrame::OnUpdateMoveVolt(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(!m_ProcessMoveVolt);	
}

void CADFrame::OnDestroy() 
{
	
	// TODO: Add your message handler code here
	ACTS1000_SaveParaAD(NULL, &ADPara);
	if(gl_bDeviceADRun)//����Ƿ��豸
	{
		CADDoc* pDoc=(CADDoc*)GetActiveDocument();
		pDoc->Public_ReleaseDevice();
	}    
	;
	CSysApp* pApp = (CSysApp*)AfxGetApp();
//	ACTS1000_ReleaseDevice(pApp->m_hDevice);
    pApp->m_bCreateADFrm = FALSE;	
	pApp->m_ADFrame = NULL;

	CMDIChildWnd::OnDestroy();	
}

void CADFrame::OnCHECKCheckOverflow() 
{
	// TODO: Add your control notification handler code here
	MessageBox(_T("OnCHECKCheckOverflow"));
}

void CADFrame::OnShowCfgAnalBar() 
{
	// TODO: Add your command handler code here
	m_bShowCfgAnalBar = !m_bShowCfgAnalBar;
	ShowControlBar(m_pWndSTCBar, m_bShowCfgAnalBar, false);
	
}

void CADFrame::OnUpdateShowCfgAnalBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bShowCfgAnalBar);
	
}

void CADFrame::OnShowStatusBar() 
{
	// TODO: Add your command handler code here
	m_bShowStatusBar = !m_bShowStatusBar;
	ShowControlBar(&m_wndShowStatus, m_bShowStatusBar, false);	
}

void CADFrame::OnUpdateShowStatusBar(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_bShowStatusBar);
}

BOOL CADFrame::PreTranslateMessage(MSG* pMsg) 
{
	//m_ToolTip.RelayEvent(pMsg);		
	return CMDIChildWnd::PreTranslateMessage(pMsg);
}

 
