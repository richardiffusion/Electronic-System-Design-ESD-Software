#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_EditText.h"
#include "Dlg_EditNetlabel.h"
#include "Dlg_EditSheetSymbolDF.h"
#include "Dlg_EditCompText.h"
#include "Dlg_Global_Text.h"
#include "Dlg_Global_Designator.h"


IMPLEMENT_SERIAL(COb_schtext, CObject, 0)
COb_schtext::COb_schtext()
{
	m_nFlag = 0;
	m_bSelection = FALSE;
	m_nFont = 0;
	
	m_nH_Justification = 0;	
	m_nV_Justification = 0;
	
	m_bMirror = FALSE;
	m_nID = TEXT_TEXT;
	
	m_nOwnerPartID = 0;
	m_nDisplayMode = 0;
	
	m_posParent = NULL;

	m_csText.Empty();
	m_bTextVisible = TRUE;
	m_bTextLocked = FALSE;
	
	m_csNameComment.Empty();
	m_bNameVisible = FALSE;
	m_bNameLocked = FALSE;

	m_bMirror = FALSE;
	m_bAutoPosition = TRUE;
	m_nParamType = 0;
	m_csUniqueID.Empty();
	m_bAllowDataBaseSynchronize = TRUE;
	m_bAllowLibrarySynchronize = TRUE;

	m_nAnChorH = 0;
	m_nAnChorV = 0;

	pDocument = NULL;
}



void COb_schtext::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_nID;
		ar << m_fSx;
		ar << m_fSy;
		ar << m_nOrientation;

		ar << m_csText;
		ar << m_bTextVisible;
		ar << m_bTextLocked;

		ar << m_csNameComment;
		ar << m_bNameVisible;
		ar << m_bNameLocked;

		ar << m_nFont;
		ar << m_nColor;

		ar << m_bMirror;
		ar << m_bAutoPosition;
		ar << m_nParamType;
		ar << m_csUniqueID;
		ar << m_bAllowDataBaseSynchronize;
		ar << m_bAllowLibrarySynchronize;

		ar << m_nH_Justification;
		ar << m_nV_Justification;

		ar << m_nAnChorH;
		ar << m_nAnChorV;

		ar << m_nOwnerPartID;
		ar << m_nDisplayMode;

		ar << m_nDiagram;
		ar << m_bSelection;
	}
	else
	{
		ar >> m_nID;
		ar >> m_fSx;
		ar >> m_fSy;
		ar >> m_nOrientation;

		ar >> m_csText;

		ar >> m_bTextVisible;
		ar >> m_bTextLocked;

		ar >> m_csNameComment;
		ar >> m_bNameVisible;
		ar >> m_bNameLocked;

		ar >> m_nFont;
		ar >> m_nColor;

		ar >> m_bMirror;
		ar >> m_bAutoPosition;
		ar >> m_nParamType;
		ar >> m_csUniqueID;
		ar >> m_bAllowDataBaseSynchronize;
		ar >> m_bAllowLibrarySynchronize;

		ar >> m_nH_Justification;
		ar >> m_nV_Justification;

		ar >> m_nAnChorH;
		ar >> m_nAnChorV;

		ar >> m_nOwnerPartID;
		ar >> m_nDisplayMode;

		ar >> m_nDiagram;
		ar >> m_bSelection;		
	}
}

void COb_schtext::Draw(CDC* pDC, CView* pV)
{
	COLORREF color;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(m_nFlag == FLAG_NORMAL) color = m_nColor;
	else if(m_nFlag == FLAG_DELETED) color = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor;

	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_Print_Color == 0) color = RGB(0,0,0);
		else if(nEDA_Print_Color == 2) color = RGB(GetGray(m_nColor), GetGray(m_nColor), GetGray(m_nColor));
	}

	COLORREF oldtextcolor;
	oldtextcolor = pDC->SetTextColor(color);
		
	int bkmode = pDC->SetBkMode(TRANSPARENT);

	ShowTrueTypeString(pDC, pV);

	pDC->SetTextColor(oldtextcolor);
	pDC->SetBkMode(bkmode);
}

void COb_schtext::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	COLORREF color;
	color = m_nColor;

	COLORREF oldtextcolor;
	oldtextcolor = pDC->SetTextColor(color);
		
	int bkmode = pDC->SetBkMode(TRANSPARENT);

	int x,y;
	double tempfx, tempfy;
	int xx,yy;
	char str[1000];
	char original_str[1000];
	int upperline[1000];
	int width;

	CString text = m_csText;
	text.Left(990);
	strcpy(original_str, text);

	LOGFONT logfont;
	logfont.lfHeight = 10;
	logfont.lfWidth = 0;
	logfont.lfEscapement = logfont.lfOrientation = 0;
	logfont.lfWeight = FW_NORMAL;
	logfont.lfItalic = 0;
	logfont.lfUnderline = 0;
	logfont.lfStrikeOut = 0;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = PROOF_QUALITY;
	logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(logfont.lfFaceName, "Times New Roman");

	double height = (double)logfont.lfHeight;
	height *= fEDA_Font_Scale;
	logfont.lfOrientation = m_nOrientation;
	logfont.lfEscapement = logfont.lfOrientation*10;

	int offset = 0;
	if(height > 300) offset = (int)(height*0.025); 


	if(m_nOrientation == 90 )
	{
		tempfx = m_fSx - (height + offset);
		tempfy = m_fSy;

		x = (int)((tempfx - origin_x)/scale);
		y = nEy - (int)((tempfy - origin_y)/scale);
	}
	else if(m_nOrientation == 180 )
	{
		tempfx = m_fSx;
		tempfy = m_fSy - (height + offset);

		x = (int)((tempfx - origin_x)/scale);
		y = nEy - (int)((tempfy - origin_y)/scale);
	}
	else if(m_nOrientation == 270 )
	{
		tempfx = m_fSx + (height + offset);
		tempfy = m_fSy;

		x = (int)((tempfx - origin_x)/scale);
		y = nEy - (int)((tempfy - origin_y)/scale);
	}
	else 
	{
		tempfx = m_fSx;
		tempfy = m_fSy + (height + offset);

		x = (int)((tempfx - origin_x)/scale);
		y = nEy - (int)((tempfy - origin_y)/scale);
	}

	xx = x;
	yy = y;


	logfont.lfHeight = (int)(height/scale);
	logfont.lfWidth = 0;
	
	if(logfont.lfHeight <= 0) return;

	CFont font, *oldfont;
	font.CreateFontIndirect(&logfont);
	oldfont = pDC->SelectObject(&font);

	UINT flag = pDC->GetTextAlign();
	UINT h;
	if(m_nH_Justification == 0) h = TA_LEFT;
	else if(m_nH_Justification == 1) h = TA_CENTER;
	else if(m_nH_Justification == 2) h = TA_RIGHT;
	else h = TA_LEFT;

	pDC->SetTextAlign(h|flag);


	int upperlinenum = 0;
	int num = 0;
	int i = 0;
	for(;;)
	{
		if(original_str[num] == '\0') 
		{
			str[i] = '\0';
			break;
		}
		else if(original_str[num] == 0x5c) //  0x5c = '\'
		{
			if(i != 0)
			{
				upperline[upperlinenum] = i; //record the upperline
				upperlinenum++;
			}
			num++;
		}
		else
		{
			str[i] = original_str[num];
			i++;
			num++;
		}
	}
			
	pDC->TextOut(xx, yy, str, strlen(str));
		

	double delta = 0;


	width = (int)(nEDA_Line_Width[LINE_SMALL]/scale);
	if(width < 1) width = 1;
	
	int xx1,yy1;
	char str1[MAX_STRING_LENGTH];
	CSize size1, size2, size;
	int start;
	for(i=0; i<upperlinenum; i++)
	{
		strcpy(str1, str);
		start = upperline[i];
		if(start <= 0) continue;
		str1[start] = '\0';
		size2 = pDC->GetTextExtent(str1, strlen(str1));
		
		if(str1[start-1] < 0) str1[start-2] = '\0';
		else str1[start-1] = '\0';

		size1 = pDC->GetTextExtent(str1, strlen(str1));
		size = pDC->GetTextExtent(str, strlen(str));
		if(m_nH_Justification == 1)
		{
			size.cx = size.cx/2;
			size1 = size1 - size;
			size2 = size2 - size;
		}
		else if(m_nH_Justification == 2)
		{
			size1 = size1 - size;
			size2 = size2 - size;
		}


		CPen pen, *oldpen;
		pen.CreatePen(PS_SOLID, width,color);
		oldpen = pDC->SelectObject(&pen);

		int dx, dy;
		if(m_nOrientation == 0)
		{
			xx1 = xx + size1.cx;
			yy1 = yy +  (int)(height*delta/scale);
			dx = xx + size2.cx;
			dy = yy1;
		}
		else if(m_nOrientation == 90)
		{
			xx1 = xx + (int)(height*delta/scale);
			yy1 = yy - size1.cx;
			dx = xx1;
			dy = yy - size2.cx;
		}
		else if(m_nOrientation == 180)
		{
			xx1 = xx - size1.cx;
			yy1 = yy - (int)(height*delta/scale);
			dx = xx - size2.cx;
			dy = yy1;
		}
		else if(m_nOrientation == 270)
		{
			xx1 = xx - (int)(height*delta/scale);
			yy1 = yy + size1.cx;
			dx = xx1;
			dy = yy + size2.cx;
		}

		pDC->MoveTo(xx1,yy1);
		pDC->LineTo(dx,dy);
		pDC->SelectObject(oldpen);
	}
	
	pDC->SelectObject(oldfont);
	pDC->SetTextAlign(flag);


	pDC->SetTextColor(oldtextcolor);
	pDC->SetBkMode(bkmode);
}

CSize COb_schtext::GetExtent(CDC* pDC, CView* pV)
{
	char original_str[1000];

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	CString text = m_csText;
	text.Left(990);
	strcpy(original_str, text);

	LOGFONT logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
	double height = (double)logfont.lfHeight;
	height *= fEDA_Font_Scale;
	logfont.lfOrientation = m_nOrientation;
	logfont.lfEscapement = logfont.lfOrientation*10;


	if(pView->m_nPrint_Flag == 1)
	{
		logfont.lfHeight = (int)((height*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		logfont.lfWidth = 0;
	}
	else
	{
		logfont.lfHeight = (int)(height/pView->m_fScale);
		logfont.lfWidth = 0;
	}
	
	if(logfont.lfHeight == 0) return 0;

	CFont font, *oldfont;
	font.CreateFontIndirect(&logfont);
	oldfont = pDC->SelectObject(&font);

	CSize size1;

	size1 = pDC->GetOutputTextExtent(original_str, strlen(original_str));
	
			
	pDC->SelectObject(oldfont);

	return size1;
}

void COb_schtext::ShowTrueTypeString(CDC* pDC, CView* pV)
{
	int x,y;
	char original_str[1000];


	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	CString text = m_csText;
	text.Left(990);
	strcpy(original_str, text);

	LOGFONT logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
	double height = (double)logfont.lfHeight;
	height *= fEDA_Font_Scale;
	logfont.lfOrientation = m_nOrientation;
	logfont.lfEscapement = logfont.lfOrientation*10;

	pView->Get_VPort_XY(&x,&y,m_fSx,m_fSy);
	if(pView->m_nPrint_Flag == 1)
	{
		logfont.lfHeight = (int)((height*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		logfont.lfWidth = 0;
		x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
	}
	else
	{
		logfont.lfHeight = (int)(height/pView->m_fScale);
		logfont.lfWidth = 0;
	}
	
	if(logfont.lfHeight == 0) return;
	
	UINT flag = pDC->GetTextAlign();
	UINT h,v;
	if(m_nH_Justification == 0) h = TA_LEFT;
	else if(m_nH_Justification == 1) h = TA_CENTER;
	else if(m_nH_Justification == 2) h = TA_RIGHT;
	else h = TA_LEFT;

	if(m_nV_Justification == 0) v = TA_BOTTOM;
	else if(m_nV_Justification == 1) v = TA_BASELINE;
	else if(m_nV_Justification == 2) v = TA_TOP;
	else v = TA_BOTTOM;

	pDC->SetTextAlign(v|h);

	CFont font, *oldfont;
	font.CreateFontIndirect(&logfont);
	oldfont = pDC->SelectObject(&font);

	pDC->TextOut(x, y, original_str, strlen(original_str));
		
	pDC->SelectObject(oldfont);
	pDC->SetTextAlign(flag);
}

void COb_schtext::DrawSignal(CDC* pDC, CView* pV)
{
	COLORREF color;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(m_nFlag == FLAG_NORMAL) color = m_nColor;
	else if(m_nFlag == FLAG_DELETED) color = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor;

	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_Print_Color == 0) color = RGB(0,0,0);
		else if(nEDA_Print_Color == 2) color = RGB(GetGray(m_nColor), GetGray(m_nColor), GetGray(m_nColor));
	}

	COLORREF oldtextcolor;
	oldtextcolor = pDC->SetTextColor(color);
		

	int bkmode = pDC->SetBkMode(TRANSPARENT);

	ShowSignalTrueTypeString(color, pDC, pV);

	pDC->SetTextColor(oldtextcolor);
	pDC->SetBkMode(bkmode);
}

CSize COb_schtext::GetSignalExtent(CDC* pDC, CView* pV)
{
	char str[1000];
	char original_str[1000];
	int upperline[1000];

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	CString text = m_csText;
	text.Left(990);
	strcpy(original_str, text);

	LOGFONT logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
	double height = (double)logfont.lfHeight;
	height *= fEDA_Font_Scale;
	logfont.lfOrientation = m_nOrientation;
	logfont.lfEscapement = logfont.lfOrientation*10;

	CFont font, *oldfont;
	if(pView->m_nPrint_Flag == 1)
	{
		logfont.lfHeight = (int)((height*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		logfont.lfWidth = 0;
	}
	else
	{
		logfont.lfHeight = (int)(height/pView->m_fScale);
		logfont.lfWidth = 0;
	}
	
	if(logfont.lfHeight <= 0) return 0;

	font.CreateFontIndirect(&logfont);

	oldfont = pDC->SelectObject(&font);

	int upperlinenum = 0;
	int num = 0;
	int i = 0;
	for(;;)
	{
		if(original_str[num] == '\0') 
		{
			str[i] = '\0';
			break;
		}
		else if(original_str[num] == 0x5c) 
		{
			if(i != 0)
			{
				upperline[upperlinenum] = i; 
				upperlinenum++;
			}
			num++;
		}
		else
		{
			str[i] = original_str[num];
			i++;
			num++;
		}
	}
			
	CSize size1;
	size1 = pDC->GetTextExtent(str, strlen(str));
			
	pDC->SelectObject(oldfont);

	return size1;
}

void COb_schtext::ShowSignalTrueTypeString(COLORREF color, CDC* pDC, CView* pV)
{
	int x,y;
	double tempfx, tempfy;
	int xx,yy;
	char str[1000];
	char original_str[1000];
	int upperline[1000];
	int width;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	CString text = m_csText;
	text.Left(990);
	strcpy(original_str, text);

	LOGFONT logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
	double height = (double)logfont.lfHeight;
	height *= fEDA_Font_Scale;
	logfont.lfOrientation = m_nOrientation;
	logfont.lfEscapement = logfont.lfOrientation*10;

	int offset = 0;
	if(height > 300) offset = (int)(height*0.025); 


	if(m_nOrientation == 90 )
	{
		tempfx = m_fSx - (height + offset);
		tempfy = m_fSy;
		pView->Get_VPort_XY(&x,&y,tempfx,tempfy);
	}
	else if(m_nOrientation == 180 )
	{
		tempfx = m_fSx;
		tempfy = m_fSy - (height + offset);
		pView->Get_VPort_XY(&x,&y,tempfx,tempfy);
	}
	else if(m_nOrientation == 270 )
	{
		tempfx = m_fSx + (height + offset);
		tempfy = m_fSy;
		pView->Get_VPort_XY(&x,&y,tempfx,tempfy);
	}
	else 
	{
		tempfx = m_fSx;
		tempfy = m_fSy + (height + offset);
		pView->Get_VPort_XY(&x,&y,tempfx,tempfy);
	}

	xx = x;
	yy = y;


	if(pView->m_nPrint_Flag == 1)
	{
		logfont.lfHeight = (int)((height*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		logfont.lfWidth = 0;
		xx = (int)(((double)xx*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		yy = (int)(((double)yy*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
	}
	else
	{
		logfont.lfHeight = (int)(height/pView->m_fScale);
		logfont.lfWidth = 0;
	}
	
	if(logfont.lfHeight <= 0) return;

	CFont font, *oldfont;
	font.CreateFontIndirect(&logfont);
	oldfont = pDC->SelectObject(&font);

	UINT flag = pDC->GetTextAlign();
	UINT h;
	if(m_nH_Justification == 0) h = TA_LEFT;
	else if(m_nH_Justification == 1) h = TA_CENTER;
	else if(m_nH_Justification == 2) h = TA_RIGHT;
	else h = TA_LEFT;

	pDC->SetTextAlign(h|flag);

	int upperlinenum = 0;
	int num = 0;
	int i = 0;
	for(;;)
	{
		if(original_str[num] == '\0') 
		{
			str[i] = '\0';
			break;
		}
		else if(original_str[num] == 0x5c) 
		{
			if(i != 0)
			{
				upperline[upperlinenum] = i;
				upperlinenum++;
			}
			num++;
		}
		else
		{
			str[i] = original_str[num];
			i++;
			num++;
		}
	}
			
	pDC->TextOut(xx, yy, str, strlen(str));
		
	double delta = 0;

	width = (int)(nEDA_Line_Width[LINE_SMALL]/pView->m_fScale);
	if(pView->m_nPrint_Flag == 1) width = (int)(width*nEDA_Print_Xdpi/1000*fEDA_Print_Scale);
	if(width < 1) width = 1;
	
	int xx1,yy1;
	char str1[MAX_STRING_LENGTH];
	CSize size1, size2, size;
	int start;
	for(i=0; i<upperlinenum; i++)
	{
		strcpy(str1, str);
		start = upperline[i];
		if(start <= 0) continue;
		str1[start] = '\0';
		size2 = pDC->GetTextExtent(str1, strlen(str1));
		
		if(str1[start-1] < 0) str1[start-2] = '\0';
		else str1[start-1] = '\0';

		size1 = pDC->GetTextExtent(str1, strlen(str1));
		size = pDC->GetTextExtent(str, strlen(str));
		if(m_nH_Justification == 1)
		{
			size.cx = size.cx/2;
			size1 = size1 - size;
			size2 = size2 - size;
		}
		else if(m_nH_Justification == 2)
		{
			size1 = size1 - size;
			size2 = size2 - size;
		}


		CPen pen, *oldpen;
		pen.CreatePen(PS_SOLID, width,color);
		oldpen = pDC->SelectObject(&pen);

		int dx, dy;
		if(m_nOrientation == 0)
		{
			xx1 = xx + size1.cx;
			yy1 = yy +  (int)(height*delta/pView->m_fScale);
			dx = xx + size2.cx;
			dy = yy1;
		}
		else if(m_nOrientation == 90)
		{
			xx1 = xx + (int)(height*delta/pView->m_fScale);
			yy1 = yy - size1.cx;
			dx = xx1;
			dy = yy - size2.cx;
		}
		else if(m_nOrientation == 180)
		{
			xx1 = xx - size1.cx;
			yy1 = yy - (int)(height*delta/pView->m_fScale);
			dx = xx - size2.cx;
			dy = yy1;
		}
		else if(m_nOrientation == 270)
		{
			xx1 = xx - (int)(height*delta/pView->m_fScale);
			yy1 = yy + size1.cx;
			dx = xx1;
			dy = yy + size2.cx;
		}

		pDC->MoveTo(xx1,yy1);
		pDC->LineTo(dx,dy);
		pDC->SelectObject(oldpen);
	}
	
	pDC->SelectObject(oldfont);
	pDC->SetTextAlign(flag);
}

BOOL COb_schtext::PickMe(CDC* pDC, CView* pV)
{
	int  wx1,wy1,wx2,wy2;
	double x1,y1,x2,y2;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(pView->m_nPrint_Flag == 1)
	{
		wx1 = (int)fEDA_Print_Minx;		wy1 = (int)fEDA_Print_Miny;
		wx2 = (int)fEDA_Print_Maxx;		wy2 = (int)fEDA_Print_Maxy;
	}
	else
	{
		wx1 = (int)pView->m_fScreenX; 
		wy1 = (int)pView->m_fScreenY;
		wx2 = (int)(pView->m_fScreenX + pView->m_fScale*pView->m_nViewport_Ex); 
		wy2 = (int)(pView->m_fScreenY + pView->m_fScale*pView->m_nViewport_Ey);

		wx1 -= 100;
		wy1 -= 100;
		wx2 += 100;
		wy2 += 100;
	}

	CSize size = GetExtent(pDC, pV);
	int delta = size.cx;

	LOGFONT logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
	double height = (double)logfont.lfHeight;
	height *= fEDA_Font_Scale;

	if(m_nOrientation == 90)
	{ 
		x1 = m_fSx - height; 
		y1 = m_fSy; 
		x2 = m_fSx; 
		y2 = m_fSy + delta*pView->m_fScale;  
	}
	else if(m_nOrientation == 180)
	{ 
		x1 = m_fSx - delta*pView->m_fScale; 
		y1 = m_fSy - height; 
		x2 = m_fSx; 
		y2 = m_fSy;  
	}
	else if(m_nOrientation == 270)
	{ 
		x1 = m_fSx; 
		y1 = m_fSy - delta*pView->m_fScale; 
		x2 = m_fSx + height; 
		y2 = m_fSy ;  
	}
	else
	{ 
		x1 = m_fSx;  
		y1 = m_fSy; 
		x2 = m_fSx + delta*pView->m_fScale;  
		y2 = m_fSy + height; 
	}

	if( ((x1 < wx1)&&(x2 < wx1)) || ((x1 > wx2)&&(x2 > wx2)) || ((y1 < wy1)&&(y2 < wy1)) || ((y1 > wy2)&&(y2 > wy2)) )	return FALSE;
	else return TRUE;

}

void COb_schtext::DrawTabbed(CDC* pDC, CView* pV)
{
	COLORREF color;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(m_nFlag == FLAG_NORMAL) color = m_nColor;
	else if(m_nFlag == FLAG_DELETED) color = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor;

	if(pView->m_nPrint_Flag != 0)
	{
		if(nEDA_Print_Color == 0) color = RGB(0,0,0);
		else if(nEDA_Print_Color == 2) color = RGB(GetGray(m_nColor), GetGray(m_nColor), GetGray(m_nColor));
	}

	COLORREF oldtextcolor;
	oldtextcolor = pDC->SetTextColor(color);
	::SetTextColor(pDC->m_hDC,color);
		
	int bkmode = pDC->SetBkMode(TRANSPARENT);

	ShowTrueTypeTabbedString(color, pDC, pV);

	pDC->SetTextColor(oldtextcolor);
	pDC->SetBkMode(bkmode);
}

void COb_schtext::ShowTrueTypeTabbedString(COLORREF color, CDC* pDC, CView* pV)
{
	int x,y;
	double tempfx, tempfy;
	int xx,yy;
	char str[1000];


	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	CString text = m_csText;
	text.Left(990);
	strcpy(str, text);

	LOGFONT logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
	double height = (double)logfont.lfHeight;
	height *= fEDA_Font_Scale;

	int offset = 0;
	if(height > 300) offset = (int)(height*0.025);


	if(m_nOrientation == 0)
	{
		tempfx = m_fSx;
		tempfy = m_fSy + (height + offset);
		//if(tempfy < 0) tempfy = 0;
		pView->Get_VPort_XY(&x,&y,tempfx,tempfy);
	}
	else if(m_nOrientation == 90 )
	{
		tempfx = m_fSx - (height + offset);
		//if(tempfx < 0) tempfx = 0;
		tempfy = m_fSy;
		pView->Get_VPort_XY(&x,&y,tempfx,tempfy);
	}
	else if(m_nOrientation == 180 )
	{
		tempfx = m_fSx;
		tempfy = m_fSy - (height + offset);
		pView->Get_VPort_XY(&x,&y,tempfx,tempfy);
	}
	else if(m_nOrientation == 270 )
	{
		tempfx = m_fSx + (height + offset);
		tempfy = m_fSy;
		pView->Get_VPort_XY(&x,&y,tempfx,tempfy);
	}

	xx = x;
	yy = y;


	CFont font, *oldfont;
	if(pView->m_nPrint_Flag == 1)
	{
		logfont.lfHeight = (int)((height*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		logfont.lfWidth = 0;
		xx = (int)(((double)xx*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
		yy = (int)(((double)yy*nEDA_Print_Ydpi)/1000*fEDA_Print_Scale);
		x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_Print_Scale);
	}
	else
	{
		logfont.lfHeight = (int)(height/pView->m_fScale);
		logfont.lfWidth = 0;
	}
	
	if(logfont.lfHeight <= 0) return;


	font.CreateFontIndirect(&logfont);

	oldfont = pDC->SelectObject(&font);


	pDC->TabbedTextOut(xx, yy, str, strlen(str), 0, NULL, x);
	
	pDC->SelectObject(oldfont);
}

int COb_schtext::GetTabbedExtent(CDC* pDC, CView* pV)
{
	char str[1000];

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	CString text = m_csText;
	text.Left(990);
	strcpy(str, text);

	LOGFONT logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
	double height = (double)logfont.lfHeight;
	height *= fEDA_Font_Scale;


	CFont font, *oldfont;

	logfont.lfHeight = (int)(height/pView->m_fScale);
	logfont.lfWidth = 0;
	
	
	if(logfont.lfHeight <= 0) return 0;

	
	font.CreateFontIndirect(&logfont);
	
	oldfont = pDC->SelectObject(&font);

			
	int pos[6];
	pos[0] = (int)((height/pView->m_fScale)*4*0.95);

	CSize size1;
	size1 = pDC->GetTabbedTextExtent(str, strlen(str), 1, pos);
	int string_extent = size1.cx;
			
	pDC->SelectObject(oldfont);

	return string_extent;
}

BOOL  COb_schtext::Is_On_Me(double fx, double fy, int diagram, CView* pV)
{
	//if(diagram != m_nDiagram) return FALSE;
	CClientDC dc(NULL);
	double fx1,fy1,fx2,fy2;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	CSize size = GetExtent(&dc, pV);
	int delta = size.cx;

	LOGFONT logfont = pDoc->m_arrayFont[diagram].GetAt(m_nFont);
	double height = (double)logfont.lfHeight;
	height *= fEDA_Font_Scale;

	if(m_nOrientation == 90)
	{ 
		fx1 = m_fSx - height; 
		fy1 = m_fSy; 
		fx2 = m_fSx; 
		fy2 = m_fSy + delta*pView->m_fScale;  
		
		if(m_nH_Justification == 1)
		{
			fy1 -= delta*pView->m_fScale/2;
			fy2 -= delta*pView->m_fScale/2;
		}
		else if(m_nH_Justification == 2)
		{
			fy1 -= delta*pView->m_fScale;
			fy2 -= delta*pView->m_fScale;
		}
		
		if(m_nV_Justification == 1)
		{
			fx1 += height/2;
			fx2 += height/2;
		}
		else if(m_nV_Justification == 2)
		{
			fx1 += height;
			fx2 += height;
		}
	}
	else if(m_nOrientation == 180)
	{ 
		fx1 = m_fSx - delta*pView->m_fScale; 
		fy1 = m_fSy - height; 
		fx2 = m_fSx; 
		fy2 = m_fSy;  
		
		if(m_nH_Justification == 1)
		{
			fx1 += delta*pView->m_fScale/2;
			fx2 += delta*pView->m_fScale/2;
		}
		else if(m_nH_Justification == 2)
		{
			fx1 += delta*pView->m_fScale;
			fx2 += delta*pView->m_fScale;
		}
		
		if(m_nV_Justification == 1)
		{
			fy1 += height/2;
			fy2 += height/2;
		}
		else if(m_nV_Justification == 2)
		{
			fy1 += height;
			fy2 += height;
		}
	}
	else if(m_nOrientation == 270)
	{ 
		fx1 = m_fSx; 
		fy1 = m_fSy - delta*pView->m_fScale; 
		fx2 = m_fSx + height; 
		fy2 = m_fSy ;  

		if(m_nH_Justification == 1)
		{
			fy1 += delta*pView->m_fScale/2;
			fy2 += delta*pView->m_fScale/2;
		}
		else if(m_nH_Justification == 2)
		{
			fy1 += delta*pView->m_fScale;
			fy2 += delta*pView->m_fScale;
		}
		
		if(m_nV_Justification == 1)
		{
			fx1 -= height/2;
			fx2 -= height/2;
		}
		else if(m_nV_Justification == 2)
		{
			fx1 -= height;
			fx2 -= height;
		}
	}
	else //if(m_nOrientation == 0)
	{ 
		fx1 = m_fSx;  
		fy1 = m_fSy; 
		fx2 = m_fSx + delta*pView->m_fScale;  
		fy2 = m_fSy + height; 
		
		if(m_nH_Justification == 1)
		{
			fx1 -= delta*pView->m_fScale/2;
			fx2 -= delta*pView->m_fScale/2;
		}
		else if(m_nH_Justification == 2)
		{
			fx1 -= delta*pView->m_fScale;
			fx2 -= delta*pView->m_fScale;
		}
		
		if(m_nV_Justification == 1)
		{
			fy1 -= height/2;
			fy2 -= height/2;
		}
		else if(m_nV_Justification == 2)
		{
			fy1 -= height;
			fy2 -= height;
		}
	}

	CRgn rgn;
	rgn.CreateRectRgn((int)fx1, (int)fy1, (int)fx2, (int)fy2);

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

BOOL  COb_schtext::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fx, fy;
	fx = m_fSx;
	fy = m_fSy;

	if((fx < fx1)||(fx > fx2)||(fy < fy1)||(fy > fy2)) return FALSE;
	return TRUE;
}

void COb_schtext::Change(POSITION pos)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(m_nID == TEXT_NETLABEL)
	{
		CDlg_EditNetlabel dlg;

		dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fSx);
		dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fSy);
		dlg.m_nColor = m_nColor;
		dlg.m_nComboOrientation = m_nOrientation/90;
		dlg.m_csEditNet = m_csText;
		
		LOGFONT lf;
		lf = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
		dlg.m_csFontName = lf.lfFaceName;

		char str[100]; itoa(lf.lfHeight, str, 10);
		dlg.m_csFontSize = str;

		if(lf.lfWeight == FW_BOLD)	dlg.m_bBold = TRUE;
		else dlg.m_bBold = FALSE;
		dlg.m_bItalic = lf.lfItalic;
		dlg.m_bUnderline = lf.lfUnderline;

		if(dlg.DoModal() == IDOK)
		{
			if(dlg.m_bCheckGlobal == TRUE)
			{
				CDlg_Global_Text subdlg;
				if(subdlg.DoModal() == IDOK)
				{
					if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
						
					if(subdlg.m_nRadioScope == 0)	
					{
						CObject* pobject;
						for(int nDiagram = 0; nDiagram < SCH_SHEET_NUM; nDiagram++)
						{
							int num = pDoc->m_listSchObject[nDiagram].GetCount();
							POSITION position, posbak;
							position = pDoc->m_listSchObject[nDiagram].GetHeadPosition();
							for(int i=0; i<num; i++)
							{	
								if(position == NULL) break;

								posbak = position;
								pobject = pDoc->m_listSchObject[nDiagram].GetNext(position);
									
								if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
								{
									COb_schtext* ptext = (COb_schtext*)pobject;
									if((ptext->m_nID != TEXT_NETLABEL)||(ptext->m_nDiagram != nDiagram)||(ptext->m_nFlag != FLAG_NORMAL))	continue;
									if(ptext == this) continue;

									if( (((subdlg.m_bCheckColor == TRUE)&&(ptext->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
										(((subdlg.m_bCheckOrientation == TRUE)&&(ptext->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
										(((subdlg.m_bCheckFont == TRUE)&&(ptext->m_nFont == m_nFont))||(subdlg.m_bCheckFont == FALSE)) )
									{
										if((ptext->m_nColor == m_nColor)||(ptext->m_nOrientation == m_nOrientation)||(ptext->m_nFont == m_nFont))
										{
											CSchUndo *pundo = new CSchUndo;
											pundo->m_nOp = UNDO_CHANGE;
											pundo->m_nOb = SCH_ELEMENT_TEXT;
											pundo->m_nDiagram = nDiagram;
											pundo->m_nPos = posbak;
											pundo->schtext.Copy(ptext);
											pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
										}
										if(ptext->m_nColor == m_nColor) ptext->m_nColor = dlg.m_nColor;
										if(ptext->m_nOrientation == m_nOrientation) ptext->m_nOrientation = dlg.m_nComboOrientation*90;
										if(ptext->m_nFont == m_nFont)
										{
											if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
											strcpy(lf.lfFaceName, dlg.m_csFontName);

											int index = dlg.m_csFontSize.Find('(');
											strcpy(str, dlg.m_csFontSize);
											if(index >= 0)	str[index] = '\0';
											lf.lfHeight = atoi(str);

											if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
											else lf.lfWeight = FW_NORMAL;
											lf.lfItalic = dlg.m_bItalic;
											lf.lfUnderline = dlg.m_bUnderline;

											int font_num = pDoc->CheckFontExist(&lf, nDiagram);
											if(font_num >= 0) ptext->m_nFont = font_num;
											else
											{
												ptext->m_nFont = pDoc->m_arrayFont[nDiagram].Add(lf);
											}
										}
									}
								}
								else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
								{
									COb_schsheetsymbol* psheetsymbol;
									psheetsymbol = (COb_schsheetsymbol*)pobject;
									for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[nDiagram].GetNext(position);
								}
								else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
								{
									COb_schcomp* pcomp;
									pcomp = (COb_schcomp*)pobject;
									for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[nDiagram].GetNext(position);
								}
							}
						}
					}
					else if(subdlg.m_nRadioScope == 1)	
					{
						CObject* pobject;
						int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
						POSITION position, posbak;
						position = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
						for(int i=0; i<num; i++)
						{	
							if(position == NULL) break;

							posbak = position;
							pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(position);
									
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
							{
								COb_schtext* ptext = (COb_schtext*)pobject;
								if((ptext->m_nID != TEXT_NETLABEL)||(ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag != FLAG_NORMAL))	continue;
								if(ptext == this) continue; 

								if( (((subdlg.m_bCheckColor == TRUE)&&(ptext->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckOrientation == TRUE)&&(ptext->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
									(((subdlg.m_bCheckFont == TRUE)&&(ptext->m_nFont == m_nFont))||(subdlg.m_bCheckFont == FALSE)) )
								{
									if((ptext->m_nColor == m_nColor)||(ptext->m_nOrientation == m_nOrientation)||(ptext->m_nFont == m_nFont))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_TEXT;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = posbak;
										pundo->schtext.Copy(ptext);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(ptext->m_nColor == m_nColor) ptext->m_nColor = dlg.m_nColor;
									if(ptext->m_nOrientation == m_nOrientation) ptext->m_nOrientation = dlg.m_nComboOrientation*90;
									if(ptext->m_nFont == m_nFont)
									{
										if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
										strcpy(lf.lfFaceName, dlg.m_csFontName);

										int index = dlg.m_csFontSize.Find('(');
										strcpy(str, dlg.m_csFontSize);
										if(index >= 0)	str[index] = '\0';
										lf.lfHeight = atoi(str);

										if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
										else lf.lfWeight = FW_NORMAL;
										lf.lfItalic = dlg.m_bItalic;
										lf.lfUnderline = dlg.m_bUnderline;

										int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
										if(font_num >= 0) ptext->m_nFont = font_num;
										else
										{
											ptext->m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
										}
									}
								}
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
							{
								COb_schsheetsymbol* psheetsymbol;
								psheetsymbol = (COb_schsheetsymbol*)pobject;
								for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
							{
								COb_schcomp* pcomp;
								pcomp = (COb_schcomp*)pobject;
								for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
							}
						}
					}
					else	
					{
						CObject* pobject;
						int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
						POSITION position, posbak;
						position = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
						for(int i=0; i<num; i++)
						{	
							if(position == NULL) break;

							posbak = position;
							pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(position);
									
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
							{
								COb_schtext* ptext = (COb_schtext*)pobject;
								if((ptext->m_bSelection != TRUE)||(ptext->m_nID != TEXT_NETLABEL)||(ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag != FLAG_NORMAL))	continue;
								if(ptext == this) continue; 

								if( (((subdlg.m_bCheckColor == TRUE)&&(ptext->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckOrientation == TRUE)&&(ptext->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
									(((subdlg.m_bCheckFont == TRUE)&&(ptext->m_nFont == m_nFont))||(subdlg.m_bCheckFont == FALSE)) )
								{
									if((ptext->m_nColor == m_nColor)||(ptext->m_nOrientation == m_nOrientation)||(ptext->m_nFont == m_nFont))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_TEXT;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = posbak;
										pundo->schtext.Copy(ptext);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(ptext->m_nColor == m_nColor) ptext->m_nColor = dlg.m_nColor;
									if(ptext->m_nOrientation == m_nOrientation) ptext->m_nOrientation = dlg.m_nComboOrientation*90;
									if(ptext->m_nFont == m_nFont)
									{
										if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
										strcpy(lf.lfFaceName, dlg.m_csFontName);

										int index = dlg.m_csFontSize.Find('(');
										strcpy(str, dlg.m_csFontSize);
										if(index >= 0)	str[index] = '\0';
										lf.lfHeight = atoi(str);

										if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
										else lf.lfWeight = FW_NORMAL;
										lf.lfItalic = dlg.m_bItalic;
										lf.lfUnderline = dlg.m_bUnderline;

										int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
										if(font_num >= 0) ptext->m_nFont = font_num;
										else
										{
											ptext->m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
										}
									}
								}
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
							{
								COb_schsheetsymbol* psheetsymbol;
								psheetsymbol = (COb_schsheetsymbol*)pobject;
								for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
							{
								COb_schcomp* pcomp;
								pcomp = (COb_schcomp*)pobject;
								//越过属于元件的元素，不单独处理
								for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
							}
						}
					}

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_TEXT;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = pos;
					pundo->schtext.Copy(this);
						
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					m_fSx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
					m_fSy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
					m_nColor = dlg.m_nColor;
					m_nOrientation = dlg.m_nComboOrientation*90;
					m_csText = dlg.m_csEditNet;
					
					if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
					strcpy(lf.lfFaceName, dlg.m_csFontName);

					int index = dlg.m_csFontSize.Find('(');
					strcpy(str, dlg.m_csFontSize);
					if(index >= 0)	str[index] = '\0';
					lf.lfHeight = atoi(str);

					if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
					else lf.lfWeight = FW_NORMAL;
					lf.lfItalic = dlg.m_bItalic;
					lf.lfUnderline = dlg.m_bUnderline;

					int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
					if(font_num >= 0) m_nFont = font_num;
					else
					{
						m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
					}

					pDoc->m_nSCH_Undo_Num++;
					if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				}
			}
			else
			{
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schtext.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

				m_fSx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
				m_fSy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
				m_nColor = dlg.m_nColor;
				m_nOrientation = dlg.m_nComboOrientation*90;
				m_csText = dlg.m_csEditNet;
				
				if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
				strcpy(lf.lfFaceName, dlg.m_csFontName);

				int index = dlg.m_csFontSize.Find('(');
				strcpy(str, dlg.m_csFontSize);
				if(index >= 0)	str[index] = '\0';
				lf.lfHeight = atoi(str);

				if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
				else lf.lfWeight = FW_NORMAL;
				lf.lfItalic = dlg.m_bItalic;
				lf.lfUnderline = dlg.m_bUnderline;

				int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
				if(font_num >= 0) m_nFont = font_num;
				else
				{
					m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
				}
			}
		}
	}
	else if(m_nID == TEXT_SHEETSYMBOLDESIGNATOR)
	{
		CDlg_EditSheetSymbolDF dlg;
		dlg.m_csTitle = "分页符号 符号名称";

		dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fSx);
		dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fSy);
		dlg.m_nColor = m_nColor;
		dlg.m_nComboOrientation = m_nOrientation/90;
		dlg.m_csEditDF = m_csText;

		dlg.m_bCheckAutoPosition = m_bAutoPosition;
		dlg.m_bCheckHide = m_bNameVisible;
		
		LOGFONT lf;
		lf = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
		dlg.m_csFontName = lf.lfFaceName;

		char str[100]; itoa(lf.lfHeight, str, 10);
		dlg.m_csFontSize = str;

		if(lf.lfWeight == FW_BOLD)	dlg.m_bBold = TRUE;
		else dlg.m_bBold = FALSE;
		dlg.m_bItalic = lf.lfItalic;
		dlg.m_bUnderline = lf.lfUnderline;

		if(dlg.DoModal() == IDOK)
		{
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = SCH_ELEMENT_TEXT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->schtext.Copy(this);
				
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

			m_fSx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
			m_fSy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
			m_nColor = dlg.m_nColor;
			m_nOrientation = dlg.m_nComboOrientation*90;
			m_csText = dlg.m_csEditDF;
		
			m_bAutoPosition = dlg.m_bCheckAutoPosition;
			m_bNameVisible = dlg.m_bCheckHide;

			if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
			strcpy(lf.lfFaceName, dlg.m_csFontName);

			int index = dlg.m_csFontSize.Find('(');
			strcpy(str, dlg.m_csFontSize);
			if(index >= 0)	str[index] = '\0';
			lf.lfHeight = atoi(str);

			if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
			else lf.lfWeight = FW_NORMAL;
			lf.lfItalic = dlg.m_bItalic;
			lf.lfUnderline = dlg.m_bUnderline;

			int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
			if(font_num >= 0) m_nFont = font_num;
			else
			{
				m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
			}
		}
	}
	else if(m_nID == TEXT_SHEETSYMBOLFILENAME)
	{
		CDlg_EditSheetSymbolDF dlg;
		dlg.m_csTitle = "分页符号 名称";
		
		dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fSx);
		dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fSy);
		dlg.m_nColor = m_nColor;
		dlg.m_nComboOrientation = m_nOrientation/90;
		dlg.m_csEditDF = m_csText;

		dlg.m_bCheckAutoPosition = m_bAutoPosition;
		dlg.m_bCheckHide = m_bNameVisible;
		
		LOGFONT lf;
		lf = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
		dlg.m_csFontName = lf.lfFaceName;

		char str[100]; itoa(lf.lfHeight, str, 10);
		dlg.m_csFontSize = str;

		if(lf.lfWeight == FW_BOLD)	dlg.m_bBold = TRUE;
		else dlg.m_bBold = FALSE;
		dlg.m_bItalic = lf.lfItalic;
		dlg.m_bUnderline = lf.lfUnderline;

		if(dlg.DoModal() == IDOK)
		{
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = SCH_ELEMENT_TEXT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->schtext.Copy(this);
				
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

			m_fSx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
			m_fSy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
			m_nColor = dlg.m_nColor;
			m_nOrientation = dlg.m_nComboOrientation*90;
			m_csText = dlg.m_csEditDF;
		
			m_bAutoPosition = dlg.m_bCheckAutoPosition;
			m_bNameVisible = dlg.m_bCheckHide;
			
			if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
			strcpy(lf.lfFaceName, dlg.m_csFontName);

			int index = dlg.m_csFontSize.Find('(');
			strcpy(str, dlg.m_csFontSize);
			if(index >= 0)	str[index] = '\0';
			lf.lfHeight = atoi(str);

			if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
			else lf.lfWeight = FW_NORMAL;
			lf.lfItalic = dlg.m_bItalic;
			lf.lfUnderline = dlg.m_bUnderline;

			int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
			if(font_num >= 0) m_nFont = font_num;
			else
			{
				m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
			}
		}		
	}
	else if(m_nID == TEXT_COMPDESIGNATOR)
	{
		CDlg_EditCompText dlg;
		dlg.m_csTitle = "元件名称";
		
		dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fSx);
		dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fSy);
		dlg.m_nColor = m_nColor;
		dlg.m_nComboOrientation = m_nOrientation/90;
		
		dlg.m_csEditText = m_csText;
		dlg.m_bCheckTextVisible = m_bTextVisible;
		dlg.m_bCheckTextLocked = m_bTextLocked;
		dlg.m_bCheckAutoPosition = m_bAutoPosition;

		dlg.m_csEditNameComment = m_csNameComment;
		dlg.m_bCheckNameVisible = m_bNameVisible;
		dlg.m_bCheckNameLocked = m_bNameLocked;

		dlg.m_nComboHJustification = m_nH_Justification;
		dlg.m_nComboVJustification = m_nV_Justification;

		LOGFONT lf;
		lf = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
		dlg.m_csFontName = lf.lfFaceName;

		char str[100]; itoa(lf.lfHeight, str, 10);
		dlg.m_csFontSize = str;

		if(lf.lfWeight == FW_BOLD)	dlg.m_bBold = TRUE;
		else dlg.m_bBold = FALSE;
		dlg.m_bItalic = lf.lfItalic;
		dlg.m_bUnderline = lf.lfUnderline;

		if(dlg.DoModal() == IDOK)
		{
			if(dlg.m_bCheckGlobal == TRUE)
			{
				CDlg_Global_Designator subdlg;
				if(subdlg.DoModal() == IDOK)
				{
					if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
						
					if(subdlg.m_nRadioScope == 0)	
					{
						CObject* pobject;
						for(int nDiagram = 0; nDiagram < SCH_SHEET_NUM; nDiagram++)
						{
							int num = pDoc->m_listSchObject[nDiagram].GetCount();
							POSITION position;
							position = pDoc->m_listSchObject[nDiagram].GetHeadPosition();
							for(int i=0; i<num; i++)
							{	
								if(position == NULL) break;
								pobject = pDoc->m_listSchObject[nDiagram].GetNext(position);

								if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
								{
									COb_schsheetsymbol* psheetsymbol;
									psheetsymbol = (COb_schsheetsymbol*)pobject;
									for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[nDiagram].GetNext(position);
								}
								else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
								{
									COb_schcomp* pcomp;
									pcomp = (COb_schcomp*)pobject;
									if((pcomp->m_nFlag != FLAG_NORMAL)||(pcomp->m_nDiagram != nDiagram))
									{
										for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[nDiagram].GetNext(position);

										continue;
									}

									for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
									{
										POSITION posbak = position;
										CObject* pobj;
										pobj = pDoc->m_listSchObject[nDiagram].GetNext(position);
										if(pobj->IsKindOf( RUNTIME_CLASS( COb_schtext )))
										{
											COb_schtext* ptext = (COb_schtext*)pobj;
											if(ptext->m_nID == TEXT_COMPDESIGNATOR)
											{
												if(ptext == this) continue; 
												
												if( (((subdlg.m_bCheckColor == TRUE)&&(ptext->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
													(((subdlg.m_bCheckOrientation == TRUE)&&(ptext->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
													(((subdlg.m_bCheckFont == TRUE)&&(ptext->m_nFont == m_nFont))||(subdlg.m_bCheckFont == FALSE)) &&
													(((subdlg.m_bCheckVisible == TRUE)&&(ptext->m_bTextVisible == m_bTextVisible))||(subdlg.m_bCheckVisible == FALSE)) &&
													(((subdlg.m_bCheckLock == TRUE)&&(ptext->m_bTextLocked == m_bTextLocked))||(subdlg.m_bCheckLock == FALSE)) &&
													(((subdlg.m_bCheckAuto == TRUE)&&(ptext->m_bAutoPosition == m_bAutoPosition))||(subdlg.m_bCheckAuto == FALSE)) )
												{
													if( (ptext->m_nColor == m_nColor)||(ptext->m_nOrientation == m_nOrientation)||(ptext->m_nFont == m_nFont)||\
														(ptext->m_bTextVisible == m_bTextVisible)||(ptext->m_bTextLocked == m_bTextLocked)||(ptext->m_bAutoPosition == m_bAutoPosition) )
													{
														CSchUndo *pundo = new CSchUndo;
														pundo->m_nOp = UNDO_CHANGE;
														pundo->m_nOb = SCH_ELEMENT_TEXT;
														pundo->m_nDiagram = nDiagram;
														pundo->m_nPos = posbak;
														pundo->schtext.Copy(ptext);
														pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
													}
													if(ptext->m_nColor == m_nColor) ptext->m_nColor = dlg.m_nColor;
													if(ptext->m_nOrientation == m_nOrientation) ptext->m_nOrientation = dlg.m_nComboOrientation*90;
													if(ptext->m_nFont == m_nFont)
													{
														if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
														strcpy(lf.lfFaceName, dlg.m_csFontName);

														int index = dlg.m_csFontSize.Find('(');
														strcpy(str, dlg.m_csFontSize);
														if(index >= 0)	str[index] = '\0';
														lf.lfHeight = atoi(str);

														if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
														else lf.lfWeight = FW_NORMAL;
														lf.lfItalic = dlg.m_bItalic;
														lf.lfUnderline = dlg.m_bUnderline;

														int font_num = pDoc->CheckFontExist(&lf, nDiagram);
														if(font_num >= 0) ptext->m_nFont = font_num;
														else
														{
															ptext->m_nFont = pDoc->m_arrayFont[nDiagram].Add(lf);
														}
													}
													if(ptext->m_bTextVisible == m_bTextVisible) ptext->m_bTextVisible = dlg.m_bCheckTextVisible;
													if(ptext->m_bTextLocked == m_bTextLocked) ptext->m_bTextLocked = dlg.m_bCheckTextLocked;
													if(ptext->m_bAutoPosition == m_bAutoPosition) ptext->m_bAutoPosition = dlg.m_bCheckAutoPosition;
												}
											}
										}
									}
								}
							}
						}
					}
					else if(subdlg.m_nRadioScope == 1)	
					{
						CObject* pobject;
						int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
						POSITION position;
						position = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
						for(int i=0; i<num; i++)
						{	
							if(position == NULL) break;
							pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(position);

							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
							{
								COb_schsheetsymbol* psheetsymbol;
								psheetsymbol = (COb_schsheetsymbol*)pobject;
								for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
							{
								COb_schcomp* pcomp;
								pcomp = (COb_schcomp*)pobject;
								if((pcomp->m_nFlag != FLAG_NORMAL)||(pcomp->m_nDiagram != m_nDiagram))
								{
									for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);

									continue;
								}

								for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
								{
									POSITION posbak = position;
									CObject* pobj;
									pobj = pDoc->m_listSchObject[m_nDiagram].GetNext(position);
									if(pobj->IsKindOf( RUNTIME_CLASS( COb_schtext )))
									{
										COb_schtext* ptext = (COb_schtext*)pobj;
										if(ptext->m_nID == TEXT_COMPDESIGNATOR) 
										{
											if(ptext == this) continue; 
												
											if( (((subdlg.m_bCheckColor == TRUE)&&(ptext->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
												(((subdlg.m_bCheckOrientation == TRUE)&&(ptext->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
												(((subdlg.m_bCheckFont == TRUE)&&(ptext->m_nFont == m_nFont))||(subdlg.m_bCheckFont == FALSE)) &&
												(((subdlg.m_bCheckVisible == TRUE)&&(ptext->m_bTextVisible == m_bTextVisible))||(subdlg.m_bCheckVisible == FALSE)) &&
												(((subdlg.m_bCheckLock == TRUE)&&(ptext->m_bTextLocked == m_bTextLocked))||(subdlg.m_bCheckLock == FALSE)) &&
												(((subdlg.m_bCheckAuto == TRUE)&&(ptext->m_bAutoPosition == m_bAutoPosition))||(subdlg.m_bCheckAuto == FALSE)) )
											{
												if( (ptext->m_nColor == m_nColor)||(ptext->m_nOrientation == m_nOrientation)||(ptext->m_nFont == m_nFont)||\
													(ptext->m_bTextVisible == m_bTextVisible)||(ptext->m_bTextLocked == m_bTextLocked)||(ptext->m_bAutoPosition == m_bAutoPosition) )
												{
													CSchUndo *pundo = new CSchUndo;
													pundo->m_nOp = UNDO_CHANGE;
													pundo->m_nOb = SCH_ELEMENT_TEXT;
													pundo->m_nDiagram = m_nDiagram;
													pundo->m_nPos = posbak;
													pundo->schtext.Copy(ptext);
													pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
												}
												if(ptext->m_nColor == m_nColor) ptext->m_nColor = dlg.m_nColor;
												if(ptext->m_nOrientation == m_nOrientation) ptext->m_nOrientation = dlg.m_nComboOrientation*90;
												if(ptext->m_nFont == m_nFont)
												{
													if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
													strcpy(lf.lfFaceName, dlg.m_csFontName);

													int index = dlg.m_csFontSize.Find('(');
													strcpy(str, dlg.m_csFontSize);
													if(index >= 0)	str[index] = '\0';
													lf.lfHeight = atoi(str);

													if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
													else lf.lfWeight = FW_NORMAL;
													lf.lfItalic = dlg.m_bItalic;
													lf.lfUnderline = dlg.m_bUnderline;

													int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
													if(font_num >= 0) ptext->m_nFont = font_num;
													else
													{
														ptext->m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
													}
												}
												if(ptext->m_bTextVisible == m_bTextVisible) ptext->m_bTextVisible = dlg.m_bCheckTextVisible;
												if(ptext->m_bTextLocked == m_bTextLocked) ptext->m_bTextLocked = dlg.m_bCheckTextLocked;
												if(ptext->m_bAutoPosition == m_bAutoPosition) ptext->m_bAutoPosition = dlg.m_bCheckAutoPosition;
											}
										}
									}
								}
							}
						}
					}
					else	
					{
						CObject* pobject;
						int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
						POSITION position;
						position = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
						for(int i=0; i<num; i++)
						{	
							if(position == NULL) break;
							pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(position);

							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
							{
								COb_schsheetsymbol* psheetsymbol;
								psheetsymbol = (COb_schsheetsymbol*)pobject;
								for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
							{
								COb_schcomp* pcomp;
								pcomp = (COb_schcomp*)pobject;
								if((pcomp->m_bSelection != TRUE)||(pcomp->m_nFlag != FLAG_NORMAL)||(pcomp->m_nDiagram != m_nDiagram))
								{
									for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);

									continue;
								}

								for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
								{
									POSITION posbak = position;
									CObject* pobj;
									pobj = pDoc->m_listSchObject[m_nDiagram].GetNext(position);
									if(pobj->IsKindOf( RUNTIME_CLASS( COb_schtext )))
									{
										COb_schtext* ptext = (COb_schtext*)pobj;
										if(ptext->m_nID == TEXT_COMPDESIGNATOR) 
										{
											if(ptext == this) continue; 
												
											if( (((subdlg.m_bCheckColor == TRUE)&&(ptext->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
												(((subdlg.m_bCheckOrientation == TRUE)&&(ptext->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
												(((subdlg.m_bCheckFont == TRUE)&&(ptext->m_nFont == m_nFont))||(subdlg.m_bCheckFont == FALSE)) &&
												(((subdlg.m_bCheckVisible == TRUE)&&(ptext->m_bTextVisible == m_bTextVisible))||(subdlg.m_bCheckVisible == FALSE)) &&
												(((subdlg.m_bCheckLock == TRUE)&&(ptext->m_bTextLocked == m_bTextLocked))||(subdlg.m_bCheckLock == FALSE)) &&
												(((subdlg.m_bCheckAuto == TRUE)&&(ptext->m_bAutoPosition == m_bAutoPosition))||(subdlg.m_bCheckAuto == FALSE)) )
											{
												if( (ptext->m_nColor == m_nColor)||(ptext->m_nOrientation == m_nOrientation)||(ptext->m_nFont == m_nFont)||\
													(ptext->m_bTextVisible == m_bTextVisible)||(ptext->m_bTextLocked == m_bTextLocked)||(ptext->m_bAutoPosition == m_bAutoPosition) )
												{
													CSchUndo *pundo = new CSchUndo;
													pundo->m_nOp = UNDO_CHANGE;
													pundo->m_nOb = SCH_ELEMENT_TEXT;
													pundo->m_nDiagram = m_nDiagram;
													pundo->m_nPos = posbak;
													pundo->schtext.Copy(ptext);
													pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
												}
												if(ptext->m_nColor == m_nColor) ptext->m_nColor = dlg.m_nColor;
												if(ptext->m_nOrientation == m_nOrientation) ptext->m_nOrientation = dlg.m_nComboOrientation*90;
												if(ptext->m_nFont == m_nFont)
												{
													if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
													strcpy(lf.lfFaceName, dlg.m_csFontName);

													int index = dlg.m_csFontSize.Find('(');
													strcpy(str, dlg.m_csFontSize);
													if(index >= 0)	str[index] = '\0';
													lf.lfHeight = atoi(str);

													if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
													else lf.lfWeight = FW_NORMAL;
													lf.lfItalic = dlg.m_bItalic;
													lf.lfUnderline = dlg.m_bUnderline;

													int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
													if(font_num >= 0) ptext->m_nFont = font_num;
													else
													{
														ptext->m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
													}
												}
												if(ptext->m_bTextVisible == m_bTextVisible) ptext->m_bTextVisible = dlg.m_bCheckTextVisible;
												if(ptext->m_bTextLocked == m_bTextLocked) ptext->m_bTextLocked = dlg.m_bCheckTextLocked;
												if(ptext->m_bAutoPosition == m_bAutoPosition) ptext->m_bAutoPosition = dlg.m_bCheckAutoPosition;
											}
										}
									}
								}
							}
						}
					}

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_TEXT;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = pos;
					pundo->schtext.Copy(this);
						
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					m_fSx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
					m_fSy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
					m_nColor = dlg.m_nColor;
					m_nOrientation = dlg.m_nComboOrientation*90;
					
					m_csText = dlg.m_csEditText;
					m_bTextVisible = dlg.m_bCheckTextVisible;
					m_bTextLocked = dlg.m_bCheckTextLocked;
					m_bAutoPosition = dlg.m_bCheckAutoPosition;

					m_csNameComment = dlg.m_csEditNameComment;
					m_bNameVisible = dlg.m_bCheckNameVisible;
					m_bNameLocked = dlg.m_bCheckNameLocked;	
				
					m_nH_Justification = dlg.m_nComboHJustification;
					m_nV_Justification = dlg.m_nComboVJustification;

					if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
					strcpy(lf.lfFaceName, dlg.m_csFontName);

					int index = dlg.m_csFontSize.Find('(');
					strcpy(str, dlg.m_csFontSize);
					if(index >= 0)	str[index] = '\0';
					lf.lfHeight = atoi(str);

					if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
					else lf.lfWeight = FW_NORMAL;
					lf.lfItalic = dlg.m_bItalic;
					lf.lfUnderline = dlg.m_bUnderline;

					int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
					if(font_num >= 0) m_nFont = font_num;
					else
					{
						m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
					}

					pDoc->m_nSCH_Undo_Num++;
					if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				}
			}
			else
			{
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schtext.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

				m_fSx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
				m_fSy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
				m_nColor = dlg.m_nColor;
				m_nOrientation = dlg.m_nComboOrientation*90;
				
				m_csText = dlg.m_csEditText;
				m_bTextVisible = dlg.m_bCheckTextVisible;
				m_bTextLocked = dlg.m_bCheckTextLocked;
				m_bAutoPosition = dlg.m_bCheckAutoPosition;

				m_csNameComment = dlg.m_csEditNameComment;
				m_bNameVisible = dlg.m_bCheckNameVisible;
				m_bNameLocked = dlg.m_bCheckNameLocked;	
			
				m_nH_Justification = dlg.m_nComboHJustification;
				m_nV_Justification = dlg.m_nComboVJustification;

				if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
				strcpy(lf.lfFaceName, dlg.m_csFontName);

				int index = dlg.m_csFontSize.Find('(');
				strcpy(str, dlg.m_csFontSize);
				if(index >= 0)	str[index] = '\0';
				lf.lfHeight = atoi(str);

				if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
				else lf.lfWeight = FW_NORMAL;
				lf.lfItalic = dlg.m_bItalic;
				lf.lfUnderline = dlg.m_bUnderline;

				int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
				if(font_num >= 0) m_nFont = font_num;
				else
				{
					m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
				}
			}
		}
	}
	else if(m_nID == TEXT_COMPCOMMENT)
	{
		CDlg_EditCompText dlg;
		dlg.m_csTitle = "元件标称值";
		
		dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fSx);
		dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fSy);
		dlg.m_nColor = m_nColor;
		dlg.m_nComboOrientation = m_nOrientation/90;
		
		dlg.m_csEditText = m_csText;
		dlg.m_bCheckTextVisible = m_bTextVisible;
		dlg.m_bCheckTextLocked = m_bTextLocked;
		dlg.m_bCheckAutoPosition = m_bAutoPosition;

		dlg.m_csEditNameComment = m_csNameComment;
		dlg.m_bCheckNameVisible = m_bNameVisible;
		dlg.m_bCheckNameLocked = m_bNameLocked;
		
		LOGFONT lf;
		lf = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
		dlg.m_csFontName = lf.lfFaceName;

		char str[100]; itoa(lf.lfHeight, str, 10);
		dlg.m_csFontSize = str;

		if(lf.lfWeight == FW_BOLD)	dlg.m_bBold = TRUE;
		else dlg.m_bBold = FALSE;
		dlg.m_bItalic = lf.lfItalic;
		dlg.m_bUnderline = lf.lfUnderline;

		if(dlg.DoModal() == IDOK)
		{
			if(dlg.m_bCheckGlobal == TRUE)
			{
				CDlg_Global_Designator subdlg;
				if(subdlg.DoModal() == IDOK)
				{
					if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
						
					if(subdlg.m_nRadioScope == 0)
					{
						CObject* pobject;
						for(int nDiagram = 0; nDiagram < SCH_SHEET_NUM; nDiagram++)
						{
							int num = pDoc->m_listSchObject[nDiagram].GetCount();
							POSITION position;
							position = pDoc->m_listSchObject[nDiagram].GetHeadPosition();
							for(int i=0; i<num; i++)
							{	
								if(position == NULL) break;
								pobject = pDoc->m_listSchObject[nDiagram].GetNext(position);

								if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
								{
									COb_schsheetsymbol* psheetsymbol;
									psheetsymbol = (COb_schsheetsymbol*)pobject;
									for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[nDiagram].GetNext(position);
								}
								else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
								{
									COb_schcomp* pcomp;
									pcomp = (COb_schcomp*)pobject;
									if((pcomp->m_nFlag != FLAG_NORMAL)||(pcomp->m_nDiagram != nDiagram))
									{
										for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[nDiagram].GetNext(position);

										continue;
									}

									for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
									{
										POSITION posbak = position;
										CObject* pobj;
										pobj = pDoc->m_listSchObject[nDiagram].GetNext(position);
										if(pobj->IsKindOf( RUNTIME_CLASS( COb_schtext )))
										{
											COb_schtext* ptext = (COb_schtext*)pobj;
											if(ptext->m_nID == TEXT_COMPCOMMENT)
											{
												if(ptext == this) continue; 
												
												if( (((subdlg.m_bCheckColor == TRUE)&&(ptext->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
													(((subdlg.m_bCheckOrientation == TRUE)&&(ptext->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
													(((subdlg.m_bCheckFont == TRUE)&&(ptext->m_nFont == m_nFont))||(subdlg.m_bCheckFont == FALSE)) &&
													(((subdlg.m_bCheckVisible == TRUE)&&(ptext->m_bTextVisible == m_bTextVisible))||(subdlg.m_bCheckVisible == FALSE)) &&
													(((subdlg.m_bCheckLock == TRUE)&&(ptext->m_bTextLocked == m_bTextLocked))||(subdlg.m_bCheckLock == FALSE)) &&
													(((subdlg.m_bCheckAuto == TRUE)&&(ptext->m_bAutoPosition == m_bAutoPosition))||(subdlg.m_bCheckAuto == FALSE)) )
												{
													if( (ptext->m_nColor == m_nColor)||(ptext->m_nOrientation == m_nOrientation)||(ptext->m_nFont == m_nFont)||\
														(ptext->m_bTextVisible == m_bTextVisible)||(ptext->m_bTextLocked == m_bTextLocked)||(ptext->m_bAutoPosition == m_bAutoPosition) )
													{
														CSchUndo *pundo = new CSchUndo;
														pundo->m_nOp = UNDO_CHANGE;
														pundo->m_nOb = SCH_ELEMENT_TEXT;
														pundo->m_nDiagram = nDiagram;
														pundo->m_nPos = posbak;
														pundo->schtext.Copy(ptext);
														pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
													}
													if(ptext->m_nColor == m_nColor) ptext->m_nColor = dlg.m_nColor;
													if(ptext->m_nOrientation == m_nOrientation) ptext->m_nOrientation = dlg.m_nComboOrientation*90;
													if(ptext->m_nFont == m_nFont)
													{
														if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
														strcpy(lf.lfFaceName, dlg.m_csFontName);

														int index = dlg.m_csFontSize.Find('(');
														strcpy(str, dlg.m_csFontSize);
														if(index >= 0)	str[index] = '\0';
														lf.lfHeight = atoi(str);

														if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
														else lf.lfWeight = FW_NORMAL;
														lf.lfItalic = dlg.m_bItalic;
														lf.lfUnderline = dlg.m_bUnderline;

														int font_num = pDoc->CheckFontExist(&lf, nDiagram);
														if(font_num >= 0) ptext->m_nFont = font_num;
														else
														{
															ptext->m_nFont = pDoc->m_arrayFont[nDiagram].Add(lf);
														}
													}
													if(ptext->m_bTextVisible == m_bTextVisible) ptext->m_bTextVisible = dlg.m_bCheckTextVisible;
													if(ptext->m_bTextLocked == m_bTextLocked) ptext->m_bTextLocked = dlg.m_bCheckTextLocked;
													if(ptext->m_bAutoPosition == m_bAutoPosition) ptext->m_bAutoPosition = dlg.m_bCheckAutoPosition;
												}
											}
										}
									}
								}
							}
						}
					}
					else if(subdlg.m_nRadioScope == 1)
					{
						CObject* pobject;
						int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
						POSITION position;
						position = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
						for(int i=0; i<num; i++)
						{	
							if(position == NULL) break;
							pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(position);

							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
							{
								COb_schsheetsymbol* psheetsymbol;
								psheetsymbol = (COb_schsheetsymbol*)pobject;
								for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
							{
								COb_schcomp* pcomp;
								pcomp = (COb_schcomp*)pobject;
								if((pcomp->m_nFlag != FLAG_NORMAL)||(pcomp->m_nDiagram != m_nDiagram))
								{
									for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);

									continue;
								}

								for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
								{
									POSITION posbak = position;
									CObject* pobj;
									pobj = pDoc->m_listSchObject[m_nDiagram].GetNext(position);
									if(pobj->IsKindOf( RUNTIME_CLASS( COb_schtext )))
									{
										COb_schtext* ptext = (COb_schtext*)pobj;
										if(ptext->m_nID == TEXT_COMPCOMMENT)
										{
											if(ptext == this) continue; 
												
											if( (((subdlg.m_bCheckColor == TRUE)&&(ptext->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
												(((subdlg.m_bCheckOrientation == TRUE)&&(ptext->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
												(((subdlg.m_bCheckFont == TRUE)&&(ptext->m_nFont == m_nFont))||(subdlg.m_bCheckFont == FALSE)) &&
												(((subdlg.m_bCheckVisible == TRUE)&&(ptext->m_bTextVisible == m_bTextVisible))||(subdlg.m_bCheckVisible == FALSE)) &&
												(((subdlg.m_bCheckLock == TRUE)&&(ptext->m_bTextLocked == m_bTextLocked))||(subdlg.m_bCheckLock == FALSE)) &&
												(((subdlg.m_bCheckAuto == TRUE)&&(ptext->m_bAutoPosition == m_bAutoPosition))||(subdlg.m_bCheckAuto == FALSE)) )
											{
												if( (ptext->m_nColor == m_nColor)||(ptext->m_nOrientation == m_nOrientation)||(ptext->m_nFont == m_nFont)||\
													(ptext->m_bTextVisible == m_bTextVisible)||(ptext->m_bTextLocked == m_bTextLocked)||(ptext->m_bAutoPosition == m_bAutoPosition) )
												{
													CSchUndo *pundo = new CSchUndo;
													pundo->m_nOp = UNDO_CHANGE;
													pundo->m_nOb = SCH_ELEMENT_TEXT;
													pundo->m_nDiagram = m_nDiagram;
													pundo->m_nPos = posbak;
													pundo->schtext.Copy(ptext);
													pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
												}
												if(ptext->m_nColor == m_nColor) ptext->m_nColor = dlg.m_nColor;
												if(ptext->m_nOrientation == m_nOrientation) ptext->m_nOrientation = dlg.m_nComboOrientation*90;
												if(ptext->m_nFont == m_nFont)
												{
													if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
													strcpy(lf.lfFaceName, dlg.m_csFontName);

													int index = dlg.m_csFontSize.Find('(');
													strcpy(str, dlg.m_csFontSize);
													if(index >= 0)	str[index] = '\0';
													lf.lfHeight = atoi(str);

													if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
													else lf.lfWeight = FW_NORMAL;
													lf.lfItalic = dlg.m_bItalic;
													lf.lfUnderline = dlg.m_bUnderline;

													int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
													if(font_num >= 0) ptext->m_nFont = font_num;
													else
													{
														ptext->m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
													}
												}
												if(ptext->m_bTextVisible == m_bTextVisible) ptext->m_bTextVisible = dlg.m_bCheckTextVisible;
												if(ptext->m_bTextLocked == m_bTextLocked) ptext->m_bTextLocked = dlg.m_bCheckTextLocked;
												if(ptext->m_bAutoPosition == m_bAutoPosition) ptext->m_bAutoPosition = dlg.m_bCheckAutoPosition;
											}
										}
									}
								}
							}
						}
					}
					else	
					{
						CObject* pobject;
						int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
						POSITION position;
						position = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
						for(int i=0; i<num; i++)
						{	
							if(position == NULL) break;
							pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(position);

							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
							{
								COb_schsheetsymbol* psheetsymbol;
								psheetsymbol = (COb_schsheetsymbol*)pobject;
								for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
							{
								COb_schcomp* pcomp;
								pcomp = (COb_schcomp*)pobject;
								if((pcomp->m_bSelection != TRUE)||(pcomp->m_nFlag != FLAG_NORMAL)||(pcomp->m_nDiagram != m_nDiagram))
								{
									for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);

									continue;
								}

								for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
								{
									POSITION posbak = position;
									CObject* pobj;
									pobj = pDoc->m_listSchObject[m_nDiagram].GetNext(position);
									if(pobj->IsKindOf( RUNTIME_CLASS( COb_schtext )))
									{
										COb_schtext* ptext = (COb_schtext*)pobj;
										if(ptext->m_nID == TEXT_COMPCOMMENT)
										{
											if(ptext == this) continue;
												
											if( (((subdlg.m_bCheckColor == TRUE)&&(ptext->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
												(((subdlg.m_bCheckOrientation == TRUE)&&(ptext->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
												(((subdlg.m_bCheckFont == TRUE)&&(ptext->m_nFont == m_nFont))||(subdlg.m_bCheckFont == FALSE)) &&
												(((subdlg.m_bCheckVisible == TRUE)&&(ptext->m_bTextVisible == m_bTextVisible))||(subdlg.m_bCheckVisible == FALSE)) &&
												(((subdlg.m_bCheckLock == TRUE)&&(ptext->m_bTextLocked == m_bTextLocked))||(subdlg.m_bCheckLock == FALSE)) &&
												(((subdlg.m_bCheckAuto == TRUE)&&(ptext->m_bAutoPosition == m_bAutoPosition))||(subdlg.m_bCheckAuto == FALSE)) )
											{
												if( (ptext->m_nColor == m_nColor)||(ptext->m_nOrientation == m_nOrientation)||(ptext->m_nFont == m_nFont)||\
													(ptext->m_bTextVisible == m_bTextVisible)||(ptext->m_bTextLocked == m_bTextLocked)||(ptext->m_bAutoPosition == m_bAutoPosition) )
												{
													CSchUndo *pundo = new CSchUndo;
													pundo->m_nOp = UNDO_CHANGE;
													pundo->m_nOb = SCH_ELEMENT_TEXT;
													pundo->m_nDiagram = m_nDiagram;
													pundo->m_nPos = posbak;
													pundo->schtext.Copy(ptext);
													pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

												}
												if(ptext->m_nColor == m_nColor) ptext->m_nColor = dlg.m_nColor;
												if(ptext->m_nOrientation == m_nOrientation) ptext->m_nOrientation = dlg.m_nComboOrientation*90;
												if(ptext->m_nFont == m_nFont)
												{
													if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
													strcpy(lf.lfFaceName, dlg.m_csFontName);

													int index = dlg.m_csFontSize.Find('(');
													strcpy(str, dlg.m_csFontSize);
													if(index >= 0)	str[index] = '\0';
													lf.lfHeight = atoi(str);

													if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
													else lf.lfWeight = FW_NORMAL;
													lf.lfItalic = dlg.m_bItalic;
													lf.lfUnderline = dlg.m_bUnderline;

													int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
													if(font_num >= 0) ptext->m_nFont = font_num;
													else
													{
														ptext->m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
													}
												}
												if(ptext->m_bTextVisible == m_bTextVisible) ptext->m_bTextVisible = dlg.m_bCheckTextVisible;
												if(ptext->m_bTextLocked == m_bTextLocked) ptext->m_bTextLocked = dlg.m_bCheckTextLocked;
												if(ptext->m_bAutoPosition == m_bAutoPosition) ptext->m_bAutoPosition = dlg.m_bCheckAutoPosition;
											}
										}
									}
								}
							}
						}
					}

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_TEXT;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = pos;
					pundo->schtext.Copy(this);
						
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					m_fSx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
					m_fSy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
					m_nColor = dlg.m_nColor;
					m_nOrientation = dlg.m_nComboOrientation*90;
					
					m_csText = dlg.m_csEditText;
					m_bTextVisible = dlg.m_bCheckTextVisible;
					m_bTextLocked = dlg.m_bCheckTextLocked;
					m_bAutoPosition = dlg.m_bCheckAutoPosition;

					m_csNameComment = dlg.m_csEditNameComment;
					m_bNameVisible = dlg.m_bCheckNameVisible;
					m_bNameLocked = dlg.m_bCheckNameLocked;	
				
					m_nH_Justification = dlg.m_nComboHJustification;
					m_nV_Justification = dlg.m_nComboVJustification;

					if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
					strcpy(lf.lfFaceName, dlg.m_csFontName);

					int index = dlg.m_csFontSize.Find('(');
					strcpy(str, dlg.m_csFontSize);
					if(index >= 0)	str[index] = '\0';
					lf.lfHeight = atoi(str);

					if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
					else lf.lfWeight = FW_NORMAL;
					lf.lfItalic = dlg.m_bItalic;
					lf.lfUnderline = dlg.m_bUnderline;

					int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
					if(font_num >= 0) m_nFont = font_num;
					else
					{
						m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
					}

					pDoc->m_nSCH_Undo_Num++;
					if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

				}
			}
			else
			{
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schtext.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

				m_fSx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
				m_fSy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
				m_nColor = dlg.m_nColor;
				m_nOrientation = dlg.m_nComboOrientation*90;
				
				m_csText = dlg.m_csEditText;
				m_bTextVisible = dlg.m_bCheckTextVisible;
				m_bTextLocked = dlg.m_bCheckTextLocked;
				m_bAutoPosition = dlg.m_bCheckAutoPosition;

				m_csNameComment = dlg.m_csEditNameComment;
				m_bNameVisible = dlg.m_bCheckNameVisible;
				m_bNameLocked = dlg.m_bCheckNameLocked;	

				if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
				strcpy(lf.lfFaceName, dlg.m_csFontName);

				int index = dlg.m_csFontSize.Find('(');
				strcpy(str, dlg.m_csFontSize);
				if(index >= 0)	str[index] = '\0';
				lf.lfHeight = atoi(str);

				if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
				else lf.lfWeight = FW_NORMAL;
				lf.lfItalic = dlg.m_bItalic;
				lf.lfUnderline = dlg.m_bUnderline;

				int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
				if(font_num >= 0) m_nFont = font_num;
				else
				{
					m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
				}
			}
		}
	}
	else
	{
		CDlg_EditText dlg;

		dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(m_fSx);
		dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(m_fSy);
		dlg.m_nColor = m_nColor;
		dlg.m_nComboOrientation = m_nOrientation/90;
		dlg.m_csEditText = m_csText;

		LOGFONT lf;
		lf = pDoc->m_arrayFont[m_nDiagram].GetAt(m_nFont);
		dlg.m_csFontName = lf.lfFaceName;

		char str[100]; itoa(lf.lfHeight, str, 10);
		dlg.m_csFontSize = str;

		if(lf.lfWeight == FW_BOLD)	dlg.m_bBold = TRUE;
		else dlg.m_bBold = FALSE;
		dlg.m_bItalic = lf.lfItalic;
		dlg.m_bUnderline = lf.lfUnderline;
		dlg.m_bCheckMirror = m_bMirror;
		dlg.m_nComboHJustification = m_nH_Justification;
		dlg.m_nComboVJustification = m_nV_Justification;

		if(dlg.DoModal() == IDOK)
		{
			if(dlg.m_bCheckGlobal == TRUE)
			{
				CDlg_Global_Text subdlg;
				if(subdlg.DoModal() == IDOK)
				{
					if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
						
					if(subdlg.m_nRadioScope == 0)	
					{
						CObject* pobject;
						for(int nDiagram = 0; nDiagram < SCH_SHEET_NUM; nDiagram++)
						{
							int num = pDoc->m_listSchObject[nDiagram].GetCount();
							POSITION position, posbak;
							position = pDoc->m_listSchObject[nDiagram].GetHeadPosition();
							for(int i=0; i<num; i++)
							{	
								if(position == NULL) break;

								posbak = position;
								pobject = pDoc->m_listSchObject[nDiagram].GetNext(position);
									
								if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
								{
									COb_schtext* ptext = (COb_schtext*)pobject;
									if((ptext->m_nID != TEXT_TEXT)||(ptext->m_nDiagram != nDiagram)||(ptext->m_nFlag != FLAG_NORMAL))	continue;
									if(ptext == this) continue; 

									if( (((subdlg.m_bCheckColor == TRUE)&&(ptext->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
										(((subdlg.m_bCheckOrientation == TRUE)&&(ptext->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
										(((subdlg.m_bCheckFont == TRUE)&&(ptext->m_nFont == m_nFont))||(subdlg.m_bCheckFont == FALSE)) )
									{
										if((ptext->m_nColor == m_nColor)||(ptext->m_nOrientation == m_nOrientation)||(ptext->m_nFont == m_nFont))
										{
											CSchUndo *pundo = new CSchUndo;
											pundo->m_nOp = UNDO_CHANGE;
											pundo->m_nOb = SCH_ELEMENT_TEXT;
											pundo->m_nDiagram = nDiagram;
											pundo->m_nPos = posbak;
											pundo->schtext.Copy(ptext);
											pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
										}
										if(ptext->m_nColor == m_nColor) ptext->m_nColor = dlg.m_nColor;
										if(ptext->m_nOrientation == m_nOrientation) ptext->m_nOrientation = dlg.m_nComboOrientation*90;
										if(ptext->m_nFont == m_nFont)
										{
											if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
											strcpy(lf.lfFaceName, dlg.m_csFontName);

											int index = dlg.m_csFontSize.Find('(');
											strcpy(str, dlg.m_csFontSize);
											if(index >= 0)	str[index] = '\0';
											lf.lfHeight = atoi(str);

											if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
											else lf.lfWeight = FW_NORMAL;
											lf.lfItalic = dlg.m_bItalic;
											lf.lfUnderline = dlg.m_bUnderline;

											int font_num = pDoc->CheckFontExist(&lf, nDiagram);
											if(font_num >= 0) ptext->m_nFont = font_num;
											else
											{
												ptext->m_nFont = pDoc->m_arrayFont[nDiagram].Add(lf);
											}
										}
									}
								}
								else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
								{
									COb_schsheetsymbol* psheetsymbol;
									psheetsymbol = (COb_schsheetsymbol*)pobject;
									for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[nDiagram].GetNext(position);
								}
								else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
								{
									COb_schcomp* pcomp;
									pcomp = (COb_schcomp*)pobject;
									for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[nDiagram].GetNext(position);
								}
							}
						}
					}
					else if(subdlg.m_nRadioScope == 1)
					{
						CObject* pobject;
						int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
						POSITION position, posbak;
						position = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
						for(int i=0; i<num; i++)
						{	
							if(position == NULL) break;

							posbak = position;
							pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(position);
									
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
							{
								COb_schtext* ptext = (COb_schtext*)pobject;
								if((ptext->m_nID != TEXT_TEXT)||(ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag != FLAG_NORMAL))	continue;
								if(ptext == this) continue; 

								if( (((subdlg.m_bCheckColor == TRUE)&&(ptext->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckOrientation == TRUE)&&(ptext->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
									(((subdlg.m_bCheckFont == TRUE)&&(ptext->m_nFont == m_nFont))||(subdlg.m_bCheckFont == FALSE)) )
								{
									if((ptext->m_nColor == m_nColor)||(ptext->m_nOrientation == m_nOrientation)||(ptext->m_nFont == m_nFont))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_TEXT;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = posbak;
										pundo->schtext.Copy(ptext);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(ptext->m_nColor == m_nColor) ptext->m_nColor = dlg.m_nColor;
									if(ptext->m_nOrientation == m_nOrientation) ptext->m_nOrientation = dlg.m_nComboOrientation*90;
									if(ptext->m_nFont == m_nFont)
									{
										if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
										strcpy(lf.lfFaceName, dlg.m_csFontName);

										int index = dlg.m_csFontSize.Find('(');
										strcpy(str, dlg.m_csFontSize);
										if(index >= 0)	str[index] = '\0';
										lf.lfHeight = atoi(str);

										if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
										else lf.lfWeight = FW_NORMAL;
										lf.lfItalic = dlg.m_bItalic;
										lf.lfUnderline = dlg.m_bUnderline;

										int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
										if(font_num >= 0) ptext->m_nFont = font_num;
										else
										{
											ptext->m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
										}
									}
								}
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
							{
								COb_schsheetsymbol* psheetsymbol;
								psheetsymbol = (COb_schsheetsymbol*)pobject;
								for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
							{
								COb_schcomp* pcomp;
								pcomp = (COb_schcomp*)pobject;
								for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
							}
						}
					}
					else	
					{
						CObject* pobject;
						int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
						POSITION position, posbak;
						position = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
						for(int i=0; i<num; i++)
						{	
							if(position == NULL) break;

							posbak = position;
							pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(position);
									
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
							{
								COb_schtext* ptext = (COb_schtext*)pobject;
								if((ptext->m_bSelection != TRUE)||(ptext->m_nID != TEXT_TEXT)||(ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag != FLAG_NORMAL))	continue;
								if(ptext == this) continue; 

								if( (((subdlg.m_bCheckColor == TRUE)&&(ptext->m_nColor == m_nColor))||(subdlg.m_bCheckColor == FALSE)) &&
									(((subdlg.m_bCheckOrientation == TRUE)&&(ptext->m_nOrientation == m_nOrientation))||(subdlg.m_bCheckOrientation == FALSE)) &&
									(((subdlg.m_bCheckFont == TRUE)&&(ptext->m_nFont == m_nFont))||(subdlg.m_bCheckFont == FALSE)) )
								{
									if((ptext->m_nColor == m_nColor)||(ptext->m_nOrientation == m_nOrientation)||(ptext->m_nFont == m_nFont))
									{
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_CHANGE;
										pundo->m_nOb = SCH_ELEMENT_TEXT;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = posbak;
										pundo->schtext.Copy(ptext);
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									}
									if(ptext->m_nColor == m_nColor) ptext->m_nColor = dlg.m_nColor;
									if(ptext->m_nOrientation == m_nOrientation) ptext->m_nOrientation = dlg.m_nComboOrientation*90;
									if(ptext->m_nFont == m_nFont)
									{
										if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
										strcpy(lf.lfFaceName, dlg.m_csFontName);

										int index = dlg.m_csFontSize.Find('(');
										strcpy(str, dlg.m_csFontSize);
										if(index >= 0)	str[index] = '\0';
										lf.lfHeight = atoi(str);

										if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
										else lf.lfWeight = FW_NORMAL;
										lf.lfItalic = dlg.m_bItalic;
										lf.lfUnderline = dlg.m_bUnderline;

										int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
										if(font_num >= 0) ptext->m_nFont = font_num;
										else
										{
											ptext->m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
										}
									}
								}
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
							{
								COb_schsheetsymbol* psheetsymbol;
								psheetsymbol = (COb_schsheetsymbol*)pobject;
								for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
							{
								COb_schcomp* pcomp;
								pcomp = (COb_schcomp*)pobject;
								for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(position);
							}
						}
					}

					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_TEXT;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = pos;
					pundo->schtext.Copy(this);
						
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

					m_fSx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
					m_fSy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
					m_nColor = dlg.m_nColor;
					m_nOrientation = dlg.m_nComboOrientation*90;
					m_csText = dlg.m_csEditText;
					m_nH_Justification = dlg.m_nComboHJustification;
					m_nV_Justification = dlg.m_nComboVJustification;

					if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
					strcpy(lf.lfFaceName, dlg.m_csFontName);

					int index = dlg.m_csFontSize.Find('(');
					strcpy(str, dlg.m_csFontSize);
					if(index >= 0)	str[index] = '\0';
					lf.lfHeight = atoi(str);

					if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
					else lf.lfWeight = FW_NORMAL;
					lf.lfItalic = dlg.m_bItalic;
					lf.lfUnderline = dlg.m_bUnderline;

					int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
					if(font_num >= 0) m_nFont = font_num;
					else
					{
						m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
					}
					
					if(m_bMirror != dlg.m_bCheckMirror)
					{
						m_bMirror = dlg.m_bCheckMirror;
						if((m_nOrientation == 0)||(m_nOrientation == 180))
						{
							if(m_nH_Justification == 0) m_nH_Justification = 2;
							else if(m_nH_Justification == 2) m_nH_Justification = 0;
						}
						else
						{
							if(m_nV_Justification == 0) m_nV_Justification = 2;
							else if(m_nV_Justification == 2) m_nV_Justification = 0;
						}
					}

					pDoc->m_nSCH_Undo_Num++;
					if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				}
			}
			else
			{
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = pos;
				pundo->schtext.Copy(this);
					
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

				m_fSx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
				m_fSy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
				m_nColor = dlg.m_nColor;
				m_nOrientation = dlg.m_nComboOrientation*90;
				m_csText = dlg.m_csEditText;
				m_nH_Justification = dlg.m_nComboHJustification;
				m_nV_Justification = dlg.m_nComboVJustification;

				if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
				strcpy(lf.lfFaceName, dlg.m_csFontName);

				int index = dlg.m_csFontSize.Find('(');
				strcpy(str, dlg.m_csFontSize);
				if(index >= 0)	str[index] = '\0';
				lf.lfHeight = atoi(str);

				if(dlg.m_bBold == TRUE) lf.lfWeight = FW_BOLD;
				else lf.lfWeight = FW_NORMAL;
				lf.lfItalic = dlg.m_bItalic;
				lf.lfUnderline = dlg.m_bUnderline;

				int font_num = pDoc->CheckFontExist(&lf, m_nDiagram);
				if(font_num >= 0) m_nFont = font_num;
				else
				{
					m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(lf);
				}
				
				if(m_bMirror != dlg.m_bCheckMirror)
				{
					m_bMirror = dlg.m_bCheckMirror;
					if((m_nOrientation == 0)||(m_nOrientation == 180))
					{
						if(m_nH_Justification == 0) m_nH_Justification = 2;
						else if(m_nH_Justification == 2) m_nH_Justification = 0;
					}
					else
					{
						if(m_nV_Justification == 0) m_nV_Justification = 2;
						else if(m_nV_Justification == 2) m_nV_Justification = 0;
					}
				}
			}
		}
	}
}

CRect COb_schtext::GetMoveTextRect(CDC* pDC, CView* pV)
{
	CESDApp* pApp = (CESDApp*)AfxGetApp();
	CESDView* pView = (CESDView*)pApp->GetCurrentView();
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	int x1,y1,x2,y2;
	
	CSize size;
	if(m_nID == TEXT_TEXT) size = GetExtent(pDC, pView);
	else size = GetSignalExtent(pDC, pView);

	if(m_nOrientation == 0)
	{ 
		pView->Get_VPort_XY(&x1,&y1,m_fSx,m_fSy);
		pView->Get_VPort_XY(&x2,&y2,0,m_fSy);
		x2 = x1 + size.cx;
		y2 = y2 - size.cy;
		if(m_nH_Justification == 1)
		{
			x1 -= size.cx/2;
			x2 -= size.cx/2;
		}
		else if(m_nH_Justification == 2)
		{
			x1 -= size.cx;
			x2 -= size.cx;
		}
		if(m_nV_Justification == 1)
		{
			y1 += size.cy/2;
			y2 += size.cy/2;
		}
		else if(m_nV_Justification == 2)
		{
			y1 += size.cy;
			y2 += size.cy;
		}
	}
	else if(m_nOrientation == 90)
	{ 
		pView->Get_VPort_XY(&x1,&y1,m_fSx,m_fSy);
		pView->Get_VPort_XY(&x2,&y2,m_fSx,0);
		y2 = y1 - size.cx;
		x2 = x2 - size.cy;
		if(m_nH_Justification == 1)
		{
			y1 += size.cx/2;
			y2 += size.cx/2;
		}
		else if(m_nH_Justification == 2)
		{
			y1 += size.cx;
			y2 += size.cx;
		}
		if(m_nV_Justification == 1)
		{
			x1 += size.cy/2;
			x2 += size.cy/2;
		}
		else if(m_nV_Justification == 2)
		{
			x1 += size.cy;
			x2 += size.cy;
		}
	}
	else if(m_nOrientation == 180)
	{ 
		pView->Get_VPort_XY(&x1,&y1,m_fSx,m_fSy);
		pView->Get_VPort_XY(&x2,&y2,0,m_fSy);
		x2 = x1 - size.cx;
		y2 = y2 + size.cy;
		if(m_nH_Justification == 1)
		{
			x1 += size.cx/2;
			x2 += size.cx/2;
		}
		else if(m_nH_Justification == 2)
		{
			x1 += size.cx;
			x2 += size.cx;
		}
		if(m_nV_Justification == 1)
		{
			y1 -= size.cy/2;
			y2 -= size.cy/2;
		}
		else if(m_nV_Justification == 2)
		{
			y1 -= size.cy;
			y2 -= size.cy;
		}
	}
	else if(m_nOrientation == 270)
	{ 
		pView->Get_VPort_XY(&x1,&y1,m_fSx,m_fSy);
		pView->Get_VPort_XY(&x2,&y2,m_fSx,0);
		y2 = y1 + size.cx;
		x2 = x2 + size.cy;
		if(m_nH_Justification == 1)
		{
			y1 -= size.cx/2;
			y2 -= size.cx/2;
		}
		else if(m_nH_Justification == 2)
		{
			y1 -= size.cx;
			y2 -= size.cx;
		}
		if(m_nV_Justification == 1)
		{
			x1 -= size.cy/2;
			x2 -= size.cy/2;
		}
		else if(m_nV_Justification == 2)
		{
			x1 -= size.cy;
			x2 -= size.cy;
		}
	}

	int xmin,ymin,xmax,ymax;
	xmin = MIN(x1,x2);
	xmax = MAX(x1,x2);
	ymin = MIN(y1,y2);
	ymax = MAX(y1,y2);

	//20%的冗余
	xmin -= (int)((xmax-xmin)*0.2);
	xmax += (int)((xmax-xmin)*0.2);
	ymin -= (int)((ymax-ymin)*0.2);
	ymax += (int)((ymax-ymin)*0.2);

	CRect rect(xmin, ymin, xmax, ymax);
	return rect;
}

void COb_schtext::Move(CClientDC* pDC, CView* pV, POSITION pos)
{
	int ch1;
	int flag;
	double x1,y1;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE; 
	CBitmap *pBitmap = NULL;	

	char szHelp[] = "移动文字: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	pView->Invalidate();

	COb_schtext atext;
	atext.pDocument = pDoc;
	atext.m_nID = m_nID;
	atext.m_fSx = m_fSx;
	atext.m_fSy = m_fSy;
	atext.m_nOrientation = m_nOrientation;
	atext.m_csText = m_csText;
	atext.m_nFont = m_nFont;
	atext.m_nColor = m_nColor;
	atext.m_bMirror = m_bMirror;
	atext.m_bAutoPosition = m_bAutoPosition;
	atext.m_nAnChorH = m_nAnChorH;
	atext.m_nAnChorV = m_nAnChorV;
	atext.m_nH_Justification = m_nH_Justification;
	atext.m_nV_Justification = m_nV_Justification;
	atext.m_nDiagram = m_nDiagram;

	CRect rect = atext.GetMoveTextRect(pDC, pV);
	pView->SaveAreaRectangleToBitmap(pDC, rect, &pBitmap);
	if(atext.m_nID == TEXT_NETLABEL) atext.DrawSignal(pDC, pV);
	else atext.Draw(pDC, pV);

	pView->m_fCurrentSnapX = atext.m_fSx;
	pView->m_fCurrentSnapY = atext.m_fSy;
	pView->Restore_LargeCrossXY();

	pView->OpenMouseCursor();

	if(m_nID == TEXT_NETLABEL)
	{
		bEDA_NeedElectricalGrid = TRUE;
	}

	pView->Write_Help(szHelp);
	flag = atext.m_nOrientation/90;
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			if(pBitmap != NULL)
			{
				pView->CopySavedAreaRectangleBack(pDC, pBitmap, rect);
				delete pBitmap;
				pBitmap = NULL;
			}
			pView->OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			if(pView->m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(pView->m_bAfterSetFocus)
			{
				pView->m_bAfterSetFocus = false;
				continue;
			}
			
			if(nEDA_RedrawFlag == 1)
			{ 
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				pView->CloseMouseCursor();
				if(pBitmap != NULL)
				{
					pView->CopySavedAreaRectangleBack(pDC, pBitmap, rect);
					delete pBitmap;
					pBitmap = NULL;
				}
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  y1 = pView->m_fCurrentSnapY;
			atext.m_fSx = x1; atext.m_fSy = y1;
			
			pView->CloseMouseCursor();
			rect = atext.GetMoveTextRect(pDC, pV);
			pView->SaveAreaRectangleToBitmap(pDC, rect, &pBitmap);

			if(atext.m_nID == TEXT_NETLABEL) atext.DrawSignal(pDC, pV);
			else atext.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			if(pBitmap != NULL)
			{
				pView->CopySavedAreaRectangleBack(pDC, pBitmap, rect);
				delete pBitmap;
				pBitmap = NULL;
			}
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  y1 = pView->m_fCurrentSnapY;
			atext.m_fSx = x1; atext.m_fSy = y1;
		
			//int font_index = pDoc->CheckFontExist(&cEDA_textfont, m_nDiagram);
			//if(font_index < 0) font_index = pDoc->m_arrayFont[m_nDiagram].Add(cEDA_textfont);

			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = SCH_ELEMENT_TEXT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pundo->schtext.Copy(this);
			pundo->schtext.m_nFlag = FLAG_NORMAL;

			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fSx = atext.m_fSx;
			m_fSy = atext.m_fSy;
			m_nOrientation = atext.m_nOrientation;
			m_bMirror = atext.m_bMirror;
			m_nH_Justification = atext.m_nH_Justification;
			m_nV_Justification = atext.m_nV_Justification;

			break;
		}
		else if((ch1==KEY_X)||(ch1==KEY_x))
		{  
			if(atext.m_bMirror == TRUE) atext.m_bMirror = FALSE;
			else atext.m_bMirror = TRUE;
			if((atext.m_nOrientation == 0)||(atext.m_nOrientation == 180))
			{
				if(atext.m_nH_Justification == 0) atext.m_nH_Justification = 2;
				else if(atext.m_nH_Justification == 2) atext.m_nH_Justification = 0;
			}
			else
			{
				if(atext.m_nV_Justification == 0) atext.m_nV_Justification = 2;
				else if(atext.m_nV_Justification == 2) atext.m_nV_Justification = 0;
			}
			
			pView->CloseMouseCursor();
			if(pBitmap != NULL)
			{
				pView->CopySavedAreaRectangleBack(pDC, pBitmap, rect);
				delete pBitmap;
				pBitmap = NULL;
			}

			rect = atext.GetMoveTextRect(pDC, pV);
			pView->SaveAreaRectangleToBitmap(pDC, rect, &pBitmap);

			if(atext.m_nID == TEXT_NETLABEL) atext.DrawSignal(pDC, pV);
			else atext.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if(ch1 == VK_SPACE)
		{ 
			flag++;
			if(flag>3) flag=0;
			
			pView->CloseMouseCursor();
			if(pBitmap != NULL)
			{
				pView->CopySavedAreaRectangleBack(pDC, pBitmap, rect);
				delete pBitmap;
				pBitmap = NULL;
			}
			
			atext.m_nOrientation = flag*90;


			rect = atext.GetMoveTextRect(pDC, pV);
			pView->SaveAreaRectangleToBitmap(pDC, rect, &pBitmap);

			if(atext.m_nID == TEXT_NETLABEL) atext.DrawSignal(pDC, pV);
			else atext.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pView->CloseMouseCursor();

	m_nFlag = FLAG_NORMAL;
	if((m_nID == TEXT_COMPDESIGNATOR)||(m_nID == TEXT_COMPCOMMENT))
	{
		COb_schcomp* pcomp = (COb_schcomp*)pDoc->m_listSchObject[m_nDiagram].GetAt(m_posParent);
		pcomp->Draw(pDC, pV);
	}
	else if(m_nID == TEXT_NETLABEL) DrawSignal(pDC, pV);
	else Draw(pDC, pV);

	if (pBitmap) delete pBitmap;

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
	
	if(m_nID == TEXT_NETLABEL)
	{
		bEDA_NeedElectricalGrid = FALSE;
		pView->m_fpHotPoint1.fx = pView->m_fpHotPoint1.fy = pView->m_fpHotPoint2.fx = pView->m_fpHotPoint2.fy = 0;
	}
	
	pView->Write_Help(szEDA_Help_Ready);
}

BOOL COb_schtext::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;
	double x1,y1;
	CBitmap *pBitmap = NULL;	

	char szHelp[] = "移动文字: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	pView->Invalidate();

	COb_schtext atext;
	atext.pDocument = pDoc;
	atext.m_nID = m_nID;
	atext.m_fSx = m_fSx;
	atext.m_fSy = m_fSy;
	atext.m_nOrientation = m_nOrientation;
	atext.m_csText = m_csText;
	atext.m_nFont = m_nFont;
	atext.m_nColor = m_nColor;
	atext.m_bMirror = m_bMirror;
	atext.m_bAutoPosition = m_bAutoPosition;
	atext.m_nAnChorH = m_nAnChorH;
	atext.m_nAnChorV = m_nAnChorV;
	atext.m_nH_Justification = m_nH_Justification;
	atext.m_nV_Justification = m_nV_Justification;
	atext.m_nDiagram = m_nDiagram;

	CRect rect = atext.GetMoveTextRect(pDC, pV);
	pView->SaveAreaRectangleToBitmap(pDC, rect, &pBitmap);
	if(atext.m_nID == TEXT_NETLABEL) atext.DrawSignal(pDC, pV);
	else atext.Draw(pDC, pV);
	pView->OpenMouseCursor();

	pView->Write_Help(szHelp);
	flag = atext.m_nOrientation/90;
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			if(pBitmap != NULL)
			{
				pView->CopySavedAreaRectangleBack(pDC, pBitmap, rect);
				delete pBitmap;
				pBitmap = NULL;
			}
			pView->OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			if(pView->m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(pView->m_bAfterSetFocus)
			{
				pView->m_bAfterSetFocus = false;
				continue;
			}
			
			
			if(nEDA_RedrawFlag == 1)
			{ 
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				pView->CloseMouseCursor();
				if(pBitmap != NULL)
				{
					pView->CopySavedAreaRectangleBack(pDC, pBitmap, rect);
					delete pBitmap;
					pBitmap = NULL;
				}
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  y1 = pView->m_fCurrentSnapY;
			atext.m_fSx = x1; atext.m_fSy = y1;
			
			pView->CloseMouseCursor();
			rect = atext.GetMoveTextRect(pDC, pV);
			pView->SaveAreaRectangleToBitmap(pDC, rect, &pBitmap);
			if(atext.m_nID == TEXT_NETLABEL) atext.DrawSignal(pDC, pV);
			else atext.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			if(pBitmap != NULL)
			{
				pView->CopySavedAreaRectangleBack(pDC, pBitmap, rect);
				delete pBitmap;
				pBitmap = NULL;
			}
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  y1 = pView->m_fCurrentSnapY;
			atext.m_fSx = x1; atext.m_fSy = y1;
		
			m_fSx = atext.m_fSx;
			m_fSy = atext.m_fSy;
			m_nOrientation = atext.m_nOrientation;
			m_bMirror = atext.m_bMirror;
			m_nH_Justification = atext.m_nH_Justification;
			m_nV_Justification = atext.m_nV_Justification;

			break;
		}
		else if((ch1==KEY_X)||(ch1==KEY_x))
		{  
			if(atext.m_bMirror == TRUE) atext.m_bMirror = FALSE;
			else atext.m_bMirror = TRUE;
			if((atext.m_nOrientation == 0)||(atext.m_nOrientation == 180))
			{
				if(atext.m_nH_Justification == 0) atext.m_nH_Justification = 2;
				else if(atext.m_nH_Justification == 2) atext.m_nH_Justification = 0;
			}
			else
			{
				if(atext.m_nV_Justification == 0) atext.m_nV_Justification = 2;
				else if(atext.m_nV_Justification == 2) atext.m_nV_Justification = 0;
			}
			
			pView->CloseMouseCursor();
			if(pBitmap != NULL)
			{
				pView->CopySavedAreaRectangleBack(pDC, pBitmap, rect);
				delete pBitmap;
				pBitmap = NULL;
			}

			rect = atext.GetMoveTextRect(pDC, pV);
			pView->SaveAreaRectangleToBitmap(pDC, rect, &pBitmap);

			if(atext.m_nID == TEXT_NETLABEL) atext.DrawSignal(pDC, pV);
			else atext.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if(ch1 == VK_SPACE)
		{ 
			flag++;
			if(flag>3) flag=0;
			

			pView->CloseMouseCursor();
			if(pBitmap != NULL)
			{
				pView->CopySavedAreaRectangleBack(pDC, pBitmap, rect);
				delete pBitmap;
				pBitmap = NULL;
			}
			
			atext.m_nOrientation = flag*90;

			rect = atext.GetMoveTextRect(pDC, pV);
			pView->SaveAreaRectangleToBitmap(pDC, rect, &pBitmap);

			if(atext.m_nID == TEXT_NETLABEL) atext.DrawSignal(pDC, pV);
			else atext.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pView->CloseMouseCursor();

	m_nFlag = FLAG_NORMAL;
	if((m_nID == TEXT_COMPDESIGNATOR)||(m_nID == TEXT_COMPCOMMENT))
	{
		COb_schcomp* pcomp = (COb_schcomp*)pDoc->m_listSchObject[m_nDiagram].GetAt(m_posParent);
		pcomp->Draw(pDC, pV);
	}
	else if(m_nID == TEXT_NETLABEL) DrawSignal(pDC, pV);
	else Draw(pDC, pV);

	if (pBitmap) delete pBitmap;

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	pView->Write_Help(szEDA_Help_Ready);
		
	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;
}

void COb_schtext::DrawSelection(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	CPen arcpen;
	arcpen.CreatePen(PS_DOT, 1, nEDA_SelectionColor);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	CRect crect;
	crect = GetMoveTextRect(pDC, pV);
	
	int x1,y1,x2,y2;
	x1 = crect.left;
	y1 = crect.top;
	x2 = crect.right;
	y2 = crect.bottom;

	double dx = (x2 - x1)/1.4;
	double dy = (y2 - y1)/1.4;

	x1 += (int)(dx*0.2);
	x2 -= (int)(dx*0.2);
	y1 += (int)(dy*0.2);
	y2 -= (int)(dy*0.2);

	pDC->MoveTo(x1, y1);
	pDC->LineTo(x1, y2);
	pDC->LineTo(x2, y2);
	pDC->LineTo(x2, y1);	
	pDC->LineTo(x1, y1);

	if((m_nID == TEXT_SHEETSYMBOLDESIGNATOR)||(m_nID == TEXT_SHEETSYMBOLFILENAME)||(m_nID == TEXT_SHEETSYMBOLTEXT))
	{
		CObject* pobject;
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_posParent);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;

			Fpoint fp[4];
			fp[0].fx = psheetsymbol->m_fX;
			fp[0].fy = psheetsymbol->m_fY;
			fp[1].fx = psheetsymbol->m_fX;
			fp[1].fy = psheetsymbol->m_fY - psheetsymbol->m_fYsize;
			fp[2].fx = psheetsymbol->m_fX + psheetsymbol->m_fXsize;
			fp[2].fy = psheetsymbol->m_fY - psheetsymbol->m_fYsize;
			fp[3].fx = psheetsymbol->m_fX + psheetsymbol->m_fXsize;
			fp[3].fy = psheetsymbol->m_fY;

			double dis = 10000000;
			int index;
			for(int m=0; m<4; m++)
			{
				double d = sqrt((m_fSx - fp[m].fx)*(m_fSx - fp[m].fx) + (m_fSy - fp[m].fy)*(m_fSy - fp[m].fy));
				if(d < dis)
				{
					dis = d;
					index = m;
				}
			}

			pView->Get_VPort_XY(&x1, &y1, fp[index].fx, fp[index].fy);
			pView->Get_VPort_XY(&x2, &y2, m_fSx, m_fSy);
			pDC->MoveTo(x1,y1);
			pDC->LineTo(x2,y2);
	
		}
	}
	else if((m_nID == TEXT_COMPDESIGNATOR)||(m_nID == TEXT_COMPCOMMENT)||(m_nID == TEXT_COMPPACKAGE)||(m_nID == TEXT_COMPTEXT))
	{
		CObject* pobject;
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(m_posParent);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;

			RECT rect = pcomp->GetCompRect();
			Fpoint fp[4];
			fp[0].fx = (double)rect.left;
			fp[0].fy = (double)rect.bottom;
			fp[1].fx = (double)rect.left;
			fp[1].fy = (double)rect.top;
			fp[2].fx = (double)rect.right;
			fp[2].fy = (double)rect.top;
			fp[3].fx = (double)rect.right;
			fp[3].fy = (double)rect.bottom;
				
			double dis = 10000000;
			int index = 0;
			for(int m=0; m<4; m++)
			{
				double d = sqrt((m_fSx - fp[m].fx)*(m_fSx - fp[m].fx) + (m_fSy - fp[m].fy)*(m_fSy - fp[m].fy));
				if(d < dis)
				{
					dis = d;
					index = m;
				}
			}

			pView->Get_VPort_XY(&x1, &y1, fp[index].fx, fp[index].fy);
			pView->Get_VPort_XY(&x2, &y2, m_fSx, m_fSy);
			pDC->MoveTo(x1,y1);
			pDC->LineTo(x2,y2);
		}
	}

	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}

void COb_schtext::Copy(COb_schtext* pschtext)
{
	m_nID = pschtext->m_nID;

	m_fSx = pschtext->m_fSx;
	m_fSy = pschtext->m_fSy;
	m_nOrientation = pschtext->m_nOrientation;

	m_csText = pschtext->m_csText;
	m_bTextVisible = pschtext->m_bTextVisible;
	m_bTextLocked = pschtext->m_bTextLocked;

	m_csNameComment = pschtext->m_csNameComment;
	m_bNameVisible = pschtext->m_bNameVisible;
	m_bNameLocked = pschtext->m_bNameLocked;

	m_nFont = pschtext->m_nFont;

	m_nColor = pschtext->m_nColor;
	
	m_bMirror = pschtext->m_bMirror;
	m_bAutoPosition = pschtext->m_bAutoPosition;
	m_nParamType = pschtext->m_nParamType;
	m_csUniqueID = pschtext->m_csUniqueID;
	m_bAllowDataBaseSynchronize = pschtext->m_bAllowDataBaseSynchronize;
	m_bAllowLibrarySynchronize = pschtext->m_bAllowLibrarySynchronize;

	m_nH_Justification = pschtext->m_nH_Justification;
	m_nV_Justification = pschtext->m_nV_Justification;

	m_nAnChorH = pschtext->m_nAnChorH;
	m_nAnChorV = pschtext->m_nAnChorV;

	m_nOwnerPartID = pschtext->m_nOwnerPartID;
	m_nDisplayMode = pschtext->m_nDisplayMode;
	m_posParent = pschtext->m_posParent;

	m_nDiagram = pschtext->m_nDiagram;
	m_bSelection = pschtext->m_bSelection;
	m_nFlag = pschtext->m_nFlag;

	pDocument = pschtext->pDocument;
}