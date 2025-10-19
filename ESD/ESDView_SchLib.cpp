void CESDView::OnLibAdd()
{
	unsigned ch1;
	char szHelp[] = "选择元件添加: <左键><回车>选择对象, <右键><ESC>结束命令";
	
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	Struct_Multi_Selection multi_selection;
	multi_selection.element = -1;
	multi_selection.pos = NULL;
	Write_Help(szHelp);
	do
	{
       ch1 = MouseKey();
       if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
			multi_selection = SelectSCHObject(m_fCurrentX, m_fCurrentY, MULTI_SELECTION);
			if(multi_selection.element == SCH_ELEMENT_COMPONENT) ch1 = VK_ESCAPE;

	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);
	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;	

	if( (multi_selection.element <= 0) || (multi_selection.pos == NULL) )  return;

	CObject* pobject =  pDoc->m_listSchObject[m_nDiagram].GetAt(multi_selection.pos);
	if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
	{
		COb_schcomp* pcomp = (COb_schcomp*)pobject;

		CDlg_SchLib_Add dlg;
		dlg.pcomp = pcomp;
		dlg.m_csEditCompName = pcomp->GetComment();
		dlg.m_csEditDescription = pcomp->m_csDescription;
		dlg.m_csEditRef = pcomp->GetDesignator();
		dlg.m_csEditRef = dlg.m_csEditRef.Left(1);
		dlg.m_csEditRef += "?";
		dlg.m_csEditPack = pcomp->GetPackage();
		dlg.m_csStaticNewPart.Format("%d/%d", pcomp->m_nPart, pcomp->m_nPartCount);
		if(dlg.DoModal() == IDOK)
		{
			if(dlg.m_nRadioLib == 0)
			{
				
				if(dlg.m_csEditCompName.IsEmpty() == TRUE)
				{
					AfxMessageBox("无法添加, 未定义元件名称.");
					return;
				}
				
				TRACE2("新库%s添加新元件%s\n",dlg.m_csLibName, dlg.m_csEditCompName);
				CString csLibraryName = dlg.m_csLibName;
				if(csLibraryName.Find(".lib") < 0) csLibraryName += ".lib";

				
				CArray <LOGFONT, LOGFONT&>  arrayFont;
				LOGFONT logfont;
				logfont.lfHeight = 10;
				logfont.lfWidth = 0;
				logfont.lfEscapement = logfont.lfOrientation = 0;
				logfont.lfWeight = FW_NORMAL;
				logfont.lfItalic = 0;
				logfont.lfUnderline = 0;
				logfont.lfStrikeOut = 0;
				logfont.lfCharSet = DEFAULT_CHARSET;
				logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
				logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
				logfont.lfQuality = PROOF_QUALITY;
				logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
				strcpy(logfont.lfFaceName, "Times New Roman");
				arrayFont.Add(logfont);
				
				
				for(int k=0; k<pcomp->m_arrayPos.GetCount(); k++)
				{
					CObject* pobject;
					POSITION pos = pcomp->m_arrayPos.GetAt(k);
					pobject = pDoc->m_listSchObject[pcomp->m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						if((ptext->m_nID == TEXT_TEXT)||(ptext->m_nID == TEXT_COMPTEXT))
						{
							if(ptext->m_nFont == 0) continue;
							LOGFONT new_logfont = pDoc->m_arrayFont[pcomp->m_nDiagram].GetAt(ptext->m_nFont);
							int index;
							for(index=0; index<arrayFont.GetCount(); index++)
							{
								logfont = arrayFont.GetAt(index);
								if( (logfont.lfHeight == new_logfont.lfHeight) && (logfont.lfOrientation == new_logfont.lfOrientation) && \
									(logfont.lfWeight == new_logfont.lfWeight) && (logfont.lfItalic == new_logfont.lfItalic) && \
									(logfont.lfUnderline == new_logfont.lfUnderline) && (logfont.lfStrikeOut == new_logfont.lfStrikeOut) && \
									(stricmp(logfont.lfFaceName, new_logfont.lfFaceName) == 0) )
								{
									break;
								}

							}

							if(index >= arrayFont.GetCount()) arrayFont.Add(new_logfont);
						}
					}
				}
				
				
				char libstring[MAX_PATH];
				sprintf(libstring,"%s\\schlib\\font.tmp",csEDA_System_Path);
				CFile cfileTemp;
				if(cfileTemp.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE){AfxMessageBox("无法打开文件."); return;}
				CArchive font_ar(&cfileTemp, CArchive::store);

				int fontnum = arrayFont.GetCount();
				font_ar << fontnum;
				for(int k=0; k<fontnum; k++)
				{
					logfont = arrayFont.GetAt(k);
					Struct_Font sFont;
					sFont.nHeight = logfont.lfHeight;
					sFont.nOrientation = logfont.lfOrientation;
					sFont.bUnderline = logfont.lfUnderline;
					if(logfont.lfWeight == FW_NORMAL) sFont.bBold = FALSE;
					else sFont.bBold = TRUE;
					sFont.bItalic = logfont.lfItalic;
					sFont.bStrikeout = logfont.lfStrikeOut;
					sFont.csFontName = logfont.lfFaceName;
					font_ar << sFont.nHeight;
					font_ar << sFont.nOrientation;
					font_ar << sFont.bUnderline;
					font_ar << sFont.bItalic;
					font_ar << sFont.bBold;
					font_ar << sFont.bStrikeout;
					font_ar << sFont.csFontName;		
				}
				font_ar.Flush();
				font_ar.Close();
				cfileTemp.Close();
	
				
				sprintf(libstring,"%s\\schlib\\element.tmp",csEDA_System_Path);
				if(cfileTemp.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE){AfxMessageBox("无法打开文件."); return;}
				CArchive temp_ar(&cfileTemp, CArchive::store);
				COb_libcomphead comphead;
				comphead.m_arrayCompName.Add(dlg.m_csEditCompName);
				comphead.m_arrayPackage.Add(dlg.m_csEditPack);
				comphead.m_csDescription = dlg.m_csEditDescription;
				comphead.m_csRef = dlg.m_csEditRef;
				comphead.m_nPartCount = pcomp->m_nPartCount;
				comphead.m_nSharedCompNum = 1;
				comphead.Serialize(temp_ar);

				
				int rotation = 0;
				//if(pcomp->m_nOrientation == 0) rotation = pcomp->m_nOrientation;
				//else  rotation = 360 - pcomp->m_nOrientation;
				double fx0, fy0, fx1, fy1, deltax, deltay;
				fx0 = pcomp->m_fX;
				fy0 = pcomp->m_fY;
				fx1 = fy1 = 0;
				deltax = fx1 - fx0;
				deltay = fy1 - fy0;
				BOOL mirror = FALSE; //pcomp->m_bMirrored;
				int element_id;
				for(int k=0; k<pcomp->m_arrayPos.GetCount(); k++)
				{
					CObject* pobject;
					POSITION pos = pcomp->m_arrayPos.GetAt(k);
					pobject = pDoc->m_listSchObject[pcomp->m_nDiagram].GetAt(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
					{
						COb_schpin* ppin = (COb_schpin*)pobject;
						COb_schpin* p_pin = new COb_schpin();
						p_pin->Copy(ppin);

						pDoc->Get_Rotate_XY(&fx1, &fy1, p_pin->m_fX - fx0, p_pin->m_fY - fy0, rotation, mirror);
						p_pin->m_fX = fx1 + fx0 + deltax;
						p_pin->m_fY = fy1 + fy0 + deltay;

						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, p_pin->m_nOrientation, rotation, mirror);
						p_pin->m_nOrientation = orientation;
						p_pin->m_nOrientation = pDoc->Get_Correct_Orientation(p_pin->m_nOrientation);	
						
						element_id = SCH_ELEMENT_PIN;
						temp_ar << element_id;
						p_pin->Serialize(temp_ar);
						delete p_pin;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
					{
						COb_schieee* pieee = (COb_schieee*)pobject;
						COb_schieee* p_ieee = new COb_schieee();
						p_ieee->Copy(pieee);

						pDoc->Get_Rotate_XY(&fx1, &fy1, p_ieee->m_fX - fx0, p_ieee->m_fY - fy0, rotation, mirror);
						p_ieee->m_fX = fx1 + fx0 + deltax;
						p_ieee->m_fY = fy1 + fy0 + deltay;
						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, p_ieee->m_nOrientation, rotation, mirror);
						p_ieee->m_nOrientation = orientation;
						p_ieee->m_nOrientation = pDoc->Get_Correct_Orientation(p_ieee->m_nOrientation);

						element_id = SCH_ELEMENT_IEEE;
						temp_ar << element_id;
						p_ieee->Serialize(temp_ar);
						delete p_ieee;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
					{
						COb_scharc* parc = (COb_scharc*)pobject;
						COb_scharc* p_arc = new COb_scharc();
						p_arc->Copy(parc);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, p_arc->m_fCx - fx0, p_arc->m_fCy - fy0, rotation, mirror);
						p_arc->m_fCx = fx1 + fx0 + deltax;
						p_arc->m_fCy = fy1 + fy0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, p_arc->m_fSangle, p_arc->m_fEangle, rotation, mirror);
						p_arc->m_fSangle = salpha;
						p_arc->m_fSangle = pDoc->Get_Correct_Angle(p_arc->m_fSangle);
						p_arc->m_fEangle = ealpha;
						p_arc->m_fEangle = pDoc->Get_Correct_Angle(p_arc->m_fEangle);

						element_id = SCH_ELEMENT_ARC;
						temp_ar << element_id;
						p_arc->Serialize(temp_ar);
						delete p_arc;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
					{
						COb_schellipse* pellipse = (COb_schellipse*)pobject;
						COb_schellipse* p_ellipse = new COb_schellipse();
						p_ellipse->Copy(pellipse);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, p_ellipse->m_fCx - fx0, p_ellipse->m_fCy - fy0, rotation, mirror);
						p_ellipse->m_fCx = fx1 + fx0 + deltax;
						p_ellipse->m_fCy = fy1 + fy0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, p_ellipse->m_fSangle, p_ellipse->m_fEangle, rotation, mirror);
						p_ellipse->m_fSangle = salpha;
						p_ellipse->m_fSangle = pDoc->Get_Correct_Angle(p_ellipse->m_fSangle);
						p_ellipse->m_fEangle = ealpha;
						p_ellipse->m_fEangle = pDoc->Get_Correct_Angle(p_ellipse->m_fEangle);
						if((rotation == 90)||(rotation == 270))
						{
							double ftemp;
							ftemp = p_ellipse->m_fRadius_y;
							p_ellipse->m_fRadius_y = p_ellipse->m_fRadius_x;
							p_ellipse->m_fRadius_x = ftemp;
						}
						
						element_id = SCH_ELEMENT_ELLIPSE;
						temp_ar << element_id;
						p_ellipse->Serialize(temp_ar);
						delete p_ellipse;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
					{
						COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
						COb_schellipsefill* p_ellipsefill = new COb_schellipsefill();
						p_ellipsefill->Copy(pellipsefill);

						pDoc->Get_Rotate_XY(&fx1, &fy1, p_ellipsefill->m_fCx - fx0, p_ellipsefill->m_fCy - fy0, rotation, mirror);
						p_ellipsefill->m_fCx = fx1 + fx0 + deltax;
						p_ellipsefill->m_fCy = fy1 + fy0 + deltay;
						if((rotation == 90)||(rotation == 270))
						{
							double ftemp;
							ftemp = p_ellipsefill->m_fRadius_y;
							p_ellipsefill->m_fRadius_y = p_ellipsefill->m_fRadius_x;
							p_ellipsefill->m_fRadius_x = ftemp;
						}

						element_id = SCH_ELEMENT_ELLIPSEFILL;
						temp_ar << element_id;
						p_ellipsefill->Serialize(temp_ar);
						delete p_ellipsefill;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
					{
						COb_schpie* ppie = (COb_schpie*)pobject;
						COb_schpie* p_pie = new COb_schpie();
						p_pie->Copy(ppie);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, p_pie->m_fCx - fx0, p_pie->m_fCy - fy0, rotation, mirror);
						p_pie->m_fCx = fx1 + fx0 + deltax;
						p_pie->m_fCy = fy1 + fy0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, p_pie->m_fSangle, p_pie->m_fEangle, rotation, mirror);
						p_pie->m_fSangle = salpha;
						p_pie->m_fSangle = pDoc->Get_Correct_Angle(p_pie->m_fSangle);
						p_pie->m_fEangle = ealpha;
						p_pie->m_fEangle = pDoc->Get_Correct_Angle(p_pie->m_fEangle);
						
						element_id = SCH_ELEMENT_PIE;
						temp_ar << element_id;
						p_pie->Serialize(temp_ar);
						delete p_pie;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
					{
						COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
						COb_schpolyline* p_polyline = new COb_schpolyline();
						p_polyline->Copy(ppolyline);

						for(int l=0; l<p_polyline->m_cVertex.GetCount(); l++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_polyline->m_cVertex[l].fx-fx0, p_polyline->m_cVertex[l].fy-fy0, rotation, mirror);
							p_polyline->m_cVertex[l].fx = fx1 + fx0 + deltax;
							p_polyline->m_cVertex[l].fy = fy1 + fy0 + deltay;
						}

						element_id = SCH_ELEMENT_POLYLINE;
						temp_ar << element_id;
						p_polyline->Serialize(temp_ar);
						delete p_polyline;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
					{
						COb_schrect* prect = (COb_schrect*)pobject;
						COb_schrect* p_rect = new COb_schrect();
						p_rect->Copy(prect);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, p_rect->m_fX1 - fx0, p_rect->m_fY1 - fy0, rotation, mirror);
						p_rect->m_fX1 = fx1 + fx0 + deltax;
						p_rect->m_fY1 = fy1 + fy0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, p_rect->m_fX2 - fx0, p_rect->m_fY2 - fy0, rotation, mirror);
						p_rect->m_fX2 = fx1 + fx0 + deltax;
						p_rect->m_fY2 = fy1 + fy0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(p_rect->m_fX1, p_rect->m_fX2);
						fmaxx = MAX(p_rect->m_fX1, p_rect->m_fX2);
						fminy = MIN(p_rect->m_fY1, p_rect->m_fY2);
						fmaxy = MAX(p_rect->m_fY1, p_rect->m_fY2);
						p_rect->m_fX1 = fminx;
						p_rect->m_fY1 = fminy;
						p_rect->m_fX2 = fmaxx;
						p_rect->m_fY2 = fmaxy;

						element_id = SCH_ELEMENT_RECT;
						temp_ar << element_id;
						p_rect->Serialize(temp_ar);
						delete p_rect;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
					{
						COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
						COb_schroundrect* p_roundrect = new COb_schroundrect();
						p_roundrect->Copy(proundrect);

						pDoc->Get_Rotate_XY(&fx1, &fy1, p_roundrect->m_fX1 - fx0, p_roundrect->m_fY1 - fy0, rotation, mirror);
						p_roundrect->m_fX1 = fx1 + fx0 + deltax;
						p_roundrect->m_fY1 = fy1 + fy0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, p_roundrect->m_fX2 - fx0, p_roundrect->m_fY2 - fy0, rotation, mirror);
						p_roundrect->m_fX2 = fx1 + fx0 + deltax;
						p_roundrect->m_fY2 = fy1 + fy0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(p_roundrect->m_fX1, p_roundrect->m_fX2);
						fmaxx = MAX(p_roundrect->m_fX1, p_roundrect->m_fX2);
						fminy = MIN(p_roundrect->m_fY1, p_roundrect->m_fY2);
						fmaxy = MAX(p_roundrect->m_fY1, p_roundrect->m_fY2);
						p_roundrect->m_fX1 = fminx;
						p_roundrect->m_fY1 = fminy;
						p_roundrect->m_fX2 = fmaxx;
						p_roundrect->m_fY2 = fmaxy;

						element_id = SCH_ELEMENT_ROUNDRECT;
						temp_ar << element_id;
						p_roundrect->Serialize(temp_ar);
						delete p_roundrect;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
					{
						COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
						COb_schpolygon* p_polygon = new COb_schpolygon();
						p_polygon->Copy(ppolygon);
						
						for(int k=0; k<p_polygon->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_polygon->m_cVertex[k].fx-fx0, p_polygon->m_cVertex[k].fy-fy0, rotation, mirror);
							p_polygon->m_cVertex[k].fx = fx1 + fx0 + deltax;
							p_polygon->m_cVertex[k].fy = fy1 + fy0 + deltay;
						}
						
						element_id = SCH_ELEMENT_POLYGON;
						temp_ar << element_id;
						p_polygon->Serialize(temp_ar);
						delete p_polygon;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
					{
						COb_schbezier* pbezier = (COb_schbezier*)pobject;
						COb_schbezier* p_bezier = new COb_schbezier();
						p_bezier->Copy(pbezier);

						for(int k=0; k<p_bezier->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_bezier->m_cVertex[k].fx-fx0, p_bezier->m_cVertex[k].fy-fy0, rotation, mirror);
							p_bezier->m_cVertex[k].fx = fx1 + fx0 + deltax;
							p_bezier->m_cVertex[k].fy = fy1 + fy0 + deltay;
						}

						element_id = SCH_ELEMENT_BEZIER;
						temp_ar << element_id;
						p_bezier->Serialize(temp_ar);
						delete p_bezier;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						if((ptext->m_nID != TEXT_TEXT)&&(ptext->m_nID != TEXT_COMPTEXT)) continue;
						COb_schtext* p_text = new COb_schtext();
						p_text->Copy(ptext);

						pDoc->Get_Rotate_XY(&fx1, &fy1, p_text->m_fSx - fx0, p_text->m_fSy - fy0, rotation, mirror);
						p_text->m_fSx = fx1 + fx0 + deltax;
						p_text->m_fSy = fy1 + fy0 + deltay;
						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, p_text->m_nOrientation, rotation, FALSE); //mirror);
						p_text->m_nOrientation = orientation;
						p_text->m_nOrientation = pDoc->Get_Correct_Orientation(p_text->m_nOrientation);
			
						if(mirror == TRUE)
						{
							p_text->m_bMirror = FALSE;
							if((p_text->m_nOrientation == 0)||(p_text->m_nOrientation == 180))
							{
								if(p_text->m_nH_Justification == 0) p_text->m_nH_Justification = 2;
								else if(p_text->m_nH_Justification == 2) p_text->m_nH_Justification = 0;
							}
							else
							{
								if(p_text->m_nV_Justification == 0) p_text->m_nV_Justification = 2;
								else if(p_text->m_nV_Justification == 2) p_text->m_nV_Justification = 0;
							}
						}
						
						LOGFONT new_logfont = pDoc->m_arrayFont[pcomp->m_nDiagram].GetAt(ptext->m_nFont);
						int index;
						for(index=0; index<arrayFont.GetCount(); index++)
						{
							logfont = arrayFont.GetAt(index);
							if( (logfont.lfHeight == new_logfont.lfHeight) && (logfont.lfOrientation == new_logfont.lfOrientation) && \
								(logfont.lfWeight == new_logfont.lfWeight) && (logfont.lfItalic == new_logfont.lfItalic) && \
								(logfont.lfUnderline == new_logfont.lfUnderline) && (logfont.lfStrikeOut == new_logfont.lfStrikeOut) && \
								(stricmp(logfont.lfFaceName, new_logfont.lfFaceName) == 0) )
							{
								break;
							}

						}
						if(index >= arrayFont.GetCount()) p_text->m_nFont = 0;
						else p_text->m_nFont = index;

						element_id = SCH_ELEMENT_TEXT;
						temp_ar << element_id;
						p_text->Serialize(temp_ar);
						delete p_text;
					}
				}
				
				
				element_id = 0;
				temp_ar << element_id;

				temp_ar.Flush();
				temp_ar.Close();
				cfileTemp.Close();

				
				sprintf(libstring,"%s\\schlib\\font.tmp",csEDA_System_Path);
				CFile cfileTemp1;
				if(cfileTemp1.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); return;}
				int fontfile_length = cfileTemp1.GetLength();

				sprintf(libstring,"%s\\schlib\\element.tmp",csEDA_System_Path);
				CFile cfileTemp2;
				if(cfileTemp2.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); return;}
				int elementfile_length = cfileTemp2.GetLength();

				
				sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, csLibraryName);
				CFile cfileUserLibrary;
				if(cfileUserLibrary.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE)
				{
					AfxMessageBox("无法打开文件."); 
					cfileTemp1.Close(); 
					cfileTemp2.Close(); 
					return;
				}
				CArchive ar(&cfileUserLibrary, CArchive::store);
				CString headformat = SCH_Library_Head;
				ar << headformat;
				int totalnum = 1;
				ar << totalnum;
				ar.Flush();
				ar.Close();

				Struct_Comp_Head comp_head;
				CString tstr = dlg.m_csEditCompName;
				tstr.Left(99);
				strcpy(comp_head.name, tstr);
				tstr = dlg.m_csEditDescription;
				tstr.Left(210);
				strcpy(comp_head.description, tstr);
				comp_head.length = elementfile_length;
				comp_head.offset = 32 + 320 + fontfile_length;

				cfileUserLibrary.Write(&comp_head, 320);

				char buffer[1000];
				for(;;)
				{
					if(fontfile_length > 1000) 
					{
						cfileTemp1.Read(buffer, 1000);
						cfileUserLibrary.Write(buffer, 1000);
						fontfile_length -= 1000;
					}
					else
					{
						cfileTemp1.Read(buffer, fontfile_length);
						cfileUserLibrary.Write(buffer, fontfile_length);
						break;
					}
				}
				for(;;)
				{
					if(elementfile_length > 1000) 
					{
						cfileTemp2.Read(buffer, 1000);
						cfileUserLibrary.Write(buffer, 1000);
						elementfile_length -= 1000;
					}
					else
					{
						cfileTemp2.Read(buffer, elementfile_length);
						cfileUserLibrary.Write(buffer, elementfile_length);
						break;
					}
				}

				cfileTemp1.Close();
				cfileTemp2.Close();
				cfileUserLibrary.Close();

				arrayEDA_SchUserLibraryName.Add(csLibraryName);
				arrayEDA_SchLibraryCompList.RemoveAll();

				
				sprintf(libstring,"%s\\schlib\\font.tmp",csEDA_System_Path);
				DeleteFile(libstring);
				sprintf(libstring,"%s\\schlib\\element.tmp",csEDA_System_Path);
				DeleteFile(libstring);
			}
			else
			{
				if(dlg.m_nRadioComp == 0)
				{
					
					TRACE2("旧库%s添加新元件%s\n",dlg.m_csLibName, dlg.m_csEditCompName);
					if(dlg.m_csEditCompName.IsEmpty() == TRUE)
					{
						AfxMessageBox("无法添加, 未定义元件名称.");
						return;
					}
					CString csLibraryName = dlg.m_csLibName;
					if(csLibraryName.Find(".lib") < 0) csLibraryName += ".lib";

					
					CArray <LOGFONT, LOGFONT&>  arrayFont;
					LOGFONT logfont;
					
					char libstring[MAX_PATH];
					sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, dlg.m_csLibName);
					CFile cfileTemp;
					if(cfileTemp.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); return;}

					Struct_Library_Head libhead;
					cfileTemp.Read(&libhead, 32);
					Struct_Comp_Head comp_head;
					cfileTemp.Read(&comp_head, 320);

					int oldfont_length = comp_head.offset - 32 - 320*libhead.totalcompnum;
					BYTE* pbuffer = (BYTE *)malloc(oldfont_length);

					cfileTemp.Seek(32 + 320*libhead.totalcompnum,  CFile::begin);
					cfileTemp.Read(pbuffer, oldfont_length);
					
					CMemFile memfile(pbuffer, oldfont_length);
					CArchive oldfont_ar(&memfile,CArchive::load);
					int fontnum;
					oldfont_ar >> fontnum;
					for(int k=0; k<fontnum; k++)
					{
						Struct_Font sFont;
						oldfont_ar >> sFont.nHeight;
						oldfont_ar >> sFont.nOrientation;
						oldfont_ar >> sFont.bUnderline;
						oldfont_ar >> sFont.bItalic;
						oldfont_ar >> sFont.bBold;
						oldfont_ar >> sFont.bStrikeout;
						oldfont_ar >> sFont.csFontName;
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
					oldfont_ar.Close();
					memfile.Close();
					free(pbuffer);
					cfileTemp.Close();

					
					for(int k=0; k<pcomp->m_arrayPos.GetCount(); k++)
					{
						CObject* pobject;
						POSITION pos = pcomp->m_arrayPos.GetAt(k);
						pobject = pDoc->m_listSchObject[pcomp->m_nDiagram].GetNext(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;
							if((ptext->m_nID == TEXT_TEXT)||(ptext->m_nID == TEXT_COMPTEXT))
							{
								if(ptext->m_nFont == 0) continue;
								LOGFONT new_logfont = pDoc->m_arrayFont[pcomp->m_nDiagram].GetAt(ptext->m_nFont);
								int index;
								for(index=0; index<arrayFont.GetCount(); index++)
								{
									logfont = arrayFont.GetAt(index);
									if( (logfont.lfHeight == new_logfont.lfHeight) && (logfont.lfOrientation == new_logfont.lfOrientation) && \
										(logfont.lfWeight == new_logfont.lfWeight) && (logfont.lfItalic == new_logfont.lfItalic) && \
										(logfont.lfUnderline == new_logfont.lfUnderline) && (logfont.lfStrikeOut == new_logfont.lfStrikeOut) && \
										(stricmp(logfont.lfFaceName, new_logfont.lfFaceName) == 0) )
									{
										break;
									}

								}

								if(index >= arrayFont.GetCount()) arrayFont.Add(new_logfont);
							}
						}
					}
					
					
					sprintf(libstring,"%s\\schlib\\font.tmp",csEDA_System_Path);
					if(cfileTemp.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE){AfxMessageBox("无法打开文件."); return;}
					CArchive font_ar(&cfileTemp, CArchive::store);

					fontnum = arrayFont.GetCount();
					font_ar << fontnum;
					for(int k=0; k<fontnum; k++)
					{
						logfont = arrayFont.GetAt(k);
						Struct_Font sFont;
						sFont.nHeight = logfont.lfHeight;
						sFont.nOrientation = logfont.lfOrientation;
						sFont.bUnderline = logfont.lfUnderline;
						if(logfont.lfWeight == FW_NORMAL) sFont.bBold = FALSE;
						else sFont.bBold = TRUE;
						sFont.bItalic = logfont.lfItalic;
						sFont.bStrikeout = logfont.lfStrikeOut;
						sFont.csFontName = logfont.lfFaceName;
						font_ar << sFont.nHeight;
						font_ar << sFont.nOrientation;
						font_ar << sFont.bUnderline;
						font_ar << sFont.bItalic;
						font_ar << sFont.bBold;
						font_ar << sFont.bStrikeout;
						font_ar << sFont.csFontName;		
					}
					font_ar.Flush();
					font_ar.Close();
					cfileTemp.Close();
		
					
					sprintf(libstring,"%s\\schlib\\element.tmp",csEDA_System_Path);
					if(cfileTemp.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE){AfxMessageBox("无法打开文件."); return;}
					CArchive temp_ar(&cfileTemp, CArchive::store);
					COb_libcomphead comphead;
					comphead.m_arrayCompName.Add(dlg.m_csEditCompName);
					comphead.m_arrayPackage.Add(dlg.m_csEditPack);
					comphead.m_csDescription = dlg.m_csEditDescription;
					comphead.m_csRef = dlg.m_csEditRef;
					comphead.m_nPartCount = pcomp->m_nPartCount;
					comphead.m_nSharedCompNum = 1;
					comphead.Serialize(temp_ar);

					
					int rotation = 0;
					//if(pcomp->m_nOrientation == 0) rotation = pcomp->m_nOrientation;
					//else  rotation = 360 - pcomp->m_nOrientation;
					double fx0, fy0, fx1, fy1, deltax, deltay;
					fx0 = pcomp->m_fX;
					fy0 = pcomp->m_fY;
					fx1 = fy1 = 0;
					deltax = fx1 - fx0;
					deltay = fy1 - fy0;
					BOOL mirror = FALSE; //pcomp->m_bMirrored;
					int element_id;
					for(int k=0; k<pcomp->m_arrayPos.GetCount(); k++)
					{
						CObject* pobject;
						POSITION pos = pcomp->m_arrayPos.GetAt(k);
						pobject = pDoc->m_listSchObject[pcomp->m_nDiagram].GetAt(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
						{
							COb_schpin* ppin = (COb_schpin*)pobject;
							COb_schpin* p_pin = new COb_schpin();
							p_pin->Copy(ppin);

							pDoc->Get_Rotate_XY(&fx1, &fy1, p_pin->m_fX - fx0, p_pin->m_fY - fy0, rotation, mirror);
							p_pin->m_fX = fx1 + fx0 + deltax;
							p_pin->m_fY = fy1 + fy0 + deltay;

							int orientation;
							pDoc->Get_Rotate_Orientation(&orientation, p_pin->m_nOrientation, rotation, mirror);
							p_pin->m_nOrientation = orientation;
							p_pin->m_nOrientation = pDoc->Get_Correct_Orientation(p_pin->m_nOrientation);	
							
							element_id = SCH_ELEMENT_PIN;
							temp_ar << element_id;
							p_pin->Serialize(temp_ar);
							delete p_pin;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
						{
							COb_schieee* pieee = (COb_schieee*)pobject;
							COb_schieee* p_ieee = new COb_schieee();
							p_ieee->Copy(pieee);

							pDoc->Get_Rotate_XY(&fx1, &fy1, p_ieee->m_fX - fx0, p_ieee->m_fY - fy0, rotation, mirror);
							p_ieee->m_fX = fx1 + fx0 + deltax;
							p_ieee->m_fY = fy1 + fy0 + deltay;
							int orientation;
							pDoc->Get_Rotate_Orientation(&orientation, p_ieee->m_nOrientation, rotation, mirror);
							p_ieee->m_nOrientation = orientation;
							p_ieee->m_nOrientation = pDoc->Get_Correct_Orientation(p_ieee->m_nOrientation);

							element_id = SCH_ELEMENT_IEEE;
							temp_ar << element_id;
							p_ieee->Serialize(temp_ar);
							delete p_ieee;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
						{
							COb_scharc* parc = (COb_scharc*)pobject;
							COb_scharc* p_arc = new COb_scharc();
							p_arc->Copy(parc);
							
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_arc->m_fCx - fx0, p_arc->m_fCy - fy0, rotation, mirror);
							p_arc->m_fCx = fx1 + fx0 + deltax;
							p_arc->m_fCy = fy1 + fy0 + deltay;
							
							double salpha, ealpha;
							pDoc->Get_Rotate_Angle(&salpha, &ealpha, p_arc->m_fSangle, p_arc->m_fEangle, rotation, mirror);
							p_arc->m_fSangle = salpha;
							p_arc->m_fSangle = pDoc->Get_Correct_Angle(p_arc->m_fSangle);
							p_arc->m_fEangle = ealpha;
							p_arc->m_fEangle = pDoc->Get_Correct_Angle(p_arc->m_fEangle);

							element_id = SCH_ELEMENT_ARC;
							temp_ar << element_id;
							p_arc->Serialize(temp_ar);
							delete p_arc;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
						{
							COb_schellipse* pellipse = (COb_schellipse*)pobject;
							COb_schellipse* p_ellipse = new COb_schellipse();
							p_ellipse->Copy(pellipse);
							
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_ellipse->m_fCx - fx0, p_ellipse->m_fCy - fy0, rotation, mirror);
							p_ellipse->m_fCx = fx1 + fx0 + deltax;
							p_ellipse->m_fCy = fy1 + fy0 + deltay;
							
							double salpha, ealpha;
							pDoc->Get_Rotate_Angle(&salpha, &ealpha, p_ellipse->m_fSangle, p_ellipse->m_fEangle, rotation, mirror);
							p_ellipse->m_fSangle = salpha;
							p_ellipse->m_fSangle = pDoc->Get_Correct_Angle(p_ellipse->m_fSangle);
							p_ellipse->m_fEangle = ealpha;
							p_ellipse->m_fEangle = pDoc->Get_Correct_Angle(p_ellipse->m_fEangle);
							if((rotation == 90)||(rotation == 270))
							{
								double ftemp;
								ftemp = p_ellipse->m_fRadius_y;
								p_ellipse->m_fRadius_y = p_ellipse->m_fRadius_x;
								p_ellipse->m_fRadius_x = ftemp;
							}
							
							element_id = SCH_ELEMENT_ELLIPSE;
							temp_ar << element_id;
							p_ellipse->Serialize(temp_ar);
							delete p_ellipse;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
						{
							COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
							COb_schellipsefill* p_ellipsefill = new COb_schellipsefill();
							p_ellipsefill->Copy(pellipsefill);

							pDoc->Get_Rotate_XY(&fx1, &fy1, p_ellipsefill->m_fCx - fx0, p_ellipsefill->m_fCy - fy0, rotation, mirror);
							p_ellipsefill->m_fCx = fx1 + fx0 + deltax;
							p_ellipsefill->m_fCy = fy1 + fy0 + deltay;
							if((rotation == 90)||(rotation == 270))
							{
								double ftemp;
								ftemp = p_ellipsefill->m_fRadius_y;
								p_ellipsefill->m_fRadius_y = p_ellipsefill->m_fRadius_x;
								p_ellipsefill->m_fRadius_x = ftemp;
							}

							element_id = SCH_ELEMENT_ELLIPSEFILL;
							temp_ar << element_id;
							p_ellipsefill->Serialize(temp_ar);
							delete p_ellipsefill;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
						{
							COb_schpie* ppie = (COb_schpie*)pobject;
							COb_schpie* p_pie = new COb_schpie();
							p_pie->Copy(ppie);
							
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_pie->m_fCx - fx0, p_pie->m_fCy - fy0, rotation, mirror);
							p_pie->m_fCx = fx1 + fx0 + deltax;
							p_pie->m_fCy = fy1 + fy0 + deltay;
							
							double salpha, ealpha;
							pDoc->Get_Rotate_Angle(&salpha, &ealpha, p_pie->m_fSangle, p_pie->m_fEangle, rotation, mirror);
							p_pie->m_fSangle = salpha;
							p_pie->m_fSangle = pDoc->Get_Correct_Angle(p_pie->m_fSangle);
							p_pie->m_fEangle = ealpha;
							p_pie->m_fEangle = pDoc->Get_Correct_Angle(p_pie->m_fEangle);
							
							element_id = SCH_ELEMENT_PIE;
							temp_ar << element_id;
							p_pie->Serialize(temp_ar);
							delete p_pie;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
						{
							COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
							COb_schpolyline* p_polyline = new COb_schpolyline();
							p_polyline->Copy(ppolyline);

							for(int l=0; l<p_polyline->m_cVertex.GetCount(); l++)
							{
								pDoc->Get_Rotate_XY(&fx1, &fy1, p_polyline->m_cVertex[l].fx-fx0, p_polyline->m_cVertex[l].fy-fy0, rotation, mirror);
								p_polyline->m_cVertex[l].fx = fx1 + fx0 + deltax;
								p_polyline->m_cVertex[l].fy = fy1 + fy0 + deltay;
							}

							element_id = SCH_ELEMENT_POLYLINE;
							temp_ar << element_id;
							p_polyline->Serialize(temp_ar);
							delete p_polyline;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
						{
							COb_schrect* prect = (COb_schrect*)pobject;
							COb_schrect* p_rect = new COb_schrect();
							p_rect->Copy(prect);
							
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_rect->m_fX1 - fx0, p_rect->m_fY1 - fy0, rotation, mirror);
							p_rect->m_fX1 = fx1 + fx0 + deltax;
							p_rect->m_fY1 = fy1 + fy0 + deltay;
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_rect->m_fX2 - fx0, p_rect->m_fY2 - fy0, rotation, mirror);
							p_rect->m_fX2 = fx1 + fx0 + deltax;
							p_rect->m_fY2 = fy1 + fy0 + deltay;
							
							double fminx, fminy, fmaxx, fmaxy;
							fminx = MIN(p_rect->m_fX1, p_rect->m_fX2);
							fmaxx = MAX(p_rect->m_fX1, p_rect->m_fX2);
							fminy = MIN(p_rect->m_fY1, p_rect->m_fY2);
							fmaxy = MAX(p_rect->m_fY1, p_rect->m_fY2);
							p_rect->m_fX1 = fminx;
							p_rect->m_fY1 = fminy;
							p_rect->m_fX2 = fmaxx;
							p_rect->m_fY2 = fmaxy;

							element_id = SCH_ELEMENT_RECT;
							temp_ar << element_id;
							p_rect->Serialize(temp_ar);
							delete p_rect;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
						{
							COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
							COb_schroundrect* p_roundrect = new COb_schroundrect();
							p_roundrect->Copy(proundrect);

							pDoc->Get_Rotate_XY(&fx1, &fy1, p_roundrect->m_fX1 - fx0, p_roundrect->m_fY1 - fy0, rotation, mirror);
							p_roundrect->m_fX1 = fx1 + fx0 + deltax;
							p_roundrect->m_fY1 = fy1 + fy0 + deltay;
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_roundrect->m_fX2 - fx0, p_roundrect->m_fY2 - fy0, rotation, mirror);
							p_roundrect->m_fX2 = fx1 + fx0 + deltax;
							p_roundrect->m_fY2 = fy1 + fy0 + deltay;
							
							double fminx, fminy, fmaxx, fmaxy;
							fminx = MIN(p_roundrect->m_fX1, p_roundrect->m_fX2);
							fmaxx = MAX(p_roundrect->m_fX1, p_roundrect->m_fX2);
							fminy = MIN(p_roundrect->m_fY1, p_roundrect->m_fY2);
							fmaxy = MAX(p_roundrect->m_fY1, p_roundrect->m_fY2);
							p_roundrect->m_fX1 = fminx;
							p_roundrect->m_fY1 = fminy;
							p_roundrect->m_fX2 = fmaxx;
							p_roundrect->m_fY2 = fmaxy;

							element_id = SCH_ELEMENT_ROUNDRECT;
							temp_ar << element_id;
							p_roundrect->Serialize(temp_ar);
							delete p_roundrect;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
						{
							COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
							COb_schpolygon* p_polygon = new COb_schpolygon();
							p_polygon->Copy(ppolygon);
							
							for(int k=0; k<p_polygon->m_cVertex.GetCount(); k++)
							{
								pDoc->Get_Rotate_XY(&fx1, &fy1, p_polygon->m_cVertex[k].fx-fx0, p_polygon->m_cVertex[k].fy-fy0, rotation, mirror);
								p_polygon->m_cVertex[k].fx = fx1 + fx0 + deltax;
								p_polygon->m_cVertex[k].fy = fy1 + fy0 + deltay;
							}
							
							element_id = SCH_ELEMENT_POLYGON;
							temp_ar << element_id;
							p_polygon->Serialize(temp_ar);
							delete p_polygon;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
						{
							COb_schbezier* pbezier = (COb_schbezier*)pobject;
							COb_schbezier* p_bezier = new COb_schbezier();
							p_bezier->Copy(pbezier);

							for(int k=0; k<p_bezier->m_cVertex.GetCount(); k++)
							{
								pDoc->Get_Rotate_XY(&fx1, &fy1, p_bezier->m_cVertex[k].fx-fx0, p_bezier->m_cVertex[k].fy-fy0, rotation, mirror);
								p_bezier->m_cVertex[k].fx = fx1 + fx0 + deltax;
								p_bezier->m_cVertex[k].fy = fy1 + fy0 + deltay;
							}

							element_id = SCH_ELEMENT_BEZIER;
							temp_ar << element_id;
							p_bezier->Serialize(temp_ar);
							delete p_bezier;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;
							if((ptext->m_nID != TEXT_TEXT)&&(ptext->m_nID != TEXT_COMPTEXT)) continue;
							COb_schtext* p_text = new COb_schtext();
							p_text->Copy(ptext);

							pDoc->Get_Rotate_XY(&fx1, &fy1, p_text->m_fSx - fx0, p_text->m_fSy - fy0, rotation, mirror);
							p_text->m_fSx = fx1 + fx0 + deltax;
							p_text->m_fSy = fy1 + fy0 + deltay;
							int orientation;
							pDoc->Get_Rotate_Orientation(&orientation, p_text->m_nOrientation, rotation, FALSE); //mirror);
							p_text->m_nOrientation = orientation;
							p_text->m_nOrientation = pDoc->Get_Correct_Orientation(p_text->m_nOrientation);
				
							if(mirror == TRUE)
							{
								p_text->m_bMirror = FALSE;
								if((p_text->m_nOrientation == 0)||(p_text->m_nOrientation == 180))
								{
									if(p_text->m_nH_Justification == 0) p_text->m_nH_Justification = 2;
									else if(p_text->m_nH_Justification == 2) p_text->m_nH_Justification = 0;
								}
								else
								{
									if(p_text->m_nV_Justification == 0) p_text->m_nV_Justification = 2;
									else if(p_text->m_nV_Justification == 2) p_text->m_nV_Justification = 0;
								}
							}
							
							LOGFONT new_logfont = pDoc->m_arrayFont[pcomp->m_nDiagram].GetAt(ptext->m_nFont);
							int index;
							for(index=0; index<arrayFont.GetCount(); index++)
							{
								logfont = arrayFont.GetAt(index);
								if( (logfont.lfHeight == new_logfont.lfHeight) && (logfont.lfOrientation == new_logfont.lfOrientation) && \
									(logfont.lfWeight == new_logfont.lfWeight) && (logfont.lfItalic == new_logfont.lfItalic) && \
									(logfont.lfUnderline == new_logfont.lfUnderline) && (logfont.lfStrikeOut == new_logfont.lfStrikeOut) && \
									(stricmp(logfont.lfFaceName, new_logfont.lfFaceName) == 0) )
								{
									break;
								}

							}
							if(index >= arrayFont.GetCount()) p_text->m_nFont = 0;
							else p_text->m_nFont = index;

							element_id = SCH_ELEMENT_TEXT;
							temp_ar << element_id;
							p_text->Serialize(temp_ar);
							delete p_text;
						}
					}
					
					
					element_id = 0;
					temp_ar << element_id;

					temp_ar.Flush();
					temp_ar.Close();
					cfileTemp.Close();

					
					sprintf(libstring,"%s\\schlib\\font.tmp",csEDA_System_Path);
					CFile cfileTemp1;
					if(cfileTemp1.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); return;}
					int fontfile_length = cfileTemp1.GetLength();

					sprintf(libstring,"%s\\schlib\\element.tmp",csEDA_System_Path);
					CFile cfileTemp2;
					if(cfileTemp2.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); cfileTemp1.Close(); return;}
					int elementfile_length = cfileTemp2.GetLength();
					
					
					sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, dlg.m_csLibName);
					char libstringbak[MAX_PATH];
					sprintf(libstringbak,"%s\\schlib\\%stmp",csEDA_System_Path, dlg.m_csLibName);
					if(MoveFile(libstring, libstringbak) != TRUE) {AfxMessageBox("无法打开文件."); cfileTemp1.Close(); cfileTemp2.Close(); return; }

					CFile cfileTemp3;
					if(cfileTemp3.Open(libstringbak, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); cfileTemp1.Close(); cfileTemp2.Close(); return;}
					int libfile_length = cfileTemp3.GetLength();
					cfileTemp3.Read(&libhead, 32);

					
					sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, csLibraryName);
					CFile cfileUserLibrary;
					if(cfileUserLibrary.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE)
					{
						AfxMessageBox("无法打开文件.");
						cfileTemp1.Close();
						cfileTemp2.Close();
						cfileTemp3.Close();
						return;
					}
					CArchive ar(&cfileUserLibrary, CArchive::store);

					CString headformat = SCH_Library_Head;
					ar << headformat;
					int totalnum = libhead.totalcompnum + 1;
					ar << totalnum;
					ar.Flush();
					ar.Close();

					for(int i=0; i<libhead.totalcompnum; i++)
					{
						Struct_Comp_Head comp_head;
						cfileTemp3.Read(&comp_head, 320);
					
						comp_head.offset += 320 + fontfile_length - oldfont_length;
						cfileUserLibrary.Write(&comp_head, 320);
					}

					CString tstr = dlg.m_csEditCompName;
					tstr.Left(99);
					strcpy(comp_head.name, tstr);
					tstr = dlg.m_csEditDescription;
					tstr.Left(210);
					strcpy(comp_head.description, tstr);
					comp_head.length = elementfile_length;
					comp_head.offset = libfile_length + 320 + fontfile_length - oldfont_length;

					cfileUserLibrary.Write(&comp_head, 320);

					char buffer[1000];
					for(;;)
					{
						if(fontfile_length > 1000) 
						{
							cfileTemp1.Read(buffer, 1000);
							cfileUserLibrary.Write(buffer, 1000);
							fontfile_length -= 1000;
						}
						else
						{
							cfileTemp1.Read(buffer, fontfile_length);
							cfileUserLibrary.Write(buffer, fontfile_length);
							break;
						}
					}

					cfileTemp3.Seek(32 + 320*libhead.totalcompnum + oldfont_length, CFile::begin);
					libfile_length -= 32 + 320*libhead.totalcompnum + oldfont_length;
					for(;;)
					{
						if(libfile_length > 1000) 
						{
							cfileTemp3.Read(buffer, 1000);
							cfileUserLibrary.Write(buffer, 1000);
							libfile_length -= 1000;
						}
						else
						{
							cfileTemp3.Read(buffer, libfile_length);
							cfileUserLibrary.Write(buffer, libfile_length);
							break;
						}
					}

					for(;;)
					{
						if(elementfile_length > 1000) 
						{
							cfileTemp2.Read(buffer, 1000);
							cfileUserLibrary.Write(buffer, 1000);
							elementfile_length -= 1000;
						}
						else
						{
							cfileTemp2.Read(buffer, elementfile_length);
							cfileUserLibrary.Write(buffer, elementfile_length);
							break;
						}
					}
					cfileTemp1.Close();
					cfileTemp2.Close();
					cfileTemp3.Close();
					cfileUserLibrary.Close();

					arrayEDA_SchLibraryCompList.RemoveAll();

					
					sprintf(libstring,"%s\\schlib\\font.tmp",csEDA_System_Path);
					DeleteFile(libstring);
					sprintf(libstring,"%s\\schlib\\element.tmp",csEDA_System_Path);
					DeleteFile(libstring);
					DeleteFile(libstringbak);
				}
				else
				{
					
					if((dlg.m_nPartCount <= 1)||((dlg.m_nComboPart+1) > dlg.m_nPartCount))
					{
						AfxMessageBox("旧元件必须具有多套单元定义."); return;
					}

					TRACE2("旧库%s合成元件%s\n",dlg.m_csLibName, dlg.m_csStaticOldName);
					if(dlg.m_csStaticOldName.IsEmpty() == TRUE)
					{
						AfxMessageBox("无法添加, 未定义元件名称.");
						return;
					}

					CString csLibraryName = dlg.m_csLibName;
					if(csLibraryName.Find(".lib") < 0) csLibraryName += ".lib";

					
					CArray <LOGFONT, LOGFONT&>  arrayFont;
					LOGFONT logfont;
					
					char libstring[MAX_PATH];
					sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, dlg.m_csLibName);
					CFile cfileTemp;
					if(cfileTemp.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); return;}

					Struct_Library_Head libhead;
					cfileTemp.Read(&libhead, 32);
					Struct_Comp_Head comp_head;
					cfileTemp.Read(&comp_head, 320);

					int oldfont_length = comp_head.offset - 32 - 320*libhead.totalcompnum;
					BYTE* pbuffer = (BYTE *)malloc(oldfont_length);

					cfileTemp.Seek(32 + 320*libhead.totalcompnum,  CFile::begin);
					cfileTemp.Read(pbuffer, oldfont_length);
					
					CMemFile memfile(pbuffer, oldfont_length);
					CArchive oldfont_ar(&memfile,CArchive::load);
					int fontnum;
					oldfont_ar >> fontnum;
					for(int k=0; k<fontnum; k++)
					{
						Struct_Font sFont;
						oldfont_ar >> sFont.nHeight;
						oldfont_ar >> sFont.nOrientation;
						oldfont_ar >> sFont.bUnderline;
						oldfont_ar >> sFont.bItalic;
						oldfont_ar >> sFont.bBold;
						oldfont_ar >> sFont.bStrikeout;
						oldfont_ar >> sFont.csFontName;
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
					oldfont_ar.Close();
					memfile.Close();
					free(pbuffer);
					cfileTemp.Close();

					
					
					for(int k=0; k<pcomp->m_arrayPos.GetCount(); k++)
					{
						CObject* pobject;
						POSITION pos = pcomp->m_arrayPos.GetAt(k);
						pobject = pDoc->m_listSchObject[pcomp->m_nDiagram].GetNext(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;
							if((ptext->m_nID == TEXT_TEXT)||(ptext->m_nID == TEXT_COMPTEXT))
							{
								if(ptext->m_nFont == 0) continue;
								LOGFONT new_logfont = pDoc->m_arrayFont[pcomp->m_nDiagram].GetAt(ptext->m_nFont);
								int index;
								for(index=0; index<arrayFont.GetCount(); index++)
								{
									logfont = arrayFont.GetAt(index);
									if( (logfont.lfHeight == new_logfont.lfHeight) && (logfont.lfOrientation == new_logfont.lfOrientation) && \
										(logfont.lfWeight == new_logfont.lfWeight) && (logfont.lfItalic == new_logfont.lfItalic) && \
										(logfont.lfUnderline == new_logfont.lfUnderline) && (logfont.lfStrikeOut == new_logfont.lfStrikeOut) && \
										(stricmp(logfont.lfFaceName, new_logfont.lfFaceName) == 0) )
									{
										break;
									}

								}

								if(index >= arrayFont.GetCount()) arrayFont.Add(new_logfont);
							}
						}
					}
					
					
					sprintf(libstring,"%s\\schlib\\font.tmp",csEDA_System_Path);
					if(cfileTemp.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE){AfxMessageBox("无法打开文件."); return;}
					CArchive font_ar(&cfileTemp, CArchive::store);

					fontnum = arrayFont.GetCount();
					font_ar << fontnum;
					for(int k=0; k<fontnum; k++)
					{
						logfont = arrayFont.GetAt(k);
						Struct_Font sFont;
						sFont.nHeight = logfont.lfHeight;
						sFont.nOrientation = logfont.lfOrientation;
						sFont.bUnderline = logfont.lfUnderline;
						if(logfont.lfWeight == FW_NORMAL) sFont.bBold = FALSE;
						else sFont.bBold = TRUE;
						sFont.bItalic = logfont.lfItalic;
						sFont.bStrikeout = logfont.lfStrikeOut;
						sFont.csFontName = logfont.lfFaceName;
						font_ar << sFont.nHeight;
						font_ar << sFont.nOrientation;
						font_ar << sFont.bUnderline;
						font_ar << sFont.bItalic;
						font_ar << sFont.bBold;
						font_ar << sFont.bStrikeout;
						font_ar << sFont.csFontName;		
					}
					font_ar.Flush();
					font_ar.Close();
					cfileTemp.Close();
		
					
					
					sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, dlg.m_csLibName);
					CFile cfileTemp4;
					if(cfileTemp4.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); return;}

					cfileTemp4.Read(&libhead, 32);
					int filepos1, filepos2;
					Struct_Comp_Head comp_head1, comp_head2;
					int oldelement_index;
					for(int i=0; i<libhead.totalcompnum; i++)
					{
						cfileTemp4.Read(&comp_head1, 320);
						if(stricmp(comp_head1.name, dlg.m_csStaticOldName) == 0) 
						{
							oldelement_index = i;
							cfileTemp4.Read(&comp_head2, 320);
							break;
						}
					}

					int oldelement_length = comp_head2.offset - comp_head1.offset -4; 
					cfileTemp4.Close();

					
					sprintf(libstring,"%s\\schlib\\element.tmp",csEDA_System_Path);
					if(cfileTemp.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE){AfxMessageBox("无法打开文件."); return;}
					CArchive temp_ar(&cfileTemp, CArchive::store);

					
					int rotation = 0;
					//if(pcomp->m_nOrientation == 0) rotation = pcomp->m_nOrientation;
					//else  rotation = 360 - pcomp->m_nOrientation;
					double fx0, fy0, fx1, fy1, deltax, deltay;
					fx0 = pcomp->m_fX;
					fy0 = pcomp->m_fY;
					fx1 = fy1 = 0;
					deltax = fx1 - fx0;
					deltay = fy1 - fy0;
					BOOL mirror = FALSE; //pcomp->m_bMirrored;
					int element_id;
					for(int k=0; k<pcomp->m_arrayPos.GetCount(); k++)
					{
						CObject* pobject;
						POSITION pos = pcomp->m_arrayPos.GetAt(k);
						pobject = pDoc->m_listSchObject[pcomp->m_nDiagram].GetAt(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
						{
							COb_schpin* ppin = (COb_schpin*)pobject;
							COb_schpin* p_pin = new COb_schpin();
							p_pin->Copy(ppin);

							pDoc->Get_Rotate_XY(&fx1, &fy1, p_pin->m_fX - fx0, p_pin->m_fY - fy0, rotation, mirror);
							p_pin->m_fX = fx1 + fx0 + deltax;
							p_pin->m_fY = fy1 + fy0 + deltay;

							int orientation;
							pDoc->Get_Rotate_Orientation(&orientation, p_pin->m_nOrientation, rotation, mirror);
							p_pin->m_nOrientation = orientation;
							p_pin->m_nOrientation = pDoc->Get_Correct_Orientation(p_pin->m_nOrientation);	
							
							p_pin->m_nOwnerPartID = dlg.m_nComboPart + 1;
							
							element_id = SCH_ELEMENT_PIN;
							temp_ar << element_id;
							p_pin->Serialize(temp_ar);
							delete p_pin;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
						{
							COb_schieee* pieee = (COb_schieee*)pobject;
							COb_schieee* p_ieee = new COb_schieee();
							p_ieee->Copy(pieee);

							pDoc->Get_Rotate_XY(&fx1, &fy1, p_ieee->m_fX - fx0, p_ieee->m_fY - fy0, rotation, mirror);
							p_ieee->m_fX = fx1 + fx0 + deltax;
							p_ieee->m_fY = fy1 + fy0 + deltay;
							int orientation;
							pDoc->Get_Rotate_Orientation(&orientation, p_ieee->m_nOrientation, rotation, mirror);
							p_ieee->m_nOrientation = orientation;
							p_ieee->m_nOrientation = pDoc->Get_Correct_Orientation(p_ieee->m_nOrientation);
							
							p_ieee->m_nOwnerPartID = dlg.m_nComboPart + 1;

							element_id = SCH_ELEMENT_IEEE;
							temp_ar << element_id;
							p_ieee->Serialize(temp_ar);
							delete p_ieee;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
						{
							COb_scharc* parc = (COb_scharc*)pobject;
							COb_scharc* p_arc = new COb_scharc();
							p_arc->Copy(parc);
							
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_arc->m_fCx - fx0, p_arc->m_fCy - fy0, rotation, mirror);
							p_arc->m_fCx = fx1 + fx0 + deltax;
							p_arc->m_fCy = fy1 + fy0 + deltay;
							
							double salpha, ealpha;
							pDoc->Get_Rotate_Angle(&salpha, &ealpha, p_arc->m_fSangle, p_arc->m_fEangle, rotation, mirror);
							p_arc->m_fSangle = salpha;
							p_arc->m_fSangle = pDoc->Get_Correct_Angle(p_arc->m_fSangle);
							p_arc->m_fEangle = ealpha;
							p_arc->m_fEangle = pDoc->Get_Correct_Angle(p_arc->m_fEangle);
							
							p_arc->m_nOwnerPartID = dlg.m_nComboPart + 1;

							element_id = SCH_ELEMENT_ARC;
							temp_ar << element_id;
							p_arc->Serialize(temp_ar);
							delete p_arc;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
						{
							COb_schellipse* pellipse = (COb_schellipse*)pobject;
							COb_schellipse* p_ellipse = new COb_schellipse();
							p_ellipse->Copy(pellipse);
							
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_ellipse->m_fCx - fx0, p_ellipse->m_fCy - fy0, rotation, mirror);
							p_ellipse->m_fCx = fx1 + fx0 + deltax;
							p_ellipse->m_fCy = fy1 + fy0 + deltay;
							
							double salpha, ealpha;
							pDoc->Get_Rotate_Angle(&salpha, &ealpha, p_ellipse->m_fSangle, p_ellipse->m_fEangle, rotation, mirror);
							p_ellipse->m_fSangle = salpha;
							p_ellipse->m_fSangle = pDoc->Get_Correct_Angle(p_ellipse->m_fSangle);
							p_ellipse->m_fEangle = ealpha;
							p_ellipse->m_fEangle = pDoc->Get_Correct_Angle(p_ellipse->m_fEangle);
							if((rotation == 90)||(rotation == 270))
							{
								double ftemp;
								ftemp = p_ellipse->m_fRadius_y;
								p_ellipse->m_fRadius_y = p_ellipse->m_fRadius_x;
								p_ellipse->m_fRadius_x = ftemp;
							}
							
							p_ellipse->m_nOwnerPartID = dlg.m_nComboPart + 1;
							
							element_id = SCH_ELEMENT_ELLIPSE;
							temp_ar << element_id;
							p_ellipse->Serialize(temp_ar);
							delete p_ellipse;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
						{
							COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
							COb_schellipsefill* p_ellipsefill = new COb_schellipsefill();
							p_ellipsefill->Copy(pellipsefill);

							pDoc->Get_Rotate_XY(&fx1, &fy1, p_ellipsefill->m_fCx - fx0, p_ellipsefill->m_fCy - fy0, rotation, mirror);
							p_ellipsefill->m_fCx = fx1 + fx0 + deltax;
							p_ellipsefill->m_fCy = fy1 + fy0 + deltay;
							if((rotation == 90)||(rotation == 270))
							{
								double ftemp;
								ftemp = p_ellipsefill->m_fRadius_y;
								p_ellipsefill->m_fRadius_y = p_ellipsefill->m_fRadius_x;
								p_ellipsefill->m_fRadius_x = ftemp;
							}

							p_ellipsefill->m_nOwnerPartID = dlg.m_nComboPart + 1;

							element_id = SCH_ELEMENT_ELLIPSEFILL;
							temp_ar << element_id;
							p_ellipsefill->Serialize(temp_ar);
							delete p_ellipsefill;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
						{
							COb_schpie* ppie = (COb_schpie*)pobject;
							COb_schpie* p_pie = new COb_schpie();
							p_pie->Copy(ppie);
							
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_pie->m_fCx - fx0, p_pie->m_fCy - fy0, rotation, mirror);
							p_pie->m_fCx = fx1 + fx0 + deltax;
							p_pie->m_fCy = fy1 + fy0 + deltay;
							
							double salpha, ealpha;
							pDoc->Get_Rotate_Angle(&salpha, &ealpha, p_pie->m_fSangle, p_pie->m_fEangle, rotation, mirror);
							p_pie->m_fSangle = salpha;
							p_pie->m_fSangle = pDoc->Get_Correct_Angle(p_pie->m_fSangle);
							p_pie->m_fEangle = ealpha;
							p_pie->m_fEangle = pDoc->Get_Correct_Angle(p_pie->m_fEangle);
							
							p_pie->m_nOwnerPartID = dlg.m_nComboPart + 1;

							element_id = SCH_ELEMENT_PIE;
							temp_ar << element_id;
							p_pie->Serialize(temp_ar);
							delete p_pie;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
						{
							COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
							COb_schpolyline* p_polyline = new COb_schpolyline();
							p_polyline->Copy(ppolyline);

							for(int l=0; l<p_polyline->m_cVertex.GetCount(); l++)
							{
								pDoc->Get_Rotate_XY(&fx1, &fy1, p_polyline->m_cVertex[l].fx-fx0, p_polyline->m_cVertex[l].fy-fy0, rotation, mirror);
								p_polyline->m_cVertex[l].fx = fx1 + fx0 + deltax;
								p_polyline->m_cVertex[l].fy = fy1 + fy0 + deltay;
							}

							p_polyline->m_nOwnerPartID = dlg.m_nComboPart + 1;

							element_id = SCH_ELEMENT_POLYLINE;
							temp_ar << element_id;
							p_polyline->Serialize(temp_ar);
							delete p_polyline;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
						{
							COb_schrect* prect = (COb_schrect*)pobject;
							COb_schrect* p_rect = new COb_schrect();
							p_rect->Copy(prect);
							
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_rect->m_fX1 - fx0, p_rect->m_fY1 - fy0, rotation, mirror);
							p_rect->m_fX1 = fx1 + fx0 + deltax;
							p_rect->m_fY1 = fy1 + fy0 + deltay;
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_rect->m_fX2 - fx0, p_rect->m_fY2 - fy0, rotation, mirror);
							p_rect->m_fX2 = fx1 + fx0 + deltax;
							p_rect->m_fY2 = fy1 + fy0 + deltay;
							
							double fminx, fminy, fmaxx, fmaxy;
							fminx = MIN(p_rect->m_fX1, p_rect->m_fX2);
							fmaxx = MAX(p_rect->m_fX1, p_rect->m_fX2);
							fminy = MIN(p_rect->m_fY1, p_rect->m_fY2);
							fmaxy = MAX(p_rect->m_fY1, p_rect->m_fY2);
							p_rect->m_fX1 = fminx;
							p_rect->m_fY1 = fminy;
							p_rect->m_fX2 = fmaxx;
							p_rect->m_fY2 = fmaxy;

							p_rect->m_nOwnerPartID = dlg.m_nComboPart + 1;

							element_id = SCH_ELEMENT_RECT;
							temp_ar << element_id;
							p_rect->Serialize(temp_ar);
							delete p_rect;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
						{
							COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
							COb_schroundrect* p_roundrect = new COb_schroundrect();
							p_roundrect->Copy(proundrect);

							pDoc->Get_Rotate_XY(&fx1, &fy1, p_roundrect->m_fX1 - fx0, p_roundrect->m_fY1 - fy0, rotation, mirror);
							p_roundrect->m_fX1 = fx1 + fx0 + deltax;
							p_roundrect->m_fY1 = fy1 + fy0 + deltay;
							pDoc->Get_Rotate_XY(&fx1, &fy1, p_roundrect->m_fX2 - fx0, p_roundrect->m_fY2 - fy0, rotation, mirror);
							p_roundrect->m_fX2 = fx1 + fx0 + deltax;
							p_roundrect->m_fY2 = fy1 + fy0 + deltay;
							
							double fminx, fminy, fmaxx, fmaxy;
							fminx = MIN(p_roundrect->m_fX1, p_roundrect->m_fX2);
							fmaxx = MAX(p_roundrect->m_fX1, p_roundrect->m_fX2);
							fminy = MIN(p_roundrect->m_fY1, p_roundrect->m_fY2);
							fmaxy = MAX(p_roundrect->m_fY1, p_roundrect->m_fY2);
							p_roundrect->m_fX1 = fminx;
							p_roundrect->m_fY1 = fminy;
							p_roundrect->m_fX2 = fmaxx;
							p_roundrect->m_fY2 = fmaxy;

							p_roundrect->m_nOwnerPartID = dlg.m_nComboPart + 1;

							element_id = SCH_ELEMENT_ROUNDRECT;
							temp_ar << element_id;
							p_roundrect->Serialize(temp_ar);
							delete p_roundrect;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
						{
							COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
							COb_schpolygon* p_polygon = new COb_schpolygon();
							p_polygon->Copy(ppolygon);
							
							for(int k=0; k<p_polygon->m_cVertex.GetCount(); k++)
							{
								pDoc->Get_Rotate_XY(&fx1, &fy1, p_polygon->m_cVertex[k].fx-fx0, p_polygon->m_cVertex[k].fy-fy0, rotation, mirror);
								p_polygon->m_cVertex[k].fx = fx1 + fx0 + deltax;
								p_polygon->m_cVertex[k].fy = fy1 + fy0 + deltay;
							}
							
							p_polygon->m_nOwnerPartID = dlg.m_nComboPart + 1;

							element_id = SCH_ELEMENT_POLYGON;
							temp_ar << element_id;
							p_polygon->Serialize(temp_ar);
							delete p_polygon;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
						{
							COb_schbezier* pbezier = (COb_schbezier*)pobject;
							COb_schbezier* p_bezier = new COb_schbezier();
							p_bezier->Copy(pbezier);

							for(int k=0; k<p_bezier->m_cVertex.GetCount(); k++)
							{
								pDoc->Get_Rotate_XY(&fx1, &fy1, p_bezier->m_cVertex[k].fx-fx0, p_bezier->m_cVertex[k].fy-fy0, rotation, mirror);
								p_bezier->m_cVertex[k].fx = fx1 + fx0 + deltax;
								p_bezier->m_cVertex[k].fy = fy1 + fy0 + deltay;
							}

							p_bezier->m_nOwnerPartID = dlg.m_nComboPart + 1;

							element_id = SCH_ELEMENT_BEZIER;
							temp_ar << element_id;
							p_bezier->Serialize(temp_ar);
							delete p_bezier;
						}
						else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;
							if((ptext->m_nID != TEXT_TEXT)&&(ptext->m_nID != TEXT_COMPTEXT)) continue;
							COb_schtext* p_text = new COb_schtext();
							p_text->Copy(ptext);

							pDoc->Get_Rotate_XY(&fx1, &fy1, p_text->m_fSx - fx0, p_text->m_fSy - fy0, rotation, mirror);
							p_text->m_fSx = fx1 + fx0 + deltax;
							p_text->m_fSy = fy1 + fy0 + deltay;
							int orientation;
							pDoc->Get_Rotate_Orientation(&orientation, p_text->m_nOrientation, rotation, FALSE); //mirror);
							p_text->m_nOrientation = orientation;
							p_text->m_nOrientation = pDoc->Get_Correct_Orientation(p_text->m_nOrientation);
				
							if(mirror == TRUE)
							{
								p_text->m_bMirror = FALSE;
								//if(p_text->m_bMirror == TRUE) p_text->m_bMirror = FALSE;
								//else p_text->m_bMirror = TRUE;
								if((p_text->m_nOrientation == 0)||(p_text->m_nOrientation == 180))
								{
									if(p_text->m_nH_Justification == 0) p_text->m_nH_Justification = 2;
									else if(p_text->m_nH_Justification == 2) p_text->m_nH_Justification = 0;
								}
								else
								{
									if(p_text->m_nV_Justification == 0) p_text->m_nV_Justification = 2;
									else if(p_text->m_nV_Justification == 2) p_text->m_nV_Justification = 0;
								}
							}
							
							LOGFONT new_logfont = pDoc->m_arrayFont[pcomp->m_nDiagram].GetAt(ptext->m_nFont);
							int index;
							for(index=0; index<arrayFont.GetCount(); index++)
							{
								logfont = arrayFont.GetAt(index);
								if( (logfont.lfHeight == new_logfont.lfHeight) && (logfont.lfOrientation == new_logfont.lfOrientation) && \
									(logfont.lfWeight == new_logfont.lfWeight) && (logfont.lfItalic == new_logfont.lfItalic) && \
									(logfont.lfUnderline == new_logfont.lfUnderline) && (logfont.lfStrikeOut == new_logfont.lfStrikeOut) && \
									(stricmp(logfont.lfFaceName, new_logfont.lfFaceName) == 0) )
								{
									break;
								}

							}
							if(index >= arrayFont.GetCount()) p_text->m_nFont = 0;
							else p_text->m_nFont = index;

							p_text->m_nOwnerPartID = dlg.m_nComboPart + 1;

							element_id = SCH_ELEMENT_TEXT;
							temp_ar << element_id;
							p_text->Serialize(temp_ar);
							delete p_text;
						}
					}
					
					
					element_id = 0;
					temp_ar << element_id;

					temp_ar.Flush();
					temp_ar.Close();
					cfileTemp.Close();

					
					sprintf(libstring,"%s\\schlib\\font.tmp",csEDA_System_Path);
					CFile cfileTemp1;
					if(cfileTemp1.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); return;}
					int fontfile_length = cfileTemp1.GetLength();

					sprintf(libstring,"%s\\schlib\\element.tmp",csEDA_System_Path);
					CFile cfileTemp2;
					if(cfileTemp2.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); cfileTemp1.Close(); return;}
					int elementfile_length = cfileTemp2.GetLength();
					
					
					sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, dlg.m_csLibName);
					char libstringbak[MAX_PATH];
					sprintf(libstringbak,"%s\\schlib\\%stmp",csEDA_System_Path, dlg.m_csLibName);
					if(MoveFile(libstring, libstringbak) != TRUE) {AfxMessageBox("无法打开文件."); cfileTemp1.Close(); cfileTemp2.Close(); return; }

					CFile cfileTemp3;
					if(cfileTemp3.Open(libstringbak, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); cfileTemp1.Close(); cfileTemp2.Close(); return;}
					int libfile_length = cfileTemp3.GetLength();
					cfileTemp3.Read(&libhead, 32);

					
					sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, csLibraryName);
					CFile cfileUserLibrary;
					if(cfileUserLibrary.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE)
					{
						AfxMessageBox("无法打开文件.");
						cfileTemp1.Close();
						cfileTemp2.Close();
						cfileTemp3.Close();
						return;
					}
					CArchive ar(&cfileUserLibrary, CArchive::store);

					CString headformat = SCH_Library_Head;
					ar << headformat;
					int totalnum = libhead.totalcompnum;
					ar << totalnum;
					ar.Flush();
					ar.Close();

					for(int i=0; i<totalnum; i++)
					{
						Struct_Comp_Head comp_head;
						cfileTemp3.Read(&comp_head, 320);
						if(i < oldelement_index)
						{
							comp_head.offset += fontfile_length - oldfont_length;   
						}
						else if( i == oldelement_index)
						{
							comp_head.offset += fontfile_length - oldfont_length;   
							comp_head.length += elementfile_length - 4;
						}
						else
						{
							comp_head.offset += (fontfile_length - oldfont_length) + (elementfile_length - 4);	
							
						}
						cfileUserLibrary.Write(&comp_head, 320);
					}

					char buffer[1000];
					for(;;)
					{
						if(fontfile_length > 1000) 
						{
							cfileTemp1.Read(buffer, 1000);
							cfileUserLibrary.Write(buffer, 1000);
							fontfile_length -= 1000;
						}
						else
						{
							cfileTemp1.Read(buffer, fontfile_length);
							cfileUserLibrary.Write(buffer, fontfile_length);
							break;
						}
					}

					
					cfileTemp3.Seek(32 + 320*libhead.totalcompnum + oldfont_length, CFile::begin);
					int length1 = comp_head2.offset - (32 + 320*totalnum + oldfont_length) - 4;
					for(;;)
					{
						if(length1 > 1000) 
						{
							cfileTemp3.Read(buffer, 1000);
							cfileUserLibrary.Write(buffer, 1000);
							length1 -= 1000;
						}
						else
						{
							cfileTemp3.Read(buffer, length1);
							cfileUserLibrary.Write(buffer, length1);
							break;
						}
					}
					
					for(;;)
					{
						if(elementfile_length > 1000) 
						{
							cfileTemp2.Read(buffer, 1000);
							cfileUserLibrary.Write(buffer, 1000);
							elementfile_length -= 1000;
						}
						else
						{
							cfileTemp2.Read(buffer, elementfile_length);
							cfileUserLibrary.Write(buffer, elementfile_length);
							break;
						}
					}
					
					cfileTemp3.Seek(comp_head2.offset, CFile::begin);
					int length2 = libfile_length - comp_head2.offset;
					for(;;)
					{
						if(length2 > 1000) 
						{
							cfileTemp3.Read(buffer, 1000);
							cfileUserLibrary.Write(buffer, 1000);
							length2 -= 1000;
						}
						else
						{
							cfileTemp3.Read(buffer, length2);
							cfileUserLibrary.Write(buffer, length2);
							break;
						}
					}

					cfileTemp1.Close();
					cfileTemp2.Close();
					cfileTemp3.Close();
					cfileUserLibrary.Close();

					arrayEDA_SchLibraryCompList.RemoveAll();

					
					sprintf(libstring,"%s\\schlib\\font.tmp",csEDA_System_Path);
					DeleteFile(libstring);
					sprintf(libstring,"%s\\schlib\\element.tmp",csEDA_System_Path);
					DeleteFile(libstring);
					DeleteFile(libstringbak);
				}
			}

		}	
	}
}

void CESDView::OnLibCombine()
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();	
	
	if(pDoc->m_arraySelection[m_nDiagram].GetCount() < 1) { AfxMessageBox("先选择元件以及组合到元件中的图形元素."); return; }

	
	int compnum = 0;
	COb_schcomp* pSourceComp;
	POSITION posSourceComp;
	for(int i=0; i<pDoc->m_arraySelection[m_nDiagram].GetCount(); i++)
	{
		POSITION pos = pDoc->m_arraySelection[m_nDiagram].GetAt(i);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram != m_nDiagram)||(pcomp->m_nFlag == FLAG_DELETED))	continue;
			pSourceComp = pcomp;
			posSourceComp = pos;
			compnum++;
		}		
	}
	if((compnum < 1) || (compnum > 1))
	{
		AfxMessageBox("选择的图形元素中应该包含一个元件."); return;
	}

	if(AfxMessageBox("选中的所有图形元素将组合到元件中, 确认?", MB_YESNO|MB_ICONWARNING) != IDYES) return;

	
	COb_schcomp* pcomp = new COb_schcomp();
	pcomp->Copy(pSourceComp);

	
	POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pcomp);
	
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
	

	pSourceComp->m_nFlag = FLAG_DELETED;
	
	pundo = new CSchUndo;
	pundo->m_nOp = UNDO_UNDELETE;
	pundo->m_nOb = SCH_ELEMENT_COMPONENT;
	pundo->m_nDiagram = m_nDiagram;
	pundo->m_nPos = posSourceComp;
	pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
	

	pcomp->m_arrayPos.RemoveAll();	
	for(int i=0; i<pDoc->m_arraySelection[m_nDiagram].GetCount(); i++)
	{
		POSITION position = pDoc->m_arraySelection[m_nDiagram].GetAt(i);
		pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(position);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
			
			COb_schpolyline* p_polyline = new COb_schpolyline();

			p_polyline->m_nLineWidth = pline->m_nLineWidth;
			p_polyline->m_nLineType = pline->m_nLineType;
			p_polyline->m_nStartLineShape = 0;
			p_polyline->m_nEndLineShape = 0;
			p_polyline->m_nLineShapeSize = 0;
			p_polyline->m_nColor = pline->m_nColor;
			p_polyline->m_cVertex.SetSize(pline->m_cVertex.GetSize());
			for(int k=0; k<pline->m_cVertex.GetSize(); k++)
			{
				p_polyline->m_cVertex[k].fx = pline->m_cVertex[k].fx;
				p_polyline->m_cVertex[k].fy = pline->m_cVertex[k].fy;
			}
			p_polyline->m_nOwnerPartID = pcomp->m_nPart;
			p_polyline->m_nDisplayMode = pcomp->m_nDisplayMode;
			p_polyline->m_nDiagram = pcomp->m_nDiagram;
			
			p_polyline->m_posParent = posParent;

			
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_polyline);
			pcomp->m_arrayPos.Add(pos);

			pline->m_nFlag = FLAG_DELETED;
			
			pundo = new CSchUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = SCH_ELEMENT_LINE;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = position;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			

		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			
			COb_schpolyline* p_polyline = new COb_schpolyline();
			p_polyline->Copy(ppolyline);
			p_polyline->m_posParent = posParent;
			p_polyline->m_nOwnerPartID = pcomp->m_nPart;
			p_polyline->m_nDisplayMode = pcomp->m_nDisplayMode;

			
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_polyline);
			pcomp->m_arrayPos.Add(pos);

			ppolyline->m_nFlag = FLAG_DELETED;
			
			pundo = new CSchUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = SCH_ELEMENT_POLYLINE;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = position;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			
			COb_schtext* p_text = new COb_schtext();
			p_text->Copy(ptext);
			p_text->m_posParent = posParent;
			p_text->m_nOwnerPartID = pcomp->m_nPart;
			p_text->m_nDisplayMode = pcomp->m_nDisplayMode;

			
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_text);
			pcomp->m_arrayPos.Add(pos);
			
			ptext->m_nFlag = FLAG_DELETED;
			
			pundo = new CSchUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = SCH_ELEMENT_TEXT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = position;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			
			COb_scharc* p_arc = new COb_scharc();
			p_arc->Copy(parc);
			p_arc->m_posParent = posParent;
			p_arc->m_nOwnerPartID = pcomp->m_nPart;
			p_arc->m_nDisplayMode = pcomp->m_nDisplayMode;

			
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_arc);
			pcomp->m_arrayPos.Add(pos);
			
			parc->m_nFlag = FLAG_DELETED;
			
			pundo = new CSchUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = SCH_ELEMENT_ARC;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = position;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			
			COb_schellipse* p_ellipse = new COb_schellipse();
			p_ellipse->Copy(pellipse);
			p_ellipse->m_posParent = posParent;
			p_ellipse->m_nOwnerPartID = pcomp->m_nPart;
			p_ellipse->m_nDisplayMode = pcomp->m_nDisplayMode;

			
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_ellipse);
			pcomp->m_arrayPos.Add(pos);
			
			pellipse->m_nFlag = FLAG_DELETED;
			
			pundo = new CSchUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = position;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			
			COb_schellipsefill* p_ellipsefill = new COb_schellipsefill();
			p_ellipsefill->Copy(pellipsefill);
			p_ellipsefill->m_posParent = posParent;
			p_ellipsefill->m_nOwnerPartID = pcomp->m_nPart;
			p_ellipsefill->m_nDisplayMode = pcomp->m_nDisplayMode;

			
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_ellipsefill);
			pcomp->m_arrayPos.Add(pos);

			pellipsefill->m_nFlag = FLAG_DELETED;
			
			pundo = new CSchUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = position;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			
			COb_schpie* p_pie = new COb_schpie();
			p_pie->Copy(ppie);
			p_pie->m_posParent = posParent;
			p_pie->m_nOwnerPartID = pcomp->m_nPart;
			p_pie->m_nDisplayMode = pcomp->m_nDisplayMode;

			
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_pie);
			pcomp->m_arrayPos.Add(pos);

			ppie->m_nFlag = FLAG_DELETED;
			
			pundo = new CSchUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = SCH_ELEMENT_PIE;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = position;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			
			COb_schpolygon* p_polygon = new COb_schpolygon();
			p_polygon->Copy(ppolygon);
			p_polygon->m_posParent = posParent;
			p_polygon->m_nOwnerPartID = pcomp->m_nPart;
			p_polygon->m_nDisplayMode = pcomp->m_nDisplayMode;

			
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_polygon);
			pcomp->m_arrayPos.Add(pos);
			
			ppolygon->m_nFlag = FLAG_DELETED;
			
			pundo = new CSchUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = SCH_ELEMENT_POLYGON;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = position;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			
			COb_schrect* p_rect = new COb_schrect();
			p_rect->Copy(prect);
			p_rect->m_posParent = posParent;
			p_rect->m_nOwnerPartID = pcomp->m_nPart;
			p_rect->m_nDisplayMode = pcomp->m_nDisplayMode;

			
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_rect);
			pcomp->m_arrayPos.Add(pos);
			
			prect->m_nFlag = FLAG_DELETED;
			
			pundo = new CSchUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = SCH_ELEMENT_RECT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = position;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			
			COb_schroundrect* p_roundrect = new COb_schroundrect();
			p_roundrect->Copy(proundrect);
			p_roundrect->m_posParent = posParent;
			p_roundrect->m_nOwnerPartID = pcomp->m_nPart;
			p_roundrect->m_nDisplayMode = pcomp->m_nDisplayMode;

			
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_roundrect);
			pcomp->m_arrayPos.Add(pos);
			
			proundrect->m_nFlag = FLAG_DELETED;
			
			pundo = new CSchUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = position;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			
			COb_schbezier* p_bezier = new COb_schbezier();
			p_bezier->Copy(pbezier);
			p_bezier->m_posParent = posParent;
			p_bezier->m_nOwnerPartID = pcomp->m_nPart;
			p_bezier->m_nDisplayMode = pcomp->m_nDisplayMode;

			
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_bezier);
			pcomp->m_arrayPos.Add(pos);

			pbezier->m_nFlag = FLAG_DELETED;
			
			pundo = new CSchUndo;
			pundo->m_nOp = UNDO_UNDELETE;
			pundo->m_nOb = SCH_ELEMENT_BEZIER;
			pundo->m_nDiagram = m_nDiagram;
			pundo->m_nPos = position;
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			
			for(int k=0; k<pSourceComp->m_arrayPos.GetCount(); k++)
			{
				POSITION pos = pSourceComp->m_arrayPos.GetAt(k);
				pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
				{
					COb_schpin* ppin = (COb_schpin*)pobject;
					
					COb_schpin* p_pin = new COb_schpin();
					p_pin->Copy(ppin);
					p_pin->m_posParent = posParent;

					
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_pin);
					pcomp->m_arrayPos.Add(pos);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
				{
					COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
					
					COb_schpolyline* p_polyline = new COb_schpolyline();
					p_polyline->Copy(ppolyline);
					p_polyline->m_posParent = posParent;

					
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_polyline);
					pcomp->m_arrayPos.Add(pos);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext = (COb_schtext*)pobject;
					
					COb_schtext* p_text = new COb_schtext();
					p_text->Copy(ptext);
					p_text->m_posParent = posParent;

					
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_text);
					pcomp->m_arrayPos.Add(pos);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
				{
					COb_scharc* parc = (COb_scharc*)pobject;
					
					COb_scharc* p_arc = new COb_scharc();
					p_arc->Copy(parc);
					p_arc->m_posParent = posParent;
					
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_arc);
					pcomp->m_arrayPos.Add(pos);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
				{
					COb_schellipse* pellipse = (COb_schellipse*)pobject;
					
					COb_schellipse* p_ellipse = new COb_schellipse();
					p_ellipse->Copy(pellipse);
					p_ellipse->m_posParent = posParent;
					
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_ellipse);
					pcomp->m_arrayPos.Add(pos);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
				{
					COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
					
					COb_schellipsefill* p_ellipsefill = new COb_schellipsefill();
					p_ellipsefill->Copy(pellipsefill);
					p_ellipsefill->m_posParent = posParent;
					
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_ellipsefill);
					pcomp->m_arrayPos.Add(pos);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
				{
					COb_schpie* ppie = (COb_schpie*)pobject;
					
					COb_schpie* p_pie = new COb_schpie();
					p_pie->Copy(ppie);
					p_pie->m_posParent = posParent;
					
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_pie);
					pcomp->m_arrayPos.Add(pos);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
				{
					COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
					
					COb_schpolygon* p_polygon = new COb_schpolygon();
					p_polygon->Copy(ppolygon);
					p_polygon->m_posParent = posParent;
					
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_polygon);
					pcomp->m_arrayPos.Add(pos);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
				{
					COb_schrect* prect = (COb_schrect*)pobject;
					
					COb_schrect* p_rect = new COb_schrect();
					p_rect->Copy(prect);
					p_rect->m_posParent = posParent;
					
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_rect);
					pcomp->m_arrayPos.Add(pos);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
				{
					COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
					
					COb_schroundrect* p_roundrect = new COb_schroundrect();
					p_roundrect->Copy(proundrect);
					p_roundrect->m_posParent = posParent;
					
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_roundrect);
					pcomp->m_arrayPos.Add(pos);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
				{
					COb_schieee* pieee = (COb_schieee*)pobject;
					
					COb_schieee* p_ieee = new COb_schieee();
					p_ieee->Copy(pieee);
					p_ieee->m_posParent = posParent;
					
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_ieee);
					pcomp->m_arrayPos.Add(pos);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
				{
					COb_schbezier* pbezier = (COb_schbezier*)pobject;
					
					COb_schbezier* p_bezier = new COb_schbezier();
					p_bezier->Copy(pbezier);
					p_bezier->m_posParent = posParent;
					
					POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(p_bezier);
					pcomp->m_arrayPos.Add(pos);
				}
			}


		}
	}

	
	pDoc->m_nSCH_Undo_Num++;
	if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	
	Invalidate();
}

void CESDView::OnLibNew()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();	

	CDlg_SchLib_New dlg;
	dlg.sheet_height = sEDA_Standard_Sheet[pDoc->m_pSCH_Sheet[m_nDiagram]->m_nStandardStyle].height;

	if(dlg.DoModal() == IDOK)
	{
		
		COb_schcomp* pcomp = new COb_schcomp();
		pcomp->pDocument = pDoc;
		pcomp->m_nDiagram = m_nDiagram;
		pcomp->m_csLibraryReference.Empty();
		pcomp->m_csDescription.Empty();
		pcomp->m_bMirrored = FALSE;
		pcomp->m_nOrientation = 0;
		pcomp->m_bShowHidePins = FALSE;
		pcomp->m_csLibraryPath.Empty();
		pcomp->m_csLibraryFileName.Empty();
		pcomp->m_csTargetFileName.Empty();
		pcomp->m_csUniqueID = "NEWCOMP";

		pcomp->m_nFillColor = cEDA_schcomp.m_nFillColor;
		pcomp->m_nColor = cEDA_schcomp.m_nColor;
		pcomp->m_nPinColor = cEDA_schcomp.m_nPinColor;

		pcomp->m_bColorLocked = FALSE;
		pcomp->m_bDesignatorLocked = FALSE;
		pcomp->m_bPartLocked = FALSE;
		pcomp->m_bPinsMoveable = TRUE;
		pcomp->m_nGraphicalType = 0;	

		
		if(dlg.m_nRadioPart == 0)
		{
			pcomp->m_nPartCount = 1;
			pcomp->m_nPart = 1;
		}
		else
		{
			pcomp->m_nPartCount = dlg.m_nEditPartNum;
			pcomp->m_nPart = dlg.m_nComboPart + 1;
		}
		
		if(dlg.m_nRadioMode == 0)
		{
			pcomp->m_nDisplayModeCount = 1;
			pcomp->m_nDisplayMode = 0;
		}
		else
		{
			pcomp->m_nDisplayModeCount = dlg.m_nEditModeNum;
			pcomp->m_nDisplayMode = dlg.m_nComboMode;
		}

		pcomp->m_fX = dlg.m_fEditX;
		pcomp->m_fY = dlg.m_fEditY;

		
		POSITION posParent = pDoc->m_listSchObject[m_nDiagram].AddTail(pcomp);
		for(int i=0; i<dlg.m_nEditPinNum; i++)
		{
			
			COb_schpin* ppin = new COb_schpin();
			if(dlg.m_nComboPinPosition == 0)  
			{
				if(i%2 == 0)
				{
					ppin->m_fX = pcomp->m_fX;
					ppin->m_fY = pcomp->m_fY - (i/2+1)*100;
					ppin->m_nOrientation = 180;
				}
				else
				{
					ppin->m_fX = pcomp->m_fX + dlg.m_fEditWidth;
					ppin->m_fY = pcomp->m_fY - (i/2+1)*100;
					ppin->m_nOrientation = 0;
				}
			}
			else if(dlg.m_nComboPinPosition == 1)  
			{
				ppin->m_fX = pcomp->m_fX;
				ppin->m_fY = pcomp->m_fY - (i+1)*100;
				ppin->m_nOrientation = 180;
			}
			else if(dlg.m_nComboPinPosition == 2)  
			{
				ppin->m_fX = pcomp->m_fX + dlg.m_fEditWidth;
				ppin->m_fY = pcomp->m_fY - (i+1)*100;
				ppin->m_nOrientation = 0;
			}
			else  
			{
				if(i%4 == 0)
				{
					ppin->m_fX = pcomp->m_fX;
					ppin->m_fY = pcomp->m_fY - (i/4+1)*100;
					ppin->m_nOrientation = 180;
				}
				else if(i%4 == 1)
				{
					ppin->m_fX = pcomp->m_fX + (dlg.m_nEditPinNum/4+1)*100;
					ppin->m_fY = pcomp->m_fY - (i/4+1)*100;
					ppin->m_nOrientation = 0;
				}
				else if(i%4 == 2)
				{
					ppin->m_fX = pcomp->m_fX + (i/4+1)*100;
					ppin->m_fY = pcomp->m_fY; 
					ppin->m_nOrientation = 90;
				}
				else if(i%4 == 3)
				{
					ppin->m_fX = pcomp->m_fX + (i/4+1)*100;
					ppin->m_fY = pcomp->m_fY - (dlg.m_nEditPinNum/4+1)*100;
					ppin->m_nOrientation = 270;
				}
			}
			ppin->m_fPinLength = dlg.m_fEditPinLength;
			ppin->m_nColor = cEDA_schpin.m_nColor;

			ppin->m_csName.Empty();
			ppin->m_bNameVisible = TRUE;
			ppin->m_csNumber.Empty();
			ppin->m_bNumberVisible = TRUE;
			ppin->m_nElectricalType = 4;
			ppin->m_csDescription.Empty();
			ppin->m_bHide = FALSE;
			ppin->m_csConnectTo.Empty();
			ppin->m_csUniqueID.Empty();
			ppin->m_bPinLocked = TRUE;
			ppin->m_nInside = 0;
			ppin->m_nInsideEdge = 0;
			ppin->m_nOutside = 0;
			ppin->m_nOutsideEdge = 0;
			ppin->m_nOwnerPartID = pcomp->m_nPart;
			ppin->m_nDisplayMode = pcomp->m_nDisplayMode;

			ppin->pDocument = pDoc;
			ppin->m_nDiagram = m_nDiagram;
			ppin->m_posParent = posParent;
			
			
			POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ppin);
			pcomp->m_arrayPos.Add(pos);
		}

		
		COb_schtext* ptext = new COb_schtext();
		ptext->pDocument = pDoc;
		ptext->m_nDiagram = m_nDiagram;
		ptext->m_nID = TEXT_COMPDESIGNATOR;
		ptext->m_fSx = pcomp->m_fX;
		ptext->m_fSy = pcomp->m_fY + 200;
		ptext->m_nOrientation = 0;

		
		ptext->m_csText = "新元件";
		ptext->m_bTextVisible = TRUE;
		ptext->m_bTextLocked = FALSE;
		
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
		
		POSITION pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ptext);
		pcomp->m_arrayPos.Add(pos);

		
		ptext = new COb_schtext();
		ptext->pDocument = pDoc;
		ptext->m_nDiagram = m_nDiagram;
		ptext->m_nID = TEXT_COMPCOMMENT;
		ptext->m_fSx = pcomp->m_fX;
		ptext->m_fSy = pcomp->m_fY + 100;
		ptext->m_nOrientation = 0;

		
		ptext->m_csText = "未定义",
		ptext->m_bTextVisible = TRUE;
		ptext->m_bTextLocked = FALSE;
		
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
		
		pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ptext);
		pcomp->m_arrayPos.Add(pos);
			
		
		ptext = new COb_schtext();
		ptext->pDocument = pDoc;
		ptext->m_nDiagram = m_nDiagram;
		ptext->m_nID = TEXT_COMPPACKAGE;
		ptext->m_fSx = pcomp->m_fX;
		ptext->m_fSy = pcomp->m_fY + 200;
		ptext->m_nOrientation = 0;

		
		ptext->m_csText.Empty();
		ptext->m_bTextVisible = FALSE;
		ptext->m_bTextLocked = TRUE;
		
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

		
		pos = pDoc->m_listSchObject[m_nDiagram].AddTail(ptext);
		pcomp->m_arrayPos.Add(pos);

		
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
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		
	}
}


void CESDView::OnLibBrowse()
{
	CDlg_SchLib_Browse dlg;
	dlg.m_bMatchMore = bEDA_SchMatchMore;
	dlg.DoModal();

	bEDA_SchMatchMore = dlg.m_bMatchMore;
}

void CESDView::OnLibDelete()
{
	CDlg_SchLib_SourceComp dlg;
	if(dlg.DoModal() == IDOK)
	{
		char s[1000];
		if(dlg.m_complist.liboffset < 0) return;

		CString userlibrary = arrayEDA_SchUserLibraryName.GetAt(dlg.m_complist.liboffset);
		sprintf(s, "元件 %s 将从库 %s 中删除, 相关数据无法恢复, 确定?", dlg.m_complist.compname, userlibrary);
		if(AfxMessageBox(s, MB_YESNO|MB_ICONWARNING) != IDYES) return;
		
		
		char libstring[MAX_PATH], libbak[MAX_PATH];
		sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, userlibrary);
		sprintf(libbak,"%s\\schlib\\%sbak",csEDA_System_Path, userlibrary);
		if(MoveFile(libstring, libbak) != TRUE) 
		{
			AfxMessageBox("操作无效, 无法移动库文件, 检查文件是否被锁定."); 
			return;
		}
		
		CFile cfileSource;
		if(cfileSource.Open(libbak, CFile::modeRead) != TRUE)
		{
			AfxMessageBox("无法打开库文件, 检查文件是否存在或者文件属性是否为只读."); 
			return;
		}
		CFile cfileDest;
		if(cfileDest.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE)
		{
			AfxMessageBox("无法创建库文件."); 
			cfileSource.Close(); 
			return;
		}

		
		Struct_Library_Head libhead;
		cfileSource.Read(&libhead, 32);
		int totalnum = libhead.totalcompnum;
		libhead.totalcompnum--;
		cfileDest.Write(&libhead, 32);

		
		int libfont_length;
		int index;
		int deleted_length;
		CArray<int , int> arrayLength;
		
		int flag = 0;
		for(int i = 0; i < totalnum; i++)
		{
			Struct_Comp_Head comphead;
			cfileSource.Read(&comphead, 320);
			if(i == 0)
			{
				libfont_length = comphead.offset - 32 - 320*libhead.totalcompnum;
			}

			if(strcmp(dlg.m_complist.compname, comphead.name) == 0)
			{
				
				index = i;
				flag = 1;
				deleted_length = comphead.length;
			}
			else
			{
				if(flag == 0)  
				{
					comphead.offset = comphead.offset - 320;
					cfileDest.Write(&comphead, 320);
				}
				else 
				{
					comphead.offset = comphead.offset - deleted_length - 320;
					cfileDest.Write(&comphead, 320);
				}
			}
			arrayLength.Add(comphead.length);
		}
	
		unsigned char* pbuffer = (unsigned char*)malloc(1000000); 

		
		cfileSource.Read(pbuffer, libfont_length);
		cfileDest.Write(pbuffer, libfont_length);

		
		for(int i = 0; i < totalnum; i++)
		{
			int length = arrayLength.GetAt(i);
			cfileSource.Read(pbuffer, length);
			if(i == index) continue;

			cfileDest.Write(pbuffer, length);
		}

		free(pbuffer);
		
		cfileSource.Close();
		DeleteFile(libbak);
		
		cfileDest.Close();
		if(totalnum <= 1)
		{
			DeleteFile(libstring);
			arrayEDA_SchUserLibraryName.RemoveAt(dlg.m_complist.liboffset);
			arrayEDA_SchLibraryCompList.RemoveAll();
		}
	}
}

void CESDView::OnLibList()
{
	CDlg_SchLib_List dlg;
	if(dlg.DoModal() == IDOK)
	{
		CFileDialog importdialog(FALSE,"","", OFN_ENABLESIZING|OFN_OVERWRITEPROMPT,"文本文件(*.txt)|*.txt|所有文件 (*.*) |*.*||",NULL);  
		char filename[MAX_PATH];
		if(importdialog.DoModal() == IDOK) 
		{
			
			sprintf(filename,"%s\\schlib\\%s",csEDA_System_Path, dlg.m_csLibraryName);	
			
			CFile cfileSource;
			if(cfileSource.Open(filename, CFile::modeRead) != TRUE)
			{
				AfxMessageBox("无法打开库文件, 检查文件是否存在或者文件属性是否为只读."); 
				return;
			}
			strcpy(filename, importdialog.GetPathName());
			CFile cfileDest;
			if(cfileDest.Open(filename, CFile::modeWrite|CFile::modeCreate) != TRUE)
			{
				cfileSource.Close();
				AfxMessageBox("无法创建文件."); 
				return;
			}

			CArchive dest_ar(&cfileDest, CArchive::store);

			COb_libcomphead comp;

			Struct_Library_Head libhead;
			cfileSource.Read(&libhead, 32);
			int totalnum = libhead.totalcompnum;	
			
			CString onelinestr;
			onelinestr = "元件库清单: ";
			onelinestr += dlg.m_csLibraryName;
			onelinestr += "\r\n";
			dest_ar.WriteString(onelinestr);

			onelinestr.Empty();
			onelinestr = "\r\n";
			onelinestr.Empty();
			onelinestr = "序号  名称                      前缀  单元  封装                      功能\r\n";
			dest_ar.WriteString(onelinestr);

			onelinestr.Empty();
			onelinestr = "----------------------------------------------------------------------------------------------------------------------------------------------------------------\r\n";
			dest_ar.WriteString(onelinestr);

			Struct_Comp_Head comphead;
			unsigned char* pbuffer = (unsigned char*)malloc(1000000); 
			for(int i = 0; i < totalnum; i++)
			{
				cfileSource.Seek(32 + 320*i, CFile::begin);
				cfileSource.Read(&comphead, 320);

				cfileSource.Seek(comphead.offset, CFile::begin);
				cfileSource.Read(pbuffer, comphead.length);

				CMemFile memfile(pbuffer, comphead.length);
				CArchive ar(&memfile,CArchive::load);
					
				comp.Serialize(ar);
				onelinestr.Empty();
				onelinestr.Format("%d", i+1);
				onelinestr += " ";
				while(onelinestr.GetLength() < 6)
				{
					onelinestr += " ";
				}
				onelinestr += comphead.name;
				onelinestr += " ";
				while(onelinestr.GetLength() < 32)
				{
					onelinestr += " ";
				}
				onelinestr += comp.m_csRef;
				onelinestr += " ";
				while(onelinestr.GetLength() < 38)
				{
					onelinestr += " ";
				}
				char partNo_str[100];
				itoa(comp.m_nPartCount, partNo_str, 10);
				onelinestr += partNo_str;
				onelinestr += " ";
				while(onelinestr.GetLength() < 44)
				{
					onelinestr += " ";
				}
				if(comp.m_arrayPackage.GetCount() > 0)	onelinestr += comp.m_arrayPackage.GetAt(0);
				onelinestr += " ";
				while(onelinestr.GetLength() < 70)
				{
					onelinestr += " ";
				}
				onelinestr += comphead.description;
				onelinestr += "\r\n";

				dest_ar.WriteString(onelinestr);

				ar.Close();
				memfile.Close();
			}		
			
			free(pbuffer);

			dest_ar.Flush();
			dest_ar.Close();
			cfileDest.Close();

			cfileSource.Close();

		}
	}
}

void CESDView::OnLibEdit()
{
	char libstring[MAX_PATH], libbak[MAX_PATH], libtemp[MAX_PATH];
	int old_length, new_length;

	CDlg_SchLib_SourceComp dlg;
	if(dlg.DoModal() == IDOK)
	{
		
		if(dlg.m_complist.liboffset < 0) return;
		CString userlibrary = arrayEDA_SchUserLibraryName.GetAt(dlg.m_complist.liboffset);
		sprintf(libstring,"%s\\schlib\\%s",csEDA_System_Path, userlibrary);	
		
		CFile cfileSource;
		if(cfileSource.Open(libstring, CFile::modeRead) != TRUE)
		{
			AfxMessageBox("无法打开库文件, 检查文件是否存在或者文件属性是否为只读."); 
			return;
		}

		int index;
		COb_libcomphead comp;

		Struct_Library_Head libhead;
		cfileSource.Read(&libhead, 32);
		int totalnum = libhead.totalcompnum;	
		
		Struct_Comp_Head comphead;
		unsigned char* pbuffer = (unsigned char*)malloc(1000000); 
		for(int i = 0; i < totalnum; i++)
		{
			cfileSource.Read(&comphead, 320);
			if(strcmp(dlg.m_complist.compname, comphead.name) == 0)
			{
				index = i;

				cfileSource.Seek(comphead.offset, CFile::begin);
				cfileSource.Read(pbuffer, comphead.length);
				old_length = comphead.length;

				CMemFile memfile(pbuffer, comphead.length);
				CArchive ar(&memfile,CArchive::load);
				
				comp.Serialize(ar);

				ar.Close();
				memfile.Close();
				break;
			}
		}		
		cfileSource.Close();


		
		CDlg_SchLib_Attrib dlg_attrib;
		dlg_attrib.m_csEditName = comphead.name;
		dlg_attrib.m_csEditDescription = comphead.description;
		dlg_attrib.m_csEditRef = comp.m_csRef;
		if(comp.m_arrayPackage.GetCount() > 0)	dlg_attrib.m_csEditPackage = comp.m_arrayPackage.GetAt(0);
		else dlg_attrib.m_csEditPackage.Empty();
		if(dlg_attrib.DoModal() != IDOK) 
		{
			free(pbuffer);
			return;
		}

		
		sprintf(libbak,"%s\\schlib\\%sbak",csEDA_System_Path, userlibrary);
		if(MoveFile(libstring, libbak) != TRUE) 
		{
			AfxMessageBox("操作无效, 无法移动库文件, 检查文件是否被锁定."); 
			free(pbuffer);
			return;
		}
		if(cfileSource.Open(libbak, CFile::modeRead) != TRUE)
		{
			AfxMessageBox("无法打开库文件, 检查文件是否存在或者文件属性是否为只读."); 
			free(pbuffer);
			return;
		}

		
		sprintf(libtemp,"%s\\schlib\\temp.bak",csEDA_System_Path);
		CFile cfileTemp;
		if(cfileTemp.Open(libtemp, CFile::modeCreate|CFile::modeWrite) != TRUE)
		{
			AfxMessageBox("无法创建库文件."); 
			cfileSource.Close(); 
			free(pbuffer);
			return;
		}

		cfileSource.Seek(32, CFile::begin);
		for(int i = 0; i < totalnum; i++)
		{
			cfileSource.Read(&comphead, 320);
			if(i == index)
			{
				cfileSource.Seek(comphead.offset, CFile::begin);
				cfileSource.Read(pbuffer, comphead.length);

				CMemFile memfile(pbuffer, comphead.length);
				CArchive ar(&memfile,CArchive::load);

				CArchive dest_ar(&cfileTemp, CArchive::store);
				
				comp.Serialize(ar);
				comp.m_csDescription = dlg_attrib.m_csEditDescription;
				comp.m_csRef = dlg_attrib.m_csEditRef;
				comp.m_arrayPackage.RemoveAll();
				comp.m_arrayPackage.Add(dlg_attrib.m_csEditPackage);
				comp.m_arrayCompName.RemoveAll();
				comp.m_arrayCompName.Add(dlg_attrib.m_csEditName);

				comp.Serialize(dest_ar);

				int element_id;
				for(;;)
				{
					ar >> element_id;
					dest_ar << element_id;
					if(element_id <= 0) break;
					if(element_id == SCH_ELEMENT_PIN)
					{
						
						COb_schpin* ppin = new COb_schpin();
						ppin->Serialize(ar);
						ppin->Serialize(dest_ar);
						delete ppin;
					}
					else if(element_id == SCH_ELEMENT_RECT)
					{
						
						COb_schrect* prect = new COb_schrect();
						prect->Serialize(ar);
						prect->Serialize(dest_ar);
						delete prect;
					}	
					else if(element_id == SCH_ELEMENT_POLYLINE)
					{
						
						COb_schpolyline* ppolyline = new COb_schpolyline();
						ppolyline->Serialize(ar);
						ppolyline->Serialize(dest_ar);
						delete ppolyline;
					}
					else if(element_id == SCH_ELEMENT_ARC)
					{
						
						COb_scharc* parc = new COb_scharc();
						parc->Serialize(ar);
						parc->Serialize(dest_ar);
						delete parc;
					}
					else if(element_id == SCH_ELEMENT_PIE)
					{
						
						COb_schpie* ppie = new COb_schpie();
						ppie->Serialize(ar);
						ppie->Serialize(dest_ar);
						delete ppie;
					}
					else if(element_id == SCH_ELEMENT_ELLIPSE)
					{
						
						COb_schellipse* pellipse = new COb_schellipse();
						pellipse->Serialize(ar);
						pellipse->Serialize(dest_ar);
						delete pellipse;
					}
					else if(element_id == SCH_ELEMENT_ELLIPSEFILL)
					{
						
						COb_schellipsefill* pellipsefill = new COb_schellipsefill();
						pellipsefill->Serialize(ar);
						pellipsefill->Serialize(dest_ar);
						delete pellipsefill;
					}
					else if(element_id == SCH_ELEMENT_POLYGON)
					{
						
						COb_schpolygon* ppolygon = new COb_schpolygon();
						ppolygon->Serialize(ar);
						ppolygon->Serialize(dest_ar);
						delete ppolygon;
					}
					else if(element_id == SCH_ELEMENT_TEXT)
					{
						
						COb_schtext* ptext = new COb_schtext();
						ptext->Serialize(ar);
						ptext->Serialize(dest_ar);
						delete ptext;
					}
					else if(element_id == SCH_ELEMENT_BEZIER)
					{
						
						COb_schbezier* pbezier = new COb_schbezier();
						pbezier->Serialize(ar);
						pbezier->Serialize(dest_ar);
						delete pbezier;
					}
					else if(element_id == SCH_ELEMENT_ROUNDRECT)
					{
						
						COb_schroundrect* proundrect = new COb_schroundrect();
						proundrect->Serialize(ar);
						proundrect->Serialize(dest_ar);
						delete proundrect;
					}
					else if(element_id == SCH_ELEMENT_IEEE)
					{
						
						COb_schieee* pieee = new COb_schieee();
						pieee->Serialize(ar);
						pieee->Serialize(dest_ar);
						delete pieee;
					}
					else if(element_id == SCH_ELEMENT_LINE)
					{
						
						AfxMessageBox("Error! unexpected lib element");
					}
				}

				ar.Close();
				memfile.Close();

				dest_ar.Flush();
				dest_ar.Close();
				break;
			}
		}
				
		cfileTemp.Close();
		if(cfileTemp.Open(libtemp, CFile::modeRead) == TRUE)
		{
			new_length = cfileTemp.GetLength();
			cfileTemp.Close();
		}

		
		
		cfileSource.Seek(0, CFile::begin);
		cfileSource.Read(&libhead, 32);
		
		CFile cfileDest;
		if(cfileDest.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE)
		{
			AfxMessageBox("无法创建库文件."); 
			cfileSource.Close(); 
			free(pbuffer);
			return;
		}
		cfileDest.Write(&libhead, 32);

		
		int libfont_length;
		CArray<int , int> arrayLength;
		
		int flag = 0;
		for(int i = 0; i < totalnum; i++)
		{
			Struct_Comp_Head comphead;
			cfileSource.Read(&comphead, 320);
			if(i == 0)
			{
				libfont_length = comphead.offset - 32 - 320*totalnum;
			}

			if(i == index)
			{
				
				flag = 1;
				strcpy(comphead.name, dlg_attrib.m_csEditName);
				strcpy(comphead.description, dlg_attrib.m_csEditDescription);
				comphead.length = new_length;
				cfileDest.Write(&comphead, 320);
			}
			else
			{
				if(flag == 0)  
				{
					cfileDest.Write(&comphead, 320);
				}
				else 
				{
					comphead.offset = comphead.offset + (new_length - old_length);
					cfileDest.Write(&comphead, 320);
				}
			}
			arrayLength.Add(comphead.length);
		}
	

		
		cfileSource.Read(pbuffer, libfont_length);
		cfileDest.Write(pbuffer, libfont_length);

		
		for(int i = 0; i < totalnum; i++)
		{
			int length = arrayLength.GetAt(i);
			cfileSource.Read(pbuffer, length);
			if(i == index) 
			{
				if(cfileTemp.Open(libtemp, CFile::modeRead) == TRUE)
				{
					cfileTemp.Read(pbuffer, new_length);
					length = new_length;
					cfileTemp.Close();
				}
				else
				{
					AfxMessageBox("无法打开库文件.");
				}
			}

			cfileDest.Write(pbuffer, length);
		}

		DeleteFile(libtemp);

		free(pbuffer);
		
		cfileSource.Close();
		DeleteFile(libbak);
		
		cfileDest.Close();
		if(totalnum <= 1) DeleteFile(libstring);
	}
}
