//

#include "stdafx.h"
#include "ESD.h"
#include "VertexListCtrl.h"
#include "inedit.h"


IMPLEMENT_DYNAMIC(CVertexListCtrl, CXTListCtrl)
CVertexListCtrl::CVertexListCtrl()
{
	iSubItemFocus = -1;
}

CVertexListCtrl::~CVertexListCtrl()
{

}


BEGIN_MESSAGE_MAP(CVertexListCtrl, CXTListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
END_MESSAGE_MAP()



// CVertexListCtrl 消息处理程序

void CVertexListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
//	if( GetFocus() != this ) SetFocus();
	CXTListCtrl::OnLButtonDown(nFlags, point);
}

void CVertexListCtrl::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if (GetFocus() != this) SetFocus();
	
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	int index;	
	int colnum;	
	if ((index = HitTestEx(point, &colnum)) != -1)
	{
		if (iSubItemFocus != colnum)  
		{
			CRect src;
			GetSubItemRect(index, iSubItemFocus, src);
			InvalidateRect(src);
			iSubItemFocus = colnum;
			GetSubItemRect(index, iSubItemFocus, src);
			InvalidateRect(src);
		}
		UINT flag = LVIS_FOCUSED;
		if ((GetItemState(index, flag) & flag) == flag)  
		{
			int irealcol = GetColumnIndex(colnum);
			
					
			if(colnum != 0) EditSubLabel(index, colnum);

		}
		else 
		{
			SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED , LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
	
	*pResult = 1;
}

int CVertexListCtrl::HitTestEx(CPoint & point, int * col) const
{
	int colnum = 0;
	int row = HitTest(point, NULL);
	
	if (col) *col = 0;
	if ((GetWindowLong(m_hWnd, GWL_STYLE) & LVS_TYPEMASK) != LVS_REPORT) return row;
	row = GetTopIndex();
	int bottom = row + GetCountPerPage();
	if (bottom > GetItemCount()) bottom = GetItemCount();
	int nColumnCount = GetColumnCount();
	for( ;row <=bottom;row++)
	{
		CRect rect;
		GetItemRect(row, &rect, LVIR_BOUNDS);
		if (rect.PtInRect(point))
		{
			for (colnum = 0; colnum < nColumnCount; colnum++)
			{
				int colwidth = GetColumnWidth(colnum);
				if (point.x >= rect.left && point.x <= (rect.left + colwidth))
				{
					if (col) *col = colnum;
					return row;
				}
				rect.left += colwidth;
			}
		}
	}
	return -1;
}

int CVertexListCtrl::GetColumnCount() const
{
	CHeaderCtrl* pHeader = (CHeaderCtrl*) GetDlgItem(0);
	return pHeader ? pHeader->GetItemCount() : 0;
}

void CVertexListCtrl::GetSubItemRect(const int nItem, const int nCol, CRect & rcItem)
{
	int nColumnCount = GetColumnCount();//pHeader->GetItemCount();
	int iColWidth = GetColumnWidth(nCol);
	if (nCol >= nColumnCount || iColWidth < 5) return;

	int offset = 0;
	for (int i = 0; i < nCol; i++) offset += GetColumnWidth(i);

	CRect rect;
	GetItemRect(nItem, &rect, LVIR_BOUNDS);

	rect.bottom --;
	rect.left += offset;
	rect.right = rect.left + GetColumnWidth(nCol);
	rcItem = rect;
}

int CVertexListCtrl::GetColumnIndex(const int iCol) const
{
	LV_COLUMN lv;
	lv.mask = LVCF_SUBITEM;
	GetColumn(iCol, &lv);
	return lv.iSubItem;
}

bool CVertexListCtrl::EnsureSubItemVisible(const int iSubItem)
{
	int nColumnCount = GetColumnCount();
	int iColWidth = GetColumnWidth(iSubItem);
	if (iSubItem >= nColumnCount || iColWidth < 5) return false;

	int offset = 0;
	for (int i = 0; i < iSubItem; i++) offset += GetColumnWidth(i);

	CRect rect, subRect;
	GetItemRect(0, &rect, LVIR_BOUNDS);
	subRect.SetRect(offset + rect.left, 0, offset + iColWidth + rect.left, 0);

	CRect rcClient;
	GetClientRect(&rcClient);

	if (subRect.left < rcClient.left)
	{
		CSize sz(rcClient.left + subRect.left, 0);
		Scroll(sz);
		return true;
	}
	else if (subRect.right > rcClient.right)
	{
		CSize sz(subRect.right - rcClient.right, 0);
		Scroll(sz);
		return true;
	}
	return true;
}

CRichEditCtrl * CVertexListCtrl::EditSubLabel(int nItem, int nCol, bool bClick)
{
	if (!EnsureVisible(nItem, TRUE)) return NULL;  

	EnsureSubItemVisible(nCol);

	int nColumnCount = GetColumnCount();
	int iColWidth = GetColumnWidth(nCol);
	if (nCol >= nColumnCount || iColWidth < 5) return NULL;

	int offset = 0;
	for (int i = 0; i < nCol; i++) offset += GetColumnWidth(i);

	CRect rect;
	GetItemRect(nItem, &rect, LVIR_BOUNDS);

	//rect.bottom -= iAutoPreviewHeight;

	/*rect.top ++;*/
	rect.bottom -= 1;

	CRect rcClient;
	GetClientRect(&rcClient);

	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;
	GetColumn(nCol, &lvcol);
	DWORD dwStyle = WS_CHILD|WS_VISIBLE;
	if ((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_LEFT) dwStyle |= ES_LEFT;
	else if ((lvcol.fmt&LVCFMT_JUSTIFYMASK) == LVCFMT_RIGHT) dwStyle |= ES_RIGHT|ES_MULTILINE;
	else dwStyle |= ES_CENTER|ES_MULTILINE;

	rect.left += offset+OFFSET_OTHER;
	rect.right = rect.left + GetColumnWidth(nCol)- OFFSET_OTHER- OFFSET_OTHER;
	if (rect.right > rcClient.right) rect.right = rcClient.right;

	DWORD dwData = GetItemData(nItem);

	CString csItem = GetItemText(nItem, nCol);

	CRichEditCtrl *pEdit = new CInEdit(nItem, nCol, csItem, bClick);
	pEdit->Create(dwStyle, rect, this, IDC_IPEDIT);
	
	return pEdit;
}

void CVertexListCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM	*plvItem = &pDispInfo->item;
	*pResult = 0;

	if (plvItem->pszText == NULL) return;
	
	SetItemText(plvItem->iItem, plvItem->iSubItem, plvItem->pszText);
	if(plvItem->iSubItem == 1) m_arrayX[plvItem->iItem] = plvItem->pszText;
	else if(plvItem->iSubItem == 2) m_arrayY[plvItem->iItem] = plvItem->pszText;

}