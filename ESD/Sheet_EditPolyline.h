#pragma once


#include "Page_Vertices.h"
#include "Page_EditPolyline.h"

// CSheet_EditPolyline

class CSheet_EditPolyline : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheet_EditPolyline)

public:
	CSheet_EditPolyline(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheet_EditPolyline(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheet_EditPolyline();

	CPage_EditPolyline page1;
	CPage_Vertices page2;

protected:
	DECLARE_MESSAGE_MAP()
};


