// Dlg_PCBEditVia.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBEditVia.h"
#include ".\dlg_pcbeditvia.h"


// CDlg_PCBEditVia 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBEditVia, CDialog)
CDlg_PCBEditVia::CDlg_PCBEditVia(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBEditVia::IDD, pParent)
	, m_csEditY(_T(""))
	, m_csEditX(_T(""))
	, m_csEditDia(_T(""))
	, m_csEditHoleDia(_T(""))
	, m_nStartLayer(0)
	, m_nEndLayer(0)
	, m_bCheckLocked(FALSE)
	, m_bCheckTentingTop(FALSE)
	, m_bCheckTentingBottom(FALSE)
	, m_csEditExpansion(_T(""))
	, m_csComboNet(_T(""))
{
}

CDlg_PCBEditVia::~CDlg_PCBEditVia()
{
}

void CDlg_PCBEditVia::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_VIA, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT_Y, m_csEditY);
	DDX_Text(pDX, IDC_EDIT_X, m_csEditX);
	DDX_Text(pDX, IDC_EDIT_DIA, m_csEditDia);
	DDX_Text(pDX, IDC_EDIT_HOLEDIA, m_csEditHoleDia);
	DDX_CBIndex(pDX, IDC_COMBO_STARTLAYER, m_nStartLayer);
	DDX_CBIndex(pDX, IDC_COMBO_ENDLAYER, m_nEndLayer);
	DDX_Check(pDX, IDC_CHECK_LOCKED, m_bCheckLocked);
	DDX_Check(pDX, IDC_CHECK_TENTING_TOP, m_bCheckTentingTop);
	DDX_Check(pDX, IDC_CHECK_TENTING_BOTTOM, m_bCheckTentingBottom);
	DDX_Text(pDX, IDC_EDIT_EXPANSION, m_csEditExpansion);
	DDX_Radio(pDX, IDC_RADIO_EXPANSION, m_nRadioExpansion);
	DDX_CBString(pDX, IDC_COMBO_NET, m_csComboNet);
}


BEGIN_MESSAGE_MAP(CDlg_PCBEditVia, CDialog)
	ON_BN_CLICKED(IDC_RADIO_EXPANSION, OnBnClickedRadioExpansion)
	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio2)
	ON_CBN_SELCHANGE(IDC_COMBO_STARTLAYER, OnCbnSelchangeComboStartlayer)
	ON_CBN_SELCHANGE(IDC_COMBO_ENDLAYER, OnCbnSelchangeComboEndlayer)
END_MESSAGE_MAP()


// CDlg_PCBEditVia 消息处理程序

BOOL CDlg_PCBEditVia::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_PCB_EDIT_VIA);

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_STARTLAYER);
	pCombo->ResetContent();
	for(int i=0; i<arrayLayer.GetCount(); i++)
	{
		pCombo->AddString(arrayLayer.GetAt(i));
	}
	pCombo->SetCurSel(m_nStartLayer);

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ENDLAYER);
	pCombo->ResetContent();
	for(int i=0; i<arrayLayer.GetCount(); i++)
	{
		pCombo->AddString(arrayLayer.GetAt(i));
	}
	pCombo->SetCurSel(m_nEndLayer);

	if((m_nStartLayer != 0)&&(m_nEndLayer != 0))
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_TENTING_TOP);
		pButton->SetCheck(FALSE);
		pButton->EnableWindow(FALSE);
	}
	if((m_nStartLayer != (arrayLayer.GetCount() -1))&&(m_nEndLayer != (arrayLayer.GetCount() -1)))
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_TENTING_BOTTOM);
		pButton->SetCheck(FALSE);
		pButton->EnableWindow(FALSE);
	}

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_NET);
	pCombo->ResetContent();
	for(int i=0; i<arrayStringNet.GetCount(); i++)
	{
		pCombo->AddString(arrayStringNet.GetAt(i));
	}
	pCombo->SelectString(-1, m_csComboNet);

	if(m_nRadioExpansion == 0) 
	{
		CWnd* pWnd = GetDlgItem(IDC_EDIT_EXPANSION);
		pWnd->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_PCBEditVia::OnBnClickedRadioExpansion()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_EXPANSION);
	pWnd->EnableWindow(FALSE);
}

void CDlg_PCBEditVia::OnBnClickedRadio2()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_EXPANSION);
	pWnd->EnableWindow(TRUE);
}

void CDlg_PCBEditVia::OnCbnSelchangeComboStartlayer()
{
	UpdateData(TRUE);
	if((m_nStartLayer != 0)&&(m_nEndLayer != 0))
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_TENTING_TOP);
		pButton->SetCheck(FALSE);
		pButton->EnableWindow(FALSE);
	}
	else
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_TENTING_TOP);
		pButton->EnableWindow(TRUE);
	}

	if((m_nStartLayer != (arrayLayer.GetCount() -1))&&(m_nEndLayer != (arrayLayer.GetCount() -1)))
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_TENTING_BOTTOM);
		pButton->SetCheck(FALSE);
		pButton->EnableWindow(FALSE);
	}
	else
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_TENTING_BOTTOM);
		pButton->EnableWindow(TRUE);
	}

}

void CDlg_PCBEditVia::OnCbnSelchangeComboEndlayer()
{
	UpdateData(TRUE);
	if((m_nStartLayer != 0)&&(m_nEndLayer != 0))
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_TENTING_TOP);
		pButton->SetCheck(FALSE);
		pButton->EnableWindow(FALSE);
	}
	else
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_TENTING_TOP);
		pButton->EnableWindow(TRUE);
	}

	if((m_nStartLayer != (arrayLayer.GetCount() -1))&&(m_nEndLayer != (arrayLayer.GetCount() -1)))
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_TENTING_BOTTOM);
		pButton->SetCheck(FALSE);
		pButton->EnableWindow(FALSE);
	}
	else
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_TENTING_BOTTOM);
		pButton->EnableWindow(TRUE);
	}
}
