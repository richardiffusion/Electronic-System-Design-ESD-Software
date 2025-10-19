// Dlg_PcbJumpComp.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PcbJumpComp.h"
#include ".\dlg_pcbjumpcomp.h"


// CDlg_PcbJumpComp 对话框

IMPLEMENT_DYNAMIC(CDlg_PcbJumpComp, CDialog)
CDlg_PcbJumpComp::CDlg_PcbJumpComp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PcbJumpComp::IDD, pParent)
	, m_csCompDesignator(_T(""))
{
}

CDlg_PcbJumpComp::~CDlg_PcbJumpComp()
{
}

void CDlg_PcbJumpComp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_csCompDesignator);
}


BEGIN_MESSAGE_MAP(CDlg_PcbJumpComp, CDialog)
END_MESSAGE_MAP()


// CDlg_PcbJumpComp 消息处理程序

BOOL CDlg_PcbJumpComp::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(csTitle);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
