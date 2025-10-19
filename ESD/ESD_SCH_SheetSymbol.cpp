#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditSheetSymbol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_schsheetsymbol, CObject, 0)
COb_schsheetsymbol::COb_schsheetsymbol()
{
	// This empty constructor should be used by serialization only
	m_nFlag = 0 ;
	m_bSelection = FALSE;
	pDocument = NULL;

}


void COb_schsheetsymbol::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fX;
		ar << m_fY;
		ar << m_fXsize;
		ar << m_fYsize;

		ar << m_nBorderWidth;
		ar << m_nFillColor;
		ar << m_nBorderColor;
		ar << m_bFillSolid;
		ar << m_bShowHiddenTextFields;
		
		ar << m_csUniqueID;
		ar << m_nDiagram;
		ar << m_bSelection;
	}
	else
	{
		ar >> m_fX;
		ar >> m_fY;
		ar >> m_fXsize;
		ar >> m_fYsize;

		ar >> m_nBorderWidth;
		ar >> m_nFillColor;
		ar >> m_nBorderColor;
		ar >> m_bFillSolid;
		ar >> m_bShowHiddenTextFields;
		
		ar >> m_csUniqueID;
		ar >> m_nDiagram;
		ar >> m_bSelection;	
	}
}

void COb_schsheetsymbol::Draw(CDC* pDC, CView* pV)
{
	COb_schrect schrect;
	schrect.m_nDiagram = m_nDiagram;
	schrect.m_nBorderWidth = m_nBorderWidth;
	schrect.m_nBorderColor = m_nBorderColor;
	schrect.m_nFillColor = m_nFillColor;
	schrect.m_bFillSolid = m_bFillSolid;
	schrect.m_bTransparent = FALSE;
	schrect.m_fX1 = m_fX;
	schrect.m_fY1 = m_fY - m_fYsize;
	schrect.m_fX2 = m_fX + m_fXsize;
	schrect.m_fY2 = m_fY;
	schrect.m_nFlag = m_nFlag;

	schrect.Draw(pDC, pV);
}

void COb_schsheetsymbol::DrawSelection(CDC* pDC, CView* pV)
{
	int x1, y1, x2, y2;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	// 创建画笔
	CPen arcpen;
	arcpen.CreatePen(PS_DOT, 1, nEDA_SelectionColor);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	
	pView->Get_VPort_XY(&x1,&y1,m_fX,m_fY);
	pView->Get_VPort_XY(&x2,&y2,m_fX+m_fXsize,m_fY-m_fYsize);

	pView->DrawSelectionKeyPoint(x1, y1, pDC);
	pView->DrawSelectionKeyPoint(x1, y2, pDC);
	pView->DrawSelectionKeyPoint(x2, y1, pDC);
	pView->DrawSelectionKeyPoint(x2, y2, pDC);
	pView->DrawSelectionKeyPoint((x1+x2)/2, y1, pDC);
	pView->DrawSelectionKeyPoint((x1+x2)/2, y2, pDC);
	pView->DrawSelectionKeyPoint(x1, (y1+y2)/2, pDC);
	pView->DrawSelectionKeyPoint(x2, (y1+y2)/2, pDC);

	Fpoint fp[4];
	fp[0].fx = m_fX;
	fp[0].fy = m_fY;
	fp[1].fx = m_fX;
	fp[1].fy = m_fY-m_fYsize;
	fp[2].fx = m_fX+m_fXsize;
	fp[2].fy = m_fY-m_fYsize;
	fp[3].fx = m_fX+m_fXsize;
	fp[3].fy = m_fY;

	CObject* pobject;
	for(int k=0; k<m_arrayPos.GetCount(); k++)
	{
		POSITION subpos = m_arrayPos.GetAt(k);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext = (COb_schtext*)pobject;
			if((ptext->m_nID == TEXT_SHEETSYMBOLDESIGNATOR)||(ptext->m_nID == TEXT_SHEETSYMBOLFILENAME)||(ptext->m_nID == TEXT_SHEETSYMBOLTEXT))
			{
				double dis = 10000000;
				int index;
				for(int m=0; m<4; m++)
				{
					double d = sqrt((ptext->m_fSx - fp[m].fx)*(ptext->m_fSx - fp[m].fx) + (ptext->m_fSy - fp[m].fy)*(ptext->m_fSy - fp[m].fy));
					if(d < dis)
					{
						dis = d;
						index = m;
					}
				}

				pView->Get_VPort_XY(&x1, &y1, fp[index].fx, fp[index].fy);
				pView->Get_VPort_XY(&x2, &y2, ptext->m_fSx, ptext->m_fSy);
				pDC->MoveTo(x1,y1);
				pDC->LineTo(x2,y2);
			}
		}
	}

	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}

BOOL  COb_schsheetsymbol::Is_On_Me(double fx, double fy)
{
	CRgn   rgn;

	rgn.CreateRectRgn( (int)m_fX, (int)m_fY, (int)(m_fX + m_fXsize), (int)(m_fY - m_fYsize));

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

BOOL  COb_schsheetsymbol::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fminx, fminy, fmaxx, fmaxy;
	fminx = m_fX;
	fminy = m_fY;
	fmaxx = m_fX + m_fXsize;
	fmaxy = m_fY - m_fYsize;
	
	if((fminx < fx1)||(fmaxx > fx2)||(fminy < fy1)||(fmaxy > fy2)) return FALSE;
	return TRUE;
}

void COb_schsheetsymbol::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_EditSheetSymbol dlg;

	dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fX);
	dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fY);
	dlg.m_csEditSizeX = pDoc->Convert_XY_To_String(m_fXsize);
	dlg.m_csEditSizeY = pDoc->Convert_XY_To_String(m_fYsize);
	dlg.m_nFillColor = m_nFillColor;
	dlg.m_nBorderColor = m_nBorderColor;
	dlg.m_bCheckDrawSolid = m_bFillSolid;
	dlg.m_nComboWidth = m_nBorderWidth;
	dlg.m_csEditDesignator = GetDesignator();
	dlg.m_csEditFilename = GetFilename();
	
	if(dlg.DoModal() == IDOK)
	{
		if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = pos;
		pundo->schsheetsymbol.Copy(this);
			
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

		m_fX = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
		m_fY = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
		m_fXsize = pDoc->Convert_String_To_XY(dlg.m_csEditSizeX);
		m_fYsize = pDoc->Convert_String_To_XY(dlg.m_csEditSizeY);
		m_nFillColor = dlg.m_nFillColor;
		m_nBorderColor = dlg.m_nBorderColor;
		m_bFillSolid = dlg.m_bCheckDrawSolid;
		m_nBorderWidth = dlg.m_nComboWidth;
		SetSheetSymbolDesignator(dlg.m_csEditDesignator);	
		SetSheetSymbolFilename(dlg.m_csEditFilename);		
		
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
}

CString COb_schsheetsymbol::GetDesignator(void)
{
	CObject* pobject;
//	COb_schsheetsymbol* psheetsymbol;
	CString csDesignator = "";
	CESDDoc* pESDDoc;

	pESDDoc = (CESDDoc*)pDocument;
	if(pESDDoc == NULL) return csDesignator;

	POSITION pos;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		pos = m_arrayPos.GetAt(i);
		pobject = pESDDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
		{
			COb_schsheetentry* psheetentry;
			psheetentry = (COb_schsheetentry*)pobject;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if(ptext->m_nID == TEXT_SHEETSYMBOLDESIGNATOR) return ptext->m_csText;
		}
	}

	return csDesignator;
}

CString COb_schsheetsymbol::GetFilename(void)
{
	CObject* pobject;
	//COb_schsheetsymbol* psheetsymbol;
	CString csFilename = "";
	CESDDoc* pESDDoc;

	pESDDoc = (CESDDoc*)pDocument;
	if(pESDDoc == NULL) return csFilename;

	POSITION pos;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		pos = m_arrayPos.GetAt(i);
		pobject = pESDDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
		{
			COb_schsheetentry* psheetentry;
			psheetentry = (COb_schsheetentry*)pobject;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if(ptext->m_nID == TEXT_SHEETSYMBOLFILENAME) return ptext->m_csText;
		}
	}

	return csFilename;
}

void COb_schsheetsymbol::SetSheetSymbolDesignator(CString& cstring)
{
	CObject* pobject;
	//COb_schsheetsymbol* psheetsymbol;
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	if(pDoc == NULL) return;

	POSITION pos;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		pos = m_arrayPos.GetAt(i);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
		{
			COb_schsheetentry* psheetentry;
			psheetentry = (COb_schsheetentry*)pobject;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if(ptext->m_nID == TEXT_SHEETSYMBOLDESIGNATOR)
			{
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schtext.Copy(ptext);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				ptext->m_csText = cstring;
			}
		}
	}
}

void COb_schsheetsymbol::SetSheetSymbolFilename(CString& cstring)
{
	CObject* pobject;
	//COb_schsheetsymbol* psheetsymbol;
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	if(pDoc == NULL) return;

	POSITION pos;
	for(int i=0; i<m_arrayPos.GetSize(); i++)
	{
		pos = m_arrayPos.GetAt(i);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
		{
			COb_schsheetentry* psheetentry;
			psheetentry = (COb_schsheetentry*)pobject;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if(ptext->m_nID == TEXT_SHEETSYMBOLFILENAME)
			{
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schtext.Copy(ptext);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				ptext->m_csText = cstring;
			}
		}
	}
}

void COb_schsheetsymbol::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	double x1,y1, x0, y0, deltax, deltay;
	POSITION subpos;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动分图符号: <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	pView->Invalidate();

	for(int i=0; i<m_arrayPos.GetCount(); i++)
	{
		CObject* pobject;
		subpos = m_arrayPos.GetAt(i);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(subpos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext = (COb_schtext*)pobject;
			if( ((ptext->m_nID == TEXT_SHEETSYMBOLDESIGNATOR)||(ptext->m_nID == TEXT_SHEETSYMBOLFILENAME))&&(ptext->m_bAutoPosition == FALSE) )
			{
				ptext->m_nFlag = FLAG_NORMAL;
				ptext->Draw(pDC, pV);
			}
		}
	}

	x0 = m_fX;
	y0 = m_fY;

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schsheetsymbol asheetsymbol;
	asheetsymbol.pDocument = pDoc;
	asheetsymbol.m_fX = m_fX;
	asheetsymbol.m_fY = m_fY;
	asheetsymbol.m_fXsize = m_fXsize;
	asheetsymbol.m_fYsize = m_fYsize;
	asheetsymbol.m_nBorderWidth = m_nBorderWidth;
	asheetsymbol.m_nFillColor = m_nFillColor;
	asheetsymbol.m_nBorderColor = m_nBorderColor;
	asheetsymbol.m_bShowHiddenTextFields = FALSE;
	asheetsymbol.m_bFillSolid = m_bFillSolid;
	asheetsymbol.m_nDiagram = m_nDiagram;

	asheetsymbol.Draw(pDC, pV);
	pView->OpenMouseCursor();

	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			asheetsymbol.Draw(pDC, pV);
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
				asheetsymbol.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			asheetsymbol.m_fX = m_fX + deltax;
			asheetsymbol.m_fY = m_fY + deltay;

			pView->CloseMouseCursor();
			asheetsymbol.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			asheetsymbol.Draw(pDC, pV);
			pView->OpenMouseCursor();
			
			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			asheetsymbol.m_fX = m_fX + deltax;
			asheetsymbol.m_fY = m_fY + deltay;

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->m_fX1 = m_fX;
			pundo->m_fY1 = m_fY;
			pundo->m_fX2 = m_fXsize;
			pundo->m_fY2 = m_fYsize;
	
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fX = asheetsymbol.m_fX;
			m_fY = asheetsymbol.m_fY;
			
			for(int i=0; i<m_arrayPos.GetCount(); i++)
			{
				CObject* pobject;
				subpos = m_arrayPos.GetAt(i);
				pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext = (COb_schtext*)pobject;
					if( ((ptext->m_nID == TEXT_SHEETSYMBOLDESIGNATOR)||(ptext->m_nID == TEXT_SHEETSYMBOLFILENAME))&&(ptext->m_bAutoPosition == TRUE) )
					{
						pundo = new CSchUndo;
						pundo->m_nOp = UNDO_MOVE;
						pundo->m_nOb = SCH_ELEMENT_TEXT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->m_fX1 = ptext->m_fSx;
						pundo->m_fY1 = ptext->m_fSy;
						pundo->m_nOrientation = ptext->m_nOrientation;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;

						ptext->m_fSx += deltax;
						ptext->m_fSy += deltay;
						ptext->Draw(pDC, pV);

					}
				}
			}

			break;
		}
		else if(ch1 == SPACE)
		{

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

void COb_schsheetsymbol::Copy(COb_schsheetsymbol* pschsheetsymbol)
{
    m_fX = pschsheetsymbol->m_fX;
	m_fY = pschsheetsymbol->m_fY;
	m_fXsize = pschsheetsymbol->m_fXsize;
	m_fYsize = pschsheetsymbol->m_fYsize;

	m_nBorderWidth = pschsheetsymbol->m_nBorderWidth;
	m_nFillColor = pschsheetsymbol->m_nFillColor;
	m_nBorderColor = pschsheetsymbol->m_nBorderColor;
	m_bFillSolid = pschsheetsymbol->m_bFillSolid;
	m_bShowHiddenTextFields = pschsheetsymbol->m_bShowHiddenTextFields;

	m_arrayPos.RemoveAll();
	for(int i=0; i<pschsheetsymbol->m_arrayPos.GetCount(); i++)
	{
		POSITION pos;
		pos = pschsheetsymbol->m_arrayPos.GetAt(i);
		m_arrayPos.Add(pos);
	}	
	
	//CArray <POSITION, POSITION&> m_arrayPos;
	//CString m_csDesignator;
	//CString m_csFilename;
	m_csUniqueID = pschsheetsymbol->m_csUniqueID;

	m_nDiagram = pschsheetsymbol->m_nDiagram;
	m_bSelection = pschsheetsymbol->m_bSelection;
	m_nFlag = pschsheetsymbol->m_nFlag;

	pDocument = pschsheetsymbol->pDocument;
}