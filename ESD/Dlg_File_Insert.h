#pragma once


// CDlg_File_Insert �Ի���

class CDlg_File_Insert : public CDialog
{
	DECLARE_DYNAMIC(CDlg_File_Insert)

public:
	CDlg_File_Insert(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_File_Insert();

	int m_nOpenMode;

// �Ի�������
	enum { IDD = IDD_MESSAGE_OPENFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
