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

CPcbUndo::CPcbUndo()
{
	changeviewpoint = FALSE;
}

CPcbUndo::~CPcbUndo()
{
	// This empty constructor should be used by serialization only
}

void CPcbUndo::Undo(void)
{
	CESDApp* pApp = (CESDApp*)AfxGetApp();
	CESDView* pView = (CESDView*)pApp->GetCurrentView();
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(	m_nOp == UNDO_DELETE)  
	{
		if(m_nOb == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(m_nIndex);
			ptrack->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ptrack->m_fX1, ptrack->m_fY1, 0);
				changeviewpoint = FALSE;
			}
		}		
		else if(m_nOb == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(m_nIndex);
			parc->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(parc->m_fCx, parc->m_fCy, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_TEXT)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(m_nIndex);
			ptext->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, 0);
				changeviewpoint = FALSE;
			}
		}		
		else if(m_nOb == PCB_ELEMENT_FILL)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(m_nIndex);
			pfill->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pfill->m_fX1, pfill->m_fY1, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(m_nIndex);
			ppad->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ppad->m_fCx, ppad->m_fCy, 0);
				changeviewpoint = FALSE;
			}
		}		
		else if(m_nOb == PCB_ELEMENT_VIA)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(m_nIndex);
			pvia->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pvia->m_fCx, pvia->m_fCy, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_REGION)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(m_nIndex);
			pregion->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pregion->m_cVertex[0].fx, pregion->m_cVertex[0].fy, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_POLYGON)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(m_nIndex);
			ppolygon->m_nFlag = FLAG_DELETED;
		}
		else if(m_nOb == PCB_ELEMENT_COORDINATE)
		{
			COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(m_nIndex);
			pcoordinate->m_nFlag = FLAG_DELETED;
		}
		else if(m_nOb == PCB_ELEMENT_DIMENSION)
		{
			COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(m_nIndex);
			pdimension->m_nFlag = FLAG_DELETED;
		}
		else if(m_nOb == PCB_ELEMENT_COMP)
		{
			COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(m_nIndex);
			pcomp->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pcomp->m_fX, pcomp->m_fY, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_NET)
		{
			COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(m_nIndex);
			pnet->m_nFlag = FLAG_DELETED;
		}
	}
	else if(m_nOp == UNDO_UNDELETE)  
	{
		if(m_nOb == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(m_nIndex);
			ptrack->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ptrack->m_fX1, ptrack->m_fY1, 0);
				changeviewpoint = FALSE;
			}
		}		
		else if(m_nOb == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(m_nIndex);
			parc->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(parc->m_fCx, parc->m_fCy, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_TEXT)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(m_nIndex);
			ptext->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, 0);
				changeviewpoint = FALSE;
			}
		}		
		else if(m_nOb == PCB_ELEMENT_FILL)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(m_nIndex);
			pfill->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pfill->m_fX1, pfill->m_fY1, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(m_nIndex);
			ppad->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ppad->m_fCx, ppad->m_fCy, 0);
				changeviewpoint = FALSE;
			}
		}		
		else if(m_nOb == PCB_ELEMENT_VIA)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(m_nIndex);
			pvia->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pvia->m_fCx, pvia->m_fCy, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_REGION)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(m_nIndex);
			pregion->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pregion->m_cVertex[0].fx, pregion->m_cVertex[0].fy, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_POLYGON)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(m_nIndex);
			ppolygon->m_nFlag = FLAG_NORMAL;
		}
		else if(m_nOb == PCB_ELEMENT_COORDINATE)
		{
			COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(m_nIndex);
			pcoordinate->m_nFlag = FLAG_NORMAL;
		}
		else if(m_nOb == PCB_ELEMENT_DIMENSION)
		{
			COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(m_nIndex);
			pdimension->m_nFlag = FLAG_NORMAL;
		}
		else if(m_nOb == PCB_ELEMENT_COMP)
		{
			COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(m_nIndex);
			pcomp->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pcomp->m_fX, pcomp->m_fY, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_NET)
		{
			COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(m_nIndex);
			pnet->m_nFlag = FLAG_NORMAL;
		}
	}
	else if(m_nOp == UNDO_CHANGE)
	{
		if(m_nOb == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(m_nIndex);
			COb_pcbarc temparc;
			temparc.Copy(parc);
			parc->Copy(&pcbarc);
			pcbarc.Copy(&temparc);
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(parc->m_fCx, parc->m_fCy, parc->m_nLayer);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(m_nIndex);
			COb_pcbtrack temptrack;
			temptrack.Copy(ptrack);
			ptrack->Copy(&pcbtrack);
			pcbtrack.Copy(&temptrack);
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ptrack->m_fX1, ptrack->m_fY1, ptrack->m_nLayer);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_TEXT)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(m_nIndex);
			COb_pcbtext temptext;
			temptext.Copy(ptext);
			ptext->Copy(&pcbtext);
			pcbtext.Copy(&temptext);
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ptext->m_fX1, ptext->m_fY1, ptext->m_nLayer);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_FILL)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(m_nIndex);
			COb_pcbfill tempfill;
			tempfill.Copy(pfill);
			pfill->Copy(&pcbfill);
			pcbfill.Copy(&tempfill);
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pfill->m_fX1, pfill->m_fY1, pfill->m_nLayer);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_VIA)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(m_nIndex);
			COb_pcbvia tempvia;
			tempvia.Copy(pvia);
			pvia->Copy(&pcbvia);
			pcbvia.Copy(&tempvia);
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pvia->m_fCx, pvia->m_fCy, pvia->m_nLayer);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(m_nIndex);
			COb_pcbpad temppad;
			temppad.Copy(ppad);
			ppad->Copy(&pcbpad);
			pcbpad.Copy(&temppad);
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ppad->m_fCx, ppad->m_fCy, ppad->m_nLayer);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_REGION)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(m_nIndex);
			COb_pcbregion tempregion;
			tempregion.Copy(pregion);
			pregion->Copy(&pcbregion);
			pcbregion.Copy(&tempregion);
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pregion->m_cVertex[0].fx, pregion->m_cVertex[0].fy, pregion->m_nLayer);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_POLYGON)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(m_nIndex);
			COb_pcbpolygon temppolygon;
			temppolygon.Copy(ppolygon);
			ppolygon->Copy(&pcbpolygon);
			pcbpolygon.Copy(&temppolygon);
		}
		else if(m_nOb == PCB_ELEMENT_COMP)
		{
			COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(m_nIndex);
			COb_pcbcomp tempcomp;
			tempcomp.Copy(pcomp);
			pcomp->Copy(&pcbcomp);
			pcbcomp.Copy(&tempcomp);
		}
		if(m_nOb == PCB_ELEMENT_NET)
		{
			COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(m_nIndex);
			COb_pcbnet tempnet;
			tempnet.Copy(pnet);
			pnet->Copy(&pcbnet);
			pcbnet.Copy(&tempnet);
		}
	}

	pView->Invalidate();
}


void CPcbUndo::Redo(void)
{
	CESDApp* pApp = (CESDApp*)AfxGetApp();
	CESDView* pView = (CESDView*)pApp->GetCurrentView();
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(	m_nOp == UNDO_DELETE)  
	{
		if(m_nOb == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(m_nIndex);
			ptrack->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ptrack->m_fX1, ptrack->m_fY1, 0);
				changeviewpoint = FALSE;
			}
		}		
		else if(m_nOb == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(m_nIndex);
			parc->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(parc->m_fCx, parc->m_fCy, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_TEXT)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(m_nIndex);
			ptext->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, 0);
				changeviewpoint = FALSE;
			}
		}		
		else if(m_nOb == PCB_ELEMENT_FILL)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(m_nIndex);
			pfill->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pfill->m_fX1, pfill->m_fY1, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(m_nIndex);
			ppad->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ppad->m_fCx, ppad->m_fCy, 0);
				changeviewpoint = FALSE;
			}
		}		
		else if(m_nOb == PCB_ELEMENT_VIA)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(m_nIndex);
			pvia->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pvia->m_fCx, pvia->m_fCy, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_REGION)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(m_nIndex);
			pregion->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pregion->m_cVertex[0].fx, pregion->m_cVertex[0].fy, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_POLYGON)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(m_nIndex);
			ppolygon->m_nFlag = FLAG_NORMAL;
		}
		else if(m_nOb == PCB_ELEMENT_COORDINATE)
		{
			COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(m_nIndex);
			pcoordinate->m_nFlag = FLAG_NORMAL;
		}
		else if(m_nOb == PCB_ELEMENT_DIMENSION)
		{
			COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(m_nIndex);
			pdimension->m_nFlag = FLAG_NORMAL;
		}
		else if(m_nOb == PCB_ELEMENT_COMP)
		{
			COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(m_nIndex);
			pcomp->m_nFlag = FLAG_NORMAL;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pcomp->m_fX, pcomp->m_fY, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_NET)
		{
			COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(m_nIndex);
			pnet->m_nFlag = FLAG_NORMAL;
		}
	}
	else if(m_nOp == UNDO_UNDELETE)  
	{
		if(m_nOb == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(m_nIndex);
			ptrack->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ptrack->m_fX1, ptrack->m_fY1, 0);
				changeviewpoint = FALSE;
			}
		}		
		else if(m_nOb == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(m_nIndex);
			parc->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(parc->m_fCx, parc->m_fCy, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_TEXT)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(m_nIndex);
			ptext->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, 0);
				changeviewpoint = FALSE;
			}
		}		
		else if(m_nOb == PCB_ELEMENT_FILL)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(m_nIndex);
			pfill->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pfill->m_fX1, pfill->m_fY1, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(m_nIndex);
			ppad->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ppad->m_fCx, ppad->m_fCy, 0);
				changeviewpoint = FALSE;
			}
		}		
		else if(m_nOb == PCB_ELEMENT_VIA)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(m_nIndex);
			pvia->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pvia->m_fCx, pvia->m_fCy, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_REGION)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(m_nIndex);
			pregion->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pregion->m_cVertex[0].fx, pregion->m_cVertex[0].fy, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_POLYGON)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(m_nIndex);
			ppolygon->m_nFlag = FLAG_DELETED;
		}
		else if(m_nOb == PCB_ELEMENT_COORDINATE)
		{
			COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(m_nIndex);
			pcoordinate->m_nFlag = FLAG_DELETED;
		}
		else if(m_nOb == PCB_ELEMENT_DIMENSION)
		{
			COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(m_nIndex);
			pdimension->m_nFlag = FLAG_DELETED;
		}
		else if(m_nOb == PCB_ELEMENT_COMP)
		{
			COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(m_nIndex);
			pcomp->m_nFlag = FLAG_DELETED;
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pcomp->m_fX, pcomp->m_fY, 0);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_NET)
		{
			COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(m_nIndex);
			pnet->m_nFlag = FLAG_DELETED;
		}
	}
	else if(m_nOp == UNDO_CHANGE)
	{
		if(m_nOb == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(m_nIndex);
			COb_pcbarc temparc;
			temparc.Copy(parc);
			parc->Copy(&pcbarc);
			pcbarc.Copy(&temparc);
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(parc->m_fCx, parc->m_fCy, parc->m_nLayer);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(m_nIndex);
			COb_pcbtrack temptrack;
			temptrack.Copy(ptrack);
			ptrack->Copy(&pcbtrack);
			pcbtrack.Copy(&temptrack);
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ptrack->m_fX1, ptrack->m_fY1, ptrack->m_nLayer);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_TEXT)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(m_nIndex);
			COb_pcbtext temptext;
			temptext.Copy(ptext);
			ptext->Copy(&pcbtext);
			pcbtext.Copy(&temptext);
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ptext->m_fX1, ptext->m_fY1, ptext->m_nLayer);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_FILL)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(m_nIndex);
			COb_pcbfill tempfill;
			tempfill.Copy(pfill);
			pfill->Copy(&pcbfill);
			pcbfill.Copy(&tempfill);
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pfill->m_fX1, pfill->m_fY1, pfill->m_nLayer);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_VIA)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(m_nIndex);
			COb_pcbvia tempvia;
			tempvia.Copy(pvia);
			pvia->Copy(&pcbvia);
			pcbvia.Copy(&tempvia);
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pvia->m_fCx, pvia->m_fCy, pvia->m_nLayer);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(m_nIndex);
			COb_pcbpad temppad;
			temppad.Copy(ppad);
			ppad->Copy(&pcbpad);
			pcbpad.Copy(&temppad);
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(ppad->m_fCx, ppad->m_fCy, ppad->m_nLayer);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_REGION)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(m_nIndex);
			COb_pcbregion tempregion;
			tempregion.Copy(pregion);
			pregion->Copy(&pcbregion);
			pcbregion.Copy(&tempregion);
			if(changeviewpoint == TRUE)
			{
				pView->Change_Viewpoint(pregion->m_cVertex[0].fx, pregion->m_cVertex[0].fy, pregion->m_nLayer);
				changeviewpoint = FALSE;
			}
		}
		else if(m_nOb == PCB_ELEMENT_POLYGON)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(m_nIndex);
			COb_pcbpolygon temppolygon;
			temppolygon.Copy(ppolygon);
			ppolygon->Copy(&pcbpolygon);
			pcbpolygon.Copy(&temppolygon);
		}
		else if(m_nOb == PCB_ELEMENT_COMP)
		{
			COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(m_nIndex);
			COb_pcbcomp tempcomp;
			tempcomp.Copy(pcomp);
			pcomp->Copy(&pcbcomp);
			pcbcomp.Copy(&tempcomp);
		}
		if(m_nOb == PCB_ELEMENT_NET)
		{
			COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(m_nIndex);
			COb_pcbnet tempnet;
			tempnet.Copy(pnet);
			pnet->Copy(&pcbnet);
			pcbnet.Copy(&tempnet);
		}
	}

	pView->Invalidate();
}
