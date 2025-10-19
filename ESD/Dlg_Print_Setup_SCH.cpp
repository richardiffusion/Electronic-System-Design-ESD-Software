// Dlg_Print_Setup_SCH.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "sysparam.h"
#include "Dlg_Print_Setup_SCH.h"
#include ".\dlg_print_setup_sch.h"


// CDlg_Print_Setup_SCH 对话框

IMPLEMENT_DYNAMIC(CDlg_Print_Setup_SCH, CDialog)
CDlg_Print_Setup_SCH::CDlg_Print_Setup_SCH(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Print_Setup_SCH::IDD, pParent)
	, m_nComboPaperSize(0)
	, m_fEditScale(0)
	, m_nComboScaleMode(0)
	, m_nRadioColor(0)
	, m_bCheckHCenter(FALSE)
	, m_bCheckVCenter(FALSE)
	, m_fEditHMargin(0)
	, m_fEditVMargin(0)
	, m_nRadioExtent(0)
{
	m_nRadioOrientation = 0;
	m_csEditCorrectionX = "1.00";
	m_csEditCorrectionY = "1.00";
	m_nCurrentPaperSizeNo = 0;
	m_nTotalPaper = 0;
}

CDlg_Print_Setup_SCH::~CDlg_Print_Setup_SCH()
{
}

void CDlg_Print_Setup_SCH::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_PRINT_PORTRAIT, m_Bitmap1);
	DDX_Control(pDX, IDC_STATIC_PRINT_LANDSCAPE, m_Bitmap2);
	DDX_Control(pDX, IDC_STATIC_S01, m_BitmapS01);
	DDX_Control(pDX, IDC_STATIC_S02, m_BitmapS02);
	DDX_Control(pDX, IDC_STATIC_S03, m_BitmapS03);
	DDX_Control(pDX, IDC_STATIC_S04, m_BitmapS04);
	DDX_Control(pDX, IDC_STATIC_SHEET, m_BitmapSheet);
	DDX_Control(pDX, IDC_STATIC_WINDOW, m_BitmapWindow);
	DDX_Radio(pDX, IDC_RADIO_ORIENTATION, m_nRadioOrientation);
	DDX_Radio(pDX, IDC_RADIO_COLOR, m_nRadioColor);
	DDX_Radio(pDX, IDC_RADIO_EXTENT, m_nRadioExtent);
	DDX_Radio(pDX, IDC_RADIO_SHEET, m_nRadioSheet);
	DDX_CBIndex(pDX, IDC_COMBO_PAPERSIZE, m_nComboPaperSize);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_fEditScale);
	DDX_CBIndex(pDX, IDC_COMBO_SCALEMODE, m_nComboScaleMode);
	DDX_Check(pDX, IDC_CHECK_H_CENTER, m_bCheckHCenter);
	DDX_Check(pDX, IDC_CHECK_V_CENTER, m_bCheckVCenter);
	DDX_Text(pDX, IDC_EDIT_H_MARGIN, m_fEditHMargin);
	DDX_Text(pDX, IDC_EDIT_V_MARGIN, m_fEditVMargin);
	DDX_Text(pDX, IDC_EDIT_CORRECTION_X, m_csEditCorrectionX);
	DDX_Text(pDX, IDC_EDIT_CORRECTION_Y, m_csEditCorrectionY);
}


BEGIN_MESSAGE_MAP(CDlg_Print_Setup_SCH, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_PRINTER, OnBnClickedButtonSetupPrinter)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnBnClickedButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_PREVIEW, OnBnClickedButtonPrintPreview)
	ON_BN_CLICKED(IDC_CHECK_H_CENTER, OnBnClickedCheckHCenter)
	ON_BN_CLICKED(IDC_CHECK_V_CENTER, OnBnClickedCheckVCenter)
	ON_CBN_SELCHANGE(IDC_COMBO_SCALEMODE, OnCbnSelchangeComboScalemode)
	ON_BN_CLICKED(IDC_RADIO_EXTENT, OnBnClickedRadioExtent)
	ON_BN_CLICKED(IDC_RADIO7, OnBnClickedRadio7)
END_MESSAGE_MAP()


// CDlg_Print_Setup_SCH 消息处理程序

void CDlg_Print_Setup_SCH::OnBnClickedButtonSetupPrinter()
{
	UpdateData(TRUE);
	EndDialog(MYID_PRINT_SETUP);
}

void CDlg_Print_Setup_SCH::OnBnClickedButtonPrint()
{
	UpdateData(TRUE);
	EndDialog(MYID_PRINT);
}

void CDlg_Print_Setup_SCH::OnBnClickedButtonPrintPreview()
{
	UpdateData(TRUE);
	EndDialog(MYID_PRINT_PREVIEW);
}

BOOL CDlg_Print_Setup_SCH::OnInitDialog()
{
	CDialog::OnInitDialog();

	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_PAPERSIZE);
	if(pCombo != NULL) 
	{
		for(int i=0; i<m_nTotalPaper; i++)
		{
			pCombo->AddString(m_szPaperName[i]);
			//TRACE(m_szPaperName[i]);
		}
	}

	for(int j=0; j<m_nTotalPaper; j++)
	{
		if(m_nCurrentPaperSizeNo == m_nPaper[j])
		{
			pCombo->SetCurSel(j);
			break;
		}
	}

	m_Bitmap1.SetBitmap(IDB_PRINT_PORTRAIT);
	m_Bitmap2.SetBitmap(IDB_PRINT_LANDSCAPE);
	m_BitmapS01.SetBitmap(IDB_PRINT_S01);
	m_BitmapS02.SetBitmap(IDB_PRINT_S01);
	m_BitmapS03.SetBitmap(IDB_PRINT_S01);
	m_BitmapS04.SetBitmap(IDB_PRINT_S01);
	m_BitmapWindow.SetBitmap(IDB_PRINT_WINDOW);
	m_BitmapSheet.SetBitmap(IDB_PRINT_SHEET);

	CWnd* pWnd;
	if(m_bCheckHCenter == TRUE)
	{
		pWnd = GetDlgItem(IDC_EDIT_H_MARGIN);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	}
	if(m_bCheckVCenter == TRUE)
	{
		pWnd = GetDlgItem(IDC_EDIT_V_MARGIN);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	}

	if(m_nComboScaleMode == 1)
	{
		pWnd = GetDlgItem(IDC_EDIT_SCALE);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	}


	if(m_nRadioExtent != 0)
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_SHEET);
		if(pButton != NULL) pButton->SetCheck(BST_CHECKED);
		pButton = (CButton*)GetDlgItem(IDC_RADIO9);
		if(pButton != NULL)
		{
			pButton->SetCheck(BST_UNCHECKED);
			pButton->EnableWindow(FALSE);
		}
	}

	return TRUE; 
}

void CDlg_Print_Setup_SCH::OnBnClickedCheckHCenter()
{
	CWnd* pWnd;
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_H_CENTER);
	if(pButton != NULL) 
	{
		if(pButton->GetCheck() == BST_CHECKED)
		{
			pWnd = GetDlgItem(IDC_EDIT_H_MARGIN);
			if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		}
		else if(pButton->GetCheck() == BST_UNCHECKED)
		{
			pWnd = GetDlgItem(IDC_EDIT_H_MARGIN);
			if(pWnd != NULL) pWnd->EnableWindow(TRUE);
		}
	}
}

void CDlg_Print_Setup_SCH::OnBnClickedCheckVCenter()
{
	CWnd* pWnd;
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_V_CENTER);
	if(pButton != NULL) 
	{
		if(pButton->GetCheck() == BST_CHECKED)
		{
			pWnd = GetDlgItem(IDC_EDIT_V_MARGIN);
			if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		}
		else if(pButton->GetCheck() == BST_UNCHECKED)
		{
			pWnd = GetDlgItem(IDC_EDIT_V_MARGIN);
			if(pWnd != NULL) pWnd->EnableWindow(TRUE);
		}
	}
}

void CDlg_Print_Setup_SCH::OnCbnSelchangeComboScalemode()
{
	CWnd* pWnd;
	CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_SCALEMODE);
	if(pCombo != NULL)
	{
		if(pCombo->GetCurSel() == 0)
		{
			pWnd = GetDlgItem(IDC_EDIT_SCALE);
			if(pWnd != NULL) pWnd->EnableWindow(TRUE);
		}
		else
		{
			pWnd = GetDlgItem(IDC_EDIT_SCALE);
			if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		}
	}
}

void CDlg_Print_Setup_SCH::OnBnClickedRadioExtent()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO9);
	if(pButton != NULL) pButton->EnableWindow(TRUE);
}


void CDlg_Print_Setup_SCH::OnBnClickedRadio7()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_SHEET);
	if(pButton != NULL) pButton->SetCheck(BST_CHECKED);
	pButton = (CButton*)GetDlgItem(IDC_RADIO9);
	if(pButton != NULL)
	{
		pButton->SetCheck(BST_UNCHECKED);
		pButton->EnableWindow(FALSE);
	}
}
