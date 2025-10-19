#pragma once

#include "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditSheetEntry �Ի���

class CDlg_EditSheetEntry : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditSheetEntry)

public:
	CDlg_EditSheetEntry(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_EditSheetEntry();

// �Ի�������
	enum { IDD = IDD_EDIT_SHEETENTRY };

	CBitmap_Transparent_Dialog m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboStyle;
	int m_nComboSide;
	CString m_csEditName;
	CString m_csEditPosition;
	int m_nComboType;

	CStatic_Color m_cFillColor;
	COLORREF m_nFillColor;
	CStatic_Color m_cBorderColor;
	COLORREF m_nBorderColor;
	CStatic_Color m_cTextColor;
	COLORREF m_nTextColor;
	afx_msg void OnStnClickedStaticTextcolor();
	afx_msg void OnStnClickedStaticFillcolor();
	afx_msg void OnStnClickedStaticBordercolor();

};
