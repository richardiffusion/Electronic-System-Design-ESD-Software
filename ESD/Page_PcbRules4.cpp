// TabLongPage.cpp : implementation file
//

#include "stdafx.h"
#include "ESD.h"
#include "Page_PcbRules4.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules4 property page

IMPLEMENT_DYNCREATE(CPage_PcbRules4, CXTPPropertyPage)

CPage_PcbRules4::CPage_PcbRules4() : CXTPPropertyPage(CPage_PcbRules4::IDD)
, m_csEditSolder(_T(""))
, m_csEditPaste(_T(""))
{
	//{{AFX_DATA_INIT(CPage_PcbRules4)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPage_PcbRules4::~CPage_PcbRules4()
{
}

void CPage_PcbRules4::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SOLDER, m_Bitmap1);
	DDX_Control(pDX, IDC_STATIC_PASTE, m_Bitmap2);
	//{{AFX_DATA_MAP(CPage_PcbRules4)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_SOLDER, m_csEditSolder);
	DDX_Text(pDX, IDC_EDIT_PASTE, m_csEditPaste);
}


BEGIN_MESSAGE_MAP(CPage_PcbRules4, CXTPPropertyPage)
	//{{AFX_MSG_MAP(CPage_PcbRules4)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules4 message handlers

BOOL CPage_PcbRules4::OnInitDialog() 
{
	CXTPPropertyPage::OnInitDialog();

	m_Bitmap1.SetBitmap(IDB_PCB_RULES_SOLDER);
	m_Bitmap2.SetBitmap(IDB_PCB_RULES_PASTE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
