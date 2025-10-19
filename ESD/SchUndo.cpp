#include "stdafx.h"
#include "struct.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

CSchUndo::CSchUndo()
{
	changeviewpoint = FALSE;
}

CSchUndo::~CSchUndo()
{
	// This empty constructor should be used by serialization only
}

void CSchUndo::Undo(void)
{
	CObject* pobject;

	CESDApp* pApp = (CESDApp*)AfxGetApp();
	CESDView* pView = (CESDView*)pApp->GetCurrentView();
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(	m_nOp == UNDO_DELETE)  
	{
		if(m_nOb == SCH_ELEMENT_COMPONENT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;
				pcomp->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pcomp->m_fX, pcomp->m_fY, pcomp->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}		
		else if(m_nOb == SCH_ELEMENT_TEXT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				ptext->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, ptext->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_BEZIER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier)))
			{
				COb_schbezier* pbezier = (COb_schbezier*)pobject;
				pbezier->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pbezier->m_cVertex[0].fx, pbezier->m_cVertex[0].fy, pbezier->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYLINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
			{
				COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
				ppolyline->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolyline->m_cVertex[0].fx, ppolyline->m_cVertex[0].fy, ppolyline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYGON)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
			{
				COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
				ppolygon->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolygon->m_cVertex[0].fx, ppolygon->m_cVertex[0].fy, ppolygon->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSEFILL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
			{
				COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
				pellipsefill->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipsefill->m_fCx, pellipsefill->m_fCy, pellipsefill->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PIE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
			{
				COb_schpie* ppie = (COb_schpie*)pobject;
				ppie->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppie->m_fCx, ppie->m_fCy, ppie->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ROUNDRECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect)))
			{
				COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
				proundrect->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(proundrect->m_fX1, proundrect->m_fY1, proundrect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
			{
				COb_schellipse* pellipse = (COb_schellipse*)pobject;
				pellipse->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipse->m_fCx, pellipse->m_fCy, pellipse->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ARC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
			{
				COb_scharc* parc = (COb_scharc*)pobject;
				parc->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(parc->m_fCx, parc->m_fCy, parc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_LINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				pline->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pline->m_cVertex[0].fx, pline->m_cVertex[0].fy, pline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_RECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect)))
			{
				COb_schrect* prect = (COb_schrect*)pobject;
				prect->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(prect->m_fX1, prect->m_fY1, prect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETSYMBOL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol)))
			{
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
				psheetsymbol->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetsymbol->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETENTRY)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry)))
			{
				COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
				psheetentry->m_nFlag = FLAG_DELETED;
				
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pDoc->m_listSchObject[m_nDiagram].GetAt(psheetentry->m_posParent);
				
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetentry->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POWER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower)))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				ppower->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppower->m_fX, ppower->m_fY, ppower->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PORT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport)))
			{
				COb_schport* pport = (COb_schport*)pobject;
				pport->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pport->m_fX, pport->m_fY, pport->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_TEXTFRAME)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe)))
			{
				COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
				ptextframe->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptextframe->m_fX1, ptextframe->m_fY1, ptextframe->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_JUNCTION)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc = (COb_schjunc*)pobject;
				pjunc->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pjunc->m_fCx, pjunc->m_fCy, pjunc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_GRAPHIC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic)))
			{
				COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
				pgraphic->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pgraphic->m_fX1, pgraphic->m_fY1, pgraphic->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
	}
	else if(m_nOp == UNDO_UNDELETE)  
	{
		if(m_nOb == SCH_ELEMENT_COMPONENT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;
				pcomp->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pcomp->m_fX, pcomp->m_fY, pcomp->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_TEXT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				ptext->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, ptext->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_BEZIER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier)))
			{
				COb_schbezier* pbezier = (COb_schbezier*)pobject;
				pbezier->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pbezier->m_cVertex[0].fx, pbezier->m_cVertex[0].fy, pbezier->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYLINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
			{
				COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
				ppolyline->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolyline->m_cVertex[0].fx, ppolyline->m_cVertex[0].fy, ppolyline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYGON)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
			{
				COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
				ppolygon->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolygon->m_cVertex[0].fx, ppolygon->m_cVertex[0].fy, ppolygon->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSEFILL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
			{
				COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
				pellipsefill->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipsefill->m_fCx, pellipsefill->m_fCy, pellipsefill->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PIE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
			{
				COb_schpie* ppie = (COb_schpie*)pobject;
				ppie->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppie->m_fCx, ppie->m_fCy, ppie->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ROUNDRECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect)))
			{
				COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
				proundrect->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(proundrect->m_fX1, proundrect->m_fY1, proundrect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
			{
				COb_schellipse* pellipse = (COb_schellipse*)pobject;
				pellipse->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipse->m_fCx, pellipse->m_fCy, pellipse->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ARC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
			{
				COb_scharc* parc = (COb_scharc*)pobject;
				parc->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(parc->m_fCx, parc->m_fCy, parc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_LINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				pline->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pline->m_cVertex[0].fx, pline->m_cVertex[0].fy, pline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_RECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect)))
			{
				COb_schrect* prect = (COb_schrect*)pobject;
				prect->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(prect->m_fX1, prect->m_fY1, prect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETSYMBOL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol)))
			{
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
				psheetsymbol->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetsymbol->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETENTRY)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry)))
			{
				COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
				psheetentry->m_nFlag = FLAG_NORMAL;
				
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pDoc->m_listSchObject[m_nDiagram].GetAt(psheetentry->m_posParent);
				
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetentry->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POWER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower)))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				ppower->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppower->m_fX, ppower->m_fY, ppower->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PORT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport)))
			{
				COb_schport* pport = (COb_schport*)pobject;
				pport->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pport->m_fX, pport->m_fY, pport->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_TEXTFRAME)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe)))
			{
				COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
				ptextframe->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptextframe->m_fX1, ptextframe->m_fY1, ptextframe->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_JUNCTION)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc = (COb_schjunc*)pobject;
				pjunc->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pjunc->m_fCx, pjunc->m_fCy, pjunc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_GRAPHIC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic)))
			{
				COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
				pgraphic->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pgraphic->m_fX1, pgraphic->m_fY1, pgraphic->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
	}
	else if(m_nOp == UNDO_MOVE)  
	{
		if(m_nOb == SCH_ELEMENT_COMPONENT)
		{
		}
		else if(m_nOb == SCH_ELEMENT_PIN)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
			{
				COb_schpin* ppin = (COb_schpin*)pobject;
				
				double ftemp;
				int itemp;
				ftemp = ppin->m_fX;
				ppin->m_fX = m_fX1;
				m_fX1 = ftemp;
				ftemp = ppin->m_fY;
				ppin->m_fY = m_fY1;
				m_fY1 = ftemp;
				itemp = ppin->m_nOrientation;
				ppin->m_nOrientation = m_nOrientation;
				m_nOrientation = itemp;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppin->m_fX, ppin->m_fY, ppin->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_TEXT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;

				double ftemp;
				int itemp;
				ftemp = ptext->m_fSx;
				ptext->m_fSx = m_fX1;
				m_fX1 = ftemp;
				ftemp = ptext->m_fSy;
				ptext->m_fSy = m_fY1;
				m_fY1 = ftemp;
				itemp = ptext->m_nOrientation;
				ptext->m_nOrientation = m_nOrientation;
				m_nOrientation = itemp;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, ptext->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_BEZIER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
			{
				COb_schbezier* pbezier = (COb_schbezier*)pobject;
				
				Fpoint fpoint;
				for(int i=0; i<pbezier->m_cVertex.GetCount(); i++)
				{
					fpoint.fx = pbezier->m_cVertex[i].fx;
					pbezier->m_cVertex[i].fx = m_cVertex[i].fx;
					m_cVertex[i].fx = fpoint.fx;
					fpoint.fy = pbezier->m_cVertex[i].fy;
					pbezier->m_cVertex[i].fy = m_cVertex[i].fy;
					m_cVertex[i].fy = fpoint.fy;
				}
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pbezier->m_cVertex[0].fx, pbezier->m_cVertex[0].fy, pbezier->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYLINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
			{
				COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
				
				Fpoint fpoint;
				for(int i=0; i<ppolyline->m_cVertex.GetCount(); i++)
				{
					fpoint.fx = ppolyline->m_cVertex[i].fx;
					ppolyline->m_cVertex[i].fx = m_cVertex[i].fx;
					m_cVertex[i].fx = fpoint.fx;
					fpoint.fy = ppolyline->m_cVertex[i].fy;
					ppolyline->m_cVertex[i].fy = m_cVertex[i].fy;
					m_cVertex[i].fy = fpoint.fy;
				}
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolyline->m_cVertex[0].fx, ppolyline->m_cVertex[0].fy, ppolyline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYGON)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
			{
				COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
				
				Fpoint fpoint;
				for(int i=0; i<ppolygon->m_cVertex.GetCount(); i++)
				{
					fpoint.fx = ppolygon->m_cVertex[i].fx;
					ppolygon->m_cVertex[i].fx = m_cVertex[i].fx;
					m_cVertex[i].fx = fpoint.fx;
					fpoint.fy = ppolygon->m_cVertex[i].fy;
					ppolygon->m_cVertex[i].fy = m_cVertex[i].fy;
					m_cVertex[i].fy = fpoint.fy;
				}
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolygon->m_cVertex[0].fx, ppolygon->m_cVertex[0].fy, ppolygon->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSEFILL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
			{
				COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
				
				double ftemp;
				ftemp = pellipsefill->m_fCx;
				pellipsefill->m_fCx = m_fX1;
				m_fX1 = ftemp;
				ftemp = pellipsefill->m_fCy;
				pellipsefill->m_fCy = m_fY1;
				m_fY1 = ftemp;
				ftemp = pellipsefill->m_fRadius_x;
				pellipsefill->m_fRadius_x = m_fRx;
				m_fRx = ftemp;
				ftemp = pellipsefill->m_fRadius_y;
				pellipsefill->m_fRadius_y = m_fRy;
				m_fRy = ftemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipsefill->m_fCx, pellipsefill->m_fCy, pellipsefill->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PIE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
			{
				COb_schpie* ppie = (COb_schpie*)pobject;
				
				double ftemp;
				ftemp = ppie->m_fCx;
				ppie->m_fCx = m_fX1;
				m_fX1 = ftemp;
				ftemp = ppie->m_fCy;
				ppie->m_fCy = m_fY1;
				m_fY1 = ftemp;
				ftemp = ppie->m_fSangle;
				ppie->m_fSangle = m_fX2;
				m_fX2 = ftemp;
				ftemp = ppie->m_fEangle;
				ppie->m_fEangle = m_fY2;
				m_fY2 = ftemp;
				ftemp = ppie->m_fRadius;
				ppie->m_fRadius = m_fRx;
				m_fRx = ftemp;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppie->m_fCx, ppie->m_fCy, ppie->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ROUNDRECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
			{
				COb_schroundrect* proundrect = (COb_schroundrect*)pobject;

				double ftemp;
				ftemp = proundrect->m_fX1;
				proundrect->m_fX1 = m_fX1;
				m_fX1 = ftemp;
				
				ftemp = proundrect->m_fY1;
				proundrect->m_fY1 = m_fY1;
				m_fY1 = ftemp;
				
				ftemp = proundrect->m_fX2;
				proundrect->m_fX2 = m_fX2;
				m_fX2 = ftemp;
				
				ftemp = proundrect->m_fY2;
				proundrect->m_fY2 = m_fY2;
				m_fY2 = ftemp;
				
				ftemp = proundrect->m_fRadius_x;
				proundrect->m_fRadius_x = m_fRx;
				m_fRx = ftemp;
				
				ftemp = proundrect->m_fRadius_y;
				proundrect->m_fRadius_y = m_fRy;
				m_fRy = ftemp;	
				
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(proundrect->m_fX1, proundrect->m_fY1, proundrect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
			{
				COb_schellipse* pellipse = (COb_schellipse*)pobject;
				
				double ftemp;
				ftemp = pellipse->m_fCx;
				pellipse->m_fCx = m_fX1;
				m_fX1 = ftemp;
				ftemp = pellipse->m_fCy;
				pellipse->m_fCy = m_fY1;
				m_fY1 = ftemp;
				ftemp = pellipse->m_fSangle;
				pellipse->m_fSangle = m_fX2;
				m_fX2 = ftemp;
				ftemp = pellipse->m_fEangle;
				pellipse->m_fEangle = m_fY2;
				m_fY2 = ftemp;
				ftemp = pellipse->m_fRadius_x;
				pellipse->m_fRadius_x = m_fRx;
				m_fRx = ftemp;
				ftemp = pellipse->m_fRadius_y;
				pellipse->m_fRadius_y = m_fRy;
				m_fRy = ftemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipse->m_fCx, pellipse->m_fCy, pellipse->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ARC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
			{
				COb_scharc* parc = (COb_scharc*)pobject;
				
				double ftemp;
				ftemp = parc->m_fCx;
				parc->m_fCx = m_fX1;
				m_fX1 = ftemp;
				ftemp = parc->m_fCy;
				parc->m_fCy = m_fY1;
				m_fY1 = ftemp;
				ftemp = parc->m_fSangle;
				parc->m_fSangle = m_fX2;
				m_fX2 = ftemp;
				ftemp = parc->m_fEangle;
				parc->m_fEangle = m_fY2;
				m_fY2 = ftemp;
				ftemp = parc->m_fRadius;
				parc->m_fRadius = m_fRx;
				m_fRx = ftemp;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(parc->m_fCx, parc->m_fCy, parc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_LINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				
				Fpoint fpoint;
				for(int i=0; i<pline->m_cVertex.GetCount(); i++)
				{
					fpoint.fx = pline->m_cVertex[i].fx;
					pline->m_cVertex[i].fx = m_cVertex[i].fx;
					m_cVertex[i].fx = fpoint.fx;
					fpoint.fy = pline->m_cVertex[i].fy;
					pline->m_cVertex[i].fy = m_cVertex[i].fy;
					m_cVertex[i].fy = fpoint.fy;
				}
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pline->m_cVertex[0].fx, pline->m_cVertex[0].fy, pline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_RECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
			{
				COb_schrect* prect = (COb_schrect*)pobject;
				
				double ftemp;
				ftemp = prect->m_fX1;
				prect->m_fX1 = m_fX1;
				m_fX1 = ftemp;
				ftemp = prect->m_fY1;
				prect->m_fY1 = m_fY1;
				m_fY1 = ftemp;
				ftemp = prect->m_fX2;
				prect->m_fX2 = m_fX2;
				m_fX2 = ftemp;
				ftemp = prect->m_fY2;
				prect->m_fY2 = m_fY2;
				m_fY2 = ftemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(prect->m_fX1, prect->m_fY1, prect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETSYMBOL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;

				double ftemp;
				ftemp = psheetsymbol->m_fX;
				psheetsymbol->m_fX = m_fX1;
				m_fX1 = ftemp;
				ftemp = psheetsymbol->m_fY;
				psheetsymbol->m_fY = m_fY1;
				m_fY1 = ftemp;
				ftemp = psheetsymbol->m_fXsize;
				psheetsymbol->m_fXsize = m_fX2;
				m_fX2 = ftemp;
				ftemp = psheetsymbol->m_fYsize;
				psheetsymbol->m_fYsize = m_fY2;
				m_fY2 = ftemp;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetsymbol->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETENTRY)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
			{
				COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;

				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pDoc->m_listSchObject[m_nDiagram].GetAt(psheetentry->m_posParent);
				
				double ftemp;
				int itemp;
				ftemp = psheetentry->m_fOffset;
				psheetentry->m_fOffset = m_fX1;
				m_fX1 = ftemp;
				itemp = psheetentry->m_nSide;
				psheetentry->m_nSide = m_nOrientation;
				m_nOrientation = itemp;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetentry->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POWER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				
				double ftemp;
				int itemp;
				ftemp = ppower->m_fX;
				ppower->m_fX = m_fX1;
				m_fX1 = ftemp;
				ftemp = ppower->m_fY;
				ppower->m_fY = m_fY1;
				m_fY1 = ftemp;
				itemp = ppower->m_nOrientation;
				ppower->m_nOrientation = m_nOrientation;
				m_nOrientation = itemp;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppower->m_fX, ppower->m_fY, ppower->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PORT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport = (COb_schport*)pobject;
				
				double ftemp;
				int itemp;
				ftemp = pport->m_fX;
				pport->m_fX = m_fX1;
				m_fX1 = ftemp;
				ftemp = pport->m_fY;
				pport->m_fY = m_fY1;
				m_fY1 = ftemp;
				ftemp = pport->m_fSize;
				pport->m_fSize = m_fX2;
				m_fX2 = ftemp;
				itemp = pport->m_nStyle;
				pport->m_nStyle = m_nOrientation;
				m_nOrientation = itemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pport->m_fX, pport->m_fY, pport->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_TEXTFRAME)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
			{
				COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
				
				double ftemp;
				ftemp = ptextframe->m_fX1;
				ptextframe->m_fX1 = m_fX1;
				m_fX1 = ftemp;
				ftemp = ptextframe->m_fY1;
				ptextframe->m_fY1 = m_fY1;
				m_fY1 = ftemp;
				ftemp = ptextframe->m_fX2;
				ptextframe->m_fX2 = m_fX2;
				m_fX2 = ftemp;
				ftemp = ptextframe->m_fY2;
				ptextframe->m_fY2 = m_fY2;
				m_fY2 = ftemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptextframe->m_fX1, ptextframe->m_fY1, ptextframe->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_JUNCTION)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc = (COb_schjunc*)pobject;
				
				double ftemp;
				ftemp = pjunc->m_fCx;
				pjunc->m_fCx = m_fX1;
				m_fX1 = ftemp;
				ftemp = pjunc->m_fCy;
				pjunc->m_fCy = m_fY1;
				m_fY1 = ftemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pjunc->m_fCx, pjunc->m_fCy, pjunc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_GRAPHIC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
			{
				COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
				
				double ftemp;
				ftemp = pgraphic->m_fX1;
				pgraphic->m_fX1 = m_fX1;
				m_fX1 = ftemp;
				ftemp = pgraphic->m_fY1;
				pgraphic->m_fY1 = m_fY1;
				m_fY1 = ftemp;
				ftemp = pgraphic->m_fX2;
				pgraphic->m_fX2 = m_fX2;
				m_fX2 = ftemp;
				ftemp = pgraphic->m_fY2;
				pgraphic->m_fY2 = m_fY2;
				m_fY2 = ftemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pgraphic->m_fX1, pgraphic->m_fY1, pgraphic->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
	}
	else if(m_nOp == UNDO_CHANGE)
	{
		if(m_nOb == SCH_ELEMENT_COMPONENT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;
				COb_schcomp tempcomp;
				tempcomp.Copy(pcomp);
				pcomp->Copy(&schcomp);
				schcomp.Copy(&tempcomp);

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pcomp->m_fX, pcomp->m_fY, pcomp->m_nDiagram);
					changeviewpoint = FALSE;
				}				
				
			}
		}
		else if(m_nOb == SCH_ELEMENT_PIN)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
			{
				COb_schpin* ppin = (COb_schpin*)pobject;
				COb_schpin temppin;
				temppin.Copy(ppin);
				ppin->Copy(&schpin);
				schpin.Copy(&temppin);

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppin->m_fX, ppin->m_fY, ppin->m_nDiagram);
					changeviewpoint = FALSE;
				}				
				
			}
		}
		else if(m_nOb == SCH_ELEMENT_IEEE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
			{
				COb_schieee* pieee = (COb_schieee*)pobject;
				COb_schieee tempieee;
				tempieee.Copy(pieee);
				pieee->Copy(&schieee);
				schieee.Copy(&tempieee);

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pieee->m_fX, pieee->m_fY, pieee->m_nDiagram);
					changeviewpoint = FALSE;
				}				
				
			}
		}
		else if(m_nOb == SCH_ELEMENT_ARC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
			{
				COb_scharc* parc = (COb_scharc*)pobject;
				COb_scharc temparc;
				temparc.Copy(parc);
				parc->Copy(&scharc);
				scharc.Copy(&temparc);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(parc->m_fCx, parc->m_fCy, parc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_BEZIER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
			{
				COb_schbezier* pbezier = (COb_schbezier*)pobject;
				COb_schbezier tempbezier;
				tempbezier.Copy(pbezier);
				pbezier->Copy(&schbezier);
				schbezier.Copy(&tempbezier);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pbezier->m_cVertex[0].fx, pbezier->m_cVertex[0].fy, pbezier->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
			{
				COb_schellipse* pellipse = (COb_schellipse*)pobject;
				COb_schellipse tempellipse;
				tempellipse.Copy(pellipse);
				pellipse->Copy(&schellipse);
				schellipse.Copy(&tempellipse);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipse->m_fCx, pellipse->m_fCy, pellipse->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSEFILL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
			{
				COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
				COb_schellipsefill tempellipsefill;
				tempellipsefill.Copy(pellipsefill);
				pellipsefill->Copy(&schellipsefill);
				schellipsefill.Copy(&tempellipsefill);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipsefill->m_fCx, pellipsefill->m_fCy, pellipsefill->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_GRAPHIC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
			{
				COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
				COb_schgraphic tempgraphic;
				tempgraphic.Copy(pgraphic);
				pgraphic->Copy(&schgraphic);
				schgraphic.Copy(&tempgraphic);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pgraphic->m_fX1, pgraphic->m_fY1, pgraphic->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_JUNCTION)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc = (COb_schjunc*)pobject;
				COb_schjunc tempjunc;
				tempjunc.Copy(pjunc);
				pjunc->Copy(&schjunc);
				schjunc.Copy(&tempjunc);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pjunc->m_fCx, pjunc->m_fCy, pjunc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_LINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				COb_schline templine;
				templine.Copy(pline);
				pline->Copy(&schline);
				schline.Copy(&templine);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pline->m_cVertex[0].fx, pline->m_cVertex[0].fy, pline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PIE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
			{
				COb_schpie* ppie = (COb_schpie*)pobject;
				COb_schpie temppie;
				temppie.Copy(ppie);
				ppie->Copy(&schpie);
				schpie.Copy(&temppie);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppie->m_fCx, ppie->m_fCy, ppie->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYGON)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
			{
				COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
				COb_schpolygon temppolygon;
				temppolygon.Copy(ppolygon);
				ppolygon->Copy(&schpolygon);
				schpolygon.Copy(&temppolygon);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolygon->m_cVertex[0].fx, ppolygon->m_cVertex[0].fy, ppolygon->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYLINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
			{
				COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
				COb_schpolyline temppolyline;
				temppolyline.Copy(ppolyline);
				ppolyline->Copy(&schpolyline);
				schpolyline.Copy(&temppolyline);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolyline->m_cVertex[0].fx, ppolyline->m_cVertex[0].fy, ppolyline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PORT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport = (COb_schport*)pobject;
				COb_schport tempport;
				tempport.Copy(pport);
				pport->Copy(&schport);
				schport.Copy(&tempport);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pport->m_fX, pport->m_fY, pport->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POWER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				COb_schpower temppower;
				temppower.Copy(ppower);
				ppower->Copy(&schpower);
				schpower.Copy(&temppower);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppower->m_fX, ppower->m_fY, ppower->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_RECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
			{
				COb_schrect* prect = (COb_schrect*)pobject;
				COb_schrect temprect;
				temprect.Copy(prect);
				prect->Copy(&schrect);
				schrect.Copy(&temprect);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(prect->m_fX1, prect->m_fY1, prect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ROUNDRECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
			{
				COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
				COb_schroundrect temproundrect;
				temproundrect.Copy(proundrect);
				proundrect->Copy(&schroundrect);
				schroundrect.Copy(&temproundrect);

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(proundrect->m_fX1, proundrect->m_fY1, proundrect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETENTRY)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
			{
				COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
				COb_schsheetentry tempsheetentry;
				tempsheetentry.Copy(psheetentry);
				psheetentry->Copy(&schsheetentry);
				schsheetentry.Copy(&tempsheetentry);

				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pDoc->m_listSchObject[m_nDiagram].GetAt(psheetentry->m_posParent);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetentry->m_nDiagram);
					changeviewpoint = FALSE;
				}				
				
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETSYMBOL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
				COb_schsheetsymbol tempsheetsymbol;
				tempsheetsymbol.Copy(psheetsymbol);
				psheetsymbol->Copy(&schsheetsymbol);
				schsheetsymbol.Copy(&tempsheetsymbol);

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetsymbol->m_nDiagram);
					changeviewpoint = FALSE;
				}				
				
			}
		}
		else if(m_nOb == SCH_ELEMENT_TEXT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				COb_schtext temptext;
				temptext.Copy(ptext);
				ptext->Copy(&schtext);
				schtext.Copy(&temptext);

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, ptext->m_nDiagram);
					changeviewpoint = FALSE;
				}				
				
			}
		}
		else if(m_nOb == SCH_ELEMENT_TEXTFRAME)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
			{
				COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
				COb_schtextframe temptextframe;
				temptextframe.Copy(ptextframe);
				ptextframe->Copy(&schtextframe);
				schtextframe.Copy(&temptextframe);

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptextframe->m_fX1, ptextframe->m_fY1, ptextframe->m_nDiagram);
					changeviewpoint = FALSE;
				}				
				
			}
		}
	}

	pView->Invalidate();
}


void CSchUndo::Redo(void)
{
	CObject* pobject;

	CESDApp* pApp = (CESDApp*)AfxGetApp();
	CESDView* pView = (CESDView*)pApp->GetCurrentView();
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	//CClientDC dc(pView);

	if(	m_nOp == UNDO_DELETE)  
	{
		if(m_nOb == SCH_ELEMENT_COMPONENT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;
				pcomp->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pcomp->m_fX, pcomp->m_fY, pcomp->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_TEXT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				ptext->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, ptext->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_BEZIER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier)))
			{
				COb_schbezier* pbezier = (COb_schbezier*)pobject;
				pbezier->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pbezier->m_cVertex[0].fx, pbezier->m_cVertex[0].fy, pbezier->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYLINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
			{
				COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
				ppolyline->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolyline->m_cVertex[0].fx, ppolyline->m_cVertex[0].fy, ppolyline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYGON)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
			{
				COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
				ppolygon->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolygon->m_cVertex[0].fx, ppolygon->m_cVertex[0].fy, ppolygon->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSEFILL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
			{
				COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
				pellipsefill->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipsefill->m_fCx, pellipsefill->m_fCy, pellipsefill->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PIE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
			{
				COb_schpie* ppie = (COb_schpie*)pobject;
				ppie->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppie->m_fCx, ppie->m_fCy, ppie->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ROUNDRECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect)))
			{
				COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
				proundrect->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(proundrect->m_fX1, proundrect->m_fY1, proundrect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
			{
				COb_schellipse* pellipse = (COb_schellipse*)pobject;
				pellipse->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipse->m_fCx, pellipse->m_fCy, pellipse->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ARC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
			{
				COb_scharc* parc = (COb_scharc*)pobject;
				parc->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(parc->m_fCx, parc->m_fCy, parc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_LINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				pline->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pline->m_cVertex[0].fx, pline->m_cVertex[0].fy, pline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_RECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect)))
			{
				COb_schrect* prect = (COb_schrect*)pobject;
				prect->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(prect->m_fX1, prect->m_fY1, prect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETSYMBOL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol)))
			{
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
				psheetsymbol->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetsymbol->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETENTRY)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry)))
			{
				COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
				psheetentry->m_nFlag = FLAG_NORMAL;
				
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pDoc->m_listSchObject[m_nDiagram].GetAt(psheetentry->m_posParent);
				
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetentry->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POWER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower)))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				ppower->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppower->m_fX, ppower->m_fY, ppower->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PORT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport)))
			{
				COb_schport* pport = (COb_schport*)pobject;
				pport->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pport->m_fX, pport->m_fY, pport->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_TEXTFRAME)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe)))
			{
				COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
				ptextframe->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptextframe->m_fX1, ptextframe->m_fY1, ptextframe->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_JUNCTION)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc = (COb_schjunc*)pobject;
				pjunc->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pjunc->m_fCx, pjunc->m_fCy, pjunc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_GRAPHIC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic)))
			{
				COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
				pgraphic->m_nFlag = FLAG_NORMAL;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pgraphic->m_fX1, pgraphic->m_fY1, pgraphic->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
	}
	else if(m_nOp == UNDO_UNDELETE)  
	{
		if(m_nOb == SCH_ELEMENT_COMPONENT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;
				pcomp->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pcomp->m_fX, pcomp->m_fY, pcomp->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}		
		else if(m_nOb == SCH_ELEMENT_TEXT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				ptext->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, ptext->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_BEZIER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier)))
			{
				COb_schbezier* pbezier = (COb_schbezier*)pobject;
				pbezier->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pbezier->m_cVertex[0].fx, pbezier->m_cVertex[0].fy, pbezier->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYLINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
			{
				COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
				ppolyline->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolyline->m_cVertex[0].fx, ppolyline->m_cVertex[0].fy, ppolyline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYGON)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
			{
				COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
				ppolygon->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolygon->m_cVertex[0].fx, ppolygon->m_cVertex[0].fy, ppolygon->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSEFILL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
			{
				COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
				pellipsefill->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipsefill->m_fCx, pellipsefill->m_fCy, pellipsefill->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PIE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
			{
				COb_schpie* ppie = (COb_schpie*)pobject;
				ppie->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppie->m_fCx, ppie->m_fCy, ppie->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ROUNDRECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect)))
			{
				COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
				proundrect->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(proundrect->m_fX1, proundrect->m_fY1, proundrect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
			{
				COb_schellipse* pellipse = (COb_schellipse*)pobject;
				pellipse->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipse->m_fCx, pellipse->m_fCy, pellipse->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ARC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
			{
				COb_scharc* parc = (COb_scharc*)pobject;
				parc->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(parc->m_fCx, parc->m_fCy, parc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_LINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				pline->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pline->m_cVertex[0].fx, pline->m_cVertex[0].fy, pline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_RECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect)))
			{
				COb_schrect* prect = (COb_schrect*)pobject;
				prect->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(prect->m_fX1, prect->m_fY1, prect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETSYMBOL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol)))
			{
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
				psheetsymbol->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetsymbol->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETENTRY)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry)))
			{
				COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
				psheetentry->m_nFlag = FLAG_DELETED;
				
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pDoc->m_listSchObject[m_nDiagram].GetAt(psheetentry->m_posParent);
				
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetentry->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POWER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower)))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				ppower->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppower->m_fX, ppower->m_fY, ppower->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PORT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport)))
			{
				COb_schport* pport = (COb_schport*)pobject;
				pport->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pport->m_fX, pport->m_fY, pport->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_TEXTFRAME)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe)))
			{
				COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
				ptextframe->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptextframe->m_fX1, ptextframe->m_fY1, ptextframe->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_JUNCTION)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc = (COb_schjunc*)pobject;
				pjunc->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pjunc->m_fCx, pjunc->m_fCy, pjunc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_GRAPHIC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic)))
			{
				COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
				pgraphic->m_nFlag = FLAG_DELETED;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pgraphic->m_fX1, pgraphic->m_fY1, pgraphic->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
	}
	else if(m_nOp == UNDO_MOVE)  
	{
		if(m_nOb == SCH_ELEMENT_COMPONENT)
		{
		}
		else if(m_nOb == SCH_ELEMENT_PIN)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
			{
				COb_schpin* ppin = (COb_schpin*)pobject;
				
				double ftemp;
				int itemp;
				ftemp = ppin->m_fX;
				ppin->m_fX = m_fX1;
				m_fX1 = ftemp;
				ftemp = ppin->m_fY;
				ppin->m_fY = m_fY1;
				m_fY1 = ftemp;
				itemp = ppin->m_nOrientation;
				ppin->m_nOrientation = m_nOrientation;
				m_nOrientation = itemp;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppin->m_fX, ppin->m_fY, ppin->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_LINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				
				Fpoint fpoint;
				for(int i=0; i<pline->m_cVertex.GetCount(); i++)
				{
					fpoint.fx = pline->m_cVertex[i].fx;
					pline->m_cVertex[i].fx = m_cVertex[i].fx;
					m_cVertex[i].fx = fpoint.fx;
					fpoint.fy = pline->m_cVertex[i].fy;
					pline->m_cVertex[i].fy = m_cVertex[i].fy;
					m_cVertex[i].fy = fpoint.fy;
				}
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pline->m_cVertex[0].fx, pline->m_cVertex[0].fy, pline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYLINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
			{
				COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
				
				Fpoint fpoint;
				for(int i=0; i<ppolyline->m_cVertex.GetCount(); i++)
				{
					fpoint.fx = ppolyline->m_cVertex[i].fx;
					ppolyline->m_cVertex[i].fx = m_cVertex[i].fx;
					m_cVertex[i].fx = fpoint.fx;
					fpoint.fy = ppolyline->m_cVertex[i].fy;
					ppolyline->m_cVertex[i].fy = m_cVertex[i].fy;
					m_cVertex[i].fy = fpoint.fy;
				}
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolyline->m_cVertex[0].fx, ppolyline->m_cVertex[0].fy, ppolyline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_BEZIER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
			{
				COb_schbezier* pbezier = (COb_schbezier*)pobject;
				
				Fpoint fpoint;
				for(int i=0; i<pbezier->m_cVertex.GetCount(); i++)
				{
					fpoint.fx = pbezier->m_cVertex[i].fx;
					pbezier->m_cVertex[i].fx = m_cVertex[i].fx;
					m_cVertex[i].fx = fpoint.fx;
					fpoint.fy = pbezier->m_cVertex[i].fy;
					pbezier->m_cVertex[i].fy = m_cVertex[i].fy;
					m_cVertex[i].fy = fpoint.fy;
				}
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pbezier->m_cVertex[0].fx, pbezier->m_cVertex[0].fy, pbezier->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYGON)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
			{
				COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
				
				Fpoint fpoint;
				for(int i=0; i<ppolygon->m_cVertex.GetCount(); i++)
				{
					fpoint.fx = ppolygon->m_cVertex[i].fx;
					ppolygon->m_cVertex[i].fx = m_cVertex[i].fx;
					m_cVertex[i].fx = fpoint.fx;
					fpoint.fy = ppolygon->m_cVertex[i].fy;
					ppolygon->m_cVertex[i].fy = m_cVertex[i].fy;
					m_cVertex[i].fy = fpoint.fy;
				}
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolygon->m_cVertex[0].fx, ppolygon->m_cVertex[0].fy, ppolygon->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_TEXT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;

				double ftemp;
				int itemp;
				ftemp = ptext->m_fSx;
				ptext->m_fSx = m_fX1;
				m_fX1 = ftemp;
				ftemp = ptext->m_fSy;
				ptext->m_fSy = m_fY1;
				m_fY1 = ftemp;
				itemp = ptext->m_nOrientation;
				ptext->m_nOrientation = m_nOrientation;
				m_nOrientation = itemp;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, ptext->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ARC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
			{
				COb_scharc* parc = (COb_scharc*)pobject;
				
				double ftemp;
				ftemp = parc->m_fCx;
				parc->m_fCx = m_fX1;
				m_fX1 = ftemp;
				ftemp = parc->m_fCy;
				parc->m_fCy = m_fY1;
				m_fY1 = ftemp;
				ftemp = parc->m_fSangle;
				parc->m_fSangle = m_fX2;
				m_fX2 = ftemp;
				ftemp = parc->m_fEangle;
				parc->m_fEangle = m_fY2;
				m_fY2 = ftemp;
				ftemp = parc->m_fRadius;
				parc->m_fRadius = m_fRx;
				m_fRx = ftemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(parc->m_fCx, parc->m_fCy, parc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_JUNCTION)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc = (COb_schjunc*)pobject;

				double ftemp;
				ftemp = pjunc->m_fCx;
				pjunc->m_fCx = m_fX1;
				m_fX1 = ftemp;
				ftemp = pjunc->m_fCy;
				pjunc->m_fCy = m_fY1;
				m_fY1 = ftemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pjunc->m_fCx, pjunc->m_fCy, pjunc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PIE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
			{
				COb_schpie* ppie = (COb_schpie*)pobject;
				
				double ftemp;
				ftemp = ppie->m_fCx;
				ppie->m_fCx = m_fX1;
				m_fX1 = ftemp;
				ftemp = ppie->m_fCy;
				ppie->m_fCy = m_fY1;
				m_fY1 = ftemp;
				ftemp = ppie->m_fSangle;
				ppie->m_fSangle = m_fX2;
				m_fX2 = ftemp;
				ftemp = ppie->m_fEangle;
				ppie->m_fEangle = m_fY2;
				m_fY2 = ftemp;
				ftemp = ppie->m_fRadius;
				ppie->m_fRadius = m_fRx;
				m_fRx = ftemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppie->m_fCx, ppie->m_fCy, ppie->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_RECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
			{
				COb_schrect* prect = (COb_schrect*)pobject;

				double ftemp;
				ftemp = prect->m_fX1;
				prect->m_fX1 = m_fX1;
				m_fX1 = ftemp;
				ftemp = prect->m_fY1;
				prect->m_fY1 = m_fY1;
				m_fY1 = ftemp;
				ftemp = prect->m_fX2;
				prect->m_fX2 = m_fX2;
				m_fX2 = ftemp;
				ftemp = prect->m_fY2;
				prect->m_fY2 = m_fY2;
				m_fY2 = ftemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(prect->m_fX1, prect->m_fY1, prect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ROUNDRECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
			{
				COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
				
				double ftemp;
				ftemp = proundrect->m_fX1;
				proundrect->m_fX1 = m_fX1;
				m_fX1 = ftemp;
				ftemp = proundrect->m_fY1;
				proundrect->m_fY1 = m_fY1;
				m_fY1 = ftemp;
				ftemp = proundrect->m_fX2;
				proundrect->m_fX2 = m_fX2;
				m_fX2 = ftemp;
				ftemp = proundrect->m_fY2;
				proundrect->m_fY2 = m_fY2;
				m_fY2 = ftemp;
				ftemp = proundrect->m_fRadius_x;
				proundrect->m_fRadius_x = m_fRx;
				m_fRx = ftemp;
				ftemp = proundrect->m_fRadius_y;
				proundrect->m_fRadius_y = m_fRy;
				m_fRy = ftemp;	

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(proundrect->m_fX1, proundrect->m_fY1, proundrect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
			{
				COb_schellipse* pellipse = (COb_schellipse*)pobject;

				double ftemp;
				ftemp = pellipse->m_fCx;
				pellipse->m_fCx = m_fX1;
				m_fX1 = ftemp;
				ftemp = pellipse->m_fCy;
				pellipse->m_fCy = m_fY1;
				m_fY1 = ftemp;
				ftemp = pellipse->m_fSangle;
				pellipse->m_fSangle = m_fX2;
				m_fX2 = ftemp;
				ftemp = pellipse->m_fEangle;
				pellipse->m_fEangle = m_fY2;
				m_fY2 = ftemp;
				ftemp = pellipse->m_fRadius_x;
				pellipse->m_fRadius_x = m_fRx;
				m_fRx = ftemp;
				ftemp = pellipse->m_fRadius_y;
				pellipse->m_fRadius_y = m_fRy;
				m_fRy = ftemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipse->m_fCx, pellipse->m_fCy, pellipse->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSEFILL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
			{
				COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
				
				double ftemp;
				ftemp = pellipsefill->m_fCx;
				pellipsefill->m_fCx = m_fX1;
				m_fX1 = ftemp;
				ftemp = pellipsefill->m_fCy;
				pellipsefill->m_fCy = m_fY1;
				m_fY1 = ftemp;
				ftemp = pellipsefill->m_fRadius_x;
				pellipsefill->m_fRadius_x = m_fRx;
				m_fRx = ftemp;
				ftemp = pellipsefill->m_fRadius_y;
				pellipsefill->m_fRadius_y = m_fRy;
				m_fRy = ftemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipsefill->m_fCx, pellipsefill->m_fCy, pellipsefill->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PORT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport = (COb_schport*)pobject;

				double ftemp;
				int itemp;
				ftemp = pport->m_fX;
				pport->m_fX = m_fX1;
				m_fX1 = ftemp;
				ftemp = pport->m_fY;
				pport->m_fY = m_fY1;
				m_fY1 = ftemp;
				ftemp = pport->m_fSize;
				pport->m_fSize = m_fX2;
				m_fX2 = ftemp;
				itemp = pport->m_nStyle;
				pport->m_nStyle = m_nOrientation;
				m_nOrientation = itemp;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pport->m_fX, pport->m_fY, pport->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POWER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				
				double ftemp;
				int itemp;
				ftemp = ppower->m_fX;
				ppower->m_fX = m_fX1;
				m_fX1 = ftemp;
				ftemp = ppower->m_fY;
				ppower->m_fY = m_fY1;
				m_fY1 = ftemp;
				itemp = ppower->m_nOrientation;
				ppower->m_nOrientation = m_nOrientation;
				m_nOrientation = itemp;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppower->m_fX, ppower->m_fY, ppower->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETENTRY)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
			{
				COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;

				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pDoc->m_listSchObject[m_nDiagram].GetAt(psheetentry->m_posParent);

				double ftemp;
				int itemp;
				ftemp = psheetentry->m_fOffset;
				psheetentry->m_fOffset = m_fX1;
				m_fX1 = ftemp;
				itemp = psheetentry->m_nSide;
				psheetentry->m_nSide = m_nOrientation;
				m_nOrientation = itemp;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetentry->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETSYMBOL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;

				double ftemp;
				ftemp = psheetsymbol->m_fX;
				psheetsymbol->m_fX = m_fX1;
				m_fX1 = ftemp;
				ftemp = psheetsymbol->m_fY;
				psheetsymbol->m_fY = m_fY1;
				m_fY1 = ftemp;
				ftemp = psheetsymbol->m_fXsize;
				psheetsymbol->m_fXsize = m_fX2;
				m_fX2 = ftemp;
				ftemp = psheetsymbol->m_fYsize;
				psheetsymbol->m_fYsize = m_fY2;
				m_fY2 = ftemp;
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetsymbol->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_TEXTFRAME)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
			{
				COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;

				double ftemp;
				ftemp = ptextframe->m_fX1;
				ptextframe->m_fX1 = m_fX1;
				m_fX1 = ftemp;
				ftemp = ptextframe->m_fY1;
				ptextframe->m_fY1 = m_fY1;
				m_fY1 = ftemp;
				ftemp = ptextframe->m_fX2;
				ptextframe->m_fX2 = m_fX2;
				m_fX2 = ftemp;
				ftemp = ptextframe->m_fY2;
				ptextframe->m_fY2 = m_fY2;
				m_fY2 = ftemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptextframe->m_fX1, ptextframe->m_fY1, ptextframe->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_GRAPHIC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
			{
				COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
				
				double ftemp;
				ftemp = pgraphic->m_fX1;
				pgraphic->m_fX1 = m_fX1;
				m_fX1 = ftemp;
				ftemp = pgraphic->m_fY1;
				pgraphic->m_fY1 = m_fY1;
				m_fY1 = ftemp;
				ftemp = pgraphic->m_fX2;
				pgraphic->m_fX2 = m_fX2;
				m_fX2 = ftemp;
				ftemp = pgraphic->m_fY2;
				pgraphic->m_fY2 = m_fY2;
				m_fY2 = ftemp;

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pgraphic->m_fX1, pgraphic->m_fY1, pgraphic->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
	}
	else if(m_nOp == UNDO_CHANGE)
	{
		if(m_nOb == SCH_ELEMENT_COMPONENT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;
				COb_schcomp tempcomp;
				tempcomp.Copy(pcomp);
				pcomp->Copy(&schcomp);
				schcomp.Copy(&tempcomp);

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pcomp->m_fX, pcomp->m_fY, pcomp->m_nDiagram);
					changeviewpoint = FALSE;
				}				
				
			}
		}
		else if(m_nOb == SCH_ELEMENT_PIN)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
			{
				COb_schpin* ppin = (COb_schpin*)pobject;
				COb_schpin temppin;
				temppin.Copy(ppin);
				ppin->Copy(&schpin);
				schpin.Copy(&temppin);

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppin->m_fX, ppin->m_fY, ppin->m_nDiagram);
					changeviewpoint = FALSE;
				}				
				
			}
		}
		else if(m_nOb == SCH_ELEMENT_IEEE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
			{
				COb_schieee* pieee = (COb_schieee*)pobject;
				COb_schieee tempieee;
				tempieee.Copy(pieee);
				pieee->Copy(&schieee);
				schieee.Copy(&tempieee);

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pieee->m_fX, pieee->m_fY, pieee->m_nDiagram);
					changeviewpoint = FALSE;
				}				
				
			}
		}
		else if(m_nOb == SCH_ELEMENT_ARC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
			{
				COb_scharc* parc = (COb_scharc*)pobject;
				
				COb_scharc temparc;
				temparc.Copy(parc);
				parc->Copy(&scharc);
				scharc.Copy(&temparc);

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(parc->m_fCx, parc->m_fCy, parc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_BEZIER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
			{
				COb_schbezier* pbezier = (COb_schbezier*)pobject;
				COb_schbezier tempbezier;
				tempbezier.Copy(pbezier);
				pbezier->Copy(&schbezier);
				schbezier.Copy(&tempbezier);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pbezier->m_cVertex[0].fx, pbezier->m_cVertex[0].fy, pbezier->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
			{
				COb_schellipse* pellipse = (COb_schellipse*)pobject;
				COb_schellipse tempellipse;
				tempellipse.Copy(pellipse);
				pellipse->Copy(&schellipse);
				schellipse.Copy(&tempellipse);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipse->m_fCx, pellipse->m_fCy, pellipse->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ELLIPSEFILL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
			{
				COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
				COb_schellipsefill tempellipsefill;
				tempellipsefill.Copy(pellipsefill);
				pellipsefill->Copy(&schellipsefill);
				schellipsefill.Copy(&tempellipsefill);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pellipsefill->m_fCx, pellipsefill->m_fCy, pellipsefill->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_GRAPHIC)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
			{
				COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
				COb_schgraphic tempgraphic;
				tempgraphic.Copy(pgraphic);
				pgraphic->Copy(&schgraphic);
				schgraphic.Copy(&tempgraphic);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pgraphic->m_fX1, pgraphic->m_fY1, pgraphic->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_JUNCTION)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc = (COb_schjunc*)pobject;
				COb_schjunc tempjunc;
				tempjunc.Copy(pjunc);
				pjunc->Copy(&schjunc);
				schjunc.Copy(&tempjunc);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pjunc->m_fCx, pjunc->m_fCy, pjunc->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_LINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				COb_schline templine;
				templine.Copy(pline);
				pline->Copy(&schline);
				schline.Copy(&templine);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pline->m_cVertex[0].fx, pline->m_cVertex[0].fy, pline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PIE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
			{
				COb_schpie* ppie = (COb_schpie*)pobject;
				COb_schpie temppie;
				temppie.Copy(ppie);
				ppie->Copy(&schpie);
				schpie.Copy(&temppie);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppie->m_fCx, ppie->m_fCy, ppie->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYGON)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
			{
				COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
				COb_schpolygon temppolygon;
				temppolygon.Copy(ppolygon);
				ppolygon->Copy(&schpolygon);
				schpolygon.Copy(&temppolygon);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolygon->m_cVertex[0].fx, ppolygon->m_cVertex[0].fy, ppolygon->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POLYLINE)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
			{
				COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
				COb_schpolyline temppolyline;
				temppolyline.Copy(ppolyline);
				ppolyline->Copy(&schpolyline);
				schpolyline.Copy(&temppolyline);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppolyline->m_cVertex[0].fx, ppolyline->m_cVertex[0].fy, ppolyline->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_PORT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport = (COb_schport*)pobject;
				COb_schport tempport;
				tempport.Copy(pport);
				pport->Copy(&schport);
				schport.Copy(&tempport);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(pport->m_fX, pport->m_fY, pport->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_POWER)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				COb_schpower temppower;
				temppower.Copy(ppower);
				ppower->Copy(&schpower);
				schpower.Copy(&temppower);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ppower->m_fX, ppower->m_fY, ppower->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_RECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
			{
				COb_schrect* prect = (COb_schrect*)pobject;
				COb_schrect temprect;
				temprect.Copy(prect);
				prect->Copy(&schrect);
				schrect.Copy(&temprect);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(prect->m_fX1, prect->m_fY1, prect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_ROUNDRECT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
			{
				COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
				COb_schroundrect temproundrect;
				temproundrect.Copy(proundrect);
				proundrect->Copy(&schroundrect);
				schroundrect.Copy(&temproundrect);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(proundrect->m_fX1, proundrect->m_fY1, proundrect->m_nDiagram);
					changeviewpoint = FALSE;
				}
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETENTRY)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
			{
				COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
				COb_schsheetentry tempsheetentry;
				tempsheetentry.Copy(psheetentry);
				psheetentry->Copy(&schsheetentry);
				schsheetentry.Copy(&tempsheetentry);

				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pDoc->m_listSchObject[m_nDiagram].GetAt(psheetentry->m_posParent);
				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetentry->m_nDiagram);
					changeviewpoint = FALSE;
				}				
				
			}
		}
		else if(m_nOb == SCH_ELEMENT_SHEETSYMBOL)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
				COb_schsheetsymbol tempsheetsymbol;
				tempsheetsymbol.Copy(psheetsymbol);
				psheetsymbol->Copy(&schsheetsymbol);
				schsheetsymbol.Copy(&tempsheetsymbol);

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(psheetsymbol->m_fX, psheetsymbol->m_fY, psheetsymbol->m_nDiagram);
					changeviewpoint = FALSE;
				}				
				
			}
		}
		else if(m_nOb == SCH_ELEMENT_TEXT)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				COb_schtext temptext;
				temptext.Copy(ptext);
				ptext->Copy(&schtext);
				schtext.Copy(&temptext);

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, ptext->m_nDiagram);
					changeviewpoint = FALSE;
				}				
				
			}
		}
		else if(m_nOb == SCH_ELEMENT_TEXTFRAME)
		{
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_nPos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
			{
				COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
				COb_schtextframe temptextframe;
				temptextframe.Copy(ptextframe);
				ptextframe->Copy(&schtextframe);
				schtextframe.Copy(&temptextframe);

				if(changeviewpoint == TRUE)
				{
					pView->Change_Viewpoint(ptextframe->m_fX1, ptextframe->m_fY1, ptextframe->m_nDiagram);
					changeviewpoint = FALSE;
				}				
				
			}
		}
	}

	pView->Invalidate();
}
