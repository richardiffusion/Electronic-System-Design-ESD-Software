#pragma once


// CDlg_PcbTearDrop �Ի���

class CDlg_PcbTearDrop : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbTearDrop)

public:
	CDlg_PcbTearDrop(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PcbTearDrop();

// �Ի�������
	enum { IDD = IDD_PCB_TEARDROP };

	int m_nRadioAction;
	int m_nRadioStyle;
	int m_nRadioSize;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckAllPad;
	BOOL m_bCheckAllVia;
	BOOL m_bCheckSelection;
	afx_msg void OnBnClickedRadioAction();
	afx_msg void OnBnClickedRadioAction2();
};
