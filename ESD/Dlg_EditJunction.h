#pragma once

#include  "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditJunction �Ի���

class CDlg_EditJunction : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditJunction)

public:
	CDlg_EditJunction(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_EditJunction();

// �Ի�������
	enum { IDD = IDD_EDIT_JUNCTION };

	CBitmap_Transparent_Dialog	m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_csEditLocationX;
	CString m_csEditLocationY;
	int m_nComboJunctionSize;

	CStatic_Color m_cColor;
	COLORREF m_nColor;
	afx_msg void OnStnClickedStaticColor();

	BOOL m_bCheckGlobal;
};
