#pragma once

#include  "Bitmap_Transparent_Dialog.h"

// CDlg_PCBEditVia �Ի���

class CDlg_PCBEditVia : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditVia)

public:
	CDlg_PCBEditVia(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PCBEditVia();

// �Ի�������
	enum { IDD = IDD_PCB_EDIT_VIA };
	
	CBitmap_Transparent_Dialog	m_Bitmap;
	CStringArray arrayLayer;
	CStringArray arrayStringNet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_csEditY;
	CString m_csEditX;
	CString m_csEditDia;
	CString m_csEditHoleDia;
	int m_nStartLayer;
	int m_nEndLayer;
	BOOL m_bCheckLocked;
	BOOL m_bCheckTentingTop;
	BOOL m_bCheckTentingBottom;
	CString m_csEditExpansion;
	int m_nRadioExpansion;
	afx_msg void OnBnClickedRadioExpansion();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnCbnSelchangeComboStartlayer();
	afx_msg void OnCbnSelchangeComboEndlayer();
	CString m_csComboNet;
};
