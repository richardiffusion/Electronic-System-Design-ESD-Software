// Dlg_PcbLib_Add.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PcbLib_Add.h"
#include ".\dlg_pcblib_add.h"
#include "systemext.h"

// CDlg_PcbLib_Add 对话框

IMPLEMENT_DYNAMIC(CDlg_PcbLib_Add, CDialog)
CDlg_PcbLib_Add::CDlg_PcbLib_Add(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PcbLib_Add::IDD, pParent)
	, m_nRadioLib(0)
	, m_csEditName(_T(""))
	, m_csEditDescription(_T(""))
{
}

CDlg_PcbLib_Add::~CDlg_PcbLib_Add()
{
}

void CDlg_PcbLib_Add::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_NEWLIB, m_nRadioLib);
	DDX_Control(pDX, IDC_LIST_LIB, m_listboxUserLibrary);
	DDX_Text(pDX, IDC_EDIT_NAME, m_csEditName);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_csEditDescription);
}


BEGIN_MESSAGE_MAP(CDlg_PcbLib_Add, CDialog)
	ON_BN_CLICKED(IDC_RADIO_NEWLIB, OnBnClickedRadioNewlib)
	ON_BN_CLICKED(IDC_RADIO_OLDLIB, OnBnClickedRadioOldlib)
END_MESSAGE_MAP()


// CDlg_PcbLib_Add 消息处理程序

BOOL CDlg_PcbLib_Add::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd;
	
	pWnd = GetDlgItem(IDC_LIST_LIB);
	pWnd->EnableWindow(FALSE);
	
	//初始化用户元件库列表
	int libnum = arrayEDA_PcbUserLibraryName.GetCount();
	for(int i=0; i<libnum; i++)
	{
		m_listboxUserLibrary.AddString(arrayEDA_PcbUserLibraryName.GetAt(i));
	}

	if(arrayEDA_PcbUserLibraryName.GetCount() > 0)
	{
		m_listboxUserLibrary.SetCurSel(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_PcbLib_Add::OnBnClickedRadioNewlib()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_LIST_LIB);
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_EDIT_LIBNAME);
	pWnd->EnableWindow(TRUE);

}

void CDlg_PcbLib_Add::OnBnClickedRadioOldlib()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_LIST_LIB);
	pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_EDIT_LIBNAME);
	pWnd->EnableWindow(FALSE);

}

void CDlg_PcbLib_Add::OnOK()
{
	UpdateData(TRUE);
	if(m_nRadioLib == 0)
	{
		CWnd* pWnd = GetDlgItem(IDC_EDIT_LIBNAME);
		pWnd->GetWindowText(m_csLibName);
	}
	else
	{
		if(m_listboxUserLibrary.GetCount() > 0)
		{
			int index = m_listboxUserLibrary.GetCurSel();
			m_listboxUserLibrary.GetText(index, m_csLibName);
		}
	}

	if(m_csLibName.IsEmpty() == TRUE)
	{
		AfxMessageBox("库名称未定义.");
		return;
	}

	CDialog::OnOK();
}
