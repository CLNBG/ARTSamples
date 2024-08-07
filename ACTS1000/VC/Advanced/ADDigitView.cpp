// ADDigitView.cpp : implementation of the CADDigitView class
//
#include "stdafx.h"
#include "Sys.h"
#include "ADDoc.h"
#include "ADDigitView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int gl_InputRange[MAX_CHANNEL_COUNT];
/////////////////////////////////////////////////////////////////////////////
// CADDigitView

IMPLEMENT_DYNCREATE(CADDigitView, CScrollView)

BEGIN_MESSAGE_MAP(CADDigitView, CScrollView)
//{{AFX_MSG_MAP(CADDigitView)
ON_WM_CONTEXTMENU()
ON_WM_CANCELMODE()
ON_COMMAND(IDM_SixteenShow, OnSixteenShow)
ON_UPDATE_COMMAND_UI(IDM_SixteenShow, OnUpdateSixteenShow)
ON_COMMAND(IDM_TenShow, OnTenShow)
ON_UPDATE_COMMAND_UI(IDM_TenShow, OnUpdateTenShow)
ON_COMMAND(IDM_DigitalVoltShowMode, OnDigitalVoltShowMode)
ON_UPDATE_COMMAND_UI(IDM_DigitalVoltShowMode, OnUpdateDigitalVoltShowMode)
ON_WM_VSCROLL()
ON_WM_CREATE()
ON_WM_SIZE()
ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
ON_MESSAGE(WM_SHOWDIGIT, ShowDigit)
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
	m_DigitalDataShowMode = VOLT_DATAMODE; // 电压值显示模式
	CurrentProgress = 0; // 置采集进度条起值为0	
}

CADDigitView::~CADDigitView()
{
	
	
}

BOOL CADDigitView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CADDigitView drawing
void CADDigitView::OnDraw(CDC* pDC)
{  
	ADDigit(pDC);	
	if(m_bProgress) // 刷新ADDigitView视图时，更新进度条的状态
	{
		CADFrame* pShow=((CSysApp*)AfxGetApp())->m_ADFrame ; // 取得子帧窗口句柄
		pProgress=(CProgressCtrl*)(pShow->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));
		pProgress->SetPos(CurrentProgress);
		CurrentProgress++;
		if(CurrentProgress>50) CurrentProgress=0;
		m_bProgress=FALSE;
	}
}

void CADDigitView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize sizeTotal;
	// TODO: calculate the total size of this view
	const SIZE conPSize = {200,1};
	const SIZE conLSize = {20,1};
	sizeTotal.cx = 500; 
	sizeTotal.cy = 1026*16; 
	SetScrollSizes(MM_TEXT, sizeTotal, CSize(sizeDefault.cx, 36*16), CSize(sizeDefault.cx, 16));
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
void CADDigitView::ADDigit(CDC *pDC)//显示ch值 
{	

	bDrawing = TRUE; // 通知数据处理线程，正在绘制数字，请不要往DrawBuffer中填充新数据	
   	CRect rect;
	int i;
	TCHAR strIndex[512], str[512];
	int ChannelIndex = 0;
	int nChannelCount=0;
	GetClientRect(&rect);
    OnPrepareDC(pDC);
    pDC->DPtoLP(&rect);   
	CBrush brush;
	brush.CreateSolidBrush(RGB(0, 0, 0));
	pDC->FillRect(rect, &brush);
	pDC->SetTextColor(RGB(0, 255, 0));
	pDC->SetBkMode(TRANSPARENT);
	int Channel = 0;
	int nTextHeight = 0;

	ULONG ulBuffID = glBufferID;
	pDC->SetTextColor(RGB(0, 255, 0));
	nChannelCount = 0;
	_stprintf(strIndex, _T("%s   "), _T("Index"));
	pDC->TextOut(-rect.left, rect.top +20, strIndex); // first line text out "Index CHX..."
	switch(m_DigitalDataShowMode)
	{
	case DEC_DATAMODE:	// 十进制显示
		_stprintf(str, _T("%s"), _T("  十进制显示数据"));
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(-rect.left, rect.top, str);
		pDC->SetTextColor(RGB(0, 255, 0));
		for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
		{	
			if (ADPara.bChannelArray[Channel] == TRUE)
			{
				_stprintf(str, _T("CH%d"), Channel);
				pDC->TextOut(-rect.left+(nChannelCount+1)*70, rect.top +20, str); // first line text out "Index CHX..."
				nChannelCount++;
			}
		}
		break;
	case HEX_DATAMODE:	// 十六进制显示
		_stprintf(str, _T("%s"),_T("  十六进制原码显示数据"));
		pDC->SetTextColor(RGB(0, 255, 255));
		pDC->TextOut(-rect.left, rect.top, str);
		pDC->SetTextColor(RGB(0, 255, 0));
		_stprintf(strIndex, _T("%s    "), _T("Index"));
		for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
		{	
			if (ADPara.bChannelArray[Channel] == TRUE)
			{
				_stprintf(str, _T("CH%d"), Channel);
				pDC->TextOut(-rect.left+(nChannelCount+1)*70, rect.top +20, str); // first line text out "Index CHX..."
				nChannelCount++;
			}
		}

		break;
	case VOLT_DATAMODE:	// 电压值显示
		_stprintf(str, _T("%s"), _T("  电压值显示数据"));
		pDC->SetTextColor(RGB(255, 255, 0));
		pDC->TextOut(-rect.left, rect.top, str);
		pDC->SetTextColor(RGB(0, 255, 0));
		_stprintf(strIndex, _T("%s   "), _T("Index"));
		for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
		{	
			if (ADPara.bChannelArray[Channel] == TRUE)
			{
			_stprintf(str, _T("  CH%d"), Channel);
			//_stprintf(strIndex, _T("%s %8s    "), strIndex, str);
			pDC->TextOut(-rect.left+(nChannelCount+1)*70, rect.top +20, str); // first line text out "Index CHX..."
			nChannelCount++;
			}
		}
		break;
	}
	nTextHeight = 20;
	//pDC->TextOut(-rect.left, rect.top +20, strIndex); // first line text out "Index CHX..."
	
	ULONG nDataLength = MAX_RAM_SIZE/gl_nChannelCount-glOffset;
	if (gl_bSmlThenSize && ADPara.SampleMode==ACTS1000_SAMPMODE_FINITE)
	{
		nDataLength = (gl_ReadSizeWords%8192)/gl_nChannelCount-glOffset;
		if (nDataLength == 0)
			nDataLength = 8192/gl_nChannelCount;
	}

// 	if (nDataLength>m_nPlotWidth)
// 	{
// 		nDataLength = m_nPlotWidth;
// 	}

	int Index = 0;
	for(i=rect.top; i<rect.bottom; i++)
	{
		if(i%16 == 0)
		{
			Index = glOffset + i/16; // 数据点索引值
//			if(Index >= MAX_RAM_SIZE/MAX_CHANNEL_COUNT) break;
//			if(Index >= MAX_RAM_SIZE/gl_nChannelCount) break;
			if(Index >= nDataLength) break;
			_stprintf(strIndex, _T("%8d"), Index); 
			pDC->TextOut(-rect.left, rect.top+nTextHeight+16, strIndex); // text out Index of data
			nChannelCount = 0;
			switch(m_DigitalDataShowMode)
			{
			case DEC_DATAMODE: // 十进制偏移码
				for(Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
				{
					
					if (ADPara.bChannelArray[Channel] == TRUE)
					{			
						_stprintf(str, _T("   %8d"), ((ADBuffer[ulBuffID][Index*gl_nChannelCount+gl_CHIdx[Channel]])&gl_wMaxLSB)-gl_MiddleLsb[Channel]);
						pDC->TextOut(-rect.left-30+(nChannelCount+1)*70, rect.top+nTextHeight+16, str); // text out decimal value every channel
						nChannelCount++;
					}
				}
				break;
			case HEX_DATAMODE: // 十六进制原码
				for(Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
				{
					//ADBuffer[ulBuffID][Index*gl_nChannelCount+gl_CHIdx[Channel]] = 65535;
					if (ADPara.bChannelArray[Channel] == TRUE)
					{	
					_stprintf(str, _T("      %5x"),
						(ADBuffer[ulBuffID][Index*gl_nChannelCount+gl_CHIdx[Channel]])&gl_wMaxLSB);
					pDC->TextOut(-rect.left-30+(nChannelCount+1)*70, rect.top+nTextHeight+16, str); // text out hex value every channel
					nChannelCount++;
					}
				}
				break;
			case VOLT_DATAMODE: // 电压值
				for(Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
				{
					
					if (ADPara.bChannelArray[Channel] == TRUE)
					{	
						_stprintf(str, _T("     %8.2f"), (((ADBuffer[ulBuffID][Index*gl_nChannelCount+gl_CHIdx[Channel]])&gl_wMaxLSB) - 
							(gl_MiddleLsb[Channel]) - gl_lLsbHalf) * gl_PerLsbVoltM[Channel]);
						pDC->TextOut(-rect.left-30+(nChannelCount+1)*70, rect.top+nTextHeight+16, str); // text out volt value
						nChannelCount++;
					}
 				}
				break;
			default:
				break;
			}

			nTextHeight += 16; // next line
		}
	}
	bDrawing = FALSE;  // 通知数据处理线程，数字绘制完成，可以往DrawBuffer中填充新数据
}


void CADDigitView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	{
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
		VERIFY(menu.LoadMenu(IDR_ChooseMode));
		
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;
		
		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();
		
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);
	}
}

void CADDigitView::OnCancelMode() 
{
	CScrollView::OnCancelMode();	
	// TODO: Add your message handler code here
}

void CADDigitView::OnSixteenShow() //数字十六进制显示
{
	// TODO: Add your command handler code here
	m_DigitalDataShowMode = HEX_DATAMODE;
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADDoc* pDoc=pApp->m_pADDoc;
	pDoc->UpdateAllViews(NULL, 1, NULL);  // 传递1，要求重绘数字视图
}

void CADDigitView::OnTenShow()//数字十进制显示 
{
	// TODO: Add your command handler code here
	m_DigitalDataShowMode = DEC_DATAMODE;
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADDoc* pDoc=pApp->m_pADDoc;
	pDoc->UpdateAllViews(NULL, 1, NULL);  // 传递1，要求重绘数字视图
}

void CADDigitView::OnDigitalVoltShowMode() 
{//电压显示
	// TODO: Add your command handler code here
	m_DigitalDataShowMode = VOLT_DATAMODE;
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADDoc* pDoc=pApp->m_pADDoc;
	pDoc->UpdateAllViews(NULL, 1, NULL);  // 传递1，要求重绘数字视图
}

void CADDigitView::OnUpdateSixteenShow(CCmdUI* pCmdUI) 
{//十六进制
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_DigitalDataShowMode == HEX_DATAMODE);
}

void CADDigitView::OnUpdateTenShow(CCmdUI* pCmdUI) 
{//十进制显示
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck (m_DigitalDataShowMode == DEC_DATAMODE);
}

void CADDigitView::OnUpdateDigitalVoltShowMode(CCmdUI* pCmdUI) 
{//电压显示
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_DigitalDataShowMode == VOLT_DATAMODE);
}

void CADDigitView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
//	RedrawWindow();  // 当用户拖动滚动条时，要求视图重画
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
	if(lHint==1)  // 如果是数字方式，就执行父类的OnUpdate来调用其OnDraw
	{
		CScrollView::OnUpdate(pSender, lHint, pHint);
	}
}

void CADDigitView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);
	CADDoc* pDoc=GetDocument();
	CRect rect;
	GetWindowRect(&rect);
	pDoc->m_nADDigitWidth=rect.Width()+7;
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
	//	return CScrollView::OnEraseBkgnd(pDC);
}

 

LRESULT CADDigitView::ShowDigit(WPARAM wParam, LPARAM lParam)
{
	this->Invalidate(); // 刷新视图
	return 1;
}

void CADDigitView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate();
	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
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
	default:
		break;
	}
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CADDigitView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if(zDelta > 0)
	{
		SendMessage(WM_VSCROLL, SB_LINEUP, NULL);
	}
	else
	{
		SendMessage(WM_VSCROLL, SB_LINEDOWN, NULL);
	}
	return CScrollView::OnMouseWheel(nFlags, zDelta, pt);
}

