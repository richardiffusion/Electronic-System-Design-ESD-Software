#pragma once


// CDlg_PCBEditSplitPlane �Ի���

class CDlg_PCBEditSplitPlane : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditSplitPlane)

public:
	CDlg_PCBEditSplitPlane(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PCBEditSplitPlane();

// �Ի�������
	enum { IDD = IDD_PCB_EDIT_SPLITPLANE };
	CStringArray arrayStringNet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_csComboNet;
};
