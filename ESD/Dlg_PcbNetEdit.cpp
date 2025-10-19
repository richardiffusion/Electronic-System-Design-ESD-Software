// Dlg_PcbNetEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "ESDDoc.h"
#include "Dlg_PcbNetEdit.h"
#include ".\dlg_pcbnetedit.h"

#include "Dlg_PcbNetParamEdit.h"


// CDlg_PcbNetEdit 对话框

IMPLEMENT_DYNAMIC(CDlg_PcbNetEdit, CDialog)
CDlg_PcbNetEdit::CDlg_PcbNetEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PcbNetEdit::IDD, pParent)
{
}

CDlg_PcbNetEdit::~CDlg_PcbNetEdit()
{
}

void CDlg_PcbNetEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NET_LIST, m_listboxNet);
	DDX_Control(pDX, IDC_PIN_LIST, m_listboxPin);
}


BEGIN_MESSAGE_MAP(CDlg_PcbNetEdit, CDialog)
	ON_LBN_SELCHANGE(IDC_NET_LIST, OnLbnSelchangeNetList)
	ON_BN_CLICKED(IDC_PIN_EDIT, OnBnClickedPinEdit)
	ON_BN_CLICKED(IDC_NET_EDIT, OnBnClickedNetEdit)
	ON_BN_CLICKED(IDC_NET_DELETE, OnBnClickedNetDelete)
	ON_BN_CLICKED(IDC_NET_ADD, OnBnClickedNetAdd)
END_MESSAGE_MAP()


// CDlg_PcbNetEdit 消息处理程序

BOOL CDlg_PcbNetEdit::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_listboxNet.SetColumnWidth(132);
	m_listboxPin.SetColumnWidth(132);


	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	//net
	m_listboxNet.ResetContent();
	for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;

		m_listboxNet.AddString(pnet->m_csNetName);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void CDlg_PcbNetEdit::OnLbnSelchangeNetList()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	int index = m_listboxNet.GetCurSel();
	if(index != LB_ERR)
	{
		CString netname;
		m_listboxNet.GetText(index, netname);

		m_listboxPin.ResetContent();
		for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
			if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nNet < 0)) continue;

			CString csPin;
			COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(ppad->m_nNet);

			if(pnet->m_csNetName == netname)
			{
				CString csDesignator;
				if(ppad->m_nComponent < 0)
				{
					csDesignator = "独立焊盘";
				}
				else
				{
					COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
					csDesignator = pcomp->GetPcbCompDesignator(ppad->m_nComponent);
				}

				csPin = csDesignator + "-" + ppad->m_csName;
				int p = m_listboxPin.AddString(csPin);
				m_listboxPin.SetItemData(p, csDesignator.GetLength());
			}
		}
	}
}

void CDlg_PcbNetEdit::OnBnClickedPinEdit()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	int index = m_listboxPin.GetCurSel();
	if(index !=  LB_ERR)
	{
		CString cstring;
		m_listboxPin.GetText(index, cstring);
		CString padname, designator;
		int p = m_listboxPin.GetItemData(index);
		
		designator = cstring.Left(p);
		padname = cstring.Right(cstring.GetLength() - p - 1);

		int compindex;
		for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
		{
			COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
			if(pcomp->m_nFlag != FLAG_NORMAL) continue;

			CString csDesignator = pcomp->GetPcbCompDesignator(i);
			if(csDesignator == designator) 
			{
				compindex = i;
				break;
			}
		}

		for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
			if((ppad->m_nComponent == compindex)&&(ppad->m_csName == padname)&&(ppad->m_nFlag == FLAG_NORMAL))
			{
				ppad->Change(i);

				int net_listindex = m_listboxNet.GetCurSel();
				int netIndex = -1;
				CString netname;
				m_listboxNet.GetText(net_listindex, netname);
				for(int j=0; j<pDoc->m_arrayPcbNet.GetCount(); j++)
				{
					COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(j);
					if(pnet->m_nFlag != FLAG_NORMAL) continue;

					if(pnet->m_csNetName == netname)
					{
						netIndex = j;
						break;
					}
				}
	
				if(ppad->m_nNet != netIndex)
				{
					m_listboxPin.DeleteString(index);
				}
				else
				{
					if(padname != ppad->m_csName)
					{
						m_listboxPin.DeleteString(index);
						CString csPin = designator + "-" + ppad->m_csName;
						int p = m_listboxPin.AddString(csPin);
						m_listboxPin.SetItemData(p, designator.GetLength());
					}
				}

				break;
			}
		}
	}
}

void CDlg_PcbNetEdit::OnBnClickedNetDelete()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	int netindex = m_listboxNet.GetCurSel();
	if(netindex != LB_ERR)
	{
		CString netname;
		m_listboxNet.GetText(netindex, netname);

		int netIndex = -1;
		for(int j=0; j<pDoc->m_arrayPcbNet.GetCount(); j++)
		{
			COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(j);
			if(pnet->m_nFlag != FLAG_NORMAL) continue;

			if(pnet->m_csNetName == netname)
			{
				netIndex = j;
				break;
			}
		}

		CString str = "确认删除这个网络: " + netname + "?";
		if(AfxMessageBox(str, MB_YESNO|MB_ICONWARNING) == IDYES)
		{
			for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
			{
				COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(i);
				if((pnet->m_csNetName == netname)&&(pnet->m_nFlag == FLAG_NORMAL))
				{
					//----------- Undo ------------------------------------------------------------------------------------
					if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}

					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_NET;
					pundo->m_nIndex = netIndex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					//------------------------------------------------------------------------------------------------------	

					pnet->m_nFlag = FLAG_DELETED;
					m_listboxNet.DeleteString(netindex);
					m_listboxPin.ResetContent();
					for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
					{
						COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
						if((parc->m_nNet == netIndex)&&(parc->m_nFlag == FLAG_NORMAL))
						{
							//----------- Undo -------------------------------------------
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = PCB_ELEMENT_ARC;
							pundo->m_nIndex = j;
							pundo->pcbarc.Copy(parc);
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							//------------------------------------------------------------				
							parc->m_nNet = -1;
						}
					}
					for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
					{
						COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
						if((ptrack->m_nNet == netIndex)&&(ptrack->m_nFlag == FLAG_NORMAL))
						{
							//----------- Undo -------------------------------------------
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = j;
							pundo->pcbtrack.Copy(ptrack);
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							//------------------------------------------------------------		
							ptrack->m_nNet = -1;
						}
					}
					for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
					{
						COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
						if((ppad->m_nNet == netIndex)&&(ppad->m_nFlag == FLAG_NORMAL))
						{
							//----------- Undo -------------------------------------------
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = PCB_ELEMENT_PAD;
							pundo->m_nIndex = j;
							pundo->pcbpad.Copy(ppad);
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							//------------------------------------------------------------		
							ppad->m_nNet = -1;
						}
					}
					for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
					{
						COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
						if((pvia->m_nNet == netIndex)&&(pvia->m_nFlag == FLAG_NORMAL))
						{
							//----------- Undo -------------------------------------------
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = PCB_ELEMENT_VIA;
							pundo->m_nIndex = j;
							pundo->pcbvia.Copy(pvia);
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							//------------------------------------------------------------	
							pvia->m_nNet = -1;
						}
					}
					for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
					{
						COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
						if((pfill->m_nNet == netIndex)&&(pfill->m_nFlag == FLAG_NORMAL))
						{
							//----------- Undo -------------------------------------------
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = PCB_ELEMENT_FILL;
							pundo->m_nIndex = j;
							pundo->pcbfill.Copy(pfill);
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							//------------------------------------------------------------	
							pfill->m_nNet = -1;
						}
					}
					for(int j=0; j<pDoc->m_arrayPcbPolygon.GetCount(); j++)
					{
						COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(j);
						if((ppolygon->m_nNet == netIndex)&&(ppolygon->m_nFlag == FLAG_NORMAL))
						{
							//----------- Undo -------------------------------------------
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = PCB_ELEMENT_POLYGON;
							pundo->m_nIndex = j;
							pundo->pcbpolygon.Copy(ppolygon);
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							//------------------------------------------------------------	
							ppolygon->m_nNet = -1;
						}
					}
					for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
					{
						COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
						if((pregion->m_nNet == netIndex)&&(pregion->m_nFlag == FLAG_NORMAL))
						{
							//----------- Undo -------------------------------------------
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = PCB_ELEMENT_REGION;
							pundo->m_nIndex = j;
							pundo->pcbregion.Copy(pregion);
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							//------------------------------------------------------------	
							pregion->m_nNet = -1;
						}
					}

					for(int j=0; j<pDoc->m_arrayPcbConnection.GetCount(); j++)
					{
						COb_pcbconnection* pconnection = pDoc->m_arrayPcbConnection.GetAt(j);
						if(pconnection->m_nNet == netIndex)
						{
							pDoc->m_arrayPcbConnection.RemoveAt(j);
							j--;
							delete pconnection;
						}
					}
					//------------------------------------------------------------------------------------------------------	
					pDoc->m_nPCB_Undo_Num++;
					if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					//------------------------------------------------------------------------------------------------------					
					break;
				}
			}
		}
	}
}

void CDlg_PcbNetEdit::OnBnClickedNetEdit()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_PcbNetParamEdit dlg;

	int netindex = m_listboxNet.GetCurSel();
	if(netindex != LB_ERR)
	{
		CString netname;
		m_listboxNet.GetText(netindex, netname);

		int netIndex = -1;
		COb_pcbnet* pnet;
		for(int j=0; j<pDoc->m_arrayPcbNet.GetCount(); j++)
		{
			pnet = pDoc->m_arrayPcbNet.GetAt(j);
			if(pnet->m_nFlag != FLAG_NORMAL) continue;

			if(pnet->m_csNetName == netname)
			{
				netIndex = j;
				break;
			}
		}
		
		dlg.pDocument = pDocument;
		dlg.nNetIndex = netIndex;
		dlg.m_bCheckStackLayerOnly = TRUE;
		dlg.m_csEditNetName = netname;
		dlg.m_nColor = pnet->m_nColor;
		if(pnet->m_bVisible == TRUE) dlg.m_bCheckHideConnection = FALSE;
		else dlg.m_bCheckHideConnection = TRUE;
		dlg.m_bCheckRemoveLoop = pnet->m_bLoopRemove;
		for(int i=0; i<SIGNAL_LAYER_NUM; i++) dlg.m_fTrackWidth[i] = pnet->m_fTrackWidth[i];	
		dlg.m_csEditTrackWidth = pDoc->Convert_XY_To_String(0);
		dlg.m_csEditVia = pDoc->Convert_XY_To_String(pnet->m_fViaDia);
		dlg.m_csEditViaHole = pDoc->Convert_XY_To_String(pnet->m_fViaHoleDia);
		
		if(dlg.DoModal() == IDOK)
		{

			//----------- Undo -----------------------------------------------------
			if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_NET;
			pundo->m_nIndex = netIndex;
			pundo->pcbnet.Copy(pnet);
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			//----------------------------------------------------------------------

			pnet->m_csNetName = dlg.m_csEditNetName;
			pnet->m_nColor = dlg.m_nColor;
			if(dlg.m_bCheckHideConnection == FALSE) pnet->m_bVisible = TRUE;
			else pnet->m_bVisible = FALSE;
			pnet->m_bLoopRemove = dlg.m_bCheckRemoveLoop;
			pnet->m_fViaDia = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditVia);
			pnet->m_fViaHoleDia = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditViaHole);
			
			double fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTrackWidth);
			if(fWidth > 0)
			{
				for(int i=0; i<SIGNAL_LAYER_NUM; i++) pnet->m_fTrackWidth[i] = fWidth;
			}
			else
			{
				CXTPReportRecords* pRecords = dlg.m_wndSignalCtrl.GetRecords();
				for(int i=0; i<pRecords->GetCount(); i++)
				{
					CXTPReportRecord* pRecord = pRecords->GetAt(i);

					CXTPReportRecordItemText* pItemIndex = (CXTPReportRecordItemText*)pRecord->GetItem(3);
					int layer = atoi(pItemIndex->GetValue());

					CXTPReportRecordItemText* pItemWidth = (CXTPReportRecordItemText*)pRecord->GetItem(0);
					 pnet->m_fTrackWidth[layer] = pDoc->Convert_PCB_String_To_XY(pItemWidth->GetValue());
				}
			}

			CXTPReportRecords* pRecords = dlg.m_wndPinsNew.GetRecords();
			for(int i=0; i<pRecords->GetCount(); i++)	
			{
				CXTPReportRecord* pRecord = pRecords->GetAt(i);
				CXTPReportRecordItemText* pItemPin = (CXTPReportRecordItemText*)pRecord->GetItem(0);
				
				CString csPin, csPadName, csDesignator;
				csPin = pItemPin->GetValue();
				int p = pItemPin->GetItemData();
				
				csDesignator = csPin.Left(p);
				csPadName = csPin.Right(csPin.GetLength() - p - 1);

				
				for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
					if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent < 0)) continue;

					if(ppad->m_csName == csPadName)
					{
						COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
						if(csDesignator == pcomp->GetPcbCompDesignator(ppad->m_nComponent))
						{
							//----------- Undo -------------------------------------------
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = PCB_ELEMENT_PAD;
							pundo->m_nIndex = j;
							pundo->pcbpad.Copy(ppad);
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							//------------------------------------------------------------									
							ppad->m_nNet = netIndex;
						}
					}
				}
			}
			
			pRecords = dlg.m_wndPinsOld.GetRecords();
			for(int i=0; i<pRecords->GetCount(); i++)
			{
				CXTPReportRecord* pRecord = pRecords->GetAt(i);
				CXTPReportRecordItemText* pItemPin = (CXTPReportRecordItemText*)pRecord->GetItem(0);
				CXTPReportRecordItemText* pItemNet = (CXTPReportRecordItemText*)pRecord->GetItem(1);

				
				if(pItemNet->GetValue() == netname)
				{				
					CString csPin, csPadName, csDesignator;
					csPin = pItemPin->GetValue();
					int p = pItemPin->GetItemData();
					
					csDesignator = csPin.Left(p);
					csPadName = csPin.Right(csPin.GetLength() - p - 1);

					for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
					{
						COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
						if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent < 0)) continue;

						if(ppad->m_csName == csPadName)
						{
							COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
							if(csDesignator == pcomp->GetPcbCompDesignator(ppad->m_nComponent))
							{
								//----------- Undo -------------------------------------------
								CPcbUndo *pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = PCB_ELEMENT_PAD;
								pundo->m_nIndex = j;
								pundo->pcbpad.Copy(ppad);
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								//------------------------------------------------------------									
								ppad->m_nNet = -1;
							}
						}
					}
				}
			}
					
			//-------------------------------------------------------------------------------
			pDoc->m_nPCB_Undo_Num++;
			if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			//-------------------------------------------------------------------------------

			if(netname != pnet->m_csNetName) 
			{
				m_listboxNet.DeleteString(netindex);
				m_listboxNet.InsertString(netindex, pnet->m_csNetName);
			}
			m_listboxNet.SetCurSel(netindex);
			OnLbnSelchangeNetList();
		}
	}

}

void CDlg_PcbNetEdit::OnBnClickedNetAdd()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_PcbNetParamEdit dlg;
		
	int netIndex = -1;
	int netindex;

	COb_pcbnet* pnet = new COb_pcbnet();
		
	CString netname = "NewNet";

	dlg.pDocument = pDocument;
	dlg.nNetIndex = netIndex;
	dlg.m_bCheckStackLayerOnly = TRUE;
	dlg.m_csEditNetName = netname;
	dlg.m_nColor = pnet->m_nColor;
	if(pnet->m_bVisible == TRUE) dlg.m_bCheckHideConnection = FALSE;
	else dlg.m_bCheckHideConnection = TRUE;
	dlg.m_bCheckRemoveLoop = pnet->m_bLoopRemove;
	for(int i=0; i<SIGNAL_LAYER_NUM; i++) dlg.m_fTrackWidth[i] = pnet->m_fTrackWidth[i];	
	dlg.m_csEditTrackWidth = pDoc->Convert_XY_To_String(0);
	dlg.m_csEditVia = pDoc->Convert_XY_To_String(pnet->m_fViaDia);
	dlg.m_csEditViaHole = pDoc->Convert_XY_To_String(pnet->m_fViaHoleDia);
		
	if(dlg.DoModal() == IDOK)
	{
		netIndex = pDoc->m_arrayPcbNet.Add(pnet);
		//----------- Undo -----------------------------------------------------
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_DELETE;
		pundo->m_nOb = PCB_ELEMENT_NET;
		pundo->m_nIndex = netIndex;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		//----------------------------------------------------------------------

		pnet->m_csNetName = dlg.m_csEditNetName;
		pnet->m_nColor = dlg.m_nColor;
		if(dlg.m_bCheckHideConnection == FALSE) pnet->m_bVisible = TRUE;
		else pnet->m_bVisible = FALSE;
		pnet->m_bLoopRemove = dlg.m_bCheckRemoveLoop;
		pnet->m_fViaDia = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditVia);
		pnet->m_fViaHoleDia = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditViaHole);
			
		double fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTrackWidth);
		if(fWidth > 0)
		{
			for(int i=0; i<SIGNAL_LAYER_NUM; i++) pnet->m_fTrackWidth[i] = fWidth;
		}
		else
		{
			CXTPReportRecords* pRecords = dlg.m_wndSignalCtrl.GetRecords();
			for(int i=0; i<pRecords->GetCount(); i++)
			{
				CXTPReportRecord* pRecord = pRecords->GetAt(i);

				CXTPReportRecordItemText* pItemIndex = (CXTPReportRecordItemText*)pRecord->GetItem(3);
				int layer = atoi(pItemIndex->GetValue());

				CXTPReportRecordItemText* pItemWidth = (CXTPReportRecordItemText*)pRecord->GetItem(0);
				 pnet->m_fTrackWidth[layer] = pDoc->Convert_PCB_String_To_XY(pItemWidth->GetValue());
			}
		}

		CXTPReportRecords* pRecords = dlg.m_wndPinsNew.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			CXTPReportRecordItemText* pItemPin = (CXTPReportRecordItemText*)pRecord->GetItem(0);
				
			CString csPin, csPadName, csDesignator;
			csPin = pItemPin->GetValue();
			int p = pItemPin->GetItemData();
				
			csDesignator = csPin.Left(p);
			csPadName = csPin.Right(csPin.GetLength() - p - 1);

			for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
				if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent < 0)) continue;

				if(ppad->m_csName == csPadName)
				{
					COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
					if(csDesignator == pcomp->GetPcbCompDesignator(ppad->m_nComponent))
					{
						//----------- Undo -------------------------------------------
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = PCB_ELEMENT_PAD;
						pundo->m_nIndex = j;
						pundo->pcbpad.Copy(ppad);
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						//------------------------------------------------------------									
						ppad->m_nNet = netIndex;
					}
				}
			}
		}
					
		//-------------------------------------------------------------------------------
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		//-------------------------------------------------------------------------------

		//更新m_listboxNet和m_listboxPin
		netindex = m_listboxNet.AddString(pnet->m_csNetName);
		m_listboxNet.SetCurSel(netindex);
		OnLbnSelchangeNetList();
	}
	else
	{
		delete pnet;
	}
}
