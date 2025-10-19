// Dlg_Global_Port.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Global_Port.h"


// CDlg_Global_Port 对话框

IMPLEMENT_DYNAMIC(CDlg_Global_Port, CDialog)

CDlg_Global_Port::CDlg_Global_Port(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Global_Port::IDD, pParent)
	, m_bCheckTextColor(TRUE)
	, m_bCheckBorderColor(TRUE)
	, m_bCheckFillColor(TRUE)
	, m_bCheckStyle(TRUE)
	, m_bCheckAlignment(TRUE)
	, m_bCheckIO(TRUE)
{
	m_nRadioScope = 0;
}

CDlg_Global_Port::~CDlg_Global_Port()
{
}

void CDlg_Global_Port::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_TEXTCOLOR, m_bCheckTextColor);
	DDX_Check(pDX, IDC_CHECK_BORDERCOLOR, m_bCheckBorderColor);
	DDX_Check(pDX, IDC_CHECK_FILLCOLOR, m_bCheckFillColor);
	DDX_Check(pDX, IDC_CHECK_STYLE, m_bCheckStyle);
	DDX_Check(pDX, IDC_CHECK_ALIGNMENT, m_bCheckAlignment);
	DDX_Check(pDX, IDC_CHECK_IO, m_bCheckIO);
	DDX_Radio(pDX, IDC_RADIO_SCOPE, m_nRadioScope);
}


BEGIN_MESSAGE_MAP(CDlg_Global_Port, CDialog)
END_MESSAGE_MAP()


// CDlg_Global_Port 消息处理程序
