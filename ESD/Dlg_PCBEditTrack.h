#pragma once

#include  "Bitmap_Transparent_Dialog.h"

// CDlg_PCBEditTrack �Ի���

class CDlg_PCBEditTrack : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PCBEditTrack)

public:
	CDlg_PCBEditTrack(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PCBEditTrack();

// �Ի�������
	enum { IDD = IDD_PCB_EDIT_TRACK };

	CBitmap_Transparent_Dialog	m_Bitmap;
	CStringArray arrayString;
	CStringArray arrayStringNet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_csEditWidth;
	CString m_csEditX2;
	CString m_csEditY2;
	CString m_csEditY1;
	CString m_csEditX1;
	BOOL m_bCheckLocked;
	BOOL m_bCheckKeepout;
	int m_nComboLayer;
	CString m_csComboNet;
};
