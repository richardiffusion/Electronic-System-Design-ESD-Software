#pragma once
#include "afxwin.h"


// CDlg_PcbNetEdit �Ի���

class CDlg_PcbNetEdit : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbNetEdit)

public:
	CDlg_PcbNetEdit(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PcbNetEdit();

// �Ի�������
	enum { IDD = IDD_PCB_NET_EDIT };

	CDocument* pDocument;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listboxNet;
	CListBox m_listboxPin;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeNetList();
	afx_msg void OnBnClickedPinEdit();
	afx_msg void OnBnClickedNetEdit();
	afx_msg void OnBnClickedNetDelete();
	afx_msg void OnBnClickedNetAdd();
};
