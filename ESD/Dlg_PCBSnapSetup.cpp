// Dlg_PCBSnapSetup.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBSnapSetup.h"


// CDlg_PCBSnapSetup 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBSnapSetup, CDialog)
CDlg_PCBSnapSetup::CDlg_PCBSnapSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBSnapSetup::IDD, pParent)
	, m_csEditSnap(_T(""))
{
}

CDlg_PCBSnapSetup::~CDlg_PCBSnapSetup()
{
}

void CDlg_PCBSnapSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SNAP, m_csEditSnap);
}


BEGIN_MESSAGE_MAP(CDlg_PCBSnapSetup, CDialog)
END_MESSAGE_MAP()


// CDlg_PCBSnapSetup 消息处理程序
