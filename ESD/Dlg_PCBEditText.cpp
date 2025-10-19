// Dlg_PCBEditText.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBEditText.h"
#include ".\dlg_pcbedittext.h"
#include "systemext.h"


// CDlg_PCBEditText 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBEditText, CDialog)
CDlg_PCBEditText::CDlg_PCBEditText(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBEditText::IDD, pParent)
	, m_csEditWidth(_T(""))
	, m_csEditHeight(_T(""))
	, m_csEditX(_T(""))
	, m_csEditY(_T(""))
	, m_csEditText(_T(""))
	, m_bCheckLocked(FALSE)
	, m_bCheckMirror(FALSE)
	, m_bCheckBold(FALSE)
	, m_bCheckItalic(FALSE)
	, m_bCheckInverted(FALSE)
	, m_csEditInverted(_T(""))
	, m_fEditRotation(0)
	, m_nComboLayer(0)
	, m_nComboFont(0)
{
	csTitle = "文字";
	m_csStrokeFontName[0] = "Default";
	m_csStrokeFontName[1] = "Sans Serif";
	m_csStrokeFontName[2] = "Serif";
}

CDlg_PCBEditText::~CDlg_PCBEditText()
{
}

void CDlg_PCBEditText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_TEXT, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_csEditWidth);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_csEditHeight);
	DDX_Text(pDX, IDC_EDIT_X, m_csEditX);
	DDX_Text(pDX, IDC_EDIT_Y, m_csEditY);
	DDX_Text(pDX, IDC_EDIT_STRING, m_csEditText);
	DDX_Check(pDX, IDC_CHECK_LOCKED, m_bCheckLocked);
	DDX_Check(pDX, IDC_CHECK_MIRROR, m_bCheckMirror);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bCheckBold);
	DDX_Check(pDX, IDC_CHECK_ITALIC, m_bCheckItalic);
	DDX_Check(pDX, IDC_CHECK_INVERT, m_bCheckInverted);
	DDX_Text(pDX, IDC_EDIT_INVERT, m_csEditInverted);
	DDX_Text(pDX, IDC_EDIT_ROTATION, m_fEditRotation);
	DDX_Radio(pDX, IDC_RADIO_TRUETYPE, m_nRadioFont);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nComboLayer);
	DDX_CBIndex(pDX, IDC_COMBO_FONT, m_nComboFont);
}


BEGIN_MESSAGE_MAP(CDlg_PCBEditText, CDialog)
	ON_BN_CLICKED(IDC_RADIO_STROKE, OnBnClickedRadioStroke)
	ON_BN_CLICKED(IDC_RADIO_TRUETYPE, OnBnClickedRadioTruetype)
END_MESSAGE_MAP()


// CDlg_PCBEditText 消息处理程序

BOOL CDlg_PCBEditText::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_PCB_EDIT_TEXT);

	SetWindowText(csTitle);
	
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_LAYER);
	pCombo->ResetContent();
	for(int i=0; i<arrayString.GetCount(); i++)
	{
		pCombo->AddString(arrayString.GetAt(i));
	}
	pCombo->SetCurSel(m_nComboLayer);
	
	CWnd* pWnd;
	if(m_nRadioFont == 0)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FONT);
		pCombo->ResetContent();
		for(int i=0; i<arrayEDA_SystemFont.GetCount(); i++)
		{
			pCombo->AddString(arrayEDA_SystemFont.GetAt(i));
		}
		pCombo->SetCurSel(m_nComboFont);
	}
	else if(m_nRadioFont == 1)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_FONT);
		pCombo->ResetContent();
		for(int i=0; i<3; i++)
		{
			pCombo->AddString(m_csStrokeFontName[i]);
		}
		pCombo->SetCurSel(m_nComboFont);

		//static 文字
		pWnd = GetDlgItem(IDC_STATIC_BOLD);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_ITALIC);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_INVERT);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_STATIC_INVERTBORDER);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		//控件
		pWnd = GetDlgItem(IDC_CHECK_BOLD);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_CHECK_ITALIC);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_CHECK_INVERT);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_EDIT_INVERT);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlg_PCBEditText::OnBnClickedRadioStroke()
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

			//static 文字
			pWnd = GetDlgItem(IDC_STATIC_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_STATIC_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_STATIC_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_STATIC_INVERTBORDER);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			//控件
			pWnd = GetDlgItem(IDC_CHECK_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_CHECK_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_CHECK_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
			pWnd = GetDlgItem(IDC_EDIT_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		}
		else if(pButton->GetCheck() == BST_UNCHECKED)
		{
		}
	}
}

void CDlg_PCBEditText::OnBnClickedRadioTruetype()
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

			//static 文字
			pWnd = GetDlgItem(IDC_STATIC_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_STATIC_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_STATIC_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_STATIC_INVERTBORDER);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			//控件
			pWnd = GetDlgItem(IDC_CHECK_BOLD);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_CHECK_ITALIC);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_CHECK_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
			pWnd = GetDlgItem(IDC_EDIT_INVERT);
			if(pWnd != NULL) pWnd->ShowWindow(SW_SHOW);
		}
		else if(pButton->GetCheck() == BST_UNCHECKED)
		{			

		}
	}
}

