#pragma once


// CDlg_PCBSnapSetup �Ի���

class CDlg_PCBSnapSetup : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBSnapSetup)

public:
	CDlg_PCBSnapSetup(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PCBSnapSetup();

// �Ի�������
	enum { IDD = IDD_PCB_SNAPSETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditSnap;
};
