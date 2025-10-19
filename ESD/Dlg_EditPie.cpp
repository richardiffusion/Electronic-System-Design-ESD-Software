// Dlg_EditPie.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditPie.h"
#include ".\dlg_editpie.h"


// CDlg_EditPie 对话框

IMPLEMENT_DYNAMIC(CDlg_EditPie, CDialog)
CDlg_EditPie::CDlg_EditPie(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditPie::IDD, pParent)
	, m_bCheckDrawSolid(FALSE)
	, m_nComboWidth(0)
	, m_fEditStartAngle(0)
	, m_fEditEndAngle(0)
	, m_csEditRadius(_T(""))
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditPie::~CDlg_EditPie()
{
}

void CDlg_EditPie::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_PIE, m_Bitmap);
	DDX_Check(pDX, IDC_CHECK_DRAWSOLID, m_bCheckDrawSolid);
	DDX_CBIndex(pDX, IDC_COMBO_BORDERWIDTH, m_nComboWidth);
	DDX_Text(pDX, IDC_EDIT_STARTANGLE, m_fEditStartAngle);
	DDX_Text(pDX, IDC_EDIT_ENDANGLE, m_fEditEndAngle);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_csEditRadius);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditPie, CDialog)
	ON_STN_CLICKED(IDC_STATIC_FILLCOLOR, OnStnClickedStaticFillcolor)
	ON_STN_CLICKED(IDC_STATIC_BORDERCOLOR, OnStnClickedStaticBordercolor)
END_MESSAGE_MAP()


// CDlg_EditPie 消息处理程序

BOOL CDlg_EditPie::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_PIE);

	m_cFillColor.SubclassDlgItem(IDC_STATIC_FILLCOLOR, this);
	m_cFillColor.m_nColor = m_nFillColor;
	m_cBorderColor.SubclassDlgItem(IDC_STATIC_BORDERCOLOR, this);
	m_cBorderColor.m_nColor = m_nBorderColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditPie::OnStnClickedStaticFillcolor()
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

void CDlg_EditPie::OnStnClickedStaticBordercolor()
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
