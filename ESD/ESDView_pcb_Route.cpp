void CESDView::OnRouteManual()
{
	int ch1;
	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4, tempx, tempy;
	int x1,y1,x2,y2,x3,y3,x4,y4,flag;
	double fcx, fcy, fradius, fsx, fsy, fex, fey, fsangle, feangle;
	int cx, cy, sx, sy, ex, ey, radius;
	int drawmode;
	char szPlaceLineStart[]  = "手工布线: <左键><回车>定义起点, <右键><ESC>结束命令";
	char szPlaceLineEnd[]  = "手工布线: <左键><回车>定义终点, <右键><ESC>返回上一步, <TAB>定义属性";
	char szInfo[9][100] = {	"<shift+空格><空格>修改模式: 90度-先水平再垂直",	"<shift+空格><空格>修改模式: 90度-先垂直再水平",\
							"<shift+空格><空格>修改模式: 90度-先圆弧再连线",	"<shift+空格><空格>修改模式: 90度-先连线再圆弧",\
							"<shift+空格><空格>修改模式: 45度-先水平垂直再45度","<shift+空格><空格>修改模式: 45度-先45度再水平垂直",\
							"<shift+空格><空格>修改模式: 45度-先圆弧再连线",	"<shift+空格><空格>修改模式: 45度-先连线再圆弧",\
							"<shift+空格><空格>修改模式: 任意角度"};
	
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	if(m_nPCB_Current_Layer > BOTTOMLAYER) 
	{
		AfxMessageBox("只能在信号层(顶层, 中间1-30层, 底层)上布线, 当前层不是信号层, 命令终止.");
		return;
	}

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	Write_Help(szPlaceLineStart);

	int width = cEDA_pcbtrack.m_fWidth/m_fScale;
	if(width < 1) width = 1;

	int layer = m_nPCB_Current_Layer;
	//COLORREF color = (~nEDA_PCB_LayerColor[m_nPCB_Current_Layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
	COLORREF color = nEDA_PCB_LayerColor[m_nPCB_Current_Layer];

	CPen *OverwritePen1,  *OverwritePen2, *oldpen;
	OverwritePen1 = new CPen(PS_SOLID, width, color);
	OverwritePen2 = new CPen(PS_DOT, 1, color);
					
	
	bEDA_NeedElectricalGrid = TRUE;

	flag = 1;
	nEDA_RedrawFlag = OFF;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			PcbElecHotPointsControl(&dc);

			fx1 = m_fCurrentSnapX;
			fy1 = m_fCurrentSnapY;
			fx2 = fx3 = fx4 = fx1; 
			fy2 = fy3 = fy4 = fy1;
			fradius = 0;

			
			int nRouteNet = -1;
			
			for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
				if( (ppad->Is_On_Me(fx1, fy1) == TRUE)&&(ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nNet >= 0)&&\
					((ppad->m_nLayer == MULTILAYER)||(ppad->m_nLayer == m_nPCB_Current_Layer)) )
				{
					nRouteNet = ppad->m_nNet;
					break;
				}
			}
			
			if(nRouteNet == -1)
			{
				for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
				{
					COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);

					if( (pvia->Is_On_Me(fx1, fy1) == TRUE)&&(pvia->m_nFlag == FLAG_NORMAL)&&(pvia->m_nNet >= 0)&&\
						(pDoc->IsConnectedToVia(m_nPCB_Current_Layer, pvia) == TRUE) )
					{
						nRouteNet = pvia->m_nNet;
						break;
					}
				}
			}
			if(nRouteNet == -1)
			{
				for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
					if( (ptrack->m_nPolygon == -1)&&(ptrack->m_nComponent == -1)&&(ptrack->m_nFlag == FLAG_NORMAL)&&\
						((ptrack->Is_On_Me(fx1, fy1) == TRUE)||((fabs(fx1 - ptrack->m_fX1) < ptrack->m_fWidth/2)&&(fabs(fy1 - ptrack->m_fY1) < ptrack->m_fWidth/2))||((fabs(fx1 - ptrack->m_fX2) < ptrack->m_fWidth/2)&&(fabs(fy1 - ptrack->m_fY2) < ptrack->m_fWidth/2)))&&\
						(ptrack->m_nNet >= 0)&&((ptrack->m_nLayer == MULTILAYER)||(ptrack->m_nLayer == m_nPCB_Current_Layer)) )
					{
						nRouteNet = ptrack->m_nNet;
						break;
					}
				}
			}
			if(nRouteNet == -1)
			{
				for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
				{
					COb_pcbarc* ppcbarc = pDoc->m_arrayPcbArc.GetAt(i);
					double fxs, fys, fxe, fye;
					fxs = ppcbarc->m_fCx + ppcbarc->m_fRadius*cos(ppcbarc->m_fSangle*fEDA_pi/180);
					fys = ppcbarc->m_fCy + ppcbarc->m_fRadius*sin(ppcbarc->m_fSangle*fEDA_pi/180);
					fxe = ppcbarc->m_fCx + ppcbarc->m_fRadius*cos(ppcbarc->m_fEangle*fEDA_pi/180);
					fye = ppcbarc->m_fCy + ppcbarc->m_fRadius*sin(ppcbarc->m_fEangle*fEDA_pi/180);
					if( (ppcbarc->m_nPolygon == -1)&&(ppcbarc->m_nComponent == -1)&&(ppcbarc->m_nFlag == FLAG_NORMAL)&&\
						((ppcbarc->Is_On_Me(fx1, fy1) == TRUE)||((fabs(fx1 - fxs) < ppcbarc->m_fWidth/2)&&(fabs(fy1 - fys) < ppcbarc->m_fWidth/2))||((fabs(fx1 - fxe) < ppcbarc->m_fWidth/2)&&(fabs(fy1 - fye) < ppcbarc->m_fWidth/2)))&&\
						(ppcbarc->m_nNet >= 0)&&((ppcbarc->m_nLayer == MULTILAYER)||(ppcbarc->m_nLayer == m_nPCB_Current_Layer)) )
					{
						nRouteNet = ppcbarc->m_nNet;
						break;
					}
				}
			}
			

			Write_Help(szPlaceLineEnd);
			Write_Info(szInfo[nEDA_PCB_PlaceTrackMode]);
			do
			{
				ch1 = MouseKey();
LABEL_1:
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{ 
					PcbElecHotPointsControl(&dc);

					Write_Help(szPlaceLineStart);
					Write_Info(szEDA_Info_Ready);

					
					drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);

					oldpen = dc.SelectObject(OverwritePen1);
					
					Get_VPort_XY(&x1,&y1,fx1,fy1);
					Get_VPort_XY(&x2,&y2,fx2,fy2);							
					if((x1 != x2)||(y1 != y2))
					{
						dc.MoveTo(x1,y1);
						dc.LineTo(x2,y2);
					}
					
					if(fradius != 0)
					{
						Get_VPort_XY(&cx, &cy, fcx, fcy);
						radius = fradius/m_fScale;
						Get_VPort_XY(&sx, &sy, fsx, fsy);
						Get_VPort_XY(&ex, &ey, fex, fey);
						dc.Arc(cx - radius, cy - radius, cx + radius, cy + radius, sx, sy, ex, ey);
					}
					
					dc.SelectObject(OverwritePen2);
					Get_VPort_XY(&x3,&y3,fx3,fy3);
					Get_VPort_XY(&x4,&y4,fx4,fy4);
					DrawDottedTrack(x3,y3,x4,y4,width/2, &dc);
					
					if(layer != m_nPCB_Current_Layer)
					{
						Get_VPort_XY(&sx, &sy, fx1 - pDoc->m_fRulesViaDia/2, fy1 - pDoc->m_fRulesViaDia/2);
						Get_VPort_XY(&ex, &ey, fx1 + pDoc->m_fRulesViaDia/2, fy1 + pDoc->m_fRulesViaDia/2);								
						dc.Ellipse(sx, sy, ex, ey);
						Get_VPort_XY(&sx, &sy, fx1 - pDoc->m_fRulesViaHoleDia/2, fy1 - pDoc->m_fRulesViaHoleDia/2);
						Get_VPort_XY(&ex, &ey, fx1 + pDoc->m_fRulesViaHoleDia/2, fy1 + pDoc->m_fRulesViaHoleDia/2);								
						dc.Ellipse(sx, sy, ex, ey);
					}
					dc.SelectObject(oldpen);
					dc.SetROP2(drawmode);
					

					ch1 = MOUSEMOVE;
					break;
				}
				else if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{ 
						flag = 1;

					}
					else
					{
						if(nEDA_RedrawFlag == ON)
						{ 
							nEDA_RedrawFlag = OFF;
						}
						else  
						{
							
							drawmode = dc.GetROP2();
							dc.SetROP2(R2_NOTXORPEN);							
							
							PcbElecHotPointsControl(&dc);

							oldpen = dc.SelectObject(OverwritePen1);
							
							Get_VPort_XY(&x1,&y1,fx1,fy1);
							Get_VPort_XY(&x2,&y2,fx2,fy2);							
							if((x1 != x2)||(y1 != y2))
							{
								dc.MoveTo(x1,y1);
								dc.LineTo(x2,y2);
							}
							
							if(fradius != 0)
							{
								Get_VPort_XY(&cx, &cy, fcx, fcy);
								radius = fradius/m_fScale;
								Get_VPort_XY(&sx, &sy, fsx, fsy);
								Get_VPort_XY(&ex, &ey, fex, fey);
								dc.Arc(cx - radius, cy - radius, cx + radius, cy + radius, sx, sy, ex, ey);
							}
							
							dc.SelectObject(OverwritePen2);
							Get_VPort_XY(&x3,&y3,fx3,fy3);
							Get_VPort_XY(&x4,&y4,fx4,fy4);
							DrawDottedTrack(x3,y3,x4,y4,width/2, &dc);
							
							if(layer != m_nPCB_Current_Layer)
							{
								Get_VPort_XY(&sx, &sy, fx1 - pDoc->m_fRulesViaDia/2, fy1 - pDoc->m_fRulesViaDia/2);
								Get_VPort_XY(&ex, &ey, fx1 + pDoc->m_fRulesViaDia/2, fy1 + pDoc->m_fRulesViaDia/2);								
								dc.Ellipse(sx, sy, ex, ey);
								Get_VPort_XY(&sx, &sy, fx1 - pDoc->m_fRulesViaHoleDia/2, fy1 - pDoc->m_fRulesViaHoleDia/2);
								Get_VPort_XY(&ex, &ey, fx1 + pDoc->m_fRulesViaHoleDia/2, fy1 + pDoc->m_fRulesViaHoleDia/2);								
								dc.Ellipse(sx, sy, ex, ey);
							}
							dc.SelectObject(oldpen);
							dc.SetROP2(drawmode);
							
						}

						
						width = cEDA_pcbtrack.m_fWidth/m_fScale;
						if(width < 1) width = 1;
						delete OverwritePen1;
						OverwritePen1 = new CPen(PS_SOLID, width, color);
						delete OverwritePen2;
						OverwritePen2 = new CPen(PS_DOT, 1, color);

						fx4 = fx3 = fx2 = m_fCurrentSnapX; 
						fy4 = fy3 = fy2 = m_fCurrentSnapY;
						fradius = 0;

						if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_START)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {} 
							else if(fx4 == fx1){} 
							else if(fy4 == fy1){}
							else
							{
								fy2 = fy3 = fy1;
							}
						}
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_END)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {} 
							else if(fx4 == fx1){} 
							else if(fy4 == fy1){}
							else
							{
								fx2 = fx3 = fx1;
							}
						}
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90ARC_START)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {} 
							else if(fx4 == fx1) {} 
							else if(fy4 == fy1) {}
							else
							{
								double deltax, deltay;
								deltax = fx4 - fx1;
								deltay = fy4 - fy1;
								//fx2 = fx1;	
								//fy2 = fy1;
								if( abs(deltax) > abs(deltay) )
								{
									fradius = abs(deltay);
									if(fradius <= fEDA_AutoArcMaxRadius)
									{
										fx2 = fx1;	
										fy2 = fy1;
										if(fx4 > fx1)
										{
											fcx = fx1 + fradius;
											fcy = fy1;
											if(fy4 > fy1)
											{
												fsangle = 90;	feangle = 180;
												fsx = fcx;		fsy = fy4;
												fex = fx2;		fey = fy2;
											}
											else
											{
												fsangle = 180; feangle = 270;
												fsx = fx2;		fsy = fy2;
												fex = fcx;		fey = fy4;
											}
										}
										else
										{
											fcx = fx1 - fradius;
											fcy = fy1;
											if(fy4 > fy1)
											{
												fsangle = 0; feangle = 90;
												fsx = fx2;  fsy = fy2;
												fex = fcx;  fey = fy4;
											}
											else
											{
												fsangle = 270; feangle = 360;
												fsx = fcx;		fsy = fy4;
												fex = fx2;		fey = fy2;
											}
										}
									}
									else
									{
										fradius = fEDA_AutoArcMaxRadius;
										fx2 = fx1;
										
										if(fx4 > fx1)
										{
											fcx = fx1 + fradius;
											if(fy4 > fy1)
											{
												fcy = fy4 - fradius;
												fy2 = fcy;
												fsangle = 90;	feangle = 180;
												fsx = fcx;		fsy = fy4;
												fex = fx2;		fey = fy2;
											}
											else
											{
												fcy = fy4 + fradius;
												fy2 = fcy;
												fsangle = 180; feangle = 270;
												fsx = fx2;		fsy = fy2;
												fex = fcx;		fey = fy4;
											}
										}
										else
										{
											fcx = fx1 - fradius;
											if(fy4 > fy1)
											{
												fcy = fy4 - fradius;
												fy2 = fcy;
												fsangle = 0; feangle = 90;
												fsx = fx2;  fsy = fy2;
												fex = fcx;  fey = fy4;
											}
											else
											{
												fcy = fy4 + fradius;
												fy2 = fcy;
												fsangle = 270; feangle = 360;
												fsx = fcx;		fsy = fy4;
												fex = fx2;		fey = fy2;
											}
										}

									}
									fx3 = fcx;	
									fy3 = fy4;
								}
								else
								{
									fradius = abs(deltax);
									if(fradius <= fEDA_AutoArcMaxRadius)
									{
										fx2 = fx1;	
										fy2 = fy1;
										if(fy4 > fy1)
										{
											fcx = fx1;
											fcy = fy1 + fradius;
											if(fx4 > fx1)
											{
												fsangle = 270; feangle = 360;
												fsx = fx2;	fsy = fy2;
												fex = fx4;	fey = fcy;
											}
											else
											{
												fsangle = 180; feangle = 270;
												fsx = fx4;	fsy = fcy;
												fex = fx2;	fey = fy2;
											}
										}
										else
										{
											fcx = fx1;
											fcy = fy1 - fradius;
											if(fx4 > fx1)
											{
												fsangle = 0; feangle = 90;
												fsx = fx4;	fsy = fcy;
												fex = fx2;	fey = fy2;
											}
											else
											{
												fsangle = 90; feangle = 180;
												fsx = fx2;	fsy = fy2;
												fex = fx4;	fey = fcy;
											}
										}
									}
									else
									{
										fradius = fEDA_AutoArcMaxRadius;
										fy2 = fy1;
										if(fy4 > fy1)
										{
											fcy = fy1 + fradius;
											if(fx4 > fx1)
											{
												fcx = fx4 - fradius;
												fx2 = fcx;
												fsangle = 270; feangle = 360;
												fsx = fx2;	fsy = fy2;
												fex = fx4;	fey = fcy;
											}
											else
											{
												fcx = fx4 + fradius;
												fx2 = fcx;
												fsangle = 180; feangle = 270;
												fsx = fx4;	fsy = fcy;
												fex = fx2;	fey = fy2;
											}
										}
										else
										{
											fcy = fy1 - fradius;
											if(fx4 > fx1)
											{
												fcx = fx4 - fradius;
												fx2 = fcx;
												fsangle = 0; feangle = 90;
												fsx = fx4;	fsy = fcy;
												fex = fx2;	fey = fy2;
											}
											else
											{
												fcx = fx4 + fradius;
												fx2 = fcx;
												fsangle = 90; feangle = 180;
												fsx = fx2;	fsy = fy2;
												fex = fx4;	fey = fcy;
											}
										}
									}
									fx3 = fx4;	
									fy3 = fcy;
								}
							}
						}
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90ARC_END)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {} 
							else if(fx4 == fx1) {} 
							else if(fy4 == fy1) {}
							else
							{
								double deltax, deltay;
								deltax = fx4 - fx1;
								deltay = fy4 - fy1;
								if( abs(deltax) > abs(deltay) )
								{
									fradius = abs(deltay);
									if(fradius <= fEDA_AutoArcMaxRadius)
									{
										fx3 = fx4;	
										fy3 = fy4;
										if(fx4 > fx1)
										{
											fcx = fx4 - fradius;
											fcy = fy4;
											if(fy4 > fy1)
											{
												fsangle = 270;	feangle = 360;
												fsx = fcx;		fsy = fy1;
												fex = fx3;		fey = fy3;
											}
											else
											{
												fsangle = 0; feangle = 90;
												fsx = fx3;		fsy = fy3;
												fex = fcx;		fey = fy1;
											}
										}
										else
										{
											fcx = fx4 + fradius;
											fcy = fy4;
											if(fy4 > fy1)
											{
												fsangle = 180; feangle = 270;
												fsx = fx3;  fsy = fy3;
												fex = fcx;  fey = fy1;
											}
											else
											{
												fsangle = 90; feangle = 180;
												fsx = fcx;		fsy = fy1;
												fex = fx3;		fey = fy3;
											}
										}
									}
									else
									{
										fradius = fEDA_AutoArcMaxRadius;
										fx3 = fx4;
										if(fx4 > fx1)
										{
											fcx = fx4 - fradius;
											if(fy4 > fy1)
											{
												fcy = fy1 + fradius;
												fy3 = fcy;
												fsangle = 270;	feangle = 360;
												fsx = fcx;		fsy = fy1;
												fex = fx3;		fey = fy3;
											}
											else
											{
												fcy = fy1 - fradius;
												fy3 = fcy;
												fsangle = 0; feangle = 90;
												fsx = fx3;		fsy = fy3;
												fex = fcx;		fey = fy1;
											}
										}
										else
										{
											fcx = fx4 + fradius;
											if(fy4 > fy1)
											{
												fcy = fy1 + fradius;
												fy3 = fcy;
												fsangle = 180; feangle = 270;
												fsx = fx3;  fsy = fy3;
												fex = fcx;  fey = fy1;
											}
											else
											{
												fcy = fy1 - fradius;
												fy3 = fcy;
												fsangle = 90; feangle = 180;
												fsx = fcx;		fsy = fy1;
												fex = fx3;		fey = fy3;
											}
										}
									}
									fx2 = fcx;	
									fy2 = fy1;
								}
								else
								{
									fradius = abs(deltax);
									if(fradius <= fEDA_AutoArcMaxRadius)
									{
										fx3 = fx4;
										fy3 = fy4;
										if(fy4 > fy1)
										{
											fcx = fx4;
											fcy = fy4 - fradius;
											if(fx4 > fx1)
											{
												fsangle = 90; feangle = 180;
												fsx = fx3;	fsy = fy3;
												fex = fx1;	fey = fcy;
											}
											else
											{
												fsangle = 0; feangle = 90;
												fsx = fx1;	fsy = fcy;
												fex = fx3;	fey = fy3;
											}
										}
										else
										{
											fcx = fx4;
											fcy = fy4 + fradius;
											if(fx4 > fx1)
											{
												fsangle = 180; feangle = 270;
												fsx = fx1;	fsy = fcy;
												fex = fx3;	fey = fy3;
											}
											else
											{
												fsangle = 270; feangle = 360;
												fsx = fx3;	fsy = fy3;
												fex = fx1;	fey = fcy;
											}
										}
									}
									else
									{
										fradius = fEDA_AutoArcMaxRadius;
										fy3 = fy4;
										if(fy4 > fy1)
										{
											fcy = fy4 - fradius;
											if(fx4 > fx1)
											{
												fcx = fx1 + fradius;
												fx3 = fcx;
												fsangle = 90; feangle = 180;
												fsx = fx3;	fsy = fy3;
												fex = fx1;	fey = fcy;
											}
											else
											{
												fcx = fx1 - fradius;
												fx3 = fcx;
												fsangle = 0; feangle = 90;
												fsx = fx1;	fsy = fcy;
												fex = fx3;	fey = fy3;
											}
										}
										else
										{
											fcy = fy4 + fradius;
											if(fx4 > fx1)
											{
												fcx = fx1 + fradius;
												fx3 = fcx;
												fsangle = 180; feangle = 270;
												fsx = fx1;	fsy = fcy;
												fex = fx3;	fey = fy3;
											}
											else
											{
												fcx = fx1 - fradius;
												fx3 = fcx;
												fsangle = 270; feangle = 360;
												fsx = fx3;	fsy = fy3;
												fex = fx1;	fey = fcy;
											}
										}
									}
									fx2 = fx1;
									fy2 = fcy;
								}
							}
						}
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_START)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {}
							else if(fx4 == fx1){}
							else if(fy4 == fy1){}
							else if(abs(fx4 - fx1) == abs(fy4 - fy1)) {}
							else if((fx4>fx1)&&(fy4>fy1))  
							{  
								tempx = fx4 - fx1;  tempy = fy4 - fy1;
								if(tempx > tempy)
								{  
										fx2 = fx4 - tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 - tempx;
										fx2 = fx1;
								}
							}
							else if((fx1 > fx4)&&(fy4 > fy1))	
							{  
								tempx = fx1 - fx4;  tempy = fy4 - fy1;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										fx2 = fx4 + tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 - tempx;
										fx2 = fx1;
								}
							}
							else if((fx4 > fx1)&&(fy1 > fy4))	
							{  
								tempx = fx4 - fx1;  tempy = fy1 - fy4;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										fx2 = fx4 - tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 + tempx;
										fx2 = fx1;
								}
							}
							else if((fx1 > fx4)&&(fy1 > fy4))	
							{  
								tempx = fx1 - fx4;  tempy = fy1 - fy4;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										fx2 = fx4 + tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 + tempx;
										fx2 = fx1;
								}
							}
							fx3 = fx2; fy3 = fy2;
						}
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_END)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {}
							else if(fx4 == fx1){}
							else if(fy4 == fy1){}
							else if(abs(fx4-fx1) == abs(fy4-fy1)) {}
							else if((fx4>fx1)&&(fy4>fy1))  
							{  
								tempx=fx4-fx1;  tempy=fy4-fy1;
								if(tempx > tempy)
								{  
										fx2 = fx1 + tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 + tempx;
										fx2 = fx4;
								}
							}
							else if((fx1>fx4)&&(fy4>fy1))	
							{  
								tempx=fx1-fx4;  tempy=fy4-fy1;
								if(tempx > tempy)
								{  
										fx2 = fx1 - tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 + tempx;
										fx2 = fx4;
								}
							}
							else if((fx4>fx1)&&(fy1>fy4))	
							{  
								tempx=fx4-fx1;  tempy=fy1-fy4;
								if(tempx > tempy)
								{  
										fx2 = fx1 + tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 - tempx;
										fx2 = fx4;
								}
							}
							else if((fx1>fx4)&&(fy1>fy4))	
							{  
								tempx=fx1-fx4;  tempy=fy1-fy4;
								if(tempx > tempy)
								{  
										fx2 = fx1 - tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 - tempx;
										fx2 = fx4;
								}
							}
							fx3 = fx2; fy3 = fy2;
						}
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45ARC_START)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {}
							else if(fx4 == fx1){}
							else if(fy4 == fy1){}
							else if(abs(fx4 - fx1) == abs(fy4 - fy1)) {}
							else if((fx4>fx1)&&(fy4>fy1))  
							{  
								tempx = fx4 - fx1;  tempy = fy4 - fy1;
								if(tempx > tempy)
								{  
										fx2 = fx4 - tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 - tempx;
										fx2 = fx1;
								}
							}
							else if((fx1 > fx4)&&(fy4 > fy1))	
							{  
								tempx = fx1 - fx4;  tempy = fy4 - fy1;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										fx2 = fx4 + tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 - tempx;
										fx2 = fx1;
								}
							}
							else if((fx4 > fx1)&&(fy1 > fy4))	
							{  
								tempx = fx4 - fx1;  tempy = fy1 - fy4;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										fx2 = fx4 - tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 + tempx;
										fx2 = fx1;
								}
							}
							else if((fx1 > fx4)&&(fy1 > fy4))	
							{  
								tempx = fx1 - fx4;  tempy = fy1 - fy4;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										fx2 = fx4 + tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 + tempx;
										fx2 = fx1;
								}
							}
							fx3 = fx2; fy3 = fy2;
						}
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45ARC_END)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {}
							else if(fx4 == fx1){}
							else if(fy4 == fy1){}
							else if(abs(fx4-fx1) == abs(fy4-fy1)) {}
							else if((fx4>fx1)&&(fy4>fy1))  
							{  
								tempx=fx4-fx1;  tempy=fy4-fy1;
								if(tempx > tempy)
								{  
										fx2 = fx1 + tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 + tempx;
										fx2 = fx4;
								}
							}
							else if((fx1>fx4)&&(fy4>fy1))	
							{  
								tempx=fx1-fx4;  tempy=fy4-fy1;
								if(tempx > tempy)
								{  
										fx2 = fx1 - tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 + tempx;
										fx2 = fx4;
								}
							}
							else if((fx4>fx1)&&(fy1>fy4))	
							{  
								tempx=fx4-fx1;  tempy=fy1-fy4;
								if(tempx > tempy)
								{  
										fx2 = fx1 + tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 - tempx;
										fx2 = fx4;
								}
							}
							else if((fx1>fx4)&&(fy1>fy4))	
							{  
								tempx=fx1-fx4;  tempy=fy1-fy4;
								if(tempx > tempy)
								{  
										fx2 = fx1 - tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 - tempx;
										fx2 = fx4;
								}
							}
							fx3 = fx2; fy3 = fy2;
						}
						else
						{

						}

						
						if(bEDA_PcbOnLineDRCEnable == TRUE)
						{
							if((fx1 != fx2)||(fy1 != fy2))
							{
								COb_pcbtrack atrack;
								atrack.pDocument = pDoc;
								atrack.m_nLayer = m_nPCB_Current_Layer;
								atrack.m_fX1 = fx1;
								atrack.m_fY1 = fy1;
								atrack.m_fX2 = fx2;
								atrack.m_fY2 = fy2;
								atrack.m_fWidth = cEDA_pcbtrack.m_fWidth;
								atrack.m_nNet = nRouteNet;
								if(FindTrackOnLineDrcErrors(&atrack) == TRUE)	
								{
									double ftemp_x1, ftemp_y1, ftemp_x2, ftemp_y2, ftemp_x, ftemp_y;
									ftemp_x1 = fx1;		
									ftemp_y1 = fy1;
									ftemp_x2 = fx2;
									ftemp_y2 = fy2;
									ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
									ftemp_y = (ftemp_y1 + ftemp_y2)/2;
									atrack.m_fX2 = ftemp_x;
									atrack.m_fY2 = ftemp_y;
									double f_x2, f_y2;
									f_x2 = fx1;
									f_y2 = fy1;
									for(;;)
									{
										if(FindTrackOnLineDrcErrors(&atrack) == FALSE)	
										{
											f_x2 = atrack.m_fX2;
											f_y2 = atrack.m_fY2;
											ftemp_x1 = ftemp_x;	
											ftemp_y1 = ftemp_y;
											ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
											ftemp_y = (ftemp_y1 + ftemp_y2)/2;
											atrack.m_fX2 = ftemp_x;
											atrack.m_fY2 = ftemp_y;
										}
										else if(FindTrackOnLineDrcErrors(&atrack) == TRUE)	
										{
											ftemp_x2 = ftemp_x;	
											ftemp_y2 = ftemp_y;
											ftemp_x = (ftemp_x1 + ftemp_x2)/2;		
											ftemp_y = (ftemp_y1 + ftemp_y2)/2;
											atrack.m_fX2 = ftemp_x;
											atrack.m_fY2 = ftemp_y;
										}

										if((fabs(ftemp_x2 - ftemp_x1) < 0.2)&&(fabs(ftemp_y2 - ftemp_y1) < 0.2))
										{
											fx2 = fx3 = fx4 = f_x2;
											fy2 = fy3 = fy4 = f_y2;
											fradius = 0;
											break;
										}
									}
								}
							}
							if(fradius != 0)
							{
								COb_pcbarc aarc;
								aarc.pDocument = pDoc;
								aarc.m_nLayer = m_nPCB_Current_Layer;
								aarc.m_fWidth = cEDA_pcbtrack.m_fWidth;
								aarc.m_fCx = fcx;
								aarc.m_fCy = fcy;
								aarc.m_fRadius = fradius;
								aarc.m_fSangle = fsangle;
								aarc.m_fEangle = feangle;
								aarc.m_nNet = nRouteNet;
								if(FindArcOnLineDrcErrors(&aarc) == TRUE)
								{
									fx2 = fx3 = fx4 = fx1;
									fy2 = fy3 = fy4 = fy1;
									fradius = 0;
								}
							}
						}
						

                        
						drawmode = dc.GetROP2();
						dc.SetROP2(R2_NOTXORPEN);
						
						oldpen = dc.SelectObject(OverwritePen1);
						
						Get_VPort_XY(&x1,&y1,fx1,fy1);
						Get_VPort_XY(&x2,&y2,fx2,fy2);							
						if((x1 != x2)||(y1 != y2))
						{
							dc.MoveTo(x1,y1);
							dc.LineTo(x2,y2);
						}
						
						if(fradius != 0)
						{
							Get_VPort_XY(&cx, &cy, fcx, fcy);
							radius = fradius/m_fScale;
							Get_VPort_XY(&sx, &sy, fsx, fsy);
							Get_VPort_XY(&ex, &ey, fex, fey);
							dc.Arc(cx - radius, cy - radius, cx + radius, cy + radius, sx, sy, ex, ey);
						}
						
						dc.SelectObject(OverwritePen2);
						Get_VPort_XY(&x3,&y3,fx3,fy3);
						Get_VPort_XY(&x4,&y4,fx4,fy4);
						DrawDottedTrack(x3,y3,x4,y4,width/2, &dc);
						
						if(layer != m_nPCB_Current_Layer)
						{
							Get_VPort_XY(&sx, &sy, fx1 - pDoc->m_fRulesViaDia/2, fy1 - pDoc->m_fRulesViaDia/2);
							Get_VPort_XY(&ex, &ey, fx1 + pDoc->m_fRulesViaDia/2, fy1 + pDoc->m_fRulesViaDia/2);								
							dc.Ellipse(sx, sy, ex, ey);
							Get_VPort_XY(&sx, &sy, fx1 - pDoc->m_fRulesViaHoleDia/2, fy1 - pDoc->m_fRulesViaHoleDia/2);
							Get_VPort_XY(&ex, &ey, fx1 + pDoc->m_fRulesViaHoleDia/2, fy1 + pDoc->m_fRulesViaHoleDia/2);								
							dc.Ellipse(sx, sy, ex, ey);
						}

						dc.SelectObject(oldpen);
						dc.SetROP2(drawmode);
						
					}
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					PcbElecHotPointsControl(&dc);
					if((fx1==fx3)&&(fy1==fy3)) continue;

					
					drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);
						
					oldpen = dc.SelectObject(OverwritePen1);
					
					Get_VPort_XY(&x1,&y1,fx1,fy1);
					Get_VPort_XY(&x2,&y2,fx2,fy2);							
					if((x1 != x2)||(y1 != y2))
					{
						dc.MoveTo(x1,y1);
						dc.LineTo(x2,y2);
					}
					
					if(fradius != 0)
					{
						Get_VPort_XY(&cx, &cy, fcx, fcy);
						radius = fradius/m_fScale;
						Get_VPort_XY(&sx, &sy, fsx, fsy);
						Get_VPort_XY(&ex, &ey, fex, fey);
						dc.Arc(cx - radius, cy - radius, cx + radius, cy + radius, sx, sy, ex, ey);
					}
					
					dc.SelectObject(OverwritePen2);
					Get_VPort_XY(&x3,&y3,fx3,fy3);
					Get_VPort_XY(&x4,&y4,fx4,fy4);
					DrawDottedTrack(x3,y3,x4,y4,width/2, &dc);
					
					if(layer != m_nPCB_Current_Layer)
					{
						Get_VPort_XY(&sx, &sy, fx1 - pDoc->m_fRulesViaDia/2, fy1 - pDoc->m_fRulesViaDia/2);
						Get_VPort_XY(&ex, &ey, fx1 + pDoc->m_fRulesViaDia/2, fy1 + pDoc->m_fRulesViaDia/2);								
						dc.Ellipse(sx, sy, ex, ey);
						Get_VPort_XY(&sx, &sy, fx1 - pDoc->m_fRulesViaHoleDia/2, fy1 - pDoc->m_fRulesViaHoleDia/2);
						Get_VPort_XY(&ex, &ey, fx1 + pDoc->m_fRulesViaHoleDia/2, fy1 + pDoc->m_fRulesViaHoleDia/2);								
						dc.Ellipse(sx, sy, ex, ey);
					}
					
					dc.SelectObject(OverwritePen1);
					dc.MoveTo(x3,y3);
					dc.LineTo(x4,y4);

					dc.SelectObject(oldpen);
					dc.SetROP2(drawmode);
					

					
					if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_START) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90_END;
					else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_END) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90_START;
					else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_START) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45_END;
					else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_END) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45_START;
					Write_Info(szInfo[nEDA_PCB_PlaceTrackMode]);
					
					POINT p;
					int ix,iy;
					Get_VPort_XY(&ix,&iy,m_fCurrentSnapX,m_fCurrentSnapY);
					p.x = ix; p.y = iy;
					DrawMyCursor(&dc,&p);
					/*Struct_Pcb_Multi_Selection selected_element_track;
					selected_element_track.index = -1;
					selected_element_track.pcbelement = PCB_ELEMENT_TRACK;
					Struct_Pcb_Multi_Selection selected_element_arc;
					selected_element_arc.index = -1;
					selected_element_arc.pcbelement = PCB_ELEMENT_ARC;*/
					if((fx1 != fx2)||(fy1 != fy2))
					{
						COb_pcbtrack* ptrack = new COb_pcbtrack();
						ptrack->pDocument = pDoc;
						ptrack->m_nLayer = m_nPCB_Current_Layer;
						ptrack->m_fX1 = fx1;
						ptrack->m_fY1 = fy1;
						ptrack->m_fX2 = fx2;
						ptrack->m_fY2 = fy2;
						ptrack->m_fWidth = cEDA_pcbtrack.m_fWidth;
						int index = pDoc->m_arrayPcbTrack.Add(ptrack);
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					
						ptrack->Draw(&dc, this);
						
					}

					if(fradius != 0)
					{
						COb_pcbarc* parc = new COb_pcbarc();
						parc->pDocument = pDoc;
						parc->m_nLayer = m_nPCB_Current_Layer;
						parc->m_fWidth = cEDA_pcbtrack.m_fWidth;
						parc->m_fCx = fcx;
						parc->m_fCy = fcy;
						parc->m_fRadius = fradius;
						parc->m_fSangle = fsangle;
						parc->m_fEangle = feangle;
						int index = pDoc->m_arrayPcbArc.Add(parc);
						
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_ARC;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
						
						parc->Draw(&dc, this);
						
					}

					if(layer != m_nPCB_Current_Layer)
					{
						layer = m_nPCB_Current_Layer;

						COb_pcbvia* pvia = new COb_pcbvia();
						pvia->pDocument = pDoc;
						pvia->m_nStartLayer = TOPLAYER;
						pvia->m_nEndLayer = BOTTOMLAYER;
						pvia->m_fDia = pDoc->m_fRulesViaDia;
						pvia->m_fHoleDia = pDoc->m_fRulesViaHoleDia;
						pvia->m_nNet = nRouteNet;
						pvia->m_fCx = fx1;
						pvia->m_fCy = fy1;

						
						int index = pDoc->m_arrayPcbVia.Add(pvia);
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_VIA;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						

						pvia->Draw(&dc, this, 0);
					}

					
					if((bEDA_PcbConnectionAutoRefresh == TRUE)&&(nRouteNet >= 0)) Refresh_Net_Ratsnest(nRouteNet);
					/*{
						m_arrayPcbHighlight.RemoveAll();
						int nNet = -1;
						if(selected_element_arc.index >= 0)
						{
							COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element_arc.index);
							if(pDoc->IsOnSignalLayer(parc->m_nLayer) == TRUE)
							{
								parc->m_nFlag = FLAG_PROCESSING;
								m_arrayPcbHighlight.Add(selected_element_arc);
								PCBMakeHighlight(FALSE);
								PcbProcessingSelection(FALSE);

								nNet = parc->m_nNet;
							}	
						}
						else if(selected_element_track.index >= 0)
						{
							COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element_track.index);
							if(pDoc->IsOnSignalLayer(ptrack->m_nLayer) == TRUE)
							{
								ptrack->m_nFlag = FLAG_PROCESSING;
								m_arrayPcbHighlight.Add(selected_element_track);
								PCBMakeHighlight(FALSE);
								PcbProcessingSelection(FALSE);
								
								nNet = ptrack->m_nNet;
							}
						}
						m_arrayPcbHighlight.RemoveAll();
					}*/
					DrawMyCursor(&dc,&p);

					fx1 = fx3;  fy1 = fy3;
					fx2 = fx4;  fy2 = fy4;
					fx3 = fx4 = fx2;
					fy3 = fy4 = fy2;
					fradius = 0;
					continue;
				}
				else if(ch1 == SPACE)
				{
					if((GetKeyState(VK_SHIFT)&0x80) != 0)
					{
						
						if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90ARC_START;
						else if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_90ARC_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90ARC_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45_START;
						//else if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45ARC_START;
						//else if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_45ARC_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45ARC_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_ANYANGLE;
						else if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_ANYANGLE;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_ANYANGLE) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90_START; 
					}
					else 
					{
						
						if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_START) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90_END;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_END) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90_START;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90ARC_START) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90ARC_END;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90ARC_END) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90ARC_START;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_START) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45_END;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_END) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45_START;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45ARC_START) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45ARC_END;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45ARC_END) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45ARC_START;
					}
					Write_Info(szInfo[nEDA_PCB_PlaceTrackMode]);
					ch1 = MOUSEMOVE;
					goto LABEL_1;
				}
				else if(ch1 == TAB)
				{
					CDlg_PCBEditTrack dlg;
					
					for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
					{
						dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
					}
					for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
					{
						if(m_nPCB_Current_Layer == pDoc->m_arrayPcbLayerShow.GetAt(i))
						{
							dlg.m_nComboLayer = i;
							break;
						}
					}
					dlg.m_csEditWidth = pDoc->Convert_XY_To_String(cEDA_pcbtrack.m_fWidth);
					if(dlg.DoModal() == IDOK)
					{
						cEDA_pcbtrack.m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditWidth);
					}				
				}
				else if(ch1 ==  VK_ADD)
				{
					
					int l_tag, layerbak;
					layerbak = m_nPCB_Current_Layer;

					CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
					CChildFrame* pChild = (CChildFrame*)pFrame->MDIGetActive();
					if(pChild != NULL)
					{
						CString cstring;
						for(l_tag=0; l_tag<pChild->m_wndSheet.m_strTitles.GetCount(); l_tag++)
						{
							if(pDoc->m_sPCB_LayerParam[m_nPCB_Current_Layer].csLayerName == pChild->m_wndSheet.m_strTitles.GetAt(l_tag)) 
							{
								for(int k=0; k<pChild->m_wndSheet.m_strTitles.GetCount(); k++)
								{
									l_tag++;
									if(l_tag >= pChild->m_wndSheet.m_strTitles.GetCount()) l_tag = 0;
									cstring = pChild->m_wndSheet.m_strTitles.GetAt(l_tag);
									
									for(int j=0; j<=MULTILAYER; j++)
									{ 
										if((pDoc->m_sPCB_LayerParam[j].csLayerName == cstring)&&(j >= TOPLAYER)&&(j <= BOTTOMLAYER))
										{
											m_nPCB_Current_Layer = j;
											break;
										}
									}
									if(m_nPCB_Current_Layer != layerbak) break;
								}
								break;
							}
						}
					}

					if(layerbak != m_nPCB_Current_Layer)
					{
						if(bEDA_PcbOnLineDRCEnable == TRUE) 
						{
							
							COb_pcbvia avia;
							avia.pDocument = pDoc;
							avia.m_nStartLayer = TOPLAYER;
							avia.m_nEndLayer = BOTTOMLAYER;
							avia.m_fDia = pDoc->m_fRulesViaDia;
							avia.m_fHoleDia = pDoc->m_fRulesViaHoleDia;
							avia.m_nNet = nRouteNet;
							avia.m_fCx = fx1;
							avia.m_fCy = fy1;
							if(FindViaOnLineDrcErrors(&avia) == FALSE)
							{
								Invalidate();
								CWnd* pWnd = GetParent();
								pWnd->Invalidate();
								
								
								color = nEDA_PCB_LayerColor[m_nPCB_Current_Layer];
								width = cEDA_pcbtrack.m_fWidth/m_fScale;
								if(width < 1) width = 1;
								delete OverwritePen1;
								OverwritePen1 = new CPen(PS_SOLID, width, color);
								delete OverwritePen2;
								OverwritePen2 = new CPen(PS_DOT, 1, color);				
							}
							else m_nPCB_Current_Layer = layerbak;							
						}
						else m_nPCB_Current_Layer = layerbak;
					}
				}
				else if(ch1 == VK_SUBTRACT)
				{
					
					int l_tag, layerbak;
					layerbak = m_nPCB_Current_Layer;

					CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
					CChildFrame* pChild = (CChildFrame*)pFrame->MDIGetActive();
					if(pChild != NULL)
					{
						CString cstring;
						for(l_tag=0; l_tag<pChild->m_wndSheet.m_strTitles.GetCount(); l_tag++)
						{
							if(pDoc->m_sPCB_LayerParam[m_nPCB_Current_Layer].csLayerName == pChild->m_wndSheet.m_strTitles.GetAt(l_tag)) 
							{
								for(int k=0; k<pChild->m_wndSheet.m_strTitles.GetCount(); k++)
								{
									l_tag--;
									if(l_tag < 0 ) l_tag = pChild->m_wndSheet.m_strTitles.GetCount() - 1;
									cstring = pChild->m_wndSheet.m_strTitles.GetAt(l_tag);
									
									for(int j=0; j<=MULTILAYER; j++)
									{ 
										if((pDoc->m_sPCB_LayerParam[j].csLayerName == cstring)&&(j >= TOPLAYER)&&(j <= BOTTOMLAYER))
										{
											m_nPCB_Current_Layer = j;
											break;
										}
									}
									if(m_nPCB_Current_Layer != layerbak) break;
								}
								break;
							}
						}
					}
					if(layerbak != m_nPCB_Current_Layer)
					{
						if(bEDA_PcbOnLineDRCEnable == TRUE) 
						{
							
							COb_pcbvia avia;
							avia.pDocument = pDoc;
							avia.m_nStartLayer = TOPLAYER;
							avia.m_nEndLayer = BOTTOMLAYER;
							avia.m_fDia = pDoc->m_fRulesViaDia;
							avia.m_fHoleDia = pDoc->m_fRulesViaHoleDia;
							avia.m_nNet = nRouteNet;
							avia.m_fCx = fx1;
							avia.m_fCy = fy1;
							if(FindViaOnLineDrcErrors(&avia) == FALSE)
							{
								Invalidate();
								CWnd* pWnd = GetParent();
								pWnd->Invalidate();
								
								
								color = nEDA_PCB_LayerColor[m_nPCB_Current_Layer];
								width = cEDA_pcbtrack.m_fWidth/m_fScale;
								if(width < 1) width = 1;
								delete OverwritePen1;
								OverwritePen1 = new CPen(PS_SOLID, width, color);
								delete OverwritePen2;
								OverwritePen2 = new CPen(PS_DOT, 1, color);				
							}
							else m_nPCB_Current_Layer = layerbak;							
						}
						else m_nPCB_Current_Layer = layerbak;
					}
				}
			}while(1);
		}
		else if( ch1 == MOUSEMOVE)
		{
			
			width = cEDA_pcbtrack.m_fWidth/m_fScale;
			if(width < 1) width = 1;
			delete OverwritePen1;
			OverwritePen1 = new CPen(PS_SOLID, width, color);
			delete OverwritePen2;
			OverwritePen2 = new CPen(PS_DOT, 1, color);
			
			if(nEDA_RedrawFlag == ON) nEDA_RedrawFlag = OFF;
			else  PcbElecHotPointsControl(&dc);
		}
		else if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
		{
			PcbElecHotPointsControl(&dc);
			break;
		}
		else if(ch1 ==  VK_ADD)
		{
			
			int l_tag, layerbak;
			layerbak = m_nPCB_Current_Layer;

			CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
			CChildFrame* pChild = (CChildFrame*)pFrame->MDIGetActive();
			if(pChild != NULL)
			{
				CString cstring;
				for(l_tag=0; l_tag<pChild->m_wndSheet.m_strTitles.GetCount(); l_tag++)
				{
					if(pDoc->m_sPCB_LayerParam[m_nPCB_Current_Layer].csLayerName == pChild->m_wndSheet.m_strTitles.GetAt(l_tag)) 
					{
						for(int k=0; k<pChild->m_wndSheet.m_strTitles.GetCount(); k++)
						{
							l_tag++;
							if(l_tag >= pChild->m_wndSheet.m_strTitles.GetCount()) l_tag = 0;
							cstring = pChild->m_wndSheet.m_strTitles.GetAt(l_tag);
									
							for(int j=0; j<=MULTILAYER; j++)
							{ 
								if((pDoc->m_sPCB_LayerParam[j].csLayerName == cstring)&&(j >= TOPLAYER)&&(j <= BOTTOMLAYER))
								{
									m_nPCB_Current_Layer = j;
									break;
								}
							}
							if(m_nPCB_Current_Layer != layerbak) break;
						}
						break;
					}
				}
			}
			if(layerbak != m_nPCB_Current_Layer)
			{
				Invalidate();
				CWnd* pWnd = GetParent();
				pWnd->Invalidate();
								
				
				color = nEDA_PCB_LayerColor[m_nPCB_Current_Layer];
				width = cEDA_pcbtrack.m_fWidth/m_fScale;
				if(width < 1) width = 1;
				delete OverwritePen1;
				OverwritePen1 = new CPen(PS_SOLID, width, color);
				delete OverwritePen2;
				OverwritePen2 = new CPen(PS_DOT, 1, color);				
			}
		}
		else if(ch1 == VK_SUBTRACT)
		{
			
			int l_tag, layerbak;
			layerbak = m_nPCB_Current_Layer;

			CMainFrame *pFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd; 
			CChildFrame* pChild = (CChildFrame*)pFrame->MDIGetActive();
			if(pChild != NULL)
			{
				CString cstring;
				for(l_tag=0; l_tag<pChild->m_wndSheet.m_strTitles.GetCount(); l_tag++)
				{
					if(pDoc->m_sPCB_LayerParam[m_nPCB_Current_Layer].csLayerName == pChild->m_wndSheet.m_strTitles.GetAt(l_tag)) 
					{
						for(int k=0; k<pChild->m_wndSheet.m_strTitles.GetCount(); k++)
						{
							l_tag--;
							if(l_tag < 0 ) l_tag = pChild->m_wndSheet.m_strTitles.GetCount() - 1;
							cstring = pChild->m_wndSheet.m_strTitles.GetAt(l_tag);
									
							for(int j=0; j<=MULTILAYER; j++)
							{ 
								if((pDoc->m_sPCB_LayerParam[j].csLayerName == cstring)&&(j >= TOPLAYER)&&(j <= BOTTOMLAYER))
								{
									m_nPCB_Current_Layer = j;
									break;
								}
							}							
							if(m_nPCB_Current_Layer != layerbak) break;
						}
						break;
					}
				}
			}
			if(layerbak != m_nPCB_Current_Layer)
			{
				Invalidate();
				CWnd* pWnd = GetParent();
				pWnd->Invalidate();
							
				
				color = nEDA_PCB_LayerColor[m_nPCB_Current_Layer];
				width = cEDA_pcbtrack.m_fWidth/m_fScale;
				if(width < 1) width = 1;
				delete OverwritePen1;
				OverwritePen1 = new CPen(PS_SOLID, width, color);
				delete OverwritePen2;
				OverwritePen2 = new CPen(PS_DOT, 1, color);				
			}
		}
		else Command_In_Function(ch1);
    }while(1);
	
	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	
	if(undo_flag == TRUE)
	{
		
		//if(bEDA_AutoOptimizeWire == TRUE) AutoOptimizeWire();

		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	//free GUI object
	delete OverwritePen1;
	delete OverwritePen2;

	Write_Help(szEDA_Help_Ready);

	
	bEDA_NeedElectricalGrid = FALSE;
	m_fpPcbHotPoint1.fx = m_fpPcbHotPoint1.fy = m_fpPcbHotPoint2.fx = m_fpPcbHotPoint2.fy = 0;
}

