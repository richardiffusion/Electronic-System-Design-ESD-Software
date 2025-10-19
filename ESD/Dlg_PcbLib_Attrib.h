#pragma once


// CDlg_PcbLib_Attrib 对话框

class CDlg_PcbLib_Attrib : public CDialog
{
	DECLARE_DYNAMIC(CDlg_PcbLib_Attrib)

public:
	CDlg_PcbLib_Attrib(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_PcbLib_Attrib();

// 对话框数据
	enum { IDD = IDD_PCBLIB_ATTRIB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_csEditName;
	CString m_csEditDescription;
};
