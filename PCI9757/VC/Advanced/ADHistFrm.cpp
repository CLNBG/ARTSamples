// HistDataFrm.cpp : implementation file

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADHistFrm

IMPLEMENT_DYNCREATE(CADHistFrm, CMDIChildWnd)

CADHistFrm::CADHistFrm()
{
	m_bStart = FALSE;
	gl_bHistProcessMoveVolt = TRUE;
}

CADHistFrm::~CADHistFrm()
{
}


BEGIN_MESSAGE_MAP(CADHistFrm, CMDIChildWnd)
	ON_WM_CREATE()
	ON_COMMAND_EX(CG_ID_VIEW_DIALOGBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(CG_ID_VIEW_DIALOGBAR, OnUpdateControlBarMenu)
	//{{AFX_MSG_MAP(CADHistFrm)
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_SIZE()
	ON_COMMAND(IDM_History_Start, OnHistoryStart)
	ON_UPDATE_COMMAND_UI(IDM_History_Start, OnUpdateHistoryStart)
	ON_COMMAND(IDM_History_Stop, OnHistoryStop)
	ON_UPDATE_COMMAND_UI(IDM_History_Stop, OnUpdateHistoryStop)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_PositionB, OnPositionB)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Offset, OnCustomdrawSLIDEROffset)
	ON_CBN_SELCHANGE(IDC_COMBO_Rate, OnSelchangeCOMBORate)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_RefrushData, OnBUTTONRefrushData)
	ON_BN_CLICKED(IDC_BUTTON_PositionA, OnBUTTONPositionA)
	ON_BN_CLICKED(IDC_BUTTON_PositionB, OnBUTTONPositionB)
	ON_COMMAND(IDM_HistMoveVolt, OnHistMoveVolt)
	ON_UPDATE_COMMAND_UI(IDM_HistMoveVolt, OnUpdateHistMoveVolt)
	ON_BN_CLICKED(IDC_BUTTON_TriggerPos, OnBUTTONTriggerPos)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADHistFrm message handlers

BOOL CADHistFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (!m_wndSplitter.CreateStatic(this, 1, 2))  // ����1��2�зָ�
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// add the first splitter pane - the default view in column 0
	// ������һ�����ӣ���0����ʹ��Ĭ�ϵ���ͼ�����ĵ�ģ�����, CADHistDigitView��
	if (!m_wndSplitter.CreateView(0, 0, 
		pContext->m_pNewViewClass, CSize(120, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	} 
	
	if (!m_wndSplitter.CreateView(0, 1,  
			RUNTIME_CLASS(CADHistWaveView), CSize(350, 50), pContext))
	{
		TRACE0("Failed to create first pane\n");
		return FALSE;
	} 	
    
	return TRUE;
	// return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

int CADHistFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Initialize dialog bar m_wndSetupBar
	if (!m_wndSetupBar.Create(this, CG_IDD_AD_HIST_DLG, 
		CBRS_BOTTOM | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_HIDE_INPLACE, 
		CG_ID_VIEW_DIALOGBAR))
	{
		TRACE0("Failed to create dialog bar m_wndSetupBar\n");
		return -1;		// fail to create
	}
	
	m_wndSetupBar.EnableDocking(CBRS_ALIGN_BOTTOM | CBRS_ALIGN_TOP);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndSetupBar);

	CButton* pRadioA = (CButton*)m_wndSetupBar.GetDlgItem(IDC_RADIO_PositionA);
	CButton* pRadioB = (CButton*)m_wndSetupBar.GetDlgItem(IDC_RADIO_PositionB);
	pRadioA->EnableWindow(FALSE);
	pRadioB->EnableWindow(FALSE);
	pRadioA->SetCheck(1);
	pRadioB->SetCheck(0);
	
	return 0;
}

void CADHistFrm::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnLButtonUp(nFlags, point);
}

void CADHistFrm::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // ��Frame��ȡ�õ�ǰ�ĵ�ָ��
	CADHistWaveView* pWaveView = (CADHistWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave));
	pDoc->m_Offset += pDoc->m_Rate;
	RedrawDataWindow();
	CMDIChildWnd::OnTimer(nIDEvent);
	pWaveView->m_ADHistScope.AppendPoly(gl_Offset);
}

void CADHistFrm::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
}

void CADHistFrm::OnHistoryStart() 
{
	// TODO: Add your command handler code here
	m_bStart = TRUE;
	SetTimer(1, 200, NULL);

}

void CADHistFrm::OnUpdateHistoryStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bStart == FALSE ? 1:0);
}

void CADHistFrm::OnHistoryStop() 
{
	// TODO: Add your command handler code here
	m_bStart = FALSE;
	KillTimer(1);
}

void CADHistFrm::OnUpdateHistoryStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bStart == TRUE ? 1:0);
}

void CADHistFrm::OnPositionA(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
} 
void CADHistFrm::OnPositionB(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(1);
} 

void CADHistFrm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CADHistFrm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CMDIChildWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CADHistFrm::OnCustomdrawSLIDEROffset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // ��Frame��ȡ�õ�ǰ�ĵ�ָ��
    CADHistWaveView* pWaveView = (CADHistWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 

	CSliderCtrl* pSliderOffset = (CSliderCtrl*)(m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));	
	int Pos=pSliderOffset->GetPos();  // ȡ�û�����λ�ã����ļ�ƫ�ƣ��֣���
	pSliderOffset->SetPos(Pos);
	pDoc->m_Offset = Pos; // �ĵ��е��ļ�ƫ�Ƶ��ڻ��ߵ�ƫ��λ�� 

	CString str;	
	CEdit* pEditOffset = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset));	
	CEdit* pEditScreenPos = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_ScreenPos));
	str.Format(_T("%d") , Pos);
	pEditOffset->SetWindowText(str);  // ��ʾ�ļ�ƫ��
	str.Format(_T("%d") , Pos);
	pEditScreenPos->SetWindowText(str);   
	RedrawDataWindow();  // �ػ�������ͼ
	pWaveView->m_ADHistScope.AppendPoly(gl_Offset);

	*pResult = 0;
}

void CADHistFrm::RedrawDataWindow(void)
{
 	CString str;
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // ��Frame��ȡ�õ�ǰ�ĵ�ָ��
    CADHistWaveView* pWaveView = (CADHistWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	CEdit* pEditPos = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_ScreenPos));

	int ChannelCount = pDoc->m_ChannelCount;
 	
    str.Format(_T("%d") , pDoc->m_Offset/ChannelCount);
    pEditPos->SetWindowText(str);	
	pDoc->ReadData();
	pWaveView->m_ADHistScope.AppendPoly(gl_Offset);
}

void CADHistFrm::OnSelchangeCOMBORate() 
{
	// TODO: Add your control notification handler code here
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // ��Frame��ȡ�õ�ǰ�ĵ�ָ��
	CComboBox* pRate = (CComboBox*)(m_wndSetupBar.GetDlgItem(IDC_COMBO_Rate));	
	CString strRate;
	int Index = pRate->GetCurSel();  // ȡ�õ�ǰѡ��������
	pRate->GetLBText(Index, strRate);   // ����ѡ��ȡ����ѡ�����ı�	
	pDoc->m_Rate = _tcstol(strRate, NULL, 10);  // ���ַ�ת��������
}

void CADHistFrm::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnClose();
}

void CADHistFrm::OnBUTTONRefrushData() 
{
	// TODO: Add your control notification handler code here
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // ��Frame��ȡ�õ�ǰ�ĵ�ָ��
	CADHistDigitView* pDigitView = (CADHistDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	pDigitView->SetListView();
}

void CADHistFrm::OnBUTTONPositionA() 
{
	CADHistDoc* pDoc = theApp.m_pADHistDoc;
 	CADHistWaveView* pWaveView = (CADHistWaveView*)CWnd::FromHandle(pDoc->m_hWndWave);
  	CADHistDigitView* pView = (CADHistDigitView*)CWnd::FromHandle(pDoc->m_hWndDigit);

	CString strPositionA;
	CStatic* pStaticA = (CStatic*)m_wndSetupBar.GetDlgItem(IDC_STATIC_PositionA);
 	pStaticA->GetWindowText(strPositionA);
	
	UINT nPos = _tcstol(strPositionA, NULL, 10);
	int nTmp = pDoc->m_Offset;
	if (nPos-512>pDoc->m_Offset && nPos>100)
	{
		
		pDoc->m_Offset = nPos - 100;
		
	}
	
	pView->SetListView();

	pDoc->m_Offset = nTmp;

	CListCtrl& List = pView->GetListCtrl();
	
 	LPCTSTR lpszmyString = strPositionA;
 	LVFINDINFO info;
 	int nIndex;
 	info.flags = LVFI_STRING;
 	info.psz = lpszmyString;
 	while ((nIndex = List.FindItem(&info)) != -1)
 	{
		if (nIndex >= 0)
		{		
			List.EnsureVisible(nIndex, FALSE);							// ʹlistctrl��һ��ɼ���������������
			List.SetItemState(nIndex, 2, LVIS_SELECTED|LVIS_FOCUSED);   // ѡ��ѡ��һ��(0,Ϊȡ��)
			List.SetFocus();
			break;
		}		
	}	

	Invalidate();	
}

void CADHistFrm::OnBUTTONPositionB() 
{
	CADHistDoc* pDoc = theApp.m_pADHistDoc;
 	CADHistWaveView* pWaveView = (CADHistWaveView*)CWnd::FromHandle(pDoc->m_hWndWave);
  	CADHistDigitView* pView = (CADHistDigitView*)CWnd::FromHandle(pDoc->m_hWndDigit);

	CString strPositionB;;
	CStatic* pStaticB = (CStatic*)m_wndSetupBar.GetDlgItem(IDC_STATIC_PositionB);
 	pStaticB->GetWindowText(strPositionB);
	
	UINT nPos = _tcstol(strPositionB, NULL, 10);
	int nTmp = pDoc->m_Offset;
	if (nPos-512>pDoc->m_Offset && nPos>100)
	{
		
		pDoc->m_Offset = nPos - 100;
		
	}
	
	pView->SetListView();

	pDoc->m_Offset = nTmp;

	CListCtrl& List = pView->GetListCtrl();
	
 	LPCTSTR lpszmyString = strPositionB;
 	LVFINDINFO info;
 	int nIndex;
 	info.flags = LVFI_STRING;
 	info.psz = lpszmyString;

	

 	while ((nIndex = List.FindItem(&info)) != -1)
 	{
		if (nIndex >= 0)
		{		
			List.EnsureVisible(nIndex, FALSE);							// ʹlistctrl��һ��ɼ���������������
			List.SetItemState(nIndex, 2, LVIS_SELECTED|LVIS_FOCUSED);  // ѡ��ѡ��һ��(0,Ϊȡ��)
			List.SetFocus();
			break;
		}		
	}	


	Invalidate();	
	
}


void CADHistFrm::OnHistMoveVolt() 
{
	WORD m_DataMiddle; // �м���ԭ��
	ULONG nArrayCount = -1;
	ULONG m = 0;
	int Channel = 0, TChannel = 0;
	CSysApp* pApp = (CSysApp *)AfxGetApp();
	CADHistDoc* pDoc = pApp->m_pADHistDoc; 
	CString strn;
	CADHistDigitView* pDigitView = (CADHistDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit));  
	ULONG iDataSize = (HALF_SIZE_WORDS - HALF_SIZE_WORDS%pDoc->m_ChannelCount) / pDoc->m_ChannelCount;
	if (gl_bHistProcessMoveVolt) // ��ʼ���м�ֵ
	{
		for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
		{
			if (pDoc->m_Header.ADPara.bChannelArray[Channel] == TRUE)
			{
				for (ULONG Index=0; Index<iDataSize; Index++)
				{
					DataAnalyze[Channel][Index] = ((pDoc->m_ADBuffer[Index * pDoc->m_ChannelCount + TChannel])&MASK_MSB); 
				}	
				TChannel++;
			}
			
		}
		
		/////////////////////////////////////////////			
		for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++) // ÿͨ�������ݸ��Խ�������
		{
			if (pDoc->m_Header.ADPara.bChannelArray[Channel] == TRUE)
			{
				m_QSortData.Sort(DataAnalyze[Channel], 0, iDataSize);
				m_DataMiddle = m_QSortData.GetMidData(); // ȡ���м���		
				gl_HistMiddleLsb[Channel] = m_DataMiddle;
				
				
			}
		}
		gl_bHistProcessMoveVolt = false;
	}	
	else
	{
		for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
		{
			gl_HistMiddleLsb[Channel] = 0; // �����ƫ��, ��ƫ��ֵΪ0
		}

		gl_bHistProcessMoveVolt = true;
	}
	
	pDigitView->SetListView();
}

void CADHistFrm::OnUpdateHistMoveVolt(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!gl_bHistProcessMoveVolt);	
	
}

void CADHistFrm::OnBUTTONTriggerPos() 
{
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // ��Frame��ȡ�õ�ǰ�ĵ�ָ��
 	CADHistWaveView* pWaveView = (CADHistWaveView*)CWnd::FromHandle(pDoc->m_hWndWave);
  	CADHistDigitView* pView = (CADHistDigitView*)CWnd::FromHandle(pDoc->m_hWndDigit);

	CSliderCtrl* pSliderOffset = (CSliderCtrl*)(m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));
	pSliderOffset->SetPos(0);

	CString strPositionA;
	CStatic* pStaticA = (CStatic*)m_wndSetupBar.GetDlgItem(IDC_STATIC_PositionA);
 	pStaticA->GetWindowText(strPositionA);
	LONG m_PositionAoffset = _tcstol(strPositionA, NULL, 10);
	int ScreenPositon = (int)pWaveView->m_ADHistScope.m_ScreenPosition;
	
	pSliderOffset->SetPos(pDoc->m_Header.nTriggerPos/pDoc->m_ChannelCount-ScreenPositon);
	pWaveView->RedrawWindow();
	
}
