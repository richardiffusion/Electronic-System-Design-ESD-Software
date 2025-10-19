#pragma once


// CDlg_PCBPowerPlane 对话框

class CDlg_PCBPowerPlane : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBPowerPlane)

public:
	CDlg_PCBPowerPlane(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCBPowerPlane();

// 对话框数据
	enum { IDD = IDD_PCB_POWERPLANE };
	CStringArray arrayStringNet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditLayerName;
	virtual BOOL OnInitDialog();
	CString m_csEditPullBack;
	CString m_csComboNet;
};
