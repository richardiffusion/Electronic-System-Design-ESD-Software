#include "stdafx.h"

#include "ESD.h"
#include "mainfrm.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "childfrm.h"
#include "sysparam.h"
#include "systemext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_schsheet, CObject, 0)

COb_schsheet::COb_schsheet()
{
	// This empty constructor should be used by serialization only
	m_nStandardStyle = 0; 

	m_nOrientation = LANDSCAPE;
	m_nBorderColor = RGB(0,0,0);
	m_nSheetColor = RGB(255,255,255);

	m_bTitleBlock = TRUE;
	m_nSheetNumberSpace = 4;
	m_bShowReferenceZones = TRUE;
	m_bShowBorder = TRUE;
	m_bShowTemplateGraphics = FALSE;
	m_nTitleBlockStyle = 0;
	//font
	m_nFont = 0;

	m_bCustom = FALSE;
	m_fCustomWidth = (double)sEDA_Standard_Sheet[0].width;
	m_fCustomHeight =(double)sEDA_Standard_Sheet[0].height;
	m_nCustomXRegionCount = sEDA_Standard_Sheet[0].xregioncount;
	m_nCustomYRegionCount = sEDA_Standard_Sheet[0].yregioncount;
	m_fCustomMargin = (double)sEDA_Standard_Sheet[0].margin;
	
	pDocument = NULL;
}


void COb_schsheet::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nStandardStyle;
		ar << m_bCustom;
		ar << m_fCustomWidth;
		ar << m_fCustomHeight;
		ar << m_fCustomMargin;
		ar << m_nCustomXRegionCount;
		ar << m_nCustomYRegionCount;
		ar << m_nOrientation;
		ar << m_nBorderColor;
		ar << m_nSheetColor;
		ar << m_bTitleBlock;
		ar << m_nTitleBlockStyle;
		ar << m_nSheetNumberSpace;
		ar << m_bShowReferenceZones;
		ar << m_bShowBorder;
		ar << m_bShowTemplateGraphics;
		ar << m_nFont;
	}
	else
	{
		ar >> m_nStandardStyle;
		ar >> m_bCustom;
		ar >> m_fCustomWidth;
		ar >> m_fCustomHeight;
		ar >> m_fCustomMargin;
		ar >> m_nCustomXRegionCount;
		ar >> m_nCustomYRegionCount;
		ar >> m_nOrientation;
		ar >> m_nBorderColor;
		ar >> m_nSheetColor;
		ar >> m_bTitleBlock;
		ar >> m_nTitleBlockStyle;
		ar >> m_nSheetNumberSpace;
		ar >> m_bShowReferenceZones;
		ar >> m_bShowBorder;
		ar >> m_bShowTemplateGraphics;
		ar >> m_nFont;
	}
}

void COb_schsheet::Draw(CDC* pDC, CView* pV)
{
	double width, height,margin;
	int xd, yd;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();


	COb_schline schline;
	schline.m_nFlag = 0;
	schline.m_nDiagram = m_nDiagram;
	schline.m_nAttrib = LINE_WIRE;
	
	COb_schtext schtext;
	schtext.m_nFlag = 0;
	schtext.m_nDiagram = m_nDiagram;
	schtext.m_nColor = m_nBorderColor;
	schtext.m_nFont = m_nFont;
	schtext.m_nOrientation = 0;

	COb_schpolygon schpolygon;
	schpolygon.m_nFlag = 0;
	schpolygon.m_nDiagram = m_nDiagram;
	schpolygon.m_nBorderWidth = 1;
	schpolygon.m_bTransparent = FALSE;
	schpolygon.m_bFillSolid = TRUE;


	if(m_bCustom == TRUE)  
	{
		width = m_fCustomWidth;
		height = m_fCustomHeight;
		xd = m_nCustomXRegionCount;
		yd = m_nCustomYRegionCount;
		margin = m_fCustomMargin;
	}
	else 
	{
		width = (double)sEDA_Standard_Sheet[m_nStandardStyle].width;
		height = (double)sEDA_Standard_Sheet[m_nStandardStyle].height;
		xd = sEDA_Standard_Sheet[m_nStandardStyle].xregioncount;
		yd = sEDA_Standard_Sheet[m_nStandardStyle].yregioncount;
		margin = (double)sEDA_Standard_Sheet[m_nStandardStyle].margin;
	}
	
	if(m_nOrientation != LANDSCAPE) 
	{
		double f; int i;
		f = width; width = height; height = f;
		i = xd;    xd = yd;    yd = i;
	}


	schpolygon.m_cVertex.SetSize(4);
	schpolygon.m_cVertex[0].fx = 0;
	schpolygon.m_cVertex[0].fy = 0;
	schpolygon.m_cVertex[1].fx = width;
	schpolygon.m_cVertex[1].fy = 0;
	schpolygon.m_cVertex[2].fx = width;
	schpolygon.m_cVertex[2].fy = height;
	schpolygon.m_cVertex[3].fx = 0;
	schpolygon.m_cVertex[3].fy = height;
	schpolygon.m_nBorderColor = m_nSheetColor;
	schpolygon.m_nFillColor = m_nSheetColor;
	if(nEDA_Print_Flag == 0)
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);


	if((pDoc->m_bSCH_VisibleGridShow == TRUE)&&(nEDA_Print_Flag == 0))
	{
		if(nEDA_SCH_VisibleGridMode == GRID_LINE)
		{
			schline.m_nColor = nEDA_SCH_VisibleGridColor;
			schline.m_nLineWidth = LINE_SMALLEST;
			schline.m_cVertex.SetSize(2);
			if( (int)(2*pDoc->m_fSCH_VisibleGrid/pView->m_fScale) >= 32)
			{
				int sx, sy;
				sx = (int)(pView->m_fScreenX/pDoc->m_fSCH_VisibleGrid);
				sx = sx/2; sx = sx*2;
				sy = (int)(pView->m_fScreenY/pDoc->m_fSCH_VisibleGrid);
				sy = sy/2; sy = sy*2;
				for(int i=sx; i<(int)width/pDoc->m_fSCH_VisibleGrid; i+=2)
				{
					if( (i*pDoc->m_fSCH_VisibleGrid) > (pView->m_fScreenX + pView->m_nViewport_Ex*pView->m_fScale) ) break;
					schline.m_nLineType = LINE_SOLID;
					schline.m_cVertex[0].fx = i*pDoc->m_fSCH_VisibleGrid;
					schline.m_cVertex[0].fy = 0;
					schline.m_cVertex[1].fx = i*pDoc->m_fSCH_VisibleGrid;
					schline.m_cVertex[1].fy = height;
					if(schline.PickMe(pV) == TRUE)
					{
						schline.Draw(pDC, pV);
					}
				}
				for(int i=sx+1; i<(int)width/pDoc->m_fSCH_VisibleGrid; i+=2)
				{
					if( (i*pDoc->m_fSCH_VisibleGrid) > (pView->m_fScreenX + pView->m_nViewport_Ex*pView->m_fScale) ) break;
					schline.m_nLineType = LINE_DOTTED;
					schline.m_cVertex[0].fx = i*pDoc->m_fSCH_VisibleGrid;
					schline.m_cVertex[0].fy = 0;
					schline.m_cVertex[1].fx = i*pDoc->m_fSCH_VisibleGrid;
					schline.m_cVertex[1].fy = height;
					if(schline.PickMe(pV) == TRUE) 
					{
						schline.Draw(pDC, pV);
					}
				}
				for(int i=sy; i<(int)height/pDoc->m_fSCH_VisibleGrid; i+=2)
				{
					if( (i*pDoc->m_fSCH_VisibleGrid) > (pView->m_fScreenY + pView->m_nViewport_Ey*pView->m_fScale) ) break;
					schline.m_nLineType = LINE_SOLID;
					schline.m_cVertex[0].fx = 0;
					schline.m_cVertex[0].fy = i*pDoc->m_fSCH_VisibleGrid;
					schline.m_cVertex[1].fx = width;
					schline.m_cVertex[1].fy = i*pDoc->m_fSCH_VisibleGrid;
					if(schline.PickMe(pV) == TRUE) {
						schline.Draw(pDC, pV);
					}
				}
				for(int i=sy+1; i<(int)height/pDoc->m_fSCH_VisibleGrid; i+=2)
				{
					if( (i*pDoc->m_fSCH_VisibleGrid) > (pView->m_fScreenY + pView->m_nViewport_Ey*pView->m_fScale) ) break;
					schline.m_nLineType = LINE_DOTTED;
					schline.m_cVertex[0].fx = 0;
					schline.m_cVertex[0].fy = i*pDoc->m_fSCH_VisibleGrid;
					schline.m_cVertex[1].fx = width;
					schline.m_cVertex[1].fy = i*pDoc->m_fSCH_VisibleGrid;
					if(schline.PickMe(pV) == TRUE) 
					{
						schline.Draw(pDC, pV);
					}
				}
			}
			else if( (int)(2*pDoc->m_fSCH_VisibleGrid/pView->m_fScale) >= 16)
			{
				int sx, sy;
				sx = (int)(pView->m_fScreenX/pDoc->m_fSCH_VisibleGrid);
				sx = sx/2; sx = sx*2;
				sy = (int)(pView->m_fScreenY/pDoc->m_fSCH_VisibleGrid);
				sy = sy/2; sy = sy*2;
				for(int i=sx; i<(int)width/pDoc->m_fSCH_VisibleGrid; i+=2)
				{
					if( (i*pDoc->m_fSCH_VisibleGrid) > (pView->m_fScreenX + pView->m_nViewport_Ex*pView->m_fScale) ) break;
					schline.m_nLineType = LINE_SOLID;
					schline.m_cVertex[0].fx = i*pDoc->m_fSCH_VisibleGrid;
					schline.m_cVertex[0].fy = 0;
					schline.m_cVertex[1].fx = i*pDoc->m_fSCH_VisibleGrid;
					schline.m_cVertex[1].fy = height;
					if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				}
				for(int i=sy; i<(int)height/pDoc->m_fSCH_VisibleGrid; i+=2)
				{
					if( (i*pDoc->m_fSCH_VisibleGrid) > (pView->m_fScreenY + pView->m_nViewport_Ey*pView->m_fScale) ) break;
					schline.m_nLineType = LINE_SOLID;
					schline.m_cVertex[0].fx = 0;
					schline.m_cVertex[0].fy = i*pDoc->m_fSCH_VisibleGrid;
					schline.m_cVertex[1].fx = width;
					schline.m_cVertex[1].fy = i*pDoc->m_fSCH_VisibleGrid;
					if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				}
			}
		}
		else
		{
			if( (int)(pDoc->m_fSCH_VisibleGrid/pView->m_fScale) >= 8)
			{
				int sx, sy;
				sx = (int)(pView->m_fScreenX/pDoc->m_fSCH_VisibleGrid);
				sx = sx/2; sx = sx*2;
				sy = (int)(pView->m_fScreenY/pDoc->m_fSCH_VisibleGrid);
				sy = sy/2; sy = sy*2;
				for(int i=sx; i<(int)width/pDoc->m_fSCH_VisibleGrid; i++)
				{
					if( (i*pDoc->m_fSCH_VisibleGrid) > (pView->m_fScreenX + pView->m_nViewport_Ex*pView->m_fScale) ) break;
					for(int j=sy; j<(int)height/pDoc->m_fSCH_VisibleGrid; j++)
					{
						if( (j*pDoc->m_fSCH_VisibleGrid) > (pView->m_fScreenY + pView->m_nViewport_Ey*pView->m_fScale) ) break;
						
						int x, y;
						pView->Get_VPort_XY(&x, &y, i*pDoc->m_fSCH_VisibleGrid, j*pDoc->m_fSCH_VisibleGrid);
						pDC->SetPixel(x, y, nEDA_SCH_VisibleGridColor);
					}
				}

			}
		}
	}

	if((m_bShowReferenceZones == TRUE)&&(nEDA_Print_Flag == 0))
	{
		schpolygon.m_nBorderColor = m_nSheetColor;
		schpolygon.m_nFillColor = m_nSheetColor;	
		schpolygon.m_cVertex.SetSize(4);
		schpolygon.m_cVertex[0].fx = 0;
		schpolygon.m_cVertex[0].fy = 0;
		schpolygon.m_cVertex[1].fx = width;
		schpolygon.m_cVertex[1].fy = 0;
		schpolygon.m_cVertex[2].fx = width;
		schpolygon.m_cVertex[2].fy = margin;
		schpolygon.m_cVertex[3].fx = 0;
		schpolygon.m_cVertex[3].fy = margin;
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);
		schpolygon.m_cVertex[0].fx = 0;
		schpolygon.m_cVertex[0].fy = 0;
		schpolygon.m_cVertex[1].fx = margin;
		schpolygon.m_cVertex[1].fy = 0;
		schpolygon.m_cVertex[2].fx = margin;
		schpolygon.m_cVertex[2].fy = height;
		schpolygon.m_cVertex[3].fx = 0;
		schpolygon.m_cVertex[3].fy = height;
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);
		schpolygon.m_cVertex[0].fx = width - margin;
		schpolygon.m_cVertex[0].fy = 0;
		schpolygon.m_cVertex[1].fx = width;
		schpolygon.m_cVertex[1].fy = 0;
		schpolygon.m_cVertex[2].fx = width;
		schpolygon.m_cVertex[2].fy = height;
		schpolygon.m_cVertex[3].fx = width - margin;
		schpolygon.m_cVertex[3].fy = height;
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);
		schpolygon.m_cVertex[0].fx = 0;
		schpolygon.m_cVertex[0].fy = height - margin;
		schpolygon.m_cVertex[1].fx = width;
		schpolygon.m_cVertex[1].fy = height - margin;
		schpolygon.m_cVertex[2].fx = width;
		schpolygon.m_cVertex[2].fy = height;
		schpolygon.m_cVertex[3].fx = 0;
		schpolygon.m_cVertex[3].fy = height;
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);
	}

	if(m_bShowBorder == TRUE)
	{
		schline.m_nLineType = LINE_SOLID;
		schline.m_nLineWidth = LINE_SMALL;
		schline.m_nColor = m_nBorderColor;
		schline.m_cVertex.SetSize(5);
		schline.m_cVertex[0].fx = 15;
		schline.m_cVertex[0].fy = 15;
		schline.m_cVertex[1].fx = width;
		schline.m_cVertex[1].fy = 15;
		schline.m_cVertex[2].fx = width;
		schline.m_cVertex[2].fy = height;
		schline.m_cVertex[3].fx = 15;
		schline.m_cVertex[3].fy = height;
		schline.m_cVertex[4].fx = 15;
		schline.m_cVertex[4].fy = 15;
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

		if(m_bShowReferenceZones == TRUE)
		{
			schline.m_nLineType = LINE_SOLID;
			schline.m_nLineWidth = LINE_SMALLEST;
			schline.m_nColor = m_nBorderColor;
			schline.m_cVertex[0].fx = margin;
			schline.m_cVertex[0].fy =  margin;
			schline.m_cVertex[1].fx = width - margin;
			schline.m_cVertex[1].fy = margin;
			schline.m_cVertex[2].fx = width - margin;
			schline.m_cVertex[2].fy = height - margin;
			schline.m_cVertex[3].fx = margin;
			schline.m_cVertex[3].fy = height - margin;
			schline.m_cVertex[4].fx = margin;
			schline.m_cVertex[4].fy = margin;
			if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

			schline.m_cVertex.SetSize(2);
			for(int i=1; i<xd; i++)
			{
				schline.m_cVertex[0].fx = i*width/xd;
				schline.m_cVertex[0].fy = 15;
				schline.m_cVertex[1].fx = i*width/xd;
				schline.m_cVertex[1].fy = margin;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
				schline.m_cVertex[0].fx = i*width/xd;
				schline.m_cVertex[0].fy = height;
				schline.m_cVertex[1].fx = i*width/xd;
				schline.m_cVertex[1].fy = height - margin;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
			}
			for(int i=1; i<yd; i++)
			{
				schline.m_cVertex[0].fx = 15;
				schline.m_cVertex[0].fy = i*height/yd;
				schline.m_cVertex[1].fx = margin;
				schline.m_cVertex[1].fy = i*height/yd;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);			
				schline.m_cVertex[0].fx = width;
				schline.m_cVertex[0].fy = i*height/yd;
				schline.m_cVertex[1].fx = width - margin;
				schline.m_cVertex[1].fy = i*height/yd;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
			}
			for(int i=0; i<xd; i++)
			{
				char s[10]; 
				itoa(i+1, s, 10);
				schtext.m_csText = s; 

				schtext.m_fSx = i*width/xd + width/(xd*2) - 25;
				schtext.m_fSy = margin/2 - 50;
				if(schtext.PickMe(pDC, pV) == TRUE) schtext.Draw(pDC, pV);
				schtext.m_fSx = i*width/xd + width/(xd*2) - 25;
				schtext.m_fSy = height - margin/2 - 50;
				if(schtext.PickMe(pDC, pV) == TRUE) schtext.Draw(pDC, pV);

			}
			for(int i=0; i<yd; i++)
			{
				char s[10] = "A"; 
				s[0] += i;
				schtext.m_csText = s; 

				schtext.m_fSx = margin/2 - 30;
				schtext.m_fSy = (yd-i)*height/yd - height/(yd*2) - 50;
				if(schtext.PickMe(pDC, pV) == TRUE) schtext.Draw(pDC, pV);
				
				schtext.m_fSx = width - margin/2 - 30;
				schtext.m_fSy = (yd-i)*height/yd - height/(yd*2) - 50;
				if(schtext.PickMe(pDC, pV) == TRUE) schtext.Draw(pDC, pV);
			}

		}
	}

	
	struct title_str {double fx; double fy; CString str;};

	char standard_sheet[18][10] = { "A4", "A3", "A2", "A1", "A0", "A", "B", "C", "D", "E", "Letter", "Legal", "Tabloid", "OrCAD A", "OrCAD B", "OrCAD C","OrCAD D", "OrCAD E" };

	Fpoint pt_std[8][2] = {	0,800,		3500,800,
							0,500,		3500,500,
							0,200,		3500,200,
							0,100,		3500,100,
							0,0,		0,800,
							500,200,	500,500,
							2500,200,	2500,500,
							2000,0,		2000,200 };
	title_str std_str[9] = {0,		700,	"Title",
							0,		400,	"Size",
							0,		100,	"Date:",
							0,		0,		"File: ",
							500,	400,	"Number",
							2500,	400,	"Revision",
							2000,	100,	"Sheet",
							2000,	0,		"Drawn By:",
							50,		250,	""};
	
	for(int i=0; i<m_nSheetNumberSpace; i++) std_str[6].str += " ";   
	std_str[6].str += "of";
	std_str[8].str = standard_sheet[m_nStandardStyle];

	//ansi
	Fpoint pt_ansi[11][2]={	0,1750,		6250,1750,
							2000,1250,	6250,1250,
							2000,630,	6250,630,
							2000,250,	6250,250,
							0,0,		0,1750,
							2000,0,		2000,1750,
							2380,250,	2380,630,
							3490,250,	3490,630,
							5890,250,	5890,630,
							3000,0,		3000,250,
							4500,0,		4500,250  };
	title_str ansi_str[7]={	2000,	530,	"Size",
							2000,	400,	"",
							2000,	150,	"Scale",
							2380,	530,	"FCSM No.",
							3490,	530,	"DWG No.",
							5890,	530,	"Rev",
							4500,	150,	"Sheet" };
	ansi_str[1].str = standard_sheet[m_nStandardStyle];

	if(m_bTitleBlock == TRUE)
	{
		schline.m_nLineType = LINE_SOLID;
		schline.m_nLineWidth = LINE_SMALLEST;
		schline.m_nColor = m_nBorderColor;
		schline.m_cVertex.SetSize(2);

		if(m_nTitleBlockStyle == 0)  //standard
		{
			Fpoint base_pt;
			if(m_bShowReferenceZones == TRUE)
			{
				base_pt.fx = width - 3500 - margin;
				base_pt.fy = margin;
			}
			else
			{
				base_pt.fx = width - 3500;
				base_pt.fy = 0;
			}
			for(int i=0; i < 8; i++)
			{
				schline.m_cVertex[0].fx = base_pt.fx + pt_std[i][0].fx;
				schline.m_cVertex[0].fy = base_pt.fy + pt_std[i][0].fy;
				schline.m_cVertex[1].fx = base_pt.fx + pt_std[i][1].fx;
				schline.m_cVertex[1].fy = base_pt.fy + pt_std[i][1].fy;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
			}
			for(int i=0; i<9; i++)
			{
				schtext.m_csText = std_str[i].str;
				schtext.m_fSx = base_pt.fx + std_str[i].fx + 50;
				schtext.m_fSy = base_pt.fy + std_str[i].fy;
				if(schtext.PickMe(pDC, pV) == TRUE) schtext.Draw(pDC, pV);
			}
		}
		else  //ansi
		{
			Fpoint base_pt;
			if(m_bShowReferenceZones == TRUE)
			{
				base_pt.fx = width - 6250 - margin;
				base_pt.fy = margin;
			}
			else
			{
				base_pt.fx = width - 62350;
				base_pt.fy = 0;
			}
			for(int i=0; i < 11; i++)
			{
				schline.m_cVertex[0].fx = base_pt.fx + pt_ansi[i][0].fx;
				schline.m_cVertex[0].fy = base_pt.fy + pt_ansi[i][0].fy;
				schline.m_cVertex[1].fx = base_pt.fx + pt_ansi[i][1].fx;
				schline.m_cVertex[1].fy = base_pt.fy + pt_ansi[i][1].fy;
				if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
			}
			for(int i=0; i<7; i++)
			{
				schtext.m_csText = ansi_str[i].str;
				schtext.m_fSx = base_pt.fx + ansi_str[i].fx + 50;
				schtext.m_fSy = base_pt.fy + ansi_str[i].fy;
				if(schtext.PickMe(pDC, pV) == TRUE) schtext.Draw(pDC, pV);
			}
		}
	}
}
