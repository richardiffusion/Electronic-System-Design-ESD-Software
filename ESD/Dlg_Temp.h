#pragma once
#include "afxcmn.h"


// CDlg_Temp �Ի���

class CDlg_Temp : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Temp)

public:
	CDlg_Temp(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Temp();

// �Ի�������
	enum { IDD = IDD_DIALOG_TEMP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_tree;
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
};
