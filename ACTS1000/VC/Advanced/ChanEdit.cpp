// ChanEdit.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"
#include "ChanEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChanEdit

CChanEdit::CChanEdit()
{
}

CChanEdit::~CChanEdit()
{
}


BEGIN_MESSAGE_MAP(CChanEdit, CEdit)
	//{{AFX_MSG_MAP(CChanEdit)
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChanEdit message handlers

int CChanEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Font.CreatePointFont(130, L"ºÚÌå");
	SetFont(&m_Font);
	
	return 0;
}

void CChanEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	ShowWindow(SW_HIDE);
	EnableWindow(FALSE);

	CWnd* pParent = (CWnd* )GetParent();
	::SendMessage(pParent->m_hWnd, WM_SET_CTRL_FOCUS, NULL, NULL);
}
