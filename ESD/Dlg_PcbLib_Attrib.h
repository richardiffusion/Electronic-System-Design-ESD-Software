#pragma once


// CDlg_PcbLib_Attrib �Ի���

class CDlg_PcbLib_Attrib : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbLib_Attrib)

public:
	CDlg_PcbLib_Attrib(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PcbLib_Attrib();

// �Ի�������
	enum { IDD = IDD_PCBLIB_ATTRIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditName;
	CString m_csEditDescription;
};
