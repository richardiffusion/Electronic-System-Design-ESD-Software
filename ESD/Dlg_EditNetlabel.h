#pragma once

#include "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditNetlabel �Ի���

class CDlg_EditNetlabel : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditNetlabel)

public:
	CDlg_EditNetlabel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_EditNetlabel();

	BOOL m_bBold;
	BOOL m_bItalic;
	BOOL m_bUnderline;
	CString m_csFontName;
	CString m_csFontSize;

// �Ի�������
	enum { IDD = IDD_EDIT_NETLABEL };

	CBitmap_Transparent_Dialog m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboOrientation;
	CString m_csEditLocationX;
	CString m_csEditLocationY;
	CString m_csEditNet;

	CStatic_Color m_cColor;
	COLORREF m_nColor;
	afx_msg void OnStnClickedStaticColor();
	afx_msg void OnBnClickedButtonFont();
	BOOL m_bCheckGlobal;
};
