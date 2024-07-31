// ADHistScope.cpp : implementation file//

#include "stdafx.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
//////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CADHistScope
CADHistScope::CADHistScope()
{
	INT iScreenWidth = GetSystemMetrics(SM_CXSCREEN);  // ȡ����Ļ���
	for (INT iChannel=0; iChannel<MAX_CHANNEL_COUNT; iChannel++)
	{
		m_pPtCHannel[iChannel] = new CPoint[iScreenWidth];		
		ASSERT(NULL != m_pPtCHannel[iChannel]);
		m_bVisable[iChannel] = TRUE;
		m_iOffset[iChannel] = 0;
		m_iDataLength[iChannel] = 0;
		m_pDataBuff[iChannel] = 0;
		m_rcChannel[iChannel].SetRect(0, 0, 0, 0);
		m_arrayVisableChannel.Add(iChannel);
	}
	
	m_nChannelCount = MAX_CHANNEL_COUNT; // ȱʡʱ��ͨ����Ϊ
	m_ScreenPosition = 0.0;	
	m_nYDecimals = 3;

	for (INT ChannelCount=0; ChannelCount<MAX_CHANNEL_COUNT; ChannelCount++)
	{
		m_fLowerLimit[ChannelCount] = -1000.0;
		m_fUpperLimit[ChannelCount] =  1000.0;
		
		m_fVisableLower[ChannelCount] = -5000.0;
		m_fVisableUpper[ChannelCount] =  5000.0;
	}

	m_nShiftPixels = 4;
	m_nHalfShiftPixels = m_nShiftPixels/2;                      
	m_nPlotShiftPixels = m_nShiftPixels + m_nHalfShiftPixels;   
	
	m_crBackColor  = RGB(0, 0, 0);  
	m_crGridColor  = RGB(0, 255, 255);  
	m_crPlotColor  = RGB(255, 128, 0); 

	
	m_crLineAColor = RGB(255, 192, 255);  // A�ߵ���ɫ
    m_crLineBColor = RGB(192, 255, 255);  // B�ߵ���ɫ
    m_crLineVColor = RGB(255, 255, 128);  // C�ߵ���ɫ	
	m_nPositionA = 0;
	m_nPositionB = 0;

	m_brushBack.CreateSolidBrush(m_crBackColor);
	
	m_strXUnitsString.Format(_T("Samples"));  
	m_strYUnitsString.Format(_T("Y units"));  
	
	m_pOldBmp = NULL;
	m_bAllChannel = TRUE;
	gl_bTileWave = TRUE;
	m_bLBtnDown = FALSE;
	m_bOnlyLine = FALSE;

	m_OldMoveX1 = m_CurMoveX1 = 0;
	m_OldMoveX2 = m_CurMoveX2 = 0;
	m_CurPosition = 0;
	m_CurVoltageY = 0;
	m_OldVoltageY = 0;
	m_nLineIndex = 0xFFFF;
	m_bRBtnDown = FALSE;	

	m_hCursorHSize = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
	m_hCursorVSize = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_SIZENS);
	m_hCursorArrow = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCursorAllSize = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
	m_nShowCursor = 0; // ��ʾ���		
	m_iActivexChannel = 0;

	m_clPen[0] = RGB(255, 0, 0);
	m_clPen[1] = RGB(255, 255, 0);
	m_clPen[2] = RGB(0, 255, 0);
	m_clPen[3] = RGB(255, 0, 255);
	m_clPen[4] = RGB(255, 0, 0);
	m_clPen[5] = RGB(255, 255, 0);
	m_clPen[6] = RGB(0, 255, 0);
	m_clPen[7] = RGB(255, 0, 255);
}  

/////////////////////////////////////////////////////////////////////////////

CADHistScope::~CADHistScope()
{
	if (m_pOldBmp != NULL)
		m_dcMem.SelectObject(m_pOldBmp);  	
	m_pOldBmp = NULL;
	
	for (INT iChannel=0; iChannel<MAX_CHANNEL_COUNT; iChannel++)
	{
		if (NULL != m_pPtCHannel[iChannel])
		{
			delete [] m_pPtCHannel[iChannel];
			m_pPtCHannel[iChannel] = NULL;
		}
	}	
} // ~CADHistScope


BEGIN_MESSAGE_MAP(CADHistScope, CWnd)
//{{AFX_MSG_MAP(CADHistScope)
ON_WM_PAINT()
ON_WM_SIZE()
ON_WM_CREATE()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_WM_SETCURSOR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CADHistScope message handlers

/////////////////////////////////////////////////////////////////////////////
BOOL CADHistScope::Create(DWORD dwStyle, const RECT& rect, 
                         CWnd* pParentWnd, UINT nID) 
{
	BOOL result;
	static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS);
	
	result = CWnd::CreateEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 
							className, NULL, dwStyle, 
							rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
							pParentWnd->GetSafeHwnd(), (HMENU)nID);
	
	return result;
	
} // Create

/////////////////////////////////////////////////////////////////////////////
void CADHistScope::SetRange(float fLower, float fUpper, INT nChannel)
{ 
	// ���ô�ֱ�����������̵Ĵ�С
	ASSERT(fUpper > fLower);
	
	m_fLowerLimit[nChannel] = fLower;
	m_fUpperLimit[nChannel] = fUpper;

	// ����������ʾ
	this->ProssDataToPT(nChannel);	
	this->Invalidate();
	
}

void CADHistScope::SetVisableRange(float fLower, float fUpper, INT nChannel)
{
	ASSERT(fUpper > fLower);
	m_fVisableLower[nChannel] = fLower;
	m_fVisableUpper[nChannel] = fUpper;	
	
	// �������� ��ʾ
	this->ProssDataToPT(nChannel);	
	this->Invalidate();
}

void CADHistScope::SetXUnits(CString string)
{
	m_strXUnitsString = string;
	this->Invalidate();	
}

void CADHistScope::SetYUnits(CString string)
{
	m_strYUnitsString = string;
	
	this->Invalidate();	
}  

void CADHistScope::SetGridColor(COLORREF color)
{
	m_crGridColor = color;
	this->Invalidate();
}  

void CADHistScope::SetPlotColor(COLORREF color)
{
	m_crPlotColor = color;		
}  

void CADHistScope::SetBackgroundColor(COLORREF color)
{
	m_crBackColor = color;
	
	m_brushBack.DeleteObject();
	m_brushBack.CreateSolidBrush(m_crBackColor);
	
	this->Invalidate();
}

void CADHistScope::OnPaint() 
{ 
	CPaintDC dc(this);
	CADHistWaveView* pView = (CADHistWaveView*)GetParent();
	CADHistDoc* pDoc = (CADHistDoc*)pView->GetDocument();
	m_nChannelCount = pDoc->m_ChannelCount;
	memcpy(m_ChCfg, pDoc->m_ChCfg, sizeof(m_ChCfg));
	m_wMaxLSB = pDoc->m_wMaxLSB;
	m_fLsbCount = pDoc->m_fLsbCount;
	m_lLsbHalf = pDoc->m_lLsbHalf;
	
	if (NULL == m_dcMem.m_hDC)
	{
		m_dcMem.CreateCompatibleDC(&dc);
	}
	
	if (NULL == m_bmpMem.m_hObject)
	{
		m_bmpMem.CreateCompatibleBitmap(&dc, m_rectClient.Width(), m_rectClient.Height());
		m_pOldBmp = m_dcMem.SelectObject(&m_bmpMem);
	}
	
	this->DrawBkGnd();   // ���Ʊ�����С�仯ʱͨ������������ֻ��Ʊ���
	
	this->DrawPoly();    // ��С�����ݱ仯ʱ����
	
	m_dcMem.SetBkMode(TRANSPARENT);		                // ���ñ���Ϊ͸��
	INT nOldDrawMode = m_dcMem.SetROP2(R2_XORPEN);      // ���û���ģʽ
	
	m_dcMem.SelectObject(&m_PenLineA);                  // ѡ����A�Ļ���
	m_dcMem.MoveTo(m_OldMoveX1, m_rectPlot.top);
	m_dcMem.LineTo(m_OldMoveX1, m_rectPlot.bottom);     // ����A
	
	TCHAR str[100];
	swprintf_s(str, _T("%s"), _T("A��"));
	m_dcMem.SetTextColor(RGB(255, 0, 0));
	m_dcMem.TextOut(m_OldMoveX1+20, m_rectPlot.top+30, str);
	
	m_dcMem.SelectObject(&m_PenLineB);				    // ѡ������B�Ļ���
	m_dcMem.MoveTo(m_OldMoveX2, m_rectPlot.top);
	m_dcMem.LineTo(m_OldMoveX2, m_rectPlot.bottom);     // ����B

	swprintf_s(str, _T("%s"), _T("B��"));
	m_dcMem.SetTextColor(RGB(255, 0, 0));
	m_dcMem.TextOut(m_OldMoveX2+20, m_rectPlot.top+30, str);
	
	if (FALSE == m_bAllChannel)
	{
		m_dcMem.SelectObject(&m_PenLineV);				// ѡ������V�Ļ���
		m_dcMem.MoveTo(m_rectPlot.left, m_OldVoltageY);
		m_dcMem.LineTo(m_rectPlot.right, m_OldVoltageY); // ����V
	}
	m_dcMem.SetROP2(nOldDrawMode);
	
	dc.BitBlt(m_rectClient.left, 
			  m_rectClient.top, 
			  m_rectClient.Width(), 
			  m_rectClient.Height(), 
			  &m_dcMem, 
			  0, 0, SRCCOPY);       //SRCCOPY);
	UpdataChannelRect();
} 

/////////////////////////////////////////////////////////////////////////////
void CADHistScope::OnSize(UINT nType, INT cx, INT cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	GetClientRect(m_rectClient); // ��ȡ��ǰ�Ŀͻ�����С
	
	m_rectPlot.left = 60;  
	m_rectPlot.top = 10;
	m_rectPlot.right = m_rectClient.right-10;
	m_rectPlot.bottom = m_rectClient.bottom - 10;
	m_nPlotHeight = m_rectPlot.Height();
	m_nPlotWidth = m_rectPlot.Width();		
	
	m_OldVoltageY = m_CurVoltageY = m_rectPlot.top + 20;
	m_OldMoveX1 = m_CurMoveX1 = m_rectPlot.left + 20;
	m_OldMoveX2 = m_CurMoveX2 = m_rectPlot.right - 20;
	m_OldVoltageY = m_CurVoltageY = m_rectPlot.top + 20;
	
	if (NULL != m_pOldBmp)
	{
		m_dcMem.SelectObject(m_pOldBmp);
		m_bmpMem.DeleteObject();
		m_pOldBmp = NULL;
	}
	
	this->UpdataChannelRect();
} 

void CADHistScope::Reset()
{
	
}

INT CADHistScope::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{ 
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_rectClient = CRect(0, 0, 1024, 1000);  
	
	m_rectPlot.left = 20;  
	m_rectPlot.top = 10;
	m_rectPlot.right = m_rectClient.right - 10;
	m_rectPlot.bottom = m_rectClient.bottom - 25;
	
	m_nPlotHeight = m_rectPlot.Height();
	m_nPlotWidth  = m_rectPlot.Width();
	
	
	m_PenLineA.CreatePen(PS_DOT, 1, m_crLineAColor);
	m_PenLineB.CreatePen(PS_DOT, 1, m_crLineBColor);
	m_PenLineV.CreatePen(PS_DOT, 1, m_crLineVColor);
	
	for (INT Channel=0; Channel<m_nChannelCount; Channel++)
	{	

		if (m_penChannel[Channel].m_hObject == NULL)
			m_penChannel[Channel].CreatePen(PS_SOLID, 1, m_clPen[Channel%COLR_COUNT]);
	}
	
	return 0;
}

// ����������������
void CADHistScope::DrawBkGnd()  
{ 
	CPen *oldPen;
	CPen solidPen(PS_SOLID, 0, m_crGridColor);
	CFont axisFont, yUnitFont, *oldFont;
	CString strTemp;
	
	m_dcMem.SetBkColor (m_crBackColor);	
	m_dcMem.FillRect(m_rectClient, &m_brushBack);	  
	
	// �����ܵĿ��	
	oldPen = m_dcMem.SelectObject(&solidPen);  // �����ܵķ���
	m_dcMem.MoveTo(m_rectPlot.left-1, m_rectPlot.top);
	m_dcMem.LineTo(m_rectPlot.right+1, m_rectPlot.top);
	m_dcMem.LineTo(m_rectPlot.right+1, m_rectPlot.bottom+1);
	m_dcMem.LineTo(m_rectPlot.left-1, m_rectPlot.bottom+1);
	m_dcMem.LineTo(m_rectPlot.left-1, m_rectPlot.top);
	m_dcMem.SelectObject (oldPen);  
	COLORREF m_Grid = RGB(200, 200, 200);
	INT Width=50;
	for ( Width=50; Width<m_rectPlot.Width(); Width+= 50) // �ڸ�50�����ػ�һ����ֱ����
	{
		for (INT Height=5; Height<m_rectPlot.Height(); Height+= 5)
		{
			m_dcMem.SetPixelV(CPoint(m_rectPlot.left + Width, m_rectPlot.top + Height), m_Grid);
		}
	}
	
	for (Width=30; Width<m_rectPlot.Height(); Width+= 30) // �ڸ�30�����ػ�һ��ˮƽ�������
	{
		for (INT Height=0; Height<m_rectPlot.Width(); Height+=10)
		{
			m_dcMem.SetPixelV(CPoint(m_rectPlot.left + Height, m_rectPlot.top + Width), m_Grid);
		}
	}	
	
	// ��ÿ��ͨ���ķֽ���(ˮƽ��)	
	if (m_bAllChannel) // ��ʾ����ͨ��
	{
		if (gl_bTileWave) // �����ƽ����ʾ
		{
			DrawAllChannelGrid(&m_dcMem); // ������ͨ���ĺ�������
		}
		else
		{
			DrawSingleCHGrid(&m_dcMem);
		}
	}
	else // ����ͨ���ĺ���������
	{
		DrawSingleCHGrid(&m_dcMem);
	}
	
	axisFont.CreateFont (14, 0, 0, 0, 300,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, 
		DEFAULT_PITCH|FF_SWISS, _T("Arial"));
	
	yUnitFont.CreateFont (14, 0, 900, 0, 300,
		FALSE, FALSE, 0, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, 
		DEFAULT_PITCH|FF_SWISS, _T("Arial"));
	
	oldFont = m_dcMem.SelectObject(&axisFont);
	m_dcMem.SetTextAlign (TA_RIGHT|TA_TOP); // ���뷽ʽ	
	
	if (m_bAllChannel)
	{
		if (gl_bTileWave) // ͬ���ɼ�ƽ����ʾ
		{
			DrawAllChannelText(&m_dcMem);
		}
		else // ������ʾ
		{
			DrawSingleCHText(&m_dcMem, 0xFFFF);
		}
		
	}
	else
	{
		DrawSingleCHText(&m_dcMem, m_iActivexChannel);
	}
}

// ��PolyLine��
void CADHistScope::OnMouseMove(UINT nFlags, CPoint point) 
{
	CADHistWaveView* pView = (CADHistWaveView*)GetParent();
	CADHistDoc* pDoc = (CADHistDoc*)pView->GetDocument();
 	CClientDC dc(this);
	if (m_rectPlot.PtInRect(point))
	{
		if (!m_bRBtnDown) m_nShowCursor = 0;
		// ͨ������λ���ж�����һ���߻�ý���
		if (abs(point.x - m_OldMoveX1) < 5) 
		{
			m_nShowCursor = 1;
		}
		if (abs(point.x - m_OldMoveX2) < 5) 
		{
			m_nShowCursor = 2;
		}	
		if (!m_bAllChannel && abs(point.y - m_OldVoltageY) < 5) 
		{
			m_nShowCursor = 3;
		}
		
		if (m_bLBtnDown || m_bRBtnDown) 
		{
			DrawMoveLine(&dc, point);
		}
	}

	if (point.x >= 60)
	{
		m_MouseOffset = point.x - 60;
	}
	else
		m_MouseOffset = 0;

	CString str;
	CADHistFrm* pADHistFrm = (CADHistFrm*)GetParentFrame();	
	CEdit* pOffsetEdit = (CEdit*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_EDIT_ScreenPos));

	CEdit* pEditOffset = (CEdit*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset));	

	pEditOffset->GetWindowText(str);
	LONGLONG lOffset = _tcstoi64(str, NULL, 10);

//	str.Format(_T("%d"), m_MouseOffset + pDoc->m_Offset);
	str.Format(_T("%I64d"), lOffset + m_MouseOffset);
	pOffsetEdit->SetWindowText(str);  // ��ʾ�ļ�ƫ��
	
	CWnd::OnMouseMove(nFlags, point);
}

void CADHistScope::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bLBtnDown = TRUE;
	if (m_rectPlot.PtInRect(point))
	{
		// ͨ������λ���ж�����һ���߻�ý���
		if (abs(point.x - m_OldMoveX1) < 5) 
		{
			m_nLineIndex = 1;
			m_bOnlyLine = TRUE;
		}

		if (abs(point.x - m_OldMoveX2) < 5) 
		{
			m_nLineIndex = 2;
			m_bOnlyLine = TRUE;
		}
		
		if (!m_bAllChannel && abs(point.y - m_OldVoltageY) < 5) 
		{
			m_nLineIndex = 3;
			m_bOnlyLine = TRUE;
		}
	}
	SetCapture();
	CWnd::OnLButtonDown(nFlags, point);
}

// ��ԭ��ֵ����(buffer������ԭ��), �ٴ���ɿ�����ʾ�ĵ�����
void CADHistScope::AppendPoly(PWORD pBuff, const INT iChannel, LONGLONG iDataSize, LONGLONG iOffset, BOOL bUpdata)
{
	ASSERT(iChannel >= 0 && iChannel < MAX_CHANNEL_COUNT);
	ASSERT(iDataSize >=0 && iOffset >= 0);
	ASSERT(NULL != pBuff);	
	if (iDataSize > 8192/m_nChannelCount)
		iDataSize = 8192/m_nChannelCount;
	if (iChannel >= 0 && iChannel < MAX_CHANNEL_COUNT)
	{
		m_iDataLength[iChannel] = (INT)iDataSize;
		m_iOffset[iChannel] = iOffset;     // ƫ��	
		m_pDataBuff[iChannel] = pBuff; 
		if (FALSE == m_bAllChannel && m_iActivexChannel == iChannel)
		{
			this->ProssDataToPT(iChannel);	
		}
		else
		{
			this->ProssDataToPT(iChannel);	
		}		
		
		if (TRUE == bUpdata)
		{
			this->Invalidate();
		}
	}
}

// ��������ĵ�
void CADHistScope::DrawPoly()
{
	CPen* pOldPen;
	CADHistWaveView* pView = (CADHistWaveView*)GetParent();
	CADHistDoc* pDoc = (CADHistDoc*)pView->GetDocument();
	LONG DataCount = (LONG)((pDoc->m_FileLength-sizeof(::_FILE_HEADER)) / (sizeof(WORD) * pDoc->m_ChannelCount));
	if (m_bAllChannel) // ����ͨ����ʾ�������ʾʱ
	{
		int iChannel = 0;
		for (INT index=0; index<m_arrayVisableChannel.GetSize(); index++) // ������ͨ���ĵ�
		{
			iChannel = m_arrayVisableChannel[index];
			if (NULL != m_pDataBuff[iChannel])
			{
				m_dcMem.SaveDC();
				if (FALSE == gl_bTileWave)
				{
					m_dcMem.IntersectClipRect(m_rectPlot);
				}
				else
				{
					m_dcMem.IntersectClipRect(m_rcChannel[iChannel]);
				}
				
				pOldPen = m_dcMem.SelectObject(&m_penChannel[iChannel % 4]);
				m_dcMem.MoveTo(m_pPtCHannel[iChannel][0]); // �ƶ�����һ����								
				if (m_iDataLength[iChannel] >= m_rectPlot.Width())
				{
					m_dcMem.Polyline(m_pPtCHannel[iChannel], m_rectPlot.Width());
				}
				else
				{
					m_dcMem.Polyline(m_pPtCHannel[iChannel], m_iDataLength[iChannel]-1);
				}
				
				m_dcMem.SelectObject(pOldPen);
				m_dcMem.RestoreDC(-1);
			}			
		}
	}
	else // ��ͨ����ʾ
	{	
		if (NULL != m_pDataBuff[m_iActivexChannel])
		{	
			m_dcMem.SaveDC();
			m_dcMem.IntersectClipRect(m_rectPlot);
			pOldPen = m_dcMem.SelectObject(&m_penChannel[m_iActivexChannel % 4]);
			m_dcMem.MoveTo(m_pPtCHannel[m_iActivexChannel][0]); // �ƶ�����һ����
			
			if (m_iDataLength[m_iActivexChannel] >= m_rectPlot.Width())
			{
				m_dcMem.Polyline(m_pPtCHannel[m_iActivexChannel], m_rectPlot.Width());
			}
			else
			{
				m_dcMem.Polyline(m_pPtCHannel[m_iActivexChannel], m_iDataLength[m_iActivexChannel]);
			}

			m_dcMem.SelectObject(pOldPen);
			m_dcMem.RestoreDC(-1);
			
		}
	}
}

void CADHistScope::OnLButtonUp(UINT nFlags, CPoint point) 
{	
	m_bOnlyLine = FALSE;
	m_bLBtnDown = FALSE; 
	::ReleaseCapture();

	static BOOL bFirstPosition = TRUE;  // ��ʼ��ı�־
	CADHistFrm* pFrame = (CADHistFrm*)GetParentFrame();
	CADHistWaveView* pView = (CADHistWaveView*)GetParent();
	CADHistDoc* pDoc = (CADHistDoc*)pView->GetDocument();
	CButton* pRadioA = (CButton*)pFrame->m_wndSetupBar.GetDlgItem(IDC_RADIO_PositionA);
	CButton* pRadioB = (CButton*)pFrame->m_wndSetupBar.GetDlgItem(IDC_RADIO_PositionB);
	CStatic* pStaticA = (CStatic*)pFrame->m_wndSetupBar.GetDlgItem(IDC_STATIC_PositionA);
	CStatic* pStaticB = (CStatic*)pFrame->m_wndSetupBar.GetDlgItem(IDC_STATIC_PositionB);
	CStatic* pDistanceAB = (CStatic*)pFrame->m_wndSetupBar.GetDlgItem(IDC_STATIC_DistanceAB);
	CString str;
	
	if (abs(point.x - m_OldMoveX1)<5 || abs(point.x - m_OldMoveX2)<5)
	{
		m_CurPosition = pDoc->m_Offset + point.x - m_rectPlot.left;
		m_ScreenPosition = point.x - m_rectPlot.left;
	}
	
	str.Format(_T("%I64d"), m_CurPosition);
	pRadioA->SetCheck(m_nLineIndex == 1 ? 1:0);
	pRadioB->SetCheck(m_nLineIndex == 2 ? 1:0);

	if (pRadioA->GetCheck())
	{
		pStaticA->SetWindowText(str);
	}

	if (pRadioB->GetCheck())
	{
		pStaticB->SetWindowText(str);
	}

	int nDistanceAB = 0;
	pStaticA->GetWindowText(str);
	m_nPositionA = _tcstol(str, NULL, 10);
	pStaticB->GetWindowText(str);
	m_nPositionB = _tcstol(str, NULL, 10);
	nDistanceAB = abs(m_nPositionA - m_nPositionB);
	str.Format(_T("%d"), nDistanceAB);
	pDistanceAB->SetWindowText(str);

	SetStatusBar();
	bFirstPosition = !bFirstPosition;
	Invalidate();
	CWnd::OnLButtonUp(nFlags, point);
}

void CADHistScope::SetStatusBar()
{	

	float fFrequency;
	CString strTimeDiv;
	CString strCountTimer;
	CString strFrequency;
	CString strVol;
	int SubstValue = abs(m_nPositionA - m_nPositionB);
	CSysApp *pApp = (CSysApp *)AfxGetApp();
	CADHistWaveView* pView = (CADHistWaveView*)GetParent();
	CADHistDoc* pDoc = (CADHistDoc*)pView->GetDocument();
	CADHistFrm* pFrame = (CADHistFrm*)GetParentFrame();

	int ChannelCount = m_nChannelCount;
	LONG nHistFrequency = pDoc->m_Header.ADMainInfo.nBaseRate/pDoc->m_Header.ADPara.FreqDivision; // ���ļ�ͷ��ȡ�ò���Ƶ��
	

	if(SubstValue)
	{
		//fCalFrequency = (float)((80000000/nHistFrequency)/nDistanceAB);
		fFrequency = (float)(nHistFrequency/SubstValue);
		if(fFrequency < 1000)
			strFrequency.Format(_T("%7.2f Hz"), fFrequency);
		if(fFrequency > 1000 && fFrequency < 1000000)
			strFrequency.Format(_T("%7.2f KHz"), fFrequency / 1000);
		if(fFrequency > 1000000)
			strFrequency.Format(_T("%7.2f MHz"), fFrequency / 1000000);		

		CStatic* pStaticFre = (CStatic*)pFrame->m_wndSetupBar.GetDlgItem(IDC_STATIC_Frequency);
		pStaticFre->SetWindowText(strFrequency);
	}
	
}

// ������ͨ����������
void CADHistScope::DrawAllChannelGrid(CDC *pDC)
{
	INT Point, ChannelCount;
	INT nGridPix = 0; // Y����������ߵĿ̶�
	float hight = (float)(m_rectPlot.Height() / float(m_nChannelCount)); // ÿͨ����Y���

	for (ChannelCount=1; ChannelCount<m_nChannelCount; ChannelCount++) // ��m_nChannelCount-1��������
	{
		nGridPix = m_rectPlot.top + (INT)(m_rectPlot.Height() * ChannelCount) / m_nChannelCount;
		for (Point=m_rectPlot.left; Point<m_rectPlot.right; Point+=2) // ÿ��2�����ػ�1��
		{
			pDC->SetPixel(Point, nGridPix, m_crGridColor); 
		}		
	}

	for (ChannelCount=0; ChannelCount<m_nChannelCount; ChannelCount++) // ��ÿͨ��������
	{
		for (Point=m_rectPlot.left; Point<m_rectPlot.right; Point+=3) // ÿ��3�����ػ�1��
		{
			pDC->SetPixel(Point, (INT)(m_rectPlot.top + (hight * ChannelCount) + hight/2.0), RGB(255, 255, 255)); 
		}
	}
}

void CADHistScope::DrawSingleCHGrid(CDC *pDC)
{
	for (INT Point=m_rectPlot.left; Point<m_rectPlot.right; Point+= 2) // ÿ��4�����ػ�1��
		pDC->SetPixel(Point, m_rectPlot.top+(INT)(m_rectPlot.Height()/2), RGB(255, 255, 255));
}

void CADHistScope::DrawAllChannelText(CDC *pDC)
{
	CString str;
	int iChannel = 0;

	for (INT index=0; index<m_nChannelCount; index++)
	{
		iChannel = m_arrayVisableChannel[index];
		pDC->SetTextColor(m_clPen[m_ChCfg[iChannel]]); // �������ֵ���ɫ

		str.Format (_T("%.*lf V"), m_nYDecimals, m_fVisableUpper[m_ChCfg[iChannel]] / 1000.0); // ����ѹֵ
		pDC->TextOut (m_rectPlot.left-4, (INT)(m_rcChannel[iChannel].top + 8), str); 
		
		pDC->SetTextAlign (TA_RIGHT | TA_BASELINE);
		str.Format (_T("%.*lf V"), m_nYDecimals, m_fVisableLower[m_ChCfg[iChannel]] / 1000.0); // ����ѹֵ
		pDC->TextOut (m_rectPlot.left-4, (INT)(m_rcChannel[iChannel].bottom - 5), str);

		
		str.Format(_T("CH %d"), m_ChCfg[iChannel]);
		pDC->TextOut(m_rectPlot.left - 4, (INT)(m_rcChannel[iChannel].CenterPoint().y), str); 
	}		
}

void CADHistScope::DrawSingleCHText(CDC *pDC, INT nChannelNum)
{
	CString str;
	if (nChannelNum != 0xFFFF)
		pDC->SetTextColor(m_clPen[nChannelNum % COLR_COUNT]);
	else
		pDC->SetTextColor(m_crGridColor);

	if (nChannelNum != 0xFFFF)
	{
		str.Format(_T("%.*lf mV"), 0, m_fVisableUpper[nChannelNum]); // ����ѹֵ
		pDC->TextOut (m_rectPlot.left - 4, m_rectPlot.top, str); 
		pDC->SetTextAlign(TA_RIGHT|TA_BASELINE);

		str.Format (_T("%.*lf mV"), 0, m_fVisableLower[nChannelNum]); // ����ѹֵ	
		pDC->TextOut (m_rectPlot.left - 4, m_rectPlot.top + m_nPlotHeight, str);
	}
	
	if (nChannelNum == 0xFFFF) // ����ǵ�����ʾʱ
		str = "CH";
	else
		str.Format(_T("CH %d"), nChannelNum); 
	// дͨ����
	pDC->TextOut(m_rectPlot.left - 4, m_rectPlot.top + m_nPlotHeight / 2 + 5, str);      
	
}

void CADHistScope::OnRButtonDown(UINT nFlags, CPoint point) 
{
	HCURSOR hCursor;
	m_bRBtnDown = TRUE;
	INT DistanceSub = abs(point.x - m_OldMoveX1) + abs(point.x - m_OldMoveX2);
	INT DistanceAB = abs(m_OldMoveX1 - m_OldMoveX2);
	if (DistanceSub == DistanceAB) // �������ʱ������������м�
	{
		m_nLineIndex = 0; // ͬʱѡ��������ʱ�ı�־ֵ
		m_nShowCursor = 4;
		hCursor = AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
		::SetCursor(hCursor);
		
		m_RBtnDownPosX = point.x;
		m_StartOldMoveX1 = m_OldMoveX1; // ���������ƶ�ʱ������1����ʼλ��
		m_StartOldMoveX2 = m_OldMoveX2; // ���������ƶ�ʱ������2����ʼλ��
		
		CRect ClipRect = m_rectPlot;
		ClientToScreen(&ClipRect);
		if (m_OldMoveX1 < m_OldMoveX2) // �����1����2�����
		{
			ClipRect.left += point.x - m_OldMoveX1;
			ClipRect.right -= m_OldMoveX2 - point.x;
		}
		else // �����1����2���ұ�
		{
			ClipRect.left += point.x - m_OldMoveX2;
			ClipRect.right -= m_OldMoveX1 - point.x;
		}

		::ClipCursor(&ClipRect);  
	}
	CWnd::OnRButtonDown(nFlags, point);
}

void CADHistScope::OnRButtonUp(UINT nFlags, CPoint point) 
{
	HCURSOR hCursor;
	hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	::SetCursor(hCursor);
	
	m_bRBtnDown = FALSE;
	m_nLineIndex = 0xFFFF;
	ClipCursor(NULL);
	CWnd::OnRButtonUp(nFlags, point);
}

void CADHistScope::DrawMoveLine(CDC *pDC, CPoint point)
{
	CPen* pOldPen;
	int nOldPositionX = point.x;
	int nOldPositionY = point.y;
	int nMode;
	pDC->SetBkMode(TRANSPARENT);         // ͸������
	nMode = pDC->SetROP2(R2_XORPEN);	 // ������ģʽ	
	switch(m_nLineIndex)
	{
	case 0: // ͬʱѡ����1��2
		pOldPen = pDC->SelectObject(&m_PenLineA); 
		pDC->MoveTo(m_OldMoveX1, m_rectPlot.top); // ������һ�λ�����A
		pDC->LineTo(m_OldMoveX1, m_rectPlot.Height()+10);	
		TCHAR str[100];
		swprintf_s(str, _T("%s"), _T("A��"));
		m_dcMem.SetTextColor(RGB(0, 0, 0));
		m_dcMem.TextOut(m_OldMoveX1, m_rectPlot.top+20, str);

		pDC->SelectObject(&m_PenLineB);
		pDC->MoveTo(m_OldMoveX2, m_rectPlot.top); // ������һ�λ�����B
		pDC->LineTo(m_OldMoveX2, m_rectPlot.Height()+10);	
		
		swprintf_s(str, _T("%s"), _T("B��"));
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(m_OldMoveX1, m_rectPlot.top+20, str);

		m_OldMoveX1 = m_StartOldMoveX1 + (point.x - m_RBtnDownPosX); 
		m_OldMoveX2 = m_StartOldMoveX2 + (point.x - m_RBtnDownPosX); 		
		pDC->SelectObject(&m_PenLineA);
		pDC->MoveTo(m_OldMoveX1, m_rectPlot.top); // ����A
		pDC->LineTo(m_OldMoveX1, m_rectPlot.Height()+10);
		swprintf_s(str, _T("%s"), _T("A��"));
		m_dcMem.SetTextColor(RGB(255, 0, 0));
		m_dcMem.TextOut(m_OldMoveX1, m_rectPlot.top+20, str);
		pDC->SelectObject(&m_PenLineB);
		pDC->MoveTo(m_OldMoveX2, m_rectPlot.top); // ����B
		pDC->LineTo(m_OldMoveX2, m_rectPlot.Height()+10);
		swprintf_s(str, _T("%s"), _T("B��"));
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(m_OldMoveX1, m_rectPlot.top+20, str);
		pDC->SelectObject(pOldPen); // �ָ��ɵĻ���
		break;

	case 1: // ѡ�������1
		pOldPen = pDC->SelectObject(&m_PenLineA);
		pDC->MoveTo(m_OldMoveX1, m_rectPlot.top); // ������һ�λ�����A
		pDC->LineTo(m_OldMoveX1, m_rectPlot.Height()+10); 
		swprintf_s(str, _T("%s"), _T("A��"));
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(m_OldMoveX1, m_rectPlot.top+20, str);

		m_OldMoveX1 = point.x;
		pDC->MoveTo(point.x, m_rectPlot.top); // ����A
		pDC->LineTo(point.x, m_rectPlot.Height()+10);
		swprintf_s(str, _T("%s"), _T("A��"));
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(point.x, m_rectPlot.top+20, str);
		pDC->SelectObject(pOldPen); // �ָ��ɵĻ���
		break;

	case 2: // ѡ�������2
		pOldPen = pDC->SelectObject(&m_PenLineB);
		pDC->MoveTo(m_OldMoveX2, m_rectPlot.top); // ������һ�λ�����B
		pDC->LineTo(m_OldMoveX2, m_rectPlot.Height()+10);	// ����B
		swprintf_s(str, _T("%s"), _T("B��"));
		pDC->SetTextColor(RGB(0, 0, 0));
		pDC->TextOut(m_OldMoveX2, m_rectPlot.top+20, str);

		m_OldMoveX2 = point.x;
		pDC->MoveTo(point.x, m_rectPlot.top);
		pDC->LineTo(point.x, m_rectPlot.Height()+10);
		swprintf_s(str, _T("%s"), _T("B��"));
		pDC->SetTextColor(RGB(255, 0, 0));
		pDC->TextOut(m_OldMoveX2, m_rectPlot.top+20, str);
		pDC->SelectObject(pOldPen); // �ָ��ɵĻ���
		break;
	case 3: // ѡ�������3
		pOldPen = pDC->SelectObject(&m_PenLineV);
		pDC->MoveTo(m_rectPlot.left,  m_OldVoltageY); // ������һ�λ�����V
		pDC->LineTo(m_rectPlot.right, m_OldVoltageY); // ����V
		m_OldVoltageY = point.y;
		pDC->MoveTo(m_rectPlot.left, point.y);
		pDC->LineTo(m_rectPlot.right, point.y);
		pDC->SelectObject(pOldPen); // �ָ��ɵĻ���
		break;

	default:
		break;
	}
	Invalidate();
	pDC->SetROP2(nMode);
}

BOOL CADHistScope::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	switch (m_nShowCursor)
	{
	case 1:
	case 2:
		::SetCursor(m_hCursorHSize); 
		break;

	case 3:
		::SetCursor(m_hCursorVSize);
		break;

	case 4:
		::SetCursor(m_hCursorAllSize);
		break;

	default:
		::SetCursor(m_hCursorArrow);
		break;
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CADHistScope::IsShowAllChannle()
{
	return m_bAllChannel;
}

BOOL CADHistScope::ShowChannel(INT iFirstChannel, INT iLastChannel, BOOL bUpata)
{
	int index=0;
	for ( index=0; index<iFirstChannel; index++)
	{
		m_bVisable[index] = FALSE;
	}
	
	for (; index<= iLastChannel; index++)
	{
		m_bVisable[index] = TRUE;
	}

	for (; index<MAX_CHANNEL_COUNT; index++)
	{
		m_bVisable[index] = FALSE;
	}

	this->UpdateVisableChannleArray();
	this->UpdataChannelRect();
	if (TRUE == bUpata)
	{
		this->Invalidate();
	}
	
	return TRUE;
}

VOID CADHistScope::ProssDataToPT(INT iChannel)
{
	if (NULL != m_pDataBuff[iChannel])
	{
		float fLsbOfPixel; 
		if (m_bAllChannel)
		{
			if (gl_bTileWave) // ��ͨ��ƽ����ʾ
			{		
				float fRang = m_fUpperLimit[iChannel] - m_fLowerLimit[iChannel];  // ���̷�Χ
				fLsbOfPixel = (float) m_fLsbCount / (m_rcChannel[iChannel].Height() * fRang / (m_fVisableUpper[iChannel] - 
					m_fVisableLower[iChannel]));
				
				// 0 ��ֵ��λ��
				long yBase =  long(m_rcChannel[iChannel].bottom + 
								   m_rcChannel[iChannel].Height() * (m_fVisableLower[iChannel] - 
								   m_fLowerLimit[iChannel])/(m_fVisableUpper[iChannel] - 
								   m_fVisableLower[iChannel]));
				
				for (INT Index=0; Index<(m_iDataLength[iChannel]); Index++) 
				{
					if (Index >= m_rectPlot.Width())
					{
						break;
					}
					
					m_pPtCHannel[iChannel][Index].x = m_rectPlot.left + 1 + Index; // X�������ʼλ��;			
					m_pPtCHannel[iChannel][Index].y = long(yBase - ((m_pDataBuff[iChannel][Index])& m_wMaxLSB) / fLsbOfPixel); 			
				}	
				
			}
			else // ��ͨ��������ʾ
			{		
				float fRang = m_fUpperLimit[iChannel] - m_fLowerLimit[iChannel];  // ���̷�Χ
				fLsbOfPixel = (float)m_fLsbCount / (m_rectPlot.Height() * fRang / 
							  (m_fVisableUpper[iChannel] - m_fVisableLower[iChannel]));
				
				// 0 ��ֵ��λ��
				long yBase = long(m_rectPlot.bottom + 
								   m_rectPlot.Height() * (m_fVisableLower[iChannel] - 
								   m_fLowerLimit[iChannel])/ (m_fVisableUpper[iChannel] -
								   m_fVisableLower[iChannel]));
				
				for (INT Index=0; Index<=m_nPlotWidth; Index++)	
				{
					if (Index >= m_rectPlot.Width())
					{
						break;
					}

					m_pPtCHannel[iChannel][Index].x = m_rectPlot.left + 1 + Index; // X�������ʼλ��;	
					m_pPtCHannel[iChannel][Index].y = long(yBase - ((m_pDataBuff[iChannel][Index])& m_wMaxLSB) / fLsbOfPixel); 				
				}
			}	
		}
		else
		{
			float fRang = m_fUpperLimit[iChannel] - m_fLowerLimit[iChannel];  // ���̷�Χ
			fLsbOfPixel = (float)m_fLsbCount / (m_rectPlot.Height() * fRang / (m_fVisableUpper[iChannel] - 
				m_fVisableLower[iChannel]));
			
			// 0 ��ֵ��λ��
			long yBase = long(m_rectPlot.bottom + 
							   m_rectPlot.Height() * (m_fVisableLower[iChannel] - 
							   m_fLowerLimit[iChannel]) / (m_fVisableUpper[iChannel] - 
							   m_fVisableLower[iChannel]));
			
			for (INT Index=0; Index<=m_nPlotWidth; Index++)	
			{
				if (Index >= m_rectPlot.Width())
				{
					break;
				}
				
				m_pPtCHannel[iChannel][Index].x = m_rectPlot.left + 1 + Index; // X�������ʼλ��;	
				m_pPtCHannel[iChannel][Index].y = long(yBase - ((m_pDataBuff[iChannel][Index])& m_wMaxLSB) / fLsbOfPixel); 				
			}
		}
	}
}

void CADHistScope::SetTileMode()
{
	gl_bTileWave = TRUE;
	for (INT iChannel=0; iChannel<m_nChannelCount; iChannel++)	
	{
		this->ProssDataToPT(iChannel);  // ��������
	}

	this->Invalidate();
}

BOOL CADHistScope::IsTileMode()
{
	return gl_bTileWave;
}

void CADHistScope::SetSuperMode()
{
	gl_bTileWave = FALSE;
	for (INT iChannel=0; iChannel<m_nChannelCount; iChannel++)
	{
		this->ProssDataToPT(iChannel);
	}

	this->Invalidate();
}

void CADHistScope::UpdateVisableChannleArray()  // ��ʾͨ������
{
	m_arrayVisableChannel.RemoveAll();
	for (int index=0; index<m_nChannelCount; index++)
	{
		if (TRUE == m_bVisable[index])
		{
			m_arrayVisableChannel.Add(index);
		}
	}
	m_nChannelCount = (INT)m_arrayVisableChannel.GetSize();
}

void CADHistScope::UpdataChannelRect()
{
// 	CPaintDC dc(this);
// 	CADHistWaveView* pView = (CADHistWaveView*)GetParent();
// 	CADHistDoc* pDoc = (CADHistDoc*)pView->GetDocument();
// 	m_nChannelCount = pDoc->m_ChannelCount;

	// ������ͨ���ľ�������
	CRect rcChanle(m_rectPlot);
	rcChanle.top = m_rectPlot.top;
	rcChanle.bottom = m_rectPlot.top + m_rectPlot.Height() / m_nChannelCount;
	
	int iChannel;
	
	for (INT index=0; index<m_nChannelCount; index++)
	{
		rcChanle.top = m_rectPlot.top + index * m_rectPlot.Height() / m_nChannelCount;
		rcChanle.bottom = m_rectPlot.top + (index+1) * m_rectPlot.Height() / m_nChannelCount;
		
 		iChannel = m_arrayVisableChannel[index];
		m_rcChannel[iChannel] = rcChanle;
		}
}
