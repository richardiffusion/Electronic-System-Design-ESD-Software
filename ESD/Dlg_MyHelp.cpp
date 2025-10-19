// Dlg_MyHelp.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_MyHelp.h"


// CDlg_MyHelp 对话框

IMPLEMENT_DYNAMIC(CDlg_MyHelp, CDialog)
CDlg_MyHelp::CDlg_MyHelp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_MyHelp::IDD, pParent)
{
}

CDlg_MyHelp::~CDlg_MyHelp()
{
}

void CDlg_MyHelp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_MyHelp, CDialog)
END_MESSAGE_MAP()


// CDlg_MyHelp 消息处理程序
