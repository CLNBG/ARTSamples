// ADDoc.cpp : implementation of the CADDoc class
//

#include "stdafx.h"
#include "Sys.h"
#include "ADDoc.h"
#include "ADDigitView.h"
#include "ADWaveView.h"
#include "ADStatusView.h"
#include "ParaCfgView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#ifdef ACTS1000_CreateSystemEvent
#define ACTS1000_CreateSystemEvent ACTS1000_TCreateSystemEvent 
#endif
////////////////////////////////////////////////////////////////////////
WORD* ADBuffer[MAX_SEGMENT]; // �������
WORD* ADDrawBuffer; // ����ˢ�»���

BOOL	m_bDrawComp= FALSE;
BOOL gl_bAutoCollect = FALSE;
int CurrentIndex; // AD���ݴ����̵߳�ǰ������������
int ReadIndex;    // AD���ݲɼ��̵߳�ǰ������������
BOOL gl_bDeviceADRun=FALSE;
BOOL gl_bAutoTrig=TRUE;
ACTS1000_PARA_AD ADPara;
ACTS1000_PARA_AD OldADPara;
ACTS1000_STATUS_AD gl_ADStatus;
ACTS1000_AD_MAIN_INFO gl_ADMainInfo;
WORD gl_wMaxLSB = 0x3FFF;
float gl_fLsbCount = 16384.0;
WORD gl_lLsbHalf = 8192;
//BOOL bCheckOverflow=FALSE; // �Ƿ���FIFO�����־
int  gl_nSampleMode = PRO_MODE;            // �ɼ�ģʽ(1,��ѯ�� 2, �жϣ� 3�� DMA)
UINT ReadDataThread(PVOID hWnd);//�����߳�
UINT ProccessDataThread(PVOID hWnd);  // ���������߳�

BOOL m_bFirstCreateDeviceFile;
int  m_ProcessGraphicMode;//Ϊ1,������ʾ
BOOL m_AnalyzeCheck;//���ƾ��ȷ���
int  gl_ScreenVolume;//������Ļ��ʾ������
int  m_TriggerVolt;//������ѹ
int gl_MiddleLsb[MAX_CHANNEL_COUNT];
float m_AnalyzeAllCount;
UINT m_OverLimitCount;
int m_ProcessMoveVolt;//Ϊ1ʱ,ƽ�Ƶ�ѹ
BOOL bCreateDevice;
BOOL m_bProgress;  // �Ƿ���½�����
int nProcessMode;  // ���ݴ���ʽ 1��������ʾ  2��������ʾ  3�����ݴ���
CEdit* pCountEdit;
CEdit* pRangeEdit;
CEdit* pRatioEdit;
CADDigitView* m_pDigitView;
CADWaveView* m_pWaveView;
CADStatusView* pADStatusView = NULL;
CParaCfgView* pParaCfgView = NULL;
HANDLE hEvent;  // �ɼ��߳�������̵߳�ͬ���ź�
int m_DigitalDataShowMode; // ���ִ�����ʾģʽ
int DeviceID;   // �ó���ʹ�õ��豸ID
HANDLE g_ArrFileOut;

int gl_nChannelCount = MAX_CHANNEL_COUNT;
int gl_nSaveFileCount = MAX_CHANNEL_COUNT;

BOOL m_bCloseFile;
HANDLE hExitEvent;
float Center, PerY;
CRect ClientRect;
float middle1;
BOOL bDrawing=FALSE;

ULONGLONG FileLenghtWords;
int nChannelArrayClient[MAX_CHANNEL_COUNT];// ͨ����
int gl_CurrentbufNum = 0; // ��ǰ�Ļ���������
int DisplayChannel[MAX_CHANNEL_COUNT];// ���ڱ�־��ͨ���Ƿ���ʾ
float OutAnalogValve; // ��ʾģ���ⴥ����ȫ�ֱ���
ULONG gl_nSampleLenth = MAX_RAM_SIZE;   // ��������
int  gl_InputRange[MAX_CHANNEL_COUNT];    // ��ͨ�����õĵ�ѹ���̷�Χ
float gl_PerLsbVoltM[MAX_CHANNEL_COUNT]; // ��λLSB�ĵ�ѹֵ
CString g_strFileFullName;
ULONG gl_StartAddr;

TCHAR g_strChannelEn[2][64] = { L"��ֹ", L"����" };
TCHAR g_strGain[4][64] = { L"1���Ŵ�", L"2���Ŵ�", L"4���Ŵ�", L"8���Ŵ�" };
TCHAR g_strRefGround[3][64] = { L"ֱ��", L"����"};
TCHAR g_strAIRange[3][64] = {L"��5V", L"��1V", L"��10V"};
TCHAR g_strInputImped[2][64] = {L"1M��", L"50��"};

ULONG m_nSampRangeCount;
ULONG m_nSampRangeVal[8];

ULONG gl_CHIdx[MAX_CHANNEL_COUNT] = 
{
	0,1,2,3,4,5,6,7
};

//##############################################
//ʹ�ö�ý�嶨ʱ����ÿ��50֡�����µ��ٶ�ˢ����Ļ
#include "stdio.h"
#include "MmSystem.h"

void PASCAL DrawWindowProc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);
#define ONE_MILLI_SECOND 1
#define TWO_SECOND 2000
// �� �� ʱ �� �� �� �ʣ� �� �� �� Ϊ �� λ ---- 
#define TIMER_ACCURACY 1
UINT wAccuracy;  // �� �� �� �� ��
UINT TimerID; // �� �� �� ʱ �� �� �� 

////////////////////////////////////////////////////////////////////////////
// CADDoc

IMPLEMENT_DYNCREATE(CADDoc, CDocument)

BEGIN_MESSAGE_MAP(CADDoc, CDocument)
//{{AFX_MSG_MAP(CADDoc)
ON_COMMAND(IDM_TileWave, OnTileWave)
ON_COMMAND(IDM_SuperPoseWave, OnSuperPoseWave)
ON_UPDATE_COMMAND_UI(IDM_SuperPoseWave, OnUpdateSuperPoseWave)
ON_UPDATE_COMMAND_UI(IDM_TileWave, OnUpdateTileWave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
///////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CADDoc construction/destruction

extern CSysApp theApp;
CADDoc::CADDoc()
{
	// TODO: add one-time construction code here
	m_bProgress=FALSE; // һ��ʼ�����½�����
	nProcessMode = DATA_MODE;    // Ĭ�Ϸ�ʽ��������ʾ
	m_TriggerVolt=(int)VOLT_RANGE;	// �ô�����ƽ�ĳ�ֵΪ���������
	m_bFirstCreateDeviceFile=TRUE;  // һ��ʼ��Ϊ��һ�δ����ļ�����
	m_bCloseFile=TRUE; // û�д��ļ�ʱ����Ϊ�ļ����ر�	
	gl_nChannelCount=0;
	ACTS1000_LoadParaAD(theApp.m_hDevice, &ADPara);  // ����ϵͳ����

	for (int i=gl_ADMainInfo.nChannelCount; i<ACTS1000_AD_MAX_CHANNELS; i++)
	{
		ADPara.bChannelArray[i] = 0;
	}

	for (int nCH=0; nCH<gl_ADMainInfo.nChannelCount; nCH++)
	{
		if (ADPara.bChannelArray[nCH])
		{
			gl_nChannelCount++;
		}
	}

	//gl_nChannelCount = MAX_CHANNEL_COUNT;

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

BOOL gl_bTerminateDrawThr = TRUE;
void CADDoc::StartDeviceAD() 
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	////////////////////////////////////////////////////////////////////////
	if(pApp->m_hDevice==INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("�Բ������豸����..."),MB_ICONERROR);
		return;
	}
	if(!gl_bTerminateDrawThr)
	{
		AfxMessageBox(_T(" �߳�û����ֹ"));
		return;
	}
	FileLenghtWords = 0; // �ļ����ȳ�ʼ��
	m_DrawWindowThread = AfxBeginThread(ProccessDataThread,NULL, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_DrawWindowThread->m_bAutoDelete = TRUE; // �Զ�ɾ��    
	
	// �������ݲɼ��봰�ڻ���ͬ���¼�����
	hEvent = ACTS1000_CreateSystemEvent();
	hExitEvent = ACTS1000_CreateSystemEvent();
	bCollectData=TRUE;  // �����豸����󣬿��Խ������ݲɼ�
// 	for(int i=0;i<MAX_CHANNEL_COUNT;i++)
// 		gl_MiddleLsb[i]=0;
	
	
	if(!m_bFirstCreateDeviceFile) // ����������ļ�����
	{
		header.HeadSizeBytes = sizeof(FILE_HEADER);   // ͷ��Ϣ��С
		header.FileType = DEFAULT_FILE_TYPE;
		header.BusType = gl_ADMainInfo.nDeviceType>>16;        // ���豸��������
		header.DeviceNum = DEFAULT_DEVICE_NUM;	 
		header.DeviceType = gl_ADMainInfo.nDeviceType&0xFFFF;	 // �豸ID��
		header.ChannelCount = gl_nChannelCount;                 // ÿͨ�����Ƕ����ģ�����Ϊ1
		header.DataWidth = gl_ADMainInfo.nSampResolution;   // 12λ
		header.bXorHighBit = 0;                  // ��λ������
		header.nRevisionID = theApp.m_nRevisionID;

		ULONG nSampRangeCount;
	ULONG nSampRangeVal[8];
		if (!ACTS1000_GetSupportRangeInfo(theApp.m_hDevice, &nSampRangeCount,nSampRangeVal))
		{
			AfxMessageBox(L"��ȥ֧��������Ϣʧ��",MB_ICONWARNING,0);

			return;
		}

		// ��û������
		switch(ADPara.InputRange[0])	// ��ѹ����
		{		
		case ACTS1000_INPUT_N1000_P1000mV: // ��1000����
			header.VoltTopRange = 1000;
			header.VoltBottomRange = -1000;
			break;		
		case ACTS1000_INPUT_N10000_P10000mV: // ��5000����
			
			header.VoltTopRange = 10000;
			header.VoltBottomRange = -10000;
			break;
		case ACTS1000_INPUT_N5000_P5000mV:	
			header.VoltTopRange = 5000;
			header.VoltBottomRange = -5000;
			break;		
		}			
		header.ADPara = ADPara;
		header.CrystalFreq = gl_ADMainInfo.nBaseRate; // ����Ƶ��
		header.HeadEndFlag = HEAD_END_FLAG;
		header.ADStatus = gl_ADStatus;

		time_t nowtime;       
		time(&nowtime);
		header.CreateFTime = (ULONGLONG)nowtime;
		memcpy(&header.ADMainInfo, &gl_ADMainInfo, sizeof(gl_ADMainInfo));

		// д�ļ�ͷ
		//	g_ArrFileOut[Index].Write((WORD*)&header, header.HeadSizeBytes);
		DWORD dTmp;
		if (!WriteFile(g_ArrFileOut, (WORD*)&header, header.HeadSizeBytes,
			&dTmp, NULL))
		{
			AfxMessageBox(L"error");
		}
	}
	
	// ����ͨ����������������ͼ������Χ
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(m_hWndDigit)); 
    CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	CADFrame* pShow=((CSysApp*)AfxGetApp())->m_ADFrame ; // ȡ����֡���ھ��
	
	CProgressCtrl* pProgress=(CProgressCtrl*)(pShow->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));	
	pProgress->SetPos(0);  // �ͷ��豸ʱ���ý�����Ϊ��ֵ
	

	// �����豸
	if(!MyStartDeviceAD(pApp->m_hDevice)) // �����豸,��ʼ�ɼ�����
	{
		AfxMessageBox(_T("�豸����ʧ��"));
		return;
	}

	gl_bDeviceADRun = TRUE;
	if(pParaCfgView != NULL)
	{
		pParaCfgView->EnableWindows(FALSE); // ���豸��ʼ����ʱ����ֹ����Ӳ������
		pParaCfgView->StartAD();
	}
	m_DrawWindowThread->ResumeThread(); // �������ƴ����߳�
}

BOOL gl_Event = FALSE;
//###################  ���������߳� ################################################3### 
UINT ProccessDataThread(PVOID hWnd)  
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
	CADFrame *pFrm=pApp->m_ADFrame ;
	CADDoc* pDoc=pApp->m_pADDoc; 
	// ���߳���ȡ����ͼָ��
	CADDigitView* pDigitView=(CADDigitView*)(CWnd::FromHandle(pDoc->m_hWndDigit)); 
	CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(pDoc->m_hWndWave)); 
	CString str;
	BOOL bFirst=TRUE;    
	ULONG WroteMB=0;  
	
	pDoc->m_Wrote8KWCounter=0;
	pDoc->m_WroteMB=0;
	CurrentIndex=0;
	int ShowCount=0,i;
	ULONG ulTime = GetTickCount();
	while(gl_bDeviceADRun)  // ѭ������AD����
	{ 
		do{
			if(!gl_bDeviceADRun) goto ExitProcDataThread;
		} while(WaitForSingleObject(hEvent, 100) != WAIT_OBJECT_0);
		gl_bTerminateDrawThr = FALSE;
		gl_Event = TRUE;
		switch(nProcessMode)  // ���ݴ���
		{
		case DATA_MODE: // ���ִ���
			if ((GetTickCount()-ulTime)>100)
			{
				::SendMessage(pDoc->m_hWndDigit, WM_SHOWDIGIT, NULL, NULL);
				m_bProgress=TRUE; // ʹOnDraw�����ܸ��½�����
				ulTime = GetTickCount();							
			}
			/*else
			{
				if (gl_bSmlThenSize)
				{
					gl_bSmlThenSize = FALSE;
				}
			}*/
			break; 
		case WAVE_MODE:  // ���δ���
			if ((GetTickCount()-ulTime)>120)
			{
				::SendMessage(pDoc->m_hWndWave,WM_SHOWWAVE, NULL, NULL);
				m_bProgress=TRUE; // ʹOnDraw�����ܸ��½�����
				ulTime = GetTickCount();
				//Sleep(1);
			}
			break;
		case SAVE_MODE: // ���ݴ��̴���				
			WroteMB=(WroteMB+32)%1024;
			pFrm->m_Progress()->SetPos((int)WroteMB);					
			str.Format(_T("%d"), (FileLenghtWords/512));
			i=str.GetLength()-3;
			while(i>0)
			{
				str.Insert(i, ',');
				i-=3;
			}
			str+=" Kb";
			pFrm->m_FileLenght()->SetWindowText(str);
			ShowCount++;
			break;
		}//end switch  
		
	}//end while
ExitProcDataThread:
	gl_bTerminateDrawThr = TRUE; // �߳���ֹ
	return TRUE;
}
//##########################################################################################

void CADDoc::Public_ReleaseDevice(void) // ��MDI���ڵ���
{
	StopDeviceAD();
}

void CADDoc::StopDeviceAD() 
{
	CSysApp *pApp=(CSysApp *)AfxGetApp();
//	gl_bDeviceADRun = FALSE; // ֹͣ���߳̽����������ݲɼ�	
	
	CADFrame* pShow=((CSysApp*)AfxGetApp())->m_ADFrame ; // ȡ����֡���ھ��
	


//	bCreateDevice = FALSE;
//	SetEvent(hExitEvent);
//	WaitForSingleObject(hExitEvent, 100);  // �ȴ��ɼ��߳��˳�
	// ��ֹ�ɼ��߳�
	CProgressCtrl* pProgress=(CProgressCtrl*)(pShow->m_wndShowStatus.GetDlgItem(IDC_PROGRESS));
	pProgress->SetPos(0);  // �ͷ��豸ʱ���ý�����Ϊ��ֵ
	
	// ���豸�����ͷź��������Ӳ������
	if(pParaCfgView != NULL)
	{
		pParaCfgView->EnableWindows(TRUE);	
	}
	bCreateDevice = FALSE;     // �ͷ��豸�����豸δ����
	

	::SendMessage(m_hWndDigit, WM_SHOWDIGIT, NULL, NULL);
	::SendMessage(m_hWndWave,WM_SHOWWAVE, NULL, NULL);
	if(!MyStopDeviceAD(pApp->m_hDevice)) // �ر�AD�豸
	{
		AfxMessageBox(_T("�ر�AD�豸ʧ��!"));
		return;
	}
	if(nProcessMode == SAVE_MODE)  // ������ݴ���ʽΪ����
	{
		
		OnCloseFile();//�ͷ��ļ�
		nProcessMode = DATA_MODE; // �ó����ַ�ʽ	
		if(pADStatusView != NULL)
		{
			CButton* pDataSave=(CButton*)(pADStatusView->GetDlgItem(IDM_DataSave));	
			pDataSave->EnableWindow(FALSE);  // ʹ���̷�ʽ��ѡ����Ч

		}
	}
	ACTS1000_ReleaseSystemEvent(hEvent);	   // �ͷ���Ϣ
	ACTS1000_ReleaseSystemEvent(hExitEvent);	   // �ͷ���Ϣ

}


void CADDoc::OnDataFileNew() 
{
	// TODO: Add your command handler code here
	CString strNewFileName;
	CString TempFileName;
	CSysApp* pApp=(CSysApp*)AfxGetApp();
	if (!(pApp->DoPromptFileName(strNewFileName, IDS_NEW_FILE,
		OFN_HIDEREADONLY | OFN_CREATEPROMPT, TRUE, NULL)))
		return;
	if(!m_bFirstCreateDeviceFile)
	{  // ������ǵ�һ�δ����ļ�����,���ͷ����ļ�����	
		if ( ! CloseHandle( g_ArrFileOut ) )
		{
			AfxMessageBox(L"�ͷ��ļ�����ʧ��...",MB_ICONSTOP,0);
		}
	} 
	
	// �������ļ�����

	int FileNameLength=strNewFileName.GetLength();
	//TempFileName=strNewFileName.Left(FileNameLength-4);


	g_ArrFileOut =CreateFile(strNewFileName, GENERIC_READ|GENERIC_WRITE, 0, NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (g_ArrFileOut == INVALID_HANDLE_VALUE )		

	{
		AfxMessageBox(_T("��ʼ���ļ�����ʧ��..."),MB_ICONSTOP,0);
		return;
	}	
	// ����򿪵��ļ�����
	g_strFileFullName = strNewFileName;

	gl_nSaveFileCount = gl_nChannelCount;


	
	CADFrame* pShow=((CSysApp*)AfxGetApp())->m_ADFrame ; // ȡ����֡���ھ��
	pShow->ShowHideDlgSave ();	//��ʾ������
	pShow->ShowHideStatus(FALSE);//����״̬��
	if(pADStatusView != NULL)
	{
		CButton* pDataSave=(CButton*)(pADStatusView->GetDlgItem(IDM_DataSave));	
		pDataSave->EnableWindow(TRUE);  // ʹ���̷�ʽ��ѡ����Ч
	}
	
	_ULARGE_INTEGER dlFreeSpace, dlTotalSpace, dlUserSpace;

	GetDiskFreeSpaceEx(strNewFileName.Left(3), &dlUserSpace, &dlUserSpace, &dlFreeSpace);
	(void)dlTotalSpace;
	(void)dlUserSpace;

	ULONGLONG DiskFreeBytes = dlFreeSpace.QuadPart; 

	// ȡ���ļ����ڴ��̿��ÿռ� */
	m_RemainMB=(ULONG)(DiskFreeBytes/(1024*1024));  // ����ж���M��	
	CString str;
	
	if(m_RemainMB<2) 
	{
		AfxMessageBox(_T("Ӳ��ʣ��ռ䲻��2M�����̲������ܽ���..."),MB_ICONSTOP,0);
		return;
	}
	str.Format(_T("%d Mb"),m_RemainMB);
	pShow->m_DiskFree()->SetWindowText (str);// �õ�ǰӲ�̿��ÿռ�MB
	
	pShow->m_Progress()->SetRange32(0,1024);// ���ý��̷�Χ
	pShow->m_Progress()->SetPos(0);
	
	/////////////
	pShow->m_FileName()->SetWindowText(strNewFileName);//��ʾ�����ļ���
	m_bFirstCreateDeviceFile=FALSE;  // ���ɹ��ļ��������ñ�־	
	
	m_bCloseFile=FALSE; // ���ļ�û�йر�,���Բ������رյ�ǰ�ļ����˵�
	if(pADStatusView != NULL)
		pADStatusView->SetDlgCheck();
	nProcessMode = SAVE_MODE; // �������ļ�ʱ����ζ��Ҫ���д��̴���
	
	pADStatusView->SetDlgCheck();
}

void CADDoc::OnCloseFile() 
{
	// TODO: Add your command handler code here
	DWORD dTmp;
	
	header.HeadSizeBytes = sizeof(FILE_HEADER);
	header.ADStatus = gl_ADStatus; // gl_ADStatus.bTrigFlag;
// 		g_ArrFileOut.SeekToBegin();
// 		g_ArrFileOut.Write(&header, sizeof(FILE_HEADER));
// 		g_ArrFileOut.Close();
	SetFilePointer(g_ArrFileOut, 0, 0, FILE_BEGIN);
	//ACTS1000_WriteFile(hFileObject[nChannel], &header, sizeof(FILE_HEADER));
	if (!WriteFile(g_ArrFileOut, &header, sizeof(FILE_HEADER), &dTmp, NULL))
	{
		AfxMessageBox(L"error");
	}
	CloseHandle( g_ArrFileOut);

	CString strTitle;
	strTitle = GetTitle(); // Ϊ��ֹSetPathName�ı��ĵ���Title������ǰ����
	// ��ӵ� ���򿪵��ļ��б���





	m_bCloseFile=TRUE;  // �ļ����ر�
	nProcessMode = DATA_MODE; // ���ر����ļ�ʱ����ζ��Ҫ�����ٽ��д��̴�������ó����ַ�ʽ
	m_bFirstCreateDeviceFile=TRUE; // �����ļ����رպ���ζ���Ժ����ļ����ǵ�һ�α����� 
	///////
	CADFrame* pADFrm = ((CSysApp*)AfxGetApp())->m_ADFrame ; // ȡ����֡���ھ��
	CButton* pDataSave=(CButton*)(pADStatusView->GetDlgItem(IDM_DataSave));	
	pDataSave->EnableWindow(TRUE);  // ʹ���̷�ʽ��ѡ����Ч
	
	CString str;
	str.Format(_T("%d"), 0);
	pADFrm->m_DiskFree()->SetWindowText(str);

	if (gl_bAutoCollect)
	{
//		SetPathName(g_strFileFullName[1]);
		SetPathName(g_strFileFullName); 
		SetTitle(strTitle); // �ָ��ĵ���Title
		pADFrm->ShowHideDlgSave (FALSE); // ���ش�����
		pADFrm->ShowHideStatus();//��ʾ״̬��
	}
	     

	pADFrm->m_Progress()->SetRange32(0,1024);  // ���ý��̷�Χ
	pADFrm->m_Progress()->SetPos(0);	
	// һ�׽���
	pADStatusView->SetDlgCheck();	
}


void CADDoc::OnTileWave() 
{
	// TODO: Add your command handler code here
	gl_bTileWave = TRUE;
	CButton* pButtonTile = (CButton*)pADStatusView->GetDlgItem(IDM_TileWave);
	pButtonTile->SetCheck(1);
	CButton* pButtonPose = (CButton*)pADStatusView->GetDlgItem(IDM_SuperPoseWave);
	pButtonPose->SetCheck(0);
    CADWaveView* pWaveView=(CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();	
	pWaveView->OnDrawPolyLine();
}

void CADDoc::OnSuperPoseWave() 
{
	// TODO: Add your command handler code here
	gl_bTileWave = FALSE;
	CButton* pButtonTile = (CButton*)pADStatusView->GetDlgItem(IDM_TileWave);
	pButtonTile->SetCheck(0);
	CButton* pButtonPose = (CButton*)pADStatusView->GetDlgItem(IDM_SuperPoseWave);
	pButtonPose->SetCheck(1);
    CADWaveView* pWaveView = (CADWaveView*)(CWnd::FromHandle(m_hWndWave)); 
	pWaveView->m_ADScopeCtrl.InvalidateCtrl();
	pWaveView->OnDrawPolyLine();
	
}

void CADDoc::OnUpdateSuperPoseWave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_bTileWave==TRUE ? 0:1);
}

void CADDoc::OnUpdateTileWave(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(gl_bTileWave==TRUE ? 1:0);
	
}

