// Dlg_EditTextFrame.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditTextFrame.h"
#include ".\dlg_edittextframe.h"
#include "dlg_font.h"
#include "dlg_textframe_text.h"

// CDlg_EditTextFrame 对话框

IMPLEMENT_DYNAMIC(CDlg_EditTextFrame, CDialog)
CDlg_EditTextFrame::CDlg_EditTextFrame(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditTextFrame::IDD, pParent)
	, m_nComboWidth(0)
	, m_nComboAlignment(0)
	, m_bCheckShowBorder(FALSE)
	, m_bCheckDrawSolid(FALSE)
	, m_bCheckWordWrap(FALSE)
	, m_bCheckClip(FALSE)
	, m_csEditLocationX1(_T(""))
	, m_csEditLocationY1(_T(""))
	, m_csEditLocationX2(_T(""))
	, m_csEditLocationY2(_T(""))
{
}

CDlg_EditTextFrame::~CDlg_EditTextFrame()
{
}

void CDlg_EditTextFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_TEXTFRAME, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_BORDERWIDTH, m_nComboWidth);
	DDX_CBIndex(pDX, IDC_COMBO_ALIGNMENT, m_nComboAlignment);
	DDX_Check(pDX, IDC_CHECK_SHOWBORDER, m_bCheckShowBorder);
	DDX_Check(pDX, IDC_CHECK_DRAWSOLID, m_bCheckDrawSolid);
	DDX_Check(pDX, IDC_CHECK_WORDWRAP, m_bCheckWordWrap);
	DDX_Check(pDX, IDC_CHECK_CLIP, m_bCheckClip);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X1, m_csEditLocationX1);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y1, m_csEditLocationY1);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X2, m_csEditLocationX2);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y2, m_csEditLocationY2);
}


BEGIN_MESSAGE_MAP(CDlg_EditTextFrame, CDialog)
	ON_STN_CLICKED(IDC_STATIC_BORDERCOLOR, OnStnClickedStaticBordercolor)
	ON_STN_CLICKED(IDC_STATIC_FILLCOLOR, OnStnClickedStaticFillcolor)
	ON_STN_CLICKED(IDC_STATIC_TEXTCOLOR, OnStnClickedStaticTextcolor)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnBnClickedButtonFont)
	ON_BN_CLICKED(IDC_BUTTON_TEXT, OnBnClickedButtonText)
END_MESSAGE_MAP()


// CDlg_EditTextFrame 消息处理程序

BOOL CDlg_EditTextFrame::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_TEXTFRAME);

	m_cFillColor.SubclassDlgItem(IDC_STATIC_FILLCOLOR, this);
	m_cFillColor.m_nColor = m_nFillColor;
	m_cBorderColor.SubclassDlgItem(IDC_STATIC_BORDERCOLOR, this);
	m_cBorderColor.m_nColor = m_nBorderColor;
	m_cTextColor.SubclassDlgItem(IDC_STATIC_TEXTCOLOR, this);
	m_cTextColor.m_nColor = m_nTextColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditTextFrame::OnStnClickedStaticBordercolor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cBorderColor.m_nColor = colordialog.GetColor();
		m_nBorderColor = m_cBorderColor.m_nColor;
		m_cBorderColor.Invalidate();
	}
}

void CDlg_EditTextFrame::OnStnClickedStaticFillcolor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cFillColor.m_nColor = colordialog.GetColor();
		m_nFillColor = m_cFillColor.m_nColor;
		m_cFillColor.Invalidate();
	}
}

void CDlg_EditTextFrame::OnStnClickedStaticTextcolor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cTextColor.m_nColor = colordialog.GetColor();
		m_nTextColor = m_cTextColor.m_nColor;
		m_cTextColor.Invalidate();
	}
}

void CDlg_EditTextFrame::OnBnClickedButtonFont()
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

void CDlg_EditTextFrame::OnBnClickedButtonText()
{
	CDlg_TextFrame_Text dlg;

	dlg.m_csEditTextFrameText = m_csText;

	if(dlg.DoModal() == IDOK)
	{
		m_csText = dlg.m_csEditTextFrameText;
	}
}
