#pragma once


// CDlg_PCB_LoadPackageByName �Ի���

class CDlg_PCB_LoadPackageByName : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCB_LoadPackageByName)

public:
	CDlg_PCB_LoadPackageByName(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PCB_LoadPackageByName();

// �Ի�������
	enum { IDD = IDD_PCB_LOAD_PACKAGE_BYNAME };

	CStringArray arrayCompName;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csComboName;
	virtual BOOL OnInitDialog();
};
