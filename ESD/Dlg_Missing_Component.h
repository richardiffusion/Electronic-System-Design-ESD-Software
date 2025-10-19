#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlg_Missing_Component 对话框

class CDlg_Missing_Component : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Missing_Component)

public:
	CDlg_Missing_Component(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Missing_Component();

// 对话框数据
	enum { IDD = IDD_MISSING_COMPONENT };
	
	CXTPReportControl m_wndReportCtrl;
	CString csNum;

	CStringArray arrayCompID;
	CStringArray arrayCompC;
	CStringArray arrayCompP;
	CStringArray arrayCompLib;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
