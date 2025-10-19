#pragma once


// CDlg_Global_Power 对话框

class CDlg_Global_Power : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Global_Power)

public:
	CDlg_Global_Power(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Global_Power();

// 对话框数据
	enum { IDD = IDD_GLOBAL_POWER };
	int bIsCrossSheetConnector;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckStyle;
	BOOL m_bCheckColor;
	BOOL m_bCheckOrientation;
	BOOL m_bCheckVisible;
	int m_nRadioScope;
	virtual BOOL OnInitDialog();
};
