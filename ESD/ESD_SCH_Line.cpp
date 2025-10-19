#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Page_EditPolyline.h"
#include "Page_EditWire.h"
#include "Page_EditBus.h"
#include "Dlg_EditBusEntry.h"
#include "Page_Vertices.h"
#include "Sheet_EditBus.h"
#include "Sheet_EditWire.h"
#include "Dlg_Global_Bus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_schline, CObject, 0)
COb_schline::COb_schline()
{
	m_nFlag = 0;
	m_bSelection = FALSE;
	
	m_nLineType = 0;
	pDocument = NULL;
}


void COb_schline::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nAttrib;
		ar << m_nLineWidth;
		ar << m_nLineType;
		ar << m_nColor;
		ar << m_nDiagram;
		ar << m_bSelection;

		int vertexNum = m_cVertex.GetCount();
		ar << vertexNum;
		for(int i=0; i<vertexNum; i++)
		{
			ar << m_cVertex[i].fx;
			ar << m_cVertex[i].fy;
		}
	}
	else
	{
		ar >> m_nAttrib;
		ar >> m_nLineWidth;
		ar >> m_nLineType;
		ar >> m_nColor;
		ar >> m_nDiagram;
		ar >> m_bSelection;

		int vertexNum;
		ar >> vertexNum;
		m_cVertex.RemoveAll();
		m_cVertex.SetSize(vertexNum);
		for(int i=0; i<vertexNum; i++)
		{
			ar >> m_cVertex[i].fx;
			ar >> m_cVertex[i].fy;
		}
	}
}

void COb_schline::Draw(CDC* pDC, CView* pV)
{
	int x1,y1,x2,y2,width;
	CPen linepen, *oldpen;
	COLORREF color;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	if(m_nFlag == FLAG_NORMAL) color = m_nColor; 
	else if(m_nFlag == FLAG_DELETED) color = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor;

	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_Print_Color == 0) color = RGB(0,0,0); 
		else if(nEDA_Print_Color == 2) color = RGB(GetGray(m_nColor), GetGray(m_nColor), GetGray(m_nColor));
		else color = m_nColor;
	}
	
	if(m_nAttrib == LINE_BUS) width = nEDA_Bus_Width[m_nLineWidth];
	else width = nEDA_Line_Width[m_nLineWidth];

	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	width = (int)(width/pView->m_fScale);
	if(m_nLineWidth == LINE_SMALLEST) width = 1;
	if(pView->m_nPrint_Flag == 1) width = (int)(width*nEDA_Print_Xdpi/1000*fEDA_Print_Scale);

	if(width < 1) width = 1;	
	
	LOGBRUSH brush;
	brush.lbStyle = BS_SOLID;  //brush.lbHatch ignored
	brush.lbColor = color;
	
	DWORD style_dash[2];
	style_dash[0] = width + width;
	style_dash[1] = width + width;

	DWORD style_dot[2];
	style_dot[0] = 1;
	style_dot[1] = width + width;

	
	switch(m_nLineType)
	{
		case LINE_SOLID: 
			linepen.CreatePen(PS_SOLID, width, color);
			break;
		case LINE_DASHED: 
			if(width <= 1) linepen.CreatePen(PS_DASH, 1, color);
			else linepen.CreatePen(PS_USERSTYLE|PS_GEOMETRIC, width, &brush, 2, style_dash);
			break;
		case LINE_DOTTED:
			if(width <= 1) linepen.CreatePen(PS_DOT, 1, color);
			else linepen.CreatePen(PS_USERSTYLE|PS_GEOMETRIC, width, &brush, 2, style_dot);
	}
	oldpen = pDC->SelectObject(&linepen);

	for(int i =0; i<m_cVertex.GetCount()-1; i++)
	{
		pView->Get_VPort_XY(&x1,&y1,m_cVertex[i].fx,m_cVertex[i].fy);
		pView->Get_VPort_XY(&x2,&y2,m_cVertex[i+1].fx,m_cVertex[i+1].fy);
		if(pView->m_nPrint_Flag == 1)
		{
			x1 = (int)(((double)x1*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
			y1 = (int)(((double)y1*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
			x2 = (int)(((double)x2*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
			y2 = (int)(((double)y2*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
		}
		pDC->MoveTo(x1,y1);
		pDC->LineTo(x2,y2);
	}

	pDC->SetBkMode(oldbkmode);
	pDC->SelectObject(oldpen);
}


void COb_schline::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	int x1,y1,x2,y2,width;
	CPen linepen, *oldpen;
	COLORREF color;
	
	color = m_nColor; 
	
	if(m_nAttrib == LINE_BUS) width = nEDA_Bus_Width[m_nLineWidth];
	else width = nEDA_Line_Width[m_nLineWidth];

	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	width = (int)(width/scale);
	if(m_nLineWidth == LINE_SMALLEST) width = 1;
	if(width < 1) width = 1;	
	
	LOGBRUSH brush;
	brush.lbStyle = BS_SOLID;  //brush.lbHatch ignored
	brush.lbColor = color;
	
	DWORD style_dash[2];
	style_dash[0] = width + width;
	style_dash[1] = width + width;

	DWORD style_dot[2];
	style_dot[0] = 1;
	style_dot[1] = width + width;

	
	switch(m_nLineType)
	{
		case LINE_SOLID: 
			linepen.CreatePen(PS_SOLID, width, color);
			break;
		case LINE_DASHED: 
			if(width <= 1) linepen.CreatePen(PS_DASH, 1, color);
			else linepen.CreatePen(PS_USERSTYLE|PS_GEOMETRIC, width, &brush, 2, style_dash);
			break;
		case LINE_DOTTED:
			if(width <= 1) linepen.CreatePen(PS_DOT, 1, color);
			else linepen.CreatePen(PS_USERSTYLE|PS_GEOMETRIC, width, &brush, 2, style_dot);
	}
	oldpen = pDC->SelectObject(&linepen);

	for(int i =0; i<m_cVertex.GetCount()-1; i++)
	{
		//pView->Get_VPort_XY(&x1,&y1,m_cVertex[i].fx,m_cVertex[i].fy);
		x1 = (int)((m_cVertex[i].fx - origin_x)/scale);
		y1 = nEy - (int)((m_cVertex[i].fy - origin_y)/scale);
		//pView->Get_VPort_XY(&x2,&y2,m_cVertex[i+1].fx,m_cVertex[i+1].fy);
		x2 = (int)((m_cVertex[i+1].fx - origin_x)/scale);
		y2 = nEy - (int)((m_cVertex[i+1].fy - origin_y)/scale);
		pDC->MoveTo(x1,y1);
		pDC->LineTo(x2,y2);
	}

	pDC->SetBkMode(oldbkmode);
	pDC->SelectObject(oldpen);
}

void COb_schline::DrawSelection(CDC* pDC, CView* pV)
{
	int x1,y1,x2,y2;
	CPen linepen, *oldpen;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	linepen.CreatePen(PS_DOT, 1, nEDA_SelectionColor);
	oldpen = pDC->SelectObject(&linepen);

	for(int i =0; i<m_cVertex.GetCount()-1; i++)
	{
		pView->Get_VPort_XY(&x1, &y1, m_cVertex[i].fx, m_cVertex[i].fy);
		pView->Get_VPort_XY(&x2, &y2, m_cVertex[i+1].fx, m_cVertex[i+1].fy);

		pDC->MoveTo(x1,y1);
		pDC->LineTo(x2,y2);
	}

	if(m_nAttrib != LINE_BUS_ENTRY)
	{
		for(int i =0; i<m_cVertex.GetCount(); i++)
		{
			int x,y;
			pView->Get_VPort_XY(&x,&y,m_cVertex[i].fx,m_cVertex[i].fy);
			pView->DrawSelectionKeyPoint(x, y, pDC);
		}
		for(int i =1; i<m_cVertex.GetCount(); i++)
		{
			int x,y;
			pView->Get_VPort_XY(&x,&y,(m_cVertex[i-1].fx+m_cVertex[i].fx)/2,(m_cVertex[i-1].fy + m_cVertex[i].fy)/2);
			pView->DrawSelectionKeyPoint(x, y, pDC);
		}
	}

	pDC->SetBkMode(oldbkmode);
	pDC->SelectObject(oldpen);
}

BOOL COb_schline::PickMe(CView* pV)
{
	CESDView* pView = (CESDView*)pV;

	int wx1,wy1,wx2,wy2;
	if(pView->m_nPrint_Flag == 1)
	{
		wx1 = (int)fEDA_Print_Minx;		wy1 = (int)fEDA_Print_Miny;
		wx2 = (int)fEDA_Print_Maxx;		wy2 = (int)fEDA_Print_Maxy;
	}
	else  //显示
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

	for(int i=0; i<m_cVertex.GetCount()-1; i++)
	{
		int x1,y1,x2,y2;
		x1 = (int)m_cVertex[i].fx;
		y1 = (int)m_cVertex[i].fy;
		x2 = (int)m_cVertex[i+1].fx;
		y2 = (int)m_cVertex[i+1].fy;

		if( ((x1 < wx1)&&(x2 < wx1)) || ((x1 > wx2)&&(x2 > wx2)) || ((y1 < wy1)&&(y2 < wy1)) || ((y1 > wy2)&&(y2 > wy2)) )	{}
		else return TRUE;

	}

	return FALSE;
}

BOOL  COb_schline::Is_On_Me(double fx, double fy)
{
	double ax,ay,bx,by;
	double alpha;

	int width;
	if(m_nAttrib == LINE_BUS) width = nEDA_Bus_Width[m_nLineWidth];
	else width = nEDA_Line_Width[m_nLineWidth];

	int delta = (int)(width/2) + nEDA_Selection_Offset;


	int vertex = m_cVertex.GetCount();
	for(int i=0; i<vertex-1; i++)
	{
		ax = m_cVertex[i].fx;
		ay = m_cVertex[i].fy;
		bx = m_cVertex[i+1].fx;
		by = m_cVertex[i+1].fy;
		if(ax == bx) alpha = fEDA_pi/2;
		else if(ay == by) alpha = 0;
		else alpha = atan((by-ay)/(bx-ax));
		
		CRgn rgn;
		CPoint ptVertex[4];
		ptVertex[0].x = (int)(ax - delta*sin(alpha));
		ptVertex[0].y = (int)(ay + delta*cos(alpha));
		ptVertex[1].x = (int)(ax + delta*sin(alpha));
		ptVertex[1].y = (int)(ay - delta*cos(alpha));
		ptVertex[2].x = (int)(bx + delta*sin(alpha));
		ptVertex[2].y = (int)(by - delta*cos(alpha));
		ptVertex[3].x = (int)(bx - delta*sin(alpha));
		ptVertex[3].y = (int)(by + delta*cos(alpha));
	
		
		rgn.CreatePolygonRgn( ptVertex, 4, ALTERNATE);

		if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	}

	return FALSE;
}

BOOL  COb_schline::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
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

void COb_schline::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	if(m_nAttrib == LINE_BUS)
	{
		CSheet_EditBus  ps(_T("总线"), NULL, 0);

		ps.page1.m_nComboWidth = m_nLineWidth;
		ps.page1.m_nColor = m_nColor;

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
				CDlg_Global_Bus dlg;
				if(dlg.DoModal() == IDOK)
				{
					if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					
					if(dlg.m_nRadioScope == 0)	
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
								
								if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
								{
									COb_schline* pline = (COb_schline*)pobject;
									if((pline->m_nAttrib != m_nAttrib)||(pline->m_nDiagram != nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;
									if(pline == this) continue; 

									if( (((dlg.m_bCheckColor == TRUE)&&(pline->m_nColor == m_nColor))||(dlg.m_bCheckColor == FALSE)) &&
										(((dlg.m_bCheckWidth == TRUE)&&(pline->m_nLineWidth == m_nLineWidth))||(dlg.m_bCheckWidth == FALSE))  )
									{
										if((pline->m_nColor == m_nColor)||(pline->m_nLineWidth == m_nLineWidth))
										{
											CSchUndo *pundo = new CSchUndo;
											pundo->m_nOp = UNDO_CHANGE;
											pundo->m_nOb = SCH_ELEMENT_LINE;
											pundo->m_nDiagram = nDiagram;
											pundo->m_nPos = posbak;
											pundo->schline.Copy(pline);
											pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

										}
										if(pline->m_nColor == m_nColor) pline->m_nColor = ps.page1.m_nColor;
										if(pline->m_nLineWidth == m_nLineWidth) pline->m_nLineWidth = ps.page1.m_nComboWidth;
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
					else if(dlg.m_nRadioScope == 1)	
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
							{
								COb_schline* pline = (COb_schline*)pobject;
								if((pline->m_nAttrib != m_nAttrib)||(pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;
								if(pline == this) continue; 

								if( (((dlg.m_bCheckColor == TRUE)&&(pline->m_nColor == m_nColor))||(dlg.m_bCheckColor == FALSE)) &&
									(((dlg.m_bCheckWidth == TRUE)&&(pline->m_nLineWidth == m_nLineWidth))||(dlg.m_bCheckWidth == FALSE))  )
								{
									if((pline->m_nColor == m_nColor)||(pline->m_nLineWidth == m_nLineWidth))
									{

										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_LINE;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = posbak;
										pundo->schline.Copy(pline);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(pline->m_nColor == m_nColor) pline->m_nColor = ps.page1.m_nColor;
									if(pline->m_nLineWidth == m_nLineWidth) pline->m_nLineWidth = ps.page1.m_nComboWidth;
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
							{
								COb_schline* pline = (COb_schline*)pobject;
								if((pline->m_bSelection != TRUE)||(pline->m_nAttrib != m_nAttrib)||(pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;
								if(pline == this) continue; 

								if( (((dlg.m_bCheckColor == TRUE)&&(pline->m_nColor == m_nColor))||(dlg.m_bCheckColor == FALSE)) &&
									(((dlg.m_bCheckWidth == TRUE)&&(pline->m_nLineWidth == m_nLineWidth))||(dlg.m_bCheckWidth == FALSE))  )
								{
									if((pline->m_nColor == m_nColor)||(pline->m_nLineWidth == m_nLineWidth))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_LINE;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = posbak;
										pundo->schline.Copy(pline);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

									}
									if(pline->m_nColor == m_nColor) pline->m_nColor = ps.page1.m_nColor;
									if(pline->m_nLineWidth == m_nLineWidth) pline->m_nLineWidth = ps.page1.m_nComboWidth;
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
					pundo->m_nOb = SCH_ELEMENT_LINE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = pos;
					pundo->schline.Copy(this);
					
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);


					m_nLineWidth = ps.page1.m_nComboWidth;
					m_nColor = ps.page1.m_nColor;

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
				pundo->m_nOb = SCH_ELEMENT_LINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schline.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

				m_nLineWidth = ps.page1.m_nComboWidth;
				m_nColor = ps.page1.m_nColor;

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
	else if(m_nAttrib == LINE_BUS_ENTRY)
	{
		CDlg_EditBusEntry dlg;
		dlg.m_csEditLocationX1 = pDoc->Convert_XY_To_String(m_cVertex[0].fx);
		dlg.m_csEditLocationY1 = pDoc->Convert_XY_To_String(m_cVertex[0].fy);
		dlg.m_csEditLocationX2 = pDoc->Convert_XY_To_String(m_cVertex[1].fx);
		dlg.m_csEditLocationY2 = pDoc->Convert_XY_To_String(m_cVertex[1].fy);
		dlg.m_nComboWidth = m_nLineWidth;
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
								
								if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
								{
									COb_schline* pline = (COb_schline*)pobject;
									if((pline->m_nAttrib != m_nAttrib)||(pline->m_nDiagram != nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;
									if(pline == this) continue; 

									if( (((subdlg.m_bCheckColor == TRUE)&&(pline->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
										(((subdlg.m_bCheckWidth == TRUE)&&(pline->m_nLineWidth == m_nLineWidth))||(subdlg.m_bCheckWidth == FALSE))  )
									{
										if((pline->m_nColor == m_nColor)||(pline->m_nLineWidth == m_nLineWidth))
										{
											CSchUndo *pundo = new CSchUndo;
											pundo->m_nOp = UNDO_CHANGE;
											pundo->m_nOb = SCH_ELEMENT_LINE;
											pundo->m_nDiagram = nDiagram;
											pundo->m_nPos = posbak;
											pundo->schline.Copy(pline);
											pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
										}
										if(pline->m_nColor == m_nColor) pline->m_nColor = dlg.m_nColor;
										if(pline->m_nLineWidth == m_nLineWidth) pline->m_nLineWidth = dlg.m_nComboWidth;
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
							{
								COb_schline* pline = (COb_schline*)pobject;
								if((pline->m_nAttrib != m_nAttrib)||(pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;
								if(pline == this) continue;

								if( (((subdlg.m_bCheckColor == TRUE)&&(pline->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckWidth == TRUE)&&(pline->m_nLineWidth == m_nLineWidth))||(subdlg.m_bCheckWidth == FALSE))  )
								{
									if((pline->m_nColor == m_nColor)||(pline->m_nLineWidth == m_nLineWidth))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_LINE;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = posbak;
										pundo->schline.Copy(pline);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(pline->m_nColor == m_nColor) pline->m_nColor = dlg.m_nColor;
									if(pline->m_nLineWidth == m_nLineWidth) pline->m_nLineWidth = dlg.m_nComboWidth;
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
							{
								COb_schline* pline = (COb_schline*)pobject;
								if((pline->m_bSelection != TRUE)||(pline->m_nAttrib != m_nAttrib)||(pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;
								if(pline == this) continue;

								if( (((subdlg.m_bCheckColor == TRUE)&&(pline->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckWidth == TRUE)&&(pline->m_nLineWidth == m_nLineWidth))||(subdlg.m_bCheckWidth == FALSE))  )
								{
									if((pline->m_nColor == m_nColor)||(pline->m_nLineWidth == m_nLineWidth))
									{

										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_LINE;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = posbak;
										pundo->schline.Copy(pline);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

									}
									if(pline->m_nColor == m_nColor) pline->m_nColor = dlg.m_nColor;
									if(pline->m_nLineWidth == m_nLineWidth) pline->m_nLineWidth = dlg.m_nComboWidth;
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
					pundo->m_nOb = SCH_ELEMENT_LINE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = pos;
					pundo->schline.Copy(this);
					
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					m_nLineWidth = dlg.m_nComboWidth;
					m_nColor = dlg.m_nColor;

					m_cVertex[0].fx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX1);
					m_cVertex[0].fy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY1);
					m_cVertex[1].fx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX2);
					m_cVertex[1].fy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY2);

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
				pundo->m_nOb = SCH_ELEMENT_LINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schline.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();


				m_nLineWidth = dlg.m_nComboWidth;
				m_nColor = dlg.m_nColor;

				m_cVertex[0].fx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX1);
				m_cVertex[0].fy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY1);
				m_cVertex[1].fx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX2);
				m_cVertex[1].fy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY2);
			}
		}
	}
	else //	if(m_nAttrib == LINE_WIRE)
	{
		CSheet_EditWire  ps(_T("导线"), NULL, 0);
		
		ps.page1.m_nComboWidth = m_nLineWidth;
		ps.page1.m_nColor = m_nColor;

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
				CDlg_Global_Bus dlg;
				if(dlg.DoModal() == IDOK)
				{

					if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}

					
					if(dlg.m_nRadioScope == 0)	
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
								
								if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
								{
									COb_schline* pline = (COb_schline*)pobject;
									if((pline->m_nAttrib != m_nAttrib)||(pline->m_nDiagram != nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;
									if(pline == this) continue; 

									if( (((dlg.m_bCheckColor == TRUE)&&(pline->m_nColor == m_nColor))||(dlg.m_bCheckColor == FALSE)) &&
										(((dlg.m_bCheckWidth == TRUE)&&(pline->m_nLineWidth == m_nLineWidth))||(dlg.m_bCheckWidth == FALSE))  )
									{
										if((pline->m_nColor == m_nColor)||(pline->m_nLineWidth == m_nLineWidth))
										{
											CSchUndo *pundo = new CSchUndo;
											pundo->m_nOp = UNDO_CHANGE;
											pundo->m_nOb = SCH_ELEMENT_LINE;
											pundo->m_nDiagram = nDiagram;
											pundo->m_nPos = posbak;
											pundo->schline.Copy(pline);
											pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
										}
										if(pline->m_nColor == m_nColor) pline->m_nColor = ps.page1.m_nColor;
										if(pline->m_nLineWidth == m_nLineWidth) pline->m_nLineWidth = ps.page1.m_nComboWidth;
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
					else if(dlg.m_nRadioScope == 1)	
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
							{
								COb_schline* pline = (COb_schline*)pobject;
								if((pline->m_nAttrib != m_nAttrib)||(pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;
								if(pline == this) continue; 

								if( (((dlg.m_bCheckColor == TRUE)&&(pline->m_nColor == m_nColor))||(dlg.m_bCheckColor == FALSE)) &&
									(((dlg.m_bCheckWidth == TRUE)&&(pline->m_nLineWidth == m_nLineWidth))||(dlg.m_bCheckWidth == FALSE))  )
								{
									if((pline->m_nColor == m_nColor)||(pline->m_nLineWidth == m_nLineWidth))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_LINE;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = posbak;
										pundo->schline.Copy(pline);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(pline->m_nColor == m_nColor) pline->m_nColor = ps.page1.m_nColor;
									if(pline->m_nLineWidth == m_nLineWidth) pline->m_nLineWidth = ps.page1.m_nComboWidth;
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
							{
								COb_schline* pline = (COb_schline*)pobject;
								if((pline->m_bSelection != TRUE)||(pline->m_nAttrib != m_nAttrib)||(pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;
								if(pline == this) continue; 

								if( (((dlg.m_bCheckColor == TRUE)&&(pline->m_nColor == m_nColor))||(dlg.m_bCheckColor == FALSE)) &&
									(((dlg.m_bCheckWidth == TRUE)&&(pline->m_nLineWidth == m_nLineWidth))||(dlg.m_bCheckWidth == FALSE))  )
								{
									if((pline->m_nColor == m_nColor)||(pline->m_nLineWidth == m_nLineWidth))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_LINE;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = posbak;
										pundo->schline.Copy(pline);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(pline->m_nColor == m_nColor) pline->m_nColor = ps.page1.m_nColor;
									if(pline->m_nLineWidth == m_nLineWidth) pline->m_nLineWidth = ps.page1.m_nComboWidth;
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
					pundo->m_nOb = SCH_ELEMENT_LINE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = pos;
					pundo->schline.Copy(this);
					
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					m_nLineWidth = ps.page1.m_nComboWidth;
					m_nColor = ps.page1.m_nColor;

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
				pundo->m_nOb = SCH_ELEMENT_LINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schline.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();


				m_nLineWidth = ps.page1.m_nComboWidth;
				m_nColor = ps.page1.m_nColor;

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
}

void COb_schline::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	BOOL clear_undo_buffer_flag = FALSE;
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动连线: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV);

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	
	
	COb_schline aline;
	aline.pDocument = pDoc;
	aline.m_nAttrib = m_nAttrib;
	aline.m_nLineWidth = m_nLineWidth;
	aline.m_nLineType = m_nLineType;
	aline.m_nColor = m_nColor;
	aline.m_nDiagram = m_nDiagram;
	aline.m_nFlag = FLAG_NORMAL;
	
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		Fpoint fpoint;
		fpoint = m_cVertex.GetAt(i);
		aline.m_cVertex.Add(fpoint);
	}
	aline.Draw(pDC, pV);

	x0 = pView->m_fCurrentSnapX;
	y0 = pView->m_fCurrentSnapY;

	double dis = 10000000;
	int index;
	for(int i=0; i<aline.m_cVertex.GetCount(); i++)
	{
		double d = sqrt((x0 - aline.m_cVertex[i].fx)*(x0 - aline.m_cVertex[i].fx) + (y0 - aline.m_cVertex[i].fy)*(y0 - aline.m_cVertex[i].fy));
		if(d < dis)
		{
			dis = d;
			index = i;
		}
	}
	x0 = aline.m_cVertex[index].fx;
	y0 = aline.m_cVertex[index].fy;

	pView->m_fCurrentSnapX = x0;
	pView->m_fCurrentSnapY = y0;
	pView->Restore_LargeCrossXY();

	pView->OpenMouseCursor();

	bEDA_NeedElectricalGrid = TRUE;

	flag = 0;
	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->ElecHotPointsControl(pDC);
			pView->CloseMouseCursor();
			aline.Draw(pDC, pV);
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
				pView->ElecHotPointsControl(pDC);
				pView->CloseMouseCursor();
				aline.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aline.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				aline.m_cVertex[i].fx = fx + x0 + deltax;
				aline.m_cVertex[i].fy = fy + y0 + deltay;
			}
			
			pView->CloseMouseCursor();
			aline.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->ElecHotPointsControl(pDC);
			pView->CloseMouseCursor();
			aline.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aline.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				aline.m_cVertex[i].fx = fx + x0 + deltax;
				aline.m_cVertex[i].fy = fy + y0 + deltay;
			}

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_LINE;
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
				m_cVertex[i].fx = aline.m_cVertex[i].fx;
				m_cVertex[i].fy = aline.m_cVertex[i].fy;
			}
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aline.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aline.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				aline.m_cVertex[i].fx = fx + x0 + deltax;
				aline.m_cVertex[i].fy = fy + y0 + deltay;
			}
			aline.Draw(pDC, pV);

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
		if((m_nAttrib == LINE_WIRE)||(m_nAttrib == LINE_BUS)) 
		{
			if(bEDA_AutoOptimizeWire == TRUE) pView->AutoOptimizeWire();
		}
		if(m_nAttrib == LINE_WIRE)
		{
			if(bEDA_AutoJunction == TRUE)
			{
				pView->AutoPlaceJunction();
				pView->AutoRemoveJunction();
			}
		}

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	pView->Write_Help(szEDA_Help_Ready);
	
	bEDA_NeedElectricalGrid = FALSE;
	pView->m_fpHotPoint1.fx = pView->m_fpHotPoint1.fy = pView->m_fpHotPoint2.fx = pView->m_fpHotPoint2.fy = 0;
}

BOOL COb_schline::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	
	char szHelp[] = "复制连线: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	
	
	COb_schline aline;
	aline.pDocument = pDoc;
	aline.m_nAttrib = m_nAttrib;
	aline.m_nLineWidth = m_nLineWidth;
	aline.m_nLineType = m_nLineType;
	aline.m_nColor = m_nColor;
	aline.m_nDiagram = m_nDiagram;
	aline.m_nFlag = FLAG_NORMAL;
	
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		Fpoint fpoint;
		fpoint = m_cVertex.GetAt(i);
		aline.m_cVertex.Add(fpoint);
	}

	aline.Draw(pDC, pV);
	pView->OpenMouseCursor();

	x0 = pView->m_fCurrentSnapX;
	y0 = pView->m_fCurrentSnapY;

	double dis = 10000000;
	int index;
	for(int i=0; i<aline.m_cVertex.GetCount(); i++)
	{
		double d = sqrt((x0 - aline.m_cVertex[i].fx)*(x0 - aline.m_cVertex[i].fx) + (y0 - aline.m_cVertex[i].fy)*(y0 - aline.m_cVertex[i].fy));
		if(d < dis)
		{
			dis = d;
			index = i;
		}
	}
	x0 = aline.m_cVertex[index].fx;
	y0 = aline.m_cVertex[index].fy;

	bEDA_NeedElectricalGrid = TRUE;

	flag = 0;
	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			aline.Draw(pDC, pV);
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
				pView->ElecHotPointsControl(pDC);

				pView->CloseMouseCursor();
				aline.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aline.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				aline.m_cVertex[i].fx = fx + x0 + deltax;
				aline.m_cVertex[i].fy = fy + y0 + deltay;
			}
			
			pView->CloseMouseCursor();
			aline.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			aline.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aline.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				aline.m_cVertex[i].fx = fx + x0 + deltax;
				aline.m_cVertex[i].fy = fy + y0 + deltay;
			}
			for(int i=0; i<m_cVertex.GetCount(); i++)
			{
				m_cVertex[i].fx = aline.m_cVertex[i].fx;
				m_cVertex[i].fy = aline.m_cVertex[i].fy;
			}
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aline.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aline.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				aline.m_cVertex[i].fx = fx + x0 + deltax;
				aline.m_cVertex[i].fy = fy + y0 + deltay;
			}
			aline.Draw(pDC, pV);

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

	bEDA_NeedElectricalGrid = FALSE;
	pView->m_fpHotPoint1.fx = pView->m_fpHotPoint1.fy = pView->m_fpHotPoint2.fx = pView->m_fpHotPoint2.fy = 0;

	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;
}

void COb_schline::Copy(COb_schline* pline)
{
	m_nAttrib = pline->m_nAttrib;

	m_nLineWidth = pline->m_nLineWidth;
	m_nLineType = pline->m_nLineType;
	m_nColor = pline->m_nColor;

	m_nDiagram = pline->m_nDiagram;
	
	//m_bSelection = pline->m_bSelection;
	m_bSelection = FALSE;

	m_nFlag = pline->m_nFlag;
	
	m_cVertex.RemoveAll();
	for(int i=0; i<pline->m_cVertex.GetCount(); i++)
	{
		Fpoint fp;
		fp = pline->m_cVertex.GetAt(i);
		m_cVertex.Add(fp);
	}

	pDocument = pline->pDocument;
}

void COb_schline::CombineVertex(void)
{
	for(int k=2; k<m_cVertex.GetCount(); k++)
	{
		if(m_cVertex[k-2].fx == m_cVertex[k-1].fx)
		{
			if(m_cVertex[k-1].fx == m_cVertex[k].fx)
			{
				m_cVertex.RemoveAt(k-1);
				k--;
			}
		}
		else if(m_cVertex[k-2].fy == m_cVertex[k-1].fy)
		{
			if(m_cVertex[k-1].fy == m_cVertex[k].fy)
			{
				m_cVertex.RemoveAt(k-1);
				k--;
			}
		}
	}
}

void COb_schline::DrawHighlight(CDC* pDC, CView* pV, COLORREF color)
{
	int x1,y1,x2,y2,width;
	CPen linepen, *oldpen;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_Print_Color == 0) color = RGB(0,0,0); 
		else if(nEDA_Print_Color == 2) color = RGB(GetGray(color), GetGray(color), GetGray(color));
		else color = color;
	}
	
	width = nEDA_Bus_Width[3];
	width = (int)(width/pView->m_fScale);
	if(m_nLineWidth == LINE_SMALLEST) width = 1;
	if(pView->m_nPrint_Flag == 1) width = (int)(width*nEDA_Print_Xdpi/1000*fEDA_Print_Scale);

	if(width < 1) width = 1;	

	linepen.CreatePen(PS_SOLID, width, color);
	oldpen = pDC->SelectObject(&linepen);

	for(int i =0; i<m_cVertex.GetCount()-1; i++)
	{
		pView->Get_VPort_XY(&x1,&y1,m_cVertex[i].fx,m_cVertex[i].fy);
		pView->Get_VPort_XY(&x2,&y2,m_cVertex[i+1].fx,m_cVertex[i+1].fy);
		if(pView->m_nPrint_Flag == 1)
		{
			x1 = (int)(((double)x1*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
			y1 = (int)(((double)y1*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
			x2 = (int)(((double)x2*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
			y2 = (int)(((double)y2*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
		}
		pDC->MoveTo(x1,y1);
		pDC->LineTo(x2,y2);
	}

	pDC->SelectObject(oldpen);

	//Draw(pDC, pV);
}