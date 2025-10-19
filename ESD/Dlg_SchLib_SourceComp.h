#pragma once
#include "afxwin.h"
#include "struct.h"


// CDlg_SchLib_SourceComp 对话框

class CDlg_SchLib_SourceComp : public CDialog
{
	DECLARE_DYNAMIC(CDlg_SchLib_SourceComp)

public:
	CDlg_SchLib_SourceComp(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_SchLib_SourceComp();

// 对话框数据
	enum { IDD = IDD_SCHLIB_SOURCE_COMP };

	Struct_CompList m_complist;
	CArray <Struct_CompList, Struct_CompList&>  m_arrayLibraryCompList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listboxUserLibrary;
	CListBox m_listboxComp;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListUserlibrary();
protected:
	virtual void OnOK();
};
