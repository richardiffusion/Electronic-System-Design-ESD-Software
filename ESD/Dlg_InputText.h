#pragma once


// CDlg_InputText 对话框

class CDlg_InputText : public CDialog
{
	DECLARE_DYNAMIC(CDlg_InputText)

public:
	CDlg_InputText(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_InputText();

// 对话框数据
	enum { IDD = IDD_INPUT_TEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditText;
};
