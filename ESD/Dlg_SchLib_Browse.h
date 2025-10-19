#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlg_SchLib_Browse 对话框

class CDlg_SchLib_Browse : public CDialog
{
	DECLARE_DYNAMIC(CDlg_SchLib_Browse)

public:
	CDlg_SchLib_Browse(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_SchLib_Browse();

// 对话框数据
	enum { IDD = IDD_SCHLIB_BROWSE };
	
	BOOL first_selection;
	BOOL first_show;

	Struct_CompList m_CompList;

	CTreeCtrl m_treeLibrary;
	CListBox m_listboxCompList;
	CListBox m_listboxUserLibrary;

	BOOL m_bMatchMore;

	int m_nComboPartNo;
	CString m_csComboPackage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowComp();
	void SetPartAndPackage();
	void ShowPackage();

protected:
	virtual void OnOK();

public:
	afx_msg void OnLbnSelchangeListComp();
	afx_msg void OnLbnDblclkListComp();
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeComboPart();
	afx_msg void OnDestroy();
	afx_msg void OnTvnSelchangedTreeLibrary(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedCheckMatch();
	afx_msg void OnLbnSelchangeListUserLibrary();
	afx_msg void OnCbnSelchangeComboPackage();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
