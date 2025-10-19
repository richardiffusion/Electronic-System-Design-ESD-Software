#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Sheet_EditPolyline.h"
#include "Dlg_Global_Polyline.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_schpolyline, CObject, 0)
COb_schpolyline::COb_schpolyline()
{
	//CArray支持自动扩展，初始设置参数是避免polyline存贮过于“碎片”化
	//m_cVertex.SetSize(10);
	m_nFlag = 0;
	m_bSelection = FALSE;
	m_nStartLineShape = 0;
	m_nEndLineShape = 0;
	m_nLineShapeSize = 0;
	
	m_nOwnerPartID = 0;
	m_nDisplayMode = 0;
	m_posParent = NULL;

	pDocument = NULL;
}


void COb_schpolyline::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nLineWidth;
		ar << m_nLineType;
		ar << m_nStartLineShape;
		ar << m_nEndLineShape;
		ar << m_nLineShapeSize;
		ar << m_nColor;
		
		int vertexNum = m_cVertex.GetCount();
		ar << vertexNum;
		for(int i=0; i<vertexNum; i++)
		{
			ar << m_cVertex[i].fx;
			ar << m_cVertex[i].fy;
		}

		ar << m_nOwnerPartID;
		ar << m_nDisplayMode;
		//ar << m_posParent;		
		
		ar << m_nDiagram;
		ar << m_bSelection;
	}
	else
	{
		ar >> m_nLineWidth;
		ar >> m_nLineType;
		ar >> m_nStartLineShape;
		ar >> m_nEndLineShape;
		ar >> m_nLineShapeSize;
		ar >> m_nColor;
		
		int vertexNum;
		ar >> vertexNum;
		m_cVertex.RemoveAll();
		m_cVertex.SetSize(vertexNum);
		for(int i=0; i<vertexNum; i++)
		{
			ar >> m_cVertex[i].fx;
			ar >> m_cVertex[i].fy;
		}

		ar >> m_nOwnerPartID;
		ar >> m_nDisplayMode;
		//ar >> m_posParent;		
		
		ar >> m_nDiagram;
		ar >> m_bSelection;
	}
}

void COb_schpolyline::Draw(CDC* pDC, CView* pV)
{

	//	CESDView* pView = (CESDView*)pV;
	COb_schline schline;
	schline.m_nFlag = m_nFlag;
	schline.m_nDiagram = m_nDiagram;
	schline.m_nAttrib = LINE_WIRE;
	schline.m_nColor = m_nColor;
	schline.m_nLineType = m_nLineType;
	schline.m_nLineWidth = m_nLineWidth;
	schline.m_cVertex.SetSize(m_cVertex.GetSize());

	for(int i =0; i<m_cVertex.GetSize(); i++)
	{
		schline.m_cVertex[i].fx = m_cVertex[i].fx;
		schline.m_cVertex[i].fy = m_cVertex[i].fy;
	}
	
	if(schline.PickMe(pV) ==TRUE) schline.Draw(pDC, pV);

	if(m_nStartLineShape == 0) {}
	else if(m_nStartLineShape == 1)
	{
		double alpha, beta;
		alpha = 18.0f*fEDA_pi/180.0f; 
	
		schline.m_cVertex.SetSize(3);

		double fx1, fy1, fx2, fy2;
		fx1 = m_cVertex[0].fx;
		fy1 = m_cVertex[0].fy;
		fx2 = m_cVertex[1].fx;
		fy2 = m_cVertex[1].fy;

		double a;
		if(m_nLineShapeSize == 0) a = 40;
		else if(m_nLineShapeSize == 1) a = 80;
		else if(m_nLineShapeSize == 2) a = 120;
		else if(m_nLineShapeSize == 3) a = 160;
		if((fx1 == fx2)&&(fy2 > fy1))		
		{
			schline.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schline.m_cVertex[0].fy = fy1 + a;
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schline.m_cVertex[2].fy = fy1 + a;
		}
		else if((fx1 == fx2)&&(fy2 < fy1)) 
		{
			schline.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schline.m_cVertex[0].fy = fy1 - a;
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schline.m_cVertex[2].fy = fy1 - a;		
		}
		else if((fy1 == fy2)&&(fx2 > fx1))	
		{
			schline.m_cVertex[0].fx = fx1 + a;
			schline.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 + a;
			schline.m_cVertex[2].fy = fy1 + a*tan(alpha);
		}
		else if((fy1 == fy2)&&(fx2 < fx1))  
		{
			schline.m_cVertex[0].fx = fx1 - a;
			schline.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 - a;
			schline.m_cVertex[2].fy = fy1 + a*tan(alpha);		
		}
		else
		{
			double k1, k2;
			k1 = (fy2-fy1)/(fx2-fx1);
			beta = atan(k1);
			k2 = tan(beta + alpha);
			if(fx2 > fx1)
			{
				schline.m_cVertex[0].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[0].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
			else
			{
				schline.m_cVertex[0].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[0].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );			
			}
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;	
			k2 = tan(beta - alpha);
			if(fx2 > fx1)
			{
				schline.m_cVertex[2].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[2].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
			else
			{
				schline.m_cVertex[2].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[2].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
		}
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nStartLineShape == 2)
	{
		COb_schpolygon schpolygon;
		schpolygon.m_nFlag = m_nFlag;
		schpolygon.m_nDiagram = m_nDiagram;
		schpolygon.m_nBorderWidth = 0;
		schpolygon.m_nBorderColor = m_nColor;
		schpolygon.m_nFillColor = m_nColor;
		schpolygon.m_bFillSolid = TRUE;
		schpolygon.m_bTransparent = FALSE;
		schpolygon.m_cVertex.SetSize(3);
		
		double alpha, beta;
		alpha = 18.0f*fEDA_pi/180.0f; 
	
		double fx1, fy1, fx2, fy2;
		fx1 = m_cVertex[0].fx;
		fy1 = m_cVertex[0].fy;
		fx2 = m_cVertex[1].fx;
		fy2 = m_cVertex[1].fy;

		double b;
		if(m_nLineShapeSize == 0) b = 15;
		else if(m_nLineShapeSize == 1) b = 30;
		else if(m_nLineShapeSize == 2) b = 45;
		else if(m_nLineShapeSize == 3) b = 60;

		if((fx1 == fx2)&&(fy2 > fy1))		
		{
			fy1 -= b;
		}
		else if((fx1 == fx2)&&(fy2 < fy1))  
		{
			fy1 += b;
		}
		else if((fy1 == fy2)&&(fx2 > fx1))		
		{
			fx1 -= b;
		}
		else if((fy1 == fy2)&&(fx2 < fx1)) 
		{
			fx1 += b;
		}
		else
		{
			double k1;
			k1 = (fy2-fy1)/(fx2-fx1);
			beta = atan(k1);
			if(fx2 > fx1)
			{
				fx1 = fx1 - b*cos(beta);
				fy1 = fy1 - b*sin(beta);
			}
			else
			{
				fx1 = fx1 + b*cos(beta);
				fy1 = fy1 + b*sin(beta);	
			}

		}

		double a;
		if(m_nLineShapeSize == 0) a = 55;
		else if(m_nLineShapeSize == 1) a = 110;
		else if(m_nLineShapeSize == 2) a = 165;
		else if(m_nLineShapeSize == 3) a = 220;
		if((fx1 == fx2)&&(fy2 > fy1))		
		{
			schpolygon.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schpolygon.m_cVertex[0].fy = fy1 + a;
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schpolygon.m_cVertex[2].fy = fy1 + a;
		}
		else if((fx1 == fx2)&&(fy2 < fy1))  
		{
			schpolygon.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schpolygon.m_cVertex[0].fy = fy1 - a;
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schpolygon.m_cVertex[2].fy = fy1 - a;		
		}
		else if((fy1 == fy2)&&(fx2 > fx1))	
		{
			schpolygon.m_cVertex[0].fx = fx1 + a;
			schpolygon.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 + a;
			schpolygon.m_cVertex[2].fy = fy1 + a*tan(alpha);
		}
		else if((fy1 == fy2)&&(fx2 < fx1))  
		{
			schpolygon.m_cVertex[0].fx = fx1 - a;
			schpolygon.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 - a;
			schpolygon.m_cVertex[2].fy = fy1 + a*tan(alpha);		
		}
		else
		{
			double k1, k2;
			k1 = (fy2-fy1)/(fx2-fx1);
			beta = atan(k1);
			k2 = tan(beta + alpha);
			if(fx2 > fx1)
			{
				schpolygon.m_cVertex[0].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[0].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
			else
			{
				schpolygon.m_cVertex[0].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[0].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );			
			}
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;	
			k2 = tan(beta - alpha);
			if(fx2 > fx1)
			{
				schpolygon.m_cVertex[2].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[2].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
			else
			{
				schpolygon.m_cVertex[2].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[2].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
		}
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);

	}
	else if(m_nStartLineShape == 3)
	{
		double alpha, beta;
		alpha = 26.565f*fEDA_pi/180.0f; 
	
		schline.m_cVertex.SetSize(3);

		double fx1, fy1, fx2, fy2;
		fx1 = m_cVertex[0].fx;
		fy1 = m_cVertex[0].fy;
		fx2 = m_cVertex[1].fx;
		fy2 = m_cVertex[1].fy;


		double a;
		if(m_nLineShapeSize == 0) a = 20;
		else if(m_nLineShapeSize == 1) a = 40;
		else if(m_nLineShapeSize == 2) a = 60;
		else if(m_nLineShapeSize == 3) a = 80;

		//one
		if((fx1 == fx2)&&(fy2 > fy1))		
		{
			schline.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schline.m_cVertex[0].fy = fy1 - a;
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schline.m_cVertex[2].fy = fy1 - a;
		}
		else if((fx1 == fx2)&&(fy2 < fy1))  
		{
			schline.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schline.m_cVertex[0].fy = fy1 + a;
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schline.m_cVertex[2].fy = fy1 + a;		
		}
		else if((fy1 == fy2)&&(fx2 > fx1))		
		{
			schline.m_cVertex[0].fx = fx1 - a;
			schline.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 - a;
			schline.m_cVertex[2].fy = fy1 + a*tan(alpha);
		}
		else if((fy1 == fy2)&&(fx2 < fx1))
		{
			schline.m_cVertex[0].fx = fx1 + a;
			schline.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 + a;
			schline.m_cVertex[2].fy = fy1 + a*tan(alpha);		
		}
		else
		{
			double k1, k2;
			k1 = (fy2-fy1)/(fx2-fx1);
			beta = atan(k1);
			k2 = tan(beta + alpha);
			if(fx2 > fx1)
			{
				schline.m_cVertex[0].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[0].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
			else
			{
				schline.m_cVertex[0].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[0].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );			
			}
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;	
			k2 = tan(beta - alpha);
			if(fx2 > fx1)
			{
				schline.m_cVertex[2].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[2].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
			else
			{
				schline.m_cVertex[2].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[2].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
		}
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

		//another
		if((fx1 == fx2)&&(fy2 > fy1))	
		{
			schline.m_cVertex[0].fy += a;
			schline.m_cVertex[1].fy += a;
			schline.m_cVertex[2].fy += a;
		}
		else if((fx1 == fx2)&&(fy2 < fy1)) 
		{
			schline.m_cVertex[0].fy -= a;
			schline.m_cVertex[1].fy -= a;
			schline.m_cVertex[2].fy -= a;		
		}
		else if((fy1 == fy2)&&(fx2 > fx1))	
		{
			schline.m_cVertex[0].fx += a;
			schline.m_cVertex[1].fx += a;
			schline.m_cVertex[2].fx += a;
		}
		else if((fy1 == fy2)&&(fx2 < fx1)) 
		{
			schline.m_cVertex[0].fx -= a;
			schline.m_cVertex[1].fx -= a;
			schline.m_cVertex[2].fx -= a;
		}
		else
		{
			double k1, k2;
			k1 = (fy2-fy1)/(fx2-fx1);
			beta = atan(k1);
			k2 = tan(beta + alpha);
			if(fx2 > fx1)
			{
				schline.m_cVertex[0].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) + a*cos(beta);
				schline.m_cVertex[0].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) + a*sin(beta);
			}
			else
			{
				schline.m_cVertex[0].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) - a*cos(beta);
				schline.m_cVertex[0].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) - a*sin(beta);			
			}
			if(fx2 > fx1)
			{
				schline.m_cVertex[1].fx = fx1 + a*cos(beta);
				schline.m_cVertex[1].fy = fy1 + a*sin(beta);
			}
			else
			{
				schline.m_cVertex[1].fx = fx1 - a*cos(beta);
				schline.m_cVertex[1].fy = fy1 - a*sin(beta);
			}
			k2 = tan(beta - alpha);
			if(fx2 > fx1)
			{
				schline.m_cVertex[2].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) + a*cos(beta);
				schline.m_cVertex[2].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) + a*sin(beta);
			}
			else
			{
				schline.m_cVertex[2].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) - a*cos(beta);
				schline.m_cVertex[2].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) - a*sin(beta);	
			}
		}
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nStartLineShape == 4)
	{
		COb_schpolygon schpolygon;
		schpolygon.m_nFlag = m_nFlag;
		schpolygon.m_nDiagram = m_nDiagram;
		schpolygon.m_nBorderWidth = 0;
		schpolygon.m_nBorderColor = m_nColor;
		schpolygon.m_nFillColor = m_nColor;
		schpolygon.m_bFillSolid = TRUE;
		schpolygon.m_bTransparent = FALSE;
		schpolygon.m_cVertex.SetSize(6);

		double alpha, beta;
		alpha = 36.870f*fEDA_pi/180.0f;
		
		double fx1, fy1, fx2, fy2;
		fx1 = m_cVertex[0].fx;
		fy1 = m_cVertex[0].fy;
		fx2 = m_cVertex[1].fx;
		fy2 = m_cVertex[1].fy;

		double b;
		if(m_nLineShapeSize == 0) b = 10;
		else if(m_nLineShapeSize == 1) b = 20;
		else if(m_nLineShapeSize == 2) b = 30;
		else if(m_nLineShapeSize == 3) b = 40;

		if((fx1 == fx2)&&(fy2 > fy1))		
		{
			fy1 -= b;
		}
		else if((fx1 == fx2)&&(fy2 < fy1)) 
		{
			fy1 += b;
		}
		else if((fy1 == fy2)&&(fx2 > fx1))	
		{
			fx1 -= b;
		}
		else if((fy1 == fy2)&&(fx2 < fx1)) 
		{
			fx1 += b;
		}
		else
		{
			double k1;
			k1 = (fy2-fy1)/(fx2-fx1);
			beta = atan(k1);
			if(fx2 > fx1)
			{
				fx1 = fx1 - b*cos(beta);
				fy1 = fy1 - b*sin(beta);
			}
			else
			{
				fx1 = fx1 + b*cos(beta);
				fy1 = fy1 + b*sin(beta);	
			}

		}

		double a;
		if(m_nLineShapeSize == 0) a = 20;
		else if(m_nLineShapeSize == 1) a = 40;
		else if(m_nLineShapeSize == 2) a = 60;
		else if(m_nLineShapeSize == 3) a = 80;

		//one
		if((fx1 == fx2)&&(fy2 > fy1))	
		{
			schpolygon.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schpolygon.m_cVertex[0].fy = fy1 - a;
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schpolygon.m_cVertex[2].fy = fy1 - a;

			schpolygon.m_cVertex[3].fx = schpolygon.m_cVertex[2].fx;
			schpolygon.m_cVertex[3].fy = schpolygon.m_cVertex[2].fy + a*1.5f;
			schpolygon.m_cVertex[4].fx = schpolygon.m_cVertex[1].fx;
			schpolygon.m_cVertex[4].fy = schpolygon.m_cVertex[1].fy + a*1.5f;
			schpolygon.m_cVertex[5].fx = schpolygon.m_cVertex[0].fx;
			schpolygon.m_cVertex[5].fy = schpolygon.m_cVertex[0].fy + a*1.5f;
		}
		else if((fx1 == fx2)&&(fy2 < fy1)) 
		{
			schpolygon.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schpolygon.m_cVertex[0].fy = fy1 + a;
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schpolygon.m_cVertex[2].fy = fy1 + a;	

			schpolygon.m_cVertex[3].fx = schpolygon.m_cVertex[2].fx;
			schpolygon.m_cVertex[3].fy = schpolygon.m_cVertex[2].fy - a*1.5f;
			schpolygon.m_cVertex[4].fx = schpolygon.m_cVertex[1].fx;
			schpolygon.m_cVertex[4].fy = schpolygon.m_cVertex[1].fy - a*1.5f;
			schpolygon.m_cVertex[5].fx = schpolygon.m_cVertex[0].fx;
			schpolygon.m_cVertex[5].fy = schpolygon.m_cVertex[0].fy - a*1.5f;
		}
		else if((fy1 == fy2)&&(fx2 > fx1))		
		{
			schpolygon.m_cVertex[0].fx = fx1 - a;
			schpolygon.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 - a;
			schpolygon.m_cVertex[2].fy = fy1 + a*tan(alpha);

			schpolygon.m_cVertex[3].fx = schpolygon.m_cVertex[2].fx + a*1.5f;
			schpolygon.m_cVertex[3].fy = schpolygon.m_cVertex[2].fy;
			schpolygon.m_cVertex[4].fx = schpolygon.m_cVertex[1].fx + a*1.5f;
			schpolygon.m_cVertex[4].fy = schpolygon.m_cVertex[1].fy;
			schpolygon.m_cVertex[5].fx = schpolygon.m_cVertex[0].fx + a*1.5f;
			schpolygon.m_cVertex[5].fy = schpolygon.m_cVertex[0].fy;
		}
		else if((fy1 == fy2)&&(fx2 < fx1)) 
		{
			schpolygon.m_cVertex[0].fx = fx1 + a;
			schpolygon.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 + a;
			schpolygon.m_cVertex[2].fy = fy1 + a*tan(alpha);
			
			schpolygon.m_cVertex[3].fx = schpolygon.m_cVertex[2].fx - a*1.5f;
			schpolygon.m_cVertex[3].fy = schpolygon.m_cVertex[2].fy;
			schpolygon.m_cVertex[4].fx = schpolygon.m_cVertex[1].fx - a*1.5f;
			schpolygon.m_cVertex[4].fy = schpolygon.m_cVertex[1].fy;
			schpolygon.m_cVertex[5].fx = schpolygon.m_cVertex[0].fx - a*1.5f;
			schpolygon.m_cVertex[5].fy = schpolygon.m_cVertex[0].fy;
		}
		else
		{
			double k1, k2;
			k1 = (fy2-fy1)/(fx2-fx1);
			beta = atan(k1);
			if(fx2 > fx1)
			{
				k2 = tan(beta + alpha);
				schpolygon.m_cVertex[0].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[0].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );

				schpolygon.m_cVertex[1].fx = fx1;
				schpolygon.m_cVertex[1].fy = fy1;	

				k2 = tan(beta - alpha);
				schpolygon.m_cVertex[2].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[2].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );

				schpolygon.m_cVertex[3].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) + a*1.5f*cos(beta);
				schpolygon.m_cVertex[3].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) + a*1.5f*sin(beta);

				schpolygon.m_cVertex[4].fx = fx1 + a*1.5f*cos(beta);
				schpolygon.m_cVertex[4].fy = fy1 + a*1.5f*sin(beta);
				
				k2 = tan(beta + alpha);
				schpolygon.m_cVertex[5].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) + a*1.5f*cos(beta);
				schpolygon.m_cVertex[5].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) + a*1.5f*sin(beta);
			}
			else
			{
				k2 = tan(beta + alpha);
				schpolygon.m_cVertex[0].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[0].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );		

				schpolygon.m_cVertex[1].fx = fx1;
				schpolygon.m_cVertex[1].fy = fy1;

				k2 = tan(beta - alpha);
				schpolygon.m_cVertex[2].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[2].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );

				schpolygon.m_cVertex[3].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) - a*1.5f*cos(beta);
				schpolygon.m_cVertex[3].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) - a*1.5f*sin(beta);	

				schpolygon.m_cVertex[4].fx = fx1 - a*1.5f*cos(beta);
				schpolygon.m_cVertex[4].fy = fy1 - a*1.5f*sin(beta);

				k2 = tan(beta + alpha);
				schpolygon.m_cVertex[5].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) - a*1.5f*cos(beta);
				schpolygon.m_cVertex[5].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) - a*1.5f*sin(beta);		
			}

		}
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);
	}
	else if(m_nStartLineShape == 5)
	{
		COb_schellipsefill schef;
		schef.m_nFlag = m_nFlag;
		schef.m_nDiagram = m_nDiagram;
		
		schef.m_nBorderColor = m_nColor;
		schef.m_nFillColor = m_nColor;
		schef.m_bFillSolid = TRUE;
		schef.m_bTransparent = FALSE;
      
		schef.m_nBorderWidth = 0;
		schef.m_fCx = m_cVertex[0].fx;
		schef.m_fCy = m_cVertex[0].fy;
		if(m_nLineShapeSize == 0) { schef.m_fRadius_x = schef.m_fRadius_y = 15; }
		else if(m_nLineShapeSize == 1) { schef.m_fRadius_x = schef.m_fRadius_y = 25; }
		else if(m_nLineShapeSize == 2) { schef.m_fRadius_x = schef.m_fRadius_y = 35; }
		else if(m_nLineShapeSize == 3) { schef.m_fRadius_x = schef.m_fRadius_y = 45; }

		if(schef.PickMe(pV) ==TRUE) schef.Draw(pDC, pV);
	}
	else if(m_nStartLineShape == 6)
	{
		COb_schpolygon schpolygon;
		schpolygon.m_nFlag = m_nFlag;
		schpolygon.m_nDiagram = m_nDiagram;
		schpolygon.m_nBorderWidth = 0;
		schpolygon.m_nBorderColor = m_nColor;
		schpolygon.m_nFillColor = m_nColor;
		schpolygon.m_bFillSolid = TRUE;
		schpolygon.m_bTransparent = FALSE;
		schpolygon.m_cVertex.SetSize(4);

		double alpha, beta;
		alpha = 45.0f*fEDA_pi/180.0f; 
	
		double fx1, fy1, fx2, fy2;
		fx1 = m_cVertex[0].fx;
		fy1 = m_cVertex[0].fy;
		fx2 = m_cVertex[1].fx;
		fy2 = m_cVertex[1].fy;


		double a;
		if(m_nLineShapeSize == 0) a = 15;
		else if(m_nLineShapeSize == 1) a = 30;
		else if(m_nLineShapeSize == 2) a = 45;
		else if(m_nLineShapeSize == 3) a = 60;
		if((fx1 == fx2)||(fy1 == fy2))
		{
			schpolygon.m_cVertex[0].fx = fx1 - a;
			schpolygon.m_cVertex[0].fy = fy1 - a;
			schpolygon.m_cVertex[1].fx = fx1 + a;
			schpolygon.m_cVertex[1].fy = fy1 - a;
			schpolygon.m_cVertex[2].fx = fx1 + a;
			schpolygon.m_cVertex[2].fy = fy1 + a;
			schpolygon.m_cVertex[3].fx = fx1 - a;
			schpolygon.m_cVertex[3].fy = fy1 + a;
		}
		else
		{
			double k1, k2;
			k1 = (fy2-fy1)/(fx2-fx1);
			if(abs(k1) == 1) k1 += 0.001f;   
			beta = atan(k1);
				k2 = tan(beta + alpha);
				schpolygon.m_cVertex[0].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[0].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
				k2 = tan(beta - alpha);
				schpolygon.m_cVertex[1].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[1].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
				k2 = tan(beta + alpha);
				schpolygon.m_cVertex[2].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[2].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
				k2 = tan(beta - alpha);
				schpolygon.m_cVertex[3].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[3].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );


		}		
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);
	}

	if(m_nEndLineShape == 0) {}
	else if(m_nEndLineShape == 1)
	{
		double alpha, beta;
		alpha = 18*fEDA_pi/180; 
	
		schline.m_cVertex.SetSize(3);

		int index = m_cVertex.GetCount();
		double fx1, fy1, fx2, fy2;
		fx1 = m_cVertex[index-1].fx;
		fy1 = m_cVertex[index-1].fy;
		fx2 = m_cVertex[index-2].fx;
		fy2 = m_cVertex[index-2].fy;


		double a;
		if(m_nLineShapeSize == 0) a = 40;
		else if(m_nLineShapeSize == 1) a = 80;
		else if(m_nLineShapeSize == 2) a = 120;
		else if(m_nLineShapeSize == 3) a = 160;
		if((fx1 == fx2)&&(fy2 > fy1))	
		{
			schline.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schline.m_cVertex[0].fy = fy1 + a;
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schline.m_cVertex[2].fy = fy1 + a;
		}
		else if((fx1 == fx2)&&(fy2 < fy1)) 
		{
			schline.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schline.m_cVertex[0].fy = fy1 - a;
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schline.m_cVertex[2].fy = fy1 - a;		
		}
		else if((fy1 == fy2)&&(fx2 > fx1))	
		{
			schline.m_cVertex[0].fx = fx1 + a;
			schline.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 + a;
			schline.m_cVertex[2].fy = fy1 + a*tan(alpha);
		}
		else if((fy1 == fy2)&&(fx2 < fx1)) 
		{
			schline.m_cVertex[0].fx = fx1 - a;
			schline.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 - a;
			schline.m_cVertex[2].fy = fy1 + a*tan(alpha);		
		}
		else
		{
			double k1, k2;
			k1 = (fy2-fy1)/(fx2-fx1);
			beta = atan(k1);
			k2 = tan(beta + alpha);
			if(fx2 > fx1)
			{
				schline.m_cVertex[0].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[0].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
			else
			{
				schline.m_cVertex[0].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[0].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );			
			}
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;	
			k2 = tan(beta - alpha);
			if(fx2 > fx1)
			{
				schline.m_cVertex[2].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[2].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
			else
			{
				schline.m_cVertex[2].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[2].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
		}
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nEndLineShape == 2)
	{
		COb_schpolygon schpolygon;
		schpolygon.m_nFlag = m_nFlag;
		schpolygon.m_nDiagram = m_nDiagram;
		schpolygon.m_nBorderWidth = 0;
		schpolygon.m_nBorderColor = m_nColor;
		schpolygon.m_nFillColor = m_nColor;
		schpolygon.m_bFillSolid = TRUE;
		schpolygon.m_bTransparent = FALSE;
		schpolygon.m_cVertex.SetSize(3);
		
		double alpha, beta;
		alpha = 18.0f*fEDA_pi/180.0f; 
	
		int index = m_cVertex.GetCount();
		double fx1, fy1, fx2, fy2;
		fx1 = m_cVertex[index-1].fx;
		fy1 = m_cVertex[index-1].fy;
		fx2 = m_cVertex[index-2].fx;
		fy2 = m_cVertex[index-2].fy;

		double b;
		if(m_nLineShapeSize == 0) b = 15;
		else if(m_nLineShapeSize == 1) b = 30;
		else if(m_nLineShapeSize == 2) b = 45;
		else if(m_nLineShapeSize == 3) b = 60;

		if((fx1 == fx2)&&(fy2 > fy1))		
		{
			fy1 -= b;
		}
		else if((fx1 == fx2)&&(fy2 < fy1))
		{
			fy1 += b;
		}
		else if((fy1 == fy2)&&(fx2 > fx1))		
		{
			fx1 -= b;
		}
		else if((fy1 == fy2)&&(fx2 < fx1))  
		{
			fx1 += b;
		}
		else
		{
			double k1;
			k1 = (fy2-fy1)/(fx2-fx1);
			beta = atan(k1);
			if(fx2 > fx1)
			{
				fx1 = fx1 - b*cos(beta);
				fy1 = fy1 - b*sin(beta);
			}
			else
			{
				fx1 = fx1 + b*cos(beta);
				fy1 = fy1 + b*sin(beta);	
			}

		}

		double a;
		if(m_nLineShapeSize == 0) a = 55;
		else if(m_nLineShapeSize == 1) a = 110;
		else if(m_nLineShapeSize == 2) a = 165;
		else if(m_nLineShapeSize == 3) a = 220;
		if((fx1 == fx2)&&(fy2 > fy1))		
		{
			schpolygon.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schpolygon.m_cVertex[0].fy = fy1 + a;
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schpolygon.m_cVertex[2].fy = fy1 + a;
		}
		else if((fx1 == fx2)&&(fy2 < fy1))  
		{
			schpolygon.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schpolygon.m_cVertex[0].fy = fy1 - a;
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schpolygon.m_cVertex[2].fy = fy1 - a;		
		}
		else if((fy1 == fy2)&&(fx2 > fx1))	
		{
			schpolygon.m_cVertex[0].fx = fx1 + a;
			schpolygon.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 + a;
			schpolygon.m_cVertex[2].fy = fy1 + a*tan(alpha);
		}
		else if((fy1 == fy2)&&(fx2 < fx1))  
		{
			schpolygon.m_cVertex[0].fx = fx1 - a;
			schpolygon.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 - a;
			schpolygon.m_cVertex[2].fy = fy1 + a*tan(alpha);		
		}
		else
		{
			double k1, k2;
			k1 = (fy2-fy1)/(fx2-fx1);
			beta = atan(k1);
			k2 = tan(beta + alpha);
			if(fx2 > fx1)
			{
				schpolygon.m_cVertex[0].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[0].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
			else
			{
				schpolygon.m_cVertex[0].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[0].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );			
			}
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;	
			k2 = tan(beta - alpha);
			if(fx2 > fx1)
			{
				schpolygon.m_cVertex[2].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[2].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
			else
			{
				schpolygon.m_cVertex[2].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[2].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
		}
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);
	}
	else if(m_nEndLineShape == 3)
	{
		double alpha, beta;
		alpha = 26.565f*fEDA_pi/180.0f; 
	
		schline.m_cVertex.SetSize(3);

		int index = m_cVertex.GetCount();
		double fx1, fy1, fx2, fy2;
		fx1 = m_cVertex[index-1].fx;
		fy1 = m_cVertex[index-1].fy;
		fx2 = m_cVertex[index-2].fx;
		fy2 = m_cVertex[index-2].fy;

		double a;
		if(m_nLineShapeSize == 0) a = 20;
		else if(m_nLineShapeSize == 1) a = 40;
		else if(m_nLineShapeSize == 2) a = 60;
		else if(m_nLineShapeSize == 3) a = 80;

		//one
		if((fx1 == fx2)&&(fy2 > fy1))	
		{
			schline.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schline.m_cVertex[0].fy = fy1 - a;
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schline.m_cVertex[2].fy = fy1 - a;
		}
		else if((fx1 == fx2)&&(fy2 < fy1)) 
		{
			schline.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schline.m_cVertex[0].fy = fy1 + a;
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schline.m_cVertex[2].fy = fy1 + a;		
		}
		else if((fy1 == fy2)&&(fx2 > fx1))	
		{
			schline.m_cVertex[0].fx = fx1 - a;
			schline.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 - a;
			schline.m_cVertex[2].fy = fy1 + a*tan(alpha);
		}
		else if((fy1 == fy2)&&(fx2 < fx1)) 
		{
			schline.m_cVertex[0].fx = fx1 + a;
			schline.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;
			schline.m_cVertex[2].fx = fx1 + a;
			schline.m_cVertex[2].fy = fy1 + a*tan(alpha);		
		}
		else
		{
			double k1, k2;
			k1 = (fy2-fy1)/(fx2-fx1);
			beta = atan(k1);
			k2 = tan(beta + alpha);
			if(fx2 > fx1)
			{
				schline.m_cVertex[0].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[0].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
			else
			{
				schline.m_cVertex[0].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[0].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );			
			}
			schline.m_cVertex[1].fx = fx1;
			schline.m_cVertex[1].fy = fy1;	
			k2 = tan(beta - alpha);
			if(fx2 > fx1)
			{
				schline.m_cVertex[2].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[2].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
			else
			{
				schline.m_cVertex[2].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schline.m_cVertex[2].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
			}
		}
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);

		//another
		if((fx1 == fx2)&&(fy2 > fy1))		
		{
			schline.m_cVertex[0].fy += a;
			schline.m_cVertex[1].fy += a;
			schline.m_cVertex[2].fy += a;
		}
		else if((fx1 == fx2)&&(fy2 < fy1))  
		{
			schline.m_cVertex[0].fy -= a;
			schline.m_cVertex[1].fy -= a;
			schline.m_cVertex[2].fy -= a;		
		}
		else if((fy1 == fy2)&&(fx2 > fx1))	
		{
			schline.m_cVertex[0].fx += a;
			schline.m_cVertex[1].fx += a;
			schline.m_cVertex[2].fx += a;
		}
		else if((fy1 == fy2)&&(fx2 < fx1))  
		{
			schline.m_cVertex[0].fx -= a;
			schline.m_cVertex[1].fx -= a;
			schline.m_cVertex[2].fx -= a;
		}
		else
		{
			double k1, k2;
			k1 = (fy2-fy1)/(fx2-fx1);
			beta = atan(k1);
			k2 = tan(beta + alpha);
			if(fx2 > fx1)
			{
				schline.m_cVertex[0].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) + a*cos(beta);
				schline.m_cVertex[0].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) + a*sin(beta);
			}
			else
			{
				schline.m_cVertex[0].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) - a*cos(beta);
				schline.m_cVertex[0].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) - a*sin(beta);			
			}
			if(fx2 > fx1)
			{
				schline.m_cVertex[1].fx = fx1 + a*cos(beta);
				schline.m_cVertex[1].fy = fy1 + a*sin(beta);
			}
			else
			{
				schline.m_cVertex[1].fx = fx1 - a*cos(beta);
				schline.m_cVertex[1].fy = fy1 - a*sin(beta);
			}
			k2 = tan(beta - alpha);
			if(fx2 > fx1)
			{
				schline.m_cVertex[2].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) + a*cos(beta);
				schline.m_cVertex[2].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) + a*sin(beta);
			}
			else
			{
				schline.m_cVertex[2].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) - a*cos(beta);
				schline.m_cVertex[2].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) - a*sin(beta);	
			}
		}
		if(schline.PickMe(pV) == TRUE) schline.Draw(pDC, pV);
	}
	else if(m_nEndLineShape == 4)
	{
		COb_schpolygon schpolygon;
		schpolygon.m_nFlag = m_nFlag;
		schpolygon.m_nDiagram = m_nDiagram;
		schpolygon.m_nBorderWidth = 0;
		schpolygon.m_nBorderColor = m_nColor;
		schpolygon.m_nFillColor = m_nColor;
		schpolygon.m_bFillSolid = TRUE;
		schpolygon.m_bTransparent = FALSE;
		schpolygon.m_cVertex.SetSize(6);

		double alpha, beta;
		alpha = 36.870f*fEDA_pi/180.0f;  
		
		int index = m_cVertex.GetCount();
		double fx1, fy1, fx2, fy2;
		fx1 = m_cVertex[index-1].fx;
		fy1 = m_cVertex[index-1].fy;
		fx2 = m_cVertex[index-2].fx;
		fy2 = m_cVertex[index-2].fy;

		double b;
		if(m_nLineShapeSize == 0) b = 10;
		else if(m_nLineShapeSize == 1) b = 20;
		else if(m_nLineShapeSize == 2) b = 30;
		else if(m_nLineShapeSize == 3) b = 40;

		if((fx1 == fx2)&&(fy2 > fy1))		
		{
			fy1 -= b;
		}
		else if((fx1 == fx2)&&(fy2 < fy1))  
		{
			fy1 += b;
		}
		else if((fy1 == fy2)&&(fx2 > fx1))	
		{
			fx1 -= b;
		}
		else if((fy1 == fy2)&&(fx2 < fx1))  
		{
			fx1 += b;
		}
		else
		{
			double k1;
			k1 = (fy2-fy1)/(fx2-fx1);
			beta = atan(k1);
			if(fx2 > fx1)
			{
				fx1 = fx1 - b*cos(beta);
				fy1 = fy1 - b*sin(beta);
			}
			else
			{
				fx1 = fx1 + b*cos(beta);
				fy1 = fy1 + b*sin(beta);	
			}

		}

		double a;
		if(m_nLineShapeSize == 0) a = 20;
		else if(m_nLineShapeSize == 1) a = 40;
		else if(m_nLineShapeSize == 2) a = 60;
		else if(m_nLineShapeSize == 3) a = 80;

		//one
		if((fx1 == fx2)&&(fy2 > fy1))		
		{
			schpolygon.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schpolygon.m_cVertex[0].fy = fy1 - a;
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schpolygon.m_cVertex[2].fy = fy1 - a;

			schpolygon.m_cVertex[3].fx = schpolygon.m_cVertex[2].fx;
			schpolygon.m_cVertex[3].fy = schpolygon.m_cVertex[2].fy + a*1.5f;
			schpolygon.m_cVertex[4].fx = schpolygon.m_cVertex[1].fx;
			schpolygon.m_cVertex[4].fy = schpolygon.m_cVertex[1].fy + a*1.5f;
			schpolygon.m_cVertex[5].fx = schpolygon.m_cVertex[0].fx;
			schpolygon.m_cVertex[5].fy = schpolygon.m_cVertex[0].fy + a*1.5f;
		}
		else if((fx1 == fx2)&&(fy2 < fy1)) 
		{
			schpolygon.m_cVertex[0].fx = fx1 - a*tan(alpha);
			schpolygon.m_cVertex[0].fy = fy1 + a;
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 + a*tan(alpha);
			schpolygon.m_cVertex[2].fy = fy1 + a;	

			schpolygon.m_cVertex[3].fx = schpolygon.m_cVertex[2].fx;
			schpolygon.m_cVertex[3].fy = schpolygon.m_cVertex[2].fy - a*1.5f;
			schpolygon.m_cVertex[4].fx = schpolygon.m_cVertex[1].fx;
			schpolygon.m_cVertex[4].fy = schpolygon.m_cVertex[1].fy - a*1.5f;
			schpolygon.m_cVertex[5].fx = schpolygon.m_cVertex[0].fx;
			schpolygon.m_cVertex[5].fy = schpolygon.m_cVertex[0].fy - a*1.5f;
		}
		else if((fy1 == fy2)&&(fx2 > fx1))	
		{
			schpolygon.m_cVertex[0].fx = fx1 - a;
			schpolygon.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 - a;
			schpolygon.m_cVertex[2].fy = fy1 + a*tan(alpha);

			schpolygon.m_cVertex[3].fx = schpolygon.m_cVertex[2].fx + a*1.5f;
			schpolygon.m_cVertex[3].fy = schpolygon.m_cVertex[2].fy;
			schpolygon.m_cVertex[4].fx = schpolygon.m_cVertex[1].fx + a*1.5f;
			schpolygon.m_cVertex[4].fy = schpolygon.m_cVertex[1].fy;
			schpolygon.m_cVertex[5].fx = schpolygon.m_cVertex[0].fx + a*1.5f;
			schpolygon.m_cVertex[5].fy = schpolygon.m_cVertex[0].fy;
		}
		else if((fy1 == fy2)&&(fx2 < fx1)) 
		{
			schpolygon.m_cVertex[0].fx = fx1 + a;
			schpolygon.m_cVertex[0].fy = fy1 - a*tan(alpha);
			schpolygon.m_cVertex[1].fx = fx1;
			schpolygon.m_cVertex[1].fy = fy1;
			schpolygon.m_cVertex[2].fx = fx1 + a;
			schpolygon.m_cVertex[2].fy = fy1 + a*tan(alpha);
			
			schpolygon.m_cVertex[3].fx = schpolygon.m_cVertex[2].fx - a*1.5f;
			schpolygon.m_cVertex[3].fy = schpolygon.m_cVertex[2].fy;
			schpolygon.m_cVertex[4].fx = schpolygon.m_cVertex[1].fx - a*1.5f;
			schpolygon.m_cVertex[4].fy = schpolygon.m_cVertex[1].fy;
			schpolygon.m_cVertex[5].fx = schpolygon.m_cVertex[0].fx - a*1.5f;
			schpolygon.m_cVertex[5].fy = schpolygon.m_cVertex[0].fy;
		}
		else
		{
			double k1, k2;
			k1 = (fy2-fy1)/(fx2-fx1);
			beta = atan(k1);
			if(fx2 > fx1)
			{
				k2 = tan(beta + alpha);
				schpolygon.m_cVertex[0].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[0].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );

				schpolygon.m_cVertex[1].fx = fx1;
				schpolygon.m_cVertex[1].fy = fy1;	

				k2 = tan(beta - alpha);
				schpolygon.m_cVertex[2].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[2].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );

				schpolygon.m_cVertex[3].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) + a*1.5f*cos(beta);
				schpolygon.m_cVertex[3].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) + a*1.5f*sin(beta);

				schpolygon.m_cVertex[4].fx = fx1 + a*1.5f*cos(beta);
				schpolygon.m_cVertex[4].fy = fy1 + a*1.5f*sin(beta);
				
				k2 = tan(beta + alpha);
				schpolygon.m_cVertex[5].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) + a*1.5f*cos(beta);
				schpolygon.m_cVertex[5].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) + a*1.5f*sin(beta);
			}
			else
			{
				k2 = tan(beta + alpha);
				schpolygon.m_cVertex[0].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[0].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );		

				schpolygon.m_cVertex[1].fx = fx1;
				schpolygon.m_cVertex[1].fy = fy1;

				k2 = tan(beta - alpha);
				schpolygon.m_cVertex[2].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[2].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );

				schpolygon.m_cVertex[3].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) - a*1.5f*cos(beta);
				schpolygon.m_cVertex[3].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) - a*1.5f*sin(beta);	

				schpolygon.m_cVertex[4].fx = fx1 - a*1.5f*cos(beta);
				schpolygon.m_cVertex[4].fy = fy1 - a*1.5f*sin(beta);

				k2 = tan(beta + alpha);
				schpolygon.m_cVertex[5].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) ) - a*1.5f*cos(beta);
				schpolygon.m_cVertex[5].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) ) - a*1.5f*sin(beta);		
			}

		}
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);
	}
	else if(m_nEndLineShape == 5)
	{
		COb_schellipsefill schef;
		schef.m_nFlag = m_nFlag;
		schef.m_nDiagram = m_nDiagram;
		
		schef.m_nBorderColor = m_nColor;
		schef.m_nFillColor = m_nColor;
		schef.m_bFillSolid = TRUE;
		schef.m_bTransparent = FALSE;
      
		schef.m_nBorderWidth = 0;

		int index = m_cVertex.GetCount();
		schef.m_fCx = m_cVertex[index-1].fx;
		schef.m_fCy = m_cVertex[index-1].fy;

		if(m_nLineShapeSize == 0) { schef.m_fRadius_x = schef.m_fRadius_y = 15; }
		else if(m_nLineShapeSize == 1) { schef.m_fRadius_x = schef.m_fRadius_y = 25; }
		else if(m_nLineShapeSize == 2) { schef.m_fRadius_x = schef.m_fRadius_y = 35; }
		else if(m_nLineShapeSize == 3) { schef.m_fRadius_x = schef.m_fRadius_y = 45; }

		if(schef.PickMe(pV) ==TRUE) schef.Draw(pDC, pV);
	}
	else if(m_nEndLineShape == 6)
	{
		COb_schpolygon schpolygon;
		schpolygon.m_nFlag = m_nFlag;
		schpolygon.m_nDiagram = m_nDiagram;
		schpolygon.m_nBorderWidth = 0;
		schpolygon.m_nBorderColor = m_nColor;
		schpolygon.m_nFillColor = m_nColor;
		schpolygon.m_bFillSolid = TRUE;
		schpolygon.m_bTransparent = FALSE;
		schpolygon.m_cVertex.SetSize(4);

		double alpha, beta;
		alpha = 45.0f*fEDA_pi/180.0f; 
	
		int index = m_cVertex.GetCount();
		double fx1, fy1, fx2, fy2;
		fx1 = m_cVertex[index-1].fx;
		fy1 = m_cVertex[index-1].fy;
		fx2 = m_cVertex[index-2].fx;
		fy2 = m_cVertex[index-2].fy;

		double a;
		if(m_nLineShapeSize == 0) a = 15;
		else if(m_nLineShapeSize == 1) a = 30;
		else if(m_nLineShapeSize == 2) a = 45;
		else if(m_nLineShapeSize == 3) a = 60;
		if((fx1 == fx2)||(fy1 == fy2))
		{
			schpolygon.m_cVertex[0].fx = fx1 - a;
			schpolygon.m_cVertex[0].fy = fy1 - a;
			schpolygon.m_cVertex[1].fx = fx1 + a;
			schpolygon.m_cVertex[1].fy = fy1 - a;
			schpolygon.m_cVertex[2].fx = fx1 + a;
			schpolygon.m_cVertex[2].fy = fy1 + a;
			schpolygon.m_cVertex[3].fx = fx1 - a;
			schpolygon.m_cVertex[3].fy = fy1 + a;
		}
		else
		{
			double k1, k2;
			k1 = (fy2-fy1)/(fx2-fx1);
			if(abs(k1) == 1) k1 += 0.001f;  
			beta = atan(k1);
				k2 = tan(beta + alpha);
				schpolygon.m_cVertex[0].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[0].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
				k2 = tan(beta - alpha);
				schpolygon.m_cVertex[1].fx = fx1 + ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[1].fy = fy1 + ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
				k2 = tan(beta + alpha);
				schpolygon.m_cVertex[2].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[2].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );
				k2 = tan(beta - alpha);
				schpolygon.m_cVertex[3].fx = fx1 - ( a*(sin(beta) + cos(beta)/k1) / (k2 + 1/k1) );
				schpolygon.m_cVertex[3].fy = fy1 - ( a*(cos(beta) + k1*sin(beta)) / (k1 + 1/k2) );


		}		
		if(schpolygon.PickMe(pV) == TRUE) schpolygon.Draw(pDC, pV);
	}
}


void COb_schpolyline::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{

	COb_schline schline;
	schline.m_nFlag = m_nFlag;
	schline.m_nDiagram = m_nDiagram;
	schline.m_nAttrib = LINE_WIRE;
	schline.m_nColor = m_nColor;
	schline.m_nLineType = m_nLineType;
	schline.m_nLineWidth = m_nLineWidth;
	schline.m_cVertex.SetSize(m_cVertex.GetSize());

	for(int i =0; i<m_cVertex.GetSize(); i++)
	{
		schline.m_cVertex[i].fx = m_cVertex[i].fx;
		schline.m_cVertex[i].fy = m_cVertex[i].fy;
	}
	
	schline.DrawInWindow(pDC, scale, origin_x, origin_y, nEy);
}

void COb_schpolyline::DrawSelection(CDC* pDC, CView* pV)
{
	int x1,y1,x2,y2;
	CPen linepen, *oldpen;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	linepen.CreatePen(PS_DOT, 1, nEDA_SelectionColor);
	oldpen = pDC->SelectObject(&linepen);

	for(int i =0; i<m_cVertex.GetCount()-1; i++)
	{
		pView->Get_VPort_XY(&x1,&y1,m_cVertex[i].fx,m_cVertex[i].fy);
		pView->Get_VPort_XY(&x2,&y2,m_cVertex[i+1].fx,m_cVertex[i+1].fy);

		pDC->MoveTo(x1,y1);
		pDC->LineTo(x2,y2);
	}

	//要点
	for(int i =0; i<m_cVertex.GetCount(); i++)
	{
		int x,y;
		pView->Get_VPort_XY(&x,&y,m_cVertex[i].fx,m_cVertex[i].fy);
		pView->DrawSelectionKeyPoint(x, y, pDC);
	}
	for(int i =1; i<m_cVertex.GetCount(); i++)
	{
		int x,y;
		pView->Get_VPort_XY(&x,&y,(m_cVertex[i-1].fx+m_cVertex[i].fx)/2,(m_cVertex[i-1].fy + m_cVertex[i].fy)/2);
		pView->DrawSelectionKeyPoint(x, y, pDC);
	}

	pDC->SetBkMode(oldbkmode);
	pDC->SelectObject(oldpen);
}

BOOL COb_schpolyline::Is_On_Me(double fx, double fy)
{
	//if(diagram != m_nDiagram) return FALSE;

	double ax,ay,bx,by;
	double alpha;

	int width;
	width = nEDA_Line_Width[m_nLineWidth];

	int delta = (int)(width/2) + nEDA_Selection_Offset;


	int vertex = m_cVertex.GetCount();
	for(int i=0; i<vertex-1; i++)
	{
		ax = m_cVertex[i].fx;
		ay = m_cVertex[i].fy;
		bx = m_cVertex[i+1].fx;
		by = m_cVertex[i+1].fy;
		if(ax == bx) alpha = fEDA_pi/2;
		else if(ay == by) alpha = 0;
		else alpha = atan((by-ay)/(bx-ax));
		
		CRgn rgn;
		CPoint ptVertex[4];
		ptVertex[0].x = (int)(ax - delta*sin(alpha));
		ptVertex[0].y = (int)(ay + delta*cos(alpha));
		ptVertex[1].x = (int)(ax + delta*sin(alpha));
		ptVertex[1].y = (int)(ay - delta*cos(alpha));
		ptVertex[2].x = (int)(bx + delta*sin(alpha));
		ptVertex[2].y = (int)(by - delta*cos(alpha));
		ptVertex[3].x = (int)(bx - delta*sin(alpha));
		ptVertex[3].y = (int)(by + delta*cos(alpha));
	
	
		rgn.CreatePolygonRgn( ptVertex, 4, ALTERNATE);

		if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	}

	return FALSE;
}

BOOL  COb_schpolyline::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		double fx, fy;
		fx = m_cVertex[i].fx;
		fy = m_cVertex[i].fy;
		if((fx < fx1)||(fx > fx2)||(fy < fy1)||(fy > fy2)) return FALSE;
	}
	return TRUE;
}

void COb_schpolyline::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CSheet_EditPolyline  ps(_T("辅助线"), NULL, 0);

	ps.page1.m_nColor = m_nColor;
	ps.page1.m_nComboWidth = m_nLineWidth;
	ps.page1.m_nComboLineType = m_nLineType;
	ps.page1.m_nComboStartLineShape = m_nStartLineShape;
	ps.page1.m_nComboEndLineShape = m_nEndLineShape;
	ps.page1.m_nComboLineShapeSize = m_nLineShapeSize;


	ps.page2.m_listCtrl.m_arrayX.RemoveAll();
	ps.page2.m_listCtrl.m_arrayY.RemoveAll();
	ps.page2.m_listCtrl.m_arrayX.SetSize(m_cVertex.GetCount());
	ps.page2.m_listCtrl.m_arrayY.SetSize(m_cVertex.GetCount());
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		ps.page2.m_listCtrl.m_arrayX[i] = pDoc->Convert_XY_To_String(m_cVertex[i].fx);
		ps.page2.m_listCtrl.m_arrayY[i] = pDoc->Convert_XY_To_String(m_cVertex[i].fy);
	}
	
	if(ps.DoModal() == IDOK)
	{
		if(ps.page1.m_bCheckGlobal == TRUE)
		{
			CDlg_Global_Polyline dlg;
			if(dlg.DoModal() == IDOK)
			{

				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
						
				if(dlg.m_nRadioScope == 0)
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
								
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
							{
								COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
								if((ppolyline->m_nDiagram != nDiagram)||(ppolyline->m_nFlag != FLAG_NORMAL))	continue;
								if(ppolyline == this) continue; 

								if( (((dlg.m_bCheckColor == TRUE)&&(ppolyline->m_nColor == m_nColor))||(dlg.m_bCheckColor == FALSE)) &&
									(((dlg.m_bCheckWidth == TRUE)&&(ppolyline->m_nLineWidth == m_nLineWidth))||(dlg.m_bCheckWidth == FALSE)) &&
									(((dlg.m_bCheckStyle == TRUE)&&(ppolyline->m_nLineType == m_nLineType))||(dlg.m_bCheckStyle == FALSE)) &&
									(((dlg.m_bCheckStart == TRUE)&&(ppolyline->m_nStartLineShape == m_nStartLineShape))||(dlg.m_bCheckStart == FALSE)) &&
									(((dlg.m_bCheckEnd == TRUE)&&(ppolyline->m_nEndLineShape == m_nEndLineShape))||(dlg.m_bCheckEnd == FALSE)) &&
									(((dlg.m_bCheckSize == TRUE)&&(ppolyline->m_nLineShapeSize == m_nLineShapeSize))||(dlg.m_bCheckSize == FALSE)) )
								{
									if( (ppolyline->m_nColor == m_nColor)||(ppolyline->m_nLineWidth == m_nLineWidth)||(ppolyline->m_nLineType == m_nLineType)||\
										(ppolyline->m_nStartLineShape == m_nStartLineShape)||(ppolyline->m_nEndLineShape == m_nEndLineShape)||(ppolyline->m_nLineShapeSize == m_nLineShapeSize) )
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_POLYLINE;
										pundo->m_nDiagram = nDiagram;
										pundo->m_nPos = posbak;
										pundo->schpolyline.Copy(ppolyline);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

									}
									if(ppolyline->m_nColor == m_nColor) ppolyline->m_nColor = ps.page1.m_nColor;
									if(ppolyline->m_nLineWidth == m_nLineWidth) ppolyline->m_nLineWidth = ps.page1.m_nComboWidth;
									if(ppolyline->m_nLineType == m_nLineType) ppolyline->m_nLineType = ps.page1.m_nComboLineType;
									if(ppolyline->m_nStartLineShape == m_nStartLineShape) ppolyline->m_nStartLineShape = ps.page1.m_nComboStartLineShape;
									if(ppolyline->m_nEndLineShape == m_nEndLineShape) ppolyline->m_nEndLineShape = ps.page1.m_nComboEndLineShape;
									if(ppolyline->m_nLineShapeSize == m_nLineShapeSize) ppolyline->m_nLineShapeSize = ps.page1.m_nComboLineShapeSize;
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
				else if(dlg.m_nRadioScope == 1)
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
						{
							COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
							if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag != FLAG_NORMAL))	continue;
							if(ppolyline == this) continue;

							if( (((dlg.m_bCheckColor == TRUE)&&(ppolyline->m_nColor == m_nColor))||(dlg.m_bCheckColor == FALSE)) &&
								(((dlg.m_bCheckWidth == TRUE)&&(ppolyline->m_nLineWidth == m_nLineWidth))||(dlg.m_bCheckWidth == FALSE)) &&
								(((dlg.m_bCheckStyle == TRUE)&&(ppolyline->m_nLineType == m_nLineType))||(dlg.m_bCheckStyle == FALSE)) &&
								(((dlg.m_bCheckStart == TRUE)&&(ppolyline->m_nStartLineShape == m_nStartLineShape))||(dlg.m_bCheckStart == FALSE)) &&
								(((dlg.m_bCheckEnd == TRUE)&&(ppolyline->m_nEndLineShape == m_nEndLineShape))||(dlg.m_bCheckEnd == FALSE)) &&
								(((dlg.m_bCheckSize == TRUE)&&(ppolyline->m_nLineShapeSize == m_nLineShapeSize))||(dlg.m_bCheckSize == FALSE)) )
							{
								if( (ppolyline->m_nColor == m_nColor)||(ppolyline->m_nLineWidth == m_nLineWidth)||(ppolyline->m_nLineType == m_nLineType)||\
									(ppolyline->m_nStartLineShape == m_nStartLineShape)||(ppolyline->m_nEndLineShape == m_nEndLineShape)||(ppolyline->m_nLineShapeSize == m_nLineShapeSize) )
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_POLYLINE;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schpolyline.Copy(ppolyline);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								}
								if(ppolyline->m_nColor == m_nColor) ppolyline->m_nColor = ps.page1.m_nColor;
								if(ppolyline->m_nLineWidth == m_nLineWidth) ppolyline->m_nLineWidth = ps.page1.m_nComboWidth;
								if(ppolyline->m_nLineType == m_nLineType) ppolyline->m_nLineType = ps.page1.m_nComboLineType;
								if(ppolyline->m_nStartLineShape == m_nStartLineShape) ppolyline->m_nStartLineShape = ps.page1.m_nComboStartLineShape;
								if(ppolyline->m_nEndLineShape == m_nEndLineShape) ppolyline->m_nEndLineShape = ps.page1.m_nComboEndLineShape;
								if(ppolyline->m_nLineShapeSize == m_nLineShapeSize) ppolyline->m_nLineShapeSize = ps.page1.m_nComboLineShapeSize;
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
								
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
						{
							COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
							if((ppolyline->m_bSelection != TRUE)||(ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag != FLAG_NORMAL))	continue;
							if(ppolyline == this) continue; 

							if( (((dlg.m_bCheckColor == TRUE)&&(ppolyline->m_nColor == m_nColor))||(dlg.m_bCheckColor == FALSE)) &&
								(((dlg.m_bCheckWidth == TRUE)&&(ppolyline->m_nLineWidth == m_nLineWidth))||(dlg.m_bCheckWidth == FALSE)) &&
								(((dlg.m_bCheckStyle == TRUE)&&(ppolyline->m_nLineType == m_nLineType))||(dlg.m_bCheckStyle == FALSE)) &&
								(((dlg.m_bCheckStart == TRUE)&&(ppolyline->m_nStartLineShape == m_nStartLineShape))||(dlg.m_bCheckStart == FALSE)) &&
								(((dlg.m_bCheckEnd == TRUE)&&(ppolyline->m_nEndLineShape == m_nEndLineShape))||(dlg.m_bCheckEnd == FALSE)) &&
								(((dlg.m_bCheckSize == TRUE)&&(ppolyline->m_nLineShapeSize == m_nLineShapeSize))||(dlg.m_bCheckSize == FALSE)) )
							{
								if( (ppolyline->m_nColor == m_nColor)||(ppolyline->m_nLineWidth == m_nLineWidth)||(ppolyline->m_nLineType == m_nLineType)||\
									(ppolyline->m_nStartLineShape == m_nStartLineShape)||(ppolyline->m_nEndLineShape == m_nEndLineShape)||(ppolyline->m_nLineShapeSize == m_nLineShapeSize) )
								{
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_POLYLINE;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = posbak;
									pundo->schpolyline.Copy(ppolyline);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								}
								if(ppolyline->m_nColor == m_nColor) ppolyline->m_nColor = ps.page1.m_nColor;
								if(ppolyline->m_nLineWidth == m_nLineWidth) ppolyline->m_nLineWidth = ps.page1.m_nComboWidth;
								if(ppolyline->m_nLineType == m_nLineType) ppolyline->m_nLineType = ps.page1.m_nComboLineType;
								if(ppolyline->m_nStartLineShape == m_nStartLineShape) ppolyline->m_nStartLineShape = ps.page1.m_nComboStartLineShape;
								if(ppolyline->m_nEndLineShape == m_nEndLineShape) ppolyline->m_nEndLineShape = ps.page1.m_nComboEndLineShape;
								if(ppolyline->m_nLineShapeSize == m_nLineShapeSize) ppolyline->m_nLineShapeSize = ps.page1.m_nComboLineShapeSize;
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
				pundo->m_nOb = SCH_ELEMENT_POLYLINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schpolyline.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);


				m_nColor = ps.page1.m_nColor;
				m_nLineWidth = ps.page1.m_nComboWidth;
				m_nLineType = ps.page1.m_nComboLineType;
				m_nStartLineShape = ps.page1.m_nComboStartLineShape;
				m_nEndLineShape = ps.page1.m_nComboEndLineShape;
				m_nLineShapeSize = ps.page1.m_nComboLineShapeSize;

				m_cVertex.RemoveAll();
				m_cVertex.SetSize(ps.page2.m_listCtrl.m_arrayX.GetCount());
				for(int i=0; i<ps.page2.m_listCtrl.m_arrayX.GetCount(); i++)
				{
					m_cVertex[i].fx = pDoc->Convert_String_To_XY(ps.page2.m_listCtrl.m_arrayX[i]);
					m_cVertex[i].fy = pDoc->Convert_String_To_XY(ps.page2.m_listCtrl.m_arrayY[i]);
				}

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
			pundo->m_nOb = SCH_ELEMENT_POLYLINE;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->schpolyline.Copy(this);
					
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();


			m_nColor = ps.page1.m_nColor;
			m_nLineWidth = ps.page1.m_nComboWidth;
			m_nLineType = ps.page1.m_nComboLineType;
			m_nStartLineShape = ps.page1.m_nComboStartLineShape;
			m_nEndLineShape = ps.page1.m_nComboEndLineShape;
			m_nLineShapeSize = ps.page1.m_nComboLineShapeSize;

			m_cVertex.RemoveAll();
			m_cVertex.SetSize(ps.page2.m_listCtrl.m_arrayX.GetCount());
			for(int i=0; i<ps.page2.m_listCtrl.m_arrayX.GetCount(); i++)
			{
				m_cVertex[i].fx = pDoc->Convert_String_To_XY(ps.page2.m_listCtrl.m_arrayX[i]);
				m_cVertex[i].fy = pDoc->Convert_String_To_XY(ps.page2.m_listCtrl.m_arrayY[i]);
			}
		}
	}
}

void COb_schpolyline::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动连线: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV);

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	
	
	COb_schpolyline apolyline;
	apolyline.pDocument = pDoc;
	apolyline.m_nLineWidth = m_nLineWidth;
	apolyline.m_nLineType = m_nLineType;
	apolyline.m_nStartLineShape = m_nStartLineShape;
	apolyline.m_nEndLineShape = m_nEndLineShape;
	apolyline.m_nLineShapeSize = m_nLineShapeSize;
	apolyline.m_nColor = m_nColor;
	apolyline.m_nDiagram = m_nDiagram;
	apolyline.m_nFlag = FLAG_NORMAL;
	
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		Fpoint fpoint;
		fpoint = m_cVertex.GetAt(i);
		apolyline.m_cVertex.Add(fpoint);
	}
	apolyline.Draw(pDC, pV);

	x0 = pView->m_fCurrentSnapX;
	y0 = pView->m_fCurrentSnapY;

	double dis = 10000000;
	int index;
	for(int i=0; i<apolyline.m_cVertex.GetCount(); i++)
	{
		double d = sqrt((x0 - apolyline.m_cVertex[i].fx)*(x0 - apolyline.m_cVertex[i].fx) + (y0 - apolyline.m_cVertex[i].fy)*(y0 - apolyline.m_cVertex[i].fy));
		if(d < dis)
		{
			dis = d;
			index = i;
		}
	}
	x0 = apolyline.m_cVertex[index].fx;
	y0 = apolyline.m_cVertex[index].fy;

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
			apolyline.Draw(pDC, pV);
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
				apolyline.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<apolyline.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				apolyline.m_cVertex[i].fx = fx + x0 + deltax;
				apolyline.m_cVertex[i].fy = fy + y0 + deltay;
			}
			
			pView->CloseMouseCursor();
			apolyline.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			apolyline.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<apolyline.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				apolyline.m_cVertex[i].fx = fx + x0 + deltax;
				apolyline.m_cVertex[i].fy = fy + y0 + deltay;
			}

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_MOVE;
			pundo->m_nOb = SCH_ELEMENT_POLYLINE;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->m_cVertex.RemoveAll();
			for(int i=0; i<m_cVertex.GetCount(); i++)
			{
				Fpoint fpoint;
				fpoint = m_cVertex.GetAt(i);
				pundo->m_cVertex.Add(fpoint);
			}			
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			for(int i=0; i<m_cVertex.GetCount(); i++)
			{
				m_cVertex[i].fx = apolyline.m_cVertex[i].fx;
				m_cVertex[i].fy = apolyline.m_cVertex[i].fy;
			}
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			apolyline.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<apolyline.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				apolyline.m_cVertex[i].fx = fx + x0 + deltax;
				apolyline.m_cVertex[i].fy = fy + y0 + deltay;
			}
			apolyline.Draw(pDC, pV);

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

BOOL COb_schpolyline::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	
	char szHelp[] = "移动连线: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	x0 = m_cVertex[0].fx;
	y0 = m_cVertex[0].fy;
	
	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	
	
	COb_schpolyline apolyline;
	apolyline.pDocument = pDoc;
	apolyline.m_nLineWidth = m_nLineWidth;
	apolyline.m_nLineType = m_nLineType;
	apolyline.m_nStartLineShape = m_nStartLineShape;
	apolyline.m_nEndLineShape = m_nEndLineShape;
	apolyline.m_nLineShapeSize = m_nLineShapeSize;
	apolyline.m_nColor = m_nColor;
	apolyline.m_nDiagram = m_nDiagram;
	apolyline.m_nFlag = FLAG_NORMAL;
	
	for(int i=0; i<m_cVertex.GetCount(); i++)
	{
		Fpoint fpoint;
		fpoint = m_cVertex.GetAt(i);
		apolyline.m_cVertex.Add(fpoint);
	}

	apolyline.Draw(pDC, pV);
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
			apolyline.Draw(pDC, pV);
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
				apolyline.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<apolyline.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				apolyline.m_cVertex[i].fx = fx + x0 + deltax;
				apolyline.m_cVertex[i].fy = fy + y0 + deltay;
			}
			
			pView->CloseMouseCursor();
			apolyline.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			apolyline.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<apolyline.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				apolyline.m_cVertex[i].fx = fx + x0 + deltax;
				apolyline.m_cVertex[i].fy = fy + y0 + deltay;
			}

			for(int i=0; i<m_cVertex.GetCount(); i++)
			{
				m_cVertex[i].fx = apolyline.m_cVertex[i].fx;
				m_cVertex[i].fy = apolyline.m_cVertex[i].fy;
			}
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			apolyline.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<apolyline.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, m_cVertex[i].fx-x0, m_cVertex[i].fy-y0, flag*90, FALSE);
				apolyline.m_cVertex[i].fx = fx + x0 + deltax;
				apolyline.m_cVertex[i].fy = fy + y0 + deltay;
			}
			apolyline.Draw(pDC, pV);

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

	pView->Write_Help(szEDA_Help_Ready);
		
	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;
}


void COb_schpolyline::Copy(COb_schpolyline* pschpolyline)
{
	m_nLineWidth = pschpolyline->m_nLineWidth;
	m_nLineType = pschpolyline->m_nLineType;
	m_nStartLineShape = pschpolyline->m_nStartLineShape;
	m_nEndLineShape = pschpolyline->m_nEndLineShape;
	m_nLineShapeSize = pschpolyline->m_nLineShapeSize;
	m_nColor = pschpolyline->m_nColor;

	m_cVertex.RemoveAll();
	for(int i=0; i<pschpolyline->m_cVertex.GetCount(); i++)
	{
		Fpoint fp;
		fp = pschpolyline->m_cVertex.GetAt(i);
		m_cVertex.Add(fp);
	}	

	m_nOwnerPartID = pschpolyline->m_nOwnerPartID;
	m_nDisplayMode = pschpolyline->m_nDisplayMode;
	m_posParent = pschpolyline->m_posParent;

	m_nDiagram = pschpolyline->m_nDiagram;
	m_bSelection = pschpolyline->m_bSelection;
	m_nFlag = pschpolyline->m_nFlag;

	pDocument = pschpolyline->pDocument;
}

void COb_schpolyline::CombineVertex(void)
{
	for(int k=2; k<m_cVertex.GetCount(); k++)
	{
		if(m_cVertex[k-2].fx == m_cVertex[k-1].fx)
		{
			if(m_cVertex[k-1].fx == m_cVertex[k].fx)
			{
				m_cVertex.RemoveAt(k-1);
				k--;
			}
		}
		else if(m_cVertex[k-2].fy == m_cVertex[k-1].fy)
		{
			if(m_cVertex[k-1].fy == m_cVertex[k].fy)
			{
				m_cVertex.RemoveAt(k-1);
				k--;
			}
		}
	}
}