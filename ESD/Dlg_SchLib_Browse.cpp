// Dlg_SchLib_Browse.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"

#include "Dlg_SchLib_Browse.h"
#include ".\dlg_schlib_browse.h"
#include "systemext.h"


// CDlg_SchLib_Browse 对话框

IMPLEMENT_DYNAMIC(CDlg_SchLib_Browse, CDialog)
CDlg_SchLib_Browse::CDlg_SchLib_Browse(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_SchLib_Browse::IDD, pParent)
	, m_bMatchMore(FALSE)
{
	first_selection = TRUE;
	first_show = TRUE;

	m_CompList.liboffset = -1;
	m_CompList.offset = -1;
}

CDlg_SchLib_Browse::~CDlg_SchLib_Browse()
{
}

void CDlg_SchLib_Browse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COMP, m_listboxCompList);
	DDX_Control(pDX, IDC_TREE_LIBRARY, m_treeLibrary);
	DDX_Control(pDX, IDC_LIST_USER_LIBRARY, m_listboxUserLibrary);
	DDX_Check(pDX, IDC_CHECK_MATCH, m_bMatchMore);
}


BEGIN_MESSAGE_MAP(CDlg_SchLib_Browse, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST_COMP, OnLbnSelchangeListComp)
	ON_LBN_DBLCLK(IDC_LIST_COMP, OnLbnDblclkListComp)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_PART, OnCbnSelchangeComboPart)
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_LIBRARY, OnTvnSelchangedTreeLibrary)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_CHECK_MATCH, OnBnClickedCheckMatch)
	ON_LBN_SELCHANGE(IDC_LIST_USER_LIBRARY, OnLbnSelchangeListUserLibrary)
	ON_CBN_SELCHANGE(IDC_COMBO_PACKAGE, OnCbnSelchangeComboPackage)
END_MESSAGE_MAP()


// CDlg_SchLib_Browse 消息处理程序
BOOL CDlg_SchLib_Browse::OnInitDialog()
{
	CDialog::OnInitDialog();

	HTREEITEM hitem[10];
	char s[260], sbak[260], itemfile[200];
	int  n = 200;

	int lib_index;
	for(int k=0; k<arrayEDA_SchLibraryTreeStructure.GetCount(); k++)
	{
		CString cstring = arrayEDA_SchLibraryTreeStructure.GetAt(k);
		strcpy(s, cstring);

		strcpy(sbak, s);

		int j;
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
		
	HTREEITEM hitem_0, hitem_1, hitem_2, hitem_3, hitem_4;
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
		if(nEDA_SchLibrarySelectedItem == item_num)
		{
			BOOL b = m_treeLibrary.Select(hitem_0,TVGN_CARET   );//TVGN_FIRSTVISIBLE);
			//BOOL b = m_treeLibrary.SelectSetFirstVisible(hitem_0);
			//BOOL b = m_treeLibrary.SelectItem(hitem_0);
			//if(b == TRUE) TRACE1("TRUE %d\n", item_num);
			//else TRACE1("FALSE %d\n", item_num);
		}
		item_num++;
		if( m_treeLibrary.ItemHasChildren(hitem_0) == TRUE)
		{
			hitem_1 = m_treeLibrary.GetChildItem(hitem_0);
			while(hitem_1 != NULL)
			{
				if(nEDA_SchLibrarySelectedItem == item_num) 
				{
					BOOL b = m_treeLibrary.Select(hitem_1,TVGN_CARET   );//TVGN_FIRSTVISIBLE);
					//BOOL b = m_treeLibrary.SelectSetFirstVisible(hitem_1);
					//BOOL b = m_treeLibrary.SelectItem(hitem_1);
					//if(b == TRUE) TRACE1("TRUE %d\n", item_num);
					//else TRACE1("FALSE %d\n", item_num);
				}
				item_num++;
				if( m_treeLibrary.ItemHasChildren(hitem_1) == TRUE)
				{
					hitem_2 = m_treeLibrary.GetChildItem(hitem_1);
					while(hitem_2 != NULL)
					{
						if(nEDA_SchLibrarySelectedItem == item_num) 
						{
							BOOL b = m_treeLibrary.Select(hitem_2,TVGN_CARET   );//TVGN_FIRSTVISIBLE);
							//BOOL b = m_treeLibrary.SelectSetFirstVisible(hitem_2);
							//BOOL b = m_treeLibrary.SelectItem(hitem_2);
							//if(b == TRUE) TRACE1("sdTRUE %d\n", item_num);
							//else TRACE1("FALSE %d\n", item_num);
						}
						item_num++;
						if( m_treeLibrary.ItemHasChildren(hitem_2) == TRUE)
						{
							hitem_3 = m_treeLibrary.GetChildItem(hitem_2);
							while(hitem_3 != NULL)
							{
								if(nEDA_SchLibrarySelectedItem == item_num)
								{
									BOOL b = m_treeLibrary.Select(hitem_3,TVGN_CARET   );//TVGN_FIRSTVISIBLE);
									//BOOL b = m_treeLibrary.SelectSetFirstVisible(hitem_3);
									//BOOL b = m_treeLibrary.SelectItem(hitem_3);
									//if(b == TRUE) TRACE1("TRUE %d\n", item_num);
									//else TRACE1("FALSE %d\n", item_num);
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
	
	if(arrayEDA_SchLibraryCompList.GetCount() != 0)
	{
		for(int i=0; i<arrayEDA_SchLibraryCompList.GetCount(); i++)
		{
			Struct_CompList complist;
			complist = arrayEDA_SchLibraryCompList.GetAt(i);
			int index = m_listboxCompList.AddString(complist.compname);
			m_listboxCompList.SetItemData(index, i);
		}
	}
		
	if( (nEDA_SchCompSelectedItem >= 0)&&(m_listboxCompList.GetCount() > 0)&&(nEDA_SchCompSelectedItem < m_listboxCompList.GetCount()) )
	{
		m_listboxCompList.SetCurSel(nEDA_SchCompSelectedItem);
	}
	m_listboxCompList.SetColumnWidth(210);

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_EDIT_SEARCH);
	pComboBox->SetWindowText(csEDA_SchSearchCompName);

	int libnum = arrayEDA_SchUserLibraryName.GetCount();
	for(int i=0; i<libnum; i++)
	{
		m_listboxUserLibrary.AddString(arrayEDA_SchUserLibraryName.GetAt(i));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_SchLib_Browse::OnTvnSelchangedTreeLibrary(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	if(first_selection == TRUE)
	{
		first_selection = FALSE;
	}
	else
	{
		HTREEITEM hitem = m_treeLibrary.GetSelectedItem( );
		int index = m_treeLibrary.GetItemData(hitem);
		if(index < 0) return; 

		m_listboxCompList.ResetContent(); 
		arrayEDA_SchLibraryCompList.RemoveAll();

		for(int i=0; i<arrayEDA_SchLibraryCompName[index].GetCount(); i++)
		{
			Struct_CompList complist;
			complist = arrayEDA_SchLibraryCompName[index].GetAt(i);
			arrayEDA_SchLibraryCompList.Add(complist);
			int subindex = m_listboxCompList.AddString(complist.compname);
			m_listboxCompList.SetItemData(subindex, i);
		}
		
		nEDA_SchCompSelectedItem = 0;
		if( (nEDA_SchCompSelectedItem >= 0)&&(m_listboxCompList.GetCount() > 0)&&(nEDA_SchCompSelectedItem < m_listboxCompList.GetCount()) )
		{
			m_listboxCompList.SetCurSel(nEDA_SchCompSelectedItem);
		}
		
		SetPartAndPackage();

		ShowComp();
		ShowPackage();
	}

	*pResult = 0;
}

void CDlg_SchLib_Browse::OnLbnSelchangeListComp()
{
	nEDA_SchCompSelectedItem = m_listboxCompList.GetCurSel();
	
	SetPartAndPackage();

	ShowComp();
	ShowPackage();
}

void CDlg_SchLib_Browse::OnLbnDblclkListComp()
{
	nEDA_SchCompSelectedItem = m_listboxCompList.GetCurSel();
	int sel_index = m_listboxCompList.GetCurSel();
	if(sel_index != LB_ERR)
	{
		int i = m_listboxCompList.GetItemData(sel_index);
		if(i >= 0)
		{
			m_CompList = arrayEDA_SchLibraryCompList.GetAt(i);
		}
	}

	CDialog::OnOK();
}

void CDlg_SchLib_Browse::OnOK()
{
	nEDA_SchCompSelectedItem = m_listboxCompList.GetCurSel();
	int sel_index = m_listboxCompList.GetCurSel();
	if(sel_index != LB_ERR)
	{
		int i = m_listboxCompList.GetItemData(sel_index);
		if(i >= 0)
		{
			m_CompList = arrayEDA_SchLibraryCompList.GetAt(i);
		}
	}

	CDialog::OnOK();
}

void CDlg_SchLib_Browse::ShowComp()
{
	double fminx, fminy, fmaxx, fmaxy;
	CTypedPtrList <CObList, CObject*> listCompElement;
	CArray <LOGFONT, LOGFONT&>  arrayFont;

	int sel_index = m_listboxCompList.GetCurSel();
	if(sel_index == LB_ERR) return;

	int i = m_listboxCompList.GetItemData(sel_index);
	if(i < 0) return;

	Struct_CompList complist;
	complist = arrayEDA_SchLibraryCompList.GetAt(i);

	int offset, length;
	unsigned char* pbuffer = (unsigned char*)malloc(1000000); //new unsigned char[250000];

	if(complist.basiclibrary == TRUE)  
	{
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
		if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE)  return;
		CArchive ar(&cfileUserLibrary, CArchive::load);

		CString headformat;
		ar >> headformat;
		if(headformat != SCH_Library_Head)	{	ar.Close();	cfileUserLibrary.Close();	return;	}
		int totalnum;
		ar >> totalnum;
		ar.Close();

		cfileUserLibrary.Seek(32 + 320*complist.offset, CFile::begin);

		Struct_Comp_Head comp_head;
		cfileUserLibrary.Read(&comp_head, 320);
		offset = comp_head.offset;
		length = comp_head.length;

		cfileUserLibrary.Seek(offset, CFile::begin);
		cfileUserLibrary.Read(pbuffer, length);

		cfileUserLibrary.Close();
	}
	
	CMemFile memfile(pbuffer, length);
	CArchive ar(&memfile,CArchive::load);
	
	COb_libcomphead comphead;
	comphead.Serialize(ar);

	int element_id;
	fminx = fminy = 100000;
	fmaxx = fmaxy = -100000;
	for(;;)
	{
		ar >> element_id;
		if(element_id <= 0) break;
		if(element_id == SCH_ELEMENT_PIN)
		{
			COb_schpin* ppin = new COb_schpin();
			ppin->Serialize(ar);
			listCompElement.AddTail(ppin);
			
			if((comphead.m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID == m_nComboPartNo))
			{
				if(ppin->m_bHide == FALSE)
				{
					if(ppin->m_nOrientation == 0)
					{
						fminx = MIN(fminx, ppin->m_fX);
						fminy = MIN(fminy, ppin->m_fY);
						fmaxx = MAX(fmaxx, ppin->m_fX + ppin->m_fPinLength);
						fmaxy = MAX(fmaxy, ppin->m_fY);
					}
					else if(ppin->m_nOrientation == 90)
					{
						fminx = MIN(fminx, ppin->m_fX);
						fminy = MIN(fminy, ppin->m_fY);
						fmaxx = MAX(fmaxx, ppin->m_fX);
						fmaxy = MAX(fmaxy, ppin->m_fY + ppin->m_fPinLength);
					}
					else if(ppin->m_nOrientation == 180)
					{
						fminx = MIN(fminx, ppin->m_fX - ppin->m_fPinLength);
						fminy = MIN(fminy, ppin->m_fY);
						fmaxx = MAX(fmaxx, ppin->m_fX);
						fmaxy = MAX(fmaxy, ppin->m_fY);
					}
					else
					{
						fminx = MIN(fminx, ppin->m_fX);
						fminy = MIN(fminy, ppin->m_fY - ppin->m_fPinLength);
						fmaxx = MAX(fmaxx, ppin->m_fX);
						fmaxy = MAX(fmaxy, ppin->m_fY);
					}
				}
			}
		}
		else if(element_id == SCH_ELEMENT_RECT)
		{
			COb_schrect* prect = new COb_schrect();
			prect->Serialize(ar);
			//listCompElement.AddTail(prect);
			listCompElement.AddHead(prect);
			
			if((comphead.m_nPartCount <= 1)||(prect->m_nOwnerPartID == 0)||(prect->m_nOwnerPartID == m_nComboPartNo))
			{
				fminx = MIN(fminx, prect->m_fX1);
				fminy = MIN(fminy, prect->m_fY1);
				fmaxx = MAX(fmaxx, prect->m_fX2);
				fmaxy = MAX(fmaxy, prect->m_fY2);
			}
		}	
		else if(element_id == SCH_ELEMENT_POLYLINE)
		{
			COb_schpolyline* ppolyline = new COb_schpolyline();
			ppolyline->Serialize(ar);
			listCompElement.AddTail(ppolyline);
			
			if((comphead.m_nPartCount <= 1)||(ppolyline->m_nOwnerPartID == 0)||(ppolyline->m_nOwnerPartID == m_nComboPartNo))
			{			
				for(int k=0;k <ppolyline->m_cVertex.GetCount(); k++)
				{
					fminx = MIN(fminx, ppolyline->m_cVertex[k].fx);
					fminy = MIN(fminy, ppolyline->m_cVertex[k].fy);
					fmaxx = MAX(fmaxx, ppolyline->m_cVertex[k].fx);
					fmaxy = MAX(fmaxy, ppolyline->m_cVertex[k].fy);
				}
			}
		}
		else if(element_id == SCH_ELEMENT_ARC)
		{
			COb_scharc* parc = new COb_scharc();
			parc->Serialize(ar);
			listCompElement.AddTail(parc);
			
			if((comphead.m_nPartCount <= 1)||(parc->m_nOwnerPartID == 0)||(parc->m_nOwnerPartID == m_nComboPartNo))
			{
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
		}
		else if(element_id == SCH_ELEMENT_PIE)
		{
			COb_schpie* ppie = new COb_schpie();
			ppie->Serialize(ar);
			listCompElement.AddTail(ppie);
		}
		else if(element_id == SCH_ELEMENT_ELLIPSE)
		{
			COb_schellipse* pellipse = new COb_schellipse();
			pellipse->Serialize(ar);
			listCompElement.AddTail(pellipse);
			if((comphead.m_nPartCount <= 1)||(pellipse->m_nOwnerPartID == 0)||(pellipse->m_nOwnerPartID == m_nComboPartNo))
			{
				if((pellipse->m_fSangle == pellipse->m_fEangle)||((pellipse->m_fEangle - pellipse->m_fSangle) == 360))
				{
					fminx = MIN(fminx, (pellipse->m_fCx - pellipse->m_fRadius_x));
					fminy = MIN(fminy, (pellipse->m_fCy - pellipse->m_fRadius_y));
					fmaxx = MAX(fmaxx, (pellipse->m_fCx + pellipse->m_fRadius_x));
					fmaxy = MAX(fmaxy, (pellipse->m_fCy + pellipse->m_fRadius_y));
				}
				else if(pellipse->m_fEangle > pellipse->m_fSangle)
				{
					if((180 > pellipse->m_fSangle)&&(180 < pellipse->m_fEangle)) fminx = MIN(fminx, (pellipse->m_fCx - pellipse->m_fRadius_x));
					else fminx = MIN(fminx, pellipse->m_fCx);

					if((270 > pellipse->m_fSangle)&&(270 < pellipse->m_fEangle)) fminy = MIN(fminy, (pellipse->m_fCy - pellipse->m_fRadius_y));
					else fminy = MIN(fminy, pellipse->m_fCy);

					fmaxx = MAX(fmaxx, pellipse->m_fCx);

					if((90 > pellipse->m_fSangle)&&(90 < pellipse->m_fEangle)) fmaxy = MAX(fmaxy, (pellipse->m_fCy + pellipse->m_fRadius_y));
					else fmaxy = MAX(fmaxy, pellipse->m_fCy);
				}
				else
				{
					if((180 > pellipse->m_fSangle)||(180 < pellipse->m_fEangle)) fminx = MIN(fminx, (pellipse->m_fCx - pellipse->m_fRadius_x));
					else fminx = MIN(fminx, pellipse->m_fCx);

					if((270 > pellipse->m_fSangle)||(270 < pellipse->m_fEangle)) fminy = MIN(fminy, (pellipse->m_fCy - pellipse->m_fRadius_y));
					else fminy = MIN(fminy, pellipse->m_fCy);

					fmaxx = MAX(fmaxx, (pellipse->m_fCx + pellipse->m_fRadius_x));

					if((90 > pellipse->m_fSangle)||(90 < pellipse->m_fEangle)) fmaxy = MAX(fmaxy, (pellipse->m_fCy + pellipse->m_fRadius_y));
					else fmaxy = MAX(fmaxy, pellipse->m_fCy);
				}
			}
		}
		else if(element_id == SCH_ELEMENT_ELLIPSEFILL)
		{
			COb_schellipsefill* pellipsefill = new COb_schellipsefill();
			pellipsefill->Serialize(ar);
			listCompElement.AddTail(pellipsefill);
			
			if((comphead.m_nPartCount <= 1)||(pellipsefill->m_nOwnerPartID == 0)||(pellipsefill->m_nOwnerPartID == m_nComboPartNo))
			{			
				fminx = MIN(fminx, pellipsefill->m_fCx - pellipsefill->m_fRadius_x);
				fminy = MIN(fminy, pellipsefill->m_fCy - pellipsefill->m_fRadius_y);
				fmaxx = MAX(fmaxx, pellipsefill->m_fCx + pellipsefill->m_fRadius_x);
				fmaxy = MAX(fmaxy, pellipsefill->m_fCy + pellipsefill->m_fRadius_y);
			}
		}
		else if(element_id == SCH_ELEMENT_POLYGON)
		{
			COb_schpolygon* ppolygon = new COb_schpolygon();
			ppolygon->Serialize(ar);
			listCompElement.AddTail(ppolygon);
			
			if((comphead.m_nPartCount <= 1)||(ppolygon->m_nOwnerPartID == 0)||(ppolygon->m_nOwnerPartID == m_nComboPartNo))
			{
				for(int k=0;k <ppolygon->m_cVertex.GetCount(); k++)
				{
					fminx = MIN(fminx, ppolygon->m_cVertex[k].fx);
					fminy = MIN(fminy, ppolygon->m_cVertex[k].fy);
					fmaxx = MAX(fmaxx, ppolygon->m_cVertex[k].fx);
					fmaxy = MAX(fmaxy, ppolygon->m_cVertex[k].fy);
				}
			}
		}
		else if(element_id == SCH_ELEMENT_TEXT)
		{
			COb_schtext* ptext = new COb_schtext();
			ptext->Serialize(ar);
			listCompElement.AddTail(ptext);
		}
		else if(element_id == SCH_ELEMENT_BEZIER)
		{
			COb_schbezier* pbezier = new COb_schbezier();
			pbezier->Serialize(ar);
			listCompElement.AddTail(pbezier);
		}
		else if(element_id == SCH_ELEMENT_ROUNDRECT)
		{
			COb_schroundrect* proundrect = new COb_schroundrect();
			proundrect->Serialize(ar);
			listCompElement.AddTail(proundrect);
			if((comphead.m_nPartCount <= 1)||(proundrect->m_nOwnerPartID == 0)||(proundrect->m_nOwnerPartID == m_nComboPartNo))
			{
				fminx = MIN(fminx, proundrect->m_fX1);
				fminy = MIN(fminy, proundrect->m_fY1);
				fmaxx = MAX(fmaxx, proundrect->m_fX2);
				fmaxy = MAX(fmaxy, proundrect->m_fY2);
			}
		}
		else if(element_id == SCH_ELEMENT_IEEE)
		{
			COb_schieee* pieee = new COb_schieee();
			pieee->Serialize(ar);
			listCompElement.AddTail(pieee);
		}
		else if(element_id == SCH_ELEMENT_LINE)
		{
			AfxMessageBox("Error! unexpected lib element");
		}
	}

	
	ar.Close();
	memfile.Close();
	free(pbuffer);
	
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_COMP_PREVIEW);	
	RECT rect;
	pWnd->GetClientRect(&rect);
	
	rect.left++;
	rect.right--;
	rect.top++;
	rect.bottom--;
	
	CClientDC dc(pWnd);
	//dc.FillSolidRect( rect.left+1, rect.top+1, rect.right-rect.left-2, rect.bottom-rect.top-2, RGB(254,251,218));
	dc.FillSolidRect( rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, RGB(254,251,218));
	dc.IntersectClipRect(&rect);
	CSize tsize = dc.GetTextExtent(comphead.m_arrayCompName.GetAt(0));
	int x = (rect.right - tsize.cx)/2 + 1;
	dc.TextOut(x,2,comphead.m_arrayCompName.GetAt(0));

	int nEx = rect.right-rect.left;
	int nEy = rect.bottom-rect.top;

	double scale1 = (fmaxx - fminx)*1.2/nEx;
	double scale2 = (fmaxy - fminy)*1.2/nEy;
	double scale;
	if(scale1 > scale2) scale = scale1;
	else scale = scale2;

	double origin_x = fminx - (nEx - (fmaxx - fminx)/scale)/2*scale;
	double origin_y = fminy - (nEy - (fmaxy - fminy)/scale)/2*scale;

	CObject* pobject;
	int num = listCompElement.GetCount();
	POSITION pos;
	pos = listCompElement.GetHeadPosition();
	for(int i=0; i<num; i++)
	{
		if(pos == NULL) break;
		pobject = listCompElement.GetNext(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
		{
			COb_schpin* ppin = (COb_schpin*)pobject;
			if((comphead.m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID == m_nComboPartNo))
			{
				ppin->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect = (COb_schrect*)pobject;
			if((comphead.m_nPartCount <= 1)||(prect->m_nOwnerPartID == 0)||(prect->m_nOwnerPartID == m_nComboPartNo))
			{
				prect->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((comphead.m_nPartCount <= 1)||(ppolyline->m_nOwnerPartID == 0)||(ppolyline->m_nOwnerPartID == m_nComboPartNo))
			{
				ppolyline->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc = (COb_scharc*)pobject;
			if((comphead.m_nPartCount <= 1)||(parc->m_nOwnerPartID == 0)||(parc->m_nOwnerPartID == m_nComboPartNo))
			{
				parc->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie = (COb_schpie*)pobject;
			if((comphead.m_nPartCount <= 1)||(ppie->m_nOwnerPartID == 0)||(ppie->m_nOwnerPartID == m_nComboPartNo))
			{
				ppie->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse = (COb_schellipse*)pobject;
			if((comphead.m_nPartCount <= 1)||(pellipse->m_nOwnerPartID == 0)||(pellipse->m_nOwnerPartID == m_nComboPartNo))
			{
				pellipse->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
			if((comphead.m_nPartCount <= 1)||(pellipsefill->m_nOwnerPartID == 0)||(pellipsefill->m_nOwnerPartID == m_nComboPartNo))
			{
				pellipsefill->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
			if((comphead.m_nPartCount <= 1)||(ppolygon->m_nOwnerPartID == 0)||(ppolygon->m_nOwnerPartID == m_nComboPartNo))
			{
				ppolygon->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext = (COb_schtext*)pobject;
			if((comphead.m_nPartCount <= 1)||(ptext->m_nOwnerPartID == 0)||(ptext->m_nOwnerPartID == m_nComboPartNo))
			{
				if(ptext->m_nID == TEXT_TEXT) ptext->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier = (COb_schbezier*)pobject;
			if((comphead.m_nPartCount <= 1)||(pbezier->m_nOwnerPartID == 0)||(pbezier->m_nOwnerPartID == m_nComboPartNo))
			{
				pbezier->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
			if((comphead.m_nPartCount <= 1)||(proundrect->m_nOwnerPartID == 0)||(proundrect->m_nOwnerPartID == m_nComboPartNo))
			{
				proundrect->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
		{
			COb_schieee* pieee = (COb_schieee*)pobject;
			if((comphead.m_nPartCount <= 1)||(pieee->m_nOwnerPartID == 0)||(pieee->m_nOwnerPartID == m_nComboPartNo))
			{
				pieee->DrawInWindow(&dc, scale, origin_x, origin_y, nEy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			AfxMessageBox("error! unexpected lib element");
			//COb_schline* pline = (COb_schline*)pobject;
			//delete pline;
		}
	}


	pos = listCompElement.GetHeadPosition();
	for(int i=0; i<num; i++)
	{
		if(pos == NULL) break;
		pobject = listCompElement.GetNext(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
		{
			COb_schpin* ppin = (COb_schpin*)pobject;
			delete ppin;

		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect = (COb_schrect*)pobject;
			delete prect;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			delete ppolyline;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc = (COb_scharc*)pobject;
			delete parc;
		}

		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie = (COb_schpie*)pobject;
			delete ppie;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse = (COb_schellipse*)pobject;
			delete pellipse;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
			delete pellipsefill;
		}

		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
			delete ppolygon;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext = (COb_schtext*)pobject;
			delete ptext;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier = (COb_schbezier*)pobject;
			delete pbezier;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
			delete proundrect;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
		{
			COb_schieee* pieee = (COb_schieee*)pobject;
			delete pieee;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			AfxMessageBox("error! unexpected lib element");
			//COb_schline* pline = (COb_schline*)pobject;
			//delete pline;
		}
	}
}

void CDlg_SchLib_Browse::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(first_show == TRUE)
	{
		first_show = FALSE;
		SetPartAndPackage();
		ShowComp();
		ShowPackage();
	}
}

void  CDlg_SchLib_Browse::SetPartAndPackage()
{
	int sel_index = m_listboxCompList.GetCurSel();
	if(sel_index == LB_ERR) return;

	int i = m_listboxCompList.GetItemData(sel_index);
	if(i < 0) return;

	Struct_CompList complist;
	complist = arrayEDA_SchLibraryCompList.GetAt(i);

	int offset, length;
	unsigned char* pbuffer = (unsigned char*)malloc(1000000); 

	if(complist.basiclibrary == TRUE) 
	{
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

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PART);
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
	m_nComboPartNo = 1;
	
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PACKAGE);
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

	ar.Close();
	memfile.Close();
	free(pbuffer);
}

void CDlg_SchLib_Browse::OnBnClickedButtonSearch()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_EDIT_SEARCH);
	pComboBox->GetWindowText(csEDA_SchSearchCompName);

	csEDA_SchSearchCompName.MakeUpper();
	
	if(csEDA_SchSearchCompName.IsEmpty() != TRUE)
	{
		arrayEDA_SchLibraryCompList.RemoveAll();
		m_listboxCompList.ResetContent();

		for(int i=0; i<SCH_SYSTEM_LIB_NUMBER; i++)
		{
			for(int j=0; j<arrayEDA_SchLibraryCompName[i].GetCount(); j++)
			{
				Struct_CompList complist;
				complist = arrayEDA_SchLibraryCompName[i].GetAt(j);
				CString cstring = complist.compname;
				cstring.MakeUpper();
				if(((m_bMatchMore == TRUE)&&(cstring.Find(csEDA_SchSearchCompName) >= 0))||(cstring == csEDA_SchSearchCompName))
				{
					arrayEDA_SchLibraryCompList.Add(complist);
					if(arrayEDA_SchLibraryCompList.GetCount() > nEDA_SchCompMaxSearchNumber)
					{
						AfxMessageBox("太多的相似元件, 查找终止, 定义更详细的名称并重新查找。");
						i=10000;
						break;
					}
				}
			}
		}

		for(int i=0; i<arrayEDA_SchUserLibraryName.GetCount(); i++)
		{
			CString userlibrary = arrayEDA_SchUserLibraryName.GetAt(i);

			int lib_index = i;

			char libstring[MAX_PATH];
			sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, userlibrary);
			
			CFile cfileUserLibrary;
			if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE)  continue;

			CArchive ar(&cfileUserLibrary, CArchive::load);

			CString headformat;
			ar >> headformat;
			if(headformat != SCH_Library_Head)	{	ar.Close();	cfileUserLibrary.Close(); continue;	}

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

				cstring.MakeUpper();
				if(((m_bMatchMore == TRUE)&&(cstring.Find(csEDA_SchSearchCompName) >= 0))||(cstring == csEDA_SchSearchCompName))
				{
					arrayEDA_SchLibraryCompList.Add(complist);
					if(arrayEDA_SchLibraryCompList.GetCount() > nEDA_SchCompMaxSearchNumber)
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

		if(arrayEDA_SchLibraryCompList.GetCount() != 0)
		{
			
			for(int i=0; i<arrayEDA_SchLibraryCompList.GetCount(); i++)
			{
				Struct_CompList complist;
				complist = arrayEDA_SchLibraryCompList.GetAt(i);
				int index = m_listboxCompList.AddString(complist.compname);
				m_listboxCompList.SetItemData(index, i);
			}
		}
		else
		{
			if(m_bMatchMore == TRUE) AfxMessageBox("未找到相同或相似名称的元件.");
			else AfxMessageBox("未找到同名元件.");
		}
		
		nEDA_SchCompSelectedItem = 0;
		if( (nEDA_SchCompSelectedItem >= 0)&&(m_listboxCompList.GetCount() > 0)&&(nEDA_SchCompSelectedItem < m_listboxCompList.GetCount()) )
		{
			m_listboxCompList.SetCurSel(nEDA_SchCompSelectedItem);
			//m_listboxCompList.GetText(nEDA_SchCompSelectedItem, m_csSelectedComponent);
			SetPartAndPackage();
			ShowComp();
			ShowPackage();
		}
	}
}

void CDlg_SchLib_Browse::OnCbnSelchangeComboPackage()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PACKAGE);
	pComboBox->GetWindowText(m_csComboPackage);

	ShowPackage();
}

void CDlg_SchLib_Browse::OnCbnSelchangeComboPart()
{
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_PART);
	int index = pComboBox->GetCurSel();
	m_nComboPartNo = index + 1;
	ShowComp();
}

void CDlg_SchLib_Browse::OnBnClickedCheckMatch()
{
	UpdateData(TRUE);
}

void CDlg_SchLib_Browse::OnDestroy()
{
	CDialog::OnDestroy();

	HTREEITEM hitem_0, hitem_1, hitem_2, hitem_3, hitem_4;
	
	int item_num = 0;
	CString cstring;

	HTREEITEM selected_item;
	selected_item = m_treeLibrary.GetSelectedItem();

	hitem_0 = m_treeLibrary.GetRootItem( );
	while (hitem_0 != NULL)
	{
		if(hitem_0 == selected_item) nEDA_SchLibrarySelectedItem = item_num;
		item_num++;

		if( m_treeLibrary.ItemHasChildren(hitem_0) == TRUE)
		{
			cstring = arrayEDA_SchLibraryTreeStructure.GetAt(item_num-1);
			if(cstring.Find("|Y") >= 0) cstring.TrimRight("|Y");
			else if(cstring.Find("|N") >= 0) cstring.TrimRight("|N");
			if( (m_treeLibrary.GetItemState(hitem_0, TVIS_EXPANDED)&TVIS_EXPANDED) != 0) cstring += "|Y";
			else cstring += "|N";
			arrayEDA_SchLibraryTreeStructure.SetAt(item_num-1, cstring);


			hitem_1 = m_treeLibrary.GetChildItem(hitem_0);
			while(hitem_1 != NULL)
			{
				if(hitem_1 == selected_item) nEDA_SchLibrarySelectedItem = item_num;
				item_num++;

				if( m_treeLibrary.ItemHasChildren(hitem_1) == TRUE)
				{
					cstring = arrayEDA_SchLibraryTreeStructure.GetAt(item_num-1);
					if(cstring.Find("|Y") >= 0) cstring.TrimRight("|Y");
					else if(cstring.Find("|N") >= 0) cstring.TrimRight("|N");
					if( (m_treeLibrary.GetItemState(hitem_1, TVIS_EXPANDED)&TVIS_EXPANDED) != 0) cstring += "|Y";
					else cstring += "|N";
					arrayEDA_SchLibraryTreeStructure.SetAt(item_num-1, cstring);

					hitem_2 = m_treeLibrary.GetChildItem(hitem_1);
					while(hitem_2 != NULL)
					{
						if(hitem_2 == selected_item) nEDA_SchLibrarySelectedItem = item_num;
						item_num++;

						if( m_treeLibrary.ItemHasChildren(hitem_2) == TRUE)
						{
							cstring = arrayEDA_SchLibraryTreeStructure.GetAt(item_num-1);
							if(cstring.Find("|Y") >= 0) cstring.TrimRight("|Y");
							else if(cstring.Find("|N") >= 0) cstring.TrimRight("|N");
							if( (m_treeLibrary.GetItemState(hitem_2, TVIS_EXPANDED)&TVIS_EXPANDED) != 0) cstring += "|Y";
							else cstring += "|N";
							arrayEDA_SchLibraryTreeStructure.SetAt(item_num-1, cstring);
							
							hitem_3 = m_treeLibrary.GetChildItem(hitem_2);
							while(hitem_3 != NULL)
							{
								if(hitem_3 == selected_item) nEDA_SchLibrarySelectedItem = item_num;
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

void CDlg_SchLib_Browse::OnLbnSelchangeListUserLibrary()
{
	CString userlibrary;
	int sel = m_listboxUserLibrary.GetCurSel();
	if(sel < 0) return;

	m_listboxUserLibrary.GetText(sel, userlibrary);

	int lib_index;
	for(lib_index=0; lib_index<arrayEDA_SchUserLibraryName.GetCount(); lib_index++)
	{
		if(userlibrary == arrayEDA_SchUserLibraryName.GetAt(lib_index)) break;
	}

	char libstring[MAX_PATH];
	sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, userlibrary);
	
	CFile cfileUserLibrary;
	if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE) return;
	CArchive ar(&cfileUserLibrary, CArchive::load);

	CString headformat;
	ar >> headformat;
	if(headformat != SCH_Library_Head) {	ar.Close();	cfileUserLibrary.Close();	return; }

	m_listboxCompList.ResetContent(); 
	arrayEDA_SchLibraryCompList.RemoveAll();

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

		arrayEDA_SchLibraryCompList.Add(complist);
		int subindex = m_listboxCompList.AddString(cstring);
		m_listboxCompList.SetItemData(subindex, j);
	}
	ar.Close();
	cfileUserLibrary.Close();

	nEDA_SchCompSelectedItem = 0;
	if( (nEDA_SchCompSelectedItem >= 0)&&(m_listboxCompList.GetCount() > 0)&&(nEDA_SchCompSelectedItem < m_listboxCompList.GetCount()) )
	{
		m_listboxCompList.SetCurSel(nEDA_SchCompSelectedItem);
		//m_listboxCompList.GetText(nEDA_SchCompSelectedItem, m_csSelectedComponent);
	}

	SetPartAndPackage();

	ShowComp();

	ShowPackage();
}

void CDlg_SchLib_Browse::ShowPackage()
{
	int sel_index = m_listboxCompList.GetCurSel();
	if(sel_index == LB_ERR) return;

	int i = m_listboxCompList.GetItemData(sel_index);
	if(i < 0) return;

	if(m_csComboPackage.IsEmpty() == TRUE)
	{
		CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PACKAGE_PREVIEW);
		RECT rect;
		pWnd->GetClientRect(&rect);
			
		rect.left++;
		rect.right--;
		rect.top++;
		rect.bottom--;
			
		CClientDC dc(pWnd);
		dc.FillSolidRect( rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, RGB(254,251,218));
		dc.IntersectClipRect(&rect);

	}
	else
	{
	
		Struct_CompList complist;
		complist = arrayEDA_SchLibraryCompList.GetAt(i);

		int schlib_number;
		for(schlib_number=0; schlib_number<SCH_SYSTEM_LIB_NUMBER; schlib_number++)
		{
			if(sch_library_offset[schlib_number] == complist.liboffset) break;
		}

		if(schlib_number < SCH_SYSTEM_LIB_NUMBER)
		{
			int pcblib_number;
			for(pcblib_number=0; pcblib_number<PCB_SYSTEM_LIB_NUMBER; pcblib_number++)
			{
				if(strcmp( sch_library_name[schlib_number], pcb_library_name[pcblib_number]) == 0) break;
			}

	
			complist.liboffset = -1;
			if(pcblib_number < PCB_SYSTEM_LIB_NUMBER)
			{
				for(int i=0; i<arrayEDA_PcbLibraryCompName[pcblib_number].GetCount(); i++)
				{
					complist = arrayEDA_PcbLibraryCompName[pcblib_number].GetAt(i);
					if(m_csComboPackage.CompareNoCase(complist.compname) == 0) break;
				}
			}

	
			if(complist.liboffset < 0)
			{
				for(int i=0; i<PCB_SYSTEM_LIB_NUMBER; i++)
				{
					for(int j=0; j<arrayEDA_PcbLibraryCompName[i].GetCount(); j++)
					{
						Struct_CompList tempcomplist;
						tempcomplist = arrayEDA_PcbLibraryCompName[i].GetAt(j);
						if(m_csComboPackage.CompareNoCase(tempcomplist.compname) == 0)
						{
							complist.basiclibrary = tempcomplist.basiclibrary;
							strcpy(complist.compname,tempcomplist.compname);
							complist.length = tempcomplist.length;
							complist.liboffset = tempcomplist.liboffset;
							complist.offset = tempcomplist.offset;
							break;
						}
					}

					if(complist.liboffset >= 0) break;
				}
			}

			if(complist.liboffset < 0)
			{
				CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PACKAGE_PREVIEW);
				RECT rect;
				pWnd->GetClientRect(&rect);
					
				rect.left++;
				rect.right--;
				rect.top++;
				rect.bottom--;
					
				CClientDC dc(pWnd);
				dc.FillSolidRect( rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top, RGB(254,251,218));
				dc.IntersectClipRect(&rect);


			}
			else
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

				int offset, length;

				unsigned char* pbuffer;
				if(complist.basiclibrary == TRUE)  
				{
					offset = complist.liboffset + complist.offset;
					length = complist.length;
			 
					pbuffer = (unsigned char*)malloc(length + 100); //new unsigned char[250000];

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

					pbuffer = (unsigned char*)malloc(length + 100); //new unsigned char[250000];

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
		}
	}
}


BOOL CDlg_SchLib_Browse::PreTranslateMessage(MSG* pMsg)
{
	if( (pMsg->message == WM_KEYDOWN)&&(pMsg->wParam==VK_RETURN) )
    {
		OnBnClickedButtonSearch();
		return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}
