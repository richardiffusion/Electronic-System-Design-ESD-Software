#pragma once

#include "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditRoundrect �Ի���

class CDlg_EditRoundrect : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditRoundrect)

public:
	CDlg_EditRoundrect(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_EditRoundrect();

// �Ի�������
	enum { IDD = IDD_EDIT_ROUNDRECT };

	CBitmap_Transparent_Dialog m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboWidth;
	BOOL m_bCheckDrawSolid;
	CString m_csEditLocationX1;
	CString m_csEditLocationY1;
	CString m_csEditLocationX2;
	CString m_csEditLocationY2;
	CString m_csEditRadiusX;
	CString m_csEditRadiusY;

	CStatic_Color m_cFillColor;
	COLORREF m_nFillColor;
	CStatic_Color m_cBorderColor;
	COLORREF m_nBorderColor;
	afx_msg void OnStnClickedStaticFillcolor();
	afx_msg void OnStnClickedStaticBordercolor();

	BOOL m_bCheckGlobal;
};
