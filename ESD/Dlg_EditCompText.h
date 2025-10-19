#pragma once

#include "Static_Color.h"

// CDlg_EditCompText 对话框

class CDlg_EditCompText : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditCompText)

public:
	CDlg_EditCompText(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_EditCompText();
	CString m_csTitle;

// 对话框数据
	enum { IDD = IDD_EDIT_COMPTEXT };
	BOOL m_bBold;
	BOOL m_bItalic;
	BOOL m_bUnderline;
	CString m_csFontName;
	CString m_csFontSize;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditNameComment;
	CString m_csEditText;
	BOOL m_bCheckNameVisible;
	BOOL m_bCheckNameLocked;
	BOOL m_bCheckTextVisible;
	BOOL m_bCheckTextLocked;
	CString m_csEditLocationX;
	CString m_csEditLocationY;
	int m_nComboOrientation;
	BOOL m_bCheckAutoPosition;

	CStatic_Color m_cColor;
	COLORREF m_nColor;
	afx_msg void OnStnClickedStaticColor();
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnBnClickedButtonFont();
	int m_nComboHJustification;
	int m_nComboVJustification;
	BOOL m_bCheckGlobal;
};
