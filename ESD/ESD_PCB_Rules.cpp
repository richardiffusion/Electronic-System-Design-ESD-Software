#include "stdafx.h"

#include "sysparam.h"
#include "systemext.h"


IMPLEMENT_SERIAL(COb_pcbrules, CObject, 0)
COb_pcbrules::COb_pcbrules()
{

	
	m_bLocked = FALSE;
	
	m_bPolygonOutline = FALSE;
	m_bUserRouted = TRUE;
	m_nUnionIndex = 0;

	m_bSelection = FALSE;
	m_nFlag = 0;
}



void COb_pcbrules::Serialize(CArchive& ar)
{

}

