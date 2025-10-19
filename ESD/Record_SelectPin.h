// MessageRecord.h: interface for the CRecord_SelectPin class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECORDSELECTPIN_H__AFFF918D_1624_41E5_8902_81B1F0C749BA__INCLUDED_)
#define AFX_RECORDSELECTPIN_H__AFFF918D_1624_41E5_8902_81B1F0C749BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//class CReportSampleView;

class CXTPReportRecordItemCheck : public CXTPReportRecordItem
{
	DECLARE_SERIAL(CXTPReportRecordItemCheck)
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


//////////////////////////////////////////////////////////////////////////
// Customized record item, used for displaying prices.
/*class CRecord_SelectPinItemPrice : public CXTPReportRecordItemNumber
{
	DECLARE_SERIAL(CRecord_SelectPinItemPrice)
public:
	// Constructs record item with the initial decimal price value.
	CRecord_SelectPinItemPrice(double dValue = .0);

	// Provides custom group captions depending on the price.
	virtual CString GetGroupCaption(CXTPReportColumn* pColumn);

	// Provides custom group values comparison based on price value, 
	// instead of based on captions.
	virtual int CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);
};

//////////////////////////////////////////////////////////////////////////
// Customized record item, used for displaying checkboxes.
class CRecord_SelectPinItemCheck : public CXTPReportRecordItem
{
	DECLARE_SERIAL(CRecord_SelectPinItemCheck)
public:
	// Constructs record item with the initial checkbox value.
	CRecord_SelectPinItemCheck(BOOL bCheck = FALSE);

	// Provides custom group captions depending on checkbox value.
	// Returns caption string ID to be read from application resources.
	virtual int GetGroupCaptionID(CXTPReportColumn* pColumn);

	// Provides custom records comparison by this item based on checkbox value, 
	// instead of based on captions.
	virtual int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);
};

//////////////////////////////////////////////////////////////////////////
// Enumerates possible Message Importance values for using by 
// CRecord_SelectPinItemImportance class
typedef enum MESSAGE_IMPORTANCE
{
	msgImportanceNormal,
	msgImportanceHigh,
	msgImportanceLow
};

//////////////////////////////////////////////////////////////////////////
// Customized record item, used for displaying importance icons.
class CRecord_SelectPinItemImportance : public CXTPReportRecordItem
{
	DECLARE_SERIAL(CRecord_SelectPinItemImportance)
public:
	// Constructs record item with the initial value.
	CRecord_SelectPinItemImportance(MESSAGE_IMPORTANCE eImportance = msgImportanceNormal);
	
	virtual void DoPropExchange(CXTPPropExchange* pPX);
protected:
	MESSAGE_IMPORTANCE m_eImportance;   // Message importance
};

//////////////////////////////////////////////////////////////////////////
// Customized record item, used for displaying attachments icons.
class CRecord_SelectPinItemAttachment : public CXTPReportRecordItem
{
	DECLARE_SERIAL(CRecord_SelectPinItemAttachment)
public:
	// Constructs record item with the initial value.
	CRecord_SelectPinItemAttachment(BOOL bHasAttachment = FALSE);

	virtual void DoPropExchange(CXTPPropExchange* pPX);
protected:
	BOOL m_bHasAttachment;	// TRUE when message has attachments, FALSE otherwise.
};

//////////////////////////////////////////////////////////////////////////
// Customized record item, used for displaying read/unread icons.
class CRecord_SelectPinItemIcon : public CXTPReportRecordItem
{
	DECLARE_SERIAL(CRecord_SelectPinItemIcon)
public:
	// Constructs record item with the initial read/unread value.
	CRecord_SelectPinItemIcon(BOOL bRead = FALSE);

	// Provides custom group captions depending on the read/unread value.
	virtual CString GetGroupCaption(CXTPReportColumn* pColumn);

	// Provides custom group values comparison based on read/unread value, 
	// instead of based on captions.
	virtual int CompareGroupCaption(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);

	// Updates record item icon index depending on read/unread value.
	void UpdateReadIcon();

	// Provides custom records comparison by this item based on read/unread value, 
	// instead of based on captions.
	int Compare(CXTPReportColumn* pColumn, CXTPReportRecordItem* pItem);

	virtual void DoPropExchange(CXTPPropExchange* pPX);
public:
	BOOL m_bRead;	// TRUE for read, FALSE for unread.
};

//////////////////////////////////////////////////////////////////////////
// Customized record Date/Time item.
// Main customization purpose is overriding GetGroupCaptionID and providing
// application-specific caption when Report control data is grouped via this item.
class CRecord_SelectPinItemDate : public CXTPReportRecordItemDateTime
{
	DECLARE_SERIAL(CRecord_SelectPinItemDate)
public:
	// Construct record item from COleDateTime value.
	CRecord_SelectPinItemDate(COleDateTime odtValue = COleDateTime::GetCurrentTime());

	// Provides custom group captions depending on the item date value.
	virtual int GetGroupCaptionID(CXTPReportColumn* pColumn);
};*/

//////////////////////////////////////////////////////////////////////////
// This class is your main custom Record class which you'll manipulate with.
// It contains any kind of specific methods like different types of constructors,
// any additional custom data as class members, any data manipulation methods.
class CRecord_SelectPin : public CXTPReportRecord
{
	DECLARE_SERIAL(CRecord_SelectPin)
public:
	
	// Construct record object using empty values on each field
	CRecord_SelectPin();
	
	// Construct record object from detailed values on each field
	/*CRecord_SelectPin(
		MESSAGE_IMPORTANCE eImportance, BOOL bChecked, int  nAttachmentBitmap,
		CString strFromName, CString strSubject,
		COleDateTime odtSent, int nMessageSize, BOOL bRead,
		double dPrice, COleDateTime odtReceived, COleDateTime odtCreated,
		CString strConversation, CString strContact, CString strMessage,
		CString strCC, CString strCategories, CString strAutoforward,
		CString strDoNotAutoarch, CString strDueBy,
		CString strPreview
		);*/

	// Clean up internal objects
	virtual ~CRecord_SelectPin();

	// Create record fields using empty values
	virtual void CreateItems();

	// Set message as read
	BOOL SetRead();

	// Overridden callback method, where we can customize any drawing item metrics.
	virtual void GetItemMetrics(XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs, XTP_REPORTRECORDITEM_METRICS* pItemMetrics);

	virtual void DoPropExchange(CXTPPropExchange* pPX);

	//CRecord_SelectPinItemIcon* m_pItemIcon;	// Display read/unread icon.
	//CRecord_SelectPinItemDate* m_pItemReceived;// Contains message receive time.
	//CXTPReportRecordItem*	m_pItemSize;	// Message size. 
											// We are storing pointer to this item for further use.

};

#endif // !defined(AFX_RECORDSELECTPIN_H__AFFF918D_1624_41E5_8902_81B1F0C749BA__INCLUDED_)
