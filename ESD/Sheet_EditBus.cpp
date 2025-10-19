// Sheet_EditBus.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Sheet_EditBus.h"
#include ".\sheet_editbus.h"


// CSheet_EditBus

IMPLEMENT_DYNAMIC(CSheet_EditBus, CPropertySheet)
CSheet_EditBus::CSheet_EditBus(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &=  ~(PSH_HASHELP);
	m_psh.dwFlags |=  PSH_NOAPPLYNOW;

	AddPage(&page1);
	AddPage(&page2);

}

CSheet_EditBus::CSheet_EditBus(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &=  ~(PSH_HASHELP);
	m_psh.dwFlags |=  PSH_NOAPPLYNOW;

	AddPage(&page1);
	AddPage(&page2);
}

CSheet_EditBus::~CSheet_EditBus()
{
}


BEGIN_MESSAGE_MAP(CSheet_EditBus, CPropertySheet)
END_MESSAGE_MAP()


