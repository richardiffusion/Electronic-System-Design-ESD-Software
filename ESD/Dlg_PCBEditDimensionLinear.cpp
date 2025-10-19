// CDlg_PCBEditDimensionLinear.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PCBEditDimensionLinear.h"
#include ".\dlg_pcbeditdimensionlinear.h"
#include "systemext.h"

// CDlg_PCBEditDimensionLinear 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBEditDimensionLinear, CDialog)
CDlg_PCBEditDimensionLinear::CDlg_PCBEditDimensionLinear(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBEditDimensionLinear::IDD, pParent)
	, m_csEditArrowLength(_T(""))
	, m_csEditGap(_T(""))
	, m_csEditArrowWidth(_T(""))
	, m_csEditTextHeight(_T(""))
	, m_csEditTextWidth(_T(""))
	, m_csEditTextGap(_T(""))
	, m_csEditExtWidth(_T(""))
	, m_csEditLineWidth(_T(""))
	, m_csEditOffset(_T(""))
	, m_fEditRotation(0)
	, m_nComboUnit(0)
	, m_nComboPrecision(0)
	, m_csEditPrefix(_T(""))
	, m_csEditSuffix(_T(""))
	, m_nComboTextPosition(0)
	, m_nComboArrowPosition(0)
	, m_bCheckLocked(FALSE)
	, m_nComboFont(0)
	, m_bCheckBold(FALSE)
	, m_bCheckItalic(FALSE)
	, m_nComboLayer(0)
	, m_nComboFormat(0)
{
	m_nRadioTrueType = 0;
	m_csStrokeFontName[0] = "Default";
	m_csStrokeFontName[1] = "Sans Serif";
	m_csStrokeFontName[2] = "Serif";
}

CDlg_PCBEditDimensionLinear::~CDlg_PCBEditDimensionLinear()
{
}

void CDlg_PCBEditDimensionLinear::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_EDIT_DIMENSION_LINEAR, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT_ARROW_LENGTH, m_csEditArrowLength);
	DDX_Text(pDX, IDC_EDIT_GAP, m_csEditGap);
	DDX_Text(pDX, IDC_EDIT_ARROWWIDTH, m_csEditArrowWidth);
	DDX_Text(pDX, IDC_EDIT_TEXT_HEIGHT, m_csEditTextHeight);
	DDX_Text(pDX, IDC_EDIT_TEXT_WIDTH, m_csEditTextWidth);
	DDX_Text(pDX, IDC_EDIT_TEXT_GAP, m_csEditTextGap);
	DDX_Text(pDX, IDC_EDIT_EXTWIDTH, m_csEditExtWidth);
	DDX_Text(pDX, IDC_EDIT_LINE_WIDTH, m_csEditLineWidth);
	DDX_Text(pDX, IDC_EDIT_OFFSET, m_csEditOffset);
	DDX_Text(pDX, IDC_EDIT_ANGLE, m_fEditRotation);
	DDX_CBIndex(pDX, IDC_COMBO_SIZEUNIT, m_nComboUnit);
	DDX_CBIndex(pDX, IDC_COMBO_PRECISION, m_nComboPrecision);
	DDX_Text(pDX, IDC_EDIT_PREFIX, m_csEditPrefix);
	DDX_Text(pDX, IDC_EDIT_SUFFIXX, m_csEditSuffix);
	DDX_CBIndex(pDX, IDC_COMBO_TEXT_POSITION, m_nComboTextPosition);
	DDX_CBIndex(pDX, IDC_COMBO_ARROW_POSITION, m_nComboArrowPosition);
	DDX_Check(pDX, IDC_CHECK_LOCKED, m_bCheckLocked);
	DDX_CBIndex(pDX, IDC_COMBO_FONT, m_nComboFont);
	DDX_Check(pDX, IDC_CHECK_BOLD, m_bCheckBold);
	DDX_Check(pDX, IDC_CHECK_ITALIC, m_bCheckItalic);
	DDX_CBIndex(pDX, IDC_COMBO_LAYER, m_nComboLayer);
	DDX_Radio(pDX, IDC_RADIO_TRUETYPE, m_nRadioTrueType);
	DDX_CBIndex(pDX, IDC_COMBO_FORMAT, m_nComboFormat);
}


BEGIN_MESSAGE_MAP(CDlg_PCBEditDimensionLinear, CDialog)
	ON_BN_CLICKED(IDC_RADIO_STROKE, OnBnClickedRadioStroke)
	ON_BN_CLICKED(IDC_RADIO_TRUETYPE, OnBnClickedRadioTruetype)
END_MESSAGE_MAP()


// CDlg_PCBEditDimensionLinear 消息处理程序

BOOL CDlg_PCBEditDimensionLinear::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_PCB_EDIT_DIMENSION_LINEAR);

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
		//字型名称
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
		//字型名称
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
		//控件
		pWnd = GetDlgItem(IDC_CHECK_BOLD);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
		pWnd = GetDlgItem(IDC_CHECK_ITALIC);
		if(pWnd != NULL) pWnd->ShowWindow(SW_HIDE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CDlg_PCBEditDimensionLinear::OnBnClickedRadioStroke()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_STROKE);
	
	CWnd* pWnd;
	if(pButton != NULL) 
	{
		if(pButton->GetCheck() == BST_CHECKED)
		{
			//字型名称
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

void CDlg_PCBEditDimensionLinear::OnBnClickedRadioTruetype()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_TRUETYPE);
	
	CWnd* pWnd;
	if(pButton != NULL) 
	{
		if(pButton->GetCheck() == BST_CHECKED)
		{
			//字型名称
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
			//控件
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
