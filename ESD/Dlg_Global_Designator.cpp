// Dlg_Global_Designator.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Global_Designator.h"


// CDlg_Global_Designator 对话框

IMPLEMENT_DYNAMIC(CDlg_Global_Designator, CDialog)

CDlg_Global_Designator::CDlg_Global_Designator(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Global_Designator::IDD, pParent)
	, m_bCheckFont(TRUE)
	, m_bCheckColor(TRUE)
	, m_bCheckOrientation(TRUE)
	, m_bCheckVisible(TRUE)
	, m_bCheckLock(TRUE)
	, m_bCheckAuto(TRUE)
{
	m_nRadioScope = 0;
}

CDlg_Global_Designator::~CDlg_Global_Designator()
{
}

void CDlg_Global_Designator::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_FONT, m_bCheckFont);
	DDX_Check(pDX, IDC_CHECK_COLOR, m_bCheckColor);
	DDX_Check(pDX, IDC_CHECK_ORIENTATION, m_bCheckOrientation);
	DDX_Check(pDX, IDC_CHECK_VISIBLE, m_bCheckVisible);
	DDX_Check(pDX, IDC_CHECK_LOCK, m_bCheckLock);
	DDX_Check(pDX, IDC_CHECK_AUTO, m_bCheckAuto);
	DDX_Radio(pDX, IDC_RADIO_SCOPE, m_nRadioScope);
}


BEGIN_MESSAGE_MAP(CDlg_Global_Designator, CDialog)
END_MESSAGE_MAP()


// CDlg_Global_Designator 消息处理程序
