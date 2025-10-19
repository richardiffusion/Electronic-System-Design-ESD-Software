// Dlg_EditGraphic.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditGraphic.h"
#include ".\dlg_editgraphic.h"


// CDlg_EditGraphic 对话框

IMPLEMENT_DYNAMIC(CDlg_EditGraphic, CDialog)
CDlg_EditGraphic::CDlg_EditGraphic(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditGraphic::IDD, pParent)
	, m_nComboWidth(0)

	, m_csEditFilename(_T(""))
	, m_bCheckShowBorder(FALSE)
	, m_bCheckXYRatio(FALSE)
	, m_csEditLocationX1(_T(""))
	, m_csEditLocationY1(_T(""))
	, m_csEditLocationX2(_T(""))
	, m_csEditLocationY2(_T(""))
{
}

CDlg_EditGraphic::~CDlg_EditGraphic()
{
}

void CDlg_EditGraphic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_GRAPHIC, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_BORDERWIDTH, m_nComboWidth);

	DDX_Text(pDX, IDC_EDIT_FILENAME, m_csEditFilename);
	DDX_Check(pDX, IDC_CHECK_SHOWBORDER, m_bCheckShowBorder);
	DDX_Check(pDX, IDC_CHECK_RATIO, m_bCheckXYRatio);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X1, m_csEditLocationX1);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y1, m_csEditLocationY1);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X2, m_csEditLocationX2);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y2, m_csEditLocationY2);
}


BEGIN_MESSAGE_MAP(CDlg_EditGraphic, CDialog)
	ON_STN_CLICKED(IDC_STATIC_BORDERCOLOR, OnStnClickedStaticBordercolor)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnBnClickedButtonBrowse)
END_MESSAGE_MAP()


// CDlg_EditGraphic 消息处理程序

BOOL CDlg_EditGraphic::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_GRAPHIC);

	m_cColor.SubclassDlgItem(IDC_STATIC_BORDERCOLOR, this);
	m_cColor.m_nColor = m_nColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditGraphic::OnStnClickedStaticBordercolor()
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

void CDlg_EditGraphic::OnBnClickedButtonBrowse()
{
	static char BASED_CODE szFileFilter[] = "图象文件(*.bmp)|*.bmp|所有文件 (*.*) |*.*||";
	CFileDialog importdialog(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_ENABLESIZING,szFileFilter,NULL);

	char filename[MAX_PATH];
	if(importdialog.DoModal() == IDOK) 
	{
		//打开文件
		strcpy(filename, importdialog.GetPathName());
		m_csEditFilename = filename;
		UpdateData(FALSE);
	}
}
