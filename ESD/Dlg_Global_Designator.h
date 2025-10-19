#pragma once


// CDlg_Global_Designator 对话框

class CDlg_Global_Designator : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Global_Designator)

public:
	CDlg_Global_Designator(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Global_Designator();

// 对话框数据
	enum { IDD = IDD_GLOBAL_DESIGNATOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckFont;
	BOOL m_bCheckColor;
	BOOL m_bCheckOrientation;
	BOOL m_bCheckVisible;
	BOOL m_bCheckLock;
	BOOL m_bCheckAuto;
	int m_nRadioScope;
};
