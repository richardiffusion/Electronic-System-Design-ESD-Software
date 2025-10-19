#pragma once


// CDlg_Global_Polyline 对话框

class CDlg_Global_Polyline : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Global_Polyline)

public:
	CDlg_Global_Polyline(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Global_Polyline();

// 对话框数据
	enum { IDD = IDD_GLOBAL_POLYLINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckStyle;
	BOOL m_bCheckColor;
	BOOL m_bCheckWidth;
	BOOL m_bCheckStart;
	BOOL m_bCheckEnd;
	BOOL m_bCheckSize;
	int m_nRadioScope;
};
