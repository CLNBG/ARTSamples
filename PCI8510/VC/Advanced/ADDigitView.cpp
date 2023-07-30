// ADDigitView.cpp : implementation of the CADDigitView class
//
#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CADDigitView

IMPLEMENT_DYNCREATE(CADDigitView, CScrollView)

BEGIN_MESSAGE_MAP(CADDigitView, CScrollView)
//{{AFX_MSG_MAP(CADDigitView)
ON_WM_CONTEXTMENU()
ON_WM_CANCELMODE()
ON_COMMAND(IDM_ShowHex, OnShowHex)
ON_UPDATE_COMMAND_UI(IDM_ShowHex, OnUpdateShowHex)
ON_COMMAND(IDM_ShowDec, OnShowDec)
ON_UPDATE_COMMAND_UI(IDM_ShowDec, OnUpdateShowDec)
ON_COMMAND(IDM_ShowVolt, OnShowVolt)
ON_UPDATE_COMMAND_UI(IDM_ShowVolt, OnUpdateShowVolt)
ON_WM_VSCROLL()
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_ERASEBKGND()
ON_WM_KEYDOWN()
ON_WM_MOUSEWHEEL()
ON_MESSAGE(WM_SHOW_DIGIT, ShowDigit)
ON_WM_HSCROLL()
//}}AFX_MSG_MAP
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADDigitView construction/destruction

CADDigitView::CADDigitView()
{
	// TODO: add construction code here
	gl_DigitShowMode = SHOW_MODE_VOLT; // 电压值显示模式
	m_CurrentProgress = 0; // 置采集进度条起值为0	
}

CADDigitView::~CADDigitView()
{
	
}

BOOL CADDigitView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CADDigitView drawing
void CADDigitView::OnDraw(CDC* pDC)
{   
	ADDigit(pDC);	
	if (gl_bProgress) // 刷新ADDigitView视图时，更新进度条的状态
	{
		CADFrm* pADFrm = ((CSysApp*)AfxGetApp())->m_ADFrm; // 取得子帧窗口句柄
		CProgressCtrl* pProgress = (CProgressCtrl*)(pADFrm->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));
		pProgress->SetPos(m_CurrentProgress);
		m_CurrentProgress++;
		if (m_CurrentProgress > 50) m_CurrentProgress = 0;
		gl_bProgress = FALSE;
	}
}

void CADDigitView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	const SIZE conPSize = {200, 1};
	const SIZE conLSize = {20, 1};
	sizeTotal.cx = 1200; 
	sizeTotal.cy = 1025*16; 
	SetScrollSizes(MM_TEXT, sizeTotal, sizeDefault, CSize(sizeDefault.cx, 16));
	CADDoc* pDoc = GetDocument();
	pDoc->m_hWndDigit = m_hWnd; 
}

/////////////////////////////////////////////////////////////////////////////
// CADDigitView printing

BOOL CADDigitView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CADDigitView diagnostics
#ifdef _DEBUG
void CADDigitView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CADDigitView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CADDoc* CADDigitView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CADDoc)));
	return (CADDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADDigitView message handlers
void CADDigitView::ADDigit(CDC* pDC) // 显示ch值 
{	
	gl_bDataProcessing = TRUE; // 通知数据处理线程，正在绘制数字，请不要往DrawBuffer中填充新数据	
   	CRect rect;
	int   Channel = 0, TChannel = 0;
	int   ADLsb   = 0, ADData = 0;
	float fADVolt = 0.0f;
	TCHAR  str[256], strIndex[256];
	int   ChannelIndex = 0;
	CString strChannel;
	
	GetClientRect(&rect);
	OnPrepareDC(pDC);
	pDC->DPtoLP(&rect);   
	pDC->SetTextColor(RGB(0, 255, 0));
	pDC->SetBkMode(TRANSPARENT);
	
	int nTextHeight = 0;
	int Index = 0, DataIndex = 0;
	
	// 在数字视图窗口，注释说明
	switch(gl_DigitShowMode)
	{
	case SHOW_MODE_DEC:	// 十进制显示
		swprintf_s(str, _T("%s"), _T("  十进制显示数据(去掉无效位D14、D15)"));
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(-rect.left, rect.top, str);
		pDC->SetTextColor(RGB(0, 255, 0));
		strChannel.Format(_T("%s"), _T("Index      "));
		for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
		{							
			swprintf_s(strIndex, _T("CH%02d         "), Channel);
			strChannel = strChannel + strIndex;
		}
		break;
		
	case SHOW_MODE_HEX:	// 十六进制显示
		swprintf_s(str, _T("%s"), _T("  十六进制原码显示数据"));
		pDC->SetTextColor(RGB(0, 255, 255));
		pDC->TextOut(-rect.left, rect.top, str);
		pDC->SetTextColor(RGB(0, 255, 0));
		strChannel.Format(_T("%s"), _T("Index  "));
		for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
		{						
			swprintf_s(strIndex, _T("CH%02d            "), Channel);
			strChannel = strChannel + strIndex;
		}
		break;
		
	case SHOW_MODE_VOLT:	// 电压值显示
		swprintf_s(str, _T("%s"), _T("  电压值显示数据"));
		pDC->SetTextColor(RGB(255, 255, 0));
		pDC->TextOut(-rect.left, rect.top, str);
		pDC->SetTextColor(RGB(0, 255, 0));
		strChannel.Format(_T("%s"), _T("Index       "));
		for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
		{						
			swprintf_s(strIndex, _T("CH%02d         "), Channel);
			strChannel = strChannel + strIndex;
		}		
		break;
		
	} // switch(gl_DigitShowMode)
	
	nTextHeight = 20;
	pDC->SetTextColor(RGB(0, 255, 0));
	pDC->TextOut(-rect.left, rect.top +40, strChannel); // first line text out "Index CHX..."	
	for (int RowY=rect.top+16; RowY<(rect.bottom); RowY++)
	{		
		if (RowY%8 == 0)
		{			
			Index = gl_Offset + RowY/8 - 1; // 数据点索引值	
			DataIndex = Index * gl_nChannelCount;			
			if (Index > HALF_SIZE_WORDS/gl_nChannelCount - 1) break;
			swprintf_s(strIndex, _T("%4d"), Index); 
			pDC->TextOut(-rect.left, rect.top+nTextHeight+36, strIndex); // text out Index of data			
			switch(gl_DigitShowMode)
			{
			case SHOW_MODE_DEC: // 十进制偏移码				
				for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
				{						
					ADLsb = (ADBuffer[gl_nDrawIndex][DataIndex])&MASK_MSB; // 从缓冲区中获得最终的原始码数据LSB					
					ADData = ADLsb - gl_MiddleLsb[Channel]; // 将原始码数据减去平移电压码值
					swprintf_s(str, _T(" %8d"), ADData); // 将AD数值转换成字符串以便显示
					pDC->TextOut(-rect.left-30+(TChannel+1)*71, rect.top+nTextHeight+36, str); // text out decimal value every channel
					TChannel++;
					if (TChannel>=gl_nChannelCount) TChannel = 0;
					DataIndex++;				
				} 
				break;
				
			case SHOW_MODE_HEX: // 十六进制原码
				for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
				{
					ADLsb = ADBuffer[gl_nDrawIndex][DataIndex]; // 从缓冲区中获得最终的原始码数据LSB(未屏蔽高2位)
					swprintf_s(str, _T("%4x"),  ADLsb&MASK_MSB);
					pDC->TextOut(-rect.left-30+(TChannel+1)*81, rect.top+nTextHeight+36, str); // text out hex value every channel
					TChannel++;
					if (TChannel>=gl_nChannelCount) TChannel = 0;
					DataIndex++;				
				}
				break;
				
			case SHOW_MODE_VOLT: // 电压值
				for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
				{
					ADLsb = (ADBuffer[gl_nDrawIndex][DataIndex])&MASK_MSB; // 从缓冲区中获得最终的原始码数据LSB
					if (ADPara.InputRange==PCI8510_INPUT_0_P5000mV || ADPara.InputRange==PCI8510_INPUT_0_P10000mV)
					{
						ADData = int(ADLsb - gl_MiddleLsb[Channel]);
					}
					else
					{
						if (gl_bProcessMoveVolt)
						{
							ADData = int(ADLsb - gl_MiddleLsb[Channel])- AD_LSB_HALF; 
						}
						else
						{
							ADData = int(ADLsb - gl_MiddleLsb[Channel]); 
						}
					}								
					
					fADVolt = gl_PerLsbVolt[Channel] * ADData;
					swprintf_s(str, _T("  %8.2f"), fADVolt);
					pDC->TextOut(-rect.left-30+(TChannel+1)*71, rect.top+nTextHeight+36, str); // text out volt value
					TChannel++;
					if (TChannel>=gl_nChannelCount) TChannel = 0;
					DataIndex++;					
				}
				break;
				
			default:
				break;
			}
			
			nTextHeight += 16; // next line
		}
	}
	
gl_bDataProcessing = FALSE;  // 通知数据处理线程，数字绘制完成，可以往DrawBuffer中填充新数据
}
	


void CADDigitView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	if (point.x == -1 && point.y == -1)
	{
		//keystroke invocation
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		
		point = rect.TopLeft();
		point.Offset(5, 5);
	}
	
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_ShowMode));
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();
	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);
}

void CADDigitView::OnShowDec() // 数字十进制显示 
{
	// TODO: Add your command handler code here
	gl_DigitShowMode = SHOW_MODE_DEC;
	CADDoc* pDoc = theApp.m_pADDoc;
	pDoc->UpdateAllViews(NULL, 1, NULL);  // 传递1，要求重绘数字视图
}

void CADDigitView::OnUpdateShowDec(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (gl_DigitShowMode == SHOW_MODE_DEC);
}

void CADDigitView::OnShowHex() // 数字十六进制显示
{
	// TODO: Add your command handler code here
	gl_DigitShowMode = SHOW_MODE_HEX;
	CADDoc* pDoc = theApp.m_pADDoc;
	pDoc->UpdateAllViews(NULL, 1, NULL);  // 传递1，要求重绘数字视图
}

void CADDigitView::OnUpdateShowHex(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (gl_DigitShowMode == SHOW_MODE_HEX);
}

void CADDigitView::OnShowVolt() 
{
	// TODO: Add your command handler code here
	gl_DigitShowMode = SHOW_MODE_VOLT;
	CADDoc* pDoc = theApp.m_pADDoc;
	pDoc->UpdateAllViews(NULL, 1, NULL);  // 传递1，要求重绘数字视图
}

void CADDigitView::OnUpdateShowVolt(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_DigitShowMode == SHOW_MODE_VOLT);
}

void CADDigitView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate();
	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
}

int CADDigitView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;	
	
	return 0;
}

void CADDigitView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (lHint == PROC_MODE_DIGIT)  // 如果是数字方式，就执行父类的OnUpdate来调用其OnDraw
	{
		CScrollView::OnUpdate(pSender, lHint, pHint);
	}
}

void CADDigitView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	CADDoc* pDoc = GetDocument();
	CRect rect;
	GetWindowRect(&rect);
	pDoc->m_nADDigitWidth = rect.Width()+7;
}

BOOL CADDigitView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CBrush brush;
	CRect rect;
	GetClientRect(rect);
	brush.CreateSolidBrush(RGB(0, 0, 0));
	pDC->FillRect(rect, &brush);
	return TRUE;
	//		return CScrollView::OnEraseBkgnd(pDC);
}

LRESULT CADDigitView::ShowDigit(WPARAM wParam, LPARAM lParam)
{
	this->Invalidate(); // 刷新视图
	return 1;
}

void CADDigitView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default	
	switch(nChar)
	{
	case VK_HOME:
		SendMessage(WM_VSCROLL, SB_TOP, NULL);
		break;
	case VK_END:
		SendMessage(WM_VSCROLL, SB_BOTTOM, NULL);
		break;
	case VK_PRIOR:
		SendMessage(WM_VSCROLL, SB_PAGEUP, NULL);
		break;
	case VK_NEXT:
		SendMessage(WM_VSCROLL, SB_PAGEDOWN, NULL);
		break;
	case VK_UP:
		SendMessage(WM_VSCROLL, SB_LINEUP, NULL);
		break;
	case VK_DOWN:
		SendMessage(WM_VSCROLL, SB_LINEDOWN, NULL);
		break;
	case VK_LEFT:
		SendMessage(WM_HSCROLL, SB_LINELEFT, NULL);
		break;
	case VK_RIGHT:
		SendMessage(WM_HSCROLL, SB_LINERIGHT, NULL);
		break;
	case 'A': // 测试用
		SetScrollPos(SB_VERT, 200, TRUE);
		Invalidate();
		break;
	default:
		break;
	}
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CADDigitView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if (zDelta > 0)
	{
		SendMessage(WM_VSCROLL, SB_LINEUP, NULL);
	}
	else
	{
		SendMessage(WM_VSCROLL, SB_LINEDOWN, NULL);
	}
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

void CADDigitView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate();
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}
