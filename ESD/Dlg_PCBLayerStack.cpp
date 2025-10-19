// Dlg_PCBLayerStack.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "ESDDoc.h"
#include "Dlg_PCBLayerStack.h"
#include ".\dlg_pcblayerstack.h"
#include "systemext.h"
#include "Dlg_PCBLayerName.h"
#include "Dlg_PCBPowerPlane.h"

// CDlg_PCBLayerStack 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBLayerStack, CDialog)
CDlg_PCBLayerStack::CDlg_PCBLayerStack(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBLayerStack::IDD, pParent)
{
}

CDlg_PCBLayerStack::~CDlg_PCBLayerStack()
{
}

void CDlg_PCBLayerStack::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PCB_LAYER_STACK, m_wndReportCtrl);
}


BEGIN_MESSAGE_MAP(CDlg_PCBLayerStack, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LAYER, OnBnClickedButtonAddLayer)
	ON_BN_CLICKED(IDC_BUTTON_ADD_PLANE, OnBnClickedButtonAddPlane)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_MOVEDOWN, OnBnClickedButtonMovedown)
	ON_BN_CLICKED(IDC_BUTTON_MOVEUP, OnBnClickedButtonMoveup)
	ON_NOTIFY(NM_DBLCLK, IDC_PCB_LAYER_STACK, OnReportItemDblClick)
END_MESSAGE_MAP()


// CDlg_PCBLayerStack 消息处理程序

BOOL CDlg_PCBLayerStack::OnInitDialog()
{
	CDialog::OnInitDialog();

	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CXTPReportColumn* pColumnIndex = new CXTPReportColumn(0, "序号", "",50, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnIndex->SetEditable(FALSE);	
	pColumnIndex->SetAllowDrag(FALSE);
	m_wndReportCtrl.AddColumn(pColumnIndex);

	CXTPReportColumn* pColumnLayer = new CXTPReportColumn(1, "PCB设计层", "", 300, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnLayer->SetEditable(FALSE);	
	pColumnLayer->SetAllowDrag(FALSE);
	m_wndReportCtrl.AddColumn(pColumnLayer);

	CXTPReportColumn* pColumnLayerNum = new CXTPReportColumn(2, "层编号", "", 50, FALSE, XTP_REPORT_NOICON, FALSE, TRUE);
	pColumnLayerNum->SetEditable(FALSE);	
	pColumnLayerNum->SetAllowDrag(FALSE);
	m_wndReportCtrl.AddColumn(pColumnLayerNum);

	int layer = 0;
	int totalnum;
	for(totalnum=0; totalnum<PCB_LAYER_NUM; totalnum++)
	{
		CXTPReportRecord* pRecord =  new CXTPReportRecord();
		
		pRecord->AddItem( new CXTPReportRecordItemNumber(totalnum+1));

		if(layer == TOPLAYER) pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[layer].csLayerName));
		else if(layer == BOTTOMLAYER) pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[layer].csLayerName));
		else if((layer >= POWERPLANE1)&&(layer <= POWERPLANE16))
		{
			CString strtemp;
			strtemp = pDoc->m_sPCB_LayerParam[layer].csLayerName;

			int powerlay = layer - POWERPLANE1;
			int plane_num = 0;
			for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
			{
				COb_pcbpolygon* ppolygon = (COb_pcbpolygon*)pDoc->m_arrayPcbPolygon.GetAt(i);
				if((ppolygon->m_nPolygonType == SPLITPLANE)&&(ppolygon->m_nLayer == layer)) plane_num++;
			}
			if(plane_num > 1) strtemp += "(多网络)";
			else if(plane_num <= 0) strtemp += "(无网络)";
			else 
			{
				strtemp += "(" + pDoc->m_csPowerPlane[powerlay] + ")";
			}
			pRecord->AddItem( new CXTPReportRecordItemText(strtemp));
		}
		else pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[layer].csLayerName));

		pRecord->AddItem( new CXTPReportRecordItemNumber(layer));

		m_wndReportCtrl.AddRecord(pRecord);

		if(layer == BOTTOMLAYER ) break;
		layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
	}
	

	LayerNum();

	m_wndReportCtrl.SetMultipleSelection(FALSE);
	m_wndReportCtrl.SetGridStyle(TRUE, xtpReportGridSolid);
	m_wndReportCtrl.SetGridStyle(FALSE, xtpReportGridSolid);
	m_wndReportCtrl.Populate();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlg_PCBLayerStack::OnBnClickedButtonAddLayer()  
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CXTPReportRow* pRow = m_wndReportCtrl.GetFocusedRow();

	int index = pRow->GetIndex();

	int totalnum = m_wndReportCtrl.GetRecords()->GetCount();
	
	if(index < (totalnum-1)) index++;
		
	int current_layer = GetCurrentLayer();
	

	if( (current_layer == 0)||(current_layer >= BOTTOMLAYER)) return;


	CXTPReportRecord* pRecord =  new CXTPReportRecord();
	pRecord->AddItem( new CXTPReportRecordItemNumber(index));
	pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[current_layer].csLayerName));
	pRecord->AddItem( new CXTPReportRecordItemNumber(current_layer));
	

	CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
	pRecords->InsertAt(index, pRecord);

	for(int i=index; i<totalnum+1; i++)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportRecordItemNumber* pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(0);
		pItem->SetValue(i+1);
	}
	m_wndReportCtrl.Populate();

	LayerNum();
}

void CDlg_PCBLayerStack::OnBnClickedButtonAddPlane()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CXTPReportRow* pRow = m_wndReportCtrl.GetFocusedRow();

	int index = pRow->GetIndex();

	int totalnum = m_wndReportCtrl.GetRecords()->GetCount();
	
	if(index < (totalnum-1)) index++;
		
	int current_plane = GetCurrentPlane();

	if((current_plane == 0)||( current_plane > POWERPLANE16)) return;


	CXTPReportRecord* pRecord =  new CXTPReportRecord();
	pRecord->AddItem( new CXTPReportRecordItemNumber(index));
	pRecord->AddItem( new CXTPReportRecordItemText(pDoc->m_sPCB_LayerParam[current_plane].csLayerName + "(无网络)"));
	pRecord->AddItem( new CXTPReportRecordItemNumber(current_plane));
	

	CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
	pRecords->InsertAt(index, pRecord);

	for(int i=index; i<totalnum+1; i++)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportRecordItemNumber* pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(0);
		pItem->SetValue(i+1);
	}
	m_wndReportCtrl.Populate();

	LayerNum();
}

void CDlg_PCBLayerStack::OnBnClickedButtonDelete()
{
	CXTPReportRow* pRow = m_wndReportCtrl.GetFocusedRow();
	
	int index = pRow->GetIndex();

	int totalnum = m_wndReportCtrl.GetRecords()->GetCount();
	
	if((index == 0)||(index == (totalnum-1))) return;

	m_wndReportCtrl.RemoveRowEx(pRow);

	totalnum = m_wndReportCtrl.GetRecords()->GetCount();
	CXTPReportRecords* pRecords = m_wndReportCtrl.GetRecords();
	for(int i=0; i<totalnum; i++)
	{
		CXTPReportRecord* pRecord = pRecords->GetAt(i);
		CXTPReportRecordItemNumber* pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(0);
		pItem->SetValue(i+1);
	}
	
	LayerNum();
}

void CDlg_PCBLayerStack::OnBnClickedButtonMovedown()
{
	CXTPReportRecords* pRecords =  m_wndReportCtrl.GetRecords();
	CXTPReportRow* pRow = m_wndReportCtrl.GetFocusedRow();

	int index = pRow->GetIndex();	

	int totalnum = m_wndReportCtrl.GetRecords()->GetCount();	

	if(index <= 0) return;
	else if(index >= (totalnum-2)) return;
	
	
	CXTPReportRecord* pRecord1 = pRecords->GetAt(index);
	CXTPReportRecordItemNumber* pnum1 = (CXTPReportRecordItemNumber*)pRecord1->GetItem(2);
	CXTPReportRecordItemText* ptext1 = (CXTPReportRecordItemText*)pRecord1->GetItem(1);
	
	CXTPReportRecord* pRecord2 = pRecords->GetAt(index+1);
	CXTPReportRecordItemNumber* pnum2 = (CXTPReportRecordItemNumber*)pRecord2->GetItem(2);
	CXTPReportRecordItemText* ptext2 = (CXTPReportRecordItemText*)pRecord2->GetItem(1);
	

	CXTPReportRecord* pNewRecord1 =  new CXTPReportRecord();
	pNewRecord1->AddItem( new CXTPReportRecordItemNumber(index+1));
	pNewRecord1->AddItem( new CXTPReportRecordItemText(ptext2->GetValue()));
	pNewRecord1->AddItem( new CXTPReportRecordItemNumber(pnum2->GetValue()));


	CXTPReportRecord* pNewRecord2 =  new CXTPReportRecord();
	pNewRecord2->AddItem( new CXTPReportRecordItemNumber(index+2));
	pNewRecord2->AddItem( new CXTPReportRecordItemText(ptext1->GetValue()));
	pNewRecord2->AddItem( new CXTPReportRecordItemNumber(pnum1->GetValue()));

	pRecords->RemoveAt(index+1);
	pRecords->RemoveAt(index);
	pRecords->InsertAt(index, pNewRecord1);
	pRecords->InsertAt(index+1, pNewRecord2);

	m_wndReportCtrl.Populate();
	
	pRow = m_wndReportCtrl.GetRows()->GetAt(index+1);
	m_wndReportCtrl.SetFocusedRow(pRow, TRUE);
}

void CDlg_PCBLayerStack::OnBnClickedButtonMoveup()
{
	CXTPReportRecords* pRecords =  m_wndReportCtrl.GetRecords();
	CXTPReportRow* pRow = m_wndReportCtrl.GetFocusedRow();

	int index = pRow->GetIndex();	

	int totalnum = m_wndReportCtrl.GetRecords()->GetCount();	

	if(index <= 1) return;
	else if(index >= (totalnum-1)) return;

	CXTPReportRecord* pRecord1 = pRecords->GetAt(index-1);
	CXTPReportRecordItemNumber* pnum1 = (CXTPReportRecordItemNumber*)pRecord1->GetItem(2);
	CXTPReportRecordItemText* ptext1 = (CXTPReportRecordItemText*)pRecord1->GetItem(1);
	
	CXTPReportRecord* pRecord2 = pRecords->GetAt(index);
	CXTPReportRecordItemNumber* pnum2 = (CXTPReportRecordItemNumber*)pRecord2->GetItem(2);
	CXTPReportRecordItemText* ptext2 = (CXTPReportRecordItemText*)pRecord2->GetItem(1);
	

	CXTPReportRecord* pNewRecord1 =  new CXTPReportRecord();
	pNewRecord1->AddItem( new CXTPReportRecordItemNumber(index));
	pNewRecord1->AddItem( new CXTPReportRecordItemText(ptext2->GetValue()));
	pNewRecord1->AddItem( new CXTPReportRecordItemNumber(pnum2->GetValue()));


	CXTPReportRecord* pNewRecord2 =  new CXTPReportRecord();
	pNewRecord2->AddItem( new CXTPReportRecordItemNumber(index+1));
	pNewRecord2->AddItem( new CXTPReportRecordItemText(ptext1->GetValue()));
	pNewRecord2->AddItem( new CXTPReportRecordItemNumber(pnum1->GetValue()));


	pRecords->RemoveAt(index);
	pRecords->RemoveAt(index-1);
	pRecords->InsertAt(index-1, pNewRecord1);
	pRecords->InsertAt(index, pNewRecord2);

	m_wndReportCtrl.Populate();

	pRow = m_wndReportCtrl.GetRows()->GetAt(index-1);
	m_wndReportCtrl.SetFocusedRow(pRow, TRUE);
}

void CDlg_PCBLayerStack::LayerNum()
{
	int totalnum = m_wndReportCtrl.GetRecords()->GetCount();
	CWnd* pWnd = GetDlgItem(IDC_STATIC_NUM);
	CString str; str.Format("当前PCB设计层数 = %d", totalnum);
	pWnd->SetWindowText(str);
}

int CDlg_PCBLayerStack::GetCurrentLayer()
{
	CXTPReportRecords* pRecords =  m_wndReportCtrl.GetRecords();
	int totalnum = pRecords->GetCount();

	for(int layer=1; layer<BOTTOMLAYER; layer++)
	{
		int i;
		for(i=0; i<totalnum; i++)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			CXTPReportRecordItemNumber* pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			if(layer == pItem->GetValue()) break;
		}

		if(i >= totalnum) return layer;
	}

	return 0;
}

int CDlg_PCBLayerStack::GetCurrentPlane()
{
	CXTPReportRecords* pRecords =  m_wndReportCtrl.GetRecords();
	int totalnum = pRecords->GetCount();

	for(int layer=POWERPLANE1; layer<=POWERPLANE16; layer++)
	{
		int i;
		for(i=0; i<totalnum; i++)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			CXTPReportRecordItemNumber* pItem = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			if(layer == pItem->GetValue()) break;
		}

		if(i >= totalnum) return layer;
	}
	return 0;
}

void CDlg_PCBLayerStack::OnReportItemDblClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	XTP_NM_REPORTREQUESTEDIT* nm = (XTP_NM_REPORTREQUESTEDIT*)pNMHDR;
	CXTPReportRow* pRow = nm->pRow;
	CXTPReportRecord* pRecord = pRow->GetRecord();

	CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pRecord->GetItem(1);
	CString csLayerName = pItemText->GetValue();
	
	CXTPReportRecordItemNumber* pItemLayer = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
	int nLayer = (int)pItemLayer->GetValue();

	if((nLayer >= POWERPLANE1)&&(nLayer <= POWERPLANE16))
	{
		CDlg_PCBPowerPlane dlg;
		dlg.m_csEditLayerName = pDoc->m_sPCB_LayerParam[nLayer].csLayerName;
		
		dlg.arrayStringNet.Add("无网络");		
		for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
		{
			COb_pcbnet* pnet = (COb_pcbnet*)pDoc->m_arrayPcbNet.GetAt(i);
			if(pnet->m_nFlag != FLAG_NORMAL) continue;
			
			dlg.arrayStringNet.Add(pnet->m_csNetName);
		}
		int nNet = -1;
		for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
		{
			COb_pcbpolygon* ppolygon = (COb_pcbpolygon*)pDoc->m_arrayPcbPolygon.GetAt(i);
			if((ppolygon->m_nPolygonType == SPLITPLANE)&&(ppolygon->m_nLayer == nLayer))
			{
				if(nNet == -1) nNet = ppolygon->m_nNet;
				else 
				{
					if(nNet != ppolygon->m_nNet) 
					{
						nNet = -2;
						break;
					}
				}
			}
		}
		if(nNet == -2) dlg.m_csComboNet = "多网络";
		else
		{
			dlg.m_csComboNet = dlg.arrayStringNet.GetAt(nNet + 1);
		}

		//pullback
		dlg.m_csEditPullBack = pDoc->Convert_XY_To_String(pDoc->m_fPlanePullBack[nLayer - POWERPLANE1]);
		
		if( dlg.DoModal() == IDOK)
		{
			CString strtemp;
			strtemp = dlg.m_csEditLayerName;

			if(nNet == -2) strtemp += "(多网络)";
			else
			{
				if(dlg.m_csComboNet == "无网络") 
				{
					strtemp += "(无网络)";
				}
				else
				{
					strtemp += "(";
					strtemp += dlg.m_csComboNet;
					strtemp += ")";
					//修改split plane网络
					int net = -1;
					for(int i=0; i<dlg.arrayStringNet.GetCount(); i++)
					{
						if(dlg.arrayStringNet.GetAt(i) == dlg.m_csComboNet)
						{
							net = i-1;
						}
					}
					for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
					{
						COb_pcbpolygon* ppolygon = (COb_pcbpolygon*)pDoc->m_arrayPcbPolygon.GetAt(i);
						if((ppolygon->m_nPolygonType == SPLITPLANE)&&(ppolygon->m_nLayer == nLayer))  ppolygon->m_nNet = net;
					}
				}
			}
			pItemText->SetValue(strtemp);

			pDoc->m_fPlanePullBack[nLayer - POWERPLANE1] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditPullBack);
		}		
	}
	else
	{
		CDlg_PCBLayerName dlg;
		dlg.m_csEditLayerName = csLayerName;
		
		if( dlg.DoModal() == IDOK)
		{
			pItemText->SetValue(dlg.m_csEditLayerName);
		}
	}
	
	*pResult = 0;
}