void CESDView::OnPcbBoardShape()
{
	int ch1;
	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4, tempx, tempy;
	int x1,y1,x2,y2,x3,y3,x4,y4,flag;
	double fcx, fcy, fradius, fsx, fsy, fex, fey, fsangle, feangle;
	int cx, cy, sx, sy, ex, ey, radius;
	int index;
	Struct_RegionVertex vertex;
	char szPlaceRegion[]  = "定义板形区域: <左键><回车>定义顶点, <右键><ESC>结束命令";
	char szInfo[9][100] = {	"<shift+空格><空格>修改模式: 90度-先水平再垂直",	"<shift+空格><空格>修改模式: 90度-先垂直再水平",\
							"<shift+空格><空格>修改模式: 90度-先圆弧再连线",	"<shift+空格><空格>修改模式: 90度-先连线再圆弧",\
							"<shift+空格><空格>修改模式: 45度-先水平垂直再45度","<shift+空格><空格>修改模式: 45度-先45度再水平垂直",\
							"<shift+空格><空格>修改模式: 45度-先圆弧再连线",	"<shift+空格><空格>修改模式: 45度-先连线再圆弧",\
							"<shift+空格><空格>修改模式: 任意角度"};
	
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	Write_Help(szPlaceRegion);

	int layer = m_nPCB_Current_Layer;
	COLORREF color = (~nEDA_PCB_LayerColor[m_nPCB_Current_Layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
	//COLORREF color = nEDA_PCB_LayerColor[m_nPCB_Current_Layer];
	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	COb_pcbregion aregion;
	aregion.pDocument = pDoc;
	aregion.m_nLayer = BACKGROUND;
	//aregion.m_nHoleNum = 0;

	fradius = 0;
	
	flag = 1;
	nEDA_RedrawFlag = OFF;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			//ElecHotPointsControl(&dc);

			fx1 = m_fCurrentSnapX;
			fy1 = m_fCurrentSnapY;
			fx2 = fx3 = fx4 = fx1; 
			fy2 = fy3 = fy4 = fy1;

			vertex.fx = fx1;	vertex.fy = fy1;
			vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
			aregion.m_cVertex.Add(vertex);

			vertex.fx = fx2;	vertex.fy = fy2;
			vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
			aregion.m_cVertex.Add(vertex);

			vertex.fx = fx3;	vertex.fy = fy3;
			vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
			aregion.m_cVertex.Add(vertex);

			vertex.fx = fx4;	vertex.fy = fy4;
			vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
			aregion.m_cVertex.Add(vertex);

			Write_Info(szInfo[nEDA_PCB_PlaceTrackMode]);
			do
			{
				ch1 = MouseKey();
				//Get_VPort_XY(&x1,&y1,fx1,fy1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{ 
					//ElecHotPointsControl(&dc);
					Write_Info(szEDA_Info_Ready);

					CloseMouseCursor();
					aregion.FillRegion(&dc, this, color);
					OpenMouseCursor();
	
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
							//ElecHotPointsControl(&dc);

							CloseMouseCursor();
							aregion.FillRegion(&dc, this, color);
							OpenMouseCursor();
						}


						fx4 = fx3 = fx2 = m_fCurrentSnapX; 
						fy4 = fy3 = fy2 = m_fCurrentSnapY;
						fcx = fcy = fradius = fsangle = feangle = 0;

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
				
						index = aregion.m_cVertex.GetCount();

						
						
						vertex.fx = fx2;	vertex.fy = fy2;
						vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
						aregion.m_cVertex.SetAt(index-3, vertex);
						
						
						vertex.fx = fx2;	vertex.fy = fy2;
						vertex.cx = fcx;
						vertex.cy = fcy;
						vertex.radius = fradius;
						vertex.sangle = fsangle;
						vertex.eangle = feangle;
						aregion.m_cVertex.SetAt(index-2, vertex);

						
						vertex.fx = fx4;	vertex.fy = fy4;
						vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
						aregion.m_cVertex.SetAt(index-1, vertex);

						CloseMouseCursor();
						aregion.FillRegion(&dc, this, color);
						OpenMouseCursor();
					}
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					//ElecHotPointsControl(&dc);
					if((fx1==fx3)&&(fy1==fy3)) continue;

					CloseMouseCursor();
					aregion.FillRegion(&dc, this, color);
					OpenMouseCursor();

					
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

					if(fradius != 0)
					{
						index = aregion.m_cVertex.GetCount();
						if((fx1 != fx2)||(fy1 != fy2))
						{
							vertex.fx = fx2;	vertex.fy = fy2;
							vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
							aregion.m_cVertex.SetAt(index-3, vertex);
						
							vertex.fx = fx2;	vertex.fy = fy2;
							vertex.cx = fcx;
							vertex.cy = fcy;
							vertex.radius = fradius;
							vertex.sangle = fsangle;
							vertex.eangle = feangle;
							aregion.m_cVertex.SetAt(index - 2, vertex);
						
							vertex.fx = fx3;	vertex.fy = fy3;
							vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
							aregion.m_cVertex.SetAt(index - 1, vertex);							
							
							vertex.fx = fx4;	vertex.fy = fy4;
							vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
							aregion.m_cVertex.Add(vertex);
							aregion.m_cVertex.Add(vertex);					
							aregion.m_cVertex.Add(vertex);											
						}
						else
						{
							vertex.fx = fx2;	vertex.fy = fy2;
							vertex.cx = fcx;
							vertex.cy = fcy;
							vertex.radius = fradius;
							vertex.sangle = fsangle;
							vertex.eangle = feangle;
							aregion.m_cVertex.SetAt(index - 3, vertex);
							
							vertex.fx = fx3;	vertex.fy = fy3;
							vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
							aregion.m_cVertex.SetAt(index - 2, vertex);
							
							vertex.fx = fx4;	vertex.fy = fy4;
							vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
							aregion.m_cVertex.Add(vertex);
							aregion.m_cVertex.Add(vertex);
						}
					}
					else
					{
						if((fx1 != fx2)||(fy1 != fy2))
						{
							vertex.fx = fx2;	vertex.fy = fy2;
							vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
							index = aregion.m_cVertex.GetCount();
							aregion.m_cVertex.SetAt(index-3, vertex);
							
							vertex.fx = fx4;	vertex.fy = fy4;
							vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
							aregion.m_cVertex.Add(vertex);
						}
					}

					aregion.FillRegion(&dc, this, color);

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
				}
			}while(1);
		}
		else if( ch1 == MOUSEMOVE)
		{
			//if(nEDA_RedrawFlag == ON) nEDA_RedrawFlag = OFF;
			//else  ElecHotPointsControl(&dc);
		}
		else if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
		{
			//ElecHotPointsControl(&dc);
			break;
		}
		else Command_In_Function(ch1);
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	dc.SetROP2(drawmode);
	
	index = aregion.m_cVertex.GetCount();
	for(int i=index-1; i>1; i--)
	{
		Struct_RegionVertex vertex1, vertex2;
		vertex1 = aregion.m_cVertex.GetAt(i);
		vertex2 = aregion.m_cVertex.GetAt(i-1);
		if( (vertex1.cx == vertex2.cx)&&(vertex1.cy == vertex2.cy)&&(vertex1.eangle = vertex2.eangle)&&(vertex1.fx == vertex2.fx)&&\
			(vertex1.fy == vertex2.fy)&&(vertex1.radius == vertex2.radius)&&(vertex1.sangle == vertex2.sangle))
		{
			aregion.m_cVertex.RemoveAt(i);
		}
	}
	if(aregion.m_cVertex.GetCount() > 2)
	{
		pDoc->m_cPCB_Sheet.m_cVertex.RemoveAll();
		for(int i=0; i<aregion.m_cVertex.GetCount(); i++)
		{
			vertex = aregion.m_cVertex.GetAt(i);
			pDoc->m_cPCB_Sheet.m_cVertex.Add(vertex);
		}
	
		Invalidate();
	}

	//if(pregion != NULL) pregion->Draw(&dc, this);
	
	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	Write_Help(szEDA_Help_Ready);

	
	
}

void CESDView::OnPcbBoardVertex()
{
	int ch1;
	double offset = 20;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);
		
	char szHelp[] = "移动板形状顶点: <左键><回车>移动顶点, <右键><ESC>取消";

	
	PcbSelectAll(FALSE);

	
	COb_pcbregion* pregion = new COb_pcbregion();
	pregion->pDocument = pDoc;
	pregion->m_nLayer = MULTILAYER;
	
	for(int i=0; i<pDoc->m_cPCB_Sheet.m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex vertex;
		vertex = pDoc->m_cPCB_Sheet.m_cVertex.GetAt(i);
		pregion->m_cVertex.Add(vertex);
	}
	pregion->m_bSelection = TRUE;
	pregion->m_nFlag = FLAG_NORMAL;

	int index = pDoc->m_arrayPcbRegion.Add(pregion);
	Invalidate();

	Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			double fx, fy;
			fx = m_fCurrentX;
			fy = m_fCurrentY;

			for(int j=0; j<pregion->m_cVertex.GetCount(); j++)
			{
				Struct_RegionVertex vertex = pregion->m_cVertex.GetAt(j);

				double fx1, fy1, fx2, fy2;
				fx1 = vertex.fx - offset; //nEDA_PcbSelection_Offset;
				fy1 = vertex.fy - offset; //nEDA_PcbSelection_Offset;
				fx2 = vertex.fx + offset; //nEDA_PcbSelection_Offset;
				fy2 = vertex.fy + offset; //nEDA_PcbSelection_Offset;
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) 
				{
					
					
					pDoc->SetModifiedFlag(TRUE);
					
					
					if(m_bCursorState == TRUE)  CloseMouseCursor();
					pregion->m_nFlag = FLAG_DELETED;
					pregion->Draw(&dc, this);

					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	

					
					COb_pcbregion aregion;
					aregion.Copy(pregion);
					aregion.m_nFlag = FLAG_NORMAL;
					
					aregion.FrameRegion(&dc, this, 1, nEDA_PCB_LayerColor[aregion.m_nLayer]);
					OpenMouseCursor();

					nEDA_RedrawFlag = 0;
					do
					{
						ch1 =  MouseKey();
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							CloseMouseCursor();
							aregion.FrameRegion(&dc, this, 1, nEDA_PCB_LayerColor[aregion.m_nLayer]);
							OpenMouseCursor();
						}
						else if(ch1 == MOUSEMOVE)
						{
							
							if( m_bActive == FALSE) continue;

							// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
							if( m_bAfterSetFocus)
							{
								 m_bAfterSetFocus = false;
								continue;
							}
							
							
							if(nEDA_RedrawFlag == 1)
							{ 
								
								nEDA_RedrawFlag = 0;
							}
							else  
							{
								CloseMouseCursor();
								aregion.FrameRegion(&dc, this, 1, nEDA_PCB_LayerColor[aregion.m_nLayer]);
								OpenMouseCursor();
							}
							
							if(vertex.radius == 0)
							{
								vertex.fx = m_fCurrentSnapX;
								vertex.fy = m_fCurrentSnapY;
								aregion.m_cVertex.SetAt(j, vertex);
							}
							else
							{
								Struct_RegionVertex v_insert;
								v_insert.cx = v_insert.cy = v_insert.sangle = v_insert.eangle = v_insert.radius = 0;
								v_insert.fx = m_fCurrentSnapX;
								v_insert.fy = m_fCurrentSnapY;
								aregion.m_cVertex.InsertAt(j, v_insert);

								vertex = aregion.m_cVertex.GetAt(j);
							}


							CloseMouseCursor();
							aregion.FrameRegion(&dc, this, 1, nEDA_PCB_LayerColor[aregion.m_nLayer]);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)) //||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							aregion.FrameRegion(&dc, this, 1, nEDA_PCB_LayerColor[aregion.m_nLayer]);
							OpenMouseCursor();

							pregion->m_cVertex.RemoveAll();
							for(int k=0; k<aregion.m_cVertex.GetCount(); k++)
							{
								Struct_RegionVertex v_insert;
								v_insert = aregion.m_cVertex.GetAt(k);
								pregion->m_cVertex.Add(v_insert);
							}

							ch1 = MOUSEMOVE;
							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					pregion->m_nFlag = FLAG_NORMAL;
					pregion->Draw(&dc, this);
					pregion->DrawSelection(&dc, this);
					OpenMouseCursor();

					break;
				}
			}
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));


	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL); m_bClipState = FALSE;

	Write_Help(szEDA_Help_Ready);
	
	pDoc->m_cPCB_Sheet.m_cVertex.RemoveAll();
	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex vertex;
		vertex = pregion->m_cVertex.GetAt(i);
		pDoc->m_cPCB_Sheet.m_cVertex.Add(vertex);
	}

	pDoc->m_arrayPcbRegion.RemoveAt(index);
	delete pregion;
	Invalidate();
}

void CESDView::OnPcbBoardMove()
{
	int ch1;
	int flag;
	double x1,y1, x0, y0, deltax, deltay;
		
	char szHelp[] = "移动板形状: <空格>旋转, <左键><回车>放置, <右键><ESC>取消";

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	pDoc->SetModifiedFlag(TRUE);
	
	
	if(m_bCursorState == TRUE) CloseMouseCursor();

	x0 = pDoc->m_cPCB_Sheet.m_cVertex[0].fx;
	y0 = pDoc->m_cPCB_Sheet.m_cVertex[0].fy;
	
	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);	

	
	COb_pcbregion aregion;
	for(int i=0; i<pDoc->m_cPCB_Sheet.m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex vertex;
		vertex = pDoc->m_cPCB_Sheet.m_cVertex.GetAt(i);
		aregion.m_cVertex.Add(vertex);
	}
	
	aregion.m_nLayer = MULTILAYER;
	aregion.m_bSelection = FALSE;
	aregion.m_nFlag = TRUE;

	COLORREF color = RGB(0,0,0);
	aregion.FrameRegion(&dc, this, 1, color);
	OpenMouseCursor();

	flag = 0;
	Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			CloseMouseCursor();
			aregion.FrameRegion(&dc, this, 1, color);
			OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			
			if(m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(m_bAfterSetFocus)
			{
				m_bAfterSetFocus = false;
				continue;
			}
			
			
			if(nEDA_RedrawFlag == 1)
			{ 
				
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				CloseMouseCursor();
				aregion.FrameRegion(&dc, this, 1, color);
				OpenMouseCursor();
			}

			x1 = m_fCurrentSnapX;  
			y1 = m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aregion.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_cPCB_Sheet.m_cVertex[i].fx-x0, pDoc->m_cPCB_Sheet.m_cVertex[i].fy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].fx = fx + x0 + deltax;
				aregion.m_cVertex[i].fy = fy + y0 + deltay;
				
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_cPCB_Sheet.m_cVertex[i].cx-x0, pDoc->m_cPCB_Sheet.m_cVertex[i].cy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].cx = fx + x0 + deltax;
				aregion.m_cVertex[i].cy = fy + y0 + deltay;
			}
			
			CloseMouseCursor();
			aregion.FrameRegion(&dc, this, 1, color);
			OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			CloseMouseCursor();
			aregion.FrameRegion(&dc, this, 1, color);
			OpenMouseCursor();

			x1 = m_fCurrentSnapX;  
			y1 = m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aregion.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_cPCB_Sheet.m_cVertex[i].fx-x0, pDoc->m_cPCB_Sheet.m_cVertex[i].fy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].fx = fx + x0 + deltax;
				aregion.m_cVertex[i].fy = fy + y0 + deltay;
			}


			for(int i=0; i<aregion.m_cVertex.GetCount(); i++)
			{
				Struct_RegionVertex vertex;
				vertex = aregion.m_cVertex.GetAt(i);
				pDoc->m_cPCB_Sheet.m_cVertex.SetAt(i,vertex);
			}
			break;
		}
		else if(ch1 == VK_SPACE)
		{ 
			CloseMouseCursor();
			aregion.FrameRegion(&dc, this, 1, color);
			
			flag++;
			if(flag>3) flag=0;

			x1 = m_fCurrentSnapX;  
			y1 = m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			for(int i=0; i<aregion.m_cVertex.GetCount(); i++)
			{
				double fx, fy;
				
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_cPCB_Sheet.m_cVertex[i].fx-x0, pDoc->m_cPCB_Sheet.m_cVertex[i].fy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].fx = fx + x0 + deltax;
				aregion.m_cVertex[i].fy = fy + y0 + deltay;
				
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_cPCB_Sheet.m_cVertex[i].cx-x0, pDoc->m_cPCB_Sheet.m_cVertex[i].cy-y0, flag*90, FALSE);
				aregion.m_cVertex[i].cx = fx + x0 + deltax;
				aregion.m_cVertex[i].cy = fy + y0 + deltay;
				
				aregion.m_cVertex[i].sangle += 90;
				aregion.m_cVertex[i].sangle = pDoc->Get_Correct_Angle(aregion.m_cVertex[i].sangle);
				aregion.m_cVertex[i].eangle += 90;
				aregion.m_cVertex[i].eangle = pDoc->Get_Correct_Angle(aregion.m_cVertex[i].eangle);
			}
			aregion.FrameRegion(&dc, this, 1, color);

			OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	dc.SetROP2(drawmode);
	
	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL); m_bClipState = FALSE;

	Write_Help(szEDA_Help_Ready);

	Invalidate();
}

void CESDView::OnDesignRuleWizard()
{
	// TODO: 在此添加命令处理程序代码
}
