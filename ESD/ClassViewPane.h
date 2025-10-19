// ClassViewPane.h : header file
//
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLASSVIEWPANE_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_)
#define AFX_CLASSVIEWPANE_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CClassViewPane window

class CClassViewPane : public CWnd
{
// Construction
public:
	CClassViewPane();

// Attributes
public:
	COfficeTreeCtrl m_wndClassView;
	CImageList m_ilClassView;
	CXTPToolBar m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClassViewPane)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClassViewPane();

	// Generated message map functions
protected:
	//{{AFX_MSG(CClassViewPane)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnProjectAddexistingitem();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLASSVIEWPANE_H__59998B87_79B4_4ED1_811C_77DA0EB311DA__INCLUDED_)
