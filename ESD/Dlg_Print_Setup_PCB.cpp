// Dlg_Print_Setup_PCB.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "sysparam.h"
#include "Dlg_Print_Setup_PCB.h"
#include ".\dlg_print_setup_pcb.h"


// CDlg_Print_Setup_PCB 对话框

IMPLEMENT_DYNAMIC(CDlg_Print_Setup_PCB, CDialog)
CDlg_Print_Setup_PCB::CDlg_Print_Setup_PCB(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Print_Setup_PCB::IDD, pParent)
	, m_nComboPaperSize(0)
	, m_nComboScaleMode(0)
	, m_fEditScale(0)
	, m_fEditHMargin(0)
	, m_fEditVMargin(0)
	, m_bCheckHCenter(FALSE)
	, m_bCheckVCenter(FALSE)
	, m_csEditCorrectionX(_T(""))
	, m_csEditCorrectionY(_T(""))
	, m_csEditX1(_T(""))
	, m_csEditY1(_T(""))
	, m_csEditX2(_T(""))
	, m_csEditY2(_T(""))
{
	m_nRadioOrientation = 0;
	m_csEditCorrectionX = "1.00";
	m_csEditCorrectionY = "1.00";
	m_nCurrentPaperSizeNo = 0;
	m_nTotalPaper = 0;
}

CDlg_Print_Setup_PCB::~CDlg_Print_Setup_PCB()
{
}

void CDlg_Print_Setup_PCB::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PRINT_PORTRAIT, m_Bitmap1);
	DDX_Control(pDX, IDC_STATIC_PRINT_LANDSCAPE, m_Bitmap2);
	DDX_Control(pDX, IDC_STATIC_SHEET, m_BitmapSheet);
	DDX_Control(pDX, IDC_STATIC_BOARD, m_BitmapBoard);
	DDX_CBIndex(pDX, IDC_COMBO_PAPERSIZE, m_nComboPaperSize);
	DDX_CBIndex(pDX, IDC_COMBO_SCALEMODE, m_nComboScaleMode);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_fEditScale);
	DDX_Text(pDX, IDC_EDIT_H_MARGIN, m_fEditHMargin);
	DDX_Text(pDX, IDC_EDIT_V_MARGIN, m_fEditVMargin);
	DDX_Check(pDX, IDC_CHECK_H_CENTER, m_bCheckHCenter);
	DDX_Check(pDX, IDC_CHECK_V_CENTER, m_bCheckVCenter);
	DDX_Text(pDX, IDC_EDIT_CORRECTION_X, m_csEditCorrectionX);
	DDX_Text(pDX, IDC_EDIT_CORRECTION_Y, m_csEditCorrectionY);
	DDX_Radio(pDX, IDC_RADIO_ORIENTATION, m_nRadioOrientation);
	DDX_Radio(pDX, IDC_RADIO_COLOR, m_nRadioColor);
	DDX_Radio(pDX, IDC_RADIO_EXTENT, m_nRadioExtent);
	DDX_Text(pDX, IDC_EDIT_XX1, m_csEditX1);
	DDX_Text(pDX, IDC_EDIT_YY1, m_csEditY1);
	DDX_Text(pDX, IDC_EDIT_XX2, m_csEditX2);
	DDX_Text(pDX, IDC_EDIT_YY2, m_csEditY2);
}


BEGIN_MESSAGE_MAP(CDlg_Print_Setup_PCB, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_PRINTER, OnBnClickedButtonSetupPrinter)
	ON_BN_CLICKED(IDC_BUTTON_SETUP_ADVANCED, OnBnClickedButtonSetupAdvanced)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_PREVIEW, OnBnClickedButtonPrintPreview)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnBnClickedButtonPrint)
	ON_BN_CLICKED(IDC_CHECK_H_CENTER, OnBnClickedCheckHCenter)
	ON_BN_CLICKED(IDC_CHECK_V_CENTER, OnBnClickedCheckVCenter)
	ON_CBN_SELCHANGE(IDC_COMBO_SCALEMODE, OnCbnSelchangeComboScalemode)
	ON_BN_CLICKED(IDC_BUTTON_DEFINE, OnBnClickedButtonDefine)
	ON_BN_CLICKED(IDC_RADIO_XY, OnBnClickedRadioXy)
	ON_BN_CLICKED(IDC_RADIO_BOARD, OnBnClickedRadioBoard)
	ON_BN_CLICKED(IDC_RADIO_EXTENT, OnBnClickedRadioExtent)
END_MESSAGE_MAP()


// CDlg_Print_Setup_PCB 消息处理程序

BOOL CDlg_Print_Setup_PCB::OnInitDialog()
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
	m_BitmapBoard.SetBitmap(IDB_PRINT_BOARD);
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

	if(m_nRadioExtent != 2)
	{
		pWnd = GetDlgItem(IDC_STATIC_XX1);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_YY1);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_XX2);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_YY2);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);

		pWnd = GetDlgItem(IDC_EDIT_XX1);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_YY1);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_XX2);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_EDIT_YY2);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
		pWnd = GetDlgItem(IDC_BUTTON_DEFINE);
		if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_Print_Setup_PCB::OnBnClickedButtonSetupPrinter()
{
	UpdateData(TRUE);
	EndDialog(MYID_PRINT_SETUP);
}

void CDlg_Print_Setup_PCB::OnBnClickedButtonSetupAdvanced()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlg_Print_Setup_PCB::OnBnClickedButtonPrintPreview()
{
	UpdateData(TRUE);
	EndDialog(MYID_PRINT_PREVIEW);
}

void CDlg_Print_Setup_PCB::OnBnClickedButtonPrint()
{
	UpdateData(TRUE);
	EndDialog(MYID_PRINT);
}

void CDlg_Print_Setup_PCB::OnBnClickedCheckHCenter()
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

void CDlg_Print_Setup_PCB::OnBnClickedCheckVCenter()
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

void CDlg_Print_Setup_PCB::OnCbnSelchangeComboScalemode()
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

void CDlg_Print_Setup_PCB::OnBnClickedButtonDefine()
{
	UpdateData(TRUE);
	EndDialog(MYID_PRINT_DEFINE);
}

void CDlg_Print_Setup_PCB::OnBnClickedRadioXy()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_STATIC_XX1);
	if(pWnd != NULL) pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_YY1);
	if(pWnd != NULL) pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_XX2);
	if(pWnd != NULL) pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_YY2);
	if(pWnd != NULL) pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_EDIT_XX1);
	if(pWnd != NULL) pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_YY1);
	if(pWnd != NULL) pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_XX2);
	if(pWnd != NULL) pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_EDIT_YY2);
	if(pWnd != NULL) pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_BUTTON_DEFINE);
	if(pWnd != NULL) pWnd->EnableWindow(TRUE);
}

void CDlg_Print_Setup_PCB::OnBnClickedRadioBoard()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_STATIC_XX1);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_YY1);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_XX2);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_YY2);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_EDIT_XX1);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_YY1);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_XX2);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_YY2);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_BUTTON_DEFINE);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);
}

void CDlg_Print_Setup_PCB::OnBnClickedRadioExtent()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_STATIC_XX1);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_YY1);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_XX2);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_YY2);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_EDIT_XX1);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_YY1);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_XX2);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_EDIT_YY2);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_BUTTON_DEFINE);
	if(pWnd != NULL) pWnd->EnableWindow(FALSE);
}
