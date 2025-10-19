#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_PCBEditText.h"
//#include "D:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\PlatformSDK\Include\gdiplus.h"
//#include "gdiplus.h"

#define Default_Offset_Height 50

IMPLEMENT_SERIAL(COb_pcbtext, CObject, 0)
COb_pcbtext::COb_pcbtext()
{
	m_nDescent = m_nAscent = m_nInternalLeading = m_nLeft = m_nRight = m_nTop = m_nBottom = -1;

	m_fX1 = m_fX2 = m_fY1 = m_fY2 = -1.0f;
	m_csText.Empty();
	m_bMirror = FALSE;
	m_bLocked = FALSE;	

	m_bDesignator = FALSE;
	m_bComment = FALSE;

	m_nStrokeFont = 0;
	m_bUseTTFont = FALSE;
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_bInverted = FALSE;
	m_csTTFontName = "Arial";
	
	m_bPolygonOutline = FALSE;
	m_bUserRouted = TRUE;
	m_nUnionIndex = 0;
	m_fInvertedBorder = 20;
	
	m_nFlag = 0;
	m_bSelection = FALSE;

	m_nComponent = -1;
}



void COb_pcbtext::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fSx;
		ar << m_fSy;
		ar << m_fHeight;
		ar << m_fWidth;
		ar << m_fRotation;

		ar << m_nLeft;
		ar << m_nRight;
		ar << m_nTop;
		ar << m_nBottom;
		ar << m_nAscent;
		ar << m_nDescent;
		ar << m_nInternalLeading;

		ar << m_fX1;
		ar << m_fY1;
		ar << m_fX2;
		ar << m_fY2;
		ar << m_nLayer;

		ar << m_csText;
		ar << m_bMirror;
		ar << m_bLocked;

		ar << m_nComponent;
		ar << m_bDesignator;
		ar << m_bComment;

		ar << m_nStrokeFont;
		ar << m_bUseTTFont;
		ar << m_bBold;
		ar << m_bItalic;
		ar << m_bInverted;
		ar << m_csTTFontName;
		ar << m_fInvertedBorder;	

		ar << m_bPolygonOutline;
		ar << m_bUserRouted;
		ar << m_nUnionIndex;

		ar << m_bSelection;	
	}
	else
	{
		ar >> m_fSx;
		ar >> m_fSy;
		ar >> m_fHeight;
		ar >> m_fWidth;
		ar >> m_fRotation;

		ar >> m_nLeft;
		ar >> m_nRight;
		ar >> m_nTop;
		ar >> m_nBottom;
		ar >> m_nAscent;
		ar >> m_nDescent;
		ar >> m_nInternalLeading;

		ar >> m_fX1;
		ar >> m_fY1;
		ar >> m_fX2;
		ar >> m_fY2;
		ar >> m_nLayer;

		ar >> m_csText;
		ar >> m_bMirror;
		ar >> m_bLocked;

		ar >> m_nComponent;
		ar >> m_bDesignator;
		ar >> m_bComment;

		ar >> m_nStrokeFont;
		ar >> m_bUseTTFont;
		ar >> m_bBold;
		ar >> m_bItalic;
		ar >> m_bInverted;
		ar >> m_csTTFontName;
		ar >> m_fInvertedBorder;	

		ar >> m_bPolygonOutline;
		ar >> m_bUserRouted;
		ar >> m_nUnionIndex;

		ar >> m_bSelection;	
	}
}

void COb_pcbtext::Draw(CDC* pDC, CView* pV)
{
	COLORREF color;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;

	if((m_fX1 == -1.0f)&&(m_fX2 == -1.0f)&&(m_fY1 == -1.0f)&&(m_fY2 == -1.0f)) UpdateStringBorder(pV, TRUE);
	
	if(pView->m_nPrint_Flag == 1)
	{
		if(nEDA_PCBPrint_Color == 0) color = RGB(0,0,0);
		else if(nEDA_PCBPrint_Color == 2) color = RGB(GetGray( nEDA_PCB_LayerColor[m_nLayer]), GetGray( nEDA_PCB_LayerColor[m_nLayer]), GetGray( nEDA_PCB_LayerColor[m_nLayer]));
		else color = nEDA_PCB_LayerColor[m_nLayer]; 
	}
	else
	{
		if(m_nFlag == FLAG_DELETED) color = nEDA_PCB_BoardAreaColor;
		else color = nEDA_PCB_LayerColor[m_nLayer];
	}	
	
	COLORREF oldtextcolor;
	oldtextcolor = pDC->SetTextColor(color);

	int bkmode = pDC->SetBkMode(TRANSPARENT);

	if(m_bUseTTFont == TRUE)
	{
		if(m_bMirror == FALSE)	ShowTTString(color, pDC, pV);
		else ShowMirrorTTString(color, pDC, pV);
	}
	else ShowStrokeString(color, pDC, pV);

	pDC->SetTextColor(oldtextcolor);
	pDC->SetBkMode(bkmode);
}

void COb_pcbtext::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	COLORREF color = nEDA_PCB_LayerColor[m_nLayer];

	COLORREF oldtextcolor;
	oldtextcolor = pDC->SetTextColor(color);

	int bkmode = pDC->SetBkMode(TRANSPARENT);


	char original_str[1000];

	CString text = m_csText;
	text.Left(990);
	strcpy(original_str, text);

	int width;
	width = (int)(m_fWidth/scale);
	if(width < 1) width = 1;
	
	CPen pen, *oldpen;
	pen.CreatePen(PS_SOLID, width, color);
	oldpen = pDC->SelectObject(&pen);

	double fx, fy;
	fx = m_fSx;
	fy = m_fSy;
	for(int i=0; i<(int)strlen(original_str); i++)
	{
		int x,y; 

		x = (int)((fx - origin_x)/scale);
		y = nEy - (int)((fy - origin_y)/scale);	

		unsigned char c = original_str[i];
		c -= 32;
		if( c > 95 ) c = 95;


		double yoffset, w;
		int number;
		if(m_nStrokeFont == 0)
		{
			yoffset = (double)nEDA_DeafultFont[c].yoffset;
			w = (double)nEDA_DeafultFont[c].w;
			number = nEDA_DeafultFont[c].number;
		}
		else if(m_nStrokeFont == 1)
		{
			yoffset = (double)nEDA_SansSerifFont[c].yoffset;
			w = (double)nEDA_SansSerifFont[c].w;
			number = nEDA_SansSerifFont[c].number;
		}
		else if(m_nStrokeFont == 2)
		{
			yoffset = (double)nEDA_SerifFont[c].yoffset;
			w = (double)nEDA_SerifFont[c].w;
			number = nEDA_SerifFont[c].number;		
		}

		w = w*m_fHeight/100.0f;

		for(int j=0; j<number; j++)
		{
			double data_fx, data_fy;
			int sx, sy;
			int pu_pd;
			if(m_nStrokeFont == 0)
			{
				pu_pd = nEDA_DeafultFont[c].pu_pd[j][0];
				data_fx = (double)nEDA_DeafultFont[c].pu_pd[j][1];
				data_fy = (double)nEDA_DeafultFont[c].pu_pd[j][2];
			}
			else if(m_nStrokeFont == 1)
			{
				pu_pd = nEDA_SansSerifFont[c].pu_pd[j][0];
				data_fx = (double)nEDA_SansSerifFont[c].pu_pd[j][1];
				data_fy = (double)nEDA_SansSerifFont[c].pu_pd[j][2];
			}
			else if(m_nStrokeFont == 2)
			{
				pu_pd = nEDA_SerifFont[c].pu_pd[j][0];
				data_fx = (double)nEDA_SerifFont[c].pu_pd[j][1];
				data_fy = (double)nEDA_SerifFont[c].pu_pd[j][2];
			}

			data_fy = data_fy*0.98f - yoffset;

			data_fx = data_fx*m_fHeight/100;
			data_fy = data_fy*m_fHeight/100;

			data_fx = data_fx/scale;
			data_fy = data_fy/scale;
		
			if(m_bMirror == FALSE)
			{
				if(m_fRotation == 0) 
				{
					sx = x + (int)data_fx;
					sy = y - (int)data_fy;
				}
				else if(m_fRotation == 90.0f)
				{
					sx = x - (int)data_fy;
					sy = y - (int)data_fx;
				}
				else if(m_fRotation == 180.0f)
				{
					sx = x - (int)data_fx;
					sy = y + (int)data_fy;
				}
				else if(m_fRotation == 270.0f)
				{
					sx = x + (int)data_fy;
					sy = y + (int)data_fx;
				}
				else
				{
					double temp_fx, temp_fy;
					GetRotationPoint(&temp_fx, &temp_fy, data_fx, data_fy);
					sx = x + (int)temp_fx;
					sy = y - (int)temp_fy;
				}
			}
			else
			{
				if(m_fRotation == 0) 
				{
					sx = x - (int)data_fx;
					sy = y - (int)data_fy;
				}
				else if(m_fRotation == 90.0f)
				{
					sx = x - (int)data_fy;
					sy = y + (int)data_fx;
				}
				else if(m_fRotation == 180.0f)
				{
					sx = x + (int)data_fx;
					sy = y + (int)data_fy;
				}
				else if(m_fRotation == 270.0f)
				{
					sx = x + (int)data_fy;
					sy = y - (int)data_fx;
				}
				else
				{
					double temp_fx, temp_fy;
					GetMirrorRotationPoint(&temp_fx, &temp_fy, data_fx, data_fy);
					sx = x + (int)temp_fx;
					sy = y - (int)temp_fy;
				}
			}

			if(pu_pd == 0) pDC->MoveTo(sx, sy);
			else pDC->LineTo(sx, sy);

		}

		if(m_bMirror == FALSE)
		{
			if(m_fRotation == 0) fx += w;
			else if(m_fRotation == 90.0f) fy += w;
			else if(m_fRotation == 180.0f) fx -= w;
			else if(m_fRotation == 270.0f) fy -= w;
			else
			{
				fx += w*cos(m_fRotation*fEDA_pi/180);
				fy += w*sin(m_fRotation*fEDA_pi/180);
			}
		}
		else
		{
			if(m_fRotation == 0) fx -= w;
			else if(m_fRotation == 90.0f) fy -= w;
			else if(m_fRotation == 180.0f) fx += w;
			else if(m_fRotation == 270.0f) fy += w;
			else
			{
				fx -= w*cos(m_fRotation*fEDA_pi/180);
				fy -= w*sin(m_fRotation*fEDA_pi/180);
			}
		}
	}

	pDC->SelectObject(oldpen);

	pDC->SetTextColor(oldtextcolor);
	pDC->SetBkMode(bkmode);
}

void COb_pcbtext::DrawSelection(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
		
	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;
	
	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	CPen arcpen;
	arcpen.CreatePen(PS_SOLID, 1, nEDA_PCB_LayerColor[SELECTIONS]);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;

	GetTextCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);

	POINT point[5];
	int x, y;
	pView->Get_VPort_XY(&x, &y, fx1, fy1);
	point[0].x = x; point[0].y = y;
	pView->Get_VPort_XY(&x, &y, fx4, fy4);
	point[1].x = x; point[1].y = y;
	pView->Get_VPort_XY(&x, &y, fx2, fy2);
	point[2].x = x; point[2].y = y;
	pView->Get_VPort_XY(&x, &y, fx3, fy3);
	point[3].x = x; point[3].y = y;
	pView->Get_VPort_XY(&x, &y, fx1, fy1);
	point[4].x = x; point[4].y = y;

	// Use the bit pattern to create a bitmap.
	WORD HatchBits[8] = { 0x55, 0xaa, 0x55, 0xaa, 0x55,  0xaa, 0x55, 0xaa };
	CBitmap bm;
	bm.CreateBitmap(8,8,1,1, HatchBits);
	// Create a pattern brush from the bitmap.	

	CBrush fillbrush;
	fillbrush.CreatePatternBrush(&bm);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	int mode = pDC->SetROP2(R2_MERGEPEN );
	pDC->Polygon(point, 5);
	pDC->SetROP2(mode);
	pDC->SelectObject(oldbrush);

	pView->Get_VPort_XY(&x, &y, m_fSx, m_fSy);
	pDC->MoveTo(x-5,y-5);
	pDC->LineTo(x+5,y+5);
	pDC->MoveTo(x-5,y+5);
	pDC->LineTo(x+5,y-5);

	double r = abs(m_fX2 - m_fX1);
	double rx, ry;
	if(m_bMirror == FALSE)
	{
		rx = m_fSx + r*cos(m_fRotation*fEDA_pi/180);
		ry = m_fSy + r*sin(m_fRotation*fEDA_pi/180);
	}
	else
	{
		rx = m_fSx - r*cos(m_fRotation*fEDA_pi/180);
		ry = m_fSy - r*sin(m_fRotation*fEDA_pi/180);
	}

	pView->Get_VPort_XY(&x, &y, rx, ry);
	pView->DrawSelectionKeyPoint(x, y, pDC);

	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}

void COb_pcbtext::DrawNotEditSelection(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	CPen arcpen;
	arcpen.CreatePen(PS_SOLID, 1, nEDA_PCB_LayerColor[SELECTIONS]);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;

	GetTextCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);

	int x, y;
	pView->Get_VPort_XY(&x, &y, fx1, fy1);
	pDC->MoveTo(x, y);
	pView->Get_VPort_XY(&x, &y, fx4, fy4);
	pDC->LineTo(x, y);
	pView->Get_VPort_XY(&x, &y, fx2, fy2);
	pDC->LineTo(x, y);
	pView->Get_VPort_XY(&x, &y, fx3, fy3);
	pDC->LineTo(x, y);	
	pView->Get_VPort_XY(&x, &y, fx1, fy1);
	pDC->LineTo(x, y);

	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}

void COb_pcbtext::ShowTTString(COLORREF color, CDC* pDC, CView* pV)
{
	char original_str[1000];

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	CString text = m_csText;
	text.Left(990);
	strcpy(original_str, text);

	LOGFONT logfont;
	logfont.lfHeight = Default_Offset_Height; 
	logfont.lfWidth = 0;
	logfont.lfOrientation = 0;
	logfont.lfEscapement = 0;
	if(m_bBold == FALSE) logfont.lfWeight = FW_NORMAL;
	else logfont.lfWeight = FW_BOLD;
	logfont.lfItalic = m_bItalic;
	logfont.lfUnderline = 0;
	logfont.lfStrikeOut = 0;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = PROOF_QUALITY;
	logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(logfont.lfFaceName, m_csTTFontName);

	if((m_nDescent < 0)||(m_nAscent < 0)||(m_nLeft < 0)||(m_nRight < 0)||(m_nTop < 0)||(m_nBottom < 0))
	{
		GetTextBitmapMargin(pDC, original_str, &logfont);

		logfont.lfHeight = (int)(m_fHeight/fEDA_ScaleMin); 
	
		CFont font, *oldfont;
		font.CreateFontIndirect(&logfont);
		oldfont = pDC->SelectObject(&font);	
		
		CSize size = pDC->GetTextExtent(text);
		
		m_fX1 = m_fSx;
		m_fY1 = m_fSy - (m_nBottom - m_nAscent)*m_fHeight/Default_Offset_Height;
		m_fX2 = m_fSx + size.cx*fEDA_ScaleMin;
		m_fY2 = m_fSy + (m_nAscent-m_nTop)*m_fHeight/Default_Offset_Height;

		m_fX2 -= 5;	

		GetRotatedTextBorder(m_fRotation);

		if(m_bInverted == TRUE)
		{
			m_fX1 -= m_fInvertedBorder;
			m_fY1 -= m_fInvertedBorder;
			m_fX2 += m_fInvertedBorder;
			m_fY2 += m_fInvertedBorder;
		}
	
	}
	
	if(pView->m_nPrint_Flag == 1)
	{
		logfont.lfHeight = (int)((m_fHeight*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
	}
	else
	{
		logfont.lfHeight = (int)(m_fHeight/pView->m_fScale);
	}
	if(logfont.lfHeight == 0) return;

	logfont.lfOrientation = (int)(m_fRotation);
	logfont.lfEscapement = (int)(m_fRotation*10);
	
	CFont font, *oldfont;
	font.CreateFontIndirect(&logfont);
	oldfont = pDC->SelectObject(&font);	

	int x,y;
	pView->Get_VPort_XY(&x,&y,m_fSx,m_fSy);

	int offset;
	offset = (int)(m_nLeft*(m_fHeight/pView->m_fScale)/Default_Offset_Height);

	if(pView->m_nPrint_Flag == 1)
	{
		x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
		offset = (int)(((double)offset*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}

	UINT flag = pDC->GetTextAlign();
	pDC->SetTextAlign(TA_BASELINE|VTA_BASELINE|TA_LEFT);

	if(m_bInverted == FALSE)	pDC->TextOut(x - (int)(offset*cos(m_fRotation*fEDA_pi/180)), y + (int)(offset*sin(m_fRotation*fEDA_pi/180)), original_str, strlen(original_str));
	else
	{
		COb_pcbfill pcbfill;
		pcbfill.m_nLayer = m_nLayer;
		pcbfill.m_fRotation = m_fRotation;
		pcbfill.m_fX1 = m_fX1;
		pcbfill.m_fY1 = m_fY1;
		pcbfill.m_fX2 = m_fX2;
		pcbfill.m_fY2 = m_fY2;
		pcbfill.m_nFlag = m_nFlag;		
		pcbfill.Draw(pDC, pV);

		COLORREF inverted_color;
		if(pView->m_nPrint_Flag == 1) inverted_color = RGB(255,255,255);
		else inverted_color = nEDA_PCB_BoardAreaColor;
		pDC->SetTextColor(inverted_color);
		pDC->TextOut(x - (int)(offset*cos(m_fRotation*fEDA_pi/180)), y + (int)(offset*sin(m_fRotation*fEDA_pi/180)), original_str, strlen(original_str));
	}

	pDC->SelectObject(oldfont);
	pDC->SetTextAlign(flag);
}

void COb_pcbtext::ShowMirrorTTString(COLORREF color, CDC* pDC, CView* pV)
{
	char original_str[1000];

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	CString text = m_csText;
	text.Left(990);
	strcpy(original_str, text);

	LOGFONT logfont;
	logfont.lfHeight = Default_Offset_Height; 
	logfont.lfWidth = 0;
	logfont.lfOrientation = 0;
	logfont.lfEscapement = 0;
	if(m_bBold == FALSE) logfont.lfWeight = FW_NORMAL;
	else logfont.lfWeight = FW_BOLD;
	logfont.lfItalic = m_bItalic;
	logfont.lfUnderline = 0;
	logfont.lfStrikeOut = 0;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = PROOF_QUALITY;
	logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
	strcpy(logfont.lfFaceName, m_csTTFontName);


	CFont font, *oldfont;
	font.CreateFontIndirect(&logfont);
	oldfont = pDC->SelectObject(&font);

	CSize size = pDC->GetTextExtent(original_str);
	
	TEXTMETRIC m;
	pDC->GetTextMetrics(&m);
	m_nAscent = m.tmAscent;

	if((m_nLeft < 0)||(m_nRight < 0)||(m_nTop < 0)||(m_nBottom < 0))
	{
		GetGDIPlusTextBitmapMargin(pDC, text.AllocSysString(), &logfont, size.cx+10, size.cy);
		
		m_fX1 = m_fSx - (m_nRight - m_nLeft)*m_fHeight/Default_Offset_Height; 
		m_fY1 = m_fSy - (m_nBottom - m_nAscent)*m_fHeight/Default_Offset_Height;
		m_fX2 = m_fSx;
		m_fY2 = m_fSy + (m_nAscent-m_nTop)*m_fHeight/Default_Offset_Height;

		m_fX1 -= 5;
		m_fY1 -= 5;

		GetRotatedTextBorder(m_fRotation);

		if(m_bInverted == TRUE)
		{
			m_fX1 -= m_fInvertedBorder;
			m_fY1 -= m_fInvertedBorder;
			m_fX2 += m_fInvertedBorder;
			m_fY2 += m_fInvertedBorder;
		}
	
	}
	
	if(pView->m_nPrint_Flag == 1)
	{
		logfont.lfHeight = (int)((m_fHeight*0.96*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
	}
	else
	{
		logfont.lfHeight = (int)(m_fHeight*0.96/pView->m_fScale);
	}
	if(logfont.lfHeight == 0) return;

	CFont font1;
	font1.CreateFontIndirect(&logfont);
	pDC->SelectObject(&font1);	

	pDC->GetTextMetrics(&m);

	int x,y; 
	pView->Get_VPort_XY(&x,&y,m_fSx,m_fSy);
	if(pView->m_nPrint_Flag == 1)
	{
		x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}	

	Graphics g(pDC->m_hDC);
	g.SetPageUnit(UnitPixel);
	g.SetTextRenderingHint(TextRenderingHintSingleBitPerPixel  );

	Gdiplus::Font gfont(pDC->m_hDC, &logfont);

	BYTE color_alpha = 0xFF;
	BYTE color_r = 0x000000ff&color;
	BYTE color_g = (0x0000ff00&color)>>8;
	BYTE color_b = (0x00FF0000&color)>>16;

	// Create an ARGB value from the four component values.
	ARGB argb = Color::MakeARGB(color_alpha, color_r, color_g, color_b);

	SolidBrush gbrush(argb);
	if(m_bInverted == FALSE) gbrush.SetColor(argb);
	else 
	{
		if(pView->m_nPrint_Flag == 1) gbrush.SetColor(Color::White);
		else
		{
			BYTE color_r = 0x000000ff&nEDA_PCB_BoardAreaColor;
			BYTE color_g = (0x0000ff00&nEDA_PCB_BoardAreaColor)>>8;
			BYTE color_b = (0x00FF0000&nEDA_PCB_BoardAreaColor)>>16;
			ARGB argb = Color::MakeARGB(color_alpha, color_r, color_g, color_b);
			gbrush.SetColor(argb);
		}
	}

	g.TranslateTransform(x, y);
	g.RotateTransform(m_fRotation*(-1.0f));
	if(m_bMirror == TRUE) g.ScaleTransform(-1.0f, 1.0f);
	else g.ScaleTransform(1.0f, 1.0f);

	int sx, sy; 
	sx = 0 - (int)(m_nLeft*(m_fHeight/pView->m_fScale)/Default_Offset_Height);
	sy = 0 - m.tmAscent;

	if(m_bInverted == FALSE) 
	{
		TCHAR wtext[1100];
		int nLen = strlen(original_str) + 1;
		int nwLen = MultiByteToWideChar(CP_ACP,   0,   original_str,   nLen,   NULL,   0);   
		MultiByteToWideChar(CP_ACP,   0,   original_str,   nLen,  (LPWSTR)wtext,   nwLen);   
		g.DrawString((LPWSTR)wtext, wcslen((LPWSTR)wtext), &gfont, PointF(sx,sy), &gbrush);
	}
	else
	{
		COb_pcbfill pcbfill;
		pcbfill.m_nLayer = m_nLayer;
		pcbfill.m_fRotation = m_fRotation;
		pcbfill.m_fX1 = m_fX1;
		pcbfill.m_fY1 = m_fY1;
		pcbfill.m_fX2 = m_fX2;
		pcbfill.m_fY2 = m_fY2;
		pcbfill.m_nFlag = m_nFlag;
		pcbfill.Draw(pDC, pV);

		TCHAR wtext[1100];
		int nLen = strlen(original_str) + 1;
		int nwLen = MultiByteToWideChar(CP_ACP,   0,   original_str,   nLen,   NULL,   0);   
		MultiByteToWideChar(CP_ACP,   0,   original_str,   nLen,  (LPWSTR)wtext,   nwLen);   
		g.DrawString((LPWSTR)wtext, wcslen((LPWSTR)wtext), &gfont, PointF(sx,sy), &gbrush);
	}

	pDC->SelectObject(oldfont);
}

void COb_pcbtext::ShowStrokeString(COLORREF color, CDC* pDC, CView* pV)
{
	char original_str[1000];

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	CString text = m_csText;
	text.Left(990);
	strcpy(original_str, text);

	int width;
	if(pView->m_nPrint_Flag == 1) width = (int)((m_fWidth*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
	else width = (int)(m_fWidth/pView->m_fScale);
	if(width < 1) width = 1;
	
	CPen pen, *oldpen;
	pen.CreatePen(PS_SOLID, width, color);
	oldpen = pDC->SelectObject(&pen);

	double fx, fy;
	fx = m_fSx;
	fy = m_fSy;
	for(int i=0; i<(int)strlen(original_str); i++)
	{
		int x,y; 
		pView->Get_VPort_XY(&x,&y,fx,fy);

		unsigned char c = original_str[i];
		c -= 32;
		if( c > 95 ) c = 95;


		double yoffset, w;
		int number;
		if(m_nStrokeFont == 0)
		{
			yoffset = (double)nEDA_DeafultFont[c].yoffset;
			w = (double)nEDA_DeafultFont[c].w;
			number = nEDA_DeafultFont[c].number;
		}
		else if(m_nStrokeFont == 1)
		{
			yoffset = (double)nEDA_SansSerifFont[c].yoffset;
			w = (double)nEDA_SansSerifFont[c].w;
			number = nEDA_SansSerifFont[c].number;
		}
		else if(m_nStrokeFont == 2)
		{
			yoffset = (double)nEDA_SerifFont[c].yoffset;
			w = (double)nEDA_SerifFont[c].w;
			number = nEDA_SerifFont[c].number;		
		}

		w = w*m_fHeight/100.0f;
		//yoffset = yoffset*(m_fHeight/(pView->m_fScale*100));
		for(int j=0; j<number; j++)
		{
			double data_fx, data_fy;
			int sx, sy;
			int pu_pd;
			if(m_nStrokeFont == 0)
			{
				pu_pd = nEDA_DeafultFont[c].pu_pd[j][0];
				data_fx = (double)nEDA_DeafultFont[c].pu_pd[j][1];
				data_fy = (double)nEDA_DeafultFont[c].pu_pd[j][2];
			}
			else if(m_nStrokeFont == 1)
			{
				pu_pd = nEDA_SansSerifFont[c].pu_pd[j][0];
				data_fx = (double)nEDA_SansSerifFont[c].pu_pd[j][1];
				data_fy = (double)nEDA_SansSerifFont[c].pu_pd[j][2];
			}
			else if(m_nStrokeFont == 2)
			{
				pu_pd = nEDA_SerifFont[c].pu_pd[j][0];
				data_fx = (double)nEDA_SerifFont[c].pu_pd[j][1];
				data_fy = (double)nEDA_SerifFont[c].pu_pd[j][2];
			}


			data_fy = data_fy*0.98f - yoffset;

			data_fx = data_fx*m_fHeight/100;
			data_fy = data_fy*m_fHeight/100;

			data_fx = data_fx/pView->m_fScale;
			data_fy = data_fy/pView->m_fScale;
		
			if(m_bMirror == FALSE)
			{
				if(m_fRotation == 0) 
				{
					sx = x + (int)data_fx;
					sy = y - (int)data_fy;
				}
				else if(m_fRotation == 90.0f)
				{
					sx = x - (int)data_fy;
					sy = y - (int)data_fx;
				}
				else if(m_fRotation == 180.0f)
				{
					sx = x - (int)data_fx;
					sy = y + (int)data_fy;
				}
				else if(m_fRotation == 270.0f)
				{
					sx = x + (int)data_fy;
					sy = y + (int)data_fx;
				}
				else
				{
					double temp_fx, temp_fy;
					GetRotationPoint(&temp_fx, &temp_fy, data_fx, data_fy);
					sx = x + (int)temp_fx;
					sy = y - (int)temp_fy;
				}
			}
			else
			{
				if(m_fRotation == 0) 
				{
					sx = x - (int)data_fx;
					sy = y - (int)data_fy;
				}
				else if(m_fRotation == 90.0f)
				{
					sx = x - (int)data_fy;
					sy = y + (int)data_fx;
				}
				else if(m_fRotation == 180.0f)
				{
					sx = x + (int)data_fx;
					sy = y + (int)data_fy;
				}
				else if(m_fRotation == 270.0f)
				{
					sx = x + (int)data_fy;
					sy = y - (int)data_fx;
				}
				else
				{
					double temp_fx, temp_fy;
					GetMirrorRotationPoint(&temp_fx, &temp_fy, data_fx, data_fy);
					sx = x + (int)temp_fx;
					sy = y - (int)temp_fy;
				}
			}

			if(pView->m_nPrint_Flag == 1)
			{
				sx = (int)(((double)sx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
				sy = (int)(((double)sy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
			}
			if(pu_pd == 0) pDC->MoveTo(sx, sy);
			else pDC->LineTo(sx, sy);

		}

		if(m_bMirror == FALSE)
		{
			if(m_fRotation == 0) fx += w;
			else if(m_fRotation == 90.0f) fy += w;
			else if(m_fRotation == 180.0f) fx -= w;
			else if(m_fRotation == 270.0f) fy -= w;
			else
			{
				fx += w*cos(m_fRotation*fEDA_pi/180);
				fy += w*sin(m_fRotation*fEDA_pi/180);
			}
		}
		else
		{
			if(m_fRotation == 0) fx -= w;
			else if(m_fRotation == 90.0f) fy -= w;
			else if(m_fRotation == 180.0f) fx += w;
			else if(m_fRotation == 270.0f) fy += w;
			else
			{
				fx -= w*cos(m_fRotation*fEDA_pi/180);
				fy -= w*sin(m_fRotation*fEDA_pi/180);
			}
		}
	}

	pDC->SelectObject(oldpen);
}


BOOL COb_pcbtext::PickMe(CDC* pDC, CView* pV)
{
	int  wx1,wy1,wx2,wy2;

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

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
	GetTextCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	

	int x1,x2,y1,y2;
	int maxx, minx, maxy, miny;
	x1 = MIN(fx1, fx2); x2 = MIN(fx3, fx4);
	minx = MIN(x1, x2);
	y1 = MIN(fy1, fy2); y2 = MIN(fy3, fy4);
	miny = MIN(y1, y2);

	x1 = MAX(fx1, fx2); x2 = MAX(fx3, fx4);
	maxx = MAX(x1, x2);
	y1 = MAX(fy1, fy2); y2 = MAX(fy3, fy4);
	maxy = MAX(y1, y2);

	if( ((minx < wx1)&&(maxx < wx1)) || ((minx > wx2)&&(maxx > wx2)) || ((miny < wy1)&&(maxy < wy1)) || ((miny > wy2)&&(maxy > wy2)) )	return FALSE;
	else return TRUE;
}

void  COb_pcbtext::GetTextCorner(double *fx11, double *fy11, double *fx22, double *fy22, double *fx33, double *fy33, double *fx44, double *fy44)
{

	double fx1,fy1,fx2,fy2;
	fx1 = m_fX1;	fy1 = m_fY1;
	fx2 = m_fX2;	fy2 = m_fY2;

	double fxm, fym;
	fxm = (fx1 + fx2)/2;
	fym = (fy1 + fy2)/2;

	fx1 -= fxm;	fy1 -= fym;
	fx2 -= fxm;	fy2 -= fym;

	double  fxx1,fyy1,fxx2,fyy2,fxx3,fyy3,fxx4,fyy4;
	if((m_fRotation == 0)||(m_fRotation == 180)||(m_fRotation == 360))
	{
		fxx1 = fx1; 	fyy1 = fy1;
		fxx2 = fx2;		fyy2 = fy2;
		fxx3 = fxx1;	fyy3 = fyy2;
		fxx4 = fxx2;	fyy4 = fyy1;
	}
	else if((m_fRotation == 90)||(m_fRotation == 270))
	{
		fxx1 = fy1; 	fyy1 = fx1;
		fxx2 = fy2;		fyy2 = fx2;
		fxx3 = fxx1;	fyy3 = fyy2;
		fxx4 = fxx2;	fyy4 = fyy1;
	}
	else
	{
		double alpha, r;
		alpha = atan(fy2/fx2);
		r = sqrt(fx2*fx2 + fy2*fy2);

		fxx2 = r*cos(alpha + m_fRotation*fEDA_pi/180);
		fyy2 = r*sin(alpha + m_fRotation*fEDA_pi/180);
		fxx1 = -fxx2;
		fyy1 = -fyy2;
		fxx4 = (fx2-fx1)*cos(m_fRotation*fEDA_pi/180) + fxx1;
		fyy4 = (fx2-fx1)*sin(m_fRotation*fEDA_pi/180) + fyy1;
		fxx3 = -fxx4;
		fyy3 = -fyy4;
	}

	fxx1 += fxm; fxx2 += fxm; fxx3 += fxm; fxx4 += fxm;
	fyy1 += fym; fyy2 += fym; fyy3 += fym; fyy4 += fym;

	*fx11 = fxx1; *fx22 = fxx2; *fx33 = fxx3; *fx44= fxx4;
	*fy11 = fyy1; *fy22 = fyy2; *fy33 = fyy3; *fy44= fyy4;
}

void COb_pcbtext::GetTextBitmapMargin(CDC* pDC, char* str, LOGFONT* logfont)
{
	int offset;
	
	CDC memdc;
	memdc.CreateCompatibleDC(pDC);

	CFont font, *oldfont;
	font.CreateFontIndirect(logfont);
	oldfont = memdc.SelectObject(&font);
	memdc.SetTextAlign(TA_TOP);

	CSize size = memdc.GetTextExtent(str);
	int cx = size.cx;
	int cy = size.cy;
	
	CBitmap bmpFace; 
	bmpFace.CreateCompatibleBitmap(pDC,cx,cy); 
	CBitmap* pOldBmp = NULL; 
	pOldBmp = memdc.SelectObject(&bmpFace); 
	memdc.FillSolidRect(0,0,cx,cy, RGB(0,0,0));
	memdc.SetTextColor(RGB(255,255,255));
	
	memdc.TextOut(0,0,str, strlen(str));

	TEXTMETRIC m;
	memdc.GetTextMetrics(&m);
	m_nAscent = m.tmAscent;
	m_nDescent = m.tmDescent;
	m_nInternalLeading = m.tmInternalLeading;

	offset = 0;
	for(int i=0; i<cx; i++)
	{
		for(int j=0; j<cy; j++)
		{
			COLORREF c = memdc.GetPixel(i,j);
			if(c != 0)
			{
				offset = i;
				i = cx;
				break;
			}
		}
	}
	m_nLeft = offset;

	offset = cx;
	for(int i=cx-1; i>=0; i--)
	{
		for(int j=0; j<cy; j++)
		{
			COLORREF c = memdc.GetPixel(i,j);
			if(c != 0)
			{
				offset = i;
				i = 0;
				break;
			}
		}
	}
	m_nRight = offset;

	offset = 0;
	for(int j=0; j<cy; j++)
	{
		for(int i=0; i<cx; i++)
		{
			COLORREF c = memdc.GetPixel(i,j);
			if(c != 0)
			{
				offset = j;
				j = cy;
				break;
			}
		}
	}
	m_nTop = offset;

	offset = cy;
	for(int j=cy-1; j>=0; j--)
	{
		for(int i=0; i<cx; i++)
		{
			COLORREF c = memdc.GetPixel(i,j);
			if(c != 0)
			{
				offset = j;
				j = 0;
				break;
			}
		}
	}
	m_nBottom = offset;

	memdc.SelectObject(oldfont);
	memdc.SelectObject(pOldBmp); 
	bmpFace.DeleteObject();
}



void COb_pcbtext::GetGDIPlusTextBitmapMargin(CDC* pDC, WCHAR* str, LOGFONT* logfont, int cx, int cy)
{
	int offset;
	
	CDC memdc;
	memdc.CreateCompatibleDC(pDC);
	
	CBitmap bmpFace; 
	bmpFace.CreateCompatibleBitmap(pDC,cx,cy); 
	CBitmap* pOldBmp = NULL; 
	pOldBmp = memdc.SelectObject(&bmpFace); 	
	
	Graphics g(memdc.m_hDC);
	Gdiplus::Font gfont(memdc.m_hDC, logfont);
	SolidBrush bkbrush(Color::Black);
	SolidBrush gbrush(Color::White);

	g.FillRectangle(&bkbrush, 0, 0, cx, cy);
	g.DrawString(str, wcslen(str), &gfont, PointF(0,0), &gbrush);

	offset = 0;
	for(int i=0; i<cx; i++)
	{
		for(int j=0; j<cy; j++)
		{
			COLORREF c = memdc.GetPixel(i,j);
			if(c != 0)
			{
				offset = i;
				i = cx;
				break;
			}
		}
	}
	m_nLeft = offset;

	offset = cx;
	for(int i=cx-1; i>=0; i--)
	{
		for(int j=0; j<cy; j++)
		{
			COLORREF c = memdc.GetPixel(i,j);
			if(c != 0)
			{
				offset = i;
				i = 0;
				break;
			}
		}
	}
	m_nRight = offset;

	offset = 0;
	for(int j=0; j<cy; j++)
	{
		for(int i=0; i<cx; i++)
		{
			COLORREF c = memdc.GetPixel(i,j);
			if(c != 0)
			{
				offset = j;
				j = cy;
				break;
			}
		}
	}
	m_nTop = offset;

	offset = cy;
	for(int j=cy-1; j>=0; j--)
	{
		for(int i=0; i<cx; i++)
		{
			COLORREF c = memdc.GetPixel(i,j);
			if(c != 0)
			{
				offset = j;
				j = 0;
				break;
			}
		}
	}
	m_nBottom = offset;

	memdc.SelectObject(pOldBmp); 
	bmpFace.DeleteObject();
}


void COb_pcbtext::GetRotationPoint(double *fxr, double *fyr, double fx, double fy)
{
	if((fx == 0)&&(fy == 0))
	{
		*fxr = fx;
		*fyr = fy;
	}
	else
	{
		double r;
		r = sqrt(fx*fx + fy*fy);
		double alpha;
		alpha = asin(fy/r);
		*fxr = r*cos(alpha + m_fRotation*fEDA_pi/180);
		*fyr = r*sin(alpha + m_fRotation*fEDA_pi/180);
	}
}

void COb_pcbtext::GetMirrorRotationPoint(double *fxr, double *fyr, double fx, double fy)
{
	if((fx == 0)&&(fy == 0))
	{
		*fxr = fx;
		*fyr = fy;
	}
	else
	{
		double r;
		r = sqrt(fx*fx + fy*fy);
		double alpha;
		alpha = asin(fy/r);

		*fxr = r*cos((180.0f + m_fRotation)*fEDA_pi/180 - alpha);
		*fyr = r*sin((180.0f + m_fRotation)*fEDA_pi/180 - alpha);
	}
}

double COb_pcbtext::GetStringWidth(CView* pV)
{
	double fw;
	CClientDC dc(pV);
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	char original_str[1000];

	CString text = m_csText;
	text.Left(990);
	strcpy(original_str, text);
		
	if(m_bUseTTFont == TRUE)
	{

		LOGFONT logfont;
		logfont.lfHeight = (int)(m_fHeight/pView->m_fScale);
		logfont.lfWidth = 0;
		logfont.lfOrientation = 0;
		logfont.lfEscapement = 0;
		if(m_bBold == FALSE) logfont.lfWeight = FW_NORMAL;
		else logfont.lfWeight = FW_BOLD;
		logfont.lfItalic = m_bItalic;
		logfont.lfUnderline = 0;
		logfont.lfStrikeOut = 0;
		logfont.lfCharSet = DEFAULT_CHARSET;
		logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
		logfont.lfQuality = PROOF_QUALITY;
		logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
		strcpy(logfont.lfFaceName, m_csTTFontName);

		CFont font, *oldfont;
		font.CreateFontIndirect(&logfont);	
		oldfont = dc.SelectObject(&font);

		CSize size = dc.GetTextExtent(m_csText);

		fw = size.cx*pView->m_fScale;
	}
	else
	{
		
		double fx, fy;
		fx = m_fSx;
		fy = m_fSy;
		double maxfx, minfy, maxfy;
		maxfx = fx;
		minfy = fy;
		maxfy = fy;

		for(int i=0; i<(int)strlen(original_str); i++)
		{
			unsigned char c = original_str[i];
			c -= 32;
			if( c > 95 ) c = 95;


			double yoffset, w;
			int number;
			if(m_nStrokeFont == 0)
			{
				yoffset = (double)nEDA_DeafultFont[c].yoffset;
				w = (double)nEDA_DeafultFont[c].w;
				number = nEDA_DeafultFont[c].number;
			}
			else if(m_nStrokeFont == 1)
			{
				yoffset = (double)nEDA_SansSerifFont[c].yoffset;
				w = (double)nEDA_SansSerifFont[c].w;
				number = nEDA_SansSerifFont[c].number;
			}
			else if(m_nStrokeFont == 2)
			{
				yoffset = (double)nEDA_SerifFont[c].yoffset;
				w = (double)nEDA_SerifFont[c].w;
				number = nEDA_SerifFont[c].number;		
			}

			w = w*m_fHeight/100.0f;

			for(int j=0; j<number; j++)
			{
				double data_fx, data_fy;
				int pu_pd;
				if(m_nStrokeFont == 0)
				{
					pu_pd = nEDA_DeafultFont[c].pu_pd[j][0];
					data_fx = (double)nEDA_DeafultFont[c].pu_pd[j][1];
					data_fy = (double)nEDA_DeafultFont[c].pu_pd[j][2];
				}
				else if(m_nStrokeFont == 1)
				{
					pu_pd = nEDA_SansSerifFont[c].pu_pd[j][0];
					data_fx = (double)nEDA_SansSerifFont[c].pu_pd[j][1];
					data_fy = (double)nEDA_SansSerifFont[c].pu_pd[j][2];
				}
				else if(m_nStrokeFont == 2)
				{
					pu_pd = nEDA_SerifFont[c].pu_pd[j][0];
					data_fx = (double)nEDA_SerifFont[c].pu_pd[j][1];
					data_fy = (double)nEDA_SerifFont[c].pu_pd[j][2];
				}


				data_fy = data_fy*0.98f - yoffset;

				data_fx = data_fx*m_fHeight/100;
				data_fy = data_fy*m_fHeight/100;
			
				maxfx = MAX(maxfx, (fx+data_fx));
				minfy = MIN(minfy, (fy+data_fy));
				maxfy = MAX(maxfy, (fy+data_fy));

			}

			fx += w;
		}
	
		fw = maxfx - m_fSx;
	}

	return fw;
}

void COb_pcbtext::GetStrokeStringRect(CView* pV, double *dx, double *dy1, double *dy2)
{
	double fw;
	CClientDC dc(pV);
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	char original_str[1000];

	CString text = m_csText;
	text.Left(990);
	strcpy(original_str, text);
		
	double fx, fy;
	fx = m_fSx;
	fy = m_fSy;
	double maxfx, minfy, maxfy;
	maxfx = fx;
	minfy = fy;
	maxfy = fy;

	for(int i=0; i<(int)strlen(original_str); i++)
	{
		unsigned char c = original_str[i];
		c -= 32;
		if( c > 95 ) c = 95;

		double yoffset, w;
		int number;
		if(m_nStrokeFont == 0)
		{
			yoffset = (double)nEDA_DeafultFont[c].yoffset;
			w = (double)nEDA_DeafultFont[c].w;
			number = nEDA_DeafultFont[c].number;
		}
		else if(m_nStrokeFont == 1)
		{
			yoffset = (double)nEDA_SansSerifFont[c].yoffset;
			w = (double)nEDA_SansSerifFont[c].w;
			number = nEDA_SansSerifFont[c].number;
		}
		else if(m_nStrokeFont == 2)
		{
			yoffset = (double)nEDA_SerifFont[c].yoffset;
			w = (double)nEDA_SerifFont[c].w;
			number = nEDA_SerifFont[c].number;		
		}

		w = w*m_fHeight/100.0f;

		for(int j=0; j<number; j++)
		{
			double data_fx, data_fy;
			int pu_pd;
			if(m_nStrokeFont == 0)
			{
				pu_pd = nEDA_DeafultFont[c].pu_pd[j][0];
				data_fx = (double)nEDA_DeafultFont[c].pu_pd[j][1];
				data_fy = (double)nEDA_DeafultFont[c].pu_pd[j][2];
			}
			else if(m_nStrokeFont == 1)
			{
				pu_pd = nEDA_SansSerifFont[c].pu_pd[j][0];
				data_fx = (double)nEDA_SansSerifFont[c].pu_pd[j][1];
				data_fy = (double)nEDA_SansSerifFont[c].pu_pd[j][2];
			}
			else if(m_nStrokeFont == 2)
			{
				pu_pd = nEDA_SerifFont[c].pu_pd[j][0];
				data_fx = (double)nEDA_SerifFont[c].pu_pd[j][1];
				data_fy = (double)nEDA_SerifFont[c].pu_pd[j][2];
			}


			data_fy = data_fy*0.98f - yoffset;

			data_fx = data_fx*m_fHeight/100;
			data_fy = data_fy*m_fHeight/100;
			
			maxfx = MAX(maxfx, (fx+data_fx));
			minfy = MIN(minfy, (fy+data_fy));
			maxfy = MAX(maxfy, (fy+data_fy));

		}

		fx += w;
	}
	
	*dx = maxfx - m_fSx;
	*dy1 = maxfy - m_fSy;
	*dy2 = minfy - m_fSy;
}


BOOL  COb_pcbtext::Is_On_Me(double fx, double fy, CView* pV)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return FALSE;

	int vertex = 4;

	CPoint point;
	CPoint* lpPoint  = (CPoint*)malloc(sizeof(point)*vertex);

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
	
	GetTextCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);

	lpPoint[0].x = fx1;
	lpPoint[0].y = fy1;
	lpPoint[1].x = fx4;
	lpPoint[1].y = fy4;
	lpPoint[2].x = fx2;
	lpPoint[2].y = fy2;
	lpPoint[3].x = fx3;
	lpPoint[3].y = fy3;
	
	CRgn   rgn;

	rgn.CreatePolygonRgn( lpPoint, vertex, ALTERNATE);

	free(lpPoint);

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

BOOL  COb_pcbtext::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fx, fy;
	fx = m_fSx;
	fy = m_fSy;

	if((fx < fx1)||(fx > fx2)||(fy < fy1)||(fy > fy2)) return FALSE;
	return TRUE;
}

void COb_pcbtext::Change(int index, CView* pV)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	CDlg_PCBEditText dlg;
	
	if(m_nComponent >= 0)
	{
		if(m_bDesignator == TRUE) dlg.csTitle = "名称(Designator)";
		else if(m_bComment == TRUE) dlg.csTitle = "标称值(Comment)";
	}

	int layer = 0;
	for(int i=0; i<=MULTILAYER; i++)
	{
		dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[layer].csLayerName);
		if(layer == BOTTOMLAYER) break;
		layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
	}
	for(int i=MECHANICAL1; i<=MECHANICAL16; i++)
	{
		if(pDoc->m_sPCB_LayerParam[i].bLayerMechEnabled == TRUE) dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[i].csLayerName);

	}
	for(int i=TOPOVERLAY; i<=MULTILAYER; i++)
	{
		if((i>=POWERPLANE1)&&(i<=POWERPLANE16)) continue;
		if((i>=MECHANICAL1)&&(i<=MECHANICAL16)) continue;
		dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[i].csLayerName);
	}
	
	for(int i=0; i<dlg.arrayString.GetCount(); i++)
	{
		if(pDoc->m_sPCB_LayerParam[m_nLayer].csLayerName == dlg.arrayString.GetAt(i)) 
		{
			dlg.m_nComboLayer = i;
			break;
		}
	}

	dlg.m_csEditText = m_csText;
	dlg.m_csEditHeight = pDoc->Convert_XY_To_String(m_fHeight);
	dlg.m_csEditWidth = pDoc->Convert_XY_To_String(m_fWidth);
	dlg.m_csEditX = pDoc->Convert_XY_To_String(m_fSx  - pDoc->m_cPCB_Sheet.m_fOriginX);
	dlg.m_csEditY = pDoc->Convert_XY_To_String(m_fSy  - pDoc->m_cPCB_Sheet.m_fOriginY);
	dlg.m_fEditRotation = m_fRotation;

	dlg.m_bCheckLocked = m_bLocked;
	dlg.m_bCheckMirror = m_bMirror;


	//TTFont
	for(int i=0; i<arrayEDA_SystemFont.GetCount(); i++)
	{
		if(m_csTTFontName == arrayEDA_SystemFont.GetAt(i)) 
		{
			dlg.nTTFont = i;
			break;
		}
	}
	dlg.nStrokeFont = m_nStrokeFont;

	if(m_bUseTTFont == TRUE)
	{
		dlg.m_nRadioFont = 0;
		dlg.m_nComboFont = dlg.nTTFont;
	}
	else 
	{
		dlg.m_nRadioFont = 1;
		dlg.m_nComboFont = dlg.nStrokeFont;
	}

	//True Type Font
	dlg.m_bCheckBold = m_bBold;
	dlg.m_bCheckItalic = m_bItalic;
	dlg.m_bCheckInverted = m_bInverted;
	dlg.m_csEditInverted = pDoc->Convert_XY_To_String(m_fInvertedBorder);
	
	if(dlg.DoModal() == IDOK)
	{

		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = PCB_ELEMENT_TEXT;
		pundo->m_nIndex = index;
		pundo->pcbtext.Copy(this);
			
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		
		for(int i=0; i<=MULTILAYER; i++)
		{
			if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
			{
				m_nLayer = i;
				break;
			}
		}
		
		m_csText = dlg.m_csEditText;
		m_fHeight = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditHeight);
		m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditWidth);
		m_fSx = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditX) + pDoc->m_cPCB_Sheet.m_fOriginX;
		m_fSy = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditY) + pDoc->m_cPCB_Sheet.m_fOriginY;
		m_fRotation = dlg.m_fEditRotation;

		m_bLocked = dlg.m_bCheckLocked;
		m_bMirror = dlg.m_bCheckMirror;
		
		if(dlg.m_nRadioFont == 0)
		{
			m_bUseTTFont = TRUE;
			m_csTTFontName = arrayEDA_SystemFont.GetAt(dlg.m_nComboFont);
		}
		else
		{
			m_bUseTTFont = FALSE;
			m_nStrokeFont = dlg.m_nComboFont;
		}

		//True Type Font
		m_bBold = dlg.m_bCheckBold;
		m_bItalic = dlg.m_bCheckItalic;
		m_bInverted = dlg.m_bCheckInverted;
		m_fInvertedBorder = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditInverted);

		UpdateStringBorder(pV, TRUE);
	}
}

void COb_pcbtext::Copy(COb_pcbtext* ppcbtext)
{
	pDocument = ppcbtext->pDocument;
	m_fSx = ppcbtext->m_fSx;
	m_fSy = ppcbtext->m_fSy;
	m_fHeight = ppcbtext->m_fHeight;
	m_fWidth = ppcbtext->m_fWidth;
	m_fRotation = ppcbtext->m_fRotation;

	m_nLeft = ppcbtext->m_nLeft;
	m_nRight = ppcbtext->m_nRight;
	m_nTop = ppcbtext->m_nTop;
	m_nBottom = ppcbtext->m_nBottom;

	m_fX1 = ppcbtext->m_fX1;
	m_fY1 = ppcbtext->m_fY1;
	m_fX2 = ppcbtext->m_fX2;
	m_fY2 = ppcbtext->m_fY2;
	m_nAscent = ppcbtext->m_nAscent;
	m_nDescent = ppcbtext->m_nDescent;
	m_nLayer = ppcbtext->m_nLayer;

	m_csText = ppcbtext->m_csText;
	m_bMirror = ppcbtext->m_bMirror;
	m_bLocked = ppcbtext->m_bLocked;
	
	m_nComponent = ppcbtext->m_nComponent;
	m_bDesignator = ppcbtext->m_bDesignator;
	m_bComment = ppcbtext->m_bComment;

	m_nStrokeFont = ppcbtext->m_nStrokeFont;
	m_bUseTTFont = ppcbtext->m_bUseTTFont;
	m_bBold = ppcbtext->m_bBold;
	m_bItalic = ppcbtext->m_bItalic;
	m_bInverted = ppcbtext->m_bInverted;
	m_csTTFontName = ppcbtext->m_csTTFontName;
	
	m_bPolygonOutline = ppcbtext->m_bPolygonOutline;
	m_bUserRouted = ppcbtext->m_bUserRouted;
	m_nUnionIndex = ppcbtext->m_nUnionIndex;
	m_fInvertedBorder = ppcbtext->m_fInvertedBorder;

	m_bSelection = FALSE; //ppcbtext->m_bSelection;
}

void COb_pcbtext::Move(CClientDC* pDC, CView* pV, int index)
{
	int ch1;
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

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_pcbtext atext;
	atext.Copy(this);
	atext.m_nFlag = FLAG_NORMAL;
	
	if(atext.m_bUseTTFont == TRUE)
	{
		atext.UpdateStringBorder(pV, FALSE);
		atext.DrawStringRect(pDC, pV);
	}
	else atext.Draw(pDC, pV);

	pView->m_fCurrentSnapX = atext.m_fSx;
	pView->m_fCurrentSnapY = atext.m_fSy;
	pView->Restore_LargeCrossXY();
	pView->OpenMouseCursor();
	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			if(atext.m_bUseTTFont == TRUE)
			{
				atext.UpdateStringBorder(pV, FALSE);
				atext.DrawStringRect(pDC, pV);
			}
			else atext.Draw(pDC, pV);
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
				if(atext.m_bUseTTFont == TRUE)
				{
					atext.UpdateStringBorder(pV, FALSE);
					atext.DrawStringRect(pDC, pV);
				}
				else atext.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  y1 = pView->m_fCurrentSnapY;
			atext.m_fSx = x1; atext.m_fSy = y1;
			
			pView->CloseMouseCursor();
			if(atext.m_bUseTTFont == TRUE)
			{
				atext.UpdateStringBorder(pV, FALSE);
				atext.DrawStringRect(pDC, pV);
			}
			else atext.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			if(atext.m_bUseTTFont == TRUE)
			{
				atext.UpdateStringBorder(pV, FALSE);
				atext.DrawStringRect(pDC, pV);
			}
			else atext.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  y1 = pView->m_fCurrentSnapY;
			atext.m_fSx = x1; atext.m_fSy = y1;
		
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_TEXT;
			pundo->m_nIndex = index;
			pundo->pcbtext.Copy(this);
			pundo->pcbtext.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fSx = atext.m_fSx;
			m_fSy = atext.m_fSy;
			m_fRotation = atext.m_fRotation;
			m_bMirror = atext.m_bMirror;
			UpdateStringBorder(pV, FALSE);

			break;
		}
		else if((ch1==KEY_X)||(ch1==KEY_x))
		{  
			pView->CloseMouseCursor();
			if(atext.m_bUseTTFont == TRUE)
			{
				atext.UpdateStringBorder(pV, FALSE);
				atext.DrawStringRect(pDC, pV);
			}
			else atext.Draw(pDC, pV);

			if(atext.m_bMirror == TRUE) atext.m_bMirror = FALSE;
			else atext.m_bMirror = TRUE;

			if(atext.m_bUseTTFont == TRUE)
			{
				atext.UpdateStringBorder(pV, FALSE);
				atext.DrawStringRect(pDC, pV);
			}
			else atext.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			if(atext.m_bUseTTFont == TRUE)
			{
				atext.UpdateStringBorder(pV, FALSE);
				atext.DrawStringRect(pDC, pV);
			}
			else atext.Draw(pDC, pV);

			atext.m_fRotation += 90;
			atext.m_fRotation = pDoc->Get_Correct_Angle(atext.m_fRotation);

			if(atext.m_bUseTTFont == TRUE)
			{
				atext.UpdateStringBorder(pV, FALSE);
				atext.DrawStringRect(pDC, pV);
			}
			else atext.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));
	
	pDC->SetROP2(drawmode);

	pView->CloseMouseCursor();

	m_nFlag = FLAG_NORMAL;
	Draw(pDC, pV);

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
	
	pView->Write_Help(szEDA_Help_Ready);
}


BOOL COb_pcbtext::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	double x1,y1;

	CBitmap *pBitmap = NULL;	

	char szHelp[] = "复制文字: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	pView->Invalidate();

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_pcbtext atext;
	atext.Copy(this);
	atext.m_nFlag = FLAG_NORMAL;
	
	if(atext.m_bUseTTFont == TRUE)
	{
		atext.UpdateStringBorder(pV, FALSE);
		atext.DrawStringRect(pDC, pV);
	}
	else atext.Draw(pDC, pV);

	pView->m_fCurrentSnapX = atext.m_fSx;
	pView->m_fCurrentSnapY = atext.m_fSy;
	pView->Restore_LargeCrossXY();
	pView->OpenMouseCursor();
	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			if(atext.m_bUseTTFont == TRUE)
			{
				atext.UpdateStringBorder(pV, FALSE);
				atext.DrawStringRect(pDC, pV);
			}
			else atext.Draw(pDC, pV);
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
				if(atext.m_bUseTTFont == TRUE)
				{
					atext.UpdateStringBorder(pV, FALSE);
					atext.DrawStringRect(pDC, pV);
				}
				else atext.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  y1 = pView->m_fCurrentSnapY;
			atext.m_fSx = x1; atext.m_fSy = y1;
			
			pView->CloseMouseCursor();
			if(atext.m_bUseTTFont == TRUE)
			{
				atext.UpdateStringBorder(pV, FALSE);
				atext.DrawStringRect(pDC, pV);
			}
			else atext.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			if(atext.m_bUseTTFont == TRUE)
			{
				atext.UpdateStringBorder(pV, FALSE);
				atext.DrawStringRect(pDC, pV);
			}
			else atext.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  y1 = pView->m_fCurrentSnapY;
			atext.m_fSx = x1; atext.m_fSy = y1;
		
			m_fSx = atext.m_fSx;
			m_fSy = atext.m_fSy;
			m_fRotation = atext.m_fRotation;
			m_bMirror = atext.m_bMirror;
			UpdateStringBorder(pV, FALSE);

			break;
		}
		else if((ch1==KEY_X)||(ch1==KEY_x))
		{  
			pView->CloseMouseCursor();
			if(atext.m_bUseTTFont == TRUE)
			{
				atext.UpdateStringBorder(pV, FALSE);
				atext.DrawStringRect(pDC, pV);
			}
			else atext.Draw(pDC, pV);

			if(atext.m_bMirror == TRUE) atext.m_bMirror = FALSE;
			else atext.m_bMirror = TRUE;

			if(atext.m_bUseTTFont == TRUE)
			{
				atext.UpdateStringBorder(pV, FALSE);
				atext.DrawStringRect(pDC, pV);
			}
			else atext.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			if(atext.m_bUseTTFont == TRUE)
			{
				atext.UpdateStringBorder(pV, FALSE);
				atext.DrawStringRect(pDC, pV);
			}
			else atext.Draw(pDC, pV);

			atext.m_fRotation += 90;
			atext.m_fRotation = pDoc->Get_Correct_Angle(atext.m_fRotation);

			if(atext.m_bUseTTFont == TRUE)
			{
				atext.UpdateStringBorder(pV, FALSE);
				atext.DrawStringRect(pDC, pV);
			}
			else atext.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));
	
	pDC->SetROP2(drawmode);

	pView->CloseMouseCursor();
	m_nFlag = FLAG_NORMAL;
	Draw(pDC, pV);

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	pView->Write_Help(szEDA_Help_Ready);

	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;
}

void COb_pcbtext::UpdateStringBorder(CView* pV, BOOL refreshbitmap)
{
	if(m_bUseTTFont == FALSE)
	{
		double fh1, fh2, fw;

		GetStrokeStringRect(pV, &fw, &fh1, &fh2);
	
		m_fX1 = m_fSx;
		m_fY1 = m_fSy + fh2;
		m_fX2 = m_fSx + fw;
		m_fY2 = m_fSy + fh1;

		double fmx, fmy;
		fmx = (m_fX1 + m_fX2)/2 - m_fSx;
		fmy = (m_fY1 + m_fY2)/2 - m_fSy;

		double fmx_rotation, fmy_rotation;	
		double deltax, deltay;
		if(m_bMirror == FALSE)
		{
			GetRotationPoint(&fmx_rotation, &fmy_rotation, fmx, fmy);
			deltax = fmx_rotation - fmx;
			deltay = fmy_rotation - fmy;
		}
		else
		{
			GetMirrorRotationPoint(&fmx_rotation, &fmy_rotation, fmx, fmy);
			deltax = fmx_rotation - fmx;
			deltay = fmy_rotation - fmy;
		}

		m_fX1 += deltax;
		m_fY1 += deltay;
		m_fX2 += deltax;
		m_fY2 += deltay;
		
	}
	else
	{
		CClientDC dc(pV);
		if(m_bMirror == FALSE)
		{
			char original_str[1000];

			CESDView* pView = (CESDView*)pV;
			CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

			CString text = m_csText;
			text.Left(990);
			strcpy(original_str, text);

			LOGFONT logfont;
			logfont.lfHeight = Default_Offset_Height; 
			logfont.lfWidth = 0;
			logfont.lfOrientation = 0;
			logfont.lfEscapement = 0;
			if(m_bBold == FALSE) logfont.lfWeight = FW_NORMAL;
			else logfont.lfWeight = FW_BOLD;
			logfont.lfItalic = m_bItalic;
			logfont.lfUnderline = 0;
			logfont.lfStrikeOut = 0;
			logfont.lfCharSet = DEFAULT_CHARSET;
			logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
			logfont.lfQuality = PROOF_QUALITY;
			logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
			strcpy(logfont.lfFaceName, m_csTTFontName);

			if(refreshbitmap == TRUE) GetTextBitmapMargin(&dc, original_str, &logfont);

			logfont.lfHeight = (int)(m_fHeight/fEDA_ScaleMin);  //假定m_fScale = fEDA_ScaleMin 放大到最大
			
			CFont font, *oldfont;
			font.CreateFontIndirect(&logfont);
			oldfont = dc.SelectObject(&font);	
				
			CSize size = dc.GetTextExtent(text);
				
			m_fX1 = m_fSx;
			m_fY1 = m_fSy - (m_nBottom - m_nAscent)*m_fHeight/Default_Offset_Height;
			m_fX2 = m_fSx + size.cx*fEDA_ScaleMin;
			m_fY2 = m_fSy + (m_nAscent-m_nTop)*m_fHeight/Default_Offset_Height;

			m_fX2 -= 5;	

			GetRotatedTextBorder(m_fRotation);

			if(m_bInverted == TRUE)
			{
				m_fX1 -= m_fInvertedBorder;
				m_fY1 -= m_fInvertedBorder;
				m_fX2 += m_fInvertedBorder;
				m_fY2 += m_fInvertedBorder;
			}
		
			dc.SelectObject(oldfont);
		}
		else
		{
			char original_str[1000];

			CESDView* pView = (CESDView*)pV;
			CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

			CString text = m_csText;
			text.Left(990);
			strcpy(original_str, text);

			LOGFONT logfont;
			logfont.lfHeight = Default_Offset_Height; 
			logfont.lfWidth = 0;
			logfont.lfOrientation = 0;
			logfont.lfEscapement = 0;
			if(m_bBold == FALSE) logfont.lfWeight = FW_NORMAL;
			else logfont.lfWeight = FW_BOLD;
			logfont.lfItalic = m_bItalic;
			logfont.lfUnderline = 0;
			logfont.lfStrikeOut = 0;
			logfont.lfCharSet = DEFAULT_CHARSET;
			logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
			logfont.lfQuality = PROOF_QUALITY;
			logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
			strcpy(logfont.lfFaceName, m_csTTFontName);


			CFont font, *oldfont;
			font.CreateFontIndirect(&logfont);
			oldfont = dc.SelectObject(&font);

			CSize size = dc.GetTextExtent(original_str);
			
			TEXTMETRIC m;
			dc.GetTextMetrics(&m);
			m_nAscent = m.tmAscent;

			if(refreshbitmap == TRUE) GetGDIPlusTextBitmapMargin(&dc, text.AllocSysString(), &logfont, size.cx+10, size.cy);
				
			m_fX1 = m_fSx - (m_nRight - m_nLeft)*m_fHeight/Default_Offset_Height; 
			m_fY1 = m_fSy - (m_nBottom - m_nAscent)*m_fHeight/Default_Offset_Height;
			m_fX2 = m_fSx;
			m_fY2 = m_fSy + (m_nAscent-m_nTop)*m_fHeight/Default_Offset_Height;

			m_fX1 -= 5;
			m_fY1 -= 5;

			GetRotatedTextBorder(m_fRotation);

			if(m_bInverted == TRUE)
			{
				m_fX1 -= m_fInvertedBorder;
				m_fY1 -= m_fInvertedBorder;
				m_fX2 += m_fInvertedBorder;
				m_fY2 += m_fInvertedBorder;
			}

			dc.SelectObject(oldfont);
		}
	}
}

void COb_pcbtext::DrawStringRect(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;
	
	COb_pcbtrack pcbtrack;
	pcbtrack.m_fWidth = 1;
	pcbtrack.m_nLayer = m_nLayer;

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;

	GetTextCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);

	pcbtrack.m_fX1 = fx1;
	pcbtrack.m_fY1 = fy1;
	pcbtrack.m_fX2 = fx4;
	pcbtrack.m_fY2 = fy4;
	pcbtrack.Draw(pDC, pV);
	pcbtrack.m_fX1 = fx4;
	pcbtrack.m_fY1 = fy4;
	pcbtrack.m_fX2 = fx2;
	pcbtrack.m_fY2 = fy2;
	pcbtrack.Draw(pDC, pV);
	pcbtrack.m_fX1 = fx2;
	pcbtrack.m_fY1 = fy2;
	pcbtrack.m_fX2 = fx3;
	pcbtrack.m_fY2 = fy3;
	pcbtrack.Draw(pDC, pV);
	pcbtrack.m_fX1 = fx3;
	pcbtrack.m_fY1 = fy3;
	pcbtrack.m_fX2 = fx1;
	pcbtrack.m_fY2 = fy1;
	pcbtrack.Draw(pDC, pV);
}

void COb_pcbtext::GetRotatedTextBorder(double angle)
{
	if(m_bMirror == FALSE)
	{
		if((angle == 0)||(angle == 360)) return;
		else if(angle == 90)
		{
			double deltax = (m_fX2 - m_fX1)/2 + ((m_fY2 + m_fY1)/2 - m_fSy);
			double deltay = (m_fX2 - m_fX1)/2 - ((m_fY2 + m_fY1)/2 - m_fSy);
			m_fX1 -= deltax;	m_fY1 += deltay;
			m_fX2 -= deltax;	m_fY2 += deltay;
		}
		else if(angle == 180)
		{
			double deltax = m_fX2 - m_fX1;
			double deltay = 2*((m_fY2 + m_fY1)/2 - m_fSy);
			m_fX1 -= deltax;	m_fY1 -= deltay;
			m_fX2 -= deltax;	m_fY2 -= deltay;
		}
		else if(angle == 270)
		{
			double deltax = (m_fX2 - m_fX1)/2 - ((m_fY2 + m_fY1)/2 - m_fSy);
			double deltay = (m_fX2 - m_fX1)/2 + ((m_fY2 + m_fY1)/2 - m_fSy);
			m_fX1 -= deltax;	m_fY1 -= deltay;
			m_fX2 -= deltax;	m_fY2 -= deltay;
		}
		else
		{
			double alpha;
			double dx, dy;
			dx = (m_fX2-m_fX1)/2;
			dy = (m_fY2 + m_fY1)/2 - m_fSy;
			double r = sqrt(dx*dx+dy*dy);
			alpha = acos(dx/r);
			
			double deltax = dx - r*cos(alpha + m_fRotation*fEDA_pi/180);
			double deltay = r*sin(alpha + m_fRotation*fEDA_pi/180) - dy;
			m_fX1 -= deltax;	m_fY1 += deltay;
			m_fX2 -= deltax;	m_fY2 += deltay;
		}
	}
	else
	{
		if((angle == 0)||(angle == 360)) return;
		else if(angle == 90)
		{
			double deltax = (m_fX2 - m_fX1)/2 - ((m_fY2 + m_fY1)/2 - m_fSy);
			double deltay = (m_fX2 - m_fX1)/2 + ((m_fY2 + m_fY1)/2 - m_fSy);
			m_fX1 += deltax;	m_fY1 -= deltay;
			m_fX2 += deltax;	m_fY2 -= deltay;
		}
		else if(angle == 180)
		{
			double deltax = m_fX2 - m_fX1;
			double deltay = 2*((m_fY2 + m_fY1)/2 - m_fSy);
			m_fX1 += deltax;	m_fY1 -= deltay;
			m_fX2 += deltax;	m_fY2 -= deltay;
		}
		else if(angle == 270)
		{
			double deltax = (m_fX2 - m_fX1)/2 + ((m_fY2 + m_fY1)/2 - m_fSy);
			double deltay = (m_fX2 - m_fX1)/2 - ((m_fY2 + m_fY1)/2 - m_fSy);
			m_fX1 += deltax;	m_fY1 += deltay;
			m_fX2 += deltax;	m_fY2 += deltay;
		}
		else
		{
			double alpha;
			double dx, dy;
			dx = (m_fX2-m_fX1)/2;
			dy = (m_fY2 + m_fY1)/2 - m_fSy;
			double r = sqrt(dx*dx+dy*dy);
			alpha = acos(dx/r);
			
			double deltax = dx - r*cos(m_fRotation*fEDA_pi/180 - alpha);
			double deltay = r*sin(m_fRotation*fEDA_pi/180 - alpha) + dy;
			m_fX1 += deltax;	m_fY1 -= deltay;
			m_fX2 += deltax;	m_fY2 -= deltay;
		}
	}
}