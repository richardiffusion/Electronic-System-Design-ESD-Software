#pragma once


// CDlg_EditSheetSymbolDF �Ի���
#include "Static_Color.h"

class CDlg_EditSheetSymbolDF : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditSheetSymbolDF)

public:
	CDlg_EditSheetSymbolDF(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_EditSheetSymbolDF();
	CString m_csTitle;

// �Ի�������
	enum { IDD = IDD_EDIT_SHEETSYMBOL_DF };
	
	BOOL m_bBold;
	BOOL m_bItalic;
	BOOL m_bUnderline;
	CString m_csFontName;
	CString m_csFontSize;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditDF;

	int m_nComboOrientation;
	BOOL m_bCheckAutoPosition;
	BOOL m_bCheckHide;
	CString m_csEditLocationX;
	CString m_csEditLocationY;	
	CStatic_Color m_cColor;
	COLORREF m_nColor;
	afx_msg void OnStnClickedStaticColor();
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	afx_msg void OnBnClickedButtonFont();
};
