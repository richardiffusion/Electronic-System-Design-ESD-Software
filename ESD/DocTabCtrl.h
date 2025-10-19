#pragma once


// CDocTabCtrl

class CDocTabCtrl : public CXTPTabClientWnd
{
	DECLARE_DYNAMIC(CDocTabCtrl)

public:
	CDocTabCtrl();
	virtual ~CDocTabCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTabFileClose();
};


