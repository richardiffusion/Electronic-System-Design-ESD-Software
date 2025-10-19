// TabLongPage.cpp : implementation file
//

#include "stdafx.h"
#include "ESD.h"
#include "Page_PcbRules7.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules7 property page

IMPLEMENT_DYNCREATE(CPage_PcbRules7, CXTPPropertyPage)

CPage_PcbRules7::CPage_PcbRules7() : CXTPPropertyPage(CPage_PcbRules7::IDD)
{
	//{{AFX_DATA_INIT(CPage_PcbRules7)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPage_PcbRules7::~CPage_PcbRules7()
{
}

void CPage_PcbRules7::DoDataExchange(CDataExchange* pDX)
{
	CXTPPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPage_PcbRules7)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPage_PcbRules7, CXTPPropertyPage)
	//{{AFX_MSG_MAP(CPage_PcbRules7)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPage_PcbRules7 message handlers

BOOL CPage_PcbRules7::OnInitDialog() 
{
	CXTPPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
