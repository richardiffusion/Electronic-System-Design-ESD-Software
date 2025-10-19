#pragma once

#include "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditPower �Ի���

class CDlg_EditPower : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditPower)

public:
	CDlg_EditPower(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_EditPower();

// �Ի�������
	enum { IDD = IDD_EDIT_POWER };
	int m_nPowerOrGround;

	CBitmap_Transparent_Dialog m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboOrientation;
	CString m_csEditLocationX;
	CString m_csEditLocationY;
	int m_nComboStyle;
	CString m_csEditNet;
	BOOL m_bCheckShowNet;

	CStatic_Color m_cColor;
	COLORREF m_nColor;
	afx_msg void OnStnClickedStaticColor();

	afx_msg void OnCbnSelchangeComboStyle();
	BOOL m_bCheckGlobal;
};
