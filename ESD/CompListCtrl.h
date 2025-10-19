#pragma once

#include "struct.h"

#define WM_USER_TAB WM_USER + 10

class CCompListCtrl : public CXTListCtrl
{
	DECLARE_DYNAMIC(CCompListCtrl)

public:
	CCompListCtrl();
	virtual ~CCompListCtrl();

	enum { OFFSET_FIRST = 2, OFFSET_OTHER = 6 };

	int iSubItemFocus;

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

