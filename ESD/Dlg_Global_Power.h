#pragma once


// CDlg_Global_Power �Ի���

class CDlg_Global_Power : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Global_Power)

public:
	CDlg_Global_Power(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Global_Power();

// �Ի�������
	enum { IDD = IDD_GLOBAL_POWER };
	int bIsCrossSheetConnector;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckStyle;
	BOOL m_bCheckColor;
	BOOL m_bCheckOrientation;
	BOOL m_bCheckVisible;
	int m_nRadioScope;
	virtual BOOL OnInitDialog();
};
