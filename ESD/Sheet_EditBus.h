#pragma once

#include "Page_Vertices.h"
#include "Page_EditBus.h"

// CSheet_EditBus

class CSheet_EditBus : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheet_EditBus)

public:
	CSheet_EditBus(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheet_EditBus(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheet_EditBus();

	CPage_EditBus page1;
	CPage_Vertices page2;

protected:
	DECLARE_MESSAGE_MAP()
public:
};


