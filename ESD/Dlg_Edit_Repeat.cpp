// Dlg_Edit_Repeat.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Edit_Repeat.h"
#include ".\dlg_edit_repeat.h"


// CDlg_Edit_Repeat 对话框

IMPLEMENT_DYNAMIC(CDlg_Edit_Repeat, CDialog)
CDlg_Edit_Repeat::CDlg_Edit_Repeat(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Edit_Repeat::IDD, pParent)
	, m_nEditNumber(1)
	, m_nEditNDelta(1)
	, m_csEditXDelta(_T(""))
	, m_csEditYDelta(_T(""))
{
}

CDlg_Edit_Repeat::~CDlg_Edit_Repeat()
{
}

void CDlg_Edit_Repeat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NUMBER, m_nEditNumber);
	DDV_MinMaxInt(pDX, m_nEditNumber, 1, 500);
	DDX_Text(pDX, IDC_EDIT_NDELTA, m_nEditNDelta);
	DDV_MinMaxInt(pDX, m_nEditNDelta, 0, 10000);
	DDX_Text(pDX, IDC_EDIT_XDELTA, m_csEditXDelta);
	DDX_Text(pDX, IDC_EDIT_YDELTA, m_csEditYDelta);
}


BEGIN_MESSAGE_MAP(CDlg_Edit_Repeat, CDialog)
END_MESSAGE_MAP()


// CDlg_Edit_Repeat 消息处理程序

BOOL CDlg_Edit_Repeat::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_csTitleString);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

