#pragma once


// CDlg_Edit_FindText 对话框

class CDlg_Edit_FindText : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Edit_FindText)

public:
	CDlg_Edit_FindText(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Edit_FindText();

// 对话框数据
	enum { IDD = IDD_EDIT_FINDTEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditText;
	int m_nComboScope;
	int m_nComboObject;
	BOOL m_bCheckCase;
	BOOL m_bCheckWord;
	BOOL m_bCheckJump;
};
