// MarkupStatic.cpp : implementation file
//

#include "stdafx.h"
//#include "MarkupSample.h"
#include "MarkupStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarkupStatic

CMarkupStatic::CMarkupStatic()
{
	m_pUIElement = NULL;
}

CMarkupStatic::~CMarkupStatic()
{
	MARKUP_RELEASE(m_pUIElement);
}


BEGIN_MESSAGE_MAP(CMarkupStatic, CStatic)
	//{{AFX_MSG_MAP(CMarkupStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarkupStatic message handlers

void CMarkupStatic::SetMarkupText(LPCSTR lpszMarkup)
{
	MARKUP_RELEASE(m_pUIElement);

	m_pUIElement = Parse(lpszMarkup);

	if (m_hWnd) Invalidate(FALSE);
}

void CMarkupStatic::OnPaint() 
{
	CPaintDC dcPaint(this);
	CXTPBufferDC dcBuffer(dcPaint);

	CXTPClientRect rc(this);
	dcBuffer.FillSolidRect(rc, GetSysColor(COLOR_WINDOW));
	
	if (m_pUIElement)
	{
		CXTPMarkupDrawingContext dc(dcBuffer);
		
		m_pUIElement->Measure(&dc, rc.Size());

		m_pUIElement->Arrange(rc);

		m_pUIElement->Render(&dc);
	}
}

BOOL CMarkupStatic::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// To Handle Hyperlinks:
	if (m_pUIElement)
	{
		CXTPMarkupContext::m_hContextWnd = m_hWnd;
		if (CXTPMarkupContext::OnWndMsg(m_pUIElement, message, wParam, lParam, pResult))
			return TRUE;
	}

	return CStatic::OnWndMsg(message, wParam, lParam, pResult);
}
