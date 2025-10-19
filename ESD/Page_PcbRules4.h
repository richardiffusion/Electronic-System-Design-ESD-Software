#if !defined(AFX_PAGE4_H__0536BF37_1F4B_457C_BD0D_42C581230099__INCLUDED_)
#define AFX_PAGE4_H__0536BF37_1F4B_457C_BD0D_42C581230099__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Bitmap_Transparent_Page.h"

// TabLongPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules4 dialog

class CPage_PcbRules4 : public CXTPPropertyPage
{
	DECLARE_DYNCREATE(CPage_PcbRules4)

// Construction
public:
	CPage_PcbRules4();
	~CPage_PcbRules4();

// Dialog Data
	//{{AFX_DATA(CPage_PcbRules4)
	enum { IDD = IDD_PCB_RULES4 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage_PcbRules4)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	CBitmap_Transparent_Page m_Bitmap1;
	CBitmap_Transparent_Page m_Bitmap2;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage_PcbRules4)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CString m_csEditSolder;
	CString m_csEditPaste;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE4_H__0536BF37_1F4B_457C_BD0D_42C581230099__INCLUDED_)
