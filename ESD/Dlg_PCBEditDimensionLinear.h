#pragma once

#include  "Bitmap_Transparent_Dialog.h"

// CDlg_PCBEditDimensionLinear 对话框

class CDlg_PCBEditDimensionLinear : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditDimensionLinear)

public:
	CDlg_PCBEditDimensionLinear(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBEditDimensionLinear();

	CBitmap_Transparent_Dialog	m_Bitmap;

// 对话框数据
	enum { IDD = IDD_PCB_EDIT_DIMENSION_LINEAR };

	CStringArray arrayString;
	CStringArray arrayFont;

	int nStrokeFont;
	int nTTFont;
	CString m_csStrokeFontName[3];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nRadioTrueType;
	CString m_csEditArrowLength;
	CString m_csEditGap;
	CString m_csEditArrowWidth;
	CString m_csEditTextHeight;
	CString m_csEditTextWidth;
	CString m_csEditTextGap;
	CString m_csEditExtWidth;
	CString m_csEditLineWidth;
	CString m_csEditOffset;
	double m_fEditRotation;
	int m_nComboUnit;
	int m_nComboPrecision;
	CString m_csEditPrefix;
	CString m_csEditSuffix;
	int m_nComboTextPosition;
	int m_nComboArrowPosition;
	BOOL m_bCheckLocked;
	int m_nComboFont;
	BOOL m_bCheckBold;
	BOOL m_bCheckItalic;
	int m_nComboLayer;
	afx_msg void OnBnClickedRadioStroke();
	afx_msg void OnBnClickedRadioTruetype();
	int m_nComboFormat;
};
