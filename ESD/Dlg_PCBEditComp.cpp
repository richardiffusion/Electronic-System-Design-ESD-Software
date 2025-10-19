// Dlg_PCBEditComp.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBEditComp.h"
#include ".\dlg_pcbeditcomp.h"
#include "systemext.h"

// CDlg_PCBEditComp 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBEditComp, CDialog)
CDlg_PCBEditComp::CDlg_PCBEditComp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBEditComp::IDD, pParent)
	, m_csEdit_ID(_T(""))
	, m_csEdit_C(_T(""))
	, m_csEdit_ID_Height(_T(""))
	, m_csEdit_C_Height(_T(""))
	, m_csEdit_ID_Width(_T(""))
	, m_csEdit_C_Width(_T(""))
	, m_nCombo_ID_Layer(0)
	, m_nCombo_C_Layer(0)
	, m_csEdit_ID_X(_T(""))
	, m_csEdit_C_X(_T(""))
	, m_csEdit_ID_Y(_T(""))
	, m_csEdit_C_Y(_T(""))
	, m_nCombo_ID_AutoPosition(0)
	, m_nCombo_C_AutoPosition(0)
	, m_bCheck_ID_Hide(FALSE)
	, m_bCheck_C_Hide(FALSE)
	, m_bCheck_ID_Mirror(FALSE)
	, m_bCheck_C_Mirror(FALSE)
	, m_nCombo_ID_Font(0)
	, m_nCombo_C_Font(0)
	, m_bCheck_ID_Bold(FALSE)
	, m_bCheck_C_Bold(FALSE)
	, m_bCheck_ID_Italic(FALSE)
	, m_bCheck_C_Italic(FALSE)
	, m_bCheck_ID_Invert(FALSE)
	, m_bCheck_C_Invert(FALSE)
	, m_csEdit_ID_Invert(_T(""))
	, m_csEdit_C_Invert(_T(""))
	, m_nComboCompLayer(0)
	, m_csEditCompX(_T(""))
	, m_csEditCompY(_T(""))
	, m_bCheckLockString(FALSE)
	, m_bCheckLock(FALSE)
	, m_csEditPackage(_T(""))
	, m_csEditLib(_T(""))
	, m_csEditDescription(_T(""))
	, m_fEdit_ID_Rotation(0)
	, m_fEdit_C_Rotation(0)
	, m_fEditCompRotation(0)
{
	m_nRadio_ID_Font = 0;
	m_nRadio_C_Font = 0;
	m_csStrokeFontName[0] = "Default";
	m_csStrokeFontName[1] = "Sans Serif";
	m_csStrokeFontName[2] = "Serif";
}

CDlg_PCBEditComp::~CDlg_PCBEditComp()
{
}

void CDlg_PCBEditComp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_csEdit_ID);
	DDX_Text(pDX, IDC_EDIT_C, m_csEdit_C);
	DDX_Text(pDX, IDC_EDIT_ID_HEIGHT, m_csEdit_ID_Height);
	DDX_Text(pDX, IDC_EDIT_C_HEIGHT, m_csEdit_C_Height);
	DDX_Text(pDX, IDC_EDIT_ID_WIDTH, m_csEdit_ID_Width);
	DDX_Text(pDX, IDC_EDIT_C_WIDTH, m_csEdit_C_Width);
	DDX_CBIndex(pDX, IDC_COMBO_ID_LAYER, m_nCombo_ID_Layer);
	DDX_CBIndex(pDX, IDC_COMBO_C_LAYER, m_nCombo_C_Layer);
	DDX_Text(pDX, IDC_EDIT_ID_X, m_csEdit_ID_X);
	DDX_Text(pDX, IDC_EDIT_C_X, m_csEdit_C_X);
	DDX_Text(pDX, IDC_EDIT_ID_Y, m_csEdit_ID_Y);
	DDX_Text(pDX, IDC_EDIT_C_Y, m_csEdit_C_Y);
	DDX_CBIndex(pDX, IDC_COMBO_ID_AUTOPOSITION, m_nCombo_ID_AutoPosition);
	DDX_CBIndex(pDX, IDC_COMBO_C_AUTOPOSITION, m_nCombo_C_AutoPosition);
	DDX_Check(pDX, IDC_CHECK_ID_HIDE, m_bCheck_ID_Hide);
	DDX_Check(pDX, IDC_CHECK_C_HIDE, m_bCheck_C_Hide);
	DDX_Check(pDX, IDC_CHECK_ID_MIRROR, m_bCheck_ID_Mirror);
	DDX_Check(pDX, IDC_CHECK_C_MIRROR, m_bCheck_C_Mirror);
	DDX_CBIndex(pDX, IDC_COMBO_ID_FONT, m_nCombo_ID_Font);
	DDX_CBIndex(pDX, IDC_COMBO_C_FONT, m_nCombo_C_Font);
	DDX_Check(pDX, IDC_CHECK_ID_BOLD, m_bCheck_ID_Bold);
	DDX_Check(pDX, IDC_CHECK_C_BOLD, m_bCheck_C_Bold);
	DDX_Check(pDX, IDC_CHECK_ID_ITALIC, m_bCheck_ID_Italic);
	DDX_Check(pDX, IDC_CHECK_C_ITALIC, m_bCheck_C_Italic);
	DDX_Check(pDX, IDC_CHECK_ID_INVERT, m_bCheck_ID_Invert);
	DDX_Check(pDX, IDC_CHECK_C_INVERT, m_bCheck_C_Invert);
	DDX_Text(pDX, IDC_EDIT_ID_INVERT, m_csEdit_ID_Invert);
	DDX_Text(pDX, IDC_EDIT_C_INVERT, m_csEdit_C_Invert);
	DDX_CBIndex(pDX, IDC_COMBO_COMP_LAYER, m_nComboCompLayer);
	DDX_Text(pDX, IDC_EDIT_COMP_X, m_csEditCompX);
	DDX_Text(pDX, IDC_EDIT_COMP_Y, m_csEditCompY);
	DDX_Check(pDX, IDC_CHECK_LOCK_STRING, m_bCheckLockString);
	DDX_Check(pDX, IDC_CHECK_LOCK, m_bCheckLock);
	DDX_Text(pDX, IDC_EDIT_PACKAGE, m_csEditPackage);
	DDX_Text(pDX, IDC_EDIT_LIB, m_csEditLib);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_csEditDescription);
	DDX_Text(pDX, IDC_EDIT_ID_ROTATION, m_fEdit_ID_Rotation);
	DDX_Text(pDX, IDC_EDIT_C_ROTATION, m_fEdit_C_Rotation);
	DDX_Text(pDX, IDC_EDIT_COMP_ROTATION, m_fEditCompRotation);
	DDX_Radio(pDX, IDC_RADIO_ID_FONT, m_nRadio_ID_Font);
	DDX_Radio(pDX, IDC_RADIO_C_FONT, m_nRadio_C_Font);
}


BEGIN_MESSAGE_MAP(CDlg_PCBEditComp, CDialog)
	ON_BN_CLICKED(IDC_RADIO_ID_FONT, OnBnClickedRadioIdFont)
	ON_BN_CLICKED(IDC_RADIO_ID_STROKE, OnBnClickedRadioIdStroke)
	ON_BN_CLICKED(IDC_RADIO_C_FONT, OnBnClickedRadioCFont)
	ON_BN_CLICKED(IDC_RADIO_C_STROKE, OnBnClickedRadioCStroke)
END_MESSAGE_MAP()


// CDlg_PCBEditComp 消息处理程序



BOOL CDlg_PCBEditComp::OnInitDialog()
{
	CDialog::OnInitDialog();

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ID_LAYER);
	pCombo->ResetContent();
	for(int i=0; i<arrayStringLayer.GetCount(); i++)
	{
		pCombo->AddString(arrayStringLayer.GetAt(i));
	}
	pCombo->SetCurSel(m_nCombo_ID_Layer);

	pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_C_LAYER);
	pCombo->ResetContent();
	for(int i=0; i<arrayStringLayer.GetCount(); i++)
	{
		pCombo->AddString(arrayStringLayer.GetAt(i));
	}
	pCombo->SetCurSel(m_nCombo_C_Layer);

	CWnd* pWnd;
	if(m_nRadio_ID_Font == 0)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ID_FONT);
		pCombo->ResetContent();
		for(int i=0; i<arrayEDA_SystemFont.GetCount(); i++)
		{
			pCombo->AddString(arrayEDA_SystemFont.GetAt(i));
		}
		pCombo->SetCurSel(m_nCombo_ID_Font);
	}
	else if(m_nRadio_ID_Font == 1)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ID_FONT);
		pCombo->ResetContent();
		for(int i=0; i<3; i++)
		{
			pCombo->AddString(m_csStrokeFontName[i]);
		}
		pCombo->SetCurSel(m_nCombo_ID_Font);

		pWnd = GetDlgItem(IDC_STATIC_ID_BOLD);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_ID_ITALIC);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_ID_INVERT);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_ID_INVERTBORDER);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);

		pWnd = GetDlgItem(IDC_CHECK_ID_BOLD);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_CHECK_ID_ITALIC);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_CHECK_ID_INVERT);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_ID_INVERT);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
	}

	if(m_nRadio_C_Font == 0)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_C_FONT);
		pCombo->ResetContent();
		for(int i=0; i<arrayEDA_SystemFont.GetCount(); i++)
		{
			pCombo->AddString(arrayEDA_SystemFont.GetAt(i));
		}
		pCombo->SetCurSel(m_nCombo_C_Font);
	}
	else if(m_nRadio_C_Font == 1)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_C_FONT);
		pCombo->ResetContent();
		for(int i=0; i<3; i++)
		{
			pCombo->AddString(m_csStrokeFontName[i]);
		}
		pCombo->SetCurSel(m_nCombo_C_Font);

		pWnd = GetDlgItem(IDC_STATIC_C_BOLD);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_C_ITALIC);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_C_INVERT);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_C_INVERTBORDER);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		//控件
		pWnd = GetDlgItem(IDC_CHECK_C_BOLD);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_CHECK_C_ITALIC);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_CHECK_C_INVERT);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_C_INVERT);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_PCBEditComp::OnBnClickedRadioIdFont()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_ID_FONT);
	
	CWnd* pWnd;
	if(pButton != NULL) 
	{
		if(pButton->GetCheck() == BST_CHECKED)
		{
			CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ID_FONT);
			pCombo->ResetContent();
			for(int i=0; i<arrayEDA_SystemFont.GetCount(); i++)
			{
				pCombo->AddString(arrayEDA_SystemFont.GetAt(i));
			}
			pCombo->SetCurSel(nTT_ID_Font);

			pWnd = GetDlgItem(IDC_STATIC_ID_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_STATIC_ID_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_STATIC_ID_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_STATIC_ID_INVERTBORDER);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			//控件
			pWnd = GetDlgItem(IDC_CHECK_ID_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_CHECK_ID_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_CHECK_ID_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_EDIT_ID_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
		}
		else if(pButton->GetCheck() == BST_UNCHECKED)
		{			

		}
	}
}

void CDlg_PCBEditComp::OnBnClickedRadioIdStroke()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_ID_STROKE);
	
	CWnd* pWnd;
	if(pButton != NULL) 
	{
		if(pButton->GetCheck() == BST_CHECKED)
		{
			CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ID_FONT);
			pCombo->ResetContent();
			for(int i=0; i<3; i++)
			{
				pCombo->AddString(m_csStrokeFontName[i]);
			}
			pCombo->SetCurSel(nStroke_ID_Font);

			pWnd = GetDlgItem(IDC_STATIC_ID_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_STATIC_ID_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_STATIC_ID_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_STATIC_ID_INVERTBORDER);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			//控件
			pWnd = GetDlgItem(IDC_CHECK_ID_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_CHECK_ID_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_CHECK_ID_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_EDIT_ID_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		}
		else if(pButton->GetCheck() == BST_UNCHECKED)
		{
		}
	}
}

void CDlg_PCBEditComp::OnBnClickedRadioCFont()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_C_FONT);
	
	CWnd* pWnd;
	if(pButton != NULL) 
	{
		if(pButton->GetCheck() == BST_CHECKED)
		{
			CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_C_FONT);
			pCombo->ResetContent();
			for(int i=0; i<arrayEDA_SystemFont.GetCount(); i++)
			{
				pCombo->AddString(arrayEDA_SystemFont.GetAt(i));
			}
			pCombo->SetCurSel(nTT_C_Font);

			pWnd = GetDlgItem(IDC_STATIC_C_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_STATIC_C_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_STATIC_C_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_STATIC_C_INVERTBORDER);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			//控件
			pWnd = GetDlgItem(IDC_CHECK_C_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_CHECK_C_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_CHECK_C_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_EDIT_C_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
		}
		else if(pButton->GetCheck() == BST_UNCHECKED)
		{			

		}
	}
}

void CDlg_PCBEditComp::OnBnClickedRadioCStroke()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_C_STROKE);
	
	CWnd* pWnd;
	if(pButton != NULL) 
	{
		if(pButton->GetCheck() == BST_CHECKED)
		{
			CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_C_FONT);
			pCombo->ResetContent();
			for(int i=0; i<3; i++)
			{
				pCombo->AddString(m_csStrokeFontName[i]);
			}
			pCombo->SetCurSel(nStroke_C_Font);

			pWnd = GetDlgItem(IDC_STATIC_C_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_STATIC_C_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_STATIC_C_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_STATIC_C_INVERTBORDER);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);

			pWnd = GetDlgItem(IDC_CHECK_C_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_CHECK_C_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_CHECK_C_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_EDIT_C_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		}
		else if(pButton->GetCheck() == BST_UNCHECKED)
		{
		}
	}
}
