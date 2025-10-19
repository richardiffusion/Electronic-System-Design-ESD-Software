//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ESD.h"
#include "ReportCtrlItem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//IMPLEMENT_SERIAL(CXTPReportRecordItemCheck, CXTPReportRecordItem, VERSIONABLE_SCHEMA | _XTP_SCHEMA_CURRENT)

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
// CXTPReportRecordItemColor

CXTPReportRecordItemColor::CXTPReportRecordItemColor()
{

}

void CXTPReportRecordItemColor::OnClick(XTP_REPORTRECORDITEM_CLICKARGS* pClickArgs)
{
	CColorDialog colordialog;
	int i = colordialog.DoModal();
	if(i == IDOK)
	{
		SetBackgroundColor(colordialog.GetColor());
	}
}

COLORREF CXTPReportRecordItemColor::GetBackgroundColor()
{
	return m_clrBackground;
}