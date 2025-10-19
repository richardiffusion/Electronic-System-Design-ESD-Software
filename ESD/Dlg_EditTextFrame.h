#pragma once

#include  "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditTextFrame 对话框

class CDlg_EditTextFrame : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditTextFrame)

public:
	CDlg_EditTextFrame(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_EditTextFrame();

// 对话框数据
	enum { IDD = IDD_EDIT_TEXTFRAME };

	BOOL m_bBold;
	BOOL m_bItalic;
	BOOL m_bUnderline;
	CString m_csFontName;
	CString m_csFontSize;

	CBitmap_Transparent_Dialog	m_Bitmap;

	CString m_csText;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboWidth;
	int m_nComboAlignment;
	BOOL m_bCheckShowBorder;
	BOOL m_bCheckDrawSolid;
	BOOL m_bCheckWordWrap;
	BOOL m_bCheckClip;
	CString m_csEditLocationX1;
	CString m_csEditLocationY1;
	CString m_csEditLocationX2;
	CString m_csEditLocationY2;
	CStatic_Color m_cFillColor;
	COLORREF m_nFillColor;
	CStatic_Color m_cBorderColor;
	COLORREF m_nBorderColor;
	CStatic_Color m_cTextColor;
	COLORREF m_nTextColor;
	afx_msg void OnStnClickedStaticBordercolor();
	afx_msg void OnStnClickedStaticFillcolor();
	afx_msg void OnStnClickedStaticTextcolor();
	afx_msg void OnBnClickedButtonFont();
	afx_msg void OnBnClickedButtonText();
};
