#pragma once


// CDlg_AnnotateReset �Ի���

class CDlg_AnnotateReset : public CDialog
{
	DECLARE_DYNAMIC(CDlg_AnnotateReset)

public:
	CDlg_AnnotateReset(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_AnnotateReset();

// �Ի�������
	enum { IDD = IDD_ANNOTATE_RESET };
	int m_nRadioScope;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
