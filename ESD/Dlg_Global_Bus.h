#pragma once


// CDlg_Global_Bus �Ի���

class CDlg_Global_Bus : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Global_Bus)

public:
	CDlg_Global_Bus(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Global_Bus();

// �Ի�������
	enum { IDD = IDD_GLOBAL_BUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckWidth;
	BOOL m_bCheckColor;
	int m_nRadioScope;
};
