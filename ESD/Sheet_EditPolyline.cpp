// Sheet_EditPolyline.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Sheet_EditPolyline.h"


// CSheet_EditPolyline

IMPLEMENT_DYNAMIC(CSheet_EditPolyline, CPropertySheet)
CSheet_EditPolyline::CSheet_EditPolyline(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &=  ~(PSH_HASHELP);
	m_psh.dwFlags |=  PSH_NOAPPLYNOW;

	AddPage(&page1);
	AddPage(&page2);
}

CSheet_EditPolyline::CSheet_EditPolyline(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &=  ~(PSH_HASHELP);
	m_psh.dwFlags |=  PSH_NOAPPLYNOW;

	AddPage(&page1);
	AddPage(&page2);
}

CSheet_EditPolyline::~CSheet_EditPolyline()
{
}


BEGIN_MESSAGE_MAP(CSheet_EditPolyline, CPropertySheet)
END_MESSAGE_MAP()


// CSheet_EditPolyline 消息处理程序
