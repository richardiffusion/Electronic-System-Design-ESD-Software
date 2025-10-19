// Dlg_PCB_LoadPackageByName.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCB_LoadPackageByName.h"
#include ".\dlg_pcb_loadpackagebyname.h"


// CDlg_PCB_LoadPackageByName 对话框

IMPLEMENT_DYNAMIC(CDlg_PCB_LoadPackageByName, CDialog)
CDlg_PCB_LoadPackageByName::CDlg_PCB_LoadPackageByName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCB_LoadPackageByName::IDD, pParent)
	, m_csComboName(_T(""))
{
}

CDlg_PCB_LoadPackageByName::~CDlg_PCB_LoadPackageByName()
{
}

void CDlg_PCB_LoadPackageByName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_NAME, m_csComboName);
}


BEGIN_MESSAGE_MAP(CDlg_PCB_LoadPackageByName, CDialog)
END_MESSAGE_MAP()


// CDlg_PCB_LoadPackageByName 消息处理程序

BOOL CDlg_PCB_LoadPackageByName::OnInitDialog()
{
	CDialog::OnInitDialog();

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_NAME);
	for(int i=0; i<arrayCompName.GetCount(); i++)
	{
		CString cstring = arrayCompName.GetAt(i);
		pCombo->AddString(cstring);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
