#pragma once

#include "Bitmap_Transparent_Page.h"
#include "Static_Color.h"

// CPage_EditWire 对话框

class CPage_EditWire : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_EditWire)

public:
	CPage_EditWire();
	virtual ~CPage_EditWire();

// 对话框数据
	enum { IDD = IDD_EDIT_WIRE };

	HBRUSH m_hBkBrush;
	CBitmap_Transparent_Page m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboWidth;
	CStatic_Color m_cColor;
	COLORREF m_nColor;
	afx_msg void OnStnClickedStaticColor();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL m_bCheckGlobal;
};
