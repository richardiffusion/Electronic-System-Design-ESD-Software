// GfxInEdit.cpp : implementation file
//

#include "stdafx.h"
#include "VertexListCtrl.h"
#include "InEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInEdit


CInEdit::CInEdit(int iItem, int iSubItem, CString sInitText, bool _bMouseSelect) : m_sInitText(sInitText)
{
	bMouseSelect = _bMouseSelect;
	m_iItem = iItem;
	m_iSubItem = iSubItem;
	m_bESC = FALSE;
}

CInEdit::~CInEdit()
{
}

//CEdit)
BEGIN_MESSAGE_MAP(CInEdit, CRichEditCtrl)
	//{{AFX_MSG_MAP(CInEdit)
	ON_WM_CHAR()
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInEdit message handlers

void CInEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// lichi changed
	if (nChar == VK_ESCAPE || nChar == VK_RETURN || nChar == VK_TAB)
	{
		if (nChar == VK_ESCAPE) 
		{
			m_bESC = TRUE;
		}
		if (nChar == VK_TAB)
		{
			SHORT sh = GetKeyState(VK_SHIFT);

			m_bESC = TRUE;
			ProcessSelect();
			CWnd * pParent = GetParent();
			int ip[2] = { m_iItem, m_iSubItem };
			DestroyWindow();
			if (sh < 0) pParent->SendMessage(WM_USER_TAB, 1, (LPARAM) &ip);
			else pParent->SendMessage(WM_USER_TAB, 0, (LPARAM) &ip);
			return;
		}
		GetParent()->SetFocus();
		return;
	}


	CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);	
	

}

int CInEdit::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CRichEditCtrl::OnCreate(lpCreateStruct) == -1) return -1;
	
	CFont font;
	font.CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH| FF_SWISS, _T("Times New Roman") );
	SetFont(&font);

	SetWindowText(m_sInitText);
	SetFocus();

	SetSel(0, -1);

	if (bMouseSelect)
	{
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		PostMessage(WM_LBUTTONDOWN, 0, MAKELONG(pt.x, pt.y));
		PostMessage(WM_LBUTTONUP, 0, MAKELONG(pt.x, pt.y));
	}

	return 0;
}

void CInEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CRichEditCtrl::OnKillFocus(pNewWnd);

	if (m_bESC)
	{
		DestroyWindow();
		return;
	}

	ProcessSelect();
	DestroyWindow();
}

void CInEdit::OnNcDestroy() 
{
	CRichEditCtrl::OnNcDestroy();
//	delete this;
}

BOOL CInEdit::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_LEFT)
		{
			long ls,le;
			GetSel(ls, le);
			if (ls == 0 && ls == le)
			{
				m_bESC = TRUE;
				ProcessSelect();
				CWnd * pParent = GetParent();
				int ip[2] = { m_iItem, m_iSubItem };
				DestroyWindow();
				pParent->SendMessage(WM_USER_TAB, 2, (LPARAM) &ip);
				return TRUE;
			}
		}
		if (pMsg->wParam == VK_RIGHT)
		{
			long ls,le;
			GetSel(ls, le);
			CString cs;
			GetWindowText(cs);

			if (ls == cs.GetLength() && ls == le)
			{
				m_bESC = TRUE;
				ProcessSelect();
				CWnd * pParent = GetParent();
				int ip[2] = { m_iItem, m_iSubItem };
				DestroyWindow();
				pParent->SendMessage(WM_USER_TAB, 3, (LPARAM) &ip);
				return TRUE;
			}
		}

		if (pMsg->wParam == VK_UP)
		{
			m_bESC = TRUE;
			ProcessSelect();
			CWnd * pParent = GetParent();
			int ip[2] = { m_iItem, m_iSubItem };
			DestroyWindow();
			pParent->SendMessage(WM_USER_TAB, 4, (LPARAM) &ip);
			return TRUE;
		}
		if (pMsg->wParam == VK_DOWN)
		{
			m_bESC = TRUE;
			ProcessSelect();
			CWnd * pParent = GetParent();
			int ip[2] = { m_iItem, m_iSubItem };
			DestroyWindow();
			pParent->SendMessage(WM_USER_TAB, 5, (LPARAM) &ip);
			return TRUE;
		}

		// lichi added below  不能象窗口发送VK_ESCAPE消息，原因不明
		if (pMsg->wParam == VK_ESCAPE)
		{
			OnChar(VK_ESCAPE, 1, 1);
			return TRUE;
		}
		if (pMsg->wParam == VK_TAB)
		{
			OnChar(VK_TAB, 1, 1);
			return TRUE;
		}

		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_DELETE || //pMsg->wParam == VK_ESCAPE || 
			pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN || 
			//pMsg->wParam == VK_TAB ||
			GetKeyState(VK_CONTROL))
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;
		}
	}
	
	return CRichEditCtrl::PreTranslateMessage(pMsg);
}

void CInEdit::ProcessSelect()
{
	CString str;
	GetWindowText(str);

	if (m_sInitText != str) 
	{
		LV_DISPINFO dispinfo;
		dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
		dispinfo.hdr.idFrom = GetDlgCtrlID();
		dispinfo.hdr.code = LVN_ENDLABELEDIT;

		dispinfo.item.mask = LVIF_TEXT;
		dispinfo.item.iItem = m_iItem;
		dispinfo.item.iSubItem = m_iSubItem;
		dispinfo.item.pszText = m_bESC ? NULL : LPTSTR((LPCTSTR)str);
		dispinfo.item.cchTextMax = str.GetLength();
		GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo);
		


	}
}

void CInEdit::PostNcDestroy() 
{
	CRichEditCtrl::PostNcDestroy();
	delete this;	
}

BOOL CInEdit::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style = cs.style | ES_AUTOHSCROLL;
	
	return CRichEditCtrl::PreCreateWindow(cs);
}
