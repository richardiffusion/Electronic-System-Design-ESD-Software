// Dlg_EditCompText.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditCompText.h"
#include ".\dlg_editcomptext.h"
#include "dlg_font.h"

// CDlg_EditCompText 对话框

IMPLEMENT_DYNAMIC(CDlg_EditCompText, CDialog)
CDlg_EditCompText::CDlg_EditCompText(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditCompText::IDD, pParent)
	, m_csEditNameComment(_T(""))
	, m_csEditText(_T(""))
	, m_bCheckNameVisible(FALSE)
	, m_bCheckNameLocked(FALSE)
	, m_bCheckTextVisible(FALSE)
	, m_bCheckTextLocked(FALSE)
	, m_nComboOrientation(0)
	, m_bCheckAutoPosition(FALSE)
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
	, m_nComboHJustification(0)
	, m_nComboVJustification(0)
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditCompText::~CDlg_EditCompText()
{
}

void CDlg_EditCompText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_csEditNameComment);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_csEditText);
	DDX_Check(pDX, IDC_CHECK_NAMEVISIBLE, m_bCheckNameVisible);
	DDX_Check(pDX, IDC_CHECK_NAMELOCKED, m_bCheckNameLocked);
	DDX_Check(pDX, IDC_CHECK_TEXTVISIBLE, m_bCheckTextVisible);
	DDX_Check(pDX, IDC_CHECK_TEXTLOCKED, m_bCheckTextLocked);
	DDX_CBIndex(pDX, IDC_COMBO_ORIENTATION, m_nComboOrientation);
	DDX_Check(pDX, IDC_CHECK_AUTOPOSITION, m_bCheckAutoPosition);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
	DDX_CBIndex(pDX, IDC_COMBO_HJ, m_nComboHJustification);
	DDX_CBIndex(pDX, IDC_COMBO_VJ, m_nComboVJustification);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditCompText, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, OnStnClickedStaticColor)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnBnClickedButtonFont)
END_MESSAGE_MAP()


// CDlg_EditCompText 消息处理程序

void CDlg_EditCompText::OnStnClickedStaticColor()
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

BOOL CDlg_EditCompText::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cColor.SubclassDlgItem(IDC_STATIC_COLOR, this);
	m_cColor.m_nColor = m_nColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int CDlg_EditCompText::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetWindowText(m_csTitle);

	return 0;
}

void CDlg_EditCompText::OnBnClickedButtonFont()
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
