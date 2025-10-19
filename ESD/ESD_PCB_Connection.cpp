#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"


IMPLEMENT_SERIAL(COb_pcbconnection, CObject, 0)
COb_pcbconnection::COb_pcbconnection()
{
	m_nLayer = MULTILAYER;
	m_bLocked = FALSE;
	m_bPolygonOutline = FALSE;
	m_bUserRouted = TRUE;
	m_nUnionIndex = 0;

	m_bSelection = FALSE;
	m_nFlag = 0;
}



void COb_pcbconnection::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fX1;
		ar << m_fY1;
		ar << m_fX2;
		ar << m_fY2;
		ar << m_nLayer1;
		ar << m_nLayer2;
		ar << m_nNet;
		
		ar << m_nLayer;
		ar << m_bLocked;
		ar << m_bPolygonOutline;
		ar << m_bUserRouted;
		ar << m_nUnionIndex;
	}
	else
	{
		ar >> m_fX1;
		ar >> m_fY1;
		ar >> m_fX2;
		ar >> m_fY2;
		ar >> m_nLayer1;
		ar >> m_nLayer2;
		ar >> m_nNet;
		
		ar >> m_nLayer;
		ar >> m_bLocked;
		ar >> m_bPolygonOutline;
		ar >> m_bUserRouted;
		ar >> m_nUnionIndex;	
	}
}

void COb_pcbconnection::Copy(COb_pcbconnection* pconnection)
{
	m_fX1 = pconnection->m_fX1;
	m_fY1 = pconnection->m_fY1;
	m_fX2 = pconnection->m_fX2;
	m_fY2 = pconnection->m_fY2;
	m_nLayer1 = pconnection->m_nLayer1;
	m_nLayer2 = pconnection->m_nLayer2;
	m_nNet = pconnection->m_nNet;
}

void COb_pcbconnection::Draw(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(pDoc->m_sPCB_LayerParam[CONNECTIONS].bLayerShow == FALSE) return;

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	


	COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(m_nNet);
	if(pnet->m_bVisible == FALSE) return;

	CPen apen;
	apen.CreatePen(PS_SOLID, nEDA_PcbConnectionDrawWidth, pnet->m_nColor);
	CPen* oldpen = pDC->SelectObject(&apen);	

	int x, y;
	pView->Get_VPort_XY(&x, &y, m_fX1, m_fY1);
	pDC->MoveTo(x, y);
	pView->Get_VPort_XY(&x, &y, m_fX2, m_fY2);
	pDC->LineTo(x, y);

	pDC->SelectObject(oldpen);
	pDC->SetROP2(drawmode);
}