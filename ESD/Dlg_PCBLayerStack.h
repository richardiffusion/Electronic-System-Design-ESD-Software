#pragma once
#include "afxcmn.h"


// CDlg_PCBLayerStack �Ի���

class CDlg_PCBLayerStack : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBLayerStack)

public:
	CDlg_PCBLayerStack(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PCBLayerStack();

// �Ի�������
	enum { IDD = IDD_PCB_LAYER_STACK };
	CXTPReportControl m_wndReportCtrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
