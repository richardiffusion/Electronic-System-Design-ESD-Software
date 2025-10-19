
int CESDView::MouseKey()
{
	MSG mymsg, yourmsg;
	int inchar = 0;
	CPoint point;

    CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return TRUE;

	CClientDC dc(this);

	
	bEDA_MouseLButtonDown = FALSE;

	
	RECT rect;
	rect.left = 0; rect.top = 0;
	rect.right = m_nViewport_Ex; rect.bottom = m_nViewport_Ey;
	ClientToScreen(&rect);
	
	ClipCursor(&rect);
	m_bClipState = TRUE;

	do
	{
		if(!PeekMessage(&mymsg,NULL,0,0,PM_NOREMOVE)) return 0;
		
		
		if(mymsg.message == 0x20a)
		{
			PeekMessage(&yourmsg,NULL,0,0,PM_REMOVE);
			
			//DispatchMessage(&mymsg);
			continue;
			
		}

		
		if( (mymsg.message == WM_MBUTTONDOWN)||(mymsg.message == WM_MBUTTONUP)||(mymsg.message == WM_MBUTTONDBLCLK) )
		{
			PeekMessage(&yourmsg,NULL,0,0,PM_REMOVE);
			continue;
		}

		if(mymsg.message == WM_LBUTTONDBLCLK) 
		{
			
			PeekMessage(&yourmsg,NULL,0,0,PM_REMOVE);
			continue;
		}

		
		if(mymsg.message != WM_KEYDOWN)
		{
			if((mymsg.message == WM_SYSKEYDOWN)||(mymsg.message == WM_SYSKEYUP))
			{
				PeekMessage(&yourmsg,NULL,0,0,PM_REMOVE);
				continue;
			}
			if(mymsg.message == WM_MOUSEMOVE)
			{
				PeekMessage(&yourmsg,NULL,0,0,PM_REMOVE);
				point.x = LOWORD(mymsg.lParam); 
				point.y = HIWORD(mymsg.lParam); 
				if( (point.x >= 0						)&&    
					(point.x <= ((int)m_nViewport_Ex)	)&&
					(point.y >= 0						)&&
					(point.y <= ((int)m_nViewport_Ey)	) )
				{
					if(m_bActive == FALSE) return 0;
	
					if(nEDA_Set_Mouse_Pos == TRUE) 
					{
						nEDA_Set_Mouse_Pos = FALSE;
						return 0;
					}

					if(	m_bCursorState == FALSE ) 
					{
						Get_Current_XY(point.x,point.y);
						//OpenMouseCursor();
						Get_LargeCross_XY();   
						m_CursorLocation.x = nEDA_LargeCrossX; 
						m_CursorLocation.y = nEDA_LargeCrossY; 
						DrawMyCursor(&dc, &m_CursorLocation); 
						m_bCursorState = TRUE;
					}
					else
					{	
						if((point.x <= 1)&&(m_fScale != fEDA_ScaleMax))
						{
							Pan_Left();
						}
						else if((point.y <= 1)&&(m_fScale != fEDA_ScaleMax))
						{
							Pan_Up();
						}
						else if((point.x >= (m_nViewport_Ex-1))&&(m_fScale != fEDA_ScaleMax))
						{
							Pan_Right();
						}
						else if((point.y >= (m_nViewport_Ey-1))&&(m_fScale != fEDA_ScaleMax))
						{
							Pan_Down();
						}

						//if(m_bCursorState == TRUE ) CloseMouseCursor();
						m_CursorLocation.x = nEDA_LargeCrossX; 
						m_CursorLocation.y = nEDA_LargeCrossY; 
						DrawMyCursor(&dc, &m_CursorLocation); 

						Get_Current_XY(point.x,point.y);

						Get_LargeCross_XY();
						m_CursorLocation.x = nEDA_LargeCrossX; 
						m_CursorLocation.y = nEDA_LargeCrossY; 
						DrawMyCursor(&dc, &m_CursorLocation); 
						//if(m_bCursorState == FALSE ) OpenMouseCursor();

						Write_XY(m_fCurrentSnapX, m_fCurrentSnapY);		
						return MOUSEMOVE;
					}
				}			
			}
			if(mymsg.message == WM_RBUTTONUP)
			{
				PeekMessage(&yourmsg,NULL,0,0,PM_REMOVE);
				return PUSHMOUSEESC;
			}
			else if(mymsg.message == WM_RBUTTONDOWN)
			{
				PeekMessage(&yourmsg,NULL,0,0,PM_REMOVE);
				//return PUSHMOUSEESC;
			}
			else if(mymsg.message == WM_LBUTTONDOWN)
			{
				PeekMessage(&yourmsg,NULL,0,0,PM_REMOVE);
				return PUSHMOUSE;
			}
			else if(mymsg.message == WM_LBUTTONUP)
			{
				PeekMessage(&yourmsg,NULL,0,0,PM_REMOVE);
				return MOUSE_LBUTTON_UP;
			}
			else
			{
				PeekMessage(&yourmsg,NULL,0,0,PM_REMOVE);			
				
				DispatchMessage(&mymsg);
			}
		}
		else 
		{
			PeekMessage(&yourmsg,NULL,0,0,PM_REMOVE);
			int vkey = (TCHAR)mymsg.wParam;    
			if((vkey == VK_F6)||(vkey == VK_PRIOR)) 
			{
				OnViewZoom_In();
				return MOUSEMOVE;
			}
			else if((vkey == VK_F5)||(vkey == VK_NEXT)) 
			{
				OnViewZoom_Out();
				return MOUSEMOVE;
			}
			else if(vkey == VK_F10) 
			{
				OnViewZoomCenter();
				return MOUSEMOVE;
			}
			else if((vkey == VK_HOME)||(vkey == VK_END)) 
			{
				OnViewZoomRedraw();
				return MOUSEMOVE;
			}
			else if(vkey == VK_LEFT)  
			{
				if((GetKeyState(VK_SHIFT)&0x80) != 0) 
				{
					if(m_nSystem_State == SCH)	m_fCurrentSnapX -= 10*pDoc->m_fSCH_SnapGrid;
					else m_fCurrentSnapX -= 10*pDoc->m_cPCB_Sheet.m_fSnapGridSize;

					if(m_fCurrentSnapX < 0) m_fCurrentSnapX = 0;
					Restore_LargeCrossXY();
					if(m_fCurrentSnapX < m_fScreenX)  Pan_Left();
				}
				else
				{
					if(m_nSystem_State == SCH)	m_fCurrentSnapX -= pDoc->m_fSCH_SnapGrid;
					else m_fCurrentSnapX -= pDoc->m_cPCB_Sheet.m_fSnapGridSize;

					if(m_fCurrentSnapX < 0) m_fCurrentSnapX = 0;
					Restore_LargeCrossXY();
					if(m_fCurrentSnapX < m_fScreenX)  Pan_Left();
				}
				return MOUSEMOVE;
			}
			else if(vkey == VK_RIGHT)
			{	
				if((GetKeyState(VK_SHIFT)&0x80) != 0)  
				{
					if(m_nSystem_State == SCH)
					{
						m_fCurrentSnapX += 10*pDoc->m_fSCH_SnapGrid;
						if(m_fCurrentSnapX > fEDA_SCH_SnapMaxx) m_fCurrentSnapX = fEDA_SCH_SnapMaxx;
					}
					else
					{
						m_fCurrentSnapX += 10*pDoc->m_cPCB_Sheet.m_fSnapGridSize;
						if(m_fCurrentSnapX > fEDA_PCB_SnapMaxx) m_fCurrentSnapX = fEDA_PCB_SnapMaxx;
					}
			
					Restore_LargeCrossXY();
					if(m_fCurrentSnapX >(m_fScreenX + m_nViewport_Ex*m_fScale)) Pan_Right();			
				}
				else
				{
					if(m_nSystem_State == SCH)
					{
						m_fCurrentSnapX += pDoc->m_fSCH_SnapGrid;
						if(m_fCurrentSnapX > fEDA_SCH_SnapMaxx) m_fCurrentSnapX = fEDA_SCH_SnapMaxx;
					}
					else
					{
						m_fCurrentSnapX += pDoc->m_cPCB_Sheet.m_fSnapGridSize;
						if(m_fCurrentSnapX > fEDA_PCB_SnapMaxx) m_fCurrentSnapX = fEDA_PCB_SnapMaxx;
					}
					Restore_LargeCrossXY();
					if(m_fCurrentSnapX >(m_fScreenX + m_nViewport_Ex*m_fScale)) Pan_Right();
				}
				return MOUSEMOVE;
			}
			else if(vkey == VK_DOWN)
			{
				if((GetKeyState(VK_SHIFT)&0x80) != 0)	
				{
					if(m_nSystem_State == SCH)	m_fCurrentSnapY -= 10*pDoc->m_fSCH_SnapGrid;
					else m_fCurrentSnapY -= 10*pDoc->m_cPCB_Sheet.m_fSnapGridSize;

					if(m_fCurrentSnapY < 0) m_fCurrentSnapY = 0;
					Restore_LargeCrossXY();
					if(m_fCurrentSnapY < m_fScreenY)  Pan_Down();
				}
				else
				{
					if(m_nSystem_State == SCH)	m_fCurrentSnapY -= pDoc->m_fSCH_SnapGrid;
					else m_fCurrentSnapY -= pDoc->m_cPCB_Sheet.m_fSnapGridSize;

					if(m_fCurrentSnapY < 0) m_fCurrentSnapY = 0;
					Restore_LargeCrossXY();
					if(m_fCurrentSnapY < m_fScreenY)  Pan_Down();
				}
				return MOUSEMOVE;
			}
			else if(vkey == VK_UP)
			{
				if((GetKeyState(VK_SHIFT)&0x80) != 0)	
				{
					if(m_nSystem_State == SCH)
					{
						m_fCurrentSnapY += 10*pDoc->m_fSCH_SnapGrid;
						if(m_fCurrentSnapY > fEDA_SCH_SnapMaxy) m_fCurrentSnapY = fEDA_SCH_SnapMaxy;
					}
					else
					{
						m_fCurrentSnapY += 10*pDoc->m_cPCB_Sheet.m_fSnapGridSize;
						if(m_fCurrentSnapY > fEDA_PCB_SnapMaxy) m_fCurrentSnapY = fEDA_PCB_SnapMaxy;
					}

					Restore_LargeCrossXY();
					if(m_fCurrentSnapY >(m_fScreenY + m_nViewport_Ey*m_fScale)) Pan_Up();	
				}
				else
				{
					if(m_nSystem_State == SCH)
					{
						m_fCurrentSnapY += pDoc->m_fSCH_SnapGrid;
						if(m_fCurrentSnapY > fEDA_SCH_SnapMaxy) m_fCurrentSnapY = fEDA_SCH_SnapMaxy;
					}
					else
					{
						m_fCurrentSnapY += pDoc->m_cPCB_Sheet.m_fSnapGridSize;
						if(m_fCurrentSnapY > fEDA_PCB_SnapMaxy) m_fCurrentSnapY = fEDA_PCB_SnapMaxy;
					}

					Restore_LargeCrossXY();
					if(m_fCurrentSnapY >(m_fScreenY + m_nViewport_Ey*m_fScale)) Pan_Up();			
				}
				return MOUSEMOVE;
			}
			else
			{
				return vkey;
			}
		}
	}while(inchar == 0);

	return 0;
}


void CESDView::Screen_XYControl()
{
	if(m_fScreenX < 0) m_fScreenX = 0;
	if(m_fScreenY < 0) m_fScreenY = 0;

	double Size_Maxx, Size_Maxy;
	if(m_nSystem_State == SCH)
	{
		Size_Maxx = fEDA_SCH_SizeMaxx;
		Size_Maxy = fEDA_SCH_SizeMaxy;
	}
	else
	{
		Size_Maxx = fEDA_PCB_SizeMaxx;
		Size_Maxy = fEDA_PCB_SizeMaxy;	
	}

	if((m_fScreenX + m_nViewport_Ex*m_fScale) > Size_Maxx)
	{
		m_fScreenX = Size_Maxx - m_nViewport_Ex*m_fScale;
		if(m_fScreenX < 0) m_fScreenX = 0;
	}
	if((m_fScreenY + m_nViewport_Ey*m_fScale) > Size_Maxy)
	{ 
		m_fScreenY = Size_Maxy - m_nViewport_Ey*m_fScale;
		if(m_fScreenY < 0) m_fScreenY = 0;
	}
}

void CESDView::ScrollBar_Control()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	CSize size, page, line;

	double Size_Maxx, Size_Maxy;
	if(m_nSystem_State == SCH)
	{
		if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_bCustom == FALSE)
		{
			if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0)  
			{
				Size_Maxx = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].width;
				Size_Maxy = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].height;
			}
			else
			{
				Size_Maxx = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].height;
				Size_Maxy = (double)sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].width;
			}
		}
		else
		{
			if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0)  
			{
				Size_Maxx = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth;
				Size_Maxy = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight;
			}
			else
			{
				Size_Maxx = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight;
				Size_Maxy = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth;
			}
		}
		
	}
	else
	{
		Size_Maxx = fEDA_PCB_SizeMaxx;
		Size_Maxy = fEDA_PCB_SizeMaxy;	
	}

	
	size.cx = (int)(Size_Maxx / m_fScale);
	size.cy = (int)(Size_Maxy / m_fScale);

	SetScrollSizes(MM_TEXT, size); 

	BOOL bHor, bVer;
	CheckScrollBars(bHor, bVer);


	
	int x_limit, y_limit;
	x_limit = size.cx;
	y_limit = size.cy;
	
	
	int x_pos, y_pos;
	x_pos = (int)(m_fScreenX/m_fScale);
	if(x_pos > x_limit) x_pos = x_limit;

	y_pos = y_limit - (int)(m_fScreenY/m_fScale) - m_nViewport_Ey;
	if(y_pos > y_limit) y_pos = y_limit;
	if(y_pos < 0) y_pos = 0;

	
	if(bHor != FALSE) SetScrollPos(SB_VERT,y_pos,TRUE);
	if(bVer != FALSE) SetScrollPos(SB_HORZ,x_pos,TRUE);
}

void CESDView::DrawMyCursor(CClientDC * pDC, POINT* cpoint)
{
	int drawmode;
	CPen OverwritePen, *oldpen;
	OverwritePen.CreatePen(PS_SOLID,1, RGB(0,0,0));

	drawmode = pDC->GetROP2();
	oldpen = pDC->SelectObject(&OverwritePen);
	pDC->SetROP2(R2_NOTXORPEN);
	
	if(nEDA_Cursor_Mode == 0)
	{
		
		pDC->MoveTo(cpoint->x - 35, cpoint->y);
		pDC->LineTo(cpoint->x + 35, cpoint->y);
		pDC->MoveTo(cpoint->x,	cpoint->y - 35);
		pDC->LineTo(cpoint->x,	cpoint->y + 35);
	}
	else if( nEDA_Cursor_Mode == 1) 
	{
		
		pDC->MoveTo(cpoint->x - 2500, cpoint->y);
		pDC->LineTo(cpoint->x + 2500, cpoint->y);
		pDC->MoveTo(cpoint->x,	cpoint->y - 2500);
		pDC->LineTo(cpoint->x,	cpoint->y + 2500);
	}
	else
	{
		pDC->MoveTo(cpoint->x - 25, cpoint->y - 25);
		pDC->LineTo(cpoint->x + 25, cpoint->y + 25);
		pDC->MoveTo(cpoint->x - 25,	cpoint->y + 25);
		pDC->LineTo(cpoint->x + 25,	cpoint->y - 25);
	}	
	
	pDC->SelectObject(oldpen);
	pDC->SetROP2(drawmode);
}

void CESDView::OnViewZoom_In() 
{
	
	double fscalemin;
	if(m_nSystem_State == SCH) fscalemin = fEDA_ScaleMin;
	else fscalemin = fEDA_ScaleMin/4;

	if(m_fScale <= fscalemin) return;

	m_fScale = m_fScale/2;
	if(m_fScale < fscalemin) m_fScale = fscalemin;

	m_fScreenX = m_fCurrentSnapX - (Snap_Lock(m_nViewport_Ex*m_fScale/2));
	m_fScreenY = m_fCurrentSnapY - (Snap_Lock(m_nViewport_Ey*m_fScale/2));

	UpdateZoomSlider();
	Screen_XYControl();
	ScrollBar_Control();
	Invalidate();
	SendMessage(WM_PAINT);
	Restore_LargeCrossXY();
}

void CESDView::OnViewZoom_Out() 
{
	
	if(m_fScale >= fEDA_ScaleMax) return;

	m_fScale = m_fScale*2;
	if(m_fScale > fEDA_ScaleMax) m_fScale = fEDA_ScaleMax;

	if(m_fScale == fEDA_ScaleMax)
	{
		m_fScreenX = 0; 
		m_fScreenY = 0;
	}
	else
	{
		m_fScreenX = m_fCurrentSnapX - (Snap_Lock(m_nViewport_Ex*m_fScale/2));
		m_fScreenY = m_fCurrentSnapY - (Snap_Lock(m_nViewport_Ey*m_fScale/2));
	}
	
	UpdateZoomSlider();
	Screen_XYControl();
	ScrollBar_Control();
	Invalidate();
	SendMessage(WM_PAINT);
	Restore_LargeCrossXY();
}

void CESDView::Restore_LargeCrossXY()
{
	CClientDC dc(this);
	if(m_bCursorState == TRUE) DrawMyCursor(&dc, &m_CursorLocation); 
	
	
	Get_LargeCross_XY(); 
	
	m_CursorLocation.x = nEDA_LargeCrossX; 
	m_CursorLocation.y = nEDA_LargeCrossY; 
	if(m_bCursorState == TRUE) DrawMyCursor(&dc, &m_CursorLocation); 

	
	POINT p;
	p.x = nEDA_LargeCrossX;  p.y = nEDA_LargeCrossY;
	ClientToScreen(&p);
	
	SetCursorPos(p.x,p.y);

	
	Write_XY(m_fCurrentSnapX, m_fCurrentSnapY);		

}

void CESDView::Get_LargeCross_XY()
{
	

	if((nEDA_System_State == PCB)&&(m_bPcbFlipView == TRUE)) nEDA_LargeCrossX = m_nViewport_Ex - (int)((m_fCurrentSnapX - m_fScreenX)/m_fScale);   
	else nEDA_LargeCrossX = (int)((m_fCurrentSnapX - m_fScreenX)/m_fScale);
	nEDA_LargeCrossY = m_nViewport_Ey - (int)((m_fCurrentSnapY - m_fScreenY)/m_fScale);
}

void CESDView::Get_VPort_XY(int * vpx, int * vpy, double fx, double fy)
{
	if(m_nPrint_Flag == 0)
	{
		*vpx = (int)((fx - m_fScreenX)/m_fScale);
		*vpy = m_nViewport_Ey - (int)((fy - m_fScreenY)/m_fScale);
	}
	else
	{
		*vpx = (int)((fx - m_fScreenX)/m_fScale);
		*vpy = (int)(-(fy - m_fScreenY)/m_fScale); 
	}
}

void CESDView::Get_Current_XY(int mouse_x, int mouse_y)
{
 	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	if((nEDA_System_State == PCB)&&(m_bPcbFlipView == TRUE))  m_fCurrentX = (m_nViewport_Ex - mouse_x)*m_fScale + m_fScreenX;	
	else m_fCurrentX = mouse_x*m_fScale + m_fScreenX;
	m_fCurrentY = (m_nViewport_Ey - mouse_y - 1)*m_fScale + m_fScreenY;

	double Size_Maxx, Size_Maxy;
	if(m_nSystem_State == SCH)
	{
		Size_Maxx = fEDA_SCH_SizeMaxx;
		Size_Maxy = fEDA_SCH_SizeMaxy;
	}
	else
	{
		Size_Maxx = fEDA_PCB_SizeMaxx;
		Size_Maxy = fEDA_PCB_SizeMaxy;	
	}

	if(m_fCurrentX > Size_Maxx)	m_fCurrentX = Size_Maxx;
	if(m_fCurrentY > Size_Maxy)	m_fCurrentY = Size_Maxy;

	if(m_nSystem_State == SCH)
	{
		double fex, fey;
		if( (bEDA_NeedElectricalGrid == TRUE)&&(pDoc->m_bSCH_ElecGridEnable == TRUE)&&( pDoc->GetElectricalPoint(&fex, &fey, m_fCurrentX, m_fCurrentY, m_nDiagram) == TRUE ) )
		{
			m_fCurrentSnapX = fex;
			m_fCurrentSnapY = fey;
			m_fpHotPoint1.fx = fex;
			m_fpHotPoint1.fy = fey;
		}
		else
		{
			m_fCurrentSnapX = Snap_Lock(m_fCurrentX);
			m_fCurrentSnapY = Snap_Lock(m_fCurrentY);
			m_fpHotPoint1.fx = 0;
			m_fpHotPoint1.fy = 0;
		}
	}
	else
	{
		double fex, fey;
		if( (bEDA_NeedElectricalGrid == TRUE)&&(pDoc->m_cPCB_Sheet.m_bElecGridEnable == TRUE)&&( pDoc->PcbGetElectricalPoint(&fex, &fey, m_fCurrentX, m_fCurrentY, m_nPCB_Current_Layer) == TRUE ) )
		{
			m_fCurrentSnapX = fex;
			m_fCurrentSnapY = fey;
			m_fpPcbHotPoint1.fx = fex;
			m_fpPcbHotPoint1.fy = fey;
		}
		else
		{
			m_fCurrentSnapX = Snap_Lock(m_fCurrentX);
			m_fCurrentSnapY = Snap_Lock(m_fCurrentY);
			m_fpPcbHotPoint1.fx = 0;
			m_fpPcbHotPoint1.fy = 0;
		}
	}
}

double CESDView::Snap_Lock(double fxy)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(pDoc->m_bSCH_SnapGridEnable == FALSE) return fxy;

	
	int n1, n2;
	double f;
	
	
	if(m_nSystem_State == SCH) f = fxy/pDoc->m_fSCH_SnapGrid;
	else f = fxy/pDoc->m_cPCB_Sheet.m_fSnapGridSize;
	
	n1 = (int)f;
	n2 = (int)((f-n1)*10000);
	if(n2 >= 5000) n1++;
	
	if(m_nSystem_State == SCH) f = (double)n1*pDoc->m_fSCH_SnapGrid;
	else f = (double)n1*pDoc->m_cPCB_Sheet.m_fSnapGridSize;

	return f;
}

void CESDView::Write_XY(double fx, double fy)
{
	char xx[200],yy[200];
	char s[200];
	char temp[200] = "-"; 
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(m_nSystem_State == SCH)
	{
		
		if((pDoc->m_nSCH_Unit%2) == 0)
		{
			CString cstring;
			cstring = pDoc->Convert_XY_To_String(fx);
			strcpy(xx, cstring);
			cstring = pDoc->Convert_XY_To_String(fy);
			strcpy(yy, cstring);
				//mil_to_str(fx,xx);
				//mil_to_str(fy,yy);
		}
		else 
		{
			CString cstring;
			cstring = pDoc->Convert_XY_To_String(fx);
			strcpy(xx, cstring);
			cstring = pDoc->Convert_XY_To_String(fy);
			strcpy(yy, cstring);
				//mm_to_str(fx,xx);
				//mm_to_str(fy,yy);
		}
	}
	else
	{
		if(pDoc->m_cPCB_Sheet.m_nUnit == UNIT_PCB_I)
		{
			//if(fx >= pDoc->m_cPCB_Sheet.m_fOriginX) 
				mil_to_str(fx - pDoc->m_cPCB_Sheet.m_fOriginX,xx);

				mil_to_str(fy - pDoc->m_cPCB_Sheet.m_fOriginY,yy);

		}
		else
		{
			//if(fx >= pDoc->m_cPCB_Sheet.m_fOriginX) 
				mm_to_str(fx - pDoc->m_cPCB_Sheet.m_fOriginX,xx);

			//if(fy >= pDoc->m_cPCB_Sheet.m_fOriginY) 
				mm_to_str(fy - pDoc->m_cPCB_Sheet.m_fOriginY,yy);

		}
	}

	
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
	char xystring[1000] = "";
	sprintf(xystring,"X:%s Y:%s",xx,yy);
	pFrame->Set_Status_XY(xystring);

	
	if(nEDA_System_State == SCH)
	{
		char unitstring[100];
		if(pDoc->m_nSCH_Unit%2 == 0) strcpy(unitstring, "英制");
		else strcpy(unitstring, "公制");
		pFrame->Set_Unit(unitstring);
		
		CString str = pDoc->Convert_XY_To_String(pDoc->m_fSCH_SnapGrid);
		char string[100];
		strcpy(string, "S:");
		strcat(string, str);
		pFrame->Set_Snap(string);

		if(pDoc->m_bSCH_ElecGridEnable == TRUE) strcpy(string, "E:允许");
		else strcpy(string, "E:禁止");
		pFrame->Set_Electrical(string);
	
		if(pDoc->m_bSCH_VisibleGridShow == TRUE) strcpy(string, "V:显示");
		else strcpy(string, "V:隐藏");
		pFrame->Set_Visible(string);

		UpdateZoomSlider();
	}
	else
	{
		char unitstring[100];
		if(pDoc->m_cPCB_Sheet.m_nUnit == UNIT_PCB_I) strcpy(unitstring, "英制");
		else strcpy(unitstring, "公制");
		pFrame->Set_Unit(unitstring);
		
		CString str = pDoc->Convert_XY_To_String(pDoc->m_cPCB_Sheet.m_fSnapGridSizeX);
		char string[100];
		strcpy(string, "S:");
		strcat(string, str);
		pFrame->Set_Snap(string);

		str = pDoc->Convert_XY_To_String(pDoc->m_cPCB_Sheet.m_fBigVisibleGridSize);
		strcpy(string, "V:");
		strcat(string, str);
		pFrame->Set_Visible(string);

		if(pDoc->m_cPCB_Sheet.m_bElecGridEnable == TRUE) strcpy(string, "E:允许");
		else strcpy(string, "E:禁止");
		pFrame->Set_Electrical(string);

		UpdateZoomSlider();
	}
}

void CESDView::OnViewZoomIn() 
{

	unsigned ch1;
	double fx,fy;
	int xs,ys;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	DrawStructButton(ID_VIEW_ZOOM_IN,TRUE);
	
	fx = m_fCurrentSnapX; fy = m_fCurrentSnapY;
	Get_VPort_XY(&xs,&ys,fx,fy);
	Write_Help("定义中心点放大视图");

	do
	{
       ch1 = MouseKey();
       if(ch1 == MOUSEMOVE)
	   {
		   fx = m_fCurrentSnapX; fy = m_fCurrentSnapY;
		   Get_VPort_XY(&xs,&ys,fx,fy);
	   }
       else if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
			
			double fscalemin;
			if(m_nSystem_State == SCH) fscalemin = fEDA_ScaleMin;
			else fscalemin = fEDA_ScaleMin/4;

			if(m_fScale <= fscalemin) break;

			m_fScale = m_fScale/1.5f;
			if(m_fScale < fscalemin) m_fScale = fscalemin;

			m_fScreenX = m_fCurrentSnapX - (Snap_Lock(m_nViewport_Ex*m_fScale/2));
			m_fScreenY = m_fCurrentSnapY - (Snap_Lock(m_nViewport_Ey*m_fScale/2));

			UpdateZoomSlider();
			Screen_XYControl();
			ScrollBar_Control();
			Invalidate();
			SendMessage(WM_PAINT);
			Restore_LargeCrossXY();
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	DrawStructButton(ID_VIEW_ZOOM_IN,FALSE);
	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnViewZoomOut() 
{
	unsigned ch1;
	double fx,fy;
	int xs,ys;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	DrawStructButton(ID_VIEW_ZOOM_OUT,TRUE);
	
	fx = m_fCurrentSnapX; fy = m_fCurrentSnapY;
	Get_VPort_XY(&xs,&ys,fx,fy);
	Write_Help("定义中心点缩小视图");

	do
	{
       ch1 = MouseKey();
       if(ch1 == MOUSEMOVE)
	   {
		   fx = m_fCurrentSnapX; fy = m_fCurrentSnapY;
		   Get_VPort_XY(&xs,&ys,fx,fy);
	   }
       else if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
			
			
			if(m_fScale >= fEDA_ScaleMax) break;

			m_fScale = m_fScale*1.5f;
			if(m_fScale > fEDA_ScaleMax) m_fScale = fEDA_ScaleMax;

			if(m_fScale == fEDA_ScaleMax)
			{
				m_fScreenX = 0; 
				m_fScreenY = 0;
			}
			else
			{
				m_fScreenX = m_fCurrentSnapX - Snap_Lock(m_nViewport_Ex*m_fScale/2);
				m_fScreenY = m_fCurrentSnapY - Snap_Lock(m_nViewport_Ey*m_fScale/2);
			}
	
			UpdateZoomSlider();
			Screen_XYControl();
			ScrollBar_Control();
			Invalidate();
			SendMessage(WM_PAINT);
			Restore_LargeCrossXY();
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	DrawStructButton(ID_VIEW_ZOOM_OUT,FALSE);
	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnViewFitDocument()
{
	double scale_x, scale_y;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(nEDA_System_State == SCH)
	{
		if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_bCustom == TRUE)
		{
			if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0)
			{
				scale_x = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth/m_nViewport_Ex;
				scale_y = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight/m_nViewport_Ey;
			}
			else
			{
				scale_x = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomHeight/m_nViewport_Ex;
				scale_y = pDoc->m_pSCH_Sheet[m_nDiagram]->m_fCustomWidth/m_nViewport_Ey;
			}
		}
		else
		{
			int style = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle;
			if(pDoc->m_pSCH_Sheet[m_nDiagram]->m_nOrientation == 0)
			{
				scale_x = (double)sEDA_Standard_Sheet[style].width/m_nViewport_Ex;
				scale_y = (double)sEDA_Standard_Sheet[style].height/m_nViewport_Ey;
			}
			else
			{
				scale_x = (double)sEDA_Standard_Sheet[style].height/m_nViewport_Ex;
				scale_y = (double)sEDA_Standard_Sheet[style].width/m_nViewport_Ey;
			}
		}

		if(scale_x > scale_y)
		{
			m_fScale = scale_x;
			m_fScreenX = 0;
			m_fScreenY = 0;
		}
		else
		{
			m_fScale = scale_y;
			m_fScreenX = 0;
			m_fScreenY = 0;
		}
	}
	else
	{
		scale_x = pDoc->m_cPCB_Sheet.m_fSheetWidth/m_nViewport_Ex;
		scale_y = pDoc->m_cPCB_Sheet.m_fSheetHeight/m_nViewport_Ey;

		if(scale_x > scale_y)
		{
			m_fScale = scale_x;
			m_fScreenX = pDoc->m_cPCB_Sheet.m_fSheetX;
			m_fScreenY = pDoc->m_cPCB_Sheet.m_fSheetY;
		}
		else
		{
			m_fScale = scale_y;
			m_fScreenX = pDoc->m_cPCB_Sheet.m_fSheetX;
			m_fScreenY = pDoc->m_cPCB_Sheet.m_fSheetY;
		}
	}

	UpdateZoomSlider();
	Screen_XYControl();
	ScrollBar_Control();
	Invalidate();
}

void CESDView::OnViewZoomCenter() 
{
	if(m_fScale == fEDA_ScaleMax)
	{
		m_fScreenX = 0;
		m_fScreenY = 0;
	}
	else
	{
		m_fScreenX = m_fCurrentSnapX - (Snap_Lock(m_nViewport_Ex*m_fScale/2));
		m_fScreenY = m_fCurrentSnapY - (Snap_Lock(m_nViewport_Ey*m_fScale/2));
		Screen_XYControl();
	}

	UpdateZoomSlider();
	Screen_XYControl();
	ScrollBar_Control();
	Invalidate();
	Restore_LargeCrossXY();
}

void CESDView::OnViewZoomRedraw() 
{
	// TODO: Add your command handler code here
	Invalidate();
	SendMessage(WM_PAINT);
}

void CESDView::Pan_Left()
{
	if(m_fScreenX > 0)
	{
		m_fScreenX -= Snap_Lock(m_nViewport_Ex*m_fScale/2);
		if(m_fScreenX < 0) m_fScreenX = 0;

		Restore_LargeCrossXY();
		ScrollBar_Control();
		Invalidate();
		SendMessage(WM_PAINT);
	}
}

void CESDView::Pan_Right()
{
	double PSnap_MAXX;
	if(m_nSystem_State == SCH) PSnap_MAXX = fEDA_SCH_SnapMaxx;
	else PSnap_MAXX = fEDA_PCB_SnapMaxx;

	if((m_fScreenX + Snap_Lock(m_nViewport_Ex*m_fScale)) < PSnap_MAXX)
	{
		m_fScreenX += Snap_Lock(m_nViewport_Ex*m_fScale/2);
	    if(m_fScreenX > PSnap_MAXX)
		{
			m_fScreenX = PSnap_MAXX - Snap_Lock(m_nViewport_Ex*m_fScale);
		}

		Restore_LargeCrossXY();
		ScrollBar_Control();
		Invalidate();
		SendMessage(WM_PAINT);
	}
}

void CESDView::Pan_Up()
{
	double PSnap_MAXY;
	if(m_nSystem_State == SCH) PSnap_MAXY = fEDA_SCH_SnapMaxy;
	else PSnap_MAXY = fEDA_PCB_SnapMaxy;

	if((m_fScreenY + Snap_Lock(m_nViewport_Ey*m_fScale)) < PSnap_MAXY)
	{
		m_fScreenY += Snap_Lock(m_nViewport_Ey*m_fScale/2);
		if(m_fScreenY > PSnap_MAXY)
		{
			m_fScreenY = PSnap_MAXY - Snap_Lock(m_nViewport_Ey*m_fScale);
		}	

		Restore_LargeCrossXY();
		ScrollBar_Control();
		Invalidate();
		SendMessage(WM_PAINT);
	}
}


void CESDView::Pan_Down()
{
		if(m_fScreenY > 0)
		{
			m_fScreenY -= Snap_Lock(m_nViewport_Ey*m_fScale/2);
			if(m_fScreenY < 0)m_fScreenY = 0;

			Restore_LargeCrossXY();
			ScrollBar_Control();
			Invalidate();
			SendMessage(WM_PAINT);
		}

}

void CESDView::Command_In_Function(int ch1)
{

	switch(ch1)
	{

    case VK_ADD:
		//if(m_nSystem_State==SCH) Plus_Page();
	    //if(m_nSystem_State==PCB) Plus_Layer();
	    break;
    case VK_SUBTRACT: 
		//if(m_nSystem_State==SCH) Minus_Page();
	    //if(m_nSystem_State==PCB) Minus_Layer();
	    break;
	default:
		char c = (char)(ch1&0x0000007f);
/*		if(m_nSystem_State==PCB)
		{
			if(c == ';') 
			{
				CL_Mouse_Move_One = TRUE;
				Plus_Layer();
			}
			else if(c == '=')
			{
				CL_Mouse_Move_One = TRUE;
				Minus_Layer();
			}
		}*/
	}
}

void CESDView::CloseMouseCursor()
{
	int x,y,drawmode;
	CClientDC dc(this);
	
	drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	Get_VPort_XY(&x, &y, m_fCurrentSnapX, m_fCurrentSnapY);
	
	if( nEDA_Cursor_Mode == 0 )
	{	
		dc.MoveTo(x - 35, y);
		dc.LineTo(x + 35, y);
		dc.MoveTo(x,	y - 35);
		dc.LineTo(x,	y + 35);
	}
	else if( nEDA_Cursor_Mode == 1 ) 
	{
		dc.MoveTo(x - 2500, y);
		dc.LineTo(x + 2500, y);
		dc.MoveTo(x,	y - 2500);
		dc.LineTo(x,	y + 2500);
	}
	else
	{
		dc.MoveTo(x - 25, y - 25);
		dc.LineTo(x + 25, y + 25);
		dc.MoveTo(x - 25, y + 25);
		dc.LineTo(x + 25, y - 25);
	}
	dc.SetROP2(drawmode);

	m_bCursorState = FALSE;
}

void CESDView::OpenMouseCursor()

{
	int x,y,drawmode;
	CClientDC dc(this);

	drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	
	Get_LargeCross_XY();
	m_CursorLocation.x = nEDA_LargeCrossX; 
	m_CursorLocation.y = nEDA_LargeCrossY; 

	Get_VPort_XY(&x, &y, m_fCurrentSnapX, m_fCurrentSnapY);

	if( nEDA_Cursor_Mode == 0 )
	{	
		dc.MoveTo(x - 35, y);
		dc.LineTo(x + 35, y);
		dc.MoveTo(x,	y - 35);
		dc.LineTo(x,	y + 35);
	}
	else if( nEDA_Cursor_Mode == 1 ) 
	{
		dc.MoveTo(x - 2500, y);
		dc.LineTo(x + 2500, y);
		dc.MoveTo(x,	y - 2500);
		dc.LineTo(x,	y + 2500);
	}
	else
	{
		dc.MoveTo(x - 25, y - 25);
		dc.LineTo(x + 25, y + 25);
		dc.MoveTo(x - 25, y + 25);
		dc.LineTo(x + 25, y - 25);
	}

	dc.SetROP2(drawmode);
	m_bCursorState = TRUE;
}

void CESDView::OnViewFitAllObjects()
{
	Fpoint fp1, fp2;

	if(nEDA_System_State == SCH)
	{
		GetAllObjectsRect(fp1, fp2);

		double scale_x, scale_y;
		scale_x = (fp2.fx - fp1.fx)/m_nViewport_Ex;
		scale_y = (fp2.fy - fp1.fy)/m_nViewport_Ey;

		m_fScale = MAX(scale_x, scale_y);

		double fscalemin;
		fscalemin = fEDA_ScaleMin;

		if(m_fScale < fscalemin) m_fScale = fscalemin;

		
		m_fScreenX = fp1.fx + (fp2.fx - fp1.fx - m_nViewport_Ex*m_fScale)/2;
		m_fScreenY = fp1.fy + (fp2.fy - fp1.fy - m_nViewport_Ey*m_fScale)/2;
	}
	else
	{
		GetAllPCBObjectsRect(fp1, fp2);

		double scale_x, scale_y;
		scale_x = (fp2.fx - fp1.fx)/m_nViewport_Ex;
		scale_y = (fp2.fy - fp1.fy)/m_nViewport_Ey;

		m_fScale = MAX(scale_x, scale_y);

		double fscalemin;
		fscalemin = fEDA_ScaleMin/4;

		if(m_fScale < fscalemin) m_fScale = fscalemin;

		
		m_fScreenX = fp1.fx + (fp2.fx - fp1.fx - m_nViewport_Ex*m_fScale)/2;
		m_fScreenY = fp1.fy + (fp2.fy - fp1.fy - m_nViewport_Ey*m_fScale)/2;
	}

	
	
	UpdateZoomSlider();
	Screen_XYControl();
	ScrollBar_Control();		
	
	Invalidate();
	Restore_LargeCrossXY();
}

void CESDView::OnView100()
{
	m_fScale = fEDA_Scale100;

	m_fScreenX = m_fCurrentSnapX - (Snap_Lock(m_nViewport_Ex*m_fScale/2));
	m_fScreenY = m_fCurrentSnapY - (Snap_Lock(m_nViewport_Ey*m_fScale/2));
	
	UpdateZoomSlider();
	Screen_XYControl();
	ScrollBar_Control();
	Invalidate();
	Restore_LargeCrossXY();
}

void CESDView::OnView200()
{
	m_fScale = fEDA_Scale100*2;

	m_fScreenX = m_fCurrentSnapX - (Snap_Lock(m_nViewport_Ex*m_fScale/2));
	m_fScreenY = m_fCurrentSnapY - (Snap_Lock(m_nViewport_Ey*m_fScale/2));

	UpdateZoomSlider();
	Screen_XYControl();
	ScrollBar_Control();
	Invalidate();
	Restore_LargeCrossXY();
}

void CESDView::OnView400()
{
	m_fScale = fEDA_Scale100*4;

	m_fScreenX = m_fCurrentSnapX - (Snap_Lock(m_nViewport_Ex*m_fScale/2));
	m_fScreenY = m_fCurrentSnapY - (Snap_Lock(m_nViewport_Ey*m_fScale/2));

	UpdateZoomSlider();
	Screen_XYControl();
	ScrollBar_Control();
	Invalidate();
	Restore_LargeCrossXY();
}

void CESDView::OnView50()
{
	m_fScale = fEDA_Scale100/2;

	m_fScreenX = m_fCurrentSnapX - (Snap_Lock(m_nViewport_Ex*m_fScale/2));
	m_fScreenY = m_fCurrentSnapY - (Snap_Lock(m_nViewport_Ey*m_fScale/2));

	UpdateZoomSlider();
	Screen_XYControl();
	ScrollBar_Control();
	Invalidate();
	Restore_LargeCrossXY();
}

void CESDView::OnViewArea()
{
	unsigned ch1;
	double fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;
	

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	MoveCursorToCenter();

	Write_Help("定义区域一角");
	flag = 0;

	int drawmode;
	drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			fx1 = m_fCurrentSnapX; fy1 = m_fCurrentSnapY;
			Get_VPort_XY(&xs,&ys,fx1,fy1);
			xe=xs;
			ye=ys;
			Write_Help("定义区域另一角");
			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xs,&ys,fx1,fy1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					goto ZOOMBREAK;

				}
				if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{ 
						flag=1;
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xs,ye);
						dc.LineTo(xs,ys);
					}
					else
					{
						if(nEDA_RedrawFlag == ON){ nEDA_RedrawFlag = OFF;}
						else 
						{ 
							dc.MoveTo(xs,ys);
							dc.LineTo(xe,ys);
							dc.LineTo(xe,ye);
							dc.LineTo(xs,ye);
							dc.LineTo(xs,ys);
						}
						fx2 = m_fCurrentSnapX; fy2 = m_fCurrentSnapY;
						Get_VPort_XY(&xe,&ye,fx2,fy2);
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xs,ye);
						dc.LineTo(xs,ys);
					}
				}
				if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{  
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					goto ZOOMBREAK;
				}
				else Command_In_Function(ch1);
			}while(1);
		}
		else Command_In_Function(ch1);
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

ZOOMBREAK:
	if((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC))
	{
		double ftemp;
		if(fx1 > fx2)
		{
			ftemp = fx2;
			fx2 = fx1;
			fx1 = ftemp;
		}
		if(fy1 > fy2)
		{
			ftemp = fy2;
			fy2 = fy1;
			fy1 = ftemp;
		}

		double scale_x, scale_y;
		scale_x = (fx2 - fx1)/m_nViewport_Ex;
		scale_y = (fy2 - fy1)/m_nViewport_Ey;

		m_fScale = MAX(scale_x, scale_y);


		double fscalemin;
		if(m_nSystem_State == SCH) fscalemin = fEDA_ScaleMin;
		else fscalemin = fEDA_ScaleMin/4;

		if(m_fScale < fscalemin) m_fScale = fscalemin;

		
		m_fScreenX = fx1 + (fx2 - fx1 - m_nViewport_Ex*m_fScale)/2;
		m_fScreenY = fy1 + (fy2 - fy1 - m_nViewport_Ey*m_fScale)/2;

		UpdateZoomSlider();
		Screen_XYControl();
		ScrollBar_Control();		
	}
	
	Invalidate();

	Restore_LargeCrossXY();

	dc.SetROP2(drawmode);
	//DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	//DrawStructButton(ID_VIEW_ZOOM_WINDOW,FALSE);
	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnViewCenter()
{

	unsigned ch1;
	double fxc, fyc, fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;


	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	//DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	//DrawStructButton(ID_VIEW_ZOOM_CENTER,TRUE);

	Write_Help("定义显示中心点");
	flag = 0;

	int drawmode;
	drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			fxc = m_fCurrentSnapX; fyc = m_fCurrentSnapY;
			Get_VPort_XY(&xs,&ys,fxc,fyc);
			fx1 = fx2 = fxc;
			fy1 = fy2 = fyc;
			xe=xs;
			ye=ys;
			Write_Help("定义显示范围");
			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xs,&ys,fx1,fy1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					goto ZOOMBREAK;

				}
				if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{ 
						flag = 1;
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xs,ye);
						dc.LineTo(xs,ys);
					}
					else
					{
						if(nEDA_RedrawFlag == ON){ nEDA_RedrawFlag = OFF;}
						else 
						{ 
							dc.MoveTo(xs,ys);
							dc.LineTo(xe,ys);
							dc.LineTo(xe,ye);
							dc.LineTo(xs,ye);
							dc.LineTo(xs,ys);
						}
						fx2 = m_fCurrentSnapX; fy2 = m_fCurrentSnapY;
						Get_VPort_XY(&xe,&ye,fx2,fy2);
						fx1 = fxc + (fxc - fx2);
						fy1 = fyc + (fyc - fy2);
						Get_VPort_XY(&xs,&ys,fx1,fy1);
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xs,ye);
						dc.LineTo(xs,ys);
					}
				}
				if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{  
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					goto ZOOMBREAK;
				}
				else Command_In_Function(ch1);
			}while(1);
		}
		else Command_In_Function(ch1);
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

ZOOMBREAK:
	if((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC))
	{
		double ftemp;
		if(fx1 > fx2)
		{
			ftemp = fx2;
			fx2 = fx1;
			fx1 = ftemp;
		}
		if(fy1 > fy2)
		{
			ftemp = fy2;
			fy2 = fy1;
			fy1 = ftemp;
		}

		double scale_x, scale_y;
		scale_x = (fx2 - fx1)/m_nViewport_Ex;
		scale_y = (fy2 - fy1)/m_nViewport_Ey;

		m_fScale = MAX(scale_x, scale_y);


		double fscalemin;
		if(m_nSystem_State == SCH) fscalemin = fEDA_ScaleMin;
		else fscalemin = fEDA_ScaleMin/4;

		if(m_fScale < fscalemin) m_fScale = fscalemin;

		
		m_fScreenX = fxc - (m_nViewport_Ex*m_fScale)/2;
		m_fScreenY = fyc - (m_nViewport_Ey*m_fScale)/2;

		UpdateZoomSlider();
		Screen_XYControl();
		ScrollBar_Control();		
	}
	
	Invalidate();

	Restore_LargeCrossXY();

	dc.SetROP2(drawmode);
	//DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	//DrawStructButton(ID_VIEW_ZOOM_CENTER,FALSE);
	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnViewZoomLast()
{
	// TODO: 在此添加命令处理程序代码
}

void CESDView::OnViewRefresh()
{
	Invalidate();
}

void CESDView::OnViewPan()
{

	unsigned ch1;
	CClientDC dc(this);

	//DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	

	Write_Help("移动鼠标或光标键到窗口边缘处自动滚屏");

	do
	{
       ch1 = MouseKey();
       if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN)) break;

	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnViewSelectedObjects()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	double ftemp;
	double fx1, fy1, fx2, fy2;
	fx1 = pDoc->m_fSelection_X1[m_nDiagram];
	fy1 = pDoc->m_fSelection_Y1[m_nDiagram];
	fx2 = pDoc->m_fSelection_X2[m_nDiagram];
	fy2 = pDoc->m_fSelection_Y2[m_nDiagram];
	if(fx1 > fx2)
	{
		ftemp = fx2;
		fx2 = fx1;
		fx1 = ftemp;
	}
	if(fy1 > fy2)
	{
		ftemp = fy2;
		fy2 = fy1;
		fy1 = ftemp;
	}

	double scale_x, scale_y;
	scale_x = (fx2 - fx1)/m_nViewport_Ex;
	scale_y = (fy2 - fy1)/m_nViewport_Ey;

	m_fScale = MAX(scale_x, scale_y);


	double fscalemin;
	if(m_nSystem_State == SCH) fscalemin = fEDA_ScaleMin;
	else fscalemin = fEDA_ScaleMin/4;

	if(m_fScale < fscalemin) m_fScale = fscalemin;

	
	m_fScreenX = fx1 + (fx2 - fx1 - m_nViewport_Ex*m_fScale)/2;
	m_fScreenY = fy1 + (fy2 - fy1 - m_nViewport_Ey*m_fScale)/2;

	UpdateZoomSlider();
	Screen_XYControl();
	ScrollBar_Control();		
	
	Invalidate();

	Restore_LargeCrossXY();
}


void CESDView::OnUpdateViewSelectedObjects(CCmdUI *pCmdUI)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(pDoc->m_arraySelection[m_nDiagram].GetCount() > 0)
	{
		pCmdUI->Enable(TRUE);
		//pCmdUI->SetCheck(TRUE);
	}
	else pCmdUI->Enable(FALSE);
}

void CESDView::OnZoomSliderScroll(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMXTPSCROLL* pNMScroll = (NMXTPSCROLL*)pNMHDR;
	CXTPStatusBarSliderPane* pPane = DYNAMIC_DOWNCAST(CXTPStatusBarSliderPane, pNMScroll->pSender);
	if (!pPane)
		return;

	int nZoom = (int)m_fScale;
	if(nZoom > 200) nZoom = 200;
	if(nZoom < 0) nZoom = 0;

	switch (pNMScroll->nSBCode)
	{
		case SB_TOP:	nZoom = 0; break;
		case SB_BOTTOM: nZoom = 200; break;

		case SB_LINEUP: nZoom = MAX(nZoom  - 1, 0); break;
		case SB_LINEDOWN: nZoom = MIN(nZoom + 1, 200); break;

		case SB_THUMBTRACK: nZoom = pNMScroll->nPos; break;

		case SB_PAGEUP: nZoom = max(nZoom - 10, 0); break;
		case SB_PAGEDOWN: nZoom = min(nZoom + 10, 200); break;
	}

	if(nZoom <= 0) { m_fScale = fEDA_ScaleMin; nZoom = 0;}
	else if(nZoom >= 200) { m_fScale = fEDA_ScaleMax; nZoom = 200; }
	else m_fScale = (double)nZoom;
	
	pPane->SetPos(nZoom);

	CXTPStatusBar* pStatusBar = pPane->GetStatusBar();
	
	CXTPStatusBarPane* pPaneZoomIndicator = pStatusBar->FindPane(ID_INDICATOR_ZOOM);

	if (pPaneZoomIndicator)
	{
		CString strZoom;
		strZoom.Format(_T("%i%%"), nZoom*10);
		pPaneZoomIndicator->SetText(strZoom);
	}
	Invalidate(TRUE);
}

void CESDView::UpdateZoomSlider()
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	int nZoom = (int)m_fScale;
	if(nZoom > 200) nZoom = 200;
	if(nZoom < 0) nZoom = 0;
	CXTPStatusBarPane* pPane = pFrame->m_wndStatusBar.FindPane(ID_INDICATOR_ZOOM);
	if (pPane)
	{
		CString strZoom;
		strZoom.Format(_T("%i%%"), nZoom*10);
		pPane->SetText(strZoom);
	}
	CXTPStatusBarSliderPane* pPane1 = (CXTPStatusBarSliderPane*)pFrame->m_wndStatusBar.FindPane(ID_INDICATOR_ZOOMSLIDER);
	if(pPane1) pPane1->SetPos(nZoom);
}


void CESDView::OnZoomIndicator()
{
	CMenu Menu;
	CPoint pt;
	GetCursorPos(&pt);
	//CPoint pt = point;
	//GetCursorPos(&pt);
	if(Menu.LoadMenu(IDR_ZOOM))
	{
		CMenu* pSubMenu = Menu.GetSubMenu(0);
		if(pSubMenu != NULL)
		{
			pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, this);
		}
	}
}

void CESDView::MoveCursorToCenter()
{
	m_fCurrentSnapX = m_fScreenX + (Snap_Lock(m_nViewport_Ex*m_fScale/2));
	m_fCurrentSnapY = m_fScreenY + (Snap_Lock(m_nViewport_Ey*m_fScale/2));
	Restore_LargeCrossXY();
}

void CESDView::MoveCursorToXY(double fx, double fy)
{
	m_fCurrentSnapX = m_fScreenX + (Snap_Lock(m_nViewport_Ex*m_fScale/2));
	m_fCurrentSnapY = m_fScreenY + (Snap_Lock(m_nViewport_Ey*m_fScale/2));
	Restore_LargeCrossXY();
}

void CESDView::OnGridCycleSnap()
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc->m_nSCH_Unit%2 == 0)
	{
		
		if(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_I[0])
		{
			pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_I[0];
		}
		else if(pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_I[2])
		{
			pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_I[0];
		}
		else if((pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_I[0])&&(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_I[1])) pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_I[1];
		else if((pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_I[1])&&(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_I[2])) pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_I[2];
	}
	else
	{
		
		if(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_M[0])
		{
			pDoc->m_fSCH_SnapGrid =fEDA_SCH_SnapGrid_M[0];
		}
		else if(pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_M[2])
		{
			pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_M[0];
		}
		else if((pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_M[0])&&(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_M[1])) pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_M[1];
		else if((pDoc->m_fSCH_SnapGrid >= fEDA_SCH_SnapGrid_M[1])&&(pDoc->m_fSCH_SnapGrid < fEDA_SCH_SnapGrid_M[2])) pDoc->m_fSCH_SnapGrid = fEDA_SCH_SnapGrid_M[2];
	}

	CString str = pDoc->Convert_XY_To_String(pDoc->m_fSCH_SnapGrid);
	char string[100];
	strcpy(string, "S:");
	strcat(string, str);
	pFrame->Set_Snap(string);
}

void CESDView::OnGridToggleElectric()
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	char string[100];
	if(pDoc->m_bSCH_ElecGridEnable == TRUE)
	{
		pDoc->m_bSCH_ElecGridEnable = FALSE;
		strcpy(string, "E:禁止");
	}
	else 
	{
		pDoc->m_bSCH_ElecGridEnable = TRUE;
		strcpy(string, "E:允许");
	}

	pFrame->Set_Electrical(string);
}

void CESDView::OnGridToggleVisible()
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	char string[100];
	if(pDoc->m_bSCH_VisibleGridShow == TRUE)
	{
		pDoc->m_bSCH_VisibleGridShow = FALSE;
		strcpy(string, "V:隐藏");
	}
	else 
	{
		pDoc->m_bSCH_VisibleGridShow = TRUE;
		strcpy(string, "V:显示");
	}

	pFrame->Set_Visible(string);
	Invalidate();
}

void CESDView::OnUpdateGridToggleElectric(CCmdUI *pCmdUI)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc->m_bSCH_ElecGridEnable == TRUE) pCmdUI->SetCheck(1);
	else	pCmdUI->SetCheck(0);
}

void CESDView::OnUpdateGridToggleVisible(CCmdUI *pCmdUI)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc->m_bSCH_VisibleGridShow == TRUE) pCmdUI->SetCheck(1);
	else	pCmdUI->SetCheck(0);
}

void CESDView::OnPcbGridElectrical()
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	char string[100];
	if(pDoc->m_cPCB_Sheet.m_bElecGridEnable == TRUE)
	{
		pDoc->m_cPCB_Sheet.m_bElecGridEnable = FALSE;
		strcpy(string, "E:禁止");
	}
	else 
	{
		pDoc->m_cPCB_Sheet.m_bElecGridEnable = TRUE;
		strcpy(string, "E:允许");
	}

	pFrame->Set_Electrical(string);
}

void CESDView::OnPcbGridVisible()
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	char string[100];
	if((pDoc->m_sPCB_LayerParam[VISIBLEGRID1].bLayerShow == TRUE)||(pDoc->m_sPCB_LayerParam[VISIBLEGRID2].bLayerShow == TRUE))
	{
		pDoc->m_sPCB_LayerParam[VISIBLEGRID1].bLayerShow = FALSE;
		pDoc->m_sPCB_LayerParam[VISIBLEGRID2].bLayerShow = FALSE;
	}
	else 
	{
		pDoc->m_sPCB_LayerParam[VISIBLEGRID1].bLayerShow = TRUE;
		pDoc->m_sPCB_LayerParam[VISIBLEGRID2].bLayerShow = TRUE;
	}

	Invalidate();
}

void CESDView::OnUpdatePcbGridElectrical(CCmdUI *pCmdUI)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc->m_cPCB_Sheet.m_bElecGridEnable == TRUE) pCmdUI->SetCheck(1);
	else	pCmdUI->SetCheck(0);
}

void CESDView::OnUpdatePcbGridVisible(CCmdUI *pCmdUI)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if((pDoc->m_sPCB_LayerParam[VISIBLEGRID1].bLayerShow == TRUE)||(pDoc->m_sPCB_LayerParam[VISIBLEGRID2].bLayerShow == TRUE)) pCmdUI->SetCheck(1);
	else	pCmdUI->SetCheck(0);
}

void CESDView::GetAllObjectsRect(Fpoint &fp1, Fpoint &fp2)
{
	double fminx, fminy, fmaxx, fmaxy;
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	fminx = fminy = 100000;
	fmaxx = fmaxy = -100000;

	int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
	POSITION pos;
	pos = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{	
		if(pos == NULL) break;

		pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;

			for(int i=0; i<pline->m_cVertex.GetCount(); i++)
			{
				fminx = MIN(fminx, pline->m_cVertex[i].fx);
				fmaxx = MAX(fmaxx, pline->m_cVertex[i].fx);
				fminy = MIN(fminy, pline->m_cVertex[i].fy);
				fmaxy = MAX(fmaxy, pline->m_cVertex[i].fy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;

			for(int i=0; i<ppolyline->m_cVertex.GetCount(); i++)
			{
				fminx = MIN(fminx, ppolyline->m_cVertex[i].fx);
				fmaxx = MAX(fmaxx, ppolyline->m_cVertex[i].fx);
				fminy = MIN(fminy, ppolyline->m_cVertex[i].fy);
				fmaxy = MAX(fmaxy, ppolyline->m_cVertex[i].fy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;

			fminx = MIN(fminx, pjunc->m_fCx);
			fmaxx = MAX(fmaxx, pjunc->m_fCx);
			fminy = MIN(fminy, pjunc->m_fCy);
			fmaxy = MAX(fmaxy, pjunc->m_fCy);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;

			fminx = MIN(fminx, ptext->m_fSx);
			fmaxx = MAX(fmaxx, ptext->m_fSx);
			fminy = MIN(fminy, ptext->m_fSy);
			fmaxy = MAX(fmaxy, ptext->m_fSy);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;

			fminx = MIN(fminx, parc->m_fCx);
			fmaxx = MAX(fmaxx, parc->m_fCx);
			fminy = MIN(fminy, parc->m_fCy);
			fmaxy = MAX(fmaxy, parc->m_fCy);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;

			fminx = MIN(fminx, pellipse->m_fCx);
			fmaxx = MAX(fmaxx, pellipse->m_fCx);
			fminy = MIN(fminy, pellipse->m_fCy);
			fmaxy = MAX(fmaxy, pellipse->m_fCy);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;

			fminx = MIN(fminx, pellipsefill->m_fCx);
			fmaxx = MAX(fmaxx, pellipsefill->m_fCx);
			fminy = MIN(fminy, pellipsefill->m_fCy);
			fmaxy = MAX(fmaxy, pellipsefill->m_fCy);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;

			fminx = MIN(fminx, ppie->m_fCx);
			fmaxx = MAX(fmaxx, ppie->m_fCx);
			fminy = MIN(fminy, ppie->m_fCy);
			fmaxy = MAX(fmaxy, ppie->m_fCy);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;

			for(int i=0; i<ppolygon->m_cVertex.GetCount(); i++)
			{
				fminx = MIN(fminx, ppolygon->m_cVertex[i].fx);
				fmaxx = MAX(fmaxx, ppolygon->m_cVertex[i].fx);
				fminy = MIN(fminy, ppolygon->m_cVertex[i].fy);
				fmaxy = MAX(fmaxy, ppolygon->m_cVertex[i].fy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;

			fminx = MIN(fminx, ptextframe->m_fX1);
			fmaxx = MAX(fmaxx, ptextframe->m_fX2);
			fminy = MIN(fminy, ptextframe->m_fY1);
			fmaxy = MAX(fmaxy, ptextframe->m_fY2);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;

			fminx = MIN(fminx, prect->m_fX1);
			fmaxx = MAX(fmaxx, prect->m_fX2);
			fminy = MIN(fminy, prect->m_fY1);
			fmaxy = MAX(fmaxy, prect->m_fY2);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;

			fminx = MIN(fminx, proundrect->m_fX1);
			fmaxx = MAX(fmaxx, proundrect->m_fX2);
			fminy = MIN(fminy, proundrect->m_fY1);
			fmaxy = MAX(fmaxy, proundrect->m_fY2);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;
			if((psheetsymbol->m_nDiagram != m_nDiagram)||(psheetsymbol->m_nFlag == FLAG_DELETED))
			{
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);

				continue;
			}

			fminx = MIN(fminx, psheetsymbol->m_fX);
			fmaxx = MAX(fmaxx, psheetsymbol->m_fX + psheetsymbol->m_fXsize);
			fminy = MIN(fminy, psheetsymbol->m_fY - psheetsymbol->m_fYsize);
			fmaxy = MAX(fmaxy, psheetsymbol->m_fY);

			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);

		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;

			fminx = MIN(fminx, pport->m_fX);
			fmaxx = MAX(fmaxx, pport->m_fX + pport->m_fSize);
			fminy = MIN(fminy, pport->m_fY - pport->m_fSize);
			fmaxy = MAX(fmaxy, pport->m_fY);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;

			fminx = MIN(fminx, ppower->m_fX);
			fmaxx = MAX(fmaxx, ppower->m_fX+100);
			fminy = MIN(fminy, ppower->m_fY);
			fmaxy = MAX(fmaxy, ppower->m_fY+100);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;

			for(int i=0; i<pbezier->m_cVertex.GetCount(); i++)
			{
				fminx = MIN(fminx, pbezier->m_cVertex[i].fx);
				fmaxx = MAX(fmaxx, pbezier->m_cVertex[i].fx);
				fminy = MIN(fminy, pbezier->m_cVertex[i].fy);
				fmaxy = MAX(fmaxy, pbezier->m_cVertex[i].fy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;

			fminx = MIN(fminx, pgraphic->m_fX1);
			fmaxx = MAX(fmaxx, pgraphic->m_fX2);
			fminy = MIN(fminy, pgraphic->m_fY1);
			fmaxy = MAX(fmaxy, pgraphic->m_fY2);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram != m_nDiagram)||(pcomp->m_nFlag == FLAG_DELETED))	
			{
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);

				continue;
			}

			RECT rect = pcomp->GetCompRect();
			fminx = MIN(fminx, rect.left); //pcomp->m_fX);
			fmaxx = MAX(fmaxx, rect.right); //pcomp->m_fX);
			fminy = MIN(fminy, rect.bottom); //pcomp->m_fY);
			fmaxy = MAX(fmaxy, rect.top); //pcomp->m_fY);
			
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
	}	
	
	fp1.fx = fminx - 200;
	fp1.fy = fminy - 200;
	fp2.fx = fmaxx + 200;
	fp2.fy = fmaxy + 200;
}

void CESDView::GetAllPCBObjectsRect(Fpoint &fp1, Fpoint &fp2)
{
	double fminx, fminy, fmaxx, fmaxy;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	fminx = fminy = 100000;
	fmaxx = fmaxy = -100000;

	
	if(pDoc->m_cPCB_Sheet.m_bShowSheet == TRUE)
	{
		fminx = pDoc->m_cPCB_Sheet.m_fSheetX;
		fminy = pDoc->m_cPCB_Sheet.m_fSheetY;
		fmaxx = pDoc->m_cPCB_Sheet.m_fSheetX + pDoc->m_cPCB_Sheet.m_fSheetWidth;
		fmaxy = pDoc->m_cPCB_Sheet.m_fSheetY + pDoc->m_cPCB_Sheet.m_fSheetHeight;
	}

	
	for(int i=0; i<pDoc->m_cPCB_Sheet.m_cVertex.GetSize(); i++)
	{
		fminx = MIN(fminx, pDoc->m_cPCB_Sheet.m_cVertex[i].fx);
		fminy = MIN(fminy, pDoc->m_cPCB_Sheet.m_cVertex[i].fy);
		fmaxx = MAX(fmaxx, pDoc->m_cPCB_Sheet.m_cVertex[i].fx);
		fmaxy = MAX(fmaxy, pDoc->m_cPCB_Sheet.m_cVertex[i].fy);
	}
	
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* ppcbarc = pDoc->m_arrayPcbArc.GetAt(i);
		if(ppcbarc->m_nFlag == FLAG_NORMAL)
		{
			fmaxx = MAX(fmaxx, ppcbarc->m_fCx + ppcbarc->m_fRadius);
			fmaxy = MAX(fmaxy, ppcbarc->m_fCy + ppcbarc->m_fRadius);
			fminx = MIN(fminx, ppcbarc->m_fCx - ppcbarc->m_fRadius);
			fminy = MIN(fminy, ppcbarc->m_fCy - ppcbarc->m_fRadius);
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if(ptrack->m_nFlag == FLAG_NORMAL)
		{
			double fx1, fy1, fx2, fy2;
			fx1 = MIN(ptrack->m_fX1, ptrack->m_fX2);
			fy1 = MIN(ptrack->m_fY1, ptrack->m_fY2);
			fx2 = MAX(ptrack->m_fX1, ptrack->m_fX2);
			fy2 = MAX(ptrack->m_fY1, ptrack->m_fY2);
			fmaxx = MAX(fmaxx, fx2);
			fmaxy = MAX(fmaxy, fy2);
			fminx = MIN(fminx, fx1);
			fminy = MIN(fminy, fy1);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag == FLAG_NORMAL)
		{
			fmaxx = MAX(fmaxx, pvia->m_fCx + pvia->m_fDia/2);
			fmaxy = MAX(fmaxy, pvia->m_fCy + pvia->m_fDia/2);
			fminx = MIN(fminx, pvia->m_fCx - pvia->m_fDia/2);
			fminy = MIN(fminy, pvia->m_fCy - pvia->m_fDia/2);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ppcbtext = pDoc->m_arrayPcbText.GetAt(i);
		if(ppcbtext->m_nFlag == FLAG_NORMAL)
		{
			fmaxx = MAX(fmaxx, ppcbtext->m_fSx);
			fmaxy = MAX(fmaxy, ppcbtext->m_fSy);
			fminx = MIN(fminx, ppcbtext->m_fSx);
			fminy = MIN(fminy, ppcbtext->m_fSy);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* ppcbfill = pDoc->m_arrayPcbFill.GetAt(i);
		if(ppcbfill->m_nFlag == FLAG_NORMAL)
		{
			fmaxx = MAX(fmaxx, MAX(ppcbfill->m_fX1, ppcbfill->m_fX2));
			fmaxy = MAX(fmaxy, MAX(ppcbfill->m_fY1, ppcbfill->m_fY2));
			fminx = MIN(fminx, MIN(ppcbfill->m_fX1, ppcbfill->m_fX2));
			fminy = MIN(fminy, MIN(ppcbfill->m_fY1, ppcbfill->m_fY2));
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag == FLAG_NORMAL)
		{
			fmaxx = MAX(fmaxx, ppad->m_fCx + ppad->m_fSizeX[0]);
			fmaxy = MAX(fmaxy, ppad->m_fCy + ppad->m_fSizeY[0]);
			fminx = MIN(fminx, ppad->m_fCx - ppad->m_fSizeX[0]);
			fminy = MIN(fminy, ppad->m_fCy - ppad->m_fSizeY[0]);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if(pregion->m_nFlag == FLAG_NORMAL)
		{
			RECT rect = pregion->GetRegionRect();
			fmaxx = MAX(fmaxx, (double)rect.right);
			fmaxy = MAX(fmaxy, (double)rect.top);
			fminx = MIN(fminx, (double)rect.left);
			fminy = MIN(fminy, (double)rect.bottom);
		}
	}

	fp1.fx = fminx - 200;
	fp1.fy = fminy - 200;
	fp2.fx = fmaxx + 200;
	fp2.fy = fmaxy + 200;
}

void CESDView::OnViewFlipBoard()
{
	/*if(m_bPcbFlipView == TRUE) m_bPcbFlipView = FALSE;
	else m_bPcbFlipView = TRUE;

	Invalidate();*/

	m_bPcbFlipView = TRUE;
	Invalidate();

	unsigned ch1;
	CClientDC dc(this);

	Write_Help("移动鼠标或光标键到窗口边缘处自动滚屏");

	do
	{
       ch1 = MouseKey();
       if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN)) break;

	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;


	m_bPcbFlipView = FALSE;
	Invalidate();
}

void CESDView::OnViewFitBoard()
{
	double scale_x, scale_y;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(nEDA_System_State == SCH)
	{
	}
	else
	{
		double minfx, minfy, maxfx, maxfy;
		pDoc->m_cPCB_Sheet.GetBoardRect(&minfx, &minfy, &maxfx, &maxfy);

		scale_x = (maxfx - minfx)/m_nViewport_Ex;
		scale_y = (maxfy - minfy)/m_nViewport_Ey;

		if(scale_x > scale_y)
		{
			m_fScale = scale_x;
			m_fScreenX = minfx;
			m_fScreenY = minfy;
		}
		else
		{
			m_fScale = scale_y;
			m_fScreenX = minfx;
			m_fScreenY = minfy;
		}
	}

	UpdateZoomSlider();
	Screen_XYControl();
	ScrollBar_Control();
	Invalidate();
}

void CESDView::DefinePrintArea()
{
	unsigned ch1;
	double fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;
	
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	MoveCursorToCenter();

	Write_Help("定义打印区域一角");
	flag = 0;

	int drawmode;
	drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			fx1 = m_fCurrentSnapX; fy1 = m_fCurrentSnapY;
			Get_VPort_XY(&xs,&ys,fx1,fy1);
			xe=xs;
			ye=ys;
			Write_Help("定义打印区域另一角");
			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xs,&ys,fx1,fy1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					goto ZOOMBREAK;

				}
				if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{ 
						flag=1;
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xs,ye);
						dc.LineTo(xs,ys);
					}
					else
					{
						if(nEDA_RedrawFlag == ON){ nEDA_RedrawFlag = OFF;}
						else 
						{ 
							dc.MoveTo(xs,ys);
							dc.LineTo(xe,ys);
							dc.LineTo(xe,ye);
							dc.LineTo(xs,ye);
							dc.LineTo(xs,ys);
						}
						fx2 = m_fCurrentSnapX; fy2 = m_fCurrentSnapY;
						Get_VPort_XY(&xe,&ye,fx2,fy2);
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xs,ye);
						dc.LineTo(xs,ys);
					}
				}
				if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{  
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					goto ZOOMBREAK;
				}
				else Command_In_Function(ch1);
			}while(1);
		}
		else Command_In_Function(ch1);
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

ZOOMBREAK:
	if((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC))
	{
		double ftemp;
		if(fx1 > fx2)
		{
			ftemp = fx2;
			fx2 = fx1;
			fx1 = ftemp;
		}
		if(fy1 > fy2)
		{
			ftemp = fy2;
			fy2 = fy1;
			fy1 = ftemp;
		}

		pDoc->m_fPCBPrintX1 = fx1;
		pDoc->m_fPCBPrintY1 = fy1;
		pDoc->m_fPCBPrintX2 = fx2;
		pDoc->m_fPCBPrintY2 = fy2;
	}
	
	dc.SetROP2(drawmode);
	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}
