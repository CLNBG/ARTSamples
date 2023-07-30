// ADDoc.cpp : implementation of the CADDoc class
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CADDoc

IMPLEMENT_DYNCREATE(CADDoc, CDocument)

BEGIN_MESSAGE_MAP(CADDoc, CDocument)
//{{AFX_MSG_MAP(CADDoc)
ON_COMMAND(IDM_TileWave, OnTileWave)
ON_UPDATE_COMMAND_UI(IDM_TileWave, OnUpdateTileWave)
ON_COMMAND(IDM_SuperPoseWave, OnSuperPoseWave)
ON_UPDATE_COMMAND_UI(IDM_SuperPoseWave, OnUpdateSuperPoseWave)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()
///////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CADDoc construction/destruction

CADDoc::CADDoc()
{	
	// TODO: add one-time construction code here
	gl_bProgress = FALSE;           // һ��ʼ�����½�����
	gl_nProcMode = PROC_MODE_DIGIT; // Ĭ�Ϸ�ʽ��������ʾ
	gl_TopTriggerVolt = (int)gl_VoltRange; // �ô�����ƽ�ĳ�ֵΪ���������
	gl_TopTriggerVolt = 0;
	gl_bCreateFile = TRUE;  // һ��ʼ��Ϊ��һ�δ����ļ�����
	gl_bCloseFile = TRUE;   // û�д��ļ�ʱ����Ϊ�ļ����ر�
	gl_nChannelCount = 2;
}

CADDoc::~CADDoc()
{

}

BOOL CADDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CADDoc serialization

void CADDoc::Serialize(CArchive& ar)
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
// CADDoc diagnostics

#ifdef _DEBUG
void CADDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CADDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

void CADDoc::StartDeviceAD() 
{
	HANDLE hDevice = theApp.m_hDevice;
	////////////////////////////////////////////////////////////////////////
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		if (hDevice == INVALID_HANDLE_VALUE)
		{
			AfxMessageBox(_T("�Բ�����PCI�豸����..."), MB_ICONERROR);
			return;
		}
	}
	
	
	CADFrm* pADFrm = (CADFrm*)(theApp.m_ADFrm); // ȡ����֡����ָ��
	CButton* pStart = (CButton*)((pADFrm->m_wndSTCBar).GetDlgItem(IDM_StartDeviceAD));
	CButton* pStop = (CButton*)((pADFrm->m_wndSTCBar).GetDlgItem(IDM_StopDeviceAD));
	
	gl_pParaCfgView->EnableControlWindows(FALSE);  // ���豸��ʼ����ʱ����ֹ����Ӳ������
	gl_pADStatusView->EnableControlWindows(FALSE);
	
	
	pStart->EnableWindow(FALSE);
	pStop->EnableWindow(TRUE);
	
	int Channel = 0;
	gl_nChannelCount = 0; 
	for (Channel=0; Channel<MAX_CHANNEL_COUNT; Channel++)
	{
		gl_nChannelCount++;
	}
	
	SYSTEMTIME  time;
	int nRandNum;
	
	if (!gl_bCreateFile) // ����������ļ���������дͷ�ļ�
	{
		::GetSystemTime(&time); 
		srand(time.wMilliseconds);					  // ������
		nRandNum =  rand();							  // �����
		m_header.HeadSizeBytes = sizeof(FILE_HEADER); // ͷ��Ϣ��С
		m_header.FileType= DEFAULT_FILE_TYPE;         // �ļ�����
		m_header.BusType = DEFAULT_BUSTYPE;           // ��������
		m_header.DeviceNum = DEFAULT_DEVICE_NUM;	  // �豸ID��
		m_header.HeadVersion = 0x0600;                // ͷ�汾��: 6.0
		m_header.ADPara = ADPara;                     // Ӳ������	
		m_header.nTriggerPos = 0;                     // ������λ��
		m_header.BatCode = nRandNum<<2;				  // ���ļ�ʶ����
		m_header.HeadEndFlag = HEAD_END_FLAG;         // �ļ�ͷ������
		
		for (int nChannel=0; nChannel<MAX_AD_CHANNELS; nChannel++)
		{
			switch(gl_ADINRange)         // ��ѹ����
			{		
			case PCI8510_INPUT_N10000_P10000mV:  // ��10000mV
				m_header.VoltBottomRange[nChannel] = -10000;
				m_header.VoltTopRange[nChannel] = 10000;
				break;
				
			case PCI8510_INPUT_N5000_P5000mV:  // ��5000mV
				m_header.VoltBottomRange[nChannel] = -5000;
				m_header.VoltTopRange[nChannel] = 5000;
				break;
				
			case PCI8510_INPUT_0_P10000mV:      // 0-10000mV
				m_header.VoltBottomRange[nChannel] = 0;
				m_header.VoltTopRange[nChannel] = 10000;
				break;
				
			case PCI8510_INPUT_0_P5000mV:    // 0-5000mV
				m_header.VoltBottomRange[nChannel] = 0;
				m_header.VoltTopRange[nChannel] = 5000;
				break;
				
			default:
				ASSERT(FALSE);
				break;
			}
			
			
			
		
		}	
		// д�ļ�ͷ
			ASSERT(_T("wirte file"));
			
			DWORD dTmp;
			if (!WriteFile(gl_hFileObject, (WORD*)&m_header, m_header.HeadSizeBytes,
				&dTmp, NULL))
			{
				AfxMessageBox(L"error");
			}

	}
	
	gl_FileLenghtWords = 0; // �ļ����ȳ�ʼ��
	
	CProgressCtrl* pProgress = (CProgressCtrl*)(pADFrm->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));	
	pProgress->SetPos(0);  // �ͷ��豸ʱ���ý�����Ϊ��ֵ
	
	memset(ADBuffer, 0x00, sizeof(ADBuffer)); // ���û���������λ����
	
	if (gl_nSampleMode == NPT_MODE) // �ǿշ�ʽ
	{
		m_hReadThread = AfxBeginThread(ReadDataThread_Npt, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	}
	if (gl_nSampleMode == HALF_MODE) // ������ʽ
	{
		m_hReadThread = AfxBeginThread(ReadDataThread_Half, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	}
	if (gl_nSampleMode == DMA_MODE) // DMA��ʽ
	{
		m_hReadThread = AfxBeginThread(ReadDataThread_Dma, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	}
	
	ASSERT(m_hReadThread != NULL);	
	m_hReadThread->m_bAutoDelete = FALSE; // �Զ�ɾ���߳�
	gl_bDeviceADRun = TRUE;               // ��֮�ɼ��߳̿��Թ�����
	
	// �������ƴ����߳�
	m_hProcThread = AfxBeginThread(ProcessDataThread, NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED); 
	ASSERT(m_hProcThread != NULL);
	m_hProcThread->m_bAutoDelete = FALSE; // �Զ�ɾ�� 
	gl_bDeviceADRun = TRUE; // �����豸������־
	
	// �������ݲɼ��봰�ڻ���ͬ���¼�����
	gl_hEvent = PCI8510_CreateSystemEvent();
	::ResetEvent(gl_hEvent);
	gl_hExitEvent = PCI8510_CreateSystemEvent(); 

	CADWaveView* pWaveView = (CADWaveView*)CView::FromHandle(m_hWndWave);
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
	
	m_hProcThread->ResumeThread();		 // �������ƴ����߳�
	m_hReadThread->ResumeThread();       // �����ɼ��߳�
	gl_bDataProcessing = FALSE;
	
}

//###################  ���������̺߳��� #########################
UINT ProcessDataThread(PVOID pThreadPara)  
{
	CADFrm* pADFrm = theApp.m_ADFrm;
	CADDoc* pADDoc = theApp.m_pADDoc;
	int SegmentIndex = 0;
	ULONG WroteMB = 0;  
	int ShowCount = 0;
	gl_nDrawIndex = 0;
	int ProcessCount = 0;
	while (gl_bDeviceADRun)  // ѭ������AD����
	{ 
		do{
			if (!gl_bDeviceADRun) goto ExitProcDataThread;
		}while (WaitForSingleObject(gl_hEvent, 256) != WAIT_OBJECT_0);  

		SegmentIndex = gl_nReadIndex - 1;
		if(SegmentIndex < 0)
		{
			SegmentIndex = MAX_SEGMENT_COUNT - 1;
			
		}
		switch(gl_nProcMode)  // �ж����ݴ���ʽ
		{
		case PROC_MODE_DIGIT: // ���ִ���
			if (gl_bDataProcessing == FALSE)
			{
				ProcessCount++;
				gl_nDrawIndex = SegmentIndex; // ����������������ˢ�£������µĻ����������ţ�ʹ֮�����»�����
				::SendMessage(pADDoc->m_hWndDigit, WM_SHOW_DIGIT, NULL, NULL);
			}
			gl_bProgress = TRUE; // ʹOnDraw�����ܸ��½�����		
			break;
			
		case PROC_MODE_WAVE:  // ���δ���
			if (gl_bDataProcessing == FALSE)
			{
				gl_nDrawIndex = SegmentIndex; // ����������������ˢ�£������µĻ����������ţ�ʹ֮�����»�����
				::SendMessage(pADDoc->m_hWndWave, WM_SHOW_WAVE, NULL, NULL);
			}
			gl_bProgress = TRUE; // ʹOnDraw�����ܸ��½�����	
			break;
			
		case PROC_MODE_SAVE: // ���ݴ��̴���	
			if (!gl_bDeviceADRun) goto ExitProcDataThread;
					
			CString strFileLenght;
			WroteMB = (WroteMB+16)%1024;
			gl_FileLenghtWords += 16; // 16Kb
			pADFrm->GetProgressCtrl()->SetPos((int)WroteMB);					
			strFileLenght.Format(_T("%d  "), gl_FileLenghtWords);
			int iDotPos = 0;
			iDotPos = strFileLenght.GetLength()-3;
			while (iDotPos>0)
			{
				strFileLenght.Insert(iDotPos, ',');
				iDotPos -= 3;
			}
			
			strFileLenght += " Kb";
			pADFrm->GetFileLenghtStatic()->SetWindowText(strFileLenght);
			ShowCount++;
			break;
		} // end switch
		CString str;
		CEdit* pEditOffset = (CEdit*)pADFrm->m_wndShowStatus.GetDlgItem(IDC_EDIT_Offset);
	} // end while
	
ExitProcDataThread:

	return TRUE;
}
//##########################################################################################

void CADDoc::Public_ReleaseDevice(void) // ��MDI���ڵ���
{
	StopDeviceAD();
}

void CADDoc::StopDeviceAD() 
{
	CSysApp* pApp = (CSysApp *)AfxGetApp();
	gl_bDeviceADRun = FALSE; // ֹͣ���߳̽����������ݲɼ�	
	Sleep(20);

	CADFrm* pADFrm = ((CSysApp*)AfxGetApp())->m_ADFrm; // ȡ����֡���ھ��

	// ֹͣ��ʱ��ȡ�ô�����λ��
	m_hADParaCfgView->KillTimer(1);

	if (!MyStopDeviceAD(pApp->m_hDevice)) // �ر�AD�豸
	{
		AfxMessageBox(_T("�ر�AD�豸ʧ��!"));
		return;
	}

	gl_bCreateDevice = FALSE;  
	PCI8510_ReleaseSystemEvent(gl_hEvent); // �ͷ���Ϣ
	gl_hEvent = INVALID_HANDLE_VALUE;
	// ��ֹ�ɼ��߳�
	CProgressCtrl* pProgress = (CProgressCtrl*)(pADFrm->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));
	pProgress->SetPos(0);  // �ͷ��豸ʱ���ý�����Ϊ��ֵ
	
	// ���豸�����ͷź��������Ӳ������
	gl_pParaCfgView->EnableControlWindows(TRUE);	
	gl_pADStatusView->EnableControlWindows(TRUE);  
	gl_bCreateDevice = FALSE;			// �ͷ��豸�����豸δ����
	if (gl_nProcMode == PROC_MODE_SAVE)  // ������ݴ���ʽΪ����
	{		
		SetFilePointer(gl_hFileObject, 0, 0, FILE_BEGIN);
		m_header.bOverflow = ADStatus.bOverflow;

		DWORD dTmp;
		if (!WriteFile(gl_hFileObject, (WORD*)&m_header, m_header.HeadSizeBytes,
			&dTmp, NULL))
		{
			AfxMessageBox(L"error");
		}

		OnCloseFile();					// �ͷ��ļ�����
		gl_nProcMode = PROC_MODE_DIGIT; // �ó�������ʾ��ʽ			
		CButton* pDataSave = (CButton*)(gl_pADStatusView->GetDlgItem(IDM_DataSave));	
		pDataSave->EnableWindow(FALSE);  // ʹ���̷�ʽ��ѡ����Ч
	}
}

void CADDoc::OnDataFileNew() 
{
	int Channel = 0;
	CString strNewFileName;
	CString TempFileName;
	HANDLE hDevice = theApp.m_hDevice;

	if (!(theApp.DoPromptFileName(strNewFileName, IDS_NEW_FILE, 
		OFN_HIDEREADONLY | OFN_CREATEPROMPT, TRUE, NULL)))
		return;
 
	if (!gl_bCreateFile)
	{
		if (!CloseHandle(gl_hFileObject))
		{
			AfxMessageBox(_T("�ͷ��ļ�����ʧ��..."), MB_ICONSTOP, 0);
		}
	}
	
	// �������ļ�����
	int FileNameLength = strNewFileName.GetLength();
	TempFileName = strNewFileName.Left(FileNameLength-4);

	strNewFileName = TempFileName + ".pci";
	
	//gl_hFileObject = CreateFile(strNewFileName, PCI8510_modeCreate|PCI8510_modeWrite);

	gl_hFileObject = CreateFile(strNewFileName, GENERIC_READ|GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	
	if (gl_hFileObject == INVALID_HANDLE_VALUE)		
	{
		AfxMessageBox(_T("�����ļ�����ʧ��..."), MB_ICONSTOP, 0);
		return;
	}	
	g_strFileFullName = strNewFileName;

	CADFrm* pADFrm = theApp.m_ADFrm;	// ȡ����֡���ھ��
	pADFrm->ShowHideDlgSave ();			// ��ʾ������
	pADFrm->ShowHideStatus(FALSE);		// ����״̬��
	
	CButton* pDataSave = (CButton*)(gl_pADStatusView->GetDlgItem(IDM_DataSave));	
	pDataSave->EnableWindow(TRUE);		// ʹ���̷�ʽ��ѡ����Ч
	
	//ULONGLONG DiskFreeBytes = PCI8510_GetDiskFreeBytes(strNewFileName.Left(3));  // ȡ���ļ����ڴ��̿��ÿռ�
	ULARGE_INTEGER  uLarge;
	GetDiskFreeSpaceEx(strNewFileName.Left(3),NULL,NULL,&uLarge);  // ȡ���ļ����ڴ��̿��ÿռ�
	ULONGLONG DiskFreeBytes=uLarge.QuadPart;

	// ȡ���ļ����ڴ��̿��ÿռ�
	m_RemainMB = (ULONG)(DiskFreeBytes/(1024*1024));  // ����ж���M��	
	CString str;
	
	if (m_RemainMB<2) 
	{
		AfxMessageBox(_T("Ӳ��ʣ��ռ䲻��2M�����̲������ܽ���..."), MB_ICONSTOP, 0);
		return;
	}

	str.Format(_T("%d Mb"), m_RemainMB);
	pADFrm->GetDiskFreeStatic()->SetWindowText(str); // �õ�ǰӲ�̿��ÿռ�MB
	
	pADFrm->GetProgressCtrl()->SetRange32(0, 1024);  // ���ý��̷�Χ
	pADFrm->GetProgressCtrl()->SetPos(0);
	
	/////////////
	pADFrm->GetFileNameStatic()->SetWindowText(strNewFileName); // ��ʾ�����ļ���
	gl_bCreateFile = FALSE;  // ���ɹ��ļ��������ñ�־	
	
	gl_bCloseFile = FALSE;	 // ���ļ�û�йر�, ���Բ������رյ�ǰ�ļ����˵�
	gl_pADStatusView->SetDlgCheck();
	
	gl_nProcMode = PROC_MODE_SAVE; // �������ļ�ʱ����ζ��Ҫ���д��̴���
	gl_pADStatusView->SetDlgCheck();	
}

void CADDoc::OnCloseFile() 
{
	CloseHandle(gl_hFileObject);
	
	this->SetPathName(g_strFileFullName);
	
	SetTitle(_T("AD���ɲ���"));

	gl_bCloseFile = TRUE;  // �ļ����ر�
	gl_nProcMode = PROC_MODE_DIGIT; // ���ر����ļ�ʱ����ζ��Ҫ�����ٽ��д��̴�������ó����ַ�ʽ
	gl_bCreateFile = TRUE; // �����ļ����رպ���ζ���Ժ����ļ����ǵ�һ�α����� 
	///////
	CADFrm* pADFrm = ((CSysApp*)AfxGetApp())->m_ADFrm; // ȡ����֡���ھ��
	CButton* pDataSave = (CButton*)(gl_pADStatusView->GetDlgItem(IDM_DataSave));	
	pDataSave->EnableWindow(TRUE);  // ʹ���̷�ʽ��ѡ����Ч
	
	CString str;
	str.Format(_T("%d"), 0);
	pADFrm->GetDiskFreeStatic()->SetWindowText(str);
	pADFrm->ShowHideDlgSave (FALSE);	// ���ش�����
	pADFrm->ShowHideStatus();			// ��ʾ״̬��
	pADFrm->GetProgressCtrl()->SetRange32(0, 1024);  // ���ý��̷�Χ
	pADFrm->GetProgressCtrl()->SetPos(0);	
	// һ�׽���
	gl_pADStatusView->SetDlgCheck();
	
}

void CADDoc::OnTileWave() 
{
	gl_bTileWave = TRUE;
	CButton* pButtonTile = (CButton*)gl_pADStatusView->GetDlgItem(IDM_TileWave);
	pButtonTile->SetCheck(1);
	CButton* pButtonPose = (CButton*)gl_pADStatusView->GetDlgItem(IDM_SuperPoseWave);
	pButtonPose->SetCheck(0);
    CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
	
}

void CADDoc::OnSuperPoseWave() 
{
	gl_bTileWave = FALSE;
	CButton* pButtonTile = (CButton*)gl_pADStatusView->GetDlgItem(IDM_TileWave);
	pButtonTile->SetCheck(0);
	CButton* pButtonPose = (CButton*)gl_pADStatusView->GetDlgItem(IDM_SuperPoseWave);
	pButtonPose->SetCheck(1);
    CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
	
}

void CADDoc::OnUpdateSuperPoseWave(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(gl_bTileWave == TRUE ? 0:1);
}

void CADDoc::OnUpdateTileWave(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(gl_bTileWave == TRUE ? 1:0);
	
}
