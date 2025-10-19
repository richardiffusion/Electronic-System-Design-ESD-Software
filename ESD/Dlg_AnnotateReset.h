#pragma once


// CDlg_AnnotateReset 对话框

class CDlg_AnnotateReset : public CDialog
{
	DECLARE_DYNAMIC(CDlg_AnnotateReset)

public:
	CDlg_AnnotateReset(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_AnnotateReset();

// 对话框数据
	enum { IDD = IDD_ANNOTATE_RESET };
	int m_nRadioScope;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
