void CESDView::OnEditChange()
{
	unsigned ch1;

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	
	
	Write_Help("选择对象修改...");
	do
	{
       ch1 = MouseKey();
       if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
		   if(m_nSystem_State == SCH)
		   {
				Struct_Multi_Selection multi_selection = SelectSCHObject(m_fCurrentX, m_fCurrentY, MULTI_SELECTION);
				EditElement(multi_selection.element, multi_selection.pos);
		   }
		   else
		   {
				Struct_Pcb_Multi_Selection multi_selection = SelectPcbObject(m_fCurrentX, m_fCurrentY, INCLUDE_CPAD);
				PcbEditElement(multi_selection.pcbelement, multi_selection.index);
		   }
	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::EditElement(int element, POSITION position)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if((element == 0)||( position == NULL)) return;

	pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(position);

	if(element == SCH_ELEMENT_COMPONENT)
	{
		COb_schcomp* pcomp = (COb_schcomp*)pobject;
		pcomp->Change(position);
	}
	else if(element == SCH_ELEMENT_PIN)
	{
		COb_schpin* ppin = (COb_schpin*)pobject;
		ppin->Change(position);
	}
	else if(element == SCH_ELEMENT_IEEE)
	{
		COb_schieee* pieee = (COb_schieee*)pobject;
		pieee->Change();
	}
	else if(element == SCH_ELEMENT_TEXT)  
	{
		COb_schtext* ptext = (COb_schtext*)pobject;
		ptext->Change(position);
	}
	else if(element == SCH_ELEMENT_BEZIER)
	{
		COb_schbezier* pbezier = (COb_schbezier*)pobject;
		pbezier->Change(position);
	}
	else if(element == SCH_ELEMENT_POLYLINE)
	{
		COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
		ppolyline->Change(position);
	}
	else if(element == SCH_ELEMENT_POLYGON)
	{
		COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
		ppolygon->Change(position);
	}
	else if(element == SCH_ELEMENT_ELLIPSEFILL)
	{
		COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
		pellipsefill->Change(position);
	}
	else if(element == SCH_ELEMENT_PIE)
	{
		COb_schpie* ppie = (COb_schpie*)pobject;
		ppie->Change(position);
	}
	else if(element == SCH_ELEMENT_ROUNDRECT)
	{
		COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
		proundrect->Change(position);
	}
	else if(element == SCH_ELEMENT_ELLIPSE)
	{
		COb_schellipse* pellipse = (COb_schellipse*)pobject;
		pellipse->Change(position);
	}
	else if(element == SCH_ELEMENT_ARC)
	{
		COb_scharc* parc = (COb_scharc*)pobject;
		parc->Change(position);
	}
	else if(element == SCH_ELEMENT_LINE)	
	{
		COb_schline* pline = (COb_schline*)pobject;
		pline->Change(position);
	}
	else if(element == SCH_ELEMENT_RECT)
	{
		COb_schrect* prect = (COb_schrect*)pobject;
		prect->Change(position);
	}
	else if(element == SCH_ELEMENT_SHEETSYMBOL)
	{
		COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
		psheetsymbol->Change(position);
	}
	else if(element == SCH_ELEMENT_SHEETENTRY)
	{
		COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
		psheetentry->Change(position);
	}
	else if(element == SCH_ELEMENT_POWER)
	{
		COb_schpower* ppower = (COb_schpower*)pobject;
		ppower->Change(position);
	}
	else if(element == SCH_ELEMENT_PORT)
	{
		COb_schport* pport = (COb_schport*)pobject;
		pport->Change(position);
	}
	else if(element == SCH_ELEMENT_TEXTFRAME)
	{
		COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
		ptextframe->Change(position);
	}
	else if(element == SCH_ELEMENT_JUNCTION)
	{
		COb_schjunc* pjunc = (COb_schjunc*)pobject;
		pjunc->Change(position);
	}
	else if(element == SCH_ELEMENT_GRAPHIC)
	{
		COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
		pgraphic->Change(position);
	}
	else if(element == SCH_ELEMENT_SHEET)
	{
		
	}
}

void CESDView::AdvancedChangeMode(double fx, double fy)
{
	CClientDC dc(this);	
	
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	POSITION position;
	for(int i=0; i<pDoc->m_arraySelection[m_nDiagram].GetCount(); i++)
	{
		position = pDoc->m_arraySelection[m_nDiagram].GetAt(i);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(position);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
			if(pline->m_nAttrib == LINE_BUS_ENTRY) continue;
			for(int k=0; k<pline->m_cVertex.GetCount(); k++)  
			{
				double fx1, fy1, fx2, fy2;
				fx1 = pline->m_cVertex[k].fx - nEDA_Selection_Offset;
				fy1 = pline->m_cVertex[k].fy - nEDA_Selection_Offset;
				fx2 = pline->m_cVertex[k].fx + nEDA_Selection_Offset;
				fy2 = pline->m_cVertex[k].fy + nEDA_Selection_Offset;
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2))
				{
					
					int ix,iy;
					Get_VPort_XY(&ix, &iy, pline->m_cVertex[k].fx, pline->m_cVertex[k].fy);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					pline->m_nFlag = FLAG_DELETED;
					pline->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schline aline;
					aline.pDocument = pDoc;
					aline.m_nAttrib = pline->m_nAttrib;
					aline.m_nLineWidth = pline->m_nLineWidth;
					aline.m_nLineType = pline->m_nLineType;
					aline.m_nColor = pline->m_nColor;
					aline.m_nDiagram = pline->m_nDiagram;
					aline.m_nFlag = FLAG_NORMAL;
					
					for(int i=0; i<pline->m_cVertex.GetCount(); i++)
					{
						Fpoint fpoint;
						fpoint = pline->m_cVertex.GetAt(i);
						aline.m_cVertex.Add(fpoint);
					}

					aline.Draw(&dc, this);
						
					
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
								ElecHotPointsControl(&dc);

								CloseMouseCursor();
								aline.Draw(&dc, this);
								OpenMouseCursor();
							}

							aline.m_cVertex[k].fx = m_fCurrentSnapX;
							aline.m_cVertex[k].fy = m_fCurrentSnapY;

							CloseMouseCursor();
							aline.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							ElecHotPointsControl(&dc);

							CloseMouseCursor();
							aline.Draw(&dc, this);
							OpenMouseCursor();

							aline.m_cVertex[k].fx = m_fCurrentSnapX;
							aline.m_cVertex[k].fy = m_fCurrentSnapY;
		
							
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;

							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = SCH_ELEMENT_LINE;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = position;
							pundo->schline.Copy(pline);
							pundo->schline.m_nFlag = FLAG_NORMAL;
	
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							aline.CombineVertex();
							pline->m_cVertex.RemoveAll();
							for(int i=0; i<aline.m_cVertex.GetCount(); i++)
							{
								Fpoint fpoint;
								fpoint = aline.m_cVertex.GetAt(i);
								pline->m_cVertex.Add(fpoint);
							}
							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					pline->m_nFlag = FLAG_NORMAL;
					pline->Draw(&dc, this);
					pline->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						if(pline->m_nAttrib == LINE_WIRE)
						{
							if(bEDA_AutoOptimizeWire == TRUE) AutoOptimizeWire();
							if(bEDA_AutoJunction == TRUE)
							{
								AutoPlaceJunction();
								AutoRemoveJunction();
							}
						}

						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);

					bEDA_NeedElectricalGrid = FALSE;
					m_fpHotPoint1.fx = m_fpHotPoint1.fy = m_fpHotPoint2.fx = m_fpHotPoint2.fy = 0;

					return;
				}
			}
			for(int k=1; k<pline->m_cVertex.GetCount(); k++)  
			{
				double fx1, fy1, fx2, fy2;
				fx1 = (pline->m_cVertex[k-1].fx+pline->m_cVertex[k].fx)/2 - nEDA_Selection_Offset;
				fy1 = (pline->m_cVertex[k-1].fy+pline->m_cVertex[k].fy)/2 - nEDA_Selection_Offset;
				fx2 = (pline->m_cVertex[k-1].fx+pline->m_cVertex[k].fx)/2 + nEDA_Selection_Offset;
				fy2 = (pline->m_cVertex[k-1].fy+pline->m_cVertex[k].fy)/2 + nEDA_Selection_Offset;
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2))
				{
					
					int ix,iy;
					Get_VPort_XY(&ix, &iy, pline->m_cVertex[k].fx, pline->m_cVertex[k].fy);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, pline->m_cVertex[k-1].fx, pline->m_cVertex[k-1].fy);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, (pline->m_cVertex[k-1].fx + pline->m_cVertex[k].fx)/2, (pline->m_cVertex[k-1].fy + pline->m_cVertex[k].fy)/2);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					pline->m_nFlag = FLAG_DELETED;
					pline->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schline aline;
					aline.pDocument = pDoc;
					aline.m_nAttrib = pline->m_nAttrib;
					aline.m_nLineWidth = pline->m_nLineWidth;
					aline.m_nLineType = pline->m_nLineType;
					aline.m_nColor = pline->m_nColor;
					aline.m_nDiagram = pline->m_nDiagram;
					aline.m_nFlag = FLAG_NORMAL;
					
					int break_flag = 0;
					if((GetKeyState(VK_CONTROL)&0x80) != 0 )
					{
						
						break_flag = 1;
						Fpoint fpoint;
						for(int i=0; i<k; i++)
						{
							fpoint = pline->m_cVertex.GetAt(i);
							aline.m_cVertex.Add(fpoint);
						}
						
						fpoint.fx = (pline->m_cVertex[k-1].fx + pline->m_cVertex[k].fx)/2;
						fpoint.fy = (pline->m_cVertex[k-1].fy + pline->m_cVertex[k].fy)/2;
						aline.m_cVertex.Add(fpoint);
						for(int i=k; i<pline->m_cVertex.GetCount(); i++)
						{
							fpoint = pline->m_cVertex.GetAt(i);
							aline.m_cVertex.Add(fpoint);
						}
					}
					else
					{
						for(int i=0; i<pline->m_cVertex.GetCount(); i++)
						{
							Fpoint fpoint;
							fpoint = pline->m_cVertex.GetAt(i);
							aline.m_cVertex.Add(fpoint);
						}
					}

					aline.Draw(&dc, this);
				
					flag = 0;
					Write_Help(szHelp);
					nEDA_RedrawFlag = 0;
					do
					{
						ch1 = MouseKey();
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							CloseMouseCursor();
							aline.Draw(&dc, this);
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
								aline.Draw(&dc, this);
								OpenMouseCursor();
							}
							if(break_flag == 1)
							{
								aline.m_cVertex[k].fx = m_fCurrentSnapX;
								aline.m_cVertex[k].fy = m_fCurrentSnapY;
							}
							else
							{
								double deltax = m_fCurrentSnapX - Snap_Lock((pline->m_cVertex[k-1].fx + pline->m_cVertex[k].fx)/2);
								double deltay = m_fCurrentSnapY - Snap_Lock((pline->m_cVertex[k-1].fy + pline->m_cVertex[k].fy)/2);
								aline.m_cVertex[k-1].fx = pline->m_cVertex[k-1].fx + deltax;
								aline.m_cVertex[k-1].fy = pline->m_cVertex[k-1].fy + deltay;
								aline.m_cVertex[k].fx = pline->m_cVertex[k].fx + deltax;
								aline.m_cVertex[k].fy = pline->m_cVertex[k].fy + deltay;
							}

							CloseMouseCursor();
							aline.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							aline.Draw(&dc, this);
							OpenMouseCursor();

		
							if(break_flag == 1)
							{
								
								if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
								{
									pDoc->Clear_Discarded_Undo_Buffer();
								}
								clear_undo_buffer_flag = TRUE;

								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_LINE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = position;
								pundo->schline.Copy(pline);
								pundo->schline.m_nFlag = FLAG_NORMAL;
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								undo_flag = TRUE;
								
								aline.CombineVertex();
								pline->m_cVertex.RemoveAll();
								for(int i=0; i<aline.m_cVertex.GetCount(); i++)
								{
									Fpoint fpoint;
									fpoint = aline.m_cVertex.GetAt(i);
									pline->m_cVertex.Add(fpoint);
								}
							}
							else
							{
								
								if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
								{
									pDoc->Clear_Discarded_Undo_Buffer();
								}
								clear_undo_buffer_flag = TRUE;

								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_LINE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = position;
								pundo->schline.Copy(pline);
								pundo->schline.m_nFlag = FLAG_NORMAL;
		
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								undo_flag = TRUE;
								

								aline.CombineVertex();
								pline->m_cVertex.RemoveAll();
								for(int i=0; i<aline.m_cVertex.GetCount(); i++)
								{
									Fpoint fpoint;
									fpoint = aline.m_cVertex.GetAt(i);
									pline->m_cVertex.Add(fpoint);
								}

							}
							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					pline->m_nFlag = FLAG_NORMAL;
					pline->Draw(&dc, this);
					pline->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						if(pline->m_nAttrib == LINE_WIRE)
						{
							if(bEDA_AutoOptimizeWire == TRUE) AutoOptimizeWire();
							if(bEDA_AutoJunction == TRUE)
							{
								AutoPlaceJunction();
								AutoRemoveJunction();
							}
						}

						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);
					return;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
			{
				double fx1, fy1, fx2, fy2;
				fx1 = ppolyline->m_cVertex[k].fx - nEDA_Selection_Offset;
				fy1 = ppolyline->m_cVertex[k].fy - nEDA_Selection_Offset;
				fx2 = ppolyline->m_cVertex[k].fx + nEDA_Selection_Offset;
				fy2 = ppolyline->m_cVertex[k].fy + nEDA_Selection_Offset;
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2))
				{
					
					int ix,iy;
					Get_VPort_XY(&ix, &iy, ppolyline->m_cVertex[k].fx, ppolyline->m_cVertex[k].fy);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					ppolyline->m_nFlag = FLAG_DELETED;
					ppolyline->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schpolyline apolyline;
					apolyline.pDocument = pDoc;
					apolyline.m_nLineWidth = ppolyline->m_nLineWidth;
					apolyline.m_nLineType = ppolyline->m_nLineType;
					apolyline.m_nStartLineShape = ppolyline->m_nStartLineShape;
					apolyline.m_nEndLineShape = ppolyline->m_nEndLineShape;
					apolyline.m_nLineShapeSize = ppolyline->m_nLineShapeSize;
					apolyline.m_nColor = ppolyline->m_nColor;
					apolyline.m_nDiagram = ppolyline->m_nDiagram;
					apolyline.m_nFlag = FLAG_NORMAL;
					
					for(int i=0; i<ppolyline->m_cVertex.GetCount(); i++)
					{
						Fpoint fpoint;
						fpoint = ppolyline->m_cVertex.GetAt(i);
						apolyline.m_cVertex.Add(fpoint);
					}

					apolyline.Draw(&dc, this);

					flag = 0;
					Write_Help(szHelp);
					nEDA_RedrawFlag = 0;
					do
					{
						ch1 = MouseKey();
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							CloseMouseCursor();
							apolyline.Draw(&dc, this);
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
								apolyline.Draw(&dc, this);
								OpenMouseCursor();
							}

							apolyline.m_cVertex[k].fx = m_fCurrentSnapX;
							apolyline.m_cVertex[k].fy = m_fCurrentSnapY;

							CloseMouseCursor();
							apolyline.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							apolyline.Draw(&dc, this);
							OpenMouseCursor();

							apolyline.m_cVertex[k].fx = m_fCurrentSnapX;
							apolyline.m_cVertex[k].fy = m_fCurrentSnapY;
		
							
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;

							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = SCH_ELEMENT_POLYLINE;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = position;
							pundo->schpolyline.Copy(ppolyline);
							pundo->schpolyline.m_nFlag = FLAG_NORMAL;
	
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							apolyline.CombineVertex();
							ppolyline->m_cVertex.RemoveAll();
							for(int i=0; i<apolyline.m_cVertex.GetCount(); i++)
							{
								Fpoint fpoint;
								fpoint = apolyline.m_cVertex.GetAt(i);
								ppolyline->m_cVertex.Add(fpoint);
							}

							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					ppolyline->m_nFlag = FLAG_NORMAL;
					ppolyline->Draw(&dc, this);
					ppolyline->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);
					return;
				}
			}
			for(int k=1; k<ppolyline->m_cVertex.GetCount(); k++)  
			{
				double fx1, fy1, fx2, fy2;
				fx1 = (ppolyline->m_cVertex[k-1].fx+ppolyline->m_cVertex[k].fx)/2 - nEDA_Selection_Offset;
				fy1 = (ppolyline->m_cVertex[k-1].fy+ppolyline->m_cVertex[k].fy)/2 - nEDA_Selection_Offset;
				fx2 = (ppolyline->m_cVertex[k-1].fx+ppolyline->m_cVertex[k].fx)/2 + nEDA_Selection_Offset;
				fy2 = (ppolyline->m_cVertex[k-1].fy+ppolyline->m_cVertex[k].fy)/2 + nEDA_Selection_Offset;
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2))
				{
					
					int ix,iy;
					Get_VPort_XY(&ix, &iy, ppolyline->m_cVertex[k].fx, ppolyline->m_cVertex[k].fy);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, ppolyline->m_cVertex[k-1].fx, ppolyline->m_cVertex[k-1].fy);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, (ppolyline->m_cVertex[k-1].fx + ppolyline->m_cVertex[k].fx)/2, (ppolyline->m_cVertex[k-1].fy + ppolyline->m_cVertex[k].fy)/2);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					ppolyline->m_nFlag = FLAG_DELETED;
					ppolyline->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schpolyline apolyline;
					apolyline.pDocument = pDoc;
					apolyline.m_nLineWidth = ppolyline->m_nLineWidth;
					apolyline.m_nLineType = ppolyline->m_nLineType;
					apolyline.m_nStartLineShape = ppolyline->m_nStartLineShape;
					apolyline.m_nEndLineShape = ppolyline->m_nEndLineShape;
					apolyline.m_nLineShapeSize = ppolyline->m_nLineShapeSize;
					apolyline.m_nColor = ppolyline->m_nColor;
					apolyline.m_nDiagram = ppolyline->m_nDiagram;
					apolyline.m_nFlag = FLAG_NORMAL;
					
					int break_flag = 0;
					if((GetKeyState(VK_CONTROL)&0x80) != 0 )
					{
						break_flag = 1;
						Fpoint fpoint;
						for(int i=0; i<k; i++)
						{
							fpoint = ppolyline->m_cVertex.GetAt(i);
							apolyline.m_cVertex.Add(fpoint);
						}
						
						fpoint.fx = (ppolyline->m_cVertex[k-1].fx + ppolyline->m_cVertex[k].fx)/2;
						fpoint.fy = (ppolyline->m_cVertex[k-1].fy + ppolyline->m_cVertex[k].fy)/2;
						apolyline.m_cVertex.Add(fpoint);
						for(int i=k; i<ppolyline->m_cVertex.GetCount(); i++)
						{
							fpoint = ppolyline->m_cVertex.GetAt(i);
							apolyline.m_cVertex.Add(fpoint);
						}
					}
					else
					{
						for(int i=0; i<ppolyline->m_cVertex.GetCount(); i++)
						{
							Fpoint fpoint;
							fpoint = ppolyline->m_cVertex.GetAt(i);
							apolyline.m_cVertex.Add(fpoint);
						}
					}

					apolyline.Draw(&dc, this);
				
					flag = 0;
					Write_Help(szHelp);
					nEDA_RedrawFlag = 0;
					do
					{
						ch1 = MouseKey();
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							CloseMouseCursor();
							apolyline.Draw(&dc, this);
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
								apolyline.Draw(&dc, this);
								OpenMouseCursor();
							}
							if(break_flag == 1)
							{
								apolyline.m_cVertex[k].fx = m_fCurrentSnapX;
								apolyline.m_cVertex[k].fy = m_fCurrentSnapY;
							}
							else
							{
								double deltax = m_fCurrentSnapX - (ppolyline->m_cVertex[k-1].fx + ppolyline->m_cVertex[k].fx)/2;
								double deltay = m_fCurrentSnapY - (ppolyline->m_cVertex[k-1].fy + ppolyline->m_cVertex[k].fy)/2;
								apolyline.m_cVertex[k-1].fx = ppolyline->m_cVertex[k-1].fx + deltax;
								apolyline.m_cVertex[k-1].fy = ppolyline->m_cVertex[k-1].fy + deltay;
								apolyline.m_cVertex[k].fx = ppolyline->m_cVertex[k].fx + deltax;
								apolyline.m_cVertex[k].fy = ppolyline->m_cVertex[k].fy + deltay;
							}

							CloseMouseCursor();
							apolyline.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							apolyline.Draw(&dc, this);
							OpenMouseCursor();

		
							if(break_flag == 1)
							{
								
								if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
								{
									pDoc->Clear_Discarded_Undo_Buffer();
								}
								clear_undo_buffer_flag = TRUE;

								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_POLYLINE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = position;
								pundo->schpolyline.Copy(ppolyline);
								pundo->schpolyline.m_nFlag = FLAG_NORMAL;
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								undo_flag = TRUE;
								
								apolyline.CombineVertex();
								ppolyline->m_cVertex.RemoveAll();
								for(int i=0; i<apolyline.m_cVertex.GetCount(); i++)
								{
									Fpoint fpoint;
									fpoint = apolyline.m_cVertex.GetAt(i);
									ppolyline->m_cVertex.Add(fpoint);
								}
							}
							else
							{
								
								if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
								{
									pDoc->Clear_Discarded_Undo_Buffer();
								}
								clear_undo_buffer_flag = TRUE;

								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_POLYLINE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = position;
								pundo->schpolyline.Copy(ppolyline);
								pundo->schpolyline.m_nFlag = FLAG_NORMAL;
			
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								undo_flag = TRUE;
								

								apolyline.CombineVertex();
								ppolyline->m_cVertex.RemoveAll();
								for(int i=0; i<apolyline.m_cVertex.GetCount(); i++)
								{
									Fpoint fpoint;
									fpoint = apolyline.m_cVertex.GetAt(i);
									ppolyline->m_cVertex.Add(fpoint);
								}
							}
							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					ppolyline->m_nFlag = FLAG_NORMAL;
					ppolyline->Draw(&dc, this);
					ppolyline->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);
					return;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
			
			

		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
				
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			
			int mode_flag = 0;
			
			double fx0, fy0, fx1, fy1, fx2, fy2;
			
			fx0 = parc->m_fCx + parc->m_fRadius*cos(parc->m_fEangle*fEDA_pi/180);
			fy0 = parc->m_fCy + parc->m_fRadius*sin(parc->m_fEangle*fEDA_pi/180);

			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 1;

			
			fx0 = parc->m_fCx + parc->m_fRadius*cos(parc->m_fSangle*fEDA_pi/180);
			fy0 = parc->m_fCy + parc->m_fRadius*sin(parc->m_fSangle*fEDA_pi/180);

			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 2;

			
			double alpha;
			if(parc->m_fEangle > parc->m_fSangle) alpha = (parc->m_fSangle + parc->m_fEangle)/2;
			else alpha = (parc->m_fSangle + parc->m_fEangle)/2 + 180;
			fx0 = parc->m_fCx + parc->m_fRadius*cos(alpha*fEDA_pi/180);
			fy0 = parc->m_fCy + parc->m_fRadius*sin(alpha*fEDA_pi/180);

			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 3;
			
			if(mode_flag != 0)
			{
					
					int ix,iy;
					
					fx0 = parc->m_fCx + parc->m_fRadius*cos(parc->m_fEangle*fEDA_pi/180);
					fy0 = parc->m_fCy + parc->m_fRadius*sin(parc->m_fEangle*fEDA_pi/180);
					Get_VPort_XY(&ix, &iy, fx0, fy0);
					HideSelectionKeyPoint(ix, iy, &dc);
					
					fx0 = parc->m_fCx + parc->m_fRadius*cos(parc->m_fSangle*fEDA_pi/180);
					fy0 = parc->m_fCy + parc->m_fRadius*sin(parc->m_fSangle*fEDA_pi/180);
					Get_VPort_XY(&ix, &iy, fx0, fy0);
					HideSelectionKeyPoint(ix, iy, &dc);
					fx0 = parc->m_fCx + parc->m_fRadius*cos(alpha*fEDA_pi/180);
					fy0 = parc->m_fCy + parc->m_fRadius*sin(alpha*fEDA_pi/180);
					Get_VPort_XY(&ix, &iy, fx0, fy0);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					parc->m_nFlag = FLAG_DELETED;
					parc->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_scharc aarc;
					aarc.pDocument = pDoc;
					aarc.m_fCx = parc->m_fCx;
					aarc.m_fCy = parc->m_fCy;
					aarc.m_fRadius = parc->m_fRadius;
					aarc.m_fSangle = parc->m_fSangle;
					aarc.m_fEangle = parc->m_fEangle;
					aarc.m_nWidth = parc->m_nWidth;
					aarc.m_nColor = parc->m_nColor;
					aarc.m_nDiagram = parc->m_nDiagram;

					aarc.Draw(&dc, this);

					flag = 0;
					Write_Help(szHelp);
					nEDA_RedrawFlag = 0;
					do
					{
						ch1 = MouseKey();
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							CloseMouseCursor();
							aarc.Draw(&dc, this);
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
								aarc.Draw(&dc, this);
								OpenMouseCursor();
							}
							
							if(mode_flag == 1)
							{
								
								double alpha;
								if(((m_fCurrentSnapX - parc->m_fCx)==0)&&((m_fCurrentSnapY - parc->m_fCy)<0)) alpha = 270;
								if(((m_fCurrentSnapX - parc->m_fCx)==0)&&((m_fCurrentSnapY - parc->m_fCy)>0)) alpha = 90;
								if(((m_fCurrentSnapY - parc->m_fCy)==0)&&((m_fCurrentSnapX - parc->m_fCx)<0)) alpha = 180;
								if(((m_fCurrentSnapY - parc->m_fCy)==0)&&((m_fCurrentSnapX - parc->m_fCx)>0)) alpha = 0;								
								else alpha = (180.0/fEDA_pi)*atan2((m_fCurrentSnapY - parc->m_fCy),(m_fCurrentSnapX - parc->m_fCx));
								alpha = pDoc->Get_Correct_Angle(alpha);

								aarc.m_fEangle = alpha;
							}
							else if(mode_flag == 2)
							{
								
								double alpha;
								if(((m_fCurrentSnapX - parc->m_fCx)==0)&&((m_fCurrentSnapY - parc->m_fCy)<0)) alpha = 270;
								if(((m_fCurrentSnapX - parc->m_fCx)==0)&&((m_fCurrentSnapY - parc->m_fCy)>0)) alpha = 90;
								if(((m_fCurrentSnapY - parc->m_fCy)==0)&&((m_fCurrentSnapX - parc->m_fCx)<0)) alpha = 180;
								if(((m_fCurrentSnapY - parc->m_fCy)==0)&&((m_fCurrentSnapX - parc->m_fCx)>0)) alpha = 0;								
								else alpha = (180.0/fEDA_pi)*atan2((m_fCurrentSnapY - parc->m_fCy),(m_fCurrentSnapX - parc->m_fCx));
								alpha = pDoc->Get_Correct_Angle(alpha);

								aarc.m_fSangle = alpha;								
							}
							else if(mode_flag == 3)
							{
								
								aarc.m_fRadius = sqrt((m_fCurrentSnapX - parc->m_fCx)*(m_fCurrentSnapX - parc->m_fCx) + (m_fCurrentSnapY - parc->m_fCy)*(m_fCurrentSnapY - parc->m_fCy));
							}

							CloseMouseCursor();
							aarc.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							aarc.Draw(&dc, this);
							OpenMouseCursor();
								
							if(aarc.m_fRadius == 0) break;

							
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;

							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = SCH_ELEMENT_ARC;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = position;
							pundo->scharc.Copy(parc);
							pundo->scharc.m_nFlag = FLAG_NORMAL;
							
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							parc->m_fSangle = aarc.m_fSangle;
							parc->m_fEangle = aarc.m_fEangle;
							parc->m_fRadius = aarc.m_fRadius;

							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					parc->m_nFlag = FLAG_NORMAL;
					parc->Draw(&dc, this);
					parc->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);
					return;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;

			int mode_flag = 0;
			
			double fx0, fy0, fx1, fy1, fx2, fy2;

			
			double alpha = pellipse->m_fEangle*fEDA_pi/180;
			double ftemp = sqrt(pellipse->m_fRadius_x*pellipse->m_fRadius_x*sin(alpha)*sin(alpha) + pellipse->m_fRadius_y*pellipse->m_fRadius_y*cos(alpha)*cos(alpha));

			fx0 = pellipse->m_fCx + pellipse->m_fRadius_x*pellipse->m_fRadius_y*cos(alpha)/ftemp;
			fy0 = pellipse->m_fCy + pellipse->m_fRadius_x*pellipse->m_fRadius_y*sin(alpha)/ftemp;
			
			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 1;

			
			alpha = pellipse->m_fSangle*fEDA_pi/180;
			ftemp = sqrt(pellipse->m_fRadius_x*pellipse->m_fRadius_x*sin(alpha)*sin(alpha) + pellipse->m_fRadius_y*pellipse->m_fRadius_y*cos(alpha)*cos(alpha));

			fx0 = pellipse->m_fCx + pellipse->m_fRadius_x*pellipse->m_fRadius_y*cos(alpha)/ftemp;
			fy0 = pellipse->m_fCy + pellipse->m_fRadius_x*pellipse->m_fRadius_y*sin(alpha)/ftemp;
			
			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 2;

			
			fx0 = pellipse->m_fCx + pellipse->m_fRadius_x;
			fy0 = pellipse->m_fCy;
				
			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 3;		
			
			fx0 = pellipse->m_fCx;
			fy0 = pellipse->m_fCy + pellipse->m_fRadius_y;
				
			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 4;
			if(mode_flag != 0)
			{
					
					int ix,iy;
					
					alpha = pellipse->m_fEangle*fEDA_pi/180;
					ftemp = sqrt(pellipse->m_fRadius_x*pellipse->m_fRadius_x*sin(alpha)*sin(alpha) + pellipse->m_fRadius_y*pellipse->m_fRadius_y*cos(alpha)*cos(alpha));
					fx0 = pellipse->m_fCx + pellipse->m_fRadius_x*pellipse->m_fRadius_y*cos(alpha)/ftemp;
					fy0 = pellipse->m_fCy + pellipse->m_fRadius_x*pellipse->m_fRadius_y*sin(alpha)/ftemp;
					Get_VPort_XY(&ix, &iy, fx0, fy0);
					HideSelectionKeyPoint(ix, iy, &dc);
					
					alpha = pellipse->m_fSangle*fEDA_pi/180;
					ftemp = sqrt(pellipse->m_fRadius_x*pellipse->m_fRadius_x*sin(alpha)*sin(alpha) + pellipse->m_fRadius_y*pellipse->m_fRadius_y*cos(alpha)*cos(alpha));
					fx0 = pellipse->m_fCx + pellipse->m_fRadius_x*pellipse->m_fRadius_y*cos(alpha)/ftemp;
					fy0 = pellipse->m_fCy + pellipse->m_fRadius_x*pellipse->m_fRadius_y*sin(alpha)/ftemp;
					Get_VPort_XY(&ix, &iy, fx0, fy0);
					HideSelectionKeyPoint(ix, iy, &dc);
					
					fx0 = pellipse->m_fCx + pellipse->m_fRadius_x;
					fy0 = pellipse->m_fCy;
					Get_VPort_XY(&ix, &iy, fx0, fy0);
					HideSelectionKeyPoint(ix, iy, &dc);
					fx0 = pellipse->m_fCx;
					fy0 = pellipse->m_fCy + pellipse->m_fRadius_y;
					Get_VPort_XY(&ix, &iy, fx0, fy0);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					pellipse->m_nFlag = FLAG_DELETED;
					pellipse->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schellipse aellipse;
					aellipse.pDocument = pDoc;
					aellipse.m_fCx = pellipse->m_fCx;
					aellipse.m_fCy = pellipse->m_fCy;
					aellipse.m_fRadius_x = pellipse->m_fRadius_x;
					aellipse.m_fRadius_y = pellipse->m_fRadius_y;
					aellipse.m_fSangle = pellipse->m_fSangle;
					aellipse.m_fEangle = pellipse->m_fEangle;
					aellipse.m_nWidth = pellipse->m_nWidth;
					aellipse.m_nColor = pellipse->m_nColor;
					aellipse.m_nDiagram = pellipse->m_nDiagram;

					aellipse.Draw(&dc, this);

					flag = 0;
					Write_Help(szHelp);
					nEDA_RedrawFlag = 0;
					do
					{
						ch1 = MouseKey();
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							CloseMouseCursor();
							aellipse.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if(ch1 == MOUSEMOVE)
						{
							
							if(m_bActive == FALSE) continue;

							
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
								aellipse.Draw(&dc, this);
								OpenMouseCursor();
							}
							
							if(mode_flag == 1)
							{
								
								double alpha;
								if(((m_fCurrentSnapX - pellipse->m_fCx)==0)&&((m_fCurrentSnapY - pellipse->m_fCy)<0)) alpha = 270;
								if(((m_fCurrentSnapX - pellipse->m_fCx)==0)&&((m_fCurrentSnapY - pellipse->m_fCy)>0)) alpha = 90;
								if(((m_fCurrentSnapY - pellipse->m_fCy)==0)&&((m_fCurrentSnapX - pellipse->m_fCx)<0)) alpha = 180;
								if(((m_fCurrentSnapY - pellipse->m_fCy)==0)&&((m_fCurrentSnapX - pellipse->m_fCx)>0)) alpha = 0;								
								else alpha = (180.0/fEDA_pi)*atan2((m_fCurrentSnapY - pellipse->m_fCy),(m_fCurrentSnapX - pellipse->m_fCx));
								alpha = pDoc->Get_Correct_Angle(alpha);

								aellipse.m_fEangle = alpha;
							}
							else if(mode_flag == 2)
							{
								
								double alpha;
								if(((m_fCurrentSnapX - pellipse->m_fCx)==0)&&((m_fCurrentSnapY - pellipse->m_fCy)<0)) alpha = 270;
								if(((m_fCurrentSnapX - pellipse->m_fCx)==0)&&((m_fCurrentSnapY - pellipse->m_fCy)>0)) alpha = 90;
								if(((m_fCurrentSnapY - pellipse->m_fCy)==0)&&((m_fCurrentSnapX - pellipse->m_fCx)<0)) alpha = 180;
								if(((m_fCurrentSnapY - pellipse->m_fCy)==0)&&((m_fCurrentSnapX - pellipse->m_fCx)>0)) alpha = 0;								
								else alpha = (180.0/fEDA_pi)*atan2((m_fCurrentSnapY - pellipse->m_fCy),(m_fCurrentSnapX - pellipse->m_fCx));
								alpha = pDoc->Get_Correct_Angle(alpha);

								aellipse.m_fSangle = alpha;								
							}
							else if(mode_flag == 3)
							{
								
								aellipse.m_fRadius_x = (int)abs(m_fCurrentSnapX - pellipse->m_fCx);
							}
							else if(mode_flag == 4)
							{
								
								aellipse.m_fRadius_y = (int)abs(m_fCurrentSnapY - pellipse->m_fCy);
							}

							CloseMouseCursor();
							aellipse.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							aellipse.Draw(&dc, this);
							OpenMouseCursor();
								
							if((aellipse.m_fRadius_x == 0)||(aellipse.m_fRadius_y == 0)) break;

							
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;

							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = position;
							pundo->schellipse.Copy(pellipse);
							pundo->schellipse.m_nFlag = FLAG_NORMAL;
							
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							pellipse->m_fSangle = aellipse.m_fSangle;
							pellipse->m_fEangle = aellipse.m_fEangle;
							pellipse->m_fRadius_x = aellipse.m_fRadius_x;
							pellipse->m_fRadius_y = aellipse.m_fRadius_y;

							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					pellipse->m_nFlag = FLAG_NORMAL;
					pellipse->Draw(&dc, this);
					pellipse->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);
					return;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			
			int mode_flag = 0;
			
			double fx0, fy0, fx1, fy1, fx2, fy2;

			
			fx0 = pellipsefill->m_fCx + pellipsefill->m_fRadius_x;
			fy0 = pellipsefill->m_fCy;
				
			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 1;
			
			fx0 = pellipsefill->m_fCx;
			fy0 = pellipsefill->m_fCy + pellipsefill->m_fRadius_y;
				
			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 2;
			if(mode_flag != 0)
			{
					
					int ix,iy;
					
					fx0 = pellipsefill->m_fCx + pellipsefill->m_fRadius_x;
					fy0 = pellipsefill->m_fCy;
					Get_VPort_XY(&ix, &iy, fx0, fy0);
					HideSelectionKeyPoint(ix, iy, &dc);
					fx0 = pellipsefill->m_fCx;
					fy0 = pellipsefill->m_fCy + pellipsefill->m_fRadius_y;
					Get_VPort_XY(&ix, &iy, fx0, fy0);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					pellipsefill->m_nFlag = FLAG_DELETED;
					pellipsefill->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schellipsefill aellipsefill;
					aellipsefill.pDocument = pDoc;
					aellipsefill.m_fCx = pellipsefill->m_fCx;
					aellipsefill.m_fCy = pellipsefill->m_fCy;
					aellipsefill.m_fRadius_x = pellipsefill->m_fRadius_x;
					aellipsefill.m_fRadius_y = pellipsefill->m_fRadius_y;
					aellipsefill.m_nBorderWidth = pellipsefill->m_nBorderWidth;
					aellipsefill.m_nBorderColor = pellipsefill->m_nBorderColor;
					aellipsefill.m_nFillColor = pellipsefill->m_nFillColor;
					aellipsefill.m_bFillSolid = pellipsefill->m_bFillSolid;
					aellipsefill.m_bTransparent = FALSE;
					aellipsefill.m_nDiagram = pellipsefill->m_nDiagram;

					aellipsefill.Draw(&dc, this);

					flag = 0;
					Write_Help(szHelp);
					nEDA_RedrawFlag = 0;
					do
					{
						ch1 = MouseKey();
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							CloseMouseCursor();
							aellipsefill.Draw(&dc, this);
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
								aellipsefill.Draw(&dc, this);
								OpenMouseCursor();
							}
							
							if(mode_flag == 1)
							{
								
								aellipsefill.m_fRadius_x = (int)abs(m_fCurrentSnapX - pellipsefill->m_fCx);
							}
							else if(mode_flag == 2)
							{
								
								aellipsefill.m_fRadius_y = (int)abs(m_fCurrentSnapY - pellipsefill->m_fCy);
							}

							CloseMouseCursor();
							aellipsefill.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							aellipsefill.Draw(&dc, this);
							OpenMouseCursor();
								
							if((aellipsefill.m_fRadius_x == 0)||(aellipsefill.m_fRadius_y == 0)) break;

							
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;

							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_MOVE;
							pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = position;
							pundo->m_fX1 = pellipsefill->m_fCx;
							pundo->m_fY1 = pellipsefill->m_fCy;
							pundo->m_fRx = pellipsefill->m_fRadius_x;
							pundo->m_fRy = pellipsefill->m_fRadius_y;
							
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							
							pellipsefill->m_fRadius_x = aellipsefill.m_fRadius_x;
							pellipsefill->m_fRadius_y = aellipsefill.m_fRadius_y;

							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					pellipsefill->m_nFlag = FLAG_NORMAL;
					pellipsefill->Draw(&dc, this);
					pellipsefill->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);
					return;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
						
			int mode_flag = 0;
			
			double fx0, fy0, fx1, fy1, fx2, fy2;
			
			fx0 = ppie->m_fCx + ppie->m_fRadius*cos(ppie->m_fEangle*fEDA_pi/180);
			fy0 = ppie->m_fCy + ppie->m_fRadius*sin(ppie->m_fEangle*fEDA_pi/180);

			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 1;

			
			fx0 = ppie->m_fCx + ppie->m_fRadius*cos(ppie->m_fSangle*fEDA_pi/180);
			fy0 = ppie->m_fCy + ppie->m_fRadius*sin(ppie->m_fSangle*fEDA_pi/180);

			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 2;

			
			double alpha;
			if(ppie->m_fEangle > ppie->m_fSangle) alpha = (ppie->m_fSangle + ppie->m_fEangle)/2;
			else alpha = (ppie->m_fSangle + ppie->m_fEangle)/2 + 180;
			fx0 = ppie->m_fCx + ppie->m_fRadius*cos(alpha*fEDA_pi/180);
			fy0 = ppie->m_fCy + ppie->m_fRadius*sin(alpha*fEDA_pi/180);

			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 3;
			
			if(mode_flag != 0)
			{
					
					int ix,iy;
					
					fx0 = ppie->m_fCx + ppie->m_fRadius*cos(ppie->m_fEangle*fEDA_pi/180);
					fy0 = ppie->m_fCy + ppie->m_fRadius*sin(ppie->m_fEangle*fEDA_pi/180);
					Get_VPort_XY(&ix, &iy, fx0, fy0);
					HideSelectionKeyPoint(ix, iy, &dc);
					
					fx0 = ppie->m_fCx + ppie->m_fRadius*cos(ppie->m_fSangle*fEDA_pi/180);
					fy0 = ppie->m_fCy + ppie->m_fRadius*sin(ppie->m_fSangle*fEDA_pi/180);
					Get_VPort_XY(&ix, &iy, fx0, fy0);
					HideSelectionKeyPoint(ix, iy, &dc);
					fx0 = ppie->m_fCx + ppie->m_fRadius*cos(alpha*fEDA_pi/180);
					fy0 = ppie->m_fCy + ppie->m_fRadius*sin(alpha*fEDA_pi/180);
					Get_VPort_XY(&ix, &iy, fx0, fy0);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					ppie->m_nFlag = FLAG_DELETED;
					ppie->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schpie apie;
					apie.pDocument = pDoc;
					apie.m_fCx = ppie->m_fCx;
					apie.m_fCy = ppie->m_fCy;
					apie.m_fRadius = ppie->m_fRadius;
					apie.m_fSangle = ppie->m_fSangle;
					apie.m_fEangle = ppie->m_fEangle;
					apie.m_nBorderWidth = ppie->m_nBorderWidth;
					apie.m_nBorderColor = ppie->m_nBorderColor;
					apie.m_nFillColor = ppie->m_nFillColor;
					apie.m_bFillSolid = ppie->m_bFillSolid;
					apie.m_nDiagram = ppie->m_nDiagram;

					apie.Draw(&dc, this);

					flag = 0;
					Write_Help(szHelp);
					nEDA_RedrawFlag = 0;
					do
					{
						ch1 = MouseKey();
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							CloseMouseCursor();
							apie.Draw(&dc, this);
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
								apie.Draw(&dc, this);
								OpenMouseCursor();
							}
							
							if(mode_flag == 1)
							{
								
								double alpha;
								if(((m_fCurrentSnapX - ppie->m_fCx)==0)&&((m_fCurrentSnapY - ppie->m_fCy)<0)) alpha = 270;
								if(((m_fCurrentSnapX - ppie->m_fCx)==0)&&((m_fCurrentSnapY - ppie->m_fCy)>0)) alpha = 90;
								if(((m_fCurrentSnapY - ppie->m_fCy)==0)&&((m_fCurrentSnapX - ppie->m_fCx)<0)) alpha = 180;
								if(((m_fCurrentSnapY - ppie->m_fCy)==0)&&((m_fCurrentSnapX - ppie->m_fCx)>0)) alpha = 0;								
								else alpha = (180.0/fEDA_pi)*atan2((m_fCurrentSnapY - ppie->m_fCy),(m_fCurrentSnapX - ppie->m_fCx));
								alpha = pDoc->Get_Correct_Angle(alpha);

								apie.m_fEangle = alpha;
							}
							else if(mode_flag == 2)
							{
								
								double alpha;
								if(((m_fCurrentSnapX - ppie->m_fCx)==0)&&((m_fCurrentSnapY - ppie->m_fCy)<0)) alpha = 270;
								if(((m_fCurrentSnapX - ppie->m_fCx)==0)&&((m_fCurrentSnapY - ppie->m_fCy)>0)) alpha = 90;
								if(((m_fCurrentSnapY - ppie->m_fCy)==0)&&((m_fCurrentSnapX - ppie->m_fCx)<0)) alpha = 180;
								if(((m_fCurrentSnapY - ppie->m_fCy)==0)&&((m_fCurrentSnapX - ppie->m_fCx)>0)) alpha = 0;								
								else alpha = (180.0/fEDA_pi)*atan2((m_fCurrentSnapY - ppie->m_fCy),(m_fCurrentSnapX - ppie->m_fCx));
								alpha = pDoc->Get_Correct_Angle(alpha);

								apie.m_fSangle = alpha;								
							}
							else if(mode_flag == 3)
							{
								
								apie.m_fRadius = sqrt((m_fCurrentSnapX - ppie->m_fCx)*(m_fCurrentSnapX - ppie->m_fCx) + (m_fCurrentSnapY - ppie->m_fCy)*(m_fCurrentSnapY - ppie->m_fCy));
							}

							CloseMouseCursor();
							apie.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							apie.Draw(&dc, this);
							OpenMouseCursor();
								
							if(apie.m_fRadius == 0) break;

							
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;

							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_MOVE;
							pundo->m_nOb = SCH_ELEMENT_PIE;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = position;
							pundo->m_fX1 = ppie->m_fCx;
							pundo->m_fY1 = ppie->m_fCy;
							pundo->m_fX2 = ppie->m_fSangle;
							pundo->m_fY2 = ppie->m_fEangle;
							pundo->m_fRx = ppie->m_fRadius;
							
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							ppie->m_fSangle = apie.m_fSangle;
							ppie->m_fEangle = apie.m_fEangle;
							ppie->m_fRadius = apie.m_fRadius;

							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					ppie->m_nFlag = FLAG_NORMAL;
					ppie->Draw(&dc, this);
					ppie->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);
					return;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
			{
				double fx1, fy1, fx2, fy2;
				fx1 = ppolygon->m_cVertex[k].fx - nEDA_Selection_Offset;
				fy1 = ppolygon->m_cVertex[k].fy - nEDA_Selection_Offset;
				fx2 = ppolygon->m_cVertex[k].fx + nEDA_Selection_Offset;
				fy2 = ppolygon->m_cVertex[k].fy + nEDA_Selection_Offset;
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2))
				{
					
					int ix,iy;
					Get_VPort_XY(&ix, &iy, ppolygon->m_cVertex[k].fx, ppolygon->m_cVertex[k].fy);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					ppolygon->m_nFlag = FLAG_DELETED;
					ppolygon->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schpolygon apolygon;
					apolygon.pDocument = pDoc;
					apolygon.m_nBorderWidth = ppolygon->m_nBorderWidth;
					apolygon.m_nBorderColor = ppolygon->m_nBorderColor;
					apolygon.m_nFillColor = ppolygon->m_nFillColor;
					apolygon.m_bFillSolid = ppolygon->m_bFillSolid;
					apolygon.m_bTransparent = FALSE;
					apolygon.m_nDiagram = ppolygon->m_nDiagram;
					apolygon.m_nFlag = FLAG_NORMAL;
					
					for(int i=0; i<ppolygon->m_cVertex.GetCount(); i++)
					{
						Fpoint fpoint;
						fpoint = ppolygon->m_cVertex.GetAt(i);
						apolygon.m_cVertex.Add(fpoint);
					}

					apolygon.Draw(&dc, this);

					flag = 0;
					Write_Help(szHelp);
					nEDA_RedrawFlag = 0;
					do
					{
						ch1 = MouseKey();
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							CloseMouseCursor();
							apolygon.Draw(&dc, this);
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
								apolygon.Draw(&dc, this);
								OpenMouseCursor();
							}

							apolygon.m_cVertex[k].fx = m_fCurrentSnapX;
							apolygon.m_cVertex[k].fy = m_fCurrentSnapY;

							CloseMouseCursor();
							apolygon.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							apolygon.Draw(&dc, this);
							OpenMouseCursor();

							apolygon.m_cVertex[k].fx = m_fCurrentSnapX;
							apolygon.m_cVertex[k].fy = m_fCurrentSnapY;
		
							
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;

							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_MOVE;
							pundo->m_nOb = SCH_ELEMENT_POLYGON;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = position;
							pundo->m_cVertex.RemoveAll();
							for(int i=0; i<ppolygon->m_cVertex.GetCount(); i++)
							{
								Fpoint fpoint;
								fpoint = ppolygon->m_cVertex.GetAt(i);
								pundo->m_cVertex.Add(fpoint);
							}			
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							ppolygon->m_cVertex[k].fx = apolygon.m_cVertex[k].fx;
							ppolygon->m_cVertex[k].fy = apolygon.m_cVertex[k].fy;
							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					ppolygon->m_nFlag = FLAG_NORMAL;
					ppolygon->Draw(&dc, this);
					ppolygon->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);
					return;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;

			int mode_flag = 0;
			
			double fx1, fy1, fx2, fy2;
			
			fx1 = ptextframe->m_fX1 - nEDA_Selection_Offset;
			fy1 = ptextframe->m_fY1 - nEDA_Selection_Offset;
			fx2 = ptextframe->m_fX1 + nEDA_Selection_Offset;
			fy2 = ptextframe->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 1;

			
			fx1 = ptextframe->m_fX1 - nEDA_Selection_Offset;
			fy1 = ptextframe->m_fY2 - nEDA_Selection_Offset;
			fx2 = ptextframe->m_fX1 + nEDA_Selection_Offset;
			fy2 = ptextframe->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 2;

			
			fx1 = ptextframe->m_fX2 - nEDA_Selection_Offset;
			fy1 = ptextframe->m_fY1 - nEDA_Selection_Offset;
			fx2 = ptextframe->m_fX2 + nEDA_Selection_Offset;
			fy2 = ptextframe->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 3;

			
			fx1 = ptextframe->m_fX2 - nEDA_Selection_Offset;
			fy1 = ptextframe->m_fY2 - nEDA_Selection_Offset;
			fx2 = ptextframe->m_fX2 + nEDA_Selection_Offset;
			fy2 = ptextframe->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 4;

			
			fx1 = (ptextframe->m_fX1+ptextframe->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = ptextframe->m_fY1 - nEDA_Selection_Offset;
			fx2 = (ptextframe->m_fX1+ptextframe->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = ptextframe->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 5;

			
			fx1 = (ptextframe->m_fX1+ptextframe->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = ptextframe->m_fY2 - nEDA_Selection_Offset;
			fx2 = (ptextframe->m_fX1+ptextframe->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = ptextframe->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 6;

			
			fx1 = ptextframe->m_fX1 - nEDA_Selection_Offset;
			fy1 = (ptextframe->m_fY1+ptextframe->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = ptextframe->m_fX1 + nEDA_Selection_Offset;
			fy2 = (ptextframe->m_fY1+ptextframe->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 7;

			
			fx1 = ptextframe->m_fX2 - nEDA_Selection_Offset;
			fy1 = (ptextframe->m_fY1+ptextframe->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = ptextframe->m_fX2 + nEDA_Selection_Offset;
			fy2 = (ptextframe->m_fY1+ptextframe->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 8;

			if(mode_flag != 0)
			{
					
					int ix,iy;
					Get_VPort_XY(&ix, &iy, ptextframe->m_fX1, ptextframe->m_fY1);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, ptextframe->m_fX2, ptextframe->m_fY1);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, ptextframe->m_fX2, ptextframe->m_fY2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, ptextframe->m_fX1, ptextframe->m_fY2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, (ptextframe->m_fX1 + ptextframe->m_fX2)/2, ptextframe->m_fY1);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, (ptextframe->m_fX1 + ptextframe->m_fX2)/2, ptextframe->m_fY2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, ptextframe->m_fX1, (ptextframe->m_fY1 + ptextframe->m_fY2)/2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, ptextframe->m_fX2, (ptextframe->m_fY1 + ptextframe->m_fY2)/2);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					ptextframe->m_nFlag = FLAG_DELETED;
					ptextframe->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schrect arect;
					arect.pDocument = pDoc;
					arect.m_fX1 = ptextframe->m_fX1;
					arect.m_fY1 = ptextframe->m_fY1;
					arect.m_fX2 = ptextframe->m_fX2;
					arect.m_fY2 = ptextframe->m_fY2;
					arect.m_nBorderWidth = ptextframe->m_nBorderWidth;
					arect.m_nFillColor = ptextframe->m_nFillColor;
					arect.m_nBorderColor = ptextframe->m_nBorderColor;
					arect.m_bTransparent = FALSE;
					arect.m_bFillSolid = ptextframe->m_bFillSolid;
					arect.m_nDiagram = ptextframe->m_nDiagram;

					arect.Draw(&dc, this);

					flag = 0;
					Write_Help(szHelp);
					nEDA_RedrawFlag = 0;
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
								arect.Draw(&dc, this);
								OpenMouseCursor();
							}
							
							if(mode_flag == 1)
							{
								arect.m_fX1 = m_fCurrentSnapX;
								arect.m_fY1 = m_fCurrentSnapY;
							}
							else if(mode_flag == 2)
							{
								arect.m_fX1 = m_fCurrentSnapX;
								arect.m_fY2 = m_fCurrentSnapY;
							}
							else if(mode_flag == 3)
							{
								arect.m_fX2 = m_fCurrentSnapX;
								arect.m_fY1 = m_fCurrentSnapY;
							}
							else if(mode_flag == 4)
							{
								arect.m_fX2 = m_fCurrentSnapX;
								arect.m_fY2 = m_fCurrentSnapY;
							}
							else if(mode_flag == 5)
							{
								arect.m_fY1 = m_fCurrentSnapY;
							}
							else if(mode_flag == 6)
							{
								arect.m_fY2 = m_fCurrentSnapY;
							}
							else if(mode_flag == 7)
							{
								arect.m_fX1 = m_fCurrentSnapX;
							}
							else if(mode_flag == 8)
							{
								arect.m_fX2 = m_fCurrentSnapX;
							}

							CloseMouseCursor();
							arect.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							arect.Draw(&dc, this);
							OpenMouseCursor();

							double fminx, fminy, fmaxx, fmaxy;
							fminx = MIN(arect.m_fX1, arect.m_fX2);
							fmaxx = MAX(arect.m_fX1, arect.m_fX2);
							fminy = MIN(arect.m_fY1, arect.m_fY2);
							fmaxy = MAX(arect.m_fY1, arect.m_fY2);
							arect.m_fX1 = fminx;
							arect.m_fY1 = fminy;
							arect.m_fX2 = fmaxx;
							arect.m_fY2 = fmaxy;

							
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;

							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_MOVE;
							pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = position;
							pundo->m_fX1 = ptextframe->m_fX1;
							pundo->m_fY1 = ptextframe->m_fY1;
							pundo->m_fX2 = ptextframe->m_fX2;
							pundo->m_fY2 = ptextframe->m_fY2;
						
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							ptextframe->m_fX1 = arect.m_fX1;
							ptextframe->m_fY1 = arect.m_fY1;
							ptextframe->m_fX2 = arect.m_fX2;
							ptextframe->m_fY2 = arect.m_fY2;
							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					ptextframe->m_nFlag = FLAG_NORMAL;
					ptextframe->Draw(&dc, this);
					ptextframe->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);
					return;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			
			int mode_flag = 0;
			
			double fx1, fy1, fx2, fy2;
			
			fx1 = prect->m_fX1 - nEDA_Selection_Offset;
			fy1 = prect->m_fY1 - nEDA_Selection_Offset;
			fx2 = prect->m_fX1 + nEDA_Selection_Offset;
			fy2 = prect->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 1;

			
			fx1 = prect->m_fX1 - nEDA_Selection_Offset;
			fy1 = prect->m_fY2 - nEDA_Selection_Offset;
			fx2 = prect->m_fX1 + nEDA_Selection_Offset;
			fy2 = prect->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 2;

			
			fx1 = prect->m_fX2 - nEDA_Selection_Offset;
			fy1 = prect->m_fY1 - nEDA_Selection_Offset;
			fx2 = prect->m_fX2 + nEDA_Selection_Offset;
			fy2 = prect->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 3;

			
			fx1 = prect->m_fX2 - nEDA_Selection_Offset;
			fy1 = prect->m_fY2 - nEDA_Selection_Offset;
			fx2 = prect->m_fX2 + nEDA_Selection_Offset;
			fy2 = prect->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 4;

			
			fx1 = (prect->m_fX1+prect->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = prect->m_fY1 - nEDA_Selection_Offset;
			fx2 = (prect->m_fX1+prect->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = prect->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 5;

			//pView->DrawSelectionKeyPoint((x1+x2)/2, y2, pDC);
			fx1 = (prect->m_fX1+prect->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = prect->m_fY2 - nEDA_Selection_Offset;
			fx2 = (prect->m_fX1+prect->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = prect->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 6;

			//pView->DrawSelectionKeyPoint(x1, (y1+y2)/2, pDC);
			fx1 = prect->m_fX1 - nEDA_Selection_Offset;
			fy1 = (prect->m_fY1+prect->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = prect->m_fX1 + nEDA_Selection_Offset;
			fy2 = (prect->m_fY1+prect->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 7;

			//pView->DrawSelectionKeyPoint(x2, (y1+y2)/2, pDC);
			fx1 = prect->m_fX2 - nEDA_Selection_Offset;
			fy1 = (prect->m_fY1+prect->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = prect->m_fX2 + nEDA_Selection_Offset;
			fy2 = (prect->m_fY1+prect->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 8;

			if(mode_flag != 0)
			{
					
					int ix,iy;
					Get_VPort_XY(&ix, &iy, prect->m_fX1, prect->m_fY1);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, prect->m_fX2, prect->m_fY1);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, prect->m_fX2, prect->m_fY2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, prect->m_fX1, prect->m_fY2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, (prect->m_fX1 + prect->m_fX2)/2, prect->m_fY1);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, (prect->m_fX1 + prect->m_fX2)/2, prect->m_fY2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, prect->m_fX1, (prect->m_fY1 + prect->m_fY2)/2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, prect->m_fX2, (prect->m_fY1 + prect->m_fY2)/2);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					prect->m_nFlag = FLAG_DELETED;
					prect->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schrect arect;
					arect.pDocument = pDoc;
					arect.m_fX1 = prect->m_fX1;
					arect.m_fY1 = prect->m_fY1;
					arect.m_fX2 = prect->m_fX2;
					arect.m_fY2 = prect->m_fY2;
					arect.m_nBorderWidth = prect->m_nBorderWidth;
					arect.m_nFillColor = prect->m_nFillColor;
					arect.m_nBorderColor = prect->m_nBorderColor;
					arect.m_bTransparent = FALSE;
					arect.m_bFillSolid = prect->m_bFillSolid;
					arect.m_nDiagram = prect->m_nDiagram;

					arect.Draw(&dc, this);

					flag = 0;
					Write_Help(szHelp);
					nEDA_RedrawFlag = 0;
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
								arect.Draw(&dc, this);
								OpenMouseCursor();
							}
							
							if(mode_flag == 1)
							{
								arect.m_fX1 = m_fCurrentSnapX;
								arect.m_fY1 = m_fCurrentSnapY;
							}
							else if(mode_flag == 2)
							{
								arect.m_fX1 = m_fCurrentSnapX;
								arect.m_fY2 = m_fCurrentSnapY;
							}
							else if(mode_flag == 3)
							{
								arect.m_fX2 = m_fCurrentSnapX;
								arect.m_fY1 = m_fCurrentSnapY;
							}
							else if(mode_flag == 4)
							{
								arect.m_fX2 = m_fCurrentSnapX;
								arect.m_fY2 = m_fCurrentSnapY;
							}
							else if(mode_flag == 5)
							{
								arect.m_fY1 = m_fCurrentSnapY;
							}
							else if(mode_flag == 6)
							{
								arect.m_fY2 = m_fCurrentSnapY;
							}
							else if(mode_flag == 7)
							{
								arect.m_fX1 = m_fCurrentSnapX;
							}
							else if(mode_flag == 8)
							{
								arect.m_fX2 = m_fCurrentSnapX;
							}

							CloseMouseCursor();
							arect.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							arect.Draw(&dc, this);
							OpenMouseCursor();

							double fminx, fminy, fmaxx, fmaxy;
							fminx = MIN(arect.m_fX1, arect.m_fX2);
							fmaxx = MAX(arect.m_fX1, arect.m_fX2);
							fminy = MIN(arect.m_fY1, arect.m_fY2);
							fmaxy = MAX(arect.m_fY1, arect.m_fY2);
							arect.m_fX1 = fminx;
							arect.m_fY1 = fminy;
							arect.m_fX2 = fmaxx;
							arect.m_fY2 = fmaxy;

							
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;

							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_MOVE;
							pundo->m_nOb = SCH_ELEMENT_RECT;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = position;
							pundo->m_fX1 = prect->m_fX1;
							pundo->m_fY1 = prect->m_fY1;
							pundo->m_fX2 = prect->m_fX2;
							pundo->m_fY2 = prect->m_fY2;
						
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							prect->m_fX1 = arect.m_fX1;
							prect->m_fY1 = arect.m_fY1;
							prect->m_fX2 = arect.m_fX2;
							prect->m_fY2 = arect.m_fY2;
							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					prect->m_nFlag = FLAG_NORMAL;
					prect->Draw(&dc, this);
					prect->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);
					return;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			
			int mode_flag = 0;
			
			double fx1, fy1, fx2, fy2;
			
			fx1 = proundrect->m_fX1 - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY1 - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX1 + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 1;

			//pView->DrawSelectionKeyPoint(x1, y2, pDC);
			fx1 = proundrect->m_fX1 - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY2 - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX1 + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 2;

			//pView->DrawSelectionKeyPoint(x2, y1, pDC);
			fx1 = proundrect->m_fX2 - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY1 - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX2 + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 3;

			//pView->DrawSelectionKeyPoint(x2, y2, pDC);
			fx1 = proundrect->m_fX2 - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY2 - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX2 + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 4;

			//pView->DrawSelectionKeyPoint((x1+x2)/2, y1, pDC);
			fx1 = (proundrect->m_fX1+proundrect->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY1 - nEDA_Selection_Offset;
			fx2 = (proundrect->m_fX1+proundrect->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 5;

			//pView->DrawSelectionKeyPoint((x1+x2)/2, y2, pDC);
			fx1 = (proundrect->m_fX1+proundrect->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY2 - nEDA_Selection_Offset;
			fx2 = (proundrect->m_fX1+proundrect->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 6;

			//pView->DrawSelectionKeyPoint(x1, (y1+y2)/2, pDC);
			fx1 = proundrect->m_fX1 - nEDA_Selection_Offset;
			fy1 = (proundrect->m_fY1+proundrect->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX1 + nEDA_Selection_Offset;
			fy2 = (proundrect->m_fY1+proundrect->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 7;

			//pView->DrawSelectionKeyPoint(x2, (y1+y2)/2, pDC);
			fx1 = proundrect->m_fX2 - nEDA_Selection_Offset;
			fy1 = (proundrect->m_fY1+proundrect->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX2 + nEDA_Selection_Offset;
			fy2 = (proundrect->m_fY1+proundrect->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 8;
			
			//pView->DrawSelectionKeyPoint(x1+rx, y1-ry, pDC);
			fx1 = proundrect->m_fX1 + proundrect->m_fRadius_x - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY1 + proundrect->m_fRadius_y - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX1 + proundrect->m_fRadius_x + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY1 + proundrect->m_fRadius_y + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2))  mode_flag = 9;
			
			//pView->DrawSelectionKeyPoint(x1+rx, y2+ry, pDC);
			fx1 = proundrect->m_fX1 + proundrect->m_fRadius_x - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY2 - proundrect->m_fRadius_y - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX1 + proundrect->m_fRadius_x + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY2 - proundrect->m_fRadius_y + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 10;
			
			//pView->DrawSelectionKeyPoint(x2-rx, y1-ry, pDC);
			fx1 = proundrect->m_fX2 - proundrect->m_fRadius_x - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY1 + proundrect->m_fRadius_y - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX2 - proundrect->m_fRadius_x + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY1 + proundrect->m_fRadius_y + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2))  mode_flag = 11;
		
			//pView->DrawSelectionKeyPoint(x2-rx, y2+ry, pDC);
			fx1 = proundrect->m_fX2 - proundrect->m_fRadius_x - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY2 - proundrect->m_fRadius_y - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX2 - proundrect->m_fRadius_x + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY2 - proundrect->m_fRadius_y + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 12;

			if(mode_flag != 0)
			{
					
					int ix,iy;
					Get_VPort_XY(&ix, &iy, proundrect->m_fX1, proundrect->m_fY1);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, proundrect->m_fX2, proundrect->m_fY1);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, proundrect->m_fX2, proundrect->m_fY2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, proundrect->m_fX1, proundrect->m_fY2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, (proundrect->m_fX1 + proundrect->m_fX2)/2, proundrect->m_fY1);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, (proundrect->m_fX1 + proundrect->m_fX2)/2, proundrect->m_fY2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, proundrect->m_fX1, (proundrect->m_fY1 + proundrect->m_fY2)/2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, proundrect->m_fX2, (proundrect->m_fY1 + proundrect->m_fY2)/2);
					HideSelectionKeyPoint(ix, iy, &dc);
					
					Get_VPort_XY(&ix, &iy, proundrect->m_fX1 + proundrect->m_fRadius_x, proundrect->m_fY1 + proundrect->m_fRadius_y);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, proundrect->m_fX1 + proundrect->m_fRadius_x, proundrect->m_fY2 - proundrect->m_fRadius_y);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, proundrect->m_fX2 - proundrect->m_fRadius_x, proundrect->m_fY1 + proundrect->m_fRadius_y);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy,proundrect->m_fX2 - proundrect->m_fRadius_x , proundrect->m_fY2 - proundrect->m_fRadius_y);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					proundrect->m_nFlag = FLAG_DELETED;
					proundrect->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schroundrect aroundrect;
					aroundrect.pDocument = pDoc;
					aroundrect.m_fX1 = proundrect->m_fX1;
					aroundrect.m_fY1 = proundrect->m_fY1;
					aroundrect.m_fX2 = proundrect->m_fX2;
					aroundrect.m_fY2 = proundrect->m_fY2;
					aroundrect.m_fRadius_x = proundrect->m_fRadius_x;
					aroundrect.m_fRadius_y = proundrect->m_fRadius_y;
					aroundrect.m_nBorderWidth = proundrect->m_nBorderWidth;
					aroundrect.m_nBorderColor = proundrect->m_nBorderColor;
					aroundrect.m_nFillColor = proundrect->m_nFillColor;
					aroundrect.m_bFillSolid = proundrect->m_bFillSolid;
					aroundrect.m_nDiagram = proundrect->m_nDiagram;

					aroundrect.Draw(&dc, this);

					flag = 0;
					Write_Help(szHelp);
					nEDA_RedrawFlag = 0;
					do
					{
						ch1 = MouseKey();
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							CloseMouseCursor();
							aroundrect.Draw(&dc, this);
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
								aroundrect.Draw(&dc, this);
								OpenMouseCursor();
							}
							
							if(mode_flag == 1)
							{
								aroundrect.m_fX1 = m_fCurrentSnapX;
								aroundrect.m_fY1 = m_fCurrentSnapY;
							}
							else if(mode_flag == 2)
							{
								aroundrect.m_fX1 = m_fCurrentSnapX;
								aroundrect.m_fY2 = m_fCurrentSnapY;
							}
							else if(mode_flag == 3)
							{
								aroundrect.m_fX2 = m_fCurrentSnapX;
								aroundrect.m_fY1 = m_fCurrentSnapY;
							}
							else if(mode_flag == 4)
							{
								aroundrect.m_fX2 = m_fCurrentSnapX;
								aroundrect.m_fY2 = m_fCurrentSnapY;
							}
							else if(mode_flag == 5)
							{
								aroundrect.m_fY1 = m_fCurrentSnapY;
							}
							else if(mode_flag == 6)
							{
								aroundrect.m_fY2 = m_fCurrentSnapY;
							}
							else if(mode_flag == 7)
							{
								aroundrect.m_fX1 = m_fCurrentSnapX;
							}
							else if(mode_flag == 8)
							{
								aroundrect.m_fX2 = m_fCurrentSnapX;
							}
							else if(mode_flag == 9)
							{
								aroundrect.m_fRadius_x = m_fCurrentSnapX - aroundrect.m_fX1;
								if(aroundrect.m_fRadius_x < 0) aroundrect.m_fRadius_x = 0;
								else if(aroundrect.m_fRadius_x > (proundrect->m_fX1 + proundrect->m_fX2)/2) aroundrect.m_fRadius_x = (proundrect->m_fX1 + proundrect->m_fX2)/2;
								
								aroundrect.m_fRadius_y = m_fCurrentSnapY - aroundrect.m_fY1;
								if(aroundrect.m_fRadius_y < 0) aroundrect.m_fRadius_y = 0;
								else if(aroundrect.m_fRadius_y > (proundrect->m_fY1 + proundrect->m_fY2)/2) aroundrect.m_fRadius_y = (proundrect->m_fY1 + proundrect->m_fY2)/2;
							}
							else if(mode_flag == 10)
							{
								aroundrect.m_fRadius_x = m_fCurrentSnapX - aroundrect.m_fX1;
								if(aroundrect.m_fRadius_x < 0) aroundrect.m_fRadius_x = 0;
								else if(aroundrect.m_fRadius_x > (proundrect->m_fX1 + proundrect->m_fX2)/2) aroundrect.m_fRadius_x = (proundrect->m_fX1 + proundrect->m_fX2)/2;
								
								aroundrect.m_fRadius_y =  aroundrect.m_fY2 - m_fCurrentSnapY;
								if(aroundrect.m_fRadius_y < 0) aroundrect.m_fRadius_y = 0;
								else if(aroundrect.m_fRadius_y > (proundrect->m_fY1 + proundrect->m_fY2)/2) aroundrect.m_fRadius_y = (proundrect->m_fY1 + proundrect->m_fY2)/2;
							}
							else if(mode_flag == 11)
							{
								aroundrect.m_fRadius_x = aroundrect.m_fX2 - m_fCurrentSnapX;
								if(aroundrect.m_fRadius_x < 0) aroundrect.m_fRadius_x = 0;
								else if(aroundrect.m_fRadius_x > (proundrect->m_fX1 + proundrect->m_fX2)/2) aroundrect.m_fRadius_x = (proundrect->m_fX1 + proundrect->m_fX2)/2;
								
								aroundrect.m_fRadius_y = m_fCurrentSnapY - aroundrect.m_fY1;
								if(aroundrect.m_fRadius_y < 0) aroundrect.m_fRadius_y = 0;
								else if(aroundrect.m_fRadius_y > (proundrect->m_fY1 + proundrect->m_fY2)/2) aroundrect.m_fRadius_y = (proundrect->m_fY1 + proundrect->m_fY2)/2;
							}
							else if(mode_flag == 12)
							{
								aroundrect.m_fRadius_x = aroundrect.m_fX2 - m_fCurrentSnapX;
								if(aroundrect.m_fRadius_x < 0) aroundrect.m_fRadius_x = 0;
								else if(aroundrect.m_fRadius_x > (proundrect->m_fX1 + proundrect->m_fX2)/2) aroundrect.m_fRadius_x = (proundrect->m_fX1 + proundrect->m_fX2)/2;
								
								aroundrect.m_fRadius_y = aroundrect.m_fY2 - m_fCurrentSnapY;
								if(aroundrect.m_fRadius_y < 0) aroundrect.m_fRadius_y = 0;
								else if(aroundrect.m_fRadius_y > (proundrect->m_fY1 + proundrect->m_fY2)/2) aroundrect.m_fRadius_y = (proundrect->m_fY1 + proundrect->m_fY2)/2;
							}
							CloseMouseCursor();
							aroundrect.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							aroundrect.Draw(&dc, this);
							OpenMouseCursor();

							double fminx, fminy, fmaxx, fmaxy;
							fminx = MIN(aroundrect.m_fX1, aroundrect.m_fX2);
							fmaxx = MAX(aroundrect.m_fX1, aroundrect.m_fX2);
							fminy = MIN(aroundrect.m_fY1, aroundrect.m_fY2);
							fmaxy = MAX(aroundrect.m_fY1, aroundrect.m_fY2);
							aroundrect.m_fX1 = fminx;
							aroundrect.m_fY1 = fminy;
							aroundrect.m_fX2 = fmaxx;
							aroundrect.m_fY2 = fmaxy;

							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;

							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_MOVE;
							pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = position;
							pundo->m_fX1 = proundrect->m_fX1;
							pundo->m_fY1 = proundrect->m_fY1;
							pundo->m_fX2 = proundrect->m_fX2;
							pundo->m_fY2 = proundrect->m_fY2;
							pundo->m_fRx = proundrect->m_fRadius_x;
							pundo->m_fRy = proundrect->m_fRadius_y;
						
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							proundrect->m_fX1 = aroundrect.m_fX1;
							proundrect->m_fY1 = aroundrect.m_fY1;
							proundrect->m_fX2 = aroundrect.m_fX2;
							proundrect->m_fY2 = aroundrect.m_fY2;
							proundrect->m_fRadius_x = aroundrect.m_fRadius_x;
							proundrect->m_fRadius_y = aroundrect.m_fRadius_y;

							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					proundrect->m_nFlag = FLAG_NORMAL;
					proundrect->Draw(&dc, this);
					proundrect->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);
					return;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;
			if((psheetsymbol->m_nDiagram != m_nDiagram)||(psheetsymbol->m_nFlag == FLAG_DELETED)) continue;
			
			int mode_flag = 0;
			
			double fx1, fy1, fx2, fy2;
			
			fx1 = psheetsymbol->m_fX - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 1;
			
			
			fx1 = psheetsymbol->m_fX - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY - psheetsymbol->m_fYsize + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 2;
			
			
			fx1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + psheetsymbol->m_fXsize + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 3;
		
			
			fx1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + psheetsymbol->m_fXsize + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY - psheetsymbol->m_fYsize + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 4;
			
			//pView->DrawSelectionKeyPoint((x1+x2)/2, y1, pDC);
			fx1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize/2 - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + psheetsymbol->m_fXsize/2 + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 5;
			
			//pView->DrawSelectionKeyPoint((x1+x2)/2, y2, pDC);
			fx1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize/2 - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + psheetsymbol->m_fXsize/2 + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY - psheetsymbol->m_fYsize + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 6;
			
			//pView->DrawSelectionKeyPoint(x1, (y1+y2)/2, pDC);
			fx1 = psheetsymbol->m_fX - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize/2 - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY - psheetsymbol->m_fYsize/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 7;
						
			//pView->DrawSelectionKeyPoint(x2, (y1+y2)/2, pDC);
			fx1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize/2 - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + psheetsymbol->m_fXsize + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY - psheetsymbol->m_fYsize/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 8;

			if(mode_flag != 0)
			{
					
					int ix,iy;
					Get_VPort_XY(&ix, &iy, psheetsymbol->m_fX, psheetsymbol->m_fY);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, psheetsymbol->m_fX, psheetsymbol->m_fY - psheetsymbol->m_fYsize);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, psheetsymbol->m_fX + psheetsymbol->m_fXsize, psheetsymbol->m_fY);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, psheetsymbol->m_fX + psheetsymbol->m_fXsize, psheetsymbol->m_fY - psheetsymbol->m_fYsize);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, psheetsymbol->m_fX + psheetsymbol->m_fXsize/2, psheetsymbol->m_fY);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, psheetsymbol->m_fX + psheetsymbol->m_fXsize/2, psheetsymbol->m_fY - psheetsymbol->m_fYsize);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, psheetsymbol->m_fX, psheetsymbol->m_fY - psheetsymbol->m_fYsize/2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, psheetsymbol->m_fX + psheetsymbol->m_fXsize, psheetsymbol->m_fY - psheetsymbol->m_fYsize/2);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					psheetsymbol->m_nFlag = FLAG_DELETED;
					psheetsymbol->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schsheetsymbol asheetsymbol;
					asheetsymbol.pDocument = pDoc;
					asheetsymbol.m_fX = psheetsymbol->m_fX;
					asheetsymbol.m_fY = psheetsymbol->m_fY;
					asheetsymbol.m_fXsize = psheetsymbol->m_fXsize;
					asheetsymbol.m_fYsize = psheetsymbol->m_fYsize;
					asheetsymbol.m_nBorderWidth = psheetsymbol->m_nBorderWidth;
					asheetsymbol.m_nFillColor = psheetsymbol->m_nFillColor;
					asheetsymbol.m_nBorderColor = psheetsymbol->m_nBorderColor;
					asheetsymbol.m_bShowHiddenTextFields = FALSE;
					asheetsymbol.m_bFillSolid = psheetsymbol->m_bFillSolid;
					asheetsymbol.m_nDiagram = psheetsymbol->m_nDiagram;

					asheetsymbol.Draw(&dc, this);
							
					double fmaxx=0, fmaxy=0;
					for(int k=0; k<psheetsymbol->m_arrayPos.GetCount(); k++)
					{
						POSITION pos;
						pos = psheetsymbol->m_arrayPos.GetAt(k);
						pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
						{
							COb_schsheetentry* psheetentry;
							psheetentry = (COb_schsheetentry*)pobject;
							if((psheetentry->m_nSide == ENTRY_SIDE_LEFT)||(psheetentry->m_nSide == ENTRY_SIDE_RIGHT))
							{
								fmaxy = MAX(fmaxy, psheetentry->m_fOffset);
							}
							else
							{
								fmaxx = MAX(fmaxx, psheetentry->m_fOffset);
							}
						}
					}
					fmaxx += 50; fmaxy += 50;

					flag = 0;
					Write_Help(szHelp);
					nEDA_RedrawFlag = 0;
					do
					{
						ch1 = MouseKey();
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							CloseMouseCursor();
							asheetsymbol.Draw(&dc, this);
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
								asheetsymbol.Draw(&dc, this);
								OpenMouseCursor();
							}
							
							if(mode_flag == 1)
							{
								asheetsymbol.m_fX = m_fCurrentSnapX;
								asheetsymbol.m_fY = m_fCurrentSnapY;
								asheetsymbol.m_fXsize = (int)abs(psheetsymbol->m_fX - asheetsymbol.m_fX + psheetsymbol->m_fXsize);
								asheetsymbol.m_fYsize = (int)abs(asheetsymbol.m_fY - psheetsymbol->m_fY + psheetsymbol->m_fYsize);
								if(asheetsymbol.m_fXsize < fmaxx) asheetsymbol.m_fXsize = fmaxx;
								if(asheetsymbol.m_fYsize < fmaxy) asheetsymbol.m_fYsize = fmaxy;
							}
							else if(mode_flag == 2)
							{
								asheetsymbol.m_fX = m_fCurrentSnapX;
								asheetsymbol.m_fXsize = (int)abs(psheetsymbol->m_fX - asheetsymbol.m_fX + psheetsymbol->m_fXsize);
								asheetsymbol.m_fYsize = (int)abs(psheetsymbol->m_fY - m_fCurrentSnapY);
								if(asheetsymbol.m_fXsize < fmaxx) asheetsymbol.m_fXsize = fmaxx;
								if(asheetsymbol.m_fYsize < fmaxy) asheetsymbol.m_fYsize = fmaxy;
							}
							else if(mode_flag == 3)
							{
								asheetsymbol.m_fY = m_fCurrentSnapY;
								asheetsymbol.m_fXsize = (int)abs(psheetsymbol->m_fX - m_fCurrentSnapX);
								asheetsymbol.m_fYsize = (int)abs(asheetsymbol.m_fY - psheetsymbol->m_fY + psheetsymbol->m_fYsize);
								if(asheetsymbol.m_fXsize < fmaxx) asheetsymbol.m_fXsize = fmaxx;
								if(asheetsymbol.m_fYsize < fmaxy) asheetsymbol.m_fYsize = fmaxy;
							}
							else if(mode_flag == 4)
							{
								asheetsymbol.m_fXsize = (int)abs(psheetsymbol->m_fX - m_fCurrentSnapX);
								asheetsymbol.m_fYsize = (int)abs(psheetsymbol->m_fY - m_fCurrentSnapY);
								if(asheetsymbol.m_fXsize < fmaxx) asheetsymbol.m_fXsize = fmaxx;
								if(asheetsymbol.m_fYsize < fmaxy) asheetsymbol.m_fYsize = fmaxy;
							}
							else if(mode_flag == 5)
							{
								asheetsymbol.m_fY = m_fCurrentSnapY;
								asheetsymbol.m_fYsize = (int)abs(asheetsymbol.m_fY - psheetsymbol->m_fY + psheetsymbol->m_fYsize);
								if(asheetsymbol.m_fYsize < fmaxy) asheetsymbol.m_fYsize = fmaxy;								
							}
							else if(mode_flag == 6)
							{
								asheetsymbol.m_fYsize = (int)abs(psheetsymbol->m_fY - m_fCurrentSnapY);
								if(asheetsymbol.m_fYsize < fmaxy) asheetsymbol.m_fYsize = fmaxy;								
							}
							else if(mode_flag == 7)
							{
								asheetsymbol.m_fX = m_fCurrentSnapX;
								asheetsymbol.m_fXsize = (int)abs(psheetsymbol->m_fX - asheetsymbol.m_fX + psheetsymbol->m_fXsize);
								if(asheetsymbol.m_fXsize < fmaxx) asheetsymbol.m_fXsize = fmaxx;
							}
							else if(mode_flag == 8)
							{
								asheetsymbol.m_fXsize = (int)abs(psheetsymbol->m_fX - m_fCurrentSnapX);
								if(asheetsymbol.m_fXsize < fmaxx) asheetsymbol.m_fXsize = fmaxx;
							}

							CloseMouseCursor();
							asheetsymbol.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							asheetsymbol.Draw(&dc, this);
							OpenMouseCursor();

							
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;

							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_MOVE;
							pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = position;
							pundo->m_fX1 = psheetsymbol->m_fX;
							pundo->m_fY1 = psheetsymbol->m_fY;
							pundo->m_fX2 = psheetsymbol->m_fXsize;
							pundo->m_fY2 = psheetsymbol->m_fYsize;
					
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							psheetsymbol->m_fX = asheetsymbol.m_fX;
							psheetsymbol->m_fY = asheetsymbol.m_fY;
							psheetsymbol->m_fXsize = asheetsymbol.m_fXsize;
							psheetsymbol->m_fYsize = asheetsymbol.m_fYsize;
							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					psheetsymbol->m_nFlag = FLAG_NORMAL;
					
					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);
					
					Invalidate();
					return;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
		{
			COb_schsheetentry* psheetentry;
			psheetentry = (COb_schsheetentry*)pobject;
			if((psheetentry->m_nDiagram != m_nDiagram)||(psheetentry->m_nFlag == FLAG_DELETED)) continue;
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			
			double fx0, fy0, fx1, fy1, fx2, fy2;
			double fminx, fminy, fmaxx, fmaxy;
			if(pport->m_nStyle < PORT_STYLE_NV)
			{
				fminx = pport->m_fX;
				fminy = pport->m_fY - 50;
				fmaxx = pport->m_fX + pport->m_fSize;
				fmaxy = pport->m_fY + 50;
			}
			else
			{
				fminx = pport->m_fX - 50;
				fminy = pport->m_fY;
				fmaxx = pport->m_fX + 50;
				fmaxy = pport->m_fY + pport->m_fSize;
			}

			
			if(pport->m_nStyle < PORT_STYLE_NV)
			{
				fx0 = fmaxx;
				fy0 = (fminy + fmaxy)/2;
			}
			else
			{
				fx0 = (fminx + fmaxx)/2;
				fy0 = fmaxy;
			}

			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2))
			{
					
					int ix,iy;
					Get_VPort_XY(&ix, &iy, fx0, fy0);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					pport->m_nFlag = FLAG_DELETED;
					pport->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schport aport;
					aport.pDocument = pDoc;
					aport.m_fX = pport->m_fX;
					aport.m_fY = pport->m_fY;
					aport.m_fSize = pport->m_fSize;
					aport.m_nStyle = pport->m_nStyle;
					aport.m_nType = pport->m_nType;
					aport.m_nFillColor = pport->m_nFillColor;
					aport.m_nBorderColor = pport->m_nBorderColor;
					aport.m_nTextColor = pport->m_nTextColor;
					aport.m_csName.Empty();
					aport.m_nAlignment = pport->m_nAlignment;
					aport.m_nDiagram = pport->m_nDiagram;
					aport.Draw(&dc, this);
					
					
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
							aport.Draw(&dc, this);
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
								ElecHotPointsControl(&dc);
								CloseMouseCursor();
								aport.Draw(&dc, this);
								OpenMouseCursor();
							}

							if(pport->m_nStyle < PORT_STYLE_NV)
							{
								aport.m_fSize = (int)abs(m_fCurrentSnapX - aport.m_fX);
								if(aport.m_fSize < 100) aport.m_fSize = 100;
							}
							else
							{
								aport.m_fSize = (int)abs(m_fCurrentSnapY - aport.m_fY);
								if(aport.m_fSize < 100) aport.m_fSize = 100;
							}
							


							CloseMouseCursor();
							aport.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							ElecHotPointsControl(&dc);

							CloseMouseCursor();
							aport.Draw(&dc, this);
							OpenMouseCursor();
		
							
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;

							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_MOVE;
							pundo->m_nOb = SCH_ELEMENT_PORT;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = position;
							pundo->m_fX1 = pport->m_fX;
							pundo->m_fY1 = pport->m_fY;
							pundo->m_fX2 = pport->m_fSize;
							pundo->m_nOrientation = pport->m_nStyle;
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							pport->m_fX = aport.m_fX;
							pport->m_fY = aport.m_fY;
							pport->m_nStyle = aport.m_nStyle;
							pport->m_fSize = aport.m_fSize;
							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					pport->m_nFlag = FLAG_NORMAL;
					pport->Draw(&dc, this);
					pport->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}
	
					bEDA_NeedElectricalGrid = FALSE;
					m_fpHotPoint1.fx = m_fpHotPoint1.fy = m_fpHotPoint2.fx = m_fpHotPoint2.fy = 0;

					Write_Help(szEDA_Help_Ready);
					return;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
			{
				double fx1, fy1, fx2, fy2;
				fx1 = pbezier->m_cVertex[k].fx - nEDA_Selection_Offset;
				fy1 = pbezier->m_cVertex[k].fy - nEDA_Selection_Offset;
				fx2 = pbezier->m_cVertex[k].fx + nEDA_Selection_Offset;
				fy2 = pbezier->m_cVertex[k].fy + nEDA_Selection_Offset;
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2))
				{
					
					int ix,iy;
					Get_VPort_XY(&ix, &iy, pbezier->m_cVertex[k].fx, pbezier->m_cVertex[k].fy);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					pbezier->m_nFlag = FLAG_DELETED;
					pbezier->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schbezier abezier;
					abezier.pDocument = pDoc;
					abezier.m_nWidth = pbezier->m_nWidth;
					abezier.m_nColor = pbezier->m_nColor;
					abezier.m_nDiagram = pbezier->m_nDiagram;
					abezier.m_nFlag = FLAG_NORMAL;
					
					for(int i=0; i<pbezier->m_cVertex.GetCount(); i++)
					{
						Fpoint fpoint;
						fpoint = pbezier->m_cVertex.GetAt(i);
						abezier.m_cVertex.Add(fpoint);
					}

					abezier.Draw(&dc, this);

					flag = 0;
					Write_Help(szHelp);
					nEDA_RedrawFlag = 0;
					do
					{
						ch1 = MouseKey();
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							CloseMouseCursor();
							abezier.Draw(&dc, this);
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
								abezier.Draw(&dc, this);
								OpenMouseCursor();
							}

							abezier.m_cVertex[k].fx = m_fCurrentSnapX;
							abezier.m_cVertex[k].fy = m_fCurrentSnapY;

							CloseMouseCursor();
							abezier.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							abezier.Draw(&dc, this);
							OpenMouseCursor();

							abezier.m_cVertex[k].fx = m_fCurrentSnapX;
							abezier.m_cVertex[k].fy = m_fCurrentSnapY;
		
							
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;

							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_MOVE;
							pundo->m_nOb = SCH_ELEMENT_BEZIER;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = position;
							pundo->m_cVertex.RemoveAll();
							for(int i=0; i<pbezier->m_cVertex.GetCount(); i++)
							{
								Fpoint fpoint;
								fpoint = pbezier->m_cVertex.GetAt(i);
								pundo->m_cVertex.Add(fpoint);
							}			
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							pbezier->m_cVertex[k].fx = abezier.m_cVertex[k].fx;
							pbezier->m_cVertex[k].fy = abezier.m_cVertex[k].fy;
							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					pbezier->m_nFlag = FLAG_NORMAL;
					pbezier->Draw(&dc, this);
					pbezier->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);
					return;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			
			int mode_flag = 0;
			
			double fx1, fy1, fx2, fy2;
			//pView->DrawSelectionKeyPoint(x1, y1, pDC);
			fx1 = pgraphic->m_fX1 - nEDA_Selection_Offset;
			fy1 = pgraphic->m_fY1 - nEDA_Selection_Offset;
			fx2 = pgraphic->m_fX1 + nEDA_Selection_Offset;
			fy2 = pgraphic->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 1;

			//pView->DrawSelectionKeyPoint(x1, y2, pDC);
			fx1 = pgraphic->m_fX1 - nEDA_Selection_Offset;
			fy1 = pgraphic->m_fY2 - nEDA_Selection_Offset;
			fx2 = pgraphic->m_fX1 + nEDA_Selection_Offset;
			fy2 = pgraphic->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 2;

			//pView->DrawSelectionKeyPoint(x2, y1, pDC);
			fx1 = pgraphic->m_fX2 - nEDA_Selection_Offset;
			fy1 = pgraphic->m_fY1 - nEDA_Selection_Offset;
			fx2 = pgraphic->m_fX2 + nEDA_Selection_Offset;
			fy2 = pgraphic->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 3;

			//pView->DrawSelectionKeyPoint(x2, y2, pDC);
			fx1 = pgraphic->m_fX2 - nEDA_Selection_Offset;
			fy1 = pgraphic->m_fY2 - nEDA_Selection_Offset;
			fx2 = pgraphic->m_fX2 + nEDA_Selection_Offset;
			fy2 = pgraphic->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 4;

			//pView->DrawSelectionKeyPoint((x1+x2)/2, y1, pDC);
			fx1 = (pgraphic->m_fX1+pgraphic->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = pgraphic->m_fY1 - nEDA_Selection_Offset;
			fx2 = (pgraphic->m_fX1+pgraphic->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = pgraphic->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 5;

			//pView->DrawSelectionKeyPoint((x1+x2)/2, y2, pDC);
			fx1 = (pgraphic->m_fX1+pgraphic->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = pgraphic->m_fY2 - nEDA_Selection_Offset;
			fx2 = (pgraphic->m_fX1+pgraphic->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = pgraphic->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 6;

			//pView->DrawSelectionKeyPoint(x1, (y1+y2)/2, pDC);
			fx1 = pgraphic->m_fX1 - nEDA_Selection_Offset;
			fy1 = (pgraphic->m_fY1+pgraphic->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = pgraphic->m_fX1 + nEDA_Selection_Offset;
			fy2 = (pgraphic->m_fY1+pgraphic->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 7;

			//pView->DrawSelectionKeyPoint(x2, (y1+y2)/2, pDC);
			fx1 = pgraphic->m_fX2 - nEDA_Selection_Offset;
			fy1 = (pgraphic->m_fY1+pgraphic->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = pgraphic->m_fX2 + nEDA_Selection_Offset;
			fy2 = (pgraphic->m_fY1+pgraphic->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 8;

			if(mode_flag != 0)
			{
					
					int ix,iy;
					Get_VPort_XY(&ix, &iy, pgraphic->m_fX1, pgraphic->m_fY1);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, pgraphic->m_fX2, pgraphic->m_fY1);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, pgraphic->m_fX2, pgraphic->m_fY2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, pgraphic->m_fX1, pgraphic->m_fY2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, (pgraphic->m_fX1 + pgraphic->m_fX2)/2, pgraphic->m_fY1);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, (pgraphic->m_fX1 + pgraphic->m_fX2)/2, pgraphic->m_fY2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, pgraphic->m_fX1, (pgraphic->m_fY1 + pgraphic->m_fY2)/2);
					HideSelectionKeyPoint(ix, iy, &dc);
					Get_VPort_XY(&ix, &iy, pgraphic->m_fX2, (pgraphic->m_fY1 + pgraphic->m_fY2)/2);
					HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					char szHelp[] = "<左键><回车>放置, <右键><ESC>取消";

					pDoc->SetModifiedFlag(TRUE);
					

					
					pgraphic->m_nFlag = FLAG_DELETED;
					pgraphic->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_schrect arect;
					arect.pDocument = pDoc;
					arect.m_fX1 = pgraphic->m_fX1;
					arect.m_fY1 = pgraphic->m_fY1;
					arect.m_fX2 = pgraphic->m_fX2;
					arect.m_fY2 = pgraphic->m_fY2;
					arect.m_nBorderWidth = pgraphic->m_nBorderWidth;
					arect.m_nFillColor = RGB(255,255,255);
					arect.m_nBorderColor = pgraphic->m_nBorderColor;
					arect.m_bTransparent = FALSE;
					arect.m_bFillSolid = TRUE;
					arect.m_nDiagram = pgraphic->m_nDiagram;

					arect.Draw(&dc, this);

					flag = 0;
					Write_Help(szHelp);
					nEDA_RedrawFlag = 0;
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
								arect.Draw(&dc, this);
								OpenMouseCursor();
							}
							
							if(mode_flag == 1)
							{
								arect.m_fX1 = m_fCurrentSnapX;
								arect.m_fY1 = m_fCurrentSnapY;
							}
							else if(mode_flag == 2)
							{
								arect.m_fX1 = m_fCurrentSnapX;
								arect.m_fY2 = m_fCurrentSnapY;
							}
							else if(mode_flag == 3)
							{
								arect.m_fX2 = m_fCurrentSnapX;
								arect.m_fY1 = m_fCurrentSnapY;
							}
							else if(mode_flag == 4)
							{
								arect.m_fX2 = m_fCurrentSnapX;
								arect.m_fY2 = m_fCurrentSnapY;
							}
							else if(mode_flag == 5)
							{
								arect.m_fY1 = m_fCurrentSnapY;
							}
							else if(mode_flag == 6)
							{
								arect.m_fY2 = m_fCurrentSnapY;
							}
							else if(mode_flag == 7)
							{
								arect.m_fX1 = m_fCurrentSnapX;
							}
							else if(mode_flag == 8)
							{
								arect.m_fX2 = m_fCurrentSnapX;
							}

							CloseMouseCursor();
							arect.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							arect.Draw(&dc, this);
							OpenMouseCursor();

							double fminx, fminy, fmaxx, fmaxy;
							fminx = MIN(arect.m_fX1, arect.m_fX2);
							fmaxx = MAX(arect.m_fX1, arect.m_fX2);
							fminy = MIN(arect.m_fY1, arect.m_fY2);
							fmaxy = MAX(arect.m_fY1, arect.m_fY2);
							arect.m_fX1 = fminx;
							arect.m_fY1 = fminy;
							arect.m_fX2 = fmaxx;
							arect.m_fY2 = fmaxy;

							
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;

							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_MOVE;
							pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = position;
							pundo->m_fX1 = pgraphic->m_fX1;
							pundo->m_fY1 = pgraphic->m_fY1;
							pundo->m_fX2 = pgraphic->m_fX2;
							pundo->m_fY2 = pgraphic->m_fY2;
						
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							pgraphic->m_fX1 = arect.m_fX1;
							pgraphic->m_fY1 = arect.m_fY1;
							pgraphic->m_fX2 = arect.m_fX2;
							pgraphic->m_fY2 = arect.m_fY2;
							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					pgraphic->m_nFlag = FLAG_NORMAL;
					pgraphic->Draw(&dc, this);
					pgraphic->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nSCH_Undo_Num++;
						if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					Write_Help(szEDA_Help_Ready);
					return;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram != m_nDiagram)||(pcomp->m_nFlag == FLAG_DELETED))	continue;
			
		}
	}
}


void CESDView::PcbEditElement(int pcbelement, int index)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if((pcbelement == 0)||( index == -1)) return;

	if(pcbelement == PCB_ELEMENT_ARC)
	{
		COb_pcbarc* ppcbarc = pDoc->m_arrayPcbArc.GetAt(index);
		ppcbarc->Change(index);
	}
	else if(pcbelement == PCB_ELEMENT_TRACK)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(index);
		ptrack->Change(index);
	}
	else if(pcbelement == PCB_ELEMENT_VIA)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(index);
		pvia->Change(index);
	}
	else if(pcbelement == PCB_ELEMENT_TEXT)
	{
		COb_pcbtext* ppcbtext = pDoc->m_arrayPcbText.GetAt(index);
		ppcbtext->Change(index, this);
	}
	else if(pcbelement == PCB_ELEMENT_FILL)
	{
		COb_pcbfill* ppcbfill = pDoc->m_arrayPcbFill.GetAt(index);
		ppcbfill->Change(index);
	}
	else if(pcbelement == PCB_ELEMENT_PAD)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(index);
		ppad->Change(index);
	}
	else if(pcbelement == PCB_ELEMENT_REGION)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(index);
		pregion->Change(index);
	}
	else if(pcbelement == PCB_ELEMENT_COMP)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(index);
		pcomp->Change(index, this);
	}
	else if(pcbelement == PCB_ELEMENT_POLYGON)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(index);
		if(ppolygon->m_nPolygonType == COPPER_POLYGON) ppolygon->ChangePolygon(this, index);
		else if(ppolygon->m_nPolygonType == SPLITPLANE) ppolygon->ChangeSplitPlane(index);
	}
	else if(pcbelement == PCB_ELEMENT_COORDINATE)
	{
		COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(index);
        pcoordinate->Change(index);
	}
	else if(pcbelement == PCB_ELEMENT_DIMENSION)
	{
		COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(index);
		if(pdimension->m_nDimensionKind == 8) pdimension->Change(index, this);
		else if(pdimension->m_nDimensionKind == 1) pdimension->Change_Linear(index, this);
	}
}

void CESDView::PcbAdvancedChangeMode(double fx, double fy)
{
	CClientDC dc(this);	
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	for(int i=0; i<pDoc->m_arrayPcbSelection.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selected_element = pDoc->m_arrayPcbSelection.GetAt(i);

		if((selected_element.pcbelement == 0)||( selected_element.index < 0)) continue;
		if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);

			int action_flag = 0;
			double fx1, fy1, fx2, fy2;
			fx1 = ptrack->m_fX1 - nEDA_PcbSelection_Offset;
			fy1 = ptrack->m_fY1 - nEDA_PcbSelection_Offset;
			fx2 = ptrack->m_fX1 + nEDA_PcbSelection_Offset;
			fy2 = ptrack->m_fY1 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) action_flag = 1;
			fx1 = ptrack->m_fX2 - nEDA_PcbSelection_Offset;
			fy1 = ptrack->m_fY2 - nEDA_PcbSelection_Offset;
			fx2 = ptrack->m_fX2 + nEDA_PcbSelection_Offset;
			fy2 = ptrack->m_fY2 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) action_flag = 2;

			fx1 = (ptrack->m_fX1 + ptrack->m_fX2)/2 - nEDA_PcbSelection_Offset;
			fy1 = (ptrack->m_fY1 + ptrack->m_fY2)/2 - nEDA_PcbSelection_Offset;
			fx2 = (ptrack->m_fX1 + ptrack->m_fX2)/2 + nEDA_PcbSelection_Offset;
			fy2 = (ptrack->m_fY1 + ptrack->m_fY2)/2 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2))
			{
				if((GetKeyState(VK_CONTROL)&0x80) != 0 ) action_flag = 4;
				else action_flag = 3;
			}

			if((action_flag == 1)||(action_flag == 2))
			{
				
				int ix,iy;
				if(action_flag == 1) Get_VPort_XY(&ix, &iy, ptrack->m_fX1, ptrack->m_fY1);
				else Get_VPort_XY(&ix, &iy, ptrack->m_fX2, ptrack->m_fY2);

				HideSelectionKeyPoint(ix, iy, &dc);

				int ch1;
				BOOL clear_undo_buffer_flag = FALSE;	
				BOOL undo_flag = FALSE;  
					
				pDoc->SetModifiedFlag(TRUE);

				
				ptrack->m_nFlag = FLAG_DELETED;
				ptrack->Draw(&dc,this);
					
				int drawmode = dc.GetROP2();
				dc.SetROP2(R2_NOTXORPEN);	
					
				
				COb_pcbtrack atrack;
				atrack.Copy(ptrack);
				atrack.pDocument = pDoc;
				atrack.m_nFlag = FLAG_NORMAL;
				atrack.Draw(&dc, this);
						
				
				bEDA_NeedElectricalGrid = TRUE;

				nEDA_RedrawFlag = 0;
				do
				{
					ch1 = MouseKey();
					if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
					{
						PcbElecHotPointsControl(&dc);

						CloseMouseCursor();
						atrack.Draw(&dc, this);
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
							PcbElecHotPointsControl(&dc);

							CloseMouseCursor();
							atrack.Draw(&dc, this);
							OpenMouseCursor();
						}

						if(action_flag == 1)
						{
							atrack.m_fX1 = m_fCurrentSnapX;
							atrack.m_fY1 = m_fCurrentSnapY;
						}
						else
						{
							atrack.m_fX2 = m_fCurrentSnapX;
							atrack.m_fY2 = m_fCurrentSnapY;
						}

						CloseMouseCursor();
						atrack.Draw(&dc, this);
						OpenMouseCursor();
					}
					else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
					{
						PcbElecHotPointsControl(&dc);

						CloseMouseCursor();
						atrack.Draw(&dc, this);
						OpenMouseCursor();
						
						if(action_flag == 1)
						{
							atrack.m_fX1 = m_fCurrentSnapX;
							atrack.m_fY1 = m_fCurrentSnapY;
						}
						else
						{
							atrack.m_fX2 = m_fCurrentSnapX;
							atrack.m_fY2 = m_fCurrentSnapY;
						}
		
							
							if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = selected_element.index;
							pundo->pcbtrack.Copy(ptrack);
							pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
							
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

						if(action_flag == 1)
						{
							ptrack->m_fX1 = atrack.m_fX1;
							ptrack->m_fY1 = atrack.m_fY1;
						}
						else
						{
							ptrack->m_fX2 = atrack.m_fX2;
							ptrack->m_fY2 = atrack.m_fY2;
						}
						break;
					}
				}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

				dc.SetROP2(drawmode);
					
				CloseMouseCursor();
				
				ptrack->m_nFlag = FLAG_NORMAL;
				ptrack->Draw(&dc, this);
				ptrack->DrawSelection(&dc, this);
				OpenMouseCursor();

				if(m_bCursorState == TRUE) CloseMouseCursor();
				
				ClipCursor(NULL); m_bClipState = FALSE;

				
				if(undo_flag == TRUE)
				{
					pDoc->m_nPCB_Undo_Num++;
					if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				}

				bEDA_NeedElectricalGrid = FALSE;
				m_fpPcbHotPoint1.fx = m_fpPcbHotPoint1.fy = m_fpPcbHotPoint2.fx = m_fpPcbHotPoint2.fy = 0;

				return;
			}
			else if(action_flag == 3)
			{
				
				int ix,iy;
				Get_VPort_XY(&ix, &iy, ptrack->m_fX1, ptrack->m_fY1);
				HideSelectionKeyPoint(ix, iy, &dc);
				Get_VPort_XY(&ix, &iy, ptrack->m_fX2, ptrack->m_fY2);
				HideSelectionKeyPoint(ix, iy, &dc);
				Get_VPort_XY(&ix, &iy, (ptrack->m_fX1 + ptrack->m_fX2)/2, (ptrack->m_fY1 + ptrack->m_fY2)/2);
				HideSelectionKeyPoint(ix, iy, &dc);

				ptrack->Move(&dc, this, selected_element.index);
				return;
			}
			else if(action_flag == 4)
			{
				
				int ix,iy;
				Get_VPort_XY(&ix, &iy, ptrack->m_fX1, ptrack->m_fY1);
				HideSelectionKeyPoint(ix, iy, &dc);
				Get_VPort_XY(&ix, &iy, ptrack->m_fX2, ptrack->m_fY2);
				HideSelectionKeyPoint(ix, iy, &dc);
				Get_VPort_XY(&ix, &iy, (ptrack->m_fX1 + ptrack->m_fX2)/2, (ptrack->m_fY1 + ptrack->m_fY2)/2);
				HideSelectionKeyPoint(ix, iy, &dc);

				int ch1;
				BOOL clear_undo_buffer_flag = FALSE;	
				BOOL undo_flag = FALSE;  
					
				pDoc->SetModifiedFlag(TRUE);

				
				ptrack->m_nFlag = FLAG_DELETED;
				ptrack->Draw(&dc,this);
					
				int drawmode = dc.GetROP2();
				dc.SetROP2(R2_NOTXORPEN);	
					
								
				
				COb_pcbtrack atrack1;
				atrack1.Copy(ptrack);
				atrack1.pDocument = pDoc;
				atrack1.m_nFlag = FLAG_NORMAL;
				atrack1.m_fX2 = m_fCurrentSnapX;
				atrack1.m_fY2 = m_fCurrentSnapY;
				atrack1.Draw(&dc, this);

				COb_pcbtrack atrack2;
				atrack2.Copy(ptrack);
				atrack2.pDocument = pDoc;
				atrack2.m_nFlag = FLAG_NORMAL;
				atrack2.m_fX1 = m_fCurrentSnapX;
				atrack2.m_fY1 = m_fCurrentSnapY;
				atrack2.Draw(&dc, this);	

				
				bEDA_NeedElectricalGrid = TRUE;

				nEDA_RedrawFlag = 0;
				do
				{
					ch1 = MouseKey();
					if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
					{
						PcbElecHotPointsControl(&dc);

						CloseMouseCursor();
						atrack1.Draw(&dc, this);
						atrack2.Draw(&dc, this);
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
							PcbElecHotPointsControl(&dc);

							CloseMouseCursor();
							atrack1.Draw(&dc, this);
							atrack2.Draw(&dc, this);
							OpenMouseCursor();
						}

						atrack1.m_fX2 = m_fCurrentSnapX;
						atrack1.m_fY2 = m_fCurrentSnapY;
						atrack2.m_fX1 = m_fCurrentSnapX;
						atrack2.m_fY1 = m_fCurrentSnapY;

						CloseMouseCursor();
						atrack1.Draw(&dc, this);
						atrack2.Draw(&dc, this);
						OpenMouseCursor();
					}
					else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
					{
						PcbElecHotPointsControl(&dc);

						CloseMouseCursor();
						atrack1.Draw(&dc, this);
						atrack2.Draw(&dc, this);
						OpenMouseCursor();
						
						atrack1.m_fX2 = m_fCurrentSnapX;
						atrack1.m_fY2 = m_fCurrentSnapY;
						atrack2.m_fX1 = m_fCurrentSnapX;
						atrack2.m_fY1 = m_fCurrentSnapY;						
		
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
							
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = selected_element.index;
						pundo->pcbtrack.Copy(ptrack);
						pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
							
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						

						ptrack->m_fX2 = atrack1.m_fX2;
						ptrack->m_fY2 = atrack1.m_fY2;

						COb_pcbtrack* pnewtrack = new COb_pcbtrack();
						pnewtrack->Copy(&atrack2);
						pnewtrack->pDocument = pDoc;
						pnewtrack->m_nFlag = FLAG_NORMAL;
						int index = pDoc->m_arrayPcbTrack.Add(pnewtrack);
						
						pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
						break;
					}
				}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

				dc.SetROP2(drawmode);
					
				CloseMouseCursor();
				
				ptrack->m_nFlag = FLAG_NORMAL;
				ptrack->Draw(&dc, this);
				ptrack->DrawSelection(&dc, this);
				atrack2.Draw(&dc, this);
				OpenMouseCursor();

				if(m_bCursorState == TRUE) CloseMouseCursor();
				
				ClipCursor(NULL); m_bClipState = FALSE;

				
				if(undo_flag == TRUE)
				{
					pDoc->m_nPCB_Undo_Num++;
					if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				}

				bEDA_NeedElectricalGrid = FALSE;
				m_fpPcbHotPoint1.fx = m_fpPcbHotPoint1.fy = m_fpPcbHotPoint2.fx = m_fpPcbHotPoint2.fy = 0;

				return;
			}
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
				
			int mode_flag = 0;		
			
			double fx0, fy0, fx1, fy1, fx2, fy2;
			
			fx0 = parc->m_fCx + parc->m_fRadius*cos(parc->m_fEangle*fEDA_pi/180);
			fy0 = parc->m_fCy + parc->m_fRadius*sin(parc->m_fEangle*fEDA_pi/180);

			fx1 = fx0 - nEDA_PcbSelection_Offset;
			fy1 = fy0 - nEDA_PcbSelection_Offset;
			fx2 = fx0 + nEDA_PcbSelection_Offset;
			fy2 = fy0 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 1;
			
			fx0 = parc->m_fCx + parc->m_fRadius*cos(parc->m_fSangle*fEDA_pi/180);
			fy0 = parc->m_fCy + parc->m_fRadius*sin(parc->m_fSangle*fEDA_pi/180);

			fx1 = fx0 - nEDA_PcbSelection_Offset;
			fy1 = fy0 - nEDA_PcbSelection_Offset;
			fx2 = fx0 + nEDA_PcbSelection_Offset;
			fy2 = fy0 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 2;

			
			double alpha;
			if(parc->m_fEangle > parc->m_fSangle) alpha = (parc->m_fSangle + parc->m_fEangle)/2;
			else alpha = (parc->m_fSangle + parc->m_fEangle)/2 + 180;
			fx0 = parc->m_fCx + parc->m_fRadius*cos(alpha*fEDA_pi/180);
			fy0 = parc->m_fCy + parc->m_fRadius*sin(alpha*fEDA_pi/180);

			fx1 = fx0 - nEDA_PcbSelection_Offset;
			fy1 = fy0 - nEDA_PcbSelection_Offset;
			fx2 = fx0 + nEDA_PcbSelection_Offset;
			fy2 = fy0 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) mode_flag = 3;
			
			if(mode_flag != 0)
			{
				
				int ix,iy;
				
				fx0 = parc->m_fCx + parc->m_fRadius*cos(parc->m_fEangle*fEDA_pi/180);
				fy0 = parc->m_fCy + parc->m_fRadius*sin(parc->m_fEangle*fEDA_pi/180);
				Get_VPort_XY(&ix, &iy, fx0, fy0);
				HideSelectionKeyPoint(ix, iy, &dc);
				
				fx0 = parc->m_fCx + parc->m_fRadius*cos(parc->m_fSangle*fEDA_pi/180);
				fy0 = parc->m_fCy + parc->m_fRadius*sin(parc->m_fSangle*fEDA_pi/180);
				Get_VPort_XY(&ix, &iy, fx0, fy0);
				HideSelectionKeyPoint(ix, iy, &dc);
				fx0 = parc->m_fCx + parc->m_fRadius*cos(alpha*fEDA_pi/180);
				fy0 = parc->m_fCy + parc->m_fRadius*sin(alpha*fEDA_pi/180);
				Get_VPort_XY(&ix, &iy, fx0, fy0);
				HideSelectionKeyPoint(ix, iy, &dc);

					int ch1;
					int flag;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					pDoc->SetModifiedFlag(TRUE);

					
					parc->m_nFlag = FLAG_DELETED;
					parc->Draw(&dc,this);
					
					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	
					
					
					COb_pcbarc aarc;
					aarc.Copy(parc);
					aarc.pDocument = pDoc;
					aarc.Draw(&dc, this);

					flag = 0;
					nEDA_RedrawFlag = 0;
					do
					{
						ch1 = MouseKey();
						if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
						{
							PcbElecHotPointsControl(&dc);

							CloseMouseCursor();
							aarc.Draw(&dc, this);
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
								PcbElecHotPointsControl(&dc);

								CloseMouseCursor();
								aarc.Draw(&dc, this);
								OpenMouseCursor();
							}
							
							if(mode_flag == 1)
							{
								
								double alpha;
								if(((m_fCurrentSnapX - parc->m_fCx)==0)&&((m_fCurrentSnapY - parc->m_fCy)<0)) alpha = 270;
								if(((m_fCurrentSnapX - parc->m_fCx)==0)&&((m_fCurrentSnapY - parc->m_fCy)>0)) alpha = 90;
								if(((m_fCurrentSnapY - parc->m_fCy)==0)&&((m_fCurrentSnapX - parc->m_fCx)<0)) alpha = 180;
								if(((m_fCurrentSnapY - parc->m_fCy)==0)&&((m_fCurrentSnapX - parc->m_fCx)>0)) alpha = 0;								
								else alpha = (180.0/fEDA_pi)*atan2((m_fCurrentSnapY - parc->m_fCy),(m_fCurrentSnapX - parc->m_fCx));
								alpha = pDoc->Get_Correct_Angle(alpha);

								aarc.m_fEangle = alpha;
							}
							else if(mode_flag == 2)
							{
								
								double alpha;
								if(((m_fCurrentSnapX - parc->m_fCx)==0)&&((m_fCurrentSnapY - parc->m_fCy)<0)) alpha = 270;
								if(((m_fCurrentSnapX - parc->m_fCx)==0)&&((m_fCurrentSnapY - parc->m_fCy)>0)) alpha = 90;
								if(((m_fCurrentSnapY - parc->m_fCy)==0)&&((m_fCurrentSnapX - parc->m_fCx)<0)) alpha = 180;
								if(((m_fCurrentSnapY - parc->m_fCy)==0)&&((m_fCurrentSnapX - parc->m_fCx)>0)) alpha = 0;								
								else alpha = (180.0/fEDA_pi)*atan2((m_fCurrentSnapY - parc->m_fCy),(m_fCurrentSnapX - parc->m_fCx));
								alpha = pDoc->Get_Correct_Angle(alpha);

								aarc.m_fSangle = alpha;								
							}
							else if(mode_flag == 3)
							{
								
								aarc.m_fRadius = sqrt((m_fCurrentSnapX - parc->m_fCx)*(m_fCurrentSnapX - parc->m_fCx) + (m_fCurrentSnapY - parc->m_fCy)*(m_fCurrentSnapY - parc->m_fCy));
							}

							CloseMouseCursor();
							aarc.Draw(&dc, this);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							PcbElecHotPointsControl(&dc);

							CloseMouseCursor();
							aarc.Draw(&dc, this);
							OpenMouseCursor();
								
							if(aarc.m_fRadius == 0) break;

							
							if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = PCB_ELEMENT_ARC;
							pundo->m_nIndex = selected_element.index;
							pundo->pcbarc.Copy(parc);
							pundo->pcbarc.m_nFlag = FLAG_NORMAL;
							
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							parc->m_fSangle = aarc.m_fSangle;
							parc->m_fEangle = aarc.m_fEangle;
							parc->m_fRadius = aarc.m_fRadius;

							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					parc->m_nFlag = FLAG_NORMAL;
					parc->Draw(&dc, this);
					parc->DrawSelection(&dc, this);
					OpenMouseCursor();

					if(m_bCursorState == TRUE) CloseMouseCursor();
					
					ClipCursor(NULL); m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nPCB_Undo_Num++;
						if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					return;
			}
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
			
			int action_flag = 0;
			
			double fx1, fy1, fx2, fy2;
			double fxx1, fyy1, fxx2, fyy2, fxx3, fyy3, fxx4, fyy4;
			pfill->GetFillCorner(&fxx1, &fyy1, &fxx2, &fyy2, &fxx3, &fyy3, &fxx4, &fyy4);
			
			fx1 = fxx1 - nEDA_PcbSelection_Offset;
			fy1 = fyy1 - nEDA_PcbSelection_Offset;
			fx2 = fxx1 + nEDA_PcbSelection_Offset;
			fy2 = fyy1 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) action_flag = 1;
			fx1 = fxx2 - nEDA_PcbSelection_Offset;
			fy1 = fyy2 - nEDA_PcbSelection_Offset;
			fx2 = fxx2 + nEDA_PcbSelection_Offset;
			fy2 = fyy2 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) action_flag = 2;
			fx1 = fxx3 - nEDA_PcbSelection_Offset;
			fy1 = fyy3 - nEDA_PcbSelection_Offset;
			fx2 = fxx3 + nEDA_PcbSelection_Offset;
			fy2 = fyy3 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) action_flag = 3;
			fx1 = fxx4 - nEDA_PcbSelection_Offset;
			fy1 = fyy4 - nEDA_PcbSelection_Offset;
			fx2 = fxx4 + nEDA_PcbSelection_Offset;
			fy2 = fyy4 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) action_flag = 4;

			double fcx, fcy;
			fcx = (pfill->m_fX1 + pfill->m_fX2)/2;
			fcy = (pfill->m_fY1 + pfill->m_fY2)/2;
			double r = abs(pfill->m_fX2 - pfill->m_fX1)*0.35;
			double rx = fcx + r*cos(pfill->m_fRotation*fEDA_pi/180);
			double ry = fcy + r*sin(pfill->m_fRotation*fEDA_pi/180);

			fx1 = rx - nEDA_PcbSelection_Offset;
			fy1 = ry - nEDA_PcbSelection_Offset;
			fx2 = rx + nEDA_PcbSelection_Offset;
			fy2 = ry + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) action_flag = 5;

			
			int ch1;
			BOOL clear_undo_buffer_flag = FALSE;	
			BOOL undo_flag = FALSE;  
				
			pDoc->SetModifiedFlag(TRUE);
				
			
			if(m_bCursorState == TRUE) CloseMouseCursor();
			pfill->m_nFlag = FLAG_DELETED;
			
			pfill->Draw(&dc, this);

			int drawmode = dc.GetROP2();
			dc.SetROP2(R2_NOTXORPEN);	

			
			COb_pcbfill afill;
			afill.Copy(pfill);
			afill.m_nFlag = FLAG_NORMAL;
			afill.Draw(&dc, this);
			OpenMouseCursor();

			nEDA_RedrawFlag = 0;
			do
			{
				ch1 = MouseKey();
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					CloseMouseCursor();
					afill.Draw(&dc, this);
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
						afill.Draw(&dc, this);
						OpenMouseCursor();
					}

					

					if(action_flag == 1)
					{
						double fx0, fy0, fx1, fy1, fx2, fy2;
						fx2 = fxx2; 
						fy2 = fyy2;
						fx1 = m_fCurrentSnapX;
						fy1 = m_fCurrentSnapY;
						fx0 = (fx1 + fx2)/2;
						fy0 = (fy1 + fy2)/2;
							
						double a_b;
						if((fx1 != fx2)||(fy1 != fy2))
						{
							if(fy1 == fy2)
							{
								if(fx2 > fx1) a_b = 0;
								else a_b = 180;
							}
							else if(fx1 == fx2)
							{
								if(fy2 > fy1) a_b = 90;
								else a_b = 270;
							}
							else
							{
								a_b = atan2((fy2-fy1),(fx2-fx1))*180/fEDA_pi;
								a_b = pDoc->Get_Correct_Angle(a_b);
							}

							double alpha = a_b - pfill->m_fRotation;
							
							double fDiag;
							fDiag = sqrt((fx2 - fx1)*(fx2 - fx1) + (fy2 - fy1)*(fy2 - fy1))/2;
								
							afill.m_fX1 = fx0 - fDiag*cos(alpha*fEDA_pi/180);
							afill.m_fY1 = fy0 - fDiag*sin(alpha*fEDA_pi/180);	
							afill.m_fX2 = fx0 + fDiag*cos(alpha*fEDA_pi/180);
							afill.m_fY2 = fy0 + fDiag*sin(alpha*fEDA_pi/180);

							double fminx, fminy, fmaxx, fmaxy;
							fminx = MIN(afill.m_fX1, afill.m_fX2);
							fminy = MIN(afill.m_fY1, afill.m_fY2);
							fmaxx = MAX(afill.m_fX1, afill.m_fX2);
							fmaxy = MAX(afill.m_fY1, afill.m_fY2);
							afill.m_fX1 = fminx;
							afill.m_fY1 = fminy;
							afill.m_fX2 = fmaxx;
							afill.m_fY2 = fmaxy;
						}
						else 
						{
							afill.m_fX2 = afill.m_fX1 = pfill->m_fX1;
							afill.m_fY2 = afill.m_fY1 = pfill->m_fY1;		
						}
					}
					else if(action_flag == 2)
					{
						double fx0, fy0, fx1, fy1, fx2, fy2;
						fx1 = fxx1; 
						fy1 = fyy1;
						fx2 = m_fCurrentSnapX;
						fy2 = m_fCurrentSnapY;
						fx0 = (fx1 + fx2)/2;
						fy0 = (fy1 + fy2)/2;
							
						double a_b;
						if((fx1 != fx2)||(fy1 != fy2))
						{
							if(fy1 == fy2)
							{
								if(fx2 > fx1) a_b = 0;
								else a_b = 180;
							}
							else if(fx1 == fx2)
							{
								if(fy2 > fy1) a_b = 90;
								else a_b = 270;
							}
							else
							{
								a_b = atan2((fy2-fy1),(fx2-fx1))*180/fEDA_pi;
								a_b = pDoc->Get_Correct_Angle(a_b);
							}

							double alpha = a_b - pfill->m_fRotation;
								
							double fDiag;
							fDiag = sqrt((fx2 - fx1)*(fx2 - fx1) + (fy2 - fy1)*(fy2 - fy1))/2;
								
							afill.m_fX1 = fx0 - fDiag*cos(alpha*fEDA_pi/180);
							afill.m_fY1 = fy0 - fDiag*sin(alpha*fEDA_pi/180);	
							afill.m_fX2 = fx0 + fDiag*cos(alpha*fEDA_pi/180);
							afill.m_fY2 = fy0 + fDiag*sin(alpha*fEDA_pi/180);

							double fminx, fminy, fmaxx, fmaxy;
							fminx = MIN(afill.m_fX1, afill.m_fX2);
							fminy = MIN(afill.m_fY1, afill.m_fY2);
							fmaxx = MAX(afill.m_fX1, afill.m_fX2);
							fmaxy = MAX(afill.m_fY1, afill.m_fY2);
							afill.m_fX1 = fminx;
							afill.m_fY1 = fminy;
							afill.m_fX2 = fmaxx;
							afill.m_fY2 = fmaxy;
						}
						else 
						{
							afill.m_fX2 = afill.m_fX1 = pfill->m_fX1;
							afill.m_fY2 = afill.m_fY1 = pfill->m_fY1;		
						}
					}
					else if(action_flag == 3)
					{
						double fx0, fy0, fx1, fy1, fx2, fy2;
						fx2 = fxx4; 
						fy2 = fyy4;
						fx1 = m_fCurrentSnapX;
						fy1 = m_fCurrentSnapY;
						fx0 = (fx1 + fx2)/2;
						fy0 = (fy1 + fy2)/2;
							
						double a_b;
						if((fx1 != fx2)||(fy1 != fy2))
						{
							if(fy1 == fy2)
							{
								if(fx2 > fx1) a_b = 0;
								else a_b = 180;
							}
							else if(fx1 == fx2)
							{
								if(fy2 > fy1) a_b = 90;
								else a_b = 270;
							}
							else
							{
								a_b = atan2((fy2-fy1),(fx2-fx1))*180/fEDA_pi;
								a_b = pDoc->Get_Correct_Angle(a_b);
							}

							double alpha = a_b - pfill->m_fRotation;
								
							double fDiag;
							fDiag = sqrt((fx2 - fx1)*(fx2 - fx1) + (fy2 - fy1)*(fy2 - fy1))/2;
								
							afill.m_fX1 = fx0 - fDiag*cos(alpha*fEDA_pi/180);
							afill.m_fY1 = fy0 - fDiag*sin(alpha*fEDA_pi/180);	
							afill.m_fX2 = fx0 + fDiag*cos(alpha*fEDA_pi/180);
							afill.m_fY2 = fy0 + fDiag*sin(alpha*fEDA_pi/180);

							double fminx, fminy, fmaxx, fmaxy;
							fminx = MIN(afill.m_fX1, afill.m_fX2);
							fminy = MIN(afill.m_fY1, afill.m_fY2);
							fmaxx = MAX(afill.m_fX1, afill.m_fX2);
							fmaxy = MAX(afill.m_fY1, afill.m_fY2);
							afill.m_fX1 = fminx;
							afill.m_fY1 = fminy;
							afill.m_fX2 = fmaxx;
							afill.m_fY2 = fmaxy;
						}
						else 
						{
							afill.m_fX2 = afill.m_fX1 = pfill->m_fX1;
							afill.m_fY2 = afill.m_fY1 = pfill->m_fY1;		
						}
					}												
					else if(action_flag == 4)
					{
						double fx0, fy0, fx1, fy1, fx2, fy2;
						fx2 = fxx3; 
						fy2 = fyy3;
						fx1 = m_fCurrentSnapX;
						fy1 = m_fCurrentSnapY;
						fx0 = (fx1 + fx2)/2;
						fy0 = (fy1 + fy2)/2;
							
						double a_b;
						if((fx1 != fx2)||(fy1 != fy2))
						{
							if(fy1 == fy2)
							{
								if(fx2 > fx1) a_b = 0;
								else a_b = 180;
							}
							else if(fx1 == fx2)
							{
								if(fy2 > fy1) a_b = 90;
								else a_b = 270;
							}
							else
							{
								a_b = atan2((fy2-fy1),(fx2-fx1))*180/fEDA_pi;
								a_b = pDoc->Get_Correct_Angle(a_b);
							}

							double alpha = a_b - pfill->m_fRotation;
								
							double fDiag;
							fDiag = sqrt((fx2 - fx1)*(fx2 - fx1) + (fy2 - fy1)*(fy2 - fy1))/2;
								
							afill.m_fX1 = fx0 - fDiag*cos(alpha*fEDA_pi/180);
							afill.m_fY1 = fy0 - fDiag*sin(alpha*fEDA_pi/180);	
							afill.m_fX2 = fx0 + fDiag*cos(alpha*fEDA_pi/180);
							afill.m_fY2 = fy0 + fDiag*sin(alpha*fEDA_pi/180);

							double fminx, fminy, fmaxx, fmaxy;
							fminx = MIN(afill.m_fX1, afill.m_fX2);
							fminy = MIN(afill.m_fY1, afill.m_fY2);
							fmaxx = MAX(afill.m_fX1, afill.m_fX2);
							fmaxy = MAX(afill.m_fY1, afill.m_fY2);
							afill.m_fX1 = fminx;
							afill.m_fY1 = fminy;
							afill.m_fX2 = fmaxx;
							afill.m_fY2 = fmaxy;
						}
						else 
						{
							afill.m_fX2 = afill.m_fX1 = pfill->m_fX1;
							afill.m_fY2 = afill.m_fY1 = pfill->m_fY1;		
						}
					}
					else if(action_flag == 5)
					{
						double fx0, fy0; 
						fx0 = (pfill->m_fX1 + pfill->m_fX2)/2;
						fy0 = (pfill->m_fY1 + pfill->m_fY2)/2;

						if(m_fCurrentSnapY == fy0)
						{
							if(m_fCurrentSnapX >= fx0) afill.m_fRotation = 0;
							else afill.m_fRotation = 180;
						}
						else if(m_fCurrentSnapX == fx0)
						{
							if(m_fCurrentSnapY >= fy0) afill.m_fRotation = 90;
							else afill.m_fRotation = 270;
						}
						else
						{
							afill.m_fRotation = atan2((m_fCurrentSnapY - fy0), (m_fCurrentSnapX - fx0))*180/fEDA_pi;
						}
					}
					CloseMouseCursor();
					afill.Draw(&dc, this);
					OpenMouseCursor();
				}
	
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
				{
					CloseMouseCursor();
					afill.Draw(&dc, this);
					OpenMouseCursor();
	
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = selected_element.index;
					pundo->pcbfill.Copy(pfill);
					pundo->pcbfill.m_nFlag = FLAG_NORMAL;
						
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					

					pfill->m_fX1 = afill.m_fX1;
					pfill->m_fY1 = afill.m_fY1;
					pfill->m_fX2 = afill.m_fX2;
					pfill->m_fY2 = afill.m_fY2;
					pfill->m_fRotation = afill.m_fRotation;

					break;
				}
			}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

			dc.SetROP2(drawmode);
				
			CloseMouseCursor();
			
			pfill->m_nFlag = FLAG_NORMAL;
			pfill->Draw(&dc, this);
			pfill->DrawSelection(&dc, this);
			OpenMouseCursor();

			if(m_bCursorState == TRUE) CloseMouseCursor();
			
			ClipCursor(NULL); m_bClipState = FALSE;

			
			if(undo_flag == TRUE)
			{
				pDoc->m_nPCB_Undo_Num++;
				if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			}

			return;
		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_TEXT)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(selected_element.index);
			
			
			double fx1, fy1, fx2, fy2;
			double r = abs(ptext->m_fX2 - ptext->m_fX1);
			double rx, ry;
			if(ptext->m_bMirror == FALSE)
			{
				rx = ptext->m_fSx + r*cos(ptext->m_fRotation*fEDA_pi/180);
				ry = ptext->m_fSy + r*sin(ptext->m_fRotation*fEDA_pi/180);
			}
			else
			{
				rx = ptext->m_fSx - r*cos(ptext->m_fRotation*fEDA_pi/180);
				ry = ptext->m_fSy - r*sin(ptext->m_fRotation*fEDA_pi/180);
			}

			fx1 = rx - nEDA_PcbSelection_Offset;
			fy1 = ry - nEDA_PcbSelection_Offset;
			fx2 = rx + nEDA_PcbSelection_Offset;
			fy2 = ry + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2))
			{
				int ch1;
				double x1,y1;
				BOOL clear_undo_buffer_flag = FALSE;	
				BOOL undo_flag = FALSE;  
				CBitmap *pBitmap = NULL;	

				pDoc->SetModifiedFlag(TRUE);
				
				
				if(m_bCursorState == TRUE) CloseMouseCursor();
				ptext->m_nFlag = FLAG_DELETED;
				Invalidate();

				int drawmode = dc.GetROP2();
				dc.SetROP2(R2_NOTXORPEN);	

				
				COb_pcbtext atext;
				atext.Copy(ptext);
				atext.pDocument = pDoc;
				atext.m_nFlag = FLAG_NORMAL;
				
				if(atext.m_bUseTTFont == TRUE)
				{
					atext.UpdateStringBorder(this, FALSE);
					atext.DrawStringRect(&dc, this);
				}
				else atext.Draw(&dc, this);

				OpenMouseCursor();

				nEDA_RedrawFlag = 0;
				do
				{
					ch1 = MouseKey();
					if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
					{
						
						CloseMouseCursor();
						if(atext.m_bUseTTFont == TRUE)
						{
							atext.UpdateStringBorder(this, FALSE);
							atext.DrawStringRect(&dc, this);
						}
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
							if(atext.m_bUseTTFont == TRUE)
							{
								atext.UpdateStringBorder(this, FALSE);
								atext.DrawStringRect(&dc, this);
							}
							else atext.Draw(&dc, this);
							OpenMouseCursor();
						}
						
						double fx0, fy0;
						fx0 = atext.m_fSx;
						fy0 = atext.m_fSy;

						if(m_fCurrentSnapY == fy0)
						{
							if(m_fCurrentSnapX >= fx0) atext.m_fRotation = 0;
							else atext.m_fRotation = 180;
						}
						else if(m_fCurrentSnapX == fx0)
						{
							if(m_fCurrentSnapY >= fy0) atext.m_fRotation = 90;
							else atext.m_fRotation = 270;
						}
						else
						{
							atext.m_fRotation = atan2((m_fCurrentSnapY - fy0), (m_fCurrentSnapX - fx0))*180/fEDA_pi;
						}


						if(atext.m_bMirror == TRUE)
						{
							atext.m_fRotation += 180;
							if(atext.m_fRotation >= 360) atext.m_fRotation -= 360;
						}

					
						CloseMouseCursor();
						if(atext.m_bUseTTFont == TRUE)
						{
							atext.UpdateStringBorder(this, FALSE);
							atext.DrawStringRect(&dc, this);
						}
						else atext.Draw(&dc, this);
						OpenMouseCursor();
					}
					else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
					{
						CloseMouseCursor();
						if(atext.m_bUseTTFont == TRUE)
						{
							atext.UpdateStringBorder(this, FALSE);
							atext.DrawStringRect(&dc, this);
						}
						else atext.Draw(&dc, this);
						OpenMouseCursor();

						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = PCB_ELEMENT_TEXT;
						pundo->m_nIndex = selected_element.index;
						pundo->pcbtext.Copy(ptext);
						pundo->pcbtext.m_nFlag = FLAG_NORMAL;
						
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
						
						ptext->m_fRotation = atext.m_fRotation;
						ptext->UpdateStringBorder(this, FALSE);

						break;
					}
				}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));
				
				dc.SetROP2(drawmode);

				CloseMouseCursor();
				
				ptext->m_nFlag = FLAG_NORMAL;
				ptext->Draw(&dc, this);
				ptext->DrawSelection(&dc, this);

				if(m_bCursorState == TRUE) CloseMouseCursor();

				
				ClipCursor(NULL); m_bClipState = FALSE;

				
				if(undo_flag == TRUE)
				{
					pDoc->m_nPCB_Undo_Num++;
					if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				}

				return;
			}
		}		
		else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);

			for(int j=0; j<pregion->m_cVertex.GetCount(); j++)
			{
				Struct_RegionVertex vertex = pregion->m_cVertex.GetAt(j);

				double fx1, fy1, fx2, fy2;
				fx1 = vertex.fx - nEDA_PcbSelection_Offset;
				fy1 = vertex.fy - nEDA_PcbSelection_Offset;
				fx2 = vertex.fx + nEDA_PcbSelection_Offset;
				fy2 = vertex.fy + nEDA_PcbSelection_Offset;
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) 
				{
					int ch1;
					double x1,y1, x0, y0, deltax, deltay;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
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
							
							/*if(vertex.radius == 0)
							{*/
								vertex.fx = m_fCurrentSnapX;
								vertex.fy = m_fCurrentSnapY;
								aregion.m_cVertex.SetAt(j, vertex);
							/*}
							else
							{
								Struct_RegionVertex v_insert;
								v_insert.cx = v_insert.cy = v_insert.sangle = v_insert.eangle = v_insert.radius = 0;
								v_insert.fx = m_fCurrentSnapX;
								v_insert.fy = m_fCurrentSnapY;
								aregion.m_cVertex.InsertAt(j, v_insert);

								vertex = aregion.m_cVertex.GetAt(j);
							}*/


							CloseMouseCursor();
							aregion.FrameRegion(&dc, this, 1, nEDA_PCB_LayerColor[aregion.m_nLayer]);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							aregion.FrameRegion(&dc, this, 1, nEDA_PCB_LayerColor[aregion.m_nLayer]);
							OpenMouseCursor();

							
							if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = PCB_ELEMENT_REGION;
							pundo->m_nIndex = selected_element.index;
							pundo->pcbregion.Copy(pregion);
							pundo->pcbregion.m_nFlag = FLAG_NORMAL;
							
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							pregion->m_cVertex.RemoveAll();
							for(int k=0; k<aregion.m_cVertex.GetCount(); k++)
							{
								Struct_RegionVertex v_insert;
								v_insert = aregion.m_cVertex.GetAt(k);
								pregion->m_cVertex.Add(v_insert);
							}
							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					pregion->m_nFlag = FLAG_NORMAL;
					pregion->Draw(&dc, this);
					pregion->DrawSelection(&dc, this);
					OpenMouseCursor();

					if( m_bCursorState == TRUE)  CloseMouseCursor();
					
					ClipCursor(NULL);  m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nPCB_Undo_Num++;
						if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					return;
				}
			}
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_POLYGON)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(selected_element.index);
			
			for(int j=0; j<ppolygon->m_cVertex.GetCount(); j++)
			{
				Struct_RegionVertex vertex = ppolygon->m_cVertex.GetAt(j);

				double fx1, fy1, fx2, fy2;
				fx1 = vertex.fx - nEDA_PcbSelection_Offset;
				fy1 = vertex.fy - nEDA_PcbSelection_Offset;
				fx2 = vertex.fx + nEDA_PcbSelection_Offset;
				fy2 = vertex.fy + nEDA_PcbSelection_Offset;
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2))
				{
					int ch1;
					double x1,y1, x0, y0, deltax, deltay;
					BOOL clear_undo_buffer_flag = FALSE;	
					BOOL undo_flag = FALSE;  
					
					pDoc->SetModifiedFlag(TRUE);
					
					
					if(m_bCursorState == TRUE)  CloseMouseCursor();
					ppolygon->m_nFlag = FLAG_DELETED;
					
					Invalidate();

					int drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);	

					
					COb_pcbregion aregion;
					for(int i=0; i<ppolygon->m_cVertex.GetCount(); i++)
					{
						Struct_RegionVertex vertex;
						vertex = ppolygon->m_cVertex.GetAt(i);
						aregion.m_cVertex.Add(vertex);
					}
					
					aregion.m_nLayer = ppolygon->m_nLayer;
					
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
							
							/*if(vertex.radius == 0)
							{*/
								vertex.fx = m_fCurrentSnapX;
								vertex.fy = m_fCurrentSnapY;
								aregion.m_cVertex.SetAt(j, vertex);
							/*}
							else
							{
								Struct_RegionVertex v_insert;
								v_insert.cx = v_insert.cy = v_insert.sangle = v_insert.eangle = v_insert.radius = 0;
								v_insert.fx = m_fCurrentSnapX;
								v_insert.fy = m_fCurrentSnapY;
								aregion.m_cVertex.InsertAt(j, v_insert);

								vertex = aregion.m_cVertex.GetAt(j);
							}*/


							CloseMouseCursor();
							aregion.FrameRegion(&dc, this, 1, nEDA_PCB_LayerColor[aregion.m_nLayer]);
							OpenMouseCursor();
						}
						else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
						{
							CloseMouseCursor();
							aregion.FrameRegion(&dc, this, 1, nEDA_PCB_LayerColor[aregion.m_nLayer]);
							OpenMouseCursor();

							
							if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_CHANGE;
							pundo->m_nOb = PCB_ELEMENT_POLYGON;
							pundo->m_nIndex = selected_element.index;
							pundo->pcbpolygon.Copy(ppolygon);
							
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							ppolygon->m_cVertex.RemoveAll();
							for(int k=0; k<aregion.m_cVertex.GetCount(); k++)
							{
								Struct_RegionVertex v_insert;
								v_insert = aregion.m_cVertex.GetAt(k);
								ppolygon->m_cVertex.Add(v_insert);
							}
							break;
						}
					}while(( ch1 != VK_ESCAPE)&&( ch1 != PUSHMOUSEESC ));

					dc.SetROP2(drawmode);
					
					CloseMouseCursor();
					
					ppolygon->m_nFlag = FLAG_NORMAL;
					ppolygon->Draw(&dc, this);
					ppolygon->DrawSelection(&dc, this);
					OpenMouseCursor();

					if( m_bCursorState == TRUE)  CloseMouseCursor();
					
					ClipCursor(NULL);  m_bClipState = FALSE;

					
					if(undo_flag == TRUE)
					{
						pDoc->m_nPCB_Undo_Num++;
						if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					}

					return;
				}
			}
		}
	}
}


void CESDView::OnEditBreakWire()
{
	unsigned ch1;
	char szHelpText[] = "截断连线: <左键><回车>选择连线截断, <右键><ESC>结束命令";

	CESDApp* pApp = (CESDApp*)AfxGetApp();
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	MoveCursorToCenter();

	Write_Help(szHelpText);
	do
	{
       ch1 = MouseKey();
       if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
			Struct_Multi_Selection multi_selection = SelectSCHObject(m_fCurrentX, m_fCurrentY, MULTI_SELECTION);
			if(multi_selection.element == SCH_ELEMENT_LINE)
			{
				CObject* pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(multi_selection.pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline)))
				{
					COb_schline* pline = (COb_schline*)pobject;
					if(pline->m_nAttrib == LINE_BUS_ENTRY) continue;

					int width;
					if(pline->m_nAttrib == LINE_BUS) width = nEDA_Bus_Width[pline->m_nLineWidth];
					else width = nEDA_Line_Width[pline->m_nLineWidth];

					int delta = (int)(width/2) + nEDA_Selection_Offset;
					int vertex = pline->m_cVertex.GetCount();
					int i;
					for(i=0; i<vertex-1; i++)
					{
						double ax,ay,bx,by;
						double alpha;
						
						ax = pline->m_cVertex[i].fx;
						ay = pline->m_cVertex[i].fy;
						bx = pline->m_cVertex[i+1].fx;
						by = pline->m_cVertex[i+1].fy;
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

						if(rgn.PtInRegion(m_fCurrentSnapX, m_fCurrentSnapY) != FALSE) break;
					}

					double fx1, fy1, fx2, fy2;
					if(i < vertex-1)
					{
						
						if(pline->m_cVertex[i].fy == pline->m_cVertex[i+1].fy)
						{
							
							if(pline->m_cVertex[i].fx > pline->m_cVertex[i+1].fx)
							{
								fx1 = m_fCurrentSnapX + 50;
								fy1 = pline->m_cVertex[i].fy;
								fx2 = m_fCurrentSnapX - 50;
								fy2 = pline->m_cVertex[i].fy;
							}
							else
							{
								fx1 = m_fCurrentSnapX - 50;
								fy1 = pline->m_cVertex[i].fy;
								fx2 = m_fCurrentSnapX + 50;
								fy2 = pline->m_cVertex[i].fy;
							}
						}
						else if(pline->m_cVertex[i].fx == pline->m_cVertex[i+1].fx)
						{
							
							if(pline->m_cVertex[i].fy > pline->m_cVertex[i+1].fy)
							{
								fx1 = pline->m_cVertex[i].fx;
								fy1 = m_fCurrentSnapY + 50;
								fx2 = pline->m_cVertex[i].fx;
								fy2 = m_fCurrentSnapY - 50;
							}
							else
							{
								fx1 = pline->m_cVertex[i].fx;
								fy1 = m_fCurrentSnapY - 50;
								fx2 = pline->m_cVertex[i].fx;
								fy2 = m_fCurrentSnapY + 50;
							}
						}
						else
						{
							fx1 = fx2 = m_fCurrentSnapX;
							fy1 = fy2 = m_fCurrentSnapY;
						}
					}
					else continue;

					
					if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_LINE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = multi_selection.pos;
					pundo->schline.Copy(pline);
					pundo->schline.m_nFlag = FLAG_NORMAL;
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
					
					COb_schline aline;
					aline.Copy(pline);
					aline.m_cVertex.RemoveAll();
					aline.m_cVertex.SetSize(i+2);
					int k;
					for(k=0;k<i+1;k++)
					{
						aline.m_cVertex[k].fx = pline->m_cVertex[k].fx;
						aline.m_cVertex[k].fy = pline->m_cVertex[k].fy;
					}
					aline.m_cVertex[k].fx = fx1;
					aline.m_cVertex[k].fy = fy1;

					
					COb_schline* pnewline = new COb_schline();
					pnewline->Copy(pline);
					pnewline->m_cVertex.RemoveAll();
					pnewline->m_cVertex.SetSize(vertex-i);
					pnewline->m_cVertex[0].fx = fx2;
					pnewline->m_cVertex[0].fy = fy2;
					for(int k=1;k<vertex-i;k++)
					{
						pnewline->m_cVertex[k].fx = pline->m_cVertex[i+k].fx;
						pnewline->m_cVertex[k].fy = pline->m_cVertex[i+k].fy;
					}
					POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewline);
						
					
					pundo = new CSchUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = SCH_ELEMENT_LINE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = newpos;
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
					
					
					
					pDoc->SetModifiedFlag(TRUE);
					pDoc->m_nSCH_Undo_Num++;
					if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					
					
					pline->Copy(&aline);
					Invalidate();
				}
			}
			else if(multi_selection.element == SCH_ELEMENT_POLYLINE)
			{
				CObject* pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(multi_selection.pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline)))
				{
					COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;

					int width = nEDA_Line_Width[ppolyline->m_nLineWidth];

					int delta = (int)(width/2) + nEDA_Selection_Offset;
					int vertex = ppolyline->m_cVertex.GetCount();

					int i;
					for(i=0; i<vertex-1; i++)
					{
						double ax,ay,bx,by;
						double alpha;
						
						ax = ppolyline->m_cVertex[i].fx;
						ay = ppolyline->m_cVertex[i].fy;
						bx = ppolyline->m_cVertex[i+1].fx;
						by = ppolyline->m_cVertex[i+1].fy;
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

						if(rgn.PtInRegion(m_fCurrentSnapX, m_fCurrentSnapY) != FALSE) break;
					}

					double fx1, fy1, fx2, fy2;
					if(i < vertex-1)
					{
						
						if(ppolyline->m_cVertex[i].fy == ppolyline->m_cVertex[i+1].fy)
						{
							
							if(ppolyline->m_cVertex[i].fx > ppolyline->m_cVertex[i+1].fx)
							{
								fx1 = m_fCurrentSnapX + 50;
								fy1 = ppolyline->m_cVertex[i].fy;
								fx2 = m_fCurrentSnapX - 50;
								fy2 = ppolyline->m_cVertex[i].fy;
							}
							else
							{
								fx1 = m_fCurrentSnapX - 50;
								fy1 = ppolyline->m_cVertex[i].fy;
								fx2 = m_fCurrentSnapX + 50;
								fy2 = ppolyline->m_cVertex[i].fy;
							}
						}
						else if(ppolyline->m_cVertex[i].fx == ppolyline->m_cVertex[i+1].fx)
						{
							
							if(ppolyline->m_cVertex[i].fy > ppolyline->m_cVertex[i+1].fy)
							{
								fx1 = ppolyline->m_cVertex[i].fx;
								fy1 = m_fCurrentSnapY + 50;
								fx2 = ppolyline->m_cVertex[i].fx;
								fy2 = m_fCurrentSnapY - 50;
							}
							else
							{
								fx1 = ppolyline->m_cVertex[i].fx;
								fy1 = m_fCurrentSnapY - 50;
								fx2 = ppolyline->m_cVertex[i].fx;
								fy2 = m_fCurrentSnapY + 50;
							}
						}
						else
						{
							fx1 = fx2 = m_fCurrentSnapX;
							fy1 = fy2 = m_fCurrentSnapY;
						}
					}
					else continue;

					
					if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = SCH_ELEMENT_POLYLINE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = multi_selection.pos;
					pundo->schpolyline.Copy(ppolyline);
					pundo->schpolyline.m_nFlag = FLAG_NORMAL;
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
					
					COb_schpolyline apolyline;
					apolyline.Copy(ppolyline);
					apolyline.m_cVertex.RemoveAll();
					apolyline.m_cVertex.SetSize(i+2);

					int k;
					for(k=0;k<i+1;k++)
					{
						apolyline.m_cVertex[k].fx = ppolyline->m_cVertex[k].fx;
						apolyline.m_cVertex[k].fy = ppolyline->m_cVertex[k].fy;
					}
					apolyline.m_cVertex[k].fx = fx1;
					apolyline.m_cVertex[k].fy = fy1;

					
					COb_schpolyline* pnewpolyline = new COb_schpolyline();
					pnewpolyline->Copy(ppolyline);
					pnewpolyline->m_cVertex.RemoveAll();
					pnewpolyline->m_cVertex.SetSize(vertex-i);
					pnewpolyline->m_cVertex[0].fx = fx2;
					pnewpolyline->m_cVertex[0].fy = fy2;
					for(int k=1;k<vertex-i;k++)
					{
						pnewpolyline->m_cVertex[k].fx = ppolyline->m_cVertex[i+k].fx;
						pnewpolyline->m_cVertex[k].fy = ppolyline->m_cVertex[i+k].fy;
					}
					POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolyline);
						
					
					pundo = new CSchUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = SCH_ELEMENT_POLYLINE;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = newpos;
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
					
					
					
					pDoc->SetModifiedFlag(TRUE);
					pDoc->m_nSCH_Undo_Num++;
					if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
					
					
					ppolyline->Copy(&apolyline);
					Invalidate();
				}
			}
	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::AutoOptimizeWire()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	return;

	CObject* pobject;
	int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
	POSITION pos;
	pos = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{	
		if(pos == NULL) break;
		POSITION posbak = pos;
		pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED)||((pline->m_nAttrib != LINE_WIRE)&&(pline->m_nAttrib != LINE_BUS)))	continue;
			int  vertex_num = pline->m_cVertex.GetCount();

				CObject* pobject_1;
				int num_1 = pDoc->m_listSchObject[m_nDiagram].GetCount();
				POSITION pos_1;
				pos_1 = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
				for(int i_1=0; i_1<num_1; i_1++)
				{	
					if(pos_1 == NULL) break;
					POSITION posbak_1 = pos_1;
					pobject_1 = pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
					
					if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schline )))
					{
						COb_schline* pline_1 = (COb_schline*)pobject_1;
						if((pline_1->m_nDiagram != m_nDiagram)||(pline_1->m_nFlag == FLAG_DELETED)||((pline_1->m_nAttrib != LINE_WIRE)&&(pline_1->m_nAttrib != LINE_BUS)))	continue;
						int  vertex_num_1 = pline_1->m_cVertex.GetCount();
						
						if((pline->m_nAttrib == pline_1->m_nAttrib)&&(pline->m_nLineWidth == pline_1->m_nLineWidth)&&(pos != pos_1))   
						{
							if((pline->m_cVertex[0].fx == pline_1->m_cVertex[0].fx)&&(pline->m_cVertex[0].fy == pline_1->m_cVertex[0].fy))
							{
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_LINE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = posbak;
								pundo->schline.Copy(pline);
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								CSchUndo *pundo_1 = new CSchUndo;
								pundo_1->m_nOp = UNDO_UNDELETE;
								pundo_1->m_nOb = SCH_ELEMENT_LINE;
								pundo_1->m_nDiagram = m_nDiagram;
								pundo_1->m_nPos = posbak_1;
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo_1);
								
								pline->m_cVertex.RemoveAll();
								pline->m_cVertex.SetSize(vertex_num + vertex_num_1 - 1);
								for(int k=0; k<vertex_num; k++)
								{
									pline->m_cVertex[k].fx = pundo->schline.m_cVertex[vertex_num - k - 1].fx;
									pline->m_cVertex[k].fy = pundo->schline.m_cVertex[vertex_num - k - 1].fy;
								}
								for(int k=1; k<vertex_num_1; k++)
								{
									pline->m_cVertex[vertex_num + k - 1].fx = pline_1->m_cVertex[k].fx;
									pline->m_cVertex[vertex_num + k - 1].fy = pline_1->m_cVertex[k].fy;
								}

								pline_1->m_bSelection = FALSE;
								pline_1->m_nFlag = FLAG_DELETED;
							}
							else if((pline->m_cVertex[0].fx == pline_1->m_cVertex[vertex_num_1-1].fx)&&(pline->m_cVertex[0].fy == pline_1->m_cVertex[vertex_num_1-1].fy))
							{
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_LINE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = posbak;
								pundo->schline.Copy(pline);
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								CSchUndo *pundo_1 = new CSchUndo;
								pundo_1->m_nOp = UNDO_UNDELETE;
								pundo_1->m_nOb = SCH_ELEMENT_LINE;
								pundo_1->m_nDiagram = m_nDiagram;
								pundo_1->m_nPos = posbak_1;
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo_1);
								
								pline->m_cVertex.RemoveAll();
								pline->m_cVertex.SetSize(vertex_num + vertex_num_1 - 1);
								for(int k=0; k<vertex_num_1; k++)
								{
									pline->m_cVertex[k].fx = pline_1->m_cVertex[k].fx;
									pline->m_cVertex[k].fy = pline_1->m_cVertex[k].fy;
								}
								for(int k=1; k<vertex_num; k++)
								{
									pline->m_cVertex[vertex_num_1 + k - 1].fx = pundo->schline.m_cVertex[k].fx;
									pline->m_cVertex[vertex_num_1 + k - 1].fy = pundo->schline.m_cVertex[k].fy;
								}

								pline_1->m_bSelection = FALSE;
								pline_1->m_nFlag = FLAG_DELETED;
							}
							else if((pline->m_cVertex[vertex_num-1].fx == pline_1->m_cVertex[0].fx)&&(pline->m_cVertex[vertex_num-1].fy == pline_1->m_cVertex[0].fy))
							{
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_LINE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = posbak;
								pundo->schline.Copy(pline);
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								CSchUndo *pundo_1 = new CSchUndo;
								pundo_1->m_nOp = UNDO_UNDELETE;
								pundo_1->m_nOb = SCH_ELEMENT_LINE;
								pundo_1->m_nDiagram = m_nDiagram;
								pundo_1->m_nPos = posbak_1;
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo_1);
								
								pline->m_cVertex.RemoveAll();
								pline->m_cVertex.SetSize(vertex_num + vertex_num_1 - 1);
								for(int k=0; k<vertex_num; k++)
								{
									pline->m_cVertex[k].fx = pundo->schline.m_cVertex[k].fx;
									pline->m_cVertex[k].fy = pundo->schline.m_cVertex[k].fy;
								}
								for(int k=1; k<vertex_num_1; k++)
								{
									pline->m_cVertex[vertex_num + k - 1].fx = pline_1->m_cVertex[k].fx;
									pline->m_cVertex[vertex_num + k - 1].fy = pline_1->m_cVertex[k].fy;
								}

								pline_1->m_bSelection = FALSE;
								pline_1->m_nFlag = FLAG_DELETED;
							}
							else if((pline->m_cVertex[vertex_num-1].fx == pline_1->m_cVertex[vertex_num_1-1].fx)&&(pline->m_cVertex[vertex_num-1].fy == pline_1->m_cVertex[vertex_num_1-1].fy))
							{
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_LINE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = posbak;
								pundo->schline.Copy(pline);
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								CSchUndo *pundo_1 = new CSchUndo;
								pundo_1->m_nOp = UNDO_UNDELETE;
								pundo_1->m_nOb = SCH_ELEMENT_LINE;
								pundo_1->m_nDiagram = m_nDiagram;
								pundo_1->m_nPos = posbak_1;
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo_1);
								
								pline->m_cVertex.RemoveAll();
								pline->m_cVertex.SetSize(vertex_num + vertex_num_1 - 1);
								for(int k=0; k<vertex_num; k++)
								{
									pline->m_cVertex[k].fx = pundo->schline.m_cVertex[k].fx;
									pline->m_cVertex[k].fy = pundo->schline.m_cVertex[k].fy;
								}
								for(int k=1; k<vertex_num_1; k++)
								{
									pline->m_cVertex[vertex_num + k - 1].fx = pline_1->m_cVertex[vertex_num_1 - k - 1].fx;
									pline->m_cVertex[vertex_num + k - 1].fy = pline_1->m_cVertex[vertex_num_1 - k - 1].fy;
								}

								pline_1->m_bSelection = FALSE;
								pline_1->m_nFlag = FLAG_DELETED;
							}
							else if(((pline->m_cVertex[0].fx == pline->m_cVertex[1].fx)&&(pline_1->m_cVertex[0].fx == pline_1->m_cVertex[1].fx)&&\
									 (pline->m_cVertex[0].fx == pline_1->m_cVertex[1].fx)&&\
									 (pline->m_cVertex[0].fy > MIN(pline_1->m_cVertex[0].fy, pline_1->m_cVertex[1].fy))&&\
									 (pline->m_cVertex[0].fy < MAX(pline_1->m_cVertex[0].fy, pline_1->m_cVertex[1].fy))&&\
									 (pline_1->m_cVertex[0].fy > MIN(pline->m_cVertex[0].fy, pline->m_cVertex[1].fy))&&\
									 (pline_1->m_cVertex[0].fy < MAX(pline->m_cVertex[0].fy, pline->m_cVertex[1].fy)) ) ||
									((pline->m_cVertex[0].fy == pline->m_cVertex[1].fy)&&(pline_1->m_cVertex[0].fy == pline_1->m_cVertex[1].fy)&&\
									 (pline->m_cVertex[0].fy == pline_1->m_cVertex[1].fy)&&\
									 (pline->m_cVertex[0].fx > MIN(pline_1->m_cVertex[0].fx, pline_1->m_cVertex[1].fx))&&\
									 (pline->m_cVertex[0].fx < MAX(pline_1->m_cVertex[0].fx, pline_1->m_cVertex[1].fx))&&\
									 (pline_1->m_cVertex[0].fx > MIN(pline->m_cVertex[0].fx, pline->m_cVertex[1].fx))&&\
									 (pline_1->m_cVertex[0].fx < MAX(pline->m_cVertex[0].fx, pline->m_cVertex[1].fx)) )  )
							{
								
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_LINE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = posbak;
								pundo->schline.Copy(pline);
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								CSchUndo *pundo_1 = new CSchUndo;
								pundo_1->m_nOp = UNDO_UNDELETE;
								pundo_1->m_nOb = SCH_ELEMENT_LINE;
								pundo_1->m_nDiagram = m_nDiagram;
								pundo_1->m_nPos = posbak_1;
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo_1);
								
								pline->m_cVertex.RemoveAll();
								pline->m_cVertex.SetSize(vertex_num + vertex_num_1 - 2);
								for(int k=0; k<vertex_num-1; k++)
								{
									pline->m_cVertex[k].fx = pundo->schline.m_cVertex[vertex_num - k - 1].fx;
									pline->m_cVertex[k].fy = pundo->schline.m_cVertex[vertex_num - k - 1].fy;
								}
								for(int k=1; k<vertex_num_1; k++)
								{
									pline->m_cVertex[vertex_num - 1 + k - 1].fx = pline_1->m_cVertex[k].fx;
									pline->m_cVertex[vertex_num - 1 + k - 1].fy = pline_1->m_cVertex[k].fy;
								}
	
								pline_1->m_bSelection = FALSE;
								pline_1->m_nFlag = FLAG_DELETED;
							}
							else if(((pline->m_cVertex[0].fx == pline->m_cVertex[1].fx)&&(pline_1->m_cVertex[vertex_num_1-1].fx == pline_1->m_cVertex[vertex_num_1-2].fx)&&\
									 (pline->m_cVertex[0].fx == pline_1->m_cVertex[vertex_num_1-1].fx)&&\
									 (pline->m_cVertex[0].fy > MIN(pline_1->m_cVertex[vertex_num_1-1].fy, pline_1->m_cVertex[vertex_num_1-2].fy))&&\
									 (pline->m_cVertex[0].fy < MAX(pline_1->m_cVertex[vertex_num_1-1].fy, pline_1->m_cVertex[vertex_num_1-2].fy))&&\
									 (pline_1->m_cVertex[vertex_num_1-1].fy > MIN(pline->m_cVertex[0].fy, pline->m_cVertex[1].fy))&&\
									 (pline_1->m_cVertex[vertex_num_1-1].fy < MAX(pline->m_cVertex[0].fy, pline->m_cVertex[1].fy)) ) ||\
									((pline->m_cVertex[0].fy == pline->m_cVertex[1].fy)&&(pline_1->m_cVertex[vertex_num_1-1].fy == pline_1->m_cVertex[vertex_num_1-2].fy)&&\
									 (pline->m_cVertex[0].fy == pline_1->m_cVertex[vertex_num_1-1].fy)&&\
									 (pline->m_cVertex[0].fx > MIN(pline_1->m_cVertex[vertex_num_1-1].fx, pline_1->m_cVertex[vertex_num_1-2].fx))&&\
									 (pline->m_cVertex[0].fx < MAX(pline_1->m_cVertex[vertex_num_1-1].fx, pline_1->m_cVertex[vertex_num_1-2].fx))&&\
									 (pline_1->m_cVertex[vertex_num_1-1].fx > MIN(pline->m_cVertex[0].fx, pline->m_cVertex[1].fx))&&\
									 (pline_1->m_cVertex[vertex_num_1-1].fx < MAX(pline->m_cVertex[0].fx, pline->m_cVertex[1].fx)) )  )
							{
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_LINE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = posbak;
								pundo->schline.Copy(pline);
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								CSchUndo *pundo_1 = new CSchUndo;
								pundo_1->m_nOp = UNDO_UNDELETE;
								pundo_1->m_nOb = SCH_ELEMENT_LINE;
								pundo_1->m_nDiagram = m_nDiagram;
								pundo_1->m_nPos = posbak_1;
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo_1);
								
								pline->m_cVertex.RemoveAll();
								pline->m_cVertex.SetSize(vertex_num + vertex_num_1 - 2);
								for(int k=0; k<vertex_num_1-1; k++)
								{
									pline->m_cVertex[k].fx = pline_1->m_cVertex[k].fx;
									pline->m_cVertex[k].fy = pline_1->m_cVertex[k].fy;
								}
								for(int k=1; k<vertex_num; k++)
								{
									pline->m_cVertex[vertex_num_1 - 1 + k - 1].fx = pundo->schline.m_cVertex[k].fx;
									pline->m_cVertex[vertex_num_1 - 1 + k - 1].fy = pundo->schline.m_cVertex[k].fy;
								}

								pline_1->m_bSelection = FALSE;
								pline_1->m_nFlag = FLAG_DELETED;
							}
							else if(((pline_1->m_cVertex[0].fx == pline_1->m_cVertex[1].fx)&&(pline->m_cVertex[vertex_num-1].fx == pline->m_cVertex[vertex_num-2].fx)&&\
									 (pline_1->m_cVertex[0].fx == pline->m_cVertex[vertex_num-1].fx)&&\
									 (pline_1->m_cVertex[0].fy > MIN(pline->m_cVertex[vertex_num-1].fy, pline->m_cVertex[vertex_num-2].fy))&&\
									 (pline_1->m_cVertex[0].fy < MAX(pline->m_cVertex[vertex_num-1].fy, pline->m_cVertex[vertex_num-2].fy))&&\
									 (pline->m_cVertex[vertex_num-1].fy > MIN(pline_1->m_cVertex[0].fy, pline_1->m_cVertex[1].fy))&&\
									 (pline->m_cVertex[vertex_num-1].fy < MAX(pline_1->m_cVertex[0].fy, pline_1->m_cVertex[1].fy)) ) ||\
									 ((pline_1->m_cVertex[0].fy == pline_1->m_cVertex[1].fy)&&(pline->m_cVertex[vertex_num-1].fy == pline->m_cVertex[vertex_num-2].fy)&&\
									 (pline_1->m_cVertex[0].fy == pline->m_cVertex[vertex_num-1].fy)&&\
									 (pline_1->m_cVertex[0].fx > MIN(pline->m_cVertex[vertex_num-1].fx, pline->m_cVertex[vertex_num-2].fx))&&\
									 (pline_1->m_cVertex[0].fx < MAX(pline->m_cVertex[vertex_num-1].fx, pline->m_cVertex[vertex_num-2].fx))&&\
									 (pline->m_cVertex[vertex_num-1].fx > MIN(pline_1->m_cVertex[0].fx, pline_1->m_cVertex[1].fx))&&\
									 (pline->m_cVertex[vertex_num-1].fx < MAX(pline_1->m_cVertex[0].fx, pline_1->m_cVertex[1].fx)) ))
							{
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_LINE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = posbak;
								pundo->schline.Copy(pline);
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								CSchUndo *pundo_1 = new CSchUndo;
								pundo_1->m_nOp = UNDO_UNDELETE;
								pundo_1->m_nOb = SCH_ELEMENT_LINE;
								pundo_1->m_nDiagram = m_nDiagram;
								pundo_1->m_nPos = posbak_1;
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo_1);
								
								pline->m_cVertex.RemoveAll();
								pline->m_cVertex.SetSize(vertex_num + vertex_num_1 - 2);
								for(int k=0; k<vertex_num-1; k++)
								{
									pline->m_cVertex[k].fx = pundo->schline.m_cVertex[k].fx;
									pline->m_cVertex[k].fy = pundo->schline.m_cVertex[k].fy;
								}
								for(int k=1; k<vertex_num_1; k++)
								{
									pline->m_cVertex[vertex_num - 1 + k - 1].fx = pline_1->m_cVertex[k].fx;
									pline->m_cVertex[vertex_num - 1 + k - 1].fy = pline_1->m_cVertex[k].fy;
								}

								pline_1->m_bSelection = FALSE;
								pline_1->m_nFlag = FLAG_DELETED;
							}
							else if(((pline->m_cVertex[vertex_num-1].fx == pline->m_cVertex[vertex_num-2].fx)&&(pline_1->m_cVertex[vertex_num_1-1].fx == pline_1->m_cVertex[vertex_num_1-2].fx)&&\
									 (pline->m_cVertex[vertex_num-1].fx == pline_1->m_cVertex[vertex_num_1-1].fx)&&\
									 (pline->m_cVertex[vertex_num-1].fy > MIN(pline_1->m_cVertex[vertex_num_1-1].fy, pline_1->m_cVertex[vertex_num_1-2].fy))&&\
									 (pline->m_cVertex[vertex_num-1].fy < MAX(pline_1->m_cVertex[vertex_num_1-1].fy, pline_1->m_cVertex[vertex_num_1-2].fy))&&\
									 (pline_1->m_cVertex[vertex_num_1-1].fy > MIN(pline->m_cVertex[vertex_num-1].fy, pline->m_cVertex[vertex_num-2].fy))&&\
									 (pline_1->m_cVertex[vertex_num_1-1].fy < MAX(pline->m_cVertex[vertex_num-1].fy, pline->m_cVertex[vertex_num-2].fy)) ) ||\
									((pline->m_cVertex[vertex_num-1].fy == pline->m_cVertex[vertex_num-2].fy)&&(pline_1->m_cVertex[vertex_num_1-1].fy == pline_1->m_cVertex[vertex_num_1-2].fy)&&\
									 (pline->m_cVertex[vertex_num-1].fy == pline_1->m_cVertex[vertex_num_1-1].fy)&&\
									 (pline->m_cVertex[vertex_num-1].fx > MIN(pline_1->m_cVertex[vertex_num_1-1].fx, pline_1->m_cVertex[vertex_num_1-2].fx))&&\
									 (pline->m_cVertex[vertex_num-1].fx < MAX(pline_1->m_cVertex[vertex_num_1-1].fx, pline_1->m_cVertex[vertex_num_1-2].fx))&&\
									 (pline_1->m_cVertex[vertex_num_1-1].fx > MIN(pline->m_cVertex[vertex_num-1].fx, pline->m_cVertex[vertex_num-2].fx))&&\
									 (pline_1->m_cVertex[vertex_num_1-1].fx < MAX(pline->m_cVertex[vertex_num-1].fx, pline->m_cVertex[vertex_num-2].fx)) )  )
							{
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_LINE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = posbak;
								pundo->schline.Copy(pline);
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								CSchUndo *pundo_1 = new CSchUndo;
								pundo_1->m_nOp = UNDO_UNDELETE;
								pundo_1->m_nOb = SCH_ELEMENT_LINE;
								pundo_1->m_nDiagram = m_nDiagram;
								pundo_1->m_nPos = posbak_1;
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo_1);
								
								pline->m_cVertex.RemoveAll();
								pline->m_cVertex.SetSize(vertex_num + vertex_num_1 - 2);
								for(int k=0; k<vertex_num-1; k++)
								{
									pline->m_cVertex[k].fx = pundo->schline.m_cVertex[k].fx;
									pline->m_cVertex[k].fy = pundo->schline.m_cVertex[k].fy;
								}
								for(int k=1; k<vertex_num_1; k++)
								{
									pline->m_cVertex[vertex_num - 1 + k - 1].fx = pline_1->m_cVertex[vertex_num_1 - k - 1].fx;
									pline->m_cVertex[vertex_num - 1 + k - 1].fy = pline_1->m_cVertex[vertex_num_1 - k - 1].fy;
								}

								pline_1->m_bSelection = FALSE;
								pline_1->m_nFlag = FLAG_DELETED;
							}
						}
					}
					else if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
					{
						COb_schsheetsymbol* psheetsymbol1;
						psheetsymbol1 = (COb_schsheetsymbol*)pobject_1;
						for(int j1=0; j1<psheetsymbol1->m_arrayPos.GetSize(); j1++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
					}
					else if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						COb_schcomp* pcomp1;
						pcomp1 = (COb_schcomp*)pobject_1;
						for(int j1=0; j1<pcomp1->m_arrayPos.GetSize(); j1++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
					}
				}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;
			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
	}
}

void CESDView::AutoPlaceJunction()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	SYSTEMTIME start_time;
	GetSystemTime(&start_time);
	

	CClientDC dc(this);

	
	CObject* pobject;
	int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
	POSITION pos;
	pos = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{	
		if(pos == NULL) break;
		POSITION posbak = pos;
		pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED)||(pline->m_nAttrib != LINE_WIRE))	continue;
			int  vertex_num = pline->m_cVertex.GetCount();

				CObject* pobject_1;
				int num_1 = pDoc->m_listSchObject[m_nDiagram].GetCount();
				POSITION pos_1;
				pos_1 = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
				for(int i_1=0; i_1<num_1; i_1++)
				{	
					if(pos_1 == NULL) break;
					POSITION posbak_1 = pos_1;
					pobject_1 = pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
					
					if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schline )))
					{
						COb_schline* pline_1 = (COb_schline*)pobject_1;
						if((pline_1->m_nDiagram != m_nDiagram)||(pline_1->m_nFlag == FLAG_DELETED)||(pline_1->m_nAttrib != LINE_WIRE))	continue;
						int  vertex_num_1 = pline_1->m_cVertex.GetCount();
						
						if(pos != pos_1)   
						{
							for(int k=0; k<vertex_num-1; k++)
							{
								if( (pline->m_cVertex[k].fx == pline->m_cVertex[k+1].fx)&&(pline_1->m_cVertex[0].fx == pline->m_cVertex[k].fx)&&\
								    (pline_1->m_cVertex[0].fy >= MIN(pline->m_cVertex[k].fy, pline->m_cVertex[k+1].fy))&&\
									(pline_1->m_cVertex[0].fy <= MAX(pline->m_cVertex[k].fy, pline->m_cVertex[k+1].fy))&&\
									((pline_1->m_cVertex[0].fx != pline->m_cVertex[0].fx)||(pline_1->m_cVertex[0].fy != pline->m_cVertex[0].fy))&&\
									((pline_1->m_cVertex[0].fx != pline->m_cVertex[vertex_num-1].fx)||(pline_1->m_cVertex[0].fy != pline->m_cVertex[vertex_num-1].fy)) )
								{
									if(pDoc->CheckJuncExist(pline_1->m_cVertex[0].fx, pline_1->m_cVertex[0].fy, m_nDiagram) == FALSE)
									{
										COb_schjunc* pjunc = new COb_schjunc;
										pjunc->pDocument = pDoc;
										pjunc->m_fCx = pline_1->m_cVertex[0].fx;
										pjunc->m_fCy = pline_1->m_cVertex[0].fy;
										pjunc->m_nSize = cEDA_schautojunc.m_nSize;
										pjunc->m_nColor = cEDA_schautojunc.m_nColor;
										pjunc->m_bManual = FALSE;
										pjunc->m_nDiagram = m_nDiagram;
										pjunc->Draw(&dc, this);

										POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pjunc);
										
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_DELETE;
										pundo->m_nOb = SCH_ELEMENT_JUNCTION;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = subpos;
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
										
									}
								}
								else if((pline->m_cVertex[k].fx == pline->m_cVertex[k+1].fx)&&(pline_1->m_cVertex[vertex_num_1-1].fx == pline->m_cVertex[k].fx)&&\
										(pline_1->m_cVertex[vertex_num_1-1].fy >= MIN(pline->m_cVertex[k].fy, pline->m_cVertex[k+1].fy))&&\
										(pline_1->m_cVertex[vertex_num_1-1].fy <= MAX(pline->m_cVertex[k].fy, pline->m_cVertex[k+1].fy))&&\
										((pline_1->m_cVertex[vertex_num_1-1].fx != pline->m_cVertex[0].fx)||(pline_1->m_cVertex[vertex_num_1-1].fy != pline->m_cVertex[0].fy))&&\
										((pline_1->m_cVertex[vertex_num_1-1].fx != pline->m_cVertex[vertex_num-1].fx)||(pline_1->m_cVertex[vertex_num_1-1].fy != pline->m_cVertex[vertex_num-1].fy)) )
								{
									if(pDoc->CheckJuncExist(pline_1->m_cVertex[vertex_num_1-1].fx, pline_1->m_cVertex[vertex_num_1-1].fy, m_nDiagram) == FALSE)
									{
										COb_schjunc* pjunc = new COb_schjunc;
										pjunc->pDocument = pDoc;
										pjunc->m_fCx = pline_1->m_cVertex[vertex_num_1-1].fx;
										pjunc->m_fCy = pline_1->m_cVertex[vertex_num_1-1].fy;
										pjunc->m_nSize = cEDA_schautojunc.m_nSize;
										pjunc->m_nColor = cEDA_schautojunc.m_nColor;
										pjunc->m_bManual = FALSE;
										pjunc->m_nDiagram = m_nDiagram;
										pjunc->Draw(&dc, this);

										POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pjunc);
										
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_DELETE;
										pundo->m_nOb = SCH_ELEMENT_JUNCTION;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = subpos;
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
										
									}
								}
								else if((pline->m_cVertex[k].fy == pline->m_cVertex[k+1].fy)&&(pline_1->m_cVertex[0].fy == pline->m_cVertex[k].fy)&&\
										(pline_1->m_cVertex[0].fx >= MIN(pline->m_cVertex[k].fx, pline->m_cVertex[k+1].fx))&&\
										(pline_1->m_cVertex[0].fx <= MAX(pline->m_cVertex[k].fx, pline->m_cVertex[k+1].fx))&&\
										((pline_1->m_cVertex[0].fx != pline->m_cVertex[0].fx)||(pline_1->m_cVertex[0].fy != pline->m_cVertex[0].fy))&&\
										((pline_1->m_cVertex[0].fx != pline->m_cVertex[vertex_num-1].fx)||(pline_1->m_cVertex[0].fy != pline->m_cVertex[vertex_num-1].fy)) )
								{
									if(pDoc->CheckJuncExist(pline_1->m_cVertex[0].fx, pline_1->m_cVertex[0].fy, m_nDiagram) == FALSE)
									{
										COb_schjunc* pjunc = new COb_schjunc;
										pjunc->pDocument = pDoc;
										pjunc->m_fCx = pline_1->m_cVertex[0].fx;
										pjunc->m_fCy = pline_1->m_cVertex[0].fy;
										pjunc->m_nSize = cEDA_schautojunc.m_nSize;
										pjunc->m_nColor = cEDA_schautojunc.m_nColor;
										pjunc->m_bManual = FALSE;
										pjunc->m_nDiagram = m_nDiagram;
										pjunc->Draw(&dc, this);

										POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pjunc);
										
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_DELETE;
										pundo->m_nOb = SCH_ELEMENT_JUNCTION;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = subpos;
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
										
									}
								}
								else if((pline->m_cVertex[k].fy == pline->m_cVertex[k+1].fy)&&(pline_1->m_cVertex[vertex_num_1-1].fy == pline->m_cVertex[k].fy)&&\
										(pline_1->m_cVertex[vertex_num_1-1].fx >= MIN(pline->m_cVertex[k].fx, pline->m_cVertex[k+1].fx))&&\
										(pline_1->m_cVertex[vertex_num_1-1].fx <= MAX(pline->m_cVertex[k].fx, pline->m_cVertex[k+1].fx))&&\
										((pline_1->m_cVertex[vertex_num_1-1].fx != pline->m_cVertex[0].fx)||(pline_1->m_cVertex[vertex_num_1-1].fy != pline->m_cVertex[0].fy))&&\
										((pline_1->m_cVertex[vertex_num_1-1].fx != pline->m_cVertex[vertex_num-1].fx)||(pline_1->m_cVertex[vertex_num_1-1].fy != pline->m_cVertex[vertex_num-1].fy)) )
								{
									if(pDoc->CheckJuncExist(pline_1->m_cVertex[vertex_num_1-1].fx, pline_1->m_cVertex[vertex_num_1-1].fy, m_nDiagram) == FALSE)
									{
										COb_schjunc* pjunc = new COb_schjunc;
										pjunc->pDocument = pDoc;
										pjunc->m_fCx = pline_1->m_cVertex[vertex_num_1-1].fx;
										pjunc->m_fCy = pline_1->m_cVertex[vertex_num_1-1].fy;
										pjunc->m_nSize = cEDA_schautojunc.m_nSize;
										pjunc->m_nColor = cEDA_schautojunc.m_nColor;
										pjunc->m_bManual = FALSE;
										pjunc->m_nDiagram = m_nDiagram;
										pjunc->Draw(&dc, this);

										POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pjunc);
										
										CSchUndo *pundo = new CSchUndo;
										pundo->m_nOp = UNDO_DELETE;
										pundo->m_nOb = SCH_ELEMENT_JUNCTION;
										pundo->m_nDiagram = m_nDiagram;
										pundo->m_nPos = subpos;
										pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
										
									}
								}
							}
						}
					}
					else if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
					{
						COb_schsheetsymbol* psheetsymbol1 = (COb_schsheetsymbol*)pobject_1;
						for(int j1=0; j1<psheetsymbol1->m_arrayPos.GetSize(); j1++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
					}
					else if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						COb_schcomp* pcomp1 = (COb_schcomp*)pobject_1;
						for(int j1=0; j1<pcomp1->m_arrayPos.GetSize(); j1++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
					}
				}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;

				CObject* pobject_1;
				int num_1 = pDoc->m_listSchObject[m_nDiagram].GetCount();
				POSITION pos_1;
				pos_1 = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
				for(int i_1=0; i_1<num_1; i_1++)
				{	
					if(pos_1 == NULL) break;
					pobject_1 = pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
					
					if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schline )))
					{
						COb_schline* pline_1 = (COb_schline*)pobject_1;
						if((pline_1->m_nDiagram != m_nDiagram)||(pline_1->m_nFlag == FLAG_DELETED)||(pline_1->m_nAttrib != LINE_WIRE))	continue;
						int  vertex_num_1 = pline_1->m_cVertex.GetCount();
						
						for(int k=0; k<vertex_num_1-1; k++)
						{
							if( (pline_1->m_cVertex[k].fx == pline_1->m_cVertex[k+1].fx)&&(ppower->m_fX == pline_1->m_cVertex[k].fx)&&\
								(ppower->m_fY >= MIN(pline_1->m_cVertex[k].fy, pline_1->m_cVertex[k+1].fy))&&\
								(ppower->m_fY <= MAX(pline_1->m_cVertex[k].fy, pline_1->m_cVertex[k+1].fy))&&\
								((ppower->m_fX != pline_1->m_cVertex[0].fx)||(ppower->m_fY != pline_1->m_cVertex[0].fy))&&\
								((ppower->m_fX != pline_1->m_cVertex[vertex_num_1-1].fx)||(ppower->m_fY != pline_1->m_cVertex[vertex_num_1-1].fy)) )
							{
								if(pDoc->CheckJuncExist(ppower->m_fX, ppower->m_fY, m_nDiagram) == FALSE)
								{
									COb_schjunc* pjunc = new COb_schjunc;
									pjunc->pDocument = pDoc;
									pjunc->m_fCx = ppower->m_fX;
									pjunc->m_fCy = ppower->m_fY;
									pjunc->m_nSize = cEDA_schautojunc.m_nSize;
									pjunc->m_nColor = cEDA_schautojunc.m_nColor;
									pjunc->m_bManual = FALSE;
									pjunc->m_nDiagram = m_nDiagram;
									pjunc->Draw(&dc, this);

									POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pjunc);
									
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_DELETE;
									pundo->m_nOb = SCH_ELEMENT_JUNCTION;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = subpos;
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									
								}
							}
							else if( (pline_1->m_cVertex[k].fy == pline_1->m_cVertex[k+1].fy)&&(ppower->m_fY == pline_1->m_cVertex[k].fy)&&\
									 (ppower->m_fX >= MIN(pline_1->m_cVertex[k].fx, pline_1->m_cVertex[k+1].fx))&&\
									 (ppower->m_fX <= MAX(pline_1->m_cVertex[k].fx, pline_1->m_cVertex[k+1].fx))&&\
									((ppower->m_fX != pline_1->m_cVertex[0].fx)||(ppower->m_fY != pline_1->m_cVertex[0].fy))&&\
									((ppower->m_fX != pline_1->m_cVertex[vertex_num_1-1].fx)||(ppower->m_fY != pline_1->m_cVertex[vertex_num_1-1].fy)) )
							{
								if(pDoc->CheckJuncExist(ppower->m_fX, ppower->m_fY, m_nDiagram) == FALSE)
								{
									COb_schjunc* pjunc = new COb_schjunc;
									pjunc->pDocument = pDoc;
									pjunc->m_fCx = ppower->m_fX;
									pjunc->m_fCy = ppower->m_fY;
									pjunc->m_nSize = cEDA_schautojunc.m_nSize;
									pjunc->m_nColor = cEDA_schautojunc.m_nColor;
									pjunc->m_bManual = FALSE;
									pjunc->m_nDiagram = m_nDiagram;
									pjunc->Draw(&dc, this);

									POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pjunc);
									
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_DELETE;
									pundo->m_nOb = SCH_ELEMENT_JUNCTION;
									pundo->m_nDiagram = m_nDiagram;
									pundo->m_nPos = subpos;
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									
								}
							}
						}
					}
					else if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
					{
						COb_schsheetsymbol* psheetsymbol1 = (COb_schsheetsymbol*)pobject_1;
						for(int j1=0; j1<psheetsymbol1->m_arrayPos.GetSize(); j1++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
					}
					else if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						COb_schcomp* pcomp1 = (COb_schcomp*)pobject_1;
						for(int j1=0; j1<pcomp1->m_arrayPos.GetSize(); j1++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
					}
				}

		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram == m_nDiagram)&&(pcomp->m_nFlag == FLAG_NORMAL))
			{
				for(int j=0; j<pcomp->m_arrayPos.GetCount(); j++)
				{
					POSITION pinpos = pcomp->m_arrayPos.GetAt(j);
					pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pinpos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
					{
						COb_schpin* ppin = (COb_schpin*)pobject;
						if(ppin->m_bHide == TRUE) continue;

						
						if(( pcomp->m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  pcomp->m_nPart))
						{
							
							if(( pcomp->m_nDisplayModeCount <= 1)||( pcomp->m_nDisplayMode == ppin->m_nDisplayMode))
							{

								double fx, fy;
								if(ppin->m_nOrientation == 0)
								{
									fx = ppin->m_fX + ppin->m_fPinLength;
									fy = ppin->m_fY;
								}
								else if(ppin->m_nOrientation == 90)
								{
									fx = ppin->m_fX;
									fy = ppin->m_fY + ppin->m_fPinLength;
								}
								else if(ppin->m_nOrientation == 180)
								{
									fx = ppin->m_fX - ppin->m_fPinLength;
									fy = ppin->m_fY;
								}
								else if(ppin->m_nOrientation == 270)
								{
									fx = ppin->m_fX;
									fy = ppin->m_fY - ppin->m_fPinLength;
								}

								CObject* pobject_1;
								int num_1 = pDoc->m_listSchObject[m_nDiagram].GetCount();
								POSITION pos_1;
								pos_1 = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
								for(int i_1=0; i_1<num_1; i_1++)
								{	
									if(pos_1 == NULL) break;
									POSITION posbak_1 = pos_1;
									pobject_1 = pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
									
									if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schline )))
									{
										COb_schline* pline_1 = (COb_schline*)pobject_1;
										if((pline_1->m_nDiagram != m_nDiagram)||(pline_1->m_nFlag != FLAG_NORMAL)||(pline_1->m_nAttrib != LINE_WIRE))	continue;
										int  vertex_num_1 = pline_1->m_cVertex.GetCount();
										
										for(int k=0; k<vertex_num_1-1; k++)
										{
											if( (pline_1->m_cVertex[k].fx == pline_1->m_cVertex[k+1].fx)&&(fx == pline_1->m_cVertex[k].fx)&&\
												(fy >= MIN(pline_1->m_cVertex[k].fy, pline_1->m_cVertex[k+1].fy))&&\
												(fy <= MAX(pline_1->m_cVertex[k].fy, pline_1->m_cVertex[k+1].fy))&&\
												((fx != pline_1->m_cVertex[0].fx)||(fy != pline_1->m_cVertex[0].fy))&&\
												((fx != pline_1->m_cVertex[vertex_num_1-1].fx)||(fy != pline_1->m_cVertex[vertex_num_1-1].fy)) )
											{
												if(pDoc->CheckJuncExist(fx, fy, m_nDiagram) == FALSE)
												{
													COb_schjunc* pjunc = new COb_schjunc;
													pjunc->pDocument = pDoc;
													pjunc->m_fCx = fx;
													pjunc->m_fCy = fy;
													pjunc->m_nSize = cEDA_schautojunc.m_nSize;
													pjunc->m_nColor = cEDA_schautojunc.m_nColor;
													pjunc->m_bManual = FALSE;
													pjunc->m_nDiagram = m_nDiagram;
													pjunc->Draw(&dc, this);

													POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pjunc);
													
													CSchUndo *pundo = new CSchUndo;
													pundo->m_nOp = UNDO_DELETE;
													pundo->m_nOb = SCH_ELEMENT_JUNCTION;
													pundo->m_nDiagram = m_nDiagram;
													pundo->m_nPos = subpos;
													pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
													
												}
											}
											else if( (pline_1->m_cVertex[k].fy == pline_1->m_cVertex[k+1].fy)&&(fy == pline_1->m_cVertex[k].fy)&&\
													(fx >= MIN(pline_1->m_cVertex[k].fx, pline_1->m_cVertex[k+1].fx))&&\
													(fx <= MAX(pline_1->m_cVertex[k].fx, pline_1->m_cVertex[k+1].fx))&&\
													((fx != pline_1->m_cVertex[0].fx)||(fy != pline_1->m_cVertex[0].fy))&&\
													((fx != pline_1->m_cVertex[vertex_num_1-1].fx)||(fy != pline_1->m_cVertex[vertex_num_1-1].fy)) )
											{
												if(pDoc->CheckJuncExist(fx, fy, m_nDiagram) == FALSE)
												{
													COb_schjunc* pjunc = new COb_schjunc;
													pjunc->pDocument = pDoc;
													pjunc->m_fCx = fx;
													pjunc->m_fCy = fy;
													pjunc->m_nSize = cEDA_schautojunc.m_nSize;
													pjunc->m_nColor = cEDA_schautojunc.m_nColor;
													pjunc->m_bManual = FALSE;
													pjunc->m_nDiagram = m_nDiagram;
													pjunc->Draw(&dc, this);

													POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pjunc);
													
													CSchUndo *pundo = new CSchUndo;
													pundo->m_nOp = UNDO_DELETE;
													pundo->m_nOb = SCH_ELEMENT_JUNCTION;
													pundo->m_nDiagram = m_nDiagram;
													pundo->m_nPos = subpos;
													pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
													
												}
											}
										}
									}
									else if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
									{
										COb_schsheetsymbol* psheetsymbol1 = (COb_schsheetsymbol*)pobject_1;
										for(int j1=0; j1<psheetsymbol1->m_arrayPos.GetSize(); j1++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
									}
									else if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
									{
										COb_schcomp* pcomp1 = (COb_schcomp*)pobject_1;
										for(int j1=0; j1<pcomp1->m_arrayPos.GetSize(); j1++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
									}
								}
							}
						}
					}
				}
			}
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
	}


	SYSTEMTIME end_time;
	GetSystemTime(&end_time);
	

	if(((end_time.wMinute - start_time.wMinute)*60 + end_time.wSecond - start_time.wSecond) > 3) 
	{
		if(AfxMessageBox("自动结点(Auto Junction)优化时间过长, 可能会影响编辑操作的流畅性, 是否关闭该功能?", MB_YESNO|MB_ICONWARNING) == IDYES) bEDA_AutoJunction = FALSE;
	}
}

void CESDView::AutoRemoveJunction()
{
	

	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	CObject* pobject;
	int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
	POSITION pos = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{	
		if(pos == NULL) break;
		POSITION posbak = pos;
		pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
	
		int flag1 = 0, flag2 = 0;		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED)||(pjunc->m_bManual == TRUE))	continue;

				CObject* pobject_1;
				int num_1 = pDoc->m_listSchObject[m_nDiagram].GetCount();
				POSITION pos_1;
				pos_1 = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
				for(int i_1=0; i_1<num_1; i_1++)
				{	
					if(pos_1 == NULL) break;
					POSITION posbak_1 = pos_1;
					pobject_1 = pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
					if((flag1 > 0)&&(flag2 > 0)) break;
					
					if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schline )))
					{
						COb_schline* pline_1 = (COb_schline*)pobject_1;
						if((pline_1->m_nDiagram != m_nDiagram)||(pline_1->m_nFlag == FLAG_DELETED)||(pline_1->m_nAttrib != LINE_WIRE))	continue;
						int  vertex_num_1 = pline_1->m_cVertex.GetCount();

						if(flag1 <= 0)
						{
							if( ((pjunc->m_fCx == pline_1->m_cVertex[0].fx)&&(pjunc->m_fCy == pline_1->m_cVertex[0].fy))||\
							    ((pjunc->m_fCx == pline_1->m_cVertex[vertex_num_1-1].fx)&&(pjunc->m_fCy == pline_1->m_cVertex[vertex_num_1-1].fy)) ) 
							{
								flag1++;
								continue;
							}
						}
						if(flag2 <=0)
						{
							for(int k=0; k<vertex_num_1-1; k++)
							{
								if(( (pline_1->m_cVertex[k].fx == pline_1->m_cVertex[k+1].fx)&&(pjunc->m_fCx == pline_1->m_cVertex[k].fx)&&\
									 (pjunc->m_fCy >= MIN(pline_1->m_cVertex[k].fy, pline_1->m_cVertex[k+1].fy))&&\
									 (pjunc->m_fCy <= MAX(pline_1->m_cVertex[k].fy, pline_1->m_cVertex[k+1].fy)) ) ||\
								   ( (pline_1->m_cVertex[k].fy == pline_1->m_cVertex[k+1].fy)&&(pjunc->m_fCy == pline_1->m_cVertex[k].fy)&&\
									 (pjunc->m_fCx >= MIN(pline_1->m_cVertex[k].fx, pline_1->m_cVertex[k+1].fx))&&\
									 (pjunc->m_fCx <= MAX(pline_1->m_cVertex[k].fx, pline_1->m_cVertex[k+1].fx)) ) ) 
								{
									flag2++;
									break;
								}
							}
						}
					}
					else if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schpower )))
					{
						COb_schpower* ppower = (COb_schpower*)pobject_1;
						if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
						if(flag1 <= 0)
						{
							if((pjunc->m_fCx == ppower->m_fX)&&(pjunc->m_fCy == ppower->m_fY))
							{
								flag1++;
								continue;
							}
						}
					}
					else if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
					{
						COb_schsheetsymbol* psheetsymbol1;
						psheetsymbol1 = (COb_schsheetsymbol*)pobject_1;
						for(int j1=0; j1<psheetsymbol1->m_arrayPos.GetSize(); j1++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
					}
					else if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						COb_schcomp* pcomp1 = (COb_schcomp*)pobject_1;
						if((pcomp1->m_nDiagram == m_nDiagram)&&(pcomp1->m_nFlag == FLAG_NORMAL))
						{
							for(int j1=0; j1<pcomp1->m_arrayPos.GetCount(); j1++)
							{
								POSITION pinpos = pcomp1->m_arrayPos.GetAt(j1);
								pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pinpos);
								if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
								{
									COb_schpin* ppin = (COb_schpin*)pobject;
									if(ppin->m_nFlag != FLAG_NORMAL) continue;
									double fx, fy;
									if(ppin->m_nOrientation == 0)
									{
										fx = ppin->m_fX + ppin->m_fPinLength;
										fy = ppin->m_fY;
									}
									else if(ppin->m_nOrientation == 90)
									{
										fx = ppin->m_fX;
										fy = ppin->m_fY + ppin->m_fPinLength;
									}
									else if(ppin->m_nOrientation == 180)
									{
										fx = ppin->m_fX - ppin->m_fPinLength;
										fy = ppin->m_fY;
									}
									else if(ppin->m_nOrientation == 270)
									{
										fx = ppin->m_fX;
										fy = ppin->m_fY - ppin->m_fPinLength;
									}
									
									if(flag1 <= 0)
									{
										if((pjunc->m_fCx == fx)&&(pjunc->m_fCy == fy))
										{
											flag1++;
											continue;
										}
									}
								}
							}
	
						}

	
						for(int j1=0; j1<pcomp1->m_arrayPos.GetSize(); j1++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos_1);
					}
				}

				if((flag1 <= 0)||(flag2<=0)) 
				{
					pjunc->m_bSelection = FALSE;
					pjunc->m_nFlag = FLAG_DELETED;

					
					CSchUndo *pundo = new CSchUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = SCH_ELEMENT_JUNCTION;
					pundo->m_nDiagram = m_nDiagram;
					pundo->m_nPos = posbak;
					pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
					

					CClientDC dc(this);
					pjunc->Draw(&dc, this);
				}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;
			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
	}
}

