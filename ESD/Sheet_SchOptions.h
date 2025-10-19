#pragma once


#include "Page_SchOptions.h"
#include "Page_SchUnit.h"

// CSheet_SchOptions

class CSheet_SchOptions : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheet_SchOptions)

public:
	CSheet_SchOptions(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheet_SchOptions(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheet_SchOptions();

	CPage_SchOptions m_PageOptions;
	CPage_SchUnit m_PageUnit;
protected:
	DECLARE_MESSAGE_MAP()
};


