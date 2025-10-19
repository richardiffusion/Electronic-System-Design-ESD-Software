#pragma once


// CDlg_PCBEditSplitPlane 对话框

class CDlg_PCBEditSplitPlane : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditSplitPlane)

public:
	CDlg_PCBEditSplitPlane(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBEditSplitPlane();

// 对话框数据
	enum { IDD = IDD_PCB_EDIT_SPLITPLANE };
	CStringArray arrayStringNet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_csComboNet;
};
