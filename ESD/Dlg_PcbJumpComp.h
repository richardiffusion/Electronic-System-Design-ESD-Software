#pragma once


// CDlg_PcbJumpComp �Ի���

class CDlg_PcbJumpComp : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbJumpComp)

public:
	CDlg_PcbJumpComp(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PcbJumpComp();

// �Ի�������
	enum { IDD = IDD_PCB_JUMP_COMP };

	CString csTitle;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csCompDesignator;
	virtual BOOL OnInitDialog();
};
