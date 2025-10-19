#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditSheetEntry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_schsheetentry, CObject, 0)
COb_schsheetentry::COb_schsheetentry()
{
	// This empty constructor should be used by serialization only
	m_nFlag = 0 ;
	m_bSelection = FALSE;

	pDocument = NULL;
}


void COb_schsheetentry::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		
		ar << m_nSide;
		ar << m_nStyle;
		ar << m_nType;
		ar << m_fOffset;
				
		ar << m_nFillColor;
		ar << m_nBorderColor;
		ar << m_nTextColor;
		
		ar << m_csName;
		
		ar << m_nDiagram;
		ar << m_bSelection;
	}
	else
	{
		
		ar >> m_nSide;
		ar >> m_nStyle;
		ar >> m_nType;
		ar >> m_fOffset;
				
		ar >> m_nFillColor;
		ar >> m_nBorderColor;
		ar >> m_nTextColor;
		
		ar >> m_csName;
		
		ar >> m_nDiagram;
		ar >> m_bSelection;	
	}
}

void COb_schsheetentry::Draw(CDC* pDC, CView* pV)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	CESDView* pView = (CESDView*)pV;

	int entry_mode; //ARROW_LEFT ARROW_RIGHT ARROW_TOP ARROW_BOTTOM ARROW_LEFT_RIGHT ARROW_TOP_BOTTOM NOARROW_LEFT_RIGHT NOARROW_TOP_BOTTOM

	if(m_nType == ENTRY_TYPE_OUTPUT)
	{
		if(m_nSide == ENTRY_SIDE_LEFT) entry_mode = ARROW_LEFT;
		else if(m_nSide == ENTRY_SIDE_RIGHT) entry_mode = ARROW_RIGHT;
		else if(m_nSide == ENTRY_SIDE_TOP) entry_mode = ARROW_TOP;
		else if(m_nSide == ENTRY_SIDE_BOTTOM) entry_mode = ARROW_BOTTOM;
	}
	else if(m_nType == ENTRY_TYPE_INPUT)
	{
		if(m_nSide == ENTRY_SIDE_LEFT) entry_mode = ARROW_RIGHT;
		else if(m_nSide == ENTRY_SIDE_RIGHT) entry_mode = ARROW_LEFT;
		else if(m_nSide == ENTRY_SIDE_TOP) entry_mode = ARROW_BOTTOM;
		else if(m_nSide == ENTRY_SIDE_BOTTOM) entry_mode = ARROW_TOP;
	}
	else if(m_nType == ENTRY_TYPE_BIDIRECTIONAL)
	{
		if((m_nSide == ENTRY_SIDE_LEFT) || (m_nSide == ENTRY_SIDE_RIGHT)) entry_mode = ARROW_LEFT_RIGHT;
		else if((m_nSide == ENTRY_SIDE_TOP) || (m_nSide == ENTRY_SIDE_BOTTOM)) entry_mode = ARROW_TOP_BOTTOM;
	}
	else
	{
		if((m_nStyle == ENTRY_STYLE_NH) || (m_nStyle == ENTRY_STYLE_NV))
		{
			if((m_nSide == ENTRY_SIDE_LEFT) || (m_nSide == ENTRY_SIDE_RIGHT)) entry_mode = NOARROW_LEFT_RIGHT;
			else if((m_nSide == ENTRY_SIDE_TOP) || (m_nSide == ENTRY_SIDE_BOTTOM)) entry_mode = NOARROW_TOP_BOTTOM;
		}
		else if((m_nStyle == ENTRY_STYLE_TOPBOTTOM) || (m_nStyle == ENTRY_STYLE_LEFTRIGHT))
		{
			if((m_nSide == ENTRY_SIDE_LEFT) || (m_nSide == ENTRY_SIDE_RIGHT)) entry_mode = ARROW_LEFT_RIGHT;
			else if((m_nSide == ENTRY_SIDE_TOP) || (m_nSide == ENTRY_SIDE_BOTTOM)) entry_mode = ARROW_TOP_BOTTOM;
		}
		else if(m_nStyle == ENTRY_STYLE_LEFT)
		{
			if((m_nSide == ENTRY_SIDE_LEFT) || (m_nSide == ENTRY_SIDE_RIGHT)) entry_mode = ARROW_LEFT;
			else if((m_nSide == ENTRY_SIDE_TOP) || (m_nSide == ENTRY_SIDE_BOTTOM)) entry_mode = ARROW_TOP;
		}
		else if(m_nStyle == ENTRY_STYLE_RIGHT)
		{
			if((m_nSide == ENTRY_SIDE_LEFT) || (m_nSide == ENTRY_SIDE_RIGHT)) entry_mode = ARROW_RIGHT;
			else if((m_nSide == ENTRY_SIDE_TOP) || (m_nSide == ENTRY_SIDE_BOTTOM)) entry_mode = ARROW_BOTTOM;
		}
		else if(m_nStyle == ENTRY_STYLE_TOP)
		{
			if((m_nSide == ENTRY_SIDE_LEFT) || (m_nSide == ENTRY_SIDE_RIGHT)) entry_mode = ARROW_RIGHT;
			else if((m_nSide == ENTRY_SIDE_TOP) || (m_nSide == ENTRY_SIDE_BOTTOM)) entry_mode = ARROW_TOP;
		}
		else if(m_nStyle == ENTRY_STYLE_BOTTOM)
		{
			if((m_nSide == ENTRY_SIDE_LEFT) || (m_nSide == ENTRY_SIDE_RIGHT)) entry_mode = ARROW_LEFT;
			else if((m_nSide == ENTRY_SIDE_TOP) || (m_nSide == ENTRY_SIDE_BOTTOM)) entry_mode = ARROW_BOTTOM;
		}
	}

	
	CObject* pobject;
	COb_schsheetsymbol* psheetsymbol;

	pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_posParent);
	if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
	{
		psheetsymbol = (COb_schsheetsymbol*)pobject;
	}
	else 
	{
		TRACE("Get sheet entry parent error.\n");
		return;
	}

	double x1,y1,x2,y2,offset;
	x1 = psheetsymbol->m_fX;
	y1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize;
	x2 = psheetsymbol->m_fX + psheetsymbol->m_fXsize;
	y2 = psheetsymbol->m_fY;
	offset = m_fOffset;

	//SheetEntry 的填充区外形
	int vertex_num;
	Fpoint p[6], hotpoint;

	if(entry_mode == ARROW_LEFT)
	{
		if(m_nSide == ENTRY_SIDE_LEFT)
		{
			hotpoint.fx = x1;
			hotpoint.fy = y2 - offset;
			vertex_num = 5;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx + 40;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx + 150;
			p[2].fy = hotpoint.fy - 40;
			p[3].fx = hotpoint.fx + 150;
			p[3].fy = hotpoint.fy + 40;
			p[4].fx = hotpoint.fx + 40;
			p[4].fy = hotpoint.fy + 40;
		}
		else
		{
			hotpoint.fx = x2;
			hotpoint.fy = y2 - offset;
			vertex_num = 5;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy - 40;
			p[1].fx = hotpoint.fx - 110;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx - 150;
			p[2].fy = hotpoint.fy;
			p[3].fx = hotpoint.fx - 110;
			p[3].fy = hotpoint.fy + 40;
			p[4].fx = hotpoint.fx;
			p[4].fy = hotpoint.fy + 40;
		}
	}
	else if(entry_mode == ARROW_RIGHT)
	{
		if(m_nSide == ENTRY_SIDE_LEFT)
		{
			hotpoint.fx = x1;
			hotpoint.fy = y2 - offset;
			vertex_num = 5;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy - 40;
			p[1].fx = hotpoint.fx + 110;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx + 150;
			p[2].fy = hotpoint.fy;
			p[3].fx = hotpoint.fx + 110;
			p[3].fy = hotpoint.fy + 40;
			p[4].fx = hotpoint.fx;
			p[4].fy = hotpoint.fy + 40;
		}
		else
		{
			hotpoint.fx = x2;
			hotpoint.fy = y2 - offset;
			vertex_num = 5;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx - 150;
			p[2].fy = hotpoint.fy - 40;
			p[3].fx = hotpoint.fx - 150;
			p[3].fy = hotpoint.fy + 40;
			p[4].fx = hotpoint.fx - 40;
			p[4].fy = hotpoint.fy + 40;
		}
	}
	else if(entry_mode == ARROW_LEFT_RIGHT)
	{
		if(m_nSide == ENTRY_SIDE_LEFT)
		{
			hotpoint.fx = x1;
			hotpoint.fy = y2 - offset;
			vertex_num = 6;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx + 40;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx + 110;
			p[2].fy = hotpoint.fy - 40;
			p[3].fx = hotpoint.fx + 150;
			p[3].fy = hotpoint.fy ;
			p[4].fx = hotpoint.fx + 110;
			p[4].fy = hotpoint.fy + 40;
			p[5].fx = hotpoint.fx + 40;
			p[5].fy = hotpoint.fy + 40;
		}
		else
		{
			hotpoint.fx = x2;
			hotpoint.fy = y2 - offset;
			vertex_num = 6;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx - 110;
			p[2].fy = hotpoint.fy - 40;
			p[3].fx = hotpoint.fx - 150;
			p[3].fy = hotpoint.fy ;
			p[4].fx = hotpoint.fx - 110;
			p[4].fy = hotpoint.fy + 40;
			p[5].fx = hotpoint.fx - 40;
			p[5].fy = hotpoint.fy + 40;
		}
	}
	else if(entry_mode == NOARROW_LEFT_RIGHT)
	{
		if(m_nSide == ENTRY_SIDE_LEFT)
		{
			hotpoint.fx = x1;
			hotpoint.fy = y2 - offset;
			vertex_num = 4;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy - 40;
			p[1].fx = hotpoint.fx + 150;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx + 150;
			p[2].fy = hotpoint.fy + 40;
			p[3].fx = hotpoint.fx;
			p[3].fy = hotpoint.fy + 40;
		}
		else
		{
			hotpoint.fx = x2;
			hotpoint.fy = y2 - offset;
			vertex_num = 4;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy - 40;
			p[1].fx = hotpoint.fx - 150;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx - 150;
			p[2].fy = hotpoint.fy + 40;
			p[3].fx = hotpoint.fx;
			p[3].fy = hotpoint.fy + 40;
		}
	}


	else if(entry_mode == ARROW_TOP)
	{
		if(m_nSide == ENTRY_SIDE_TOP)
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y2;
			vertex_num = 5;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx - 40;
			p[2].fy = hotpoint.fy - 150;
			p[3].fx = hotpoint.fx + 40;
			p[3].fy = hotpoint.fy - 150;
			p[4].fx = hotpoint.fx + 40;
			p[4].fy = hotpoint.fy - 40;
		}
		else
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y1;
			vertex_num = 5;
			p[0].fx = hotpoint.fx - 40;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy + 110;
			p[2].fx = hotpoint.fx;
			p[2].fy = hotpoint.fy + 150;
			p[3].fx = hotpoint.fx + 40;
			p[3].fy = hotpoint.fy + 110;
			p[4].fx = hotpoint.fx + 40;
			p[4].fy = hotpoint.fy;
		}
	}
	else if(entry_mode == ARROW_BOTTOM)
	{
		if(m_nSide == ENTRY_SIDE_TOP)
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y2;
			vertex_num = 5;
			p[0].fx = hotpoint.fx - 40;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy - 110;
			p[2].fx = hotpoint.fx;
			p[2].fy = hotpoint.fy - 150;
			p[3].fx = hotpoint.fx + 40;
			p[3].fy = hotpoint.fy - 110;
			p[4].fx = hotpoint.fx + 40;
			p[4].fy = hotpoint.fy;
		}
		else
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y1;
			vertex_num = 5;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy + 40;
			p[2].fx = hotpoint.fx - 40;
			p[2].fy = hotpoint.fy + 150;
			p[3].fx = hotpoint.fx + 40;
			p[3].fy = hotpoint.fy + 150;
			p[4].fx = hotpoint.fx + 40;
			p[4].fy = hotpoint.fy + 40;
		}
	}
	else if(entry_mode == ARROW_TOP_BOTTOM)
	{
		if(m_nSide == ENTRY_SIDE_TOP)
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y2;
			vertex_num = 6;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx - 40;
			p[2].fy = hotpoint.fy - 110;
			p[3].fx = hotpoint.fx ;
			p[3].fy = hotpoint.fy - 150;
			p[4].fx = hotpoint.fx + 40;
			p[4].fy = hotpoint.fy - 110;
			p[5].fx = hotpoint.fx + 40;
			p[5].fy = hotpoint.fy - 40;
		}
		else
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y1;
			vertex_num = 6;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy + 40;
			p[2].fx = hotpoint.fx - 40;
			p[2].fy = hotpoint.fy + 110;
			p[3].fx = hotpoint.fx ;
			p[3].fy = hotpoint.fy + 150;
			p[4].fx = hotpoint.fx + 40;
			p[4].fy = hotpoint.fy + 110;
			p[5].fx = hotpoint.fx + 40;
			p[5].fy = hotpoint.fy + 40;
		}
	}
	else if(entry_mode == NOARROW_TOP_BOTTOM)
	{
		if(m_nSide == ENTRY_SIDE_TOP)
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y2;
			vertex_num = 4;
			p[0].fx = hotpoint.fx - 40;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy - 150;
			p[2].fx = hotpoint.fx + 40;
			p[2].fy = hotpoint.fy - 150;
			p[3].fx = hotpoint.fx + 40;
			p[3].fy = hotpoint.fy;
		}
		else
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y1;
			vertex_num = 4;
			p[0].fx = hotpoint.fx - 40;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy + 150;
			p[2].fx = hotpoint.fx + 40;
			p[2].fy = hotpoint.fy + 150;
			p[3].fx = hotpoint.fx + 40;
			p[3].fy = hotpoint.fy;
		}
	}

	COb_schpolygon schpolygon;
	schpolygon.m_nFlag = m_nFlag;
	schpolygon.m_nDiagram = m_nDiagram;
	schpolygon.m_nBorderWidth = 0;
	schpolygon.m_nBorderColor = m_nBorderColor;
	schpolygon.m_nFillColor = m_nFillColor;
	schpolygon.m_bFillSolid = TRUE;
	schpolygon.m_bTransparent = FALSE;
	schpolygon.m_cVertex.SetSize(vertex_num);
	for(int i=0; i<vertex_num; i++)
	{
		schpolygon.m_cVertex[i].fx = p[i].fx;
		schpolygon.m_cVertex[i].fy = p[i].fy;
	}
	
	if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);


	COb_schtext schtext;
	schtext.m_nFlag = m_nFlag;
	schtext.m_nDiagram = m_nDiagram;
	schtext.m_nColor = m_nTextColor;
	schtext.m_nFont = 0;
	schtext.m_nOrientation = 0;
	schtext.m_csText = m_csName;
	//offset = (double)schtext.GetSignalExtent(pDC, pV);

	if(m_nSide == ENTRY_SIDE_LEFT) 
	{
		schtext.m_fSx = psheetsymbol->m_fX + 200.0f;
		schtext.m_fSy = psheetsymbol->m_fY - m_fOffset - 50.0f;
		schtext.m_nH_Justification = 0;
	}
	else if(m_nSide == ENTRY_SIDE_RIGHT) 
	{
		schtext.m_fSx = psheetsymbol->m_fX + psheetsymbol->m_fXsize - 200.0f; // - offset*pView->m_fScale;
		schtext.m_fSy = psheetsymbol->m_fY - m_fOffset - 50.0f;
		schtext.m_nH_Justification = 2;
	}
	else if(m_nSide == ENTRY_SIDE_TOP) 
	{
		schtext.m_nOrientation = 90;
		schtext.m_fSx = psheetsymbol->m_fX + m_fOffset + 50.0f;
		schtext.m_fSy = psheetsymbol->m_fY - 200.0f; // - offset*pView->m_fScale;
		schtext.m_nH_Justification = 2;
	}
	else if(m_nSide == ENTRY_SIDE_BOTTOM) 
	{
		schtext.m_nOrientation = 90;
		schtext.m_fSx = psheetsymbol->m_fX + m_fOffset + 50.0f;
		schtext.m_fSy = psheetsymbol->m_fY - psheetsymbol->m_fYsize + 200.0f;
		schtext.m_nH_Justification = 0;
	}
	
	if(schtext.PickMe(pDC, pV) == TRUE) schtext.DrawSignal(pDC, pV);

}

void COb_schsheetentry::DrawSelection(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	CPen arcpen;
	arcpen.CreatePen(PS_DOT, 1, nEDA_SelectionColor);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pDoc->m_listSchObject[m_nDiagram].GetAt(m_posParent);

	double fminx, fminy, fmaxx, fmaxy;
	int x1,y1,x2,y2;
	if(m_nSide == ENTRY_SIDE_LEFT) 
	{
		fminx = psheetsymbol->m_fX;
		fminy = psheetsymbol->m_fY - m_fOffset  - 45;
		fmaxx = psheetsymbol->m_fX + 150;
		fmaxy = psheetsymbol->m_fY - m_fOffset  + 45;
	}
	else if(m_nSide == ENTRY_SIDE_RIGHT) 
	{
		fminx = psheetsymbol->m_fX +  psheetsymbol->m_fXsize - 150;
		fminy = psheetsymbol->m_fY - m_fOffset  - 45;
		fmaxx = psheetsymbol->m_fX +  psheetsymbol->m_fXsize;
		fmaxy = psheetsymbol->m_fY - m_fOffset  + 45;
	}
	else if(m_nSide == ENTRY_SIDE_TOP) 
	{
		fminx = psheetsymbol->m_fX +  m_fOffset - 45;
		fminy = psheetsymbol->m_fY - 150;
		fmaxx = psheetsymbol->m_fX +  m_fOffset + 45;
		fmaxy = psheetsymbol->m_fY;
	}
	else if(m_nSide == ENTRY_SIDE_BOTTOM) 
	{
		fminx = psheetsymbol->m_fX +  m_fOffset - 45;
		fminy = psheetsymbol->m_fY - psheetsymbol->m_fYsize;
		fmaxx = psheetsymbol->m_fX +  m_fOffset + 45;
		fmaxy = psheetsymbol->m_fY - psheetsymbol->m_fYsize + 150;
	}
	
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

BOOL  COb_schsheetentry::Is_On_Me(double fx, double fy)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	//if(diagram != m_nDiagram) return FALSE;

	int entry_mode; //ARROW_LEFT ARROW_RIGHT ARROW_TOP ARROW_BOTTOM ARROW_LEFT_RIGHT ARROW_TOP_BOTTOM NOARROW_LEFT_RIGHT NOARROW_TOP_BOTTOM

	if(m_nType == ENTRY_TYPE_OUTPUT)
	{
		if(m_nSide == ENTRY_SIDE_LEFT) entry_mode = ARROW_LEFT;
		else if(m_nSide == ENTRY_SIDE_RIGHT) entry_mode = ARROW_RIGHT;
		else if(m_nSide == ENTRY_SIDE_TOP) entry_mode = ARROW_TOP;
		else if(m_nSide == ENTRY_SIDE_BOTTOM) entry_mode = ARROW_BOTTOM;
	}
	else if(m_nType == ENTRY_TYPE_INPUT)
	{
		if(m_nSide == ENTRY_SIDE_LEFT) entry_mode = ARROW_RIGHT;
		else if(m_nSide == ENTRY_SIDE_RIGHT) entry_mode = ARROW_LEFT;
		else if(m_nSide == ENTRY_SIDE_TOP) entry_mode = ARROW_BOTTOM;
		else if(m_nSide == ENTRY_SIDE_BOTTOM) entry_mode = ARROW_TOP;
	}
	else if(m_nType == ENTRY_TYPE_BIDIRECTIONAL)
	{
		if((m_nSide == ENTRY_SIDE_LEFT) || (m_nSide == ENTRY_SIDE_RIGHT)) entry_mode = ARROW_LEFT_RIGHT;
		else if((m_nSide == ENTRY_SIDE_TOP) || (m_nSide == ENTRY_SIDE_BOTTOM)) entry_mode = ARROW_TOP_BOTTOM;
	}
	else
	{
		if((m_nStyle == ENTRY_STYLE_NH) || (m_nStyle == ENTRY_STYLE_NV))
		{
			if((m_nSide == ENTRY_SIDE_LEFT) || (m_nSide == ENTRY_SIDE_RIGHT)) entry_mode = NOARROW_LEFT_RIGHT;
			else if((m_nSide == ENTRY_SIDE_TOP) || (m_nSide == ENTRY_SIDE_BOTTOM)) entry_mode = NOARROW_TOP_BOTTOM;
		}
		else if((m_nStyle == ENTRY_STYLE_TOPBOTTOM) || (m_nStyle == ENTRY_STYLE_LEFTRIGHT))
		{
			if((m_nSide == ENTRY_SIDE_LEFT) || (m_nSide == ENTRY_SIDE_RIGHT)) entry_mode = ARROW_LEFT_RIGHT;
			else if((m_nSide == ENTRY_SIDE_TOP) || (m_nSide == ENTRY_SIDE_BOTTOM)) entry_mode = ARROW_TOP_BOTTOM;
		}
		else if(m_nStyle == ENTRY_STYLE_LEFT)
		{
			if((m_nSide == ENTRY_SIDE_LEFT) || (m_nSide == ENTRY_SIDE_RIGHT)) entry_mode = ARROW_LEFT;
			else if((m_nSide == ENTRY_SIDE_TOP) || (m_nSide == ENTRY_SIDE_BOTTOM)) entry_mode = ARROW_TOP;
		}
		else if(m_nStyle == ENTRY_STYLE_RIGHT)
		{
			if((m_nSide == ENTRY_SIDE_LEFT) || (m_nSide == ENTRY_SIDE_RIGHT)) entry_mode = ARROW_RIGHT;
			else if((m_nSide == ENTRY_SIDE_TOP) || (m_nSide == ENTRY_SIDE_BOTTOM)) entry_mode = ARROW_BOTTOM;
		}
		else if(m_nStyle == ENTRY_STYLE_TOP)
		{
			if((m_nSide == ENTRY_SIDE_LEFT) || (m_nSide == ENTRY_SIDE_RIGHT)) entry_mode = ARROW_RIGHT;
			else if((m_nSide == ENTRY_SIDE_TOP) || (m_nSide == ENTRY_SIDE_BOTTOM)) entry_mode = ARROW_TOP;
		}
		else if(m_nStyle == ENTRY_STYLE_BOTTOM)
		{
			if((m_nSide == ENTRY_SIDE_LEFT) || (m_nSide == ENTRY_SIDE_RIGHT)) entry_mode = ARROW_LEFT;
			else if((m_nSide == ENTRY_SIDE_TOP) || (m_nSide == ENTRY_SIDE_BOTTOM)) entry_mode = ARROW_BOTTOM;
		}
	}

	CObject* pobject;
	COb_schsheetsymbol* psheetsymbol;

	pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_posParent);
	if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
	{
		psheetsymbol = (COb_schsheetsymbol*)pobject;
	}
	else 
	{
		TRACE("Get sheet entry parent error.\n");
		return FALSE;
	}

	double x1,y1,x2,y2,offset;
	x1 = psheetsymbol->m_fX;
	y1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize;
	x2 = psheetsymbol->m_fX + psheetsymbol->m_fXsize;
	y2 = psheetsymbol->m_fY;
	offset = m_fOffset;

	int vertex_num;
	Fpoint p[6], hotpoint;

	if(entry_mode == ARROW_LEFT)
	{
		if(m_nSide == ENTRY_SIDE_LEFT)
		{
			hotpoint.fx = x1;
			hotpoint.fy = y2 - offset;
			vertex_num = 5;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx + 40;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx + 150;
			p[2].fy = hotpoint.fy - 40;
			p[3].fx = hotpoint.fx + 150;
			p[3].fy = hotpoint.fy + 40;
			p[4].fx = hotpoint.fx + 40;
			p[4].fy = hotpoint.fy + 40;
		}
		else
		{
			hotpoint.fx = x2;
			hotpoint.fy = y2 - offset;
			vertex_num = 5;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy - 40;
			p[1].fx = hotpoint.fx - 110;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx - 150;
			p[2].fy = hotpoint.fy;
			p[3].fx = hotpoint.fx - 110;
			p[3].fy = hotpoint.fy + 40;
			p[4].fx = hotpoint.fx;
			p[4].fy = hotpoint.fy + 40;
		}
	}
	else if(entry_mode == ARROW_RIGHT)
	{
		if(m_nSide == ENTRY_SIDE_LEFT)
		{
			hotpoint.fx = x1;
			hotpoint.fy = y2 - offset;
			vertex_num = 5;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy - 40;
			p[1].fx = hotpoint.fx + 110;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx + 150;
			p[2].fy = hotpoint.fy;
			p[3].fx = hotpoint.fx + 110;
			p[3].fy = hotpoint.fy + 40;
			p[4].fx = hotpoint.fx;
			p[4].fy = hotpoint.fy + 40;
		}
		else
		{
			hotpoint.fx = x2;
			hotpoint.fy = y2 - offset;
			vertex_num = 5;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx - 150;
			p[2].fy = hotpoint.fy - 40;
			p[3].fx = hotpoint.fx - 150;
			p[3].fy = hotpoint.fy + 40;
			p[4].fx = hotpoint.fx - 40;
			p[4].fy = hotpoint.fy + 40;
		}
	}
	else if(entry_mode == ARROW_LEFT_RIGHT)
	{
		if(m_nSide == ENTRY_SIDE_LEFT)
		{
			hotpoint.fx = x1;
			hotpoint.fy = y2 - offset;
			vertex_num = 6;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx + 40;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx + 110;
			p[2].fy = hotpoint.fy - 40;
			p[3].fx = hotpoint.fx + 150;
			p[3].fy = hotpoint.fy ;
			p[4].fx = hotpoint.fx + 110;
			p[4].fy = hotpoint.fy + 40;
			p[5].fx = hotpoint.fx + 40;
			p[5].fy = hotpoint.fy + 40;
		}
		else
		{
			hotpoint.fx = x2;
			hotpoint.fy = y2 - offset;
			vertex_num = 6;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx - 110;
			p[2].fy = hotpoint.fy - 40;
			p[3].fx = hotpoint.fx - 150;
			p[3].fy = hotpoint.fy ;
			p[4].fx = hotpoint.fx - 110;
			p[4].fy = hotpoint.fy + 40;
			p[5].fx = hotpoint.fx - 40;
			p[5].fy = hotpoint.fy + 40;
		}
	}
	else if(entry_mode == NOARROW_LEFT_RIGHT)
	{
		if(m_nSide == ENTRY_SIDE_LEFT)
		{
			hotpoint.fx = x1;
			hotpoint.fy = y2 - offset;
			vertex_num = 4;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy - 40;
			p[1].fx = hotpoint.fx + 150;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx + 150;
			p[2].fy = hotpoint.fy + 40;
			p[3].fx = hotpoint.fx;
			p[3].fy = hotpoint.fy + 40;
		}
		else
		{
			hotpoint.fx = x2;
			hotpoint.fy = y2 - offset;
			vertex_num = 4;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy - 40;
			p[1].fx = hotpoint.fx - 150;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx - 150;
			p[2].fy = hotpoint.fy + 40;
			p[3].fx = hotpoint.fx;
			p[3].fy = hotpoint.fy + 40;
		}
	}


	else if(entry_mode == ARROW_TOP)
	{
		if(m_nSide == ENTRY_SIDE_TOP)
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y2;
			vertex_num = 5;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx - 40;
			p[2].fy = hotpoint.fy - 150;
			p[3].fx = hotpoint.fx + 40;
			p[3].fy = hotpoint.fy - 150;
			p[4].fx = hotpoint.fx + 40;
			p[4].fy = hotpoint.fy - 40;
		}
		else
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y1;
			vertex_num = 5;
			p[0].fx = hotpoint.fx - 40;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy + 110;
			p[2].fx = hotpoint.fx;
			p[2].fy = hotpoint.fy + 150;
			p[3].fx = hotpoint.fx + 40;
			p[3].fy = hotpoint.fy + 110;
			p[4].fx = hotpoint.fx + 40;
			p[4].fy = hotpoint.fy;
		}
	}
	else if(entry_mode == ARROW_BOTTOM)
	{
		if(m_nSide == ENTRY_SIDE_TOP)
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y2;
			vertex_num = 5;
			p[0].fx = hotpoint.fx - 40;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy - 110;
			p[2].fx = hotpoint.fx;
			p[2].fy = hotpoint.fy - 150;
			p[3].fx = hotpoint.fx + 40;
			p[3].fy = hotpoint.fy - 110;
			p[4].fx = hotpoint.fx + 40;
			p[4].fy = hotpoint.fy;
		}
		else
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y1;
			vertex_num = 5;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy + 40;
			p[2].fx = hotpoint.fx - 40;
			p[2].fy = hotpoint.fy + 150;
			p[3].fx = hotpoint.fx + 40;
			p[3].fy = hotpoint.fy + 150;
			p[4].fx = hotpoint.fx + 40;
			p[4].fy = hotpoint.fy + 40;
		}
	}
	else if(entry_mode == ARROW_TOP_BOTTOM)
	{
		if(m_nSide == ENTRY_SIDE_TOP)
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y2;
			vertex_num = 6;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy - 40;
			p[2].fx = hotpoint.fx - 40;
			p[2].fy = hotpoint.fy - 110;
			p[3].fx = hotpoint.fx ;
			p[3].fy = hotpoint.fy - 150;
			p[4].fx = hotpoint.fx + 40;
			p[4].fy = hotpoint.fy - 110;
			p[5].fx = hotpoint.fx + 40;
			p[5].fy = hotpoint.fy - 40;
		}
		else
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y1;
			vertex_num = 6;
			p[0].fx = hotpoint.fx;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy + 40;
			p[2].fx = hotpoint.fx - 40;
			p[2].fy = hotpoint.fy + 110;
			p[3].fx = hotpoint.fx ;
			p[3].fy = hotpoint.fy + 150;
			p[4].fx = hotpoint.fx + 40;
			p[4].fy = hotpoint.fy + 110;
			p[5].fx = hotpoint.fx + 40;
			p[5].fy = hotpoint.fy + 40;
		}
	}
	else if(entry_mode == NOARROW_TOP_BOTTOM)
	{
		if(m_nSide == ENTRY_SIDE_TOP)
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y2;
			vertex_num = 4;
			p[0].fx = hotpoint.fx - 40;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy - 150;
			p[2].fx = hotpoint.fx + 40;
			p[2].fy = hotpoint.fy - 150;
			p[3].fx = hotpoint.fx + 40;
			p[3].fy = hotpoint.fy;
		}
		else
		{
			hotpoint.fx = x1 + offset;
			hotpoint.fy = y1;
			vertex_num = 4;
			p[0].fx = hotpoint.fx - 40;
			p[0].fy = hotpoint.fy;
			p[1].fx = hotpoint.fx - 40;
			p[1].fy = hotpoint.fy + 150;
			p[2].fx = hotpoint.fx + 40;
			p[2].fy = hotpoint.fy + 150;
			p[3].fx = hotpoint.fx + 40;
			p[3].fy = hotpoint.fy;
		}
	}

	CPoint cpoint[6];
	for(int i=0; i<6; i++)
	{
		cpoint[i].x = (int)p[i].fx;
		cpoint[i].y = (int)p[i].fy;
	}

	CRgn   rgn;

	rgn.CreatePolygonRgn(cpoint, vertex_num, ALTERNATE);

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

void COb_schsheetentry::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_EditSheetEntry dlg;

	dlg.m_csEditPosition = pDoc->Convert_XY_To_String(m_fOffset);
	
	dlg.m_nFillColor = m_nFillColor;
	dlg.m_nBorderColor = m_nBorderColor;
	dlg.m_nTextColor = m_nTextColor;
	
	dlg.m_csEditName = m_csName;
	
	dlg.m_nComboSide = m_nSide;
	dlg.m_nComboStyle = m_nStyle;
	dlg.m_nComboType = m_nType;



	if(dlg.DoModal() == IDOK)
	{
		if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = SCH_ELEMENT_SHEETENTRY;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = pos;
		pundo->schsheetentry.Copy(this);
			
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

		m_fOffset = pDoc->Convert_String_To_XY(dlg.m_csEditPosition);
		
		m_nFillColor = dlg.m_nFillColor;
		m_nBorderColor = dlg.m_nBorderColor;
		m_nTextColor = dlg.m_nTextColor;
		
		m_csName = dlg.m_csEditName;
		
		m_nSide = dlg.m_nComboSide;
		m_nStyle = dlg.m_nComboStyle;
		m_nType = dlg.m_nComboType;
	}
}

void COb_schsheetentry::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	int flag;
	double x1,y1;
	BOOL clear_undo_buffer_flag = FALSE;
	BOOL undo_flag = FALSE; 
	
	char szHelp[] = "移动子图入口: <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	CObject* pobject;
	COb_schsheetsymbol* psheetsymbol;
	pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_posParent);
	if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
	{
		psheetsymbol = (COb_schsheetsymbol*)pobject;
	}
	else 
	{
		TRACE("Get sheet entry parent error.\n");
		return;
	}

	COLORREF colorbak = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor;
	if(psheetsymbol->m_bFillSolid == TRUE) pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor = psheetsymbol->m_nFillColor;
	

	pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV);
	
	if(psheetsymbol->m_bFillSolid == TRUE) pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor = colorbak;

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_schsheetentry asheetentry;
	asheetentry.pDocument = pDoc;
	asheetentry.m_nSide = m_nSide;
	asheetentry.m_nStyle = m_nStyle;
	asheetentry.m_nType = m_nType;
	asheetentry.m_fOffset = m_fOffset;
	asheetentry.m_nFillColor = m_nFillColor;
	asheetentry.m_nBorderColor = m_nBorderColor;
	asheetentry.m_nTextColor = m_nTextColor;
	asheetentry.m_csName.Empty();
	asheetentry.m_nDiagram = m_nDiagram;
	asheetentry.m_posParent = m_posParent;

	asheetentry.Draw(pDC, pV);
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
			asheetentry.Draw(pDC, pV);
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
				asheetentry.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;

			if(x1 < psheetsymbol->m_fX) 
			{
				x1 = psheetsymbol->m_fX;
				asheetentry.m_nSide = ENTRY_SIDE_LEFT;
				asheetentry.m_fOffset = psheetsymbol->m_fY - y1;
			}
			if(x1 > psheetsymbol->m_fX + psheetsymbol->m_fXsize)
			{
				x1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize;
				asheetentry.m_nSide = ENTRY_SIDE_RIGHT;
				asheetentry.m_fOffset = psheetsymbol->m_fY - y1;
			}
			if(y1 > psheetsymbol->m_fY)
			{
				y1 = psheetsymbol->m_fY;
				asheetentry.m_nSide = ENTRY_SIDE_TOP;
				asheetentry.m_fOffset = x1 - psheetsymbol->m_fX;
			}
			if(y1 < psheetsymbol->m_fY - psheetsymbol->m_fYsize)
			{
				asheetentry.m_nSide = ENTRY_SIDE_BOTTOM;
				asheetentry.m_fOffset = x1 - psheetsymbol->m_fX;
				y1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize;
			}
			
			if((x1 > psheetsymbol->m_fX)&&(x1 < psheetsymbol->m_fX + psheetsymbol->m_fXsize)&&(y1 < psheetsymbol->m_fY)&&(y1 > psheetsymbol->m_fY - psheetsymbol->m_fYsize))
			{
				double fxmin = MIN((x1 - psheetsymbol->m_fX), (psheetsymbol->m_fX + psheetsymbol->m_fXsize - x1));
				double fymin = MIN((psheetsymbol->m_fY - y1), (y1 - (psheetsymbol->m_fY - psheetsymbol->m_fYsize)));
				if(fxmin < fymin)
				{
					if((x1 - psheetsymbol->m_fX) < (psheetsymbol->m_fX + psheetsymbol->m_fXsize - x1))
					{
						x1 = psheetsymbol->m_fX;
						asheetentry.m_nSide = ENTRY_SIDE_LEFT;
						asheetentry.m_fOffset = psheetsymbol->m_fY - y1;
					}
					else
					{
						x1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize;
						asheetentry.m_nSide = ENTRY_SIDE_RIGHT;
						asheetentry.m_fOffset = psheetsymbol->m_fY - y1;
					}
				}
				else
				{
					if((psheetsymbol->m_fY - y1) < (y1 - (psheetsymbol->m_fY - psheetsymbol->m_fYsize)))
					{
						y1 = psheetsymbol->m_fY;
						asheetentry.m_nSide = ENTRY_SIDE_TOP;
						asheetentry.m_fOffset = x1 - psheetsymbol->m_fX;
					}
					else 
					{
						y1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize;
						asheetentry.m_nSide = ENTRY_SIDE_BOTTOM;
						asheetentry.m_fOffset = x1 - psheetsymbol->m_fX;
					}
				}
			}
			
			pView->CloseMouseCursor();
			asheetentry.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			asheetentry.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;

			if(x1 < psheetsymbol->m_fX) 
			{
				x1 = psheetsymbol->m_fX;
				asheetentry.m_nSide = ENTRY_SIDE_LEFT;
				asheetentry.m_fOffset = psheetsymbol->m_fY - y1;
			}
			if(x1 > psheetsymbol->m_fX + psheetsymbol->m_fXsize)
			{
				x1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize;
				asheetentry.m_nSide = ENTRY_SIDE_RIGHT;
				asheetentry.m_fOffset = psheetsymbol->m_fY - y1;
			}
			if(y1 > psheetsymbol->m_fY)
			{
				y1 = psheetsymbol->m_fY;
				asheetentry.m_nSide = ENTRY_SIDE_TOP;
				asheetentry.m_fOffset = x1 - psheetsymbol->m_fX;
			}
			if(y1 < psheetsymbol->m_fY - psheetsymbol->m_fYsize)
			{
				asheetentry.m_nSide = ENTRY_SIDE_BOTTOM;
				asheetentry.m_fOffset = x1 - psheetsymbol->m_fX;
				y1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize;
			}
			
			if((x1 > psheetsymbol->m_fX)&&(x1 < psheetsymbol->m_fX + psheetsymbol->m_fXsize)&&(y1 < psheetsymbol->m_fY)&&(y1 > psheetsymbol->m_fY - psheetsymbol->m_fYsize))
			{
				double fxmin = MIN((x1 - psheetsymbol->m_fX), (psheetsymbol->m_fX + psheetsymbol->m_fXsize - x1));
				double fymin = MIN((psheetsymbol->m_fY - y1), (y1 - (psheetsymbol->m_fY - psheetsymbol->m_fYsize)));
				if(fxmin < fymin)
				{
					if((x1 - psheetsymbol->m_fX) < (psheetsymbol->m_fX + psheetsymbol->m_fXsize - x1))
					{
						x1 = psheetsymbol->m_fX;
						asheetentry.m_nSide = ENTRY_SIDE_LEFT;
						asheetentry.m_fOffset = psheetsymbol->m_fY - y1;
					}
					else
					{
						x1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize;
						asheetentry.m_nSide = ENTRY_SIDE_RIGHT;
						asheetentry.m_fOffset = psheetsymbol->m_fY - y1;
					}
				}
				else
				{
					if((psheetsymbol->m_fY - y1) < (y1 - (psheetsymbol->m_fY - psheetsymbol->m_fYsize)))
					{
						y1 = psheetsymbol->m_fY;
						asheetentry.m_nSide = ENTRY_SIDE_TOP;
						asheetentry.m_fOffset = x1 - psheetsymbol->m_fX;
					}
					else 
					{
						y1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize;
						asheetentry.m_nSide = ENTRY_SIDE_BOTTOM;
						asheetentry.m_fOffset = x1 - psheetsymbol->m_fX;
					}
				}
			}

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_SHEETENTRY;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->m_fX1 = m_fOffset;
			pundo->m_nOrientation = m_nSide;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_nSide = asheetentry.m_nSide;
			m_fOffset = asheetentry.m_fOffset;
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

	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	pView->Write_Help(szEDA_Help_Ready);
}

void COb_schsheetentry::Copy(COb_schsheetentry* pschsheetentry)
{
	m_nSide = pschsheetentry->m_nSide;
	m_nStyle = pschsheetentry->m_nStyle;
	m_nType = pschsheetentry->m_nType;
	m_fOffset = pschsheetentry->m_fOffset;
	
	m_nFillColor = pschsheetentry->m_nFillColor;
	m_nBorderColor = pschsheetentry->m_nBorderColor;
	m_nTextColor = pschsheetentry->m_nTextColor;

	m_csName = pschsheetentry->m_csName;

	m_nDiagram = pschsheetentry->m_nDiagram;
	m_bSelection = pschsheetentry->m_bSelection;
	m_nFlag = pschsheetentry->m_nFlag;

	m_posParent = pschsheetentry->m_posParent;

	pDocument = pschsheetentry->pDocument;
}