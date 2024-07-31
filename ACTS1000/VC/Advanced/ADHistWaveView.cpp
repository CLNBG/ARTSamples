// HistWaveView.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"
#include "ADHistFrm.h"
#include "ADHistWaveView.h"
#include "ADHistDigitView.h"

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
	m_ScreenVolume = (int)VOLT_RANGE;  // 定义量程为10V	
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
	pDoc->m_hWndWave = m_hWnd; 
	CSliderCtrl* pSliderOffset = (CSliderCtrl*)(pHistFrm->m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));	
	pSliderOffset->SetPos(0);	// 屏幕显示在开始处

	// 创建绘图窗体
	CRect rect;
 	GetClientRect(rect);
  	m_ADHistScope.Create(WS_CHILD | WS_VISIBLE, rect, this);
 	m_ADHistScope.SetXUnits(_T("X"));
 	m_ADHistScope.SetYUnits(_T("Y"));
	m_ADHistScope.ShowChannel(0, MAX_CHANNEL_COUNT-1);

	for (int Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++) // 初始化电压量程
	{
// 		m_ADHistScope.SetRange(pDoc->m_Header.VoltBottomRange, pDoc->m_Header.VoltTopRange, Channel);
// 		m_ADHistScope.SetVisableRange(pDoc->m_Header.VoltBottomRange, pDoc->m_Header.VoltTopRange, Channel);
		switch (pDoc->m_Header.ADPara.InputRange[Channel])
		{
		case ACTS1000_INPUT_N10000_P10000mV:	

			
				m_ADHistScope.SetRange(-10000.0, 10000.0, Channel);	
				m_ADHistScope.SetVisableRange(-10000.0, 10000.0, Channel);
				break;
		case ACTS1000_INPUT_N5000_P5000mV:	
				m_ADHistScope.SetRange(-5000.0, 5000.0, Channel);	
				m_ADHistScope.SetVisableRange(-5000.0, 5000.0, Channel);	

			break;
		case ACTS1000_INPUT_N1000_P1000mV:
			m_ADHistScope.SetRange(-1000.0, 1000.0, Channel);
			m_ADHistScope.SetVisableRange(-1000.0, 1000.0, Channel);
			break;
		}
	}

	m_ADHistScope.ShowWindow(SW_SHOWNA);

  	m_ScrollBar.Create(SBS_HORZ | WS_CHILD | WS_VISIBLE, CRect(rect.left, rect.bottom+20, rect.right, rect.bottom), this, IDC_SCROLLBAR_ScreenCur);
	m_ScrollBar.SetScrollRange(0, (8192/m_ChannelCount)); // 设置滑动条范围（字）

}

void CADHistWaveView::OnDraw(CDC *pDC)
{
	
}

void CADHistWaveView::OnDrawPolyLine()
{
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();

	if (NULL != pDoc)
	{
		LONGLONG iDataLenth = 0;
		PWORD pBuff = NULL;
		for (int iChannel=0; iChannel<pDoc->m_ChannelCount; iChannel++)
		{
			pBuff = pDoc->GetBuffer(iChannel, pDoc->m_Offset, &iDataLenth);

			if (pBuff != NULL)
			{
				m_ADHistScope.AppendPoly(pBuff, iChannel, iDataLenth, pDoc->m_Offset, FALSE);	
			}		
		}
	}
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

	case SB_PAGEDOWN:    
		Pos += nPageSize;     
		break;
		
	case SB_PAGEUP:  
		Pos -= nPageSize;     
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

	if (Pos<0)
		Pos = 0;
	m_ScrollBar.SetScrollPos(Pos);
	
	pDoc->m_ScreenOffset = Pos - Pos%ChannelCount;		// 文档中的屏幕偏移等于滑尺的屏幕位置偏移 

	LONGLONG i64Pos = Pos;
	i64Pos+= pDoc->m_Offset;
	LONGLONG ulOld = pDoc->m_Offset;


	pDoc->m_Offset = i64Pos;

	CString str;	
	CEdit* pEditOffset = (CEdit*)(pHistFrm->m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset));	
	CEdit* pEditScreenPos = (CEdit*)(pHistFrm->m_wndSetupBar.GetDlgItem(IDC_EDIT_ScreenPos));
	str.Format(_T("%I64d"), i64Pos);
	pEditOffset->SetWindowText(str);  // 显示文件偏移
 	str.Format(_T("%I64d"), i64Pos);
	pEditScreenPos->SetWindowText(str);    

 	RedrawWindow();  // 重绘数据视图
	
	LONGLONG iDataLenth = 0;
	PWORD pBuff = NULL;
	for (int iChannel=0; iChannel<pDoc->m_ChannelCount; iChannel++)
	{
		pBuff = pDoc->GetBuffer(iChannel, pDoc->m_Offset, &iDataLenth);
		if (iDataLenth == -1)
		{
			iDataLenth= 0;
		}
		
		if (pBuff != NULL)
		{
			m_ADHistScope.AppendPoly(pBuff, iChannel, iDataLenth, pDoc->m_Offset, FALSE);	
		}			
	}

	pDoc->m_Offset = ulOld;

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CADHistWaveView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}
