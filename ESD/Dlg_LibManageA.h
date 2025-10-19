#pragma once


// CDlg_LibManageA 对话框

class CDlg_LibManageA : public CDialog
{
	DECLARE_DYNAMIC(CDlg_LibManageA)

public:
	CDlg_LibManageA(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_LibManageA();

// 对话框数据
	enum { IDD = IDD_DLG_LIBMANAGEA };

	int m_nLib;
	int m_nComp;
	CFile m_cfile;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
