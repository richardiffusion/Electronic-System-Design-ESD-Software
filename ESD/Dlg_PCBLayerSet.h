#pragma once


// CDlg_PCBLayerSet �Ի���

class CDlg_PCBLayerSet : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBLayerSet)

public:
	CDlg_PCBLayerSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PCBLayerSet();

// �Ի�������
	enum { IDD = IDD_PCB_LAYER_SET };

	CXTPReportControl m_wndLayerName;
	CXTPReportControl m_wndLayerSet;

	int m_nSel;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
