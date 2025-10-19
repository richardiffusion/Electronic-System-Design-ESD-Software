#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditPower.h"
#include "Dlg_EditOffSheet.h"
#include "Dlg_Global_Power.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_schpower, CObject, 0)
COb_schpower::COb_schpower()
{
	// This empty constructor should be used by serialization only
	m_nFlag = 0 ;
	m_bSelection = FALSE;

	pDocument = NULL;
}


void COb_schpower::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fX;
		ar << m_fY;
		ar << m_nStyle;
		ar << m_nOrientation;
	
		ar << m_bShowNetName;
		ar << m_bIsCrossSheetConnector;
		ar << m_csNetName;
	
		ar << m_nColor;
		ar << m_nDiagram;
		ar << m_bSelection;
	}
	else
	{
		ar >> m_fX;
		ar >> m_fY;
		ar >> m_nStyle;
		ar >> m_nOrientation;
	
		ar >> m_bShowNetName;
		ar >> m_bIsCrossSheetConnector;
		ar >> m_csNetName;
	
		ar >> m_nColor;
		ar >> m_nDiagram;
		ar >> m_bSelection;	
	}
}

void COb_schpower::Draw(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;

	COb_schline schline;
	schline.m_nFlag = m_nFlag;
	schline.m_nDiagram = m_nDiagram;
	schline.m_nAttrib = LINE_WIRE;
	schline.m_nColor = m_nColor;
	schline.m_nLineType = LINE_SOLID;
	schline.m_nLineWidth = LINE_SMALL;
	schline.m_cVertex.SetSize(2);

	COb_scharc scharc;
	scharc.m_nFlag = m_nFlag;
	scharc.m_nDiagram = m_nDiagram;
	scharc.m_nColor = m_nColor;
	scharc.m_nWidth = LINE_SMALLEST;

	COb_schtext schtext;
	schtext.m_nFlag = m_nFlag;
	schtext.m_nDiagram = m_nDiagram;
	schtext.m_nColor = m_nColor;
	schtext.m_nFont = 0;
	schtext.m_nOrientation = 0;
	schtext.m_csText = m_csNetName;
	schtext.m_nID = TEXT_TEXT;
	
	schtext.m_fSx = schtext.m_fSy = 0;
	//int offset = schtext.GetExtent(pDC, pV);

	double fx, fy;
	
	if(m_nOrientation == 0)
	{
		schtext.m_nH_Justification = 0;
		if(m_bIsCrossSheetConnector == TRUE)
		{
			if(m_nStyle == 0)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 20;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 20;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 70;
				schline.m_cVertex[1].fy = m_fY + 50;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 20;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 70;
				schline.m_cVertex[1].fy = m_fY - 50;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 70;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 120;
				schline.m_cVertex[1].fy = m_fY - 50; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 70;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 120;
				schline.m_cVertex[1].fy = m_fY + 50; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX + 140;
				fy = m_fY - 50;
			}
			else
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 70;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 70;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 20;
				schline.m_cVertex[1].fy = m_fY + 50;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 70;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 20;
				schline.m_cVertex[1].fy = m_fY - 50;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 120;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 70;
				schline.m_cVertex[1].fy = m_fY - 50; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 120;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 70;
				schline.m_cVertex[1].fy = m_fY + 50; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX + 140;
				fy = m_fY - 50;
			}
		}
		else
		{
			if(m_nStyle == POWER_STYLE_CIRCLE)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX+40;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				scharc.m_fCx = m_fX + 70;
				scharc.m_fCy = m_fY;
				scharc.m_fRadius = 30;
				scharc.m_fSangle = 0;
				scharc.m_fEangle = 360;
				if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);

				fx = m_fX + 120;
				fy = m_fY - 50;
			}
			else if(m_nStyle == POWER_STYLE_ARROW)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 40;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 40;
				schline.m_cVertex[0].fy = m_fY - 30;
				schline.m_cVertex[1].fx = m_fX + 40;
				schline.m_cVertex[1].fy = m_fY + 30;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 40;
				schline.m_cVertex[0].fy = m_fY - 30;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 40;
				schline.m_cVertex[0].fy = m_fY + 30;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);		
				
				fx = m_fX + 120;
				fy = m_fY - 50;
			}
			else if(m_nStyle == POWER_STYLE_BAR)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 100;
				schline.m_cVertex[0].fy = m_fY - 50;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY + 50;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				
				fx = m_fX + 120;
				fy = m_fY - 50;
			}
			else if(m_nStyle == POWER_STYLE_WAVE)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 60;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				scharc.m_fCx = m_fX + 100;
				scharc.m_fCy = m_fY;
				scharc.m_fRadius = 40;
				scharc.m_fSangle = 180;
				scharc.m_fEangle = 270;
				if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);
				scharc.m_fCx = m_fX + 20;
				scharc.m_fCy = m_fY;
				scharc.m_fRadius = 40;
				scharc.m_fSangle = 0;
				scharc.m_fEangle = 90;
				if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);

				fx = m_fX + 120;
				fy = m_fY - 50;
			}
			else if(m_nStyle == POWER_STYLE_POWERGROUND)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 130;
				schline.m_cVertex[0].fy = m_fY - 70;
				schline.m_cVertex[1].fx = m_fX + 130;
				schline.m_cVertex[1].fy = m_fY + 70;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 160;
				schline.m_cVertex[0].fy = m_fY - 40;
				schline.m_cVertex[1].fx = m_fX + 160;
				schline.m_cVertex[1].fy = m_fY + 40; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 190;
				schline.m_cVertex[0].fy = m_fY - 10;
				schline.m_cVertex[1].fx = m_fX + 190;
				schline.m_cVertex[1].fy = m_fY + 10; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX + 220;
				fy = m_fY - 50;
			}
			else if(m_nStyle == POWER_STYLE_SIGNALGROUND)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX + 200;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 100;
				schline.m_cVertex[0].fy = m_fY + 100;
				schline.m_cVertex[1].fx = m_fX + 200;
				schline.m_cVertex[1].fy = m_fY; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX + 220;
				fy = m_fY - 50;
			}
			else if(m_nStyle == POWER_STYLE_EARTH)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX + 200;
				schline.m_cVertex[1].fy = m_fY - 150;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 100;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX + 200;
				schline.m_cVertex[1].fy = m_fY - 50; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 100;
				schline.m_cVertex[0].fy = m_fY + 100;
				schline.m_cVertex[1].fx = m_fX + 200;
				schline.m_cVertex[1].fy = m_fY + 50; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX + 220;
				fy = m_fY - 50;
			}
		}
	}
	else if(m_nOrientation == 90)
	{
		schtext.m_nH_Justification = 1;
		if(m_bIsCrossSheetConnector == TRUE)
		{
			if(m_nStyle == 0)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY + 20;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY + 20;
				schline.m_cVertex[1].fx = m_fX + 50;
				schline.m_cVertex[1].fy = m_fY + 70;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY + 20;
				schline.m_cVertex[1].fx = m_fX - 50;
				schline.m_cVertex[1].fy = m_fY + 70;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY + 70;
				schline.m_cVertex[1].fx = m_fX - 50; 
				schline.m_cVertex[1].fy = m_fY + 120;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY + 70;
				schline.m_cVertex[1].fx = m_fX + 50; 
				schline.m_cVertex[1].fy = m_fY + 120;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX; // - (double)offset*pView->m_fScale/2;
				fy = m_fY + 140 - 25;
			}
			else
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY + 70;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY + 70;
				schline.m_cVertex[1].fx = m_fX + 50;
				schline.m_cVertex[1].fy = m_fY + 20;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY + 70;
				schline.m_cVertex[1].fx = m_fX - 50;
				schline.m_cVertex[1].fy = m_fY + 20;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY + 120;
				schline.m_cVertex[1].fx = m_fX - 50; 
				schline.m_cVertex[1].fy = m_fY + 70;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY + 120;
				schline.m_cVertex[1].fx = m_fX + 50; 
				schline.m_cVertex[1].fy = m_fY + 70;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX; // - (double)offset*pView->m_fScale/2;
				fy = m_fY + 140 - 25;
			}
		}
		else
		{
			if(m_nStyle == POWER_STYLE_CIRCLE)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY+40;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				scharc.m_fCx = m_fX;
				scharc.m_fCy = m_fY + 70;
				scharc.m_fRadius = 30;
				scharc.m_fSangle = 0;
				scharc.m_fEangle = 360;
				if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);

				fx = m_fX; // - (double)offset*pView->m_fScale/2;
				fy = m_fY + 120 - 25;
			}
			else if(m_nStyle == POWER_STYLE_ARROW)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY + 40;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 30;
				schline.m_cVertex[0].fy = m_fY + 40;
				schline.m_cVertex[1].fx = m_fX + 30;
				schline.m_cVertex[1].fy = m_fY + 40;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 30;
				schline.m_cVertex[0].fy = m_fY + 40;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 30;
				schline.m_cVertex[0].fy = m_fY + 40;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);		
				
				fx = m_fX; // - (double)offset*pView->m_fScale/2;
				fy = m_fY + 120 - 25;
			}
			else if(m_nStyle == POWER_STYLE_BAR)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 50;
				schline.m_cVertex[0].fy = m_fY + 100;
				schline.m_cVertex[1].fx = m_fX + 50;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				
				fx = m_fX; // - (double)offset*pView->m_fScale/2;
				fy = m_fY + 120 - 25;
			}
			else if(m_nStyle == POWER_STYLE_WAVE)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY + 60;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				scharc.m_fCx = m_fX;
				scharc.m_fCy = m_fY + 100;
				scharc.m_fRadius = 40;
				scharc.m_fSangle = 270;
				scharc.m_fEangle = 360;
				if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);
				scharc.m_fCx = m_fX;
				scharc.m_fCy = m_fY + 20;
				scharc.m_fRadius = 40;
				scharc.m_fSangle = 90;
				scharc.m_fEangle = 180;
				if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);

				fx = m_fX; // - (double)offset*pView->m_fScale/2;
				fy = m_fY + 120 - 25;
			}
			else if(m_nStyle == POWER_STYLE_POWERGROUND)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY + 100;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 70;
				schline.m_cVertex[0].fy = m_fY + 130;
				schline.m_cVertex[1].fx = m_fX + 70;
				schline.m_cVertex[1].fy = m_fY + 130;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 40;
				schline.m_cVertex[0].fy = m_fY + 160;
				schline.m_cVertex[1].fx = m_fX + 40; 
				schline.m_cVertex[1].fy = m_fY + 160;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 10;
				schline.m_cVertex[0].fy = m_fY + 190;
				schline.m_cVertex[1].fx = m_fX + 10; 
				schline.m_cVertex[1].fy = m_fY + 190;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX; // - (double)offset*pView->m_fScale/2;
				fy = m_fY + 220 - 25;
			}
			else if(m_nStyle == POWER_STYLE_SIGNALGROUND)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY + 100;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY + 100;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY + 200;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 100;
				schline.m_cVertex[0].fy = m_fY + 100;
				schline.m_cVertex[1].fx = m_fX; 
				schline.m_cVertex[1].fy = m_fY + 200;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX;// - (double)offset*pView->m_fScale/2;
				fy = m_fY + 220 - 25;
			}
			else if(m_nStyle == POWER_STYLE_EARTH)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY + 100;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY + 100;
				schline.m_cVertex[1].fx = m_fX - 50;
				schline.m_cVertex[1].fy = m_fY + 200;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY + 100;
				schline.m_cVertex[1].fx = m_fX + 50; 
				schline.m_cVertex[1].fy = m_fY + 200;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 100;
				schline.m_cVertex[0].fy = m_fY + 100;
				schline.m_cVertex[1].fx = m_fX + 150; 
				schline.m_cVertex[1].fy = m_fY + 200;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX;// - (double)offset*pView->m_fScale/2;
				fy = m_fY + 220 - 25;
			}
		}

	}
	else if(m_nOrientation == 180)
	{
		schtext.m_nH_Justification = 2;
		if(m_bIsCrossSheetConnector == TRUE)
		{
			if(m_nStyle == 0)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 20;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 20;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 70;
				schline.m_cVertex[1].fy = m_fY + 50;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 20;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 70;
				schline.m_cVertex[1].fy = m_fY - 50;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 70;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 120;
				schline.m_cVertex[1].fy = m_fY - 50; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 70;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 120;
				schline.m_cVertex[1].fy = m_fY + 50; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX - 140; // - (double)offset*pView->m_fScale;
				fy = m_fY - 50;
			}
			else
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 70;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 70;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 20;
				schline.m_cVertex[1].fy = m_fY + 50;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 70;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 20;
				schline.m_cVertex[1].fy = m_fY - 50;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 120;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 70;
				schline.m_cVertex[1].fy = m_fY - 50; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 120;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 70;
				schline.m_cVertex[1].fy = m_fY + 50; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX - 140; // - (double)offset*pView->m_fScale;
				fy = m_fY - 50;
			}
		}
		else
		{
			if(m_nStyle == POWER_STYLE_CIRCLE)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX-40;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				scharc.m_fCx = m_fX - 70;
				scharc.m_fCy = m_fY;
				scharc.m_fRadius = 30;
				scharc.m_fSangle = 0;
				scharc.m_fEangle = 360;
				if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);

				fx = m_fX - 120; // - (double)offset*pView->m_fScale;
				fy = m_fY - 50;
			}
			else if(m_nStyle == POWER_STYLE_ARROW)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 40;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 40;
				schline.m_cVertex[0].fy = m_fY - 30;
				schline.m_cVertex[1].fx = m_fX - 40;
				schline.m_cVertex[1].fy = m_fY + 30;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 40;
				schline.m_cVertex[0].fy = m_fY - 30;
				schline.m_cVertex[1].fx = m_fX - 100;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 40;
				schline.m_cVertex[0].fy = m_fY + 30;
				schline.m_cVertex[1].fx = m_fX - 100;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);		
				
				fx = m_fX - 120; // - (double)offset*pView->m_fScale;
				fy = m_fY - 50;
			}
			else if(m_nStyle == POWER_STYLE_BAR)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 100;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY - 50;
				schline.m_cVertex[1].fx = m_fX - 100;
				schline.m_cVertex[1].fy = m_fY + 50;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				
				fx = m_fX - 120; // - (double)offset*pView->m_fScale;
				fy = m_fY - 50;
			}
			else if(m_nStyle == POWER_STYLE_WAVE)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 60;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				scharc.m_fCx = m_fX - 100;
				scharc.m_fCy = m_fY;
				scharc.m_fRadius = 40;
				scharc.m_fSangle = 0;
				scharc.m_fEangle = 90;
				if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);
				scharc.m_fCx = m_fX - 20;
				scharc.m_fCy = m_fY;
				scharc.m_fRadius = 40;
				scharc.m_fSangle = 180;
				scharc.m_fEangle = 270;
				if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);

				fx = m_fX - 120; // - (double)offset*pView->m_fScale;
				fy = m_fY - 50;
			}
			else if(m_nStyle == POWER_STYLE_POWERGROUND)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 100;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX - 100;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 130;
				schline.m_cVertex[0].fy = m_fY - 70;
				schline.m_cVertex[1].fx = m_fX - 130;
				schline.m_cVertex[1].fy = m_fY + 70;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 160;
				schline.m_cVertex[0].fy = m_fY - 40;
				schline.m_cVertex[1].fx = m_fX - 160;
				schline.m_cVertex[1].fy = m_fY + 40; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 190;
				schline.m_cVertex[0].fy = m_fY - 10;
				schline.m_cVertex[1].fx = m_fX - 190;
				schline.m_cVertex[1].fy = m_fY + 10; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX - 220; // - (double)offset*pView->m_fScale;
				fy = m_fY - 50;
			}
			else if(m_nStyle == POWER_STYLE_SIGNALGROUND)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 100;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX - 100;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX - 200;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY + 100;
				schline.m_cVertex[1].fx = m_fX - 200;
				schline.m_cVertex[1].fy = m_fY; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX - 220; // - (double)offset*pView->m_fScale;
				fy = m_fY - 50;
			}
			else if(m_nStyle == POWER_STYLE_EARTH)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 100;
				schline.m_cVertex[1].fy = m_fY;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX - 100;
				schline.m_cVertex[1].fy = m_fY + 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX - 200;
				schline.m_cVertex[1].fy = m_fY - 50;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX - 200;
				schline.m_cVertex[1].fy = m_fY + 50; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY + 100;
				schline.m_cVertex[1].fx = m_fX - 200;
				schline.m_cVertex[1].fy = m_fY + 150; 
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX - 220; // - (double)offset*pView->m_fScale;
				fy = m_fY - 50;
			}
		}
	}
	else if(m_nOrientation == 270)
	{
		schtext.m_nH_Justification = 1;
		if(m_bIsCrossSheetConnector == TRUE)
		{
			if(m_nStyle == 0)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY - 20;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY - 20;
				schline.m_cVertex[1].fx = m_fX + 50;
				schline.m_cVertex[1].fy = m_fY - 70;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY - 20;
				schline.m_cVertex[1].fx = m_fX - 50;
				schline.m_cVertex[1].fy = m_fY - 70;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY - 70;
				schline.m_cVertex[1].fx = m_fX - 50; 
				schline.m_cVertex[1].fy = m_fY - 120;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY - 70;
				schline.m_cVertex[1].fx = m_fX + 50; 
				schline.m_cVertex[1].fy = m_fY - 120;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX; // - (double)offset*pView->m_fScale/2;
				fy = m_fY - 140 - 85;
			}
			else
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY - 70;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY - 70;
				schline.m_cVertex[1].fx = m_fX + 50;
				schline.m_cVertex[1].fy = m_fY - 20;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY - 70;
				schline.m_cVertex[1].fx = m_fX - 50;
				schline.m_cVertex[1].fy = m_fY - 20;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY - 120;
				schline.m_cVertex[1].fx = m_fX - 50; 
				schline.m_cVertex[1].fy = m_fY - 70;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY - 120;
				schline.m_cVertex[1].fx = m_fX + 50; 
				schline.m_cVertex[1].fy = m_fY - 70;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX; // - (double)offset*pView->m_fScale/2;
				fy = m_fY - 140 - 85;
			}
		}
		else
		{
			if(m_nStyle == POWER_STYLE_CIRCLE)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY-40;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				scharc.m_fCx = m_fX;
				scharc.m_fCy = m_fY - 70;
				scharc.m_fRadius = 30;
				scharc.m_fSangle = 0;
				scharc.m_fEangle = 360;
				if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);

				fx = m_fX;// - (double)offset*pView->m_fScale/2;
				fy = m_fY - 120 - 85;
			}
			else if(m_nStyle == POWER_STYLE_ARROW)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY - 40;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 30;
				schline.m_cVertex[0].fy = m_fY - 40;
				schline.m_cVertex[1].fx = m_fX + 30;
				schline.m_cVertex[1].fy = m_fY - 40;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 30;
				schline.m_cVertex[0].fy = m_fY - 40;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY - 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 30;
				schline.m_cVertex[0].fy = m_fY - 40;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY - 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);		
				
				fx = m_fX;// - (double)offset*pView->m_fScale/2;
				fy = m_fY - 120 - 85;
			}
			else if(m_nStyle == POWER_STYLE_BAR)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY - 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 50;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX + 50;
				schline.m_cVertex[1].fy = m_fY - 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				
				fx = m_fX;// - (double)offset*pView->m_fScale/2;
				fy = m_fY - 120 - 85;
			}
			else if(m_nStyle == POWER_STYLE_WAVE)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY - 60;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				scharc.m_fCx = m_fX;
				scharc.m_fCy = m_fY - 100;
				scharc.m_fRadius = 40;
				scharc.m_fSangle = 90;
				scharc.m_fEangle = 180;
				if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);
				scharc.m_fCx = m_fX;
				scharc.m_fCy = m_fY - 20;
				scharc.m_fRadius = 40;
				scharc.m_fSangle = 270;
				scharc.m_fEangle = 360;
				if(scharc.PickMe(pV) == TRUE) scharc.Draw(pDC, pV);

				fx = m_fX;//- (double)offset*pView->m_fScale/2;
				fy = m_fY - 120 - 85;
			}
			else if(m_nStyle == POWER_STYLE_POWERGROUND)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY - 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY - 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 70;
				schline.m_cVertex[0].fy = m_fY - 130;
				schline.m_cVertex[1].fx = m_fX + 70;
				schline.m_cVertex[1].fy = m_fY - 130;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 40;
				schline.m_cVertex[0].fy = m_fY - 160;
				schline.m_cVertex[1].fx = m_fX + 40; 
				schline.m_cVertex[1].fy = m_fY - 160;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 10;
				schline.m_cVertex[0].fy = m_fY - 190;
				schline.m_cVertex[1].fx = m_fX + 10; 
				schline.m_cVertex[1].fy = m_fY - 190;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX;// - (double)offset*pView->m_fScale/2;
				fy = m_fY - 220 - 85;
			}
			else if(m_nStyle == POWER_STYLE_SIGNALGROUND)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY - 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY - 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY - 200;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX; 
				schline.m_cVertex[1].fy = m_fY - 200;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX;// - (double)offset*pView->m_fScale/2;
				fy = m_fY - 220 - 85;
			}
			else if(m_nStyle == POWER_STYLE_EARTH)
			{
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY;
				schline.m_cVertex[1].fx = m_fX;
				schline.m_cVertex[1].fy = m_fY - 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX + 100;
				schline.m_cVertex[1].fy = m_fY - 100;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX - 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX - 150;
				schline.m_cVertex[1].fy = m_fY - 200;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX - 50; 
				schline.m_cVertex[1].fy = m_fY - 200;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = m_fX + 100;
				schline.m_cVertex[0].fy = m_fY - 100;
				schline.m_cVertex[1].fx = m_fX + 50; 
				schline.m_cVertex[1].fy = m_fY - 200;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

				fx = m_fX;// - (double)offset*pView->m_fScale/2;
				fy = m_fY - 220 - 85;
			}
		}
	}
	
	if(m_bShowNetName == TRUE)
	{
		schtext.m_fSx = fx;
		schtext.m_fSy = fy;
		if(schtext.PickMe(pDC, pV) == TRUE) schtext.Draw(pDC, pV);
	}

	//Hot Point
	if((pView->m_fScale < 10)&&(nEDA_Print_Flag == 0))
	{
		CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
		CPen apen;
		apen.CreatePen(PS_SOLID, 1, pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor);
		CPen* oldpen = pDC->SelectObject(&apen);
		int x, y, x0, y0;
		pView->Get_VPort_XY(&x, &y, m_fX, m_fY);
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

void COb_schpower::DrawSelection(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	CPen arcpen;
	arcpen.CreatePen(PS_DOT, 1, nEDA_SelectionColor);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	double fminx, fminy, fmaxx, fmaxy;
	if(m_nOrientation == 90 )
	{
		fminx = m_fX - 100;
		fminy = m_fY;
		fmaxx = m_fX + 100;
		fmaxy = m_fY + 200;
	}
	else if(m_nOrientation == 180 )
	{
		fminx = m_fX - 200;
		fminy = m_fY - 100;
		fmaxx = m_fX;
		fmaxy = m_fY + 100;
	}
	else if(m_nOrientation == 270 )
	{
		fminx = m_fX - 100;
		fminy = m_fY - 200;
		fmaxx = m_fX + 100;
		fmaxy = m_fY;
	}
	else
	{
		fminx = m_fX;
		fminy = m_fY - 100;
		fmaxx = m_fX + 200;
		fmaxy = m_fY + 100;
	}
	
	int x1,y1,x2,y2;
	pView->Get_VPort_XY(&x1, &y1, fminx, fminy);
	pView->Get_VPort_XY(&x2, &y2, fmaxx, fmaxy);

	pDC->MoveTo(x1, y1);
	pDC->LineTo(x1, y2);
	pDC->LineTo(x2, y2);
	pDC->LineTo(x2, y1);	
	pDC->LineTo(x1, y1);


	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}

BOOL  COb_schpower::Is_On_Me(double ffx, double ffy, CView* pV)
{
	//if(diagram != m_nDiagram) return FALSE;
	CClientDC dc(NULL);
	CClientDC* pDC = &dc;

	CESDView* pView = (CESDView*)pV;

	COb_schline schline;
	schline.m_nFlag = 0;
	schline.m_nDiagram = m_nDiagram;
	schline.m_nAttrib = LINE_WIRE;
	schline.m_nColor = m_nColor;
	schline.m_nLineType = LINE_SOLID;
	schline.m_nLineWidth = LINE_SMALL;
	schline.m_cVertex.SetSize(2);

	COb_scharc scharc;
	scharc.m_nFlag = 0;
	scharc.m_nDiagram = m_nDiagram;
	scharc.m_nColor = m_nColor;
	scharc.m_nWidth = LINE_SMALLEST;

	COb_schtext schtext;
	schtext.m_nFlag = 0;
	schtext.m_nDiagram = m_nDiagram;
	schtext.m_nColor = m_nColor;
	schtext.m_nFont = 0;
	schtext.m_nOrientation = 0;
	schtext.m_csText = m_csNetName;
	
	schtext.m_fSx = schtext.m_fSy = 0;
	//int offset = schtext.GetExtent(pDC, pV);

	double fx, fy;
	double fX1, fY1, fX2, fY2;

	if(m_nOrientation == 0)
	{
		schtext.m_nH_Justification = 0;
		if(m_bIsCrossSheetConnector == TRUE)
		{
			fX1 = m_fX;
			fY1 = m_fY - 50;
			fX2 = m_fX + 100;
			fY2 = m_fY + 50;

			fx = m_fX + 140;
			fy = m_fY - 50;
		}
		else
		{
			if((m_nStyle == POWER_STYLE_CIRCLE)||(m_nStyle == POWER_STYLE_ARROW)||(m_nStyle == POWER_STYLE_BAR)||(m_nStyle == POWER_STYLE_WAVE))
			{
				fX1 = m_fX;
				fY1 = m_fY - 50;
				fX2 = m_fX + 100;
				fY2 = m_fY + 50;

				fx = m_fX + 120;
				fy = m_fY - 50;
			}
			else
			{
				fX1 = m_fX;
				fY1 = m_fY - 100;
				fX2 = m_fX + 200;
				fY2 = m_fY + 100;

				fx = m_fX + 220;
				fy = m_fY - 50;
			}
		}
	}
	else if(m_nOrientation == 90)
	{
		schtext.m_nH_Justification = 1;
		if(m_bIsCrossSheetConnector == TRUE)
		{
			fX1 = m_fX - 50;
			fY1 = m_fY;
			fX2 = m_fX + 50;
			fY2 = m_fY + 100;

			fx = m_fX;// - (double)offset*pView->m_fScale/2;
			fy = m_fY + 140 - 25;
		}
		else
		{
			if((m_nStyle == POWER_STYLE_CIRCLE)||(m_nStyle == POWER_STYLE_ARROW)||(m_nStyle == POWER_STYLE_BAR)||(m_nStyle == POWER_STYLE_WAVE))
			{
				fX1 = m_fX - 50;
				fY1 = m_fY;
				fX2 = m_fX + 50;
				fY2 = m_fY + 100;

				fx = m_fX;// - (double)offset*pView->m_fScale/2;
				fy = m_fY + 120 - 25;
			}
			else 
			{
				fX1 = m_fX - 100;
				fY1 = m_fY;
				fX2 = m_fX + 100;
				fY2 = m_fY + 200;

				fx = m_fX;// - (double)offset*pView->m_fScale/2;
				fy = m_fY + 220 - 25;
			}
		}
	}
	else if(m_nOrientation == 180)
	{
		schtext.m_nH_Justification = 2;
		if(m_bIsCrossSheetConnector == TRUE)
		{
			fX1 = m_fX - 100;
			fY1 = m_fY - 50;
			fX2 = m_fX;
			fY2 = m_fY + 50;
			
			fx = m_fX - 140;// - (double)offset*pView->m_fScale;
			fy = m_fY - 50;
		}
		else
		{
			if((m_nStyle == POWER_STYLE_CIRCLE)||(m_nStyle == POWER_STYLE_ARROW)||(m_nStyle == POWER_STYLE_BAR)||(m_nStyle == POWER_STYLE_WAVE))
			{
				fX1 = m_fX - 100;
				fY1 = m_fY - 50;
				fX2 = m_fX;
				fY2 = m_fY + 50;

				fx = m_fX - 120;// - (double)offset*pView->m_fScale;
				fy = m_fY - 50;
			}
			else
			{
				fX1 = m_fX - 200;
				fY1 = m_fY - 100;
				fX2 = m_fX;
				fY2 = m_fY + 100;

				fx = m_fX - 220;// - (double)offset*pView->m_fScale;
				fy = m_fY - 50;
			}
		}
	}
	else if(m_nOrientation == 270)
	{
		schtext.m_nH_Justification = 1;
		if(m_bIsCrossSheetConnector == TRUE)
		{
			fX1 = m_fX - 50;
			fY1 = m_fY - 100;
			fX2 = m_fX + 50;
			fY2 = m_fY;

			fx = m_fX;// - (double)offset*pView->m_fScale/2;
			fy = m_fY - 140 - 85;
		}
		else
		{
			if((m_nStyle == POWER_STYLE_CIRCLE)||(m_nStyle == POWER_STYLE_ARROW)||(m_nStyle == POWER_STYLE_BAR)||(m_nStyle == POWER_STYLE_WAVE))
			{
				fX1 = m_fX - 50;
				fY1 = m_fY - 100;
				fX2 = m_fX + 50;
				fY2 = m_fY;

				fx = m_fX;// - (double)offset*pView->m_fScale/2;
				fy = m_fY - 120 - 85;
			}
			else
			{
				fX1 = m_fX - 100;
				fY1 = m_fY - 200;
				fX2 = m_fX + 100;
				fY2 = m_fY;

				fx = m_fX;// - (double)offset*pView->m_fScale/2;
				fy = m_fY - 220 - 85;
			}
		}
	}
	
	CRgn   rgn;
	rgn.CreateRectRgn( (int)fX1, (int)fY1, (int)fX2, (int)fY2);
	if(rgn.PtInRegion((int)ffx, (int)ffy) != FALSE) return TRUE;

	if(m_bShowNetName == TRUE)
	{
		schtext.m_fSx = fx;
		schtext.m_fSy = fy;
		if(schtext.Is_On_Me(ffx, ffy, m_nDiagram, pV) == TRUE) return TRUE;
	}

	return FALSE;
}

BOOL  COb_schpower::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fx, fy;
	fx = m_fX;
	fy = m_fY;

	if((fx < fx1)||(fx > fx2)||(fy < fy1)||(fy > fy2)) return FALSE;
	return TRUE;
}

void COb_schpower::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(m_bIsCrossSheetConnector == TRUE)
	{
		CDlg_EditOffSheet dlg;
		
		dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fX);
		dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fY);
		dlg.m_nColor = m_nColor;
		dlg.m_nComboOrientation = m_nOrientation/90;
		dlg.m_csEditNet = m_csNetName;
		dlg.m_nComboStyle = m_nStyle;

		if(dlg.DoModal() == IDOK)
		{
			if(dlg.m_bCheckGlobal == TRUE)
			{
				CDlg_Global_Power subdlg;
				subdlg.bIsCrossSheetConnector = 1;
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
									
								if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
								{
									COb_schpower* ppower = (COb_schpower*)pobject;
									if((ppower->m_bIsCrossSheetConnector != TRUE)||(ppower->m_nDiagram != nDiagram)||(ppower->m_nFlag != FLAG_NORMAL))	continue;
									if(ppower == this) continue; 

									if( (((subdlg.m_bCheckColor == TRUE)&&(ppower->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
										(((subdlg.m_bCheckStyle == TRUE)&&(ppower->m_nStyle == m_nStyle))||(subdlg.m_bCheckStyle == FALSE)) &&
										(((subdlg.m_bCheckOrientation == TRUE)&&(ppower->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) )
									{
										if((ppower->m_nColor == m_nColor)||(ppower->m_nStyle == m_nStyle)||(ppower->m_nOrientation == m_nOrientation))
										{
											CSchUndo *pundo = new CSchUndo;
											pundo->m_nOp = UNDO_CHANGE;
											pundo->m_nOb = SCH_ELEMENT_POWER;
											pundo->m_nDiagram = nDiagram;
											pundo->m_nPos = posbak;
											pundo->schpower.Copy(ppower);
											pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

										}
										if(ppower->m_nColor == m_nColor) ppower->m_nColor = dlg.m_nColor;
										if(ppower->m_nOrientation == m_nOrientation) ppower->m_nOrientation = dlg.m_nComboOrientation*90;
										if(ppower->m_nStyle == m_nStyle) ppower->m_nStyle = dlg.m_nComboStyle;
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
									
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
							{
								COb_schpower* ppower = (COb_schpower*)pobject;
								if((ppower->m_bIsCrossSheetConnector != TRUE)||(ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag != FLAG_NORMAL))	continue;
								if(ppower == this) continue; 

								if( (((subdlg.m_bCheckColor == TRUE)&&(ppower->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckStyle == TRUE)&&(ppower->m_nStyle == m_nStyle))||(subdlg.m_bCheckStyle == FALSE)) &&
									(((subdlg.m_bCheckOrientation == TRUE)&&(ppower->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) )
								{
									if((ppower->m_nColor == m_nColor)||(ppower->m_nStyle == m_nStyle)||(ppower->m_nOrientation == m_nOrientation))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_POWER;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = posbak;
										pundo->schpower.Copy(ppower);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

									}
									if(ppower->m_nColor == m_nColor) ppower->m_nColor = dlg.m_nColor;
									if(ppower->m_nOrientation == m_nOrientation) ppower->m_nOrientation = dlg.m_nComboOrientation*90;
									if(ppower->m_nStyle == m_nStyle) ppower->m_nStyle = dlg.m_nComboStyle;
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
									
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
							{
								COb_schpower* ppower = (COb_schpower*)pobject;
								if((ppower->m_bSelection != TRUE)||(ppower->m_bIsCrossSheetConnector != TRUE)||(ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag != FLAG_NORMAL))	continue;
								if(ppower == this) continue;

								if( (((subdlg.m_bCheckColor == TRUE)&&(ppower->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckStyle == TRUE)&&(ppower->m_nStyle == m_nStyle))||(subdlg.m_bCheckStyle == FALSE)) &&
									(((subdlg.m_bCheckOrientation == TRUE)&&(ppower->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) )
								{
									if((ppower->m_nColor == m_nColor)||(ppower->m_nStyle == m_nStyle)||(ppower->m_nOrientation == m_nOrientation))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_POWER;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = posbak;
										pundo->schpower.Copy(ppower);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(ppower->m_nColor == m_nColor) ppower->m_nColor = dlg.m_nColor;
									if(ppower->m_nOrientation == m_nOrientation) ppower->m_nOrientation = dlg.m_nComboOrientation*90;
									if(ppower->m_nStyle == m_nStyle) ppower->m_nStyle = dlg.m_nComboStyle;
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
					pundo->m_nOb = SCH_ELEMENT_POWER;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = pos;
					pundo->schpower.Copy(this);
						
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					m_fX = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
					m_fY = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
					m_nColor = dlg.m_nColor;
					m_nOrientation = dlg.m_nComboOrientation*90;
					m_csNetName = dlg.m_csEditNet;
					m_nStyle = dlg.m_nComboStyle;

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
				pundo->m_nOb = SCH_ELEMENT_POWER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schpower.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

				m_fX = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
				m_fY = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
				m_nColor = dlg.m_nColor;
				m_nOrientation = dlg.m_nComboOrientation*90;
				m_csNetName = dlg.m_csEditNet;
				m_nStyle = dlg.m_nComboStyle;
			}
		}
	}
	else
	{
        CDlg_EditPower dlg;
		dlg.m_nPowerOrGround = -1;
		dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fX);
		dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fY);
		dlg.m_nColor = m_nColor;
		dlg.m_nComboOrientation = m_nOrientation/90;
		dlg.m_csEditNet = m_csNetName;
		dlg.m_nComboStyle = m_nStyle;
		dlg.m_bCheckShowNet = m_bShowNetName;

		if(dlg.DoModal() == IDOK)
		{
			if(dlg.m_bCheckGlobal == TRUE)
			{
				CDlg_Global_Power subdlg;
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
									
								if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
								{
									COb_schpower* ppower = (COb_schpower*)pobject;
									if((ppower->m_bIsCrossSheetConnector == TRUE)||(ppower->m_nDiagram != nDiagram)||(ppower->m_nFlag != FLAG_NORMAL))	continue;
									if(ppower == this) continue; 

									if( (((subdlg.m_bCheckColor == TRUE)&&(ppower->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
										(((subdlg.m_bCheckStyle == TRUE)&&(ppower->m_nStyle == m_nStyle))||(subdlg.m_bCheckStyle == FALSE)) &&
										(((subdlg.m_bCheckOrientation == TRUE)&&(ppower->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
										(((subdlg.m_bCheckVisible == TRUE)&&(ppower->m_bShowNetName == m_bShowNetName))||(subdlg.m_bCheckVisible == FALSE)) )
									{
										if( (ppower->m_nColor == m_nColor)||(ppower->m_nStyle == m_nStyle)||\
											(ppower->m_nOrientation == m_nOrientation)||(ppower->m_bShowNetName == m_bShowNetName))
										{
											CSchUndo *pundo = new CSchUndo;
											pundo->m_nOp = UNDO_CHANGE;
											pundo->m_nOb = SCH_ELEMENT_POWER;
											pundo->m_nDiagram = nDiagram;
											pundo->m_nPos = posbak;
											pundo->schpower.Copy(ppower);
											pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
										}
										if(ppower->m_nColor == m_nColor) ppower->m_nColor = dlg.m_nColor;
										if(ppower->m_nOrientation == m_nOrientation) ppower->m_nOrientation = dlg.m_nComboOrientation*90;
										if(ppower->m_nStyle == m_nStyle) ppower->m_nStyle = dlg.m_nComboStyle;
										if(ppower->m_bShowNetName == m_bShowNetName) ppower->m_bShowNetName = dlg.m_bCheckShowNet;
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
									
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
							{
								COb_schpower* ppower = (COb_schpower*)pobject;
								if((ppower->m_bIsCrossSheetConnector == TRUE)||(ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag != FLAG_NORMAL))	continue;
								if(ppower == this) continue; 

								if( (((subdlg.m_bCheckColor == TRUE)&&(ppower->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckStyle == TRUE)&&(ppower->m_nStyle == m_nStyle))||(subdlg.m_bCheckStyle == FALSE)) &&
									(((subdlg.m_bCheckOrientation == TRUE)&&(ppower->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
									(((subdlg.m_bCheckVisible == TRUE)&&(ppower->m_bShowNetName == m_bShowNetName))||(subdlg.m_bCheckVisible == FALSE)) )
								{
									if( (ppower->m_nColor == m_nColor)||(ppower->m_nStyle == m_nStyle)||\
										(ppower->m_nOrientation == m_nOrientation)||(ppower->m_bShowNetName == m_bShowNetName))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_POWER;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = posbak;
										pundo->schpower.Copy(ppower);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(ppower->m_nColor == m_nColor) ppower->m_nColor = dlg.m_nColor;
									if(ppower->m_nOrientation == m_nOrientation) ppower->m_nOrientation = dlg.m_nComboOrientation*90;
									if(ppower->m_nStyle == m_nStyle) ppower->m_nStyle = dlg.m_nComboStyle;
									if(ppower->m_bShowNetName == m_bShowNetName) ppower->m_bShowNetName = dlg.m_bCheckShowNet;
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
									
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
							{
								COb_schpower* ppower = (COb_schpower*)pobject;
								if((ppower->m_bSelection != TRUE)||(ppower->m_bIsCrossSheetConnector == TRUE)||(ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag != FLAG_NORMAL))	continue;
								if(ppower == this) continue; 

								if( (((subdlg.m_bCheckColor == TRUE)&&(ppower->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckStyle == TRUE)&&(ppower->m_nStyle == m_nStyle))||(subdlg.m_bCheckStyle == FALSE)) &&
									(((subdlg.m_bCheckOrientation == TRUE)&&(ppower->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
									(((subdlg.m_bCheckVisible == TRUE)&&(ppower->m_bShowNetName == m_bShowNetName))||(subdlg.m_bCheckVisible == FALSE)) )
								{
									if( (ppower->m_nColor == m_nColor)||(ppower->m_nStyle == m_nStyle)||\
										(ppower->m_nOrientation == m_nOrientation)||(ppower->m_bShowNetName == m_bShowNetName))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_POWER;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = posbak;
										pundo->schpower.Copy(ppower);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(ppower->m_nColor == m_nColor) ppower->m_nColor = dlg.m_nColor;
									if(ppower->m_nOrientation == m_nOrientation) ppower->m_nOrientation = dlg.m_nComboOrientation*90;
									if(ppower->m_nStyle == m_nStyle) ppower->m_nStyle = dlg.m_nComboStyle;
									if(ppower->m_bShowNetName == m_bShowNetName) ppower->m_bShowNetName = dlg.m_bCheckShowNet;
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
					pundo->m_nOb = SCH_ELEMENT_POWER;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = pos;
					pundo->schpower.Copy(this);
						
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					m_fX = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
					m_fY = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
					m_nColor = dlg.m_nColor;
					m_nOrientation = dlg.m_nComboOrientation*90;
					m_csNetName = dlg.m_csEditNet;
					m_nStyle = dlg.m_nComboStyle;
					m_bShowNetName = dlg.m_bCheckShowNet;

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
				pundo->m_nOb = SCH_ELEMENT_POWER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schpower.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

				m_fX = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
				m_fY = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
				m_nColor = dlg.m_nColor;
				m_nOrientation = dlg.m_nComboOrientation*90;
				m_csNetName = dlg.m_csEditNet;
				m_nStyle = dlg.m_nComboStyle;
				m_bShowNetName = dlg.m_bCheckShowNet;
			}
		}
	}
}

void COb_schpower::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE; 
	
	char szHelp[] = "移动电源符号: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV);

	x0 = m_fX;
	y0 = m_fY;
	
	pView->m_fCurrentSnapX = x0;
	pView->m_fCurrentSnapY = y0;
	pView->Restore_LargeCrossXY();

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schpower apower;
	apower.pDocument = pDoc;
	apower.m_fX = m_fX;
	apower.m_fY = m_fY;
	apower.m_nStyle = m_nStyle;
	apower.m_nOrientation = m_nOrientation;
	apower.m_bShowNetName = FALSE;
	apower.m_bIsCrossSheetConnector = m_bIsCrossSheetConnector;
	apower.m_csNetName = m_csNetName;
	apower.m_nColor = m_nColor;
	apower.m_nDiagram = m_nDiagram;
	apower.Draw(pDC, pV);
	pView->OpenMouseCursor();
	
	bEDA_NeedElectricalGrid = TRUE;
	
	flag = apower.m_nOrientation/90;
	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			apower.Draw(pDC, pV);
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
				apower.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX-x0, m_fY-y0, flag*90, FALSE);
			apower.m_fX = fx + x0 + deltax;
			apower.m_fY = fy + y0 + deltay;
			
			pView->CloseMouseCursor();
			apower.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			apower.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;
			
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX-x0, m_fY-y0, flag*90, FALSE);
			apower.m_fX = fx + x0 + deltax;
			apower.m_fY = fy + y0 + deltay;

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_POWER;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->m_fX1 = m_fX;
			pundo->m_fY1 = m_fY;
			pundo->m_nOrientation = m_nOrientation;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fX = apower.m_fX;
			m_fY = apower.m_fY;
			m_nOrientation = apower.m_nOrientation;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			apower.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX-x0, m_fY-y0, flag*90, FALSE);
			apower.m_fX = fx + x0 + deltax;
			apower.m_fY = fy + y0 + deltay;

			apower.m_nOrientation = flag*90;

			apower.Draw(pDC, pV);

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
		if(bEDA_AutoJunction == TRUE)
		{
			pView->AutoPlaceJunction();
			pView->AutoRemoveJunction();
		}

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
	
	bEDA_NeedElectricalGrid = FALSE;
	pView->m_fpHotPoint1.fx = pView->m_fpHotPoint1.fy = pView->m_fpHotPoint2.fx = pView->m_fpHotPoint2.fy = 0;

	pView->Write_Help(szEDA_Help_Ready);
}

BOOL COb_schpower::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	
	char szHelp[] = "复制电源符号: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	x0 = m_fX;
	y0 = m_fY;
	
	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	


	COb_schpower apower;
	apower.pDocument = pDoc;
	apower.m_fX = m_fX;
	apower.m_fY = m_fY;
	apower.m_nStyle = m_nStyle;
	apower.m_nOrientation = m_nOrientation;
	apower.m_bShowNetName = FALSE;
	apower.m_bIsCrossSheetConnector = m_bIsCrossSheetConnector;
	apower.m_csNetName = m_csNetName;
	apower.m_nColor = m_nColor;
	apower.m_nDiagram = m_nDiagram;
	apower.Draw(pDC, pV);
	pView->OpenMouseCursor();
	
	bEDA_NeedElectricalGrid = TRUE;

	flag = apower.m_nOrientation/90;
	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			apower.Draw(pDC, pV);
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
				apower.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX-x0, m_fY-y0, flag*90, FALSE);
			apower.m_fX = fx + x0 + deltax;
			apower.m_fY = fy + y0 + deltay;
			
			pView->CloseMouseCursor();
			apower.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			apower.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;
			
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX-x0, m_fY-y0, flag*90, FALSE);
			apower.m_fX = fx + x0 + deltax;
			apower.m_fY = fy + y0 + deltay;

			m_fX = apower.m_fX;
			m_fY = apower.m_fY;
			m_nOrientation = apower.m_nOrientation;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			apower.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX-x0, m_fY-y0, flag*90, FALSE);
			apower.m_fX = fx + x0 + deltax;
			apower.m_fY = fy + y0 + deltay;

			apower.m_nOrientation = flag*90;

			apower.Draw(pDC, pV);

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
	
	bEDA_NeedElectricalGrid = FALSE;
	pView->m_fpHotPoint1.fx = pView->m_fpHotPoint1.fy = pView->m_fpHotPoint2.fx = pView->m_fpHotPoint2.fy = 0;
	
	pView->Write_Help(szEDA_Help_Ready);
		
	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;
}

void COb_schpower::Copy(COb_schpower* pschpower)
{
	m_fX = pschpower->m_fX;
	m_fY = pschpower->m_fY;
	m_nStyle = pschpower->m_nStyle;
	m_nOrientation = pschpower->m_nOrientation;
	m_bShowNetName = pschpower->m_bShowNetName;
	m_bIsCrossSheetConnector = pschpower->m_bIsCrossSheetConnector;
	m_csNetName = pschpower->m_csNetName;
	
	m_nColor = pschpower->m_nColor;

	m_nDiagram = pschpower->m_nDiagram;
	m_bSelection = pschpower->m_bSelection;
	m_nFlag = pschpower->m_nFlag;

	pDocument = pschpower->pDocument;
}