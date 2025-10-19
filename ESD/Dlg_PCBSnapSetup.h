#pragma once


// CDlg_PCBSnapSetup 对话框

class CDlg_PCBSnapSetup : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBSnapSetup)

public:
	CDlg_PCBSnapSetup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBSnapSetup();

// 对话框数据
	enum { IDD = IDD_PCB_SNAPSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditSnap;
};
