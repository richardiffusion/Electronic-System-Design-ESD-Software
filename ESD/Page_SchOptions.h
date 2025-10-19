#pragma once
#include "afxwin.h"

#include "ESDDoc.h"
#include "Static_Color.h"

// CPage_SchOptions 对话框

class CPage_SchOptions : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_SchOptions)

public:
	CPage_SchOptions();
	virtual ~CPage_SchOptions();

// 对话框数据
	enum { IDD = IDD_SCH_SHEET_OPTIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_nComboSheetNum;
	CString m_csEditSheetName;
	BOOL m_bCheckSheetEnable;
	int m_nComboOrientation;
	BOOL m_bCheckTitle;
	int m_nComboTitle;
	int m_nEditNumberSpace;
	BOOL m_bCheckReference;
	BOOL m_bCheckShowBorder;
	int m_nEditStandard;
	BOOL m_bCheckSnap;
	BOOL m_bCheckVisible;
	BOOL m_bCheckElectrical;
	CString m_csEditSnap;
	CString m_csEditVisible;
	CString m_csEditElectrical;

	BOOL m_bCheckCustom;
	CString m_csEditCustomWidth;
	CString m_csEditCustomHeight;
	int m_nEditXCount;
	int m_nEditYCount;
	CString m_csEditMargin;

	CESDDoc* m_pDoc;

	CStatic_Color m_cBorderColor;
	CStatic_Color m_cSheetColor;

	afx_msg void OnCbnSelchangeComboSheetnum();
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedCheckCustom();
	afx_msg void OnBnClickedButtonUpdate();

	afx_msg void OnBnClickedButtonAllon();
	afx_msg void OnBnClickedButtonUsedon();
	afx_msg void OnStnClickedStaticBordercolor();
	afx_msg void OnStnClickedStaticSheetcolor();
	afx_msg void OnBnClickedButtonBordercolorDefault();
	afx_msg void OnBnClickedButtonSheetcolorDefault();

	afx_msg void OnBnClickedButtonUserSheet();
};
