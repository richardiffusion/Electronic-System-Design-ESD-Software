#pragma once


// CDlg_Global_Polyline �Ի���

class CDlg_Global_Polyline : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Global_Polyline)

public:
	CDlg_Global_Polyline(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Global_Polyline();

// �Ի�������
	enum { IDD = IDD_GLOBAL_POLYLINE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckStyle;
	BOOL m_bCheckColor;
	BOOL m_bCheckWidth;
	BOOL m_bCheckStart;
	BOOL m_bCheckEnd;
	BOOL m_bCheckSize;
	int m_nRadioScope;
};
