#pragma once


// CDlg_Edit_JumpComp �Ի���

class CDlg_Edit_JumpComp : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Edit_JumpComp)

public:
	CDlg_Edit_JumpComp(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Edit_JumpComp();

// �Ի�������
	enum { IDD = IDD_EDIT_JUMP_COMPONENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditText;
	int m_nComboScope;
};
