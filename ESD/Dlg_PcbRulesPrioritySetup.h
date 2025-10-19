#pragma once


// CDlg_PcbRulesPrioritySetup 对话框

class CDlg_PcbRulesPrioritySetup : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbRulesPrioritySetup)

public:
	CDlg_PcbRulesPrioritySetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PcbRulesPrioritySetup();

// 对话框数据
	enum { IDD = IDD_PCB_RULES_PRIORITY_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CDocument* pDocument;
	int m_nEditPriority;
	CString m_csComboName;
	int m_nComboType;
	void GetNet();
	void GetComp();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboType();
};
