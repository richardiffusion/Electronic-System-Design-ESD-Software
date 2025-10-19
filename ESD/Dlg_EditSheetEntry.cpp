// Dlg_EditSheetEntry.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditSheetEntry.h"
#include ".\dlg_editsheetentry.h"


// CDlg_EditSheetEntry 对话框

IMPLEMENT_DYNAMIC(CDlg_EditSheetEntry, CDialog)
CDlg_EditSheetEntry::CDlg_EditSheetEntry(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditSheetEntry::IDD, pParent)
	, m_nComboStyle(0)
	, m_nComboSide(0)
	, m_csEditName(_T(""))
	, m_nComboType(0)
	, m_csEditPosition(_T(""))
{
}

CDlg_EditSheetEntry::~CDlg_EditSheetEntry()
{
}

void CDlg_EditSheetEntry::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_SHEETENTRY, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_STYLE, m_nComboStyle);
	DDX_CBIndex(pDX, IDC_COMBO_SIDE, m_nComboSide);
	DDX_Text(pDX, IDC_EDIT_NAME, m_csEditName);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nComboType);
	DDX_Text(pDX, IDC_EDIT_POSITION, m_csEditPosition);
}


BEGIN_MESSAGE_MAP(CDlg_EditSheetEntry, CDialog)
	ON_STN_CLICKED(IDC_STATIC_TEXTCOLOR, OnStnClickedStaticTextcolor)
	ON_STN_CLICKED(IDC_STATIC_FILLCOLOR, OnStnClickedStaticFillcolor)
	ON_STN_CLICKED(IDC_STATIC_BORDERCOLOR, OnStnClickedStaticBordercolor)
END_MESSAGE_MAP()


// CDlg_EditSheetEntry 消息处理程序

BOOL CDlg_EditSheetEntry::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_SHEETENTRY);

	m_cFillColor.SubclassDlgItem(IDC_STATIC_FILLCOLOR, this);
	m_cFillColor.m_nColor = m_nFillColor;
	m_cBorderColor.SubclassDlgItem(IDC_STATIC_BORDERCOLOR, this);
	m_cBorderColor.m_nColor = m_nBorderColor;
	m_cTextColor.SubclassDlgItem(IDC_STATIC_TEXTCOLOR, this);
	m_cTextColor.m_nColor = m_nTextColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditSheetEntry::OnStnClickedStaticTextcolor()
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

void CDlg_EditSheetEntry::OnStnClickedStaticFillcolor()
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

void CDlg_EditSheetEntry::OnStnClickedStaticBordercolor()
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
