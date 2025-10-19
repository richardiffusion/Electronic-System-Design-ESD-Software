#if !defined(AFX_PAGE7_H__0536BF37_1F4B_457C_BD0D_42C581230099__INCLUDED_)
#define AFX_PAGE7_H__0536BF37_1F4B_457C_BD0D_42C581230099__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabLongPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules7 dialog

class CPage_PcbRules7 : public CXTPPropertyPage
{
	DECLARE_DYNCREATE(CPage_PcbRules7)

// Construction
public:
	CPage_PcbRules7();
	~CPage_PcbRules7();

// Dialog Data
	//{{AFX_DATA(CPage_PcbRules7)
	enum { IDD = IDD_PCB_RULES7 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage_PcbRules7)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage_PcbRules7)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE7_H__0536BF37_1F4B_457C_BD0D_42C581230099__INCLUDED_)
