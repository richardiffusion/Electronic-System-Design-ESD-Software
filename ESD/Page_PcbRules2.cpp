// TabLongPage.cpp : implementation file
//

#include "stdafx.h"
#include "ESD.h"
#include "Page_PcbRules2.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules2 property page

IMPLEMENT_DYNCREATE(CPage_PcbRules2, CXTPPropertyPage)

CPage_PcbRules2::CPage_PcbRules2() : CXTPPropertyPage(CPage_PcbRules2::IDD)
, m_csEditWidth(_T(""))
, m_csEditViaSize(_T(""))
, m_csEditHoleSize(_T(""))
{
	//{{AFX_DATA_INIT(CPage_PcbRules2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPage_PcbRules2::~CPage_PcbRules2()
{
}

void CPage_PcbRules2::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_WIDTH, m_BitmapWidth);
	DDX_Control(pDX, IDC_STATIC_VIA, m_BitmapVia);
	//{{AFX_DATA_MAP(CPage_PcbRules2)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_csEditWidth);
	DDX_Text(pDX, IDC_EDIT_VIASIZE, m_csEditViaSize);
	DDX_Text(pDX, IDC_EDIT_HOLESIZE, m_csEditHoleSize);
}


BEGIN_MESSAGE_MAP(CPage_PcbRules2, CXTPPropertyPage)
	//{{AFX_MSG_MAP(CPage_PcbRules2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules2 message handlers

BOOL CPage_PcbRules2::OnInitDialog() 
{
	CXTPPropertyPage::OnInitDialog();
	m_BitmapWidth.SetBitmap(IDB_PCB_RULES_WIDTH);
	m_BitmapVia.SetBitmap(IDB_PCB_RULES_VIA);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

