// Dlg_EditPower.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditPower.h"
#include ".\dlg_editpower.h"
#include "systemext.h"


// CDlg_EditPower 对话框

IMPLEMENT_DYNAMIC(CDlg_EditPower, CDialog)
CDlg_EditPower::CDlg_EditPower(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditPower::IDD, pParent)
	, m_nComboOrientation(0)
	, m_nComboStyle(0)
	, m_csEditNet(_T(""))
	, m_bCheckShowNet(FALSE)
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditPower::~CDlg_EditPower()
{
}

void CDlg_EditPower::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_POWER, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_ORIENTATION, m_nComboOrientation);
	DDX_CBIndex(pDX, IDC_COMBO_STYLE, m_nComboStyle);
	DDX_Text(pDX, IDC_EDIT_NET, m_csEditNet);
	DDX_Check(pDX, IDC_CHECK_SHOW_NETNAME, m_bCheckShowNet);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditPower, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, OnStnClickedStaticColor)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, OnCbnSelchangeComboStyle)
END_MESSAGE_MAP()


// CDlg_EditPower 消息处理程序

BOOL CDlg_EditPower::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_POWER);

	m_cColor.SubclassDlgItem(IDC_STATIC_COLOR, this);
	m_cColor.m_nColor = m_nColor;

	if(m_nPowerOrGround == 0)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_STYLE);
		if(pCombo)
		{
			pCombo->DeleteString(6);
			pCombo->DeleteString(5);
			pCombo->DeleteString(4);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditPower::OnStnClickedStaticColor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cColor.m_nColor = colordialog.GetColor();
		m_nColor = m_cColor.m_nColor;
		m_cColor.Invalidate();
	}
}

void CDlg_EditPower::OnCbnSelchangeComboStyle()
{
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_STYLE);
	if(pCombo)
	{
		int index = pCombo->GetCurSel();
		if(index > 3)
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_SHOW_NETNAME);
			if(pButton) pButton->SetCheck(FALSE);

			CWnd* pWnd = GetDlgItem(IDC_EDIT_NET);
			if(index == 4) pWnd->SetWindowText(csEDA_PowerGround);
			else if(index == 5) pWnd->SetWindowText(csEDA_SignalGround);
			else if(index == 6) pWnd->SetWindowText(csEDA_EarthGround);
		}
		else
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_SHOW_NETNAME);
			if(pButton) pButton->SetCheck(TRUE);
		}
	}
}
