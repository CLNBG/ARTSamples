//CADScopeCtrl.cpp : implementation file //

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////
//CADScopeCtrl
CADScopeCtrl::CADScopeCtrl()
{
	m_nChannelNum = 0;
	m_dPreviousPosition =   0.0;	
	m_nYDecimals = 3;
	for (int Channel=0; Channel<MAX_AD_CHANNELS; Channel++)
	{
		m_dLowerLimit[Channel] = -1000.0;
		m_dUpperLimit[Channel] =  1000.0;
	}
	
	m_dRange      =  20000.0;    
	m_nShiftPixels     = 4;
	m_nHalfShiftPixels = m_nShiftPixels/2;                      
	m_nPlotShiftPixels = m_nShiftPixels + m_nHalfShiftPixels;   
	
	m_crBackColor  = RGB(0,   0,   0);  
	m_crGridColor  = RGB(0, 255, 255);  
	m_crPlotColor  = RGB(255, 128, 0); 
	m_clPen[0] = RGB(255, 0, 0);
	m_clPen[1] = RGB(255, 255, 0);
	m_clPen[2] = RGB(0, 255, 0);
	m_clPen[3] = RGB(255, 0, 255);
	m_clPen[4] = RGB(255, 0, 0);
	m_clPen[5] = RGB(255, 255, 0);
	m_clPen[6] = RGB(0, 255, 0);
	m_clPen[7] = RGB(255, 0, 255);
	m_clPen[8] = RGB(255, 0, 0);
	m_clPen[9] = RGB(255, 255, 0);
	m_clPen[10] = RGB(0, 255, 0);
	m_clPen[11] = RGB(255, 0, 255);
	m_clPen[12] = RGB(255, 0, 0);
	m_clPen[13] = RGB(255, 255, 0);
	m_clPen[14] = RGB(0, 255, 0);
	m_clPen[15] = RGB(255, 0, 255);
	
	m_crLineAColor = RGB(255, 192, 255);  // A�ߵ���ɫ
    m_crLineBColor = RGB(192, 255, 255);  // B�ߵ���ɫ
    m_crLineVColor = RGB(255, 255, 128);  // C�ߵ���ɫ
	
	m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor);
	m_brushBack.CreateSolidBrush(m_crBackColor);
	
	m_strXUnitsString.Format(_T("Samples"));  
	m_strYUnitsString.Format(_T("Y units"));  
	
	m_pbitmapOldGrid = NULL;
	m_pbitmapOldPlot = NULL;
 	m_nChannelCount = 2;
	m_bDrawPoly = FALSE; 
	m_bAllChannel = TRUE;
	m_bInitialed = FALSE;
	m_bLBtnDown  = FALSE;
	m_bOnlyLine  = FALSE;
	m_nPtCount = 0; 
	m_OldMoveX1 = m_CurMoveX1 = 0;
	m_OldMoveX2 = m_CurMoveX2 = 0;
	m_CurMoveY = 0;
	m_CurVoltageY = 0;
	m_OldVoltageY = 0;
	m_nLineIndex = 0xFFFF;
	m_bRBtnDown = FALSE;
	m_Offset = 0;
	m_hCursorHSize = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
	m_hCursorVSize = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_SIZENS);
	m_hCursorArrow = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hCursorAllSize = (HCURSOR)AfxGetApp()->LoadStandardCursor(IDC_SIZEALL);
	m_nShowCursor = 0; // ��ʾ���
} // CADScopeCtrl

///////////////////////////////////////////////////////////////////////////

CADScopeCtrl::~CADScopeCtrl()
{
	if (m_pbitmapOldGrid != NULL)
		m_dcGrid.SelectObject(m_pbitmapOldGrid);  
	if (m_pbitmapOldPlot != NULL)
		m_dcPlot.SelectObject(m_pbitmapOldPlot);	
}

BEGIN_MESSAGE_MAP(CADScopeCtrl, CWnd)
//{{AFX_MSG_MAP(CADScopeCtrl)
ON_WM_PAINT()
ON_WM_SIZE()
ON_WM_CREATE()
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDBLCLK()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
ON_WM_RBUTTONDOWN()
ON_WM_RBUTTONUP()
ON_WM_SETCURSOR()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////
//ADScopeCtrl message handlers
///////////////////////////////////////////////////////////////////////////
BOOL CADScopeCtrl::Create(DWORD dwStyle, const RECT& rect, 
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

///////////////////////////////////////////////////////////////////////////
// ���ô�ֱ�����������̵Ĵ�С
void CADScopeCtrl::SetRange(double dLower, double dUpper, int nChannel)
{
	ASSERT(dUpper > dLower);
	
	m_dLowerLimit[nChannel]  = dLower;
	m_dUpperLimit[nChannel]  = dUpper;
	m_nYDecimals      = 2;    //  С����λ��
	m_dRange          = dUpper - dLower;
	m_dVerticalFactor = (double)m_nPlotHeight / m_dRange; 
	
	InvalidateCtrl();
	
}  // SetRange

///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetXUnits(CString string)
{
	m_strXUnitsString = string;
	
	InvalidateCtrl();
	
}  // SetXUnits

///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetYUnits(CString string)
{
	m_strYUnitsString = string;
	
	InvalidateCtrl();
	
}  // SetYUnits

///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetGridColor(COLORREF color)
{
	m_crGridColor = color;
	
	InvalidateCtrl();
	
}  // SetGridColor


///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetPlotColor(COLORREF color)
{
	m_crPlotColor = color;
	
	m_penPlot.DeleteObject();
	m_penPlot.CreatePen(PS_SOLID, 0, m_crPlotColor);
	
	InvalidateCtrl();
	
}  // SetPlotColor


///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::SetBackgroundColor(COLORREF color)
{
	m_crBackColor = color;
	
	m_brushBack.DeleteObject();
	m_brushBack.CreateSolidBrush(m_crBackColor);
	
	InvalidateCtrl();
	
}  // SetBackgroundColor

///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::InvalidateCtrl()
{
	CClientDC dc(this);
	if (m_dcGrid.GetSafeHdc() == NULL)
	{
		m_dcGrid.CreateCompatibleDC(&dc);	
		m_bitmapGrid.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight);
		m_pbitmapOldGrid = m_dcGrid.SelectObject(&m_bitmapGrid);
	}

	if (m_dcPlot.GetSafeHdc() == NULL)
	{
		m_dcPlot.CreateCompatibleDC(&dc);
		m_bitmapPlot.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight);
		m_pbitmapOldPlot = m_dcPlot.SelectObject(&m_bitmapPlot);
	}

	if (m_PenLineA.m_hObject == NULL)
		m_PenLineA.CreatePen(PS_DASHDOT, 1, m_crLineAColor); // ����A�ߵĻ���
	if (m_PenLineB.m_hObject == NULL)
		m_PenLineB.CreatePen(PS_DASHDOT, 1, m_crLineBColor); // ����B�ߵĻ���
	if (m_PenLineV.m_hObject == NULL)
		m_PenLineV.CreatePen(PS_DASHDOT, 1, m_crLineVColor); // ����V�ߵĻ���

	int Channel = 0;
	for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
	{
		if (m_penChannel[Channel].m_hObject == NULL)
			m_penChannel[Channel].CreatePen(PS_SOLID, 1, m_clPen[Channel]);
	}
	
// 	for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
// 	{
// 		m_dLowerLimit[Channel] = -gl_ScreenVolume/2.0;
// 		m_dUpperLimit[Channel] = gl_ScreenVolume/2.0;
// 	}

	if (gl_bTileWave)  // �����ƽ����ʾ
	{
		int Tchannel = 0;
		for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
		{
			if (::ADPara.bChannelArray[Channel] == TRUE)
			{
				Tchannel++;
			}
				
		}
		gl_nChannelCount = Tchannel;
		m_nChannelCount = gl_nChannelCount;
	}
		
	DrawBkGnd();  // ������
	DrawPoly();   
	m_bInitialed = TRUE;
	
} // InvalidateCtrl

///////////////////////////////////////////////////////////////////////////
double CADScopeCtrl::AppendPoint(double dNewPoint)
{
	
	double dPrevious;
	
	dPrevious = m_dCurrentPosition;
	m_dCurrentPosition = dNewPoint;
	DrawPoint();
	
	Invalidate();
	
	return dPrevious;
	
} // AppendPoint

//////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::OnPaint() 
{
	// �����m_nClientWidth, m_ClientHeightҲ��ȡ����m_rectClient�Ĵ�С
	CPaintDC dc(this);  // device context for painting
	CDC memDC;
	CBitmap memBitmap;
	CBitmap* oldBitmap; // bitmap originally found in CMemDC
	memDC.CreateCompatibleDC(&dc);
	memBitmap.CreateCompatibleBitmap(&dc, m_nClientWidth, m_nClientHeight);
	oldBitmap = (CBitmap *)memDC.SelectObject(&memBitmap);
	CString str;
	
	if (memDC.GetSafeHdc() != NULL)
	{
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
			&m_dcGrid, 0, 0, SRCCOPY);
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
			&m_dcPlot, 0, 0, SRCPAINT);  //SRCPAINT
		memDC.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
			&m_dcMove, 0, 0, SRCPAINT);  //SRCPAINT		
		dc.BitBlt(0, 0, m_nClientWidth, m_nClientHeight, 
			&memDC, 0, 0, SRCCOPY);       //SRCCOPY);
		
	}
	this->UpdateChannelCount();

	memDC.SelectObject(oldBitmap);
	
	int	nDrawMode;
	dc.SetBkMode(TRANSPARENT);		                // ���ñ���Ϊ͸��
	nDrawMode = dc.SetROP2(R2_XORPEN);              // ���û���ģʽ
	
	dc.SelectObject(&m_PenLineA);                   // ѡ����A�Ļ���
	dc.MoveTo(m_OldMoveX1, m_rectPlot.top);
	dc.LineTo(m_OldMoveX1, m_rectPlot.Height()+10); // ����A	
	
	dc.SelectObject(&m_PenLineB);				    // ѡ������B�Ļ���
	dc.MoveTo(m_OldMoveX2, m_rectPlot.top);
	dc.LineTo(m_OldMoveX2, m_rectPlot.Height()+10); // ����B	
	if (!m_bAllChannel)
	{
		dc.SelectObject(&m_PenLineV);				// ѡ������V�Ļ���
		dc.MoveTo(m_rectPlot.left, m_OldVoltageY);
		dc.LineTo(m_rectPlot.right, m_OldVoltageY);	// ����V
	}
	dc.SetROP2(nDrawMode);


} // OnPaint

//#####################################################################
// �������ĵ�(λͼ���ƶ�������̬)
void CADScopeCtrl::DrawPoint()
{
	int currX, prevX, currY, prevY;
	
	CPen* oldPen;
	CRect rectCleanUp;
	
	if (m_dcPlot.GetSafeHdc() != NULL)
	{
		m_dcPlot.BitBlt(m_rectPlot.left, m_rectPlot.top+1, 
			m_nPlotWidth, m_nPlotHeight, &m_dcPlot, 
			m_rectPlot.left+1/*2/*m_nShiftPixels*/, m_rectPlot.top+1, 
			SRCCOPY);		
		
		rectCleanUp = m_rectPlot;
		rectCleanUp.left  = rectCleanUp.right - 1;
		
		m_dcPlot.FillRect(rectCleanUp, &m_brushBack);
		
		oldPen = m_dcPlot.SelectObject(&m_penPlot);
		
		prevX = m_rectPlot.right-2; // m_nPlotShiftPixels;
		prevY = m_rectPlot.bottom - (long)m_dPreviousPosition-10;
		m_dcPlot.MoveTo (prevX, prevY);
		
		currX = m_rectPlot.right-1; // m_nHalfShiftPixels;
		currY = m_rectPlot.bottom -(long)m_dCurrentPosition-10;
		m_dcPlot.LineTo (currX, currY);
		
		m_dcPlot.SelectObject(oldPen);
		
		m_dPreviousPosition = m_dCurrentPosition;		
	}
	
} // end DrawPoint

///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (m_bInitialed) 
	{ 
		GetClientRect(m_rectClient); // ��ȡ��ǰ�Ŀͻ�����С
		
		m_nClientHeight = m_rectClient.Height(); 
		m_nClientWidth  = m_rectClient.Width();
		
		m_rectPlot.left   = 60;  
		m_rectPlot.top    = 10;
		m_rectPlot.right  = m_rectClient.right -10;
		m_rectPlot.bottom = m_rectClient.bottom - 10; //-25;
		
		m_nPlotHeight = m_rectPlot.Height();
		m_nPlotWidth  = m_rectPlot.Width();	
		
		m_dVerticalFactor = (double)m_nPlotHeight / m_dRange; 	
		InvalidateCtrl(); // ���»���
		
		m_OldVoltageY = m_CurVoltageY = m_rectPlot.top + 20;
		m_OldMoveX1   = m_CurMoveX1 = m_rectPlot.left + 20;
		m_OldMoveX2   = m_CurMoveX2 = m_rectPlot.right - 20;
		m_OldVoltageY = m_CurVoltageY = m_rectPlot.top + 20;		
	}
} // OnSize

///////////////////////////////////////////////////////////////////////////
void CADScopeCtrl::Reset()
{
	InvalidateCtrl();
}

///////////////////////////////////////////////////////////////////////////
int CADScopeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{ 
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_rectClient = CRect(0, 0, 1400, 1000);   
	m_nClientHeight = m_rectClient.Height();
	m_nClientWidth  = m_rectClient.Width();
	
	m_rectPlot.left   = 60;  
	m_rectPlot.top    = 10;
	m_rectPlot.right  = m_rectClient.right-10;
	m_rectPlot.bottom = m_rectClient.bottom-25;
	
	m_nPlotHeight = m_rectPlot.Height();
	m_nPlotWidth  = m_rectPlot.Width();
	
	m_dVerticalFactor = (double)m_nPlotHeight / m_dRange; 
	
	return 0;

}

//########################################################################
// ����������������
void CADScopeCtrl::DrawBkGnd()  
{ 
	int nCharacters = 0;
	CPen* oldPen;
	CPen solidPen(PS_SOLID, 0, m_crGridColor);
	CFont axisFont, yUnitFont;
	CFont* oldFont;
	CString strTemp;
	//---------------------------------------------------------------------------
	m_dcGrid.SetBkColor (m_crBackColor);	
	// fill the grid background
	m_dcGrid.FillRect(m_rectClient, &m_brushBack);	
	// ���������ִ���ռ�õ��ַ����
	nCharacters = abs((int)log10(fabs(m_dUpperLimit[0])));
	nCharacters = max(nCharacters, abs((int)log10(fabs(m_dLowerLimit[0]))));
	nCharacters = nCharacters + 4 + m_nYDecimals;  
	
	PerY = (int)(m_nPlotHeight / m_nChannelCount); // ÿͨ����Y���
	//----------------------------------------------------------------------------------
	// �����ܵĿ��	
	oldPen = m_dcGrid.SelectObject (&solidPen);  // �����ܵķ���
	m_dcGrid.MoveTo (m_rectPlot.left-1, m_rectPlot.top);
	m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.top);
	m_dcGrid.LineTo (m_rectPlot.right+1, m_rectPlot.bottom+1);
	m_dcGrid.LineTo (m_rectPlot.left-1, m_rectPlot.bottom+1);
	m_dcGrid.LineTo (m_rectPlot.left-1, m_rectPlot.top);
	m_dcGrid.SelectObject (oldPen);  
	COLORREF m_Grid = RGB(200, 200, 200);
	int HLine = 0, VLine = 0;
	for (VLine=50; VLine<m_rectPlot.Width(); VLine+= 50) // ���50�����ػ�һ����ֱ����
	{
		for (int HLine=5; HLine<m_rectPlot.Height(); HLine+=5) // ����ֱ��������
		{
			m_dcGrid.SetPixelV(CPoint(m_rectPlot.left + VLine, m_rectPlot.top + HLine), m_Grid);
		}
	}
	
	for (HLine=30; HLine<m_rectPlot.Height(); HLine+=30) // ���30�����ػ�һ��ˮƽ�������
	{
		for (VLine=0; VLine<m_rectPlot.Width(); VLine+=10) // ��ˮƽ��������
		{
			m_dcGrid.SetPixelV(CPoint(m_rectPlot.left + VLine, m_rectPlot.top + HLine), m_Grid);
		}
	}	
	//----------------------------------------------------------------------------
	// ��ÿ��ͨ���ķֽ���(ˮƽ��)
	
	if (m_bAllChannel) // ��ʾ����ͨ��
	{
		if (gl_bTileWave) // �����ƽ����ʾ
		{
			DrawAllChannelGrid(&m_dcGrid); // ������ͨ���ĺ�������
		}
		else
		{
			DrawSingleCHGrid(&m_dcGrid);
		}
	}
	else // ����ͨ���ĺ���������
	{
		DrawSingleCHGrid(&m_dcGrid);
	}

	//--------------------------------------------------------------------------------
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
	
	oldFont = m_dcGrid.SelectObject(&axisFont);
	m_dcGrid.SetTextAlign (TA_RIGHT|TA_TOP); // ���뷽ʽ	
	
	//----------------------------------------------------------------------
	
	if (m_bAllChannel)
	{
		if (gl_bTileWave) // ƽ����ʾ
		{
			DrawAllChannelText(&m_dcGrid);
		}
		else // ������ʾ
		{
			DrawSingleCHText(&m_dcGrid, 0);
		}
		
	}
	else
	{
		DrawSingleCHText(&m_dcGrid, m_nChannelNum);
	}

	InvalidateRect(m_rectClient); // ˢ������
	
}

//########################################################################
// ��PolyLine��
void CADScopeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
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

	m_MouseOffset = point.x;	
	SetStatusBar(); // ����״̬����Ƶ�ʵ�
	
	CWnd::OnMouseMove(nFlags, point);
}

void CADScopeCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString str;
	m_nChannel = 0;
	if (gl_bTileWave && m_rectPlot.PtInRect(point)) // �������ڱ߿���ʱ
	{
		// ͨ���ж����˫��ʱ��λ�ã���������ʾ��һ·��ͨ��		
		if (m_bAllChannel) 
		{
			for (int Channel=0; Channel<MAX_AD_CHANNELS; Channel++) //�ж����˫��λ��
			{
				if (ADPara.bChannelArray[Channel])
				{
					if (abs(point.y - HeightMid[m_nChannel]) < m_rectPlot.Height()/(2*m_nChannelCount))
					{	
						m_nChannelNum = Channel; // ȷ��˫����ĵ�ͨ����
						str.Format(_T("%d") , Channel);
						SetYUnits("��ͨ��-"+str);
						break;
					}
					m_nChannel++;
				}
				
				
			}
// 			m_dLowerLimit[m_nChannelNum] = -gl_ScreenVolume/2.0;
// 			m_dUpperLimit[m_nChannelNum] = gl_ScreenVolume/2.0;			
		}
		
		m_bAllChannel = !m_bAllChannel; 
		InvalidateCtrl();
	}
	
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CADScopeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
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

//#####################################################################################
// ��ԭ��ֵ����(buffer������ԭ��), �ٴ���ɿ�����ʾ�ĵ�����
void CADScopeCtrl::AppendPoly(int  Offset)
{
	m_Offset = Offset;     // ����ƫ��
	ProcessData(); // ��������
	DrawPoly(); // ����
}

//###############################################################################
// ��������, �ѻ����е�����ת���ɿ�����ʾ�ĵ�����
void CADScopeCtrl::ProcessData()  
{	
	UpdateChannelCount();
	float m_ScreenVolume = AD_VOLT_RANGE; // ��ͨ����ʾʱ����Ļ�������Ǻ�����ѹֵһ�µ�
	float LsbOfPixel;
	
//	LsbOfPixel = (float)(((gl_ScreenVolume/AD_VOLT_RANGE)*AD_LSB_COUNT)/(PerY)); // ÿ���ض�Ӧ����ֵ 

	LsbOfPixel = (float)(AD_LSB_COUNT/PerY); // ÿ���ض�Ӧ����ֵ

	Center = (int)(PerY/2.0)+m_rectPlot.top;
	int Channel = 0, TChannel = 0, Index = 0, StartX = 0;
	PLONG  ptOffset; // ����ָ��
	int Offset = 0, DataY = 0;

	StartX = m_rectPlot.left+1; // X�������ʼλ��
	
	if (gl_bTileWave) // ��ͨ��ƽ����ʾ
	{
		for (Channel=0; Channel<MAX_AD_CHANNELS; Channel++)
		{
			if (ADPara.bChannelArray[Channel] == TRUE)
			{				
				for (Index=0; Index<=m_nPlotWidth; Index++) // ��ʼ��1024����(����ʱ��λͼ�Ĵ�С) 
				{
					ptOffset = &ADBuffer[gl_nDrawIndex][(m_Offset +Index) * m_nChannelCount]; // ָ���ƫ����
					pointxy[TChannel][Index].x = StartX + Index;
					
					if (!gl_bProcessMoveVolt)
					{
						DataY = (int)(((((ptOffset[TChannel])&0xFFFF) - gl_MiddleLsb[Channel]) / LsbOfPixel)); // ��ԭʼ�����ݼ�ȥƽ�Ƶ�ѹ��ֵ�Ͱ��ƫ����ֵ
					}
					else
					{
						DataY = (int)(((((ptOffset[TChannel])&MASK_MSB) - AD_LSB_HALF) / LsbOfPixel)); 
					}
					pointxy[TChannel][Index].y = (int)(Center) - DataY;
				}
				HeightMid[TChannel] = Center; // ����ͨ���м�λ������
				Center += PerY;
				TChannel++;
			}
		} 
	}
	else // ��ͨ��������ʾ
	{
		for (Channel=0; Channel<MAX_AD_CHANNELS; Channel++)
		{
			if (ADPara.bChannelArray[Channel] == TRUE)
			{	
				LsbOfPixel = (float)(AD_LSB_COUNT/m_rectPlot.Height()); // ÿ���ض�Ӧ����ֵ
			//	LsbOfPixel = (float)(((gl_ScreenVolume/AD_VOLT_RANGE)*AD_LSB_COUNT)/(m_rectPlot.Height())); // ÿ���ض�Ӧ����ֵ    
				int StartX = m_rectPlot.left;
				Center = (int)(m_rectPlot.Height() / 2) + m_rectPlot.top;
				for (int Index=0; Index<=m_nPlotWidth; Index++)	
				{
					ptOffset = &ADBuffer[gl_nDrawIndex][(m_Offset +Index) * m_nChannelCount]; // ָ���ƫ����
					pointxy[TChannel][Index].x = StartX + Index;	
					DataY = (int)(((((ptOffset[TChannel])&MASK_MSB) - AD_LSB_HALF) / LsbOfPixel));
					pointxy[TChannel][Index].y = (int)(Center) - DataY;
				}
				TChannel++;
			}
		}
	}
	
	m_bDrawPoly = TRUE; 
	
}

//##############################################################################
// ��������ĵ�
void CADScopeCtrl::DrawPoly()
{	
	int DataY = 0;
	gl_bDataProcessing = TRUE;
	CPen* oldPen;
	m_dcPlot.SetBkColor (m_crBackColor);
	m_dcPlot.FillRect(m_rectClient, &m_brushBack);
	m_dcPlot.SetTextColor(RGB(255, 158, 0));	
	//---------------------------------------------------------------------------------


	int nDrawCount=MAX_SEGMENT_SIZE/2/m_nChannelCount - 1-m_Offset;
	if (nDrawCount > m_rectPlot.Width())
	{
		nDrawCount = m_rectPlot.Width() -m_Offset ;
	}


// 	UINT nDrawCount=0;
// 	if (m_nChannelCount < 5)
// 	{
// 		nDrawCount = m_rectPlot.Width() - 1;
// 	}
// 	else
// 	{
// 		nDrawCount = MAX_SEGMENT_SIZE/2/m_nChannelCount - 1;
// 	}



	if (m_bAllChannel || !gl_bTileWave) // ����ͨ����ʾ�������ʾʱ
	{
		for (int Channel = 0; Channel<m_nChannelCount; Channel++) // ������ͨ���ĵ�
		{
			oldPen = m_dcPlot.SelectObject(&m_penChannel[Channel]);
			//m_dcPlot.Polyline(pointxy[Channel], m_rectPlot.Width());
			m_dcPlot.Polyline(pointxy[Channel], nDrawCount);
		}
	}
	else // ��ͨ����ʾ
	{	
		LONG* ptOffset;
		float LsbOfPixel;

		m_dcPlot.SelectObject(&m_penChannel[m_nChannelNum]);
		LsbOfPixel = (float)(((gl_ScreenVolume/(gl_InputRange[m_nChannelNum] * 1.0))*AD_LSB_COUNT)/(m_rectPlot.Height())); // ÿ���ض�Ӧ����ֵ    
		int StartX = m_rectPlot.left;
		Center = (int)(m_nPlotHeight / 2) + m_rectPlot.top;
	//	ptOffset = &ADBuffer[gl_nDrawIndex][m_Offset]; // ָ���ƫ����
	
		ptOffset = &ADBuffer[gl_nDrawIndex][(m_Offset)  * m_nChannelCount]; 
		for (int Index=0; Index<=nDrawCount/*m_nPlotWidth*/; Index++)	
		{			
			pointTemp[Index].x = StartX + Index;
			
			if (Index * m_nChannelCount + m_nChannel >=4096)
			{
				return;
			}

			if (!gl_bProcessMoveVolt)
			{
				DataY = (int)(((((ptOffset[Index * m_nChannelCount + m_nChannel])&MASK_MSB) - gl_MiddleLsb[m_nChannelNum]) / LsbOfPixel));
			}
			else
			{
				DataY = (int)(((((ptOffset[Index * m_nChannelCount + m_nChannel])&MASK_MSB) - AD_LSB_HALF) / LsbOfPixel));
			}

			pointTemp[Index].y = (int)(Center) - DataY;
		}
		oldPen = m_dcPlot.SelectObject(&m_penChannel[m_nChannelNum]);
		//m_dcPlot.Polyline(pointTemp, m_nPlotWidth);
		m_dcPlot.Polyline(pointTemp, nDrawCount);
	}

	gl_bDataProcessing = FALSE;
	Invalidate();
}

void CADScopeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_nLineIndex = 0xFFFF;
	m_bOnlyLine = FALSE;
	m_bLBtnDown = FALSE; 
	::ReleaseCapture(); 

	CWnd::OnLButtonUp(nFlags, point);
}

//#######################################################################
void CADScopeCtrl::SetStatusBar()
{
	float fTimePixel;
	float fFrequency;
	CString strTimeDiv;
	CString strCountTimer;
	CString strFrequency;
	CString strVol;
	int SubstValue = abs(m_OldMoveX1 - m_OldMoveX2);
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	CADFrm* pADFrm = (CADFrm*)pApp->m_ADFrm;
	CEdit* pEditDiv = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_STATIC_TimeOfDiv); 
	fTimePixel = (float)(1000000.0/ADPara.Frequency); // uS

	int Tchannel = 0;
	for (int Channel=0; Channel<MAX_AD_CHANNELS; Channel++)
	{
		if (ADPara.bChannelArray[Channel] == TRUE)
		{
			Tchannel++;
		}
		
	}
	gl_nChannelCount = Tchannel;
	m_nChannelCount = gl_nChannelCount;

	// ��������֮���ʱ���
	if (fTimePixel < 1000)
	{
		strCountTimer.Format(_T("%7.2f uS") , fTimePixel * SubstValue);
	}

	if (fTimePixel >= 1000.0 && fTimePixel < 1000000.0)
	{
		strCountTimer.Format(_T("%7.2f mS") , fTimePixel * SubstValue/1000.0);
	}

	CEdit* pEditTime = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_CountTime);
	pEditTime->SetWindowText(strCountTimer);

	// ����Ƶ�ʵĴ�С
    fFrequency = (float)(1000000.0 / (fTimePixel * SubstValue));
	if (fFrequency < 1000.0)
	{
		strFrequency.Format(_T("%7.2f Hz") , fFrequency);
	}

	if (fFrequency >= 1000.0 && fFrequency < 1000000.0)
	{
		strFrequency.Format(_T("%7.2f KHz") , fFrequency/1000.0);
	}

	if (fFrequency > 1000000.0)
	{
		strFrequency.Format(_T("%7.2f MHz") , fFrequency/1000000.0);
	}
	
	CEdit* pEditFre = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_MeasureFreq);
	pEditFre->SetWindowText(strFrequency);

	// ������ѹֵ��ѹֵ
	float LsbOfPixel = (float)((gl_ScreenVolume*1.0)/(m_rectPlot.Height())); // ÿ���ض�Ӧ����ֵ    
	
	int Center = m_rectPlot.Height()/2 + m_rectPlot.top;
	int nOffset = Center - m_OldVoltageY;
	m_VolOffset = nOffset * LsbOfPixel;
	strVol.Format(_T("%7.2f") , m_VolOffset);
	CEdit* pEditVol = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_Volt);
	pEditVol->SetWindowText(strVol);

	CEdit* pEditOffset = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_Offset);
	CString strOffset;
	strOffset.Format(_T("%d") , m_MouseOffset - m_rectPlot.left + gl_Offset);
	if (m_MouseOffset >= m_rectPlot.left && m_MouseOffset <= m_rectPlot.right)
	{
		pEditOffset->SetWindowText(strOffset);
	}	
}

int CADScopeCtrl::FindTrigger(int nChannel, int nValue) // ��ֹ���ζ���
{
	return 0;
}

// ������ͨ����������
void CADScopeCtrl::DrawAllChannelGrid(CDC* pDC)
{
	int X = 0, Channel = 0;
	int nGridPix = 0; // Y����������ߵĿ̶�
	float hight = (float)(m_rectPlot.Height() / m_nChannelCount); // ÿͨ����Y���
	
	for (Channel=1; Channel<m_nChannelCount; Channel++) // ��m_nChannelCount-1��������
	{
		nGridPix = m_rectPlot.top + (int)(m_rectPlot.Height() * Channel) / m_nChannelCount;
		for (int X=m_rectPlot.left; X<m_rectPlot.right; X+=2) // ÿ��2�����ػ�1��
		{
			pDC->SetPixel(X, nGridPix, m_crGridColor); 
		}	
	}

	for (Channel=0; Channel<m_nChannelCount; Channel++) // ��ÿͨ��������
	{
		for (X=m_rectPlot.left; X<m_rectPlot.right; X+=3) // ÿ��3�����ػ�1��
		{
			pDC->SetPixel(X, (int)(m_rectPlot.top + (hight * Channel) + hight/2.0), RGB(255, 255, 255)); 
		}
		
	}
}

void CADScopeCtrl::DrawSingleCHGrid(CDC* pDC)
{
	for (int X=m_rectPlot.left; X<m_rectPlot.right; X+=2) // ÿ��4�����ػ�1��
	{
		pDC->SetPixel(X, m_rectPlot.top+(int)(m_rectPlot.Height()/2), RGB(255, 255, 255));	
	}
}


void CADScopeCtrl::DrawAllChannelText(CDC* pDC)
{
	CString str;
	float hight = (float)(m_rectPlot.Height() / m_nChannelCount); // ÿͨ����Y���
	int Tchannel = 0;
	for (int Channel=0; Channel<MAX_AD_CHANNELS; Channel++)
	{		
		if (ADPara.bChannelArray[Channel])
		{
			pDC->SetTextColor(m_clPen[Channel]); // �������ֵ���ɫ
			str.Format (_T("%.*lf V") , m_nYDecimals, m_dUpperLimit[Channel]/1000.0); // ����ѹֵ
			pDC->TextOut (m_rectPlot.left-4, (int)(m_rectPlot.top+hight*Tchannel+8), str); 
			
			pDC->SetTextAlign (TA_RIGHT|TA_BASELINE);
			str.Format (_T("%.*lf V") , m_nYDecimals, m_dLowerLimit[Channel]/1000.0); // ����ѹֵ
			pDC->TextOut (m_rectPlot.left-4, (int)(m_rectPlot.top+hight*(Tchannel+1)-5), str);
			
			str.Format(_T("CH %d") , Channel);
			pDC->TextOut(m_rectPlot.left-4, (int)(m_rectPlot.top+hight*Tchannel+hight/2+5), str);
			Tchannel++;
		}
		     
	}	
}

void CADScopeCtrl::DrawSingleCHText(CDC* pDC, int nChannelNum)
{
	CString strInfo;
	if (nChannelNum != 0xFFFF)
		pDC->SetTextColor(m_clPen[nChannelNum]);
	else
		pDC->SetTextColor(m_crGridColor);

	double dUpperLimit[MAX_AD_CHANNELS];
	double dLowerLimit[MAX_AD_CHANNELS];
	dUpperLimit[nChannelNum] = gl_ScreenVolume/2.0;
	dLowerLimit[nChannelNum] = -gl_ScreenVolume/2.0;

//	strInfo.Format (_T("%.*lf mV", 0, m_dUpperLimit[nChannelNum]); // ����ѹֵ
	strInfo.Format (_T("%.*lf mV") , 0, dUpperLimit[nChannelNum] ); // ����ѹֵ
	pDC->TextOut (m_rectPlot.left-4, m_rectPlot.top, strInfo); 
	pDC->SetTextAlign (TA_RIGHT|TA_BASELINE);
//	strInfo.Format (_T("%.*lf mV") , 0, m_dLowerLimit[nChannelNum]); // ����ѹֵ
	strInfo.Format (_T("%.*lf mV") , 0, dLowerLimit[nChannelNum]); // ����ѹֵ
	
	pDC->TextOut (m_rectPlot.left-4, m_rectPlot.top+m_nPlotHeight, strInfo);
	
	if (nChannelNum == 0xFFFF) // ����ǵ�����ʾʱ
		strInfo = "CH";
	else
		strInfo.Format(_T("CH %d") , nChannelNum); 

	// дͨ����
	pDC->TextOut(m_rectPlot.left-4, m_rectPlot.top+m_nPlotHeight/2+5, strInfo);      
	
}

void CADScopeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HCURSOR hCursor;
	m_bRBtnDown = TRUE;
	int DistanceSub = abs(point.x - m_OldMoveX1) + abs(point.x - m_OldMoveX2);
	int DistanceAB = abs(m_OldMoveX1 - m_OldMoveX2);
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

void CADScopeCtrl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	HCURSOR hCursor;
	
	hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	::SetCursor(hCursor);
	
	m_bRBtnDown = FALSE;
	m_nLineIndex = 0xFFFF;
	ClipCursor(NULL);
	CWnd::OnRButtonUp(nFlags, point);
}

void CADScopeCtrl::DrawMoveLine(CDC* pDC, CPoint point)
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
		pDC->LineTo(m_OldMoveX1, m_rectPlot.Height()+10);	// ����A
		pDC->SelectObject(&m_PenLineB);
		pDC->MoveTo(m_OldMoveX2, m_rectPlot.top); // ������һ�λ�����B
		pDC->LineTo(m_OldMoveX2, m_rectPlot.Height()+10);	// ����B		
		m_OldMoveX1 = m_StartOldMoveX1 + (point.x - m_RBtnDownPosX); 
		m_OldMoveX2 = m_StartOldMoveX2 + (point.x - m_RBtnDownPosX); 		
		pDC->SelectObject(&m_PenLineA);
		pDC->MoveTo(m_OldMoveX1, m_rectPlot.top);
		pDC->LineTo(m_OldMoveX1, m_rectPlot.Height()+10);
		pDC->SelectObject(&m_PenLineB);
		pDC->MoveTo(m_OldMoveX2, m_rectPlot.top);
		pDC->LineTo(m_OldMoveX2, m_rectPlot.Height()+10);
		pDC->SelectObject(pOldPen); // �ָ��ɵĻ���
		break;

	case 1: // ѡ�������1
		pOldPen = pDC->SelectObject(&m_PenLineA);
		pDC->MoveTo(m_OldMoveX1, m_rectPlot.top); // ������һ�λ�����A
		pDC->LineTo(m_OldMoveX1, m_rectPlot.Height()+10); // ����A
		m_OldMoveX1 = point.x;
		pDC->MoveTo(point.x, m_rectPlot.top);
		pDC->LineTo(point.x, m_rectPlot.Height()+10);
		pDC->SelectObject(pOldPen); // �ָ��ɵĻ���
		break;

	case 2: // ѡ�������2
		pOldPen = pDC->SelectObject(&m_PenLineB);
		pDC->MoveTo(m_OldMoveX2, m_rectPlot.top); // ������һ�λ�����B
		pDC->LineTo(m_OldMoveX2, m_rectPlot.Height()+10);	// ����B
		m_OldMoveX2 = point.x;
		pDC->MoveTo(point.x, m_rectPlot.top);
		pDC->LineTo(point.x, m_rectPlot.Height()+10);
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
	pDC->SetROP2(nMode);
	
}

BOOL CADScopeCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	switch(m_nShowCursor)
	{
	case 1: // jump to 2
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

void CADScopeCtrl::UpdateChannelCount()
{
	m_nChannelCount = gl_nChannelCount;		

}

