#pragma once


// CDlg_SchLib_Attrib �Ի���

class CDlg_SchLib_Attrib : public CDialog
{
	DECLARE_DYNAMIC(CDlg_SchLib_Attrib)

public:
	CDlg_SchLib_Attrib(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_SchLib_Attrib();

// �Ի�������
	enum { IDD = IDD_SCHLIB_ATTRIB };
	BOOL m_bSystemLib;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditName;
	CString m_csEditDescription;
	CString m_csEditRef;
	CString m_csEditPackage;
	virtual BOOL OnInitDialog();
};
