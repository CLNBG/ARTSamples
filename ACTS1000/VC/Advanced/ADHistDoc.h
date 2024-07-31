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
   	HWND m_hWndDigit;        // ���ִ��ھ��
	HWND m_hWndWave;         // ���δ��ھ��

// Attributes
public:
	int m_nDigitWidth;
	CFile m_File;
	CString m_FileName;
	__int64 m_FileLength;
	__int64	m_SliderRatio;	// Sliderѹ����
	FILE_HEADER m_Header;  // �����ļ�ͷ��Ϣ

// 	LONG m_VoltBottomRange[8];       // ��������(mV)
// 	LONG m_VoltTopRange[8];	        // ��������(mV)	
	int  m_ChannelCount;
	LONGLONG m_SliderOffset;  // Sliderƫ��(��)
	LONGLONG m_Offset;        // �ļ�ƫ��(��)
	WORD m_ADBuffer[8192];
	int m_Rate;            // �ط��ٶ�
    ULONG m_ScreenOffset;  // ��Ļλ��
	int m_nFileNum;        // �ļ���
	LONG m_nCount;          // ����ͨ�������ݸ���
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
