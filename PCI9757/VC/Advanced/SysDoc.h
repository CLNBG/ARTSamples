// SysDoc.h : interface of the CSysDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSDOC_H__26173768_F668_486F_B5B7_44059C74221D__INCLUDED_)
#define AFX_SYSDOC_H__26173768_F668_486F_B5B7_44059C74221D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSysDoc : public CDocument
{
protected: // create from serialization only
	CSysDoc();
	DECLARE_DYNCREATE(CSysDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSysDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSysDoc)

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSDOC_H__26173768_F668_486F_B5B7_44059C74221D__INCLUDED_)
