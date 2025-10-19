// Dlg_PCBEditCoordinate.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBEditCoordinate.h"
#include ".\dlg_pcbeditcoordinate.h"
#include "systemext.h"

// CDlg_PCBEditCoordinate 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBEditCoordinate, CDialog)
CDlg_PCBEditCoordinate::CDlg_PCBEditCoordinate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBEditCoordinate::IDD, pParent)
	, m_csEditSize(_T(""))
	, m_csEditLineWidth(_T(""))
	, m_csEditTextLineWidth(_T(""))
	, m_csEditTextHeight(_T(""))
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
	, m_bCheckLocked(FALSE)
	, m_bCheckBold(FALSE)
	, m_bCheckItalic(FALSE)
	, m_nComboFont(0)
	, m_nComboStyle(0)
	, m_nComboLayer(0)
{
	m_nRadioTrueType = 0;
	m_csStrokeFontName[0] = "Default";
	m_csStrokeFontName[1] = "Sans Serif";
	m_csStrokeFontName[2] = "Serif";
}

CDlg_PCBEditCoordinate::~CDlg_PCBEditCoordinate()
{
}

void CDlg_PCBEditCoordinate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_COORDINATE, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT_SIZE, m_csEditSize);
	DDX_Text(pDX, IDC_EDIT_LINEWIDTH, m_csEditLineWidth);
	DDX_Text(pDX, IDC_EDIT_TEXT_LINEWIDTH, m_csEditTextLineWidth);
	DDX_Text(pDX, IDC_EDIT_TEXT_HEIGHT, m_csEditTextHeight);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
	DDX_Check(pDX, IDC_CHECK_LOCKED, m_bCheckLocked);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bCheckBold);
	DDX_Check(pDX, IDC_CHECK_ITALIC, m_bCheckItalic);
	DDX_CBIndex(pDX, IDC_COMBO_FONT, m_nComboFont);
	DDX_CBIndex(pDX, IDC_COMBO_STYLE, m_nComboStyle);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nComboLayer);
	DDX_Radio(pDX, IDC_RADIO_TRUETYPE, m_nRadioTrueType);
}


BEGIN_MESSAGE_MAP(CDlg_PCBEditCoordinate, CDialog)
	ON_BN_CLICKED(IDC_RADIO_STROKE, OnBnClickedRadioStroke)
	ON_BN_CLICKED(IDC_RADIO_TRUETYPE, OnBnClickedRadioTruetype)
END_MESSAGE_MAP()


// CDlg_PCBEditCoordinate 消息处理程序

BOOL CDlg_PCBEditCoordinate::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_PCB_EDIT_COORDINATE);


	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LAYER);
	pCombo->ResetContent();
	for(int i=0; i<arrayString.GetCount(); i++)
	{
		pCombo->AddString(arrayString.GetAt(i));
	}
	pCombo->SetCurSel(m_nComboLayer);

	CWnd* pWnd;
	if(m_nRadioTrueType == 0)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FONT);
		pCombo->ResetContent();
		for(int i=0; i<arrayEDA_SystemFont.GetCount(); i++)
		{
			pCombo->AddString(arrayEDA_SystemFont.GetAt(i));
		}
		pCombo->SetCurSel(m_nComboFont);
	}
	else if(m_nRadioTrueType == 1)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FONT);
		pCombo->ResetContent();
		for(int i=0; i<3; i++)
		{
			pCombo->AddString(m_csStrokeFontName[i]);
		}
		pCombo->SetCurSel(m_nComboFont);

		pWnd = GetDlgItem(IDC_STATIC_BOLD);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_ITALIC);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);

		pWnd = GetDlgItem(IDC_CHECK_BOLD);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_CHECK_ITALIC);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlg_PCBEditCoordinate::OnBnClickedRadioStroke()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_STROKE);
	
	CWnd* pWnd;
	if(pButton != NULL) 
	{
		if(pButton->GetCheck() == BST_CHECKED)
		{

			CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FONT);
			pCombo->ResetContent();
			for(int i=0; i<3; i++)
			{
				pCombo->AddString(m_csStrokeFontName[i]);
			}
			pCombo->SetCurSel(nStrokeFont);

			pWnd = GetDlgItem(IDC_STATIC_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_STATIC_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			//控件
			pWnd = GetDlgItem(IDC_CHECK_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_CHECK_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		}
		else if(pButton->GetCheck() == BST_UNCHECKED)
		{
		}
	}
}

void CDlg_PCBEditCoordinate::OnBnClickedRadioTruetype()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_TRUETYPE);
	
	CWnd* pWnd;
	if(pButton != NULL) 
	{
		if(pButton->GetCheck() == BST_CHECKED)
		{

			CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FONT);
			pCombo->ResetContent();
			for(int i=0; i<arrayEDA_SystemFont.GetCount(); i++)
			{
				pCombo->AddString(arrayEDA_SystemFont.GetAt(i));
			}
			pCombo->SetCurSel(nTTFont);


			pWnd = GetDlgItem(IDC_STATIC_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_STATIC_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);

			pWnd = GetDlgItem(IDC_CHECK_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_CHECK_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
		}
		else if(pButton->GetCheck() == BST_UNCHECKED)
		{			

		}
	}
}
