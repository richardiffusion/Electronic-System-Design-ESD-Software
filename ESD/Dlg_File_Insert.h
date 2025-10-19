#pragma once


// CDlg_File_Insert 对话框

class CDlg_File_Insert : public CDialog
{
	DECLARE_DYNAMIC(CDlg_File_Insert)

public:
	CDlg_File_Insert(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_File_Insert();

	int m_nOpenMode;

// 对话框数据
	enum { IDD = IDD_MESSAGE_OPENFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
