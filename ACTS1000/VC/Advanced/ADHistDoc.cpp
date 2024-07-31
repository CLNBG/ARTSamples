// HistDataDoc.cpp : implementation file
//

#include "stdafx.h"
#include "ADHistDoc.h"
#include "ADHistDigitView.h"
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
	m_SliderOffset = 0;
	m_Rate = 1;	
	m_nFileNum = 0;
	m_ReadDataSize = 4096;
	m_bAsynMode = FALSE;
	m_HistScreenWith = GetSystemMetrics(SM_CXSCREEN);  // 取得屏幕宽度
	for (int index=0; index<MAX_CHANNEL_COUNT; index++)
	{
		m_HistDataBuffer[index] = new WORD[m_HistScreenWith];
	}
}

BOOL CADHistDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CADHistDoc::~CADHistDoc()
{
	CSysApp *pApp = (CSysApp*)AfxGetApp();
	//pApp->m_strFilePath = "";
	for (int index=0; index<MAX_CHANNEL_COUNT; index++)
	{
		if (NULL != m_HistDataBuffer[index])
		{
			delete [] m_HistDataBuffer[index];
		}	
	}
}

BEGIN_MESSAGE_MAP(CADHistDoc, CDocument)
	//{{AFX_MSG_MAP(CADHistDoc)
	ON_COMMAND(IDM_HistoryData, OnHistoryData)
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
	CSysApp *pApp = (CSysApp*)AfxGetApp();
	//TCHAR* pFileName = pApp->m_strFilePath.GetBuffer(sizeof(pApp->m_strFilePath)); // 类型转换

	if (m_File.Open(lpszPathName, CFile::modeRead)) // 打开文件
	{
		m_File.Seek(0, CFile::begin);
		m_File.Read((WORD*)&m_Header, sizeof(m_Header));

		m_ChannelCount = m_Header.ChannelCount;
		m_ReadDataSize = m_ReadDataSize-m_ReadDataSize%(m_ChannelCount);
	}
	m_FileLength = ((m_File.GetLength() /*- sizeof(::_FILE_HEADER)*/)); // 文件长度(字)

	if (m_FileLength<0x7FFFFFFF)
	{
		m_SliderRatio = 1;
	}
	else
	{
		m_SliderRatio = (m_FileLength/0x7FFFFFFF)+1;
// 		if (m_SliderRatio<100)
// 		{
// 			m_SliderRatio = 100;
// 		}
	}


	m_fLsbCount = (float)m_Header.ADMainInfo.nSampCodeCount;
	m_wMaxLSB = (WORD)(m_Header.ADMainInfo.nSampCodeCount-1);
	m_lLsbHalf = (WORD)(m_Header.ADMainInfo.nSampCodeCount/2);

	int nSampChanCount = 0;
	for (int nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
	{
		if (m_Header.ADPara.bChannelArray[nChannel] == TRUE)
		{	
			m_ChCfg[nSampChanCount] =nChannel;
			nSampChanCount++;
		}
	}

	return TRUE;
}

void CADHistDoc::ReadData(void)
{	
	m_File.Seek(0, CFile::begin);
	if (m_nCount>256) m_nCount = 256;

	LONGLONG loffset =  (m_Offset * sizeof(WORD))*m_ChannelCount + m_Header.HeadSizeBytes;
	m_File.Seek(loffset, CFile::begin);
	try 
	{
		m_nCount = m_File.Read((ULONG*)&m_ADBuffer, sizeof(m_ADBuffer));
	}
	catch(...)
	{
		AfxMessageBox(_T("文件访问过程出现异常错误"));
		return;
	}
	
	m_nCount = m_nCount/sizeof(WORD)/m_ChannelCount;
}

void CADHistDoc::OnHistoryData() 
{
	// TODO: Add your command handler code here
	((CSysApp*)AfxGetApp())->OnHistoryData();	
}

PWORD CADHistDoc::GetBuffer(LONGLONG* pDataSizeWords) //
{
	ASSERT(pDataSizeWords != NULL);
	m_File.Seek(0, CFile::begin);

	LONGLONG loffset = sizeof(::_FILE_HEADER)+(m_Offset*sizeof(WORD))*m_ChannelCount;
	
	*pDataSizeWords = (m_FileLength /*- m_Header.HeadSizeBytes*/-loffset)/(sizeof(WORD)*m_ChannelCount);

	if (*pDataSizeWords == -1)
	{
		int a= 0;
	}
	m_File.Seek(loffset, CFile::begin);
	try
	{
		m_File.Read((ULONG*)&m_ADBuffer, sizeof(m_ADBuffer));
	}
	catch(...)
	{
		AfxMessageBox(_T("文件访问过程出现异常错误"));
		return 0;
	}
	
	return m_ADBuffer;
}


PWORD CADHistDoc::GetBuffer(int iChannel, LONGLONG iOffset, LONGLONG* pDataSizeWords)
{
	ASSERT(iOffset >= 0);
	if ((iChannel>=0) && (iChannel<MAX_CHANNEL_COUNT))
	{
		if ((iChannel>=0) && (iChannel<MAX_CHANNEL_COUNT))
		{
			LONGLONG iDataLength = 0;  // 整个缓冲区长度
			PWORD pDataBuff = this->GetBuffer(&iDataLength);
		
 			//iDataLength = iDataLength/m_ChannelCount; // 转化为单个通道的长度
			// 单个通道数据点数
			
			int iDataPos = 0; // 数据在大缓冲区中的位置
			int iPos=0;
			for ( iPos=0; iPos<m_HistScreenWith; iPos++)//??
			{
				if (iPos > iDataLength)
				{
					break;  
				}
				// 计算在大缓冲区中的位置
				iDataPos = iPos * m_ChannelCount + (iChannel - 0);
				m_HistDataBuffer[iChannel][iPos] =	(SHORT)(pDataBuff[iDataPos] & m_wMaxLSB);

				if ((iOffset+iPos) > (LONGLONG)((m_FileLength-sizeof(::_FILE_HEADER))/(sizeof(WORD) * m_ChannelCount)))
				{
					m_HistDataBuffer[iChannel][iPos] = NULL;
				}
			}

			if (pDataSizeWords != NULL)
			{   
				// 返回数据点个数
				*pDataSizeWords = iPos-1;
			}

			return m_HistDataBuffer[iChannel];
		}
		else
		{
			ASSERT(FALSE); // 没有对该通道采集数据
			return NULL;
		}

	}
	else
	{
		ASSERT(FALSE); // 通道参数不合理
		return NULL;
	}

	return NULL;	
}