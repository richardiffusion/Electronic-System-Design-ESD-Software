// Dlg_PCBLib_Browse.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"

#include "Dlg_PcbLib_Browse.h"
#include ".\dlg_pcblib_browse.h"
#include "systemext.h"


// CDlg_PcbLib_Browse 对话框

IMPLEMENT_DYNAMIC(CDlg_PcbLib_Browse, CDialog)
CDlg_PcbLib_Browse::CDlg_PcbLib_Browse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PcbLib_Browse::IDD, pParent)
	, m_bMatchMore(FALSE)
{
	//first_selection = TRUE;
	//first_show = TRUE;
}

CDlg_PcbLib_Browse::~CDlg_PcbLib_Browse()
{
}

void CDlg_PcbLib_Browse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COMP, m_listboxCompList);
	DDX_Control(pDX, IDC_TREE_LIBRARY, m_treeLibrary);
	DDX_Control(pDX, IDC_LIST_USER_LIBRARY, m_listboxUserLibrary);
	DDX_Check(pDX, IDC_CHECK_MATCH, m_bMatchMore);
}


BEGIN_MESSAGE_MAP(CDlg_PcbLib_Browse, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_COMP, OnLbnSelchangeListComp)
	ON_LBN_DBLCLK(IDC_LIST_COMP, OnLbnDblclkListComp)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_LIBRARY, OnTvnSelchangedTreeLibrary)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_CHECK_MATCH, OnBnClickedCheckMatch)
	ON_LBN_SELCHANGE(IDC_LIST_USER_LIBRARY, OnLbnSelchangeListUserLibrary)
END_MESSAGE_MAP()


// CDlg_PcbLib_Browse 消息处理程序
BOOL CDlg_PcbLib_Browse::OnInitDialog()
{
	CDialog::OnInitDialog();

	HTREEITEM hitem[10];
	char s[260], sbak[260];
	int  j, n = 200;

	int lib_index;
	for(int k=0; k<arrayEDA_PcbLibraryTreeStructure.GetCount(); k++)
	{
		CString cstring = arrayEDA_PcbLibraryTreeStructure.GetAt(k);
		strcpy(s, cstring);

		strcpy(sbak, s);
		for(j=0; j<10; j++) 
		{
			if(sbak[j] != 0x09) break;
		}
		memcpy(s,  sbak+j, strlen(sbak+j)+1);


		int i;
		for(i=strlen(s); i>0; i--)
		{
			if(s[i] == '|') break;
		}
		if( i > 0)
		{
			s[i] = '\0';
			memcpy(sbak, s+i+1, strlen(s+i+1)+1);
		}
		else 
		{
			sbak[0] = '\0';
		}


		if(j == 0) 
		{
			hitem[j] = m_treeLibrary.InsertItem(s, 0, 0, TVI_ROOT, TVI_LAST);
			if(strlen(sbak) != 0)
			{
				if(stricmp(sbak,"Y") == 0) m_treeLibrary.SetItemData(hitem[j], -2);		
				else if(stricmp(sbak,"N") == 0) m_treeLibrary.SetItemData(hitem[j], -1);	
				else
				{
					lib_index = atoi(sbak);
					m_treeLibrary.SetItemData(hitem[j], lib_index);
				}
			}
			else m_treeLibrary.SetItemData(hitem[j], -1);
		}
		else 
		{
			if(strlen(sbak) != 0)
			{

				if(stricmp(sbak,"Y") == 0)	
				{
					hitem[j] = m_treeLibrary.InsertItem (s, 0, 0, hitem[j-1], TVI_LAST);
					m_treeLibrary.SetItemData(hitem[j], -2);
				}
				else if(stricmp(sbak,"N") == 0)	
				{
					hitem[j] = m_treeLibrary.InsertItem (s, 0, 0, hitem[j-1], TVI_LAST);
					m_treeLibrary.SetItemData(hitem[j], -1);
				}
				else	
				{
					hitem[j] = m_treeLibrary.InsertItem (s, 1, 1, hitem[j-1], TVI_LAST);
					lib_index = atoi(sbak);
					m_treeLibrary.SetItemData(hitem[j], lib_index);
				}
			}
			else 
			{
				hitem[j] = m_treeLibrary.InsertItem (s, 0, 0, hitem[j-1], TVI_LAST);
				m_treeLibrary.SetItemData(hitem[j], -1);	
			}
		}
	}
		
	HTREEITEM hitem_0, hitem_1, hitem_2, hitem_3; //, hitem_4;
	hitem_0 = m_treeLibrary.GetRootItem( );
	while (hitem_0 != NULL)
	{
		if( m_treeLibrary.ItemHasChildren(hitem_0) == TRUE)
		{
			if(m_treeLibrary.GetItemData(hitem_0) == -1) m_treeLibrary.Expand(hitem_0, TVE_COLLAPSE);
			else m_treeLibrary.Expand(hitem_0, TVE_EXPAND);

			hitem_1 = m_treeLibrary.GetChildItem(hitem_0);
			while(hitem_1 != NULL)
			{
				if( m_treeLibrary.ItemHasChildren(hitem_1) == TRUE)
				{
					if(m_treeLibrary.GetItemData(hitem_1) == -1) m_treeLibrary.Expand(hitem_1, TVE_COLLAPSE);
					else m_treeLibrary.Expand(hitem_1, TVE_EXPAND);

					hitem_2 = m_treeLibrary.GetChildItem(hitem_1);
					while(hitem_2 != NULL)
					{
						if( m_treeLibrary.ItemHasChildren(hitem_2) == TRUE)
						{
							if(m_treeLibrary.GetItemData(hitem_2) == -1) m_treeLibrary.Expand(hitem_2, TVE_COLLAPSE);
							else m_treeLibrary.Expand(hitem_2, TVE_EXPAND);

							hitem_3 = m_treeLibrary.GetChildItem(hitem_2);
							while(hitem_3 != NULL)
							{
								
								hitem_3 = m_treeLibrary.GetNextSiblingItem(hitem_3);
							}
						}
						hitem_2 = m_treeLibrary.GetNextSiblingItem(hitem_2);
					}
				}
				hitem_1 = m_treeLibrary.GetNextSiblingItem(hitem_1);
			}
		}
		hitem_0 = m_treeLibrary.GetNextSiblingItem(hitem_0);
	}

	hitem_0 = m_treeLibrary.GetRootItem( );
	int item_num = 0;
	while (hitem_0 != NULL)
	{
		if(nEDA_PcbLibrarySelectedItem == item_num)
		{
			BOOL b = m_treeLibrary.Select(hitem_0,TVGN_CARET   );
		}
		item_num++;
		if( m_treeLibrary.ItemHasChildren(hitem_0) == TRUE)
		{
			hitem_1 = m_treeLibrary.GetChildItem(hitem_0);
			while(hitem_1 != NULL)
			{
				if(nEDA_PcbLibrarySelectedItem == item_num) 
				{
					BOOL b = m_treeLibrary.Select(hitem_1,TVGN_CARET   );
				}
				item_num++;
				if( m_treeLibrary.ItemHasChildren(hitem_1) == TRUE)
				{
					hitem_2 = m_treeLibrary.GetChildItem(hitem_1);
					while(hitem_2 != NULL)
					{
						if(nEDA_PcbLibrarySelectedItem == item_num) 
						{
							BOOL b = m_treeLibrary.Select(hitem_2,TVGN_CARET   );
						}
						item_num++;
						if( m_treeLibrary.ItemHasChildren(hitem_2) == TRUE)
						{
							hitem_3 = m_treeLibrary.GetChildItem(hitem_2);
							while(hitem_3 != NULL)
							{
								if(nEDA_PcbLibrarySelectedItem == item_num)
								{
									BOOL b = m_treeLibrary.Select(hitem_3,TVGN_CARET   );
								}
								item_num++;
								
								hitem_3 = m_treeLibrary.GetNextSiblingItem(hitem_3);
							}
						}
						hitem_2 = m_treeLibrary.GetNextSiblingItem(hitem_2);
					}
				}
				hitem_1 = m_treeLibrary.GetNextSiblingItem(hitem_1);
			}
		}
		hitem_0 = m_treeLibrary.GetNextSiblingItem(hitem_0);
	}

	
	if(arrayEDA_PcbLibraryCompList.GetCount() != 0)
	{
		for(int i=0; i<arrayEDA_PcbLibraryCompList.GetCount(); i++)
		{
			Struct_CompList complist;
			complist = arrayEDA_PcbLibraryCompList.GetAt(i);
			int index = m_listboxCompList.AddString(complist.compname);
			m_listboxCompList.SetItemData(index, i);
		}
	}
		
	if( (nEDA_PcbCompSelectedItem >= 0)&&(m_listboxCompList.GetCount() > 0)&&(nEDA_PcbCompSelectedItem < m_listboxCompList.GetCount()) )
	{
		m_listboxCompList.SetCurSel(nEDA_PcbCompSelectedItem);
	
	}
	m_listboxCompList.SetColumnWidth(175);

	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_EDIT_SEARCH);
	pComboBox->SetWindowText(csEDA_PcbSearchCompName);

	
	int libnum = arrayEDA_PcbUserLibraryName.GetCount();
	for(int i=0; i<libnum; i++)
	{
		m_listboxUserLibrary.AddString(arrayEDA_PcbUserLibraryName.GetAt(i));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_PcbLib_Browse::OnTvnSelchangedTreeLibrary(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hitem = m_treeLibrary.GetSelectedItem( );
	int index = m_treeLibrary.GetItemData(hitem);
	if(index < 0) return; 

	
	m_listboxCompList.ResetContent(); 
	arrayEDA_PcbLibraryCompList.RemoveAll();

	for(int i=0; i<arrayEDA_PcbLibraryCompName[index].GetCount(); i++)
	{
		Struct_CompList complist;
		complist = arrayEDA_PcbLibraryCompName[index].GetAt(i);
		arrayEDA_PcbLibraryCompList.Add(complist);
		int subindex = m_listboxCompList.AddString(complist.compname);
		m_listboxCompList.SetItemData(subindex, i);
	}
		
	nEDA_PcbCompSelectedItem = 0;
	if( (nEDA_PcbCompSelectedItem >= 0)&&(m_listboxCompList.GetCount() > 0)&&(nEDA_PcbCompSelectedItem < m_listboxCompList.GetCount()) )
	{
		m_listboxCompList.SetCurSel(nEDA_PcbCompSelectedItem);
		//m_listboxCompList.GetText(nEDA_PcbCompSelectedItem, m_csSelectedComponent);
	}
		

	ShowPackage();

	*pResult = 0;
}

void CDlg_PcbLib_Browse::OnLbnSelchangeListComp()
{
	
	nEDA_PcbCompSelectedItem = m_listboxCompList.GetCurSel();
	
	ShowPackage();
}

void CDlg_PcbLib_Browse::OnLbnDblclkListComp()
{
	nEDA_PcbCompSelectedItem = m_listboxCompList.GetCurSel();
	int sel_index = m_listboxCompList.GetCurSel();
	if(sel_index != LB_ERR)
	{
		int i = m_listboxCompList.GetItemData(sel_index);
		if(i >= 0)
		{
			m_CompList = arrayEDA_PcbLibraryCompList.GetAt(i);
		}
	}

	CDialog::OnOK();
}

void CDlg_PcbLib_Browse::OnOK()
{
	nEDA_PcbCompSelectedItem = m_listboxCompList.GetCurSel();
	int sel_index = m_listboxCompList.GetCurSel();
	if(sel_index != LB_ERR)
	{
		int i = m_listboxCompList.GetItemData(sel_index);
		if(i >= 0)
		{
			m_CompList = arrayEDA_PcbLibraryCompList.GetAt(i);
		}
	}

	CDialog::OnOK();
}

void CDlg_PcbLib_Browse::OnPaint()
{
	CPaintDC dc(this); 
	ShowPackage();
}

void CDlg_PcbLib_Browse::OnBnClickedButtonSearch()
{

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_EDIT_SEARCH);
	pComboBox->GetWindowText(csEDA_PcbSearchCompName);

	csEDA_PcbSearchCompName.MakeUpper();
	
	if(csEDA_PcbSearchCompName.IsEmpty() != TRUE)
	{

		arrayEDA_PcbLibraryCompList.RemoveAll();
		m_listboxCompList.ResetContent();

		for(int i=0; i<PCB_SYSTEM_LIB_NUMBER; i++)
		{
			for(int j=0; j<arrayEDA_PcbLibraryCompName[i].GetCount(); j++)
			{
				Struct_CompList complist;
				complist = arrayEDA_PcbLibraryCompName[i].GetAt(j);
				CString cstring = complist.compname;
				cstring.MakeUpper();
				if(((m_bMatchMore == TRUE)&&(cstring.Find(csEDA_PcbSearchCompName) >= 0))||(cstring == csEDA_PcbSearchCompName))
				{
					arrayEDA_PcbLibraryCompList.Add(complist);
					if(arrayEDA_PcbLibraryCompList.GetCount() > nEDA_PcbCompMaxSearchNumber)
					{
						AfxMessageBox("太多的相似元件, 查找终止, 定义更详细的名称并重新查找。");
						i=10000;
						break;
					}
				}
			}
		}

		for(int i=0; i<arrayEDA_PcbUserLibraryName.GetCount(); i++)
		{
			CString userlibrary = arrayEDA_PcbUserLibraryName.GetAt(i);

			int lib_index = i;


			char libstring[MAX_PATH];
			sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, userlibrary);
			
			CFile cfileUserLibrary;
			if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE)  continue;

			CArchive ar(&cfileUserLibrary, CArchive::load);

			CString headformat;
			ar >> headformat;
			if(headformat != PCB_Library_Head)	{	ar.Close();	cfileUserLibrary.Close(); continue;	}

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
				//strcat(complist.compname, "(用户元件库)");
				complist.basiclibrary = FALSE;
				complist.liboffset = lib_index;
				complist.offset = j;			
				complist.length = 0;

				cstring.MakeUpper();
				if(((m_bMatchMore == TRUE)&&(cstring.Find(csEDA_PcbSearchCompName) >= 0))||(cstring == csEDA_PcbSearchCompName))
				{
					arrayEDA_PcbLibraryCompList.Add(complist);
					if(arrayEDA_PcbLibraryCompList.GetCount() > nEDA_PcbCompMaxSearchNumber)
					{
						AfxMessageBox("太多的相似元件, 查找终止, 定义更详细的名称并重新查找。");
						i=10000;
						break;
					}
				}

			}
			ar.Close();
			cfileUserLibrary.Close();
		}
		
		if(arrayEDA_PcbLibraryCompList.GetCount() != 0)
		{
			
			for(int i=0; i<arrayEDA_PcbLibraryCompList.GetCount(); i++)
			{
				Struct_CompList complist;
				complist = arrayEDA_PcbLibraryCompList.GetAt(i);
				int index = m_listboxCompList.AddString(complist.compname);
				m_listboxCompList.SetItemData(index, i);
			}
		}
		else
		{
			if(m_bMatchMore == TRUE) AfxMessageBox("未找到相同或相似名称的元件.");
			else AfxMessageBox("未找到同名元件.");
		}
		
		nEDA_PcbCompSelectedItem = 0;
		if( (nEDA_PcbCompSelectedItem >= 0)&&(m_listboxCompList.GetCount() > 0)&&(nEDA_PcbCompSelectedItem < m_listboxCompList.GetCount()) )
		{
			m_listboxCompList.SetCurSel(nEDA_PcbCompSelectedItem);
			ShowPackage();
		}
	}
}

void CDlg_PcbLib_Browse::OnBnClickedCheckMatch()
{
	UpdateData(TRUE);
}

void CDlg_PcbLib_Browse::OnDestroy()
{
	CDialog::OnDestroy();

	HTREEITEM hitem_0, hitem_1, hitem_2, hitem_3; 
	
	int item_num = 0;
	CString cstring;

	HTREEITEM selected_item;
	selected_item = m_treeLibrary.GetSelectedItem();

	hitem_0 = m_treeLibrary.GetRootItem( );
	while (hitem_0 != NULL)
	{
		if(hitem_0 == selected_item) nEDA_PcbLibrarySelectedItem = item_num;
		item_num++;

		if( m_treeLibrary.ItemHasChildren(hitem_0) == TRUE)
		{
			cstring = arrayEDA_PcbLibraryTreeStructure.GetAt(item_num-1);
			if(cstring.Find("|Y") >= 0) cstring.TrimRight("|Y");
			else if(cstring.Find("|N") >= 0) cstring.TrimRight("|N");
			if( (m_treeLibrary.GetItemState(hitem_0, TVIS_EXPANDED)&TVIS_EXPANDED) != 0) cstring += "|Y";
			else cstring += "|N";
			arrayEDA_PcbLibraryTreeStructure.SetAt(item_num-1, cstring);


			hitem_1 = m_treeLibrary.GetChildItem(hitem_0);
			while(hitem_1 != NULL)
			{
				if(hitem_1 == selected_item) nEDA_PcbLibrarySelectedItem = item_num;
				item_num++;

				if( m_treeLibrary.ItemHasChildren(hitem_1) == TRUE)
				{
					cstring = arrayEDA_PcbLibraryTreeStructure.GetAt(item_num-1);
					if(cstring.Find("|Y") >= 0) cstring.TrimRight("|Y");
					else if(cstring.Find("|N") >= 0) cstring.TrimRight("|N");
					if( (m_treeLibrary.GetItemState(hitem_1, TVIS_EXPANDED)&TVIS_EXPANDED) != 0) cstring += "|Y";
					else cstring += "|N";
					arrayEDA_PcbLibraryTreeStructure.SetAt(item_num-1, cstring);

					hitem_2 = m_treeLibrary.GetChildItem(hitem_1);
					while(hitem_2 != NULL)
					{
						if(hitem_2 == selected_item) nEDA_PcbLibrarySelectedItem = item_num;
						item_num++;

						if( m_treeLibrary.ItemHasChildren(hitem_2) == TRUE)
						{
							//cstring = m_treeLibrary.GetItemText(hitem_2);
							//TRACE2("%s %d\n", cstring, item_num-1);
							cstring = arrayEDA_PcbLibraryTreeStructure.GetAt(item_num-1);
							if(cstring.Find("|Y") >= 0) cstring.TrimRight("|Y");
							else if(cstring.Find("|N") >= 0) cstring.TrimRight("|N");
							if( (m_treeLibrary.GetItemState(hitem_2, TVIS_EXPANDED)&TVIS_EXPANDED) != 0) cstring += "|Y";
							else cstring += "|N";
							arrayEDA_PcbLibraryTreeStructure.SetAt(item_num-1, cstring);
							
							hitem_3 = m_treeLibrary.GetChildItem(hitem_2);
							while(hitem_3 != NULL)
							{
								//cstring = m_treeLibrary.GetItemText(hitem_3);
								//TRACE2("%s %d\n", cstring, item_num);
								if(hitem_3 == selected_item) nEDA_PcbLibrarySelectedItem = item_num;
								item_num++;

								
								hitem_3 = m_treeLibrary.GetNextSiblingItem(hitem_3);
							}
						}
						hitem_2 = m_treeLibrary.GetNextSiblingItem(hitem_2);
					}

				}
				
				hitem_1 = m_treeLibrary.GetNextSiblingItem(hitem_1);
			}
		}
		hitem_0 = m_treeLibrary.GetNextSiblingItem(hitem_0);
	}
}

void CDlg_PcbLib_Browse::OnLbnSelchangeListUserLibrary()
{
	CString userlibrary;
	int sel = m_listboxUserLibrary.GetCurSel();
	if(sel < 0) return;

	m_listboxUserLibrary.GetText(sel, userlibrary);


	int lib_index;
	for(lib_index=0; lib_index<arrayEDA_PcbUserLibraryName.GetCount(); lib_index++)
	{
		if(userlibrary == arrayEDA_PcbUserLibraryName.GetAt(lib_index)) break;
	}


	char libstring[MAX_PATH];
	sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, userlibrary);
	
	CFile cfileUserLibrary;
	if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE) return;
	CArchive ar(&cfileUserLibrary, CArchive::load);


	CString headformat;
	ar >> headformat;
	if(headformat != PCB_Library_Head) {	ar.Close();	cfileUserLibrary.Close();	return; }


	m_listboxCompList.ResetContent(); 
	arrayEDA_PcbLibraryCompList.RemoveAll();

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

		arrayEDA_PcbLibraryCompList.Add(complist);
		int subindex = m_listboxCompList.AddString(cstring);
		m_listboxCompList.SetItemData(subindex, j);
	}
	ar.Close();
	cfileUserLibrary.Close();

	nEDA_PcbCompSelectedItem = 0;
	if( (nEDA_PcbCompSelectedItem >= 0)&&(m_listboxCompList.GetCount() > 0)&&(nEDA_PcbCompSelectedItem < m_listboxCompList.GetCount()) )
	{
		m_listboxCompList.SetCurSel(nEDA_PcbCompSelectedItem);
		
	}

	ShowPackage();
}

void CDlg_PcbLib_Browse::ShowPackage()
{
	double fminx, fminy, fmaxx, fmaxy;
	CTypedPtrArray <CObArray, COb_pcbarc*> arrayPcbArc;
	CTypedPtrArray <CObArray, COb_pcbtrack*> arrayPcbTrack;
	CTypedPtrArray <CObArray, COb_pcbtext*> arrayPcbText;
	CTypedPtrArray <CObArray, COb_pcbfill*> arrayPcbFill;
	CTypedPtrArray <CObArray, COb_pcbregion*> arrayPcbRegion;	
	CTypedPtrArray <CObArray, COb_pcbpad*> arrayPcbPad;
	CTypedPtrArray <CObArray, COb_pcbvia*> arrayPcbVia;
	CString csPackageName, csDescription;

	int sel_index = m_listboxCompList.GetCurSel();
	if(sel_index == LB_ERR) return;

	int i = m_listboxCompList.GetItemData(sel_index);
	if(i < 0) return;

	Struct_CompList complist;
	complist = arrayEDA_PcbLibraryCompList.GetAt(i);

	int offset, length;

	unsigned char* pbuffer;

	if(complist.basiclibrary == TRUE)  
	{
		offset = complist.liboffset + complist.offset;
		length = complist.length;
 
		pbuffer = (unsigned char*)malloc(length + 100); 

		cfileEDA_PcbLibrary.Seek(offset, CFile::begin);
		cfileEDA_PcbLibrary.Read(pbuffer, length);
	}
	else 
	{
		CString userlibrary, compname;
		userlibrary = arrayEDA_PcbUserLibraryName.GetAt(complist.liboffset);

		char libstring[MAX_PATH];
		sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, userlibrary);

		CFile cfileUserLibrary;
		if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE)  return;
		CArchive ar(&cfileUserLibrary, CArchive::load);

		CString headformat;
		ar >> headformat;
		if(headformat != PCB_Library_Head)	{	ar.Close();	cfileUserLibrary.Close();	return;	}
		int totalnum;
		ar >> totalnum;
		ar.Close();

		cfileUserLibrary.Seek(32 + 320*complist.offset, CFile::begin);

		Struct_Comp_Head comp_head;
		cfileUserLibrary.Read(&comp_head, 320);
		offset = comp_head.offset;
		length = comp_head.length;

		pbuffer = (unsigned char*)malloc(length + 100); 

		cfileUserLibrary.Seek(offset, CFile::begin);
		cfileUserLibrary.Read(pbuffer, length);

		cfileUserLibrary.Close();
	}
	
	CMemFile memfile(pbuffer, length);
	CArchive ar(&memfile,CArchive::load);
	
	fminx = fminy = 100000;
	fmaxx = fmaxy = -100000;
	int arcnum, tracknum, fillnum, padnum, vianum, textnum, regionnum;

	ar >> arcnum;
	for(int i=0; i<arcnum; i++)
	{
		COb_pcbarc* parc = new COb_pcbarc();
		parc->Serialize(ar);
		arrayPcbArc.Add(parc);
		if((parc->m_fSangle == parc->m_fEangle)||((parc->m_fEangle - parc->m_fSangle) == 360))
		{
			fminx = MIN(fminx, (parc->m_fCx - parc->m_fRadius));
			fminy = MIN(fminy, (parc->m_fCy - parc->m_fRadius));
			fmaxx = MAX(fmaxx, (parc->m_fCx + parc->m_fRadius));
			fmaxy = MAX(fmaxy, (parc->m_fCy + parc->m_fRadius));
		}
		else if(parc->m_fEangle > parc->m_fSangle)
		{
			if((180 > parc->m_fSangle)&&(180 < parc->m_fEangle)) fminx = MIN(fminx, (parc->m_fCx - parc->m_fRadius));
			else fminx = MIN(fminx, parc->m_fCx);

			if((270 > parc->m_fSangle)&&(270 < parc->m_fEangle)) fminy = MIN(fminy, (parc->m_fCy - parc->m_fRadius));
			else fminy = MIN(fminy, parc->m_fCy);

			fmaxx = MAX(fmaxx, parc->m_fCx);

			if((90 > parc->m_fSangle)&&(90 < parc->m_fEangle)) fmaxy = MAX(fmaxy, (parc->m_fCy + parc->m_fRadius));
			else fmaxy = MAX(fmaxy, parc->m_fCy);
		}
		else
		{
			if((180 > parc->m_fSangle)||(180 < parc->m_fEangle)) fminx = MIN(fminx, (parc->m_fCx - parc->m_fRadius));
			else fminx = MIN(fminx, parc->m_fCx);

			if((270 > parc->m_fSangle)||(270 < parc->m_fEangle)) fminy = MIN(fminy, (parc->m_fCy - parc->m_fRadius));
			else fminy = MIN(fminy, parc->m_fCy);

			fmaxx = MAX(fmaxx, (parc->m_fCx + parc->m_fRadius));

			if((90 > parc->m_fSangle)||(90 < parc->m_fEangle)) fmaxy = MAX(fmaxy, (parc->m_fCy + parc->m_fRadius));
			else fmaxy = MAX(fmaxy, parc->m_fCy);
		}
	}
	ar >> tracknum;
	for(int i=0; i<tracknum; i++)
	{
		COb_pcbtrack* ptrack = new COb_pcbtrack();
		ptrack->Serialize(ar);
		arrayPcbTrack.Add(ptrack);
		fminx = MIN(fminx, ptrack->m_fX1);
		fminy = MIN(fminy, ptrack->m_fY1);
		fmaxx = MAX(fmaxx, ptrack->m_fX2);
		fmaxy = MAX(fmaxy, ptrack->m_fY2);
	}
	ar >> fillnum;
	for(int i=0; i<fillnum; i++)
	{
		COb_pcbfill* pfill = new COb_pcbfill();
		pfill->Serialize(ar);
		arrayPcbFill.Add(pfill);
		fminx = MIN(fminx, pfill->m_fX1);
		fminy = MIN(fminy, pfill->m_fY1);
		fmaxx = MAX(fmaxx, pfill->m_fX2);
		fmaxy = MAX(fmaxy, pfill->m_fY2);
	}
	ar >> padnum;
	for(int i=0; i<padnum; i++)
	{
		COb_pcbpad* ppad = new COb_pcbpad();
		ppad->Serialize(ar);
		arrayPcbPad.Add(ppad);
		fminx = MIN(fminx, ppad->m_fCx - ppad->m_fSizeX[0]);
		fminy = MIN(fminy, ppad->m_fCy - ppad->m_fSizeY[0]);
		fmaxx = MAX(fmaxx, ppad->m_fCx + ppad->m_fSizeX[0]);
		fmaxy = MAX(fmaxy, ppad->m_fCy + ppad->m_fSizeY[0]);
	}
	ar >> vianum;
	for(int i=0; i<vianum; i++)
	{
		COb_pcbvia* pvia = new COb_pcbvia();
		pvia->Serialize(ar);
		arrayPcbVia.Add(pvia);
		fminx = MIN(fminx, pvia->m_fCx - pvia->m_fDia);
		fminy = MIN(fminy, pvia->m_fCy - pvia->m_fDia);
		fmaxx = MAX(fmaxx, pvia->m_fCx + pvia->m_fDia);
		fmaxy = MAX(fmaxy, pvia->m_fCy + pvia->m_fDia);
	}
	ar >> textnum;
	for(int i=0; i<textnum; i++)
	{
		COb_pcbtext* ptext = new COb_pcbtext();
		ptext->Serialize(ar);
		arrayPcbText.Add(ptext);
		fminx = MIN(fminx, ptext->m_fSx);
		fminy = MIN(fminy, ptext->m_fSy);
		fmaxx = MAX(fmaxx, ptext->m_fSx + ptext->m_csText.GetLength()*ptext->m_fHeight);
		fmaxy = MAX(fmaxy, ptext->m_fSy + ptext->m_fHeight);
	}
	ar >> regionnum;
	for(int i=0; i<regionnum; i++)
	{
		COb_pcbregion* pregion = new COb_pcbregion();
		pregion->Serialize(ar);
		arrayPcbRegion.Add(pregion);
		for(int k=0;k <pregion->m_cVertex.GetCount(); k++)
		{
			fminx = MIN(fminx, pregion->m_cVertex[k].fx);
			fminy = MIN(fminy, pregion->m_cVertex[k].fy);
			fmaxx = MAX(fmaxx, pregion->m_cVertex[k].fx);
			fmaxy = MAX(fmaxy, pregion->m_cVertex[k].fy);
		}
	}

	ar.Close();
	memfile.Close();
	free(pbuffer);

	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PACKAGE_PREVIEW);
	RECT rect;
	pWnd->GetClientRect(&rect);
	
	rect.left++;
	rect.right--;
	rect.top++;
	rect.bottom--;
	
	CClientDC dc(pWnd);
	dc.FillSolidRect( rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, nEDA_PCB_BoardAreaColor);
	dc.IntersectClipRect(&rect);
	CSize tsize = dc.GetTextExtent(complist.compname);
	int x = (rect.right - tsize.cx)/2 + 1;
	dc.SetTextColor(nEDA_PCB_LayerColor[TOPOVERLAY]);
	dc.TextOut(x,2, complist.compname);

	int nEx = rect.right-rect.left;
	int nEy = rect.bottom-rect.top;

	double scale1 = (fmaxx - fminx)*1.2/nEx;
	double scale2 = (fmaxy - fminy)*1.2/nEy;
	double scale;
	if(scale1 > scale2) scale = scale1;
	else scale = scale2;

	double origin_x = fminx - (nEx - (fmaxx - fminx)/scale)/2*scale;
	double origin_y = fminy - (nEy - (fmaxy - fminy)/scale)/2*scale;

	for(int i=0; i<arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = arrayPcbArc.GetAt(i);
		parc->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
	}
	for(int i=0; i<arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = arrayPcbTrack.GetAt(i);
		ptrack->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
	}	
	for(int i=0; i<arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = arrayPcbPad.GetAt(i);
		ppad->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
	}
	for(int i=0; i<arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = arrayPcbVia.GetAt(i);
		pvia->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
	}
	for(int i=0; i<arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = arrayPcbFill.GetAt(i);
		pfill->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
	}
	for(int i=0; i<arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = arrayPcbText.GetAt(i);
		ptext->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
	}
	for(int i=0; i<arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = arrayPcbRegion.GetAt(i);
		pregion->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
	}

	while (!arrayPcbArc.IsEmpty())
	{
		delete arrayPcbArc.GetAt(0);
        arrayPcbArc.RemoveAt(0);
	}
	while (!arrayPcbTrack.IsEmpty())
	{
		delete arrayPcbTrack.GetAt(0);
        arrayPcbTrack.RemoveAt(0);
	}
	while (!arrayPcbPad.IsEmpty())
	{
		delete arrayPcbPad.GetAt(0);
        arrayPcbPad.RemoveAt(0);
	}
	while (!arrayPcbVia.IsEmpty())
	{
		delete arrayPcbVia.GetAt(0);
        arrayPcbVia.RemoveAt(0);
	}
	while (!arrayPcbText.IsEmpty())
	{
		delete arrayPcbText.GetAt(0);
        arrayPcbText.RemoveAt(0);
	}
	while (!arrayPcbFill.IsEmpty())
	{
		delete arrayPcbFill.GetAt(0);
        arrayPcbFill.RemoveAt(0);
	}
	while (!arrayPcbRegion.IsEmpty())
	{
		delete arrayPcbRegion.GetAt(0);
        arrayPcbRegion.RemoveAt(0);
	}
}

BOOL CDlg_PcbLib_Browse::PreTranslateMessage(MSG* pMsg)
{
	if( (pMsg->message == WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN) )
    {
		OnBnClickedButtonSearch();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
