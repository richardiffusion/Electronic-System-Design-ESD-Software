// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHILDFRM_H__4D2E51D1_C35B_45F1_95B6_E17ED8A32D48__INCLUDED_)
#define AFX_CHILDFRM_H__4D2E51D1_C35B_45F1_95B6_E17ED8A32D48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "sheetswnd.h"

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

	CSheetsWnd m_wndSheet;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChildFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	BOOL OnCreateClient( LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext);

// Generated message map functions
protected:
	//{{AFX_MSG(CChildFrame)

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:


	afx_msg void OnPaint();
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHILDFRM_H__4D2E51D1_C35B_45F1_95B6_E17ED8A32D48__INCLUDED_)
