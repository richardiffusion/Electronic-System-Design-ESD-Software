void CESDView::OnPcbPlaceArcCircle()
{
	int ch1;
	double x1,y1,x2,y2;
	int xcenter,ycenter,xstart,ystart;
	int x3,y3,xend,yend;
	double alpha1,alpha2;
	int r;

	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	char szPlaceArcCenter[] = "放置圆弧: <左键><回车>定义圆心, <右键><ESC>结束命令, <TAB>定义属性";
	char szPlaceArcStart[] = "放置圆弧: <左键><回车>定义半径, <右键><ESC>返回上一步, <TAB>定义属性";
	char szInfo[] = "<TAB>属性";	

	RECT rect;
	POINT p1,p2;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	int layer = m_nPCB_Current_Layer;
	COLORREF color = (~nEDA_PCB_LayerColor[layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
	//COLORREF color = nEDA_PCB_LayerColor[layer];

	int width = cEDA_pcbarc.m_fWidth/m_fScale;
	if(width < 1) width = 1;
	CPen *arcPen, *oldpen;
	arcPen = new CPen(PS_SOLID, width, color);
	oldpen = dc.SelectObject(arcPen);
	
PLACEARC:
	Write_Help(szPlaceArcCenter);
	Write_Info(szInfo);

	bEDA_NeedElectricalGrid = TRUE;

	nEDA_RedrawFlag = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			PcbElecHotPointsControl(&dc);
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			PcbElecHotPointsControl(&dc);

			
			x1 = m_fCurrentSnapX;  y1 = m_fCurrentSnapY;
			Get_VPort_XY(&xcenter,&ycenter,x1,y1);
			x2 = x1; y2 = y1;
			xstart = xcenter; ystart = ycenter;
			r = (int)sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
			Write_Help(szPlaceArcStart);

			rect.left = (int)(xcenter - r/m_fScale);
			rect.top = (int)(ycenter - r/m_fScale);
			rect.right = (int)(xcenter + r/m_fScale);
			rect.bottom = (int)(ycenter + r/m_fScale);
			p1.x = p1.y = p2.x = p2.y = 0;

			dc.Arc(&rect,p1,p2);

			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xcenter,&ycenter,x1,y1); 
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
				{ 
					PcbElecHotPointsControl(&dc);

					rect.left = (int)(xcenter - r/m_fScale);
					rect.top = (int)(ycenter - r/m_fScale);
					rect.right = (int)(xcenter + r/m_fScale);
					rect.bottom = (int)(ycenter + r/m_fScale);
					p1.x = p1.y = p2.x = p2.y = 0;

					dc.Arc(&rect,p1,p2);
					ch1 = MOUSEMOVE;
					Write_Help(szPlaceArcCenter);
					break;
				}
				else if(ch1 == MOUSEMOVE)
				{
					if(nEDA_RedrawFlag == 1){ nEDA_RedrawFlag = 0;}
					else  
					{ 
						PcbElecHotPointsControl(&dc);

						rect.left = xcenter - r/m_fScale;
						rect.top = ycenter - r/m_fScale;
						rect.right = xcenter + r/m_fScale;
						rect.bottom = ycenter + r/m_fScale;
						p1.x = p1.y = p2.x = p2.y = 0;
						dc.Arc(&rect,p1,p2);
					}

					width = cEDA_pcbarc.m_fWidth/m_fScale;
					if(width < 1) width = 1;
					delete arcPen;
					arcPen = new CPen(PS_SOLID, width, color);
					dc.SelectObject(arcPen);

					
					x2 = m_fCurrentSnapX; y2 = m_fCurrentSnapY;
					Get_VPort_XY(&xstart,&ystart,x2,y2);
					r = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

					rect.left = xcenter - r/m_fScale;
					rect.top = ycenter - r/m_fScale;
					rect.right = xcenter + r/m_fScale;
					rect.bottom = ycenter + r/m_fScale;
					p1.x = p1.y = p2.x = p2.y = 0;

					dc.Arc(&rect,p1,p2);
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					PcbElecHotPointsControl(&dc);

					rect.left = xcenter - r/m_fScale;
					rect.top = ycenter - r/m_fScale;
					rect.right = xcenter + r/m_fScale;
					rect.bottom = ycenter + r/m_fScale;
					p1.x = p1.y = p2.x = p2.y = 0;

					dc.Arc(&rect,p1,p2);

					COb_pcbarc* parc = new COb_pcbarc();
					parc->pDocument = pDoc;
					parc->m_nLayer = layer;
					parc->m_fCx = x1;
					parc->m_fCy = y1;
					parc->m_fRadius = r;
					parc->m_fSangle = 0;
					parc->m_fEangle = 360;
					parc->m_fWidth = cEDA_pcbarc.m_fWidth;

					int index = pDoc->m_arrayPcbArc.Add(parc);

					dc.SetROP2(drawmode);
					CloseMouseCursor();
					parc->Draw(&dc, this);
					OpenMouseCursor();
					dc.SetROP2(R2_NOTXORPEN);

					
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
					

					goto PLACEARC;					}
				else if(ch1 == TAB)
				{
					CDlg_PCBEditArc dlg;
					
					for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
					{
						dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
					}
					for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
					{
						if(layer == pDoc->m_arrayPcbLayerShow.GetAt(i))
						{
							dlg.m_nComboLayer = i;
							break;
						}
					}
					dlg.m_csEditWidth = pDoc->Convert_XY_To_String(cEDA_pcbarc.m_fWidth);
					if(dlg.DoModal() == IDOK)
					{
						cEDA_pcbarc.m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditWidth);
						
						for(int i=0; i<=MULTILAYER; i++)
						{
							if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
							{
								layer = i;
								break;
							}
						}
						
						color = nEDA_PCB_LayerColor[layer];
						width = cEDA_pcbarc.m_fWidth/m_fScale;
						if(width < 1) width = 1;
						delete arcPen;
						arcPen = new CPen(PS_SOLID, width, color);
						dc.SelectObject(arcPen);
					}
				}
			}while(1);
		}
		else if(ch1 == MOUSEMOVE)
		{
			if(nEDA_RedrawFlag == 1) nEDA_RedrawFlag = 0;
			else  PcbElecHotPointsControl(&dc);
		}
		else if(ch1 == TAB)
		{
			CDlg_PCBEditArc dlg;
			
			for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
			{
				dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
			}
			for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
			{
				if(layer == pDoc->m_arrayPcbLayerShow.GetAt(i))
				{
					dlg.m_nComboLayer = i;
					break;
				}
			}
			dlg.m_csEditWidth = pDoc->Convert_XY_To_String(cEDA_pcbarc.m_fWidth);
			if(dlg.DoModal() == IDOK)
			{
				cEDA_pcbarc.m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditWidth);
				
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
					{
						layer = i;
						break;
					}
				}
				
				color = nEDA_PCB_LayerColor[layer];
				width = cEDA_pcbarc.m_fWidth/m_fScale;
				if(width < 1) width = 1;
				delete arcPen;
				arcPen = new CPen(PS_SOLID, width, color);
				dc.SelectObject(arcPen);
			}
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);

	delete arcPen;

	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);

	
	bEDA_NeedElectricalGrid = FALSE;
	m_fpPcbHotPoint1.fx = m_fpPcbHotPoint1.fy = m_fpPcbHotPoint2.fx = m_fpPcbHotPoint2.fy = 0;
}

void CESDView::OnPcbPlaceArcCenter()
{
	int ch1;
	double x1,y1,x2,y2;
	int xcenter,ycenter,xstart,ystart;
	int x3,y3,xend,yend;
	double alpha1,alpha2;
	int r;

	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	char szPlaceArcCenter[] = "放置圆弧: <左键><回车>定义圆心, <右键><ESC>结束命令, <TAB>定义属性";
	char szPlaceArcStart[] = "放置圆弧: <左键><回车>定义半径和起始角度, <右键><ESC>返回上一步, <TAB>定义属性";
	char szPlaceArcEnd[] = "放置圆弧: <左键><回车>定义终止角度, <右键><ESC>返回第一步, <TAB>定义属性";
	char szInfo[] = "<TAB>属性";	

	RECT rect;
	POINT p1,p2;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	int layer = m_nPCB_Current_Layer;
	COLORREF color = (~nEDA_PCB_LayerColor[layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
	//COLORREF color = nEDA_PCB_LayerColor[layer];

	int width = cEDA_pcbarc.m_fWidth/m_fScale;
	if(width < 1) width = 1;

	CPen *arcPen, *oldpen;
	arcPen = new CPen(PS_SOLID, width, color);
	oldpen = dc.SelectObject(arcPen);
	
PLACEARC:
	Write_Help(szPlaceArcCenter);
	Write_Info(szInfo);
	
	
	bEDA_NeedElectricalGrid = TRUE;

	nEDA_RedrawFlag = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			PcbElecHotPointsControl(&dc);
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			PcbElecHotPointsControl(&dc);

			
			x1 = m_fCurrentSnapX;  y1 = m_fCurrentSnapY;
			Get_VPort_XY(&xcenter,&ycenter,x1,y1);
			x2 = x1; y2 = y1;
			xstart = xcenter; ystart = ycenter;
			r = (int)sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
			Write_Help(szPlaceArcStart);

			rect.left = (int)(xcenter - r/m_fScale);
			rect.top = (int)(ycenter - r/m_fScale);
			rect.right = (int)(xcenter + r/m_fScale);
			rect.bottom = (int)(ycenter + r/m_fScale);
			p1.x = (int)(xcenter - r/m_fScale); 
			p1.y = ycenter;
			p2.x = (int)(xcenter - r/m_fScale); 
			p2.y = ycenter;
			dc.Arc(&rect,p1,p2);

			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xcenter,&ycenter,x1,y1); 
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
				{ 
					PcbElecHotPointsControl(&dc);

					rect.left = (int)(xcenter - r/m_fScale);
					rect.top = (int)(ycenter - r/m_fScale);
					rect.right = (int)(xcenter + r/m_fScale);
					rect.bottom = (int)(ycenter + r/m_fScale);
					p1.x = (int)(xcenter - r/m_fScale); 
					p1.y = ycenter;
					p2.x = (int)(xcenter - r/m_fScale);
					p2.y = ycenter;
					dc.Arc(&rect,p1,p2);
					ch1 = MOUSEMOVE;
					Write_Help(szPlaceArcCenter);
					break;
				}
				else if(ch1 == MOUSEMOVE)
				{
					if(nEDA_RedrawFlag == 1){ nEDA_RedrawFlag = 0;}
					else  
					{ 
						PcbElecHotPointsControl(&dc);

						rect.left = xcenter - r/m_fScale;
						rect.top = ycenter - r/m_fScale;
						rect.right = xcenter + r/m_fScale;
						rect.bottom = ycenter + r/m_fScale;
						p1.x = xcenter - r/m_fScale; p1.y = ycenter;
						p2.x = xcenter - r/m_fScale; p2.y = ycenter;
						dc.Arc(&rect,p1,p2);
					}

					width = cEDA_pcbarc.m_fWidth/m_fScale;
					if(width < 1) width = 1;
					delete arcPen;
					arcPen = new CPen(PS_SOLID, width, color);
					dc.SelectObject(arcPen);

					
					x2 = m_fCurrentSnapX; y2 = m_fCurrentSnapY;
					Get_VPort_XY(&xstart,&ystart,x2,y2);
					r = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

					rect.left = xcenter - r/m_fScale;
					rect.top = ycenter - r/m_fScale;
					rect.right = xcenter + r/m_fScale;
					rect.bottom = ycenter + r/m_fScale;
					p1.x = xcenter - r/m_fScale; p1.y = ycenter;
					p2.x = xcenter - r/m_fScale; p2.y = ycenter;
					dc.Arc(&rect,p1,p2);
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					PcbElecHotPointsControl(&dc);

					if((int)r == 0) continue;

					rect.left = xcenter - r/m_fScale;
					rect.top = ycenter - r/m_fScale;
					rect.right = xcenter + r/m_fScale;
					rect.bottom = ycenter + r/m_fScale;
					p1.x = xcenter - r/m_fScale; p1.y = ycenter;
					p2.x = xcenter - r/m_fScale; p2.y = ycenter;
					dc.Arc(&rect,p1,p2);

					
					if(((x2-x1)==0)&&(y2-y1)<0) alpha1 = 270;
					else if(((x2-x1)==0)&&(y2-y1)>=0) alpha1 = 90;
					else if(((y2-y1)==0)&&(x2-x1)<0) alpha1 = 180;
					else if(((y2-y1)==0)&&(x2-x1)>=0) alpha1 = 0;
					else alpha1 = (180.0/fEDA_pi)*atan2((y2-y1),(x2-x1));

					alpha1 = pDoc->Get_Correct_Angle(alpha1);

					alpha2 = alpha1;

					x3 = x2; y3 = y2;
					xend = xstart; yend = ystart;
					Write_Help(szPlaceArcEnd);

					rect.left = xcenter - r/m_fScale;
					rect.top = ycenter - r/m_fScale;
					rect.right = xcenter + r/m_fScale;
					rect.bottom = ycenter + r/m_fScale;
					p1.x = xstart;	p1.y = ystart;
					p2.x = xend;	p2.y = yend;
					dc.Arc(&rect,p1,p2);

					do
					{
						ch1 = MouseKey();
						
						Get_VPort_XY(&xcenter,&ycenter,x1,y1);
						Get_VPort_XY(&xstart,&ystart,x2,y2);
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
						{
							PcbElecHotPointsControl(&dc);

							rect.left = xcenter - r/m_fScale;
							rect.top = ycenter - r/m_fScale;
							rect.right = xcenter + r/m_fScale;
							rect.bottom = ycenter + r/m_fScale;
							p1.x = xstart;	p1.y = ystart;
							p2.x = xend;	p2.y = yend;
							dc.Arc(&rect,p1,p2);

							goto PLACEARC;
						}
						else if(ch1==MOUSEMOVE)
						{
							if(nEDA_RedrawFlag==1){ nEDA_RedrawFlag=0;}
							else  
							{ 
								PcbElecHotPointsControl(&dc);

								rect.left = xcenter - r/m_fScale;
								rect.top = ycenter - r/m_fScale;
								rect.right = xcenter + r/m_fScale;
								rect.bottom = ycenter + r/m_fScale;
								p1.x = xstart;	p1.y = ystart;
								p2.x = xend;	p2.y = yend;
								dc.Arc(&rect,p1,p2);
							}
							
							width = cEDA_pcbarc.m_fWidth/m_fScale;
							if(width < 1) width = 1;
							delete arcPen;
							arcPen = new CPen(PS_SOLID, width, color);
							dc.SelectObject(arcPen);

							
							x3 = m_fCurrentSnapX; y3 = m_fCurrentSnapY;
							Get_VPort_XY(&xend,&yend,x3,y3);

							rect.left = xcenter - r/m_fScale;
							rect.top = ycenter - r/m_fScale;
							rect.right = xcenter + r/m_fScale;
							rect.bottom = ycenter + r/m_fScale;
							p1.x = xstart;	p1.y = ystart;
							p2.x = xend;	p2.y = yend;
							dc.Arc(&rect,p1,p2);								
						}
						else if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
						{
							PcbElecHotPointsControl(&dc);

							rect.left = xcenter - r/m_fScale;
							rect.top = ycenter - r/m_fScale;
							rect.right = xcenter + r/m_fScale;
							rect.bottom = ycenter + r/m_fScale;
							p1.x = xstart;	p1.y = ystart;
							p2.x = xend;	p2.y = yend;
							dc.Arc(&rect,p1,p2);

							
							if(((x3-x1)==0)&&(y3-y1)<0) alpha2 = 270;
							else if(((x3-x1)==0)&&(y3-y1)>=0) alpha2 = 90;
							else if(((y3-y1)==0)&&(x3-x1)<0) alpha2 = 180;
							else if(((y3-y1)==0)&&(x3-x1)>=0) alpha2 = 0;
							else alpha2 = (180.0/fEDA_pi)*atan2((y3-y1),(x3-x1));

							alpha2 = pDoc->Get_Correct_Angle(alpha2);

							COb_pcbarc* parc = new COb_pcbarc();
							parc->pDocument = pDoc;
							parc->m_nLayer = layer;
							parc->m_fCx = x1;
							parc->m_fCy = y1;
							parc->m_fRadius = r;
							parc->m_fSangle = alpha1;
							parc->m_fEangle = alpha2;
							parc->m_fWidth = cEDA_pcbarc.m_fWidth;

							int index = pDoc->m_arrayPcbArc.Add(parc);

							dc.SetROP2(drawmode);
							CloseMouseCursor();
							parc->Draw(&dc, this);
							OpenMouseCursor();
							dc.SetROP2(R2_NOTXORPEN);
							
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
							

							goto PLACEARC;	
						}
						else if(ch1 == TAB)
						{
							CDlg_PCBEditArc dlg;
							
							for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
							{
								dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
							}
							for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
							{
								if(layer == pDoc->m_arrayPcbLayerShow.GetAt(i))
								{
									dlg.m_nComboLayer = i;
									break;
								}
							}
							dlg.m_csEditWidth = pDoc->Convert_XY_To_String(cEDA_pcbarc.m_fWidth);
							if(dlg.DoModal() == IDOK)
							{
								cEDA_pcbarc.m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditWidth);
								
								for(int i=0; i<=MULTILAYER; i++)
								{
									if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
									{
										layer = i;
										break;
									}
								}
								
								color = nEDA_PCB_LayerColor[layer];
								width = cEDA_pcbarc.m_fWidth/m_fScale;
								if(width < 1) width = 1;
								delete arcPen;
								arcPen = new CPen(PS_SOLID, width, color);
								dc.SelectObject(arcPen);
							}
						}
					}while(1);
				}
				else if(ch1 == TAB)
				{
					CDlg_PCBEditArc dlg;
					
					for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
					{
						dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
					}
					for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
					{
						if(layer == pDoc->m_arrayPcbLayerShow.GetAt(i))
						{
							dlg.m_nComboLayer = i;
							break;
						}
					}
					dlg.m_csEditWidth = pDoc->Convert_XY_To_String(cEDA_pcbarc.m_fWidth);
					if(dlg.DoModal() == IDOK)
					{
						cEDA_pcbarc.m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditWidth);
						
						for(int i=0; i<=MULTILAYER; i++)
						{
							if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
							{
								layer = i;
								break;
							}
						}
						//color = (~nEDA_PCB_LayerColor[layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
						color = nEDA_PCB_LayerColor[layer];
						width = cEDA_pcbarc.m_fWidth/m_fScale;
						if(width < 1) width = 1;
						delete arcPen;
						arcPen = new CPen(PS_SOLID, width, color);
						dc.SelectObject(arcPen);
					}
				}
			}while(1);
		}
		else if(ch1 == MOUSEMOVE)
		{
			if(nEDA_RedrawFlag == 1) nEDA_RedrawFlag = 0;
			else  PcbElecHotPointsControl(&dc);
		}
		else if(ch1 == TAB)
		{
			CDlg_PCBEditArc dlg;
			
			for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
			{
				dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
			}
			for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
			{
				if(layer == pDoc->m_arrayPcbLayerShow.GetAt(i))
				{
					dlg.m_nComboLayer = i;
					break;
				}
			}
			dlg.m_csEditWidth = pDoc->Convert_XY_To_String(cEDA_pcbarc.m_fWidth);
			if(dlg.DoModal() == IDOK)
			{
				cEDA_pcbarc.m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditWidth);
				
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
					{
						layer = i;
						break;
					}
				}
				//color = (~nEDA_PCB_LayerColor[layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
				color = nEDA_PCB_LayerColor[layer];
				width = cEDA_pcbarc.m_fWidth/m_fScale;
				if(width < 1) width = 1;
				delete arcPen;
				arcPen = new CPen(PS_SOLID, width, color);
				dc.SelectObject(arcPen);
			}
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);

	delete arcPen;

	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);
	
	
	bEDA_NeedElectricalGrid = FALSE;
	m_fpPcbHotPoint1.fx = m_fpPcbHotPoint1.fy = m_fpPcbHotPoint2.fx = m_fpPcbHotPoint2.fy = 0;
}

void CESDView::OnPcbPlaceArcEdge()
{
	int ch1;
	double x1, y1, x2, y2, x3, y3, cx, cy, r;
	int xend, yend, xstart, ystart, xcenter, ycenter;
	double alpha2,alpha1;
	
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	char szPlaceArcStart[] = "放置圆弧: <左键><回车>定义起点, <右键><ESC>结束命令, <TAB>定义属性";
	char szPlaceArcEnd[] = "放置圆弧: <左键><回车>定义终点, <右键><ESC>返回上一步, <TAB>定义属性";
	char szPlaceArc[] = "放置圆弧: <左键><回车>定义圆弧形状, <右键><ESC>返回第一步, <TAB>定义属性";
	char szInfo[] = "<TAB>属性";

	RECT rect;
	POINT p1,p2;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);
	
	int layer = m_nPCB_Current_Layer;
	COLORREF color = (~nEDA_PCB_LayerColor[layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
	//COLORREF color = nEDA_PCB_LayerColor[layer];

	int width = cEDA_pcbarc.m_fWidth/m_fScale;
	if(width < 1) width = 1;
	CPen *arcPen, *oldpen;
	arcPen = new CPen(PS_SOLID, width, color);
	oldpen = dc.SelectObject(arcPen);

PLACEARC:
	Write_Help(szPlaceArcStart);  
	Write_Info(szInfo);
	
	
	bEDA_NeedElectricalGrid = TRUE;

	nEDA_RedrawFlag = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			PcbElecHotPointsControl(&dc);
		}
		else if(ch1 == MOUSEMOVE)
		{
			if(nEDA_RedrawFlag == 1) nEDA_RedrawFlag = 0;
			else  PcbElecHotPointsControl(&dc);
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			PcbElecHotPointsControl(&dc);

			x2 = m_fCurrentSnapX;  y2 = m_fCurrentSnapY;
			Get_VPort_XY(&xend,&yend,x2,y2);
			
			x1 = x2; y1 = y2;
			xstart = xend; ystart = yend;
			
			r = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))/2;
			cx = (x2 + x1)/2;
			cy = (y2 + y1)/2;
			Write_Help("定义圆弧的另一个端点");  

			rect.left = (xend+xstart)/2 - r/m_fScale;
			rect.top = (yend+ystart)/2 - r/m_fScale;
			rect.right = (xend+xstart)/2 + r/m_fScale;
			rect.bottom = (yend+ystart)/2 + r/m_fScale;
			p1.x = xstart; p1.y = ystart;
			p2.x = xend; p2.y = yend;	
			dc.Arc(&rect,p1,p2);

			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xend,&yend,x2,y2);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
				{ 
					PcbElecHotPointsControl(&dc);

					Write_Help("定义圆弧的一个端点");
					rect.left = (xend+xstart)/2 - r/m_fScale;
					rect.top = (yend+ystart)/2 - r/m_fScale;
					rect.right = (xend+xstart)/2 + r/m_fScale;
					rect.bottom = (yend+ystart)/2 + r/m_fScale;
					p1.x = xstart; p1.y = ystart;
					p2.x = xend; p2.y = yend;	
					dc.Arc(&rect,p1,p2);
					ch1 = MOUSEMOVE;
					break;
				}
				else if(ch1 == MOUSEMOVE)
				{
					if(nEDA_RedrawFlag == 1){ nEDA_RedrawFlag = 0;}
					else  
					{ 
						PcbElecHotPointsControl(&dc);

						rect.left = (xend+xstart)/2 - r/m_fScale;
						rect.top = (yend+ystart)/2 - r/m_fScale;
						rect.right = (xend+xstart)/2 + r/m_fScale;
						rect.bottom = (yend+ystart)/2 + r/m_fScale;
						p1.x = xstart; p1.y = ystart;
						p2.x = xend; p2.y = yend;
						dc.Arc(&rect,p1,p2);
					}

					width = cEDA_pcbarc.m_fWidth/m_fScale;
					if(width < 1) width = 1;
					delete arcPen;
					arcPen = new CPen(PS_SOLID, width, color);
					dc.SelectObject(arcPen);

					x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
					Get_VPort_XY(&xstart,&ystart,x1,y1);
					r = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))/2;
					cx = (x2 + x1)/2;
					cy = (y2 + y1)/2;
					
					rect.left = (xend+xstart)/2 - r/m_fScale;
					rect.top = (yend+ystart)/2 - r/m_fScale;
					rect.right = (xend+xstart)/2 + r/m_fScale;
					rect.bottom = (yend+ystart)/2 + r/m_fScale;
					p1.x = xstart; p1.y = ystart;
					p2.x = xend; p2.y = yend;
					dc.Arc(&rect,p1,p2);					
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					PcbElecHotPointsControl(&dc);

					if( (x1 == x2)&&(y1 == y2)) continue;

					rect.left = (xend+xstart)/2 - r/m_fScale;
					rect.top = (yend+ystart)/2 - r/m_fScale;
					rect.right = (xend+xstart)/2 + r/m_fScale;
					rect.bottom = (yend+ystart)/2 + r/m_fScale;
					p1.x = xstart; p1.y = ystart;
					p2.x = xend; p2.y = yend;
					dc.Arc(&rect,p1,p2);
						
	
					x3 = x1; y3 = y1;
					if( x1 == x2) 
					{
						cx = x3;
						cy = (y2+y1)/2;
					}
					else if(y1 == y2) 
					{
						cx = (x2+x1)/2;
						cy = y3;
					}
					else 
					{
						double k1,k2, k11, k22;
						k1 = (y1-y2)/(x1-x2);		
						k2 = -1/k1;								
						int xz,yz;								
						xz = (x2+x1)/2;
						yz = (y2+y1)/2;
						
						
						k11 = 1/k1;
						k22 = 1/k2;	
						cx = (yz - y3 +  k1*x3 -  k2*xz)/(k1-k2);
						cy = (xz - x3 + k11*y3 - k22*yz)/(k11-k22);
					}
					r = sqrt((cx-x2)*(cx-x2)+(cy-y2)*(cy-y2));
					Get_VPort_XY(&xcenter,&ycenter,cx,cy);					

					Write_Help("定义半径");

					rect.left = xcenter - r/m_fScale;
					rect.top = ycenter - r/m_fScale;
					rect.right = xcenter + r/m_fScale;
					rect.bottom = ycenter + r/m_fScale;
					p1.x = xstart; p1.y = ystart;
					p2.x = xend; p2.y = yend;
					dc.Arc(&rect, p1, p2);

					do
					{
						ch1 = MouseKey();
						Get_VPort_XY(&xend,&yend,x2,y2);
						Get_VPort_XY(&xstart,&ystart,x1,y1);
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
						{
							PcbElecHotPointsControl(&dc);

							rect.left = xcenter - r/m_fScale;
							rect.top = ycenter - r/m_fScale;
							rect.right = xcenter + r/m_fScale;
							rect.bottom = ycenter + r/m_fScale;
							p1.x = xstart; p1.y = ystart;
							p2.x = xend; p2.y = yend;
							dc.Arc(&rect, p1, p2);
							goto PLACEARC;
						}
						else if(ch1==MOUSEMOVE)
						{
							
							if(nEDA_RedrawFlag==1){ nEDA_RedrawFlag=0;}
							else  
							{ 
								PcbElecHotPointsControl(&dc);

								rect.left = xcenter - r/m_fScale;
								rect.top = ycenter - r/m_fScale;
								rect.right = xcenter + r/m_fScale;
								rect.bottom = ycenter + r/m_fScale;
								p1.x = xstart; p1.y = ystart;
								p2.x = xend; p2.y = yend;
								dc.Arc(&rect, p1, p2);
							}

							width = cEDA_pcbarc.m_fWidth/m_fScale;
							if(width < 1) width = 1;
							delete arcPen;
							arcPen = new CPen(PS_SOLID, width, color);
							dc.SelectObject(arcPen);

							x3 = m_fCurrentSnapX; y3 = m_fCurrentSnapY;

							if( x1 == x2)
							{
								cx = x3;
								cy = (y2+y1)/2;
							}
							else if(y1 == y2)
							{
								cx = (x2+x1)/2;
								cy = y3;
							}
							else
							{
								double k1,k2, k11, k22;
								k1 = (y1-y2)/(x1-x2);		
								k2 = -1/k1;								
								int xz,yz;								
								xz = (x2+x1)/2;
								yz = (y2+y1)/2;
								
								k11 = 1/k1;
								k22 = 1/k2;	
								cx = (yz - y3 +  k1*x3 -  k2*xz)/(k1-k2);
								cy = (xz - x3 + k11*y3 - k22*yz)/(k11-k22);
							}
							r = sqrt((cx-x2)*(cx-x2) + (cy-y2)*(cy-y2));
							Get_VPort_XY(&xcenter,&ycenter,cx,cy);

							rect.left = xcenter - r/m_fScale;
							rect.top = ycenter - r/m_fScale;
							rect.right = xcenter + r/m_fScale;
							rect.bottom = ycenter + r/m_fScale;
							p1.x = xstart; p1.y = ystart;
							p2.x = xend; p2.y = yend;
							dc.Arc(&rect, p1, p2);
						}
						else if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
						{
							PcbElecHotPointsControl(&dc);

							rect.left = xcenter - r/m_fScale;
							rect.top = ycenter - r/m_fScale;
							rect.right = xcenter + r/m_fScale;
							rect.bottom = ycenter + r/m_fScale;
							p1.x = xstart; p1.y = ystart;
							p2.x = xend; p2.y = yend;
							dc.Arc(&rect, p1, p2);

							
							if(((cx-x2)==0)&&(cy-y2)<0) alpha1 = 270;
							else if(((cx-x2)==0)&&(cy-y2)>=0) alpha1 = 90;
							else if(((cy-y2)==0)&&(cx-x2)<0) alpha1 = 180;
							else if(((cy-y2)==0)&&(cx-x2)>=0) alpha1 = 0;
							
							else alpha1 = (180/fEDA_pi)*atan2((y2-cy),(x2-cx));
							alpha1 = pDoc->Get_Correct_Angle(alpha1);
							
							
							if(((cx-x1)==0)&&(cy-y1)<0) alpha2 = 270;
							else if(((cx-x1)==0)&&(cy-y1)>=0) alpha2 = 90;
							else if(((cy-y1)==0)&&(cx-x1)<0) alpha2 = 180;
							else if(((cy-y1)==0)&&(cx-x1)>=0) alpha2 = 0;
							else alpha2 = (180/fEDA_pi)*atan2((y1-cy),(x1-cx));
							alpha2 = pDoc->Get_Correct_Angle(alpha2);

							COb_pcbarc* parc = new COb_pcbarc;
							parc->pDocument = pDoc;
							parc->m_nLayer = layer;
							parc->m_fCx = cx;
							parc->m_fCy = cy;
							parc->m_fRadius = r;
							parc->m_fSangle = alpha2;
							parc->m_fEangle = alpha1;
							parc->m_fWidth = cEDA_pcbarc.m_fWidth;

							int index = pDoc->m_arrayPcbArc.Add(parc);

							dc.SetROP2(drawmode);
							CloseMouseCursor();
							parc->Draw(&dc, this);
							OpenMouseCursor();
							dc.SetROP2(R2_NOTXORPEN);
							
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

							goto PLACEARC;	
						}
						else if(ch1 == TAB)
						{
							CDlg_PCBEditArc dlg;

							for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
							{
								dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
							}
							for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
							{
								if(layer == pDoc->m_arrayPcbLayerShow.GetAt(i))
								{
									dlg.m_nComboLayer = i;
									break;
								}
							}
							dlg.m_csEditWidth = pDoc->Convert_XY_To_String(cEDA_pcbarc.m_fWidth);
							if(dlg.DoModal() == IDOK)
							{
								cEDA_pcbarc.m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditWidth);
								
								for(int i=0; i<=MULTILAYER; i++)
								{
									if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
									{
										layer = i;
										break;
									}
								}
								//color = (~nEDA_PCB_LayerColor[layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
								color = nEDA_PCB_LayerColor[layer];
								width = cEDA_pcbarc.m_fWidth/m_fScale;
								if(width < 1) width = 1;
								delete arcPen;
								arcPen = new CPen(PS_SOLID, width, color);
								dc.SelectObject(arcPen);
							}
						}
					}while(1);
				}
				else if(ch1 == TAB)
				{
					CDlg_PCBEditArc dlg;
					
					for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
					{
						dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
					}
					for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
					{
						if(layer == pDoc->m_arrayPcbLayerShow.GetAt(i))
						{
							dlg.m_nComboLayer = i;
							break;
						}
					}
					dlg.m_csEditWidth = pDoc->Convert_XY_To_String(cEDA_pcbarc.m_fWidth);
					if(dlg.DoModal() == IDOK)
					{
						cEDA_pcbarc.m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditWidth);
						
						for(int i=0; i<=MULTILAYER; i++)
						{
							if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
							{
								layer = i;
								break;
							}
						}
						//color = (~nEDA_PCB_LayerColor[layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
						color = nEDA_PCB_LayerColor[layer];
						width = cEDA_pcbarc.m_fWidth/m_fScale;
						if(width < 1) width = 1;
						delete arcPen;
						arcPen = new CPen(PS_SOLID, width, color);
						dc.SelectObject(arcPen);
					}
				}
			}while(1);
		}
		else if(ch1 == TAB)
		{
			CDlg_PCBEditArc dlg;
			
			for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
			{
				dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
			}
			for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
			{
				if(layer == pDoc->m_arrayPcbLayerShow.GetAt(i))
				{
					dlg.m_nComboLayer = i;
					break;
				}
			}
			dlg.m_csEditWidth = pDoc->Convert_XY_To_String(cEDA_pcbarc.m_fWidth);
			if(dlg.DoModal() == IDOK)
			{
				cEDA_pcbarc.m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditWidth);
				
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
					{
						layer = i;
						break;
					}
				}
				//color = (~nEDA_PCB_LayerColor[layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
				color = nEDA_PCB_LayerColor[layer];
				width = cEDA_pcbarc.m_fWidth/m_fScale;
				if(width < 1) width = 1;
				delete arcPen;
				arcPen = new CPen(PS_SOLID, width, color);
				dc.SelectObject(arcPen);
			}
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	

	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);

	delete arcPen;	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);

	
	bEDA_NeedElectricalGrid = FALSE;
	m_fpPcbHotPoint1.fx = m_fpPcbHotPoint1.fy = m_fpPcbHotPoint2.fx = m_fpPcbHotPoint2.fy = 0;
}

void CESDView::DrawDottedTrack(int x1, int y1, int x2, int y2, int width, CDC* pDC)
{
	POINT point[6];
	if((x1 == x2)&&(y1 == y2)) return;
	else if(x1 == x2)
	{
		if(y1 < y2)
		{
			point[0].x = x1;			point[0].y = y1;
			point[1].x = x1 - width;	point[1].y = y1 + width;
			point[2].x = x2 - width;	point[2].y = y2 - width;
			point[3].x = x2;			point[3].y = y2;
			point[4].x = x2 + width;	point[4].y = y2 - width;
			point[5].x = x1 + width;	point[5].y = y1 + width;
		}
		else
		{
			point[0].x = x1;			point[0].y = y1;
			point[1].x = x1 - width;	point[1].y = y1 - width;
			point[2].x = x2 - width;	point[2].y = y2 + width;
			point[3].x = x2;			point[3].y = y2;
			point[4].x = x2 + width;	point[4].y = y2 + width;
			point[5].x = x1 + width;	point[5].y = y1 - width;
		}
	}
	else if(y1 == y2)
	{
		if(x1 < x2)
		{
			point[0].x = x1;			point[0].y = y1;
			point[1].x = x1 + width;	point[1].y = y1 + width;
			point[2].x = x2 - width;	point[2].y = y2 + width;
			point[3].x = x2;			point[3].y = y2;
			point[4].x = x2 - width;	point[4].y = y2 - width;
			point[5].x = x1 + width;	point[5].y = y1 - width;
		}
		else
		{
			point[0].x = x1;			point[0].y = y1;
			point[1].x = x1 - width;	point[1].y = y1 - width;
			point[2].x = x2 + width;	point[2].y = y2 - width;
			point[3].x = x2;			point[3].y = y2;
			point[4].x = x2 + width;	point[4].y = y2 + width;
			point[5].x = x1 - width;	point[5].y = y1 + width;
		}
	}
	else
	{
		int delta = (int)sqrt((double)(2*width*width));
		if(x1 < x2)
		{
			if(y1 > y2)
			{
				point[0].x = x1;			point[0].y = y1;
				point[1].x = x1;			point[1].y = y1 - delta;
				point[2].x = x2 - delta;	point[2].y = y2;
				point[3].x = x2;			point[3].y = y2;
				point[4].x = x2;			point[4].y = y2 + delta;
				point[5].x = x1 + delta;	point[5].y = y1;
			}
			else
			{
				point[0].x = x1;			point[0].y = y1;
				point[1].x = x1;			point[1].y = y1 + delta;
				point[2].x = x2 - delta;	point[2].y = y2;
				point[3].x = x2;			point[3].y = y2;
				point[4].x = x2;			point[4].y = y2 - delta;
				point[5].x = x1 + delta;	point[5].y = y1;
			}
		}
		else
		{
			if(y1 > y2)
			{
				point[0].x = x1;			point[0].y = y1;
				point[1].x = x1;			point[1].y = y1 - delta;
				point[2].x = x2 + delta;	point[2].y = y2;
				point[3].x = x2;			point[3].y = y2;
				point[4].x = x2;			point[4].y = y2 + delta;
				point[5].x = x1 - delta;	point[5].y = y1;
			}
			else
			{
				point[0].x = x1;			point[0].y = y1;
				point[1].x = x1;			point[1].y = y1 + delta;
				point[2].x = x2 + delta;	point[2].y = y2;
				point[3].x = x2;			point[3].y = y2;
				point[4].x = x2;			point[4].y = y2 - delta;
				point[5].x = x1 - delta;	point[5].y = y1;
			}
		}
	}

	pDC->MoveTo(point[5].x, point[5].y);
	for(int i=0; i<6; i++)
	{
		pDC->LineTo(point[i].x, point[i].y);
	}
}

void CESDView::OnPcbPlaceTrack()
{
	int ch1;
	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4, tempx, tempy;
	int x1,y1,x2,y2,x3,y3,x4,y4,flag;
	double fcx, fcy, fradius, fsx, fsy, fex, fey, fsangle, feangle;
	int cx, cy, sx, sy, ex, ey, radius;
	int drawmode;
	char szPlaceLineStart[]  = "放置连线: <左键><回车>定义起点, <右键><ESC>结束命令";
	char szPlaceLineEnd[]  = "放置连线: <左键><回车>定义终点, <右键><ESC>返回上一步, <TAB>定义属性";
	char szInfo[9][100] = {	"<shift+空格><空格>修改模式: 90度-先水平再垂直",	"<shift+空格><空格>修改模式: 90度-先垂直再水平",\
							"<shift+空格><空格>修改模式: 90度-先圆弧再连线",	"<shift+空格><空格>修改模式: 90度-先连线再圆弧",\
							"<shift+空格><空格>修改模式: 45度-先水平垂直再45度","<shift+空格><空格>修改模式: 45度-先45度再水平垂直",\
							"<shift+空格><空格>修改模式: 45度-先圆弧再连线",	"<shift+空格><空格>修改模式: 45度-先连线再圆弧",\
							"<shift+空格><空格>修改模式: 任意角度"};
	
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	Write_Help(szPlaceLineStart);

	int width = cEDA_pcbtrack.m_fWidth/m_fScale;
	if(width < 1) width = 1;

	int layer = m_nPCB_Current_Layer;
	
	COLORREF color = nEDA_PCB_LayerColor[m_nPCB_Current_Layer];

	CPen *OverwritePen1,  *OverwritePen2, *oldpen;
	OverwritePen1 = new CPen(PS_SOLID, width, color);
	OverwritePen2 = new CPen(PS_DOT, 1, color);
					
	
	bEDA_NeedElectricalGrid = TRUE;
	
	fradius = 0;

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

			Write_Help(szPlaceLineEnd);
			Write_Info(szInfo[nEDA_PCB_PlaceTrackMode]);
			do
			{
				ch1 = MouseKey();
LABEL_1:
				//Get_VPort_XY(&x1,&y1,fx1,fy1);
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
					if((fx1 != fx2)||(fy1 != fy2))
					{
						COb_pcbtrack* ptrack = new COb_pcbtrack();
						ptrack->pDocument = pDoc;
						ptrack->m_nLayer = layer;
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
						parc->m_nLayer = layer;
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
						//shift+space
						if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90ARC_START;
						else if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_90ARC_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90ARC_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45_START;
						//else if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45ARC_START;
						//else if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_45ARC_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45ARC_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_ANYANGLE;
						else if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_ANYANGLE;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_ANYANGLE) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90_START; 
					}
					else 
					{
						//space
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
						if(layer == pDoc->m_arrayPcbLayerShow.GetAt(i))
						{
							dlg.m_nComboLayer = i;
							break;
						}
					}
					dlg.m_csEditWidth = pDoc->Convert_XY_To_String(cEDA_pcbtrack.m_fWidth);
					if(dlg.DoModal() == IDOK)
					{
						cEDA_pcbtrack.m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditWidth);
						
						for(int i=0; i<=MULTILAYER; i++)
						{
							if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
							{
								layer = i;
								break;
							}
						}

						
						//color = (~nEDA_PCB_LayerColor[layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
						color = nEDA_PCB_LayerColor[layer];
						width = cEDA_pcbtrack.m_fWidth/m_fScale;
						if(width < 1) width = 1;
						delete OverwritePen1;
						OverwritePen1 = new CPen(PS_SOLID, width, color);
						delete OverwritePen2;
						OverwritePen2 = new CPen(PS_DOT, 1, color);
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
		else Command_In_Function(ch1);
    }while(1);
	
	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	
	if(undo_flag == TRUE)
	{
		

		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	
	delete OverwritePen1;
	delete OverwritePen2;

	Write_Help(szEDA_Help_Ready);

	
	bEDA_NeedElectricalGrid = FALSE;
	m_fpPcbHotPoint1.fx = m_fpPcbHotPoint1.fy = m_fpPcbHotPoint2.fx = m_fpPcbHotPoint2.fy = 0;
}

void CESDView::OnPcbPlacePad()
{
	int ch1;
	double x1,y1;
	int xs,ys;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	char szPlaceJunc[] = "放置焊盘: <左键><回车>放置结点, <右键><ESC>结束命令, <TAB>定义属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	MoveCursorToCenter();
	
	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
	Get_VPort_XY(&xs,&ys,x1,y1);
	Write_Help(szPlaceJunc);
	
	COb_pcbpad apad;
	apad.pDocument = pDoc;
	apad.m_fCx = x1;
	apad.m_fCy = y1;
	apad.m_fRotation = cEDA_pcbpad.m_fRotation;
	apad.m_fHoleDia = cEDA_pcbpad.m_fHoleDia;
	apad.m_nHoleType = cEDA_pcbpad.m_nHoleType;
	apad.m_fHoleRotation = cEDA_pcbpad.m_fHoleRotation;
	apad.m_fHoleWidth = cEDA_pcbpad.m_fHoleWidth;
	apad.m_nPadMode = cEDA_pcbpad.m_nPadMode;
	for(int i=0; i<SIGNAL_LAYER_NUM; i++)
	{
		apad.m_fSizeX[i] = cEDA_pcbpad.m_fSizeX[i];
		apad.m_fSizeY[i] = cEDA_pcbpad.m_fSizeY[i];
		apad.m_nShape[i] = cEDA_pcbpad.m_nShape[i];
	}

	nEDA_RedrawFlag = 0;
	apad.Draw(&dc, this, DRAW_DRAFT);

	
	bEDA_NeedElectricalGrid = TRUE;

	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
		{
			PcbElecHotPointsControl(&dc);
			apad.Draw(&dc, this, DRAW_DRAFT);
		}
		else if(ch1 == MOUSEMOVE)
		{
			if(nEDA_RedrawFlag == 1){ nEDA_RedrawFlag = 0;}
			else  
			{ 
				PcbElecHotPointsControl(&dc);
				apad.Draw(&dc, this, DRAW_DRAFT);
			}
			
			x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
			apad.m_fCx = x1;
			apad.m_fCy = y1;
			apad.Draw(&dc, this, DRAW_DRAFT);
		}
		else if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			PcbElecHotPointsControl(&dc);
			
			apad.Draw(&dc, this, DRAW_DRAFT);
			
			if(Exist_PV_Near_Pad(&apad) == TRUE)
			{
			   int j = AfxMessageBox("与另一个焊盘或者过孔距离太近, 确定?", MB_YESNO|MB_ICONWARNING);
			   if( j != IDYES ) continue;
			}

			COb_pcbpad* ppad = new COb_pcbpad();
			ppad->pDocument = pDoc;

			ppad->m_nLayer = cEDA_pcbpad.m_nLayer;
			ppad->m_fCx = x1;
			ppad->m_fCy = y1;
			ppad->m_fRotation = cEDA_pcbpad.m_fRotation;
			
			ppad->m_fHoleDia = cEDA_pcbpad.m_fHoleDia;
			ppad->m_nHoleType = cEDA_pcbpad.m_nHoleType;
			ppad->m_fHoleRotation = cEDA_pcbpad.m_fHoleRotation;
			ppad->m_fHoleWidth = cEDA_pcbpad.m_fHoleWidth;
			
			ppad->m_csName = cEDA_pcbpad.m_csName;
			ppad->m_nDaisychain = cEDA_pcbpad.m_nDaisychain;
			ppad->m_bLocked = cEDA_pcbpad.m_bLocked;
			ppad->m_bPlated = cEDA_pcbpad.m_bPlated;

			ppad->m_nPadMode = cEDA_pcbpad.m_nPadMode;
            for(int i=0; i<SIGNAL_LAYER_NUM; i++)
			{
				ppad->m_fSizeX[i] = cEDA_pcbpad.m_fSizeX[i];
				ppad->m_fSizeY[i] = cEDA_pcbpad.m_fSizeY[i];
				ppad->m_nShape[i] = cEDA_pcbpad.m_nShape[i];
			}

			ppad->m_bTentingTop = cEDA_pcbpad.m_bTentingTop;
			ppad->m_bTentingBottom = cEDA_pcbpad.m_bTentingBottom;
			ppad->m_nCSEV = cEDA_pcbpad.m_nCSEV;
			ppad->m_fCSE = cEDA_pcbpad.m_fCSE;
			ppad->m_nCPEV = cEDA_pcbpad.m_nCPEV;
			ppad->m_fCPE = cEDA_pcbpad.m_fCPE;

			int index = pDoc->m_arrayPcbPad.Add(ppad);
			
			int padnum = atoi(cEDA_pcbpad.m_csName);
			padnum++;
			char s[100];
			itoa(padnum, s, 10);
			cEDA_pcbpad.m_csName  = s;

			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_PAD;
			pundo->m_nIndex = index;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			
			dc.SetROP2(drawmode);
			CloseMouseCursor();

			ppad->Draw(&dc, this, DRAW_NORMAL);

			OpenMouseCursor();
			dc.SetROP2(R2_NOTXORPEN);

			apad.Draw(&dc, this, DRAW_DRAFT);
			continue;
	   }
		else if(ch1 == VK_SPACE)
		{
			apad.Draw(&dc, this, DRAW_DRAFT);

			cEDA_pcbpad.m_fRotation += 90;
			cEDA_pcbpad.m_fRotation = pDoc->Get_Correct_Angle(cEDA_pcbpad.m_fRotation);
			apad.m_fRotation = cEDA_pcbpad.m_fRotation;
			
			apad.Draw(&dc, this, DRAW_DRAFT);
	   }
	   else if(ch1 == TAB)
	   {
			CDlg_PCBEditPad dlg;
			dlg.pDoc = pDoc;
			
			
			for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
			{
				dlg.arrayStringLayer.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
			}
			for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
			{
				if(cEDA_pcbpad.m_nLayer == pDoc->m_arrayPcbLayerShow.GetAt(i))
				{
					dlg.m_nComboLayer = i;
					break;
				}
			}

			dlg.m_fEditRotation = cEDA_pcbpad.m_fRotation;

			dlg.m_csEditHoleSize = pDoc->Convert_XY_To_String(cEDA_pcbpad.m_fHoleDia);
			dlg.m_nRadioHoleType = cEDA_pcbpad.m_nHoleType;
			dlg.m_fHoleRotation = cEDA_pcbpad.m_fHoleRotation;
			dlg.m_csEditHoleWidth = pDoc->Convert_XY_To_String(cEDA_pcbpad.m_fHoleWidth);

			dlg.m_csEditDesignator = cEDA_pcbpad.m_csName;
			dlg.m_nComboType = cEDA_pcbpad.m_nDaisychain;
			dlg.m_bCheckLocked = cEDA_pcbpad.m_bLocked;
			dlg.m_bCheckPlated = cEDA_pcbpad.m_bPlated;

			dlg.m_nRadioPadMode = cEDA_pcbpad.m_nPadMode;
			dlg.m_csEditXSizeTop = pDoc->Convert_XY_To_String(cEDA_pcbpad.m_fSizeX[TOPLAYER]);
			dlg.m_csEditYSizeTop = pDoc->Convert_XY_To_String(cEDA_pcbpad.m_fSizeY[TOPLAYER]);
			dlg.m_nComboShapeTop = cEDA_pcbpad.m_nShape[TOPLAYER];
			dlg.m_csEditXSizeMid = pDoc->Convert_XY_To_String(cEDA_pcbpad.m_fSizeX[MIDLAYER1]);
			dlg.m_csEditYSizeMid = pDoc->Convert_XY_To_String(cEDA_pcbpad.m_fSizeY[MIDLAYER1]);
			dlg.m_nComboShapeMid = cEDA_pcbpad.m_nShape[MIDLAYER1];
			dlg.m_csEditXSizeBottom = pDoc->Convert_XY_To_String(cEDA_pcbpad.m_fSizeX[BOTTOMLAYER]);
			dlg.m_csEditYSizeBottom = pDoc->Convert_XY_To_String(cEDA_pcbpad.m_fSizeY[BOTTOMLAYER]);
			dlg.m_nComboShapeBottom = cEDA_pcbpad.m_nShape[BOTTOMLAYER];

			dlg.m_bCheckTentingTop = cEDA_pcbpad.m_bTentingTop;
			dlg.m_bCheckTentingBottom = cEDA_pcbpad.m_bTentingBottom;

			
			dlg.m_nRadioPaste = cEDA_pcbpad.m_nCPEV - 1;
			dlg.m_csEditPaste = pDoc->Convert_XY_To_String(cEDA_pcbpad.m_fCPE);
			
			dlg.m_nRadioSolder = cEDA_pcbpad.m_nCSEV - 1;
			dlg.m_csEditSolder = pDoc->Convert_XY_To_String(cEDA_pcbpad.m_fCSE);

			if(dlg.DoModal() == IDOK)
			{
				
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(dlg.arrayStringLayer.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
					{
						cEDA_pcbpad.m_nLayer = i;
						break;
					}
				}

				
				if(cEDA_pcbpad.m_nLayer == MULTILAYER)
				{
					cEDA_pcbpad.m_fHoleDia = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditHoleSize);
					cEDA_pcbpad.m_nHoleType = dlg.m_nRadioHoleType;
					cEDA_pcbpad.m_fHoleRotation = dlg.m_fHoleRotation;
					cEDA_pcbpad.m_fHoleWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditHoleWidth);

					if(cEDA_pcbpad.m_fHoleDia <= 0) cEDA_pcbpad.m_fHoleDia = 20;
				}
				else
				{
					cEDA_pcbpad.m_fHoleDia = 0;
					cEDA_pcbpad.m_nHoleType = PADHOLE_ROUND;
					cEDA_pcbpad.m_fHoleRotation = 0;
					cEDA_pcbpad.m_fHoleWidth = 0;
				}

				cEDA_pcbpad.m_fRotation = dlg.m_fEditRotation;

				cEDA_pcbpad.m_csName = dlg.m_csEditDesignator;
				cEDA_pcbpad.m_nDaisychain = dlg.m_nComboType;
				cEDA_pcbpad.m_bLocked = dlg.m_bCheckLocked;
				cEDA_pcbpad.m_bPlated = dlg.m_bCheckPlated;

				cEDA_pcbpad.m_nPadMode = dlg.m_nRadioPadMode;
				if(cEDA_pcbpad.m_nPadMode == PAD_MODE_SIMPLE)
				{
					for(int i=0; i<32; i++)
					{
						cEDA_pcbpad.m_fSizeX[i] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditXSizeTop);
						cEDA_pcbpad.m_fSizeY[i] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditYSizeTop);
						cEDA_pcbpad.m_nShape[i] = dlg.m_nComboShapeTop;
					}
				}
				else
				{
					cEDA_pcbpad.m_fSizeX[TOPLAYER] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditXSizeTop);
					cEDA_pcbpad.m_fSizeY[TOPLAYER] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditYSizeTop);
					cEDA_pcbpad.m_nShape[TOPLAYER] = dlg.m_nComboShapeTop;

					cEDA_pcbpad.m_fSizeX[BOTTOMLAYER] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditXSizeBottom);
					cEDA_pcbpad.m_fSizeY[BOTTOMLAYER] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditYSizeBottom);
					cEDA_pcbpad.m_nShape[BOTTOMLAYER] = dlg.m_nComboShapeBottom;

					for(int i=MIDLAYER1; i<=MIDLAYER30; i++)
					{
						cEDA_pcbpad.m_fSizeX[i] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditXSizeMid);
						cEDA_pcbpad.m_fSizeY[i] = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditYSizeMid);
						cEDA_pcbpad.m_nShape[i] = dlg.m_nComboShapeMid;
					}
				}

				cEDA_pcbpad.m_bTentingTop = dlg.m_bCheckTentingTop;
				cEDA_pcbpad.m_bTentingBottom = dlg.m_bCheckTentingBottom;

				cEDA_pcbpad.m_nCPEV = dlg.m_nRadioPaste + 1;
				cEDA_pcbpad.m_fCPE = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditPaste);
				cEDA_pcbpad.m_nCSEV = dlg.m_nRadioSolder + 1;
				cEDA_pcbpad.m_fCSE = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditSolder);

				
				apad.m_nLayer = cEDA_pcbpad.m_nLayer;
				apad.m_fRotation = cEDA_pcbpad.m_fRotation;
				apad.m_fHoleDia = cEDA_pcbpad.m_fHoleDia;
				apad.m_nHoleType = cEDA_pcbpad.m_nHoleType;
				apad.m_fHoleRotation = cEDA_pcbpad.m_fHoleRotation;
				apad.m_fHoleWidth = cEDA_pcbpad.m_fHoleWidth;
				apad.m_nPadMode = cEDA_pcbpad.m_nPadMode;
				for(int i=0; i<SIGNAL_LAYER_NUM; i++)
				{
					apad.m_fSizeX[i] = cEDA_pcbpad.m_fSizeX[i];
					apad.m_fSizeY[i] = cEDA_pcbpad.m_fSizeY[i];
					apad.m_nShape[i] = cEDA_pcbpad.m_nShape[i];
				}
			}
	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	
	bEDA_NeedElectricalGrid = FALSE;
	m_fpPcbHotPoint1.fx = m_fpPcbHotPoint1.fy = m_fpPcbHotPoint2.fx = m_fpPcbHotPoint2.fy = 0;
}

BOOL CESDView::Exist_PV_Near_Pad(COb_pcbpad* ppad_origin)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{	
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag != FLAG_NORMAL) continue;

		if(((ppad->m_nLayer == ppad_origin->m_nLayer)||(ppad->m_nLayer == MULTILAYER)||(ppad_origin->m_nLayer == MULTILAYER))&&(Pad_Pad_Connect(ppad_origin, ppad, 0) == TRUE))
		{
			return TRUE;
		}
	}
	
	
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag != FLAG_NORMAL) continue;
		
		if((pDoc->IsConnectedToVia(ppad_origin->m_nLayer, pvia) == TRUE)&&(Pad_Via_Connect(ppad_origin, pvia, 0) == TRUE))
		{
			return TRUE;
		}
	}

	return FALSE;
}

void CESDView::OnPcbPlaceVia()
{
	int ch1;
	double x1,y1;
	int xs,ys;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	char szPlaceJunc[] = "放置过孔: <左键><回车>放置结点, <右键><ESC>结束命令, <TAB>定义属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	MoveCursorToCenter();

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
	Get_VPort_XY(&xs,&ys,x1,y1);
	Write_Help(szPlaceJunc);
	
	COb_pcbvia avia;
	avia.pDocument = pDoc;
	avia.m_fCx = x1;
	avia.m_fCy = y1;
	avia.m_fDia = cEDA_pcbvia.m_fDia;
	avia.m_fHoleDia = cEDA_pcbvia.m_fHoleDia;
	avia.m_nLayer = MULTILAYER;
	avia.m_nStartLayer = TOPLAYER;//cEDA_pcbvia.m_nStartLayer;
	avia.m_nEndLayer = BOTTOMLAYER;//cEDA_pcbvia.m_nEndLayer;

	nEDA_RedrawFlag = 0;
	avia.Draw(&dc, this, DRAW_DRAFT);

	
	bEDA_NeedElectricalGrid = TRUE;

	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
		{
			PcbElecHotPointsControl(&dc);

			avia.Draw(&dc, this, DRAW_DRAFT);
		}
		else if(ch1 == MOUSEMOVE)
		{
			if(nEDA_RedrawFlag == 1){ nEDA_RedrawFlag = 0;}
			else  
			{ 
				PcbElecHotPointsControl(&dc);

				avia.Draw(&dc, this, DRAW_DRAFT);
			}
			
			x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
			avia.m_fCx = x1;
			avia.m_fCy = y1;
			avia.Draw(&dc, this, DRAW_DRAFT);
	   }
       else if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
			PcbElecHotPointsControl(&dc);
			avia.Draw(&dc, this, DRAW_DRAFT);
			
			if(Exist_PV_Near_Via(&avia) == TRUE)
			{
			   int j = AfxMessageBox("与另一个焊盘或者过孔距离太近, 确定?", MB_YESNO|MB_ICONWARNING);
			   if( j != IDYES ) continue;
			}

			COb_pcbvia* pvia = new COb_pcbvia();
			pvia->pDocument = pDoc;

			pvia->m_fCx = x1;
			pvia->m_fCy = y1;
			pvia->m_fDia = cEDA_pcbvia.m_fDia;
			pvia->m_fHoleDia = cEDA_pcbvia.m_fHoleDia;
			//pvia->m_nLayer = MULTILAYER;
			pvia->m_nStartLayer = TOPLAYER;//cEDA_pcbvia.m_nStartLayer;
			pvia->m_nEndLayer = BOTTOMLAYER;//cEDA_pcbvia.m_nEndLayer;
			pvia->m_bTentingTop = cEDA_pcbvia.m_bTentingTop;
			pvia->m_bTentingBottom = cEDA_pcbvia.m_bTentingBottom;
			pvia->m_nCSEV = cEDA_pcbvia.m_nCSEV;
			pvia->m_fCSE = cEDA_pcbvia.m_fCSE;

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
			

			dc.SetROP2(drawmode);
			CloseMouseCursor();
			pvia->Draw(&dc, this, 0);
			OpenMouseCursor();
			dc.SetROP2(R2_NOTXORPEN);

			avia.Draw(&dc, this, DRAW_DRAFT);
			continue;
	   }
	   else if(ch1 == TAB)
	   {
			CDlg_PCBEditVia dlg;
		
			
			int layer = 0;
			for(int i=0; i<=MULTILAYER; i++)
			{
				dlg.arrayLayer.Add(pDoc->m_sPCB_LayerParam[layer].csLayerName);
				if(layer == BOTTOMLAYER) break;
				layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
			}

			for(int i=0; i<dlg.arrayLayer.GetCount(); i++)
			{
				if(pDoc->m_sPCB_LayerParam[cEDA_pcbvia.m_nStartLayer].csLayerName == dlg.arrayLayer.GetAt(i)) 
				{
					dlg.m_nStartLayer = i;
					break;
				}
			}
			for(int i=0; i<dlg.arrayLayer.GetCount(); i++)
			{
				if(pDoc->m_sPCB_LayerParam[cEDA_pcbvia.m_nEndLayer].csLayerName == dlg.arrayLayer.GetAt(i)) 
				{
					dlg.m_nEndLayer = i;
					break;
				}
			}
			
			dlg.m_csEditDia = pDoc->Convert_XY_To_String(cEDA_pcbvia.m_fDia);
			dlg.m_csEditHoleDia = pDoc->Convert_XY_To_String(cEDA_pcbvia.m_fHoleDia);

			dlg.m_bCheckTentingBottom = cEDA_pcbvia.m_bTentingBottom;
			dlg.m_bCheckTentingTop = cEDA_pcbvia.m_bTentingTop;
			dlg.m_nRadioExpansion = cEDA_pcbvia.m_nCSEV - 1;
			dlg.m_csEditExpansion = pDoc->Convert_XY_To_String(cEDA_pcbvia.m_fCSE);
			
			if(dlg.DoModal() == IDOK)
			{
				
				if(dlg.m_nStartLayer > dlg.m_nEndLayer)
				{
					int i = dlg.m_nStartLayer;
					dlg.m_nStartLayer = dlg.m_nEndLayer;
					dlg.m_nEndLayer = i;
				}
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(dlg.arrayLayer.GetAt(dlg.m_nStartLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
					{
						cEDA_pcbvia.m_nStartLayer = i;
						break;
					}
				}
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(dlg.arrayLayer.GetAt(dlg.m_nEndLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
					{
						cEDA_pcbvia.m_nEndLayer = i;
						break;
					}
				}

				cEDA_pcbvia.m_fDia = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditDia);
				cEDA_pcbvia.m_fHoleDia = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditHoleDia);

				//m_bLocked = dlg.m_bCheckLocked;
				cEDA_pcbvia.m_bTentingBottom = dlg.m_bCheckTentingBottom;
				cEDA_pcbvia.m_bTentingTop = dlg.m_bCheckTentingTop;
				cEDA_pcbvia.m_nCSEV = dlg.m_nRadioExpansion + 1;
				cEDA_pcbvia.m_fCSE = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditExpansion);

				avia.m_fDia = cEDA_pcbvia.m_fDia;
				avia.m_fHoleDia = cEDA_pcbvia.m_fHoleDia;
			}
	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	
	bEDA_NeedElectricalGrid = FALSE;
	m_fpPcbHotPoint1.fx = m_fpPcbHotPoint1.fy = m_fpPcbHotPoint2.fx = m_fpPcbHotPoint2.fy = 0;
}

BOOL CESDView::Exist_PV_Near_Via(COb_pcbvia* pvia_origin)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag != FLAG_NORMAL) continue;
		
		if((pDoc->IsConnectedToVia(ppad->m_nLayer, pvia_origin) == TRUE)&&(Pad_Via_Connect(ppad, pvia_origin, 0) == TRUE)) return TRUE;
	}

	
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag != FLAG_NORMAL) continue;
		
		if((pDoc->IsConnected_ViaToVia(pvia, pvia_origin) == TRUE)&&(Via_Via_Connect(pvia, pvia_origin, 0) == TRUE)) return TRUE;
	}

	return FALSE;
}

void CESDView::OnPcbPlaceFill()
{
	unsigned ch1;
	double fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;
	CRect rect;
	
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	char szPlaceOneCorner[] = "放置矩形充填区: <左键><回车>定义区域一角, <右键><ESC>结束命令";
	char szPlaceAnotherCorner[] = "放置长方形: <左键><回车>定义区域另一角, <右键><ESC>结束命令";
	char szInfo[] = "<TAB>属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	MoveCursorToCenter();
	flag = 0;

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);
	
	int layer = m_nPCB_Current_Layer;
	//COLORREF color = (~nEDA_PCB_LayerColor[layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
	COLORREF color = nEDA_PCB_LayerColor[layer];

	CPen *pPen, *oldpen;
	pPen = new CPen(PS_SOLID, 1, color);
	oldpen = dc.SelectObject(pPen);
	CBrush *pBrush, *oldBursh;
	pBrush = new CBrush(color);
	oldBursh = dc.SelectObject(pBrush);

PLACEFILL:
	Write_Help(szPlaceOneCorner);
	Write_Info(szInfo);
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			fx1 = m_fCurrentSnapX; fy1 = m_fCurrentSnapY;
			Get_VPort_XY(&xs,&ys,fx1,fy1);
			xe=xs;
			ye=ys;
			Write_Help(szPlaceAnotherCorner);
			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xs,&ys,fx1,fy1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					rect.left = xs;		rect.right = xe;
					rect.bottom = ye;	rect.top = ys;
					dc.Rectangle(&rect);
					break;

				}
				if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{ 
						flag=1;
						rect.left = xs;		rect.right = xe;
						rect.bottom = ye;	rect.top = ys;
						dc.Rectangle(&rect);
					}
					else
					{
						if(nEDA_RedrawFlag == ON){ nEDA_RedrawFlag = OFF;}
						else 
						{
							rect.left = xs;		rect.right = xe;
							rect.bottom = ye;	rect.top = ys;
							dc.Rectangle(&rect);
						}
						fx2 = m_fCurrentSnapX; fy2 = m_fCurrentSnapY;
						Get_VPort_XY(&xe,&ye,fx2,fy2);

						rect.left = xs;		rect.right = xe;
						rect.bottom = ye;	rect.top = ys;
						dc.Rectangle(&rect);
					}
				}
				if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{  
					rect.left = xs;		rect.right = xe;
					rect.bottom = ye;	rect.top = ys;
					dc.Rectangle(&rect);
					
					double fminx, fmaxx, fminy, fmaxy;
					fminx = MIN(fx1,fx2);
					fminy = MIN(fy1,fy2);
					fmaxx = MAX(fx1,fx2);
					fmaxy = MAX(fy1,fy2);

					COb_pcbfill* pfill = new COb_pcbfill();
					pfill->pDocument = pDoc;
					pfill->m_fX1 = fminx;
					pfill->m_fY1 = fminy;
					pfill->m_fX2 = fmaxx;
					pfill->m_fY2 = fmaxy;
					pfill->m_fRotation = 0;
					pfill->m_nLayer = layer;
					int index = pDoc->m_arrayPcbFill.Add(pfill);

					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					

					dc.SetROP2(drawmode);
					CloseMouseCursor();
					pfill->Draw(&dc, this);
					OpenMouseCursor();
					dc.SetROP2(R2_NOTXORPEN);
					
					goto PLACEFILL;
				}
				else if(ch1 == TAB)
				{
					CDlg_PCBEditFill dlg;
					
					for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
					{
						dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
					}
					for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
					{
						if(layer == pDoc->m_arrayPcbLayerShow.GetAt(i))
						{
							dlg.m_nComboLayer = i;
							break;
						}
					}
					if(dlg.DoModal() == IDOK)
					{
						
						
						for(int i=0; i<=MULTILAYER; i++)
						{
							if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
							{
								layer = i;
								break;
							}
						}
					}
					color = nEDA_PCB_LayerColor[layer];
					delete pPen;
					pPen = new CPen(PS_SOLID, 1, color);
					dc.SelectObject(pPen);
					delete pBrush;
					pBrush = new CBrush(color);
					dc.SelectObject(pBrush);
				}
			}while(1);
		}
		else if(ch1 == TAB)
		{
			CDlg_PCBEditFill dlg;
			
			for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
			{
				dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
			}
			for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
			{
				if(layer == pDoc->m_arrayPcbLayerShow.GetAt(i))
				{
					dlg.m_nComboLayer = i;
					break;
				}
			}
			if(dlg.DoModal() == IDOK)
			{
				
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
					{
						layer = i;
						break;
					}
				}
				color = nEDA_PCB_LayerColor[layer];
				delete pPen;
				pPen = new CPen(PS_SOLID, 1, color);
				dc.SelectObject(pPen);
				delete pBrush;
				pBrush = new CBrush(color);
				dc.SelectObject(pBrush);
			}
		}
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);

	delete pPen;
	delete pBrush;

	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);
}

void CESDView::OnPcbPlaceString()
{
	int ch1;
	int flag;
	double x1,y1;
	POSITION pos;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	CBitmap *pBitmap = NULL;
	

	char szPlaceText[] = "放置文字: <空格>旋转, <左键><回车>放置, <右键><ESC>结束命令";
	char szInfo[] = "<TAB>修改文字及属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	MoveCursorToCenter();
	
	x1 = m_fCurrentSnapX; 
	y1 = m_fCurrentSnapY;

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	int layer = m_nPCB_Current_Layer;
	//COLORREF color = (~nEDA_PCB_LayerColor[layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
	COLORREF color = nEDA_PCB_LayerColor[layer];

	if(cEDA_pcbtext.m_csText.IsEmpty() == TRUE) cEDA_pcbtext.m_csText = "String";
	
	
	COb_pcbtext atext;
	atext.pDocument = pDoc;
	atext.m_fHeight = cEDA_pcbtext.m_fHeight;
	atext.m_fWidth = cEDA_pcbtext.m_fWidth;
	atext.m_fRotation = cEDA_pcbtext.m_fRotation;
	atext.m_csText = cEDA_pcbtext.m_csText;
	atext.m_bMirror = cEDA_pcbtext.m_bMirror;
	atext.m_nStrokeFont = cEDA_pcbtext.m_nStrokeFont;
	atext.m_bUseTTFont = cEDA_pcbtext.m_bUseTTFont;
	atext.m_bBold = cEDA_pcbtext.m_bBold;
	atext.m_bItalic = cEDA_pcbtext.m_bItalic;
	atext.m_bInverted = cEDA_pcbtext.m_bInverted;
	atext.m_csTTFontName = cEDA_pcbtext.m_csTTFontName;
	atext.m_fInvertedBorder = cEDA_pcbtext.m_fInvertedBorder;
	atext.m_nLayer = layer;

	atext.UpdateStringBorder(this, TRUE);
	if(atext.m_bUseTTFont == TRUE) atext.DrawStringRect(&dc, this);
	else atext.Draw(&dc, this);

	Write_Help(szPlaceText);
	Write_Info(szInfo);

	flag = cEDA_schtext.m_nOrientation/90;
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			
			CloseMouseCursor();
			atext.UpdateStringBorder(this, FALSE);
			if(atext.m_bUseTTFont == TRUE) atext.DrawStringRect(&dc, this);
			else atext.Draw(&dc, this);
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
				atext.UpdateStringBorder(this, FALSE);
				if(atext.m_bUseTTFont == TRUE) atext.DrawStringRect(&dc, this);
				else atext.Draw(&dc, this);
				OpenMouseCursor();
			}

			x1 = m_fCurrentSnapX;  y1 = m_fCurrentSnapY;
			atext.m_fSx = x1; atext.m_fSy = y1;
			
			
			CloseMouseCursor();
			atext.UpdateStringBorder(this, FALSE);
			if(atext.m_bUseTTFont == TRUE) atext.DrawStringRect(&dc, this);
			else atext.Draw(&dc, this);
			OpenMouseCursor();

			//atext.GetMoveTextRect(&dc, this);
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			
			CloseMouseCursor();
			atext.UpdateStringBorder(this, FALSE);
			if(atext.m_bUseTTFont == TRUE) atext.DrawStringRect(&dc, this);
			else atext.Draw(&dc, this);
			OpenMouseCursor();

			atext.m_fSx = x1; atext.m_fSy = y1;
			
			COb_pcbtext* ptext = new COb_pcbtext();
			ptext->Copy(&atext);
			ptext->UpdateStringBorder(this, TRUE);

			int index = pDoc->m_arrayPcbText.Add(ptext);
			
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_TEXT;
			pundo->m_nIndex = index;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			

			dc.SetROP2(drawmode);
			CloseMouseCursor();
			ptext->Draw(&dc, this);
			dc.SetROP2(R2_NOTXORPEN);

			atext.UpdateStringBorder(this, FALSE);
			if(atext.m_bUseTTFont == TRUE) atext.DrawStringRect(&dc, this);
			else atext.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if((ch1==KEY_X)||(ch1==KEY_x))
		{  
			CloseMouseCursor();
			atext.UpdateStringBorder(this, FALSE);
			if(atext.m_bUseTTFont == TRUE) atext.DrawStringRect(&dc, this);
			else atext.Draw(&dc, this);
			
			if(atext.m_bMirror == TRUE) atext.m_bMirror = FALSE;
			else atext.m_bMirror = TRUE;
			cEDA_pcbtext.m_bMirror = atext.m_bMirror;

			atext.UpdateStringBorder(this, FALSE);
			if(atext.m_bUseTTFont == TRUE) atext.DrawStringRect(&dc, this);
			else atext.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if(ch1 == VK_SPACE)
		{ 
			CloseMouseCursor();
			atext.UpdateStringBorder(this, FALSE);
			if(atext.m_bUseTTFont == TRUE) atext.DrawStringRect(&dc, this);
			else atext.Draw(&dc, this);
			
			atext.m_fRotation += 90;
			cEDA_pcbtext.m_fRotation += 90;

			atext.UpdateStringBorder(this, FALSE);
			if(atext.m_bUseTTFont == TRUE) atext.DrawStringRect(&dc, this);
			else atext.Draw(&dc, this);
			OpenMouseCursor();
		}
		else  if(ch1 == TAB)
		{
			CDlg_PCBEditText dlg;
			
			for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
			{
				dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
			}
			for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
			{
				if(layer == pDoc->m_arrayPcbLayerShow.GetAt(i))
				{
					dlg.m_nComboLayer = i;
					break;
				}
			}
			dlg.m_csEditText = cEDA_pcbtext.m_csText;
			dlg.m_csEditHeight = pDoc->Convert_XY_To_String(cEDA_pcbtext.m_fHeight);
			dlg.m_csEditWidth = pDoc->Convert_XY_To_String(cEDA_pcbtext.m_fWidth);
			dlg.m_fEditRotation = cEDA_pcbtext.m_fRotation;
			dlg.m_bCheckLocked = cEDA_pcbtext.m_bLocked;
			dlg.m_bCheckMirror = cEDA_pcbtext.m_bMirror;
			
			for(int i=0; i<arrayEDA_SystemFont.GetCount(); i++)
			{
				if(cEDA_pcbtext.m_csTTFontName == arrayEDA_SystemFont.GetAt(i)) 
				{
					dlg.nTTFont = i;
					break;
				}
			}
			dlg.nStrokeFont = cEDA_pcbtext.m_nStrokeFont;
			
			if(cEDA_pcbtext.m_bUseTTFont == TRUE)
			{
				dlg.m_nRadioFont = 0;
				dlg.m_nComboFont = dlg.nTTFont;
			}
			else 
			{
				dlg.m_nRadioFont = 1;
				dlg.m_nComboFont = dlg.nStrokeFont;
			}

			
			dlg.m_bCheckBold = cEDA_pcbtext.m_bBold;
			dlg.m_bCheckItalic = cEDA_pcbtext.m_bItalic;
			dlg.m_bCheckInverted = cEDA_pcbtext.m_bInverted;
			dlg.m_csEditInverted = pDoc->Convert_XY_To_String(cEDA_pcbtext.m_fInvertedBorder);
			
			if(dlg.DoModal() == IDOK)
			{
				
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
					{
						layer = i;
						break;
					}
				}
				
				cEDA_pcbtext.m_csText = dlg.m_csEditText;
				cEDA_pcbtext.m_fHeight = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditHeight);
				cEDA_pcbtext.m_fWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditWidth);
				cEDA_pcbtext.m_fRotation = dlg.m_fEditRotation;

				cEDA_pcbtext.m_bLocked = dlg.m_bCheckLocked;
				cEDA_pcbtext.m_bMirror = dlg.m_bCheckMirror;
				
				
				if(dlg.m_nRadioFont == 0)
				{
					cEDA_pcbtext.m_bUseTTFont = TRUE;
					cEDA_pcbtext.m_csTTFontName = arrayEDA_SystemFont.GetAt(dlg.m_nComboFont);
				}
				else
				{
					cEDA_pcbtext.m_bUseTTFont = FALSE;
					cEDA_pcbtext.m_nStrokeFont = dlg.m_nComboFont;
				}

				
				cEDA_pcbtext.m_bBold = dlg.m_bCheckBold;
				cEDA_pcbtext.m_bItalic = dlg.m_bCheckItalic;
				cEDA_pcbtext.m_bInverted = dlg.m_bCheckInverted;
				cEDA_pcbtext.m_fInvertedBorder = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditInverted);

				atext.m_fHeight = cEDA_pcbtext.m_fHeight;
				atext.m_fWidth = cEDA_pcbtext.m_fWidth;
				atext.m_fRotation = cEDA_pcbtext.m_fRotation;
				atext.m_csText = cEDA_pcbtext.m_csText;
				atext.m_bMirror = cEDA_pcbtext.m_bMirror;
				atext.m_nStrokeFont = cEDA_pcbtext.m_nStrokeFont;
				atext.m_bUseTTFont = cEDA_pcbtext.m_bUseTTFont;
				atext.m_bBold = cEDA_pcbtext.m_bBold;
				atext.m_bItalic = cEDA_pcbtext.m_bItalic;
				atext.m_bInverted = cEDA_pcbtext.m_bInverted;
				atext.m_csTTFontName = cEDA_pcbtext.m_csTTFontName;
				atext.m_fInvertedBorder = cEDA_pcbtext.m_fInvertedBorder;
				atext.m_nLayer = layer;

				atext.UpdateStringBorder(this, TRUE);

			}
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	if (pBitmap) delete pBitmap;

	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL); m_bClipState = FALSE;

	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);
}

void CESDView::OnPcbPlaceRegion()
{
	int ch1;
	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4, tempx, tempy;
	int x1,y1,x2,y2,x3,y3,x4,y4,flag;
	double fcx, fcy, fradius, fsx, fsy, fex, fey, fsangle, feangle;
	int cx, cy, sx, sy, ex, ey, radius;
	int index;
	Struct_RegionVertex vertex;
	char szPlaceRegion[]  = "放置区域: <左键><回车>定义顶点, <右键><ESC>结束命令";
	char szInfo[9][100] = {	"<shift+空格><空格>修改模式: 90度-先水平再垂直",	"<shift+空格><空格>修改模式: 90度-先垂直再水平",\
							"<shift+空格><空格>修改模式: 90度-先圆弧再连线",	"<shift+空格><空格>修改模式: 90度-先连线再圆弧",\
							"<shift+空格><空格>修改模式: 45度-先水平垂直再45度","<shift+空格><空格>修改模式: 45度-先45度再水平垂直",\
							"<shift+空格><空格>修改模式: 45度-先圆弧再连线",	"<shift+空格><空格>修改模式: 45度-先连线再圆弧",\
							"<shift+空格><空格>修改模式: 任意角度"};
	
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	Write_Help(szPlaceRegion);

	int layer = m_nPCB_Current_Layer;
	COLORREF color = (~nEDA_PCB_LayerColor[m_nPCB_Current_Layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
	
	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	COb_pcbregion aregion;
	aregion.pDocument = pDoc;
	aregion.m_nLayer = layer;
	
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
		COb_pcbregion* pregion = new COb_pcbregion();
		pregion->Copy(&aregion);
		pregion->pDocument = pDoc;
		index = pDoc->m_arrayPcbRegion.Add(pregion);
		
		if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_DELETE;
		pundo->m_nOb = PCB_ELEMENT_REGION;
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		

		CloseMouseCursor();
		pregion->Draw(&dc, this);
		OpenMouseCursor();

	}


	//if(pregion != NULL) pregion->Draw(&dc, this);
	
	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	Write_Help(szEDA_Help_Ready);

}


void CESDView::OnPcbPlacePolygonCutout()
{
	int ch1;
	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4, tempx, tempy;
	int x1,y1,x2,y2,x3,y3,x4,y4,flag;
	double fcx, fcy, fradius, fsx, fsy, fex, fey, fsangle, feangle;
	int cx, cy, sx, sy, ex, ey, radius;
	int index;
	Struct_RegionVertex vertex;
	char szPlaceRegion[]  = "放置挖空区: <左键><回车>定义顶点, <右键><ESC>结束命令";
	char szInfo[9][100] = {	"<shift+空格><空格>修改模式: 90度-先水平再垂直",	"<shift+空格><空格>修改模式: 90度-先垂直再水平",\
							"<shift+空格><空格>修改模式: 90度-先圆弧再连线",	"<shift+空格><空格>修改模式: 90度-先连线再圆弧",\
							"<shift+空格><空格>修改模式: 45度-先水平垂直再45度","<shift+空格><空格>修改模式: 45度-先45度再水平垂直",\
							"<shift+空格><空格>修改模式: 45度-先圆弧再连线",	"<shift+空格><空格>修改模式: 45度-先连线再圆弧",\
							"<shift+空格><空格>修改模式: 任意角度"};
	
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
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
	aregion.m_nLayer = layer;
	//aregion.m_nHoleNum = 0;
	aregion.m_bPolygonCutout = TRUE;

	
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
					aregion.DottedFrameRegion(&dc, this, color);
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
							
							CloseMouseCursor();
							aregion.DottedFrameRegion(&dc, this, color);
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
						aregion.DottedFrameRegion(&dc, this, color);
						OpenMouseCursor();
					}
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					//ElecHotPointsControl(&dc);
					if((fx1==fx3)&&(fy1==fy3)) continue;

					CloseMouseCursor();
					aregion.DottedFrameRegion(&dc, this, color);
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

					aregion.DottedFrameRegion(&dc, this, color);

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
		COb_pcbregion* pregion = new COb_pcbregion();
		pregion->Copy(&aregion);
		pregion->pDocument = pDoc;
		index = pDoc->m_arrayPcbRegion.Add(pregion);
		
		if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_DELETE;
		pundo->m_nOb = PCB_ELEMENT_REGION;
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		

		CloseMouseCursor();
		pregion->Draw(&dc, this);
		OpenMouseCursor();

	}


	//if(pregion != NULL) pregion->Draw(&dc, this);
	
	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	Write_Help(szEDA_Help_Ready);

	
}

void CESDView::OnPcbPlacePolygon()
{
	int ch1;
	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4, tempx, tempy;
	int x1,y1,x2,y2,x3,y3,x4,y4,flag;
	double fcx, fcy, fradius, fsx, fsy, fex, fey, fsangle, feangle;
	int cx, cy, sx, sy, ex, ey, radius;
	int index;
	Struct_RegionVertex vertex;
	char szPlaceRegion[]  = "放置覆铜: <左键><回车>定义顶点, <右键><ESC>结束命令";
	char szInfo[9][100] = {	"<shift+空格><空格>修改模式: 90度-先水平再垂直",	"<shift+空格><空格>修改模式: 90度-先垂直再水平",\
							"<shift+空格><空格>修改模式: 90度-先圆弧再连线",	"<shift+空格><空格>修改模式: 90度-先连线再圆弧",\
							"<shift+空格><空格>修改模式: 45度-先水平垂直再45度","<shift+空格><空格>修改模式: 45度-先45度再水平垂直",\
							"<shift+空格><空格>修改模式: 45度-先圆弧再连线",	"<shift+空格><空格>修改模式: 45度-先连线再圆弧",\
							"<shift+空格><空格>修改模式: 任意角度"};
	
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	if(m_nPCB_Current_Layer > BOTTOMLAYER) 
	{
		AfxMessageBox("覆铜区只能放在信号层上, 命令终止.");
		return;
	}

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	CDlg_PCBEditPolygon dlg;

	
	if(cEDA_pcbpolygon.m_nHatchStyle == PCB_HATCH_SOLID) dlg.m_nFillMode = 0;
	else if((cEDA_pcbpolygon.m_nHatchStyle ==  PCB_HATCH_90) || (cEDA_pcbpolygon.m_nHatchStyle ==  PCB_HATCH_45) || \
		    (cEDA_pcbpolygon.m_nHatchStyle ==  PCB_HATCH_H ) || (cEDA_pcbpolygon.m_nHatchStyle ==  PCB_HATCH_V ) )
	{
		dlg.m_nFillMode = 1;
	}
	else dlg.m_nFillMode = 2;
	
	
	dlg.m_bCheckRemoveIsland = cEDA_pcbpolygon.m_bRemoveIslandsByArea;
	dlg.m_nEditRemoveIsland = (int)ceil(cEDA_pcbpolygon.m_fAreaThreshold);
	dlg.m_bCheckRemoveNeck = cEDA_pcbpolygon.m_bRemoveNecks;
	dlg.m_csEditRemoveNeck = pDoc->Convert_XY_To_String(cEDA_pcbpolygon.m_fNeckWidthThreshold);
	dlg.m_csEditArcPrecision = pDoc->Convert_XY_To_String(cEDA_pcbpolygon.m_fArcResolution);
	
	dlg.m_csEditPourGap = pDoc->Convert_XY_To_String(cEDA_pcbpolygon.m_fPourGap);

	
	dlg.m_csEditTrackWidth = pDoc->Convert_XY_To_String(cEDA_pcbpolygon.m_fTrackWidth);
	dlg.m_csEditGridSize = pDoc->Convert_XY_To_String(cEDA_pcbpolygon.m_fGridSize);
	if(cEDA_pcbpolygon.m_bUseOctagons == FALSE)
	{
		dlg.m_nPadStyle = 0;
	}
	else dlg.m_nPadStyle = 1;
	if(cEDA_pcbpolygon.m_nHatchStyle ==  PCB_HATCH_45) dlg.m_nHatchStyle = 1;
	else if(cEDA_pcbpolygon.m_nHatchStyle ==  PCB_HATCH_H) dlg.m_nHatchStyle = 2;
	else if(cEDA_pcbpolygon.m_nHatchStyle ==  PCB_HATCH_V) dlg.m_nHatchStyle = 3;
	else dlg.m_nHatchStyle = 0;

	
	dlg.m_csName = cEDA_pcbpolygon.m_csName;
	
	for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
	{
		dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
	}
	for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
	{
		if(cEDA_pcbpolygon.m_nLayer == pDoc->m_arrayPcbLayerShow.GetAt(i))
		{
			dlg.m_nComboLayer = i;
			break;
		}
	}
	dlg.m_csEditMinLength = pDoc->Convert_XY_To_String(cEDA_pcbpolygon.m_fMinPrimLength);
	dlg.m_bCheckPrimitiveLock = cEDA_pcbpolygon.m_bPrimitiveLocked;
	dlg.m_bCheckLocked = cEDA_pcbpolygon.m_bLocked;
	dlg.m_bCheckIgnoreViolation = cEDA_pcbpolygon.m_bIgnoreViolations;

	
	dlg.arrayStringNet.Add("无网络");
	for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = (COb_pcbnet*)pDoc->m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;
	
		dlg.arrayStringNet.Add(pnet->m_csNetName);
	}
	dlg.m_csComboNet = "无网络"; 

	dlg.m_nComboPourStyle = cEDA_pcbpolygon.m_nPourOverStyle;
	dlg.m_bCheckRemoveDead = FALSE;

	if(dlg.DoModal() != IDOK)  return;
		
	
	if(dlg.m_nFillMode == 0) cEDA_pcbpolygon.m_nHatchStyle = PCB_HATCH_SOLID;
	else if(dlg.m_nFillMode == 2) cEDA_pcbpolygon.m_nHatchStyle = PCB_HATCH_NONE;
	else
	{
		if(dlg.m_nHatchStyle == 1) cEDA_pcbpolygon.m_nHatchStyle = PCB_HATCH_45;
		else if(dlg.m_nHatchStyle == 2) cEDA_pcbpolygon.m_nHatchStyle = PCB_HATCH_H;
		else if(dlg.m_nHatchStyle == 3) cEDA_pcbpolygon.m_nHatchStyle = PCB_HATCH_V;
		else cEDA_pcbpolygon.m_nHatchStyle = PCB_HATCH_90;
	}

	
	cEDA_pcbpolygon.m_bRemoveIslandsByArea = dlg.m_bCheckRemoveIsland;
	cEDA_pcbpolygon.m_fAreaThreshold = (double)(dlg.m_nEditRemoveIsland);
	cEDA_pcbpolygon.m_bRemoveNecks = dlg.m_bCheckRemoveNeck;
	cEDA_pcbpolygon.m_fNeckWidthThreshold = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditRemoveNeck);
	cEDA_pcbpolygon.m_fArcResolution = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditArcPrecision);
	
	cEDA_pcbpolygon.m_fPourGap = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditPourGap);
	if(cEDA_pcbpolygon.m_fPourGap < 5) cEDA_pcbpolygon.m_fPourGap = 5;
		

	cEDA_pcbpolygon.m_fTrackWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTrackWidth);
	cEDA_pcbpolygon.m_fGridSize = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditGridSize);
	if(dlg.m_nPadStyle == 0) cEDA_pcbpolygon.m_bUseOctagons = FALSE;
	else cEDA_pcbpolygon.m_bUseOctagons = TRUE;
	
	
	cEDA_pcbpolygon.m_csName = dlg.m_csName;

	for(int i=0; i<=MULTILAYER; i++)
	{
		if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
		{
			cEDA_pcbpolygon.m_nLayer = i;
			break;
		}
	}
	cEDA_pcbpolygon.m_fMinPrimLength = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditMinLength);
	cEDA_pcbpolygon.m_bPrimitiveLocked = dlg.m_bCheckPrimitiveLock;
	cEDA_pcbpolygon.m_bLocked = dlg.m_bCheckLocked;
	cEDA_pcbpolygon.m_bIgnoreViolations = dlg.m_bCheckIgnoreViolation;

	
	for(int i=0; i<dlg.arrayStringNet.GetCount(); i++)
	{
		if(dlg.arrayStringNet.GetAt(i) == dlg.m_csComboNet)
		{
			cEDA_pcbpolygon.m_nNet = i-1;
		}
	}
	cEDA_pcbpolygon.m_nPourOverStyle = dlg.m_nComboPourStyle;
	cEDA_pcbpolygon.m_bRemoveDead = dlg.m_bCheckRemoveDead;


	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	Write_Help(szPlaceRegion);

	int layer = m_nPCB_Current_Layer;
	//COLORREF color = (~nEDA_PCB_LayerColor[m_nPCB_Current_Layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
	COLORREF color = nEDA_PCB_LayerColor[m_nPCB_Current_Layer];
	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	COb_pcbregion aregion;
	aregion.pDocument = pDoc;
	aregion.m_nLayer = layer;
	aregion.m_bPolygonCutout = TRUE;

	
	//bEDA_NeedElectricalGrid = TRUE;
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
					aregion.FrameRegion(&dc, this, 1, color);
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
							aregion.FrameRegion(&dc, this, 1, color);
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
						aregion.FrameRegion(&dc, this, 1, color);
						OpenMouseCursor();
					}
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					//ElecHotPointsControl(&dc);
					if((fx1==fx3)&&(fy1==fy3)) continue;

					CloseMouseCursor();
					aregion.FrameRegion(&dc, this, 1, color);
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

					aregion.FrameRegion(&dc, this, 1, color);

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
		COb_pcbpolygon* ppolygon = new COb_pcbpolygon();
		ppolygon->Copy(&cEDA_pcbpolygon);
		if(ppolygon->m_fPourGap < pDoc->m_fRulesClearance) ppolygon->m_fPourGap = pDoc->m_fRulesClearance;
		ppolygon->pDocument = pDoc;
		ppolygon->m_nPolygonType = COPPER_POLYGON;
		ppolygon->m_nLayer = layer;

		ppolygon->m_nNet = cEDA_pcbpolygon.m_nNet;
		for(int i=0; i<aregion.m_cVertex.GetCount(); i++)
		{
			vertex = aregion.m_cVertex.GetAt(i);
			ppolygon->m_cVertex.Add(vertex);
		}
		int newindex = pDoc->m_arrayPcbPolygon.Add(ppolygon);
		
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		
		if(CreatePolygonPour(ppolygon, newindex) == TRUE) 
		{
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_POLYGON;
			pundo->m_nIndex = newindex;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

			pDoc->m_nPCB_Undo_Num++;
			if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}
		else 
		{
			pDoc->m_arrayPcbPolygon.RemoveAt(newindex);
			delete ppolygon;
		}
	}

	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	Write_Help(szEDA_Help_Ready);

	
}

void CESDView::OnPcbPlaceCoordinate()
{
	int ch1;
	double x1,y1;
	int xs,ys;
	int drawmode;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	char szPlaceJunc[] = "放置坐标标记: <左键><回车>放置, <右键><ESC>结束命令, <TAB>定义属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	MoveCursorToCenter();

	drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
	Get_VPort_XY(&xs,&ys,x1,y1);
	Write_Help(szPlaceJunc);
	
	int layer = m_nPCB_Current_Layer;
	//COLORREF color = (~nEDA_PCB_LayerColor[layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
	COLORREF color = nEDA_PCB_LayerColor[layer];

	COb_pcbcoordinate acoordinate;
	acoordinate.Copy(&cEDA_pcbcoordinate);
	acoordinate.pDocument = pDoc;
	acoordinate.m_nLayer = layer;
	acoordinate.m_fX = x1;
	acoordinate.m_fY = y1;

	acoordinate.Draw(&dc, this, 1);
	nEDA_RedrawFlag = 0;
	
	//bEDA_NeedElectricalGrid = TRUE;
	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
		{
			acoordinate.Draw(&dc, this, 1);
		}
		else if(ch1 == MOUSEMOVE)
		{
			if(nEDA_RedrawFlag == 1){ nEDA_RedrawFlag = 0;}
			else  
			{ 
				acoordinate.Draw(&dc, this, 1);
			}
			
			x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
			acoordinate.m_fX = x1;
			acoordinate.m_fY = y1;
			acoordinate.Draw(&dc, this, 1);
	   }
       else if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
			//if(Exist_Junction(x1,y1) == TRUE) continue;
			acoordinate.Draw(&dc, this, 1);

			COb_pcbcoordinate* pcoordinate = new COb_pcbcoordinate();
			pcoordinate->Copy(&acoordinate);
			int index = pDoc->m_arrayPcbCoordinate.Add(pcoordinate);

			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_COORDINATE;
			pundo->m_nIndex = index;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			

			dc.SetROP2(drawmode);
			CloseMouseCursor();
			pcoordinate->Draw(&dc, this, 0);
			OpenMouseCursor();
			dc.SetROP2(R2_NOTXORPEN);

			acoordinate.Draw(&dc, this, 1);

			continue;
		}
		else if(ch1 == TAB)
		{
			acoordinate.Draw(&dc, this, 1);
			
			CDlg_PCBEditCoordinate dlg;

			
			for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
			{
				dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
			}
			for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
			{
				if(layer == pDoc->m_arrayPcbLayerShow.GetAt(i))
				{
					dlg.m_nComboLayer = i;
					break;
				}
			}
			
			dlg.m_csEditLineWidth = pDoc->Convert_XY_To_String(cEDA_pcbcoordinate.m_fLineWidth);
			dlg.m_csEditSize = pDoc->Convert_XY_To_String(cEDA_pcbcoordinate.m_fSize);
			dlg.m_csEditTextHeight = pDoc->Convert_XY_To_String(cEDA_pcbcoordinate.m_fTextHeight);
			dlg.m_csEditTextLineWidth = pDoc->Convert_XY_To_String(cEDA_pcbcoordinate.m_fTextWidth);

			dlg.m_nComboStyle = cEDA_pcbcoordinate.m_nUnitStyle;
			dlg.m_bCheckLocked = cEDA_pcbcoordinate.m_bLocked;
			
			
			for(int i=0; i<arrayEDA_SystemFont.GetCount(); i++)
			{
				if(cEDA_pcbcoordinate.m_csTTFontName == arrayEDA_SystemFont.GetAt(i)) 
				{
					dlg.nTTFont = i;
					break;
				}
			}
			dlg.nStrokeFont = cEDA_pcbcoordinate.m_nStrokeFont;

			
			if(cEDA_pcbcoordinate.m_bUseTTFont == TRUE)
			{
				dlg.m_nRadioTrueType = 0;
				dlg.m_nComboFont = dlg.nTTFont;
			}
			else 
			{
				dlg.m_nRadioTrueType = 1;
				dlg.m_nComboFont = dlg.nStrokeFont;
			}

			
			dlg.m_bCheckBold = cEDA_pcbcoordinate.m_bBold;
			dlg.m_bCheckItalic = cEDA_pcbcoordinate.m_bItalic;
			
			if(dlg.DoModal() == IDOK)
			{
				
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
					{
						layer = i;
						break;
					}
				}
				
				cEDA_pcbcoordinate.m_fLineWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditLineWidth);
				cEDA_pcbcoordinate.m_fSize = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditSize);
				cEDA_pcbcoordinate.m_fTextHeight = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTextHeight);
				cEDA_pcbcoordinate.m_fTextWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTextLineWidth);

				cEDA_pcbcoordinate.m_nUnitStyle = dlg.m_nComboStyle;
				cEDA_pcbcoordinate.m_bLocked = dlg.m_bCheckLocked;
				
				
				if(dlg.m_nRadioTrueType == 0)
				{
					cEDA_pcbcoordinate.m_bUseTTFont = TRUE;
					cEDA_pcbcoordinate.m_csTTFontName = arrayEDA_SystemFont.GetAt(dlg.m_nComboFont);
				}
				else
				{
					cEDA_pcbcoordinate.m_bUseTTFont = FALSE;
					cEDA_pcbcoordinate.m_nStrokeFont = dlg.m_nComboFont;
				}

				
				cEDA_pcbcoordinate.m_bBold = dlg.m_bCheckBold;
				cEDA_pcbcoordinate.m_bItalic = dlg.m_bCheckItalic;

				acoordinate.Copy(&cEDA_pcbcoordinate);
				acoordinate.pDocument = pDoc;
				acoordinate.m_nLayer = layer;
				acoordinate.m_fX = x1;
				acoordinate.m_fY = y1;
			}		
		   
			acoordinate.Draw(&dc, this, 1);
	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	
	//bEDA_NeedElectricalGrid = FALSE;
	//m_fpHotPoint1.fx = m_fpHotPoint1.fy = m_fpHotPoint2.fx = m_fpHotPoint2.fy = 0;
}

void CESDView::OnPcbPlaceDimension()
{
	int ch1;
	double x1,y1,x2,y2;
	int xs,ys;
	int drawmode;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	char szPlaceJunc[] = "放置尺寸标记: <左键><回车>放置, <右键><ESC>结束命令, <TAB>定义属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	MoveCursorToCenter();

	drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	Write_Help(szPlaceJunc);

	int layer = m_nPCB_Current_Layer;
	//COLORREF color = (~nEDA_PCB_LayerColor[layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
	COLORREF color = nEDA_PCB_LayerColor[layer];

	COb_pcbdimension adimension;
	adimension.Copy(&cEDA_pcbdimension);
	adimension.pDocument = pDoc;
	adimension.m_nLayer = layer;
	adimension.m_nDimensionKind = 8;

PLACE_DIMENSION_8:
	
	//bEDA_NeedElectricalGrid = TRUE;
	do
	{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
			adimension.m_fXs = adimension.m_fXe = x1;
			adimension.m_fYs = adimension.m_fYe = y1;
			adimension.Draw(&dc, this, 1);
			nEDA_RedrawFlag = 0;
			do
			{
				ch1 = MouseKey();
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
				{
					adimension.Draw(&dc, this, 1);
				}
				else if(ch1 == MOUSEMOVE)
				{
					if(nEDA_RedrawFlag == 1){ nEDA_RedrawFlag = 0;}
					else  
					{ 
						adimension.Draw(&dc, this, 1);
					}
					
					x2 = m_fCurrentSnapX; y2 = m_fCurrentSnapY;
					if(x1 > x2)
					{
						adimension.m_fXs = x2;
						adimension.m_fYs = y2;
						adimension.m_fXe = x1;
						adimension.m_fYe = y1;
					}
					else 
					{
						adimension.m_fXs = x1;
						adimension.m_fYs = y1;
						adimension.m_fXe = x2;
						adimension.m_fYe = y2;
					}

					adimension.UpdateDimensionText(this);
					adimension.Draw(&dc, this, 1);
				}
				else if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
				{
					//if(Exist_Junction(x1,y1) == TRUE) continue;
					adimension.Draw(&dc, this, 1);

					COb_pcbdimension* pdimension = new COb_pcbdimension();
					pdimension->Copy(&adimension);
					pdimension->pDocument = pDoc;
					int index = pDoc->m_arrayPcbDimension.Add(pdimension);

					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_DIMENSION;
					pundo->m_nIndex = index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					

					dc.SetROP2(drawmode);
					CloseMouseCursor();
					pdimension->Draw(&dc, this, 0);
					OpenMouseCursor();
					dc.SetROP2(R2_NOTXORPEN);

					goto PLACE_DIMENSION_8;
				}
				else if(ch1 == TAB)
				{
					adimension.Draw(&dc, this, 1);
					CDlg_PCBEditDimension dlg;

					
					for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
					{
						dlg.arrayString.Add(pDoc->m_sPCB_LayerParam[pDoc->m_arrayPcbLayerShow.GetAt(i)].csLayerName);
					}
					for(int i=0; i<pDoc->m_arrayPcbLayerShow.GetCount(); i++)
					{
						if(adimension.m_nLayer == pDoc->m_arrayPcbLayerShow.GetAt(i))
						{
							dlg.m_nComboLayer = i;
							break;
						}
					}

					dlg.m_csEditLineHeight = pDoc->Convert_XY_To_String(adimension.m_fHeight);
					dlg.m_csEditLineWidth = pDoc->Convert_XY_To_String(adimension.m_fLineWidth);
					dlg.m_csEditTextHeight = pDoc->Convert_XY_To_String(adimension.m_fTextHeight);
					dlg.m_csEditTextWidth = pDoc->Convert_XY_To_String(adimension.m_fTextLineWidth);

					dlg.m_nComboStyle = adimension.m_nUnitStyle;
					dlg.m_bCheckLocked = adimension.m_bLocked;
					
					
					for(int i=0; i<arrayEDA_SystemFont.GetCount(); i++)
					{
						if(adimension.m_csTTFontName == arrayEDA_SystemFont.GetAt(i)) 
						{
							dlg.nTTFont = i;
							break;
						}
					}
					dlg.nStrokeFont = adimension.m_nStrokeFont;

					
					if(adimension.m_bUseTTFont == TRUE)
					{
						dlg.m_nRadioTrueType = 0;
						dlg.m_nComboFont = dlg.nTTFont;
					}
					else 
					{
						dlg.m_nRadioTrueType = 1;
						dlg.m_nComboFont = dlg.nStrokeFont;
					}

					
					dlg.m_bCheckBold = adimension.m_bBold;
					dlg.m_bCheckItalic = adimension.m_bItalic;
					
					if(dlg.DoModal() == IDOK)
					{
						
						for(int i=0; i<=MULTILAYER; i++)
						{
							if(dlg.arrayString.GetAt(dlg.m_nComboLayer) == pDoc->m_sPCB_LayerParam[i].csLayerName)
							{
								adimension.m_nLayer = i;
								break;
							}
						}

						adimension.m_fHeight = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditLineHeight);
						adimension.m_fLineWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditLineWidth);
						adimension.m_fTextHeight = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTextHeight);
						adimension.m_fTextLineWidth = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditTextWidth);

						adimension.m_nUnitStyle = dlg.m_nComboStyle;
						adimension.m_bLocked = dlg.m_bCheckLocked;
						
						
						if(dlg.m_nRadioTrueType == 0)
						{
							adimension.m_bUseTTFont = TRUE;
							adimension.m_csTTFontName = arrayEDA_SystemFont.GetAt(dlg.m_nComboFont);
						}
						else
						{
							adimension.m_bUseTTFont = FALSE;
							adimension.m_nStrokeFont = dlg.m_nComboFont;
						}

						
						adimension.m_bBold = dlg.m_bCheckBold;
						adimension.m_bItalic = dlg.m_bCheckItalic;

						adimension.UpdateDimensionText(this);

						cEDA_pcbdimension.m_fHeight = adimension.m_fHeight;
						cEDA_pcbdimension.m_fLineWidth = adimension.m_fLineWidth;
						cEDA_pcbdimension.m_fTextHeight = adimension.m_fTextHeight;
						cEDA_pcbdimension.m_fTextLineWidth = adimension.m_fTextLineWidth;
						cEDA_pcbdimension.m_nUnitStyle = adimension.m_nUnitStyle;
						cEDA_pcbdimension.m_bUseTTFont = adimension.m_bUseTTFont;
						cEDA_pcbdimension.m_csTTFontName = adimension.m_csTTFontName;
						cEDA_pcbdimension.m_nStrokeFont = adimension.m_nStrokeFont;
						cEDA_pcbdimension.m_bBold = adimension.m_bBold;
						cEDA_pcbdimension.m_bItalic = adimension.m_bItalic;
					}					
				   
					adimension.Draw(&dc, this, 1);
				}
			}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	if(m_bCursorState == TRUE) CloseMouseCursor();
	
	ClipCursor(NULL);  m_bClipState = FALSE;

	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	
	//bEDA_NeedElectricalGrid = FALSE;
	//m_fpHotPoint1.fx = m_fpHotPoint1.fy = m_fpHotPoint2.fx = m_fpHotPoint2.fy = 0;
}

void CESDView::OnPcbPlaceComp()
{
	CString csDesignator;
	CString csComment;
	CString csDescription;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	Struct_CompList complist;

	CDlg_PlacePackage dlg;

	int nReturn = dlg.DoModal();
	if((nReturn != IDOK) && (nReturn != 101)) return;

	if(nReturn == IDOK)
	{
		complist = dlg.m_PackageList;
		csDesignator = dlg.m_csEditDesignator;
		csComment = dlg.m_csEditComment;
	}
	else if(nReturn == 101)
	{
		CDlg_PcbLib_Browse dlg2;
		dlg2.m_bMatchMore = bEDA_PcbMatchMore;
		
		if(dlg2.DoModal() != IDOK) return;

		bEDA_PcbMatchMore = dlg2.m_bMatchMore;
		complist = dlg2.m_CompList;
		if(complist.liboffset < 0) return;

		
		Struct_CompList temp_complist;
		int i;
		for(i=0; i<arrayEDA_PcbCompHistory.GetCount(); i++)
		{
			temp_complist = arrayEDA_PcbCompHistory.GetAt(i);
			CString str1 = temp_complist.compname;
			str1.MakeUpper();
			CString str2 = complist.compname;
			str2.MakeUpper();
			if(str1 == str2) break;
		}
		if(i >= arrayEDA_PcbCompHistory.GetCount()) 
		{
			arrayEDA_PcbCompHistory.InsertAt(0,complist);
			csEDA_PcbCompHistory = complist.compname;
		}

		csDesignator.Empty();
		csComment = complist.compname;
	}

	

	int offset, length;
	unsigned char* pbuffer;
	
	if(complist.basiclibrary == TRUE)  
	{
		
		offset = complist.liboffset + complist.offset;
		length = complist.length;
		
		
		cfileEDA_PcbLibrary.Seek(complist.liboffset, CFile::begin);
		Struct_Library_Head libhead;
		cfileEDA_PcbLibrary.Read(&libhead, 32);
		for(int i=0; i<libhead.totalcompnum; i++)
		{
			cfileEDA_PcbLibrary.Seek(complist.liboffset + 32 + 320*i, CFile::begin);
			Struct_Comp_Head comp_head;
			cfileEDA_PcbLibrary.Read(&comp_head, 320);
			CString cstring = comp_head.name;
			if(strcmp(comp_head.name, complist.compname) == 0)
			{
				csDescription = comp_head.description;
				break;
			}
		}

		pbuffer = (unsigned char*)malloc(length + 100);

		cfileEDA_PcbLibrary.Seek(offset, CFile::begin);
		cfileEDA_PcbLibrary.Read(pbuffer, length);
	}
	else
	{
		
		
		CString userlibrary, compname;
		userlibrary = arrayEDA_PcbUserLibraryName.GetAt(complist.liboffset);
		
		char libstring[MAX_PATH];
		sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, userlibrary);

		CFile cfileUserLibrary;
		if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE)  return;
		CArchive ar(&cfileUserLibrary, CArchive::load);
		
		CString headformat;
		ar >> headformat;
		if(headformat != PCB_Library_Head)	{	ar.Close();	cfileUserLibrary.Close();	return;	}
		int totalnum;
		ar >> totalnum;
		ar.Close();

		
		cfileUserLibrary.Seek(32 + 320*complist.offset, CFile::begin);

		Struct_Comp_Head comp_head;
		cfileUserLibrary.Read(&comp_head, 320);
		csDescription = comp_head.description;
		offset = comp_head.offset;
		length = comp_head.length;

		pbuffer = (unsigned char*)malloc(length + 100); 

		cfileUserLibrary.Seek(offset, CFile::begin);
		cfileUserLibrary.Read(pbuffer, length);

		cfileUserLibrary.Close();
	}

	CMemFile memfile(pbuffer, length);
	CArchive ar(&memfile,CArchive::load);
	
		
	
	COb_pcbcomp* pcomp = new COb_pcbcomp();
	pcomp->pDocument = pDoc;
	pcomp->m_fX = 0;
	pcomp->m_fY = 0;
	pcomp->m_fRotation = 0;
	pcomp->m_fHeight = 50;

	pcomp->m_nLayer = TOPLAYER;

	pcomp->m_bNameOn = cEDA_pcbcomp.m_bNameOn;
	//pcomp->m_nNamePosition = cEDA_pcbcomp.m_nNamePosition;
	pcomp->m_nNamePosition = LEFTTOP;
	pcomp->m_bCommentOn = cEDA_pcbcomp.m_bCommentOn;
	//pcomp->m_nCommentPosition = cEDA_pcbcomp.m_nCommentPosition;
	pcomp->m_nCommentPosition = LEFTTOP;

	pcomp->m_csPattern = complist.compname;
	pcomp->m_csFootprintDescription = csDescription;

	
	int index = pDoc->m_arrayPcbComp.Add(pcomp);

	int arcnum, tracknum, fillnum, padnum, vianum, textnum, regionnum;

	ar >> arcnum;
	for(int i=0; i<arcnum; i++)
	{
		COb_pcbarc* parc = new COb_pcbarc();
		parc->Serialize(ar);
		parc->pDocument = pDoc;
		parc->m_nComponent = index;
		pDoc->m_arrayPcbArc.Add(parc);
	}
	ar >> tracknum;
	for(int i=0; i<tracknum; i++)
	{
		COb_pcbtrack* ptrack = new COb_pcbtrack();
		ptrack->Serialize(ar);
		ptrack->pDocument = pDoc;
		ptrack->m_nComponent = index;
		pDoc->m_arrayPcbTrack.Add(ptrack);
	}
	ar >> fillnum;
	for(int i=0; i<fillnum; i++)
	{
		COb_pcbfill* pfill = new COb_pcbfill();
		pfill->Serialize(ar);
		pfill->pDocument = pDoc;
		pfill->m_nComponent = index;
		pDoc->m_arrayPcbFill.Add(pfill);
	}
	ar >> padnum;
	for(int i=0; i<padnum; i++)
	{
		COb_pcbpad* ppad = new COb_pcbpad();
		ppad->Serialize(ar);
		ppad->pDocument = pDoc;
		ppad->m_nComponent = index;
		pDoc->m_arrayPcbPad.Add(ppad);
	}
	ar >> vianum;
	for(int i=0; i<vianum; i++)
	{
		COb_pcbvia* pvia = new COb_pcbvia();
		pvia->Serialize(ar);
		pvia->pDocument = pDoc;
		pvia->m_nComponent = index;
		pDoc->m_arrayPcbVia.Add(pvia);
	}
	ar >> textnum;
	for(int i=0; i<textnum; i++)
	{
		COb_pcbtext* ptext = new COb_pcbtext();
		ptext->Serialize(ar);
		ptext->pDocument = pDoc;
		ptext->m_nComponent = index;
		pDoc->m_arrayPcbText.Add(ptext);
	}
	ar >> regionnum;
	for(int i=0; i<regionnum; i++)
	{
		COb_pcbregion* pregion = new COb_pcbregion();
		pregion->Serialize(ar);
		pregion->pDocument = pDoc;
		pregion->m_nComponent = index;
		pDoc->m_arrayPcbRegion.Add(pregion);
	}

	ar.Close();
	memfile.Close();
	free(pbuffer);

	
	COb_pcbtext* ptext = new COb_pcbtext();
	ptext->pDocument = pDoc;
	ptext->m_nComponent = index;
	ptext->m_nLayer = TOPOVERLAY;
	ptext->m_fSx = pcomp->m_fX;
	ptext->m_fSy = pcomp->m_fY + 100;
	ptext->m_bComment = FALSE;
	ptext->m_bDesignator = TRUE;

	ptext->m_fRotation = cEDA_pcbcomp_designator.m_fRotation;
	if(csDesignator.IsEmpty() == TRUE) ptext->m_csText = cEDA_pcbcomp_designator.m_csText;
	else ptext->m_csText = csDesignator;
	ptext->m_fHeight = cEDA_pcbcomp_designator.m_fHeight;
	ptext->m_fWidth = cEDA_pcbcomp_designator.m_fWidth;
	ptext->m_bMirror = cEDA_pcbcomp_designator.m_bMirror;
	ptext->m_nStrokeFont = cEDA_pcbcomp_designator.m_nStrokeFont;
	ptext->m_bUseTTFont = cEDA_pcbcomp_designator.m_bUseTTFont;
	ptext->m_bBold = cEDA_pcbcomp_designator.m_bBold;
	ptext->m_bItalic = cEDA_pcbcomp_designator.m_bItalic;
	ptext->m_bInverted = cEDA_pcbcomp_designator.m_bInverted;
	ptext->m_csTTFontName = cEDA_pcbcomp_designator.m_csTTFontName;
	ptext->m_fInvertedBorder = cEDA_pcbcomp_designator.m_fInvertedBorder;

	pDoc->m_arrayPcbText.Add(ptext);

	
	ptext = new COb_pcbtext();
	ptext->pDocument = pDoc;
	ptext->m_nComponent = index;
	ptext->m_nLayer = TOPOVERLAY;
	ptext->m_fSx = pcomp->m_fX;
	ptext->m_fSy = pcomp->m_fY;
	ptext->m_bComment = TRUE;
	ptext->m_bDesignator = FALSE;

	ptext->m_fRotation = cEDA_pcbcomp_comment.m_fRotation;
	if(csComment.IsEmpty() == TRUE) ptext->m_csText = cEDA_pcbcomp_comment.m_csText;
	else ptext->m_csText = csComment;
	ptext->m_fHeight = cEDA_pcbcomp_comment.m_fHeight;
	ptext->m_fWidth = cEDA_pcbcomp_comment.m_fWidth;
	ptext->m_bMirror = cEDA_pcbcomp_comment.m_bMirror;
	ptext->m_nStrokeFont = cEDA_pcbcomp_comment.m_nStrokeFont;
	ptext->m_bUseTTFont = cEDA_pcbcomp_comment.m_bUseTTFont;
	ptext->m_bBold = cEDA_pcbcomp_comment.m_bBold;
	ptext->m_bItalic = cEDA_pcbcomp_comment.m_bItalic;
	ptext->m_bInverted = cEDA_pcbcomp_comment.m_bInverted;
	ptext->m_csTTFontName = cEDA_pcbcomp_comment.m_csTTFontName;
	ptext->m_fInvertedBorder = cEDA_pcbcomp_comment.m_fInvertedBorder;

	pDoc->m_arrayPcbText.Add(ptext);
		
	MoveCursorToCenter();
	if(pcomp->MovePlacePackage(&dc, this, index) == FALSE)
	{
		
		pDoc->m_arrayPcbComp.RemoveAt(index);
		delete pcomp;
		for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
			if(parc->m_nComponent == index)
			{
				delete parc;
				pDoc->m_arrayPcbArc.RemoveAt(i);
				i--;
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
			if(ptrack->m_nComponent == index)
			{
				delete ptrack;
				pDoc->m_arrayPcbTrack.RemoveAt(i);
				i--;
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
			if(ppad->m_nComponent == index)
			{
				delete ppad;
				pDoc->m_arrayPcbPad.RemoveAt(i);
				i--;
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
			if(pfill->m_nComponent == index)
			{
				delete pfill;
				pDoc->m_arrayPcbFill.RemoveAt(i);
				i--;
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
			if(ptext->m_nComponent == index)
			{
				delete ptext;
				pDoc->m_arrayPcbText.RemoveAt(i);
				i--;
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
			if(pvia->m_nComponent == index)
			{
				delete pvia;
				pDoc->m_arrayPcbVia.RemoveAt(i);
				i--;
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
			if(pregion->m_nComponent == index)
			{
				delete pregion;
				pDoc->m_arrayPcbRegion.RemoveAt(i);
				i--;
			}
		}
	}
	else
	{
		
		
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_DELETE;
		pundo->m_nOb = PCB_ELEMENT_COMP;
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		

		
		for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
			if(parc->m_nComponent == index)
			{
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = PCB_ELEMENT_ARC;
				pundo->m_nIndex = i;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
			if(ptrack->m_nComponent == index)
			{
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = PCB_ELEMENT_TRACK;
				pundo->m_nIndex = i;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
			if(ppad->m_nComponent == index)
			{
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = PCB_ELEMENT_PAD;
				pundo->m_nIndex = i;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
			if(pfill->m_nComponent == index)
			{
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = PCB_ELEMENT_FILL;
				pundo->m_nIndex = i;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
			if(ptext->m_nComponent == index)
			{
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = PCB_ELEMENT_TEXT;
				pundo->m_nIndex = i;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
			if(pvia->m_nComponent == index)
			{
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = PCB_ELEMENT_VIA;
				pundo->m_nIndex = i;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
			if(pregion->m_nComponent == index)
			{
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = PCB_ELEMENT_REGION;
				pundo->m_nIndex = i;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
		}
		
		
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Invalidate();
}

void CESDView::OnPcbEditRepeat()
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	MoveCursorToCenter();

	
	unsigned ch1;
	char szHelpText[] = "重复放置: <左键><回车>选择对象, <右键><ESC>结束命令";

	Write_Help(szHelpText);
	do
	{
       ch1 = MouseKey();
       if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
			Struct_Pcb_Multi_Selection multi_selection = SelectPcbObject(m_fCurrentX, m_fCurrentY, NOT_INCLUDE_CPAD);
			int element = multi_selection.pcbelement;
			int index = multi_selection.index;
			if((element == 0)||( index < 0)) continue;

			CDlg_Edit_Repeat dlg;
			if(element == PCB_ELEMENT_COMP) dlg.m_csTitleString = "复制元件";
			else if(element == PCB_ELEMENT_TEXT) 
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(index);
				if(ptext->m_nComponent >= 0) continue;
				
				dlg.m_csTitleString = "复制文字";
			}
			else if(element == PCB_ELEMENT_TRACK) dlg.m_csTitleString = "复制连线";
			else if(element == PCB_ELEMENT_ARC) dlg.m_csTitleString = "复制圆弧";
			else if(element == PCB_ELEMENT_PAD) dlg.m_csTitleString = "复制焊盘";
			else if(element == PCB_ELEMENT_VIA) dlg.m_csTitleString = "复制过孔";
			else if(element == PCB_ELEMENT_FILL) dlg.m_csTitleString = "复制矩形填充区";
			else if(element == PCB_ELEMENT_REGION) dlg.m_csTitleString = "复制区域";
			else continue;

			dlg.m_nEditNumber = nEDA_PcbRepeatNumber;
			dlg.m_nEditNDelta = nEDA_PcbRepeatNumberDelta;
			dlg.m_csEditXDelta = pDoc->Convert_XY_To_String(fEDA_PcbRepeatDeltaX);
			dlg.m_csEditYDelta = pDoc->Convert_XY_To_String(fEDA_PcbRepeatDeltaY);
			if(dlg.DoModal() == IDOK)
			{
				double deltax, deltay;
				int number, ndelta;
				fEDA_PcbRepeatDeltaX = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditXDelta);
				fEDA_PcbRepeatDeltaY = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditYDelta);
				nEDA_PcbRepeatNumber = dlg.m_nEditNumber;
				nEDA_PcbRepeatNumberDelta = dlg.m_nEditNDelta;
				
				deltax = fEDA_PcbRepeatDeltaX;
				deltay = fEDA_PcbRepeatDeltaY;
				number = nEDA_PcbRepeatNumber;
				ndelta = nEDA_PcbRepeatNumberDelta;

				if(element == PCB_ELEMENT_COMP)
				{
					COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(index);
					for(int n=1; n<=number; n++)
					{
						COb_pcbcomp* pnewcomp = new COb_pcbcomp();
						pnewcomp->Copy(pcomp);
						pnewcomp->m_bSelection = FALSE;
						int newindex = pDoc->m_arrayPcbComp.Add(pnewcomp);
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;

						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_COMP;
						pundo->m_nIndex = newindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
						
						pnewcomp->m_fX += n*deltax;
						pnewcomp->m_fY += n*deltay;

						
						for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
						{
							COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
							if((ptrack->m_nFlag == FLAG_NORMAL)&&(ptrack->m_nComponent == index))
							{
								COb_pcbtrack* pnewtrack = new COb_pcbtrack();

								pnewtrack->Copy(ptrack);
								pnewtrack->m_bSelection = FALSE;
								pnewtrack->m_nComponent = newindex;
								pnewtrack->m_fX1 += n*deltax;
								pnewtrack->m_fY1 += n*deltay;
								pnewtrack->m_fX2 += n*deltax;
								pnewtrack->m_fY2 += n*deltay;

								int subindex = pDoc->m_arrayPcbTrack.Add(pnewtrack);
								
								CPcbUndo *pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_TRACK;
								pundo->m_nIndex = subindex;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								
							}
						}
						for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
						{
							COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
							if((parc->m_nFlag == FLAG_NORMAL)&&(parc->m_nComponent == index))
							{
								COb_pcbarc* pnewarc = new COb_pcbarc();

								pnewarc->Copy(parc);
								pnewarc->m_bSelection = FALSE;
								pnewarc->m_nComponent = newindex;
								pnewarc->m_fCx += n*deltax;
								pnewarc->m_fCy += n*deltay;

								int subindex = pDoc->m_arrayPcbArc.Add(pnewarc);
								
								CPcbUndo *pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_ARC;
								pundo->m_nIndex = subindex;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								
							}
						}
						for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
						{
							COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
							if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == index))
							{
								COb_pcbpad* pnewpad = new COb_pcbpad();

								pnewpad->Copy(ppad);
								pnewpad->m_bSelection = FALSE;
								pnewpad->m_nComponent = newindex;
								pnewpad->m_fCx += n*deltax;
								pnewpad->m_fCy += n*deltay;

								int subindex = pDoc->m_arrayPcbPad.Add(pnewpad);
								
								CPcbUndo *pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_PAD;
								pundo->m_nIndex = subindex;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								
							}
						}
						for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
						{
							COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
							if((pvia->m_nFlag == FLAG_NORMAL)&&(pvia->m_nComponent == index))
							{
								COb_pcbvia* pnewvia = new COb_pcbvia();

								pnewvia->Copy(pvia);
								pnewvia->m_bSelection = FALSE;
								pnewvia->m_nComponent = newindex;
								pnewvia->m_fCx += n*deltax;
								pnewvia->m_fCy += n*deltay;

								int subindex = pDoc->m_arrayPcbVia.Add(pnewvia);
								
								CPcbUndo *pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_VIA;
								pundo->m_nIndex = subindex;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								
							}
						}
						for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
						{
							COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
							if((pfill->m_nFlag == FLAG_NORMAL)&&(pfill->m_nComponent == index))
							{
								COb_pcbfill* pnewfill = new COb_pcbfill();

								pnewfill->Copy(pfill);
								pnewfill->m_bSelection = FALSE;
								pnewfill->m_nComponent = newindex;
								pnewfill->m_fX1 += n*deltax;
								pnewfill->m_fY1 += n*deltay;
								pnewfill->m_fX2 += n*deltax;
								pnewfill->m_fY2 += n*deltay;

								int subindex = pDoc->m_arrayPcbFill.Add(pnewfill);
								
								CPcbUndo *pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_FILL;
								pundo->m_nIndex = subindex;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								
							}
						}
						for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
						{
							COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
							if((ptext->m_nFlag == FLAG_NORMAL)&&(ptext->m_nComponent == index))
							{
								COb_pcbtext* pnewtext = new COb_pcbtext();

								pnewtext->Copy(ptext);
								pnewtext->m_bSelection = FALSE;
								pnewtext->m_nComponent = newindex;
								pnewtext->m_fSx += n*deltax;
								pnewtext->m_fSy += n*deltay;

								int subindex = pDoc->m_arrayPcbText.Add(pnewtext);
								
								CPcbUndo *pundo = new CPcbUndo;
								pundo->m_nOp = UNDO_DELETE;
								pundo->m_nOb = PCB_ELEMENT_TEXT;
								pundo->m_nIndex = subindex;
								pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
								
							}
						}
					}
				}
				else if(element == PCB_ELEMENT_TEXT)
				{
					COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(index);
					for(int n=1; n<=number; n++)
					{						
						COb_pcbtext* pnewtext = new COb_pcbtext();
						pnewtext->Copy(ptext);
						pnewtext->m_bSelection = FALSE;
						pnewtext->m_fSx += n*deltax;
						pnewtext->m_fSy += n*deltay;

						int newindex = pDoc->m_arrayPcbText.Add(pnewtext);

						
						int length = pnewtext->m_csText.GetLength();
						char c = (pnewtext->m_csText.GetAt(length - 1))&0x007f;
						if(isdigit(c) != 0)
						{
							int base = c - 48;
							base += n*ndelta;
							CString str;
							str.Format("%d", base);
							pnewtext->m_csText = pnewtext->m_csText.Left(length-1);
							pnewtext->m_csText += str;
						}
						pnewtext->UpdateStringBorder(this, TRUE);
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;

						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TEXT;
						pundo->m_nIndex = newindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					}
				}
				else if(element == PCB_ELEMENT_TRACK)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(index);
					for(int n=1; n<=number; n++)
					{						
						COb_pcbtrack* pnewtrack = new COb_pcbtrack();
						pnewtrack->Copy(ptrack);
						pnewtrack->m_bSelection = FALSE;
						pnewtrack->m_fX1 += n*deltax;
						pnewtrack->m_fY1 += n*deltay;
						pnewtrack->m_fX2 += n*deltax;
						pnewtrack->m_fY2 += n*deltay;

						int newindex = pDoc->m_arrayPcbTrack.Add(pnewtrack);
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;

						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = newindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					}
				}
				else if(element == PCB_ELEMENT_ARC)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(index);
					for(int n=1; n<=number; n++)
					{						
						COb_pcbarc* pnewarc = new COb_pcbarc();
						pnewarc->Copy(parc);
						pnewarc->m_bSelection = FALSE;
						pnewarc->m_fCx += n*deltax;
						pnewarc->m_fCy += n*deltay;

						int newindex = pDoc->m_arrayPcbArc.Add(pnewarc);
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;

						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_ARC;
						pundo->m_nIndex = newindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					}
				}
				else if(element == PCB_ELEMENT_PAD)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(index);
					for(int n=1; n<=number; n++)
					{						
						COb_pcbpad* pnewpad = new COb_pcbpad();
						pnewpad->Copy(ppad);
						pnewpad->m_bSelection = FALSE;
						pnewpad->m_fCx += n*deltax;
						pnewpad->m_fCy += n*deltay;

						int newindex = pDoc->m_arrayPcbPad.Add(pnewpad);
						
						int length = pnewpad->m_csName.GetLength();
						char c = (pnewpad->m_csName.GetAt(length - 1))&0x007f;
						if(isdigit(c) != 0)
						{
							int base = c - 48;
							base += n*ndelta;
							CString str;
							str.Format("%d", base);
							pnewpad->m_csName = pnewpad->m_csName.Left(length-1);
							pnewpad->m_csName += str;
						}
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;

						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_PAD;
						pundo->m_nIndex = newindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					}
				}
				else if(element == PCB_ELEMENT_VIA)
				{
					COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(index);
					for(int n=1; n<=number; n++)
					{						
						COb_pcbvia* pnewvia = new COb_pcbvia();
						pnewvia->Copy(pvia);
						pnewvia->m_bSelection = FALSE;
						pnewvia->m_fCx += n*deltax;
						pnewvia->m_fCy += n*deltay;

						int newindex = pDoc->m_arrayPcbVia.Add(pnewvia);
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;

						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_VIA;
						pundo->m_nIndex = newindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					}
				}
				else if(element == PCB_ELEMENT_FILL)
				{
					COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(index);
					for(int n=1; n<=number; n++)
					{						
						COb_pcbfill* pnewfill = new COb_pcbfill();
						pnewfill->Copy(pfill);
						pnewfill->m_bSelection = FALSE;
						pnewfill->m_fX1 += n*deltax;
						pnewfill->m_fY1 += n*deltay;
						pnewfill->m_fX2 += n*deltax;
						pnewfill->m_fY2 += n*deltay;

						int newindex = pDoc->m_arrayPcbFill.Add(pnewfill);
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;

						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_FILL;
						pundo->m_nIndex = newindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					}
				}
				else if(element == PCB_ELEMENT_REGION)
				{
					COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(index);
					for(int n=1; n<=number; n++)
					{						
						COb_pcbregion* pnewregion = new COb_pcbregion();
						pnewregion->Copy(pregion);
						pnewregion->m_bSelection = FALSE;
						for(int j=0; j<pnewregion->m_cVertex.GetCount(); j++)
						{
							Struct_RegionVertex vertex = pnewregion->m_cVertex.GetAt(j);
							vertex.fx += n*deltax;
							vertex.fy += n*deltay;
							if(vertex.radius != 0)
							{
								vertex.cx += n*deltax;
								vertex.cy += n*deltay;
							}
							pnewregion->m_cVertex.SetAt(j, vertex);
						}

						int newindex = pDoc->m_arrayPcbRegion.Add(pnewregion);
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;

						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_REGION;
						pundo->m_nIndex = newindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					}
				}
			}
			break;
	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
	
	
	if(undo_flag == TRUE)
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
}


