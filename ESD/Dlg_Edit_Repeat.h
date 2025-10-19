#pragma once


// CDlg_Edit_Repeat 对话框

class CDlg_Edit_Repeat : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Edit_Repeat)

public:
	CDlg_Edit_Repeat(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Edit_Repeat();

// 对话框数据
	enum { IDD = IDD_EDIT_REPEAT };
	CString m_csTitleString;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nEditNumber;
	int m_nEditNDelta;
	CString m_csEditXDelta;
	CString m_csEditYDelta;
	virtual BOOL OnInitDialog();
protected:

};
