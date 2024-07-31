// HistDataFrm.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"
#include "ADHistFrm.h"

#include "ADHistDigitView.h"
#include "ADHistWaveView.h"

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
	m_ShowMode = 2;  // 波形回放（1为数字回放）
	m_PositionAoffset = 0;
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
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_Offset, OnCustomdrawSLIDEROffset)
	ON_CBN_SELCHANGE(IDC_COMBO_Rate, OnSelchangeCOMBORate)
	ON_EN_CHANGE(IDC_EDIT_Offset, OnChangeEDITOffset)
	ON_WM_CLOSE()
 	ON_BN_CLICKED(IDC_BUTTON_RefrushData, OnBUTTONRefrushData)
 	ON_BN_CLICKED(IDC_BUTTON_PositionA, OnBUTTONPositionA)
 	ON_BN_CLICKED(IDC_BUTTON_PositionB, OnBUTTONPositionB)

	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_Goto, &CADHistFrm::OnBnClickedGoto)
	ON_EN_KILLFOCUS(IDC_EDIT_Offset, &CADHistFrm::OnEnKillfocusEditOffset)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADHistFrm message handlers

BOOL CADHistFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
		if (!m_wndSplitter.CreateStatic(this, 1, 2))  // 创建1行2列分割
	{
		TRACE0("Failed to CreateStaticSplitter\n");
		return FALSE;
	}

	// add the first splitter pane - the default view in column 0
	// 创建第一个格子，在0列中使用默认的视图（由文档模板决定）
	if (!m_wndSplitter.CreateView(0, 0,
		pContext->m_pNewViewClass, CSize(250, 50), pContext))
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

	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

int CADHistFrm::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	{
		// Initialize dialog bar m_wndSetupBar
		if (!m_wndSetupBar.Create(this, CG_IDD_HistoryDlg,
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

	}		
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
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // 在Frame中取得当前文档指针
	CADHistWaveView* pWaveView = (CADHistWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave));
	//pDoc->m_Offset += (pDoc->m_Rate*pDoc->m_SliderRatio);

// 	pDoc->m_SliderOffset += pDoc->m_Rate;
// 	pDoc->m_Offset = pDoc->m_SliderOffset*pDoc->m_SliderRatio;

	pDoc->m_Offset += pDoc->m_Rate;
	pDoc->m_SliderOffset = pDoc->m_Offset/pDoc->m_SliderRatio;

	LONG nChannelCount = pDoc->m_ChannelCount;
	LONGLONG lPointCount = (pDoc->m_FileLength-sizeof(::_FILE_HEADER)) / (sizeof(WORD) * nChannelCount);
	if (pDoc->m_Offset >= (lPointCount-pDoc->m_ReadDataSize/nChannelCount))
	{
		pDoc->m_Offset = lPointCount-pDoc->m_ReadDataSize/nChannelCount;
		pDoc->m_SliderOffset = pDoc->m_Offset/pDoc->m_SliderRatio;
		KillTimer(1);
	}
	RedrawDataWindow();
	CMDIChildWnd::OnTimer(nIDEvent);
}

void CADHistFrm::OnSize(UINT nType, int cx, int cy) 
{
	CMDIChildWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	

}

void CADHistFrm::OnHistoryStart() 
{
	// TODO: Add your command handler code here
	m_bStart=TRUE;
	SetTimer(1, 200, NULL);
}

void CADHistFrm::OnUpdateHistoryStart(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bStart==FALSE?1:0);
}

void CADHistFrm::OnHistoryStop() 
{
	// TODO: Add your command handler code here
	m_bStart=FALSE;
	KillTimer(1);
}

void CADHistFrm::OnUpdateHistoryStop(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bStart==TRUE?1:0);
}


void CADHistFrm::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CADHistFrm::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default


	CMDIChildWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CADHistFrm::OnCustomdrawSLIDEROffset(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // 在Frame中取得当前文档指针

	CSliderCtrl* pSliderOffset = (CSliderCtrl*)(m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));	
	int Pos = pSliderOffset->GetPos();  // 取得滑动条位置（即文件偏移（字））
	pSliderOffset->SetPos(Pos);
	pDoc->m_SliderOffset = Pos; 
	pDoc->m_Offset = Pos*pDoc->m_SliderRatio; // 文档中的文件偏移等于滑尺的偏移位置
    int ChannelCount;
	ChannelCount = pDoc->m_ChannelCount;
	CString str;	
	CEdit* pEditOffset = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset));	
	CEdit* pEditScreenPos = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_ScreenPos));
	str.Format(L"%I64d", pDoc->m_Offset);
	pEditOffset->SetWindowText(str);  // 显示文件偏移
	str.Format(L"%I64d", pDoc->m_Offset);
	pEditScreenPos->SetWindowText(str);   
	RedrawDataWindow();  // 重绘数据视图
	Invalidate();

	*pResult = 0;
}

void CADHistFrm::RedrawDataWindow(void)
{
	CString strScreenPos;
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // 在Frame中取得当前文档指针
	CADHistDigitView* pDigitView = (CADHistDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
    CADHistWaveView* pWaveView = (CADHistWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	int ScreenPositon = (int)pWaveView->m_ADHistScope.m_ScreenPosition;
    int ChannelCount = pDoc->m_ChannelCount;
	CEdit* pEditScreenPos = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_ScreenPos));
    strScreenPos.Format(_T("%I64d"), pDoc->m_Offset/*/ChannelCount*/);
    pEditScreenPos->SetWindowText(strScreenPos);

	CString strOffset;	
	CEdit* pEditOffset = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset));	
	//strOffset.Format(_T("%d"), Pos);
	pEditOffset->SetWindowText(strScreenPos);  // 显示文件偏移

	CSliderCtrl* pSliderOffset = (CSliderCtrl*)(m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));	

	//pSliderOffset->SetPos(pDoc->m_Offset/pDoc->m_SliderRatio);
	pSliderOffset->SetPos((int)pDoc->m_SliderOffset);

	pWaveView->RedrawWindow();
	LONGLONG iDataLenth = 0;
	PWORD pBuff = NULL;
	for (int iChannel=0; iChannel<pDoc->m_ChannelCount; iChannel++)
	{
		pBuff = pDoc->GetBuffer(iChannel, pDoc->m_Offset, &iDataLenth);
		
		if (pBuff != NULL)
		{
			pWaveView->m_ADHistScope.AppendPoly(pBuff, iChannel, iDataLenth, pDoc->m_Offset, FALSE);	
		}			
	}
}

void CADHistFrm::OnSelchangeCOMBORate() 
{
	// TODO: Add your control notification handler code here
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // 在Frame中取得当前文档指针
	CComboBox* pRate=(CComboBox*)(m_wndSetupBar.GetDlgItem(IDC_COMBO_Rate));	
	CString str;
	int Index=pRate->GetCurSel();  // 取得当前选择索引号
	pRate->GetLBText(Index,str);   // 根据选择，取得所选择项文本	
	pDoc->m_Rate=wcstol(str,NULL, 10);  // 将字符转换成数字
}

void CADHistFrm::OnChangeEDITOffset() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CMDIChildWnd::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
}

void CADHistFrm::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CMDIChildWnd::OnClose();
}

void CADHistFrm::OnBUTTONRefrushData() 
{
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // 在Frame中取得当前文档指针
	CADHistDigitView* pDigitView = (CADHistDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	pDigitView->SetListView();
}

void CADHistFrm::ActivateFrame(int nCmdShow) 
{
	CListBox* pList = (CListBox*)(m_wndSetupBar.GetDlgItem(IDC_FILESINFO));
	pList->RedrawWindow();
	pList->SendMessage(WS_VSCROLL, SB_LINEDOWN, NULL);
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

CDocument* CADHistFrm::GetActiveDocument() 
{
	return CMDIChildWnd::GetActiveDocument();
}

BOOL CADHistFrm::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	return CMDIChildWnd::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext);
}

void CADHistFrm::OnBUTTONPositionA() 
{
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // 在Frame中取得当前文档指针
 	CADHistWaveView* pWaveView = (CADHistWaveView*)CWnd::FromHandle(pDoc->m_hWndWave);
  	CADHistDigitView* pView = (CADHistDigitView*)CWnd::FromHandle(pDoc->m_hWndDigit);

	CString strPositionA;
	CStatic* pStaticA = (CStatic*)m_wndSetupBar.GetDlgItem(IDC_STATIC_PositionA);
 	pStaticA->GetWindowText(strPositionA);
	pView->SetListView();

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
			List.EnsureVisible(nIndex, FALSE);							// 使listctrl中一项可见，即滚动滚动条
			List.SetItemState(nIndex, 2, LVIS_SELECTED|LVIS_FOCUSED);   // 选中一行(0,为取消)
			List.SetFocus();
			break;
		}		
	}	

	Invalidate();	
	
}

void CADHistFrm::OnBUTTONPositionB() 
{
	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // 在Frame中取得当前文档指针
 	CADHistWaveView* pWaveView = (CADHistWaveView*)CWnd::FromHandle(pDoc->m_hWndWave);
  	CADHistDigitView* pView = (CADHistDigitView*)CWnd::FromHandle(pDoc->m_hWndDigit);

	CString strPositionB;;
	CStatic* pStaticB = (CStatic*)m_wndSetupBar.GetDlgItem(IDC_STATIC_PositionB);
 	pStaticB->GetWindowText(strPositionB);
	pView->SetListView();

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
			List.EnsureVisible(nIndex, FALSE);							// 使listctrl中一项可见，即滚动滚动条
			List.SetItemState(nIndex, 2, LVIS_SELECTED|LVIS_FOCUSED);  // 选中选中一行(0,为取消)
			List.SetFocus();
			break;
		}		
	}	

	Invalidate();	
	
}

void CADHistFrm::OnBUTTONTriggerPos() 
{

}

void CADHistFrm::OnBnClickedGoto()
{
	// TODO: 在此添加控件通知处理程序代码

	CADHistDoc* pDoc = (CADHistDoc*)GetActiveDocument();  // 在Frame中取得当前文档指针
	CEdit* pEditOffset = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset));	
	CString str;
	pEditOffset->GetWindowText(str);  // 显示文件偏移
	ULONG ul = _tcstol(str, NULL, 10);
	CSliderCtrl* pSliderOffset = (CSliderCtrl*)(m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));	

	pSliderOffset->SetPos(ul/pDoc->m_SliderRatio);


	pDoc->m_Offset = ul/pDoc->m_SliderRatio*pDoc->m_SliderRatio; // 文档中的文件偏移等于滑尺的偏移位置
// 	int ChannelCount;
// 	ChannelCount = pDoc->m_ChannelCount;
// 	CString str;	
// 	CEdit* pEditOffset = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset));	
// 	CEdit* pEditScreenPos = (CEdit*)(m_wndSetupBar.GetDlgItem(IDC_EDIT_ScreenPos));
// 	str.Format(L"%I64d", pDoc->m_Offset);
// 	pEditOffset->SetWindowText(str);  // 显示文件偏移
// 	str.Format(L"%I64d", pDoc->m_Offset);
// 	pEditScreenPos->SetWindowText(str);   
	RedrawDataWindow();  // 重绘数据视图
	Invalidate();


}

void CADHistFrm::OnEnKillfocusEditOffset()
{
	// TODO: 在此添加控件通知处理程序代码

}
