// FirstPage.cpp : implementation file
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_FirstPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlg_FirstPage dialog


CDlg_FirstPage::CDlg_FirstPage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_FirstPage::IDD, pParent)
{
	NONCLIENTMETRICS ncm;
	::ZeroMemory(&ncm, sizeof(NONCLIENTMETRICS));
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	VERIFY(::SystemParametersInfo(SPI_GETNONCLIENTMETRICS,
		sizeof(NONCLIENTMETRICS), &ncm, 0));

	ncm.lfMenuFont.lfWeight = FW_BOLD;
	ncm.lfMenuFont.lfUnderline = TRUE;
	strcpy(ncm.lfMenuFont.lfFaceName, "Arial");
	m_fontBold.CreateFontIndirect(&ncm.lfMenuFont);
}


void CDlg_FirstPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlg_FirstPage)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlg_FirstPage, CDialog)
	//{{AFX_MSG_MAP(CDlg_FirstPage)
	//}}AFX_MSG_MAP
//	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlg_FirstPage message handlers


HBRUSH CDlg_FirstPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CDlg_FirstPage::show_text(CString& cstring)
{
	CWnd  *pWnd1;
	pWnd1 = (CWnd*)GetDlgItem( IDC_STATIC_SETUP1 );

	pWnd1->SetWindowText(cstring);
	
}
BOOL CDlg_FirstPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
