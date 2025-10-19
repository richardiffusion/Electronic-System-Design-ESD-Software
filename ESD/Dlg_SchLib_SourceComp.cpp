// Dlg_SchLib_SourceComp.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_SchLib_SourceComp.h"
#include ".\dlg_schlib_sourcecomp.h"
#include "systemext.h"

// CDlg_SchLib_SourceComp 对话框

IMPLEMENT_DYNAMIC(CDlg_SchLib_SourceComp, CDialog)
CDlg_SchLib_SourceComp::CDlg_SchLib_SourceComp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_SchLib_SourceComp::IDD, pParent)
{
	m_complist.liboffset = -1;
}

CDlg_SchLib_SourceComp::~CDlg_SchLib_SourceComp()
{
}

void CDlg_SchLib_SourceComp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USERLIB, m_listboxUserLibrary);
	DDX_Control(pDX, IDC_LIST_COMP, m_listboxComp);
}


BEGIN_MESSAGE_MAP(CDlg_SchLib_SourceComp, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_USERLIB, OnLbnSelchangeListUserlibrary)
END_MESSAGE_MAP()


// CDlg_SchLib_SourceComp 消息处理程序

BOOL CDlg_SchLib_SourceComp::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(nEDA_System_State == SCH)
	{
		int libnum = arrayEDA_SchUserLibraryName.GetCount();
		for(int i=0; i<libnum; i++)
		{
			m_listboxUserLibrary.AddString(arrayEDA_SchUserLibraryName.GetAt(i));
		}

		if(arrayEDA_SchUserLibraryName.GetCount() > 0)
		{
			m_listboxUserLibrary.SetCurSel(0);
			OnLbnSelchangeListUserlibrary();
		}
	}
	else
	{
		int libnum = arrayEDA_PcbUserLibraryName.GetCount();
		for(int i=0; i<libnum; i++)
		{
			m_listboxUserLibrary.AddString(arrayEDA_PcbUserLibraryName.GetAt(i));
		}

		if(arrayEDA_PcbUserLibraryName.GetCount() > 0)
		{
			m_listboxUserLibrary.SetCurSel(0);
			OnLbnSelchangeListUserlibrary();
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_SchLib_SourceComp::OnLbnSelchangeListUserlibrary()
{
	CString userlibrary;
	int sel = m_listboxUserLibrary.GetCurSel();
	if(sel < 0) return;

	m_listboxUserLibrary.GetText(sel, userlibrary);

	if(nEDA_System_State == SCH)
	{
		int lib_index;
		for(lib_index=0; lib_index<arrayEDA_SchUserLibraryName.GetCount(); lib_index++)
		{
			if(userlibrary == arrayEDA_SchUserLibraryName.GetAt(lib_index)) break;
		}

		char libstring[MAX_PATH];
		sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, userlibrary);
		
		CFile cfileUserLibrary;
		if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); return;}
		CArchive ar(&cfileUserLibrary, CArchive::load);

		CString headformat;
		ar >> headformat;
		if(headformat != SCH_Library_Head) {	AfxMessageBox("库格式无法识别."); ar.Close();	cfileUserLibrary.Close();	return; }

		m_listboxComp.ResetContent(); 
		m_arrayLibraryCompList.RemoveAll();

		int totalnum;
		ar >> totalnum;
		char comp_name[100];
		char comp_description[212];
		for(int j = 0; j < totalnum; j++)
		{
			ar.Read(comp_name, 100);
			ar.Read(comp_description, 212);
			int offset, length;
			ar >> offset;
			ar >> length;
			CString cstring = comp_name;
			
			Struct_CompList complist;
			strcpy(complist.compname, comp_name);
			complist.basiclibrary = FALSE;
			complist.liboffset = lib_index; 
			complist.offset = j;			
			complist.length = 0;
			m_arrayLibraryCompList.Add(complist);
			int subindex = m_listboxComp.AddString(cstring);
			m_listboxComp.SetItemData(subindex, j);
		}
		ar.Close();
		cfileUserLibrary.Close();

		if(m_listboxComp.GetCount() > 0) m_listboxComp.SetCurSel(0);
	}
	else
	{
		int lib_index;
		for(lib_index=0; lib_index<arrayEDA_PcbUserLibraryName.GetCount(); lib_index++)
		{
			if(userlibrary == arrayEDA_PcbUserLibraryName.GetAt(lib_index)) break;
		}

		char libstring[MAX_PATH];
		sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, userlibrary);
		
		CFile cfileUserLibrary;
		if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); return;}
		CArchive ar(&cfileUserLibrary, CArchive::load);

		CString headformat;
		ar >> headformat;
		if(headformat != PCB_Library_Head) {	AfxMessageBox("库格式无法识别."); ar.Close();	cfileUserLibrary.Close();	return; }

		m_listboxComp.ResetContent(); 
		m_arrayLibraryCompList.RemoveAll();

		int totalnum;
		ar >> totalnum;
		char comp_name[100];
		char comp_description[212];
		for(int j = 0; j < totalnum; j++)
		{
			ar.Read(comp_name, 100);
			ar.Read(comp_description, 212);
			int offset, length;
			ar >> offset;
			ar >> length;
			CString cstring = comp_name;
			
			Struct_CompList complist;
			strcpy(complist.compname, comp_name);
			complist.basiclibrary = FALSE;
			complist.liboffset = lib_index; 
			complist.offset = j;			
			complist.length = 0;
			m_arrayLibraryCompList.Add(complist);
			int subindex = m_listboxComp.AddString(cstring);
			m_listboxComp.SetItemData(subindex, j);
		}
		ar.Close();
		cfileUserLibrary.Close();

		if(m_listboxComp.GetCount() > 0) m_listboxComp.SetCurSel(0);
	}
}

void CDlg_SchLib_SourceComp::OnOK()
{
	int sel_index = m_listboxComp.GetCurSel();
	if(sel_index != LB_ERR)
	{
		int i = m_listboxComp.GetItemData(sel_index);
		if(i >= 0)
		{
			m_complist = m_arrayLibraryCompList.GetAt(i);
		}
	}

	CDialog::OnOK();
}
