#pragma once


// CDlg_InputText �Ի���

class CDlg_InputText : public CDialog
{
	DECLARE_DYNAMIC(CDlg_InputText)

public:
	CDlg_InputText(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_InputText();

// �Ի�������
	enum { IDD = IDD_INPUT_TEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditText;
};
