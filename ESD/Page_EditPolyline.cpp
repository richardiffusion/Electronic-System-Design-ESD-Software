// Page_EditPolyline.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Page_EditPolyline.h"
#include ".\page_editpolyline.h"


// CPage_EditPolyline 对话框

IMPLEMENT_DYNAMIC(CPage_EditPolyline, CPropertyPage)
CPage_EditPolyline::CPage_EditPolyline()
	: CPropertyPage(CPage_EditPolyline::IDD)
	, m_nComboWidth(0)
	, m_nComboLineType(0)
	, m_nComboStartLineShape(0)
	, m_nComboEndLineShape(0)
	, m_nComboLineShapeSize(0)
	, m_bCheckGlobal(FALSE)
{
	m_hBkBrush = ::CreateSolidBrush(RGB(255, 255, 255)); 
}

CPage_EditPolyline::~CPage_EditPolyline()
{
}

void CPage_EditPolyline::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_BITMAP_POLYLINE, m_Bitmap);
	DDX_CBIndex(pDX, IDC_COMBO_LINEWIDTH, m_nComboWidth);
	DDX_CBIndex(pDX, IDC_COMBO_LINESTYLE, m_nComboLineType);
	DDX_CBIndex(pDX, IDC_COMBO_STARTSHAPE, m_nComboStartLineShape);
	DDX_CBIndex(pDX, IDC_COMBO_ENDSHAPE, m_nComboEndLineShape);
	DDX_CBIndex(pDX, IDC_COMBO_SIZE, m_nComboLineShapeSize);
	DDX_Check(pDX, IDC_CHECK_GLOBAL, m_bCheckGlobal);
}


BEGIN_MESSAGE_MAP(CPage_EditPolyline, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_LINEWIDTH, OnCbnSelchangeComboLinewidth)
	ON_STN_CLICKED(IDC_STATIC_COLOR, OnStnClickedStaticColor)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CPage_EditPolyline 消息处理程序

BOOL CPage_EditPolyline::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_Bitmap.SetBitmap(IDB_EDIT_POLYLINE);

	m_cColor.SubclassDlgItem(IDC_STATIC_COLOR, this);
	m_cColor.m_nColor = m_nColor;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPage_EditPolyline::OnCbnSelchangeComboLinewidth()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CPage_EditPolyline::OnStnClickedStaticColor()
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

HBRUSH CPage_EditPolyline::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
