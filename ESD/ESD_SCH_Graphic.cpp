#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditGraphic.h"
#include "dib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_schgraphic, CObject, 0)
COb_schgraphic::COb_schgraphic()
{
	// This empty constructor should be used by serialization only
	m_nFlag = 0 ;
	m_bSelection = FALSE;
	m_nOwnerPartID = 0;
	m_nDisplayMode = 0;
	m_posParent = NULL;

	pDocument = NULL;
}


void COb_schgraphic::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fX1;
		ar << m_fY1;
		ar << m_fX2;
		ar << m_fY2;

		ar << m_nBorderWidth;
		ar << m_nBorderColor;
		ar << m_bAspect;
		ar << m_bBorderOn;
		ar << m_bEmbedded;
		ar << m_csFileName;

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
		ar >> m_nBorderColor;
		ar >> m_bAspect;
		ar >> m_bBorderOn;
		ar >> m_bEmbedded;
		ar >> m_csFileName;

		ar >> m_nOwnerPartID;
		ar >> m_nDisplayMode;

		ar >> m_nDiagram;
		ar >> m_bSelection;		
	}
}

void COb_schgraphic::Draw(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	CDib m_cDib;

	COLORREF color;

	if(m_nFlag == FLAG_NORMAL) color = m_nBorderColor;
	else if(m_nFlag == FLAG_DELETED) color = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor;

	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_Print_Color == 0) color = RGB(0,0,0);
		else if(nEDA_Print_Color == 2) color = RGB(GetGray(m_nBorderColor),GetGray(m_nBorderColor),GetGray(m_nBorderColor));
		else color = m_nBorderColor;
	}


	CFile cfile;
	if((cfile.Open(m_csFileName, CFile::modeRead) != 0)&&(m_cDib.Read(cfile) != 0))
	{
		RECT dcRect, picRect;

		picRect.left = m_cDib.Width();
		picRect.bottom = m_cDib.Height();
		picRect.right = 0; 
		picRect.top = 0;

		int x1,y1,x2,y2;
		pView->Get_VPort_XY(&x1, &y1, m_fX1, m_fY1);
		pView->Get_VPort_XY(&x2, &y2, m_fX2, m_fY2);
		if(pView->m_nPrint_Flag == 1)
		{
			x1 = (int)(((double)x1*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
			y1 = (int)(((double)y1*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
			x2 = (int)(((double)x2*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
			y2 = (int)(((double)y2*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
		}		
		
		if(m_bAspect == TRUE)
		{
			double f1,f2;
			f1 = abs((double)picRect.left/picRect.bottom);
			f2 = abs((double)(x2-x1)/(y2-y1));
			if(f1 > f2)
			{
				dcRect.left = x2;
				dcRect.bottom = y1;
				dcRect.right = x1;
				dcRect.top = y1 - (x2-x1);
			}
			else
			{
				dcRect.left = x1-(y2-y1);
				dcRect.bottom = y1;
				dcRect.right = x1;
				dcRect.top = y2;
			}
		}
		else
		{
			dcRect.left = x2;
			dcRect.bottom = y1;
			dcRect.right = x1;
			dcRect.top = y2;
		}

		m_cDib.Paint(pDC->m_hDC, &dcRect, &picRect);
		
		cfile.Close();
	}
	else
	{
		COb_schtext schtext;
		schtext.m_nFlag = m_nFlag;
		schtext.m_nDiagram = m_nDiagram;
		schtext.m_nColor = color;
		schtext.m_nFont = 0;
		schtext.m_nOrientation = 0;
		schtext.m_csText = CString("找不到图像文件或无法显示图像:") + m_csFileName;
		schtext.m_fSx = m_fX1;
		schtext.m_fSy = m_fY2 - 100;
		if(schtext.PickMe(pDC, pV) == TRUE) schtext.DrawTabbed(pDC, pV);
	}

	if((m_bBorderOn == TRUE)||(m_nFlag == FLAG_DELETED))
	{
		COb_schrect schrect;
		schrect.m_nFlag = m_nFlag;
		schrect.m_nDiagram = m_nDiagram;
		schrect.m_nBorderColor = color;
		schrect.m_nBorderWidth = m_nBorderWidth;
		schrect.m_bFillSolid = FALSE;
		schrect.m_bTransparent = FALSE;
		schrect.m_fX1 = m_fX1;
		schrect.m_fY1 = m_fY1;
		schrect.m_fX2 = m_fX2;
		schrect.m_fY2 = m_fY2;

		if(m_nFlag == FLAG_DELETED)
		{
			schrect.m_bFillSolid = TRUE;
		}

		schrect.Draw(pDC, pV);
	}
}

void COb_schgraphic::DrawSelection(CDC* pDC, CView* pV)
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

BOOL COb_schgraphic::PickMe(CView* pV)
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

void COb_schgraphic::LoadBitmap()
{

}

BOOL  COb_schgraphic::Is_On_Me(double fx, double fy)
{
	CRgn   rgn;
	rgn.CreateRectRgn( (int)m_fX1, (int)m_fY1, (int)m_fX2, (int)m_fY2);
	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

BOOL  COb_schgraphic::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fminx, fminy, fmaxx, fmaxy;
	fminx = MIN(m_fX1, m_fX2);
	fminy = MIN(m_fY1, m_fY2);
	fmaxx = MAX(m_fX1, m_fX2);
	fmaxy = MAX(m_fY1, m_fY2);
	
	if((fminx < fx1)||(fmaxx > fx2)||(fminy < fy1)||(fmaxy > fy2)) return FALSE;
	return TRUE;
}

void COb_schgraphic::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_EditGraphic dlg;

	dlg.m_csEditLocationX1 = pDoc->Convert_XY_To_String(m_fX1);
	dlg.m_csEditLocationY1 = pDoc->Convert_XY_To_String(m_fY1);
	dlg.m_csEditLocationX2 = pDoc->Convert_XY_To_String(m_fX2);
	dlg.m_csEditLocationY2 = pDoc->Convert_XY_To_String(m_fY2);
	dlg.m_nComboWidth = m_nBorderWidth;
	dlg.m_nColor = m_nBorderColor;
	dlg.m_bCheckShowBorder = m_bBorderOn;
	dlg.m_bCheckXYRatio = m_bAspect;
	dlg.m_csEditFilename = m_csFileName;
	
	if(dlg.DoModal() == IDOK)
	{
		if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = pos;
		pundo->schgraphic.Copy(this);
			
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

		m_fX1 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX1);
		m_fY1 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY1);
		m_fX2 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX2);
		m_fY2 = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY2);
		m_nBorderWidth = dlg.m_nComboWidth;
		m_nBorderColor = dlg.m_nColor;
		m_bBorderOn = dlg.m_bCheckShowBorder;
		m_bAspect = dlg.m_bCheckXYRatio;
		m_csFileName = dlg.m_csEditFilename;

	}
}

void COb_schgraphic::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	double x1,y1, x0, y0, deltax, deltay;
	BOOL clear_undo_buffer_flag = FALSE;
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动图像: <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	pView->Invalidate();

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
	arect.m_nFillColor = RGB(255,255,255);
	arect.m_nBorderColor = m_nBorderColor;
	arect.m_bTransparent = FALSE;
	arect.m_bFillSolid = TRUE;
	arect.m_nDiagram = m_nDiagram;

	arect.Draw(pDC, pV);
	pView->OpenMouseCursor();

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

			arect.m_fX1 = m_fX1 + deltax;
			arect.m_fY1 = m_fY1 + deltay;
			arect.m_fX2 = m_fX2 + deltax;
			arect.m_fY2 = m_fY2 + deltay;

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

			arect.m_fX1 = m_fX1 + deltax;
			arect.m_fY1 = m_fY1 + deltay;
			arect.m_fX2 = m_fX2 + deltax;
			arect.m_fY2 = m_fY2 + deltay;

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
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
			
			break;
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	
	m_nFlag = FLAG_NORMAL;

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;


	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	pView->Write_Help(szEDA_Help_Ready);
	pView->Invalidate();
}

BOOL COb_schgraphic::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	double x1,y1, x0, y0, deltax, deltay;
	
	char szHelp[] = "复制图像: <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	pView->Invalidate();

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
	arect.m_nFillColor = RGB(255,255,255);
	arect.m_nBorderColor = m_nBorderColor;
	arect.m_bTransparent = FALSE;
	arect.m_bFillSolid = TRUE;
	arect.m_nDiagram = m_nDiagram;

	pView->OpenMouseCursor();

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

			arect.m_fX1 = m_fX1 + deltax;
			arect.m_fY1 = m_fY1 + deltay;
			arect.m_fX2 = m_fX2 + deltax;
			arect.m_fY2 = m_fY2 + deltay;

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

			arect.m_fX1 = m_fX1 + deltax;
			arect.m_fY1 = m_fY1 + deltay;
			arect.m_fX2 = m_fX2 + deltax;
			arect.m_fY2 = m_fY2 + deltay;

			m_fX1 = arect.m_fX1;
			m_fY1 = arect.m_fY1;
			m_fX2 = arect.m_fX2;
			m_fY2 = arect.m_fY2;
			
			break;
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	
	m_nFlag = FLAG_NORMAL;

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	pView->Write_Help(szEDA_Help_Ready);
	pView->Invalidate();
		
	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;
}

void COb_schgraphic::Copy(COb_schgraphic* pschgraphic)
{
    m_fX1 = pschgraphic->m_fX1;
	m_fY1 = pschgraphic->m_fY1;
	m_fX2 = pschgraphic->m_fX2;
	m_fY2 = pschgraphic->m_fY2;

	m_nBorderWidth = pschgraphic->m_nBorderWidth;
	m_nBorderColor = pschgraphic->m_nBorderColor;
	m_bAspect = pschgraphic->m_bAspect;
	m_bBorderOn = pschgraphic->m_bBorderOn;
	m_bEmbedded = pschgraphic->m_bEmbedded;
	m_csFileName = pschgraphic->m_csFileName;

	m_nOwnerPartID = pschgraphic->m_nOwnerPartID;
	m_nDisplayMode = pschgraphic->m_nDisplayMode;
	m_posParent = pschgraphic->m_posParent;

	m_nDiagram = pschgraphic->m_nDiagram;
	m_bSelection = pschgraphic->m_bSelection;
	m_nFlag = pschgraphic->m_nFlag;

	pDocument = pschgraphic->pDocument;
}