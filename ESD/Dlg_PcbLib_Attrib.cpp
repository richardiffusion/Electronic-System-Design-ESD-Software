// Dlg_PcbLibAttrib.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PcbLib_Attrib.h"


// CDlg_PcbLib_Attrib 对话框

IMPLEMENT_DYNAMIC(CDlg_PcbLib_Attrib, CDialog)
CDlg_PcbLib_Attrib::CDlg_PcbLib_Attrib(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PcbLib_Attrib::IDD, pParent)
	, m_csEditName(_T(""))
	, m_csEditDescription(_T(""))
{
}

CDlg_PcbLib_Attrib::~CDlg_PcbLib_Attrib()
{
}

void CDlg_PcbLib_Attrib::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_csEditName);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_csEditDescription);
}


BEGIN_MESSAGE_MAP(CDlg_PcbLib_Attrib, CDialog)
END_MESSAGE_MAP()


// CDlg_PcbLib_Attrib 消息处理程序
