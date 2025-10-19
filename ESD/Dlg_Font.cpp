// Dlg_Font.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "systemext.h"
#include "Dlg_Font.h"
#include ".\dlg_font.h"


// CDlg_Font 对话框

IMPLEMENT_DYNAMIC(CDlg_Font, CDialog)
CDlg_Font::CDlg_Font(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Font::IDD, pParent)
{
	m_strFontName = csEDA_Font_FontName;
	m_strFontSize = csEDA_Font_FontSize;
	m_bBold = bEDA_Font_Bold;
	m_bItalic = bEDA_Font_Italic;
	m_bUnderline = bEDA_Font_Underline;
}

CDlg_Font::~CDlg_Font()
{
	csEDA_Font_FontName = m_strFontName;
	csEDA_Font_FontSize = m_strFontSize;
	bEDA_Font_Bold = m_bBold;
	bEDA_Font_Italic = m_bItalic;
	bEDA_Font_Underline = m_bUnderline;
}

void CDlg_Font::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_FONT_SAMPLE, m_txtSample);
	DDX_Control(pDX, IDC_FONT_SIZE, m_wndComboSize);
	DDX_Control(pDX, IDC_FONT_FONT, m_wndComboFont);
	DDX_Check(pDX, IDC_FONT_BOLD, m_bBold);
	DDX_Check(pDX, IDC_FONT_ITALIC, m_bItalic);
	DDX_Check(pDX, IDC_FONT_UNDERLINE, m_bUnderline);
}


BEGIN_MESSAGE_MAP(CDlg_Font, CDialog)
	ON_BN_CLICKED(IDC_FONT_UNDERLINE, OnBnClickedFontUnderline)
	ON_BN_CLICKED(IDC_FONT_ITALIC, OnBnClickedFontItalic)
	ON_BN_CLICKED(IDC_FONT_BOLD, OnBnClickedFontBold)
	ON_CBN_SELENDOK(IDC_FONT_SIZE, OnCbnSelendokFontSize)
	ON_CBN_SELENDOK(IDC_FONT_FONT, OnCbnSelendokFontFont)
END_MESSAGE_MAP()


// CDlg_Font 消息处理程序

BOOL CDlg_Font::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndComboFont.InitControl(m_strFontName, 285);
	
	// set the selection.
	if (m_wndComboSize.SelectString(-1, m_strFontSize) == CB_ERR)
	{
		m_wndComboSize.SetWindowText(m_strFontSize);
	}

	UpdateFont();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_Font::UpdateFont()
{
	UpdateData();
	m_wndComboFont.GetSelFont(m_lf);
	m_strFontName = m_lf.lfFaceName;

	int iSel = m_wndComboSize.GetCurSel();
	if (iSel != CB_ERR)
	{
		m_wndComboSize.GetLBText(iSel, m_strFontSize);
		m_lf.lfHeight = -(_ttoi(m_strFontSize));
	}
	else
	{
		m_wndComboSize.GetWindowText(m_strFontSize);
		m_lf.lfHeight = -(_ttoi(m_strFontSize));
	}

	m_lf.lfWeight    = m_bBold      ? FW_BOLD : FW_NORMAL;
	m_lf.lfItalic    = m_bItalic    ? (BYTE)1 : (BYTE)0;
	m_lf.lfUnderline = m_bUnderline ? (BYTE)1 : (BYTE)0;

	//m_txtSample.UpdateFont(m_lf, m_crCurrent);
	m_txtSample.UpdateFont(m_lf, RGB(0,0,0));
}

void CDlg_Font::OnBnClickedFontUnderline()
{
	UpdateFont();
}

void CDlg_Font::OnBnClickedFontItalic()
{
	UpdateFont();
}

void CDlg_Font::OnBnClickedFontBold()
{
	UpdateFont();
}

void CDlg_Font::OnCbnSelendokFontSize()
{
	UpdateFont();
}

void CDlg_Font::OnCbnSelendokFontFont()
{
	UpdateFont();
}
