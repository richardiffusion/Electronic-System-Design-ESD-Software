#pragma once


// CDlg_PcbNetName �Ի���

class CDlg_PcbNetName : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbNetName)

public:
	CDlg_PcbNetName(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PcbNetName();

// �Ի�������
	enum { IDD = IDD_PCB_NETNAME };
	
	CStringArray arrayString;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csComboNet;
	virtual BOOL OnInitDialog();
};
