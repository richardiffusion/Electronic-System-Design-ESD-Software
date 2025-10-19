#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditPin.h"

IMPLEMENT_SERIAL(COb_schpin, CObject, 0)
COb_schpin::COb_schpin()
{
	m_nFlag = 0;
	m_bSelection = FALSE;
	m_nOwnerPartID = 0;
	m_nDisplayMode = 0;
	m_posParent = NULL;

	pDocument = NULL;
}



void COb_schpin::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fX;
		ar << m_fY;
		ar << m_fPinLength;
		ar << m_nOrientation;
		ar << m_nColor;

		ar << m_csName;
		ar << m_bNameVisible;
		
		ar << m_csNumber;
		ar << m_bNumberVisible;
		
		ar << m_nElectricalType;
		ar << m_csDescription;
		
		ar << m_bHide;
		ar << m_csConnectTo;
		ar << m_csUniqueID;
		
		ar << m_bPinLocked;
		
		ar << m_nInside;
		ar << m_nInsideEdge;
		ar << m_nOutside;
		ar << m_nOutsideEdge;
		
		ar << m_nOwnerPartID;
		ar << m_nDisplayMode;
		
		ar << m_nDiagram;
		ar << m_bSelection;
	}
	else
	{
		ar >> m_fX;
		ar >> m_fY;
		ar >> m_fPinLength;
		ar >> m_nOrientation;
		ar >> m_nColor;

		ar >> m_csName;
		ar >> m_bNameVisible;
		
		ar >> m_csNumber;
		ar >> m_bNumberVisible;
		
		ar >> m_nElectricalType;
		ar >> m_csDescription;
		
		ar >> m_bHide;
		ar >> m_csConnectTo;
		ar >> m_csUniqueID;
		
		ar >> m_bPinLocked;
		
		ar >> m_nInside;
		ar >> m_nInsideEdge;
		ar >> m_nOutside;
		ar >> m_nOutsideEdge;
		
		ar >> m_nOwnerPartID;
		ar >> m_nDisplayMode;
		
		ar >> m_nDiagram;
		ar >> m_bSelection;	
	}
}

void COb_schpin::Draw(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;

	if(m_bHide == TRUE) return;

	COb_schline schline;
	schline.m_nDiagram = m_nDiagram;
	schline.m_nAttrib = LINE_WIRE;
	schline.m_nColor = m_nColor;
	schline.m_nLineType = LINE_SOLID;
	schline.m_nLineWidth = LINE_SMALL;
	schline.m_cVertex.SetSize(2);

	schline.m_cVertex[0].fx = m_fX;
	schline.m_cVertex[0].fy = m_fY;
	if(m_nOrientation == 0)
	{
		schline.m_cVertex[1].fx = m_fX + m_fPinLength;
		schline.m_cVertex[1].fy = m_fY;
	}
	else if(m_nOrientation == 90)
	{
		schline.m_cVertex[1].fx = m_fX;
		schline.m_cVertex[1].fy = m_fY + m_fPinLength;
	}
	else if(m_nOrientation == 180)
	{
		schline.m_cVertex[1].fx = m_fX - m_fPinLength;
		schline.m_cVertex[1].fy = m_fY;
	}
	else if(m_nOrientation == 270)
	{
		schline.m_cVertex[1].fx = m_fX;
		schline.m_cVertex[1].fy = m_fY - m_fPinLength;
	}
	if(schline.PickMe(pV) == TRUE)
	{
		schline.Draw(pDC, pV);

		//Hot Point
		if((pView->m_fScale < 10)&&(nEDA_Print_Flag == 0))
		{
			CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
			CPen apen;
			apen.CreatePen(PS_SOLID, 1, pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor);
			CPen* oldpen = pDC->SelectObject(&apen);
			int x, y, x0, y0;
			pView->Get_VPort_XY(&x, &y, schline.m_cVertex[1].fx, schline.m_cVertex[1].fy);
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

	COb_schtext schtext;
	schtext.m_nFlag = 0;
	schtext.m_nDiagram = m_nDiagram;
	schtext.m_nColor = m_nColor;
	schtext.m_nFont = 0;
	schtext.m_nOrientation = m_nOrientation%180;

	if(m_bNumberVisible == TRUE)
	{
		schtext.m_csText = m_csNumber;
		
		schtext.m_fSx = schtext.m_fSy = 0;

		if(m_nOrientation == 0)
		{
			schtext.m_fSx = m_fX + 100 - 20;
			schtext.m_fSy = m_fY;
			schtext.m_nH_Justification = 0;
		}
		else if(m_nOrientation == 90)
		{
			schtext.m_fSx = m_fX;
			schtext.m_fSy = m_fY + 100 - 20;
			schtext.m_nH_Justification = 0;
		}
		else if(m_nOrientation == 180)
		{
			schtext.m_fSx = m_fX - 100 + 20; // - (double)offset*pView->m_fScale;
			schtext.m_fSy = m_fY;
			schtext.m_nH_Justification = 2;
		}
		else if(m_nOrientation == 270)
		{
			schtext.m_fSx = m_fX;
			schtext.m_fSy = m_fY - 100 + 20; // - (double)offset*pView->m_fScale;
			schtext.m_nH_Justification = 2;
		}
		if(schtext.PickMe(pDC, pV) == TRUE) schtext.Draw(pDC, pV);		
	}
	
	COb_schieee schieee;
	schieee.m_nDiagram = m_nDiagram;
	schieee.m_nColor = m_nColor;
	schieee.m_nWidth = 0;

	int delta;
	if(m_nOutsideEdge == 0)
	{
		delta = 0;
	}
	else if(m_nOutsideEdge == 1)
	{
		delta = 60;
		schieee.m_nIeeeSymbol = m_nOutsideEdge;
		schieee.m_fSize = 10.0f;
		schieee.m_bMirrored = FALSE;
		schieee.m_nOrientation = 0;
		if(m_nOrientation == 0)
		{
			schieee.m_fX = m_fX + 30;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + 30;
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_fX = m_fX - 30;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - 30;
		}
		schieee.Draw(pDC, pV);
	}
	else if(m_nOutsideEdge == 4)
	{
		delta = 60;
		schieee.m_nIeeeSymbol = m_nOutsideEdge;
		schieee.m_fSize = 5;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + 60;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60;
			schieee.m_fY = m_fY;			
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - 60;	
		}
		schieee.Draw(pDC, pV);		
	}
	else if(m_nOutsideEdge == 17)
	{
		delta = 60;
		schieee.m_nIeeeSymbol = m_nOutsideEdge;
		schieee.m_fSize = 5;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY;			
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY;	
		}
		schieee.Draw(pDC, pV);	
	}

	if((m_nOutside == 2)||((m_nOutside == 0)&&(m_nElectricalType == 0)))
	{
		schieee.m_nIeeeSymbol = 2;
		schieee.m_fSize = 5;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 180;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - delta;
			schieee.m_fY = m_fY + 20;			
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - delta;	
		}
		schieee.Draw(pDC, pV);	
	}
	else if((m_nOutside == 33)||((m_nOutside == 0)&&(m_nElectricalType == 2)))
	{
		schieee.m_nIeeeSymbol = 33;
		schieee.m_fSize = 10;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + delta + 60;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + delta + 60;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 180;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - delta - 60;
			schieee.m_fY = m_fY;			
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - delta - 60;	
		}
		schieee.Draw(pDC, pV);	
	}
	else if((m_nOutside == 34)||((m_nOutside == 0)&&(m_nElectricalType == 1)))
	{
		schieee.m_nIeeeSymbol = 34;
		schieee.m_fSize = 10;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + delta;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 180;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - delta;
			schieee.m_fY = m_fY;			
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - delta;	
		}
		schieee.Draw(pDC, pV);	
	}
	else if(m_nOutside == 5)
	{
		schieee.m_nIeeeSymbol = m_nOutside;
		schieee.m_fSize = 6.66666667f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + delta + 10;
			schieee.m_fY = m_fY + 40;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 40;
			schieee.m_fY = m_fY + delta + 10;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - delta - 50;
			schieee.m_fY = m_fY + 40;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX -40;
			schieee.m_fY = m_fY - delta -50;	
		}
		schieee.Draw(pDC, pV);
	}
	else if(m_nOutside == 6)
	{
		schieee.m_nIeeeSymbol = m_nOutside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + delta + 10;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + delta + 10;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - delta - 50;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - delta - 50;	
		}
		schieee.Draw(pDC, pV);
	}
	else if(m_nOutside == 25)
	{
		schieee.m_nIeeeSymbol = m_nOutside;
		schieee.m_fSize = 10.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + delta + 30;
			schieee.m_fY = m_fY + 30;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 30;
			schieee.m_fY = m_fY + delta + 30;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - delta - 60;
			schieee.m_fY = m_fY + 30;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 30;
			schieee.m_fY = m_fY - delta - 60;	
		}
		schieee.Draw(pDC, pV);
	}


	delta = 0;
	if(m_nInsideEdge == 3)
	{
		schieee.m_nIeeeSymbol = 11;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 180;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 20;
			schieee.m_fY = m_fY;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY;
		}
		schieee.Draw(pDC, pV);
		delta += 60;
	}

	if(m_nInside == 8)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60 - delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - 60 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60 + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 180;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + 60 + delta;
		}
		schieee.Draw(pDC, pV);
		delta += 40;
	}
	else if(m_nInside == 9)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60 - delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - 60 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60 + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + 20 + delta;
		}
		schieee.Draw(pDC, pV);
		delta += 40;
	}
	else if(m_nInside == 10)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60 - delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - 60 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60 + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + 20 + delta;
		}
		schieee.Draw(pDC, pV);
		delta += 40;
	}
	else if(m_nInside == 11)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60 - delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - 60 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60 + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 20;
			schieee.m_fY = m_fY + 60 + delta;
		}
		schieee.Draw(pDC, pV);
		delta += 40;	
	}
	else if(m_nInside == 22)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60 - delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - 60 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60 + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + 20 + delta;
		}
		schieee.Draw(pDC, pV);
		delta += 40;
	}
	else if(m_nInside == 23)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60 - delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - 60 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60 + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + 20 + delta;
		}
		schieee.Draw(pDC, pV);
		delta += 40;
	}
	else if(m_nInside == 24)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60 - delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - 60 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60 + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + 20 + delta;
		}
		schieee.Draw(pDC, pV);
		delta += 40;
	}
	else if(m_nInside == 32)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 10.0f;
		schieee.m_nOrientation = 0;
		schieee.m_bMirrored = FALSE;
		if(m_nOrientation == 0)
		{
			schieee.m_fX = m_fX - 40 - delta;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - 40 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_fX = m_fX + 40 + delta;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + 40 + delta;
		}
		schieee.Draw(pDC, pV);
		delta += 40;
	}
	else if(m_nInside == 12)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 140 - delta;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - 140 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 140 + delta;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + 20 + delta;
		}
		schieee.Draw(pDC, pV);
		delta += 120;
	}
	else if(m_nInside == 13)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 10.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 180 - delta;
			schieee.m_fY = m_fY - 40;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 40;
			schieee.m_fY = m_fY - 180 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20 + delta;
			schieee.m_fY = m_fY - 40;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 40;
			schieee.m_fY = m_fY + 20 + delta;
		}
		schieee.Draw(pDC, pV);
		delta += 160;
	}
	else if(m_nInside == 30)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 80 - delta;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - 80 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 80 + delta;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + 80 + delta;
		}
		schieee.Draw(pDC, pV);
		delta += 60;
	}

	if(m_bNameVisible == TRUE)
	{
		schtext.m_csText = m_csName;
		
		schtext.m_fSx = schtext.m_fSy = 0;
		//int offset = schtext.GetSignalExtent(pDC, pV);


		if(m_nOrientation == 0)
		{
			schtext.m_fSx = m_fX - 70 - delta; // - (double)offset*pView->m_fScale;
			schtext.m_fSy = m_fY - 50;
			schtext.m_nH_Justification = 2;
		}
		else if(m_nOrientation == 90)
		{
			schtext.m_fSx = m_fX + 50;
			schtext.m_fSy = m_fY - 70 - delta; // - (double)offset*pView->m_fScale;
			schtext.m_nH_Justification = 2;
		}
		else if(m_nOrientation == 180)
		{
			schtext.m_fSx = m_fX + 70 + delta;
			schtext.m_fSy = m_fY - 50;
			schtext.m_nH_Justification = 0;
		}
		else if(m_nOrientation == 270)
		{
			schtext.m_fSx = m_fX + 50;
			schtext.m_fSy = m_fY + 70 + delta;
			schtext.m_nH_Justification = 0;
		}
		if(schtext.PickMe(pDC, pV) == TRUE) schtext.DrawSignal(pDC, pV);		
	}
}


void COb_schpin::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	if(m_bHide == TRUE) return;

	COb_schline schline;
	schline.m_nDiagram = m_nDiagram;
	schline.m_nAttrib = LINE_WIRE;
	schline.m_nColor = m_nColor;
	schline.m_nLineType = LINE_SOLID;
	schline.m_nLineWidth = LINE_SMALL;
	schline.m_cVertex.SetSize(2);

	schline.m_cVertex[0].fx = m_fX;
	schline.m_cVertex[0].fy = m_fY;
	if(m_nOrientation == 0)
	{
		schline.m_cVertex[1].fx = m_fX + m_fPinLength;
		schline.m_cVertex[1].fy = m_fY;
	}
	else if(m_nOrientation == 90)
	{
		schline.m_cVertex[1].fx = m_fX;
		schline.m_cVertex[1].fy = m_fY + m_fPinLength;
	}
	else if(m_nOrientation == 180)
	{
		schline.m_cVertex[1].fx = m_fX - m_fPinLength;
		schline.m_cVertex[1].fy = m_fY;
	}
	else if(m_nOrientation == 270)
	{
		schline.m_cVertex[1].fx = m_fX;
		schline.m_cVertex[1].fy = m_fY - m_fPinLength;
	}
	schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

	COb_schtext schtext;
	schtext.m_nFlag = 0;
	schtext.m_nDiagram = m_nDiagram;
	schtext.m_nColor = m_nColor;
	schtext.m_nFont = 0;
	schtext.m_nOrientation = m_nOrientation%180;

	if(m_bNumberVisible == TRUE)
	{
		schtext.m_csText = m_csNumber;
		
		schtext.m_fSx = schtext.m_fSy = 0;
		//int offset = schtext.GetExtent(pDC, pV);

		if(m_nOrientation == 0)
		{
			schtext.m_fSx = m_fX + 100 - 20;
			schtext.m_fSy = m_fY;
			schtext.m_nH_Justification = 0;
		}
		else if(m_nOrientation == 90)
		{
			schtext.m_fSx = m_fX;
			schtext.m_fSy = m_fY + 100 - 20;
			schtext.m_nH_Justification = 0;
		}
		else if(m_nOrientation == 180)
		{
			schtext.m_fSx = m_fX - 100 + 20; // - (double)offset*pView->m_fScale;
			schtext.m_fSy = m_fY;
			schtext.m_nH_Justification = 2;
		}
		else if(m_nOrientation == 270)
		{
			schtext.m_fSx = m_fX;
			schtext.m_fSy = m_fY - 100 + 20; // - (double)offset*pView->m_fScale;
			schtext.m_nH_Justification = 2;
		}
		schtext.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);	
	}
	
	COb_schieee schieee;
	schieee.m_nDiagram = m_nDiagram;
	schieee.m_nColor = m_nColor;
	schieee.m_nWidth = 0;

	int delta;
	if(m_nOutsideEdge == 0)
	{
		delta = 0;
	}
	else if(m_nOutsideEdge == 1)
	{
		delta = 60;
		schieee.m_nIeeeSymbol = m_nOutsideEdge;
		schieee.m_fSize = 10.0f;
		schieee.m_bMirrored = FALSE;
		schieee.m_nOrientation = 0;
		if(m_nOrientation == 0)
		{
			schieee.m_fX = m_fX + 30;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + 30;
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_fX = m_fX - 30;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - 30;
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nOutsideEdge == 4)
	{
		delta = 60;
		schieee.m_nIeeeSymbol = m_nOutsideEdge;
		schieee.m_fSize = 5;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + 60;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60;
			schieee.m_fY = m_fY;			
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - 60;	
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);	
	}
	else if(m_nOutsideEdge == 17)
	{
		delta = 60;
		schieee.m_nIeeeSymbol = m_nOutsideEdge;
		schieee.m_fSize = 5;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY;			
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY;	
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}

	if((m_nOutside == 2)||((m_nOutside == 0)&&(m_nElectricalType == 0)))
	{
		schieee.m_nIeeeSymbol = 2;
		schieee.m_fSize = 5;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 180;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - delta;
			schieee.m_fY = m_fY + 20;			
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - delta;	
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if((m_nOutside == 33)||((m_nOutside == 0)&&(m_nElectricalType == 2)))
	{
		schieee.m_nIeeeSymbol = 33;
		schieee.m_fSize = 10;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + delta + 60;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + delta + 60;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 180;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - delta - 60;
			schieee.m_fY = m_fY;			
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - delta - 60;	
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if((m_nOutside == 34)||((m_nOutside == 0)&&(m_nElectricalType == 1)))
	{
		schieee.m_nIeeeSymbol = 34;
		schieee.m_fSize = 10;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + delta;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 180;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - delta;
			schieee.m_fY = m_fY;			
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - delta;	
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nOutside == 5)
	{
		schieee.m_nIeeeSymbol = m_nOutside;
		schieee.m_fSize = 6.66666667f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + delta + 10;
			schieee.m_fY = m_fY + 40;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 40;
			schieee.m_fY = m_fY + delta + 10;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - delta - 50;
			schieee.m_fY = m_fY + 40;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX -40;
			schieee.m_fY = m_fY - delta -50;	
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nOutside == 6)
	{
		schieee.m_nIeeeSymbol = m_nOutside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + delta + 10;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + delta + 10;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - delta - 50;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - delta - 50;	
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nOutside == 25)
	{
		schieee.m_nIeeeSymbol = m_nOutside;
		schieee.m_fSize = 10.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + delta + 30;
			schieee.m_fY = m_fY + 30;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 30;
			schieee.m_fY = m_fY + delta + 30;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - delta - 60;
			schieee.m_fY = m_fY + 30;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 30;
			schieee.m_fY = m_fY - delta - 60;	
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}


	delta = 0;
	if(m_nInsideEdge == 3)
	{
		schieee.m_nIeeeSymbol = 11;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 180;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 20;
			schieee.m_fY = m_fY;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY;
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		delta += 60;
	}

	if(m_nInside == 8)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60 - delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - 60 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60 + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 180;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + 60 + delta;
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		delta += 40;
	}
	else if(m_nInside == 9)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60 - delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - 60 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60 + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + 20 + delta;
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		delta += 40;
	}
	else if(m_nInside == 10)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60 - delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - 60 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60 + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + 20 + delta;
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		delta += 40;
	}
	else if(m_nInside == 11)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60 - delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - 60 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60 + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 20;
			schieee.m_fY = m_fY + 60 + delta;
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		delta += 40;	
	}
	else if(m_nInside == 22)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60 - delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - 60 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60 + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + 20 + delta;
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		delta += 40;
	}
	else if(m_nInside == 23)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60 - delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - 60 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60 + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + 20 + delta;
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		delta += 40;
	}
	else if(m_nInside == 24)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 60 - delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY - 60 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 60 + delta;
			schieee.m_fY = m_fY - 20;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20;
			schieee.m_fY = m_fY + 20 + delta;
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		delta += 40;
	}
	else if(m_nInside == 32)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 10.0f;
		schieee.m_nOrientation = 0;
		schieee.m_bMirrored = FALSE;
		if(m_nOrientation == 0)
		{
			schieee.m_fX = m_fX - 40 - delta;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - 40 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_fX = m_fX + 40 + delta;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + 40 + delta;
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		delta += 40;
	}
	else if(m_nInside == 12)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 140 - delta;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - 140 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 140 + delta;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + 20 + delta;
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		delta += 120;
	}
	else if(m_nInside == 13)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 10.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 180 - delta;
			schieee.m_fY = m_fY - 40;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 40;
			schieee.m_fY = m_fY - 180 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 20 + delta;
			schieee.m_fY = m_fY - 40;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX + 40;
			schieee.m_fY = m_fY + 20 + delta;
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		delta += 160;
	}
	else if(m_nInside == 30)
	{
		schieee.m_nIeeeSymbol = m_nInside;
		schieee.m_fSize = 5.0f;
		if(m_nOrientation == 0)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX - 80 - delta;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 90)
		{
			schieee.m_nOrientation = 90;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY - 80 - delta;		
		}
		else if(m_nOrientation == 180)
		{
			schieee.m_nOrientation = 0;
			schieee.m_bMirrored = TRUE;
			schieee.m_fX = m_fX + 80 + delta;
			schieee.m_fY = m_fY;
		}
		else if(m_nOrientation == 270)
		{
			schieee.m_nOrientation = 270;
			schieee.m_bMirrored = FALSE;
			schieee.m_fX = m_fX;
			schieee.m_fY = m_fY + 80 + delta;
		}
		schieee.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
		delta += 60;
	}

	if(m_bNameVisible == TRUE)
	{
		schtext.m_csText = m_csName;
		
		schtext.m_fSx = schtext.m_fSy = 0;

		if(m_nOrientation == 0)
		{
			schtext.m_fSx = m_fX - 70 - delta; // - (double)offset*pView->m_fScale;
			schtext.m_fSy = m_fY - 50;
			schtext.m_nH_Justification = 2;
		}
		else if(m_nOrientation == 90)
		{
			schtext.m_fSx = m_fX + 50;
			schtext.m_fSy = m_fY - 70 - delta; // - (double)offset*pView->m_fScale;
			schtext.m_nH_Justification = 2;
		}
		else if(m_nOrientation == 180)
		{
			schtext.m_fSx = m_fX + 70 + delta;
			schtext.m_fSy = m_fY - 50;
			schtext.m_nH_Justification = 0;
		}
		else if(m_nOrientation == 270)
		{
			schtext.m_fSx = m_fX + 50;
			schtext.m_fSy = m_fY + 70 + delta;
			schtext.m_nH_Justification = 0;
		}
		schtext.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
}

void COb_schpin::DrawOverwrite(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;

	if(m_bHide == TRUE) return;

	COb_schline schline;
	schline.m_nDiagram = m_nDiagram;
	schline.m_nAttrib = LINE_WIRE;
	schline.m_nColor = m_nColor;
	schline.m_nLineType = LINE_SOLID;
	schline.m_nLineWidth = LINE_SMALL;
	schline.m_cVertex.SetSize(2);

	schline.m_cVertex[0].fx = m_fX;
	schline.m_cVertex[0].fy = m_fY;
	if(m_nOrientation == 0)
	{
		schline.m_cVertex[1].fx = m_fX + m_fPinLength;
		schline.m_cVertex[1].fy = m_fY;
	}
	else if(m_nOrientation == 90)
	{
		schline.m_cVertex[1].fx = m_fX;
		schline.m_cVertex[1].fy = m_fY + m_fPinLength;
	}
	else if(m_nOrientation == 180)
	{
		schline.m_cVertex[1].fx = m_fX - m_fPinLength;
		schline.m_cVertex[1].fy = m_fY;
	}
	else if(m_nOrientation == 270)
	{
		schline.m_cVertex[1].fx = m_fX;
		schline.m_cVertex[1].fy = m_fY - m_fPinLength;
	}
	if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
}

BOOL COb_schpin::Is_On_Me(double fx, double fy)
{
	int delta = 1 + nEDA_Selection_Offset;
	
	double fx1, fy1, fx2, fy2;

	fx1 = m_fX;
	fy1 = m_fY;
	if(m_nOrientation == 0)
	{
		fx1 = m_fX;
		fy1 = m_fY - delta;
		fx2 = m_fX + m_fPinLength;
		fy2 = m_fY + delta;
	}
	else if(m_nOrientation == 90)
	{
		fx1 = m_fX - delta;
		fy1 = m_fY;
		fx2 = m_fX + delta;
		fy2 = m_fY + m_fPinLength;
	}
	else if(m_nOrientation == 180)
	{
		fx1 = m_fX - m_fPinLength;
		fy1 = m_fY - delta;
		fx2 = m_fX;
		fy2 = m_fY + delta;
	}
	else if(m_nOrientation == 270)
	{
		fx1 = m_fX - delta;
		fy1 = m_fY - m_fPinLength;
		fx2 = m_fX + delta;
		fy2 = m_fY;
	}

	CRgn rgn;
	rgn.CreateRectRgn((int)fx1, (int)fy1, (int)fx2, (int)fy2);

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

void COb_schpin::Change(POSITION pos)
{
	int		  inside_cov1[40] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 7, 8, 9, 0, 0, 0, 0,  0,10,  0,11, 0, 0, 0, 0, 0, 0, 0};
	int  inside_edge_cov1[40] = {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0};
	int outside_edge_cov1[40] = {0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0};
	int		 outside_cov1[40] = {0, 0, 1, 0, 0, 2, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0,  0, 0,  0, 0, 5, 6, 0, 0, 0, 0, 0};
	
	int		  inside_cov2[12] = {0, 8, 9,10,11,12,13,22,23,24,30,32};
	int  inside_edge_cov2[2 ] = {0, 3};
	int outside_edge_cov2[4 ] = {0, 1, 4, 17};
	int		 outside_cov2[7 ] = {0, 2, 5, 6,25,33,34};

	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_EditPin dlg;
	dlg.pDoc = pDoc;
	
	dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fX);
	dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fY);
	dlg.m_csEditPinLength = pDoc->Convert_XY_To_String(m_fPinLength);
	dlg.m_nComboOrientation = m_nOrientation/90;
	dlg.m_nColor = m_nColor;

	if(m_nInside > 39) dlg.m_nComboInside = 0;
	else dlg.m_nComboInside = inside_cov1[m_nInside];
	if(m_nInsideEdge > 39) dlg.m_nComboInsideEdge = 0;
	else dlg.m_nComboInsideEdge = inside_edge_cov1[m_nInsideEdge];
	if(m_nOutside > 39) dlg.m_nComboOutside = 0;
	else dlg.m_nComboOutside = outside_cov1[m_nOutside];
	if(m_nOutsideEdge > 39) dlg.m_nComboOutsideEdge = 0;
	else dlg.m_nComboOutsideEdge = outside_edge_cov1[m_nOutsideEdge];

	dlg.m_csEditPinName = m_csName;
	dlg.m_csEditPinNumber = m_csNumber;
	dlg.m_csEditDescription = m_csDescription;
	dlg.m_nComboType = m_nElectricalType;
	dlg.m_bCheckNameVisible = m_bNameVisible;
	dlg.m_bCheckNumberVisible = m_bNumberVisible;
	dlg.m_bCheckPinHide = m_bHide;
	dlg.m_nEditPartNum = m_nOwnerPartID;
	dlg.m_csEditConnectTo =  m_csConnectTo.IsEmpty()?m_csName:m_csConnectTo;
	dlg.m_csEditUniqueID = m_csUniqueID;

	dlg.m_cPinDemo.m_pPin = this;
	
	if(dlg.DoModal() == IDOK)
	{
		if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = SCH_ELEMENT_PIN;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = pos;
		pundo->schpin.Copy(this);
			
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

		m_fX = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
		m_fY = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
		m_fPinLength = pDoc->Convert_String_To_XY(dlg.m_csEditPinLength);
		m_nOrientation = dlg.m_nComboOrientation*90;
		m_nColor = dlg.m_nColor;
		
		m_nInside = inside_cov2[dlg.m_nComboInside];
		m_nInsideEdge = inside_edge_cov2[dlg.m_nComboInsideEdge];
		m_nOutside = outside_cov2[dlg.m_nComboOutside];
		m_nOutsideEdge = outside_edge_cov2[dlg.m_nComboOutsideEdge];

		m_csName = dlg.m_csEditPinName;
		m_csNumber = dlg.m_csEditPinNumber;
		m_csDescription = dlg.m_csEditDescription;
		m_nElectricalType = dlg.m_nComboType;
		m_bNameVisible = dlg.m_bCheckNameVisible;
		m_bNumberVisible = dlg.m_bCheckNumberVisible;
		m_bHide = dlg.m_bCheckPinHide;
		m_nOwnerPartID = dlg.m_nEditPartNum;
		m_csConnectTo = dlg.m_csEditConnectTo;
	}
}

void COb_schpin::Copy(COb_schpin* pschpin)
{
	m_fX = pschpin->m_fX;
	m_fY = pschpin->m_fY;
	m_fPinLength = pschpin->m_fPinLength;
	m_nOrientation = pschpin->m_nOrientation;
	m_nColor = pschpin->m_nColor;

	m_csName = pschpin->m_csName;
	m_bNameVisible = pschpin->m_bNameVisible;
	
	m_csNumber = pschpin->m_csNumber;
	m_bNumberVisible = pschpin->m_bNumberVisible;
	
	m_nElectricalType = pschpin->m_nElectricalType;
	m_csDescription = pschpin->m_csDescription;

	m_bHide = pschpin->m_bHide;
	m_csConnectTo = pschpin->m_csConnectTo;
	m_csUniqueID = pschpin->m_csUniqueID;

	m_bPinLocked = pschpin->m_bPinLocked;

	m_nInside = pschpin->m_nInside;
	m_nInsideEdge = pschpin->m_nInsideEdge;
	m_nOutside = pschpin->m_nOutside;
	m_nOutsideEdge = pschpin->m_nOutsideEdge;

	m_nOwnerPartID = pschpin->m_nOwnerPartID;
	m_nDisplayMode = pschpin->m_nDisplayMode;
	m_posParent = pschpin->m_posParent;

	m_nDiagram = pschpin->m_nDiagram;
	m_bSelection = pschpin->m_bSelection;
	m_nFlag = pschpin->m_nFlag;

	pDocument = pschpin->pDocument;
}

void COb_schpin::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;
	
	char szHelp[] = "移动引脚: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	pView->Invalidate();

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	
	
	COb_schpin apin;
	apin.Copy(this);
	apin.m_bNameVisible = FALSE;
	apin.m_bNumberVisible = FALSE;
	apin.m_nInside = 0;
	apin.m_nInsideEdge = 0;
	apin.m_nOutside = 0;
	apin.m_nOutsideEdge = 0;

	x0 = apin.m_fX;
	y0 = apin.m_fY;
	pView->m_fCurrentSnapX = x0;
	pView->m_fCurrentSnapY = y0;
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
			apin.Draw(pDC, pV);
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
				apin.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX-x0, m_fY-y0, flag*90, FALSE);
			apin.m_fX = fx + x0 + deltax;
			apin.m_fY = fy + y0 + deltay;
			
			pView->CloseMouseCursor();
			apin.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			apin.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX-x0, m_fY-y0, flag*90, FALSE);
			apin.m_fX = fx + x0 + deltax;
			apin.m_fY = fy + y0 + deltay;

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_PIN;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->m_fX1 = m_fX;
			pundo->m_fY1 = m_fY;
			pundo->m_nOrientation = m_nOrientation;

			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fX = apin.m_fX;
			m_fY = apin.m_fY;
			m_nOrientation = apin.m_nOrientation;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			apin.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX-x0, m_fY-y0, flag*90, FALSE);
			apin.m_fX = fx + x0 + deltax;
			apin.m_fY = fy + y0 + deltay;
			int orientation;
			pDoc->Get_Rotate_Orientation(&orientation, m_nOrientation, flag*90, FALSE);
			apin.m_nOrientation = orientation;
			apin.m_nOrientation = pDoc->Get_Correct_Orientation(apin.m_nOrientation);

			apin.Draw(pDC, pV);

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

void COb_schpin::DrawHighlight(CDC* pDC, CView* pV, COLORREF color)
{
	CESDView* pView = (CESDView*)pV;

	if(m_bHide == TRUE) return;

	COb_schline schline;
	schline.m_nDiagram = m_nDiagram;
	schline.m_nAttrib = LINE_WIRE;
	schline.m_nColor = color;
	schline.m_nLineType = LINE_SOLID;
	schline.m_nLineWidth = LINE_LARGE;
	schline.m_cVertex.SetSize(2);

	schline.m_cVertex[0].fx = m_fX;
	schline.m_cVertex[0].fy = m_fY;
	if(m_nOrientation == 0)
	{
		schline.m_cVertex[1].fx = m_fX + m_fPinLength;
		schline.m_cVertex[1].fy = m_fY;
	}
	else if(m_nOrientation == 90)
	{
		schline.m_cVertex[1].fx = m_fX;
		schline.m_cVertex[1].fy = m_fY + m_fPinLength;
	}
	else if(m_nOrientation == 180)
	{
		schline.m_cVertex[1].fx = m_fX - m_fPinLength;
		schline.m_cVertex[1].fy = m_fY;
	}
	else if(m_nOrientation == 270)
	{
		schline.m_cVertex[1].fx = m_fX;
		schline.m_cVertex[1].fy = m_fY - m_fPinLength;
	}
	if(schline.PickMe(pV) == TRUE)
	{
		schline.Draw(pDC, pV);

		//Hot Point
		if((pView->m_fScale < 10)&&(nEDA_Print_Flag == 0))
		{
			CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
			CPen apen;
			apen.CreatePen(PS_SOLID, 1, pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor);
			CPen* oldpen = pDC->SelectObject(&apen);
			int x, y, x0, y0;
			pView->Get_VPort_XY(&x, &y, schline.m_cVertex[1].fx, schline.m_cVertex[1].fy);
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