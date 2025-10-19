#pragma once
#include "afxwin.h"


// CDlg_SchLib_List 对话框

class CDlg_SchLib_List : public CDialog
{
	DECLARE_DYNAMIC(CDlg_SchLib_List)

public:
	CDlg_SchLib_List(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_SchLib_List();

// 对话框数据
	enum { IDD = IDD_SCHLIB_LIST };
	CString m_csLibraryName;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_listboxList;
protected:
	virtual void OnOK();
};
