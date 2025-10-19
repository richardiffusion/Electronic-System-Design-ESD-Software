// Dlg_EditArc.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditArc.h"
#include ".\dlg_editarc.h"


// CDlg_EditArc 对话框

IMPLEMENT_DYNAMIC(CDlg_EditArc, CDialog)
CDlg_EditArc::CDlg_EditArc(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditArc::IDD, pParent)
	, m_fEditStartAngle(0)
	, m_fEditEndAngle(0)
	, m_nComboWidth(0)
	, m_csEditRadius(_T(""))
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditArc::~CDlg_EditArc()
{
}

void CDlg_EditArc::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_ARC, m_Bitmap);
	DDX_Text(pDX, IDC_EDIT_STARTANGLE, m_fEditStartAngle);
	DDX_Text(pDX, IDC_EDIT_ENDANGLE, m_fEditEndAngle);
	DDX_CBIndex(pDX, IDC_COMBO_LINEWIDTH, m_nComboWidth);
	DDX_Text(pDX, IDC_EDIT_RADIUS, m_csEditRadius);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditArc, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, OnStnClickedStaticColor)
END_MESSAGE_MAP()


// CDlg_EditArc 消息处理程序

BOOL CDlg_EditArc::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_ARC);

	m_cColor.SubclassDlgItem(IDC_STATIC_COLOR, this);
	m_cColor.m_nColor = m_nColor;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditArc::OnStnClickedStaticColor()
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

