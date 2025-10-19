#pragma once


// CDlg_Edit_ReplaceText 对话框

class CDlg_Edit_ReplaceText : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Edit_ReplaceText)

public:
	CDlg_Edit_ReplaceText(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Edit_ReplaceText();

// 对话框数据
	enum { IDD = IDD_EDIT_REPLACETEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditText;
	CString m_csEditReplaceText;
	int m_nComboScope;
	int m_nComboObject;
	BOOL m_bCheckCase;
	BOOL m_bCheckPrompt;
	BOOL m_bCheckWord;
};
