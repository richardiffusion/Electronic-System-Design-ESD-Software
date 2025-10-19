#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditRoundrect.h"
#include "Dlg_Global_Fill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_schroundrect, CObject, 0)
COb_schroundrect::COb_schroundrect()
{
	// This empty constructor should be used by serialization only
	m_nFlag = 0 ;
	m_bSelection = FALSE;
	m_nOwnerPartID = 0;
	m_nDisplayMode = 0;
	m_posParent = NULL;

	pDocument = NULL;
}


void COb_schroundrect::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fX1;
		ar << m_fY1;
		ar << m_fX2;
		ar << m_fY2;
		
		ar << m_fRadius_x;
		ar << m_fRadius_y;

		ar << m_nBorderWidth;
		ar << m_nFillColor;
		ar << m_nBorderColor;
		ar << m_bFillSolid;
		
		ar << m_nOwnerPartID;
		ar << m_nDisplayMode;

		ar << m_nDiagram;
		ar << m_bSelection;
	}
	else
	{
		ar >> m_fX1;
		ar >> m_fY1;
		ar >> m_fX2;
		ar >> m_fY2;
		
		ar >> m_fRadius_x;
		ar >> m_fRadius_y;

		ar >> m_nBorderWidth;
		ar >> m_nFillColor;
		ar >> m_nBorderColor;
		ar >> m_bFillSolid;
		
		ar >> m_nOwnerPartID;
		ar >> m_nDisplayMode;

		ar >> m_nDiagram;
		ar >> m_bSelection;	
	}
}

void COb_schroundrect::Draw(CDC* pDC, CView* pV)
{
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
	}

	int width;

	width = nEDA_Line_Width[m_nBorderWidth];

	width = (int)(width/pView->m_fScale);
	if(width < 1) width = 1;
	
	if(pView->m_nPrint_Flag == 1) width = (int)(width*nEDA_Print_Xdpi/1000*fEDA_Print_Scale);
	if(width < 1) width = 1;

	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,width,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
 
	CBrush fillbrush;
	if(m_bFillSolid == FALSE) fillbrush.CreateSolidBrush(RGB(255,255,255));
	else fillbrush.CreateSolidBrush(fillcolor);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	int x1,y1,x2,y2,rx,ry;
	pView->Get_VPort_XY(&x1, &y1, m_fX1, m_fY1);
	pView->Get_VPort_XY(&x2, &y2, m_fX2, m_fY2);
	rx = (int)(2*m_fRadius_x/pView->m_fScale);
	ry = (int)(2*m_fRadius_y/pView->m_fScale);

	if(pView->m_nPrint_Flag == 1)
	{
		x1 = (int)(((double)x1*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		y1 = (int)(((double)y1*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
		x2 = (int)(((double)x2*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		y2 = (int)(((double)y2*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
		rx = (int)(((double)rx*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		ry = (int)(((double)ry*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
	}

		
	pDC->RoundRect(x1,y1,x2,y2,rx,ry);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

	
void COb_schroundrect::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	COLORREF color, fillcolor;

	color = m_nBorderColor;
	fillcolor = m_nFillColor;
	
	int width;

	width = nEDA_Line_Width[m_nBorderWidth];

	width = (int)(width/scale);
	if(width < 1) width = 1;

	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,width,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
 
	CBrush fillbrush;
	if(m_bFillSolid == FALSE) fillbrush.CreateSolidBrush(RGB(255,255,255));
	else fillbrush.CreateSolidBrush(fillcolor);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	int x1,y1,x2,y2,rx,ry;

	x1 = (int)((m_fX1 - origin_x)/scale);
	y1 = nEy - (int)((m_fY1 - origin_y)/scale);
	//pView->Get_VPort_XY(&x2, &y2, m_fX2, m_fY2);
	x2 = (int)((m_fX2 - origin_x)/scale);
	y2 = nEy - (int)((m_fY2 - origin_y)/scale);

	rx = (int)(2*m_fRadius_x/scale);
	ry = (int)(2*m_fRadius_y/scale);

	pDC->RoundRect(x1,y1,x2,y2,rx,ry);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

	void COb_schroundrect::DrawSelection(CDC* pDC, CView* pV)
{
	int x1, y1, x2, y2, rx, ry;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	CPen arcpen;
	arcpen.CreatePen(PS_DOT, 1, nEDA_SelectionColor);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	pView->Get_VPort_XY(&x1,&y1,m_fX1,m_fY1);
	pView->Get_VPort_XY(&x2,&y2,m_fX2,m_fY2);
	rx = (int)(m_fRadius_x/pView->m_fScale);
	ry = (int)(m_fRadius_y/pView->m_fScale);

	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y1);
	pDC->LineTo(x2,y2);
	pDC->LineTo(x1,y2);
	pDC->LineTo(x1,y1);

	pDC->MoveTo(x1,y1);
	pDC->LineTo(x1+rx,y1);
	pDC->LineTo(x1+rx,y1-ry);
	pDC->LineTo(x1,y1-ry);
	pDC->LineTo(x1,y1);

	pDC->MoveTo(x1,y2);
	pDC->LineTo(x1+rx,y2);
	pDC->LineTo(x1+rx,y2+ry);
	pDC->LineTo(x1,y2+ry);
	pDC->LineTo(x1,y2);
	
	pDC->MoveTo(x2,y1);
	pDC->LineTo(x2-rx,y1);
	pDC->LineTo(x2-rx,y1-ry);
	pDC->LineTo(x2,y1-ry);
	pDC->LineTo(x2,y1);	

	pDC->MoveTo(x2,y2);
	pDC->LineTo(x2-rx,y2);
	pDC->LineTo(x2-rx,y2+ry);
	pDC->LineTo(x2,y2+ry);
	pDC->LineTo(x2,y2);

	pView->DrawSelectionKeyPoint(x1, y1, pDC);
	pView->DrawSelectionKeyPoint(x1, y2, pDC);
	pView->DrawSelectionKeyPoint(x2, y1, pDC);
	pView->DrawSelectionKeyPoint(x2, y2, pDC);

	pView->DrawSelectionKeyPoint((x1+x2)/2, y1, pDC);
	pView->DrawSelectionKeyPoint((x1+x2)/2, y2, pDC);
	pView->DrawSelectionKeyPoint(x1, (y1+y2)/2, pDC);
	pView->DrawSelectionKeyPoint(x2, (y1+y2)/2, pDC);

	pView->DrawSelectionKeyPoint(x1+rx, y1-ry, pDC);
	pView->DrawSelectionKeyPoint(x1+rx, y2+ry, pDC);
	pView->DrawSelectionKeyPoint(x2-rx, y1-ry, pDC);
	pView->DrawSelectionKeyPoint(x2-rx, y2+ry, pDC);
	
	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}

BOOL COb_schroundrect::PickMe(CView* pV)
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
	
	int x1,y1,x2,y2;
	x1 = (int)m_fX1;
	y1 = (int)m_fY1;
	x2 = (int)m_fX2;
	y2 = (int)m_fY2;

	if( ((x1 < wx1)&&(x2 < wx1)) || ((x1 > wx2)&&(x2 > wx2)) || ((y1 < wy1)&&(y2 < wy1)) || ((y1 > wy2)&&(y2 > wy2)) )	return FALSE;
	else return TRUE;
}

BOOL  COb_schroundrect::Is_On_Me(double fx, double fy)
{
	//if(diagram != m_nDiagram) return FALSE;

	CRgn   rgn;

	rgn.CreateRoundRectRgn( (int)m_fX1, (int)m_fY1, (int)m_fX2, (int)m_fY2, (int)(2*m_fRadius_x), (int)(2*m_fRadius_y));

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

BOOL  COb_schroundrect::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fminx, fminy, fmaxx, fmaxy;
	fminx = MIN(m_fX1, m_fX2);
	fminy = MIN(m_fY1, m_fY2);
	fmaxx = MAX(m_fX1, m_fX2);
	fmaxy = MAX(m_fY1, m_fY2);
	
	if((fminx < fx1)||(fmaxx > fx2)||(fminy < fy1)||(fmaxy > fy2)) return FALSE;
	return TRUE;
}

void COb_schroundrect::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_EditRoundrect dlg;

	dlg.m_csEditLocationX1 = pDoc->Convert_XY_To_String(m_fX1);
	dlg.m_csEditLocationY1 = pDoc->Convert_XY_To_String(m_fY1);
	dlg.m_csEditLocationX2 = pDoc->Convert_XY_To_String(m_fX2);
	dlg.m_csEditLocationY2 = pDoc->Convert_XY_To_String(m_fY2);
	dlg.m_csEditRadiusX = pDoc->Convert_XY_To_String(m_fRadius_x);
	dlg.m_csEditRadiusY = pDoc->Convert_XY_To_String(m_fRadius_y);
	dlg.m_bCheckDrawSolid = m_bFillSolid;
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
							{
								COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
								if((proundrect->m_nDiagram != nDiagram)||(proundrect->m_nFlag != FLAG_NORMAL))	continue;
								if(proundrect == this) continue;

								if( (((subdlg.m_bCheckColor == TRUE)&&(proundrect->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckWidth == TRUE)&&(proundrect->m_nBorderWidth == m_nBorderWidth))||(subdlg.m_bCheckWidth == FALSE)) && 
									(((subdlg.m_bCheckFillColor == TRUE)&&(proundrect->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
									(((subdlg.m_bCheckSolid == TRUE)&&(proundrect->m_bFillSolid == m_bFillSolid))||(subdlg.m_bCheckSolid == FALSE)) )
								{
									if( (proundrect->m_nBorderColor == m_nBorderColor)||(proundrect->m_nBorderWidth == m_nBorderWidth)||\
										(proundrect->m_nFillColor == m_nFillColor)||(proundrect->m_bFillSolid == m_bFillSolid) )
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
										pundo->m_nDiagram = nDiagram;
										pundo->m_nPos = posbak;
										pundo->schroundrect.Copy(proundrect);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(proundrect->m_nBorderColor == m_nBorderColor) proundrect->m_nBorderColor = dlg.m_nBorderColor;
									if(proundrect->m_nBorderWidth == m_nBorderWidth) proundrect->m_nBorderWidth = dlg.m_nComboWidth;
									if(proundrect->m_nFillColor == m_nFillColor) proundrect->m_nFillColor = dlg.m_nFillColor;
									if(proundrect->m_bFillSolid == m_bFillSolid) proundrect->m_bFillSolid = dlg.m_bCheckDrawSolid;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
						{
							COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
							if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag != FLAG_NORMAL))	continue;
							if(proundrect == this) continue; 

							if( (((subdlg.m_bCheckColor == TRUE)&&(proundrect->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(proundrect->m_nBorderWidth == m_nBorderWidth))||(subdlg.m_bCheckWidth == FALSE)) && 
								(((subdlg.m_bCheckFillColor == TRUE)&&(proundrect->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
								(((subdlg.m_bCheckSolid == TRUE)&&(proundrect->m_bFillSolid == m_bFillSolid))||(subdlg.m_bCheckSolid == FALSE)) )
							{
								if( (proundrect->m_nBorderColor == m_nBorderColor)||(proundrect->m_nBorderWidth == m_nBorderWidth)||\
									(proundrect->m_nFillColor == m_nFillColor)||(proundrect->m_bFillSolid == m_bFillSolid) )
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schroundrect.Copy(proundrect);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

								}
								if(proundrect->m_nBorderColor == m_nBorderColor) proundrect->m_nBorderColor = dlg.m_nBorderColor;
								if(proundrect->m_nBorderWidth == m_nBorderWidth) proundrect->m_nBorderWidth = dlg.m_nComboWidth;
								if(proundrect->m_nFillColor == m_nFillColor) proundrect->m_nFillColor = dlg.m_nFillColor;
								if(proundrect->m_bFillSolid == m_bFillSolid) proundrect->m_bFillSolid = dlg.m_bCheckDrawSolid;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
						{
							COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
							if((proundrect->m_bSelection != TRUE)||(proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag != FLAG_NORMAL))	continue;
							if(proundrect == this) continue; 

							if( (((subdlg.m_bCheckColor == TRUE)&&(proundrect->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(proundrect->m_nBorderWidth == m_nBorderWidth))||(subdlg.m_bCheckWidth == FALSE)) && 
								(((subdlg.m_bCheckFillColor == TRUE)&&(proundrect->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
								(((subdlg.m_bCheckSolid == TRUE)&&(proundrect->m_bFillSolid == m_bFillSolid))||(subdlg.m_bCheckSolid == FALSE)) )
							{
								if( (proundrect->m_nBorderColor == m_nBorderColor)||(proundrect->m_nBorderWidth == m_nBorderWidth)||\
									(proundrect->m_nFillColor == m_nFillColor)||(proundrect->m_bFillSolid == m_bFillSolid) )
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schroundrect.Copy(proundrect);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								}
								if(proundrect->m_nBorderColor == m_nBorderColor) proundrect->m_nBorderColor = dlg.m_nBorderColor;
								if(proundrect->m_nBorderWidth == m_nBorderWidth) proundrect->m_nBorderWidth = dlg.m_nComboWidth;
								if(proundrect->m_nFillColor == m_nFillColor) proundrect->m_nFillColor = dlg.m_nFillColor;
								if(proundrect->m_bFillSolid == m_bFillSolid) proundrect->m_bFillSolid = dlg.m_bCheckDrawSolid;
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
				pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schroundrect.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				m_fX1 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX1);
				m_fY1 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY1);
				m_fX2 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX2);
				m_fY2 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY2);
			
				double fminx, fminy, fmaxx, fmaxy;
				fminx = MIN(m_fX1, m_fX2);
				fmaxx = MAX(m_fX1, m_fX2);
				fminy = MIN(m_fY1, m_fY2);
				fmaxy = MAX(m_fY1, m_fY2);
				m_fX1 = fminx;
				m_fY1 = fminy;
				m_fX2 = fmaxx;
				m_fY2 = fmaxy;

				m_fRadius_x = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusX);
				m_fRadius_y = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusY);
				m_bFillSolid = dlg.m_bCheckDrawSolid;
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
			pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->schroundrect.Copy(this);
				
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

			m_fX1 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX1);
			m_fY1 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY1);
			m_fX2 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX2);
			m_fY2 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY2);
		
			double fminx, fminy, fmaxx, fmaxy;
			fminx = MIN(m_fX1, m_fX2);
			fmaxx = MAX(m_fX1, m_fX2);
			fminy = MIN(m_fY1, m_fY2);
			fmaxy = MAX(m_fY1, m_fY2);
			m_fX1 = fminx;
			m_fY1 = fminy;
			m_fX2 = fmaxx;
			m_fY2 = fmaxy;

			m_fRadius_x = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusX);
			m_fRadius_y = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusY);
			m_bFillSolid = dlg.m_bCheckDrawSolid;
			m_nBorderWidth = dlg.m_nComboWidth;
			m_nFillColor = dlg.m_nFillColor;
			m_nBorderColor = dlg.m_nBorderColor;
		}
	}
}

void COb_schroundrect::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE; 
	
	char szHelp[] = "移动圆方形: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV);

	x0 = m_fX1;
	y0 = m_fY1;
	
	pView->m_fCurrentSnapX = x0;
	pView->m_fCurrentSnapY = y0;
	pView->Restore_LargeCrossXY();

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schroundrect aroundrect;
	aroundrect.pDocument = pDoc;
	aroundrect.m_fX1 = m_fX1;
	aroundrect.m_fY1 = m_fY1;
	aroundrect.m_fX2 = m_fX2;
	aroundrect.m_fY2 = m_fY2;
	aroundrect.m_fRadius_x = m_fRadius_x;
	aroundrect.m_fRadius_y = m_fRadius_y;
	aroundrect.m_nBorderWidth = m_nBorderWidth;
	aroundrect.m_nBorderColor = m_nBorderColor;
	aroundrect.m_nFillColor = m_nFillColor;
	aroundrect.m_bFillSolid = m_bFillSolid;
	aroundrect.m_nDiagram = m_nDiagram;

	aroundrect.Draw(pDC, pV);
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
			aroundrect.Draw(pDC, pV);
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
				aroundrect.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, flag*90, FALSE);
			aroundrect.m_fX1 = fx + x0 + deltax;
			aroundrect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, flag*90, FALSE);
			aroundrect.m_fX2 = fx + x0 + deltax;
			aroundrect.m_fY2 = fy + y0 + deltay;

			if((flag == 1)||(flag == 3)) 
			{
				aroundrect.m_fRadius_x = m_fRadius_y;
				aroundrect.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aroundrect.m_fRadius_x = m_fRadius_x;
				aroundrect.m_fRadius_y = m_fRadius_y;
			}

			pView->CloseMouseCursor();
			aroundrect.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			aroundrect.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, flag*90, FALSE);
			aroundrect.m_fX1 = fx + x0 + deltax;
			aroundrect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, flag*90, FALSE);
			aroundrect.m_fX2 = fx + x0 + deltax;
			aroundrect.m_fY2 = fy + y0 + deltay;

			if((flag == 1)||(flag == 3)) 
			{
				aroundrect.m_fRadius_x = m_fRadius_y;
				aroundrect.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aroundrect.m_fRadius_x = m_fRadius_x;
				aroundrect.m_fRadius_y = m_fRadius_y;
			}

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->m_fX1 = m_fX1;
			pundo->m_fY1 = m_fY1;
			pundo->m_fX2 = m_fX2;
			pundo->m_fY2 = m_fY2;
			pundo->m_fRx = m_fRadius_x;
			pundo->m_fRy = m_fRadius_y;
			
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fX1 = aroundrect.m_fX1;
			m_fY1 = aroundrect.m_fY1;
			m_fX2 = aroundrect.m_fX2;
			m_fY2 = aroundrect.m_fY2;
			m_fRadius_x = aroundrect.m_fRadius_x;
			m_fRadius_y = aroundrect.m_fRadius_y;

			double fminx, fminy, fmaxx, fmaxy;
			fminx = MIN(m_fX1, m_fX2);
			fmaxx = MAX(m_fX1, m_fX2);
			fminy = MIN(m_fY1, m_fY2);
			fmaxy = MAX(m_fY1, m_fY2);
			m_fX1 = fminx;
			m_fY1 = fminy;
			m_fX2 = fmaxx;
			m_fY2 = fmaxy;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aroundrect.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, flag*90, FALSE);
			aroundrect.m_fX1 = fx + x0 + deltax;
			aroundrect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, flag*90, FALSE);
			aroundrect.m_fX2 = fx + x0 + deltax;
			aroundrect.m_fY2 = fy + y0 + deltay;

			if((flag == 1)||(flag == 3)) 
			{
				aroundrect.m_fRadius_x = m_fRadius_y;
				aroundrect.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aroundrect.m_fRadius_x = m_fRadius_x;
				aroundrect.m_fRadius_y = m_fRadius_y;
			}

			aroundrect.Draw(pDC, pV);

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

BOOL COb_schroundrect::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	
	char szHelp[] = "复制圆方形: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;


	x0 = m_fX1; //pView->m_fCurrentSnapX;
	y0 = m_fY1; //pView->m_fCurrentSnapY;
	
	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	//复制对象
	COb_schroundrect aroundrect;
	aroundrect.pDocument = pDoc;
	aroundrect.m_fX1 = m_fX1;
	aroundrect.m_fY1 = m_fY1;
	aroundrect.m_fX2 = m_fX2;
	aroundrect.m_fY2 = m_fY2;
	aroundrect.m_fRadius_x = m_fRadius_x;
	aroundrect.m_fRadius_y = m_fRadius_y;
	aroundrect.m_nBorderWidth = m_nBorderWidth;
	aroundrect.m_nBorderColor = m_nBorderColor;
	aroundrect.m_nFillColor = m_nFillColor;
	aroundrect.m_bFillSolid = m_bFillSolid;
	aroundrect.m_nDiagram = m_nDiagram;

	aroundrect.Draw(pDC, pV);
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
			aroundrect.Draw(pDC, pV);
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
				aroundrect.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, flag*90, FALSE);
			aroundrect.m_fX1 = fx + x0 + deltax;
			aroundrect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, flag*90, FALSE);
			aroundrect.m_fX2 = fx + x0 + deltax;
			aroundrect.m_fY2 = fy + y0 + deltay;

			if((flag == 1)||(flag == 3)) 
			{
				aroundrect.m_fRadius_x = m_fRadius_y;
				aroundrect.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aroundrect.m_fRadius_x = m_fRadius_x;
				aroundrect.m_fRadius_y = m_fRadius_y;
			}

			pView->CloseMouseCursor();
			aroundrect.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			aroundrect.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, flag*90, FALSE);
			aroundrect.m_fX1 = fx + x0 + deltax;
			aroundrect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, flag*90, FALSE);
			aroundrect.m_fX2 = fx + x0 + deltax;
			aroundrect.m_fY2 = fy + y0 + deltay;

			if((flag == 1)||(flag == 3)) 
			{
				aroundrect.m_fRadius_x = m_fRadius_y;
				aroundrect.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aroundrect.m_fRadius_x = m_fRadius_x;
				aroundrect.m_fRadius_y = m_fRadius_y;
			}

			m_fX1 = aroundrect.m_fX1;
			m_fY1 = aroundrect.m_fY1;
			m_fX2 = aroundrect.m_fX2;
			m_fY2 = aroundrect.m_fY2;
			m_fRadius_x = aroundrect.m_fRadius_x;
			m_fRadius_y = aroundrect.m_fRadius_y;

			double fminx, fminy, fmaxx, fmaxy;
			fminx = MIN(m_fX1, m_fX2);
			fmaxx = MAX(m_fX1, m_fX2);
			fminy = MIN(m_fY1, m_fY2);
			fmaxy = MAX(m_fY1, m_fY2);
			m_fX1 = fminx;
			m_fY1 = fminy;
			m_fX2 = fmaxx;
			m_fY2 = fmaxy;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aroundrect.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, flag*90, FALSE);
			aroundrect.m_fX1 = fx + x0 + deltax;
			aroundrect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, flag*90, FALSE);
			aroundrect.m_fX2 = fx + x0 + deltax;
			aroundrect.m_fY2 = fy + y0 + deltay;

			if((flag == 1)||(flag == 3)) 
			{
				aroundrect.m_fRadius_x = m_fRadius_y;
				aroundrect.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aroundrect.m_fRadius_x = m_fRadius_x;
				aroundrect.m_fRadius_y = m_fRadius_y;
			}

			aroundrect.Draw(pDC, pV);

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

void COb_schroundrect::Copy(COb_schroundrect* pschroundrect)
{
	m_fX1 = pschroundrect->m_fX1;
	m_fY1 = pschroundrect->m_fY1;
	m_fX2 = pschroundrect->m_fX2;
	m_fY2 = pschroundrect->m_fY2;
	m_fRadius_x = pschroundrect->m_fRadius_x;
	m_fRadius_y = pschroundrect->m_fRadius_y;
	m_nBorderWidth = pschroundrect->m_nBorderWidth;
	m_nFillColor = pschroundrect->m_nFillColor;
	m_nBorderColor = pschroundrect->m_nBorderColor;

	m_bFillSolid = pschroundrect->m_bFillSolid;

	m_nOwnerPartID = pschroundrect->m_nOwnerPartID;
	m_nDisplayMode = pschroundrect->m_nDisplayMode;
	m_posParent = pschroundrect->m_posParent;

	m_nDiagram = pschroundrect->m_nDiagram;
	m_bSelection = pschroundrect->m_bSelection;
	m_nFlag = pschroundrect->m_nFlag;

	pDocument = pschroundrect->pDocument;
}