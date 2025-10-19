void CESDView::PcbMoveElement(int pcbelement, int index)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	if((pcbelement == 0)||( index < 0)) return;
	if(pcbelement == PCB_ELEMENT_TRACK)
	{
		COb_pcbtrack* ptrack = (COb_pcbtrack*)pDoc->m_arrayPcbTrack.GetAt(index);
        ptrack->Move(&dc, this, index);
	}
	if(pcbelement == PCB_ELEMENT_ARC)
	{
		COb_pcbarc* parc = (COb_pcbarc*)pDoc->m_arrayPcbArc.GetAt(index);
        parc->Move(&dc, this, index);
	}
	if(pcbelement == PCB_ELEMENT_FILL)
	{
		COb_pcbfill* pfill = (COb_pcbfill*)pDoc->m_arrayPcbFill.GetAt(index);
        pfill->Move(&dc, this, index);
	}
	if(pcbelement == PCB_ELEMENT_PAD)
	{
		COb_pcbpad* ppad = (COb_pcbpad*)pDoc->m_arrayPcbPad.GetAt(index);
        ppad->Move(&dc, this, index);
	}
	if(pcbelement == PCB_ELEMENT_VIA)
	{
		COb_pcbvia* pvia = (COb_pcbvia*)pDoc->m_arrayPcbVia.GetAt(index);
        pvia->Move(&dc, this, index);
	}
	if(pcbelement == PCB_ELEMENT_REGION)
	{
		COb_pcbregion* pregion = (COb_pcbregion*)pDoc->m_arrayPcbRegion.GetAt(index);
        pregion->Move(&dc, this, index);
	}
	if(pcbelement == PCB_ELEMENT_TEXT)
	{
		COb_pcbtext* ptext = (COb_pcbtext*)pDoc->m_arrayPcbText.GetAt(index);
        ptext->Move(&dc, this, index);
	}
	if(pcbelement == PCB_ELEMENT_POLYGON)
	{
		COb_pcbpolygon* ppolygon = (COb_pcbpolygon*)pDoc->m_arrayPcbPolygon.GetAt(index);
		if(ppolygon->m_nPolygonType == COPPER_POLYGON) ppolygon->Move(&dc, this, index);
	}
	if(pcbelement == PCB_ELEMENT_COMP)
	{
		COb_pcbcomp* pcomp = (COb_pcbcomp*)pDoc->m_arrayPcbComp.GetAt(index);
		pcomp->Move(&dc, this, index);
	}
	if(pcbelement == PCB_ELEMENT_COORDINATE)
	{
		COb_pcbcoordinate* pcoordinate = (COb_pcbcoordinate*)pDoc->m_arrayPcbCoordinate.GetAt(index);
		pcoordinate->Move(&dc, this, index);
	}
	if(pcbelement == PCB_ELEMENT_DIMENSION)
	{
		COb_pcbdimension* pdimension = (COb_pcbdimension*)pDoc->m_arrayPcbDimension.GetAt(index);
		if(pdimension->m_nDimensionKind == 8) pdimension->Move(&dc, this, index);
	}
}

void CESDView::PcbCopyElement(int pcbelement, int index)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);
	
	if((pcbelement == 0)||( index < 0)) return;
	if(pcbelement == PCB_ELEMENT_TRACK)
	{
		COb_pcbtrack* ptrack = (COb_pcbtrack*)pDoc->m_arrayPcbTrack.GetAt(index);

		COb_pcbtrack* pnewtrack = new COb_pcbtrack();
		pnewtrack->Copy(ptrack);
		pnewtrack->m_bSelection = FALSE;

		int newindex = pDoc->m_arrayPcbTrack.Add(pnewtrack);
		if(pnewtrack->MovePlaced(&dc, this) == FALSE)
		{
			
			pDoc->m_arrayPcbTrack.RemoveAt(newindex);
			delete pnewtrack;
		}
		else
		{
			
			if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_TRACK;
			pundo->m_nIndex = newindex;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

			pDoc->m_nPCB_Undo_Num++;
			if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}	
	}
	if(pcbelement == PCB_ELEMENT_ARC)
	{
		COb_pcbarc* parc = (COb_pcbarc*)pDoc->m_arrayPcbArc.GetAt(index);
		
		COb_pcbarc* pnewarc = new COb_pcbarc();
		pnewarc->Copy(parc);
		pnewarc->m_bSelection = FALSE;

		int newindex = pDoc->m_arrayPcbArc.Add(pnewarc);
		if(pnewarc->MovePlaced(&dc, this) == FALSE)
		{
			
			pDoc->m_arrayPcbArc.RemoveAt(newindex);
			delete pnewarc;
		}
		else
		{
			
			if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_ARC;
			pundo->m_nIndex = newindex;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

			pDoc->m_nPCB_Undo_Num++;
			if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}	
	}
	if(pcbelement == PCB_ELEMENT_FILL)
	{
		COb_pcbfill* pfill = (COb_pcbfill*)pDoc->m_arrayPcbFill.GetAt(index);
		
		COb_pcbfill* pnewfill = new COb_pcbfill();
		pnewfill->Copy(pfill);
		pnewfill->m_bSelection = FALSE;

		int newindex = pDoc->m_arrayPcbFill.Add(pnewfill);
		if(pnewfill->MovePlaced(&dc, this) == FALSE)
		{
			
			pDoc->m_arrayPcbFill.RemoveAt(newindex);
			delete pnewfill;
		}
		else
		{
			
			if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_FILL;
			pundo->m_nIndex = newindex;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

			pDoc->m_nPCB_Undo_Num++;
			if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}	
	}
	if(pcbelement == PCB_ELEMENT_PAD)
	{
		COb_pcbpad* ppad = (COb_pcbpad*)pDoc->m_arrayPcbPad.GetAt(index);
		
		COb_pcbpad* pnewpad = new COb_pcbpad();
		pnewpad->Copy(ppad);
		pnewpad->m_bSelection = FALSE;

		int newindex = pDoc->m_arrayPcbPad.Add(pnewpad);
		if(pnewpad->MovePlaced(&dc, this) == FALSE)
		{
			
			pDoc->m_arrayPcbPad.RemoveAt(newindex);
			delete pnewpad;
		}
		else
		{
			
			if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_PAD;
			pundo->m_nIndex = newindex;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

			pDoc->m_nPCB_Undo_Num++;
			if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}	
	}
	if(pcbelement == PCB_ELEMENT_VIA)
	{
		COb_pcbvia* pvia = (COb_pcbvia*)pDoc->m_arrayPcbVia.GetAt(index);
		
		COb_pcbvia* pnewvia = new COb_pcbvia();
		pnewvia->Copy(pvia);
		pnewvia->m_bSelection = FALSE;

		int newindex = pDoc->m_arrayPcbVia.Add(pnewvia);
		if(pnewvia->MovePlaced(&dc, this) == FALSE)
		{
			
			pDoc->m_arrayPcbVia.RemoveAt(newindex);
			delete pnewvia;
		}
		else
		{
			
			if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_VIA;
			pundo->m_nIndex = newindex;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

			pDoc->m_nPCB_Undo_Num++;
			if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}	
	}
	if(pcbelement == PCB_ELEMENT_REGION)
	{
		COb_pcbregion* pregion = (COb_pcbregion*)pDoc->m_arrayPcbRegion.GetAt(index);
		
		COb_pcbregion* pnewregion = new COb_pcbregion();
		pnewregion->Copy(pregion);
		pnewregion->m_bSelection = FALSE;

		int newindex = pDoc->m_arrayPcbRegion.Add(pnewregion);
		if(pnewregion->MovePlaced(&dc, this) == FALSE)
		{
			
			pDoc->m_arrayPcbRegion.RemoveAt(newindex);
			delete pnewregion;
		}
		else
		{
			
			if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_REGION;
			pundo->m_nIndex = newindex;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

			pDoc->m_nPCB_Undo_Num++;
			if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}
	}
	if(pcbelement == PCB_ELEMENT_TEXT)
	{
		COb_pcbtext* ptext = (COb_pcbtext*)pDoc->m_arrayPcbText.GetAt(index);
		if(ptext->m_nComponent == -1)
		{
			COb_pcbtext* pnewtext = new COb_pcbtext();
			pnewtext->Copy(ptext);
			pnewtext->m_bSelection = FALSE;

			int newindex = pDoc->m_arrayPcbText.Add(pnewtext);
			if(pnewtext->MovePlaced(&dc, this) == FALSE)
			{
				
				pDoc->m_arrayPcbText.RemoveAt(newindex);
				delete pnewtext;
			}
			else
			{
				
				if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = PCB_ELEMENT_TEXT;
				pundo->m_nIndex = newindex;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

				pDoc->m_nPCB_Undo_Num++;
				if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
		}
	}
	if(pcbelement == PCB_ELEMENT_POLYGON)
	{
		COb_pcbpolygon* ppolygon = (COb_pcbpolygon*)pDoc->m_arrayPcbPolygon.GetAt(index);
		if(ppolygon->m_nPolygonType == COPPER_POLYGON) 
		{
			COb_pcbpolygon* pnewpolygon = new COb_pcbpolygon();
			pnewpolygon->Copy(ppolygon);
			pnewpolygon->m_bSelection = FALSE;

			int newindex = pDoc->m_arrayPcbPolygon.Add(pnewpolygon);
			if(pnewpolygon->MovePlaced(&dc, this) == FALSE)
			{
				
				pDoc->m_arrayPcbPolygon.RemoveAt(newindex);
				delete pnewpolygon;
			}
			else
			{
				
				if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = PCB_ELEMENT_POLYGON;
				pundo->m_nIndex = newindex;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
				CreatePolygonPour(pnewpolygon, newindex);
				
				pDoc->m_nPCB_Undo_Num++;
				if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
		}
	}
	if(pcbelement == PCB_ELEMENT_COMP)
	{
		COb_pcbcomp* pcomp = (COb_pcbcomp*)pDoc->m_arrayPcbComp.GetAt(index);
		
		COb_pcbcomp* pnewcomp = new COb_pcbcomp();
		pnewcomp->Copy(pcomp);
		pnewcomp->m_bSelection = FALSE;

		int newindex = pDoc->m_arrayPcbComp.Add(pnewcomp);

		for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
			if((parc->m_nFlag == FLAG_NORMAL)&&(parc->m_nComponent == index))
			{
				COb_pcbarc* pnewarc = new COb_pcbarc();
				pnewarc->Copy(parc);
				pnewarc->m_bSelection = FALSE;
				pnewarc->m_nComponent = newindex;
				pDoc->m_arrayPcbArc.Add(pnewarc);
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
			if((ptrack->m_nFlag == FLAG_NORMAL)&&(ptrack->m_nComponent == index))
			{
				COb_pcbtrack* pnewtrack = new COb_pcbtrack();
				pnewtrack->Copy(ptrack);
				pnewtrack->m_bSelection = FALSE;
				pnewtrack->m_nComponent = newindex;
				pDoc->m_arrayPcbTrack.Add(pnewtrack);
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
				pDoc->m_arrayPcbText.Add(pnewtext);
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
				pDoc->m_arrayPcbFill.Add(pnewfill);
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
				pDoc->m_arrayPcbVia.Add(pnewvia);
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
				pDoc->m_arrayPcbPad.Add(pnewpad);
			}
		}

		if(pnewcomp->MovePlacePackage(&dc, this, newindex) == FALSE)
		{
			
			pDoc->m_arrayPcbComp.RemoveAt(newindex);
			delete pnewcomp;
			for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
				if(parc->m_nComponent == newindex)
				{
					delete parc;
					pDoc->m_arrayPcbArc.RemoveAt(i);
					i--;
				}
			}
			for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
				if(ptrack->m_nComponent == newindex)
				{
					delete ptrack;
					pDoc->m_arrayPcbTrack.RemoveAt(i);
					i--;
				}
			}
			for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
				if(ppad->m_nComponent == newindex)
				{
					delete ppad;
					pDoc->m_arrayPcbPad.RemoveAt(i);
					i--;
				}
			}
			for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
				if(pfill->m_nComponent == newindex)
				{
					delete pfill;
					pDoc->m_arrayPcbFill.RemoveAt(i);
					i--;
				}
			}
			for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
				if(ptext->m_nComponent == newindex)
				{
					delete ptext;
					pDoc->m_arrayPcbText.RemoveAt(i);
					i--;
				}
			}
			for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
				if(pvia->m_nComponent == newindex)
				{
					delete pvia;
					pDoc->m_arrayPcbVia.RemoveAt(i);
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
			pundo->m_nIndex = newindex;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			

			
			for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
				if(parc->m_nComponent == newindex)
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
				if(ptrack->m_nComponent == newindex)
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
				if(ppad->m_nComponent == newindex)
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
				if(pfill->m_nComponent == newindex)
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
				if(ptext->m_nComponent == newindex)
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
				if(pvia->m_nComponent == newindex)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = i;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
				}
			}
			
			
			pDoc->m_nPCB_Undo_Num++;
			if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		}
	}
	if(pcbelement == PCB_ELEMENT_COORDINATE)
	{
		COb_pcbcoordinate* pcoordinate = (COb_pcbcoordinate*)pDoc->m_arrayPcbCoordinate.GetAt(index);
		pcoordinate->Move(&dc, this, index);
	}
	if(pcbelement == PCB_ELEMENT_DIMENSION)
	{
		COb_pcbdimension* pdimension = (COb_pcbdimension*)pDoc->m_arrayPcbDimension.GetAt(index);
		if(pdimension->m_nDimensionKind == 8) pdimension->Move(&dc, this, index);
	}
}

void CESDView::PcbPickupMoveOrCopy(int pcbelement, int index)
{
	bEDA_MouseMoveElement = TRUE;
	if((GetKeyState(VK_SHIFT)&0x80) != 0) PcbCopyElement(pcbelement, index);
	else PcbMoveElement(pcbelement, index);
	bEDA_MouseMoveElement = FALSE;	
}

void CESDView::PcbMoveAndRotateSelection(double x0, double y0,  double deltax, double deltay, int rotation, BOOL mirror)
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nComponent != -1)||(parc->m_nPolygon != -1)) continue;
		if(parc->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_ARC;
			pundo->m_nIndex = i;
			pundo->pcbarc.Copy(parc);
			pundo->pcbarc.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, parc->m_fCx - x0, parc->m_fCy - y0, rotation, mirror);
			parc->m_fCx = fx + x0 + deltax;
			parc->m_fCy = fy + y0 + deltay;
					
			double salpha, ealpha;
			pDoc->Get_Rotate_Angle(&salpha, &ealpha, parc->m_fSangle, parc->m_fEangle, rotation, mirror);
			parc->m_fSangle = salpha;
			parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
			parc->m_fEangle = ealpha;
			parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nComponent != -1)||(ptrack->m_nPolygon != -1)) continue;
		if(ptrack->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_TRACK;
			pundo->m_nIndex = i;
			pundo->pcbtrack.Copy(ptrack);
			pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, ptrack->m_fX1 - x0, ptrack->m_fY1 - y0, rotation, mirror);
			ptrack->m_fX1 = fx + x0 + deltax;
			ptrack->m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, ptrack->m_fX2 - x0, ptrack->m_fY2 - y0, rotation, mirror);
			ptrack->m_fX2 = fx + x0 + deltax;
			ptrack->m_fY2 = fy + y0 + deltay;
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_nComponent != -1)) continue;
		if(ptext->m_bSelection == TRUE)
		{			
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_TEXT;
			pundo->m_nIndex = i;
			pundo->pcbtext.Copy(ptext);
			pundo->pcbtext.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - x0, ptext->m_fSy - y0, rotation, mirror);
			ptext->m_fSx = fx + x0 + deltax;
			ptext->m_fSy = fy + y0 + deltay;

			ptext->m_fRotation += rotation;
			if(mirror == TRUE) 
			{
				if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
				else ptext->m_bMirror = TRUE;
				ptext->m_fRotation = 360 - ptext->m_fRotation;
			}

			ptext->m_fRotation = pDoc->Get_Correct_Angle(ptext->m_fRotation);
			ptext->UpdateStringBorder(this, FALSE);
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent != -1)) continue;
		if(pfill->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_FILL;
			pundo->m_nIndex = i;
			pundo->pcbfill.Copy(pfill);
			pundo->pcbfill.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double fx, fy;
			double fmx, fmy, dx, dy;
			dx = abs(pfill->m_fX1 - pfill->m_fX2);
			dy = abs(pfill->m_fY1 - pfill->m_fY2);
			fmx = (pfill->m_fX1 + pfill->m_fX2)/2;
			fmy = (pfill->m_fY1 + pfill->m_fY2)/2;
			pDoc->Get_Rotate_XY(&fx, &fy, fmx - x0, fmy - y0, rotation, mirror);
			fmx = fx + x0 + deltax;
			fmy = fy + y0 + deltay;

			pfill->m_fX1 = fmx - dx/2;
			pfill->m_fY1 = fmy - dy/2;
			pfill->m_fX2 = fmx + dx/2;
			pfill->m_fY2 = fmy + dy/2;
			
			if(mirror == TRUE)
			{
				pfill->m_fRotation = 0 - pfill->m_fRotation;
				pfill->m_fRotation -= rotation;
			}
			else
			{
				pfill->m_fRotation += rotation;
			}
			pfill->m_fRotation = pDoc->Get_Correct_Angle(pfill->m_fRotation);
		}			
	}
	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nComponent != -1)||(pregion->m_nPolygon != -1)) continue;
		if(pregion->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_REGION;
			pundo->m_nIndex = i;
			pundo->pcbregion.Copy(pregion);
			pundo->pcbregion.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			

			for(int j=0; j<pregion->m_cVertex.GetCount(); j++)
			{
				Struct_RegionVertex vertex = pregion->m_cVertex.GetAt(j);
				
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

				pregion->m_cVertex.SetAt(j, vertex);
			}

			for(int j=0; j<pregion->m_arrayHoles.GetCount(); j++)
			{
				CArray<Fpoint, Fpoint>* p_arrayHole = pregion->m_arrayHoles.GetAt(j);
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
	for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
		if(ppolygon->m_nFlag != FLAG_NORMAL) continue;
		if((ppolygon->m_bSelection == TRUE)&&(ppolygon->m_nPolygonType == COPPER_POLYGON))
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_POLYGON;
			pundo->m_nIndex = i;
			pundo->pcbpolygon.Copy(ppolygon);
			pundo->pcbpolygon.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			

			for(int j=0; j<ppolygon->m_cVertex.GetCount(); j++)
			{
				Struct_RegionVertex vertex = ppolygon->m_cVertex.GetAt(j);
				
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

				ppolygon->m_cVertex.SetAt(j, vertex);
			}
			
			if(AfxMessageBox("重新构建覆铜区域?", MB_YESNO|MB_ICONWARNING) == IDYES)
			{
				
				for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
					if((parc->m_nPolygon == i)&&(parc->m_nFlag == FLAG_NORMAL))
					{
						parc->m_bSelection = FALSE;
						parc->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_ARC;
						pundo->m_nIndex = j;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}
				for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
					if((ptrack->m_nPolygon == i)&&(ptrack->m_nFlag == FLAG_NORMAL))
					{
						ptrack->m_bSelection = FALSE;
						ptrack->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = j;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}
				for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
				{
					COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
					if((pregion->m_nPolygon == i)&&(pregion->m_nFlag == FLAG_NORMAL))
					{
						pregion->m_bSelection = FALSE;
						pregion->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_REGION;
						pundo->m_nIndex = j;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}

				
				CreatePolygonPour(ppolygon, i);

			}
			else
			{
				
				for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
					if((parc->m_nPolygon == i)&&(parc->m_nFlag == FLAG_NORMAL))
					{
						parc->MoveAndRotate(this, x0, y0, deltax, deltay, rotation, FALSE, j);
					}
				}
				for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
					if((ptrack->m_nPolygon == i)&&(ptrack->m_nFlag == FLAG_NORMAL))
					{
						ptrack->MoveAndRotate(this, x0, y0, deltax, deltay, rotation, FALSE, j);
					}
				}
				for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
				{
					COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
					if((pregion->m_nPolygon == i)&&(pregion->m_nFlag == FLAG_NORMAL))
					{
						pregion->MoveAndRotate(this, x0, y0, deltax, deltay, rotation, FALSE, j);
					}
				}
			}
		}			
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent != -1)) continue;
		if(ppad->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_PAD;
			pundo->m_nIndex = i;
			pundo->pcbpad.Copy(ppad);
			pundo->pcbpad.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, ppad->m_fCx - x0, ppad->m_fCy - y0, rotation, mirror);
			ppad->m_fCx = fx + x0 + deltax;
			ppad->m_fCy = fy + y0 + deltay;

			if(mirror == TRUE)
			{
				ppad->m_fRotation = 0 - ppad->m_fRotation;
				ppad->m_fRotation -= rotation;
			}
			else
			{
				ppad->m_fRotation += rotation;
			}
			ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
		}				
	}		
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nComponent != -1)) continue;
		if(pvia->m_bSelection == TRUE)
		{
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_VIA;
			pundo->m_nIndex = i;
			pundo->pcbvia.Copy(pvia);
			pundo->pcbvia.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, pvia->m_fCx - x0, pvia->m_fCy - y0, rotation, mirror);
			pvia->m_fCx = fx + x0 + deltax;
			pvia->m_fCy = fy + y0 + deltay;
		}				
	}	
	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag != FLAG_NORMAL) continue;
		if(pcomp->m_bSelection == TRUE)
		{			
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = PCB_ELEMENT_COMP;
			pundo->m_nIndex = i;
			pundo->pcbcomp.Copy(pcomp);
			pundo->pcbcomp.m_nFlag = FLAG_NORMAL;
			
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, pcomp->m_fX - x0, pcomp->m_fY - y0, rotation, mirror);
			pcomp->m_fX = fx + x0 + deltax;
			pcomp->m_fY = fy + y0 + deltay;
			pcomp->m_fRotation += rotation;
			if(mirror == TRUE) pcomp->m_fRotation = 180 - pcomp->m_fRotation;
			pcomp->m_fRotation = pDoc->Get_Correct_Angle(pcomp->m_fRotation);

			for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
				if(ptrack->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = j;
					pundo->pcbtrack.Copy(ptrack);
					pundo->pcbtrack.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ptrack->m_fX1 - x0, ptrack->m_fY1 - y0, rotation, mirror);
					ptrack->m_fX1 = fx + x0 + deltax;
					ptrack->m_fY1 = fy + y0 + deltay;
					pDoc->Get_Rotate_XY(&fx, &fy, ptrack->m_fX2 - x0, ptrack->m_fY2 - y0, rotation, mirror);
					ptrack->m_fX2 = fx + x0 + deltax;
					ptrack->m_fY2 = fy + y0 + deltay;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
				if(parc->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = j;
					pundo->pcbarc.Copy(parc);
					pundo->pcbarc.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, parc->m_fCx - x0, parc->m_fCy - y0, rotation, mirror);
					parc->m_fCx = fx + x0 + deltax;
					parc->m_fCy = fy + y0 + deltay;
							
					double salpha, ealpha;
					pDoc->Get_Rotate_Angle(&salpha, &ealpha, parc->m_fSangle, parc->m_fEangle, rotation, mirror);
					parc->m_fSangle = salpha;
					parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
					parc->m_fEangle = ealpha;
					parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
				if(pfill->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = j;
					pundo->pcbfill.Copy(pfill);
					pundo->pcbfill.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					double fx, fy;
					double fmx, fmy, dx, dy;
					dx = abs(pfill->m_fX1 - pfill->m_fX2);
					dy = abs(pfill->m_fY1 - pfill->m_fY2);
					fmx = (pfill->m_fX1 + pfill->m_fX2)/2;
					fmy = (pfill->m_fY1 + pfill->m_fY2)/2;
					pDoc->Get_Rotate_XY(&fx, &fy, fmx - x0, fmy - y0, rotation, mirror);
					fmx = fx + x0 + deltax;
					fmy = fy + y0 + deltay;

					pfill->m_fX1 = fmx - dx/2;
					pfill->m_fY1 = fmy - dy/2;
					pfill->m_fX2 = fmx + dx/2;
					pfill->m_fY2 = fmy + dy/2;
					
					if(mirror == TRUE)
					{
						pfill->m_fRotation = 0 - pfill->m_fRotation;
						pfill->m_fRotation -= rotation;
					}
					else
					{
						pfill->m_fRotation += rotation;
					}
					pfill->m_fRotation = pDoc->Get_Correct_Angle(pfill->m_fRotation);
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
				if(ppad->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_PAD;
					pundo->m_nIndex = j;
					pundo->pcbpad.Copy(ppad);
					pundo->pcbpad.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ppad->m_fCx - x0, ppad->m_fCy - y0, rotation, mirror);
					ppad->m_fCx = fx + x0 + deltax;
					ppad->m_fCy = fy + y0 + deltay;

					if(mirror == TRUE)
					{
						ppad->m_fRotation = 0 - ppad->m_fRotation;
						ppad->m_fRotation -= rotation;
					}
					else
					{
						ppad->m_fRotation += rotation;
					}
					ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
				if(pvia->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = j;
					pundo->pcbvia.Copy(pvia);
					pundo->pcbvia.m_nFlag = FLAG_NORMAL;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pvia->m_fCx - x0, pvia->m_fCy - y0, rotation, mirror);
					pvia->m_fCx = fx + x0 + deltax;
					pvia->m_fCy = fy + y0 + deltay;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbText.GetCount(); j++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(j);
				if(ptext->m_nComponent == i)
				{
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_CHANGE;
					pundo->m_nOb = PCB_ELEMENT_TEXT;
					pundo->m_nIndex = j;
					pundo->pcbtext.Copy(ptext);
					pundo->pcbtext.m_nFlag = FLAG_NORMAL;
					
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - x0, ptext->m_fSy - y0, rotation, mirror);
					ptext->m_fSx = fx + x0 + deltax;
					ptext->m_fSy = fy + y0 + deltay;

					ptext->m_fRotation += rotation;
					if(mirror == TRUE) 
					{
						if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
						else ptext->m_bMirror = TRUE;
						ptext->m_fRotation = 360 - ptext->m_fRotation;
					}

					ptext->m_fRotation = pDoc->Get_Correct_Angle(ptext->m_fRotation);
					ptext->UpdateStringBorder(this, FALSE);
				}
			}
		}
	}	

	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
	
	Invalidate();
}

void CESDView::PcbCopyAndRotateSelection(double x0, double y0,  double deltax, double deltay, int rotation, BOOL mirror)
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nComponent != -1)||(parc->m_nPolygon != -1)) continue;
		if(parc->m_bSelection == TRUE)
		{
			COb_pcbarc* pnewarc = new COb_pcbarc();
			pnewarc->Copy(parc);
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
	}
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nComponent != -1)||(ptrack->m_nPolygon != -1)) continue;
		if(ptrack->m_bSelection == TRUE)
		{
			COb_pcbtrack* pnewtrack = new COb_pcbtrack();
			pnewtrack->Copy(ptrack);
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
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_nComponent != -1)) continue;
		if(ptext->m_bSelection == TRUE)
		{		
			COb_pcbtext* pnewtext = new COb_pcbtext();
			pnewtext->Copy(ptext);
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
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)) continue;
		if(pfill->m_bSelection == TRUE)
		{
			COb_pcbfill* pnewfill = new COb_pcbfill();
			pnewfill->Copy(pfill);
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
	}
	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nComponent >= 0)||(pregion->m_nPolygon >= 0)) continue;
		if(pregion->m_bSelection == TRUE)
		{
			COb_pcbregion* pnewregion = new COb_pcbregion();
			pnewregion->Copy(pregion);
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
	}
	for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
		if(ppolygon->m_nFlag != FLAG_NORMAL) continue;
		if(ppolygon->m_bSelection == TRUE)
		{
			COb_pcbpolygon* pnewpolygon = new COb_pcbpolygon();
			pnewpolygon->Copy(ppolygon);
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
			undo_flag = TRUE;
			

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

			CreatePolygonPour(pnewpolygon, newindex);
		}			
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent >= 0)) continue;
		if(ppad->m_bSelection == TRUE)
		{
			COb_pcbpad* pnewpad = new COb_pcbpad();
			pnewpad->Copy(ppad);
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
	}		
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nComponent >= 0)) continue;
		if(pvia->m_bSelection == TRUE)
		{
			COb_pcbvia* pnewvia = new COb_pcbvia();
			pnewvia->Copy(pvia);
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
	}	
	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag != FLAG_NORMAL) continue;
		if(pcomp->m_bSelection == TRUE)
		{		
			COb_pcbcomp* pnewcomp = new COb_pcbcomp();
			pnewcomp->Copy(pcomp);
			int newindex = pDoc->m_arrayPcbComp.Add(pnewcomp);
			
			if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_COMP;
			pundo->m_nIndex = newindex;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, pnewcomp->m_fX - x0, pnewcomp->m_fY - y0, rotation, mirror);
			pnewcomp->m_fX = fx + x0 + deltax;
			pnewcomp->m_fY = fy + y0 + deltay;
			pnewcomp->m_fRotation += rotation;
			if(mirror == TRUE) pnewcomp->m_fRotation = 180 - pnewcomp->m_fRotation;
			pnewcomp->m_fRotation = pDoc->Get_Correct_Angle(pnewcomp->m_fRotation);

			for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
				if(ptrack->m_nComponent == i)
				{
					COb_pcbtrack* pnewtrack = new COb_pcbtrack();
					pnewtrack->Copy(ptrack);
					pnewtrack->m_nComponent = newindex;
                    int newsubindex = pDoc->m_arrayPcbTrack.Add(pnewtrack);
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = newsubindex;
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
			}
			for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
				if(parc->m_nComponent == i)
				{
					COb_pcbarc* pnewarc = new COb_pcbarc();
					pnewarc->Copy(parc);
					pnewarc->m_nComponent = newindex;
					int newsubindex = pDoc->m_arrayPcbArc.Add(pnewarc);

					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = newsubindex;
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
			}
			for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
				if(pfill->m_nComponent == i)
				{
					COb_pcbfill* pnewfill = new COb_pcbfill();
					pnewfill->Copy(pfill);
					pnewfill->m_nComponent = newindex;
					int newsubindex = pDoc->m_arrayPcbFill.Add(pnewfill);
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = newsubindex;
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
			}
			for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
				if(ppad->m_nComponent == i)
				{
					COb_pcbpad* pnewpad = new COb_pcbpad();
					pnewpad->Copy(ppad);
					pnewpad->m_nComponent = newindex;
					int newsubindex = pDoc->m_arrayPcbPad.Add(pnewpad);
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_PAD;
					pundo->m_nIndex = newsubindex;
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
			}
			for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
				if(pvia->m_nComponent == i)
				{
					COb_pcbvia* pnewvia = new COb_pcbvia();
					pnewvia->Copy(pvia);
					pnewvia->m_nComponent = newindex;
					int newsubindex = pDoc->m_arrayPcbVia.Add(pnewvia);
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = newsubindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					undo_flag = TRUE;
					
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pnewvia->m_fCx - x0, pnewvia->m_fCy - y0, rotation, mirror);
					pnewvia->m_fCx = fx + x0 + deltax;
					pnewvia->m_fCy = fy + y0 + deltay;
				}
			}
			for(int j=0; j<pDoc->m_arrayPcbText.GetCount(); j++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(j);
				if(ptext->m_nComponent == i)
				{
					COb_pcbtext* pnewtext = new COb_pcbtext();
					pnewtext->Copy(ptext);
					pnewtext->m_nComponent = newindex;
					int newsubindex = pDoc->m_arrayPcbText.Add(pnewtext);
					
					if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
					{
						pDoc->Clear_Discarded_Undo_Buffer();
					}
					clear_undo_buffer_flag = TRUE;
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_TEXT;
					pundo->m_nIndex = newsubindex;
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
			}
		}
	}	

	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
	
	Invalidate();
}

void CESDView::PcbMoveSelection(void)
{
	int ch1;
	int flag;
	BOOL mirror;
	double x0, y0, x1, y1, deltax, deltay;

	char szMoveText[] = "移动选择: <左键><回车>放置所有移动图形, <空格>旋转, <X键>镜像, <右键><ESC>结束命令";
	
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
	
	int compflag = 0;
	for(int i=0; i<pDoc->m_arrayPcbSelection.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selection = pDoc->m_arrayPcbSelection.GetAt(i);
		if(selection.pcbelement == PCB_ELEMENT_COMP) 
		{
			compflag = 1;
			break;
		}
	}

	if(pDoc->m_arrayPcbSelection.GetCount() == 0) return;

	pDoc->SetModifiedFlag(TRUE);

	
	PcbProcessingSelection(TRUE);

	x0 = m_fCurrentSnapX;
	y0 = m_fCurrentSnapY;

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);	

	
	COb_pcbfill afill;
	afill.pDocument = pDoc;
	afill.m_fX1 = pDoc->m_fPcbSelection_X1;
	afill.m_fY1 = pDoc->m_fPcbSelection_Y1;
	afill.m_fX2 = pDoc->m_fPcbSelection_X2;
	afill.m_fY2 = pDoc->m_fPcbSelection_Y2;
	afill.m_nLayer = TOPLAYER;
	afill.m_fRotation = 0;

	CloseMouseCursor();
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
				PcbProcessingSelection(FALSE);
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
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X1 - x0, pDoc->m_fPcbSelection_Y1 - y0, flag*90, mirror);
				afill.m_fX1 = fx + x0 + deltax;
				afill.m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X2 - x0, pDoc->m_fPcbSelection_Y2 - y0, flag*90, mirror);
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
					
				PcbProcessingSelection(FALSE);
				PcbMoveAndRotateSelection( x0, y0, deltax, deltay, flag*90, mirror);
				
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
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X1 - x0, pDoc->m_fPcbSelection_Y1 - y0, flag*90, mirror);
				afill.m_fX1 = fx + x0 + deltax;
				afill.m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X2 - x0, pDoc->m_fPcbSelection_Y2 - y0, flag*90, mirror);
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
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X1 - x0, pDoc->m_fPcbSelection_Y1 - y0, flag*90, mirror);
				afill.m_fX1 = fx + x0 + deltax;
				afill.m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X2 - x0, pDoc->m_fPcbSelection_Y2 - y0, flag*90, mirror);
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

	pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
	Invalidate();
}

void CESDView::OnPcbEditDuplicate()
{TRACE("OnPcbEditDuplicate\n");
	int ch1;
	int flag;
	BOOL mirror;
	double x0, y0, x1, y1, deltax, deltay;

	char szMoveText[] = "复制选择: <左键><回车>放置所有移动图形, <空格>旋转, <X键>镜像, <右键><ESC>结束命令";
	
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
	
	int compflag = 0;
	for(int i=0; i<pDoc->m_arrayPcbSelection.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selection = pDoc->m_arrayPcbSelection.GetAt(i);
		if(selection.pcbelement == PCB_ELEMENT_COMP) 
		{
			compflag = 1;
			break;
		}
	}

	if(pDoc->m_arrayPcbSelection.GetCount() == 0) return;

	pDoc->SetModifiedFlag(TRUE);

	
	//PcbProcessingSelection(TRUE);

	x0 = pDoc->m_fPcbSelection_X1;
	y0 = pDoc->m_fPcbSelection_Y1;
	m_fCurrentSnapX = x0;
    m_fCurrentSnapY = y0;
	Restore_LargeCrossXY();
	//x0 = m_fCurrentSnapX;
	//y0 = m_fCurrentSnapY;

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);	

	
	COb_pcbfill afill;
	afill.pDocument = pDoc;
	afill.m_fX1 = pDoc->m_fPcbSelection_X1;
	afill.m_fY1 = pDoc->m_fPcbSelection_Y1;
	afill.m_fX2 = pDoc->m_fPcbSelection_X2;
	afill.m_fY2 = pDoc->m_fPcbSelection_Y2;
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
				//PcbProcessingSelection(FALSE);
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
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X1 - x0, pDoc->m_fPcbSelection_Y1 - y0, flag*90, mirror);
				afill.m_fX1 = fx + x0 + deltax;
				afill.m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X2 - x0, pDoc->m_fPcbSelection_Y2 - y0, flag*90, mirror);
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
					
				//PcbProcessingSelection(FALSE);
				PcbCopyAndRotateSelection( x0, y0, deltax, deltay, flag*90, mirror);
				
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
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X1 - x0, pDoc->m_fPcbSelection_Y1 - y0, flag*90, mirror);
				afill.m_fX1 = fx + x0 + deltax;
				afill.m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X2 - x0, pDoc->m_fPcbSelection_Y2 - y0, flag*90, mirror);
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
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X1 - x0, pDoc->m_fPcbSelection_Y1 - y0, flag*90, mirror);
				afill.m_fX1 = fx + x0 + deltax;
				afill.m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X2 - x0, pDoc->m_fPcbSelection_Y2 - y0, flag*90, mirror);
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

	pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
	Invalidate();
}

void CESDView::OnPcbEditMoveSelection()
{
	int ch1;
	int flag;
	BOOL mirror;
	double x0, y0, x1, y1, deltax, deltay;

	char szHelp[] = "移动选择: <左键><回车>选择基准点移动, <右键><ESC>结束命令";
	char szMoveText[] = "移动选择: <左键><回车>放置所有移动图形, <空格>旋转, <X键>镜像, <右键><ESC>结束命令";
	
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
	if(pDoc->m_arrayPcbSelection.GetCount() == 0) return;

	Write_Help(szHelp);
	do
	{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
			
			int compflag = 0;
			for(int i=0; i<pDoc->m_arrayPcbSelection.GetCount(); i++)
			{
				Struct_Pcb_Multi_Selection selection = pDoc->m_arrayPcbSelection.GetAt(i);
				if(selection.pcbelement == PCB_ELEMENT_COMP) 
				{
					compflag = 1;
					break;
				}
			}

			if(pDoc->m_arrayPcbSelection.GetCount() == 0) return;

			
			PcbProcessingSelection(TRUE);

			x0 = m_fCurrentSnapX;
			y0 = m_fCurrentSnapY;

			int drawmode = dc.GetROP2();
			dc.SetROP2(R2_NOTXORPEN);	

			
			COb_pcbfill afill;
			afill.pDocument = pDoc;
			afill.m_fX1 = pDoc->m_fPcbSelection_X1;
			afill.m_fY1 = pDoc->m_fPcbSelection_Y1;
			afill.m_fX2 = pDoc->m_fPcbSelection_X2;
			afill.m_fY2 = pDoc->m_fPcbSelection_Y2;
			afill.m_nLayer = TOPLAYER;
			afill.m_fRotation = 0;

			CloseMouseCursor();
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
					PcbProcessingSelection(FALSE);
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
					pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X1 - x0, pDoc->m_fPcbSelection_Y1 - y0, flag*90, mirror);
					afill.m_fX1 = fx + x0 + deltax;
					afill.m_fY1 = fy + y0 + deltay;
					pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X2 - x0, pDoc->m_fPcbSelection_Y2 - y0, flag*90, mirror);
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
							
					PcbProcessingSelection(FALSE);
					PcbMoveAndRotateSelection( x0, y0, deltax, deltay, flag*90, mirror);
						
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
					pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X1 - x0, pDoc->m_fPcbSelection_Y1 - y0, flag*90, mirror);
					afill.m_fX1 = fx + x0 + deltax;
					afill.m_fY1 = fy + y0 + deltay;
					pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X2 - x0, pDoc->m_fPcbSelection_Y2 - y0, flag*90, mirror);
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
					pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X1 - x0, pDoc->m_fPcbSelection_Y1 - y0, flag*90, mirror);
					afill.m_fX1 = fx + x0 + deltax;
					afill.m_fY1 = fy + y0 + deltay;
					pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fPcbSelection_X2 - x0, pDoc->m_fPcbSelection_Y2 - y0, flag*90, mirror);
					afill.m_fX2 = fx + x0 + deltax;
					afill.m_fY2 = fy + y0 + deltay;

					CloseMouseCursor();
					afill.DrawFrame(&dc, this);
					OpenMouseCursor();
				}
			}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
			break;
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;

	pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
	Invalidate();
}