// Dlg_TextFrame_Text.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_TextFrame_Text.h"


// CDlg_TextFrame_Text �Ի���

IMPLEMENT_DYNAMIC(CDlg_TextFrame_Text, CDialog)
CDlg_TextFrame_Text::CDlg_TextFrame_Text(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_TextFrame_Text::IDD, pParent)
	, m_csEditTextFrameText(_T(""))
{
}

CDlg_TextFrame_Text::~CDlg_TextFrame_Text()
{
}

void CDlg_TextFrame_Text::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_TEXTFRAME_TEXT, m_csEditTextFrameText);
}


BEGIN_MESSAGE_MAP(CDlg_TextFrame_Text, CDialog)
END_MESSAGE_MAP()


// CDlg_TextFrame_Text ��Ϣ�������
