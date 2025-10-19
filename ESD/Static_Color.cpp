// DrawButton.cpp : implementation file
//

#include "stdafx.h"
#include "ESD.h"
#include "Static_Color.h"
#include "systemext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatic_Color

CStatic_Color::CStatic_Color()
{
}

CStatic_Color::~CStatic_Color()
{
}


BEGIN_MESSAGE_MAP(CStatic_Color, CStatic)
	//{{AFX_MSG_MAP(CStatic_Color)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatic_Color message handlers

void CStatic_Color::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	ASSERT(pDC != NULL);

	RECT rect;
	GetClientRect(&rect);

	CPen cpen;
	cpen.CreatePen(PS_SOLID, 1, RGB(221,221,221));
	CPen* oldpen = pDC->SelectObject(&cpen);

	CBrush fillbrush;
	fillbrush.CreateSolidBrush(m_nColor);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	pDC->Rectangle(&rect);

	//pDC->FillSolidRect(rect.left, rect.top, rect.right-rect.left, rect.bottom - rect.top, m_nColor);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}
