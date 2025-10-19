// Dlg_EditRoundrect.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditRoundrect.h"
#include ".\dlg_editroundrect.h"


// CDlg_EditRoundrect 对话框

IMPLEMENT_DYNAMIC(CDlg_EditRoundrect, CDialog)
CDlg_EditRoundrect::CDlg_EditRoundrect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditRoundrect::IDD, pParent)
	, m_nComboWidth(0)
	, m_bCheckDrawSolid(FALSE)
	, m_csEditLocationX1(_T(""))
	, m_csEditLocationY1(_T(""))
	, m_csEditLocationX2(_T(""))
	, m_csEditLocationY2(_T(""))
	, m_csEditRadiusX(_T(""))
	, m_csEditRadiusY(_T(""))
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditRoundrect::~CDlg_EditRoundrect()
{
}

void CDlg_EditRoundrect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_ROUNDRECT, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_BORDERWIDTH, m_nComboWidth);
	DDX_Check(pDX, IDC_CHECK_DRAWSOLID, m_bCheckDrawSolid);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X1, m_csEditLocationX1);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y1, m_csEditLocationY1);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X2, m_csEditLocationX2);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y2, m_csEditLocationY2);
	DDX_Text(pDX, IDC_EDIT_RADIUS_X, m_csEditRadiusX);
	DDX_Text(pDX, IDC_EDIT_RADIUS_Y, m_csEditRadiusY);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditRoundrect, CDialog)
	ON_STN_CLICKED(IDC_STATIC_FILLCOLOR, OnStnClickedStaticFillcolor)
	ON_STN_CLICKED(IDC_STATIC_BORDERCOLOR, OnStnClickedStaticBordercolor)
END_MESSAGE_MAP()


// CDlg_EditRoundrect 消息处理程序

BOOL CDlg_EditRoundrect::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_ROUNDRECT);

	m_cFillColor.SubclassDlgItem(IDC_STATIC_FILLCOLOR, this);
	m_cFillColor.m_nColor = m_nFillColor;
	m_cBorderColor.SubclassDlgItem(IDC_STATIC_BORDERCOLOR, this);
	m_cBorderColor.m_nColor = m_nBorderColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditRoundrect::OnStnClickedStaticFillcolor()
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

void CDlg_EditRoundrect::OnStnClickedStaticBordercolor()
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
