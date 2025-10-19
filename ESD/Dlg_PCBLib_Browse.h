#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlg_PcbLib_Browse 对话框

class CDlg_PcbLib_Browse : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbLib_Browse)

public:
	CDlg_PcbLib_Browse(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PcbLib_Browse();

// 对话框数据
	enum { IDD = IDD_PCBLIB_BROWSE };
	
	//BOOL first_selection;
	//BOOL first_show;

	Struct_CompList m_CompList;

	CTreeCtrl m_treeLibrary;
	CListBox m_listboxCompList;
	CListBox m_listboxUserLibrary;

	BOOL m_bMatchMore;

	//下面变量记录part选项和package选项, 需要自己更新和设置
	int m_nComboPartNo;
	CString m_csComboPackage;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ShowPackage();

protected:
	virtual void OnOK();

public:
	afx_msg void OnLbnSelchangeListComp();
	afx_msg void OnLbnDblclkListComp();
	afx_msg void OnPaint();

	afx_msg void OnDestroy();
	afx_msg void OnTvnSelchangedTreeLibrary(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedCheckMatch();
	afx_msg void OnLbnSelchangeListUserLibrary();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
