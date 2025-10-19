// Dlg_Temp.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Dlg_Temp.h"
#include ".\dlg_temp.h"

#include "systemext.h"

// CDlg_Temp �Ի���

IMPLEMENT_DYNAMIC(CDlg_Temp, CDialog)
CDlg_Temp::CDlg_Temp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_Temp::IDD, pParent)
{

}

CDlg_Temp::~CDlg_Temp()
{
}

void CDlg_Temp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
}


BEGIN_MESSAGE_MAP(CDlg_Temp, CDialog)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDlg_Temp ��Ϣ�������

BOOL CDlg_Temp::OnInitDialog()
{
	CDialog::OnInitDialog();

	//��ʼ��sch_v1.dat����Ԫ����
	HTREEITEM hitem[10];
	char s[260], sbak[260], itemfile[200];
	int  i, j, n = 200;


	int lib_index;
	for(int k=0; k<arrayEDA_SchLibraryTreeStructure.GetCount(); k++)
	{
		CString cstring = arrayEDA_SchLibraryTreeStructure.GetAt(k);
		strcpy(s, cstring);

		// ����ַ���ǰ���м���Tab, ȥ����ЩTab
		strcpy(sbak, s);
		for(j=0; j<10; j++)  // ����Ŀ����ȿ���
		{
			if(sbak[j] != 0x09) break;
		}
		memcpy(s,  sbak+j, strlen(sbak+j)+1);


		// ����ַ������Ƿ�������
		for(int i=strlen(s); i>0; i--)
		{
			if(s[i] == '|') break;
		}
		if( i > 0) // ������
		{
			s[i] = '\0';
			memcpy(sbak, s+i+1, strlen(s+i+1)+1);
		}
		else  // û��
		{
			sbak[0] = '\0';
		}


		if(j == 0)  //����Ŀ
		{
			hitem[j] = m_tree.InsertItem(s, 0, 0, TVI_ROOT, TVI_LAST);
			if(strlen(sbak) != 0)
			{
				if(stricmp(sbak,"Y") == 0) m_tree.SetItemData(hitem[j], -2);		//����, expanded
				else if(stricmp(sbak,"N") == 0) m_tree.SetItemData(hitem[j], -1);	//����, collapse
				else	//����
				{
					lib_index = atoi(sbak);
					m_tree.SetItemData(hitem[j], lib_index);
				}
			}
			else m_tree.SetItemData(hitem[j], -1);	//����, �����������ʱ
		}
		else 
		{
			if(strlen(sbak) != 0)
			{

				if(stricmp(sbak,"Y") == 0)			//����, expanded
				{
					hitem[j] = m_tree.InsertItem (s, 0, 0, hitem[j-1], TVI_LAST);
					m_tree.SetItemData(hitem[j], -2);
				}
				else if(stricmp(sbak,"N") == 0)		//����, collapse
				{
					hitem[j] = m_tree.InsertItem (s, 0, 0, hitem[j-1], TVI_LAST);
					m_tree.SetItemData(hitem[j], -1);
				}
				else	//����
				{
					hitem[j] = m_tree.InsertItem (s, 1, 1, hitem[j-1], TVI_LAST);
					lib_index = atoi(sbak);
					m_tree.SetItemData(hitem[j], lib_index);
				}
			}
			else 
			{
				hitem[j] = m_tree.InsertItem (s, 0, 0, hitem[j-1], TVI_LAST);
				m_tree.SetItemData(hitem[j], -1);	
			}
		}
	}
		
	//��������item 
	//1.��չ/��������Ŀ
	HTREEITEM hitem_0, hitem_1, hitem_2, hitem_3, hitem_4;
	hitem_0 = m_tree.GetRootItem( );
	while (hitem_0 != NULL)
	{
		if( m_tree.ItemHasChildren(hitem_0) == TRUE)
		{
			if(m_tree.GetItemData(hitem_0) == -1) m_tree.Expand(hitem_0, TVE_COLLAPSE);
			else m_tree.Expand(hitem_0, TVE_EXPAND);

			hitem_1 = m_tree.GetChildItem(hitem_0);
			while(hitem_1 != NULL)
			{
				if( m_tree.ItemHasChildren(hitem_1) == TRUE)
				{
					if(m_tree.GetItemData(hitem_1) == -1) m_tree.Expand(hitem_1, TVE_COLLAPSE);
					else m_tree.Expand(hitem_1, TVE_EXPAND);

					hitem_2 = m_tree.GetChildItem(hitem_1);
					while(hitem_2 != NULL)
					{
						if( m_tree.ItemHasChildren(hitem_2) == TRUE)
						{
							if(m_tree.GetItemData(hitem_2) == -1) m_tree.Expand(hitem_2, TVE_COLLAPSE);
							else m_tree.Expand(hitem_2, TVE_EXPAND);

							hitem_3 = m_tree.GetChildItem(hitem_2);
							while(hitem_3 != NULL)
							{
								//������㸸�ӽṹ
								hitem_3 = m_tree.GetNextSiblingItem(hitem_3);
							}
						}
						hitem_2 = m_tree.GetNextSiblingItem(hitem_2);
					}
				}
				hitem_1 = m_tree.GetNextSiblingItem(hitem_1);
			}
		}
		hitem_0 = m_tree.GetNextSiblingItem(hitem_0);
	}

	//2.ѡ����Ŀ
	hitem_0 = m_tree.GetRootItem( );
	int item_num = 0;
	while (hitem_0 != NULL)
	{
		if(nEDA_SchLibrarySelectedItem == item_num)
		{
			BOOL b = m_tree.Select(hitem_0,TVGN_CARET   );//TVGN_FIRSTVISIBLE);
			//BOOL b = m_tree.SelectSetFirstVisible(hitem_0);
			//BOOL b = m_tree.SelectItem(hitem_0);
			//if(b == TRUE) TRACE1("TRUE %d\n", item_num);
			//else TRACE1("FALSE %d\n", item_num);
		}
		item_num++;
		if( m_tree.ItemHasChildren(hitem_0) == TRUE)
		{
			hitem_1 = m_tree.GetChildItem(hitem_0);
			while(hitem_1 != NULL)
			{
				if(nEDA_SchLibrarySelectedItem == item_num) 
				{
					BOOL b = m_tree.Select(hitem_1,TVGN_CARET   );//TVGN_FIRSTVISIBLE);
					//BOOL b = m_tree.SelectSetFirstVisible(hitem_1);
					//BOOL b = m_tree.SelectItem(hitem_1);
					//if(b == TRUE) TRACE1("TRUE %d\n", item_num);
					//else TRACE1("FALSE %d\n", item_num);
				}
				item_num++;
				if( m_tree.ItemHasChildren(hitem_1) == TRUE)
				{
					hitem_2 = m_tree.GetChildItem(hitem_1);
					while(hitem_2 != NULL)
					{
						if(nEDA_SchLibrarySelectedItem == item_num) 
						{
							BOOL b = m_tree.Select(hitem_2,TVGN_CARET   );//TVGN_FIRSTVISIBLE);
							//BOOL b = m_tree.SelectSetFirstVisible(hitem_2);
							//BOOL b = m_tree.SelectItem(hitem_2);
							//if(b == TRUE) TRACE1("sdTRUE %d\n", item_num);
							//else TRACE1("FALSE %d\n", item_num);
						}
						item_num++;
						if( m_tree.ItemHasChildren(hitem_2) == TRUE)
						{
							hitem_3 = m_tree.GetChildItem(hitem_2);
							while(hitem_3 != NULL)
							{
								if(nEDA_SchLibrarySelectedItem == item_num)
								{
									BOOL b = m_tree.Select(hitem_3,TVGN_CARET   );//TVGN_FIRSTVISIBLE);
									//BOOL b = m_tree.SelectSetFirstVisible(hitem_3);
									//BOOL b = m_tree.SelectItem(hitem_3);
									//if(b == TRUE) TRACE1("TRUE %d\n", item_num);
									//else TRACE1("FALSE %d\n", item_num);
								}
								item_num++;
								//������㸸�ӽṹ
								hitem_3 = m_tree.GetNextSiblingItem(hitem_3);
							}
						}
						hitem_2 = m_tree.GetNextSiblingItem(hitem_2);
					}
				}
				hitem_1 = m_tree.GetNextSiblingItem(hitem_1);
			}
		}
		hitem_0 = m_tree.GetNextSiblingItem(hitem_0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlg_Temp::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//TRACE("1\n");

	HTREEITEM hitem = m_tree.GetSelectedItem( );

	int index = m_tree.GetItemData(	hitem);

	char s[100];  itoa(index, s, 10);
	//TRACE(s);TRACE("\n");

	*pResult = 0;
}

void CDlg_Temp::OnDestroy()
{
	CDialog::OnDestroy();

	//����CTreeCtrl����Ŀ��չ��������Ϣ
	HTREEITEM hitem_0, hitem_1, hitem_2, hitem_3, hitem_4;
	
	int item_num = 0;
	CString cstring;

	HTREEITEM selected_item;
	selected_item = m_tree.GetSelectedItem();

	//��������item, 1.���浱ǰѡ����Ŀ  2.���游��Ŀ����չ/������״̬
	hitem_0 = m_tree.GetRootItem( );
	while (hitem_0 != NULL)
	{
		//cstring = m_tree.GetItemText(hitem_0);
		//TRACE2("%s %d\n", cstring, item_num);
		if(hitem_0 == selected_item) nEDA_SchLibrarySelectedItem = item_num;
		item_num++;

		if( m_tree.ItemHasChildren(hitem_0) == TRUE)
		{
			//cstring = m_tree.GetItemText(hitem_0);
			//TRACE2("%s %d\n", cstring, item_num-1);
			cstring = arrayEDA_SchLibraryTreeStructure.GetAt(item_num-1);
			if(cstring.Find("|Y") >= 0) cstring.TrimRight("|Y");
			else if(cstring.Find("|N") >= 0) cstring.TrimRight("|N");
			if( (m_tree.GetItemState(hitem_0, TVIS_EXPANDED)&TVIS_EXPANDED) != 0) cstring += "|Y";
			else cstring += "|N";
			arrayEDA_SchLibraryTreeStructure.SetAt(item_num-1, cstring);


			hitem_1 = m_tree.GetChildItem(hitem_0);
			while(hitem_1 != NULL)
			{
				//cstring = m_tree.GetItemText(hitem_1);
				//TRACE2("%s %d\n", cstring, item_num);
				if(hitem_1 == selected_item) nEDA_SchLibrarySelectedItem = item_num;
				item_num++;

				if( m_tree.ItemHasChildren(hitem_1) == TRUE)
				{
					//cstring = m_tree.GetItemText(hitem_1);
					//TRACE2("%s %d\n", cstring, item_num-1);
					cstring = arrayEDA_SchLibraryTreeStructure.GetAt(item_num-1);
					if(cstring.Find("|Y") >= 0) cstring.TrimRight("|Y");
					else if(cstring.Find("|N") >= 0) cstring.TrimRight("|N");
					if( (m_tree.GetItemState(hitem_1, TVIS_EXPANDED)&TVIS_EXPANDED) != 0) cstring += "|Y";
					else cstring += "|N";
					arrayEDA_SchLibraryTreeStructure.SetAt(item_num-1, cstring);

					hitem_2 = m_tree.GetChildItem(hitem_1);
					while(hitem_2 != NULL)
					{
						//cstring = m_tree.GetItemText(hitem_2);
						//TRACE2("%s %d\n", cstring, item_num);
						if(hitem_2 == selected_item) nEDA_SchLibrarySelectedItem = item_num;
						item_num++;

						if( m_tree.ItemHasChildren(hitem_2) == TRUE)
						{
							//cstring = m_tree.GetItemText(hitem_2);
							//TRACE2("%s %d\n", cstring, item_num-1);
							cstring = arrayEDA_SchLibraryTreeStructure.GetAt(item_num-1);
							if(cstring.Find("|Y") >= 0) cstring.TrimRight("|Y");
							else if(cstring.Find("|N") >= 0) cstring.TrimRight("|N");
							if( (m_tree.GetItemState(hitem_2, TVIS_EXPANDED)&TVIS_EXPANDED) != 0) cstring += "|Y";
							else cstring += "|N";
							arrayEDA_SchLibraryTreeStructure.SetAt(item_num-1, cstring);
							
							hitem_3 = m_tree.GetChildItem(hitem_2);
							while(hitem_3 != NULL)
							{
								//cstring = m_tree.GetItemText(hitem_3);
								//TRACE2("%s %d\n", cstring, item_num);
								if(hitem_3 == selected_item) nEDA_SchLibrarySelectedItem = item_num;
								item_num++;

								//������㸸�ӽṹ
								hitem_3 = m_tree.GetNextSiblingItem(hitem_3);
							}
						}
						hitem_2 = m_tree.GetNextSiblingItem(hitem_2);
					}

				}
				
				hitem_1 = m_tree.GetNextSiblingItem(hitem_1);
			}
		}
		hitem_0 = m_tree.GetNextSiblingItem(hitem_0);
	}
}

