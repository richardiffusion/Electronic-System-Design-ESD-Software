// Dlg_EditRect.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditRect.h"
#include ".\dlg_editrect.h"


// CDlg_EditRect 对话框

IMPLEMENT_DYNAMIC(CDlg_EditRect, CDialog)
CDlg_EditRect::CDlg_EditRect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditRect::IDD, pParent)
	, m_nComboWidth(0)
	, m_bCheckDrawSolid(FALSE)
	, m_bCheckTransparent(FALSE)
	, m_csEditLocationX1(_T(""))
	, m_csEditLocationY1(_T(""))
	, m_csEditLocationX2(_T(""))
	, m_csEditLocationY2(_T(""))
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditRect::~CDlg_EditRect()
{
}

void CDlg_EditRect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_RECT, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_BORDERWIDTH, m_nComboWidth);
	DDX_Check(pDX, IDC_CHECK_DRAWSOLID, m_bCheckDrawSolid);
	DDX_Check(pDX, IDC_CHECK_TRANSPARENT, m_bCheckTransparent);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X1, m_csEditLocationX1);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y1, m_csEditLocationY1);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X2, m_csEditLocationX2);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y2, m_csEditLocationY2);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditRect, CDialog)
	ON_STN_CLICKED(IDC_STATIC_FILLCOLOR, OnStnClickedStaticFillcolor)
	ON_STN_CLICKED(IDC_STATIC_BORDERCOLOR, OnStnClickedStaticBordercolor)
END_MESSAGE_MAP()


// CDlg_EditRect 消息处理程序

BOOL CDlg_EditRect::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_RECT);

	m_cFillColor.SubclassDlgItem(IDC_STATIC_FILLCOLOR, this);
	m_cFillColor.m_nColor = m_nFillColor;
	m_cBorderColor.SubclassDlgItem(IDC_STATIC_BORDERCOLOR, this);
	m_cBorderColor.m_nColor = m_nBorderColor;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditRect::OnStnClickedStaticFillcolor()
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

void CDlg_EditRect::OnStnClickedStaticBordercolor()
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
