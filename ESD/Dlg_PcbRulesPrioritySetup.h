#pragma once


// CDlg_PcbRulesPrioritySetup �Ի���

class CDlg_PcbRulesPrioritySetup : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbRulesPrioritySetup)

public:
	CDlg_PcbRulesPrioritySetup(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PcbRulesPrioritySetup();

// �Ի�������
	enum { IDD = IDD_PCB_RULES_PRIORITY_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
