// Dlg_Edit_ReplaceText.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Edit_ReplaceText.h"


// CDlg_Edit_ReplaceText 对话框

IMPLEMENT_DYNAMIC(CDlg_Edit_ReplaceText, CDialog)
CDlg_Edit_ReplaceText::CDlg_Edit_ReplaceText(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Edit_ReplaceText::IDD, pParent)
	, m_csEditText(_T(""))
	, m_csEditReplaceText(_T(""))
	, m_nComboScope(0)
	, m_nComboObject(0)
	, m_bCheckCase(TRUE)
	, m_bCheckPrompt(FALSE)
	, m_bCheckWord(FALSE)
{
}

CDlg_Edit_ReplaceText::~CDlg_Edit_ReplaceText()
{
}

void CDlg_Edit_ReplaceText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_csEditText);
	DDX_Text(pDX, IDC_EDIT_REPLACETEXT, m_csEditReplaceText);
	DDX_CBIndex(pDX, IDC_COMBO_SCOPE, m_nComboScope);
	DDX_CBIndex(pDX, IDC_COMBO_OBJECT, m_nComboObject);
	DDX_Check(pDX, IDC_CHECK_CASE, m_bCheckCase);
	DDX_Check(pDX, IDC_CHECK_PROMPT, m_bCheckPrompt);
	DDX_Check(pDX, IDC_CHECK_WORD, m_bCheckWord);
}


BEGIN_MESSAGE_MAP(CDlg_Edit_ReplaceText, CDialog)
END_MESSAGE_MAP()


// CDlg_Edit_ReplaceText 消息处理程序
