#pragma once


// CDlg_MyHelp �Ի���

class CDlg_MyHelp : public CDialog
{
	DECLARE_DYNAMIC(CDlg_MyHelp)

public:
	CDlg_MyHelp(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_MyHelp();

// �Ի�������
	enum { IDD = IDD_MY_HELP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
