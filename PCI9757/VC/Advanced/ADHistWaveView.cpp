// HistWaveView.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADHistWaveView
IMPLEMENT_DYNCREATE(CADHistWaveView, CScrollView)

CADHistWaveView::CADHistWaveView()
{
	m_ScreenVolume = (int)AD_VOLT_RANGE;  // 定义量程为10V	
	m_ChannelCount = 0;
}

CADHistWaveView::~CADHistWaveView()
{
}

BEGIN_MESSAGE_MAP(CADHistWaveView, CScrollView)
	//{{AFX_MSG_MAP(CADHistWaveView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_HSCROLL()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADHistWaveView drawing

void CADHistWaveView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	// TODO: calculate the total size of this view
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument(); 
	CADHistFrm* pHistFrm = (CADHistFrm*)GetParentFrame();
	m_ChannelCount = pDoc->m_ChannelCount;
	CSize sizeTotal;
	sizeTotal.cx = 0; 
	sizeTotal.cy = 0;
	SetScrollSizes(MM_TEXT, sizeTotal);
	CPoint point;
	pDoc->m_hWndWave = m_hWnd; 
	point.x = (LONG)pDoc->m_Header.nTriggerPos;
	CSliderCtrl* pSliderOffset = (CSliderCtrl*)(pHistFrm->m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));	
	pSliderOffset->SetPos(point.x - 100);	// 屏幕显示在触发点位置处

	// 创建绘图窗体
	CRect rect;
 	GetClientRect(rect);
  	m_ADHistScope.Create(WS_CHILD | WS_VISIBLE, rect, this);
 	m_ADHistScope.SetXUnits("X");
 	m_ADHistScope.SetYUnits("Y");
 	m_ADHistScope.SetRange(-5, 5, 1);
 	m_ADHistScope.InvalidateCtrl();

  	m_ScrollBar.Create(SBS_HORZ | WS_CHILD | WS_VISIBLE, CRect(rect.left, rect.bottom+20, rect.right, rect.bottom), this, IDC_SCROLLBAR_ScreenCur);
	m_ScrollBar.SetScrollRange(0, (4096/m_ChannelCount-512)); // 设置滑动条范围（字）

}

void CADHistWaveView::OnDraw(CDC *pDC)
{
	
}

void CADHistWaveView::OnDrawPolyLine()
{
	m_ADHistScope.AppendPoly(gl_Offset);
}

/////////////////////////////////////////////////////////////////////////////
// CADHistWaveView diagnostics

#ifdef _DEBUG
void CADHistWaveView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CADHistWaveView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADHistWaveView message handlers

void CADHistWaveView::OnSize(UINT nType, int cx, int cy) 
{
	CScrollView::OnSize(nType, cx, cy);

	if (m_ADHistScope.m_hWnd != NULL)
	{
		m_ADHistScope.SetWindowPos(NULL, 0, 0, cx, cy-20, SWP_DRAWFRAME);
		m_ScrollBar.SetWindowPos(NULL, 0, cy-20, cx, 20, SWP_NOOWNERZORDER);
		this->OnDrawPolyLine();
	}
}

void CADHistWaveView::OnDestroy() 
{
	CScrollView::OnDestroy();	
}


void CADHistWaveView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	CADHistFrm* pHistFrm = (CADHistFrm*)GetParentFrame();
	int ChannelCount = pDoc->m_ChannelCount;	
	int Pos = m_ScrollBar.GetScrollPos();  // 取得滑动条位置（即文件偏移（字））
	int nPageSize = 8192/m_ChannelCount/20;
	switch(nSBCode)
    {
	case SB_LINERIGHT: 
		Pos += 1;        
		break;

	case SB_LINELEFT:   
		Pos -= 1;        
		break;

	case SB_PAGELEFT:    
		Pos -= nPageSize;     
		break;

	case SB_PAGERIGHT:  
		Pos += nPageSize;     
		break;

	case SB_THUMBPOSITION:
		Pos = nPos;  
		break;
	case SB_THUMBTRACK:
		Pos = nPos;
		break;

	default : 
		break;

    }

	m_ScrollBar.SetScrollPos(Pos);
	
	pDoc->m_ScreenOffset = Pos - Pos%ChannelCount;		// 文档中的屏幕偏移等于滑尺的屏幕位置偏移 
	pDoc->m_Offset = Pos;

	CString str;	
	CEdit* pEditOffset = (CEdit*)(pHistFrm->m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset));	
	CEdit* pEditScreenPos = (CEdit*)(pHistFrm->m_wndSetupBar.GetDlgItem(IDC_EDIT_ScreenPos));
	str.Format(_T("%d") , Pos);
	pEditOffset->SetWindowText(str);  // 显示文件偏移
 	str.Format(_T("%d") , Pos);
	pEditScreenPos->SetWindowText(str);    

	pHistFrm->RedrawDataWindow();  // 重绘数据视图
	m_ADHistScope.AppendPoly(pDoc->m_ScreenOffset);

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CADHistWaveView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;
	GetClientRect(rect);
	pDC->FillSolidRect(rect, RGB(0, 0, 0));
	return TRUE;
	
	// 	return CScrollView::OnEraseBkgnd(pDC);
}
