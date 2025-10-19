// Dlg_EditText.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditText.h"
#include ".\dlg_edittext.h"
#include "Dlg_Font.h"
#include "systemext.h"


// CDlg_EditText 对话框

IMPLEMENT_DYNAMIC(CDlg_EditText, CDialog)
CDlg_EditText::CDlg_EditText(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditText::IDD, pParent)
	, m_csEditText(_T(""))

	, m_nComboOrientation(0)
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
	, m_nComboHJustification(0)
	, m_nComboVJustification(0)
	, m_bCheckMirror(FALSE)
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditText::~CDlg_EditText()
{
	
}

void CDlg_EditText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_TEXT, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_csEditText);

	DDX_CBIndex(pDX, IDC_COMBO_ORIENTATION, m_nComboOrientation);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
	DDX_CBIndex(pDX, IDC_COMBO_HJ, m_nComboHJustification);
	DDX_CBIndex(pDX, IDC_COMBO_VJ, m_nComboVJustification);
	DDX_Check(pDX, IDC_CHECK_MIRROR, m_bCheckMirror);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditText, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, OnStnClickedStaticColor)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnBnClickedButtonFont)
END_MESSAGE_MAP()


// CDlg_EditText 消息处理程序

BOOL CDlg_EditText::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_TEXT);
	
	m_cColor.SubclassDlgItem(IDC_STATIC_COLOR, this);
	m_cColor.m_nColor = m_nColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditText::OnStnClickedStaticColor()
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

void CDlg_EditText::OnBnClickedButtonFont()
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
