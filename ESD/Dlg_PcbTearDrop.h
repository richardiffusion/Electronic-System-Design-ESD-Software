#pragma once


// CDlg_PcbTearDrop 对话框

class CDlg_PcbTearDrop : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbTearDrop)

public:
	CDlg_PcbTearDrop(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PcbTearDrop();

// 对话框数据
	enum { IDD = IDD_PCB_TEARDROP };

	int m_nRadioAction;
	int m_nRadioStyle;
	int m_nRadioSize;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckAllPad;
	BOOL m_bCheckAllVia;
	BOOL m_bCheckSelection;
	afx_msg void OnBnClickedRadioAction();
	afx_msg void OnBnClickedRadioAction2();
};
