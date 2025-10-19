#pragma once

#include  "Bitmap_Transparent_Page.h"
#include "Static_Color.h"

// CPage_EditPolyline 对话框

class CPage_EditPolyline : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_EditPolyline)

public:
	CPage_EditPolyline();
	virtual ~CPage_EditPolyline();

// 对话框数据
	enum { IDD = IDD_EDIT_POLYLINE };

	HBRUSH m_hBkBrush;
	CBitmap_Transparent_Page	m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboLinewidth();
	int m_nComboWidth;
	int m_nComboLineType;
	int m_nComboStartLineShape;
	int m_nComboEndLineShape;
	int m_nComboLineShapeSize;

	CStatic_Color m_cColor;
	COLORREF m_nColor;
	afx_msg void OnStnClickedStaticColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL m_bCheckGlobal;
};
