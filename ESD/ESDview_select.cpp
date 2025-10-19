struct Struct_Multi_Selection CESDView::SelectSCHObject(double fx, double fy, int mode)
{
	Struct_Multi_Selection selected_element;
	CArray<Struct_Multi_Selection, Struct_Multi_Selection&> array_element;

	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	int num = pDoc->m_listSchObject[m_nDiagram].GetCount();
	POSITION pos, position;
	pos = pDoc->m_listSchObject[m_nDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{	
		if(pos == NULL) break;
		position = pos;
		pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
			if(pline->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_LINE;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("line\n");
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			if(ppolyline->Is_On_Me(fx, fy) == TRUE)
			{
				selected_element.element = SCH_ELEMENT_POLYLINE;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("polyline\n");
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED)||(pjunc->m_bManual == FALSE))	continue;
			if(pjunc->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_JUNCTION;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("junction\n");
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if(ptext->Is_On_Me(fx, fy, ptext->m_nDiagram, this) == TRUE) 
			{
				if(ptext->m_nID == TEXT_TEXT) 
				{
					selected_element.element = SCH_ELEMENT_TEXT;
					selected_element.pos = position;
					if(mode == THE_FIRST_ONE) return selected_element;
					array_element.Add(selected_element);
					TRACE("Text:  ");
				}
				else if(ptext->m_nID == TEXT_NETLABEL)
				{
					selected_element.element = SCH_ELEMENT_TEXT;
					selected_element.pos = position;
					if(mode == THE_FIRST_ONE) return selected_element;
					array_element.Add(selected_element);
					TRACE("Netlabel:  ");
				}
				else if(ptext->m_nID == TEXT_SHEETTEXT)
				{

				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if(parc->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_ARC;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("arc\n");
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if(pellipse->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_ELLIPSE;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("ellipse\n");
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if(pellipsefill->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_ELLIPSEFILL;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("ellipsefill\n");
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if(ppie->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_PIE;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("pie\n");
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if(ppolygon->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_POLYGON;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("polygon\n");
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
			if(ptextframe->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_TEXTFRAME;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("textframe\n");
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if(prect->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_RECT;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("rect\n");
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if(proundrect->Is_On_Me(fx, fy) == TRUE)
			{
				selected_element.element = SCH_ELEMENT_ROUNDRECT;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("roundrect\n");
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

			
			int entry_flag = 0;
			POSITION posbak = position;
			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
			{
				position = pos;
				pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
				{
					COb_schsheetentry* psheetentry;
					psheetentry = (COb_schsheetentry*)pobject;
					if((psheetentry->m_nDiagram != m_nDiagram)||(psheetentry->m_nFlag == FLAG_DELETED))	continue;
					if(psheetentry->Is_On_Me(fx, fy) == TRUE)
					{
						selected_element.element = SCH_ELEMENT_SHEETENTRY;
						selected_element.pos = position;
						if(mode == THE_FIRST_ONE) return selected_element;
						array_element.Add(selected_element);
						entry_flag = 1;
						TRACE("sheet entry\n");
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext;
					ptext = (COb_schtext*)pobject;
					if(ptext->Is_On_Me(fx, fy, ptext->m_nDiagram, this) == TRUE) 
					{
						if(ptext->m_nID == TEXT_SHEETSYMBOLDESIGNATOR) 
						{
							selected_element.element = SCH_ELEMENT_TEXT; 
							selected_element.pos = position;
							if(mode == THE_FIRST_ONE) return selected_element;
							array_element.Add(selected_element);
							TRACE("designator\n");
						}
						else 
						{
							selected_element.element = SCH_ELEMENT_TEXT; 
							selected_element.pos = position;
							if(mode == THE_FIRST_ONE) return selected_element;
							array_element.Add(selected_element);
							TRACE("filename\n");
						}
					}
				}
			}

			if(entry_flag == 0)
			{
				if(psheetsymbol->Is_On_Me(fx, fy) == TRUE)
				{
					selected_element.element = SCH_ELEMENT_SHEETSYMBOL;
					selected_element.pos = posbak;
					if(mode == THE_FIRST_ONE) return selected_element;
					array_element.Add(selected_element);
					TRACE("sheetsymbol\n");
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(pport->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_PORT;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("port\n");
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			if(ppower->Is_On_Me(fx, fy, this) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_POWER;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("power\n");
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if(pbezier->Is_On_Me(fx, fy) == TRUE)
			{
				selected_element.element = SCH_ELEMENT_BEZIER;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("bezier\n");
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			if(pgraphic->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_GRAPHIC;
				selected_element.pos = position;
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
				TRACE("graphic\n");
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram != m_nDiagram)||(pcomp->m_nFlag == FLAG_DELETED))
			{
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)	pDoc->m_listSchObject[m_nDiagram].GetNext(pos);

				continue;
			}
			
			selected_element = pcomp->Is_On_Me(fx, fy, pDoc, this, m_nDiagram, position);

			if(selected_element.element != 0) 
			{
				if(mode == THE_FIRST_ONE) return selected_element;
				array_element.Add(selected_element);
			}
			
			if(selected_element.element == SCH_ELEMENT_COMPONENT) TRACE("Component\n");
			else if(selected_element.element == SCH_ELEMENT_PIN) TRACE("Pin\n");

			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
			{
				pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
			}
		}
	}

	if(array_element.GetCount() == 0)
	{
		selected_element.element = 0;
		selected_element.pos = NULL;
	}
	else if(array_element.GetCount() == 1)
	{
		selected_element.element = array_element[0].element;
		selected_element.pos = array_element[0].pos;
	}
	else
	{
		CMenu menu;
		CMenu* psubmenu;
		menu.LoadMenu(IDR_MULTI_SELECTION);
		psubmenu = menu.GetSubMenu(0);
		nEDA_Multi_Selection = -1;

		int total = MIN(array_element.GetCount(), 20);
		int i;
		for(i=0; i<total; i++)
		{
			int element = array_element[i].element;
			psubmenu->ModifyMenu(i, MF_BYPOSITION, ID_SELECTION_1 + i,  szEDA_Multi_Selection[element]);
		}
		for(int j=i; j<20; j++)
		{
			psubmenu->DeleteMenu(i, MF_BYPOSITION);
		}

		int x,y;
		Get_VPort_XY(&x, &y, fx, fy);
		CPoint pt;
		pt.x = x; pt.y = y;
		ClientToScreen(&pt);
		x = pt.x; y = pt.y; 
		psubmenu->TrackPopupMenu(TPM_LEFTALIGN , x, y, this);

		MSG mymsg;
		PeekMessage(&mymsg,NULL,0,0,PM_REMOVE);
		DispatchMessage(&mymsg);

		if(nEDA_Multi_Selection >= 0)
		{
			selected_element.element = array_element[nEDA_Multi_Selection].element;
			selected_element.pos = array_element[nEDA_Multi_Selection].pos;
		}
		else
		{
			selected_element.element = 0;
			selected_element.pos = NULL;
		}
		
		
		nEDA_Multi_Selection = -1;
	}

	return selected_element;
}

void CESDView::SelectElement(int element, POSITION position)
{
	TRACE1("Selection:%d\n", element);
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	if((element == 0)||( position == NULL)) return;

	pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(position);

	if(element == SCH_ELEMENT_COMPONENT)
	{
		COb_schcomp* pcomp = (COb_schcomp*)pobject;
		pcomp->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_PIN)
	{
		COb_schpin* ppin = (COb_schpin*)pobject;
		ppin->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_IEEE)
	{
		COb_schieee* pieee = (COb_schieee*)pobject;
		pieee->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_TEXT)  
	{
		COb_schtext* ptext = (COb_schtext*)pobject;
		ptext->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_BEZIER)
	{
		COb_schbezier* pbezier = (COb_schbezier*)pobject;
		pbezier->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_POLYLINE)
	{
		COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
		ppolyline->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_POLYGON)
	{
		COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
		ppolygon->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_ELLIPSEFILL)
	{
		COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
		pellipsefill->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_PIE)
	{
		COb_schpie* ppie = (COb_schpie*)pobject;
		ppie->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_ROUNDRECT)
	{
		COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
		proundrect->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_ELLIPSE)
	{
		COb_schellipse* pellipse = (COb_schellipse*)pobject;
		pellipse->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_ARC)
	{
		COb_scharc* parc = (COb_scharc*)pobject;
		parc->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_LINE)	
	{
		COb_schline* pline = (COb_schline*)pobject;
		pline->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_RECT)
	{
		COb_schrect* prect = (COb_schrect*)pobject;
		prect->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_SHEETSYMBOL)
	{
		COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
		psheetsymbol->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_SHEETENTRY)
	{
		COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
		psheetentry->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_POWER)
	{
		COb_schpower* ppower = (COb_schpower*)pobject;
		ppower->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_PORT)
	{
		COb_schport* pport = (COb_schport*)pobject;
		pport->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_TEXTFRAME)
	{
		COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
		ptextframe->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_JUNCTION)
	{
		COb_schjunc* pjunc = (COb_schjunc*)pobject;
		pjunc->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_GRAPHIC)
	{
		COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
		pgraphic->m_bSelection = TRUE;
	}
	else if(element == SCH_ELEMENT_SHEET)
	{
		
	}

	
	pDoc->m_fpSelection[m_nDiagram] = GetSelectionCenter();
}

void CESDView::MouseSelectInsideArea(void)
{
	unsigned ch1;
	CClientDC dc(this);
	double fx1,fy1,fx2,fy2;
	int xs,ys,xe,ye,flag;

	int drawmode;
	drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	fx1 = m_fCurrentSnapX; fy1 = m_fCurrentSnapY;
	fx2 = fx1; fy2 = fy1;
	Get_VPort_XY(&xs,&ys,fx1,fy1);
	xe=xs;
	ye=ys;
	flag = 0;
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
			
			if(GetAsyncKeyState(VK_LBUTTON) >= 0) break;
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
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN)||(ch1 == MOUSE_LBUTTON_UP))
		{  
			dc.MoveTo(xs,ys);
			dc.LineTo(xe,ys);
			dc.LineTo(xe,ye);
			dc.LineTo(xs,ye);
			dc.LineTo(xs,ys);
			break;
		}
		else Command_In_Function(ch1);
	}while(1);

	if((fx1 != fx2)&&(fy1 != fy2))
	{
		double fminx, fminy, fmaxx, fmaxy;
		fminx = MIN(fx1, fx2);
		fmaxx = MAX(fx1, fx2);
		fminy = MIN(fy1, fy2);
		fmaxy = MAX(fy1, fy2);

		SelectInsideArea(fminx, fminy, fmaxx, fmaxy, m_nDiagram);
	}

	dc.SetROP2(drawmode);
	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}


void CESDView::SelectInsideArea(double fx1, double fy1, double fx2, double fy2, int nDiagram)
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
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
			if(pline->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pline->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			if(ppolyline->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppolyline->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
			if(pjunc->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pjunc->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if(ptext->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ptext->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if(parc->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) parc->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if(pellipse->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pellipse->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if(pellipsefill->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pellipsefill->m_bSelection = TRUE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if(ppie->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppie->m_bSelection = TRUE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if(ppolygon->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppolygon->m_bSelection = TRUE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
			if(ptextframe->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ptextframe->m_bSelection = TRUE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if(prect->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) prect->m_bSelection = TRUE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if(proundrect->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) proundrect->m_bSelection = TRUE;	
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
			if(psheetsymbol->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE)
			{
				psheetsymbol->m_bSelection = TRUE;	

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
				{
					pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
					{
						COb_schsheetentry* psheetentry;
						psheetentry = (COb_schsheetentry*)pobject;
						if((psheetentry->m_nDiagram != m_nDiagram)||(psheetentry->m_nFlag == FLAG_DELETED))	continue;
						
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext;
						ptext = (COb_schtext*)pobject;
						ptext->m_bSelection = TRUE;	
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(pport->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pport->m_bSelection = TRUE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			if(ppower->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppower->m_bSelection = TRUE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if(pbezier->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE)
			{
				pbezier->m_bSelection = TRUE;	
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			if(pgraphic->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pgraphic->m_bSelection = TRUE;
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
			if(pcomp->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE)
			{
				pcomp->m_bSelection = TRUE;
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
				{
					pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext;
						ptext = (COb_schtext*)pobject;
						ptext->m_bSelection = TRUE;
					}
				}
			}
		}
	}

	
	pDoc->m_fpSelection[m_nDiagram] = GetSelectionCenter();
}

void CESDView::DrawSelectionKeyPoint(int x, int y, CDC* pDC)
{
	int d = 4;
	
	COLORREF color;
	if(m_nSystem_State == SCH)  color = nEDA_SelectionColor;
	else color = nEDA_PCB_LayerColor[SELECTIONS];

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(0,0,0));
	CPen* oldpen =  pDC->SelectObject(&pen);
 
	CBrush brush;
	brush.CreateSolidBrush(color);
	CBrush* oldbrush = pDC->SelectObject(&brush);
	
	int x1,y1,x2,y2;
	x1 = x - d;
	y1 = y - d;
	x2 = x + d;
	y2 = y + d;

	pDC->Rectangle(x1,y1,x2,y2);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void CESDView::HideSelectionKeyPoint(int x, int y, CDC* pDC)
{
	int d = 4;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();	

	COLORREF color;
	if(m_nSystem_State == SCH)  color = pDoc->m_pSCH_Sheet[m_nDiagram]->m_nSheetColor;
	else color = nEDA_PCB_BoardAreaColor;

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, color);
	CPen* oldpen =  pDC->SelectObject(&pen);
 
	CBrush brush;
	brush.CreateSolidBrush(color);
	CBrush* oldbrush = pDC->SelectObject(&brush);
	
	int x1,y1,x2,y2;
	x1 = x - d;
	y1 = y - d;
	x2 = x + d;
	y2 = y + d;

	pDC->Rectangle(x1,y1,x2,y2);

	pDC->SelectObject(oldpen);
	pDC->SelectObject(oldbrush);
}

void CESDView::SelectAll(BOOL Is_Selecting)
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
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) pline->m_bSelection = TRUE;
			else pline->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) ppolyline->m_bSelection = TRUE;
			else ppolyline->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) pjunc->m_bSelection = TRUE;
			else pjunc->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) ptext->m_bSelection = TRUE;
			else ptext->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) parc->m_bSelection = TRUE;
			else parc->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) pellipse->m_bSelection = TRUE;
			else pellipse->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) pellipsefill->m_bSelection = TRUE;
			else pellipsefill->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) ppie->m_bSelection = TRUE;
			else ppie->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) ppolygon->m_bSelection = TRUE;
			else ppolygon->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) ptextframe->m_bSelection = TRUE;
			else ptextframe->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) prect->m_bSelection = TRUE;
			else prect->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) proundrect->m_bSelection = TRUE;
			else proundrect->m_bSelection = FALSE;	
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
			if(Is_Selecting == TRUE) psheetsymbol->m_bSelection = TRUE;
			else psheetsymbol->m_bSelection = FALSE;	

			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
			{
				pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
				{
					COb_schsheetentry* psheetentry;
					psheetentry = (COb_schsheetentry*)pobject;
					if((psheetentry->m_nDiagram != m_nDiagram)||(psheetentry->m_nFlag == FLAG_DELETED))	continue;
					if(Is_Selecting == TRUE) {} 
					else psheetentry->m_bSelection = FALSE;	
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext;
					ptext = (COb_schtext*)pobject;
					if(Is_Selecting == TRUE) ptext->m_bSelection = TRUE;
					else ptext->m_bSelection = FALSE;	
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) pport->m_bSelection = TRUE;
			else pport->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) ppower->m_bSelection = TRUE;
			else ppower->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) pbezier->m_bSelection = TRUE;
			else pbezier->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			if(Is_Selecting == TRUE) pgraphic->m_bSelection = TRUE;
			else pgraphic->m_bSelection = FALSE;	
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
			if(Is_Selecting == TRUE) pcomp->m_bSelection = TRUE;
			else pcomp->m_bSelection = FALSE;
			
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
			{
				pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext;
					ptext = (COb_schtext*)pobject;
					if(Is_Selecting == TRUE) ptext->m_bSelection = TRUE;
					else ptext->m_bSelection = FALSE;	
				}
			}
		}
	}

	
	pDoc->m_fpSelection[m_nDiagram] = GetSelectionCenter();

}

void CESDView::ToggleSelection(void)
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
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
			if( pline->m_bSelection == TRUE) pline->m_bSelection = FALSE;
			else pline->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			if( ppolyline->m_bSelection == TRUE) ppolyline->m_bSelection = FALSE;
			else ppolyline->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
			if( pjunc->m_bSelection == TRUE) pjunc->m_bSelection = FALSE;
			else pjunc->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if( ptext->m_bSelection == TRUE) ptext->m_bSelection = FALSE;
			else ptext->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if( parc->m_bSelection == TRUE) parc->m_bSelection = FALSE;
			else parc->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if( pellipse->m_bSelection == TRUE) pellipse->m_bSelection = FALSE;
			else pellipse->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if( pellipsefill->m_bSelection == TRUE) pellipsefill->m_bSelection = FALSE;
			else pellipsefill->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if( ppie->m_bSelection == TRUE) ppie->m_bSelection = FALSE;
			else ppie->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if( ppolygon->m_bSelection == TRUE) ppolygon->m_bSelection = FALSE;
			else ppolygon->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
			if( ptextframe->m_bSelection == TRUE) ptextframe->m_bSelection = FALSE;
			else ptextframe->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if( prect->m_bSelection == TRUE) prect->m_bSelection = FALSE;
			else prect->m_bSelection = TRUE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if( proundrect->m_bSelection == TRUE) proundrect->m_bSelection = FALSE;
			else proundrect->m_bSelection = TRUE;
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
			if( psheetsymbol->m_bSelection == TRUE) psheetsymbol->m_bSelection = FALSE;
			else psheetsymbol->m_bSelection = TRUE;	

			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
			{
				pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
				{
					COb_schsheetentry* psheetentry;
					psheetentry = (COb_schsheetentry*)pobject;
					if((psheetentry->m_nDiagram != m_nDiagram)||(psheetentry->m_nFlag == FLAG_DELETED))	continue;
					
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext;
					ptext = (COb_schtext*)pobject;
					if( ptext->m_bSelection == TRUE) ptext->m_bSelection = FALSE;
					else ptext->m_bSelection = TRUE;	
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if( pport->m_bSelection == TRUE) pport->m_bSelection = FALSE;
			else pport->m_bSelection = TRUE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			if( ppower->m_bSelection == TRUE) ppower->m_bSelection = FALSE;
			else ppower->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if( pbezier->m_bSelection == TRUE) pbezier->m_bSelection = FALSE;
			else pbezier->m_bSelection = TRUE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			if( pgraphic->m_bSelection == TRUE) pgraphic->m_bSelection = FALSE;
			else pgraphic->m_bSelection = TRUE;	
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
			if( pcomp->m_bSelection == TRUE) pcomp->m_bSelection = FALSE;
			else pcomp->m_bSelection = TRUE;	
			
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
			{
				pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext;
					ptext = (COb_schtext*)pobject;
					if( ptext->m_bSelection == TRUE) ptext->m_bSelection = FALSE;
					else ptext->m_bSelection = TRUE;
				}
			}
		}
	}
}

void CESDView::DeSelectInsideArea(double fx1, double fy1, double fx2, double fy2, int nDiagram)
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
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
			if(pline->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pline->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			if(ppolyline->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppolyline->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
			if(pjunc->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pjunc->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if(ptext->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ptext->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if(parc->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) parc->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if(pellipse->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pellipse->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if(pellipsefill->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pellipsefill->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if(ppie->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppie->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if(ppolygon->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppolygon->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
			if(ptextframe->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ptextframe->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if(prect->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) prect->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if(proundrect->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) proundrect->m_bSelection = FALSE;	
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
			if(psheetsymbol->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE)
			{
				psheetsymbol->m_bSelection = FALSE;	

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
				{
					pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
					{
						COb_schsheetentry* psheetentry;
						psheetentry = (COb_schsheetentry*)pobject;
						if((psheetentry->m_nDiagram != m_nDiagram)||(psheetentry->m_nFlag == FLAG_DELETED))	continue;
						psheetentry->m_bSelection = FALSE;	
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext;
						ptext = (COb_schtext*)pobject;
						ptext->m_bSelection = FALSE;	
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(pport->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pport->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			if(ppower->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppower->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if(pbezier->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE)	pbezier->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			if(pgraphic->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pgraphic->m_bSelection = FALSE;
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
			if(pcomp->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE)
			{
				pcomp->m_bSelection = FALSE;
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
				{
					pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext;
						ptext = (COb_schtext*)pobject;
						ptext->m_bSelection = FALSE;
					}
				}
			}
		}
	}

	
	pDoc->m_fpSelection[m_nDiagram] = GetSelectionCenter();
}


void CESDView::DeSelectOutsideArea(double fx1, double fy1, double fx2, double fy2, int nDiagram)
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
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
			if(pline->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) pline->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			if(ppolyline->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) ppolyline->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
			if(pjunc->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) pjunc->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if(ptext->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) ptext->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if(parc->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) parc->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if(pellipse->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) pellipse->m_bSelection = FALSE;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if(pellipsefill->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) pellipsefill->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if(ppie->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) ppie->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if(ppolygon->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) ppolygon->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
			if(ptextframe->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) ptextframe->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if(prect->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) prect->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if(proundrect->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) proundrect->m_bSelection = FALSE;	
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
			if(psheetsymbol->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE)
			{
				psheetsymbol->m_bSelection = FALSE;	

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
				{
					pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
					{
						COb_schsheetentry* psheetentry;
						psheetentry = (COb_schsheetentry*)pobject;
						if((psheetentry->m_nDiagram != m_nDiagram)||(psheetentry->m_nFlag == FLAG_DELETED))	continue;
						psheetentry->m_bSelection = FALSE;	
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext;
						ptext = (COb_schtext*)pobject;
						ptext->m_bSelection = FALSE;	
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(pport->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) pport->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			if(ppower->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) ppower->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if(pbezier->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE)	pbezier->m_bSelection = FALSE;	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			if(pgraphic->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) pgraphic->m_bSelection = FALSE;
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
			if(pcomp->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE)
			{
				pcomp->m_bSelection = FALSE;
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
				{
					pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext;
						ptext = (COb_schtext*)pobject;
						ptext->m_bSelection = FALSE;
					}
				}
			}
		}
	}

	
	pDoc->m_fpSelection[m_nDiagram] = GetSelectionCenter();
}

Fpoint CESDView::GetSelectionCenter(void)
{
	double fminx, fminy, fmaxx, fmaxy;
	POSITION posbak;
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	fminx = fminy = 100000;
	fmaxx = fmaxy = -100000;
	pDoc->m_arraySelection[m_nDiagram].RemoveAll();

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
			if(pline->m_bSelection == TRUE)
			{
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				for(int i=0; i<pline->m_cVertex.GetCount(); i++)
				{
					fminx = MIN(fminx, pline->m_cVertex[i].fx);
					fmaxx = MAX(fmaxx, pline->m_cVertex[i].fx);
					fminy = MIN(fminy, pline->m_cVertex[i].fy);
					fmaxy = MAX(fmaxy, pline->m_cVertex[i].fy);
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			if(ppolyline->m_bSelection == TRUE)
			{
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				for(int i=0; i<ppolyline->m_cVertex.GetCount(); i++)
				{
					fminx = MIN(fminx, ppolyline->m_cVertex[i].fx);
					fmaxx = MAX(fmaxx, ppolyline->m_cVertex[i].fx);
					fminy = MIN(fminy, ppolyline->m_cVertex[i].fy);
					fmaxy = MAX(fmaxy, ppolyline->m_cVertex[i].fy);
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
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				fminx = MIN(fminx, pjunc->m_fCx);
				fmaxx = MAX(fmaxx, pjunc->m_fCx);
				fminy = MIN(fminy, pjunc->m_fCy);
				fmaxy = MAX(fmaxy, pjunc->m_fCy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if(ptext->m_bSelection == TRUE)
			{
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				fminx = MIN(fminx, ptext->m_fSx);
				fmaxx = MAX(fmaxx, ptext->m_fSx);
				fminy = MIN(fminy, ptext->m_fSy);
				fmaxy = MAX(fmaxy, ptext->m_fSy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if(parc->m_bSelection == TRUE)
			{
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				fminx = MIN(fminx, parc->m_fCx);
				fmaxx = MAX(fmaxx, parc->m_fCx);
				fminy = MIN(fminy, parc->m_fCy);
				fmaxy = MAX(fmaxy, parc->m_fCy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if(pellipse->m_bSelection == TRUE)
			{
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				fminx = MIN(fminx, pellipse->m_fCx);
				fmaxx = MAX(fmaxx, pellipse->m_fCx);
				fminy = MIN(fminy, pellipse->m_fCy);
				fmaxy = MAX(fmaxy, pellipse->m_fCy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if(pellipsefill->m_bSelection == TRUE)
			{
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				fminx = MIN(fminx, pellipsefill->m_fCx);
				fmaxx = MAX(fmaxx, pellipsefill->m_fCx);
				fminy = MIN(fminy, pellipsefill->m_fCy);
				fmaxy = MAX(fmaxy, pellipsefill->m_fCy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if(ppie->m_bSelection == TRUE)
			{
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				fminx = MIN(fminx, ppie->m_fCx);
				fmaxx = MAX(fmaxx, ppie->m_fCx);
				fminy = MIN(fminy, ppie->m_fCy);
				fmaxy = MAX(fmaxy, ppie->m_fCy);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if(ppolygon->m_bSelection == TRUE)
			{
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				for(int i=0; i<ppolygon->m_cVertex.GetCount(); i++)
				{
					fminx = MIN(fminx, ppolygon->m_cVertex[i].fx);
					fmaxx = MAX(fmaxx, ppolygon->m_cVertex[i].fx);
					fminy = MIN(fminy, ppolygon->m_cVertex[i].fy);
					fmaxy = MAX(fmaxy, ppolygon->m_cVertex[i].fy);
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
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				fminx = MIN(fminx, ptextframe->m_fX1);
				fmaxx = MAX(fmaxx, ptextframe->m_fX2);
				fminy = MIN(fminy, ptextframe->m_fY1);
				fmaxy = MAX(fmaxy, ptextframe->m_fY2);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if(prect->m_bSelection == TRUE)
			{
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				fminx = MIN(fminx, prect->m_fX1);
				fmaxx = MAX(fmaxx, prect->m_fX2);
				fminy = MIN(fminy, prect->m_fY1);
				fmaxy = MAX(fmaxy, prect->m_fY2);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if(proundrect->m_bSelection == TRUE)
			{
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				fminx = MIN(fminx, proundrect->m_fX1);
				fmaxx = MAX(fmaxx, proundrect->m_fX2);
				fminy = MIN(fminy, proundrect->m_fY1);
				fmaxy = MAX(fmaxy, proundrect->m_fY2);
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
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				fminx = MIN(fminx, psheetsymbol->m_fX);
				fmaxx = MAX(fmaxx, psheetsymbol->m_fX + psheetsymbol->m_fXsize);
				fminy = MIN(fminy, psheetsymbol->m_fY - psheetsymbol->m_fYsize);
				fmaxy = MAX(fmaxy, psheetsymbol->m_fY);
			}	

			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);

		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(pport->m_bSelection == TRUE)
			{
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				fminx = MIN(fminx, pport->m_fX);
				fmaxx = MAX(fmaxx, pport->m_fX + pport->m_fSize);
				fminy = MIN(fminy, pport->m_fY - pport->m_fSize);
				fmaxy = MAX(fmaxy, pport->m_fY);
			}		
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			if(ppower->m_bSelection == TRUE)
			{
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				fminx = MIN(fminx, ppower->m_fX);
				fmaxx = MAX(fmaxx, ppower->m_fX+100);
				fminy = MIN(fminy, ppower->m_fY);
				fmaxy = MAX(fmaxy, ppower->m_fY+100);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if(pbezier->m_bSelection == TRUE)
			{
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				for(int i=0; i<pbezier->m_cVertex.GetCount(); i++)
				{
					fminx = MIN(fminx, pbezier->m_cVertex[i].fx);
					fmaxx = MAX(fmaxx, pbezier->m_cVertex[i].fx);
					fminy = MIN(fminy, pbezier->m_cVertex[i].fy);
					fmaxy = MAX(fmaxy, pbezier->m_cVertex[i].fy);
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
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				fminx = MIN(fminx, pgraphic->m_fX1);
				fmaxx = MAX(fmaxx, pgraphic->m_fX2);
				fminy = MIN(fminy, pgraphic->m_fY1);
				fmaxy = MAX(fmaxy, pgraphic->m_fY2);
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
				pDoc->m_arraySelection[m_nDiagram].Add(posbak);
				RECT rect = pcomp->GetCompRect();
				fminx = MIN(fminx, rect.left);
				fmaxx = MAX(fmaxx, rect.right);
				fminy = MIN(fminy, rect.bottom);
				fmaxy = MAX(fmaxy, rect.top);
			}
			
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
	}	
	
	if(fminx == fmaxx) fmaxx += 100;
	if(fminy == fmaxy) fmaxy += 100;

	pDoc->m_fSelection_X1[m_nDiagram] = fminx;
	pDoc->m_fSelection_Y1[m_nDiagram] = fminy;
	pDoc->m_fSelection_X2[m_nDiagram] = fmaxx;
	pDoc->m_fSelection_Y2[m_nDiagram] = fmaxy;

	Fpoint cp;
	cp.fx = (fmaxx + fminx)/2;
	cp.fy = (fmaxy + fminy)/2;

	int itemp;
	itemp = (int)(cp.fx/100);
	cp.fx = itemp*100;
	itemp = (int)(cp.fy/100);
	cp.fy = itemp*100;

	return cp;
}

struct Struct_Multi_Selection CESDView::FindSelectedObject(double fx, double fy)
{
	Struct_Multi_Selection selected_element;
	selected_element.pos = NULL;
	selected_element.element = 0;

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
			if(pline->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_LINE;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			if(ppolyline->Is_On_Me(fx, fy) == TRUE)
			{
				selected_element.element = SCH_ELEMENT_POLYLINE;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
			if(pjunc->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_JUNCTION;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if(ptext->Is_On_Me(fx, fy, ptext->m_nDiagram, this) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_TEXT;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if(parc->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_ARC;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if(pellipse->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_ELLIPSE;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if(pellipsefill->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_ELLIPSEFILL;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if(ppie->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_PIE;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if(ppolygon->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_POLYGON;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
			if(ptextframe->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_TEXTFRAME;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if(prect->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_RECT;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if(proundrect->Is_On_Me(fx, fy) == TRUE)
			{
				selected_element.element = SCH_ELEMENT_ROUNDRECT;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;
			if((psheetsymbol->m_nDiagram != m_nDiagram)||(psheetsymbol->m_nFlag == FLAG_DELETED)) continue;
			if(psheetsymbol->Is_On_Me(fx, fy) == TRUE)
			{
				selected_element.element = SCH_ELEMENT_SHEETSYMBOL;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
		{
			COb_schsheetentry* psheetentry;
			psheetentry = (COb_schsheetentry*)pobject;
			if((psheetentry->m_nDiagram != m_nDiagram)||(psheetentry->m_nFlag == FLAG_DELETED)) continue;
			if(psheetentry->Is_On_Me(fx, fy) == TRUE)
			{
				selected_element.element = SCH_ELEMENT_SHEETENTRY;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(pport->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_PORT;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			if(ppower->Is_On_Me(fx, fy, this) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_POWER;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if(pbezier->Is_On_Me(fx, fy) == TRUE)
			{
				selected_element.element = SCH_ELEMENT_BEZIER;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			if(pgraphic->Is_On_Me(fx, fy) == TRUE) 
			{
				selected_element.element = SCH_ELEMENT_GRAPHIC;
				selected_element.pos = position;
				return selected_element;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram != m_nDiagram)||(pcomp->m_nFlag == FLAG_DELETED))	continue;
			selected_element = pcomp->Is_On_Me(fx, fy, pDoc, this, m_nDiagram, position);
			if(selected_element.element != 0)
			{
				selected_element.element = SCH_ELEMENT_COMPONENT;
				selected_element.pos = position;
				return selected_element;
			}
		}		
	}

	return selected_element;
}

void CESDView::ProcessingSelection(BOOL mode)
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
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram != m_nDiagram)||(pcomp->m_nFlag == FLAG_DELETED))	
			{
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);

				continue;
			}
			if(mode == TRUE)
			{
				if(pcomp->m_bSelection == TRUE)
				{
					pcomp->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(pcomp->m_nFlag == FLAG_PROCESSING) pcomp->m_nFlag = FLAG_NORMAL;
			}
			
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;

			if(mode == TRUE)
			{
				if(ptext->m_bSelection == TRUE)
				{
					ptext->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(ptext->m_nFlag == FLAG_PROCESSING) ptext->m_nFlag = FLAG_NORMAL;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;

			if(mode == TRUE)
			{
				if(pbezier->m_bSelection == TRUE)
				{
					pbezier->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(pbezier->m_nFlag == FLAG_PROCESSING) pbezier->m_nFlag = FLAG_NORMAL;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;

			if(mode == TRUE)
			{
				if(ppolyline->m_bSelection == TRUE)
				{
					ppolyline->m_nFlag = FLAG_PROCESSING;
				}	
			}
			else
			{
				if(ppolyline->m_nFlag == FLAG_PROCESSING) ppolyline->m_nFlag = FLAG_NORMAL;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;

			if(mode == TRUE)
			{
				if(ppolygon->m_bSelection == TRUE)
				{
					ppolygon->m_nFlag = FLAG_PROCESSING;
				}	
			}
			else
			{
				if(ppolygon->m_nFlag == FLAG_PROCESSING) ppolygon->m_nFlag = FLAG_NORMAL;
			}			

		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			
			if(mode == TRUE)
			{
				if(pellipsefill->m_bSelection == TRUE)
				{
					pellipsefill->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(pellipsefill->m_nFlag == FLAG_PROCESSING) pellipsefill->m_nFlag = FLAG_NORMAL;
			}	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			
			if(mode == TRUE)
			{
				if(ppie->m_bSelection == TRUE)
				{
					ppie->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(ppie->m_nFlag == FLAG_PROCESSING) ppie->m_nFlag = FLAG_NORMAL;
			}				
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			
			if(mode == TRUE)
			{
				if(proundrect->m_bSelection == TRUE)
				{
					proundrect->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(proundrect->m_nFlag == FLAG_PROCESSING) proundrect->m_nFlag = FLAG_NORMAL;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			
			if(mode == TRUE)
			{
				if(pellipse->m_bSelection == TRUE)
				{
					pellipse->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(pellipse->m_nFlag == FLAG_PROCESSING) pellipse->m_nFlag = FLAG_NORMAL;
			}			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			
			if(mode == TRUE)
			{
				if(parc->m_bSelection == TRUE)
				{
					parc->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(parc->m_nFlag == FLAG_PROCESSING) parc->m_nFlag = FLAG_NORMAL;
			}			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
			
			if(mode == TRUE)
			{
				if( pline->m_bSelection == TRUE)
				{
					pline->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(pline->m_nFlag == FLAG_PROCESSING) pline->m_nFlag = FLAG_NORMAL;
			}			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			
			if(mode == TRUE)
			{
				if(prect->m_bSelection == TRUE)
				{
					prect->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(prect->m_nFlag == FLAG_PROCESSING) prect->m_nFlag = FLAG_NORMAL;
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
			if(mode == TRUE)
			{
				if(psheetsymbol->m_bSelection == TRUE)
				{
					psheetsymbol->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(psheetsymbol->m_nFlag == FLAG_PROCESSING) psheetsymbol->m_nFlag = FLAG_NORMAL;
			}			

			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			
			if(mode == TRUE)
			{
				if(ppower->m_bSelection == TRUE)
				{
					ppower->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(ppower->m_nFlag == FLAG_PROCESSING) ppower->m_nFlag = FLAG_NORMAL;
			}

		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(mode == TRUE)
			{
				if(pport->m_bSelection == TRUE)
				{
					pport->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(pport->m_nFlag == FLAG_PROCESSING) pport->m_nFlag = FLAG_NORMAL;
			}			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
			
			if(mode == TRUE)
			{
				if(ptextframe->m_bSelection == TRUE)
				{
					ptextframe->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(ptextframe->m_nFlag == FLAG_PROCESSING) ptextframe->m_nFlag = FLAG_NORMAL;
			}				
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
			if(mode == TRUE)
			{
				if(pjunc->m_bSelection == TRUE)
				{
					pjunc->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(pjunc->m_nFlag == FLAG_PROCESSING) pjunc->m_nFlag = FLAG_NORMAL;
			}				
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			if(mode == TRUE)
			{
				if(pgraphic->m_bSelection == TRUE)
				{
					pgraphic->m_nFlag = FLAG_PROCESSING;
				}
			}
			else
			{
				if(pgraphic->m_nFlag == FLAG_PROCESSING) pgraphic->m_nFlag = FLAG_NORMAL;
			}				

		}
	}
	
	Invalidate();
}

int CESDView::FindSelectionKeyPoint(double fx, double fy)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	POSITION position;
	for(int i=0; i<pDoc->m_arraySelection[m_nDiagram].GetCount(); i++)
	{
		position = pDoc->m_arraySelection[m_nDiagram].GetAt(i);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(position);
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
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 1;
			}
			for(int k=1; k<pline->m_cVertex.GetCount(); k++)
			{
				double fx1, fy1, fx2, fy2;
				fx1 = (pline->m_cVertex[k-1].fx+pline->m_cVertex[k].fx)/2 - nEDA_Selection_Offset;
				fy1 = (pline->m_cVertex[k-1].fy+pline->m_cVertex[k].fy)/2 - nEDA_Selection_Offset;
				fx2 = (pline->m_cVertex[k-1].fx+pline->m_cVertex[k].fx)/2 + nEDA_Selection_Offset;
				fy2 = (pline->m_cVertex[k-1].fy+pline->m_cVertex[k].fy)/2 + nEDA_Selection_Offset;
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 2;
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
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 1;
			}
			for(int k=1; k<ppolyline->m_cVertex.GetCount(); k++)
			{
				double fx1, fy1, fx2, fy2;
				fx1 = (ppolyline->m_cVertex[k-1].fx+ppolyline->m_cVertex[k].fx)/2 - nEDA_Selection_Offset;
				fy1 = (ppolyline->m_cVertex[k-1].fy+ppolyline->m_cVertex[k].fy)/2 - nEDA_Selection_Offset;
				fx2 = (ppolyline->m_cVertex[k-1].fx+ppolyline->m_cVertex[k].fx)/2 + nEDA_Selection_Offset;
				fy2 = (ppolyline->m_cVertex[k-1].fy+ppolyline->m_cVertex[k].fy)/2 + nEDA_Selection_Offset;
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 2;
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
			
			
			double fx0, fy0, fx1, fy1, fx2, fy2;
			
			fx0 = parc->m_fCx + parc->m_fRadius*cos(parc->m_fEangle*fEDA_pi/180);
			fy0 = parc->m_fCy + parc->m_fRadius*sin(parc->m_fEangle*fEDA_pi/180);

			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 3;
			
			fx0 = parc->m_fCx + parc->m_fRadius*cos(parc->m_fSangle*fEDA_pi/180);
			fy0 = parc->m_fCy + parc->m_fRadius*sin(parc->m_fSangle*fEDA_pi/180);

			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 4;

			
			double alpha;
			if(parc->m_fEangle > parc->m_fSangle) alpha = (parc->m_fSangle + parc->m_fEangle)/2;
			else alpha = (parc->m_fSangle + parc->m_fEangle)/2 + 180;
			fx0 = parc->m_fCx + parc->m_fRadius*cos(alpha*fEDA_pi/180);
			fy0 = parc->m_fCy + parc->m_fRadius*sin(alpha*fEDA_pi/180);

			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 5;	
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;

			
			double fx0, fy0, fx1, fy1, fx2, fy2;

			
			double alpha = pellipse->m_fEangle*fEDA_pi/180;
			double ftemp = sqrt(pellipse->m_fRadius_x*pellipse->m_fRadius_x*sin(alpha)*sin(alpha) + pellipse->m_fRadius_y*pellipse->m_fRadius_y*cos(alpha)*cos(alpha));

			fx0 = pellipse->m_fCx + pellipse->m_fRadius_x*pellipse->m_fRadius_y*cos(alpha)/ftemp;
			fy0 = pellipse->m_fCy + pellipse->m_fRadius_x*pellipse->m_fRadius_y*sin(alpha)/ftemp;
			
			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 3;	

			
			alpha = pellipse->m_fSangle*fEDA_pi/180;
			ftemp = sqrt(pellipse->m_fRadius_x*pellipse->m_fRadius_x*sin(alpha)*sin(alpha) + pellipse->m_fRadius_y*pellipse->m_fRadius_y*cos(alpha)*cos(alpha));

			fx0 = pellipse->m_fCx + pellipse->m_fRadius_x*pellipse->m_fRadius_y*cos(alpha)/ftemp;
			fy0 = pellipse->m_fCy + pellipse->m_fRadius_x*pellipse->m_fRadius_y*sin(alpha)/ftemp;
			
			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 4;	

			
			fx0 = pellipse->m_fCx + pellipse->m_fRadius_x;
			fy0 = pellipse->m_fCy;
				
			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 6;			
			
			fx0 = pellipse->m_fCx;
			fy0 = pellipse->m_fCy + pellipse->m_fRadius_y;
				
			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 7;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			
			double fx0, fy0, fx1, fy1, fx2, fy2;

			
			fx0 = pellipsefill->m_fCx + pellipsefill->m_fRadius_x;
			fy0 = pellipsefill->m_fCy;
				
			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 6;			
			
			fx0 = pellipsefill->m_fCx;
			fy0 = pellipsefill->m_fCy + pellipsefill->m_fRadius_y;
				
			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 7;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			
			double fx0, fy0, fx1, fy1, fx2, fy2;
			
			fx0 = ppie->m_fCx + ppie->m_fRadius*cos(ppie->m_fEangle*fEDA_pi/180);
			fy0 = ppie->m_fCy + ppie->m_fRadius*sin(ppie->m_fEangle*fEDA_pi/180);

			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 3;
			
			fx0 = ppie->m_fCx + ppie->m_fRadius*cos(ppie->m_fSangle*fEDA_pi/180);
			fy0 = ppie->m_fCy + ppie->m_fRadius*sin(ppie->m_fSangle*fEDA_pi/180);

			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 4;

			
			double alpha;
			if(ppie->m_fEangle > ppie->m_fSangle) alpha = (ppie->m_fSangle + ppie->m_fEangle)/2;
			else alpha = (ppie->m_fSangle + ppie->m_fEangle)/2 + 180;
			fx0 = ppie->m_fCx + ppie->m_fRadius*cos(alpha*fEDA_pi/180);
			fy0 = ppie->m_fCy + ppie->m_fRadius*sin(alpha*fEDA_pi/180);

			fx1 = fx0 - nEDA_Selection_Offset;
			fy1 = fy0 - nEDA_Selection_Offset;
			fx2 = fx0 + nEDA_Selection_Offset;
			fy2 = fy0 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 5;	
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
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 10;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != m_nDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
			
			double fx1, fy1, fx2, fy2;
			//pView->DrawSelectionKeyPoint(x1, y1, pDC);
			fx1 = ptextframe->m_fX1 - nEDA_Selection_Offset;
			fy1 = ptextframe->m_fY1 - nEDA_Selection_Offset;
			fx2 = ptextframe->m_fX1 + nEDA_Selection_Offset;
			fy2 = ptextframe->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x1, y2, pDC);
			fx1 = ptextframe->m_fX1 - nEDA_Selection_Offset;
			fy1 = ptextframe->m_fY2 - nEDA_Selection_Offset;
			fx2 = ptextframe->m_fX1 + nEDA_Selection_Offset;
			fy2 = ptextframe->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x2, y1, pDC);
			fx1 = ptextframe->m_fX2 - nEDA_Selection_Offset;
			fy1 = ptextframe->m_fY1 - nEDA_Selection_Offset;
			fx2 = ptextframe->m_fX2 + nEDA_Selection_Offset;
			fy2 = ptextframe->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
		
			//pView->DrawSelectionKeyPoint(x2, y2, pDC);
			fx1 = ptextframe->m_fX2 - nEDA_Selection_Offset;
			fy1 = ptextframe->m_fY2 - nEDA_Selection_Offset;
			fx2 = ptextframe->m_fX2 + nEDA_Selection_Offset;
			fy2 = ptextframe->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint((x1+x2)/2, y1, pDC);
			fx1 = (ptextframe->m_fX1+ptextframe->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = ptextframe->m_fY1 - nEDA_Selection_Offset;
			fx2 = (ptextframe->m_fX1+ptextframe->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = ptextframe->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint((x1+x2)/2, y2, pDC);
			fx1 = (ptextframe->m_fX1+ptextframe->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = ptextframe->m_fY2 - nEDA_Selection_Offset;
			fx2 = (ptextframe->m_fX1+ptextframe->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = ptextframe->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x1, (y1+y2)/2, pDC);
			fx1 = ptextframe->m_fX1 - nEDA_Selection_Offset;
			fy1 = (ptextframe->m_fY1+ptextframe->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = ptextframe->m_fX1 + nEDA_Selection_Offset;
			fy2 = (ptextframe->m_fY1+ptextframe->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
						
			//pView->DrawSelectionKeyPoint(x2, (y1+y2)/2, pDC);
			fx1 = ptextframe->m_fX2 - nEDA_Selection_Offset;
			fy1 = (ptextframe->m_fY1+ptextframe->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = ptextframe->m_fX2 + nEDA_Selection_Offset;
			fy2 = (ptextframe->m_fY1+ptextframe->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			
			double fx1, fy1, fx2, fy2;
			//pView->DrawSelectionKeyPoint(x1, y1, pDC);
			fx1 = prect->m_fX1 - nEDA_Selection_Offset;
			fy1 = prect->m_fY1 - nEDA_Selection_Offset;
			fx2 = prect->m_fX1 + nEDA_Selection_Offset;
			fy2 = prect->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x1, y2, pDC);
			fx1 = prect->m_fX1 - nEDA_Selection_Offset;
			fy1 = prect->m_fY2 - nEDA_Selection_Offset;
			fx2 = prect->m_fX1 + nEDA_Selection_Offset;
			fy2 = prect->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x2, y1, pDC);
			fx1 = prect->m_fX2 - nEDA_Selection_Offset;
			fy1 = prect->m_fY1 - nEDA_Selection_Offset;
			fx2 = prect->m_fX2 + nEDA_Selection_Offset;
			fy2 = prect->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
		
			//pView->DrawSelectionKeyPoint(x2, y2, pDC);
			fx1 = prect->m_fX2 - nEDA_Selection_Offset;
			fy1 = prect->m_fY2 - nEDA_Selection_Offset;
			fx2 = prect->m_fX2 + nEDA_Selection_Offset;
			fy2 = prect->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint((x1+x2)/2, y1, pDC);
			fx1 = (prect->m_fX1+prect->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = prect->m_fY1 - nEDA_Selection_Offset;
			fx2 = (prect->m_fX1+prect->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = prect->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint((x1+x2)/2, y2, pDC);
			fx1 = (prect->m_fX1+prect->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = prect->m_fY2 - nEDA_Selection_Offset;
			fx2 = (prect->m_fX1+prect->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = prect->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x1, (y1+y2)/2, pDC);
			fx1 = prect->m_fX1 - nEDA_Selection_Offset;
			fy1 = (prect->m_fY1+prect->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = prect->m_fX1 + nEDA_Selection_Offset;
			fy2 = (prect->m_fY1+prect->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
						
			//pView->DrawSelectionKeyPoint(x2, (y1+y2)/2, pDC);
			fx1 = prect->m_fX2 - nEDA_Selection_Offset;
			fy1 = (prect->m_fY1+prect->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = prect->m_fX2 + nEDA_Selection_Offset;
			fy2 = (prect->m_fY1+prect->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			
			double fx1, fy1, fx2, fy2;
			
			//pView->DrawSelectionKeyPoint(x1, y1, pDC);
			fx1 = proundrect->m_fX1 - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY1 - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX1 + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x1, y2, pDC);
			fx1 = proundrect->m_fX1 - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY2 - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX1 + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x2, y1, pDC);
			fx1 = proundrect->m_fX2 - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY1 - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX2 + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
		
			//pView->DrawSelectionKeyPoint(x2, y2, pDC);
			fx1 = proundrect->m_fX2 - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY2 - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX2 + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;

			
			//pView->DrawSelectionKeyPoint((x1+x2)/2, y1, pDC);
			fx1 = (proundrect->m_fX1+proundrect->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY1 - nEDA_Selection_Offset;
			fx2 = (proundrect->m_fX1+proundrect->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint((x1+x2)/2, y2, pDC);
			fx1 = (proundrect->m_fX1+proundrect->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY2 - nEDA_Selection_Offset;
			fx2 = (proundrect->m_fX1+proundrect->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x1, (y1+y2)/2, pDC);
			fx1 = proundrect->m_fX1 - nEDA_Selection_Offset;
			fy1 = (proundrect->m_fY1+proundrect->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX1 + nEDA_Selection_Offset;
			fy2 = (proundrect->m_fY1+proundrect->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
						
			//pView->DrawSelectionKeyPoint(x2, (y1+y2)/2, pDC);
			fx1 = proundrect->m_fX2 - nEDA_Selection_Offset;
			fy1 = (proundrect->m_fY1+proundrect->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX2 + nEDA_Selection_Offset;
			fy2 = (proundrect->m_fY1+proundrect->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;

			
			//pView->DrawSelectionKeyPoint(x1+rx, y1-ry, pDC);
			fx1 = proundrect->m_fX1 + proundrect->m_fRadius_x - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY1 + proundrect->m_fRadius_y - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX1 + proundrect->m_fRadius_x + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY1 + proundrect->m_fRadius_y + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 8;
			
			//pView->DrawSelectionKeyPoint(x1+rx, y2+ry, pDC);
			fx1 = proundrect->m_fX1 + proundrect->m_fRadius_x - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY2 - proundrect->m_fRadius_y - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX1 + proundrect->m_fRadius_x + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY2 - proundrect->m_fRadius_y + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 8;
			
			//pView->DrawSelectionKeyPoint(x2-rx, y1-ry, pDC);
			fx1 = proundrect->m_fX2 - proundrect->m_fRadius_x - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY1 + proundrect->m_fRadius_y - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX2 - proundrect->m_fRadius_x + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY1 + proundrect->m_fRadius_y + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 8;
		
			//pView->DrawSelectionKeyPoint(x2-rx, y2+ry, pDC);
			fx1 = proundrect->m_fX2 - proundrect->m_fRadius_x - nEDA_Selection_Offset;
			fy1 = proundrect->m_fY2 - proundrect->m_fRadius_y - nEDA_Selection_Offset;
			fx2 = proundrect->m_fX2 - proundrect->m_fRadius_x + nEDA_Selection_Offset;
			fy2 = proundrect->m_fY2 - proundrect->m_fRadius_y + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 8;
		
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;
			if((psheetsymbol->m_nDiagram != m_nDiagram)||(psheetsymbol->m_nFlag == FLAG_DELETED)) continue;
			
			
			double fx1, fy1, fx2, fy2;
			//pView->DrawSelectionKeyPoint(x1, y1, pDC);
			fx1 = psheetsymbol->m_fX - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x1, y2, pDC);
			fx1 = psheetsymbol->m_fX - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY - psheetsymbol->m_fYsize + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x2, y1, pDC);
			fx1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + psheetsymbol->m_fXsize + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
		
			//pView->DrawSelectionKeyPoint(x2, y2, pDC);
			fx1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + psheetsymbol->m_fXsize + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY - psheetsymbol->m_fYsize + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint((x1+x2)/2, y1, pDC);
			fx1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize/2 - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + psheetsymbol->m_fXsize/2 + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint((x1+x2)/2, y2, pDC);
			fx1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize/2 - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + psheetsymbol->m_fXsize/2 + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY - psheetsymbol->m_fYsize + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x1, (y1+y2)/2, pDC);
			fx1 = psheetsymbol->m_fX - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize/2 - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY - psheetsymbol->m_fYsize/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
						
			//pView->DrawSelectionKeyPoint(x2, (y1+y2)/2, pDC);
			fx1 = psheetsymbol->m_fX + psheetsymbol->m_fXsize - nEDA_Selection_Offset;
			fy1 = psheetsymbol->m_fY - psheetsymbol->m_fYsize/2 - nEDA_Selection_Offset;
			fx2 = psheetsymbol->m_fX + psheetsymbol->m_fXsize + nEDA_Selection_Offset;
			fy2 = psheetsymbol->m_fY - psheetsymbol->m_fYsize/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
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
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
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
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 10;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != m_nDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			
			
			double fx1, fy1, fx2, fy2;
			//pView->DrawSelectionKeyPoint(x1, y1, pDC);
			fx1 = pgraphic->m_fX1 - nEDA_Selection_Offset;
			fy1 = pgraphic->m_fY1 - nEDA_Selection_Offset;
			fx2 = pgraphic->m_fX1 + nEDA_Selection_Offset;
			fy2 = pgraphic->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x1, y2, pDC);
			fx1 = pgraphic->m_fX1 - nEDA_Selection_Offset;
			fy1 = pgraphic->m_fY2 - nEDA_Selection_Offset;
			fx2 = pgraphic->m_fX1 + nEDA_Selection_Offset;
			fy2 = pgraphic->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x2, y1, pDC);
			fx1 = pgraphic->m_fX2 - nEDA_Selection_Offset;
			fy1 = pgraphic->m_fY1 - nEDA_Selection_Offset;
			fx2 = pgraphic->m_fX2 + nEDA_Selection_Offset;
			fy2 = pgraphic->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
		
			//pView->DrawSelectionKeyPoint(x2, y2, pDC);
			fx1 = pgraphic->m_fX2 - nEDA_Selection_Offset;
			fy1 = pgraphic->m_fY2 - nEDA_Selection_Offset;
			fx2 = pgraphic->m_fX2 + nEDA_Selection_Offset;
			fy2 = pgraphic->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint((x1+x2)/2, y1, pDC);
			fx1 = (pgraphic->m_fX1+pgraphic->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = pgraphic->m_fY1 - nEDA_Selection_Offset;
			fx2 = (pgraphic->m_fX1+pgraphic->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = pgraphic->m_fY1 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint((x1+x2)/2, y2, pDC);
			fx1 = (pgraphic->m_fX1+pgraphic->m_fX2)/2 - nEDA_Selection_Offset;
			fy1 = pgraphic->m_fY2 - nEDA_Selection_Offset;
			fx2 = (pgraphic->m_fX1+pgraphic->m_fX2)/2 + nEDA_Selection_Offset;
			fy2 = pgraphic->m_fY2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			
			//pView->DrawSelectionKeyPoint(x1, (y1+y2)/2, pDC);
			fx1 = pgraphic->m_fX1 - nEDA_Selection_Offset;
			fy1 = (pgraphic->m_fY1+pgraphic->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = pgraphic->m_fX1 + nEDA_Selection_Offset;
			fy2 = (pgraphic->m_fY1+pgraphic->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
						
			//pView->DrawSelectionKeyPoint(x2, (y1+y2)/2, pDC);
			fx1 = pgraphic->m_fX2 - nEDA_Selection_Offset;
			fy1 = (pgraphic->m_fY1+pgraphic->m_fY2)/2 - nEDA_Selection_Offset;
			fx2 = pgraphic->m_fX2 + nEDA_Selection_Offset;
			fy2 = (pgraphic->m_fY1+pgraphic->m_fY2)/2 + nEDA_Selection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram != m_nDiagram)||(pcomp->m_nFlag == FLAG_DELETED))	continue;
			
		}		
	}

	return -1;
}

