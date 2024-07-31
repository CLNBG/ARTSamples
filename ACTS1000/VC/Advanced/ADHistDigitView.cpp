// HistDigitView.cpp:implementation file
//

#include "stdafx.h"
#include "Sys.h"
#include "ADHistDoc.h"
#include "ADHistDigitView.h"
#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CADHistDigitView

IMPLEMENT_DYNCREATE(CADHistDigitView, CListView)

CADHistDigitView::CADHistDigitView()
{
	nChooseMode = 3;
}

CADHistDigitView::~CADHistDigitView()
{

}


BEGIN_MESSAGE_MAP(CADHistDigitView, CListView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CADHistDigitView)
	ON_COMMAND(IDM_SixteenShow, OnSixteenShow)
	ON_UPDATE_COMMAND_UI(IDM_SixteenShow, OnUpdateSixteenShow)
	ON_COMMAND(IDM_DigitalVoltShowMode, OnDigitalVoltShowMode)
	ON_UPDATE_COMMAND_UI(IDM_DigitalVoltShowMode, OnUpdateDigitalVoltShowMode)
	ON_COMMAND(IDM_TenShow, OnTenShow)
	ON_UPDATE_COMMAND_UI(IDM_TenShow, OnUpdateTenShow)
	ON_COMMAND(IDM_REFURBISH, OnRefurbish)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADHistDigitView drawing

void CADHistDigitView::OnDraw(CDC* )
{	
}
/////////////////////////////////////////////////////////////////////////////
// CADHistDigitView diagnostics

#ifdef _DEBUG
void CADHistDigitView::AssertValid() const
{
	CListView::AssertValid();
}

void CADHistDigitView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
CADHistDoc* CADHistDigitView::GetDocument()// non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CADHistDoc)));
	return (CADHistDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CADHistDigitView message handlers

void CADHistDigitView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	DWORD type;
	type= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;

	// 改变列表视图的风格，在这里为显示格线
	GetListCtrl().SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(type));	

	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	pDoc->m_hWndDigit = m_hWnd; 

	CString str;
	CListCtrl& List = GetListCtrl();
	
	int mChannelCount = pDoc->m_ChannelCount;

	long VoltTopRange, VoltBottomRange;
	VoltTopRange = pDoc->m_Header.VoltTopRange;       // 电压的最大值
	VoltBottomRange = pDoc->m_Header.VoltBottomRange; // 电压的最小值
	if (abs(VoltBottomRange) <= 5) m_jixing = 1;
	else 
		m_jixing = 0;

	int nCnt =0;
	for (int nCH=0; nCH<pDoc->m_Header.ADMainInfo.nChannelCount; nCH++)
	{
		if (pDoc->m_Header.ADPara.bChannelArray[nCH] == 1)
		{

			//m_PerLsbVolt[nCnt] = (float)((pDoc->m_Header.VoltTopRange-pDoc->m_Header.VoltBottomRange)*1.0 / pDoc->m_fLsbCount);	
	
			
			switch (pDoc->m_Header.ADPara.InputRange[nCH])
			{
			case ACTS1000_INPUT_N5000_P5000mV:

				m_PerLsbVolt[nCnt] = (float)(10000.0 / pDoc->m_fLsbCount);	
				
// 				pDoc->m_VoltBottomRange[nCnt] = -5000;
// 				pDoc->m_VoltTopRange[nCnt] = 5000;
				break;
			case ACTS1000_INPUT_N1000_P1000mV:
				m_PerLsbVolt[nCnt] = (float)(2000.0 / pDoc->m_fLsbCount);
// 				pDoc->m_VoltBottomRange[nCnt] = -1000;
// 				pDoc->m_VoltTopRange[nCnt] = 1000;
				break;
			default:
				m_PerLsbVolt[nCnt] = (float)(20000.0 / pDoc->m_fLsbCount);
// 				pDoc->m_VoltBottomRange[nCnt] = -5000;
// 				pDoc->m_VoltTopRange[nCnt] = 5000;
			}
			nCnt++;
		}

	}

	CADHistFrm* pFrame = (CADHistFrm*)GetParentFrame();	
	CListBox* pList = (CListBox*)(pFrame->m_wndSetupBar.GetDlgItem(IDC_FILESINFO));
	switch(pDoc->m_Header.BusType)
	{//1:PCI, 2:USB, 3:ISA, 4:PC104
		case PCI_BUS:
			str="BusType:PCI";
			break;
		case USB_BUS:
			str="BusType:USB";
			break;
		case ISA_BUS:
			str="BusType:ISA";
			break;
		case PC104_BUS:
			str="BusType:PC104";
			break;
		case PXI_BUS:
			str="BusType:PXI";
			break;
		case PCIE_BUS:
			str="BusType:PCIE";
			break;
	}//板卡类型
	
	pList->AddString (str);
	str.Format(L"DeviceNum:%x",pDoc->m_Header.DeviceType );
	pList->AddString (str);
	str.Format(L"HeadSizeBytes:%d",pDoc->m_Header.HeadSizeBytes );
	pList->AddString(str);	

	str.Format(L"分辨率:%d位", pDoc->m_Header.DataWidth);
	pList->AddString(str);
//	str.Format(L"电压量程:%d～%dmV",pDoc->m_Header.VoltBottomRange,pDoc->m_Header.VoltTopRange);
//	pList->AddString (str);//电压量程
	str.Format(L"触发源:%d",pDoc->m_Header.ADPara.TriggerSource);
	pList->AddString (str);//触发源
	str.Format(L"触发方向:%d",pDoc->m_Header.ADPara.TriggerDir);
	pList->AddString (str);


	CEdit* pEditOffset = (CEdit*)(pFrame->m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset));	
	str.Format(L"%I64d", pDoc->m_Offset);
	pEditOffset->SetWindowText(str); // 显示文件偏移

	CEdit* pLength = (CEdit*)(pFrame->m_wndSetupBar.GetDlgItem(IDC_EDIT_FileLength));	
	str.Format(L"%I64d", pDoc->m_FileLength);
	pLength->SetWindowText(str); // 显示文件长度（字）
	
	CSliderCtrl* pSliderOffset = (CSliderCtrl*)(pFrame->m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));	

	// 之所以除以通道数，是求出每通道点数
    pSliderOffset->SetRange(0, ((pDoc->m_FileLength-sizeof(::_FILE_HEADER))/(sizeof(WORD) * mChannelCount)-pDoc->m_ReadDataSize/mChannelCount)/pDoc->m_SliderRatio); // 设置滑动条范围（字）
	pSliderOffset->SetPageSize(1000*pDoc->m_SliderRatio);

	CEdit* pEditPerChannel = (CEdit*)(pFrame->m_wndSetupBar.GetDlgItem(IDC_EDIT_PerChannel));	
	
	str.Format(_T("%I64d点/通道"), (pDoc->m_FileLength-sizeof(::_FILE_HEADER)) / (sizeof(WORD) * mChannelCount));
	pEditPerChannel->SetWindowText(str);  // 显示每通道点数
	CComboBox* pRate = (CComboBox*)(pFrame->m_wndSetupBar.GetDlgItem(IDC_COMBO_Rate));	
	pRate->SetCurSel(0);
	
	CStatic* pLabel = (CStatic*)pFrame->m_wndSetupBar.GetDlgItem(IDC_STATIC_CardType);
	pLabel->SetWindowText(pDoc->m_File.GetFilePath()); 

	List.InsertColumn(0, L"Index" ,LVCFMT_LEFT,42,-1);  // 加入第一列
	
	int Row = 0;	
	for (Row=0; Row<(int)(pDoc->m_ReadDataSize/mChannelCount); Row++)
	{
		str.Format(L"%d", Row);
		List.InsertItem(Row, str); // 添加行号,应一个紧接一个插入	
	} 
	//str.Format(L"数据回放:%s",pDoc->m_File.GetFileName());
	str.Format(L"%s",pDoc->m_File.GetFileName());
	pDoc->SetTitle(str); 
	SetListView(); // 向列表中添加数据
	GetListCtrl().SetHotItem(0);
}

void CADHistDigitView::SetListView(void)
{
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	CListCtrl& List = GetListCtrl();
	int Col = 0, Row = 0;
	List.ShowWindow(SW_HIDE);
	List.SetRedraw(FALSE);
	int index(0);
	int nEndRow = 0;
	int nChannel = 0;
	INT ChannelCount = 	pDoc->m_ChannelCount;

	pDoc->ReadData();
	int ch=0;
// 	for (nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
// 	{
// 		if (gl_ChannelArrary[nChannel] == TRUE)
// 		{	
// 			gl_Channel[ch] =nChannel;
// 			ch++;
// 		}
// 	}

	switch (nChooseMode)
	{
	case 1:
		for (nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
		{
			if (pDoc->m_Header.ADPara.bChannelArray[nChannel] == TRUE)
			{	
				CString strChannel;
				List.DeleteColumn(nChannel + 1);
				strChannel.Format(_T("CH%d"), nChannel + 0);
				List.InsertColumn(nChannel + 1, strChannel, LVCFMT_LEFT, 70, -1);
			}
		}
		break;
	case 2:
		for (nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
		{
			if (pDoc->m_Header.ADPara.bChannelArray[nChannel] == TRUE)
			{	
				CString strChannel;
				List.DeleteColumn(nChannel + 1);
				strChannel.Format(_T("CH%d"), nChannel + 0);
				List.InsertColumn(nChannel + 1, strChannel, LVCFMT_LEFT, 85, -1);
		
			}
		}
		break;
	case 3:
		for (nChannel=0; nChannel<MAX_CHANNEL_COUNT; nChannel++)
		{
			if (pDoc->m_Header.ADPara.bChannelArray[nChannel] == TRUE)
			{	
				CString strChannel;
				List.DeleteColumn(nChannel + 1);
				strChannel.Format(_T("CH%d"), nChannel + 0);
				List.InsertColumn(nChannel + 1, strChannel, LVCFMT_LEFT, 70, -1);
			}
		}
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	__int64 start = pDoc->m_Offset - pDoc->m_Offset % ChannelCount;
	for (Row=0; Row<(INT)(pDoc->m_ReadDataSize/ChannelCount); Row++)
	{
		if((pDoc->m_Offset + Row) < (pDoc->m_FileLength-sizeof(::_FILE_HEADER))/(sizeof(WORD) * ChannelCount))
		{
			swprintf_s(str, _T("%I64d"), pDoc->m_Offset + Row);
			List.SetItemText(Row, 0, str);  // 更新Index列	
			for (Col=0; Col<(INT)ChannelCount; Col++)
			{
				switch (nChooseMode)
				{
				case 1: // 十进制显示			
					swprintf_s(str, _T("%d"), (pDoc->m_ADBuffer[index]) & pDoc->m_wMaxLSB);
					break;
					
				case 2: // 十六进制显示
					swprintf_s(str, _T("%x"), (pDoc->m_ADBuffer[index]) & pDoc->m_wMaxLSB);
					break;
					
				case 3:
					{

						_stprintf(str,_T("%.2f"), (float)((((pDoc->m_ADBuffer[Row*ChannelCount+Col])&pDoc->m_wMaxLSB)-pDoc->m_lLsbHalf)*m_PerLsbVolt[Col]));	
// 						float tempResult;
// 						tempResult = (float)((pDoc->m_ADBuffer[index]) & pDoc->m_wMaxLSB);
// 						tempResult = ((float)(tempResult/LSB_COUNT)) * 2 * voltMid[ADPara.InputRange[gl_Channel[Col]]];
// 						if (m_jixing == 0)
// 							swprintf_s(str, _T("%.2f"), (tempResult - voltMid[ADPara.InputRange[gl_Channel[Col]]]));
// 						else
// 							swprintf_s(str, _T("%.2f"), (tempResult));
						
						break;
					}
				default:
					ASSERT(FALSE);
					break;
				}
				if (index > (INT)(pDoc->m_nCount * ChannelCount)) return;	
				List.SetItemText(Row, (start % ChannelCount) + 1, str);// 添加各行内容
				start++;
				index++;
			}
			nEndRow = Row;
		}
		else
			List.DeleteItem(nEndRow+1);
	} 
	List.SetRedraw(TRUE);
	List.ShowWindow(SW_SHOW);
}

BOOL CADHistDigitView::PreCreateWindow(CREATESTRUCT& cs) 
{	
	// TODO: Add your specialized code here and/or call the base class
	cs.style &= ~(LVS_LIST | LVS_ICON | LVS_SMALLICON);
	cs.style |= LVS_REPORT;
	cs.style |= LVS_SINGLESEL;
	cs.style |= LVS_SHOWSELALWAYS;
	return CListView::PreCreateWindow(cs);
}

void CADHistDigitView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	CListView::OnUpdate(pSender, lHint, pHint);
	
}

#include "resource.h"
void CADHistDigitView::OnContextMenu(CWnd*, CPoint point)
{

	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_HisMouseR));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
}

void CADHistDigitView::OnSixteenShow() 
{
	// TODO: Add your command handler code here
	//AfxMessageBox(L"十六进制");	
	nChooseMode=2;
	SetListView();//刷新列表
}

void CADHistDigitView::OnUpdateSixteenShow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(nChooseMode==2);
}

void CADHistDigitView::OnDigitalVoltShowMode() 
{
	// TODO: Add your command handler code here
	//AfxMessageBox(L"电压显示");
	nChooseMode=3;
	SetListView();//刷新列表
}

void CADHistDigitView::OnUpdateDigitalVoltShowMode(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(nChooseMode==3);
}

void CADHistDigitView::OnTenShow() 
{
	// TODO: Add your command handler code here
	//AfxMessageBox(L"十进制显示");
	nChooseMode=1;//刷新列表
	SetListView();//刷新列表
}

void CADHistDigitView::OnUpdateTenShow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(nChooseMode==1);
}

void CADHistDigitView::OnRefurbish() //刷新列表
{
	// TODO: Add your command handler code here
	SetListView();//刷新列表

}

void CADHistDigitView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	CADHistDoc* pDoc=(CADHistDoc*)GetDocument();
	CRect rect;	
	GetWindowRect(&rect);	
	pDoc->m_nDigitWidth=rect.right+7;
}


void CADHistDigitView::SetHotItem(int Index)
{
	GetListCtrl().SetHotItem(Index);
}

void CADHistDigitView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default


	CListView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CADHistDigitView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListView::OnVScroll(nSBCode, nPos, pScrollBar);
}

