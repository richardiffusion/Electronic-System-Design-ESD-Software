#pragma once

#include "ESDDoc.h"

// CDlg_Report_CompNet �Ի���

class CDlg_Report_CompNet : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Report_CompNet)

public:
	CDlg_Report_CompNet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Report_CompNet();

// �Ի�������
	enum { IDD = IDD_REPORT_COMPNET };
	
	CXTPReportControl m_wndReportNetList;
	CXTPReportControl m_wndReportNet;
	CXTPReportControl m_wndReportComp;

	CESDDoc* pDoc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnReportItemClick(NMHDR *pNMHDR, LRESULT *pResult);
};
