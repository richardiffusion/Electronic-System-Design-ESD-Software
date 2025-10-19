void CESDView::OnPcbToolUnrouteAll()
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	
	if(AfxMessageBox("信号层所有布线将被删除，确认?", MB_YESNO|MB_ICONQUESTION) != IDYES)	return;
	pDoc->SetModifiedFlag(TRUE);

	
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nLayer >BOTTOMLAYER)||(ptrack->m_nComponent >= 0)) continue;

		ptrack->m_bSelection = FALSE;
		ptrack->m_nFlag = FLAG_DELETED;
		
		if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = PCB_ELEMENT_TRACK;
		pundo->m_nIndex = i;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	
	
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nLayer >BOTTOMLAYER)||(parc->m_nComponent >= 0)) continue;

		parc->m_bSelection = FALSE;
		parc->m_nFlag = FLAG_DELETED;
		
		if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = PCB_ELEMENT_ARC;
		pundo->m_nIndex = i;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	
	
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nLayer >BOTTOMLAYER)||(pfill->m_nComponent >= 0)) continue;

		pfill->m_bSelection = FALSE;
		pfill->m_nFlag = FLAG_DELETED;
		
		if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = PCB_ELEMENT_FILL;
		pundo->m_nIndex = i;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}	
	
	
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nComponent >= 0)) continue;

		pvia->m_bSelection = FALSE;
		pvia->m_nFlag = FLAG_DELETED;
		
		if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = PCB_ELEMENT_VIA;
		pundo->m_nIndex = i;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}

	
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent >= 0)) continue;

		ppad->m_bSelection = FALSE;
		ppad->m_nFlag = FLAG_DELETED;
		
		if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = PCB_ELEMENT_PAD;
		pundo->m_nIndex = i;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	
	
	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nLayer > BOTTOMLAYER)||(pregion->m_nComponent >= 0)) continue;

		pregion->m_bSelection = FALSE;
		pregion->m_nFlag = FLAG_DELETED;
		
		if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = PCB_ELEMENT_REGION;
		pundo->m_nIndex = i;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}

	
	for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
		if((ppolygon->m_nFlag != FLAG_NORMAL)||(ppolygon->m_nLayer > BOTTOMLAYER)) continue;

		ppolygon->m_bSelection = FALSE;
		ppolygon->m_nFlag = FLAG_DELETED;
		
		if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = PCB_ELEMENT_POLYGON;
		pundo->m_nIndex = i;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}	
	
	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		
		
		Refresh_Ratsnest();
	}
	
	Invalidate();
}

void CESDView::OnPcbToolUnrouteComponent()
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();


	unsigned ch1;
	char szMoveText[] = "选择删除元件连接: <左键><回车>选择元件, <右键><ESC>结束命令";
	MoveCursorToCenter();

	Write_Help(szMoveText);
	do{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			Struct_Pcb_Multi_Selection selected_element = SelectPcbObject(m_fCurrentX, m_fCurrentY, NOT_INCLUDE_CPAD);

			
			DrawPcbHighlight(&dc, 0);
			m_arrayPcbHighlight.RemoveAll();
			if(selected_element.pcbelement == PCB_ELEMENT_COMP)
			{
				COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(selected_element.index);
				if(pcomp->m_nFlag != FLAG_NORMAL) continue;

				for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
					if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == selected_element.index))
					{
						Struct_Pcb_Multi_Selection element;
						element.pcbelement = PCB_ELEMENT_PAD;
						element.index = i;
						ppad->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(element);
					}
				}

				PCBMakeHighlight(TRUE);
				PcbProcessingSelection(FALSE);
			}

			CArray<int, int> arrayPolygon;
			arrayPolygon.RemoveAll();
			for(int i=0; i<m_arrayPcbHighlight.GetCount(); i++)
			{
				Struct_Pcb_Multi_Selection selected_element = m_arrayPcbHighlight.GetAt(i);
				if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
					if(ptrack->m_nComponent >= 0) continue;
					
					
					if(ptrack->m_nPolygon >= 0)
					{
						int j;
						for(j=0; j<arrayPolygon.GetCount(); j++)
						{
							int iPolygon = arrayPolygon.GetAt(j);
							if(iPolygon == ptrack->m_nPolygon) break;
						}
						if(j >= arrayPolygon.GetCount()) arrayPolygon.Add(ptrack->m_nPolygon);
					}

					ptrack->m_bSelection = FALSE;
					ptrack->m_nFlag = FLAG_DELETED;
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = selected_element.index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					
				}
				else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
					if(parc->m_nComponent >= 0) continue;
					
					
					if(parc->m_nPolygon >= 0)
					{
						int j;
						for(j=0; j<arrayPolygon.GetCount(); j++)
						{
							int iPolygon = arrayPolygon.GetAt(j);
							if(iPolygon == parc->m_nPolygon) break;
						}
						if(j >= arrayPolygon.GetCount()) arrayPolygon.Add(parc->m_nPolygon);
					}

					parc->m_bSelection = FALSE;
					parc->m_nFlag = FLAG_DELETED;
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = selected_element.index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					
				}
				else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
				{
					COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
					if(pfill->m_nComponent >= 0) continue;
					
					pfill->m_bSelection = FALSE;
					pfill->m_nFlag = FLAG_DELETED;
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = selected_element.index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					
				}	
				else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
					if(ppad->m_nComponent >= 0) continue;

					ppad->m_bSelection = FALSE;
					ppad->m_nFlag = FLAG_DELETED;
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_PAD;
					pundo->m_nIndex = selected_element.index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					
				}
				else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
				{
					COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
					if(pvia->m_nComponent >= 0) continue;

					pvia->m_bSelection = FALSE;
					pvia->m_nFlag = FLAG_DELETED;
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = selected_element.index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					
				}		

				else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
				{
					COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
					if(pregion->m_nComponent >= 0) continue;
					
					
					if(pregion->m_nPolygon >= 0)
					{
						int j;
						for(j=0; j<arrayPolygon.GetCount(); j++)
						{
							int iPolygon = arrayPolygon.GetAt(j);
							if(iPolygon == pregion->m_nPolygon) break;
						}
						if(j >= arrayPolygon.GetCount()) arrayPolygon.Add(pregion->m_nPolygon);
					}
					
					pregion->m_bSelection = FALSE;
					pregion->m_nFlag = FLAG_DELETED;
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_REGION;
					pundo->m_nIndex = selected_element.index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					
				}
			}

			
			for(int i=0; i<arrayPolygon.GetCount(); i++)
			{
				int iPolygon = arrayPolygon.GetAt(i);
				COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(iPolygon);

				ppolygon->m_bSelection = FALSE;
				ppolygon->m_nFlag = FLAG_DELETED;
				
				if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = PCB_ELEMENT_POLYGON;
				pundo->m_nIndex = iPolygon;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
					
				
				for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
					if((parc->m_nPolygon == iPolygon)&&(parc->m_nFlag == FLAG_NORMAL))
					{
						parc->m_bSelection = FALSE;
						parc->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_ARC;
						pundo->m_nIndex = i;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}
				for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
					if((ptrack->m_nPolygon == iPolygon)&&(ptrack->m_nFlag == FLAG_NORMAL))
					{
						ptrack->m_bSelection = FALSE;
						ptrack->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = i;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}
				for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
				{
					COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
					if((pregion->m_nPolygon == iPolygon)&&(pregion->m_nFlag == FLAG_NORMAL))
					{
						pregion->m_bSelection = FALSE;
						pregion->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_REGION;
						pundo->m_nIndex = i;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}			
			}

			m_arrayPcbHighlight.RemoveAll();
			Invalidate();

			break;
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
			
	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

		
		
	}
	
	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnPcbToolUnrouteConnection()
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();


	unsigned ch1;
	char szMoveText[] = "选择删除连接: <左键><回车>选择连接, <右键><ESC>结束命令";
	MoveCursorToCenter();

	Write_Help(szMoveText);
	do{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			Struct_Pcb_Multi_Selection selected_element = SelectPcbObject(m_fCurrentX, m_fCurrentY, PCB_HIGHLIGHT_SELECTION);

			
			DrawPcbHighlight(&dc, 0);
			m_arrayPcbHighlight.RemoveAll();
			if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
				if(pDoc->IsOnSignalLayer(ptrack->m_nLayer) == TRUE)
				{
					ptrack->m_nFlag = FLAG_PROCESSING;
					m_arrayPcbHighlight.Add(selected_element);
					PCBMakeHighlight(TRUE);
					PcbProcessingSelection(FALSE);
				}
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
				if(pDoc->IsOnSignalLayer(parc->m_nLayer) == TRUE)
				{
					parc->m_nFlag = FLAG_PROCESSING;
					m_arrayPcbHighlight.Add(selected_element);
					PCBMakeHighlight(TRUE);
					PcbProcessingSelection(FALSE);
				}			
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
				if(pDoc->IsOnSignalLayer(ppad->m_nLayer) == TRUE)
				{
					ppad->m_nFlag = FLAG_PROCESSING;
					m_arrayPcbHighlight.Add(selected_element);
					PCBMakeHighlight(TRUE);
					PcbProcessingSelection(FALSE);
				}				
			}	
			else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
				pvia->m_nFlag = FLAG_PROCESSING;
				m_arrayPcbHighlight.Add(selected_element);
				PCBMakeHighlight(TRUE);
				PcbProcessingSelection(FALSE);
			}		
			else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
				if(pDoc->IsOnSignalLayer(pfill->m_nLayer) == TRUE)
				{
					pfill->m_nFlag = FLAG_PROCESSING;
					m_arrayPcbHighlight.Add(selected_element);
					PCBMakeHighlight(TRUE);
					PcbProcessingSelection(FALSE);
				}
			}	
			else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
			{
				COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
				if(pDoc->IsOnSignalLayer(pregion->m_nLayer) == TRUE)
				{
					pregion->m_nFlag = FLAG_PROCESSING;
					m_arrayPcbHighlight.Add(selected_element);
					PCBMakeHighlight(TRUE);
					PcbProcessingSelection(FALSE);
				}				
			}	
			/*else if(selected_element.pcbelement == PCB_ELEMENT_POLYGON)
			{
				COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(selected_element.index);
				if(pDoc->IsOnSignalLayer(ppolygon->m_nLayer) == TRUE)
				{
					m_arrayPcbHighlight.Add(selected_element);
					PCBMakeHighlight(TRUE);
				}				
			}*/

			CArray<int, int> arrayPolygon;
			arrayPolygon.RemoveAll();
			for(int i=0; i<m_arrayPcbHighlight.GetCount(); i++)
			{
				Struct_Pcb_Multi_Selection selected_element = m_arrayPcbHighlight.GetAt(i);
				if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
					if(ptrack->m_nComponent >= 0) continue;
					
					
					if(ptrack->m_nPolygon >= 0)
					{
						int j;
						for(j=0; j<arrayPolygon.GetCount(); j++)
						{
							int iPolygon = arrayPolygon.GetAt(j);
							if(iPolygon == ptrack->m_nPolygon) break;
						}
						if(j >= arrayPolygon.GetCount()) arrayPolygon.Add(ptrack->m_nPolygon);
					}

					ptrack->m_bSelection = FALSE;
					ptrack->m_nFlag = FLAG_DELETED;
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = selected_element.index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					
				}
				else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
					if(parc->m_nComponent >= 0) continue;
					
					
					if(parc->m_nPolygon >= 0)
					{
						int j;
						for(j=0; j<arrayPolygon.GetCount(); j++)
						{
							int iPolygon = arrayPolygon.GetAt(j);
							if(iPolygon == parc->m_nPolygon) break;
						}
						if(j >= arrayPolygon.GetCount()) arrayPolygon.Add(parc->m_nPolygon);
					}

					parc->m_bSelection = FALSE;
					parc->m_nFlag = FLAG_DELETED;
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = selected_element.index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					
				}
				else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
				{
					COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
					if(pfill->m_nComponent >= 0) continue;
					
					pfill->m_bSelection = FALSE;
					pfill->m_nFlag = FLAG_DELETED;
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = selected_element.index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					
				}	
				else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
					if(ppad->m_nComponent >= 0) continue;

					ppad->m_bSelection = FALSE;
					ppad->m_nFlag = FLAG_DELETED;
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_PAD;
					pundo->m_nIndex = selected_element.index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					
				}
				else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
				{
					COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
					if(pvia->m_nComponent >= 0) continue;

					pvia->m_bSelection = FALSE;
					pvia->m_nFlag = FLAG_DELETED;
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = selected_element.index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					
				}		

				else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
				{
					COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
					if(pregion->m_nComponent >= 0) continue;
					
					
					if(pregion->m_nPolygon >= 0)
					{
						int j;
						for(j=0; j<arrayPolygon.GetCount(); j++)
						{
							int iPolygon = arrayPolygon.GetAt(j);
							if(iPolygon == pregion->m_nPolygon) break;
						}
						if(j >= arrayPolygon.GetCount()) arrayPolygon.Add(pregion->m_nPolygon);
					}
					
					pregion->m_bSelection = FALSE;
					pregion->m_nFlag = FLAG_DELETED;
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_REGION;
					pundo->m_nIndex = selected_element.index;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					
				}
			}

			
			for(int i=0; i<arrayPolygon.GetCount(); i++)
			{
				int iPolygon = arrayPolygon.GetAt(i);
				COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(iPolygon);

				ppolygon->m_bSelection = FALSE;
				ppolygon->m_nFlag = FLAG_DELETED;
				
				if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = PCB_ELEMENT_POLYGON;
				pundo->m_nIndex = iPolygon;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
					
				
				for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
					if((parc->m_nPolygon == iPolygon)&&(parc->m_nFlag == FLAG_NORMAL))
					{
						parc->m_bSelection = FALSE;
						parc->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_ARC;
						pundo->m_nIndex = i;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}
				for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
					if((ptrack->m_nPolygon == iPolygon)&&(ptrack->m_nFlag == FLAG_NORMAL))
					{
						ptrack->m_bSelection = FALSE;
						ptrack->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = i;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}
				for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
				{
					COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
					if((pregion->m_nPolygon == iPolygon)&&(pregion->m_nFlag == FLAG_NORMAL))
					{
						pregion->m_bSelection = FALSE;
						pregion->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_REGION;
						pundo->m_nIndex = i;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}			
			}

			m_arrayPcbHighlight.RemoveAll();
			Invalidate();

		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
			
	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	
	}
	
	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::OnPcbToolUnrouteNet()
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(pDoc->m_arrayPcbNet.GetCount() <= 0) 
	{
		AfxMessageBox("未找到PCB网络信息, 命令终止.");
		return;
	}

	CDlg_PcbNetName dlg;

	for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;

		dlg.arrayString.Add(pnet->m_csNetName);
	}

	if(dlg.DoModal() == IDOK)
	{
		for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
		{
			COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(i);
			if(pnet->m_nFlag != FLAG_NORMAL) continue;

			int iNet = i;
			if(dlg.m_csComboNet == pnet->m_csNetName)
			{
				
				DrawPcbHighlight(&dc, 0);
				m_arrayPcbHighlight.RemoveAll();

				Struct_Pcb_Multi_Selection selected_element;
				for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
					if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nNet != iNet)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;

					if(pDoc->IsOnSignalLayer(ptrack->m_nLayer) == TRUE)
					{
						selected_element.pcbelement = PCB_ELEMENT_TRACK;
						selected_element.index = j;
						ptrack->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(selected_element);
					}
				}
				for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
					if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nNet != iNet)||(parc->m_nComponent >= 0)||(parc->m_nPolygon >= 0)) continue;
					
					if(pDoc->IsOnSignalLayer(parc->m_nLayer) == TRUE)
					{
						selected_element.pcbelement = PCB_ELEMENT_ARC;
						selected_element.index = j;
						parc->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(selected_element);
					}
				}
				for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
				{
					COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
					if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nNet != iNet)||(pfill->m_nComponent >= 0)) continue;

					if(pDoc->IsOnSignalLayer(pfill->m_nLayer) == TRUE)
					{
						selected_element.pcbelement = PCB_ELEMENT_FILL;
						selected_element.index = j;
						pfill->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(selected_element);
					}					
				}
				for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
					if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nNet != iNet)) continue;
					if(pDoc->IsOnSignalLayer(ppad->m_nLayer) == TRUE)
					{
						selected_element.pcbelement = PCB_ELEMENT_PAD;
						selected_element.index = j;
						ppad->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(selected_element);
					}					
				}
				for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
				{
					COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
					if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nNet != iNet)||(pvia->m_nComponent >= 0)) continue;
						
					selected_element.pcbelement = PCB_ELEMENT_VIA;
					selected_element.index = j;					
					pvia->m_nFlag = FLAG_PROCESSING;
					m_arrayPcbHighlight.Add(selected_element);
				}
				for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
				{
					COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
					if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nNet != iNet)||(pregion->m_nComponent >= 0)||(pregion->m_nPolygon >= 0)) continue;
					if(pDoc->IsOnSignalLayer(pregion->m_nLayer) == TRUE)
					{
						selected_element.pcbelement = PCB_ELEMENT_REGION;
						selected_element.index = j;		
						pregion->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(selected_element);
					}					
				}
				PCBMakeHighlight(TRUE);
				PcbProcessingSelection(FALSE);

				
				CArray<int, int> arrayPolygon;
				arrayPolygon.RemoveAll();
				for(int i=0; i<m_arrayPcbHighlight.GetCount(); i++)
				{
					Struct_Pcb_Multi_Selection selected_element = m_arrayPcbHighlight.GetAt(i);
					if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
					{
						COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
						if(ptrack->m_nComponent >= 0) continue;
						
						
						if(ptrack->m_nPolygon >= 0)
						{
							int j;
							for(j=0; j<arrayPolygon.GetCount(); j++)
							{
								int iPolygon = arrayPolygon.GetAt(j);
								if(iPolygon == ptrack->m_nPolygon) break;
							}
							if(j >= arrayPolygon.GetCount()) arrayPolygon.Add(ptrack->m_nPolygon);
						}

						ptrack->m_bSelection = FALSE;
						ptrack->m_nFlag = FLAG_DELETED;
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = selected_element.index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					}
					else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
					{
						COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
						if(parc->m_nComponent >= 0) continue;
						
						
						if(parc->m_nPolygon >= 0)
						{
							int j;
							for(j=0; j<arrayPolygon.GetCount(); j++)
							{
								int iPolygon = arrayPolygon.GetAt(j);
								if(iPolygon == parc->m_nPolygon) break;
							}
							if(j >= arrayPolygon.GetCount()) arrayPolygon.Add(parc->m_nPolygon);
						}

						parc->m_bSelection = FALSE;
						parc->m_nFlag = FLAG_DELETED;
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_ARC;
						pundo->m_nIndex = selected_element.index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					}
					else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
					{
						COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
						if(pfill->m_nComponent >= 0) continue;
						
						pfill->m_bSelection = FALSE;
						pfill->m_nFlag = FLAG_DELETED;
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_FILL;
						pundo->m_nIndex = selected_element.index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					}	
					else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
					{
						COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
						if(ppad->m_nComponent >= 0) continue;

						ppad->m_bSelection = FALSE;
						ppad->m_nFlag = FLAG_DELETED;
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_PAD;
						pundo->m_nIndex = selected_element.index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					}
					else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
					{
						COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
						if(pvia->m_nComponent >= 0) continue;

						pvia->m_bSelection = FALSE;
						pvia->m_nFlag = FLAG_DELETED;
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_VIA;
						pundo->m_nIndex = selected_element.index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					}		

					else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
					{
						COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
						if(pregion->m_nComponent >= 0) continue;
						
						
						if(pregion->m_nPolygon >= 0)
						{
							int j;
							for(j=0; j<arrayPolygon.GetCount(); j++)
							{
								int iPolygon = arrayPolygon.GetAt(j);
								if(iPolygon == pregion->m_nPolygon) break;
							}
							if(j >= arrayPolygon.GetCount()) arrayPolygon.Add(pregion->m_nPolygon);
						}
						
						pregion->m_bSelection = FALSE;
						pregion->m_nFlag = FLAG_DELETED;
						
						if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_REGION;
						pundo->m_nIndex = selected_element.index;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					}
				}

				
				for(int i=0; i<arrayPolygon.GetCount(); i++)
				{
					int iPolygon = arrayPolygon.GetAt(i);
					COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(iPolygon);

					ppolygon->m_bSelection = FALSE;
					ppolygon->m_nFlag = FLAG_DELETED;
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_POLYGON;
					pundo->m_nIndex = iPolygon;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					
						
					
					for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
					{
						COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
						if((parc->m_nPolygon == iPolygon)&&(parc->m_nFlag == FLAG_NORMAL))
						{
							parc->m_bSelection = FALSE;
							parc->m_nFlag = FLAG_DELETED;
							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_UNDELETE;
							pundo->m_nOb = PCB_ELEMENT_ARC;
							pundo->m_nIndex = i;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							
						}
					}
					for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
					{
						COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
						if((ptrack->m_nPolygon == iPolygon)&&(ptrack->m_nFlag == FLAG_NORMAL))
						{
							ptrack->m_bSelection = FALSE;
							ptrack->m_nFlag = FLAG_DELETED;
							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_UNDELETE;
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = i;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							
						}
					}
					for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
					{
						COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
						if((pregion->m_nPolygon == iPolygon)&&(pregion->m_nFlag == FLAG_NORMAL))
						{
							pregion->m_bSelection = FALSE;
							pregion->m_nFlag = FLAG_DELETED;
							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_UNDELETE;
							pundo->m_nOb = PCB_ELEMENT_REGION;
							pundo->m_nIndex = i;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							
						}
					}			
				}

				m_arrayPcbHighlight.RemoveAll();
				Invalidate();
				
				
				if(undo_flag == TRUE)
				{
					pDoc->m_nPCB_Undo_Num++;
					if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

				}
				
				break;
			}
		}
	}
}