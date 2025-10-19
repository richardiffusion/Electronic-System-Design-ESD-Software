// Dlg_InputText.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_InputText.h"


// CDlg_InputText 对话框

IMPLEMENT_DYNAMIC(CDlg_InputText, CDialog)
CDlg_InputText::CDlg_InputText(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_InputText::IDD, pParent)
	, m_csEditText(_T(""))
{
}

CDlg_InputText::~CDlg_InputText()
{
}

void CDlg_InputText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_csEditText);
}


BEGIN_MESSAGE_MAP(CDlg_InputText, CDialog)
END_MESSAGE_MAP()


// CDlg_InputText 消息处理程序
