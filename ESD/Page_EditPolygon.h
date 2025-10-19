#pragma once

#include "Bitmap_Transparent_Page.h"
#include "Static_Color.h"

// CPage_EditPolygon 对话框

class CPage_EditPolygon : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_EditPolygon)

public:
	CPage_EditPolygon();
	virtual ~CPage_EditPolygon();

// 对话框数据
	enum { IDD = IDD_EDIT_POLYGON };

	HBRUSH m_hBkBrush;
	CBitmap_Transparent_Page m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboWidth;
	BOOL m_bCheckDrawSolid;
	BOOL m_bCheckTransparent;

	CStatic_Color m_cFillColor;
	COLORREF m_nFillColor;
	CStatic_Color m_cBorderColor;
	COLORREF m_nBorderColor;
	afx_msg void OnStnClickedStaticFillcolor();
	afx_msg void OnStnClickedStaticBordercolor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL m_bCheckGlobal;
};
