#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlg_TextFrame_Text 对话框

class CDlg_TextFrame_Text : public CDialog
{
	DECLARE_DYNAMIC(CDlg_TextFrame_Text)

public:
	CDlg_TextFrame_Text(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlg_TextFrame_Text();

// 对话框数据
	enum { IDD = IDD_TEXTFRAME_TEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	CString m_csEditTextFrameText;
};
