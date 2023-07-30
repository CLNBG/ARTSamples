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
   	HWND m_hWndDigit;        // ���ִ��ھ��
	HWND m_hWndWave;         // ���δ��ھ��

// Attributes
public:
// 	int m_nDigitWidth;
	CFile m_File0;          // ��ʷ�ļ�
	CFile m_File1;          // ��ʷ�ļ�
	DWORD m_FileLength;     // �ļ�����
	FILE_HEADER m_Header;   // �����ļ�ͷ��Ϣ
	FILE_HEADER m_Header1;  // �����ļ�ͷ��Ϣ
    int  m_ChannelCount;    // ͨ����
    int  m_ChannelCount1;   // ͨ����
    ULONG m_Offset;         // �ļ�ƫ��(��)
	int m_Rate;             // �ط��ٶ�
    ULONG m_ScreenOffset;   // ��Ļλ��
	int m_nFileNum;         // �ļ���
    LONG m_ADBuffer[HALF_SIZE_WORDS]; // �ļ����ݻ�����
	LONG m_ADBuffer1[HALF_SIZE_WORDS];// �ļ����ݻ�����
	LONG m_nCount;          // ����ͨ�������ݸ���
	LONG m_nCount1;         // ����ͨ�������ݸ���
	BOOL m_bFileOpen;       // �ļ��Ƿ񱻴�
	BOOL m_bFileOpen1;      // �ļ��Ƿ񱻴�
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
	UINT m_ReadDataSize; // �����ݸ���
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
