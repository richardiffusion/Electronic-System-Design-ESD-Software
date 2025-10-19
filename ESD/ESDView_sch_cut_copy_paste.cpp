void CESDView::OnEditCut()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	if(nEDA_System_State == SCH)
	{
		
		while (!listEDA_SchClipBoard.IsEmpty())
		{
			delete listEDA_SchClipBoard.RemoveHead();
		}

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
					
					COb_schcomp* pnewcomp = new COb_schcomp();
					pnewcomp->Copy(pcomp);
					pnewcomp->m_bSelection = FALSE;
					POSITION newpos = listEDA_SchClipBoard.AddTail(pnewcomp);
						
					
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

							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewpin);
							pnewcomp->m_arrayPos.Add(pos1);
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
						{
							COb_schieee* pieee = (COb_schieee*)pobject;
							
							COb_schieee* pnewieee = new COb_schieee();
							pnewieee->Copy(pieee);
							pnewieee->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewieee);
							pnewcomp->m_arrayPos.Add(pos1);
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;
										
							COb_schtext* pnewtext = new COb_schtext();
							pnewtext->Copy(ptext);
							pnewtext->m_posParent = newpos;
							pnewtext->m_bSelection = FALSE;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewtext);
							pnewcomp->m_arrayPos.Add(pos1);

							
							if(arrayEDA_ClipBoardFont.GetCount() < (ptext->m_nFont + 1)) arrayEDA_ClipBoardFont.SetSize(ptext->m_nFont + 1);
							LOGFONT logfont;
							logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(ptext->m_nFont);
							arrayEDA_ClipBoardFont.SetAt(ptext->m_nFont, logfont);
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
						{
							COb_schbezier* pbezier = (COb_schbezier*)pobject;
										
							COb_schbezier* pnewbezier = new COb_schbezier();
							pnewbezier->Copy(pbezier);
							pnewbezier->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewbezier);
							pnewcomp->m_arrayPos.Add(pos1);				
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
						{
							COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;

							COb_schpolyline* pnewpolyline = new COb_schpolyline();
							pnewpolyline->Copy(ppolyline);
							pnewpolyline->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewpolyline);
							pnewcomp->m_arrayPos.Add(pos1);				
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
						{
							COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
								
							COb_schpolygon* pnewpolygon = new COb_schpolygon();
							pnewpolygon->Copy(ppolygon);
							pnewpolygon->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewpolygon);
							pnewcomp->m_arrayPos.Add(pos1);					
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
						{
							COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
										
							COb_schellipsefill* pnewellipsefill = new COb_schellipsefill();
							pnewellipsefill->Copy(pellipsefill);
							pnewellipsefill->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewellipsefill);
							pnewcomp->m_arrayPos.Add(pos1);	
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
						{
							COb_schpie* ppie = (COb_schpie*)pobject;
								
							COb_schpie* pnewpie = new COb_schpie();
							pnewpie->Copy(ppie);
							pnewpie->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewpie);
							pnewcomp->m_arrayPos.Add(pos1);	
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
						{
							COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
								
							COb_schroundrect* pnewroundrect = new COb_schroundrect();
							pnewroundrect->Copy(proundrect);
							pnewroundrect->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewroundrect);
							pnewcomp->m_arrayPos.Add(pos1);	
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
						{
							COb_schellipse* pellipse = (COb_schellipse*)pobject;
								
							COb_schellipse* pnewellipse = new COb_schellipse();
							pnewellipse->Copy(pellipse);
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewellipse);
							pnewcomp->m_arrayPos.Add(pos1);	
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
						{
							COb_scharc* parc = (COb_scharc*)pobject;

							COb_scharc* pnewarc = new COb_scharc();
							pnewarc->Copy(parc);
							pnewarc->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewarc);
							pnewcomp->m_arrayPos.Add(pos1);	
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
						{
							COb_schrect* prect = (COb_schrect*)pobject;
										
							COb_schrect* pnewrect = new COb_schrect();
							pnewrect->Copy(prect);
							pnewrect->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewrect);
							pnewcomp->m_arrayPos.Add(pos1);	
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
						{
							COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
										
							COb_schtextframe* pnewtextframe = new COb_schtextframe();
							pnewtextframe->Copy(ptextframe);
							pnewtextframe->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewtextframe);
							pnewcomp->m_arrayPos.Add(pos1);	

							
							if(arrayEDA_ClipBoardFont.GetCount() < (ptextframe->m_nFont + 1)) arrayEDA_ClipBoardFont.SetSize(ptextframe->m_nFont + 1);
							LOGFONT logfont;
							logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(ptextframe->m_nFont);
							arrayEDA_ClipBoardFont.SetAt(ptextframe->m_nFont, logfont);
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
						{
							COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
										
							COb_schgraphic* pnewgraphic = new COb_schgraphic();
							pnewgraphic->Copy(pgraphic);
							pnewgraphic->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewgraphic);
							pnewcomp->m_arrayPos.Add(pos1);	
						}
					}

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
					
					COb_schtext* pnewtext = new COb_schtext();
					pnewtext->Copy(ptext);
					pnewtext->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewtext);
					
					if(arrayEDA_ClipBoardFont.GetCount() < (ptext->m_nFont + 1)) arrayEDA_ClipBoardFont.SetSize(ptext->m_nFont + 1);
					LOGFONT logfont;
					logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(ptext->m_nFont);
					arrayEDA_ClipBoardFont.SetAt(ptext->m_nFont, logfont);

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
					
					COb_schbezier* pnewbezier = new COb_schbezier();
					pnewbezier->Copy(pbezier);
					pnewbezier->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewbezier);

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
					
					COb_schpolyline* pnewpolyline = new COb_schpolyline();
					pnewpolyline->Copy(ppolyline);
					pnewpolyline->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewpolyline);

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
					
					COb_schpolygon* pnewpolygon = new COb_schpolygon();
					pnewpolygon->Copy(ppolygon);
					pnewpolygon->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewpolygon);

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
					
					COb_schellipsefill* pnewellipsefill = new COb_schellipsefill();
					pnewellipsefill->Copy(pellipsefill);
					pnewellipsefill->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewellipsefill);
					
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
					
					COb_schpie* pnewpie = new COb_schpie();
					pnewpie->Copy(ppie);
					pnewpie->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewpie);

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
					
					COb_schroundrect* pnewroundrect = new COb_schroundrect();
					pnewroundrect->Copy(proundrect);
					pnewroundrect->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewroundrect);

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
					
					COb_schellipse* pnewellipse = new COb_schellipse();
					pnewellipse->Copy(pellipse);
					pnewellipse->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewellipse);

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
					
					COb_scharc* pnewarc = new COb_scharc();
					pnewarc->Copy(parc);
					pnewarc->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewarc);

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
					
					COb_schline* pnewline = new COb_schline();
					pnewline->Copy(pline);
					pnewline->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewline);

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
					
					COb_schrect* pnewrect = new COb_schrect();
					pnewrect->Copy(prect);
					pnewrect->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewrect);

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
					
					COb_schsheetsymbol* pnewsheetsymbol = new COb_schsheetsymbol();
					pnewsheetsymbol->Copy(psheetsymbol);
					pnewsheetsymbol->m_bSelection = FALSE;
					POSITION newpos = listEDA_SchClipBoard.AddTail(pnewsheetsymbol);
						
					
					pnewsheetsymbol->m_arrayPos.RemoveAll();
					for(int i=0; i<pnewsheetsymbol->m_arrayPos.GetSize(); i++)
					{
						CObject* pobject;
						POSITION subpos;
						subpos = pnewsheetsymbol->m_arrayPos.GetAt(i);
						pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
						{
							COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
										
							COb_schsheetentry* pnewsheetentry = new COb_schsheetentry();
							pnewsheetentry->Copy(psheetentry);
							pnewsheetentry->m_posParent = newpos;

							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewsheetentry);
							pnewsheetsymbol->m_arrayPos.Add(pos1);
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;
										
							COb_schtext* pnewtext = new COb_schtext();
							pnewtext->Copy(ptext);
							pnewtext->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewtext);
							pnewsheetsymbol->m_arrayPos.Add(pos1);
							
							if(arrayEDA_ClipBoardFont.GetCount() < (ptext->m_nFont + 1)) arrayEDA_ClipBoardFont.SetSize(ptext->m_nFont + 1);
							LOGFONT logfont;
							logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(ptext->m_nFont);
							arrayEDA_ClipBoardFont.SetAt(ptext->m_nFont, logfont);
						}
					}

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
					
					COb_schpower* pnewpower = new COb_schpower();
					pnewpower->Copy(ppower);
					pnewpower->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewpower);

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
					
					COb_schport* pnewport = new COb_schport();
					pnewport->Copy(pport);
					pnewport->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewport);

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
					
					COb_schtextframe* pnewtextframe = new COb_schtextframe();
					pnewtextframe->Copy(ptextframe);
					pnewtextframe->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewtextframe);
					
					if(arrayEDA_ClipBoardFont.GetCount() < (ptextframe->m_nFont + 1)) arrayEDA_ClipBoardFont.SetSize(ptextframe->m_nFont + 1);
					LOGFONT logfont;
					logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(ptextframe->m_nFont);
					arrayEDA_ClipBoardFont.SetAt(ptextframe->m_nFont, logfont);

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
					
					COb_schjunc* pnewjunc = new COb_schjunc();
					pnewjunc->Copy(pjunc);
					pnewjunc->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewjunc);

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
					
					COb_schgraphic* pnewgraphic = new COb_schgraphic();
					pnewgraphic->Copy(pgraphic);
					pnewgraphic->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewgraphic);

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
		
		fEDA_ClipBoard_X1 = pDoc->m_fSelection_X1[m_nDiagram];
		fEDA_ClipBoard_Y1 = pDoc->m_fSelection_Y1[m_nDiagram];
		fEDA_ClipBoard_X2 = pDoc->m_fSelection_X2[m_nDiagram];
		fEDA_ClipBoard_Y2 = pDoc->m_fSelection_Y2[m_nDiagram];
		
		if(undo_flag == TRUE)
		{
			AutoRemoveJunction();

			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		}	
		
		pDoc->m_arraySelection[m_nDiagram].RemoveAll();
		Invalidate();
	}
}

void CESDView::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(pDoc->m_arraySelection[m_nDiagram].GetCount() > 0) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CESDView::OnEditPaste()
{
	int ch1;
	int flag;
	BOOL mirror;
	double x0, y0, x1, y1, deltax, deltay;

	char szHelp[] = "粘贴: <左键><回车>粘贴, <空格>旋转, <X键>镜像, <右键><ESC>结束命令";
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	x0 = fEDA_ClipBoard_X1;
	y0 = fEDA_ClipBoard_Y1;
	m_fCurrentSnapX = x0;
	m_fCurrentSnapY = y0;
	Restore_LargeCrossXY();

	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);	

	
	COb_schrect arect;
	arect.pDocument = pDoc;
	arect.m_fX1 = fEDA_ClipBoard_X1;
	arect.m_fY1 = fEDA_ClipBoard_Y1;
	arect.m_fX2 = fEDA_ClipBoard_X2;
	arect.m_fY2 = fEDA_ClipBoard_Y2;
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
			pDoc->Get_Rotate_XY(&fx, &fy, fEDA_ClipBoard_X1 - x0, fEDA_ClipBoard_Y1 - y0, flag*90, mirror);
			arect.m_fX1 = fx + x0 + deltax;
			arect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, fEDA_ClipBoard_X2 - x0, fEDA_ClipBoard_Y2 - y0, flag*90, mirror);
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
					
			int rotation = flag*90;

			BOOL clear_undo_buffer_flag = FALSE;	
			BOOL undo_flag = FALSE;  

			POSITION posbak;
			CObject* pobject;
			CESDDoc* pDoc = (CESDDoc*)GetDocument();

			int num = listEDA_SchClipBoard.GetCount();
			POSITION pos;
			pos = listEDA_SchClipBoard.GetHeadPosition();
			for(int i=0; i<num; i++)
			{	
				if(pos == NULL) break;
				posbak = pos;
				pobject = listEDA_SchClipBoard.GetNext(pos);
				
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
				{
					COb_schline* pline = (COb_schline*)pobject;

					COb_schline* pnewline = new COb_schline();
					pnewline->Copy(pline);
					pnewline->pDocument = pDoc;
					pnewline->m_nDiagram = m_nDiagram;
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
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
				{
					COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;

					COb_schpolyline* pnewpolyline = new COb_schpolyline();
					pnewpolyline->Copy(ppolyline);
					pnewpolyline->pDocument = pDoc;
					pnewpolyline->m_nDiagram = m_nDiagram;
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
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
				{
					COb_schjunc* pjunc = (COb_schjunc*)pobject;

					COb_schjunc* pnewjunc = new COb_schjunc();
					pnewjunc->Copy(pjunc);
					pnewjunc->pDocument = pDoc;
					pnewjunc->m_nDiagram = m_nDiagram;
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
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext = (COb_schtext*)pobject;

					COb_schtext* pnewtext = new COb_schtext();
					pnewtext->Copy(ptext);
					pnewtext->pDocument = pDoc;
					pnewtext->m_nDiagram = m_nDiagram;
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
					pDoc->Get_Rotate_Orientation(&orientation, pnewtext->m_nOrientation, rotation, FALSE); //mirror);
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

					LOGFONT logfont = arrayEDA_ClipBoardFont.GetAt(ptext->m_nFont);
					int font_index = pDoc->CheckFontExist(&logfont, m_nDiagram);
					if(font_index < 0) font_index = pDoc->m_arrayFont[m_nDiagram].Add(logfont);
					pnewtext->m_nFont = font_index;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
				{
					COb_scharc* parc = (COb_scharc*)pobject;

					COb_scharc* pnewarc = new COb_scharc();
					pnewarc->Copy(parc);
					pnewarc->pDocument = pDoc;
					pnewarc->m_nDiagram = m_nDiagram;
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
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
				{
					COb_schellipse* pellipse = (COb_schellipse*)pobject;

					COb_schellipse* pnewellipse = new COb_schellipse();
					pnewellipse->Copy(pellipse);
					pnewellipse->pDocument = pDoc;
					pnewellipse->m_nDiagram = m_nDiagram;
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
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
				{
					COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;

					COb_schellipsefill* pnewellipsefill = new COb_schellipsefill();
					pnewellipsefill->Copy(pellipsefill);
					pnewellipsefill->pDocument = pDoc;
					pnewellipsefill->m_nDiagram = m_nDiagram;
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
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
				{
					COb_schpie* ppie = (COb_schpie*)pobject;

					COb_schpie* pnewpie = new COb_schpie();
					pnewpie->Copy(ppie);
					pnewpie->pDocument = pDoc;
					pnewpie->m_nDiagram = m_nDiagram;
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
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
				{
					COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;

					COb_schpolygon* pnewpolygon = new COb_schpolygon();
					pnewpolygon->Copy(ppolygon);
					pnewpolygon->pDocument = pDoc;
					pnewpolygon->m_nDiagram = m_nDiagram;
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
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
				{
					COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;

					COb_schtextframe* pnewtextframe = new COb_schtextframe();
					pnewtextframe->Copy(ptextframe);
					pnewtextframe->pDocument = pDoc;
					pnewtextframe->m_nDiagram = m_nDiagram;
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

					LOGFONT logfont = arrayEDA_ClipBoardFont.GetAt(ptextframe->m_nFont);
					int font_index = pDoc->CheckFontExist(&logfont, m_nDiagram);
					if(font_index < 0) font_index = pDoc->m_arrayFont[m_nDiagram].Add(logfont);
					pnewtextframe->m_nFont = font_index;
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
				{
					COb_schrect* prect = (COb_schrect*)pobject;

					COb_schrect* pnewrect = new COb_schrect();
					pnewrect->Copy(prect);
					pnewrect->pDocument = pDoc;
					pnewrect->m_nDiagram = m_nDiagram;
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
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
				{
					COb_schroundrect* proundrect = (COb_schroundrect*)pobject;

					COb_schroundrect* pnewroundrect = new COb_schroundrect();
					pnewroundrect->Copy(proundrect);
					pnewroundrect->pDocument = pDoc;
					pnewroundrect->m_nDiagram = m_nDiagram;
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
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
				{
					COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;

					double dx, dy;
					COb_schsheetsymbol* pnewsheetsymbol = new COb_schsheetsymbol();
					pnewsheetsymbol->Copy(psheetsymbol);
					pnewsheetsymbol->pDocument = pDoc;
					pnewsheetsymbol->m_nDiagram = m_nDiagram;
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
						pobject = listEDA_SchClipBoard.GetNext(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
						{
							COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;

							COb_schsheetentry* pnewsheetentry = new COb_schsheetentry();
							pnewsheetentry->Copy(psheetentry);
							pnewsheetentry->pDocument = pDoc;
							pnewsheetentry->m_nDiagram = m_nDiagram;
							pnewsheetentry->m_posParent = newpos;
							
							POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewsheetentry);

							
							pnewsheetsymbol->m_arrayPos.Add(pos1);
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;

							COb_schtext* pnewtext = new COb_schtext();
							pnewtext->Copy(ptext);
							pnewtext->pDocument = pDoc;
							pnewtext->m_nDiagram = m_nDiagram;
							pnewtext->m_posParent = newpos;
							POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtext);
							
							pnewsheetsymbol->m_arrayPos.Add(pos1);
							pnewtext->m_fSx += dx;
							pnewtext->m_fSy += dy;
							
							LOGFONT logfont = arrayEDA_ClipBoardFont.GetAt(ptext->m_nFont);
							int font_index = pDoc->CheckFontExist(&logfont, m_nDiagram);
							if(font_index < 0) font_index = pDoc->m_arrayFont[m_nDiagram].Add(logfont);
							pnewtext->m_nFont = font_index;
						}
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
				{
					COb_schport* pport = (COb_schport*)pobject;

					COb_schport* pnewport = new COb_schport();
					pnewport->Copy(pport);
					pnewport->pDocument = pDoc;
					pnewport->m_nDiagram = m_nDiagram;
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
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
				{
					COb_schpower* ppower = (COb_schpower*)pobject;

					COb_schpower* pnewpower = new COb_schpower();
					pnewpower->Copy(ppower);
					pnewpower->pDocument = pDoc;
					pnewpower->m_nDiagram = m_nDiagram;
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
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
				{
					COb_schbezier* pbezier = (COb_schbezier*)pobject;

					COb_schbezier* pnewbezier = new COb_schbezier();
					pnewbezier->Copy(pbezier);
					pnewbezier->pDocument = pDoc;
					pnewbezier->m_nDiagram = m_nDiagram;
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
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
				{
					COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;

					COb_schgraphic* pnewgraphic = new COb_schgraphic();
					pnewgraphic->Copy(pgraphic);
					pnewgraphic->pDocument = pDoc;
					pnewgraphic->m_nDiagram = m_nDiagram;
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
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
				{
					COb_schcomp* pcomp = (COb_schcomp*)pobject;

					COb_schcomp* pnewcomp = new COb_schcomp();
					pnewcomp->Copy(pcomp);
					pnewcomp->pDocument = pDoc;
					pnewcomp->m_nDiagram = m_nDiagram;
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
					
					for(int k=0; k<pcomp->m_arrayPos.GetSize(); k++)
					{
						CObject* pobject;
						POSITION subpos;
						subpos = pcomp->m_arrayPos.GetAt(k);
						pobject = listEDA_SchClipBoard.GetAt(subpos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
						{
							COb_schpin* ppin = (COb_schpin*)pobject;
								
							COb_schpin* pnewpin = new COb_schpin();
							pnewpin->Copy(ppin);
							pnewpin->pDocument = pDoc;
							pnewpin->m_nDiagram = m_nDiagram;
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
							pnewieee->pDocument = pDoc;
							pnewieee->m_nDiagram = m_nDiagram;
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
							pnewtext->pDocument = pDoc;
							pnewtext->m_nDiagram = m_nDiagram;
							pnewtext->m_posParent = newpos;
							
							POSITION pos1 = pDoc->m_listSchObject[m_nDiagram].AddTail(pnewtext);

							
							pnewcomp->m_arrayPos.Add(pos1);

							pDoc->Get_Rotate_XY(&fx1, &fy1, pnewtext->m_fSx - x0, pnewtext->m_fSy - y0, rotation, mirror);
							pnewtext->m_fSx = fx1 + x0 + deltax;
							pnewtext->m_fSy = fy1 + y0 + deltay;
							int orientation;
							pDoc->Get_Rotate_Orientation(&orientation, pnewtext->m_nOrientation, rotation, FALSE); //mirror);
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

							LOGFONT logfont = arrayEDA_ClipBoardFont.GetAt(ptext->m_nFont);
							int font_index = pDoc->CheckFontExist(&logfont, m_nDiagram);
							if(font_index < 0) font_index = pDoc->m_arrayFont[m_nDiagram].Add(logfont);
							pnewtext->m_nFont = font_index;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
						{
							COb_schbezier* pbezier = (COb_schbezier*)pobject;
							
							COb_schbezier* pnewbezier = new COb_schbezier();
							pnewbezier->Copy(pbezier);
							pnewbezier->pDocument = pDoc;
							pnewbezier->m_nDiagram = m_nDiagram;
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
							pnewpolyline->pDocument = pDoc;
							pnewpolyline->m_nDiagram = m_nDiagram;
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
							pnewpolygon->pDocument = pDoc;
							pnewpolygon->m_nDiagram = m_nDiagram;
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
							pnewellipsefill->pDocument = pDoc;
							pnewellipsefill->m_nDiagram = m_nDiagram;
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
							pnewpie->pDocument = pDoc;
							pnewpie->m_nDiagram = m_nDiagram;
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
							pnewroundrect->pDocument = pDoc;
							pnewroundrect->m_nDiagram = m_nDiagram;
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
							pnewellipse->pDocument = pDoc;
							pnewellipse->m_nDiagram = m_nDiagram;
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
							pnewarc->pDocument = pDoc;
							pnewarc->m_nDiagram = m_nDiagram;
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
							pnewrect->pDocument = pDoc;
							pnewrect->m_posParent = newpos;
							pnewrect->m_nDiagram = m_nDiagram;
							
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
							pnewtextframe->pDocument = pDoc;
							pnewtextframe->m_nDiagram = m_nDiagram;
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
							
							LOGFONT logfont = arrayEDA_ClipBoardFont.GetAt(ptextframe->m_nFont);
							int font_index = pDoc->CheckFontExist(&logfont, m_nDiagram);
							if(font_index < 0) font_index = pDoc->m_arrayFont[m_nDiagram].Add(logfont);
							pnewtextframe->m_nFont = font_index;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
						{
							COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
							
							COb_schgraphic* pnewgraphic = new COb_schgraphic();
							pnewgraphic->Copy(pgraphic);
							pnewgraphic->pDocument = pDoc;
							pnewgraphic->m_nDiagram = m_nDiagram;
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
					
					for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) listEDA_SchClipBoard.GetNext(pos);
				}
			}
			
			if(undo_flag == TRUE)
			{
				pDoc->m_nSCH_Undo_Num++;
				if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
			}	
			
			Invalidate();

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
			pDoc->Get_Rotate_XY(&fx, &fy, fEDA_ClipBoard_X1 - x0, fEDA_ClipBoard_Y1 - y0, flag*90, mirror);
			arect.m_fX1 = fx + x0 + deltax;
			arect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, fEDA_ClipBoard_X2 - x0, fEDA_ClipBoard_Y2 - y0, flag*90, mirror);
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
			pDoc->Get_Rotate_XY(&fx, &fy, fEDA_ClipBoard_X1 - x0, fEDA_ClipBoard_Y1 - y0, flag*90, mirror);
			arect.m_fX1 = fx + x0 + deltax;
			arect.m_fY1 = fy + y0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, fEDA_ClipBoard_X2 - x0, fEDA_ClipBoard_Y2 - y0, flag*90, mirror);
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

void CESDView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	if(listEDA_SchClipBoard.GetCount() > 0) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CESDView::OnEditCopy()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	LOGFONT logfont;

	if(nEDA_System_State == SCH)
	{
		
		while (!listEDA_SchClipBoard.IsEmpty())
		{
			delete listEDA_SchClipBoard.RemoveHead();
		}
		arrayEDA_ClipBoardFont.RemoveAll();

		CObject* pobject;

		int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
		POSITION pos;
		pos = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
		for(int i=0; i<num; i++)
		{	
			if(pos == NULL) break;
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
					
					COb_schcomp* pnewcomp = new COb_schcomp();
					pnewcomp->Copy(pcomp);
					pnewcomp->m_bSelection = FALSE;
					POSITION newpos = listEDA_SchClipBoard.AddTail(pnewcomp);
						
					
					pnewcomp->m_arrayPos.RemoveAll();
					for(int k=0; k<pcomp->m_arrayPos.GetSize(); k++)
					{
						CObject* pobject;
						POSITION subpos;
						subpos = pcomp->m_arrayPos.GetAt(k);
						pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
						{
							COb_schpin* ppin = (COb_schpin*)pobject;
										
							COb_schpin* pnewpin = new COb_schpin();
							pnewpin->Copy(ppin);
							pnewpin->m_posParent = newpos;

							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewpin);
							pnewcomp->m_arrayPos.Add(pos1);
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
						{
							COb_schieee* pieee = (COb_schieee*)pobject;
							
							COb_schieee* pnewieee = new COb_schieee();
							pnewieee->Copy(pieee);
							pnewieee->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewieee);
							pnewcomp->m_arrayPos.Add(pos1);
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;
										
							COb_schtext* pnewtext = new COb_schtext();
							pnewtext->Copy(ptext);
							pnewtext->m_posParent = newpos;
							pnewtext->m_bSelection = FALSE;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewtext);
							pnewcomp->m_arrayPos.Add(pos1);

							if(arrayEDA_ClipBoardFont.GetCount() < (ptext->m_nFont + 1)) arrayEDA_ClipBoardFont.SetSize(ptext->m_nFont + 1);
							LOGFONT logfont;
							logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(ptext->m_nFont);
							arrayEDA_ClipBoardFont.SetAt(ptext->m_nFont, logfont);
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
						{
							COb_schbezier* pbezier = (COb_schbezier*)pobject;
										
							COb_schbezier* pnewbezier = new COb_schbezier();
							pnewbezier->Copy(pbezier);
							pnewbezier->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewbezier);
							pnewcomp->m_arrayPos.Add(pos1);				
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
						{
							COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;

							COb_schpolyline* pnewpolyline = new COb_schpolyline();
							pnewpolyline->Copy(ppolyline);
							pnewpolyline->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewpolyline);
							pnewcomp->m_arrayPos.Add(pos1);				
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
						{
							COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
								
							COb_schpolygon* pnewpolygon = new COb_schpolygon();
							pnewpolygon->Copy(ppolygon);
							pnewpolygon->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewpolygon);
							pnewcomp->m_arrayPos.Add(pos1);					
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
						{
							COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
										
							COb_schellipsefill* pnewellipsefill = new COb_schellipsefill();
							pnewellipsefill->Copy(pellipsefill);
							pnewellipsefill->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewellipsefill);
							pnewcomp->m_arrayPos.Add(pos1);	
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
						{
							COb_schpie* ppie = (COb_schpie*)pobject;
								
							COb_schpie* pnewpie = new COb_schpie();
							pnewpie->Copy(ppie);
							pnewpie->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewpie);
							pnewcomp->m_arrayPos.Add(pos1);	
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
						{
							COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
								
							COb_schroundrect* pnewroundrect = new COb_schroundrect();
							pnewroundrect->Copy(proundrect);
							pnewroundrect->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewroundrect);
							pnewcomp->m_arrayPos.Add(pos1);	
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
						{
							COb_schellipse* pellipse = (COb_schellipse*)pobject;
								
							COb_schellipse* pnewellipse = new COb_schellipse();
							pnewellipse->Copy(pellipse);
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewellipse);
							pnewcomp->m_arrayPos.Add(pos1);	
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
						{
							COb_scharc* parc = (COb_scharc*)pobject;

							COb_scharc* pnewarc = new COb_scharc();
							pnewarc->Copy(parc);
							pnewarc->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewarc);
							pnewcomp->m_arrayPos.Add(pos1);	
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
						{
							COb_schrect* prect = (COb_schrect*)pobject;
										
							COb_schrect* pnewrect = new COb_schrect();
							pnewrect->Copy(prect);
							pnewrect->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewrect);
							pnewcomp->m_arrayPos.Add(pos1);	
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
						{
							COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
										
							COb_schtextframe* pnewtextframe = new COb_schtextframe();
							pnewtextframe->Copy(ptextframe);
							pnewtextframe->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewtextframe);
							pnewcomp->m_arrayPos.Add(pos1);	
							
							if(arrayEDA_ClipBoardFont.GetCount() < (ptextframe->m_nFont + 1)) arrayEDA_ClipBoardFont.SetSize(ptextframe->m_nFont + 1);
							LOGFONT logfont;
							logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(ptextframe->m_nFont);
							arrayEDA_ClipBoardFont.SetAt(ptextframe->m_nFont, logfont);
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
						{
							COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
										
							COb_schgraphic* pnewgraphic = new COb_schgraphic();
							pnewgraphic->Copy(pgraphic);
							pnewgraphic->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewgraphic);
							pnewcomp->m_arrayPos.Add(pos1);	
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
					
					COb_schtext* pnewtext = new COb_schtext();
					pnewtext->Copy(ptext);
					pnewtext->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewtext);
					
					if(arrayEDA_ClipBoardFont.GetCount() < (ptext->m_nFont + 1)) arrayEDA_ClipBoardFont.SetSize(ptext->m_nFont + 1);
					LOGFONT logfont;
					logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(ptext->m_nFont);
					arrayEDA_ClipBoardFont.SetAt(ptext->m_nFont, logfont);
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
					listEDA_SchClipBoard.AddTail(pnewbezier);
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
					listEDA_SchClipBoard.AddTail(pnewpolyline);
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
					listEDA_SchClipBoard.AddTail(pnewpolygon);
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
					listEDA_SchClipBoard.AddTail(pnewellipsefill);
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
					listEDA_SchClipBoard.AddTail(pnewpie);
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
					listEDA_SchClipBoard.AddTail(pnewroundrect);
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
					listEDA_SchClipBoard.AddTail(pnewellipse);
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
					listEDA_SchClipBoard.AddTail(pnewarc);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
				if( pline->m_bSelection == TRUE)
				{
					
					COb_schline* pnewline = new COb_schline();
					pnewline->Copy(pline);
					pnewline->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewline);
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
					listEDA_SchClipBoard.AddTail(pnewrect);
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
					
					COb_schsheetsymbol* pnewsheetsymbol = new COb_schsheetsymbol();
					pnewsheetsymbol->Copy(psheetsymbol);
					pnewsheetsymbol->m_bSelection = FALSE;
					POSITION newpos = listEDA_SchClipBoard.AddTail(pnewsheetsymbol);
						
					
					pnewsheetsymbol->m_arrayPos.RemoveAll();
					for(int i=0; i<pnewsheetsymbol->m_arrayPos.GetSize(); i++)
					{
						CObject* pobject;
						POSITION subpos;
						subpos = pnewsheetsymbol->m_arrayPos.GetAt(i);
						pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
						{
							COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
										
							COb_schsheetentry* pnewsheetentry = new COb_schsheetentry();
							pnewsheetentry->Copy(psheetentry);
							pnewsheetentry->m_posParent = newpos;

							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewsheetentry);
							pnewsheetsymbol->m_arrayPos.Add(pos1);
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;
										
							COb_schtext* pnewtext = new COb_schtext();
							pnewtext->Copy(ptext);
							pnewtext->m_posParent = newpos;
							POSITION pos1 = listEDA_SchClipBoard.AddTail(pnewtext);
							pnewsheetsymbol->m_arrayPos.Add(pos1);
							
							if(arrayEDA_ClipBoardFont.GetCount() < (ptext->m_nFont + 1)) arrayEDA_ClipBoardFont.SetSize(ptext->m_nFont + 1);
							LOGFONT logfont;
							logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(ptext->m_nFont);
							arrayEDA_ClipBoardFont.SetAt(ptext->m_nFont, logfont);
						}
					}

					for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
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
					listEDA_SchClipBoard.AddTail(pnewpower);
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
					listEDA_SchClipBoard.AddTail(pnewport);
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
					listEDA_SchClipBoard.AddTail(pnewtextframe);
					
					if(arrayEDA_ClipBoardFont.GetCount() < (ptextframe->m_nFont + 1)) arrayEDA_ClipBoardFont.SetSize(ptextframe->m_nFont + 1);
					LOGFONT logfont;
					logfont = pDoc->m_arrayFont[m_nDiagram].GetAt(ptextframe->m_nFont);
					arrayEDA_ClipBoardFont.SetAt(ptextframe->m_nFont, logfont);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc;
				pjunc = (COb_schjunc*)pobject;
				if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
				if(pjunc->m_bSelection == TRUE)
				{
					
					COb_schjunc* pnewjunc = new COb_schjunc();
					pnewjunc->Copy(pjunc);
					pnewjunc->m_bSelection = FALSE;
					listEDA_SchClipBoard.AddTail(pnewjunc);
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
					listEDA_SchClipBoard.AddTail(pnewgraphic);
				}
			}
		}

		fEDA_ClipBoard_X1 = pDoc->m_fSelection_X1[m_nDiagram];
		fEDA_ClipBoard_Y1 = pDoc->m_fSelection_Y1[m_nDiagram];
		fEDA_ClipBoard_X2 = pDoc->m_fSelection_X2[m_nDiagram];
		fEDA_ClipBoard_Y2 = pDoc->m_fSelection_Y2[m_nDiagram];

	}
}

void CESDView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(pDoc->m_arraySelection[m_nDiagram].GetCount() > 0) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}
void CESDView::OnEditOriginReset()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->m_cPCB_Sheet.m_fOriginX = 0;
	pDoc->m_cPCB_Sheet.m_fOriginY = 0;

	Invalidate();
}

void CESDView::OnEditOriginSet()
{
	unsigned ch1;
	double fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	if(pDoc == NULL) return;

	MoveCursorToCenter();

	Write_Help("定义相对坐标原点");
	do
	{ 
		ch1 = MouseKey();
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC)&&(ch1 != PUSHMOUSE)&&(ch1 != VK_RETURN));
			
	pDoc->m_cPCB_Sheet.m_fOriginX = m_fCurrentSnapX; 
	pDoc->m_cPCB_Sheet.m_fOriginY = m_fCurrentSnapY;
	Invalidate();

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}
