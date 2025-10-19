#include "stdafx.h"

#include "sysparam.h"
#include "systemext.h"


IMPLEMENT_SERIAL(COb_schsheetsymboltext, CObject, 0)
COb_schsheetsymboltext::COb_schsheetsymboltext()
{
	m_nFlag = 0;
	m_nFont = 0;
	m_nSelection = 0;
	
	m_bHidden = FALSE;
	m_bNotAutoPosition = FALSE;
	m_nAnChorH = 0;
	m_nAnChorV = 0;
}



void COb_schsheetsymboltext::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{

	}
	else
	{

	}
}

void COb_schsheetsymboltext::Draw(CDC* pDC, CView* pV)
{
	COLORREF color;

	if(m_bHidden == TRUE) return;

	if(m_nFlag == FLAG_NORMAL) color = m_nColor;
	else if(m_nFlag == FLAG_DELETED) color = nEDA_BK_Color;
	else if(m_nFlag == FLAG_SELECTED) color = nEDA_Selected_Color;

	if(nEDA_Print_Flag != 0)
	{
		if(nEDA_Print_Color == 0) color = RGB(0,0,0);
	}

	
	// 设置文字颜色
	// 用pDC->SetTextColor(color)设置的颜色对于打印预览来说是近似颜色，
	// 由于在CPreviewDC中，SetTextColor 实际执行的是	
	//   ::SetTextColor(m_hDC, ::GetNearestColor(m_hAttribDC, Color));
	// 和::SetTextColor(m_hAttribDC, Color);
	// 因此，模拟显示只能得到一个近似的颜色
	COLORREF oldtextcolor;
	oldtextcolor = pDC->SetTextColor(color);
	::SetTextColor(pDC->m_hDC,color);
		
	// 设定背景模式
	int bkmode = pDC->SetBkMode(TRANSPARENT);

	// 输出文字
	ShowTrueTypeString(color, pDC, pV);


	// 恢复设备原来设置
	pDC->SetTextColor(oldtextcolor);
	pDC->SetBkMode(bkmode);
}
