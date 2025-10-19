#pragma once


// CDlg_MyHelp 对话框

class CDlg_MyHelp : public CDialog
{
	DECLARE_DYNAMIC(CDlg_MyHelp)

public:
	CDlg_MyHelp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_MyHelp();

// 对话框数据
	enum { IDD = IDD_MY_HELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
