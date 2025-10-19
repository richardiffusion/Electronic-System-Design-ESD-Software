#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_PcbEditPad.h"

IMPLEMENT_SERIAL(COb_pcbpad, CObject, 0)
COb_pcbpad::COb_pcbpad()
{
	m_nPadMode = PAD_MODE_SIMPLE;
	for(int i=0; i<SIGNAL_LAYER_NUM; i++)
	{
		m_fSizeX[i] = m_fSizeY[i] = 50;
		m_nShape[i] = PAD_SHAPE_ROUND;
	}
	m_nHoleType = PADHOLE_ROUND;
	m_fHoleDia = 30;
	m_fHoleRotation = 0;
	m_fHoleWidth = 0;

	m_csName.Empty();

	m_nLayer = MULTILAYER;
	m_nNet = -1;
	m_nComponent = -1;
	
	m_bLocked = FALSE;
	m_bPlated = TRUE;
	m_nDaisychain = 0;
    m_nJumperID = 0;

	m_nCPLV = 0;
	m_nCPL = 1;

	m_nCPEV = 1;	
	m_fCPE = 0;		

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

	m_bPolygonOutline = FALSE;
	m_bUserRouted = TRUE;
	m_nUnionIndex = -1;

	m_bSelection = FALSE;
	m_nFlag = 0;
}

void COb_pcbpad::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fCx;
		ar << m_fCy;
		ar << m_fRotation;	
		
		ar << m_nPadMode;
		for(int i=0; i<SIGNAL_LAYER_NUM; i++)
		{
			ar << m_fSizeX[i];
			ar << m_fSizeY[i];
			ar << m_nShape[i];
		}
		
		ar << m_nHoleType;
		ar << m_fHoleDia;
		ar << m_fHoleRotation;
		ar << m_fHoleWidth;
		
		ar << m_csName;
		ar << m_nLayer;
		ar << m_nNet;
		ar << m_nComponent;

		ar << m_bLocked;
		ar << m_bPlated;
		ar << m_nDaisychain;
		ar << m_nJumperID;
		
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
		ar << m_nCPEV;
		ar << m_fCPE;
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
		ar >> m_fRotation;	
		
		ar >> m_nPadMode;
		for(int i=0; i<SIGNAL_LAYER_NUM; i++)
		{
			ar >> m_fSizeX[i];
			ar >> m_fSizeY[i];
			ar >> m_nShape[i];
		}
		
		ar >> m_nHoleType;
		ar >> m_fHoleDia;
		ar >> m_fHoleRotation;
		ar >> m_fHoleWidth;
		
		ar >> m_csName;
		ar >> m_nLayer;
		ar >> m_nNet;
		ar >> m_nComponent;

		ar >> m_bLocked;
		ar >> m_bPlated;
		ar >> m_nDaisychain;
		ar >> m_nJumperID;
		
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
		ar >> m_nCPEV;
		ar >> m_fCPE;
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

void COb_pcbpad::Draw(CDC* pDC, CView* pV, int mode)
{
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(m_nLayer != MULTILAYER)
	{
		BOOL bPad = FALSE;
		if(mode == DRAW_NORMAL)
		{
			if(pView->m_nPCB_Current_Layer == BOTTOMSOLDER)
			{
				if(DrawBottomPaste(pDC, pV) == TRUE) bPad = TRUE;
				if(DrawTopPaste(pDC, pV) == TRUE) bPad = TRUE;
				if(DrawTopSolder(pDC, pV) == TRUE) bPad = TRUE;
				if(DrawBottomSolder(pDC, pV) == TRUE) bPad = TRUE;
			}
			else if(pView->m_nPCB_Current_Layer == TOPSOLDER)
			{
				if(DrawBottomPaste(pDC, pV) == TRUE) bPad = TRUE;
				if(DrawTopPaste(pDC, pV) == TRUE) bPad = TRUE;
				if(DrawBottomSolder(pDC, pV) == TRUE) bPad = TRUE;
				if(DrawTopSolder(pDC, pV) == TRUE) bPad = TRUE;
			}
			else if(pView->m_nPCB_Current_Layer == BOTTOMPASTE)
			{
				if(DrawBottomSolder(pDC, pV) == TRUE) bPad = TRUE;
				if(DrawTopSolder(pDC, pV) == TRUE) bPad = TRUE;
				if(DrawTopPaste(pDC, pV) == TRUE) bPad = TRUE;
				if(DrawBottomPaste(pDC, pV) == TRUE) bPad = TRUE;
			}
			else
			{
				if(DrawBottomSolder(pDC, pV) == TRUE) bPad = TRUE;
				if(DrawTopSolder(pDC, pV) == TRUE) bPad = TRUE;
				if(DrawBottomPaste(pDC, pV) == TRUE) bPad = TRUE;
				if(DrawTopPaste(pDC, pV) == TRUE) bPad = TRUE;
			}
		}
		
		if(DrawPad(pDC, pV) == TRUE) bPad = TRUE;

		if((bPad == TRUE)&&(mode == DRAW_NORMAL)) DrawNetName(pDC, pV);
	}
	else 
	{
		BOOL bPad = FALSE;
		if(mode == DRAW_NORMAL)
		{
			if(pView->m_nPCB_Current_Layer == BOTTOMSOLDER)
			{
				if(DrawTopSolder(pDC, pV) == TRUE) bPad = TRUE;
				if(DrawBottomSolder(pDC, pV) == TRUE) bPad = TRUE;
			}
			else
			{
				if(DrawBottomSolder(pDC, pV) == TRUE) bPad = TRUE;
				if(DrawTopSolder(pDC, pV) == TRUE) bPad = TRUE;
			}		
			if(DrawInternalPlane(pDC, pV) == TRUE) bPad = TRUE;		
		}
		if(DrawPad(pDC, pV) == TRUE) bPad = TRUE;
		if(DrawPadHoles(pDC, pV) == TRUE) bPad = TRUE;

		if(mode == DRAW_NORMAL)
		{
			DrawPlaneMark(pDC, pV);
			if(bPad == TRUE) DrawNetName(pDC, pV);	
		}
	}
}


void COb_pcbpad::DrawSingleLayerPadSpecial(CDC* pDC, CView* pV, int layer)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(layer == TOPSOLDER)
	{
		if(DrawTopSolder(pDC, pV) == TRUE) DrawNetName(pDC, pV);	
	}
	else if(layer == BOTTOMSOLDER)
	{
		if(DrawBottomSolder(pDC, pV) == TRUE) DrawNetName(pDC, pV);	
	}
	else if(layer == TOPPASTE)
	{
		if(DrawTopPaste(pDC, pV) == TRUE) DrawNetName(pDC, pV);	
	}
	else if(layer == BOTTOMPASTE)
	{
		if(DrawBottomPaste(pDC, pV) == TRUE) DrawNetName(pDC, pV);	
	}
}


BOOL COb_pcbpad::PickMe(CView* pV)
{
	int wx1,wy1,wx2,wy2;

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

	double maxf,maxfx,maxfy;
	maxfx = maxfy = 0;

	for(int i=0; i<32; i++)
	{
		maxfx = MAX(maxfx, m_fSizeX[i]);
		maxfy = MAX(maxfy, m_fSizeY[i]);
	}
	maxf = MAX(maxfx, maxfy);

	int minx, miny, maxx, maxy;
	
	minx = (int)(m_fCx - maxf/2);
	miny = (int)(m_fCy - maxf/2);
	maxx = (int)(m_fCx + maxf/2);
	maxy = (int)(m_fCy + maxf/2);

   	if( ((minx < wx1)&&(maxx < wx1)) || ((minx > wx2)&&(maxx > wx2)) || ((miny < wy1)&&(maxy < wy1)) || ((miny > wy2)&&(maxy > wy2)) )	return FALSE;
	else return TRUE;
}

void COb_pcbpad::DrawPadRound(CDC* pDC, CView* pV, int layer, int color_layer, double expansion)
{
	COLORREF color;
	CESDView* pView = (CESDView*)pV;

	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_PCBPrint_Color == 0) 	color = RGB(0,0,0);
		else if(nEDA_PCBPrint_Color == 2) color = RGB(GetGray( nEDA_PCB_LayerColor[color_layer]), GetGray( nEDA_PCB_LayerColor[color_layer]), GetGray( nEDA_PCB_LayerColor[color_layer]));
		else color = nEDA_PCB_LayerColor[color_layer];
	}
	else
	{
		if(m_nFlag == FLAG_DELETED) color = nEDA_PCB_BoardAreaColor;
		else color = nEDA_PCB_LayerColor[color_layer];
	}
	
	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,1,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
 
	CBrush fillbrush;
	fillbrush.CreateSolidBrush(color);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	if(m_fSizeX[layer] == m_fSizeY[layer])
	{
		double fcx, fcy, fr;
		int cx,cy,r;
		fcx = m_fCx;
		fcy = m_fCy;
		fr = m_fSizeX[layer]/2 + expansion;
			
		pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
		r = (int)(fr/pView->m_fScale);
		if(pView->m_nPrint_Flag == 1)
		{
			cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
			cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
			r = (int)(((double)r*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
		}	

		pDC->Ellipse(cx-r, cy-r, cx+r, cy+r);

	}
	else if(m_fSizeX[layer] > m_fSizeY[layer])
	{
		double fcx, fcy, fr;
		int cx, cy, r;
		
		fr = m_fSizeY[layer] + expansion*2;
		r = (int)(fr/pView->m_fScale);
		if(pView->m_nPrint_Flag == 1)
		{
			r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		}		

		CPen pen;
		pen.CreatePen(PS_SOLID, r, color);
		CPen* oldpen =  pDC->SelectObject(&pen);

		fcx = m_fCx - cos(m_fRotation*fEDA_pi/180)*(m_fSizeX[layer] - m_fSizeY[layer])/2;
		fcy = m_fCy - sin(m_fRotation*fEDA_pi/180)*(m_fSizeX[layer] - m_fSizeY[layer])/2;
					
		pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
		if(pView->m_nPrint_Flag == 1)
		{
			cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
			cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
		}	
		pDC->MoveTo(cx, cy);

		fcx = m_fCx + cos(m_fRotation*fEDA_pi/180)*(m_fSizeX[layer] - m_fSizeY[layer])/2;
		fcy = m_fCy + sin(m_fRotation*fEDA_pi/180)*(m_fSizeX[layer] - m_fSizeY[layer])/2;
					
		pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
		if(pView->m_nPrint_Flag == 1)
		{
			cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
			cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
		}	
		pDC->LineTo(cx, cy);

		pDC->SelectObject(oldpen);
	}
	else
	{
		double fcx, fcy, fr;
		int cx, cy, r;

		fr = m_fSizeX[layer] + expansion*2;
		r = (int)(fr/pView->m_fScale);
		if(pView->m_nPrint_Flag == 1)
		{
			r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		}	

		CPen pen;
		pen.CreatePen(PS_SOLID, r, color);
		CPen* oldpen =  pDC->SelectObject(&pen);

		fcx = m_fCx - sin(m_fRotation*fEDA_pi/180)*(m_fSizeY[layer] - m_fSizeX[layer])/2;
		fcy = m_fCy + cos(m_fRotation*fEDA_pi/180)*(m_fSizeY[layer] - m_fSizeX[layer])/2;

		pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
		if(pView->m_nPrint_Flag == 1)
		{
			cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
			cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
		}	
		pDC->MoveTo(cx, cy);

		fcx = m_fCx + sin(m_fRotation*fEDA_pi/180)*(m_fSizeY[layer] - m_fSizeX[layer])/2;
		fcy = m_fCy - cos(m_fRotation*fEDA_pi/180)*(m_fSizeY[layer] - m_fSizeX[layer])/2;
					
		pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
		if(pView->m_nPrint_Flag == 1)
		{
			cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
			cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
		}	
		pDC->LineTo(cx, cy);

		pDC->SelectObject(oldpen);
	}
	
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void COb_pcbpad::DrawPadRectangle(CDC* pDC, CView* pV, int layer, int color_layer, double expansion)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();	

	COb_pcbfill pcbfill;
	pcbfill.m_nLayer = color_layer;
	pcbfill.m_fX1 = m_fCx - m_fSizeX[layer]/2 - expansion;
	pcbfill.m_fY1 = m_fCy - m_fSizeY[layer]/2 - expansion;
	pcbfill.m_fX2 = m_fCx + m_fSizeX[layer]/2 + expansion;
	pcbfill.m_fY2 = m_fCy + m_fSizeY[layer]/2 + expansion;
	pcbfill.m_fRotation = m_fRotation;
	pcbfill.m_nFlag = m_nFlag;

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
	pcbfill.GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	

	COLORREF color;
	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_PCBPrint_Color == 0) color = RGB(0,0,0);
		else if(nEDA_PCBPrint_Color == 2) color = RGB(GetGray( nEDA_PCB_LayerColor[color_layer]), GetGray( nEDA_PCB_LayerColor[color_layer]), GetGray( nEDA_PCB_LayerColor[color_layer]));
		else color = nEDA_PCB_LayerColor[color_layer]; 
	}
	else
	{
		if(m_nFlag == FLAG_DELETED) color = nEDA_PCB_BoardAreaColor;
		else color = nEDA_PCB_LayerColor[color_layer];
	}

	POINT point[5];
	int x,y;
	pView->Get_VPort_XY(&x,&y,fx1,fy1);
	if(pView->m_nPrint_Flag == 1)
	{
		x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}
	point[0].x = x; 
	point[0].y = y;
	point[4].x = x; 
	point[4].y = y;
	
	pView->Get_VPort_XY(&x,&y,fx3,fy3);
	if(pView->m_nPrint_Flag == 1)
	{
		x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}
	point[1].x = x;
	point[1].y = y;
	
	pView->Get_VPort_XY(&x,&y,fx2,fy2);
	if(pView->m_nPrint_Flag == 1)
	{
		x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}
	point[2].x = x;
	point[2].y = y;
	
	pView->Get_VPort_XY(&x,&y,fx4,fy4);
	if(pView->m_nPrint_Flag == 1)
	{
		x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}
	point[3].x = x;
	point[3].y = y;

	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,1,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
 
	CBrush fillbrush;
	fillbrush.CreateSolidBrush(color);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);		
	
	pDC->Polygon(point, 5);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);

}

void COb_pcbpad::DrawPadOctagonal(CDC* pDC, CView* pV, int layer, int color_layer, double expansion)
{
	CESDView* pView = (CESDView*)pV;

	COLORREF color;
	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_PCBPrint_Color == 0) color = RGB(0,0,0);
		else if(nEDA_PCBPrint_Color == 2) color = RGB(GetGray( nEDA_PCB_LayerColor[color_layer]), GetGray( nEDA_PCB_LayerColor[color_layer]), GetGray( nEDA_PCB_LayerColor[color_layer]));
		else color = nEDA_PCB_LayerColor[color_layer]; //彩色打印
	}
	else
	{
		if(m_nFlag == FLAG_DELETED) color = nEDA_PCB_BoardAreaColor;
		else color = nEDA_PCB_LayerColor[color_layer];
	}

	Fpoint fpoint[9];
	if( m_fSizeX[layer] >= m_fSizeY[layer])
	{
		fpoint[0].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/2 + (m_fSizeY[layer]+2*expansion)/4;
		fpoint[0].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/2;				
		fpoint[1].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/2;
		fpoint[1].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/4;
		fpoint[2].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/2;
		fpoint[2].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/4;
		fpoint[3].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/2 + (m_fSizeY[layer]+2*expansion)/4;
		fpoint[3].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/2;
		fpoint[4].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/2 - (m_fSizeY[layer]+2*expansion)/4;
		fpoint[4].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/2;
		fpoint[5].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/2;
		fpoint[5].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/4;
		fpoint[6].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/2;
		fpoint[6].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/4;
		fpoint[7].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/2 - (m_fSizeY[layer]+2*expansion)/4;
		fpoint[7].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/2;

		for(int i=0; i<8; i++)
		{
			fpoint[i].fx -= m_fCx;
			fpoint[i].fy -= m_fCy;
		}

		double alpha, fr;
		for(int i=0; i<8; i++)
		{
			if((m_fRotation == 0)||(m_fRotation == 180)||(m_fRotation == 360))
			{
				fpoint[i].fx = m_fCx + fpoint[i].fx;
				fpoint[i].fy = m_fCy + fpoint[i].fy;
			}
			else if((m_fRotation == 90)||(m_fRotation == 270))
			{
				double temp;
				temp = fpoint[i].fx;
				fpoint[i].fx = m_fCx + fpoint[i].fy;
				fpoint[i].fy = m_fCy + temp;
			}
			else
			{
				alpha = atan2(fpoint[i].fy, fpoint[i].fx);
				fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
				fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
				fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
			}
		}
	}
	else
	{
		fpoint[0].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/4;
		fpoint[0].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/2;
		fpoint[1].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/2;
		fpoint[1].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/2 + (m_fSizeX[layer]+2*expansion)/4;
		fpoint[2].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/2;
		fpoint[2].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/2 - (m_fSizeX[layer]+2*expansion)/4;
		fpoint[3].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/4;
		fpoint[3].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/2;
		fpoint[4].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/4;
		fpoint[4].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/2;
		fpoint[5].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/2;
		fpoint[5].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/2 - (m_fSizeX[layer]+2*expansion)/4;
		fpoint[6].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/2;
		fpoint[6].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/2 + (m_fSizeX[layer]+2*expansion)/4;
		fpoint[7].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/4;
		fpoint[7].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/2;

		for(int i=0; i<8; i++)
		{
			fpoint[i].fx -= m_fCx;
			fpoint[i].fy -= m_fCy;
		}

		double alpha, fr;
		for(int i=0; i<8; i++)
		{
			if((m_fRotation == 0)||(m_fRotation == 180)||(m_fRotation == 360))
			{
				fpoint[i].fx = m_fCx + fpoint[i].fx;
				fpoint[i].fy = m_fCy + fpoint[i].fy;
			}
			else if((m_fRotation == 90)||(m_fRotation == 270))
			{
				double temp;
				temp = fpoint[i].fx;
				fpoint[i].fx = m_fCx + fpoint[i].fy;
				fpoint[i].fy = m_fCy + temp;
			}
			else
			{
				alpha = atan2(fpoint[i].fy, fpoint[i].fx);
				fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
				fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
				fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
			}
		}
	}

	POINT point[9];
	for(int i=0; i<8; i++)
	{
		int x,y;
		pView->Get_VPort_XY(&x,&y,fpoint[i].fx, fpoint[i].fy);
		if(pView->m_nPrint_Flag == 1)
		{
			x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
			y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
		}
		point[i].x = x;
		point[i].y = y;
	}
	point[8].x = point[0].x;
	point[8].y = point[0].y;

	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,1,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
 
	CBrush fillbrush;
	fillbrush.CreateSolidBrush(color);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);		
	
	pDC->Polygon(point, 9);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void COb_pcbpad::DrawPadRoundedRectangle(CDC* pDC, CView* pV, int layer, int color_layer, double expansion)
{
	DrawPadOctagonal(pDC, pV, layer, color_layer, expansion);
}

BOOL  COb_pcbpad::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double frx, fry;
	frx = m_fSizeX[0]/2;
	fry = m_fSizeY[0]/2;

	double fminx, fminy, fmaxx, fmaxy;
	fminx = m_fCx - frx;
	fminy = m_fCy - fry;
	fmaxx = m_fCx + frx;
	fmaxy = m_fCy + fry;
	
	if((fminx < fx1)||(fmaxx > fx2)||(fminy < fy1)||(fmaxy > fy2)) return FALSE;
	return TRUE;
}

BOOL COb_pcbpad::Is_On_Me(double fx, double fy)
{
	if(IsVisible() == FALSE) return FALSE;

	if((m_nLayer != MULTILAYER)||(m_nPadMode == PAD_MODE_SIMPLE))
	{
		if(m_nShape[TOPLAYER] == PAD_SHAPE_RECTANGLE) return Is_On_Rect(fx,fy, TOPLAYER);
		else if(m_nShape[TOPLAYER] == PAD_SHAPE_OCTAGONAL) return Is_On_Oct(fx,fy,TOPLAYER);
		else return Is_On_Round(fx,fy,TOPLAYER);
	}
	else 
	{
		if(m_nShape[TOPLAYER] == PAD_SHAPE_RECTANGLE)
		{
			if(Is_On_Rect(fx,fy, TOPLAYER) == TRUE) return TRUE;
		}
		else if(m_nShape[TOPLAYER] == PAD_SHAPE_OCTAGONAL)
		{
			if(Is_On_Oct(fx,fy,TOPLAYER) == TRUE) return TRUE;
		}
		else
		{
			if(Is_On_Round(fx,fy,TOPLAYER) == TRUE) return TRUE;
		}

		if(m_nShape[BOTTOMLAYER] == PAD_SHAPE_RECTANGLE)
		{
			if(Is_On_Rect(fx,fy, BOTTOMLAYER) == TRUE) return TRUE;
		}
		else if(m_nShape[BOTTOMLAYER] == PAD_SHAPE_OCTAGONAL)
		{
			if(Is_On_Oct(fx,fy,BOTTOMLAYER) == TRUE) return TRUE;
		}
		else
		{
			if(Is_On_Round(fx,fy,BOTTOMLAYER) == TRUE) return TRUE;
		}

		if(m_nShape[MIDLAYER1] == PAD_SHAPE_RECTANGLE)
		{
			if(Is_On_Rect(fx,fy, MIDLAYER1) == TRUE) return TRUE;
		}
		else if(m_nShape[MIDLAYER1] == PAD_SHAPE_OCTAGONAL)
		{
			if(Is_On_Oct(fx,fy,MIDLAYER1) == TRUE) return TRUE;
		}
		else
		{
			if(Is_On_Round(fx,fy,MIDLAYER1) == TRUE) return TRUE;
		}

		return FALSE;
	}
}

BOOL COb_pcbpad::Is_On_Round(double fx, double fy, int layer)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(m_fSizeX[layer] == m_fSizeY[layer])
	{
		double cx, cy, r;
		cx = m_fCx;
		cy = m_fCy;
		r = m_fSizeX[layer]/2;

		return pDoc->InDistance_Point_Point(fx, fy, cx, cy, r);
	}
	else if(m_fSizeX[layer] > m_fSizeY[layer])
	{
		double fx1, fy1, fx2, fy2, r;

		fx1 = m_fCx - cos(m_fRotation*fEDA_pi/180)*(m_fSizeX[layer] - m_fSizeY[layer])/2;
		fy1 = m_fCy - sin(m_fRotation*fEDA_pi/180)*(m_fSizeX[layer] - m_fSizeY[layer])/2;

		fx2 = m_fCx + cos(m_fRotation*fEDA_pi/180)*(m_fSizeX[layer] - m_fSizeY[layer])/2;
		fy2 = m_fCy + sin(m_fRotation*fEDA_pi/180)*(m_fSizeX[layer] - m_fSizeY[layer])/2;

		r = m_fSizeY[layer]/2;

		return pDoc->InDistance_Point_LINE(fx, fy, fx1, fy1, fx2, fy2, r);
	}
	else
	{
		double fx1, fy1, fx2, fy2, r;

		fx1 = m_fCx - sin(m_fRotation*fEDA_pi/180)*(m_fSizeY[layer] - m_fSizeX[layer])/2;
		fy1 = m_fCy + cos(m_fRotation*fEDA_pi/180)*(m_fSizeY[layer] - m_fSizeX[layer])/2;

		fx2 = m_fCx + sin(m_fRotation*fEDA_pi/180)*(m_fSizeY[layer] - m_fSizeX[layer])/2;
		fy2 = m_fCy - cos(m_fRotation*fEDA_pi/180)*(m_fSizeY[layer] - m_fSizeX[layer])/2;

		r = m_fSizeX[layer]/2;
		
		return pDoc->InDistance_Point_LINE(fx, fy, fx1, fy1, fx2, fy2, r);
	}

	return FALSE;
}

BOOL COb_pcbpad::Is_On_Rect(double fx, double fy, int layer)
{
	int vertex = 4;

	CPoint point;
	CPoint* lpPoint  = (CPoint*)malloc(sizeof(point)*vertex);

	double fx1,fy1,fx2,fy2;
	fx1 = m_fCx - m_fSizeX[layer]/2;	fy1 = m_fCy - m_fSizeY[layer]/2;
	fx2 = m_fCx + m_fSizeX[layer]/2;	fy2 = m_fCy + m_fSizeY[layer]/2;
	

	double fxm, fym;
	fxm = (fx1 + fx2)/2;
	fym = (fy1 + fy2)/2;


	fx1 -= fxm;	fy1 -= fym;
	fx2 -= fxm;	fy2 -= fym;


	double  fxx1,fyy1,fxx2,fyy2,fxx3,fyy3,fxx4,fyy4;
	double alpha, r;
	alpha = atan(fy2/fx2);
	r = sqrt(fx2*fx2 + fy2*fy2);

	fxx2 = r*cos(alpha + m_fRotation*fEDA_pi/180);
	fyy2 = r*sin(alpha + m_fRotation*fEDA_pi/180);
	fxx1 = -fxx2;
	fyy1 = -fyy2;
	fxx4 = (fx2-fx1)*cos(m_fRotation*fEDA_pi/180) + fxx1;
	fyy4 = (fx2-fx1)*sin(m_fRotation*fEDA_pi/180) + fyy1;
	fxx3 = -fxx4;
	fyy3 = -fyy4;

	fxx1 += fxm; fxx2 += fxm; fxx3 += fxm; fxx4 += fxm;
	fyy1 += fym; fyy2 += fym; fyy3 += fym; fyy4 += fym;


	lpPoint[0].x = fxx1;
	lpPoint[0].y = fyy1;
	lpPoint[1].x = fxx4;
	lpPoint[1].y = fyy4;
	lpPoint[2].x = fxx2;
	lpPoint[2].y = fyy2;
	lpPoint[3].x = fxx3;
	lpPoint[3].y = fyy3;
		
	CRgn   rgn;
	rgn.CreatePolygonRgn( lpPoint, vertex, ALTERNATE);

	free(lpPoint);

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

BOOL COb_pcbpad::Is_On_Oct(double fx, double fy, int layer)
{
	Fpoint fpoint[9];
	if( m_fSizeX[layer] >= m_fSizeY[layer])
	{
		fpoint[0].fx = m_fCx - m_fSizeX[layer]/2 + m_fSizeY[layer]/4;
		fpoint[0].fy = m_fCy - m_fSizeY[layer]/2;				
		fpoint[1].fx = m_fCx - m_fSizeX[layer]/2;
		fpoint[1].fy = m_fCy - m_fSizeY[layer]/4;
		fpoint[2].fx = m_fCx - m_fSizeX[layer]/2;
		fpoint[2].fy = m_fCy + m_fSizeY[layer]/4;
		fpoint[3].fx = m_fCx - m_fSizeX[layer]/2 + m_fSizeY[layer]/4;
		fpoint[3].fy = m_fCy + m_fSizeY[layer]/2;
		fpoint[4].fx = m_fCx + m_fSizeX[layer]/2 - m_fSizeY[layer]/4;
		fpoint[4].fy = m_fCy + m_fSizeY[layer]/2;
		fpoint[5].fx = m_fCx + m_fSizeX[layer]/2;
		fpoint[5].fy = m_fCy + m_fSizeY[layer]/4;
		fpoint[6].fx = m_fCx + m_fSizeX[layer]/2;
		fpoint[6].fy = m_fCy - m_fSizeY[layer]/4;
		fpoint[7].fx = m_fCx + m_fSizeX[layer]/2 - m_fSizeY[layer]/4;
		fpoint[7].fy = m_fCy - m_fSizeY[layer]/2;

		for(int i=0; i<8; i++)
		{
			fpoint[i].fx -= m_fCx;
			fpoint[i].fy -= m_fCy;
		}

		double alpha, fr;
		for(int i=4; i<8; i++)
		{
			alpha = atan(fpoint[i].fy/fpoint[i].fx);
			fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
			fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
			fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
		}
		for(int i=0; i<4; i++)
		{
			alpha = atan(fpoint[i].fy/fpoint[i].fx) + fEDA_pi;
			fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
			fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
			fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
		}
	}
	else
	{
		fpoint[0].fx = m_fCx - m_fSizeX[layer]/4;
		fpoint[0].fy = m_fCy - m_fSizeY[layer]/2;
		fpoint[1].fx = m_fCx - m_fSizeX[layer]/2;
		fpoint[1].fy = m_fCy - m_fSizeY[layer]/2 + m_fSizeX[layer]/4;
		fpoint[2].fx = m_fCx - m_fSizeX[layer]/2;
		fpoint[2].fy = m_fCy + m_fSizeY[layer]/2 - m_fSizeX[layer]/4;
		fpoint[3].fx = m_fCx - m_fSizeX[layer]/4;
		fpoint[3].fy = m_fCy + m_fSizeY[layer]/2;
		fpoint[4].fx = m_fCx + m_fSizeX[layer]/4;
		fpoint[4].fy = m_fCy + m_fSizeY[layer]/2;
		fpoint[5].fx = m_fCx + m_fSizeX[layer]/2;
		fpoint[5].fy = m_fCy + m_fSizeY[layer]/2 - m_fSizeX[layer]/4;
		fpoint[6].fx = m_fCx + m_fSizeX[layer]/2;
		fpoint[6].fy = m_fCy - m_fSizeY[layer]/2 + m_fSizeX[layer]/4;
		fpoint[7].fx = m_fCx + m_fSizeX[layer]/4;
		fpoint[7].fy = m_fCy - m_fSizeY[layer]/2;

		for(int i=0; i<8; i++)
		{
			fpoint[i].fx -= m_fCx;
			fpoint[i].fy -= m_fCy;
		}

		double alpha, fr;
		for(int i=4; i<8; i++)
		{
			alpha = atan(fpoint[i].fy/fpoint[i].fx);
			fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
			fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
			fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
		}
		for(int i=0; i<4; i++)
		{
			alpha = atan(fpoint[i].fy/fpoint[i].fx) + fEDA_pi;
			fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
			fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
			fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
		}

	}

	POINT point[9];
	for(int i=0; i<8; i++)
	{
		point[i].x = (int)fpoint[i].fx;
		point[i].y = (int)fpoint[i].fy;
	}
	point[8].x = point[0].x;
	point[8].y = point[0].y;

	CRgn   rgn;
	rgn.CreatePolygonRgn( point, 9, ALTERNATE);

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

void  COb_pcbpad::GetRectCorner(double *fx11, double *fy11, double *fx22, double *fy22, double *fx33, double *fy33, double *fx44, double *fy44, int layer)
{
	double fx1,fy1,fx2,fy2;
	fx1 = m_fCx - m_fSizeX[layer]/2;	fy1 = m_fCy - m_fSizeY[layer]/2;
	fx2 = m_fCx + m_fSizeX[layer]/2;	fy2 = m_fCy + m_fSizeY[layer]/2;
	
	double fxm, fym;
	fxm = (fx1 + fx2)/2;
	fym = (fy1 + fy2)/2;

	fx1 -= fxm;	fy1 -= fym;
	fx2 -= fxm;	fy2 -= fym;

	double  fxx1,fyy1,fxx2,fyy2,fxx3,fyy3,fxx4,fyy4;
	double alpha, r;
	alpha = atan(fy2/fx2);
	r = sqrt(fx2*fx2 + fy2*fy2);

	fxx2 = r*cos(alpha + m_fRotation*fEDA_pi/180);
	fyy2 = r*sin(alpha + m_fRotation*fEDA_pi/180);
	fxx1 = -fxx2;
	fyy1 = -fyy2;
	fxx4 = (fx2-fx1)*cos(m_fRotation*fEDA_pi/180) + fxx1;
	fyy4 = (fx2-fx1)*sin(m_fRotation*fEDA_pi/180) + fyy1;
	fxx3 = -fxx4;
	fyy3 = -fyy4;

	//坐标平移
	fxx1 += fxm; fxx2 += fxm; fxx3 += fxm; fxx4 += fxm;
	fyy1 += fym; fyy2 += fym; fyy3 += fym; fyy4 += fym;

	*fx11 = fxx1; *fx22 = fxx2; *fx33 = fxx3; *fx44= fxx4;
	*fy11 = fyy1; *fy22 = fyy2; *fy33 = fyy3; *fy44= fyy4;
}

void COb_pcbpad::Change(int index)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_PCBEditPad dlg;
	dlg.pDoc = pDoc;
	
	for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
	{
		dlg.arrayStringLayer.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
	}
	for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
	{
		if(m_nLayer == pDoc->m_arrayPcbLayerShow.GetAt(i))
		{
			dlg.m_nComboLayer = i;
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

	dlg.m_csEditPadX = pDoc->Convert_XY_To_String(m_fCx  - pDoc->m_cPCB_Sheet.m_fOriginX);
	dlg.m_csEditPadY = pDoc->Convert_XY_To_String(m_fCy  - pDoc->m_cPCB_Sheet.m_fOriginY);
	dlg.m_fEditRotation = m_fRotation;

	dlg.m_csEditHoleSize = pDoc->Convert_XY_To_String(m_fHoleDia);
	dlg.m_nRadioHoleType = m_nHoleType;
	dlg.m_fHoleRotation = m_fHoleRotation;
	dlg.m_csEditHoleWidth = pDoc->Convert_XY_To_String(m_fHoleWidth);

	dlg.m_csEditDesignator = m_csName;
	dlg.m_nComboType = m_nDaisychain;
	dlg.m_bCheckLocked = m_bLocked;
	dlg.m_bCheckPlated = m_bPlated;

	dlg.m_nRadioPadMode = m_nPadMode;
	dlg.m_csEditXSizeTop = pDoc->Convert_XY_To_String(m_fSizeX[TOPLAYER]);
	dlg.m_csEditYSizeTop = pDoc->Convert_XY_To_String(m_fSizeY[TOPLAYER]);
	dlg.m_nComboShapeTop = m_nShape[TOPLAYER];
	dlg.m_csEditXSizeMid = pDoc->Convert_XY_To_String(m_fSizeX[MIDLAYER1]);
	dlg.m_csEditYSizeMid = pDoc->Convert_XY_To_String(m_fSizeY[MIDLAYER1]);
	dlg.m_nComboShapeMid = m_nShape[MIDLAYER1];
	dlg.m_csEditXSizeBottom = pDoc->Convert_XY_To_String(m_fSizeX[BOTTOMLAYER]);
	dlg.m_csEditYSizeBottom = pDoc->Convert_XY_To_String(m_fSizeY[BOTTOMLAYER]);
	dlg.m_nComboShapeBottom = m_nShape[BOTTOMLAYER];

	dlg.m_bCheckTentingTop = m_bTentingTop;
	dlg.m_bCheckTentingBottom = m_bTentingBottom;


	dlg.m_nRadioPaste = m_nCPEV - 1;
	dlg.m_csEditPaste = pDoc->Convert_XY_To_String(m_fCPE);

	dlg.m_nRadioSolder = m_nCSEV - 1;
	dlg.m_csEditSolder = pDoc->Convert_XY_To_String(m_fCSE);
	
	
	if(dlg.DoModal() == IDOK)
	{
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = PCB_ELEMENT_PAD;
		pundo->m_nIndex = index;
		pundo->pcbpad.Copy(this);
			
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		

		for(int i=0; i<=MULTILAYER; i++)
		{
			if(dlg.arrayStringLayer.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
			{
				m_nLayer = i;
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

		if(m_nLayer == MULTILAYER)
		{
			m_fHoleDia = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditHoleSize);
			m_nHoleType = dlg.m_nRadioHoleType;
			m_fHoleRotation = dlg.m_fHoleRotation;
			m_fHoleWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditHoleWidth);

			if(m_fHoleDia <= 0) m_fHoleDia = 20;
		}
		else
		{
			m_fHoleDia = 0;
			m_nHoleType = PADHOLE_ROUND;
			m_fHoleRotation = 0;
			m_fHoleWidth = 0;
		}

		m_fCx = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditPadX) + pDoc->m_cPCB_Sheet.m_fOriginX;
		m_fCy = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditPadY) + pDoc->m_cPCB_Sheet.m_fOriginY;
		m_fRotation = dlg.m_fEditRotation;

		m_csName = dlg.m_csEditDesignator;
		m_nDaisychain = dlg.m_nComboType;
		m_bLocked = dlg.m_bCheckLocked;
		m_bPlated = dlg.m_bCheckPlated;

		m_nPadMode = dlg.m_nRadioPadMode;
		if(m_nPadMode == PAD_MODE_SIMPLE)
		{
			for(int i=0; i<32; i++)
			{
				m_fSizeX[i] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditXSizeTop);
				m_fSizeY[i] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditYSizeTop);
				m_nShape[i] = dlg.m_nComboShapeTop;
			}
		}
		else
		{
			m_fSizeX[TOPLAYER] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditXSizeTop);
			m_fSizeY[TOPLAYER] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditYSizeTop);
			m_nShape[TOPLAYER] = dlg.m_nComboShapeTop;

			m_fSizeX[BOTTOMLAYER] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditXSizeBottom);
			m_fSizeY[BOTTOMLAYER] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditYSizeBottom);
			m_nShape[BOTTOMLAYER] = dlg.m_nComboShapeBottom;

			for(int i=MIDLAYER1; i<=MIDLAYER30; i++)
			{
				m_fSizeX[i] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditXSizeMid);
				m_fSizeY[i] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditYSizeMid);
				m_nShape[i] = dlg.m_nComboShapeMid;
			}
		}

		m_bTentingTop = dlg.m_bCheckTentingTop;
		m_bTentingBottom = dlg.m_bCheckTentingBottom;

		m_nCPEV = dlg.m_nRadioPaste + 1;
		m_fCPE = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditPaste);
		m_nCSEV = dlg.m_nRadioSolder + 1;
		m_fCSE = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditSolder);
	}
}

void COb_pcbpad::Copy(COb_pcbpad* ppad)
{
	pDocument = ppad->pDocument;

	m_fCx = ppad->m_fCx;
	m_fCy = ppad->m_fCy;
	m_fRotation = ppad->m_fRotation;
	
	m_nPadMode = ppad->m_nPadMode;
	for(int i=0; i<32; i++)
	{
		m_fSizeX[i] = ppad->m_fSizeX[i];
		m_fSizeY[i] = ppad->m_fSizeY[i];
		m_nShape[i] = ppad->m_nShape[i];
	}

	m_nHoleType = ppad->m_nHoleType;
	m_fHoleDia = ppad->m_fHoleDia;
	m_fHoleRotation = ppad->m_fHoleRotation;
	m_fHoleWidth = ppad->m_fHoleWidth;

	m_csName = ppad->m_csName;
	m_nLayer = ppad->m_nLayer;
	m_nNet = ppad->m_nNet;
	m_nComponent = ppad->m_nComponent;

	m_bLocked = ppad->m_bLocked;
	m_bPlated = ppad->m_bPlated;
	m_nDaisychain = ppad->m_nDaisychain;
	m_nJumperID = ppad->m_nJumperID;
	
	m_bTestingTop = ppad->m_bTestingTop;
	m_bTestingBottom = ppad->m_bTestingBottom;
	m_bTenting = ppad->m_bTenting;
	m_bTentingTop = ppad->m_bTentingTop;
	m_bTentingBottom = ppad->m_bTentingBottom;

	m_bPolygonOutline = ppad->m_bPolygonOutline;
	m_bUserRouted = ppad->m_bUserRouted;
	m_nUnionIndex = ppad->m_nUnionIndex;

	m_nCPLV = ppad->m_nCPLV;
	m_nCPL = ppad->m_nCPL;
	
	m_nCPEV = ppad->m_nCPEV;	
	m_fCPE = ppad->m_fCPE;	

	m_nCSEV = ppad->m_nCSEV;	
	m_fCSE = ppad->m_fCSE;	

	m_nCPCV = ppad->m_nCPCV;
	m_fCPC = ppad->m_fCPC;	

	m_nCCSV = ppad->m_nCCSV;
	m_nCCS = ppad->m_nCCS;	
	m_nCENV = ppad->m_nCENV;
	m_nCEN = ppad->m_nCEN;	
	m_nCCWV = ppad->m_nCCWV;
	m_fCCW = ppad->m_fCCW;	
	m_nCAGV = ppad->m_nCAGV;
	m_fCAG = ppad->m_fCAG;	
	m_nCPRV = ppad->m_nCPRV;
	m_fCPR = ppad->m_fCPR;	

	m_bSelection = FALSE;
}

BOOL COb_pcbpad::IsVisible()
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	BOOL bPadShow = FALSE;
	if(m_nLayer == MULTILAYER)
	{
		if(pDoc->m_sPCB_LayerParam[MULTILAYER].bLayerShow == TRUE) bPadShow = TRUE;
		else
		{
			int layer = 0;
			for(int i=0; i<PCB_LAYER_NUM; i++)
			{
				if(pDoc->m_sPCB_LayerParam[layer].bLayerShow == TRUE) 
				{
					bPadShow = TRUE;
					break;
				}
				if(layer == BOTTOMLAYER) break;
				layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
			}
		}
	}
	else if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == TRUE) bPadShow = TRUE;

	return bPadShow;
}

BOOL COb_pcbpad::ConnectToInternalPlane(int planelayer)
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
							break;
						}
					}
				}

				if(j >= pDoc->m_arrayPcbPolygon.GetCount())	return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL COb_pcbpad::DrawTopPaste(CDC* pDC, CView* pV)
{	
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	if((pDoc->m_sPCB_LayerParam[TOPPASTE].bLayerShow != TRUE)||(m_nLayer != TOPLAYER)) return FALSE;
	
	double expansion;
	if(m_nCPEV == 1) expansion = pDoc->m_fPasteMaskExpansion;
	else expansion = m_fCPE;

	if(m_nShape[TOPLAYER] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, TOPLAYER, TOPPASTE, expansion);
	else if(m_nShape[TOPLAYER] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, TOPLAYER, TOPPASTE, expansion);
	else if(m_nShape[TOPLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE)	DrawPadRoundedRectangle(pDC, pV, TOPLAYER, TOPPASTE, expansion);
	else DrawPadRound(pDC, pV, TOPLAYER, TOPPASTE, expansion);

	return TRUE;
}

BOOL COb_pcbpad::DrawBottomPaste(CDC* pDC, CView* pV)
{	
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if((pDoc->m_sPCB_LayerParam[BOTTOMPASTE].bLayerShow != TRUE)||(m_nLayer != BOTTOMLAYER)) return FALSE;

	double expansion;
	if(m_nCPEV == 1) expansion = pDoc->m_fPasteMaskExpansion;
	else expansion = m_fCPE;

	if(m_nShape[TOPLAYER] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, TOPLAYER, BOTTOMPASTE, expansion);
	else if(m_nShape[TOPLAYER] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, TOPLAYER, BOTTOMPASTE, expansion);
	else if(m_nShape[TOPLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE)	DrawPadRoundedRectangle(pDC, pV, TOPLAYER, BOTTOMPASTE, expansion);
	else DrawPadRound(pDC, pV, TOPLAYER, BOTTOMPASTE, expansion);

	return TRUE;
}

BOOL COb_pcbpad::DrawTopSolder(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	if((pDoc->m_sPCB_LayerParam[TOPSOLDER].bLayerShow != TRUE)||(m_bTentingTop == TRUE)) return FALSE;	
	if((m_nLayer != MULTILAYER)&&(m_nLayer != TOPLAYER)) return FALSE;

	double expansion;
	if(m_nCSEV == 1) expansion = pDoc->m_fSolderMaskExpansion;
	else expansion = m_fCSE;

	if(m_nShape[TOPLAYER] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, TOPLAYER, TOPSOLDER, expansion);
	else if(m_nShape[TOPLAYER] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, TOPLAYER, TOPSOLDER, expansion);
	else if(m_nShape[TOPLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE)	DrawPadRoundedRectangle(pDC, pV, TOPLAYER, TOPSOLDER, expansion);
	else DrawPadRound(pDC, pV, TOPLAYER, TOPSOLDER, expansion);

	return TRUE;
}

BOOL COb_pcbpad::DrawBottomSolder(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if((pDoc->m_sPCB_LayerParam[BOTTOMSOLDER].bLayerShow != TRUE)||(m_bTentingBottom == TRUE)) return FALSE;
	if((m_nLayer != MULTILAYER)&&(m_nLayer != BOTTOMLAYER)) return FALSE;

	double expansion;
	if(m_nCSEV == 1) expansion = pDoc->m_fSolderMaskExpansion;
	else expansion = m_fCSE;
		
	if((m_nLayer != MULTILAYER)||(m_nPadMode == PAD_MODE_SIMPLE))
	{
		if(m_nShape[TOPLAYER] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, TOPLAYER, BOTTOMSOLDER, expansion);
		else if(m_nShape[TOPLAYER] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, TOPLAYER, BOTTOMSOLDER, expansion);
		else if(m_nShape[TOPLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE)	DrawPadRoundedRectangle(pDC, pV, TOPLAYER, BOTTOMSOLDER, expansion);
		else DrawPadRound(pDC, pV, TOPLAYER, BOTTOMSOLDER, expansion);
	}
	else if(m_nPadMode == PAD_MODE_T_M_B)
	{
		if(m_nShape[BOTTOMLAYER] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, BOTTOMLAYER, BOTTOMSOLDER, expansion);
		else if(m_nShape[BOTTOMLAYER] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, BOTTOMLAYER, BOTTOMSOLDER, expansion);
		else if(m_nShape[BOTTOMLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE)	DrawPadRoundedRectangle(pDC, pV, BOTTOMLAYER, BOTTOMSOLDER, expansion);
		else DrawPadRound(pDC, pV, BOTTOMLAYER, BOTTOMSOLDER, expansion);
	}

	return TRUE;
}

BOOL COb_pcbpad::DrawInternalPlane(CDC* pDC, CView* pV)
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
		else if(m_nCCS == DIRECT_CONNECT)	//Direct Connect
		{
		}
	}
	else
	{
		double clearance = m_fCPC;
		double expansion = clearance - (m_fSizeX[TOPLAYER] - m_fHoleDia)/2;

		DrawPadRound(pDC, pV, TOPLAYER, layer, expansion);
		return TRUE;
	}

	return FALSE;
}

BOOL COb_pcbpad::DrawPad(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE)
	{
		if(m_nLayer == MULTILAYER)
		{
			int i;
			for(i=0; i<SIGNAL_LAYER_NUM; i++)
			{
				if(pDoc->m_sPCB_LayerParam[i].bLayerShow == TRUE)  break;
			}
			if(i >= SIGNAL_LAYER_NUM) return FALSE;
		}
		else return FALSE;
	}

	if((m_nLayer != MULTILAYER)||(m_nPadMode == PAD_MODE_SIMPLE))
	{
		if(m_nShape[TOPLAYER] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, TOPLAYER, m_nLayer, 0);
		else if(m_nShape[TOPLAYER] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, TOPLAYER, m_nLayer, 0);
		else if(m_nShape[TOPLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE)	DrawPadRoundedRectangle(pDC, pV, TOPLAYER, m_nLayer, 0);
		else DrawPadRound(pDC, pV, TOPLAYER, m_nLayer, 0);
	}
	else if(m_nPadMode == PAD_MODE_T_M_B)
	{
		if(pView->m_nPCB_Current_Layer == BOTTOMLAYER)
		{
			if(pDoc->LayerVisible(TOPLAYER) == TRUE)
			{
				
				if(m_nShape[TOPLAYER] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, TOPLAYER, TOPLAYER, 0);
				else if(m_nShape[TOPLAYER] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, TOPLAYER, TOPLAYER, 0);
				else if(m_nShape[TOPLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE) DrawPadRoundedRectangle(pDC, pV, TOPLAYER, TOPLAYER, 0);
				else DrawPadRound(pDC, pV, TOPLAYER, TOPLAYER, 0);
			}
			
			if((pView->m_nPCB_Current_Layer >= MIDLAYER1)&&(pView->m_nPCB_Current_Layer <= MIDLAYER30))
			{
				if(m_nShape[MIDLAYER1] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, MIDLAYER1, pView->m_nPCB_Current_Layer, 0);
				else if(m_nShape[MIDLAYER1] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, MIDLAYER1, pView->m_nPCB_Current_Layer, 0);
				else if(m_nShape[MIDLAYER1] == PAD_SHAPE_ROUNDEDRECTANGLE) DrawPadRoundedRectangle(pDC, pV, MIDLAYER1, pView->m_nPCB_Current_Layer, 0);
				else DrawPadRound(pDC, pV, MIDLAYER1, pView->m_nPCB_Current_Layer, 0);
			}
			else
			{
				int midlayer;
				for(midlayer=MIDLAYER1; midlayer<=MIDLAYER30; midlayer++) 
				{
					if(pDoc->LayerVisible(midlayer) == TRUE) break;
				}
				if(midlayer<=MIDLAYER30)
				{
					if(m_nShape[MIDLAYER1] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, MIDLAYER1, midlayer, 0);
					else if(m_nShape[MIDLAYER1] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, MIDLAYER1, midlayer, 0);
					else if(m_nShape[MIDLAYER1] == PAD_SHAPE_ROUNDEDRECTANGLE) DrawPadRoundedRectangle(pDC, pV, MIDLAYER1, midlayer, 0);
					else DrawPadRound(pDC, pV, MIDLAYER1, midlayer, 0);
				}
			}
			if(pDoc->LayerVisible(BOTTOMLAYER) == TRUE)
			{
				
				if(m_nShape[BOTTOMLAYER] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, BOTTOMLAYER, BOTTOMLAYER, 0);
				else if(m_nShape[BOTTOMLAYER] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, BOTTOMLAYER, BOTTOMLAYER, 0);
				else if(m_nShape[BOTTOMLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE) DrawPadRoundedRectangle(pDC, pV, BOTTOMLAYER, BOTTOMLAYER, 0);
				else DrawPadRound(pDC, pV, BOTTOMLAYER, BOTTOMLAYER, 0);
			}
		}
		else
		{
			if(pDoc->LayerVisible(BOTTOMLAYER) == TRUE)
			{
				
				if(m_nShape[BOTTOMLAYER] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, BOTTOMLAYER, BOTTOMLAYER, 0);
				else if(m_nShape[BOTTOMLAYER] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, BOTTOMLAYER, BOTTOMLAYER, 0);
				else if(m_nShape[BOTTOMLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE) DrawPadRoundedRectangle(pDC, pV, BOTTOMLAYER, BOTTOMLAYER, 0);
				else DrawPadRound(pDC, pV, BOTTOMLAYER, BOTTOMLAYER, 0);
			}
			
			if((pView->m_nPCB_Current_Layer >= MIDLAYER1)&&(pView->m_nPCB_Current_Layer <= MIDLAYER30))
			{
				if(m_nShape[MIDLAYER1] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, MIDLAYER1, pView->m_nPCB_Current_Layer, 0);
				else if(m_nShape[MIDLAYER1] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, MIDLAYER1, pView->m_nPCB_Current_Layer, 0);
				else if(m_nShape[MIDLAYER1] == PAD_SHAPE_ROUNDEDRECTANGLE) DrawPadRoundedRectangle(pDC, pV, MIDLAYER1, pView->m_nPCB_Current_Layer, 0);
				else DrawPadRound(pDC, pV, MIDLAYER1, pView->m_nPCB_Current_Layer, 0);
			}
			else
			{
				int midlayer;
				for(midlayer=MIDLAYER1; midlayer<=MIDLAYER30; midlayer++) 
				{
					if(pDoc->LayerVisible(midlayer) == TRUE) break;
				}
				if(midlayer<=MIDLAYER30)
				{
					if(m_nShape[MIDLAYER1] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, MIDLAYER1, midlayer, 0);
					else if(m_nShape[MIDLAYER1] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, MIDLAYER1, midlayer, 0);
					else if(m_nShape[MIDLAYER1] == PAD_SHAPE_ROUNDEDRECTANGLE) DrawPadRoundedRectangle(pDC, pV, MIDLAYER1, midlayer, 0);
					else DrawPadRound(pDC, pV, MIDLAYER1, midlayer, 0);
				}
			}
			if(pDoc->LayerVisible(TOPLAYER) == TRUE)
			{
				
				if(m_nShape[TOPLAYER] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, TOPLAYER, TOPLAYER, 0);
				else if(m_nShape[TOPLAYER] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, TOPLAYER, TOPLAYER, 0);
				else if(m_nShape[TOPLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE) DrawPadRoundedRectangle(pDC, pV, TOPLAYER, TOPLAYER, 0);
				else DrawPadRound(pDC, pV, TOPLAYER, TOPLAYER, 0);
			}
		}
	}
	else
	{
		
	}
	
	return TRUE;
}

BOOL COb_pcbpad::DrawPadHoles(CDC* pDC, CView* pV)
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

	if(((pView->m_nPrint_Flag == 0)||((pView->m_nPrint_Flag == 1)&&(bEDA_PCBPrint_Holes == TRUE)))&&(pDoc->m_sPCB_LayerParam[PADHOLES].bLayerShow == TRUE))
	{
		COLORREF color;
		if(pView->m_nPrint_Flag == 1)
		{
			color = RGB(255,255,255);
		}
		else
		{
			if(m_nFlag == FLAG_DELETED) color = nEDA_PCB_BoardAreaColor;
			else color = nEDA_PCB_LayerColor[PADHOLES];
		}
		
		CPen holepen, *oldpen;
		holepen.CreatePen(PS_SOLID,1,color);
		oldpen = pDC->SelectObject(&holepen);
			 
		CBrush holebrush, *oldbrush;
		holebrush.CreateSolidBrush(color);
		oldbrush = pDC->SelectObject(&holebrush);

		if(m_nHoleType == PADHOLE_ROUND)
		{
			//int cx, cy, r;
			//pView->Get_VPort_XY(&cx,&cy,m_fCx, m_fCy);
			int r = (int)(m_fHoleDia/pView->m_fScale);

			if(pView->m_nPrint_Flag == 1) r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);

			RECT rect;
			rect.left = cx - r/2;
			rect.right = cx + r/2;
			rect.top = cy - r/2;
			rect.bottom = cy + r/2;

			pDC->Ellipse(&rect);
		}
		else if(m_nHoleType == PADHOLE_SQUARE)
		{
			COb_pcbfill pcbfill;
			pcbfill.m_nLayer = PADHOLES;
			pcbfill.m_fX1 = m_fCx - m_fHoleDia/2;
			pcbfill.m_fY1 = m_fCy - m_fHoleDia/2;
			pcbfill.m_fX2 = m_fCx + m_fHoleDia/2;
			pcbfill.m_fY2 = m_fCy + m_fHoleDia/2;
			pcbfill.m_fRotation = m_fHoleRotation;
			//pcbfill.Draw(pDC, pV);
			double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
			pcbfill.GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	

			POINT point[5];
			int x,y;
			pView->Get_VPort_XY(&x,&y,fx1,fy1);
			if(pView->m_nPrint_Flag == 1)
			{
				x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
			}
			point[0].x = x; 
			point[0].y = y;
			point[4].x = x; 
			point[4].y = y;
			
			pView->Get_VPort_XY(&x,&y,fx3,fy3);
			if(pView->m_nPrint_Flag == 1)
			{
				x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
			}
			point[1].x = x;
			point[1].y = y;
			
			pView->Get_VPort_XY(&x,&y,fx2,fy2);
			if(pView->m_nPrint_Flag == 1)
			{
				x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
			}
			point[2].x = x;
			point[2].y = y;
			
			pView->Get_VPort_XY(&x,&y,fx4,fy4);
			if(pView->m_nPrint_Flag == 1)
			{
				x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
			}
			point[3].x = x;
			point[3].y = y;

			pDC->Polygon(point, 5);
		}
		else if(m_nHoleType == PADHOLE_SLOT)
		{
			if(m_fHoleWidth == m_fHoleDia)
			{
				double fcx, fcy, fr;
				int cx,cy,r;
				fcx = m_fCx;
				fcy = m_fCy;
				fr = m_fHoleDia/2;
					
				pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
				r = (int)(fr/pView->m_fScale);
				if(pView->m_nPrint_Flag == 1)
				{
					cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
					cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
					r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				}
				pDC->Ellipse(cx-r, cy-r, cx+r, cy+r);
			}
			else if(m_fHoleWidth > m_fHoleDia)
			{
				double fcx, fcy, fr;
				int cx, cy, r;

				fcx = m_fCx - cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
				fcy = m_fCy - sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
				fr = m_fHoleDia;
						
				pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
				r = (int)(fr/pView->m_fScale);
				if(pView->m_nPrint_Flag == 1)
				{
					cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
					cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
					r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				}
				pDC->MoveTo(cx, cy);


				fcx = m_fCx + cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
				fcy = m_fCy + sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
								
				pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
				r = (int)(fr/pView->m_fScale);
				if(pView->m_nPrint_Flag == 1)
				{
					cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
					cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
					r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				}

				CPen pen;
				pen.CreatePen(PS_SOLID, r, color);
				CPen* oldpen =  pDC->SelectObject(&pen);
						
				pDC->LineTo(cx, cy);
				pDC->SelectObject(oldpen);
			}
			else
			{
				double fcx, fcy, fr;
				int cx, cy, r;

				fcx = m_fCx - sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
				fcy = m_fCy + cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
				fr = m_fHoleWidth;
								
				pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
				r = (int)(fr/pView->m_fScale);
				if(pView->m_nPrint_Flag == 1)
				{
					cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
					cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
					r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				}
				pDC->MoveTo(cx, cy);


				fcx = m_fCx + sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
				fcy = m_fCy - cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
				fr = m_fHoleWidth;
							
				pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
				r = (int)(fr/pView->m_fScale);
				if(pView->m_nPrint_Flag == 1)
				{
					cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
					cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
					r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				}

				CPen pen;
				pen.CreatePen(PS_SOLID, r, color);
				CPen* oldpen =  pDC->SelectObject(&pen);
					
				pDC->LineTo(cx, cy);
				pDC->SelectObject(oldpen);
			}
		}

		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);

		return TRUE;
	}
	else return FALSE;
}

void COb_pcbpad::DrawPlaneMark(CDC* pDC, CView* pV)
{	
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if((pView->m_nPrint_Flag == 0)&&(m_nNet >= 0)&&(m_nLayer == MULTILAYER))
	{
		for(int layer=POWERPLANE1; layer <= POWERPLANE16; layer++)
		{
			if(ConnectToInternalPlane(layer) == TRUE)
			{
				if(m_nCCS == 0)
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
				else if(m_nCCS == 1)
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

void COb_pcbpad::DrawNetName(CDC* pDC, CView* pV)
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

			if((MIN(m_fSizeX[TOPLAYER], m_fSizeY[TOPLAYER])/pView->m_fScale) < height) height = height/2;

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
			CSize size;
			
			if(m_nNet >= 0)
			{
				COb_pcbnet* pcbnet = (COb_pcbnet*)pDoc->m_arrayPcbNet.GetAt(m_nNet);
				text = pcbnet->m_csNetName;
			}
			else text.Empty();			
			
			if(text.IsEmpty() == TRUE)
			{
				text = m_csName;
				size = pDC->GetTextExtent(text);
				x = cx - size.cx/2;
				y = cy - size.cy/2;
				pDC->TextOut(x, y, text, strlen(text));
			}
			else
			{
				size = pDC->GetTextExtent(text);
				x = cx - size.cx/2;
				y = cy;
				pDC->TextOut(x, y, text, strlen(text));
				
				text = m_csName;
				size = pDC->GetTextExtent(text);
				x = cx - size.cx/2;
				y = cy - size.cy;
				pDC->TextOut(x, y, text, strlen(text));
			}

			pDC->SelectObject(oldfont);
			
			pDC->SetTextColor(oldtextcolor);
			pDC->SetBkMode(bkmode);
		}
	}
}

void COb_pcbpad::Move(CClientDC* pDC, CView* pV,int index)
{
	int ch1;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE; 
	
	char szHelp[] = "移动焊盘: <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV, DRAW_NORMAL);


	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_pcbpad apad;
	apad.Copy(this);
	apad.m_nFlag = FLAG_NORMAL;
	apad.Draw(pDC, pV, DRAW_DRAFT);

	pView->m_fCurrentSnapX = apad.m_fCx;
	pView->m_fCurrentSnapY = apad.m_fCy;
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
			apad.Draw(pDC, pV, DRAW_DRAFT);
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
				apad.Draw(pDC, pV, DRAW_DRAFT);
				//if(apad.m_nLayer != MULTILAYER) apad.DrawSingleLayerPad(pDC, pV, 1);
				//else apad.DrawMultiLayerPad(pDC, pV, 1);
				pView->OpenMouseCursor();
			}

			apad.m_fCx = pView->m_fCurrentSnapX;
			apad.m_fCy = pView->m_fCurrentSnapY;

			pView->CloseMouseCursor();
			apad.Draw(pDC, pV, DRAW_DRAFT);
			//if(apad.m_nLayer != MULTILAYER) apad.DrawSingleLayerPad(pDC, pV, 1);
			//else apad.DrawMultiLayerPad(pDC, pV, 1);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->PcbElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			apad.Draw(pDC, pV, DRAW_DRAFT);
			//if(apad.m_nLayer != MULTILAYER) apad.DrawSingleLayerPad(pDC, pV, 1);
			//else apad.DrawMultiLayerPad(pDC, pV, 1);
			pView->OpenMouseCursor();

			apad.m_fCx = pView->m_fCurrentSnapX;
			apad.m_fCy = pView->m_fCurrentSnapY;

	
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_PAD;
			pundo->m_nIndex = index;
			pundo->pcbpad.Copy(this);
			pundo->pcbpad.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
	
			m_fCx = apad.m_fCx;
			m_fCy = apad.m_fCy;
			m_fRotation = apad.m_fRotation;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			apad.Draw(pDC, pV, DRAW_DRAFT);
			//if(apad.m_nLayer != MULTILAYER) apad.DrawSingleLayerPad(pDC, pV, 1);
			//else apad.DrawMultiLayerPad(pDC, pV, 1);
			
			apad.m_fRotation += 90;
			apad.m_fRotation = pDoc->Get_Correct_Angle(apad.m_fRotation);

			apad.Draw(pDC, pV, DRAW_DRAFT);
			//if(apad.m_nLayer != MULTILAYER) apad.DrawSingleLayerPad(pDC, pV, 1);
			//else apad.DrawMultiLayerPad(pDC, pV, 1);
			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	
	pView->CloseMouseCursor();

	m_nFlag = FLAG_NORMAL;
	Draw(pDC, pV, DRAW_NORMAL);

	pView->OpenMouseCursor();

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		//if(bEDA_AutoJunction == TRUE) pView->AutoRemoveJunction();
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
	
	bEDA_NeedElectricalGrid = FALSE;
	pView->m_fpPcbHotPoint1.fx = pView->m_fpPcbHotPoint1.fy = pView->m_fpPcbHotPoint2.fx = pView->m_fpPcbHotPoint2.fy = 0;
	
	pView->Write_Help(szEDA_Help_Ready);
}

BOOL COb_pcbpad::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;

	char szHelp[] = "复制焊盘: <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	pView->Invalidate();

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	


	COb_pcbpad apad;
	apad.Copy(this);
	apad.m_nFlag = FLAG_NORMAL;
	apad.Draw(pDC, pV, DRAW_DRAFT);

	pView->m_fCurrentSnapX = apad.m_fCx;
	pView->m_fCurrentSnapY = apad.m_fCy;
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
			apad.Draw(pDC, pV, DRAW_DRAFT);

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
				apad.Draw(pDC, pV, DRAW_DRAFT);
				//if(apad.m_nLayer != MULTILAYER) apad.DrawSingleLayerPad(pDC, pV, 1);
				//else apad.DrawMultiLayerPad(pDC, pV, 1);
				pView->OpenMouseCursor();
			}

			apad.m_fCx = pView->m_fCurrentSnapX;
			apad.m_fCy = pView->m_fCurrentSnapY;

			pView->CloseMouseCursor();
			apad.Draw(pDC, pV, DRAW_DRAFT);
			//if(apad.m_nLayer != MULTILAYER) apad.DrawSingleLayerPad(pDC, pV, 1);
			//else apad.DrawMultiLayerPad(pDC, pV, 1);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->PcbElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			apad.Draw(pDC, pV, DRAW_DRAFT);
			//if(apad.m_nLayer != MULTILAYER) apad.DrawSingleLayerPad(pDC, pV, 1);
			//else apad.DrawMultiLayerPad(pDC, pV, 1);
			pView->OpenMouseCursor();

			apad.m_fCx = pView->m_fCurrentSnapX;
			apad.m_fCy = pView->m_fCurrentSnapY;

			m_fCx = apad.m_fCx;
			m_fCy = apad.m_fCy;
			m_fRotation = apad.m_fRotation;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			apad.Draw(pDC, pV, DRAW_DRAFT);
			//if(apad.m_nLayer != MULTILAYER) apad.DrawSingleLayerPad(pDC, pV, 1);
			//else apad.DrawMultiLayerPad(pDC, pV, 1);
			
			apad.m_fRotation += 90;
			apad.m_fRotation = pDoc->Get_Correct_Angle(apad.m_fRotation);
			
			apad.Draw(pDC, pV, DRAW_DRAFT);
			//if(apad.m_nLayer != MULTILAYER) apad.DrawSingleLayerPad(pDC, pV, 1);
			//else apad.DrawMultiLayerPad(pDC, pV, 1);
			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	
	pView->CloseMouseCursor();
	m_nFlag = FLAG_NORMAL;
	Draw(pDC, pV, DRAW_NORMAL);

	pView->OpenMouseCursor();

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	ClipCursor(NULL); pView->m_bClipState = FALSE;

	pView->Write_Help(szEDA_Help_Ready);
	
	bEDA_NeedElectricalGrid = FALSE;
	pView->m_fpPcbHotPoint1.fx = pView->m_fpPcbHotPoint1.fy = pView->m_fpPcbHotPoint2.fx = pView->m_fpPcbHotPoint2.fy = 0;

	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;
}

void COb_pcbpad::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	if(m_nShape[TOPLAYER] == PAD_SHAPE_RECTANGLE)
	{
		COb_pcbfill pcbfill;
		pcbfill.m_nLayer = m_nLayer;
		pcbfill.m_fX1 = m_fCx - m_fSizeX[TOPLAYER]/2;
		pcbfill.m_fY1 = m_fCy - m_fSizeY[TOPLAYER]/2;
		pcbfill.m_fX2 = m_fCx + m_fSizeX[TOPLAYER]/2;
		pcbfill.m_fY2 = m_fCy + m_fSizeY[TOPLAYER]/2;
		pcbfill.m_fRotation = m_fRotation;
		pcbfill.m_nFlag = m_nFlag;
		pcbfill.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if((m_nShape[TOPLAYER] == PAD_SHAPE_OCTAGONAL)||(m_nShape[TOPLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE))
	{
		COLORREF color = nEDA_PCB_LayerColor[m_nLayer];

		Fpoint fpoint[9];
		if( m_fSizeX[TOPLAYER] >= m_fSizeY[TOPLAYER])
		{
			fpoint[0].fx = m_fCx - m_fSizeX[TOPLAYER]/2 + m_fSizeY[TOPLAYER]/4;
			fpoint[0].fy = m_fCy - m_fSizeY[TOPLAYER]/2;				
			fpoint[1].fx = m_fCx - m_fSizeX[TOPLAYER]/2;
			fpoint[1].fy = m_fCy - m_fSizeY[TOPLAYER]/4;
			fpoint[2].fx = m_fCx - m_fSizeX[TOPLAYER]/2;
			fpoint[2].fy = m_fCy + m_fSizeY[TOPLAYER]/4;
			fpoint[3].fx = m_fCx - m_fSizeX[TOPLAYER]/2 + m_fSizeY[TOPLAYER]/4;
			fpoint[3].fy = m_fCy + m_fSizeY[TOPLAYER]/2;
			fpoint[4].fx = m_fCx + m_fSizeX[TOPLAYER]/2 - m_fSizeY[TOPLAYER]/4;
			fpoint[4].fy = m_fCy + m_fSizeY[TOPLAYER]/2;
			fpoint[5].fx = m_fCx + m_fSizeX[TOPLAYER]/2;
			fpoint[5].fy = m_fCy + m_fSizeY[TOPLAYER]/4;
			fpoint[6].fx = m_fCx + m_fSizeX[TOPLAYER]/2;
			fpoint[6].fy = m_fCy - m_fSizeY[TOPLAYER]/4;
			fpoint[7].fx = m_fCx + m_fSizeX[TOPLAYER]/2 - m_fSizeY[TOPLAYER]/4;
			fpoint[7].fy = m_fCy - m_fSizeY[TOPLAYER]/2;

			for(int i=0; i<8; i++)
			{
				fpoint[i].fx -= m_fCx;
				fpoint[i].fy -= m_fCy;
			}

			double alpha, fr;
			for(int i=0; i<8; i++)
			{
				if((m_fRotation == 0)||(m_fRotation == 180)||(m_fRotation == 360))
				{
					fpoint[i].fx = m_fCx + fpoint[i].fx;
					fpoint[i].fy = m_fCy + fpoint[i].fy;
				}
				else if((m_fRotation == 90)||(m_fRotation == 270))
				{
					double temp;
					temp = fpoint[i].fx;
					fpoint[i].fx = m_fCx + fpoint[i].fy;
					fpoint[i].fy = m_fCy + temp;
				}
				else
				{
					alpha = atan2(fpoint[i].fy, fpoint[i].fx);
					fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
					fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
					fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
				}
			}
		}
		else
		{
			fpoint[0].fx = m_fCx - m_fSizeX[TOPLAYER]/4;
			fpoint[0].fy = m_fCy - m_fSizeY[TOPLAYER]/2;
			fpoint[1].fx = m_fCx - m_fSizeX[TOPLAYER]/2;
			fpoint[1].fy = m_fCy - m_fSizeY[TOPLAYER]/2 + m_fSizeX[TOPLAYER]/4;
			fpoint[2].fx = m_fCx - m_fSizeX[TOPLAYER]/2;
			fpoint[2].fy = m_fCy + m_fSizeY[TOPLAYER]/2 - m_fSizeX[TOPLAYER]/4;
			fpoint[3].fx = m_fCx - m_fSizeX[TOPLAYER]/4;
			fpoint[3].fy = m_fCy + m_fSizeY[TOPLAYER]/2;
			fpoint[4].fx = m_fCx + m_fSizeX[TOPLAYER]/4;
			fpoint[4].fy = m_fCy + m_fSizeY[TOPLAYER]/2;
			fpoint[5].fx = m_fCx + m_fSizeX[TOPLAYER]/2;
			fpoint[5].fy = m_fCy + m_fSizeY[TOPLAYER]/2 - m_fSizeX[TOPLAYER]/4;
			fpoint[6].fx = m_fCx + m_fSizeX[TOPLAYER]/2;
			fpoint[6].fy = m_fCy - m_fSizeY[TOPLAYER]/2 + m_fSizeX[TOPLAYER]/4;
			fpoint[7].fx = m_fCx + m_fSizeX[TOPLAYER]/4;
			fpoint[7].fy = m_fCy - m_fSizeY[TOPLAYER]/2;

			for(int i=0; i<8; i++)
			{
				fpoint[i].fx -= m_fCx;
				fpoint[i].fy -= m_fCy;
			}

			double alpha, fr;
			for(int i=0; i<8; i++)
			{
				if((m_fRotation == 0)||(m_fRotation == 180)||(m_fRotation == 360))
				{
					fpoint[i].fx = m_fCx + fpoint[i].fx;
					fpoint[i].fy = m_fCy + fpoint[i].fy;
				}
				else if((m_fRotation == 90)||(m_fRotation == 270))
				{
					double temp;
					temp = fpoint[i].fx;
					fpoint[i].fx = m_fCx + fpoint[i].fy;
					fpoint[i].fy = m_fCy + temp;
				}
				else
				{
					alpha = atan2(fpoint[i].fy, fpoint[i].fx);
					fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
					fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
					fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
				}
			}
		}

		POINT point[9];
		for(int i=0; i<8; i++)
		{
			int x,y;
			x = (int)((fpoint[i].fx - origin_x)/scale);
			y = nEy - (int)((fpoint[i].fy - origin_y)/scale);	
			//pView->Get_VPort_XY(&x,&y,fpoint[i].fx, fpoint[i].fy);
			point[i].x = x;
			point[i].y = y;
		}
		point[8].x = point[0].x;
		point[8].y = point[0].y;

		CPen fillpen;
		fillpen.CreatePen(PS_SOLID,1,color);
		CPen* oldpen =  pDC->SelectObject(&fillpen);
	 
		CBrush fillbrush;
		fillbrush.CreateSolidBrush(color);
		CBrush* oldbrush = pDC->SelectObject(&fillbrush);		
		
		pDC->Polygon(point, 9);

		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);
	}
	else 
	{
		COLORREF color = nEDA_PCB_LayerColor[m_nLayer];

		CPen fillpen;
		fillpen.CreatePen(PS_SOLID,1,color);
		CPen* oldpen =  pDC->SelectObject(&fillpen);
	 
		CBrush fillbrush;
		fillbrush.CreateSolidBrush(color);
		CBrush* oldbrush = pDC->SelectObject(&fillbrush);

		if(m_fSizeX[TOPLAYER] == m_fSizeY[TOPLAYER])
		{
			int cx,cy,r;
				
			//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
			cx = (int)((m_fCx - origin_x)/scale);
			cy = nEy - (int)((m_fCy - origin_y)/scale);	
			r = (int)(m_fSizeX[TOPLAYER]/2/scale);

			pDC->Ellipse(cx-r, cy-r, cx+r, cy+r);

		}
		else if(m_fSizeX[TOPLAYER] > m_fSizeY[TOPLAYER])
		{
			double fcx, fcy;
			int cx, cy, r;

			r = (int)(m_fSizeY[TOPLAYER]/scale);

			CPen pen;
			pen.CreatePen(PS_SOLID, r, color);
			CPen* oldpen =  pDC->SelectObject(&pen);

			fcx = m_fCx - cos(m_fRotation*fEDA_pi/180)*(m_fSizeX[TOPLAYER] - m_fSizeY[TOPLAYER])/2;
			fcy = m_fCy - sin(m_fRotation*fEDA_pi/180)*(m_fSizeX[TOPLAYER] - m_fSizeY[TOPLAYER])/2;
			//pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
			cx = (int)((fcx - origin_x)/scale);
			cy = nEy - (int)((fcy - origin_y)/scale);	
	
			pDC->MoveTo(cx, cy);

			fcx = m_fCx + cos(m_fRotation*fEDA_pi/180)*(m_fSizeX[TOPLAYER] - m_fSizeY[TOPLAYER])/2;
			fcy = m_fCy + sin(m_fRotation*fEDA_pi/180)*(m_fSizeX[TOPLAYER] - m_fSizeY[TOPLAYER])/2;
						
			//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
			cx = (int)((fcx - origin_x)/scale);
			cy = nEy - (int)((fcy - origin_y)/scale);	

			pDC->LineTo(cx, cy);

			pDC->SelectObject(oldpen);
		}
		else
		{
			double fcx, fcy;
			int cx, cy, r;

			r = (int)(m_fSizeX[TOPLAYER]/scale);

			CPen pen;
			pen.CreatePen(PS_SOLID, r, color);
			CPen* oldpen =  pDC->SelectObject(&pen);

			fcx = m_fCx - sin(m_fRotation*fEDA_pi/180)*(m_fSizeY[TOPLAYER] - m_fSizeX[TOPLAYER])/2;
			fcy = m_fCy + cos(m_fRotation*fEDA_pi/180)*(m_fSizeY[TOPLAYER] - m_fSizeX[TOPLAYER])/2;

			//pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
			cx = (int)((fcx - origin_x)/scale);
			cy = nEy - (int)((fcy - origin_y)/scale);	

			pDC->MoveTo(cx, cy);

			fcx = m_fCx + sin(m_fRotation*fEDA_pi/180)*(m_fSizeY[TOPLAYER] - m_fSizeX[TOPLAYER])/2;
			fcy = m_fCy - cos(m_fRotation*fEDA_pi/180)*(m_fSizeY[TOPLAYER] - m_fSizeX[TOPLAYER])/2;
						
			//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
			cx = (int)((fcx - origin_x)/scale);
			cy = nEy - (int)((fcy - origin_y)/scale);	

			pDC->LineTo(cx, cy);

			pDC->SelectObject(oldpen);
		}
		
		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);
	}
	
	int cx,cy;

	cx = (int)((m_fCx - origin_x)/scale);
	cy = nEy - (int)((m_fCy - origin_y)/scale);	

	COLORREF color = nEDA_PCB_LayerColor[PADHOLES];

	CPen holepen, *oldpen;
	holepen.CreatePen(PS_SOLID,1,color);
	oldpen = pDC->SelectObject(&holepen);
			 
	CBrush holebrush, *oldbrush;
	holebrush.CreateSolidBrush(color);
	oldbrush = pDC->SelectObject(&holebrush);

	if(m_nHoleType == PADHOLE_ROUND)
	{
		int r = (int)(m_fHoleDia/scale);

		RECT rect;
		rect.left = cx - r/2;
		rect.right = cx + r/2;
		rect.top = cy - r/2;
		rect.bottom = cy + r/2;

		pDC->Ellipse(&rect);
	}
	else if(m_nHoleType == PADHOLE_SQUARE)
	{
		COb_pcbfill pcbfill;
		pcbfill.m_nLayer = PADHOLES;
		pcbfill.m_fX1 = m_fCx - m_fHoleDia/2;
		pcbfill.m_fY1 = m_fCy - m_fHoleDia/2;
		pcbfill.m_fX2 = m_fCx + m_fHoleDia/2;
		pcbfill.m_fY2 = m_fCy + m_fHoleDia/2;
		pcbfill.m_fRotation = m_fHoleRotation;

		double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
		pcbfill.GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	

		POINT point[5];
		int x,y;
		//pView->Get_VPort_XY(&x,&y,fx1,fy1);
		x = (int)((fx1 - origin_x)/scale);
		y = nEy - (int)((fy1 - origin_y)/scale);	
		point[0].x = x; 
		point[0].y = y;
		point[4].x = x; 
		point[4].y = y;
			
		//pView->Get_VPort_XY(&x,&y,fx3,fy3);
		x = (int)((fx3 - origin_x)/scale);
		y = nEy - (int)((fy3 - origin_y)/scale);	
		point[1].x = x;
		point[1].y = y;
			
		//pView->Get_VPort_XY(&x,&y,fx2,fy2);
		x = (int)((fx2 - origin_x)/scale);
		y = nEy - (int)((fy2 - origin_y)/scale);	
		point[2].x = x;
		point[2].y = y;
			
		//pView->Get_VPort_XY(&x,&y,fx4,fy4);
		x = (int)((fx4 - origin_x)/scale);
		y = nEy - (int)((fy4 - origin_y)/scale);
		point[3].x = x;
		point[3].y = y;

		pDC->Polygon(point, 5);
	}
	else if(m_nHoleType == PADHOLE_SLOT)
	{
		if(m_fHoleWidth == m_fHoleDia)
		{
			double fcx, fcy, fr;
			int cx,cy,r;
			fcx = m_fCx;
			fcy = m_fCy;
			fr = m_fHoleDia/2;
					
			//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
			cx = (int)((fcx - origin_x)/scale);
			cy = nEy - (int)((fcy - origin_y)/scale);	
			r = (int)(fr/scale);

			pDC->Ellipse(cx-r, cy-r, cx+r, cy+r);
		}
		else if(m_fHoleWidth > m_fHoleDia)
		{
			double fcx, fcy, fr;
			int cx, cy, r;

			fcx = m_fCx - cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
			fcy = m_fCy - sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
			fr = m_fHoleDia;
					
			//pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
			cx = (int)((fcx - origin_x)/scale);
			cy = nEy - (int)((fcy - origin_y)/scale);	
			r = (int)(fr/scale);

			pDC->MoveTo(cx, cy);
			
			fcx = m_fCx + cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
			fcy = m_fCy + sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
							
			//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
			cx = (int)((fcx - origin_x)/scale);
			cy = nEy - (int)((fcy - origin_y)/scale);	
			r = (int)(fr/scale);
				
			CPen pen;
			pen.CreatePen(PS_SOLID, r, color);
			CPen* oldpen =  pDC->SelectObject(&pen);
					
			pDC->LineTo(cx, cy);
			pDC->SelectObject(oldpen);
		}
		else
		{
			double fcx, fcy, fr;
			int cx, cy, r;
			
			fcx = m_fCx - sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
			fcy = m_fCy + cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
			fr = m_fHoleWidth;
							
			//pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
			cx = (int)((fcx - origin_x)/scale);
			cy = nEy - (int)((fcy - origin_y)/scale);	
			r = (int)(fr/scale);

			pDC->MoveTo(cx, cy);
				
			fcx = m_fCx + sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
			fcy = m_fCy - cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
			fr = m_fHoleWidth;
						
			//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
			cx = (int)((fcx - origin_x)/scale);
			cy = nEy - (int)((fcy - origin_y)/scale);	
			r = (int)(fr/scale);

			CPen pen;
			pen.CreatePen(PS_SOLID, r, color);
			CPen* oldpen =  pDC->SelectObject(&pen);
				
			pDC->LineTo(cx, cy);
			pDC->SelectObject(oldpen);
		}
	}

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);

}

void COb_pcbpad::DrawInLayerWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy, int layer)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	if( (layer == TOPPASTE) || (layer == BOTTOMPASTE) )
	{
		if( ((layer == TOPPASTE)&&(m_nLayer == TOPLAYER))||((layer == BOTTOMPASTE)&&(m_nLayer == BOTTOMLAYER)) )
		{
			int size_layer = TOPLAYER;
			double expansion;	
			if(m_nCPEV == 1) expansion = pDoc->m_fPasteMaskExpansion;
			else expansion = m_fCPE;

			if(m_nShape[size_layer] == PAD_SHAPE_RECTANGLE)
			{
				COb_pcbfill pcbfill;
				pcbfill.m_nLayer = layer;
				pcbfill.m_fX1 = m_fCx - m_fSizeX[size_layer]/2 - expansion;
				pcbfill.m_fY1 = m_fCy - m_fSizeY[size_layer]/2 - expansion;
				pcbfill.m_fX2 = m_fCx + m_fSizeX[size_layer]/2 + expansion;
				pcbfill.m_fY2 = m_fCy + m_fSizeY[size_layer]/2 + expansion;
				pcbfill.m_fRotation = m_fRotation;
				pcbfill.m_nFlag = m_nFlag;
				pcbfill.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
			}
			else if((m_nShape[size_layer] == PAD_SHAPE_OCTAGONAL)||(m_nShape[size_layer] == PAD_SHAPE_ROUNDEDRECTANGLE))
			{
				COLORREF color = nEDA_PCB_LayerColor[layer];

				Fpoint fpoint[9];
				if( m_fSizeX[size_layer] >= m_fSizeY[size_layer])
				{
					fpoint[0].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/2 + (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[0].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/2;				
					fpoint[1].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[1].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[2].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[2].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[3].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/2 + (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[3].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/2;
					fpoint[4].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/2 - (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[4].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/2;
					fpoint[5].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[5].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[6].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[6].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[7].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/2 - (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[7].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/2;

					for(int i=0; i<8; i++)
					{
						fpoint[i].fx -= m_fCx;
						fpoint[i].fy -= m_fCy;
					}

					double alpha, fr;
					for(int i=0; i<8; i++)
					{
						if((m_fRotation == 0)||(m_fRotation == 180)||(m_fRotation == 360))
						{
							fpoint[i].fx = m_fCx + fpoint[i].fx;
							fpoint[i].fy = m_fCy + fpoint[i].fy;
						}
						else if((m_fRotation == 90)||(m_fRotation == 270))
						{
							double temp;
							temp = fpoint[i].fx;
							fpoint[i].fx = m_fCx + fpoint[i].fy;
							fpoint[i].fy = m_fCy + temp;
						}
						else
						{
							alpha = atan2(fpoint[i].fy, fpoint[i].fx);
							fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
							fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
							fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
						}
					}
				}
				else
				{
					fpoint[0].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[0].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/2;
					fpoint[1].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[1].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/2 + (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[2].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[2].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/2 - (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[3].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[3].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/2;
					fpoint[4].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[4].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/2;
					fpoint[5].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[5].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/2 - (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[6].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[6].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/2 + (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[7].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[7].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/2;

					for(int i=0; i<8; i++)
					{
						fpoint[i].fx -= m_fCx;
						fpoint[i].fy -= m_fCy;
					}

					double alpha, fr;
					for(int i=0; i<8; i++)
					{
						if((m_fRotation == 0)||(m_fRotation == 180)||(m_fRotation == 360))
						{
							fpoint[i].fx = m_fCx + fpoint[i].fx;
							fpoint[i].fy = m_fCy + fpoint[i].fy;
						}
						else if((m_fRotation == 90)||(m_fRotation == 270))
						{
							double temp;
							temp = fpoint[i].fx;
							fpoint[i].fx = m_fCx + fpoint[i].fy;
							fpoint[i].fy = m_fCy + temp;
						}
						else
						{
							alpha = atan2(fpoint[i].fy, fpoint[i].fx);
							fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
							fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
							fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
						}
					}
				}

				POINT point[9];
				for(int i=0; i<8; i++)
				{
					int x,y;
					x = (int)((fpoint[i].fx - origin_x)/scale);
					y = nEy - (int)((fpoint[i].fy - origin_y)/scale);	
					//pView->Get_VPort_XY(&x,&y,fpoint[i].fx, fpoint[i].fy);
					point[i].x = x;
					point[i].y = y;
				}
				point[8].x = point[0].x;
				point[8].y = point[0].y;

				CPen fillpen;
				fillpen.CreatePen(PS_SOLID,1,color);
				CPen* oldpen =  pDC->SelectObject(&fillpen);
			 
				CBrush fillbrush;
				fillbrush.CreateSolidBrush(color);
				CBrush* oldbrush = pDC->SelectObject(&fillbrush);		
				
				pDC->Polygon(point, 9);

				pDC->SelectObject(oldpen);
				pDC->SelectObject(oldbrush);
			}
			else
			{
				COLORREF color = nEDA_PCB_LayerColor[layer];

				CPen fillpen;
				fillpen.CreatePen(PS_SOLID,1,color);
				CPen* oldpen =  pDC->SelectObject(&fillpen);
			 
				CBrush fillbrush;
				fillbrush.CreateSolidBrush(color);
				CBrush* oldbrush = pDC->SelectObject(&fillbrush);

				if(m_fSizeX[size_layer] == m_fSizeY[size_layer])
				{
					double fr = m_fSizeX[size_layer]/2 + expansion;
					int cx,cy,r;
						
					//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
					cx = (int)((m_fCx - origin_x)/scale);
					cy = nEy - (int)((m_fCy - origin_y)/scale);	
					r = (int)(fr/scale);

					pDC->Ellipse(cx-r, cy-r, cx+r, cy+r);

				}
				else if(m_fSizeX[size_layer] > m_fSizeY[size_layer])
				{
					double fcx, fcy;
					int cx, cy, r;
					double fr = m_fSizeY[size_layer] + expansion*2;

					r = (int)(fr/scale);

					CPen pen;
					pen.CreatePen(PS_SOLID, r, color);
					CPen* oldpen =  pDC->SelectObject(&pen);

					fcx = m_fCx - cos(m_fRotation*fEDA_pi/180)*(m_fSizeX[size_layer] - m_fSizeY[size_layer])/2;
					fcy = m_fCy - sin(m_fRotation*fEDA_pi/180)*(m_fSizeX[size_layer] - m_fSizeY[size_layer])/2;
					//pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
					cx = (int)((fcx - origin_x)/scale);
					cy = nEy - (int)((fcy - origin_y)/scale);	
			
					pDC->MoveTo(cx, cy);

					fcx = m_fCx + cos(m_fRotation*fEDA_pi/180)*(m_fSizeX[size_layer] - m_fSizeY[size_layer])/2;
					fcy = m_fCy + sin(m_fRotation*fEDA_pi/180)*(m_fSizeX[size_layer] - m_fSizeY[size_layer])/2;
								
					//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
					cx = (int)((fcx - origin_x)/scale);
					cy = nEy - (int)((fcy - origin_y)/scale);	

					pDC->LineTo(cx, cy);

					pDC->SelectObject(oldpen);
				}
				else
				{
					double fcx, fcy;
					int cx, cy, r;
					double fr = m_fSizeX[size_layer] + expansion*2;

					r = (int)(fr/scale);

					CPen pen;
					pen.CreatePen(PS_SOLID, r, color);
					CPen* oldpen =  pDC->SelectObject(&pen);

					fcx = m_fCx - sin(m_fRotation*fEDA_pi/180)*(m_fSizeY[size_layer] - m_fSizeX[size_layer])/2;
					fcy = m_fCy + cos(m_fRotation*fEDA_pi/180)*(m_fSizeY[size_layer] - m_fSizeX[size_layer])/2;

					//pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
					cx = (int)((fcx - origin_x)/scale);
					cy = nEy - (int)((fcy - origin_y)/scale);	

					pDC->MoveTo(cx, cy);

					fcx = m_fCx + sin(m_fRotation*fEDA_pi/180)*(m_fSizeY[size_layer] - m_fSizeX[size_layer])/2;
					fcy = m_fCy - cos(m_fRotation*fEDA_pi/180)*(m_fSizeY[size_layer] - m_fSizeX[size_layer])/2;
								
					//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
					cx = (int)((fcx - origin_x)/scale);
					cy = nEy - (int)((fcy - origin_y)/scale);	

					pDC->LineTo(cx, cy);

					pDC->SelectObject(oldpen);
				}
				
				pDC->SelectObject(oldpen);
				pDC->SelectObject(oldbrush);
			}
		}
	}
	else if( (layer == TOPSOLDER) || (layer == BOTTOMSOLDER) )
	{
		if( ((layer == TOPSOLDER)&&(m_bTentingTop == FALSE)&&((m_nLayer == MULTILAYER)||(m_nLayer == TOPLAYER))) ||\
			((layer == BOTTOMSOLDER)&&(m_bTentingBottom == FALSE)&&((m_nLayer == MULTILAYER)||(m_nLayer == BOTTOMLAYER))) )
		{
			int size_layer;
			if(m_nLayer != MULTILAYER) size_layer = TOPLAYER;
			else
			{
				if(m_nPadMode == PAD_MODE_SIMPLE) size_layer = TOPLAYER;
				else
				{
					if(layer == TOPSOLDER) size_layer = TOPLAYER;
					else size_layer = BOTTOMLAYER;
				}
			}

			double expansion;	
			if(m_nCSEV == 1) expansion = pDoc->m_fSolderMaskExpansion;
			else expansion = m_fCSE;

			if(m_nShape[size_layer] == PAD_SHAPE_RECTANGLE)
			{
				COb_pcbfill pcbfill;
				pcbfill.m_nLayer = layer;
				pcbfill.m_fX1 = m_fCx - m_fSizeX[size_layer]/2 - expansion;
				pcbfill.m_fY1 = m_fCy - m_fSizeY[size_layer]/2 - expansion;
				pcbfill.m_fX2 = m_fCx + m_fSizeX[size_layer]/2 + expansion;
				pcbfill.m_fY2 = m_fCy + m_fSizeY[size_layer]/2 + expansion;
				pcbfill.m_fRotation = m_fRotation;
				pcbfill.m_nFlag = m_nFlag;
				pcbfill.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
			}
			else if((m_nShape[size_layer] == PAD_SHAPE_OCTAGONAL)||(m_nShape[size_layer] == PAD_SHAPE_ROUNDEDRECTANGLE))
			{

				COLORREF color = nEDA_PCB_LayerColor[layer];

				Fpoint fpoint[9];
				if( m_fSizeX[size_layer] >= m_fSizeY[size_layer])
				{
					fpoint[0].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/2 + (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[0].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/2;				
					fpoint[1].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[1].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[2].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[2].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[3].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/2 + (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[3].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/2;
					fpoint[4].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/2 - (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[4].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/2;
					fpoint[5].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[5].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[6].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[6].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[7].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/2 - (m_fSizeY[size_layer]+2*expansion)/4;
					fpoint[7].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/2;

					for(int i=0; i<8; i++)
					{
						fpoint[i].fx -= m_fCx;
						fpoint[i].fy -= m_fCy;
					}

					double alpha, fr;
					for(int i=0; i<8; i++)
					{
						if((m_fRotation == 0)||(m_fRotation == 180)||(m_fRotation == 360))
						{
							fpoint[i].fx = m_fCx + fpoint[i].fx;
							fpoint[i].fy = m_fCy + fpoint[i].fy;
						}
						else if((m_fRotation == 90)||(m_fRotation == 270))
						{
							double temp;
							temp = fpoint[i].fx;
							fpoint[i].fx = m_fCx + fpoint[i].fy;
							fpoint[i].fy = m_fCy + temp;
						}
						else
						{
							alpha = atan2(fpoint[i].fy, fpoint[i].fx);
							fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
							fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
							fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
						}
					}
				}
				else
				{
					fpoint[0].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[0].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/2;
					fpoint[1].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[1].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/2 + (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[2].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[2].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/2 - (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[3].fx = m_fCx - (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[3].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/2;
					fpoint[4].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[4].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/2;
					fpoint[5].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[5].fy = m_fCy + (m_fSizeY[size_layer]+2*expansion)/2 - (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[6].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/2;
					fpoint[6].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/2 + (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[7].fx = m_fCx + (m_fSizeX[size_layer]+2*expansion)/4;
					fpoint[7].fy = m_fCy - (m_fSizeY[size_layer]+2*expansion)/2;

					for(int i=0; i<8; i++)
					{
						fpoint[i].fx -= m_fCx;
						fpoint[i].fy -= m_fCy;
					}

					double alpha, fr;
					for(int i=0; i<8; i++)
					{
						if((m_fRotation == 0)||(m_fRotation == 180)||(m_fRotation == 360))
						{
							fpoint[i].fx = m_fCx + fpoint[i].fx;
							fpoint[i].fy = m_fCy + fpoint[i].fy;
						}
						else if((m_fRotation == 90)||(m_fRotation == 270))
						{
							double temp;
							temp = fpoint[i].fx;
							fpoint[i].fx = m_fCx + fpoint[i].fy;
							fpoint[i].fy = m_fCy + temp;
						}
						else
						{
							alpha = atan2(fpoint[i].fy, fpoint[i].fx);
							fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
							fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
							fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
						}
					}
				}

				POINT point[9];
				for(int i=0; i<8; i++)
				{
					int x,y;
					x = (int)((fpoint[i].fx - origin_x)/scale);
					y = nEy - (int)((fpoint[i].fy - origin_y)/scale);	
					//pView->Get_VPort_XY(&x,&y,fpoint[i].fx, fpoint[i].fy);
					point[i].x = x;
					point[i].y = y;
				}
				point[8].x = point[0].x;
				point[8].y = point[0].y;

				CPen fillpen;
				fillpen.CreatePen(PS_SOLID,1,color);
				CPen* oldpen =  pDC->SelectObject(&fillpen);
			 
				CBrush fillbrush;
				fillbrush.CreateSolidBrush(color);
				CBrush* oldbrush = pDC->SelectObject(&fillbrush);		
				
				pDC->Polygon(point, 9);

				pDC->SelectObject(oldpen);
				pDC->SelectObject(oldbrush);
			}
			else
			{
				COLORREF color = nEDA_PCB_LayerColor[layer];

				CPen fillpen;
				fillpen.CreatePen(PS_SOLID,1,color);
				CPen* oldpen =  pDC->SelectObject(&fillpen);
			 
				CBrush fillbrush;
				fillbrush.CreateSolidBrush(color);
				CBrush* oldbrush = pDC->SelectObject(&fillbrush);

				if(m_fSizeX[size_layer] == m_fSizeY[size_layer])
				{
					double fr = m_fSizeX[size_layer]/2 + expansion;
					int cx,cy,r;
						
					//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
					cx = (int)((m_fCx - origin_x)/scale);
					cy = nEy - (int)((m_fCy - origin_y)/scale);	
					r = (int)(fr/scale);

					pDC->Ellipse(cx-r, cy-r, cx+r, cy+r);

				}
				else if(m_fSizeX[size_layer] > m_fSizeY[size_layer])
				{
					double fcx, fcy;
					int cx, cy, r;
					double fr = m_fSizeY[size_layer] + expansion*2;

					r = (int)(fr/scale);

					CPen pen;
					pen.CreatePen(PS_SOLID, r, color);
					CPen* oldpen =  pDC->SelectObject(&pen);

					fcx = m_fCx - cos(m_fRotation*fEDA_pi/180)*(m_fSizeX[size_layer] - m_fSizeY[size_layer])/2;
					fcy = m_fCy - sin(m_fRotation*fEDA_pi/180)*(m_fSizeX[size_layer] - m_fSizeY[size_layer])/2;
					//pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
					cx = (int)((fcx - origin_x)/scale);
					cy = nEy - (int)((fcy - origin_y)/scale);	
			
					pDC->MoveTo(cx, cy);

					fcx = m_fCx + cos(m_fRotation*fEDA_pi/180)*(m_fSizeX[size_layer] - m_fSizeY[size_layer])/2;
					fcy = m_fCy + sin(m_fRotation*fEDA_pi/180)*(m_fSizeX[size_layer] - m_fSizeY[size_layer])/2;
								
					//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
					cx = (int)((fcx - origin_x)/scale);
					cy = nEy - (int)((fcy - origin_y)/scale);	

					pDC->LineTo(cx, cy);

					pDC->SelectObject(oldpen);
				}
				else
				{
					double fcx, fcy;
					int cx, cy, r;
					double fr = m_fSizeX[size_layer] + expansion*2;

					r = (int)(fr/scale);

					CPen pen;
					pen.CreatePen(PS_SOLID, r, color);
					CPen* oldpen =  pDC->SelectObject(&pen);

					fcx = m_fCx - sin(m_fRotation*fEDA_pi/180)*(m_fSizeY[size_layer] - m_fSizeX[size_layer])/2;
					fcy = m_fCy + cos(m_fRotation*fEDA_pi/180)*(m_fSizeY[size_layer] - m_fSizeX[size_layer])/2;

					//pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
					cx = (int)((fcx - origin_x)/scale);
					cy = nEy - (int)((fcy - origin_y)/scale);	

					pDC->MoveTo(cx, cy);

					fcx = m_fCx + sin(m_fRotation*fEDA_pi/180)*(m_fSizeY[size_layer] - m_fSizeX[size_layer])/2;
					fcy = m_fCy - cos(m_fRotation*fEDA_pi/180)*(m_fSizeY[size_layer] - m_fSizeX[size_layer])/2;
								
					//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
					cx = (int)((fcx - origin_x)/scale);
					cy = nEy - (int)((fcy - origin_y)/scale);	

					pDC->LineTo(cx, cy);

					pDC->SelectObject(oldpen);
				}
				
				pDC->SelectObject(oldpen);
				pDC->SelectObject(oldbrush);
			}
		}
	}
	else if((layer >= POWERPLANE1)&&(layer <= POWERPLANE16))
	{
		if(m_nLayer == MULTILAYER)
		{
			COLORREF color = nEDA_PCB_LayerColor[layer];
			
			int cx,cy;

			cx = (int)((m_fCx - origin_x)/scale);
			cy = nEy - (int)((m_fCy - origin_y)/scale);	

				
			if((m_nCCS != NO_CONNECT)&&(m_nNet >= 0)&&(ConnectToInternalPlane(layer) == TRUE))	
			{
				if(m_nCCS == RELIEF_CONNECT)
				{

					double fr = (m_fHoleDia + m_fCAG)/2 + m_fCPR;
					double alpha = asin((m_fCCW/2 + m_fCAG/2)/fr);
							
					int r = (int)(fr/scale);
						
					int width = (int)(m_fCAG/scale);
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
				else if(m_nCCS == DIRECT_CONNECT)	//Direct Connect
				{
				}
			}
			else
			{
				double clearance = m_fCPC;
				double expansion = clearance - (m_fSizeX[TOPLAYER] - m_fHoleDia)/2;

				CPen fillpen;
				fillpen.CreatePen(PS_SOLID,1,color);
				CPen* oldpen =  pDC->SelectObject(&fillpen);
				 
				CBrush fillbrush;
				fillbrush.CreateSolidBrush(color);
				CBrush* oldbrush = pDC->SelectObject(&fillbrush);

				double fr = m_fSizeX[TOPLAYER]/2 + expansion;
				int cx,cy,r;
							
				//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
				cx = (int)((m_fCx - origin_x)/scale);
				cy = nEy - (int)((m_fCy - origin_y)/scale);	
				r = (int)(fr/scale);

				pDC->Ellipse(cx-r, cy-r, cx+r, cy+r);
			}
		}
	}
	else if((m_nLayer == MULTILAYER)||(m_nLayer == layer))
	{
		int size_layer;
		if(m_nLayer != MULTILAYER) size_layer = TOPLAYER;
		else
		{
			if(m_nPadMode == PAD_MODE_SIMPLE) size_layer = TOPLAYER;
			else
			{
				if(layer == BOTTOMLAYER) size_layer = BOTTOMLAYER;
				else if( ( layer > TOPLAYER)&&(layer < BOTTOMLAYER) ) size_layer = MIDLAYER1;
				else size_layer = TOPLAYER;
			}
		}

		if(m_nShape[size_layer] == PAD_SHAPE_RECTANGLE)
		{
			COb_pcbfill pcbfill;
			pcbfill.m_nLayer = m_nLayer;
			pcbfill.m_fX1 = m_fCx - m_fSizeX[size_layer]/2;
			pcbfill.m_fY1 = m_fCy - m_fSizeY[size_layer]/2;
			pcbfill.m_fX2 = m_fCx + m_fSizeX[size_layer]/2;
			pcbfill.m_fY2 = m_fCy + m_fSizeY[size_layer]/2;
			pcbfill.m_fRotation = m_fRotation;
			pcbfill.m_nFlag = m_nFlag;
			pcbfill.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		}
		else if((m_nShape[size_layer] == PAD_SHAPE_OCTAGONAL)||(m_nShape[size_layer] == PAD_SHAPE_ROUNDEDRECTANGLE))
		{
			COLORREF color = nEDA_PCB_LayerColor[m_nLayer];

			Fpoint fpoint[9];
			if( m_fSizeX[size_layer] >= m_fSizeY[size_layer])
			{
				fpoint[0].fx = m_fCx - m_fSizeX[size_layer]/2 + m_fSizeY[size_layer]/4;
				fpoint[0].fy = m_fCy - m_fSizeY[size_layer]/2;				
				fpoint[1].fx = m_fCx - m_fSizeX[size_layer]/2;
				fpoint[1].fy = m_fCy - m_fSizeY[size_layer]/4;
				fpoint[2].fx = m_fCx - m_fSizeX[size_layer]/2;
				fpoint[2].fy = m_fCy + m_fSizeY[size_layer]/4;
				fpoint[3].fx = m_fCx - m_fSizeX[size_layer]/2 + m_fSizeY[size_layer]/4;
				fpoint[3].fy = m_fCy + m_fSizeY[size_layer]/2;
				fpoint[4].fx = m_fCx + m_fSizeX[size_layer]/2 - m_fSizeY[size_layer]/4;
				fpoint[4].fy = m_fCy + m_fSizeY[size_layer]/2;
				fpoint[5].fx = m_fCx + m_fSizeX[size_layer]/2;
				fpoint[5].fy = m_fCy + m_fSizeY[size_layer]/4;
				fpoint[6].fx = m_fCx + m_fSizeX[size_layer]/2;
				fpoint[6].fy = m_fCy - m_fSizeY[size_layer]/4;
				fpoint[7].fx = m_fCx + m_fSizeX[size_layer]/2 - m_fSizeY[size_layer]/4;
				fpoint[7].fy = m_fCy - m_fSizeY[size_layer]/2;

				for(int i=0; i<8; i++)
				{
					fpoint[i].fx -= m_fCx;
					fpoint[i].fy -= m_fCy;
				}

				double alpha, fr;
				for(int i=0; i<8; i++)
				{
					if((m_fRotation == 0)||(m_fRotation == 180)||(m_fRotation == 360))
					{
						fpoint[i].fx = m_fCx + fpoint[i].fx;
						fpoint[i].fy = m_fCy + fpoint[i].fy;
					}
					else if((m_fRotation == 90)||(m_fRotation == 270))
					{
						double temp;
						temp = fpoint[i].fx;
						fpoint[i].fx = m_fCx + fpoint[i].fy;
						fpoint[i].fy = m_fCy + temp;
					}
					else
					{
						alpha = atan2(fpoint[i].fy, fpoint[i].fx);
						fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
						fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
						fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
					}
				}
			}
			else
			{
				fpoint[0].fx = m_fCx - m_fSizeX[size_layer]/4;
				fpoint[0].fy = m_fCy - m_fSizeY[size_layer]/2;
				fpoint[1].fx = m_fCx - m_fSizeX[size_layer]/2;
				fpoint[1].fy = m_fCy - m_fSizeY[size_layer]/2 + m_fSizeX[size_layer]/4;
				fpoint[2].fx = m_fCx - m_fSizeX[size_layer]/2;
				fpoint[2].fy = m_fCy + m_fSizeY[size_layer]/2 - m_fSizeX[size_layer]/4;
				fpoint[3].fx = m_fCx - m_fSizeX[size_layer]/4;
				fpoint[3].fy = m_fCy + m_fSizeY[size_layer]/2;
				fpoint[4].fx = m_fCx + m_fSizeX[size_layer]/4;
				fpoint[4].fy = m_fCy + m_fSizeY[size_layer]/2;
				fpoint[5].fx = m_fCx + m_fSizeX[size_layer]/2;
				fpoint[5].fy = m_fCy + m_fSizeY[size_layer]/2 - m_fSizeX[size_layer]/4;
				fpoint[6].fx = m_fCx + m_fSizeX[size_layer]/2;
				fpoint[6].fy = m_fCy - m_fSizeY[size_layer]/2 + m_fSizeX[size_layer]/4;
				fpoint[7].fx = m_fCx + m_fSizeX[size_layer]/4;
				fpoint[7].fy = m_fCy - m_fSizeY[size_layer]/2;

				for(int i=0; i<8; i++)
				{
					fpoint[i].fx -= m_fCx;
					fpoint[i].fy -= m_fCy;
				}

				double alpha, fr;
				for(int i=0; i<8; i++)
				{
					if((m_fRotation == 0)||(m_fRotation == 180)||(m_fRotation == 360))
					{
						fpoint[i].fx = m_fCx + fpoint[i].fx;
						fpoint[i].fy = m_fCy + fpoint[i].fy;
					}
					else if((m_fRotation == 90)||(m_fRotation == 270))
					{
						double temp;
						temp = fpoint[i].fx;
						fpoint[i].fx = m_fCx + fpoint[i].fy;
						fpoint[i].fy = m_fCy + temp;
					}
					else
					{
						alpha = atan2(fpoint[i].fy, fpoint[i].fx);
						fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
						fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
						fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
					}
				}
			}

			POINT point[9];
			for(int i=0; i<8; i++)
			{
				int x,y;
				x = (int)((fpoint[i].fx - origin_x)/scale);
				y = nEy - (int)((fpoint[i].fy - origin_y)/scale);	
				//pView->Get_VPort_XY(&x,&y,fpoint[i].fx, fpoint[i].fy);
				point[i].x = x;
				point[i].y = y;
			}
			point[8].x = point[0].x;
			point[8].y = point[0].y;

			CPen fillpen;
			fillpen.CreatePen(PS_SOLID,1,color);
			CPen* oldpen =  pDC->SelectObject(&fillpen);
		 
			CBrush fillbrush;
			fillbrush.CreateSolidBrush(color);
			CBrush* oldbrush = pDC->SelectObject(&fillbrush);		
			
			pDC->Polygon(point, 9);

			pDC->SelectObject(oldpen);
			pDC->SelectObject(oldbrush);
		}
		else 
		{
			COLORREF color = nEDA_PCB_LayerColor[m_nLayer];

			CPen fillpen;
			fillpen.CreatePen(PS_SOLID,1,color);
			CPen* oldpen =  pDC->SelectObject(&fillpen);
		 
			CBrush fillbrush;
			fillbrush.CreateSolidBrush(color);
			CBrush* oldbrush = pDC->SelectObject(&fillbrush);

			if(m_fSizeX[size_layer] == m_fSizeY[size_layer])
			{
				int cx,cy,r;
					
				//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
				cx = (int)((m_fCx - origin_x)/scale);
				cy = nEy - (int)((m_fCy - origin_y)/scale);	
				r = (int)(m_fSizeX[size_layer]/2/scale);

				pDC->Ellipse(cx-r, cy-r, cx+r, cy+r);

			}
			else if(m_fSizeX[size_layer] > m_fSizeY[size_layer])
			{
				double fcx, fcy;
				int cx, cy, r;

				r = (int)(m_fSizeY[size_layer]/scale);

				CPen pen;
				pen.CreatePen(PS_SOLID, r, color);
				CPen* oldpen =  pDC->SelectObject(&pen);

				fcx = m_fCx - cos(m_fRotation*fEDA_pi/180)*(m_fSizeX[size_layer] - m_fSizeY[size_layer])/2;
				fcy = m_fCy - sin(m_fRotation*fEDA_pi/180)*(m_fSizeX[size_layer] - m_fSizeY[size_layer])/2;
				//pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
				cx = (int)((fcx - origin_x)/scale);
				cy = nEy - (int)((fcy - origin_y)/scale);	
		
				pDC->MoveTo(cx, cy);

				fcx = m_fCx + cos(m_fRotation*fEDA_pi/180)*(m_fSizeX[size_layer] - m_fSizeY[size_layer])/2;
				fcy = m_fCy + sin(m_fRotation*fEDA_pi/180)*(m_fSizeX[size_layer] - m_fSizeY[size_layer])/2;
							
				//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
				cx = (int)((fcx - origin_x)/scale);
				cy = nEy - (int)((fcy - origin_y)/scale);	

				pDC->LineTo(cx, cy);

				pDC->SelectObject(oldpen);
			}
			else
			{
				double fcx, fcy;
				int cx, cy, r;

				r = (int)(m_fSizeX[size_layer]/scale);

				CPen pen;
				pen.CreatePen(PS_SOLID, r, color);
				CPen* oldpen =  pDC->SelectObject(&pen);

				fcx = m_fCx - sin(m_fRotation*fEDA_pi/180)*(m_fSizeY[size_layer] - m_fSizeX[size_layer])/2;
				fcy = m_fCy + cos(m_fRotation*fEDA_pi/180)*(m_fSizeY[size_layer] - m_fSizeX[size_layer])/2;

				//pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
				cx = (int)((fcx - origin_x)/scale);
				cy = nEy - (int)((fcy - origin_y)/scale);	

				pDC->MoveTo(cx, cy);

				fcx = m_fCx + sin(m_fRotation*fEDA_pi/180)*(m_fSizeY[size_layer] - m_fSizeX[size_layer])/2;
				fcy = m_fCy - cos(m_fRotation*fEDA_pi/180)*(m_fSizeY[size_layer] - m_fSizeX[size_layer])/2;
							
				//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
				cx = (int)((fcx - origin_x)/scale);
				cy = nEy - (int)((fcy - origin_y)/scale);	

				pDC->LineTo(cx, cy);

				pDC->SelectObject(oldpen);
			}
			
			pDC->SelectObject(oldpen);
			pDC->SelectObject(oldbrush);
		}
	}


	
	int cx,cy;
	cx = (int)((m_fCx - origin_x)/scale);
	cy = nEy - (int)((m_fCy - origin_y)/scale);	

	COLORREF color = nEDA_PCB_LayerColor[PADHOLES];

	CPen holepen, *oldpen;
	holepen.CreatePen(PS_SOLID,1,color);
	oldpen = pDC->SelectObject(&holepen);
			 
	CBrush holebrush, *oldbrush;
	holebrush.CreateSolidBrush(color);
	oldbrush = pDC->SelectObject(&holebrush);

	if(m_nHoleType == PADHOLE_ROUND)
	{
		int r = (int)(m_fHoleDia/scale);

		RECT rect;
		rect.left = cx - r/2;
		rect.right = cx + r/2;
		rect.top = cy - r/2;
		rect.bottom = cy + r/2;

		pDC->Ellipse(&rect);
	}
	else if(m_nHoleType == PADHOLE_SQUARE)
	{
		COb_pcbfill pcbfill;
		pcbfill.m_nLayer = PADHOLES;
		pcbfill.m_fX1 = m_fCx - m_fHoleDia/2;
		pcbfill.m_fY1 = m_fCy - m_fHoleDia/2;
		pcbfill.m_fX2 = m_fCx + m_fHoleDia/2;
		pcbfill.m_fY2 = m_fCy + m_fHoleDia/2;
		pcbfill.m_fRotation = m_fHoleRotation;

		double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
		pcbfill.GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	

		POINT point[5];
		int x,y;
		//pView->Get_VPort_XY(&x,&y,fx1,fy1);
		x = (int)((fx1 - origin_x)/scale);
		y = nEy - (int)((fy1 - origin_y)/scale);	
		point[0].x = x; 
		point[0].y = y;
		point[4].x = x; 
		point[4].y = y;
			
		//pView->Get_VPort_XY(&x,&y,fx3,fy3);
		x = (int)((fx3 - origin_x)/scale);
		y = nEy - (int)((fy3 - origin_y)/scale);	
		point[1].x = x;
		point[1].y = y;
			
		//pView->Get_VPort_XY(&x,&y,fx2,fy2);
		x = (int)((fx2 - origin_x)/scale);
		y = nEy - (int)((fy2 - origin_y)/scale);	
		point[2].x = x;
		point[2].y = y;
			
		//pView->Get_VPort_XY(&x,&y,fx4,fy4);
		x = (int)((fx4 - origin_x)/scale);
		y = nEy - (int)((fy4 - origin_y)/scale);
		point[3].x = x;
		point[3].y = y;

		pDC->Polygon(point, 5);
	}
	else if(m_nHoleType == PADHOLE_SLOT)
	{
		if(m_fHoleWidth == m_fHoleDia)
		{
			double fcx, fcy, fr;
			int cx,cy,r;
			fcx = m_fCx;
			fcy = m_fCy;
			fr = m_fHoleDia/2;
					
			//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
			cx = (int)((fcx - origin_x)/scale);
			cy = nEy - (int)((fcy - origin_y)/scale);	
			r = (int)(fr/scale);

			pDC->Ellipse(cx-r, cy-r, cx+r, cy+r);
		}
		else if(m_fHoleWidth > m_fHoleDia)
		{
			double fcx, fcy, fr;
			int cx, cy, r;

			fcx = m_fCx - cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
			fcy = m_fCy - sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
			fr = m_fHoleDia;
					
			//pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
			cx = (int)((fcx - origin_x)/scale);
			cy = nEy - (int)((fcy - origin_y)/scale);	
			r = (int)(fr/scale);

			pDC->MoveTo(cx, cy);
			
			fcx = m_fCx + cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
			fcy = m_fCy + sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
							
			//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
			cx = (int)((fcx - origin_x)/scale);
			cy = nEy - (int)((fcy - origin_y)/scale);	
			r = (int)(fr/scale);
				
			CPen pen;
			pen.CreatePen(PS_SOLID, r, color);
			CPen* oldpen =  pDC->SelectObject(&pen);
					
			pDC->LineTo(cx, cy);
			pDC->SelectObject(oldpen);
		}
		else
		{
			double fcx, fcy, fr;
			int cx, cy, r;
			
			fcx = m_fCx - sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
			fcy = m_fCy + cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
			fr = m_fHoleWidth;
							
			//pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
			cx = (int)((fcx - origin_x)/scale);
			cy = nEy - (int)((fcy - origin_y)/scale);	
			r = (int)(fr/scale);

			pDC->MoveTo(cx, cy);
				
			fcx = m_fCx + sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
			fcy = m_fCy - cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
			fr = m_fHoleWidth;
						
			//pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
			cx = (int)((fcx - origin_x)/scale);
			cy = nEy - (int)((fcy - origin_y)/scale);	
			r = (int)(fr/scale);

			CPen pen;
			pen.CreatePen(PS_SOLID, r, color);
			CPen* oldpen =  pDC->SelectObject(&pen);
				
			pDC->LineTo(cx, cy);
			pDC->SelectObject(oldpen);
		}
	}

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);

	int height = 12;

	color = RGB(255,255,255);
			
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
	CSize size;
			
	text = m_csName;
	size = pDC->GetTextExtent(text);
	x = cx - size.cx/2;
	y = cy - size.cy/2;
	pDC->TextOut(x, y, text, strlen(text));


	pDC->SelectObject(oldfont);
		
	pDC->SetTextColor(oldtextcolor);
	pDC->SetBkMode(bkmode);

}

void COb_pcbpad::DrawSelection(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pDocument;	
	
	if(IsVisible() == FALSE) return;

	if(m_nShape[TOPLAYER] == PAD_SHAPE_RECTANGLE)	DrawSelectedPadRectangle(pDC, pV, TOPLAYER, SELECTIONS, 0);
	else if(m_nShape[TOPLAYER] == PAD_SHAPE_OCTAGONAL)	DrawSelectedPadOctagonal(pDC, pV, TOPLAYER, SELECTIONS, 0);
	else DrawSelectedPadRound(pDC, pV, TOPLAYER, SELECTIONS, 0);
}

void COb_pcbpad::DrawSelectedPadRectangle(CDC* pDC, CView* pV, int layer, int color_layer, double expansion)
{
	COb_pcbfill pcbfill;
	pcbfill.m_nLayer = SELECTIONS;
	pcbfill.m_fX1 = m_fCx - m_fSizeX[layer]/2 - expansion;
	pcbfill.m_fY1 = m_fCy - m_fSizeY[layer]/2 - expansion;
	pcbfill.m_fX2 = m_fCx + m_fSizeX[layer]/2 + expansion;
	pcbfill.m_fY2 = m_fCy + m_fSizeY[layer]/2 + expansion;
	pcbfill.m_fRotation = m_fRotation;
	pcbfill.m_nFlag = m_nFlag;
	pcbfill.DrawSelectionRect(pDC, pV);
}

void COb_pcbpad::DrawSelectedPadOctagonal(CDC* pDC, CView* pV, int layer, int color_layer, double expansion)
{
	CESDView* pView = (CESDView*)pV;

	COLORREF color = nEDA_PCB_LayerColor[color_layer];

	Fpoint fpoint[9];
	if( m_fSizeX[layer] >= m_fSizeY[layer])
	{
		fpoint[0].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/2 + (m_fSizeY[layer]+2*expansion)/4;
		fpoint[0].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/2;				
		fpoint[1].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/2;
		fpoint[1].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/4;
		fpoint[2].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/2;
		fpoint[2].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/4;
		fpoint[3].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/2 + (m_fSizeY[layer]+2*expansion)/4;
		fpoint[3].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/2;
		fpoint[4].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/2 - (m_fSizeY[layer]+2*expansion)/4;
		fpoint[4].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/2;
		fpoint[5].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/2;
		fpoint[5].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/4;
		fpoint[6].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/2;
		fpoint[6].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/4;
		fpoint[7].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/2 - (m_fSizeY[layer]+2*expansion)/4;
		fpoint[7].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/2;

		for(int i=0; i<8; i++)
		{
			fpoint[i].fx -= m_fCx;
			fpoint[i].fy -= m_fCy;
		}

		double alpha, fr;
		for(int i=0; i<8; i++)
		{
			if((m_fRotation == 0)||(m_fRotation == 180)||(m_fRotation == 360))
			{
				fpoint[i].fx = m_fCx + fpoint[i].fx;
				fpoint[i].fy = m_fCy + fpoint[i].fy;
			}
			else if((m_fRotation == 90)||(m_fRotation == 270))
			{
				double temp;
				temp = fpoint[i].fx;
				fpoint[i].fx = m_fCx + fpoint[i].fy;
				fpoint[i].fy = m_fCy + temp;
			}
			else
			{
				alpha = atan2(fpoint[i].fy, fpoint[i].fx);
				fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
				fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
				fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
			}
		}
	}
	else
	{
		fpoint[0].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/4;
		fpoint[0].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/2;
		fpoint[1].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/2;
		fpoint[1].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/2 + (m_fSizeX[layer]+2*expansion)/4;
		fpoint[2].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/2;
		fpoint[2].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/2 - (m_fSizeX[layer]+2*expansion)/4;
		fpoint[3].fx = m_fCx - (m_fSizeX[layer]+2*expansion)/4;
		fpoint[3].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/2;
		fpoint[4].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/4;
		fpoint[4].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/2;
		fpoint[5].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/2;
		fpoint[5].fy = m_fCy + (m_fSizeY[layer]+2*expansion)/2 - (m_fSizeX[layer]+2*expansion)/4;
		fpoint[6].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/2;
		fpoint[6].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/2 + (m_fSizeX[layer]+2*expansion)/4;
		fpoint[7].fx = m_fCx + (m_fSizeX[layer]+2*expansion)/4;
		fpoint[7].fy = m_fCy - (m_fSizeY[layer]+2*expansion)/2;

		for(int i=0; i<8; i++)
		{
			fpoint[i].fx -= m_fCx;
			fpoint[i].fy -= m_fCy;
		}

		double alpha, fr;
		for(int i=0; i<8; i++)
		{
			if((m_fRotation == 0)||(m_fRotation == 180)||(m_fRotation == 360))
			{
				fpoint[i].fx = m_fCx + fpoint[i].fx;
				fpoint[i].fy = m_fCy + fpoint[i].fy;
			}
			else if((m_fRotation == 90)||(m_fRotation == 270))
			{
				double temp;
				temp = fpoint[i].fx;
				fpoint[i].fx = m_fCx + fpoint[i].fy;
				fpoint[i].fy = m_fCy + temp;
			}
			else
			{
				alpha = atan2(fpoint[i].fy, fpoint[i].fx);
				fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
				fpoint[i].fx = m_fCx + fr*cos(alpha + m_fRotation*fEDA_pi/180);
				fpoint[i].fy = m_fCy + fr*sin(alpha + m_fRotation*fEDA_pi/180);
			}
		}
	}

	POINT point[9];
	for(int i=0; i<8; i++)
	{
		int x,y;
		pView->Get_VPort_XY(&x,&y,fpoint[i].fx, fpoint[i].fy);
		if(pView->m_nPrint_Flag == 1)
		{
			x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
			y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
		}
		point[i].x = x;
		point[i].y = y;
	}
	point[8].x = point[0].x;
	point[8].y = point[0].y;

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

	int mode = pDC->SetROP2(R2_MERGEPEN );
	pDC->Polygon(point, 9);
	pDC->SetROP2(mode);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void COb_pcbpad::DrawSelectedPadRound(CDC* pDC, CView* pV, int layer, int color_layer, double expansion)
{
	COLORREF color = nEDA_PCB_LayerColor[color_layer];
	CESDView* pView = (CESDView*)pV;

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

	if(m_fSizeX[layer] == m_fSizeY[layer])
	{
		double fcx, fcy, fr;
		int cx,cy,r;
		fcx = m_fCx;
		fcy = m_fCy;
		fr = m_fSizeX[layer]/2 + expansion;
			
		pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
		r = (int)(fr/pView->m_fScale);

		int mode = pDC->SetROP2(R2_MERGEPEN );
		pDC->Ellipse(cx-r, cy-r, cx+r, cy+r);
		pDC->SetROP2(mode);
	}
	else if(m_fSizeX[layer] > m_fSizeY[layer])
	{
		double fcx, fcy, fr;
		int cx, cy, r;
		
		fr = m_fSizeY[layer]/2 + expansion*2;
		r = (int)(fr/pView->m_fScale);
		
		fcx = m_fCx - cos(m_fRotation*fEDA_pi/180)*(m_fSizeX[layer] - m_fSizeY[layer])/2;
		fcy = m_fCy - sin(m_fRotation*fEDA_pi/180)*(m_fSizeX[layer] - m_fSizeY[layer])/2;
		pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
	
		CRgn  rgnA, rgnB, rgnC;
		rgnA.CreateEllipticRgn(cx-r, cy-r, cx+r, cy+r);

		fcx = m_fCx + cos(m_fRotation*fEDA_pi/180)*(m_fSizeX[layer] - m_fSizeY[layer])/2;
		fcy = m_fCy + sin(m_fRotation*fEDA_pi/180)*(m_fSizeX[layer] - m_fSizeY[layer])/2;
		pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
		rgnB.CreateEllipticRgn(cx-r, cy-r, cx+r, cy+r);

		COb_pcbfill pcbfill;
		pcbfill.m_fRotation = m_fRotation;
		pcbfill.m_fX1 = m_fCx - (m_fSizeX[layer] - m_fSizeY[layer])/2;
		pcbfill.m_fY1 = m_fCy - m_fSizeY[layer]/2;
		pcbfill.m_fX2 = m_fCx + (m_fSizeX[layer] - m_fSizeY[layer])/2;
		pcbfill.m_fY2 = m_fCy + m_fSizeY[layer]/2;

		double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
		pcbfill.GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);
	
		POINT point[5];
		int x,y;
		pView->Get_VPort_XY(&x,&y,fx1,fy1);
		point[0].x = x; point[0].y = y;
		point[4].x = x; point[4].y = y;
	
		pView->Get_VPort_XY(&x,&y,fx3,fy3);
		point[1].x = x;	point[1].y = y;
	
		pView->Get_VPort_XY(&x,&y,fx2,fy2);
		point[2].x = x;	point[2].y = y;
	
		pView->Get_VPort_XY(&x,&y,fx4,fy4);
		point[3].x = x; point[3].y = y;

		rgnC.CreatePolygonRgn(point, 5, ALTERNATE);

		CRgn temprgn;
		temprgn.CreateRectRgn( 0, 0, 50, 50 );

		temprgn.CombineRgn(&rgnA, &rgnB, RGN_OR);
		temprgn.CombineRgn(&rgnC, &temprgn, RGN_OR);

		int mode = pDC->SetROP2(R2_MERGEPEN );
		pDC->FillRgn(&temprgn, &fillbrush);
		pDC->SetROP2(mode);
	}
	else
	{
		double fcx, fcy, fr;
		int cx, cy, r;

		fr = m_fSizeX[layer]/2 + expansion*2;
		r = (int)(fr/pView->m_fScale);

		fcx = m_fCx - sin(m_fRotation*fEDA_pi/180)*(m_fSizeY[layer] - m_fSizeX[layer])/2;
		fcy = m_fCy + cos(m_fRotation*fEDA_pi/180)*(m_fSizeY[layer] - m_fSizeX[layer])/2;
		pView->Get_VPort_XY(&cx,&cy,fcx,fcy);

		CRgn  rgnA, rgnB, rgnC;
		rgnA.CreateEllipticRgn(cx-r, cy-r, cx+r, cy+r);

		fcx = m_fCx + sin(m_fRotation*fEDA_pi/180)*(m_fSizeY[layer] - m_fSizeX[layer])/2;
		fcy = m_fCy - cos(m_fRotation*fEDA_pi/180)*(m_fSizeY[layer] - m_fSizeX[layer])/2;
		pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
		rgnB.CreateEllipticRgn(cx-r, cy-r, cx+r, cy+r);
		
		
		COb_pcbfill pcbfill;
		pcbfill.m_fRotation = m_fRotation;
		pcbfill.m_fX1 = m_fCx - m_fSizeX[layer]/2;
		pcbfill.m_fY1 = m_fCy - (m_fSizeY[layer] - m_fSizeX[layer])/2;
		pcbfill.m_fX2 = m_fCx + m_fSizeX[layer]/2;
		pcbfill.m_fY2 = m_fCy + (m_fSizeY[layer] - m_fSizeX[layer])/2;

		double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
		pcbfill.GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);
	
		POINT point[5];
		int x,y;
		pView->Get_VPort_XY(&x,&y,fx1,fy1);
		point[0].x = x; point[0].y = y;
		point[4].x = x; point[4].y = y;
	
		pView->Get_VPort_XY(&x,&y,fx3,fy3);
		point[1].x = x;	point[1].y = y;
	
		pView->Get_VPort_XY(&x,&y,fx2,fy2);
		point[2].x = x;	point[2].y = y;
	
		pView->Get_VPort_XY(&x,&y,fx4,fy4);
		point[3].x = x; point[3].y = y;

		rgnC.CreatePolygonRgn(point, 5, ALTERNATE);

		CRgn temprgn;
		temprgn.CreateRectRgn( 0, 0, 50, 50 );

		temprgn.CombineRgn(&rgnA, &rgnB, RGN_OR);
		temprgn.CombineRgn(&rgnC, &temprgn, RGN_OR);

		int mode = pDC->SetROP2(R2_MERGEPEN );
		pDC->FillRgn(&temprgn, &fillbrush);
		pDC->SetROP2(mode);
	}
	
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void COb_pcbpad::DrawHighlighted(CDC* pDC, CView* pV, int layer)
{
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(layer == CONNECTIONS) layer = SELECTIONS;

	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE)
	{
		if(m_nLayer == MULTILAYER)
		{
			int i;
			for(i=0; i<SIGNAL_LAYER_NUM; i++)
			{
				if(pDoc->m_sPCB_LayerParam[i].bLayerShow == TRUE)  break;
			}
			if(i >= SIGNAL_LAYER_NUM) return;
		}
		else return;
	}

	if((m_nLayer != MULTILAYER)||(m_nPadMode == PAD_MODE_SIMPLE))
	{
		if(m_nShape[TOPLAYER] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, TOPLAYER, layer, 0);
		else if(m_nShape[TOPLAYER] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, TOPLAYER, layer, 0);
		else if(m_nShape[TOPLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE)	DrawPadRoundedRectangle(pDC, pV, TOPLAYER, layer, 0);
		else DrawPadRound(pDC, pV, TOPLAYER, layer, 0);
	}
	else if(m_nPadMode == PAD_MODE_T_M_B)
	{
		if(pDoc->LayerVisible(BOTTOMLAYER) == TRUE)
		{
			if(m_nShape[BOTTOMLAYER] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, BOTTOMLAYER, layer, 0);
			else if(m_nShape[BOTTOMLAYER] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, BOTTOMLAYER, layer, 0);
			else if(m_nShape[BOTTOMLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE) DrawPadRoundedRectangle(pDC, pV, BOTTOMLAYER, layer, 0);
			else DrawPadRound(pDC, pV, BOTTOMLAYER, layer, 0);
		}
		
		int midlayer;
		for(midlayer=MIDLAYER1; midlayer<=MIDLAYER30; midlayer++) 
		{
			if(pDoc->LayerVisible(midlayer) == TRUE) break;
		}
		if(midlayer<=MIDLAYER30)
		{
			if(m_nShape[MIDLAYER1] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, MIDLAYER1, layer, 0);
			else if(m_nShape[MIDLAYER1] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, MIDLAYER1, layer, 0);
			else if(m_nShape[MIDLAYER1] == PAD_SHAPE_ROUNDEDRECTANGLE) DrawPadRoundedRectangle(pDC, pV, MIDLAYER1, layer, 0);
			else DrawPadRound(pDC, pV, MIDLAYER1, layer, 0);
		}

		if(pDoc->LayerVisible(TOPLAYER) == TRUE)
		{
			if(m_nShape[TOPLAYER] == PAD_SHAPE_RECTANGLE)	DrawPadRectangle(pDC, pV, TOPLAYER, layer, 0);
			else if(m_nShape[TOPLAYER] == PAD_SHAPE_OCTAGONAL)	DrawPadOctagonal(pDC, pV, TOPLAYER, layer, 0);
			else if(m_nShape[TOPLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE) DrawPadRoundedRectangle(pDC, pV, TOPLAYER, layer, 0);
			else DrawPadRound(pDC, pV, TOPLAYER, layer, 0);
		}
	}
	else
	{
		
	}
	
	if((m_fHoleDia != 0)&&(pDoc->m_sPCB_LayerParam[PADHOLES].bLayerShow == TRUE))
	{
		int cx,cy;
		pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);

		COLORREF color;
		color = nEDA_PCB_LayerColor[PADHOLES];
			
		CPen holepen, *oldpen;
		holepen.CreatePen(PS_SOLID,1,color);
		oldpen = pDC->SelectObject(&holepen);
				 
		CBrush holebrush, *oldbrush;
		holebrush.CreateSolidBrush(color);
		oldbrush = pDC->SelectObject(&holebrush);

		if(m_nHoleType == PADHOLE_ROUND)
		{
			int r = (int)(m_fHoleDia/pView->m_fScale);
			RECT rect;
			rect.left = cx - r/2;
			rect.right = cx + r/2;
			rect.top = cy - r/2;
			rect.bottom = cy + r/2;
			pDC->Ellipse(&rect);
		}
		else if(m_nHoleType == PADHOLE_SQUARE)
		{
			COb_pcbfill pcbfill;
			pcbfill.m_nLayer = PADHOLES;
			pcbfill.m_fX1 = m_fCx - m_fHoleDia/2;
			pcbfill.m_fY1 = m_fCy - m_fHoleDia/2;
			pcbfill.m_fX2 = m_fCx + m_fHoleDia/2;
			pcbfill.m_fY2 = m_fCy + m_fHoleDia/2;
			pcbfill.m_fRotation = m_fHoleRotation;

			double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
			pcbfill.GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	

			POINT point[5];
			int x,y;
			pView->Get_VPort_XY(&x,&y,fx1,fy1);
			point[0].x = x; 
			point[0].y = y;
			point[4].x = x; 
			point[4].y = y;
				
			pView->Get_VPort_XY(&x,&y,fx3,fy3);
			point[1].x = x;
			point[1].y = y;
				
			pView->Get_VPort_XY(&x,&y,fx2,fy2);
			point[2].x = x;
			point[2].y = y;
				
			pView->Get_VPort_XY(&x,&y,fx4,fy4);
			point[3].x = x;
			point[3].y = y;

			pDC->Polygon(point, 5);
		}
		else if(m_nHoleType == PADHOLE_SLOT)
		{
			if(m_fHoleWidth == m_fHoleDia)
			{
				double fcx, fcy, fr;
				int cx,cy,r;
				fcx = m_fCx;
				fcy = m_fCy;
				fr = m_fHoleDia/2;
						
				pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
				r = (int)(fr/pView->m_fScale);
				pDC->Ellipse(cx-r, cy-r, cx+r, cy+r);
			}
			else if(m_fHoleWidth > m_fHoleDia)
			{
				double fcx, fcy, fr;
				int cx, cy, r;

				fcx = m_fCx - cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
				fcy = m_fCy - sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
				fr = m_fHoleDia;
							
				pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
				r = (int)(fr/pView->m_fScale);
				pDC->MoveTo(cx, cy);

				fcx = m_fCx + cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
				fcy = m_fCy + sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleWidth - m_fHoleDia)/2;
								
				pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
				r = (int)(fr/pView->m_fScale);

				CPen pen;
				pen.CreatePen(PS_SOLID, r, color);
				CPen* oldpen =  pDC->SelectObject(&pen);
						
				pDC->LineTo(cx, cy);
				pDC->SelectObject(oldpen);
			}
			else
			{
				double fcx, fcy, fr;
				int cx, cy, r;

				fcx = m_fCx - sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
				fcy = m_fCy + cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
				fr = m_fHoleWidth;
									
				pView->Get_VPort_XY(&cx,&cy,fcx,fcy);
				r = (int)(fr/pView->m_fScale);
				pDC->MoveTo(cx, cy);

				fcx = m_fCx + sin(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
				fcy = m_fCy - cos(m_fHoleRotation*fEDA_pi/180)*(m_fHoleDia - m_fHoleWidth)/2;
				fr = m_fHoleWidth;
								
				pView->Get_VPort_XY(&cx,&cy,fcx, fcy);
				r = (int)(fr/pView->m_fScale);

				CPen pen;
				pen.CreatePen(PS_SOLID, r, color);
				CPen* oldpen =  pDC->SelectObject(&pen);
					
				pDC->LineTo(cx, cy);
				pDC->SelectObject(oldpen);
			}
		}

		pDC->SelectObject(oldpen);
		pDC->SelectObject(oldbrush);
	}

	int cx,cy;
	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);

	int height;
	if(pView->m_fScale <= 2)
	{
		if(pView->m_fScale > 1.2) height = 10;
		else if(pView->m_fScale > 0.6) height = 14;
		else height = 20;
		if((MIN(m_fSizeX[TOPLAYER], m_fSizeY[TOPLAYER])/pView->m_fScale) < height) height = height/2;

		COLORREF color;
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
		CSize size;
			
		if(m_nNet >= 0)
		{
			COb_pcbnet* pcbnet = (COb_pcbnet*)pDoc->m_arrayPcbNet.GetAt(m_nNet);
			text = pcbnet->m_csNetName;
		}
		else text.Empty();			
			
		if(text.IsEmpty() == TRUE)
		{
			text = m_csName;
			size = pDC->GetTextExtent(text);
			x = cx - size.cx/2;
			y = cy - size.cy/2;
			pDC->TextOut(x, y, text, strlen(text));
		}
		else
		{
			size = pDC->GetTextExtent(text);
			x = cx - size.cx/2;
			y = cy;
			pDC->TextOut(x, y, text, strlen(text));
				
			text = m_csName;
			size = pDC->GetTextExtent(text);
			x = cx - size.cx/2;
			y = cy - size.cy;
			pDC->TextOut(x, y, text, strlen(text));
		}

		pDC->SelectObject(oldfont);
			
		pDC->SetTextColor(oldtextcolor);
		pDC->SetBkMode(bkmode);
	}
}

BOOL COb_pcbpad::TearDropExist(CView* pV)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	CESDView* pView = (CESDView*)pV;

	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_bTearDrop == FALSE)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;

		if((m_nLayer != MULTILAYER)&&(m_nLayer != ptrack->m_nLayer)) continue;

		if(pView->Line_Pad_Connect(ptrack, this, 0) == TRUE) return TRUE;
	}
	
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_bTearDrop == FALSE)||(parc->m_nComponent >= 0)||(parc->m_nPolygon >= 0)) continue;

		if((m_nLayer != MULTILAYER)&&(m_nLayer != parc->m_nLayer)) continue;

		if(pView->Pad_Arc_Connect(this, parc, 0) == TRUE) return TRUE;
	}
	
	return FALSE;
}

