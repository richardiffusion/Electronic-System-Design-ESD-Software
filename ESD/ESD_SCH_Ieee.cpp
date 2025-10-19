#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditIeee.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_schieee, CObject, 0)
COb_schieee::COb_schieee()
{
	// This empty constructor should be used by serialization only
	m_nFlag = 0 ;
	m_bSelection = FALSE;
	m_nOwnerPartID = 0;
	m_nDisplayMode = 0;
	m_posParent = NULL;

	pDocument = NULL;
}


void COb_schieee::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fX;
		ar << m_fY;
		ar << m_fSize;
		ar << m_nOrientation;
		ar << m_nOwnerPartID;
		ar << m_nDisplayMode;

		ar << m_nWidth;
		ar << m_bMirrored;
		ar << m_nIeeeSymbol;
		
		ar << m_nColor;
		
		ar << m_nDiagram;
		ar << m_bSelection;
	}
	else
	{
		ar >> m_fX;
		ar >> m_fY;
		ar >> m_fSize;
		ar >> m_nOrientation;
		ar >> m_nOwnerPartID;
		ar >> m_nDisplayMode;

		ar >> m_nWidth;
		ar >> m_bMirrored;
		ar >> m_nIeeeSymbol;
		
		ar >> m_nColor;
		
		ar >> m_nDiagram;
		ar >> m_bSelection;		
	}
}

void COb_schieee::Draw(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;

	COb_schline schline;
	schline.m_nDiagram = m_nDiagram;
	schline.m_nAttrib = LINE_WIRE;
	schline.m_nColor = m_nColor;
	schline.m_nLineType = LINE_SOLID;
	schline.m_nLineWidth = m_nWidth;

	COb_schellipsefill schef;
	schef.m_nDiagram = m_nDiagram;
	schef.m_nBorderWidth = m_nWidth;
	schef.m_bFillSolid = TRUE;
	schef.m_bTransparent = FALSE;
	schef.m_nBorderColor = m_nColor;
	schef.m_nFillColor = RGB(255,255,255);

	COb_schellipse schellipse;
	schellipse.m_nDiagram = m_nDiagram;
	schellipse.m_nColor = m_nColor;
	schellipse.m_nWidth = m_nWidth;

	COb_scharc scharc;
	scharc.m_nDiagram = m_nDiagram;
	scharc.m_nColor = m_nColor;
	scharc.m_nWidth = m_nWidth;

	COb_schpolygon schpolygon;
	schpolygon.m_nDiagram = m_nDiagram;
	schpolygon.m_bFillSolid = TRUE;
	schpolygon.m_bTransparent = FALSE;
	schpolygon.m_nBorderColor = m_nColor;
	schpolygon.m_nBorderWidth = m_nWidth;
	schpolygon.m_nFillColor = RGB(255,255,255);

	if(m_nIeeeSymbol == 1)
	{
		schef.m_fCx = m_fX;
		schef.m_fCy = m_fY;
		schef.m_fRadius_x = schef.m_fRadius_y = m_fSize*3;

		if(schef.PickMe(pV) == TRUE) schef.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 2)
	{
		double fx,fy;
        schpolygon.m_cVertex.SetSize(3);
		pView->Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, 8, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[2].fy = m_fY + fy*m_fSize;
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 3)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(3);
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	
	}
	else if(m_nIeeeSymbol == 4)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(4);
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	
	}
	else if(m_nIeeeSymbol == 5)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 6, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 6, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

		pView->Get_Rotate_XY(&fx, &fy, 3, 3, m_nOrientation, m_bMirrored);
		scharc.m_fCx = m_fX + fx*m_fSize;
		scharc.m_fCy = m_fY + fy*m_fSize;	
		scharc.m_fRadius = m_fSize*3;
		pView->Get_Rotate_Angle(&fx, &fy, 0, 180, m_nOrientation, m_bMirrored);
		scharc.m_fSangle = fx;
		scharc.m_fEangle = fy;        
		if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 6)
	{

		double fx,fy;
        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 8, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 7)
	{

		double fx,fy;
        schline.m_cVertex.SetSize(4);
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 6, -3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 6, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 6, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 8)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(3);
		pView->Get_Rotate_XY(&fx, &fy, 8, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	
	}
	else if(m_nIeeeSymbol == 9)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(5);
		pView->Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 4, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 4, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 10)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(4);
		pView->Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 4, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	
	}
	else if(m_nIeeeSymbol == 11)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(4);
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);		
	}
	else if(m_nIeeeSymbol == 12)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(6);
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 16, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 16, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 24, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[5].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[5].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	
	}
	else if(m_nIeeeSymbol == 13)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(7);
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 4, 1, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 4, 7, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 16, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, 7, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, 1, m_nOrientation, m_bMirrored);
		schline.m_cVertex[5].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[5].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[6].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[6].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	
	}
	else if(m_nIeeeSymbol == 14)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, -2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 20, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 20, -2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 20, 2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 15)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(4);
		pView->Get_Rotate_XY(&fx, &fy, 0, -11, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 3, -11, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 3, 11, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 11, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	
	}
	else if(m_nIeeeSymbol == 16)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(7);
		pView->Get_Rotate_XY(&fx, &fy, 1, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 2, 1, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 2, 2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 3, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 2, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 2, 5, m_nOrientation, m_bMirrored);
		schline.m_cVertex[5].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[5].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 1, 6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[6].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[6].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	
	}
	else if(m_nIeeeSymbol == 17)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 18)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, -4, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 5, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, -2, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, -3, -5, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

		schline.m_cVertex.SetSize(4);
		pView->Get_Rotate_XY(&fx, &fy, 3, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 3, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 4, -3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, -6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	
	}
	else if(m_nIeeeSymbol == 19)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(3);
		pView->Get_Rotate_XY(&fx, &fy, 0, 6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, -6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, -8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, -2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 20)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(3);
		pView->Get_Rotate_XY(&fx, &fy, 12, 6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, -6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, -2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, -8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 21)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(7);
		pView->Get_Rotate_XY(&fx, &fy, 2, 2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 1, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, -3, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, -2, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, -4, -4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 1, -4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[5].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[5].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 2, -3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[6].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[6].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	
	}
	else if(m_nIeeeSymbol == 22)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(5);
		pView->Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 4, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 4, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 23)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(5);
		pView->Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 4, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 4, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 24)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(5);
		pView->Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 4, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 4, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	

		schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 8, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 25)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, -2, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 3, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 5, 2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, -1, -1, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 2, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 1, -1, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 4, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 26)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(4);
		pView->Get_Rotate_XY(&fx, &fy, 40, 20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, -20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 40, -20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	

		pView->Get_Rotate_XY(&fx, &fy, 40, 0, m_nOrientation, m_bMirrored);
		scharc.m_fCx = m_fX + fx*m_fSize;
		scharc.m_fCy = m_fY + fy*m_fSize;
		scharc.m_fRadius = m_fSize*20;
		pView->Get_Rotate_Angle(&fx, &fy, 270, 90, m_nOrientation, m_bMirrored);
		scharc.m_fSangle = fx;
		scharc.m_fEangle = fy;        
		if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 27)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(4);
		pView->Get_Rotate_XY(&fx, &fy, 40, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, -20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 40, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	
	}
	else if(m_nIeeeSymbol == 28)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 6, 10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, -10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 6, -10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 25, 20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, -20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 25, -20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

		pView->Get_Rotate_XY(&fx, &fy, -20, 0, m_nOrientation, m_bMirrored);
		scharc.m_fCx = m_fX + fx*m_fSize;
		scharc.m_fCy = m_fY + fy*m_fSize;
		scharc.m_fRadius = m_fSize*28;
		pView->Get_Rotate_Angle(&fx, &fy, 315, 45, m_nOrientation, m_bMirrored);
		scharc.m_fSangle = fx;
		scharc.m_fEangle = fy;        
		if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);

		pView->Get_Rotate_XY(&fx, &fy, 25, -5, m_nOrientation, m_bMirrored);
		schellipse.m_fCx = m_fX + fx*m_fSize;
		schellipse.m_fCy = m_fY + fy*m_fSize;
		if((m_nOrientation == 90)||(m_nOrientation == 270))
		{
			schellipse.m_fRadius_y = m_fSize*40;
			schellipse.m_fRadius_x = m_fSize*25;
		}
		else
		{
			schellipse.m_fRadius_x = m_fSize*40;
			schellipse.m_fRadius_y = m_fSize*25;
		}
		pView->Get_Rotate_Angle(&fx, &fy, 7.4f, 90, m_nOrientation, m_bMirrored);
		schellipse.m_fSangle = fx;
		schellipse.m_fEangle = fy;        
		if(schellipse.PickMe(pV) == TRUE) schellipse.Draw(pDC, pV);

		pView->Get_Rotate_XY(&fx, &fy, 25, 5, m_nOrientation, m_bMirrored);
		schellipse.m_fCx = m_fX + fx*m_fSize;
		schellipse.m_fCy = m_fY + fy*m_fSize;
		if((m_nOrientation == 90)||(m_nOrientation == 270))
		{
			schellipse.m_fRadius_y = m_fSize*40;
			schellipse.m_fRadius_x = m_fSize*25;
		}
		else
		{
			schellipse.m_fRadius_x = m_fSize*40;
			schellipse.m_fRadius_y = m_fSize*25;
		}
		pView->Get_Rotate_Angle(&fx, &fy, 270, 352.6f, m_nOrientation, m_bMirrored);
		schellipse.m_fSangle = fx;
		schellipse.m_fEangle = fy;
		if(schellipse.PickMe(pV) == TRUE) schellipse.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 29)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 6, 10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, -10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 6, -10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 5, 20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 30, 20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 5, -20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 30, -20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

		pView->Get_Rotate_XY(&fx, &fy, -20, 0, m_nOrientation, m_bMirrored);
		scharc.m_fCx = m_fX + fx*m_fSize;
		scharc.m_fCy = m_fY + fy*m_fSize;
		scharc.m_fRadius = m_fSize*28;
		pView->Get_Rotate_Angle(&fx, &fy, 315, 45, m_nOrientation, m_bMirrored);
		scharc.m_fSangle = fx;
		scharc.m_fEangle = fy;        
		if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);

		pView->Get_Rotate_XY(&fx, &fy, -15, 0, m_nOrientation, m_bMirrored);
		scharc.m_fCx = m_fX + fx*m_fSize;
		scharc.m_fCy = m_fY + fy*m_fSize;
		scharc.m_fRadius = m_fSize*28;
		pView->Get_Rotate_Angle(&fx, &fy, 315, 45, m_nOrientation, m_bMirrored);
		scharc.m_fSangle = fx;
		scharc.m_fEangle = fy;        
		if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);

		pView->Get_Rotate_XY(&fx, &fy, 30, -5, m_nOrientation, m_bMirrored);
		schellipse.m_fCx = m_fX + fx*m_fSize;
		schellipse.m_fCy = m_fY + fy*m_fSize;
		if((m_nOrientation == 90)||(m_nOrientation == 270))
		{
			schellipse.m_fRadius_y = m_fSize*40;
			schellipse.m_fRadius_x = m_fSize*25;
		}
		else
		{
			schellipse.m_fRadius_x = m_fSize*40;
			schellipse.m_fRadius_y = m_fSize*25;
		}
		pView->Get_Rotate_Angle(&fx, &fy, 7.4f, 90, m_nOrientation, m_bMirrored);
		schellipse.m_fSangle = fx;
		schellipse.m_fEangle = fy;        
		if(schellipse.PickMe(pV) == TRUE) schellipse.Draw(pDC, pV);

		pView->Get_Rotate_XY(&fx, &fy, 30, 5, m_nOrientation, m_bMirrored);
		schellipse.m_fCx = m_fX + fx*m_fSize;
		schellipse.m_fCy = m_fY + fy*m_fSize;
		if((m_nOrientation == 90)||(m_nOrientation == 270))
		{
			schellipse.m_fRadius_y = m_fSize*40;
			schellipse.m_fRadius_x = m_fSize*25;
		}
		else
		{
			schellipse.m_fRadius_x = m_fSize*40;
			schellipse.m_fRadius_y = m_fSize*25;
		}
		pView->Get_Rotate_Angle(&fx, &fy, 270, 352.6f, m_nOrientation, m_bMirrored);
		schellipse.m_fSangle = fx;
		schellipse.m_fEangle = fy;
		if(schellipse.PickMe(pV) == TRUE) schellipse.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 30)
	{

		double fx,fy;
        schline.m_cVertex.SetSize(4);
		pView->Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 6, -3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 6, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	

        schline.m_cVertex.SetSize(2);
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 6, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 31)
	{

		double fx,fy;
        schpolygon.m_cVertex.SetSize(3);
		pView->Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 12, 8, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[2].fy = m_fY + fy*m_fSize;
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);

        schpolygon.m_cVertex.SetSize(3);
		pView->Get_Rotate_XY(&fx, &fy, 14, 0, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 14, 8, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 26, 4, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[2].fy = m_fY + fy*m_fSize;
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);
	}
	else if(m_nIeeeSymbol == 32)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(5);
		pView->Get_Rotate_XY(&fx, &fy, 2, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, 2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, -2, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 0, -2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 2, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);	
	}
	else if(m_nIeeeSymbol == 33)
	{
		double fx,fy;
        schpolygon.m_cVertex.SetSize(3);
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, -6, 2, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, -6, -2, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[2].fy = m_fY + fy*m_fSize;
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);		
	}
	else if(m_nIeeeSymbol == 34)
	{
		double fx,fy;
        schpolygon.m_cVertex.SetSize(3);
		pView->Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 6, 2, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 6, -2, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[2].fy = m_fY + fy*m_fSize;
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);	

        schpolygon.m_cVertex.SetSize(3);
		pView->Get_Rotate_XY(&fx, &fy, 13, 0, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[0].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 7, 2, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[1].fy = m_fY + fy*m_fSize;
		pView->Get_Rotate_XY(&fx, &fy, 7, -2, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[2].fy = m_fY + fy*m_fSize;
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);	
	}
}



void COb_schieee::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	COb_schline schline;
	schline.m_nDiagram = m_nDiagram;
	schline.m_nAttrib = LINE_WIRE;
	schline.m_nColor = m_nColor;
	schline.m_nLineType = LINE_SOLID;
	schline.m_nLineWidth = m_nWidth;

	COb_schellipsefill schef;
	schef.m_nDiagram = m_nDiagram;
	schef.m_nBorderWidth = m_nWidth;
	schef.m_bFillSolid = TRUE;
	schef.m_bTransparent = FALSE;
	schef.m_nBorderColor = m_nColor;
	schef.m_nFillColor = RGB(255,255,255);

	COb_schellipse schellipse;
	schellipse.m_nDiagram = m_nDiagram;
	schellipse.m_nColor = m_nColor;
	schellipse.m_nWidth = m_nWidth;

	COb_scharc scharc;
	scharc.m_nDiagram = m_nDiagram;
	scharc.m_nColor = m_nColor;
	scharc.m_nWidth = m_nWidth;

	COb_schpolygon schpolygon;
	schpolygon.m_nDiagram = m_nDiagram;
	schpolygon.m_bFillSolid = TRUE;
	schpolygon.m_bTransparent = FALSE;
	schpolygon.m_nBorderColor = m_nColor;
	schpolygon.m_nBorderWidth = m_nWidth;
	schpolygon.m_nFillColor = RGB(255,255,255);

	if(m_nIeeeSymbol == 1)
	{
		schef.m_fCx = m_fX;
		schef.m_fCy = m_fY;
		schef.m_fRadius_x = schef.m_fRadius_y = m_fSize*3;

		schef.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 2)
	{
		double fx,fy;
        schpolygon.m_cVertex.SetSize(3);
		Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, 8, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[2].fy = m_fY + fy*m_fSize;
		schpolygon.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 3)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(3);
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 4)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(4);
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 5)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 6, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 6, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

		Get_Rotate_XY(&fx, &fy, 3, 3, m_nOrientation, m_bMirrored);
		scharc.m_fCx = m_fX + fx*m_fSize;
		scharc.m_fCy = m_fY + fy*m_fSize;	
		scharc.m_fRadius = m_fSize*3;
		Get_Rotate_Angle(&fx, &fy, 0, 180, m_nOrientation, m_bMirrored);
		scharc.m_fSangle = fx;
		scharc.m_fEangle = fy;        
		scharc.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 6)
	{

		double fx,fy;
        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 8, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 7)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(4);
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 6, -3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 6, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 6, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 8)
	{

		double fx,fy;
        schline.m_cVertex.SetSize(3);
		Get_Rotate_XY(&fx, &fy, 8, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 9)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(5);
		Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 4, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 4, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 10)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(4);
		Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 4, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 11)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(4);
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);	
	}
	else if(m_nIeeeSymbol == 12)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(6);
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 16, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 16, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 24, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[5].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[5].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 13)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(7);
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 4, 1, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 4, 7, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 16, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, 7, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, 1, m_nOrientation, m_bMirrored);
		schline.m_cVertex[5].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[5].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[6].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[6].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 14)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, -2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 20, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 20, -2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 20, 2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 15)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(4);
		Get_Rotate_XY(&fx, &fy, 0, -11, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 3, -11, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 3, 11, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 11, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 16)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(7);
		Get_Rotate_XY(&fx, &fy, 1, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 2, 1, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 2, 2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 3, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 2, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 2, 5, m_nOrientation, m_bMirrored);
		schline.m_cVertex[5].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[5].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 1, 6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[6].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[6].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 17)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 18)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, -4, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 5, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, -2, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, -3, -5, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(4);
		Get_Rotate_XY(&fx, &fy, 3, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 3, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 4, -3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, -6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 19)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(3);
		Get_Rotate_XY(&fx, &fy, 0, 6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, -6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);	

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, -8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, -2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 20)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(3);
		Get_Rotate_XY(&fx, &fy, 12, 6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, -6, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);	

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, -2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, -8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 21)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(7);
		Get_Rotate_XY(&fx, &fy, 2, 2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 1, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, -3, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, -2, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, -4, -4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 1, -4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[5].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[5].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 2, -3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[6].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[6].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 22)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(5);
		Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 4, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 4, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);	

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 23)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(5);
		Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 4, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 4, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 24)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(5);
		Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 4, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 4, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 8, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 8, 4, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 25)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, -2, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 3, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 5, 2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, -1, -1, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 2, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 1, -1, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 4, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 26)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(4);
		Get_Rotate_XY(&fx, &fy, 40, 20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, -20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 40, -20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

		Get_Rotate_XY(&fx, &fy, 40, 0, m_nOrientation, m_bMirrored);
		scharc.m_fCx = m_fX + fx*m_fSize;
		scharc.m_fCy = m_fY + fy*m_fSize;
		scharc.m_fRadius = m_fSize*20;
		Get_Rotate_Angle(&fx, &fy, 270, 90, m_nOrientation, m_bMirrored);
		scharc.m_fSangle = fx;
		scharc.m_fEangle = fy;        
		scharc.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 27)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(4);
		Get_Rotate_XY(&fx, &fy, 40, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, -20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 40, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 28)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 6, 10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, -10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 6, -10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 25, 20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, -20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 25, -20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

		Get_Rotate_XY(&fx, &fy, -20, 0, m_nOrientation, m_bMirrored);
		scharc.m_fCx = m_fX + fx*m_fSize;
		scharc.m_fCy = m_fY + fy*m_fSize;
		scharc.m_fRadius = m_fSize*28;
		Get_Rotate_Angle(&fx, &fy, 315, 45, m_nOrientation, m_bMirrored);
		scharc.m_fSangle = fx;
		scharc.m_fEangle = fy;        
		scharc.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

		Get_Rotate_XY(&fx, &fy, 25, -5, m_nOrientation, m_bMirrored);
		schellipse.m_fCx = m_fX + fx*m_fSize;
		schellipse.m_fCy = m_fY + fy*m_fSize;
		if((m_nOrientation == 90)||(m_nOrientation == 270))
		{
			schellipse.m_fRadius_y = m_fSize*40;
			schellipse.m_fRadius_x = m_fSize*25;
		}
		else
		{
			schellipse.m_fRadius_x = m_fSize*40;
			schellipse.m_fRadius_y = m_fSize*25;
		}
		Get_Rotate_Angle(&fx, &fy, 7.4f, 90, m_nOrientation, m_bMirrored);
		schellipse.m_fSangle = fx;
		schellipse.m_fEangle = fy;        
		schellipse.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

		Get_Rotate_XY(&fx, &fy, 25, 5, m_nOrientation, m_bMirrored);
		schellipse.m_fCx = m_fX + fx*m_fSize;
		schellipse.m_fCy = m_fY + fy*m_fSize;
		if((m_nOrientation == 90)||(m_nOrientation == 270))
		{
			schellipse.m_fRadius_y = m_fSize*40;
			schellipse.m_fRadius_x = m_fSize*25;
		}
		else
		{
			schellipse.m_fRadius_x = m_fSize*40;
			schellipse.m_fRadius_y = m_fSize*25;
		}
		Get_Rotate_Angle(&fx, &fy, 270, 352.6f, m_nOrientation, m_bMirrored);
		schellipse.m_fSangle = fx;
		schellipse.m_fEangle = fy;
		schellipse.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 29)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 6, 10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, -10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 6, -10, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 5, 20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 30, 20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 5, -20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 30, -20, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

		Get_Rotate_XY(&fx, &fy, -20, 0, m_nOrientation, m_bMirrored);
		scharc.m_fCx = m_fX + fx*m_fSize;
		scharc.m_fCy = m_fY + fy*m_fSize;
		scharc.m_fRadius = m_fSize*28;
		Get_Rotate_Angle(&fx, &fy, 315, 45, m_nOrientation, m_bMirrored);
		scharc.m_fSangle = fx;
		scharc.m_fEangle = fy;        
		scharc.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

		Get_Rotate_XY(&fx, &fy, -15, 0, m_nOrientation, m_bMirrored);
		scharc.m_fCx = m_fX + fx*m_fSize;
		scharc.m_fCy = m_fY + fy*m_fSize;
		scharc.m_fRadius = m_fSize*28;
		Get_Rotate_Angle(&fx, &fy, 315, 45, m_nOrientation, m_bMirrored);
		scharc.m_fSangle = fx;
		scharc.m_fEangle = fy;        
		scharc.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

		Get_Rotate_XY(&fx, &fy, 30, -5, m_nOrientation, m_bMirrored);
		schellipse.m_fCx = m_fX + fx*m_fSize;
		schellipse.m_fCy = m_fY + fy*m_fSize;
		if((m_nOrientation == 90)||(m_nOrientation == 270))
		{
			schellipse.m_fRadius_y = m_fSize*40;
			schellipse.m_fRadius_x = m_fSize*25;
		}
		else
		{
			schellipse.m_fRadius_x = m_fSize*40;
			schellipse.m_fRadius_y = m_fSize*25;
		}
		Get_Rotate_Angle(&fx, &fy, 7.4f, 90, m_nOrientation, m_bMirrored);
		schellipse.m_fSangle = fx;
		schellipse.m_fEangle = fy;        
		schellipse.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

		Get_Rotate_XY(&fx, &fy, 30, 5, m_nOrientation, m_bMirrored);
		schellipse.m_fCx = m_fX + fx*m_fSize;
		schellipse.m_fCy = m_fY + fy*m_fSize;
		if((m_nOrientation == 90)||(m_nOrientation == 270))
		{
			schellipse.m_fRadius_y = m_fSize*40;
			schellipse.m_fRadius_x = m_fSize*25;
		}
		else
		{
			schellipse.m_fRadius_x = m_fSize*40;
			schellipse.m_fRadius_y = m_fSize*25;
		}
		Get_Rotate_Angle(&fx, &fy, 270, 352.6f, m_nOrientation, m_bMirrored);
		schellipse.m_fSangle = fx;
		schellipse.m_fEangle = fy;
		schellipse.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 30)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(4);
		Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 6, -3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 6, 3, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schline.m_cVertex.SetSize(2);
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 6, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 31)
	{
		double fx,fy;
        schpolygon.m_cVertex.SetSize(3);
		Get_Rotate_XY(&fx, &fy, 0, 4, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, 0, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 12, 8, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[2].fy = m_fY + fy*m_fSize;
		schpolygon.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schpolygon.m_cVertex.SetSize(3);
		Get_Rotate_XY(&fx, &fy, 14, 0, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 14, 8, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 26, 4, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[2].fy = m_fY + fy*m_fSize;
		schpolygon.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 32)
	{
		double fx,fy;
        schline.m_cVertex.SetSize(5);
		Get_Rotate_XY(&fx, &fy, 2, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, 2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, -2, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[2].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 0, -2, m_nOrientation, m_bMirrored);
		schline.m_cVertex[3].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[3].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 2, 0, m_nOrientation, m_bMirrored);
		schline.m_cVertex[4].fx = m_fX + fx*m_fSize;
		schline.m_cVertex[4].fy = m_fY + fy*m_fSize;
		schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 33)
	{
		double fx,fy;
        schpolygon.m_cVertex.SetSize(3);
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, -6, 2, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, -6, -2, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[2].fy = m_fY + fy*m_fSize;
		schpolygon.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
	else if(m_nIeeeSymbol == 34)
	{
		double fx,fy;
        schpolygon.m_cVertex.SetSize(3);
		Get_Rotate_XY(&fx, &fy, 0, 0, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 6, 2, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 6, -2, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[2].fy = m_fY + fy*m_fSize;
		schpolygon.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);

        schpolygon.m_cVertex.SetSize(3);
		Get_Rotate_XY(&fx, &fy, 13, 0, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[0].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[0].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 7, 2, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[1].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[1].fy = m_fY + fy*m_fSize;
		Get_Rotate_XY(&fx, &fy, 7, -2, m_nOrientation, m_bMirrored);
		schpolygon.m_cVertex[2].fx = m_fX + fx*m_fSize;
		schpolygon.m_cVertex[2].fy = m_fY + fy*m_fSize;
		schpolygon.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
	}
}

void COb_schieee::Change(void)
{
	CDlg_EditIeee dlg;
	dlg.DoModal();
}

void COb_schieee::Copy(COb_schieee* pschieee)
{
	m_fX = pschieee->m_fX;
	m_fY = pschieee->m_fY;
	m_fSize = pschieee->m_fSize;
	m_nOrientation = pschieee->m_nOrientation;
	m_nOwnerPartID = pschieee->m_nOwnerPartID;
	m_nDisplayMode = pschieee->m_nDisplayMode;

	m_nWidth = pschieee->m_nWidth;
	m_bMirrored = pschieee->m_bMirrored;
	m_nIeeeSymbol = pschieee->m_nIeeeSymbol;
	
	m_nColor = pschieee->m_nColor;

	m_nDiagram = pschieee->m_nDiagram;
	m_bSelection = pschieee->m_bSelection;
	m_nFlag = pschieee->m_nFlag;

	m_posParent = pschieee->m_posParent;

	pDocument = pschieee->pDocument;
}

void COb_schieee::Get_Rotate_XY(double* fx, double* fy, double old_fx, double old_fy, int orientation, BOOL mirrored)
{
	int state;
	state = (mirrored == TRUE? 1 : 0)*4 + orientation/90;
	*fx = fEDA_Rotate_XY[state][0]*old_fx + fEDA_Rotate_XY[state][1]*old_fy;
	*fy = fEDA_Rotate_XY[state][2]*old_fx + fEDA_Rotate_XY[state][3]*old_fy;
}

void COb_schieee::Get_Rotate_Angle(double* sangle, double* eangle, double old_sangle, double old_eangle, int orientation, BOOL mirrored)
{
	double sa, ea;

	if(mirrored == FALSE)
	{
		sa = old_sangle + orientation;
		if(sa >= 360) sa -= 360;
		ea = old_eangle + orientation;
		if(ea >= 360) ea -= 360;
	}
	else
	{
		sa = old_eangle + orientation;
		if(sa >= 360) sa -= 360;
		ea = old_sangle + orientation;
		if(ea >= 360) ea -= 360;
		sa = 180 - sa;
		if(sa < 0) sa += 360;
		ea = 180 - ea;
		if(ea < 0) ea += 360;
	}
	*sangle = sa;
	*eangle = ea;
}
