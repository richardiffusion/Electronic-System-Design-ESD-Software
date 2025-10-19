// Dlg_Global_Fill.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Global_Fill.h"


// CDlg_Global_Fill 对话框

IMPLEMENT_DYNAMIC(CDlg_Global_Fill, CDialog)

CDlg_Global_Fill::CDlg_Global_Fill(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Global_Fill::IDD, pParent)
	, m_bCheckWidth(TRUE)
	, m_bCheckColor(TRUE)
	, m_bCheckFillColor(TRUE)
	, m_bCheckTransparent(TRUE)
	, m_bCheckSolid(TRUE)
{
	m_nRadioScope = 0;
}

CDlg_Global_Fill::~CDlg_Global_Fill()
{
}

void CDlg_Global_Fill::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_WIDTH, m_bCheckWidth);
	DDX_Check(pDX, IDC_CHECK_COLOR, m_bCheckColor);
	DDX_Check(pDX, IDC_CHECK_FILLCOLOR, m_bCheckFillColor);
	DDX_Check(pDX, IDC_CHECK_TRANSPARENT, m_bCheckTransparent);
	DDX_Check(pDX, IDC_CHECK_SOLID, m_bCheckSolid);
	DDX_Radio(pDX, IDC_RADIO_SCOPE, m_nRadioScope);
}


BEGIN_MESSAGE_MAP(CDlg_Global_Fill, CDialog)
END_MESSAGE_MAP()


// CDlg_Global_Fill 消息处理程序
