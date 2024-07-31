// AIChanList.cpp : implementation file
//

#include "stdafx.h"
#include "Sys.h"

#include "AIChanList.h"
#include "ParaCfgView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAIChanList

CAIChanList::CAIChanList()
{
	m_bEnableEx = TRUE;
	m_nCurItem = 0;
	m_nCurSubItem = 0;
}

CAIChanList::~CAIChanList()
{
}

BEGIN_MESSAGE_MAP(CAIChanList, CListCtrl)
	//{{AFX_MSG_MAP(CAIChanList)
	ON_WM_LBUTTONDOWN()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_CREATE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETFOCUS()
	ON_WM_CONTEXTMENU()
	ON_WM_DRAWITEM()
	ON_COMMAND(IDM_SameParam, OnSameParam)
	ON_MESSAGE(WM_SET_CTRL_FOCUS, OnSetCtrlFocus)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAIChanList message handlers
void CAIChanList::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bEnableEx == FALSE) return;

	*pResult = 0;
	LPNMITEMACTIVATE lpItem = (LPNMITEMACTIVATE)pNMHDR;
	m_nChanIdx = lpItem->iItem;
	m_nType = lpItem->iSubItem;

	if(m_nChanIdx != -1)
	{
		CRect rcCtrl;
		GetSubItemRect(m_nChanIdx, m_nType, LVIR_LABEL, rcCtrl);
		RestoreCtrl(rcCtrl);
	}
}

extern CSysApp theApp;
BOOL CAIChanList::RestoreCtrl(CRect rcCtrl)
{
	CParaCfgView* pParent = (CParaCfgView*)GetParent();
	CADDoc* pDoc = (CADDoc*)pParent->GetDocument();
	CString strMsg, strTmp, strCur;
	int nCurSel = 0, bChannelEn = 0, nSampleRange = 0, nSampleGain = 0, nCouplingType = 0;
	switch(m_nType)
	{
// 	case LIST_COL_CHANNEL:
// 		rcCtrl.bottom = rcCtrl.bottom + 200;
// 		m_wndCombo.MoveWindow(rcCtrl);
// 		m_wndCombo.ResetContent();
// 		for(nChannel=0; nChannel<ACTS1000_AD_MAX_CHANNELS; nChannel++)
// 		{
// 			strMsg.Format("AI%d", nChannel);
// 			m_wndCombo.AddString(strMsg);
// 		}
// 
// 		//??
// 		strCur.Format("AI%d", ADPara.bChannelArray[m_nChanIdx]);
// 		nCurSel = m_wndCombo.FindString(0, strCur);
// 		if(nCurSel == -1)
// 			nCurSel = 0;
// 
// 		m_wndCombo.SetCurSel(nCurSel);
// 		m_wndCombo.EnableWindow(TRUE);
// 		m_wndCombo.ShowWindow(SW_SHOW);
// 		m_wndCombo.SendMessage(CB_SHOWDROPDOWN, TRUE, 0);
// 		m_wndCombo.SetFocus();
// 		break;

		
	case LIST_COL_EN:	
		rcCtrl.bottom = rcCtrl.bottom + 200;
		m_wndCombo.MoveWindow(rcCtrl);
		m_wndCombo.ResetContent();
		for(bChannelEn=0; bChannelEn<2; bChannelEn++)
		{
			m_wndCombo.AddString(g_strChannelEn[bChannelEn]);
		}
		
		nCurSel = ADPara.bChannelArray[m_nChanIdx];
		
		m_wndCombo.SetCurSel(nCurSel);
		m_wndCombo.EnableWindow(TRUE);
		m_wndCombo.ShowWindow(SW_SHOW);
		m_wndCombo.SendMessage(CB_SHOWDROPDOWN, TRUE, 0);
		m_wndCombo.SetFocus();
        break;

 	case LIST_COL_SAMPRANGE:
 		rcCtrl.bottom = rcCtrl.bottom + 200;
 		m_wndCombo.MoveWindow(rcCtrl);
 		m_wndCombo.ResetContent();
		nCurSel = 0;
 		for(nSampleRange=0; nSampleRange<m_nSampRangeCount; nSampleRange++)
 		{
			m_wndCombo.AddString(g_strAIRange[m_nSampRangeVal[nSampleRange]]);
			if (ADPara.InputRange[m_nChanIdx] == m_nSampRangeVal[nSampleRange])
			{
				nCurSel = nSampleRange;
			}
 		}

 		//nCurSel = ADPara.InputRange[m_nChanIdx];
 		m_wndCombo.SetCurSel(nCurSel);
 		m_wndCombo.EnableWindow(TRUE);
 		m_wndCombo.ShowWindow(SW_SHOW);
 		m_wndCombo.SendMessage(CB_SHOWDROPDOWN, TRUE, 0);
 		m_wndCombo.SetFocus();
 		break;
	case LIST_COL_REFGND:
		rcCtrl.bottom = rcCtrl.bottom + 200;
		m_wndCombo.MoveWindow(rcCtrl);
		m_wndCombo.ResetContent();
		for(nCouplingType=0; nCouplingType<2; nCouplingType++)
		{
			m_wndCombo.AddString(g_strRefGround[nCouplingType]);
		}			
		nCurSel = ADPara.CouplingType[m_nChanIdx];
		m_wndCombo.SetCurSel(nCurSel);
		m_wndCombo.EnableWindow(TRUE);
		m_wndCombo.ShowWindow(SW_SHOW);
		m_wndCombo.SendMessage(CB_SHOWDROPDOWN, TRUE, 0);
		m_wndCombo.SetFocus();
		break;
	case LIST_COL_IMPED:
		rcCtrl.bottom = rcCtrl.bottom + 200;
		m_wndCombo.MoveWindow(rcCtrl);
		m_wndCombo.ResetContent();
		for(nCouplingType=0; nCouplingType<2; nCouplingType++)
		{
			m_wndCombo.AddString(g_strInputImped[nCouplingType]);
		}			
		nCurSel = ADPara.InputImped[m_nChanIdx];
		m_wndCombo.SetCurSel(nCurSel);
		m_wndCombo.EnableWindow(TRUE);
		m_wndCombo.ShowWindow(SW_SHOW);
		m_wndCombo.SendMessage(CB_SHOWDROPDOWN, TRUE, 0);
		m_wndCombo.SetFocus();
		break;
	}
	return TRUE;
}

// 当Edit或ComboBox失去焦点时就会调用此函数，以取得改变后的值
LRESULT CAIChanList::OnSetCtrlFocus(WPARAM wParam, LPARAM lParam)
{
	CParaCfgView* pParent = (CParaCfgView*)GetParent();

	CString strMsg;
	int nCurSel = 0;
	int nChannel = 0;
	 ULONG nSampleRange = 0;
	ULONG nSampChanCount = 0;
	switch(m_nType)
	{
	case LIST_COL_CHANNEL:
		nCurSel = m_wndCombo.GetCurSel();
		m_wndCombo.GetLBText(nCurSel, strMsg);
		ADPara.bChannelArray[m_nChanIdx] = nCurSel;
		SetItemText(m_nChanIdx, m_nType, strMsg);
		break;

		
	case LIST_COL_EN:
		nCurSel = m_wndCombo.GetCurSel();
		m_wndCombo.GetLBText(nCurSel, strMsg);
		ADPara.bChannelArray[m_nChanIdx] = nCurSel;
		if (gl_ADMainInfo.nChannelCount>2)
		{
			switch (m_nChanIdx)
			{
			case 0:
				ADPara.bChannelArray[0] = 1;
				ADPara.bChannelArray[1] = 0;
				ADPara.bChannelArray[2] = 0;
				ADPara.bChannelArray[3] = 0;
				ADPara.bChannelArray[4] = 0;
				ADPara.bChannelArray[5] = 0;
				ADPara.bChannelArray[6] = 0;
				ADPara.bChannelArray[7] = 0;
				break;
			case 1:
				if (nCurSel == 0)
				{
					ADPara.bChannelArray[0] = 1;
					ADPara.bChannelArray[1] = 0;
					ADPara.bChannelArray[2] = 0;
					ADPara.bChannelArray[3] = 0;
					ADPara.bChannelArray[4] = 0;
					ADPara.bChannelArray[5] = 0;
					ADPara.bChannelArray[6] = 0;
					ADPara.bChannelArray[7] = 0;
				}
				else
				{
					ADPara.bChannelArray[0] = 1;
					ADPara.bChannelArray[1] = 1;
					ADPara.bChannelArray[2] = 0;
					ADPara.bChannelArray[3] = 0;
					ADPara.bChannelArray[4] = 0;
					ADPara.bChannelArray[5] = 0;
					ADPara.bChannelArray[6] = 0;
					ADPara.bChannelArray[7] = 0;
				}
				break;
			case 2:
				if (nCurSel == 0)
				{
					ADPara.bChannelArray[0] = 1;
					ADPara.bChannelArray[1] = 1;
					ADPara.bChannelArray[2] = 0;
					ADPara.bChannelArray[3] = 0;
					ADPara.bChannelArray[4] = 0;
					ADPara.bChannelArray[5] = 0;
					ADPara.bChannelArray[6] = 0;
					ADPara.bChannelArray[7] = 0;
				}
				else
				{
					ADPara.bChannelArray[0] = 1;
					ADPara.bChannelArray[1] = 1;
					ADPara.bChannelArray[2] = 1;
					ADPara.bChannelArray[3] = 1;
					ADPara.bChannelArray[4] = 0;
					ADPara.bChannelArray[5] = 0;
					ADPara.bChannelArray[6] = 0;
					ADPara.bChannelArray[7] = 0;
				}
				break;
			case 3:
				if (nCurSel == 0)
				{
					ADPara.bChannelArray[0] = 1;
					ADPara.bChannelArray[1] = 1;
					ADPara.bChannelArray[2] = 0;
					ADPara.bChannelArray[3] = 0;
					ADPara.bChannelArray[4] = 0;
					ADPara.bChannelArray[5] = 0;
					ADPara.bChannelArray[6] = 0;
					ADPara.bChannelArray[7] = 0;
				}
				else
				{
					ADPara.bChannelArray[0] = 1;
					ADPara.bChannelArray[1] = 1;
					ADPara.bChannelArray[2] = 1;
					ADPara.bChannelArray[3] = 1;
					ADPara.bChannelArray[4] = 0;
					ADPara.bChannelArray[5] = 0;
					ADPara.bChannelArray[6] = 0;
					ADPara.bChannelArray[7] = 0;
				}
				break;
			case 4:
			case 5:
			case 6:
			case 7:
				if (nCurSel == 0)
				{
					ADPara.bChannelArray[0] = 1;
					ADPara.bChannelArray[1] = 1;
					ADPara.bChannelArray[2] = 1;
					ADPara.bChannelArray[3] = 1;
					ADPara.bChannelArray[4] = 0;
					ADPara.bChannelArray[5] = 0;
					ADPara.bChannelArray[6] = 0;
					ADPara.bChannelArray[7] = 0;
				}
				else
				{
					ADPara.bChannelArray[0] = 1;
					ADPara.bChannelArray[1] = 1;
					ADPara.bChannelArray[2] = 1;
					ADPara.bChannelArray[3] = 1;
					ADPara.bChannelArray[4] = 1;
					ADPara.bChannelArray[5] = 1;
					ADPara.bChannelArray[6] = 1;
					ADPara.bChannelArray[7] = 1;
				}
				break;
			}
		}


		//SetItemText(m_nChanIdx, m_nType, strMsg);
		//pParent->SetCurveRange(m_nChanIdx, nCurSel);
		for(nChannel=0; nChannel<gl_ADMainInfo.nChannelCount; nChannel++)
		{

			SetItemText(nChannel, m_nType, g_strChannelEn[ADPara.bChannelArray[nChannel]]);	
			if(ADPara.bChannelArray[nChannel])
			{
				//g_lChCfg[nSampChanCount] = nChannel;
				gl_CHIdx[nChannel] = nSampChanCount;
				nSampChanCount++;
			}
		}
				
		if(nSampChanCount == 0)
		{	
			AfxMessageBox(L"采样通道不能全部禁止，请重新设定");		   
			m_wndCombo.GetLBText(1, strMsg);
			SetItemText(m_nChanIdx, m_nType, strMsg);
			ADPara.bChannelArray[m_nChanIdx] = 1;
		}
		else
		{
//??? 			g_nSampChanCount = nSampChanCount;
// 			ADPara.nSampChanCount = nSampChanCount;
// 			pParent->OnSelchangeCOMBOnSampChanCount();

			gl_nChannelCount = nSampChanCount;
		}
		pParent->EnableVootRange();
 		break;

 	case LIST_COL_SAMPRANGE:
 		nCurSel = m_wndCombo.GetCurSel();
 		m_wndCombo.GetLBText(nCurSel, strMsg);
 		ADPara.InputRange[m_nChanIdx] = m_nSampRangeVal[nCurSel];
 		SetItemText(m_nChanIdx, m_nType, strMsg);		
		//???pParent->SetCurveRange(m_nChanIdx, nCurSel);

		pParent->EnableVootRange();
 		break;

	case LIST_COL_REFGND:
		nCurSel = m_wndCombo.GetCurSel();
		m_wndCombo.GetLBText(nCurSel, strMsg);
		ADPara.CouplingType[m_nChanIdx] = nCurSel;
		SetItemText(m_nChanIdx, m_nType, strMsg);		
		break;

	case LIST_COL_IMPED:
		nCurSel = m_wndCombo.GetCurSel();
		m_wndCombo.GetLBText(nCurSel, strMsg);
		ADPara.InputImped[m_nChanIdx] = nCurSel;
		SetItemText(m_nChanIdx, m_nType, strMsg);		
		break;
	}

	return TRUE;
}

void CAIChanList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_bEnableEx == FALSE) return;

	// TODO: Add your message handler code here and/or call default
	SetFocus();
	CListCtrl::OnLButtonDown(nFlags, point);
}

BOOL CAIChanList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	m_wndCombo.ShowWindow(SW_HIDE);
	m_wndCombo.EnableWindow(FALSE);	

	m_wndEdit.ShowWindow(SW_HIDE);
	m_wndEdit.EnableWindow(FALSE);	
	
	return CListCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CAIChanList::PreSubclassWindow() 
{
	// TODO: Add your specialized creation code here	
	CListCtrl::PreSubclassWindow();

	// 必须在这里创建
	DWORD dwStyle;
	CRect Rect(0, 0, 10, 10);

	dwStyle = WS_BORDER | WS_CHILD | ES_LEFT | ES_AUTOHSCROLL;
	if( !m_wndEdit.Create(dwStyle, Rect, this, 0 ) )
		MessageBox(L"m_wndEdit.Create Error");		

	dwStyle = WS_CHILD | CBS_DROPDOWNLIST | WS_VSCROLL;
	if( !m_wndCombo.Create(dwStyle, Rect, this, 0 ) )
		MessageBox(L"m_wndCombo.Create Error");		

}

void CAIChanList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	m_wndCombo.ShowWindow(SW_HIDE);
	m_wndCombo.EnableWindow(FALSE);	

	m_wndEdit.ShowWindow(SW_HIDE);
	m_wndEdit.EnableWindow(FALSE);	

	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CAIChanList::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	m_wndCombo.ShowWindow(SW_HIDE);
	m_wndCombo.EnableWindow(FALSE);	

	m_wndEdit.ShowWindow(SW_HIDE);
	m_wndEdit.EnableWindow(FALSE);	

	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CAIChanList::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHEADER* pNMHeader = (NMHEADER*)lParam;
	if(pNMHeader->hdr.code == -12) // 如果鼠标左键在Header上按下时
	{
 		m_wndCombo.ShowWindow(SW_HIDE);
		m_wndCombo.EnableWindow(FALSE);	

		m_wndEdit.ShowWindow(SW_HIDE);
		m_wndEdit.EnableWindow(FALSE);
	}
 
	return CListCtrl::OnNotify(wParam, lParam, pResult);	
}

void CAIChanList::SetSameParam(int nCurItem, int nSubItem)
{
	CString strNote;
	int nID = 0, nItem = 0;
	CParaCfgView* pParent = (CParaCfgView*)GetParent();
	int nChannel=0;
	ULONG nSampChanCount = 0;

	//ACTS1000_PARA_AD CHParam = ADPara.CHParam[nCurItem];
	LONG bCHChannelArray = ADPara.bChannelArray[nCurItem];
	LONG lCHInputRange = ADPara.InputRange[nCurItem];
	LONG lCHCouplingType = ADPara.CouplingType[nCurItem];
	LONG lCHInputImped = ADPara.InputImped[nCurItem];

	switch(nSubItem)
	{
// 	case LIST_COL_CHANNEL:
// 		strNote.Format("确定要将其他所有通道号设定成与%d号的一样吗？", nCurItem);
// 		nID = AfxMessageBox(strNote, MB_YESNO);
// 		if(nID == IDNO)
// 			break;
// 
// 		strNote.Format("AI%d", bCHChannelArray);
// 		for(nItem=0; nItem<GetItemCount(); nItem++)
// 		{
// 			ADPara.bChannelArray[nItem] = bCHChannelArray;
// 			SetItemText(nItem, nSubItem, strNote);
// 		}
// 		break;	

	case LIST_COL_EN:
		strNote.Format(L"确定要将其他所有通道使能设定成与%d号的一样吗？", nCurItem);
		nID = AfxMessageBox(strNote, MB_YESNO);
		if(nID == IDNO)
			break; 
		
		strNote = GetItemText(nCurItem, nSubItem);			
		
		for(nItem=0; nItem<GetItemCount(); nItem++)
		{
			ADPara.bChannelArray[nItem] = bCHChannelArray;
			SetItemText(nItem, nSubItem, strNote);
		}		
		
		if(bCHChannelArray)
		{
		} 
		else
		{
			//AfxMessageBox(L"采样通道不能全部禁止，请重新设定");
			
			for(nItem=0; nItem<GetItemCount(); nItem++)
			{
				ADPara.bChannelArray[nItem] = 0;
				SetItemText(nItem, nSubItem, strNote);
			}		
			ADPara.bChannelArray[0] = 1;
			SetItemText(0, nSubItem, L"允许");
			
		}		
		
		nSampChanCount = 0;
		for( nChannel=0; nChannel<gl_ADMainInfo.nChannelCount; nChannel++)
		{
			if (ADPara.bChannelArray[nChannel]==1)
			{
				//g_lChCfg[nSampChanCount] = nChannel;
				gl_CHIdx[nChannel] = nSampChanCount;
				nSampChanCount++;
			}
		}
//??? 		g_nSampChanCount = nSampChanCount;
// 		ADPara.nSampChanCount = nSampChanCount;
// 		pParent->OnSelchangeCOMBOnSampChanCount();
		gl_nChannelCount = nSampChanCount;
		pParent->EnableVootRange();
		break;

 	case LIST_COL_SAMPRANGE:
 		strNote.Format(L"确定要将其他所有采样范围设定成与%d号的一样吗？", nCurItem);
 		nID = AfxMessageBox(strNote, MB_YESNO);
 		if(nID == IDNO)
 			break; 
 
 		strNote = GetItemText(nCurItem, nSubItem);
 		for(nItem=0; nItem<GetItemCount(); nItem++)
 		{			
 			ADPara.InputRange[nItem] = lCHInputRange;
 			SetItemText(nItem, nSubItem, strNote);
			//???pParent->SetCurveRange(nItem, lCHInputRange);
 		}
		pParent->EnableVootRange();
 		break;
		
	case LIST_COL_REFGND:
		strNote.Format(L"确定要将其他所有参考地设定成与%d号的一样吗？", nCurItem);
		nID = AfxMessageBox(strNote, MB_YESNO);
		if(nID == IDNO)
			break;

		strNote = GetItemText(nCurItem, nSubItem);
		for(nItem=0; nItem<GetItemCount(); nItem++)
		{
			ADPara.CouplingType[nItem] = lCHCouplingType;
			SetItemText(nItem, nSubItem, strNote);
		}
		break;

	case LIST_COL_IMPED:
		strNote.Format(L"确定要将其他所有阻抗设定成与%d号的一样吗？", nCurItem);
		nID = AfxMessageBox(strNote, MB_YESNO);
		if(nID == IDNO)
			break;

		strNote = GetItemText(nCurItem, nSubItem);
		for(nItem=0; nItem<GetItemCount(); nItem++)
		{
			ADPara.InputImped[nItem] = lCHInputImped;
			SetItemText(nItem, nSubItem, strNote);
		}
		break;
	}
} // SetSameParam

void CAIChanList::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if(m_bEnableEx == FALSE) return;

	CString strTemp, strMsg;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = 0, nSubItem = 0; 
	nItem = GetSelectionMark();
	if(nItem < 0) nItem = 0;
	nSubItem = pNMListView->iSubItem;
	if(nSubItem < 0) nSubItem = 0;
	SetSameParam(nItem, nSubItem);
	*pResult = 0;
} // OnColumnclick

void CAIChanList::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)(pNMHDR);
	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_NEWFONT;
	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	if ( pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	// This is the notification message for an item.  We'll request
	// notifications before each subitem's prepaint stage.
	else // if ( pLVCD->nmcd.dwDrawStage==CDDS_ITEMPREPAINT ) 
	{
		*pResult = CDRF_DODEFAULT;
	}
}

int CAIChanList::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	CFont axisFont;
	axisFont.CreateFont(54, 120, 11, 32, 400,
		FALSE, FALSE, 10, ANSI_CHARSET,
		OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, 
		DEFAULT_PITCH|FF_SWISS, L"MS Sans Serif");

	SetFont(&axisFont);	

	return 0;
}

BOOL CAIChanList::EnableWindowEx(BOOL bEnable)
{
	BOOL bOldEnable = m_bEnableEx;
	m_bEnableEx = bEnable;
	return bOldEnable;
}

void CAIChanList::OnSetFocus(CWnd* pOldWnd) 
{
	// 必须隐掉，否则当该控件获得焦点时再由其他进程窗口当住再恢复窗前时List会显示模糊。
	//	CListCtrl::OnSetFocus(pOldWnd); 
}

void CAIChanList::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	int nIndex = 0, nItemCount, nSeledCount = 0;
	nItemCount = GetItemCount();
	for(nIndex=0; nIndex<nItemCount; nIndex++)
	{
		UINT nStatus = GetItemState(nIndex, LVIS_SELECTED);
		if(nStatus == 0)
		{
			continue;
		}
		nSeledCount++;
	}

	if(nSeledCount == 0)
		return;

	if (point.x == -1 && point.y == -1)
	{
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);			
		point = rect.TopLeft();
		point.Offset(5, 5);
	}
	
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_ListCHParam));
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, pWndPopupOwner);
	
} // OnContextMenu

void CAIChanList::OnSameParam() 
{
	int nItem = GetSelectionMark();
	if(nItem != -1)
	{
		SetSameParam(m_nCurItem, m_nCurSubItem);
	}	
}

void CAIChanList::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)(pNMHDR);
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int nItem = 0, nSubItem = 0; 
	m_nCurItem = pNMListView->iItem;
	m_nCurSubItem = pNMListView->iSubItem;
	*pResult = 0;
}
