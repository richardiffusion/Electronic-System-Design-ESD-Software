#pragma once


// CDlg_Report �Ի���

class CDlg_Report : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Report)

public:
	CDlg_Report(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Report();

// �Ի�������
	enum { IDD = IDD_REPORT };
	CListBox	m_ReportList;

	CStringArray m_arrayReport;

	int m_nNum;
	CString m_TitleText;
	CString m_Selected_Text;
	int m_nSelected;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
