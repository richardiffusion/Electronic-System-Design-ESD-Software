#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditEllipse.h"
#include "Dlg_Global_Bus.h"

IMPLEMENT_SERIAL(COb_schellipse, CObject, 0)
COb_schellipse::COb_schellipse()
{
	// This empty constructor should be used by serialization only
	m_nFlag = 0;
	m_bSelection = FALSE;
	m_nOwnerPartID = 0;
	m_nDisplayMode = 0;
	m_posParent = NULL;

	pDocument = NULL;
}



void COb_schellipse::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fCx;
		ar << m_fCy;
		ar << m_fRadius_x;
		ar << m_fRadius_y;
		ar << m_fSangle;
		ar << m_fEangle;
		ar << m_nWidth;
		ar << m_nColor;
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
		ar >> m_fSangle;
		ar >> m_fEangle;
		ar >> m_nWidth;
		ar >> m_nColor;
		ar >> m_nOwnerPartID;
		ar >> m_nDisplayMode;
		ar >> m_nDiagram;
		ar >> m_bSelection;	
	}
}

void COb_schellipse::Draw(CDC* pDC, CView* pV)
{
	int cx,cy;
	int rx, ry;
	COLORREF color;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	if(m_nFlag == FLAG_NORMAL) color = m_nColor;
	else if(m_nFlag == FLAG_DELETED) color = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor;


	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_Print_Color == 0) color = RGB(0,0,0);
		else if(nEDA_Print_Color == 2) color = RGB(GetGray(m_nColor), GetGray(m_nColor), GetGray(m_nColor));
	}


	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
	rx = (int)(m_fRadius_x/pView->m_fScale);
	ry = (int)(m_fRadius_y/pView->m_fScale);
	
	int width;
	width = nEDA_Line_Width[m_nWidth];

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

	CPen arcpen;
	arcpen.CreatePen(PS_SOLID, width, color);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	CRect rect;
	CPoint sp, ep;

	rect.left = cx - rx;
	rect.right = cx + rx;
	rect.top = cy - ry;
	rect.bottom = cy + ry;
	
	double alpha, ftemp;
	
	if(abs(m_fEangle - m_fSangle) == 360)
	{
		pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, (cx-rx), cy, (cx-rx), cy);
	}
	else
	{
		alpha = m_fSangle*fEDA_pi/180;
		ftemp = sqrt(rx*rx*sin(alpha)*sin(alpha) + ry*ry*cos(alpha)*cos(alpha));
		sp.x = (int)(cx + rx*ry*cos(alpha)/ftemp);
		sp.y = (int)(cy - rx*ry*sin(alpha)/ftemp);

		alpha = m_fEangle*fEDA_pi/180;
		ftemp = sqrt(rx*rx*sin(alpha)*sin(alpha) + ry*ry*cos(alpha)*cos(alpha));
		ep.x = (int)(cx + rx*ry*cos(alpha)/ftemp);
		ep.y = (int)(cy - rx*ry*sin(alpha)/ftemp);


		pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);
	}

	pDC->SelectObject(oldpen);
}

void COb_schellipse::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	int cx,cy;
	int rx, ry;
	COLORREF color;

	color = m_nColor;

	cx = (int)((m_fCx - origin_x)/scale);
	cy = nEy - (int)((m_fCy - origin_y)/scale);

	rx = (int)(m_fRadius_x/scale);
	ry = (int)(m_fRadius_y/scale);
	
	int width;
	width = nEDA_Line_Width[m_nWidth];

	width = (int)(width/scale);
	if(width < 1) width = 1;

	CPen arcpen;
	arcpen.CreatePen(PS_SOLID, width, color);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	CRect rect;
	CPoint sp, ep;

	rect.left = cx - rx;
	rect.right = cx + rx;
	rect.top = cy - ry;
	rect.bottom = cy + ry;
	
	double alpha, ftemp;
	if(abs(m_fEangle - m_fSangle) == 360)
	{
		pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, (cx-rx), cy, (cx-rx), cy);
	}
	else
	{
		alpha = m_fSangle*fEDA_pi/180;
		ftemp = sqrt(rx*rx*sin(alpha)*sin(alpha) + ry*ry*cos(alpha)*cos(alpha));
		sp.x = (int)(cx + rx*ry*cos(alpha)/ftemp);
		sp.y = (int)(cy - rx*ry*sin(alpha)/ftemp);

		alpha = m_fEangle*fEDA_pi/180;
		ftemp = sqrt(rx*rx*sin(alpha)*sin(alpha) + ry*ry*cos(alpha)*cos(alpha));
		ep.x = (int)(cx + rx*ry*cos(alpha)/ftemp);
		ep.y = (int)(cy - rx*ry*sin(alpha)/ftemp);

		pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);
	}

	pDC->SelectObject(oldpen);
}

void COb_schellipse::DrawSelection(CDC* pDC, CView* pV)
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

	CRect rect;
	CPoint sp, ep;

	rect.left = cx - rx;
	rect.right = cx + rx;
	rect.top = cy - ry;
	rect.bottom = cy + ry;
	
	double alpha, ftemp;
	
	alpha = m_fEangle*fEDA_pi/180;
	ftemp = sqrt(rx*rx*sin(alpha)*sin(alpha) + ry*ry*cos(alpha)*cos(alpha));
	sp.x = (int)(cx + rx*ry*cos(alpha)/ftemp);
	sp.y = (int)(cy - rx*ry*sin(alpha)/ftemp);

	alpha = m_fSangle*fEDA_pi/180;
	ftemp = sqrt(rx*rx*sin(alpha)*sin(alpha) + ry*ry*cos(alpha)*cos(alpha));
	ep.x = (int)(cx + rx*ry*cos(alpha)/ftemp);
	ep.y = (int)(cy - rx*ry*sin(alpha)/ftemp);

	pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);

	pDC->MoveTo(sp.x, sp.y);
	pDC->LineTo(cx, cy);
	pDC->LineTo(ep.x, ep.y);

	pView->DrawSelectionKeyPoint(sp.x, sp.y, pDC);
	pView->DrawSelectionKeyPoint(ep.x, ep.y, pDC);
	pView->DrawSelectionKeyPoint(cx + rx, cy, pDC);
	pView->DrawSelectionKeyPoint(cx, cy - ry, pDC);

	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}

BOOL COb_schellipse::PickMe(CView* pV)
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

BOOL  COb_schellipse::Is_On_Me(double fx, double fy)
{
	int width;
	width = nEDA_Line_Width[m_nWidth];

	int delta = (int)(width/2) + nEDA_Selection_Offset;

	CRgn rgn, rgn1, rgn2;
	rgn1.CreateEllipticRgn((int)(m_fCx - (m_fRadius_x - delta)), (int)(m_fCy - (m_fRadius_y - delta)), (int)(m_fCx + (m_fRadius_x - delta)), (int)(m_fCy + (m_fRadius_y - delta)) );
	rgn2.CreateEllipticRgn((int)(m_fCx - (m_fRadius_x + delta)), (int)(m_fCy - (m_fRadius_y + delta)), (int)(m_fCx + (m_fRadius_x + delta)), (int)(m_fCy + (m_fRadius_y + delta)) );
	rgn.CreateRectRgn(0,0,50,50);

	rgn.CombineRgn(&rgn1, &rgn2, RGN_XOR);

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) 
	{
		double alpha;
		if(fx == m_fCx)
		{
			if(fy > m_fCy) alpha = 90.0f;
			else alpha = 270.0f;
		}
		else if(fy == m_fCy) 
		{
			if(fx > m_fCx) alpha = 0;
			else alpha = 180;
		}
		else
		{
			alpha = 180/fEDA_pi*atan((fy - m_fCy)/(fx - m_fCx));
			if(fx < m_fCx) alpha += 180;
		}
		if(alpha < 0) alpha += 360;

		if(m_fEangle == m_fSangle) return TRUE;
		else if(m_fEangle > m_fSangle)	
		{
			if((alpha > m_fSangle)&&(alpha < m_fEangle)) return TRUE;
		}
		else
		{
			if((alpha > m_fSangle)||(alpha < m_fEangle)) return TRUE;
		}
	}
	
	return FALSE;
}

BOOL  COb_schellipse::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fminx, fminy, fmaxx, fmaxy;
	fminx = m_fCx - m_fRadius_x;
	fminy = m_fCy - m_fRadius_y;
	fmaxx = m_fCx + m_fRadius_x;
	fmaxy = m_fCy + m_fRadius_y;
	
	if((fminx < fx1)||(fmaxx > fx2)||(fminy < fy1)||(fmaxy > fy2)) return FALSE;
	return TRUE;
}

void COb_schellipse::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_EditEllipse dlg;
	TRACE("ok\n\n\n");	
	dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fCx);
	dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fCy);
	dlg.m_csEditRadiusX = pDoc->Convert_XY_To_String(m_fRadius_x);
	dlg.m_csEditRadiusY = pDoc->Convert_XY_To_String(m_fRadius_y);
	dlg.m_fEditStartAngle = m_fSangle;
	dlg.m_fEditEndAngle = m_fEangle;
	dlg.m_nComboWidth = m_nWidth;
	dlg.m_nColor = m_nColor;
	
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bCheckGlobal == TRUE)
		{
			CDlg_Global_Bus subdlg;
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
							{
								COb_schellipse* pellipse = (COb_schellipse*)pobject;
								if((pellipse->m_nDiagram != nDiagram)||(pellipse->m_nFlag != FLAG_NORMAL))	continue;
								if(pellipse == this) continue; 

								if( (((subdlg.m_bCheckColor == TRUE)&&(pellipse->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckWidth == TRUE)&&(pellipse->m_nWidth == m_nWidth))||(subdlg.m_bCheckWidth == FALSE))  )
								{
									if((pellipse->m_nColor == m_nColor)||(pellipse->m_nWidth == m_nWidth))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
										pundo->m_nDiagram = nDiagram;
										pundo->m_nPos = posbak;
										pundo->schellipse.Copy(pellipse);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(pellipse->m_nColor == m_nColor) pellipse->m_nColor = dlg.m_nColor;
									if(pellipse->m_nWidth == m_nWidth) pellipse->m_nWidth = dlg.m_nComboWidth;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
						{
							COb_schellipse* pellipse = (COb_schellipse*)pobject;
							if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag != FLAG_NORMAL))	continue;
							if(pellipse == this) continue; 

							if( (((subdlg.m_bCheckColor == TRUE)&&(pellipse->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(pellipse->m_nWidth == m_nWidth))||(subdlg.m_bCheckWidth == FALSE))  )
							{
								if((pellipse->m_nColor == m_nColor)||(pellipse->m_nWidth == m_nWidth))
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schellipse.Copy(pellipse);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

								}
								if(pellipse->m_nColor == m_nColor) pellipse->m_nColor = dlg.m_nColor;
								if(pellipse->m_nWidth == m_nWidth) pellipse->m_nWidth = dlg.m_nComboWidth;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
						{
							COb_schellipse* pellipse = (COb_schellipse*)pobject;
							if((pellipse->m_bSelection != TRUE)||(pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag != FLAG_NORMAL))	continue;
							if(pellipse == this) continue; 

							if( (((subdlg.m_bCheckColor == TRUE)&&(pellipse->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(pellipse->m_nWidth == m_nWidth))||(subdlg.m_bCheckWidth == FALSE))  )
							{
								if((pellipse->m_nColor == m_nColor)||(pellipse->m_nWidth == m_nWidth))
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schellipse.Copy(pellipse);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								}
								if(pellipse->m_nColor == m_nColor) pellipse->m_nColor = dlg.m_nColor;
								if(pellipse->m_nWidth == m_nWidth) pellipse->m_nWidth = dlg.m_nComboWidth;
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
				pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schellipse.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				m_fCx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
				m_fCy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
				m_fRadius_x = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusX);
				m_fRadius_y = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusY);
				m_fSangle = dlg.m_fEditStartAngle;
				m_fEangle = dlg.m_fEditEndAngle;
				m_nWidth = dlg.m_nComboWidth;
				m_nColor = dlg.m_nColor;

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
			pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->schellipse.Copy(this);
				
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

			m_fCx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
			m_fCy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
			m_fRadius_x = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusX);
			m_fRadius_y = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusY);
			m_fSangle = dlg.m_fEditStartAngle;
			m_fEangle = dlg.m_fEditEndAngle;
			m_nWidth = dlg.m_nComboWidth;
			m_nColor = dlg.m_nColor;
		}
	}
}

void COb_schellipse::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	int flag;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动园弧: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV);

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schellipse aellipse;
	aellipse.pDocument = pDoc;
	aellipse.m_fCx = m_fCx;
	aellipse.m_fCy = m_fCy;
	aellipse.m_fRadius_x = m_fRadius_x;
	aellipse.m_fRadius_y = m_fRadius_y;
	aellipse.m_fSangle = m_fSangle;
	aellipse.m_fEangle = m_fEangle;
	aellipse.m_nWidth = m_nWidth;
	aellipse.m_nColor = m_nColor;
	aellipse.m_nDiagram = m_nDiagram;
	aellipse.Draw(pDC, pV);

	pView->m_fCurrentSnapX = aellipse.m_fCx;
	pView->m_fCurrentSnapY = aellipse.m_fCy;
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
			aellipse.Draw(pDC, pV);
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
				aellipse.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			aellipse.m_fCx = pView->m_fCurrentSnapX;
			aellipse.m_fCy = pView->m_fCurrentSnapY;

			aellipse.m_fSangle = m_fSangle + flag*90;
			aellipse.m_fSangle = pDoc->Get_Correct_Angle(aellipse.m_fSangle);
			aellipse.m_fEangle = m_fEangle + flag*90;
			aellipse.m_fEangle = pDoc->Get_Correct_Angle(aellipse.m_fEangle);

			if((flag == 1)||(flag == 3)) 
			{
				aellipse.m_fRadius_x = m_fRadius_y;
				aellipse.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aellipse.m_fRadius_x = m_fRadius_x;
				aellipse.m_fRadius_y = m_fRadius_y;
			}

			pView->CloseMouseCursor();
			aellipse.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			aellipse.Draw(pDC, pV);
			pView->OpenMouseCursor();

			aellipse.m_fCx = pView->m_fCurrentSnapX;
			aellipse.m_fCy = pView->m_fCurrentSnapY;

			aellipse.m_fSangle = m_fSangle + flag*90;
			aellipse.m_fSangle = pDoc->Get_Correct_Angle(aellipse.m_fSangle);
			aellipse.m_fEangle = m_fEangle + flag*90;
			aellipse.m_fEangle = pDoc->Get_Correct_Angle(aellipse.m_fEangle);

			if((flag == 1)||(flag == 3)) 
			{
				aellipse.m_fRadius_x = m_fRadius_y;
				aellipse.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aellipse.m_fRadius_x = m_fRadius_x;
				aellipse.m_fRadius_y = m_fRadius_y;
			}

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->m_fX1 = m_fCx;
			pundo->m_fY1 = m_fCy;
			pundo->m_fX2 = m_fSangle;
			pundo->m_fY2 = m_fEangle;
			pundo->m_fRx = m_fRadius_x;
			pundo->m_fRy = m_fRadius_y;
			
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fCx = aellipse.m_fCx;
			m_fCy = aellipse.m_fCy;
			m_fSangle = aellipse.m_fSangle;
			m_fEangle = aellipse.m_fEangle;
			m_fRadius_x = aellipse.m_fRadius_x;
			m_fRadius_y = aellipse.m_fRadius_y;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aellipse.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			aellipse.m_fCx = pView->m_fCurrentSnapX;
			aellipse.m_fCy = pView->m_fCurrentSnapY;

			aellipse.m_fSangle = m_fSangle + flag*90;
			aellipse.m_fSangle = pDoc->Get_Correct_Angle(aellipse.m_fSangle);
			aellipse.m_fEangle = m_fEangle + flag*90;
			aellipse.m_fEangle = pDoc->Get_Correct_Angle(aellipse.m_fEangle);
			
			if((flag == 1)||(flag == 3)) 
			{
				aellipse.m_fRadius_x = m_fRadius_y;
				aellipse.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aellipse.m_fRadius_x = m_fRadius_x;
				aellipse.m_fRadius_y = m_fRadius_y;
			}

			aellipse.Draw(pDC, pV);

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

	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	pView->Write_Help(szEDA_Help_Ready);
}

BOOL COb_schellipse::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;
	
	char szHelp[] = "复制园弧: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schellipse aellipse;
	aellipse.pDocument = pDoc;
	aellipse.m_fCx = pView->m_fCurrentSnapX;
	aellipse.m_fCy = pView->m_fCurrentSnapY;
	aellipse.m_fRadius_x = m_fRadius_x;
	aellipse.m_fRadius_y = m_fRadius_y;
	aellipse.m_fSangle = m_fSangle;
	aellipse.m_fEangle = m_fEangle;
	aellipse.m_nWidth = m_nWidth;
	aellipse.m_nColor = m_nColor;
	aellipse.m_nDiagram = m_nDiagram;

	aellipse.Draw(pDC, pV);
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
			aellipse.Draw(pDC, pV);
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
				aellipse.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			aellipse.m_fCx = pView->m_fCurrentSnapX;
			aellipse.m_fCy = pView->m_fCurrentSnapY;

			aellipse.m_fSangle = m_fSangle + flag*90;
			aellipse.m_fSangle = pDoc->Get_Correct_Angle(aellipse.m_fSangle);
			aellipse.m_fEangle = m_fEangle + flag*90;
			aellipse.m_fEangle = pDoc->Get_Correct_Angle(aellipse.m_fEangle);

			if((flag == 1)||(flag == 3)) 
			{
				aellipse.m_fRadius_x = m_fRadius_y;
				aellipse.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aellipse.m_fRadius_x = m_fRadius_x;
				aellipse.m_fRadius_y = m_fRadius_y;
			}

			pView->CloseMouseCursor();
			aellipse.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			aellipse.Draw(pDC, pV);
			pView->OpenMouseCursor();

			aellipse.m_fCx = pView->m_fCurrentSnapX;
			aellipse.m_fCy = pView->m_fCurrentSnapY;

			aellipse.m_fSangle = m_fSangle + flag*90;
			aellipse.m_fSangle = pDoc->Get_Correct_Angle(aellipse.m_fSangle);
			aellipse.m_fEangle = m_fEangle + flag*90;
			aellipse.m_fEangle = pDoc->Get_Correct_Angle(aellipse.m_fEangle);

			if((flag == 1)||(flag == 3)) 
			{
				aellipse.m_fRadius_x = m_fRadius_y;
				aellipse.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aellipse.m_fRadius_x = m_fRadius_x;
				aellipse.m_fRadius_y = m_fRadius_y;
			}

			m_fCx = aellipse.m_fCx;
			m_fCy = aellipse.m_fCy;
			m_fSangle = aellipse.m_fSangle;
			m_fEangle = aellipse.m_fEangle;
			m_fRadius_x = aellipse.m_fRadius_x;
			m_fRadius_y = aellipse.m_fRadius_y;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aellipse.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			aellipse.m_fCx = pView->m_fCurrentSnapX;
			aellipse.m_fCy = pView->m_fCurrentSnapY;

			aellipse.m_fSangle = m_fSangle + flag*90;
			aellipse.m_fSangle = pDoc->Get_Correct_Angle(aellipse.m_fSangle);
			aellipse.m_fEangle = m_fEangle + flag*90;
			aellipse.m_fEangle = pDoc->Get_Correct_Angle(aellipse.m_fEangle);
			
			if((flag == 1)||(flag == 3)) 
			{
				aellipse.m_fRadius_x = m_fRadius_y;
				aellipse.m_fRadius_y = m_fRadius_x;
			}
			else
			{
				aellipse.m_fRadius_x = m_fRadius_x;
				aellipse.m_fRadius_y = m_fRadius_y;
			}

			aellipse.Draw(pDC, pV);

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

void COb_schellipse::Copy(COb_schellipse* pschellipse)
{
	m_fCx = pschellipse->m_fCx;
	m_fCy = pschellipse->m_fCy;
	m_fRadius_x = pschellipse->m_fRadius_x;
	m_fRadius_y = pschellipse->m_fRadius_y;
	m_fSangle = pschellipse->m_fSangle;
	m_fEangle = pschellipse->m_fEangle;

	m_nWidth = pschellipse->m_nWidth;
	m_nColor = pschellipse->m_nColor;

	m_nOwnerPartID = pschellipse->m_nOwnerPartID;
	m_nDisplayMode = pschellipse->m_nDisplayMode;
	m_posParent = pschellipse->m_posParent;
	
	m_nDiagram = pschellipse->m_nDiagram;
	m_bSelection = pschellipse->m_bSelection;
	m_nFlag = pschellipse->m_nFlag;

	pDocument = pschellipse->pDocument;
}