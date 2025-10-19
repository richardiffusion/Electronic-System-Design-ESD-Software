#pragma once

#include  "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditGraphic �Ի���

class CDlg_EditGraphic : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditGraphic)

public:
	CDlg_EditGraphic(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_EditGraphic();

// �Ի�������
	enum { IDD = IDD_EDIT_GRAPHIC };

	CBitmap_Transparent_Dialog	m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboWidth;
	CString m_csEditLocationX1;
	CString m_csEditLocationY1;
	CString m_csEditLocationX2;
	CString m_csEditLocationY2;
	CString m_csEditFilename;
	BOOL m_bCheckShowBorder;
	BOOL m_bCheckXYRatio;

	CStatic_Color m_cColor;
	COLORREF m_nColor;
	afx_msg void OnStnClickedStaticBordercolor();
	afx_msg void OnBnClickedButtonBrowse();

};
