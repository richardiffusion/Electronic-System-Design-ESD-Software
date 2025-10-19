#pragma once
#include "struct.h"


// CDlg_PlaceComp 对话框

class CDlg_PlaceComp : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PlaceComp)

public:
	CDlg_PlaceComp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PlaceComp();

// 对话框数据
	enum { IDD = IDD_PLACE_COMP };

	Struct_CompList m_CompList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSearch();
	CString m_csComboCompName;
	CString m_csEditDesignator;
	CString m_csEditComment;
	CString m_csComboPackage;
	int m_nComboPart;

	virtual BOOL OnInitDialog();
	void SetParam(Struct_CompList& complist);

protected:
	virtual void OnOK();

public:
	afx_msg void OnCbnSelchangeComboCompName();
	afx_msg void OnCbnSelchangeComboCompPackage();
	afx_msg void OnCbnSelchangeComboCompPartid();
	afx_msg void OnEnChangeEditCompComment();
	afx_msg void OnEnChangeEditCompPrefix();
	afx_msg void OnBnClickedButtonClear();
};
