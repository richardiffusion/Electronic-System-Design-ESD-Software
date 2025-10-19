#pragma once

#include  "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditText �Ի���

class CDlg_EditText : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditText)

public:
	CDlg_EditText(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_EditText();

	BOOL m_bBold;
	BOOL m_bItalic;
	BOOL m_bUnderline;
	CString m_csFontName;
	CString m_csFontSize;


// �Ի�������
	enum { IDD = IDD_EDIT_TEXT };

	CBitmap_Transparent_Dialog	m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_csEditText;
	CString m_csEditLocationX;
	CString m_csEditLocationY;
	int m_nComboOrientation;

	CStatic_Color m_cColor;
	COLORREF m_nColor;
	afx_msg void OnStnClickedStaticColor();

	afx_msg void OnBnClickedButtonFont();

	int m_nComboHJustification;
	int m_nComboVJustification;
	BOOL m_bCheckMirror;
	BOOL m_bCheckGlobal;
};
