// SysDoc.cpp : implementation of the CSysDoc class
//

#include "stdafx.h"
#include "Sys.h"

#include "SysDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysDoc

IMPLEMENT_DYNCREATE(CSysDoc, CDocument)

BEGIN_MESSAGE_MAP(CSysDoc, CDocument)
	//{{AFX_MSG_MAP(CSysDoc)
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysDoc construction/destruction

CSysDoc::CSysDoc()
{
	// TODO: add one-time construction code here

}

CSysDoc::~CSysDoc()
{
}

BOOL CSysDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSysDoc serialization

void CSysDoc::Serialize(CArchive& ar)
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
// CSysDoc diagnostics

#ifdef _DEBUG
void CSysDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSysDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSysDoc commands

