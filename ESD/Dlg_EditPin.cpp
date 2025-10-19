// Dlg_EditPin.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_EditPin.h"
#include ".\dlg_editpin.h"
#include "systemext.h"
//#include "


// CDlg_EditPin 对话框

IMPLEMENT_DYNAMIC(CDlg_EditPin, CDialog)
CDlg_EditPin::CDlg_EditPin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_EditPin::IDD, pParent)
	, m_csEditPinName(_T(""))
	, m_csEditPinNumber(_T(""))
	, m_nComboType(0)
	, m_csEditDescription(_T(""))
	, m_csEditConnectTo(_T(""))
	, m_csEditLocationX(_T(""))
	, m_csEditLocationY(_T(""))
	, m_csEditPinLength(_T(""))
	, m_nComboOrientation(0)
	, m_nComboInside(0)
	, m_nComboInsideEdge(0)
	, m_nComboOutsideEdge(0)
	, m_nComboOutside(0)
	, m_nEditPartNum(0)
	, m_bCheckPinHide(FALSE)
	, m_bCheckNameVisible(FALSE)
	, m_bCheckNumberVisible(FALSE)
	, m_csEditUniqueID(_T(""))

{
	first_show = TRUE;
}

CDlg_EditPin::~CDlg_EditPin()
{
}

void CDlg_EditPin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PINNAME, m_csEditPinName);
	DDX_Text(pDX, IDC_EDIT_PINNUMBER, m_csEditPinNumber);
	DDX_CBIndex(pDX, IDC_COMBO_ELECTRICAL_TYPE, m_nComboType);
	DDX_Text(pDX, IDC_EDITDESCRIPTION, m_csEditDescription);
	DDX_Text(pDX, IDC_EDIT_CONNECT_TO, m_csEditConnectTo);
	DDX_Text(pDX, IDC_EDIT_PIN_LOCATION_X, m_csEditLocationX);
	DDX_Text(pDX, IDC_EDIT_PIN_LOCATION_Y, m_csEditLocationY);
	DDX_Text(pDX, IDC_EDIT_PIN_LENGTH, m_csEditPinLength);
	DDX_CBIndex(pDX, IDC_COMBO_PIN_ORIENTATION, m_nComboOrientation);
	DDX_CBIndex(pDX, IDC_COMBO_INSIDE, m_nComboInside);
	DDX_CBIndex(pDX, IDC_COMBO_INSIDE_EDGE, m_nComboInsideEdge);
	DDX_CBIndex(pDX, IDC_COMBO_OUTSIDE_EDGE, m_nComboOutsideEdge);
	DDX_CBIndex(pDX, IDC_COMBO_OUTSIDE, m_nComboOutside);
	DDX_Control(pDX, IDC_EDIT_PARTNUM, m_editPartNum);
	DDX_Control(pDX, IDC_SPIN_PARTNUM, m_spinPartNum);
	DDX_Text(pDX, IDC_EDIT_PARTNUM, m_nEditPartNum);
	DDX_Check(pDX, IDC_CHECK_HIDE, m_bCheckPinHide);
	DDX_Check(pDX, IDC_CHECK_PINNAME_VISIBLE, m_bCheckNameVisible);
	DDX_Check(pDX, IDC_CHECK_PINNUMBER_VISIBLE, m_bCheckNumberVisible);
	DDX_Text(pDX, IDC_EDIT_UNIQUEID, m_csEditUniqueID);

}


BEGIN_MESSAGE_MAP(CDlg_EditPin, CDialog)
	ON_STN_CLICKED(IDC_STATIC_COLOR, OnStnClickedStaticColor)
	ON_BN_CLICKED(IDC_CHECK_HIDE, OnBnClickedCheckHide)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_PINNAME_VISIBLE, OnBnClickedCheckPinnameVisible)
	ON_BN_CLICKED(IDC_CHECK_PINNUMBER_VISIBLE, OnBnClickedCheckPinnumberVisible)
	ON_CBN_SELCHANGE(IDC_COMBO_ELECTRICAL_TYPE, OnCbnSelchangeComboElectricalType)
	ON_CBN_SELCHANGE(IDC_COMBO_INSIDE, OnCbnSelchangeComboInside)
	ON_CBN_SELCHANGE(IDC_COMBO_INSIDE_EDGE, OnCbnSelchangeComboInsideEdge)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTSIDE, OnCbnSelchangeComboOutside)
	ON_CBN_SELCHANGE(IDC_COMBO_OUTSIDE_EDGE, OnCbnSelchangeComboOutsideEdge)
	ON_CBN_SELCHANGE(IDC_COMBO_PIN_ORIENTATION, OnCbnSelchangeComboPinOrientation)
	ON_EN_CHANGE(IDC_EDIT_PIN_LENGTH, OnEnChangeEditPinLength)
	ON_EN_CHANGE(IDC_EDIT_PINNAME, OnEnChangeEditPinname)
	ON_EN_CHANGE(IDC_EDIT_PINNUMBER, OnEnChangeEditPinnumber)
END_MESSAGE_MAP()


// CDlg_EditPin 消息处理程序

BOOL CDlg_EditPin::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cColor.SubclassDlgItem(IDC_STATIC_COLOR, this);
	m_cColor.m_nColor = m_nColor;

	m_cPinDemo.SubclassDlgItem(IDC_STATIC_PINDEMO, this);

	m_spinPartNum.SetBuddy(&m_editPartNum);
	m_spinPartNum.SetRange(0,4);

	CWnd* pWnd = GetDlgItem(IDC_EDIT_CONNECT_TO);
	if(m_bCheckPinHide == FALSE) pWnd->EnableWindow(FALSE);
	else
	{
		pWnd->EnableWindow(TRUE);
		if(m_csEditConnectTo.IsEmpty() == TRUE)
		{
			m_csEditConnectTo = m_csEditPinName;
			pWnd->SetWindowText(m_csEditConnectTo);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_EditPin::OnStnClickedStaticColor()
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		m_cColor.m_nColor = colordialog.GetColor();
		m_nColor = m_cColor.m_nColor;
		m_cColor.Invalidate();
	}

	DrawPin();
}

void CDlg_EditPin::OnBnClickedCheckHide()
{
	CWnd* pWnd = GetDlgItem(IDC_EDIT_CONNECT_TO);

	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_HIDE);
	if(pButton->GetCheck() == BST_UNCHECKED) pWnd->EnableWindow(FALSE);
	else 
	{
		pWnd->EnableWindow(TRUE);
		if(m_csEditConnectTo.IsEmpty() == TRUE)
		{
			m_csEditConnectTo = m_csEditPinName;
			pWnd->SetWindowText(m_csEditConnectTo);
		}
	}

	DrawPin();
}

void CDlg_EditPin::DrawPin()
{
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PINDEMO);
	RECT rect;
	pWnd->GetClientRect(&rect);
	
	rect.left++;
	rect.right--;
	rect.top++;
	rect.bottom--;
	
	CClientDC dc(pWnd);
	dc.FillSolidRect( rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, RGB(254,251,218));
	dc.IntersectClipRect(&rect);

	int x = (rect.right - rect.left)/2 ;
	int y = (rect.bottom - rect.top)/2;

	int nEx = rect.right-rect.left;
	int nEy = rect.bottom-rect.top;

	UpdateData(TRUE);

	COb_schpin apin;
	apin.pDocument = pDoc;

	apin.m_fPinLength = pDoc->Convert_String_To_XY(m_csEditPinLength);
	apin.m_nOrientation = m_nComboOrientation*90;
	apin.m_nColor = m_nColor;

	apin.m_csName = m_csEditPinName;
	apin.m_bNameVisible = m_bCheckNameVisible;
	apin.m_csNumber = m_csEditPinNumber;
	apin.m_bNumberVisible = m_bCheckNumberVisible;
	apin.m_nElectricalType = m_nComboType;
	apin.m_bHide = m_bCheckPinHide;

				  		  	   
	int		  inside_cov2[12] = {0, 8, 9,10,11,12,13,22,23,24,30,32};
	int  inside_edge_cov2[2 ] = {0, 3};
	int outside_edge_cov2[4 ] = {0, 1, 4, 17};
	int		 outside_cov2[7 ] = {0, 2, 5, 6,25,33,34};
	apin.m_nInside = inside_cov2[m_nComboInside];
	apin.m_nInsideEdge = inside_edge_cov2[m_nComboInsideEdge];
	apin.m_nOutside = outside_cov2[m_nComboOutside];
	apin.m_nOutsideEdge = outside_edge_cov2[m_nComboOutsideEdge];
	
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, cEDA_schcomp.m_nColor);
	dc.SelectObject(&pen);
	if(apin.m_nOrientation == 0)
	{
		dc.FillSolidRect( rect.left, rect.top, (rect.right-rect.left)/2, rect.bottom-rect.top, cEDA_schcomp.m_nFillColor);
		dc.MoveTo((rect.right-rect.left)/2,rect.top);
		dc.LineTo((rect.right-rect.left)/2,rect.bottom);
	}
	else if(apin.m_nOrientation == 90)
	{
		dc.FillSolidRect( rect.left, (rect.bottom-rect.top)/2, rect.right, rect.bottom, cEDA_schcomp.m_nFillColor);
		dc.MoveTo(rect.left, (rect.bottom-rect.top)/2);
		dc.LineTo(rect.right, (rect.bottom-rect.top)/2);
	}	
	else if(apin.m_nOrientation == 180)
	{
		dc.FillSolidRect((rect.right-rect.left)/2, rect.top, rect.right, rect.bottom-rect.top, cEDA_schcomp.m_nFillColor);
		dc.MoveTo((rect.right-rect.left)/2,rect.top);
		dc.LineTo((rect.right-rect.left)/2,rect.bottom);
	}
	else
	{
		dc.FillSolidRect( rect.left, rect.top, rect.right, (rect.bottom-rect.top)/2, cEDA_schcomp.m_nFillColor);
		dc.MoveTo(rect.left, (rect.bottom-rect.top)/2);
		dc.LineTo(rect.right, (rect.bottom-rect.top)/2);
	}

	double scale = (apin.m_fPinLength + dc.GetTextExtent(apin.m_csName).cx)*3/nEx;
	apin.m_fX = x*scale;
	apin.m_fY = y*scale;
	apin.DrawInWindow(&dc, scale, 0, 0, nEy);

	//apin.DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
}
void CDlg_EditPin::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if(first_show == TRUE)
	{
		first_show = FALSE;
		DrawPin();
	}
}

void CDlg_EditPin::OnBnClickedCheckPinnameVisible()
{
	DrawPin();
}

void CDlg_EditPin::OnBnClickedCheckPinnumberVisible()
{
	DrawPin();
}

void CDlg_EditPin::OnCbnSelchangeComboElectricalType()
{
	DrawPin();
}

void CDlg_EditPin::OnCbnSelchangeComboInside()
{
	DrawPin();
}

void CDlg_EditPin::OnCbnSelchangeComboInsideEdge()
{
	DrawPin();
}

void CDlg_EditPin::OnCbnSelchangeComboOutside()
{
	DrawPin();
}

void CDlg_EditPin::OnCbnSelchangeComboOutsideEdge()
{
	DrawPin();
}

void CDlg_EditPin::OnCbnSelchangeComboPinOrientation()
{
	DrawPin();
}

void CDlg_EditPin::OnEnChangeEditPinLength()
{
	DrawPin();
}

void CDlg_EditPin::OnEnChangeEditPinname()
{
	DrawPin();
}

void CDlg_EditPin::OnEnChangeEditPinnumber()
{
	DrawPin();
}
