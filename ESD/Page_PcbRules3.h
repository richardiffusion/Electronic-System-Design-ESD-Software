#if !defined(AFX_PAGE3_H__0536BF37_1F4B_457C_BD0D_42C581230099__INCLUDED_)
#define AFX_PAGE3_H__0536BF37_1F4B_457C_BD0D_42C581230099__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Bitmap_Transparent_Page.h"

// TabLongPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules3 dialog

class CPage_PcbRules3 : public CXTPPropertyPage
{
	DECLARE_DYNCREATE(CPage_PcbRules3)

// Construction
public:
	CPage_PcbRules3();
	~CPage_PcbRules3();

// Dialog Data
	//{{AFX_DATA(CPage_PcbRules3)
	enum { IDD = IDD_PCB_RULES3 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA
	
	CXTPReportControl m_wndReportCtrl;
	CXTPReportControl m_wndReportCtrl_Priority;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPage_PcbRules3)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	CBitmap_Transparent_Page m_Bitmap1;
	CBitmap_Transparent_Page m_Bitmap2;
	CBitmap_Transparent_Page m_Bitmap3;

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPage_PcbRules3)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CDocument* pDocument;
	CString m_csEditSetBack;
	int m_nComboTopology;
	int m_nComboStyle;
	afx_msg void OnCbnSelchangeComboStyle();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE3_H__0536BF37_1F4B_457C_BD0D_42C581230099__INCLUDED_)
