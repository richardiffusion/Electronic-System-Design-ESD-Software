#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"


IMPLEMENT_SERIAL(COb_pcbclass, CObject, 0)
COb_pcbclass::COb_pcbclass()
{
	m_csName.Empty();
	m_nLayer = MULTILAYER;
	m_nKind = 0;
	m_bSuperClass = TRUE;
	m_arrayMember.RemoveAll();
	
	m_bLocked = FALSE;
	
	m_bPolygonOutline = FALSE;
	m_bUserRouted = TRUE;
	m_nUnionIndex = 0;

	m_bSelection = FALSE;
	m_nFlag = 0;
}



void COb_pcbclass::Serialize(CArchive& ar)
{
	/*if (ar.IsStoring())
	{
		ar << m_nCx;
		ar << m_nCy;
		ar << m_nRadius_x;
		ar << m_nRadius_y;
		ar << m_fSangle;
		ar << m_fEangle;
		ar << m_nWidth;
		ar << m_nColor;
		ar << m_nFillmode;
		ar << m_nFillcolor;
		ar << m_nDiagram;
		ar << m_nFlag;

	}
	else
	{
		ar >> m_nCx;
		ar >> m_nCy;
		ar >> m_nRadius_x;
		ar >> m_nRadius_y;
		ar >> m_fSangle;
		ar >> m_fEangle;
		ar >> m_nWidth;
		ar >> m_nColor;
		ar >> m_nFillmode;
		ar >> m_nFillcolor;
		ar >> m_nDiagram;
		ar >> m_nFlag;		
	}*/
}

