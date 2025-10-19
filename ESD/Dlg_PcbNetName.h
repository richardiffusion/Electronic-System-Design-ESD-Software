#pragma once


// CDlg_PcbNetName 对话框

class CDlg_PcbNetName : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbNetName)

public:
	CDlg_PcbNetName(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PcbNetName();

// 对话框数据
	enum { IDD = IDD_PCB_NETNAME };
	
	CStringArray arrayString;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csComboNet;
	virtual BOOL OnInitDialog();
};
