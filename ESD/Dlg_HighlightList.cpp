// Dlg_HighlightList.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_HighlightList.h"
#include ".\dlg_highlightlist.h"


// CDlg_HighlightList 对话框

IMPLEMENT_DYNAMIC(CDlg_HighlightList, CDialog)
CDlg_HighlightList::CDlg_HighlightList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_HighlightList::IDD, pParent)
{
}

CDlg_HighlightList::~CDlg_HighlightList()
{
}

void CDlg_HighlightList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MARKUP, m_wndStatic);
}


BEGIN_MESSAGE_MAP(CDlg_HighlightList, CDialog)
END_MESSAGE_MAP()


// CDlg_HighlightList 消息处理程序

BOOL CDlg_HighlightList::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndStatic.SetMarkupText((LPCSTR)pListBuffer);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

