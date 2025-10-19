// Page_EditPolygon.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Page_EditPolygon.h"
#include ".\page_editpolygon.h"


// CPage_EditPolygon 对话框

IMPLEMENT_DYNAMIC(CPage_EditPolygon, CPropertyPage)
CPage_EditPolygon::CPage_EditPolygon()
	: CPropertyPage(CPage_EditPolygon::IDD)
	, m_nComboWidth(0)
	, m_bCheckDrawSolid(FALSE)
	, m_bCheckTransparent(FALSE)
	, m_bCheckGlobal(FALSE)
{
	m_hBkBrush = ::CreateSolidBrush(RGB(255, 255, 255)); 
}

CPage_EditPolygon::~CPage_EditPolygon()
{
}

void CPage_EditPolygon::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_EDIT_POLYGON, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_BORDERWIDTH, m_nComboWidth);
	DDX_Check(pDX, IDC_CHECK_DRAWSOLID, m_bCheckDrawSolid);
	DDX_Check(pDX, IDC_CHECK_TRANSPARENT, m_bCheckTransparent);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CPage_EditPolygon, CPropertyPage)
	ON_STN_CLICKED(IDC_STATIC_FILLCOLOR, OnStnClickedStaticFillcolor)
	ON_STN_CLICKED(IDC_STATIC_BORDERCOLOR, OnStnClickedStaticBordercolor)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPage_EditPolygon 消息处理程序

BOOL CPage_EditPolygon::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_POLYGON);

	m_cFillColor.SubclassDlgItem(IDC_STATIC_FILLCOLOR, this);
	m_cFillColor.m_nColor = m_nFillColor;
	m_cBorderColor.SubclassDlgItem(IDC_STATIC_BORDERCOLOR, this);
	m_cBorderColor.m_nColor = m_nBorderColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPage_EditPolygon::OnStnClickedStaticFillcolor()
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

void CPage_EditPolygon::OnStnClickedStaticBordercolor()
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

HBRUSH CPage_EditPolygon::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//#define CTLCOLOR_MSGBOX         0		//CTLCOLOR_MSGBOX       Message   box  
	//#define CTLCOLOR_EDIT           1		//CTLCOLOR_EDIT       Edit   control    
	//#define CTLCOLOR_LISTBOX        2		//CTLCOLOR_LISTBOX       List-box   control     
	//#define CTLCOLOR_BTN            3		//CTLCOLOR_BTN       Button   control    
	//#define CTLCOLOR_DLG            4		//CTLCOLOR_DLG       Dialog   box  
	//#define CTLCOLOR_SCROLLBAR      5		//CTLCOLOR_SCROLLBAR       Scroll-bar   control  
	//#define CTLCOLOR_STATIC         6		//CTLCOLOR_STATIC       Static   control
	//#define CTLCOLOR_MAX            7

	if((nCtlColor   ==   CTLCOLOR_DLG)   || (nCtlColor   == CTLCOLOR_STATIC )) 
	{   
		//   if   about   to   paint   background   -   use   our   brush   to   paint   required   color.   
		if(m_hBkBrush)   
			return   m_hBkBrush;   
	} 
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
