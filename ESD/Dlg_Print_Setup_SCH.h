#pragma once

#include  "Bitmap_Transparent_Dialog.h"

// CDlg_Print_Setup_SCH 对话框

class CDlg_Print_Setup_SCH : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Print_Setup_SCH)

public:
	CDlg_Print_Setup_SCH(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Print_Setup_SCH();

// 对话框数据
	enum { IDD = IDD_PRINT_SETUP_SCH };

	CBitmap_Transparent_Dialog	m_Bitmap1;
	CBitmap_Transparent_Dialog	m_Bitmap2;

	CBitmap_Transparent_Dialog	m_BitmapS01;
	CBitmap_Transparent_Dialog	m_BitmapS02;
	CBitmap_Transparent_Dialog	m_BitmapS03;
	CBitmap_Transparent_Dialog	m_BitmapS04;
	CBitmap_Transparent_Dialog	m_BitmapWindow;
	CBitmap_Transparent_Dialog	m_BitmapSheet;

	int m_nCurrentPaperSizeNo;
	int m_nTotalPaper;
	int m_nPaper[300];
	char m_szPaperName[300][64];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSetupPrinter();
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnBnClickedButtonPrintPreview();
	int m_nRadioOrientation;
	virtual BOOL OnInitDialog();
	int m_nComboPaperSize;
	double m_fEditScale;
	int m_nComboScaleMode;
	int m_nRadioColor;
	BOOL m_bCheckHCenter;
	BOOL m_bCheckVCenter;
	double m_fEditHMargin;
	double m_fEditVMargin;
	int m_nRadioExtent;
	int m_nRadioSheet;
	afx_msg void OnBnClickedCheckHCenter();
	afx_msg void OnBnClickedCheckVCenter();
	afx_msg void OnCbnSelchangeComboScalemode();
	afx_msg void OnBnClickedRadioExtent();
	afx_msg void OnBnClickedRadio7();
	CString m_csEditCorrectionX;
	CString m_csEditCorrectionY;
};
