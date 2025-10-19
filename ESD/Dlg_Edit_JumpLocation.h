#pragma once


// CDlg_Edit_JumpLocation 对话框

class CDlg_Edit_JumpLocation : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Edit_JumpLocation)

public:
	CDlg_Edit_JumpLocation(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Edit_JumpLocation();

// 对话框数据
	enum { IDD = IDD_EDIT_JUMP_LOCATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CString m_csEditX;
	CString m_csEditY;
};
