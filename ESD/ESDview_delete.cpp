void CESDView::DeleteElement(int element, POSITION position)
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	if((element == 0)||( position == NULL)) return;
	pDoc->SetModifiedFlag(TRUE);

	pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(position);

	if(element == SCH_ELEMENT_COMPONENT)
	{
		COb_schcomp* pcomp = (COb_schcomp*)pobject;
		pcomp->m_bSelection = FALSE;
		pcomp->m_nFlag = FLAG_DELETED;
				
		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_COMPONENT;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_TEXT)  
	{
		COb_schtext* ptext = (COb_schtext*)pobject;
		if( (ptext->m_nID != TEXT_SHEETSYMBOLDESIGNATOR) && (ptext->m_nID != TEXT_SHEETSYMBOLFILENAME) && (ptext->m_nID != TEXT_COMPDESIGNATOR)  &&\
			(ptext->m_nID != TEXT_COMPCOMMENT) && (ptext->m_nID != TEXT_COMPPACKAGE) )
		{
			ptext->m_bSelection = FALSE;
			ptext->m_nFlag = FLAG_DELETED;

			
			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;

			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = SCH_ELEMENT_TEXT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = position;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
		}
	}
	else if(element == SCH_ELEMENT_BEZIER)
	{
		COb_schbezier* pbezier = (COb_schbezier*)pobject;
		pbezier->m_bSelection = FALSE;
		pbezier->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_BEZIER;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_POLYLINE)
	{
		COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
		ppolyline->m_bSelection = FALSE;
		ppolyline->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_POLYLINE;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_POLYGON)
	{
		COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
		ppolygon->m_bSelection = FALSE;
		ppolygon->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_POLYGON;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_ELLIPSEFILL)
	{
		COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
		pellipsefill->m_bSelection = FALSE;	
		pellipsefill->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_PIE)
	{
		COb_schpie* ppie = (COb_schpie*)pobject;
		ppie->m_bSelection = FALSE;
		ppie->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_PIE;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_ROUNDRECT)
	{
		COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
		proundrect->m_bSelection = FALSE;
		proundrect->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_ELLIPSE)
	{
		COb_schellipse* pellipse = (COb_schellipse*)pobject;
		pellipse->m_bSelection = FALSE;
		pellipse->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_ARC)
	{
		COb_scharc* parc = (COb_scharc*)pobject;
		parc->m_bSelection = FALSE;
		parc->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_ARC;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_LINE)	
	{
		COb_schline* pline = (COb_schline*)pobject;
		pline->m_bSelection = FALSE;
		pline->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_LINE;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_RECT)
	{
		COb_schrect* prect = (COb_schrect*)pobject;
		prect->m_bSelection = FALSE;
		prect->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_RECT;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_SHEETSYMBOL)
	{
		COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
		psheetsymbol->m_bSelection = FALSE;
		psheetsymbol->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_SHEETENTRY)
	{
		COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
		psheetentry->m_bSelection = FALSE;
		psheetentry->m_nFlag = FLAG_DELETED;
					
		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;	
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_SHEETENTRY;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_POWER)
	{
		COb_schpower* ppower = (COb_schpower*)pobject;
		ppower->m_bSelection = FALSE;
		ppower->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_POWER;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_PORT)
	{
		COb_schport* pport = (COb_schport*)pobject;
		pport->m_bSelection = FALSE;
		pport->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_PORT;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_TEXTFRAME)
	{
		COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
		ptextframe->m_bSelection = FALSE;
		ptextframe->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_JUNCTION)
	{
		COb_schjunc* pjunc = (COb_schjunc*)pobject;
		pjunc->m_bSelection = FALSE;
		pjunc->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;

		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_JUNCTION;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	else if(element == SCH_ELEMENT_GRAPHIC)
	{
		COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
		pgraphic->m_bSelection = FALSE;
		pgraphic->m_nFlag = FLAG_DELETED;

		
		if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CSchUndo *pundo = new CSchUndo;
		pundo->m_nOp = UNDO_UNDELETE;
		pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
		pundo->m_nDiagram = m_nDiagram;
		pundo->m_nPos = position;
		pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	
	
	if(undo_flag == TRUE)
	{
		AutoRemoveJunction();

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}

	Invalidate();
}

void CESDView::PcbDeleteElement(int pcbelement, int index)
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	if((pcbelement == 0)||( index < 0)) return;
	pDoc->SetModifiedFlag(TRUE);

	if(pcbelement == PCB_ELEMENT_TRACK)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(index);
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
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	if(pcbelement == PCB_ELEMENT_ARC)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(index);
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
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	if(pcbelement == PCB_ELEMENT_TEXT)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(index);
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
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	if(pcbelement == PCB_ELEMENT_FILL)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(index);
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
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	if(pcbelement == PCB_ELEMENT_PAD)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(index);
		if(ppad->m_nComponent < 0)
		{
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
			pundo->m_nIndex = index;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
		}
		else
		{
			int compindex = ppad->m_nComponent;
			COb_pcbcomp* pcomp =  pDoc->m_arrayPcbComp.GetAt(compindex);
			if(pcomp != NULL)
			{
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
				pundo->m_nIndex = compindex;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				

				
				for(int subindex=0; subindex<pDoc->m_arrayPcbTrack.GetCount(); subindex++)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(subindex);
					if(ptrack->m_nComponent == compindex)
					{
						ptrack->m_bSelection = FALSE;
						ptrack->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = subindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}
				for(int subindex=0; subindex<pDoc->m_arrayPcbArc.GetCount(); subindex++)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(subindex);
					if(parc->m_nComponent == compindex)
					{
						parc->m_bSelection = FALSE;
						parc->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_ARC;
						pundo->m_nIndex = subindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}
				for(int subindex=0; subindex<pDoc->m_arrayPcbText.GetCount(); subindex++)
				{
					COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(subindex);
					if(ptext->m_nComponent == compindex)
					{
						ptext->m_bSelection = FALSE;
						ptext->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_TEXT;
						pundo->m_nIndex = subindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}
				for(int subindex=0; subindex<pDoc->m_arrayPcbFill.GetCount(); subindex++)
				{
					COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(subindex);
					if(pfill->m_nComponent == compindex)
					{
						pfill->m_bSelection = FALSE;
						pfill->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_FILL;
						pundo->m_nIndex = subindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}
				for(int subindex=0; subindex<pDoc->m_arrayPcbPad.GetCount(); subindex++)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(subindex);
					if(ppad->m_nComponent == compindex)
					{
						ppad->m_bSelection = FALSE;
						ppad->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_PAD;
						pundo->m_nIndex = subindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}
				for(int subindex=0; subindex<pDoc->m_arrayPcbRegion.GetCount(); subindex++)
				{
					COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(subindex);
					if(pregion->m_nComponent == compindex)
					{
						pregion->m_bSelection = FALSE;
						pregion->m_nFlag = FLAG_DELETED;
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_UNDELETE;
						pundo->m_nOb = PCB_ELEMENT_REGION;
						pundo->m_nIndex = subindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
					}
				}
			}
		}
	}
	if(pcbelement == PCB_ELEMENT_VIA)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(index);
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
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	if(pcbelement == PCB_ELEMENT_REGION)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(index);
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
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	if(pcbelement == PCB_ELEMENT_POLYGON)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(index);
		if(ppolygon->m_nPolygonType == COPPER_POLYGON)
		{
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
			pundo->m_nIndex = index;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			
			
			for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
				if(parc->m_nPolygon == index)
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
				if(ptrack->m_nPolygon == index)
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
				if(pregion->m_nPolygon == index)
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
	}
	if(pcbelement == PCB_ELEMENT_COORDINATE)
	{
		COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(index);
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
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	if(pcbelement == PCB_ELEMENT_DIMENSION)
	{
		COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(index);
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
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		undo_flag = TRUE;
		
	}
	if(pcbelement == PCB_ELEMENT_COMP)
	{
		COb_pcbcomp* pcomp =  pDoc->m_arrayPcbComp.GetAt(index);
		int compindex =index;
		if(pcomp != NULL)
		{
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
			pundo->m_nIndex = compindex;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			

			
			for(int subindex=0; subindex<pDoc->m_arrayPcbTrack.GetCount(); subindex++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(subindex);
				if(ptrack->m_nComponent == compindex)
				{
					ptrack->m_bSelection = FALSE;
					ptrack->m_nFlag = FLAG_DELETED;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = subindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
				}
			}
			for(int subindex=0; subindex<pDoc->m_arrayPcbArc.GetCount(); subindex++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(subindex);
				if(parc->m_nComponent == compindex)
				{
					parc->m_bSelection = FALSE;
					parc->m_nFlag = FLAG_DELETED;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = subindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
				}
			}
			for(int subindex=0; subindex<pDoc->m_arrayPcbText.GetCount(); subindex++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(subindex);
				if(ptext->m_nComponent == compindex)
				{
					ptext->m_bSelection = FALSE;
					ptext->m_nFlag = FLAG_DELETED;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_TEXT;
					pundo->m_nIndex = subindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
				}
			}
			for(int subindex=0; subindex<pDoc->m_arrayPcbFill.GetCount(); subindex++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(subindex);
				if(pfill->m_nComponent == compindex)
				{
					pfill->m_bSelection = FALSE;
					pfill->m_nFlag = FLAG_DELETED;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = subindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
				}
			}
			for(int subindex=0; subindex<pDoc->m_arrayPcbPad.GetCount(); subindex++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(subindex);
				if(ppad->m_nComponent == compindex)
				{
					ppad->m_bSelection = FALSE;
					ppad->m_nFlag = FLAG_DELETED;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_PAD;
					pundo->m_nIndex = subindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
				}
			}
			for(int subindex=0; subindex<pDoc->m_arrayPcbVia.GetCount(); subindex++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(subindex);
				if(pvia->m_nComponent == compindex)
				{
					pvia->m_bSelection = FALSE;
					pvia->m_nFlag = FLAG_DELETED;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_VIA;
					pundo->m_nIndex = subindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
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

void CESDView::DeleteSelection(void)
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	POSITION posbak;
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
	POSITION pos;
	pos = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{	
		if(pos == NULL) break;
		posbak = pos;
		pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram != m_nDiagram)||(pcomp->m_nFlag == FLAG_DELETED))	
			{
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);

				continue;
			}
			if(pcomp->m_bSelection == TRUE)
			{
				pcomp->m_bSelection = FALSE;
				pcomp->m_nFlag = FLAG_DELETED;
				
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_COMPONENT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				

				for(int k=0; k<pcomp->m_arrayPos.GetCount(); k++)
				{
					POSITION subpos = pcomp->m_arrayPos.GetAt(k);
					pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						ptext->m_bSelection = FALSE;
					}
				}
			}
			
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if(ptext->m_bSelection == TRUE)
			{
				ptext->m_bSelection = FALSE;
				ptext->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if(pbezier->m_bSelection == TRUE)
			{
				pbezier->m_bSelection = FALSE;
				pbezier->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_BEZIER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			if(ppolyline->m_bSelection == TRUE)
			{
				ppolyline->m_bSelection = FALSE;
				ppolyline->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_POLYLINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if(ppolygon->m_bSelection == TRUE)
			{
				ppolygon->m_bSelection = FALSE;
				ppolygon->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_POLYGON;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if(pellipsefill->m_bSelection == TRUE)
			{
				pellipsefill->m_bSelection = FALSE;	
				pellipsefill->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if(ppie->m_bSelection == TRUE)
			{
				ppie->m_bSelection = FALSE;
				ppie->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_PIE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if(proundrect->m_bSelection == TRUE)
			{
				proundrect->m_bSelection = FALSE;
				proundrect->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if(pellipse->m_bSelection == TRUE)
			{
				pellipse->m_bSelection = FALSE;
				pellipse->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if(parc->m_bSelection == TRUE)
			{
				parc->m_bSelection = FALSE;
				parc->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_ARC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
			if( pline->m_bSelection == TRUE)
			{
				pline->m_bSelection = FALSE;
				pline->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_LINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if(prect->m_bSelection == TRUE)
			{
				prect->m_bSelection = FALSE;
				prect->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_RECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
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
			if(psheetsymbol->m_bSelection == TRUE)
			{
				psheetsymbol->m_bSelection = FALSE;
				psheetsymbol->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
			}
			else 
			{
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) 
				{
					posbak = pos;
					pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
					{
						COb_schsheetentry* psheetentry;
						psheetentry = (COb_schsheetentry*)pobject;
						if((psheetentry->m_nDiagram != m_nDiagram)||(psheetentry->m_nFlag == FLAG_DELETED))	continue;
						
						if(psheetentry->m_bSelection == TRUE)
						{
							psheetentry->m_bSelection = FALSE;
							psheetentry->m_nFlag = FLAG_DELETED;
						
							
							if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CSchUndo *pundo = new CSchUndo;
							pundo->m_nOp = UNDO_UNDELETE;
							pundo->m_nOb = SCH_ELEMENT_SHEETENTRY;
							pundo->m_nDiagram = m_nDiagram;
							pundo->m_nPos = posbak;
							pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							
						}
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			if(ppower->m_bSelection == TRUE)
			{
				ppower->m_bSelection = FALSE;
				ppower->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_POWER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(pport->m_bSelection == TRUE)
			{
				pport->m_bSelection = FALSE;
				pport->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_PORT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
			if(ptextframe->m_bSelection == TRUE)
			{
				ptextframe->m_bSelection = FALSE;
				ptextframe->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
			if(pjunc->m_bSelection == TRUE)
			{
				pjunc->m_bSelection = FALSE;
				pjunc->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_JUNCTION;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			if(pgraphic->m_bSelection == TRUE)
			{
				pgraphic->m_bSelection = FALSE;
				pgraphic->m_nFlag = FLAG_DELETED;

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
			}
		}
	}
	
	if(undo_flag == TRUE)
	{
		pDoc->SetModifiedFlag(TRUE);
		AutoRemoveJunction();

		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}	
	
	pDoc->m_arraySelection[m_nDiagram].RemoveAll();
	Invalidate();
}

void CESDView::PcbDeleteSelection(void)
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
	}
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nComponent != -1)||(ptrack->m_nPolygon != -1)) continue;
		if(ptrack->m_bSelection == TRUE)
		{
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
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_nComponent != -1)) continue;
		if(ptext->m_bSelection == TRUE)
		{			
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
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent != -1)) continue;
		if(pfill->m_bSelection == TRUE)
		{
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
	}
	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nComponent != -1)||(pregion->m_nPolygon != -1)) continue;
		if(pregion->m_bSelection == TRUE)
		{
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
	}
	for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
		if((ppolygon->m_nFlag != FLAG_NORMAL)||(ppolygon->m_nPolygonType != COPPER_POLYGON)) continue;
		if(ppolygon->m_bSelection == TRUE)
		{
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
				if(parc->m_nPolygon == i)
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
				if(ptrack->m_nPolygon == i)
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
				if(pregion->m_nPolygon == i)
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
		}			
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent != -1)) continue;
		if(ppad->m_bSelection == TRUE)
		{
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
	}		
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nComponent != -1)) continue;
		if(pvia->m_bSelection == TRUE)
		{
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
	}	
	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag != FLAG_NORMAL) continue;
		if(pcomp->m_bSelection == TRUE)
		{			
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
			

			
			for(int subindex=0; subindex<pDoc->m_arrayPcbTrack.GetCount(); subindex++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(subindex);
				if(ptrack->m_nComponent == i)
				{
					ptrack->m_bSelection = FALSE;
					ptrack->m_nFlag = FLAG_DELETED;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = subindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
				}
			}
			for(int subindex=0; subindex<pDoc->m_arrayPcbArc.GetCount(); subindex++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(subindex);
				if(parc->m_nComponent == i)
				{
					parc->m_bSelection = FALSE;
					parc->m_nFlag = FLAG_DELETED;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_ARC;
					pundo->m_nIndex = subindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
				}
			}
			for(int subindex=0; subindex<pDoc->m_arrayPcbText.GetCount(); subindex++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(subindex);
				if(ptext->m_nComponent == i)
				{
					ptext->m_bSelection = FALSE;
					ptext->m_nFlag = FLAG_DELETED;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_TEXT;
					pundo->m_nIndex = subindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
				}
			}
			for(int subindex=0; subindex<pDoc->m_arrayPcbFill.GetCount(); subindex++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(subindex);
				if(pfill->m_nComponent == i)
				{
					pfill->m_bSelection = FALSE;
					pfill->m_nFlag = FLAG_DELETED;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_FILL;
					pundo->m_nIndex = subindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
				}
			}
			for(int subindex=0; subindex<pDoc->m_arrayPcbPad.GetCount(); subindex++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(subindex);
				if(ppad->m_nComponent == i)
				{
					ppad->m_bSelection = FALSE;
					ppad->m_nFlag = FLAG_DELETED;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_PAD;
					pundo->m_nIndex = subindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
				}
			}
			for(int subindex=0; subindex<pDoc->m_arrayPcbRegion.GetCount(); subindex++)
			{
				COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(subindex);
				if(pregion->m_nComponent == i)
				{
					pregion->m_bSelection = FALSE;
					pregion->m_nFlag = FLAG_DELETED;
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_UNDELETE;
					pundo->m_nOb = PCB_ELEMENT_REGION;
					pundo->m_nIndex = subindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					
				}
			}
		}
	}	
	for(int i=0; i<pDoc->m_arrayPcbCoordinate.GetCount(); i++)
	{
		COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(i);
		if(pcoordinate->m_nFlag != FLAG_NORMAL) continue;
		if(pcoordinate->m_bSelection == TRUE)
		{			
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
	}	
	for(int i=0; i<pDoc->m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(i);
		if(pdimension->m_nFlag != FLAG_NORMAL) continue;
		if(pdimension->m_bSelection == TRUE)
		{
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
	}

	//Undo
	if(undo_flag == TRUE)
	{
		pDoc->SetModifiedFlag(TRUE);

		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
	
	pDoc->m_arrayPcbSelection.RemoveAll();
	Invalidate();
}