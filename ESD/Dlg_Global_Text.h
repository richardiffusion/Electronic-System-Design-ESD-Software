#pragma once


// CDlg_Global_Text �Ի���

class CDlg_Global_Text : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Global_Text)

public:
	CDlg_Global_Text(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Global_Text();

// �Ի�������
	enum { IDD = IDD_GLOBAL_TEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bCheckFont;
	BOOL m_bCheckColor;
	BOOL m_bCheckOrientation;
	int m_nRadioScope;
};
