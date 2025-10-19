#if !defined(AFX_GFXINEDIT_H__31F5CEAD_E5CA_11D1_86E7_000000000000__INCLUDED_)
#define AFX_GFXINEDIT_H__31F5CEAD_E5CA_11D1_86E7_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// GfxInEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInEdit window

class CInEdit : public CRichEditCtrl//CEdit
{
// Construction
public:
	CInEdit(int iItem, int iSubItem, CString sInitText, bool _bMouseSelect = true);

// Attributes
public:
	bool bMouseSelect;

// Operations
public:

private:
	int m_iItem;
	int m_iSubItem;
	CString m_sInitText;
	BOOL m_bESC;// To indicate whether ESC key was pressed

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInEdit)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void ProcessSelect();
	virtual ~CInEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	//}}AFX_MSG


	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GFXINEDIT_H__31F5CEAD_E5CA_11D1_86E7_000000000000__INCLUDED_)
