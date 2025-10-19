#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditRect.h"
#include "Dlg_Global_Fill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_schrect, CObject, 0)
COb_schrect::COb_schrect()
{
	// This empty constructor should be used by serialization only
	m_nFlag = 0 ;
	m_bSelection = FALSE;
	m_nOwnerPartID = 0;
	m_nDisplayMode = 0;
	m_posParent = NULL;

	pDocument = NULL;
}


void COb_schrect::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fX1;
		ar << m_fY1;
		ar << m_fX2;
		ar << m_fY2;

		ar << m_nBorderWidth;
		ar << m_nFillColor;
		ar << m_nBorderColor;
		ar << m_bTransparent;
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

		ar >> m_nBorderWidth;
		ar >> m_nFillColor;
		ar >> m_nBorderColor;
		ar >> m_bTransparent;
		ar >> m_bFillSolid;

		ar >> m_nOwnerPartID;
		ar >> m_nDisplayMode;

		ar >> m_nDiagram;
		ar >> m_bSelection;	
	}
}

void COb_schrect::Draw(CDC* pDC, CView* pV)
{
	COb_schpolygon schpolygon;
	schpolygon.m_nFlag = 0;
	schpolygon.m_nDiagram = m_nDiagram;
	schpolygon.m_nBorderWidth = m_nBorderWidth;
	schpolygon.m_nBorderColor = m_nBorderColor;
	schpolygon.m_nFillColor = m_nFillColor;
	schpolygon.m_bFillSolid = m_bFillSolid;
	schpolygon.m_bTransparent = m_bTransparent;
	schpolygon.m_cVertex.SetSize(4);
	schpolygon.m_nFlag = m_nFlag;

	schpolygon.m_cVertex[0].fx = m_fX1;
	schpolygon.m_cVertex[0].fy = m_fY1;
	schpolygon.m_cVertex[1].fx = m_fX2;
	schpolygon.m_cVertex[1].fy = m_fY1;
	schpolygon.m_cVertex[2].fx = m_fX2;
	schpolygon.m_cVertex[2].fy = m_fY2;
	schpolygon.m_cVertex[3].fx = m_fX1;
	schpolygon.m_cVertex[3].fy = m_fY2;

	if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);

}


void COb_schrect::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	COb_schpolygon schpolygon;
	schpolygon.m_nFlag = 0;
	schpolygon.m_nDiagram = m_nDiagram;
	schpolygon.m_nBorderWidth = m_nBorderWidth;
	schpolygon.m_nBorderColor = m_nBorderColor;
	schpolygon.m_nFillColor = m_nFillColor;
	schpolygon.m_bFillSolid = m_bFillSolid;
	schpolygon.m_bTransparent = m_bTransparent;
	schpolygon.m_cVertex.SetSize(4);
	schpolygon.m_nFlag = m_nFlag;

	schpolygon.m_cVertex[0].fx = m_fX1;
	schpolygon.m_cVertex[0].fy = m_fY1;
	schpolygon.m_cVertex[1].fx = m_fX2;
	schpolygon.m_cVertex[1].fy = m_fY1;
	schpolygon.m_cVertex[2].fx = m_fX2;
	schpolygon.m_cVertex[2].fy = m_fY2;
	schpolygon.m_cVertex[3].fx = m_fX1;
	schpolygon.m_cVertex[3].fy = m_fY2;

	schpolygon.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
}

void COb_schrect::DrawSelection(CDC* pDC, CView* pV)
{
	int x1, y1, x2, y2;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	pView->Get_VPort_XY(&x1,&y1,m_fX1,m_fY1);
	pView->Get_VPort_XY(&x2,&y2,m_fX2,m_fY2);

	pView->DrawSelectionKeyPoint(x1, y1, pDC);
	pView->DrawSelectionKeyPoint(x1, y2, pDC);
	pView->DrawSelectionKeyPoint(x2, y1, pDC);
	pView->DrawSelectionKeyPoint(x2, y2, pDC);
	pView->DrawSelectionKeyPoint((x1+x2)/2, y1, pDC);
	pView->DrawSelectionKeyPoint((x1+x2)/2, y2, pDC);
	pView->DrawSelectionKeyPoint(x1, (y1+y2)/2, pDC);
	pView->DrawSelectionKeyPoint(x2, (y1+y2)/2, pDC);

}

BOOL  COb_schrect::Is_On_Me(double fx, double fy)
{
	CRgn   rgn;

	rgn.CreateRectRgn( (int)m_fX1, (int)m_fY1, (int)m_fX2, (int)m_fY2);

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

BOOL  COb_schrect::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fminx, fminy, fmaxx, fmaxy;
	fminx = MIN(m_fX1, m_fX2);
	fminy = MIN(m_fY1, m_fY2);
	fmaxx = MAX(m_fX1, m_fX2);
	fmaxy = MAX(m_fY1, m_fY2);
	
	if((fminx < fx1)||(fmaxx > fx2)||(fminy < fy1)||(fmaxy > fy2)) return FALSE;
	return TRUE;
}

void COb_schrect::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_EditRect dlg;
	
	dlg.m_csEditLocationX1 = pDoc->Convert_XY_To_String(m_fX1);
	dlg.m_csEditLocationY1 = pDoc->Convert_XY_To_String(m_fY1);
	dlg.m_csEditLocationX2 = pDoc->Convert_XY_To_String(m_fX2);
	dlg.m_csEditLocationY2 = pDoc->Convert_XY_To_String(m_fY2);
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
							{
								COb_schrect* prect = (COb_schrect*)pobject;
								if((prect->m_nDiagram != nDiagram)||(prect->m_nFlag != FLAG_NORMAL))	continue;
								if(prect == this) continue; 

								if( (((subdlg.m_bCheckColor == TRUE)&&(prect->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckWidth == TRUE)&&(prect->m_nBorderWidth == m_nBorderWidth))||(subdlg.m_bCheckWidth == FALSE)) && 
									(((subdlg.m_bCheckFillColor == TRUE)&&(prect->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
									(((subdlg.m_bCheckTransparent == TRUE)&&(prect->m_bTransparent == m_bTransparent))||(subdlg.m_bCheckTransparent == FALSE)) &&
									(((subdlg.m_bCheckSolid == TRUE)&&(prect->m_bFillSolid == m_bFillSolid))||(subdlg.m_bCheckSolid == FALSE)) )
								{
									if( (prect->m_nBorderColor == m_nBorderColor)||(prect->m_nBorderWidth == m_nBorderWidth)||(prect->m_nFillColor == m_nFillColor)||\
										(prect->m_bTransparent == m_bTransparent)||(prect->m_bFillSolid == m_bFillSolid) )
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_RECT;
										pundo->m_nDiagram = nDiagram;
										pundo->m_nPos = posbak;
										pundo->schrect.Copy(prect);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(prect->m_nBorderColor == m_nBorderColor) prect->m_nBorderColor = dlg.m_nBorderColor;
									if(prect->m_nBorderWidth == m_nBorderWidth) prect->m_nBorderWidth = dlg.m_nComboWidth;
									if(prect->m_nFillColor == m_nFillColor) prect->m_nFillColor = dlg.m_nFillColor;
									if(prect->m_bTransparent == m_bTransparent) prect->m_bTransparent = dlg.m_bCheckTransparent;
									if(prect->m_bFillSolid == m_bFillSolid) prect->m_bFillSolid = dlg.m_bCheckDrawSolid;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
						{
							COb_schrect* prect = (COb_schrect*)pobject;
							if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag != FLAG_NORMAL))	continue;
							if(prect == this) continue;

							if( (((subdlg.m_bCheckColor == TRUE)&&(prect->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(prect->m_nBorderWidth == m_nBorderWidth))||(subdlg.m_bCheckWidth == FALSE)) && 
								(((subdlg.m_bCheckFillColor == TRUE)&&(prect->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
								(((subdlg.m_bCheckTransparent == TRUE)&&(prect->m_bTransparent == m_bTransparent))||(subdlg.m_bCheckTransparent == FALSE)) &&
								(((subdlg.m_bCheckSolid == TRUE)&&(prect->m_bFillSolid == m_bFillSolid))||(subdlg.m_bCheckSolid == FALSE)) )
							{
								if( (prect->m_nBorderColor == m_nBorderColor)||(prect->m_nBorderWidth == m_nBorderWidth)||(prect->m_nFillColor == m_nFillColor)||\
									(prect->m_bTransparent == m_bTransparent)||(prect->m_bFillSolid == m_bFillSolid) )
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_RECT;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schrect.Copy(prect);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								}
								if(prect->m_nBorderColor == m_nBorderColor) prect->m_nBorderColor = dlg.m_nBorderColor;
								if(prect->m_nBorderWidth == m_nBorderWidth) prect->m_nBorderWidth = dlg.m_nComboWidth;
								if(prect->m_nFillColor == m_nFillColor) prect->m_nFillColor = dlg.m_nFillColor;
								if(prect->m_bTransparent == m_bTransparent) prect->m_bTransparent = dlg.m_bCheckTransparent;
								if(prect->m_bFillSolid == m_bFillSolid) prect->m_bFillSolid = dlg.m_bCheckDrawSolid;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
						{
							COb_schrect* prect = (COb_schrect*)pobject;
							if((prect->m_bSelection != TRUE)||(prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag != FLAG_NORMAL))	continue;
							if(prect == this) continue; 

							if( (((subdlg.m_bCheckColor == TRUE)&&(prect->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(prect->m_nBorderWidth == m_nBorderWidth))||(subdlg.m_bCheckWidth == FALSE)) && 
								(((subdlg.m_bCheckFillColor == TRUE)&&(prect->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
								(((subdlg.m_bCheckTransparent == TRUE)&&(prect->m_bTransparent == m_bTransparent))||(subdlg.m_bCheckTransparent == FALSE)) &&
								(((subdlg.m_bCheckSolid == TRUE)&&(prect->m_bFillSolid == m_bFillSolid))||(subdlg.m_bCheckSolid == FALSE)) )
							{
								if( (prect->m_nBorderColor == m_nBorderColor)||(prect->m_nBorderWidth == m_nBorderWidth)||(prect->m_nFillColor == m_nFillColor)||\
									(prect->m_bTransparent == m_bTransparent)||(prect->m_bFillSolid == m_bFillSolid) )
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_RECT;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schrect.Copy(prect);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								}
								if(prect->m_nBorderColor == m_nBorderColor) prect->m_nBorderColor = dlg.m_nBorderColor;
								if(prect->m_nBorderWidth == m_nBorderWidth) prect->m_nBorderWidth = dlg.m_nComboWidth;
								if(prect->m_nFillColor == m_nFillColor) prect->m_nFillColor = dlg.m_nFillColor;
								if(prect->m_bTransparent == m_bTransparent) prect->m_bTransparent = dlg.m_bCheckTransparent;
								if(prect->m_bFillSolid == m_bFillSolid) prect->m_bFillSolid = dlg.m_bCheckDrawSolid;
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
				pundo->m_nOb = SCH_ELEMENT_RECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schrect.Copy(this);
					
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
			pundo->m_nOb = SCH_ELEMENT_RECT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->schrect.Copy(this);
				
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

			m_bFillSolid = dlg.m_bCheckDrawSolid;
			m_bTransparent = dlg.m_bCheckTransparent;
			m_nBorderWidth = dlg.m_nComboWidth;
			m_nFillColor = dlg.m_nFillColor;
			m_nBorderColor = dlg.m_nBorderColor;
		}
	}
}

void COb_schrect::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE; 
	
	char szHelp[] = "移动方形: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

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

	COb_schrect arect;
	arect.pDocument = pDoc;
	arect.m_fX1 = m_fX1;
	arect.m_fY1 = m_fY1;
	arect.m_fX2 = m_fX2;
	arect.m_fY2 = m_fY2;
	arect.m_nBorderWidth = m_nBorderWidth;
	arect.m_nFillColor = m_nFillColor;
	arect.m_nBorderColor = m_nBorderColor;
	arect.m_bTransparent = FALSE;
	arect.m_bFillSolid = m_bFillSolid;
	arect.m_nDiagram = m_nDiagram;

	arect.Draw(pDC, pV);
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
			arect.Draw(pDC, pV);
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
				arect.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, flag*90, FALSE);
			arect.m_fX1 = fx + x0 + deltax;
			arect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, flag*90, FALSE);
			arect.m_fX2 = fx + x0 + deltax;
			arect.m_fY2 = fy + y0 + deltay;


			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, flag*90, FALSE);
			arect.m_fX1 = fx + x0 + deltax;
			arect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, flag*90, FALSE);
			arect.m_fX2 = fx + x0 + deltax;
			arect.m_fY2 = fy + y0 + deltay;

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_RECT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->m_fX1 = m_fX1;
			pundo->m_fY1 = m_fY1;
			pundo->m_fX2 = m_fX2;
			pundo->m_fY2 = m_fY2;
		
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fX1 = arect.m_fX1;
			m_fY1 = arect.m_fY1;
			m_fX2 = arect.m_fX2;
			m_fY2 = arect.m_fY2;
			
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
			arect.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, flag*90, FALSE);
			arect.m_fX1 = fx + x0 + deltax;
			arect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, flag*90, FALSE);
			arect.m_fX2 = fx + x0 + deltax;
			arect.m_fY2 = fy + y0 + deltay;

			arect.Draw(pDC, pV);

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

BOOL COb_schrect::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	
	char szHelp[] = "复制方形: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	x0 = m_fX1; 
	y0 = m_fY1; 
	
	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schrect arect;
	arect.pDocument = pDoc;
	arect.m_fX1 = m_fX1;
	arect.m_fY1 = m_fY1;
	arect.m_fX2 = m_fX2;
	arect.m_fY2 = m_fY2;
	arect.m_nBorderWidth = m_nBorderWidth;
	arect.m_nFillColor = m_nFillColor;
	arect.m_nBorderColor = m_nBorderColor;
	arect.m_bTransparent = FALSE;
	arect.m_bFillSolid = m_bFillSolid;
	arect.m_nDiagram = m_nDiagram;

	arect.Draw(pDC, pV);
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
			arect.Draw(pDC, pV);
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
				arect.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, flag*90, FALSE);
			arect.m_fX1 = fx + x0 + deltax;
			arect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, flag*90, FALSE);
			arect.m_fX2 = fx + x0 + deltax;
			arect.m_fY2 = fy + y0 + deltay;


			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			arect.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, flag*90, FALSE);
			arect.m_fX1 = fx + x0 + deltax;
			arect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, flag*90, FALSE);
			arect.m_fX2 = fx + x0 + deltax;
			arect.m_fY2 = fy + y0 + deltay;

			m_fX1 = arect.m_fX1;
			m_fY1 = arect.m_fY1;
			m_fX2 = arect.m_fX2;
			m_fY2 = arect.m_fY2;
			
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
			arect.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, flag*90, FALSE);
			arect.m_fX1 = fx + x0 + deltax;
			arect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, flag*90, FALSE);
			arect.m_fX2 = fx + x0 + deltax;
			arect.m_fY2 = fy + y0 + deltay;

			arect.Draw(pDC, pV);

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

void COb_schrect::Copy(COb_schrect* pschrect)
{
    m_fX1 = pschrect->m_fX1;
	m_fY1 = pschrect->m_fY1;
	m_fX2 = pschrect->m_fX2;
	m_fY2 = pschrect->m_fY2;

	m_nBorderWidth = pschrect->m_nBorderWidth;
	m_nFillColor = pschrect->m_nFillColor;
	m_nBorderColor = pschrect->m_nBorderColor;
	m_bTransparent = pschrect->m_bTransparent;
	m_bFillSolid = pschrect->m_bFillSolid;

	m_nOwnerPartID = pschrect->m_nOwnerPartID;
	m_nDisplayMode = pschrect->m_nDisplayMode;
	m_posParent = pschrect->m_posParent;

	m_nDiagram = pschrect->m_nDiagram;
	m_bSelection = pschrect->m_bSelection;
	m_nFlag = pschrect->m_nFlag;

	pDocument = pschrect->pDocument;
}