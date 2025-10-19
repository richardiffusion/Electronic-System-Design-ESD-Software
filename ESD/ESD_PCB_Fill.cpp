#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_PcbEditFill.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_pcbfill, CObject, 0)
COb_pcbfill::COb_pcbfill()
{
	m_nNet = -1;
	m_nComponent = -1;

	m_bLocked = FALSE;
	m_bKeepout = FALSE;

	m_bPolygonOutline = FALSE;
	m_bUserRouted = TRUE;
	m_nUnionIndex = 0;

	m_bSelection = FALSE;
	m_nFlag = 0;
}

    double m_fX1, m_fY1, m_fX2, m_fY2;
	double m_fRotation;

	int m_nLayer;
	int m_nNet;
	int m_nComponent;

	BOOL m_bLocked;
	BOOL m_bKeepout;

	BOOL m_bPolygonOutline;
	BOOL m_bUserRouted;
	int m_nUnionIndex;

	BOOL m_bSelection;

void COb_pcbfill::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fX1;
		ar << m_fY1;
		ar << m_fX2;
		ar << m_fY2;
		ar << m_fRotation;

		ar << m_nLayer;
		ar << m_nNet;
		ar << m_nComponent;

		ar << m_bLocked;
		ar << m_bKeepout;

		ar << m_bPolygonOutline;
		ar << m_bUserRouted;
		ar << m_nUnionIndex;
		ar << m_bSelection;
	}
	else
	{
		ar >> m_fX1;
		ar >> m_fY1;
		ar >> m_fX2;
		ar >> m_fY2;
		ar >> m_fRotation;

		ar >> m_nLayer;
		ar >> m_nNet;
		ar >> m_nComponent;

		ar >> m_bLocked;
		ar >> m_bKeepout;

		ar >> m_bPolygonOutline;
		ar >> m_bUserRouted;
		ar >> m_nUnionIndex;
		ar >> m_bSelection;		
	}
}

void COb_pcbfill::Draw(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();	
	
	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;

	COLORREF color;
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

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
	GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	

	POINT point[5];
	int x,y;
	pView->Get_VPort_XY(&x,&y,fx1,fy1);
	if(pView->m_nPrint_Flag == 1)
	{
		x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}
	point[0].x = x; 
	point[0].y = y;
	point[4].x = x; 
	point[4].y = y;
	
	pView->Get_VPort_XY(&x,&y,fx3,fy3);
	if(pView->m_nPrint_Flag == 1)
	{
		x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}
	point[1].x = x;
	point[1].y = y;
	
	pView->Get_VPort_XY(&x,&y,fx2,fy2);
	if(pView->m_nPrint_Flag == 1)
	{
		x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}
	point[2].x = x;
	point[2].y = y;
	
	pView->Get_VPort_XY(&x,&y,fx4,fy4);
	if(pView->m_nPrint_Flag == 1)
	{
		x = (int)(((double)x*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		y = (int)(((double)y*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
	}
	point[3].x = x;
	point[3].y = y;

	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,1,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
 
	CBrush fillbrush;
	fillbrush.CreateSolidBrush(color);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);		
	
	pDC->Polygon(point, 5);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void COb_pcbfill::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	COLORREF color = nEDA_PCB_LayerColor[m_nLayer];

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
	GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	

	POINT point[5];
	int x,y;
	//pView->Get_VPort_XY(&x,&y,fx1,fy1);
	x = (int)((fx1 - origin_x)/scale);
	y = nEy - (int)((fy1 - origin_y)/scale);
	point[0].x = x; 
	point[0].y = y;
	point[4].x = x; 
	point[4].y = y;
	
	//pView->Get_VPort_XY(&x,&y,fx3,fy3);
	x = (int)((fx3 - origin_x)/scale);
	y = nEy - (int)((fy3 - origin_y)/scale);
	point[1].x = x;
	point[1].y = y;
	
	//pView->Get_VPort_XY(&x,&y,fx2,fy2);
	x = (int)((fx2 - origin_x)/scale);
	y = nEy - (int)((fy2 - origin_y)/scale);
	point[2].x = x;
	point[2].y = y;
	
	//pView->Get_VPort_XY(&x,&y,fx4,fy4);
	x = (int)((fx4 - origin_x)/scale);
	y = nEy - (int)((fy4 - origin_y)/scale);
	point[3].x = x;
	point[3].y = y;

	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,1,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
 
	CBrush fillbrush;
	fillbrush.CreateSolidBrush(color);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);		
	
	pDC->Polygon(point, 5);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void COb_pcbfill::DrawSelection(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;

	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	CPen arcpen;
	arcpen.CreatePen(PS_SOLID, 1, nEDA_PCB_LayerColor[SELECTIONS]);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;

	GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);

	DrawSelectionRect(pDC, pV);

	int x, y;

	double fcx, fcy;
	fcx = (m_fX1 + m_fX2)/2;
	fcy = (m_fY1 + m_fY2)/2;
	pView->Get_VPort_XY(&x, &y, fcx, fcy);
	pDC->MoveTo(x+5,y);
	pDC->LineTo(x-5,y);
	pDC->MoveTo(x,y+5);
	pDC->LineTo(x,y-5);

	double r = abs(m_fX2 - m_fX1)*0.35;
	double rx = fcx + r*cos(m_fRotation*fEDA_pi/180);
	double ry = fcy + r*sin(m_fRotation*fEDA_pi/180);

	pDC->MoveTo(x,y);
	pView->Get_VPort_XY(&x, &y, rx, ry);
	pDC->LineTo(x,y);
	pView->DrawSelectionKeyPoint(x, y, pDC);

	pView->Get_VPort_XY(&x, &y, fx1, fy1);
	pView->DrawSelectionKeyPoint(x, y, pDC);
	pView->Get_VPort_XY(&x, &y, fx4, fy4);
	pView->DrawSelectionKeyPoint(x, y, pDC);	
	pView->Get_VPort_XY(&x, &y, fx2, fy2);
	pView->DrawSelectionKeyPoint(x, y, pDC);	
	pView->Get_VPort_XY(&x, &y, fx3, fy3);
	pView->DrawSelectionKeyPoint(x, y, pDC);	
	

	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}

BOOL COb_pcbfill::PickMe(CView* pV)
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

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
	GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	

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

void  COb_pcbfill::GetFillCorner(double *fx11, double *fy11, double *fx22, double *fy22, double *fx33, double *fy33, double *fx44, double *fy44)
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

BOOL  COb_pcbfill::Is_On_Me(double fx, double fy)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return FALSE;

	int vertex = 4;

	CPoint point;
	CPoint* lpPoint  = (CPoint*)malloc(sizeof(point)*vertex);

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
	
	GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);

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

BOOL COb_pcbfill::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
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

void COb_pcbfill::Change(int index)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_PCBEditFill dlg;

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
		
	dlg.m_csEditX1 = pDoc->Convert_XY_To_String(m_fX1  - pDoc->m_cPCB_Sheet.m_fOriginX);
	dlg.m_csEditY1 = pDoc->Convert_XY_To_String(m_fY1  - pDoc->m_cPCB_Sheet.m_fOriginY);
	dlg.m_csEditX2 = pDoc->Convert_XY_To_String(m_fX2  - pDoc->m_cPCB_Sheet.m_fOriginX);
	dlg.m_csEditY2 = pDoc->Convert_XY_To_String(m_fY2  - pDoc->m_cPCB_Sheet.m_fOriginY);
	dlg.m_fEditRotation = m_fRotation;
	dlg.m_bCheckKeepout = m_bKeepout;
	dlg.m_bCheckLocked = m_bLocked;
	
	if(dlg.DoModal() == IDOK)
	{
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = PCB_ELEMENT_FILL;
		pundo->m_nIndex = index;
		pundo->pcbfill.Copy(this);
			
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

		m_fX1 = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditX1) + pDoc->m_cPCB_Sheet.m_fOriginX;
		m_fY1 = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditY1) + pDoc->m_cPCB_Sheet.m_fOriginY;
		m_fX2 = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditX2) + pDoc->m_cPCB_Sheet.m_fOriginX;
		m_fY2 = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditY2) + pDoc->m_cPCB_Sheet.m_fOriginY;
		double fxmin, fxmax, fymin, fymax;
		fxmin = MIN(m_fX1, m_fX2);
		fxmax = MAX(m_fX1, m_fX2);
		fymin = MIN(m_fY1, m_fY2);
		fymax = MAX(m_fY1, m_fY2);
		m_fX1 = fxmin;
		m_fY1 = fymin;
		m_fX2 = fxmax;
		m_fY2 = fymax;
		
		m_fRotation = dlg.m_fEditRotation;
		m_bKeepout = dlg.m_bCheckKeepout;
		m_bLocked = dlg.m_bCheckLocked;
	}
}

void COb_pcbfill::Copy(COb_pcbfill* ppcbfill)
{
	pDocument = ppcbfill->pDocument;
	m_fX1 = ppcbfill->m_fX1;
	m_fY1 = ppcbfill->m_fY1;
	m_fX2 = ppcbfill->m_fX2;
	m_fY2 = ppcbfill->m_fY2;
	m_fRotation = ppcbfill->m_fRotation;

	m_nLayer = ppcbfill->m_nLayer;
	m_nNet = ppcbfill->m_nNet;
	m_nComponent = ppcbfill->m_nComponent;

	m_bLocked = ppcbfill->m_bLocked;
	m_bKeepout = ppcbfill->m_bKeepout;

	//m_nIndexForSave = ppcbfill->m_nIndexForSave;
	m_bPolygonOutline = ppcbfill->m_bPolygonOutline;
	m_bUserRouted = ppcbfill->m_bUserRouted;
	m_nUnionIndex = ppcbfill->m_nUnionIndex;

	m_bSelection = FALSE; //ppcbfill->m_bSelection;

}

void COb_pcbfill::Move(CClientDC* pDC, CView* pV, int index)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动矩形充填: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	Draw(pDC, pV);

	x0 = m_fX1;
	y0 = m_fY1;
	
	pView->m_fCurrentSnapX = x0;
	pView->m_fCurrentSnapY = y0;
	pView->Restore_LargeCrossXY();

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_pcbfill afill;
	afill.Copy(this);
	afill.m_nFlag = FLAG_NORMAL;

	afill.Draw(pDC, pV);
	pView->OpenMouseCursor();

	flag = 0;
	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			afill.Draw(pDC, pV);
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
				afill.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, 90*flag, FALSE);
			afill.m_fX1 = fx + x0 + deltax;
			afill.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, 90*flag, FALSE);
			afill.m_fX2 = fx + x0 + deltax;
			afill.m_fY2 = fy + y0 + deltay;
			
			double fminx, fminy, fmaxx, fmaxy;
			fminx = MIN(afill.m_fX1, afill.m_fX2);
			fmaxx = MAX(afill.m_fX1, afill.m_fX2);
			fminy = MIN(afill.m_fY1, afill.m_fY2);
			fmaxy = MAX(afill.m_fY1, afill.m_fY2);
			afill.m_fX1 = fminx;
			afill.m_fY1 = fminy;
			afill.m_fX2 = fmaxx;
			afill.m_fY2 = fmaxy;

			pView->CloseMouseCursor();
			afill.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			afill.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, 90*flag, FALSE);
			afill.m_fX1 = fx + x0 + deltax;
			afill.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, 90*flag, FALSE);
			afill.m_fX2 = fx + x0 + deltax;
			afill.m_fY2 = fy + y0 + deltay;

			double fminx, fminy, fmaxx, fmaxy;
			fminx = MIN(afill.m_fX1, afill.m_fX2);
			fmaxx = MAX(afill.m_fX1, afill.m_fX2);
			fminy = MIN(afill.m_fY1, afill.m_fY2);
			fmaxy = MAX(afill.m_fY1, afill.m_fY2);
			afill.m_fX1 = fminx;
			afill.m_fY1 = fminy;
			afill.m_fX2 = fmaxx;
			afill.m_fY2 = fmaxy;			

			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_FILL;
			pundo->m_nIndex = index;
			pundo->pcbfill.Copy(this);
			pundo->pcbfill.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;

			m_fX1 = afill.m_fX1;
			m_fY1 = afill.m_fY1;
			m_fX2 = afill.m_fX2;
			m_fY2 = afill.m_fY2;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			afill.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, 90*flag, FALSE);
			afill.m_fX1 = fx + x0 + deltax;
			afill.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, 90*flag, FALSE);
			afill.m_fX2 = fx + x0 + deltax;
			afill.m_fY2 = fy + y0 + deltay;
			
			double fminx, fminy, fmaxx, fmaxy;
			fminx = MIN(afill.m_fX1, afill.m_fX2);
			fmaxx = MAX(afill.m_fX1, afill.m_fX2);
			fminy = MIN(afill.m_fY1, afill.m_fY2);
			fmaxy = MAX(afill.m_fY1, afill.m_fY2);
			afill.m_fX1 = fminx;
			afill.m_fY1 = fminy;
			afill.m_fX2 = fmaxx;
			afill.m_fY2 = fmaxy;				
			
			afill.Draw(pDC, pV);

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

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	pView->Write_Help(szEDA_Help_Ready);
}

BOOL COb_pcbfill::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
	
	char szHelp[] = "复制矩形充填: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	pView->Invalidate();
	//Draw(pDC, pV);

	x0 = m_fX1;
	y0 = m_fY1;
	
	pView->m_fCurrentSnapX = x0;
	pView->m_fCurrentSnapY = y0;
	pView->Restore_LargeCrossXY();

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_pcbfill afill;
	afill.Copy(this);
	afill.m_nFlag = FLAG_NORMAL;

	afill.Draw(pDC, pV);
	pView->OpenMouseCursor();

	flag = 0;
	pView->Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			afill.Draw(pDC, pV);
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
				afill.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, 90*flag, FALSE);
			afill.m_fX1 = fx + x0 + deltax;
			afill.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, 90*flag, FALSE);
			afill.m_fX2 = fx + x0 + deltax;
			afill.m_fY2 = fy + y0 + deltay;
			
			double fminx, fminy, fmaxx, fmaxy;
			fminx = MIN(afill.m_fX1, afill.m_fX2);
			fmaxx = MAX(afill.m_fX1, afill.m_fX2);
			fminy = MIN(afill.m_fY1, afill.m_fY2);
			fmaxy = MAX(afill.m_fY1, afill.m_fY2);
			afill.m_fX1 = fminx;
			afill.m_fY1 = fminy;
			afill.m_fX2 = fmaxx;
			afill.m_fY2 = fmaxy;

			pView->CloseMouseCursor();
			afill.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			afill.Draw(pDC, pV);
			pView->OpenMouseCursor();

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, 90*flag, FALSE);
			afill.m_fX1 = fx + x0 + deltax;
			afill.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, 90*flag, FALSE);
			afill.m_fX2 = fx + x0 + deltax;
			afill.m_fY2 = fy + y0 + deltay;

			double fminx, fminy, fmaxx, fmaxy;
			fminx = MIN(afill.m_fX1, afill.m_fX2);
			fmaxx = MAX(afill.m_fX1, afill.m_fX2);
			fminy = MIN(afill.m_fY1, afill.m_fY2);
			fmaxy = MAX(afill.m_fY1, afill.m_fY2);
			afill.m_fX1 = fminx;
			afill.m_fY1 = fminy;
			afill.m_fX2 = fmaxx;
			afill.m_fY2 = fmaxy;			

			m_fX1 = afill.m_fX1;
			m_fY1 = afill.m_fY1;
			m_fX2 = afill.m_fX2;
			m_fY2 = afill.m_fY2;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			afill.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX1 - x0, m_fY1 - y0, 90*flag, FALSE);
			afill.m_fX1 = fx + x0 + deltax;
			afill.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fX2 - x0, m_fY2 - y0, 90*flag, FALSE);
			afill.m_fX2 = fx + x0 + deltax;
			afill.m_fY2 = fy + y0 + deltay;
			
			double fminx, fminy, fmaxx, fmaxy;
			fminx = MIN(afill.m_fX1, afill.m_fX2);
			fmaxx = MAX(afill.m_fX1, afill.m_fX2);
			fminy = MIN(afill.m_fY1, afill.m_fY2);
			fmaxy = MAX(afill.m_fY1, afill.m_fY2);
			afill.m_fX1 = fminx;
			afill.m_fY1 = fminy;
			afill.m_fX2 = fmaxx;
			afill.m_fY2 = fmaxy;				
			
			afill.Draw(pDC, pV);

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
	
	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;
}

void COb_pcbfill::DrawSelectionRect(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	CPen arcpen;
	arcpen.CreatePen(PS_SOLID, 1, nEDA_PCB_LayerColor[SELECTIONS]);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;

	GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);
	
	POINT point[5];
	int x, y;
	pView->Get_VPort_XY(&x, &y, fx1, fy1);
	point[0].x = x;
	point[0].y = y;
	pView->Get_VPort_XY(&x, &y, fx4, fy4);
	point[1].x = x;
	point[1].y = y;
	pView->Get_VPort_XY(&x, &y, fx2, fy2);
	point[2].x = x;
	point[2].y = y;
	pView->Get_VPort_XY(&x, &y, fx3, fy3);
	point[3].x = x;
	point[3].y = y;
	pView->Get_VPort_XY(&x, &y, fx1, fy1);
	point[4].x = x;
	point[4].y = y;

	// Use the bit pattern to create a bitmap.
	WORD HatchBits[8] = { 0x81, 0x42, 0x24, 0x18, 0x18,  0x24, 0x42, 0x81 };
	CBitmap bm;
	bm.CreateBitmap(8,8,1,1, HatchBits);
	// Create a pattern brush from the bitmap.	
	CBrush fillbrush;
	fillbrush.CreatePatternBrush(&bm);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);

	int mode = pDC->SetROP2(R2_MERGEPEN );
	pDC->Polygon(point, 5);
	pDC->SetROP2(mode);

	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
	pDC->SelectObject(oldbrush);
}

void COb_pcbfill::DrawFrame(CDC* pDC, CView* pV)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	CPen arcpen;
	arcpen.CreatePen(PS_SOLID, 1, RGB(50,50,50));//nEDA_PCB_LayerColor[SELECTIONS]);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;

	GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);
	
	POINT point[5];
	int x, y;
	pView->Get_VPort_XY(&x, &y, fx1, fy1);
	point[0].x = x;
	point[0].y = y;
	pView->Get_VPort_XY(&x, &y, fx4, fy4);
	point[1].x = x;
	point[1].y = y;
	pView->Get_VPort_XY(&x, &y, fx2, fy2);
	point[2].x = x;
	point[2].y = y;
	pView->Get_VPort_XY(&x, &y, fx3, fy3);
	point[3].x = x;
	point[3].y = y;
	pView->Get_VPort_XY(&x, &y, fx1, fy1);
	point[4].x = x;
	point[4].y = y;

	//pDC->Polygon(point, 5);
	pDC->Polyline(point, 5);

	pDC->SelectObject(oldpen);
}

void COb_pcbfill::DrawHighlighted(CDC* pDC, CView* pV, int layer)
{
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();	
	
	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;

	COLORREF color;
	if(layer == CONNECTIONS) color = nEDA_PCB_LayerColor[SELECTIONS];
	else if(layer == DRCERROR) color = nEDA_PCB_LayerColor[DRCERROR];

	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
	GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	

	POINT point[5];
	int x,y;
	pView->Get_VPort_XY(&x,&y,fx1,fy1);
	point[0].x = x; 
	point[0].y = y;
	point[4].x = x; 
	point[4].y = y;
	
	pView->Get_VPort_XY(&x,&y,fx3,fy3);
	point[1].x = x;
	point[1].y = y;
	
	pView->Get_VPort_XY(&x,&y,fx2,fy2);
	point[2].x = x;
	point[2].y = y;
	
	pView->Get_VPort_XY(&x,&y,fx4,fy4);
	point[3].x = x;
	point[3].y = y;

	CPen fillpen;
	fillpen.CreatePen(PS_SOLID,1,color);
	CPen* oldpen =  pDC->SelectObject(&fillpen);
 
	CBrush fillbrush;
	fillbrush.CreateSolidBrush(color);
	CBrush* oldbrush = pDC->SelectObject(&fillbrush);		
	
	pDC->Polygon(point, 5);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}