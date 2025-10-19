#pragma once

#include "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditEllipseFill 对话框

class CDlg_EditEllipseFill : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditEllipseFill)

public:
	CDlg_EditEllipseFill(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_EditEllipseFill();

// 对话框数据
	enum { IDD = IDD_EDIT_ELLIPSEFILL };

	CBitmap_Transparent_Dialog m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboWidth;
	
	CString m_csEditRadiusX;
	CString m_csEditRadiusY;
	BOOL m_bCheckDrawSolid;
	BOOL m_bCheckTransparent;

	CStatic_Color m_cFillColor;
	COLORREF m_nFillColor;
	CStatic_Color m_cBorderColor;
	COLORREF m_nBorderColor;
	afx_msg void OnStnClickedStaticFillcolor();
	afx_msg void OnStnClickedStaticBordercolor();

	CString m_csEditLocationX;
	CString m_csEditLocationY;
	BOOL m_bCheckGlobal;
};
