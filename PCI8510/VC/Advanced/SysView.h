// SysView.h : interface of the CSysView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSVIEW_H__029D7400_B11F_48EB_8FE4_CE07174013E7__INCLUDED_)
#define AFX_SYSVIEW_H__029D7400_B11F_48EB_8FE4_CE07174013E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSysView : public CView
{
protected: // create from serialization only
	CSysView();
	DECLARE_DYNCREATE(CSysView)

// Attributes
public:
	CSysDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSysView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSysView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SysView.cpp
inline CSysDoc* CSysView::GetDocument()
   { return (CSysDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSVIEW_H__029D7400_B11F_48EB_8FE4_CE07174013E7__INCLUDED_)
