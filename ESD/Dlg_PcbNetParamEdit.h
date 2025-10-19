#pragma once

#include  "Bitmap_Transparent_Dialog.h"
#include "Static_Color.h"

// CDlg_PcbNetParamEdit 对话框

class CDlg_PcbNetParamEdit : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbNetParamEdit)

public:
	CDlg_PcbNetParamEdit(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PcbNetParamEdit();

	CBitmap_Transparent_Dialog	m_Bitmap;

	CStatic_Color m_cColor;
	COLORREF m_nColor;

	CXTPReportControl m_wndSignalCtrl;
	CXTPReportControl m_wndPinsOld;
	CXTPReportControl m_wndPinsNew;

	double m_fTrackWidth[SIGNAL_LAYER_NUM];

	int nNetIndex;

// 对话框数据
	enum { IDD = IDD_PCB_NET_PARAM_EDIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CDocument* pDocument;
	void SingalReportCtrl(BOOL use_stack_layers_only, BOOL create_column);
	void PinsOldCtrl(BOOL create_column);
	void PinsNewCtrl(BOOL create_column);
	virtual BOOL OnInitDialog();
	afx_msg void OnStnClickedStaticColor();
	CString m_csEditTrackWidth;
	CString m_csEditViaHole;
	CString m_csEditVia;
	CString m_csEditNetName;
	BOOL m_bCheckHideConnection;
	BOOL m_bCheckRemoveLoop;
	BOOL m_bCheckStackLayerOnly;
	afx_msg void OnBnClickedCheckStackLayerOnly();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedOk();
};
