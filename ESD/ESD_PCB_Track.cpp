#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_PcbEditTrack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_pcbtrack, CObject, 0)
COb_pcbtrack::COb_pcbtrack()
{

	m_nNet = -1;
	m_nPolygon = -1;
	
	m_bLocked = FALSE;
	m_bKeepout = FALSE;
	
	m_bSelection = FALSE;
	m_nFlag = 0;

	//m_bUserRouted = TRUE;
	m_bTearDrop = FALSE;
	m_bPolygonOutline = FALSE;
	m_nSubPolyIndex = 0;
	m_nUnionIndex = -1;

	m_nComponent = -1;
}


void COb_pcbtrack::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fX1;
		ar << m_fY1;
		ar << m_fX2;
		ar << m_fY2;
		ar << m_fWidth;

		ar << m_nLayer;
		ar << m_nNet;
		ar << m_nComponent;
		ar << m_nPolygon;

		ar << m_bLocked;
		ar << m_bKeepout;

		ar << m_bPolygonOutline;
		//ar << m_bUserRouted;
		ar << m_bTearDrop;

		ar << m_nUnionIndex;
		ar << m_nSubPolyIndex;

		ar << m_bSelection;
	}
	else
	{
		ar >> m_fX1;
		ar >> m_fY1;
		ar >> m_fX2;
		ar >> m_fY2;
		ar >> m_fWidth;

		ar >> m_nLayer;
		ar >> m_nNet;
		ar >> m_nComponent;
		ar >> m_nPolygon;

		ar >> m_bLocked;
		ar >> m_bKeepout;

		ar >> m_bPolygonOutline;
		//ar >> m_bUserRouted;
		ar >> m_bTearDrop;

		ar >> m_nUnionIndex;
		ar >> m_nSubPolyIndex;

		ar >> m_bSelection;
	}
}

void COb_pcbtrack::Draw(CDC* pDC, CView* pV)
{

	int x1,y1,x2,y2;
	COLORREF color;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;


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
	
	pView->Get_VPort_XY(&x1,&y1,m_fX1,m_fY1);
	pView->Get_VPort_XY(&x2,&y2,m_fX2,m_fY2);
	if(pView->m_nPrint_Flag == 1)
	{
		x1 = (int)(((double)x1*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		y1 = (int)(((double)y1*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
		x2 = (int)(((double)x2*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		y2 = (int)(((double)y2*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
		
	}

	//线宽
	int width;
	if(pView->m_nPrint_Flag == 1) width = (int)(m_fWidth*nEDA_Print_Xdpi/1000*fEDA_PCBPrint_Scale);
	else width = (int)(m_fWidth/pView->m_fScale);
	if(width < 1) width = 1;

	if((pView->m_nPrint_Flag == 0)&&(m_bKeepout == TRUE)&&(width > 1)&&(m_nFlag == FLAG_NORMAL))  // m_bKeepout = TRUE
	{
		CPen linepen, keepout_pen, *oldpen;
		keepout_pen.CreatePen(PS_SOLID, width, nEDA_PCB_LayerColor[KEEPOUTLAYER]);
		oldpen = pDC->SelectObject(&keepout_pen);

		pDC->MoveTo(x1,y1);
		pDC->LineTo(x2,y2);

		if(width > 20) width -= 8;
		else if(width > 12) width -= 6;
		else if(width > 8 ) width -= 4;
		else if(width > 3 ) width -= 2;
		else width = 1;

		linepen.CreatePen(PS_SOLID, width, color);
		oldpen = pDC->SelectObject(&linepen);

		pDC->MoveTo(x1,y1);
		pDC->LineTo(x2,y2);

		pDC->SelectObject(oldpen);		
	}
	else
	{
		CPen linepen, *oldpen;
		linepen.CreatePen(PS_SOLID, width, color);

		oldpen = pDC->SelectObject(&linepen);

		pDC->MoveTo(x1,y1);
		pDC->LineTo(x2,y2);

		pDC->SelectObject(oldpen);
	}

	int drawmode = pDC->GetROP2();

	if((pView->m_nPrint_Flag == 0)&&(m_nNet >= 0)&&(m_nFlag == FLAG_NORMAL)&&(drawmode != R2_NOTXORPEN))
	{
		if(  ((m_fX1 != m_fX2)&&(m_fY1 != m_fY2)) || ((m_fX1 == m_fX2)&&(m_fY1 == m_fY2)) ) 
		{
		}
		else if(m_fX1 == m_fX2)
		{

			double f_mx, f_my;
			f_mx = m_fX1;
			f_my = (m_fY1 + m_fY2)/2;
			int cx, cy;
			pView->Get_VPort_XY(&cx, &cy, f_mx, f_my);

			int height;
			if(pView->m_fScale <= 2)
			{
	
				if(pView->m_fScale > 1.2) height = 6;
				else if(pView->m_fScale > 0.4) height = 10;
				else height = 14;

				COLORREF netcolor = RGB(255,255,255);
				
				COLORREF oldtextcolor;
				oldtextcolor = pDC->SetTextColor(netcolor);

				int bkmode = pDC->SetBkMode(TRANSPARENT);

				LOGFONT logfont;
				logfont.lfHeight = height; 
				logfont.lfWidth = 0;
				logfont.lfOrientation = 90;	
				logfont.lfEscapement = 900;
				logfont.lfWeight = FW_NORMAL;
				logfont.lfItalic = 0;
				logfont.lfUnderline = 0;
				logfont.lfStrikeOut = 0;
				logfont.lfCharSet = DEFAULT_CHARSET;
				logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
				logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
				logfont.lfQuality = PROOF_QUALITY;
				logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
				strcpy(logfont.lfFaceName,"Arial");

				CFont font, *oldfont;
				font.CreateFontIndirect(&logfont);	
				oldfont = pDC->SelectObject(&font);

				int x,y;			
				CString text;
				if(m_nNet >= 0)
				{
					COb_pcbnet* pcbnet = (COb_pcbnet*)pDoc->m_arrayPcbNet.GetAt(m_nNet);
					text = pcbnet->m_csNetName;
				}
				else text.Empty();

				CSize size = pDC->GetTextExtent(text);
				x = cx - size.cy/2;
				y = cy + size.cx/2;
				pDC->TextOut(x, y, text, strlen(text));

				pDC->SelectObject(oldfont);
				
				pDC->SetTextColor(oldtextcolor);
				pDC->SetBkMode(bkmode);

			}

		}
		else if(m_fY1 == m_fY2)
		{
	
			double f_mx, f_my;
			f_mx = (m_fX1 + m_fX2)/2;
			f_my = m_fY1;
			int cx, cy;
			pView->Get_VPort_XY(&cx,&cy,f_mx, f_my);

			int height;
			if(pView->m_fScale <= 2)
			{
				if(pView->m_fScale > 1.2) height = 6;
				else if(pView->m_fScale > 0.4) height = 10;
				else height = 14;

				COLORREF netcolor = RGB(255,255,255);
				
				COLORREF oldtextcolor;
				oldtextcolor = pDC->SetTextColor(netcolor);

				int bkmode = pDC->SetBkMode(TRANSPARENT);

				LOGFONT logfont;
				logfont.lfHeight = height; 
				logfont.lfWidth = 0;
				logfont.lfOrientation = 0;	
				logfont.lfEscapement = 0;
				logfont.lfWeight = FW_NORMAL;
				logfont.lfItalic = 0;
				logfont.lfUnderline = 0;
				logfont.lfStrikeOut = 0;
				logfont.lfCharSet = DEFAULT_CHARSET;
				logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
				logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
				logfont.lfQuality = PROOF_QUALITY;
				logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
				strcpy(logfont.lfFaceName,"Arial");

				CFont font, *oldfont;
				font.CreateFontIndirect(&logfont);	
				oldfont = pDC->SelectObject(&font);
				
				int x,y;			
				CString text;
				if(m_nNet >= 0)
				{
					COb_pcbnet* pcbnet = (COb_pcbnet*)pDoc->m_arrayPcbNet.GetAt(m_nNet);
					text = pcbnet->m_csNetName;
				}
				else text.Empty();
				CSize size = pDC->GetTextExtent(text);
				x = cx - size.cx/2;
				y = cy - size.cy/2;
				pDC->TextOut(x, y, text, strlen(text));

				pDC->SelectObject(oldfont);
				
				pDC->SetTextColor(oldtextcolor);
				pDC->SetBkMode(bkmode);
			}
		}
	}
}

void COb_pcbtrack::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	int x1,y1,x2,y2;

	COLORREF color = nEDA_PCB_LayerColor[m_nLayer];

	int width;
	width = (int)(m_fWidth/scale);
	if(width < 1) width = 1;

	CPen linepen, *oldpen;
	linepen.CreatePen(PS_SOLID, width, color);
	oldpen = pDC->SelectObject(&linepen);

	//pView->Get_VPort_XY(&x1,&y1,m_fX1,m_fY1);
	x1 = (int)((m_fX1 - origin_x)/scale);
	y1 = nEy - (int)((m_fY1 - origin_y)/scale);
	//pView->Get_VPort_XY(&x2,&y2,m_fX2,m_fY2);
	x2 = (int)((m_fX2 - origin_x)/scale);
	y2 = nEy - (int)((m_fY2 - origin_y)/scale);
	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y2);

	pDC->SelectObject(oldpen);
}

void COb_pcbtrack::DrawSelection(CDC* pDC, CView* pV)
{
	int x1,y1,x2,y2;
	CPen linepen, *oldpen;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;

	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	linepen.CreatePen(PS_DOT, 1, nEDA_PCB_LayerColor[SELECTIONS]);
	oldpen = pDC->SelectObject(&linepen);

	pView->Get_VPort_XY(&x1, &y1, m_fX1, m_fY1);
	pView->Get_VPort_XY(&x2, &y2, m_fX2, m_fY2);

	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y2);

	int x,y;
	pView->Get_VPort_XY(&x,&y,m_fX1,m_fY1);
	pView->DrawSelectionKeyPoint(x, y, pDC);
	pView->Get_VPort_XY(&x,&y,m_fX2,m_fY2);
	pView->DrawSelectionKeyPoint(x, y, pDC);

	pView->Get_VPort_XY(&x,&y,(m_fX1 + m_fX2)/2,(m_fY1 + m_fY2)/2);
	pView->DrawSelectionKeyPoint(x, y, pDC);

	pDC->SetBkMode(oldbkmode);
	pDC->SelectObject(oldpen);
}

BOOL COb_pcbtrack::PickMe(CView* pV)
{
	CESDView* pView = (CESDView*)pV;

	int wx1,wy1,wx2,wy2;
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

	int x1,y1,x2,y2;
	x1 = (int)MIN(m_fX1, m_fX2) - (int)m_fWidth;
	y1 = (int)MIN(m_fY1, m_fY2) - (int)m_fWidth;
	x2 = (int)MAX(m_fX1, m_fX2) + (int)m_fWidth;
	y2 = (int)MAX(m_fY1, m_fY2) + (int)m_fWidth;


	if( ((x1 < wx1)&&(x2 < wx1)) || ((x1 > wx2)&&(x2 > wx2)) || ((y1 < wy1)&&(y2 < wy1)) || ((y1 > wy2)&&(y2 > wy2)) )	return FALSE;
	else return TRUE;
}

void COb_pcbtrack::Change(int index)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_PCBEditTrack dlg;
	
	for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
	{
		dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
	}
	for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
	{
		if(m_nLayer == pDoc->m_arrayPcbLayerShow.GetAt(i))
		{
			dlg.m_nComboLayer = i;
			break;
		}
	}

	dlg.arrayStringNet.Add("无网络");
	for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = (COb_pcbnet*)pDoc->m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;
	
		dlg.arrayStringNet.Add(pnet->m_csNetName);
	}
	dlg.m_csComboNet = dlg.arrayStringNet.GetAt(m_nNet + 1);

	dlg.m_csEditWidth = pDoc->Convert_XY_To_String(m_fWidth);
	dlg.m_csEditX1 = pDoc->Convert_XY_To_String(m_fX1  - pDoc->m_cPCB_Sheet.m_fOriginX);
	dlg.m_csEditY1 = pDoc->Convert_XY_To_String(m_fY1  - pDoc->m_cPCB_Sheet.m_fOriginY);
	dlg.m_csEditX2 = pDoc->Convert_XY_To_String(m_fX2  - pDoc->m_cPCB_Sheet.m_fOriginX);
	dlg.m_csEditY2 = pDoc->Convert_XY_To_String(m_fY2  - pDoc->m_cPCB_Sheet.m_fOriginY);
	dlg.m_bCheckLocked = m_bLocked;
	dlg.m_bCheckKeepout = m_bKeepout;

	if(dlg.DoModal() == IDOK)
	{
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = PCB_ELEMENT_TRACK;
		pundo->m_nIndex = index;
		pundo->pcbtrack.Copy(this);
			
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

		for(int i=0; i<dlg.arrayStringNet.GetCount(); i++)
		{
			if(dlg.arrayStringNet.GetAt(i) == dlg.m_csComboNet)
			{
				m_nNet = i-1;
			}
		}

		m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditWidth);
		m_fX1 = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditX1) + pDoc->m_cPCB_Sheet.m_fOriginX;
		m_fY1 = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditY1) + pDoc->m_cPCB_Sheet.m_fOriginY;
		m_fX2 = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditX2) + pDoc->m_cPCB_Sheet.m_fOriginX;
		m_fY2 = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditY2) + pDoc->m_cPCB_Sheet.m_fOriginY;
		m_bLocked = dlg.m_bCheckLocked;
		m_bKeepout = dlg.m_bCheckKeepout;
	}
}

void COb_pcbtrack::Copy(COb_pcbtrack* ppcbtrack)
{
	m_fX1 = ppcbtrack->m_fX1;
	m_fY1 = ppcbtrack->m_fY1;
	m_fX2 = ppcbtrack->m_fX2;
	m_fY2 = ppcbtrack->m_fY2;
	m_fWidth = ppcbtrack->m_fWidth;
	m_nLayer = ppcbtrack->m_nLayer;

	m_bLocked = ppcbtrack->m_bLocked;	
	m_bKeepout = ppcbtrack->m_bKeepout;
	m_bTearDrop = ppcbtrack->m_bTearDrop;


	m_nNet = ppcbtrack->m_nNet;
	m_nComponent = ppcbtrack->m_nComponent;
	m_nPolygon = ppcbtrack->m_nPolygon;

	m_bSelection = FALSE; //ppcbtrack->m_bSelection;
	m_nFlag = ppcbtrack->m_nFlag;

	pDocument = ppcbtrack->pDocument;
}

BOOL COb_pcbtrack::Is_On_Me(double fx, double fy)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return FALSE;

	double ax,ay,bx,by;
	double alpha;

	double delta = m_fWidth/2 + nEDA_Selection_Offset;

	ax = m_fX1;
	ay = m_fY1;
	bx = m_fX2;
	by = m_fY2;
	if(ax == bx) alpha = fEDA_pi/2;
	else if(ay == by) alpha = 0;
	else alpha = atan((by-ay)/(bx-ax));
		
	CRgn rgn;
	CPoint ptVertex[4];
	ptVertex[0].x = (int)(ax - delta*sin(alpha));
	ptVertex[0].y = (int)(ay + delta*cos(alpha));
	ptVertex[1].x = (int)(ax + delta*sin(alpha));
	ptVertex[1].y = (int)(ay - delta*cos(alpha));
	ptVertex[2].x = (int)(bx + delta*sin(alpha));
	ptVertex[2].y = (int)(by - delta*cos(alpha));
	ptVertex[3].x = (int)(bx - delta*sin(alpha));
	ptVertex[3].y = (int)(by + delta*cos(alpha));

	rgn.CreatePolygonRgn( ptVertex, 4, ALTERNATE);

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) return TRUE;

	return FALSE;
}

BOOL  COb_pcbtrack::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fx, fy;
	fx = m_fX1;
	fy = m_fY1;
	if((fx < fx1)||(fx > fx2)||(fy < fy1)||(fy > fy2)) return FALSE;
	fx = m_fX2;
	fy = m_fY2;
	if((fx < fx1)||(fx > fx2)||(fy < fy1)||(fy > fy2)) return FALSE;
	return TRUE;
}

void COb_pcbtrack::Move(CClientDC* pDC, CView* pV, int index)
{
	int ch1;
	int flag;
	double fx, fy;
	double x1,y1, x0, y0, deltax, deltay;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	char szHelp[] = "移动连线: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV);

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	
	
	COb_pcbtrack atrack;
	atrack.Copy(this);
	atrack.m_nFlag = FLAG_NORMAL;
	atrack.Draw(pDC, pV);

	x0 = pView->m_fCurrentSnapX;
	y0 = pView->m_fCurrentSnapY;

	if(sqrt((m_fX1 - x0)*(m_fX1 - x0) + (m_fY1 - y0)*(m_fY1 - y0)) >= sqrt((m_fX2 - x0)*(m_fX2 - x0) + (m_fY2 - y0)*(m_fY2 - y0)))
	{
		x0 = m_fX2;
		y0 = m_fY2;
	}
	else
	{
		x0 = m_fX1;
		y0 = m_fY1;
	}
	
	pView->m_fCurrentSnapX = x0;
	pView->m_fCurrentSnapY = y0;
	pView->Restore_LargeCrossXY();

	pView->OpenMouseCursor();

	bEDA_NeedElectricalGrid = TRUE;

	flag = 0;
	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->PcbElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			atrack.Draw(pDC, pV);
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
				pView->PcbElecHotPointsControl(pDC);

				pView->CloseMouseCursor();
				atrack.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1-x0, m_fY1-y0, flag*90, FALSE);
			atrack.m_fX1 = fx + x0 + deltax;
			atrack.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2-x0, m_fY2-y0, flag*90, FALSE);
			atrack.m_fX2 = fx + x0 + deltax;
			atrack.m_fY2 = fy + y0 + deltay;

			pView->CloseMouseCursor();
			atrack.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->PcbElecHotPointsControl(pDC);
			pView->CloseMouseCursor();
			atrack.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;
			
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1-x0, m_fY1-y0, flag*90, FALSE);
			atrack.m_fX1 = fx + x0 + deltax;
			atrack.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2-x0, m_fY2-y0, flag*90, FALSE);
			atrack.m_fX2 = fx + x0 + deltax;
			atrack.m_fY2 = fy + y0 + deltay;

			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_TRACK;
			pundo->m_nIndex = index;
			pundo->pcbtrack.Copy(this);
			pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;


			m_fX1 = atrack.m_fX1;  m_fY1 = atrack.m_fY1;
			m_fX2 = atrack.m_fX2;  m_fY2 = atrack.m_fY2;
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			atrack.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1-x0, m_fY1-y0, flag*90, FALSE);
			atrack.m_fX1 = fx + x0 + deltax;
			atrack.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2-x0, m_fY2-y0, flag*90, FALSE);
			atrack.m_fX2 = fx + x0 + deltax;
			atrack.m_fY2 = fy + y0 + deltay;

			atrack.Draw(pDC, pV);

			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	
	pView->CloseMouseCursor();

	m_nFlag = FLAG_NORMAL;
	Draw(pDC, pV);
	pView->OpenMouseCursor();

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;


	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	pView->Write_Help(szEDA_Help_Ready);
	
	bEDA_NeedElectricalGrid = FALSE;
	pView->m_fpPcbHotPoint1.fx = pView->m_fpPcbHotPoint1.fy = pView->m_fpPcbHotPoint2.fx = pView->m_fpPcbHotPoint2.fy = 0;
}

void COb_pcbtrack::MoveAndRotate(CView* pV, double x0, double y0,  double deltax, double deltay, int rotation, BOOL mirror, int index)
{
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	CPcbUndo *pundo = new CPcbUndo;
	pundo->m_nOp = UNDO_CHANGE;
	pundo->m_nOb = PCB_ELEMENT_TRACK;
	pundo->m_nIndex = index;
	pundo->pcbtrack.Copy(this);
	pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
    
	double fx, fy;
	pDoc->Get_Rotate_XY(&fx, &fy, m_fX1-x0, m_fY1-y0, rotation, mirror);
	m_fX1 = fx + x0 + deltax;
	m_fY1 = fy + y0 + deltay;	
	pDoc->Get_Rotate_XY(&fx, &fy, m_fX2-x0, m_fY2-y0, rotation, mirror);
	m_fX2 = fx + x0 + deltax;
	m_fY2 = fy + y0 + deltay;	
}

BOOL COb_pcbtrack::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;
	double fx, fy;
	double x1,y1, x0, y0, deltax, deltay;
	
	char szHelp[] = "复制连线: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	pView->Invalidate();

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	
	
	COb_pcbtrack atrack;
	atrack.Copy(this);
	atrack.m_nFlag = FLAG_NORMAL;
	atrack.Draw(pDC, pV);

	x0 = pView->m_fCurrentSnapX;
	y0 = pView->m_fCurrentSnapY;

	if(sqrt((m_fX1 - x0)*(m_fX1 - x0) + (m_fY1 - y0)*(m_fY1 - y0)) >= sqrt((m_fX2 - x0)*(m_fX2 - x0) + (m_fY2 - y0)*(m_fY2 - y0)))
	{
		x0 = m_fX2;
		y0 = m_fY2;
	}
	else
	{
		x0 = m_fX1;
		y0 = m_fY1;
	}
	
	pView->m_fCurrentSnapX = x0;
	pView->m_fCurrentSnapY = y0;
	pView->Restore_LargeCrossXY();

	pView->OpenMouseCursor();

	bEDA_NeedElectricalGrid = TRUE;

	flag = 0;
	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->PcbElecHotPointsControl(pDC);
			pView->CloseMouseCursor();
			atrack.Draw(pDC, pV);
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
				pView->PcbElecHotPointsControl(pDC);

				pView->CloseMouseCursor();
				atrack.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1-x0, m_fY1-y0, flag*90, FALSE);
			atrack.m_fX1 = fx + x0 + deltax;
			atrack.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2-x0, m_fY2-y0, flag*90, FALSE);
			atrack.m_fX2 = fx + x0 + deltax;
			atrack.m_fY2 = fy + y0 + deltay;

			pView->CloseMouseCursor();
			atrack.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->PcbElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			atrack.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;
			
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1-x0, m_fY1-y0, flag*90, FALSE);
			atrack.m_fX1 = fx + x0 + deltax;
			atrack.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2-x0, m_fY2-y0, flag*90, FALSE);
			atrack.m_fX2 = fx + x0 + deltax;
			atrack.m_fY2 = fy + y0 + deltay;

			m_fX1 = atrack.m_fX1;  m_fY1 = atrack.m_fY1;
			m_fX2 = atrack.m_fX2;  m_fY2 = atrack.m_fY2;
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			atrack.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1-x0, m_fY1-y0, flag*90, FALSE);
			atrack.m_fX1 = fx + x0 + deltax;
			atrack.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2-x0, m_fY2-y0, flag*90, FALSE);
			atrack.m_fX2 = fx + x0 + deltax;
			atrack.m_fY2 = fy + y0 + deltay;

			atrack.Draw(pDC, pV);

			pView->OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	pDC->SetROP2(drawmode);
	
	pView->CloseMouseCursor();

	m_nFlag = FLAG_NORMAL;
	Draw(pDC, pV);
	pView->OpenMouseCursor();

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();

	ClipCursor(NULL); pView->m_bClipState = FALSE;

	pView->Write_Help(szEDA_Help_Ready);
	
	bEDA_NeedElectricalGrid = FALSE;
	pView->m_fpPcbHotPoint1.fx = pView->m_fpPcbHotPoint1.fy = pView->m_fpPcbHotPoint2.fx = pView->m_fpPcbHotPoint2.fy = 0;

	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;	
}

void COb_pcbtrack::DrawHighlighted(CDC* pDC, CView* pV, int layer)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;
	
	COLORREF color;
	if(layer == CONNECTIONS) color = nEDA_PCB_LayerColor[SELECTIONS];
	else if(layer == DRCERROR) color = nEDA_PCB_LayerColor[DRCERROR];

	int x1, y1, x2, y2, width;
	pView->Get_VPort_XY(&x1,&y1,m_fX1,m_fY1);
	pView->Get_VPort_XY(&x2,&y2,m_fX2,m_fY2);
	width = (int)(m_fWidth/pView->m_fScale);
	if(width < 1) width = 1;

	CPen linepen, keepout_pen, *oldpen;
	keepout_pen.CreatePen(PS_SOLID, width, nEDA_PCB_LayerColor[m_nLayer]);
	oldpen = pDC->SelectObject(&keepout_pen);

	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y2);

	if(width > 20) width -= 8;
	else if(width > 12) width -= 6;
	else if(width > 8 ) width -= 4;
	else if(width > 3 ) width -= 2;
	else width = 1;

	linepen.CreatePen(PS_SOLID, width, color);
	oldpen = pDC->SelectObject(&linepen);

	pDC->MoveTo(x1,y1);
	pDC->LineTo(x2,y2);

	pDC->SelectObject(oldpen);		
}

void COb_pcbtrack::GetTrackVertexOutline( CArray<Struct_VertexOutline, Struct_VertexOutline&>& arrayVertexOutline, double drc)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	if((m_fX1 == m_fX2)&&(m_fY1 == m_fY2)) return;

	double alpha;
	if(m_fX1 == m_fX2)
	{
		if(m_fY1 < m_fY2) alpha = 90;
		else alpha = 270;
	}
	else if(m_fY1 == m_fY2)
	{
		if(m_fX1 < m_fX2) alpha = 0;
		else alpha = 180;
	}
	else alpha = atan2((m_fY2 - m_fY1), (m_fX2 - m_fX1))*180/fEDA_pi;
	
	if(alpha < 0) alpha += 360;
	
	double fr = m_fWidth/2 + drc + 3;	

	Struct_VertexOutline vertexoutline;

	vertexoutline.kind = 1;
	vertexoutline.cx = m_fX1;
	vertexoutline.cy = m_fY1;
	vertexoutline.r = fr;
	vertexoutline.sangle = 90 + alpha;
	vertexoutline.sangle = pDoc->Get_Correct_Angle(vertexoutline.sangle);
	vertexoutline.eangle = 270 + alpha;
	vertexoutline.eangle = pDoc->Get_Correct_Angle(vertexoutline.eangle);

	vertexoutline.x1 = vertexoutline.cx + vertexoutline.r*cos(vertexoutline.sangle*fEDA_pi/180);
	vertexoutline.y1 = vertexoutline.cy + vertexoutline.r*sin(vertexoutline.sangle*fEDA_pi/180);

	vertexoutline.x2 = vertexoutline.cx + vertexoutline.r*cos(vertexoutline.eangle*fEDA_pi/180);
	vertexoutline.y2 = vertexoutline.cy + vertexoutline.r*sin(vertexoutline.eangle*fEDA_pi/180);
	arrayVertexOutline.Add(vertexoutline);

	vertexoutline.kind = 0;
	vertexoutline.x1 = m_fX1 + fr*cos((270+alpha)*fEDA_pi/180);
	vertexoutline.y1 = m_fY1 + fr*sin((270+alpha)*fEDA_pi/180);
	vertexoutline.x2 = m_fX2 + fr*cos((270+alpha)*fEDA_pi/180);
	vertexoutline.y2 = m_fY2 + fr*sin((270+alpha)*fEDA_pi/180);
	vertexoutline.cx = vertexoutline.cy = vertexoutline.r = vertexoutline.sangle = vertexoutline.eangle = 0;
	arrayVertexOutline.Add(vertexoutline);

	vertexoutline.kind = 1;
	vertexoutline.cx = m_fX2;
	vertexoutline.cy = m_fY2;
	vertexoutline.r = fr;
	vertexoutline.sangle = 270 + alpha;
	vertexoutline.sangle = pDoc->Get_Correct_Angle(vertexoutline.sangle);
	vertexoutline.eangle = 90 + alpha;
	vertexoutline.eangle = pDoc->Get_Correct_Angle(vertexoutline.eangle);

	vertexoutline.x1 = vertexoutline.cx + vertexoutline.r*cos(vertexoutline.sangle*fEDA_pi/180);
	vertexoutline.y1 = vertexoutline.cy + vertexoutline.r*sin(vertexoutline.sangle*fEDA_pi/180);

	vertexoutline.x2 = vertexoutline.cx + vertexoutline.r*cos(vertexoutline.eangle*fEDA_pi/180);
	vertexoutline.y2 = vertexoutline.cy + vertexoutline.r*sin(vertexoutline.eangle*fEDA_pi/180);
	arrayVertexOutline.Add(vertexoutline);


	vertexoutline.kind = 0;
	vertexoutline.x1 = m_fX2 + fr*cos((90+alpha)*fEDA_pi/180);
	vertexoutline.y1 = m_fY2 + fr*sin((90+alpha)*fEDA_pi/180);
	vertexoutline.x2 = m_fX1 + fr*cos((90+alpha)*fEDA_pi/180);
	vertexoutline.y2 = m_fY1 + fr*sin((90+alpha)*fEDA_pi/180);
	vertexoutline.cx = vertexoutline.cy = vertexoutline.r = vertexoutline.sangle = vertexoutline.eangle = 0;
	arrayVertexOutline.Add(vertexoutline);
}