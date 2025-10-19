#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditSheetSymbol.h"
#include ".\dlg_editsheetsymbol.h"


// CDlg_EditSheetSymbol 对话框

IMPLEMENT_DYNAMIC(CDlg_EditSheetSymbol, CDialog)
CDlg_EditSheetSymbol::CDlg_EditSheetSymbol(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditSheetSymbol::IDD, pParent)
	, m_nComboWidth(0)
	, m_bCheckDrawSolid(FALSE)
	, m_csEditDesignator(_T(""))
	, m_csEditFilename(_T(""))
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
	, m_csEditSizeX(_T(""))
	, m_csEditSizeY(_T(""))
{
}

CDlg_EditSheetSymbol::~CDlg_EditSheetSymbol()
{
}

void CDlg_EditSheetSymbol::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_SHEETSYMBOL, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_BORDERWIDTH, m_nComboWidth);
	DDX_Check(pDX, IDC_CHECK_DRAWSOLID, m_bCheckDrawSolid);
	DDX_Text(pDX, IDC_EDIT_DESIGNATOR, m_csEditDesignator);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_csEditFilename);
	DDX_Text(pDX, IDC_EDIT_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_LOCATION_Y, m_csEditLocationY);
	DDX_Text(pDX, IDC_EDIT_SIZE_X, m_csEditSizeX);
	DDX_Text(pDX, IDC_EDIT_SIZE_Y, m_csEditSizeY);
}


BEGIN_MESSAGE_MAP(CDlg_EditSheetSymbol, CDialog)
	ON_STN_CLICKED(IDC_STATIC_FILLCOLOR, OnStnClickedStaticFillcolor)
	ON_STN_CLICKED(IDC_STATIC_BORDERCOLOR, OnStnClickedStaticBordercolor)
END_MESSAGE_MAP()


// CDlg_EditSheetSymbol 消息处理程序

BOOL CDlg_EditSheetSymbol::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_SHEETSYMBOL);
	
	m_cFillColor.SubclassDlgItem(IDC_STATIC_FILLCOLOR, this);
	m_cFillColor.m_nColor = m_nFillColor;
	m_cBorderColor.SubclassDlgItem(IDC_STATIC_BORDERCOLOR, this);
	m_cBorderColor.m_nColor = m_nBorderColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditSheetSymbol::OnStnClickedStaticFillcolor()
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

void CDlg_EditSheetSymbol::OnStnClickedStaticBordercolor()
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
