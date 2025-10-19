//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECORDSELECTPIN_H__AFFF918D_1624_41E5_8902_81B1F0C749BA__INCLUDED_)
#define AFX_RECORDSELECTPIN_H__AFFF918D_1624_41E5_8902_81B1F0C749BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPReportRecordItemCheck : public CXTPReportRecordItem
{
	//DECLARE_SERIAL(CXTPReportRecordItemCheck)
public:
	// Constructs record item with the initial checkbox value.
	CXTPReportRecordItemCheck(BOOL bCheck = FALSE);

	// Provides custom group captions depending on checkbox value.
	// Returns caption string ID to be read from application resources.
	virtual int GetGroupCaptionID(CXTPReportColumn* pColumn);

	// Provides custom records comparison by this item based on checkbox value, 
	// instead of based on captions.
	virtual int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);
	virtual void DrawCheckBox(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem);
};


class CXTPReportRecordItemColor : public CXTPReportRecordItem
{
public:
	// Constructs record item with the initial checkbox value.
	CXTPReportRecordItemColor();
	
	COLORREF GetBackgroundColor();
	virtual void OnClick(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs);

};

#endif // !defined(AFX_RECORDSELECTPIN_H__AFFF918D_1624_41E5_8902_81B1F0C749BA__INCLUDED_)
