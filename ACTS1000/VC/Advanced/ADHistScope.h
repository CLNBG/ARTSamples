// ADHistScope.h : header file
//

#ifndef __ADHistScope_H__
#define __ADHistScope_H__
/////////////////////////////////////////////////////////////////////////////
// CADHistScope window

#define  COLR_COUNT 4

class CADHistScope : public CWnd
{
	// Construction
public:
	CADHistScope();
	virtual ~CADHistScope();
	
	// Attributes
public:
	BOOL IsShowAllChannle();
	
	BOOL ShowChannel(INT iFirstChannel, INT iLastChannel, BOOL bUpata = TRUE);
	
	void AppendPoly(PWORD pBuff, const INT iChannel, LONGLONG iDataSize, LONGLONG  iOffset = 0, BOOL bUpdata = TRUE);

	void SetRange(float dLower, float dUpper, INT nChannel);         // 设置数据量程
	void SetVisableRange(float dLower, float dUpper, INT nChannel);  // 设置显示范围

	void SetXUnits(CString string);
	void SetYUnits(CString string);
	void SetGridColor(COLORREF color);
	void SetPlotColor(COLORREF color);
	void SetBackgroundColor(COLORREF color);	
	void Reset();	

	// Operations
public:
	void SetTileMode();
	BOOL IsTileMode();
	void SetSuperMode();
	
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADHistScope)
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID=NULL);
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	void DrawSingleCHText(CDC *pDC, INT nChannelNum);
	void DrawAllChannelText(CDC *pDC);
	void DrawSingleCHGrid(CDC *pDC);
	void DrawAllChannelGrid(CDC *pDC);
	void SetStatusBar();

protected:
	INT		m_nChannelCount;
	INT		m_iActivexChannel;   // 放大显示时的通道索引	
	WORD m_wMaxLSB;
	float m_fLsbCount;
	WORD m_lLsbHalf;
	ULONG	m_ChCfg[MAX_CHANNEL_COUNT];

	// 通道信息
	PPOINT	 m_pPtCHannel[MAX_CHANNEL_COUNT]; // 所有通道显示的点数据	
	LONGLONG		m_iOffset[MAX_CHANNEL_COUNT];	
	INT		m_iDataLength[MAX_CHANNEL_COUNT];	
	PWORD	m_pDataBuff[MAX_CHANNEL_COUNT];
	CRect   m_rcChannel[MAX_CHANNEL_COUNT];
	BOOL    m_bVisable[MAX_CHANNEL_COUNT];

	float  m_fLowerLimit[MAX_CHANNEL_COUNT];        // lower bounds
	float  m_fUpperLimit[MAX_CHANNEL_COUNT];        // upper bounds

	float  m_fVisableLower[MAX_CHANNEL_COUNT];        // 显示范围上限
	float  m_fVisableUpper[MAX_CHANNEL_COUNT];        // 显示范围下限

	CArray<INT, INT&>m_arrayVisableChannel;    // 可见通道序列
	void UpdateVisableChannleArray();  // 显示通道序列
	void UpdataChannelRect();

	INT      m_nRBtnMoveX;
	INT		 m_MouseOffset;		 // 鼠标在绘图区的偏移位置
	INT	     m_OldMoveX1;		 // 各条能移动的线的位置
	INT		 m_OldMoveX2;
	LONGLONG      m_CurPosition;
	INT		 m_CurMoveX1;
	INT		 m_CurMoveX2;

	INT m_RBtnDownPosX;
	INT m_StartOldMoveX1;
	INT m_StartOldMoveX2;

    COLORREF m_crLineAColor;  // A线的颜色
	COLORREF m_crLineBColor;  // B线的颜色
	COLORREF m_crLineVColor;  // C线的颜色 
	
	CPen     m_PenLineA;	  // A线的画笔
	CPen     m_PenLineB;	  // B线的画笔
	CPen     m_PenLineV;      // V线的画笔

	int      m_nPositionA;
	int      m_nPositionB;

	HCURSOR  m_hCursorHSize;
	HCURSOR  m_hCursorVSize;
	HCURSOR  m_hCursorArrow;
	HCURSOR  m_hCursorAllSize;
	INT      m_nShowCursor;
	INT m_CurVoltageY;
	INT m_OldVoltageY;

	INT m_nLineIndex;
	BOOL m_bAllChannel;
	
	INT m_nShiftPixels;           
	INT m_nYDecimals;
	
	CString m_strXUnitsString;
	CString m_strYUnitsString;
	
	COLORREF m_crBackColor;        
	COLORREF m_crGridColor;        
	COLORREF m_crPlotColor;        
	
	double m_dCurrentPosition;

public:
		double m_ScreenPosition; 

private:
	VOID ProssDataToPT(INT iChannel);
	void DrawBkGnd(); // 画网格线
	void DrawPoly();

protected:
	CPen m_penChannel[MAX_CHANNEL_COUNT];
	COLORREF m_clPen[MAX_CHANNEL_COUNT]; 

	void DrawMoveLine(CDC *pDC, CPoint poINT);
	BOOL m_bRBtnDown;

	BOOL m_bOnlyLine;
	BOOL m_bLBtnDown;

	//{{AFX_MSG(CADHistScope)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, INT cx, INT cy); 
	afx_msg INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint poINT);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint poINT);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint poINT);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint poINT);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint poINT);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
		
	INT		m_nHalfShiftPixels;
	INT		m_nPlotShiftPixels;

	INT		m_nPlotHeight;
	INT		m_nPlotWidth;  // 	
	CRect   m_rectClient;
	CRect   m_rectPlot;	
	CBrush  m_brushBack;	

protected:
	// 内存绘制相关的
	CDC     m_dcMem;
	CBitmap m_bmpMem;
	CBitmap* m_pOldBmp;	
};

/////////////////////////////////////////////////////////////////////////////
#endif
