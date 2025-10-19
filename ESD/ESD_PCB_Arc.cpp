#include "stdafx.h"

#include "ESD.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "sysparam.h"
#include "systemext.h"

#include "Dlg_PcbEditArc.h"


IMPLEMENT_SERIAL(COb_pcbarc, CObject, 0)
COb_pcbarc::COb_pcbarc()
{
	m_nNet = -1;
	m_nComponent = -1;
	m_nPolygon = -1;
	
	m_bLocked = FALSE;
	m_bKeepout = FALSE;

	m_bTearDrop = FALSE;
	//m_bUserRouted = TRUE;
	m_bPolygonOutline = FALSE;

	m_nSubPolyIndex = 0;
	m_nUnionIndex = -1;

	m_bSelection = FALSE;
	m_nFlag = 0;

	pDocument = NULL;
}

void COb_pcbarc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fCx;
		ar << m_fCy;
		ar << m_fRadius;
		ar << m_fSangle;
		ar << m_fEangle;
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
		ar >> m_fCx;
		ar >> m_fCy;
		ar >> m_fRadius;
		ar >> m_fSangle;
		ar >> m_fEangle;
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

void COb_pcbarc::Draw(CDC* pDC, CView* pV)
{
	int cx,cy;
	int r;
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

	int width;
	if(pView->m_nPrint_Flag == 1) width = (int)((m_fWidth*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
	else width = (int)(m_fWidth/pView->m_fScale);
	if(width < 1) width = 1;

	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
	
	if(pView->m_nPrint_Flag == 1)
	{
		cx = (int)(((double)cx*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
		cy = (int)(((double)cy*nEDA_Print_Ydpi)/1000*fEDA_PCBPrint_Scale);
		r =  (int)(((double)m_fRadius*nEDA_Print_Xdpi)/1000*fEDA_PCBPrint_Scale);
	}
	else r = (int)(m_fRadius/pView->m_fScale);

	CRect rect;
	CPoint sp, ep;

	rect.left = cx - r;
	rect.right = cx + r;
	rect.top = cy - r;
	rect.bottom = cy + r;

	if((m_fSangle == 0)||(m_fSangle == 360.0))
	{
		sp.x = cx + r;
		sp.y = cy;
	}
	else if(m_fSangle == 90)
	{
		sp.x = cx;
		sp.y = cy - r;
	}
	else if(m_fSangle == 180)
	{
		sp.x = cx - r;
		sp.y = cy;

	}
	else if(m_fSangle == 270)
	{
		sp.x = cx;
		sp.y = cy + r;
	}
	else
	{
		if((m_fSangle > 0)&&(m_fSangle < 180))
		{
			sp.x = cx + (int)(r/tan(m_fSangle*fEDA_pi/180));
			sp.y = cy - r;
		}
		else
		{
			sp.x = cx - (int)(r/tan(m_fSangle*fEDA_pi/180));
			sp.y = cy + r;
		}
	}

	if((m_fEangle == 0)||(m_fEangle == 360))
	{
		ep.x = cx + r;
		ep.y = cy;
	}
	else if(m_fEangle == 180)
	{
		ep.x = cx - r;
		ep.y = cy;
	}
	else
	{
		if((m_fEangle > 0)&&(m_fEangle < 180))
		{
			ep.x = cx + (int)(r/tan(m_fEangle*fEDA_pi/180));
			ep.y = cy - r;
		}
		else
		{
			ep.x = cx - (int)(r/tan(m_fEangle*fEDA_pi/180));
			ep.y = cy + r;
		}
	}

	if((pView->m_nPrint_Flag == 0)&&(m_bKeepout == TRUE)&&(width > 1)&&(m_nFlag == FLAG_NORMAL))  // m_bKeepout = TRUE
	{
		CPen arcpen, keepout_pen, *oldpen;
		keepout_pen.CreatePen(PS_SOLID, width, nEDA_PCB_LayerColor[KEEPOUTLAYER]);
		oldpen = pDC->SelectObject(&keepout_pen);

		pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);

		if(width > 20) width -= 8;
		else if(width > 12) width -= 6;
		else if(width > 8 ) width -= 4;
		else if(width > 3 ) width -= 2;
		else width = 1;

		arcpen.CreatePen(PS_SOLID, width, color);
		oldpen = pDC->SelectObject(&arcpen);

		pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);

		pDC->SelectObject(oldpen);		
	}
	else
	{
		CPen arcpen;
		arcpen.CreatePen(PS_SOLID, width, color);
		CPen* oldpen = pDC->SelectObject(&arcpen);

		pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);

		pDC->SelectObject(oldpen);
	}
}

void COb_pcbarc::DrawInWindow(CDC* pDC, double scale, double origin_x, double origin_y, int nEy)
{
	int cx,cy;
	int r;
	COLORREF color = nEDA_PCB_LayerColor[m_nLayer];

	int width;
	width = (int)(m_fWidth/scale);
	if(width < 1) width = 1;

	cx = (int)((m_fCx - origin_x)/scale);
	cy = nEy - (int)((m_fCy - origin_y)/scale);	
	r = (int)(m_fRadius/scale);

	CRect rect;
	CPoint sp, ep;

	rect.left = cx - r;
	rect.right = cx + r;
	rect.top = cy - r;
	rect.bottom = cy + r;


	if((m_fSangle == 0)||(m_fSangle == 360))
	{
		sp.x = cx + r;
		sp.y = cy;
	}
	else if(m_fSangle == 90)
	{
		sp.x = cx;
		sp.y = cy - r;
	}
	else if(m_fSangle == 180)
	{
		sp.x = cx - r;
		sp.y = cy;
	}
	else if(m_fSangle == 270)
	{
		sp.x = cx;
		sp.y = cy + r;
	}
	else
	{
		if((m_fSangle > 0)&&(m_fSangle < 180))
		{
			sp.x = cx + (int)(r/tan(m_fSangle*fEDA_pi/180));
			sp.y = cy - r;
		}
		else
		{
			sp.x = cx - (int)(r/tan(m_fSangle*fEDA_pi/180));
			sp.y = cy + r;
		}
	}

	if((m_fEangle == 0)||(m_fEangle == 360))
	{
		ep.x = cx + r;
		ep.y = cy;
	}
	else if(m_fEangle == 180)
	{
		ep.x = cx - r;
		ep.y = cy;
	}
	else
	{
		if((m_fEangle > 0)&&(m_fEangle < 180))
		{
			ep.x = cx + (int)(r/tan(m_fEangle*fEDA_pi/180));
			ep.y = cy - r;
		}
		else
		{
			ep.x = cx - (int)(r/tan(m_fEangle*fEDA_pi/180));
			ep.y = cy + r;
		}
	}
	
	CPen arcpen;
	arcpen.CreatePen(PS_SOLID, width, color);
	CPen* oldpen = pDC->SelectObject(&arcpen);

	pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);

	pDC->SelectObject(oldpen);
}

void COb_pcbarc::DrawSelection(CDC* pDC, CView* pV)
{
	int cx,cy;
	int r;

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();
	
	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;

	int oldbkmode = pDC->SetBkMode(TRANSPARENT);

	CPen arcpen;
	arcpen.CreatePen(PS_DOT, 1, nEDA_PCB_LayerColor[SELECTIONS]);
	CPen* oldpen = pDC->SelectObject(&arcpen);


	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
	r = (int)(m_fRadius/pView->m_fScale);

	CRect rect;
	CPoint sp, ep, mp;

	rect.left = cx - r;
	rect.right = cx + r;
	rect.top = cy - r;
	rect.bottom = cy + r;

	sp.x = cx + (int)(r*cos(m_fEangle*fEDA_pi/180));
	sp.y = cy - (int)(r*sin(m_fEangle*fEDA_pi/180));

	ep.x = cx + (int)(r*cos(m_fSangle*fEDA_pi/180));
	ep.y = cy - (int)(r*sin(m_fSangle*fEDA_pi/180));

	pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, ep.x, ep.y, sp.x, sp.y);

	pDC->MoveTo(sp.x, sp.y);
	pDC->LineTo(cx, cy);
	pDC->LineTo(ep.x, ep.y);

	double alpha;
	if(m_fEangle > m_fSangle) alpha = (m_fSangle + m_fEangle)/2;
	else alpha = (m_fSangle + m_fEangle)/2 + 180;
	mp.x = cx + (int)(r*cos(alpha*fEDA_pi/180));
	mp.y = cy - (int)(r*sin(alpha*fEDA_pi/180));
	
	pView->DrawSelectionKeyPoint(sp.x, sp.y, pDC);
	pView->DrawSelectionKeyPoint(ep.x, ep.y, pDC);
	pView->DrawSelectionKeyPoint(mp.x, mp.y, pDC);

	pDC->SelectObject(oldpen);
	pDC->SetBkMode(oldbkmode);
}

BOOL COb_pcbarc::PickMe(CView* pV)
{
	int wx1,wy1,wx2,wy2;
	int minx,miny,maxx,maxy;

	CESDView* pView = (CESDView*)pV;
    
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

	minx = (int)(m_fCx - m_fRadius - m_fWidth/2);
	miny = (int)(m_fCy - m_fRadius - m_fWidth/2);
	maxx = (int)(m_fCx + m_fRadius + m_fWidth/2);
	maxy = (int)(m_fCy + m_fRadius + m_fWidth/2);

   	if( ((minx < wx1)&&(maxx < wx1)) || ((minx > wx2)&&(maxx > wx2)) || ((miny < wy1)&&(maxy < wy1)) || ((miny > wy2)&&(maxy > wy2)) )	return FALSE;
	else return TRUE;
}

BOOL  COb_pcbarc::Is_On_Me(double fx, double fy)
{
    CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return FALSE;

	double delta = m_fWidth/2 + nEDA_PcbSelection_Offset;

	CRgn rgn, rgn1, rgn2;
	rgn1.CreateEllipticRgn((int)(m_fCx - (m_fRadius - delta)), (int)(m_fCy - (m_fRadius - delta)), (int)(m_fCx + (m_fRadius - delta)), (int)(m_fCy + (m_fRadius - delta)) );
	rgn2.CreateEllipticRgn((int)(m_fCx - (m_fRadius + delta)), (int)(m_fCy - (m_fRadius + delta)), (int)(m_fCx + (m_fRadius + delta)), (int)(m_fCy + (m_fRadius + delta)) );
	rgn.CreateRectRgn(0,0,50,50);

	rgn.CombineRgn(&rgn1, &rgn2, RGN_XOR);

	if(rgn.PtInRegion((int)fx, (int)fy) != FALSE) 
	{
		double alpha;
		if(fx == m_fCx)
		{
			if(fy > m_fCy) alpha = 90.0f;
			else alpha = 270.0f;
		}
		else if(fy == m_fCy) 
		{
			if(fx > m_fCx) alpha = 0;
			else alpha = 180;
		}
		else
		{
			alpha = 180/fEDA_pi*atan((fy - m_fCy)/(fx - m_fCx));
			if(fx < m_fCx) alpha += 180;
		}
		if(alpha < 0) alpha += 360;

		if(m_fEangle == m_fSangle) return TRUE;
		else if(m_fEangle > m_fSangle)
		{
			if((alpha > m_fSangle)&&(alpha < m_fEangle)) return TRUE;
		}
		else
		{
			if((alpha > m_fSangle)||(alpha < m_fEangle)) return TRUE;
		}
	}
	
	return FALSE;
}


BOOL  COb_pcbarc::Is_Inside_Area(double fx1, double fy1, double fx2, double fy2)
{
	double fminx, fminy, fmaxx, fmaxy;
	fminx = m_fCx - m_fRadius;
	fminy = m_fCy - m_fRadius;
	fmaxx = m_fCx + m_fRadius;
	fmaxy = m_fCy + m_fRadius;
	
	if((fminx < fx1)||(fmaxx > fx2)||(fminy < fy1)||(fmaxy > fy2)) return FALSE;
	return TRUE;
}

void COb_pcbarc::Change(int index)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;

	CDlg_PCBEditArc dlg;

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

	dlg.m_fEditSangle = m_fSangle;
	dlg.m_fEditEangle = m_fEangle;
	dlg.m_csEditWidth = pDoc->Convert_XY_To_String(m_fWidth);
	dlg.m_csEditRadius = pDoc->Convert_XY_To_String(m_fRadius);
	dlg.m_csEditX = pDoc->Convert_XY_To_String(m_fCx  - pDoc->m_cPCB_Sheet.m_fOriginX);
	dlg.m_csEditY = pDoc->Convert_XY_To_String(m_fCy  - pDoc->m_cPCB_Sheet.m_fOriginY);
	dlg.m_bCheckLocked = m_bLocked;
	dlg.m_bCheckKeepout = m_bKeepout;
	
	if(dlg.DoModal() == IDOK)
	{
		//----------- Undo ------------------------------------------------------------------------------------
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}

		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_CHANGE;
		pundo->m_nOb = PCB_ELEMENT_ARC;
		pundo->m_nIndex = index;
		pundo->pcbarc.Copy(this);
			
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		//------------------------------------------------------------------------------------------------------
		

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
		
		m_fSangle = dlg.m_fEditSangle;
		m_fEangle = dlg.m_fEditEangle;
		m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditWidth);
		m_fRadius = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditRadius);
		m_fCx = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditX) + pDoc->m_cPCB_Sheet.m_fOriginX;
		m_fCy = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditY) + pDoc->m_cPCB_Sheet.m_fOriginY;
		m_bLocked = dlg.m_bCheckLocked;
		m_bKeepout = dlg.m_bCheckKeepout;
	}
}

void COb_pcbarc::Copy(COb_pcbarc* ppcbarc)
{
	m_fCx = ppcbarc->m_fCx;
	m_fCy = ppcbarc->m_fCy;
	m_fRadius = ppcbarc->m_fRadius;
	m_fSangle = ppcbarc->m_fSangle;
	m_fEangle = ppcbarc->m_fEangle;
	m_fWidth = ppcbarc->m_fWidth;
	
	m_nLayer = ppcbarc->m_nLayer;
	
	m_bLocked = ppcbarc->m_bLocked;
	m_bKeepout = ppcbarc->m_bKeepout;
	m_bTearDrop = ppcbarc->m_bTearDrop;

	m_nNet = ppcbarc->m_nNet;
	m_nComponent = ppcbarc->m_nComponent;
	m_nPolygon = ppcbarc->m_nPolygon;

	m_bSelection = FALSE; //ppcbarc->m_bSelection;
	m_nFlag = ppcbarc->m_nFlag;

	pDocument = ppcbarc->pDocument;
}

void COb_pcbarc::Move(CClientDC* pDC, CView* pV, int index)
{
	int ch1;
	int flag;
	BOOL clear_undo_buffer_flag = FALSE;
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "移动园弧: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;
	Draw(pDC, pV);

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_pcbarc aarc;
	aarc.Copy(this);
	aarc.pDocument = pDoc;
	aarc.m_nFlag = FLAG_NORMAL;
	aarc.Draw(pDC, pV);

	double fsx, fsy, fex, fey;
	fsx = aarc.m_fCx + aarc.m_fRadius*cos(aarc.m_fSangle*fEDA_pi/180);
	fsy = aarc.m_fCy + aarc.m_fRadius*sin(aarc.m_fSangle*fEDA_pi/180);
	fex = aarc.m_fCx + aarc.m_fRadius*cos(aarc.m_fEangle*fEDA_pi/180);
	fey = aarc.m_fCy + aarc.m_fRadius*sin(aarc.m_fEangle*fEDA_pi/180);

	double d1, d2;
	d1 = sqrt((pView->m_fCurrentSnapX - fsx)*(pView->m_fCurrentSnapX - fsx) + (pView->m_fCurrentSnapY - fsy)*(pView->m_fCurrentSnapY - fsy));
	d2 = sqrt((pView->m_fCurrentSnapX - fex)*(pView->m_fCurrentSnapX - fex) + (pView->m_fCurrentSnapY - fey)*(pView->m_fCurrentSnapY - fey));

	int arcflag = 0;
	if(d1 >= d2)
	{
		arcflag = 2;
		pView->m_fCurrentSnapX = fex;
		pView->m_fCurrentSnapY = fey;
	}
	else
	{
		arcflag = 1;
		pView->m_fCurrentSnapX = fsx;
		pView->m_fCurrentSnapY = fsy;
	}
			
	double x1,y1, x0, y0, deltax, deltay;
	x1 = x0 = pView->m_fCurrentSnapX;
	y1 = y0 = pView->m_fCurrentSnapY;
	deltax = deltay = 0;

	pView->Restore_LargeCrossXY();
	pView->OpenMouseCursor();

	bEDA_NeedElectricalGrid = TRUE;

	pView->Write_Help(szHelp);
	flag = 0;
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->PcbElecHotPointsControl(pDC);

			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
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
				aarc.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}
			
			x1 = pView->m_fCurrentSnapX;  
			y1 = pView->m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;
	
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fCx - x0, m_fCy - y0, 90*flag, FALSE);
			aarc.m_fCx = fx + x0 + deltax;
			aarc.m_fCy = fy + y0 + deltay;

			aarc.m_fSangle = m_fSangle + flag*90;
			aarc.m_fSangle = pDoc->Get_Correct_Angle(aarc.m_fSangle);
			aarc.m_fEangle = m_fEangle + flag*90;
			aarc.m_fEangle = pDoc->Get_Correct_Angle(aarc.m_fEangle);

			
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->PcbElecHotPointsControl(pDC);
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
			pView->OpenMouseCursor();

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fCx - x0, m_fCy - y0, 90*flag, FALSE);
			aarc.m_fCx = fx + x0 + deltax;
			aarc.m_fCy = fy + y0 + deltay;

			aarc.m_fSangle = m_fSangle + flag*90;
			aarc.m_fSangle = pDoc->Get_Correct_Angle(aarc.m_fSangle);
			aarc.m_fEangle = m_fEangle + flag*90;
			aarc.m_fEangle = pDoc->Get_Correct_Angle(aarc.m_fEangle);

			//----------- Undo ----------------------------------------------------------------------------------
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_ARC;
			pundo->m_nIndex = index;
			pundo->pcbarc.Copy(this);
			pundo->pcbarc.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			//---------------------------------------------------------------------------------------------------

			m_fCx = aarc.m_fCx;
			m_fCy = aarc.m_fCy;
			m_fSangle = aarc.m_fSangle;
			m_fEangle = aarc.m_fEangle;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;
			
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, m_fCx - x0, m_fCy - y0, 90*flag, FALSE);
			aarc.m_fCx = fx + x0 + deltax;
			aarc.m_fCy = fy + y0 + deltay;

			aarc.m_fSangle = m_fSangle + flag*90;
			aarc.m_fSangle = pDoc->Get_Correct_Angle(aarc.m_fSangle);
			aarc.m_fEangle = m_fEangle + flag*90;
			aarc.m_fEangle = pDoc->Get_Correct_Angle(aarc.m_fEangle);

			aarc.Draw(pDC, pV);

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

void COb_pcbarc::MoveAndRotate(CView* pV, double x0, double y0,  double deltax, double deltay, int rotation, BOOL mirror, int index)
{
	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();


	CPcbUndo *pundo = new CPcbUndo;
	pundo->m_nOp = UNDO_CHANGE;
	pundo->m_nOb = PCB_ELEMENT_ARC;
	pundo->m_nIndex = index;
	pundo->pcbarc.Copy(this);
	pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

    m_fCx += deltax;
	m_fCy += deltay;

	double sangle, eangle;
	pDoc->Get_Rotate_Angle(&sangle, &eangle, m_fSangle, m_fEangle, rotation, mirror);
	m_fSangle = sangle;
	m_fSangle = pDoc->Get_Correct_Angle(m_fSangle);
	m_fEangle = eangle;
	m_fEangle = pDoc->Get_Correct_Angle(m_fEangle);
}

BOOL COb_pcbarc::MovePlaced(CClientDC* pDC, CView* pV)
{
	int ch1;
	int flag;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	char szHelp[] = "复制园弧: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDView* pView = (CESDView*)pV;
	CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	

	if(pView->m_bCursorState == TRUE) pView->CloseMouseCursor();
	m_nFlag = FLAG_DELETED;

	pView->Invalidate();

	int drawmode = pDC->GetROP2();
	pDC->SetROP2(R2_NOTXORPEN);	

	COb_pcbarc aarc;
	aarc.Copy(this);
	aarc.pDocument = pDoc;
	aarc.m_nFlag = FLAG_NORMAL;
	aarc.Draw(pDC, pV);

	pView->m_fCurrentSnapX = aarc.m_fCx;
	pView->m_fCurrentSnapY = aarc.m_fCy;
	pView->Restore_LargeCrossXY();

	pView->OpenMouseCursor();

	pView->Write_Help(szHelp);
	flag = 0;
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = pView->MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
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
				aarc.Draw(pDC, pV);
				pView->OpenMouseCursor();
			}

			aarc.m_fCx = pView->m_fCurrentSnapX;
			aarc.m_fCy = pView->m_fCurrentSnapY;

			aarc.m_fSangle = m_fSangle + flag*90;
			aarc.m_fSangle = pDoc->Get_Correct_Angle(aarc.m_fSangle);
			aarc.m_fEangle = m_fEangle + flag*90;
			aarc.m_fEangle = pDoc->Get_Correct_Angle(aarc.m_fEangle);

			
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
			pView->OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
			pView->OpenMouseCursor();

			aarc.m_fCx = pView->m_fCurrentSnapX;
			aarc.m_fCy = pView->m_fCurrentSnapY;

			aarc.m_fSangle = m_fSangle + flag*90;
			aarc.m_fSangle = pDoc->Get_Correct_Angle(aarc.m_fSangle);
			aarc.m_fEangle = m_fEangle + flag*90;
			aarc.m_fEangle = pDoc->Get_Correct_Angle(aarc.m_fEangle);

			m_fCx = aarc.m_fCx;
			m_fCy = aarc.m_fCy;
			m_fSangle = aarc.m_fSangle;
			m_fEangle = aarc.m_fEangle;

			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			pView->CloseMouseCursor();
			aarc.Draw(pDC, pV);
			
			flag++;
			if(flag>3) flag=0;

			aarc.m_fCx = pView->m_fCurrentSnapX;
			aarc.m_fCy = pView->m_fCurrentSnapY;

			aarc.m_fSangle = m_fSangle + flag*90;
			aarc.m_fSangle = pDoc->Get_Correct_Angle(aarc.m_fSangle);
			aarc.m_fEangle = m_fEangle + flag*90;
			aarc.m_fEangle = pDoc->Get_Correct_Angle(aarc.m_fEangle);

			aarc.Draw(pDC, pV);

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
	
	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC)) return FALSE;
	else return TRUE;
}

void COb_pcbarc::DrawHighlighted(CDC* pDC, CView* pV, int layer)
{

	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();	
	

	if(pDoc->m_sPCB_LayerParam[m_nLayer].bLayerShow == FALSE) return;
		

	COLORREF color;
	if(layer == CONNECTIONS) color = nEDA_PCB_LayerColor[SELECTIONS];
	else if(layer == DRCERROR) color = nEDA_PCB_LayerColor[DRCERROR];

	int cx, cy, r, width;
	pView->Get_VPort_XY(&cx,&cy,m_fCx,m_fCy);
	r = (int)(m_fRadius/pView->m_fScale);
	width = (int)(m_fWidth/pView->m_fScale);
	if(width < 1) width = 1;

	CRect rect;
	CPoint sp, ep;

	rect.left = cx - r;
	rect.right = cx + r;
	rect.top = cy - r;
	rect.bottom = cy + r;

	if((m_fSangle == 0)||(m_fSangle == 360))
	{
		sp.x = cx + r;
		sp.y = cy;
	}
	else if(m_fSangle == 90)
	{
		sp.x = cx;
		sp.y = cy - r;
	}
	else if(m_fSangle == 180)
	{
		sp.x = cx - r;
		sp.y = cy;
	}
	else if(m_fSangle == 270)
	{
		sp.x = cx;
		sp.y = cy + r;
	}
	else
	{
		if((m_fSangle > 0)&&(m_fSangle < 180))
		{
			sp.x = cx + (int)(r/tan(m_fSangle*fEDA_pi/180));
			sp.y = cy - r;
		}
		else
		{
			sp.x = cx - (int)(r/tan(m_fSangle*fEDA_pi/180));
			sp.y = cy + r;
		}
	}

	if((m_fEangle == 0)||(m_fEangle == 360))
	{
		ep.x = cx + r;
		ep.y = cy;
	}
	else if(m_fEangle == 180)
	{
		ep.x = cx - r;
		ep.y = cy;
	}
	else
	{
		if((m_fEangle > 0)&&(m_fEangle < 180))
		{
			ep.x = cx + (int)(r/tan(m_fEangle*fEDA_pi/180));
			ep.y = cy - r;
		}
		else
		{
			ep.x = cx - (int)(r/tan(m_fEangle*fEDA_pi/180));
			ep.y = cy + r;
		}
	}
	
	CPen arcpen, keepout_pen, *oldpen;
	keepout_pen.CreatePen(PS_SOLID, width, nEDA_PCB_LayerColor[m_nLayer]);
	oldpen = pDC->SelectObject(&keepout_pen);

	pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);

	if(width > 20) width -= 8;
	else if(width > 12) width -= 6;
	else if(width > 8 ) width -= 4;
	else if(width > 3 ) width -= 2;
	else width = 1;

	arcpen.CreatePen(PS_SOLID, width, color);
	oldpen = pDC->SelectObject(&arcpen);

	pDC->Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);

	pDC->SelectObject(oldpen);		
}

void COb_pcbarc::GetArcVertexOutline( CArray<Struct_VertexOutline, Struct_VertexOutline&>& arrayVertexOutline, double drc)
{
	CESDDoc* pDoc = (CESDDoc*)pDocument;
	
	if(m_fRadius == 0) return;

	double fsx, fsy, fex, fey;
	fsx = m_fCx + (m_fRadius*cos(m_fSangle*fEDA_pi/180));
	fsy = m_fCy + (m_fRadius*sin(m_fSangle*fEDA_pi/180));
	
	fex = m_fCx + (m_fRadius*cos(m_fEangle*fEDA_pi/180));
	fey = m_fCy + (m_fRadius*sin(m_fEangle*fEDA_pi/180));
	
	double r = m_fWidth/2 + drc + 2;  

	Struct_VertexOutline vertexoutline;
	vertexoutline.kind = 1;	
	if((m_fSangle == m_fEangle)||(abs(m_fSangle - m_fEangle) == 360))
	{
		vertexoutline.cx = m_fCx;
		vertexoutline.cy = m_fCy;
		vertexoutline.r = m_fRadius + r;
		vertexoutline.sangle = 0;
		vertexoutline.eangle = 360;
		vertexoutline.x1 = vertexoutline.cx + vertexoutline.r;
		vertexoutline.y1 = vertexoutline.cy;
		vertexoutline.x2 = vertexoutline.cx + vertexoutline.r;
		vertexoutline.y2 = vertexoutline.cy;
		arrayVertexOutline.Add(vertexoutline);

		vertexoutline.cx = m_fCx;
		vertexoutline.cy = m_fCy;
		vertexoutline.r = m_fRadius - r;
		vertexoutline.sangle = 0;
		vertexoutline.eangle = 360;
		vertexoutline.x1 = vertexoutline.cx + vertexoutline.r;
		vertexoutline.y1 = vertexoutline.cy;
		vertexoutline.x2 = vertexoutline.cx + vertexoutline.r;
		vertexoutline.y2 = vertexoutline.cy;
		arrayVertexOutline.Add(vertexoutline);
	}
	else
	{

		double sx1, sy1, ex1, ey1;
		double sx2, sy2, ex2, ey2;
		sx1 = m_fCx + (m_fRadius - r)*cos(m_fSangle*fEDA_pi/180);
		sy1 = m_fCy + (m_fRadius - r)*sin(m_fSangle*fEDA_pi/180);
		ex1 = m_fCx + (m_fRadius - r)*cos(m_fEangle*fEDA_pi/180);
		ey1 = m_fCy + (m_fRadius - r)*sin(m_fEangle*fEDA_pi/180);

		sx2 = m_fCx + (m_fRadius + r)*cos(m_fSangle*fEDA_pi/180);
		sy2 = m_fCy + (m_fRadius + r)*sin(m_fSangle*fEDA_pi/180);
		ex2 = m_fCx + (m_fRadius + r)*cos(m_fEangle*fEDA_pi/180);
		ey2 = m_fCy + (m_fRadius + r)*sin(m_fEangle*fEDA_pi/180);

		vertexoutline.cx = fsx;
		vertexoutline.cy = fsy;
		vertexoutline.r = r;
		vertexoutline.sangle = 180 + m_fSangle;
		vertexoutline.sangle = pDoc->Get_Correct_Angle(vertexoutline.sangle);
		vertexoutline.eangle = m_fSangle;
		vertexoutline.x1 = sx1;		vertexoutline.y1 = sy1;	
		vertexoutline.x2 = sx2;		vertexoutline.y2 = sy2;	
		arrayVertexOutline.Add(vertexoutline);

		vertexoutline.cx = m_fCx;
		vertexoutline.cy = m_fCy;
		vertexoutline.r = m_fRadius + r;
		vertexoutline.sangle = m_fSangle;
		vertexoutline.eangle = m_fEangle;
		vertexoutline.x1 = sx2;		vertexoutline.y1 = sy2;	
		vertexoutline.x2 = ex2;		vertexoutline.y2 = ey2;	
		arrayVertexOutline.Add(vertexoutline);

		vertexoutline.cx = fex;
		vertexoutline.cy = fey;
		vertexoutline.r = r;
		vertexoutline.sangle = m_fEangle;
		vertexoutline.eangle = 180 + m_fEangle;
		vertexoutline.eangle = pDoc->Get_Correct_Angle(vertexoutline.eangle);
		vertexoutline.x1 = ex2;		vertexoutline.y1 = ey2;	
		vertexoutline.x2 = ex1;		vertexoutline.y2 = ey1;	
		arrayVertexOutline.Add(vertexoutline);

		vertexoutline.cx = m_fCx;
		vertexoutline.cy = m_fCy;
		vertexoutline.r = m_fRadius - r;
		vertexoutline.sangle = m_fSangle;
		vertexoutline.eangle = m_fEangle;
		vertexoutline.x1 = sx1;		vertexoutline.y1 = sy1;	
		vertexoutline.x2 = ex1;		vertexoutline.y2 = ey1;	
		arrayVertexOutline.Add(vertexoutline);
	}
}