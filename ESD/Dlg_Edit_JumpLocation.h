#pragma once


// CDlg_Edit_JumpLocation �Ի���

class CDlg_Edit_JumpLocation : public CDialog
{
	DECLARE_DYNAMIC(CDlg_Edit_JumpLocation)

public:
	CDlg_Edit_JumpLocation(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_Edit_JumpLocation();

// �Ի�������
	enum { IDD = IDD_EDIT_JUMP_LOCATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	CString m_csEditX;
	CString m_csEditY;
};
