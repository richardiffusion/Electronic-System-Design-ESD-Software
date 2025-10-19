// SchSheetOptionsSheet.cpp : 实现文件
//

#include "stdafx.h"
#include "ESD.h"
#include "Sheet_SchOptions.h"


// CSheet_SchOptions

IMPLEMENT_DYNAMIC(CSheet_SchOptions, CPropertySheet)
CSheet_SchOptions::CSheet_SchOptions(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &=  ~(PSH_HASHELP);
	m_psh.dwFlags |=  PSH_NOAPPLYNOW;

	AddPage(&m_PageOptions);
	AddPage(&m_PageUnit);
	//AddPage(&m_nDiagram);
}

CSheet_SchOptions::CSheet_SchOptions(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_psh.dwFlags &=  ~(PSH_HASHELP);
	m_psh.dwFlags |=  PSH_NOAPPLYNOW;

	AddPage(&m_PageOptions);
	AddPage(&m_PageUnit);
	//AddPage(&m_nDiagram);
}

CSheet_SchOptions::~CSheet_SchOptions()
{
}


BEGIN_MESSAGE_MAP(CSheet_SchOptions, CPropertySheet)
END_MESSAGE_MAP()


// CSheet_SchOptions 消息处理程序
