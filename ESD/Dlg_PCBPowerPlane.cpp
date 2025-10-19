// Dlg_PCBPowerPlane.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBPowerPlane.h"
#include ".\dlg_pcbpowerplane.h"


// CDlg_PCBPowerPlane 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBPowerPlane, CDialog)
CDlg_PCBPowerPlane::CDlg_PCBPowerPlane(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBPowerPlane::IDD, pParent)
	, m_csEditLayerName(_T(""))
	, m_csEditPullBack(_T(""))
	, m_csComboNet(_T(""))
{
}

CDlg_PCBPowerPlane::~CDlg_PCBPowerPlane()
{
}

void CDlg_PCBPowerPlane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LAYERNAME, m_csEditLayerName);
	DDX_Text(pDX, IDC_EDIT_PULLBACK, m_csEditPullBack);
	DDX_CBString(pDX, IDC_COMBO_NET, m_csComboNet);
}


BEGIN_MESSAGE_MAP(CDlg_PCBPowerPlane, CDialog)
END_MESSAGE_MAP()


// CDlg_PCBPowerPlane 消息处理程序

BOOL CDlg_PCBPowerPlane::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_NET);
	pCombo->ResetContent();
	for(int i=0; i<arrayStringNet.GetCount(); i++)
	{
		pCombo->AddString(arrayStringNet.GetAt(i));
	}
	
	if(m_csComboNet == "多网络")
	{
		pCombo->ResetContent();
		pCombo->AddString(m_csComboNet);
		pCombo->SetCurSel(0);
		pCombo->EnableWindow(FALSE);
	}
	else pCombo->SelectString(-1, m_csComboNet);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
