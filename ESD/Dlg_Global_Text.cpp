// Dlg_Global_Text.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Global_Text.h"


// CDlg_Global_Text 对话框

IMPLEMENT_DYNAMIC(CDlg_Global_Text, CDialog)

CDlg_Global_Text::CDlg_Global_Text(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Global_Text::IDD, pParent)
	, m_bCheckFont(TRUE)
	, m_bCheckColor(TRUE)
	, m_bCheckOrientation(TRUE)
{
	m_nRadioScope = 0;
}

CDlg_Global_Text::~CDlg_Global_Text()
{
}

void CDlg_Global_Text::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_FONT, m_bCheckFont);
	DDX_Check(pDX, IDC_CHECK_COLOR, m_bCheckColor);
	DDX_Check(pDX, IDC_CHECK_ORIENTATION, m_bCheckOrientation);
	DDX_Radio(pDX, IDC_RADIO_SCOPE, m_nRadioScope);
}


BEGIN_MESSAGE_MAP(CDlg_Global_Text, CDialog)
END_MESSAGE_MAP()


// CDlg_Global_Text 消息处理程序
