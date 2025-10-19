// Dlg_SchLib_Attrib.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_SchLib_Attrib.h"
#include ".\dlg_schlib_attrib.h"


// CDlg_SchLib_Attrib 对话框

IMPLEMENT_DYNAMIC(CDlg_SchLib_Attrib, CDialog)
CDlg_SchLib_Attrib::CDlg_SchLib_Attrib(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_SchLib_Attrib::IDD, pParent)
	, m_csEditName(_T(""))
	, m_csEditDescription(_T(""))
	, m_csEditRef(_T(""))
	, m_csEditPackage(_T(""))
{
	m_bSystemLib = FALSE;
}

CDlg_SchLib_Attrib::~CDlg_SchLib_Attrib()
{
}

void CDlg_SchLib_Attrib::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_csEditName);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_csEditDescription);
	DDX_Text(pDX, IDC_EDIT_REF, m_csEditRef);
	DDX_Text(pDX, IDC_EDIT_PACKAGE, m_csEditPackage);
	DDV_MaxChars(pDX, m_csEditName, 99);
	DDV_MaxChars(pDX, m_csEditDescription, 210);
}


BEGIN_MESSAGE_MAP(CDlg_SchLib_Attrib, CDialog)
END_MESSAGE_MAP()


// CDlg_SchLib_Attrib 消息处理程序

BOOL CDlg_SchLib_Attrib::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_bSystemLib == TRUE)
	{
		CWnd* pWnd = GetDlgItem(IDC_EDIT_REF);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_PACKAGE);
		pWnd->EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
