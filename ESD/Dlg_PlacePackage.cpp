// Dlg_PlacePackage.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PlacePackage.h"
#include ".\dlg_placepackage.h"
#include "systemext.h"

// CDlg_PlacePackage 对话框

IMPLEMENT_DYNAMIC(CDlg_PlacePackage, CDialog)
CDlg_PlacePackage::CDlg_PlacePackage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PlacePackage::IDD, pParent)
{
	m_PackageList.compname[0] = '\0';
}

CDlg_PlacePackage::~CDlg_PlacePackage()
{
}

void CDlg_PlacePackage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_COMP_NAME, m_csComboCompName);
	DDX_Text(pDX, IDC_EDIT_COMP_PREFIX, m_csEditDesignator);
	DDX_Text(pDX, IDC_EDIT_COMP_COMMENT, m_csEditComment);
}


BEGIN_MESSAGE_MAP(CDlg_PlacePackage, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnBnClickedButtonSearch)
	ON_CBN_SELCHANGE(IDC_COMBO_COMP_NAME, OnCbnSelchangeComboCompName)
	ON_EN_CHANGE(IDC_EDIT_COMP_COMMENT, OnEnChangeEditCompComment)
	ON_EN_CHANGE(IDC_EDIT_COMP_PREFIX, OnEnChangeEditCompPrefix)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CDlg_PlacePackage 消息处理程序

BOOL CDlg_PlacePackage::OnInitDialog()
{
	CDialog::OnInitDialog();

	if( arrayEDA_PcbCompHistory.IsEmpty() != TRUE)
	{
		CComboBox* pComboCompName = (CComboBox*)GetDlgItem(IDC_COMBO_COMP_NAME);
		Struct_CompList complist;
		for(int i=0; i<arrayEDA_PcbCompHistory.GetCount(); i++)
		{
			complist = arrayEDA_PcbCompHistory.GetAt(i);
			pComboCompName->AddString(complist.compname);
		}

		pComboCompName->SetWindowText(csEDA_PcbCompHistory);
		for(int i=0; i<arrayEDA_PcbCompHistory.GetCount(); i++)
		{
			complist = arrayEDA_PcbCompHistory.GetAt(i);
			CString str1 = complist.compname;
			str1.MakeUpper();
			CString str2 = csEDA_PcbCompHistory;
			str2.MakeUpper();
			if(str1 == str2) break;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_PlacePackage::OnBnClickedButtonSearch()
{
	int nRet = 101; //ID_Search_Comp

	EndDialog(nRet);
}

void CDlg_PlacePackage::OnOK()
{
	CComboBox* pComboCompName = (CComboBox*)GetDlgItem(IDC_COMBO_COMP_NAME);
	
	CString csCompName;
	pComboCompName->GetWindowText(csCompName);

	if(csCompName.IsEmpty() == TRUE)
	{
		AfxMessageBox("无效元件名称.");
		return;
	}

	Struct_CompList complist;
	int i;
	for(i=0; i<arrayEDA_PcbCompHistory.GetCount(); i++)
	{
		complist = arrayEDA_PcbCompHistory.GetAt(i);
		CString str1 = complist.compname;
		str1.MakeUpper();
		CString str2 = csCompName;
		str2.MakeUpper();
		if(str1 == str2)
		{
			m_PackageList = complist;
			CDialog::OnOK();
			return;
		}
	}

	for(int k=0; k<PCB_SYSTEM_LIB_NUMBER; k++)
	{
		for(int j=0; j<arrayEDA_PcbLibraryCompName[k].GetCount(); j++)
		{
			Struct_CompList complist;
			complist = arrayEDA_PcbLibraryCompName[k].GetAt(j);
			CString str1 = complist.compname;
			str1.MakeUpper();
			CString str2 = csCompName;
			str2.MakeUpper();
			if(str1 == str2)
			{
				arrayEDA_PcbCompHistory.InsertAt(0,complist);
				csEDA_PcbCompHistory = complist.compname;
				m_PackageList = complist;
				CDialog::OnOK();
				return;
			}
		}
	}

	for(int k=0; k<arrayEDA_PcbUserLibraryName.GetCount(); k++)
	{
		CString userlibrary = arrayEDA_PcbUserLibraryName.GetAt(k);

		int lib_index = k;

		char libstring[MAX_PATH];
		sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, userlibrary);
			
		CFile cfileUserLibrary;
		if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE)  continue;

		CArchive ar(&cfileUserLibrary, CArchive::load);

		CString headformat;
		ar >> headformat;
		if(headformat != PCB_Library_Head)
		{
			ar.Close();
			cfileUserLibrary.Close();
			continue;
		}

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
			CString str1 = comp_name;
			str1.MakeUpper();

			Struct_CompList complist;
			strcpy(complist.compname, comp_name);
			complist.basiclibrary = FALSE;
			complist.liboffset = lib_index; 
			complist.offset = j;			
			complist.length = 0;

			CString str2 = csCompName;
			str2.MakeUpper();
			if(str1 == str2 )
			{
				arrayEDA_PcbCompHistory.InsertAt(0,complist);
				csEDA_PcbCompHistory = complist.compname;
				m_PackageList = complist;
				ar.Close();
				cfileUserLibrary.Close();
				CDialog::OnOK();
				return;
			}
		}
		ar.Close();
		cfileUserLibrary.Close();
	}

	char s[200];
	sprintf(s, "名称为 %s 的封装未找到.", csCompName);
	AfxMessageBox(s);
}

void CDlg_PlacePackage::OnCbnSelchangeComboCompName()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_COMP_NAME);
	int index = pComboBox->GetCurSel();
	CString cstr;
	pComboBox->GetLBText(index, cstr);
	Struct_CompList complist;
	for(int i=0; i<arrayEDA_PcbCompHistory.GetCount(); i++)
	{
		complist = arrayEDA_PcbCompHistory.GetAt(i);
		CString cstring = complist.compname;
		cstring.MakeUpper();
		cstr.MakeUpper();
		if(cstr == cstring)
		{
			csEDA_PcbCompHistory = cstring;
			break;
		}
	}
}

void CDlg_PlacePackage::OnEnChangeEditCompComment()
{
	//Comment
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMP_COMMENT);
	pEdit->GetWindowText(m_csEditComment);
}

void CDlg_PlacePackage::OnEnChangeEditCompPrefix()
{
	//Comment
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMP_PREFIX);
	pEdit->GetWindowText(m_csEditDesignator);
}

void CDlg_PlacePackage::OnBnClickedButtonClear()
{
	if(AfxMessageBox("清除封装历史记录, 确认?", MB_YESNO|MB_ICONWARNING) == IDYES)
	{
		CComboBox* pComboCompName = (CComboBox*)GetDlgItem(IDC_COMBO_COMP_NAME);
		pComboCompName->ResetContent();
		arrayEDA_PcbCompHistory.RemoveAll();
	
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMP_PREFIX);
		pEdit->SetWindowText(NULL);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMP_COMMENT);
		pEdit->SetWindowText(NULL);
	}
}
