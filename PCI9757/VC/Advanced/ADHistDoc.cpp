// HistDataDoc.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CADHistDoc
IMPLEMENT_DYNCREATE(CADHistDoc, CDocument)

CADHistDoc::CADHistDoc()
{
	m_Offset = 0;
	m_Rate = 1;	
	m_nFileNum = 0;
	m_ReadDataSize = 256;
	m_bFileOpen = FALSE;
	m_nCount = 0;
	memset(&m_Header, 0x00, sizeof(m_Header));
}

BOOL CADHistDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CADHistDoc::~CADHistDoc()
{
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	pApp->m_strFilePath = "";
}

BEGIN_MESSAGE_MAP(CADHistDoc, CDocument)
	//{{AFX_MSG_MAP(CADHistDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADHistDoc diagnostics
#ifdef _DEBUG
void CADHistDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CADHistDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADHistDoc serialization

void CADHistDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CADHistDoc commands

BOOL CADHistDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	DWORD nFileHeaderSize;
	CString str;
	if (m_File.Open(lpszPathName, CFile::modeRead)) // 打开文件
	{
		m_File.Seek(0, CFile::begin);
		m_File.Read((WORD*)&m_Header, sizeof(m_Header)); // 读取文件头
		nFileHeaderSize = sizeof(m_Header);
		m_ReadDataSize = 2048;
		// 读出第一批数据，以首屏显示	
		m_File.Seek(nFileHeaderSize, CFile::begin);  
		m_File.Read(m_ADBuffer, m_ReadDataSize*sizeof(LONG));
		m_FileLength = (ULONG)((m_File.GetLength()-nFileHeaderSize)/sizeof(LONG)); // 文件长度(字)
		m_bFileOpen = TRUE;
	}
	else
	{
		 AfxMessageBox(_T("Failed to Open File"));
		return TRUE;
	}

	this->SetPathName(lpszPathName);

	return TRUE;
}

void CADHistDoc::ReadData(void)
{
	int Channel = 0, nChannel = 0;
	for (Channel  = 0; Channel < MAX_CHANNEL_COUNT; Channel++)
	{
		if (m_Header.ADPara.bChannelArray[Channel] == TRUE)
		{
			nChannel++;
		}
	}

	m_File.Seek(0, CFile::begin);
	m_ChannelCount = nChannel;
	if (m_nCount>256) m_nCount = 256;

	LONG loffset = sizeof(::_FILE_HEADER)+(m_Offset*sizeof(LONG))*m_ChannelCount;
	m_File.Seek(loffset, CFile::begin);
	try
	{
		m_nCount = m_File.Read((LONG*)&m_ADBuffer, sizeof(m_ADBuffer));
	}
	catch(...)
	{
		AfxMessageBox(_T("文件访问过程出现异常错误"));
		return;
	}
	
	m_nCount = m_nCount/sizeof(LONG)/m_ChannelCount;
}


void CADHistDoc::OnCloseDocument() 
{
	if (m_bFileOpen == TRUE)
	{
		m_File.Close();
	}
	
	CDocument::OnCloseDocument();
}
