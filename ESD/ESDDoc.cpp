// ESD.cpp : implementation of the CESDDoc class
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2007 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESD.h"
#include "io.h"

#include "mainfrm.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "ChildFrm.h"
#include "systemext.h"
#include "sysparam.h"
#include "struct.h"
#include ".\esddoc.h"

#include "Dlg_ReportElements.h"
#include "Dlg_Report_CompNet.h"
#include "Dlg_SchLib_Browse.h"

#include "Dlg_PCBBoardOptions.h"
#include "Dlg_PCBLayerStack.h"
#include "Dlg_PCBLayerSet.h"
#include "ReportCtrlItem.h"
#include "Dlg_PCBSnapSetup.h"
#include "Dlg_PCBBoardInfo.h"
#include "Dlg_PcbNetEdit.h"

#include "Page_PcbRules1.h"
#include "Page_PcbRules2.h"
#include "Page_PcbRules3.h"
#include "Page_PcbRules4.h"
#include "Page_PcbRules5.h"
#include "Page_PcbRules6.h"
#include "Page_PcbRules7.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CESDDoc

IMPLEMENT_DYNCREATE(CESDDoc, CDocument)

BEGIN_MESSAGE_MAP(CESDDoc, CDocument)
	//{{AFX_MSG_MAP(CESDDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_REPORT_ELEMENTS, OnReportElements)
	ON_COMMAND(ID_PCB_BOARD_OPTIONS, OnPcbBoardOptions)
	ON_COMMAND(ID_PCB_LAYER_STACK, OnPcbLayerStack)
	ON_COMMAND(ID_VIEW_GRIDS_TOGGLE_VISIBLE, OnViewGridsToggleVisible)
	ON_COMMAND(ID_VIEW_TOGGLE_UNITS, OnViewToggleUnits)
	ON_COMMAND(ID_PCB_VIEW_SNAP_SETUP, OnPcbViewSnapSetup)
	ON_COMMAND(ID_PCB_LAYER_SET, OnPcbLayerSet)
	ON_COMMAND(ID_REPORT_BOARD, OnReportBoard)
	//ON_COMMAND(ID_PCB_LIB_MAKE, OnPcbLibMake)
	//ON_COMMAND(ID_PCB_LIB_MAKELIB, OnPcbLibMakelib)
	ON_COMMAND(ID_PCB_NET_EDIT, OnPcbNetEdit)
	ON_COMMAND(ID_PCB_NET_CLEANALL, OnPcbNetCleanall)
	ON_COMMAND(ID_PCB_NET_CLEANONE, OnPcbNetCleanone)
	ON_COMMAND(ID_PCB_NET_CLEARALL, OnPcbNetClearall)
	ON_COMMAND(ID_REPORT_COMPNET, OnReportCompnet)
	ON_COMMAND(ID_PCB_DESIGN_RULES, OnPcbDesignRules)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CESDDoc construction/destruction

CESDDoc::CESDDoc()
{
	m_nDocumentType = DOC_ESD;
	m_nFileSaveType = m_nDocumentType;

	for(int i=0; i<SCH_SHEET_NUM; i++)
	{
		m_listSchObject[i].RemoveAll();

		m_arrayFont[i].RemoveAll();
		//array_font.SetSize(1);
		LOGFONT logfont;
		logfont.lfHeight = 10;
		logfont.lfWidth = 0;
		logfont.lfEscapement = logfont.lfOrientation = 0;
		logfont.lfWeight = FW_NORMAL;
		logfont.lfItalic = 0;
		logfont.lfUnderline = 0;
		logfont.lfStrikeOut = 0;
		logfont.lfCharSet = DEFAULT_CHARSET;
		logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logfont.lfQuality = PROOF_QUALITY;
		logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
		strcpy(logfont.lfFaceName, "Times New Roman");
		m_arrayFont[i].Add(logfont);

		m_fSCH_ScreenX[i] = 0;
		m_fSCH_ScreenY[i] = 0;
		m_fSCH_Scale[i] = 10;

		m_pSCH_Sheet[i] = new COb_schsheet();
		m_pSCH_Sheet[i]->m_nDiagram = i;
		m_pSCH_Sheet[i]->m_nStandardStyle = nEDA_SheetStandardStyle;

		char str1[100] = "SCH_";
		char str2[100];
		itoa(i+1, str2, 10);
		if( (i+1) < 10) 
		{
			strcat(str1,"0");
			strcat(str1,str2);
		}
		else strcat(str1,str2);
		m_csSCH_SheetName[i] = str1;
		
		m_bSCH_SheetShow[i] = FALSE;
	}

	m_bSCH_SheetShow[0] = TRUE;

	m_nSCH_Unit = nEDA_SCH_Default_Unit_Type;
	m_bSCH_VisibleGridShow = bEDA_SCH_VisibleGridShow;
	m_bSCH_SnapGridEnable = bEDA_SCH_SnapGridEnable;
	m_bSCH_ElecGridEnable = bEDA_SCH_ElecGridEnable;
	m_fSCH_VisibleGrid = fEDA_SCH_VisibleGrid;
	m_fSCH_ElecGrid = fEDA_SCH_ElecGrid;
	if((m_nSCH_Unit%2) == 0) m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_I[2];
	else  m_fSCH_ElecGrid = fEDA_SCH_SnapGrid_M[2];
	
	// UNDO
	m_nSCH_Undo_Num = 0;
	for(int i=0; i<SCH_UNDO_NUMBER; i++) m_listSchUndo[i].RemoveAll();

	//selection
	for(int i=0; i<SCH_SHEET_NUM; i++)
	{
		m_arraySelection[i].RemoveAll();
		m_fpSelection[i].fx = 0;
		m_fpSelection[i].fy = 0;
	}

	m_fPCB_ScreenX = 0;
	m_fPCB_ScreenY = 0;
	m_fPCB_Scale = 10;

	for(int i=0; i<PCB_LAYER_NUM; i++) 
	{
		m_sPCB_LayerParam[i].csLayerName = csEDA_PCB_LayerName[i];
		m_sPCB_LayerParam[i].bLayerShow = FALSE;
		m_sPCB_LayerParam[i].nLayerPrev = -1;
		m_sPCB_LayerParam[i].nLayerNext = -1;
		m_sPCB_LayerParam[i].bLayerMechEnabled = FALSE;
		m_sPCB_LayerParam[i].bLayerSheetLinked = FALSE;
		m_sPCB_LayerParam[i].csLayerCopThick = "1.4mil";
		m_sPCB_LayerParam[i].csLayerDielConst = "4.800";
		m_sPCB_LayerParam[i].csLayerDielHeight = "12.6mil";
		m_sPCB_LayerParam[i].csLayerDielMaterial = "FR-4";
		m_sPCB_LayerParam[i].csLayerDielType = "0";
	}

	m_sPCB_LayerParam[TOPLAYER].nLayerNext = BOTTOMLAYER;
	m_sPCB_LayerParam[BOTTOMLAYER].nLayerPrev = TOPLAYER;
	m_sPCB_LayerParam[MECHANICAL1].bLayerMechEnabled = TRUE;

	m_sPCB_LayerParam[TOPLAYER].bLayerShow = TRUE;
	m_sPCB_LayerParam[BOTTOMLAYER].bLayerShow = TRUE;
	m_sPCB_LayerParam[TOPOVERLAY].bLayerShow = TRUE;
	m_sPCB_LayerParam[KEEPOUTLAYER].bLayerShow = TRUE;
	m_sPCB_LayerParam[MECHANICAL1].bLayerShow = TRUE;
	m_sPCB_LayerParam[MULTILAYER].bLayerShow = TRUE;
	m_sPCB_LayerParam[CONNECTIONS].bLayerShow = TRUE;
	m_sPCB_LayerParam[DRCERROR].bLayerShow = TRUE;	
	m_sPCB_LayerParam[SELECTIONS].bLayerShow = TRUE;	
	m_sPCB_LayerParam[VISIBLEGRID2].bLayerShow = TRUE;
	m_sPCB_LayerParam[VISIBLEGRID1].bLayerShow = TRUE;
	m_sPCB_LayerParam[PADHOLES].bLayerShow = TRUE;
	m_sPCB_LayerParam[VIAHOLES].bLayerShow = TRUE;

	//m_arrayNet.RemoveAll();
	for(int i=0; i<POWER_LAYER_NUM; i++)
	{
		m_csPowerPlane[i].Empty();
		m_fPlanePullBack[i] = 20;  //20mil
	}


	InitLayerShowArray();

	Struct_PCBLayerSet layerset;
	layerset.m_csSetName = "所有层";  layerset.m_nActiveLayer = 0; layerset.m_bFlipBoard = FALSE;  layerset.m_bLocked = FALSE; layerset.m_bCurrentSelection = TRUE;
	for(int i=0; i<=MULTILAYER; i++) layerset.m_bLayerShow[i] = TRUE;
	m_arrayPCBLayerSet.Add(layerset);
	layerset.m_csSetName = "信号层";  layerset.m_nActiveLayer = 0; layerset.m_bFlipBoard = FALSE;  layerset.m_bLocked = FALSE; layerset.m_bCurrentSelection = FALSE;
	for(int i=0; i<=MULTILAYER; i++) layerset.m_bLayerShow[i] = FALSE;
	for(int i=TOPLAYER; i<=BOTTOMLAYER; i++) layerset.m_bLayerShow[i] = TRUE;
	layerset.m_bLayerShow[MULTILAYER] = TRUE;
	m_arrayPCBLayerSet.Add(layerset);
	layerset.m_csSetName = "内平面";  layerset.m_nActiveLayer = 0; layerset.m_bFlipBoard = FALSE;  layerset.m_bLocked = FALSE; layerset.m_bCurrentSelection = FALSE;
	for(int i=0; i<=MULTILAYER; i++) layerset.m_bLayerShow[i] = FALSE;
	for(int i=POWERPLANE1; i<=POWERPLANE16; i++) layerset.m_bLayerShow[i] = TRUE;
	m_arrayPCBLayerSet.Add(layerset);
	layerset.m_csSetName = "非信号层";  layerset.m_nActiveLayer = 0; layerset.m_bFlipBoard = FALSE;  layerset.m_bLocked = FALSE; layerset.m_bCurrentSelection = FALSE;
	for(int i=0; i<=MULTILAYER; i++) layerset.m_bLayerShow[i] = TRUE;
	for(int i=TOPLAYER; i<=BOTTOMLAYER; i++) layerset.m_bLayerShow[i] = FALSE;	
	for(int i=POWERPLANE1; i<=POWERPLANE16; i++) layerset.m_bLayerShow[i] = FALSE;
	m_arrayPCBLayerSet.Add(layerset);
	layerset.m_csSetName = "机械层";  layerset.m_nActiveLayer = 0; layerset.m_bFlipBoard = FALSE;  layerset.m_bLocked = FALSE; layerset.m_bCurrentSelection = FALSE;
	for(int i=0; i<=MULTILAYER; i++) layerset.m_bLayerShow[i] = FALSE;
	for(int i=MECHANICAL1; i<=MECHANICAL16; i++) layerset.m_bLayerShow[i] = TRUE;
	m_arrayPCBLayerSet.Add(layerset);

	m_nPCB_Undo_Num = 0;
	for(int i=0; i<PCB_UNDO_NUMBER; i++) m_listPcbUndo[i].RemoveAll();

	for(int i=0; i<10; i++)
	{
		m_fPCB_Marker[i].fx = 0;
		m_fPCB_Marker[i].fy = 0;
	}
	
	for(int i=0; i<8; i++) m_bSelectionMemoryLock[i] = FALSE;

	m_arrayPcbSelection.RemoveAll();
	m_fpPcbSelection.fx = 0;
	m_fpPcbSelection.fy = 0;	
	
	m_fPCBPrintX1 = 0;
	m_fPCBPrintY1 = 0;
	m_fPCBPrintX2 = 10000;
	m_fPCBPrintY2 = 8000;

	m_fArcPrecision = 0.5;

	m_fRulesClearance = 10.0f;
	m_fRulesTrackWidth = 12.0f;
	m_fRulesViaDia = 50.0f;
	m_fRulesViaHoleDia = 28.0f;
	m_nRulesCornerStyle = 0;
	m_fRulesCornerSetback = 100.0f;
	m_nRulesTopology = 0;
	for(int i=0; i<SIGNAL_LAYER_NUM; i++)
	{
		if((i%2) == 0) m_nRulesLayer[i] = HORIZONTAL_ROUTE;
		else m_nRulesLayer[i] = VERTICAL_ROUTE;
	}
	
	m_arrayRulesPriority.RemoveAll();


	m_fSolderMaskExpansion = 4;

	m_fPasteMaskExpansion = 0;

	m_fPlaneClearance = 20;

	m_nPlaneConnectStyle = 0;
	m_nPlaneConductors = 4;
	m_fPlaneConductorWidth = 10.0f;
	m_fPlaneAirGap = 10.0f;
	m_fPlaneExpansion = 20.0f;

	m_nPolygonConnectStyle = 0;
	m_nPolygonConductors = 4;
	m_fPolygonConductorWidth = 10.0f;

}

CESDDoc::~CESDDoc()
{
	for(int i=0; i<SCH_SHEET_NUM; i++)
	{
		if(m_pSCH_Sheet[i] != NULL) delete m_pSCH_Sheet[i];
		while (!m_listSchObject[i].IsEmpty())
		{
			delete m_listSchObject[i].RemoveHead();
		}
	}
	
	while (!m_arraySchNet.IsEmpty())
	{
		delete m_arraySchNet.GetAt(0);
        m_arraySchNet.RemoveAt(0);
	}	

	while (!m_arrayPcbArc.IsEmpty())
	{
		delete m_arrayPcbArc.GetAt(0);
        m_arrayPcbArc.RemoveAt(0);
	}
	while (!m_arrayPcbPad.IsEmpty())
	{
		delete m_arrayPcbPad.GetAt(0);
        m_arrayPcbPad.RemoveAt(0);
	}
	while (!m_arrayPcbVia.IsEmpty())
	{
		delete m_arrayPcbVia.GetAt(0);
        m_arrayPcbVia.RemoveAt(0);
	}
	while (!m_arrayPcbTrack.IsEmpty())
	{
		delete m_arrayPcbTrack.GetAt(0);
        m_arrayPcbTrack.RemoveAt(0);
	}
	while (!m_arrayPcbText.IsEmpty())
	{
		delete m_arrayPcbText.GetAt(0);
        m_arrayPcbText.RemoveAt(0);
	}
	while (!m_arrayPcbFill.IsEmpty())
	{
		delete m_arrayPcbFill.GetAt(0);
        m_arrayPcbFill.RemoveAt(0);
	}
	while (!m_arrayPcbRegion.IsEmpty())
	{
		delete m_arrayPcbRegion.GetAt(0);
        m_arrayPcbRegion.RemoveAt(0);
	}
	while (!m_arrayPcbCoordinate.IsEmpty())
	{
		delete m_arrayPcbCoordinate.GetAt(0);
        m_arrayPcbCoordinate.RemoveAt(0);
	}
	while (!m_arrayPcbDimension.IsEmpty())
	{
		delete m_arrayPcbDimension.GetAt(0);
        m_arrayPcbDimension.RemoveAt(0);
	}
	while (!m_arrayPcbComp.IsEmpty())
	{
		delete m_arrayPcbComp.GetAt(0);
		m_arrayPcbComp.RemoveAt(0);
	}
	while (!m_arrayPcbPolygon.IsEmpty())
	{
		delete m_arrayPcbPolygon.GetAt(0);
		m_arrayPcbPolygon.RemoveAt(0);
	}
	while (!m_arrayPcbNet.IsEmpty())
	{
		delete m_arrayPcbNet.GetAt(0);
		m_arrayPcbNet.RemoveAt(0);
	}
	while (!m_arrayPcbClass.IsEmpty())
	{
		delete m_arrayPcbClass.GetAt(0);
		m_arrayPcbClass.RemoveAt(0);
	}
	while (!m_arrayPcbConnection.IsEmpty())
	{
		delete m_arrayPcbConnection.GetAt(0);
		m_arrayPcbConnection.RemoveAt(0);
	}

	POSITION pos;
	CSchUndo* pundo;
	for(int j=0; j<SCH_UNDO_NUMBER; j++)
	{
		int num = m_listSchUndo[j].GetCount();
		pos = m_listSchUndo[j].GetHeadPosition();
		for(int i=0; i<num; i++)
		{
			if(pos == NULL) break;
			pundo = m_listSchUndo[j].GetNext(pos);
			delete pundo;
		}
		m_listSchUndo[j].RemoveAll();	
	}
	
	CPcbUndo* pPCBundo;
	for(int j=0; j<PCB_UNDO_NUMBER; j++)
	{
		int num = m_listPcbUndo[j].GetCount();
		pos = m_listPcbUndo[j].GetHeadPosition();
		for(int i=0; i<num; i++)
		{
			if(pos == NULL) break;
			pPCBundo = m_listPcbUndo[j].GetNext(pos);
			delete pPCBundo;
		}
		m_listPcbUndo[j].RemoveAll();	
	}
}


/////////////////////////////////////////////////////////////////////////////
// CESDDoc diagnostics

#ifdef _DEBUG
void CESDDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CESDDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CESDDoc commands

BOOL CESDDoc::OnNewDocument()
{
	// auto open the file which was edited most recently
	//if(nEDA_Lastfile_Flag == 0) return FALSE;

/*	int i;
	i=0;
	CMultiDocTemplate *pDocTemplate = (CMultiDocTemplate*)GetDocTemplate();
	if(pDocTemplate != NULL)
	{
		CESDDoc* pDoc;
		POSITION pos2 = pDocTemplate->GetFirstDocPosition();
		while(pos2 != NULL)
		{
			pDoc = (CESDDoc*)pDocTemplate->GetNextDoc(pos2);
			if(pDoc != NULL)
			{
				i++;
				if(i > MAXOPENFILENUMBER) 
				{
					return FALSE;
				}
			}
		}
	}

	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;*/

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


BOOL CESDDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
		char string[MAX_PATH];
	int l = strlen(lpszPathName);
	memcpy(string,lpszPathName,l);

	CString tempstr = lpszPathName;
	int i = tempstr.ReverseFind('.');

	if(i >= 0)	string[i] = '\0';
	strcat(string, ".bak");

		DeleteFile(string);

		MoveFile(lpszPathName,string);

		if(tempstr.Find(".dgn") >= 0)
	{
		if(i >= 0)	string[i] = '\0';
		strcat(string, ".eda");
		strcpy((char*)lpszPathName, string);
	}

	return CDocument::OnSaveDocument(lpszPathName);
}

/////////////////////////////////////////////////////////////////////////////
// CESDDoc serialization

void CESDDoc::Serialize(CArchive& ar)
{

	if (ar.IsStoring())
	{
		if(m_nFileSaveType == DOC_ESD)
		{
			TRACE2("保存:ESD Design File, DocType=%d SaveTyoe=%d\n",m_nDocumentType, m_nFileSaveType);
			Save_ESD(ar);
		}
		else if(m_nFileSaveType == DOC_AD6_SCH)
		{
			TRACE("保存:AD6 SCH ASCII File\n");
			Save_AD6_SCH_ASCII_File(ar, 0);
		}
		else if(m_nFileSaveType == DOC_AD6_PCB)
		{
			TRACE("保存:AD6 PCB ASCII File\n");
			Save_AD6_PCB_ASCII_File(ar);
		}
		else if(m_nFileSaveType == DOC_99_SCH)
		{
			TRACE("保存:99 SCH ASCII\n");
			Save_99_SCH_ASCII_File(ar, 0);
		}
		else if(m_nFileSaveType == DOC_99_PCB)
		{
			TRACE("保存:99 PCB ASCII\n");
			Save_99_PCB_ASCII_File(ar);
		}
		else
		{
			AfxMessageBox("文件格式无法识别, 保存文件失败.",MB_ICONERROR);
		}
	}
	else  
	{
		if(nEDA_System_State == SCH)
		{

			CString onelinestr;

			if(ar.ReadString(onelinestr) == TRUE)
			{
				if(onelinestr.Find(csESDFileHead_V1_1) >= 0) 
				{
					m_nDocumentType = DOC_ESD;
					m_nFileSaveType = m_nDocumentType;
					TRACE("打开:ESD Design File V1_1\n\n");
					Open_ESD(ar, 1);
				}
				else if(onelinestr.Find(csESDFileHead_V1_2) >= 0)
				{
					m_nDocumentType = DOC_ESD;
					m_nFileSaveType = m_nDocumentType;
					TRACE("打开:ESD Design File V1_2\n\n");
					Open_ESD(ar, 2);
				}
				else if(onelinestr.Find(csESDFileHead_V1_0) >= 0) 
				{
					m_nDocumentType = DOC_ESD;
					m_nFileSaveType = m_nDocumentType;
					TRACE("打开:ESD V1 Design File\n\n");
					Open_Eda_V1(ar);
				}
				else if(onelinestr.Find("|HEADER=Protel for Windows - Schematic Capture Ascii File Version 5.0") >= 0) 
				{
					m_nDocumentType = DOC_AD6_SCH;
					m_nFileSaveType = m_nDocumentType;
					int total_linenum;
					int pos = onelinestr.ReverseFind('=');
					if(pos >= 0)
					{
						pos = onelinestr.GetLength() - pos - 1;				
						onelinestr = onelinestr.Right(pos);
						total_linenum = atoi(onelinestr); 
					}
					else total_linenum = 0;
					if(total_linenum <= 0) 
					{
						AfxMessageBox("文件格式无法识别, 打开文件失败。", MB_ICONERROR);
						return;
					}

					TRACE("打开:AD6 SCH ASCII File\n\n");	
					Open_AD6_SCH_ASCII_File(ar, total_linenum, 0);
				}
				else if(onelinestr.Find("Protel for Windows - Schematic Capture Ascii File Version") >= 0)
				{
					m_nDocumentType = DOC_99_SCH;
					m_nFileSaveType = m_nDocumentType;
					TRACE("打开:99 SCH ASCII\n\n");
					Open_99_SCH_ASCII_File(ar, 0);
				}
				else if(onelinestr.Find("DXF") >= 0) m_nDocumentType = DOC_DXF;
				else
				{
					AfxMessageBox("文件格式无法识别, 打开文件失败。", MB_ICONERROR);
					return;
				}
			}
			else 
			{
				AfxMessageBox("文件格式无法识别, 打开文件失败。", MB_ICONERROR);
				return;
			}
		}
		else
		{
			CString onelinestr;
			if(ar.ReadString(onelinestr) == TRUE)
			{
				if(onelinestr.Find(csESDFileHead_V1_1) >= 0) 
				{
					m_nDocumentType = DOC_ESD;
					m_nFileSaveType = m_nDocumentType;
					TRACE("打开:ESD Design File v1_1\n");
					Open_ESD(ar, 1);
				}
				else if(onelinestr.Find(csESDFileHead_V1_2) >= 0) 
				{
					m_nDocumentType = DOC_ESD;
					m_nFileSaveType = m_nDocumentType;
					TRACE("打开:ESD Design File V1_2\n");
					Open_ESD(ar, 2);
				}
				else if(onelinestr.Find(csESDFileHead_V1_0) >= 0) 
				{
					m_nDocumentType = DOC_ESD;
					m_nFileSaveType = m_nDocumentType;
					TRACE("打开:EDA_V1 Design File\n");
					Open_Eda_V1(ar);
				}
				else if(onelinestr.Find("|KIND=Protel_Advanced_PCB|VERSION=5.00") >= 0) 
				{
					m_nDocumentType = DOC_AD6_PCB;
					m_nFileSaveType = m_nDocumentType;
					TRACE("打开:AD6 PCB ASCII File\n");
					Open_AD6_PCB_ASCII_File(ar, onelinestr);
				}
				else if(onelinestr.Find("|KIND=Protel_Advanced_PCB|VERSION=3.00") >= 0)
				{
					m_nDocumentType = DOC_99_PCB;
					m_nFileSaveType = m_nDocumentType;
					TRACE("打开:99 PCB ASCII\n");
					Open_99_PCB_ASCII_File(ar, onelinestr);
				}
				else if(onelinestr.Find("DXF") >= 0) m_nDocumentType = DOC_DXF;
				else
				{
					AfxMessageBox("文件格式无法识别, 打开文件失败。", MB_ICONERROR);
					return;
				}
			}
			else 
			{
				AfxMessageBox("文件格式无法识别, 打开文件失败。", MB_ICONERROR);
				return;
			}
		}
	}
}


void CESDDoc::OnFileSaveAs()
{
	static char BASED_CODE szFileFilter[1000];
	if(nEDA_System_State == SCH)
	{
		strcpy(szFileFilter, "	ESD (*.eda) |*.eda|\
								Altium Designer 6 SCH 5.0 (*.schdoc) |*.schdoc|\
								Protel 99(SE) Schematic (*.sch) |*.sch||");
	}
	else
	{
		strcpy(szFileFilter,"	ESD (*.eda) |*.eda|\
								Altium Designer 6 PCB 5.0 (*.pcbdoc) |*.pcbdoc|\
								Protel 99(SE) PCB 3.0 (*.pcb) |*.pcb||");
	}

	char defaultfilename[_MAX_PATH];
	CString csDefaultfilename = GetTitle();
	int p = csDefaultfilename.ReverseFind('.');
	csDefaultfilename = csDefaultfilename.Left(p);
	strcpy(defaultfilename, csDefaultfilename);

	CFileDialog importdialog(FALSE,"",defaultfilename,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_OVERWRITEPROMPT,szFileFilter,NULL);  //OFN_ALLOWMULTISELECT
	
	if(nEDA_System_State == SCH)
	{
		if(m_nDocumentType == DOC_ESD) importdialog.m_ofn.nFilterIndex = 1;
		else if(m_nDocumentType == DOC_AD6_SCH) importdialog.m_ofn.nFilterIndex = 2;
		else if(m_nDocumentType == DOC_99_SCH) importdialog.m_ofn.nFilterIndex = 3;
	}
	else
	{
		if(m_nDocumentType == DOC_ESD) importdialog.m_ofn.nFilterIndex = 1;
		else if(m_nDocumentType == DOC_AD6_PCB) importdialog.m_ofn.nFilterIndex = 2;
		else if(m_nDocumentType == DOC_99_PCB) importdialog.m_ofn.nFilterIndex = 3;
	}

	char filename[MAX_PATH];
	if(importdialog.DoModal() == IDOK) 
	{
		if(nEDA_System_State == SCH)
		{
			if(importdialog.m_ofn.nFilterIndex == 1) m_nFileSaveType = DOC_ESD;
			else if(importdialog.m_ofn.nFilterIndex == 2) m_nFileSaveType = DOC_AD6_SCH;
			else if(importdialog.m_ofn.nFilterIndex == 3) m_nFileSaveType = DOC_99_SCH;
		}
		else
		{
			if(importdialog.m_ofn.nFilterIndex == 1) m_nFileSaveType = DOC_ESD;
			else if(importdialog.m_ofn.nFilterIndex == 2) m_nFileSaveType = DOC_AD6_PCB;
			else if(importdialog.m_ofn.nFilterIndex == 3) m_nFileSaveType = DOC_99_PCB;
		}
		
		strcpy(filename, importdialog.GetPathName());
		CString newName = filename;

		if((m_nDocumentType == DOC_ESD)&&((m_nFileSaveType == DOC_AD6_SCH)||(m_nFileSaveType == DOC_99_SCH)))
		{
			CESDApp* pApp = (CESDApp*)AfxGetApp();
			CESDView* pView = (CESDView*)pApp->GetCurrentView();
			int i;
			for(i=0; i<SCH_SHEET_NUM; i++)
			{
				if( ( Get_SCH_Elements_Param(0, i, 0) > 0)&&( i != pView->m_nDiagram) ) break;
			}
			if( (i < SCH_SHEET_NUM)||( Get_PCB_Object(0) > 0) )
			{
				CFile myfile;
				if(myfile.Open(filename, CFile::modeWrite|CFile::modeCreate) == TRUE)
				{
					CArchive ar(&myfile, CArchive::store);
					if(m_nFileSaveType == DOC_AD6_SCH)	Save_AD6_SCH_ASCII_File(ar, pView->m_nDiagram);
					else if(m_nFileSaveType == DOC_99_SCH)	Save_99_SCH_ASCII_File(ar, pView->m_nDiagram);
					ar.Flush();
					myfile.Close();
				}
				else AfxMessageBox("无法打开文件。", MB_ICONSTOP);
				return;
			}
		}
		else if((m_nDocumentType == DOC_ESD)&&((m_nFileSaveType == DOC_AD6_PCB)||(m_nFileSaveType == DOC_99_PCB)))
		{
			CFile myfile;
			if(myfile.Open(filename, CFile::modeWrite|CFile::modeCreate) == TRUE)
			{
				CArchive ar(&myfile, CArchive::store);
				if(m_nFileSaveType == DOC_AD6_PCB)	Save_AD6_PCB_ASCII_File(ar);
				else if(m_nFileSaveType == DOC_99_PCB) Save_99_PCB_ASCII_File(ar);
			}
			else AfxMessageBox("无法打开文件。", MB_ICONSTOP);
			return;
		}

		if (!OnSaveDocument(newName))
		{
			AfxMessageBox("保存文件错误");
			// be sure to delete the file
			TRY
			{
				CFile::Remove(newName);
			}
			CATCH_ALL(e)
			{
				TRACE(traceAppMsg, 0, "Warning: failed to delete file after failed SaveAs.\n");
				//DELETE_EXCEPTION(e);
			}
			END_CATCH_ALL
		}

		m_nDocumentType = m_nFileSaveType;

		// reset the title and change the document name
		SetPathName(newName);
	}
}

BOOL CESDDoc::Get_KeyWord_Data(CString& oneline, CString& keyword, CString& data)
{ 
	int pos;
	CString str = oneline;
	pos = str.Find(keyword);
	if(pos < 0) return FALSE;
	pos = str.GetLength() - pos;
	data = str.Right(pos);

	pos = data.GetLength() - keyword.GetLength();
	data = data.Right(pos);
	
	pos = data.Find('|'); 
	if(pos >= 0) data = data.Left(pos);
	
	return TRUE;
}

BOOL CESDDoc::Get_Text_Data(CString& oneline, CString& keyword, CString& data)
{          
	int pos;
	CString str = oneline;
	pos = str.Find(keyword);
	if(pos < 0) return FALSE;
	pos = str.GetLength() - pos;
	data = str.Right(pos);

	pos = data.GetLength() - keyword.GetLength();
	data = data.Right(pos);
	
	pos = data.Find("|WIDTH=");
	if(pos >= 0) data = data.Left(pos);
	
	return TRUE;
}

int CESDDoc::Divide_l(char *string, char s123[200][200])
{ 
	int i=0,j=0,k=0,flag;
	flag=1;
	for(;;)
	{
		if(string[i]=='\0') 
		{
			s123[j][k]=0; 
			j++; 
			break; 
		}
		if((string[i]==0x0d)||(string[i]==0x0a))
		{
			i++;
			continue;
		}
		if((string[i]==0x20)||(string[i] == 0x09))
		{
			if(flag==0) 
			{
				flag=1;
				s123[j][k]=0;
				j++;
				if(j >= 200) break;
				k=0;
			}
			else if(flag==1)
			{
			}
			i++;
			continue;
		}
		else
		{
			flag = 0;
			s123[j][k] = string[i];
			i++; 
			k++;
		}
	}
	return j;
}

int CESDDoc::Divide_long(char *string, char s123[200][200])
{ 
	int i=0,j=0,k=0,flag, sw;
	flag=1;
	sw = 0;
	for(;;)
	{
		if(string[i]=='\0') 
		{
			s123[j][k]=0; 
			j++; 
			break; 
		}
		if((string[i]==0x0d)||(string[i]==0x0a))
		{
			i++;
			continue;
		}
		if(((string[i]==0x20)||(string[i] == 0x09))&&(sw == 0))
		{
			if(flag==0) 
			{
				flag=1;
				s123[j][k]=0;
				j++;
				if(j >= 200) break;
				k=0;
			}
			else if(flag==1)
			{
			}
			i++;
			continue;
		}
		else
		{
			if(string[i] == 0x27) sw = (sw+1)&0x01;
			flag = 0;
			s123[j][k] = string[i];
			i++; 
			k++;
		}
	}
	return j;
}

void CESDDoc::Get_Rotate_XY(double* fx, double* fy, double old_fx, double old_fy, int orientation, BOOL mirrored)
{
	int state;
	state = (mirrored == TRUE? 1 : 0)*4 + orientation/90;
	*fx = fEDA_Rotate_XY[state][0]*old_fx + fEDA_Rotate_XY[state][1]*old_fy;
	*fy = fEDA_Rotate_XY[state][2]*old_fx + fEDA_Rotate_XY[state][3]*old_fy;
}

void CESDDoc::Get_Angle_XY(double* dx, double* dy, double old_dx, double old_dy, double angle)
{
	if((old_dx == 0)&&(old_dy == 0))
	{
		*dx = 0; *dy = 0;
	}
	else
	{
		double alpha, belta;
		if(old_dy == 0)
		{
			if(old_dx > 0) alpha = 0;
			else alpha = 180;
		}
		else if(old_dx == 0)
		{
			if(old_dy > 0) alpha = 90;
			else alpha = -90;
		}
		else alpha = atan2(old_dy, old_dx)*180/fEDA_pi;

		belta = alpha + angle;
		*dx = sqrt(old_dx*old_dx + old_dy*old_dy)*cos(belta*fEDA_pi/180);
		*dy = sqrt(old_dx*old_dx + old_dy*old_dy)*sin(belta*fEDA_pi/180);
	}
}

void CESDDoc::Get_Rotate_Orientation(int* new_o, int old_o, int orientation, BOOL mirrored)
{
	if(mirrored == FALSE)
	{
		*new_o = (old_o + orientation)%360;
	}
	else
	{
		int i;
		i = (old_o + orientation)%360;
		if(i == 0) *new_o = 180;
		else if(i == 180) *new_o = 0;
		else *new_o = i;
	}
}

void CESDDoc::Get_Rotate_Angle(double* sangle, double* eangle, double old_sangle, double old_eangle, int orientation, BOOL mirrored)
{
	double sa, ea;

	if(mirrored == FALSE)
	{
		sa = old_sangle + orientation;
		if(sa >= 360) sa -= 360;
		ea = old_eangle + orientation;
		if(ea >= 360) ea -= 360;
	}
	else
	{
		sa = old_eangle + orientation;
		if(sa >= 360) sa -= 360;
		ea = old_sangle + orientation;
		if(ea >= 360) ea -= 360;
		sa = 180 - sa;
		if(sa < 0) sa += 360;
		ea = 180 - ea;
		if(ea < 0) ea += 360;
	}
	*sangle = sa;
	*eangle = ea;
}

int CESDDoc::ReadNetString(CFile * pFile, char * lpbuf)
{
	int i,j;
	char c[2];
	i = 0;
	for(;;)
	{
		j = pFile->Read(c,1);
		
		if(j<=0) 
		{
			lpbuf[i] = '\0';
			return -1;
		}
		if(c[0] == 0x0A) 
		{
			lpbuf[i] = '\0';
			return 0;
		}
		else lpbuf[i] = c[0];
		i++;
		if(i>80) return -1;
	}
}

void CESDDoc::Convert_LayerNameToChinese()
{
	for(int i=0; i<PCB_LAYER_NUM; i++)
	{
		m_sPCB_LayerParam[i].csLayerName = csEDA_PCB_LayerName[i];
	}
}

void CESDDoc::Convert_LayerNameToEnglish()
{
	for(int i=0; i<PCB_LAYER_NUM; i++)
	{
		m_sPCB_LayerParam[i].csLayerName = csEDA_PCB_EnglishLayerName[i];
	}
}

int CESDDoc::EDA_V1_ESD_PCBLayerConvert(int oldlayer)
{
	//#define  TOPLAYER       0
	//#define  MIDLAYER1      1
	//#define  MIDLAYER2      2
	//#define  MIDLAYER3      3
	//#define  MIDLAYER4      4
	//#define  BOTTOMLAYER    5
	//#define  TOPOVERLAY     6
	//#define  BOTTOMOVERLAY  7
	//#define  GROUNDLAYER    8
	//#define  POWERLAYER     9
	//#define  BOARDLAYER    10  -------> Mechanical 1
	//#define  KEEPOUTLAYER  11
	//#define  TOPSOLDER     12
	//#define  BOTTOMSOLDER  13
	//#define  TOPPASTE		 14
	//#define  BOTTOMPASTE   15
	//#define  MULTILAYER    16
	int newlayer = MECHANICAL1;
	for(int i=0; i<5; i++) //TOP, Mid1-4
	{
		if(oldlayer == i) newlayer = i;
	}
	if(oldlayer == 5) newlayer = BOTTOMLAYER;
	if(oldlayer == 6) newlayer = TOPOVERLAY;
	if(oldlayer == 7) newlayer = BOTTOMOVERLAY;
	if(oldlayer == 8) newlayer = POWERPLANE1;
	if(oldlayer == 9) newlayer = POWERPLANE2;
	if(oldlayer == 10) newlayer = MECHANICAL1;
	if(oldlayer == 11) newlayer = KEEPOUTLAYER;
	if(oldlayer == 12) newlayer = TOPSOLDER;
	if(oldlayer == 13) newlayer = BOTTOMSOLDER;
	if(oldlayer == 14) newlayer = TOPPASTE;
	if(oldlayer == 15) newlayer = BOTTOMPASTE;
	if(oldlayer == 16) newlayer = MULTILAYER;

	return newlayer;
}

int CESDDoc::CheckFontExist(LOGFONT* logfont, int diagram)
{
	for(int i=0; i<m_arrayFont[diagram].GetCount(); i++)
	{
		LOGFONT font;
		font = m_arrayFont[diagram].GetAt(i);
		if(	(stricmp(font.lfFaceName,logfont->lfFaceName) ==0)&&\
			(font.lfHeight == logfont->lfHeight)&&\
			(font.lfOrientation == logfont->lfOrientation)&&\
			(font.lfWeight == logfont->lfWeight)&&\
			(font.lfItalic == logfont->lfItalic)&&\
			(font.lfStrikeOut == logfont->lfStrikeOut)&&\
			(font.lfUnderline == logfont->lfUnderline) ) return i;
	}

	return -1;
}

void CESDDoc::Get_Document_Maxy(double* fmax_x, double* fmax_y, double* fmin_x, double* fmin_y)
{
	COb_pcbarc* parc;
	COb_pcbpad* ppad;
	COb_pcbvia* pvia;
	COb_pcbtrack* ptrack;
	COb_pcbtext* ptext;
	COb_pcbfill* pfill;
	COb_pcbregion* pregion;
	COb_pcbcoordinate* pcoordinate;
	COb_pcbdimension* pdimension;

	int num;

	double fmaxx = 0, fmaxy = 0;
	double fminx = 100000, fminy = 100000;

	num = m_arrayPcbCoordinate.GetCount();
	for(int i=0; i<num; i++)
	{	
		pcoordinate = m_arrayPcbCoordinate.GetAt(i);
		if(pcoordinate->m_nFlag == FLAG_DELETED)	continue;
		fmaxx = MAX(fmaxx, pcoordinate->m_fX + pcoordinate->m_fSize);
		fmaxy = MAX(fmaxy, pcoordinate->m_fY + pcoordinate->m_fSize);
		fminx = MIN(fminx, pcoordinate->m_fX - pcoordinate->m_fSize);
		fminy = MIN(fminy, pcoordinate->m_fY - pcoordinate->m_fSize);
	}

	num = m_arrayPcbDimension.GetCount();
	for(int i=0; i<num; i++)
	{	
		pdimension = m_arrayPcbDimension.GetAt(i);
		if(pdimension->m_nFlag == FLAG_DELETED)	continue;
		fmaxx = MAX(fmaxx, pdimension->m_fXe);
		fmaxy = MAX(fmaxy, pdimension->m_fYe);
		fminx = MIN(fminx, pdimension->m_fXs);
		fminy = MIN(fminy, pdimension->m_fYs);
	}

	num = m_arrayPcbTrack.GetCount();
	for(int i=0; i<num; i++)
	{	
		ptrack = m_arrayPcbTrack.GetAt(i);
		if(ptrack->m_nFlag == FLAG_DELETED)	continue;
		fmaxx = MAX(fmaxx, ptrack->m_fX1);
		fmaxy = MAX(fmaxy, ptrack->m_fY1);
		fminx = MIN(fminx, ptrack->m_fX1);
		fminy = MIN(fminy, ptrack->m_fY1);
		fmaxx = MAX(fmaxx, ptrack->m_fX2);
		fmaxy = MAX(fmaxy, ptrack->m_fY2);
		fminx = MIN(fminx, ptrack->m_fX2);
		fminy = MIN(fminy, ptrack->m_fY2);
	}

	num = m_arrayPcbArc.GetCount();
	for(int i=0; i<num; i++)
	{	
		parc = m_arrayPcbArc.GetAt(i);
		if(parc->m_nFlag == FLAG_DELETED)	continue;
		fmaxx = MAX(fmaxx, parc->m_fCx + parc->m_fRadius);
		fmaxy = MAX(fmaxy, parc->m_fCy + parc->m_fRadius);
		fminx = MIN(fminx, parc->m_fCx - parc->m_fRadius);
		fminy = MIN(fminy, parc->m_fCy - parc->m_fRadius);
	}

	num = m_arrayPcbText.GetCount();
	for(int i=0; i<num; i++)
	{	
		ptext = m_arrayPcbText.GetAt(i);
		if(ptext->m_nFlag == FLAG_DELETED)	continue;
		fmaxx = MAX(fmaxx, ptext->m_fSx + ptext->m_fHeight*ptext->m_csText.GetLength());
		fmaxy = MAX(fmaxy, ptext->m_fSy + ptext->m_fHeight*ptext->m_csText.GetLength());
		fminx = MIN(fminx, ptext->m_fSx + ptext->m_fHeight*ptext->m_csText.GetLength());
		fminy = MIN(fminy, ptext->m_fSy + ptext->m_fHeight*ptext->m_csText.GetLength());
	}

	num = m_arrayPcbRegion.GetCount();
	for(int i=0; i<num; i++)
	{	
		pregion = m_arrayPcbRegion.GetAt(i);
		if(pregion->m_nFlag == FLAG_DELETED)	continue;
		for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
		{
			fmaxx = MAX(fmaxx, pregion->m_cVertex[i].fx);
			fmaxy = MAX(fmaxy, pregion->m_cVertex[i].fy);
			fminx = MIN(fminx, pregion->m_cVertex[i].fx);
			fminy = MIN(fminy, pregion->m_cVertex[i].fy);
		}
	}

	num = m_arrayPcbFill.GetCount();
	for(int i=0; i<num; i++)
	{	
		pfill = m_arrayPcbFill.GetAt(i);
		if(pfill->m_nFlag == FLAG_DELETED)	continue;
		fmaxx = MAX(fmaxx, pfill->m_fX1);
		fmaxy = MAX(fmaxy, pfill->m_fY1);
		fminx = MIN(fminx, pfill->m_fX1);
		fminy = MIN(fminy, pfill->m_fY1);
		fmaxx = MAX(fmaxx, pfill->m_fX2);
		fmaxy = MAX(fmaxy, pfill->m_fY2);
		fminx = MIN(fminx, pfill->m_fX2);
		fminy = MIN(fminy, pfill->m_fY2);
	}

	num = m_arrayPcbPad.GetCount();
	for(int i=0; i<num; i++)
	{	
		ppad = m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag == FLAG_DELETED)	continue;
		fmaxx = MAX(fmaxx, ppad->m_fCx);
		fmaxy = MAX(fmaxy, ppad->m_fCy);
		fminx = MIN(fminx, ppad->m_fCx);
		fminy = MIN(fminy, ppad->m_fCy);
	}

	num = m_arrayPcbVia.GetCount();
	for(int i=0; i<num; i++)
	{	
		pvia = m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag == FLAG_DELETED)	continue;
		fmaxx = MAX(fmaxx, pvia->m_fCx);
		fmaxy = MAX(fmaxy, pvia->m_fCy);
		fminx = MIN(fminx, pvia->m_fCx);
		fminy = MIN(fminy, pvia->m_fCy);
	}

	*fmax_x = fmaxx + 400; if(*fmax_x > 99999) *fmax_x = 99999;
	*fmax_y = fmaxy + 400; if(*fmax_y > 99999) *fmax_y = 99999;
	*fmin_x = fminx - 400; if(*fmin_x < 0) *fmin_x = 0;
	*fmin_y = fminy - 400; if(*fmin_y < 0) *fmin_y = 0;
}


int CESDDoc::GetSchSheetUsed()
{
	int sheetused = 0;

	for(int i=0; i<SCH_SHEET_NUM; i++)
	{
		if(m_listSchObject[i].GetCount() != 0)
		{
			sheetused = sheetused|(0x01<<i);
		}
	}

	return sheetused;
}

CString CESDDoc::Convert_XY_To_String(double fxy)
{
	CString cstring;
    int ndig = 10;
    int dec,sign;
	char xx[100],*str;
	int flag = 0;

	if(nEDA_System_State == SCH)
	{
		if((m_nSCH_Unit == UNIT_MIL)||(m_nSCH_Unit == UNIT_INCH)||(m_nSCH_Unit == UNIT_DXP)||(m_nSCH_Unit == UNIT_AUTO_I))
		{

				 if(m_nSCH_Unit == UNIT_MIL ) fxy = fxy;
			else if(m_nSCH_Unit == UNIT_INCH) fxy = fxy/1000;
			else if(m_nSCH_Unit == UNIT_DXP ) fxy = fxy/10;
			else
			{	
				if(fxy < 500.0f) fxy = fxy;
				else 
				{
					fxy = fxy/1000;
					flag = 1;
				}
			}

			str = _fcvt(fxy, 3, &dec, &sign); 

			if(strlen(str) > 0)
			{
				if(dec<=0)
				{
					if(dec == 0) strcpy(xx, "0.");
					else if(dec == -1) strcpy(xx, "0.0");
					else if(dec == -2) strcpy(xx, "0.00");
					else strcpy(xx, "0.000");
					strcat(xx, str);
				}
				else
				{
					for(int i=0; i<dec; i++)
					{
						xx[i] = str[i];
						xx[i+1] = NULL;
					}
					if((int)strlen(str) > dec )
					{
						strcat(xx,".");
						strcat(xx, str+dec);
					}
				}
			}
			else strcpy(xx, NULL);
			
			cstring = xx;

			int dot = cstring.Find(".");

			if(dot >= 0) 
			{
				cstring.TrimRight('0');
				cstring.TrimRight('.');
			}
			if(cstring.IsEmpty() == TRUE) cstring = "0";

				 if(m_nSCH_Unit == UNIT_MIL ) cstring = cstring + "mil";
			else if(m_nSCH_Unit == UNIT_INCH) cstring = cstring + "in";
			else if(m_nSCH_Unit == UNIT_DXP ) {}
			else
			{	
				if(flag == 0) cstring = cstring + "mil";
				else cstring = cstring + "in";
			}
		}
		else
		{
				 if(m_nSCH_Unit == UNIT_MM) fxy = fxy*25.4/1000;
			else if(m_nSCH_Unit == UNIT_CM) fxy = fxy*25.4/10000;
			else if(m_nSCH_Unit == UNIT_M ) fxy = fxy*25.4/1000000;
			else
			{	
				fxy = fxy*25.4/1000;
				if(fxy < 100.0f) fxy = fxy;
				else if( (fxy>= 100.0f)&&(fxy <1000.0f) )
				{
					fxy = fxy/10;
					flag = 1;
				}
				else
				{
					fxy = fxy/1000;
					flag = 2;
				}
			}

			str = _fcvt(fxy, 2, &dec, &sign); 

			if(strlen(str) > 0)
			{
				if(dec<=0)
				{
					if(dec == 0) strcpy(xx, "0.");
					else if(dec == -1) strcpy(xx, "0.0");
					else strcpy(xx, "0.00");
					strcat(xx, str);
				}
				else
				{
					for(int i=0; i<dec; i++)
					{
						xx[i] = str[i];
						xx[i+1] = NULL;
					}
					if((int)strlen(str) > dec )
					{
						strcat(xx,".");
						strcat(xx, str+dec);
					}
				}
			}
			else strcpy(xx, NULL);
			
			cstring = xx;

			int dot = cstring.Find(".");

			if(dot >= 0) 
			{
				cstring.TrimRight('0');
				cstring.TrimRight('.');
			}
			if(cstring.IsEmpty() == TRUE) cstring = "0";

				 if(m_nSCH_Unit == UNIT_MM ) cstring = cstring + "mm";
			else if(m_nSCH_Unit == UNIT_CM ) cstring = cstring + "cm";
			else if(m_nSCH_Unit == UNIT_M  ) cstring = cstring + "m";
			else
			{	
				if(flag == 0) cstring = cstring + "mm";
				else if(flag == 1) cstring = cstring + "cm";
				else cstring = cstring + "m";
			}
		}
	}
	else
	{
		if(m_cPCB_Sheet.m_nUnit == UNIT_PCB_M)
		{
			fxy = fxy*25.4/1000;

			str = _fcvt(fxy, 3, &dec, &sign); 

			if(strlen(str) > 0)
			{
				if(dec<=0)
				{
					if(dec == 0) strcpy(xx, "0.");
					else if(dec == -1) strcpy(xx, "0.0");
					else strcpy(xx, "0.00");
					strcat(xx, str);
				}
				else
				{
					for(int i=0; i<dec; i++)
					{
						xx[i] = str[i];
						xx[i+1] = NULL;
					}
					if((int)strlen(str) > dec )
					{
						strcat(xx,".");
						strcat(xx, str+dec);
					}
				}
			}
			else strcpy(xx, NULL);
			
			cstring = xx;

			int dot = cstring.Find(".");

			if(dot >= 0) 
			{
				cstring.TrimRight('0');
				cstring.TrimRight('.');
			}
			if(cstring.IsEmpty() == TRUE) cstring = "0";

			cstring = cstring + "mm";
		}
		else
		{
			fxy = fxy;

			str = _fcvt(fxy, 3, &dec, &sign); 

			if(strlen(str) > 0)
			{
				if(dec<=0)
				{
					if(dec == 0) strcpy(xx, "0.");
					else if(dec == -1) strcpy(xx, "0.0");
					else if(dec == -2) strcpy(xx, "0.00");
					else strcpy(xx, "0.000");
					strcat(xx, str);
				}
				else
				{
					for(int i=0; i<dec; i++)
					{
						xx[i] = str[i];
						xx[i+1] = NULL;
					}
					if((int)strlen(str) > dec )
					{
						strcat(xx,".");
						strcat(xx, str+dec);
					}
				}
			}
			else strcpy(xx, NULL);
			
			cstring = xx;

			int dot = cstring.Find(".");

			if(dot >= 0) 
			{
				cstring.TrimRight('0');
				cstring.TrimRight('.');
			}
			if(cstring.IsEmpty() == TRUE) cstring = "0";

			cstring = cstring + "mil";
		}
	}
    
	if(fxy < 0) cstring = "-" + cstring;
	return cstring;
}

double CESDDoc::Convert_String_To_XY(CString& cstring)
{
	double fxy = 0;

	cstring.MakeLower();
	if(cstring.Find("mil") > 0)
	{
		cstring.TrimRight("milcnh");
		fxy = atof(cstring);
	}
	else if((cstring.Find("in") > 0)||(cstring.Find("inch") > 0))
	{
		cstring.TrimRight("milcnh");
		fxy = atof(cstring);
		fxy = fxy*1000;
	}
	else if(cstring.Find("mm") > 0)
	{
		cstring.TrimRight("milcnh");
		fxy = atof(cstring);
		fxy = fxy*1000/25.4;
	}
	else if(cstring.Find("cm") > 0)
	{
		cstring.TrimRight("milcnh");
		fxy = atof(cstring);
		fxy = fxy*10000/25.4;
	}
	else if(cstring.Find("m") > 0)
	{
		cstring.TrimRight("milcnh");
		fxy = atof(cstring);
		fxy = fxy*1000000/25.4;
	}
	else
	{
		if(nEDA_System_State == SCH)
		{
			if((m_nSCH_Unit == UNIT_MIL)||(m_nSCH_Unit == UNIT_INCH)||(m_nSCH_Unit == UNIT_DXP)||(m_nSCH_Unit == UNIT_AUTO_I))
			{
				if(m_nSCH_Unit == UNIT_MIL )
				{
					cstring.TrimRight("milcnh");
					fxy = atof(cstring);
				}
				else if(m_nSCH_Unit == UNIT_INCH)
				{
					cstring.TrimRight("milcnh");
					fxy = atof(cstring);
					fxy = fxy*1000;
				}
				else if(m_nSCH_Unit == UNIT_DXP )
				{
					cstring.TrimRight("milcnh");
					fxy = atof(cstring);
					fxy = fxy*10;
				}
				else
				{	
					cstring.TrimRight("milcnh");
					fxy = atof(cstring);
				}
			}
			else
			{
				if(m_nSCH_Unit == UNIT_MM)
				{
					cstring.TrimRight("milcnh");
					fxy = atof(cstring);
					fxy = fxy*1000/25.4;
				}
				else if(m_nSCH_Unit == UNIT_CM)
				{
					cstring.TrimRight("milcnh");
					fxy = atof(cstring);
					fxy = fxy*10000/25.4;
				}
				else if(m_nSCH_Unit == UNIT_M )
				{
					cstring.TrimRight("milcnh");
					fxy = atof(cstring);
					fxy = fxy*1000000/25.4;
				}
				else
				{	
					cstring.TrimRight("milcnh");
					fxy = atof(cstring);
					fxy = fxy*1000/25.4;
				}
			}
		}
	}

	if(fxy > 99999.999f) fxy = 99999.999f;
	if(fxy < -99999.999f) fxy = -99999.999f;
	return fxy;
}

double CESDDoc::Convert_PCB_String_To_XY(CString& cstring)
{
	if((cstring.Find("mm") < 0)&&(cstring.Find("mil") < 0))
	{
		if(m_cPCB_Sheet.m_nUnit == UNIT_PCB_M) cstring += "mm";
		else cstring += "mil";
	}
	double f = Convert_String_To_XY(cstring);
	return f;
}

int CESDDoc::Get_SCH_Elements_Param(int kind, int nDiagram, int type)   
{
	CObject* pobject;
	int sch_objects_num = 0;
	int sch_ad6_linenum = 0;
	int sch_elements_num = 0;

	int num = m_listSchObject[nDiagram].GetCount();
	POSITION pos;
	pos = m_listSchObject[nDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{	
		if(pos == NULL) break;
		pobject = m_listSchObject[nDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if(kind == 0) sch_objects_num++;
			else if( (kind == SCH_ELEMENT_WIRE) && (pline->m_nAttrib == LINE_WIRE)) sch_objects_num++;
			else if( (kind == SCH_ELEMENT_BUS) && (pline->m_nAttrib == LINE_BUS)) sch_objects_num++;
			else if( (kind == SCH_ELEMENT_BUSENTRY) && (pline->m_nAttrib == LINE_BUS_ENTRY))  sch_objects_num++;
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_POLYLINE))sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_JUNCTION )) sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++; 
			sch_elements_num++;
			if(kind == 0) 
			{
				if((ptext->m_nID == TEXT_TEXT)||(ptext->m_nID == TEXT_NETLABEL)) sch_objects_num++;  
			}
			else if((kind == SCH_ELEMENT_TEXT) && (ptext->m_nID == TEXT_TEXT)) sch_objects_num++;
			else if((kind == SCH_ELEMENT_NETLABEL) && (ptext->m_nID == TEXT_NETLABEL)) sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_ARC)) sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_ELLIPSE)) sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_ELLIPSEFILL)) sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_PIE)) sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_POLYGON)) sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_TEXTFRAME)) sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_RECT)) sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_ROUNDRECT)) sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;
			if((psheetsymbol->m_nDiagram != nDiagram)||(psheetsymbol->m_nFlag == FLAG_DELETED))
			{
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) m_listSchObject[nDiagram].GetNext(pos);
				continue;
			}
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_SHEETSYMBOL)) sch_objects_num++;

			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
			{
				pobject = m_listSchObject[nDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
				{
					COb_schsheetentry* psheetentry;
					psheetentry = (COb_schsheetentry*)pobject;
					if((psheetentry->m_nDiagram != nDiagram)||(psheetentry->m_nFlag == FLAG_DELETED))	continue;

					sch_ad6_linenum++;
					sch_elements_num++;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext;
					ptext = (COb_schtext*)pobject;
					sch_ad6_linenum++; 
					sch_elements_num++;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_PORT)) sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_POWER)) sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_BEZIER)) sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			
			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_GRAPHIC)) sch_objects_num++;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram != nDiagram)||(pcomp->m_nFlag == FLAG_DELETED))
			{
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) m_listSchObject[nDiagram].GetNext(pos);
				continue;
			}

			sch_ad6_linenum++;
			sch_elements_num++;
			if((kind == 0) || (kind == SCH_ELEMENT_COMPONENT)) sch_objects_num++;

			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
			{
				pobject = m_listSchObject[nDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
				{
					COb_schpin* ppin = (COb_schpin*)pobject;
					sch_ad6_linenum++;
					sch_elements_num++;
					//引脚uniqueID 文字行
					sch_ad6_linenum++;
					if(ppin->m_csConnectTo.IsEmpty() ==FALSE) sch_ad6_linenum++;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
				{
					COb_schieee* pieee = (COb_schieee*)pobject;
					sch_ad6_linenum++;
					sch_elements_num++;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
				{
					COb_scharc* parc = (COb_scharc*)pobject;
					sch_ad6_linenum++;
					sch_elements_num++;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
				{
					COb_schellipse* pellipse = (COb_schellipse*)pobject;
					sch_ad6_linenum++;
					sch_elements_num++;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
				{
					COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
					sch_ad6_linenum++;
					sch_elements_num++;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
				{
					COb_schpie* ppie = (COb_schpie*)pobject;
					sch_ad6_linenum++;
					sch_elements_num++;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
				{
					COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
					sch_ad6_linenum++;
					sch_elements_num++;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
				{
					COb_schrect* prect = (COb_schrect*)pobject;
					sch_ad6_linenum++;
					sch_elements_num++;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
				{
					COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
					sch_ad6_linenum++;
					sch_elements_num++;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
				{
					COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
					sch_ad6_linenum++;
					sch_elements_num++;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
				{
					COb_schbezier* pbezier = (COb_schbezier*)pobject;
					sch_ad6_linenum++;
					sch_elements_num++;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext = (COb_schtext*)pobject;
					sch_ad6_linenum++;
					sch_elements_num++;
					if(ptext->m_nID == TEXT_COMPPACKAGE) sch_ad6_linenum+=3;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
				{
					COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
					sch_ad6_linenum++;
					sch_elements_num++;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
				{
					COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
					sch_ad6_linenum++;
					sch_elements_num++;
				}
			}
		}
	}

	if(type == 0) return sch_objects_num;
	else if(type == 1) return sch_elements_num;
	else return sch_ad6_linenum;
}

void CESDDoc::OnReportElements()
{
	CDlg_ReportElements dlg;
	
	dlg.pDoc = this;
	dlg.m_nCurrentDiagram = 0;
	int r = dlg.DoModal();

}

void CESDDoc::OnReportCompnet()
{
	if(m_arraySchNet.GetCount() <= 0) 
	{
		AfxMessageBox("未找到网络信息, 网络未编译或设计没有网络。");
		return;
	}

	CDlg_Report_CompNet dlg;
	
	dlg.pDoc = this;
	dlg.DoModal();
}

int CESDDoc::Get_PCB_Object(int kind)
{
	return 1;
}

int CESDDoc::Get_Correct_Orientation(int angle)
{
	for(;;)
	{
		if(angle > 360) angle -= 360;
		else if(angle < 0) angle += 360;
		else break;
	}

	if((angle == 0)||(angle == 90)||(angle==180)||(angle==270)) return angle;
	else return 0;
}

double CESDDoc::Get_Correct_Angle(double angle)
{
	for(;;)
	{
		if(angle > 360.0) angle -= 360.0;
		else if(angle < 0) angle += 360.0;
		else break;
	}

	double delta = 0.01;
	if(abs(angle) < delta) angle = 0.0;
	else if(abs(angle - 90.0) < delta) angle = 90.0;
	else if(abs(angle - 180.0) < delta) angle = 180.0;
	else if(abs(angle - 270.0) < delta) angle = 270.0;
	else if(abs(angle - 360.0) < delta) angle = 360.0;

	return angle;
}
	
void CESDDoc::Clear_Discarded_Undo_Buffer(void)
{
	int i,j,num;
	if(nEDA_System_State == SCH)
	{
		for(j = m_nSCH_Undo_Num; j < SCH_UNDO_NUMBER; j++)
		{
			if(m_listSchUndo[j].IsEmpty() == TRUE) break;

			num = m_listSchUndo[j].GetCount();
			POSITION pos = m_listSchUndo[j].GetHeadPosition();
			for(i=0; i<num; i++)
			{
				if(pos == NULL) break;
				CSchUndo* pundo = m_listSchUndo[j].GetNext(pos);
				delete pundo;
			}
			m_listSchUndo[j].RemoveAll();
		}
	}
	else
	{
		for(j = m_nPCB_Undo_Num; j < PCB_UNDO_NUMBER; j++)
		{
			if(m_listPcbUndo[j].IsEmpty() == TRUE) break;

			num = m_listPcbUndo[j].GetCount();
			POSITION pos = m_listPcbUndo[j].GetHeadPosition();
			for(i=0; i<num; i++)
			{
				if(pos == NULL) break;
				CPcbUndo* pundo = m_listPcbUndo[j].GetNext(pos);
				delete pundo;
			}
			m_listPcbUndo[j].RemoveAll();
		}
	}
}

void CESDDoc::Undo_Buffer_Transfer(void)
{
	int i,j,num;
	POSITION pos;

	if(nEDA_System_State == SCH)
	{
		CSchUndo* pundo;

		num = m_listSchUndo[0].GetCount();
		pos = m_listSchUndo[0].GetHeadPosition();
		for(i=0; i<num; i++)
		{
			if(pos == NULL) break;
			pundo = m_listSchUndo[0].GetNext(pos);
			delete pundo;	
		}
		m_listSchUndo[0].RemoveAll();

	
		for(j=1; j<SCH_UNDO_NUMBER; j++)
		{
			num = m_listSchUndo[j].GetCount();
			pos = m_listSchUndo[j].GetHeadPosition();
			for(i=0; i<num; i++)
			{
				if(pos == NULL) break;
				pundo = m_listSchUndo[j].GetNext(pos);
				m_listSchUndo[j-1].AddTail(pundo);
			}		
			m_listSchUndo[j].RemoveAll();
		}

		m_nSCH_Undo_Num--;
	}
	else
	{
		CPcbUndo* pundo;

		num = m_listPcbUndo[0].GetCount();
		pos = m_listPcbUndo[0].GetHeadPosition();
		for(i=0; i<num; i++)
		{
			if(pos == NULL) break;
			pundo = m_listPcbUndo[0].GetNext(pos);
			delete pundo;	
		}
		m_listPcbUndo[0].RemoveAll();

		for(j=1; j<PCB_UNDO_NUMBER; j++)
		{
			num = m_listPcbUndo[j].GetCount();
			pos = m_listPcbUndo[j].GetHeadPosition();
			for(i=0; i<num; i++)
			{
				if(pos == NULL) break;
				pundo = m_listPcbUndo[j].GetNext(pos);
				m_listPcbUndo[j-1].AddTail(pundo);
			}		
			m_listPcbUndo[j].RemoveAll();
		}

		m_nPCB_Undo_Num--;
	}
}

BOOL CESDDoc::CheckWireEnd(double fx, double fy, int nDiagram)
{
	CObject* pobject;
	int num = m_listSchObject[nDiagram].GetCount();
	POSITION pos;
	pos = m_listSchObject[nDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{	
		if(pos == NULL) break;
		pobject = m_listSchObject[nDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != nDiagram)||(pline->m_nFlag == FLAG_DELETED)||((pline->m_nAttrib != LINE_WIRE)&&(pline->m_nAttrib != LINE_BUS)))	continue;
			
			int  vertex_num = pline->m_cVertex.GetCount();
			if((pline->m_cVertex[0].fx == fx)&&(pline->m_cVertex[0].fy == fy)) return TRUE;
			else if((pline->m_cVertex[vertex_num-1].fx == fx)&&(pline->m_cVertex[vertex_num-1].fy == fy)) return TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;
			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) m_listSchObject[nDiagram].GetNext(pos);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pobject = m_listSchObject[nDiagram].GetNext(pos);
		}
	}

	return FALSE;
}

BOOL CESDDoc::GetElectricalPoint(double* rfx, double* rfy, double fx, double fy, int nDiagram)
{
	CObject* pobject;
	int num = m_listSchObject[nDiagram].GetCount();
	POSITION pos;
	pos = m_listSchObject[nDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{	
		if(pos == NULL) break;
		pobject = m_listSchObject[nDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if( (pline->m_nDiagram != nDiagram)||(pline->m_nFlag == FLAG_DELETED)||\
				((pline->m_nAttrib != LINE_WIRE)&&(pline->m_nAttrib != LINE_BUS)&&(pline->m_nAttrib != LINE_BUS_ENTRY)) )	continue;
			
			int  vertex_num = pline->m_cVertex.GetCount();
			for(int k=0; k<vertex_num; k++)
			{
				if((abs(pline->m_cVertex[k].fx - fx) <= m_fSCH_ElecGrid)&&(abs(pline->m_cVertex[k].fy - fy) <= m_fSCH_ElecGrid))
				{
					*rfx = pline->m_cVertex[k].fx;
					*rfy = pline->m_cVertex[k].fy;
					return TRUE;
				}
			}
			for(int k=1; k<vertex_num; k++)
			{
				if(pline->m_cVertex[k].fx == pline->m_cVertex[k-1].fx)	
				{
					double snaplock_fy;
					if(m_bSCH_SnapGridEnable == TRUE)
					{
						int n1, n2;
						double f;

						f = fy/m_fSCH_SnapGrid;
						n1 = (int)f;
						n2 = (int)((f-n1)*10000);
						if(n2 >= 5000) n1++;
						f = (double)n1*m_fSCH_SnapGrid;
						snaplock_fy = f;
					}
					else snaplock_fy = fy;

					if( (abs(pline->m_cVertex[k].fx - fx) <= m_fSCH_ElecGrid) && \
						( snaplock_fy > MIN(pline->m_cVertex[k].fy, pline->m_cVertex[k-1].fy)) && \
						( snaplock_fy < MAX(pline->m_cVertex[k].fy, pline->m_cVertex[k-1].fy)) )
					{
						*rfx = pline->m_cVertex[k].fx;
						*rfy = snaplock_fy;
						return TRUE;
					}
				}
				else if(pline->m_cVertex[k].fy == pline->m_cVertex[k-1].fy)
				{
					double snaplock_fx;
					if(m_bSCH_SnapGridEnable == TRUE)
					{
						int n1, n2;
						double f;

						f = fx/m_fSCH_SnapGrid;
						n1 = (int)f;
						n2 = (int)((f-n1)*10000);
						if(n2 >= 5000) n1++;
						f = (double)n1*m_fSCH_SnapGrid;
						snaplock_fx = f;
					}
					else snaplock_fx = fx;

					if( (abs(pline->m_cVertex[k].fy - fy) <= m_fSCH_ElecGrid) && \
						( snaplock_fx > MIN(pline->m_cVertex[k].fx, pline->m_cVertex[k-1].fx)) && \
						( snaplock_fx < MAX(pline->m_cVertex[k].fx, pline->m_cVertex[k-1].fx)) )
					{
						*rfx = snaplock_fx;
						*rfy = pline->m_cVertex[k].fy;
						return TRUE;
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;
			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) m_listSchObject[nDiagram].GetNext(pos);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram == nDiagram)&&(pcomp->m_nFlag == FLAG_NORMAL))	
			{
				for(int k=0; k< pcomp->m_arrayPos.GetCount(); k++)
				{
					POSITION subpos = pcomp->m_arrayPos.GetAt(k);
					pobject = m_listSchObject[nDiagram].GetAt(subpos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
					{
						COb_schpin* ppin = (COb_schpin*)pobject;
						double px, py;
						if(ppin->m_nOrientation == 0)
						{
							px = ppin->m_fX + ppin->m_fPinLength;
							py = ppin->m_fY;
						}
						else if(ppin->m_nOrientation == 90)
						{
							px = ppin->m_fX;
							py = ppin->m_fY + ppin->m_fPinLength;
						}
						else if(ppin->m_nOrientation == 180)
						{
							px = ppin->m_fX - ppin->m_fPinLength;
							py = ppin->m_fY;
						}
						else if(ppin->m_nOrientation == 270)
						{
							px = ppin->m_fX;
							py = ppin->m_fY - ppin->m_fPinLength;
						}
						if((abs(px - fx) <= m_fSCH_ElecGrid)&&(abs(py - fy) <= m_fSCH_ElecGrid))
						{
							*rfx = px;
							*rfy = py;
							return TRUE;
						}
					}
				}
			}

			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pobject = m_listSchObject[nDiagram].GetNext(pos);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != nDiagram)||(ppower->m_nFlag != FLAG_NORMAL))	continue;
			if((abs(ppower->m_fX - fx) <= m_fSCH_ElecGrid)&&(abs(ppower->m_fY - fy) <= m_fSCH_ElecGrid))
			{
				*rfx = ppower->m_fX;
				*rfy = ppower->m_fY;
				return TRUE;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != nDiagram)||(pjunc->m_nFlag != FLAG_NORMAL))	continue;
			if((abs(pjunc->m_fCx - fx) <= m_fSCH_ElecGrid)&&(abs(pjunc->m_fCy - fy) <= m_fSCH_ElecGrid))
			{
				*rfx = pjunc->m_fCx;
				*rfy = pjunc->m_fCy;
				return TRUE;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != nDiagram)||(ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_nID != TEXT_NETLABEL))	continue;
			if((abs(ptext->m_fSx - fx) <= m_fSCH_ElecGrid)&&(abs(ptext->m_fSy - fy) <= m_fSCH_ElecGrid))
			{
				*rfx = ptext->m_fSx;
				*rfy = ptext->m_fSy;
				return TRUE;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != nDiagram)||(pport->m_nFlag != FLAG_NORMAL))	continue;
			if((abs(pport->m_fX - fx) <= m_fSCH_ElecGrid)&&(abs(pport->m_fY - fy) <= m_fSCH_ElecGrid))
			{
				*rfx = pport->m_fX;
				*rfy = pport->m_fY;
				return TRUE;
			}
			double px, py;
			if(pport->m_nStyle < 4)
			{
				px = pport->m_fX + pport->m_fSize;
				py = pport->m_fY;
			}
			else
			{
				px = pport->m_fX;
				py = pport->m_fY + pport->m_fSize;
			}
			if((abs(px - fx) <= m_fSCH_ElecGrid)&&(abs(py - fy) <= m_fSCH_ElecGrid))
			{
				*rfx = px;
				*rfy = py;
				return TRUE;
			}			
		}
	}
	return FALSE;
}

BOOL CESDDoc::CheckJuncExist(double fx, double fy, int nDiagram)
{
	CObject* pobject;
	int num =m_listSchObject[nDiagram].GetCount();
	POSITION pos;
	pos = m_listSchObject[nDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{	
		if(pos == NULL) break;
		pobject = m_listSchObject[nDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
			if((pjunc->m_fCx == fx)&&(pjunc->m_fCy == fy)) return TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;
			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) m_listSchObject[nDiagram].GetNext(pos);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) m_listSchObject[nDiagram].GetNext(pos);
		}
	}

	return FALSE;
}

BOOL CESDDoc::PcbGetElectricalPoint(double* rfx, double* rfy, double fx, double fy, int layer)
{
	for(int i=0; i<m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nPolygon != -1)||(ptrack->m_nComponent != -1)) continue;

		if(m_cPCB_Sheet.m_bAllLayersElecGrid == TRUE) //||(ptrack->m_nLayer == layer))
		{
			if((abs(ptrack->m_fX1 - fx) <= m_cPCB_Sheet.m_fElecGridRange)&&(abs(ptrack->m_fY1 - fy) <= m_cPCB_Sheet.m_fElecGridRange))
			{
				*rfx = ptrack->m_fX1;
				*rfy = ptrack->m_fY1;
				return TRUE;
			}
			else if((abs(ptrack->m_fX2 - fx) <= m_cPCB_Sheet.m_fElecGridRange)&&(abs(ptrack->m_fY2 - fy) <= m_cPCB_Sheet.m_fElecGridRange))
			{
				*rfx = ptrack->m_fX2;
				*rfy = ptrack->m_fY2;
				return TRUE;
			}

			if(ptrack->m_fX1 == ptrack->m_fX2)	
			{
				double snaplock_fy;

				int n1, n2;
				double f;

				f = fy/m_cPCB_Sheet.m_fSnapGridSize;
				n1 = (int)f;
				n2 = (int)((f-n1)*10000);
				if(n2 >= 5000) n1++;
				f = (double)n1*m_cPCB_Sheet.m_fSnapGridSize;
				snaplock_fy = f;

				if( (abs(ptrack->m_fX1 - fx) <= m_cPCB_Sheet.m_fElecGridRange) && \
					( snaplock_fy > MIN(ptrack->m_fY1, ptrack->m_fY2)) && \
					( snaplock_fy < MAX(ptrack->m_fY1, ptrack->m_fY2)) )
				{
					*rfx = ptrack->m_fX1;
					*rfy = snaplock_fy;
					return TRUE;
				}
			}
			else if(ptrack->m_fY1 == ptrack->m_fY2)
			{
				double snaplock_fx;

				int n1, n2;
				double f;

				f = fx/m_cPCB_Sheet.m_fSnapGridSize;
				n1 = (int)f;
				n2 = (int)((f-n1)*10000);
				if(n2 >= 5000) n1++;
				f = (double)n1*m_cPCB_Sheet.m_fSnapGridSize;
				snaplock_fx = f;

				if( (abs(ptrack->m_fY1 - fy) <= m_cPCB_Sheet.m_fElecGridRange) && \
					( snaplock_fx > MIN(ptrack->m_fX1, ptrack->m_fX2)) && \
					( snaplock_fx < MAX(ptrack->m_fX1, ptrack->m_fX2)) )
				{
					*rfx = snaplock_fx;
					*rfy = ptrack->m_fY1;
					return TRUE;
				}
			}
		}
	}

	for(int i=0; i<m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon != -1)||(parc->m_nComponent != -1)) continue;
		if(m_cPCB_Sheet.m_bAllLayersElecGrid == TRUE)
		{
			double cx,cy,r;
			cx = parc->m_fCx;
			cy = parc->m_fCy;
			r = parc->m_fRadius;
			double fsx, fsy, fex, fey;

			fsx = cx + r*cos(parc->m_fSangle*fEDA_pi/180);
			fsy = cy + r*sin(parc->m_fSangle*fEDA_pi/180);
			fex = cx + r*cos(parc->m_fEangle*fEDA_pi/180);
			fey = cy + r*sin(parc->m_fEangle*fEDA_pi/180);
			
			if((abs(fsx - fx) <= m_cPCB_Sheet.m_fElecGridRange)&&(abs(fsy - fy) <= m_cPCB_Sheet.m_fElecGridRange))
			{
				*rfx = fsx;
				*rfy = fsy;
				return TRUE;
			}
			else if((abs(fex - fx) <= m_cPCB_Sheet.m_fElecGridRange)&&(abs(fey - fy) <= m_cPCB_Sheet.m_fElecGridRange))
			{
				*rfx = fex;
				*rfy = fey;
				return TRUE;
			}
		}
	}

	for(int i=0; i<m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon != -1)||(pregion->m_nComponent != -1)) continue;
		if(m_cPCB_Sheet.m_bAllLayersElecGrid == TRUE) 
		{
			for(int j=0; j<pregion->m_cVertex.GetCount(); j++)
			{
				Struct_RegionVertex vertex;
				vertex = pregion->m_cVertex.GetAt(j);
				if((abs(vertex.fx - fx) <= m_cPCB_Sheet.m_fElecGridRange)&&(abs(vertex.fy - fy) <= m_cPCB_Sheet.m_fElecGridRange))
				{
					*rfx = vertex.fx;
					*rfy = vertex.fy;
					return TRUE;
				}
			}
		}
	}

	for(int i=0; i<m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag != FLAG_NORMAL) continue;
		if(m_cPCB_Sheet.m_bAllLayersElecGrid == TRUE)
		{
			if((abs(ppad->m_fCx - fx) <= m_cPCB_Sheet.m_fElecGridRange)&&(abs(ppad->m_fCy - fy) <= m_cPCB_Sheet.m_fElecGridRange))
			{
				*rfx = ppad->m_fCx;
				*rfy = ppad->m_fCy;
				return TRUE;
			}
		}
	}		
	for(int i=0; i<m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag != FLAG_NORMAL) continue;
		if(m_cPCB_Sheet.m_bAllLayersElecGrid == TRUE)
		{
			if((abs(pvia->m_fCx - fx) <= m_cPCB_Sheet.m_fElecGridRange)&&(abs(pvia->m_fCy - fy) <= m_cPCB_Sheet.m_fElecGridRange))
			{
				*rfx = pvia->m_fCx;
				*rfy = pvia->m_fCy;
				return TRUE;
			}
		}
	}	
	for(int i=0; i<m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = m_arrayPcbFill.GetAt(i);
		if(pfill->m_nFlag != FLAG_NORMAL) continue;
		if(m_cPCB_Sheet.m_bAllLayersElecGrid == TRUE)
		{
			double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
			pfill->GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);
			if((abs(fx1 - fx) <= m_cPCB_Sheet.m_fElecGridRange)&&(abs(fy1 - fy) <= m_cPCB_Sheet.m_fElecGridRange))
			{
				*rfx = fx1;
				*rfy = fy1;
				return TRUE;
			}
			if((abs(fx2 - fx) <= m_cPCB_Sheet.m_fElecGridRange)&&(abs(fy2 - fy) <= m_cPCB_Sheet.m_fElecGridRange))
			{
				*rfx = fx2;
				*rfy = fy2;
				return TRUE;
			}	
			if((abs(fx3 - fx) <= m_cPCB_Sheet.m_fElecGridRange)&&(abs(fy3 - fy) <= m_cPCB_Sheet.m_fElecGridRange))
			{
				*rfx = fx3;
				*rfy = fy3;
				return TRUE;
			}
			if((abs(fx4 - fx) <= m_cPCB_Sheet.m_fElecGridRange)&&(abs(fy4 - fy) <= m_cPCB_Sheet.m_fElecGridRange))
			{
				*rfx = fx4;
				*rfy = fy4;
				return TRUE;
			}	
		}
	}	

	return FALSE;
}

void CESDDoc::OnPcbBoardOptions()
{
	CDlg_PCBBoardOptions dlg;
	dlg.m_pDoc = this;

	dlg.m_nComboUnit = m_cPCB_Sheet.m_nUnit;

	dlg.m_csComboSnapX = Convert_XY_To_String(m_cPCB_Sheet.m_fSnapGridSizeX);
	dlg.m_csComboSnapY = Convert_XY_To_String(m_cPCB_Sheet.m_fSnapGridSizeY);

	dlg.m_csComboCompX = Convert_XY_To_String(m_cPCB_Sheet.m_fComponentGridSizeX);
	dlg.m_csComboCompY = Convert_XY_To_String(m_cPCB_Sheet.m_fComponentGridSizeY);

	dlg.m_bCheckEGridEnable = m_cPCB_Sheet.m_bElecGridEnable;
	dlg.m_csComboEGridRange = Convert_XY_To_String(m_cPCB_Sheet.m_fElecGridRange);
	dlg.m_bCheckAllLayer = m_cPCB_Sheet.m_bAllLayersElecGrid;

	if(m_cPCB_Sheet.m_bDotGrid == TRUE) dlg.m_nComboVGridStyle = 0;
	else dlg.m_nComboVGridStyle = 1;
	dlg.m_csComboVGrid1 = Convert_XY_To_String(m_cPCB_Sheet.m_fBigVisibleGridSize);
	dlg.m_csComboVGrid2 = Convert_XY_To_String(m_cPCB_Sheet.m_fVisibleGridSize);

	dlg.m_csEditSheetX = Convert_XY_To_String(m_cPCB_Sheet.m_fSheetX);
	dlg.m_csEditSheetY = Convert_XY_To_String(m_cPCB_Sheet.m_fSheetY);
	dlg.m_csEditSheetWidth = Convert_XY_To_String(m_cPCB_Sheet.m_fSheetWidth);
	dlg.m_csEditSheetHeight = Convert_XY_To_String(m_cPCB_Sheet.m_fSheetHeight);
	dlg.m_bCheckLockSheet = m_cPCB_Sheet.m_bLockSheet;
	dlg.m_bCheckShowSheet = m_cPCB_Sheet.m_bShowSheet;	


	if(dlg.DoModal() == IDOK)
	{

		m_cPCB_Sheet.m_nUnit = dlg.m_nComboUnit;
		
		m_cPCB_Sheet.m_fSnapGridSizeX = Convert_PCB_String_To_XY(dlg.m_csComboSnapX);
		m_cPCB_Sheet.m_fSnapGridSize = m_cPCB_Sheet.m_fSnapGridSizeY = m_cPCB_Sheet.m_fSnapGridSizeX;

		m_cPCB_Sheet.m_fComponentGridSizeX = Convert_PCB_String_To_XY(dlg.m_csComboCompX);
		m_cPCB_Sheet.m_fComponentGridSizeY = Convert_PCB_String_To_XY(dlg.m_csComboCompY);

		m_cPCB_Sheet.m_bElecGridEnable = dlg.m_bCheckEGridEnable;
		if(dlg.m_csComboEGridRange.Find("锁定网格") >= 0)
		{
			if(dlg.m_csComboEGridRange.Find("0.25x") >= 0) m_cPCB_Sheet.m_fElecGridRange = 0.25*m_cPCB_Sheet.m_fSnapGridSizeX;
			else if(dlg.m_csComboEGridRange.Find("0.5x") >= 0) m_cPCB_Sheet.m_fElecGridRange = 0.5*m_cPCB_Sheet.m_fSnapGridSizeX;
			else if(dlg.m_csComboEGridRange.Find("0.75x") >= 0) m_cPCB_Sheet.m_fElecGridRange = 0.75*m_cPCB_Sheet.m_fSnapGridSizeX;
			else if(dlg.m_csComboEGridRange.Find("1x") >= 0) m_cPCB_Sheet.m_fElecGridRange = m_cPCB_Sheet.m_fSnapGridSizeX;
		}
		else m_cPCB_Sheet.m_fElecGridRange = Convert_PCB_String_To_XY(dlg.m_csComboEGridRange);
		m_cPCB_Sheet.m_bAllLayersElecGrid = dlg.m_bCheckAllLayer;

		if(dlg.m_nComboVGridStyle == 0)	m_cPCB_Sheet.m_bDotGrid = TRUE;
		else m_cPCB_Sheet.m_bDotGrid = FALSE;

		if(dlg.m_csComboVGrid1.Find("锁定网格") >= 0)
		{
			if(dlg.m_csComboVGrid1.Find("1x") >= 0) m_cPCB_Sheet.m_fBigVisibleGridSize = m_cPCB_Sheet.m_fSnapGridSizeX;
			else if(dlg.m_csComboVGrid1.Find("2x") >= 0) m_cPCB_Sheet.m_fBigVisibleGridSize = 2*m_cPCB_Sheet.m_fSnapGridSizeX;
			else if(dlg.m_csComboVGrid1.Find("5x") >= 0) m_cPCB_Sheet.m_fBigVisibleGridSize = 5*m_cPCB_Sheet.m_fSnapGridSizeX;
			else if(dlg.m_csComboVGrid1.Find("10x") >= 0) m_cPCB_Sheet.m_fBigVisibleGridSize = 10*m_cPCB_Sheet.m_fSnapGridSizeX;
		}
		else m_cPCB_Sheet.m_fBigVisibleGridSize = Convert_PCB_String_To_XY(dlg.m_csComboVGrid1);

		if(dlg.m_csComboVGrid2.Find("锁定网格") >= 0)
		{
			if(dlg.m_csComboVGrid2.Find("1x") >= 0) m_cPCB_Sheet.m_fVisibleGridSize = m_cPCB_Sheet.m_fSnapGridSizeX;
			else if(dlg.m_csComboVGrid2.Find("2x") >= 0) m_cPCB_Sheet.m_fVisibleGridSize = 2*m_cPCB_Sheet.m_fSnapGridSizeX;
			else if(dlg.m_csComboVGrid2.Find("5x") >= 0) m_cPCB_Sheet.m_fVisibleGridSize = 5*m_cPCB_Sheet.m_fSnapGridSizeX;
			else if(dlg.m_csComboVGrid2.Find("10x") >= 0) m_cPCB_Sheet.m_fVisibleGridSize = 10*m_cPCB_Sheet.m_fSnapGridSizeX;
		}
		else m_cPCB_Sheet.m_fVisibleGridSize = Convert_PCB_String_To_XY(dlg.m_csComboVGrid2);

		m_cPCB_Sheet.m_fSheetX = Convert_PCB_String_To_XY(dlg.m_csEditSheetX);
		m_cPCB_Sheet.m_fSheetY = Convert_PCB_String_To_XY(dlg.m_csEditSheetY);
		m_cPCB_Sheet.m_fSheetWidth = Convert_PCB_String_To_XY(dlg.m_csEditSheetWidth);
		m_cPCB_Sheet.m_fSheetHeight = Convert_PCB_String_To_XY(dlg.m_csEditSheetHeight);

		m_cPCB_Sheet.m_bLockSheet = dlg.m_bCheckLockSheet;
		m_cPCB_Sheet.m_bShowSheet = dlg.m_bCheckShowSheet;

	}
}

void CESDDoc::OnPcbLayerStack()
{
	// 设置层的数目和顺序
	CDlg_PCBLayerStack dlg;
	dlg.pDocument = this;

	if(dlg.DoModal() == IDOK)
	{
		//原内平面层使用情况
		int nPlaneUsedOld[POWER_LAYER_NUM];
		for(int i=0; i<POWER_LAYER_NUM; i++) nPlaneUsedOld[i] = 0;
		for(int i=0; i<m_arrayPcbLayerShow.GetCount(); i++)
		{
			int layer = m_arrayPcbLayerShow.GetAt(i);
			if((layer >= POWERPLANE1)&&(layer <= POWERPLANE16))
			{
				nPlaneUsedOld[layer - POWERPLANE1] = 1;
			}
		}

		//原信号层使用情况
		int nSignalUsedOld[SIGNAL_LAYER_NUM];
		for(int i=0; i<SIGNAL_LAYER_NUM; i++) nSignalUsedOld[i] = 0;
		for(int i=0; i<m_arrayPcbLayerShow.GetCount(); i++)
		{
			int layer = m_arrayPcbLayerShow.GetAt(i);
			if((layer >= TOPLAYER)&&(layer <= BOTTOMLAYER))
			{
				nSignalUsedOld[layer] = 1;
			}
		}

		CXTPReportRecords* pRecords = dlg.m_wndReportCtrl.GetRecords();
		int layer0, layer1, layer2;
		layer0 = -1;
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecord* pRecord1 = pRecords->GetAt(i);
			CXTPReportRecordItemNumber* pItem1 = (CXTPReportRecordItemNumber*)pRecord1->GetItem(2);
			layer1 = pItem1->GetValue();
			
			if((layer1 >= POWERPLANE1)&&(layer1 <= POWERPLANE16))
			{
				//电源层修改名称和网络
				CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pRecord1->GetItem(1);
				CString csLayerName;
				csLayerName = pItemText->GetValue();

				CString csName;
				CString csNet;
				int itemp = csLayerName.Find('(');
				csName = csLayerName.Left(itemp);
				csNet = csLayerName.Right(csLayerName.GetLength() - itemp - 1);
				itemp = csNet.Find(')');
				csNet = csNet.Left(itemp);

				m_sPCB_LayerParam[layer1].csLayerName = csName;
				m_csPowerPlane[layer1 - POWERPLANE1] = csNet;
			}
			else
			{
				//信号层修改名称
				CXTPReportRecordItemText* pItemText = (CXTPReportRecordItemText*)pRecord1->GetItem(1);
				m_sPCB_LayerParam[layer1].csLayerName = pItemText->GetValue();
			}


			//自动打开设计层
			m_sPCB_LayerParam[layer1].bLayerShow = TRUE;
			
			m_sPCB_LayerParam[layer1].nLayerPrev = layer0;			
			if(i < (pRecords->GetCount()-1))
			{
				CXTPReportRecord* pRecord2 = pRecords->GetAt(i+1);
				CXTPReportRecordItemNumber* pItem2 = (CXTPReportRecordItemNumber*)pRecord2->GetItem(2);
				layer2 = pItem2->GetValue();
			
				m_sPCB_LayerParam[layer1].nLayerNext = layer2;
			}
			else m_sPCB_LayerParam[layer1].nLayerNext = -1;

			layer0 = layer1;
		}
		
		InitLayerShowArray();
		
		int nPlaneUsedNew[POWER_LAYER_NUM];
		for(int i=0; i<POWER_LAYER_NUM; i++)	nPlaneUsedNew[i] = 0;
		for(int i=0; i<m_arrayPcbLayerShow.GetCount(); i++)
		{
			int layer = m_arrayPcbLayerShow.GetAt(i);
			if((layer >= POWERPLANE1)&&(layer <= POWERPLANE16))
			{
				nPlaneUsedNew[layer - POWERPLANE1] = 1;
			}
		}

		//打开了新的内平面层
		for(int i=0; i<POWER_LAYER_NUM; i++)
		{
			int layer = i + POWERPLANE1;
			if(nPlaneUsedNew[i] == nPlaneUsedOld[i])
			{
				if((nPlaneUsedNew[i] == 1)&&(m_csPowerPlane[i] != "多网络"))
				{
					//修改内平面
					int net = -1;
					for(int j = 0; j<m_arrayPcbNet.GetCount(); j++)
					{
						COb_pcbnet* pnet = (COb_pcbnet*)m_arrayPcbNet.GetAt(j);
						if(pnet->m_nFlag != FLAG_NORMAL) continue;
	
						if(pnet->m_csNetName == m_csPowerPlane[i]) 
						{
							net = j;
							break;
						}
					}
					for(int j=0; j<m_arrayPcbPolygon.GetCount(); j++)
					{
						COb_pcbpolygon* ppolygon = (COb_pcbpolygon*)m_arrayPcbPolygon.GetAt(j);
						if((ppolygon->m_nPolygonType == SPLITPLANE)&&(ppolygon->m_nLayer == layer)) ppolygon->m_nNet = net;
					}
				}
			}
			else
			{
				if(nPlaneUsedOld[i] == 1)
				{
					//删除了内平面
					DeletePCBLayerObject(layer);
				}
				else
				{
					//添加了内平面
					AddPCBInternalPlaneObject(layer);
				}
			}
		}

		//新信号层使用情况
		int nSignalUsedNew[SIGNAL_LAYER_NUM];
		for(int i=0; i<SIGNAL_LAYER_NUM; i++) nSignalUsedNew[i] = 0;
		for(int i=0; i<m_arrayPcbLayerShow.GetCount(); i++)
		{
			int layer = m_arrayPcbLayerShow.GetAt(i);
			if((layer >= TOPLAYER)&&(layer <= BOTTOMLAYER))
			{
				nSignalUsedNew[layer] = 1;
			}
		}
		//删除了中间层
		for(int i=0; i<SIGNAL_LAYER_NUM; i++)
		{
			if(nSignalUsedNew[i] == nSignalUsedOld[i])
			{
			}
			else
			{
				if(nSignalUsedOld[i] == 1)
				{
					//删除了信号层
					//DeletePCBLayerObject(i);
				}
				else
				{
					//添加了信号层
				}
			}
		}

		CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
		CMDIChildWnd* pChild = pFrame->MDIGetActive();
		if(pChild != NULL)  pChild->Invalidate();
	}
}

BOOL CESDDoc::InDistance_Point_Point(double fx1, double fy1, double fx2, double fy2, double distance)
{
	if(abs(fx1 - fx2) > distance) return FALSE;
	if(abs(fy1 - fy2) > distance) return FALSE;

	double d = SQRT(fx1, fy1, fx2, fy2);
	if(d > distance) return FALSE;
	else return TRUE;
}

BOOL CESDDoc::InDistance_Point_LINE(double fx, double fy, double fsx, double fsy, double fex, double fey, double distance)
{
	double ftemp;
	double m, f;
	double ftempx,ftempy;
	double fmaxx,fminx,fmaxy,fminy;

	if((fsx==fex)&&(fsy==fey)) return InDistance_Point_Point(fx, fy, fsx, fsy, distance);

	fmaxx = MAX(fsx,fex);
	fminx = MIN(fsx,fex);
	fmaxy = MAX(fsy,fey);
	fminy = MIN(fsy,fey);
	if((fx < fminx)&&(abs(fx-fminx) > distance)) return FALSE;
	if((fx > fmaxx)&&(abs(fx-fmaxx) > distance)) return FALSE;
	if((fy < fminy)&&(abs(fy-fminy) > distance)) return FALSE;
	if((fy > fmaxy)&&(abs(fy-fmaxy) > distance)) return FALSE;

	if(fex == fsx) 
	{ 
		if((fy >= fminy)&&(fy <= fmaxy))
		{
			if(abs(fx - fsx) > distance) return FALSE;
			else return TRUE;
		}
		else 
		{ 
			if(fsy > fey)
			{
				if(fy > fsy)
				{
					if((abs(fx - fsx) > distance)||(abs(fy - fsy) > distance)) return FALSE;
					ftemp = SQRT( fx, fy, fsx, fsy);
				}
				else
				{
					if((abs(fx - fex) > distance)||(abs(fy - fey) > distance)) return FALSE;
					ftemp = SQRT(fx, fy, fex, fey);
				}
			}
			else
			{
				if(fy < fsy)
				{
					if((abs(fx - fsx) > distance)||(abs(fy - fsy) > distance)) return FALSE;
					ftemp = SQRT(fx, fy, fsx, fsy);
				}
				else
				{
					if((abs(fx - fex) > distance)||(abs(fy - fey) > distance)) return FALSE;
					ftemp = SQRT(fx, fy, fex, fey);
				}
			}
			if(ftemp > distance) return FALSE;
			else return TRUE;
		}
	}
	else if(fey == fsy)
	{
		if((fx >= fminx)&&(fx <= fmaxx))
		{
			if(abs(fy - fsy) > distance) return FALSE;
			else return TRUE;
		}
		else 
		{
			if(fsx >= fex)
			{
				if(fx > fsx)
				{
					if((abs(fx - fsx) > distance)||(abs(fy - fsy) > distance)) return FALSE;
					ftemp = SQRT(fx, fy, fsx, fsy);
				}
				else
				{
					if((abs(fx - fex) > distance)||(abs(fy - fey) > distance)) return FALSE;
					ftemp = SQRT(fx, fy, fex, fey);
				}
			}
			else
			{
				if(fx < fsx)
				{
					if((abs(fx - fsx) > distance)||(abs(fy - fsy) > distance)) return FALSE;
					ftemp = SQRT(fx, fy, fsx, fsy);
				}
				else
				{
					if((abs(fx - fex) > distance)||(abs(fy - fey) > distance)) return FALSE;
					ftemp = SQRT(fx, fy, fex, fey);
				}
			}
			if(ftemp > distance) return FALSE;
			else return TRUE;
		}
	}
	else
	{
		m = (fey - fsy)/(fex - fsx);
		f = (m*m*fsx + fx - m*fsy + m*fy)/(m*m+1);
		ftempx = f;
		ftempy = m*(f-fsx) + fsy;
		if( ((ftempx >= fsx)&&(ftempx <= fex)&&(ftempy >= fsy)&&(ftempy <= fey))||\
			((ftempx >= fsx)&&(ftempx <= fex)&&(ftempy >= fey)&&(ftempy <= fsy))||\
			((ftempx >= fex)&&(ftempx <= fsx)&&(ftempy >= fsy)&&(ftempy <= fey))||\
			((ftempx >= fex)&&(ftempx <= fsx)&&(ftempy >= fey)&&(ftempy <= fsy)))
		{
			if((abs(fx - ftempx) > distance)||(abs(fy - ftempy) > distance)) return FALSE;
			ftemp = SQRT(fx, fy, ftempx, ftempy);
			if(ftemp > distance) return FALSE;
			else return TRUE;
		}
		else
		{
			ftemp = MIN(SQRT(fx, fy, fsx, fsy), SQRT(fx, fy, fex, fey));
			if(ftemp > distance) return FALSE;
			else return TRUE;
		}
	}
}

void CESDDoc::InitLayerShowArray()
{
	m_arrayPcbLayerShow.RemoveAll();

	int layer = 0;
	for(int i=0; i<PCB_LAYER_NUM; i++)
	{
		if(((layer > BOTTOMLAYER)&&(layer < POWERPLANE1))||(layer > POWERPLANE16)) layer = BOTTOMLAYER;
		if(m_sPCB_LayerParam[layer].bLayerShow == TRUE) m_arrayPcbLayerShow.Add(layer);

		if(layer == BOTTOMLAYER) break;
		layer = m_sPCB_LayerParam[layer].nLayerNext;
	}

	for(int i=MECHANICAL1; i<=MECHANICAL16; i++)
	{
		if((m_sPCB_LayerParam[i].bLayerShow == TRUE)&&(m_sPCB_LayerParam[i].bLayerMechEnabled == TRUE)) m_arrayPcbLayerShow.Add(i);
	}

	for(int i=TOPOVERLAY; i<=MULTILAYER; i++)
	{
		if((i>=POWERPLANE1)&&(i<=POWERPLANE16)) continue;
		if((i>=MECHANICAL1)&&(i<=MECHANICAL16)) continue;
		if(m_sPCB_LayerParam[i].bLayerShow == TRUE) m_arrayPcbLayerShow.Add(i);
	}
}

BOOL CESDDoc::LayerVisible(int layer)
{
	for(int i=0; i<m_arrayPcbLayerShow.GetCount(); i++)
	{
		int l = m_arrayPcbLayerShow.GetAt(i);
		if(l == layer) return TRUE;
	}

	return FALSE;
}

void CESDDoc::OnViewGridsToggleVisible()
{
	if(m_cPCB_Sheet.m_bDotGrid == TRUE) m_cPCB_Sheet.m_bDotGrid = FALSE;
	else m_cPCB_Sheet.m_bDotGrid = TRUE;

   POSITION pos = GetFirstViewPosition();
   while (pos != NULL)
   {
      CView* pView = GetNextView(pos);
	  pView->Invalidate();
   }
}

void CESDDoc::OnViewToggleUnits()
{
	if(m_cPCB_Sheet.m_nUnit == UNIT_PCB_I) m_cPCB_Sheet.m_nUnit = UNIT_PCB_M;
	else m_cPCB_Sheet.m_nUnit = UNIT_PCB_I;
}

void CESDDoc::OnPcbViewSnapSetup()
{
	CDlg_PCBSnapSetup dlg;
	dlg.m_csEditSnap = Convert_XY_To_String(m_cPCB_Sheet.m_fSnapGridSize);
	if(dlg.DoModal() == IDOK)
	{
		m_cPCB_Sheet.m_fSnapGridSize = Convert_PCB_String_To_XY(dlg.m_csEditSnap);
		if(m_cPCB_Sheet.m_fSnapGridSize < 1) m_cPCB_Sheet.m_fSnapGridSize = 1;
		else if(m_cPCB_Sheet.m_fSnapGridSize > 1000) m_cPCB_Sheet.m_fSnapGridSize = 1000;
		m_cPCB_Sheet.m_fSnapGridSizeX = m_cPCB_Sheet.m_fSnapGridSizeY = m_cPCB_Sheet.m_fSnapGridSize;
	}
}

void CESDDoc::DeletePCBLayerObject(int layer)
{
	for(int i=0; i<m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = (COb_pcbarc*)m_arrayPcbArc.GetAt(i);
		if(parc->m_nLayer == layer) parc->m_nFlag = FLAG_DELETED;
	}
	for(int i=0; i<m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = (COb_pcbtrack*)m_arrayPcbTrack.GetAt(i);
		if(ptrack->m_nLayer == layer) ptrack->m_nFlag = FLAG_DELETED;
	}
	for(int i=0; i<m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = (COb_pcbtext*)m_arrayPcbText.GetAt(i);
		if(ptext->m_nLayer == layer) ptext->m_nFlag = FLAG_DELETED;
	}
	for(int i=0; i<m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = (COb_pcbfill*)m_arrayPcbFill.GetAt(i);
		if(pfill->m_nLayer == layer) pfill->m_nFlag = FLAG_DELETED;
	}
	for(int i=0; i<m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = (COb_pcbregion*)m_arrayPcbRegion.GetAt(i);
		if(pregion->m_nLayer == layer) pregion->m_nFlag = FLAG_DELETED;
	}
	for(int i=0; i<m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = (COb_pcbpolygon*)m_arrayPcbPolygon.GetAt(i);
		if(ppolygon->m_nLayer == layer) ppolygon->m_nFlag = FLAG_DELETED;
	}
	for(int i=0; i<m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = (COb_pcbpad*)m_arrayPcbPad.GetAt(i);
		if(ppad->m_nLayer == layer) ppad->m_nFlag = FLAG_DELETED;
	}
	for(int i=0; i<m_arrayPcbCoordinate.GetCount(); i++)
	{
		COb_pcbcoordinate* pcoordinate = (COb_pcbcoordinate*)m_arrayPcbCoordinate.GetAt(i);
		if(pcoordinate->m_nLayer == layer) pcoordinate->m_nFlag = FLAG_DELETED;
	}
	for(int i=0; i<m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = (COb_pcbdimension*)m_arrayPcbDimension.GetAt(i);
		if(pdimension->m_nLayer == layer) pdimension->m_nFlag = FLAG_DELETED;
	}
}

void CESDDoc::AddPCBInternalPlaneObject(int layer)
{
	double startx, starty;
	if((layer < POWERPLANE1)||(layer > POWERPLANE16)) return;

	int nNet = -1;
	for(int i=0; i<m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = (COb_pcbnet*)m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;
	
		if(pnet->m_csNetName == m_csPowerPlane[layer - POWERPLANE1]) 
		{
			nNet = i;
			break;
		}
	}

	COb_pcbregion aregion;
	aregion.pDocument = this;
	aregion.m_nLayer = layer;
	for(int i=0; i<m_cPCB_Sheet.m_cVertex.GetCount(); i++)
	{
 		Struct_RegionVertex vertex;
 		vertex = m_cPCB_Sheet.m_cVertex.GetAt(i);
		aregion.m_cVertex.Add(vertex);
	}

	CArray<Fpoint, Fpoint&> arrayVertex;	
	aregion.GetRegionVertex(arrayVertex);
	//CleanVertex(arrayVertex);

	COb_pcbpolygon* ppolygon = new COb_pcbpolygon();
	ppolygon->pDocument = this;
	ppolygon->m_nNet = nNet;
	ppolygon->m_nLayer = layer;

	ppolygon->m_bLocked = FALSE;
	ppolygon->m_nPolygonType = SPLITPLANE;
	ppolygon->m_bPrimitiveLocked = TRUE;
	ppolygon->m_bPourOver = FALSE;
	ppolygon->m_bRemoveDead = FALSE;

	ppolygon->m_fGridSize = 20;
	ppolygon->m_fTrackWidth = 0.01;
	ppolygon->m_nHatchStyle = 0;
	ppolygon->m_bUseOctagons = FALSE;
	ppolygon->m_fMinPrimLength = 3;
	
	ppolygon->m_bShelved = FALSE;
	ppolygon->m_nRestoreLayer = -1;
	ppolygon->m_bRemoveIslandsByArea = TRUE;
	ppolygon->m_bRemoveNecks = TRUE;
	ppolygon->m_fAreaThreshold = 2500;
	ppolygon->m_fArcResolution = 0.5;
	ppolygon->m_fNeckWidthThreshold = 5;
	ppolygon->m_nPourOverStyle = 2;

	ppolygon->m_csName.Empty();
	ppolygon->m_nPourIndex = -1;
	ppolygon->m_bIgnoreViolations = FALSE;

	ppolygon->m_bPolygonOutline = FALSE;
	ppolygon->m_bUserRouted = TRUE;
	ppolygon->m_nUnionIndex = 0;
	ppolygon->m_nSubPolyIndex = 0;
 		
	for(int i=0; i<arrayVertex.GetCount(); i++)
	{
		Fpoint fp;
		fp = arrayVertex.GetAt(i);
		Struct_RegionVertex vertex;
		vertex.cx = vertex.cy = vertex.radius = vertex.sangle = vertex.eangle = 0;
		vertex.fx = fp.fx;
		vertex.fy = fp.fy;
		ppolygon->m_cVertex.Add(vertex);
	}

	m_arrayPcbPolygon.Add(ppolygon);

	for(int i=0; i<arrayVertex.GetCount(); i++)
	{
		Fpoint fp1, fp2;
		fp1 = arrayVertex.GetAt(i);
		if(i == (arrayVertex.GetCount() - 1)) fp2 = arrayVertex.GetAt(0);
		else fp2 = arrayVertex.GetAt(i+1);

		if((fp1.fx == fp2.fx)&&(fp1.fy == fp2.fy)) continue;
	
		//创建track
		COb_pcbtrack* pnewtrack = new COb_pcbtrack();
		pnewtrack->pDocument = this;
		pnewtrack->m_nNet = -1;
		pnewtrack->m_nComponent = -1;
		pnewtrack->m_nPolygon = -2;	
		pnewtrack->m_fWidth = m_fPlanePullBack[layer - POWERPLANE1]*2;
		pnewtrack->m_nLayer = layer;
		pnewtrack->m_fX1 = fp1.fx;
		pnewtrack->m_fY1 = fp1.fy;
		pnewtrack->m_fX2 = fp2.fx;
		pnewtrack->m_fY2 = fp2.fy;

		m_arrayPcbTrack.Add(pnewtrack);
	}
}

void CESDDoc::mil_to_str(double fx, char *string, int format, int precision)
{
	int flag = 0;
    int ndig = 10;
    int dec,sign;
    char xx[100],*str;

	//double  fx:	Number to be converted 
	//precision:	Number of digits after decimal point 
	//int *dec:	Pointer to stored decimal-point position 
	//int *sign:	Pointer to stored sign indicator 
	str = _fcvt(fx, precision, &dec, &sign);

	if(strlen(str) > 0)
	{
		if(dec<=0)
		{
			if(dec == 0) strcpy(xx, "0.");
			else if(dec == -1) strcpy(xx, "0.0");
			else if(dec == -2) strcpy(xx, "0.00");
			else if(dec == -3) strcpy(xx, "0.000");
			else if(dec == -4) strcpy(xx, "0.0000");
			else if(dec == -5) strcpy(xx, "0.00000");
			else if(dec == -6) strcpy(xx, "0.000000");
			strcat(xx, str);
		}
		else
		{
			for(int i=0; i<dec; i++)
			{
				xx[i] = str[i];
				xx[i+1] = NULL;
			}
			if((int)strlen(str) > dec )
			{
				strcat(xx,".");
				strcat(xx, str+dec);
			}
		}
	}
	else strcpy(xx, NULL);

	CString cstring = xx;
	if(cstring.Find('.') >= 0)
	{
		for(;;)
		{
			int length = strlen(xx);
			if(xx[length - 1] == '0')
			{
				xx[length - 1] = '\0';
			}
			else if(xx[length - 1] == '.')
			{
				xx[length - 1] = '\0';
				break;
			}
			else break;
		}	
	}

	if(format == 0)
	{
		strcpy(string, xx);
	}
	else if(format == 1)
	{
		strcat(xx, "mil");
		strcpy(string ,xx);
	}
}

void CESDDoc::OnPcbLayerSet()
{
	CDlg_PCBLayerSet dlg;
	dlg.pDocument = this;

	dlg.DoModal();
}

void CESDDoc::OnReportBoard()
{
	CDlg_PCBBoardInfo dlg;
	int num;

	num = 0;
	for(int i=0; i<m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = m_arrayPcbTrack.GetAt(i);
		if(ptrack->m_nFlag == FLAG_NORMAL) num++;
	}
	dlg.m_nTracks = num;

	num = 0;
	for(int i=0; i<m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = m_arrayPcbArc.GetAt(i);
		if(parc->m_nFlag == FLAG_NORMAL) num++;
	}
	dlg.m_nArcs = num;

	num = 0;
	for(int i=0; i<m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag == FLAG_NORMAL) num++;
	}
	dlg.m_nPads = num;

	num = 0;
	for(int i=0; i<m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag == FLAG_NORMAL) num++;
	}
	dlg.m_nVias = num;

	num = 0;
	for(int i=0; i<m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = m_arrayPcbRegion.GetAt(i);
		if(pregion->m_nFlag == FLAG_NORMAL) num++;
	}
	dlg.m_nRegions = num;

	num = 0;
	for(int i=0; i<m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag == FLAG_NORMAL)&&(ptext->m_nComponent == -1)) num++;
	}
	dlg.m_nTexts = num;

	num = 0;
	for(int i=0; i<m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = m_arrayPcbFill.GetAt(i);
		if(pfill->m_nFlag == FLAG_NORMAL) num++;
	}
	dlg.m_nFills = num;

	num = 0;
	for(int i=0; i<m_arrayPcbCoordinate.GetCount(); i++)
	{
		COb_pcbcoordinate* pcoordinate = m_arrayPcbCoordinate.GetAt(i);
		if(pcoordinate->m_nFlag == FLAG_NORMAL) num++;
	}
	dlg.m_nCoordinates = num;

	num = 0;
	for(int i=0; i<m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = m_arrayPcbDimension.GetAt(i);
		if(pdimension->m_nFlag == FLAG_NORMAL) num++;
	}
	dlg.m_nDimensions = num;

	num = 0;
	for(int i=0; i<m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = m_arrayPcbPolygon.GetAt(i);
		if((ppolygon->m_nFlag == FLAG_NORMAL)&&(ppolygon->m_nPolygonType == COPPER_POLYGON)) num++;
	}
	dlg.m_nPolygons = num;

	num = 0;
	for(int i=0; i<m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nHoleType == PADHOLE_ROUND)&&(ppad->m_fHoleDia != 0)) num++;
	}
	dlg.m_nRoundHoles = num;
	num=0;
	for(int i=0; i<m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag == FLAG_NORMAL)&&(pvia->m_fHoleDia != 0)) num++;
	}
	dlg.m_nRoundHoles += num;

	num = 0;
	for(int i=0; i<m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nHoleType == PADHOLE_SLOT)) num++;
	}
	dlg.m_nSlotHoles = num;

	num = 0;
	for(int i=0; i<m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nHoleType == PADHOLE_SQUARE)) num++;
	}
	dlg.m_nSquareHoles = num;

	dlg.m_nDRCViolations = m_arrayPcbDrcErrors.GetCount();

	dlg.m_nConnections = m_arrayPcbConnection.GetCount();

	num = 0;
	int top = 0, bottom = 0;
	for(int i=0; i<m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag == FLAG_NORMAL)
		{
			dlg.m_arrayCompName.Add(pcomp->GetPcbCompDesignator(i));
			num++;
			if(pcomp->m_nLayer == TOPLAYER) top++;
			else if(pcomp->m_nLayer == BOTTOMLAYER) bottom++;
		}
	}
	dlg.m_nComponents = num;
	dlg.m_nTopComponents = top;
	dlg.m_nBottomComponents = bottom;
	
	num = 0;
	for(int i=0; i<m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;
	
		dlg.m_arrayNetName.Add(pnet->m_csNetName);
		num++;
	}
	dlg.m_nNets = num;

	dlg.pDocument = this;

	dlg.DoModal();
}

#include "ESDDoc_Save.cpp"
#include "ESDDoc_Open.cpp"

void CESDDoc::OnPcbNetEdit()
{
	CDlg_PcbNetEdit dlg;
	dlg.pDocument = this;
	
	if(dlg.DoModal() == IDOK)
	{

	}
}

void CESDDoc::OnPcbNetCleanall()
{
	// TODO: 在此添加命令处理程序代码
}

void CESDDoc::OnPcbNetCleanone()
{
	// TODO: 在此添加命令处理程序代码
}

void CESDDoc::OnPcbNetClearall()
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	if(AfxMessageBox("确认删除所有网络?", MB_YESNO|MB_ICONWARNING) == IDYES)
	{
		for(int i=0; i<m_arrayPcbNet.GetCount(); i++)
		{
			COb_pcbnet* pnet = m_arrayPcbNet.GetAt(i);
			if(pnet->m_nFlag != FLAG_NORMAL) continue;

			if((m_listPcbUndo[m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = PCB_ELEMENT_NET;
			pundo->m_nIndex = i;
			m_listPcbUndo[m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			pnet->m_nFlag = FLAG_DELETED;
		}
		
		if(undo_flag == TRUE)
		{
			for(int j=0; j<m_arrayPcbArc.GetCount(); j++)
			{
				COb_pcbarc* parc = m_arrayPcbArc.GetAt(j);
				if(parc->m_nFlag == FLAG_NORMAL)
				{
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = j;
					pundo->pcbarc.Copy(parc);
					m_listPcbUndo[m_nPCB_Undo_Num].AddTail(pundo);

					parc->m_nNet = -1;
				}
			}
			for(int j=0; j<m_arrayPcbTrack.GetCount(); j++)
			{
				COb_pcbtrack* ptrack = m_arrayPcbTrack.GetAt(j);
				if(ptrack->m_nFlag == FLAG_NORMAL)
				{
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = j;
					pundo->pcbtrack.Copy(ptrack);
					m_listPcbUndo[m_nPCB_Undo_Num].AddTail(pundo);

					ptrack->m_nNet = -1;
				}
			}
			for(int j=0; j<m_arrayPcbPad.GetCount(); j++)
			{
				COb_pcbpad* ppad = m_arrayPcbPad.GetAt(j);
				if(ppad->m_nFlag == FLAG_NORMAL)
				{
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_PAD;
					pundo->m_nIndex = j;
					pundo->pcbpad.Copy(ppad);
					m_listPcbUndo[m_nPCB_Undo_Num].AddTail(pundo);

					ppad->m_nNet = -1;
				}
			}
			for(int j=0; j<m_arrayPcbVia.GetCount(); j++)
			{
				COb_pcbvia* pvia = m_arrayPcbVia.GetAt(j);
				if(pvia->m_nFlag == FLAG_NORMAL)
				{
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = j;
					pundo->pcbvia.Copy(pvia);
					m_listPcbUndo[m_nPCB_Undo_Num].AddTail(pundo);

					pvia->m_nNet = -1;
				}
			}
			for(int j=0; j<m_arrayPcbFill.GetCount(); j++)
			{
				COb_pcbfill* pfill = m_arrayPcbFill.GetAt(j);
				if(pfill->m_nFlag == FLAG_NORMAL)
				{
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = j;
					pundo->pcbfill.Copy(pfill);
					m_listPcbUndo[m_nPCB_Undo_Num].AddTail(pundo);

					pfill->m_nNet = -1;
				}
			}
			for(int j=0; j<m_arrayPcbPolygon.GetCount(); j++)
			{
				COb_pcbpolygon* ppolygon = m_arrayPcbPolygon.GetAt(j);
				if(ppolygon->m_nFlag == FLAG_NORMAL)
				{
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_POLYGON;
					pundo->m_nIndex = j;
					pundo->pcbpolygon.Copy(ppolygon);
					m_listPcbUndo[m_nPCB_Undo_Num].AddTail(pundo);

					ppolygon->m_nNet = -1;
				}
			}
			for(int j=0; j<m_arrayPcbRegion.GetCount(); j++)
			{
				COb_pcbregion* pregion = m_arrayPcbRegion.GetAt(j);
				if(pregion->m_nFlag == FLAG_NORMAL)
				{
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_REGION;
					pundo->m_nIndex = j;
					pundo->pcbregion.Copy(pregion);
					m_listPcbUndo[m_nPCB_Undo_Num].AddTail(pundo);

					pregion->m_nNet = -1;
				}
			}
			
			while (!m_arrayPcbConnection.IsEmpty())
			{
				delete m_arrayPcbConnection.GetAt(0);
				m_arrayPcbConnection.RemoveAt(0);
			}

			m_nPCB_Undo_Num++;
			if(m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) Undo_Buffer_Transfer();
		}
	}
}

BOOL CESDDoc::IsOnSignalLayer(int layer)
{
	if(layer == MULTILAYER) return TRUE;
	else if(((layer>= TOPLAYER)&&(layer <= BOTTOMLAYER)) || ((layer >= POWERPLANE1)&&(layer <= POWERPLANE16))) return TRUE;
	
	return FALSE;
}

BOOL CESDDoc::IsConnectedToVia(int nLayer, COb_pcbvia* pvia)
{
	if(nLayer == MULTILAYER) return TRUE;
	else if((pvia->m_nStartLayer == TOPLAYER)&&(pvia->m_nEndLayer == BOTTOMLAYER))
	{
		if(IsOnSignalLayer(nLayer) == TRUE) return TRUE;
	}
	else
	{
		int flag = 0;
		int layer = 0;
		for(int i=0; i<PCB_LAYER_NUM; i++)
		{
			if(layer == pvia->m_nStartLayer) flag = 1;
			if((flag == 1)&&(nLayer == layer)) return TRUE;

			if(layer == pvia->m_nEndLayer) break;
			if(layer == BOTTOMLAYER) break;
			layer = m_sPCB_LayerParam[layer].nLayerNext;
		}	
	}

	return FALSE;
}

BOOL CESDDoc::IsConnected_ViaToVia(COb_pcbvia* pvia1, COb_pcbvia* pvia2)
{
	if( ((pvia1->m_nStartLayer == TOPLAYER)&&(pvia1->m_nEndLayer == BOTTOMLAYER)) || ((pvia2->m_nStartLayer == TOPLAYER)&&(pvia2->m_nEndLayer == BOTTOMLAYER)) ) return TRUE;
	else
	{
		int flag1 = 0, flag2 = 0;
		int layer = 0;
		for(int i=0; i<PCB_LAYER_NUM; i++)
		{
			if(layer == pvia1->m_nStartLayer) flag1 = 1;
			if(layer == pvia2->m_nStartLayer) flag2 = 1;
			if((flag1 == 1)&&(flag2 == 1)) return TRUE;

			if(layer == pvia1->m_nEndLayer) break;
			if(layer == pvia2->m_nEndLayer) break;
			if(layer == BOTTOMLAYER) break;
			layer = m_sPCB_LayerParam[layer].nLayerNext;
		}	
	}
	
	return FALSE;
}


void CESDDoc::OnPcbDesignRules()
{
	CXTPPropertySheet ps(_T("设置设计规则"));
	CXTPPropertyPageListNavigator* pList = new CXTPPropertyPageListNavigator();	
	pList->SetListStyle(xtListBoxStandard); //	xtListBoxStandard,	xtListBoxOfficeXP,	xtListBoxOffice2007,
	ps.SetNavigator(pList);
	ps.SetResizable(FALSE);
	ps.m_psh.dwFlags |=  PSH_NOAPPLYNOW;

	CPage_PcbRules1 page1;
	page1.m_csEditClearance = Convert_XY_To_String(m_fRulesClearance);
	ps.AddPage(&page1);
	
	CPage_PcbRules2 page2;
	page2.m_csEditWidth = Convert_XY_To_String(m_fRulesTrackWidth);
	page2.m_csEditViaSize = Convert_XY_To_String(m_fRulesViaDia);
	page2.m_csEditHoleSize = Convert_XY_To_String(m_fRulesViaHoleDia);
	ps.AddPage(&page2);

	CPage_PcbRules3 page3;
	page3.pDocument = this;
	page3.m_nComboTopology = m_nRulesTopology;
	page3.m_nComboStyle = m_nRulesCornerStyle;
	page3.m_csEditSetBack = Convert_XY_To_String(m_fRulesCornerSetback);
	ps.AddPage(&page3);
	
	CPage_PcbRules4 page4;
	page4.m_csEditSolder = Convert_XY_To_String(m_fSolderMaskExpansion);
	page4.m_csEditPaste = Convert_XY_To_String(m_fPasteMaskExpansion);
	ps.AddPage(&page4);

	CPage_PcbRules5 page5;
	if(m_nPlaneConductors == 2) page5.m_nRadioType = 0;
	else page5.m_nRadioType = 1;
	page5.m_nComboStyle = m_nPlaneConnectStyle;
	page5.m_csEditClearance = Convert_XY_To_String(m_fPlaneClearance);
	page5.m_csEditWidth = Convert_XY_To_String(m_fPlaneConductorWidth);
	page5.m_csEditGap = Convert_XY_To_String(m_fPlaneAirGap);
	page5.m_csEditExpansion = Convert_XY_To_String(m_fPlaneExpansion);
	ps.AddPage(&page5);

	CPage_PcbRules6 page6;
	if(m_nPolygonConductors == 2) page6.m_nRadioType = 0;
	else page6.m_nRadioType = 1;
	page6.m_nComboStyle = m_nPolygonConnectStyle;
	page6.m_csEditWidth = Convert_XY_To_String(m_fPolygonConductorWidth);
	ps.AddPage(&page6);

	CPage_PcbRules7 page7;
	ps.AddPage(&page7);

	if(ps.DoModal() == IDOK)
	{
		m_fRulesClearance = Convert_PCB_String_To_XY(page1.m_csEditClearance);

		m_fRulesTrackWidth = Convert_PCB_String_To_XY(page2.m_csEditWidth);
		m_fRulesViaDia = Convert_PCB_String_To_XY(page2.m_csEditViaSize);
		m_fRulesViaHoleDia = Convert_PCB_String_To_XY(page2.m_csEditHoleSize);

		m_nRulesTopology = page3.m_nComboTopology;
		m_nRulesCornerStyle = page3.m_nComboStyle;
		m_fRulesCornerSetback = Convert_PCB_String_To_XY(page3.m_csEditSetBack);

		CXTPReportRecords* pRecords = page3.m_wndReportCtrl.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			
			CXTPReportRecordItemNumber* pNumber = (CXTPReportRecordItemNumber*)pRecord->GetItem(2);
			int layer = pNumber->GetValue();
			CXTPReportRecordItemText* pText = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			CString cstring = pText->GetValue();
			int style;
			if(cstring == "禁止布线") style = 0;
			else if(cstring == "水平布线") style = 1;
			else if(cstring == "垂直布线") style = 2;
			else if(cstring == "任意角度") style = 3;

			m_nRulesLayer[layer] = style;
		}

		m_arrayRulesPriority.RemoveAll();
		pRecords = page3.m_wndReportCtrl_Priority.GetRecords();
		for(int i=0; i<pRecords->GetCount(); i++)
		{
			Struct_Route_Priority priority;
			CXTPReportRecord* pRecord = pRecords->GetAt(i);
			
			CXTPReportRecordItemText* pType = (CXTPReportRecordItemText*)pRecord->GetItem(0);
			CString csType = pType->GetValue();
			if(csType == "网络") priority.nType = 0;
			else priority.nType = 1;

			CXTPReportRecordItemText* pName = (CXTPReportRecordItemText*)pRecord->GetItem(1);
			priority.csName = pName->GetValue();

			CXTPReportRecordItemText* pPriority = (CXTPReportRecordItemText*)pRecord->GetItem(2);
			CString csPriority = pPriority->GetValue();
			char str[100];
			strcpy(str, csPriority);
			priority.nPriority = atoi(str);

			if(priority.csName != "未定义") m_arrayRulesPriority.Add(priority);
		}

		m_fSolderMaskExpansion = Convert_PCB_String_To_XY(page4.m_csEditSolder);
		m_fPasteMaskExpansion = Convert_PCB_String_To_XY(page4.m_csEditPaste);

		if(page5.m_nRadioType == 0) m_nPlaneConductors = 2;
		else m_nPlaneConductors = 4;
		m_nPlaneConnectStyle = page5.m_nComboStyle;
		m_fPlaneClearance = Convert_PCB_String_To_XY(page5.m_csEditClearance);
		m_fPlaneConductorWidth = Convert_PCB_String_To_XY(page5.m_csEditWidth);
		m_fPlaneAirGap = Convert_PCB_String_To_XY(page5.m_csEditGap);
		m_fPlaneExpansion = Convert_PCB_String_To_XY(page5.m_csEditExpansion);

		if(page6.m_nRadioType == 0) m_nPolygonConductors = 2;
		else m_nPolygonConductors = 4;
		m_nPolygonConnectStyle = page6.m_nComboStyle;
		m_fPolygonConductorWidth = Convert_PCB_String_To_XY(page6.m_csEditWidth);
	}
}
