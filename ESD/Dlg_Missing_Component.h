#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlg_Missing_Component �Ի���

class CDlg_Missing_Component : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Missing_Component)

public:
	CDlg_Missing_Component(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Missing_Component();

// �Ի�������
	enum { IDD = IDD_MISSING_COMPONENT };
	
	CXTPReportControl m_wndReportCtrl;
	CString csNum;

	CStringArray arrayCompID;
	CStringArray arrayCompC;
	CStringArray arrayCompP;
	CStringArray arrayCompLib;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
