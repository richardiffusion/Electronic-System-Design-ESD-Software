#pragma once

#include "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_EditEllipse �Ի���

class CDlg_EditEllipse : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditEllipse)

public:
	CDlg_EditEllipse(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_EditEllipse();

// �Ի�������
	enum { IDD = IDD_EDIT_ELLIPSE };

	CBitmap_Transparent_Dialog m_Bitmap;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboWidth;
	CString m_csEditLocationX;
	CString m_csEditLocationY;
	CString m_csEditRadiusX;
	CString m_csEditRadiusY;
	double m_fEditStartAngle;
	double m_fEditEndAngle;

	CStatic_Color m_cColor;
	COLORREF m_nColor;
	afx_msg void OnStnClickedStaticColor();

	BOOL m_bCheckGlobal;
};
