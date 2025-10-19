#pragma once
#include "ESDDoc.h"

// CDlg_PCBEditPad 对话框

class CDlg_PCBEditPad : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditPad)

public:
	CDlg_PCBEditPad(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBEditPad();

// 对话框数据
	enum { IDD = IDD_PCB_EDIT_PAD };

	CStringArray arrayStringLayer;
	CStringArray arrayStringNet;
	CXTTabCtrl m_ctrlTab;
	CESDDoc* pDoc;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CString m_csEditPadX;
	CString m_csEditPadY;

	double m_fEditRotation;
	CString m_csEditHoleSize;
	double m_fHoleRotation;
	CString m_csEditHoleWidth;
	CString m_csEditDesignator;
	int m_nComboLayer;
	int m_nComboType;
	BOOL m_bCheckPlated;
	BOOL m_bCheckLocked;
	CString m_csEditXSizeTop;
	CString m_csEditYSizeTop;
	int m_nComboShapeTop;
	CString m_csEditXSizeMid;
	CString m_csEditYSizeMid;
	int m_nComboShapeMid;
	CString m_csEditXSizeBottom;
	CString m_csEditYSizeBottom;
	int m_nComboShapeBottom;
	BOOL m_bCheckTentingTop;
	BOOL m_bCheckTentingBottom;
	CString m_csEditPaste;
	CString m_csEditSolder;
	int m_nRadioHoleType;
	int m_nRadioPadMode;
	int m_nRadioPaste;
	int m_nRadioSolder;

	void DrawPad();
	int GetLayerNumber(int index);

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioPadMode();
	afx_msg void OnBnClickedRadioPadMode2();
	afx_msg void OnBnClickedRadioHoleshape();
	afx_msg void OnBnClickedRadioHoleshape1();
	afx_msg void OnBnClickedRadioHoleshape2();
	afx_msg void OnBnClickedRadioPaste();
	afx_msg void OnBnClickedRadioPaste1();
	afx_msg void OnBnClickedRadioSolder();
	afx_msg void OnBnClickedRadioSolder1();
	afx_msg void OnNMClickTabCtrl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeComboLayer();
	CString m_csComboNet;
	afx_msg void OnBnClickedCheckTentingBottom();
	afx_msg void OnBnClickedCheckTentingTop();
	afx_msg void OnCbnSelchangeComboNet();
	afx_msg void OnCbnSelchangeComboShapeBottom();
	afx_msg void OnCbnSelchangeComboShapeMid();
	afx_msg void OnCbnSelchangeComboShapeTop();
	afx_msg void OnEnChangeEditHoleRotation();
	afx_msg void OnEnChangeEditHoleWidth();
	afx_msg void OnEnChangeEditHolesize();
	afx_msg void OnEnChangeEditPadRotation();
	afx_msg void OnEnChangeEditPadX();
	afx_msg void OnEnChangeEditPadY();
	afx_msg void OnEnChangeEditPaste();
	afx_msg void OnEnChangeEditPcbDesignator();
	afx_msg void OnEnChangeEditSolder();
	afx_msg void OnEnChangeEditXsizeBottom();
	afx_msg void OnEnChangeEditXsizeMid();
	afx_msg void OnEnChangeEditXsizeTop();
	afx_msg void OnEnChangeEditYsizeBottom();
	afx_msg void OnEnChangeEditYsizeMid();
	afx_msg void OnEnChangeEditYsizeTop();
};
