#pragma once

#include "Page_Vertices.h"
#include "Page_EditPolygon.h"

// CSheet_EditPolygon

class CSheet_EditPolygon : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheet_EditPolygon)

public:
	CSheet_EditPolygon(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheet_EditPolygon(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheet_EditPolygon();

	CPage_EditPolygon page1;
	CPage_Vertices page2;

protected:
	DECLARE_MESSAGE_MAP()
};


