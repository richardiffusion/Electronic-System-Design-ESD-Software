// SchSheetUnitPage.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Page_SchUnit.h"
//#include ".\schsheetunitpage.h"


// CPage_SchUnit 对话框

IMPLEMENT_DYNAMIC(CPage_SchUnit, CPropertyPage)
CPage_SchUnit::CPage_SchUnit()
	: CPropertyPage(CPage_SchUnit::IDD)
	, m_bCheckI(FALSE)
	, m_bCheckM(FALSE)
	, m_nComboI(0)
	, m_nComboM(0)
{
}

CPage_SchUnit::~CPage_SchUnit()
{
}

void CPage_SchUnit::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_I, m_bCheckI);
	DDX_Check(pDX, IDC_CHECK_M, m_bCheckM);
	DDX_CBIndex(pDX, IDC_COMBO_I, m_nComboI);
	DDX_CBIndex(pDX, IDC_COMBO_M, m_nComboM);
}


BEGIN_MESSAGE_MAP(CPage_SchUnit, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_I, OnBnClickedCheckI)
	ON_BN_CLICKED(IDC_CHECK_M, OnBnClickedCheckM)
	ON_CBN_SELCHANGE(IDC_COMBO_M, OnCbnSelchangeComboM)
	ON_CBN_SELCHANGE(IDC_COMBO_I, OnCbnSelchangeComboI)
END_MESSAGE_MAP()


// CPage_SchUnit 消息处理程序

void CPage_SchUnit::OnBnClickedCheckI()
{
	int c;
	CButton* pButton;
	CWnd* pWnd;

	pButton = (CButton*)GetDlgItem(IDC_CHECK_I);
	c = pButton->GetCheck();
	
    if(c == BST_UNCHECKED)
	{
		pButton = (CButton*)GetDlgItem(IDC_CHECK_M);
		pButton->SetCheck(TRUE);
		pWnd = GetDlgItem(IDC_COMBO_M);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_COMBO_I);
		pWnd->EnableWindow(FALSE);
	}
	else
	{
		pButton = (CButton*)GetDlgItem(IDC_CHECK_M);
		pButton->SetCheck(FALSE);
		pWnd = GetDlgItem(IDC_COMBO_M);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_COMBO_I);
		pWnd->EnableWindow(TRUE);
	}
	SetStaticUnit();
}

void CPage_SchUnit::OnBnClickedCheckM()
{
	int c;
	CButton* pButton;
	CWnd* pWnd;

	pButton = (CButton*)GetDlgItem(IDC_CHECK_M);
	c = pButton->GetCheck();
	
    if(c == BST_UNCHECKED)
	{
		pButton = (CButton*)GetDlgItem(IDC_CHECK_I);
		pButton->SetCheck(TRUE);
		pWnd = GetDlgItem(IDC_COMBO_I);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_COMBO_M);
		pWnd->EnableWindow(FALSE);
	}
	else
	{
		pButton = (CButton*)GetDlgItem(IDC_CHECK_I);
		pButton->SetCheck(FALSE);
		pWnd = GetDlgItem(IDC_COMBO_I);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_COMBO_M);
		pWnd->EnableWindow(TRUE);
	}
	SetStaticUnit();
}

BOOL CPage_SchUnit::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	
	int c;
	CButton* pButton;
	CWnd* pWnd;

	pButton = (CButton*)GetDlgItem(IDC_CHECK_I);
	c = pButton->GetCheck();
	
    if(c == BST_UNCHECKED)
	{
		pButton = (CButton*)GetDlgItem(IDC_CHECK_M);
		pButton->SetCheck(TRUE);
		pWnd = GetDlgItem(IDC_COMBO_M);
		pWnd->EnableWindow(TRUE);
		pWnd = GetDlgItem(IDC_COMBO_I);
		pWnd->EnableWindow(FALSE);
	}
	else
	{
		pButton = (CButton*)GetDlgItem(IDC_CHECK_M);
		pButton->SetCheck(FALSE);
		pWnd = GetDlgItem(IDC_COMBO_M);
		pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_COMBO_I);
		pWnd->EnableWindow(TRUE);
	}
	SetStaticUnit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPage_SchUnit::SetStaticUnit()
{
	int c;
	int nCombo;
	CButton* pButton;
	CComboBox* pCombo;
	CWnd* pWnd;

	pButton = (CButton*)GetDlgItem(IDC_CHECK_I);
	c = pButton->GetCheck();
    if(c == BST_UNCHECKED)
	{
		char* str[4] = {"mm", "cm", "meters(米)", "自动"};
		
		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_M);
		nCombo = pCombo->GetCurSel();
		CString cstring("当前长度单位: ");
		cstring = cstring + "公制/" + str[nCombo];

		pWnd = GetDlgItem(IDC_STATIC_UNIT);
		pWnd->SetWindowText(cstring);

	}
	else if(c == BST_CHECKED)
	{
		char* str[4] = {"mils", "inches", "10 mils", "自动"};
		
		pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_I);
		nCombo = pCombo->GetCurSel();		
		CString cstring("当前长度单位: ");
		cstring = cstring + "英制/" + str[nCombo];
		
		pWnd = GetDlgItem(IDC_STATIC_UNIT);
		pWnd->SetWindowText(cstring);
	}
}

void CPage_SchUnit::OnCbnSelchangeComboM()
{
	SetStaticUnit();
}

void CPage_SchUnit::OnCbnSelchangeComboI()
{
	SetStaticUnit();
}
