#pragma once

#include "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditPort 对话框

class CDlg_EditPort : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditPort)

public:
	CDlg_EditPort(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_EditPort();

// 对话框数据
	enum { IDD = IDD_EDIT_PORT };

	CBitmap_Transparent_Dialog m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_csEditSize;
	CString m_csEditLocationX;
	CString m_csEditLocationY;
	CStatic_Color m_cFillColor;
	COLORREF m_nFillColor;
	CStatic_Color m_cBorderColor;
	COLORREF m_nBorderColor;
	CStatic_Color m_cTextColor;
	COLORREF m_nTextColor;
	afx_msg void OnStnClickedStaticTextcolor();
	afx_msg void OnStnClickedStaticFillcolor();
	afx_msg void OnStnClickedStaticBordercolor();
	CString m_csEditName;
	int m_nComboType;
	int m_nComboStyle;
	int m_nComboAlignment;

	afx_msg void OnCbnSelchangeComboStyle();

	BOOL m_bCheckGlobal;
};
