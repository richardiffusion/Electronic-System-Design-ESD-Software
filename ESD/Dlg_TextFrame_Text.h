#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlg_TextFrame_Text �Ի���

class CDlg_TextFrame_Text : public CDialog
{
	DECLARE_DYNAMIC(CDlg_TextFrame_Text)

public:
	CDlg_TextFrame_Text(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlg_TextFrame_Text();

// �Ի�������
	enum { IDD = IDD_TEXTFRAME_TEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:

	CString m_csEditTextFrameText;
};
