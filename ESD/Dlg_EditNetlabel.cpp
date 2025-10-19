// Dlg_EditNetlabel.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditNetlabel.h"
#include ".\dlg_editnetlabel.h"
#include "Dlg_Font.h"


// CDlg_EditNetlabel 对话框

IMPLEMENT_DYNAMIC(CDlg_EditNetlabel, CDialog)
CDlg_EditNetlabel::CDlg_EditNetlabel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditNetlabel::IDD, pParent)
	, m_nComboOrientation(0)
	, m_csEditNet(_T(""))
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditNetlabel::~CDlg_EditNetlabel()
{
}

void CDlg_EditNetlabel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_NETLABEL, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_ORIENTATION, m_nComboOrientation);

	DDX_Text(pDX, IDC_EDIT_NET, m_csEditNet);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditNetlabel, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, OnStnClickedStaticColor)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnBnClickedButtonFont)
END_MESSAGE_MAP()


// CDlg_EditNetlabel 消息处理程序

BOOL CDlg_EditNetlabel::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_NETLABEL);
	
	m_cColor.SubclassDlgItem(IDC_STATIC_COLOR, this);
	m_cColor.m_nColor = m_nColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditNetlabel::OnStnClickedStaticColor()
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

void CDlg_EditNetlabel::OnBnClickedButtonFont()
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
