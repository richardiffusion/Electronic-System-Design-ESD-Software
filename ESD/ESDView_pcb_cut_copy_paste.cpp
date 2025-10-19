void CESDView::OnPcbEditCut()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	if(nEDA_System_State == PCB)
	{

		while (!arrayEDA_PcbClipBoardArc.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardArc.GetAt(0);
			arrayEDA_PcbClipBoardArc.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardPad.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardPad.GetAt(0);
			arrayEDA_PcbClipBoardPad.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardVia.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardVia.GetAt(0);
			arrayEDA_PcbClipBoardVia.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardTrack.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardTrack.GetAt(0);
			arrayEDA_PcbClipBoardTrack.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardText.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardText.GetAt(0);
			arrayEDA_PcbClipBoardText.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardFill.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardFill.GetAt(0);
			arrayEDA_PcbClipBoardFill.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardRegion.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardRegion.GetAt(0);
			arrayEDA_PcbClipBoardRegion.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardCoordinate.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardCoordinate.GetAt(0);
			arrayEDA_PcbClipBoardCoordinate.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardDimension.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardDimension.GetAt(0);
			arrayEDA_PcbClipBoardDimension.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardComp.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardComp.GetAt(0);
			arrayEDA_PcbClipBoardComp.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardPolygon.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardPolygon.GetAt(0);
			arrayEDA_PcbClipBoardPolygon.RemoveAt(0);
		}

		BOOL clear_undo_buffer_flag = FALSE;	
		BOOL undo_flag = FALSE;  

		for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
			if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_bSelection != TRUE)||(parc->m_nComponent >= 0)||(parc->m_nPolygon >= 0)) continue;

			
			COb_pcbarc* pnewarc = new COb_pcbarc();
			pnewarc->Copy(parc);
			pnewarc->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardArc.Add(pnewarc);

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
		for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
			if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_bSelection != TRUE)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;

			
			COb_pcbtrack* pnewtrack = new COb_pcbtrack();
			pnewtrack->Copy(ptrack);
			pnewtrack->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardTrack.Add(pnewtrack);

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
		for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
			if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_bSelection != TRUE)||(pfill->m_nComponent >= 0)) continue;

			
			COb_pcbfill* pnewfill = new COb_pcbfill();
			pnewfill->Copy(pfill);
			pnewfill->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardFill.Add(pnewfill);

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
		for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
			if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_bSelection != TRUE)||(ppad->m_nComponent >= 0)) continue;

			
			COb_pcbpad* pnewpad = new COb_pcbpad();
			pnewpad->Copy(ppad);
			pnewpad->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardPad.Add(pnewpad);

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
		for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
			if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_bSelection != TRUE)||(pvia->m_nComponent >= 0)) continue;

			
			COb_pcbvia* pnewvia = new COb_pcbvia();
			pnewvia->Copy(pvia);
			pnewvia->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardVia.Add(pnewvia);

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
		for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
			if((ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_bSelection != TRUE)||(ptext->m_nComponent >= 0)) continue;

			
			COb_pcbtext* pnewtext = new COb_pcbtext();
			pnewtext->Copy(ptext);
			pnewtext->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardText.Add(pnewtext);

			ptext->m_bSelection = FALSE;
			ptext->m_nFlag = FLAG_DELETED;
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = PCB_ELEMENT_TEXT;
			pundo->m_nIndex = i;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
		}
		for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_bSelection != TRUE)||(pregion->m_nComponent >= 0)||(pregion->m_nPolygon >= 0)) continue;

			
			COb_pcbregion* pnewregion = new COb_pcbregion();
			pnewregion->Copy(pregion);
			pnewregion->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardRegion.Add(pnewregion);

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
		for(int i=0; i<pDoc->m_arrayPcbCoordinate.GetCount(); i++)
		{
			COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(i);
			if((pcoordinate->m_nFlag != FLAG_NORMAL)||(pcoordinate->m_bSelection != TRUE)) continue;

			
			COb_pcbcoordinate* pnewcoordinate = new COb_pcbcoordinate();
			pnewcoordinate->Copy(pcoordinate);
			pnewcoordinate->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardCoordinate.Add(pnewcoordinate);

			pcoordinate->m_bSelection = FALSE;
			pcoordinate->m_nFlag = FLAG_DELETED;
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = PCB_ELEMENT_COORDINATE;
			pundo->m_nIndex = i;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
		}
		for(int i=0; i<pDoc->m_arrayPcbDimension.GetCount(); i++)
		{
			COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(i);
			if((pdimension->m_nFlag != FLAG_NORMAL)||(pdimension->m_bSelection != TRUE)) continue;

			
			COb_pcbdimension* pnewdimension = new COb_pcbdimension();
			pnewdimension->Copy(pdimension);
			pnewdimension->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardDimension.Add(pnewdimension);

			pdimension->m_bSelection = FALSE;
			pdimension->m_nFlag = FLAG_DELETED;
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = PCB_ELEMENT_DIMENSION;
			pundo->m_nIndex = i;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
		}
		for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
		{
			COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
			if((pcomp->m_nFlag != FLAG_NORMAL)||(pcomp->m_bSelection != TRUE)) continue;

			
			COb_pcbcomp* pnewcomp = new COb_pcbcomp();
			pnewcomp->Copy(pcomp);
			pnewcomp->m_bSelection = FALSE;
			int compindex = arrayEDA_PcbClipBoardComp.Add(pnewcomp);

			pcomp->m_bSelection = FALSE;
			pcomp->m_nFlag = FLAG_DELETED;
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = PCB_ELEMENT_COMP;
			pundo->m_nIndex = i;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			

			for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
				if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nComponent != i)) continue;
				
				
				COb_pcbarc* pnewarc = new COb_pcbarc();
				pnewarc->Copy(parc);
				pnewarc->m_bSelection = FALSE;
				pnewarc->m_nComponent = compindex;
				arrayEDA_PcbClipBoardArc.Add(pnewarc);

				parc->m_bSelection = FALSE;
				parc->m_nFlag = FLAG_DELETED;
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = PCB_ELEMENT_ARC;
				pundo->m_nIndex = j;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
			for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
				if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nComponent != i)) continue;
				
				
				COb_pcbtrack* pnewtrack = new COb_pcbtrack();
				pnewtrack->Copy(ptrack);
				pnewtrack->m_bSelection = FALSE;
				pnewtrack->m_nComponent = compindex;
				arrayEDA_PcbClipBoardTrack.Add(pnewtrack);

				ptrack->m_bSelection = FALSE;
				ptrack->m_nFlag = FLAG_DELETED;
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = PCB_ELEMENT_TRACK;
				pundo->m_nIndex = j;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
			for(int j=0; j<pDoc->m_arrayPcbText.GetCount(); j++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(j);
				if((ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_nComponent != i)) continue;
				
				
				COb_pcbtext* pnewtext = new COb_pcbtext();
				pnewtext->Copy(ptext);
				pnewtext->m_bSelection = FALSE;
				pnewtext->m_nComponent = compindex;
				arrayEDA_PcbClipBoardText.Add(pnewtext);

				ptext->m_bSelection = FALSE;
				ptext->m_nFlag = FLAG_DELETED;
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = PCB_ELEMENT_TEXT;
				pundo->m_nIndex = j;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
			for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
				if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent != i)) continue;
				
				
				COb_pcbfill* pnewfill = new COb_pcbfill();
				pnewfill->Copy(pfill);
				pnewfill->m_bSelection = FALSE;
				pnewfill->m_nComponent = compindex;
				arrayEDA_PcbClipBoardFill.Add(pnewfill);

				pfill->m_bSelection = FALSE;
				pfill->m_nFlag = FLAG_DELETED;
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = PCB_ELEMENT_FILL;
				pundo->m_nIndex = j;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
			for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
				if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent != i)) continue;
				
				
				COb_pcbpad* pnewpad = new COb_pcbpad();
				pnewpad->Copy(ppad);
				pnewpad->m_bSelection = FALSE;
				pnewpad->m_nComponent = compindex;
				arrayEDA_PcbClipBoardPad.Add(pnewpad);

				ppad->m_bSelection = FALSE;
				ppad->m_nFlag = FLAG_DELETED;
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = PCB_ELEMENT_PAD;
				pundo->m_nIndex = j;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
			for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
				if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nComponent != i)) continue;
				
				
				COb_pcbvia* pnewvia = new COb_pcbvia();
				pnewvia->Copy(pvia);
				pnewvia->m_bSelection = FALSE;
				pnewvia->m_nComponent = compindex;
				arrayEDA_PcbClipBoardVia.Add(pnewvia);

				pvia->m_bSelection = FALSE;
				pvia->m_nFlag = FLAG_DELETED;
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = PCB_ELEMENT_VIA;
				pundo->m_nIndex = j;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
			if((ppolygon->m_nFlag != FLAG_NORMAL)||(ppolygon->m_bSelection != TRUE)) continue;

			
			COb_pcbpolygon* pnewpolygon = new COb_pcbpolygon();
			pnewpolygon->Copy(ppolygon);
			pnewpolygon->m_bSelection = FALSE;
			int polygonindex = arrayEDA_PcbClipBoardPolygon.Add(pnewpolygon);

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
			

			for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
				if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon != i)) continue;
				
				
				COb_pcbarc* pnewarc = new COb_pcbarc();
				pnewarc->Copy(parc);
				pnewarc->m_bSelection = FALSE;
				pnewarc->m_nPolygon = polygonindex;
				arrayEDA_PcbClipBoardArc.Add(pnewarc);

				parc->m_bSelection = FALSE;
				parc->m_nFlag = FLAG_DELETED;
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = PCB_ELEMENT_ARC;
				pundo->m_nIndex = j;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
			for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
				if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nPolygon != i)) continue;
				
				
				COb_pcbtrack* pnewtrack = new COb_pcbtrack();
				pnewtrack->Copy(ptrack);
				pnewtrack->m_bSelection = FALSE;
				pnewtrack->m_nPolygon = polygonindex;
				arrayEDA_PcbClipBoardTrack.Add(pnewtrack);

				ptrack->m_bSelection = FALSE;
				ptrack->m_nFlag = FLAG_DELETED;
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = PCB_ELEMENT_TRACK;
				pundo->m_nIndex = j;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
			for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
			{
				COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
				if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon != i)) continue;
				
				
				COb_pcbregion* pnewregion = new COb_pcbregion();
				pnewregion->Copy(pregion);
				pnewregion->m_bSelection = FALSE;
				pnewregion->m_nComponent = polygonindex;
				arrayEDA_PcbClipBoardRegion.Add(pnewregion);

				pregion->m_bSelection = FALSE;
				pregion->m_nFlag = FLAG_DELETED;
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = PCB_ELEMENT_REGION;
				pundo->m_nIndex = j;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
		}
		
		fEDA_PcbClipBoard_X1 = pDoc->m_fPcbSelection_X1;
		fEDA_PcbClipBoard_Y1 = pDoc->m_fPcbSelection_Y1;
		fEDA_PcbClipBoard_X2 = pDoc->m_fPcbSelection_X2;
		fEDA_PcbClipBoard_Y2 = pDoc->m_fPcbSelection_Y2;
		
		
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		

		pDoc->m_arrayPcbSelection.RemoveAll();
		Invalidate();
	}
}

void CESDView::OnUpdatePcbEditCut(CCmdUI *pCmdUI)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(pDoc->m_arrayPcbSelection.GetCount() > 0) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CESDView::OnPcbEditPaste()
{
	int ch1;
	int flag;
	BOOL mirror;
	double x0, y0, x1, y1, deltax, deltay;

	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	char szMoveText[] = "粘贴: <左键><回车>确认粘贴, <空格>旋转, <X键>镜像, <右键><ESC>结束命令";
	
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);
	
	
	int compflag = 0;
	if(arrayEDA_PcbClipBoardComp.GetCount() > 0 ) compflag = 1;

	x0 = fEDA_PcbClipBoard_X1;
	y0 = fEDA_PcbClipBoard_Y1;
	m_fCurrentSnapX = x0;
	m_fCurrentSnapY = y0;

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);	

	
	COb_pcbfill afill;
	afill.pDocument = pDoc;
	afill.m_fX1 = fEDA_PcbClipBoard_X1;
	afill.m_fY1 = fEDA_PcbClipBoard_Y1;
	afill.m_fX2 = fEDA_PcbClipBoard_X2;
	afill.m_fY2 = fEDA_PcbClipBoard_Y2;

	afill.m_nLayer = TOPLAYER;
	afill.m_fRotation = 0;

	if(m_bCursorState == TRUE) CloseMouseCursor();
	afill.DrawFrame(&dc, this);
	OpenMouseCursor();

		nEDA_RedrawFlag = 0;
		Write_Help(szMoveText);
		flag = 0;
		mirror = FALSE;
		do
		{
			ch1 = MouseKey();
			if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
			{
				
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
					afill.DrawFrame(&dc, this);
					OpenMouseCursor();
				}

				x1 = m_fCurrentSnapX;  
				y1 = m_fCurrentSnapY;
				deltax = x1 - x0;
				deltay = y1 - y0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, fEDA_PcbClipBoard_X1 - x0, fEDA_PcbClipBoard_Y1 - y0, flag*90, mirror);
				afill.m_fX1 = fx + x0 + deltax;
				afill.m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, fEDA_PcbClipBoard_X2 - x0, fEDA_PcbClipBoard_Y2 - y0, flag*90, mirror);
				afill.m_fX2 = fx + x0 + deltax;
				afill.m_fY2 = fy + y0 + deltay;
	
				CloseMouseCursor();
				afill.DrawFrame(&dc, this);
				OpenMouseCursor();
			}
			else if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN)||((bEDA_MouseMoveElement == TRUE)&&(ch1 == MOUSE_LBUTTON_UP)))
			{
				CloseMouseCursor();
				afill.DrawFrame(&dc, this);
				OpenMouseCursor();				
					
				x1 = m_fCurrentSnapX;  
				y1 = m_fCurrentSnapY;
				deltax = x1 - x0;
				deltay = y1 - y0;
				double rotation = flag*90;

				for(int i=0; i<arrayEDA_PcbClipBoardArc.GetCount(); i++)
				{
					COb_pcbarc* parc = arrayEDA_PcbClipBoardArc.GetAt(i);
					if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nComponent >= 0)||(parc->m_nPolygon >= 0)) continue;

					COb_pcbarc* pnewarc = new COb_pcbarc();
					pnewarc->Copy(parc);
					pnewarc->pDocument = pDoc;
					pnewarc->m_nNet = -1;
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
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pnewarc->m_fCx - x0, pnewarc->m_fCy - y0, rotation, mirror);
					pnewarc->m_fCx = fx + x0 + deltax;
					pnewarc->m_fCy = fy + y0 + deltay;
								
					double salpha, ealpha;
					pDoc->Get_Rotate_Angle(&salpha, &ealpha, pnewarc->m_fSangle, pnewarc->m_fEangle, rotation, mirror);
					pnewarc->m_fSangle = salpha;
					pnewarc->m_fSangle = pDoc->Get_Correct_Angle(pnewarc->m_fSangle);
					pnewarc->m_fEangle = ealpha;
					pnewarc->m_fEangle = pDoc->Get_Correct_Angle(pnewarc->m_fEangle);
				}
				for(int i=0; i<arrayEDA_PcbClipBoardTrack.GetCount(); i++)
				{
					COb_pcbtrack* ptrack = arrayEDA_PcbClipBoardTrack.GetAt(i);
					if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;

					COb_pcbtrack* pnewtrack = new COb_pcbtrack();
					pnewtrack->Copy(ptrack);
					pnewtrack->pDocument = pDoc;
					pnewtrack->m_nNet = -1;
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
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pnewtrack->m_fX1 - x0, pnewtrack->m_fY1 - y0, rotation, mirror);
					pnewtrack->m_fX1 = fx + x0 + deltax;
					pnewtrack->m_fY1 = fy + y0 + deltay;
					pDoc->Get_Rotate_XY(&fx, &fy, pnewtrack->m_fX2 - x0, pnewtrack->m_fY2 - y0, rotation, mirror);
					pnewtrack->m_fX2 = fx + x0 + deltax;
					pnewtrack->m_fY2 = fy + y0 + deltay;
				}
				for(int i=0; i<arrayEDA_PcbClipBoardText.GetCount(); i++)
				{
					COb_pcbtext* ptext = arrayEDA_PcbClipBoardText.GetAt(i);
					if((ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_nComponent >= 0)) continue;

					COb_pcbtext* pnewtext = new COb_pcbtext();
					pnewtext->Copy(ptext);
					pnewtext->pDocument = pDoc;
					int newindex = pDoc->m_arrayPcbText.Add(pnewtext);
					
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
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pnewtext->m_fSx - x0, pnewtext->m_fSy - y0, rotation, mirror);
					pnewtext->m_fSx = fx + x0 + deltax;
					pnewtext->m_fSy = fy + y0 + deltay;

					pnewtext->m_fRotation += rotation;
					if(mirror == TRUE) 
					{
						if(pnewtext->m_bMirror == TRUE) pnewtext->m_bMirror = FALSE;
						else pnewtext->m_bMirror = TRUE;
						pnewtext->m_fRotation = 360 - pnewtext->m_fRotation;
					}

					pnewtext->m_fRotation = pDoc->Get_Correct_Angle(pnewtext->m_fRotation);
					pnewtext->UpdateStringBorder(this, FALSE);
				}
				for(int i=0; i<arrayEDA_PcbClipBoardFill.GetCount(); i++)
				{
					COb_pcbfill* pfill = arrayEDA_PcbClipBoardFill.GetAt(i);
					if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)) continue;

					COb_pcbfill* pnewfill = new COb_pcbfill();
					pnewfill->Copy(pfill);
					pnewfill->pDocument = pDoc;
					pnewfill->m_nNet = -1;
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
					
					double fx, fy;
					double fmx, fmy, dx, dy;
					dx = abs(pnewfill->m_fX1 - pnewfill->m_fX2);
					dy = abs(pnewfill->m_fY1 - pnewfill->m_fY2);
					fmx = (pnewfill->m_fX1 + pnewfill->m_fX2)/2;
					fmy = (pnewfill->m_fY1 + pnewfill->m_fY2)/2;
					pDoc->Get_Rotate_XY(&fx, &fy, fmx - x0, fmy - y0, rotation, mirror);
					fmx = fx + x0 + deltax;
					fmy = fy + y0 + deltay;

					pnewfill->m_fX1 = fmx - dx/2;
					pnewfill->m_fY1 = fmy - dy/2;
					pnewfill->m_fX2 = fmx + dx/2;
					pnewfill->m_fY2 = fmy + dy/2;
						
					if(mirror == TRUE)
					{
						pnewfill->m_fRotation = 0 - pnewfill->m_fRotation;
						pnewfill->m_fRotation -= rotation;
					}
					else
					{
						pnewfill->m_fRotation += rotation;
					}
					pnewfill->m_fRotation = pDoc->Get_Correct_Angle(pnewfill->m_fRotation);
				}
				for(int i=0; i<arrayEDA_PcbClipBoardRegion.GetCount(); i++)
				{
					COb_pcbregion* pregion = arrayEDA_PcbClipBoardRegion.GetAt(i);
					if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nComponent >= 0)||(pregion->m_nPolygon >= 0)) continue;

					COb_pcbregion* pnewregion = new COb_pcbregion();
					pnewregion->Copy(pregion);
					pnewregion->pDocument = pDoc;
					pnewregion->m_nNet = -1;
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
					

					for(int j=0; j<pnewregion->m_cVertex.GetCount(); j++)
					{
						Struct_RegionVertex vertex = pnewregion->m_cVertex.GetAt(j);
						
						double fx, fy;
						pDoc->Get_Rotate_XY(&fx, &fy, vertex.fx - x0, vertex.fy - y0, rotation, mirror);
						vertex.fx = fx + x0 + deltax;
						vertex.fy = fy + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx, &fy, vertex.cx - x0, vertex.cy - y0, rotation, mirror);
						vertex.cx = fx + x0 + deltax;
						vertex.cy = fy + y0 + deltay;
					
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, vertex.sangle, vertex.eangle, rotation, mirror);
						vertex.sangle = salpha;
						vertex.sangle = pDoc->Get_Correct_Angle(vertex.sangle);
						vertex.eangle = ealpha;
						vertex.eangle = pDoc->Get_Correct_Angle(vertex.eangle);

						pnewregion->m_cVertex.SetAt(j, vertex);
					}

					for(int j=0; j<pnewregion->m_arrayHoles.GetCount(); j++)
					{
						CArray<Fpoint, Fpoint>* p_arrayHole = pnewregion->m_arrayHoles.GetAt(j);
						for(int k=0; k<p_arrayHole->GetCount(); k++)
						{
							Fpoint fpoint = p_arrayHole->GetAt(k);
							double fx, fy;
							pDoc->Get_Rotate_XY(&fx, &fy, fpoint.fx - x0, fpoint.fy - y0, rotation, mirror);
							fpoint.fx = fx + x0 + deltax;
							fpoint.fy = fy + y0 + deltay;
							p_arrayHole->SetAt(k, fpoint);
						}
					}
				}
				for(int i=0; i<arrayEDA_PcbClipBoardPolygon.GetCount(); i++)
				{
					COb_pcbpolygon* ppolygon = arrayEDA_PcbClipBoardPolygon.GetAt(i);
					if(ppolygon->m_nFlag != FLAG_NORMAL) continue;

					COb_pcbpolygon* pnewpolygon = new COb_pcbpolygon();
					pnewpolygon->Copy(ppolygon);
					pnewpolygon->pDocument = pDoc;
					pnewpolygon->m_nNet = -1;
					int newindex = pDoc->m_arrayPcbPolygon.Add(pnewpolygon);
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_POLYGON;
					pundo->m_nIndex = newindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					

					for(int j=0; j<pnewpolygon->m_cVertex.GetCount(); j++)
					{
						Struct_RegionVertex vertex = pnewpolygon->m_cVertex.GetAt(j);
						
						double fx, fy;
						pDoc->Get_Rotate_XY(&fx, &fy, vertex.fx - x0, vertex.fy - y0, rotation, mirror);
						vertex.fx = fx + x0 + deltax;
						vertex.fy = fy + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx, &fy, vertex.cx - x0, vertex.cy - y0, rotation, mirror);
						vertex.cx = fx + x0 + deltax;
						vertex.cy = fy + y0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, vertex.sangle, vertex.eangle, rotation, mirror);
						vertex.sangle = salpha;
						vertex.sangle = pDoc->Get_Correct_Angle(vertex.sangle);
						vertex.eangle = ealpha;
						vertex.eangle = pDoc->Get_Correct_Angle(vertex.eangle);

						pnewpolygon->m_cVertex.SetAt(j, vertex);
					}

					
					
					for(int j=0; j<arrayEDA_PcbClipBoardTrack.GetCount(); j++)
					{
						COb_pcbtrack* ptrack = arrayEDA_PcbClipBoardTrack.GetAt(j);
						if(ptrack->m_nPolygon == i)
						{
							COb_pcbtrack* pnewtrack = new COb_pcbtrack();
							pnewtrack->Copy(ptrack);
							pnewtrack->m_nPolygon = newindex;
							pnewtrack->pDocument = pDoc;
							pnewtrack->m_nNet = -1;
							int newsubindex = pDoc->m_arrayPcbTrack.Add(pnewtrack);
							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = newsubindex;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							
							double fx, fy;
							pDoc->Get_Rotate_XY(&fx, &fy, pnewtrack->m_fX1 - x0, pnewtrack->m_fY1 - y0, rotation, mirror);
							pnewtrack->m_fX1 = fx + x0 + deltax;
							pnewtrack->m_fY1 = fy + y0 + deltay;
							pDoc->Get_Rotate_XY(&fx, &fy, pnewtrack->m_fX2 - x0, pnewtrack->m_fY2 - y0, rotation, mirror);
							pnewtrack->m_fX2 = fx + x0 + deltax;
							pnewtrack->m_fY2 = fy + y0 + deltay;
						}
					}
					for(int j=0; j<arrayEDA_PcbClipBoardArc.GetCount(); j++)
					{
						COb_pcbarc* parc = arrayEDA_PcbClipBoardArc.GetAt(j);
						if(parc->m_nPolygon == i)
						{
							COb_pcbarc* pnewarc = new COb_pcbarc();
							pnewarc->Copy(parc);
							pnewarc->m_nPolygon = newindex;
							pnewarc->pDocument = pDoc;
							pnewarc->m_nNet = -1;
							int newsubindex = pDoc->m_arrayPcbArc.Add(pnewarc);

							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = PCB_ELEMENT_ARC;
							pundo->m_nIndex = newsubindex;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							
							double fx, fy;
							pDoc->Get_Rotate_XY(&fx, &fy, pnewarc->m_fCx - x0, pnewarc->m_fCy - y0, rotation, mirror);
							pnewarc->m_fCx = fx + x0 + deltax;
							pnewarc->m_fCy = fy + y0 + deltay;
										
							double salpha, ealpha;
							pDoc->Get_Rotate_Angle(&salpha, &ealpha, pnewarc->m_fSangle, pnewarc->m_fEangle, rotation, mirror);
							pnewarc->m_fSangle = salpha;
							pnewarc->m_fSangle = pDoc->Get_Correct_Angle(pnewarc->m_fSangle);
							pnewarc->m_fEangle = ealpha;
							pnewarc->m_fEangle = pDoc->Get_Correct_Angle(pnewarc->m_fEangle);
						}
					}

					for(int j=0; j<arrayEDA_PcbClipBoardRegion.GetCount(); j++)
					{
						COb_pcbregion* pregion = arrayEDA_PcbClipBoardRegion.GetAt(j);
						if(pregion->m_nPolygon == i)
						{
							COb_pcbregion* pnewregion = new COb_pcbregion();
							pnewregion->Copy(pregion);
							pnewregion->m_nPolygon = newindex;
							pnewregion->pDocument = pDoc;
							pnewregion->m_nNet = -1;
							int newsubindex = pDoc->m_arrayPcbRegion.Add(pnewregion);
							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = PCB_ELEMENT_REGION;
							pundo->m_nIndex = newsubindex;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							

							for(int k=0; k<pnewregion->m_cVertex.GetCount(); k++)
							{
								Struct_RegionVertex vertex = pnewregion->m_cVertex.GetAt(k);
								
								double fx, fy;
								pDoc->Get_Rotate_XY(&fx, &fy, vertex.fx - x0, vertex.fy - y0, rotation, mirror);
								vertex.fx = fx + x0 + deltax;
								vertex.fy = fy + y0 + deltay;
								pDoc->Get_Rotate_XY(&fx, &fy, vertex.cx - x0, vertex.cy - y0, rotation, mirror);
								vertex.cx = fx + x0 + deltax;
								vertex.cy = fy + y0 + deltay;
							
								double salpha, ealpha;
								pDoc->Get_Rotate_Angle(&salpha, &ealpha, vertex.sangle, vertex.eangle, rotation, mirror);
								vertex.sangle = salpha;
								vertex.sangle = pDoc->Get_Correct_Angle(vertex.sangle);
								vertex.eangle = ealpha;
								vertex.eangle = pDoc->Get_Correct_Angle(vertex.eangle);

								pnewregion->m_cVertex.SetAt(j, vertex);
							}

							for(int m=0; m<pnewregion->m_arrayHoles.GetCount(); m++)
							{
								CArray<Fpoint, Fpoint>* p_arrayHole = pnewregion->m_arrayHoles.GetAt(m);
								for(int k=0; k<p_arrayHole->GetCount(); k++)
								{
									Fpoint fpoint = p_arrayHole->GetAt(k);
									double fx, fy;
									pDoc->Get_Rotate_XY(&fx, &fy, fpoint.fx - x0, fpoint.fy - y0, rotation, mirror);
									fpoint.fx = fx + x0 + deltax;
									fpoint.fy = fy + y0 + deltay;
									p_arrayHole->SetAt(k, fpoint);
								}
							}
						}
					}
				}
				for(int i=0; i<arrayEDA_PcbClipBoardPad.GetCount(); i++)
				{
					COb_pcbpad* ppad = arrayEDA_PcbClipBoardPad.GetAt(i);
					if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent >= 0)) continue;

					COb_pcbpad* pnewpad = new COb_pcbpad();
					pnewpad->Copy(ppad);
					pnewpad->pDocument = pDoc;
					pnewpad->m_nNet = -1;
					int newindex = pDoc->m_arrayPcbPad.Add(pnewpad);
					
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
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pnewpad->m_fCx - x0, pnewpad->m_fCy - y0, rotation, mirror);
					pnewpad->m_fCx = fx + x0 + deltax;
					pnewpad->m_fCy = fy + y0 + deltay;

					if(mirror == TRUE)
					{
						pnewpad->m_fRotation = 0 - pnewpad->m_fRotation;
						pnewpad->m_fRotation -= rotation;
					}
					else
					{
						pnewpad->m_fRotation += rotation;
					}
					pnewpad->m_fRotation = pDoc->Get_Correct_Angle(pnewpad->m_fRotation);
				}		
				for(int i=0; i<arrayEDA_PcbClipBoardVia.GetCount(); i++)
				{
					COb_pcbvia* pvia = arrayEDA_PcbClipBoardVia.GetAt(i);
					if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nComponent >= 0)) continue;

					COb_pcbvia* pnewvia = new COb_pcbvia();
					pnewvia->Copy(pvia);
					pnewvia->pDocument = pDoc;
					pnewvia->m_nNet = -1;
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
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pnewvia->m_fCx - x0, pnewvia->m_fCy - y0, rotation, mirror);
					pnewvia->m_fCx = fx + x0 + deltax;
					pnewvia->m_fCy = fy + y0 + deltay;
				}	
				for(int i=0; i<arrayEDA_PcbClipBoardComp.GetCount(); i++)
				{
					COb_pcbcomp* pcomp = arrayEDA_PcbClipBoardComp.GetAt(i);
					if(pcomp->m_nFlag != FLAG_NORMAL) continue;

					COb_pcbcomp* pnewcomp = new COb_pcbcomp();
					pnewcomp->Copy(pcomp);
					pnewcomp->pDocument = pDoc;
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
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pnewcomp->m_fX - x0, pnewcomp->m_fY - y0, rotation, mirror);
					pnewcomp->m_fX = fx + x0 + deltax;
					pnewcomp->m_fY = fy + y0 + deltay;
					pnewcomp->m_fRotation += rotation;
					if(mirror == TRUE) pnewcomp->m_fRotation = 180 - pnewcomp->m_fRotation;
					pnewcomp->m_fRotation = pDoc->Get_Correct_Angle(pnewcomp->m_fRotation);

					for(int j=0; j<arrayEDA_PcbClipBoardTrack.GetCount(); j++)
					{
						COb_pcbtrack* ptrack = arrayEDA_PcbClipBoardTrack.GetAt(j);
						if(ptrack->m_nComponent == i)
						{
							COb_pcbtrack* pnewtrack = new COb_pcbtrack();
							pnewtrack->Copy(ptrack);
							pnewtrack->pDocument = pDoc;
							pnewtrack->m_nNet = -1;
							pnewtrack->m_nComponent = newindex;
							int newsubindex = pDoc->m_arrayPcbTrack.Add(pnewtrack);
							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = newsubindex;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							
							double fx, fy;
							pDoc->Get_Rotate_XY(&fx, &fy, pnewtrack->m_fX1 - x0, pnewtrack->m_fY1 - y0, rotation, mirror);
							pnewtrack->m_fX1 = fx + x0 + deltax;
							pnewtrack->m_fY1 = fy + y0 + deltay;
							pDoc->Get_Rotate_XY(&fx, &fy, pnewtrack->m_fX2 - x0, pnewtrack->m_fY2 - y0, rotation, mirror);
							pnewtrack->m_fX2 = fx + x0 + deltax;
							pnewtrack->m_fY2 = fy + y0 + deltay;
						}
					}
					for(int j=0; j<arrayEDA_PcbClipBoardArc.GetCount(); j++)
					{
						COb_pcbarc* parc = arrayEDA_PcbClipBoardArc.GetAt(j);
						if(parc->m_nComponent == i)
						{
							COb_pcbarc* pnewarc = new COb_pcbarc();
							pnewarc->Copy(parc);
							pnewarc->pDocument = pDoc;
							pnewarc->m_nNet = -1;
							pnewarc->m_nComponent = newindex;
							int newsubindex = pDoc->m_arrayPcbArc.Add(pnewarc);

							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = PCB_ELEMENT_ARC;
							pundo->m_nIndex = newsubindex;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							
							double fx, fy;
							pDoc->Get_Rotate_XY(&fx, &fy, pnewarc->m_fCx - x0, pnewarc->m_fCy - y0, rotation, mirror);
							pnewarc->m_fCx = fx + x0 + deltax;
							pnewarc->m_fCy = fy + y0 + deltay;
										
							double salpha, ealpha;
							pDoc->Get_Rotate_Angle(&salpha, &ealpha, pnewarc->m_fSangle, pnewarc->m_fEangle, rotation, mirror);
							pnewarc->m_fSangle = salpha;
							pnewarc->m_fSangle = pDoc->Get_Correct_Angle(pnewarc->m_fSangle);
							pnewarc->m_fEangle = ealpha;
							pnewarc->m_fEangle = pDoc->Get_Correct_Angle(pnewarc->m_fEangle);
						}
					}
					for(int j=0; j<arrayEDA_PcbClipBoardFill.GetCount(); j++)
					{
						COb_pcbfill* pfill = arrayEDA_PcbClipBoardFill.GetAt(j);
						if(pfill->m_nComponent == i)
						{
							COb_pcbfill* pnewfill = new COb_pcbfill();
							pnewfill->Copy(pfill);
							pnewfill->pDocument  = pDoc;
							pnewfill->m_nNet = -1;
							pnewfill->m_nComponent = newindex;
							int newsubindex = pDoc->m_arrayPcbFill.Add(pnewfill);
							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = PCB_ELEMENT_FILL;
							pundo->m_nIndex = newsubindex;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							
							double fx, fy;
							double fmx, fmy, dx, dy;
							dx = abs(pnewfill->m_fX1 - pnewfill->m_fX2);
							dy = abs(pnewfill->m_fY1 - pnewfill->m_fY2);
							fmx = (pnewfill->m_fX1 + pnewfill->m_fX2)/2;
							fmy = (pnewfill->m_fY1 + pnewfill->m_fY2)/2;
							pDoc->Get_Rotate_XY(&fx, &fy, fmx - x0, fmy - y0, rotation, mirror);
							fmx = fx + x0 + deltax;
							fmy = fy + y0 + deltay;

							pnewfill->m_fX1 = fmx - dx/2;
							pnewfill->m_fY1 = fmy - dy/2;
							pnewfill->m_fX2 = fmx + dx/2;
							pnewfill->m_fY2 = fmy + dy/2;
							
							if(mirror == TRUE)
							{
								pnewfill->m_fRotation = 0 - pnewfill->m_fRotation;
								pnewfill->m_fRotation -= rotation;
							}
							else
							{
								pnewfill->m_fRotation += rotation;
							}
							pnewfill->m_fRotation = pDoc->Get_Correct_Angle(pnewfill->m_fRotation);
						}
					}
					for(int j=0; j<arrayEDA_PcbClipBoardPad.GetCount(); j++)
					{
						COb_pcbpad* ppad = arrayEDA_PcbClipBoardPad.GetAt(j);
						if(ppad->m_nComponent == i)
						{
							COb_pcbpad* pnewpad = new COb_pcbpad();
							pnewpad->Copy(ppad);
							pnewpad->pDocument = pDoc;
							pnewpad->m_nNet = -1;
							pnewpad->m_nComponent = newindex;
							int newsubindex = pDoc->m_arrayPcbPad.Add(pnewpad);
							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = PCB_ELEMENT_PAD;
							pundo->m_nIndex = newsubindex;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							
							double fx, fy;
							pDoc->Get_Rotate_XY(&fx, &fy, pnewpad->m_fCx - x0, pnewpad->m_fCy - y0, rotation, mirror);
							pnewpad->m_fCx = fx + x0 + deltax;
							pnewpad->m_fCy = fy + y0 + deltay;

							if(mirror == TRUE)
							{
								pnewpad->m_fRotation = 0 - pnewpad->m_fRotation;
								pnewpad->m_fRotation -= rotation;
							}
							else
							{
								pnewpad->m_fRotation += rotation;
							}
							pnewpad->m_fRotation = pDoc->Get_Correct_Angle(pnewpad->m_fRotation);
						}
					}
					for(int j=0; j<arrayEDA_PcbClipBoardVia.GetCount(); j++)
					{
						COb_pcbvia* pvia = arrayEDA_PcbClipBoardVia.GetAt(j);
						if(pvia->m_nComponent == i)
						{
							COb_pcbvia* pnewvia = new COb_pcbvia();
							pnewvia->Copy(pvia);
							pnewvia->pDocument = pDoc;
							pnewvia->m_nNet = -1;
							pnewvia->m_nComponent = newindex;
							int newsubindex = pDoc->m_arrayPcbVia.Add(pnewvia);
							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = PCB_ELEMENT_VIA;
							pundo->m_nIndex = newsubindex;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							
							double fx, fy;
							pDoc->Get_Rotate_XY(&fx, &fy, pnewvia->m_fCx - x0, pnewvia->m_fCy - y0, rotation, mirror);
							pnewvia->m_fCx = fx + x0 + deltax;
							pnewvia->m_fCy = fy + y0 + deltay;
						}
					}
					for(int j=0; j<arrayEDA_PcbClipBoardText.GetCount(); j++)
					{
						COb_pcbtext* ptext = arrayEDA_PcbClipBoardText.GetAt(j);
						if(ptext->m_nComponent == i)
						{
							COb_pcbtext* pnewtext = new COb_pcbtext();
							pnewtext->Copy(ptext);
							pnewtext->pDocument = pDoc;
							pnewtext->m_nComponent = newindex;
							int newsubindex = pDoc->m_arrayPcbText.Add(pnewtext);
							
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_DELETE;
							pundo->m_nOb = PCB_ELEMENT_TEXT;
							pundo->m_nIndex = newsubindex;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							
							double fx, fy;
							pDoc->Get_Rotate_XY(&fx, &fy, pnewtext->m_fSx - x0, pnewtext->m_fSy - y0, rotation, mirror);
							pnewtext->m_fSx = fx + x0 + deltax;
							pnewtext->m_fSy = fy + y0 + deltay;

							pnewtext->m_fRotation += rotation;
							if(mirror == TRUE) 
							{
								if(pnewtext->m_bMirror == TRUE) pnewtext->m_bMirror = FALSE;
								else pnewtext->m_bMirror = TRUE;
								pnewtext->m_fRotation = 360 - pnewtext->m_fRotation;
							}

							pnewtext->m_fRotation = pDoc->Get_Correct_Angle(pnewtext->m_fRotation);
							pnewtext->UpdateStringBorder(this, FALSE);
						}
					}
				}	
				
				
				if(undo_flag == TRUE)
				{
					pDoc->m_nPCB_Undo_Num++;
					if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				}				
				
				break;
			}
			else if(ch1 == SPACE)
			{
				CloseMouseCursor();
				afill.DrawFrame(&dc, this);
				OpenMouseCursor();

				flag++;
				if(flag>3) flag=0;

				x1 = m_fCurrentSnapX;  
				y1 = m_fCurrentSnapY;
				deltax = x1 - x0;
				deltay = y1 - y0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, fEDA_PcbClipBoard_X1 - x0, fEDA_PcbClipBoard_Y1 - y0, flag*90, mirror);
				afill.m_fX1 = fx + x0 + deltax;
				afill.m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, fEDA_PcbClipBoard_X2 - x0, fEDA_PcbClipBoard_Y2 - y0, flag*90, mirror);
				afill.m_fX2 = fx + x0 + deltax;
				afill.m_fY2 = fy + y0 + deltay;

				CloseMouseCursor();
				afill.DrawFrame(&dc, this);
				OpenMouseCursor();
			}
			else if((ch1 == KEY_x)||(ch1 == KEY_X))
			{
				if(compflag == 1)
				{
					if(AfxMessageBox("在同一个设计层上翻转元件会产生安装错误, 元件的换层翻转操作应该在移动元件时使用'L'键实现, 仍然继续?", MB_YESNO|MB_ICONWARNING) != IDYES)
						continue;
				}
				CloseMouseCursor();
				afill.DrawFrame(&dc, this);
				OpenMouseCursor();

				if(mirror == FALSE) mirror = TRUE;
				else mirror = FALSE;

				x1 = m_fCurrentSnapX;  
				y1 = m_fCurrentSnapY;
				deltax = x1 - x0;
				deltay = y1 - y0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, fEDA_PcbClipBoard_X1 - x0, fEDA_PcbClipBoard_Y1 - y0, flag*90, mirror);
				afill.m_fX1 = fx + x0 + deltax;
				afill.m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, fEDA_PcbClipBoard_X2 - x0, fEDA_PcbClipBoard_Y2 - y0, flag*90, mirror);
				afill.m_fX2 = fx + x0 + deltax;
				afill.m_fY2 = fy + y0 + deltay;

				CloseMouseCursor();
				afill.DrawFrame(&dc, this);
				OpenMouseCursor();
			}
		}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;

	//pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
	Invalidate();
}

void CESDView::OnUpdatePcbEditPaste(CCmdUI *pCmdUI)
{
	if( (arrayEDA_PcbClipBoardArc.GetCount() > 0) || (arrayEDA_PcbClipBoardTrack.GetCount() > 0) ||\
		(arrayEDA_PcbClipBoardText.GetCount() > 0) || (arrayEDA_PcbClipBoardFill.GetCount() > 0) ||\
		(arrayEDA_PcbClipBoardRegion.GetCount() > 0) || (arrayEDA_PcbClipBoardPolygon.GetCount() > 0) ||\
		(arrayEDA_PcbClipBoardPad.GetCount() > 0) || (arrayEDA_PcbClipBoardVia.GetCount() > 0) ||\
		(arrayEDA_PcbClipBoardComp.GetCount() > 0) || (arrayEDA_PcbClipBoardCoordinate.GetCount() > 0) ||\
		(arrayEDA_PcbClipBoardDimension.GetCount() > 0) ) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CESDView::OnPcbEditCopy()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(nEDA_System_State == PCB)
	{
		
		while (!arrayEDA_PcbClipBoardArc.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardArc.GetAt(0);
			arrayEDA_PcbClipBoardArc.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardPad.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardPad.GetAt(0);
			arrayEDA_PcbClipBoardPad.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardVia.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardVia.GetAt(0);
			arrayEDA_PcbClipBoardVia.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardTrack.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardTrack.GetAt(0);
			arrayEDA_PcbClipBoardTrack.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardText.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardText.GetAt(0);
			arrayEDA_PcbClipBoardText.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardFill.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardFill.GetAt(0);
			arrayEDA_PcbClipBoardFill.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardRegion.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardRegion.GetAt(0);
			arrayEDA_PcbClipBoardRegion.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardCoordinate.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardCoordinate.GetAt(0);
			arrayEDA_PcbClipBoardCoordinate.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardDimension.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardDimension.GetAt(0);
			arrayEDA_PcbClipBoardDimension.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardComp.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardComp.GetAt(0);
			arrayEDA_PcbClipBoardComp.RemoveAt(0);
		}
		while (!arrayEDA_PcbClipBoardPolygon.IsEmpty())
		{
			delete arrayEDA_PcbClipBoardPolygon.GetAt(0);
			arrayEDA_PcbClipBoardPolygon.RemoveAt(0);
		}

		for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
			if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_bSelection != TRUE)||(parc->m_nComponent >= 0)||(parc->m_nPolygon >= 0)) continue;

			
			COb_pcbarc* pnewarc = new COb_pcbarc();
			pnewarc->Copy(parc);
			pnewarc->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardArc.Add(pnewarc);
		}
		for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
			if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_bSelection != TRUE)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;

			
			COb_pcbtrack* pnewtrack = new COb_pcbtrack();
			pnewtrack->Copy(ptrack);
			pnewtrack->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardTrack.Add(pnewtrack);
		}
		for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
			if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_bSelection != TRUE)||(pfill->m_nComponent >= 0)) continue;

			
			COb_pcbfill* pnewfill = new COb_pcbfill();
			pnewfill->Copy(pfill);
			pnewfill->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardFill.Add(pnewfill);
		}
		for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
			if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_bSelection != TRUE)||(ppad->m_nComponent >= 0)) continue;

			
			COb_pcbpad* pnewpad = new COb_pcbpad();
			pnewpad->Copy(ppad);
			pnewpad->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardPad.Add(pnewpad);
		}
		for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
			if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_bSelection != TRUE)||(pvia->m_nComponent >= 0)) continue;

			
			COb_pcbvia* pnewvia = new COb_pcbvia();
			pnewvia->Copy(pvia);
			pnewvia->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardVia.Add(pnewvia);
		}
		for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
			if((ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_bSelection != TRUE)||(ptext->m_nComponent >= 0)) continue;

			
			COb_pcbtext* pnewtext = new COb_pcbtext();
			pnewtext->Copy(ptext);
			pnewtext->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardText.Add(pnewtext);
		}
		for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_bSelection != TRUE)||(pregion->m_nComponent >= 0)||(pregion->m_nPolygon >= 0)) continue;

			
			COb_pcbregion* pnewregion = new COb_pcbregion();
			pnewregion->Copy(pregion);
			pnewregion->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardRegion.Add(pnewregion);
		}
		for(int i=0; i<pDoc->m_arrayPcbCoordinate.GetCount(); i++)
		{
			COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(i);
			if((pcoordinate->m_nFlag != FLAG_NORMAL)||(pcoordinate->m_bSelection != TRUE)) continue;

			
			COb_pcbcoordinate* pnewcoordinate = new COb_pcbcoordinate();
			pnewcoordinate->Copy(pcoordinate);
			pnewcoordinate->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardCoordinate.Add(pnewcoordinate);
		}
		for(int i=0; i<pDoc->m_arrayPcbDimension.GetCount(); i++)
		{
			COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(i);
			if((pdimension->m_nFlag != FLAG_NORMAL)||(pdimension->m_bSelection != TRUE)) continue;

			
			COb_pcbdimension* pnewdimension = new COb_pcbdimension();
			pnewdimension->Copy(pdimension);
			pnewdimension->m_bSelection = FALSE;
			arrayEDA_PcbClipBoardDimension.Add(pnewdimension);
		}
		for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
		{
			COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
			if((pcomp->m_nFlag != FLAG_NORMAL)||(pcomp->m_bSelection != TRUE)) continue;

			
			COb_pcbcomp* pnewcomp = new COb_pcbcomp();
			pnewcomp->Copy(pcomp);
			pnewcomp->m_bSelection = FALSE;
			int compindex = arrayEDA_PcbClipBoardComp.Add(pnewcomp);

			for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
				if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nComponent != i)) continue;
				
				
				COb_pcbarc* pnewarc = new COb_pcbarc();
				pnewarc->Copy(parc);
				pnewarc->m_bSelection = FALSE;
				pnewarc->m_nComponent = compindex;
				arrayEDA_PcbClipBoardArc.Add(pnewarc);
			}
			for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
				if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nComponent != i)) continue;
				
				
				COb_pcbtrack* pnewtrack = new COb_pcbtrack();
				pnewtrack->Copy(ptrack);
				pnewtrack->m_bSelection = FALSE;
				pnewtrack->m_nComponent = compindex;
				arrayEDA_PcbClipBoardTrack.Add(pnewtrack);
			}
			for(int j=0; j<pDoc->m_arrayPcbText.GetCount(); j++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(j);
				if((ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_nComponent != i)) continue;
				
				
				COb_pcbtext* pnewtext = new COb_pcbtext();
				pnewtext->Copy(ptext);
				pnewtext->m_bSelection = FALSE;
				pnewtext->m_nComponent = compindex;
				arrayEDA_PcbClipBoardText.Add(pnewtext);
			}
			for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
				if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent != i)) continue;
				
				
				COb_pcbfill* pnewfill = new COb_pcbfill();
				pnewfill->Copy(pfill);
				pnewfill->m_bSelection = FALSE;
				pnewfill->m_nComponent = compindex;
				arrayEDA_PcbClipBoardFill.Add(pnewfill);
			}
			for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
				if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent != i)) continue;
				
				
				COb_pcbpad* pnewpad = new COb_pcbpad();
				pnewpad->Copy(ppad);
				pnewpad->m_bSelection = FALSE;
				pnewpad->m_nComponent = compindex;
				arrayEDA_PcbClipBoardPad.Add(pnewpad);
			}
			for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
				if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nComponent != i)) continue;
				
				
				COb_pcbvia* pnewvia = new COb_pcbvia();
				pnewvia->Copy(pvia);
				pnewvia->m_bSelection = FALSE;
				pnewvia->m_nComponent = compindex;
				arrayEDA_PcbClipBoardVia.Add(pnewvia);
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
			if((ppolygon->m_nFlag != FLAG_NORMAL)||(ppolygon->m_bSelection != TRUE)) continue;

			
			COb_pcbpolygon* pnewpolygon = new COb_pcbpolygon();
			pnewpolygon->Copy(ppolygon);
			pnewpolygon->m_bSelection = FALSE;
			int polygonindex = arrayEDA_PcbClipBoardPolygon.Add(pnewpolygon);

			for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
				if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon != i)) continue;
				
				
				COb_pcbarc* pnewarc = new COb_pcbarc();
				pnewarc->Copy(parc);
				pnewarc->m_bSelection = FALSE;
				pnewarc->m_nPolygon = polygonindex;
				arrayEDA_PcbClipBoardArc.Add(pnewarc);
			}
			for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
				if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nPolygon != i)) continue;
				
				
				COb_pcbtrack* pnewtrack = new COb_pcbtrack();
				pnewtrack->Copy(ptrack);
				pnewtrack->m_bSelection = FALSE;
				pnewtrack->m_nPolygon = polygonindex;
				arrayEDA_PcbClipBoardTrack.Add(pnewtrack);
			}
			for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
			{
				COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
				if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon != i)) continue;
				
				
				COb_pcbregion* pnewregion = new COb_pcbregion();
				pnewregion->Copy(pregion);
				pnewregion->m_bSelection = FALSE;
				pnewregion->m_nComponent = polygonindex;
				arrayEDA_PcbClipBoardRegion.Add(pnewregion);
			}
		}		

		fEDA_PcbClipBoard_X1 = pDoc->m_fPcbSelection_X1;
		fEDA_PcbClipBoard_Y1 = pDoc->m_fPcbSelection_Y1;
		fEDA_PcbClipBoard_X2 = pDoc->m_fPcbSelection_X2;
		fEDA_PcbClipBoard_Y2 = pDoc->m_fPcbSelection_Y2;

		pDoc->m_arrayPcbSelection.RemoveAll();
		Invalidate();
	}
}

void CESDView::OnUpdatePcbEditCopy(CCmdUI *pCmdUI)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(pDoc->m_arrayPcbSelection.GetCount() > 0) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

