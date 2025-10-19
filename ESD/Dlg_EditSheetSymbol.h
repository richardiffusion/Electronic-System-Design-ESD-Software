#pragma once

#include "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditSheetSymbol 对话框

class CDlg_EditSheetSymbol : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditSheetSymbol)

public:
	CDlg_EditSheetSymbol(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_EditSheetSymbol();

// 对话框数据
	enum { IDD = IDD_EDIT_SHEETSYMBOL };

	CBitmap_Transparent_Dialog m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboWidth;

	BOOL m_bCheckDrawSolid;
	CString m_csEditDesignator;
	CString m_csEditFilename;
	CString m_csEditLocationX;
	CString m_csEditLocationY;
	CString m_csEditSizeX;
	CString m_csEditSizeY;		
	CStatic_Color m_cFillColor;
	COLORREF m_nFillColor;
	CStatic_Color m_cBorderColor;
	COLORREF m_nBorderColor;
	afx_msg void OnStnClickedStaticFillcolor();
	afx_msg void OnStnClickedStaticBordercolor();

};
