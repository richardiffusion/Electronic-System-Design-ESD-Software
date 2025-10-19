#pragma once

#include  "Bitmap_Transparent_Dialog.h"

// CDlg_PCBEditCoordinate 对话框

class CDlg_PCBEditCoordinate : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditCoordinate)

public:
	CDlg_PCBEditCoordinate(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBEditCoordinate();

	CBitmap_Transparent_Dialog	m_Bitmap;

// 对话框数据
	enum { IDD = IDD_PCB_EDIT_COORDINATE };

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
	CString m_csEditSize;
	CString m_csEditLineWidth;
	CString m_csEditTextLineWidth;
	CString m_csEditTextHeight;
	CString m_csEditLocationX;
	CString m_csEditLocationY;
	BOOL m_bCheckLocked;
	BOOL m_bCheckBold;
	BOOL m_bCheckItalic;
	int m_nComboFont;
	int m_nComboStyle;
	int m_nComboLayer;
	afx_msg void OnBnClickedRadioStroke();
	afx_msg void OnBnClickedRadioTruetype();
};
