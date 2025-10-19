#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditJunction.h"
#include "Dlg_Global_Bus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_schjunc, CObject, 0)
COb_schjunc::COb_schjunc()
{
	// This empty constructor should be used by serialization only
	m_nFlag = 0 ;
	m_bSelection = FALSE;
	m_bManual = FALSE;

	pDocument = NULL;
}


void COb_schjunc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fCx;
		ar << m_fCy;
		ar << m_nSize;
		ar << m_nColor;
		ar << m_bManual;
		ar << m_nDiagram;
		ar << m_bSelection;
	}
	else
	{
		ar >> m_fCx;
		ar >> m_fCy;
		ar >> m_nSize;
		ar >> m_nColor;
		ar >> m_bManual;
		ar >> m_nDiagram;
		ar >> m_bSelection;	
	}
}

void COb_schjunc::Draw(CDC* pDC, CView* pV)
{
	int r;
	r = nEDA_Junc_Size[m_nSize];

	COb_schellipsefill schef;
	schef.m_nFlag = m_nFlag;
	schef.m_nDiagram = m_nDiagram;
	schef.m_nBorderWidth = 0;
	schef.m_nBorderColor = m_nColor;
	schef.m_nFillColor = m_nColor;
	schef.m_bFillSolid = TRUE;
	schef.m_bTransparent = FALSE;
	schef.m_fRadius_x = schef.m_fRadius_y = (double)r;
	schef.m_fCx = m_fCx;
	schef.m_fCy = m_fCy;

	if(schef.PickMe(pV) == TRUE)
	{
		schef.Draw(pDC, pV);

		//Hot Point
		CESDView* pView = (CESDView*)pV;
		if((pView->m_fScale < 10)&&(nEDA_Print_Flag == 0)&&(m_bManual != FALSE))
		{
			CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
			CPen apen;
			apen.CreatePen(PS_SOLID, 1, pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor);
			CPen* oldpen = pDC->SelectObject(&apen);
			int x, y, x0, y0;
			pView->Get_VPort_XY(&x, &y, m_fCx, m_fCy);
			x0 = x + 1; y0 = y + 1;
			pDC->MoveTo(x0, y0); pDC->LineTo(x0+1,y0);pDC->LineTo(x0+1,y0+1);pDC->LineTo(x0,y0+1);pDC->LineTo(x0,y0);
			x0 = x + 1; y0 = y - 3;
			pDC->MoveTo(x0, y0); pDC->LineTo(x0+1,y0);pDC->LineTo(x0+1,y0+1);pDC->LineTo(x0,y0+1);pDC->LineTo(x0,y0);
			x0 = x - 3; y0 = y - 3;
			pDC->MoveTo(x0, y0); pDC->LineTo(x0+1,y0);pDC->LineTo(x0+1,y0+1);pDC->LineTo(x0,y0+1);pDC->LineTo(x0,y0);
			x0 = x - 3; y0 = y + 1;
			pDC->MoveTo(x0, y0); pDC->LineTo(x0+1,y0);pDC->LineTo(x0+1,y0+1);pDC->LineTo(x0,y0+1);pDC->LineTo(x0,y0);
			pDC->SelectObject(oldpen);
		}
	}

}

void COb_schjunc::DrawSelection(CDC* pDC, CView* pV)
{
	int cx, cy, r;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
	r = (int)(nEDA_Junc_Size[m_nSize]/pView->m_fScale);

	int oldbkmode = pDC->SetBkMode(TRANSPARENT);
	
	CPen arcpen;
	arcpen.CreatePen(PS_DOT, 1, nEDA_SelectionColor);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	pDC->MoveTo(cx - r, cy - r);
	pDC->LineTo(cx - r, cy + r);
	pDC->LineTo(cx + r, cy + r);
	pDC->LineTo(cx + r, cy - r);	
	pDC->LineTo(cx - r, cy - r);


	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}

BOOL  COb_schjunc::Is_On_Me(double fx, double fy)
{

	int r;
	r = nEDA_Junc_Size[m_nSize];

	CRgn rgn;
	rgn.CreateEllipticRgn((int)(m_fCx - r), (int)(m_fCy - r), (int)(m_fCx + r), (int)(m_fCy + r) );

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

BOOL  COb_schjunc::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	int r;
	r = nEDA_Junc_Size[m_nSize];

	double fminx, fminy, fmaxx, fmaxy;
	fminx = m_fCx - r;
	fminy = m_fCy - r;
	fmaxx = m_fCx + r;
	fmaxy = m_fCy + r;
	
	if((fminx < fx1)||(fmaxx > fx2)||(fminy < fy1)||(fmaxy > fy2)) return FALSE;
	return TRUE;
}

void COb_schjunc::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_EditJunction dlg;

	dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fCx);
	dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fCy);
	dlg.m_nComboJunctionSize = m_nSize;
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
							{
								COb_schjunc* pjunc = (COb_schjunc*)pobject;
								if((pjunc->m_nDiagram != nDiagram)||(pjunc->m_nFlag != FLAG_NORMAL))	continue;
								if(pjunc == this) continue; 

								if( (((subdlg.m_bCheckColor == TRUE)&&(pjunc->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckWidth == TRUE)&&(pjunc->m_nSize == m_nSize))||(subdlg.m_bCheckWidth == FALSE))  )
								{
									if((pjunc->m_nColor == m_nColor)||(pjunc->m_nSize == m_nSize))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_JUNCTION;
										pundo->m_nDiagram = nDiagram;
										pundo->m_nPos = posbak;
										pundo->schjunc.Copy(pjunc);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

									}
									if(pjunc->m_nColor == m_nColor) pjunc->m_nColor = dlg.m_nColor;
									if(pjunc->m_nSize == m_nSize) pjunc->m_nSize = dlg.m_nComboJunctionSize;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
						{
							COb_schjunc* pjunc = (COb_schjunc*)pobject;
							if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag != FLAG_NORMAL))	continue;
							if(pjunc == this) continue; 

							if( (((subdlg.m_bCheckColor == TRUE)&&(pjunc->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(pjunc->m_nSize == m_nSize))||(subdlg.m_bCheckWidth == FALSE))  )
							{
								if((pjunc->m_nColor == m_nColor)||(pjunc->m_nSize == m_nSize))
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_JUNCTION;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schjunc.Copy(pjunc);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

								}
								if(pjunc->m_nColor == m_nColor) pjunc->m_nColor = dlg.m_nColor;
								if(pjunc->m_nSize == m_nSize) pjunc->m_nSize = dlg.m_nComboJunctionSize;
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
				else	//选择
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
						{
							COb_schjunc* pjunc = (COb_schjunc*)pobject;
							if((pjunc->m_bSelection != TRUE)||(pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag != FLAG_NORMAL))	continue;
							if(pjunc == this) continue;

							if( (((subdlg.m_bCheckColor == TRUE)&&(pjunc->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
								(((subdlg.m_bCheckWidth == TRUE)&&(pjunc->m_nSize == m_nSize))||(subdlg.m_bCheckWidth == FALSE))  )
							{
								if((pjunc->m_nColor == m_nColor)||(pjunc->m_nSize == m_nSize))
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_JUNCTION;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schjunc.Copy(pjunc);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

								}
								if(pjunc->m_nColor == m_nColor) pjunc->m_nColor = dlg.m_nColor;
								if(pjunc->m_nSize == m_nSize) pjunc->m_nSize = dlg.m_nComboJunctionSize;
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
				pundo->m_nOb = SCH_ELEMENT_JUNCTION;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schjunc.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		
				m_fCx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
				m_fCy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
				m_nSize = dlg.m_nComboJunctionSize;
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
			pundo->m_nOb = SCH_ELEMENT_JUNCTION;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->schjunc.Copy(this);
				
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

			m_fCx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
			m_fCy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
			m_nSize = dlg.m_nComboJunctionSize;
			m_nColor = dlg.m_nColor;
		}
	}
}

void COb_schjunc::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	BOOL clear_undo_buffer_flag = FALSE;
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动结点: <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV);

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schjunc ajunc;
	ajunc.pDocument = pDoc;
	ajunc.m_fCx = m_fCx;
	ajunc.m_fCy = m_fCy;
	ajunc.m_nSize = m_nSize;
	ajunc.m_nColor = m_nColor;
	ajunc.m_bManual = m_bManual;
	ajunc.m_nDiagram = m_nDiagram;
	ajunc.Draw(pDC, pV);

	pView->m_fCurrentSnapX = ajunc.m_fCx;
	pView->m_fCurrentSnapY = ajunc.m_fCy;
	pView->Restore_LargeCrossXY();
	
	pView->OpenMouseCursor();

	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			ajunc.Draw(pDC, pV);
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
				ajunc.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			ajunc.m_fCx = pView->m_fCurrentSnapX;
			ajunc.m_fCy = pView->m_fCurrentSnapY;

			pView->CloseMouseCursor();
			ajunc.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			ajunc.Draw(pDC, pV);
			pView->OpenMouseCursor();

			ajunc.m_fCx = pView->m_fCurrentSnapX;
			ajunc.m_fCy = pView->m_fCurrentSnapY;

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_JUNCTION;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->m_fX1 = m_fCx;
			pundo->m_fY1 = m_fCy;
			
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fCx = ajunc.m_fCx;
			m_fCy = ajunc.m_fCy;

			break;
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
		if(bEDA_AutoJunction == TRUE) pView->AutoRemoveJunction();

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	pView->Write_Help(szEDA_Help_Ready);
}

BOOL COb_schjunc::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	
	char szHelp[] = "复制结点: <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schjunc ajunc;
	ajunc.pDocument = pDoc;
	ajunc.m_fCx = pView->m_fCurrentSnapX;
	ajunc.m_fCy = pView->m_fCurrentSnapY;
	ajunc.m_nSize = m_nSize;
	ajunc.m_nColor = m_nColor;
	ajunc.m_bManual = m_bManual;
	ajunc.m_nDiagram = m_nDiagram;

	ajunc.Draw(pDC, pV);
	pView->OpenMouseCursor();

	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			ajunc.Draw(pDC, pV);
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
				ajunc.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			ajunc.m_fCx = pView->m_fCurrentSnapX;
			ajunc.m_fCy = pView->m_fCurrentSnapY;

			pView->CloseMouseCursor();
			ajunc.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			ajunc.Draw(pDC, pV);
			pView->OpenMouseCursor();

			ajunc.m_fCx = pView->m_fCurrentSnapX;
			ajunc.m_fCy = pView->m_fCurrentSnapY;

			m_fCx = ajunc.m_fCx;
			m_fCy = ajunc.m_fCy;

			break;
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

void COb_schjunc::Copy(COb_schjunc* pschjunc)
{
	m_fCx = pschjunc->m_fCx;
	m_fCy = pschjunc->m_fCy;
	m_nSize = pschjunc->m_nSize;
	m_nColor = pschjunc->m_nColor;
	m_bManual = pschjunc->m_bManual;

	m_nDiagram = pschjunc->m_nDiagram;
	m_bSelection = pschjunc->m_bSelection;
	m_nFlag = pschjunc->m_nFlag;
	
	//pos
	pDocument = pschjunc->pDocument;
}