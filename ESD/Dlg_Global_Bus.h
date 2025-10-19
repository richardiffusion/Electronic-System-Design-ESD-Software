#pragma once


// CDlg_Global_Bus 对话框

class CDlg_Global_Bus : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Global_Bus)

public:
	CDlg_Global_Bus(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Global_Bus();

// 对话框数据
	enum { IDD = IDD_GLOBAL_BUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckWidth;
	BOOL m_bCheckColor;
	int m_nRadioScope;
};
