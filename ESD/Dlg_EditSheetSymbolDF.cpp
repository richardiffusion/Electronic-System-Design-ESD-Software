// Dlg_EditSheetSymbolDF.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditSheetSymbolDF.h"
#include ".\dlg_editsheetsymboldf.h"
#include "Dlg_Font.h"


// CDlg_EditSheetSymbolDF 对话框

IMPLEMENT_DYNAMIC(CDlg_EditSheetSymbolDF, CDialog)
CDlg_EditSheetSymbolDF::CDlg_EditSheetSymbolDF(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditSheetSymbolDF::IDD, pParent)
	, m_csEditDF(_T(""))

	, m_nComboOrientation(0)
	, m_bCheckAutoPosition(FALSE)
	, m_bCheckHide(FALSE)
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
{
}

CDlg_EditSheetSymbolDF::~CDlg_EditSheetSymbolDF()
{
}

void CDlg_EditSheetSymbolDF::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DF, m_csEditDF);

	DDX_CBIndex(pDX, IDC_COMBO_ORIENTATION, m_nComboOrientation);
	DDX_Check(pDX, IDC_CHECK_AUTOPOSITION, m_bCheckAutoPosition);
	DDX_Check(pDX, IDC_CHECK_HIDE, m_bCheckHide);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
}


BEGIN_MESSAGE_MAP(CDlg_EditSheetSymbolDF, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, OnStnClickedStaticColor)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnBnClickedButtonFont)
END_MESSAGE_MAP()


// CDlg_EditSheetSymbolDF 消息处理程序

void CDlg_EditSheetSymbolDF::OnStnClickedStaticColor()
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

BOOL CDlg_EditSheetSymbolDF::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cColor.SubclassDlgItem(IDC_STATIC_COLOR, this);
	m_cColor.m_nColor = m_nColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int CDlg_EditSheetSymbolDF::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(m_csTitle);

	return 0;
}

void CDlg_EditSheetSymbolDF::OnBnClickedButtonFont()
{
	CDlg_Font dlg;
	
	dlg.m_bBold = m_bBold;	
	dlg.m_bItalic = m_bItalic;
	dlg.m_bUnderline = m_bUnderline;
	dlg.m_strFontName = m_csFontName;
	dlg.m_strFontSize = m_csFontSize;

	if(dlg.DoModal() == IDOK)
	{
		m_bBold = dlg.m_bBold;
		m_bItalic = dlg.m_bItalic;
		m_bUnderline = dlg.m_bUnderline;
		m_csFontName = dlg.m_strFontName;
		m_csFontSize = dlg.m_strFontSize;
	}
}
