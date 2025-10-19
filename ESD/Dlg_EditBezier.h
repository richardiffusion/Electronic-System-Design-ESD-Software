#pragma once

#include  "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditBezier �Ի���

class CDlg_EditBezier : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditBezier)

public:
	CDlg_EditBezier(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_EditBezier();

// �Ի�������
	enum { IDD = IDD_EDIT_BEZIER };

	CBitmap_Transparent_Dialog	m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboWidth;

	CStatic_Color m_cColor;
	COLORREF m_nColor;
	afx_msg void OnStnClickedStaticColor();
	BOOL m_bCheckGlobal;
};
