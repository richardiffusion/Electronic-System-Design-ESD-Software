#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_PCBEditRegion.h"

IMPLEMENT_SERIAL(COb_pcbregion, CObject, 0)
COb_pcbregion::COb_pcbregion()
{
	m_cVertex.RemoveAll();
	m_arrayHoles.RemoveAll();
	//m_nHoleNum = 0;
	//for(int i=0; i<500; i++) m_cHoles[i].RemoveAll();

	m_nNet = -1;
	m_nComponent = -1;
	m_nPolygon = -1;

	m_bLocked = FALSE;
	m_bKeepout = FALSE;
	m_bPolygonCutout = FALSE;
	m_bBoardCutout = FALSE;

	//------ 下面参数的意义有待明确
	//m_nIndexForSave = -1;
	m_bPolygonOutline = FALSE;
	m_bUserRouted = TRUE;
	m_nUnionIndex = -1;
	m_nSubPolyIndex = -1;

	m_bSelection = FALSE;	
	m_nFlag = 0;
}

COb_pcbregion::~COb_pcbregion()
{
	while(!m_arrayHoles.IsEmpty())
	{
		delete m_arrayHoles.GetAt(0);
		m_arrayHoles.RemoveAt(0);
	}
}

void COb_pcbregion::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
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


		num = m_arrayHoles.GetCount();
		ar << num;
		for(int i=0; i<num; i++)
		{
			CArray<Fpoint, Fpoint>* p_arrayHole = m_arrayHoles.GetAt(i);
			int num_hole = p_arrayHole->GetCount();
			ar << num_hole;
			for(int j=0; j<num_hole; j++)
			{
				Fpoint point;
				point = p_arrayHole->GetAt(j);
				ar << point.fx;
				ar << point.fy;
			}
		}	

		ar << m_nLayer;
		ar << m_nNet;
		ar << m_nComponent;
		ar << m_nPolygon;

		ar << m_bLocked;
		ar << m_bKeepout;
		ar << m_bPolygonCutout;
		ar << m_bBoardCutout;

		ar << m_bPolygonOutline;
		ar << m_bUserRouted;
		ar << m_nUnionIndex;
		ar << m_nSubPolyIndex;

		ar << m_bSelection;	
	}
	else
	{
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

		ar >> num;
		for(int i=0; i<num; i++)
		{
			int num_hole;
			CArray<Fpoint, Fpoint>* p_arrayHole = new CArray<Fpoint, Fpoint>;
			ar >> num_hole;
			for(int j=0; j<num_hole; j++)
			{
				Fpoint point;
				ar >> point.fx;
				ar >> point.fy;
				p_arrayHole->Add(point);
			}
			m_arrayHoles.Add(p_arrayHole);
		}	
		
		ar >> m_nLayer;
		ar >> m_nNet;
		ar >> m_nComponent;
		ar >> m_nPolygon;

		ar >> m_bLocked;
		ar >> m_bKeepout;
		ar >> m_bPolygonCutout;
		ar >> m_bBoardCutout;

		ar >> m_bPolygonOutline;
		ar >> m_bUserRouted;
		ar >> m_nUnionIndex;
		ar >> m_nSubPolyIndex;

		ar >> m_bSelection;	
	}
}

void COb_pcbregion::Draw(CDC* pDC, CView* pV)
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
		if(m_nFlag == FLAG_DELETED) color = nEDA_PCB_BoardAreaColor;
		else color = nEDA_PCB_LayerColor[m_nLayer];
	}

	if(m_bPolygonCutout == TRUE)
	{
		DottedFrameRegion(pDC, pV, color);
	}
	else if(m_bBoardCutout == TRUE)
	{
		if(pView->m_nPrint_Flag == 1)
		{
			COLORREF cutout_color = RGB(255,255,255);
			FillRegion(pDC, pV, cutout_color);
			FrameRegion(pDC, pV, 2, color);
		}
		else
		{
			FillRegion(pDC, pV, color);
			FrameRegion(pDC, pV, 2, color);
		}
	}
	else 
	{
		if((pView->m_nPrint_Flag == 0)&&(m_bKeepout == TRUE)&&(m_nFlag == FLAG_NORMAL))  
		{
			FillRegion(pDC, pV, color);
			FrameRegion(pDC, pV, 2, nEDA_PCB_LayerColor[KEEPOUTLAYER]);
		}
		else 
		{
			FillRegion(pDC, pV, color);
			FrameRegion(pDC, pV, 2, color);
		}
	}
}

void COb_pcbregion::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	CPoint point;
	COLORREF color = nEDA_PCB_LayerColor[m_nLayer];

	if(m_bPolygonCutout == TRUE)
	{
		int x,y;
		CPoint point;

		int oldbkmode = pDC->SetBkMode(TRANSPARENT);

		CPen mypen, *oldpen;
		mypen.CreatePen(PS_DOT, 1, color);
		oldpen = pDC->SelectObject(&mypen);
		
		int startx, starty;
		BOOL bPreviousIsArc = FALSE;
		for(int i=0; i<m_cVertex.GetCount(); i++)
		{
			Struct_RegionVertex  vertex;
			vertex = m_cVertex.GetAt(i);
			if(i == 0)
			{
				//pView->Get_VPort_XY(&x, &y, vertex.fx, vertex.fy);
				x = (int)((vertex.fx - origin_x)/scale);
				y = nEy - (int)((vertex.fy - origin_y)/scale);	

				startx = x;
				starty = y;
				pDC->MoveTo(x, y);
			}

			if(vertex.radius == 0)
			{
				bPreviousIsArc = FALSE;
				if(i != 0)
				{
					x = (int)((vertex.fx - origin_x)/scale);
					y = nEy - (int)((vertex.fy - origin_y)/scale);	

					pDC->LineTo(x, y);
				}
			}
			else
			{
				x = (int)((vertex.fx - origin_x)/scale);
				y = nEy - (int)((vertex.fy - origin_y)/scale);	

				if(bPreviousIsArc == FALSE) pDC->LineTo(x, y);
				else pDC->MoveTo(x, y);
				
				double sx, sy;
				sx = vertex.cx + vertex.radius*cos(vertex.sangle*fEDA_pi/180);
				sy = vertex.cy + vertex.radius*sin(vertex.sangle*fEDA_pi/180);
				
				x = (int)((vertex.fx - origin_x)/scale);
				y = nEy - (int)((vertex.fy - origin_y)/scale);	


				int r = (int)(vertex.radius/scale);

				int sweep_angle;
				sweep_angle = (int)(vertex.eangle - vertex.sangle);
				if(sweep_angle < 0) sweep_angle += 360;			

				if((fabs(vertex.fx - sx) < 0.01f)&&(fabs(vertex.fy - sy) < 0.01f))
				{
					pDC->AngleArc(x, y, r, vertex.sangle, sweep_angle);
				}
				else
				{
					x = (int)((sx - origin_x)/scale);
					y = nEy - (int)((sy - origin_y)/scale);	

					pDC->MoveTo(x, y);

					x = (int)((vertex.fx - origin_x)/scale);
					y = nEy - (int)((vertex.fy - origin_y)/scale);	

					pDC->AngleArc(x, y, r, vertex.sangle, sweep_angle);

					x = (int)((sx - origin_x)/scale);
					y = nEy - (int)((sy - origin_y)/scale);	
	
					pDC->MoveTo(x, y);
				}
				bPreviousIsArc = TRUE;
			}
		}
		pDC->LineTo(startx, starty);
		
		pDC->SetBkMode(oldbkmode);
		pDC->SelectObject(oldpen);
	}
	else
	{
		CArray<Fpoint, Fpoint&> arrayVertex;	
		POINT* p;	
		GetRegionVertex(arrayVertex);

		int vertex_number = arrayVertex.GetSize();
		p = (POINT*)malloc(2*vertex_number*sizeof(int));
		for(int i=0; i<vertex_number; i++)  
		{
			int x,y;
			x = (int)((arrayVertex[i].fx - origin_x)/scale);
			y = nEy - (int)((arrayVertex[i].fy - origin_y)/scale);	

			p[i].x = x;
			p[i].y = y;
		}

		CRgn rgn;
		rgn.CreatePolygonRgn(p, vertex_number, ALTERNATE);

		free(p);

		CBrush fillbrush;
		fillbrush.CreateSolidBrush(color);

		pDC->FillRgn(&rgn, &fillbrush);
	}
}

void COb_pcbregion::DrawSelection(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;
	
	CArray<Fpoint, Fpoint&> arrayVertex;	
	POINT* p;	
	GetRegionVertex(arrayVertex);

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
	CPoint* lpPoint;
	for(int i=0; i<m_arrayHoles.GetCount(); i++)
	{
		CArray<Fpoint, Fpoint>* p_arrayHole = m_arrayHoles.GetAt(i);
		CRgn rgn1, rgn2;
		rgn2.CreateRectRgn( 0, 0, 50, 50 );

		int holevertex = p_arrayHole->GetCount();
		if(holevertex == 0) continue;
		lpPoint  = (CPoint*)malloc(sizeof(point)*holevertex);

		for(int j=0; j<holevertex; j++)
		{
			Fpoint fp;
			fp = p_arrayHole->GetAt(j);
			int x,y;
			pView->Get_VPort_XY(&x, &y, fp.fx, fp.fy);
			lpPoint[j].x = x;
			lpPoint[j].y = y;
		}

		rgn1.CreatePolygonRgn( lpPoint, holevertex, ALTERNATE);
		free(lpPoint);
	
		rgn2.CombineRgn(&rgn, &rgn1, RGN_DIFF);
		rgn.CopyRgn(&rgn2);
	}

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

	for(int i =0; i<m_cVertex.GetCount(); i++)
	{
		int x,y;
		pView->Get_VPort_XY(&x,&y,m_cVertex[i].fx,m_cVertex[i].fy);
		pView->DrawSelectionKeyPoint(x, y, pDC);
	}
}

void COb_pcbregion::FillRegion(CDC* pDC, CView* pV, COLORREF color)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	CESDView* pView = (CESDView*)pV;

	CArray<Fpoint, Fpoint&> arrayVertex;	
	POINT* p;	
	GetRegionVertex(arrayVertex);

	int vertex_number = arrayVertex.GetSize();
	p = (POINT*)malloc(2*vertex_number*sizeof(int));
	for(int i=0; i<vertex_number; i++)  
	{
		int x,y;
		pView->Get_VPort_XY(&x, &y, arrayVertex[i].fx, arrayVertex[i].fy);
		if(pView->m_nPrint_Flag == 1)
		{
			x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
			y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
		}
		p[i].x = x;
		p[i].y = y;
	}

	CRgn rgn;
	rgn.CreatePolygonRgn(p, vertex_number, ALTERNATE);

	free(p);

	CPoint point;
	CPoint* lpPoint;

	for(int i=0; i<m_arrayHoles.GetCount(); i++)
	{
		CArray<Fpoint, Fpoint>* p_arrayHole = m_arrayHoles.GetAt(i);
		CRgn rgn1, rgn2;
		rgn2.CreateRectRgn( 0, 0, 50, 50 );

		int holevertex = p_arrayHole->GetCount();
		if(holevertex == 0) continue;
		lpPoint  = (CPoint*)malloc(sizeof(point)*holevertex);

		for(int j=0; j<holevertex; j++)
		{
			Fpoint fp;
			fp = p_arrayHole->GetAt(j);

			int x,y;
			pView->Get_VPort_XY(&x, &y, fp.fx, fp.fy);
			if(pView->m_nPrint_Flag == 1)
			{
				x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
			}
			lpPoint[j].x = x;
			lpPoint[j].y = y;
		}

		rgn1.CreatePolygonRgn( lpPoint, holevertex, ALTERNATE);
		free(lpPoint);
	
		rgn2.CombineRgn(&rgn, &rgn1, RGN_DIFF);
		rgn.CopyRgn(&rgn2);
	}

	CBrush fillbrush;
	fillbrush.CreateSolidBrush(color);

	pDC->FillRgn(&rgn, &fillbrush);
}

void COb_pcbregion::FrameRegion(CDC* pDC, CView* pV, int width, COLORREF color)
{
	int x,y;
	CPoint point;
	//CPoint* lpPoint;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();


	CPen mypen;
	mypen.CreatePen(PS_SOLID, width, color);
	pDC->SelectObject(&mypen);
	
	int startx, starty;
	BOOL bPreviousIsArc = FALSE;
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex;
		vertex = m_cVertex.GetAt(i);
		if(i == 0)
		{
			pView->Get_VPort_XY(&x, &y, vertex.fx, vertex.fy);
			if(pView->m_nPrint_Flag == 1)
			{
				x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
			}
			startx = x;
			starty = y;
			pDC->MoveTo(x, y);
		}

		if(vertex.radius == 0)
		{
			bPreviousIsArc = FALSE;
			if(i != 0)
			{
				pView->Get_VPort_XY(&x, &y, vertex.fx, vertex.fy);
				if(pView->m_nPrint_Flag == 1)
				{
					x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
					y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
				}
				pDC->LineTo(x, y);
			}
		}
		else
		{
			pView->Get_VPort_XY(&x, &y, vertex.fx, vertex.fy);
			if(pView->m_nPrint_Flag == 1)
			{
				x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
			}
			if(bPreviousIsArc == FALSE) pDC->LineTo(x, y);
			else pDC->MoveTo(x, y);
			
			double sx, sy;
			sx = vertex.cx + vertex.radius*cos(vertex.sangle*fEDA_pi/180);
			sy = vertex.cy + vertex.radius*sin(vertex.sangle*fEDA_pi/180);
			
			pView->Get_VPort_XY(&x, &y, vertex.cx, vertex.cy);
			if(pView->m_nPrint_Flag == 1)
			{
				x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
			}

			int r = (int)(vertex.radius/pView->m_fScale);
			if(pView->m_nPrint_Flag == 1)
			{
				r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
			}
			int sweep_angle;
			sweep_angle = (int)(vertex.eangle - vertex.sangle);
			if(sweep_angle < 0) sweep_angle += 360;			

			if((fabs(vertex.fx - sx) < 0.01f)&&(fabs(vertex.fy - sy) < 0.01f))
			{
				pDC->AngleArc(x, y, r, vertex.sangle, sweep_angle);
			}
			else
			{
				pView->Get_VPort_XY(&x, &y, sx, sy);
				if(pView->m_nPrint_Flag == 1)
				{
					x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
					y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
				}
				pDC->MoveTo(x, y);

				pView->Get_VPort_XY(&x, &y, vertex.cx, vertex.cy);
				if(pView->m_nPrint_Flag == 1)
				{
					x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
					y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
				}
				pDC->AngleArc(x, y, r, vertex.sangle, sweep_angle);

				pView->Get_VPort_XY(&x, &y, sx, sy);
				if(pView->m_nPrint_Flag == 1)
				{
					x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
					y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
				}	
				pDC->MoveTo(x, y);
			}
			bPreviousIsArc = TRUE;
		}
	}
	pDC->LineTo(startx, starty);

	for(int i=0; i<m_arrayHoles.GetCount(); i++)
	{
		CArray<Fpoint, Fpoint>* p_arrayHole = m_arrayHoles.GetAt(i);
		int holevertex = p_arrayHole->GetCount();
		if(holevertex == 0) continue;

		int startx, starty;
		for(int j=0; j<holevertex; j++)
		{
			Fpoint fp;
			fp = p_arrayHole->GetAt(j);

			int x,y;
			pView->Get_VPort_XY(&x, &y, fp.fx, fp.fy);
			if(pView->m_nPrint_Flag == 1)
			{
				x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
			}
			if(j == 0) 
			{
				startx = x;
				starty = y;
				pDC->MoveTo(x,y);
			}
			else pDC->LineTo(x,y);
		}
		pDC->LineTo(startx,starty);
	}
}

void COb_pcbregion::DottedFrameRegion(CDC* pDC, CView* pV, COLORREF color)
{
	int x,y;
	CPoint point;
	//CPoint* lpPoint;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();


	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	CPen mypen, *oldpen;
	mypen.CreatePen(PS_DOT, 1, color);
	oldpen = pDC->SelectObject(&mypen);
	
	int startx, starty;
	BOOL bPreviousIsArc = FALSE;
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex;
		vertex = m_cVertex.GetAt(i);
		if(i == 0)
		{
			pView->Get_VPort_XY(&x, &y, vertex.fx, vertex.fy);
			if(pView->m_nPrint_Flag == 1)
			{
				x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
			}
			startx = x;
			starty = y;
			pDC->MoveTo(x, y);
		}

		if(vertex.radius == 0)
		{
			bPreviousIsArc = FALSE;
			if(i != 0)
			{
				pView->Get_VPort_XY(&x, &y, vertex.fx, vertex.fy);
				if(pView->m_nPrint_Flag == 1)
				{
					x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
					y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
				}
				pDC->LineTo(x, y);
			}
		}
		else
		{
			pView->Get_VPort_XY(&x, &y, vertex.fx, vertex.fy);
			if(pView->m_nPrint_Flag == 1)
			{
				x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
			}
			if(bPreviousIsArc == FALSE) pDC->LineTo(x, y);
			else pDC->MoveTo(x, y);
			
			double sx, sy;
			sx = vertex.cx + vertex.radius*cos(vertex.sangle*fEDA_pi/180);
			sy = vertex.cy + vertex.radius*sin(vertex.sangle*fEDA_pi/180);
			
			pView->Get_VPort_XY(&x, &y, vertex.cx, vertex.cy);
			if(pView->m_nPrint_Flag == 1)
			{
				x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
			}

			int r = (int)(vertex.radius/pView->m_fScale);
			if(pView->m_nPrint_Flag == 1)
			{
				r = (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
			}
			int sweep_angle;
			sweep_angle = (int)(vertex.eangle - vertex.sangle);
			if(sweep_angle < 0) sweep_angle += 360;			

			if((fabs(vertex.fx - sx) < 0.01f)&&(fabs(vertex.fy - sy) < 0.01f))
			{
				pDC->AngleArc(x, y, r, vertex.sangle, sweep_angle);
			}
			else
			{
				pView->Get_VPort_XY(&x, &y, sx, sy);
				if(pView->m_nPrint_Flag == 1)
				{
					x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
					y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
				}
				pDC->MoveTo(x, y);

				pView->Get_VPort_XY(&x, &y, vertex.cx, vertex.cy);
				if(pView->m_nPrint_Flag == 1)
				{
					x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
					y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
				}
				pDC->AngleArc(x, y, r, vertex.sangle, sweep_angle);

				pView->Get_VPort_XY(&x, &y, sx, sy);
				if(pView->m_nPrint_Flag == 1)
				{
					x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
					y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
				}	
				pDC->MoveTo(x, y);
			}
			bPreviousIsArc = TRUE;
		}
	}
	pDC->LineTo(startx, starty);
	
	pDC->SetBkMode(oldbkmode);
	pDC->SelectObject(oldpen);
}

BOOL COb_pcbregion::PickMe(CView* pV)
{
	CESDView* pView = (CESDView*)pV;

	int wx1,wy1,wx2,wy2;
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

	int x1,y1,x2,y2;	
	int maxx, minx, maxy, miny;
	x1 = (int)m_cVertex[0].fx;
	y1 = (int)m_cVertex[0].fy;
	x2 = (int)m_cVertex[1].fx;
	y2 = (int)m_cVertex[1].fy;
	minx = MIN(x1,x2); miny = MIN(y1,y2); 
	maxx = MAX(x1,x2); maxy = MAX(y1,y2);

	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		x1 = (int)m_cVertex[i].fx;
		y1 = (int)m_cVertex[i].fy;
		minx = MIN(x1,minx); miny = MIN(y1,miny); 
		maxx = MAX(x1,maxx); maxy = MAX(y1,maxy);
	}

	if( ((minx < wx1)&&(maxx < wx1)) || ((minx > wx2)&&(maxx > wx2)) || ((miny < wy1)&&(maxy < wy1)) || ((miny > wy2)&&(maxy > wy2)) )	return FALSE;
	else return TRUE;


}

BOOL  COb_pcbregion::Is_On_Me(double fx, double fy)
{
	CPoint point;
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return FALSE;

	CArray<Fpoint, Fpoint&> arrayVertex;	
	POINT* p;	
	GetRegionVertex(arrayVertex);

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

	if(rgn.PtInRegion((int)fx, (int)fy) == FALSE) return FALSE;
	
	for(int i=0; i<m_arrayHoles.GetCount(); i++)
	{
		CArray<Fpoint, Fpoint>* p_arrayHole = m_arrayHoles.GetAt(i);
		CRgn rgn1;

		int holevertex = p_arrayHole->GetCount();
		if(holevertex == 0) continue;
		CPoint* lpPoint  = (CPoint*)malloc(sizeof(point)*holevertex);

		for(int j=0; j<holevertex; j++)
		{
			Fpoint fp;
			fp = p_arrayHole->GetAt(j);

			lpPoint[j].x = (int)fp.fx;
			lpPoint[j].y = (int)fp.fy;
		}

		rgn1.CreatePolygonRgn(lpPoint, holevertex, ALTERNATE);
		free(lpPoint);

		if(rgn1.PtInRegion((int)fx, (int)fy) == TRUE) return FALSE;
	}	

	return TRUE;
	
}

BOOL  COb_pcbregion::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
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

void COb_pcbregion::Change(int index)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_PCBEditRegion dlg;
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
	dlg.arrayStringNet.Add("无网络");
	for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = (COb_pcbnet*)pDoc->m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;
	
		dlg.arrayStringNet.Add(pnet->m_csNetName);
	}
	dlg.m_csComboNet = dlg.arrayStringNet.GetAt(m_nNet + 1);

	dlg.m_csMultiLayerName = pDoc->m_sPCB_LayerParam[MULTILAYER].csLayerName;
	dlg.m_bCheckBoardCutout = m_bBoardCutout;
	dlg.m_bCheckKeepout = m_bKeepout;
	dlg.m_bCheckLocked = m_bLocked;
	dlg.m_bCheckPolygonCutout = m_bPolygonCutout;

	if(dlg.DoModal() == IDOK)
	{
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = PCB_ELEMENT_REGION;
		pundo->m_nIndex = index;
		pundo->pcbregion.Copy(this);
			
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

		for(int i=0; i<=MULTILAYER; i++)
		{
			if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
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

		m_bBoardCutout = dlg.m_bCheckBoardCutout;
		m_bKeepout = dlg.m_bCheckKeepout;
		m_bLocked = dlg.m_bCheckLocked;
		m_bPolygonCutout = dlg.m_bCheckPolygonCutout;
	}
}

void COb_pcbregion::Copy(COb_pcbregion* pregion)
{
	pDocument = pregion->pDocument;
	
	m_cVertex.SetSize(pregion->m_cVertex.GetCount());
	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		m_cVertex[i].fx = pregion->m_cVertex[i].fx;
		m_cVertex[i].fy = pregion->m_cVertex[i].fy;
		m_cVertex[i].cx = pregion->m_cVertex[i].cx;
		m_cVertex[i].cy = pregion->m_cVertex[i].cy;
		m_cVertex[i].sangle = pregion->m_cVertex[i].sangle;
		m_cVertex[i].eangle = pregion->m_cVertex[i].eangle;
		m_cVertex[i].radius = pregion->m_cVertex[i].radius;
	}

	m_arrayHoles.RemoveAll();
	for(int i=0; i<pregion->m_arrayHoles.GetCount(); i++)
	{
		CArray<Fpoint, Fpoint>* p_arrayHole = pregion->m_arrayHoles.GetAt(i);
		CArray<Fpoint, Fpoint>* p_newHole = new CArray<Fpoint, Fpoint>;
		for(int j=0; j<p_arrayHole->GetCount(); j++)
		{
			Fpoint fp = p_arrayHole->GetAt(j);
			p_newHole->Add(fp);
		}
		m_arrayHoles.Add(p_newHole);
	}

	m_nLayer = pregion->m_nLayer;
	m_nNet = pregion->m_nNet;
	m_nComponent = pregion->m_nComponent;
	m_nPolygon = pregion->m_nPolygon;

	m_bLocked = pregion->m_bLocked;
	m_bKeepout = pregion->m_bKeepout;
	m_bPolygonCutout = pregion->m_bPolygonCutout;
	m_bBoardCutout = pregion->m_bBoardCutout;

	//------ 下面参数的意义有待明确
	//int m_nIndexForSave;
	m_bPolygonOutline = pregion->m_bPolygonOutline;
	m_bUserRouted = pregion->m_bUserRouted;
	m_nUnionIndex = pregion->m_nUnionIndex;
	m_nSubPolyIndex = pregion->m_nSubPolyIndex;

	m_bSelection = FALSE; //pregion->m_bSelection;
}

RECT COb_pcbregion::GetRegionRect()
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

void COb_pcbregion::GetRegionVertex(CArray<Fpoint, Fpoint&>& arrayVertex)
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

		if(vertex.radius <= 0)
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

void COb_pcbregion::Move(CClientDC* pDC, CView* pV, int index)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动区域: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV);

	x0 = m_cVertex[0].fx;
	y0 = m_cVertex[0].fy;
	
	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_pcbregion aregion;
	aregion.Copy(this);
	aregion.m_nFlag = FLAG_NORMAL;
	
	//aregion.Draw(pDC, pV);
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
			//aregion.Draw(pDC, pV);
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
			pundo->m_nOb = PCB_ELEMENT_REGION;
			pundo->m_nIndex = index;
			pundo->pcbregion.Copy(this);
			pundo->pcbregion.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;


			for(int i=0; i<m_cVertex.GetCount(); i++)
			{
				m_cVertex[i].fx = aregion.m_cVertex[i].fx;
				m_cVertex[i].fy = aregion.m_cVertex[i].fy;
				m_cVertex[i].cx = aregion.m_cVertex[i].cx;
				m_cVertex[i].cy = aregion.m_cVertex[i].cy;
				m_cVertex[i].sangle = aregion.m_cVertex[i].sangle;
				m_cVertex[i].eangle = aregion.m_cVertex[i].eangle;
			}
			for(int i=0; i<m_arrayHoles.GetCount(); i++)
			{
				CArray<Fpoint, Fpoint>* p_arrayHole = m_arrayHoles.GetAt(i);
				for(int j=0; j<p_arrayHole->GetCount(); j++)
				{
					Fpoint fp = p_arrayHole->GetAt(j);
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, fp.fx-x0, fp.fy-y0, flag*90, FALSE);
					fp.fx = fx + x0 + deltax;
					fp.fy = fy + y0 + deltay;	
					p_arrayHole->SetAt(j, fp);
				}
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
	
	pView->CloseMouseCursor();

	m_nFlag = FLAG_NORMAL;
	Draw(pDC, pV);
	pView->OpenMouseCursor();

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	pView->Write_Help(szEDA_Help_Ready);
}

void COb_pcbregion::MoveAndRotate(CView* pV, double x0, double y0,  double deltax, double deltay, int rotation, BOOL mirror, int index)
{
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();


	CPcbUndo *pundo = new CPcbUndo;
	pundo->m_nOp = UNDO_CHANGE;
	pundo->m_nOb = PCB_ELEMENT_REGION;
	pundo->m_nIndex = index;
	pundo->pcbregion.Copy(this);
	pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

    
	for(int i=0;  i<m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex vertex;
		vertex = m_cVertex.GetAt(i);
		//fx,fy
		double fx, fy;
		pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, rotation, mirror);
		m_cVertex[i].fx = fx + x0 + deltax;
		m_cVertex[i].fy = fy + y0 + deltay;
		//cx,cy
		pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].cx-x0, m_cVertex[i].cy-y0, rotation, mirror);
		m_cVertex[i].cx = fx + x0 + deltax;
		m_cVertex[i].cy = fy + y0 + deltay;
		//sangle, eangle
		double sangle, eangle;
		pDoc->Get_Rotate_Angle(&sangle, &eangle, vertex.sangle, vertex.eangle, rotation, mirror);
		vertex.sangle = sangle;
		vertex.sangle = pDoc->Get_Correct_Angle(vertex.sangle);
		vertex.eangle = eangle;
		vertex.eangle = pDoc->Get_Correct_Angle(vertex.eangle);

		m_cVertex.SetAt(i, vertex);
	}

	for(int i=0; i<m_arrayHoles.GetCount(); i++)
	{
		CArray<Fpoint, Fpoint>* p_arrayHole = m_arrayHoles.GetAt(i);
		for(int j=0; j<p_arrayHole->GetCount(); j++)
		{
			Fpoint fp = p_arrayHole->GetAt(j);
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, fp.fx - x0, fp.fy - y0, rotation, mirror);
			fp.fx = fx + x0 + deltax;
			fp.fy = fy + y0 + deltay;
			p_arrayHole->SetAt(j, fp);
		}
	}
}

BOOL COb_pcbregion::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	
	char szHelp[] = "复制区域: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	pView->Invalidate();

	x0 = m_cVertex[0].fx;
	y0 = m_cVertex[0].fy;
	
	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_pcbregion aregion;
	aregion.Copy(this);
	aregion.m_nFlag = FLAG_NORMAL;
	
	//aregion.Draw(pDC, pV);
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
			//aregion.Draw(pDC, pV);
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
				//aregion.Draw(pDC, pV);
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
			//aregion.Draw(pDC, pV);
			aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			//aregion.Draw(pDC, pV);
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

			for(int i=0; i<m_cVertex.GetCount(); i++)
			{
				m_cVertex[i].fx = aregion.m_cVertex[i].fx;
				m_cVertex[i].fy = aregion.m_cVertex[i].fy;
				m_cVertex[i].cx = aregion.m_cVertex[i].cx;
				m_cVertex[i].cy = aregion.m_cVertex[i].cy;
				m_cVertex[i].sangle = aregion.m_cVertex[i].sangle;
				m_cVertex[i].eangle = aregion.m_cVertex[i].eangle;
			}
			for(int i=0; i<m_arrayHoles.GetCount(); i++)
			{
				CArray<Fpoint, Fpoint>* p_arrayHole = m_arrayHoles.GetAt(i);
				for(int j=0; j<p_arrayHole->GetCount(); j++)
				{
					Fpoint fp = p_arrayHole->GetAt(j);
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, fp.fx-x0, fp.fy-y0, flag*90, FALSE);
					fp.fx = fx + x0 + deltax;
					fp.fy = fy + y0 + deltay;	
					p_arrayHole->SetAt(j, fp);
				}
			}
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			//aregion.Draw(pDC, pV);
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
			//aregion.Draw(pDC, pV);
			aregion.FrameRegion(pDC, pV, 1, nEDA_PCB_LayerColor[m_nLayer]);

			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	
	pView->CloseMouseCursor();

	m_nFlag = FLAG_NORMAL;
	Draw(pDC, pV);
	pView->OpenMouseCursor();

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;
	pView->Write_Help(szEDA_Help_Ready);

	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;
}

void COb_pcbregion::DrawHighlighted(CDC* pDC, CView* pV, int layer)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;

	COLORREF color;
	if(layer == CONNECTIONS) color = nEDA_PCB_LayerColor[SELECTIONS];
	else if(layer == DRCERROR) color = nEDA_PCB_LayerColor[DRCERROR];
	
	FillRegion(pDC, pV, color);
	FrameRegion(pDC, pV, 2, nEDA_PCB_LayerColor[m_nLayer]);
}