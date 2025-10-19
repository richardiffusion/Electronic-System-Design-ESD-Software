void CESDView::MoveElement(int element, POSITION position)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	if((element == 0)||( position == NULL)) return;

	pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(position);

	if(element == SCH_ELEMENT_COMPONENT)
	{
		COb_schcomp* pcomp = (COb_schcomp*)pobject;
		pcomp->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_PIN)
	{
		COb_schpin* ppin = (COb_schpin*)pobject;
		ppin->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_IEEE)
	{
		COb_schieee* pieee = (COb_schieee*)pobject;
		
	}
	else if(element == SCH_ELEMENT_TEXT)  
	{
		COb_schtext* ptext = (COb_schtext*)pobject;
		ptext->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_BEZIER)
	{
		COb_schbezier* pbezier = (COb_schbezier*)pobject;
		pbezier->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_POLYLINE)
	{
		COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
		ppolyline->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_POLYGON)
	{
		COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
		ppolygon->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_ELLIPSEFILL)
	{
		COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
		pellipsefill->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_PIE)
	{
		COb_schpie* ppie = (COb_schpie*)pobject;
		ppie->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_ROUNDRECT)
	{
		COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
		proundrect->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_ELLIPSE)
	{
		COb_schellipse* pellipse = (COb_schellipse*)pobject;
		pellipse->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_ARC)
	{
		COb_scharc* parc = (COb_scharc*)pobject;
		parc->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_LINE)	
	{
		COb_schline* pline = (COb_schline*)pobject;
		pline->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_RECT)
	{
		COb_schrect* prect = (COb_schrect*)pobject;
		prect->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_SHEETSYMBOL)
	{
		COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
		psheetsymbol->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_SHEETENTRY)
	{
		COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
		psheetentry->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_POWER)
	{
		COb_schpower* ppower = (COb_schpower*)pobject;
		ppower->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_PORT)
	{
		COb_schport* pport = (COb_schport*)pobject;
		pport->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_TEXTFRAME)
	{
		COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
		ptextframe->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_JUNCTION)
	{
		COb_schjunc* pjunc = (COb_schjunc*)pobject;
		pjunc->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_GRAPHIC)
	{
		COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
		pgraphic->Move(&dc, this, position);
	}
	else if(element == SCH_ELEMENT_SHEET)
	{
		
	}
}

void CESDView::CopyElement(int element, POSITION position)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	if((element == 0)||( position == NULL)) return;

	pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(position);

	if(element == SCH_ELEMENT_COMPONENT)
	{
		COb_schcomp* pcomp = (COb_schcomp*)pobject;

		COb_schcomp* pnewcomp = new COb_schcomp();
		pnewcomp->Copy(pcomp);
		pnewcomp->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewcomp);
						
		pnewcomp->m_arrayPos.RemoveAll();
		
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
				
				POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpin);

				
				pnewcomp->m_arrayPos.Add(pos1);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
			{
				COb_schieee* pieee = (COb_schieee*)pobject;
			
				COb_schieee* pnewieee = new COb_schieee();
				pnewieee->Copy(pieee);
				pnewieee->m_posParent = newpos;
				
				POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewieee);

				
				pnewcomp->m_arrayPos.Add(pos1);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
						
				COb_schtext* pnewtext = new COb_schtext();
				pnewtext->Copy(ptext);
				pnewtext->m_posParent = newpos;
				pnewtext->m_bSelection = FALSE;
				
				POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtext);

				
				pnewcomp->m_arrayPos.Add(pos1);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
			{
				COb_schbezier* pbezier = (COb_schbezier*)pobject;
						
				COb_schbezier* pnewbezier = new COb_schbezier();
				pnewbezier->Copy(pbezier);
				pnewbezier->m_posParent = newpos;
				
				POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewbezier);

				
				pnewcomp->m_arrayPos.Add(pos1);				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
			{
				COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;

				COb_schpolyline* pnewpolyline = new COb_schpolyline();
				pnewpolyline->Copy(ppolyline);
				pnewpolyline->m_posParent = newpos;
				
				POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolyline);

				
				pnewcomp->m_arrayPos.Add(pos1);				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
			{
				COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
				
				COb_schpolygon* pnewpolygon = new COb_schpolygon();
				pnewpolygon->Copy(ppolygon);
				pnewpolygon->m_posParent = newpos;
				
				POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolygon);

				
				pnewcomp->m_arrayPos.Add(pos1);					
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
			{
				COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
						
				COb_schellipsefill* pnewellipsefill = new COb_schellipsefill();
				pnewellipsefill->Copy(pellipsefill);
				pnewellipsefill->m_posParent = newpos;
				
				POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipsefill);

				
				pnewcomp->m_arrayPos.Add(pos1);	
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
			{
				COb_schpie* ppie = (COb_schpie*)pobject;
				
				COb_schpie* pnewpie = new COb_schpie();
				pnewpie->Copy(ppie);
				pnewpie->m_posParent = newpos;
				
				POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpie);

				
				pnewcomp->m_arrayPos.Add(pos1);	
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
			{
				COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
				
				COb_schroundrect* pnewroundrect = new COb_schroundrect();
				pnewroundrect->Copy(proundrect);
				pnewroundrect->m_posParent = newpos;
				
				POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewroundrect);

				
				pnewcomp->m_arrayPos.Add(pos1);	
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
			{
				COb_schellipse* pellipse = (COb_schellipse*)pobject;
				
				COb_schellipse* pnewellipse = new COb_schellipse();
				pnewellipse->Copy(pellipse);
				pnewellipse->m_posParent = newpos;
				
				POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipse);

				
				pnewcomp->m_arrayPos.Add(pos1);	
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
			{
				COb_scharc* parc = (COb_scharc*)pobject;

				COb_scharc* pnewarc = new COb_scharc();
				pnewarc->Copy(parc);
				pnewarc->m_posParent = newpos;
				
				POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewarc);

				
				pnewcomp->m_arrayPos.Add(pos1);	
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
			{
				COb_schrect* prect = (COb_schrect*)pobject;
						
				COb_schrect* pnewrect = new COb_schrect();
				pnewrect->Copy(prect);
				pnewrect->m_posParent = newpos;
				
				POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewrect);

				
				pnewcomp->m_arrayPos.Add(pos1);	
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
			{
				COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
						
				COb_schtextframe* pnewtextframe = new COb_schtextframe();
				pnewtextframe->Copy(ptextframe);
				pnewtextframe->m_posParent = newpos;
				
				POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtextframe);

				
				pnewcomp->m_arrayPos.Add(pos1);	
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
			{
				COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
						
				COb_schgraphic* pnewgraphic = new COb_schgraphic();
				pnewgraphic->Copy(pgraphic);
				pnewgraphic->m_posParent = newpos;
				
				POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewgraphic);

				
				pnewcomp->m_arrayPos.Add(pos1);	
			}
		}
		if(pnewcomp->MovePlaceComp(&dc, this) == FALSE)
		{
			
			for(int k=0; k<pnewcomp->m_arrayPos.GetCount(); k++)
			{
				delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
			}
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_COMPONENT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

			if(bEDA_AutoJunction == TRUE) AutoPlaceJunction();

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			

			pnewcomp->Draw(&dc, this);
		}
	}
	else if(element == SCH_ELEMENT_TEXT)  
	{
		COb_schtext* ptext = (COb_schtext*)pobject;
		if((ptext->m_nID == TEXT_TEXT) || (ptext->m_nID == TEXT_NETLABEL))
		{
			COb_schtext* pnewtext = new COb_schtext();
			pnewtext->Copy(ptext);
			pnewtext->m_bSelection = FALSE;
			POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtext);
			
			if(pnewtext->MovePlaced(&dc, this) == FALSE)
			{
				
				delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
			}
			else
			{
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = newpos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}	
		}
	}
	else if(element == SCH_ELEMENT_BEZIER)
	{
		COb_schbezier* pbezier = (COb_schbezier*)pobject;
				
		COb_schbezier* pnewbezier = new COb_schbezier();
		pnewbezier->Copy(pbezier);
		pnewbezier->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewbezier);
						
		if(pnewbezier->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_BEZIER;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}
	}
	else if(element == SCH_ELEMENT_POLYLINE)
	{
		COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
				
		COb_schpolyline* pnewpolyline = new COb_schpolyline();
		pnewpolyline->Copy(ppolyline);
		pnewpolyline->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolyline);
		
		if(pnewpolyline->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_POLYLINE;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}		
	}
	else if(element == SCH_ELEMENT_POLYGON)
	{
		COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
				
		COb_schpolygon* pnewpolygon = new COb_schpolygon();
		pnewpolygon->Copy(ppolygon);
		pnewpolygon->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolygon);
		
		if(pnewpolygon->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_POLYGON;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}
	}
	else if(element == SCH_ELEMENT_ELLIPSEFILL)
	{
		COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
				
		COb_schellipsefill* pnewellipsefill = new COb_schellipsefill();
		pnewellipsefill->Copy(pellipsefill);
		pnewellipsefill->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipsefill);
		
		if(pnewellipsefill->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}		
	}
	else if(element == SCH_ELEMENT_PIE)
	{
		COb_schpie* ppie = (COb_schpie*)pobject;
				
		COb_schpie* pnewpie = new COb_schpie();
		pnewpie->Copy(ppie);
		pnewpie->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpie);
		
		if(pnewpie->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_PIE;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}		
	}
	else if(element == SCH_ELEMENT_ROUNDRECT)
	{
		COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
				
		COb_schroundrect* pnewroundrect = new COb_schroundrect();
		pnewroundrect->Copy(proundrect);
		pnewroundrect->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewroundrect);
		
		if(pnewroundrect->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}		
	}
	else if(element == SCH_ELEMENT_ELLIPSE)
	{
		COb_schellipse* pellipse = (COb_schellipse*)pobject;
				
		COb_schellipse* pnewellipse = new COb_schellipse();
		pnewellipse->Copy(pellipse);
		pnewellipse->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipse);
		
		if(pnewellipse->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}		
	}
	else if(element == SCH_ELEMENT_ARC)
	{
		COb_scharc* parc = (COb_scharc*)pobject;
				
		COb_scharc* pnewarc = new COb_scharc();
		pnewarc->Copy(parc);
		pnewarc->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewarc);
		
		
		if(pnewarc->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_ARC;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}		
	}
	else if(element == SCH_ELEMENT_LINE)	
	{
		COb_schline* pline = (COb_schline*)pobject;
				
		COb_schline* pnewline = new COb_schline();
		pnewline->Copy(pline);
		pnewline->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewline);
		
		if(pnewline->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_LINE;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

			if(pnewline->m_nAttrib == LINE_WIRE)
			{
				if(bEDA_AutoOptimizeWire == TRUE) AutoOptimizeWire();
				if(bEDA_AutoJunction == TRUE) AutoPlaceJunction();
			}

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}
	}
	else if(element == SCH_ELEMENT_RECT)
	{
		COb_schrect* prect = (COb_schrect*)pobject;
				
		COb_schrect* pnewrect = new COb_schrect();
		pnewrect->Copy(prect);
		pnewrect->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewrect);
		
		if(pnewrect->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_RECT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}		
	}
	else if(element == SCH_ELEMENT_POWER)
	{
		COb_schpower* ppower = (COb_schpower*)pobject;
				
		COb_schpower* pnewpower = new COb_schpower();
		pnewpower->Copy(ppower);
		pnewpower->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpower);
		
		if(pnewpower->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_POWER;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);

			if(bEDA_AutoJunction == TRUE) AutoPlaceJunction();

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}		
	}
	else if(element == SCH_ELEMENT_PORT)
	{
		COb_schport* pport = (COb_schport*)pobject;
				
		COb_schport* pnewport = new COb_schport();
		pnewport->Copy(pport);
		pnewport->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewport);
		
		if(pnewport->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_PORT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}		
	}
	else if(element == SCH_ELEMENT_TEXTFRAME)
	{
		COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
				
		COb_schtextframe* pnewtextframe = new COb_schtextframe();
		pnewtextframe->Copy(ptextframe);
		pnewtextframe->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtextframe);
		
		if(pnewtextframe->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
			pnewtextframe->Draw(&dc, this);
		}
	}
	else if(element == SCH_ELEMENT_JUNCTION)
	{
		COb_schjunc* pjunc = (COb_schjunc*)pobject;

		COb_schjunc* pnewjunc = new COb_schjunc();
		pnewjunc->Copy(pjunc);
		pnewjunc->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewjunc);
		
		if(pnewjunc->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_JUNCTION;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}
	}
	else if(element == SCH_ELEMENT_GRAPHIC)
	{
		COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
				
		COb_schgraphic* pnewgraphic = new COb_schgraphic();
		pnewgraphic->Copy(pgraphic);
		pnewgraphic->m_bSelection = FALSE;
		POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewgraphic);
						
		if(pnewgraphic->MovePlaced(&dc, this) == FALSE)
		{
			
			delete pDoc->m_listSchObject[m_nDiagram].RemoveTail();
		}
		else
		{
			
			if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = newpos;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			
		}
	}
	else if(element == SCH_ELEMENT_SHEET)
	{
		
		
	}
}

void CESDView::PickupMoveOrCopy(int element, POSITION position)
{
	bEDA_MouseMoveElement = TRUE;
	if((GetKeyState(VK_SHIFT)&0x80) != 0) CopyElement(element, position);
	else MoveElement(element, position);
	bEDA_MouseMoveElement = FALSE;	
	
}

void CESDView::MoveAndRotateSelection(double x0, double y0,  double deltax, double deltay, int rotation, BOOL mirror)
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
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
			if( pline->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_LINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = pline->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				

				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pline->m_cVertex[k].fx - x0, pline->m_cVertex[k].fy - y0, rotation, mirror);
					pline->m_cVertex[k].fx = fx + x0 + deltax;
					pline->m_cVertex[k].fy = fy + y0 + deltay;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			if(ppolyline->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POLYLINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = ppolyline->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ppolyline->m_cVertex[k].fx - x0, ppolyline->m_cVertex[k].fy - y0, rotation, mirror);
					ppolyline->m_cVertex[k].fx = fx + x0 + deltax;
					ppolyline->m_cVertex[k].fy = fy + y0 + deltay;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
			if(pjunc->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_JUNCTION;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = pjunc->m_fCx;
				pundo->m_fY1 = pjunc->m_fCy;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pjunc->m_fCx - x0, pjunc->m_fCy - y0, rotation, mirror);
				pjunc->m_fCx = fx + x0 + deltax;
				pjunc->m_fCy = fy + y0 + deltay;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if(ptext->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schtext.Copy(ptext);
				pundo->schtext.m_nFlag = FLAG_NORMAL;

				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				

				double fx, fy;					
				pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - x0, ptext->m_fSy - y0, rotation, mirror);
				ptext->m_fSx = fx + x0 + deltax;
				ptext->m_fSy = fy + y0 + deltay;

				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ptext->m_nOrientation, rotation, FALSE); 
				ptext->m_nOrientation = orientation;
				ptext->m_nOrientation = pDoc->Get_Correct_Orientation(ptext->m_nOrientation);
		
				if(mirror == TRUE)
				{
					if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
					else ptext->m_bMirror = TRUE;
					if((ptext->m_nOrientation == 0)||(ptext->m_nOrientation == 180))
					{
						if(ptext->m_nH_Justification == 0) ptext->m_nH_Justification = 2;
						else if(ptext->m_nH_Justification == 2) ptext->m_nH_Justification = 0;
					}
					else
					{
						if(ptext->m_nV_Justification == 0) ptext->m_nV_Justification = 2;
						else if(ptext->m_nV_Justification == 2) ptext->m_nV_Justification = 0;
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if(parc->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ARC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->scharc.Copy(parc);
				pundo->scharc.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
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
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if(pellipse->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schellipse.Copy(pellipse);
				pundo->schellipse.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pellipse->m_fCx - x0, pellipse->m_fCy - y0, rotation, mirror);
				pellipse->m_fCx = fx + x0 + deltax;
				pellipse->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, pellipse->m_fSangle, pellipse->m_fEangle, rotation, mirror);
				pellipse->m_fSangle = salpha;
				pellipse->m_fSangle = pDoc->Get_Correct_Angle(pellipse->m_fSangle);
				pellipse->m_fEangle = ealpha;
				pellipse->m_fEangle = pDoc->Get_Correct_Angle(pellipse->m_fEangle);
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = pellipse->m_fRadius_y;
					pellipse->m_fRadius_y = pellipse->m_fRadius_x;
					pellipse->m_fRadius_x = ftemp;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if(pellipsefill->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schellipsefill.Copy(pellipsefill);
				pundo->schellipsefill.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pellipsefill->m_fCx - x0, pellipsefill->m_fCy - y0, rotation, mirror);
				pellipsefill->m_fCx = fx + x0 + deltax;
				pellipsefill->m_fCy = fy + y0 + deltay;
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = pellipsefill->m_fRadius_y;
					pellipsefill->m_fRadius_y = pellipsefill->m_fRadius_x;
					pellipsefill->m_fRadius_x = ftemp;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if(ppie->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_PIE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schpie.Copy(ppie);
				pundo->schpie.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppie->m_fCx - x0, ppie->m_fCy - y0, rotation, mirror);
				ppie->m_fCx = fx + x0 + deltax;
				ppie->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, ppie->m_fSangle, ppie->m_fEangle, rotation, mirror);
				ppie->m_fSangle = salpha;
				ppie->m_fSangle = pDoc->Get_Correct_Angle(ppie->m_fSangle);
				ppie->m_fEangle = ealpha;
				ppie->m_fEangle = pDoc->Get_Correct_Angle(ppie->m_fEangle);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if(ppolygon->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POLYGON;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = ppolygon->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ppolygon->m_cVertex[k].fx - x0, ppolygon->m_cVertex[k].fy - y0, rotation, mirror);
					ppolygon->m_cVertex[k].fx = fx + x0 + deltax;
					ppolygon->m_cVertex[k].fy = fy + y0 + deltay;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
			if(ptextframe->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = ptextframe->m_fX1;
				pundo->m_fY1 = ptextframe->m_fY1;
				pundo->m_fX2 = ptextframe->m_fX2;
				pundo->m_fY2 = ptextframe->m_fY2;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				double fx, fy, dx, dy;
				dx = ptextframe->m_fX2 - ptextframe->m_fX1;
				dy = ptextframe->m_fY2 - ptextframe->m_fY1;
				pDoc->Get_Rotate_XY(&fx, &fy, ptextframe->m_fX1 - x0, ptextframe->m_fY1 - y0, rotation, mirror);
				ptextframe->m_fX1 = fx + x0 + deltax;
				ptextframe->m_fY1 = fy + y0 + deltay;
				ptextframe->m_fX2 = ptextframe->m_fX1 + dx;
				ptextframe->m_fY2 = ptextframe->m_fY1 + dy;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if(prect->m_bSelection == TRUE)
			{

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_RECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = prect->m_fX1;
				pundo->m_fY1 = prect->m_fY1;
				pundo->m_fX2 = prect->m_fX2;
				pundo->m_fY2 = prect->m_fY2;
			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, prect->m_fX1 - x0, prect->m_fY1 - y0, rotation, mirror);
				prect->m_fX1 = fx + x0 + deltax;
				prect->m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, prect->m_fX2 - x0, prect->m_fY2 - y0, rotation, mirror);
				prect->m_fX2 = fx + x0 + deltax;
				prect->m_fY2 = fy + y0 + deltay;

				double fminx, fminy, fmaxx, fmaxy;
				fminx = MIN(prect->m_fX1, prect->m_fX2);
				fmaxx = MAX(prect->m_fX1, prect->m_fX2);
				fminy = MIN(prect->m_fY1, prect->m_fY2);
				fmaxy = MAX(prect->m_fY1, prect->m_fY2);
				prect->m_fX1 = fminx;
				prect->m_fY1 = fminy;
				prect->m_fX2 = fmaxx;
				prect->m_fY2 = fmaxy;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if(proundrect->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = proundrect->m_fX1;
				pundo->m_fY1 = proundrect->m_fY1;
				pundo->m_fX2 = proundrect->m_fX2;
				pundo->m_fY2 = proundrect->m_fY2;
				pundo->m_fRx = proundrect->m_fRadius_x;
				pundo->m_fRy = proundrect->m_fRadius_y;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, proundrect->m_fX1 - x0, proundrect->m_fY1 - y0, rotation, mirror);
				proundrect->m_fX1 = fx + x0 + deltax;
				proundrect->m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, proundrect->m_fX2 - x0, proundrect->m_fY2 - y0, rotation, mirror);
				proundrect->m_fX2 = fx + x0 + deltax;
				proundrect->m_fY2 = fy + y0 + deltay;
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = proundrect->m_fRadius_y;
					proundrect->m_fRadius_y = proundrect->m_fRadius_x;
					proundrect->m_fRadius_x = ftemp;
				}

				double fminx, fminy, fmaxx, fmaxy;
				fminx = MIN(proundrect->m_fX1, proundrect->m_fX2);
				fmaxx = MAX(proundrect->m_fX1, proundrect->m_fX2);
				fminy = MIN(proundrect->m_fY1, proundrect->m_fY2);
				fmaxy = MAX(proundrect->m_fY1, proundrect->m_fY2);
				proundrect->m_fX1 = fminx;
				proundrect->m_fY1 = fminy;
				proundrect->m_fX2 = fmaxx;
				proundrect->m_fY2 = fmaxy;
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
			
			double dx, dy;
			if(psheetsymbol->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = psheetsymbol->m_fX;
				pundo->m_fY1 = psheetsymbol->m_fY;
				pundo->m_fX2 = psheetsymbol->m_fXsize;
				pundo->m_fY2 = psheetsymbol->m_fYsize;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				double fx, fy, fxbak, fybak;
				fxbak = psheetsymbol->m_fX;
				fybak = psheetsymbol->m_fY;

				pDoc->Get_Rotate_XY(&fx, &fy, psheetsymbol->m_fX - x0, psheetsymbol->m_fY - y0, rotation, mirror);
				psheetsymbol->m_fX = fx + x0 + deltax;
				psheetsymbol->m_fY = fy + y0 + deltay;

				dx = psheetsymbol->m_fX - fxbak;
				dy = psheetsymbol->m_fY - fybak;

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) 
				{
					posbak = pos;
					pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext;
						ptext = (COb_schtext*)pobject;

						
						pundo = new CSchUndo;
						pundo->m_nOp = UNDO_MOVE;
						pundo->m_nOb = SCH_ELEMENT_TEXT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = posbak;
						pundo->m_fX1 = ptext->m_fSx;
						pundo->m_fY1 = ptext->m_fSy;
						pundo->m_nOrientation = ptext->m_nOrientation;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
						ptext->m_fSx += dx;
						ptext->m_fSy += dy;
					}
				}
			}
			else 
			{
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(pport->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_PORT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schport.Copy(pport);
				pundo->schport.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pport->m_fX - x0, pport->m_fY - y0, rotation, mirror);
				pport->m_fX = fx + x0 + deltax;
				pport->m_fY = fy + y0 + deltay;
				if(rotation == 90)
				{
					if(pport->m_nStyle == PORT_STYLE_NH) pport->m_nStyle = PORT_STYLE_NV;
					else if(pport->m_nStyle == PORT_STYLE_NV) pport->m_nStyle = PORT_STYLE_NH;
					else if(pport->m_nStyle == PORT_STYLE_LEFTRIGHT) pport->m_nStyle = PORT_STYLE_TOPBOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOPBOTTOM) pport->m_nStyle = PORT_STYLE_LEFTRIGHT;
					else if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_BOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_LEFT;
				}
				else if(rotation == 180)
				{
					if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_LEFT;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_BOTTOM;
				}
				else if(rotation == 270)
				{
					if(pport->m_nStyle == PORT_STYLE_NH) pport->m_nStyle = PORT_STYLE_NV;
					else if(pport->m_nStyle == PORT_STYLE_NV) pport->m_nStyle = PORT_STYLE_NH;
					else if(pport->m_nStyle == PORT_STYLE_LEFTRIGHT) pport->m_nStyle = PORT_STYLE_TOPBOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOPBOTTOM) pport->m_nStyle = PORT_STYLE_LEFTRIGHT;
					else if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_LEFT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_BOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_RIGHT;
				}

				if(mirror == TRUE)
				{
					if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_LEFT;
				}

				/*if(pport->m_nStyle < PORT_STYLE_NV)
				{
					pport->m_fX -= pport->m_fSize;
				}*/
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			if(ppower->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POWER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = ppower->m_fX;
				pundo->m_fY1 = ppower->m_fY;
				pundo->m_nOrientation = ppower->m_nOrientation;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppower->m_fX - x0, ppower->m_fY - y0, rotation, mirror);
				ppower->m_fX = fx + x0 + deltax;
				ppower->m_fY = fy + y0 + deltay;
				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ppower->m_nOrientation, rotation, mirror);
				ppower->m_nOrientation = orientation;
				ppower->m_nOrientation = pDoc->Get_Correct_Orientation(ppower->m_nOrientation);

				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if(pbezier->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_BEZIER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = pbezier->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pbezier->m_cVertex[k].fx - x0, pbezier->m_cVertex[k].fy - y0, rotation, mirror);
					pbezier->m_cVertex[k].fx = fx + x0 + deltax;
					pbezier->m_cVertex[k].fy = fy + y0 + deltay;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			if(pgraphic->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = pgraphic->m_fX1;
				pundo->m_fY1 = pgraphic->m_fY1;
				pundo->m_fX2 = pgraphic->m_fX2;
				pundo->m_fY2 = pgraphic->m_fY2;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				double fx, fy, dx, dy;
				dx = pgraphic->m_fX2 - pgraphic->m_fX1;
				dy = pgraphic->m_fY2 - pgraphic->m_fY1;
				pDoc->Get_Rotate_XY(&fx, &fy, pgraphic->m_fX1 - x0, pgraphic->m_fY1 - y0, rotation, mirror);
				pgraphic->m_fX1 = fx + x0 + deltax;
				pgraphic->m_fY1 = fy + y0 + deltay;
				pgraphic->m_fX2 = pgraphic->m_fX1 + dx;
				pgraphic->m_fY2 = pgraphic->m_fY1 + dy;
			}
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
			if(pcomp->m_bSelection == TRUE)
			{
						
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;

						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_COMPONENT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = posbak;
						pundo->schcomp.Copy(pcomp);
						pundo->schcomp.m_nFlag = FLAG_NORMAL;
						
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
						double fx1, fy1;
						pDoc->Get_Rotate_XY(&fx1, &fy1, pcomp->m_fX - x0, pcomp->m_fY - y0, rotation, mirror);
						pcomp->m_fX = fx1 + x0 + deltax;
						pcomp->m_fY = fy1 + y0 + deltay;
						pcomp->m_nOrientation += rotation;
						pcomp->m_nOrientation = pDoc->Get_Correct_Orientation(pcomp->m_nOrientation);
						if(mirror == TRUE)
						{
							if(pcomp->m_bMirrored == TRUE) pcomp->m_bMirrored = FALSE;
							else pcomp->m_bMirrored = TRUE;
						}
						
						for(int i=0; i<pcomp->m_arrayPos.GetSize(); i++)
						{
							CObject* pobject;
							POSITION subpos;
							subpos = pcomp->m_arrayPos.GetAt(i);
							pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
							{
								COb_schpin* ppin = (COb_schpin*)pobject;
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_PIN;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = subpos;
								pundo->schpin.Copy(ppin);
										
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								

								pDoc->Get_Rotate_XY(&fx1, &fy1, ppin->m_fX - x0, ppin->m_fY - y0, rotation, mirror);
								ppin->m_fX = fx1 + x0 + deltax;
								ppin->m_fY = fy1 + y0 + deltay;

								int orientation;
								pDoc->Get_Rotate_Orientation(&orientation, ppin->m_nOrientation, rotation, mirror);
								ppin->m_nOrientation = orientation;
								ppin->m_nOrientation = pDoc->Get_Correct_Orientation(ppin->m_nOrientation);					
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
							{
								COb_schieee* pieee = (COb_schieee*)pobject;
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_IEEE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = subpos;
								pundo->schieee.Copy(pieee);
										
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								pDoc->Get_Rotate_XY(&fx1, &fy1, pieee->m_fX - x0, pieee->m_fY - y0, rotation, mirror);
								pieee->m_fX = fx1 + x0 + deltax;
								pieee->m_fY = fy1 + y0 + deltay;
								int orientation;
								pDoc->Get_Rotate_Orientation(&orientation, pieee->m_nOrientation, rotation, mirror);
								pieee->m_nOrientation = orientation;
								pieee->m_nOrientation = pDoc->Get_Correct_Orientation(pieee->m_nOrientation);
			
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
							{
								COb_schtext* ptext = (COb_schtext*)pobject;
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_TEXT;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = subpos;
								pundo->schtext.Copy(ptext);
										
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								pDoc->Get_Rotate_XY(&fx1, &fy1, ptext->m_fSx - x0, ptext->m_fSy - y0, rotation, mirror);
								ptext->m_fSx = fx1 + x0 + deltax;
								ptext->m_fSy = fy1 + y0 + deltay;
								int orientation;
								pDoc->Get_Rotate_Orientation(&orientation, ptext->m_nOrientation, rotation, FALSE); 
								ptext->m_nOrientation = orientation;
								ptext->m_nOrientation = pDoc->Get_Correct_Orientation(ptext->m_nOrientation);
					
								if(mirror == TRUE)
								{
									if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
									else ptext->m_bMirror = TRUE;
									if((ptext->m_nOrientation == 0)||(ptext->m_nOrientation == 180))
									{
										if(ptext->m_nH_Justification == 0) ptext->m_nH_Justification = 2;
										else if(ptext->m_nH_Justification == 2) ptext->m_nH_Justification = 0;
									}
									else
									{
										if(ptext->m_nV_Justification == 0) ptext->m_nV_Justification = 2;
										else if(ptext->m_nV_Justification == 2) ptext->m_nV_Justification = 0;
									}
								}
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
							{
								COb_schbezier* pbezier = (COb_schbezier*)pobject;
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_BEZIER;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = subpos;
								pundo->schbezier.Copy(pbezier);
										
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
								{
									pDoc->Get_Rotate_XY(&fx1, &fy1, pbezier->m_cVertex[k].fx-x0, pbezier->m_cVertex[k].fy-y0, rotation, mirror);
									pbezier->m_cVertex[k].fx = fx1 + x0 + deltax;
									pbezier->m_cVertex[k].fy = fy1 + y0 + deltay;
								}
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
							{
								COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_POLYLINE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = subpos;
								pundo->schpolyline.Copy(ppolyline);
										
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
								{
									pDoc->Get_Rotate_XY(&fx1, &fy1, ppolyline->m_cVertex[k].fx-x0, ppolyline->m_cVertex[k].fy-y0, rotation, mirror);
									ppolyline->m_cVertex[k].fx = fx1 + x0 + deltax;
									ppolyline->m_cVertex[k].fy = fy1 + y0 + deltay;
								}
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
							{
								COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_POLYGON;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = subpos;
								pundo->schpolygon.Copy(ppolygon);
										
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
								{
									pDoc->Get_Rotate_XY(&fx1, &fy1, ppolygon->m_cVertex[k].fx-x0, ppolygon->m_cVertex[k].fy-y0, rotation, mirror);
									ppolygon->m_cVertex[k].fx = fx1 + x0 + deltax;
									ppolygon->m_cVertex[k].fy = fy1 + y0 + deltay;
								}
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
							{
								COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = subpos;
								pundo->schellipsefill.Copy(pellipsefill);
										
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								pDoc->Get_Rotate_XY(&fx1, &fy1, pellipsefill->m_fCx - x0, pellipsefill->m_fCy - y0, rotation, mirror);
								pellipsefill->m_fCx = fx1 + x0 + deltax;
								pellipsefill->m_fCy = fy1 + y0 + deltay;
								if((rotation == 90)||(rotation == 270))
								{
									double ftemp;
									ftemp = pellipsefill->m_fRadius_y;
									pellipsefill->m_fRadius_y = pellipsefill->m_fRadius_x;
									pellipsefill->m_fRadius_x = ftemp;
								}
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
							{
								COb_schpie* ppie = (COb_schpie*)pobject;
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_PIE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = subpos;
								pundo->schpie.Copy(ppie);
										
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								pDoc->Get_Rotate_XY(&fx1, &fy1, ppie->m_fCx - x0, ppie->m_fCy - y0, rotation, mirror);
								ppie->m_fCx = fx1 + x0 + deltax;
								ppie->m_fCy = fy1 + y0 + deltay;
								
								double salpha, ealpha;
								pDoc->Get_Rotate_Angle(&salpha, &ealpha, ppie->m_fSangle, ppie->m_fEangle, rotation, mirror);
								ppie->m_fSangle = salpha;
								ppie->m_fSangle = pDoc->Get_Correct_Angle(ppie->m_fSangle);
								ppie->m_fEangle = ealpha;
								ppie->m_fEangle = pDoc->Get_Correct_Angle(ppie->m_fEangle);
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
							{
								COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = subpos;
								pundo->schroundrect.Copy(proundrect);
										
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX1 - x0, proundrect->m_fY1 - y0, rotation, mirror);
								proundrect->m_fX1 = fx1 + x0 + deltax;
								proundrect->m_fY1 = fy1 + y0 + deltay;
								pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX2 - x0, proundrect->m_fY2 - y0, rotation, mirror);
								proundrect->m_fX2 = fx1 + x0 + deltax;
								proundrect->m_fY2 = fy1 + y0 + deltay;
								
								double fminx, fminy, fmaxx, fmaxy;
								fminx = MIN(proundrect->m_fX1, proundrect->m_fX2);
								fmaxx = MAX(proundrect->m_fX1, proundrect->m_fX2);
								fminy = MIN(proundrect->m_fY1, proundrect->m_fY2);
								fmaxy = MAX(proundrect->m_fY1, proundrect->m_fY2);
								proundrect->m_fX1 = fminx;
								proundrect->m_fY1 = fminy;
								proundrect->m_fX2 = fmaxx;
								proundrect->m_fY2 = fmaxy;
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
							{
								COb_schellipse* pellipse = (COb_schellipse*)pobject;
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = subpos;
								pundo->schellipse.Copy(pellipse);
										
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								pDoc->Get_Rotate_XY(&fx1, &fy1, pellipse->m_fCx - x0, pellipse->m_fCy - y0, rotation, mirror);
								pellipse->m_fCx = fx1 + x0 + deltax;
								pellipse->m_fCy = fy1 + y0 + deltay;
								
								double salpha, ealpha;
								pDoc->Get_Rotate_Angle(&salpha, &ealpha, pellipse->m_fSangle, pellipse->m_fEangle, rotation, mirror);
								pellipse->m_fSangle = salpha;
								pellipse->m_fSangle = pDoc->Get_Correct_Angle(pellipse->m_fSangle);
								pellipse->m_fEangle = ealpha;
								pellipse->m_fEangle = pDoc->Get_Correct_Angle(pellipse->m_fEangle);
								if((rotation == 90)||(rotation == 270))
								{
									double ftemp;
									ftemp = pellipse->m_fRadius_y;
									pellipse->m_fRadius_y = pellipse->m_fRadius_x;
									pellipse->m_fRadius_x = ftemp;
								}
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
							{
								COb_scharc* parc = (COb_scharc*)pobject;
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_ARC;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = subpos;
								pundo->scharc.Copy(parc);
										
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								pDoc->Get_Rotate_XY(&fx1, &fy1, parc->m_fCx - x0, parc->m_fCy - y0, rotation, mirror);
								parc->m_fCx = fx1 + x0 + deltax;
								parc->m_fCy = fy1 + y0 + deltay;
								
								double salpha, ealpha;
								pDoc->Get_Rotate_Angle(&salpha, &ealpha, parc->m_fSangle, parc->m_fEangle, rotation, mirror);
								parc->m_fSangle = salpha;
								parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
								parc->m_fEangle = ealpha;
								parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
							{
								COb_schrect* prect = (COb_schrect*)pobject;
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_RECT;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = subpos;
								pundo->schrect.Copy(prect);
										
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX1 - x0, prect->m_fY1 - y0, rotation, mirror);
								prect->m_fX1 = fx1 + x0 + deltax;
								prect->m_fY1 = fy1 + y0 + deltay;
								pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX2 - x0, prect->m_fY2 - y0, rotation, mirror);
								prect->m_fX2 = fx1 + x0 + deltax;
								prect->m_fY2 = fy1 + y0 + deltay;
								
								double fminx, fminy, fmaxx, fmaxy;
								fminx = MIN(prect->m_fX1, prect->m_fX2);
								fmaxx = MAX(prect->m_fX1, prect->m_fX2);
								fminy = MIN(prect->m_fY1, prect->m_fY2);
								fmaxy = MAX(prect->m_fY1, prect->m_fY2);
								prect->m_fX1 = fminx;
								prect->m_fY1 = fminy;
								prect->m_fX2 = fmaxx;
								prect->m_fY2 = fmaxy;
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
							{
								COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = subpos;
								pundo->schtextframe.Copy(ptextframe);
										
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX1 - x0, ptextframe->m_fY1 - y0, rotation, mirror);
								ptextframe->m_fX1 = fx1 + x0 + deltax;
								ptextframe->m_fY1 = fy1 + y0 + deltay;
								pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX2 - x0, ptextframe->m_fY2 - y0, rotation, mirror);
								ptextframe->m_fX2 = fx1 + x0 + deltax;
								ptextframe->m_fY2 = fy1 + y0 + deltay;
								
								double fminx, fminy, fmaxx, fmaxy;
								fminx = MIN(ptextframe->m_fX1, ptextframe->m_fX2);
								fmaxx = MAX(ptextframe->m_fX1, ptextframe->m_fX2);
								fminy = MIN(ptextframe->m_fY1, ptextframe->m_fY2);
								fmaxy = MAX(ptextframe->m_fY1, ptextframe->m_fY2);
								ptextframe->m_fX1 = fminx;
								ptextframe->m_fY1 = fminy;
								ptextframe->m_fX2 = fmaxx;
								ptextframe->m_fY2 = fmaxy;
							}
							else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
							{
								COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
								
								CSchUndo *pundo = new CSchUndo;
								pundo->m_nOp = UNDO_CHANGE;
								pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
								pundo->m_nDiagram = m_nDiagram;
								pundo->m_nPos = subpos;
								pundo->schgraphic.Copy(pgraphic);
										
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								
								pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX1 - x0, pgraphic->m_fY1 - y0, rotation, mirror);
								pgraphic->m_fX1 = fx1 + x0 + deltax;
								pgraphic->m_fY1 = fy1 + y0 + deltay;
								pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX2 - x0, pgraphic->m_fY2 - y0, rotation, mirror);
								pgraphic->m_fX2 = fx1 + x0 + deltax;
								pgraphic->m_fY2 = fy1 + y0 + deltay;
								
								double fminx, fminy, fmaxx, fmaxy;
								fminx = MIN(pgraphic->m_fX1, pgraphic->m_fX2);
								fmaxx = MAX(pgraphic->m_fX1, pgraphic->m_fX2);
								fminy = MIN(pgraphic->m_fY1, pgraphic->m_fY2);
								fmaxy = MAX(pgraphic->m_fY1, pgraphic->m_fY2);
								pgraphic->m_fX1 = fminx;
								pgraphic->m_fY1 = fminy;
								pgraphic->m_fX2 = fmaxx;
								pgraphic->m_fY2 = fmaxy;
							}
						}
			}
			
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
	}
	
	if(undo_flag == TRUE)
	{
		if(bEDA_AutoOptimizeWire == TRUE) AutoOptimizeWire();
		if(bEDA_AutoJunction == TRUE)
		{
			AutoPlaceJunction();
			AutoRemoveJunction();
		}

		pDoc->SetModifiedFlag(TRUE);
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}	
	
	Invalidate();
}

void CESDView::OnUpdateEditMoveSelection(CCmdUI *pCmdUI)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(pDoc->m_arraySelection[m_nDiagram].GetCount() > 0)
	{
		pCmdUI->Enable(TRUE);
		
	}
	else pCmdUI->Enable(FALSE);
}

void CESDView::OnEditMoveSelection()
{
	int ch1;
	int flag;
	BOOL mirror;
	double x0, y0, x1, y1, deltax, deltay;

	char szHelp[] = "移动选择: <左键><回车>选择基准点移动, <右键><ESC>结束命令";
	char szMoveText2[] = "移动选择: <左键><回车>放置所有移动图形, <空格>旋转, <X键>镜像, <右键><ESC>结束命令";
	
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	pDoc->m_fpSelection[m_nDiagram] = GetSelectionCenter();
	if(pDoc->m_arraySelection[m_nDiagram].GetCount() == 0) return;

	Write_Help(szHelp);
	do
	{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			
			ProcessingSelection(TRUE);

			x0 = m_fCurrentSnapX;
			y0 = m_fCurrentSnapY;

			int drawmode = dc.GetROP2();
			dc.SetROP2(R2_NOTXORPEN);	

			
			COb_schrect arect;
			arect.pDocument = pDoc;
			arect.m_fX1 = pDoc->m_fSelection_X1[m_nDiagram];
			arect.m_fY1 = pDoc->m_fSelection_Y1[m_nDiagram];
			arect.m_fX2 = pDoc->m_fSelection_X2[m_nDiagram];
			arect.m_fY2 = pDoc->m_fSelection_Y2[m_nDiagram];
			arect.m_nBorderWidth = 0;
			arect.m_nFillColor = RGB(255,255,255);
			arect.m_nBorderColor = RGB(0,0,0);
			arect.m_bTransparent = FALSE;
			arect.m_bFillSolid = FALSE;
			arect.m_nDiagram = m_nDiagram;

			CloseMouseCursor();
			arect.Draw(&dc, this);
			OpenMouseCursor();

			nEDA_RedrawFlag = 0;
			Write_Help(szMoveText2);
			flag = 0;
			mirror = FALSE;
			do
			{
				ch1 = MouseKey();
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					ProcessingSelection(FALSE);
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

					x1 = m_fCurrentSnapX;  
					y1 = m_fCurrentSnapY;
					deltax = x1 - x0;
					deltay = y1 - y0;

					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fSelection_X1[m_nDiagram] - x0, pDoc->m_fSelection_Y1[m_nDiagram] - y0, flag*90, mirror);
					arect.m_fX1 = fx + x0 + deltax;
					arect.m_fY1 = fy + y0 + deltay;
					pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fSelection_X2[m_nDiagram] - x0, pDoc->m_fSelection_Y2[m_nDiagram] - y0, flag*90, mirror);
					arect.m_fX2 = fx + x0 + deltax;
					arect.m_fY2 = fy + y0 + deltay;


					CloseMouseCursor();
					arect.Draw(&dc, this);
					OpenMouseCursor();
				}
				else if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
				{

					CloseMouseCursor();
					arect.Draw(&dc, this);
					OpenMouseCursor();				
					
					x1 = m_fCurrentSnapX;  
					y1 = m_fCurrentSnapY;
					deltax = x1 - x0;
					deltay = y1 - y0;
					
					ProcessingSelection(FALSE);
					MoveAndRotateSelection( x0, y0, deltax, deltay, flag*90, mirror);

					
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
					pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fSelection_X1[m_nDiagram] - x0, pDoc->m_fSelection_Y1[m_nDiagram] - y0, flag*90, mirror);
					arect.m_fX1 = fx + x0 + deltax;
					arect.m_fY1 = fy + y0 + deltay;
					pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fSelection_X2[m_nDiagram] - x0, pDoc->m_fSelection_Y2[m_nDiagram] - y0, flag*90, mirror);
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
					pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fSelection_X1[m_nDiagram] - x0, pDoc->m_fSelection_Y1[m_nDiagram] - y0, flag*90, mirror);
					arect.m_fX1 = fx + x0 + deltax;
					arect.m_fY1 = fy + y0 + deltay;
					pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fSelection_X2[m_nDiagram] - x0, pDoc->m_fSelection_Y2[m_nDiagram] - y0, flag*90, mirror);
					arect.m_fX2 = fx + x0 + deltax;
					arect.m_fY2 = fy + y0 + deltay;

					CloseMouseCursor();
					arect.Draw(&dc, this);
					OpenMouseCursor();

				}
			}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
			break;
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;

	pDoc->m_fpSelection[m_nDiagram] = GetSelectionCenter();
	Invalidate();
}

void CESDView::MoveSelection(void)
{
	int ch1;
	int flag;
	BOOL mirror;
	double x0, y0, x1, y1, deltax, deltay;

	char szMoveText[] = "移动选择: <左键><回车>放置所有移动图形, <空格>旋转, <X键>镜像, <右键><ESC>结束命令";
	
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	pDoc->m_fpSelection[m_nDiagram] = GetSelectionCenter();
	if(pDoc->m_arraySelection[m_nDiagram].GetCount() == 0) return;

	
	ProcessingSelection(TRUE);

	x0 = m_fCurrentSnapX;
	y0 = m_fCurrentSnapY;

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);	

		
		COb_schrect arect;
		arect.pDocument = pDoc;
		arect.m_fX1 = pDoc->m_fSelection_X1[m_nDiagram];
		arect.m_fY1 = pDoc->m_fSelection_Y1[m_nDiagram];
		arect.m_fX2 = pDoc->m_fSelection_X2[m_nDiagram];
		arect.m_fY2 = pDoc->m_fSelection_Y2[m_nDiagram];
		arect.m_nBorderWidth = 0;
		arect.m_nFillColor = RGB(255,255,255);
		arect.m_nBorderColor = RGB(0,0,0);
		arect.m_bTransparent = FALSE;
		arect.m_bFillSolid = FALSE;
		arect.m_nDiagram = m_nDiagram;

		CloseMouseCursor();
		arect.Draw(&dc, this);
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
				ProcessingSelection(FALSE);
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

				x1 = m_fCurrentSnapX;  
				y1 = m_fCurrentSnapY;
				deltax = x1 - x0;
				deltay = y1 - y0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fSelection_X1[m_nDiagram] - x0, pDoc->m_fSelection_Y1[m_nDiagram] - y0, flag*90, mirror);
				arect.m_fX1 = fx + x0 + deltax;
				arect.m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fSelection_X2[m_nDiagram] - x0, pDoc->m_fSelection_Y2[m_nDiagram] - y0, flag*90, mirror);
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
					
				ProcessingSelection(FALSE);
				MoveAndRotateSelection( x0, y0, deltax, deltay, flag*90, mirror);
				
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
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fSelection_X1[m_nDiagram] - x0, pDoc->m_fSelection_Y1[m_nDiagram] - y0, flag*90, mirror);
				arect.m_fX1 = fx + x0 + deltax;
				arect.m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fSelection_X2[m_nDiagram] - x0, pDoc->m_fSelection_Y2[m_nDiagram] - y0, flag*90, mirror);
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
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fSelection_X1[m_nDiagram] - x0, pDoc->m_fSelection_Y1[m_nDiagram] - y0, flag*90, mirror);
				arect.m_fX1 = fx + x0 + deltax;
				arect.m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pDoc->m_fSelection_X2[m_nDiagram] - x0, pDoc->m_fSelection_Y2[m_nDiagram] - y0, flag*90, mirror);
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

	pDoc->m_fpSelection[m_nDiagram] = GetSelectionCenter();
	Invalidate();
}

void CESDView::OnEditMoveFront()
{
	unsigned ch1;
	char szHelpText[] = "顺序最前: <左键><回车>选择对象, <右键><ESC>结束命令";
	
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	Write_Help(szHelpText);
	do
	{
       ch1 = MouseKey();
       if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
			Struct_Multi_Selection multi_selection = SelectSCHObject(m_fCurrentX, m_fCurrentY, MULTI_SELECTION);
			if((multi_selection.element == 0)||( multi_selection.pos == NULL)) continue;

			CObject* pobject;
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(multi_selection.pos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;

				COb_schcomp* pnewcomp = new COb_schcomp();
				pnewcomp->Copy(pcomp);
				pnewcomp->m_arrayPos.RemoveAll();
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewcomp);
				
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
				
				
				
				pcomp->m_bSelection = FALSE;
				pcomp->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_COMPONENT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				

				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
				{
					POSITION pos = pcomp->m_arrayPos.GetAt(j);
					pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
					{
						COb_schpin* ppin = (COb_schpin*)pobject;
						COb_schpin* pnewpin = new COb_schpin();
						pnewpin->Copy(ppin);
						pnewpin->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpin);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
					{
						COb_schieee* pieee = (COb_schieee*)pobject;
						COb_schieee* pnewieee = new COb_schieee();
						pnewieee->Copy(pieee);
						pnewieee->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewieee);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
					{
						COb_scharc* parc = (COb_scharc*)pobject;
						COb_scharc* pnewarc = new COb_scharc();
						pnewarc->Copy(parc);
						pnewarc->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewarc);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
					{
						COb_schellipse* pellipse = (COb_schellipse*)pobject;
						COb_schellipse* pnewellipse = new COb_schellipse();
						pnewellipse->Copy(pellipse);
						pnewellipse->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipse);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
					{
						COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
						COb_schellipsefill* pnewellipsefill = new COb_schellipsefill();
						pnewellipsefill->Copy(pellipsefill);
						pnewellipsefill->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipsefill);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
					{
						COb_schpie* ppie = (COb_schpie*)pobject;
						COb_schpie* pnewpie = new COb_schpie();
						pnewpie->Copy(ppie);
						pnewpie->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpie);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
					{
						COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
						COb_schpolyline* pnewpolyline = new COb_schpolyline();
						pnewpolyline->Copy(ppolyline);
						pnewpolyline->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolyline);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
					{
						COb_schrect* prect = (COb_schrect*)pobject;
						COb_schrect* pnewrect = new COb_schrect();
						pnewrect->Copy(prect);
						pnewrect->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewrect);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
					{
						COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
						COb_schroundrect* pnewroundrect = new COb_schroundrect();
						pnewroundrect->Copy(proundrect);
						pnewroundrect->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewroundrect);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
					{
						COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
						COb_schpolygon* pnewpolygon = new COb_schpolygon();
						pnewpolygon->Copy(ppolygon);
						pnewpolygon->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolygon);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
					{
						COb_schbezier* pbezier = (COb_schbezier*)pobject;
						COb_schbezier* pnewbezier = new COb_schbezier();
						pnewbezier->Copy(pbezier);
						pnewbezier->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewbezier);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						COb_schtext* pnewtext = new COb_schtext();
						pnewtext->Copy(ptext);
						pnewtext->m_bSelection = FALSE;
						pnewtext->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtext);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
					{
						COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
						COb_schtextframe* pnewtextframe = new COb_schtextframe();
						pnewtextframe->Copy(ptextframe);
						pnewtextframe->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtextframe);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
					{
						COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
						COb_schgraphic* pnewgraphic = new COb_schgraphic();
						pnewgraphic->Copy(pgraphic);
						pnewgraphic->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewgraphic);
						pnewcomp->m_arrayPos.Add(subpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;

				
				COb_schsheetsymbol* pnewsheetsymbol= new COb_schsheetsymbol();
				pnewsheetsymbol->Copy(psheetsymbol);
				pnewsheetsymbol->m_arrayPos.RemoveAll();
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewsheetsymbol);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				psheetsymbol->m_bSelection = FALSE;
				psheetsymbol->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
				{
					POSITION pos = psheetsymbol->m_arrayPos.GetAt(j);
					pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
					{
						COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
						COb_schsheetentry* pnewsheetentry = new COb_schsheetentry();
						pnewsheetentry->Copy(psheetentry);
						pnewsheetentry->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewsheetentry);
						pnewsheetsymbol->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						COb_schtext* pnewtext = new COb_schtext();
						pnewtext->Copy(ptext);
						pnewtext->m_posParent = posParent;
						POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtext);
						pnewsheetsymbol->m_arrayPos.Add(subpos);
					}
				}
			}			
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				
				COb_schline* pnewline= new COb_schline();
				pnewline->Copy(pline);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewline);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_LINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				pline->m_bSelection = FALSE;
				pline->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_LINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
			{
				COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
				
				COb_schpolyline* pnewpolyline= new COb_schpolyline();
				pnewpolyline->Copy(ppolyline);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolyline);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_POLYLINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				ppolyline->m_bSelection = FALSE;
				ppolyline->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_POLYLINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc = (COb_schjunc*)pobject;
				
				COb_schjunc* pnewjunc= new COb_schjunc();
				pnewjunc->Copy(pjunc);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewjunc);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_JUNCTION;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				pjunc->m_bSelection = FALSE;
				pjunc->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_JUNCTION;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				
				COb_schtext* pnewtext= new COb_schtext();
				pnewtext->Copy(ptext);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtext);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				ptext->m_bSelection = FALSE;
				ptext->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
			{
				COb_scharc* parc = (COb_scharc*)pobject;
				
				COb_scharc* pnewarc= new COb_scharc();
				pnewarc->Copy(parc);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewarc);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_ARC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				parc->m_bSelection = FALSE;
				parc->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_ARC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
			{
				COb_schellipse* pellipse = (COb_schellipse*)pobject;
				
				COb_schellipse* pnewellipse= new COb_schellipse();
				pnewellipse->Copy(pellipse);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipse);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				pellipse->m_bSelection = FALSE;
				pellipse->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
			{
				COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
				
				COb_schellipsefill* pnewellipsefill= new COb_schellipsefill();
				pnewellipsefill->Copy(pellipsefill);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipsefill);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				pellipsefill->m_bSelection = FALSE;
				pellipsefill->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
			{
				COb_schpie* ppie = (COb_schpie*)pobject;
				
				COb_schpie* pnewpie= new COb_schpie();
				pnewpie->Copy(ppie);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpie);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_PIE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				ppie->m_bSelection = FALSE;
				ppie->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_PIE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
			{
				COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
				
				COb_schpolygon* pnewpolygon= new COb_schpolygon();
				pnewpolygon->Copy(ppolygon);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolygon);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_POLYGON;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				ppolygon->m_bSelection = FALSE;
				ppolygon->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_POLYGON;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
			{
				COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
				
				COb_schtextframe* pnewtextframe= new COb_schtextframe();
				pnewtextframe->Copy(ptextframe);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtextframe);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				ptextframe->m_bSelection = FALSE;
				ptextframe->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
			{
				COb_schrect* prect = (COb_schrect*)pobject;
				
				COb_schrect* pnewrect= new COb_schrect();
				pnewrect->Copy(prect);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewrect);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_RECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				prect->m_bSelection = FALSE;
				prect->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_RECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
			{
				COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
				
				COb_schroundrect* pnewroundrect= new COb_schroundrect();
				pnewroundrect->Copy(proundrect);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewroundrect);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				proundrect->m_bSelection = FALSE;
				proundrect->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport = (COb_schport*)pobject;
				
				COb_schport* pnewport= new COb_schport();
				pnewport->Copy(pport);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewport);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_PORT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				pport->m_bSelection = FALSE;
				pport->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_PORT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				
				COb_schpower* pnewpower= new COb_schpower();
				pnewpower->Copy(ppower);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpower);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_POWER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				ppower->m_bSelection = FALSE;
				ppower->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_POWER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
			{
				COb_schbezier* pbezier = (COb_schbezier*)pobject;
				
				COb_schbezier* pnewbezier= new COb_schbezier();
				pnewbezier->Copy(pbezier);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewbezier);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_BEZIER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				pbezier->m_bSelection = FALSE;
				pbezier->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_BEZIER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
			{
				COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
				
				COb_schgraphic* pnewgraphic= new COb_schgraphic();
				pnewgraphic->Copy(pgraphic);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewgraphic);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				pgraphic->m_bSelection = FALSE;
				pgraphic->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}

			Write_Help(szHelpText);
			ch1 = VK_ESCAPE;
	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
	
	Invalidate();
}

void CESDView::OnEditMoveBack()
{
	unsigned ch1;
	char szHelpText[] = "顺序最后: <左键><回车>选择对象, <右键><ESC>结束命令";
	
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	Write_Help(szHelpText);
	do
	{
       ch1 = MouseKey();
       if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
			Struct_Multi_Selection multi_selection = SelectSCHObject(m_fCurrentX, m_fCurrentY, MULTI_SELECTION);
			if((multi_selection.element == 0)||( multi_selection.pos == NULL)) continue;

			CObject* pobject;
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(multi_selection.pos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;
				CArray <POSITION, POSITION&> arrayPos;
				CObject* pobject = NULL;
				
				int totalnum = pcomp->m_arrayPos.GetSize();
				for(int i=0; i<totalnum; i++)
				{
					POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddHead(pobject);
					arrayPos.Add(subpos);
				}

				
				COb_schcomp* pnewcomp = new COb_schcomp();
				pnewcomp->Copy(pcomp);
				pnewcomp->m_arrayPos.RemoveAll();
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewcomp);
				
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
				
				
				
				pcomp->m_bSelection = FALSE;
				pcomp->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_COMPONENT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				

				for(int j=0; j<totalnum; j++)
				{
					POSITION pos = pcomp->m_arrayPos.GetAt(j);
					pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
					{
						COb_schpin* ppin = (COb_schpin*)pobject;
						COb_schpin* pnewpin = new COb_schpin();
						pnewpin->Copy(ppin);
						pnewpin->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewpin);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
					{
						COb_schieee* pieee = (COb_schieee*)pobject;
						COb_schieee* pnewieee = new COb_schieee();
						pnewieee->Copy(pieee);
						pnewieee->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewieee);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
					{
						COb_scharc* parc = (COb_scharc*)pobject;
						COb_scharc* pnewarc = new COb_scharc();
						pnewarc->Copy(parc);
						pnewarc->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewarc);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
					{
						COb_schellipse* pellipse = (COb_schellipse*)pobject;
						COb_schellipse* pnewellipse = new COb_schellipse();
						pnewellipse->Copy(pellipse);
						pnewellipse->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewellipse);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
					{
						COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
						COb_schellipsefill* pnewellipsefill = new COb_schellipsefill();
						pnewellipsefill->Copy(pellipsefill);
						pnewellipsefill->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewellipsefill);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
					{
						COb_schpie* ppie = (COb_schpie*)pobject;
						COb_schpie* pnewpie = new COb_schpie();
						pnewpie->Copy(ppie);
						pnewpie->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewpie);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
					{
						COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
						COb_schpolyline* pnewpolyline = new COb_schpolyline();
						pnewpolyline->Copy(ppolyline);
						pnewpolyline->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewpolyline);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
					{
						COb_schrect* prect = (COb_schrect*)pobject;
						COb_schrect* pnewrect = new COb_schrect();
						pnewrect->Copy(prect);
						pnewrect->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewrect);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
					{
						COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
						COb_schroundrect* pnewroundrect = new COb_schroundrect();
						pnewroundrect->Copy(proundrect);
						pnewroundrect->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewroundrect);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
					{
						COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
						COb_schpolygon* pnewpolygon = new COb_schpolygon();
						pnewpolygon->Copy(ppolygon);
						pnewpolygon->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewpolygon);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
					{
						COb_schbezier* pbezier = (COb_schbezier*)pobject;
						COb_schbezier* pnewbezier = new COb_schbezier();
						pnewbezier->Copy(pbezier);
						pnewbezier->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewbezier);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						COb_schtext* pnewtext = new COb_schtext();
						pnewtext->Copy(ptext);
						pnewtext->m_bSelection = FALSE;
						pnewtext->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewtext);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
					{
						COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
						COb_schtextframe* pnewtextframe = new COb_schtextframe();
						pnewtextframe->Copy(ptextframe);
						pnewtextframe->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewtextframe);
						pnewcomp->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
					{
						COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
						COb_schgraphic* pnewgraphic = new COb_schgraphic();
						pnewgraphic->Copy(pgraphic);
						pnewgraphic->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewgraphic);
						pnewcomp->m_arrayPos.Add(subpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
				CArray <POSITION, POSITION&> arrayPos;
				CObject* pobject = NULL;
				
				int totalnum = psheetsymbol->m_arrayPos.GetSize();
				for(int i=0; i<totalnum; i++)
				{
					POSITION subpos = pDoc->m_listSchObject[m_nDiagram].AddHead(pobject);
					arrayPos.Add(subpos);
				}

				
				COb_schsheetsymbol* pnewsheetsymbol= new COb_schsheetsymbol();
				pnewsheetsymbol->Copy(psheetsymbol);
				pnewsheetsymbol->m_arrayPos.RemoveAll();
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewsheetsymbol);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				psheetsymbol->m_bSelection = FALSE;
				psheetsymbol->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
				{
					POSITION pos = psheetsymbol->m_arrayPos.GetAt(j);
					pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
					{
						COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
						COb_schsheetentry* pnewsheetentry = new COb_schsheetentry();
						pnewsheetentry->Copy(psheetentry);
						pnewsheetentry->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewsheetentry);
						pnewsheetsymbol->m_arrayPos.Add(subpos);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						COb_schtext* pnewtext = new COb_schtext();
						pnewtext->Copy(ptext);
						pnewtext->m_posParent = posParent;
						POSITION subpos = arrayPos.GetAt(totalnum - j - 1);
						pDoc->m_listSchObject[m_nDiagram].SetAt(subpos, pnewtext);
						pnewsheetsymbol->m_arrayPos.Add(subpos);
					}
				}
			}			
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				
				COb_schline* pnewline= new COb_schline();
				pnewline->Copy(pline);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewline);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_LINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				pline->m_bSelection = FALSE;
				pline->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_LINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
			{
				COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
				
				COb_schpolyline* pnewpolyline= new COb_schpolyline();
				pnewpolyline->Copy(ppolyline);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewpolyline);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_POLYLINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				ppolyline->m_bSelection = FALSE;
				ppolyline->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_POLYLINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc = (COb_schjunc*)pobject;
				
				COb_schjunc* pnewjunc= new COb_schjunc();
				pnewjunc->Copy(pjunc);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewjunc);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_JUNCTION;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				pjunc->m_bSelection = FALSE;
				pjunc->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_JUNCTION;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				
				COb_schtext* pnewtext= new COb_schtext();
				pnewtext->Copy(ptext);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewtext);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				ptext->m_bSelection = FALSE;
				ptext->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_TEXT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
			{
				COb_scharc* parc = (COb_scharc*)pobject;
				
				COb_scharc* pnewarc= new COb_scharc();
				pnewarc->Copy(parc);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewarc);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_ARC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				parc->m_bSelection = FALSE;
				parc->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_ARC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
			{
				COb_schellipse* pellipse = (COb_schellipse*)pobject;
				
				COb_schellipse* pnewellipse= new COb_schellipse();
				pnewellipse->Copy(pellipse);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewellipse);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				pellipse->m_bSelection = FALSE;
				pellipse->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
			{
				COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
				
				COb_schellipsefill* pnewellipsefill= new COb_schellipsefill();
				pnewellipsefill->Copy(pellipsefill);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewellipsefill);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				pellipsefill->m_bSelection = FALSE;
				pellipsefill->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
			{
				COb_schpie* ppie = (COb_schpie*)pobject;
				
				COb_schpie* pnewpie= new COb_schpie();
				pnewpie->Copy(ppie);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewpie);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_PIE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				ppie->m_bSelection = FALSE;
				ppie->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_PIE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
			{
				COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
				
				COb_schpolygon* pnewpolygon= new COb_schpolygon();
				pnewpolygon->Copy(ppolygon);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewpolygon);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_POLYGON;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				ppolygon->m_bSelection = FALSE;
				ppolygon->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_POLYGON;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
			{
				COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
				
				COb_schtextframe* pnewtextframe= new COb_schtextframe();
				pnewtextframe->Copy(ptextframe);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewtextframe);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				ptextframe->m_bSelection = FALSE;
				ptextframe->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
			{
				COb_schrect* prect = (COb_schrect*)pobject;
				
				COb_schrect* pnewrect= new COb_schrect();
				pnewrect->Copy(prect);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewrect);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_RECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				prect->m_bSelection = FALSE;
				prect->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_RECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
			{
				COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
				
				COb_schroundrect* pnewroundrect= new COb_schroundrect();
				pnewroundrect->Copy(proundrect);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewroundrect);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				proundrect->m_bSelection = FALSE;
				proundrect->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport = (COb_schport*)pobject;
				
				COb_schport* pnewport= new COb_schport();
				pnewport->Copy(pport);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewport);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_PORT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				pport->m_bSelection = FALSE;
				pport->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_PORT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				
				COb_schpower* pnewpower= new COb_schpower();
				pnewpower->Copy(ppower);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewpower);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_POWER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				ppower->m_bSelection = FALSE;
				ppower->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_POWER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
			{
				COb_schbezier* pbezier = (COb_schbezier*)pobject;
				
				COb_schbezier* pnewbezier= new COb_schbezier();
				pnewbezier->Copy(pbezier);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewbezier);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_BEZIER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				pbezier->m_bSelection = FALSE;
				pbezier->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_BEZIER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				

				
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
			{
				COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
				
				COb_schgraphic* pnewgraphic= new COb_schgraphic();
				pnewgraphic->Copy(pgraphic);
				POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddHead(pnewgraphic);
				
				if(pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posParent;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				
				
				pgraphic->m_bSelection = FALSE;
				pgraphic->m_nFlag = FLAG_DELETED;
				
				pundo = new CSchUndo;
				pundo->m_nOp = UNDO_UNDELETE;
				pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = multi_selection.pos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				


				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
				
			}

			Write_Help(szHelpText);
			ch1 = VK_ESCAPE;
	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
	
	Invalidate();
}

void CESDView::CopyAndRotateSelection(double x0, double y0,  double deltax, double deltay, int rotation, BOOL mirror)
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
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;

			if( pline->m_bSelection == TRUE)
			{
				COb_schline* pnewline = new COb_schline();
				pnewline->Copy(pline);
				pnewline->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewline);
						
				
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
				

				for(int k=0; k<pnewline->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pnewline->m_cVertex[k].fx - x0, pnewline->m_cVertex[k].fy - y0, rotation, mirror);
					pnewline->m_cVertex[k].fx = fx + x0 + deltax;
					pnewline->m_cVertex[k].fy = fy + y0 + deltay;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;

			if(ppolyline->m_bSelection == TRUE)
			{
				COb_schpolyline* pnewpolyline = new COb_schpolyline();
				pnewpolyline->Copy(ppolyline);
				pnewpolyline->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolyline);
						
				
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
				

				for(int k=0; k<pnewpolyline->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pnewpolyline->m_cVertex[k].fx - x0, pnewpolyline->m_cVertex[k].fy - y0, rotation, mirror);
					pnewpolyline->m_cVertex[k].fx = fx + x0 + deltax;
					pnewpolyline->m_cVertex[k].fy = fy + y0 + deltay;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;

			if(pjunc->m_bSelection == TRUE)
			{
				COb_schjunc* pnewjunc = new COb_schjunc();
				pnewjunc->Copy(pjunc);
				pnewjunc->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewjunc);
						
				
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
				

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pnewjunc->m_fCx - x0, pnewjunc->m_fCy - y0, rotation, mirror);
				pnewjunc->m_fCx = fx + x0 + deltax;
				pnewjunc->m_fCy = fy + y0 + deltay;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if(ptext->m_bSelection == TRUE)
			{
				COb_schtext* pnewtext = new COb_schtext();
				pnewtext->Copy(ptext);
				pnewtext->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtext);
						
				
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
				


				double fx, fy;					
				pDoc->Get_Rotate_XY(&fx, &fy, pnewtext->m_fSx - x0, pnewtext->m_fSy - y0, rotation, mirror);
				pnewtext->m_fSx = fx + x0 + deltax;
				pnewtext->m_fSy = fy + y0 + deltay;

				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, pnewtext->m_nOrientation, rotation, FALSE); 
				pnewtext->m_nOrientation = orientation;
				pnewtext->m_nOrientation = pDoc->Get_Correct_Orientation(pnewtext->m_nOrientation);
		
				if(mirror == TRUE)
				{
					if(pnewtext->m_bMirror == TRUE) pnewtext->m_bMirror = FALSE;
					else pnewtext->m_bMirror = TRUE;
					if((pnewtext->m_nOrientation == 0)||(pnewtext->m_nOrientation == 180))
					{
						if(pnewtext->m_nH_Justification == 0) pnewtext->m_nH_Justification = 2;
						else if(pnewtext->m_nH_Justification == 2) pnewtext->m_nH_Justification = 0;
					}
					else
					{
						if(pnewtext->m_nV_Justification == 0) pnewtext->m_nV_Justification = 2;
						else if(pnewtext->m_nV_Justification == 2) pnewtext->m_nV_Justification = 0;
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if(parc->m_bSelection == TRUE)
			{
				COb_scharc* pnewarc = new COb_scharc();
				pnewarc->Copy(parc);
				pnewarc->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewarc);
						
				
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
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if(pellipse->m_bSelection == TRUE)
			{
				COb_schellipse* pnewellipse = new COb_schellipse();
				pnewellipse->Copy(pellipse);
				pnewellipse->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipse);
						
				
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
				

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pnewellipse->m_fCx - x0, pnewellipse->m_fCy - y0, rotation, mirror);
				pnewellipse->m_fCx = fx + x0 + deltax;
				pnewellipse->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, pnewellipse->m_fSangle, pnewellipse->m_fEangle, rotation, mirror);
				pnewellipse->m_fSangle = salpha;
				pnewellipse->m_fSangle = pDoc->Get_Correct_Angle(pnewellipse->m_fSangle);
				pnewellipse->m_fEangle = ealpha;
				pnewellipse->m_fEangle = pDoc->Get_Correct_Angle(pnewellipse->m_fEangle);
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = pnewellipse->m_fRadius_y;
					pnewellipse->m_fRadius_y = pnewellipse->m_fRadius_x;
					pnewellipse->m_fRadius_x = ftemp;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if(pellipsefill->m_bSelection == TRUE)
			{
				COb_schellipsefill* pnewellipsefill = new COb_schellipsefill();
				pnewellipsefill->Copy(pellipsefill);
				pnewellipsefill->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipsefill);
						
				
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
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pnewellipsefill->m_fCx - x0, pnewellipsefill->m_fCy - y0, rotation, mirror);
				pnewellipsefill->m_fCx = fx + x0 + deltax;
				pnewellipsefill->m_fCy = fy + y0 + deltay;
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = pnewellipsefill->m_fRadius_y;
					pnewellipsefill->m_fRadius_y = pnewellipsefill->m_fRadius_x;
					pnewellipsefill->m_fRadius_x = ftemp;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if(ppie->m_bSelection == TRUE)
			{
				COb_schpie* pnewpie = new COb_schpie();
				pnewpie->Copy(ppie);
				pnewpie->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpie);
						
				
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
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pnewpie->m_fCx - x0, pnewpie->m_fCy - y0, rotation, mirror);
				pnewpie->m_fCx = fx + x0 + deltax;
				pnewpie->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, pnewpie->m_fSangle, pnewpie->m_fEangle, rotation, mirror);
				pnewpie->m_fSangle = salpha;
				pnewpie->m_fSangle = pDoc->Get_Correct_Angle(pnewpie->m_fSangle);
				pnewpie->m_fEangle = ealpha;
				pnewpie->m_fEangle = pDoc->Get_Correct_Angle(pnewpie->m_fEangle);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if(ppolygon->m_bSelection == TRUE)
			{
				COb_schpolygon* pnewpolygon = new COb_schpolygon();
				pnewpolygon->Copy(ppolygon);
				pnewpolygon->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolygon);
						
				
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
				
				for(int k=0; k<pnewpolygon->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pnewpolygon->m_cVertex[k].fx - x0, pnewpolygon->m_cVertex[k].fy - y0, rotation, mirror);
					pnewpolygon->m_cVertex[k].fx = fx + x0 + deltax;
					pnewpolygon->m_cVertex[k].fy = fy + y0 + deltay;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
			if(ptextframe->m_bSelection == TRUE)
			{
				COb_schtextframe* pnewtextframe = new COb_schtextframe();
				pnewtextframe->Copy(ptextframe);
				pnewtextframe->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtextframe);
						
				
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
				
				double fx, fy, dx, dy;
				dx = pnewtextframe->m_fX2 - pnewtextframe->m_fX1;
				dy = pnewtextframe->m_fY2 - pnewtextframe->m_fY1;
				pDoc->Get_Rotate_XY(&fx, &fy, pnewtextframe->m_fX1 - x0, pnewtextframe->m_fY1 - y0, rotation, mirror);
				pnewtextframe->m_fX1 = fx + x0 + deltax;
				pnewtextframe->m_fY1 = fy + y0 + deltay;
				pnewtextframe->m_fX2 = pnewtextframe->m_fX1 + dx;
				pnewtextframe->m_fY2 = pnewtextframe->m_fY1 + dy;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if(prect->m_bSelection == TRUE)
			{
				COb_schrect* pnewrect = new COb_schrect();
				pnewrect->Copy(prect);
				pnewrect->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewrect);
						
				
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
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pnewrect->m_fX1 - x0, pnewrect->m_fY1 - y0, rotation, mirror);
				pnewrect->m_fX1 = fx + x0 + deltax;
				pnewrect->m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pnewrect->m_fX2 - x0, pnewrect->m_fY2 - y0, rotation, mirror);
				pnewrect->m_fX2 = fx + x0 + deltax;
				pnewrect->m_fY2 = fy + y0 + deltay;

				double fminx, fminy, fmaxx, fmaxy;
				fminx = MIN(pnewrect->m_fX1, pnewrect->m_fX2);
				fmaxx = MAX(pnewrect->m_fX1, pnewrect->m_fX2);
				fminy = MIN(pnewrect->m_fY1, pnewrect->m_fY2);
				fmaxy = MAX(pnewrect->m_fY1, pnewrect->m_fY2);
				pnewrect->m_fX1 = fminx;
				pnewrect->m_fY1 = fminy;
				pnewrect->m_fX2 = fmaxx;
				pnewrect->m_fY2 = fmaxy;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if(proundrect->m_bSelection == TRUE)
			{
				COb_schroundrect* pnewroundrect = new COb_schroundrect();
				pnewroundrect->Copy(proundrect);
				pnewroundrect->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewroundrect);
						
				
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
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pnewroundrect->m_fX1 - x0, pnewroundrect->m_fY1 - y0, rotation, mirror);
				pnewroundrect->m_fX1 = fx + x0 + deltax;
				pnewroundrect->m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, pnewroundrect->m_fX2 - x0, pnewroundrect->m_fY2 - y0, rotation, mirror);
				pnewroundrect->m_fX2 = fx + x0 + deltax;
				pnewroundrect->m_fY2 = fy + y0 + deltay;
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = pnewroundrect->m_fRadius_y;
					pnewroundrect->m_fRadius_y = pnewroundrect->m_fRadius_x;
					pnewroundrect->m_fRadius_x = ftemp;
				}

				double fminx, fminy, fmaxx, fmaxy;
				fminx = MIN(pnewroundrect->m_fX1, pnewroundrect->m_fX2);
				fmaxx = MAX(pnewroundrect->m_fX1, pnewroundrect->m_fX2);
				fminy = MIN(pnewroundrect->m_fY1, pnewroundrect->m_fY2);
				fmaxy = MAX(pnewroundrect->m_fY1, pnewroundrect->m_fY2);
				pnewroundrect->m_fX1 = fminx;
				pnewroundrect->m_fY1 = fminy;
				pnewroundrect->m_fX2 = fmaxx;
				pnewroundrect->m_fY2 = fmaxy;
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
			
			double dx, dy;
			if(psheetsymbol->m_bSelection == TRUE)
			{
				COb_schsheetsymbol* pnewsheetsymbol = new COb_schsheetsymbol();
				pnewsheetsymbol->Copy(psheetsymbol);
				pnewsheetsymbol->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewsheetsymbol);
						
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = newpos;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				double fx, fy, fxbak, fybak;
				fxbak = pnewsheetsymbol->m_fX;
				fybak = pnewsheetsymbol->m_fY;

				pDoc->Get_Rotate_XY(&fx, &fy, pnewsheetsymbol->m_fX - x0, pnewsheetsymbol->m_fY - y0, rotation, mirror);
				pnewsheetsymbol->m_fX = fx + x0 + deltax;
				pnewsheetsymbol->m_fY = fy + y0 + deltay;

				dx = pnewsheetsymbol->m_fX - fxbak;
				dy = pnewsheetsymbol->m_fY - fybak;

				
				pnewsheetsymbol->m_arrayPos.RemoveAll();
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) 
				{
					posbak = pos;
					pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
					{
						COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;

						COb_schsheetentry* pnewsheetentry = new COb_schsheetentry();
						pnewsheetentry->Copy(psheetentry);
						pnewsheetentry->m_posParent = newpos;
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewsheetentry);

						
						pnewsheetsymbol->m_arrayPos.Add(pos1);

					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;

						COb_schtext* pnewtext = new COb_schtext();
						pnewtext->Copy(ptext);
						pnewtext->m_posParent = newpos;
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtext);
						
						pnewsheetsymbol->m_arrayPos.Add(pos1);
						pnewtext->m_fSx += dx;
						pnewtext->m_fSy += dy;
					}
				}
			}
			else 
			{
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(pport->m_bSelection == TRUE)
			{
				COb_schport* pnewport = new COb_schport();
				pnewport->Copy(pport);
				pnewport->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewport);
						
				
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
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pnewport->m_fX - x0, pnewport->m_fY - y0, rotation, mirror);
				pnewport->m_fX = fx + x0 + deltax;
				pnewport->m_fY = fy + y0 + deltay;
				if(rotation == 90)
				{
					if(pnewport->m_nStyle == PORT_STYLE_NH) pnewport->m_nStyle = PORT_STYLE_NV;
					else if(pnewport->m_nStyle == PORT_STYLE_NV) pnewport->m_nStyle = PORT_STYLE_NH;
					else if(pnewport->m_nStyle == PORT_STYLE_LEFTRIGHT) pnewport->m_nStyle = PORT_STYLE_TOPBOTTOM;
					else if(pnewport->m_nStyle == PORT_STYLE_TOPBOTTOM) pnewport->m_nStyle = PORT_STYLE_LEFTRIGHT;
					else if(pnewport->m_nStyle == PORT_STYLE_LEFT)  pnewport->m_nStyle = PORT_STYLE_BOTTOM;
					else if(pnewport->m_nStyle == PORT_STYLE_BOTTOM) pnewport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pnewport->m_nStyle == PORT_STYLE_RIGHT) pnewport->m_nStyle = PORT_STYLE_TOP;
					else if(pnewport->m_nStyle == PORT_STYLE_TOP) pnewport->m_nStyle = PORT_STYLE_LEFT;
				}
				else if(rotation == 180)
				{
					if(pnewport->m_nStyle == PORT_STYLE_LEFT)  pnewport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pnewport->m_nStyle == PORT_STYLE_RIGHT) pnewport->m_nStyle = PORT_STYLE_LEFT;
					else if(pnewport->m_nStyle == PORT_STYLE_BOTTOM) pnewport->m_nStyle = PORT_STYLE_TOP;
					else if(pnewport->m_nStyle == PORT_STYLE_TOP) pnewport->m_nStyle = PORT_STYLE_BOTTOM;
				}
				else if(rotation == 270)
				{
					if(pnewport->m_nStyle == PORT_STYLE_NH) pnewport->m_nStyle = PORT_STYLE_NV;
					else if(pnewport->m_nStyle == PORT_STYLE_NV) pnewport->m_nStyle = PORT_STYLE_NH;
					else if(pnewport->m_nStyle == PORT_STYLE_LEFTRIGHT) pnewport->m_nStyle = PORT_STYLE_TOPBOTTOM;
					else if(pnewport->m_nStyle == PORT_STYLE_TOPBOTTOM) pnewport->m_nStyle = PORT_STYLE_LEFTRIGHT;
					else if(pnewport->m_nStyle == PORT_STYLE_LEFT)  pnewport->m_nStyle = PORT_STYLE_TOP;
					else if(pnewport->m_nStyle == PORT_STYLE_BOTTOM) pnewport->m_nStyle = PORT_STYLE_LEFT;
					else if(pnewport->m_nStyle == PORT_STYLE_RIGHT) pnewport->m_nStyle = PORT_STYLE_BOTTOM;
					else if(pnewport->m_nStyle == PORT_STYLE_TOP) pnewport->m_nStyle = PORT_STYLE_RIGHT;
				}

				if(mirror == TRUE)
				{
					if(pnewport->m_nStyle == PORT_STYLE_LEFT)  pnewport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pnewport->m_nStyle == PORT_STYLE_RIGHT) pnewport->m_nStyle = PORT_STYLE_LEFT;
				}

				if(pnewport->m_nStyle < PORT_STYLE_NV)
				{
					pnewport->m_fX -= pnewport->m_fSize;
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
				COb_schpower* pnewpower = new COb_schpower();
				pnewpower->Copy(ppower);
				pnewpower->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpower);
						
				
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
				
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pnewpower->m_fX - x0, pnewpower->m_fY - y0, rotation, mirror);
				pnewpower->m_fX = fx + x0 + deltax;
				pnewpower->m_fY = fy + y0 + deltay;
				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, pnewpower->m_nOrientation, rotation, mirror);
				pnewpower->m_nOrientation = orientation;
				pnewpower->m_nOrientation = pDoc->Get_Correct_Orientation(pnewpower->m_nOrientation);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if(pbezier->m_bSelection == TRUE)
			{
				COb_schbezier* pnewbezier = new COb_schbezier();
				pnewbezier->Copy(pbezier);
				pnewbezier->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewbezier);
						
				
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
				
				for(int k=0; k<pnewbezier->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pnewbezier->m_cVertex[k].fx - x0, pnewbezier->m_cVertex[k].fy - y0, rotation, mirror);
					pnewbezier->m_cVertex[k].fx = fx + x0 + deltax;
					pnewbezier->m_cVertex[k].fy = fy + y0 + deltay;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			if(pgraphic->m_bSelection == TRUE)
			{
				COb_schgraphic* pnewgraphic = new COb_schgraphic();
				pnewgraphic->Copy(pgraphic);
				pnewgraphic->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewgraphic);
						
				
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
				
				double fx, fy, dx, dy;
				dx = pnewgraphic->m_fX2 - pnewgraphic->m_fX1;
				dy = pnewgraphic->m_fY2 - pnewgraphic->m_fY1;
				pDoc->Get_Rotate_XY(&fx, &fy, pnewgraphic->m_fX1 - x0, pnewgraphic->m_fY1 - y0, rotation, mirror);
				pnewgraphic->m_fX1 = fx + x0 + deltax;
				pnewgraphic->m_fY1 = fy + y0 + deltay;
				pnewgraphic->m_fX2 = pnewgraphic->m_fX1 + dx;
				pnewgraphic->m_fY2 = pnewgraphic->m_fY1 + dy;
			}
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
			if(pcomp->m_bSelection == TRUE)
			{
				COb_schcomp* pnewcomp = new COb_schcomp();
				pnewcomp->Copy(pcomp);
				pnewcomp->m_bSelection = FALSE;
				POSITION newpos = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewcomp);
						
				
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
				

				double fx1, fy1;
				pDoc->Get_Rotate_XY(&fx1, &fy1, pnewcomp->m_fX - x0, pnewcomp->m_fY - y0, rotation, mirror);
				pnewcomp->m_fX = fx1 + x0 + deltax;
				pnewcomp->m_fY = fy1 + y0 + deltay;
				pnewcomp->m_nOrientation += rotation;
				pnewcomp->m_nOrientation = pDoc->Get_Correct_Orientation(pnewcomp->m_nOrientation);
				if(mirror == TRUE)
				{
					if(pnewcomp->m_bMirrored == TRUE) pnewcomp->m_bMirrored = FALSE;
					else pnewcomp->m_bMirrored = TRUE;
				}

				pnewcomp->m_arrayPos.RemoveAll();
				
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
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpin);

						
						pnewcomp->m_arrayPos.Add(pos1);

						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewpin->m_fX - x0, pnewpin->m_fY - y0, rotation, mirror);
						pnewpin->m_fX = fx1 + x0 + deltax;
						pnewpin->m_fY = fy1 + y0 + deltay;

						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, pnewpin->m_nOrientation, rotation, mirror);
						pnewpin->m_nOrientation = orientation;
						pnewpin->m_nOrientation = pDoc->Get_Correct_Orientation(pnewpin->m_nOrientation);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
					{
						COb_schieee* pieee = (COb_schieee*)pobject;
						
						COb_schieee* pnewieee = new COb_schieee();
						pnewieee->Copy(pieee);
						pnewieee->m_posParent = newpos;
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewieee);

						
						pnewcomp->m_arrayPos.Add(pos1);

						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewieee->m_fX - x0, pnewieee->m_fY - y0, rotation, mirror);
						pnewieee->m_fX = fx1 + x0 + deltax;
						pnewieee->m_fY = fy1 + y0 + deltay;
						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, pnewieee->m_nOrientation, rotation, mirror);
						pnewieee->m_nOrientation = orientation;
						pnewieee->m_nOrientation = pDoc->Get_Correct_Orientation(pnewieee->m_nOrientation);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						
						COb_schtext* pnewtext = new COb_schtext();
						pnewtext->Copy(ptext);
						pnewtext->m_posParent = newpos;
						pnewtext->m_bSelection = FALSE;
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtext);

						
						pnewcomp->m_arrayPos.Add(pos1);

						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewtext->m_fSx - x0, pnewtext->m_fSy - y0, rotation, mirror);
						pnewtext->m_fSx = fx1 + x0 + deltax;
						pnewtext->m_fSy = fy1 + y0 + deltay;
						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, pnewtext->m_nOrientation, rotation, FALSE); 
						pnewtext->m_nOrientation = orientation;
						pnewtext->m_nOrientation = pDoc->Get_Correct_Orientation(pnewtext->m_nOrientation);
			
						if(mirror == TRUE)
						{
							if(pnewtext->m_bMirror == TRUE) pnewtext->m_bMirror = FALSE;
							else pnewtext->m_bMirror = TRUE;
							if((pnewtext->m_nOrientation == 0)||(pnewtext->m_nOrientation == 180))
							{
								if(pnewtext->m_nH_Justification == 0) pnewtext->m_nH_Justification = 2;
								else if(pnewtext->m_nH_Justification == 2) pnewtext->m_nH_Justification = 0;
							}
							else
							{
								if(pnewtext->m_nV_Justification == 0) pnewtext->m_nV_Justification = 2;
								else if(pnewtext->m_nV_Justification == 2) pnewtext->m_nV_Justification = 0;
							}
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
					{
						COb_schbezier* pbezier = (COb_schbezier*)pobject;
						
						COb_schbezier* pnewbezier = new COb_schbezier();
						pnewbezier->Copy(pbezier);
						pnewbezier->m_posParent = newpos;
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewbezier);

						
						pnewcomp->m_arrayPos.Add(pos1);				

						for(int k=0; k<pnewbezier->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, pnewbezier->m_cVertex[k].fx-x0, pnewbezier->m_cVertex[k].fy-y0, rotation, mirror);
							pnewbezier->m_cVertex[k].fx = fx1 + x0 + deltax;
							pnewbezier->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
					{
						COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;

						COb_schpolyline* pnewpolyline = new COb_schpolyline();
						pnewpolyline->Copy(ppolyline);
						pnewpolyline->m_posParent = newpos;
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolyline);

						
						pnewcomp->m_arrayPos.Add(pos1);				

						for(int k=0; k<pnewpolyline->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, pnewpolyline->m_cVertex[k].fx-x0, pnewpolyline->m_cVertex[k].fy-y0, rotation, mirror);
							pnewpolyline->m_cVertex[k].fx = fx1 + x0 + deltax;
							pnewpolyline->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
					{
						COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
						
						COb_schpolygon* pnewpolygon = new COb_schpolygon();
						pnewpolygon->Copy(ppolygon);
						pnewpolygon->m_posParent = newpos;
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpolygon);

						
						pnewcomp->m_arrayPos.Add(pos1);					

						for(int k=0; k<pnewpolygon->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, pnewpolygon->m_cVertex[k].fx-x0, pnewpolygon->m_cVertex[k].fy-y0, rotation, mirror);
							pnewpolygon->m_cVertex[k].fx = fx1 + x0 + deltax;
							pnewpolygon->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
					{
						COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
						
						COb_schellipsefill* pnewellipsefill = new COb_schellipsefill();
						pnewellipsefill->Copy(pellipsefill);
						pnewellipsefill->m_posParent = newpos;
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipsefill);

						
						pnewcomp->m_arrayPos.Add(pos1);	

						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewellipsefill->m_fCx - x0, pnewellipsefill->m_fCy - y0, rotation, mirror);
						pnewellipsefill->m_fCx = fx1 + x0 + deltax;
						pnewellipsefill->m_fCy = fy1 + y0 + deltay;
						if((rotation == 90)||(rotation == 270))
						{
							double ftemp;
							ftemp = pnewellipsefill->m_fRadius_y;
							pnewellipsefill->m_fRadius_y = pnewellipsefill->m_fRadius_x;
							pnewellipsefill->m_fRadius_x = ftemp;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
					{
						COb_schpie* ppie = (COb_schpie*)pobject;
						
						COb_schpie* pnewpie = new COb_schpie();
						pnewpie->Copy(ppie);
						pnewpie->m_posParent = newpos;
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewpie);

						
						pnewcomp->m_arrayPos.Add(pos1);	

						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewpie->m_fCx - x0, pnewpie->m_fCy - y0, rotation, mirror);
						pnewpie->m_fCx = fx1 + x0 + deltax;
						pnewpie->m_fCy = fy1 + y0 + deltay;
								
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, pnewpie->m_fSangle, pnewpie->m_fEangle, rotation, mirror);
						pnewpie->m_fSangle = salpha;
						pnewpie->m_fSangle = pDoc->Get_Correct_Angle(pnewpie->m_fSangle);
						pnewpie->m_fEangle = ealpha;
						pnewpie->m_fEangle = pDoc->Get_Correct_Angle(pnewpie->m_fEangle);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
					{
						COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
						
						COb_schroundrect* pnewroundrect = new COb_schroundrect();
						pnewroundrect->Copy(proundrect);
						pnewroundrect->m_posParent = newpos;
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewroundrect);

						
						pnewcomp->m_arrayPos.Add(pos1);	
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewroundrect->m_fX1 - x0, pnewroundrect->m_fY1 - y0, rotation, mirror);
						pnewroundrect->m_fX1 = fx1 + x0 + deltax;
						pnewroundrect->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewroundrect->m_fX2 - x0, pnewroundrect->m_fY2 - y0, rotation, mirror);
						pnewroundrect->m_fX2 = fx1 + x0 + deltax;
						pnewroundrect->m_fY2 = fy1 + y0 + deltay;
								
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(pnewroundrect->m_fX1, pnewroundrect->m_fX2);
						fmaxx = MAX(pnewroundrect->m_fX1, pnewroundrect->m_fX2);
						fminy = MIN(pnewroundrect->m_fY1, pnewroundrect->m_fY2);
						fmaxy = MAX(pnewroundrect->m_fY1, pnewroundrect->m_fY2);
						pnewroundrect->m_fX1 = fminx;
						pnewroundrect->m_fY1 = fminy;
						pnewroundrect->m_fX2 = fmaxx;
						pnewroundrect->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
					{
						COb_schellipse* pellipse = (COb_schellipse*)pobject;
						
						COb_schellipse* pnewellipse = new COb_schellipse();
						pnewellipse->Copy(pellipse);
						pnewellipse->m_posParent = newpos;
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewellipse);

						
						pnewcomp->m_arrayPos.Add(pos1);	

						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewellipse->m_fCx - x0, pnewellipse->m_fCy - y0, rotation, mirror);
						pnewellipse->m_fCx = fx1 + x0 + deltax;
						pnewellipse->m_fCy = fy1 + y0 + deltay;
								
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, pnewellipse->m_fSangle, pnewellipse->m_fEangle, rotation, mirror);
						pnewellipse->m_fSangle = salpha;
						pnewellipse->m_fSangle = pDoc->Get_Correct_Angle(pnewellipse->m_fSangle);
						pnewellipse->m_fEangle = ealpha;
						pnewellipse->m_fEangle = pDoc->Get_Correct_Angle(pnewellipse->m_fEangle);
						if((rotation == 90)||(rotation == 270))
						{
							double ftemp;
							ftemp = pnewellipse->m_fRadius_y;
							pnewellipse->m_fRadius_y = pnewellipse->m_fRadius_x;
							pnewellipse->m_fRadius_x = ftemp;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
					{
						COb_scharc* parc = (COb_scharc*)pobject;

						COb_scharc* pnewarc = new COb_scharc();
						pnewarc->Copy(parc);
						pnewarc->m_posParent = newpos;
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewarc);

						
						pnewcomp->m_arrayPos.Add(pos1);	

						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewarc->m_fCx - x0, pnewarc->m_fCy - y0, rotation, mirror);
						pnewarc->m_fCx = fx1 + x0 + deltax;
						pnewarc->m_fCy = fy1 + y0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, pnewarc->m_fSangle, pnewarc->m_fEangle, rotation, mirror);
						pnewarc->m_fSangle = salpha;
						pnewarc->m_fSangle = pDoc->Get_Correct_Angle(pnewarc->m_fSangle);
						pnewarc->m_fEangle = ealpha;
						pnewarc->m_fEangle = pDoc->Get_Correct_Angle(pnewarc->m_fEangle);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
					{
						COb_schrect* prect = (COb_schrect*)pobject;
						
						COb_schrect* pnewrect = new COb_schrect();
						pnewrect->Copy(prect);
						pnewrect->m_posParent = newpos;
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewrect);

						
						pnewcomp->m_arrayPos.Add(pos1);	

						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewrect->m_fX1 - x0, pnewrect->m_fY1 - y0, rotation, mirror);
						pnewrect->m_fX1 = fx1 + x0 + deltax;
						pnewrect->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewrect->m_fX2 - x0, pnewrect->m_fY2 - y0, rotation, mirror);
						pnewrect->m_fX2 = fx1 + x0 + deltax;
						pnewrect->m_fY2 = fy1 + y0 + deltay;
								
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(pnewrect->m_fX1, pnewrect->m_fX2);
						fmaxx = MAX(pnewrect->m_fX1, pnewrect->m_fX2);
						fminy = MIN(pnewrect->m_fY1, pnewrect->m_fY2);
						fmaxy = MAX(pnewrect->m_fY1, pnewrect->m_fY2);
						pnewrect->m_fX1 = fminx;
						pnewrect->m_fY1 = fminy;
						pnewrect->m_fX2 = fmaxx;
						pnewrect->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
					{
						COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
						
						COb_schtextframe* pnewtextframe = new COb_schtextframe();
						pnewtextframe->Copy(ptextframe);
						pnewtextframe->m_posParent = newpos;
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtextframe);

						
						pnewcomp->m_arrayPos.Add(pos1);	

						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewtextframe->m_fX1 - x0, pnewtextframe->m_fY1 - y0, rotation, mirror);
						pnewtextframe->m_fX1 = fx1 + x0 + deltax;
						pnewtextframe->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewtextframe->m_fX2 - x0, pnewtextframe->m_fY2 - y0, rotation, mirror);
						pnewtextframe->m_fX2 = fx1 + x0 + deltax;
						pnewtextframe->m_fY2 = fy1 + y0 + deltay;
								
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(pnewtextframe->m_fX1, pnewtextframe->m_fX2);
						fmaxx = MAX(pnewtextframe->m_fX1, pnewtextframe->m_fX2);
						fminy = MIN(pnewtextframe->m_fY1, pnewtextframe->m_fY2);
						fmaxy = MAX(pnewtextframe->m_fY1, pnewtextframe->m_fY2);
						pnewtextframe->m_fX1 = fminx;
						pnewtextframe->m_fY1 = fminy;
						pnewtextframe->m_fX2 = fmaxx;
						pnewtextframe->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
					{
						COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
						
						COb_schgraphic* pnewgraphic = new COb_schgraphic();
						pnewgraphic->Copy(pgraphic);
						pnewgraphic->m_posParent = newpos;
						
						POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewgraphic);

						
						pnewcomp->m_arrayPos.Add(pos1);	

						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewgraphic->m_fX1 - x0, pnewgraphic->m_fY1 - y0, rotation, mirror);
						pnewgraphic->m_fX1 = fx1 + x0 + deltax;
						pnewgraphic->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, pnewgraphic->m_fX2 - x0, pnewgraphic->m_fY2 - y0, rotation, mirror);
						pnewgraphic->m_fX2 = fx1 + x0 + deltax;
						pnewgraphic->m_fY2 = fy1 + y0 + deltay;
								
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(pnewgraphic->m_fX1, pnewgraphic->m_fX2);
						fmaxx = MAX(pnewgraphic->m_fX1, pnewgraphic->m_fX2);
						fminy = MIN(pnewgraphic->m_fY1, pnewgraphic->m_fY2);
						fmaxy = MAX(pnewgraphic->m_fY1, pnewgraphic->m_fY2);
						pnewgraphic->m_fX1 = fminx;
						pnewgraphic->m_fY1 = fminy;
						pnewgraphic->m_fX2 = fmaxx;
						pnewgraphic->m_fY2 = fmaxy;
					}
				}
			}
			
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
	}
	
	if(undo_flag == TRUE)
	{
		if(bEDA_AutoOptimizeWire == TRUE) AutoOptimizeWire();
		if(bEDA_AutoJunction == TRUE) AutoPlaceJunction();

		pDoc->SetModifiedFlag(TRUE);
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}	
	
	Invalidate();
}
