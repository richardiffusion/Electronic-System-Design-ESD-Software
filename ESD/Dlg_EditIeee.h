#pragma once


// CDlg_EditIeee 对话框

class CDlg_EditIeee : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditIeee)

public:
	CDlg_EditIeee(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_EditIeee();

// 对话框数据
	enum { IDD = IDD_EDIT_IEEE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
