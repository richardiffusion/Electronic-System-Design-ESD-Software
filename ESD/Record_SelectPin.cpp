// MessageRecord.cpp: implementation of the CRecord_SelectPin class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Record_SelectPin.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CRecord_SelectPin, CXTPReportRecord, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
IMPLEMENT_SERIAL(CXTPReportRecordItemCheck, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

/*IMPLEMENT_SERIAL(CRecord_SelectPinItemPrice, CXTPReportRecordItemNumber, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
IMPLEMENT_SERIAL(CRecord_SelectPinItemCheck, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
IMPLEMENT_SERIAL(CRecord_SelectPinItemImportance, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
IMPLEMENT_SERIAL(CRecord_SelectPinItemAttachment, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
IMPLEMENT_SERIAL(CRecord_SelectPinItemIcon, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)
IMPLEMENT_SERIAL(CRecord_SelectPinItemDate, CXTPReportRecordItemDateTime, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)*/


//////////////////////////////////////////////////////////////////////////
// CXTPReportRecordItemCheck

CXTPReportRecordItemCheck::CXTPReportRecordItemCheck(BOOL bCheck)
{
	HasCheckbox(TRUE);
	SetChecked(bCheck);
}

int CXTPReportRecordItemCheck::GetGroupCaptionID(CXTPReportColumn* /*pColumn*/)
{
	return IsChecked();
}

int CXTPReportRecordItemCheck::Compare(CXTPReportColumn* /*pColumn*/, CXTPReportRecordItem* pItem)
{
	return int(IsChecked()) - int(pItem->IsChecked());

}

void CXTPReportRecordItemCheck::DrawCheckBox(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, CRect& rcItem)
{
	BOOL bEditable = IsEditable() && (pDrawArgs->pColumn == NULL || pDrawArgs->pColumn->IsEditable());

	CDC* pDC = pDrawArgs->pDC;
	int eState = (bEditable ? 0: 2) + (IsChecked() ? 1 : 0);

	CXTPReportPaintManager* pPaintManager = pDrawArgs->pControl->GetPaintManager();

	int nIconAlign = pDrawArgs->pColumn ?
		pDrawArgs->pColumn->GetAlignment() & xtpColumnIconMask : 0;

	m_rcGlyph = rcItem;
	CSize sizeGlyph = pPaintManager->DrawGlyph(NULL, m_rcGlyph, eState + 2);

	CRect rcGlyph(m_rcGlyph);
	// horizontal alignment

	//总是居中对齐
	nIconAlign = xtpColumnIconCenter;

	switch(nIconAlign)
	{
	case xtpColumnIconRight:
		rcGlyph.left = rcItem.right - sizeGlyph.cx - 2;
		// shift text box to the left
		rcItem.right -= (2 + sizeGlyph.cx);
		break;
	case xtpColumnIconCenter:
		rcGlyph.left = rcItem.CenterPoint().x - sizeGlyph.cx / 2 - 1;
		rcGlyph.right = rcGlyph.left + sizeGlyph.cx;
		break; // left text box as is - draw on image...
	case xtpColumnIconLeft:
	default:
		rcGlyph.left += 2;
		// shift text box to the right
		rcItem.left += 2 + sizeGlyph.cx;
		rcGlyph.right = rcItem.left - 1;
		break;
	}
	// vertical alignment
	switch(nIconAlign)
	{
	case xtpColumnIconTop:
		rcGlyph.bottom = rcItem.top + sizeGlyph.cy + 2;
		break;
	case xtpColumnIconBottom:
		rcGlyph.top = rcItem.bottom - sizeGlyph.cy - 2;
		break;
	}
	// draw the glyph
	pPaintManager->DrawGlyph(pDC, rcGlyph, eState + 2);
}


//////////////////////////////////////////////////////////////////////////
// CRecord_SelectPinItemPrice

/*CRecord_SelectPinItemPrice::CRecord_SelectPinItemPrice(double dValue)
: CXTPReportRecordItemNumber(dValue, _T("$ %.2f"))
{

}
CString CRecord_SelectPinItemPrice::GetGroupCaption(CXTPReportColumn* )  //pColumn
{
	if (m_dValue > 20)
		return _T("Record Price > 20");

	if (m_dValue > 5)
		return _T("Record Price 5 - 20");

	return _T("Record Price < 5");
}

int CRecord_SelectPinItemPrice::CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem)
{
	return GetGroupCaption(pColumn).Compare(pItem->GetGroupCaption(pColumn));
}

//////////////////////////////////////////////////////////////////////////
// CRecord_SelectPinItemCheck

CRecord_SelectPinItemCheck::CRecord_SelectPinItemCheck(BOOL bCheck)
{
	HasCheckbox(TRUE);
	SetChecked(bCheck);
}

int CRecord_SelectPinItemCheck::GetGroupCaptionID(CXTPReportColumn* )      //pColumn
{
	return IsChecked()? IDS_GROUP_CHECKED_TRUE: IDS_GROUP_CHECKED_FALSE;
}

int CRecord_SelectPinItemCheck::Compare(CXTPReportColumn*, CXTPReportRecordItem* pItem)
{
	return int(IsChecked()) - int(pItem->IsChecked());

}


//////////////////////////////////////////////////////////////////////////
// CRecord_SelectPinItemImportance

CRecord_SelectPinItemImportance::CRecord_SelectPinItemImportance(MESSAGE_IMPORTANCE eImportance)
	: m_eImportance(eImportance)
{
	SetIconIndex(eImportance == msgImportanceHigh ? 6 :
								 eImportance == msgImportanceLow ? 9: -1);

	SetGroupPriority(
		m_eImportance == msgImportanceHigh? IDS_GROUP_IMPOPRTANCE_HIGH:
		m_eImportance == msgImportanceLow?  IDS_GROUP_IMPOPRTANCE_LOW: IDS_GROUP_IMPOPRTANCE_NORMAL);

	SetSortPriority(GetGroupPriority());
	
	CString strToolTip;
	strToolTip.LoadString(GetGroupPriority());
	
	SetTooltip(strToolTip);
}

void CRecord_SelectPinItemImportance::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPReportRecordItem::DoPropExchange(pPX);

	PX_Enum(pPX, _T("Importance"), m_eImportance, msgImportanceNormal);
}


//////////////////////////////////////////////////////////////////////////
// CRecord_SelectPinItemAttachment

CRecord_SelectPinItemAttachment::CRecord_SelectPinItemAttachment(BOOL bHasAttachment)
	: m_bHasAttachment(bHasAttachment)
{
	SetIconIndex(bHasAttachment ? 8 : -1);
	SetGroupPriority(m_bHasAttachment? IDS_GROUP_ATTACHMENTS_TRUE: IDS_GROUP_ATTACHMENTS_FALSE);
	SetSortPriority(GetGroupPriority());
}

void CRecord_SelectPinItemAttachment::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPReportRecordItem::DoPropExchange(pPX);

	PX_Bool(pPX, _T("HasAttachment"), m_bHasAttachment);
}

//////////////////////////////////////////////////////////////////////////
// CRecord_SelectPinItemIcon

CRecord_SelectPinItemIcon::CRecord_SelectPinItemIcon(BOOL bRead)
	: m_bRead(bRead)
{
	UpdateReadIcon();
}

void CRecord_SelectPinItemIcon::UpdateReadIcon()
{
	SetIconIndex(m_bRead ? 4 : 3);

}

int CRecord_SelectPinItemIcon::Compare(CXTPReportColumn* , CXTPReportRecordItem* pItem)
{
	return int(m_bRead) - int(((CRecord_SelectPinItemIcon*)pItem)->m_bRead);
}


CString CRecord_SelectPinItemIcon::GetGroupCaption(CXTPReportColumn* )
{
	if (m_bRead)
		return _T("Message status: Read");
	else
		return _T("Message status: Unread");
}

int CRecord_SelectPinItemIcon::CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem)
{
	return GetGroupCaption(pColumn).Compare(pItem->GetGroupCaption(pColumn));
}

void CRecord_SelectPinItemIcon::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPReportRecordItem::DoPropExchange(pPX);

	PX_Bool(pPX, _T("Read"), m_bRead);
}


//////////////////////////////////////////////////////////////////////////
// CRecord_SelectPinItemSent

CRecord_SelectPinItemDate::CRecord_SelectPinItemDate(COleDateTime odtValue)
	: CXTPReportRecordItemDateTime(odtValue)
{
}

int CRecord_SelectPinItemDate::GetGroupCaptionID(CXTPReportColumn* )
{
	COleDateTime odtNow(COleDateTime::GetCurrentTime());

	if (m_odtValue.GetYear() < odtNow.GetYear())
		return IDS_GROUP_DATE_OLDER;

	if (m_odtValue.GetMonth() < odtNow.GetMonth())
		return IDS_GROUP_DATE_THISYEAR;

	if (m_odtValue.GetDay() < odtNow.GetDay())
		return IDS_GROUP_DATE_THISMONTH;

	if (m_odtValue.m_dt <= odtNow.m_dt)
		return IDS_GROUP_DATE_TODAY;

	return -1;

}
*/


//////////////////////////////////////////////////////////////////////
// CRecord_SelectPin class

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRecord_SelectPin::CRecord_SelectPin()
{
/*	m_pItemIcon = NULL;
	m_pItemReceived = NULL;
	m_pItemSize = NULL;

	CreateItems();*/
}

/*CRecord_SelectPin::CRecord_SelectPin(
					MESSAGE_IMPORTANCE eImportance, BOOL bChecked, int  nAttachmentBitmap,
					CString strFromName, CString strSubject,
					COleDateTime odtReceived, int nMessageSize, BOOL bRead,
					double dPrice, COleDateTime odtSent, COleDateTime odtCreated,
					CString strConversation, CString strContact, CString strMessage,
					CString strCC, CString strCategories, CString strAutoforward,
					CString strDoNotAutoarch, CString strDueBy,
					CString strPreview)
{
	AddItem(new CRecord_SelectPinItemImportance(eImportance));
	m_pItemIcon = (CRecord_SelectPinItemIcon*)AddItem(new CRecord_SelectPinItemIcon(bRead));
	AddItem(new CRecord_SelectPinItemAttachment(nAttachmentBitmap));
	AddItem(new CXTPReportRecordItemText(strFromName));
	AddItem(new CXTPReportRecordItemText(strSubject));
	m_pItemReceived = (CRecord_SelectPinItemDate*)AddItem(new CRecord_SelectPinItemDate(odtReceived));
	m_pItemSize = AddItem(new CXTPReportRecordItemNumber(nMessageSize));
	AddItem(new CRecord_SelectPinItemCheck(bChecked));
	AddItem(new CRecord_SelectPinItemPrice(dPrice));
	AddItem(new CRecord_SelectPinItemDate(odtCreated));
	AddItem(new CRecord_SelectPinItemDate(odtSent));
	AddItem(new CXTPReportRecordItemText(strConversation));
	AddItem(new CXTPReportRecordItemText(strContact));
	AddItem(new CXTPReportRecordItemText(strMessage));
	AddItem(new CXTPReportRecordItemText(strCC));
	AddItem(new CXTPReportRecordItemText(strCategories));
	AddItem(new CXTPReportRecordItemText(strAutoforward));
	AddItem(new CXTPReportRecordItemText(strDoNotAutoarch));
	AddItem(new CXTPReportRecordItemText(strDueBy));


	SetPreviewItem(new CXTPReportRecordItemPreview(strPreview));

}*/

void CRecord_SelectPin::CreateItems()
{
	// Initialize record items with empty values
	
/*	COleDateTime dtNow(COleDateTime::GetCurrentTime());

	// 0 
	AddItem(new CRecord_SelectPinItemImportance(msgImportanceNormal));

	// 1 ***
	m_pItemIcon = (CRecord_SelectPinItemIcon*)AddItem(new CRecord_SelectPinItemIcon(TRUE));
	
	// 2 
	AddItem(new CRecord_SelectPinItemAttachment(0));
	
	// 3 
	AddItem(new CXTPReportRecordItemText(_T("")));

	// 4 
	AddItem(new CXTPReportRecordItemText(_T("")));

	// 5 ***
	m_pItemReceived = (CRecord_SelectPinItemDate*)AddItem(new CRecord_SelectPinItemDate(dtNow));

	// 6 ***
	m_pItemSize = AddItem(new CXTPReportRecordItemNumber(0));
	
	AddItem(new CRecord_SelectPinItemCheck(FALSE));
	AddItem(new CRecord_SelectPinItemPrice(0));
	AddItem(new CRecord_SelectPinItemDate(dtNow));
	AddItem(new CRecord_SelectPinItemDate(dtNow));
	AddItem(new CXTPReportRecordItemText(_T("")));
	AddItem(new CXTPReportRecordItemText(_T("")));
	AddItem(new CXTPReportRecordItemText(_T("")));
	AddItem(new CXTPReportRecordItemText(_T("")));
	AddItem(new CXTPReportRecordItemText(_T("")));
	AddItem(new CXTPReportRecordItemText(_T("")));
	AddItem(new CXTPReportRecordItemText(_T("")));
	AddItem(new CXTPReportRecordItemText(_T("")));

	SetPreviewItem(new CXTPReportRecordItemPreview(_T("")));*/
}

CRecord_SelectPin::~CRecord_SelectPin()
{

}

BOOL CRecord_SelectPin::SetRead()
{
/*	ASSERT(m_pItemIcon);
	if (m_pItemIcon->m_bRead)
		return FALSE;

	m_pItemIcon->m_bRead = TRUE;
	m_pItemIcon->UpdateReadIcon();*/

	return TRUE;
}

void CRecord_SelectPin::GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
{
	CXTPReportRecord::GetItemMetrics(pDrawArgs, pItemMetrics);
}

void CRecord_SelectPin::DoPropExchange(CXTPPropExchange* pPX)
{
	CXTPReportRecord::DoPropExchange(pPX);

	if (pPX->IsLoading())
	{
		// 1 - m_pItemIcon = (CRecord_SelectPinItemIcon*)AddItem(new CRecord_SelectPinItemIcon(TRUE));
/*		ASSERT_KINDOF(CRecord_SelectPinItemIcon, GetItem(1));
		m_pItemIcon = DYNAMIC_DOWNCAST(CRecord_SelectPinItemIcon, GetItem(1));
		ASSERT(m_pItemIcon);
		
		// 5 - m_pItemReceived = (CRecord_SelectPinItemDate*)AddItem(new CRecord_SelectPinItemDate(dtNow));
		ASSERT_KINDOF(CRecord_SelectPinItemDate, GetItem(5));
		m_pItemReceived = DYNAMIC_DOWNCAST(CRecord_SelectPinItemDate, GetItem(5));
		ASSERT(m_pItemReceived);
		
		// 6 - m_pItemSize = AddItem(new CXTPReportRecordItemNumber(0));
		ASSERT_KINDOF(CXTPReportRecordItemNumber, GetItem(6));
		m_pItemSize = DYNAMIC_DOWNCAST(CXTPReportRecordItemNumber, GetItem(6));
		ASSERT(m_pItemSize);		*/
	}
}
