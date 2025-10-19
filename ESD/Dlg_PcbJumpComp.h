#pragma once


// CDlg_PcbJumpComp 对话框

class CDlg_PcbJumpComp : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbJumpComp)

public:
	CDlg_PcbJumpComp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PcbJumpComp();

// 对话框数据
	enum { IDD = IDD_PCB_JUMP_COMP };

	CString csTitle;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csCompDesignator;
	virtual BOOL OnInitDialog();
};
