#pragma once


// CDlg_PCBPowerPlane �Ի���

class CDlg_PCBPowerPlane : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBPowerPlane)

public:
	CDlg_PCBPowerPlane(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PCBPowerPlane();

// �Ի�������
	enum { IDD = IDD_PCB_POWERPLANE };
	CStringArray arrayStringNet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditLayerName;
	virtual BOOL OnInitDialog();
	CString m_csEditPullBack;
	CString m_csComboNet;
};
