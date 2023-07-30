#if !defined(AFX_HistDataDoc_H__D442B985_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_)
#define AFX_HistDataDoc_H__D442B985_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistDataDoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CADHistDoc document

class CADHistDoc : public CDocument
{
protected:
	CADHistDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CADHistDoc)

public:
   	HWND m_hWndDigit;        // 数字窗口句柄
	HWND m_hWndWave;         // 波形窗口句柄

// Attributes
public:
// 	int m_nDigitWidth;
	CFile m_File;          // 历史文件
	DWORD m_FileLength;    // 文件长度
	FILE_HEADER m_Header;  // 保存文件头信息
    int  m_ChannelCount;   // 通道数
    ULONG m_Offset;        // 文件偏移(字)
	int m_Rate;            // 回放速度
    ULONG m_ScreenOffset;  // 屏幕位置
	int m_nFileNum;        // 文件号
    LONG m_ADBuffer[HALF_SIZE_WORDS]; // 文件数据缓冲区
	LONG m_nCount;          // 各个通道的数据个数
	BOOL m_bFileOpen;       // 文件是否被打开
public:
	void ReadData(void);
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CADHistDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnNewDocument();
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	UINT m_ReadDataSize; // 读数据个数
	virtual ~CADHistDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CADHistDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HistDataDoc_H__D442B985_8439_11D1_B87A_A3845A0CA5FE__INCLUDED_)
