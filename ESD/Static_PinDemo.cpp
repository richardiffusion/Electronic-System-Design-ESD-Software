// DrawButton.cpp : implementation file
//

#include "stdafx.h"
#include "ESD.h"
#include "Static_PinDemo.h"
#include "systemext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatic_PinDemo

CStatic_PinDemo::CStatic_PinDemo()
{
}

CStatic_PinDemo::~CStatic_PinDemo()
{
}


BEGIN_MESSAGE_MAP(CStatic_PinDemo, CStatic)
	//{{AFX_MSG_MAP(CStatic_PinDemo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatic_PinDemo message handlers

void CStatic_PinDemo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	ASSERT(pDC != NULL);

	RECT rect;
	GetClientRect(&rect);

	CPen cpen;
	cpen.CreatePen(PS_SOLID, 1, RGB(221,221,221));
	CPen* oldpen = pDC->SelectObject(&cpen);

	CBrush fillbrush;
	fillbrush.CreateSolidBrush(RGB(255,255,255));
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	pDC->Rectangle(&rect);

	

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}
