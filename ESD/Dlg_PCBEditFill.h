#pragma once

#include  "Bitmap_Transparent_Dialog.h"

// CDlg_PCBEditFill �Ի���

class CDlg_PCBEditFill : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditFill)

public:
	CDlg_PCBEditFill(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PCBEditFill();

	CBitmap_Transparent_Dialog	m_Bitmap;

	CStringArray arrayString;
	CStringArray arrayStringNet;

// �Ի�������
	enum { IDD = IDD_PCB_EDIT_FILL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_csEditY2;
	CString m_csEditX2;
	CString m_csEditY1;
	CString m_csEditX1;
	BOOL m_bCheckLocked;
	BOOL m_bCheckKeepout;
	int m_nComboLayer;
	CString m_csComboNet;
	double m_fEditRotation;
};
