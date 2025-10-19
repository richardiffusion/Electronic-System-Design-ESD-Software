// Dlg_SchLib_Add.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_SchLib_Add.h"
#include ".\dlg_schlib_add.h"
#include "systemext.h"


// CDlg_SchLib_Add 对话框

IMPLEMENT_DYNAMIC(CDlg_SchLib_Add, CDialog)
CDlg_SchLib_Add::CDlg_SchLib_Add(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_SchLib_Add::IDD, pParent)
	, m_nRadioLib(0)
	, m_nRadioComp(0)
	, m_csEditCompName(_T(""))
	, m_csEditRef(_T(""))
	, m_csEditPack(_T(""))
	, m_csEditDescription(_T(""))
	, m_csStaticNewPart(_T(""))
	, m_nComboPart(0)
	, m_csStaticOldName(_T(""))
{
	m_nPartCount = 1;
}

CDlg_SchLib_Add::~CDlg_SchLib_Add()
{
}

void CDlg_SchLib_Add::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_NEWLIB, m_nRadioLib);
	DDX_Radio(pDX, IDC_RADIO_NEWCOMP, m_nRadioComp);
	DDX_Control(pDX, IDC_LIST_LIB, m_listboxUserLibrary);
	DDX_Text(pDX, IDC_EDIT_NAME, m_csEditCompName);
	DDX_Text(pDX, IDC_EDIT_REF, m_csEditRef);
	DDX_Text(pDX, IDC_EDIT_PACK, m_csEditPack);
	DDX_Text(pDX, IDC_EDIT_DESCRIPTION, m_csEditDescription);
	DDX_Control(pDX, IDC_COMBO_PART, m_comboboxPart);
	DDX_Text(pDX, IDC_STATIC_PART, m_csStaticNewPart);
	DDX_Control(pDX, IDC_LIST_COMP, m_listboxComp);
	DDX_CBIndex(pDX, IDC_COMBO_PART, m_nComboPart);
	DDX_Text(pDX, IDC_STATIC_OLDNAME, m_csStaticOldName);
}


BEGIN_MESSAGE_MAP(CDlg_SchLib_Add, CDialog)
	ON_BN_CLICKED(IDC_RADIO_NEWCOMP, OnBnClickedRadioNewcomp)
	ON_BN_CLICKED(IDC_RADIO_NEWLIB, OnBnClickedRadioNewlib)
	ON_BN_CLICKED(IDC_RADIO_OLDCOMP, OnBnClickedRadioOldcomp)
	ON_BN_CLICKED(IDC_RADIO_OLDLIB, OnBnClickedRadioOldlib)
	ON_BN_CLICKED(IDC_BUTTON_CHECK, OnBnClickedButtonCheck)
	ON_LBN_SELCHANGE(IDC_LIST_LIB, OnLbnSelchangeListLib)
	ON_LBN_SELCHANGE(IDC_LIST_COMP, OnLbnSelchangeListComp)
END_MESSAGE_MAP()


// CDlg_SchLib_Add 消息处理程序

BOOL CDlg_SchLib_Add::OnInitDialog()
{
	CDialog::OnInitDialog();

	CWnd* pWnd;
	
	pWnd = GetDlgItem(IDC_LIST_LIB);
	pWnd->EnableWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_COMBO_PART);
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_LIST_COMP);
	pWnd->EnableWindow(FALSE);

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_OLDCOMP);
	pButton->EnableWindow(FALSE);
	
	int libnum = arrayEDA_SchUserLibraryName.GetCount();
	for(int i=0; i<libnum; i++)
	{
		m_listboxUserLibrary.AddString(arrayEDA_SchUserLibraryName.GetAt(i));
	}

	if(arrayEDA_SchUserLibraryName.GetCount() > 0)
	{
		m_listboxUserLibrary.SetCurSel(0);
		OnLbnSelchangeListLib();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_SchLib_Add::OnBnClickedRadioNewcomp()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_COMBO_PART);
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_LIST_COMP);
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_EDIT_NAME);
	pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_EDIT_REF);
	pWnd->EnableWindow(TRUE);
	
	pWnd = GetDlgItem(IDC_EDIT_PACK);
	pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_EDIT_DESCRIPTION);
	pWnd->EnableWindow(TRUE);
	
	pWnd = GetDlgItem(IDC_BUTTON_CHECK);
	pWnd->EnableWindow(TRUE);
}

void CDlg_SchLib_Add::OnBnClickedRadioNewlib()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_LIST_LIB);
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_EDIT_LIBNAME);
	pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_COMBO_PART);
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_LIST_COMP);
	pWnd->EnableWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_EDIT_NAME);
	pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_EDIT_REF);
	pWnd->EnableWindow(TRUE);
	
	pWnd = GetDlgItem(IDC_EDIT_PACK);
	pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_EDIT_DESCRIPTION);
	pWnd->EnableWindow(TRUE);
	
	pWnd = GetDlgItem(IDC_BUTTON_CHECK);
	pWnd->EnableWindow(TRUE);

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_NEWCOMP);
	pButton->SetCheck(TRUE);
	pButton = (CButton*)GetDlgItem(IDC_RADIO_OLDCOMP);
	pButton->SetCheck(FALSE);
	pButton->EnableWindow(FALSE);
}

void CDlg_SchLib_Add::OnBnClickedRadioOldcomp()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_COMBO_PART);
	pWnd->EnableWindow(TRUE);
	
	pWnd = GetDlgItem(IDC_LIST_COMP);
	pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_EDIT_NAME);
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_EDIT_REF);
	pWnd->EnableWindow(FALSE);
	
	pWnd = GetDlgItem(IDC_EDIT_PACK);
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_EDIT_DESCRIPTION);
	pWnd->EnableWindow(FALSE);

	pWnd = GetDlgItem(IDC_BUTTON_CHECK);
	pWnd->EnableWindow(FALSE);

	OnLbnSelchangeListLib();
}

void CDlg_SchLib_Add::OnBnClickedRadioOldlib()
{
	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_LIST_LIB);
	pWnd->EnableWindow(TRUE);

	pWnd = GetDlgItem(IDC_EDIT_LIBNAME);
	pWnd->EnableWindow(FALSE);

	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_OLDCOMP);
	pButton->EnableWindow(TRUE);

}

void CDlg_SchLib_Add::OnBnClickedButtonCheck()
{
	UpdateData(TRUE);
	CString csCompName = m_csEditCompName;
	csCompName.MakeUpper();

	CString str;

	if(m_csEditCompName.IsEmpty() != TRUE)
	{
		int num=0;
		for(int i=0; i<SCH_SYSTEM_LIB_NUMBER; i++)
		{
			for(int j=0; j<arrayEDA_SchLibraryCompName[i].GetCount(); j++)
			{
				Struct_CompList complist;
				complist = arrayEDA_SchLibraryCompName[i].GetAt(j);
				CString cstring = complist.compname;
				cstring.MakeUpper();
				if(cstring == csCompName) num++;
			}
		}

		if(num > 0)	str.Format("标准库发现%d个同名元件.\n", num);

		for(int i=0; i<arrayEDA_SchUserLibraryName.GetCount(); i++)
		{
			CString userlibrary = arrayEDA_SchUserLibraryName.GetAt(i);

			char libstring[MAX_PATH];
			sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, userlibrary);
			
			CFile cfileUserLibrary;
			if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE)  continue;

			Struct_Library_Head libhead;
			cfileUserLibrary.Read(&libhead, 32);

			int totalnum = libhead.totalcompnum;

			for(int j = 0; j < totalnum; j++)
			{
				Struct_Comp_Head comphead;
				cfileUserLibrary.Read(&comphead, 320);
				CString cstring = comphead.name;
				cstring.MakeUpper();
				if(cstring == csCompName)
				{
					CString tempstr;
					tempstr.Format("在用户库%s中发现同名元件.\n", userlibrary);
					str += tempstr;
				}
			}
			cfileUserLibrary.Close();
		}
		
		if(str.IsEmpty() == TRUE) AfxMessageBox("未找到同名元件.");
		else AfxMessageBox(str);
	}
	else AfxMessageBox("元件名称未定义.");
}


void CDlg_SchLib_Add::OnLbnSelchangeListLib()
{
	if(m_listboxComp.IsWindowEnabled() == FALSE) return;

	CString userlibrary;
	int sel = m_listboxUserLibrary.GetCurSel();
	if(sel < 0) return;

	m_listboxUserLibrary.GetText(sel, userlibrary);

	char libstring[MAX_PATH];
	sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, userlibrary);
	
	CFile cfileUserLibrary;
	if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); return;}

	Struct_Library_Head libhead;
	cfileUserLibrary.Read(&libhead, 32);

	int totalnum = libhead.totalcompnum;
	m_listboxComp.ResetContent();
	for(int j = 0; j < totalnum; j++)
	{
		Struct_Comp_Head comphead;
		cfileUserLibrary.Read(&comphead, 320);
		CString cstring = comphead.name;
		m_listboxComp.AddString(cstring);
	}
	cfileUserLibrary.Close();

}

void CDlg_SchLib_Add::OnLbnSelchangeListComp()
{

	int index = m_listboxComp.GetCurSel();
	CString cstring;
	if(index != LB_ERR )
	{
		m_listboxComp.GetText(index, cstring);

		CWnd* pWnd;
		pWnd = GetDlgItem(IDC_STATIC_OLDNAME);
		pWnd->SetWindowText(cstring);
	}
	m_nPartCount = 1;

	CString userlibrary;
	int sel = m_listboxUserLibrary.GetCurSel();
	if(sel < 0) return;
	m_listboxUserLibrary.GetText(sel, userlibrary);
	

	char libstring[MAX_PATH];
	sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, userlibrary);
	
	CFile cfileUserLibrary;
	if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); return;}

	Struct_Library_Head libhead;
	cfileUserLibrary.Read(&libhead, 32);
	int totalnum = libhead.totalcompnum;
	for(int j = 0; j < totalnum; j++)
	{
		Struct_Comp_Head comp_head;
		cfileUserLibrary.Read(&comp_head, 320);

		CString cstr = comp_head.name;
		if(cstr == cstring)
		{
			unsigned char* pbuffer = (unsigned char*)malloc(1000000); 
			int offset = comp_head.offset;
			int length = comp_head.length;

			cfileUserLibrary.Seek(offset, CFile::begin);
			cfileUserLibrary.Read(pbuffer, length);

			CMemFile memfile(pbuffer, length);
			CArchive ar(&memfile,CArchive::load);
			
			COb_libcomphead comphead;
			comphead.Serialize(ar);
				
			CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PART);
			pComboBox->ResetContent();
			for(int k=0; k<comphead.m_nPartCount; k++)
			{
				char s[2];
				s[0] = 65 + k; s[1] = '\0';
				CString strPart = s;
				pComboBox->AddString(strPart);
			}
			m_nPartCount = comphead.m_nPartCount;

			if(pComboBox->GetCount() > 0) 
			{
				if((pcomp->m_nPartCount > 1) && (pcomp->m_nPart <= pComboBox->GetCount() ))
				{
					pComboBox->SetCurSel(pcomp->m_nPart - 1);
				}
				else 
				{
					pComboBox->SetCurSel(0);
				}
			}

			ar.Close();
			memfile.Close();
			free(pbuffer);
			break;
		}
	}
	cfileUserLibrary.Close();
}

void CDlg_SchLib_Add::OnOK()
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
