#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Sheet_EditPolygon.h"
#include "Dlg_Global_Fill.h"

IMPLEMENT_SERIAL(COb_schpolygon, CObject, 0)
COb_schpolygon::COb_schpolygon()
{
	// This empty constructor should be used by serialization only
	m_nFlag = 0;
	m_bSelection = FALSE;
	m_nOwnerPartID = 0;
	m_nDisplayMode = 0;
	m_posParent = NULL;

	pDocument = NULL;
}



void COb_schpolygon::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nBorderWidth;
		ar << m_nFillColor;
		ar << m_nBorderColor;
		ar << m_bTransparent;
		ar << m_bFillSolid;

		int vertexNum = m_cVertex.GetCount();
		ar << vertexNum;
		for(int i=0; i<vertexNum; i++)
		{
			ar << m_cVertex[i].fx;
			ar << m_cVertex[i].fy;
		}

		ar << m_nOwnerPartID;
		ar << m_nDisplayMode;
		//ar << m_posParent;		
		
		ar << m_nDiagram;
		ar << m_bSelection;

	}
	else
	{
		ar >> m_nBorderWidth;
		ar >> m_nFillColor;
		ar >> m_nBorderColor;
		ar >> m_bTransparent;
		ar >> m_bFillSolid;

		int vertexNum;
		ar >> vertexNum;
		m_cVertex.RemoveAll();
		m_cVertex.SetSize(vertexNum);
		for(int i=0; i<vertexNum; i++)
		{
			ar >> m_cVertex[i].fx;
			ar >> m_cVertex[i].fy;
		}

		ar >> m_nOwnerPartID;
		ar >> m_nDisplayMode;
		//ar >> m_posParent;		
		
		ar >> m_nDiagram;
		ar >> m_bSelection;
	}
}

void COb_schpolygon::Draw(CDC* pDC, CView* pV)
{
	int i; 
	COLORREF color, fillcolor;
	POINT p[10000];
	int x,y;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	
	if(m_nFlag == FLAG_NORMAL) 
	{
		color = m_nBorderColor;
		fillcolor = m_nFillColor;
	}
	else if(m_nFlag == FLAG_DELETED) 
	{
		color = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor;
		fillcolor = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor;
	}
	
	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_Print_Color == 0) 
		{
			color = RGB(0,0,0);
			fillcolor = RGB(0,0,0);
		}
		else if(nEDA_Print_Color == 2) 
		{
			color = RGB(GetGray(m_nBorderColor),GetGray(m_nBorderColor),GetGray(m_nBorderColor));
			fillcolor = RGB(GetGray(m_nFillColor),GetGray(m_nFillColor),GetGray(m_nFillColor));
		}
		else 
		{
			color = m_nBorderColor;
			fillcolor = m_nFillColor;
		}
	}

	int width;

	width = nEDA_Line_Width[m_nBorderWidth];

	width = (int)(width/pView->m_fScale);
	if(width < 1) width = 1;
	
	if(pView->m_nPrint_Flag == 1) width = (int)(width*nEDA_Print_Xdpi/1000*fEDA_Print_Scale);

	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,width,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
 
	CBrush fillbrush;
	fillbrush.CreateSolidBrush(fillcolor);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);


	int mode;
	if((m_bTransparent == TRUE)&&(m_nFlag == FLAG_NORMAL))
	{
		mode = pDC->SetROP2(R2_MASKPEN);
	}
	
	int vertex_number = m_cVertex.GetSize();
	for(i=0; i<vertex_number; i++)  
	{
		pView->Get_VPort_XY(&x, &y, m_cVertex[i].fx, m_cVertex[i].fy);
		p[i].x = x;
		p[i].y = y;
		if(pView->m_nPrint_Flag == 1)
		{
			p[i].x = (int)(((double)p[i].x*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
			p[i].y = (int)(((double)p[i].y*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
		}

	}
	
	if(m_bFillSolid == TRUE) pDC->Polygon(p,vertex_number);
	else 
	{
		if((p[0].x != p[vertex_number-1].x)||(p[0].y != p[vertex_number-1].y))
		{
			p[vertex_number].x = p[0].x;
			p[vertex_number].y = p[0].y;
			vertex_number++;
		}
		pDC->Polyline(p,vertex_number);
	}

	if((m_bTransparent == TRUE)&&(m_nFlag == FLAG_NORMAL))
	{
		pDC->SetROP2(mode);
	}
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void COb_schpolygon::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	int i; 
	COLORREF color, fillcolor;
	POINT p[10000];
	int x,y;
	
	color = m_nBorderColor;
	fillcolor = m_nFillColor;

	int width;
	//线宽
	width = nEDA_Line_Width[m_nBorderWidth];

	width = (int)(width/scale);
	if(width < 1) width = 1;
	
	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,width,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
 
	CBrush fillbrush;
	fillbrush.CreateSolidBrush(fillcolor);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	int mode;
	if((m_bTransparent == TRUE)&&(m_nFlag == FLAG_NORMAL))
	{
		mode = pDC->SetROP2(R2_MASKPEN);
	}
	
	int vertex_number = m_cVertex.GetSize();
	for(i=0; i<vertex_number; i++)  
	{
		//pView->Get_VPort_XY(&x, &y, m_cVertex[i].fx, m_cVertex[i].fy);
		x = (int)((m_cVertex[i].fx - origin_x)/scale);
		y = nEy - (int)((m_cVertex[i].fy - origin_y)/scale);
		p[i].x = x;
		p[i].y = y;
	}
	
	if(m_bFillSolid == TRUE) pDC->Polygon(p,vertex_number);
	else 
	{
		if((p[0].x != p[vertex_number-1].x)||(p[0].y != p[vertex_number-1].y))
		{
			p[vertex_number].x = p[0].x;
			p[vertex_number].y = p[0].y;
			vertex_number++;
		}
		pDC->Polyline(p,vertex_number);
	}

	if((m_bTransparent == TRUE)&&(m_nFlag == FLAG_NORMAL))
	{
		pDC->SetROP2(mode);
	}
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void COb_schpolygon::DrawSelection(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
	
	for(int i =0; i<m_cVertex.GetCount(); i++)
	{
		int x,y;
		pView->Get_VPort_XY(&x,&y,m_cVertex[i].fx,m_cVertex[i].fy);
		pView->DrawSelectionKeyPoint(x, y, pDC);
	}

}

BOOL COb_schpolygon::PickMe(CView* pV)
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

BOOL  COb_schpolygon::Is_On_Me(double fx, double fy)
{
	
	int vertex = m_cVertex.GetCount();

	CPoint point;
	CPoint* lpPoint  = (CPoint*)malloc(sizeof(point)*vertex);


	for(int i=0; i<vertex; i++)
	{
		lpPoint[i].x = (int)m_cVertex[i].fx;
		lpPoint[i].y = (int)m_cVertex[i].fy;
	}

	CRgn   rgn;

	rgn.CreatePolygonRgn( lpPoint, vertex, ALTERNATE);

	free(lpPoint);

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

BOOL  COb_schpolygon::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
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

void COb_schpolygon::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CSheet_EditPolygon  ps(_T("多边形"), NULL, 0);

	ps.page1.m_bCheckDrawSolid = m_bFillSolid;
	ps.page1.m_bCheckTransparent = m_bTransparent;
	ps.page1.m_nComboWidth = m_nBorderWidth;
	ps.page1.m_nFillColor = m_nFillColor;
	ps.page1.m_nBorderColor = m_nBorderColor;
	
	ps.page2.m_listCtrl.m_arrayX.RemoveAll();
	ps.page2.m_listCtrl.m_arrayY.RemoveAll();
	ps.page2.m_listCtrl.m_arrayX.SetSize(m_cVertex.GetCount());
	ps.page2.m_listCtrl.m_arrayY.SetSize(m_cVertex.GetCount());
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		ps.page2.m_listCtrl.m_arrayX[i] = pDoc->Convert_XY_To_String(m_cVertex[i].fx);
		ps.page2.m_listCtrl.m_arrayY[i] = pDoc->Convert_XY_To_String(m_cVertex[i].fy);
	}
	
	if(ps.DoModal() == IDOK)
	{
		if(ps.page1.m_bCheckGlobal == TRUE)
		{
			CDlg_Global_Fill subdlg;
			if(subdlg.DoModal() == IDOK)
			{
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}

					
				if(subdlg.m_nRadioScope == 0)	
				{
					CObject* pobject;
					for(int nDiagram = 0; nDiagram < SCH_SHEET_NUM; nDiagram++)
					{
						int num = pDoc->m_listSchObject[nDiagram].GetCount();
						POSITION position, posbak;
						position = pDoc->m_listSchObject[nDiagram].GetHeadPosition();
						for(int i=0; i<num; i++)
						{	
							if(position == NULL) break;

							posbak = position;
							pobject = pDoc->m_listSchObject[nDiagram].GetNext(position);
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
							{
								COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
								if((ppolygon->m_nDiagram != nDiagram)||(ppolygon->m_nFlag != FLAG_NORMAL))	continue;
								if(ppolygon == this) continue; 

								if( (((subdlg.m_bCheckColor == TRUE)&&(ppolygon->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckWidth == TRUE)&&(ppolygon->m_nBorderWidth == m_nBorderWidth))||(subdlg.m_bCheckWidth == FALSE)) && 
									(((subdlg.m_bCheckFillColor == TRUE)&&(ppolygon->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
									(((subdlg.m_bCheckTransparent == TRUE)&&(ppolygon->m_bTransparent == m_bTransparent))||(subdlg.m_bCheckTransparent == FALSE)) &&
									(((subdlg.m_bCheckSolid == TRUE)&&(ppolygon->m_bFillSolid == m_bFillSolid))||(subdlg.m_bCheckSolid == FALSE)) )
								{
									if( (ppolygon->m_nBorderColor == m_nBorderColor)||(ppolygon->m_nBorderWidth == m_nBorderWidth)||(ppolygon->m_nFillColor == m_nFillColor)||\
										(ppolygon->m_bTransparent == m_bTransparent)||(ppolygon->m_bFillSolid == m_bFillSolid) )
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_POLYGON;
										pundo->m_nDiagram = nDiagram;
										pundo->m_nPos = posbak;
										pundo->schpolygon.Copy(ppolygon);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

									}
									if(ppolygon->m_nBorderColor == m_nBorderColor) ppolygon->m_nBorderColor = ps.page1.m_nBorderColor;
									if(ppolygon->m_nBorderWidth == m_nBorderWidth) ppolygon->m_nBorderWidth = ps.page1.m_nComboWidth;
									if(ppolygon->m_nFillColor == m_nFillColor) ppolygon->m_nFillColor = ps.page1.m_nFillColor;
									if(ppolygon->m_bTransparent == m_bTransparent) ppolygon->m_bTransparent = ps.page1.m_bCheckTransparent;
									if(ppolygon->m_bFillSolid == m_bFillSolid) ppolygon->m_bFillSolid = ps.page1.m_bCheckDrawSolid;
								}
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
							{
								COb_schsheetsymbol* psheetsymbol;
								psheetsymbol = (COb_schsheetsymbol*)pobject;
								for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[nDiagram].GetNext(position);
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
							{
								COb_schcomp* pcomp;
								pcomp = (COb_schcomp*)pobject;

								for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[nDiagram].GetNext(position);
							}
						}
					}
				}
				else if(subdlg.m_nRadioScope == 1)	
				{
					CObject* pobject;
					int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
					POSITION position, posbak;
					position = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
					for(int i=0; i<num; i++)
					{	
						if(position == NULL) break;

						posbak = position;
						pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(position);
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
						{
							COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
							if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag != FLAG_NORMAL))	continue;
							if(ppolygon == this) continue; 

							if( (((subdlg.m_bCheckColor == TRUE)&&(ppolygon->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(ppolygon->m_nBorderWidth == m_nBorderWidth))||(subdlg.m_bCheckWidth == FALSE)) && 
								(((subdlg.m_bCheckFillColor == TRUE)&&(ppolygon->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
								(((subdlg.m_bCheckTransparent == TRUE)&&(ppolygon->m_bTransparent == m_bTransparent))||(subdlg.m_bCheckTransparent == FALSE)) &&
								(((subdlg.m_bCheckSolid == TRUE)&&(ppolygon->m_bFillSolid == m_bFillSolid))||(subdlg.m_bCheckSolid == FALSE)) )
							{
								if( (ppolygon->m_nBorderColor == m_nBorderColor)||(ppolygon->m_nBorderWidth == m_nBorderWidth)||(ppolygon->m_nFillColor == m_nFillColor)||\
									(ppolygon->m_bTransparent == m_bTransparent)||(ppolygon->m_bFillSolid == m_bFillSolid) )
								{

									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_POLYGON;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schpolygon.Copy(ppolygon);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

								}
								if(ppolygon->m_nBorderColor == m_nBorderColor) ppolygon->m_nBorderColor = ps.page1.m_nBorderColor;
								if(ppolygon->m_nBorderWidth == m_nBorderWidth) ppolygon->m_nBorderWidth = ps.page1.m_nComboWidth;
								if(ppolygon->m_nFillColor == m_nFillColor) ppolygon->m_nFillColor = ps.page1.m_nFillColor;
								if(ppolygon->m_bTransparent == m_bTransparent) ppolygon->m_bTransparent = ps.page1.m_bCheckTransparent;
								if(ppolygon->m_bFillSolid == m_bFillSolid) ppolygon->m_bFillSolid = ps.page1.m_bCheckDrawSolid;
							}
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
						{
							COb_schsheetsymbol* psheetsymbol;
							psheetsymbol = (COb_schsheetsymbol*)pobject;
							for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
						{
							COb_schcomp* pcomp;
							pcomp = (COb_schcomp*)pobject;

							for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
						}
					}
				}
				else	
				{
					CObject* pobject;
					int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
					POSITION position, posbak;
					position = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
					for(int i=0; i<num; i++)
					{	
						if(position == NULL) break;

						posbak = position;
						pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(position);
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
						{
							COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
							if((ppolygon->m_bSelection != TRUE)||(ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag != FLAG_NORMAL))	continue;
							if(ppolygon == this) continue; 

							if( (((subdlg.m_bCheckColor == TRUE)&&(ppolygon->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(ppolygon->m_nBorderWidth == m_nBorderWidth))||(subdlg.m_bCheckWidth == FALSE)) && 
								(((subdlg.m_bCheckFillColor == TRUE)&&(ppolygon->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
								(((subdlg.m_bCheckTransparent == TRUE)&&(ppolygon->m_bTransparent == m_bTransparent))||(subdlg.m_bCheckTransparent == FALSE)) &&
								(((subdlg.m_bCheckSolid == TRUE)&&(ppolygon->m_bFillSolid == m_bFillSolid))||(subdlg.m_bCheckSolid == FALSE)) )
							{
								if( (ppolygon->m_nBorderColor == m_nBorderColor)||(ppolygon->m_nBorderWidth == m_nBorderWidth)||(ppolygon->m_nFillColor == m_nFillColor)||\
									(ppolygon->m_bTransparent == m_bTransparent)||(ppolygon->m_bFillSolid == m_bFillSolid) )
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_POLYGON;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schpolygon.Copy(ppolygon);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

								}
								if(ppolygon->m_nBorderColor == m_nBorderColor) ppolygon->m_nBorderColor = ps.page1.m_nBorderColor;
								if(ppolygon->m_nBorderWidth == m_nBorderWidth) ppolygon->m_nBorderWidth = ps.page1.m_nComboWidth;
								if(ppolygon->m_nFillColor == m_nFillColor) ppolygon->m_nFillColor = ps.page1.m_nFillColor;
								if(ppolygon->m_bTransparent == m_bTransparent) ppolygon->m_bTransparent = ps.page1.m_bCheckTransparent;
								if(ppolygon->m_bFillSolid == m_bFillSolid) ppolygon->m_bFillSolid = ps.page1.m_bCheckDrawSolid;
							}
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
						{
							COb_schsheetsymbol* psheetsymbol;
							psheetsymbol = (COb_schsheetsymbol*)pobject;
							for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
						{
							COb_schcomp* pcomp;
							pcomp = (COb_schcomp*)pobject;

							for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
						}
					}
				}

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_POLYGON;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schpolygon.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				m_bFillSolid = ps.page1.m_bCheckDrawSolid;
				m_bTransparent = ps.page1.m_bCheckTransparent;
				m_nBorderWidth = ps.page1.m_nComboWidth;
				m_nFillColor = ps.page1.m_nFillColor;
				m_nBorderColor = ps.page1.m_nBorderColor;

				m_cVertex.RemoveAll();
				m_cVertex.SetSize(ps.page2.m_listCtrl.m_arrayX.GetCount());
				for(int i=0; i<ps.page2.m_listCtrl.m_arrayX.GetCount(); i++)
				{
					m_cVertex[i].fx = pDoc->Convert_String_To_XY(ps.page2.m_listCtrl.m_arrayX[i]);
					m_cVertex[i].fy = pDoc->Convert_String_To_XY(ps.page2.m_listCtrl.m_arrayY[i]);
				}

				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			}
		}
		else
		{
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = SCH_ELEMENT_POLYGON;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->schpolygon.Copy(this);
					
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			

			m_bFillSolid = ps.page1.m_bCheckDrawSolid;
			m_bTransparent = ps.page1.m_bCheckTransparent;
			m_nBorderWidth = ps.page1.m_nComboWidth;
			m_nFillColor = ps.page1.m_nFillColor;
			m_nBorderColor = ps.page1.m_nBorderColor;

			m_cVertex.RemoveAll();
			m_cVertex.SetSize(ps.page2.m_listCtrl.m_arrayX.GetCount());
			for(int i=0; i<ps.page2.m_listCtrl.m_arrayX.GetCount(); i++)
			{
				m_cVertex[i].fx = pDoc->Convert_String_To_XY(ps.page2.m_listCtrl.m_arrayX[i]);
				m_cVertex[i].fy = pDoc->Convert_String_To_XY(ps.page2.m_listCtrl.m_arrayY[i]);
			}
		}
	}
}

void COb_schpolygon::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE; 
	
	char szHelp[] = "移动多边形: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV);

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schpolygon apolygon;
	apolygon.pDocument = pDoc;

	apolygon.m_nBorderWidth = m_nBorderWidth;
	apolygon.m_nBorderColor = m_nBorderColor;
	apolygon.m_nFillColor = m_nFillColor;
	apolygon.m_bFillSolid = m_bFillSolid;
	apolygon.m_bTransparent = FALSE;
	apolygon.m_nDiagram = m_nDiagram;
	apolygon.m_nFlag = FLAG_NORMAL;
	
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		Fpoint fpoint;
		fpoint = m_cVertex.GetAt(i);
		apolygon.m_cVertex.Add(fpoint);
	}
	apolygon.Draw(pDC, pV);

	x0 = pView->m_fCurrentSnapX;
	y0 = pView->m_fCurrentSnapY;

	double dis = 10000000;
	int index;
	for(int i=0; i<apolygon.m_cVertex.GetCount(); i++)
	{
		double d = sqrt((x0 - apolygon.m_cVertex[i].fx)*(x0 - apolygon.m_cVertex[i].fx) + (y0 - apolygon.m_cVertex[i].fy)*(y0 - apolygon.m_cVertex[i].fy));
		if(d < dis)
		{
			dis = d;
			index = i;
		}
	}
	x0 = apolygon.m_cVertex[index].fx;
	y0 = apolygon.m_cVertex[index].fy;

	pView->m_fCurrentSnapX = x0;
	pView->m_fCurrentSnapY = y0;
	pView->Restore_LargeCrossXY();

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
			apolygon.Draw(pDC, pV);
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
				apolygon.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<apolygon.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				apolygon.m_cVertex[i].fx = fx + x0 + deltax;
				apolygon.m_cVertex[i].fy = fy + y0 + deltay;
			}
			
			pView->CloseMouseCursor();
			apolygon.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			apolygon.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<apolygon.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				apolygon.m_cVertex[i].fx = fx + x0 + deltax;
				apolygon.m_cVertex[i].fy = fy + y0 + deltay;
			}

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_POLYGON;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->m_cVertex.RemoveAll();
			for(int i=0; i<m_cVertex.GetCount(); i++)
			{
				Fpoint fpoint;
				fpoint = m_cVertex.GetAt(i);
				pundo->m_cVertex.Add(fpoint);
			}			
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			for(int i=0; i<m_cVertex.GetCount(); i++)
			{
				m_cVertex[i].fx = apolygon.m_cVertex[i].fx;
				m_cVertex[i].fy = apolygon.m_cVertex[i].fy;
			}
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			apolygon.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<apolygon.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				apolygon.m_cVertex[i].fx = fx + x0 + deltax;
				apolygon.m_cVertex[i].fy = fy + y0 + deltay;
			}
			apolygon.Draw(pDC, pV);

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
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	pView->Write_Help(szEDA_Help_Ready);
}

BOOL COb_schpolygon::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	
	char szHelp[] = "复制多边形: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	x0 = m_cVertex[0].fx;
	y0 = m_cVertex[0].fy;
	
	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schpolygon apolygon;
	apolygon.pDocument = pDoc;

	apolygon.m_nBorderWidth = m_nBorderWidth;
	apolygon.m_nBorderColor = m_nBorderColor;
	apolygon.m_nFillColor = m_nFillColor;
	apolygon.m_bFillSolid = m_bFillSolid;
	apolygon.m_bTransparent = FALSE;
	apolygon.m_nDiagram = m_nDiagram;
	apolygon.m_nFlag = FLAG_NORMAL;
	
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		Fpoint fpoint;
		fpoint = m_cVertex.GetAt(i);
		apolygon.m_cVertex.Add(fpoint);
	}

	apolygon.Draw(pDC, pV);
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
			apolygon.Draw(pDC, pV);
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
				apolygon.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<apolygon.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				apolygon.m_cVertex[i].fx = fx + x0 + deltax;
				apolygon.m_cVertex[i].fy = fy + y0 + deltay;
			}
			
			pView->CloseMouseCursor();
			apolygon.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			apolygon.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<apolygon.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				apolygon.m_cVertex[i].fx = fx + x0 + deltax;
				apolygon.m_cVertex[i].fy = fy + y0 + deltay;
			}

			for(int i=0; i<m_cVertex.GetCount(); i++)
			{
				m_cVertex[i].fx = apolygon.m_cVertex[i].fx;
				m_cVertex[i].fy = apolygon.m_cVertex[i].fy;
			}
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			apolygon.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<apolygon.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				apolygon.m_cVertex[i].fx = fx + x0 + deltax;
				apolygon.m_cVertex[i].fy = fy + y0 + deltay;
			}
			apolygon.Draw(pDC, pV);

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

void COb_schpolygon::Copy(COb_schpolygon* pschpolygon)
{
	m_nBorderWidth = pschpolygon->m_nBorderWidth;
	m_nFillColor = pschpolygon->m_nFillColor;
	m_nBorderColor = pschpolygon->m_nBorderColor;
	m_bTransparent = pschpolygon->m_bTransparent;
	m_bFillSolid = pschpolygon->m_bFillSolid;
	
	m_cVertex.RemoveAll();
	for(int i=0; i<pschpolygon->m_cVertex.GetCount(); i++)
	{
		Fpoint fp;
		fp = pschpolygon->m_cVertex.GetAt(i);
		m_cVertex.Add(fp);
	}	

	m_nOwnerPartID = pschpolygon->m_nOwnerPartID;
	m_nDisplayMode = pschpolygon->m_nDisplayMode;
	m_posParent = pschpolygon->m_posParent;

	m_nDiagram = pschpolygon->m_nDiagram;
	m_bSelection = pschpolygon->m_bSelection;
	m_nFlag = pschpolygon->m_nFlag;

	pDocument = pschpolygon->pDocument;
}
