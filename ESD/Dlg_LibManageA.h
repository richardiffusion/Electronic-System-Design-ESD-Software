#pragma once


// CDlg_LibManageA �Ի���

class CDlg_LibManageA : public CDialog
{
	DECLARE_DYNAMIC(CDlg_LibManageA)

public:
	CDlg_LibManageA(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_LibManageA();

// �Ի�������
	enum { IDD = IDD_DLG_LIBMANAGEA };

	int m_nLib;
	int m_nComp;
	CFile m_cfile;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
