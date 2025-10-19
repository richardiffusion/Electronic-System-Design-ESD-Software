#pragma once

#include "Static_Color.h"

// CDlg_Perference 对话框

class CDlg_Perference : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Perference)

public:
	CDlg_Perference(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Perference();

// 对话框数据
	enum { IDD = IDD_PERFERENCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CStatic_Color m_cSelColor;
	COLORREF m_nSelColor;
	CStatic_Color m_cGridColor;
	COLORREF m_nGridColor;
	CStatic_Color m_cAutoJuncColor;
	COLORREF m_nAutoJuncColor;

	int m_nComboCursor;
	BOOL m_bCheckAutoJunc;
	BOOL m_bCheckAutoWire;
	BOOL m_bCheckAutoDiagram;
	BOOL m_bCheckBackup;
	int m_nEditBackup;
	double m_fEditNameOffset;
	double m_fEditNumberOffset;
	double m_fEditI1;
	double m_fEditI2;
	double m_fEditI3;
	double m_fEditM1;
	double m_fEditM2;
	double m_fEditM3;
	CString m_csEditPowerGround;
	CString m_csEditSignalGround;
	CString m_csEditEarth;
	
	int m_nComboGrid;
	int m_nRadioIDSuffix;

	BOOL m_bCheckClearSel;
	int m_nComboSheetSize;
	virtual BOOL OnInitDialog();

	afx_msg void OnStnClickedStaticGridcolor();
	afx_msg void OnBnClickedButtonDefault();
	afx_msg void OnStnClickedStaticAutojunccolor();
	afx_msg void OnStnClickedStaticSelcolor();
};
