#pragma once


// CDlg_PcbHighlightPins 对话框

class CDlg_PcbHighlightPins : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbHighlightPins)

public:
	CDlg_PcbHighlightPins(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PcbHighlightPins();

// 对话框数据
	enum { IDD = IDD_PCB_HIGHLIGH_PINS };
		
	CXTPReportControl m_wndPins;
	CString csNet;
	CStringArray compID;
	CStringArray pinNum;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
