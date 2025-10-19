#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"


IMPLEMENT_SERIAL(COb_pcbnet, CObject, 0)
COb_pcbnet::COb_pcbnet()
{
	m_csNetName.Empty();

	m_bVisible = TRUE;
	m_bLoopRemove = TRUE;
	m_nColor = 7709086;

	for(int i=0; i<SIGNAL_LAYER_NUM; i++) m_fTrackWidth[i] = 0;
	m_fViaDia = 0;
	m_fViaHoleDia = 0;

	m_bSelection = FALSE;
	m_nFlag = FLAG_NORMAL;
}



void COb_pcbnet::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_csNetName;
		ar << m_bVisible;
		ar << m_bLoopRemove;
		ar << m_nColor;

		for(int i=0; i<SIGNAL_LAYER_NUM; i++)
		{
			ar << m_fTrackWidth[i];
		}
		ar << m_fViaDia;
		ar << m_fViaHoleDia;
		
		ar << m_bSelection;
		ar << m_nFlag;
	}
	else
	{
		ar >> m_csNetName;
		ar >> m_bVisible;
		ar >> m_bLoopRemove;
		ar >> m_nColor;

		for(int i=0; i<SIGNAL_LAYER_NUM; i++)
		{
			ar >> m_fTrackWidth[i];
		}
		ar >> m_fViaDia;
		ar >> m_fViaHoleDia;
		
		ar >> m_bSelection;
		ar >> m_nFlag;
	}
}

void COb_pcbnet::Copy(COb_pcbnet* pnet)
{
	pDocument = pnet->pDocument;

	m_csNetName = pnet->m_csNetName;
	m_bVisible = pnet->m_bVisible;
	m_bLoopRemove = pnet->m_bLoopRemove;
	m_nColor = pnet->m_nColor;
	
	for(int i=0; i<SIGNAL_LAYER_NUM; i++)  m_fTrackWidth[i] = pnet->m_fTrackWidth[i] ;
	m_fViaDia = pnet->m_fViaDia;
	m_fViaHoleDia = pnet->m_fViaHoleDia;
	
	m_bSelection = pnet->m_bSelection;
	m_nFlag = pnet->m_nFlag;
}
