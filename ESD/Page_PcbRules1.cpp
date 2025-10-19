// TabLongPage.cpp : implementation file
//

#include "stdafx.h"
#include "ESD.h"
#include "Page_PcbRules1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules1 property page

IMPLEMENT_DYNCREATE(CPage_PcbRules1, CXTPPropertyPage)

CPage_PcbRules1::CPage_PcbRules1() : CXTPPropertyPage(CPage_PcbRules1::IDD)
, m_csEditClearance(_T(""))
{
	//{{AFX_DATA_INIT(CPage_PcbRules1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPage_PcbRules1::~CPage_PcbRules1()
{
}

void CPage_PcbRules1::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CLEARANCE, m_Bitmap);
	//{{AFX_DATA_MAP(CPage_PcbRules1)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_CLEARANCE, m_csEditClearance);
}


BEGIN_MESSAGE_MAP(CPage_PcbRules1, CXTPPropertyPage)
	//{{AFX_MSG_MAP(CPage_PcbRules1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules1 message handlers

BOOL CPage_PcbRules1::OnInitDialog() 
{
	CXTPPropertyPage::OnInitDialog();
		
	m_Bitmap.SetBitmap(IDB_PCB_RULES_CLEARANCE);

//	ModifyStyle(0, WS_CLIPCHILDREN | WS_VSCROLL);
	
	/*for (int i = 0; i < 5; i++)
	{
		m_captions[i].SubclassDlgItem(IDC_STATIC_SUBCAPTION1 + i, this);
		SetResize(IDC_STATIC_SUBCAPTION1 + i, SZ_TOP_LEFT, SZ_TOP_RIGHT);
	}*/
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
