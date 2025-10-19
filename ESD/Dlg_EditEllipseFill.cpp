// Dlg_EditEllipse.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditEllipseFill.h"
#include ".\dlg_editellipsefill.h"


// CDlg_EditEllipseFill 对话框

IMPLEMENT_DYNAMIC(CDlg_EditEllipseFill, CDialog)
CDlg_EditEllipseFill::CDlg_EditEllipseFill(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditEllipseFill::IDD, pParent)
	, m_nComboWidth(0)
	, m_bCheckDrawSolid(FALSE)
	, m_bCheckTransparent(FALSE)
	, m_csEditRadiusX(_T(""))
	, m_csEditRadiusY(_T(""))
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditEllipseFill::~CDlg_EditEllipseFill()
{
}

void CDlg_EditEllipseFill::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_ELLIPSEFILL, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_BORDERWIDTH, m_nComboWidth);
	DDX_Check(pDX, IDC_CHECK_DRAWSOLID, m_bCheckDrawSolid);
	DDX_Check(pDX, IDC_CHECK_TRANSPARENT, m_bCheckTransparent);
	DDX_Text(pDX, IDC_EDIT_RADIUS_X, m_csEditRadiusX);
	DDX_Text(pDX, IDC_EDIT_RADIUS_Y, m_csEditRadiusY);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditEllipseFill, CDialog)
	ON_STN_CLICKED(IDC_STATIC_FILLCOLOR, OnStnClickedStaticFillcolor)
	ON_STN_CLICKED(IDC_STATIC_BORDERCOLOR, OnStnClickedStaticBordercolor)
END_MESSAGE_MAP()


// CDlg_EditEllipseFill 消息处理程序

BOOL CDlg_EditEllipseFill::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_ELLIPSEFILL);

	m_cFillColor.SubclassDlgItem(IDC_STATIC_FILLCOLOR, this);
	m_cFillColor.m_nColor = m_nFillColor;
	m_cBorderColor.SubclassDlgItem(IDC_STATIC_BORDERCOLOR, this);
	m_cBorderColor.m_nColor = m_nBorderColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditEllipseFill::OnStnClickedStaticFillcolor()
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

void CDlg_EditEllipseFill::OnStnClickedStaticBordercolor()
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
