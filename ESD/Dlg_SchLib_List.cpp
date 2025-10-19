// Dlg_SchLib_List.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_SchLib_List.h"
#include ".\dlg_schlib_list.h"
#include "systemext.h"


// CDlg_SchLib_List 对话框

IMPLEMENT_DYNAMIC(CDlg_SchLib_List, CDialog)
CDlg_SchLib_List::CDlg_SchLib_List(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_SchLib_List::IDD, pParent)
{
}

CDlg_SchLib_List::~CDlg_SchLib_List()
{
}

void CDlg_SchLib_List::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_listboxList);
}


BEGIN_MESSAGE_MAP(CDlg_SchLib_List, CDialog)
END_MESSAGE_MAP()


// CDlg_SchLib_List 消息处理程序

BOOL CDlg_SchLib_List::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(nEDA_System_State == SCH)
	{
		int libnum = arrayEDA_SchUserLibraryName.GetCount();
		for(int i=0; i<libnum; i++)
		{
			m_listboxList.AddString(arrayEDA_SchUserLibraryName.GetAt(i));
		}

		if(arrayEDA_SchUserLibraryName.GetCount() > 0)
		{
			m_listboxList.SetCurSel(0);
		}
	}
	else
	{
		int libnum = arrayEDA_PcbUserLibraryName.GetCount();
		for(int i=0; i<libnum; i++)
		{
			m_listboxList.AddString(arrayEDA_PcbUserLibraryName.GetAt(i));
		}

		if(arrayEDA_PcbUserLibraryName.GetCount() > 0)
		{
			m_listboxList.SetCurSel(0);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_SchLib_List::OnOK()
{
	int i = m_listboxList.GetCurSel();
	if(i >= 0)
	{
		m_listboxList.GetText(i, m_csLibraryName);
	}

	CDialog::OnOK();
}
