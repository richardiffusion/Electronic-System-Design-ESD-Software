// Dlg_PcbTearDrop.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PcbTearDrop.h"
#include ".\dlg_pcbteardrop.h"


// CDlg_PcbTearDrop 对话框

IMPLEMENT_DYNAMIC(CDlg_PcbTearDrop, CDialog)
CDlg_PcbTearDrop::CDlg_PcbTearDrop(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PcbTearDrop::IDD, pParent)
	, m_bCheckAllPad(TRUE)
	, m_bCheckAllVia(TRUE)
	, m_bCheckSelection(FALSE)
{
	m_nRadioAction = 0;
	m_nRadioStyle = 1;
	m_nRadioSize = 0;
}

CDlg_PcbTearDrop::~CDlg_PcbTearDrop()
{
}

void CDlg_PcbTearDrop::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_ALLPAD, m_bCheckAllPad);
	DDX_Check(pDX, IDC_CHECK_ALLVIA, m_bCheckAllVia);
	DDX_Check(pDX, IDC_CHECK_SELECTION, m_bCheckSelection);
	DDX_Radio(pDX, IDC_RADIO_ACTION, m_nRadioAction);
	DDX_Radio(pDX, IDC_RADIO_STYLE, m_nRadioStyle);
	DDX_Radio(pDX, IDC_RADIO_SIZE, m_nRadioSize);
}


BEGIN_MESSAGE_MAP(CDlg_PcbTearDrop, CDialog)
	ON_BN_CLICKED(IDC_RADIO_ACTION, OnBnClickedRadioAction)
	ON_BN_CLICKED(IDC_RADIO_ACTION2, OnBnClickedRadioAction2)
END_MESSAGE_MAP()


// CDlg_PcbTearDrop 消息处理程序

void CDlg_PcbTearDrop::OnBnClickedRadioAction()
{
	//CWnd* pWnd = GetDlgItem(IDC_RADIO_STYLE);
	//pWnd->EnableWindow(TRUE);
	CWnd* pWnd = GetDlgItem(IDC_RADIO_STYLE2);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_SIZE);
	pWnd->EnableWindow(TRUE);
	pWnd = GetDlgItem(IDC_RADIO_SIZE2);
	pWnd->EnableWindow(TRUE);
}

void CDlg_PcbTearDrop::OnBnClickedRadioAction2()
{
	CWnd* pWnd = GetDlgItem(IDC_RADIO_STYLE2);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_SIZE);
	pWnd->EnableWindow(FALSE);
	pWnd = GetDlgItem(IDC_RADIO_SIZE2);
	pWnd->EnableWindow(FALSE);
}
