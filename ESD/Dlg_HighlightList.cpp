// Dlg_HighlightList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_HighlightList.h"
#include ".\dlg_highlightlist.h"


// CDlg_HighlightList �Ի���

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


// CDlg_HighlightList ��Ϣ�������

BOOL CDlg_HighlightList::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_wndStatic.SetMarkupText((LPCSTR)pListBuffer);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

