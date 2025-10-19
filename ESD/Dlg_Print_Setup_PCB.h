#pragma once

#include  "Bitmap_Transparent_Dialog.h"

// CDlg_Print_Setup_PCB 对话框

class CDlg_Print_Setup_PCB : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Print_Setup_PCB)

public:
	CDlg_Print_Setup_PCB(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_Print_Setup_PCB();

// 对话框数据
	enum { IDD = IDD_PRINT_SETUP_PCB };
	
	CBitmap_Transparent_Dialog	m_Bitmap1;
	CBitmap_Transparent_Dialog	m_Bitmap2;

	CBitmap_Transparent_Dialog	m_BitmapSheet;	
	CBitmap_Transparent_Dialog	m_BitmapBoard;

	int m_nCurrentPaperSizeNo;
	int m_nTotalPaper;
	int m_nPaper[300];
	char m_szPaperName[300][64];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboPaperSize;
	int m_nComboScaleMode;
	double m_fEditScale;
	double m_fEditHMargin;
	double m_fEditVMargin;
	BOOL m_bCheckHCenter;
	BOOL m_bCheckVCenter;
	CString m_csEditCorrectionX;
	CString m_csEditCorrectionY;
	int m_nRadioOrientation;
	int m_nRadioColor;
	int m_nRadioExtent;
	afx_msg void OnBnClickedButtonSetupPrinter();
	afx_msg void OnBnClickedButtonSetupAdvanced();
	afx_msg void OnBnClickedButtonPrintPreview();
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnBnClickedCheckHCenter();
	afx_msg void OnBnClickedCheckVCenter();
	afx_msg void OnCbnSelchangeComboScalemode();
	CString m_csEditX1;
	CString m_csEditY1;
	CString m_csEditX2;
	CString m_csEditY2;
	afx_msg void OnBnClickedButtonDefine();
	afx_msg void OnBnClickedRadioXy();
	afx_msg void OnBnClickedRadioBoard();
	afx_msg void OnBnClickedRadioExtent();
};
