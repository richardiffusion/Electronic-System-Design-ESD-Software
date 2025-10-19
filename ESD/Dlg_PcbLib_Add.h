#pragma once


// CDlg_PcbLib_Add 对话框

class CDlg_PcbLib_Add : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbLib_Add)

public:
	CDlg_PcbLib_Add(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PcbLib_Add();

// 对话框数据
	enum { IDD = IDD_PCBLIB_ADD };
	int m_nRadioLib;
	CString m_csLibName;

	CListBox m_listboxUserLibrary;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
