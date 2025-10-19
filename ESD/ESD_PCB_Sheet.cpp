#include "stdafx.h"

#include "ESD.h"
#include "mainfrm.h"
#include "ESDDoc.h"
#include "ESDView.h"
#include "childfrm.h"
#include "sysparam.h"
#include "systemext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Component

IMPLEMENT_SERIAL(COb_pcbsheet, CObject, 0)

COb_pcbsheet::COb_pcbsheet()
{
	//Float Origin
	m_fOriginX = 0;
	m_fOriginY = 0;

	//Grid
	m_fBigVisibleGridSize = 100.0f;
	m_fVisibleGridSize = 5.0f;
	m_fElecGridRange = 8.0f;
	m_bElecGridEnable = TRUE;
	m_bAllLayersElecGrid = TRUE;
	m_fSnapGridSize = 5.0f;
	m_fSnapGridSizeX = 5.0f;
	m_fSnapGridSizeY = 5.0f;
	m_fTrackGridSize = 20.0f;
	m_fViaGridSize = 20.0f;
	m_fComponentGridSize = 20.0f;
	m_fComponentGridSizeX = 20.0f;
	m_fComponentGridSizeY = 20.0f;
	
	//网格Grid或点Grid
	m_bDotGrid = FALSE;
	
	//公英制单位	
	m_nUnit = UNIT_PCB_I;

	//Designator显示模式
	m_nDesignatorDisplayMode = 0;

	//Sheet primitive lock
	m_bPrimitiveLock = TRUE;

	//Border Outline的vertix集合
	m_cVertex.SetSize(5);
	m_cVertex[0].fx = 1000.0f;
	m_cVertex[0].fy = 1000.0f;
	m_cVertex[1].fx = 1000.0f;
	m_cVertex[1].fy = 5000.0f;
	m_cVertex[2].fx = 7000.0f;
	m_cVertex[2].fy = 5000.0f;
	m_cVertex[3].fx = 7000.0f;
	m_cVertex[3].fy = 1000.0f;
	m_cVertex[4].fx = 1000.0f;
	m_cVertex[4].fy = 1000.0f;

	//Sheet Size
	m_fSheetX = 0.0f;
	m_fSheetY = 0.0f;
	m_fSheetWidth = 9000.0f;
	m_fSheetHeight = 7000.0f;
	
	m_bShowSheet = FALSE;
	m_bLockSheet = TRUE;

	//图框数据
}

COb_pcbsheet::~COb_pcbsheet()
{
	while(!m_arraySheetTrack.IsEmpty())
	{
		delete m_arraySheetTrack.GetAt(0);
		m_arraySheetTrack.RemoveAt(0);
	}
	while(!m_arraySheetText.IsEmpty())
	{
		delete m_arraySheetText.GetAt(0);
		m_arraySheetText.RemoveAt(0);
	}
}

void COb_pcbsheet::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_fOriginX;
		ar << m_fOriginY;
		ar << m_fBigVisibleGridSize;
		ar << m_fVisibleGridSize;
		ar << m_fElecGridRange;
		ar << m_bElecGridEnable;
		ar << m_fSnapGridSize;
		ar << m_fSnapGridSizeX;
		ar << m_fSnapGridSizeY;
		ar << m_fTrackGridSize;
		ar << m_fViaGridSize;
		ar << m_fComponentGridSize;
		ar << m_fComponentGridSizeX;
		ar << m_fComponentGridSizeY;
		
		ar << m_bDotGrid;
		ar << m_nUnit;
		ar << m_nDesignatorDisplayMode;
		ar << m_bPrimitiveLock;

		int num = m_cVertex.GetCount();
		ar << num;
		for(int i=0; i<num; i++)
		{
			Struct_RegionVertex vertex;
			vertex = m_cVertex.GetAt(i);
			ar << vertex.cx;
			ar << vertex.cy;
			ar << vertex.eangle;
			ar << vertex.fx;
			ar << vertex.fy;
			ar << vertex.radius;
			ar << vertex.sangle;
		}

		ar << m_fSheetX;
		ar << m_fSheetY;
		ar << m_fSheetWidth;
		ar << m_fSheetHeight;
		ar << m_bShowSheet;
		ar << m_bLockSheet;
	
		//track
		num = m_arraySheetTrack.GetCount();
		ar << num;
		for(int i=0; i<num; i++)
		{
			COb_pcbtrack* ptrack = m_arraySheetTrack.GetAt(i);
			ptrack->Serialize(ar);
		}
		//text
		num = m_arraySheetText.GetCount();
		ar << num;
		for(int i=0; i<num; i++)
		{
			COb_pcbtext* ptext = m_arraySheetText.GetAt(i);
			ptext->Serialize(ar);
		}
	}
	else
	{
		ar >> m_fOriginX;
		ar >> m_fOriginY;
		ar >> m_fBigVisibleGridSize;
		ar >> m_fVisibleGridSize;
		ar >> m_fElecGridRange;
		ar >> m_bElecGridEnable;
		ar >> m_fSnapGridSize;
		ar >> m_fSnapGridSizeX;
		ar >> m_fSnapGridSizeY;
		ar >> m_fTrackGridSize;
		ar >> m_fViaGridSize;
		ar >> m_fComponentGridSize;
		ar >> m_fComponentGridSizeX;
		ar >> m_fComponentGridSizeY;
		
		ar >> m_bDotGrid;
		ar >> m_nUnit;
		ar >> m_nDesignatorDisplayMode;
		ar >> m_bPrimitiveLock;

		m_cVertex.RemoveAll();
		int num;
		ar >> num;
		for(int i=0; i<num; i++)
		{
			Struct_RegionVertex vertex;
			ar >> vertex.cx;
			ar >> vertex.cy;
			ar >> vertex.eangle;
			ar >> vertex.fx;
			ar >> vertex.fy;
			ar >> vertex.radius;
			ar >> vertex.sangle;
			m_cVertex.Add(vertex);
		}

		ar >> m_fSheetX;
		ar >> m_fSheetY;
		ar >> m_fSheetWidth;
		ar >> m_fSheetHeight;
		ar >> m_bShowSheet;
		ar >> m_bLockSheet;

		//track
		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbtrack* ptrack = new COb_pcbtrack();
			ptrack->Serialize(ar);
			m_arraySheetTrack.Add(ptrack);
		}
		//text
		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbtext* ptext = new COb_pcbtext();
			ptext->Serialize(ar);
			m_arraySheetText.Add(ptext);
		}

	}
}

void COb_pcbsheet::Draw(CDC* pDC, CView* pV)
{	
	COb_pcbtrack pcbtrack;
	CESDView* pView = (CESDView*)pV;
    CESDDoc* pDoc = (CESDDoc*)pView->GetDocument();

    //Sheet
	if(m_bShowSheet == TRUE)
	{
		int x1,y1,x2, y2;
		RECT rect;
		pView->Get_VPort_XY(&x1,&y1,m_fSheetX,m_fSheetY);
		pView->Get_VPort_XY(&x2,&y2,m_fSheetX+m_fSheetWidth,m_fSheetY+m_fSheetHeight);

		rect.left = x1;
		rect.bottom = y1;
		rect.right = x2;
		rect.top = y2;

		pDC->FillSolidRect(&rect,nEDA_PCB_SheetAreaColor);

		COLORREF colorbak = nEDA_PCB_LayerColor[SELECTIONS];
		nEDA_PCB_LayerColor[SELECTIONS] = nEDA_PCB_SheetLineColor;

		for(int i=0; i<m_arraySheetTrack.GetCount(); i++)
		{
			COb_pcbtrack* ptrack = m_arraySheetTrack.GetAt(i);
			COb_pcbtrack atrack;
			atrack.Copy(ptrack);
			atrack.pDocument = pDoc;
			atrack.m_fX1 += m_fSheetX;
			atrack.m_fY1 += m_fSheetY;
			atrack.m_fX2 += m_fSheetX;
			atrack.m_fY2 += m_fSheetY;
			atrack.Draw(pDC, pV);
		}
		for(int i=0; i<m_arraySheetText.GetCount(); i++)
		{
			COb_pcbtext* ptext = m_arraySheetText.GetAt(i);
			COb_pcbtext atext;
			atext.Copy(ptext);
			atext.pDocument = pDoc;
			atext.m_fSx += m_fSheetX;
			atext.m_fSy += m_fSheetY;
			atext.UpdateStringBorder(pV, FALSE);
			atext.Draw(pDC, pV);
		}
		nEDA_PCB_LayerColor[SELECTIONS] = colorbak;
	}

	if(pView->m_nPrint_Flag == 0)
	{
		COLORREF line_color, area_color;

		line_color = nEDA_PCB_BoardLineColor;
		area_color = nEDA_PCB_BoardAreaColor;
		
		int vertex = m_cVertex.GetCount();
		CPoint point;
		CPoint* lpPoint  = (CPoint*)malloc(sizeof(point)*vertex);

		for(int i=0; i<m_cVertex.GetCount(); i++)
		{
			int x,y;
			pView->Get_VPort_XY(&x, &y, m_cVertex[i].fx, m_cVertex[i].fy);
			lpPoint[i].x = x;
			lpPoint[i].y = y;
		}
		
		CRgn rgn;
		rgn.CreatePolygonRgn( lpPoint, vertex, ALTERNATE);
		free(lpPoint);
		
		for(int i=0; i<vertex; i++)
		{
			CRgn rgn1, rgn2;
			rgn2.CreateRectRgn( 0, 0, 50, 50 );

			if(m_cVertex[i].radius == 0) continue;
			int cx,cy,r;
			pView->Get_VPort_XY(&cx, &cy, m_cVertex[i].cx, m_cVertex[i].cy);
			r = (int)(m_cVertex[i].radius/pView->m_fScale);	
			rgn1.CreateEllipticRgn(cx - r, cy - r, cx + r, cy + r);
			
			if(rgn.PtInRegion(cx, cy) != FALSE)
			{
				rgn2.CombineRgn(&rgn, &rgn1, RGN_OR);
			}
			else
			{
				rgn2.CombineRgn(&rgn, &rgn1, RGN_DIFF);
			}
			rgn.CopyRgn(&rgn2);
		}
		
		CBrush fillbrush;
		fillbrush.CreateSolidBrush(area_color);

		pDC->FillRgn(&rgn, &fillbrush);

		double minfx, minfy, maxfx, maxfy;
		GetBoardRect(&minfx, &minfy, &maxfx, &maxfy);

		pcbtrack.m_fWidth = 0.01f;
		pcbtrack.m_nLayer = VISIBLEGRID1;	
		if(pDoc->m_sPCB_LayerParam[VISIBLEGRID1].bLayerShow == TRUE)
		{
			if(m_bDotGrid == FALSE)
			{
				if( (int)( m_fVisibleGridSize/pView->m_fScale) >= 10)
				{
					int delta = 0;
					for(;;)
					{
						double gx = m_fOriginX + delta*m_fVisibleGridSize;
						if( (gx < 0) || (gx > 100000.0f) ) break;
						if( (gx >= minfx) && (gx <= maxfx) )
						{
							pcbtrack.m_fX1 = gx;
							pcbtrack.m_fY1 = minfy;
							pcbtrack.m_fX2 = gx;
							pcbtrack.m_fY2 = maxfy;

							if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);
						}
						delta++;
					}

					delta = 0;
					for(;;)
					{
						double gx = m_fOriginX - delta*m_fVisibleGridSize;
						if( (gx < 0) || (gx > 100000.0f) ) break;
						if( (gx >= minfx) && (gx <= maxfx) )
						{
							pcbtrack.m_fX1 = gx;
							pcbtrack.m_fY1 = minfy;
							pcbtrack.m_fX2 = gx;
							pcbtrack.m_fY2 = maxfy;

							if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);
						}
						delta++;
					}

					delta = 0;
					for(;;)
					{
						double gy = m_fOriginY + delta*m_fVisibleGridSize;
						if( (gy < 0) || (gy > 100000.0f) ) break;
						if( (gy >= minfy) && (gy <= maxfy) )
						{
							pcbtrack.m_fX1 = minfx;
							pcbtrack.m_fY1 = gy;
							pcbtrack.m_fX2 = maxfx;
							pcbtrack.m_fY2 = gy;

							if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);
						}
						delta++;
					}

					delta = 0;
					for(;;)
					{
						double gy = m_fOriginY - delta*m_fVisibleGridSize;
						if( (gy < 0) || (gy > 100000.0f) ) break;
						if( (gy >= minfy) && (gy <= maxfy) )
						{
							pcbtrack.m_fX1 = minfx;
							pcbtrack.m_fY1 = gy;
							pcbtrack.m_fX2 = maxfx;
							pcbtrack.m_fY2 = gy;

							if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);
						}
						delta++;
					}				
				}
			}
			else
			{
				if( (int)( m_fVisibleGridSize/pView->m_fScale) >= 10)
				{
					
					int delta_x=0,delta_y=0;
					int x, y;
					double gx, gy;
					gx = m_fOriginX + delta_x*m_fVisibleGridSize;				

					delta_y = 0;
					for(;;)
					{
						gy = m_fOriginY + delta_y*m_fVisibleGridSize;
						if( (gy < 0) || (gy > 100000.0f) ) break;
						if( (gy >= minfy) && (gy <= maxfy) )
						{
							pView->Get_VPort_XY(&x, &y, gx, gy);
							if((y >= 0)&&(y <= pView->m_nViewport_Ey))
							{
								delta_x = 0;
								for(;;)	
								{
									gx = m_fOriginX + delta_x*m_fVisibleGridSize;
									if( (gx < 0) || (gx > 100000.0f) ) break;
									if( (gx >= minfx) && (gx <= maxfx) )
									{
										pView->Get_VPort_XY(&x, &y, gx, gy);
										if((x >= 0)&&(x <= pView->m_nViewport_Ex))
										{
											pDC->SetPixel(x, y, nEDA_PCB_LayerColor[VISIBLEGRID1]);
										}
									}
									delta_x++;
								}
								
								delta_x = 0;
								for(;;)	
								{
									gx = m_fOriginX - delta_x*m_fVisibleGridSize;
									if( (gx < 0) || (gx > 100000.0f) ) break;
									if( (gx >= minfx) && (gx <= maxfx) )
									{
										pView->Get_VPort_XY(&x, &y, gx, gy);
										if((x >= 0)&&(x <= pView->m_nViewport_Ex))
										{
											pDC->SetPixel(x, y, nEDA_PCB_LayerColor[VISIBLEGRID1]);
										}
									}
									delta_x++;
								}
							}
						}
						delta_y++;
					}
					
					delta_y = 0;
					for(;;)
					{
						gy = m_fOriginY - delta_y*m_fVisibleGridSize;
						if( (gy < 0) || (gy > 100000.0f) ) break;
						if( (gy >= minfy) && (gy <= maxfy) )
						{
							pView->Get_VPort_XY(&x, &y, gx, gy);
							if((y >= 0)&&(y <= pView->m_nViewport_Ey))
							{
								delta_x = 0;
								for(;;)	
								{
									gx = m_fOriginX + delta_x*m_fVisibleGridSize;
									if( (gx < 0) || (gx > 100000.0f) ) break;
									if( (gx >= minfx) && (gx <= maxfx) )
									{
										int x, y;
										pView->Get_VPort_XY(&x, &y, gx, gy);
										if((x >= 0)&&(x <= pView->m_nViewport_Ex))
										{
											pDC->SetPixel(x, y, nEDA_PCB_LayerColor[VISIBLEGRID1]);
										}
									}
									delta_x++;
								}
								
								delta_x = 0;
								for(;;)	
								{
									gx = m_fOriginX - delta_x*m_fVisibleGridSize;
									if( (gx < 0) || (gx > 100000.0f) ) break;
									if( (gx >= minfx) && (gx <= maxfx) )
									{
										int x, y;
										pView->Get_VPort_XY(&x, &y, gx, gy);
										if((x >= 0)&&(x <= pView->m_nViewport_Ex))
										{
											pDC->SetPixel(x, y, nEDA_PCB_LayerColor[VISIBLEGRID1]);
										}
									}
									delta_x++;
								}
							}
						}
						delta_y++;
					}
				}
			}
		}

		pcbtrack.m_fWidth = 0.01f;
		pcbtrack.m_nLayer = VISIBLEGRID2;
		if(pDoc->m_sPCB_LayerParam[VISIBLEGRID2].bLayerShow == TRUE)
		{
			if(m_bDotGrid == FALSE)
			{
				if( (int)( m_fBigVisibleGridSize/pView->m_fScale) >= 10)
				{
					int delta = 0;
					for(;;)
					{
						double gx = m_fOriginX + delta*m_fBigVisibleGridSize;
						if( (gx < 0) || (gx > 100000.0f) ) break;
						if( (gx >= minfx) && (gx <= maxfx) )
						{
							pcbtrack.m_fX1 = gx;
							pcbtrack.m_fY1 = minfy;
							pcbtrack.m_fX2 = gx;
							pcbtrack.m_fY2 = maxfy;

							if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);
						}
						delta++;
					}

					delta = 0;
					for(;;)
					{
						double gx = m_fOriginX - delta*m_fBigVisibleGridSize;
						if( (gx < 0) || (gx > 100000.0f) ) break;
						if( (gx >= minfx) && (gx <= maxfx) )
						{
							pcbtrack.m_fX1 = gx;
							pcbtrack.m_fY1 = minfy;
							pcbtrack.m_fX2 = gx;
							pcbtrack.m_fY2 = maxfy;

							if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);
						}
						delta++;
					}

					delta = 0;
					for(;;)
					{
						double gy = m_fOriginY + delta*m_fBigVisibleGridSize;
						if( (gy < 0) || (gy > 100000.0f) ) break;
						if( (gy >= minfy) && (gy <= maxfy) )
						{
							pcbtrack.m_fX1 = minfx;
							pcbtrack.m_fY1 = gy;
							pcbtrack.m_fX2 = maxfx;
							pcbtrack.m_fY2 = gy;

							if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);
						}
						delta++;
					}

					delta = 0;
					for(;;)
					{
						double gy = m_fOriginY - delta*m_fBigVisibleGridSize;
						if( (gy < 0) || (gy > 100000.0f) ) break;
						if( (gy >= minfy) && (gy <= maxfy) )
						{
							pcbtrack.m_fX1 = minfx;
							pcbtrack.m_fY1 = gy;
							pcbtrack.m_fX2 = maxfx;
							pcbtrack.m_fY2 = gy;

							if(pcbtrack.PickMe(pV) == TRUE) pcbtrack.Draw(pDC, pV);
						}
						delta++;
					}
				}
			}
			else
			{
				if( (int)( m_fBigVisibleGridSize/pView->m_fScale) >= 10)
				{
					
					int delta_x=0,delta_y=0;
					int x,y;
					double gx, gy;
					gx = m_fOriginX + delta_x*m_fBigVisibleGridSize;

					delta_y = 0;
					for(;;)
					{
						gy = m_fOriginY + delta_y*m_fBigVisibleGridSize;
						if( (gy < 0) || (gy > 100000.0f) ) break;
						if( (gy >= minfy) && (gy <= maxfy) )
						{
							pView->Get_VPort_XY(&x, &y, gx, gy);
							if((y >= 0)&&(y <= pView->m_nViewport_Ey))
							{
								delta_x = 0;
								for(;;)	
								{
									gx = m_fOriginX + delta_x*m_fBigVisibleGridSize;
									if( (gx < 0) || (gx > 100000.0f) ) break;
									if( (gx >= minfx) && (gx <= maxfx) )
									{
										pView->Get_VPort_XY(&x, &y, gx, gy);
										if((x >= 0)&&(x <= pView->m_nViewport_Ex))
										{
											pDC->SetPixel(x, y, nEDA_PCB_LayerColor[VISIBLEGRID2]);
										}
									}
									delta_x++;
								}
								
								delta_x = 0;
								for(;;)	
								{
									gx = m_fOriginX - delta_x*m_fBigVisibleGridSize;
									if( (gx < 0) || (gx > 100000.0f) ) break;
									if( (gx >= minfx) && (gx <= maxfx) )
									{
										pView->Get_VPort_XY(&x, &y, gx, gy);
										if((x >= 0)&&(x <= pView->m_nViewport_Ex))
										{
											pDC->SetPixel(x, y, nEDA_PCB_LayerColor[VISIBLEGRID2]);
										}
									}
									delta_x++;
								}
							}
						}
						delta_y++;
					}
					
					delta_y = 0;
					for(;;)
					{
						gy = m_fOriginY - delta_y*m_fBigVisibleGridSize;
						if( (gy < 0) || (gy > 100000.0f) ) break;
						if( (gy >= minfy) && (gy <= maxfy) )
						{
							pView->Get_VPort_XY(&x, &y, gx, gy);
							if((y >= 0)&&(y <= pView->m_nViewport_Ey))
							{
								delta_x = 0;
								for(;;)	
								{
									gx = m_fOriginX + delta_x*m_fBigVisibleGridSize;
									if( (gx < 0) || (gx > 100000.0f) ) break;
									if( (gx >= minfx) && (gx <= maxfx) )
									{
										int x, y;
										pView->Get_VPort_XY(&x, &y, gx, gy);
										if((x >= 0)&&(x <= pView->m_nViewport_Ex)) pDC->SetPixel(x, y, nEDA_PCB_LayerColor[VISIBLEGRID2]);

									}
									delta_x++;
								}
								
								delta_x = 0;
								for(;;)	
								{
									gx = m_fOriginX - delta_x*m_fBigVisibleGridSize;
									if( (gx < 0) || (gx > 100000.0f) ) break;
									if( (gx >= minfx) && (gx <= maxfx) )
									{
										int x, y;
										pView->Get_VPort_XY(&x, &y, gx, gy);
										if((x >= 0)&&(x <= pView->m_nViewport_Ex)) pDC->SetPixel(x, y, nEDA_PCB_LayerColor[VISIBLEGRID2]);

									}
									delta_x++;
								}
							}
						}
						delta_y++;
					}
				}
			}
		}
	}
}

void COb_pcbsheet::GetBoardRect(double* fx1, double* fy1, double* fx2, double* fy2)
{
	int vertex_num = m_cVertex.GetSize();
	double minfx = 1000000.0f;
	double minfy = 1000000.0f;
	double maxfx = -1000000.0f;
	double maxfy = -1000000.0f;
	for(int i=0; i<m_cVertex.GetSize(); i++)
	{
		minfx = MIN(minfx, m_cVertex[i].fx);
		minfy = MIN(minfy, m_cVertex[i].fy);
		maxfx = MAX(maxfx, m_cVertex[i].fx);
		maxfy = MAX(maxfy, m_cVertex[i].fy);
	}

	*fx1 = minfx;	*fy1 = minfy;
	*fx2 = maxfx;	*fy2 = maxfy;
}