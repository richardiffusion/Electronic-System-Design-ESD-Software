// Sheet_EditWire.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Sheet_EditWire.h"
#include ".\sheet_editwire.h"


// CSheet_EditWire

IMPLEMENT_DYNAMIC(CSheet_EditWire, CPropertySheet)
CSheet_EditWire::CSheet_EditWire(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &=  ~(PSH_HASHELP);
	m_psh.dwFlags |=  PSH_NOAPPLYNOW;

	AddPage(&page1);
	AddPage(&page2);
}

CSheet_EditWire::CSheet_EditWire(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &=  ~(PSH_HASHELP);
	m_psh.dwFlags |=  PSH_NOAPPLYNOW;

	AddPage(&page1);
	AddPage(&page2);
}

CSheet_EditWire::~CSheet_EditWire()
{
}


BEGIN_MESSAGE_MAP(CSheet_EditWire, CPropertySheet)
END_MESSAGE_MAP()


// CSheet_EditWire 消息处理程序

