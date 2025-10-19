#pragma once
#include "afxcmn.h"
#include "MarkupStatic.h"

// CDlg_HighlightList 对话框

class CDlg_HighlightList : public CDialog
{
	DECLARE_DYNAMIC(CDlg_HighlightList)

public:
	CDlg_HighlightList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_HighlightList();

// 对话框数据
	enum { IDD = IDD_HIGHLIGHT_LIST };
	char* pListBuffer;
	CMarkupStatic	m_wndStatic;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();

};
