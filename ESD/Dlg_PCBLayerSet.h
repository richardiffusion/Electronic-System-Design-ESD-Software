#pragma once


// CDlg_PCBLayerSet 对话框

class CDlg_PCBLayerSet : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBLayerSet)

public:
	CDlg_PCBLayerSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBLayerSet();

// 对话框数据
	enum { IDD = IDD_PCB_LAYER_SET };

	CXTPReportControl m_wndLayerName;
	CXTPReportControl m_wndLayerSet;

	int m_nSel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CDocument* pDocument;
	virtual BOOL OnInitDialog();
	void LayerNameSelChanged(int sel);
	void SaveLayerSetParam(int old_sel);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLayerNameClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLayerNameDblClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnBnClickedButtonWrite();
};
