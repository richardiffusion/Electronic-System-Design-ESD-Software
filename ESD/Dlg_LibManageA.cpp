// Dlg_LibManageA.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_LibManageA.h"

// CDlg_LibManageA 对话框

IMPLEMENT_DYNAMIC(CDlg_LibManageA, CDialog)

CDlg_LibManageA::CDlg_LibManageA(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_LibManageA::IDD, pParent)
{
	m_nLib = m_nComp = 0;
}

CDlg_LibManageA::~CDlg_LibManageA()
{
}

void CDlg_LibManageA::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_LibManageA, CDialog)
END_MESSAGE_MAP()


// CDlg_LibManageA 消息处理程序

BOOL CDlg_LibManageA::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
