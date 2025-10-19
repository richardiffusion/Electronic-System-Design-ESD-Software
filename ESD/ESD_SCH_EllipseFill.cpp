#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditEllipseFill.h"
#include "Dlg_Global_Fill.h"

IMPLEMENT_SERIAL(COb_schellipsefill, CObject, 0)
COb_schellipsefill::COb_schellipsefill()
{
	// This empty constructor should be used by serialization only
	m_nFlag = 0;
	m_bSelection = FALSE;
	m_nOwnerPartID = 0;
	m_nDisplayMode = 0;
	m_posParent = NULL;

	pDocument = NULL;
}



void COb_schellipsefill::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fCx;
		ar << m_fCy;
		ar << m_fRadius_x;
		ar << m_fRadius_y;
		ar << m_nBorderWidth;
		ar << m_nBorderColor;
		ar << m_nFillColor;
		ar << m_bFillSolid;
		ar << m_bTransparent;
		ar << m_nOwnerPartID;
		ar << m_nDisplayMode;
		ar << m_nDiagram;
		ar << m_bSelection;

	}
	else
	{
		ar >> m_fCx;
		ar >> m_fCy;
		ar >> m_fRadius_x;
		ar >> m_fRadius_y;
		ar >> m_nBorderWidth;
		ar >> m_nBorderColor;
		ar >> m_nFillColor;
		ar >> m_bFillSolid;
		ar >> m_bTransparent;
		ar >> m_nOwnerPartID;
		ar >> m_nDisplayMode;
		ar >> m_nDiagram;
		ar >> m_bSelection;	
	}
}

void COb_schellipsefill::Draw(CDC* pDC, CView* pV)
{
	int cx,cy;
	int rx, ry;
	COLORREF color, fillcolor;

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
			color = RGB(GetGray(m_nBorderColor), GetGray(m_nBorderColor), GetGray(m_nBorderColor));
			fillcolor = RGB(GetGray(m_nFillColor), GetGray(m_nFillColor), GetGray(m_nFillColor));
		}	
		else 
		{
			color = m_nBorderColor;
			fillcolor = m_nFillColor;
		}
	}


	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
	rx = (int)(m_fRadius_x/pView->m_fScale);
	ry = (int)(m_fRadius_y/pView->m_fScale);
	
	int width;
	width = nEDA_Line_Width[m_nBorderWidth];

	if(pView->m_nPrint_Flag == 1)
	{
		cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
		rx = (int)(((double)rx*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		ry = (int)(((double)ry*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
	}
	
	if(pView->m_nPrint_Flag == 1)
	{
		width = (int)(((double)width*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		if(width < 1) width = 1;
	}
	else
	{
		width = (int)(width/pView->m_fScale);
		if(width < 1) width = 1;
	}

	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,width,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
 
	CBrush fillbrush;
	fillbrush.CreateSolidBrush(fillcolor);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	int mode;
	if((m_bTransparent == TRUE)&&(m_nFlag == FLAG_NORMAL))
	{
		mode = pDC->SetROP2(R2_MASKPEN  );
	}

	RECT rect;
	rect.left = cx - rx;
	rect.right = cx + rx;
	rect.top = cy - ry;
	rect.bottom = cy + ry;

	if(m_bFillSolid == TRUE) pDC->Ellipse(&rect);
	else 
	{
		CPoint sp, ep;
		sp.x = cx + rx;
		sp.y = cy;
		ep.x = cx + rx;
		ep.y = cy;
		pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);
	}
	
	if((m_bTransparent == TRUE)&&(m_nFlag == FLAG_NORMAL))
	{
		pDC->SetROP2(mode);
	}
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}


void COb_schellipsefill::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	int cx,cy;
	int rx, ry;
	COLORREF color, fillcolor;

	color = m_nBorderColor;
	fillcolor = m_nFillColor;

	cx = (int)((m_fCx - origin_x)/scale);
	cy = nEy - (int)((m_fCy - origin_y)/scale);

	rx = (int)(m_fRadius_x/scale);
	ry = (int)(m_fRadius_y/scale);
	
	int width;
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
		mode = pDC->SetROP2(R2_MASKPEN  );
	}

	RECT rect;
	rect.left = cx - rx;
	rect.right = cx + rx;
	rect.top = cy - ry;
	rect.bottom = cy + ry;

	if(m_bFillSolid == TRUE) pDC->Ellipse(&rect);
	else 
	{
		CPoint sp, ep;
		sp.x = cx + rx;
		sp.y = cy;
		ep.x = cx + rx;
		ep.y = cy;
		pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);
	}
	
	if((m_bTransparent == TRUE)&&(m_nFlag == FLAG_NORMAL))
	{
		pDC->SetROP2(mode);
	}
	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void COb_schellipsefill::DrawSelection(CDC* pDC, CView* pV)
{
	int cx,cy;
	int rx, ry;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
	rx = (int)(m_fRadius_x/pView->m_fScale);
	ry = (int)(m_fRadius_y/pView->m_fScale);
	
	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	CPen arcpen;
	arcpen.CreatePen(PS_DOT, 1, nEDA_SelectionColor);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	pDC->MoveTo(cx + rx, cy);
	pDC->LineTo(cx, cy);
	pDC->LineTo(cx, cy - ry);

	pDC->SelectObject(oldpen);

	pView->DrawSelectionKeyPoint(cx + rx, cy, pDC);
	pView->DrawSelectionKeyPoint(cx, cy - ry, pDC);

	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}

BOOL COb_schellipsefill::PickMe(CView* pV)
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

	minx = (int)(m_fCx - m_fRadius_x);
	miny = (int)(m_fCy - m_fRadius_y);
	maxx = (int)(m_fCx + m_fRadius_x);
	maxy = (int)(m_fCy + m_fRadius_y);

   	if( ((minx < wx1)&&(maxx < wx1)) || ((minx > wx2)&&(maxx > wx2)) || ((miny < wy1)&&(maxy < wy1)) || ((miny > wy2)&&(maxy > wy2)) )	return FALSE;
	else return TRUE;
}

BOOL  COb_schellipsefill::Is_On_Me(double fx, double fy)
{
	int width;
	width = nEDA_Line_Width[m_nBorderWidth];

	int delta = (int)(width/2) + nEDA_Selection_Offset;

	CRgn rgn;
	rgn.CreateEllipticRgn((int)(m_fCx - (m_fRadius_x + delta)), (int)(m_fCy - (m_fRadius_y + delta)), (int)(m_fCx + (m_fRadius_x + delta)), (int)(m_fCy + (m_fRadius_y + delta)) );

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

BOOL  COb_schellipsefill::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fminx, fminy, fmaxx, fmaxy;
	fminx = m_fCx - m_fRadius_x;
	fminy = m_fCy - m_fRadius_y;
	fmaxx = m_fCx + m_fRadius_x;
	fmaxy = m_fCy + m_fRadius_y;
	
	if((fminx < fx1)||(fmaxx > fx2)||(fminy < fy1)||(fmaxy > fy2)) return FALSE;
	return TRUE;
}

void COb_schellipsefill::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	CDlg_EditEllipseFill dlg;
	dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fCx);
	dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fCy);
	dlg.m_csEditRadiusX = pDoc->Convert_XY_To_String(m_fRadius_x);
	dlg.m_csEditRadiusY = pDoc->Convert_XY_To_String(m_fRadius_y);

	dlg.m_bCheckDrawSolid = m_bFillSolid;
	dlg.m_bCheckTransparent = m_bTransparent;
	dlg.m_nComboWidth = m_nBorderWidth;
	dlg.m_nFillColor = m_nFillColor;
	dlg.m_nBorderColor = m_nBorderColor;
	
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bCheckGlobal == TRUE)
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
							{
								COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
								if((pellipsefill->m_nDiagram != nDiagram)||(pellipsefill->m_nFlag != FLAG_NORMAL))	continue;
								if(pellipsefill == this) continue; 

								if( (((subdlg.m_bCheckColor == TRUE)&&(pellipsefill->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckWidth == TRUE)&&(pellipsefill->m_nBorderWidth == m_nBorderWidth))||(subdlg.m_bCheckWidth == FALSE)) && 
									(((subdlg.m_bCheckFillColor == TRUE)&&(pellipsefill->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
									(((subdlg.m_bCheckTransparent == TRUE)&&(pellipsefill->m_bTransparent == m_bTransparent))||(subdlg.m_bCheckTransparent == FALSE)) &&
									(((subdlg.m_bCheckSolid == TRUE)&&(pellipsefill->m_bFillSolid == m_bFillSolid))||(subdlg.m_bCheckSolid == FALSE)) )
								{
									if( (pellipsefill->m_nBorderColor == m_nBorderColor)||(pellipsefill->m_nBorderWidth == m_nBorderWidth)||(pellipsefill->m_nFillColor == m_nFillColor)||\
										(pellipsefill->m_bTransparent == m_bTransparent)||(pellipsefill->m_bFillSolid == m_bFillSolid) )
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
										pundo->m_nDiagram = nDiagram;
										pundo->m_nPos = posbak;
										pundo->schellipsefill.Copy(pellipsefill);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(pellipsefill->m_nBorderColor == m_nBorderColor) pellipsefill->m_nBorderColor = dlg.m_nBorderColor;
									if(pellipsefill->m_nBorderWidth == m_nBorderWidth) pellipsefill->m_nBorderWidth = dlg.m_nComboWidth;
									if(pellipsefill->m_nFillColor == m_nFillColor) pellipsefill->m_nFillColor = dlg.m_nFillColor;
									if(pellipsefill->m_bTransparent == m_bTransparent) pellipsefill->m_bTransparent = dlg.m_bCheckTransparent;
									if(pellipsefill->m_bFillSolid == m_bFillSolid) pellipsefill->m_bFillSolid = dlg.m_bCheckDrawSolid;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
						{
							COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
							if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag != FLAG_NORMAL))	continue;
							if(pellipsefill == this) continue; 

							if( (((subdlg.m_bCheckColor == TRUE)&&(pellipsefill->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(pellipsefill->m_nBorderWidth == m_nBorderWidth))||(subdlg.m_bCheckWidth == FALSE)) && 
								(((subdlg.m_bCheckFillColor == TRUE)&&(pellipsefill->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
								(((subdlg.m_bCheckTransparent == TRUE)&&(pellipsefill->m_bTransparent == m_bTransparent))||(subdlg.m_bCheckTransparent == FALSE)) &&
								(((subdlg.m_bCheckSolid == TRUE)&&(pellipsefill->m_bFillSolid == m_bFillSolid))||(subdlg.m_bCheckSolid == FALSE)) )
							{
								if( (pellipsefill->m_nBorderColor == m_nBorderColor)||(pellipsefill->m_nBorderWidth == m_nBorderWidth)||(pellipsefill->m_nFillColor == m_nFillColor)||\
									(pellipsefill->m_bTransparent == m_bTransparent)||(pellipsefill->m_bFillSolid == m_bFillSolid) )
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schellipsefill.Copy(pellipsefill);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

								}
								if(pellipsefill->m_nBorderColor == m_nBorderColor) pellipsefill->m_nBorderColor = dlg.m_nBorderColor;
								if(pellipsefill->m_nBorderWidth == m_nBorderWidth) pellipsefill->m_nBorderWidth = dlg.m_nComboWidth;
								if(pellipsefill->m_nFillColor == m_nFillColor) pellipsefill->m_nFillColor = dlg.m_nFillColor;
								if(pellipsefill->m_bTransparent == m_bTransparent) pellipsefill->m_bTransparent = dlg.m_bCheckTransparent;
								if(pellipsefill->m_bFillSolid == m_bFillSolid) pellipsefill->m_bFillSolid = dlg.m_bCheckDrawSolid;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
						{
							COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
							if((pellipsefill->m_bSelection != TRUE)||(pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag != FLAG_NORMAL))	continue;
							if(pellipsefill == this) continue; //排除当前ellipsefill

							if( (((subdlg.m_bCheckColor == TRUE)&&(pellipsefill->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(pellipsefill->m_nBorderWidth == m_nBorderWidth))||(subdlg.m_bCheckWidth == FALSE)) && 
								(((subdlg.m_bCheckFillColor == TRUE)&&(pellipsefill->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
								(((subdlg.m_bCheckTransparent == TRUE)&&(pellipsefill->m_bTransparent == m_bTransparent))||(subdlg.m_bCheckTransparent == FALSE)) &&
								(((subdlg.m_bCheckSolid == TRUE)&&(pellipsefill->m_bFillSolid == m_bFillSolid))||(subdlg.m_bCheckSolid == FALSE)) )
							{
								if( (pellipsefill->m_nBorderColor == m_nBorderColor)||(pellipsefill->m_nBorderWidth == m_nBorderWidth)||(pellipsefill->m_nFillColor == m_nFillColor)||\
									(pellipsefill->m_bTransparent == m_bTransparent)||(pellipsefill->m_bFillSolid == m_bFillSolid) )
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schellipsefill.Copy(pellipsefill);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								}
								if(pellipsefill->m_nBorderColor == m_nBorderColor) pellipsefill->m_nBorderColor = dlg.m_nBorderColor;
								if(pellipsefill->m_nBorderWidth == m_nBorderWidth) pellipsefill->m_nBorderWidth = dlg.m_nComboWidth;
								if(pellipsefill->m_nFillColor == m_nFillColor) pellipsefill->m_nFillColor = dlg.m_nFillColor;
								if(pellipsefill->m_bTransparent == m_bTransparent) pellipsefill->m_bTransparent = dlg.m_bCheckTransparent;
								if(pellipsefill->m_bFillSolid == m_bFillSolid) pellipsefill->m_bFillSolid = dlg.m_bCheckDrawSolid;
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
				pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schellipsefill.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);


				m_fCx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
				m_fCy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
				m_fRadius_x = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusX);
				m_fRadius_y = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusY);
				m_bFillSolid = dlg.m_bCheckDrawSolid;
				m_bTransparent = dlg.m_bCheckTransparent;
				m_nBorderWidth = dlg.m_nComboWidth;
				m_nFillColor = dlg.m_nFillColor;
				m_nBorderColor = dlg.m_nBorderColor;

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
			pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->schellipsefill.Copy(this);
				
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

			m_fCx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
			m_fCy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
			m_fRadius_x = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusX);
			m_fRadius_y = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusY);
			m_bFillSolid = dlg.m_bCheckDrawSolid;
			m_bTransparent = dlg.m_bCheckTransparent;
			m_nBorderWidth = dlg.m_nComboWidth;
			m_nFillColor = dlg.m_nFillColor;
			m_nBorderColor = dlg.m_nBorderColor;
		}
	}
}

void COb_schellipsefill::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	int flag;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动椭圆: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	Draw(pDC, pV);

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schellipsefill aellipsefill;
	aellipsefill.pDocument = pDoc;
	aellipsefill.m_fCx = m_fCx;
	aellipsefill.m_fCy = m_fCy;
	aellipsefill.m_fRadius_x = m_fRadius_x;
	aellipsefill.m_fRadius_y = m_fRadius_y;
	aellipsefill.m_nBorderWidth = m_nBorderWidth;
	aellipsefill.m_nBorderColor = m_nBorderColor;
	aellipsefill.m_nFillColor = m_nFillColor;
	aellipsefill.m_bFillSolid = m_bFillSolid;
	aellipsefill.m_bTransparent = FALSE;
	aellipsefill.m_nDiagram = m_nDiagram;
	aellipsefill.Draw(pDC, pV);

	pView->m_fCurrentSnapX = aellipsefill.m_fCx;
	pView->m_fCurrentSnapY = aellipsefill.m_fCy;
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
			aellipsefill.Draw(pDC, pV);
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
				aellipsefill.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			aellipsefill.m_fCx = pView->m_fCurrentSnapX;
			aellipsefill.m_fCy = pView->m_fCurrentSnapY;

			if((flag == 1)||(flag == 3)) 
			{
				aellipsefill.m_fRadius_x = m_fRadius_y;
				aellipsefill.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aellipsefill.m_fRadius_x = m_fRadius_x;
				aellipsefill.m_fRadius_y = m_fRadius_y;
			}

			pView->CloseMouseCursor();
			aellipsefill.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			aellipsefill.Draw(pDC, pV);
			pView->OpenMouseCursor();

			aellipsefill.m_fCx = pView->m_fCurrentSnapX;
			aellipsefill.m_fCy = pView->m_fCurrentSnapY;

			if((flag == 1)||(flag == 3)) 
			{
				aellipsefill.m_fRadius_x = m_fRadius_y;
				aellipsefill.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aellipsefill.m_fRadius_x = m_fRadius_x;
				aellipsefill.m_fRadius_y = m_fRadius_y;
			}

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->m_fX1 = m_fCx;
			pundo->m_fY1 = m_fCy;
			pundo->m_fRx = m_fRadius_x;
			pundo->m_fRy = m_fRadius_y;
			
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fCx = aellipsefill.m_fCx;
			m_fCy = aellipsefill.m_fCy;
			m_fRadius_x = aellipsefill.m_fRadius_x;
			m_fRadius_y = aellipsefill.m_fRadius_y;
			
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aellipsefill.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			aellipsefill.m_fCx = pView->m_fCurrentSnapX;
			aellipsefill.m_fCy = pView->m_fCurrentSnapY;

			if((flag == 1)||(flag == 3)) 
			{
				aellipsefill.m_fRadius_x = m_fRadius_y;
				aellipsefill.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aellipsefill.m_fRadius_x = m_fRadius_x;
				aellipsefill.m_fRadius_y = m_fRadius_y;
			}

			aellipsefill.Draw(pDC, pV);

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

BOOL COb_schellipsefill::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;

	//COb_schtext* pschtext;
	char szHelp[] = "复制椭圆: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schellipsefill aellipsefill;
	aellipsefill.pDocument = pDoc;
	aellipsefill.m_fCx = pView->m_fCurrentSnapX;
	aellipsefill.m_fCy = pView->m_fCurrentSnapY;
	aellipsefill.m_fRadius_x = m_fRadius_x;
	aellipsefill.m_fRadius_y = m_fRadius_y;
	aellipsefill.m_nBorderWidth = m_nBorderWidth;
	aellipsefill.m_nBorderColor = m_nBorderColor;
	aellipsefill.m_nFillColor = m_nFillColor;
	aellipsefill.m_bFillSolid = m_bFillSolid;
	aellipsefill.m_bTransparent = FALSE;
	aellipsefill.m_nDiagram = m_nDiagram;

	aellipsefill.Draw(pDC, pV);
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
			aellipsefill.Draw(pDC, pV);
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
				aellipsefill.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			aellipsefill.m_fCx = pView->m_fCurrentSnapX;
			aellipsefill.m_fCy = pView->m_fCurrentSnapY;

			if((flag == 1)||(flag == 3)) 
			{
				aellipsefill.m_fRadius_x = m_fRadius_y;
				aellipsefill.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aellipsefill.m_fRadius_x = m_fRadius_x;
				aellipsefill.m_fRadius_y = m_fRadius_y;
			}

			pView->CloseMouseCursor();
			aellipsefill.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			aellipsefill.Draw(pDC, pV);
			pView->OpenMouseCursor();

			aellipsefill.m_fCx = pView->m_fCurrentSnapX;
			aellipsefill.m_fCy = pView->m_fCurrentSnapY;

			if((flag == 1)||(flag == 3)) 
			{
				aellipsefill.m_fRadius_x = m_fRadius_y;
				aellipsefill.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aellipsefill.m_fRadius_x = m_fRadius_x;
				aellipsefill.m_fRadius_y = m_fRadius_y;
			}

			m_fCx = aellipsefill.m_fCx;
			m_fCy = aellipsefill.m_fCy;
			m_fRadius_x = aellipsefill.m_fRadius_x;
			m_fRadius_y = aellipsefill.m_fRadius_y;
			
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aellipsefill.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			aellipsefill.m_fCx = pView->m_fCurrentSnapX;
			aellipsefill.m_fCy = pView->m_fCurrentSnapY;

			if((flag == 1)||(flag == 3)) 
			{
				aellipsefill.m_fRadius_x = m_fRadius_y;
				aellipsefill.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aellipsefill.m_fRadius_x = m_fRadius_x;
				aellipsefill.m_fRadius_y = m_fRadius_y;
			}

			aellipsefill.Draw(pDC, pV);

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

void COb_schellipsefill::Copy(COb_schellipsefill* pschellipsefill)
{
	m_fCx = pschellipsefill->m_fCx;
	m_fCy = pschellipsefill->m_fCy;
	m_fRadius_x = pschellipsefill->m_fRadius_x;
	m_fRadius_y = pschellipsefill->m_fRadius_y;

	m_nBorderWidth = pschellipsefill->m_nBorderWidth;
	m_nBorderColor = pschellipsefill->m_nBorderColor;
	m_nFillColor = pschellipsefill->m_nFillColor;
	m_bFillSolid = pschellipsefill->m_bFillSolid;
	m_bTransparent = pschellipsefill->m_bTransparent;

	m_nOwnerPartID = pschellipsefill->m_nOwnerPartID;
	m_nDisplayMode = pschellipsefill->m_nDisplayMode;
	m_posParent = pschellipsefill->m_posParent;

	m_nDiagram = pschellipsefill->m_nDiagram;
	m_bSelection = pschellipsefill->m_bSelection;
	m_nFlag = pschellipsefill->m_nFlag;

	pDocument = pschellipsefill->pDocument;
}