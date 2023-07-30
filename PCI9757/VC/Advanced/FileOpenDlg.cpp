// FileOpenDlg.cpp : implementation file
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileOpenDlg dialog

CFileOpenDlg::CFileOpenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFileOpenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileOpenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nBatCode = 0;
}

void CFileOpenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileOpenDlg)
	DDX_Control(pDX, IDC_EDIT_FilePath, m_Edit_Path);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileOpenDlg, CDialog)
	//{{AFX_MSG_MAP(CFileOpenDlg)
	ON_BN_CLICKED(IDC_BUTTON_FileSel0, OnBUTTONFileSel0)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileOpenDlg message handlers

void CFileOpenDlg::OnOK() 
{
	// TODO: Add extra validation here
	CSysApp* pApp = (CSysApp*)AfxGetApp();
	CButton* pButton;
	SetWindowText(_T("���ڴ��ļ�....."));
	// ���ı�����ȡ���ļ�·��
	m_Edit_Path.GetWindowText(pApp->m_strFilePath);

	// ��ֹ������ť
	pButton	= (CButton*)GetDlgItem(IDOK);
	pButton->EnableWindow(FALSE);
	pButton = (CButton*)GetDlgItem(IDCANCEL);
	pButton->EnableWindow(FALSE);
	CDialog::OnOK();
}

BOOL CFileOpenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CSysApp* pApp = (CSysApp*)AfxGetApp();

	for (int Channel=0; Channel<MAX_AD_CHANNELS; Channel++)
	{
		if (pApp->m_strFilePath != "")
		InintFilePath(pApp->m_strFilePath); // �ô�·������ʼ������·��
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFileOpenDlg::OnBUTTONFileSel0() 
{
	// TODO: Add your control notification handler code here
	CString strFilePath;
	strFilePath = SelFilePath();  // ѡ���ļ�·��
	ReadFileHead(strFilePath, 0); // ���ļ�ͷ, �����ļ�·��
}


// �����ļ��򿪶Ի���ѡ���ļ�·��
CString CFileOpenDlg::SelFilePath() 
{
	CString strFileName;
	if (!(theApp.DoPromptFileName(strFileName, IDS_ADHist, 
		OFN_HIDEREADONLY, TRUE, NULL))) 
		return "";
	
	return strFileName;
}

// ���ļ�ͷ
BOOL CFileOpenDlg::ReadFileHead(CString strFilepath, int Channel)
{
	CString str;
	CFile file;
	FILE_HEADER FileHeader;  // �����ļ�ͷ��Ϣ
	if (!file.Open(strFilepath, CFile::modeRead)) // ���ļ�
		return FALSE;

	file.Seek(0, CFile::begin);
	file.Read((WORD*)(&FileHeader), sizeof(FileHeader)); // ��ȡ�ļ�ͷ
	file.Close(); // �ر��ļ�
	if (FileHeader.DeviceNum != DEFAULT_DEVICE_NUM)
	{
		AfxMessageBox(_T("���ļ�����PCI9757�忨�����ļ���������ѡ��!"));
		return FALSE;
	}

	if (FileHeader.BatCode != m_nBatCode) // �������ͬһ�������ļ�
	{
		AfxMessageBox(_T("���ļ�������ͨ������ͬ���ļ���������ѡ��!"));
		return FALSE;
	}

	m_Edit_Path.SetWindowText(strFilepath); // ����·��
	return TRUE;
}

// ��ʼ���ļ�·�����ı�����
void CFileOpenDlg::InintFilePath(CString strPath)
{
	CFile file;
	FILE_HEADER FileHeader;  // �����ļ�ͷ��Ϣ
	CString strTail, strBase, strWhole, str;
	if (!file.Open(strPath, CFile::modeRead)) // ���ļ�
	{
		AfxMessageBox(_T("�ļ��򿪴�����޴��ļ�:")+strPath);
		return;
	}

	file.Seek(0, CFile::begin);
	file.Read((WORD*)&FileHeader, sizeof(FileHeader)); // ��ȡ�ļ�ͷ
	file.Close(); // �ر��ļ�
	m_nBatCode = FileHeader.BatCode;     // ͬ���ļ���
	strBase = strPath.Left(strPath.GetLength() - 5); // ȡ�û����ļ���
	m_Edit_Path.SetWindowText(strPath);
	strTail.Format(_T(".pci") , NULL);
	strWhole = strBase + strTail;
	if (!file.Open(strWhole, CFile::modeRead)) // ���ļ�strWhole
	{
		AfxMessageBox(_T("δ�����������ļ�"));
		m_Edit_Path.SetWindowText(_T(""));
	}
	
	file.Seek(0, CFile::begin);
	file.Read((WORD*)&FileHeader, sizeof(FileHeader)); // ��ȡ�ļ�ͷ
	file.Close(); // �ر��ļ�

}
