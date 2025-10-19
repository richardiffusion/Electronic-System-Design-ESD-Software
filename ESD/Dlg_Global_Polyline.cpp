// Dlg_Global_Polyline.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_Global_Polyline.h"


// CDlg_Global_Polyline 对话框

IMPLEMENT_DYNAMIC(CDlg_Global_Polyline, CDialog)

CDlg_Global_Polyline::CDlg_Global_Polyline(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Global_Polyline::IDD, pParent)
	, m_bCheckStyle(TRUE)
	, m_bCheckColor(TRUE)
	, m_bCheckWidth(TRUE)
	, m_bCheckStart(TRUE)
	, m_bCheckEnd(TRUE)
	, m_bCheckSize(TRUE)
{
	m_nRadioScope = 0;
}

CDlg_Global_Polyline::~CDlg_Global_Polyline()
{
}

void CDlg_Global_Polyline::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_STYLE, m_bCheckStyle);
	DDX_Check(pDX, IDC_CHECK_COLOR, m_bCheckColor);
	DDX_Check(pDX, IDC_CHECK_WIDTH, m_bCheckWidth);
	DDX_Check(pDX, IDC_CHECK_START, m_bCheckStart);
	DDX_Check(pDX, IDC_CHECK_END, m_bCheckEnd);
	DDX_Check(pDX, IDC_CHECK_SIZE, m_bCheckSize);
	DDX_Radio(pDX, IDC_RADIO_SCOPE, m_nRadioScope);
}


BEGIN_MESSAGE_MAP(CDlg_Global_Polyline, CDialog)
END_MESSAGE_MAP()


// CDlg_Global_Polyline 消息处理程序
