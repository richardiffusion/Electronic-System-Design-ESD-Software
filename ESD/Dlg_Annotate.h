#pragma once
#include "afxcmn.h"


// CDlg_Annotate �Ի���

class CDlg_Annotate : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Annotate)

public:
	CDlg_Annotate(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Annotate();

// �Ի�������
	enum { IDD = IDD_ANNOTATE };
	int m_nRadioScope;
	CStringArray m_arrayStringA;
	CStringArray m_arrayStringE;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nComboOrder;
	afx_msg void OnCbnSelchangeComboOrder();
	int m_nEditStartNum;
	CListCtrl m_listA;
	CListCtrl m_listE;
};
