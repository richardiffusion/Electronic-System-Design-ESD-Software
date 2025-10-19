#pragma once


// CDlg_PCBLayerName 对话框

class CDlg_PCBLayerName : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBLayerName)

public:
	CDlg_PCBLayerName(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBLayerName();

// 对话框数据
	enum { IDD = IDD_PCB_LAYERNAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditLayerName;
};
