#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditPort.h"
#include "Dlg_Global_Port.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_schport, CObject, 0)
COb_schport::COb_schport()
{
	// This empty constructor should be used by serialization only
	m_nFlag = 0 ;
	m_bSelection = FALSE;

	pDocument = NULL;
}


void COb_schport::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fX;
		ar << m_fY;
		ar << m_fSize;
		
		ar << m_nStyle;
		ar << m_nType;
		ar << m_csUniqueID;
		
		ar << m_nFillColor;
		ar << m_nBorderColor;
		ar << m_nTextColor;
		
		ar << m_csName;
		ar << m_nAlignment;
		ar << m_nDiagram;
		ar << m_bSelection;
	}
	else
	{
		ar >> m_fX;
		ar >> m_fY;
		ar >> m_fSize;
		
		ar >> m_nStyle;
		ar >> m_nType;
		ar >> m_csUniqueID;
		
		ar >> m_nFillColor;
		ar >> m_nBorderColor;
		ar >> m_nTextColor;
		
		ar >> m_csName;
		ar >> m_nAlignment;
		ar >> m_nDiagram;
		ar >> m_bSelection;	
	}
}

void COb_schport::Draw(CDC* pDC, CView* pV)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	CESDView* pView = (CESDView*)pV;

	double x,y,size;
	x = m_fX;
	y = m_fY;
	size = m_fSize;

	int vertex_num;
	Fpoint p[6], hotpoint;

	BOOL bLeft = pDoc->CheckWireEnd(m_fX, m_fY, m_nDiagram);
	BOOL bRight = pDoc->CheckWireEnd(m_fX+m_fSize, m_fY, m_nDiagram);
	BOOL bBottom = pDoc->CheckWireEnd(m_fX, m_fY, m_nDiagram);
	BOOL bTop = pDoc->CheckWireEnd(m_fX, m_fY+m_fSize, m_nDiagram);

	if(m_nStyle < PORT_STYLE_NV)
	{
		hotpoint.fx = x;
		hotpoint.fy = y;
		vertex_num = 5;
		p[0].fx = hotpoint.fx;
		p[0].fy = hotpoint.fy - 50;
		p[1].fx = hotpoint.fx + size - 50;
		p[1].fy = hotpoint.fy - 50;
		p[2].fx = hotpoint.fx + size ;
		p[2].fy = hotpoint.fy;
		p[3].fx = hotpoint.fx + size - 50;
		p[3].fy = hotpoint.fy + 50;
		p[4].fx = hotpoint.fx;
		p[4].fy = hotpoint.fy + 50;
		
		if( m_nType == PORT_TYPE_UNSPECIFIED ) 
		{
			if(m_nStyle == PORT_STYLE_NH)
			{
				if((bLeft == TRUE) || (bRight == TRUE))
				{
					hotpoint.fx = x;
					hotpoint.fy = y;
					vertex_num = 4;
					p[0].fx = hotpoint.fx;
					p[0].fy = hotpoint.fy - 50;
					p[1].fx = hotpoint.fx + size;
					p[1].fy = hotpoint.fy - 50;
					p[2].fx = hotpoint.fx + size;
					p[2].fy = hotpoint.fy + 50;
					p[3].fx = hotpoint.fx;
					p[3].fy = hotpoint.fy + 50;
				}
			}
			else if(m_nStyle == PORT_STYLE_LEFT)
			{
				if((bLeft == TRUE) || (bRight == TRUE))
				{
					hotpoint.fx = x;
					hotpoint.fy = y;
					vertex_num = 5;
					p[0].fx = hotpoint.fx;
					p[0].fy = hotpoint.fy;
					p[1].fx = hotpoint.fx + 50;
					p[1].fy = hotpoint.fy - 50;
					p[2].fx = hotpoint.fx + size;
					p[2].fy = hotpoint.fy - 50;
					p[3].fx = hotpoint.fx + size;
					p[3].fy = hotpoint.fy + 50;
					p[4].fx = hotpoint.fx + 50;
					p[4].fy = hotpoint.fy + 50;
				}
			}
			else if(m_nStyle == PORT_STYLE_RIGHT) {} 
			else if(m_nStyle == PORT_STYLE_LEFTRIGHT)
			{
				if((bLeft == TRUE) || (bRight == TRUE))
				{
					hotpoint.fx = x;
					hotpoint.fy = y;
					vertex_num = 6;
					p[0].fx = hotpoint.fx;
					p[0].fy = hotpoint.fy;
					p[1].fx = hotpoint.fx + 50;
					p[1].fy = hotpoint.fy - 50;
					p[2].fx = hotpoint.fx + size - 50;
					p[2].fy = hotpoint.fy - 50;
					p[3].fx = hotpoint.fx + size;
					p[3].fy = hotpoint.fy ;
					p[4].fx = hotpoint.fx + size - 50;
					p[4].fy = hotpoint.fy + 50;
					p[5].fx = hotpoint.fx + 50;
					p[5].fy = hotpoint.fy + 50;
				}
			}
		}
		else if(m_nType == PORT_TYPE_OUTPUT)
		{
			if((bLeft == TRUE)&&(bRight == TRUE))
			{
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 4;
				p[0].fx = hotpoint.fx;
				p[0].fy = hotpoint.fy - 50;
				p[1].fx = hotpoint.fx + size;
				p[1].fy = hotpoint.fy - 50;
				p[2].fx = hotpoint.fx + size;
				p[2].fy = hotpoint.fy + 50;
				p[3].fx = hotpoint.fx;
				p[3].fy = hotpoint.fy + 50;
			}
			else if(bLeft == TRUE){	} 
			else if(bRight == TRUE)
			{
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 5;
				p[0].fx = hotpoint.fx;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx + 50;
				p[1].fy = hotpoint.fy - 50;
				p[2].fx = hotpoint.fx + size;
				p[2].fy = hotpoint.fy - 50;
				p[3].fx = hotpoint.fx + size;
				p[3].fy = hotpoint.fy + 50;
				p[4].fx = hotpoint.fx + 50;
				p[4].fy = hotpoint.fy + 50;
			}
		}
		else if(m_nType == PORT_TYPE_INPUT)
		{
			if((bLeft == TRUE)&&(bRight == TRUE))
			{		
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 6;
				p[0].fx = hotpoint.fx;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx + 50;
				p[1].fy = hotpoint.fy - 50;
				p[2].fx = hotpoint.fx + size - 50;
				p[2].fy = hotpoint.fy - 50;
				p[3].fx = hotpoint.fx + size;
				p[3].fy = hotpoint.fy ;
				p[4].fx = hotpoint.fx + size - 50;
				p[4].fy = hotpoint.fy + 50;
				p[5].fx = hotpoint.fx + 50;
				p[5].fy = hotpoint.fy + 50;
			}
			else if(bLeft == TRUE)
			{
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 5;
				p[0].fx = hotpoint.fx;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx + 50;
				p[1].fy = hotpoint.fy - 50;
				p[2].fx = hotpoint.fx + size;
				p[2].fy = hotpoint.fy - 50;
				p[3].fx = hotpoint.fx + size;
				p[3].fy = hotpoint.fy + 50;
				p[4].fx = hotpoint.fx + 50;
				p[4].fy = hotpoint.fy + 50;
			} 
			else if(bRight == TRUE) { } 
		}
		else if(m_nType == PORT_TYPE_BIDIRECTIONAL)
		{
			if((bLeft == TRUE)||(bRight == TRUE))
			{		
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 6;
				p[0].fx = hotpoint.fx;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx + 50;
				p[1].fy = hotpoint.fy - 50;
				p[2].fx = hotpoint.fx + size - 50;
				p[2].fy = hotpoint.fy - 50;
				p[3].fx = hotpoint.fx + size;
				p[3].fy = hotpoint.fy ;
				p[4].fx = hotpoint.fx + size - 50;
				p[4].fy = hotpoint.fy + 50;
				p[5].fx = hotpoint.fx + 50;
				p[5].fy = hotpoint.fy + 50;
			}
		}
	}
	else 
	{
		hotpoint.fx = x;
		hotpoint.fy = y;
		vertex_num = 5;
		p[0].fx = hotpoint.fx;
		p[0].fy = hotpoint.fy;
		p[1].fx = hotpoint.fx - 50;
		p[1].fy = hotpoint.fy + 50;
		p[2].fx = hotpoint.fx - 50;
		p[2].fy = hotpoint.fy + size;
		p[3].fx = hotpoint.fx + 50;
		p[3].fy = hotpoint.fy + size;
		p[4].fx = hotpoint.fx + 50;
		p[4].fy = hotpoint.fy + 50;
		
		if( m_nType == PORT_TYPE_UNSPECIFIED )
		{
			if(m_nStyle == PORT_STYLE_NV)
			{
				if((bTop == TRUE) || (bBottom == TRUE))
				{
					hotpoint.fx = x;
					hotpoint.fy = y;
					vertex_num = 4;
					p[0].fx = hotpoint.fx - 50;
					p[0].fy = hotpoint.fy;
					p[1].fx = hotpoint.fx - 50;
					p[1].fy = hotpoint.fy + size;
					p[2].fx = hotpoint.fx + 50;
					p[2].fy = hotpoint.fy + size;
					p[3].fx = hotpoint.fx + 50;
					p[3].fy = hotpoint.fy;
				}
			}
			else if(m_nStyle == PORT_STYLE_TOP)
			{
				if((bTop == TRUE) || (bBottom == TRUE))
				{
					hotpoint.fx = x;
					hotpoint.fy = y;
					vertex_num = 5;
					p[0].fx = hotpoint.fx - 50;
					p[0].fy = hotpoint.fy;
					p[1].fx = hotpoint.fx - 50;
					p[1].fy = hotpoint.fy + size - 50;
					p[2].fx = hotpoint.fx;
					p[2].fy = hotpoint.fy + size;
					p[3].fx = hotpoint.fx + 50;
					p[3].fy = hotpoint.fy + size - 50;
					p[4].fx = hotpoint.fx + 50;
					p[4].fy = hotpoint.fy;
				}
			}
			else if(m_nStyle == PORT_STYLE_BOTTOM) {} 
			else if(m_nStyle == PORT_STYLE_TOPBOTTOM)
			{
				if((bTop == TRUE) || (bBottom == TRUE))
				{
					hotpoint.fx = x;
					hotpoint.fy = y;
					vertex_num = 6;
					p[0].fx = hotpoint.fx;
					p[0].fy = hotpoint.fy;
					p[1].fx = hotpoint.fx - 50;
					p[1].fy = hotpoint.fy + 50;
					p[2].fx = hotpoint.fx - 50;
					p[2].fy = hotpoint.fy + size - 50;
					p[3].fx = hotpoint.fx ;
					p[3].fy = hotpoint.fy + size;
					p[4].fx = hotpoint.fx + 50;
					p[4].fy = hotpoint.fy + size - 50;
					p[5].fx = hotpoint.fx + 50;
					p[5].fy = hotpoint.fy + 50;
				}
			}
		}
		else if(m_nType == PORT_TYPE_OUTPUT)
		{
			if((bTop == TRUE)&&(bBottom == TRUE))
			{
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 4;
				p[0].fx = hotpoint.fx - 50;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx - 50;
				p[1].fy = hotpoint.fy + size;
				p[2].fx = hotpoint.fx + 50;
				p[2].fy = hotpoint.fy + size;
				p[3].fx = hotpoint.fx + 50;
				p[3].fy = hotpoint.fy;
			}
			else if(bTop == TRUE){	}
			else if(bBottom == TRUE)
			{
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 5;
				p[0].fx = hotpoint.fx - 50;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx - 50;
				p[1].fy = hotpoint.fy + size - 50;
				p[2].fx = hotpoint.fx;
				p[2].fy = hotpoint.fy + size;
				p[3].fx = hotpoint.fx + 50;
				p[3].fy = hotpoint.fy + size - 50;
				p[4].fx = hotpoint.fx + 50;
				p[4].fy = hotpoint.fy;
			}
		}
		else if(m_nType == PORT_TYPE_INPUT)
		{
			if((bTop == TRUE)&&(bBottom == TRUE))
			{		
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 6;
				p[0].fx = hotpoint.fx;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx - 50;
				p[1].fy = hotpoint.fy + 50;
				p[2].fx = hotpoint.fx - 50;
				p[2].fy = hotpoint.fy + size - 50;
				p[3].fx = hotpoint.fx ;
				p[3].fy = hotpoint.fy + size;
				p[4].fx = hotpoint.fx + 50;
				p[4].fy = hotpoint.fy + size - 50;
				p[5].fx = hotpoint.fx + 50;
				p[5].fy = hotpoint.fy + 50;
			}
			else if(bTop == TRUE)
			{
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 5;
				p[0].fx = hotpoint.fx - 50;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx - 50;
				p[1].fy = hotpoint.fy + size - 50;
				p[2].fx = hotpoint.fx;
				p[2].fy = hotpoint.fy + size;
				p[3].fx = hotpoint.fx + 50;
				p[3].fy = hotpoint.fy + size - 50;
				p[4].fx = hotpoint.fx + 50;
				p[4].fy = hotpoint.fy;
			} 
			else if(bBottom == TRUE) { } 
		}
		else if(m_nType == PORT_TYPE_BIDIRECTIONAL)
		{
			if((bTop == TRUE)||(bBottom == TRUE))
			{		
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 6;
				p[0].fx = hotpoint.fx;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx - 50;
				p[1].fy = hotpoint.fy + 50;
				p[2].fx = hotpoint.fx - 50;
				p[2].fy = hotpoint.fy + size - 50;
				p[3].fx = hotpoint.fx ;
				p[3].fy = hotpoint.fy + size;
				p[4].fx = hotpoint.fx + 50;
				p[4].fy = hotpoint.fy + size - 50;
				p[5].fx = hotpoint.fx + 50;
				p[5].fy = hotpoint.fy + 50;
			}
		}
	}

	COb_schpolygon schpolygon;
	schpolygon.m_nFlag = m_nFlag;
	schpolygon.m_nDiagram = m_nDiagram;
	schpolygon.m_nBorderWidth = 0;
	schpolygon.m_nBorderColor = m_nBorderColor;
	schpolygon.m_nFillColor = m_nFillColor;
	if((pView->m_nPrint_Flag == 1)&&(nEDA_Print_Color == 0)) schpolygon.m_bFillSolid = FALSE;
	else schpolygon.m_bFillSolid = TRUE;
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


	if((m_nStyle == PORT_STYLE_LEFT)||(m_nStyle == PORT_STYLE_RIGHT)||(m_nStyle == PORT_STYLE_LEFTRIGHT)||(m_nStyle == PORT_STYLE_NH))
	{
		if(m_nAlignment == TEXT_ALIGNMENT_LEFT)
		{
			schtext.m_fSx = m_fX + 100.0f;
			schtext.m_fSy = m_fY - 50.0f;
			schtext.m_nH_Justification = 0;
		}
		else if(m_nAlignment == TEXT_ALIGNMENT_RIGHT)
		{
			schtext.m_fSx = m_fX + m_fSize - 100.0f; // - offset*pView->m_fScale;
			schtext.m_fSy = m_fY - 50.0f;
			schtext.m_nH_Justification = 2;
		}
		else
		{
			schtext.m_fSx = m_fX + m_fSize/2; // - (offset*pView->m_fScale)/2;
			schtext.m_fSy = m_fY - 50.0f;
			schtext.m_nH_Justification = 1;
		}
	}
	else
	{
		schtext.m_nOrientation = 90;
		if(m_nAlignment == TEXT_ALIGNMENT_TOP)
		{
			schtext.m_fSx = m_fX + 50.0f;
			schtext.m_fSy = m_fY + m_fSize - 100.0f; // - offset*pView->m_fScale;
			schtext.m_nH_Justification = 2;
		}
		else if(m_nAlignment == TEXT_ALIGNMENT_BOTTOM)
		{
			schtext.m_fSx = m_fX + 50.0f;
			schtext.m_fSy = m_fY + 100.0f;
			schtext.m_nH_Justification = 0;
		}
		else
		{
			schtext.m_fSx = m_fX + 50.0f;
			schtext.m_fSy = m_fY + m_fSize/2; // - (offset*pView->m_fScale)/2;
			schtext.m_nH_Justification = 1;
		}
	}
	
	if(schtext.PickMe(pDC, pV) ==TRUE) schtext.DrawSignal(pDC, pV);

}

void COb_schport::DrawSelection(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	int oldbkmode = pDC->SetBkMode(TRANSPARENT);	

	CPen arcpen;
	arcpen.CreatePen(PS_DOT, 1, nEDA_SelectionColor);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	double fminx, fminy, fmaxx, fmaxy;
	int x,y, x1,y1,x2,y2;
	if(m_nStyle < PORT_STYLE_NV)
	{
		fminx = m_fX;
		fminy = m_fY - 50;
		fmaxx = m_fX + m_fSize;
		fmaxy = m_fY + 50;
	}
	else
	{
		fminx = m_fX - 50;
		fminy = m_fY;
		fmaxx = m_fX + 50;
		fmaxy = m_fY + m_fSize;
	}
	
	pView->Get_VPort_XY(&x1, &y1, fminx, fminy);
	pView->Get_VPort_XY(&x2, &y2, fmaxx, fmaxy);

	pDC->MoveTo(x1, y1);
	pDC->LineTo(x1, y2);
	pDC->LineTo(x2, y2);
	pDC->LineTo(x2, y1);	
	pDC->LineTo(x1, y1);


	if(m_nStyle < PORT_STYLE_NV)
	{
		x = x2;
		y = (y1+y2)/2;
	}
	else
	{
		x = (x1+x2)/2;
		y = y2;
	}
	pView->DrawSelectionKeyPoint(x, y, pDC);

	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}

BOOL  COb_schport::Is_On_Me(double fx, double fy)
{
	double x,y,size;
	x = m_fX;
	y = m_fY;
	size = m_fSize;

	int vertex_num;
	Fpoint p[6], hotpoint;
	
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	BOOL bLeft = pDoc->CheckWireEnd(m_fX, m_fY, m_nDiagram);
	BOOL bRight = pDoc->CheckWireEnd(m_fX+m_fSize, m_fY, m_nDiagram);
	BOOL bBottom = pDoc->CheckWireEnd(m_fX, m_fY, m_nDiagram);
	BOOL bTop = pDoc->CheckWireEnd(m_fX, m_fY+m_fSize, m_nDiagram);

	if(m_nStyle < PORT_STYLE_NV) 
	{
		hotpoint.fx = x;
		hotpoint.fy = y;
		vertex_num = 5;
		p[0].fx = hotpoint.fx;
		p[0].fy = hotpoint.fy - 50;
		p[1].fx = hotpoint.fx + size - 50;
		p[1].fy = hotpoint.fy - 50;
		p[2].fx = hotpoint.fx + size ;
		p[2].fy = hotpoint.fy;
		p[3].fx = hotpoint.fx + size - 50;
		p[3].fy = hotpoint.fy + 50;
		p[4].fx = hotpoint.fx;
		p[4].fy = hotpoint.fy + 50;
		
		if( m_nType == PORT_TYPE_UNSPECIFIED ) 
		{
			if(m_nStyle == PORT_STYLE_NH)
			{
				if((bLeft == TRUE) || (bRight == TRUE))
				{
					hotpoint.fx = x;
					hotpoint.fy = y;
					vertex_num = 4;
					p[0].fx = hotpoint.fx;
					p[0].fy = hotpoint.fy - 50;
					p[1].fx = hotpoint.fx + size;
					p[1].fy = hotpoint.fy - 50;
					p[2].fx = hotpoint.fx + size;
					p[2].fy = hotpoint.fy + 50;
					p[3].fx = hotpoint.fx;
					p[3].fy = hotpoint.fy + 50;
				}
			}
			else if(m_nStyle == PORT_STYLE_LEFT)
			{
				if((bLeft == TRUE) || (bRight == TRUE))
				{
					hotpoint.fx = x;
					hotpoint.fy = y;
					vertex_num = 5;
					p[0].fx = hotpoint.fx;
					p[0].fy = hotpoint.fy;
					p[1].fx = hotpoint.fx + 50;
					p[1].fy = hotpoint.fy - 50;
					p[2].fx = hotpoint.fx + size;
					p[2].fy = hotpoint.fy - 50;
					p[3].fx = hotpoint.fx + size;
					p[3].fy = hotpoint.fy + 50;
					p[4].fx = hotpoint.fx + 50;
					p[4].fy = hotpoint.fy + 50;
				}
			}
			else if(m_nStyle == PORT_STYLE_RIGHT) {} 
			else if(m_nStyle == PORT_STYLE_LEFTRIGHT)
			{
				if((bLeft == TRUE) || (bRight == TRUE))
				{
					hotpoint.fx = x;
					hotpoint.fy = y;
					vertex_num = 6;
					p[0].fx = hotpoint.fx;
					p[0].fy = hotpoint.fy;
					p[1].fx = hotpoint.fx + 50;
					p[1].fy = hotpoint.fy - 50;
					p[2].fx = hotpoint.fx + size - 50;
					p[2].fy = hotpoint.fy - 50;
					p[3].fx = hotpoint.fx + size;
					p[3].fy = hotpoint.fy ;
					p[4].fx = hotpoint.fx + size - 50;
					p[4].fy = hotpoint.fy + 50;
					p[5].fx = hotpoint.fx + 50;
					p[5].fy = hotpoint.fy + 50;
				}
			}
		}
		else if(m_nType == PORT_TYPE_OUTPUT)
		{
			if((bLeft == TRUE)&&(bRight == TRUE))
			{
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 4;
				p[0].fx = hotpoint.fx;
				p[0].fy = hotpoint.fy - 50;
				p[1].fx = hotpoint.fx + size;
				p[1].fy = hotpoint.fy - 50;
				p[2].fx = hotpoint.fx + size;
				p[2].fy = hotpoint.fy + 50;
				p[3].fx = hotpoint.fx;
				p[3].fy = hotpoint.fy + 50;
			}
			else if(bLeft == TRUE){	} 
			else if(bRight == TRUE)
			{
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 5;
				p[0].fx = hotpoint.fx;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx + 50;
				p[1].fy = hotpoint.fy - 50;
				p[2].fx = hotpoint.fx + size;
				p[2].fy = hotpoint.fy - 50;
				p[3].fx = hotpoint.fx + size;
				p[3].fy = hotpoint.fy + 50;
				p[4].fx = hotpoint.fx + 50;
				p[4].fy = hotpoint.fy + 50;
			}
		}
		else if(m_nType == PORT_TYPE_INPUT)
		{
			if((bLeft == TRUE)&&(bRight == TRUE))
			{		
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 6;
				p[0].fx = hotpoint.fx;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx + 50;
				p[1].fy = hotpoint.fy - 50;
				p[2].fx = hotpoint.fx + size - 50;
				p[2].fy = hotpoint.fy - 50;
				p[3].fx = hotpoint.fx + size;
				p[3].fy = hotpoint.fy ;
				p[4].fx = hotpoint.fx + size - 50;
				p[4].fy = hotpoint.fy + 50;
				p[5].fx = hotpoint.fx + 50;
				p[5].fy = hotpoint.fy + 50;
			}
			else if(bLeft == TRUE)
			{
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 5;
				p[0].fx = hotpoint.fx;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx + 50;
				p[1].fy = hotpoint.fy - 50;
				p[2].fx = hotpoint.fx + size;
				p[2].fy = hotpoint.fy - 50;
				p[3].fx = hotpoint.fx + size;
				p[3].fy = hotpoint.fy + 50;
				p[4].fx = hotpoint.fx + 50;
				p[4].fy = hotpoint.fy + 50;
			} 
			else if(bRight == TRUE) { }
		}
		else if(m_nType == PORT_TYPE_BIDIRECTIONAL)
		{
			if((bLeft == TRUE)||(bRight == TRUE))
			{		
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 6;
				p[0].fx = hotpoint.fx;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx + 50;
				p[1].fy = hotpoint.fy - 50;
				p[2].fx = hotpoint.fx + size - 50;
				p[2].fy = hotpoint.fy - 50;
				p[3].fx = hotpoint.fx + size;
				p[3].fy = hotpoint.fy ;
				p[4].fx = hotpoint.fx + size - 50;
				p[4].fy = hotpoint.fy + 50;
				p[5].fx = hotpoint.fx + 50;
				p[5].fy = hotpoint.fy + 50;
			}
		}
	}
	else 
	{
		hotpoint.fx = x;
		hotpoint.fy = y;
		vertex_num = 5;
		p[0].fx = hotpoint.fx;
		p[0].fy = hotpoint.fy;
		p[1].fx = hotpoint.fx - 50;
		p[1].fy = hotpoint.fy + 50;
		p[2].fx = hotpoint.fx - 50;
		p[2].fy = hotpoint.fy + size;
		p[3].fx = hotpoint.fx + 50;
		p[3].fy = hotpoint.fy + size;
		p[4].fx = hotpoint.fx + 50;
		p[4].fy = hotpoint.fy + 50;
		
		if( m_nType == PORT_TYPE_UNSPECIFIED ) 
		{
			if(m_nStyle == PORT_STYLE_NV)
			{
				if((bTop == TRUE) || (bBottom == TRUE))
				{
					hotpoint.fx = x;
					hotpoint.fy = y;
					vertex_num = 4;
					p[0].fx = hotpoint.fx - 50;
					p[0].fy = hotpoint.fy;
					p[1].fx = hotpoint.fx - 50;
					p[1].fy = hotpoint.fy + size;
					p[2].fx = hotpoint.fx + 50;
					p[2].fy = hotpoint.fy + size;
					p[3].fx = hotpoint.fx + 50;
					p[3].fy = hotpoint.fy;
				}
			}
			else if(m_nStyle == PORT_STYLE_TOP)
			{
				if((bTop == TRUE) || (bBottom == TRUE))
				{
					hotpoint.fx = x;
					hotpoint.fy = y;
					vertex_num = 5;
					p[0].fx = hotpoint.fx - 50;
					p[0].fy = hotpoint.fy;
					p[1].fx = hotpoint.fx - 50;
					p[1].fy = hotpoint.fy + size - 50;
					p[2].fx = hotpoint.fx;
					p[2].fy = hotpoint.fy + size;
					p[3].fx = hotpoint.fx + 50;
					p[3].fy = hotpoint.fy + size - 50;
					p[4].fx = hotpoint.fx + 50;
					p[4].fy = hotpoint.fy;
				}
			}
			else if(m_nStyle == PORT_STYLE_BOTTOM) {} 
			else if(m_nStyle == PORT_STYLE_TOPBOTTOM)
			{
				if((bTop == TRUE) || (bBottom == TRUE))
				{
					hotpoint.fx = x;
					hotpoint.fy = y;
					vertex_num = 6;
					p[0].fx = hotpoint.fx;
					p[0].fy = hotpoint.fy;
					p[1].fx = hotpoint.fx - 50;
					p[1].fy = hotpoint.fy + 50;
					p[2].fx = hotpoint.fx - 50;
					p[2].fy = hotpoint.fy + size - 50;
					p[3].fx = hotpoint.fx ;
					p[3].fy = hotpoint.fy + size;
					p[4].fx = hotpoint.fx + 50;
					p[4].fy = hotpoint.fy + size - 50;
					p[5].fx = hotpoint.fx + 50;
					p[5].fy = hotpoint.fy + 50;
				}
			}
		}
		else if(m_nType == PORT_TYPE_OUTPUT)
		{
			if((bTop == TRUE)&&(bBottom == TRUE))
			{
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 4;
				p[0].fx = hotpoint.fx - 50;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx - 50;
				p[1].fy = hotpoint.fy + size;
				p[2].fx = hotpoint.fx + 50;
				p[2].fy = hotpoint.fy + size;
				p[3].fx = hotpoint.fx + 50;
				p[3].fy = hotpoint.fy;
			}
			else if(bTop == TRUE){	} 
			else if(bBottom == TRUE)
			{
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 5;
				p[0].fx = hotpoint.fx - 50;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx - 50;
				p[1].fy = hotpoint.fy + size - 50;
				p[2].fx = hotpoint.fx;
				p[2].fy = hotpoint.fy + size;
				p[3].fx = hotpoint.fx + 50;
				p[3].fy = hotpoint.fy + size - 50;
				p[4].fx = hotpoint.fx + 50;
				p[4].fy = hotpoint.fy;
			}
		}
		else if(m_nType == PORT_TYPE_INPUT)
		{
			if((bTop == TRUE)&&(bBottom == TRUE))
			{		
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 6;
				p[0].fx = hotpoint.fx;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx - 50;
				p[1].fy = hotpoint.fy + 50;
				p[2].fx = hotpoint.fx - 50;
				p[2].fy = hotpoint.fy + size - 50;
				p[3].fx = hotpoint.fx ;
				p[3].fy = hotpoint.fy + size;
				p[4].fx = hotpoint.fx + 50;
				p[4].fy = hotpoint.fy + size - 50;
				p[5].fx = hotpoint.fx + 50;
				p[5].fy = hotpoint.fy + 50;
			}
			else if(bTop == TRUE)
			{
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 5;
				p[0].fx = hotpoint.fx - 50;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx - 50;
				p[1].fy = hotpoint.fy + size - 50;
				p[2].fx = hotpoint.fx;
				p[2].fy = hotpoint.fy + size;
				p[3].fx = hotpoint.fx + 50;
				p[3].fy = hotpoint.fy + size - 50;
				p[4].fx = hotpoint.fx + 50;
				p[4].fy = hotpoint.fy;
			} 
			else if(bBottom == TRUE) { } 
		}
		else if(m_nType == PORT_TYPE_BIDIRECTIONAL)
		{
			if((bTop == TRUE)||(bBottom == TRUE))
			{		
				hotpoint.fx = x;
				hotpoint.fy = y;
				vertex_num = 6;
				p[0].fx = hotpoint.fx;
				p[0].fy = hotpoint.fy;
				p[1].fx = hotpoint.fx - 50;
				p[1].fy = hotpoint.fy + 50;
				p[2].fx = hotpoint.fx - 50;
				p[2].fy = hotpoint.fy + size - 50;
				p[3].fx = hotpoint.fx ;
				p[3].fy = hotpoint.fy + size;
				p[4].fx = hotpoint.fx + 50;
				p[4].fy = hotpoint.fy + size - 50;
				p[5].fx = hotpoint.fx + 50;
				p[5].fy = hotpoint.fy + 50;
			}
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

BOOL  COb_schport::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fminx, fminy, fmaxx, fmaxy;
	if(m_nStyle < PORT_STYLE_NV)
	{
		fminx = m_fX;
		fminy = m_fY - 50;
		fmaxx = m_fX + m_fSize;
		fmaxy = m_fY + 50;
	}
	else
	{
		fminx = m_fX - 50;
		fminy = m_fY;
		fmaxx = m_fX + 50;
		fmaxy = m_fY + m_fSize;
	}
	
	if((fminx < fx1)||(fmaxx > fx2)||(fminy < fy1)||(fmaxy > fy2)) return FALSE;
	return TRUE;
}

void COb_schport::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_EditPort dlg;

	dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fX);
	dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fY);
	dlg.m_csEditSize = pDoc->Convert_XY_To_String(m_fSize);	

	dlg.m_nFillColor = m_nFillColor;
	dlg.m_nBorderColor = m_nBorderColor;
	dlg.m_nTextColor = m_nTextColor;
	
	dlg.m_csEditName = m_csName;
	
	dlg.m_nComboType = m_nType;
	dlg.m_nComboStyle = m_nStyle;
	dlg.m_nComboAlignment = m_nAlignment;


	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_bCheckGlobal == TRUE)
		{
			CDlg_Global_Port subdlg;
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
							{
								COb_schport* pport = (COb_schport*)pobject;
								if((pport->m_nDiagram != nDiagram)||(pport->m_nFlag != FLAG_NORMAL))	continue;
								if(pport == this) continue; 

								if( (((subdlg.m_bCheckTextColor == TRUE)&&(pport->m_nTextColor == m_nTextColor))||(subdlg.m_bCheckTextColor == FALSE)) &&
									(((subdlg.m_bCheckBorderColor == TRUE)&&(pport->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckBorderColor == FALSE)) && 
									(((subdlg.m_bCheckFillColor == TRUE)&&(pport->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
									(((subdlg.m_bCheckStyle == TRUE)&&(pport->m_nStyle == m_nStyle))||(subdlg.m_bCheckStyle == FALSE)) &&
									(((subdlg.m_bCheckAlignment == TRUE)&&(pport->m_nAlignment == m_nAlignment))||(subdlg.m_bCheckAlignment == FALSE)) &&
									(((subdlg.m_bCheckIO == TRUE)&&(pport->m_nType == m_nType))||(subdlg.m_bCheckIO == FALSE)) )
								{
									if( (pport->m_nBorderColor == m_nBorderColor)||(pport->m_nTextColor == m_nTextColor)||(pport->m_nFillColor == m_nFillColor)||\
										(pport->m_nStyle == m_nStyle)||(pport->m_nAlignment == m_nAlignment)||(pport->m_nType == m_nType) )
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_PORT;
										pundo->m_nDiagram = nDiagram;
										pundo->m_nPos = posbak;
										pundo->schport.Copy(pport);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

									}
									if(pport->m_nBorderColor == m_nBorderColor) pport->m_nBorderColor = dlg.m_nBorderColor;
									if(pport->m_nTextColor == m_nTextColor) pport->m_nTextColor = dlg.m_nTextColor;
									if(pport->m_nFillColor == m_nFillColor) pport->m_nFillColor = dlg.m_nFillColor;
									if(pport->m_nStyle == m_nStyle) pport->m_nStyle = dlg.m_nComboStyle;
									if(pport->m_nType == m_nType) pport->m_nType = dlg.m_nComboType;
									if(pport->m_nAlignment == m_nAlignment) pport->m_nAlignment = dlg.m_nComboAlignment;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
						{
							COb_schport* pport = (COb_schport*)pobject;
							if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag != FLAG_NORMAL))	continue;
							if(pport == this) continue; 

							if( (((subdlg.m_bCheckTextColor == TRUE)&&(pport->m_nTextColor == m_nTextColor))||(subdlg.m_bCheckTextColor == FALSE)) &&
								(((subdlg.m_bCheckBorderColor == TRUE)&&(pport->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckBorderColor == FALSE)) && 
								(((subdlg.m_bCheckFillColor == TRUE)&&(pport->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
								(((subdlg.m_bCheckStyle == TRUE)&&(pport->m_nStyle == m_nStyle))||(subdlg.m_bCheckStyle == FALSE)) &&
								(((subdlg.m_bCheckAlignment == TRUE)&&(pport->m_nAlignment == m_nAlignment))||(subdlg.m_bCheckAlignment == FALSE)) &&
								(((subdlg.m_bCheckIO == TRUE)&&(pport->m_nType == m_nType))||(subdlg.m_bCheckIO == FALSE)) )
							{
								if( (pport->m_nBorderColor == m_nBorderColor)||(pport->m_nTextColor == m_nTextColor)||(pport->m_nFillColor == m_nFillColor)||\
									(pport->m_nStyle == m_nStyle)||(pport->m_nAlignment == m_nAlignment)||(pport->m_nType == m_nType) )
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_PORT;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schport.Copy(pport);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

								}
								if(pport->m_nBorderColor == m_nBorderColor) pport->m_nBorderColor = dlg.m_nBorderColor;
								if(pport->m_nTextColor == m_nTextColor) pport->m_nTextColor = dlg.m_nTextColor;
								if(pport->m_nFillColor == m_nFillColor) pport->m_nFillColor = dlg.m_nFillColor;
								if(pport->m_nStyle == m_nStyle) pport->m_nStyle = dlg.m_nComboStyle;
								if(pport->m_nType == m_nType) pport->m_nType = dlg.m_nComboType;
								if(pport->m_nAlignment == m_nAlignment) pport->m_nAlignment = dlg.m_nComboAlignment;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
						{
							COb_schport* pport = (COb_schport*)pobject;
							if((pport->m_bSelection != TRUE)||(pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag != FLAG_NORMAL))	continue;
							if(pport == this) continue;

							if( (((subdlg.m_bCheckTextColor == TRUE)&&(pport->m_nTextColor == m_nTextColor))||(subdlg.m_bCheckTextColor == FALSE)) &&
								(((subdlg.m_bCheckBorderColor == TRUE)&&(pport->m_nBorderColor == m_nBorderColor))||(subdlg.m_bCheckBorderColor == FALSE)) && 
								(((subdlg.m_bCheckFillColor == TRUE)&&(pport->m_nFillColor == m_nFillColor))||(subdlg.m_bCheckFillColor == FALSE)) &&
								(((subdlg.m_bCheckStyle == TRUE)&&(pport->m_nStyle == m_nStyle))||(subdlg.m_bCheckStyle == FALSE)) &&
								(((subdlg.m_bCheckAlignment == TRUE)&&(pport->m_nAlignment == m_nAlignment))||(subdlg.m_bCheckAlignment == FALSE)) &&
								(((subdlg.m_bCheckIO == TRUE)&&(pport->m_nType == m_nType))||(subdlg.m_bCheckIO == FALSE)) )
							{
								if( (pport->m_nBorderColor == m_nBorderColor)||(pport->m_nTextColor == m_nTextColor)||(pport->m_nFillColor == m_nFillColor)||\
									(pport->m_nStyle == m_nStyle)||(pport->m_nAlignment == m_nAlignment)||(pport->m_nType == m_nType) )
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_PORT;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schport.Copy(pport);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								}
								if(pport->m_nBorderColor == m_nBorderColor) pport->m_nBorderColor = dlg.m_nBorderColor;
								if(pport->m_nTextColor == m_nTextColor) pport->m_nTextColor = dlg.m_nTextColor;
								if(pport->m_nFillColor == m_nFillColor) pport->m_nFillColor = dlg.m_nFillColor;
								if(pport->m_nStyle == m_nStyle) pport->m_nStyle = dlg.m_nComboStyle;
								if(pport->m_nType == m_nType) pport->m_nType = dlg.m_nComboType;
								if(pport->m_nAlignment == m_nAlignment) pport->m_nAlignment = dlg.m_nComboAlignment;
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
				pundo->m_nOb = SCH_ELEMENT_PORT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schport.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				m_fX = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
				m_fY = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
				m_fSize = pDoc->Convert_String_To_XY(dlg.m_csEditSize);

				m_nFillColor = dlg.m_nFillColor;
				m_nBorderColor = dlg.m_nBorderColor;
				m_nTextColor = dlg.m_nTextColor;
				
				m_csName = dlg.m_csEditName;
				
				m_nType = dlg.m_nComboType;
				m_nStyle = dlg.m_nComboStyle;
				m_nAlignment = dlg.m_nComboAlignment;

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
			pundo->m_nOb = SCH_ELEMENT_PORT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->schport.Copy(this);
				
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

			m_fX = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
			m_fY = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
			m_fSize = pDoc->Convert_String_To_XY(dlg.m_csEditSize);

			m_nFillColor = dlg.m_nFillColor;
			m_nBorderColor = dlg.m_nBorderColor;
			m_nTextColor = dlg.m_nTextColor;
			
			m_csName = dlg.m_csEditName;
			
			m_nType = dlg.m_nComboType;
			m_nStyle = dlg.m_nComboStyle;
			m_nAlignment = dlg.m_nComboAlignment;
		}
	}
}

void COb_schport::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	double x1,y1, x0, y0;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动接口: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

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

	COb_schport aport;
	aport.pDocument = pDoc;
	aport.m_fX = m_fX;
	aport.m_fY = m_fY;
	aport.m_fSize = m_fSize;
	aport.m_nStyle = m_nStyle;
	aport.m_nType = m_nType;
	aport.m_nFillColor = m_nFillColor;
	aport.m_nBorderColor = m_nBorderColor;
	aport.m_nTextColor = m_nTextColor;
	aport.m_csName.Empty();
	aport.m_nAlignment = m_nAlignment;
	aport.m_nDiagram = m_nDiagram;
	aport.Draw(pDC, pV);
	pView->OpenMouseCursor();
	
	bEDA_NeedElectricalGrid = TRUE;

	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			aport.Draw(pDC, pV);
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
				aport.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;

			aport.m_fX = x1;
			aport.m_fY = y1;
			
			pView->CloseMouseCursor();
			aport.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			aport.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			
			aport.m_fX = x1;
			aport.m_fY = y1;

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_PORT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->m_fX1 = m_fX;
			pundo->m_fY1 = m_fY;
			pundo->m_fX2 = m_fSize;
			pundo->m_nOrientation = m_nStyle;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;


			m_fX = aport.m_fX;
			m_fY = aport.m_fY;
			m_nStyle = aport.m_nStyle;
			m_fSize = aport.m_fSize;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aport.Draw(pDC, pV);
			
			if(aport.m_nStyle == PORT_STYLE_NH) aport.m_nStyle = PORT_STYLE_NV;
			else if(aport.m_nStyle == PORT_STYLE_NV) aport.m_nStyle = PORT_STYLE_NH;
			else if(aport.m_nStyle == PORT_STYLE_LEFTRIGHT) aport.m_nStyle = PORT_STYLE_TOPBOTTOM;
			else if(aport.m_nStyle == PORT_STYLE_TOPBOTTOM) aport.m_nStyle = PORT_STYLE_LEFTRIGHT;
			else if(aport.m_nStyle == PORT_STYLE_LEFT)  aport.m_nStyle = PORT_STYLE_BOTTOM;
			else if(aport.m_nStyle == PORT_STYLE_BOTTOM) aport.m_nStyle = PORT_STYLE_LEFT;
			else if(aport.m_nStyle == PORT_STYLE_RIGHT) aport.m_nStyle = PORT_STYLE_TOP;
			else if(aport.m_nStyle == PORT_STYLE_TOP) aport.m_nStyle = PORT_STYLE_RIGHT;

			aport.Draw(pDC, pV);

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
	
	bEDA_NeedElectricalGrid = FALSE;
	pView->m_fpHotPoint1.fx = pView->m_fpHotPoint1.fy = pView->m_fpHotPoint2.fx = pView->m_fpHotPoint2.fy = 0;

	pView->Write_Help(szEDA_Help_Ready);
}

BOOL COb_schport::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	double x1,y1, x0, y0;
	
	char szHelp[] = "复制接口: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	x0 = m_fX;
	y0 = m_fY;
	
	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	//复制对象
	COb_schport aport;
	aport.pDocument = pDoc;
	aport.m_fX = m_fX;
	aport.m_fY = m_fY;
	aport.m_fSize = m_fSize;
	aport.m_nStyle = m_nStyle;
	aport.m_nType = m_nType;
	aport.m_nFillColor = m_nFillColor;
	aport.m_nBorderColor = m_nBorderColor;
	aport.m_nTextColor = m_nTextColor;
	aport.m_csName.Empty();
	aport.m_nAlignment = m_nAlignment;
	aport.m_nDiagram = m_nDiagram;
	aport.Draw(pDC, pV);
	pView->OpenMouseCursor();
	
	bEDA_NeedElectricalGrid = TRUE;
	
	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			aport.Draw(pDC, pV);
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
				aport.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;

			aport.m_fX = x1;
			aport.m_fY = y1;
			
			pView->CloseMouseCursor();
			aport.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->ElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			aport.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			
			aport.m_fX = x1;
			aport.m_fY = y1;

			m_fX = aport.m_fX;
			m_fY = aport.m_fY;
			m_nStyle = aport.m_nStyle;
			m_fSize = aport.m_fSize;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aport.Draw(pDC, pV);
			
			if(aport.m_nStyle == PORT_STYLE_NH) aport.m_nStyle = PORT_STYLE_NV;
			else if(aport.m_nStyle == PORT_STYLE_NV) aport.m_nStyle = PORT_STYLE_NH;
			else if(aport.m_nStyle == PORT_STYLE_LEFTRIGHT) aport.m_nStyle = PORT_STYLE_TOPBOTTOM;
			else if(aport.m_nStyle == PORT_STYLE_TOPBOTTOM) aport.m_nStyle = PORT_STYLE_LEFTRIGHT;
			else if(aport.m_nStyle == PORT_STYLE_LEFT)  aport.m_nStyle = PORT_STYLE_BOTTOM;
			else if(aport.m_nStyle == PORT_STYLE_BOTTOM) aport.m_nStyle = PORT_STYLE_LEFT;
			else if(aport.m_nStyle == PORT_STYLE_RIGHT) aport.m_nStyle = PORT_STYLE_TOP;
			else if(aport.m_nStyle == PORT_STYLE_TOP) aport.m_nStyle = PORT_STYLE_RIGHT;

			aport.Draw(pDC, pV);

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

void COb_schport::Copy(COb_schport* pschport)
{
	m_fX = pschport->m_fX;
	m_fY = pschport->m_fY;
	m_fSize = pschport->m_fSize;

	m_nStyle = pschport->m_nStyle;
	m_nType = pschport->m_nType;
	m_csUniqueID = pschport->m_csUniqueID;
	
	m_nFillColor = pschport->m_nFillColor;
	m_nBorderColor = pschport->m_nBorderColor;
	m_nTextColor = pschport->m_nTextColor;

	m_csName = pschport->m_csName;
	m_nAlignment = pschport->m_nAlignment;

	m_nDiagram = pschport->m_nDiagram;
	m_bSelection = pschport->m_bSelection;
	m_nFlag = pschport->m_nFlag;

	pDocument = pschport->pDocument;
}