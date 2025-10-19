void CESDView::OnPlaceWire()
{
	int ch1;
	double x1,y1,x2,y2,x3,y3,tempx, tempy;
	int xs,ys,xe,ye,xe1,ye1,flag;
	int drawmode;
	char szPlaceLineStart[]  = "放置导线: <左键><回车>定义起点, <右键><ESC>结束命令";
	char szPlaceLineEnd[]  = "放置导线: <左键><回车>定义终点, <右键><ESC>返回上一步, <TAB>定义属性";
	char szInfo[6][100] = {	"<shift+空格><空格>修改模式: 90度-水平",	"<shift+空格><空格>修改模式: 90度-垂直",	"<shift+空格><空格>修改模式: 45度-水平垂直",\
							"<shift+空格><空格>修改模式: 45度-45度",	"<shift+空格><空格>修改模式: 任意角度",		"<shift+空格><空格>修改模式: 自动连线"	};
	
	POSITION line_pos;
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作
	
	COb_schline* pline = NULL;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	Write_Help(szPlaceLineStart);

	CPen *OverwritePen1,  *OverwritePen2, *oldpen;
	OverwritePen1 = new CPen(PS_SOLID,1,cEDA_schwire.m_nColor);
	OverwritePen2 = new CPen(PS_DOT, 1,cEDA_schwire.m_nColor);

	//开放Electricl Grid
	bEDA_NeedElectricalGrid = TRUE;

	flag = 1;
	nEDA_RedrawFlag = OFF;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			ElecHotPointsControl(&dc);

			x1 = m_fCurrentSnapX;
			y1 = m_fCurrentSnapY;
			x2 = x3 = x1; 
			y2 = y3 = y1;
			Get_VPort_XY(&xs,&ys,x1,y1);
			xe = xe1 = xs;
			ye = ye1 = ys;
			Write_Help(szPlaceLineEnd);
			Write_Info(szInfo[nEDA_SCH_PlaceWireMode]);
			do
			{
				ch1 = MouseKey();
LABEL_1:
				Get_VPort_XY(&xs,&ys,x1,y1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{ 
					ElecHotPointsControl(&dc);
					Write_Help(szPlaceLineStart);
					Write_Info(szEDA_Info_Ready);

					drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);
	
					dc.MoveTo(xs,ys);
					oldpen = dc.SelectObject(OverwritePen1);
					dc.LineTo(xe,ye);
					dc.SelectObject(OverwritePen2);
					dc.LineTo(xe1,ye1);

					dc.SelectObject(oldpen);
					dc.SetROP2(drawmode);

					ch1 = MOUSEMOVE;
					pline = NULL;	//再次绘制新线段
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
							//去掉画线的最后一个点---当前起点
							//if((m_fpHotPoint1.fx == x1)&&(m_fpHotPoint1.fy == y1))
							//{
							//	m_fpHotPoint1.fx = 0;
							//	m_fpHotPoint1.fy = 0;
							//}
							ElecHotPointsControl(&dc);
							drawmode = dc.GetROP2();
							dc.SetROP2(R2_NOTXORPEN);

							dc.MoveTo(xs,ys);
							oldpen = dc.SelectObject(OverwritePen1);
							dc.LineTo(xe,ye);
							dc.SelectObject(OverwritePen2);
							dc.LineTo(xe1,ye1);

							dc.SelectObject(oldpen);
							dc.SetROP2(drawmode);
						}
						x3 = x2 = m_fCurrentSnapX; y3 = y2 = m_fCurrentSnapY;
						Get_VPort_XY(&xe,&ye,x2,y2);
						xe1 = xe;  ye1 = ye;
						if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START)
						{
							if((x3 == x1)&&(y3 == y1)) {} //重合
							else if(x3 == x1){} //垂直
							else if(y3 == y1){}//水平
							else
							{
								x2 = x3;
								y2 = y1;
							}
							Get_VPort_XY(&xe,&ye,x2,y2);
							Get_VPort_XY(&xe1,&ye1,x3,y3);
						}
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_END)
						{
							if((x3 == x1)&&(y3 == y1)) {} //重合
							else if(x3 == x1){} //垂直
							else if(y3 == y1){}//水平
							else
							{
								x2 = x1;
								y2 = y3;
							}
							Get_VPort_XY(&xe,&ye,x2,y2);
							Get_VPort_XY(&xe1,&ye1,x3,y3);
						}
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_START)
						{
							if((x3 == x1)&&(y3 == y1)) {} //重合
							else if(x3 == x1){} //垂直
							else if(y3 == y1){}//水平
							else if(abs(x3-x1) == abs(y3-y1)) {}//45度
							else if((x3>x1)&&(y3>y1))  // I象限
							{  
								tempx=x3-x1;  tempy=y3-y1;
								if(tempx > tempy)
								{  
										x2 = x3 - tempy;
										y2 = y1;
								}
								else
								{ 
										y2 = y3 - tempx;
										x2 = x1;
								}
							}
							else if((x1>x3)&&(y3>y1))	//II象限
							{  
								tempx=x1-x3;  tempy=y3-y1;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										x2 = x3 + tempy;
										y2 = y1;
								}
								else
								{ 
										y2 = y3 - tempx;
										x2 = x1;
								}
							}
							else if((x3>x1)&&(y1>y3))	//IV象限
							{  
								tempx=x3-x1;  tempy=y1-y3;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										x2 = x3 - tempy;
										y2 = y1;
								}
								else
								{ 
										y2 = y3 + tempx;
										x2 = x1;
								}
							}
							else if((x1>x3)&&(y1>y3))	//III象限
							{  
								tempx=x1-x3;  tempy=y1-y3;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										x2 = x3 + tempy;
										y2 = y1;
								}
								else
								{ 
										y2 = y3 + tempx;
										x2 = x1;
								}
							}
							Get_VPort_XY(&xe,&ye,x2,y2);
							Get_VPort_XY(&xe1,&ye1,x3,y3);
						}
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_END)
						{
							if((x3 == x1)&&(y3 == y1)) {} //重合
							else if(x3 == x1){} //垂直
							else if(y3 == y1){}//水平
							else if(abs(x3-x1) == abs(y3-y1)) {}//45度
							else if((x3>x1)&&(y3>y1))  // I象限
							{  
								tempx=x3-x1;  tempy=y3-y1;
								if(tempx > tempy)
								{  
										x2 = x1 + tempy;
										y2 = y3;
								}
								else
								{ 
										y2 = y1 + tempx;
										x2 = x3;
								}
							}
							else if((x1>x3)&&(y3>y1))	//II象限
							{  
								tempx=x1-x3;  tempy=y3-y1;
								if(tempx > tempy)
								{  
										x2 = x1 - tempy;
										y2 = y3;
								}
								else
								{ 
										y2 = y1 + tempx;
										x2 = x3;
								}
							}
							else if((x3>x1)&&(y1>y3))	//IV象限
							{  
								tempx=x3-x1;  tempy=y1-y3;
								if(tempx > tempy)
								{  
										x2 = x1 + tempy;
										y2 = y3;
								}
								else
								{ 
										y2 = y1 - tempx;
										x2 = x3;
								}
							}
							else if((x1>x3)&&(y1>y3))	//III象限
							{  
								tempx=x1-x3;  tempy=y1-y3;
								if(tempx > tempy)
								{  
										x2 = x1 - tempy;
										y2 = y3;
								}
								else
								{ 
										y2 = y1 - tempx;
										x2 = x3;
								}
							}
							Get_VPort_XY(&xe,&ye,x2,y2);
							Get_VPort_XY(&xe1,&ye1,x3,y3);
						}
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_ANYANGLE)
						{

						}
						drawmode = dc.GetROP2();
						dc.SetROP2(R2_NOTXORPEN);
							
						dc.MoveTo(xs,ys);
						oldpen = dc.SelectObject(OverwritePen1);
						dc.LineTo(xe,ye);
						dc.SelectObject(OverwritePen2);
						dc.LineTo(xe1,ye1);

						dc.SelectObject(oldpen);
						dc.SetROP2(drawmode);
					}
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					ElecHotPointsControl(&dc);
					if((x1==x3)&&(y1==y3)) continue;

					drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);
						
					dc.MoveTo(xs,ys);
					oldpen = dc.SelectObject(OverwritePen1);
					dc.LineTo(xe,ye);
					dc.SelectObject(OverwritePen2);
					dc.LineTo(xe1,ye1);
					dc.SelectObject(OverwritePen1);
					dc.MoveTo(xe,ye);
					dc.LineTo(xe1,ye1);

					//oldpen = dc.SelectObject(OverwritePen);
					//dc.MoveTo(xs,ys);
					//dc.LineTo(xe,ye);

					dc.SelectObject(oldpen);
					dc.SetROP2(drawmode);

					//自动交换画线模式
					if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_END;
					else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_END) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_START;
					else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_START) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_END;
					else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_END) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_START;
					Write_Info(szInfo[nEDA_SCH_PlaceWireMode]);

					// 创建并存储连线
					if(pline == NULL) 
					{
						pline = new COb_schline();
						pline->pDocument = pDoc;
						pline->m_nAttrib = cEDA_schwire.m_nAttrib;
						pline->m_nLineWidth = cEDA_schwire.m_nLineWidth;
						pline->m_nLineType = cEDA_schwire.m_nLineType;
						pline->m_nColor = cEDA_schwire.m_nColor;
						pline->m_nDiagram = m_nDiagram;
						pline->m_cVertex.SetSize(1);
						line_pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pline);
						
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_LINE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = line_pos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//--------------------------------------------------------------------------------------------------
					}
					int vertexSize = pline->m_cVertex.GetCount();
					pline->m_cVertex.SetSize(vertexSize + 1);
					pline->m_cVertex[vertexSize-1].fx = x1;
					pline->m_cVertex[vertexSize-1].fy = y1;
					pline->m_cVertex[vertexSize].fx = x2;
					pline->m_cVertex[vertexSize].fy = y2;
					if(bEDA_AutoJunction == TRUE) AutoPlaceJunction();

					//检查是否自动放置接点
					/*if((CL_Auto_Junction == TRUE)&&(pschline->m_nLinetype == 0))
					{
						int i,num;
						char *p;
						COb_schline* ptempline;

						num = pDoc->list_schline.GetCount();
						pos_1 = pDoc->list_schline.GetHeadPosition();
						for(i=0; i<num; i++)
						{	
							if(pos_1 == NULL) break;
							ptempline = pDoc->list_schline.GetNext(pos_1);
							if( (ptempline->m_nDiagram != m_nDiagram)||\
							    (ptempline->m_nFlag == FLAG_DELETED)||(ptempline->m_nLinetype != 0) )
							{
								continue;
							}
							if(ptempline->Is_On_Line(x2,y2) == FALSE) continue;
							if( ((ptempline->m_nSx == ptempline->m_nEx)&&(pschline->m_nSx == pschline->m_nEx)) ||\
								((ptempline->m_nSy == ptempline->m_nEy)&&(pschline->m_nSy == pschline->m_nEy)) ||\
								((ptempline->m_nSx != ptempline->m_nEx)&&(pschline->m_nSx != pschline->m_nEx)&&\
								 ((int)((pschline->m_nEy - pschline->m_nSy)/(pschline->m_nEx - pschline->m_nSx)) == (int)(ptempline->m_nEy - ptempline->m_nSy)/(ptempline->m_nEx - ptempline->m_nSx))) )continue;
							if( ((ptempline->m_nSx == x2)&&(ptempline->m_nSy == y2)) ||\
								((ptempline->m_nEx == x2)&&(ptempline->m_nEy == y2))  ) continue;
							if(Exist_Junction(x2,y2) != 0) continue;
							
							COb_schjunc* pjunc = new COb_schjunc;
							pjunc->pDocument = pDoc;
							pjunc->m_fCx = x2;
							pjunc->m_fCy = y2;
							pjunc->m_nSize = cEDA_schjunc.m_nSize;
							pjunc->m_nColor = cEDA_schjunc.m_nColor;
							pjunc->m_bLocked = FALSE;
							pjunc->m_nDiagram = m_nDiagram;
							junc_pos = pDoc->list_schjunc.AddTail(pschjunc);
							pjunc->Draw(&dc, pView);
							
							//----------- Undo ----------------------------------------------------------------------------
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = UNDO_JUNC;
							pundo->m_nPos = junc_pos;
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							//---------------------------------------------------------------------------------------------
						}
					}*/
					
					POINT p;
					int ix,iy;
					Get_VPort_XY(&ix,&iy,m_fCurrentSnapX,m_fCurrentSnapY);
					p.x = ix; p.y = iy;
					DrawMyCursor(&dc,&p);
					pline->Draw(&dc, this);
					DrawMyCursor(&dc,&p);

					x1 = x2;  y1 = y2;
					x2 = x3;  y2 = y3;
					xs = xe;  ys = ye;
					xe = xe1; ye = ye1;
					continue;
				}
				else if(ch1 == SPACE)
				{
					if((GetKeyState(VK_SHIFT)&0x80) != 0)
					{
						//shift+space
						if((nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START)||(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_END)) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_START;
						else if((nEDA_SCH_PlaceWireMode == SCH_PLACE_45_START)||(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_END)) nEDA_SCH_PlaceWireMode = SCH_PLACE_ANYANGLE;
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_ANYANGLE) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_START; //SCH_PLACE_AUTO
						//else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_AUTO) nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START;
					}
					else 
					{
						//space
						if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_END;
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_END) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_START;
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_START) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_END;
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_END) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_START;
						//else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_ANYANGLE) nEDA_SCH_PlaceWireMode = SCH_PLACE_AUTO;
						//else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_AUTO) nEDA_SCH_PlaceWireMode = SCH_PLACE_ANYANGLE;
					}
					Write_Info(szInfo[nEDA_SCH_PlaceWireMode]);
					ch1 = MOUSEMOVE;
					goto LABEL_1;
				}
				else if(ch1 == TAB)
				{
					CSheet_EditWire  ps(_T("导线"), NULL, 0);
		
					//属性页
					ps.page1.m_nComboWidth = cEDA_schwire.m_nLineWidth;
					ps.page1.m_nColor = cEDA_schwire.m_nColor;
					//顶点页
					ps.RemovePage(1);
					if(ps.DoModal() == IDOK)
					{
						//属性
						cEDA_schwire.m_nLineWidth = ps.page1.m_nComboWidth;
						cEDA_schwire.m_nColor = ps.page1.m_nColor;

						if(pline != NULL)
						{
							pline->m_nColor = cEDA_schwire.m_nColor;
							pline->m_nLineWidth = cEDA_schwire.m_nLineWidth;
							//重新显示已绘制的wire
						}

						delete OverwritePen1;
						delete OverwritePen2;
						OverwritePen1 = new CPen(PS_SOLID,1,cEDA_schwire.m_nColor);
						OverwritePen2 = new CPen(PS_DOT, 1,cEDA_schwire.m_nColor);
					}					
				}
			}while(1);
		}
		else if( ch1 == MOUSEMOVE)
		{
			if(nEDA_RedrawFlag == ON) nEDA_RedrawFlag = OFF;
			else  ElecHotPointsControl(&dc);
		}
		else if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
		{
			ElecHotPointsControl(&dc);
			break;
		}
		else Command_In_Function(ch1);
    }while(1);
	
	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		//优化连线
		if(bEDA_AutoOptimizeWire == TRUE) AutoOptimizeWire();

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}



	//free GUI object
	delete OverwritePen1;
	delete OverwritePen2;

	Write_Help(szEDA_Help_Ready);

	//电路网格热点Disable
	bEDA_NeedElectricalGrid = FALSE;
	m_fpHotPoint1.fx = m_fpHotPoint1.fy = m_fpHotPoint2.fx = m_fpHotPoint2.fy = 0;
}

void CESDView::OnPlaceBus()
{
	int ch1;
	double x1,y1,x2,y2,x3,y3,tempx, tempy;
	int xs,ys,xe,ye,xe1,ye1,flag;
	int drawmode;
	char szPlaceLineStart[]  = "放置总线: <左键><回车>定义起点, <右键><ESC>结束命令";
	char szPlaceLineEnd[]  = "放置总线: <左键><回车>定义终点, <右键><ESC>返回上一步, <TAB>定义属性";
	char szInfo[6][100] = {	"<shift+空格><空格>修改模式: 90度-水平",	"<shift+空格><空格>修改模式: 90度-垂直",	"<shift+空格><空格>修改模式: 45度-水平垂直",\
							"<shift+空格><空格>修改模式: 45度-45度",	"<shift+空格><空格>修改模式: 任意角度",		"<shift+空格><空格>修改模式: 自动连线"	};
	
	POSITION line_pos;
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作
	
	COb_schline* pline = NULL;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	Write_Help(szPlaceLineStart);

	CPen *OverwritePen1,  *OverwritePen2, *oldpen;
	OverwritePen1 = new CPen(PS_SOLID,1,cEDA_schbus.m_nColor);
	OverwritePen2 = new CPen(PS_DOT, 1,cEDA_schbus.m_nColor);

	//开放Electricl Grid
	bEDA_NeedElectricalGrid = TRUE;

	flag = 1;
	nEDA_RedrawFlag = OFF;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			ElecHotPointsControl(&dc);

			x1 = m_fCurrentSnapX;
			y1 = m_fCurrentSnapY;
			x2 = x3 = x1; 
			y2 = y3 = y1;
			Get_VPort_XY(&xs,&ys,x1,y1);
			xe = xe1 = xs;
			ye = ye1 = ys;
			Write_Help(szPlaceLineEnd);
			Write_Info(szInfo[nEDA_SCH_PlaceWireMode]);
			do
			{
				ch1 = MouseKey();
LABEL_1:
				Get_VPort_XY(&xs,&ys,x1,y1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{ 
					ElecHotPointsControl(&dc);
					Write_Help(szPlaceLineStart);
					Write_Info(szEDA_Info_Ready);

					drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);
	
					dc.MoveTo(xs,ys);
					oldpen = dc.SelectObject(OverwritePen1);
					dc.LineTo(xe,ye);
					dc.SelectObject(OverwritePen2);
					dc.LineTo(xe1,ye1);

					dc.SelectObject(oldpen);
					dc.SetROP2(drawmode);

					ch1 = MOUSEMOVE;
					pline = NULL;	//再次绘制新线段
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
							ElecHotPointsControl(&dc);

							drawmode = dc.GetROP2();
							dc.SetROP2(R2_NOTXORPEN);

							dc.MoveTo(xs,ys);
							oldpen = dc.SelectObject(OverwritePen1);
							dc.LineTo(xe,ye);
							dc.SelectObject(OverwritePen2);
							dc.LineTo(xe1,ye1);

							dc.SelectObject(oldpen);
							dc.SetROP2(drawmode);
						}
						x3 = x2 = m_fCurrentSnapX; y3 = y2 = m_fCurrentSnapY;
						Get_VPort_XY(&xe,&ye,x2,y2);
						xe1 = xe;  ye1 = ye;
						if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START)
						{
							if((x3 == x1)&&(y3 == y1)) {} //重合
							else if(x3 == x1){} //垂直
							else if(y3 == y1){}//水平
							else
							{
								x2 = x3;
								y2 = y1;
							}
							Get_VPort_XY(&xe,&ye,x2,y2);
							Get_VPort_XY(&xe1,&ye1,x3,y3);
						}
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_END)
						{
							if((x3 == x1)&&(y3 == y1)) {} //重合
							else if(x3 == x1){} //垂直
							else if(y3 == y1){}//水平
							else
							{
								x2 = x1;
								y2 = y3;
							}
							Get_VPort_XY(&xe,&ye,x2,y2);
							Get_VPort_XY(&xe1,&ye1,x3,y3);
						}
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_START)
						{
							if((x3 == x1)&&(y3 == y1)) {} //重合
							else if(x3 == x1){} //垂直
							else if(y3 == y1){}//水平
							else if(abs(x3-x1) == abs(y3-y1)) {}//45度
							else if((x3>x1)&&(y3>y1))  // I象限
							{  
								tempx=x3-x1;  tempy=y3-y1;
								if(tempx > tempy)
								{  
										x2 = x3 - tempy;
										y2 = y1;
								}
								else
								{ 
										y2 = y3 - tempx;
										x2 = x1;
								}
							}
							else if((x1>x3)&&(y3>y1))	//II象限
							{  
								tempx=x1-x3;  tempy=y3-y1;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										x2 = x3 + tempy;
										y2 = y1;
								}
								else
								{ 
										y2 = y3 - tempx;
										x2 = x1;
								}
							}
							else if((x3>x1)&&(y1>y3))	//IV象限
							{  
								tempx=x3-x1;  tempy=y1-y3;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										x2 = x3 - tempy;
										y2 = y1;
								}
								else
								{ 
										y2 = y3 + tempx;
										x2 = x1;
								}
							}
							else if((x1>x3)&&(y1>y3))	//III象限
							{  
								tempx=x1-x3;  tempy=y1-y3;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										x2 = x3 + tempy;
										y2 = y1;
								}
								else
								{ 
										y2 = y3 + tempx;
										x2 = x1;
								}
							}
							Get_VPort_XY(&xe,&ye,x2,y2);
							Get_VPort_XY(&xe1,&ye1,x3,y3);
						}
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_END)
						{
							if((x3 == x1)&&(y3 == y1)) {} //重合
							else if(x3 == x1){} //垂直
							else if(y3 == y1){}//水平
							else if(abs(x3-x1) == abs(y3-y1)) {}//45度
							else if((x3>x1)&&(y3>y1))  // I象限
							{  
								tempx=x3-x1;  tempy=y3-y1;
								if(tempx > tempy)
								{  
										x2 = x1 + tempy;
										y2 = y3;
								}
								else
								{ 
										y2 = y1 + tempx;
										x2 = x3;
								}
							}
							else if((x1>x3)&&(y3>y1))	//II象限
							{  
								tempx=x1-x3;  tempy=y3-y1;
								if(tempx > tempy)
								{  
										x2 = x1 - tempy;
										y2 = y3;
								}
								else
								{ 
										y2 = y1 + tempx;
										x2 = x3;
								}
							}
							else if((x3>x1)&&(y1>y3))	//IV象限
							{  
								tempx=x3-x1;  tempy=y1-y3;
								if(tempx > tempy)
								{  
										x2 = x1 + tempy;
										y2 = y3;
								}
								else
								{ 
										y2 = y1 - tempx;
										x2 = x3;
								}
							}
							else if((x1>x3)&&(y1>y3))	//III象限
							{  
								tempx=x1-x3;  tempy=y1-y3;
								if(tempx > tempy)
								{  
										x2 = x1 - tempy;
										y2 = y3;
								}
								else
								{ 
										y2 = y1 - tempx;
										x2 = x3;
								}
							}
							Get_VPort_XY(&xe,&ye,x2,y2);
							Get_VPort_XY(&xe1,&ye1,x3,y3);
						}
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_ANYANGLE)
						{

						}
						drawmode = dc.GetROP2();
						dc.SetROP2(R2_NOTXORPEN);
							
						dc.MoveTo(xs,ys);
						oldpen = dc.SelectObject(OverwritePen1);
						dc.LineTo(xe,ye);
						dc.SelectObject(OverwritePen2);
						dc.LineTo(xe1,ye1);

						dc.SelectObject(oldpen);
						dc.SetROP2(drawmode);
					}
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					ElecHotPointsControl(&dc);
					if((x1==x3)&&(y1==y3)) continue;

					drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);
						
					dc.MoveTo(xs,ys);
					oldpen = dc.SelectObject(OverwritePen1);
					dc.LineTo(xe,ye);
					dc.SelectObject(OverwritePen2);
					dc.LineTo(xe1,ye1);
					dc.SelectObject(OverwritePen1);
					dc.MoveTo(xe,ye);
					dc.LineTo(xe1,ye1);

					//oldpen = dc.SelectObject(OverwritePen);
					//dc.MoveTo(xs,ys);
					//dc.LineTo(xe,ye);

					dc.SelectObject(oldpen);
					dc.SetROP2(drawmode);

					//自动交换画线模式
					if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_END;
					else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_END) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_START;
					else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_START) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_END;
					else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_END) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_START;
					Write_Info(szInfo[nEDA_SCH_PlaceWireMode]);

					// 创建并存储连线
					if(pline == NULL) 
					{
						pline = new COb_schline();
						pline->pDocument = pDoc;
						pline->m_nAttrib = cEDA_schbus.m_nAttrib;
						pline->m_nLineWidth = cEDA_schbus.m_nLineWidth;
						pline->m_nLineType = cEDA_schbus.m_nLineType;
						pline->m_nColor = cEDA_schbus.m_nColor;
						pline->m_nDiagram = m_nDiagram;
						pline->m_cVertex.SetSize(1);
						line_pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pline);
						
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_LINE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = line_pos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//--------------------------------------------------------------------------------------------------
					}
					int vertexSize = pline->m_cVertex.GetCount();
					pline->m_cVertex.SetSize(vertexSize + 1);
					pline->m_cVertex[vertexSize-1].fx = x1;
					pline->m_cVertex[vertexSize-1].fy = y1;
					pline->m_cVertex[vertexSize].fx = x2;
					pline->m_cVertex[vertexSize].fy = y2;

					POINT p;
					int ix,iy;
					Get_VPort_XY(&ix,&iy,m_fCurrentSnapX,m_fCurrentSnapY);
					p.x = ix; p.y = iy;
					DrawMyCursor(&dc,&p);
					pline->Draw(&dc, this);
					DrawMyCursor(&dc,&p);

					x1 = x2;  y1 = y2;
					x2 = x3;  y2 = y3;
					xs = xe;  ys = ye;
					xe = xe1; ye = ye1;
					continue;
				}
				else if(ch1 == SPACE)
				{
					if((GetKeyState(VK_SHIFT)&0x80) != 0)
					{
						//shift+space
						if((nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START)||(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_END)) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_START;
						else if((nEDA_SCH_PlaceWireMode == SCH_PLACE_45_START)||(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_END)) nEDA_SCH_PlaceWireMode = SCH_PLACE_ANYANGLE;
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_ANYANGLE) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_START; //SCH_PLACE_AUTO
						//else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_AUTO) nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START;
					}
					else 
					{
						//space
						if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_END;
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_END) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_START;
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_START) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_END;
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_END) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_START;
						//else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_ANYANGLE) nEDA_SCH_PlaceWireMode = SCH_PLACE_AUTO;
						//else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_AUTO) nEDA_SCH_PlaceWireMode = SCH_PLACE_ANYANGLE;
					}
					Write_Info(szInfo[nEDA_SCH_PlaceWireMode]);
					ch1 = MOUSEMOVE;
					goto LABEL_1;
				}
				else if(ch1 == TAB)
				{
					CSheet_EditBus  ps(_T("总线"), NULL, 0);
		
					//属性页
					ps.page1.m_nComboWidth = cEDA_schbus.m_nLineWidth;
					ps.page1.m_nColor = cEDA_schbus.m_nColor;
					//顶点页
					ps.RemovePage(1);
					if(ps.DoModal() == IDOK)
					{
						//属性
						cEDA_schbus.m_nLineWidth = ps.page1.m_nComboWidth;
						cEDA_schbus.m_nColor = ps.page1.m_nColor;

						if(pline != NULL)
						{
							pline->m_nColor = cEDA_schbus.m_nColor;
							pline->m_nLineWidth = cEDA_schbus.m_nLineWidth;
							//重新显示已绘制的wire
						}

						delete OverwritePen1;
						delete OverwritePen2;
						OverwritePen1 = new CPen(PS_SOLID, 1, cEDA_schbus.m_nColor);
						OverwritePen2 = new CPen(PS_DOT, 1, cEDA_schbus.m_nColor);
					}					
				}
			}while(1);
		}
		else if( ch1 == MOUSEMOVE)
		{
			if(nEDA_RedrawFlag == ON) nEDA_RedrawFlag = OFF;
			else  ElecHotPointsControl(&dc);
		}
		else if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
		{
			ElecHotPointsControl(&dc);
			break;
		}
		else Command_In_Function(ch1);
    }while(1);
	
	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	//free GUI object
	delete OverwritePen1;
	delete OverwritePen2;

	Write_Help(szEDA_Help_Ready);
	
	bEDA_NeedElectricalGrid = FALSE;
	m_fpHotPoint1.fx = m_fpHotPoint1.fy = m_fpHotPoint2.fx = m_fpHotPoint2.fy = 0;
}

void CESDView::OnPlacePolyline()
{
	int ch1;
	double x1,y1,x2,y2,x3,y3,tempx, tempy;
	int xs,ys,xe,ye,xe1,ye1,flag;
	int drawmode;
	char szPlaceLineStart[]  = "放置连线: <左键><回车>定义起点, <右键><ESC>结束命令";
	char szPlaceLineEnd[]  = "放置连线: <左键><回车>定义终点, <右键><ESC>返回上一步, <TAB>定义属性";
	char szInfo[6][100] = {	"<shift+空格><空格>修改模式: 90度-水平",	"<shift+空格><空格>修改模式: 90度-垂直",	"<shift+空格><空格>修改模式: 45度-水平垂直",\
							"<shift+空格><空格>修改模式: 45度-45度",	"<shift+空格><空格>修改模式: 任意角度",		"<shift+空格><空格>修改模式: 自动连线"	};
	
	POSITION line_pos;
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作
	
	COb_schpolyline* ppolyline = NULL;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	Write_Help(szPlaceLineStart);

	CPen *OverwritePen1,  *OverwritePen2, *oldpen;
	OverwritePen1 = new CPen(PS_SOLID,1,cEDA_schpolyline.m_nColor);
	OverwritePen2 = new CPen(PS_DOT, 1,cEDA_schpolyline.m_nColor);
					
	drawmode = dc.GetROP2();
	flag = 0;
	nEDA_RedrawFlag = OFF;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			x1 = m_fCurrentSnapX;
			y1 = m_fCurrentSnapY;
			x2 = x3 = x1; 
			y2 = y3 = y1;
			Get_VPort_XY(&xs,&ys,x1,y1);
			xe = xe1 = xs;
			ye = ye1 = ys;
			Write_Help(szPlaceLineEnd);
			Write_Info(szInfo[nEDA_SCH_PlaceWireMode]);
			do
			{
				ch1 = MouseKey();
LABEL_1:
				Get_VPort_XY(&xs,&ys,x1,y1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{ 
					Write_Help(szPlaceLineStart);
					Write_Info(szEDA_Info_Ready);

					dc.SetROP2(R2_NOTXORPEN);
	
					dc.MoveTo(xs,ys);
					oldpen = dc.SelectObject(OverwritePen1);
					dc.LineTo(xe,ye);
					dc.SelectObject(OverwritePen2);
					dc.LineTo(xe1,ye1);

					dc.SelectObject(oldpen);
					dc.SetROP2(drawmode);

					ch1 = MOUSEMOVE;
					if(ppolyline != NULL)
					{
						ppolyline->m_nEndLineShape = cEDA_schpolyline.m_nEndLineShape;
						ppolyline->Draw(&dc, this);
						ppolyline = NULL;	//再次绘制新线段
					}
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
							//drawmode = dc.GetROP2();
							dc.SetROP2(R2_NOTXORPEN);

							dc.MoveTo(xs,ys);
							oldpen = dc.SelectObject(OverwritePen1);
							dc.LineTo(xe,ye);
							dc.SelectObject(OverwritePen2);
							dc.LineTo(xe1,ye1);

							dc.SelectObject(oldpen);
							dc.SetROP2(drawmode);
						}
						x3 = x2 = m_fCurrentSnapX; y3 = y2 = m_fCurrentSnapY;
						Get_VPort_XY(&xe,&ye,x2,y2);
						xe1 = xe;  ye1 = ye;
						if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START)
						{
							if((x3 == x1)&&(y3 == y1)) {} //重合
							else if(x3 == x1){} //垂直
							else if(y3 == y1){}//水平
							else
							{
								x2 = x3;
								y2 = y1;
							}
							Get_VPort_XY(&xe,&ye,x2,y2);
							Get_VPort_XY(&xe1,&ye1,x3,y3);
						}
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_END)
						{
							if((x3 == x1)&&(y3 == y1)) {} //重合
							else if(x3 == x1){} //垂直
							else if(y3 == y1){}//水平
							else
							{
								x2 = x1;
								y2 = y3;
							}
							Get_VPort_XY(&xe,&ye,x2,y2);
							Get_VPort_XY(&xe1,&ye1,x3,y3);
						}
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_START)
						{
							if((x3 == x1)&&(y3 == y1)) {} //重合
							else if(x3 == x1){} //垂直
							else if(y3 == y1){}//水平
							else if(abs(x3-x1) == abs(y3-y1)) {}//45度
							else if((x3>x1)&&(y3>y1))  // I象限
							{  
								tempx=x3-x1;  tempy=y3-y1;
								if(tempx > tempy)
								{  
										x2 = x3 - tempy;
										y2 = y1;
								}
								else
								{ 
										y2 = y3 - tempx;
										x2 = x1;
								}
							}
							else if((x1>x3)&&(y3>y1))	//II象限
							{  
								tempx=x1-x3;  tempy=y3-y1;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										x2 = x3 + tempy;
										y2 = y1;
								}
								else
								{ 
										y2 = y3 - tempx;
										x2 = x1;
								}
							}
							else if((x3>x1)&&(y1>y3))	//IV象限
							{  
								tempx=x3-x1;  tempy=y1-y3;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										x2 = x3 - tempy;
										y2 = y1;
								}
								else
								{ 
										y2 = y3 + tempx;
										x2 = x1;
								}
							}
							else if((x1>x3)&&(y1>y3))	//III象限
							{  
								tempx=x1-x3;  tempy=y1-y3;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										x2 = x3 + tempy;
										y2 = y1;
								}
								else
								{ 
										y2 = y3 + tempx;
										x2 = x1;
								}
							}
							Get_VPort_XY(&xe,&ye,x2,y2);
							Get_VPort_XY(&xe1,&ye1,x3,y3);
						}
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_END)
						{
							if((x3 == x1)&&(y3 == y1)) {} //重合
							else if(x3 == x1){} //垂直
							else if(y3 == y1){}//水平
							else if(abs(x3-x1) == abs(y3-y1)) {}//45度
							else if((x3>x1)&&(y3>y1))  // I象限
							{  
								tempx=x3-x1;  tempy=y3-y1;
								if(tempx > tempy)
								{  
										x2 = x1 + tempy;
										y2 = y3;
								}
								else
								{ 
										y2 = y1 + tempx;
										x2 = x3;
								}
							}
							else if((x1>x3)&&(y3>y1))	//II象限
							{  
								tempx=x1-x3;  tempy=y3-y1;
								if(tempx > tempy)
								{  
										x2 = x1 - tempy;
										y2 = y3;
								}
								else
								{ 
										y2 = y1 + tempx;
										x2 = x3;
								}
							}
							else if((x3>x1)&&(y1>y3))	//IV象限
							{  
								tempx=x3-x1;  tempy=y1-y3;
								if(tempx > tempy)
								{  
										x2 = x1 + tempy;
										y2 = y3;
								}
								else
								{ 
										y2 = y1 - tempx;
										x2 = x3;
								}
							}
							else if((x1>x3)&&(y1>y3))	//III象限
							{  
								tempx=x1-x3;  tempy=y1-y3;
								if(tempx > tempy)
								{  
										x2 = x1 - tempy;
										y2 = y3;
								}
								else
								{ 
										y2 = y1 - tempx;
										x2 = x3;
								}
							}
							Get_VPort_XY(&xe,&ye,x2,y2);
							Get_VPort_XY(&xe1,&ye1,x3,y3);
						}
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_ANYANGLE)
						{

						}
						//drawmode = dc.GetROP2();
						dc.SetROP2(R2_NOTXORPEN);
							
						dc.MoveTo(xs,ys);
						oldpen = dc.SelectObject(OverwritePen1);
						dc.LineTo(xe,ye);
						dc.SelectObject(OverwritePen2);
						dc.LineTo(xe1,ye1);

						dc.SelectObject(oldpen);
						dc.SetROP2(drawmode);
					}
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					if((x1==x3)&&(y1==y3)) continue;

					//drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);
						
					dc.MoveTo(xs,ys);
					oldpen = dc.SelectObject(OverwritePen1);
					dc.LineTo(xe,ye);
					dc.SelectObject(OverwritePen2);
					dc.LineTo(xe1,ye1);
					dc.SelectObject(OverwritePen1);
					dc.MoveTo(xe,ye);
					dc.LineTo(xe1,ye1);

					//oldpen = dc.SelectObject(OverwritePen);
					//dc.MoveTo(xs,ys);
					//dc.LineTo(xe,ye);

					dc.SelectObject(oldpen);
					dc.SetROP2(drawmode);

					//自动交换画线模式
					if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_END;
					else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_END) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_START;
					else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_START) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_END;
					else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_END) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_START;
					Write_Info(szInfo[nEDA_SCH_PlaceWireMode]);

					// 创建并存储连线
					if(ppolyline == NULL) 
					{
						ppolyline = new COb_schpolyline;
						ppolyline->pDocument = pDoc;
						ppolyline->m_nColor = cEDA_schpolyline.m_nColor;
						ppolyline->m_nLineWidth = cEDA_schpolyline.m_nLineWidth;
						ppolyline->m_nLineType = cEDA_schpolyline.m_nLineType;
						ppolyline->m_nStartLineShape = cEDA_schpolyline.m_nStartLineShape;
						ppolyline->m_nEndLineShape = 0;//最后ESC结束时再修改此项  ppolyline->m_nEndLineShape = cEDA_schpolyline.m_nEndLineShape;  
						ppolyline->m_nLineShapeSize = cEDA_schpolyline.m_nLineShapeSize;
						ppolyline->m_nDiagram = m_nDiagram;
						ppolyline->m_cVertex.SetSize(1);
						line_pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ppolyline);
						
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_POLYLINE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = line_pos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//--------------------------------------------------------------------------------------------------
					}
					int vertexSize = ppolyline->m_cVertex.GetCount();
					ppolyline->m_cVertex.SetSize(vertexSize + 1);
					ppolyline->m_cVertex[vertexSize-1].fx = x1;
					ppolyline->m_cVertex[vertexSize-1].fy = y1;
					ppolyline->m_cVertex[vertexSize].fx = x2;
					ppolyline->m_cVertex[vertexSize].fy = y2;

					POINT p;
					int ix,iy;
					Get_VPort_XY(&ix,&iy,m_fCurrentSnapX,m_fCurrentSnapY);
					p.x = ix; p.y = iy;
					DrawMyCursor(&dc,&p);
					ppolyline->Draw(&dc, this);
					DrawMyCursor(&dc,&p);

					x1 = x2;  y1 = y2;
					x2 = x3;  y2 = y3;
					xs = xe;  ys = ye;
					xe = xe1; ye = ye1;
					continue;
				}
				else if(ch1 == SPACE)
				{
					if((GetKeyState(VK_SHIFT)&0x80) != 0)
					{
						//shift+space
						if((nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START)||(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_END)) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_START;
						else if((nEDA_SCH_PlaceWireMode == SCH_PLACE_45_START)||(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_END)) nEDA_SCH_PlaceWireMode = SCH_PLACE_ANYANGLE;
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_ANYANGLE) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_START; //SCH_PLACE_AUTO
						//else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_AUTO) nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START;
					}
					else 
					{
						//space
						if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_START) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_END;
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_90_END) nEDA_SCH_PlaceWireMode = SCH_PLACE_90_START;
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_START) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_END;
						else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_45_END) nEDA_SCH_PlaceWireMode = SCH_PLACE_45_START;
						//else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_ANYANGLE) nEDA_SCH_PlaceWireMode = SCH_PLACE_AUTO;
						//else if(nEDA_SCH_PlaceWireMode == SCH_PLACE_AUTO) nEDA_SCH_PlaceWireMode = SCH_PLACE_ANYANGLE;
					}
					Write_Info(szInfo[nEDA_SCH_PlaceWireMode]);
					ch1 = MOUSEMOVE;
					goto LABEL_1;
				}
				else if(ch1 == TAB)
				{
					CSheet_EditPolyline  ps(_T("连线"), NULL, 0);

					//属性页
					ps.page1.m_nColor = cEDA_schpolyline.m_nColor;
					ps.page1.m_nComboWidth = cEDA_schpolyline.m_nLineWidth;
					ps.page1.m_nComboLineType = cEDA_schpolyline.m_nLineType;
					ps.page1.m_nComboStartLineShape = cEDA_schpolyline.m_nStartLineShape;
					ps.page1.m_nComboEndLineShape = cEDA_schpolyline.m_nEndLineShape;
					ps.page1.m_nComboLineShapeSize = cEDA_schpolyline.m_nLineShapeSize;

					//顶点页
					ps.RemovePage(1);

					if(ps.DoModal() == IDOK)
					{
						//属性页
						cEDA_schpolyline.m_nColor = ps.page1.m_nColor;
						cEDA_schpolyline.m_nLineWidth = ps.page1.m_nComboWidth;
						cEDA_schpolyline.m_nLineType = ps.page1.m_nComboLineType;
						cEDA_schpolyline.m_nStartLineShape = ps.page1.m_nComboStartLineShape;
						cEDA_schpolyline.m_nEndLineShape = ps.page1.m_nComboEndLineShape;
						cEDA_schpolyline.m_nLineShapeSize = ps.page1.m_nComboLineShapeSize;
										
						delete OverwritePen1;
						delete OverwritePen2;
						OverwritePen1 = new CPen(PS_SOLID,1,cEDA_schpolyline.m_nColor);
						OverwritePen2 = new CPen(PS_DOT, 1,cEDA_schpolyline.m_nColor);

						if(ppolyline!=NULL)
						{
							ppolyline->m_nColor = cEDA_schpolyline.m_nColor;
							ppolyline->m_nLineWidth = cEDA_schpolyline.m_nLineWidth;
							ppolyline->m_nLineType = cEDA_schpolyline.m_nLineType;
							ppolyline->m_nStartLineShape = cEDA_schpolyline.m_nStartLineShape;
							//ppolyline->m_nEndLineShape = cEDA_schpolyline.m_nEndLineShape;
							ppolyline->m_nLineShapeSize = cEDA_schpolyline.m_nLineShapeSize;
						}
					}
				}
			}while(1);
		}
		else Command_In_Function(ch1);
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));
	
	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	//free GUI object
	delete OverwritePen1;
	delete OverwritePen2;

	//DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	//DrawStructButton(ID_EDIT_PLACE_LINE,FALSE);
	Write_Help(szEDA_Help_Ready);
}

void CESDView::OnPlaceJunction()
{
	int ch1;
	double x1,y1;
	int xs,ys;
	POSITION pos;
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	char szPlaceJunc[] = "放置结点: <左键><回车>放置结点, <右键><ESC>结束命令, <TAB>定义属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	MoveCursorToCenter();

	x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
	Get_VPort_XY(&xs,&ys,x1,y1);
	Write_Help(szPlaceJunc);
	
	//开放Electricl Grid
	bEDA_NeedElectricalGrid = TRUE;

	do
	{
       ch1 = MouseKey();
       if(ch1 == MOUSEMOVE)
	   {
		   x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
		   Get_VPort_XY(&xs,&ys,x1,y1);
	   }
       else if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
			if(Exist_Junction(x1,y1) == TRUE) continue;

			COb_schjunc* pjunc = new COb_schjunc;
			pjunc->pDocument = pDoc;
			pjunc->m_fCx = x1;
			pjunc->m_fCy = y1;
			pjunc->m_nSize = cEDA_schjunc.m_nSize;
			pjunc->m_nColor = cEDA_schjunc.m_nColor;
			pjunc->m_bManual = TRUE;
			pjunc->m_nDiagram = m_nDiagram;

			pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pjunc);

			//----------- Undo ----------------------------------------------------------
			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_JUNCTION;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			//---------------------------------------------------------------------------

			POINT p;
			p.x = xs; p.y = ys;
			DrawMyCursor(&dc,&p);
			pjunc->Draw(&dc, this);
			DrawMyCursor(&dc,&p);
			continue;
	   }
	   else if(ch1 == TAB)
	   {
		   	CDlg_EditJunction dlg;

			dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(cEDA_schjunc.m_fCx);
			dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(cEDA_schjunc.m_fCy);
			dlg.m_nComboJunctionSize = cEDA_schjunc.m_nSize;
			dlg.m_nColor = cEDA_schjunc.m_nColor;
			
			if(dlg.DoModal() == IDOK)
			{
				cEDA_schjunc.m_fCx = pDoc->Convert_String_To_XY(dlg.m_csEditLocationX);
				cEDA_schjunc.m_fCy = pDoc->Convert_String_To_XY(dlg.m_csEditLocationY);
				cEDA_schjunc.m_nSize = dlg.m_nComboJunctionSize;
				cEDA_schjunc.m_nColor = dlg.m_nColor;
			}
	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	
	bEDA_NeedElectricalGrid = FALSE;
	m_fpHotPoint1.fx = m_fpHotPoint1.fy = m_fpHotPoint2.fx = m_fpHotPoint2.fy = 0;
}

BOOL CESDView::Exist_Junction(double x, double y)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
	POSITION pos;
	pos = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{	
		if(pos == NULL) break;
		pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
			if((pjunc->m_fCx == x)&&(pjunc->m_fCy == y)) return TRUE;
		}
	}
	
	return FALSE;
}

void CESDView::OnPlaceText()
{
	int ch1;
	int flag;
	double x1,y1;
	POSITION pos;
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作
	CBitmap *pBitmap = NULL;
	
	COb_schtext atext;
	char szPlaceText[] = "放置文字: <空格>旋转, <左键><回车>放置, <右键><ESC>结束命令";
	char szInfo[] = "<TAB>修改文字及属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	MoveCursorToCenter();
	
	x1 = m_fCurrentSnapX; 
	y1 = m_fCurrentSnapY;

	atext.pDocument = pDoc;
	atext.m_nID = TEXT_TEXT;
	atext.m_fSx = x1;
	atext.m_fSy = y1;
	atext.m_nOrientation = cEDA_schtext.m_nOrientation;
	atext.m_csText = cEDA_schtext.m_csText;
	atext.m_nFont = cEDA_schtext.m_nFont;
	atext.m_nColor = cEDA_schtext.m_nColor;
	atext.m_bMirror = cEDA_schtext.m_bMirror;
	atext.m_nH_Justification = cEDA_schtext.m_nH_Justification;
	atext.m_nV_Justification = cEDA_schtext.m_nV_Justification;
	atext.m_nDiagram = m_nDiagram;

	//保存屏幕信息, 首次显示文字
	CRect rect = atext.GetMoveTextRect(&dc, this);
	SaveAreaRectangleToBitmap(&dc, rect, &pBitmap);
	atext.Draw(&dc, this);

	Write_Help(szPlaceText);
	Write_Info(szInfo);

	flag = cEDA_schtext.m_nOrientation/90;
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			//复原，隐藏文字显示
			CloseMouseCursor();
			if(pBitmap != NULL)
			{
				CopySavedAreaRectangleBack(&dc, pBitmap, rect);
				delete pBitmap;
				pBitmap = NULL;
			}
			OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			//视未获得focus时不响应鼠标移动
			if(m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(m_bAfterSetFocus)
			{
				m_bAfterSetFocus = false;
				continue;
			}
			
			
			if(nEDA_RedrawFlag == 1)
			{ 
				//屏幕刚重新显示过则不需要复原操作
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				//复原，隐藏文字显示
				CloseMouseCursor();
				if(pBitmap != NULL)
				{
					CopySavedAreaRectangleBack(&dc, pBitmap, rect);
					delete pBitmap;
					pBitmap = NULL;
				}
				OpenMouseCursor();
			}

			x1 = m_fCurrentSnapX;  y1 = m_fCurrentSnapY;
			atext.m_fSx = x1; atext.m_fSy = y1;
			
			//移动文字后，在新的位置保存覆盖前图形
			CloseMouseCursor();
			rect = atext.GetMoveTextRect(&dc, this);
			SaveAreaRectangleToBitmap(&dc, rect, &pBitmap);
			//显示文字
			atext.Draw(&dc, this);
			OpenMouseCursor();

			//atext.GetMoveTextRect(&dc, this);
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			//判断是否超出边界
			//if(atext.OutOfBorder(&dc) == TRUE) continue;
			//复原，隐藏文字显示
			CloseMouseCursor();
			if(pBitmap != NULL)
			{
				CopySavedAreaRectangleBack(&dc, pBitmap, rect);
				delete pBitmap;
				pBitmap = NULL;
			}
			OpenMouseCursor();

			atext.m_fSx = x1; atext.m_fSy = y1;
			
			cEDA_schtext.m_nOrientation = atext.m_nOrientation;
			cEDA_textfont.lfOrientation = atext.m_nOrientation;
			int font_index = pDoc->CheckFontExist(&cEDA_textfont, m_nDiagram);
			if(font_index < 0) font_index = pDoc->m_arrayFont[m_nDiagram].Add(cEDA_textfont);


			COb_schtext* ptext = new COb_schtext;
			ptext->pDocument = pDoc;
			ptext->m_nID = TEXT_TEXT;
			ptext->m_fSx = atext.m_fSx;
			ptext->m_fSy = atext.m_fSy;
			ptext->m_nOrientation = atext.m_nOrientation;

			ptext->m_csText = atext.m_csText;
			ptext->m_nFont = font_index;
			ptext->m_nColor = atext.m_nColor;
			
			ptext->m_bMirror = atext.m_bMirror;
			ptext->m_nH_Justification = atext.m_nH_Justification;
			ptext->m_nV_Justification = atext.m_nV_Justification;

			ptext->m_nDiagram = m_nDiagram;

			pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ptext);
			ptext->Draw(&dc, this);
			
			//----------- Undo -------------------------------------------------------
			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_TEXT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			//------------------------------------------------------------------------
		}
		else if((ch1==KEY_X)||(ch1==KEY_x))
		{  

		}
		else if(ch1 == VK_SPACE)
		{ 
			flag++;
			if(flag>3) flag=0;
			
			//复原，隐藏文字显示
			CloseMouseCursor();
			if(pBitmap != NULL)
			{
				CopySavedAreaRectangleBack(&dc, pBitmap, rect);
				delete pBitmap;
				pBitmap = NULL;
			}
			
			atext.m_nOrientation = flag*90;
			cEDA_schtext.m_nOrientation = flag*90;

			//移动文字后，在新的位置保存覆盖前图形
			rect = atext.GetMoveTextRect(&dc, this);
			SaveAreaRectangleToBitmap(&dc, rect, &pBitmap);
			//显示文字
			atext.Draw(&dc, this);
			OpenMouseCursor();
		}
		else  if(ch1 == TAB)
		{
			CDlg_EditText dlg;

			dlg.m_csEditLocationX = pDoc->Convert_XY_To_String(cEDA_schtext.m_fSx);
			dlg.m_csEditLocationY = pDoc->Convert_XY_To_String(cEDA_schtext.m_fSy);
			dlg.m_nColor = cEDA_schtext.m_nColor;
			dlg.m_nComboOrientation = cEDA_schtext.m_nOrientation/90;
			dlg.m_csEditText = cEDA_schtext.m_csText;

			//字型参数
			dlg.m_csFontName = cEDA_textfont.lfFaceName;

			char str[100]; itoa(cEDA_textfont.lfHeight, str, 10);
			dlg.m_csFontSize = str;

			if(cEDA_textfont.lfWeight == FW_BOLD)	dlg.m_bBold = TRUE;
			else dlg.m_bBold = FALSE;
			dlg.m_bItalic = cEDA_textfont.lfItalic;
			dlg.m_bUnderline = cEDA_textfont.lfUnderline;
			dlg.m_bCheckMirror = cEDA_schtext.m_bMirror;
			dlg.m_nComboHJustification = cEDA_schtext.m_nH_Justification;
			dlg.m_nComboVJustification = cEDA_schtext.m_nV_Justification;

			if(dlg.DoModal() == IDOK)
			{
				cEDA_schtext.m_nColor = dlg.m_nColor;
				cEDA_schtext.m_nOrientation = dlg.m_nComboOrientation*90;
				cEDA_schtext.m_csText = dlg.m_csEditText;
				cEDA_schtext.m_nH_Justification = dlg.m_nComboHJustification;
				cEDA_schtext.m_nV_Justification = dlg.m_nComboVJustification;

				//字型参数
				if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
				strcpy(cEDA_textfont.lfFaceName, dlg.m_csFontName);

				int index = dlg.m_csFontSize.Find('(');
				strcpy(str, dlg.m_csFontSize);
				if(index >= 0)	str[index] = '\0';
				cEDA_textfont.lfHeight = atoi(str);

				if(dlg.m_bBold == TRUE) cEDA_textfont.lfWeight = FW_BOLD;
				else cEDA_textfont.lfWeight = FW_NORMAL;
				cEDA_textfont.lfItalic = dlg.m_bItalic;
				cEDA_textfont.lfUnderline = dlg.m_bUnderline;

				int font_num = pDoc->CheckFontExist(&cEDA_textfont, m_nDiagram);
				if(font_num >= 0) cEDA_schtext.m_nFont = font_num;
				else
				{
					cEDA_schtext.m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(cEDA_textfont);
				}
				
				if(cEDA_schtext.m_bMirror != dlg.m_bCheckMirror)
				{
					cEDA_schtext.m_bMirror = dlg.m_bCheckMirror;
					if((cEDA_schtext.m_nOrientation == 0)||(cEDA_schtext.m_nOrientation == 180))
					{
						if(cEDA_schtext.m_nH_Justification == 0) cEDA_schtext.m_nH_Justification = 2;
						else if(cEDA_schtext.m_nH_Justification == 2) cEDA_schtext.m_nH_Justification = 0;
					}
					else
					{
						if(cEDA_schtext.m_nV_Justification == 0) cEDA_schtext.m_nV_Justification = 2;
						else if(cEDA_schtext.m_nV_Justification == 2) cEDA_schtext.m_nV_Justification = 0;
					}
				}

				//将cEDA_schtext的更新写入 atext
				atext.m_csText = cEDA_schtext.m_csText;
				atext.m_nFont = cEDA_schtext.m_nFont;
				atext.m_nOrientation = cEDA_schtext.m_nOrientation;
				atext.m_nColor = cEDA_schtext.m_nColor;
				atext.m_bMirror = cEDA_schtext.m_bMirror;
				atext.m_nH_Justification = cEDA_schtext.m_nH_Justification;
				atext.m_nV_Justification = cEDA_schtext.m_nV_Justification;

				flag = cEDA_schtext.m_nOrientation/90;
			}
		
			Invalidate(); //由于dlg消失和atext.Draw存在时间差, 会导致atext.Draw绘制的图形在dlg之下的部分出现问题, 所以重新显示画面修正
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	if (pBitmap) delete pBitmap;

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL); m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	//CL_Auto_Pan=OFF;
	//DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	//DrawStructButton(ID_EDIT_PLACE_TEXT,FALSE);
	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);
}

// This function creates a new bitmap in *ppBitmap and saves the area
// to the bitmap of choice.
//
//		CDC * pdc			-- the dc to draw on
//		CRect& rc			-- the area to save
//		CBitmap** ppBitmap	-- a pointer to a bitmap pointer where to save
//
// caller responsible to free *ppBitmap
void CESDView::SaveAreaRectangleToBitmap(CDC* pdc, CRect& rc, CBitmap** ppBitmap)
{
	CDC			dcMem;			// memory dc

	// initialize other memory dc
	dcMem.CreateCompatibleDC(pdc);

	// Create a compatible bitmap.
	*ppBitmap = new CBitmap;
	(*ppBitmap)->CreateBitmap(rc.Width(), 
					   rc.Height(),
					   pdc->GetDeviceCaps(PLANES),
					   pdc->GetDeviceCaps(BITSPIXEL),
					   NULL) ;

	// Select the bitmap into the memory DC.
	CBitmap* pOldBitmap = dcMem.SelectObject(*ppBitmap);

	// Blt the memory device context to the screen.
	dcMem.BitBlt(	0,				// dst X
					0,				// dst Y
					rc.Width(),		// dst width
					rc.Height(),	// dst height
					pdc,			// source DC
					rc.left,		// source starting X
					rc.top,			// source starting Y
					SRCCOPY) ;

	// cleanup
	dcMem.SelectObject(pOldBitmap);
}


// ok, this function copies the previously saved bitmap back to its
// original area, in effect restoring the background
//
//		CDC * pdc			-- the dc to draw on
//		CBitmap* ppBitmap	-- a pointer to a bitmap we should use to restore
//		CRect& rcOld		-- the old area to paint over
//
void CESDView::CopySavedAreaRectangleBack(CDC *pdc, CBitmap* pBitmap, CRect& rcOld)
{
	CDC			dcMem;			// memory dc

	// at first, we need to copy the background from 
	// our saved bitmap back to the screen DC
	
	// initialize other memory dc
	dcMem.CreateCompatibleDC(pdc);

	// Select the bitmap into the memory DC.
	CBitmap* pOldBitmap = dcMem.SelectObject(pBitmap);

	// Blt the bitmap to screen
	pdc->BitBlt( rcOld.left,
				 rcOld.top,
				 rcOld.Width(),
				 rcOld.Height(),
				 &dcMem,
				 0,
				 0,
				 SRCCOPY);
	
	// cleanup
	dcMem.SelectObject(pOldBitmap);
}

void CESDView::OnPlaceNetlabel()
{
	int ch1;
	int flag;
	double x1,y1;
	POSITION pos;
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作
	CBitmap *pBitmap = NULL;
	
	COb_schtext atext;
	char szPlaceText[] = "放置网络标号: <空格>旋转, <左键><回车>放置, <右键><ESC>结束命令";
	char szInfo[] = "<TAB>修改文字及属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	MoveCursorToCenter();
	
	x1 = m_fCurrentSnapX; 
	y1 = m_fCurrentSnapY;

	atext.pDocument = pDoc;
	atext.m_nID = cEDA_schnetlabel.m_nID;
	atext.m_fSx = x1;
	atext.m_fSy = y1;
	atext.m_nOrientation = cEDA_schnetlabel.m_nOrientation;
	atext.m_csText = cEDA_schnetlabel.m_csText;
	atext.m_nFont = cEDA_schnetlabel.m_nFont;
	atext.m_nColor = cEDA_schnetlabel.m_nColor;
	atext.m_bMirror = cEDA_schnetlabel.m_bMirror;
	atext.m_nH_Justification = cEDA_schnetlabel.m_nH_Justification;
	atext.m_nV_Justification = cEDA_schnetlabel.m_nV_Justification;
	atext.m_nDiagram = m_nDiagram;

	//保存屏幕信息, 首次显示文字
	CRect rect = atext.GetMoveTextRect(&dc, this);
	SaveAreaRectangleToBitmap(&dc, rect, &pBitmap);
	atext.DrawSignal(&dc, this);

	Write_Help(szPlaceText);
	Write_Info(szInfo);
	
	//开放Electricl Grid
	bEDA_NeedElectricalGrid = TRUE;

	flag = cEDA_schnetlabel.m_nOrientation/90;
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			//ElecHotPointsControl(&dc);

			//复原，隐藏文字显示
			CloseMouseCursor();
			if(pBitmap != NULL)
			{
				CopySavedAreaRectangleBack(&dc, pBitmap, rect);
				delete pBitmap;
				pBitmap = NULL;
			}
			OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			//视未获得focus时不响应鼠标移动
			if(m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(m_bAfterSetFocus)
			{
				m_bAfterSetFocus = false;
				continue;
			}
			
			
			if(nEDA_RedrawFlag == 1)
			{ 
				//屏幕刚重新显示过则不需要复原操作
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				//ElecHotPointsControl(&dc);

				//复原，隐藏文字显示
				CloseMouseCursor();
				if(pBitmap != NULL)
				{
					CopySavedAreaRectangleBack(&dc, pBitmap, rect);
					delete pBitmap;
					pBitmap = NULL;
				}
				OpenMouseCursor();
			}

			x1 = m_fCurrentSnapX;  y1 = m_fCurrentSnapY;
			atext.m_fSx = x1; atext.m_fSy = y1;
			
			//移动文字后，在新的位置保存覆盖前图形
			CloseMouseCursor();
			rect = atext.GetMoveTextRect(&dc, this);
			SaveAreaRectangleToBitmap(&dc, rect, &pBitmap);
			//显示文字
			atext.DrawSignal(&dc, this);
			OpenMouseCursor();

			//atext.GetMoveTextRect(&dc, this);
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			//ElecHotPointsControl(&dc);

			//判断是否超出边界
			//if(atext.OutOfBorder(&dc) == TRUE) continue;
			//复原，隐藏文字显示
			CloseMouseCursor();
			if(pBitmap != NULL)
			{
				CopySavedAreaRectangleBack(&dc, pBitmap, rect);
				delete pBitmap;
				pBitmap = NULL;
			}
			OpenMouseCursor();

			atext.m_fSx = x1; atext.m_fSy = y1;
			
			cEDA_schnetlabel.m_nOrientation = atext.m_nOrientation;
			cEDA_netlabelfont.lfOrientation = atext.m_nOrientation;
			int font_index = pDoc->CheckFontExist(&cEDA_netlabelfont, m_nDiagram);
			if(font_index < 0) font_index = pDoc->m_arrayFont[m_nDiagram].Add(cEDA_netlabelfont);


			COb_schtext* ptext = new COb_schtext;
			ptext->pDocument = pDoc;
			ptext->m_nID = atext.m_nID;
			ptext->m_fSx = atext.m_fSx;
			ptext->m_fSy = atext.m_fSy;
			ptext->m_nOrientation = atext.m_nOrientation;

			ptext->m_csText = atext.m_csText;
			ptext->m_nFont = font_index;
			ptext->m_nColor = atext.m_nColor;
			
			ptext->m_bMirror = atext.m_bMirror;
			ptext->m_nH_Justification = atext.m_nH_Justification;
			ptext->m_nV_Justification = atext.m_nV_Justification;

			ptext->m_nDiagram = m_nDiagram;

			pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ptext);
			ptext->DrawSignal(&dc, this);
			
			//----------- Undo -------------------------------------------------------
			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_TEXT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			//------------------------------------------------------------------------
		}
		else if((ch1==KEY_X)||(ch1==KEY_x))
		{  

		}
		else if(ch1 == VK_SPACE)
		{ 
			flag++;
			if(flag>3) flag=0;
			
			//复原，隐藏文字显示
			CloseMouseCursor();
			if(pBitmap != NULL)
			{
				CopySavedAreaRectangleBack(&dc, pBitmap, rect);
				delete pBitmap;
				pBitmap = NULL;
			}
			
			atext.m_nOrientation = flag*90;
			cEDA_schnetlabel.m_nOrientation = flag*90;

			//移动文字后，在新的位置保存覆盖前图形
			rect = atext.GetMoveTextRect(&dc, this);
			SaveAreaRectangleToBitmap(&dc, rect, &pBitmap);
			//显示文字
			atext.DrawSignal(&dc, this);
			OpenMouseCursor();
		}
		else  if(ch1 == TAB)
		{
			CDlg_EditNetlabel dlg;

			dlg.m_nColor = cEDA_schnetlabel.m_nColor;
			dlg.m_nComboOrientation = cEDA_schnetlabel.m_nOrientation/90;
			dlg.m_csEditNet = cEDA_schnetlabel.m_csText;
			
			//字型参数
			dlg.m_csFontName = cEDA_netlabelfont.lfFaceName;

			char str[100]; itoa(cEDA_netlabelfont.lfHeight, str, 10);
			dlg.m_csFontSize = str;

			if(cEDA_netlabelfont.lfWeight == FW_BOLD)	dlg.m_bBold = TRUE;
			else dlg.m_bBold = FALSE;
			dlg.m_bItalic = cEDA_netlabelfont.lfItalic;
			dlg.m_bUnderline = cEDA_netlabelfont.lfUnderline;

			if(dlg.DoModal() == IDOK)
			{
				cEDA_schnetlabel.m_nColor = dlg.m_nColor;
				cEDA_schnetlabel.m_nOrientation = dlg.m_nComboOrientation*90;
				cEDA_schnetlabel.m_csText = dlg.m_csEditNet;

				//字型参数
				if(dlg.m_csFontName.GetLength() > 31) dlg.m_csFontName.SetAt(31, '\0');
				strcpy(cEDA_netlabelfont.lfFaceName, dlg.m_csFontName);

				int index = dlg.m_csFontSize.Find('(');
				strcpy(str, dlg.m_csFontSize);
				if(index >= 0)	str[index] = '\0';
				cEDA_netlabelfont.lfHeight = atoi(str);

				if(dlg.m_bBold == TRUE) cEDA_netlabelfont.lfWeight = FW_BOLD;
				else cEDA_netlabelfont.lfWeight = FW_NORMAL;
				cEDA_netlabelfont.lfItalic = dlg.m_bItalic;
				cEDA_netlabelfont.lfUnderline = dlg.m_bUnderline;

				int font_num = pDoc->CheckFontExist(&cEDA_netlabelfont, m_nDiagram);
				if(font_num >= 0) cEDA_schnetlabel.m_nFont = font_num;
				else
				{
					cEDA_schnetlabel.m_nFont = pDoc->m_arrayFont[m_nDiagram].Add(cEDA_netlabelfont);
				}
				
				//将cEDA_schnetlabel的更新写入 atext
				atext.m_csText = cEDA_schnetlabel.m_csText;
				atext.m_nFont = cEDA_schnetlabel.m_nFont;
				atext.m_nOrientation = cEDA_schnetlabel.m_nOrientation;
				atext.m_nColor = cEDA_schnetlabel.m_nColor;

				flag = cEDA_schnetlabel.m_nOrientation/90;
			}
		
			Invalidate(); //由于dlg消失和atext.Draw存在时间差, 会导致atext.Draw绘制的图形在dlg之下的部分出现问题, 所以重新显示画面修正
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	if (pBitmap) delete pBitmap;

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL); m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);
	
	bEDA_NeedElectricalGrid = FALSE;
	m_fpHotPoint1.fx = m_fpHotPoint1.fy = m_fpHotPoint2.fx = m_fpHotPoint2.fy = 0;
}

void CESDView::OnPlaceArccenter()
{
	int ch1;
	double x1,y1,x2,y2;
	int xcenter,ycenter,xstart,ystart;
	int x3,y3,xend,yend;
	double alpha1,alpha2;
	int r;

	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	char szPlaceArcCenter[] = "放置圆弧: <左键><回车>定义圆心, <右键><ESC>结束命令";
	char szPlaceArcStart[] = "放置圆弧: <左键><回车>定义半径和起始角度, <右键><ESC>返回上一步";
	char szPlaceArcEnd[] = "放置圆弧: <左键><回车>定义终止角度, <右键><ESC>返回第一步";
	char szInfo[] = "<TAB>属性";	

	POSITION pos;

	RECT rect;
	POINT p1,p2;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);
	
	CPen *OverwritePen, *oldpen;
	OverwritePen = new CPen(PS_SOLID,1,cEDA_scharc.m_nColor);
	oldpen = dc.SelectObject(OverwritePen);
	
PLACEARC:
	Write_Help(szPlaceArcCenter);
	Write_Info(szInfo);

	nEDA_RedrawFlag = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			//圆心坐标(x1,y1)
			x1 = m_fCurrentSnapX;  y1 = m_fCurrentSnapY;
			Get_VPort_XY(&xcenter,&ycenter,x1,y1);
			x2 = x1; y2 = y1;
			xstart = xcenter; ystart = ycenter;
			r = (int)sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
			Write_Help(szPlaceArcStart);


			//dc.SelectObject(&linepen);
			dc.MoveTo(xcenter,ycenter);
			dc.LineTo(xstart,ystart);
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
				Get_VPort_XY(&xcenter,&ycenter,x1,y1); //必须更新xs,ys的数值，因为sscale和screen_x, screen_y随时会变化
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
				{ 
					//dc.SelectObject(&linepen);
					dc.MoveTo(xcenter,ycenter);
					dc.LineTo(xstart,ystart);
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
						//dc.SelectObject(&linepen);
						dc.MoveTo(xcenter,ycenter);
						dc.LineTo(xstart,ystart);
						
						rect.left = xcenter - r/m_fScale;
						rect.top = ycenter - r/m_fScale;
						rect.right = xcenter + r/m_fScale;
						rect.bottom = ycenter + r/m_fScale;
						p1.x = xcenter - r/m_fScale; p1.y = ycenter;
						p2.x = xcenter - r/m_fScale; p2.y = ycenter;
						dc.Arc(&rect,p1,p2);
					}
					//确定起点坐标，半径
					x2 = m_fCurrentSnapX; y2 = m_fCurrentSnapY;
					Get_VPort_XY(&xstart,&ystart,x2,y2);
					r = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
						
					//dc.SelectObject(&linepen);
					dc.MoveTo(xcenter,ycenter);
					dc.LineTo(xstart,ystart);
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
					if((int)r == 0) continue;

					//dc.SelectObject(&linepen);
					dc.MoveTo(xcenter,ycenter);
					dc.LineTo(xstart,ystart);
					rect.left = xcenter - r/m_fScale;
					rect.top = ycenter - r/m_fScale;
					rect.right = xcenter + r/m_fScale;
					rect.bottom = ycenter + r/m_fScale;
					p1.x = xcenter - r/m_fScale; p1.y = ycenter;
					p2.x = xcenter - r/m_fScale; p2.y = ycenter;
					dc.Arc(&rect,p1,p2);

					// 确定起点坐标(x2,y2), alpha1
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


					//dc.SelectObject(&linepen);	
					dc.MoveTo(xcenter,ycenter);
					dc.LineTo(xend,yend);
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
						//更新(x1,y1),(x2,y2)的屏幕坐标
						Get_VPort_XY(&xcenter,&ycenter,x1,y1);
						Get_VPort_XY(&xstart,&ystart,x2,y2);
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
						{
							//dc.SelectObject(&linepen);
							dc.MoveTo(xcenter,ycenter);
							dc.LineTo(xend,yend);
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
								//dc.SelectObject(&linepen);
								dc.MoveTo(xcenter,ycenter);
								dc.LineTo(xend,yend);
								rect.left = xcenter - r/m_fScale;
								rect.top = ycenter - r/m_fScale;
								rect.right = xcenter + r/m_fScale;
								rect.bottom = ycenter + r/m_fScale;
								p1.x = xstart;	p1.y = ystart;
								p2.x = xend;	p2.y = yend;
								dc.Arc(&rect,p1,p2);
							}
								
							//确定终点坐标(x3,y3)
							x3 = m_fCurrentSnapX; y3 = m_fCurrentSnapY;
							Get_VPort_XY(&xend,&yend,x3,y3);
							
							//dc.SelectObject(&linepen);
							dc.MoveTo(xcenter,ycenter);
							dc.LineTo(xend,yend);
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
							//dc.SelectObject(&linepen);
							dc.MoveTo(xcenter,ycenter);
							dc.LineTo(xend,yend);
							rect.left = xcenter - r/m_fScale;
							rect.top = ycenter - r/m_fScale;
							rect.right = xcenter + r/m_fScale;
							rect.bottom = ycenter + r/m_fScale;
							p1.x = xstart;	p1.y = ystart;
							p2.x = xend;	p2.y = yend;
							dc.Arc(&rect,p1,p2);

							//确定alpha2
							if(((x3-x1)==0)&&(y3-y1)<0) alpha2 = 270;
							else if(((x3-x1)==0)&&(y3-y1)>=0) alpha2 = 90;
							else if(((y3-y1)==0)&&(x3-x1)<0) alpha2 = 180;
							else if(((y3-y1)==0)&&(x3-x1)>=0) alpha2 = 0;
							else alpha2 = (180.0/fEDA_pi)*atan2((y3-y1),(x3-x1));

							alpha2 = pDoc->Get_Correct_Angle(alpha2);

							COb_scharc* parc = new COb_scharc;
							parc->pDocument = pDoc;
							parc->m_fCx = x1;
							parc->m_fCy = y1;
							parc->m_fRadius = r;
							parc->m_fSangle = alpha1;
							parc->m_fEangle = alpha2;
							parc->m_nWidth = cEDA_scharc.m_nWidth;
							parc->m_nColor = cEDA_scharc.m_nColor;
							parc->m_nDiagram = m_nDiagram;

							pos = pDoc->m_listSchObject[m_nDiagram].AddTail(parc);

							dc.SetROP2(drawmode);
							CloseMouseCursor();
							parc->Draw(&dc, this);
							OpenMouseCursor();
							dc.SetROP2(R2_NOTXORPEN);
							
							//----------- Undo ---------------------------------------------------
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = SCH_ELEMENT_ARC;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = pos;
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							//--------------------------------------------------------------------

							goto PLACEARC;	
						}
						else if(ch1 == TAB)
						{
							CDlg_EditArc dlg;
							dlg.m_nComboWidth = cEDA_scharc.m_nWidth;
							dlg.m_nColor = cEDA_scharc.m_nColor;
							if(dlg.DoModal() == IDOK)
							{
								cEDA_scharc.m_nWidth = dlg.m_nComboWidth;
								cEDA_scharc.m_nColor = dlg.m_nColor;
	
								delete OverwritePen;
								OverwritePen = new CPen(PS_SOLID,1,cEDA_scharc.m_nColor);
								dc.SelectObject(OverwritePen);
							}
						}
					}while(1);
				}
				else if(ch1 == TAB)
				{
					CDlg_EditArc dlg;
					dlg.m_nComboWidth = cEDA_scharc.m_nWidth;
					dlg.m_nColor = cEDA_scharc.m_nColor;
					if(dlg.DoModal() == IDOK)
					{
						cEDA_scharc.m_nWidth = dlg.m_nComboWidth;
						cEDA_scharc.m_nColor = dlg.m_nColor;

						delete OverwritePen;
						OverwritePen = new CPen(PS_SOLID,1,cEDA_scharc.m_nColor);
						dc.SelectObject(OverwritePen);
					}
				}
			}while(1);
		}
		else if(ch1 == TAB)
		{
			CDlg_EditArc dlg;
			dlg.m_nComboWidth = cEDA_scharc.m_nWidth;
			dlg.m_nColor = cEDA_scharc.m_nColor;
			if(dlg.DoModal() == IDOK)
			{
				cEDA_scharc.m_nWidth = dlg.m_nComboWidth;
				cEDA_scharc.m_nColor = dlg.m_nColor;

				delete OverwritePen;
				OverwritePen = new CPen(PS_SOLID,1,cEDA_scharc.m_nColor);
				dc.SelectObject(OverwritePen);
			}
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);

	delete OverwritePen;

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	//CL_Auto_Pan=OFF;
	//DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	//DrawStructButton(ID_EDIT_PLACE_ARCCENTER,FALSE);
	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);
}

void CESDView::OnPlaceArcedged()
{
	int ch1;
	double x1, y1, x2, y2, x3, y3, cx, cy, r;
	int xend, yend, xstart, ystart, xcenter, ycenter;
	double alpha2,alpha1;
	
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	char szPlaceArcStart[] = "放置圆弧: <左键><回车>定义起点, <右键><ESC>结束命令";
	char szPlaceArcEnd[] = "放置圆弧: <左键><回车>定义终点, <右键><ESC>返回上一步";
	char szPlaceArc[] = "放置圆弧: <左键><回车>定义圆弧形状, <右键><ESC>返回第一步";
	char szInfo[] = "<TAB>属性";

	POSITION pos;
	RECT rect;
	POINT p1,p2;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);
	
	CPen *OverwritePen, *oldpen;
	OverwritePen = new CPen(PS_SOLID,1,cEDA_scharc.m_nColor);
	oldpen = dc.SelectObject(OverwritePen);

PLACEARC:
	Write_Help(szPlaceArcStart);  //定义终点x2,y2,xend,yend
	Write_Info(szInfo);

	nEDA_RedrawFlag = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			x2 = m_fCurrentSnapX;  y2 = m_fCurrentSnapY;
			Get_VPort_XY(&xend,&yend,x2,y2);
			//先定义起点和终点相同，不然会出现未定义先使用的错误
			x1 = x2; y1 = y2;
			xstart = xend; ystart = yend;
			//半径,圆心
			r = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))/2;
			cx = (x2 + x1)/2;
			cy = (y2 + y1)/2;
			Write_Help("定义圆弧的另一个端点");  //定义x1,y1,xstart,ystart

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
						rect.left = (xend+xstart)/2 - r/m_fScale;
						rect.top = (yend+ystart)/2 - r/m_fScale;
						rect.right = (xend+xstart)/2 + r/m_fScale;
						rect.bottom = (yend+ystart)/2 + r/m_fScale;
						p1.x = xstart; p1.y = ystart;
						p2.x = xend; p2.y = yend;
						dc.Arc(&rect,p1,p2);
					}
					x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
					Get_VPort_XY(&xstart,&ystart,x1,y1);
					r = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2))/2;
					cx = (x2 + x1)/2;
					cy = (y2 + y1)/2;
					//先假设圆心在起点终点的连线中间
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
					if( (x1 == x2)&&(y1 == y2)) continue;

					rect.left = (xend+xstart)/2 - r/m_fScale;
					rect.top = (yend+ystart)/2 - r/m_fScale;
					rect.right = (xend+xstart)/2 + r/m_fScale;
					rect.bottom = (yend+ystart)/2 + r/m_fScale;
					p1.x = xstart; p1.y = ystart;
					p2.x = xend; p2.y = yend;
					dc.Arc(&rect,p1,p2);
						
	
					//预先定义x3,y3，圆心和半径也是为了避免未定义先使用的错误
					//注意，圆心在x2,x1连线的中垂线上
					x3 = x1; y3 = y1;
					if( x1 == x2) //垂直线
					{
						cx = x3;
						cy = (y2+y1)/2;
					}
					else if(y1 == y2) //水平线
					{
						cx = (x2+x1)/2;
						cy = y3;
					}
					else 
					{
						double k1,k2, k11, k22;
						k1 = (y1-y2)/(x1-x2);		//起点终点连线斜率，其中垂线斜率是 -1/k
						k2 = -1/k1;								//中垂线斜率是 -1/k
						int xz,yz;								//起点终点连线的中点坐标
						xz = (x2+x1)/2;
						yz = (y2+y1)/2;
						// 直线1    y-y3 = k1(x-x3) 或者  k11(y-y3) = x-x3
						// 直线2    y-yz = k2(x-xz) 或者  k22(y-yz) = x-xz
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
								rect.left = xcenter - r/m_fScale;
								rect.top = ycenter - r/m_fScale;
								rect.right = xcenter + r/m_fScale;
								rect.bottom = ycenter + r/m_fScale;
								p1.x = xstart; p1.y = ystart;
								p2.x = xend; p2.y = yend;
								dc.Arc(&rect, p1, p2);
							}
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
								k1 = (y1-y2)/(x1-x2);		//起点终点连线斜率，其中垂线斜率是 -1/k
								k2 = -1/k1;								//中垂线斜率是 -1/k
								int xz,yz;								//起点终点连线的中点坐标
								xz = (x2+x1)/2;
								yz = (y2+y1)/2;
								// 直线1    y-y3 = k1(x-x3) 或者  k11(y-y3) = x-x3
								// 直线2    y-yz = k2(x-xz) 或者  k22(y-yz) = x-xz
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
							rect.left = xcenter - r/m_fScale;
							rect.top = ycenter - r/m_fScale;
							rect.right = xcenter + r/m_fScale;
							rect.bottom = ycenter + r/m_fScale;
							p1.x = xstart; p1.y = ystart;
							p2.x = xend; p2.y = yend;
							dc.Arc(&rect, p1, p2);

							//计算alpha1
							if(((cx-x2)==0)&&(cy-y2)<0) alpha1 = 270;
							else if(((cx-x2)==0)&&(cy-y2)>=0) alpha1 = 90;
							else if(((cy-y2)==0)&&(cx-x2)<0) alpha1 = 180;
							else if(((cy-y2)==0)&&(cx-x2)>=0) alpha1 = 0;
							//圆心坐标在前面，不加负号，和place_arccenter比较
							else alpha1 = (180/fEDA_pi)*atan2((y2-cy),(x2-cx));
							alpha1 = pDoc->Get_Correct_Angle(alpha1);
							
							//计算alpha2
							if(((cx-x1)==0)&&(cy-y1)<0) alpha2 = 270;
							else if(((cx-x1)==0)&&(cy-y1)>=0) alpha2 = 90;
							else if(((cy-y1)==0)&&(cx-x1)<0) alpha2 = 180;
							else if(((cy-y1)==0)&&(cx-x1)>=0) alpha2 = 0;
							else alpha2 = (180/fEDA_pi)*atan2((y1-cy),(x1-cx));
							alpha2 = pDoc->Get_Correct_Angle(alpha2);

							COb_scharc* parc = new COb_scharc;
							parc->pDocument = pDoc;
							parc->m_fCx = cx;
							parc->m_fCy = cy;
							parc->m_fRadius = r;
							parc->m_fSangle = alpha2;
							parc->m_fEangle = alpha1;
							parc->m_nWidth = cEDA_scharc.m_nWidth;
							parc->m_nColor = cEDA_scharc.m_nColor;
							parc->m_nDiagram = m_nDiagram;

							pos = pDoc->m_listSchObject[m_nDiagram].AddTail(parc);

							dc.SetROP2(drawmode);
							CloseMouseCursor();
							parc->Draw(&dc, this);
							OpenMouseCursor();
							dc.SetROP2(R2_NOTXORPEN);
							
							//----------- Undo -------------------------------------------------------------------------------------------
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = SCH_ELEMENT_ARC;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = pos;
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							//------------------------------------------------------------------------------------------------------------

							goto PLACEARC;	
						}
						else if(ch1 == TAB)
						{
							CDlg_EditArc dlg;
							dlg.m_nComboWidth = cEDA_scharc.m_nWidth;
							dlg.m_nColor = cEDA_scharc.m_nColor;
							if(dlg.DoModal() == IDOK)
							{
								cEDA_scharc.m_nWidth = dlg.m_nComboWidth;
								cEDA_scharc.m_nColor = dlg.m_nColor;

								delete OverwritePen;
								OverwritePen = new CPen(PS_SOLID,1,cEDA_scharc.m_nColor);
								dc.SelectObject(OverwritePen);
							}
						}
					}while(1);
				}
				else if(ch1 == TAB)
				{
					CDlg_EditArc dlg;
					dlg.m_nComboWidth = cEDA_scharc.m_nWidth;
					dlg.m_nColor = cEDA_scharc.m_nColor;
					if(dlg.DoModal() == IDOK)
					{
						cEDA_scharc.m_nWidth = dlg.m_nComboWidth;
						cEDA_scharc.m_nColor = dlg.m_nColor;

						delete OverwritePen;
						OverwritePen = new CPen(PS_SOLID,1,cEDA_scharc.m_nColor);
						dc.SelectObject(OverwritePen);
					}
				}
			}while(1);
		}
		else if(ch1 == TAB)
		{
			CDlg_EditArc dlg;
			dlg.m_nComboWidth = cEDA_scharc.m_nWidth;
			dlg.m_nColor = cEDA_scharc.m_nColor;
			if(dlg.DoModal() == IDOK)
			{
				cEDA_scharc.m_nWidth = dlg.m_nComboWidth;
				cEDA_scharc.m_nColor = dlg.m_nColor;

				delete OverwritePen;
				OverwritePen = new CPen(PS_SOLID,1,cEDA_scharc.m_nColor);
				dc.SelectObject(OverwritePen);
			}
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	

	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);

	delete OverwritePen;	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	//CL_Auto_Pan=OFF;
	//DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	//DrawStructButton(ID_EDIT_PLACE_ARCCENTER,FALSE);
	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);
}


void CESDView::OnPlacePie()
{
	int ch1;
	double x1,y1,x2,y2;
	int xcenter,ycenter,xstart,ystart;
	int x3,y3,xend,yend;
	double alpha1,alpha2;
	int r;

	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	char szPlaceArcCenter[] = "放置饼形图: <左键><回车>定义圆心, <右键><ESC>结束命令";
	char szPlaceArcStart[] = "放置饼形图: <左键><回车>定义半径和起始角度, <右键><ESC>返回上一步";
	char szPlaceArcEnd[] = "放置饼形图: <左键><回车>定义终止角度, <右键><ESC>返回第一步";
	char szInfo[] = "<TAB>属性";	

	POSITION pos;

	RECT rect;
	POINT p1,p2;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);
	
	CPen *OverwritePen, *oldpen;
	OverwritePen = new CPen(PS_SOLID,1,cEDA_scharc.m_nColor);
	oldpen = dc.SelectObject(OverwritePen);
	
PLACEARC:
	Write_Help(szPlaceArcCenter);
	Write_Info(szInfo);

	nEDA_RedrawFlag = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			//圆心坐标(x1,y1)
			x1 = m_fCurrentSnapX;  y1 = m_fCurrentSnapY;
			Get_VPort_XY(&xcenter,&ycenter,x1,y1);
			x2 = x1; y2 = y1;
			xstart = xcenter; ystart = ycenter;
			r = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
			Write_Help(szPlaceArcStart);


			//dc.SelectObject(&linepen);
			dc.MoveTo(xcenter,ycenter);
			dc.LineTo(xstart,ystart);
			rect.left = xcenter - r/m_fScale;
			rect.top = ycenter - r/m_fScale;
			rect.right = xcenter + r/m_fScale;
			rect.bottom = ycenter + r/m_fScale;
			p1.x = xcenter - r/m_fScale; p1.y = ycenter;
			p2.x = xcenter - r/m_fScale; p2.y = ycenter;
			dc.Arc(&rect,p1,p2);

			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xcenter,&ycenter,x1,y1); //必须更新xs,ys的数值，因为sscale和screen_x, screen_y随时会变化
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
				{ 
					//dc.SelectObject(&linepen);
					dc.MoveTo(xcenter,ycenter);
					dc.LineTo(xstart,ystart);
					rect.left = xcenter - r/m_fScale;
					rect.top = ycenter - r/m_fScale;
					rect.right = xcenter + r/m_fScale;
					rect.bottom = ycenter + r/m_fScale;
					p1.x = xcenter - r/m_fScale; p1.y = ycenter;
					p2.x = xcenter - r/m_fScale; p2.y = ycenter;
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
						//dc.SelectObject(&linepen);
						dc.MoveTo(xcenter,ycenter);
						dc.LineTo(xstart,ystart);
						
						rect.left = xcenter - r/m_fScale;
						rect.top = ycenter - r/m_fScale;
						rect.right = xcenter + r/m_fScale;
						rect.bottom = ycenter + r/m_fScale;
						p1.x = xcenter - r/m_fScale; p1.y = ycenter;
						p2.x = xcenter - r/m_fScale; p2.y = ycenter;
						dc.Arc(&rect,p1,p2);
					}
					//确定起点坐标，半径
					x2 = m_fCurrentSnapX; y2 = m_fCurrentSnapY;
					Get_VPort_XY(&xstart,&ystart,x2,y2);
					r = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
						
					//dc.SelectObject(&linepen);
					dc.MoveTo(xcenter,ycenter);
					dc.LineTo(xstart,ystart);
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
					if((int)r == 0) continue;

					//dc.SelectObject(&linepen);
					dc.MoveTo(xcenter,ycenter);
					dc.LineTo(xstart,ystart);
					rect.left = xcenter - r/m_fScale;
					rect.top = ycenter - r/m_fScale;
					rect.right = xcenter + r/m_fScale;
					rect.bottom = ycenter + r/m_fScale;
					p1.x = xcenter - r/m_fScale; p1.y = ycenter;
					p2.x = xcenter - r/m_fScale; p2.y = ycenter;
					dc.Arc(&rect,p1,p2);

					// 确定起点坐标(x2,y2), alpha1
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


					//dc.SelectObject(&linepen);	
					dc.MoveTo(xcenter,ycenter);
					dc.LineTo(xend,yend);
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
						//更新(x1,y1),(x2,y2)的屏幕坐标
						Get_VPort_XY(&xcenter,&ycenter,x1,y1);
						Get_VPort_XY(&xstart,&ystart,x2,y2);
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
						{
							//dc.SelectObject(&linepen);
							dc.MoveTo(xcenter,ycenter);
							dc.LineTo(xend,yend);
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
								//dc.SelectObject(&linepen);
								dc.MoveTo(xcenter,ycenter);
								dc.LineTo(xend,yend);
								rect.left = xcenter - r/m_fScale;
								rect.top = ycenter - r/m_fScale;
								rect.right = xcenter + r/m_fScale;
								rect.bottom = ycenter + r/m_fScale;
								p1.x = xstart;	p1.y = ystart;
								p2.x = xend;	p2.y = yend;
								dc.Arc(&rect,p1,p2);
							}
								
							//确定终点坐标(x3,y3)
							x3 = m_fCurrentSnapX; y3 = m_fCurrentSnapY;
							Get_VPort_XY(&xend,&yend,x3,y3);
							
							//dc.SelectObject(&linepen);
							dc.MoveTo(xcenter,ycenter);
							dc.LineTo(xend,yend);
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
							//dc.SelectObject(&linepen);
							dc.MoveTo(xcenter,ycenter);
							dc.LineTo(xend,yend);
							rect.left = xcenter - r/m_fScale;
							rect.top = ycenter - r/m_fScale;
							rect.right = xcenter + r/m_fScale;
							rect.bottom = ycenter + r/m_fScale;
							p1.x = xstart;	p1.y = ystart;
							p2.x = xend;	p2.y = yend;
							dc.Arc(&rect,p1,p2);

							//确定alpha2
							if(((x3-x1)==0)&&(y3-y1)<0) alpha2 = 270;
							else if(((x3-x1)==0)&&(y3-y1)>=0) alpha2 = 90;
							else if(((y3-y1)==0)&&(x3-x1)<0) alpha2 = 180;
							else if(((y3-y1)==0)&&(x3-x1)>=0) alpha2 = 0;
							else alpha2 = (180.0/fEDA_pi)*atan2((y3-y1),(x3-x1));

							alpha2 = pDoc->Get_Correct_Angle(alpha2);

							COb_schpie* ppie = new COb_schpie;
							ppie->pDocument = pDoc;
							ppie->m_fCx = x1;
							ppie->m_fCy = y1;
							ppie->m_fRadius = r;
							ppie->m_fSangle = alpha1;
							ppie->m_fEangle = alpha2;
							ppie->m_nBorderWidth = cEDA_schpie.m_nBorderWidth;
							ppie->m_nBorderColor = cEDA_schpie.m_nBorderColor;
							ppie->m_nFillColor = cEDA_schpie.m_nFillColor;
							ppie->m_bFillSolid = cEDA_schpie.m_bFillSolid;
							ppie->m_nDiagram = m_nDiagram;

							pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ppie);

							dc.SetROP2(drawmode);
							CloseMouseCursor();
							ppie->Draw(&dc, this);
							OpenMouseCursor();
							dc.SetROP2(R2_NOTXORPEN);
							
							//----------- Undo ------------------------------------------------------------------------------------
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = SCH_ELEMENT_PIE;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = pos;
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							//-------------------------------------------------------------------------------------------------------

							goto PLACEARC;	
						}
						else if(ch1 == TAB)
						{
							CDlg_EditPie dlg;
							dlg.m_nComboWidth = cEDA_schpie.m_nBorderWidth;
							dlg.m_bCheckDrawSolid = cEDA_schpie.m_bFillSolid;
							dlg.m_nFillColor = cEDA_schpie.m_nFillColor;
							dlg.m_nBorderColor = cEDA_schpie.m_nBorderColor;
							
							if(dlg.DoModal() == IDOK)
							{
								cEDA_schpie.m_nBorderWidth = dlg.m_nComboWidth;
								cEDA_schpie.m_bFillSolid = dlg.m_bCheckDrawSolid;
								cEDA_schpie.m_nFillColor = dlg.m_nFillColor;
								cEDA_schpie.m_nBorderColor = dlg.m_nBorderColor;
								
								delete OverwritePen;
								OverwritePen = new CPen(PS_SOLID,1,cEDA_schpie.m_nBorderColor);
								dc.SelectObject(OverwritePen);
							}
						}
					}while(1);
				}
				else if(ch1 == TAB)
				{
					CDlg_EditPie dlg;
					dlg.m_nComboWidth = cEDA_schpie.m_nBorderWidth;
					dlg.m_bCheckDrawSolid = cEDA_schpie.m_bFillSolid;
					dlg.m_nFillColor = cEDA_schpie.m_nFillColor;
					dlg.m_nBorderColor = cEDA_schpie.m_nBorderColor;
					
					if(dlg.DoModal() == IDOK)
					{
						cEDA_schpie.m_nBorderWidth = dlg.m_nComboWidth;
						cEDA_schpie.m_bFillSolid = dlg.m_bCheckDrawSolid;
						cEDA_schpie.m_nFillColor = dlg.m_nFillColor;
						cEDA_schpie.m_nBorderColor = dlg.m_nBorderColor;
						
						delete OverwritePen;
						OverwritePen = new CPen(PS_SOLID,1,cEDA_schpie.m_nBorderColor);
						dc.SelectObject(OverwritePen);
					}
				}
			}while(1);
		}
		else if(ch1 == TAB)
		{
			CDlg_EditPie dlg;
			dlg.m_nComboWidth = cEDA_schpie.m_nBorderWidth;
			dlg.m_bCheckDrawSolid = cEDA_schpie.m_bFillSolid;
			dlg.m_nFillColor = cEDA_schpie.m_nFillColor;
			dlg.m_nBorderColor = cEDA_schpie.m_nBorderColor;
			
			if(dlg.DoModal() == IDOK)
			{
				cEDA_schpie.m_nBorderWidth = dlg.m_nComboWidth;
				cEDA_schpie.m_bFillSolid = dlg.m_bCheckDrawSolid;
				cEDA_schpie.m_nFillColor = dlg.m_nFillColor;
				cEDA_schpie.m_nBorderColor = dlg.m_nBorderColor;
				
				delete OverwritePen;
				OverwritePen = new CPen(PS_SOLID,1,cEDA_schpie.m_nBorderColor);
				dc.SelectObject(OverwritePen);
			}
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);

	delete OverwritePen;

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	//CL_Auto_Pan=OFF;
	//DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	//DrawStructButton(ID_EDIT_PLACE_ARCCENTER,FALSE);
	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);
}

void CESDView::OnPlaceEarcCenter()
{
	int ch1;
	double x1, y1, x2, y2, x3, y3;
	int xstart, ystart, xend, yend, xcenter, ycenter;
	double alpha1,alpha2;
	double ra, rb;
	int xa,ya;

	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	char szPlaceArcCenter[] = "放置圆弧: <左键><回车>定义中心, <右键><ESC>结束命令";
	char szPlaceArcRadiusX[] = "放置圆弧: <左键><回车>定义X半径, <右键><ESC>返回第一步";
	char szPlaceArcRadiusY[] = "放置圆弧: <左键><回车>定义Y半径, <右键><ESC>返回第一步";
	char szPlaceArcStart[] = "放置圆弧: <左键><回车>定义起始角度, <右键><ESC>返回第一步";
	char szPlaceArcEnd[] = "放置圆弧: <左键><回车>定义终止角度, <右键><ESC>返回第一步";
	char szInfo[] = "<TAB>属性";

	POSITION pos;

	RECT rect;
	POINT p1,p2;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);
	CPen *OverwritePen, *oldpen;
	OverwritePen = new CPen(PS_SOLID, 1, cEDA_schellipse.m_nColor);
	oldpen = dc.SelectObject(OverwritePen);

PLACEARC:
	Write_Help(szPlaceArcCenter);
	Write_Info(szInfo);
	nEDA_RedrawFlag = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
			Get_VPort_XY(&xcenter,&ycenter,x1,y1);
			x2 = x1;
			y2 = y1;
			xa = xcenter;
			ya = ycenter;
			ra = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
			rb = ra/5;
			Write_Help(szPlaceArcRadiusX);

			rect.left = xcenter - ra/m_fScale;
			rect.top = ycenter - rb/m_fScale;
			rect.right = xcenter + ra/m_fScale;
			rect.bottom = ycenter + rb/m_fScale;
			p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
			p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
			dc.Arc(&rect,p1,p2);

			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xcenter,&ycenter,x1,y1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					rect.left = xcenter - ra/m_fScale;
					rect.top = ycenter - rb/m_fScale;
					rect.right = xcenter + ra/m_fScale;
					rect.bottom = ycenter + rb/m_fScale;
					p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
					p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
					dc.Arc(&rect,p1,p2);
					ch1 = MOUSEMOVE;
					Write_Help(szPlaceArcCenter);
					break;
				}
				if(ch1==MOUSEMOVE)
				{
					if(nEDA_RedrawFlag==1){ nEDA_RedrawFlag=0;}
					else 
					{ 
						rect.left = xcenter - ra/m_fScale;
						rect.top = ycenter - rb/m_fScale;
						rect.right = xcenter + ra/m_fScale;
						rect.bottom = ycenter + rb/m_fScale;
						p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
						p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
						dc.Arc(&rect,p1,p2);		
					}
					x2 = m_fCurrentSnapX; y2 = m_fCurrentSnapY;
					ra = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
					rb = ra/5;

					rect.left = xcenter - ra/m_fScale;
					rect.top = ycenter - rb/m_fScale;
					rect.right = xcenter + ra/m_fScale;
					rect.bottom = ycenter + rb/m_fScale;
					p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
					p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
					dc.Arc(&rect,p1,p2);
				}
				if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{  
					if( ra == 0) continue;

					rect.left = xcenter - ra/m_fScale;
					rect.top = ycenter - rb/m_fScale;
					rect.right = xcenter + ra/m_fScale;
					rect.bottom = ycenter + rb/m_fScale;
					p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
					p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
					dc.Arc(&rect,p1,p2);

					x2 = m_fCurrentSnapX; y2 = m_fCurrentSnapY;
					rb = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
					Write_Help(szPlaceArcRadiusY);

					rect.left = xcenter - ra/m_fScale;
					rect.top = ycenter - rb/m_fScale;
					rect.right = xcenter + ra/m_fScale;
					rect.bottom = ycenter + rb/m_fScale;
					p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
					p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
					dc.Arc(&rect,p1,p2);
					do
					{
						ch1 = MouseKey();
						Get_VPort_XY(&xcenter,&ycenter,x1,y1);
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							rect.left = xcenter - ra/m_fScale;
							rect.top = ycenter - rb/m_fScale;
							rect.right = xcenter + ra/m_fScale;
							rect.bottom = ycenter + rb/m_fScale;
							p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
							p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
							dc.Arc(&rect,p1,p2);
							ch1 = MOUSEMOVE;
							Write_Help(szPlaceArcCenter);
							goto PLACEARC;
						}
						if(ch1==MOUSEMOVE)
						{
							if(nEDA_RedrawFlag==1){ nEDA_RedrawFlag=0;}
							else 
							{ 
								rect.left = xcenter - ra/m_fScale;
								rect.top = ycenter - rb/m_fScale;
								rect.right = xcenter + ra/m_fScale;
								rect.bottom = ycenter + rb/m_fScale;
								p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
								p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
								dc.Arc(&rect,p1,p2);		
							}
							x2 = m_fCurrentSnapX; y2 = m_fCurrentSnapY;
							rb = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

							rect.left = xcenter - ra/m_fScale;
							rect.top = ycenter - rb/m_fScale;
							rect.right = xcenter + ra/m_fScale;
							rect.bottom = ycenter + rb/m_fScale;
							p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
							p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
							dc.Arc(&rect,p1,p2);
						}
						if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
						{  
							if( rb == 0) continue;

							rect.left = xcenter - ra/m_fScale;
							rect.top = ycenter - rb/m_fScale;
							rect.right = xcenter + ra/m_fScale;
							rect.bottom = ycenter + rb/m_fScale;
							p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
							p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
							dc.Arc(&rect,p1,p2);

							//起始角度
							x2 = m_fCurrentSnapX;  y2 = m_fCurrentSnapY;
							Write_Help(szPlaceArcStart);

							xstart = xcenter;
							ystart = ycenter;

							dc.MoveTo(xcenter,ycenter);
							dc.LineTo(xstart,ystart);
							rect.left = xcenter - ra/m_fScale;
							rect.top = ycenter - rb/m_fScale;
							rect.right = xcenter + ra/m_fScale;
							rect.bottom = ycenter + rb/m_fScale;
							p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
							p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
							dc.Arc(&rect,p1,p2);

							do
							{
								ch1 = MouseKey();
								Get_VPort_XY(&xcenter,&ycenter,x1,y1); //必须更新xs,ys的数值，因为sscale和screen_x, screen_y随时会变化
								if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
								{ 
									dc.MoveTo(xcenter,ycenter);
									dc.LineTo(xstart,ystart);
									rect.left = xcenter - ra/m_fScale;
									rect.top = ycenter - rb/m_fScale;
									rect.right = xcenter + ra/m_fScale;
									rect.bottom = ycenter + rb/m_fScale;
									p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
									p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
									dc.Arc(&rect,p1,p2);
									ch1 = MOUSEMOVE;
									Write_Help(szPlaceArcCenter);
									goto PLACEARC;
								}
								else if(ch1 == MOUSEMOVE)
								{
									if(nEDA_RedrawFlag == 1){ nEDA_RedrawFlag = 0;}
									else  
									{ 
										dc.MoveTo(xcenter,ycenter);
										dc.LineTo(xstart,ystart);
										
										rect.left = xcenter - ra/m_fScale;
										rect.top = ycenter - rb/m_fScale;
										rect.right = xcenter + ra/m_fScale;
										rect.bottom = ycenter + rb/m_fScale;
										p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
										p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
										dc.Arc(&rect,p1,p2);
									}
									//确定起点坐标
									x2 = m_fCurrentSnapX; y2 = m_fCurrentSnapY;
									Get_VPort_XY(&xstart,&ystart,x2,y2);
										
									dc.MoveTo(xcenter,ycenter);
									dc.LineTo(xstart,ystart);
									rect.left = xcenter - ra/m_fScale;
									rect.top = ycenter - rb/m_fScale;
									rect.right = xcenter + ra/m_fScale;
									rect.bottom = ycenter + rb/m_fScale;
									p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
									p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
									dc.Arc(&rect,p1,p2);
								}
								else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
								{
									dc.MoveTo(xcenter,ycenter);
									dc.LineTo(xstart,ystart);
									rect.left = xcenter - ra/m_fScale;
									rect.top = ycenter - rb/m_fScale;
									rect.right = xcenter + ra/m_fScale;
									rect.bottom = ycenter + rb/m_fScale;
									p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
									p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
									dc.Arc(&rect,p1,p2);

									// 确定起点坐标(x2,y2), alpha1
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


									dc.MoveTo(xcenter,ycenter);
									dc.LineTo(xend,yend);
									rect.left = xcenter - ra/m_fScale;
									rect.top = ycenter - rb/m_fScale;
									rect.right = xcenter + ra/m_fScale;
									rect.bottom = ycenter + rb/m_fScale;
									p1.x = xstart;	p1.y = ystart;
									p2.x = xend;	p2.y = yend;
									dc.Arc(&rect,p1,p2);

									do
									{
										ch1 = MouseKey();
										//更新(x1,y1),(x2,y2)的屏幕坐标
										Get_VPort_XY(&xcenter,&ycenter,x1,y1);
										Get_VPort_XY(&xstart,&ystart,x2,y2);
										if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
										{
											dc.MoveTo(xcenter,ycenter);
											dc.LineTo(xend,yend);
											rect.left = xcenter - ra/m_fScale;
											rect.top = ycenter - rb/m_fScale;
											rect.right = xcenter + ra/m_fScale;
											rect.bottom = ycenter + rb/m_fScale;
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
												dc.MoveTo(xcenter,ycenter);
												dc.LineTo(xend,yend);
												rect.left = xcenter - ra/m_fScale;
												rect.top = ycenter - rb/m_fScale;
												rect.right = xcenter + ra/m_fScale;
												rect.bottom = ycenter + rb/m_fScale;
												p1.x = xstart;	p1.y = ystart;
												p2.x = xend;	p2.y = yend;
												dc.Arc(&rect,p1,p2);
											}
												
											//确定终点坐标(x3,y3)
											x3 = m_fCurrentSnapX; y3 = m_fCurrentSnapY;
											Get_VPort_XY(&xend,&yend,x3,y3);
											
											dc.MoveTo(xcenter,ycenter);
											dc.LineTo(xend,yend);
											rect.left = xcenter - ra/m_fScale;
											rect.top = ycenter - rb/m_fScale;
											rect.right = xcenter + ra/m_fScale;
											rect.bottom = ycenter + rb/m_fScale;
											p1.x = xstart;	p1.y = ystart;
											p2.x = xend;	p2.y = yend;
											dc.Arc(&rect,p1,p2);								
										}
										else if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
										{
											dc.MoveTo(xcenter,ycenter);
											dc.LineTo(xend,yend);
											rect.left = xcenter - ra/m_fScale;
											rect.top = ycenter - rb/m_fScale;
											rect.right = xcenter + ra/m_fScale;
											rect.bottom = ycenter + rb/m_fScale;
											p1.x = xstart;	p1.y = ystart;
											p2.x = xend;	p2.y = yend;
											dc.Arc(&rect,p1,p2);

											//确定alpha2
											if(((x3-x1)==0)&&(y3-y1)<0) alpha2 = 270;
											else if(((x3-x1)==0)&&(y3-y1)>=0) alpha2 = 90;
											else if(((y3-y1)==0)&&(x3-x1)<0) alpha2 = 180;
											else if(((y3-y1)==0)&&(x3-x1)>=0) alpha2 = 0;
											else alpha2 = (180.0/fEDA_pi)*atan2((y3-y1),(x3-x1));
											alpha2 = pDoc->Get_Correct_Angle(alpha2);

											COb_schellipse* pellipse = new COb_schellipse;
											pellipse->pDocument = pDoc;
											pellipse->m_fCx = x1;
											pellipse->m_fCy = y1;
											pellipse->m_fRadius_x = ra;
											pellipse->m_fRadius_y = rb;
											pellipse->m_fSangle = alpha1;
											pellipse->m_fEangle = alpha2;
											pellipse->m_nWidth = cEDA_schellipse.m_nWidth;
											pellipse->m_nColor = cEDA_schellipse.m_nColor;
											pellipse->m_nDiagram = m_nDiagram;

											pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pellipse);

											dc.SetROP2(drawmode);
											CloseMouseCursor();

											pellipse->Draw(&dc, this);
											OpenMouseCursor();
											dc.SetROP2(R2_NOTXORPEN);
											
											//----------- Undo -------------------------------------------------------------------------------------
											if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
											{
												pDoc->Clear_Discarded_Undo_Buffer();
											}
											clear_undo_buffer_flag = TRUE;
											CSchUndo *pundo = new CSchUndo;
											pundo->m_nOp = UNDO_DELETE;
											pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
											pundo->m_nDiagram = m_nDiagram;
											pundo->m_nPos = pos;
											pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
											undo_flag = TRUE;
											//-------------------------------------------------------------------------------------------------------

											goto PLACEARC;	
										}
										else if(ch1 == TAB)
										{
											CDlg_EditEllipse dlg;
											dlg.m_nComboWidth = cEDA_schellipse.m_nWidth;
											dlg.m_nColor = cEDA_schellipse.m_nColor;
											if(dlg.DoModal() == IDOK)
											{
												cEDA_schellipse.m_nWidth = dlg.m_nComboWidth;
												cEDA_schellipse.m_nColor = dlg.m_nColor;

												delete OverwritePen;
												OverwritePen = new CPen(PS_SOLID,1,cEDA_schellipse.m_nColor);
												dc.SelectObject(OverwritePen);
											}
										}
									}while(1);
								}
								else if(ch1 == TAB)
								{
									CDlg_EditEllipse dlg;
									dlg.m_nComboWidth = cEDA_schellipse.m_nWidth;
									dlg.m_nColor = cEDA_schellipse.m_nColor;
									if(dlg.DoModal() == IDOK)
									{
										cEDA_schellipse.m_nWidth = dlg.m_nComboWidth;
										cEDA_schellipse.m_nColor = dlg.m_nColor;

										delete OverwritePen;
										OverwritePen = new CPen(PS_SOLID,1,cEDA_schellipse.m_nColor);
										dc.SelectObject(OverwritePen);
									}
								}
							}while(1);
						}
						else  if(ch1 == TAB)
						{
							CDlg_EditEllipse dlg;
							dlg.m_nComboWidth = cEDA_schellipse.m_nWidth;
							dlg.m_nColor = cEDA_schellipse.m_nColor;
							if(dlg.DoModal() == IDOK)
							{
								cEDA_schellipse.m_nWidth = dlg.m_nComboWidth;
								cEDA_schellipse.m_nColor = dlg.m_nColor;

								delete OverwritePen;
								OverwritePen = new CPen(PS_SOLID,1,cEDA_schellipse.m_nColor);
								dc.SelectObject(OverwritePen);
							}
						}
					}while(1);
				}
				else if(ch1 == TAB)
				{
					CDlg_EditEllipse dlg;
					dlg.m_nComboWidth = cEDA_schellipse.m_nWidth;
					dlg.m_nColor = cEDA_schellipse.m_nColor;
					if(dlg.DoModal() == IDOK)
					{
						cEDA_schellipse.m_nWidth = dlg.m_nComboWidth;
						cEDA_schellipse.m_nColor = dlg.m_nColor;

						delete OverwritePen;
						OverwritePen = new CPen(PS_SOLID,1,cEDA_schellipse.m_nColor);
						dc.SelectObject(OverwritePen);
					}
				}
			}while(1);
		}
		else if(ch1 == TAB)
		{
			CDlg_EditEllipse dlg;
			dlg.m_nComboWidth = cEDA_schellipse.m_nWidth;
			dlg.m_nColor = cEDA_schellipse.m_nColor;
			if(dlg.DoModal() == IDOK)
			{
				cEDA_schellipse.m_nWidth = dlg.m_nComboWidth;
				cEDA_schellipse.m_nColor = dlg.m_nColor;

				delete OverwritePen;
				OverwritePen = new CPen(PS_SOLID,1,cEDA_schellipse.m_nColor);
				dc.SelectObject(OverwritePen);
			}
		}
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);
	
	delete OverwritePen;	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	//CL_Auto_Pan=OFF;
	//DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	//DrawStructButton(ID_EDIT_PLACE_ARCCENTER,FALSE);
	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);
}

void CESDView::OnPlaceEarcEdged()
{
	int ch1;
	double x1, y1, x2, y2, x3, y3, cx, cy, rx, ry;
	int xstart, ystart, xend, yend, xcenter, ycenter;
	double alpha2,alpha1;
	
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	char szPlaceEarcStart[] = "放置圆弧: <左键><回车>定义圆弧的一个端点, <右键><ESC>结束命令";
	char szPlaceEarcEnd[] = "放置圆弧: <左键><回车>定义圆弧的另一个端点, <右键><ESC>返回上一步";
	char szPlaceEarc[] = "放置圆弧: <左键><回车>定义圆弧形状, <右键><ESC>返回上一步";
	char szInfo[] = "<TAB>属性";
	
	POSITION pos;
	RECT rect;
	POINT p1,p2;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);
	CPen *OverwritePen, *oldpen;
	OverwritePen = new CPen(PS_SOLID, 1, cEDA_schellipse.m_nColor);
	oldpen = dc.SelectObject(OverwritePen);

PLACEARC:
	Write_Help(szPlaceEarcStart);  //定义终点x2,y2,xend,yend
	Write_Info(szInfo);

	nEDA_RedrawFlag = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			x2 = m_fCurrentSnapX;  y2 = m_fCurrentSnapY;
			Get_VPort_XY(&xend,&yend,x2,y2);
			//先定义起点和终点相同，不然会出现未定义先使用的错误
			x1 = x2; y1 = y2;
			xstart = xend; ystart = yend;
			//半径,圆心
			if(x1 == x2)
			{
				cx = x1;
				cy = (y1+y2)/2;
				ry = abs(y1-y2)/2;
				rx = ry/2;
			}
			else if(y1 == y2)
			{
				cx = (x1+x2)/2;
				cy = y1;
				rx = abs(x1-x2)/2;
				ry = rx/2;
			}
			else
			{
				cx = x1;
				cy = y2;
				rx = abs(x1-x2);
				ry = abs(y1-y2);
				if(rx == 0) rx = ry/2;
				if(ry == 0) ry = rx/2;
			}
			Get_VPort_XY(&xcenter,&ycenter,cx,cy);
			

			Write_Help(szPlaceEarcEnd);  //定义x1,y1,xstart,ystart

			rect.left = xcenter - rx/m_fScale;
			rect.top = ycenter - ry/m_fScale;
			rect.right = xcenter + rx/m_fScale;
			rect.bottom = ycenter + ry/m_fScale;
			p1.x = xstart; p1.y = ystart;
			p2.x = xend; p2.y = yend;	
			dc.Arc(&rect,p1,p2);

			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xend,&yend,x2,y2);
				Get_VPort_XY(&xcenter,&ycenter,cx,cy);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
				{ 
					Write_Help(szPlaceEarcStart);
					rect.left = xcenter - rx/m_fScale;
					rect.top = ycenter - ry/m_fScale;
					rect.right = xcenter + rx/m_fScale;
					rect.bottom = ycenter + ry/m_fScale;
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
						rect.left = xcenter - rx/m_fScale;
						rect.top = ycenter - ry/m_fScale;
						rect.right = xcenter + rx/m_fScale;
						rect.bottom = ycenter + ry/m_fScale;
						p1.x = xstart; p1.y = ystart;
						p2.x = xend; p2.y = yend;
						dc.Arc(&rect,p1,p2);
					}
					x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
					Get_VPort_XY(&xstart,&ystart,x1,y1);

					if(x1 == x2)
					{
						cx = x1;
						cy = (y1+y2)/2;
						ry = abs(y1-y2)/2;
						rx = ry/2;
					}
					else if(y1 == y2)
					{
						cx = (x1+x2)/2;
						cy = y1;
						rx = abs(x1-x2)/2;
						ry = rx/2;
					}
					else
					{
						cx = x1;
						cy = y2;
						rx = abs(x1-x2);
						ry = abs(y1-y2);
						if(rx == 0) rx = ry/2;
						if(ry == 0) ry = rx/2;
					}

					Get_VPort_XY(&xcenter,&ycenter,cx,cy);
					
					//先假设圆心在起点终点的连线中间
					rect.left = xcenter - rx/m_fScale;
					rect.top = ycenter - ry/m_fScale;
					rect.right = xcenter + rx/m_fScale;
					rect.bottom = ycenter + ry/m_fScale;
					p1.x = xstart; p1.y = ystart;
					p2.x = xend; p2.y = yend;
					dc.Arc(&rect,p1,p2);					
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					if( (x1 == x2)&&(y1 == y2)) continue;

					rect.left = xcenter - rx/m_fScale;
					rect.top = ycenter - ry/m_fScale;
					rect.right = xcenter + rx/m_fScale;
					rect.bottom = ycenter + ry/m_fScale;
                    p1.x = xstart; p1.y = ystart;
					p2.x = xend; p2.y = yend;
					dc.Arc(&rect,p1,p2);
						
	
					//预先定义x3,y3，圆心和半径也是为了避免未定义先使用的错误
					//使当前坐标为椭圆圆心
					x3 = x1; y3 = y1;

					if(x1 == x2)
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
						cx = x3;
						cy = y3;	
					}
					// (x1,y1), (x2,y2)为通过椭圆的两个点，反求rx,ry
					double f1,f2;
					f1 = (y1-cy)*(y1-cy)-(y2-cy)*(y2-cy);
					f2 = (x1-cx)*(x1-cx)-(x2-cx)*(x2-cx);
					
					rx = sqrt( ( (x2-cx)*(x2-cx)*(y1-cy)*(y1-cy) - (x1-cx)*(x1-cx)*(y2-cy)*(y2-cy))/f1 );
					ry = sqrt( ( (y2-cy)*(y2-cy)*(x1-cx)*(x1-cx) - (y1-cy)*(y1-cy)*(x2-cx)*(x2-cx))/f2 );

					Get_VPort_XY(&xcenter,&ycenter,cx,cy);					

					Write_Help(szPlaceEarc);

					rect.left = xcenter - rx/m_fScale;
					rect.top = ycenter - ry/m_fScale;
					rect.right = xcenter + rx/m_fScale;
					rect.bottom = ycenter + ry/m_fScale;
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
							rect.left = xcenter - rx/m_fScale;
							rect.top = ycenter - ry/m_fScale;
							rect.right = xcenter + rx/m_fScale;
							rect.bottom = ycenter + ry/m_fScale;
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
								rect.left = xcenter - rx/m_fScale;
								rect.top = ycenter - ry/m_fScale;
								rect.right = xcenter + rx/m_fScale;
								rect.bottom = ycenter + ry/m_fScale;
								p1.x = xstart; p1.y = ystart;
								p2.x = xend; p2.y = yend;
								dc.Arc(&rect, p1, p2);
							}
							x3 = m_fCurrentSnapX; y3 = m_fCurrentSnapY;

							double bak_cx, bak_cy;
							bak_cx = cx;
							bak_cy = cy;

							if(x1 == x2)
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
								cx = x3;
								cy = y3;	
							}
							// (x1,y1), (x2,y2)为通过椭圆的两个点，反求rx,ry
							f1 = (y1-cy)*(y1-cy)-(y2-cy)*(y2-cy);
							f2 = (x1-cx)*(x1-cx)-(x2-cx)*(x2-cx);

							double bakx, baky;
							bakx = rx;
							baky = ry;
							rx = sqrt(((x2-cx)*(x2-cx)*(y1-cy)*(y1-cy)-(x1-cx)*(x1-cx)*(y2-cy)*(y2-cy))/f1 );
							ry = sqrt(((y2-cy)*(y2-cy)*(x1-cx)*(x1-cx)-(y1-cy)*(y1-cy)*(x2-cx)*(x2-cx))/f2 );
							if(((int)rx == 0)||((int)ry == 0))
							{
								rx = bakx;
								ry = baky;
								cx = bak_cx;
								cy = bak_cy;
							}
							Get_VPort_XY(&xcenter,&ycenter,cx,cy);

							rect.left = xcenter - rx/m_fScale;
							rect.top = ycenter - ry/m_fScale;
							rect.right = xcenter + rx/m_fScale;
							rect.bottom = ycenter + ry/m_fScale;
							p1.x = xstart; p1.y = ystart;
							p2.x = xend; p2.y = yend;
							dc.Arc(&rect, p1, p2);
						}
						else if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
						{
							if((rx == 0) || (ry == 0)) continue;

							rect.left = xcenter - rx/m_fScale;
							rect.top = ycenter - ry/m_fScale;
							rect.right = xcenter + rx/m_fScale;
							rect.bottom = ycenter + ry/m_fScale;
							p1.x = xstart; p1.y = ystart;
							p2.x = xend; p2.y = yend;
							dc.Arc(&rect, p1, p2);

							//计算alpha2
							if(((cx-x2)==0)&&(cy-y2)<0) alpha1 = 270;
							else if(((cx-x2)==0)&&(cy-y2)>=0) alpha1 = 90;
							else if(((cy-y2)==0)&&(cx-x2)<0) alpha1 = 180;
							else if(((cy-y2)==0)&&(cx-x2)>=0) alpha1 = 0;
							else alpha1 = (180.0/fEDA_pi)*atan2((y2-cy),(x2-cx));
							alpha1 = pDoc->Get_Correct_Angle(alpha1);
							
							//计算alpha1
							if(((cx-x1)==0)&&(cy-y1)<0) alpha2 = 270;
							else if(((cx-x1)==0)&&(cy-y1)>=0) alpha2 = 90;
							else if(((cy-y1)==0)&&(cx-x1)<0) alpha2 = 180;
							else if(((cy-y1)==0)&&(cx-x1)>=0) alpha2 = 0;
							else alpha2 = (180/fEDA_pi)*atan2((y1-cy),(x1-cx));
							alpha2 = pDoc->Get_Correct_Angle(alpha2);

							COb_schellipse* pellipse = new COb_schellipse;
							pellipse->pDocument = pDoc;
							pellipse->m_fCx = cx;
							pellipse->m_fCy = cy;
							pellipse->m_fRadius_x = rx;
							pellipse->m_fRadius_y = ry;
							pellipse->m_fSangle = alpha2;
							pellipse->m_fEangle = alpha1;
							pellipse->m_nWidth = cEDA_schellipse.m_nWidth;
							pellipse->m_nColor = cEDA_schellipse.m_nColor;
							pellipse->m_nDiagram = m_nDiagram;
		
							pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pellipse);

							dc.SetROP2(drawmode);
							CloseMouseCursor();
							pellipse->Draw(&dc, this);
							OpenMouseCursor();
							dc.SetROP2(R2_NOTXORPEN);
								
							//----------- Undo -------------------------------------------------------------------------------------
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = pos;
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							//-------------------------------------------------------------------------------------------------------

							goto PLACEARC;	
						}
						else if(ch1 == TAB)
						{
							CDlg_EditEllipse dlg;
							dlg.m_nComboWidth = cEDA_schellipse.m_nWidth;
							dlg.m_nColor = cEDA_schellipse.m_nColor;
							if(dlg.DoModal() == IDOK)
							{
								cEDA_schellipse.m_nWidth = dlg.m_nComboWidth;
								cEDA_schellipse.m_nColor = dlg.m_nColor;

								delete OverwritePen;
								OverwritePen = new CPen(PS_SOLID,1,cEDA_schellipse.m_nColor);
								dc.SelectObject(OverwritePen);
							}
						}
					}while(1);
				}
				else if(ch1 == TAB)
				{
					CDlg_EditEllipse dlg;
					dlg.m_nComboWidth = cEDA_schellipse.m_nWidth;
					dlg.m_nColor = cEDA_schellipse.m_nColor;
					if(dlg.DoModal() == IDOK)
					{
						cEDA_schellipse.m_nWidth = dlg.m_nComboWidth;
						cEDA_schellipse.m_nColor = dlg.m_nColor;

						delete OverwritePen;
						OverwritePen = new CPen(PS_SOLID,1,cEDA_schellipse.m_nColor);
						dc.SelectObject(OverwritePen);
					}
				}
			}while(1);
		}
		else if(ch1 == TAB)
		{
			CDlg_EditEllipse dlg;
			dlg.m_nComboWidth = cEDA_schellipse.m_nWidth;
			dlg.m_nColor = cEDA_schellipse.m_nColor;
			if(dlg.DoModal() == IDOK)
			{
				cEDA_schellipse.m_nWidth = dlg.m_nComboWidth;
				cEDA_schellipse.m_nColor = dlg.m_nColor;

				delete OverwritePen;
				OverwritePen = new CPen(PS_SOLID,1,cEDA_schellipse.m_nColor);
				dc.SelectObject(OverwritePen);
			}
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);

	delete OverwritePen;	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	//CL_Auto_Pan=OFF;
	//DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	//DrawStructButton(ID_EDIT_PLACE_ARCCENTER,FALSE);
	
	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);
}

void CESDView::OnPlaceEllipticalFill()
{
	int ch1;
	double x1, y1, x2, y2;
	int xcenter, ycenter;
	double ra, rb;
	int xa,ya;

	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	char szPlaceArcCenter[] = "放置椭圆: <左键><回车>定义中心, <右键><ESC>结束命令";
	char szPlaceArcRadiusX[] = "放置椭圆: <左键><回车>定义X半径, <右键><ESC>返回第一步";
	char szPlaceArcRadiusY[] = "放置椭圆: <左键><回车>定义Y半径, <右键><ESC>返回第一步";
	char szInfo[] = "<TAB>属性";

	POSITION pos;

	RECT rect;
	POINT p1,p2;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	MoveCursorToCenter();

	pDoc->SetModifiedFlag(TRUE);

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);
	CPen *OverwritePen, *oldpen;
	OverwritePen = new CPen(PS_SOLID, 1, cEDA_schellipsefill.m_nBorderColor);

	oldpen = dc.SelectObject(OverwritePen);

PLACEARC:
	Write_Help(szPlaceArcCenter);
	Write_Info(szInfo);
	nEDA_RedrawFlag = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
			Get_VPort_XY(&xcenter,&ycenter,x1,y1);
			x2 = x1;
			y2 = y1;
			xa = xcenter;
			ya = ycenter;
			ra = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
			rb = ra/5;
			Write_Help(szPlaceArcRadiusX);

			rect.left = xcenter - ra/m_fScale;
			rect.top = ycenter - rb/m_fScale;
			rect.right = xcenter + ra/m_fScale;
			rect.bottom = ycenter + rb/m_fScale;
			p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
			p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
			dc.Arc(&rect,p1,p2);

			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xcenter,&ycenter,x1,y1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					rect.left = xcenter - ra/m_fScale;
					rect.top = ycenter - rb/m_fScale;
					rect.right = xcenter + ra/m_fScale;
					rect.bottom = ycenter + rb/m_fScale;
					p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
					p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
					dc.Arc(&rect,p1,p2);
					ch1 = MOUSEMOVE;
					Write_Help(szPlaceArcCenter);
					break;
				}
				if(ch1==MOUSEMOVE)
				{
					if(nEDA_RedrawFlag==1){ nEDA_RedrawFlag=0;}
					else 
					{ 
						rect.left = xcenter - ra/m_fScale;
						rect.top = ycenter - rb/m_fScale;
						rect.right = xcenter + ra/m_fScale;
						rect.bottom = ycenter + rb/m_fScale;
						p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
						p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
						dc.Arc(&rect,p1,p2);		
					}
					x2 = m_fCurrentSnapX; y2 = m_fCurrentSnapY;
					ra = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
					rb = ra/5;

					rect.left = xcenter - ra/m_fScale;
					rect.top = ycenter - rb/m_fScale;
					rect.right = xcenter + ra/m_fScale;
					rect.bottom = ycenter + rb/m_fScale;
					p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
					p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
					dc.Arc(&rect,p1,p2);
				}
				if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{  
					if( ra == 0) continue;

					rect.left = xcenter - ra/m_fScale;
					rect.top = ycenter - rb/m_fScale;
					rect.right = xcenter + ra/m_fScale;
					rect.bottom = ycenter + rb/m_fScale;
					p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
					p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
					dc.Arc(&rect,p1,p2);

					x2 = m_fCurrentSnapX; y2 = m_fCurrentSnapY;
					rb = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
					Write_Help(szPlaceArcRadiusY);

					rect.left = xcenter - ra/m_fScale;
					rect.top = ycenter - rb/m_fScale;
					rect.right = xcenter + ra/m_fScale;
					rect.bottom = ycenter + rb/m_fScale;
					p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
					p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
					dc.Arc(&rect,p1,p2);
					do
					{
						ch1 = MouseKey();
						Get_VPort_XY(&xcenter,&ycenter,x1,y1);
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							rect.left = xcenter - ra/m_fScale;
							rect.top = ycenter - rb/m_fScale;
							rect.right = xcenter + ra/m_fScale;
							rect.bottom = ycenter + rb/m_fScale;
							p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
							p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
							dc.Arc(&rect,p1,p2);
							ch1 = MOUSEMOVE;
							Write_Help(szPlaceArcCenter);
							goto PLACEARC;
						}
						if(ch1==MOUSEMOVE)
						{
							if(nEDA_RedrawFlag==1){ nEDA_RedrawFlag=0;}
							else 
							{ 
								rect.left = xcenter - ra/m_fScale;
								rect.top = ycenter - rb/m_fScale;
								rect.right = xcenter + ra/m_fScale;
								rect.bottom = ycenter + rb/m_fScale;
								p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
								p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
								dc.Arc(&rect,p1,p2);		
							}
							x2 = m_fCurrentSnapX; y2 = m_fCurrentSnapY;
							rb = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));

							rect.left = xcenter - ra/m_fScale;
							rect.top = ycenter - rb/m_fScale;
							rect.right = xcenter + ra/m_fScale;
							rect.bottom = ycenter + rb/m_fScale;
							p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
							p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
							dc.Arc(&rect,p1,p2);
						}
						if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
						{  
							if( rb == 0) continue;

							rect.left = xcenter - ra/m_fScale;
							rect.top = ycenter - rb/m_fScale;
							rect.right = xcenter + ra/m_fScale;
							rect.bottom = ycenter + rb/m_fScale;
							p1.x = xcenter - ra/m_fScale; p1.y = ycenter;
							p2.x = xcenter - ra/m_fScale; p2.y = ycenter;
							dc.Arc(&rect,p1,p2);

							COb_schellipsefill* pellipsefill = new COb_schellipsefill;
							pellipsefill->pDocument = pDoc;
							pellipsefill->m_fCx = x1;
							pellipsefill->m_fCy = y1;
							pellipsefill->m_fRadius_x = ra;
							pellipsefill->m_fRadius_y = rb;
							pellipsefill->m_nBorderWidth = cEDA_schellipsefill.m_nBorderWidth;
							pellipsefill->m_nBorderColor = cEDA_schellipsefill.m_nBorderColor;
							pellipsefill->m_nFillColor = cEDA_schellipsefill.m_nFillColor;
							pellipsefill->m_bFillSolid = cEDA_schellipsefill.m_bFillSolid;
							pellipsefill->m_bTransparent = cEDA_schellipsefill.m_bTransparent;
							pellipsefill->m_nDiagram = m_nDiagram;

							pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pellipsefill);

							dc.SetROP2(drawmode);
							CloseMouseCursor();

							pellipsefill->Draw(&dc, this);
							OpenMouseCursor();
							dc.SetROP2(R2_NOTXORPEN);
											
							//----------- Undo -------------------------------------------------------------------------------------
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = pos;
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							//-------------------------------------------------------------------------------------------------------

							goto PLACEARC;	
						}
						else  if(ch1 == TAB)
						{
							CDlg_EditEllipseFill dlg;

							dlg.m_bCheckDrawSolid = cEDA_schellipsefill.m_bFillSolid;
							dlg.m_bCheckTransparent = cEDA_schellipsefill.m_bTransparent;
							dlg.m_nComboWidth = cEDA_schellipsefill.m_nBorderWidth;
							dlg.m_nFillColor = cEDA_schellipsefill.m_nFillColor;
							dlg.m_nBorderColor = cEDA_schellipsefill.m_nBorderColor;
							
							if(dlg.DoModal() == IDOK)
							{
								cEDA_schellipsefill.m_bFillSolid = dlg.m_bCheckDrawSolid;
								cEDA_schellipsefill.m_bTransparent = dlg.m_bCheckTransparent;
								cEDA_schellipsefill.m_nBorderWidth = dlg.m_nComboWidth;
								cEDA_schellipsefill.m_nFillColor = dlg.m_nFillColor;
								cEDA_schellipsefill.m_nBorderColor = dlg.m_nBorderColor;

								delete OverwritePen;
								OverwritePen = new CPen(PS_SOLID,1,cEDA_schellipsefill.m_nBorderColor);
								dc.SelectObject(OverwritePen);
							}
						}
					}while(1);
				}
				else if(ch1 == TAB)
				{
					CDlg_EditEllipseFill dlg;

					dlg.m_bCheckDrawSolid = cEDA_schellipsefill.m_bFillSolid;
					dlg.m_bCheckTransparent = cEDA_schellipsefill.m_bTransparent;
					dlg.m_nComboWidth = cEDA_schellipsefill.m_nBorderWidth;
					dlg.m_nFillColor = cEDA_schellipsefill.m_nFillColor;
					dlg.m_nBorderColor = cEDA_schellipsefill.m_nBorderColor;
					
					if(dlg.DoModal() == IDOK)
					{
						cEDA_schellipsefill.m_bFillSolid = dlg.m_bCheckDrawSolid;
						cEDA_schellipsefill.m_bTransparent = dlg.m_bCheckTransparent;
						cEDA_schellipsefill.m_nBorderWidth = dlg.m_nComboWidth;
						cEDA_schellipsefill.m_nFillColor = dlg.m_nFillColor;
						cEDA_schellipsefill.m_nBorderColor = dlg.m_nBorderColor;

						delete OverwritePen;
						OverwritePen = new CPen(PS_SOLID,1,cEDA_schellipsefill.m_nBorderColor);
						dc.SelectObject(OverwritePen);
					}
				}
			}while(1);
		}
		else if(ch1 == TAB)
		{
			CDlg_EditEllipseFill dlg;

			dlg.m_bCheckDrawSolid = cEDA_schellipsefill.m_bFillSolid;
			dlg.m_bCheckTransparent = cEDA_schellipsefill.m_bTransparent;
			dlg.m_nComboWidth = cEDA_schellipsefill.m_nBorderWidth;
			dlg.m_nFillColor = cEDA_schellipsefill.m_nFillColor;
			dlg.m_nBorderColor = cEDA_schellipsefill.m_nBorderColor;
			
			if(dlg.DoModal() == IDOK)
			{
				cEDA_schellipsefill.m_bFillSolid = dlg.m_bCheckDrawSolid;
				cEDA_schellipsefill.m_bTransparent = dlg.m_bCheckTransparent;
				cEDA_schellipsefill.m_nBorderWidth = dlg.m_nComboWidth;
				cEDA_schellipsefill.m_nFillColor = dlg.m_nFillColor;
				cEDA_schellipsefill.m_nBorderColor = dlg.m_nBorderColor;

				delete OverwritePen;
				OverwritePen = new CPen(PS_SOLID,1,cEDA_schellipsefill.m_nBorderColor);
				dc.SelectObject(OverwritePen);
			}
		}
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);
	
	delete OverwritePen;	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);
}

void CESDView::OnPlacePolygon()
{
	int number;
	Fpoint point[10000];
	int i; 
	int ch1;
	double x1,y1,x2,y2,tempx,tempy;
	int xs,ys,xe,ye,flag;
	int tempxs,tempys,tempxe,tempye;
	POSITION pos;

	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	char szPlacePolygon[] = "放置多边形: <左键><回车>定义多边形顶点, <右键><ESC>结束命令";
	char szInfo[] = "<TAB>属性, <ALT>模式";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	MoveCursorToCenter();

	pDoc->SetModifiedFlag(TRUE);
	
	Write_Help(szPlacePolygon);
	Write_Info(szInfo);

	flag = 0;

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);
	CPen *OverwritePen, *oldpen;
	OverwritePen = new CPen(PS_SOLID, 1, cEDA_schpolygon.m_nBorderColor);
	oldpen = dc.SelectObject(OverwritePen);
	
	nEDA_RedrawFlag=0;
	do
	{
		ch1=MouseKey();
		if((ch1==PUSHMOUSE)||(ch1==ENTER))
		{
			x1 = m_fCurrentSnapX;  y1 = m_fCurrentSnapY;
			Get_VPort_XY(&xs,&ys,x1,y1);
			xe = xs;
			ye = ys;
			number = 0;
			point[number].fx = x1;
			point[number].fy = y1;
			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xs,&ys,x1,y1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					if(number < 2)
					{
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ye);
						for(i = 0; i < number; i++)
						{
							Get_VPort_XY(&tempxs,&tempys,point[i].fx,point[i].fy);
							Get_VPort_XY(&tempxe,&tempye,point[i+1].fx,point[i+1].fy);
							dc.MoveTo(tempxs,tempys);
							dc.LineTo(tempxe,tempye);
						}
						break;
					}
					else
					{
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ye);
						for(i=0;i<number;i++)
						{
							Get_VPort_XY(&tempxs,&tempys,point[i].fx,point[i].fy);
							Get_VPort_XY(&tempxe,&tempye,point[i+1].fx,point[i+1].fy);
							dc.MoveTo(tempxs,tempys);
							dc.LineTo(tempxe,tempye);
						}
						if((point[number].fx!=point[0].fx)||\
						(point[number].fy!=point[0].fy))
						{
							number++;
							point[number].fx=point[0].fx;
							point[number].fy=point[0].fy;
						}
						number++;
							
						COb_schpolygon *ppolygon = new COb_schpolygon;
						ppolygon->pDocument = pDoc;
						ppolygon->m_cVertex.SetSize(number);
						for(i=0; i<number; i++)
						{
							ppolygon->m_cVertex[i].fx = point[i].fx;
							ppolygon->m_cVertex[i].fy = point[i].fy;
						}
						ppolygon->m_nBorderColor = cEDA_schpolygon.m_nBorderColor;
						ppolygon->m_nFillColor = cEDA_schpolygon.m_nFillColor;
						ppolygon->m_nBorderWidth = cEDA_schpolygon.m_nBorderWidth;
						ppolygon->m_bFillSolid = cEDA_schpolygon.m_bFillSolid;
						ppolygon->m_bTransparent = cEDA_schpolygon.m_bTransparent;
						ppolygon->m_nDiagram = m_nDiagram;

						pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ppolygon);

						//----------- Undo -------------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_POLYGON;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = pos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//-------------------------------------------------------------------------------------------------------

						dc.SetROP2(drawmode);
						CloseMouseCursor();
						ppolygon->Draw(&dc, this);
						OpenMouseCursor();
						break;
					}
				}
				else if(ch1==MOUSEMOVE)
				{
					if(flag == 0)
					{ 
						flag = 1;
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ye);
					}
					else
					{
						if(nEDA_RedrawFlag==1){ }
						else  
						{
							dc.MoveTo(xs,ys);
							dc.LineTo(xe,ye);
						}
						x2 = m_fCurrentSnapX;  y2 = m_fCurrentSnapY;
						Get_VPort_XY(&xe,&ye,x2,y2);
						
						if((GetKeyState(VK_MENU)&0x80) != 0)
						{ 
							if((x2>=x1)&&(y2>=y1))
							{
								tempx=x2-x1;  tempy=y2-y1;
								if(tempx>=tempy)
								{ 
									if(tempx<=(tempy+10))
									{
										x2=x1+tempy;
									}
									else
									{ 
										y2=y1;
									}
								}
								else
								{ 
									if(tempy<=(tempx+10))
									{
										y2=y1+tempx;
									}
									else
									{ 
										x2=x1;
									}
								}
							}
							else if((x1>=x2)&&(y2>=y1))
							{  
								tempx=x1-x2;  tempy=y2-y1;
								if(tempx>=tempy)
								{  
									if(tempx<=(tempy+10))
									{
										x2=x1-tempy;
									}
									else
									{  
										y2=y1;
									}
								}
								else
								{ 
									if(tempy<=(tempx+10))
									{
										y2=y1+tempx;
									}
									else
									{ 
										x2=x1;
									}
								}
							}
							else if((x2>=x1)&&(y1>=y2))
							{ 
								tempx=x2-x1;  tempy=y1-y2;
								if(tempx>=tempy)
								{ 
									if(tempx<=(tempy+10))
									{
										x2=x1+tempy;
									}
									else
									{  
										y2=y1;
									}
								}
								else
								{ 
									if(tempy<=(tempx+10))
									{
										y2=y1-tempx;
									}
									else
									{  
										x2=x1;
									}
								}
							}
							else if((x1>=x2)&&(y1>=y2))
							{ 
								tempx=x1-x2;  tempy=y1-y2;
								if(tempx>=tempy)
								{  
									if(tempx<=(tempy+10))
									{
										x2=x1-tempy;
									}
									else
									{ 
										y2=y1;
									}
								}
								else
								{ 
									if(tempy<=(tempx+10))
									{
										y2=y1-tempx;
									}
									else
									{ 
										x2=x1;
									}
								}
							}
							Get_VPort_XY(&xe,&ye,x2,y2);
						}
						
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ye);
						
						if(nEDA_RedrawFlag==1)
						{
							for(i=0;i<number;i++)
							{
								Get_VPort_XY(&tempxs,&tempys,point[i].fx,point[i].fy);
								Get_VPort_XY(&tempxe,&tempye,point[i+1].fx,point[i+1].fy);
								dc.MoveTo(tempxs,tempys);
								dc.LineTo(tempxe,tempye);
							}
							nEDA_RedrawFlag=0;
						}
					}
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					if(number<=9997)
					{
						number++;
						point[number].fx=x2;
						point[number].fy=y2;
						x1=x2;y1=y2;
						xs=xe;ys=ye;
					}
					continue;
				}
				else if(ch1 == TAB)
				{
					CSheet_EditPolygon  ps(_T("多边形"), NULL, 0);
					
					//属性页
					ps.page1.m_bCheckDrawSolid = cEDA_schpolygon.m_bFillSolid;
					ps.page1.m_bCheckTransparent = cEDA_schpolygon.m_bTransparent;
					ps.page1.m_nComboWidth = cEDA_schpolygon.m_nBorderWidth;
					ps.page1.m_nFillColor = cEDA_schpolygon.m_nFillColor;
					ps.page1.m_nBorderColor = cEDA_schpolygon.m_nBorderColor;
					
					//顶点页	
					ps.RemovePage(1);
			
					if(ps.DoModal() == IDOK)
					{
						//属性
						cEDA_schpolygon.m_bFillSolid = ps.page1.m_bCheckDrawSolid;
						cEDA_schpolygon.m_bTransparent = ps.page1.m_bCheckTransparent;
						cEDA_schpolygon.m_nBorderWidth = ps.page1.m_nComboWidth;
						cEDA_schpolygon.m_nFillColor = ps.page1.m_nFillColor;
						cEDA_schpolygon.m_nBorderColor = ps.page1.m_nBorderColor;

						delete OverwritePen;
						OverwritePen = new CPen(PS_SOLID, 1, cEDA_schpolygon.m_nBorderColor);
						dc.SelectObject(OverwritePen);
					}
				}
			}while(1);
		}
		else if(ch1 == TAB)
		{
			CSheet_EditPolygon  ps(_T("多边形"), NULL, 0);
			
			//属性页
			ps.page1.m_bCheckDrawSolid = cEDA_schpolygon.m_bFillSolid;
			ps.page1.m_bCheckTransparent = cEDA_schpolygon.m_bTransparent;
			ps.page1.m_nComboWidth = cEDA_schpolygon.m_nBorderWidth;
			ps.page1.m_nFillColor = cEDA_schpolygon.m_nFillColor;
			ps.page1.m_nBorderColor = cEDA_schpolygon.m_nBorderColor;
			
			//顶点页	
			ps.RemovePage(1);
	
			if(ps.DoModal() == IDOK)
			{
				//属性
				cEDA_schpolygon.m_bFillSolid = ps.page1.m_bCheckDrawSolid;
				cEDA_schpolygon.m_bTransparent = ps.page1.m_bCheckTransparent;
				cEDA_schpolygon.m_nBorderWidth = ps.page1.m_nComboWidth;
				cEDA_schpolygon.m_nFillColor = ps.page1.m_nFillColor;
				cEDA_schpolygon.m_nBorderColor = ps.page1.m_nBorderColor;

				delete OverwritePen;
				OverwritePen = new CPen(PS_SOLID, 1, cEDA_schpolygon.m_nBorderColor);
				dc.SelectObject(OverwritePen);
			}
		}
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);

	delete OverwritePen;	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);
}

void CESDView::OnPlaceRect()
{
	unsigned ch1;
	double fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;
	POSITION pos;
	
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	char szPlaceOneCorner[] = "放置长方形: <左键><回车>定义区域一角, <右键><ESC>结束命令";
	char szPlaceAnotherCorner[] = "放置长方形: <左键><回车>定义区域另一角, <右键><ESC>结束命令";
	char szInfo[] = "<TAB>属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	MoveCursorToCenter();

	Write_Help(szPlaceOneCorner);
	Write_Info(szInfo);
	flag = 0;

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);
	CPen *OverwritePen, *oldpen;
	OverwritePen = new CPen(PS_SOLID, 1, cEDA_schrect.m_nBorderColor);
	oldpen = dc.SelectObject(OverwritePen);

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
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					break;

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
					
					double fxmin, fxmax, fymin, fymax;
					fxmin = MIN(fx1,fx2);
					fymin = MIN(fy1,fy2);
					fxmax = MAX(fx1,fx2);
					fymax = MAX(fy1,fy2);

					COb_schrect *prect = new COb_schrect;
					prect->pDocument = pDoc;
					prect->m_fX1 = fxmin;
					prect->m_fY1 = fymin;
					prect->m_fX2 = fxmax;
					prect->m_fY2 = fymax;
					prect->m_nBorderWidth = cEDA_schrect.m_nBorderWidth;
					prect->m_nBorderColor = cEDA_schrect.m_nBorderColor;
					prect->m_nFillColor = cEDA_schrect.m_nFillColor;
					prect->m_bFillSolid = cEDA_schrect.m_bFillSolid;
					prect->m_bTransparent = cEDA_schrect.m_bTransparent;
					prect->m_nDiagram = m_nDiagram;

					pos = pDoc->m_listSchObject[m_nDiagram].AddTail(prect);

					//----------- Undo -------------------------------------------------------------------------------------
					if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = SCH_ELEMENT_RECT;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = pos;
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					//-------------------------------------------------------------------------------------------------------

					dc.SetROP2(drawmode);
					CloseMouseCursor();
					prect->Draw(&dc, this);
					OpenMouseCursor();
					
					ch1 = VK_ESCAPE;
					break;
				}
				else if(ch1 == TAB)
				{
					CDlg_EditRect dlg;
					
					dlg.m_bCheckDrawSolid = cEDA_schrect.m_bFillSolid;
					dlg.m_bCheckTransparent = cEDA_schrect.m_bTransparent;
					dlg.m_nComboWidth = cEDA_schrect.m_nBorderWidth;
					dlg.m_nFillColor = cEDA_schrect.m_nFillColor;
					dlg.m_nBorderColor = cEDA_schrect.m_nBorderColor;
					
					if(dlg.DoModal() == IDOK)
					{
						cEDA_schrect.m_bFillSolid = dlg.m_bCheckDrawSolid;
						cEDA_schrect.m_bTransparent = dlg.m_bCheckTransparent;
						cEDA_schrect.m_nBorderWidth = dlg.m_nComboWidth;
						cEDA_schrect.m_nFillColor = dlg.m_nFillColor;
						cEDA_schrect.m_nBorderColor = dlg.m_nBorderColor;
						
						delete OverwritePen;
						OverwritePen = new CPen(PS_SOLID, 1, cEDA_schrect.m_nBorderColor);
						dc.SelectObject(OverwritePen);
					}
				}
			}while(1);
		}
		else if(ch1 == TAB)
		{
			CDlg_EditRect dlg;
			
			dlg.m_bCheckDrawSolid = cEDA_schrect.m_bFillSolid;
			dlg.m_bCheckTransparent = cEDA_schrect.m_bTransparent;
			dlg.m_nComboWidth = cEDA_schrect.m_nBorderWidth;
			dlg.m_nFillColor = cEDA_schrect.m_nFillColor;
			dlg.m_nBorderColor = cEDA_schrect.m_nBorderColor;
			
			if(dlg.DoModal() == IDOK)
			{
				cEDA_schrect.m_bFillSolid = dlg.m_bCheckDrawSolid;
				cEDA_schrect.m_bTransparent = dlg.m_bCheckTransparent;
				cEDA_schrect.m_nBorderWidth = dlg.m_nComboWidth;
				cEDA_schrect.m_nFillColor = dlg.m_nFillColor;
				cEDA_schrect.m_nBorderColor = dlg.m_nBorderColor;
				
				delete OverwritePen;
				OverwritePen = new CPen(PS_SOLID, 1, cEDA_schrect.m_nBorderColor);
				dc.SelectObject(OverwritePen);
			}
		}
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);

	delete OverwritePen;	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);
}

void CESDView::OnPlaceRoundrect()
{
	unsigned ch1;
	double fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;
	POSITION pos;
	
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	char szPlaceOneCorner[] = "放置方圆形: <左键><回车>定义区域一角, <右键><ESC>结束命令";
	char szPlaceAnotherCorner[] = "放置方圆形: <左键><回车>定义区域另一角, <右键><ESC>结束命令";
	char szInfo[] = "<TAB>属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	Write_Help(szPlaceOneCorner);
	Write_Info(szInfo);
	flag = 0;

	MoveCursorToCenter();
	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);
	CPen *OverwritePen, *oldpen;
	OverwritePen = new CPen(PS_SOLID, 1, cEDA_schroundrect.m_nBorderColor);
	oldpen = dc.SelectObject(OverwritePen);

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
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					break;

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
					
					double fxmin, fxmax, fymin, fymax;
					fxmin = MIN(fx1,fx2);
					fymin = MIN(fy1,fy2);
					fxmax = MAX(fx1,fx2);
					fymax = MAX(fy1,fy2);

					COb_schroundrect *proundrect = new COb_schroundrect;
					proundrect->pDocument = pDoc;
					proundrect->m_fX1 = fxmin;
					proundrect->m_fY1 = fymin;
					proundrect->m_fX2 = fxmax;
					proundrect->m_fY2 = fymax;
					proundrect->m_fRadius_x = cEDA_schroundrect.m_fRadius_x;
					proundrect->m_fRadius_y = cEDA_schroundrect.m_fRadius_y;

					proundrect->m_nBorderWidth = cEDA_schroundrect.m_nBorderWidth;
					proundrect->m_nBorderColor = cEDA_schroundrect.m_nBorderColor;
					proundrect->m_nFillColor = cEDA_schroundrect.m_nFillColor;
					proundrect->m_bFillSolid = cEDA_schroundrect.m_bFillSolid;
					proundrect->m_nDiagram = m_nDiagram;

					pos = pDoc->m_listSchObject[m_nDiagram].AddTail(proundrect);

					//----------- Undo -------------------------------------------------------------------------------------
					if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = pos;
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					//-------------------------------------------------------------------------------------------------------

					dc.SetROP2(drawmode);
					CloseMouseCursor();
					proundrect->Draw(&dc, this);
					OpenMouseCursor();
					
					ch1 = VK_ESCAPE;
					break;
				}
				else if(ch1 == TAB)
				{
					CDlg_EditRoundrect dlg;
					
					dlg.m_bCheckDrawSolid = cEDA_schroundrect.m_bFillSolid;
					dlg.m_nComboWidth = cEDA_schroundrect.m_nBorderWidth;
					dlg.m_nFillColor = cEDA_schroundrect.m_nFillColor;
					dlg.m_nBorderColor = cEDA_schroundrect.m_nBorderColor;
					dlg.m_csEditRadiusX = pDoc->Convert_XY_To_String(cEDA_schroundrect.m_fRadius_x);
					dlg.m_csEditRadiusY = pDoc->Convert_XY_To_String(cEDA_schroundrect.m_fRadius_y);					
					if(dlg.DoModal() == IDOK)
					{
						cEDA_schroundrect.m_bFillSolid = dlg.m_bCheckDrawSolid;
						cEDA_schroundrect.m_nBorderWidth = dlg.m_nComboWidth;
						cEDA_schroundrect.m_nFillColor = dlg.m_nFillColor;
						cEDA_schroundrect.m_nBorderColor = dlg.m_nBorderColor;
						cEDA_schroundrect.m_fRadius_x = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusX);
						cEDA_schroundrect.m_fRadius_y = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusY);						
						delete OverwritePen;
						OverwritePen = new CPen(PS_SOLID, 1, cEDA_schroundrect.m_nBorderColor);
						dc.SelectObject(OverwritePen);
					}
				}
			}while(1);
		}
		else if(ch1 == TAB)
		{
			CDlg_EditRoundrect dlg;
			
			dlg.m_bCheckDrawSolid = cEDA_schroundrect.m_bFillSolid;
			dlg.m_nComboWidth = cEDA_schroundrect.m_nBorderWidth;
			dlg.m_nFillColor = cEDA_schroundrect.m_nFillColor;
			dlg.m_nBorderColor = cEDA_schroundrect.m_nBorderColor;
			dlg.m_csEditRadiusX = pDoc->Convert_XY_To_String(cEDA_schroundrect.m_fRadius_x);
			dlg.m_csEditRadiusY = pDoc->Convert_XY_To_String(cEDA_schroundrect.m_fRadius_y);
			
			if(dlg.DoModal() == IDOK)
			{
				cEDA_schroundrect.m_bFillSolid = dlg.m_bCheckDrawSolid;
				cEDA_schroundrect.m_nBorderWidth = dlg.m_nComboWidth;
				cEDA_schroundrect.m_nFillColor = dlg.m_nFillColor;
				cEDA_schroundrect.m_nBorderColor = dlg.m_nBorderColor;
				cEDA_schroundrect.m_fRadius_x = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusX);
				cEDA_schroundrect.m_fRadius_y = pDoc->Convert_String_To_XY(dlg.m_csEditRadiusY);
				
				delete OverwritePen;
				OverwritePen = new CPen(PS_SOLID, 1, cEDA_schroundrect.m_nBorderColor);
				dc.SelectObject(OverwritePen);
			}
		}
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);

	delete OverwritePen;	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	Write_Info(szEDA_Info_Ready);
}

void CESDView::OnPlaceTextframe()
{
	unsigned ch1;
	double fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;
	POSITION pos;
	
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	char szPlaceOneCorner[] = "放置文字框: <左键><回车>定义区域一角, <右键><ESC>结束命令";
	char szPlaceAnotherCorner[] = "放置文字框: <左键><回车>定义区域另一角, <右键><ESC>结束命令";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	Write_Help(szPlaceOneCorner);
	flag = 0;

	MoveCursorToCenter();
	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);
	CPen *OverwritePen, *oldpen;
	OverwritePen = new CPen(PS_SOLID, 1, cEDA_schtextframe.m_nBorderColor);
	oldpen = dc.SelectObject(OverwritePen);

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
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					break;

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
					
					double fxmin, fxmax, fymin, fymax;
					fxmin = MIN(fx1,fx2);
					fymin = MIN(fy1,fy2);
					fxmax = MAX(fx1,fx2);
					fymax = MAX(fy1,fy2);

					COb_schtextframe *ptextframe = new COb_schtextframe;
					ptextframe->pDocument = pDoc;
					ptextframe->m_fX1 = fxmin;
					ptextframe->m_fY1 = fymin;
					ptextframe->m_fX2 = fxmax;
					ptextframe->m_fY2 = fymax;
					ptextframe->m_nBorderWidth = cEDA_schtextframe.m_nBorderWidth;
					ptextframe->m_nFont = cEDA_schtextframe.m_nFont;
					ptextframe->m_nBorderColor = cEDA_schtextframe.m_nBorderColor;
					ptextframe->m_nFillColor = cEDA_schtextframe.m_nFillColor;
					ptextframe->m_nTextColor = cEDA_schtextframe.m_nTextColor;
					ptextframe->m_bFillSolid = cEDA_schtextframe.m_bFillSolid;
					ptextframe->m_bDrawBorder = cEDA_schtextframe.m_bDrawBorder;
					ptextframe->m_nAlignment = cEDA_schtextframe.m_nAlignment;
					ptextframe->m_bClipToArea = cEDA_schtextframe.m_bClipToArea;
					ptextframe->m_bWordWrap = cEDA_schtextframe.m_bWordWrap;
					ptextframe->m_nDiagram = m_nDiagram;
					CString cstring = "Text";
					ptextframe->m_arrayText.Add(cstring);

					pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ptextframe);

					//----------- Undo -------------------------------------------------------------------------------------
					if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = pos;
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					//-------------------------------------------------------------------------------------------------------

					dc.SetROP2(drawmode);
					CloseMouseCursor();
					ptextframe->Draw(&dc, this);
					OpenMouseCursor();
					
					ch1 = VK_ESCAPE;
					break;
				}
			}while(1);
		}
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);

	delete OverwritePen;	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
}



void CESDView::OnPlaceImage()
{
	unsigned ch1;
	double fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;
	POSITION pos;
	
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	char szPlaceOneCorner[] = "放置图像: <左键><回车>定义区域一角, <右键><ESC>结束命令";
	char szPlaceAnotherCorner[] = "放置图像: <左键><回车>定义区域另一角, <右键><ESC>结束命令";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	Write_Help(szPlaceOneCorner);
	flag = 0;

	MoveCursorToCenter();
	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);
	CPen *OverwritePen, *oldpen;
	OverwritePen = new CPen(PS_SOLID, 1, cEDA_schgraphic.m_nBorderColor);
	oldpen = dc.SelectObject(OverwritePen);

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
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					break;

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
					
					double fxmin, fxmax, fymin, fymax;
					fxmin = MIN(fx1,fx2);
					fymin = MIN(fy1,fy2);
					fxmax = MAX(fx1,fx2);
					fymax = MAX(fy1,fy2);

					static char BASED_CODE szFileFilter[] = "图像文件(*.bmp)|*.bmp|所有文件 (*.*) |*.*||";
					CFileDialog importdialog(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY|OFN_ENABLESIZING,szFileFilter,NULL);
					if(importdialog.DoModal() != IDOK) 
					{
						ch1 = VK_ESCAPE;
						break;
					}

					COb_schgraphic *pgraphic = new COb_schgraphic;
					pgraphic->pDocument = pDoc;
					pgraphic->m_fX1 = fxmin;
					pgraphic->m_fY1 = fymin;
					pgraphic->m_fX2 = fxmax;
					pgraphic->m_fY2 = fymax;
					pgraphic->m_nBorderWidth = cEDA_schgraphic.m_nBorderWidth;
					pgraphic->m_nBorderColor = cEDA_schgraphic.m_nBorderColor;
					pgraphic->m_bBorderOn = cEDA_schgraphic.m_bBorderOn;
					pgraphic->m_bAspect = cEDA_schgraphic.m_bAspect;
					pgraphic->m_nDiagram = m_nDiagram;

					pgraphic->m_csFileName = importdialog.GetPathName();

					pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pgraphic);

					//----------- Undo -------------------------------------------------------------------------------------
					if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = pos;
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					//-------------------------------------------------------------------------------------------------------

					dc.SetROP2(drawmode);
					CloseMouseCursor();
					pgraphic->Draw(&dc, this);
					OpenMouseCursor();
					
					ch1 = VK_ESCAPE;
					break;
				}
			}while(1);
		}
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);

	delete OverwritePen;	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
}

void CESDView::OnPlaceBezier()
{
	int ch1;
	double x1, y1, x2, y2, x3, y3, x4, y4;
	Fpoint fpoint;

	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	char szPlaceBezier1[] = "放置样条曲线: <左键><回车>定义起点, <右键><ESC>结束命令";
	char szPlaceBezier2[] = "放置样条曲线: <左键><回车>定义第一顶点, <右键><ESC>返回第一步";
	char szPlaceBezier3[] = "放置样条曲线: <左键><回车>定义第二顶点, <右键><ESC>返回第一步";
	char szPlaceBezier4[] = "放置样条曲线: <左键><回车>定义终点, <右键><ESC>返回第一步";

	POSITION pos;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	COb_schbezier* pbezier = NULL;

	COb_schbezier abezier;
	abezier.m_nWidth = cEDA_schbezier.m_nWidth;
	abezier.m_nColor = cEDA_schbezier.m_nColor;
	abezier.m_nDiagram = m_nDiagram;
	abezier.m_cVertex.RemoveAll();	

PLACEBEZIER:
	Write_Help(szPlaceBezier1);
	nEDA_RedrawFlag = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			if(abezier.m_cVertex.GetCount() != 1)
			{
				x1 = m_fCurrentSnapX; y1 = m_fCurrentSnapY;
				fpoint.fx = x1; fpoint.fy = y1;
				abezier.m_cVertex.Add(fpoint);
			}
			x2 = m_fCurrentSnapX; y2 = m_fCurrentSnapY;
			fpoint.fx = x2; fpoint.fy = y2;
			abezier.m_cVertex.Add(fpoint);
			abezier.Draw(&dc, this);

			Write_Help(szPlaceBezier2);
			do
			{
				ch1 = MouseKey();
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					abezier.Draw(&dc, this);
					//如果接着前面的bezier继续绘制, 仅保留第一个点
					if(pbezier != NULL)
					{
						fpoint.fx = abezier.m_cVertex[0].fx;
						fpoint.fy = abezier.m_cVertex[0].fy;
						abezier.m_cVertex.RemoveAll();										
						abezier.m_cVertex.Add(fpoint);
					}
					else abezier.m_cVertex.RemoveAll();

					ch1 = MOUSEMOVE;
					Write_Help(szPlaceBezier1);
					break;
				}
				if(ch1==MOUSEMOVE)
				{
					if(nEDA_RedrawFlag==1){ nEDA_RedrawFlag=0;}
					else 
					{ 
						abezier.Draw(&dc, this);	
					}

					x2 = m_fCurrentSnapX; y2 = m_fCurrentSnapY;
					abezier.m_cVertex[1].fx = x2;
					abezier.m_cVertex[1].fy = y2;
					abezier.Draw(&dc, this);
				}
				if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{  
					abezier.Draw(&dc, this);
					x3 = m_fCurrentSnapX; y3 = m_fCurrentSnapY;
					fpoint.fx = x3; fpoint.fy = y3;
					abezier.m_cVertex.Add(fpoint);
					fpoint.fx = x3; fpoint.fy = y3;
					abezier.m_cVertex.Add(fpoint);
					abezier.Draw(&dc, this);
					Write_Help(szPlaceBezier3);
					do
					{
						ch1 = MouseKey();
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							abezier.Draw(&dc, this);
							//如果接着前面的bezier继续绘制, 仅保留第一个点
							if(pbezier != NULL)
							{
								fpoint.fx = abezier.m_cVertex[0].fx;
								fpoint.fy = abezier.m_cVertex[0].fy;
								abezier.m_cVertex.RemoveAll();										
								abezier.m_cVertex.Add(fpoint);
							}
							else abezier.m_cVertex.RemoveAll();
							ch1 = MOUSEMOVE;
							goto PLACEBEZIER;
						}
						if(ch1==MOUSEMOVE)
						{
							if(nEDA_RedrawFlag==1){ nEDA_RedrawFlag=0;}
							else 
							{ 
								abezier.Draw(&dc, this);
							}
							x3 = m_fCurrentSnapX; y3 = m_fCurrentSnapY;
							abezier.m_cVertex[2].fx = x3;
							abezier.m_cVertex[2].fy = y3;
							abezier.m_cVertex[3].fx = x3;
							abezier.m_cVertex[3].fy = y3;
							abezier.Draw(&dc, this);
						}
						if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
						{  
							abezier.Draw(&dc, this);
							x3 = m_fCurrentSnapX; y3 = m_fCurrentSnapY;
							abezier.m_cVertex[2].fx = x3;
							abezier.m_cVertex[2].fy = y3;
							abezier.m_cVertex[3].fx = x3;
							abezier.m_cVertex[3].fy = y3;
							//abezier.m_cVertex.Add(fpoint);
							abezier.Draw(&dc, this);
							Write_Help(szPlaceBezier4);
							do
							{
								ch1 = MouseKey();
								if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
								{ 
									abezier.Draw(&dc, this);
									//如果接着前面的bezier继续绘制, 仅保留第一个点
									if(pbezier != NULL)
									{
										fpoint.fx = abezier.m_cVertex[0].fx;
										fpoint.fy = abezier.m_cVertex[0].fy;
										abezier.m_cVertex.RemoveAll();										
										abezier.m_cVertex.Add(fpoint);
									}
									else abezier.m_cVertex.RemoveAll();
									ch1 = MOUSEMOVE;
									goto PLACEBEZIER;
								}
								else if(ch1 == MOUSEMOVE)
								{
									if(nEDA_RedrawFlag == 1){ nEDA_RedrawFlag = 0;}
									else  
									{ 
										abezier.Draw(&dc, this);
									}
									
									x4 = m_fCurrentSnapX; y4 = m_fCurrentSnapY;
									abezier.m_cVertex[3].fx = x4;
									abezier.m_cVertex[3].fy = y4;
									abezier.Draw(&dc, this);
								}
								else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
								{
									abezier.Draw(&dc, this);

									if(pbezier == NULL)
									{
										pbezier = new COb_schbezier;
										pbezier->pDocument = pDoc;
										pbezier->m_nWidth = cEDA_schbezier.m_nWidth;
										pbezier->m_nColor = cEDA_schbezier.m_nColor;
										pbezier->m_nDiagram = m_nDiagram;
										for(int i=0; i<abezier.m_cVertex.GetCount(); i++)
										{
											fpoint = abezier.m_cVertex.GetAt(i);
											pbezier->m_cVertex.Add(fpoint);
										}
										pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pbezier);

										dc.SetROP2(drawmode);
										CloseMouseCursor();
										pbezier->Draw(&dc, this);
										OpenMouseCursor();
										dc.SetROP2(R2_NOTXORPEN);
												
										//----------- Undo -------------------------------------------------------------------------------------
										if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
										{
											pDoc->Clear_Discarded_Undo_Buffer();
										}
										clear_undo_buffer_flag = TRUE;
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_DELETE;
										pundo->m_nOb = SCH_ELEMENT_BEZIER;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = pos;
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
										undo_flag = TRUE;
										//-------------------------------------------------------------------------------------------------------

									}
									else
									{
										for(int i=1; i<abezier.m_cVertex.GetCount(); i++)
										{
											fpoint = abezier.m_cVertex.GetAt(i);
											pbezier->m_cVertex.Add(fpoint);
										}
										
										dc.SetROP2(drawmode);
										CloseMouseCursor();
										pbezier->Draw(&dc, this);
										OpenMouseCursor();
										dc.SetROP2(R2_NOTXORPEN);
									}

									fpoint.fx = abezier.m_cVertex[3].fx;
									fpoint.fy = abezier.m_cVertex[3].fy;
									abezier.m_cVertex.RemoveAll();										
									abezier.m_cVertex.Add(fpoint);
									goto PLACEBEZIER;	
								}
							}while(1);
						}
					}while(1);
				}
			}while(1);
		}
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	dc.SetROP2(drawmode);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
}

void CESDView::OnPlacePort()
{
	int ch1;
	POSITION pos;
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作
	
	char szPlaceText[] = "放置端口: <空格>旋转, <左键><回车>放置, <右键><ESC>取消, <TAB>定义属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);	

	//创建对象
	COb_schport aport;
	aport.pDocument = pDoc;
	aport.m_fX = m_fCurrentSnapX;
	aport.m_fY = m_fCurrentSnapY;
	aport.m_fSize = cEDA_schport.m_fSize;
	aport.m_nStyle = cEDA_schport.m_nStyle;
	aport.m_nType = cEDA_schport.m_nType;
	aport.m_nFillColor = cEDA_schport.m_nFillColor;
	aport.m_nBorderColor = cEDA_schport.m_nBorderColor;
	aport.m_nTextColor = cEDA_schport.m_nTextColor;
	aport.m_nAlignment = cEDA_schport.m_nAlignment;
	aport.m_csName.Empty();
	aport.m_nDiagram = m_nDiagram;

	aport.Draw(&dc, this);
	OpenMouseCursor();
	
	//开放Electricl Grid
	bEDA_NeedElectricalGrid = TRUE;
	
	Write_Help(szPlaceText);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			ElecHotPointsControl(&dc);

			CloseMouseCursor();
			aport.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			//视未获得focus时不响应鼠标移动
			if(m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(m_bAfterSetFocus)
			{
				m_bAfterSetFocus = false;
				continue;
			}
			
			
			if(nEDA_RedrawFlag == 1)
			{ 
				//屏幕刚重新显示过则不需要复原操作
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				ElecHotPointsControl(&dc);
				CloseMouseCursor();
				aport.Draw(&dc, this);
				OpenMouseCursor();
			}

			aport.m_fX = m_fCurrentSnapX;
			aport.m_fY = m_fCurrentSnapY;
			
			CloseMouseCursor();
			aport.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			ElecHotPointsControl(&dc);

			CloseMouseCursor();
			aport.Draw(&dc, this);
			OpenMouseCursor();

			COb_schport* pport = new COb_schport;
			pport->pDocument = pDoc;
			pport->m_fX = aport.m_fX;
			pport->m_fY = aport.m_fY;
			pport->m_fSize = aport.m_fSize;
			pport->m_nStyle = aport.m_nStyle;
			pport->m_nType = aport.m_nType;
			pport->m_csUniqueID.Empty();
			pport->m_nFillColor = aport.m_nFillColor;
			pport->m_nBorderColor = aport.m_nBorderColor;
			pport->m_nTextColor =aport.m_nTextColor;
			pport->m_csName = cEDA_schport.m_csName;
			pport->m_nAlignment = aport.m_nAlignment;
			pport->m_nDiagram = m_nDiagram;

			pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pport);

			dc.SetROP2(drawmode);
			CloseMouseCursor();
			pport->Draw(&dc, this);
			OpenMouseCursor();
			dc.SetROP2(R2_NOTXORPEN);
												
			//----------- Undo -------------------------------------------------------------------------------------
			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_PORT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			//-------------------------------------------------------------------------------------------------------
			
			CloseMouseCursor();
			aport.Draw(&dc, this);
			OpenMouseCursor();
			
		}
		else if(ch1 == VK_SPACE)
		{ 
			CloseMouseCursor();
			aport.Draw(&dc, this);

			aport.m_fX = m_fCurrentSnapX; 
			aport.m_fY = m_fCurrentSnapY;

			if(aport.m_nStyle == PORT_STYLE_NH) aport.m_nStyle = PORT_STYLE_NV;
			else if(aport.m_nStyle == PORT_STYLE_NV) aport.m_nStyle = PORT_STYLE_NH;
			else if(aport.m_nStyle == PORT_STYLE_LEFTRIGHT) aport.m_nStyle = PORT_STYLE_TOPBOTTOM;
			else if(aport.m_nStyle == PORT_STYLE_TOPBOTTOM) aport.m_nStyle = PORT_STYLE_LEFTRIGHT;
			else if(aport.m_nStyle == PORT_STYLE_LEFT)  aport.m_nStyle = PORT_STYLE_BOTTOM;
			else if(aport.m_nStyle == PORT_STYLE_BOTTOM) aport.m_nStyle = PORT_STYLE_LEFT;
			else if(aport.m_nStyle == PORT_STYLE_RIGHT) aport.m_nStyle = PORT_STYLE_TOP;
			else if(aport.m_nStyle == PORT_STYLE_TOP) aport.m_nStyle = PORT_STYLE_RIGHT;

			aport.Draw(&dc, this);

			OpenMouseCursor();
		}
		else if(ch1 == TAB)
		{
			CloseMouseCursor();
			aport.Draw(&dc, this);
			
			CDlg_EditPort dlg;

			dlg.m_csEditSize = pDoc->Convert_XY_To_String(aport.m_fSize);	

			dlg.m_nFillColor = aport.m_nFillColor;
			dlg.m_nBorderColor = aport.m_nBorderColor;
			dlg.m_nTextColor = aport.m_nTextColor;
			
			dlg.m_csEditName = cEDA_schport.m_csName;
			
			dlg.m_nComboType = aport.m_nType;
			dlg.m_nComboStyle = aport.m_nStyle;
			dlg.m_nComboAlignment = aport.m_nAlignment;


			if(dlg.DoModal() == IDOK)
			{

				aport.m_fSize = pDoc->Convert_String_To_XY(dlg.m_csEditSize);

				aport.m_nFillColor = dlg.m_nFillColor;
				aport.m_nBorderColor = dlg.m_nBorderColor;
				aport.m_nTextColor = dlg.m_nTextColor;
				aport.m_nType = dlg.m_nComboType;
				aport.m_nStyle = dlg.m_nComboStyle;
				aport.m_nAlignment = dlg.m_nComboAlignment;
				
				cEDA_schport.m_csName = dlg.m_csEditName;
				cEDA_schport.m_nFillColor = aport.m_nFillColor;
				cEDA_schport.m_nBorderColor = aport.m_nBorderColor;
				cEDA_schport.m_nTextColor = aport.m_nTextColor;
				cEDA_schport.m_nType = aport.m_nType;
				cEDA_schport.m_nStyle = aport.m_nStyle;
				cEDA_schport.m_nAlignment = aport.m_nAlignment;

			}

			aport.Draw(&dc, this);
			OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	dc.SetROP2(drawmode);
	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL); m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	
	bEDA_NeedElectricalGrid = FALSE;
	m_fpHotPoint1.fx = m_fpHotPoint1.fy = m_fpHotPoint2.fx = m_fpHotPoint2.fy = 0;
}

void CESDView::OnPlaceBusentry()
{
	int ch1;
	int flag;
	POSITION pos;
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作
	
	char szHelp[] = "放置总线入口: <空格>旋转, <左键><回车>放置, <右键><ESC>结束命令";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	MoveCursorToCenter();
	
	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);	
	
	//创建对象
	COb_schline aline;
	aline.pDocument = pDoc;

	aline.m_nColor = cEDA_schbusentry.m_nColor;
	aline.m_nAttrib = cEDA_schbusentry.m_nAttrib;
	aline.m_nLineWidth = cEDA_schbusentry.m_nLineWidth;
	aline.m_nLineType = cEDA_schbusentry.m_nLineType;
	aline.m_nDiagram = m_nDiagram;
	aline.m_cVertex.SetSize(2);
	aline.m_cVertex[0].fx = m_fCurrentSnapX;
	aline.m_cVertex[0].fy = m_fCurrentSnapY;
	aline.m_cVertex[1].fx = m_fCurrentSnapX + 100;
	aline.m_cVertex[1].fy = m_fCurrentSnapY + 100;
	aline.Draw(&dc, this);
	OpenMouseCursor();

	//开放Electricl Grid
	bEDA_NeedElectricalGrid = TRUE;

	flag = 0;
	Write_Help(szHelp);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			ElecHotPointsControl(&dc);
			CloseMouseCursor();
			aline.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			//视未获得focus时不响应鼠标移动
			if(m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(m_bAfterSetFocus)
			{
				m_bAfterSetFocus = false;
				continue;
			}
			
			
			if(nEDA_RedrawFlag == 1)
			{ 
				//屏幕刚重新显示过则不需要复原操作
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				ElecHotPointsControl(&dc);
				CloseMouseCursor();
				aline.Draw(&dc, this);
				OpenMouseCursor();
			}

			if(flag == 0)
			{
				aline.m_cVertex[0].fx = m_fCurrentSnapX;
				aline.m_cVertex[0].fy = m_fCurrentSnapY;
				aline.m_cVertex[1].fx = m_fCurrentSnapX + 100;
				aline.m_cVertex[1].fy = m_fCurrentSnapY + 100;
			}
			else if(flag == 1)
			{
				aline.m_cVertex[0].fx = m_fCurrentSnapX;
				aline.m_cVertex[0].fy = m_fCurrentSnapY;
				aline.m_cVertex[1].fx = m_fCurrentSnapX - 100;
				aline.m_cVertex[1].fy = m_fCurrentSnapY + 100;
			}
			else if(flag == 2)
			{
				aline.m_cVertex[0].fx = m_fCurrentSnapX;
				aline.m_cVertex[0].fy = m_fCurrentSnapY;
				aline.m_cVertex[1].fx = m_fCurrentSnapX - 100;
				aline.m_cVertex[1].fy = m_fCurrentSnapY - 100;
			}			
			else if(flag == 3)
			{
				aline.m_cVertex[0].fx = m_fCurrentSnapX;
				aline.m_cVertex[0].fy = m_fCurrentSnapY;
				aline.m_cVertex[1].fx = m_fCurrentSnapX + 100;
				aline.m_cVertex[1].fy = m_fCurrentSnapY - 100;
			}	

			CloseMouseCursor();
			aline.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			ElecHotPointsControl(&dc);

			CloseMouseCursor();
			aline.Draw(&dc, this);
			OpenMouseCursor();

			COb_schline* pline = new COb_schline();
			pline->pDocument = pDoc;
			pline->m_nColor = aline.m_nColor;
			pline->m_nAttrib = aline.m_nAttrib;
			pline->m_nLineWidth = aline.m_nLineWidth;
			pline->m_nLineType = aline.m_nLineType;
			pline->m_nDiagram = aline.m_nDiagram;
			pline->m_cVertex.SetSize(2);
			pline->m_cVertex[0].fx = aline.m_cVertex[0].fx;
			pline->m_cVertex[0].fy = aline.m_cVertex[0].fy;
			pline->m_cVertex[1].fx = aline.m_cVertex[1].fx;
			pline->m_cVertex[1].fy = aline.m_cVertex[1].fy;

			pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pline);

			dc.SetROP2(drawmode);
			CloseMouseCursor();
			pline->Draw(&dc, this);
			dc.SetROP2(R2_NOTXORPEN);
												
			//----------- Undo -------------------------------------------------------------------------------------
			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_LINE;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			//-------------------------------------------------------------------------------------------------------

			pline->Draw(&dc, this);
			OpenMouseCursor();

		}
		else if(ch1 == VK_SPACE)
		{ 
			CloseMouseCursor();
			aline.Draw(&dc, this);
			
			flag++;
			if(flag>3) flag=0;

			if(flag == 0)
			{
				aline.m_cVertex[0].fx = m_fCurrentSnapX;
				aline.m_cVertex[0].fy = m_fCurrentSnapY;
				aline.m_cVertex[1].fx = m_fCurrentSnapX + 100;
				aline.m_cVertex[1].fy = m_fCurrentSnapY + 100;
			}
			else if(flag == 1)
			{
				aline.m_cVertex[0].fx = m_fCurrentSnapX;
				aline.m_cVertex[0].fy = m_fCurrentSnapY;
				aline.m_cVertex[1].fx = m_fCurrentSnapX - 100;
				aline.m_cVertex[1].fy = m_fCurrentSnapY + 100;
			}
			else if(flag == 2)
			{
				aline.m_cVertex[0].fx = m_fCurrentSnapX;
				aline.m_cVertex[0].fy = m_fCurrentSnapY;
				aline.m_cVertex[1].fx = m_fCurrentSnapX - 100;
				aline.m_cVertex[1].fy = m_fCurrentSnapY - 100;
			}			
			else if(flag == 3)
			{
				aline.m_cVertex[0].fx = m_fCurrentSnapX;
				aline.m_cVertex[0].fy = m_fCurrentSnapY;
				aline.m_cVertex[1].fx = m_fCurrentSnapX + 100;
				aline.m_cVertex[1].fy = m_fCurrentSnapY - 100;
			}	

			aline.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if(ch1 == TAB)
		{
			CDlg_EditBusEntry dlg;
			dlg.m_nComboWidth = cEDA_schbusentry.m_nLineWidth;
			dlg.m_nColor = cEDA_schbusentry.m_nColor;
			
			if(dlg.DoModal() == IDOK)
			{
				cEDA_schbusentry.m_nLineWidth = dlg.m_nComboWidth;
				cEDA_schbusentry.m_nColor = dlg.m_nColor;

				aline.m_nLineWidth = cEDA_schbusentry.m_nLineWidth;
				aline.m_nColor = cEDA_schbusentry.m_nColor;
			}
			
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	dc.SetROP2(drawmode);
	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL); m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	
	bEDA_NeedElectricalGrid = FALSE;
	m_fpHotPoint1.fx = m_fpHotPoint1.fy = m_fpHotPoint2.fx = m_fpHotPoint2.fy = 0;
}

void CESDView::OnPlaceOffsheet()
{
	int ch1;
	int flag;
	POSITION pos;
	double x1,y1;
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作
	
	char szPlaceText[] = "放置电源符号: <空格>旋转, <左键><回车>放置, <右键><ESC>取消, <TAB>定义属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	MoveCursorToCenter();

	pDoc->SetModifiedFlag(TRUE);
	
	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);	
	
	//创建对象
	COb_schpower apower;
	apower.pDocument = pDoc;
	apower.m_fX = m_fCurrentSnapX;
	apower.m_fY = m_fCurrentSnapY;
	apower.m_nStyle = cEDA_schoffsheet.m_nStyle;
	apower.m_nOrientation = cEDA_schoffsheet.m_nOrientation;
	apower.m_bShowNetName = cEDA_schoffsheet.m_bShowNetName;
	apower.m_bIsCrossSheetConnector = cEDA_schoffsheet.m_bIsCrossSheetConnector;
	apower.m_csNetName.Empty();
	apower.m_nColor = cEDA_schoffsheet.m_nColor;
	apower.m_nDiagram = m_nDiagram;

	apower.Draw(&dc, this);
	OpenMouseCursor();

	//开放Electricl Grid
	bEDA_NeedElectricalGrid = TRUE;
	
	flag = apower.m_nOrientation/90;
	Write_Help(szPlaceText);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			ElecHotPointsControl(&dc);
			CloseMouseCursor();
			apower.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			//视未获得focus时不响应鼠标移动
			if(m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(m_bAfterSetFocus)
			{
				m_bAfterSetFocus = false;
				continue;
			}
			
			
			if(nEDA_RedrawFlag == 1)
			{ 
				//屏幕刚重新显示过则不需要复原操作
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				ElecHotPointsControl(&dc);
				CloseMouseCursor();
				apower.Draw(&dc, this);
				OpenMouseCursor();
			}

			x1 = m_fCurrentSnapX;  
			y1 = m_fCurrentSnapY;

			apower.m_fX = x1;
			apower.m_fY = y1;
			
			CloseMouseCursor();
			apower.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			ElecHotPointsControl(&dc);
			CloseMouseCursor();
			apower.Draw(&dc, this);
			OpenMouseCursor();

			COb_schpower* ppower = new COb_schpower;
			ppower->pDocument = pDoc;
			ppower->m_fX = apower.m_fX;
			ppower->m_fY = apower.m_fY;
			ppower->m_nStyle = apower.m_nStyle;
			ppower->m_nOrientation = apower.m_nOrientation;
			ppower->m_bShowNetName = apower.m_bShowNetName;
			ppower->m_bIsCrossSheetConnector = apower.m_bIsCrossSheetConnector;
			ppower->m_csNetName = cEDA_schoffsheet.m_csNetName;
			ppower->m_nColor = apower.m_nColor;
			ppower->m_nDiagram = apower.m_nDiagram;

			pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ppower);

			dc.SetROP2(drawmode);
			CloseMouseCursor();
			ppower->Draw(&dc, this);
			OpenMouseCursor();
			dc.SetROP2(R2_NOTXORPEN);
												
			//----------- Undo -------------------------------------------------------------------------------------
			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_POWER;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			//-------------------------------------------------------------------------------------------------------

			CloseMouseCursor();
			apower.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if(ch1 == VK_SPACE)
		{ 
			CloseMouseCursor();
			apower.Draw(&dc, this);
			
			flag++;
			if(flag>3) flag=0;

			x1 = m_fCurrentSnapX;  
			y1 = m_fCurrentSnapY;

			apower.m_fX = x1;
			apower.m_fY = y1;

			apower.m_nOrientation = flag*90;

			apower.Draw(&dc, this);

			OpenMouseCursor();
		}
		else if(ch1 == TAB)
		{
			CloseMouseCursor();
			apower.Draw(&dc, this);

			CDlg_EditOffSheet dlg;

			dlg.m_nColor = apower.m_nColor;
			dlg.m_nComboOrientation = apower.m_nOrientation/90;
			dlg.m_csEditNet = cEDA_schoffsheet.m_csNetName;
			dlg.m_nComboStyle = apower.m_nStyle;
			if(dlg.DoModal() == IDOK)
			{
				apower.m_nColor = dlg.m_nColor;
				apower.m_nOrientation = dlg.m_nComboOrientation*90;
				apower.m_nStyle = dlg.m_nComboStyle;
	
				cEDA_schoffsheet.m_csNetName = dlg.m_csEditNet;
				cEDA_schoffsheet.m_nColor = apower.m_nColor;
				cEDA_schoffsheet.m_nOrientation = apower.m_nOrientation;
				cEDA_schoffsheet.m_nStyle = apower.m_nStyle;

				flag = apower.m_nOrientation/90;
			}

			apower.Draw(&dc, this);
			OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	dc.SetROP2(drawmode);
	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL); m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		if(bEDA_AutoJunction == TRUE) AutoPlaceJunction();

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	bEDA_NeedElectricalGrid = FALSE;
	m_fpHotPoint1.fx = m_fpHotPoint1.fy = m_fpHotPoint2.fx = m_fpHotPoint2.fy = 0;
}

void CESDView::OnPlacePower()
{
	int ch1;
	int flag;
	POSITION pos;
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作
	
	char szPlaceText[] = "放置电源符号: <空格>旋转, <左键><回车>放置, <右键><ESC>取消, <TAB>定义属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	MoveCursorToCenter();

	pDoc->SetModifiedFlag(TRUE);
	
	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);	
	
	//创建对象
	COb_schpower apower;
	apower.pDocument = pDoc;
	apower.m_fX = m_fCurrentSnapX;
	apower.m_fY = m_fCurrentSnapY;
	apower.m_nStyle = cEDA_schpower.m_nStyle;
	apower.m_nOrientation = cEDA_schpower.m_nOrientation;
	apower.m_bShowNetName = cEDA_schpower.m_bShowNetName;
	apower.m_bIsCrossSheetConnector = cEDA_schpower.m_bIsCrossSheetConnector;
	apower.m_csNetName.Empty();
	apower.m_nColor = cEDA_schpower.m_nColor;
	apower.m_nDiagram = m_nDiagram;

	apower.Draw(&dc, this);
	OpenMouseCursor();

	//开放Electricl Grid
	bEDA_NeedElectricalGrid = TRUE;

	flag = apower.m_nOrientation/90;
	Write_Help(szPlaceText);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			ElecHotPointsControl(&dc);
			CloseMouseCursor();
			apower.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			//视未获得focus时不响应鼠标移动
			if(m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(m_bAfterSetFocus)
			{
				m_bAfterSetFocus = false;
				continue;
			}
			
			
			if(nEDA_RedrawFlag == 1)
			{ 
				//屏幕刚重新显示过则不需要复原操作
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				ElecHotPointsControl(&dc);
				CloseMouseCursor();
				apower.Draw(&dc, this);
				OpenMouseCursor();
			}

			apower.m_fX = m_fCurrentSnapX;  
			apower.m_fY = m_fCurrentSnapY;
			
			CloseMouseCursor();
			apower.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			ElecHotPointsControl(&dc);

			CloseMouseCursor();
			apower.Draw(&dc, this);
			OpenMouseCursor();

			COb_schpower* ppower = new COb_schpower;
			ppower->pDocument = pDoc;
			ppower->m_fX = apower.m_fX;
			ppower->m_fY = apower.m_fY;
			ppower->m_nStyle = apower.m_nStyle;
			ppower->m_nOrientation = apower.m_nOrientation;
			ppower->m_bShowNetName = apower.m_bShowNetName;
			ppower->m_bIsCrossSheetConnector = apower.m_bIsCrossSheetConnector;
			ppower->m_csNetName = cEDA_schpower.m_csNetName;
			ppower->m_nColor = apower.m_nColor;
			ppower->m_nDiagram = apower.m_nDiagram;

			pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ppower);

			dc.SetROP2(drawmode);
			CloseMouseCursor();
			ppower->Draw(&dc, this);
			OpenMouseCursor();
			dc.SetROP2(R2_NOTXORPEN);
												
			//----------- Undo -------------------------------------------------------------------------------------
			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_POWER;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			//-------------------------------------------------------------------------------------------------------

			CloseMouseCursor();
			apower.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if(ch1 == VK_SPACE)
		{ 
			CloseMouseCursor();
			apower.Draw(&dc, this);
			
			flag++;
			if(flag>3) flag=0;

			apower.m_fX = m_fCurrentSnapX;  
			apower.m_fY = m_fCurrentSnapY;

			apower.m_nOrientation = flag*90;

			apower.Draw(&dc, this);

			OpenMouseCursor();
		}
		else if(ch1 == TAB)
		{
			CloseMouseCursor();
			apower.Draw(&dc, this);

			CDlg_EditPower dlg;
			dlg.m_nPowerOrGround = 0;
			dlg.m_nColor = apower.m_nColor;
			dlg.m_nComboOrientation = apower.m_nOrientation/90;
			dlg.m_csEditNet = cEDA_schpower.m_csNetName;
			dlg.m_nComboStyle = apower.m_nStyle;
			dlg.m_bCheckShowNet = apower.m_bShowNetName;

			if(dlg.DoModal() == IDOK)
			{
				apower.m_nColor = dlg.m_nColor;
				apower.m_nOrientation = dlg.m_nComboOrientation*90;
				apower.m_nStyle = dlg.m_nComboStyle;
				apower.m_bShowNetName = dlg.m_bCheckShowNet;

				cEDA_schpower.m_csNetName = dlg.m_csEditNet;
				cEDA_schpower.m_nColor = apower.m_nColor;
				cEDA_schpower.m_nOrientation = apower.m_nOrientation;
				cEDA_schpower.m_nStyle = apower.m_nStyle;
				cEDA_schpower.m_bShowNetName = apower.m_bShowNetName;

				flag = apower.m_nOrientation/90;
			}

			apower.Draw(&dc, this);
			OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	dc.SetROP2(drawmode);
	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL); m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		if(bEDA_AutoJunction == TRUE) AutoPlaceJunction();

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	
	bEDA_NeedElectricalGrid = FALSE;
	m_fpHotPoint1.fx = m_fpHotPoint1.fy = m_fpHotPoint2.fx = m_fpHotPoint2.fy = 0;
}

void CESDView::OnPlaceGround()
{
	int ch1;
	int flag;
	POSITION pos;
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作
	
	char szPlaceText[] = "放置电源符号: <空格>旋转, <左键><回车>放置, <右键><ESC>取消, <TAB>定义属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	MoveCursorToCenter();
	
	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);	
	
	//创建对象
	COb_schpower apower;
	apower.pDocument = pDoc;
	apower.m_fX = m_fCurrentSnapX;
	apower.m_fY = m_fCurrentSnapY;
	apower.m_nStyle = cEDA_schground.m_nStyle;
	apower.m_nOrientation = cEDA_schground.m_nOrientation;
	apower.m_bShowNetName = cEDA_schground.m_bShowNetName;
	apower.m_bIsCrossSheetConnector = cEDA_schground.m_bIsCrossSheetConnector;
	apower.m_csNetName.Empty();
	apower.m_nColor = cEDA_schground.m_nColor;
	apower.m_nDiagram = m_nDiagram;

	apower.Draw(&dc, this);
	OpenMouseCursor();

	//开放Electricl Grid
	bEDA_NeedElectricalGrid = TRUE;

	flag = apower.m_nOrientation/90;
	Write_Help(szPlaceText);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			ElecHotPointsControl(&dc);
			CloseMouseCursor();
			apower.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			//视未获得focus时不响应鼠标移动
			if(m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(m_bAfterSetFocus)
			{
				m_bAfterSetFocus = false;
				continue;
			}
			
			
			if(nEDA_RedrawFlag == 1)
			{ 
				//屏幕刚重新显示过则不需要复原操作
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				ElecHotPointsControl(&dc);
				CloseMouseCursor();
				apower.Draw(&dc, this);
				OpenMouseCursor();
			}

			apower.m_fX = m_fCurrentSnapX;  
			apower.m_fY = m_fCurrentSnapY;
			
			CloseMouseCursor();
			apower.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			ElecHotPointsControl(&dc);

			CloseMouseCursor();
			apower.Draw(&dc, this);
			OpenMouseCursor();

			COb_schpower* ppower = new COb_schpower;
			ppower->pDocument = pDoc;
			ppower->m_fX = apower.m_fX;
			ppower->m_fY = apower.m_fY;
			ppower->m_nStyle = apower.m_nStyle;
			ppower->m_nOrientation = apower.m_nOrientation;
			ppower->m_bShowNetName = apower.m_bShowNetName;
			ppower->m_bIsCrossSheetConnector = apower.m_bIsCrossSheetConnector;
			ppower->m_csNetName = cEDA_schground.m_csNetName;
			ppower->m_nColor = apower.m_nColor;
			ppower->m_nDiagram = apower.m_nDiagram;

			pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ppower);

			dc.SetROP2(drawmode);
			CloseMouseCursor();
			ppower->Draw(&dc, this);
			OpenMouseCursor();
			dc.SetROP2(R2_NOTXORPEN);
												
			//----------- Undo -------------------------------------------------------------------------------------
			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_POWER;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			//-------------------------------------------------------------------------------------------------------

			CloseMouseCursor();
			apower.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if(ch1 == VK_SPACE)
		{ 
			CloseMouseCursor();
			apower.Draw(&dc, this);
			
			flag++;
			if(flag>3) flag=0;

			apower.m_fX = m_fCurrentSnapX;  
			apower.m_fY = m_fCurrentSnapY;

			apower.m_nOrientation = flag*90;

			apower.Draw(&dc, this);

			OpenMouseCursor();
		}
		else if(ch1 == TAB)
		{
			CloseMouseCursor();
			apower.Draw(&dc, this);

			CDlg_EditPower dlg;
			dlg.m_nPowerOrGround = -1;
			dlg.m_nColor = apower.m_nColor;
			dlg.m_nComboOrientation = apower.m_nOrientation/90;
			dlg.m_csEditNet = cEDA_schground.m_csNetName;
			dlg.m_nComboStyle = apower.m_nStyle;
			dlg.m_bCheckShowNet = apower.m_bShowNetName;

			if(dlg.DoModal() == IDOK)
			{
				apower.m_nColor = dlg.m_nColor;
				apower.m_nOrientation = dlg.m_nComboOrientation*90;
				apower.m_nStyle = dlg.m_nComboStyle;
				apower.m_bShowNetName = dlg.m_bCheckShowNet;

				cEDA_schground.m_csNetName = dlg.m_csEditNet;
				cEDA_schground.m_nColor = apower.m_nColor;
				cEDA_schground.m_nOrientation = apower.m_nOrientation;
				cEDA_schground.m_nStyle = apower.m_nStyle;
				cEDA_schground.m_bShowNetName = apower.m_bShowNetName;

				flag = apower.m_nOrientation/90;
			}

			apower.Draw(&dc, this);
			OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	dc.SetROP2(drawmode);
	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL); m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		if(bEDA_AutoJunction == TRUE) AutoPlaceJunction();

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
	
	bEDA_NeedElectricalGrid = FALSE;
	m_fpHotPoint1.fx = m_fpHotPoint1.fy = m_fpHotPoint2.fx = m_fpHotPoint2.fy = 0;
}

void CESDView::OnPlaceSheetsymbol()
{
	unsigned ch1;
	double fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;
	POSITION posParent, pos;
	
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	char szPlaceOneCorner[] = "放置分页符号: <左键><回车>定义区域一角, <右键><ESC>结束命令";
	char szPlaceAnotherCorner[] = "放置子页符号: <左键><回车>定义区域另一角, <右键><ESC>结束命令";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	MoveCursorToCenter();

	int drawmode = dc.GetROP2();
	CPen *OverwritePen, *oldpen;
	OverwritePen = new CPen(PS_SOLID, 1, cEDA_schsheetsymbol.m_nBorderColor);
	oldpen = dc.SelectObject(OverwritePen);
LABEL:

	dc.SetROP2(R2_NOTXORPEN);
	Write_Help(szPlaceOneCorner);
	flag = 0;
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
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					break;

				}
				else if(ch1==MOUSEMOVE)
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
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{  
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xs,ye);
					dc.LineTo(xs,ys);
					
					double fxmin, fxmax, fymin, fymax;
					fxmin = MIN(fx1,fx2);
					fymin = MIN(fy1,fy2);
					fxmax = MAX(fx1,fx2);
					fymax = MAX(fy1,fy2);

					COb_schsheetsymbol *psheetsymbol = new COb_schsheetsymbol;
					psheetsymbol->pDocument = pDoc;
					psheetsymbol->m_fX = fxmin;
					psheetsymbol->m_fY = fymax;
					psheetsymbol->m_fXsize = fxmax - fxmin;
					psheetsymbol->m_fYsize = fymax - fymin;
					psheetsymbol->m_nBorderWidth = cEDA_schsheetsymbol.m_nBorderWidth;
					psheetsymbol->m_nBorderColor = cEDA_schsheetsymbol.m_nBorderColor;
					psheetsymbol->m_nFillColor = cEDA_schsheetsymbol.m_nFillColor;
					psheetsymbol->m_bFillSolid = cEDA_schsheetsymbol.m_bFillSolid;
					psheetsymbol->m_bShowHiddenTextFields = cEDA_schsheetsymbol.m_bShowHiddenTextFields;
					psheetsymbol->m_nDiagram = m_nDiagram;

					posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(psheetsymbol);
					
					//----------- Undo -------------------------------------------------------------------------------------
					if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = posParent;
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					//-------------------------------------------------------------------------------------------------------
					dc.SetROP2(drawmode);
					CloseMouseCursor();
					psheetsymbol->Draw(&dc, this);
					OpenMouseCursor();

					//32 Designator
					COb_schtext* ptext = new COb_schtext;
					ptext->pDocument = pDoc;
					ptext->m_nID = TEXT_SHEETSYMBOLDESIGNATOR;
					ptext->m_fSx = psheetsymbol->m_fX;
					ptext->m_fSy = psheetsymbol->m_fY + 100;
					ptext->m_nOrientation = 0;

					ptext->m_csText = csEDA_schsheetsymboldesignator;
					ptext->m_nFont = 0;
					ptext->m_nColor = 8388608;
					ptext->m_bAutoPosition = TRUE;
					ptext->m_nDiagram = m_nDiagram;
					ptext->m_posParent = posParent;

					pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ptext);

					psheetsymbol->m_arrayPos.Add(pos);
					//----------- Undo -------------------------------------------------------
					//pundo = new CSchUndo;
					//pundo->m_nOp = UNDO_DELETE;
					//pundo->m_nOb = SCH_ELEMENT_TEXT;
					//pundo->m_nDiagram = m_nDiagram;
					//pundo->m_nPos = pos;
					//pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
					//------------------------------------------------------------------------
					CloseMouseCursor();
					ptext->Draw(&dc, this);
					OpenMouseCursor();

					//33 File Name
					ptext = new COb_schtext;
					ptext->pDocument = pDoc;
					ptext->m_nID = TEXT_SHEETSYMBOLFILENAME;
					ptext->m_fSx = psheetsymbol->m_fX;
					ptext->m_fSy = psheetsymbol->m_fY;
					ptext->m_nOrientation = 0;

					ptext->m_csText = csEDA_schsheetsymbolfilename;
					ptext->m_nFont = 0;
					ptext->m_nColor = 8388608;
					ptext->m_bAutoPosition = TRUE;
					ptext->m_nDiagram = m_nDiagram;
					ptext->m_posParent = posParent;

					pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ptext);

					psheetsymbol->m_arrayPos.Add(pos);
					//----------- Undo -------------------------------------------------------
					//pundo = new CSchUndo;
					//pundo->m_nOp = UNDO_DELETE;
					//pundo->m_nOb = SCH_ELEMENT_TEXT;
					//pundo->m_nDiagram = m_nDiagram;
					//pundo->m_nPos = pos;
					//pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
					//------------------------------------------------------------------------
					CloseMouseCursor();
					ptext->Draw(&dc, this);
					OpenMouseCursor();				

					goto LABEL;
				}

				else if(ch1 == TAB)
				{
					CDlg_EditSheetSymbol dlg;

					dlg.m_nFillColor = cEDA_schsheetsymbol.m_nFillColor;
					dlg.m_nBorderColor = cEDA_schsheetsymbol.m_nBorderColor;
					dlg.m_bCheckDrawSolid = cEDA_schsheetsymbol.m_bFillSolid;
					dlg.m_nComboWidth = cEDA_schsheetsymbol.m_nBorderWidth;
					dlg.m_csEditDesignator = csEDA_schsheetsymboldesignator;
					dlg.m_csEditFilename = csEDA_schsheetsymbolfilename;
					
					if(dlg.DoModal() == IDOK)
					{
						cEDA_schsheetsymbol.m_nFillColor = dlg.m_nFillColor;
						cEDA_schsheetsymbol.m_nBorderColor = dlg.m_nBorderColor;
						cEDA_schsheetsymbol.m_bFillSolid = dlg.m_bCheckDrawSolid;
						cEDA_schsheetsymbol.m_nBorderWidth = dlg.m_nComboWidth;

						csEDA_schsheetsymboldesignator = dlg.m_csEditDesignator;
						csEDA_schsheetsymbolfilename = dlg.m_csEditFilename;
					}
				}
			}while(1);
		}
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	dc.SetROP2(drawmode);
	dc.SelectObject(oldpen);

	delete OverwritePen;

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
}

void CESDView::OnPlaceSheetentry()
{
	unsigned ch1;
	POSITION posParent;
	char szSelectSheetSymbol[] = "选择分页符号..., <右键><ESC>结束命令";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	MoveCursorToCenter();

	Write_Help(szSelectSheetSymbol);
	do
	{
       ch1 = MouseKey();
       if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
			Struct_Multi_Selection multi_selection = SelectSCHObject(m_fCurrentX, m_fCurrentY, MULTI_SELECTION);
			if(multi_selection.element == SCH_ELEMENT_SHEETSYMBOL)
			{
				posParent = multi_selection.pos;
				CObject* pobject;
				pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(posParent);
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;

				//==============================================================================================

				int flag;
				double x1,y1; //, x0, y0;
				BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
				BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作
				
				char szPlaceSheetEntry[] = "放置分页入口: <左键><回车>放置, <右键><ESC>结束命令";

				pDoc->SetModifiedFlag(TRUE);


				CloseMouseCursor();
				int drawmode = dc.GetROP2();
				dc.SetROP2(R2_NOTXORPEN);	

				//创建对象
				COb_schsheetentry asheetentry;
				asheetentry.pDocument = pDoc;
				asheetentry.m_nSide = 0;
				asheetentry.m_nStyle = cEDA_schsheetentry.m_nStyle;
				asheetentry.m_nType = cEDA_schsheetentry.m_nType;
				asheetentry.m_fOffset = 0;
				asheetentry.m_nFillColor = cEDA_schsheetentry.m_nFillColor;
				asheetentry.m_nBorderColor = cEDA_schsheetentry.m_nBorderColor;
				asheetentry.m_nTextColor = cEDA_schsheetentry.m_nTextColor;
				asheetentry.m_csName.Empty();
				asheetentry.m_nDiagram = m_nDiagram;
				asheetentry.m_posParent = posParent;

				asheetentry.Draw(&dc, this);
				OpenMouseCursor();

				flag = 0;
				Write_Help(szPlaceSheetEntry);
				nEDA_RedrawFlag = 0;
				do
				{
					ch1 = MouseKey();
					if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
					{
						CloseMouseCursor();
						asheetentry.Draw(&dc, this);
						OpenMouseCursor();
					}
					else if(ch1 == MOUSEMOVE)
					{
						//视未获得focus时不响应鼠标移动
						if(m_bActive == FALSE) continue;

						// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
						if(m_bAfterSetFocus)
						{
							m_bAfterSetFocus = false;
							continue;
						}
						
						
						if(nEDA_RedrawFlag == 1)
						{ 
							//屏幕刚重新显示过则不需要复原操作
							nEDA_RedrawFlag = 0;
						}
						else  
						{
							CloseMouseCursor();
							asheetentry.Draw(&dc, this);
							OpenMouseCursor();
						}

						x1 = m_fCurrentSnapX;  
						y1 = m_fCurrentSnapY;
						//锁定x,y在sheet symbol的四个边上，按照距离优先原则
						if(x1 < psheetsymbol->m_fX) 
						{
							x1 = psheetsymbol->m_fX;
							asheetentry.m_nSide = ENTRY_SIDE_LEFT;
							asheetentry.m_fOffset = psheetsymbol->m_fY - y1;
						}
						if(x1 > psheetsymbol->m_fX + psheetsymbol->m_fXsize)
						{
							x1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize;
							asheetentry.m_nSide = ENTRY_SIDE_RIGHT;
							asheetentry.m_fOffset = psheetsymbol->m_fY - y1;
						}
						if(y1 > psheetsymbol->m_fY)
						{
							y1 = psheetsymbol->m_fY;
							asheetentry.m_nSide = ENTRY_SIDE_TOP;
							asheetentry.m_fOffset = x1 - psheetsymbol->m_fX;
						}
						if(y1 < psheetsymbol->m_fY - psheetsymbol->m_fYsize)
						{
							asheetentry.m_nSide = ENTRY_SIDE_BOTTOM;
							asheetentry.m_fOffset = x1 - psheetsymbol->m_fX;
							y1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize;
						}
						
						if((x1 > psheetsymbol->m_fX)&&(x1 < psheetsymbol->m_fX + psheetsymbol->m_fXsize)&&(y1 < psheetsymbol->m_fY)&&(y1 > psheetsymbol->m_fY - psheetsymbol->m_fYsize))
						{
							double fxmin = MIN((x1 - psheetsymbol->m_fX), (psheetsymbol->m_fX + psheetsymbol->m_fXsize - x1));
							double fymin = MIN((psheetsymbol->m_fY - y1), (y1 - (psheetsymbol->m_fY - psheetsymbol->m_fYsize)));
							if(fxmin < fymin)
							{
								if((x1 - psheetsymbol->m_fX) < (psheetsymbol->m_fX + psheetsymbol->m_fXsize - x1))
								{
									x1 = psheetsymbol->m_fX;
									asheetentry.m_nSide = ENTRY_SIDE_LEFT;
									asheetentry.m_fOffset = psheetsymbol->m_fY - y1;
								}
								else
								{
									x1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize;
									asheetentry.m_nSide = ENTRY_SIDE_RIGHT;
									asheetentry.m_fOffset = psheetsymbol->m_fY - y1;
								}
							}
							else
							{
								if((psheetsymbol->m_fY - y1) < (y1 - (psheetsymbol->m_fY - psheetsymbol->m_fYsize)))
								{
									y1 = psheetsymbol->m_fY;
									asheetentry.m_nSide = ENTRY_SIDE_TOP;
									asheetentry.m_fOffset = x1 - psheetsymbol->m_fX;
								}
								else 
								{
									y1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize;
									asheetentry.m_nSide = ENTRY_SIDE_BOTTOM;
									asheetentry.m_fOffset = x1 - psheetsymbol->m_fX;
								}
							}
						}
						
						CloseMouseCursor();
						asheetentry.Draw(&dc, this);
						OpenMouseCursor();
					}
					else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
					{
						CloseMouseCursor();

						//创建sheetentry
						COb_schsheetentry* psheetentry = new COb_schsheetentry();
						psheetentry->pDocument = pDoc;
						psheetentry->m_nDiagram = asheetentry.m_nDiagram;	
						psheetentry->m_fOffset =  asheetentry.m_fOffset;
						psheetentry->m_nSide = asheetentry.m_nSide;
						psheetentry->m_nType = asheetentry.m_nType;
						psheetentry->m_nStyle = asheetentry.m_nStyle;
						psheetentry->m_csName = cEDA_schsheetentry.m_csName;
						psheetentry->m_nBorderColor = asheetentry.m_nBorderColor;
						psheetentry->m_nFillColor = asheetentry.m_nFillColor;
						psheetentry->m_nTextColor = asheetentry.m_nTextColor;
						psheetentry->m_posParent = posParent;


						//加入元素列表
						POSITION pos = pDoc->m_listSchObject[asheetentry.m_nDiagram].InsertAfter(posParent, psheetentry);

						//将sheetentry的pos添加到sheetsymbol的pos数组中
						psheetsymbol->m_arrayPos.Add(pos);

						//----------- Undo -------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;

						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_SHEETENTRY;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = pos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//------------------------------------------------------------------------

						dc.SetROP2(drawmode);
						psheetentry->Draw(&dc, this);
						dc.SetROP2(R2_NOTXORPEN);	

						//cEDA_schsheetentry ++
						int index = atoi(cEDA_schsheetentry.m_csName);
						cEDA_schsheetentry.m_csName.Format("%d", index+1);

						asheetentry.Draw(&dc, this);
						OpenMouseCursor();
					}
					else if(ch1 == TAB)
					{
						CloseMouseCursor();
						asheetentry.Draw(&dc, this);
						OpenMouseCursor();

						CDlg_EditSheetEntry dlg;
					
						dlg.m_nFillColor = cEDA_schsheetentry.m_nFillColor;
						dlg.m_nBorderColor = cEDA_schsheetentry.m_nBorderColor;
						dlg.m_nTextColor = cEDA_schsheetentry.m_nTextColor;
						dlg.m_csEditName = cEDA_schsheetentry.m_csName;
						dlg.m_nComboStyle = cEDA_schsheetentry.m_nStyle;
						dlg.m_nComboType = cEDA_schsheetentry.m_nType;

						if(dlg.DoModal() == IDOK)
						{
							cEDA_schsheetentry.m_nFillColor = dlg.m_nFillColor;
							cEDA_schsheetentry.m_nBorderColor = dlg.m_nBorderColor;
							cEDA_schsheetentry.m_nTextColor = dlg.m_nTextColor;
							cEDA_schsheetentry.m_csName = dlg.m_csEditName;
							cEDA_schsheetentry.m_nStyle = dlg.m_nComboStyle;
							cEDA_schsheetentry.m_nType = dlg.m_nComboType;

							asheetentry.m_nStyle = cEDA_schsheetentry.m_nStyle;
							asheetentry.m_nType = cEDA_schsheetentry.m_nType;
							asheetentry.m_nFillColor = cEDA_schsheetentry.m_nFillColor;
							asheetentry.m_nBorderColor = cEDA_schsheetentry.m_nBorderColor;
							asheetentry.m_nTextColor = cEDA_schsheetentry.m_nTextColor;
						}
						
						CloseMouseCursor();
						asheetentry.Draw(&dc, this);
						OpenMouseCursor();
					}
				}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

				dc.SetROP2(drawmode);
				
				if(m_bCursorState == TRUE) CloseMouseCursor();
				//释放鼠标锁定区域；
				ClipCursor(NULL); m_bClipState = FALSE;

				//Undo
				if(undo_flag == TRUE)
				{
					pDoc->m_nSCH_Undo_Num++;
					if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				}

				Write_Help(szEDA_Help_Ready);
				Write_Info(szEDA_Info_Ready);
				//=========================================================================================================================

				break;
			}
	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnPlacePart()
{
	CString csDesignator;
	CString csComment;
	CString csPackage;
	int nPart;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	Struct_CompList complist;

	CDlg_PlaceComp dlg;

	int nReturn = dlg.DoModal();
	if((nReturn != IDOK) && (nReturn != 101)) return;

	if(nReturn == IDOK)
	{
		complist = dlg.m_CompList;
		csDesignator = dlg.m_csEditDesignator;
		csComment = dlg.m_csEditComment;
		csPackage = dlg.m_csComboPackage;
		nPart = dlg.m_nComboPart;
	}
	else if(nReturn == 101)
	{
		CDlg_SchLib_Browse dlg2;
		dlg2.m_bMatchMore = bEDA_SchMatchMore;
		
		if(dlg2.DoModal() != IDOK) return;

		bEDA_SchMatchMore = dlg2.m_bMatchMore;
		complist = dlg2.m_CompList;
		if(complist.liboffset < 0) return;
	
		//在 CompHistory中查找是否有该元件的记录, 没有添加
		Struct_CompList temp_complist;
		int i;
		for(i=0; i<arrayEDA_SchCompHistory.GetCount(); i++)
		{
			temp_complist = arrayEDA_SchCompHistory.GetAt(i);
			CString str1 = temp_complist.compname;
			str1.MakeUpper();
			CString str2 = complist.compname;
			str2.MakeUpper();
			if(str1 == str2) break;
		}
		if(i >= arrayEDA_SchCompHistory.GetCount()) 
		{
			arrayEDA_SchCompHistory.InsertAt(0,complist);
			csEDA_SchCompHistory = complist.compname;
		}

		csDesignator.Empty();
		csComment = complist.compname;
		csPackage = dlg2.m_csComboPackage;
		nPart = dlg2.m_nComboPartNo;
	}

	int basiclibrarynum;
	if(complist.basiclibrary == TRUE)
	{
		for(basiclibrarynum = 0; basiclibrarynum < SCH_SYSTEM_LIB_NUMBER; basiclibrarynum++)
		{
			if(sch_library_offset[basiclibrarynum] == complist.liboffset) break;
		}
	}

	//读入元件数据
	CArray <LOGFONT, LOGFONT&>  arrayFont;

	int offset, length;
	unsigned char* pbuffer = (unsigned char*)malloc(1000000);//new unsigned char[200000];
	//在这里区分是基本元件库还是用户自定义元件库
	if(complist.basiclibrary == TRUE)  //基本元件库SCH_V1.LIB
	{
		//读入字型数据
		cfileEDA_SchLibrary.Seek(complist.liboffset, CFile::begin);
		Struct_Library_Head libhead;
		cfileEDA_SchLibrary.Read(&libhead, 32);

		cfileEDA_SchLibrary.Seek((complist.liboffset + 32+320*libhead.totalcompnum), CFile::begin);
		cfileEDA_SchLibrary.Read(pbuffer, 10000);
		
		CMemFile memfile(pbuffer, 10000);
		CArchive ar(&memfile,CArchive::load);
		int fontnum;
		ar >> fontnum;
		for(int k=0; k<fontnum; k++)
		{
			Struct_Font sFont;
			ar >> sFont.nHeight;
			ar >> sFont.nOrientation;
			ar >> sFont.bUnderline;
			ar >> sFont.bItalic;
			ar >> sFont.bBold;
			ar >> sFont.bStrikeout;
			ar >> sFont.csFontName;
			LOGFONT logfont;
			logfont.lfHeight = sFont.nHeight;
			logfont.lfWidth = 0;
			logfont.lfEscapement = 0;
			logfont.lfOrientation = sFont.nOrientation;
			if(sFont.bBold == FALSE) logfont.lfWeight = FW_NORMAL;
			else logfont.lfWeight = FW_BOLD;
			logfont.lfItalic = sFont.bItalic;
			logfont.lfUnderline = sFont.bUnderline;
			logfont.lfStrikeOut = sFont.bStrikeout;
			logfont.lfCharSet = DEFAULT_CHARSET;
			logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
			logfont.lfQuality = PROOF_QUALITY;
			logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
			strcpy(logfont.lfFaceName, sFont.csFontName);
			arrayFont.Add(logfont);
		}

		ar.Close();
		memfile.Close();

		offset = complist.liboffset + complist.offset;
		length = complist.length;

		cfileEDA_SchLibrary.Seek(offset, CFile::begin);
		cfileEDA_SchLibrary.Read(pbuffer, length);
	}
	else //用户元件库, 在schlib目录下*.lib
	{
		//complist.liboffset 中保存库的序号, complist.offset保存元件在库中的序号
		CString userlibrary, compname;
		userlibrary = arrayEDA_SchUserLibraryName.GetAt(complist.liboffset);
		//打开库文件
		char libstring[MAX_PATH];
		sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, userlibrary);

		CFile cfileUserLibrary;
		if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE) 
		{
			char stemp[100];
			sprintf(stemp, "无法打开用户元件库文件 %s.",libstring);
			AfxMessageBox(stemp, MB_ICONERROR);
			return;
		}
		CArchive ar(&cfileUserLibrary, CArchive::load);

		//识别元件库格式
		CString headformat;
		ar >> headformat;
		if(headformat != SCH_Library_Head)
		{
			char str[_MAX_PATH];
			sprintf(str,"用户元件库文件 %s 格式无法识别.", libstring);
			AfxMessageBox(str, MB_ICONERROR);
			ar.Close();
			cfileUserLibrary.Close();
			return;
		}

		int totalnum;
		ar >> totalnum;

		ar.Read(pbuffer, 320*totalnum);
		
		int fontnum;
		ar >> fontnum;

		for(int k=0; k<fontnum; k++)
		{
			Struct_Font sFont;
			ar >> sFont.nHeight;
			ar >> sFont.nOrientation;
			ar >> sFont.bUnderline;
			ar >> sFont.bItalic;
			ar >> sFont.bBold;
			ar >> sFont.bStrikeout;
			ar >> sFont.csFontName;
			LOGFONT logfont;
			logfont.lfHeight = sFont.nHeight;
			logfont.lfWidth = 0;
			logfont.lfEscapement = 0;
			logfont.lfOrientation = sFont.nOrientation;
			if(sFont.bBold == FALSE) logfont.lfWeight = FW_NORMAL;
			else logfont.lfWeight = FW_BOLD;
			logfont.lfItalic = sFont.bItalic;
			logfont.lfUnderline = sFont.bUnderline;
			logfont.lfStrikeOut = sFont.bStrikeout;
			logfont.lfCharSet = DEFAULT_CHARSET;
			logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
			logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
			logfont.lfQuality = PROOF_QUALITY;
			logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
			strcpy(logfont.lfFaceName, sFont.csFontName);
			arrayFont.Add(logfont);
		}			
		ar.Close();


		//找到320字节元件头信息
		cfileUserLibrary.Seek(32 + 320*complist.offset, CFile::begin);

		Struct_Comp_Head comp_head;
		cfileUserLibrary.Read(&comp_head, 320);
		offset = comp_head.offset;
		length = comp_head.length;

		cfileUserLibrary.Seek(offset, CFile::begin);
		cfileUserLibrary.Read(pbuffer, length);

		cfileUserLibrary.Close();
	}

	CMemFile memfile(pbuffer, length);
	CArchive ar(&memfile,CArchive::load);
	
	COb_libcomphead comphead;
	comphead.Serialize(ar);
			
	//创建Comp
	COb_schcomp* pcomp = new COb_schcomp();
	pcomp->pDocument = pDoc;
	pcomp->m_nDiagram = m_nDiagram;	
	pcomp->m_csLibraryReference = complist.compname;
	pcomp->m_csDescription = comphead.m_csDescription;
	pcomp->m_nPartCount = comphead.m_nPartCount;
	if(pcomp->m_nPartCount <= 0) pcomp->m_nPartCount = 1;
	pcomp->m_nDisplayModeCount = 1;
	pcomp->m_fX = 0;
	pcomp->m_fY = 0;
	pcomp->m_nDisplayMode = 0;
	pcomp->m_bMirrored = FALSE;
	pcomp->m_nOrientation = 0;
	pcomp->m_nPart = nPart;
	pcomp->m_bShowHidePins = FALSE;
	pcomp->m_csLibraryPath.Empty();
	if(complist.basiclibrary == TRUE)
	{
		//首选封装库
		//pcomp->m_csLibraryFileName = "系统元件库";
		int schlib_number;
		for(schlib_number=0; schlib_number<SCH_SYSTEM_LIB_NUMBER; schlib_number++)
		{
			if(sch_library_offset[schlib_number] == complist.liboffset) break;
		}
		if(schlib_number < SCH_SYSTEM_LIB_NUMBER)	pcomp->m_csLibraryFileName = sch_library_name[schlib_number];
		else pcomp->m_csLibraryFileName.Empty();
	}
	else 
	{
		pcomp->m_csLibraryFileName = "用户元件库:";
		pcomp->m_csLibraryFileName += arrayEDA_SchUserLibraryName.GetAt(complist.liboffset);
	}
	
	pcomp->m_csTargetFileName.Empty();
	pcomp->m_csUniqueID.Empty();

	pcomp->m_nFillColor = cEDA_schcomp.m_nFillColor;
	pcomp->m_nColor = cEDA_schcomp.m_nColor;
	pcomp->m_nPinColor = cEDA_schcomp.m_nPinColor;

	pcomp->m_bColorLocked = FALSE;
	pcomp->m_bDesignatorLocked = FALSE;
	pcomp->m_bPartLocked = FALSE;
	pcomp->m_bPinsMoveable = FALSE;
	pcomp->m_nGraphicalType = 0;		

	//加入元素列表
	POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pcomp);

	int element_id;
	for(;;)
	{
		ar >> element_id;
		if(element_id <= 0) break;
		if(element_id == SCH_ELEMENT_PIN)
		{
			//创建Pin
			COb_schpin* ppin = new COb_schpin();
			ppin->Serialize(ar);
			ppin->pDocument = pDoc;
			ppin->m_nDiagram = m_nDiagram;
			ppin->m_posParent = posParent;
			//加入元素列表
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ppin);
			pcomp->m_arrayPos.Add(pos);
		}
		else if(element_id == SCH_ELEMENT_RECT)
		{
			//创建Rectamgle
			COb_schrect* prect = new COb_schrect();
			prect->Serialize(ar);
			prect->pDocument = pDoc;
			prect->m_nDiagram = m_nDiagram;	
			prect->m_posParent = posParent;
			double ftemp;
			if(prect->m_fX1 > prect->m_fX2) { ftemp = prect->m_fX1; prect->m_fX1 = prect->m_fX2; prect->m_fX2 = ftemp;}
			if(prect->m_fY1 > prect->m_fY2) { ftemp = prect->m_fY1; prect->m_fY1 = prect->m_fY2; prect->m_fY2 = ftemp;}
			
			//加入元素列表, 与posParent后面第一个元素对倒, 如果后面第一个是空, 即这个rect自己就是第一个(新库全是如此), 则直接添加
			POSITION insertpos = posParent;
			CObject* pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(insertpos);
			POSITION pos;
			if(insertpos == NULL) pos =  pDoc->m_listSchObject[m_nDiagram].AddTail(prect);
			else
			{
				CObject* pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(insertpos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
				{
					pos =  pDoc->m_listSchObject[m_nDiagram].AddTail(prect);
				}
				else
				{
					pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pobject);
					pDoc->m_listSchObject[m_nDiagram].SetAt(insertpos, prect);
				}
			}
			pcomp->m_arrayPos.Add(pos);
			//POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(prect);
			//pcomp->m_arrayPos.Add(pos);
		}	
		else if(element_id == SCH_ELEMENT_POLYLINE)
		{
			if((complist.basiclibrary == TRUE) && (basiclibrarynum >=227) && (basiclibrarynum <= 289))
			{
				//创建polyline
				COb_schpolyline  apolyline;
				apolyline.Serialize(ar);
				for(int j=0; j<pcomp->m_nPartCount; j++)
				{
					//创建polyline
					COb_schpolyline* ppolyline = new COb_schpolyline();
					ppolyline->Copy(&apolyline);
					ppolyline->pDocument = pDoc;
					ppolyline->m_nDiagram = m_nDiagram;	
					ppolyline->m_posParent = posParent;
					ppolyline->m_nOwnerPartID = j+1;
					//加入元素列表
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ppolyline);
					pcomp->m_arrayPos.Add(pos);
				}
			}
			else
			{
				//创建polyline
				COb_schpolyline* ppolyline = new COb_schpolyline();
				ppolyline->Serialize(ar);
				ppolyline->pDocument = pDoc;
				ppolyline->m_nDiagram = m_nDiagram;	
				ppolyline->m_posParent = posParent;
				//加入元素列表
				POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ppolyline);
				pcomp->m_arrayPos.Add(pos);
			}
		}
		else if(element_id == SCH_ELEMENT_ARC)
		{
			if((complist.basiclibrary == TRUE) && (basiclibrarynum >=227) && (basiclibrarynum <= 289))
			{
				//创建polyline
				COb_scharc  aarc;
				aarc.Serialize(ar);
				for(int j=0; j<pcomp->m_nPartCount; j++)
				{
					//创建arc
					COb_scharc* parc = new COb_scharc();
					parc->Copy(&aarc);
					parc->pDocument = pDoc;
					parc->m_nDiagram = m_nDiagram;	
					parc->m_posParent = posParent;
					parc->m_nOwnerPartID = j+1;
					//加入元素列表
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(parc);
					pcomp->m_arrayPos.Add(pos);
				}
			}
			else
			{
				//创建arc
				COb_scharc* parc = new COb_scharc();
				parc->Serialize(ar);
				parc->pDocument = pDoc;
				parc->m_nDiagram = m_nDiagram;	
				parc->m_posParent = posParent;
				//加入元素列表
				POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(parc);
				pcomp->m_arrayPos.Add(pos);
			}
		}
		else if(element_id == SCH_ELEMENT_PIE)
		{
			//创建pie
			COb_schpie* ppie = new COb_schpie();
			ppie->Serialize(ar);
			ppie->pDocument = pDoc;
			ppie->m_nDiagram = m_nDiagram;	
			ppie->m_posParent = posParent;
			//加入元素列表
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ppie);
			pcomp->m_arrayPos.Add(pos);
		}
		else if(element_id == SCH_ELEMENT_ELLIPSE)
		{
			//创建ellipse
			COb_schellipse* pellipse = new COb_schellipse();
			pellipse->Serialize(ar);
			pellipse->pDocument = pDoc;
			pellipse->m_nDiagram = m_nDiagram;	
			pellipse->m_posParent = posParent;
			//加入元素列表
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pellipse);
			pcomp->m_arrayPos.Add(pos);
		}
		else if(element_id == SCH_ELEMENT_ELLIPSEFILL)
		{
			//创建ellipsefill
			COb_schellipsefill* pellipsefill = new COb_schellipsefill();
			pellipsefill->Serialize(ar);
			pellipsefill->pDocument = pDoc;
			pellipsefill->m_nDiagram = m_nDiagram;	
			pellipsefill->m_posParent = posParent;
			//加入元素列表
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pellipsefill);
			pcomp->m_arrayPos.Add(pos);
		}
		else if(element_id == SCH_ELEMENT_POLYGON)
		{
			if((complist.basiclibrary == TRUE) && (basiclibrarynum >=227) && (basiclibrarynum <= 289))
			{
				//创建polyline
				COb_schpolygon  apolygon;
				apolygon.Serialize(ar);
				for(int j=0; j<pcomp->m_nPartCount; j++)
				{
					//创建polygon
					COb_schpolygon* ppolygon = new COb_schpolygon();
					ppolygon->Copy(&apolygon);
					ppolygon->pDocument = pDoc;
					ppolygon->m_nDiagram = m_nDiagram;	
					ppolygon->m_posParent = posParent;
					ppolygon->m_nOwnerPartID = j+1;
					//加入元素列表
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ppolygon);
					pcomp->m_arrayPos.Add(pos);
				}
			}
			else
			{
				//创建polygon
				COb_schpolygon* ppolygon = new COb_schpolygon();
				ppolygon->Serialize(ar);
				ppolygon->pDocument = pDoc;
				ppolygon->m_nDiagram = m_nDiagram;	
				ppolygon->m_posParent = posParent;
				//加入元素列表
				POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ppolygon);
				pcomp->m_arrayPos.Add(pos);
			}
		}
		else if(element_id == SCH_ELEMENT_TEXT)
		{
			//创建文字
			COb_schtext* ptext = new COb_schtext();
			ptext->Serialize(ar);
			ptext->pDocument = pDoc;
			ptext->m_nDiagram = m_nDiagram;	
			ptext->m_posParent = posParent;
			if(ptext->m_nOwnerPartID <= 0) ptext->m_nOwnerPartID = 1;
			//加入元素列表
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ptext);
			pcomp->m_arrayPos.Add(pos);
			
			//添加字符集
			LOGFONT logfont;
			logfont = arrayFont.GetAt(ptext->m_nFont);
			int font_index = pDoc->CheckFontExist(&logfont, m_nDiagram);
			if(font_index < 0) font_index = pDoc->m_arrayFont[m_nDiagram].Add(logfont);
			ptext->m_nFont = font_index;
		}
		else if(element_id == SCH_ELEMENT_BEZIER)
		{
			//创建bezier
			COb_schbezier* pbezier = new COb_schbezier();
			pbezier->Serialize(ar);
			pbezier->pDocument = pDoc;
			pbezier->m_nDiagram = m_nDiagram;	
			pbezier->m_posParent = posParent;
			//加入元素列表
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pbezier);
			pcomp->m_arrayPos.Add(pos);
		}
		else if(element_id == SCH_ELEMENT_ROUNDRECT)
		{
			//创建RoundRect
			COb_schroundrect* proundrect = new COb_schroundrect();
			proundrect->Serialize(ar);
			proundrect->pDocument = pDoc;
			proundrect->m_nDiagram = m_nDiagram;	
			proundrect->m_posParent = posParent;
			//加入元素列表
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(proundrect);
			pcomp->m_arrayPos.Add(pos);
		}
		else if(element_id == SCH_ELEMENT_IEEE)
		{
			//创建Ieee
			COb_schieee* pieee = new COb_schieee();
			pieee->Serialize(ar);
			pieee->pDocument = pDoc;
			pieee->m_nDiagram = m_nDiagram;	
			pieee->m_posParent = posParent;
			//加入元素列表
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pieee);
			pcomp->m_arrayPos.Add(pos);
		}
		else if(element_id == SCH_ELEMENT_LINE)
		{
			//创建polyline
			AfxMessageBox("Error! unexpected lib element");
		}
	}

	//=========================================================== 创建Designator
	COb_schtext* ptext = new COb_schtext();
	ptext->pDocument = pDoc;
	ptext->m_nDiagram = m_nDiagram;
	ptext->m_nID = TEXT_COMPDESIGNATOR;
	ptext->m_fSx = pcomp->m_fX;
	ptext->m_fSy = pcomp->m_fY + 100;
	ptext->m_nOrientation = 0;

	//主文字信息
	comphead.m_csRef.TrimRight('?');
	if(csDesignator.IsEmpty() == TRUE) ptext->m_csText = comphead.m_csRef + "?";
	else ptext->m_csText = csDesignator;
	ptext->m_bTextVisible = TRUE;
	ptext->m_bTextLocked = FALSE;
	//文字信息的分类名称
	ptext->m_csNameComment = "Designator";
	ptext->m_bNameVisible = FALSE;
	ptext->m_bNameLocked = TRUE;

	ptext->m_nFont = 0;
	ptext->m_nColor = 8388608;

	ptext->m_bMirror = FALSE;		ptext->m_bAutoPosition = TRUE;		ptext->m_nParamType = 0;
	ptext->m_csUniqueID.Empty();		ptext->m_bAllowDataBaseSynchronize = FALSE;		ptext->m_bAllowLibrarySynchronize = FALSE;
	ptext->m_nH_Justification = 0;		ptext->m_nV_Justification = 0;		ptext->m_nAnChorH = 0;		ptext->m_nAnChorV = 0;
	ptext->m_nOwnerPartID = 0;		ptext->m_nDisplayMode = 0;		ptext->m_nDiagram = m_nDiagram;		ptext->m_bSelection = FALSE;
	ptext->m_nFlag = FLAG_NORMAL;
		
	ptext->m_posParent = posParent;
	//加入元素列表
	POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ptext);
	pcomp->m_arrayPos.Add(pos);

	//===============================================================  创建Comment
	ptext = new COb_schtext();
	ptext->pDocument = pDoc;
	ptext->m_nDiagram = m_nDiagram;
	ptext->m_nID = TEXT_COMPCOMMENT;
	ptext->m_fSx = pcomp->m_fX;
	ptext->m_fSy = pcomp->m_fY;
	ptext->m_nOrientation = 0;

	//主文字信息
	if(csComment.IsEmpty() == TRUE) ptext->m_csText = complist.compname;
	else ptext->m_csText = csComment;
	ptext->m_bTextVisible = TRUE;
	ptext->m_bTextLocked = FALSE;
	//文字信息的分类名称
	ptext->m_csNameComment = "Comment";
	ptext->m_bNameVisible = FALSE;
	ptext->m_bNameLocked = TRUE;

	ptext->m_nFont = 0;
	ptext->m_nColor = 8388608;

	ptext->m_bMirror = FALSE;		ptext->m_bAutoPosition = TRUE;		ptext->m_nParamType = 0;
	ptext->m_csUniqueID.Empty();		ptext->m_bAllowDataBaseSynchronize = FALSE;		ptext->m_bAllowLibrarySynchronize = FALSE;
	ptext->m_nH_Justification = 0;		ptext->m_nV_Justification = 0;		ptext->m_nAnChorH = 0;		ptext->m_nAnChorV = 0;
	ptext->m_nOwnerPartID = 0;		ptext->m_nDisplayMode = 0;		ptext->m_nDiagram = m_nDiagram;		ptext->m_bSelection = FALSE;
	ptext->m_nFlag = FLAG_NORMAL;
		
	ptext->m_posParent = posParent;
	//加入元素列表
	pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ptext);
	pcomp->m_arrayPos.Add(pos);
		
	//=================================================================  创建Package
	ptext = new COb_schtext();
	ptext->pDocument = pDoc;
	ptext->m_nDiagram = m_nDiagram;
	ptext->m_nID = TEXT_COMPPACKAGE;
	ptext->m_fSx = pcomp->m_fX;
	ptext->m_fSy = pcomp->m_fY + 200;
	ptext->m_nOrientation = 0;

	//主文字信息
	if(csPackage.IsEmpty() == TRUE) 
	{
		if(comphead.m_arrayPackage.GetCount() > 0)	ptext->m_csText = comphead.m_arrayPackage.GetAt(0);
	}
	else ptext->m_csText = csPackage;

	ptext->m_bTextVisible = FALSE;
	ptext->m_bTextLocked = TRUE;
	//文字信息的分类名称
	ptext->m_csNameComment = "Comment";
	ptext->m_bNameVisible = FALSE;
	ptext->m_bNameLocked = TRUE;

	ptext->m_nFont = 0;
	ptext->m_nColor = 8388608;

	ptext->m_bMirror = FALSE;
	ptext->m_bAutoPosition = TRUE;
	ptext->m_nParamType = 0;
	ptext->m_csUniqueID.Empty();
	ptext->m_bAllowDataBaseSynchronize = FALSE;
	ptext->m_bAllowLibrarySynchronize = FALSE;
	ptext->m_nH_Justification = 0;
	ptext->m_nV_Justification = 0;
	ptext->m_nAnChorH = 0;
	ptext->m_nAnChorV = 0;
	ptext->m_nOwnerPartID = 0;
	ptext->m_nDisplayMode = 0;
	ptext->m_nDiagram = m_nDiagram;
	ptext->m_bSelection = FALSE;
	ptext->m_nFlag = FLAG_NORMAL;

	ptext->m_posParent = posParent;

	//加入元素列表
	pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ptext);
	pcomp->m_arrayPos.Add(pos);

	ar.Close();
	memfile.Close();
	free(pbuffer);

	//处理多套隐藏引脚重复问题
	if((pcomp->m_nPartCount > 1)&&(pcomp->m_nPartCount < 26))
	{
		CObject* pobject;
		for(int i=0; i<pcomp->m_arrayPos.GetCount(); i++)
		{
			POSITION subpos = pcomp->m_arrayPos.GetAt(i);
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
			{
				COb_schpin* ppin = (COb_schpin*)pobject;

				if(ppin->m_nOwnerPartID > 0)
				{
					//标志
					int flag[26];
					for(int k=0; k <pcomp->m_nPartCount; k++) flag[k] = 0;

					//检索所有引脚, 看是否存在每个单元都存在此引脚
					for(int k=0; k<pcomp->m_arrayPos.GetCount(); k++)
					{
						POSITION pos = pcomp->m_arrayPos.GetAt(k);
						pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
						{
							COb_schpin* ppin_1 = (COb_schpin*)pobject;
							if((ppin_1->m_nOwnerPartID > 0)&&(ppin_1->m_csNumber == ppin->m_csNumber)) flag[ppin_1->m_nOwnerPartID-1] = 1;
						}
					}

					int check;
					for(check=0;check <pcomp->m_nPartCount; check++) 
					{
						if(flag[check] == 0) break;
					}
					
					if(check >= pcomp->m_nPartCount) //重复引脚
					{
						//将第一个设置为公用，其余的删除
						for(int k=0; k<pcomp->m_arrayPos.GetCount(); k++)
						{
							POSITION pos = pcomp->m_arrayPos.GetAt(k);
							pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
							{
								COb_schpin* ppin_1 = (COb_schpin*)pobject;

								if((ppin_1->m_nOwnerPartID > 0)&&(ppin_1->m_nOwnerPartID != ppin->m_nOwnerPartID)&&(ppin_1->m_csNumber == ppin->m_csNumber))
								{
									pcomp->m_arrayPos.RemoveAt(k);
									k--;
									delete ppin_1;
									pDoc->m_listSchObject[m_nDiagram].RemoveAt(pos);
								}
							}
						}
						ppin->m_nOwnerPartID = 0; //公用
					}
				}
			}
		}
	}

	MoveCursorToCenter();
	if(pcomp->MovePlaceComp(&dc, this) == FALSE)
	{
		//清除内存申请和元素记录
		for(int k=0; k<pcomp->m_arrayPos.GetCount(); k++)
		{
			//POSITION pos = pcomp->m_arrayPos.GetAt(k);
			//pDoc->m_listSchObject[m_nDiagram].RemoveAt(pos);
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		//pDoc->m_listSchObject[m_nDiagram].RemoveAt(posParent);
		delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
	}
	else
	{

		//----------- Undo --------------------------------------------------------------------------------
		if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_DELETE;
		pundo->m_nOb = SCH_ELEMENT_COMPONENT;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = posParent;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

		if(bEDA_AutoJunction == TRUE) AutoPlaceJunction();

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		//--------------------------------------------------------------------------------------------------

		pcomp->Draw(&dc, this);
	}

	//Invalidate();
}

/*void CESDView::OnPlaceIeee(UINT nID)
{
	int ieee_index = nID - ID_PLACE_IEEE1 + 1;

	int ch1;
	int flag;
	POSITION pos;
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作
	
	char szHelpText[] = "放置IEEE符号: <空格>旋转, <左键><回车>放置, <右键><ESC>取消, <TAB>定义属性";

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);
	
	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);	
	
	//创建对象
	COb_schieee aieee;

	aieee.pDocument = pDoc;
	aieee.m_fX = m_fCurrentSnapX;
	aieee.m_fY = m_fCurrentSnapY;
	aieee.m_fSize = cEDA_schieee.m_fSize;
	aieee.m_nOrientation = cEDA_schieee.m_nOrientation;
	aieee.m_nWidth = cEDA_schieee.m_nWidth;
	aieee.m_bMirrored = cEDA_schieee.m_bMirrored;
	aieee.m_nIeeeSymbol = ieee_index;
	aieee.m_nColor = cEDA_schieee.m_nColor;
	aieee.m_nDiagram = m_nDiagram;

	aieee.Draw(&dc, this);
	OpenMouseCursor();

	flag = aieee.m_nOrientation/90;
	Write_Help(szHelpText);
	nEDA_RedrawFlag = 0;
	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			CloseMouseCursor();
			aieee.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			//视未获得focus时不响应鼠标移动
			if(m_bActive == FALSE) continue;

			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(m_bAfterSetFocus)
			{
				m_bAfterSetFocus = false;
				continue;
			}
			
			
			if(nEDA_RedrawFlag == 1)
			{ 
				//屏幕刚重新显示过则不需要复原操作
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				CloseMouseCursor();
				aieee.Draw(&dc, this);
				OpenMouseCursor();
			}

			aieee.m_fX = m_fCurrentSnapX;  
			aieee.m_fY = m_fCurrentSnapY;
			
			CloseMouseCursor();
			aieee.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			CloseMouseCursor();
			aieee.Draw(&dc, this);
			OpenMouseCursor();

			COb_schieee* pieee = new COb_schieee;
			pieee->pDocument = pDoc;
			pieee->m_fX = aieee.m_fX;
			pieee->m_fY = aieee.m_fY;
			pieee->m_fSize = aieee.m_fSize;
			pieee->m_nOrientation = aieee.m_nOrientation;
			pieee->m_nWidth = aieee.m_nWidth;
			pieee->m_bMirrored = aieee.m_bMirrored;
			pieee->m_nIeeeSymbol = aieee.m_nIeeeSymbol;
			pieee->m_nColor = aieee.m_nColor;
			pieee->m_nDiagram = aieee.m_nDiagram;

			pos = pDoc->m_listSchObject[m_nDiagram].AddTail(pieee);

			dc.SetROP2(drawmode);
			CloseMouseCursor();
			pieee->Draw(&dc, this);
			OpenMouseCursor();
			dc.SetROP2(R2_NOTXORPEN);
												
			//----------- Undo -------------------------------------------------------------------------------------
			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_IEEE;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = pos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			//-------------------------------------------------------------------------------------------------------

			CloseMouseCursor();
			aieee.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if(ch1 == VK_SPACE)
		{ 
			CloseMouseCursor();
			aieee.Draw(&dc, this);
			
			flag++;
			if(flag>3) flag=0;

			aieee.m_fX = m_fCurrentSnapX;  
			aieee.m_fY = m_fCurrentSnapY;

			aieee.m_nOrientation = flag*90;

			aieee.Draw(&dc, this);

			OpenMouseCursor();
		}
		else if(ch1 == TAB)
		{
			CloseMouseCursor();
			aieee.Draw(&dc, this);

			CDlg_EditIeee dlg;
			dlg.m_nColor = aieee.m_nColor;
			dlg.m_nComboOrientation = aieee.m_nOrientation/90;
			dlg.m_csEditNet = cEDA_schieee.m_csNetName;
			dlg.m_nComboStyle = aieee.m_nStyle;
			dlg.m_bCheckShowNet = aieee.m_bShowNetName;

			if(dlg.DoModal() == IDOK)
			{
				aieee.m_nColor = dlg.m_nColor;
				aieee.m_nOrientation = dlg.m_nComboOrientation*90;
				aieee.m_nStyle = dlg.m_nComboStyle;
				aieee.m_bShowNetName = dlg.m_bCheckShowNet;

				cEDA_schieee.m_csNetName = dlg.m_csEditNet;
				cEDA_schieee.m_nColor = aieee.m_nColor;
				cEDA_schieee.m_nOrientation = aieee.m_nOrientation;
				cEDA_schieee.m_nStyle = aieee.m_nStyle;
				cEDA_schieee.m_bShowNetName = aieee.m_bShowNetName;

				flag = aieee.m_nOrientation/90;
			}

			aieee.Draw(&dc, this);
			OpenMouseCursor();
		}
	}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

	dc.SetROP2(drawmode);
	

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL); m_bClipState = FALSE;

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Write_Help(szEDA_Help_Ready);
}*/

void CESDView::OnEditRepeat()
{
	BOOL clear_undo_buffer_flag = FALSE;	//更新Undo Buffer需要清除后面废弃的Undo操作(因Redo产生), 但是不能清楚刚更新的内容，所以只能清除1次
	BOOL undo_flag = FALSE;  //Undo更新标记, 用来控制命令结束后m_nSCH_Undo_Num++操作

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	MoveCursorToCenter();

	//选择重放的对象
	unsigned ch1;
	char szHelpText[] = "重复放置: <左键><回车>选择对象, <右键><ESC>结束命令";

	Write_Help(szHelpText);
	do
	{
       ch1 = MouseKey();
       if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
			Struct_Multi_Selection multi_selection = SelectSCHObject(m_fCurrentX, m_fCurrentY, MULTI_SELECTION);
			int element = multi_selection.element;
			POSITION position = multi_selection.pos;
			if((element == 0)||( position == NULL)) continue;

			CObject* pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(position);

			CDlg_Edit_Repeat dlg;
			if(element == SCH_ELEMENT_COMPONENT) dlg.m_csTitleString = "复制元件";
			else if(element == SCH_ELEMENT_TEXT) 
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				if(ptext->m_nID == TEXT_TEXT) dlg.m_csTitleString = "复制文字";
				else if(ptext->m_nID == TEXT_NETLABEL) dlg.m_csTitleString = "复制网络标号";
				else continue;
			}
			else if(element == SCH_ELEMENT_BEZIER) dlg.m_csTitleString = "复制样条曲线";
			else if(element == SCH_ELEMENT_POLYLINE) dlg.m_csTitleString = "复制连线";
			else if(element == SCH_ELEMENT_POLYGON) dlg.m_csTitleString = "复制多边形";
			else if(element == SCH_ELEMENT_ELLIPSEFILL) dlg.m_csTitleString = "复制椭圆填充区";
			else if(element == SCH_ELEMENT_PIE) dlg.m_csTitleString = "复制饼形图";
			else if(element == SCH_ELEMENT_ROUNDRECT) dlg.m_csTitleString = "复制圆角矩形";
			else if(element == SCH_ELEMENT_ELLIPSE) dlg.m_csTitleString = "复制椭圆圆弧";
			else if(element == SCH_ELEMENT_ARC) dlg.m_csTitleString = "复制圆弧";
			else if(element == SCH_ELEMENT_LINE)
			{
				COb_schline* pline = (COb_schline*)pobject;
				if(pline->m_nAttrib == LINE_WIRE) dlg.m_csTitleString = "复制导线";
				else if(pline->m_nAttrib == LINE_BUS) dlg.m_csTitleString = "复制总线";
				else if(pline->m_nAttrib == LINE_BUS_ENTRY) dlg.m_csTitleString = "复制总线入口";
				else continue;
			}
			else if(element == SCH_ELEMENT_RECT) dlg.m_csTitleString = "复制矩形";
			else if(element == SCH_ELEMENT_POWER) dlg.m_csTitleString = "复制电源符号";
			else if(element == SCH_ELEMENT_PORT) dlg.m_csTitleString = "复制接口符号";
			else if(element == SCH_ELEMENT_TEXTFRAME) dlg.m_csTitleString = "复制文字框";
			else if(element == SCH_ELEMENT_JUNCTION) dlg.m_csTitleString = "复制结点";
			else if(element == SCH_ELEMENT_GRAPHIC) dlg.m_csTitleString = "复制图象";
			else continue;

			dlg.m_nEditNumber = nEDA_SchRepeatNumber;
			dlg.m_nEditNDelta = nEDA_SchRepeatNumberDelta;
			dlg.m_csEditXDelta = pDoc->Convert_XY_To_String(fEDA_SchRepeatDeltaX);
			dlg.m_csEditYDelta = pDoc->Convert_XY_To_String(fEDA_SchRepeatDeltaY);
			if(dlg.DoModal() == IDOK)
			{
				double deltax, deltay;
				int number, ndelta;
				fEDA_SchRepeatDeltaX = pDoc->Convert_String_To_XY(dlg.m_csEditXDelta);
				fEDA_SchRepeatDeltaY = pDoc->Convert_String_To_XY(dlg.m_csEditYDelta);
				nEDA_SchRepeatNumber = dlg.m_nEditNumber;
				nEDA_SchRepeatNumberDelta = dlg.m_nEditNDelta;
				deltax = fEDA_SchRepeatDeltaX;
				deltay = fEDA_SchRepeatDeltaY;
				number = nEDA_SchRepeatNumber;
				ndelta = nEDA_SchRepeatNumberDelta;

				if(element == SCH_ELEMENT_COMPONENT)
				{
					COb_schcomp* pcomp = (COb_schcomp*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_schcomp* pnewcomp = new COb_schcomp();
						pnewcomp->Copy(pcomp);
						pnewcomp->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewcomp);
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_COMPONENT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//--------------------------------------------------------------------------------------------------
						
						pnewcomp->m_fX += n*deltax;
						pnewcomp->m_fY += n*deltay;

						pnewcomp->m_arrayPos.RemoveAll();
						//引脚和图形部分
						for(int i=0; i<pcomp->m_arrayPos.GetSize(); i++)
						{
							CObject* pobject;
							POSITION subpos;
							subpos = pcomp->m_arrayPos.GetAt(i);
							pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
							{
								COb_schpin* ppin = (COb_schpin*)pobject;
										
								COb_schpin* pnewpin = new COb_schpin();
								pnewpin->Copy(ppin);
								pnewpin->m_posParent = newpos;
								pnewpin->m_fX += n*deltax;
								pnewpin->m_fY += n*deltay;
								//加入元素列表
								POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpin);

								//将pin的pos添加到sheetsymbol的pos数组中
								pnewcomp->m_arrayPos.Add(pos1);
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
							{
								COb_schieee* pieee = (COb_schieee*)pobject;
							
								COb_schieee* pnewieee = new COb_schieee();
								pnewieee->Copy(pieee);
								pnewieee->m_posParent = newpos;
								pnewieee->m_fX += n*deltax;
								pnewieee->m_fY += n*deltay;
								//加入元素列表
								POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewieee);

								//将pin的pos添加到sheetsymbol的pos数组中
								pnewcomp->m_arrayPos.Add(pos1);
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
							{
								COb_schtext* ptext = (COb_schtext*)pobject;
										
								COb_schtext* pnewtext = new COb_schtext();
								pnewtext->Copy(ptext);
								pnewtext->m_posParent = newpos;
								pnewtext->m_fSx += n*deltax;
								pnewtext->m_fSy += n*deltay;
								pnewtext->m_bSelection = FALSE;
								if(pnewtext->m_nID == TEXT_COMPDESIGNATOR)
								{
									int index = pnewtext->m_csText.GetLength();
									char c = (pnewtext->m_csText.GetAt(index - 1))&0x007f;
									if(isdigit(c) != 0)
									{
										int base = c - 48;
										base += n*ndelta;
										CString str;
										str.Format("%d", base);
										pnewtext->m_csText = pnewtext->m_csText.Left(index-1);
										pnewtext->m_csText += str;
									}
								}
								//加入元素列表
								POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtext);

								//将pin的pos添加到sheetsymbol的pos数组中
								pnewcomp->m_arrayPos.Add(pos1);
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
							{
								COb_schbezier* pbezier = (COb_schbezier*)pobject;
										
								COb_schbezier* pnewbezier = new COb_schbezier();
								pnewbezier->Copy(pbezier);
								pnewbezier->m_posParent = newpos;
								for(int k=0; k<pnewbezier->m_cVertex.GetCount(); k++)
								{
									pnewbezier->m_cVertex[k].fx += n*deltax;
									pnewbezier->m_cVertex[k].fy += n*deltay;
								}
								//加入元素列表
								POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewbezier);

								//将pin的pos添加到sheetsymbol的pos数组中
								pnewcomp->m_arrayPos.Add(pos1);				
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
							{
								COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;

								COb_schpolyline* pnewpolyline = new COb_schpolyline();
								pnewpolyline->Copy(ppolyline);
								pnewpolyline->m_posParent = newpos;
								for(int k=0; k<pnewpolyline->m_cVertex.GetCount(); k++)
								{
									pnewpolyline->m_cVertex[k].fx += n*deltax;
									pnewpolyline->m_cVertex[k].fy += n*deltay;
								}
								//加入元素列表
								POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolyline);

								//将pin的pos添加到sheetsymbol的pos数组中
								pnewcomp->m_arrayPos.Add(pos1);				
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
							{
								COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
								
								COb_schpolygon* pnewpolygon = new COb_schpolygon();
								pnewpolygon->Copy(ppolygon);
								pnewpolygon->m_posParent = newpos;
								for(int k=0; k<pnewpolygon->m_cVertex.GetCount(); k++)
								{
									pnewpolygon->m_cVertex[k].fx += n*deltax;
									pnewpolygon->m_cVertex[k].fy += n*deltay;
								}
								//加入元素列表
								POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolygon);

								//将pin的pos添加到sheetsymbol的pos数组中
								pnewcomp->m_arrayPos.Add(pos1);					
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
							{
								COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
										
								COb_schellipsefill* pnewellipsefill = new COb_schellipsefill();
								pnewellipsefill->Copy(pellipsefill);
								pnewellipsefill->m_posParent = newpos;
								pnewellipsefill->m_fCx += n*deltax;
								pnewellipsefill->m_fCy += n*deltay;
								//加入元素列表
								POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipsefill);

								//将pin的pos添加到sheetsymbol的pos数组中
								pnewcomp->m_arrayPos.Add(pos1);	
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
							{
								COb_schpie* ppie = (COb_schpie*)pobject;
								
								COb_schpie* pnewpie = new COb_schpie();
								pnewpie->Copy(ppie);
								pnewpie->m_posParent = newpos;
								pnewpie->m_fCx += n*deltax;
								pnewpie->m_fCy += n*deltay;
								//加入元素列表
								POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpie);

								//将pin的pos添加到sheetsymbol的pos数组中
								pnewcomp->m_arrayPos.Add(pos1);	
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
							{
								COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
								
								COb_schroundrect* pnewroundrect = new COb_schroundrect();
								pnewroundrect->Copy(proundrect);
								pnewroundrect->m_posParent = newpos;
								pnewroundrect->m_fX1 += n*deltax;
								pnewroundrect->m_fY1 += n*deltay;
								pnewroundrect->m_fX2 += n*deltax;
								pnewroundrect->m_fY2 += n*deltay;
								//加入元素列表
								POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewroundrect);

								//将pin的pos添加到sheetsymbol的pos数组中
								pnewcomp->m_arrayPos.Add(pos1);	
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
							{
								COb_schellipse* pellipse = (COb_schellipse*)pobject;
								
								COb_schellipse* pnewellipse = new COb_schellipse();
								pnewellipse->Copy(pellipse);
								pnewellipse->m_posParent = newpos;
								pnewellipse->m_fCx += n*deltax;
								pnewellipse->m_fCy += n*deltay;
								//加入元素列表
								POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipse);

								//将pin的pos添加到sheetsymbol的pos数组中
								pnewcomp->m_arrayPos.Add(pos1);	
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
							{
								COb_scharc* parc = (COb_scharc*)pobject;

								COb_scharc* pnewarc = new COb_scharc();
								pnewarc->Copy(parc);
								pnewarc->m_posParent = newpos;
								pnewarc->m_fCx += n*deltax;
								pnewarc->m_fCy += n*deltay;
								//加入元素列表
								POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewarc);

								//将pin的pos添加到sheetsymbol的pos数组中
								pnewcomp->m_arrayPos.Add(pos1);	
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
							{
								COb_schrect* prect = (COb_schrect*)pobject;
										
								COb_schrect* pnewrect = new COb_schrect();
								pnewrect->Copy(prect);
								pnewrect->m_posParent = newpos;
								pnewrect->m_fX1 += n*deltax;
								pnewrect->m_fY1 += n*deltay;
								pnewrect->m_fX2 += n*deltax;
								pnewrect->m_fY2 += n*deltay;
								//加入元素列表
								POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewrect);

								//将pin的pos添加到sheetsymbol的pos数组中
								pnewcomp->m_arrayPos.Add(pos1);	
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
							{
								COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
										
								COb_schtextframe* pnewtextframe = new COb_schtextframe();
								pnewtextframe->Copy(ptextframe);
								pnewtextframe->m_posParent = newpos;
								pnewtextframe->m_fX1 += n*deltax;
								pnewtextframe->m_fY1 += n*deltay;
								pnewtextframe->m_fX2 += n*deltax;
								pnewtextframe->m_fY2 += n*deltay;
								//加入元素列表
								POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtextframe);

								//将pin的pos添加到sheetsymbol的pos数组中
								pnewcomp->m_arrayPos.Add(pos1);	
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
							{
								COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
										
								COb_schgraphic* pnewgraphic = new COb_schgraphic();
								pnewgraphic->Copy(pgraphic);
								pnewgraphic->m_posParent = newpos;
								pnewgraphic->m_fX1 += n*deltax;
								pnewgraphic->m_fY1 += n*deltay;
								pnewgraphic->m_fX2 += n*deltax;
								pnewgraphic->m_fY2 += n*deltay;
								//加入元素列表
								POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewgraphic);

								//将pin的pos添加到sheetsymbol的pos数组中
								pnewcomp->m_arrayPos.Add(pos1);	
							}
						}
					}
				}
				else if(element == SCH_ELEMENT_TEXT)  //TEXT, NETLABEL, SHEETSYMBOLDESIGNATOR, SHEETSYMBOLFILENAME, COMPDESIGNATOR, COMPCOMMENT, COMPTEXT, 
				{

					COb_schtext* ptext = (COb_schtext*)pobject;
					if((ptext->m_nID == TEXT_TEXT) || (ptext->m_nID == TEXT_NETLABEL))
					{
						for(int n=1; n<=number; n++)
						{						
							COb_schtext* pnewtext = new COb_schtext();
							pnewtext->Copy(ptext);
							pnewtext->m_bSelection = FALSE;
							POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtext);
							//----------- Undo --------------------------------------------------------------------------------
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = SCH_ELEMENT_TEXT;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = newpos;
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							//--------------------------------------------------------------------------------------------------
							pnewtext->m_fSx += n*deltax;
							pnewtext->m_fSy += n*deltay;

							int index = pnewtext->m_csText.GetLength();
							char c = (pnewtext->m_csText.GetAt(index - 1))&0x007f;
							if(isdigit(c) != 0)
							{
								int base = c - 48;
								base += n*ndelta;
								CString str;
								str.Format("%d", base);
								pnewtext->m_csText = pnewtext->m_csText.Left(index-1);
								pnewtext->m_csText += str;
							}
						}
					}
				}
				else if(element == SCH_ELEMENT_BEZIER)
				{
					COb_schbezier* pbezier = (COb_schbezier*)pobject;
					for(int n=1; n<=number; n++)
					{	
						COb_schbezier* pnewbezier = new COb_schbezier();
						pnewbezier->Copy(pbezier);
						pnewbezier->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewbezier);
						for(int k=0; k<pnewbezier->m_cVertex.GetCount(); k++)
						{
							pnewbezier->m_cVertex[k].fx += n*deltax;
							pnewbezier->m_cVertex[k].fy += n*deltay;
						}

						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_BEZIER;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//--------------------------------------------------------------------------------------------------
					}
				}
				else if(element == SCH_ELEMENT_POLYLINE)
				{
					COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_schpolyline* pnewpolyline = new COb_schpolyline();
						pnewpolyline->Copy(ppolyline);
						pnewpolyline->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolyline);
								
						for(int k=0; k<pnewpolyline->m_cVertex.GetCount(); k++)
						{
							pnewpolyline->m_cVertex[k].fx += n*deltax;
							pnewpolyline->m_cVertex[k].fy += n*deltay;
						}
						
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_POLYLINE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//--------------------------------------------------------------------------------------------------
					}
				}
				else if(element == SCH_ELEMENT_POLYGON)
				{
					COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_schpolygon* pnewpolygon = new COb_schpolygon();
						pnewpolygon->Copy(ppolygon);
						pnewpolygon->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolygon);
								
						for(int k=0; k<pnewpolygon->m_cVertex.GetCount(); k++)
						{
							pnewpolygon->m_cVertex[k].fx += n*deltax;
							pnewpolygon->m_cVertex[k].fy += n*deltay;
						}

						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_POLYGON;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//--------------------------------------------------------------------------------------------------
					}
				}
				else if(element == SCH_ELEMENT_ELLIPSEFILL)
				{
					COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_schellipsefill* pnewellipsefill = new COb_schellipsefill();
						pnewellipsefill->Copy(pellipsefill);
						pnewellipsefill->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipsefill);
						pnewellipsefill->m_fCx += n*deltax;
						pnewellipsefill->m_fCy += n*deltay;
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//--------------------------------------------------------------------------------------------------	
					}
				}
				else if(element == SCH_ELEMENT_PIE)
				{
					COb_schpie* ppie = (COb_schpie*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_schpie* pnewpie = new COb_schpie();
						pnewpie->Copy(ppie);
						pnewpie->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpie);
						pnewpie->m_fCx += n*deltax;
						pnewpie->m_fCy += n*deltay;
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_PIE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//---------------------------------------------------------------------------------------------------
					}
				}
				else if(element == SCH_ELEMENT_ROUNDRECT)
				{
					COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_schroundrect* pnewroundrect = new COb_schroundrect();
						pnewroundrect->Copy(proundrect);
						pnewroundrect->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewroundrect);
						pnewroundrect->m_fX1 += n*deltax;
						pnewroundrect->m_fY1 += n*deltay;
						pnewroundrect->m_fX2 += n*deltax;
						pnewroundrect->m_fY2 += n*deltay;						
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//---------------------------------------------------------------------------------------------------
					}
				}
				else if(element == SCH_ELEMENT_ELLIPSE)
				{
					COb_schellipse* pellipse = (COb_schellipse*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_schellipse* pnewellipse = new COb_schellipse();
						pnewellipse->Copy(pellipse);
						pnewellipse->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipse);
						pnewellipse->m_fCx += n*deltax;
						pnewellipse->m_fCy += n*deltay;						
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//---------------------------------------------------------------------------------------------------
					}
				}
				else if(element == SCH_ELEMENT_ARC)
				{
					COb_scharc* parc = (COb_scharc*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_scharc* pnewarc = new COb_scharc();
						pnewarc->Copy(parc);
						pnewarc->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewarc);
						pnewarc->m_fCx += n*deltax;
						pnewarc->m_fCy += n*deltay;						
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_ARC;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//---------------------------------------------------------------------------------------------------
					}
				}
				else if(element == SCH_ELEMENT_LINE)	//WIRE, BUS, BUS_ENTRY 入口
				{
					COb_schline* pline = (COb_schline*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_schline* pnewline = new COb_schline();
						pnewline->Copy(pline);
						pnewline->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewline);
						for(int k=0; k<pnewline->m_cVertex.GetCount(); k++)
						{
							pnewline->m_cVertex[k].fx += n*deltax;
							pnewline->m_cVertex[k].fy += n*deltay;
						}						
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_LINE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//---------------------------------------------------------------------------------------------------
					}
				}
				else if(element == SCH_ELEMENT_RECT)
				{
					COb_schrect* prect = (COb_schrect*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_schrect* pnewrect = new COb_schrect();
						pnewrect->Copy(prect);
						pnewrect->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewrect);
						pnewrect->m_fX1 += n*deltax;
						pnewrect->m_fY1 += n*deltay;
						pnewrect->m_fX2 += n*deltax;
						pnewrect->m_fY2 += n*deltay;						
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_RECT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//---------------------------------------------------------------------------------------------------
					}
				}
				else if(element == SCH_ELEMENT_POWER)
				{
					COb_schpower* ppower = (COb_schpower*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_schpower* pnewpower = new COb_schpower();
						pnewpower->Copy(ppower);
						pnewpower->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpower);
						pnewpower->m_fX += n*deltax;
						pnewpower->m_fY += n*deltay;
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_POWER;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//---------------------------------------------------------------------------------------------------
					}
				}
				else if(element == SCH_ELEMENT_PORT)
				{
					COb_schport* pport = (COb_schport*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_schport* pnewport = new COb_schport();
						pnewport->Copy(pport);
						pnewport->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewport);
						pnewport->m_fX += n*deltax;
						pnewport->m_fY += n*deltay;
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_PORT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//---------------------------------------------------------------------------------------------------
					}
				}
				else if(element == SCH_ELEMENT_TEXTFRAME)
				{
					COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_schtextframe* pnewtextframe = new COb_schtextframe();
						pnewtextframe->Copy(ptextframe);
						pnewtextframe->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtextframe);
						pnewtextframe->m_fX1 += n*deltax;
						pnewtextframe->m_fY1 += n*deltay;
						pnewtextframe->m_fX2 += n*deltax;
						pnewtextframe->m_fY2 += n*deltay;						
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//---------------------------------------------------------------------------------------------------
					}
				}
				else if(element == SCH_ELEMENT_JUNCTION)
				{
					COb_schjunc* pjunc = (COb_schjunc*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_schjunc* pnewjunc = new COb_schjunc();
						pnewjunc->Copy(pjunc);
						pnewjunc->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewjunc);
						pnewjunc->m_fCx += n*deltax;
						pnewjunc->m_fCy += n*deltay;
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_JUNCTION;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//---------------------------------------------------------------------------------------------------
					}
				}
				else if(element == SCH_ELEMENT_GRAPHIC)
				{
					COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
					for(int n=1; n<=number; n++)
					{
						COb_schgraphic* pnewgraphic = new COb_schgraphic();
						pnewgraphic->Copy(pgraphic);
						pnewgraphic->m_bSelection = FALSE;
						POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewgraphic);
						pnewgraphic->m_fX1 += n*deltax;
						pnewgraphic->m_fY1 += n*deltay;
						pnewgraphic->m_fX2 += n*deltax;
						pnewgraphic->m_fY2 += n*deltay;										
						//----------- Undo --------------------------------------------------------------------------------
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = newpos;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						//---------------------------------------------------------------------------------------------------
					}
				}
				else if(element == SCH_ELEMENT_SHEET)
				{
					//COb_schellipse* pellipse = (COb_schellipse*)pobject;
					//pellipse->Move();
				}

			}
			break;
	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
	
	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
}

void CESDView::OnEditDuplicate()
{
	int ch1;
	int flag;
	BOOL mirror;
	double x0, y0, x1, y1, deltax, deltay;

	char szHelp[] = "复制选择对象: <左键><回车>复制, <空格>旋转, <X键>镜像, <右键><ESC>结束命令";
	
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	pDoc->m_fpSelection[m_nDiagram] = GetSelectionCenter();
	if(pDoc->m_arraySelection[m_nDiagram].GetCount() == 0) return;

	//要考虑pDoc->m_fSelection_X1和pDoc->m_fSelection_Y1的网格锁定
	double fSelection_X1, fSelection_Y1, fSelection_X2, fSelection_Y2;
	fSelection_X1 = Snap_Lock(pDoc->m_fSelection_X1[m_nDiagram]);
	fSelection_Y1 = Snap_Lock(pDoc->m_fSelection_Y1[m_nDiagram]);
	fSelection_X2 = Snap_Lock(pDoc->m_fSelection_X2[m_nDiagram]);
	fSelection_Y2 = Snap_Lock(pDoc->m_fSelection_Y2[m_nDiagram]);

	x0 = fSelection_X1;
	y0 = fSelection_Y1;
	m_fCurrentSnapX = x0;
	m_fCurrentSnapY = y0;
	Restore_LargeCrossXY();

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);	

	//复制对象
	COb_schrect arect;
	arect.pDocument = pDoc;
	arect.m_fX1 = fSelection_X1;
	arect.m_fY1 = fSelection_Y1;
	arect.m_fX2 = fSelection_X2;
	arect.m_fY2 = fSelection_Y2;
	arect.m_nBorderWidth = 0;
	arect.m_nFillColor = RGB(255,255,255);
	arect.m_nBorderColor = RGB(0,0,0);
	arect.m_bTransparent = FALSE;
	arect.m_bFillSolid = FALSE;
	arect.m_nDiagram = m_nDiagram;
	arect.Draw(&dc, this);

	nEDA_RedrawFlag = 0;
	Write_Help(szHelp);
	flag = 0;
	mirror = FALSE;
	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			CloseMouseCursor();
			arect.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if(ch1 == MOUSEMOVE)
		{
			//视未获得focus时不响应鼠标移动
			if(m_bActive == FALSE) continue;
			// if we just got a setfocus, we get one 'free' mousemove message from windows.  we have to ignore this one, otherwise the drawing gets messed up
			if(m_bAfterSetFocus)
			{
				m_bAfterSetFocus = false;
				continue;
			}
					
			if(nEDA_RedrawFlag == 1)
			{ 
				//屏幕刚重新显示过则不需要复原操作
				nEDA_RedrawFlag = 0;
			}
			else  
			{
				CloseMouseCursor();
				arect.Draw(&dc, this);
				OpenMouseCursor();
			}

			x1 = m_fCurrentSnapX;  
			y1 = m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, fSelection_X1 - x0, fSelection_Y1 - y0, flag*90, mirror);
			arect.m_fX1 = fx + x0 + deltax;
			arect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, fSelection_X2 - x0, fSelection_Y2 - y0, flag*90, mirror);
			arect.m_fX2 = fx + x0 + deltax;
			arect.m_fY2 = fy + y0 + deltay;


			CloseMouseCursor();
			arect.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
		{
			CloseMouseCursor();
			arect.Draw(&dc, this);
			OpenMouseCursor();				
					
			x1 = m_fCurrentSnapX;  
			y1 = m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;
					
			CopyAndRotateSelection( x0, y0, deltax, deltay, flag*90, mirror);

			break;
		}
		else if(ch1 == SPACE)
		{
			CloseMouseCursor();
			arect.Draw(&dc, this);
			OpenMouseCursor();

			flag++;
			if(flag>3) flag=0;

			x1 = m_fCurrentSnapX;  
			y1 = m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, fSelection_X1 - x0, fSelection_Y1 - y0, flag*90, mirror);
			arect.m_fX1 = fx + x0 + deltax;
			arect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, fSelection_X2 - x0, fSelection_Y2 - y0, flag*90, mirror);
			arect.m_fX2 = fx + x0 + deltax;
			arect.m_fY2 = fy + y0 + deltay;

			CloseMouseCursor();
			arect.Draw(&dc, this);
			OpenMouseCursor();
		}
		else if((ch1 == KEY_x)||(ch1 == KEY_X))
		{
			CloseMouseCursor();
			arect.Draw(&dc, this);
			OpenMouseCursor();

			if(mirror == FALSE) mirror = TRUE;
			else mirror = FALSE;

			x1 = m_fCurrentSnapX;  
			y1 = m_fCurrentSnapY;
			deltax = x1 - x0;
			deltay = y1 - y0;

			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, fSelection_X1 - x0, fSelection_Y1 - y0, flag*90, mirror);
			arect.m_fX1 = fx + x0 + deltax;
			arect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, fSelection_X2 - x0, fSelection_Y2 - y0, flag*90, mirror);
			arect.m_fX2 = fx + x0 + deltax;
			arect.m_fY2 = fy + y0 + deltay;

			CloseMouseCursor();
			arect.Draw(&dc, this);
			OpenMouseCursor();
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;

	Invalidate();
}

void CESDView::OnUpdateEditDuplicate(CCmdUI *pCmdUI)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(pDoc->m_arraySelection[m_nDiagram].GetCount() > 0)
	{
		pCmdUI->Enable(TRUE);
		//pCmdUI->SetCheck(TRUE);
	}
	else pCmdUI->Enable(FALSE);
}