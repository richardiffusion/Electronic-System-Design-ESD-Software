#pragma once

#include "Page_Vertices.h"
#include "Page_EditWire.h"

// CSheet_EditWire

class CSheet_EditWire : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheet_EditWire)

public:
	CSheet_EditWire(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheet_EditWire(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheet_EditWire();

	CPage_EditWire page1;
	CPage_Vertices page2;

protected:
	DECLARE_MESSAGE_MAP()
public:
};


