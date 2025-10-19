#pragma once

#include  "Bitmap_Transparent_Dialog.h"

// CDlg_PCBEditDimension 对话框

class CDlg_PCBEditDimension : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditDimension)

public:
	CDlg_PCBEditDimension(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBEditDimension();
	
	CBitmap_Transparent_Dialog	m_Bitmap;

// 对话框数据
	enum { IDD = IDD_PCB_EDIT_DIMENSION };

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
	CString m_csEditTextHeight;
	CString m_csEditEndY;
	CString m_csEditEndX;
	CString m_csEditLineHeight;
	CString m_csEditTextWidth;
	CString m_csEditLineWidth;
	CString m_csEditStartX;
	CString m_csEditStartY;
	int m_nComboLayer;
	int m_nComboStyle;
	int m_nComboFont;
	BOOL m_bCheckLocked;
	BOOL m_bCheckBold;
	BOOL m_bCheckItalic;
	afx_msg void OnBnClickedRadioStroke();
	afx_msg void OnBnClickedRadioTruetype();
};
