#pragma once

#include "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditPie �Ի���

class CDlg_EditPie : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditPie)

public:
	CDlg_EditPie(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_EditPie();

// �Ի�������
	enum { IDD = IDD_EDIT_PIE };

	CBitmap_Transparent_Dialog m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_bCheckDrawSolid;
	int m_nComboWidth;
	CString m_csEditRadius;
	CString m_csEditLocationX;
	CString m_csEditLocationY;
	double m_fEditStartAngle;
	double m_fEditEndAngle;

	CStatic_Color m_cFillColor;
	COLORREF m_nFillColor;
	CStatic_Color m_cBorderColor;
	COLORREF m_nBorderColor;
	afx_msg void OnStnClickedStaticFillcolor();
	afx_msg void OnStnClickedStaticBordercolor();

	BOOL m_bCheckGlobal;
};
