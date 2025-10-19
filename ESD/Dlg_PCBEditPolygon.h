#pragma once

#include  "Bitmap_Transparent_Dialog.h"

// CDlg_PCBEditPolygon 对话框

class CDlg_PCBEditPolygon : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditPolygon)

public:
	CDlg_PCBEditPolygon(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBEditPolygon();

// 对话框数据
	enum { IDD = IDD_PCB_EDIT_POLYGON };

	CBitmap_Transparent_Dialog	m_Bitmap1_1;
	CBitmap_Transparent_Dialog	m_Bitmap1_2;
	CBitmap_Transparent_Dialog	m_Bitmap1_3;
	CBitmap_Transparent_Dialog	m_Bitmap1_4;
	CBitmap_Transparent_Dialog	m_Bitmap2;
	CBitmap_Transparent_Dialog	m_Bitmap3;

	int m_nFillMode;
	int m_nHatchStyle;
	int m_nPadStyle;

	CStringArray arrayString;
	CStringArray arrayStringNet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL m_bCheckRemoveIsland;
	CString m_csEditArcPrecision;
	BOOL m_bCheckRemoveNeck;
	CString m_csEditRemoveNeck;
	CString m_csEditTrackWidth;
	CString m_csEditGridSize;
	int m_nComboLayer;
	int m_nEditRemoveIsland;
	CString m_csName;
	CString m_csEditMinLength;
	BOOL m_bCheckPrimitiveLock;
	BOOL m_bCheckLocked;
	BOOL m_bCheckIgnoreViolation;
	int m_nComboPourStyle;
	BOOL m_bCheckRemoveDead;
	afx_msg void OnBnClickedRadioFillmode1();
	afx_msg void OnBnClickedRadioFillmode2();
	afx_msg void OnBnClickedRadioFillmode3();
	afx_msg void OnBnClickedCheckRemoveIsland();
	afx_msg void OnBnClickedCheckRemoveNeck();
	CString m_csComboNet;
	CString m_csEditPourGap;
};

