#pragma once

#include "ESDDoc.h"

// CDlg_Report_CompNet 对话框

class CDlg_Report_CompNet : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Report_CompNet)

public:
	CDlg_Report_CompNet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Report_CompNet();

// 对话框数据
	enum { IDD = IDD_REPORT_COMPNET };
	
	CXTPReportControl m_wndReportNetList;
	CXTPReportControl m_wndReportNet;
	CXTPReportControl m_wndReportComp;

	CESDDoc* pDoc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnReportItemClick(NMHDR *pNMHDR, LRESULT *pResult);
};
