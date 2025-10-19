#pragma once
#include "afxcmn.h"


// CDlg_Temp 对话框

class CDlg_Temp : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Temp)

public:
	CDlg_Temp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Temp();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEMP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_tree;
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
};
