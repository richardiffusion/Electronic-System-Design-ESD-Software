#pragma once


// CDlg_EditIeee �Ի���

class CDlg_EditIeee : public CDialog
{
	DECLARE_DYNAMIC(CDlg_EditIeee)

public:
	CDlg_EditIeee(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_EditIeee();

// �Ի�������
	enum { IDD = IDD_EDIT_IEEE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
