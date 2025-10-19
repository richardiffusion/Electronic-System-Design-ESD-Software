#pragma once

#include "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditRect 对话框

class CDlg_EditRect : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditRect)

public:
	CDlg_EditRect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_EditRect();

// 对话框数据
	enum { IDD = IDD_EDIT_RECT };

	CBitmap_Transparent_Dialog m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboWidth;

	BOOL m_bCheckDrawSolid;
	BOOL m_bCheckTransparent;
	CString m_csEditLocationX1;
	CString m_csEditLocationY1;
	CString m_csEditLocationX2;
	CString m_csEditLocationY2;
	CStatic_Color m_cFillColor;
	COLORREF m_nFillColor;
	CStatic_Color m_cBorderColor;
	COLORREF m_nBorderColor;
	afx_msg void OnStnClickedStaticFillcolor();
	afx_msg void OnStnClickedStaticBordercolor();

	BOOL m_bCheckGlobal;
};
