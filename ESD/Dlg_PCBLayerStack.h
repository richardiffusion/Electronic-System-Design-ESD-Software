#pragma once
#include "afxcmn.h"


// CDlg_PCBLayerStack 对话框

class CDlg_PCBLayerStack : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBLayerStack)

public:
	CDlg_PCBLayerStack(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBLayerStack();

// 对话框数据
	enum { IDD = IDD_PCB_LAYER_STACK };
	CXTPReportControl m_wndReportCtrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CDocument* pDocument;
	virtual BOOL OnInitDialog();
	void LayerNum();
	int GetCurrentLayer();
	int GetCurrentPlane();
	afx_msg void OnBnClickedButtonAddLayer();
	afx_msg void OnBnClickedButtonAddPlane();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonMovedown();
	afx_msg void OnBnClickedButtonMoveup();
	afx_msg void OnReportItemDblClick(NMHDR *pNMHDR, LRESULT *pResult);
};
