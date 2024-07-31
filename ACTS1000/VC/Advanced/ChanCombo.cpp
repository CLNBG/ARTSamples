// ChanCombo.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"
#include "ChanCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChanCombo

CChanCombo::CChanCombo()
{
}

CChanCombo::~CChanCombo()
{
}


BEGIN_MESSAGE_MAP(CChanCombo, CComboBox)
	//{{AFX_MSG_MAP(CChanCombo)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChanCombo message handlers

void CChanCombo::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);

	ShowWindow(SW_HIDE);
	EnableWindow(FALSE);

	// CWnd* pParent = (CWnd* )GetParent();
	// ::SendMessage(pParent->m_hWnd, WM_SET_CTRL_FOCUS, NULL, NULL);
}

int CChanCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_Font.CreatePointFont(90, L"ËÎÌו");
	SetFont(&m_Font);

	return 0;
}

void CChanCombo::OnSelchange() 
{
	CWnd* pParent = (CWnd* )GetParent();
	::SendMessage(pParent->m_hWnd, WM_SET_CTRL_FOCUS, NULL, NULL);
}
