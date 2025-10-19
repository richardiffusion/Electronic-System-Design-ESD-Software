#pragma once


// CDlg_PCB_LoadPackageByName 对话框

class CDlg_PCB_LoadPackageByName : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCB_LoadPackageByName)

public:
	CDlg_PCB_LoadPackageByName(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCB_LoadPackageByName();

// 对话框数据
	enum { IDD = IDD_PCB_LOAD_PACKAGE_BYNAME };

	CStringArray arrayCompName;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csComboName;
	virtual BOOL OnInitDialog();
};
