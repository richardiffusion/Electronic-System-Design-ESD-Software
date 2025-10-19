#pragma once

//#include "struct.h"

#define WM_USER_TAB WM_USER + 10

class CVertexListCtrl : public CXTListCtrl
{
	DECLARE_DYNAMIC(CVertexListCtrl)

public:
	CVertexListCtrl();
	virtual ~CVertexListCtrl();

	enum { OFFSET_FIRST = 2, OFFSET_OTHER = 6 };

	int iSubItemFocus; //当前被编辑的subItem列号

	
	//CArray<Fpoint, Fpoint&> m_cVertex;
	CStringArray m_arrayX;
	CStringArray m_arrayY;

	int HitTestEx(CPoint & point, int * col) const;
	int GetColumnCount() const;
	void GetSubItemRect(const int nItem, const int nCol, CRect &rcItem);
	int GetColumnIndex(const int iCol) const;
	bool EnsureSubItemVisible(const int iSubItem);
	CRichEditCtrl * EditSubLabel(int nItem, int nCol, bool bClick = true);

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};


