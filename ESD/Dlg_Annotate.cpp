// Dlg_Annotate.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Annotate.h"
#include ".\dlg_annotate.h"


// CDlg_Annotate 对话框

IMPLEMENT_DYNAMIC(CDlg_Annotate, CDialog)
CDlg_Annotate::CDlg_Annotate(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Annotate::IDD, pParent)
	, m_nComboOrder(0)
	, m_nEditStartNum(0)
{
}

CDlg_Annotate::~CDlg_Annotate()
{
}

void CDlg_Annotate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_ORDER, m_nComboOrder);
	DDX_Radio(pDX, IDC_RADIO_DIAGRAM, m_nRadioScope);
	DDX_Text(pDX, IDC_EDIT_STARTNUM, m_nEditStartNum);
	DDV_MinMaxInt(pDX, m_nEditStartNum, 1, 10000000);
	DDX_Control(pDX, IDC_LIST_A, m_listA);
	DDX_Control(pDX, IDC_LIST_E, m_listE);
}


BEGIN_MESSAGE_MAP(CDlg_Annotate, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_ORDER, OnCbnSelchangeComboOrder)
END_MESSAGE_MAP()


// CDlg_Annotate 消息处理程序

BOOL CDlg_Annotate::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_STATIC_ORDER1);
	if(m_nComboOrder == 0) pWnd->ShowWindow(TRUE);
	else pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_ORDER2);
	if(m_nComboOrder == 1) pWnd->ShowWindow(TRUE);
	else pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_ORDER3);
	if(m_nComboOrder == 2) pWnd->ShowWindow(TRUE);
	else pWnd->ShowWindow(FALSE);	
	pWnd = GetDlgItem(IDC_STATIC_ORDER4);
	if(m_nComboOrder == 3) pWnd->ShowWindow(TRUE);
	else pWnd->ShowWindow(FALSE);	

	m_listA.InsertColumn(0, _T("元件名称"), LVCFMT_LEFT, 150);
	int iItem;
	for (iItem = 0; iItem < m_arrayStringA.GetCount(); ++iItem)
	{
		CString str;
		str = m_arrayStringA.GetAt(iItem);
		m_listA.InsertItem(iItem, str, 0);
	}	
	m_listE.InsertColumn(0, _T("元件名称"), LVCFMT_LEFT, 150);
	for (iItem = 0; iItem < m_arrayStringE.GetCount(); ++iItem)
	{
		CString str;
		str = m_arrayStringE.GetAt(iItem);
		m_listE.InsertItem(iItem, str, 0);
	}		
	

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_Annotate::OnCbnSelchangeComboOrder()
{
	UpdateData(TRUE);
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_STATIC_ORDER1);
	if(m_nComboOrder == 0) pWnd->ShowWindow(TRUE);
	else pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_ORDER2);
	if(m_nComboOrder == 1) pWnd->ShowWindow(TRUE);
	else pWnd->ShowWindow(FALSE);
	pWnd = GetDlgItem(IDC_STATIC_ORDER3);
	if(m_nComboOrder == 2) pWnd->ShowWindow(TRUE);
	else pWnd->ShowWindow(FALSE);	
	pWnd = GetDlgItem(IDC_STATIC_ORDER4);
	if(m_nComboOrder == 3) pWnd->ShowWindow(TRUE);
	else pWnd->ShowWindow(FALSE);	
}

