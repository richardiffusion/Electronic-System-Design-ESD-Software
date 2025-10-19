#pragma once
#include "struct.h"

// CDlg_PlacePackage 对话框

class CDlg_PlacePackage : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PlacePackage)

public:
	CDlg_PlacePackage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PlacePackage();

// 对话框数据
	enum { IDD = IDD_PLACE_PACKAGE };
	
	Struct_CompList m_PackageList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonSearch();
	CString m_csComboCompName;
	CString m_csEditDesignator;
	CString m_csEditComment;

protected:
	virtual void OnOK();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboCompName();
	afx_msg void OnEnChangeEditCompComment();
	afx_msg void OnEnChangeEditCompPrefix();
	afx_msg void OnBnClickedButtonClear();
};
