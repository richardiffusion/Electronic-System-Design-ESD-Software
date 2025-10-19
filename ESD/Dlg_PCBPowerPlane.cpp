// Dlg_PCBPowerPlane.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBPowerPlane.h"
#include ".\dlg_pcbpowerplane.h"


// CDlg_PCBPowerPlane �Ի���

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


// CDlg_PCBPowerPlane ��Ϣ�������

BOOL CDlg_PCBPowerPlane::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_NET);
	pCombo->ResetContent();
	for(int i=0; i<arrayStringNet.GetCount(); i++)
	{
		pCombo->AddString(arrayStringNet.GetAt(i));
	}
	
	if(m_csComboNet == "������")
	{
		pCombo->ResetContent();
		pCombo->AddString(m_csComboNet);
		pCombo->SetCurSel(0);
		pCombo->EnableWindow(FALSE);
	}
	else pCombo->SelectString(-1, m_csComboNet);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
