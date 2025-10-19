#pragma once


// CDlg_Edit_JumpComp 对话框

class CDlg_Edit_JumpComp : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Edit_JumpComp)

public:
	CDlg_Edit_JumpComp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Edit_JumpComp();

// 对话框数据
	enum { IDD = IDD_EDIT_JUMP_COMPONENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditText;
	int m_nComboScope;
};
