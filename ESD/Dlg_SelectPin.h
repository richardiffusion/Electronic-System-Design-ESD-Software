#pragma once

#include "struct.h"

// CDlg_SelectPin �Ի���

class CDlg_SelectPin : public CDialog
{
	DECLARE_DYNAMIC(CDlg_SelectPin)

public:
	CDlg_SelectPin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_SelectPin();

// �Ի�������
	enum { IDD = IDD_SELECT_PIN };
	CXTPReportControl m_wndReportCtrl;

	int m_nPartCount;
	int m_nPart;
	CTypedPtrArray <CObArray, COb_schpin*> m_arrayPins;
	CTypedPtrArray <CObArray, CXTPReportRecord*> m_arrayRecords;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonEditPin();
	afx_msg void OnBnClickedOk();
};
