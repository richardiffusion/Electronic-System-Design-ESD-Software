// Dlg_Global_Bus.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Global_Bus.h"


// CDlg_Global_Bus 对话框

IMPLEMENT_DYNAMIC(CDlg_Global_Bus, CDialog)

CDlg_Global_Bus::CDlg_Global_Bus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Global_Bus::IDD, pParent)
	, m_bCheckWidth(TRUE)
	, m_bCheckColor(TRUE)
{
	m_nRadioScope = 0;
}

CDlg_Global_Bus::~CDlg_Global_Bus()
{
}

void CDlg_Global_Bus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_WIDTH, m_bCheckWidth);
	DDX_Check(pDX, IDC_CHECK_COLOR, m_bCheckColor);
	DDX_Radio(pDX, IDC_RADIO_SCOPE, m_nRadioScope);
}


BEGIN_MESSAGE_MAP(CDlg_Global_Bus, CDialog)
END_MESSAGE_MAP()


// CDlg_Global_Bus 消息处理程序
