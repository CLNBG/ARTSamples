// Sys.h : main header file for the SYS application
//
#include "ADDoc.h"
#include "ADHistDoc.h"
#include "ADHistFrm.h"
#include "ADFrm.h"



#if !defined(AFX_SYS_H__D485DEFD_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
#define AFX_SYS_H__D485DEFD_85AB_11D1_B878_A68EB300DD63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CSysApp:
// See Sys.cpp for the implementation of this class
//
class CSysApp : public CWinApp
{
public:
	CSysApp();

public:
	// ���������������ڿ����Ӵ����Ψһ��
	BOOL m_bCreateADFrm;  
	BOOL m_bCreateDIOFrm;
	BOOL m_bCreateDAFrm;
	BOOL m_bCreatePLSFrm;

	HANDLE m_hDevice;  // InitInstance�б�����, ��ExitInstance���ͷ�
	BOOL m_bCreateDevice;
	HWND m_hParaCfgView;

public:
	CADFrm* m_ADFrm;	// �������ݲɼ���֡����
	CADHistFrm* m_HistDataFrm;
	CMultiDocTemplate* pADTemplate;
	CMultiDocTemplate* pHistDataTemplate;

	CADDoc* m_pADDigitListDoc;
	CADDoc* m_pADDoc;
	CADHistDoc* m_pADHistDoc;

	HANDLE m_hMutex;
	CString m_strFilePath; // �򿪲ɼ��ļ�ʱ�������ļ�·��
public:
	int m_CurrentDeviceID;  // ��¼��ǰ�豸ID��
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSysApp)
	afx_msg void OnAppAbout();
	afx_msg void OnUpdateCloseFile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDataFileNew(CCmdUI* pCmdUI);
	afx_msg void OnCloseFile();
	afx_msg void OnCollectData();
	afx_msg void OnDataFileNew();
	afx_msg void OnStartDeviceAD();
	afx_msg void OnStopDeviceAD();
	afx_msg void OnUpdateStopDeviceAD(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOpenIo(CCmdUI* pCmdUI);
	afx_msg void OnOpenAD();
	afx_msg void OnUpdateOpenAD(CCmdUI* pCmdUI);
	afx_msg void OnOpenADHist();
	afx_msg void OnUpdateADHist(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStartDeviceAD(CCmdUI* pCmdUI);
	afx_msg void OnOpenDIO();
	afx_msg void OnUpdateOpenDIO(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDataSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDigitalShow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGraphicShow(CCmdUI* pCmdUI);
	afx_msg void OnMDataSave();
	afx_msg void OnMDigitalShow();
	afx_msg void OnMGraphicShow();
	afx_msg void OnListDeviceDlg();
	afx_msg void OnDmaMode();
	afx_msg void OnUpdateDmaMode(CCmdUI* pCmdUI);
	afx_msg void OnNptMode();
 	afx_msg void OnUpdateNptMode(CCmdUI* pCmdUI);
	afx_msg void OnHalfMode();
 	afx_msg void OnUpdateHalfMode(CCmdUI* pCmdUI);
	afx_msg void OnINTMode();
	afx_msg void OnUpdateINTMode(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
private:
	afx_msg void OnADCalibration();
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYS_H__D485DEFD_85AB_11D1_B878_A68EB300DD63__INCLUDED_)
