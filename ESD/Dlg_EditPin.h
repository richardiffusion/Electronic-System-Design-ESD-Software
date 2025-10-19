#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "Static_Color.h"
#include "Static_PinDemo.h"
#include "ESDDoc.h"

// CDlg_EditPin 对话框

class CDlg_EditPin : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditPin)

public:
	CDlg_EditPin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_EditPin();

// 对话框数据
	enum { IDD = IDD_EDIT_PIN };
	CESDDoc* pDoc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditPinName;
	CString m_csEditPinNumber;
	int m_nComboType;
	CString m_csEditDescription;
	CString m_csEditConnectTo;
	CString m_csEditLocationX;
	CString m_csEditLocationY;
	CString m_csEditPinLength;
	int m_nComboOrientation;
	int m_nComboInside;
	int m_nComboInsideEdge;
	int m_nComboOutsideEdge;
	int m_nComboOutside;
	int m_nEditPartNum;
	CEdit m_editPartNum;
	CSpinButtonCtrl m_spinPartNum;
	
	CStatic_Color m_cColor;
	COLORREF m_nColor;
	CStatic_PinDemo m_cPinDemo;

	void DrawPin();
	BOOL first_show;

	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedStaticColor();
	BOOL m_bCheckPinHide;
	BOOL m_bCheckNameVisible;
	BOOL m_bCheckNumberVisible;
	CString m_csEditUniqueID;
	afx_msg void OnBnClickedCheckHide();

	afx_msg void OnPaint();
	afx_msg void OnBnClickedCheckPinnameVisible();
	afx_msg void OnBnClickedCheckPinnumberVisible();
	afx_msg void OnCbnSelchangeComboElectricalType();
	afx_msg void OnCbnSelchangeComboInside();
	afx_msg void OnCbnSelchangeComboInsideEdge();
	afx_msg void OnCbnSelchangeComboOutside();
	afx_msg void OnCbnSelchangeComboOutsideEdge();
	afx_msg void OnCbnSelchangeComboPinOrientation();
	afx_msg void OnEnChangeEditPinLength();
	afx_msg void OnEnChangeEditPinname();
	afx_msg void OnEnChangeEditPinnumber();
};
