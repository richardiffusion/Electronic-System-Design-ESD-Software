#pragma once


// CDlg_PCB_LoadPackageXY 对话框

class CDlg_PCB_LoadPackageXY : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCB_LoadPackageXY)

public:
	CDlg_PCB_LoadPackageXY(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PCB_LoadPackageXY();

// 对话框数据
	enum { IDD = IDD_PCB_LOAD_PACKAGE_XY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEdit_X;
	CString m_csEdit_Y;
};
