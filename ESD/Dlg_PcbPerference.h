#pragma once


// CDlg_PcbPerference �Ի���

class CDlg_PcbPerference : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbPerference)

public:
	CDlg_PcbPerference(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PcbPerference();

// �Ի�������
	enum { IDD = IDD_PCB_PERFERENCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int m_nEditNumber;
	int m_nEditNDelta;
	CString m_csEditXDelta;
	CString m_csEditYDelta;
	BOOL m_bCheckRatsnestEnable;
	BOOL m_bCheckOnlineDrc;
	CString m_csEditMaxRadius;
	int m_nEditUndoBuffer;
	BOOL m_bCheckPolygonHighlight;
	CString m_csEditLoadPackageX;
	CString m_csEditLoadPackageY;
	BOOL m_bCheckFileImport;
	BOOL m_bCheckFileExport;
};
