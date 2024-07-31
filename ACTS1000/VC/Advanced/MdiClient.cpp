 // MdiClient.cpp : implementation file
//
/////////////////////////////////////////////////////////////////////////////
// This class does subclass the MDI-CLIENT window.
// Subclassing means that all messages are first routed to this class, then 
// to the original window (in this case the MDI-CLIENT).
// We need this to get notifications of the creation and deletion of the 
// MDI child frames (contain views).
/////////////////////////////////////////////////////////////////////////////
//
// Copyright ?1998 Written by Dieter Fauth 
//		mailto:fauthd@zvw.de 
//  
// This code may be used in compiled form in any way you desire. This    
// file may be redistributed unmodified by any means PROVIDING it is     
// not sold for profit without the authors written consent, and     
// providing that this notice and the authors name and all copyright     
// notices remains intact. If the source code in this file is used in     
// any  commercial application then a statement along the lines of     
// "Portions Copyright ?1999 Dieter Fauth" must be included in     
// the startup banner, "About" box or printed documentation. An email     
// letting me know that you are using it would be nice as well. That's     
// not much to ask considering the amount of work that went into this.    
//    
// This file is provided "as is" with no expressed or implied warranty.    
// The author accepts no liability for any damage/loss of business that    
// this product may cause.    
//  
// ==========================================================================  
// HISTORY:	  
// ==========================================================================  
//			1.00	08 May 1999	- Initial release.  
// ==========================================================================  
//  
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TabCtrlBarDoc.h"
#include "MdiClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMdiClient

CMdiClient::CMdiClient(): m_sizeClient(0, 0)
{
	m_crBkColor = GetSysColor(COLOR_DESKTOP);
	m_pWndTabs = NULL;
}


CMdiClient::~CMdiClient()
{
}


BEGIN_MESSAGE_MAP(CMdiClient, CWnd)
	//{{AFX_MSG_MAP(CMdiClient)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_MESSAGE(WM_MDICREATE,OnMDICreate)
	ON_MESSAGE(WM_MDIDESTROY,OnMDIDestroy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
void CMdiClient::AddHandle(HWND hWnd)
{
	ASSERT(m_pWndTabs != NULL);
	// 添加列表
	m_pWndTabs->AddHandle(hWnd);
}

void CMdiClient::RemoveHandle(HWND hWnd)
{
	ASSERT(m_pWndTabs != NULL);
	m_pWndTabs->RemoveHandle(hWnd);
}

/////////////////////////////////////////////////////////////////////////////
// CMdiClient message handlers

LRESULT CMdiClient::OnMDICreate(WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = (HWND) DefWindowProc(WM_MDICREATE,  wParam, lParam);
	AddHandle(hWnd);
	return (LRESULT) hWnd;
}

LRESULT CMdiClient::OnMDIDestroy(WPARAM wParam, LPARAM lParam)
{
	RemoveHandle((HWND) wParam);
	return DefWindowProc(WM_MDIDESTROY,  wParam, lParam);
}



BOOL CMdiClient::OnEraseBkgnd(CDC* pDC) 
{	
	return CWnd::OnEraseBkgnd(pDC);
}
void CMdiClient::OnSize(UINT nType, int cx, int cy) 
{
	// 当客户区域变化时，被调用。即当子帧窗口被最大化时，也会产生此消息
	CWnd::OnSize(nType, cx, cy);
	
	// 如果应用程序当启动，则保存这个大小参数，即可返回
    if ((m_sizeClient.cx == 0) && (m_sizeClient.cy == 0))
	{
        m_sizeClient.cx = cx;
        m_sizeClient.cy = cy;
		
        return ;
	}	
    // 如果客户窗口大小未发生变化，则返回
    if ((m_sizeClient.cx == cx) && ( m_sizeClient.cy == cy))
    { 
        return;
    }	
	// 保存新值
    m_sizeClient.cx = cx;
    m_sizeClient.cy = cy;
	// 强制重绘
    RedrawWindow(NULL, NULL,
        RDW_INVALIDATE | RDW_ERASE | RDW_ERASENOW | RDW_ALLCHILDREN);    
	
    return;                
}