// Dlg_EditBezier.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditBezier.h"
#include ".\dlg_editbezier.h"


// CDlg_EditBezier 对话框

IMPLEMENT_DYNAMIC(CDlg_EditBezier, CDialog)
CDlg_EditBezier::CDlg_EditBezier(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditBezier::IDD, pParent)
	, m_nComboWidth(0)
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditBezier::~CDlg_EditBezier()
{
}

void CDlg_EditBezier::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_BEZIER, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_WIDTH, m_nComboWidth);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditBezier, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, OnStnClickedStaticColor)
END_MESSAGE_MAP()


// CDlg_EditBezier 消息处理程序

BOOL CDlg_EditBezier::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_BEZIER);
	
	m_cColor.SubclassDlgItem(IDC_STATIC_COLOR, this);
	m_cColor.m_nColor = m_nColor;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditBezier::OnStnClickedStaticColor()
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
