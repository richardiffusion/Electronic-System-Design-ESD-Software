#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"
#include "Dlg_Global_Bus.h"

#include "Dlg_EditArc.h"

IMPLEMENT_SERIAL(COb_scharc, CObject, 0)
COb_scharc::COb_scharc()
{
	// This empty constructor should be used by serialization only
	m_nFlag = 0;
	m_bSelection = FALSE;
	m_nOwnerPartID = 0;
	m_nDisplayMode = 0;
	m_posParent = NULL;

	pDocument = NULL;
}



void COb_scharc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fCx;
		ar << m_fCy;
		ar << m_fRadius;
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
		ar >> m_fRadius;
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

void COb_scharc::Draw(CDC* pDC, CView* pV)
{
	int cx,cy;
	int r;
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
	r = (int)(m_fRadius/pView->m_fScale);

	int width;
	width = nEDA_Line_Width[m_nWidth];

	if(pView->m_nPrint_Flag == 1)
	{
		cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
		r =  (int)(((double)r*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
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

	rect.left = cx - r;
	rect.right = cx + r;
	rect.top = cy - r;
	rect.bottom = cy + r;

	sp.x = cx + (int)(r*cos(m_fSangle*fEDA_pi/180));
	sp.y = cy - (int)(r*sin(m_fSangle*fEDA_pi/180));

	ep.x = cx + (int)(r*cos(m_fEangle*fEDA_pi/180));
	ep.y = cy - (int)(r*sin(m_fEangle*fEDA_pi/180));

	pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);

	pDC->SelectObject(oldpen);
}

void COb_scharc::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	int cx,cy;
	int r;
	COLORREF color;

	
	color = m_nColor;
	
	cx = (int)((m_fCx - origin_x)/scale);
	cy = nEy - (int)((m_fCy - origin_y)/scale);

	r = (int)(m_fRadius/scale);

	int width;
	width = nEDA_Line_Width[m_nWidth];

	width = (int)(width/scale);
	if(width < 1) width = 1;

	CPen arcpen;
	arcpen.CreatePen(PS_SOLID, width, color);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	CRect rect;
	CPoint sp, ep;

	rect.left = cx - r;
	rect.right = cx + r;
	rect.top = cy - r;
	rect.bottom = cy + r;

	sp.x = cx + (int)(r*cos(m_fSangle*fEDA_pi/180));
	sp.y = cy - (int)(r*sin(m_fSangle*fEDA_pi/180));

	ep.x = cx + (int)(r*cos(m_fEangle*fEDA_pi/180));
	ep.y = cy - (int)(r*sin(m_fEangle*fEDA_pi/180));

	pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);

	pDC->SelectObject(oldpen);
}

void COb_scharc::DrawSelection(CDC* pDC, CView* pV)
{
	int cx,cy;
	int r;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	CPen arcpen;
	arcpen.CreatePen(PS_DOT, 1, nEDA_SelectionColor);
	CPen* oldpen = pDC->SelectObject(&arcpen);


	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
	r = (int)(m_fRadius/pView->m_fScale);

	CRect rect;
	CPoint sp, ep, mp;

	rect.left = cx - r;
	rect.right = cx + r;
	rect.top = cy - r;
	rect.bottom = cy + r;

	sp.x = cx + (int)(r*cos(m_fEangle*fEDA_pi/180));
	sp.y = cy - (int)(r*sin(m_fEangle*fEDA_pi/180));

	ep.x = cx + (int)(r*cos(m_fSangle*fEDA_pi/180));
	ep.y = cy - (int)(r*sin(m_fSangle*fEDA_pi/180));

	pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);

	pDC->MoveTo(sp.x, sp.y);
	pDC->LineTo(cx, cy);
	pDC->LineTo(ep.x, ep.y);

	double alpha;
	if(m_fEangle > m_fSangle) alpha = (m_fSangle + m_fEangle)/2;
	else alpha = (m_fSangle + m_fEangle)/2 + 180;
	mp.x = cx + (int)(r*cos(alpha*fEDA_pi/180));
	mp.y = cy - (int)(r*sin(alpha*fEDA_pi/180));
	
	pView->DrawSelectionKeyPoint(sp.x, sp.y, pDC);
	pView->DrawSelectionKeyPoint(ep.x, ep.y, pDC);
	pView->DrawSelectionKeyPoint(mp.x, mp.y, pDC);

	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);

}

BOOL COb_scharc::PickMe(CView* pV)
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

	minx = (int)(m_fCx - m_fRadius);
	miny = (int)(m_fCy - m_fRadius);
	maxx = (int)(m_fCx + m_fRadius);
	maxy = (int)(m_fCy + m_fRadius);

   	if( ((minx < wx1)&&(maxx < wx1)) || ((minx > wx2)&&(maxx > wx2)) || ((miny < wy1)&&(maxy < wy1)) || ((miny > wy2)&&(maxy > wy2)) )	return FALSE;
	else return TRUE;
}


BOOL  COb_scharc::Is_On_Me(double fx, double fy)
{
	int width;
	width = nEDA_Line_Width[m_nWidth];

	int delta = (int)(width/2) + nEDA_Selection_Offset;

	CRgn rgn, rgn1, rgn2;
	rgn1.CreateEllipticRgn((int)(m_fCx - (m_fRadius - delta)), (int)(m_fCy - (m_fRadius - delta)), (int)(m_fCx + (m_fRadius - delta)), (int)(m_fCy + (m_fRadius - delta)) );
	rgn2.CreateEllipticRgn((int)(m_fCx - (m_fRadius + delta)), (int)(m_fCy - (m_fRadius + delta)), (int)(m_fCx + (m_fRadius + delta)), (int)(m_fCy + (m_fRadius + delta)) );
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

BOOL  COb_scharc::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fminx, fminy, fmaxx, fmaxy;
	fminx = m_fCx - m_fRadius;
	fminy = m_fCy - m_fRadius;
	fmaxx = m_fCx + m_fRadius;
	fmaxy = m_fCy + m_fRadius;
	
	if((fminx < fx1)||(fmaxx > fx2)||(fminy < fy1)||(fmaxy > fy2)) return FALSE;
	return TRUE;
}

void COb_scharc::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_EditArc dlg;
	
	dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fCx);
	dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fCy);
	dlg.m_csEditRadius = pDoc->Convert_XY_To_String(m_fRadius);
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
							{
								COb_scharc* parc = (COb_scharc*)pobject;
								if((parc->m_nDiagram != nDiagram)||(parc->m_nFlag != FLAG_NORMAL))	continue;
								if(parc == this) continue; 

								if( (((subdlg.m_bCheckColor == TRUE)&&(parc->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckWidth == TRUE)&&(parc->m_nWidth == m_nWidth))||(subdlg.m_bCheckWidth == FALSE))  )
								{
									if((parc->m_nColor == m_nColor)||(parc->m_nWidth == m_nWidth))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_ARC;
										pundo->m_nDiagram = nDiagram;
										pundo->m_nPos = posbak;
										pundo->scharc.Copy(parc);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
	
									}
									if(parc->m_nColor == m_nColor) parc->m_nColor = dlg.m_nColor;
									if(parc->m_nWidth == m_nWidth) parc->m_nWidth = dlg.m_nComboWidth;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
						{
							COb_scharc* parc = (COb_scharc*)pobject;
							if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag != FLAG_NORMAL))	continue;
							if(parc == this) continue; 

							if( (((subdlg.m_bCheckColor == TRUE)&&(parc->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(parc->m_nWidth == m_nWidth))||(subdlg.m_bCheckWidth == FALSE))  )
							{
								if((parc->m_nColor == m_nColor)||(parc->m_nWidth == m_nWidth))
								{
									
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_ARC;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->scharc.Copy(parc);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									
								}
								if(parc->m_nColor == m_nColor) parc->m_nColor = dlg.m_nColor;
								if(parc->m_nWidth == m_nWidth) parc->m_nWidth = dlg.m_nComboWidth;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
						{
							COb_scharc* parc = (COb_scharc*)pobject;
							if((parc->m_bSelection != TRUE)||(parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag != FLAG_NORMAL))	continue;
							if(parc == this) continue; 

							if( (((subdlg.m_bCheckColor == TRUE)&&(parc->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(parc->m_nWidth == m_nWidth))||(subdlg.m_bCheckWidth == FALSE))  )
							{
								if((parc->m_nColor == m_nColor)||(parc->m_nWidth == m_nWidth))
								{
									
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_ARC;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->scharc.Copy(parc);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									
								}
								if(parc->m_nColor == m_nColor) parc->m_nColor = dlg.m_nColor;
								if(parc->m_nWidth == m_nWidth) parc->m_nWidth = dlg.m_nComboWidth;
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
				pundo->m_nOb = SCH_ELEMENT_ARC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->scharc.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				m_fCx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
				m_fCy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
				m_fRadius = pDoc->Convert_String_To_XY(dlg.m_csEditRadius);
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
			pundo->m_nOb = SCH_ELEMENT_ARC;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->scharc.Copy(this);
				
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
			m_fCx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
			m_fCy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
			m_fRadius = pDoc->Convert_String_To_XY(dlg.m_csEditRadius);
			m_fSangle = dlg.m_fEditStartAngle;
			m_fEangle = dlg.m_fEditEndAngle;
			m_nWidth = dlg.m_nComboWidth;
			m_nColor = dlg.m_nColor;
		}
	}
}

void COb_scharc::Move(CClientDC* pDC, CView* pV, POSITION pos)
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

	COb_scharc aarc;
	aarc.pDocument = pDoc;
	aarc.m_fCx = m_fCx;
	aarc.m_fCy = m_fCy;
	aarc.m_fRadius = m_fRadius;
	aarc.m_fSangle = m_fSangle;
	aarc.m_fEangle = m_fEangle;
	aarc.m_nWidth = m_nWidth;
	aarc.m_nColor = m_nColor;
	aarc.m_nDiagram = m_nDiagram;
	aarc.Draw(pDC, pV);

	pView->m_fCurrentSnapX = aarc.m_fCx;
	pView->m_fCurrentSnapY = aarc.m_fCy;
	pView->Restore_LargeCrossXY();

	pView->OpenMouseCursor();

	pView->Write_Help(szHelp);
	flag = 0;
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
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
				aarc.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			aarc.m_fCx = pView->m_fCurrentSnapX;
			aarc.m_fCy = pView->m_fCurrentSnapY;

			aarc.m_fSangle = m_fSangle + flag*90;
			aarc.m_fSangle = pDoc->Get_Correct_Angle(aarc.m_fSangle);
			aarc.m_fEangle = m_fEangle + flag*90;
			aarc.m_fEangle = pDoc->Get_Correct_Angle(aarc.m_fEangle);

			
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
			pView->OpenMouseCursor();

			aarc.m_fCx = pView->m_fCurrentSnapX;
			aarc.m_fCy = pView->m_fCurrentSnapY;

			aarc.m_fSangle = m_fSangle + flag*90;
			aarc.m_fSangle = pDoc->Get_Correct_Angle(aarc.m_fSangle);
			aarc.m_fEangle = m_fEangle + flag*90;
			aarc.m_fEangle = pDoc->Get_Correct_Angle(aarc.m_fEangle);

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_ARC;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->m_fX1 = m_fCx;
			pundo->m_fY1 = m_fCy;
			pundo->m_fX2 = m_fSangle;
			pundo->m_fY2 = m_fEangle;
			pundo->m_fRx = m_fRadius;
			
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fCx = aarc.m_fCx;
			m_fCy = aarc.m_fCy;
			m_fSangle = aarc.m_fSangle;
			m_fEangle = aarc.m_fEangle;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			aarc.m_fCx = pView->m_fCurrentSnapX;
			aarc.m_fCy = pView->m_fCurrentSnapY;

			aarc.m_fSangle = m_fSangle + flag*90;
			aarc.m_fSangle = pDoc->Get_Correct_Angle(aarc.m_fSangle);
			aarc.m_fEangle = m_fEangle + flag*90;
			aarc.m_fEangle = pDoc->Get_Correct_Angle(aarc.m_fEangle);

			aarc.Draw(pDC, pV);

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

BOOL COb_scharc::MovePlaced(CClientDC* pDC, CView* pV) //, POSITION pos)
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

	COb_scharc aarc;
	aarc.pDocument = pDoc;
	aarc.m_fCx = pView->m_fCurrentSnapX;
	aarc.m_fCy = pView->m_fCurrentSnapY;
	aarc.m_fRadius = m_fRadius;
	aarc.m_fSangle = m_fSangle;
	aarc.m_fEangle = m_fEangle;
	aarc.m_nWidth = m_nWidth;
	aarc.m_nColor = m_nColor;
	aarc.m_nDiagram = m_nDiagram;

	aarc.Draw(pDC, pV);
	pView->OpenMouseCursor();

	pView->Write_Help(szHelp);
	flag = 0;
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
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
				aarc.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			aarc.m_fCx = pView->m_fCurrentSnapX;
			aarc.m_fCy = pView->m_fCurrentSnapY;

			aarc.m_fSangle = m_fSangle + flag*90;
			aarc.m_fSangle = pDoc->Get_Correct_Angle(aarc.m_fSangle);
			aarc.m_fEangle = m_fEangle + flag*90;
			aarc.m_fEangle = pDoc->Get_Correct_Angle(aarc.m_fEangle);

			
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
			pView->OpenMouseCursor();

			aarc.m_fCx = pView->m_fCurrentSnapX;
			aarc.m_fCy = pView->m_fCurrentSnapY;

			aarc.m_fSangle = m_fSangle + flag*90;
			aarc.m_fSangle = pDoc->Get_Correct_Angle(aarc.m_fSangle);
			aarc.m_fEangle = m_fEangle + flag*90;
			aarc.m_fEangle = pDoc->Get_Correct_Angle(aarc.m_fEangle);

			m_fCx = aarc.m_fCx;
			m_fCy = aarc.m_fCy;
			m_fSangle = aarc.m_fSangle;
			m_fEangle = aarc.m_fEangle;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			aarc.m_fCx = pView->m_fCurrentSnapX;
			aarc.m_fCy = pView->m_fCurrentSnapY;

			aarc.m_fSangle = m_fSangle + flag*90;
			aarc.m_fSangle = pDoc->Get_Correct_Angle(aarc.m_fSangle);
			aarc.m_fEangle = m_fEangle + flag*90;
			aarc.m_fEangle = pDoc->Get_Correct_Angle(aarc.m_fEangle);

			aarc.Draw(pDC, pV);

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

void COb_scharc::Copy(COb_scharc* pscharc)
{
	m_fCx = pscharc->m_fCx;
	m_fCy = pscharc->m_fCy;
	m_fRadius = pscharc->m_fRadius;
	m_fSangle = pscharc->m_fSangle;
	m_fEangle = pscharc->m_fEangle;

	m_nWidth = pscharc->m_nWidth;
	m_nColor = pscharc->m_nColor;
	
	m_nOwnerPartID = pscharc->m_nOwnerPartID;
	m_nDisplayMode = pscharc->m_nDisplayMode;
	m_posParent	 = pscharc->m_posParent;

	m_nDiagram = pscharc->m_nDiagram;
	m_bSelection = pscharc->m_bSelection;
	m_nFlag = pscharc->m_nFlag;

	pDocument = pscharc->pDocument;
}