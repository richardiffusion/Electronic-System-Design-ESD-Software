// TabLongPage.cpp : implementation file
//

#include "stdafx.h"
#include "ESD.h"
#include "Page_PcbRules6.h"
#include ".\page_pcbrules6.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules6 property page

IMPLEMENT_DYNCREATE(CPage_PcbRules6, CXTPPropertyPage)

CPage_PcbRules6::CPage_PcbRules6() : CXTPPropertyPage(CPage_PcbRules6::IDD)
, m_nComboStyle(0)
, m_csEditWidth(_T(""))
{
	//{{AFX_DATA_INIT(CPage_PcbRules6)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPage_PcbRules6::~CPage_PcbRules6()
{
}

void CPage_PcbRules6::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_POLYGON1, m_Bitmap1);
	DDX_Control(pDX, IDC_STATIC_POLYGON2, m_Bitmap2);
	//{{AFX_DATA_MAP(CPage_PcbRules6)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_CBIndex(pDX, IDC_COMBO_STYLE, m_nComboStyle);
	DDX_Text(pDX, IDC_EDIT_POLYGON_WIDTH, m_csEditWidth);
	DDX_Radio(pDX, IDC_RADIO_CONDUCTOR_2, m_nRadioType);
}


BEGIN_MESSAGE_MAP(CPage_PcbRules6, CXTPPropertyPage)
	//{{AFX_MSG_MAP(CPage_PcbRules6)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_CONDUCTOR_2, OnBnClickedRadioConductor2)
	ON_BN_CLICKED(IDC_RADIO_CONDUCTOR_4, OnBnClickedRadioConductor4)
	ON_CBN_SELCHANGE(IDC_COMBO_STYLE, OnCbnSelchangeComboStyle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules6 message handlers

BOOL CPage_PcbRules6::OnInitDialog() 
{
	CXTPPropertyPage::OnInitDialog();
	m_Bitmap1.SetBitmap(IDB_PCB_RULES_POLYGON1);
	m_Bitmap2.SetBitmap(IDB_PCB_RULES_POLYGON2);

	CWnd* pWnd;
	if(m_nRadioType == 0)
	{
		pWnd = GetDlgItem(IDC_STATIC_POLYGON1);
		pWnd->ShowWindow(TRUE);
		pWnd = GetDlgItem(IDC_STATIC_POLYGON2);
		pWnd->ShowWindow(FALSE);
	}
	else
	{
		pWnd = GetDlgItem(IDC_STATIC_POLYGON1);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_POLYGON2);
		pWnd->ShowWindow(TRUE);
	}
	
	if(m_nComboStyle == 0)
	{
		//conductor 2,4
		pWnd = GetDlgItem(IDC_STATIC_CONDUCTOR);
		pWnd->ShowWindow(TRUE);
		
		pWnd = GetDlgItem(IDC_RADIO_CONDUCTOR_2);
		pWnd->ShowWindow(TRUE);
		
		pWnd = GetDlgItem(IDC_RADIO_CONDUCTOR_4);
		pWnd->ShowWindow(TRUE);
		
		//width, gap, expansion
		pWnd = GetDlgItem(IDC_STATIC_POLYGON_WIDTH);
		pWnd->ShowWindow(TRUE);
		
		pWnd = GetDlgItem(IDC_EDIT_POLYGON_WIDTH);
		pWnd->ShowWindow(TRUE);
		
		//picture
		pWnd = GetDlgItem(IDC_STATIC_POLYGON1);
		pWnd->ShowWindow(TRUE);
		
		pWnd = GetDlgItem(IDC_STATIC_POLYGON2);
		pWnd->ShowWindow(TRUE);

		if(m_nRadioType == 0)
		{
			pWnd = GetDlgItem(IDC_STATIC_POLYGON2);
			pWnd->ShowWindow(FALSE);
		}
		else
		{
			pWnd = GetDlgItem(IDC_STATIC_POLYGON1);
			pWnd->ShowWindow(FALSE);
		}
	}
	else 
	{
		//conductor 2,4
		pWnd = GetDlgItem(IDC_STATIC_CONDUCTOR);
		pWnd->ShowWindow(FALSE);
		
		pWnd = GetDlgItem(IDC_RADIO_CONDUCTOR_2);
		pWnd->ShowWindow(FALSE);		
		
		pWnd = GetDlgItem(IDC_RADIO_CONDUCTOR_4);
		pWnd->ShowWindow(FALSE);	
		
		//width, gap, expansion
		pWnd = GetDlgItem(IDC_STATIC_POLYGON_WIDTH);
		pWnd->ShowWindow(FALSE);
		
		pWnd = GetDlgItem(IDC_EDIT_POLYGON_WIDTH);
		pWnd->ShowWindow(FALSE);
		
		//picture
		pWnd = GetDlgItem(IDC_STATIC_POLYGON1);
		pWnd->ShowWindow(FALSE);
		
		pWnd = GetDlgItem(IDC_STATIC_POLYGON2);
		pWnd->ShowWindow(FALSE);	
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CPage_PcbRules6::OnBnClickedRadioConductor2()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_STATIC_POLYGON1);
	pWnd->ShowWindow(TRUE);
	pWnd = GetDlgItem(IDC_STATIC_POLYGON2);
	pWnd->ShowWindow(FALSE);
}

void CPage_PcbRules6::OnBnClickedRadioConductor4()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_STATIC_POLYGON1);
	pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_POLYGON2);
	pWnd->ShowWindow(TRUE);
}


void CPage_PcbRules6::OnCbnSelchangeComboStyle()
{
	UpdateData(TRUE);
	CWnd* pWnd;
	
	if(m_nComboStyle == 0)
	{
		//conductor 2,4
		pWnd = GetDlgItem(IDC_STATIC_CONDUCTOR);
		pWnd->ShowWindow(TRUE);
		
		pWnd = GetDlgItem(IDC_RADIO_CONDUCTOR_2);
		pWnd->ShowWindow(TRUE);
		
		pWnd = GetDlgItem(IDC_RADIO_CONDUCTOR_4);
		pWnd->ShowWindow(TRUE);
		
		//width, gap, expansion
		pWnd = GetDlgItem(IDC_STATIC_POLYGON_WIDTH);
		pWnd->ShowWindow(TRUE);
		
		pWnd = GetDlgItem(IDC_EDIT_POLYGON_WIDTH);
		pWnd->ShowWindow(TRUE);
		
		//picture
		pWnd = GetDlgItem(IDC_STATIC_POLYGON1);
		pWnd->ShowWindow(TRUE);
		
		pWnd = GetDlgItem(IDC_STATIC_POLYGON2);
		pWnd->ShowWindow(TRUE);

		if(m_nRadioType == 0)
		{
			pWnd = GetDlgItem(IDC_STATIC_POLYGON2);
			pWnd->ShowWindow(FALSE);
		}
		else
		{
			pWnd = GetDlgItem(IDC_STATIC_POLYGON1);
			pWnd->ShowWindow(FALSE);
		}
	}
	else 
	{
		//conductor 2,4
		pWnd = GetDlgItem(IDC_STATIC_CONDUCTOR);
		pWnd->ShowWindow(FALSE);
		
		pWnd = GetDlgItem(IDC_RADIO_CONDUCTOR_2);
		pWnd->ShowWindow(FALSE);		
		
		pWnd = GetDlgItem(IDC_RADIO_CONDUCTOR_4);
		pWnd->ShowWindow(FALSE);	
		
		//width, gap, expansion
		pWnd = GetDlgItem(IDC_STATIC_POLYGON_WIDTH);
		pWnd->ShowWindow(FALSE);
		
		pWnd = GetDlgItem(IDC_EDIT_POLYGON_WIDTH);
		pWnd->ShowWindow(FALSE);
		
		//picture
		pWnd = GetDlgItem(IDC_STATIC_POLYGON1);
		pWnd->ShowWindow(FALSE);
		
		pWnd = GetDlgItem(IDC_STATIC_POLYGON2);
		pWnd->ShowWindow(FALSE);	
	}
}
