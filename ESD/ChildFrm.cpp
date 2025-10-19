// ChildFrm.cpp : implementation of the CChildFrame class
//
// This file is a part of the XTREME TOOLKIT PRO MFC class library.
// (c)1998-2007 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESD.h"
#include "systemext.h"

#include "ChildFrm.h"
#include ".\childfrm.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)

	//}}AFX_MSG_MAP

	ON_WM_PAINT()
	ON_WM_MDIACTIVATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here

}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers


BOOL CChildFrame::OnCreateClient( LPCREATESTRUCT lpcs,
	CCreateContext* pContext)
{

	m_wndSheet.Attach(this);
	m_wndSheet.RefreshPage();
	if(nEDA_Sheet_State == 0) m_wndSheet.HideSheet();

	return CMDIChildWnd::OnCreateClient(lpcs, pContext);

}

void CChildFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	m_wndSheet.RefreshPage();
	m_wndSheet.Invalidate();
}

void CChildFrame::OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd)
{
	CESDView *pView = (CESDView*)GetActiveView();
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;


	if((pView != NULL) && (pView->m_nPrint_Flag == 0)) 
	{
		CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);

		if(bActivate == TRUE) 
		{  
			CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
			if((pView->m_nSystem_State) != nEDA_System_State)
			{
				if((pView->m_nSystem_State) == SCH) pFrame->OnToSch1(); 
				else pFrame->OnToPcb1();
			}
		}
	}
	else CMDIChildWnd::OnMDIActivate(bActivate, pActivateWnd, pDeactivateWnd);
}
