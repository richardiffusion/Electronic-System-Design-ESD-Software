#pragma once


// CDlg_Global_Text 对话框

class CDlg_Global_Text : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Global_Text)

public:
	CDlg_Global_Text(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Global_Text();

// 对话框数据
	enum { IDD = IDD_GLOBAL_TEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckFont;
	BOOL m_bCheckColor;
	BOOL m_bCheckOrientation;
	int m_nRadioScope;
};
