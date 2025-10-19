// Dlg_EditOffSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditOffSheet.h"
#include ".\dlg_editoffsheet.h"


// CDlg_EditOffSheet 对话框

IMPLEMENT_DYNAMIC(CDlg_EditOffSheet, CDialog)
CDlg_EditOffSheet::CDlg_EditOffSheet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditOffSheet::IDD, pParent)
	, m_nComboStyle(0)
	, m_nComboOrientation(0)
	, m_csEditNet(_T(""))
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditOffSheet::~CDlg_EditOffSheet()
{
}

void CDlg_EditOffSheet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_OFFSHEET, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_STYLE, m_nComboStyle);
	DDX_CBIndex(pDX, IDC_COMBO_ORIENTATION, m_nComboOrientation);
	DDX_Text(pDX, IDC_EDIT_NET, m_csEditNet);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditOffSheet, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, OnStnClickedStaticColor)
END_MESSAGE_MAP()


// CDlg_EditOffSheet 消息处理程序

BOOL CDlg_EditOffSheet::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_OFFSHEET);
	
	m_cColor.SubclassDlgItem(IDC_STATIC_COLOR, this);
	m_cColor.m_nColor = m_nColor;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditOffSheet::OnStnClickedStaticColor()
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
