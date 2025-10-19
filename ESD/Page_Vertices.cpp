// Page_Vertices.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Page_Vertices.h"
#include "struct.h"
#include ".\page_vertices.h"

#define BoolType(b) b?true:false

// CPage_Vertices 对话框

IMPLEMENT_DYNAMIC(CPage_Vertices, CPropertyPage)
CPage_Vertices::CPage_Vertices()
	: CPropertyPage(CPage_Vertices::IDD)
{
	m_nSortedCol = 1;
	m_bAscending = true;
}

CPage_Vertices::~CPage_Vertices()
{
}

void CPage_Vertices::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_LIST_VERTICES, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CPage_Vertices, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnBnClickedButtonRemove)
END_MESSAGE_MAP()


// CPage_Vertices 消息处理程序

BOOL CPage_Vertices::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_listCtrl.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(1, _T("X坐标"), LVCFMT_LEFT, 100);
	m_listCtrl.InsertColumn(2, _T("Y坐标"), LVCFMT_LEFT, 100);
	
	int iItem;
	for (iItem = 0; iItem < m_listCtrl.m_arrayX.GetCount(); ++iItem)
	{
		CString strItem;
		CString strX;
		CString strY;

		strItem.Format(_T("%d"), iItem);
		strX = m_listCtrl.m_arrayX[iItem];
		strY = m_listCtrl.m_arrayY[iItem];

		m_listCtrl.InsertItem(iItem, strItem, 0);
		m_listCtrl.SetItem(iItem, 1, LVIF_TEXT, strX, 0, NULL, NULL, NULL);
		m_listCtrl.SetItem(iItem, 2, LVIF_TEXT, strY, 0, NULL, NULL, NULL);
	}

	HWND hWndHeader = m_listCtrl.GetDlgItem(0)->GetSafeHwnd();
	m_header.SubclassWindow(hWndHeader);


	m_header.EnableAutoSize(TRUE);
	m_header.ResizeColumnsToFit();

	m_header.FreezeColumn(0);
	m_header.FreezeColumn(1);
	m_header.FreezeColumn(2);

	CXTHeaderCtrlTheme* pTheme = m_header.GetTheme();
	if (pTheme)
	{
		DWORD dwStyle = pTheme->GetDrawStyle()| XTTHEME_WINXPTHEMES | XTTHEME_SORTARROW;
		pTheme->SetDrawStyle(dwStyle, &m_header);
		m_header.RedrawWindow();
	}

	//Enable Full Row Selection
	m_listCtrl.ModifyExtendedStyle(0, LVS_EX_FULLROWSELECT);

	//Enable Grid Lines
	m_listCtrl.ModifyExtendedStyle(0, LVS_EX_GRIDLINES);

	//Enable Check Box
	//m_listCtrl.ModifyExtendedStyle( 0, LVS_EX_CHECKBOXES);

	//Enable Drag Drop
	m_listCtrl.ModifyExtendedStyle(0, LVS_EX_HEADERDRAGDROP);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPage_Vertices::SortColumn(int iCol, bool bAsc)
{
	m_bAscending = bAsc;
	m_nSortedCol = iCol;

	// set sort image for header and sort column.
	m_listCtrl.SetSortImage(m_nSortedCol, m_bAscending);

	CXTSortClass csc(&m_listCtrl, m_nSortedCol);
	csc.Sort(m_bAscending, xtSortString);
}

BOOL CPage_Vertices::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	HD_NOTIFY *pHDNotify = (HD_NOTIFY*)lParam;

	if (pHDNotify->hdr.code == HDN_ITEMCLICKA ||
		pHDNotify->hdr.code == HDN_ITEMCLICKW)
	{
		if (pHDNotify->iItem == m_nSortedCol)
			SortColumn(pHDNotify->iItem, !m_bAscending);
		else
			SortColumn(pHDNotify->iItem, BoolType(m_header.GetAscending()));
	}

	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void CPage_Vertices::OnBnClickedButtonAdd()
{
	int selection_index = m_listCtrl.GetSelectionMark( );	

	if(selection_index < 0) selection_index = 0;

	CString strItem = m_listCtrl.GetItemText(selection_index, 0);
	CString strX = m_listCtrl.GetItemText(selection_index, 1);
	CString strY = m_listCtrl.GetItemText(selection_index, 2);

	m_listCtrl.InsertItem(selection_index, strItem, 0);
	m_listCtrl.SetItem(selection_index, 1, LVIF_TEXT, strX, 0, NULL, NULL, NULL);
	m_listCtrl.SetItem(selection_index, 2, LVIF_TEXT, strY, 0, NULL, NULL, NULL);
	
	for(int i=selection_index+1; i<m_listCtrl.GetItemCount(); i++)
	{
		strItem.Format(_T("%d"), i);
		m_listCtrl.SetItemText(i, 0, strItem);
	}

	m_listCtrl.m_arrayX.InsertAt(selection_index, strX);
	m_listCtrl.m_arrayY.InsertAt(selection_index, strY);
}

void CPage_Vertices::OnBnClickedButtonRemove()
{
	int selection_index = m_listCtrl.GetSelectionMark( );	
	if(selection_index < 0) return; 
    
	int i = m_listCtrl.GetItemCount();
	if(i <= 2) return;

	m_listCtrl.DeleteItem(selection_index);
	m_listCtrl.m_arrayX.RemoveAt(selection_index);
	m_listCtrl.m_arrayY.RemoveAt(selection_index);
}

