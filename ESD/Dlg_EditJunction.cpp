// Dlg_EditJunction.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditJunction.h"
#include ".\dlg_editjunction.h"


// CDlg_EditJunction 对话框

IMPLEMENT_DYNAMIC(CDlg_EditJunction, CDialog)
CDlg_EditJunction::CDlg_EditJunction(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditJunction::IDD, pParent)
	, m_nComboJunctionSize(0)
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditJunction::~CDlg_EditJunction()
{
}

void CDlg_EditJunction::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_JUNCTION, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_JUNCTIONSIZE, m_nComboJunctionSize);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditJunction, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, OnStnClickedStaticColor)
END_MESSAGE_MAP()


// CDlg_EditJunction 消息处理程序

BOOL CDlg_EditJunction::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_JUNCTION);
		
	m_cColor.SubclassDlgItem(IDC_STATIC_COLOR, this);
	m_cColor.m_nColor = m_nColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditJunction::OnStnClickedStaticColor()
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
