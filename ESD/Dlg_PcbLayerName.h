#pragma once


// CDlg_PCBLayerName �Ի���

class CDlg_PCBLayerName : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBLayerName)

public:
	CDlg_PCBLayerName(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PCBLayerName();

// �Ի�������
	enum { IDD = IDD_PCB_LAYERNAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditLayerName;
};
