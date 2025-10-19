// Dlg_PlaceComp.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Dlg_PlaceComp.h"
#include ".\dlg_placecomp.h"
#include "systemext.h"


// CDlg_PlaceComp 对话框

IMPLEMENT_DYNAMIC(CDlg_PlaceComp, CDialog)
CDlg_PlaceComp::CDlg_PlaceComp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PlaceComp::IDD, pParent)
	, m_csComboCompName(_T(""))
	, m_csEditDesignator(_T(""))
	, m_csEditComment(_T(""))
	, m_csComboPackage(_T(""))
{
	m_CompList.compname[0] = '\0';
	m_nComboPart = 1;
}

CDlg_PlaceComp::~CDlg_PlaceComp()
{
}

void CDlg_PlaceComp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_CBString(pDX, IDC_COMBO_COMP_NAME, m_csComboCompName);
	DDX_Text(pDX, IDC_EDIT_COMP_PREFIX, m_csEditDesignator);
	DDX_Text(pDX, IDC_EDIT_COMP_COMMENT, m_csEditComment);
	DDX_CBString(pDX, IDC_COMBO_COMP_PACKAGE, m_csComboPackage);
}


BEGIN_MESSAGE_MAP(CDlg_PlaceComp, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnBnClickedButtonSearch)
	ON_CBN_SELCHANGE(IDC_COMBO_COMP_NAME, OnCbnSelchangeComboCompName)
	ON_CBN_SELCHANGE(IDC_COMBO_COMP_PACKAGE, OnCbnSelchangeComboCompPackage)
	ON_CBN_SELCHANGE(IDC_COMBO_COMP_PARTID, OnCbnSelchangeComboCompPartid)
	ON_EN_CHANGE(IDC_EDIT_COMP_COMMENT, OnEnChangeEditCompComment)
	ON_EN_CHANGE(IDC_EDIT_COMP_PREFIX, OnEnChangeEditCompPrefix)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
END_MESSAGE_MAP()


// CDlg_PlaceComp 消息处理程序

BOOL CDlg_PlaceComp::OnInitDialog()
{
	CDialog::OnInitDialog();

	if( arrayEDA_SchCompHistory.IsEmpty() != TRUE)
	{
		CComboBox* pComboCompName = (CComboBox*)GetDlgItem(IDC_COMBO_COMP_NAME);
		Struct_CompList complist;
		for(int i=0; i<arrayEDA_SchCompHistory.GetCount(); i++)
		{
			complist = arrayEDA_SchCompHistory.GetAt(i);
			pComboCompName->AddString(complist.compname);
		}

		pComboCompName->SetWindowText(csEDA_SchCompHistory);
		for(int i=0; i<arrayEDA_SchCompHistory.GetCount(); i++)
		{
			complist = arrayEDA_SchCompHistory.GetAt(i);
			CString str1 = complist.compname;
			str1.MakeUpper();
			CString str2 = csEDA_SchCompHistory;
			str2.MakeUpper();
			if(str1 == str2) break;
		}
		SetParam(complist);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_PlaceComp::OnBnClickedButtonSearch()
{
	int nRet = 101; //ID_Search_Comp

	EndDialog(nRet);
}

void CDlg_PlaceComp::OnOK()
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
	for(i=0; i<arrayEDA_SchCompHistory.GetCount(); i++)
	{
		complist = arrayEDA_SchCompHistory.GetAt(i);
		CString str1 = complist.compname;
		str1.MakeUpper();
		CString str2 = csCompName;
		str2.MakeUpper();
		if(str1 == str2)
		{
			m_CompList = complist;
			SetParam(complist);
			CDialog::OnOK();
			return;
		}
	}

	for(int k=0; k<SCH_SYSTEM_LIB_NUMBER; k++)
	{
		for(int j=0; j<arrayEDA_SchLibraryCompName[k].GetCount(); j++)
		{
			Struct_CompList complist;
			complist = arrayEDA_SchLibraryCompName[k].GetAt(j);
			CString str1 = complist.compname;
			str1.MakeUpper();
			CString str2 = csCompName;
			str2.MakeUpper();
			if(str1 == str2)
			{
				arrayEDA_SchCompHistory.InsertAt(0,complist);
				csEDA_SchCompHistory = complist.compname;
				m_CompList = complist;
				SetParam(complist);
				CDialog::OnOK();
				return;
			}
		}
	}

	for(int k=0; k<arrayEDA_SchUserLibraryName.GetCount(); k++)
	{
		CString userlibrary = arrayEDA_SchUserLibraryName.GetAt(k);

		int lib_index = k;

		char libstring[MAX_PATH];
		sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, userlibrary);
			
		CFile cfileUserLibrary;
		if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE)  continue;

		CArchive ar(&cfileUserLibrary, CArchive::load);

		CString headformat;
		ar >> headformat;
		if(headformat != SCH_Library_Head)
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
				arrayEDA_SchCompHistory.InsertAt(0,complist);
				csEDA_SchCompHistory = complist.compname;
				m_CompList = complist;
				SetParam(complist);
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
	sprintf(s, "名称为 %s 的元件未找到.", csCompName);
	AfxMessageBox(s);
}

void CDlg_PlaceComp::SetParam(Struct_CompList& complist)
{
	int offset, length;
	unsigned char* pbuffer = (unsigned char*)malloc(1000000); //new unsigned char[250000];

	if(complist.basiclibrary == TRUE) 
	{
		//基本元件库SCH_V1.LIB
		offset = complist.liboffset + complist.offset;
		length = complist.length;

		cfileEDA_SchLibrary.Seek(offset, CFile::begin);
		cfileEDA_SchLibrary.Read(pbuffer, length);
	}
	else 
	{

		CString userlibrary, compname;
		userlibrary = arrayEDA_SchUserLibraryName.GetAt(complist.liboffset);

		char libstring[MAX_PATH];
		sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, userlibrary);

		CFile cfileUserLibrary;
		if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE) return; 
		CArchive ar(&cfileUserLibrary, CArchive::load);


		CString headformat;
		ar >> headformat;
		if(headformat != SCH_Library_Head)	{	ar.Close();	cfileUserLibrary.Close();	return; }
		int totalnum;
		ar >> totalnum;
		ar.Close();

		cfileUserLibrary.Seek(32 + 320*complist.offset, CFile::begin);

		Struct_Comp_Head comphead;
		cfileUserLibrary.Read(&comphead, 320);
		offset = comphead.offset;
		length = comphead.length;

		cfileUserLibrary.Seek(offset, CFile::begin);
		cfileUserLibrary.Read(pbuffer, length);

		cfileUserLibrary.Close();
	}

	CMemFile memfile(pbuffer, length);
	CArchive ar(&memfile,CArchive::load);
	
	COb_libcomphead comphead;
	comphead.Serialize(ar);

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_COMP_PARTID);
	pComboBox->ResetContent();
	for(int i=0; i<comphead.m_nPartCount; i++)
	{
		char str[10];
		str[0] = 65 + i;
		str[1] = '\0';
		pComboBox->AddString(str);
	}
	if(comphead.m_nPartCount > 1) pComboBox->SetCurSel(0);
	if(comphead.m_nPartCount <= 1) pComboBox->EnableWindow(FALSE);
	else pComboBox->EnableWindow(TRUE);
	if(	m_nComboPart > comphead.m_nPartCount) m_nComboPart = 1;
	
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_COMP_PACKAGE);
	pComboBox->ResetContent();
	for(int i=0; i<comphead.m_arrayPackage.GetCount(); i++)
	{
		pComboBox->AddString(comphead.m_arrayPackage.GetAt(i));
	}

	if(comphead.m_arrayPackage.GetCount() != 0) 
	{
		pComboBox->SetWindowText(comphead.m_arrayPackage.GetAt(0));
		m_csComboPackage = comphead.m_arrayPackage.GetAt(0);
		pComboBox->SetCurSel(0);
	}
	else 
	{
		pComboBox->SetWindowText("未定义");
		m_csComboPackage.Empty();
	}
	if(comphead.m_arrayPackage.GetCount() <= 1) pComboBox->EnableWindow(FALSE);
	else pComboBox->EnableWindow(TRUE);
	
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMP_COMMENT);
	pEdit->SetWindowText(complist.compname);
	m_csEditComment = complist.compname;

	pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMP_PREFIX);
	comphead.m_csRef.TrimRight('?');
	CString cstr = comphead.m_csRef + "?";
	pEdit->SetWindowText(cstr);
	m_csEditDesignator = cstr;

	ar.Close();
	memfile.Close();
	free(pbuffer);
}

void CDlg_PlaceComp::OnCbnSelchangeComboCompName()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_COMP_NAME);
	int index = pComboBox->GetCurSel();
	CString cstr;
	pComboBox->GetLBText(index, cstr);
	Struct_CompList complist;
	for(int i=0; i<arrayEDA_SchCompHistory.GetCount(); i++)
	{
		complist = arrayEDA_SchCompHistory.GetAt(i);
		CString cstring = complist.compname;
		cstring.MakeUpper();
		cstr.MakeUpper();
		if(cstr == cstring)
		{
			csEDA_SchCompHistory = cstring;
			break;
		}
	}
	
	SetParam(complist);
}

void CDlg_PlaceComp::OnCbnSelchangeComboCompPackage()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_COMP_PACKAGE);
	int index = pComboBox->GetCurSel();
	pComboBox->GetLBText(index, m_csComboPackage);
}

void CDlg_PlaceComp::OnCbnSelchangeComboCompPartid()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_COMP_PARTID);
	int index = pComboBox->GetCurSel();
	m_nComboPart = index + 1;
}

void CDlg_PlaceComp::OnEnChangeEditCompComment()
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMP_COMMENT);
	pEdit->GetWindowText(m_csEditComment);
}

void CDlg_PlaceComp::OnEnChangeEditCompPrefix()
{
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMP_PREFIX);
	pEdit->GetWindowText(m_csEditDesignator);
}

void CDlg_PlaceComp::OnBnClickedButtonClear()
{
	if(AfxMessageBox("清除元件历史记录, 确认?", MB_YESNO|MB_ICONWARNING) == IDYES)
	{
		CComboBox* pComboCompName = (CComboBox*)GetDlgItem(IDC_COMBO_COMP_NAME);
		pComboCompName->ResetContent();
		arrayEDA_SchCompHistory.RemoveAll();
	
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMP_PREFIX);
		pEdit->SetWindowText(NULL);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_COMP_COMMENT);
		pEdit->SetWindowText(NULL);

		pComboCompName = (CComboBox*)GetDlgItem(IDC_COMBO_COMP_PACKAGE);
		pComboCompName->ResetContent();
		pComboCompName = (CComboBox*)GetDlgItem(IDC_COMBO_COMP_PARTID);
		pComboCompName->ResetContent();
	}
}
