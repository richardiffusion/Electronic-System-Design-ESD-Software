// Dlg_PcbNetName.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PcbNetName.h"
#include ".\dlg_pcbnetname.h"


// CDlg_PcbNetName 对话框

IMPLEMENT_DYNAMIC(CDlg_PcbNetName, CDialog)
CDlg_PcbNetName::CDlg_PcbNetName(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PcbNetName::IDD, pParent)
	, m_csComboNet(_T(""))
{
}

CDlg_PcbNetName::~CDlg_PcbNetName()
{
}

void CDlg_PcbNetName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_NET, m_csComboNet);
}


BEGIN_MESSAGE_MAP(CDlg_PcbNetName, CDialog)
END_MESSAGE_MAP()


// CDlg_PcbNetName 消息处理程序

BOOL CDlg_PcbNetName::OnInitDialog()
{
	CDialog::OnInitDialog();

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_NET);
	for(int i=0; i<arrayString.GetCount(); i++)
	{
		CString cstring;
		cstring = arrayString.GetAt(i);
		pComboBox->AddString(cstring);
	}

	if(arrayString.GetCount() > 0)	pComboBox->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
