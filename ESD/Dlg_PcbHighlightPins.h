#pragma once


// CDlg_PcbHighlightPins �Ի���

class CDlg_PcbHighlightPins : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbHighlightPins)

public:
	CDlg_PcbHighlightPins(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_PcbHighlightPins();

// �Ի�������
	enum { IDD = IDD_PCB_HIGHLIGH_PINS };
		
	CXTPReportControl m_wndPins;
	CString csNet;
	CStringArray compID;
	CStringArray pinNum;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
