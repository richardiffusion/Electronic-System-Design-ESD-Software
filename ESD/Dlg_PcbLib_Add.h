#pragma once


// CDlg_PcbLib_Add �Ի���

class CDlg_PcbLib_Add : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbLib_Add)

public:
	CDlg_PcbLib_Add(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PcbLib_Add();

// �Ի�������
	enum { IDD = IDD_PCBLIB_ADD };
	int m_nRadioLib;
	CString m_csLibName;

	CListBox m_listboxUserLibrary;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_csEditName;
	CString m_csEditDescription;
	afx_msg void OnBnClickedRadioNewlib();
	afx_msg void OnBnClickedRadioOldlib();
protected:
	virtual void OnOK();
};
