#pragma once


// CDlg_PCB_LoadPackageXY �Ի���

class CDlg_PCB_LoadPackageXY : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCB_LoadPackageXY)

public:
	CDlg_PCB_LoadPackageXY(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PCB_LoadPackageXY();

// �Ի�������
	enum { IDD = IDD_PCB_LOAD_PACKAGE_XY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEdit_X;
	CString m_csEdit_Y;
};
