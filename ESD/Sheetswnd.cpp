/************************************
  REVISION LOG ENTRY
  Revision By: Mihai Filimon
  Revised on 7/19/98 11:29:37 AM
  Comments: SheetsWnd.cpp : implementation file
 ************************************/

#include "stdafx.h"
#include "SheetsWnd.h"
#include "systemext.h"
//#include "sysparam.h"
#include "childfrm.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include ".\sheetswnd.h"
//#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DEFAULTFORMATDRAWTEXT DT_CENTER | DT_VCENTER | DT_SINGLELINE

/////////////////////////////////////////////////////////////////////////////
// CSheetsWnd

#define LRB 14
#define FONTNAME _T("宋体")


// Function name	: CSheetsWnd::CSheetsWnd
// Description	    : Default constructor of this object
// Return type		: 
CSheetsWnd::CSheetsWnd()
{
	m_wndProcParent = NULL;
	m_DrawRect.SetRectEmpty();
	m_nHorzScroll = 0;
	m_nMaxHorzScroll = 0;

	m_nActive = 0;
	m_bShowState = TRUE;
	m_nColorBlock = 12;
}

// Function name	: CSheetsWnd::~CSheetsWnd
// Description	    : Destructor
// Return type		: 
CSheetsWnd::~CSheetsWnd()
{
}

BEGIN_MESSAGE_MAP(CSheetsWnd, CWnd)
	//{{AFX_MSG_MAP(CSheetsWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
	ON_WM_CHAR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheetsWnd message handlers

// Function name	: WindowProcParent
// Description	    : 
// Return type		: LRESULT CALLBACK 
// Argument         :  HWND hwnd
// Argument         : UINT uMsg
// Argument         : WPARAM wParam
// Argument         : LPARAM lParam
LRESULT CALLBACK WindowProcParent( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch (uMsg)
	{
	case WM_DESTROY:
			{
				CSheetsWnd::RestoreWndProc(hwnd);
				break;
			}
		case WM_WINDOWPOSCHANGED:
		case WM_SIZE:
			{
				//cann't be shielded because the MDI strategy cann't work either without the two messages
				CallWindowProc(CSheetsWnd::GetWndSheet(hwnd)->GetWindowProc(), hwnd, uMsg, wParam, lParam );
				CSheetsWnd::ResizeStatical(hwnd);
				return NULL;
			}
		case WM_PARENTNOTIFY:
			{
				if (LOWORD(wParam) == WM_CREATE)
					CSheetsWnd::GetWndSheet(hwnd)->Invalidate();
				break;
			}
	}
	return CallWindowProc(CSheetsWnd::GetWndSheet(hwnd)->GetWindowProc(), hwnd, uMsg, wParam, lParam );
}

// Function name	: CSheetsWnd::GetRect
// Description	    : Return the wished rect of this object
// Return type		: CRect 
// Argument         : CWnd* pWndParent
CRect CSheetsWnd::GetRect(CWnd* pWndParent)
{
	CRect rect; pWndParent->GetClientRect(rect);

	//在window me 下，图层控制条太窄！ 固定为18
	//rect.top = rect.bottom - GetSystemMetrics(SM_CYVTHUMB);
	
	//lichi added
	rect.top = rect.bottom - nEDA_Sheet_Tab_Width;

	return rect;
}

// Function name	: CSheetsWnd::GetRect
// Description	    : Return the wished rect of this object
// Return type		: CRect 
// Argument         : HWND hWndParent
CRect CSheetsWnd::GetRect(HWND hWndParent)
{
	return GetRect(CWnd::FromHandle(hWndParent));
}

// Function name	: CSheetsWnd::Attach
// Description	    : Create object as child of pWndParent with rgbBackground
// Return type		: BOOL 
// Argument         : CWnd * pWndParent
// Argument         : COLORREF rgbBackground
BOOL CSheetsWnd::Attach(CWnd * pWndParent, COLORREF rgbBackground)
{
	ASSERT (pWndParent && IsWindow(pWndParent->m_hWnd));
	CRect rect = GetRect(pWndParent);
	m_brBackground.CreateSolidBrush(rgbBackground);
	ASSERT(pWndParent->GetDlgItem(IDTHIS) == NULL);
	pWndParent->ModifyStyle(0, WS_CLIPCHILDREN);
	BOOL bResult = Create(AfxRegisterWndClass(0,AfxGetApp()->LoadStandardCursor(IDC_ARROW),(HBRUSH)m_brBackground,0), _T(""), WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, rect, pWndParent, IDTHIS);
	if (bResult)
		// Subclass window procedure of parent object for receive some message. See WindowProcParent for detail
		m_wndProcParent = (WNDPROC)SetWindowLong(pWndParent->m_hWnd, GWL_WNDPROC, (long)WindowProcParent);
	return bResult;
}

// Function name	: CSheetsWnd::GetWndSheet
// Description	    : Return pointer to sheet window
// Return type		: CSheetsWnd* 
// Argument         : HWND hWndParent
CSheetsWnd* CSheetsWnd::GetWndSheet(HWND hWndParent)
{
	return (CSheetsWnd*)CWnd::FromHandle(::GetDlgItem(hWndParent, IDTHIS));
}

// Function name	: CSheetsWnd::RestoreWndProc
// Description	    : When parent of this object become to be destroyed then restore window procedure
// Return type		: void 
// Argument         : HWND hWndParent
void CSheetsWnd::RestoreWndProc(HWND hWndParent)
{
	if (IsWindow(hWndParent))
	{
		long curent = (long)((CSheetsWnd*)CWnd::FromHandle(::GetDlgItem(hWndParent, IDTHIS)))->m_wndProcParent;
		if (GetWindowLong(hWndParent, GWL_WNDPROC) != curent)
			SetWindowLong(hWndParent, GWL_WNDPROC, curent);
	}
}

// Function name	: CSheetsWnd::Resize
// Description	    : 
// Return type		: void 
// Argument         : HWND hWndParent
void CSheetsWnd::Resize(HWND hWndParent)
{
	if(m_bShowState == TRUE)
	{
		CRect rect = GetRect(hWndParent);
		MoveWindow(rect);
		rect.bottom = rect.top; rect.top = 0;
		CWnd* pChild = GetFirstView();
		while (pChild)
		{
			pChild->MoveWindow(rect);
			pChild = GetNextView();
		}
		GetClientRect(rect);
		m_DrawRect.right = rect.right;		
	}
	else
	{
		//CRect rect = GetRect(hWndParent);
		//MoveWindow(rect);
		CRect rect = GetRect(hWndParent);
		rect.top = 0;
		CWnd* pChild = GetFirstView();
		while (pChild)
		{
			pChild->MoveWindow(rect);
			pChild = GetNextView();
		}
		m_DrawRect.SetRectEmpty();
		//GetClientRect(rect);
		//m_DrawRect.right = rect.right;	
	}
}

// Function name	: CSheetsWnd::ResizeStatical
// Description	    : All childs of parent, without this must be resized
// Return type		: void 
// Argument         : HWND hWndParent
void CSheetsWnd::ResizeStatical(HWND hWndParent)
{
	//添加此条件，可以避免打印预览时切换窗口引起的显示问题
	if(nEDA_Print_Flag == 0)
	{
		if (CSheetsWnd* pThis = GetWndSheet(hWndParent))
			pThis->Resize(hWndParent);
	}
}

// Function name	: CSheetsWnd::GetFirstView
// Description	    : Get first view from object parent
// Return type		: CWnd* 
CWnd* CSheetsWnd::GetFirstView()
{
	m_pViewFind = GetParent()->GetWindow(GW_CHILD);
	while (m_pViewFind && m_pViewFind->m_hWnd == m_hWnd)
		m_pViewFind = m_pViewFind->GetWindow(GW_HWNDNEXT);
	return m_pViewFind;
}

// Function name	: CSheetsWnd::GetNextView
// Description	    : Get next view from object parent
// Return type		: CWnd* 
CWnd* CSheetsWnd::GetNextView()
{
	if (m_pViewFind)
	{
		m_pViewFind = m_pViewFind->GetWindow(GW_HWNDNEXT);
		while (m_pViewFind && m_pViewFind->m_hWnd == m_hWnd)
			m_pViewFind = m_pViewFind->GetWindow(GW_HWNDNEXT);
		return m_pViewFind;
	}
	return NULL;
}


// Function name	: CSheetsWnd::OnCreate
// Description	    : Create a new things.
// Return type		: int 
// Argument         : LPCREATESTRUCT lpCreateStruct
int CSheetsWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rect; GetClientRect(rect);
	m_font.CreateFont(-rect.Height() * 8 / 10, 0, 0, 0, FW_LIGHT, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FONTNAME);
	CSize sxy(2 * rect.Height(), rect.Height());
	m_btns.Create(WS_CHILD | WS_VISIBLE | SBS_HORZ, CRect(CPoint(rect.left, rect.top), sxy), this, IDBTNS);
	rect.left += sxy.cx + 2;
	m_DrawRect = rect;
	return 0;
}

// Function name	: CSheetsWnd::OnEraseBkgnd
// Description	    : 
// Return type		: BOOL 
// Argument         : CDC* pDC
BOOL CSheetsWnd::OnEraseBkgnd(CDC* pDC) 
{
	Draw(pDC);
	pDC->ExcludeClipRect(EraseExclude());
	CBrush* pOldBrush = pDC->SelectObject(&m_brBackground);
	CRect rect;
	pDC->GetClipBox(&rect);     // Erase the area needed
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
}

// Function name	: CSheetsWnd::Draw
// Description	    : Draw the pages of object
// Return type		: void 
// Argument         : CDC * pDC
void CSheetsWnd::Draw(CDC * pDC)
{

	CESDView* pView = (CESDView*)GetFirstView();
	if(pView == NULL) return;

	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	if(pDoc == NULL) return;

	if(pView->m_nSystem_State == SCH)
	{
		if (!m_DrawRect.IsRectNull())
		{
			CDC dc; CBitmap bitmap;
			if (dc.CreateCompatibleDC(pDC))
				if (bitmap.CreateCompatibleBitmap(pDC, m_DrawRect.Width(), m_DrawRect.Height()))
				{
					CBitmap* pOldBitmap = dc.SelectObject(&bitmap);

					CBrush* pOldBrush = dc.SelectObject(&m_brBackground);
					dc.PatBlt(0,0, m_DrawRect.Width(), m_DrawRect.Height(), PATCOPY);
					CFont* pOldFont = dc.SelectObject(&m_font);
					dc.SetBkMode(TRANSPARENT);
					
					int n_activepage = GetActivePage();
					int n_page = GetFirstPage();
					
					CRgn rgnActive;
					static const int mP = 5;
					CRect rtActive; CPoint pActivePage[mP];
					int cxB = LRB, cx = m_nHorzScroll+6, next = NULL;
					m_nMaxHorzScroll = 0;
					COLORREF oldtextcolor = dc.GetTextColor();

					while (n_page >= 0)
					{
						CString title = GetPageTitle(n_page);
						CRect rt(cx,0,0,0);
						dc.DrawText(title, rt, DEFAULTFORMATDRAWTEXT | DT_CALCRECT);
						rt.top = 1; rt.bottom = m_DrawRect.Height();
						rt.left -= 6; rt.right += 4;
						CPoint pPage[mP];
						pPage[0] = CPoint(rt.left , 0+1);
						pPage[1] = CPoint(rt.left, rt.bottom - 3);
						pPage[2] = CPoint(rt.left + 3, rt.bottom );
						pPage[3] = CPoint(rt.right, rt.bottom);
						pPage[4] = CPoint(rt.right + cxB, 0+1);
						CRgn rgn; rgn.CreatePolygonRgn(pPage, 5, WINDING);
						dc.FillRgn(&rgn, &CBrush(RGB(215,215,215)));               
						dc.FrameRgn(&rgn, &CBrush(RGB(172,168,153)), 1, 1);			
						CPoint pPage1[mP];
						pPage1[0] = CPoint(rt.left+1 , 0+1);
						pPage1[1] = CPoint(rt.left+1, rt.bottom - 3);
						pPage1[2] = CPoint(rt.left + 2, rt.bottom - 1 );
						pPage1[3] = CPoint(rt.right-1, rt.bottom-1);
						pPage1[4] = CPoint(rt.right + cxB-1, 0+1);
						CRgn rgn1; rgn1.CreatePolygonRgn(pPage1, 5, WINDING);
						dc.FillRgn(&rgn1, &CBrush(RGB(215,215,215)));               
						dc.FrameRgn(&rgn1, &CBrush(RGB(205,205,205)), 1, 1);		
					
						dc.SetTextColor(RGB(100,100,100));							
						dc.DrawText(title, rt, DEFAULTFORMATDRAWTEXT);				
						
						if ((n_activepage >= 0) && (n_page == n_activepage))
						{
							rgnActive.CreatePolygonRgn(pPage, 5, ALTERNATE);
							rtActive = rt;
							for (int i = 0; i < mP ; i++)
								pActivePage[i] = pPage[i];
						}
						
						n_page = GetNextPage();
						
						CPen pen(PS_SOLID,1,RGB(127,157,185)); 
						CPen *oldpen = dc.SelectObject(&pen);
						dc.MoveTo(pPage[0].x, 1); 
						dc.LineTo(pPage[4].x, 1);					if(n_page < 0) 
						{
							dc.MoveTo(pPage[4].x, 1); 
							dc.LineTo(3000, 1);
						}
						dc.SelectObject(oldpen);
						
						next = rt.Width()+ LRB-8;
						cx += next;
						m_nMaxHorzScroll += next;
					}
					if (n_activepage >= 0)
					{
						dc.FillRgn(&rgnActive, &CBrush(RGB(255,255,255)));
						dc.FrameRgn(&rgnActive, &CBrush(RGB(127,157,185)), 1, 1);
						dc.SetTextColor(RGB(0,0,0));
						dc.DrawText(GetPageTitle(n_activepage), rtActive, DEFAULTFORMATDRAWTEXT);
						int pROP2 = dc.SetROP2(R2_WHITE);
						dc.MoveTo(pActivePage[0].x, pActivePage[0].y); 
						dc.LineTo(pActivePage[4].x-1,pActivePage[4].y);
					}
					pDC->BitBlt(m_DrawRect.left, m_DrawRect.top, m_DrawRect.Width(), m_DrawRect.Height(), &dc, 0,0, SRCCOPY);
					dc.SelectObject(pOldFont);
					dc.SelectObject(pOldBrush);
					dc.SelectObject(pOldBitmap);
					dc.SetTextColor(oldtextcolor);
				}
		}
	}
	else
	{
		if (!m_DrawRect.IsRectNull())
		{
			CDC dc; CBitmap bitmap;
			if (dc.CreateCompatibleDC(pDC))
				if (bitmap.CreateCompatibleBitmap(pDC, m_DrawRect.Width(), m_DrawRect.Height()))
				{
					CBitmap* pOldBitmap = dc.SelectObject(&bitmap);

					CBrush* pOldBrush = dc.SelectObject(&m_brBackground);
					dc.PatBlt(0,0, m_DrawRect.Width(), m_DrawRect.Height(), PATCOPY);
					CFont* pOldFont = dc.SelectObject(&m_font);
					dc.SetBkMode(TRANSPARENT);
					
					int n_activepage = GetActivePage();
					int n_page = GetFirstPage();
					
					CRgn rgnActive;
					static const int mP = 5;
					CRect rtActive; CPoint pActivePage[mP];
					int cxB = LRB, cx = m_nHorzScroll+6, next = NULL;
					m_nMaxHorzScroll = 0;
					COLORREF oldtextcolor = dc.GetTextColor();

					while (n_page >= 0)
					{
						CString title = GetPageTitle(n_page);
						CRect rt(cx,0,0,0);
						dc.DrawText(title, rt, DEFAULTFORMATDRAWTEXT | DT_CALCRECT);
						rt.top = 1; rt.bottom = m_DrawRect.Height();
						rt.left -= 6; rt.right += 4;
						CPoint pPage[mP];
						pPage[0] = CPoint(rt.left , 0+1);
						pPage[1] = CPoint(rt.left, rt.bottom - 3);
						pPage[2] = CPoint(rt.left + 3, rt.bottom );
						pPage[3] = CPoint(rt.right+m_nColorBlock, rt.bottom);
						pPage[4] = CPoint(rt.right+m_nColorBlock+cxB, 0+1);
						CRgn rgn; rgn.CreatePolygonRgn(pPage, 5, WINDING);
						dc.FillRgn(&rgn, &CBrush(RGB(215,215,215)));                
						dc.FrameRgn(&rgn, &CBrush(RGB(172,168,153)), 1, 1);			
						CPoint pPage1[mP];
						pPage1[0] = CPoint(rt.left+1 , 0+1);
						pPage1[1] = CPoint(rt.left+1, rt.bottom - 3);
						pPage1[2] = CPoint(rt.left + 2, rt.bottom - 1 );
						pPage1[3] = CPoint(rt.right+m_nColorBlock-1, rt.bottom-1);
						pPage1[4] = CPoint(rt.right+m_nColorBlock+cxB-1, 0+1);
						CRgn rgn1; rgn1.CreatePolygonRgn(pPage1, 5, WINDING);
						dc.FillRgn(&rgn1, &CBrush(RGB(215,215,215)));                
						dc.FrameRgn(&rgn1, &CBrush(RGB(205,205,205)), 1, 1);			
					
						dc.SetTextColor(RGB(100,100,100));							
						dc.DrawText(title, rt, DEFAULTFORMATDRAWTEXT);				
						
						CPoint pPage2[4];
						pPage2[0] = CPoint(rt.right-2 ,2);
						pPage2[1] = CPoint(rt.right-2, rt.bottom - 3);
						pPage2[2] = CPoint(rt.right+10, rt.bottom - 3);
						pPage2[3] = CPoint(rt.right+10, 2);
						CRgn rgn2; rgn2.CreatePolygonRgn(pPage2, 4, WINDING);
						int i, j;
						j = 0;
						COLORREF layercolor = RGB(0,0,0);;
						for(i=0; i<=MULTILAYER; i++)
						{
							if(pDoc->m_sPCB_LayerParam[i].bLayerShow == TRUE)
							{
								CString str = pDoc->m_sPCB_LayerParam[i].csLayerName;
								if(str == title)  
								{
									layercolor = nEDA_PCB_LayerColor[i];
									break;
								}
							}
						}
						dc.FillRgn(&rgn2, &CBrush(layercolor));

						if ((n_activepage >= 0) && (n_page == n_activepage))
						{
							rgnActive.CreatePolygonRgn(pPage, 5, ALTERNATE);
							rtActive = rt;
							for (int i = 0; i < mP ; i++)
								pActivePage[i] = pPage[i];
						}
						
						n_page = GetNextPage();
						
						CPen pen(PS_SOLID,1,RGB(127,157,185)); 
						CPen *oldpen = dc.SelectObject(&pen);
						dc.MoveTo(pPage[0].x, 1); 
						dc.LineTo(pPage[4].x, 1);					
						if(n_page < 0) 
						{
							
							dc.MoveTo(pPage[4].x, 1); 
							dc.LineTo(3000, 1);
						}
						dc.SelectObject(oldpen);
						
						next = rt.Width()+ LRB - 8 + m_nColorBlock;
						cx += next;
						m_nMaxHorzScroll += next;
					}
					if (n_activepage >= 0)
					{
						dc.FillRgn(&rgnActive, &CBrush(RGB(255,255,255)));
						dc.FrameRgn(&rgnActive, &CBrush(RGB(127,157,185)), 1, 1);
						dc.SetTextColor(RGB(0,0,0));
						dc.DrawText(GetPageTitle(n_activepage), rtActive, DEFAULTFORMATDRAWTEXT);

						CPoint pPage[4];
						pPage[0] = CPoint(rtActive.right-2 ,2);
						pPage[1] = CPoint(rtActive.right-2, rtActive.bottom - 3);
						pPage[2] = CPoint(rtActive.right+10, rtActive.bottom - 3);
						pPage[3] = CPoint(rtActive.right+10, 2);
						CRgn rgn; rgn.CreatePolygonRgn(pPage, 4, WINDING);
						int i, j;
						j = 0;
						CString title = GetPageTitle(n_activepage);
						COLORREF layercolor = RGB(0,0,0);;
						for(i=0; i<=MULTILAYER; i++)
						{
							if(pDoc->m_sPCB_LayerParam[i].bLayerShow == TRUE)
							{
								CString str = pDoc->m_sPCB_LayerParam[i].csLayerName;
								if(str == title)  
								{
									layercolor = nEDA_PCB_LayerColor[i];
									break;
								}
							}
						}
						dc.FillRgn(&rgn, &CBrush(layercolor));

						int pROP2 = dc.SetROP2(R2_WHITE);
						dc.MoveTo(pActivePage[0].x, pActivePage[0].y); 
						dc.LineTo(pActivePage[4].x-1,pActivePage[4].y);
					}
					pDC->BitBlt(m_DrawRect.left, m_DrawRect.top, m_DrawRect.Width(), m_DrawRect.Height(), &dc, 0,0, SRCCOPY);
					dc.SelectObject(pOldFont);
					dc.SelectObject(pOldBrush);
					dc.SelectObject(pOldBitmap);
					dc.SetTextColor(oldtextcolor);
				}
		}
	}
}

// Function name	: CSheetsWnd::OnHScroll
// Description	    : Left right buttons
// Return type		: void 
// Argument         : UINT nSBCode
// Argument         : UINT nPos
// Argument         : CScrollBar* pScrollBar
void CSheetsWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar->m_hWnd == m_btns.m_hWnd)
	{
		int nCX = 0;
		switch (nSBCode)
		{
			case SB_LINELEFT:
			case SB_LINERIGHT:
				{
					nCX = (nSBCode == SB_LINELEFT ? 1 : - 1) * 16;
					break;
				}
		}
		int n = m_nHorzScroll + nCX;
		if ( n <= 0)
			if (abs(n) < m_nMaxHorzScroll)
		{
			m_nHorzScroll = n;	
			Invalidate();
		}
	}
	
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}


// Function name	: CSheetsWnd::OnLButtonDown
// Description	    : 
// Return type		: void 
// Argument         : UINT nFlags
// Argument         : CPoint point
void CSheetsWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//SetFocus();
	if (m_DrawRect.PtInRect(point))
		SetActivePage(GetViewFromPoint(point));

	CWnd::OnLButtonDown(nFlags, point);
}

// Function name	: CSheetsWnd::OnLButtonUp
// Description	    : 
// Return type		: void 
// Argument         : UINT nFlags
// Argument         : CPoint point
void CSheetsWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{

	CWnd::OnLButtonUp(nFlags, point);
}

// Function name	: CSheetsWnd::OnSetCursor
// Description	    : Set the cursor.
// Return type		: BOOL 
// Argument         : CWnd* pWnd
// Argument         : UINT nHitTest
// Argument         : UINT message
BOOL CSheetsWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint pCursor;
	if (GetCursorPos(&pCursor))
	{
		ScreenToClient(&pCursor);
	}
	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

// Function name	: CSheetsWnd::GetViewFromPoint
// Description	    : Return pointer to view s.t. page of view contain point
// Return type		: CWnd* 
// Argument         : CPoint point
int CSheetsWnd::GetViewFromPoint(CPoint point)
{

	CESDView* pView = (CESDView*)GetFirstView();
	if(pView == NULL) return -1;

	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	if(pDoc == NULL) return -1;

	if(pView->m_nSystem_State == SCH)
	{
		if (m_DrawRect.PtInRect(point))
		{
			int n_page = GetFirstPage();
			
			CDC* pDC = GetDC();
			pDC->SetViewportOrg(m_DrawRect.TopLeft());
			CFont* pOldFont = pDC->SelectObject(&m_font);
			int cxB = LRB, cx = cxB;
			int aPointX = -m_nHorzScroll + point.x - m_DrawRect.left;
			while (n_page >= 0)
			{
				CRect rt(cx,0,0,0);
				pDC->DrawText(GetPageTitle(n_page), rt, DEFAULTFORMATDRAWTEXT | DT_CALCRECT);
				rt.top = 0; rt.bottom = m_DrawRect.Height();
				rt.left -= 6; rt.right += 4;
				if ((aPointX > cx - LRB) && (aPointX  <= cx + rt.Width()-10))
				//if ((aPointX > cx ) && (aPointX  <= cx + rt.Width() -10))
					break;
				n_page = GetNextPage();
				int next = rt.Width() + LRB - 8 ;
				cx += next;
			}
			pDC->SelectObject(pOldFont);
			ReleaseDC(pDC);
			return n_page;
		}
	}
	else
	{
		if (m_DrawRect.PtInRect(point))
		{
			int n_page = GetFirstPage();
			
			CDC* pDC = GetDC();
			pDC->SetViewportOrg(m_DrawRect.TopLeft());
			CFont* pOldFont = pDC->SelectObject(&m_font);
			int cxB = LRB, cx = cxB;
			int aPointX = -m_nHorzScroll + point.x - m_DrawRect.left;
			while (n_page >= 0)
			{
				CRect rt(cx,0,0,0);
				pDC->DrawText(GetPageTitle(n_page), rt, DEFAULTFORMATDRAWTEXT | DT_CALCRECT);
				rt.top = 0; rt.bottom = m_DrawRect.Height();
				rt.left -= 6; rt.right += 4 + m_nColorBlock;
				if ((aPointX > cx - LRB) && (aPointX  <= cx + rt.Width()-10))
				//if ((aPointX > cx ) && (aPointX  <= cx + rt.Width() -10))
					break;
				n_page = GetNextPage();
				int next = rt.Width() + LRB - 8 ;
				cx += next;
			}
			pDC->SelectObject(pOldFont);
			ReleaseDC(pDC);
			return n_page;
		}
	}
	return -1;
}

// Function name	: CSheetsWnd::GetDrawRect
// Description	    : Return the painting rect
// Return type		: CRect 
CRect CSheetsWnd::GetDrawRect() const
{
	return m_DrawRect;
}

// Function name	: CSheetsWnd::EraseExclude
// Description	    : When WM_ERASEBKGND occurs, then CSheetsWnd::OnEraseBkGnd,
// require a rect that will be excluded from area erased. That's it.
// Return type		: CRect 
CRect CSheetsWnd::EraseExclude()
{
	return m_DrawRect;
}

// Function name	: CSheetsWnd::GetWindowProc
// Description	    : Return the window procedure of Parent object
// Return type		: WNDPROC 
WNDPROC CSheetsWnd::GetWindowProc()
{
	return m_wndProcParent;
}

// Function name	: CSheetsWnd::IsEqual
// Description	    : Compare if two string is equal, call from JumpToPage
// Return type		: BOOL 
// Argument         : CString s
// Argument         : CString d
BOOL CSheetsWnd::IsEqual(CString s, CString d)
{
	s = s.Left(1); s.MakeUpper();
	d = d.Left(1); d.MakeUpper();
	return s == d;
}

// Function name	: CSheetAlpha::GetPageIndex
// Description	    : Search for a pags with title lpszTitle, and return its position
// Return type		: int 
// Argument         : LPCTSTR lpszTitle
int CSheetsWnd::GetPageIndex(LPCTSTR lpszTitle)
{
	int n = -1;
	for (int i = 0; i < m_strTitles.GetSize(); i++)
		if (m_strTitles[i].CompareNoCase(lpszTitle) == 0)
		{
			n = i;
			break;
		}
	return n;
}

// Function name	: CSheetsWnd::AddPage
// Description	    : Add a new page to object, and return index position
// Return type		: int 
// Argument         : LPCTSTR lpszTitle
int CSheetsWnd::AddPage(LPCTSTR lpszTitle)
{
	int nResult = m_strTitles.Add(lpszTitle);
	Invalidate();
	return nResult;
}

// Function name	: CSheetsWnd::RemovePage
// Description	    : Remove page with title lpszTitle if founded
// Return type		: void 
// Argument         : LPCTSTR lpszTitle
void CSheetsWnd::RemovePage(LPCTSTR lpszTitle)
{
	int nPage = GetPageIndex(lpszTitle);
	if (nPage >= 0)
	{
		m_strTitles.RemoveAt(nPage);
		Invalidate();
	}
}

void CSheetsWnd::RefreshPage()
{
	m_strTitles.RemoveAll();

	CESDView* pView = (CESDView*)GetFirstView();
	if(pView == NULL) return;

	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	if(pDoc == NULL) return;

	if(pView->m_nSystem_State == SCH)
	{
		int i, j;
		j = 0;
		for(i=0; i<SCH_SHEET_NUM; i++)
		{
			if(pDoc->m_bSCH_SheetShow[i] == TRUE)
			{
				m_strTitles.Add(pDoc->m_csSCH_SheetName[i]);
				
				if(pView->m_nDiagram == i)	m_nActive = j;
				
				j++;
			}
		}
	}
	else
	{
		for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
		{
			int layer = pDoc->m_arrayPcbLayerShow.GetAt(i);
			m_strTitles.Add(pDoc->m_sPCB_LayerParam[layer].csLayerName);
			if(pView->m_nPCB_Current_Layer == layer)	m_nActive = i;
		}

	}
}

int CSheetsWnd::GetFirstPage()
{
	m_nFind = -1;
	return GetNextPage();
}

int CSheetsWnd::GetNextPage()
{
	if (++m_nFind < m_strTitles.GetSize())
		return m_nFind;
	return -1;
}

int CSheetsWnd::GetActivePage()
{
	if ((m_nActive>=0) && (m_nActive < m_strTitles.GetSize()))
	{
		m_nFind = m_nActive;
		return m_nFind;
	}
	return -1;
}

void CSheetsWnd::SetActivePage(int n_page)
{
	if (n_page >= 0)
	{
		if(n_page != m_nActive)
		{
			m_nActive = n_page;
			Invalidate();
		}
		
		CESDView* pView = (CESDView*)GetFirstView();
		if(pView == NULL) return;

		CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
		if(pDoc == NULL) return;

		if(pView->m_nSystem_State == SCH)
		{
			int i, j;
			j = 0;
			for(i=0; i<SCH_SHEET_NUM; i++)
			{
				if(pDoc->m_bSCH_SheetShow[i] == TRUE)
				{
					if( strcmp(pDoc->m_csSCH_SheetName[i], m_strTitles[j]) == 0)
					{
						if(m_nActive == j) 
						{
							//pView->SelectAll(FALSE);

							pDoc->m_fSCH_ScreenX[pView->m_nDiagram] = pView->m_fScreenX;
							pDoc->m_fSCH_ScreenY[pView->m_nDiagram] = pView->m_fScreenY;
							pDoc->m_fSCH_Scale[pView->m_nDiagram] = pView->m_fScale;
							pView->m_nDiagram = i;
							pView->m_fScreenX = pDoc->m_fSCH_ScreenX[pView->m_nDiagram];
							pView->m_fScreenY = pDoc->m_fSCH_ScreenY[pView->m_nDiagram];
							pView->m_fScale = pDoc->m_fSCH_Scale[pView->m_nDiagram];
						
							pView->Invalidate();
							break;
						}
						j++;
					}
				}
			}
		}
		else
		{
			int i, j;
			j = m_nActive;
			for(i=0; i<=MULTILAYER; i++)
			{
				if(pDoc->m_sPCB_LayerParam[i].bLayerShow == TRUE)
				{
					if( pDoc->m_sPCB_LayerParam[i].csLayerName == m_strTitles[j] )
					{
						pView->m_nPCB_Current_Layer = i;
						pView->Invalidate();
						break;
					}
				}
			}
		}
	}
}

CString CSheetsWnd::GetPageTitle(int nPage)
{
	return m_strTitles[nPage];
}

void CSheetsWnd::ShowSheet()
{
	ShowWindow(TRUE);
	m_bShowState = TRUE;

	CRect rect; 
	GetClientRect(rect);
	rect.left += 2 * rect.Height() + 2;
	m_DrawRect = rect;

	CWnd* pWnd = GetParent();
	Resize(pWnd->m_hWnd);
	Invalidate();
}

void CSheetsWnd::HideSheet()
{
	ShowWindow(FALSE);
	m_bShowState = FALSE;

	CWnd* pWnd = GetParent();
	Resize(pWnd->m_hWnd);
}

void CSheetsWnd::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CESDView* pView = (CESDView*)GetFirstView();
	if(pView == NULL) return;

	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	if(pDoc == NULL) return;

	if(nChar == 43)  
	{
		if(pView->m_nSystem_State == SCH) pView->Plus_Sheet();
		else pView->Plus_Layer();
	}
	else if(nChar == 45) 
	{
		if(pView->m_nSystem_State == SCH) pView->Minus_Sheet();
		else pView->Minus_Layer();
	}

	CWnd::OnChar(nChar, nRepCnt, nFlags);
}

