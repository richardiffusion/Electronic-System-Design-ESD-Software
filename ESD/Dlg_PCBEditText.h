#pragma once

#include  "Bitmap_Transparent_Dialog.h"

// CDlg_PCBEditText 对话框

class CDlg_PCBEditText : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditText)

public:
	CDlg_PCBEditText(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBEditText();

	CBitmap_Transparent_Dialog	m_Bitmap;
	CStringArray arrayFont;
	CStringArray arrayString;
	CString csTitle;

	int nStrokeFont;
	int nTTFont;
	CString m_csStrokeFontName[3];

// 对话框数据
	enum { IDD = IDD_PCB_EDIT_TEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_csEditWidth;
	CString m_csEditHeight;
	CString m_csEditX;
	CString m_csEditY;
	CString m_csEditText;
	BOOL m_bCheckLocked;
	BOOL m_bCheckMirror;
	BOOL m_bCheckBold;
	BOOL m_bCheckItalic;
	BOOL m_bCheckInverted;
	CString m_csEditInverted;

	int  m_nRadioFont;
	double m_fEditRotation;
	afx_msg void OnBnClickedRadioStroke();
	afx_msg void OnBnClickedRadioTruetype();
	int m_nComboLayer;
	int m_nComboFont;
};

