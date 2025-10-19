// Sheet_EditPolygon.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ESD.h"
#include "Sheet_EditPolygon.h"


// CSheet_EditPolygon

IMPLEMENT_DYNAMIC(CSheet_EditPolygon, CPropertySheet)
CSheet_EditPolygon::CSheet_EditPolygon(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &=  ~(PSH_HASHELP);
	m_psh.dwFlags |=  PSH_NOAPPLYNOW;

	AddPage(&page1);
	AddPage(&page2);
}

CSheet_EditPolygon::CSheet_EditPolygon(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &=  ~(PSH_HASHELP);
	m_psh.dwFlags |=  PSH_NOAPPLYNOW;

	AddPage(&page1);
	AddPage(&page2);
}

CSheet_EditPolygon::~CSheet_EditPolygon()
{
}


BEGIN_MESSAGE_MAP(CSheet_EditPolygon, CPropertySheet)
END_MESSAGE_MAP()


// CSheet_EditPolygon ��Ϣ�������
