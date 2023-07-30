// SysView.cpp : implementation of the CSysView class
//

#include "stdafx.h"
#include "Sys.h"

#include "SysDoc.h"
#include "SysView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysView

IMPLEMENT_DYNCREATE(CSysView, CView)

BEGIN_MESSAGE_MAP(CSysView, CView)
	//{{AFX_MSG_MAP(CSysView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysView construction/destruction

CSysView::CSysView()
{
	// TODO: add construction code here

}

CSysView::~CSysView()
{
}

BOOL CSysView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSysView drawing

void CSysView::OnDraw(CDC* pDC)
{
	CSysDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CSysView printing

BOOL CSysView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSysView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSysView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CSysView diagnostics

#ifdef _DEBUG
void CSysView::AssertValid() const
{
	CView::AssertValid();
}

void CSysView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSysDoc* CSysView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSysDoc)));
	return (CSysDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSysView message handlers
