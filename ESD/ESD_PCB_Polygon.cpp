#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_PCBEditPolygon.h"
#include "Dlg_PCBEditSplitPlane.h"

IMPLEMENT_SERIAL(COb_pcbpolygon, CObject, 0)
COb_pcbpolygon::COb_pcbpolygon()
{
	m_cVertex.RemoveAll();

	m_nNet = -1;

	m_bLocked = FALSE;


	//------ 下面参数的意义有待明确
	m_bPolygonOutline = FALSE;
	m_bUserRouted = TRUE;
	m_nUnionIndex = -1;
	m_nSubPolyIndex = -1;

	m_fPourGap = 20;

	m_bSelection = FALSE;	
	m_nFlag = 0;
}



void COb_pcbpolygon::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nNet;	
		ar << m_nLayer;

		ar << m_bLocked;
		ar << m_nPolygonType;  // Polygon, Split Plane
		ar << m_bPrimitiveLocked;
		ar << m_bPourOver;
		ar << m_bRemoveDead;

		ar << m_fGridSize;
		ar << m_fTrackWidth;
		ar << m_nHatchStyle;	//90 Degree, 45 Degree, Horizontal, Vertical
		ar << m_bUseOctagons;	//Arcs, Octagons
		ar << m_fMinPrimLength;
		
		int num = m_cVertex.GetCount();
		ar << num;
		for(int i=0; i<num; i++)
		{
			Struct_RegionVertex vertex;
			vertex = m_cVertex.GetAt(i);
			ar << vertex.cx;
			ar << vertex.cy;
			ar << vertex.eangle;
			ar << vertex.fx;
			ar << vertex.fy;
			ar << vertex.radius;
			ar << vertex.sangle;
		}

		ar << m_bShelved;
		ar << m_nRestoreLayer;
		ar << m_bRemoveIslandsByArea;
		ar << m_bRemoveNecks;
		ar << m_fAreaThreshold;
		ar << m_fArcResolution;
		ar << m_fNeckWidthThreshold;
		ar << m_nPourOverStyle;

		ar << m_csName;
		ar << m_nPourIndex;
		ar << m_bIgnoreViolations;

		ar << m_bPolygonOutline;
		ar << m_bUserRouted;
		ar << m_nUnionIndex;
		ar << m_nSubPolyIndex;

		ar << m_fPourGap;

		ar << m_bSelection;	
	}
	else
	{
		ar >> m_nNet;	
		ar >> m_nLayer;

		ar >> m_bLocked;
		ar >> m_nPolygonType;  // Polygon, Split Plane
		ar >> m_bPrimitiveLocked;
		ar >> m_bPourOver;
		ar >> m_bRemoveDead;

		ar >> m_fGridSize;
		ar >> m_fTrackWidth;
		ar >> m_nHatchStyle;	//90 Degree, 45 Degree, Horizontal, Vertical
		ar >> m_bUseOctagons;	//Arcs, Octagons
		ar >> m_fMinPrimLength;
		
		int num;
		ar >> num;
		for(int i=0; i<num; i++)
		{
			Struct_RegionVertex vertex;
			ar >> vertex.cx;
			ar >> vertex.cy;
			ar >> vertex.eangle;
			ar >> vertex.fx;
			ar >> vertex.fy;
			ar >> vertex.radius;
			ar >> vertex.sangle;
			m_cVertex.Add(vertex);
		}

		ar >> m_bShelved;
		ar >> m_nRestoreLayer;
		ar >> m_bRemoveIslandsByArea;
		ar >> m_bRemoveNecks;
		ar >> m_fAreaThreshold;
		ar >> m_fArcResolution;
		ar >> m_fNeckWidthThreshold;
		ar >> m_nPourOverStyle;

		ar >> m_csName;
		ar >> m_nPourIndex;
		ar >> m_bIgnoreViolations;

		ar >> m_bPolygonOutline;
		ar >> m_bUserRouted;
		ar >> m_nUnionIndex;
		ar >> m_nSubPolyIndex;

		ar >> m_fPourGap;

		ar >> m_bSelection;	
	}
}

BOOL  COb_pcbpolygon::Is_On_Me(double fx, double fy)
{
	COb_pcbregion aregion;
	aregion.pDocument = pDocument;
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex vertex;
		vertex = m_cVertex.GetAt(i);

		aregion.m_cVertex.Add(vertex);
	}

	CArray<Fpoint, Fpoint&> arrayVertex;	
	
	POINT* p;	
	aregion.GetRegionVertex(arrayVertex);

	int vertex_number = arrayVertex.GetSize();
	p = (POINT*)malloc(2*vertex_number*sizeof(int));
	for(int i=0; i<vertex_number; i++)  
	{
		p[i].x = (int)arrayVertex[i].fx;
		p[i].y = (int)arrayVertex[i].fy;
	}

	CRgn rgn;
	rgn.CreatePolygonRgn(p, vertex_number, ALTERNATE);

	free(p);

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;
	return FALSE;
}

BOOL  COb_pcbpolygon::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		double fx, fy;
		fx = m_cVertex[i].fx;
		fy = m_cVertex[i].fy;
		if((fx < fx1)||(fx > fx2)||(fy < fy1)||(fy > fy2)) return FALSE;
	}
	return TRUE;
}

void COb_pcbpolygon::ChangePolygon(CView* pV, int index)
{
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	CDlg_PCBEditPolygon dlg;

	if(m_nHatchStyle == PCB_HATCH_SOLID) dlg.m_nFillMode = 0;
	else if((m_nHatchStyle ==  PCB_HATCH_90) || (m_nHatchStyle ==  PCB_HATCH_45) || (m_nHatchStyle ==  PCB_HATCH_H) || (m_nHatchStyle ==  PCB_HATCH_V)) dlg.m_nFillMode = 1;
	else dlg.m_nFillMode = 2;
	
	dlg.m_bCheckRemoveIsland = m_bRemoveIslandsByArea;
	dlg.m_nEditRemoveIsland = (int)ceil(m_fAreaThreshold);
	dlg.m_bCheckRemoveNeck = m_bRemoveNecks;
	dlg.m_csEditRemoveNeck = pDoc->Convert_XY_To_String(m_fNeckWidthThreshold);
	dlg.m_csEditArcPrecision = pDoc->Convert_XY_To_String(m_fArcResolution);

	dlg.m_csEditPourGap = pDoc->Convert_XY_To_String(m_fPourGap);


	dlg.m_csEditTrackWidth = pDoc->Convert_XY_To_String(m_fTrackWidth);
	dlg.m_csEditGridSize = pDoc->Convert_XY_To_String(m_fGridSize);
	if(m_bUseOctagons == FALSE) dlg.m_nPadStyle = 0;
	else dlg.m_nPadStyle = 1;
	if(m_nHatchStyle ==  PCB_HATCH_45) dlg.m_nHatchStyle = 1;
	else if(m_nHatchStyle ==  PCB_HATCH_H) dlg.m_nHatchStyle = 2;
	else if(m_nHatchStyle ==  PCB_HATCH_V) dlg.m_nHatchStyle = 3;
	else dlg.m_nHatchStyle = 0;

	dlg.m_csName = m_csName;

	for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
	{
		dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
	}
	for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
	{
		if(m_nLayer == pDoc->m_arrayPcbLayerShow.GetAt(i))
		{
			dlg.m_nComboLayer = i;
			break;
		}
	}
	dlg.m_csEditMinLength = pDoc->Convert_XY_To_String(m_fMinPrimLength);
	dlg.m_bCheckPrimitiveLock = m_bPrimitiveLocked;
	dlg.m_bCheckLocked = m_bLocked;
	dlg.m_bCheckIgnoreViolation = m_bIgnoreViolations;

	dlg.arrayStringNet.Add("无网络");
	for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = (COb_pcbnet*)pDoc->m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;
	
		dlg.arrayStringNet.Add(pnet->m_csNetName);
	}
	dlg.m_csComboNet = dlg.arrayStringNet.GetAt(m_nNet + 1);

	dlg.m_nComboPourStyle = m_nPourOverStyle;
	dlg.m_bCheckRemoveDead = FALSE;

	if(dlg.DoModal() == IDOK)
	{
		
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = PCB_ELEMENT_POLYGON;
		pundo->m_nIndex = index;
		pundo->pcbpolygon.Copy(this);
			
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		
		
		
		if(dlg.m_nFillMode == 0) m_nHatchStyle = PCB_HATCH_SOLID;
		else if(dlg.m_nFillMode == 2) m_nHatchStyle = PCB_HATCH_NONE;
		else
		{
			if(dlg.m_nHatchStyle == 1) m_nHatchStyle = PCB_HATCH_45;
			else if(dlg.m_nHatchStyle == 2) m_nHatchStyle = PCB_HATCH_H;
			else if(dlg.m_nHatchStyle == 3) m_nHatchStyle = PCB_HATCH_V;
			else m_nHatchStyle = PCB_HATCH_90;
		}

		
		m_bRemoveIslandsByArea = dlg.m_bCheckRemoveIsland;
		m_fAreaThreshold = (double)(dlg.m_nEditRemoveIsland);
		m_bRemoveNecks = dlg.m_bCheckRemoveNeck;
		m_fNeckWidthThreshold = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditRemoveNeck);
		m_fArcResolution = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditArcPrecision);
		
		m_fPourGap = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditPourGap);
		if(m_fPourGap < pDoc->m_fRulesClearance) m_fPourGap = pDoc->m_fRulesClearance;
		
		
		m_fTrackWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTrackWidth);
		m_fGridSize = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditGridSize);
		if(dlg.m_nPadStyle == 0) m_bUseOctagons = FALSE;
		else m_bUseOctagons = TRUE;
		
		
		m_csName = dlg.m_csName;
		
		for(int i=0; i<=MULTILAYER; i++)
		{
			if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
			{
				m_nLayer = i;
				break;
			}
		}
		m_fMinPrimLength = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditMinLength);
		m_bPrimitiveLocked = dlg.m_bCheckPrimitiveLock;
		m_bLocked = dlg.m_bCheckLocked;
		m_bIgnoreViolations = dlg.m_bCheckIgnoreViolation;

		
		for(int i=0; i<dlg.arrayStringNet.GetCount(); i++)
		{
			if(dlg.arrayStringNet.GetAt(i) == dlg.m_csComboNet)
			{
				m_nNet = i-1;
			}
		}
		m_nPourOverStyle = dlg.m_nComboPourStyle;
		m_bRemoveDead = dlg.m_bCheckRemoveDead;
		
		if(AfxMessageBox("重新构建覆铜区域?", MB_YESNO|MB_ICONWARNING) == IDYES)
		{
		
			for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
				if((parc->m_nPolygon == index)&&(parc->m_nFlag == FLAG_NORMAL))
				{
					parc->m_bSelection = FALSE;
					parc->m_nFlag = FLAG_DELETED;
		
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = i;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		
				}
			}
			for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
				if((ptrack->m_nPolygon == index)&&(ptrack->m_nFlag == FLAG_NORMAL))
				{
					ptrack->m_bSelection = FALSE;
					ptrack->m_nFlag = FLAG_DELETED;
		
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = i;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		
				}
			}
			for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
			{
				COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
				if((pregion->m_nPolygon == index)&&(pregion->m_nFlag == FLAG_NORMAL))
				{
					pregion->m_bSelection = FALSE;
					pregion->m_nFlag = FLAG_DELETED;
		
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_REGION;
					pundo->m_nIndex = i;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		
				}
			}

			pView->CreatePolygonPour(this, index);

		}

		
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		

	}
}

void COb_pcbpolygon::ChangeSplitPlane(int index)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_PCBEditSplitPlane dlg;
	dlg.arrayStringNet.Add("无网络");
	for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = (COb_pcbnet*)pDoc->m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;
	
		dlg.arrayStringNet.Add(pnet->m_csNetName);
	}
	dlg.m_csComboNet = dlg.arrayStringNet.GetAt(m_nNet + 1);
	if(dlg.DoModal() == IDOK)
	{
		for(int i=0; i<dlg.arrayStringNet.GetCount(); i++)
		{
			if(dlg.arrayStringNet.GetAt(i) == dlg.m_csComboNet)
			{
				m_nNet = i-1;
			}
		}
	}
}
void COb_pcbpolygon::Copy(COb_pcbpolygon* ppolygon)
{
	pDocument = ppolygon->pDocument;
	
	m_cVertex.SetSize(ppolygon->m_cVertex.GetCount());
	for(int i=0; i<ppolygon->m_cVertex.GetCount(); i++)
	{
		m_cVertex[i].fx = ppolygon->m_cVertex[i].fx;
		m_cVertex[i].fy = ppolygon->m_cVertex[i].fy;
		m_cVertex[i].cx = ppolygon->m_cVertex[i].cx;
		m_cVertex[i].cy = ppolygon->m_cVertex[i].cy;
		m_cVertex[i].sangle = ppolygon->m_cVertex[i].sangle;
		m_cVertex[i].eangle = ppolygon->m_cVertex[i].eangle;
		m_cVertex[i].radius = ppolygon->m_cVertex[i].radius;
	}
	
	m_nNet = ppolygon->m_nNet;
	m_nLayer = ppolygon->m_nLayer;

	m_bLocked = ppolygon->m_bLocked;
	m_nPolygonType = ppolygon->m_nPolygonType;
	m_bPrimitiveLocked = ppolygon->m_bPrimitiveLocked;
	m_bPourOver = ppolygon->m_bPourOver;
	m_bRemoveDead = ppolygon->m_bRemoveDead;

	m_fGridSize = ppolygon->m_fGridSize;
	m_fTrackWidth = ppolygon->m_fTrackWidth;
	m_nHatchStyle = ppolygon->m_nHatchStyle;
	m_bUseOctagons = ppolygon->m_bUseOctagons;
	m_fMinPrimLength = ppolygon->m_fMinPrimLength;
	
	m_bShelved = ppolygon->m_bShelved;
	m_nRestoreLayer = ppolygon->m_nRestoreLayer;
	m_bRemoveIslandsByArea = ppolygon->m_bRemoveIslandsByArea;
	m_bRemoveNecks = ppolygon->m_bRemoveNecks;
	m_fAreaThreshold = ppolygon->m_fAreaThreshold;
	m_fArcResolution = ppolygon->m_fArcResolution;
	m_fNeckWidthThreshold = ppolygon->m_fNeckWidthThreshold;
	m_nPourOverStyle = ppolygon->m_nPourOverStyle;
	m_csName = ppolygon->m_csName;
	m_nPourIndex = ppolygon->m_nPourIndex;
	m_bIgnoreViolations = ppolygon->m_bIgnoreViolations;

	m_bPolygonOutline = ppolygon->m_bPolygonOutline;
	m_bUserRouted = ppolygon->m_bUserRouted;
	m_nUnionIndex = ppolygon->m_nUnionIndex;
	m_nSubPolyIndex = ppolygon->m_nSubPolyIndex;

	m_fPourGap = ppolygon->m_fPourGap;

	m_bSelection = FALSE; //ppolygon->m_bSelection;
	m_nFlag = ppolygon->m_nFlag;
}

RECT COb_pcbpolygon::GetPolygonRect()
{
	double minfx = 1000000.0f;
	double minfy = 1000000.0f;
	double maxfx = -1000000.0f;
	double maxfy = -1000000.0f;
	for(int i=0; i<m_cVertex.GetSize(); i++)
	{
		minfx = MIN(minfx, m_cVertex[i].fx);
		minfy = MIN(minfy, m_cVertex[i].fy);
		maxfx = MAX(maxfx, m_cVertex[i].fx);
		maxfy = MAX(maxfy, m_cVertex[i].fy);
	}

	RECT rect;
	rect.left = (int)minfx;
	rect.right = (int)maxfx;
	rect.bottom = (int)minfy;
	rect.top = (int)maxfy;

	return rect;
}

void COb_pcbpolygon::Move(CClientDC* pDC, CView* pV, int index)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动覆铜区: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	pView->CloseMouseCursor();

	x0 = m_cVertex[0].fx;
	y0 = m_cVertex[0].fy;
	
	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_pcbregion aregion;
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex vertex;
		vertex = m_cVertex.GetAt(i);
		aregion.m_cVertex.Add(vertex);
	}

	aregion.m_nLayer = m_nLayer;
	aregion.m_bSelection = m_bSelection;
	aregion.m_nFlag = m_nFlag;

	aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);
	pView->OpenMouseCursor();

	flag = 0;
	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);
			pView->OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			if(pView->m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(pView->m_bAfterSetFocus)
			{
				pView->m_bAfterSetFocus = false;
				continue;
			}
			
			
			if(nEDA_RedrawFlag == 1)
			{ 
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				pView->CloseMouseCursor();
				aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aregion.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				//fx,fy
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].fx = fx + x0 + deltax;
				aregion.m_cVertex[i].fy = fy + y0 + deltay;
				//cx,cy
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].cx-x0, m_cVertex[i].cy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].cx = fx + x0 + deltax;
				aregion.m_cVertex[i].cy = fy + y0 + deltay;
			}
			
			pView->CloseMouseCursor();
			aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aregion.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].fx = fx + x0 + deltax;
				aregion.m_cVertex[i].fy = fy + y0 + deltay;
			}

	
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_POLYGON;
			pundo->m_nIndex = index;
			pundo->pcbpolygon.Copy(this);
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
	
			for(int i=0; i<aregion.m_cVertex.GetCount(); i++)
			{
				Struct_RegionVertex vertex;
				vertex = aregion.m_cVertex.GetAt(i);
				m_cVertex.SetAt(i,vertex);
			}
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aregion.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				//fx,fy
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].fx = fx + x0 + deltax;
				aregion.m_cVertex[i].fy = fy + y0 + deltay;
				//cx,cy
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].cx-x0, m_cVertex[i].cy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].cx = fx + x0 + deltax;
				aregion.m_cVertex[i].cy = fy + y0 + deltay;
				//sangle, eangle
				aregion.m_cVertex[i].sangle += 90;
				aregion.m_cVertex[i].sangle = pDoc->Get_Correct_Angle(aregion.m_cVertex[i].sangle);
				aregion.m_cVertex[i].eangle += 90;
				aregion.m_cVertex[i].eangle = pDoc->Get_Correct_Angle(aregion.m_cVertex[i].eangle);
			}
			aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);

			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		if(AfxMessageBox("重新构建覆铜区域?", MB_YESNO|MB_ICONWARNING) == IDYES)
		{
			for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
				if((parc->m_nPolygon == index)&&(parc->m_nFlag == FLAG_NORMAL))
				{
					parc->m_bSelection = FALSE;
					parc->m_nFlag = FLAG_DELETED;

					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = i;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

				}
			}
			for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
				if((ptrack->m_nPolygon == index)&&(ptrack->m_nFlag == FLAG_NORMAL))
				{
					ptrack->m_bSelection = FALSE;
					ptrack->m_nFlag = FLAG_DELETED;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = i;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
				}
			}
			for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
			{
				COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
				if((pregion->m_nPolygon == index)&&(pregion->m_nFlag == FLAG_NORMAL))
				{
					pregion->m_bSelection = FALSE;
					pregion->m_nFlag = FLAG_DELETED;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_REGION;
					pundo->m_nIndex = i;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
				}
			}

			pView->CreatePolygonPour(this, index);

		}
		else
		{
			for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
				if((parc->m_nPolygon == index)&&(parc->m_nFlag == FLAG_NORMAL))
				{
					parc->MoveAndRotate(pV, x0, y0, deltax, deltay, flag*90, FALSE, i);
				}
			}
			for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
				if((ptrack->m_nPolygon == index)&&(ptrack->m_nFlag == FLAG_NORMAL))
				{
					ptrack->MoveAndRotate(pV, x0, y0, deltax, deltay, flag*90, FALSE, i);
				}
			}
			for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
			{
				COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
				if((pregion->m_nPolygon == index)&&(pregion->m_nFlag == FLAG_NORMAL))
				{
					pregion->MoveAndRotate(pV, x0, y0, deltax, deltay, flag*90, FALSE, i);
				}
			}
		}

	
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	
	}

	pView->Write_Help(szEDA_Help_Ready);
}

BOOL COb_pcbpolygon::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	
	char szHelp[] = "复制覆铜区: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	pView->CloseMouseCursor();

	x0 = m_cVertex[0].fx;
	y0 = m_cVertex[0].fy;
	
	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_pcbregion aregion;
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex vertex;
		vertex = m_cVertex.GetAt(i);
		aregion.m_cVertex.Add(vertex);
	}
	//aregion.m_nHoleNum = 0;
	aregion.m_nLayer = m_nLayer;
	aregion.m_bSelection = m_bSelection;
	aregion.m_nFlag = m_nFlag;

	aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);
	pView->OpenMouseCursor();

	flag = 0;
	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);
			pView->OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			if(pView->m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(pView->m_bAfterSetFocus)
			{
				pView->m_bAfterSetFocus = false;
				continue;
			}
			
			
			if(nEDA_RedrawFlag == 1)
			{ 
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				pView->CloseMouseCursor();
				aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aregion.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				//fx,fy
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].fx = fx + x0 + deltax;
				aregion.m_cVertex[i].fy = fy + y0 + deltay;
				//cx,cy
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].cx-x0, m_cVertex[i].cy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].cx = fx + x0 + deltax;
				aregion.m_cVertex[i].cy = fy + y0 + deltay;
			}
			
			pView->CloseMouseCursor();
			aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aregion.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].fx = fx + x0 + deltax;
				aregion.m_cVertex[i].fy = fy + y0 + deltay;
			}

			for(int i=0; i<aregion.m_cVertex.GetCount(); i++)
			{
				Struct_RegionVertex vertex;
				vertex = aregion.m_cVertex.GetAt(i);
				m_cVertex.SetAt(i,vertex);
			}
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aregion.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				//fx,fy
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].fx = fx + x0 + deltax;
				aregion.m_cVertex[i].fy = fy + y0 + deltay;
				//cx,cy
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].cx-x0, m_cVertex[i].cy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].cx = fx + x0 + deltax;
				aregion.m_cVertex[i].cy = fy + y0 + deltay;
				//sangle, eangle
				aregion.m_cVertex[i].sangle += 90;
				aregion.m_cVertex[i].sangle = pDoc->Get_Correct_Angle(aregion.m_cVertex[i].sangle);
				aregion.m_cVertex[i].eangle += 90;
				aregion.m_cVertex[i].eangle = pDoc->Get_Correct_Angle(aregion.m_cVertex[i].eangle);
			}
			aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);

			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	ClipCursor(NULL); pView->m_bClipState = FALSE;

	pView->Write_Help(szEDA_Help_Ready);

	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;
}

BOOL COb_pcbpolygon::ShowPolygonOutline(int index)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if(parc->m_nPolygon == index) return FALSE;
	}
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if(ptrack->m_nPolygon == index) return FALSE;
	}
	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if(pregion->m_nPolygon == index) return FALSE;
	}

	return TRUE;
}

void COb_pcbpolygon::Draw(CDC* pDC, CView* pV)
{
	COLORREF color;
	CPoint point;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;

	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_PCBPrint_Color == 0) color = RGB(0,0,0);
		else if(nEDA_PCBPrint_Color == 2) color = RGB(GetGray( nEDA_PCB_LayerColor[m_nLayer]), GetGray( nEDA_PCB_LayerColor[m_nLayer]), GetGray( nEDA_PCB_LayerColor[m_nLayer]));
		else color = nEDA_PCB_LayerColor[m_nLayer]; 
	}
	else
	{
		if(m_nFlag == FLAG_NORMAL) color = nEDA_PCB_LayerColor[m_nLayer];
		else if(m_nFlag == FLAG_DELETED) color = nEDA_PCB_BoardAreaColor;
	}

	COb_pcbregion aregion;
	aregion.pDocument = pDocument;
	aregion.m_nLayer = m_nLayer;
	for(int i=0;i <m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex vertex;
		vertex = m_cVertex.GetAt(i);
		aregion.m_cVertex.Add(vertex);
	}
	
	if(aregion.PickMe(pV) == TRUE) aregion.FrameRegion(pDC, pV, 1, color);
}

void COb_pcbpolygon::DrawSelection(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pDocument;	
	
	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;
	
	CArray<Fpoint, Fpoint&> arrayVertex;	
	POINT* p;	
	GetPolygonVertex(arrayVertex);

	int vertex_number = arrayVertex.GetSize();
	p = (POINT*)malloc(2*vertex_number*sizeof(int));
	for(int i=0; i<vertex_number; i++)  
	{
		int x,y;
		pView->Get_VPort_XY(&x, &y, arrayVertex[i].fx, arrayVertex[i].fy);
		p[i].x = x;
		p[i].y = y;
	}

	CRgn rgn;
	rgn.CreatePolygonRgn(p, vertex_number, ALTERNATE);

	free(p);

	CPoint point;
	// Use the bit pattern to create a bitmap.
	//WORD HatchBits[8] = { 0x81, 0x42, 0x24, 0x18, 0x18,  0x24, 0x42, 0x81 };
	WORD HatchBits[8] = { 0x55, 0xaa, 0x55, 0xaa, 0x55,  0xaa, 0x55, 0xaa };
	CBitmap bm;
	bm.CreateBitmap(8,8,1,1, HatchBits);

	// Create a pattern brush from the bitmap.
	CBrush fillbrush;
	fillbrush.CreatePatternBrush(&bm);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	int mode = pDC->SetROP2(R2_MERGEPEN );
	pDC->FillRgn(&rgn, &fillbrush);
	
	pDC->SetROP2(mode);
	pDC->SelectObject(oldbrush);

	if(m_nPolygonType == COPPER_POLYGON)
	{
		for(int i =0; i<m_cVertex.GetCount(); i++)
		{
			int x,y;
			pView->Get_VPort_XY(&x,&y,m_cVertex[i].fx,m_cVertex[i].fy);
			pView->DrawSelectionKeyPoint(x, y, pDC);
		}
	}
}

void COb_pcbpolygon::GetPolygonVertex(CArray<Fpoint, Fpoint&>& arrayVertex)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	double startx, starty;
	Fpoint fp;

	arrayVertex.RemoveAll();
	for(int j=0; j<m_cVertex.GetCount(); j++)
	{
 		Struct_RegionVertex vertex;
 		vertex = m_cVertex.GetAt(j);
		
		if(j == 0)
		{
			startx = vertex.fx;
			starty = vertex.fy;
		}

		if(vertex.radius == 0)
		{
			fp.fx = vertex.fx;
			fp.fy = vertex.fy;
			arrayVertex.Add(fp);
		}
		else
		{
			double delta_angle;
			delta_angle = acos((vertex.radius - pDoc->m_fArcPrecision)/vertex.radius)*180/fEDA_pi;
			if(delta_angle <= 1) delta_angle = 1;

			fp.fx = vertex.fx;
			fp.fy = vertex.fy;
			arrayVertex.Add(fp);

			double sx, sy;//, ex, ey;
			sx = vertex.cx + vertex.radius*cos(vertex.sangle*fEDA_pi/180);
			sy = vertex.cy + vertex.radius*sin(vertex.sangle*fEDA_pi/180);
			
			if((fabs(vertex.fx - sx) < 0.01f)&&(fabs(vertex.fy - sy) < 0.01f))
			{
				for(;;)
				{
					if(vertex.eangle < vertex.sangle) vertex.eangle += 360;
					else break;
				}

				double f = vertex.sangle; 
				for(;;)
				{
					fp.fx = vertex.cx + vertex.radius*cos(f*fEDA_pi/180);
					fp.fy = vertex.cy + vertex.radius*sin(f*fEDA_pi/180);
					arrayVertex.Add(fp);

					//f += 5;
					f += delta_angle;
					if(f >= vertex.eangle)
					{
						f = vertex.eangle;
						fp.fx = vertex.cx + vertex.radius*cos(f*fEDA_pi/180);
						fp.fy = vertex.cy + vertex.radius*sin(f*fEDA_pi/180);
						arrayVertex.Add(fp);
						break;
					}
				}
			}
			else
			{
				for(;;)
				{
					if(vertex.eangle < vertex.sangle) vertex.eangle += 360;
					else break;
				}

				double f = vertex.eangle; 
				for(;;)
				{
					fp.fx = vertex.cx + vertex.radius*cos(f*fEDA_pi/180);
					fp.fy = vertex.cy + vertex.radius*sin(f*fEDA_pi/180);
					arrayVertex.Add(fp);

					//f -= 5;
					f -= delta_angle;
					if(f <= vertex.sangle)
					{
						f = vertex.sangle;
						fp.fx = vertex.cx + vertex.radius*cos(f*fEDA_pi/180);
						fp.fy = vertex.cy + vertex.radius*sin(f*fEDA_pi/180);
						arrayVertex.Add(fp);
						break;
					}
				}
			}
		}
	}

	if((fp.fx != startx)&&(fp.fy != starty))
	{
		fp.fx = startx;
		fp.fy = starty;
		arrayVertex.Add(fp);
	}
}