#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditBezier.h"
#include "Dlg_Global_Bus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_schbezier, CObject, 0)
COb_schbezier::COb_schbezier()
{
	//CArray支持自动扩展，初始设置参数是避免polyline存贮过于“碎片”化
	//m_cVertex.SetSize(10);
	m_nFlag = 0;
	m_bSelection = FALSE;
	m_nOwnerPartID = 0;
	m_nDisplayMode = 0;
	m_posParent = NULL;

	pDocument = NULL;
}


void COb_schbezier::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nWidth;
		ar << m_nColor;
		
		int vertexNum = m_cVertex.GetCount();
		ar << vertexNum;
		for(int i=0; i<vertexNum; i++)
		{
			ar << m_cVertex[i].fx;
			ar << m_cVertex[i].fy;
		}

		ar << m_nOwnerPartID;
		ar << m_nDisplayMode;
		ar << m_nDiagram;
		ar << m_bSelection;
	}
	else
	{
		ar >> m_nWidth;
		ar >> m_nColor;
		
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
		ar >> m_nDiagram;
		ar >> m_bSelection;
	}
}

void COb_schbezier::Draw(CDC* pDC, CView* pV)
{
	int x1,y1,x2,y2,x3,y3,x4,y4,width;
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
		else color = m_nColor; //彩色打印
	}	
	
	width = nEDA_Line_Width[m_nWidth];

	width = (int)(width/pView->m_fScale);
	if(pView->m_nPrint_Flag == 1) width = (int)(width*nEDA_Print_Xdpi/1000*fEDA_Print_Scale);
	
	if(width < 1) width = 1;

	linepen.CreatePen(PS_SOLID, width, color);
	oldpen = pDC->SelectObject(&linepen);

	int count = m_cVertex.GetSize();
	if(count < 2) return;
	POINT point[4];

	if( ((count-1)/3) >= 1)
	{
		for(int i = 0; i<(count-1)/3 ; i++)
		{
			pView->Get_VPort_XY(&x1,&y1,m_cVertex[i*3].fx,m_cVertex[i*3].fy);
			pView->Get_VPort_XY(&x2,&y2,m_cVertex[i*3+1].fx,m_cVertex[i*3+1].fy);
			pView->Get_VPort_XY(&x3,&y3,m_cVertex[i*3+2].fx,m_cVertex[i*3+2].fy);
			pView->Get_VPort_XY(&x4,&y4,m_cVertex[i*3+3].fx,m_cVertex[i*3+3].fy);
			if(pView->m_nPrint_Flag == 1)
			{
				x1 = (int)(((double)x1*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
				y1 = (int)(((double)y1*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
				x2 = (int)(((double)x2*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
				y2 = (int)(((double)y2*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
				x3 = (int)(((double)x3*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
				y3 = (int)(((double)y3*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
				x4 = (int)(((double)x4*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
				y4 = (int)(((double)y4*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
			}
			point[0].x = x1; point[0].y = y1;
			point[1].x = x2; point[1].y = y2;
			point[2].x = x3; point[2].y = y3;
			point[3].x = x4; point[3].y = y4;
			pDC->PolyBezier(point, 4);
		}
	}
	else
	{
		COb_schline schline;
		schline.m_nFlag = m_nFlag;
		schline.m_nDiagram = m_nDiagram;
		schline.m_nAttrib = LINE_WIRE;
		schline.m_nColor = m_nColor;
		schline.m_nLineType = LINE_SOLID;
		schline.m_nLineWidth = m_nWidth;
		
		int itemp = (count - 1)/3;
		if(((count-1)%3) == 1)
		{
			schline.m_cVertex.SetSize(2);

			schline.m_cVertex[0].fx = m_cVertex[itemp*3].fx;
			schline.m_cVertex[0].fy = m_cVertex[itemp*3].fy;
			schline.m_cVertex[1].fx = m_cVertex[itemp*3+1].fx;
			schline.m_cVertex[1].fy = m_cVertex[itemp*3+1].fy;
			if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
		}
		else if(((count-1)%3) == 2)
		{
			schline.m_cVertex.SetSize(3);

			schline.m_cVertex[0].fx = m_cVertex[itemp*3].fx;
			schline.m_cVertex[0].fy = m_cVertex[itemp*3].fy;
			schline.m_cVertex[1].fx = m_cVertex[itemp*3+1].fx;
			schline.m_cVertex[1].fy = m_cVertex[itemp*3+1].fy;
			schline.m_cVertex[2].fx = m_cVertex[itemp*3+2].fx;
			schline.m_cVertex[2].fy = m_cVertex[itemp*3+2].fy;
			if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
		}

	}


	pDC->SelectObject(oldpen);
}


void COb_schbezier::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	int x1,y1,x2,y2,x3,y3,x4,y4,width;
	CPen linepen, *oldpen;
	COLORREF color;
	
	color = m_nColor; 
		
	width = nEDA_Line_Width[m_nWidth];

	width = (int)(width/scale);
	if(width < 1) width = 1;

	linepen.CreatePen(PS_SOLID, width, color);
	oldpen = pDC->SelectObject(&linepen);

	int count = m_cVertex.GetSize();
	if(count < 2) return;
	POINT point[4];

	if( ((count-1)/3) >= 1)
	{
		for(int i = 0; i<(count-1)/3 ; i++)
		{
			//pView->Get_VPort_XY(&x1,&y1,m_cVertex[i*3].fx,m_cVertex[i*3].fy);
			x1 = (int)((m_cVertex[i*3].fx - origin_x)/scale);
			y1 = nEy - (int)((m_cVertex[i*3].fy - origin_y)/scale);
			//pView->Get_VPort_XY(&x2,&y2,m_cVertex[i*3+1].fx,m_cVertex[i*3+1].fy);
			x2 = (int)((m_cVertex[i*3+1].fx - origin_x)/scale);
			y2 = nEy - (int)((m_cVertex[i*3+1].fy - origin_y)/scale);
			//pView->Get_VPort_XY(&x3,&y3,m_cVertex[i*3+2].fx,m_cVertex[i*3+2].fy);
			x3 = (int)((m_cVertex[i*3+2].fx - origin_x)/scale);
			y3 = nEy - (int)((m_cVertex[i*3+2].fy - origin_y)/scale);
			//pView->Get_VPort_XY(&x4,&y4,m_cVertex[i*3+3].fx,m_cVertex[i*3+3].fy);
			x4 = (int)((m_cVertex[i*3+3].fx - origin_x)/scale);
			y4 = nEy - (int)((m_cVertex[i*3+3].fy - origin_y)/scale);

			point[0].x = x1; point[0].y = y1;
			point[1].x = x2; point[1].y = y2;
			point[2].x = x3; point[2].y = y3;
			point[3].x = x4; point[3].y = y4;
			pDC->PolyBezier(point, 4);
		}
	}
	else
	{
		COb_schline schline;
		schline.m_nFlag = 0;
		schline.m_nDiagram = m_nDiagram;
		schline.m_nAttrib = LINE_WIRE;
		schline.m_nColor = m_nColor;
		schline.m_nLineType = LINE_SOLID;
		schline.m_nLineWidth = LINE_SMALL;
		
		int itemp = (count - 1)/3;
		if(((count-1)%3) == 1)
		{
			schline.m_cVertex.SetSize(2);

			schline.m_cVertex[0].fx = m_cVertex[itemp*3].fx;
			schline.m_cVertex[0].fy = m_cVertex[itemp*3].fy;
			schline.m_cVertex[1].fx = m_cVertex[itemp*3+1].fx;
			schline.m_cVertex[1].fy = m_cVertex[itemp*3+1].fy;
			schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		}
		else if(((count-1)%3) == 2)
		{
			schline.m_cVertex.SetSize(3);

			schline.m_cVertex[0].fx = m_cVertex[itemp*3].fx;
			schline.m_cVertex[0].fy = m_cVertex[itemp*3].fy;
			schline.m_cVertex[1].fx = m_cVertex[itemp*3+1].fx;
			schline.m_cVertex[1].fy = m_cVertex[itemp*3+1].fy;
			schline.m_cVertex[2].fx = m_cVertex[itemp*3+2].fx;
			schline.m_cVertex[2].fy = m_cVertex[itemp*3+2].fy;
			schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		}

	}


	pDC->SelectObject(oldpen);
}

void COb_schbezier::DrawSelection(CDC* pDC, CView* pV)
{
	int x1,y1,x2,y2;
	CPen linepen, *oldpen;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	linepen.CreatePen(PS_DOT, 1, nEDA_SelectionColor);
	oldpen = pDC->SelectObject(&linepen);

	int count = m_cVertex.GetSize();
	if(count < 2) return;

	for(int i =0; i<m_cVertex.GetCount()-1; i++)
	{
		pView->Get_VPort_XY(&x1,&y1,m_cVertex[i].fx,m_cVertex[i].fy);
		pView->Get_VPort_XY(&x2,&y2,m_cVertex[i+1].fx,m_cVertex[i+1].fy);

		pDC->MoveTo(x1,y1);
		pDC->LineTo(x2,y2);
	}

	for(int i =0; i<m_cVertex.GetCount(); i++)
	{
		int x,y;
		pView->Get_VPort_XY(&x,&y,m_cVertex[i].fx,m_cVertex[i].fy);
		pView->DrawSelectionKeyPoint(x, y, pDC);
	}
	
	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}

BOOL COb_schbezier::PickMe(CView* pV)
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

BOOL COb_schbezier::Is_On_Me(double fx, double fy)
{
	int width;
	width = nEDA_Line_Width[m_nWidth];
	int delta = (int)(width/2) + nEDA_Selection_Offset;

	int vertex = m_cVertex.GetCount();
	for(int i=0; i<vertex-1; i++)
	{
		CRgn   rgn;
		rgn.CreateRectRgn( (int)(m_cVertex[i].fx - delta), (int)(m_cVertex[i].fy - delta), (int)(m_cVertex[i].fx + delta), (int)(m_cVertex[i].fy + delta) );
		if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	}

	return FALSE;
}

BOOL COb_schbezier::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
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

void COb_schbezier::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_EditBezier dlg;
	
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
							{
								COb_schbezier* pbezier = (COb_schbezier*)pobject;
								if((pbezier->m_nDiagram != nDiagram)||(pbezier->m_nFlag != FLAG_NORMAL))	continue;
								if(pbezier == this) continue; 

								if( (((subdlg.m_bCheckColor == TRUE)&&(pbezier->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckWidth == TRUE)&&(pbezier->m_nWidth == m_nWidth))||(subdlg.m_bCheckWidth == FALSE))  )
								{
									if((pbezier->m_nColor == m_nColor)||(pbezier->m_nWidth == m_nWidth))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_BEZIER;
										pundo->m_nDiagram = nDiagram;
										pundo->m_nPos = posbak;
										pundo->schbezier.Copy(pbezier);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(pbezier->m_nColor == m_nColor) pbezier->m_nColor = dlg.m_nColor;
									if(pbezier->m_nWidth == m_nWidth) pbezier->m_nWidth = dlg.m_nComboWidth;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
						{
							COb_schbezier* pbezier = (COb_schbezier*)pobject;
							if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag != FLAG_NORMAL))	continue;
							if(pbezier == this) continue; 

							if( (((subdlg.m_bCheckColor == TRUE)&&(pbezier->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(pbezier->m_nWidth == m_nWidth))||(subdlg.m_bCheckWidth == FALSE))  )
							{
								if((pbezier->m_nColor == m_nColor)||(pbezier->m_nWidth == m_nWidth))
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_BEZIER;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schbezier.Copy(pbezier);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								}
								if(pbezier->m_nColor == m_nColor) pbezier->m_nColor = dlg.m_nColor;
								if(pbezier->m_nWidth == m_nWidth) pbezier->m_nWidth = dlg.m_nComboWidth;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
						{
							COb_schbezier* pbezier = (COb_schbezier*)pobject;
							if((pbezier->m_bSelection != TRUE)||(pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag != FLAG_NORMAL))	continue;
							if(pbezier == this) continue; //排除当前bezier

							if( (((subdlg.m_bCheckColor == TRUE)&&(pbezier->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(pbezier->m_nWidth == m_nWidth))||(subdlg.m_bCheckWidth == FALSE))  )
							{
								if((pbezier->m_nColor == m_nColor)||(pbezier->m_nWidth == m_nWidth))
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_BEZIER;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schbezier.Copy(pbezier);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								}
								if(pbezier->m_nColor == m_nColor) pbezier->m_nColor = dlg.m_nColor;
								if(pbezier->m_nWidth == m_nWidth) pbezier->m_nWidth = dlg.m_nComboWidth;
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
				pundo->m_nOb = SCH_ELEMENT_BEZIER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schbezier.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

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
			pundo->m_nOb = SCH_ELEMENT_BEZIER;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->schbezier.Copy(this);
				
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

			m_nWidth = dlg.m_nComboWidth;
			m_nColor = dlg.m_nColor;
		}
	}
}

void COb_schbezier::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE; 
	
	char szHelp[] = "移动样条曲线: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV);

	x0 = m_cVertex[0].fx;
	y0 = m_cVertex[0].fy;

	pView->m_fCurrentSnapX = x0;
	pView->m_fCurrentSnapY = y0;
	pView->Restore_LargeCrossXY();
	
	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schbezier abezier;
	abezier.pDocument = pDoc;
	abezier.m_nWidth = m_nWidth;
	abezier.m_nColor = m_nColor;
	abezier.m_nDiagram = m_nDiagram;
	abezier.m_nFlag = FLAG_NORMAL;
	
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		Fpoint fpoint;
		fpoint = m_cVertex.GetAt(i);
		abezier.m_cVertex.Add(fpoint);
	}

	abezier.Draw(pDC, pV);
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
			abezier.Draw(pDC, pV);
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
				abezier.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<abezier.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				abezier.m_cVertex[i].fx = fx + x0 + deltax;
				abezier.m_cVertex[i].fy = fy + y0 + deltay;
			}
			
			pView->CloseMouseCursor();
			abezier.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			abezier.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<abezier.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				abezier.m_cVertex[i].fx = fx + x0 + deltax;
				abezier.m_cVertex[i].fy = fy + y0 + deltay;
			}

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_BEZIER;
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
				m_cVertex[i].fx = abezier.m_cVertex[i].fx;
				m_cVertex[i].fy = abezier.m_cVertex[i].fy;
			}
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			abezier.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<abezier.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				abezier.m_cVertex[i].fx = fx + x0 + deltax;
				abezier.m_cVertex[i].fy = fy + y0 + deltay;
			}
			abezier.Draw(pDC, pV);

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

BOOL COb_schbezier::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;

	char szHelp[] = "复制样条曲线: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	x0 = m_cVertex[0].fx;
	y0 = m_cVertex[0].fy;
	
	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schbezier abezier;
	abezier.pDocument = pDoc;
	abezier.m_nWidth = m_nWidth;
	abezier.m_nColor = m_nColor;
	abezier.m_nDiagram = m_nDiagram;
	abezier.m_nFlag = FLAG_NORMAL;
	
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		Fpoint fpoint;
		fpoint = m_cVertex.GetAt(i);
		abezier.m_cVertex.Add(fpoint);
	}

	abezier.Draw(pDC, pV);
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
			abezier.Draw(pDC, pV);
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
				abezier.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<abezier.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				abezier.m_cVertex[i].fx = fx + x0 + deltax;
				abezier.m_cVertex[i].fy = fy + y0 + deltay;
			}
			
			pView->CloseMouseCursor();
			abezier.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			abezier.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<abezier.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				abezier.m_cVertex[i].fx = fx + x0 + deltax;
				abezier.m_cVertex[i].fy = fy + y0 + deltay;
			}

			for(int i=0; i<m_cVertex.GetCount(); i++)
			{
				m_cVertex[i].fx = abezier.m_cVertex[i].fx;
				m_cVertex[i].fy = abezier.m_cVertex[i].fy;
			}
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			abezier.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<abezier.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				abezier.m_cVertex[i].fx = fx + x0 + deltax;
				abezier.m_cVertex[i].fy = fy + y0 + deltay;
			}
			abezier.Draw(pDC, pV);

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

void COb_schbezier::Copy(COb_schbezier* pbezier)
{
	m_nWidth = pbezier->m_nWidth;
	m_nColor = pbezier->m_nColor;

	m_cVertex.RemoveAll();
	for(int i=0; i<pbezier->m_cVertex.GetCount(); i++)
	{
		Fpoint fp;
		fp = pbezier->m_cVertex.GetAt(i);
		m_cVertex.Add(fp);
	}
	
	m_nOwnerPartID = pbezier->m_nOwnerPartID;
	m_nDisplayMode = pbezier->m_nDisplayMode;
	m_posParent = pbezier->m_posParent;

	m_nDiagram = pbezier->m_nDiagram;
	m_bSelection = pbezier->m_bSelection;
	m_nFlag = pbezier->m_nFlag;

	pDocument = pbezier->pDocument;
}