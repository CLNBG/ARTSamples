#if !defined(AFX_HistDataDoc_H__D442B985_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_)
#define AFX_HistDataDoc_H__D442B985_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistDataDoc.h : header file
//
#include "Sys.h"
#include "ADHistFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CADHistDoc document

class CADHistDoc : public CDocument
{
protected:
	CADHistDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CADHistDoc)
	PWORD  m_HistDataBuffer[MAX_CHANNEL_COUNT];
	INT		m_HistScreenWith; 

public:
   	HWND m_hWndDigit;        // 数字窗口句柄
	HWND m_hWndWave;         // 波形窗口句柄

// Attributes
public:
	int m_nDigitWidth;
	CFile m_File;
	CString m_FileName;
	__int64 m_FileLength;
	__int64	m_SliderRatio;	// Slider压缩比
	FILE_HEADER m_Header;  // 保存文件头信息

// 	LONG m_VoltBottomRange[8];       // 量程下限(mV)
// 	LONG m_VoltTopRange[8];	        // 量程上限(mV)	
	int  m_ChannelCount;
	LONGLONG m_SliderOffset;  // Slider偏移(字)
	LONGLONG m_Offset;        // 文件偏移(字)
	WORD m_ADBuffer[8192];
	int m_Rate;            // 回放速度
    ULONG m_ScreenOffset;  // 屏幕位置
	int m_nFileNum;        // 文件号
	LONG m_nCount;          // 各个通道的数据个数
	WORD m_wMaxLSB;
	float m_fLsbCount;
	WORD m_lLsbHalf;
	ULONG m_ChCfg[MAX_CHANNEL_COUNT];
public:
	void ReadData(void);
	PWORD GetBuffer(LONGLONG* pDataSizeWords);
	PWORD GetBuffer(int iChannel = 0, LONGLONG iOffSet = 0, LONGLONG* pDataSizeWords = NULL);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADHistDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	UINT m_ReadDataSize;
	int m_nDigitLine;
	virtual ~CADHistDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	BOOL m_bAsynMode;
	//{{AFX_MSG(CADHistDoc)
	afx_msg void OnHistoryData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HistDataDoc_H__D442B985_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_)
