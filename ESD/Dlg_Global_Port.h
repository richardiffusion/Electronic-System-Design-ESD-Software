#pragma once


// CDlg_Global_Port 对话框

class CDlg_Global_Port : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Global_Port)

public:
	CDlg_Global_Port(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Global_Port();

// 对话框数据
	enum { IDD = IDD_GLOBAL_PORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckTextColor;
	BOOL m_bCheckBorderColor;
	BOOL m_bCheckFillColor;
	BOOL m_bCheckStyle;
	BOOL m_bCheckAlignment;
	BOOL m_bCheckIO;
	int m_nRadioScope;
};
