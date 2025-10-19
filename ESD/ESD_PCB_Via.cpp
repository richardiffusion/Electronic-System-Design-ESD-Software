#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_PcbEditVia.h"

IMPLEMENT_SERIAL(COb_pcbvia, CObject, 0)
COb_pcbvia::COb_pcbvia()
{
	m_nNet = -1;
	m_nPolygon = -1;
	m_nComponent = -1;	

	m_nLayer = MULTILAYER;
	m_bLocked = FALSE;

	m_nCPLV = 0;	
	m_nCPL = 0;
	
	m_nCSEV = 1;	
	m_fCSE = 4.0f;	

	m_nCPCV = 1;	
	m_fCPC = 20.0f;	

	m_nCCSV = 0;	
	m_nCCS = 0;		
	m_nCENV = 1;	
	m_nCEN = 4;		
	m_nCCWV = 1;	
	m_fCCW = 10.0f;	
	m_nCAGV = 1;	
	m_fCAG = 10.0f;	
	m_nCPRV = 1;	
	m_fCPR = 20.0f;	

	m_bTestingTop = FALSE;
	m_bTestingBottom = FALSE;
	m_bTenting = FALSE;
	m_bTentingTop = FALSE;
	m_bTentingBottom = FALSE;

	m_bSelection = FALSE;
	m_nFlag = 0;
}

void COb_pcbvia::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fCx;
		ar << m_fCy;
		ar << m_fDia;
		ar << m_fHoleDia;
		ar << m_nStartLayer;
		ar << m_nEndLayer;

		ar << m_nLayer;
		ar << m_nNet;
		ar << m_nComponent;
		ar << m_nPolygon;

		ar << m_bLocked;

		ar << m_bTestingTop;
		ar << m_bTestingBottom;
		ar << m_bTenting;
		ar << m_bTentingTop;
		ar << m_bTentingBottom;

		ar << m_bPolygonOutline;
		ar << m_bUserRouted;
		ar << m_nUnionIndex;

		ar << m_nCPLV;
		ar << m_nCPL;
		ar << m_nCSEV;
		ar << m_fCSE;
		ar << m_nCPCV;
		ar << m_fCPC;
		ar << m_nCCSV;
		ar << m_nCCS;
		ar << m_nCENV;
		ar << m_nCEN;
		ar << m_nCCWV;
		ar << m_fCCW;
		ar << m_nCAGV;
		ar << m_fCAG;
		ar << m_nCPRV;
		ar << m_fCPR;

		ar << m_bSelection;	
	}
	else
	{
		ar >> m_fCx;
		ar >> m_fCy;
		ar >> m_fDia;
		ar >> m_fHoleDia;
		ar >> m_nStartLayer;
		ar >> m_nEndLayer;

		ar >> m_nLayer;
		ar >> m_nNet;
		ar >> m_nComponent;
		ar >> m_nPolygon;

		ar >> m_bLocked;

		ar >> m_bTestingTop;
		ar >> m_bTestingBottom;
		ar >> m_bTenting;
		ar >> m_bTentingTop;
		ar >> m_bTentingBottom;

		ar >> m_bPolygonOutline;
		ar >> m_bUserRouted;
		ar >> m_nUnionIndex;

		ar >> m_nCPLV;
		ar >> m_nCPL;
		ar >> m_nCSEV;
		ar >> m_fCSE;
		ar >> m_nCPCV;
		ar >> m_fCPC;
		ar >> m_nCCSV;
		ar >> m_nCCS;
		ar >> m_nCENV;
		ar >> m_nCEN;
		ar >> m_nCCWV;
		ar >> m_fCCW;
		ar >> m_nCAGV;
		ar >> m_fCAG;
		ar >> m_nCPRV;
		ar >> m_fCPR;

		ar >> m_bSelection;		
	}
}

BOOL COb_pcbvia::IsVisible()
{	
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if((m_nStartLayer == TOPLAYER)&&(m_nEndLayer == BOTTOMLAYER))
	{
		if(pDoc->m_sPCB_LayerParam[MULTILAYER].bLayerShow == TRUE) return TRUE;
	}
	int layer = TOPLAYER;
	int flag = 0;
	for(int i=0; i<PCB_LAYER_NUM; i++)
	{
		if((layer == m_nStartLayer)||(layer == m_nEndLayer))
		{
			if(flag == 0) flag = 1;
			else flag = 2;
		}
		if(flag > 0)
		{
			if(pDoc->m_sPCB_LayerParam[layer].bLayerShow == TRUE) return TRUE;
		}
		if(flag == 2) break;

		if(layer == BOTTOMLAYER) break;
		layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
	}

	return FALSE;
}

void COb_pcbvia::Draw(CDC* pDC, CView* pV, int mode)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	if(IsVisible() == FALSE) return;

	BOOL bVia = FALSE;
	
	if(mode == 0)
	{
		if(pView->m_nPCB_Current_Layer == BOTTOMSOLDER)
		{
			if(DrawTopSolder(pDC, pV) == TRUE) bVia = TRUE;
			if(DrawBottomSolder(pDC, pV) == TRUE) bVia = TRUE;
		}
		else
		{
			if(DrawBottomSolder(pDC, pV) == TRUE) bVia = TRUE;
			if(DrawTopSolder(pDC, pV) == TRUE) bVia = TRUE;
		}
		if(DrawInternalPlane(pDC, pV) == TRUE) bVia = TRUE;
	}
	if(DrawVia(pDC, pV) == TRUE) bVia = TRUE;
	if(DrawViaHoles(pDC, pV) == TRUE) bVia = TRUE;
	if(mode == 0)
	{
		DrawPlaneMark(pDC, pV);
		if(bVia == TRUE) DrawNetName(pDC, pV);
	}
}

void COb_pcbvia::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	int cx,cy;

	cx = (int)((m_fCx - origin_x)/scale);
	cy = nEy - (int)((m_fCy - origin_y)/scale);	

	int r = (int)(m_fDia/2/scale);

	COLORREF color = nEDA_PCB_LayerColor[MULTILAYER];

	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,1,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
	 
	CBrush fillbrush;
	fillbrush.CreateSolidBrush(color);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	RECT rect;
	rect.left = cx - r;
	rect.right = cx + r;
	rect.top = cy - r;
	rect.bottom = cy + r;

	pDC->Ellipse(&rect);
			
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);


	r = (int)(m_fHoleDia/2/scale);
	color = nEDA_PCB_LayerColor[VIAHOLES];

	CPen holepen;
	holepen.CreatePen(PS_SOLID,1,color);
	oldpen =  pDC->SelectObject(&holepen);
		 
	CBrush holebrush;
	holebrush.CreateSolidBrush(color);
	oldbrush = pDC->SelectObject(&holebrush);
			
	rect.left = cx - r;
	rect.right = cx + r;
	rect.top = cy - r;
	rect.bottom = cy + r;

	pDC->Ellipse(&rect);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void COb_pcbvia::DrawSelection(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pDocument;	

	if(IsVisible() == FALSE) return;
	
	int cx,cy;

	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
	int r = (int)(m_fDia/2/pView->m_fScale);

	COLORREF color = nEDA_PCB_LayerColor[SELECTIONS];

	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,1,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
	 
		
	// Use the bit pattern to create a bitmap.
	WORD HatchBits[8] = { 0x81, 0x42, 0x24, 0x18, 0x18,  0x24, 0x42, 0x81 };
	CBitmap bm;
	bm.CreateBitmap(8,8,1,1, HatchBits);

	// Create a pattern brush from the bitmap.
	CBrush fillbrush;
	fillbrush.CreatePatternBrush(&bm);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	RECT rect;
	rect.left = cx - r;
	rect.right = cx + r;
	rect.top = cy - r;
	rect.bottom = cy + r;

	int mode = pDC->SetROP2(R2_MERGEPEN );
	pDC->Ellipse(&rect);
	
	pDC->SetROP2(mode);
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);

}

BOOL COb_pcbvia::PickMe(CView* pV)
{
	int wx1,wy1,wx2,wy2;
	int minx,miny,maxx,maxy;

	CESDView* pView = (CESDView*)pV;
    
	if(pView->m_nPrint_Flag == 1)
	{
		wx1 = (int)fEDA_Print_Minx;		wy1 = (int)fEDA_Print_Miny;
		wx2 = (int)fEDA_Print_Maxx;		wy2 = (int)fEDA_Print_Maxy;
	}
	else
	{
		wx1 = (int)pView->m_fScreenX; 
		wy1 = (int)pView->m_fScreenY;
		wx2 = (int)(pView->m_fScreenX + pView->m_fScale*pView->m_nViewport_Ex); 
		wy2 = (int)(pView->m_fScreenY + pView->m_fScale*pView->m_nViewport_Ey);
	 
		wx1 -= 100;
		wy1 -= 100;
		wx2 += 100;
		wy2 += 100;
	}

	minx = (int)(m_fCx - m_fDia/2);
	miny = (int)(m_fCy - m_fDia/2);
	maxx = (int)(m_fCx + m_fDia/2);
	maxy = (int)(m_fCy + m_fDia/2);

   	if( ((minx < wx1)&&(maxx < wx1)) || ((minx > wx2)&&(maxx > wx2)) || ((miny < wy1)&&(maxy < wy1)) || ((miny > wy2)&&(maxy > wy2)) )	return FALSE;
	else return TRUE;
}

BOOL  COb_pcbvia::Is_On_Me(double fx, double fy)
{
    CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(IsVisible() == FALSE) return FALSE;

	double radius = m_fDia/2;

	CRgn rgn;
	rgn.CreateEllipticRgn((int)(m_fCx - radius), (int)(m_fCy - radius), (int)(m_fCx + radius), (int)(m_fCy + radius) );

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

BOOL  COb_pcbvia::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fr;
	fr = m_fDia/2;

	double fminx, fminy, fmaxx, fmaxy;
	fminx = m_fCx - fr;
	fminy = m_fCy - fr;
	fmaxx = m_fCx + fr;
	fmaxy = m_fCy + fr;
	
	if((fminx < fx1)||(fmaxx > fx2)||(fminy < fy1)||(fmaxy > fy2)) return FALSE;
	return TRUE;
}

void COb_pcbvia::Change(int index)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_PCBEditVia dlg;
	
	int layer = 0;
	for(int i=0; i<=MULTILAYER; i++)
	{
		dlg.arrayLayer.Add(pDoc->m_sPCB_LayerParam[layer].csLayerName);
		if(layer == BOTTOMLAYER) break;
		layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
	}

	for(int i=0; i<dlg.arrayLayer.GetCount(); i++)
	{
		if(pDoc->m_sPCB_LayerParam[m_nStartLayer].csLayerName == dlg.arrayLayer.GetAt(i)) 
		{
			dlg.m_nStartLayer = i;
			break;
		}
	}
	for(int i=0; i<dlg.arrayLayer.GetCount(); i++)
	{
		if(pDoc->m_sPCB_LayerParam[m_nEndLayer].csLayerName == dlg.arrayLayer.GetAt(i)) 
		{
			dlg.m_nEndLayer = i;
			break;
		}
	}
	
	dlg.arrayStringNet.Add("无网络");
	for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = (COb_pcbnet*)pDoc->m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;
	
		dlg.arrayStringNet.Add(pnet->m_csNetName);
	}
	dlg.m_csComboNet = dlg.arrayStringNet.GetAt(m_nNet + 1);

	dlg.m_csEditX = pDoc->Convert_XY_To_String(m_fCx  - pDoc->m_cPCB_Sheet.m_fOriginX);
	dlg.m_csEditY = pDoc->Convert_XY_To_String(m_fCy  - pDoc->m_cPCB_Sheet.m_fOriginY);
	dlg.m_csEditDia = pDoc->Convert_XY_To_String(m_fDia);
	dlg.m_csEditHoleDia = pDoc->Convert_XY_To_String(m_fHoleDia);

	dlg.m_bCheckLocked = m_bLocked;
	dlg.m_bCheckTentingBottom = m_bTentingBottom;
	dlg.m_bCheckTentingTop = m_bTentingTop;
	dlg.m_nRadioExpansion = m_nCSEV - 1;
	dlg.m_csEditExpansion = pDoc->Convert_XY_To_String(m_fCSE);
	
	if(dlg.DoModal() == IDOK)
	{
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = PCB_ELEMENT_VIA;
		pundo->m_nIndex = index;
		pundo->pcbvia.Copy(this);
			
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		
		if(dlg.m_nStartLayer > dlg.m_nEndLayer)
		{
			int i = dlg.m_nStartLayer;
			dlg.m_nStartLayer = dlg.m_nEndLayer;
			dlg.m_nEndLayer = i;
		}
		for(int i=0; i<=MULTILAYER; i++)
		{
			if(dlg.arrayLayer.GetAt(dlg.m_nStartLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
			{
				m_nStartLayer = i;
				break;
			}
		}
		for(int i=0; i<=MULTILAYER; i++)
		{
			if(dlg.arrayLayer.GetAt(dlg.m_nEndLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
			{
				m_nEndLayer = i;
				break;
			}
		}

		for(int i=0; i<dlg.arrayStringNet.GetCount(); i++)
		{
			if(dlg.arrayStringNet.GetAt(i) == dlg.m_csComboNet)
			{
				m_nNet = i-1;
			}
		}

		m_fCx = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditX) + pDoc->m_cPCB_Sheet.m_fOriginX;
		m_fCy = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditY) + pDoc->m_cPCB_Sheet.m_fOriginY;
		m_fDia = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditDia);
		m_fHoleDia = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditHoleDia);

		m_bLocked = dlg.m_bCheckLocked;
		m_bTentingBottom = dlg.m_bCheckTentingBottom;
		m_bTentingTop = dlg.m_bCheckTentingTop;
		m_nCSEV = dlg.m_nRadioExpansion + 1;
		m_fCSE = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditExpansion);
	}
}

void COb_pcbvia::Copy(COb_pcbvia* ppcbvia)
{
	pDocument = ppcbvia->pDocument;
	m_fCx = ppcbvia->m_fCx;
	m_fCy = ppcbvia->m_fCy;
	m_fDia = ppcbvia->m_fDia;
	m_fHoleDia = ppcbvia->m_fHoleDia;
	m_nStartLayer = ppcbvia->m_nStartLayer;
	m_nEndLayer = ppcbvia->m_nEndLayer;

	m_nLayer = ppcbvia->m_nLayer;
	m_nNet = ppcbvia->m_nNet;
	m_nComponent = ppcbvia->m_nComponent;
	m_nPolygon = ppcbvia->m_nPolygon;

	m_bLocked = ppcbvia->m_bLocked;

	m_bTestingTop = ppcbvia->m_bTestingTop;
	m_bTestingBottom = ppcbvia->m_bTestingBottom;
	m_bTenting = ppcbvia->m_bTenting;
	m_bTentingTop = ppcbvia->m_bTentingTop;
	m_bTentingBottom = ppcbvia->m_bTentingBottom;

	m_bPolygonOutline = ppcbvia->m_bPolygonOutline;
	m_bUserRouted = ppcbvia->m_bUserRouted;
	m_nUnionIndex = ppcbvia->m_nUnionIndex;

	m_nCPLV = ppcbvia->m_nCPLV;	
	m_nCPL = ppcbvia->m_nCPL;
	
	m_nCSEV = ppcbvia->m_nCSEV;	
	m_fCSE = ppcbvia->m_fCSE;	
	m_nCPCV = ppcbvia->m_nCPCV;	
	m_fCPC = ppcbvia->m_fCPC;	
	m_nCCSV = ppcbvia->m_nCCSV;	
	m_nCCS = ppcbvia->m_nCCS;	
	m_nCENV = ppcbvia->m_nCENV;	
	m_nCEN = ppcbvia->m_nCEN;	
	m_nCCWV = ppcbvia->m_nCCWV;	
	m_fCCW = ppcbvia->m_fCCW;	
	m_nCAGV = ppcbvia->m_nCAGV;	
	m_fCAG = ppcbvia->m_fCAG;	
	m_nCPRV = ppcbvia->m_nCPRV;	
	m_fCPR = ppcbvia->m_fCPR;	

	m_bSelection = FALSE; 
}

BOOL COb_pcbvia::ConnectToInternalPlane(int planelayer)
{
    CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = (COb_pcbpolygon*)pDoc->m_arrayPcbPolygon.GetAt(i);
		if((ppolygon->m_nFlag == FLAG_NORMAL)&&(ppolygon->m_nPolygonType == SPLITPLANE)&&(ppolygon->m_nLayer == planelayer))
		{
			if((ppolygon->Is_On_Me(m_fCx, m_fCy) == TRUE)&&(ppolygon->m_nNet == m_nNet))
			{
				int j;
				for(j=0; j<pDoc->m_arrayPcbPolygon.GetCount(); j++)
				{
					COb_pcbpolygon* ppolygon1 = (COb_pcbpolygon*)pDoc->m_arrayPcbPolygon.GetAt(j);
					if((ppolygon1->m_nPolygonType == SPLITPLANE)&&(ppolygon1->m_nLayer == ppolygon->m_nLayer))
					{
						if((ppolygon->Is_On_Me(ppolygon1->m_cVertex[0].fx,ppolygon1->m_cVertex[0].fy) == TRUE)&&(ppolygon1->Is_On_Me(m_fCx, m_fCy) == TRUE)&&(ppolygon1->m_nNet != ppolygon->m_nNet)) 
						{
							//ppolygon的内部包含ppolygon1, 且ppolygon和ppolygon1不属于相同网络, 
							break;

						}
					}
				}

				if(j >= pDoc->m_arrayPcbPolygon.GetCount()) return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL COb_pcbvia::DrawTopSolder(CDC* pDC, CView* pV)
{

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	if((pDoc->m_sPCB_LayerParam[TOPSOLDER].bLayerShow == FALSE)||(m_bTentingTop == TRUE)) return FALSE;
	if(m_nStartLayer != TOPLAYER) return FALSE;

	double expansion;
	if(m_nCSEV == 1) expansion = pDoc->m_fSolderMaskExpansion;
	else expansion = m_fCSE;

	int cx,cy;
	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
	if(pView->m_nPrint_Flag == 1)
	{
		cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}	

	int r = (int)((m_fDia/2 + expansion)/pView->m_fScale);
	if(pView->m_nPrint_Flag == 1) r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);

	COLORREF color;

	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_PCBPrint_Color == 0) 	color = RGB(0,0,0);
		else if(nEDA_PCBPrint_Color == 2) color = RGB(GetGray( nEDA_PCB_LayerColor[TOPSOLDER]), GetGray( nEDA_PCB_LayerColor[TOPSOLDER]), GetGray( nEDA_PCB_LayerColor[TOPSOLDER]));
		else color = nEDA_PCB_LayerColor[TOPSOLDER];
	}
	else
	{
		if(m_nFlag == FLAG_DELETED) color = nEDA_PCB_BoardAreaColor;
		else color = nEDA_PCB_LayerColor[TOPSOLDER];
	}

	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,1,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
	 
	CBrush fillbrush;
	fillbrush.CreateSolidBrush(color);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	RECT rect;
	rect.left = cx - r;
	rect.right = cx + r;
	rect.top = cy - r;
	rect.bottom = cy + r;

	pDC->Ellipse(&rect);
			
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);

	return TRUE;
}

BOOL COb_pcbvia::DrawBottomSolder(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if((pDoc->m_sPCB_LayerParam[BOTTOMSOLDER].bLayerShow == FALSE)||(m_bTentingBottom == TRUE)) return FALSE;
	if(m_nEndLayer != BOTTOMLAYER) return FALSE;

		double expansion;
	if(m_nCSEV == 1) expansion = pDoc->m_fSolderMaskExpansion;
	else expansion = m_fCSE;

		int cx,cy;
	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
	if(pView->m_nPrint_Flag == 1)
	{
		cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}

		int r = (int)((m_fDia/2 + expansion)/pView->m_fScale);
	if(pView->m_nPrint_Flag == 1) r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);

	COLORREF color;

	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_PCBPrint_Color == 0) 	color = RGB(0,0,0);
		else if(nEDA_PCBPrint_Color == 2) color = RGB(GetGray( nEDA_PCB_LayerColor[BOTTOMSOLDER]), GetGray( nEDA_PCB_LayerColor[BOTTOMSOLDER]), GetGray( nEDA_PCB_LayerColor[BOTTOMSOLDER]));
		else color = nEDA_PCB_LayerColor[BOTTOMSOLDER];
	}
	else
	{
		if(m_nFlag == FLAG_DELETED) color = nEDA_PCB_BoardAreaColor;
		else color = nEDA_PCB_LayerColor[BOTTOMSOLDER];
	}

	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,1,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
	 
	CBrush fillbrush;
	fillbrush.CreateSolidBrush(color);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	RECT rect;
	rect.left = cx - r;
	rect.right = cx + r;
	rect.top = cy - r;
	rect.bottom = cy + r;

	pDC->Ellipse(&rect);
			
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);

	return TRUE;
}

BOOL COb_pcbvia::DrawInternalPlane(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if((pView->m_nPCB_Current_Layer < POWERPLANE1)||(pView->m_nPCB_Current_Layer > POWERPLANE16)) return FALSE;

	
	if(m_fHoleDia <= 0) return FALSE;

	int layer = pView->m_nPCB_Current_Layer;
		
	COLORREF color;
		if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_PCBPrint_Color == 0) 	color = RGB(0,0,0);
		else if(nEDA_PCBPrint_Color == 2) color = RGB(GetGray( nEDA_PCB_LayerColor[layer]), GetGray( nEDA_PCB_LayerColor[layer]), GetGray( nEDA_PCB_LayerColor[layer]));
		else color = nEDA_PCB_LayerColor[layer];
	}
	else
	{
		if(m_nFlag == FLAG_DELETED) color = nEDA_PCB_BoardAreaColor;
		else color = nEDA_PCB_LayerColor[layer];
	}
		
	int cx,cy;
	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
	if(pView->m_nPrint_Flag == 1)
	{
		cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}

	if((m_nCCS != NO_CONNECT)&&(m_nNet >= 0)&&(ConnectToInternalPlane(layer) == TRUE))	
	{
		if(m_nCCS == RELIEF_CONNECT)
		{
			double fr = (m_fHoleDia + m_fCAG)/2 + m_fCPR;
			double alpha = asin((m_fCCW/2 + m_fCAG/2)/fr);
					
			int r = (int)(fr/pView->m_fScale);
			if(pView->m_nPrint_Flag == 1)	r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
			
			int width = (int)(m_fCAG/pView->m_fScale);
			if(pView->m_nPrint_Flag == 1) width = (int)(width*nEDA_Print_Xdpi/1000*fEDA_PCBPrint_Scale);
			if(width < 1) width = 1;

			CPen fillpen;
			fillpen.CreatePen(PS_SOLID, width, color);
			CPen* oldpen =  pDC->SelectObject(&fillpen);						

			if((int)(alpha*180/fEDA_pi) < 45)
			{
				if(m_nCEN == 2)
				{
					int x1, y1, x2, y2;
					x1 = cx + r*sin(alpha);
					y1 = cy + r*cos(alpha);
					x2 = cx + r*sin(alpha);
					y2 = cy - r*cos(alpha);
					pDC->Arc(cx-r, cy-r, cx+r, cy+r, x1, y1, x2, y2);
							
					x1 = cx - r*sin(alpha);
					y1 = cy - r*cos(alpha);
					x2 = cx - r*sin(alpha);
					y2 = cy + r*cos(alpha);
					pDC->Arc(cx-r, cy-r, cx+r, cy+r, x1, y1, x2, y2);
				}
				else if(m_nCEN == 4)
				{
					int x1, y1, x2, y2;
					x1 = cx + r*sin(alpha);
					y1 = cy + r*cos(alpha);
					x2 = cx + r*cos(alpha);
					y2 = cy + r*sin(alpha);
					pDC->Arc(cx-r, cy-r, cx+r, cy+r, x1, y1, x2, y2);

					x1 = cx + r*cos(alpha);
					y1 = cy - r*sin(alpha);
					x2 = cx + r*sin(alpha);
					y2 = cy - r*cos(alpha);
					pDC->Arc(cx-r, cy-r, cx+r, cy+r, x1, y1, x2, y2);
							
					x1 = cx - r*sin(alpha);
					y1 = cy - r*cos(alpha);
					x2 = cx - r*cos(alpha);
					y2 = cy - r*sin(alpha);
					pDC->Arc(cx-r, cy-r, cx+r, cy+r, x1, y1, x2, y2);

					x1 = cx - r*cos(alpha);
					y1 = cy + r*sin(alpha);
					x2 = cx - r*sin(alpha);
					y2 = cy + r*cos(alpha);
					pDC->Arc(cx-r, cy-r, cx+r, cy+r, x1, y1, x2, y2);
				}
			}

			pDC->SelectObject(oldpen);
		}
		else if(m_nCCS == DIRECT_CONNECT)
		{
			
		}
		return FALSE; 
	}
	else
	{
		double clearance = m_fCPC;

		int r = (int)((m_fHoleDia/2 + clearance)/pView->m_fScale);
		if(pView->m_nPrint_Flag == 1) r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);

		CPen fillpen;
		fillpen.CreatePen(PS_SOLID,1,color);
		CPen* oldpen =  pDC->SelectObject(&fillpen);
			 
		CBrush fillbrush;
		fillbrush.CreateSolidBrush(color);
		CBrush* oldbrush = pDC->SelectObject(&fillbrush);

		RECT rect;
		rect.left = cx - r;
		rect.right = cx + r;
		rect.top = cy - r;
		rect.bottom = cy + r;

		pDC->Ellipse(&rect);
			
		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);

		return TRUE;
	}
}

BOOL COb_pcbvia::DrawVia(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if((m_nStartLayer != TOPLAYER)||(m_nEndLayer != BOTTOMLAYER)||(pDoc->m_sPCB_LayerParam[MULTILAYER].bLayerShow != TRUE))
	{
		int layer = TOPLAYER;
		int flag = 0;
		for(int i=0; i<PCB_LAYER_NUM; i++)
		{
			if((layer == m_nStartLayer)||(layer == m_nEndLayer))
			{
				if(flag == 0) flag = 1;
				else flag = 2;
			}
			if(flag > 0)
			{
				if(pDoc->m_sPCB_LayerParam[layer].bLayerShow == TRUE)
				{
					if((layer >= TOPLAYER)&&(layer <= BOTTOMLAYER)) break;
				}
			}
			if((flag == 2)||(layer == BOTTOMLAYER)) return FALSE;
			layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
		}
	}
	
	int cx,cy;
	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
	if(pView->m_nPrint_Flag == 1)
	{
		cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}

	if((m_nStartLayer == TOPLAYER)&&(m_nEndLayer == BOTTOMLAYER))
	{
		int r = (int)(m_fDia/2/pView->m_fScale);
		if(pView->m_nPrint_Flag == 1) r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);

		COLORREF color;

		if(pView->m_nPrint_Flag == 1)
		{
			if(nEDA_PCBPrint_Color == 0) 	color = RGB(0,0,0);
			else if(nEDA_PCBPrint_Color == 2) color = RGB(GetGray( nEDA_PCB_LayerColor[MULTILAYER]), GetGray( nEDA_PCB_LayerColor[MULTILAYER]), GetGray( nEDA_PCB_LayerColor[MULTILAYER]));
			else color = nEDA_PCB_LayerColor[MULTILAYER];
		}
		else
		{
			if(m_nFlag == FLAG_DELETED) color = nEDA_PCB_BoardAreaColor;
			else color = nEDA_PCB_LayerColor[MULTILAYER];
		}

		CPen fillpen;
		fillpen.CreatePen(PS_SOLID,1,color);
		CPen* oldpen =  pDC->SelectObject(&fillpen);
	 
		CBrush fillbrush;
		fillbrush.CreateSolidBrush(color);
		CBrush* oldbrush = pDC->SelectObject(&fillbrush);

		RECT rect;
		rect.left = cx - r;
		rect.right = cx + r;
		rect.top = cy - r;
		rect.bottom = cy + r;

		pDC->Ellipse(&rect);
			
		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);
	}
	else
	{
		int r = (int)(m_fDia/2/pView->m_fScale);
		if(pView->m_nPrint_Flag == 1) r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);

		COLORREF color, color1, color2;

		if(pView->m_nPrint_Flag == 1)
		{
			if(nEDA_PCBPrint_Color == 0)
			{
				color = color1 = color2 = RGB(0,0,0);
			}
			else if(nEDA_PCBPrint_Color == 2)
			{
				color = RGB(GetGray( nEDA_PCB_LayerColor[MULTILAYER]), GetGray( nEDA_PCB_LayerColor[MULTILAYER]), GetGray( nEDA_PCB_LayerColor[MULTILAYER]));
				color1 = RGB(GetGray( nEDA_PCB_LayerColor[m_nStartLayer]), GetGray( nEDA_PCB_LayerColor[m_nStartLayer]), GetGray( nEDA_PCB_LayerColor[m_nStartLayer]));
				color2 = RGB(GetGray( nEDA_PCB_LayerColor[m_nEndLayer]), GetGray( nEDA_PCB_LayerColor[m_nEndLayer]), GetGray( nEDA_PCB_LayerColor[m_nEndLayer]));
			}
			else 
			{
				color = nEDA_PCB_LayerColor[MULTILAYER];
				color1 = nEDA_PCB_LayerColor[m_nStartLayer];
				color2 = nEDA_PCB_LayerColor[m_nEndLayer];
			}
		}
		else
		{
			if(m_nFlag == FLAG_DELETED)
			{
				color = color1 = color2 = nEDA_PCB_BoardAreaColor;
			}
			else
			{
				color = nEDA_PCB_LayerColor[MULTILAYER];
				color1 = nEDA_PCB_LayerColor[m_nStartLayer];
				color2 = nEDA_PCB_LayerColor[m_nEndLayer];
			}
		}

		CPen viapen1;
		viapen1.CreatePen(PS_SOLID,1,color1);
		CPen* oldpen =  pDC->SelectObject(&viapen1);
	 
		CBrush viabrush1;
		viabrush1.CreateSolidBrush(color1);
		CBrush* oldbrush = pDC->SelectObject(&viabrush1);

		RECT rect;
		rect.left = cx - r;
		rect.right = cx + r;
		rect.top = cy - r;
		rect.bottom = cy + r;

		POINT spoint, epoint;
		spoint.x = cx;
		spoint.y = cy - r;
		epoint.x = cx;
		epoint.y = cy + r;

		pDC->Pie(&rect, spoint, epoint);
			
		CPen viapen2;
		viapen2.CreatePen(PS_SOLID,1,color2);
		oldpen =  pDC->SelectObject(&viapen2);
		 
		CBrush viabrush2;
		viabrush2.CreateSolidBrush(color2);
		oldbrush = pDC->SelectObject(&viabrush2);

		spoint.x = cx;
		spoint.y = cy + r;
		epoint.x = cx;
		epoint.y = cy - r;

		pDC->Pie(&rect, spoint, epoint);

		CPen viapen;
		viapen.CreatePen(PS_SOLID,1,color);
		oldpen =  pDC->SelectObject(&viapen);

		spoint.x = cx;
		spoint.y = cy + r;
		epoint.x = cx;
		epoint.y = cy + r;			

		pDC->Arc(&rect, spoint, epoint); 
	
		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);
	}

	return TRUE;
}

BOOL COb_pcbvia::DrawViaHoles(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(((pView->m_nPrint_Flag == 0)||((pView->m_nPrint_Flag == 1)&&(bEDA_PCBPrint_Holes == TRUE)))&&(pDoc->m_sPCB_LayerParam[VIAHOLES].bLayerShow == TRUE))
	{
		int cx,cy;
		pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
		if(pView->m_nPrint_Flag == 1)
		{
			cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
			cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
		}
			
		int r = (int)(m_fHoleDia/2/pView->m_fScale);
		if(pView->m_nPrint_Flag == 1) r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);

		COLORREF color;

		if(pView->m_nPrint_Flag == 1)
		{
			color = RGB(255,255,255);
		}
		else
		{
			if(m_nFlag == FLAG_DELETED) color = nEDA_PCB_BoardAreaColor;
			else color = nEDA_PCB_LayerColor[VIAHOLES];
		}

		CPen holepen;
		holepen.CreatePen(PS_SOLID,1,color);
		CPen* oldpen =  pDC->SelectObject(&holepen);
		 
		CBrush holebrush;
		holebrush.CreateSolidBrush(color);
		CBrush* oldbrush = pDC->SelectObject(&holebrush);
			
		RECT rect;
		rect.left = cx - r;
		rect.right = cx + r;
		rect.top = cy - r;
		rect.bottom = cy + r;

		pDC->Ellipse(&rect);

		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);

		return TRUE;
	}
	else return FALSE;
}

void COb_pcbvia::DrawPlaneMark(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	//plane 显示
	if((pView->m_nPrint_Flag == 0)&&(m_nNet >= 0))
	{
		for(int layer=POWERPLANE1; layer <= POWERPLANE16; layer++)
		{
			if(ConnectToInternalPlane(layer) == TRUE)
			{
				if(m_nCCS == RELIEF_CONNECT)
				{
					COb_pcbtrack track;
					track.m_nFlag = m_nFlag;
					track.m_fWidth = 1;
					track.m_nLayer = layer;
					track.m_fX1 = m_fCx - 40;
					track.m_fY1 = m_fCy;
					track.m_fX2 = m_fCx + 40;
					track.m_fY2 = m_fCy;
					track.Draw(pDC, pV);
					track.m_fX1 = m_fCx;
					track.m_fY1 = m_fCy - 40;
					track.m_fX2 = m_fCx;
					track.m_fY2 = m_fCy + 40;
					track.Draw(pDC, pV);
				}
				else if(m_nCCS == DIRECT_CONNECT)
				{
					COb_pcbtrack track;
					track.m_nFlag = m_nFlag;
					track.m_fWidth = 1;
					track.m_nLayer = layer;
					track.m_fX1 = m_fCx - 40;
					track.m_fY1 = m_fCy - 40;
					track.m_fX2 = m_fCx + 40;
					track.m_fY2 = m_fCy + 40;
					track.Draw(pDC, pV);
					track.m_fX1 = m_fCx - 40;
					track.m_fY1 = m_fCy + 40;
					track.m_fX2 = m_fCx + 40;
					track.m_fY2 = m_fCy - 40;
					track.Draw(pDC, pV);
				}
				else
				{
				}
			}
		}
	}
}

void COb_pcbvia::DrawNetName(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	int cx,cy;
	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
	if(pView->m_nPrint_Flag == 1)
	{
		cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}

	if(pView->m_nPrint_Flag == 0)
	{
		int height;
		if(pView->m_fScale <= 2)
		{
			if(pView->m_fScale > 1.2) height = 10;
			else if(pView->m_fScale > 0.6) height = 14;
			else height = 20;

			COLORREF color;
			if(m_nFlag == FLAG_DELETED) color = nEDA_PCB_BoardAreaColor;	
			else color = RGB(255,255,255); 
			
			COLORREF oldtextcolor;
			oldtextcolor = pDC->SetTextColor(color);

			int bkmode = pDC->SetBkMode(TRANSPARENT);

			LOGFONT logfont;
			logfont.lfHeight = height; 
			logfont.lfWidth = 0;
			logfont.lfOrientation = 0;	
			logfont.lfEscapement = 0;
			logfont.lfWeight = FW_NORMAL;
			logfont.lfItalic = 0;
			logfont.lfUnderline = 0;
			logfont.lfStrikeOut = 0;
			logfont.lfCharSet = DEFAULT_CHARSET;
			logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
			logfont.lfQuality = PROOF_QUALITY;
			logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
			strcpy(logfont.lfFaceName,"Arial");

			CFont font, *oldfont;
			font.CreateFontIndirect(&logfont);	
			oldfont = pDC->SelectObject(&font);
			
			int x,y;			
			CString text;
			if(m_nNet >= 0)
			{
				COb_pcbnet* pcbnet = (COb_pcbnet*)pDoc->m_arrayPcbNet.GetAt(m_nNet);
				text = pcbnet->m_csNetName;
			}
			else text.Empty();
			CSize size = pDC->GetTextExtent(text);
			x = cx - size.cx/2;
			y = cy - size.cy/2;
			pDC->TextOut(x, y, text, strlen(text));

			pDC->SelectObject(oldfont);
			
			pDC->SetTextColor(oldtextcolor);
			pDC->SetBkMode(bkmode);
		}
	}
}

void COb_pcbvia::Move(CClientDC* pDC, CView* pV,int index)
{
	int ch1;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动过孔: <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV, 0);

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_pcbvia avia;
	avia.Copy(this);
	avia.m_nFlag = FLAG_NORMAL;

	avia.Draw(pDC, pV, 1);

	pView->m_fCurrentSnapX = avia.m_fCx;
	pView->m_fCurrentSnapY = avia.m_fCy;
	pView->Restore_LargeCrossXY();
	pView->OpenMouseCursor();
	pView->Write_Help(szHelp);

	bEDA_NeedElectricalGrid = TRUE;

	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->PcbElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			avia.Draw(pDC, pV, 1);
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
				pView->PcbElecHotPointsControl(pDC);

				pView->CloseMouseCursor();
				avia.Draw(pDC, pV, 1);
				pView->OpenMouseCursor();
			}

			avia.m_fCx = pView->m_fCurrentSnapX;
			avia.m_fCy = pView->m_fCurrentSnapY;

			pView->CloseMouseCursor();
			avia.Draw(pDC, pV, 1);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->PcbElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			avia.Draw(pDC, pV, 1);
			pView->OpenMouseCursor();

			avia.m_fCx = pView->m_fCurrentSnapX;
			avia.m_fCy = pView->m_fCurrentSnapY;

			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_VIA;
			pundo->m_nIndex = index;
			pundo->pcbvia.Copy(this);
			pundo->pcbvia.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fCx = avia.m_fCx;
			m_fCy = avia.m_fCy;

			break;
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	
	pView->CloseMouseCursor();

	m_nFlag = FLAG_NORMAL;
	Draw(pDC, pV, 0);
	pView->OpenMouseCursor();

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;


	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	bEDA_NeedElectricalGrid = FALSE;
	pView->m_fpPcbHotPoint1.fx = pView->m_fpPcbHotPoint1.fy = pView->m_fpPcbHotPoint2.fx = pView->m_fpPcbHotPoint2.fy = 0;

	pView->Write_Help(szEDA_Help_Ready);
}

BOOL COb_pcbvia::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;

	char szHelp[] = "复制过孔: <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	pView->Invalidate();

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_pcbvia avia;
	avia.Copy(this);
	avia.m_nFlag = FLAG_NORMAL;

	avia.Draw(pDC, pV, 1);

	pView->m_fCurrentSnapX = avia.m_fCx;
	pView->m_fCurrentSnapY = avia.m_fCy;
	pView->Restore_LargeCrossXY();
	pView->OpenMouseCursor();
	pView->Write_Help(szHelp);

	bEDA_NeedElectricalGrid = TRUE;

	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->PcbElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			avia.Draw(pDC, pV, 1);
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
				pView->PcbElecHotPointsControl(pDC);

				pView->CloseMouseCursor();
				avia.Draw(pDC, pV, 1);
				pView->OpenMouseCursor();
			}

			avia.m_fCx = pView->m_fCurrentSnapX;
			avia.m_fCy = pView->m_fCurrentSnapY;

			pView->CloseMouseCursor();
			avia.Draw(pDC, pV, 1);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->PcbElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			avia.Draw(pDC, pV, 1);
			pView->OpenMouseCursor();

			avia.m_fCx = pView->m_fCurrentSnapX;
			avia.m_fCy = pView->m_fCurrentSnapY;

			m_fCx = avia.m_fCx;
			m_fCy = avia.m_fCy;

			break;
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	
	pView->CloseMouseCursor();
	m_nFlag = FLAG_NORMAL;
	Draw(pDC, pV, 0);
	pView->OpenMouseCursor();

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	pView->Write_Help(szEDA_Help_Ready);
	
	bEDA_NeedElectricalGrid = FALSE;
	pView->m_fpPcbHotPoint1.fx = pView->m_fpPcbHotPoint1.fy = pView->m_fpPcbHotPoint2.fx = pView->m_fpPcbHotPoint2.fy = 0;

	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;
}

void COb_pcbvia::DrawHighlighted(CDC* pDC, CView* pV, int layer)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	if(IsVisible() == FALSE) return;
	
	int cx,cy;
	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);

	int r = (int)(m_fDia/2/pView->m_fScale);

	COLORREF color;
	if(layer == CONNECTIONS) color = nEDA_PCB_LayerColor[SELECTIONS];
	else if(layer == DRCERROR) color = nEDA_PCB_LayerColor[DRCERROR];

	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,1,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
	 
	CBrush fillbrush;
	fillbrush.CreateSolidBrush(color);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	RECT rect;
	rect.left = cx - r;
	rect.right = cx + r;
	rect.top = cy - r;
	rect.bottom = cy + r;

	pDC->Ellipse(&rect);
		
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);

	r = (int)(m_fHoleDia/2/pView->m_fScale);

	color = nEDA_PCB_LayerColor[VIAHOLES];

	CPen holepen;
	holepen.CreatePen(PS_SOLID,1,color);
	oldpen =  pDC->SelectObject(&holepen);
		 
	CBrush holebrush;
	holebrush.CreateSolidBrush(color);
	oldbrush = pDC->SelectObject(&holebrush);
			
	rect.left = cx - r;
	rect.right = cx + r;
	rect.top = cy - r;
	rect.bottom = cy + r;

	pDC->Ellipse(&rect);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);

	int height;
	if(pView->m_fScale <= 2)
	{
		if(pView->m_fScale > 1.2) height = 10;
		else if(pView->m_fScale > 0.6) height = 14;
		else height = 20;

		color = nEDA_PCB_BoardAreaColor;	
			
		COLORREF oldtextcolor;
		oldtextcolor = pDC->SetTextColor(color);

		int bkmode = pDC->SetBkMode(TRANSPARENT);

		LOGFONT logfont;
		logfont.lfHeight = height; 
		logfont.lfWidth = 0;
		logfont.lfOrientation = 0;	
		logfont.lfEscapement = 0;
		logfont.lfWeight = FW_NORMAL;
		logfont.lfItalic = 0;
		logfont.lfUnderline = 0;
		logfont.lfStrikeOut = 0;
		logfont.lfCharSet = DEFAULT_CHARSET;
		logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logfont.lfQuality = PROOF_QUALITY;
		logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
		strcpy(logfont.lfFaceName,"Arial");

		CFont font, *oldfont;
		font.CreateFontIndirect(&logfont);	
		oldfont = pDC->SelectObject(&font);
		
		int x,y;			
		CString text;
		if(m_nNet >= 0)
		{
			COb_pcbnet* pcbnet = (COb_pcbnet*)pDoc->m_arrayPcbNet.GetAt(m_nNet);
			text = pcbnet->m_csNetName;
		}
		else text.Empty();
		CSize size = pDC->GetTextExtent(text);
		x = cx - size.cx/2;
		y = cy - size.cy/2;
		pDC->TextOut(x, y, text, strlen(text));

		pDC->SelectObject(oldfont);
			
		pDC->SetTextColor(oldtextcolor);
		pDC->SetBkMode(bkmode);
	}
}

BOOL COb_pcbvia::TearDropExist(CView* pV)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	CESDView* pView = (CESDView*)pV;

	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_bTearDrop == FALSE)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;
		
		if((pDoc->IsConnectedToVia(ptrack->m_nLayer, this) == TRUE)&&(pView->Line_Via_Connect(ptrack, this, 0) == TRUE)) return TRUE;
	}
	
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_bTearDrop == FALSE)||(parc->m_nComponent >= 0)||(parc->m_nPolygon >= 0)) continue;

		if((pDoc->IsConnectedToVia(parc->m_nLayer, this) == TRUE)&&(pView->Via_Arc_Connect(this, parc, 0) == TRUE)) return TRUE;

	}
	
	return FALSE;
}

void COb_pcbvia::GetViaVertexOutline( CArray<Struct_VertexOutline, Struct_VertexOutline&>& arrayVertexOutline, double drc)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	double fr;
	fr = m_fDia/2 + drc + 1;
	
	if(m_fDia == 0) return;

	Struct_VertexOutline vertexoutline;

	vertexoutline.kind = 1;
	vertexoutline.cx = m_fCx;
	vertexoutline.cy = m_fCy;
	vertexoutline.r = fr;
	vertexoutline.sangle = 0;
	vertexoutline.eangle = 360;
	vertexoutline.x1 = vertexoutline.cx + vertexoutline.r;
	vertexoutline.y1 = vertexoutline.cy;
	vertexoutline.x2 = vertexoutline.cx + vertexoutline.r;
	vertexoutline.y2 = vertexoutline.cy;

	arrayVertexOutline.Add(vertexoutline);
}