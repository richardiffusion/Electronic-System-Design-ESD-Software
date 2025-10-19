#pragma once


#include "ESDDoc.h"
// CPage_SchUnit 对话框

class CPage_SchUnit : public CPropertyPage
{
	DECLARE_DYNAMIC(CPage_SchUnit)

public:
	CPage_SchUnit();
	virtual ~CPage_SchUnit();

	CESDDoc* m_pDoc;

// 对话框数据
	enum { IDD = IDD_SCH_SHEET_UNITS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckI;
	BOOL m_bCheckM;
	int m_nComboI;
	int m_nComboM;

	void SetStaticUnit();

	afx_msg void OnBnClickedCheckI();
	afx_msg void OnBnClickedCheckM();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboM();
	afx_msg void OnCbnSelchangeComboI();
};
