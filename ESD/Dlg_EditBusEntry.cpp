// Dlg_EditBusEntry.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditBusEntry.h"
#include ".\dlg_editbusentry.h"


// CDlg_EditBusEntry 对话框

IMPLEMENT_DYNAMIC(CDlg_EditBusEntry, CDialog)
CDlg_EditBusEntry::CDlg_EditBusEntry(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditBusEntry::IDD, pParent)
	, m_nComboWidth(0)
	, m_csEditLocationX1(_T(""))
	, m_csEditLocationY1(_T(""))
	, m_csEditLocationX2(_T(""))
	, m_csEditLocationY2(_T(""))
	, m_bCheckGlobal(FALSE)
{
}

CDlg_EditBusEntry::~CDlg_EditBusEntry()
{
}

void CDlg_EditBusEntry::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_BUSENTRY, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_WIDTH, m_nComboWidth);

	DDX_Text(pDX, IDC_EDIT_LOCATION_X1, m_csEditLocationX1);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y1, m_csEditLocationY1);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X2, m_csEditLocationX2);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y2, m_csEditLocationY2);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CDlg_EditBusEntry, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, OnStnClickedStaticColor)
END_MESSAGE_MAP()


// CDlg_EditBusEntry 消息处理程序

BOOL CDlg_EditBusEntry::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_BUSENTRY);

	m_cColor.SubclassDlgItem(IDC_STATIC_COLOR, this);
	m_cColor.m_nColor = m_nColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditBusEntry::OnStnClickedStaticColor()
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
