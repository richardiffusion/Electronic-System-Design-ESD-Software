#pragma once


// CDlg_Global_Port �Ի���

class CDlg_Global_Port : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Global_Port)

public:
	CDlg_Global_Port(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Global_Port();

// �Ի�������
	enum { IDD = IDD_GLOBAL_PORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckTextColor;
	BOOL m_bCheckBorderColor;
	BOOL m_bCheckFillColor;
	BOOL m_bCheckStyle;
	BOOL m_bCheckAlignment;
	BOOL m_bCheckIO;
	int m_nRadioScope;
};
