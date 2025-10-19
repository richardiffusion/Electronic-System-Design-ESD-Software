// Dlg_PCBBoardInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "ESDDoc.h"
#include "sysparam.h"
#include "systemext.h"
#include "Dlg_PCBBoardInfo.h"
#include ".\dlg_pcbboardinfo.h"


// CDlg_PCBBoardInfo 对话框

IMPLEMENT_DYNAMIC(CDlg_PCBBoardInfo, CDialog)
CDlg_PCBBoardInfo::CDlg_PCBBoardInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlg_PCBBoardInfo::IDD, pParent)
{
}

CDlg_PCBBoardInfo::~CDlg_PCBBoardInfo()
{
}

void CDlg_PCBBoardInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_COMP, m_listComp);
	DDX_Control(pDX, IDC_LIST_NET, m_listNet);
	DDX_Control(pDX, IDC_TAB_PLANE, m_ctrlTab);
	DDX_Control(pDX, IDC_LIST_PLANE_NET, m_listPlaneNet);
	DDX_Control(pDX, IDC_LIST_PLANE_PIN, m_listPlanePin);
}


BEGIN_MESSAGE_MAP(CDlg_PCBBoardInfo, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_PLANE, OnTcnSelchangeTabPlane)
	ON_LBN_SELCHANGE(IDC_LIST_PLANE_NET, OnLbnSelchangeListPlaneNet)
END_MESSAGE_MAP()


// CDlg_PCBBoardInfo 消息处理程序

BOOL CDlg_PCBBoardInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	//图形对象统计
	char str[100];
	CWnd* pWnd;
	itoa(m_nTracks, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_TRACK);
	pWnd->SetWindowText(str);
	
	itoa(m_nArcs, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_ARC);
	pWnd->SetWindowText(str);
	
	itoa(m_nPads, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_PAD);
	pWnd->SetWindowText(str);
	
	itoa(m_nVias, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_VIA);
	pWnd->SetWindowText(str);
	
	itoa(m_nRegions, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_REGION);
	pWnd->SetWindowText(str);
	
	itoa(m_nTexts, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_TEXT);
	pWnd->SetWindowText(str);
	
	itoa(m_nFills, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_FILL);
	pWnd->SetWindowText(str);
	
	itoa(m_nCoordinates, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_COORDINATE);
	pWnd->SetWindowText(str);
	
	itoa(m_nDimensions, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_DIMENSION);
	pWnd->SetWindowText(str);
	
	itoa(m_nPolygons, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_POLYGON);
	pWnd->SetWindowText(str);
	
	itoa(m_nRoundHoles, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_HOLE);
	pWnd->SetWindowText(str);
	
	itoa(m_nSlotHoles, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_SLOT_HOLE);
	pWnd->SetWindowText(str);
	
	itoa(m_nSquareHoles, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_SQUARE_HOLE);
	pWnd->SetWindowText(str);

	itoa(m_nConnections, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_CONNECTION);
	pWnd->SetWindowText(str);

	itoa(m_nDRCViolations, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_DRC_VIOLATION);
	pWnd->SetWindowText(str);

	itoa(m_nComponents, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_COMPONENT);
	pWnd->SetWindowText(str);

	itoa(m_nTopComponents, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_COMPONENT_TOP);
	pWnd->SetWindowText(str);

	itoa(m_nBottomComponents, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_COMPONENT_BOTTOM);
	pWnd->SetWindowText(str);

	for(int i=0; i<m_arrayCompName.GetCount(); i++)
	{
		CString name = m_arrayCompName.GetAt(i);
		m_listComp.AddString(name);
	}
	m_listComp.SetColumnWidth(90);
	
	itoa(m_nNets, str, 10);
	pWnd = GetDlgItem(IDC_STATIC_NET);
	pWnd->SetWindowText(str);

	for(int i=0; i<m_arrayNetName.GetCount(); i++)
	{
		CString name = m_arrayNetName.GetAt(i);
		m_listNet.AddString(name);
	}
	m_listNet.SetColumnWidth(90);

	CESDDoc* pDoc= (CESDDoc*)pDocument;
	

	int layer = 0;
	for(int i=0; i<PCB_LAYER_NUM; i++)
	{
		if((layer >= POWERPLANE1)&&(layer <= POWERPLANE16)) 
		{
			m_arrayPlaneName.Add(pDoc->m_sPCB_LayerParam[layer].csLayerName);
			m_arrayPlaneLayer.Add(layer);
		}

		if(layer == BOTTOMLAYER) break;
		layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
	}

	if(m_arrayPlaneName.GetCount() <= 0)
	{
		m_ctrlTab.ShowWindow(FALSE);
		m_listPlaneNet.ShowWindow(FALSE);
		m_listPlanePin.ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_PLANE_NET);
		pWnd->ShowWindow(FALSE);
		pWnd = GetDlgItem(IDC_STATIC_PLANE_PIN);
		pWnd->ShowWindow(FALSE);
	}
	else
	{
		pWnd = GetDlgItem(IDC_STATIC_NOTFOUND);
		pWnd->ShowWindow(FALSE);

		for(int i=0; i<m_arrayPlaneName.GetCount(); i++) m_ctrlTab.InsertItem(i, m_arrayPlaneName.GetAt(i));
		
		SetPlaneNet();
	}
	


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlg_PCBBoardInfo::OnTcnSelchangeTabPlane(NMHDR *pNMHDR, LRESULT *pResult)
{
	SetPlaneNet();

	*pResult = 0;
}

void CDlg_PCBBoardInfo::SetPlaneNet()
{
	CESDDoc* pDoc= (CESDDoc*)pDocument;
	int layer = m_arrayPlaneLayer.GetAt(m_ctrlTab.GetCurSel());

	m_listPlaneNet.ResetContent();
	for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
		if((ppolygon->m_nLayer == layer)&&(ppolygon->m_nPolygonType == SPLITPLANE))
		{
			int net = ppolygon->m_nNet;
			if(net >= 0)
			{
				COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(net);
				if(pnet->m_nFlag == FLAG_NORMAL) m_listPlaneNet.AddString(pnet->m_csNetName);
			}
		}
	}

	if(m_listPlaneNet.GetCount() > 0)
	{
		CString netname;
		m_listPlaneNet.GetText(0, netname);
		SetPlanePin(netname);
	}
}

void CDlg_PCBBoardInfo::OnLbnSelchangeListPlaneNet()
{
	int index = m_listPlaneNet.GetCurSel();
	CString netname;
	m_listPlaneNet.GetText(index, netname);
	SetPlanePin(netname);
}

void CDlg_PCBBoardInfo::SetPlanePin(CString& netname)
{
	CESDDoc* pDoc= (CESDDoc*)pDocument;
	
	int layer = m_arrayPlaneLayer.GetAt(m_ctrlTab.GetCurSel());
	m_listPlanePin.ResetContent();
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		int net = ppad->m_nNet;
		if(net >= 0)
		{
			COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(net);

			if((pnet->m_csNetName == netname)&&(pnet->m_nFlag == FLAG_NORMAL))
			{
				CString compname;
				if(ppad->m_nComponent < 0) compname = "焊盘";
				else
				{
					COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
					compname = pcomp->GetPcbCompDesignator(ppad->m_nComponent);
				}

				CString pinname;
				if((ppad->m_nCCS != NO_CONNECT)&&(ppad->ConnectToInternalPlane(layer) == TRUE)&&(ppad->m_nLayer == MULTILAYER))
				{
					if(ppad->m_nCCS == RELIEF_CONNECT) pinname = "十字:";
					else if(ppad->m_nCCS == DIRECT_CONNECT) pinname = "直接:";
				}
				else  pinname = "未连接:";
				pinname += compname + "-";
				pinname += ppad->m_csName;
				m_listPlanePin.AddString(pinname);
			}
		}
	}
}
