// Dlg_Edit_FindText.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Edit_FindText.h"


// CDlg_Edit_FindText 对话框

IMPLEMENT_DYNAMIC(CDlg_Edit_FindText, CDialog)
CDlg_Edit_FindText::CDlg_Edit_FindText(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Edit_FindText::IDD, pParent)
	, m_csEditText(_T(""))
	, m_nComboScope(0)
	, m_nComboObject(0)
	, m_bCheckCase(FALSE)
	, m_bCheckWord(FALSE)
	, m_bCheckJump(FALSE)
{
}

CDlg_Edit_FindText::~CDlg_Edit_FindText()
{
}

void CDlg_Edit_FindText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_csEditText);
	DDX_CBIndex(pDX, IDC_COMBO_SCOPE, m_nComboScope);
	DDX_CBIndex(pDX, IDC_COMBO_OBJECT, m_nComboObject);
	DDX_Check(pDX, IDC_CHECK_CASE, m_bCheckCase);
	DDX_Check(pDX, IDC_CHECK_WORD, m_bCheckWord);
	DDX_Check(pDX, IDC_CHECK_JUMP, m_bCheckJump);
}


BEGIN_MESSAGE_MAP(CDlg_Edit_FindText, CDialog)
END_MESSAGE_MAP()


// CDlg_Edit_FindText 消息处理程序
