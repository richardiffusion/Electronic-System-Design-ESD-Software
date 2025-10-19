#pragma once

#include "Bitmap_Transparent_Page.h"
#include "Static_Color.h"


class CPage_EditBus : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_EditBus)

public:
	CPage_EditBus();
	virtual ~CPage_EditBus();

	enum { IDD = IDD_EDIT_BUS };

	HBRUSH m_hBkBrush;
	//CStatic_SheetCtrlBackground m_cStaticWidth;
	CBitmap_Transparent_Page m_Bitmap;
	//CBitmap_Transparent_Dialog m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);  

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
