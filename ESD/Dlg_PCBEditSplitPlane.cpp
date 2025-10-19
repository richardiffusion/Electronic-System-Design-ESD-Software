// Dlg_PCBEditSplitPlane.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBEditSplitPlane.h"
#include ".\dlg_pcbeditsplitplane.h"


// CDlg_PCBEditSplitPlane �Ի���

IMPLEMENT_DYNAMIC(CDlg_PCBEditSplitPlane, CDialog)
CDlg_PCBEditSplitPlane::CDlg_PCBEditSplitPlane(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBEditSplitPlane::IDD, pParent)
	, m_csComboNet(_T(""))
{
}

CDlg_PCBEditSplitPlane::~CDlg_PCBEditSplitPlane()
{
}

void CDlg_PCBEditSplitPlane::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_NET, m_csComboNet);
}


BEGIN_MESSAGE_MAP(CDlg_PCBEditSplitPlane, CDialog)
END_MESSAGE_MAP()


// CDlg_PCBEditSplitPlane ��Ϣ�������

BOOL CDlg_PCBEditSplitPlane::OnInitDialog()
{
	CDialog::OnInitDialog();

	//����
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_NET);
	pCombo->ResetContent();
	for(int i=0; i<arrayStringNet.GetCount(); i++)
	{
		pCombo->AddString(arrayStringNet.GetAt(i));
	}
	pCombo->SelectString(-1, m_csComboNet);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
