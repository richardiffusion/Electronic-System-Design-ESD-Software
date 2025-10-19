#include "stdafx.h"

#include "sysparam.h"
#include "systemext.h"


IMPLEMENT_SERIAL(COb_schsheetsymboltext, CObject, 0)
COb_schsheetsymboltext::COb_schsheetsymboltext()
{
	m_nFlag = 0;
	m_nFont = 0;
	m_nSelection = 0;
	
	m_bHidden = FALSE;
	m_bNotAutoPosition = FALSE;
	m_nAnChorH = 0;
	m_nAnChorV = 0;
}



void COb_schsheetsymboltext::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{


	}
	else
	{

}

void COb_schsheetsymboltext::Draw(CDC* pDC, CView* pV)
{
	COLORREF color;

	if(m_bHidden == TRUE) return;

	if(m_nFlag == FLAG_NORMAL) color = m_nColor;
	else if(m_nFlag == FLAG_DELETED) color = nEDA_BK_Color;
	else if(m_nFlag == FLAG_SELECTED) color = nEDA_Selected_Color;

	if(nEDA_Print_Flag != 0)
	{
		if(nEDA_Print_Color == 0) color = RGB(0,0,0);
	}

	COLORREF oldtextcolor;
	oldtextcolor = pDC->SetTextColor(color);
	::SetTextColor(pDC->m_hDC,color);

	int bkmode = pDC->SetBkMode(TRANSPARENT);

	ShowTrueTypeString(color, pDC, pV);

	pDC->SetTextColor(oldtextcolor);
	pDC->SetBkMode(bkmode);
}

