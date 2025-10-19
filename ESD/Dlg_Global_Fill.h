#pragma once


// CDlg_Global_Fill 对话框

class CDlg_Global_Fill : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Global_Fill)

public:
	CDlg_Global_Fill(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Global_Fill();

// 对话框数据
	enum { IDD = IDD_GLOBAL_FILL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckWidth;
	BOOL m_bCheckColor;
	BOOL m_bCheckFillColor;
	BOOL m_bCheckTransparent;
	BOOL m_bCheckSolid;
	int m_nRadioScope;
};
