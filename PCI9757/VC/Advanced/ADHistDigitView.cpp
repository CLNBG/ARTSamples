// HistDigitView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"

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
	m_ChooseMode = 3;
}

CADHistDigitView::~CADHistDigitView()
{
}


BEGIN_MESSAGE_MAP(CADHistDigitView, CListView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CADHistDigitView)
	ON_COMMAND(IDM_ShowHex, OnShowHex)
	ON_UPDATE_COMMAND_UI(IDM_ShowHex, OnUpdateShowHex)
	ON_COMMAND(IDM_ShowVolt, OnShowVolt)
	ON_UPDATE_COMMAND_UI(IDM_ShowVolt, OnUpdateShowVolt)
	ON_COMMAND(IDM_ShowDec, OnShowDec)
	ON_UPDATE_COMMAND_UI(IDM_ShowDec, OnUpdateShowDec)
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CADHistDigitView drawing

void CADHistDigitView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	
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
	// TODO: Add your specialized code here and/or call the base class
	int Channel = 0, TChannel = 0;
	int ChannelCount = 0;	

	DWORD type;
	type= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;	
	// �ı��б���ͼ�ķ��������Ϊ��ʾ����
	GetListCtrl().SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LPARAM(type));	

	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	pDoc->m_hWndDigit = m_hWnd; 

	CString str;
	CListCtrl& List = GetListCtrl();
	
// 	long VoltTopRange, VoltBottomRange;
// 	VoltTopRange = pDoc->m_Header.VoltTopRange;       // ��ѹ�����ֵ
// 	VoltBottomRange = pDoc->m_Header.VoltBottomRange; // ��ѹ����Сֵ
	for (Channel=0; Channel<MAX_AD_CHANNELS; Channel++)
	{
		if (pDoc->m_Header.ADPara.bChannelArray[Channel] == TRUE)
		{
			m_PerLsbVolt[Channel] = (float)((pDoc->m_Header.VoltTopRange[Channel] - pDoc->m_Header.VoltBottomRange[Channel]) / AD_LSB_COUNT);
		}
	}

	CADHistFrm* pADHistFrm = (CADHistFrm*)GetParentFrame();
	CADHistWaveView* pADHistWaveView = (CADHistWaveView*)pADHistFrm->GetActiveView();
	CListBox* pList = (CListBox*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_FILESINFO));
	for (Channel = 0; Channel < MAX_CHANNEL_COUNT; Channel++)
	{
		if (pDoc->m_Header.ADPara.bChannelArray[Channel] == TRUE)
		{
			str.Format(_T("CH%d"), Channel); // ȡ��ͷ��Ϣ�����ͨ����
			List.InsertColumn(TChannel, str, LVCFMT_LEFT, 75, -1);
			TChannel++;
		}
	}
	ChannelCount = TChannel;
		
	switch(pDoc->m_Header.BusType)
	{ //1:PCI, 2:USB, 3:ISA, 4:PC104, 5:PXI
		case PCI_BUS:
			str = "BusType:PCI";
			break;
		case USB_BUS:
			str = "BusType:USB";
			break;
		case ISA_BUS:
			str = "BusType:ISA";
			break;
		case PC104_BUS:
			str = "BusType:PC104";
		case PXI_BUS:
			str = "BusType:CPCI";
			break;
	} // �忨����	

	pList->AddString(str);
	str.Format(_T("DeviceNum:%x") , pDoc->m_Header.DeviceNum);
	pList->AddString(str);
	str.Format(_T("HeadSizeBytes:%d") , pDoc->m_Header.HeadSizeBytes);
	pList->AddString(str);	
	str.Format(_T("����Ƶ��:%dKHz") , pDoc->m_Header.ADPara.Frequency/1000);
	pList->AddString (str);	// ����Ƶ�� 

	switch (pDoc->m_Header.ADPara.TriggerMode)
	{
	case 0:
		str.Format(_T("����ģʽ: ����ڴ���"));
		break;
	case 1:
		str.Format(_T("����ģʽ: Ӳ���ⴥ��(�󴥷�)"));
		break;
	}
	pList->AddString (str); // ������ʽ
	
	switch (pDoc->m_Header.ADPara.TriggerSource)
	{
	case 0:
		str.Format(_T("����Դ: ATR�ⴥ��"));
		break;
	case 1:
		str.Format(_T("����Դ: DTR�ⴥ��"));
		break;
	case 2:
		str.Format(_T("����Դ: CONVST_IN"));
		break;
	}
	pList->AddString(str); // ����Դ

	switch (pDoc->m_Header.ADPara.TriggerType)
	{
	case 0:
		str.Format(_T("��������: ���ش���"));
		break;
	case 1:
		str.Format(_T("��������: ���崥��(��ƽ)"));
		break;
	}
	pList->AddString(str); // ��������

	switch (pDoc->m_Header.ADPara.TriggerDir)
	{
	case 0:
		str.Format(_T("��������: ���򴥷�"));
		break;
	case 1:
		str.Format(_T("��������: ���򴥷�"));
		break;
	case 2:
		str.Format(_T("��������: �����򴥷�"));
		break;
	}
	pList->AddString(str); // ��������

	switch (pDoc->m_Header.ADPara.ClockSource)
	{
	case 0:
		str.Format(_T("ʱ��Դ: �ڲ�ʱ��Դ"));
		break;
	case 1:
		str.Format(_T("ʱ��Դ: �ⲿʱ��Դ"));
		break;
	}
	pList->AddString(str); // ʱ��Դ

	str.Format(_T("����λ��:%d") , pDoc->m_Header.nTriggerPos);
	pList->AddString(str); // ����λ��
	CEdit* pOffsetEdit = (CEdit*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_EDIT_Offset));	
	str.Format(_T("%d") , pDoc->m_Offset);
	pOffsetEdit->SetWindowText(str);  // ��ʾ�ļ�ƫ��

	CEdit* pLength = (CEdit*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_EDIT_FileLength));	
	str.Format(_T("%d") , pDoc->m_FileLength);
	pLength->SetWindowText(str);  // ��ʾ�ļ����ȣ��֣�
	
	CSliderCtrl* pSliderOffset = (CSliderCtrl*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_SLIDER_Offset));	
	// ֮���Գ���ͨ�����������ÿͨ������
	pSliderOffset->SetRange(0, ((pDoc->m_FileLength-pDoc->m_ReadDataSize)/ChannelCount)); // ���û�������Χ���֣�
	pSliderOffset->SetPageSize(1000);
	pSliderOffset->SetPos(0);

	CEdit* pEditPerChannel = (CEdit*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_EDIT_PerChannel));	
    str.Format(_T("%d") , pDoc->m_FileLength/ChannelCount);
	pEditPerChannel->SetWindowText(str);  // ��ʾÿͨ������

	CComboBox* pRateCombo = (CComboBox*)(pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_COMBO_Rate));	
	pRateCombo->SetCurSel(0);
	
	CStatic* pCardTypeStatic = (CStatic*)pADHistFrm->m_wndSetupBar.GetDlgItem(IDC_STATIC_CardType);
	pCardTypeStatic->SetWindowText(pDoc->m_File.GetFilePath()); // �ļ�·����

	List.InsertColumn(0, _T("Index") , LVCFMT_LEFT, 42, -1);  // �����һ��
	int Row = 0, Col = 0;
 	
	for (Row=0; Row<(int)(pDoc->m_ReadDataSize/ChannelCount); Row++)
	{
		str.Format(_T("%d") , Row);
		List.InsertItem(Row, str);  // ����к�, Ӧһ������һ������	
	} 
	str.Format(_T("���ݻط�:%s") , pDoc->m_File.GetFileName());
	pDoc->SetTitle(str); 
 	SetListView(); // ���б����������
	GetListCtrl().SetHotItem(0);
}

void CADHistDigitView::SetListView(void)
{
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	int Channel = 0;
	CListCtrl& List = GetListCtrl();
	int Col = 0, Row = 0;
	int ADLsb = 0, ADData = 0;
	float fADVolt = 0.0f;
	pDoc->ReadData();

	CString strChannel;
	int TChannel = 0;
	for (Channel = 0; Channel < MAX_CHANNEL_COUNT; Channel++)
	{
		if (pDoc->m_Header.ADPara.bChannelArray[Channel] == TRUE)
		{
			List.DeleteColumn(TChannel + 1);
			strChannel.Format(_T("CH%d") , Channel); // ȡ��ͷ��Ϣ�����ͨ����
			List.InsertColumn(TChannel + 1, strChannel, LVCFMT_LEFT, 75, -1);
			TChannel++;
		}
		
	}

	int nChannelCount = TChannel;
	List.ShowWindow(SW_HIDE);
	List.SetRedraw(FALSE);
	int index(0);
 	int start = pDoc->m_Offset - pDoc->m_Offset % nChannelCount;

	///////////////////////////	
	TCHAR strList[200];
	for (Row=0; Row<(int)(pDoc->m_ReadDataSize/nChannelCount); Row++)
	{
		_stprintf_s(strList, _T("%d") , pDoc->m_Offset+Row);
		List.SetItemText(Row, 0, strList);  // ����Index��
// 		for (Col=0; Col<nChannelCount; Col++)  
// 		{
			for (int Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
			{
				if (pDoc->m_Header.ADPara.bChannelArray[Channel] == TRUE)
				{					
					switch(m_ChooseMode)
					{
					case 1: // ʮ������ʾ
						if (!gl_bHistProcessMoveVolt)
						{
							_stprintf_s(strList, _T("%d") , ((pDoc->m_ADBuffer[index]) & MASK_MSB) - gl_HistMiddleLsb[Channel]);
						}
						else
						{
							_stprintf_s(strList, _T("%d") , ((pDoc->m_ADBuffer[index]) & MASK_MSB));
						}						
						break;

					case 2: // ʮ��������ʾ
						ADLsb = pDoc->m_ADBuffer[index]; // �ӻ������л�����յ�ԭʼ������LSB(δ���θ�2λ)
						_stprintf_s(strList, _T("%6x") , ADLsb & MASK_MSB);
						break;

					case 3:
						ADLsb = ((pDoc->m_ADBuffer[index])) & MASK_MSB; // �ӻ������л�����յ�ԭʼ������LSB
						if (!gl_bHistProcessMoveVolt)
						{
							ADData = ADLsb - gl_HistMiddleLsb[Channel];
						}
						else
						{
							
							switch(pDoc->m_Header.ADPara.InputRange[Channel])
							{
							case PCI9757_INPUT_N10000_P10000mV:
							case PCI9757_INPUT_N5000_P5000mV:
							case PCI9757_INPUT_N2500_P2500mV:								
								ADData = ADLsb - AD_LSB_HALF;
								break;
							case PCI9757_INPUT_0_P10000mV:
							case PCI9757_INPUT_0_P5000mV:							
								ADData = ADLsb;
							    break;
							default:
							    break;
							}
						}
						fADVolt = m_PerLsbVolt[Channel] * ADData;					
						_stprintf_s(strList, _T("%8.2f") , fADVolt);	
						break;					
					}
					
					
					if (index >(INT)((pDoc->m_nCount) * nChannelCount))
					{
						List.SetRedraw(TRUE);
						List.ShowWindow(SW_SHOW);
						return;	
					}
					List.SetItemText(Row, (start%nChannelCount)+1, strList);// ��Ӹ�������
					//List.SetItemText(Row, Col+1, strList);// ��Ӹ�������
					start++;
					index++;
				}
			}
		//}
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
	CListView::OnUpdate(pSender, lHint, pHint);	
}

void CADHistDigitView::OnContextMenu(CWnd*, CPoint point)
{
	// CG: This block was added by the Pop-up Menu component
	if (point.x == -1 && point.y == -1)
	{
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		
		point = rect.TopLeft();
		point.Offset(5, 5);
	}
	
	CMenu menu;
	VERIFY(menu.LoadMenu(IDR_ShowMode));
	
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
	{
		pWndPopupOwner = pWndPopupOwner->GetParent();
	}
	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
		point.x, point.y, 
		pWndPopupOwner);
}

void CADHistDigitView::OnShowHex() 
{
	// TODO: Add your command handler code here
	m_ChooseMode = 2;
	SetListView(); // ˢ���б�
}

void CADHistDigitView::OnUpdateShowHex(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_ChooseMode == 2);
}

void CADHistDigitView::OnShowVolt() 
{
	// TODO: Add your command handler code here
	m_ChooseMode = 3;
	SetListView(); // ˢ���б�
}

void CADHistDigitView::OnUpdateShowVolt(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_ChooseMode == 3);
}

void CADHistDigitView::OnShowDec() 
{
	// TODO: Add your command handler code here
	m_ChooseMode = 1; // ˢ���б�
	SetListView(); // ˢ���б�
}

void CADHistDigitView::OnUpdateShowDec(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_ChooseMode == 1);
}

void CADHistDigitView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
	CADHistDoc* pDoc = (CADHistDoc*)GetDocument();
	CRect rect;	
	GetWindowRect(&rect);	
// 	pDoc->m_nDigitWidth = rect.right+7;

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

