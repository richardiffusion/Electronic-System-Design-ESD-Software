// Dlg_PCBLayerSet.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "systemext.h"
#include "ReportCtrlItem.h"
#include "Dlg_PCBLayerSet.h"
#include ".\dlg_pcblayerset.h"
#include "Dlg_PCBLayerName.h"

// CDlg_PCBLayerSet 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBLayerSet, CDialog)
CDlg_PCBLayerSet::CDlg_PCBLayerSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBLayerSet::IDD, pParent)
{
}

CDlg_PCBLayerSet::~CDlg_PCBLayerSet()
{
}

void CDlg_PCBLayerSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PCB_LAYER_SET_NAME, m_wndLayerName);
	DDX_Control(pDX, IDC_PCB_LAYER_SET_SHOW, m_wndLayerSet);
}


BEGIN_MESSAGE_MAP(CDlg_PCBLayerSet, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_PCB_LAYER_SET_NAME, OnLayerNameClick)
	ON_NOTIFY(NM_DBLCLK, IDC_PCB_LAYER_SET_NAME, OnLayerNameDblClick)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_WRITE, OnBnClickedButtonWrite)
END_MESSAGE_MAP()


// CDlg_PCBLayerSet 消息处理程序

BOOL CDlg_PCBLayerSet::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	CXTPReportColumn* pColumnName = new CXTPReportColumn(0, "组合标识", "",200, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnName->SetEditable(TRUE);
	pColumnName->SetAllowDrag(FALSE);
	m_wndLayerName.AddColumn(pColumnName);


	for(int i=0; i<pDoc->m_arrayPCBLayerSet.GetCount(); i++)
	{
		Struct_PCBLayerSet layerset;
		layerset = pDoc->m_arrayPCBLayerSet.GetAt(i);

		CXTPReportRecord* pRecord =  new CXTPReportRecord();
		pRecord->SetEditable(TRUE);
		pRecord->AddItem( new CXTPReportRecordItemText(layerset.m_csSetName));

		m_wndLayerName.AddRecord(pRecord);
	}

	m_wndLayerName.SetMultipleSelection(FALSE);
	m_wndLayerName.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndLayerName.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndLayerName.Populate();

	CXTPReportColumn* pColumnLayerSetName = new CXTPReportColumn(0, "层名称", "",230, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnLayerSetName->SetEditable(TRUE);
	pColumnLayerSetName->SetAllowDrag(FALSE);
	m_wndLayerSet.AddColumn(pColumnLayerSetName);

	CXTPReportColumn* pColumnLayerSetCheck = new CXTPReportColumn(1, "显示", "",50, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnLayerSetCheck->SetEditable(TRUE);
	pColumnLayerSetCheck->SetAllowDrag(FALSE);
	pColumnLayerSetCheck->SetHeaderAlignment(DT_CENTER);
	m_wndLayerSet.AddColumn(pColumnLayerSetCheck);

	CArray<int, int> arrayLayerSet;
	arrayLayerSet.RemoveAll();

	int layer = 0;
	for(int i=0; i<PCB_LAYER_NUM; i++)
	{
		if(((layer > BOTTOMLAYER)&&(layer < POWERPLANE1))||(layer > POWERPLANE16)) layer = BOTTOMLAYER;		
		arrayLayerSet.Add(layer);

		if(layer == BOTTOMLAYER) break;
		layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
	}

	for(int i=MECHANICAL1; i<=MECHANICAL16; i++)
	{
		if(pDoc->m_sPCB_LayerParam[i].bLayerMechEnabled == TRUE) arrayLayerSet.Add(i);
	}

	for(int i=TOPOVERLAY; i<=MULTILAYER; i++)
	{
		if((i>=POWERPLANE1)&&(i<=POWERPLANE16)) continue;
		if((i>=MECHANICAL1)&&(i<=MECHANICAL16)) continue;
		arrayLayerSet.Add(i);
	}

	for(int i=0; i<arrayLayerSet.GetCount(); i++)
	{
		int layer = arrayLayerSet.GetAt(i);
		CXTPReportRecord* pRecord =  new CXTPReportRecord();
			

		pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[layer].csLayerName));

		pRecord->AddItem(new CXTPReportRecordItemCheck(FALSE));

		m_wndLayerSet.AddRecord(pRecord);
	}

	m_wndLayerSet.SetMultipleSelection(FALSE);
	m_wndLayerSet.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndLayerSet.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndLayerSet.Populate();	
	
	int sel = 0;
	LayerNameSelChanged(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_PCBLayerSet::OnBnClickedButtonAdd()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_PCBLayerName dlg;
	dlg.m_csEditLayerName.Empty();
		
	if( dlg.DoModal() == IDOK)
	{
		CString layername;
		layername = dlg.m_csEditLayerName;
		if(layername.IsEmpty() == TRUE) layername = "自定义";

		CXTPReportRecord* pRecord =  new CXTPReportRecord();
		pRecord->SetEditable(TRUE);
		pRecord->AddItem( new CXTPReportRecordItemText(layername));

		int sel = m_wndLayerName.AddRecord(pRecord)->GetIndex();

		Struct_PCBLayerSet layerset;
		layerset.m_csSetName = layername;  layerset.m_nActiveLayer = 0; layerset.m_bFlipBoard = FALSE;  layerset.m_bLocked = FALSE; layerset.m_bCurrentSelection = FALSE;
		for(int i=0; i<=MULTILAYER; i++) layerset.m_bLayerShow[i] = FALSE;
		pDoc->m_arrayPCBLayerSet.Add(layerset);

		m_wndLayerName.Populate();
		
		SaveLayerSetParam(m_nSel);
		LayerNameSelChanged(sel);
	}
}

void CDlg_PCBLayerSet::OnBnClickedButtonApply()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	SaveLayerSetParam(m_nSel);

	CXTPReportRows* pRows = m_wndLayerName.GetRows();
	for(int i=0; i<pRows->GetCount(); i++)
	{
		Struct_PCBLayerSet layerset;
		layerset = pDoc->m_arrayPCBLayerSet.GetAt(i);

		CXTPReportRow* pRow = pRows->GetAt(i);
		
		CXTPReportRecord* pRecord = pRow->GetRecord();
		CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecord->GetItem(0);

		layerset.m_csSetName = pItem->GetValue();
		layerset.m_bCurrentSelection = pRow->IsSelected();

		pDoc->m_arrayPCBLayerSet.SetAt(i, layerset);
	}

	Struct_PCBLayerSet layerset;
	layerset = pDoc->m_arrayPCBLayerSet.GetAt(m_nSel);
	for(int i=0; i<=MULTILAYER; i++)
	{
		pDoc->m_sPCB_LayerParam[i].bLayerShow = layerset.m_bLayerShow[i];
	}
	pDoc->InitLayerShowArray();

	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CESDView* pView = (CESDView*)pDoc->GetNextView(pos);
		pView->CheckPcbCurrentLayer();
		pView->Invalidate();
		CWnd* pWnd = pView->GetParentFrame();
		pWnd->Invalidate();
	}
}

void CDlg_PCBLayerSet::OnBnClickedButtonDelete()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CXTPReportSelectedRows* pRows = m_wndLayerName.GetSelectedRows();
	CXTPReportRow* pRow = pRows->GetAt(0);
	
	int index = pRow->GetIndex();
	if(index < 5) return;

	CXTPReportRecord* pRecord = pRow->GetRecord();
	CXTPReportRecords* pRecords = pRecord->GetRecords();
	pRecords->RemoveRecord(pRecord);

	m_wndLayerName.Populate();

	pDoc->m_arrayPCBLayerSet.RemoveAt(index);

	index--;
	LayerNameSelChanged(index);
}

void CDlg_PCBLayerSet::OnBnClickedOk()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	SaveLayerSetParam(m_nSel);

	CXTPReportRows* pRows = m_wndLayerName.GetRows();
	for(int i=0; i<pRows->GetCount(); i++)
	{
		Struct_PCBLayerSet layerset;
		layerset = pDoc->m_arrayPCBLayerSet.GetAt(i);

		CXTPReportRow* pRow = pRows->GetAt(i);
		
		CXTPReportRecord* pRecord = pRow->GetRecord();
		CXTPReportRecordItemText* pItem = (CXTPReportRecordItemText*)pRecord->GetItem(0);

		layerset.m_csSetName = pItem->GetValue();
		layerset.m_bCurrentSelection = pRow->IsSelected();

		pDoc->m_arrayPCBLayerSet.SetAt(i, layerset);
	}

	Struct_PCBLayerSet layerset;
	layerset = pDoc->m_arrayPCBLayerSet.GetAt(m_nSel);
	for(int i=0; i<=MULTILAYER; i++)
	{
		pDoc->m_sPCB_LayerParam[i].bLayerShow = layerset.m_bLayerShow[i];
	}
	pDoc->InitLayerShowArray();

	POSITION pos = pDoc->GetFirstViewPosition();
	while (pos != NULL)
	{
		CESDView* pView = (CESDView*)pDoc->GetNextView(pos);
		pView->CheckPcbCurrentLayer();
		pView->Invalidate();
		CWnd* pWnd = pView->GetParentFrame();
		pWnd->Invalidate();
	}

	OnOK();
}

void CDlg_PCBLayerSet::OnLayerNameClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*struct XTP_NM_REPORTRECORDITEM
	{
		NMHDR hdr;                          // Standard structure, containing information about a notification message.
		CXTPReportRow* pRow;                // Pointer to the row associated with the notification.
		CXTPReportRecordItem* pItem;        // Pointer to the record item associated with the notification.
		CXTPReportColumn* pColumn;          // Pointer to the column associated with the notification.
		int   nHyperlink;                   // Index of clicked Hyperlink, if any, or -1 otherwise.
		POINT pt;                           // Point where the message has happened.
	};*/

	CESDDoc* pDoc = (CESDDoc*)pDocument;
	XTP_NM_REPORTREQUESTEDIT* nm = (XTP_NM_REPORTREQUESTEDIT*)pNMHDR;
	CXTPReportRow* pRow = nm->pRow;

	int index;
	if(pRow != NULL) index = pRow->GetIndex();
	else index = 0;

	SaveLayerSetParam(m_nSel);
	LayerNameSelChanged(index);

	*pResult = 0;
}

void CDlg_PCBLayerSet::OnLayerNameDblClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*struct XTP_NM_REPORTRECORDITEM
	{
		NMHDR hdr;                          // Standard structure, containing information about a notification message.
		CXTPReportRow* pRow;                // Pointer to the row associated with the notification.
		CXTPReportRecordItem* pItem;        // Pointer to the record item associated with the notification.
		CXTPReportColumn* pColumn;          // Pointer to the column associated with the notification.
		int   nHyperlink;                   // Index of clicked Hyperlink, if any, or -1 otherwise.
		POINT pt;                           // Point where the message has happened.
	};*/

	CESDDoc* pDoc = (CESDDoc*)pDocument;
	XTP_NM_REPORTREQUESTEDIT* nm = (XTP_NM_REPORTREQUESTEDIT*)pNMHDR;
	CXTPReportRow* pRow = nm->pRow;

	CXTPReportRecord* pRecord = pRow->GetRecord();
	CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(0);

	int index = pRow->GetIndex();
	SaveLayerSetParam(m_nSel);
	LayerNameSelChanged(index);

	if(index > 4)
	{
		CDlg_PCBLayerName dlg;
		dlg.m_csEditLayerName = pItemText->GetValue();
		
		if( dlg.DoModal() == IDOK)
		{
			CString layername;
			layername = dlg.m_csEditLayerName;
			if(layername.IsEmpty() == TRUE) layername = "自定义";

			pItemText->SetValue(layername);
		}
	}

	*pResult = 0;
}

void CDlg_PCBLayerSet::LayerNameSelChanged(int sel)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	m_nSel = sel;

	Struct_PCBLayerSet layerset;
	layerset = pDoc->m_arrayPCBLayerSet.GetAt(sel);
	
	//Set ReportList
	CXTPReportRecords* pSetRecords = m_wndLayerSet.GetRecords();
	for(int i=0; i<pSetRecords->GetCount(); i++)
	{
		CXTPReportRecord* pRecord = pSetRecords->GetAt(i);
		
		int layer;			
		CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(0);
		CString layername = pItemText->GetValue();
		for(int i=0; i<PCB_LAYER_NUM; i++)
		{
			if(layername == pDoc->m_sPCB_LayerParam[i].csLayerName)
			{
				layer = i;
				break;
			}
		}

		CXTPReportRecordItemCheck* pItemCheck = (CXTPReportRecordItemCheck*)pRecord->GetItem(1);
		
		if(layerset.m_bLayerShow[layer] == TRUE) 
		{
			pItemCheck->SetChecked(TRUE);
		}
		else  pItemCheck->SetChecked(FALSE);
	}
	m_wndLayerSet.Populate();	

	if(sel < 5) 
	{
		CWnd* pWnd = GetDlgItem(IDC_BUTTON_DELETE);
		pWnd->EnableWindow(FALSE);

		CXTPReportColumns* pColumns = m_wndLayerSet.GetColumns();
		CXTPReportColumn* pColumn = pColumns->GetAt(1);
		pColumn->SetEditable(FALSE);
	}
	else
	{
		CWnd* pWnd = GetDlgItem(IDC_BUTTON_DELETE);
		pWnd->EnableWindow(TRUE);
		
		CXTPReportColumns* pColumns = m_wndLayerSet.GetColumns();
		CXTPReportColumn* pColumn = pColumns->GetAt(1);
		pColumn->SetEditable(TRUE);
	}

	CXTPReportRows* pRows = m_wndLayerName.GetRows();
	CXTPReportRow* pRow = pRows->GetAt(sel);
	pRow->SetSelected(TRUE);
}

void CDlg_PCBLayerSet::SaveLayerSetParam(int old_sel)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(old_sel < 5) return;

	Struct_PCBLayerSet layerset;
	layerset = pDoc->m_arrayPCBLayerSet.GetAt(old_sel);

	CXTPReportRecords* pRecords = m_wndLayerSet.GetRecords();
	for(int i=0; i<pRecords->GetCount(); i++)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
	
		int layer;
		CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(0);
		CString layername = pItemText->GetValue();
		for(int i=0; i<PCB_LAYER_NUM; i++)
		{
			if(layername == pDoc->m_sPCB_LayerParam[i].csLayerName)
			{
				layer = i;
				break;
			}
		}

		CXTPReportRecordItemCheck* pItemCheck = (CXTPReportRecordItemCheck*)pRecord->GetItem(1);

		layerset.m_bLayerShow[layer] = pItemCheck->IsChecked();
	}
	
	pDoc->m_arrayPCBLayerSet.SetAt(old_sel, layerset);
}

void CDlg_PCBLayerSet::OnBnClickedButtonLoad()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CFileDialog importdialog(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_ENABLESIZING,"PCB层设置文件(*.layerset)|*.layerset||",NULL);  //OFN_ALLOWMULTISELECT
	if(importdialog.DoModal() == IDOK) 
	{
		CFile myfile;
		if(myfile.Open(importdialog.GetPathName(), CFile::modeRead) == TRUE)
		{
			CArchive ar(&myfile, CArchive::load);
			CString onelinestr;
			int num = 5;
			while(ar.ReadString(onelinestr) == TRUE)
			{
				num++;
				char str[100];
				itoa(num, str, 10);

				Struct_PCBLayerSet layerset;
				for(int i=0; i<=MULTILAYER; i++) layerset.m_bLayerShow[i] = FALSE;
				CString keyword, data, cstring;
				cstring = onelinestr;
				keyword = "NAME=";
				if(pDoc->Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
				{
					layerset.m_csSetName = data;
					if(layerset.m_csSetName.IsEmpty() == TRUE) layerset.m_csSetName = "自定义";
					TRACE1("%s ",layerset.m_csSetName);
				}
				keyword = "LAYERS=";
				if(pDoc->Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
				{
					for(;;)
					{
						CString layername;
						int index = data.Find(',');
						if(index >= 0)
						{
							layername = data.Left(index);
							data = data.Right(data.GetLength() - index - 1);
						}
						else
						{
							layername = data;
							for(int i=0; i<PCB_LAYER_NUM; i++)
							{
								if(csEDA_PCB_EnglishLayerSetName[i] == layername) layerset.m_bLayerShow[i] = TRUE;
							}
							break;
						}

						for(int i=0; i<PCB_LAYER_NUM; i++)
						{
							if(csEDA_PCB_EnglishLayerSetName[i] == layername) layerset.m_bLayerShow[i] = TRUE;
						}
					}

				}

				layerset.m_nActiveLayer = 0; layerset.m_bFlipBoard = FALSE;  layerset.m_bLocked = FALSE; layerset.m_bCurrentSelection = FALSE;
				pDoc->m_arrayPCBLayerSet.Add(layerset);

				CString layername;
				layername = layerset.m_csSetName;
				if(layername.IsEmpty() == TRUE) layername = "自定义";

				CXTPReportRecord* pRecord =  new CXTPReportRecord();
				pRecord->SetEditable(TRUE);
				pRecord->AddItem( new CXTPReportRecordItemText(layername));

				m_wndLayerName.AddRecord(pRecord);
				m_wndLayerName.Populate();
			}
			
			ar.Close();
			myfile.Close();
		}
	}
					
}

void CDlg_PCBLayerSet::OnBnClickedButtonWrite()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CXTPReportRecords* pRecords = m_wndLayerName.GetRecords();
	if(pRecords->GetCount() <= 5)
	{
		AfxMessageBox("除系统预定义外, 未发现用户自定义设置, 放弃保存.", MB_OK|MB_ICONSTOP);
		return;
	}

	SaveLayerSetParam(m_nSel);

	CString defaultfilename = pDoc->GetTitle();
	CFileDialog importdialog(FALSE,"",defaultfilename,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_OVERWRITEPROMPT,"PCB层设置文件(*.layerset)|*.layerset||",NULL);  //OFN_ALLOWMULTISELECT
	
	char filename[MAX_PATH];
	if(importdialog.DoModal() == IDOK) 
	{
		strcpy(filename, importdialog.GetPathName());
		CFile myfile;
		if(myfile.Open(filename, CFile::modeWrite|CFile::modeCreate) == TRUE)
		{
			CArchive ar(&myfile, CArchive::store);
			CString onelinestr, keyword;
			char str[100];
			onelinestr.Empty();
			for(int i=5; i<pDoc->m_arrayPCBLayerSet.GetCount(); i++)
			{
				int activelayer = 0, flag = 0;
				
				Struct_PCBLayerSet layerset;
				layerset = pDoc->m_arrayPCBLayerSet.GetAt(i);
				
				char numstr[100];
				itoa(i+1, numstr, 10);
				
				keyword = "LAYERSET"; keyword += numstr; keyword += "NAME=";
				keyword += layerset.m_csSetName;
				onelinestr += keyword;

				keyword = "|LAYERSET"; keyword += numstr; keyword += "LAYERS=";
				for(int j=0; j<PCB_LAYER_NUM; j++)
				{
					if(layerset.m_bLayerShow[j] == TRUE)
					{
						if(flag > 0) keyword += ",";
						keyword += csEDA_PCB_EnglishLayerSetName[j];
						if(flag == 0) activelayer = j;
						flag++;
					}
				}
				onelinestr += keyword;
				
				keyword = "|LAYERSET"; keyword += numstr; keyword += "ACTIVELAYER=";
				itoa(activelayer, str, 10);
				keyword += str;
				onelinestr += keyword;
				
				keyword = "|LAYERSET"; keyword += numstr; keyword += "ISCURRENT=FALSE";
				onelinestr += keyword;

				keyword = "|LAYERSET"; keyword += numstr; keyword += "ISLOCKED=TRUE";
				onelinestr += keyword;

				keyword = "|LAYERSET"; keyword += numstr; keyword += "FLIPBOARD=FALSE";
				onelinestr += keyword;

				//结尾
				onelinestr += "\r\n";
				ar.WriteString(onelinestr);
				onelinestr.Empty();	
			}
			ar.Flush();
			myfile.Close();
		}
	}
}
