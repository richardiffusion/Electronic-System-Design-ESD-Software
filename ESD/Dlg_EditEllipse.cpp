// Dlg_EditEllipticalArc.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditEllipse.h"
#include ".\dlg_editellipse.h"


// CDlg_EditEllipse 对话框

IMPLEMENT_DYNAMIC(CDlg_EditEllipse, CDialog)
CDlg_EditEllipse::CDlg_EditEllipse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditEllipse::IDD, pParent)
	, m_nComboWidth(0)
	, m_fEditStartAngle(0)
	, m_fEditEndAngle(0)

	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
	, m_csEditRadiusX(_T(""))
	, m_csEditRadiusY(_T(""))
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditEllipse::~CDlg_EditEllipse()
{
}

void CDlg_EditEllipse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_ELLIPSE, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_BORDERWIDTH, m_nComboWidth);
	DDX_Text(pDX, IDC_EDIT_STARTANGLE, m_fEditStartAngle);
	DDX_Text(pDX, IDC_EDIT_ENDANGLE, m_fEditEndAngle);

	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
	DDX_Text(pDX, IDC_EDIT_RADIUS_X, m_csEditRadiusX);
	DDX_Text(pDX, IDC_EDIT_RADIUS_Y, m_csEditRadiusY);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditEllipse, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, OnStnClickedStaticColor)
END_MESSAGE_MAP()


// CDlg_EditEllipse 消息处理程序

BOOL CDlg_EditEllipse::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_ELLIPSE);
	
	m_cColor.SubclassDlgItem(IDC_STATIC_COLOR, this);
	m_cColor.m_nColor = m_nColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditEllipse::OnStnClickedStaticColor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cColor.m_nColor = colordialog.GetColor();
		m_nColor = m_cColor.m_nColor;
		m_cColor.Invalidate();
	}}
