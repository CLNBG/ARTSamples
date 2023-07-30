// OScopeCtrl.h : header file
//

#ifndef __ADHistScope_H__
#define __ADHistScope_H__
/////////////////////////////////////////////////////////////////////////////

class CADHistScope : public CWnd
{
	// Construction
public:
	CADHistScope();
	
	// Attributes
public:
	double AppendPoint(double dNewPoint);
	void SetRange(double dLower, double dUpper, int nChannel);
	void SetXUnits(CString string);
	void SetYUnits(CString string);
	void SetGridColor(COLORREF color);
	void SetPlotColor(COLORREF color);
	void SetBackgroundColor(COLORREF color);
	void InvalidateCtrl();
	
	void Reset();
	
	// Operations
public:
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADHistScope)
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID=NULL);
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	void DrawSingleCHText(CDC* pDC, int nChannelNum);
	void DrawAllChannelText(CDC* pDC);
	void DrawSingleCHGrid(CDC* pDC);
	void DrawAllChannelGrid(CDC* pDC);
	int FindTrigger(int nChannel, int nValue);
	void SetStatusBar();
	void AppendPoly(int  Offset);
	int		m_nChannelCount;		// 通道数
	int		m_nChannelNum;			// 通道号
	int		PerY;					// 每通道的Y宽度
	int		m_Offset;				// 段内偏移
	int		m_Center;               // 没通道中间位置坐标
	POINT	 pointxy[32][16384];	// 所有通道显示的点数据
	CPoint	 pointTemp[16384];		// 用来存放单通道显示的点数据
	COLORREF m_clPen[MAX_AD_CHANNELS]; // 每通道话笔颜色
	int		 m_MouseOffset;			// 鼠标在绘图区的偏移位置
	int	     m_OldMoveX1;			// 各条能移动的线的位置
	int		 m_OldMoveX2;
	int      m_CurPosition;
	int		 m_CurMoveX1;
	int		 m_CurMoveX2;
	double m_ScreenPosition; 
//////////////////
	int m_RBtnDownPosX;          
	int m_StartOldMoveX1;
	int m_StartOldMoveX2;
/////////////////


    COLORREF m_crLineAColor;   // A线的颜色
	COLORREF m_crLineBColor;   // B线的颜色
	COLORREF m_crLineVColor;   // C线的颜色 
	
	CPen     m_PenLineA;	   // A线的画笔
	CPen     m_PenLineB;	   // B线的画笔
	CPen     m_PenLineV;       // V线的画笔
	HCURSOR  m_hCursorHSize;   // 水平光标
	HCURSOR  m_hCursorVSize;   // 垂直光标
	HCURSOR  m_hCursorArrow;   // 标准光标
	HCURSOR  m_hCursorAllSize; // 调整窗口大小光标
	int      m_nShowCursor;    // 显示光标
	int		 m_OldVoltageY;    // V线Y坐标
	float m_VolOffset;         // 文件偏移
	int m_nLineIndex;          // A/B/V线的ID
	BOOL m_bAllChannel;        // 所有通道
	int		m_nPlotWidth;
	         
	int m_nYDecimals;          // 小数点位数
	
	CString m_strXUnitsString;
	CString m_strYUnitsString;
	
	COLORREF m_crBackColor;        
	COLORREF m_crGridColor;        
	COLORREF m_crPlotColor;        
	
	double m_dCurrentPosition;   
	double m_dPreviousPosition;  
	
	virtual ~CADHistScope();
private:
	UINT m_nLoop;
	void DrawBkGnd(); // 画网格线
	void DrawPoint();
	void DrawPoly();

public:
	void ProcessData();

protected:
	void DrawMoveLine(CDC *pDC, CPoint point);
	BOOL m_bRBtnDown;
	CPen m_penChannel[MAX_CHANNEL_COUNT];
	BOOL m_bOnlyLine;
	BOOL m_bLBtnDown;
	//{{AFX_MSG(CADHistScope)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy); 
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
		int m_nHalfShiftPixels;
	int		m_nPlotShiftPixels;
	int		m_nClientHeight;
	int		m_nClientWidth;
	int		m_nPlotHeight; 
	BOOL	m_bDrawPoly;
	BOOL	m_bInitialed;
	double  m_dLowerLimit[MAX_CHANNEL_COUNT];        // lower bounds
	double  m_dUpperLimit[MAX_CHANNEL_COUNT];        // upper bounds
	double  m_dRange;
	double  m_dVerticalFactor;
	
	CRect   m_rectClient;
	CRect   m_rectPlot;
	CPen    m_penPlot;
	CBrush  m_brushBack;
	
	CDC     m_dcGrid;
	CDC     m_dcPlot;
	CDC	    m_dcMove;
	CBitmap* m_pbitmapOldGrid;
	CBitmap* m_pbitmapOldPlot;
	CBitmap m_bitmapGrid;
	CBitmap m_bitmapPlot;
	
};

/////////////////////////////////////////////////////////////////////////////
#endif
