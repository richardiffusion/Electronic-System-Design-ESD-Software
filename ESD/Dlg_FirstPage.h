#if !defined(AFX_FIRSTPAGE_H__66312965_48FD_11D3_9BF6_00FE0AC10000__INCLUDED_)
#define AFX_FIRSTPAGE_H__66312965_48FD_11D3_9BF6_00FE0AC10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FirstPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlg_FirstPage dialog

class CDlg_FirstPage : public CDialog
{
// Construction
public:
	CDlg_FirstPage(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CDlg_FirstPage)
	enum { IDD = IDD_FIRSTPAGE };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlg_FirstPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	CFont m_fontBold;

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlg_FirstPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	void show_text(CString& cstring);

	virtual BOOL OnInitDialog();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FIRSTPAGE_H__66312965_48FD_11D3_9BF6_00FE0AC10000__INCLUDED_)
