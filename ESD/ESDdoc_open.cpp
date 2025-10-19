void CESDDoc::Open_99_SCH_ASCII_File(CArchive& ar, int nCurrentDiagram)
{
	int total_linenum;
	CString onelinestr;
	int library_flag = 0;


	CStringArray stringarray;

	for(;;)
	{
		if(ar.ReadString(onelinestr) == FALSE) break;
		stringarray.Add(onelinestr);
	}

	total_linenum = stringarray.GetSize();

	int lib_num, lib_pos;
	char s[1000], ss[200][200], special[200][200];

	for(int i=0; i<total_linenum; i++)
    {
		onelinestr = stringarray.GetAt(i);
		if(onelinestr.GetLength() > 999) onelinestr.SetAt(999, '\0');
		strcpy(s, onelinestr);

		int j=Divide_l(s,ss);
		if(stricmp(ss[0], "[Font_Table]") == 0)
		{
			i++;
			onelinestr = stringarray.GetAt(i);
			int fontnum = atoi(onelinestr); 

			if(fontnum > 0) m_arrayFont[nCurrentDiagram].RemoveAll();
			for(int k=0; k<fontnum; k++)
			{
				i++;
				onelinestr = stringarray.GetAt(i);
				strcpy(s, onelinestr);

				int j = Divide_l(s,ss);
				onelinestr = CString(ss[6]);
				for(int t=1; t<j-6; t++)
				{
					onelinestr += CString(" ") + CString(ss[6+t]);
				}

				LOGFONT logfont;
				logfont.lfHeight = atoi(ss[0]);
				logfont.lfOrientation = atoi(ss[1]);
				logfont.lfOrientation = Get_Correct_Orientation(logfont.lfOrientation);

				onelinestr.Left(31);
				strcpy(logfont.lfFaceName,onelinestr);

				if(strcmp(ss[2], "0") == 0) logfont.lfUnderline = FALSE;
				else logfont.lfUnderline = TRUE;
				if(strcmp(ss[3], "0") == 0) logfont.lfItalic = FALSE;
				else logfont.lfItalic = TRUE;
				if(strcmp(ss[4], "0") == 0) logfont.lfWeight = FW_NORMAL;
				else logfont.lfWeight = FW_BOLD;
				if(strcmp(ss[5], "0") == 0) logfont.lfStrikeOut = FALSE;
				else logfont.lfStrikeOut = TRUE;

				//其他参数
				logfont.lfWidth = 0;
				logfont.lfEscapement = logfont.lfOrientation*10;
				logfont.lfCharSet = DEFAULT_CHARSET; // always
				logfont.lfOutPrecision = OUT_DEFAULT_PRECIS; // always
				logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS; // always
				logfont.lfQuality = PROOF_QUALITY; //always
				logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN; //always
				m_arrayFont[nCurrentDiagram].Add(logfont);
			}

			continue;
		}
		if(stricmp(ss[0], "Future") == 0)
		{
			m_bSCH_ElecGridEnable = atoi(ss[1]);
			m_fSCH_ElecGrid = atoi(ss[2]);
			m_pSCH_Sheet[nCurrentDiagram]->m_nStandardStyle = atoi(ss[3]);

			continue;
		}
		if(stricmp(ss[0], "Library") == 0)
		{
			if(library_flag == 0) 
			{
				library_flag ++;
				if(stricmp(ss[1], "Version") == 0) continue;
				i++;
				onelinestr = stringarray.GetAt(i);
				onelinestr.TrimLeft();
				onelinestr.TrimRight();
				lib_num = atoi(onelinestr);

				lib_pos = i;
			}
			continue;
		}
		if(stricmp(ss[0], "Sheet") == 0)
		{
			i++;
			onelinestr = stringarray.GetAt(i);
			strcpy(s, onelinestr);

			int j = Divide_l(s,ss);

			m_pSCH_Sheet[nCurrentDiagram]->m_nTitleBlockStyle = atoi(ss[0]);

			if(m_pSCH_Sheet[nCurrentDiagram]->m_nStandardStyle == 0) m_pSCH_Sheet[nCurrentDiagram]->m_nStandardStyle = atoi(ss[1]);

			m_pSCH_Sheet[nCurrentDiagram]->m_nOrientation = atoi(ss[2]);

			if(strcmp(ss[3],"0") == 0) m_pSCH_Sheet[nCurrentDiagram]->m_bShowBorder = FALSE;
			else m_pSCH_Sheet[nCurrentDiagram]->m_bShowBorder = TRUE;

			if(strcmp(ss[4],"0") == 0) m_pSCH_Sheet[nCurrentDiagram]->m_bTitleBlock = FALSE;
			else m_pSCH_Sheet[nCurrentDiagram]->m_bTitleBlock = TRUE;

			m_pSCH_Sheet[nCurrentDiagram]->m_nBorderColor = atoi(ss[5]);

			m_pSCH_Sheet[nCurrentDiagram]->m_nSheetColor = atoi(ss[6]);

			if(strcmp(ss[7],"0") == 0) m_bSCH_SnapGridEnable = FALSE;
			else m_bSCH_SnapGridEnable = TRUE;

			m_fSCH_SnapGrid = atoi(ss[8]);

			if(strcmp(ss[9],"0") == 0)  m_bSCH_VisibleGridShow = FALSE;
			else m_bSCH_VisibleGridShow = TRUE;

			m_fSCH_VisibleGrid = atoi(ss[10]);

			m_pSCH_Sheet[nCurrentDiagram]->m_fCustomWidth = atoi(ss[11])*10.0f;

			m_pSCH_Sheet[nCurrentDiagram]->m_fCustomHeight = atoi(ss[12])*10.0f;

			if(strcmp(ss[13],"0") == 0) m_pSCH_Sheet[nCurrentDiagram]->m_bCustom = FALSE;
			else m_pSCH_Sheet[nCurrentDiagram]->m_bCustom = TRUE;


			for(;;)
			{
				i++;
				onelinestr = stringarray.GetAt(i);
				strcpy(s, onelinestr);

				int j = Divide_long(s,special);

				if(stricmp(special[0],"EndSheet") == 0)	break;
				else if(stricmp(special[0], "Wire") == 0)
				{
					COb_schline* pline = new COb_schline();
					pline->pDocument = this;
					pline->m_nDiagram = nCurrentDiagram;	
							
					pline->m_nAttrib = LINE_WIRE;

					pline->m_nLineWidth = atoi(special[1]);

					pline->m_nColor = atoi(special[2]);

					int vertex = atoi(special[4]);
					pline->m_cVertex.SetSize(vertex);
					for(int k=0; k<vertex; k++)
					{
						pline->m_cVertex[k].fx = atoi(special[5+2*k])*10.0f;
						pline->m_cVertex[k].fy = atoi(special[5+2*k+1])*10.0f;
					}

					pline->m_nLineType = 0;
							
					m_listSchObject[nCurrentDiagram].AddTail(pline);

				}
				else if(stricmp(special[0], "Polyline") == 0)
				{
					COb_schpolyline* ppolyline = new COb_schpolyline();
					ppolyline->pDocument = this;
					ppolyline->m_nDiagram = nCurrentDiagram;	


					ppolyline->m_nLineWidth = atoi(special[1]);

					ppolyline->m_nLineType = atoi(special[2]);

					ppolyline->m_nColor = atoi(special[3]);

					int vertex = atoi(special[5]);
					ppolyline->m_cVertex.SetSize(vertex);
					for(int k=0; k<vertex; k++)
					{
						ppolyline->m_cVertex[k].fx = atoi(special[6+2*k])*10.0f;
						ppolyline->m_cVertex[k].fy = atoi(special[6+2*k+1])*10.0f;
					}

					ppolyline->m_nStartLineShape = 0;

					ppolyline->m_nEndLineShape = 0;

					ppolyline->m_nLineShapeSize = 0;

					m_listSchObject[nCurrentDiagram].AddTail(ppolyline);
				}
				else if(stricmp(special[0], "Bezier") == 0)
				{
					COb_schbezier* pbezier = new COb_schbezier();
					pbezier->pDocument = this;
					pbezier->m_nDiagram = nCurrentDiagram;	

					pbezier->m_nWidth = atoi(special[1]);

					pbezier->m_nColor = atoi(special[2]);

					int vertex = 0;
					vertex = atoi(special[4]);
					pbezier->m_cVertex.SetSize(vertex);
					for(int k=0; k < vertex; k++)
					{
						pbezier->m_cVertex[k].fx = atoi(special[5+2*k])*10.0f;
						pbezier->m_cVertex[k].fy = atoi(special[5+2*k+1])*10.0f;
					}
					
					m_listSchObject[nCurrentDiagram].AddTail(pbezier);

				}
				else if(stricmp(special[0], "Rectangle") == 0)
				{
					COb_schrect* prect = new COb_schrect();
					prect->pDocument = this;
					prect->m_nDiagram = nCurrentDiagram;	

					prect->m_fX1 = atoi(special[1])*10.0f;

					prect->m_fY1 = atoi(special[2])*10.0f;

					prect->m_fX2 = atoi(special[3])*10.0f;

					prect->m_fY2 = atoi(special[4])*10.0f;
				
					prect->m_nBorderWidth = atoi(special[5]);
					
					prect->m_nBorderColor = atoi(special[6]);
					prect->m_nFillColor = atoi(special[7]);
					
					prect->m_bFillSolid = atoi(special[9]);

					prect->m_bTransparent = FALSE;

					m_listSchObject[nCurrentDiagram].AddTail(prect);

				}
				else if(stricmp(special[0], "Image") == 0)
				{
					COb_schgraphic* pgraphic = new COb_schgraphic();
					pgraphic->pDocument = this;
					pgraphic->m_nDiagram = nCurrentDiagram;	

					pgraphic->m_fX1 = atoi(special[1])*10.0f;

					pgraphic->m_fY1 = atoi(special[2])*10.0f;

					pgraphic->m_fX2 = atoi(special[3])*10.0f;

					pgraphic->m_fY2 = atoi(special[4])*10.0f;


					pgraphic->m_nBorderWidth = atoi(special[5]);

					pgraphic->m_nBorderColor = atoi(special[6]);

					pgraphic->m_bBorderOn = atoi(special[8]);

					pgraphic->m_bAspect = atoi(special[9]);
					
					pgraphic->m_bEmbedded = FALSE;

					pgraphic->m_csFileName = (char*)special[10];
					pgraphic->m_csFileName.TrimLeft(0x27);
					pgraphic->m_csFileName.TrimRight(0x27);

					pgraphic->LoadBitmap();

					m_listSchObject[nCurrentDiagram].AddTail(pgraphic);

				}
				else if(stricmp(special[0], "RoundRectangle") == 0)
				{
					COb_schroundrect* proundrect = new COb_schroundrect();
					proundrect->pDocument = this;
					proundrect->m_nDiagram = nCurrentDiagram;	
					
					proundrect->m_fX1 = atoi(special[1])*10.0f;

					proundrect->m_fY1 = atoi(special[2])*10.0f;

					proundrect->m_fX2 = atoi(special[3])*10.0f;

					proundrect->m_fY2 = atoi(special[4])*10.0f;

					proundrect->m_fRadius_x = atoi(special[5])*10.0f;

					proundrect->m_fRadius_y = atoi(special[6])*10.0f;
						

					proundrect->m_nBorderWidth = atoi(special[7]);

					proundrect->m_nBorderColor = atoi(special[8]);
					proundrect->m_nFillColor = atoi(special[9]);

					proundrect->m_bFillSolid = atoi(special[11]);

					m_listSchObject[nCurrentDiagram].AddTail(proundrect);

				}
				else if(stricmp(special[0], "Bus") == 0)
				{
					COb_schline* pline = new COb_schline();
					pline->pDocument = this;
					pline->m_nDiagram = nCurrentDiagram;	
							
					pline->m_nAttrib = LINE_BUS;

					pline->m_nLineWidth = atoi(special[1]);

					pline->m_nColor = atoi(special[2]);

					int vertex = atoi(special[4]);
					pline->m_cVertex.SetSize(vertex);
					for(int k=0; k<vertex; k++)
					{
						pline->m_cVertex[k].fx = atoi(special[5+2*k])*10.0f;
						pline->m_cVertex[k].fy = atoi(special[5+2*k+1])*10.0f;
					}

					pline->m_nLineType = 0;
							
					m_listSchObject[nCurrentDiagram].AddTail(pline);

				}
				else if(stricmp(special[0], "BusEntry") == 0)
				{
					COb_schline* pline = new COb_schline();
					pline->pDocument = this;
					pline->m_nDiagram = nCurrentDiagram;	
							
					pline->m_nAttrib = LINE_BUS_ENTRY;


					int vertex = 2;
					pline->m_cVertex.SetSize(vertex);
					for(int k=0; k<vertex; k++)
					{
						pline->m_cVertex[k].fx = atoi(special[1+2*k])*10.0f;
						pline->m_cVertex[k].fy = atoi(special[1+2*k+1])*10.0f;
					}


					pline->m_nLineWidth = atoi(special[5]);

					pline->m_nColor = atoi(special[6]);

					pline->m_nLineType = 0;
							

					m_listSchObject[nCurrentDiagram].AddTail(pline);

				}
				else if(stricmp(special[0], "Junction") == 0)
				{
					COb_schjunc* pjunc = new COb_schjunc();
					pjunc->pDocument = this;
					pjunc->m_nDiagram = nCurrentDiagram;	

					pjunc->m_fCx = atoi(special[1])*10.0f;

					pjunc->m_fCy = atoi(special[2])*10.0f;

					pjunc->m_nSize = atoi(special[3]);

					pjunc->m_nColor = atoi(special[4]);
					
					pjunc->m_bManual = FALSE;

					m_listSchObject[nCurrentDiagram].AddTail(pjunc);
				}
				else if(stricmp(special[0], "Port") == 0)
				{
					COb_schport* pport = new COb_schport();
					pport->pDocument = this;
					pport->m_nDiagram = nCurrentDiagram;	

					pport->m_nStyle = atoi(special[1]);

					pport->m_nType = atoi(special[2]);

					pport->m_nAlignment = atoi(special[3]);

					pport->m_fSize = atoi(special[4])*10.0f;

					pport->m_fX = atoi(special[5])*10.0f;

					pport->m_fY = atoi(special[6])*10.0f;

					pport->m_nBorderColor = atoi(special[7]);
					pport->m_nFillColor = atoi(special[8]);
					pport->m_nTextColor = atoi(special[9]);

					pport->m_csName = (char*)special[11];
					pport->m_csName.TrimLeft(0x27);
					pport->m_csName.TrimRight(0x27);

					pport->m_csUniqueID.Empty();

					m_listSchObject[nCurrentDiagram].AddTail(pport);

				}
				else if(stricmp(special[0], "Label") == 0)
				{
					COb_schtext* ptext = new COb_schtext();
					ptext->pDocument = this;
					ptext->m_nDiagram = nCurrentDiagram;	

					ptext->m_nID = TEXT_TEXT;
					ptext->m_csNameComment.Empty();

					ptext->m_fSx = atoi(special[1])*10.0f;

					ptext->m_fSy = atoi(special[2])*10.0f;

					ptext->m_nOrientation = atoi(special[3])*90;
					ptext->m_nOrientation = Get_Correct_Orientation(ptext->m_nOrientation);

					ptext->m_nColor = atoi(special[4]);

					ptext->m_nFont = atoi(special[5]) - 1;

					ptext->m_csText = (char*)special[7];
					ptext->m_csText.TrimLeft(0x27);
					ptext->m_csText.TrimRight(0x27);

					ptext->m_bMirror = FALSE;

					ptext->m_nH_Justification = 0;
					ptext->m_nV_Justification = 0;

					m_listSchObject[nCurrentDiagram].AddTail(ptext);

				}
				else if(stricmp(special[0], "NetLabel") == 0)
				{
					COb_schtext* ptext = new COb_schtext();
					ptext->pDocument = this;
					ptext->m_nDiagram = nCurrentDiagram;	

					ptext->m_nID = TEXT_NETLABEL;
					ptext->m_csNameComment.Empty();

					ptext->m_fSx = atoi(special[1])*10.0f;

					ptext->m_fSy = atoi(special[2])*10.0f;

					ptext->m_nOrientation = atoi(special[3])*90;
					ptext->m_nOrientation = Get_Correct_Orientation(ptext->m_nOrientation);

					ptext->m_nColor = atoi(special[4]);

					ptext->m_nFont = atoi(special[5]) - 1;

					ptext->m_csText = (char*)special[7];
					ptext->m_csText.TrimLeft(0x27);
					ptext->m_csText.TrimRight(0x27);

					ptext->m_bMirror = FALSE;

					ptext->m_nH_Justification = 0;
					ptext->m_nV_Justification = 0;

					m_listSchObject[nCurrentDiagram].AddTail(ptext);

				}
				else if(stricmp(special[0], "Arc") == 0)
				{
					COb_scharc* parc = new COb_scharc();
					parc->pDocument = this;
					parc->m_nDiagram = nCurrentDiagram;	

					parc->m_fCx = atoi(special[1])*10.0f;
					parc->m_fCy = atoi(special[2])*10.0f;

					parc->m_fRadius = atoi(special[3])*10.0f;

					parc->m_nWidth = atoi(special[4]);

					parc->m_fSangle = atof(special[5]);
					parc->m_fEangle = atof(special[6]);
					parc->m_fSangle = Get_Correct_Angle(parc->m_fSangle);
					parc->m_fEangle = Get_Correct_Angle(parc->m_fEangle);
					if(parc->m_fSangle == parc->m_fEangle)
					{
						parc->m_fSangle = 0;
						parc->m_fEangle = 360;
					}

					parc->m_nColor = atoi(special[7]);

					m_listSchObject[nCurrentDiagram].AddTail(parc);

				}
				else if(stricmp(special[0], "Pie") == 0)
				{
					COb_schpie* ppie = new COb_schpie();
					ppie->pDocument = this;
					ppie->m_nDiagram = nCurrentDiagram;	

					ppie->m_fCx = atoi(special[1])*10.0f;
					ppie->m_fCy = atoi(special[2])*10.0f;

					ppie->m_fRadius = atoi(special[3])*10.0f;

					ppie->m_nBorderWidth = atoi(special[4]);

					ppie->m_fSangle = atof(special[5]);
					ppie->m_fEangle = atof(special[6]);
					ppie->m_fSangle = Get_Correct_Angle(ppie->m_fSangle);
					ppie->m_fEangle = Get_Correct_Angle(ppie->m_fEangle);
					if(ppie->m_fSangle == ppie->m_fEangle)
					{
						ppie->m_fSangle = 0;
						ppie->m_fEangle = 360;
					}

					ppie->m_nBorderColor = atoi(special[7]);
					ppie->m_nFillColor = atoi(special[8]);

					ppie->m_bFillSolid = atoi(special[9]);

					m_listSchObject[nCurrentDiagram].AddTail(ppie);
				}
				else if(stricmp(special[0], "EllipticalArc") == 0)
				{
					COb_schellipse* pellipse = new COb_schellipse();
					pellipse->pDocument = this;
					pellipse->m_nDiagram = nCurrentDiagram;	
					
					pellipse->m_fCx = atoi(special[1])*10.0f;
					pellipse->m_fCy = atoi(special[2])*10.0f;

					pellipse->m_fRadius_x = atoi(special[3])*10.0f;
					pellipse->m_fRadius_y = atoi(special[4])*10.0f;

					pellipse->m_nWidth = atoi(special[5]);

					pellipse->m_fSangle = atof(special[6]);
					pellipse->m_fEangle = atof(special[7]);
					pellipse->m_fSangle = Get_Correct_Angle(pellipse->m_fSangle);
					pellipse->m_fEangle = Get_Correct_Angle(pellipse->m_fEangle);					
					if(pellipse->m_fSangle == pellipse->m_fEangle)
					{
						pellipse->m_fSangle = 0;
						pellipse->m_fEangle = 360;
					}

					pellipse->m_nColor = atoi(special[8]);

					m_listSchObject[nCurrentDiagram].AddTail(pellipse);

				}
				else if(stricmp(special[0], "Ellipse") == 0)
				{
					COb_schellipsefill* pellipsefill = new COb_schellipsefill();
					pellipsefill->pDocument = this;
					pellipsefill->m_nDiagram = nCurrentDiagram;	
							
					pellipsefill->m_fCx = atoi(special[1])*10.0f;

					pellipsefill->m_fCy = atoi(special[2])*10.0f;
					
					pellipsefill->m_fRadius_x = atoi(special[3])*10.0f;

					pellipsefill->m_fRadius_y = atoi(special[4])*10.0f;

					pellipsefill->m_nBorderWidth = atoi(special[5]);

					pellipsefill->m_nBorderColor = atoi(special[6]);

					pellipsefill->m_nFillColor = atoi(special[7]);

					pellipsefill->m_bFillSolid = atoi(special[8]);

					pellipsefill->m_bTransparent = FALSE;

					m_listSchObject[nCurrentDiagram].AddTail(pellipsefill);

				}
				else if(stricmp(special[0], "Polygon") == 0)
				{
					COb_schpolygon* ppolygon = new COb_schpolygon();
					ppolygon->pDocument = this;
					ppolygon->m_nDiagram = nCurrentDiagram;

					ppolygon->m_nBorderWidth = atoi(special[1]);

					ppolygon->m_nBorderColor = atoi(special[2]);

					ppolygon->m_nFillColor = atoi(special[3]);

					ppolygon->m_bFillSolid = atoi(special[4]);

					ppolygon->m_bTransparent = FALSE;

					int vertex = atoi(special[6]);
					ppolygon->m_cVertex.SetSize(vertex);
					for(int k=0; k<vertex; k++)
					{
						ppolygon->m_cVertex[k].fx = atoi(special[7+2*k])*10.0f;
						ppolygon->m_cVertex[k].fy = atoi(special[7+2*k+1])*10.0f;
					}

					m_listSchObject[nCurrentDiagram].AddTail(ppolygon);
		
				}
				else if(stricmp(special[0], "TextFrame") == 0)
				{
					COb_schtextframe* ptextframe = new COb_schtextframe();
					ptextframe->pDocument = this;
					ptextframe->m_nDiagram = nCurrentDiagram;	

					ptextframe->m_fX1 = atoi(special[1])*10.0f;

					ptextframe->m_fY1 = atoi(special[2])*10.0f;

					ptextframe->m_fX2 = atoi(special[3])*10.0f;

					ptextframe->m_fY2 = atoi(special[4])*10.0f;
					if(ptextframe->m_fY2 < ptextframe->m_fY1)
					{
						double f = ptextframe->m_fY1;
						ptextframe->m_fY1 = ptextframe->m_fY2;
						ptextframe->m_fY2 = f;
					}

					ptextframe->m_nBorderWidth = atoi(special[5]);

					ptextframe->m_nBorderColor = atoi(special[6]);
					ptextframe->m_nFillColor = atoi(special[7]);
					ptextframe->m_nTextColor = atoi(special[8]);

					ptextframe->m_nFont = atoi(special[9]) - 1;

					ptextframe->m_bFillSolid = atoi(special[10]);

					ptextframe->m_bDrawBorder = atoi(special[11]);

					ptextframe->m_nAlignment = atoi(special[12]);

					ptextframe->m_bWordWrap = atoi(special[13]);

					ptextframe->m_bClipToArea = atoi(special[14]);

					for(;;)
					{
						i++;
						onelinestr = stringarray.GetAt(i);
						strcpy(s, onelinestr);

						int j = Divide_long(s,special);
						if(stricmp(special[0],"Begin") == 0) continue;
						else if(stricmp(special[0],"End") == 0)	break;
						else
						{
							onelinestr.TrimLeft();
							onelinestr.TrimRight();
							ptextframe->m_arrayText.Add(onelinestr);
						}
					}

					m_listSchObject[nCurrentDiagram].AddTail(ptextframe);

				}
				else if(stricmp(special[0], "PowerObject") == 0)
				{
					COb_schpower* ppower = new COb_schpower();
					ppower->pDocument = this;
					ppower->m_nDiagram = nCurrentDiagram;	
					
					ppower->m_nStyle = atoi(special[1]);
					
					ppower->m_fX = atoi(special[2])*10.0f;

					ppower->m_fY = atoi(special[3])*10.0f;

					ppower->m_nOrientation = atoi(special[4])*90;
					ppower->m_nOrientation = Get_Correct_Orientation(ppower->m_nOrientation);

					ppower->m_nColor = atoi(special[5]);

					ppower->m_csNetName = (char*)special[7];
					ppower->m_csNetName.TrimLeft(0x27);
					ppower->m_csNetName.TrimRight(0x27);

					if(ppower->m_nStyle < 4) ppower->m_bShowNetName = TRUE;
					else ppower->m_bShowNetName = FALSE;

					ppower->m_bIsCrossSheetConnector = FALSE;

					m_listSchObject[nCurrentDiagram].AddTail(ppower);					

				}
				else if(stricmp(special[0], "SheetSymbol") == 0)
				{
					COb_schsheetsymbol* psheetsymbol = new COb_schsheetsymbol();
					psheetsymbol->pDocument = this;
					psheetsymbol->m_nDiagram = nCurrentDiagram;	

					psheetsymbol->m_fX = atoi(special[1])*10.0f;

					psheetsymbol->m_fY = atoi(special[2])*10.0f;

					psheetsymbol->m_fXsize = atoi(special[3])*10.0f;

					psheetsymbol->m_fYsize = atoi(special[4])*10.0f;

					psheetsymbol->m_nBorderWidth = atoi(special[5]);

					psheetsymbol->m_nBorderColor = atoi(special[6]);
					psheetsymbol->m_nFillColor = atoi(special[7]);
					
					psheetsymbol->m_bFillSolid = atoi(special[9]);

					psheetsymbol->m_bShowHiddenTextFields = FALSE;

					psheetsymbol->m_csUniqueID.Empty();

					POSITION posParent = m_listSchObject[nCurrentDiagram].AddTail(psheetsymbol);
				
					for(;;)
					{
						i++;
						onelinestr = stringarray.GetAt(i);
						strcpy(s, onelinestr);

						int j = Divide_long(s,special);

						if(stricmp(special[0],"SheetFileName") == 0)
						{
							COb_schtext* ptext = new COb_schtext();
							ptext->pDocument = this;
							ptext->m_nDiagram = nCurrentDiagram;	

							ptext->m_nID = TEXT_SHEETSYMBOLFILENAME;

							ptext->m_fSx = atoi(special[1])*10.0f;

							ptext->m_fSy = atoi(special[2])*10.0f;

							ptext->m_nOrientation = atoi(special[3])*90;
							ptext->m_nOrientation = Get_Correct_Orientation(ptext->m_nOrientation);

							ptext->m_nColor = atoi(special[4]);

							ptext->m_nFont = atoi(special[5]) - 1;

							if(atoi(special[7]) == 0)	ptext->m_bTextVisible = TRUE;
							else ptext->m_bTextVisible = FALSE;

							ptext->m_csText = (char*)special[8];
							ptext->m_csText.TrimLeft(0x27);
							ptext->m_csText.TrimRight(0x27);

							ptext->m_bAutoPosition = FALSE;

							ptext->m_nAnChorH = 0;	

							ptext->m_nAnChorV = 0;

							ptext->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptext);

							psheetsymbol->m_arrayPos.Add(pos);
						}
						else if(stricmp(special[0],"SheetName") == 0)
						{
							COb_schtext* ptext = new COb_schtext();
							ptext->pDocument = this;
							ptext->m_nDiagram = nCurrentDiagram;	

							ptext->m_nID = TEXT_SHEETSYMBOLDESIGNATOR;

							ptext->m_fSx = atoi(special[1])*10.0f;

							ptext->m_fSy = atoi(special[2])*10.0f;

							ptext->m_nOrientation = atoi(special[3])*90;
							ptext->m_nOrientation = Get_Correct_Orientation(ptext->m_nOrientation);

							ptext->m_nColor = atoi(special[4]);

							ptext->m_nFont = atoi(special[5]) - 1;

							if(atoi(special[7]) == 0)	ptext->m_bTextVisible = TRUE;
							else ptext->m_bTextVisible = FALSE;

							ptext->m_csText = (char*)special[8];
							ptext->m_csText.TrimLeft(0x27);
							ptext->m_csText.TrimRight(0x27);

							ptext->m_bAutoPosition = FALSE;

							ptext->m_nAnChorH = 0;	

							ptext->m_nAnChorV = 0;

							ptext->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptext);

							psheetsymbol->m_arrayPos.Add(pos);
						}
						else if(stricmp(special[0],"SheetNet") == 0)
						{
							COb_schsheetentry* psheetentry = new COb_schsheetentry();
							psheetentry->pDocument = this;
							psheetentry->m_nDiagram = nCurrentDiagram;	

							psheetentry->m_posParent = posParent;
							
							psheetentry->m_nType = atoi(special[1]);

							psheetentry->m_nStyle = atoi(special[2]);

							psheetentry->m_nSide = atoi(special[3]);

							psheetentry->m_fOffset = atoi(special[4])*100.0f; 

							psheetentry->m_nBorderColor = atoi(special[5]);
							psheetentry->m_nFillColor = atoi(special[6]);
							psheetentry->m_nTextColor = atoi(special[7]);

							psheetentry->m_csName = (char*)special[9];
							psheetentry->m_csName.TrimLeft(0x27);
							psheetentry->m_csName.TrimRight(0x27);

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(psheetentry);

							psheetsymbol->m_arrayPos.Add(pos);
						}
						else if(stricmp(special[0],"EndSheetSymbol") == 0) break;
					}
				}
				else if(stricmp(special[0], "Part") == 0)
				{
					COb_schcomp* pcomp = new COb_schcomp();
					pcomp->pDocument = this;
					pcomp->m_nDiagram = nCurrentDiagram;	

					pcomp->m_fX = atoi(special[1])*10.0f;

					pcomp->m_fY = atoi(special[2])*10.0f;

					pcomp->m_nDisplayModeCount = 1;

					pcomp->m_nDisplayMode = atoi(special[3]);

					pcomp->m_bMirrored = atoi(special[4]);

					pcomp->m_nOrientation = atoi(special[5])*90;
					pcomp->m_nOrientation = Get_Correct_Orientation(pcomp->m_nOrientation);

					pcomp->m_nPart = atoi(special[7]);
					
					int library_component_id = atoi(special[8]);
					
					pcomp->m_bShowHidePins = atoi(special[10]);
			
					pcomp->m_csLibraryReference = (char*)special[11];
					pcomp->m_csLibraryReference.TrimLeft(0x27);
					pcomp->m_csLibraryReference.TrimRight(0x27);

					pcomp->m_csLibraryFileName.Empty();
					
					pcomp->m_csUniqueID.Empty();

					pcomp->m_nGraphicalType = 0;		

					pcomp->m_bDesignatorLocked = FALSE;

					pcomp->m_bPartLocked = FALSE;

					pcomp->m_bColorLocked = FALSE;

					pcomp->m_nColor = cEDA_schcomp.m_nColor;
					pcomp->m_nFillColor = cEDA_schcomp.m_nFillColor;
					pcomp->m_nPinColor = cEDA_schcomp.m_nPinColor;

					POSITION posParent = m_listSchObject[nCurrentDiagram].AddTail(pcomp);

					i++;
					onelinestr = stringarray.GetAt(i);
					strcpy(s, onelinestr);

					int j = Divide_long(s,special);
					if(stricmp(special[0], "Designator") == 0)
					{
						COb_schtext* ptext = new COb_schtext();
						ptext->pDocument = this;
						ptext->m_nDiagram = nCurrentDiagram;	

						ptext->m_nID = TEXT_COMPDESIGNATOR;
						ptext->m_csNameComment = "Designator";
						ptext->m_bNameLocked = TRUE;
						
						ptext->m_fSx = atoi(special[1])*10.0f;

						ptext->m_fSy = atoi(special[2])*10.0f;

						ptext->m_nOrientation = atoi(special[3])*90;
						ptext->m_nOrientation = Get_Correct_Orientation(ptext->m_nOrientation);

						ptext->m_nColor = atoi(special[4]);

						ptext->m_nFont = atoi(special[5]) - 1;

						ptext->m_bTextVisible = (atoi(special[7])+1)&0x01;

						ptext->m_csText = (char*)special[8];
						ptext->m_csText.TrimLeft(0x27);
						ptext->m_csText.TrimRight(0x27);

						ptext->m_bAutoPosition = FALSE;

						ptext->m_posParent = posParent;

						POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptext);

						pcomp->m_arrayPos.Add(pos);
					}

					i++;
					onelinestr = stringarray.GetAt(i);
					strcpy(s, onelinestr);

					j = Divide_long(s,special);
					if(stricmp(special[0], "PartType") == 0)
					{						
						COb_schtext* ptext = new COb_schtext();
						ptext->pDocument = this;
						ptext->m_nDiagram = nCurrentDiagram;	

						ptext->m_nID = TEXT_COMPCOMMENT;
						ptext->m_csNameComment = "Comment";
						ptext->m_bNameLocked = TRUE;
						
						ptext->m_fSx = atoi(special[1])*10.0f;
						ptext->m_fSy = atoi(special[2])*10.0f;

						ptext->m_nOrientation = atoi(special[3])*90;
						ptext->m_nOrientation = Get_Correct_Orientation(ptext->m_nOrientation);

						ptext->m_nColor = atoi(special[4]);

						ptext->m_nFont = atoi(special[5]) - 1;

						ptext->m_bTextVisible = (atoi(special[7])+1)&0x01;

						ptext->m_csText = (char*)special[8];
						ptext->m_csText.TrimLeft(0x27);
						ptext->m_csText.TrimRight(0x27);

						ptext->m_bAutoPosition = FALSE;

						ptext->m_posParent = posParent;

						POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptext);

						pcomp->m_arrayPos.Add(pos);
					}

					int line_pos = lib_pos;
					for(int k=0; k<lib_num; k++)
					{
						if(k == library_component_id) break;
						for(;;)
						{
							line_pos++;
							onelinestr = stringarray.GetAt(line_pos);
							strcpy(s, onelinestr);

							j = Divide_long(s,special);
							if(stricmp(special[0], "EndComponent") == 0) break;
						}
					}

					for(;;)
					{
						line_pos++;
						onelinestr = stringarray.GetAt(line_pos);
						strcpy(s, onelinestr);

						j = Divide_long(s,special);
						if(stricmp(special[0], "Component") == 0)
						{
							line_pos++;
							onelinestr = stringarray.GetAt(line_pos);
							strcpy(s, onelinestr);							
							Divide_long(s,special);

							pcomp->m_nPartCount = atoi(special[0]);

							line_pos++;

							line_pos++;

							line_pos++;
							onelinestr = stringarray.GetAt(line_pos);
							onelinestr.TrimLeft();
							onelinestr.TrimRight();

							pcomp->m_csDescription = onelinestr;

							line_pos++;
							onelinestr = stringarray.GetAt(line_pos);
							onelinestr.TrimLeft();
							onelinestr.TrimRight();

							COb_schtext* ptext = new COb_schtext();
							ptext->pDocument = this;
							ptext->m_nDiagram = nCurrentDiagram;	

							ptext->m_nID = TEXT_COMPPACKAGE;

							ptext->m_csNameComment = "Package";

							ptext->m_bNameVisible = FALSE;
											
							ptext->m_fSx = pcomp->m_fX;

							ptext->m_fSy = pcomp->m_fY;	

							ptext->m_nOrientation = 0;

							ptext->m_nFont = 0;

							ptext->m_nColor = 0;

							ptext->m_csText = onelinestr;

							ptext->m_bTextVisible = FALSE;

							ptext->m_bAutoPosition = FALSE;

							ptext->m_posParent = posParent;
					
							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptext);

							pcomp->m_arrayPos.Add(pos);

							line_pos++;

							line_pos++;

							line_pos++;

							line_pos++;

							line_pos++;

							line_pos++;

							line_pos++;

							line_pos++;

							line_pos++;

							line_pos++;

							line_pos++;

							line_pos++;

							line_pos++;

							line_pos++;


							for(int ownerpart_id = 0; ownerpart_id <pcomp->m_nPartCount; ownerpart_id++)
							{
								for(;;)
								{
									line_pos++;
									onelinestr = stringarray.GetAt(line_pos);
									strcpy(s, onelinestr);

									j = Divide_long(s,special);
									if(stricmp(special[0], "Part") == 0) break;
								}

								for(;;)
								{
									line_pos++;
									onelinestr = stringarray.GetAt(line_pos);
									strcpy(s, onelinestr);
									j = Divide_long(s,special);

									if(stricmp(special[0], "EndNormalPart") == 0) break;
									else if(stricmp(special[0], "Pin") == 0)
									{
											COb_schpin* ppin = new COb_schpin();
											ppin->pDocument = this;
											ppin->m_nDiagram = nCurrentDiagram;

											ppin->m_nOutsideEdge = atoi(special[1]);

											ppin->m_nInsideEdge = atoi(special[2])*3;

											ppin->m_nElectricalType = atoi(special[3]);

											ppin->m_bHide = atoi(special[4]);

											ppin->m_bNameVisible = atoi(special[5]);

											ppin->m_bNumberVisible = atoi(special[6]);

											ppin->m_fPinLength = atoi(special[7])*10.0f;

											ppin->m_fX = atoi(special[8])*10.0f;

											ppin->m_fY = atoi(special[9])*10.0f;
											double fx, fy;
											Get_Rotate_XY(&fx, &fy, ppin->m_fX,  ppin->m_fY, pcomp->m_nOrientation, pcomp->m_bMirrored);
											ppin->m_fX = fx + pcomp->m_fX; 
											ppin->m_fY = fy + pcomp->m_fY;

											ppin->m_nOrientation = atoi(special[10])*90;
											int new_o;
											Get_Rotate_Orientation(&new_o, ppin->m_nOrientation, pcomp->m_nOrientation, pcomp->m_bMirrored);
											ppin->m_nOrientation = new_o;
											ppin->m_nOrientation = Get_Correct_Orientation(ppin->m_nOrientation);

											ppin->m_csName = (char*)special[12];
											ppin->m_csName.TrimLeft(0x27);
											ppin->m_csName.TrimRight(0x27);

											ppin->m_csNumber = (char*)special[13];
											ppin->m_csNumber.TrimLeft(0x27);
											ppin->m_csNumber.TrimRight(0x27);								
											
											ppin->m_nColor = atoi(special[11]);
											ppin->m_bPinLocked = TRUE;

											ppin->m_nOwnerPartID = ownerpart_id+1;

											ppin->m_nDisplayMode = 0;

											ppin->m_nInside = 0;

											ppin->m_nOutside = 0;									

											ppin->m_csDescription.Empty();

											ppin->m_csConnectTo = ppin->m_csName;
											ppin->m_csUniqueID.Empty();
											ppin->m_posParent = posParent;
							
											POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ppin);

											pcomp->m_arrayPos.Add(pos);
									}
									else if(stricmp(special[0], "Rectangle") == 0)
									{
										COb_schrect* prect = new COb_schrect();
										prect->pDocument = this;
										prect->m_nDiagram = nCurrentDiagram;	

										prect->m_fX1 = atoi(special[1])*10.0f;

										prect->m_fY1 = atoi(special[2])*10.0f;

										double fx, fy;
										Get_Rotate_XY(&fx, &fy, prect->m_fX1, prect->m_fY1,  pcomp->m_nOrientation, pcomp->m_bMirrored);
										prect->m_fX1 = fx + pcomp->m_fX; 
										prect->m_fY1 = fy + pcomp->m_fY;

										prect->m_fX2 = atoi(special[3])*10.0f;

										prect->m_fY2 = atoi(special[4])*10.0f;

										Get_Rotate_XY(&fx, &fy, prect->m_fX2, prect->m_fY2,  pcomp->m_nOrientation, pcomp->m_bMirrored);
										prect->m_fX2 = fx + pcomp->m_fX; 
										prect->m_fY2 = fy + pcomp->m_fY;									

										prect->m_nBorderWidth = atoi(special[5]);
										
										prect->m_nBorderColor = atoi(special[6]);
										prect->m_nFillColor = atoi(special[7]);
										
										prect->m_bFillSolid = atoi(special[9]);

										prect->m_bTransparent = FALSE;

										prect->m_nOwnerPartID = ownerpart_id+1;

										prect->m_nDisplayMode = 0;

										prect->m_posParent = posParent;

										POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(prect);

										pcomp->m_arrayPos.Add(pos);
									}
									else if(stricmp(special[0], "Polyline") == 0)
									{
										COb_schpolyline* ppolyline = new COb_schpolyline();
										ppolyline->pDocument = this;
										ppolyline->m_nDiagram = nCurrentDiagram;	

										ppolyline->m_nLineWidth = atoi(special[1]);

										ppolyline->m_nLineType = atoi(special[2]);

										ppolyline->m_nColor = atoi(special[3]);

										int vertex = atoi(special[5]);
										ppolyline->m_cVertex.SetSize(vertex);
										for(int k=0; k<vertex; k++)
										{
											ppolyline->m_cVertex[k].fx = atoi(special[6+2*k])*10.0f;
											ppolyline->m_cVertex[k].fy = atoi(special[6+2*k+1])*10.0f;
											double fx, fy;
											Get_Rotate_XY(&fx, &fy, ppolyline->m_cVertex[k].fx, ppolyline->m_cVertex[k].fy,  pcomp->m_nOrientation, pcomp->m_bMirrored);
											ppolyline->m_cVertex[k].fx = fx + pcomp->m_fX; 
											ppolyline->m_cVertex[k].fy = fy + pcomp->m_fY;
										}

										ppolyline->m_nStartLineShape = 0;

										ppolyline->m_nEndLineShape = 0;

										ppolyline->m_nLineShapeSize = 0;

										ppolyline->m_nOwnerPartID = ownerpart_id+1;

										ppolyline->m_nDisplayMode = 0;
											
										ppolyline->m_posParent = posParent;

										POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ppolyline);

										pcomp->m_arrayPos.Add(pos);
									}
									else if(stricmp(special[0], "Bezier") == 0)
									{
										COb_schbezier* pbezier = new COb_schbezier();
										pbezier->pDocument = this;
										pbezier->m_nDiagram = nCurrentDiagram;	

										pbezier->m_nWidth = atoi(special[1]);

										pbezier->m_nColor = atoi(special[2]);

										int vertex = 0;
										vertex = atoi(special[4]);
										pbezier->m_cVertex.SetSize(vertex);
										for(int k=0; k < vertex; k++)
										{
											pbezier->m_cVertex[k].fx = atoi(special[5+2*k])*10.0f;
											pbezier->m_cVertex[k].fy = atoi(special[5+2*k+1])*10.0f;
											double fx, fy;
											Get_Rotate_XY(&fx, &fy, pbezier->m_cVertex[k].fx, pbezier->m_cVertex[k].fy,  pcomp->m_nOrientation, pcomp->m_bMirrored);
											pbezier->m_cVertex[k].fx = fx + pcomp->m_fX; 
											pbezier->m_cVertex[k].fy = fy + pcomp->m_fY;
										}
										
										pbezier->m_nOwnerPartID = ownerpart_id+1;

										pbezier->m_nDisplayMode = 0;										
										
										pbezier->m_posParent = posParent;

										POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pbezier);

										pcomp->m_arrayPos.Add(pos);
									}
									else if(stricmp(special[0], "Arc") == 0)
									{
										COb_scharc* parc = new COb_scharc();
										parc->pDocument = this;
										parc->m_nDiagram = nCurrentDiagram;	

										parc->m_fCx = atoi(special[1])*10.0f;
										parc->m_fCy = atoi(special[2])*10.0f;
										double fx, fy;
										Get_Rotate_XY(&fx, &fy, parc->m_fCx, parc->m_fCy,  pcomp->m_nOrientation, pcomp->m_bMirrored);
										parc->m_fCx = fx + pcomp->m_fX; 
										parc->m_fCy = fy + pcomp->m_fY;

										parc->m_fRadius = atoi(special[3])*10.0f;

										parc->m_nWidth = atoi(special[4]);

										parc->m_fSangle = atof(special[5]);
										parc->m_fEangle = atof(special[6]);
										if(parc->m_fSangle == parc->m_fEangle)
										{
											parc->m_fSangle = 0;
											parc->m_fEangle = 360;
										}
										double sangle, eangle;
										Get_Rotate_Angle(&sangle, &eangle, parc->m_fSangle, parc->m_fEangle, pcomp->m_nOrientation, pcomp->m_bMirrored);

										sangle = Get_Correct_Angle(sangle);
										eangle = Get_Correct_Angle(eangle);
										parc->m_fSangle = sangle;
										parc->m_fEangle = eangle;


										parc->m_nColor = atoi(special[7]);

										parc->m_nOwnerPartID = ownerpart_id+1;

										parc->m_nDisplayMode = 0;												

										parc->m_posParent = posParent;

										POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(parc);

										pcomp->m_arrayPos.Add(pos);
									}
									else if(stricmp(special[0], "Pie") == 0)
									{
										COb_schpie* ppie = new COb_schpie();
										ppie->pDocument = this;
										ppie->m_nDiagram = nCurrentDiagram;	

										ppie->m_fCx = atoi(special[1])*10.0f;
										ppie->m_fCy = atoi(special[2])*10.0f;
										double fx, fy;
										Get_Rotate_XY(&fx, &fy, ppie->m_fCx, ppie->m_fCy,  pcomp->m_nOrientation, pcomp->m_bMirrored);
										ppie->m_fCx = fx + pcomp->m_fX; 
										ppie->m_fCy = fy + pcomp->m_fY;									
										
										ppie->m_fRadius = atoi(special[3])*10.0f;

										ppie->m_nBorderWidth = atoi(special[4]);

										ppie->m_fSangle = atof(special[5]);
										ppie->m_fEangle = atof(special[6]);
										if(ppie->m_fSangle == ppie->m_fEangle)
										{
											ppie->m_fSangle = 0;
											ppie->m_fEangle = 360;
										}
										double sangle, eangle;
										Get_Rotate_Angle(&sangle, &eangle, ppie->m_fSangle, ppie->m_fEangle, pcomp->m_nOrientation, pcomp->m_bMirrored);
										sangle = Get_Correct_Angle(sangle);
										eangle = Get_Correct_Angle(eangle);										
										ppie->m_fSangle = sangle;
										ppie->m_fEangle = eangle;

										ppie->m_nBorderColor = atoi(special[7]);
										ppie->m_nFillColor = atoi(special[8]);

										ppie->m_bFillSolid = atoi(special[9]);

										ppie->m_nOwnerPartID = ownerpart_id+1;

										ppie->m_nDisplayMode = 0;	

										ppie->m_posParent = posParent;

										POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ppie);

										pcomp->m_arrayPos.Add(pos);
									}
									else if(stricmp(special[0], "EllipticalArc") == 0)
									{
										COb_schellipse* pellipse = new COb_schellipse();
										pellipse->pDocument = this;
										pellipse->m_nDiagram = nCurrentDiagram;	
										
										pellipse->m_fCx = atoi(special[1])*10.0f;
										pellipse->m_fCy = atoi(special[2])*10.0f;
										double fx, fy;
										Get_Rotate_XY(&fx, &fy, pellipse->m_fCx, pellipse->m_fCy,  pcomp->m_nOrientation, pcomp->m_bMirrored);
										pellipse->m_fCx = fx + pcomp->m_fX; 
										pellipse->m_fCy = fy + pcomp->m_fY;	

										pellipse->m_fRadius_x = atoi(special[3])*10.0f;
										pellipse->m_fRadius_y = atoi(special[4])*10.0f;
										if((pcomp->m_nOrientation == 90) || (pcomp->m_nOrientation == 270))
										{
											double dtemp = pellipse->m_fRadius_x;
											pellipse->m_fRadius_x = pellipse->m_fRadius_y;
											pellipse->m_fRadius_y = dtemp;
										}

										pellipse->m_nWidth = atoi(special[5]);

										pellipse->m_fSangle = atof(special[6]);
										pellipse->m_fEangle = atof(special[7]);
										if(pellipse->m_fSangle == pellipse->m_fEangle)
										{
											pellipse->m_fSangle = 0;
											pellipse->m_fEangle = 360;
										}
										double sangle, eangle;
										Get_Rotate_Angle(&sangle, &eangle, pellipse->m_fSangle, pellipse->m_fEangle, pcomp->m_nOrientation, pcomp->m_bMirrored);
										sangle = Get_Correct_Angle(sangle);
										eangle = Get_Correct_Angle(eangle);
										pellipse->m_fSangle = sangle;
										pellipse->m_fEangle = eangle;

										pellipse->m_nColor = atoi(special[8]);

										pellipse->m_nOwnerPartID = ownerpart_id+1;

										pellipse->m_nDisplayMode = 0;											

										pellipse->m_posParent = posParent;

										POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pellipse);

										pcomp->m_arrayPos.Add(pos);
									}
									else if(stricmp(special[0], "Ellipse") == 0)
									{
										COb_schellipsefill* pellipsefill = new COb_schellipsefill();
										pellipsefill->pDocument = this;
										pellipsefill->m_nDiagram = nCurrentDiagram;	
												
										pellipsefill->m_fCx = atoi(special[1])*10.0f;

										pellipsefill->m_fCy = atoi(special[2])*10.0f;
										double fx, fy;
										Get_Rotate_XY(&fx, &fy, pellipsefill->m_fCx, pellipsefill->m_fCy,  pcomp->m_nOrientation, pcomp->m_bMirrored);
										pellipsefill->m_fCx = fx + pcomp->m_fX; 
										pellipsefill->m_fCy = fy + pcomp->m_fY;											

										pellipsefill->m_fRadius_x = atoi(special[3])*10.0f;
										pellipsefill->m_fRadius_y = atoi(special[4])*10.0f;
										if((pcomp->m_nOrientation == 90) || (pcomp->m_nOrientation == 270))
										{
											double dtemp = pellipsefill->m_fRadius_x;
											pellipsefill->m_fRadius_x = pellipsefill->m_fRadius_y;
											pellipsefill->m_fRadius_y = dtemp;
										}

										pellipsefill->m_nBorderWidth = atoi(special[5]);

										pellipsefill->m_nBorderColor = atoi(special[6]);

										pellipsefill->m_nFillColor = atoi(special[7]);

										pellipsefill->m_bFillSolid = atoi(special[8]);

										pellipsefill->m_bTransparent = FALSE;

										pellipsefill->m_nOwnerPartID = ownerpart_id+1;

										pellipsefill->m_nDisplayMode = 0;	

										pellipsefill->m_posParent = posParent;

										POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pellipsefill);

										pcomp->m_arrayPos.Add(pos);
									}
									else if(stricmp(special[0], "Polygon") == 0)
									{
										COb_schpolygon* ppolygon = new COb_schpolygon();
										ppolygon->pDocument = this;
										ppolygon->m_nDiagram = nCurrentDiagram;	
								
										ppolygon->m_nBorderWidth = atoi(special[1]);

										ppolygon->m_nBorderColor = atoi(special[2]);

										ppolygon->m_nFillColor = atoi(special[3]);

										ppolygon->m_bFillSolid = atoi(special[4]);

										ppolygon->m_bTransparent = FALSE;

										int vertex = atoi(special[6]);
										ppolygon->m_cVertex.SetSize(vertex);
										for(int k=0; k<vertex; k++)
										{
											ppolygon->m_cVertex[k].fx = atoi(special[7+2*k])*10.0f;
											ppolygon->m_cVertex[k].fy = atoi(special[7+2*k+1])*10.0f;
											double fx, fy;
											Get_Rotate_XY(&fx, &fy, ppolygon->m_cVertex[k].fx, ppolygon->m_cVertex[k].fy,  pcomp->m_nOrientation, pcomp->m_bMirrored);
											ppolygon->m_cVertex[k].fx = fx + pcomp->m_fX; 
											ppolygon->m_cVertex[k].fy = fy + pcomp->m_fY;
										}
										
										ppolygon->m_nOwnerPartID = ownerpart_id+1;

										ppolygon->m_nDisplayMode = 0;													

										ppolygon->m_posParent = posParent;

										POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ppolygon);

										pcomp->m_arrayPos.Add(pos);
									}
									else if(stricmp(special[0], "Line") == 0)
									{
										COb_schpolyline* ppolyline = new COb_schpolyline();
										ppolyline->pDocument = this;
										ppolyline->m_nDiagram = nCurrentDiagram;	
										
										int vertex = 2;
										ppolyline->m_cVertex.SetSize(vertex);
										for(int k=0; k<vertex; k++)
										{
											ppolyline->m_cVertex[k].fx = atoi(special[1+2*k])*10.0f;
											ppolyline->m_cVertex[k].fy = atoi(special[1+2*k+1])*10.0f;
											double fx, fy;
											Get_Rotate_XY(&fx, &fy, ppolyline->m_cVertex[k].fx, ppolyline->m_cVertex[k].fy,  pcomp->m_nOrientation, pcomp->m_bMirrored);
											ppolyline->m_cVertex[k].fx = fx + pcomp->m_fX; 
											ppolyline->m_cVertex[k].fy = fy + pcomp->m_fY;
										}

										ppolyline->m_nLineWidth = atoi(special[5]);

										ppolyline->m_nLineType = atoi(special[6]);

										ppolyline->m_nColor = atoi(special[7]);

										ppolyline->m_nStartLineShape = 0;

										ppolyline->m_nEndLineShape = 0;

										ppolyline->m_nLineShapeSize = 0;
											
										ppolyline->m_nOwnerPartID = ownerpart_id+1;

										ppolyline->m_nDisplayMode = 0;			
										
										ppolyline->m_posParent = posParent;

										POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ppolyline);

										pcomp->m_arrayPos.Add(pos);

									}
									else if(stricmp(special[0], "Label") == 0)
									{
										COb_schtext* ptext = new COb_schtext();
										ptext->pDocument = this;
										ptext->m_nDiagram = nCurrentDiagram;	

										ptext->m_nID = TEXT_TEXT;
										ptext->m_csNameComment.Empty();

										ptext->m_fSx = atoi(special[1])*10.0f;

										ptext->m_fSy = atoi(special[2])*10.0f;
										double fx, fy;
										Get_Rotate_XY(&fx, &fy, ptext->m_fSx, ptext->m_fSy,  pcomp->m_nOrientation, pcomp->m_bMirrored);
										ptext->m_fSx = fx + pcomp->m_fX; 
										ptext->m_fSy = fy + pcomp->m_fY;

										ptext->m_nOrientation = atoi(special[3])*90;
										int new_o;

										Get_Rotate_Orientation(&new_o, ptext->m_nOrientation, pcomp->m_nOrientation, FALSE);
										ptext->m_nOrientation = new_o;
										ptext->m_nOrientation = Get_Correct_Orientation(ptext->m_nOrientation);

										ptext->m_nColor = atoi(special[4]);

										ptext->m_nFont = atoi(special[5]) - 1;

										ptext->m_csText = (char*)special[7];
										ptext->m_csText.TrimLeft(0x27);
										ptext->m_csText.TrimRight(0x27);

										ptext->m_bMirror = FALSE;

										ptext->m_nOwnerPartID = ownerpart_id+1;

										ptext->m_nDisplayMode = 0;	
										
										ptext->m_posParent = posParent;

										POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptext);
										
										pcomp->m_arrayPos.Add(pos);
									}
									else if(stricmp(special[0], "RoundRectangle") == 0)
									{
										COb_schroundrect* proundrect = new COb_schroundrect();
										proundrect->pDocument = this;
										proundrect->m_nDiagram = nCurrentDiagram;	
										
										proundrect->m_fX1 = atoi(special[1])*10.0f;

										proundrect->m_fY1 = atoi(special[2])*10.0f;
										double fx, fy;
										Get_Rotate_XY(&fx, &fy, proundrect->m_fX1, proundrect->m_fY1,  pcomp->m_nOrientation, pcomp->m_bMirrored);
										proundrect->m_fX1 = fx + pcomp->m_fX; 
										proundrect->m_fY1 = fy + pcomp->m_fY;

										proundrect->m_fX2 = atoi(special[3])*10.0f;

										proundrect->m_fY2 = atoi(special[4])*10.0f;
										Get_Rotate_XY(&fx, &fy, proundrect->m_fX2, proundrect->m_fY2,  pcomp->m_nOrientation, pcomp->m_bMirrored);
										proundrect->m_fX2 = fx + pcomp->m_fX; 
										proundrect->m_fY2 = fy + pcomp->m_fY;

										proundrect->m_fRadius_x = atoi(special[5])*10.0f;

										proundrect->m_fRadius_y = atoi(special[6])*10.0f;
										if((pcomp->m_nOrientation == 90) || (pcomp->m_nOrientation == 270))
										{
											double dtemp = proundrect->m_fRadius_x;
											proundrect->m_fRadius_x = proundrect->m_fRadius_y;
											proundrect->m_fRadius_y = dtemp;
										}											

										proundrect->m_nBorderWidth = atoi(special[7]);

										proundrect->m_nBorderColor = atoi(special[8]);
										proundrect->m_nFillColor = atoi(special[9]);

										proundrect->m_bFillSolid = atoi(special[11]);
										
										proundrect->m_nOwnerPartID = ownerpart_id+1;

										proundrect->m_nDisplayMode = 0;	

										proundrect->m_posParent = posParent;

										POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(proundrect);
										
										pcomp->m_arrayPos.Add(pos);
									}
									else if(stricmp(special[0], "Symbol") == 0)
									{
										COb_schieee* pieee = new COb_schieee();
										pieee->pDocument = this;
										pieee->m_nDiagram = nCurrentDiagram;

										pieee->m_nIeeeSymbol = atoi(special[1]);

										pieee->m_fX = atoi(special[2])*10.0f;

										pieee->m_fY = atoi(special[3])*10.0f;
										double fx, fy;
										Get_Rotate_XY(&fx, &fy, pieee->m_fX, pieee->m_fY,  pcomp->m_nOrientation, pcomp->m_bMirrored);
										pieee->m_fX = fx + pcomp->m_fX; 
										pieee->m_fY = fy + pcomp->m_fY;

										pieee->m_fSize = atoi(special[4]);

										pieee->m_nOrientation = atoi(special[5])*90;
										int new_o;
										Get_Rotate_Orientation(&new_o, pieee->m_nOrientation, pcomp->m_nOrientation, pcomp->m_bMirrored);
										pieee->m_nOrientation = new_o;
										pieee->m_nOrientation = Get_Correct_Orientation(pieee->m_nOrientation);

										pieee->m_nWidth = atoi(special[6]);

										pieee->m_nColor = atoi(special[7]);

										pieee->m_nOwnerPartID = ownerpart_id+1;

										pieee->m_nDisplayMode = 0;

										pieee->m_bMirrored = FALSE;

										pieee->m_posParent = posParent;

										POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pieee);

										pcomp->m_arrayPos.Add(pos);
									}
								}
							}
					
						}
						else if(stricmp(special[0], "EndComponent") == 0) break;
					}

					m_listSchObject[nCurrentDiagram].SetAt(posParent, pcomp);
				}
			}
			continue;
		}
	}
}

void CESDDoc::Open_ESD(CArchive& ar, int version)
{
	if((version == 1)||(version == 2))
	{
		for(int nCurrentDiagram=0; nCurrentDiagram<SCH_SHEET_NUM; nCurrentDiagram++)
		{
			int fontNum;
			ar >> fontNum;
			m_arrayFont[nCurrentDiagram].RemoveAll();
			for(int fontNo=0; fontNo<fontNum; fontNo++)
			{
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
				
				ar >> logfont.lfHeight;
				ar >> logfont.lfOrientation;
				ar >> logfont.lfUnderline;
				ar >> logfont.lfItalic;
				ar >> logfont.lfWeight;
				ar >> logfont.lfStrikeOut;
				CString csFaceName;
				ar >> csFaceName;
				strcpy(logfont.lfFaceName, csFaceName);
				m_arrayFont[nCurrentDiagram].Add(logfont);
			}
		}

		for(int nCurrentDiagram=0; nCurrentDiagram<SCH_SHEET_NUM; nCurrentDiagram++)
		{
			ar >> m_fSCH_ScreenX[nCurrentDiagram];			
			ar >> m_fSCH_ScreenY[nCurrentDiagram];			
			ar >> m_fSCH_Scale[nCurrentDiagram];			
			ar >> m_bSCH_SheetShow[nCurrentDiagram];		
			ar >> m_csSCH_SheetName[nCurrentDiagram];		
			m_pSCH_Sheet[nCurrentDiagram]->Serialize(ar);	
		}

		ar >> m_nSCH_Unit;
		ar >> m_bSCH_SnapGridEnable;
		ar >> m_fSCH_SnapGrid;
		ar >> m_bSCH_VisibleGridShow;
		ar >> m_fSCH_VisibleGrid;
		ar >> m_bSCH_ElecGridEnable;
		ar >> m_fSCH_ElecGrid;

		for(int nCurrentDiagram=0; nCurrentDiagram<SCH_SHEET_NUM; nCurrentDiagram++)
		{
			m_listSchObject[nCurrentDiagram].RemoveAll();
			int num;
			ar >> num;
			for(int i=0; i<num; i++)
			{	
				int elementNo;
				ar >> elementNo;

				if(elementNo == SCH_ELEMENT_LINE)
				{
					COb_schline* pline = new COb_schline();
					pline->pDocument = this;
					pline->m_nDiagram = nCurrentDiagram;
					pline->Serialize(ar);

					m_listSchObject[nCurrentDiagram].AddTail(pline);				
				}
				else if(elementNo == SCH_ELEMENT_POLYLINE)
				{
					COb_schpolyline* ppolyline = new COb_schpolyline();
					ppolyline->pDocument = this;
					ppolyline->m_nDiagram = nCurrentDiagram;
					ppolyline->Serialize(ar);

					m_listSchObject[nCurrentDiagram].AddTail(ppolyline);
		
				}
				else if(elementNo == SCH_ELEMENT_POLYGON)
				{
					COb_schpolygon* ppolygon = new COb_schpolygon();
					ppolygon->pDocument = this;
					ppolygon->m_nDiagram = nCurrentDiagram;	
					ppolygon->Serialize(ar);

					m_listSchObject[nCurrentDiagram].AddTail(ppolygon);
				}
				else if(elementNo == SCH_ELEMENT_JUNCTION)
				{
					COb_schjunc* pjunc = new COb_schjunc();
					pjunc->pDocument = this;
					pjunc->m_nDiagram = nCurrentDiagram;
					pjunc->Serialize(ar);

					m_listSchObject[nCurrentDiagram].AddTail(pjunc);
				}
				else if(elementNo == SCH_ELEMENT_ELLIPSEFILL)
				{
					COb_schellipsefill* pellipsefill = new COb_schellipsefill();
					pellipsefill->pDocument = this;
					pellipsefill->m_nDiagram = nCurrentDiagram;
					pellipsefill->Serialize(ar);

					POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pellipsefill);
				}
				else if(elementNo == SCH_ELEMENT_ELLIPSE)
				{
					COb_schellipse* pellipse = new COb_schellipse();
					pellipse->pDocument = this;
					pellipse->m_nDiagram = nCurrentDiagram;	
					pellipse->Serialize(ar);
						
					m_listSchObject[nCurrentDiagram].AddTail(pellipse);
				}
				else if(elementNo == SCH_ELEMENT_ARC)
				{
					COb_scharc* parc = new COb_scharc();
					parc->pDocument = this;
					parc->m_nDiagram = nCurrentDiagram;	
					parc->Serialize(ar);

					m_listSchObject[nCurrentDiagram].AddTail(parc);
				}
				else if(elementNo == SCH_ELEMENT_PIE)
				{
					COb_schpie* ppie = new COb_schpie();
					ppie->pDocument = this;
					ppie->m_nDiagram = nCurrentDiagram;	
					ppie->Serialize(ar);

					m_listSchObject[nCurrentDiagram].AddTail(ppie);
				}
				else if(elementNo == SCH_ELEMENT_RECT)
				{
					COb_schrect* prect = new COb_schrect();
					prect->pDocument = this;
					prect->m_nDiagram = nCurrentDiagram;
					prect->Serialize(ar);

					m_listSchObject[nCurrentDiagram].AddTail(prect);
				}
				else if(elementNo == SCH_ELEMENT_ROUNDRECT)
				{
					COb_schroundrect* proundrect = new COb_schroundrect();
					proundrect->pDocument = this;
					proundrect->m_nDiagram = nCurrentDiagram;
					proundrect->Serialize(ar);

					m_listSchObject[nCurrentDiagram].AddTail(proundrect);
				}
				else if(elementNo == SCH_ELEMENT_PORT)
				{
					COb_schport* pport = new COb_schport();
					pport->pDocument = this;
					pport->m_nDiagram = nCurrentDiagram;
					pport->Serialize(ar);

					m_listSchObject[nCurrentDiagram].AddTail(pport);
				}		
				else if(elementNo == SCH_ELEMENT_POWER)
				{
					COb_schpower* ppower = new COb_schpower();
					ppower->pDocument = this;
					ppower->m_nDiagram = nCurrentDiagram;
					ppower->Serialize(ar);

					m_listSchObject[nCurrentDiagram].AddTail(ppower);
				}	
				else if(elementNo == SCH_ELEMENT_BEZIER)
				{
					COb_schbezier* pbezier = new COb_schbezier();
					pbezier->pDocument = this;
					pbezier->m_nDiagram = nCurrentDiagram;	
					pbezier->Serialize(ar);
		
					m_listSchObject[nCurrentDiagram].AddTail(pbezier);
				}	
				else if(elementNo == SCH_ELEMENT_GRAPHIC)
				{
					COb_schgraphic* pgraphic = new COb_schgraphic();
					pgraphic->pDocument = this;
					pgraphic->m_nDiagram = nCurrentDiagram;	
					pgraphic->Serialize(ar);

					pgraphic->LoadBitmap();

					m_listSchObject[nCurrentDiagram].AddTail(pgraphic);
				}	
				else if(elementNo == SCH_ELEMENT_TEXTFRAME)
				{
					COb_schtextframe* ptextframe = new COb_schtextframe();
					ptextframe->pDocument = this;
					ptextframe->m_nDiagram = nCurrentDiagram;
					ptextframe->Serialize(ar);

					m_listSchObject[nCurrentDiagram].AddTail(ptextframe);
				}
				else if(elementNo == SCH_ELEMENT_TEXT)
				{
					COb_schtext* ptext = new COb_schtext();
					ptext->pDocument = this;
					ptext->m_nDiagram = nCurrentDiagram;
					ptext->Serialize(ar);

					m_listSchObject[nCurrentDiagram].AddTail(ptext);
				}
				else if(elementNo == SCH_ELEMENT_SHEETSYMBOL)
				{
					COb_schsheetsymbol* psheetsymbol = new COb_schsheetsymbol();
					psheetsymbol->pDocument = this;
					psheetsymbol->m_nDiagram = nCurrentDiagram;	
					psheetsymbol->Serialize(ar);

					POSITION posParent = m_listSchObject[nCurrentDiagram].AddTail(psheetsymbol);

					int subelementNum;
					ar >> subelementNum;
					for(int k=0; k<subelementNum; k++)
					{
						int subelementNo;
						ar >> subelementNo;
						if(subelementNo == SCH_ELEMENT_SHEETENTRY)
						{
							COb_schsheetentry* psheetentry = new COb_schsheetentry();
							psheetentry->pDocument = this;
							psheetentry->m_nDiagram = nCurrentDiagram;	
							psheetentry->Serialize(ar);

							psheetentry->m_posParent = posParent;
							
							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(psheetentry);

							psheetsymbol->m_arrayPos.Add(pos);
						}
						else if(subelementNo == SCH_ELEMENT_TEXT)
						{
							COb_schtext* ptext = new COb_schtext();
							ptext->pDocument = this;
							ptext->m_nDiagram = nCurrentDiagram;
							ptext->Serialize(ar);

							ptext->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptext);

							psheetsymbol->m_arrayPos.Add(pos);
						}
					}

					i += subelementNum;
				}
				else if(elementNo == SCH_ELEMENT_COMPONENT)
				{
					COb_schcomp* pcomp = new COb_schcomp();
					pcomp->pDocument = this;
					pcomp->m_nDiagram = nCurrentDiagram;
					pcomp->Serialize(ar);

					POSITION posParent = m_listSchObject[nCurrentDiagram].AddTail(pcomp);
				
					int subelementNum;
					ar >> subelementNum;
					for(int k=0; k<subelementNum; k++)
					{
						ar >> elementNo;
						if(elementNo == SCH_ELEMENT_PIN)
						{
							COb_schpin* ppin = new COb_schpin();
							ppin->pDocument = this;
							ppin->m_nDiagram = nCurrentDiagram;
							ppin->Serialize(ar);

							ppin->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ppin);
				
							pcomp->m_arrayPos.Add(pos);
						}
						else if(elementNo == SCH_ELEMENT_IEEE)
						{
							COb_schieee* pieee = new COb_schieee();
							pieee->pDocument = this;
							pieee->m_nDiagram = nCurrentDiagram;
							pieee->Serialize(ar);

							pieee->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pieee);

							pcomp->m_arrayPos.Add(pos);
						}
						else if(elementNo == SCH_ELEMENT_ARC)
						{
							COb_scharc* parc = new COb_scharc();
							parc->pDocument = this;
							parc->m_nDiagram = nCurrentDiagram;	
							parc->Serialize(ar);

							parc->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(parc);

							if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
						}
						else if(elementNo == SCH_ELEMENT_ELLIPSE)
						{
							COb_schellipse* pellipse = new COb_schellipse();
							pellipse->pDocument = this;
							pellipse->m_nDiagram = nCurrentDiagram;	
							pellipse->Serialize(ar);

							pellipse->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pellipse);

							if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
						}
						else if(elementNo == SCH_ELEMENT_ELLIPSEFILL)
						{
							COb_schellipsefill* pellipsefill = new COb_schellipsefill();
							pellipsefill->pDocument = this;
							pellipsefill->m_nDiagram = nCurrentDiagram;
							pellipsefill->Serialize(ar);

							pellipsefill->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pellipsefill);

							if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
						}
						else if(elementNo == SCH_ELEMENT_PIE)
						{
							COb_schpie* ppie = new COb_schpie();
							ppie->pDocument = this;
							ppie->m_nDiagram = nCurrentDiagram;	
							ppie->Serialize(ar);

							ppie->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ppie);

							if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
						}
						else if(elementNo == SCH_ELEMENT_POLYLINE)
						{
							COb_schpolyline* ppolyline = new COb_schpolyline();
							ppolyline->pDocument = this;
							ppolyline->m_nDiagram = nCurrentDiagram;
							ppolyline->Serialize(ar);

							ppolyline->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ppolyline);

							if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
						}
						else if(elementNo == SCH_ELEMENT_RECT)
						{
							COb_schrect* prect = new COb_schrect();
							prect->pDocument = this;
							prect->m_nDiagram = nCurrentDiagram;
							prect->Serialize(ar);

							prect->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(prect);

							if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
						}
						else if(elementNo == SCH_ELEMENT_ROUNDRECT)
						{
							COb_schroundrect* proundrect = new COb_schroundrect();
							proundrect->pDocument = this;
							proundrect->m_nDiagram = nCurrentDiagram;
							proundrect->Serialize(ar);

							proundrect->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(proundrect);

							if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
						}
						else if(elementNo == SCH_ELEMENT_POLYGON)
						{
							COb_schpolygon* ppolygon = new COb_schpolygon();
							ppolygon->pDocument = this;
							ppolygon->m_nDiagram = nCurrentDiagram;	
							ppolygon->Serialize(ar);

							ppolygon->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ppolygon);

							if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
						}
						else if(elementNo == SCH_ELEMENT_BEZIER)
						{
							COb_schbezier* pbezier = new COb_schbezier();
							pbezier->pDocument = this;
							pbezier->m_nDiagram = nCurrentDiagram;	
							pbezier->Serialize(ar);

							pbezier->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pbezier);

							if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
						}
						else if(elementNo == SCH_ELEMENT_TEXTFRAME)
						{
							COb_schtextframe* ptextframe = new COb_schtextframe();
							ptextframe->pDocument = this;
							ptextframe->m_nDiagram = nCurrentDiagram;
							ptextframe->Serialize(ar);

							ptextframe->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptextframe);

							if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
						}		
						else if(elementNo == SCH_ELEMENT_GRAPHIC)
						{
							COb_schgraphic* pgraphic = new COb_schgraphic();
							pgraphic->pDocument = this;
							pgraphic->m_nDiagram = nCurrentDiagram;	
							pgraphic->Serialize(ar);

							pgraphic->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pgraphic);

							if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
						}					
						else if(elementNo == SCH_ELEMENT_TEXT)
						{
							COb_schtext* ptext = new COb_schtext();
							ptext->pDocument = this;
							ptext->m_nDiagram = nCurrentDiagram;
							ptext->Serialize(ar);

							ptext->m_posParent = posParent;

							POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptext);

							pcomp->m_arrayPos.Add(pos);
						}
					}
					i += subelementNum;
				}
			}
		}
		
		m_arrayCompDesignator.RemoveAll();
		m_arrayCompComment.RemoveAll();
		m_arrayCompPackage.RemoveAll();
		m_arrayPackageLibraryName.RemoveAll();
		int num;
		ar >> num;
		for(int i=0; i<num; i++)
		{
			CString cstring;
			ar >> cstring;
			m_arrayCompDesignator.Add(cstring);
			ar >> cstring;
			m_arrayCompComment.Add(cstring);		
			ar >> cstring;
			m_arrayCompPackage.Add(cstring);
			ar >> cstring;
			m_arrayPackageLibraryName.Add(cstring);	
		}

		ar >> num;
		for(int i=0; i<num; i++) 
		{
			COb_schnet* pschnet = new COb_schnet();
			m_arraySchNet.Add(pschnet);
			pschnet->Serialize(ar);
		}
	}

	if(version == 2)
	{
		for(int i=0; i<PCB_LAYER_NUM; i++)
		{
			ar >> m_sPCB_LayerParam[i].csLayerName;
			ar >> m_sPCB_LayerParam[i].nLayerPrev;
			ar >> m_sPCB_LayerParam[i].nLayerNext;
			ar >> m_sPCB_LayerParam[i].bLayerMechEnabled;
			ar >> m_sPCB_LayerParam[i].bLayerSheetLinked;
			ar >> m_sPCB_LayerParam[i].csLayerCopThick;
			ar >> m_sPCB_LayerParam[i].csLayerDielType;
			ar >> m_sPCB_LayerParam[i].csLayerDielConst;
			ar >> m_sPCB_LayerParam[i].csLayerDielHeight;
			ar >> m_sPCB_LayerParam[i].csLayerDielMaterial;
			
			ar >> m_sPCB_LayerParam[i].bLayerShow;
		}

		m_cPCB_Sheet.Serialize(ar);

		int num;
		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbnet* pnet = new COb_pcbnet();
			pnet->Serialize(ar);
			pnet->pDocument = this;

			m_arrayPcbNet.Add(pnet);
		}
		
		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbcomp* pcomp = new COb_pcbcomp();
			pcomp->Serialize(ar);
			pcomp->pDocument = this;

			m_arrayPcbComp.Add(pcomp);
		}
		
		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbpolygon* ppolygon = new COb_pcbpolygon();
			ppolygon->Serialize(ar);
			ppolygon->pDocument = this;

			m_arrayPcbPolygon.Add(ppolygon);
		}
		
		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbconnection* pconnection = new COb_pcbconnection();
			pconnection->Serialize(ar);

			m_arrayPcbConnection.Add(pconnection);
		}

		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbtrack* ptrack = new COb_pcbtrack();
			ptrack->Serialize(ar);
			ptrack->pDocument = this;

			m_arrayPcbTrack.Add(ptrack);
		}

		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbarc* parc = new COb_pcbarc();
			parc->Serialize(ar);
			parc->pDocument = this;

			m_arrayPcbArc.Add(parc);
		}

		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbfill* pfill = new COb_pcbfill();
			pfill->Serialize(ar);
			pfill->pDocument = this;

			m_arrayPcbFill.Add(pfill);
		}

		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbtext* ptext = new COb_pcbtext();
			ptext->Serialize(ar);
			ptext->pDocument = this;

			m_arrayPcbText.Add(ptext);
		}

		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbpad* ppad = new COb_pcbpad();
			ppad->Serialize(ar);
			ppad->pDocument = this;

			m_arrayPcbPad.Add(ppad);
		}

		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbvia* pvia = new COb_pcbvia();
			pvia->Serialize(ar);
			pvia->pDocument = this;

			m_arrayPcbVia.Add(pvia);
		}

		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbregion* pregion = new COb_pcbregion();
			pregion->Serialize(ar);
			pregion->pDocument = this;

			m_arrayPcbRegion.Add(pregion);
		}

		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbcoordinate* pcoordinate = new COb_pcbcoordinate();
			pcoordinate->Serialize(ar);
			pcoordinate->pDocument = this;

			m_arrayPcbCoordinate.Add(pcoordinate);
		}

		ar >> num;
		for(int i=0; i<num; i++)
		{
			COb_pcbdimension* pdimension = new COb_pcbdimension();
			pdimension->Serialize(ar);
			pdimension->pDocument = this;

			m_arrayPcbDimension.Add(pdimension);
		}
		InitLayerShowArray();
	}
}

void CESDDoc::Open_Eda_V1(CArchive& ar)
{
	struct Line
	{  
		unsigned char  id;
		unsigned short startx,starty,endx,endy;
		unsigned char  type, layer, linewidth, attrib, u1,u2,flag;  

	};

	struct Junction
	{  
		unsigned char   id;
		unsigned short  centerx,centery;
		unsigned char   Dx, Dy, d;
		unsigned char   padtype;
		unsigned char   attrib, layer, padname[4], flag; 
	};

	struct String
	{  
		unsigned char   id;
		unsigned short  sx, sy, height;
		unsigned char   align, layer, attrib, flag;
		char   text[33];
		char   fontname[16];

		unsigned char   width,fonttype,u3,u4;      
	};

	struct Arc
	{  
		unsigned char   id;
		unsigned short  centerx, centery, radius;
		short  stangle, endangle;
		unsigned char   linewidth, attrib, u1, layer, flag;     
	};

	struct Vertex
	{ 
		unsigned short x, y;      
	};

	struct Fill
	{ 
		unsigned char   id;
		unsigned short  vertexnum, fillpattern, total16byte;
		unsigned char   u1, u2, type, layer, u3, attrib, u4, u5, flag;
		struct Vertex   pline[100]; 
	};

	struct Comphead
	{
		char   name[26];
		unsigned int   offset;
		short   length;
	};

	struct Comp
	{
	short x;
	short y;
	short pinnum;
	unsigned char num;
	unsigned char lnum;
	unsigned char jnum;
	unsigned char snum;
	unsigned char anum;
	unsigned char fnum;
	char ref[3];
	unsigned char type;
	char package1[16];
	char package2[16];
	char package3[16];   
	};

	struct Pin
	{
		unsigned char type;
		unsigned char copy, num[4], name[16];
		unsigned short xs,ys,xe,ye;
		unsigned char p1,p2;
	};

	struct Component           
	{  
		unsigned char  id;
		unsigned short cx,cy;
		short  angle;
		short  total16byte;     
		unsigned char  type, layer, u1, attrib, u2, u3, flag;
		unsigned short IDx,IDy,IDsize;
		unsigned char  IDstate;
		char  ID[17];
		unsigned short Px,Py,Psize;
		unsigned char  Pstate;
		char  P[16];
		unsigned short Cx,Cy,Csize;
		unsigned char  Cstate;
		char  C[26];
		short x, y, pinnum;
		unsigned char  num, lnum, jnum, snum, anum, fnum;
		char  ref[3];
		unsigned char  comptype ; 
	};

	struct Packagehead
	{
		short u1;
		unsigned char   namelength;
		char   name[13];
		short   u2;
		short   objectnum;
		short   offset;
		short   u[5];
	};

	struct Package
	{  
		unsigned char  id;
		unsigned short  cx,cy;
		short  angle;
		short  total16byte;      
		unsigned char  type, layer, u1, attrib, IDwidth, Cwidth, flag;
		unsigned short IDx,IDy,IDsize;
		unsigned char  IDstate;

		char  ID[17];
		unsigned short Px,Py;
		unsigned char  IDalign, Calign;
		unsigned char  Pstate;
		char  P[16];
		unsigned short Cx,Cy,Csize;
		unsigned char  Cstate;

		char  C[26];
		unsigned short x, y, padnum;
		unsigned char  num, lnum, jnum, snum, anum, fnum;
		char  ref[3];
		unsigned char  comptype ; 			
	};

	struct DBox
	{	unsigned char id;
		unsigned short x1,y1,x2,y2, total16byte;
		unsigned char layer, attrib, border, fonttype, flag;
		unsigned short height, width, d, ld, u3, u4, u5, u6; 
	};
	struct  Line gnline;
	struct  Junction gnjunction;
	struct  Component gncomponent;
	struct  Package gnpackage;
	struct  String gnstring;
	struct  Arc gnarc;
	struct  Fill gnfill;
	struct  Pin gnpin;
	struct DBox dbox;

	CFile* pFile = ar.GetFile();
	unsigned char tempmem[20000];

		char s1[100], s[100];
		CString s2;
		int i;
		short int l;

	char m_szPower_Plane[40];
	char m_szGround_Plane[40];
	unsigned short m_nPCB_Limmin_X;
	unsigned short m_nPCB_Limmin_Y;
	unsigned short m_nPCB_Limmax_X;
	unsigned short m_nPCB_Limmax_Y;
	unsigned short m_nSCH_Limmax_Y[20];
	unsigned short m_nSCH_Limmax_X[20];
	unsigned short m_nSCH_Limmin_Y[20];
	unsigned short m_nSCH_Limmin_X[20];
	unsigned char m_nPCB_Layer_Switch[20];
	unsigned char m_nSCH_Layer_Switch[20];
	char m_szSCH_Layer_Name[20][16];

	pFile->Seek(0,CFile::begin);
	pFile->Read(s1,26);
	s2 ="Protel V4.0 Design File\n";
	if(strcmp(s1,s2) != 0)
	{
		AfxMessageBox("无效数据格式，文件读入失败.", MB_OK|MB_ICONSTOP);
		return;
	}
		
	LOGFONT logfont;
	logfont.lfHeight = 10;
	logfont.lfWidth = 0;
	logfont.lfEscapement = 0;
	logfont.lfOrientation = 0;
	logfont.lfWeight = FW_NORMAL;
	logfont.lfItalic = FALSE;
	logfont.lfUnderline = FALSE;
	logfont.lfStrikeOut = FALSE;
	logfont.lfCharSet = DEFAULT_CHARSET;
	logfont.lfOutPrecision = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily = DEFAULT_PITCH;
	strcpy(logfont.lfFaceName, "Times New Roman");

	pFile->Seek(80,CFile::begin);
	int m_nPCB_Floatpoint_X, m_nPCB_Floatpoint_Y;
	pFile->Read(&m_nPCB_Floatpoint_X,2);
	pFile->Read(&m_nPCB_Floatpoint_Y,2);
	m_cPCB_Sheet.m_fOriginX = (double)m_nPCB_Floatpoint_X;
	m_cPCB_Sheet.m_fOriginY = (double)m_nPCB_Floatpoint_Y;

	pFile->Seek(96,CFile::begin);
	pFile->Read(m_szGround_Plane,40);
	pFile->Read(m_szPower_Plane,40);
		
	CString planestr;
	planestr = m_szGround_Plane;
	m_csPowerPlane[0] = planestr;
	planestr = m_szPower_Plane;
	m_csPowerPlane[1] = planestr;
		
	pFile->Seek(176,CFile::begin);

	for(i=0;i<20;i++)
	{  
		pFile->Read(&m_nSCH_Limmin_X[i],2);
		pFile->Read(&m_nSCH_Limmin_Y[i],2);
		pFile->Read(&m_nSCH_Limmax_X[i],2);
		pFile->Read(&m_nSCH_Limmax_Y[i],2);
	}
	pFile->Read(&m_nPCB_Limmin_X,2);
	pFile->Read(&m_nPCB_Limmin_Y,2);
	pFile->Read(&m_nPCB_Limmax_X,2);
	pFile->Read(&m_nPCB_Limmax_Y,2);

	pFile->Read(&m_nSCH_Layer_Switch,20);
	for(i=0; i<20; i++)
	{
		if(m_nSCH_Layer_Switch[i] == 1) 	m_bSCH_SheetShow[i] = TRUE;
		else m_bSCH_SheetShow[i] = FALSE;
	}
	pFile->Read(&m_nPCB_Layer_Switch,20);
	for(i=0; i<=16; i++)
	{
		int newlayer = EDA_V1_ESD_PCBLayerConvert(i);
		if(m_nPCB_Layer_Switch[i] == 1) m_sPCB_LayerParam[newlayer].bLayerShow = TRUE;
		else m_sPCB_LayerParam[newlayer].bLayerShow = FALSE;
	}

	for(i=0;i<20;i++)
	{ 
		pFile->Seek(384+i*16,CFile::begin);
		pFile->Read(s,16);
		if(strcmp(s,"")!=0) strcpy(m_szSCH_Layer_Name[i],s);
		m_csSCH_SheetName[i] = m_szSCH_Layer_Name[i];
	}
	for(i=0;i<20;i++)
	{ 
		pFile->Seek(704+i*16,CFile::begin);
		pFile->Read(s,15);
	}

	int sch_maxx[SCH_SHEET_NUM],sch_maxy[SCH_SHEET_NUM];
	for(i=0;i<SCH_SHEET_NUM;i++) sch_maxx[i] = sch_maxy[i] = 0;

	pFile->Seek(1024,CFile::begin);
	for(;;)
	{
		int readlength;
		unsigned char temp[64];
		readlength = pFile->Read(temp,16);
		gnline.id = (unsigned char)temp[0];
		gnline.startx = (unsigned short)temp[1] + ((unsigned short)temp[2]<<8);
		gnline.starty = (unsigned short)temp[3] + ((unsigned short)temp[4]<<8);
		gnline.endx = (unsigned short)temp[5] + ((unsigned short)temp[6]<<8);
		gnline.endy = (unsigned short)temp[7] + ((unsigned short)temp[8]<<8);
		gnline.type = (unsigned char)temp[9];
		gnline.layer = (unsigned char)temp[10];
		gnline.linewidth = (unsigned char)temp[11];
		gnline.attrib = (unsigned char)temp[12];
		if(readlength == 0) break;

		if(gnline.id == 0x10)
		{
			if(gnline.attrib == SCH)
			{
				sch_maxx[gnline.layer] = MAX(sch_maxx[gnline.layer], gnline.startx);
				sch_maxx[gnline.layer] = MAX(sch_maxx[gnline.layer], gnline.endx);
				sch_maxy[gnline.layer] = MAX(sch_maxy[gnline.layer], gnline.starty);
				sch_maxy[gnline.layer] = MAX(sch_maxy[gnline.layer], gnline.endy);
			}
		}	
		else if((gnline.id==0x20)||(gnline.id==0x21)||(gnline.id==0x22))
		{
		}
		else if((gnline.id==0x30)||(gnline.id==0x31))
		{  
			gnstring.id = gnline.id;
			gnstring.sx =  (unsigned short)temp[1] + ((unsigned short)temp[2]<<8);
			gnstring.sy =  (unsigned short)temp[3] + ((unsigned short)temp[4]<<8);
			gnstring.height = (unsigned short)temp[5] + ((unsigned short)temp[6]<<8);
			gnstring.align = (unsigned char)temp[7];
			gnstring.layer = (unsigned char)temp[8];
			gnstring.attrib = (unsigned char)temp[9];
			if(gnstring.attrib == SCH)
			{
				sch_maxx[gnstring.layer] = MAX(sch_maxx[gnstring.layer], gnstring.sx);
				sch_maxy[gnstring.layer] = MAX(sch_maxy[gnstring.layer], gnstring.sy);
			}

			pFile->Seek(-16,CFile::current);
			pFile->Read(temp,64);
		}
		else if(gnline.id==0x40)
		{   
			gncomponent.id = gnline.id;
			gncomponent.cx = (unsigned short)temp[1] + ((unsigned short)temp[2]<<8);
			gncomponent.cy = (unsigned short)temp[3] + ((unsigned short)temp[4]<<8);
			gncomponent.angle = (short)temp[5] + ((short)temp[6]<<8);
			gncomponent.total16byte = (unsigned short)temp[7] + ((unsigned short)temp[8]<<8);
			gncomponent.type = (unsigned char)temp[9];
			gncomponent.layer = (unsigned char)temp[10];
			gncomponent.attrib = (unsigned char)temp[12];
			
			sch_maxx[gncomponent.layer] = MAX(sch_maxx[gncomponent.layer], (gncomponent.cx+500));
			sch_maxy[gncomponent.layer] = MAX(sch_maxy[gncomponent.layer], (gncomponent.cy+500));
			
			gncomponent.total16byte = (unsigned short)temp[7] + ((unsigned short)temp[8]<<8);
			pFile->Seek(-16,CFile::current);
			pFile->Read(tempmem,gncomponent.total16byte*16);
		}
		else if(gnline.id==0x50)
		{ 
			dbox.total16byte = (unsigned short)temp[9] + ((unsigned short)temp[10]<<8);
			pFile->Seek(-16,CFile::current);
			pFile->Read(tempmem,dbox.total16byte*16);
		}
		else if(gnline.id == 0x60)
		{
			gnarc.id = gnline.id;
			gnarc.centerx = (unsigned short)temp[1] + ((unsigned short)temp[2]<<8);
			gnarc.centery = (unsigned short)temp[3] + ((unsigned short)temp[4]<<8);
			gnarc.radius  = (unsigned short)temp[5] + ((unsigned short)temp[6]<<8);
			gnarc.stangle = (short)temp[7] + ((short)temp[8]<<8);
			gnarc.endangle = (short)temp[9] + ((short)temp[10]<<8);
			gnarc.linewidth = (unsigned char)temp[11];
			gnarc.attrib = (unsigned char)temp[12];
			gnarc.layer = (unsigned char)temp[14];

			if(gnarc.attrib == SCH)
			{
				sch_maxx[gnarc.layer] = MAX(sch_maxx[gnarc.layer], (gnarc.centerx + gnarc.radius));
				sch_maxy[gnarc.layer] = MAX(sch_maxy[gnarc.layer], (gnarc.centery + gnarc.radius));
			}
		}
		else if(gnline.id==0x70)
		{
			gnfill.total16byte = (unsigned short)temp[5] + ((unsigned short)temp[6]<<8);
			pFile->Seek(-16,CFile::current);
			pFile->Read(tempmem,gnfill.total16byte*16);
		}
		else if(gnline.id==0x80)
		{	
			gnpackage.total16byte = (unsigned short)temp[7] + ((unsigned short)temp[8]<<8);
			pFile->Seek(-16,CFile::current);
			pFile->Read(tempmem,gnpackage.total16byte*16);
		}
		else if(gnline.id==0xa0)
		{
			break;
		}
	}

	for(i=0;i<SCH_SHEET_NUM;i++) 
	{
		if(	sch_maxx[i] == 0) sch_maxx[i] = 47000;
		else sch_maxx[i] += 100;
		if(	sch_maxy[i] == 0) sch_maxy[i] = 33300;
		else sch_maxy[i] += 100;
	}

	pFile->Seek(1024,CFile::begin);
	for(;;)
	{
		int readlength;
		unsigned char temp[64];
		readlength = pFile->Read(temp,16);
		gnline.id = (unsigned char)temp[0];
		gnline.startx = (unsigned short)temp[1] + ((unsigned short)temp[2]<<8);
		gnline.starty = (unsigned short)temp[3] + ((unsigned short)temp[4]<<8);
		gnline.endx = (unsigned short)temp[5] + ((unsigned short)temp[6]<<8);
		gnline.endy = (unsigned short)temp[7] + ((unsigned short)temp[8]<<8);
		gnline.type = (unsigned char)temp[9];
		gnline.layer = (unsigned char)temp[10];
		gnline.linewidth = (unsigned char)temp[11];
		gnline.attrib = (unsigned char)temp[12];

		if(readlength == 0) break;
		if(gnline.id == 0x10)
		{  
			if(gnline.attrib == SCH)
			{
				if((gnline.type == 0)||(gnline.type == 1)||(gnline.type == 16)||(gnline.type == 17))
				{
					COb_schline* pline = new COb_schline();
					pline->pDocument = this;
					pline->m_nDiagram = gnline.layer;	
								
					pline->m_nAttrib = LINE_WIRE;

					if(gnline.type == 16) pline->m_nLineWidth = 2;
					else if(gnline.type == 17) pline->m_nLineWidth = 3;
					else pline->m_nLineWidth = 1;

					pline->m_nColor = cEDA_schwire.m_nColor;
						
					int vertex = 2;
					pline->m_cVertex.SetSize(vertex);
					pline->m_cVertex[0].fx = (double)((unsigned int)gnline.startx);
					pline->m_cVertex[0].fy = (double)(sch_maxy[gnline.layer] - (unsigned int)gnline.starty);
					pline->m_cVertex[1].fx = (double)((unsigned int)gnline.endx);
					pline->m_cVertex[1].fy = (double)(sch_maxy[gnline.layer] - (unsigned int)gnline.endy);

					pline->m_nLineType = 0;
								
					m_listSchObject[gnline.layer].AddTail(pline);
				}
				else
				{
					COb_schpolyline* ppolyline = new COb_schpolyline();
					ppolyline->pDocument = this;
					ppolyline->m_nDiagram = gnline.layer;	
						
					int vertex = 2;
					ppolyline->m_cVertex.SetSize(vertex);
					ppolyline->m_cVertex[0].fx = (double)((unsigned int)gnline.startx);
					ppolyline->m_cVertex[0].fy = (double)(sch_maxy[gnline.layer] - (unsigned int)gnline.starty);
					ppolyline->m_cVertex[1].fx = (double)((unsigned int)gnline.endx);
					ppolyline->m_cVertex[1].fy = (double)(sch_maxy[gnline.layer] - (unsigned int)gnline.endy);
	
					if((gnline.type ==18)||(gnline.type ==20)||(gnline.type ==22)||(gnline.type ==24))	ppolyline->m_nLineWidth = 2;
					else if((gnline.type ==19)||(gnline.type ==21)||(gnline.type ==23)||(gnline.type ==25))	ppolyline->m_nLineWidth = 3;
					else ppolyline->m_nLineWidth = 1;
			
					     if((gnline.type == 2)||(gnline.type == 3)||(gnline.type == 18)||(gnline.type == 19)) ppolyline->m_nLineType = 0; 
					else if((gnline.type == 4)||(gnline.type == 5)||(gnline.type == 20)||(gnline.type == 21)) ppolyline->m_nLineType = 1; 
					else  ppolyline->m_nLineType = 2; //dotted

					ppolyline->m_nColor = cEDA_schpolyline.m_nColor;

					ppolyline->m_nStartLineShape = 0;

					ppolyline->m_nEndLineShape = 0;

					ppolyline->m_nLineShapeSize = 0;
									
					m_listSchObject[gnline.layer].AddTail(ppolyline);
				}
			}
			else
			{
				COb_pcbtrack* ptrack = new COb_pcbtrack();
				ptrack->pDocument = this;
					
				ptrack->m_fWidth = gnline.linewidth;

				ptrack->m_nLayer = EDA_V1_ESD_PCBLayerConvert(gnline.layer);

				ptrack->m_fX1 = (double)((unsigned int)gnline.startx);
				ptrack->m_fY1 = (double)((unsigned int)gnline.starty);
				ptrack->m_fX2 = (double)((unsigned int)gnline.endx);
				ptrack->m_fY2 = (double)((unsigned int)gnline.endy);
							
				m_arrayPcbTrack.Add(ptrack);
			}

		}
		else if((gnline.id==0x20)||(gnline.id==0x21)||(gnline.id==0x22))
		{  
			gnjunction.id = gnline.id;
			gnjunction.centerx = (unsigned short)temp[1] + ((unsigned short)temp[2]<<8);
			gnjunction.centery = (unsigned short)temp[3] + ((unsigned short)temp[4]<<8);
			gnjunction.Dx = (unsigned char)temp[5];
			gnjunction.Dy = (unsigned char)temp[6];
			gnjunction.d = (unsigned char)temp[7];
			gnjunction.padtype = (unsigned char)temp[8];
			gnjunction.attrib = (unsigned char)temp[9];
			gnjunction.layer = (unsigned char)temp[10];
			gnjunction.padname[0] = (unsigned char)temp[11];
			gnjunction.padname[1] = (unsigned char)temp[12];
			gnjunction.padname[2] = (unsigned char)temp[13];
			gnjunction.padname[3] = (unsigned char)temp[14];

			if(gnjunction.layer >= SCH_SHEET_NUM) continue;
			if(gnjunction.attrib == SCH)
			{
				COb_schjunc* pjunc = new COb_schjunc();
				pjunc->pDocument = this;
				pjunc->m_nDiagram = gnjunction.layer;

				pjunc->m_fCx = (double)((unsigned int)gnjunction.centerx);

				pjunc->m_fCy = (double)(sch_maxy[gnjunction.layer] - (unsigned int)gnjunction.centery);

				if(gnjunction.Dx == 20) pjunc->m_nSize = 0;
				else if(gnjunction.Dx == 25) pjunc->m_nSize = 1;
				else if(gnjunction.Dx == 30) pjunc->m_nSize = 2;
				else pjunc->m_nSize = 1;

				pjunc->m_nColor = cEDA_schjunc.m_nColor;
					
				pjunc->m_bManual = FALSE;
					
				m_listSchObject[gnjunction.layer].AddTail(pjunc);
			}
			else
			{
				if(gnjunction.id == 0x21)
				{
					COb_pcbpad* ppad = new COb_pcbpad();
					ppad->pDocument = this;

					ppad->m_nLayer = EDA_V1_ESD_PCBLayerConvert(gnjunction.layer);

					ppad->m_csName = gnjunction.padname;

					ppad->m_fCx = (double)((unsigned int)gnjunction.centerx);
					ppad->m_fCy = (double)((unsigned int)gnjunction.centery);
						
					ppad->m_fRotation = 0;

					ppad->m_nPadMode = PAD_MODE_SIMPLE;

					for(int itemp = 0; itemp < 32; itemp++)
					{
						ppad->m_fSizeX[itemp] = gnjunction.Dx;
					}
					for(int itemp = 0; itemp < 32; itemp++)
					{
						ppad->m_fSizeY[itemp] = gnjunction.Dy;
					}

					int shape;
					if( ((gnjunction.padtype&0x03) == 1) || ( (gnjunction.padtype&0x03) == 3 ) ) shape = 0;
					else shape = 1;
					for(int itemp = 0; itemp < 32; itemp++)
					{
						ppad->m_nShape[itemp] = shape;
					}
		
					ppad->m_fHoleDia = gnjunction.d;
						
					ppad->m_nUnionIndex = -1;		

					ppad->m_bUserRouted = TRUE;

					ppad->m_bPolygonOutline = FALSE;
						
					if( ((gnjunction.padtype>>4)&0x01) == 1)
					{
						CString string;
						string = m_csPowerPlane[0];
						int k;
						for(k=0; k<m_arrayPcbNet.GetCount(); k++)
						{
							COb_pcbnet* pcbnet = (COb_pcbnet*)m_arrayPcbNet.GetAt(k);
							if(pcbnet->m_nFlag != FLAG_NORMAL) continue;
	
							if(string == pcbnet->m_csNetName) break;
						}
						if(k < m_arrayPcbNet.GetCount()) ppad->m_nNet = k;
					}
					else if( ((gnjunction.padtype>>5)&0x01) == 1)
					{
						CString string;
						string = m_csPowerPlane[1];
						
						int k;
						for(k=0; k<m_arrayPcbNet.GetCount(); k++)
						{
							COb_pcbnet* pcbnet = (COb_pcbnet*)m_arrayPcbNet.GetAt(k);
							if(pcbnet->m_nFlag != FLAG_NORMAL) continue;
	
							if(string == pcbnet->m_csNetName) break;
						}
						if(k < m_arrayPcbNet.GetCount()) ppad->m_nNet = k;
					}

					m_arrayPcbPad.Add(ppad);
				}
				else
				{
					COb_pcbvia* pvia = new COb_pcbvia();
					pvia->pDocument = this;

					pvia->m_nLayer = EDA_V1_ESD_PCBLayerConvert(gnjunction.layer);

					pvia->m_nStartLayer = TOPLAYER;

					pvia->m_nEndLayer = BOTTOMLAYER;

					pvia->m_fCx = (double)((unsigned int)gnjunction.centerx);
					pvia->m_fCy = (double)((unsigned int)gnjunction.centery);

					pvia->m_fDia = gnjunction.Dx;

					pvia->m_fHoleDia = gnjunction.d;

					pvia->m_nUnionIndex = -1;		

					pvia->m_bUserRouted = TRUE;

					pvia->m_bPolygonOutline = FALSE;			
						

					if( ((gnjunction.padtype>>4)&0x01) == 1)
					{
						CString string;
						string = m_csPowerPlane[0];

						int k;
						for(k=0; k<m_arrayPcbNet.GetCount(); k++)
						{
							COb_pcbnet* pcbnet = (COb_pcbnet*)m_arrayPcbNet.GetAt(k);
							if(pcbnet->m_nFlag != FLAG_NORMAL) continue;
	
							if(string == pcbnet->m_csNetName) break;
						}
						if(k < m_arrayPcbNet.GetCount()) pvia->m_nNet = k;
					}
					else if( ((gnjunction.padtype>>5)&0x01) == 1)
					{
						CString string;
						string = m_csPowerPlane[1];

						int k;
						for(k=0; k<m_arrayPcbNet.GetCount(); k++)
						{
							COb_pcbnet* pcbnet = (COb_pcbnet*)m_arrayPcbNet.GetAt(k);
							if(pcbnet->m_nFlag != FLAG_NORMAL) continue;
	
							if(string == pcbnet->m_csNetName) break;
						}
						if(k < m_arrayPcbNet.GetCount()) pvia->m_nNet = k;
					}

					m_arrayPcbVia.Add(pvia);
				}
			}
		}
		else if((gnline.id==0x30)||(gnline.id==0x31))
		{  
			pFile->Seek(-16,CFile::current);
			pFile->Read(temp,64);
			gnstring.id = gnline.id;
			gnstring.sx =  (unsigned short)temp[1] + ((unsigned short)temp[2]<<8);
			gnstring.sy =  (unsigned short)temp[3] + ((unsigned short)temp[4]<<8);
			gnstring.height = (unsigned short)temp[5] + ((unsigned short)temp[6]<<8);
			gnstring.align = (unsigned char)temp[7];
			gnstring.layer = (unsigned char)temp[8];
			gnstring.attrib = (unsigned char)temp[9];
			for(i=0;i<33;i++)
				gnstring.text[i] = (unsigned char)temp[11+i];
			for(i=0;i<16;i++)
				gnstring.fontname[i] = (unsigned char)temp[44+i];
			gnstring.width = temp[60];
			gnstring.fonttype = temp[61];

			if(gnstring.attrib == SCH)
			{
				int orientation;
				if(strlen(gnstring.fontname) == 0) strcpy(gnstring.fontname, "宋体"); //"Times New Roman");
				//logfont.lfWidth = 0;
				logfont.lfHeight = ((unsigned int)gnstring.height)/(fEDA_Font_Scale*1.25);
				strcpy(logfont.lfFaceName, (char*)gnstring.fontname);
						
				if(gnstring.align == 14) orientation = 270;
				else if(gnstring.align == 90) orientation = 90;
				else if(gnstring.align == 180) orientation = 180;
				else orientation = 0;
				logfont.lfOrientation = orientation;
				logfont.lfEscapement = logfont.lfOrientation*10;

				int font_index = CheckFontExist(&logfont, (int)gnstring.layer);
				if(font_index < 0) font_index = m_arrayFont[gnstring.layer].Add(logfont);

				COb_schtext* ptext = new COb_schtext();
				ptext->pDocument = this;
				ptext->m_nDiagram = gnstring.layer;

				if(gnstring.id == 0x30) ptext->m_nID = TEXT_TEXT;
				else if(gnstring.id == 0x31) ptext->m_nID = TEXT_NETLABEL;
					
				ptext->m_csNameComment.Empty();

				ptext->m_fSx = (double)((unsigned int)gnstring.sx);

				ptext->m_fSy = (double)(sch_maxy[gnstring.layer] - (unsigned int)gnstring.sy);

				ptext->m_nOrientation = orientation;

				if(gnstring.id == 0x30) ptext->m_nColor = cEDA_schtext.m_nColor;
				else if(gnstring.id == 0x31) ptext->m_nColor = cEDA_schnetlabel.m_nColor;

				ptext->m_nFont = font_index;

				ptext->m_csText = gnstring.text;
					
				ptext->m_bMirror = FALSE;

				ptext->m_nH_Justification = 0;
				ptext->m_nV_Justification = 0;

				m_listSchObject[gnstring.layer].AddTail(ptext);
			}
			else
			{
				COb_pcbtext* ptext = new COb_pcbtext();
				ptext->pDocument = this;

				ptext->m_fSx = (double)gnstring.sx;
				ptext->m_fSy = (double)gnstring.sy;

				ptext->m_fHeight = ((double)gnstring.height)*0.7f;
				ptext->m_fWidth = gnstring.width;

				if(gnstring.align == 0)
				{
					ptext->m_fRotation = 0;
					ptext->m_bMirror = FALSE;
				}
				if(gnstring.align == 90)
				{
					ptext->m_fRotation = 90;
					ptext->m_bMirror = FALSE;
				}
				if(gnstring.align == 180)
				{
					ptext->m_fRotation = 180;
					ptext->m_bMirror = FALSE;
				}
				if(gnstring.align == 14)
				{
					ptext->m_fRotation = 270;
					ptext->m_bMirror = FALSE;
				}
				if(gnstring.align == 76)
				{
					ptext->m_fRotation = 0;
					ptext->m_bMirror = TRUE;
				}
				if(gnstring.align == 166)
				{
					ptext->m_fRotation = 270;
					ptext->m_bMirror = TRUE;
				}
				if(gnstring.align == 152)
				{
					ptext->m_fRotation = 180;
					ptext->m_bMirror = TRUE;
				}
				if(gnstring.align == 242)
				{
					ptext->m_fRotation = 90;
					ptext->m_bMirror = TRUE;
				}

				ptext->m_csText = gnstring.text;

				ptext->m_nStrokeFont = 0;

				ptext->m_nLayer = EDA_V1_ESD_PCBLayerConvert(gnstring.layer);
					
				ptext->m_bUseTTFont = FALSE;

				ptext->m_csTTFontName = "Times New Roman";

				ptext->m_bBold = FALSE;
				ptext->m_bItalic = FALSE;
				ptext->m_bInverted = FALSE;

				ptext->m_nUnionIndex = -1;		

				ptext->m_bUserRouted = TRUE;

				ptext->m_bPolygonOutline = FALSE;	

				m_arrayPcbText.Add(ptext);
			}
		}
		else if(gnline.id==0x40)
		{   
			gncomponent.id = gnline.id;
			gncomponent.cx = (unsigned short)temp[1] + ((unsigned short)temp[2]<<8);
			gncomponent.cy = (unsigned short)temp[3] + ((unsigned short)temp[4]<<8);
			gncomponent.angle = (short)temp[5] + ((short)temp[6]<<8);
			gncomponent.total16byte = (unsigned short)temp[7] + ((unsigned short)temp[8]<<8);
			gncomponent.type = (unsigned char)temp[9];
			gncomponent.layer = (unsigned char)temp[10];
			gncomponent.attrib = (unsigned char)temp[12];
			if((gncomponent.attrib==SCH)&&(gncomponent.total16byte>6))
			{   
				pFile->Seek(-16,CFile::current);
				pFile->Read(tempmem,gncomponent.total16byte*16);
				//if(Save_EMMComp(compmem, gncomponent.total16byte) == -1) break;
			}
			else if(gncomponent.total16byte!=0)
			{
				pFile->Seek(-16,CFile::current);
				pFile->Seek(gncomponent.total16byte*16,CFile::current);
				continue;
			}

			gncomponent.IDx = (unsigned short)tempmem[16] + ((unsigned short)tempmem[17]<<8);
			gncomponent.IDy = (unsigned short)tempmem[18] + ((unsigned short)tempmem[19]<<8);
			gncomponent.IDsize = (unsigned short)tempmem[20] + ((unsigned short)tempmem[21]<<8);
			gncomponent.IDstate = (unsigned char)tempmem[22];
			for(i=0; i<17; i++) gncomponent.ID[i] = (unsigned char)tempmem[23+i];
			gncomponent.Px = (unsigned short)tempmem[40] + ((unsigned short)tempmem[41]<<8);
			gncomponent.Py = (unsigned short)tempmem[42] + ((unsigned short)tempmem[43]<<8);
			gncomponent.Psize = (unsigned short)tempmem[44] + ((unsigned short)tempmem[45]<<8);
			gncomponent.Pstate = (unsigned char)tempmem[46];
			for(i=0; i<16; i++) gncomponent.P[i] = (unsigned char)tempmem[47+i];
			gncomponent.Cx = (unsigned short)tempmem[63] + ((unsigned short)tempmem[64]<<8);
			gncomponent.Cy = (unsigned short)tempmem[65] + ((unsigned short)tempmem[66]<<8);
			gncomponent.Csize = (unsigned short)tempmem[67] + ((unsigned short)tempmem[68]<<8);
			gncomponent.Cstate = (unsigned char)tempmem[69];
			for(i=0; i<26; i++) gncomponent.C[i] = (unsigned char)tempmem[70+i];
			gncomponent.x =  (short)tempmem[96] + ((short)tempmem[97]<<8);
			gncomponent.y =  (short)tempmem[98] + ((short)tempmem[99]<<8);
			gncomponent.pinnum =  (short)tempmem[100] + ((short)tempmem[101]<<8);
			gncomponent.num = (unsigned char)tempmem[102];
			gncomponent.lnum = (unsigned char)tempmem[103];
			gncomponent.jnum = (unsigned char)tempmem[104];
			gncomponent.snum = (unsigned char)tempmem[105];
			gncomponent.anum = (unsigned char)tempmem[106];
			gncomponent.fnum = (unsigned char)tempmem[107];
			gncomponent.ref[0] = (unsigned char)tempmem[108];
			gncomponent.ref[1] = (unsigned char)tempmem[109];
			gncomponent.ref[2] = (unsigned char)tempmem[110];
			gncomponent.comptype = (unsigned char)tempmem[111];

			if(gncomponent.pinnum == 1)
			{
				char* p = (char*)tempmem;
				p+=112;

				unsigned char pintemp[32];
				memcpy(pintemp, p, 32);
				p+=32;

				gnpin.type = (unsigned char)pintemp[0];
				gnpin.copy = (unsigned char)pintemp[1];
				for(int k=0;k<4;k++) gnpin.num[k] = pintemp[2+k];
				for(int k=0;k<16;k++) gnpin.name[k] = pintemp[6+k];
				gnpin.xs = (unsigned short)pintemp[22] + ((unsigned short)pintemp[23]<<8);
				gnpin.ys = (unsigned short)pintemp[24] + ((unsigned short)pintemp[25]<<8);
				gnpin.xe = (unsigned short)pintemp[26] + ((unsigned short)pintemp[27]<<8);
				gnpin.ye = (unsigned short)pintemp[28] + ((unsigned short)pintemp[29]<<8);
				gnpin.p1 = pintemp[30];
				gnpin.p2 = pintemp[31];

				if(((gnpin.type&0x07) == 7) || ((gnpin.type&0x07) == 4))
				{
					COb_schpower* ppower = new COb_schpower();
					ppower->pDocument = this;
					ppower->m_fX = (double)gnpin.xs;
					ppower->m_fY = (double)(sch_maxy[gncomponent.layer] - gnpin.ys);
					ppower->m_nStyle = POWER_STYLE_BAR;
					if(gncomponent.lnum < 10) ppower->m_nOrientation = gncomponent.angle - 90;
					else ppower->m_nOrientation = gncomponent.angle + 90;
					ppower->m_nOrientation = Get_Correct_Orientation(ppower->m_nOrientation);
					ppower->m_bShowNetName = TRUE;
					ppower->m_bIsCrossSheetConnector = FALSE;
					ppower->m_csNetName = gnpin.name;
					ppower->m_nColor = cEDA_schpower.m_nColor;
					ppower->m_nDiagram = gncomponent.layer;

					m_listSchObject[gncomponent.layer].AddTail(ppower);
					continue;
				}
			}

			COb_schcomp* pcomp = new COb_schcomp();
			pcomp->pDocument = this;
			pcomp->m_nDiagram = gncomponent.layer;	

			pcomp->m_fX = gncomponent.cx;

			pcomp->m_fY = (sch_maxy[gncomponent.layer] - gncomponent.cy);

			pcomp->m_nDisplayModeCount = 1;

			pcomp->m_nDisplayMode = 1;

			if(gncomponent.angle < 4) 
			{
				pcomp->m_bMirrored = FALSE;
				pcomp->m_nOrientation = gncomponent.angle*90;
			}
			else
			{
				pcomp->m_bMirrored = TRUE;
				pcomp->m_nOrientation = (gncomponent.angle-4)*90;
			}

			pcomp->m_nPartCount = gncomponent.num;
			if(pcomp->m_nPartCount <= 0) pcomp->m_nPartCount = 1;

			int length = strlen(gncomponent.ID);
			if(gncomponent.ID[length-2] == ':')	pcomp->m_nPart = gncomponent.ID[length-1] - 64;
			else pcomp->m_nPart = 1;
					
			pcomp->m_bShowHidePins = FALSE;
			
			pcomp->m_csLibraryReference = gncomponent.ref;

			pcomp->m_csLibraryFileName.Empty();
					
			pcomp->m_csUniqueID.Empty();

			pcomp->m_nGraphicalType = 0;		

			pcomp->m_bDesignatorLocked = FALSE;

			pcomp->m_bPartLocked = FALSE;

			pcomp->m_bColorLocked = FALSE;

			pcomp->m_nColor = cEDA_schcomp.m_nColor;
			pcomp->m_nFillColor = cEDA_schcomp.m_nFillColor;
			pcomp->m_nPinColor = cEDA_schcomp.m_nPinColor;

			POSITION posParent = m_listSchObject[gncomponent.layer].AddTail(pcomp);


			logfont.lfHeight = gncomponent.IDsize/fEDA_Font_Scale;
			strcpy(logfont.lfFaceName,"宋体");
			logfont.lfOrientation = ((gncomponent.IDstate&0x02)>>1)*90;
			logfont.lfEscapement = logfont.lfOrientation*10;
			int font_index = CheckFontExist(&logfont, (int)gncomponent.layer);
			if(font_index < 0) font_index = m_arrayFont[gncomponent.layer].Add(logfont);

			COb_schtext* ptext = new COb_schtext();
			ptext->pDocument = this;
			ptext->m_nDiagram = gncomponent.layer;	

			ptext->m_nID = TEXT_COMPDESIGNATOR;
			ptext->m_csNameComment = "Designator";
			ptext->m_bNameLocked = TRUE;
						

			ptext->m_fSx = gncomponent.IDx;

			ptext->m_fSy = sch_maxy[gncomponent.layer] - gncomponent.IDy;

			ptext->m_nOrientation = ((gncomponent.IDstate&0x02)>>1)*90;

			ptext->m_nColor = cEDA_schtext.m_nColor;
					
			ptext->m_nFont = font_index;

			ptext->m_bTextVisible = (gncomponent.IDstate+1)&0x01;

			ptext->m_csText = gncomponent.ID;
			int npos = ptext->m_csText.Find(':');
			if(npos >= 0) ptext->m_csText = ptext->m_csText.Left(npos);

			ptext->m_bAutoPosition = FALSE;

			ptext->m_posParent = posParent;

			POSITION pos = m_listSchObject[gncomponent.layer].AddTail(ptext);

			pcomp->m_arrayPos.Add(pos);

			logfont.lfHeight = gncomponent.Csize/fEDA_Font_Scale;
			strcpy(logfont.lfFaceName,"宋体");
			logfont.lfOrientation = ((gncomponent.Cstate&0x02)>>1)*90;
			logfont.lfEscapement = logfont.lfOrientation*10;
			font_index = CheckFontExist(&logfont, (int)gncomponent.layer);
			if(font_index < 0) font_index = m_arrayFont[gncomponent.layer].Add(logfont);

			ptext = new COb_schtext();
			ptext->pDocument = this;
			ptext->m_nDiagram = gncomponent.layer;	

			ptext->m_nID = TEXT_COMPCOMMENT;
			ptext->m_csNameComment = "Comment";
			ptext->m_bNameLocked = TRUE;
						
			ptext->m_fSx = gncomponent.Cx;

			ptext->m_fSy = sch_maxy[gncomponent.layer] - gncomponent.Cy;

			ptext->m_nOrientation = ((gncomponent.Cstate&0x02)>>1)*90;

			ptext->m_nColor = cEDA_schtext.m_nColor;
					
			ptext->m_nFont = font_index;

			ptext->m_bTextVisible = (gncomponent.Cstate+1)&0x01;

			ptext->m_csText = gncomponent.C;

			ptext->m_bAutoPosition = FALSE;

			ptext->m_posParent = posParent;

			pos = m_listSchObject[gncomponent.layer].AddTail(ptext);

			pcomp->m_arrayPos.Add(pos);

			logfont.lfHeight = gncomponent.Csize/fEDA_Font_Scale;
			strcpy(logfont.lfFaceName,"宋体");
			logfont.lfOrientation = 0;
			logfont.lfEscapement = logfont.lfOrientation*10;
			font_index = CheckFontExist(&logfont, (int)gncomponent.layer);
			if(font_index < 0) font_index = m_arrayFont[gncomponent.layer].Add(logfont);

			ptext = new COb_schtext();
			ptext->pDocument = this;
			ptext->m_nDiagram = gncomponent.layer;	

			ptext->m_nID = TEXT_COMPPACKAGE;
			ptext->m_csNameComment = "Package";
			ptext->m_bNameLocked = TRUE;
						
			ptext->m_fSx = gncomponent.Cx;

			ptext->m_fSy = sch_maxy[gncomponent.layer] - (gncomponent.Cy + 100);

			ptext->m_nOrientation = 0;

			ptext->m_nColor = cEDA_schtext.m_nColor;
					
			ptext->m_nFont = font_index;

			ptext->m_bTextVisible = FALSE;

			ptext->m_csText = gncomponent.P;

			ptext->m_bAutoPosition = FALSE;

			ptext->m_posParent = posParent;

			pos = m_listSchObject[gncomponent.layer].AddTail(ptext);

			pcomp->m_arrayPos.Add(pos);

			char* p = (char*)tempmem;
			p+=112;
			for(i=0;i<gncomponent.pinnum;i++)
			{
				unsigned char pintemp[32];
				memcpy(pintemp, p, 32);
				p+=32;

				gnpin.type = (unsigned char)pintemp[0];
				gnpin.copy = (unsigned char)pintemp[1];
				for(int k=0;k<4;k++) gnpin.num[k] = pintemp[2+k];
				for(int k=0;k<16;k++) gnpin.name[k] = pintemp[6+k];
				gnpin.xs = (unsigned short)pintemp[22] + ((unsigned short)pintemp[23]<<8);
				gnpin.ys = (unsigned short)pintemp[24] + ((unsigned short)pintemp[25]<<8);
				gnpin.xe = (unsigned short)pintemp[26] + ((unsigned short)pintemp[27]<<8);
				gnpin.ye = (unsigned short)pintemp[28] + ((unsigned short)pintemp[29]<<8);
				gnpin.p1 = pintemp[30];
				gnpin.p2 = pintemp[31];

				COb_schpin* ppin = new COb_schpin();
				ppin->pDocument = this;
				ppin->m_nDiagram = gncomponent.layer;
			
				ppin->m_nOutsideEdge = (gnpin.type>>3)&0x01;

				ppin->m_nInsideEdge = ((gnpin.type>>4)&0x01)*3;
				

				if((gnpin.type&0x07) == 0) ppin->m_nElectricalType = 0;
				else if((gnpin.type&0x07) == 1) ppin->m_nElectricalType = 2;
				else if((gnpin.type&0x07) == 2) ppin->m_nElectricalType = 1;
				else if((gnpin.type&0x07) == 3) ppin->m_nElectricalType = 3;
				else if((gnpin.type&0x07) == 4) ppin->m_nElectricalType = 7;
				else if((gnpin.type&0x07) == 5) ppin->m_nElectricalType = 4;
				else if((gnpin.type&0x07) == 6) ppin->m_nElectricalType = 5;
				else if((gnpin.type&0x07) == 7) ppin->m_nElectricalType = 7;

				if(gncomponent.pinnum == 1) ppin->m_nElectricalType = 4;


				if( ((gnpin.type&0x07) == 7) || ((gnpin.type&0x07) == 4)  ) ppin->m_bHide =TRUE;
				else ppin->m_bHide = FALSE;
					
				if(gncomponent.comptype == 1)
				{
					ppin->m_bNameVisible = TRUE;
					ppin->m_bNumberVisible = TRUE;
				}
				else if(gncomponent.comptype == 2)
				{
					ppin->m_bNameVisible = FALSE;
					ppin->m_bNumberVisible = TRUE;
				}
				else if(gncomponent.comptype == 3)
				{
					ppin->m_bNameVisible = TRUE;
					ppin->m_bNumberVisible = FALSE;
				}
				else
				{
					ppin->m_bNameVisible = FALSE;
					ppin->m_bNumberVisible = FALSE;
				}

				ppin->m_fPinLength = 100 + (((gnpin.type>>7)+1)&0x01)*200;

				ppin->m_fX = (double)gnpin.xe;

				ppin->m_fY = (double)(sch_maxy[gncomponent.layer] - gnpin.ye);

				if(gnpin.xs == gnpin.xe)
				{
					if(gnpin.ys > gnpin.ye)	ppin->m_nOrientation = 270;
					else ppin->m_nOrientation = 90;
				}
				else if(gnpin.ys == gnpin.ye)
				{
					if(gnpin.xs > gnpin.xe)	ppin->m_nOrientation = 0;
					else ppin->m_nOrientation = 180;
				}
				else ppin->m_nOrientation = 0;

				ppin->m_csName = gnpin.name;

				ppin->m_csNumber = gnpin.num;
								
				ppin->m_nColor = cEDA_schpin.m_nColor;
				ppin->m_bPinLocked = TRUE;

				ppin->m_nOwnerPartID = (gnpin.copy == 0) ? 0:(gnpin.copy- 64);

				ppin->m_nDisplayMode = 0;

				ppin->m_nInside = 0;

				ppin->m_nOutside = 0;									

				ppin->m_csDescription.Empty();

				ppin->m_csConnectTo = ppin->m_csName;
				ppin->m_csUniqueID.Empty();
				ppin->m_posParent = posParent;
							
				POSITION pos = m_listSchObject[gncomponent.layer].AddTail(ppin);

				pcomp->m_arrayPos.Add(pos);
			}
			for(i=0;i<gncomponent.lnum;i++)
			{
				unsigned char linetemp[16];
				memcpy(linetemp, p, 16);
				p += 16;

				gnline.startx = (unsigned short)linetemp[1] + ((unsigned short)linetemp[2]<<8);
				gnline.starty = (unsigned short)linetemp[3] + ((unsigned short)linetemp[4]<<8);
				gnline.endx = (unsigned short)linetemp[5] + ((unsigned short)linetemp[6]<<8);
				gnline.endy = (unsigned short)linetemp[7] + ((unsigned short)linetemp[8]<<8);
				gnline.type = (unsigned char)linetemp[9];
				gnline.layer = (unsigned char)linetemp[10];
				gnline.linewidth = (unsigned char)linetemp[11];
				gnline.attrib = (unsigned char)linetemp[12];
						
				for(int j=0; j<pcomp->m_nPartCount; j++)
				{
					COb_schpolyline* ppolyline = new COb_schpolyline();
					ppolyline->pDocument = this;
					ppolyline->m_nDiagram = gncomponent.layer;	
							
					int vertex = 2;
					ppolyline->m_cVertex.SetSize(vertex);
					ppolyline->m_cVertex[0].fx = (double)((unsigned int)gnline.startx);
					ppolyline->m_cVertex[0].fy = (double)(sch_maxy[gncomponent.layer] - (unsigned int)gnline.starty);
					ppolyline->m_cVertex[1].fx = (double)((unsigned int)gnline.endx);
					ppolyline->m_cVertex[1].fy = (double)(sch_maxy[gncomponent.layer] - (unsigned int)gnline.endy);
		
					ppolyline->m_nLineWidth = 1;
				
					ppolyline->m_nLineType = 0;

					ppolyline->m_nColor = cEDA_schpolyline.m_nColor;

					ppolyline->m_bSelection = FALSE;

					ppolyline->m_nOwnerPartID = j+1;

					ppolyline->m_nDisplayMode = 0;	
											
					ppolyline->m_nStartLineShape = 0;

					ppolyline->m_nEndLineShape = 0;

					ppolyline->m_nLineShapeSize = 0;
								
					ppolyline->m_posParent = posParent;

					POSITION pos = m_listSchObject[gncomponent.layer].AddTail(ppolyline);

					pcomp->m_arrayPos.Add(pos);
				}
			}
			for(i=0;i<gncomponent.jnum;i++)
			{
				unsigned char junctemp[16];
				memcpy(junctemp, p, 16);
				p += 16;
				gnjunction.centerx = (unsigned short)junctemp[1] + ((unsigned short)junctemp[2]<<8);
				gnjunction.centery = (unsigned short)junctemp[3] + ((unsigned short)junctemp[4]<<8);
				gnjunction.Dx = (unsigned char)junctemp[5];
				gnjunction.Dy = (unsigned char)junctemp[6];
				gnjunction.d = (unsigned char)junctemp[7];
				gnjunction.padtype = (unsigned char)junctemp[8];
				gnjunction.attrib = (unsigned char)junctemp[9];
				gnjunction.layer = (unsigned char)junctemp[10];
				gnjunction.padname[0] = (unsigned char)junctemp[11];
				gnjunction.padname[1] = (unsigned char)junctemp[12];
				gnjunction.padname[2] = (unsigned char)junctemp[13];
				gnjunction.padname[3] = (unsigned char)junctemp[14];

				for(int j=0; j<pcomp->m_nPartCount; j++)
				{
					COb_schellipsefill* pellipsefill = new COb_schellipsefill();
					pellipsefill->pDocument = this;
					pellipsefill->m_nDiagram = gncomponent.layer;	
													
					pellipsefill->m_fCx = (double)gnjunction.centerx;

					pellipsefill->m_fCy = sch_maxy[gncomponent.layer] - (double)gnjunction.centery;

					pellipsefill->m_fRadius_x = gnjunction.Dx;

					pellipsefill->m_fRadius_y = gnjunction.Dx;

					pellipsefill->m_nBorderWidth = 0;

					pellipsefill->m_nBorderColor = cEDA_schpolyline.m_nColor;

					pellipsefill->m_nFillColor = cEDA_schpolyline.m_nColor;

					pellipsefill->m_bFillSolid = TRUE;

					pellipsefill->m_bTransparent = FALSE;

					pellipsefill->m_nOwnerPartID = j+1;

					pellipsefill->m_nDisplayMode = 0;	

					pellipsefill->m_posParent = posParent;

					POSITION pos = m_listSchObject[gncomponent.layer].AddTail(pellipsefill);

					pcomp->m_arrayPos.Add(pos);
				}
			}
			for(i=0; i<gncomponent.snum; i++)
			{
				unsigned char texttemp[64];
				memcpy(texttemp, p, 64);
				p += 64;

				gnstring.sx =  (unsigned short)texttemp[1] + ((unsigned short)texttemp[2]<<8);
				gnstring.sy =  (unsigned short)texttemp[3] + ((unsigned short)texttemp[4]<<8);
				gnstring.height = (unsigned short)texttemp[5] + ((unsigned short)texttemp[6]<<8);
				gnstring.align = (unsigned char)texttemp[7];
				gnstring.layer = (unsigned char)texttemp[8];
				gnstring.attrib = (unsigned char)texttemp[9];
				for(int j=0;j<33;j++)
					gnstring.text[j] = (unsigned char)texttemp[11+j];
				for(int j=0;j<16;j++)
					gnstring.fontname[j] = (unsigned char)texttemp[44+j];
				gnstring.width = texttemp[60];
				gnstring.fonttype = texttemp[61];

				int orientation;
				if(strlen(gnstring.fontname) == 0) strcpy(gnstring.fontname, "宋体"); //"Times New Roman");

				logfont.lfHeight = ((unsigned int)gnstring.height)/fEDA_Font_Scale;
				strcpy(logfont.lfFaceName, (char*)gnstring.fontname);
							
				if(gnstring.align == 14) orientation = 270;
				else if(gnstring.align == 90) orientation = 90;
				else if(gnstring.align == 180) orientation = 180;
				else orientation = 0;
				logfont.lfOrientation = orientation;
				logfont.lfEscapement = logfont.lfOrientation*10;

				int font_index = CheckFontExist(&logfont, (int)gncomponent.layer);
				if(font_index < 0) font_index = m_arrayFont[gncomponent.layer].Add(logfont);

				for(int j=0; j<pcomp->m_nPartCount; j++)
				{
					COb_schtext* ptext = new COb_schtext();
					ptext->pDocument = this;
					ptext->m_nDiagram = gncomponent.layer;

					ptext->m_nID = TEXT_TEXT;
							
					ptext->m_csNameComment.Empty();

					ptext->m_fSx = (double)gnstring.sx;

					ptext->m_fSy = (double)(sch_maxy[gncomponent.layer] - gnstring.sy);

					ptext->m_nOrientation = orientation;

					ptext->m_nColor = cEDA_schtext.m_nColor;

					ptext->m_nFont = font_index;
					
					ptext->m_csText = gnstring.text;

					ptext->m_nOwnerPartID = j+1;

					ptext->m_nDisplayMode = 0;							

					ptext->m_bMirror = FALSE;

					ptext->m_nH_Justification = 0;
					ptext->m_nV_Justification = 0;

					ptext->m_posParent = posParent;

					POSITION pos = m_listSchObject[gncomponent.layer].AddTail(ptext);
											
					pcomp->m_arrayPos.Add(pos);
				}

			}
			for(i=0;i<gncomponent.anum;i++)
			{
				unsigned char arctemp[16];
				memcpy(arctemp, p, 16);
				p += 16;

				gnarc.id = gnline.id;
				gnarc.centerx = (unsigned short)arctemp[1] + ((unsigned short)arctemp[2]<<8);
				gnarc.centery = (unsigned short)arctemp[3] + ((unsigned short)arctemp[4]<<8);
				gnarc.radius  = (unsigned short)arctemp[5] + ((unsigned short)arctemp[6]<<8);
				gnarc.stangle = (short)arctemp[7] + ((short)arctemp[8]<<8);
				gnarc.endangle = (short)arctemp[9] + ((short)arctemp[10]<<8);
				gnarc.linewidth = (unsigned char)arctemp[11];
				gnarc.attrib = (unsigned char)arctemp[12];
				gnarc.layer = (unsigned char)arctemp[14];

				for(int j=0; j<pcomp->m_nPartCount; j++)
				{
					COb_scharc* parc = new COb_scharc();
					parc->pDocument = this;
					parc->m_nDiagram = gncomponent.layer;

					parc->m_fCx = (double)((unsigned int)gnarc.centerx);
					parc->m_fCy = (double)(sch_maxy[gncomponent.layer] - (unsigned int)gnarc.centery);

					parc->m_fRadius = (double)((unsigned int)gnarc.radius);

					if((gnarc.linewidth > 14) && (gnarc.linewidth < 40)) parc->m_nWidth = 2;
					else if(gnarc.linewidth > 39) parc->m_nWidth = 3;
					else parc->m_nWidth = 1;

					parc->m_fSangle = (double)gnarc.stangle;
					parc->m_fEangle = (double)gnarc.endangle;

					parc->m_fSangle = Get_Correct_Angle(parc->m_fSangle);
					parc->m_fEangle = Get_Correct_Angle(parc->m_fEangle);
			
					if(parc->m_fSangle == parc->m_fEangle)
					{
						parc->m_fSangle = 0;
						parc->m_fEangle = 360;
					}
					parc->m_nOwnerPartID = j+1;

					parc->m_nDisplayMode = 0;				

					parc->m_nColor = cEDA_schpolyline.m_nColor;
								
					parc->m_posParent = posParent;

					POSITION pos = m_listSchObject[gncomponent.layer].AddTail(parc);

					pcomp->m_arrayPos.Add(pos);
				}
                                        
			}
			for(i=0; i<gncomponent.fnum; i++)
			{
				unsigned char filltemp[16];
				memcpy(filltemp, p, 16);
				gnfill.vertexnum = (unsigned short)filltemp[1] + ((unsigned short)filltemp[2]<<8);
				gnfill.fillpattern = (unsigned short)filltemp[3] + ((unsigned short)filltemp[4]<<8);
				gnfill.total16byte = (unsigned short)filltemp[5] + ((unsigned short)filltemp[6]<<8);
				gnfill.type = (unsigned char)filltemp[9];
				gnfill.layer = (unsigned char)filltemp[10];
				gnfill.attrib = (unsigned char)filltemp[12];
				memcpy(tempmem, p, gnfill.total16byte*16);
				p += gnfill.total16byte*16;

				for(int j=0; j<pcomp->m_nPartCount; j++)
				{
					COb_schpolygon* ppolygon = new COb_schpolygon();
					ppolygon->pDocument = this;
					ppolygon->m_nDiagram = gncomponent.layer;
							
					ppolygon->m_nBorderWidth = 0;

					ppolygon->m_nBorderColor = cEDA_schpolyline.m_nColor;

					ppolygon->m_nFillColor = cEDA_schpolyline.m_nColor;

					ppolygon->m_bFillSolid = TRUE;

					ppolygon->m_nOwnerPartID = j+1;

					ppolygon->m_nDisplayMode = 0;	

					ppolygon->m_bTransparent = FALSE;

					int vertex = gnfill.vertexnum;
					ppolygon->m_cVertex.SetSize(vertex);

					for(int k=0; k<vertex; k++)
					{
						ppolygon->m_cVertex[k].fx = (double)((unsigned int)((unsigned short)tempmem[16+4*k] + ((unsigned short)tempmem[17+4*k]<<8)));
						ppolygon->m_cVertex[k].fy = (double)(sch_maxy[gncomponent.layer] - (unsigned int)((unsigned short)tempmem[18+4*k] + ((unsigned short)tempmem[19+4*k]<<8)));
					}
										
					ppolygon->m_posParent = posParent;

					POSITION pos = m_listSchObject[gncomponent.layer].AddTail(ppolygon);
			
					pcomp->m_arrayPos.Add(pos);
				}
			}
		}
		else if(gnline.id==0x50)
		{ 
			dbox.id = gnline.id;
			dbox.x1 = (unsigned short)temp[1] + ((unsigned short)temp[2]<<8);
			dbox.y1 = (unsigned short)temp[3] + ((unsigned short)temp[4]<<8);
			dbox.x2 = (unsigned short)temp[5] + ((unsigned short)temp[6]<<8);
			dbox.y2 = (unsigned short)temp[7] + ((unsigned short)temp[8]<<8);
			dbox.total16byte = (unsigned short)temp[9] + ((unsigned short)temp[10]<<8);
			dbox.layer = (unsigned char)temp[11];
			dbox.attrib = (unsigned char)temp[12];
			dbox.border = (unsigned char)temp[13];
			dbox.fonttype = (unsigned char)temp[14];

			if(((dbox.attrib==SCH)||(dbox.attrib==PCB))&&(dbox.total16byte>=2))
			{ 
				pFile->Seek(-16,CFile::current);
				pFile->Read(tempmem,dbox.total16byte*16);
				//if(Save_EMMDBox(compmem, dbox.total16byte)==-1) break;
			}
			else if(dbox.total16byte!=0)
			{ 
				pFile->Seek(-16,CFile::current);	
				pFile->Read(tempmem,dbox.total16byte*16);
			}
			dbox.height = (unsigned short)temp[16] + ((unsigned short)temp[17]<<8);
			dbox.width = (unsigned short)temp[18] + ((unsigned short)temp[19]<<8);
			dbox.d = (unsigned short)temp[20] + ((unsigned short)temp[21]<<8);
			dbox.ld = (unsigned short)temp[22] + ((unsigned short)temp[23]<<8);

			if(dbox.attrib == SCH)
			{
				COb_schtextframe* ptextframe = new COb_schtextframe();
				ptextframe->pDocument = this;
				ptextframe->m_nDiagram = dbox.layer;	

				ptextframe->m_fX1 = (double)dbox.x1;

				ptextframe->m_fY1 = (double)(sch_maxy[dbox.layer] - dbox.y1);

				ptextframe->m_fX2 = (double)dbox.x2;

				ptextframe->m_fY2 = (double)(sch_maxy[dbox.layer] - dbox.y2);

				if(ptextframe->m_fY2 < ptextframe->m_fY1)
				{
					double f = ptextframe->m_fY1;
					ptextframe->m_fY1 = ptextframe->m_fY2;
					ptextframe->m_fY2 = f;
				}

				ptextframe->m_nBorderWidth = 1;

				ptextframe->m_nBorderColor = cEDA_schtextframe.m_nBorderColor;
				ptextframe->m_nFillColor = cEDA_schtextframe.m_nFillColor;
				ptextframe->m_nTextColor = cEDA_schtextframe.m_nTextColor;
					
				ptextframe->m_nFont = 0;

				ptextframe->m_bFillSolid = FALSE;

				ptextframe->m_bDrawBorder = dbox.border;

				ptextframe->m_nAlignment = 0;

				ptextframe->m_bWordWrap = FALSE;

				ptextframe->m_bClipToArea = FALSE;

				char* temp_p;
				temp_p = (char*)tempmem;
				temp_p += 32;
				for(int k=0; k<100; k++)
				{
					char string[200];
					memcpy(string, temp_p, strlen(temp_p)+1);
					temp_p += strlen(temp_p)+1;
					CString onelinestr;
					onelinestr = string;
					ptextframe->m_arrayText.Add(onelinestr);
				}

				m_listSchObject[dbox.layer].AddTail(ptextframe);
			}
			else
			{

			}

		}
		else if(gnline.id==0x60)
		{
			gnarc.id = gnline.id;
			gnarc.centerx = (unsigned short)temp[1] + ((unsigned short)temp[2]<<8);
			gnarc.centery = (unsigned short)temp[3] + ((unsigned short)temp[4]<<8);
			gnarc.radius  = (unsigned short)temp[5] + ((unsigned short)temp[6]<<8);
			gnarc.stangle = (short)temp[7] + ((short)temp[8]<<8);
			gnarc.endangle = (short)temp[9] + ((short)temp[10]<<8);
			gnarc.linewidth = (unsigned char)temp[11];
			gnarc.attrib = (unsigned char)temp[12];
			gnarc.layer = (unsigned char)temp[14];

			if(gnarc.attrib == SCH)
			{
				COb_scharc* parc = new COb_scharc();
				parc->pDocument = this;
				parc->m_nDiagram = gnarc.layer;

				parc->m_fCx = (double)((unsigned int)gnarc.centerx);
				parc->m_fCy = (double)(sch_maxy[gnarc.layer] - (unsigned int)gnarc.centery);

				parc->m_fRadius = (double)((unsigned int)gnarc.radius);

				if(gnarc.linewidth < 5) parc->m_nWidth = 0;
				else if((gnarc.linewidth >= 5)&&(gnarc.linewidth < 15)) parc->m_nWidth = 1;
				else if((gnarc.linewidth > 14)&&(gnarc.linewidth < 40)) parc->m_nWidth = 2;
				else parc->m_nWidth = 3;

				parc->m_fSangle = (double)gnarc.stangle;
				parc->m_fEangle = (double)gnarc.endangle;
				parc->m_fSangle = Get_Correct_Angle(parc->m_fSangle);
				parc->m_fEangle = Get_Correct_Angle(parc->m_fEangle);

				if(parc->m_fSangle == parc->m_fEangle)
				{
					parc->m_fSangle = 0;
					parc->m_fEangle = 360;
				}

				parc->m_nColor = cEDA_scharc.m_nColor;

				m_listSchObject[gnarc.layer].AddTail(parc);
			}
			else
			{
				COb_pcbarc* parc = new COb_pcbarc();
				parc->pDocument = this;

				parc->m_nPolygon = -1;

				parc->m_fWidth = gnarc.linewidth;

				parc->m_nLayer = EDA_V1_ESD_PCBLayerConvert(gnarc.layer);

				parc->m_fCx = (double)((unsigned int)gnarc.centerx);
				parc->m_fCy = (double)((unsigned int)gnarc.centery);
					
				parc->m_fRadius = (double)((unsigned int)gnarc.radius);

				parc->m_fSangle = (double)gnarc.stangle;		
				parc->m_fEangle = (double)gnarc.endangle;
				parc->m_fSangle = Get_Correct_Angle(parc->m_fSangle);
				parc->m_fEangle = Get_Correct_Angle(parc->m_fEangle);

				if(parc->m_fSangle == parc->m_fEangle)
				{
					parc->m_fSangle = 0;
					parc->m_fEangle = 360;
				}
				m_arrayPcbArc.Add(parc);
			}
		}
		else if(gnline.id==0x70)
		{
			gnfill.id = gnline.id;
			gnfill.vertexnum = (unsigned short)temp[1] + ((unsigned short)temp[2]<<8);
			gnfill.fillpattern = (unsigned short)temp[3] + ((unsigned short)temp[4]<<8);
			gnfill.total16byte = (unsigned short)temp[5] + ((unsigned short)temp[6]<<8);
			gnfill.type = (unsigned char)temp[9];
			gnfill.layer = (unsigned char)temp[10];
			gnfill.attrib = (unsigned char)temp[12];

			if(((gnfill.attrib==SCH)||(gnfill.attrib==PCB))&&(gnfill.total16byte>=2))
			{  
				pFile->Seek(-16,CFile::current);
				pFile->Read(tempmem,gnfill.total16byte*16);
			}
			else if(gnfill.total16byte!=0)
			{ 
				pFile->Seek(-16,CFile::current);
				pFile->Read(tempmem,gnfill.total16byte*16);
				continue;
			}

			if(gnfill.attrib == SCH)
			{
				COb_schpolygon* ppolygon = new COb_schpolygon();
				ppolygon->pDocument = this;
				ppolygon->m_nDiagram = gnfill.layer;
			
				ppolygon->m_nBorderWidth = 1;

				ppolygon->m_nBorderColor = cEDA_schpolygon.m_nBorderColor;

				ppolygon->m_nFillColor = cEDA_schpolygon.m_nFillColor;

				ppolygon->m_bFillSolid = TRUE;


				ppolygon->m_bTransparent = FALSE;

				int vertex = gnfill.vertexnum;
				ppolygon->m_cVertex.SetSize(vertex);

				for(int k=0; k<vertex; k++)
				{
					ppolygon->m_cVertex[k].fx = (double)((unsigned int)((unsigned short)tempmem[16+4*k] + ((unsigned short)tempmem[17+4*k]<<8)));
					ppolygon->m_cVertex[k].fy = (double)(sch_maxy[gnfill.layer] - (unsigned int)((unsigned short)tempmem[18+4*k] + ((unsigned short)tempmem[19+4*k]<<8)));
				}
							
				m_listSchObject[gnfill.layer].AddTail(ppolygon);
			}
			else
			{
				COb_pcbregion* pregion = new COb_pcbregion();
				pregion->pDocument = this;

				int vertex = gnfill.vertexnum;
				pregion->m_cVertex.SetSize(vertex);
				for(int k=0; k < vertex; k++)
				{
					pregion->m_cVertex[k].fx = (double)((unsigned int)((unsigned short)tempmem[16+4*k] + ((unsigned short)tempmem[17+4*k]<<8)));
					pregion->m_cVertex[k].fy = (double)((unsigned int)((unsigned short)tempmem[18+4*k] + ((unsigned short)tempmem[19+4*k]<<8)));
					pregion->m_cVertex[k].cx = 0;
					pregion->m_cVertex[k].cy = 0;
					pregion->m_cVertex[k].sangle = 0;
					pregion->m_cVertex[k].eangle = 0;
					pregion->m_cVertex[k].radius = 0;
				}

				pregion->m_nLayer = EDA_V1_ESD_PCBLayerConvert(gnfill.layer);

				m_arrayPcbRegion.Add(pregion);
			}

		}
		else if(gnline.id==0x80)
		{	
			gnpackage.id = gnline.id;
			gnpackage.cx = (unsigned short)temp[1] + ((unsigned short)temp[2]<<8);
			gnpackage.cy = (unsigned short)temp[3] + ((unsigned short)temp[4]<<8);
			gnpackage.angle = (short)temp[5] + ((short)temp[6]<<8);
			gnpackage.total16byte = (unsigned short)temp[7] + ((unsigned short)temp[8]<<8);
			gnpackage.type = (unsigned char)temp[9];
			gnpackage.layer = (unsigned char)temp[10];
			gnpackage.attrib = (unsigned char)temp[12];
			gnpackage.IDwidth = (unsigned char)temp[13];
			gnpackage.Cwidth = (unsigned char)temp[14];

			if(gnpackage.total16byte>6)
			{   
				pFile->Seek(-16,CFile::current);
				pFile->Read(tempmem,gnpackage.total16byte*16);
			}
			else
			{
				pFile->Seek(-16,CFile::current);
				pFile->Seek(gnpackage.total16byte*16,CFile::current);
				continue;
			}

			gnpackage.IDx = (unsigned short)tempmem[16] + ((unsigned short)tempmem[17]<<8);
			gnpackage.IDy = (unsigned short)tempmem[18] + ((unsigned short)tempmem[19]<<8);
			gnpackage.IDsize = (unsigned short)tempmem[20] + ((unsigned short)tempmem[21]<<8);
			gnpackage.IDstate = (unsigned char)tempmem[22];
			for(i=0; i<17; i++) gnpackage.ID[i] = (unsigned char)tempmem[23+i];
			gnpackage.Px = (unsigned short)tempmem[40] + ((unsigned short)tempmem[41]<<8);
			gnpackage.Py = (unsigned short)tempmem[42] + ((unsigned short)tempmem[43]<<8);
			gnpackage.IDalign = temp[44];
			gnpackage.Calign = temp[45];
			gnpackage.Pstate = (unsigned char)tempmem[46];
			for(i=0; i<16; i++) gnpackage.P[i] = (unsigned char)tempmem[47+i];
			gnpackage.Cx = (unsigned short)tempmem[63] + ((unsigned short)tempmem[64]<<8);
			gnpackage.Cy = (unsigned short)tempmem[65] + ((unsigned short)tempmem[66]<<8);
			gnpackage.Csize = (unsigned short)tempmem[67] + ((unsigned short)tempmem[68]<<8);
			gnpackage.Cstate = (unsigned char)tempmem[69];
			for(i=0; i<26; i++) gnpackage.C[i] = (unsigned char)tempmem[70+i];
			gnpackage.x =  (short)tempmem[96] + ((short)tempmem[97]<<8);
			gnpackage.y =  (short)tempmem[98] + ((short)tempmem[99]<<8);
			gnpackage.padnum =  (short)tempmem[100] + ((short)tempmem[101]<<8);
			gnpackage.num = (unsigned char)tempmem[102];
			gnpackage.lnum = (unsigned char)tempmem[103];
			gnpackage.jnum = (unsigned char)tempmem[104];
			gnpackage.snum = (unsigned char)tempmem[105];
			gnpackage.anum = (unsigned char)tempmem[106];
			gnpackage.fnum = (unsigned char)tempmem[107];
			gnpackage.ref[0] = (unsigned char)tempmem[108];
			gnpackage.ref[1] = (unsigned char)tempmem[109];
			gnpackage.ref[2] = (unsigned char)tempmem[110];
			gnpackage.comptype = (unsigned char)tempmem[111];

			if(gnpackage.IDwidth==0) gnpackage.IDwidth=4;
			if(gnpackage.Cwidth==0)  gnpackage.Cwidth=4;
			if((gnpackage.IDalign!=0)&&(gnpackage.IDalign!=90)&&(gnpackage.IDalign!=180)&&(gnpackage.IDalign!=14)&&\
				(gnpackage.IDalign!=152)&&(gnpackage.IDalign!=166)&&(gnpackage.IDalign!=76)&&(gnpackage.IDalign!=242))
				gnpackage.IDalign=0;
			if(((gnpackage.IDstate&0x02)>>1)!=0) gnpackage.IDalign=90;
			if((gnpackage.Calign!=0)&&(gnpackage.Calign!=90)&&(gnpackage.Calign!=180)&&(gnpackage.Calign!=14)&&\
				(gnpackage.Calign!=152)&&(gnpackage.Calign!=166)&&(gnpackage.Calign!=76)&&(gnpackage.Calign!=242))
				gnpackage.Calign=0;
			if(((gnpackage.Cstate&0x02)>>1)!=0) gnpackage.Calign=90;

			COb_pcbcomp* pcbcomp = new COb_pcbcomp();
			pcbcomp->pDocument = this;
				
			pcbcomp->m_fX = (double)gnpackage.cx;
			pcbcomp->m_fY = (double)gnpackage.cy;

			pcbcomp->m_nLayer = EDA_V1_ESD_PCBLayerConvert(gnpackage.layer);

			pcbcomp->m_fHeight = 1;

			if(gnpackage.angle < 4) 
			{
				pcbcomp->m_fRotation = gnpackage.angle*90;
			}
			else if(gnpackage.angle == 4) pcbcomp->m_fRotation = 180;
			else if(gnpackage.angle == 5) pcbcomp->m_fRotation = 90;
			else if(gnpackage.angle == 6) pcbcomp->m_fRotation = 0;
			else if(gnpackage.angle == 7) pcbcomp->m_fRotation = 270;
				
			pcbcomp->m_csPattern.Empty();		
			pcbcomp->m_csSourceDesignator.Empty();	
			pcbcomp->m_csSourceUniqueID.Empty();	
			pcbcomp->m_csSourceHierarchicalPath.Empty();	
			pcbcomp->m_csSourceFootprintLibrary.Empty();	
			pcbcomp->m_csSourceComponentLibrary.Empty();	
			pcbcomp->m_csSourceLibReference.Empty();	
			pcbcomp->m_csSourceDescription.Empty();	
			pcbcomp->m_csFootprintDescription.Empty();	
			pcbcomp->m_csDefaultPcb3dModel.Empty();	

			pcbcomp->m_bSelection = FALSE;

			pcbcomp->m_bNameOn = (gnpackage.IDstate+1)&0x01;

			pcbcomp->m_bCommentOn = (gnpackage.Cstate+1)&0x01;

			pcbcomp->m_bLocked = FALSE;			
			pcbcomp->m_bPrimitiveLock = FALSE;	
			pcbcomp->m_bLockStrings = FALSE;				
			pcbcomp->m_bEnablePinSwapping = FALSE;
			pcbcomp->m_bEnablePartSwapping = FALSE;

			pcbcomp->m_nGroupNum = 0;
			pcbcomp->m_nCount = 0;
			pcbcomp->m_nComponentKind = 0;
			pcbcomp->m_nComponentKindVersion2 = 0;
			pcbcomp->m_nChannelOffset = -1;

			pcbcomp->m_nUnionIndex = -1;		

			pcbcomp->m_bPolygonOutline = FALSE;

			pcbcomp->m_bUserRouted = TRUE;				

			int indexParent = m_arrayPcbComp.Add(pcbcomp);

			COb_pcbtext* ptext = new COb_pcbtext();
			ptext->pDocument = this;

			ptext->m_fSx = (double)gnpackage.IDx;
			ptext->m_fSy = (double)gnpackage.IDy;

			ptext->m_fHeight = ((double)gnpackage.IDsize)*0.70f;
			ptext->m_fWidth = (double)gnpackage.IDwidth;

			if(gnpackage.IDalign == 0)
			{
				ptext->m_fRotation = 0;
				ptext->m_bMirror = FALSE;
			}
			if(gnpackage.IDalign == 90)
			{
				ptext->m_fRotation = 90;
				ptext->m_bMirror = FALSE;
			}
			if(gnpackage.IDalign == 180)
			{
				ptext->m_fRotation = 180;
				ptext->m_bMirror = FALSE;
			}
			if(gnpackage.IDalign == 14)
			{
				ptext->m_fRotation = 270;
				ptext->m_bMirror = FALSE;
			}
			if(gnpackage.IDalign == 76)
			{
				ptext->m_fRotation = 0;
				ptext->m_bMirror = TRUE;
			}
			if(gnpackage.IDalign == 166)
			{
				ptext->m_fRotation = 270;
				ptext->m_bMirror = TRUE;
			}
			if(gnpackage.IDalign == 152)
			{
				ptext->m_fRotation = 180;
				ptext->m_bMirror = TRUE;
			}
			if(gnpackage.IDalign == 242)
			{
				ptext->m_fRotation = 90;
				ptext->m_bMirror = TRUE;
			}

			ptext->m_csText = gnpackage.ID;

			ptext->m_nStrokeFont = 0;

			ptext->m_nComponent = indexParent;

			ptext->m_bDesignator = TRUE;

			ptext->m_bComment = FALSE;

			ptext->m_nLayer = EDA_V1_ESD_PCBLayerConvert(gnpackage.IDstate>>2);

			ptext->m_bUseTTFont = FALSE;

			ptext->m_csTTFontName = "Times New Roman";

			ptext->m_bBold = FALSE;
			ptext->m_bItalic = FALSE;
			ptext->m_bInverted = FALSE;

			ptext->m_bLocked = FALSE;			

			ptext->m_nUnionIndex = -1;		

			ptext->m_bUserRouted = TRUE;

			ptext->m_bPolygonOutline = FALSE;

			m_arrayPcbText.Add(ptext);

			ptext = new COb_pcbtext();
			ptext->pDocument = this;

			ptext->m_fSx = (double)gnpackage.Cx;
			ptext->m_fSy = (double)gnpackage.Cy;

			ptext->m_fHeight = ((double)gnpackage.Csize)*0.70f;
			ptext->m_fWidth = (double)gnpackage.Cwidth;

			if(gnpackage.Calign == 0)
			{
				ptext->m_fRotation = 0;
				ptext->m_bMirror = FALSE;
			}
			if(gnpackage.Calign == 90)
			{
				ptext->m_fRotation = 90;
				ptext->m_bMirror = FALSE;
			}
			if(gnpackage.Calign == 180)
			{
				ptext->m_fRotation = 180;
				ptext->m_bMirror = FALSE;
			}
			if(gnpackage.Calign == 14)
			{
				ptext->m_fRotation = 270;
				ptext->m_bMirror = FALSE;
			}
			if(gnpackage.Calign == 76)
			{
				ptext->m_fRotation = 0;
				ptext->m_bMirror = TRUE;
			}
			if(gnpackage.Calign == 166)
			{
				ptext->m_fRotation = 270;
				ptext->m_bMirror = TRUE;
			}
			if(gnpackage.Calign == 152)
			{
				ptext->m_fRotation = 180;
				ptext->m_bMirror = TRUE;
			}
			if(gnpackage.Calign == 242)
			{
				ptext->m_fRotation = 90;
				ptext->m_bMirror = TRUE;
			}

			ptext->m_csText = gnpackage.C;

			ptext->m_nStrokeFont = 0;

			ptext->m_nComponent = indexParent;

			ptext->m_bDesignator = TRUE;

			ptext->m_bComment = FALSE;

			ptext->m_nLayer = EDA_V1_ESD_PCBLayerConvert(gnpackage.Cstate>>2);

			ptext->m_bUseTTFont = FALSE;

			ptext->m_csTTFontName = "Times New Roman";

			ptext->m_bBold = FALSE;
			ptext->m_bItalic = FALSE;
			ptext->m_bInverted = FALSE;

			ptext->m_bLocked = FALSE;			
	
			ptext->m_nUnionIndex = -1;		

			ptext->m_bUserRouted = TRUE;

			ptext->m_bPolygonOutline = FALSE;

			m_arrayPcbText.Add(ptext);

			char* p = (char*)tempmem;
			p+=112;
			for(i=0;i<gnpackage.padnum;i++)
			{
				unsigned char padtemp[16];
				memcpy(padtemp, p, 16);
				p+=16;

				gnjunction.id = gnline.id;
				gnjunction.centerx = (unsigned short)padtemp[1] + ((unsigned short)padtemp[2]<<8);
				gnjunction.centery = (unsigned short)padtemp[3] + ((unsigned short)padtemp[4]<<8);
				gnjunction.Dx = (unsigned char)padtemp[5];
				gnjunction.Dy = (unsigned char)padtemp[6];
				gnjunction.d = (unsigned char)padtemp[7];
				gnjunction.padtype = (unsigned char)padtemp[8];
				gnjunction.attrib = (unsigned char)padtemp[9];
				gnjunction.layer = (unsigned char)padtemp[10];
				gnjunction.padname[0] = (unsigned char)padtemp[11];
				gnjunction.padname[1] = (unsigned char)padtemp[12];
				gnjunction.padname[2] = (unsigned char)padtemp[13];
				gnjunction.padname[3] = (unsigned char)padtemp[14];

				COb_pcbpad* ppad = new COb_pcbpad();
				ppad->pDocument = this;

				ppad->m_nLayer = EDA_V1_ESD_PCBLayerConvert(gnjunction.layer);

				ppad->m_csName = gnjunction.padname;

				ppad->m_fCx = (double)((unsigned int)gnjunction.centerx);
				ppad->m_fCy = (double)((unsigned int)gnjunction.centery);
						
				ppad->m_fRotation = 0;

				ppad->m_nPadMode = PAD_MODE_SIMPLE;

				for(int itemp = 0; itemp < 32; itemp++)
				{
					ppad->m_fSizeX[itemp] = gnjunction.Dx;
				}
				for(int itemp = 0; itemp < 32; itemp++)
				{
					ppad->m_fSizeY[itemp] = gnjunction.Dy;
				}

				int shape;
				if( ((gnjunction.padtype&0x03) == 1) || ( (gnjunction.padtype&0x03) == 3 ) ) shape = 0;
				else shape = 1;
				for(int itemp = 0; itemp < 32; itemp++)
				{
					ppad->m_nShape[itemp] = shape;
				}
	
				ppad->m_fHoleDia = gnjunction.d;

				ppad->m_nComponent = indexParent;


				ppad->m_nUnionIndex = -1;		

				ppad->m_bUserRouted = TRUE;

				ppad->m_bPolygonOutline = FALSE;
					
				if( ((gnjunction.padtype>>4)&0x01) == 1)
				{
					CString string;
					string = m_csPowerPlane[0];

					int k;
					for(k=0; k<m_arrayPcbNet.GetCount(); k++)
					{
						COb_pcbnet* pcbnet = (COb_pcbnet*)m_arrayPcbNet.GetAt(k);
						if(pcbnet->m_nFlag != FLAG_NORMAL) continue;
	
						if(string == pcbnet->m_csNetName) break;
					}
					if(k < m_arrayPcbNet.GetCount()) ppad->m_nNet = k;
				}
				else if( ((gnjunction.padtype>>5)&0x01) == 1)
				{
					CString string;
					string = m_csPowerPlane[1];
					
					int k;
					for(k=0; k<m_arrayPcbNet.GetCount(); k++)
					{
						COb_pcbnet* pcbnet = (COb_pcbnet*)m_arrayPcbNet.GetAt(k);
						if(pcbnet->m_nFlag != FLAG_NORMAL) continue;
	
						if(string == pcbnet->m_csNetName) break;
					}
					if(k < m_arrayPcbNet.GetCount()) ppad->m_nNet = k;
				}

				m_arrayPcbPad.Add(ppad);
			}
			for(i=0; i<gnpackage.lnum; i++)
			{
				unsigned char linetemp[16];
				memcpy(linetemp, p, 16);
				p+=16;

				gnline.startx = (unsigned short)linetemp[1] + ((unsigned short)linetemp[2]<<8);
				gnline.starty = (unsigned short)linetemp[3] + ((unsigned short)linetemp[4]<<8);
				gnline.endx = (unsigned short)linetemp[5] + ((unsigned short)linetemp[6]<<8);
				gnline.endy = (unsigned short)linetemp[7] + ((unsigned short)linetemp[8]<<8);
				gnline.type = (unsigned char)linetemp[9];
				gnline.layer = (unsigned char)linetemp[10];
				gnline.linewidth = (unsigned char)linetemp[11];
				gnline.attrib = (unsigned char)linetemp[12];
					
				COb_pcbtrack* ptrack = new COb_pcbtrack();
				ptrack->pDocument = this;
					
				ptrack->m_fWidth = gnline.linewidth;

				ptrack->m_nLayer = EDA_V1_ESD_PCBLayerConvert(gnline.layer);

				ptrack->m_nComponent = indexParent;

				ptrack->m_fX1 = (double)((unsigned int)gnline.startx);
				ptrack->m_fY1 = (double)((unsigned int)gnline.starty);
				ptrack->m_fX2 = (double)((unsigned int)gnline.endx);
				ptrack->m_fY2 = (double)((unsigned int)gnline.endy);
						
				m_arrayPcbTrack.Add(ptrack);
			}
			for(i=0; i<gnpackage.jnum; i++)
			{
				unsigned char padtemp[16];
				memcpy(padtemp, p, 16);
				p+=16;

				gnjunction.id = gnline.id;
				gnjunction.centerx = (unsigned short)padtemp[1] + ((unsigned short)padtemp[2]<<8);
				gnjunction.centery = (unsigned short)padtemp[3] + ((unsigned short)padtemp[4]<<8);
				gnjunction.Dx = (unsigned char)padtemp[5];
				gnjunction.Dy = (unsigned char)padtemp[6];
				gnjunction.d = (unsigned char)padtemp[7];
				gnjunction.padtype = (unsigned char)padtemp[8];
				gnjunction.attrib = (unsigned char)padtemp[9];
				gnjunction.layer = (unsigned char)padtemp[10];
				gnjunction.padname[0] = (unsigned char)padtemp[11];
				gnjunction.padname[1] = (unsigned char)padtemp[12];
				gnjunction.padname[2] = (unsigned char)padtemp[13];
				gnjunction.padname[3] = (unsigned char)padtemp[14];

				COb_pcbpad* ppad = new COb_pcbpad();
				ppad->pDocument = this;

				ppad->m_nLayer = EDA_V1_ESD_PCBLayerConvert(gnjunction.layer);

				ppad->m_csName = gnjunction.padname;

				ppad->m_fCx = (double)((unsigned int)gnjunction.centerx);
				ppad->m_fCy = (double)((unsigned int)gnjunction.centery);
						
				ppad->m_fRotation = 0;

				ppad->m_nPadMode = PAD_MODE_SIMPLE;

				for(int itemp = 0; itemp < 32; itemp++)
				{
					ppad->m_fSizeX[itemp] = gnjunction.Dx;
				}
				for(int itemp = 0; itemp < 32; itemp++)
				{
					ppad->m_fSizeY[itemp] = gnjunction.Dy;
				}

				int shape;
				if( ((gnjunction.padtype&0x03) == 1) || ( (gnjunction.padtype&0x03) == 3 ) ) shape = 0;
				else shape = 1;
				for(int itemp = 0; itemp < 32; itemp++)
				{
					ppad->m_nShape[itemp] = shape;
				}
		
				ppad->m_fHoleDia = gnjunction.d;

				ppad->m_nComponent = indexParent;


				ppad->m_nUnionIndex = -1;		

				ppad->m_bUserRouted = TRUE;

				ppad->m_bPolygonOutline = FALSE;
						
				if( ((gnjunction.padtype>>4)&0x01) == 1)
				{
					CString string;
					string = m_csPowerPlane[0];

					int k;
					for(k=0; k<m_arrayPcbNet.GetCount(); k++)
					{
						COb_pcbnet* pcbnet = (COb_pcbnet*)m_arrayPcbNet.GetAt(k);
						if(pcbnet->m_nFlag != FLAG_NORMAL) continue;
	
						if(string == pcbnet->m_csNetName) break;
					}
					if(k < m_arrayPcbNet.GetCount()) ppad->m_nNet = k;
				}
				else if( ((gnjunction.padtype>>5)&0x01) == 1)
				{
					CString string;
					string = m_csPowerPlane[1];

					int k;
					for(k=0; k<m_arrayPcbNet.GetCount(); k++)
					{
						COb_pcbnet* pcbnet = (COb_pcbnet*)m_arrayPcbNet.GetAt(k);
						if(pcbnet->m_nFlag != FLAG_NORMAL) continue;
	
						if(string == pcbnet->m_csNetName) break;
					}
					if(k < m_arrayPcbNet.GetCount()) ppad->m_nNet = k;
				}

				m_arrayPcbPad.Add(ppad);
			}
			for(i=0; i<gnpackage.snum; i++)
			{
				unsigned char texttemp[64];
				memcpy(texttemp, p, 64);
				p+=64;
			}
			for(i=0; i<gnpackage.anum; i++)
			{
				unsigned char arctemp[16];
				memcpy(arctemp, p, 16);
				p+=16;

				gnarc.centerx = (unsigned short)arctemp[1] + ((unsigned short)arctemp[2]<<8);
				gnarc.centery = (unsigned short)arctemp[3] + ((unsigned short)arctemp[4]<<8);
				gnarc.radius  = (unsigned short)arctemp[5] + ((unsigned short)arctemp[6]<<8);
				gnarc.stangle = (short)arctemp[7] + ((short)arctemp[8]<<8);
				gnarc.endangle = (short)arctemp[9] + ((short)arctemp[10]<<8);
				gnarc.linewidth = (unsigned char)arctemp[11];
				gnarc.attrib = (unsigned char)arctemp[12];
				gnarc.layer = (unsigned char)arctemp[14];

				COb_pcbarc* parc = new COb_pcbarc();
				parc->pDocument = this;

				parc->m_nPolygon = -1;
				parc->m_nComponent = indexParent;

				parc->m_fWidth = gnarc.linewidth;

				parc->m_nLayer = EDA_V1_ESD_PCBLayerConvert(gnarc.layer);

				parc->m_fCx = (double)((unsigned int)gnarc.centerx);
				parc->m_fCy = (double)((unsigned int)gnarc.centery);
					
				parc->m_fRadius = (double)((unsigned int)gnarc.radius);

				parc->m_fSangle = (double)gnarc.stangle;		
				parc->m_fEangle = (double)gnarc.endangle;
				parc->m_fSangle = Get_Correct_Angle(parc->m_fSangle);
				parc->m_fEangle = Get_Correct_Angle(parc->m_fEangle);

				if(parc->m_fSangle == parc->m_fEangle)
				{
					parc->m_fSangle = 0;
					parc->m_fEangle = 360;
				}

				m_arrayPcbArc.Add(parc);
			}
		}
		else if(gnline.id==0xa0)
		{      
			for(;;)
			{
				l = ReadNetString(pFile,s);

				if(l == -1) break;
				if(strcmp(s,"[")==0)
				{  
					ReadNetString(pFile,s);	
					ReadNetString(pFile,s);	
					ReadNetString(pFile,s);	
				}
				else if(strcmp(s,"]")==0) { }
				else if(strcmp(s,"(")==0)
				{  
					ReadNetString(pFile,s);	

					COb_pcbnet* pcbnet = new COb_pcbnet();
					pcbnet->m_csNetName = s;
					m_arrayPcbNet.Add(pcbnet);

					for(;;)
					{
						l = ReadNetString(pFile,s);	
	
						if(l == -1) break;
						if(strcmp(s,")")==0) 
						{ 
							break;
						}
						ReadNetString(pFile,s);	
					}
				}
			}
			break;
		}
	}
	
	double fmaxx, fmaxy, fminx, fminy;
	Get_Document_Maxy(&fmaxx, &fmaxy, &fminx, &fminy);

	m_cPCB_Sheet.m_cVertex.SetSize(5);
	m_cPCB_Sheet.m_cVertex[0].fx = fminx;
	m_cPCB_Sheet.m_cVertex[0].fy = fminy;
	m_cPCB_Sheet.m_cVertex[1].fx = fminx;
	m_cPCB_Sheet.m_cVertex[1].fy = fmaxy;
	m_cPCB_Sheet.m_cVertex[2].fx = fmaxx;
	m_cPCB_Sheet.m_cVertex[2].fy = fmaxy;
	m_cPCB_Sheet.m_cVertex[3].fx = fmaxx;
	m_cPCB_Sheet.m_cVertex[3].fy = fminy;
	m_cPCB_Sheet.m_cVertex[4].fx = fminx;
	m_cPCB_Sheet.m_cVertex[4].fy = fminy;

	for(i=0;i<SCH_SHEET_NUM;i++)
	{
		m_pSCH_Sheet[i]->m_bCustom = TRUE;
		m_pSCH_Sheet[i]->m_fCustomWidth = sch_maxx[i];
		m_pSCH_Sheet[i]->m_fCustomHeight = sch_maxy[i];
		m_pSCH_Sheet[i]->m_bTitleBlock = FALSE;
		m_pSCH_Sheet[i]->m_bShowReferenceZones = FALSE;
		m_pSCH_Sheet[i]->m_bShowBorder = FALSE;
		m_pSCH_Sheet[i]->m_nDiagram = i;
	}

	m_cPCB_Sheet.m_fOriginX = 0;
	m_cPCB_Sheet.m_fOriginY = 0;
}

void CESDDoc::Open_AD6_SCH_ASCII_File(CArchive& ar, int total_linenum, int nCurrentDiagram)
{
	CString onelinestr;
				
	CStringArray stringarray;
	stringarray.SetSize(total_linenum);

	for(int i=0; i<total_linenum; i++)
	{
		if(ar.ReadString(onelinestr) == FALSE) 
		{
			AfxMessageBox("意外的数据格式错误，文件丢失部分数据，打开文档失败。");
			return;
		}
		stringarray.SetAt(i, onelinestr);
	}

	Open_AD6_SCH_File(total_linenum, stringarray, nCurrentDiagram);
}

void CESDDoc::Open_AD6_PCB_ASCII_File(CArchive& ar, CString& firstline)
{
	int total_linenum;
	CString onelinestr;

	CStringArray stringarray;

	stringarray.Add(firstline);

	while(ar.ReadString(onelinestr) == TRUE)
	{
		stringarray.Add(onelinestr);
	}

	total_linenum = stringarray.GetSize();

	CArray <POSITION, POSITION&> posArray;
	posArray.RemoveAll();
	posArray.SetSize(total_linenum);

	
	CString cstring, keyword,data;

	for(int linenum=0; linenum < total_linenum; linenum++)
	{
		cstring = stringarray.GetAt(linenum);


		keyword = "RECORD=";
		if(Get_KeyWord_Data(cstring, keyword, data) == FALSE) continue;
		if(data == "Board")
		{
			keyword = "|ORIGINX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fOriginX = atof(data);
			}
			keyword = "|ORIGINY=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fOriginY = atof(data);
			}
			keyword = "|BIGVISIBLEGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				//data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fBigVisibleGridSize = atof(data)/10000.0;
			}
			keyword = "|VISIBLEGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				//data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fVisibleGridSize = atof(data)/10000.0;
			}			
			keyword = "|ELECTRICALGRIDRANGE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fElecGridRange = atof(data);
			}

			keyword = "|ELECTRICALGRIDENABLED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	m_cPCB_Sheet.m_bElecGridEnable = TRUE;
				else m_cPCB_Sheet.m_bElecGridEnable = FALSE;
			}
			keyword = "|SNAPGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				//data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fSnapGridSize = atof(data)/10000.0;
			}
			keyword = "|SNAPGRIDSIZEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				//data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fSnapGridSizeX = atof(data)/10000.0;
			}
			keyword = "|SNAPGRIDSIZEY=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				//data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fSnapGridSizeY = atof(data)/10000.0;
			}
			keyword = "|TRACKGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				//data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fTrackGridSize = atof(data)/10000.0;
			}
			keyword = "|VIAGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				//data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fViaGridSize = atof(data)/10000.0;
			}
			keyword = "|COMPONENTGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				//data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fComponentGridSize = atof(data)/10000.0;
			}
			keyword = "|COMPONENTGRIDSIZEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				//data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fComponentGridSizeX = atof(data)/10000.0;
			}
			keyword = "COMPONENTGRIDSIZEY=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				//data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fComponentGridSizeY = atof(data)/10000.0;
			}

			keyword = "DOTGRID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	m_cPCB_Sheet.m_bDotGrid = TRUE;
				else m_cPCB_Sheet.m_bDotGrid = FALSE;
			}

			keyword = "DISPLAYUNIT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_cPCB_Sheet.m_nUnit = atoi(data);

			keyword = "DESIGNATORDISPLAYMODE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_cPCB_Sheet.m_nDesignatorDisplayMode = atoi(data);
			
			keyword = "PRIMITIVELOCK=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	m_cPCB_Sheet.m_bPrimitiveLock = TRUE;
				else m_cPCB_Sheet.m_bPrimitiveLock = FALSE;
			}

			int vertex_num = 0;
			for(int i=0; i<10000; i++)
			{
				char s[100];
				itoa(i, s, 10);
				//KINDx
				keyword = CString("KIND") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex_num++;
				else break;
			}

			if(vertex_num > 4)
			{
				m_cPCB_Sheet.m_cVertex.RemoveAll();
				m_cPCB_Sheet.m_cVertex.SetSize(vertex_num);
				for(int i=0; i < vertex_num; i++)
				{
					char s[100];
					itoa(i, s, 10);

					keyword = CString("|VX") + s;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_cPCB_Sheet.m_cVertex[i].fx = atof(data);

					keyword = CString("|VY") + s;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_cPCB_Sheet.m_cVertex[i].fy = atof(data);

					keyword = CString("|CX") + s;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_cPCB_Sheet.m_cVertex[i].cx = atof(data);
					keyword = CString("|CY") + s;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_cPCB_Sheet.m_cVertex[i].cy = atof(data);

					keyword = CString("|SA") + s;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_cPCB_Sheet.m_cVertex[i].sangle = atof(data);

					keyword = CString("|EA") + s;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_cPCB_Sheet.m_cVertex[i].eangle = atof(data);

					keyword = CString("|R") + s;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_cPCB_Sheet.m_cVertex[i].radius = atof(data);
				}				
			}

			keyword = "SHEETX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fSheetX = atof(data);
				if(m_cPCB_Sheet.m_fSheetX < 0)m_cPCB_Sheet.m_fSheetX = 0;
			}
			keyword = "SHEETY=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fSheetY = atof(data);
				if(m_cPCB_Sheet.m_fSheetY < 0)m_cPCB_Sheet.m_fSheetY = 0;
			}
			keyword = "SHEETWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fSheetWidth = atof(data);
			}
			keyword = "SHEETHEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fSheetHeight = atof(data);
			}
			keyword = "SHOWSHEET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	m_cPCB_Sheet.m_bShowSheet = TRUE;
				else m_cPCB_Sheet.m_bShowSheet = FALSE;
			}
			keyword = "LOCKSHEET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	m_cPCB_Sheet.m_bLockSheet = TRUE;
				else m_cPCB_Sheet.m_bLockSheet = FALSE;
			}
				
			for(int lay=0; lay<POWER_LAYER_NUM; lay++)
			{
				char layer_num[100];
				itoa(lay+1, layer_num, 10);
				CString str1, str2;

				str1 = "PLANE";
				str2 = "NETNAME=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
				{
					if(data == "(No Net)") m_csPowerPlane[lay] = "无网络";
					else if(data == "(Multiple Nets)") m_csPowerPlane[lay] = "多网络";
					else m_csPowerPlane[lay] = data;
				}

				str1 = "PLANE";
				str2 = "PULLBACK=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_fPlanePullBack[lay] = atof(data);
			}

			for(int lay=0; lay<PCB_LAYER_NUM; lay++)
			{
				char layer_num[100];
				itoa(lay+1, layer_num, 10);
				CString str1, str2;

				str1 = "LAYER";
				str2 = "NAME=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
				{
					m_sPCB_LayerParam[lay].csLayerName = data;

				}
				
				str2 = "PREV=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	m_sPCB_LayerParam[lay].nLayerPrev = atoi(data) - 1;

				str2 = "NEXT=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	m_sPCB_LayerParam[lay].nLayerNext = atoi(data) - 1;

				str2 = "MECHENABLED=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	
				{
					if(data == "FALSE")	m_sPCB_LayerParam[lay].bLayerMechEnabled = FALSE;
					else m_sPCB_LayerParam[lay].bLayerMechEnabled = TRUE;
				}

				str2 = "SHEETLINKED=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	
				{
					if(data == "FALSE")	m_sPCB_LayerParam[lay].bLayerSheetLinked = FALSE;
					else m_sPCB_LayerParam[lay].bLayerSheetLinked = TRUE;
				}

				str2 = "COPTHICK=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	m_sPCB_LayerParam[lay].csLayerCopThick = data;
				str2 = "DIELTYPE=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	m_sPCB_LayerParam[lay].csLayerDielType = data;
				str2 = "DIELCONST=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	m_sPCB_LayerParam[lay].csLayerDielConst = data;
				str2 = "DIELHEIGHT=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	m_sPCB_LayerParam[lay].csLayerDielHeight = data;
				str2 = "DIELMATERIAL=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	m_sPCB_LayerParam[lay].csLayerDielMaterial = data;
			}
			
			if(bEDA_ConvertEnglishLayerName == TRUE) 
			{
				Convert_LayerNameToChinese();
			}

			keyword = "TOGGLELAYERS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				for(int i=0; i<PCB_LAYER_NUM; i++)
				{
					if(data.GetAt(i) == '0') m_sPCB_LayerParam[i].bLayerShow = FALSE;
					else m_sPCB_LayerParam[i].bLayerShow = TRUE;
				}
			}
			
		}
		else if(data == "EngineeringChangeOrderOptions")
		{
		}
		else if(data == "OutputOptions")
		{
		}
		else if(data == "PrinterOptions")
		{
		}
		else if(data == "GerberOptions")
		{
		}
		else if(data == "AdvancedPlacerOptions")
		{
		}
		else if(data == "DesignRuleCheckerOptions")
		{
		}		
		else if(data == "PinSwapOptions")
		{
		}		
		else if(data == "Class")
		{
			COb_pcbclass* pclass = new COb_pcbclass();

			keyword = "|NAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				pclass->m_csName = data;
			}
			
			keyword = "|KIND=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				pclass->m_nKind = atoi(data);
			}
			
			keyword = "|SUPERCLASS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data.CompareNoCase("TRUE") == 0) pclass->m_bSuperClass = TRUE;
				else  pclass->m_bSuperClass = FALSE;
			}

			keyword = "|LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data.CompareNoCase("TRUE") == 0) pclass->m_bLocked = TRUE;
				else  pclass->m_bLocked = FALSE;
			}
			
			int index=0;
			for(;;)
			{
				char s[100];
				itoa(index, s, 10);
				keyword = CString("|M") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pclass->m_arrayMember.Add(data);
				else break;
				index++;
			}

			m_arrayPcbClass.Add(pclass);
		}		
		else if(data == "Net")
		{
			COb_pcbnet* pcbnet = new COb_pcbnet();			

			keyword = "|NAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				pcbnet->m_csNetName = data;
			}
			keyword = "|VISIBLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pcbnet->m_bVisible = TRUE;
				else pcbnet->m_bVisible = FALSE;
			}
			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				pcbnet->m_nColor = atoi(data);
			}
			keyword = "|LOOPREMOVAL=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pcbnet->m_bLoopRemove = TRUE;
				else pcbnet->m_bLoopRemove = FALSE;
			}

			keyword = "|TOPLAYER_MRWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				data = data.TrimRight("mil");
				pcbnet->m_fTrackWidth[TOPLAYER] = atof(data);
			}
			keyword = "|BOTTOMLAYER_MRWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				data = data.TrimRight("mil");
				pcbnet->m_fTrackWidth[BOTTOMLAYER] = atof(data);
			}
			for(int i=1; i<BOTTOMLAYER; i++)
			{
				char str[100];
				itoa(i, str, 10);
				keyword = "|MIDLAYER";
				keyword += str;
				keyword += "_MRWIDTH=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
				{
					data = data.TrimRight("mil");
					pcbnet->m_fTrackWidth[i] = atof(data);
				}
			}
			keyword = "|MRVIASIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				data = data.TrimRight("mil");
				pcbnet->m_fViaDia = atof(data);
			}
			keyword = "|MRVIAHOLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				data = data.TrimRight("mil");
				pcbnet->m_fViaHoleDia = atof(data);
			}

			m_arrayPcbNet.Add(pcbnet);
		}		
		else if(data == "Component")
		{
			COb_pcbcomp* pcbcomp = new COb_pcbcomp();
			pcbcomp->pDocument = this;
			
			keyword = "|X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcbcomp->m_fX = atof(data);
			}
			keyword = "|Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcbcomp->m_fY = atof(data);
			}

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "TOP") pcbcomp->m_nLayer = TOPLAYER;
				else if(data == "BOTTOM") pcbcomp->m_nLayer = BOTTOMLAYER;
			}
			else pcbcomp->m_nLayer = TOPLAYER;

			keyword = "HEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcbcomp->m_fHeight = atof(data);
			}

			keyword = "ROTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_fRotation = atof(data);
			else pcbcomp->m_fRotation = 0;
			
			keyword = "|PATTERN=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csPattern = data;
			else pcbcomp->m_csPattern.Empty();		
			keyword = "|SOURCEDESIGNATOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csSourceDesignator= data;
			else pcbcomp->m_csSourceDesignator.Empty();	
			keyword = "|SOURCEUNIQUEID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csSourceUniqueID = data;
			else pcbcomp->m_csSourceUniqueID.Empty();	
			keyword = "|SOURCEHIERARCHICALPATH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csSourceHierarchicalPath = data;
			else pcbcomp->m_csSourceHierarchicalPath.Empty();	
			keyword = "|SOURCEFOOTPRINTLIBRARY=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csSourceFootprintLibrary = data;
			else pcbcomp->m_csSourceFootprintLibrary.Empty();	
			keyword = "|SOURCECOMPONENTLIBRARY=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csSourceComponentLibrary = data;
			else pcbcomp->m_csSourceComponentLibrary.Empty();	
			keyword = "|SOURCELIBREFERENCE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csSourceLibReference = data;
			else pcbcomp->m_csSourceLibReference.Empty();	
			keyword = "|SOURCEDESCRIPTION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csSourceDescription = data;
			else pcbcomp->m_csSourceDescription.Empty();	
			keyword = "|FOOTPRINTDESCRIPTION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csFootprintDescription = data;
			else pcbcomp->m_csFootprintDescription.Empty();	
			keyword = "|DEFAULTPCB3DMODEL=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csDefaultPcb3dModel = data;
			else pcbcomp->m_csDefaultPcb3dModel.Empty();	

			keyword = "NAMEON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bNameOn = FALSE;
				else pcbcomp->m_bNameOn = TRUE;
			}
			else pcbcomp->m_bNameOn = FALSE;	

			keyword = "COMMENTON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bCommentOn = FALSE;
				else pcbcomp->m_bCommentOn = TRUE;
			}
			else pcbcomp->m_bCommentOn = FALSE;	

			keyword = "NAMEAUTOPOSITION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nNamePosition = atoi(data);
			else pcbcomp->m_nNamePosition = 0;

			keyword = "COMMENTAUTOPOSITION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nCommentPosition = atoi(data);
			else pcbcomp->m_nCommentPosition = 0;			
			
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bLocked = FALSE;
				else pcbcomp->m_bLocked = TRUE;
			}
			else pcbcomp->m_bLocked = FALSE;			
			keyword = "PRIMITIVELOCK=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bPrimitiveLock = FALSE;
				else pcbcomp->m_bPrimitiveLock = TRUE;
			}
			else pcbcomp->m_bPrimitiveLock = FALSE;	
			keyword = "LOCKSTRINGS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bLockStrings = FALSE;
				else pcbcomp->m_bLockStrings = TRUE;
			}
			else pcbcomp->m_bLockStrings = FALSE;				
			keyword = "ENABLEPINSWAPPING=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bEnablePinSwapping = FALSE;
				else pcbcomp->m_bEnablePinSwapping = TRUE;
			}
			else pcbcomp->m_bEnablePinSwapping = FALSE;
			keyword = "ENABLEPARTSWAPPING=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bEnablePartSwapping = FALSE;
				else pcbcomp->m_bEnablePartSwapping = TRUE;
			}
			else pcbcomp->m_bEnablePartSwapping = FALSE;

			keyword = "GROUPNUM=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nGroupNum = atoi(data);
			else pcbcomp->m_nGroupNum = 0;
			keyword = "COUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nCount = atoi(data);
			else pcbcomp->m_nCount = 0;
			keyword = "COMPONENTKIND=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nComponentKind = atoi(data);
			else pcbcomp->m_nComponentKind = 0;
			keyword = "COMPONENTKINDVERSION2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nComponentKindVersion2 = atoi(data);
			else pcbcomp->m_nComponentKindVersion2 = 0;
			keyword = "HANNELOFFSET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nChannelOffset = atoi(data);
			else pcbcomp->m_nChannelOffset = -1;
		
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nUnionIndex = atoi(data);
			else pcbcomp->m_nUnionIndex = -1;		

			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bPolygonOutline = FALSE;
				else pcbcomp->m_bPolygonOutline = TRUE;
			}
			else pcbcomp->m_bPolygonOutline = FALSE;

			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bUserRouted = FALSE;
				else pcbcomp->m_bUserRouted = TRUE;
			}
			else pcbcomp->m_bUserRouted = FALSE;				

			m_arrayPcbComp.Add(pcbcomp);
		}		
		else if(data == "Polygon")
		{
			COb_pcbpolygon* ppolygon = new COb_pcbpolygon();
			ppolygon->pDocument = this;

			keyword = "|NET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_nNet = atoi(data);
			else ppolygon->m_nNet = -1;
			
			keyword = "|LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						ppolygon->m_nLayer = i;
						break;
					}
				}
			}
			else ppolygon->m_nLayer = 0;
			
			keyword = "|PRIMITIVELOCK=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	ppolygon->m_bPrimitiveLocked = TRUE;
				else ppolygon->m_bPrimitiveLocked = FALSE;
			}
			else ppolygon->m_bPrimitiveLocked = FALSE;
			
			keyword = "|LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppolygon->m_bLocked = FALSE;
				else ppolygon->m_bLocked = TRUE;
			}
			else ppolygon->m_bLocked = FALSE;				

			keyword = "|POLYGONTYPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Polygon") ppolygon->m_nPolygonType = COPPER_POLYGON;
				else if(data == "Split Plane") ppolygon->m_nPolygonType = SPLITPLANE;
			}

			int index=0;
			ppolygon->m_cVertex.RemoveAll();
			for(;;)
			{
				Struct_RegionVertex vertex;
				char s[100];
				itoa(index, s, 10);
				keyword = CString("|KIND") + s;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
				{
					keyword = CString("|VX") + s;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex.fx = atof(data);

					keyword = CString("|VY") + s;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex.fy = atof(data);

					keyword = CString("|CX") + s;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex.cx = atof(data);
					keyword = CString("|CY") + s;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex.cy = atof(data);

					keyword = CString("|SA") + s;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex.sangle = atof(data);

					keyword = CString("|EA") + s;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex.eangle = atof(data);

					keyword = CString("|R") + s;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex.radius = atof(data);

					ppolygon->m_cVertex.Add(vertex);
				}
				else break;

				index++;
			}
	
			keyword = "|POUROVER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppolygon->m_bPourOver = FALSE;
				else ppolygon->m_bPourOver = TRUE;
			}
			else ppolygon->m_bPourOver = FALSE;

			//Removed
			keyword = "|REMOVEDEAD=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppolygon->m_bRemoveDead = FALSE;
				else ppolygon->m_bRemoveDead = TRUE;
			}
			else ppolygon->m_bRemoveDead = FALSE;	
					
			//Grid Size
			keyword = "|GRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_fGridSize = atof(data);

			//TRACKWIDTH
			keyword = "|TRACKWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_fTrackWidth = atof(data);

			//MINPRIMLENGTH
			keyword = "|MINPRIMLENGTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_fMinPrimLength = atof(data);

			//HATCHSTYLE
			keyword = "|HATCHSTYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Solid") ppolygon->m_nHatchStyle = PCB_HATCH_SOLID;
				else if(data == "90Degree") ppolygon->m_nHatchStyle = PCB_HATCH_90;
				else if(data == "45Degree") ppolygon->m_nHatchStyle = PCB_HATCH_45;
				else if(data == "Horizontal") ppolygon->m_nHatchStyle = PCB_HATCH_H;
				else if(data == "Vertical") ppolygon->m_nHatchStyle = PCB_HATCH_V;
				else if(data == "None") ppolygon->m_nHatchStyle = PCB_HATCH_NONE;
				else ppolygon->m_nHatchStyle = PCB_HATCH_SOLID;
			}

			//USEOCTAGONS
			keyword = "|USEOCTAGONS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppolygon->m_bUseOctagons = FALSE;
				else ppolygon->m_bUseOctagons = TRUE;
			}
			else ppolygon->m_bUseOctagons = FALSE;	

			//SHELVED
			keyword = "|SHELVED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppolygon->m_bShelved = FALSE;
				else ppolygon->m_bShelved = TRUE;
			}
			else ppolygon->m_bShelved = FALSE;

			//restore 
			keyword = "|RESTORELAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						ppolygon->m_nRestoreLayer = i;
						break;
					}
				}
			}
			else ppolygon->m_nRestoreLayer = 0;

			//RESTORENET

			//REMOVEISLANDSBYAREA
			keyword = "|REMOVEISLANDSBYAREA=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppolygon->m_bRemoveIslandsByArea = FALSE;
				else ppolygon->m_bRemoveIslandsByArea = TRUE;
			}
			else ppolygon->m_bRemoveIslandsByArea = FALSE;	

			//REMOVENECKS
			keyword = "|REMOVENECKS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppolygon->m_bRemoveNecks = FALSE;
				else ppolygon->m_bRemoveNecks = TRUE;
			}
			else ppolygon->m_bRemoveNecks = FALSE;	

			//AREATHRESHOLD
			keyword = "|AREATHRESHOLD=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_fAreaThreshold = atof(data)/100000000;
			
			//ARCRESOLUTION
			keyword = "|ARCRESOLUTION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_fArcResolution = atof(data);
			
			//NECKWIDTHTHRESHOLD
			keyword = "|NECKWIDTHTHRESHOLD=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_fNeckWidthThreshold = atof(data);

			//POUROVERSTYLE
			keyword = "|POUROVERSTYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_nPourOverStyle = atoi(data);

			//POURINDEX
			keyword = "|POURINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_nPourIndex = atoi(data);

			//IGNOREVIOLATIONS
			keyword = "|IGNOREVIOLATIONS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppolygon->m_bIgnoreViolations = FALSE;
				else ppolygon->m_bIgnoreViolations = TRUE;
			}
			else ppolygon->m_bIgnoreViolations = FALSE;	

			//|NAME=84,111,112,32,76,97,121,101,114,45,78,111,32,78,101,116
			keyword = "|NAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				CString string;
				for(;;)
				{
					int index = data.Find(',');
					CString str;
					if(index >= 0) str = data.Left(index);
					else str = data;
					int i = atoi(str);
					char c = (char)i;
					string += c;
					data = data.Right(data.GetLength() - index - 1);

					if(index < 0) break;
				}
				ppolygon->m_csName = string;
			}

			//UnionIndex
			keyword = "|UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_nUnionIndex = atoi(data);
			else ppolygon->m_nUnionIndex = -1;		
			//SubPolyIndex
			keyword = "|SUBPOLYINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_nSubPolyIndex = atoi(data);
			else ppolygon->m_nSubPolyIndex = -1;
			//User Routed
			keyword = "|USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppolygon->m_bUserRouted = FALSE;
				else ppolygon->m_bUserRouted = TRUE;
			}
			else ppolygon->m_bUserRouted = FALSE;	
			//Polygon Outline
			keyword = "|POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppolygon->m_bPolygonOutline = FALSE;
				else ppolygon->m_bPolygonOutline = TRUE;
			}
			else ppolygon->m_bPolygonOutline = FALSE;

			m_arrayPcbPolygon.Add(ppolygon);

		}	
		else if(data == "Dimension")
		{
			COb_pcbdimension* pdimension = new COb_pcbdimension();
			pdimension->pDocument = this;

			keyword = "|LX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fLx = atof(data);
			}
			keyword = "|LY=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fLy = atof(data);
			}
			keyword = "|HX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fHx = atof(data);
			}
			keyword = "|HY=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fHy = atof(data);
			}

			//m_fXs, m_fYs, m_fXe, m_fYe, m_fHeight, m_fWidth
			keyword = "|X1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fXs = atof(data);
			}
			keyword = "|Y1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fYs = atof(data);
			}
			keyword = "|X2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fXe = atof(data);
			}
			keyword = "|Y2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fYe = atof(data);
			}
			keyword = "HEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fHeight = atof(data);
			}
			keyword = "LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fLineWidth = atof(data);
			}

			keyword = "|ARROWSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fArrowSize = atof(data);
			}
			keyword = "|ARROWLINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fArrowLineWidth = atof(data);
			}
			keyword = "|ARROWLENGTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fArrowLength = atof(data);
			}
			keyword = "|ARROWPOSITION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Inside") pdimension->m_nArrowPosition = 0;
				else if(data == "Outside") pdimension->m_nArrowPosition = 1;
			}

			keyword = "|EXTENSIONOFFSET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fExtensionOffset = atof(data);
			}
			keyword = "|EXTENSIONLINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fExtensionLineWidth = atof(data);
			}
			keyword = "|EXTENSIONPICKGAP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fExtensionPickGap = atof(data);
			}

			keyword = "|TEXTHEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextHeight = atof(data);
			}
			
			keyword = "|FONT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				for(int i=0; i<3; i++)
				{
					if( data == csPCB_StrokeFontName[i] )
					{
						pdimension->m_nStrokeFont = i;
						break;
					}
				}
			}
			else pdimension->m_nStrokeFont = 0;
		
			keyword = "|STYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Brackets") pdimension->m_nUnitStyle = UNIT_STYLE_BRACKETS;
				else if(data == "Normal") pdimension->m_nUnitStyle = UNIT_STYLE_NORMAL;
				else pdimension->m_nUnitStyle = UNIT_STYLE_NONE;
			}

			keyword = "|TEXTPOSITION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Auto") pdimension->m_nTextPosition = TEXTPOSITION_AUTO;
				else if(data == "Center") pdimension->m_nTextPosition = TEXTPOSITION_CENTER;
				else if(data == "Top") pdimension->m_nTextPosition = TEXTPOSITION_TOP;
				else if(data == "Bottom") pdimension->m_nTextPosition = TEXTPOSITION_BOTTOM;
				else if(data == "Right") pdimension->m_nTextPosition = TEXTPOSITION_RIGHT;
				else if(data == "Left") pdimension->m_nTextPosition = TEXTPOSITION_LEFT;
				else if(data == "InsideRight") pdimension->m_nTextPosition = TEXTPOSITION_INSIDERIGHT;
				else if(data == "InsideLeft") pdimension->m_nTextPosition = TEXTPOSITION_INSIDELEFT;
				else if(data == "UniDirectional") pdimension->m_nTextPosition = TEXTPOSITION_UNIDIRECTIONAL;
				else if(data == "Manual") pdimension->m_nTextPosition = TEXTPOSITION_MANUAL;
			}			
			
			keyword = "|TEXTGAP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextGap = atof(data);
			}
			
			keyword = "|TEXTFORMAT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "10mil") pdimension->m_nTextFormat = 2;		
				else if(data == "10 (mil)") pdimension->m_nTextFormat = 3;
				else pdimension->m_nTextFormat = 1;							
			}
			keyword = "|TEXTDIMENSIONUNIT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Mils") pdimension->m_nTextDimensionUnit = UNIT_MIL;
				else if(data == "Millimeters") pdimension->m_nTextDimensionUnit = UNIT_MM;
				else if(data == "Inches") pdimension->m_nTextDimensionUnit = UNIT_INCH;
				else if(data == "Centimeters") pdimension->m_nTextDimensionUnit = UNIT_CM;
				else if(data == "AutomaticUnit") pdimension->m_nTextDimensionUnit = UNIT_AUTO;
			}
			keyword = "|TEXTPRECISION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_nTextPrecision = atoi(data);

			keyword = "|TEXTPREFIX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_csPrefix = data;
			keyword = "TEXTSUFFIX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_csSuffix = data;



			//m_fSx, m_fSy, m_fTextWidth, m_fTextHeight
			keyword = "TEXT1X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextSx = atof(data);
			}
			keyword = "TEXT1Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextSy = atof(data);
			}
			keyword = "TEXTHEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextHeight = atof(data);
			}
			keyword = "TEXTWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextLineWidth = atof(data);
			}
			keyword = "TEXT1ANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				pdimension->m_fRotation = pdimension->m_fTextRotation = atof(data);
			}
			else
			{
				pdimension->m_fRotation = pdimension->m_fTextRotation = 0;
			}

			keyword = "DIMENSIONLAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pdimension->m_nLayer = i;
						break;
					}
				}
			}
			else pdimension->m_nLayer = 0;

			keyword = "DIMENSIONKIND=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 	pdimension->m_nDimensionKind = atoi(data);

			keyword = "USETTFONTS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pdimension->m_bUseTTFont = TRUE;
				else pdimension->m_bUseTTFont = FALSE;
			}
			else pdimension->m_bUseTTFont = FALSE;

			keyword = "FONTNAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_csTTFontName = data;

			keyword = "BOLD=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pdimension->m_bBold = TRUE;
				else pdimension->m_bBold = FALSE;
			}
			else pdimension->m_bBold = FALSE;
			keyword = "ITALIC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pdimension->m_bItalic = TRUE;
				else pdimension->m_bItalic = FALSE;
			}
			else pdimension->m_bItalic = FALSE;

			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bLocked = FALSE;
				else pdimension->m_bLocked = TRUE;
			}
			//Primitive Lock
			keyword = "PRIMITIVELOCK=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bPrimitiveLock = FALSE;
				else pdimension->m_bPrimitiveLock = TRUE;
			}

			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_nUnionIndex = atoi(data);
			else pdimension->m_nUnionIndex = -1;		
			//User Routed
			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bUserRouted = FALSE;
				else pdimension->m_bUserRouted = TRUE;
			}
			else pdimension->m_bUserRouted = FALSE;	
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bPolygonOutline = FALSE;
				else pdimension->m_bPolygonOutline = TRUE;
			}
			else pdimension->m_bPolygonOutline = FALSE;

			m_arrayPcbDimension.Add(pdimension);
		}
		else if(data == "Coordinate")
		{
			COb_pcbcoordinate* pcoordinate = new COb_pcbcoordinate();
			pcoordinate->pDocument = this;

			//m_fSx, m_fSy;m_fHeight;m_fWidth;m_fRotation;m_fX1, m_fY1, m_fX2, m_fY2;
			keyword = "|X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcoordinate->m_fX = atof(data);
			}
			keyword = "|Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcoordinate->m_fY = atof(data);
			}
			keyword = "SIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcoordinate->m_fSize = atof(data);
			}
			keyword = "LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcoordinate->m_fLineWidth = atof(data);
			}
			keyword = "TEXTHEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcoordinate->m_fTextHeight = atof(data);
			}
			keyword = "TEXTWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcoordinate->m_fTextWidth = atof(data);
			}
			keyword = "ROTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcoordinate->m_fRotation = atof(data);
			else pcoordinate->m_fRotation = 0;

			//Stroke Font
			keyword = "FONT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				for(int i=0; i<3; i++)
				{
					if( data == csPCB_StrokeFontName[i] )
					{
						pcoordinate->m_nStrokeFont = i;
						break;
					}
				}
			}
			else pcoordinate->m_nStrokeFont = 0;

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pcoordinate->m_nLayer = i;
						break;
					}
				}
			}
			else pcoordinate->m_nLayer = 0;

			keyword = "STYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Brackets") pcoordinate->m_nUnitStyle = UNIT_STYLE_BRACKETS;
				else if(data == "Normal") pcoordinate->m_nUnitStyle = UNIT_STYLE_NORMAL;
				else pcoordinate->m_nUnitStyle = UNIT_STYLE_NONE;
			}
			
			keyword = "USETTFONTS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pcoordinate->m_bUseTTFont = TRUE;
				else pcoordinate->m_bUseTTFont = FALSE;
			}
			else pcoordinate->m_bUseTTFont = FALSE;

			keyword = "FONTNAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcoordinate->m_csTTFontName = data;
			//Bold, Italic, Inverted
			keyword = "BOLD=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pcoordinate->m_bBold = TRUE;
				else pcoordinate->m_bBold = FALSE;
			}
			else pcoordinate->m_bBold = FALSE;
			keyword = "ITALIC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pcoordinate->m_bItalic = TRUE;
				else pcoordinate->m_bItalic = FALSE;
			}
			else pcoordinate->m_bItalic = FALSE;


			//Selection
			/*keyword = "SELECTION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcoordinate->m_bSelection = FALSE;
				else pcoordinate->m_bSelection = TRUE;
			}
			else pcoordinate->m_bSelection = FALSE;*/

			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcoordinate->m_bLocked = FALSE;
				else pcoordinate->m_bLocked = TRUE;
			}
			//Primitive Lock
			keyword = "PRIMITIVELOCK=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcoordinate->m_bPrimitiveLock = FALSE;
				else pcoordinate->m_bPrimitiveLock = TRUE;
			}

			//IndexForSave
			//keyword = "INDEXFORSAVE=";
			//if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcoordinate->m_nIndexForSave = atoi(data);
			//else pcoordinate->m_nIndexForSave = -1;			
			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcoordinate->m_nUnionIndex = atoi(data);
			else pcoordinate->m_nUnionIndex = -1;		
			//User Routed
			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcoordinate->m_bUserRouted = FALSE;
				else pcoordinate->m_bUserRouted = TRUE;
			}
			else pcoordinate->m_bUserRouted = FALSE;	
			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcoordinate->m_bPolygonOutline = FALSE;
				else pcoordinate->m_bPolygonOutline = TRUE;
			}
			else pcoordinate->m_bPolygonOutline = FALSE;

			m_arrayPcbCoordinate.Add(pcoordinate);
		}
		else if(data == "Connection")
		{
			COb_pcbconnection* pconnection = new COb_pcbconnection();

			keyword = "|NET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				pconnection->m_nNet = atoi(data);
			}

			keyword = "|X1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pconnection->m_fX1 = atof(data);
			}

			keyword = "|Y1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pconnection->m_fY1 = atof(data);
			}

			keyword = "|X2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pconnection->m_fX2 = atof(data);
			}

			keyword = "|Y2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pconnection->m_fY2 = atof(data);
			}
			
			keyword = "|LAYER1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				pconnection->m_nLayer1 = atoi(data);
			}

			keyword = "|LAYER2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				pconnection->m_nLayer2 = atoi(data);
			}
			
			m_arrayPcbConnection.Add(pconnection);
		}
		else if(data == "DXPRule")
		{
		}
		else if(data == "Rule")
		{
		}
		else if(data == "Embedded")
		{
			COb_pcbdimension* pdimension = new COb_pcbdimension();
			pdimension->pDocument = this;

			keyword = "|LX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fLx = atof(data);
			}
			keyword = "|LY=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fLy = atof(data);
			}
			keyword = "|HX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fHx = atof(data);
			}
			keyword = "|HY=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fHy = atof(data);
			}

			keyword = "|X1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fXs = atof(data);
			}
			keyword = "|Y1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fYs = atof(data);
			}
			keyword = "|X2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fXe = atof(data);
			}
			keyword = "|Y2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fYe = atof(data);
			}
			
			keyword = "|ANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_fRotation = atof(data);
			else pdimension->m_fRotation = 0;
			
			keyword = "|HEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fHeight = atof(data);
			}

			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fLineWidth = atof(data);
			}

			keyword = "|ARROWSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fArrowSize = atof(data);
			}
			keyword = "|ARROWLINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fArrowLineWidth = atof(data);
			}
			keyword = "|ARROWLENGTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fArrowLength = atof(data);
			}
			keyword = "|ARROWPOSITION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Inside") pdimension->m_nArrowPosition = 0;
				else if(data == "Outside") pdimension->m_nArrowPosition = 1;
			}

			keyword = "|EXTENSIONOFFSET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fExtensionOffset = atof(data);
			}
			keyword = "|EXTENSIONLINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fExtensionLineWidth = atof(data);
			}
			keyword = "|EXTENSIONPICKGAP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fExtensionPickGap = atof(data);
			}

			keyword = "|TEXTHEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextHeight = atof(data);
			}
			
			keyword = "|TEXTLINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextLineWidth = atof(data);
			}
			
			keyword = "|FONT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				for(int i=0; i<3; i++)
				{
					if( data == csPCB_StrokeFontName[i] )
					{
						pdimension->m_nStrokeFont = i;
						break;
					}
				}
			}
			else pdimension->m_nStrokeFont = 0;
			
			keyword = "|STYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Brackets") pdimension->m_nUnitStyle = UNIT_STYLE_BRACKETS;
				else if(data == "Normal") pdimension->m_nUnitStyle = UNIT_STYLE_NORMAL;
				else pdimension->m_nUnitStyle = UNIT_STYLE_NONE;
			}
	
			keyword = "|TEXTPOSITION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Auto") pdimension->m_nTextPosition = TEXTPOSITION_AUTO;
				else if(data == "Center") pdimension->m_nTextPosition = TEXTPOSITION_CENTER;
				else if(data == "Top") pdimension->m_nTextPosition = TEXTPOSITION_TOP;
				else if(data == "Bottom") pdimension->m_nTextPosition = TEXTPOSITION_BOTTOM;
				else if(data == "Right") pdimension->m_nTextPosition = TEXTPOSITION_RIGHT;
				else if(data == "Left") pdimension->m_nTextPosition = TEXTPOSITION_LEFT;
				else if(data == "InsideRight") pdimension->m_nTextPosition = TEXTPOSITION_INSIDERIGHT;
				else if(data == "InsideLeft") pdimension->m_nTextPosition = TEXTPOSITION_INSIDELEFT;
				else if(data == "UniDirectional") pdimension->m_nTextPosition = TEXTPOSITION_UNIDIRECTIONAL;
				else if(data == "Manual") pdimension->m_nTextPosition = TEXTPOSITION_MANUAL;
			}			
			
			keyword = "|TEXTGAP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextGap = atof(data);
			}
			
			keyword = "|TEXTFORMAT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "10mil") pdimension->m_nTextFormat = 2;		
				else if(data == "10 (mil)") pdimension->m_nTextFormat = 3;
				else pdimension->m_nTextFormat = 1;							
			}
			keyword = "|TEXTDIMENSIONUNIT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Mils") pdimension->m_nTextDimensionUnit = UNIT_MIL;
				else if(data == "Millimeters") pdimension->m_nTextDimensionUnit = UNIT_MM;
				else if(data == "Inches") pdimension->m_nTextDimensionUnit = UNIT_INCH;
				else if(data == "Centimeters") pdimension->m_nTextDimensionUnit = UNIT_CM;
				else if(data == "AutomaticUnit") pdimension->m_nTextDimensionUnit = UNIT_AUTO;
			}
			keyword = "|TEXTPRECISION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_nTextPrecision = atoi(data);

			keyword = "|TEXTPREFIX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_csPrefix = data;
			keyword = "TEXTSUFFIX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_csSuffix = data;

			keyword = "|TEXT1X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextSx = atof(data);
			}
			keyword = "|TEXT1Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextSy = atof(data);
			}
			keyword = "|TEXT1ANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_fTextRotation = atof(data);
			else pdimension->m_fTextRotation = 0;

			keyword = "|USETTFONTS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pdimension->m_bUseTTFont = TRUE;
				else pdimension->m_bUseTTFont = FALSE;
			}
			else pdimension->m_bUseTTFont = FALSE;

			keyword = "|BOLD=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pdimension->m_bBold = TRUE;
				else pdimension->m_bBold = FALSE;
			}
			else pdimension->m_bBold = FALSE;
			
			keyword = "|ITALIC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pdimension->m_bItalic = TRUE;
				else pdimension->m_bItalic = FALSE;
			}
			else pdimension->m_bItalic = FALSE;
			
			keyword = "|FONTNAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_csTTFontName = data;


			int vertex = 0;
			keyword = "|REFERENCES_COUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex = atoi(data);
			else vertex = 0;
			pdimension->m_cRefPoint.SetSize(vertex);
			for(int i=0; i < vertex; i++)
			{
				char s[100];
				itoa(i, s, 10);
				//参考点X坐标  REFERENCE0POINTX
				keyword = CString("|REFERENCE") + s + CString("POINTX");
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
				{
					data = data.TrimRight("mil");
					pdimension->m_cRefPoint[i].fx = atof(data);
				}
				//参考点Y坐标  REFERENCE0POINTY
				keyword = CString("|REFERENCE") + s + CString("POINTY");
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
				{
					data = data.TrimRight("mil");
					pdimension->m_cRefPoint[i].fy = atof(data);
				}
			}
			
			keyword = "|DIMENSIONLAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pdimension->m_nLayer = i;
						break;
					}
				}
			}
			else pdimension->m_nLayer = 0;

			keyword = "|DIMENSIONKIND=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 	pdimension->m_nDimensionKind = atoi(data);

			keyword = "|DIMENSIONLOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bLocked = FALSE;
				else pdimension->m_bLocked = TRUE;
			}

			keyword = "|PRIMITIVELOCK=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bPrimitiveLock = FALSE;
				else pdimension->m_bPrimitiveLock = TRUE;
			}
	
			keyword = "|UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_nUnionIndex = atoi(data);
			else pdimension->m_nUnionIndex = -1;		

			keyword = "|USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bUserRouted = FALSE;
				else pdimension->m_bUserRouted = TRUE;
			}
			else pdimension->m_bUserRouted = FALSE;	

			keyword = "|POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bPolygonOutline = FALSE;
				else pdimension->m_bPolygonOutline = TRUE;
			}
			else pdimension->m_bPolygonOutline = FALSE;

			m_arrayPcbDimension.Add(pdimension);
		}
		else if(data == "Arc")
		{
			COb_pcbarc* parc = new COb_pcbarc();
			parc->pDocument = this;

			keyword = "NET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nNet = atoi(data);

			keyword = "COMPONENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nComponent = atoi(data);


			keyword = "POLYGON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nPolygon = atoi(data);
			else parc->m_nPolygon = -1;

			keyword = "WIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				parc->m_fWidth = atof(data);
			}

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						parc->m_nLayer = i;
						break;
					}
				}
			}
			else parc->m_nLayer = 0;

			keyword = "LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				parc->m_fCx = atof(data);
			}
			keyword = "LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				parc->m_fCy = atof(data);
			}

			keyword = "RADIUS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				parc->m_fRadius = atof(data);
			}

			keyword = "STARTANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_fSangle = atof(data);
			else parc->m_fSangle = 0;				
			keyword = "ENDANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_fEangle = atof(data);
			else parc->m_fEangle = 0;

			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	parc->m_bLocked = FALSE;
				else parc->m_bLocked = TRUE;
			}
			else parc->m_bLocked = FALSE;			
			//Keepout
			keyword = "KEEPOUT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	parc->m_bKeepout = FALSE;
				else parc->m_bKeepout = TRUE;
			}
			else parc->m_bKeepout = FALSE;

			//IndexForSave
			//keyword = "INDEXFORSAVE=";
			//if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nIndexForSave = atoi(data);
			//else parc->m_nIndexForSave = -1;			
			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nUnionIndex = atoi(data);
			else parc->m_nUnionIndex = -1;		
			//SubPolyIndex
			keyword = "SUBPOLYINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nSubPolyIndex = atoi(data);
			else parc->m_nSubPolyIndex = -1;

			//TearDrop
			keyword = "TEARDROP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	parc->m_bTearDrop = FALSE;
				else parc->m_bTearDrop = TRUE;
			}
			else parc->m_bTearDrop = FALSE;

			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	parc->m_bPolygonOutline = FALSE;
				else parc->m_bPolygonOutline = TRUE;
			}
			else parc->m_bPolygonOutline = FALSE;


			m_arrayPcbArc.Add(parc);

		}
		else if(data == "Pad")
		{
			COb_pcbpad* ppad = new COb_pcbpad();
			ppad->pDocument = this;

			keyword = "NET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nNet = atoi(data);

			keyword = "COMPONENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nComponent = atoi(data);

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						ppad->m_nLayer = i;
						break;
					}
				}
			}
			else ppad->m_nLayer = MULTILAYER;

			keyword = "NAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_csName = data;
			else ppad->m_csName.Empty();

			keyword = "|X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCx = atof(data);
			}
			keyword = "|Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCy = atof(data);
			}

			keyword = "ROTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_fRotation = atof(data);
			else ppad->m_fRotation = 0;

			keyword = "PADMODE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nPadMode = atoi(data);
			else ppad->m_nPadMode = PAD_MODE_SIMPLE;
			if(ppad->m_nPadMode == PAD_MODE_FULLSTACK) ppad->m_nPadMode = PAD_MODE_T_M_B; 


			keyword = "|XSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				for(int itemp = 0; itemp < 32; itemp++)
				{
					ppad->m_fSizeX[itemp] = atof(data);
				}
			}
			keyword = "|YSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				for(int itemp = 0; itemp < 32; itemp++)
				{
					ppad->m_fSizeY[itemp] = atof(data);
				}
			}
			keyword = "|SHAPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				int shape = 0;
				for(int itemp=0; itemp<4; itemp++)
				{
					if(data == csPCB_PadShape[itemp]) shape = itemp;
					if(shape == PAD_SHAPE_ROUNDEDRECTANGLE) shape = PAD_SHAPE_OCTAGONAL;
				}
				for(int itemp = 0; itemp < 32; itemp++)
				{
					ppad->m_nShape[itemp] = shape;
				}
			}

			keyword = "|TOPXSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fSizeX[TOPLAYER] = atof(data);
			}
			keyword = "|TOPYSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fSizeY[TOPLAYER] = atof(data);
			}
			keyword = "|BOTXSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fSizeX[BOTTOMLAYER] = atof(data);
			}
			keyword = "|BOTYSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fSizeY[BOTTOMLAYER] = atof(data);
			}
			keyword = "|TOPSHAPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int itemp=0; itemp<4; itemp++)
				{
					if(data == csPCB_PadShape[itemp]) ppad->m_nShape[TOPLAYER] = itemp;
					if(ppad->m_nShape[TOPLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE) ppad->m_nShape[TOPLAYER] = PAD_SHAPE_OCTAGONAL;
				}
			}
			keyword = "|BOTSHAPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int itemp=0; itemp<4; itemp++)
				{
					if(data == csPCB_PadShape[itemp]) ppad->m_nShape[BOTTOMLAYER] = itemp;
					if(ppad->m_nShape[BOTTOMLAYER] == PAD_SHAPE_ROUNDEDRECTANGLE) ppad->m_nShape[BOTTOMLAYER] = PAD_SHAPE_OCTAGONAL;
				}
			}			

			keyword = "|MIDXSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				for(int itemp = 1; itemp < 31; itemp++)
				{
					ppad->m_fSizeX[itemp] = atof(data);
				}
			}
			keyword = "|MIDYSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				for(int itemp = 1; itemp < 31; itemp++)
				{
					ppad->m_fSizeY[itemp] = atof(data);
				}
			}
			keyword = "|MIDSHAPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				int shape = 0;
				for(int itemp=0; itemp<4; itemp++)
				{
					if(data == csPCB_PadShape[itemp]) shape = itemp;
					if(shape == PAD_SHAPE_ROUNDEDRECTANGLE) shape = PAD_SHAPE_OCTAGONAL;
				}
				for(int itemp = 1; itemp < 31; itemp++)
				{
					ppad->m_nShape[itemp] = shape;
				}

			}


			keyword = "HOLETYPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				ppad->m_nHoleType = atoi(data);
			}

			keyword = "HOLESIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fHoleDia = atof(data);
			}

			keyword = "HOLEROTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_fHoleRotation = atof(data);
			else ppad->m_fHoleRotation = 0;

			keyword = "HOLEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fHoleWidth = atof(data);
			}
			
			keyword = "DAISYCHAIN=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Load") ppad->m_nDaisychain = 0;
				else if(data == "Terminator") ppad->m_nDaisychain = 1;
				else if(data == "Source") ppad->m_nDaisychain = 2;
				else ppad->m_nDaisychain = 0;
			}

			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bLocked = FALSE;
				else ppad->m_bLocked = TRUE;
			}
			else ppad->m_bLocked = FALSE;			

			keyword = "PLATED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bPlated = FALSE;
				else ppad->m_bPlated = TRUE;
			}
			else ppad->m_bPlated = FALSE;

			keyword = "PADJUMPERID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				ppad->m_nJumperID = atoi(data);
			}	

			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nUnionIndex = atoi(data);
			else ppad->m_nUnionIndex = -1;		

			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bUserRouted = FALSE;
				else ppad->m_bUserRouted = TRUE;
			}
			else ppad->m_bUserRouted = FALSE;		

			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bPolygonOutline = FALSE;
				else ppad->m_bPolygonOutline = TRUE;
			}
			else ppad->m_bPolygonOutline = FALSE;

			keyword = "TENTING=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bTenting = FALSE;
				else ppad->m_bTenting = TRUE;
			}
			else ppad->m_bTenting = FALSE;
			keyword = "TENTINGTOP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bTentingTop = FALSE;
				else ppad->m_bTentingTop = TRUE;
			}
			else ppad->m_bTentingTop = FALSE;
			keyword = "TENTINGBOTTOM=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bTentingBottom = FALSE;
				else ppad->m_bTentingBottom = TRUE;
			}
			else ppad->m_bTentingBottom = FALSE;

			keyword = "TESTPOINT_TOP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bTestingTop = FALSE;
				else ppad->m_bTestingTop = TRUE;
			}
			else ppad->m_bTestingTop = FALSE;
			keyword = "TESTPOINT_BOTTOM=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bTestingBottom = FALSE;
				else ppad->m_bTestingBottom = TRUE;
			}
			else ppad->m_bTestingBottom = FALSE;
	
			keyword = "CPLV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCPLV = atoi(data);
			keyword = "CPL=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCPL = atoi(data);

			keyword = "CPEV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCPEV = atoi(data);
			keyword = "CPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCPE = atof(data);
			}			

			keyword = "CSEV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCSEV = atoi(data);
			keyword = "CSE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCSE = atof(data);
			}

			keyword = "CPCV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCPCV = atoi(data);
			keyword = "CPC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCPC = atof(data);
			}

			keyword = "CCSV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCCSV = atoi(data);
			keyword = "CCS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				int i;
				for(i=0; i<3; i++)
				{
					if(data == csPCB_PlaneStyle[i]) break;
				}
				if(i < 3) ppad->m_nCCS = i;
			}
			keyword = "CENV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCENV = atoi(data);
			keyword = "CEN=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCEN = atoi(data);

			keyword = "CCWV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCCWV = atoi(data);
			keyword = "CCW=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCCW = atof(data);
			}

			keyword = "CAGV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCAGV = atoi(data);
			keyword = "CAG=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCAG = atof(data);
			}

			keyword = "CPRV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCPRV = atoi(data);
			keyword = "CPR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCPR = atof(data);
			}

			m_arrayPcbPad.Add(ppad);
		}
		else if(data == "Via")
		{
			COb_pcbvia* pvia = new COb_pcbvia();
			pvia->pDocument = this;

			keyword = "NET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nNet = atoi(data);

			keyword = "COMPONENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nComponent = atoi(data);

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pvia->m_nLayer = i;
						break;
					}
				}
			}
			else pvia->m_nLayer = MULTILAYER;

			keyword = "STARTLAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pvia->m_nStartLayer = i;
						break;
					}
				}
			}
			else pvia->m_nStartLayer = TOPLAYER;

			keyword = "ENDLAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pvia->m_nEndLayer = i;
						break;
					}
				}
			}
			else pvia->m_nEndLayer = BOTTOMLAYER;

			keyword = "|X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCx = atof(data);
			}
			keyword = "|Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCy = atof(data);
			}

			keyword = "|DIAMETER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fDia = atof(data);
			}

			keyword = "|HOLESIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fHoleDia = atof(data);
			}

			//Locked
			keyword = "|LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bLocked = FALSE;
				else pvia->m_bLocked = TRUE;
			}
			else pvia->m_bLocked = FALSE;			
						
			//UnionIndex
			keyword = "|UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nUnionIndex = atoi(data);
			else pvia->m_nUnionIndex = -1;		
			//User Routed
			keyword = "|USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bUserRouted = FALSE;
				else pvia->m_bUserRouted = TRUE;
			}
			else pvia->m_bUserRouted = FALSE;				
			//Polygon Outline
			keyword = "|POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bPolygonOutline = FALSE;
				else pvia->m_bPolygonOutline = TRUE;
			}
			else pvia->m_bPolygonOutline = FALSE;
			
			//Tenting
			keyword = "|TENTING=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bTenting = FALSE;
				else pvia->m_bTenting = TRUE;
			}
			else pvia->m_bTenting = FALSE;
			keyword = "|TENTINGTOP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bTentingTop = FALSE;
				else pvia->m_bTentingTop = TRUE;
			}
			else pvia->m_bTentingTop = FALSE;
			keyword = "|TENTINGBOTTOM=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bTentingBottom = FALSE;
				else pvia->m_bTentingBottom = TRUE;
			}
			else pvia->m_bTentingBottom = FALSE;
			//Testing
			keyword = "TESTPOINT_TOP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bTestingTop = FALSE;
				else pvia->m_bTestingTop = TRUE;
			}
			else pvia->m_bTestingTop = FALSE;
			keyword = "TESTPOINT_BOTTOM=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bTestingBottom = FALSE;
				else pvia->m_bTestingBottom = TRUE;
			}
			else pvia->m_bTestingBottom = FALSE;
	
			//?
			keyword = "CPLV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCPLV = atoi(data);
			keyword = "CPL=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCPL = atoi(data);

			keyword = "CSEV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCSEV = atoi(data);
			keyword = "CSE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCSE = atof(data);
			}

			keyword = "CPCV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCPCV = atoi(data);
			keyword = "CPC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCPC = atof(data);
			}

			keyword = "CCSV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCCSV = atoi(data);
			keyword = "CCS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				int i;
				for(i=0; i<3; i++)
				{
					if(data == csPCB_PlaneStyle[i]) break;
				}
				if(i < 3) pvia->m_nCCS = i;
			}
			keyword = "CENV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCENV = atoi(data);
			keyword = "CEN=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCEN = atoi(data);

			keyword = "CCWV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCCWV = atoi(data);
			keyword = "CCW=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCCW = atof(data);
			}

			keyword = "CAGV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCAGV = atoi(data);
			keyword = "CAG=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCAG = atof(data);
			}

			keyword = "CPRV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCPRV = atoi(data);
			keyword = "CPR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCPR = atof(data);
			}

			m_arrayPcbVia.Add(pvia);

		}
		else if(data == "Track")
		{
			COb_pcbtrack* ptrack = new COb_pcbtrack();
			ptrack->pDocument = this;
			
			keyword = "NET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptrack->m_nNet = atoi(data);

			keyword = "COMPONENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptrack->m_nComponent = atoi(data);

			keyword = "POLYGON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptrack->m_nPolygon = atoi(data);
			else ptrack->m_nPolygon = -1;

			keyword = "WIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptrack->m_fWidth = atof(data);
			}
			else ptrack->m_fWidth = 4;

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						ptrack->m_nLayer = i;
						break;
					}
				}
			}
			else ptrack->m_nLayer = 0;


			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptrack->m_bLocked = FALSE;
				else ptrack->m_bLocked = TRUE;
			}
			else ptrack->m_bLocked = FALSE;			
			//Keepout
			keyword = "KEEPOUT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptrack->m_bKeepout = FALSE;
				else ptrack->m_bKeepout = TRUE;
			}
			else ptrack->m_bKeepout = FALSE;

			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptrack->m_nUnionIndex = atoi(data);
			else ptrack->m_nUnionIndex = -1;		
			//SubPolyIndex
			keyword = "SUBPOLYINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptrack->m_nSubPolyIndex = atoi(data);
			else ptrack->m_nSubPolyIndex = -1;
			//User Routed
			/*keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptrack->m_bUserRouted = FALSE;
				else ptrack->m_bUserRouted = TRUE;
			}
			else ptrack->m_bUserRouted = FALSE;	*/
			//TearDrop
			keyword = "TEARDROP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptrack->m_bTearDrop = FALSE;
				else ptrack->m_bTearDrop = TRUE;
			}
			else ptrack->m_bTearDrop = FALSE;

			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptrack->m_bPolygonOutline = FALSE;
				else ptrack->m_bPolygonOutline = TRUE;
			}
			else ptrack->m_bPolygonOutline = FALSE;

			//X1,Y1,X2,Y2
			keyword = "X1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptrack->m_fX1 = atof(data);
			}
			else ptrack->m_fX1 = 0;
			keyword = "Y1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptrack->m_fY1 = atof(data);
			}
			else ptrack->m_fX2 = 0;
			keyword = "X2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptrack->m_fX2 = atof(data);
			}
			else ptrack->m_fY2 = 0;
			keyword = "Y2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptrack->m_fY2 = atof(data);
			}
			else ptrack->m_fX1 = 0;
					
			m_arrayPcbTrack.Add(ptrack);
		}
		else if(data == "Text")
		{
			COb_pcbtext* ptext = new COb_pcbtext();
			ptext->pDocument = this;

			keyword = "|X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fSx = atof(data);
			}
			keyword = "|Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fSy = atof(data);
			}
			keyword = "X1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fX1 = atof(data);
			}
			keyword = "Y1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fY1 = atof(data);
			}
			keyword = "X2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fX2 = atof(data);
			}
			keyword = "Y2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fY2 = atof(data);
			}
			keyword = "HEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fHeight = atof(data);
			}
			keyword = "WIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fWidth = atof(data);
			}
			keyword = "ROTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_fRotation = atof(data);
			else ptext->m_fRotation = 0;

			keyword = "TEXT=";
			if(Get_Text_Data(cstring, keyword, data) == TRUE) ptext->m_csText = data;
			else ptext->m_csText.Empty();

			//Stroke Font
			keyword = "FONT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				for(int i=0; i<3; i++)
				{
					if( data == csPCB_StrokeFontName[i] )
					{
						ptext->m_nStrokeFont = i;
						break;
					}
				}
			}
			else ptext->m_nStrokeFont = 0;


			keyword = "COMPONENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nComponent = atoi(data);

			keyword = "DESIGNATOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "True")	ptext->m_bDesignator = TRUE;
				else ptext->m_bDesignator = FALSE;
			}

			keyword = "COMMENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "True")	ptext->m_bComment = TRUE;
				else ptext->m_bComment = FALSE;
			}

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						ptext->m_nLayer = i;
						break;
					}
				}
			}
			else ptext->m_nLayer = 0;
			
			//Mirror
			keyword = "MIRROR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	ptext->m_bMirror = TRUE;
				else ptext->m_bMirror = FALSE;
			}
			else ptext->m_bMirror = FALSE;			

			//True Type Font
			keyword = "USETTFONTS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	ptext->m_bUseTTFont = TRUE;
				else ptext->m_bUseTTFont = FALSE;
			}
			else ptext->m_bUseTTFont = FALSE;

			keyword = "FONTNAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csTTFontName = data;
			//Bold, Italic, Inverted
			keyword = "BOLD=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	ptext->m_bBold = TRUE;
				else ptext->m_bBold = FALSE;
			}
			else ptext->m_bBold = FALSE;
			keyword = "ITALIC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	ptext->m_bItalic = TRUE;
				else ptext->m_bItalic = FALSE;
			}
			else ptext->m_bItalic = FALSE;
			keyword = "INVERTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	ptext->m_bInverted = TRUE;
				else ptext->m_bInverted = FALSE;
			}
			else ptext->m_bInverted = FALSE;
			keyword = "INVERTEDTTTEXTBORDER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fInvertedBorder = atof(data);
			}

			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptext->m_bLocked = FALSE;
				else ptext->m_bLocked = TRUE;
			}
			else ptext->m_bLocked = FALSE;			

			//IndexForSave
			//keyword = "INDEXFORSAVE=";
			//if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nIndexForSave = atoi(data);
			//else ptext->m_nIndexForSave = -1;			
			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nUnionIndex = atoi(data);
			else ptext->m_nUnionIndex = -1;		
			//User Routed
			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptext->m_bUserRouted = FALSE;
				else ptext->m_bUserRouted = TRUE;
			}
			else ptext->m_bUserRouted = FALSE;	
			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptext->m_bPolygonOutline = FALSE;
				else ptext->m_bPolygonOutline = TRUE;
			}
			else ptext->m_bPolygonOutline = FALSE;

			m_arrayPcbText.Add(ptext);
		}
		else if(data == "Fill")
		{
			COb_pcbfill* pfill = new COb_pcbfill();
			pfill->pDocument = this;

			//m_fRotation,m_fX1, m_fY1, m_fX2, m_fY2;
			keyword = "X1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pfill->m_fX1 = atof(data);
			}
			keyword = "Y1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pfill->m_fY1 = atof(data);
			}
			keyword = "X2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pfill->m_fX2 = atof(data);
			}
			keyword = "Y2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pfill->m_fY2 = atof(data);
			}
			keyword = "ROTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pfill->m_fRotation = atof(data);
			else pfill->m_fRotation = 0;

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pfill->m_nLayer = i;
						break;
					}
				}
			}
			else pfill->m_nLayer = 0;
			
			keyword = "NET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pfill->m_nNet = atoi(data);

			keyword = "COMPONENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pfill->m_nComponent = atoi(data);

			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pfill->m_bLocked = FALSE;
				else pfill->m_bLocked = TRUE;
			}
			//Keepout
			keyword = "KEEPOUT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pfill->m_bKeepout = FALSE;
				else pfill->m_bKeepout = TRUE;
			}

			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pfill->m_nUnionIndex = atoi(data);
			else pfill->m_nUnionIndex = -1;		
			//User Routed
			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pfill->m_bUserRouted = FALSE;
				else pfill->m_bUserRouted = TRUE;
			}
			else pfill->m_bUserRouted = FALSE;	
			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pfill->m_bPolygonOutline = FALSE;
				else pfill->m_bPolygonOutline = TRUE;
			}
			else pfill->m_bPolygonOutline = FALSE;

			m_arrayPcbFill.Add(pfill);
		}
		else if(data == "Region")
		{
			COb_pcbregion* pregion = new COb_pcbregion();
			pregion->pDocument = this;

			int vertex = 0;
			keyword = "MAINCONTOURVERTEXCOUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex = atoi(data);
			else vertex = 0;
			pregion->m_cVertex.SetSize(vertex);
			for(int i=0; i < vertex; i++)
			{
				char s[100];
				itoa(i, s, 10);

				keyword = CString("|VX") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_cVertex[i].fx = atof(data);

				keyword = CString("|VY") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_cVertex[i].fy = atof(data);

				keyword = CString("|CX") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_cVertex[i].cx = atof(data);
				keyword = CString("|CY") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_cVertex[i].cy = atof(data);

				keyword = CString("|SA") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_cVertex[i].sangle = atof(data);

				keyword = CString("|EA") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_cVertex[i].eangle = atof(data);

				keyword = CString("|R") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_cVertex[i].radius = atof(data);
			}

			int nHoleNum;
			keyword = "HOLECOUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) nHoleNum = atoi(data);
			else nHoleNum = 0;

			for(int i=0;i<nHoleNum; i++)
			{
				int holevertexcount = 0;
				char s[100];
				itoa(i, s, 10);

				keyword = CString("HOLE") + s;
				keyword += "VERTEXCOUNT=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) holevertexcount = atoi(data);

				CArray<Fpoint, Fpoint>* p_arrayHole = new CArray<Fpoint, Fpoint>;
				for(int j=0; j<holevertexcount; j++)
				{
					Fpoint fp;
					char ss[100];
					itoa(j, ss, 10);
					//每个节点X,Y
					keyword = CString("H") + s + "VX" + ss;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) fp.fx = atof(data)/10000;
					keyword = CString("H") + s + "VY" + ss;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) fp.fy = atof(data)/10000;

					p_arrayHole->Add(fp);
				}

				pregion->m_arrayHoles.Add(p_arrayHole);
			}

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pregion->m_nLayer = i;
						break;
					}
				}
			}
			else pregion->m_nLayer = 0;

			keyword = "NET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_nNet = atoi(data);

			keyword = "COMPONENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_nComponent = atoi(data);

			keyword = "POLYGON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_nPolygon = atoi(data);

			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pregion->m_bLocked = FALSE;
				else pregion->m_bLocked = TRUE;
			}
			else pregion->m_bLocked = FALSE;			
			//Polygon Cutout
			keyword = "KIND=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "1")	pregion->m_bPolygonCutout = TRUE;
				else pregion->m_bPolygonCutout = FALSE;
			}
			//Board Cutout
			keyword = "ISBOARDCUTOUT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pregion->m_bBoardCutout = TRUE;
				else pregion->m_bBoardCutout = FALSE;
			}
			if(pregion->m_bBoardCutout == TRUE) pregion->m_nLayer = MULTILAYER;

			//keepout
			keyword = "KEEPOUT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pregion->m_bKeepout = FALSE;
				else pregion->m_bKeepout = TRUE;
			}

			//IndexForSave
			//keyword = "INDEXFORSAVE=";
			//if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_nIndexForSave = atoi(data);
			//else pregion->m_nIndexForSave = -1;			
			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_nUnionIndex = atoi(data);
			else pregion->m_nUnionIndex = -1;		
			//SubPolyIndex
			keyword = "SUBPOLYINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_nSubPolyIndex = atoi(data);
			else pregion->m_nSubPolyIndex = -1;
			//User Routed
			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pregion->m_bUserRouted = FALSE;
				else pregion->m_bUserRouted = TRUE;
			}
			else pregion->m_bUserRouted = FALSE;	
			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pregion->m_bPolygonOutline = FALSE;
				else pregion->m_bPolygonOutline = TRUE;
			}
			else pregion->m_bPolygonOutline = FALSE;

			m_arrayPcbRegion.Add(pregion);
		}
	}

	InitLayerShowArray();
}

void CESDDoc::Open_99_PCB_ASCII_File(CArchive& ar, CString& firstline)
{
	int total_linenum;
	CString onelinestr;

	CStringArray stringarray;

	stringarray.Add(firstline);

	while(ar.ReadString(onelinestr) == TRUE)
	{
		stringarray.Add(onelinestr);
	}

	total_linenum = stringarray.GetSize();


	CArray <POSITION, POSITION&> posArray;
	posArray.RemoveAll();
	posArray.SetSize(total_linenum);

	
	CString cstring, keyword,data;

	for(int linenum=0; linenum < total_linenum; linenum++)
	{
		cstring = stringarray.GetAt(linenum);

		keyword = "RECORD=";
		if(Get_KeyWord_Data(cstring, keyword, data) == FALSE) continue;
		if(data == "Board")
		{
			keyword = "ORIGINX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fOriginX = atof(data);
			}
			keyword = "ORIGINY=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fOriginY = atof(data);
			}
			keyword = "BIGVISIBLEGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				//data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fBigVisibleGridSize = atof(data)/10000;
			}
			keyword = "|VISIBLEGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				//data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fVisibleGridSize = atof(data)/10000;
			}			
			keyword = "ELECTRICALGRIDRANGE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				m_cPCB_Sheet.m_fElecGridRange = atof(data);
			}

			keyword = "ELECTRICALGRIDENABLED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	m_cPCB_Sheet.m_bElecGridEnable = TRUE;
				else m_cPCB_Sheet.m_bElecGridEnable = FALSE;
			}
			keyword = "SNAPGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				m_cPCB_Sheet.m_fSnapGridSize = atof(data)/10000;
			}
			m_cPCB_Sheet.m_fSnapGridSizeX = m_cPCB_Sheet.m_fSnapGridSizeY = m_cPCB_Sheet.m_fSnapGridSize;

			keyword = "TRACKGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				m_cPCB_Sheet.m_fTrackGridSize = atof(data)/10000;
			}
			keyword = "VIAGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				m_cPCB_Sheet.m_fViaGridSize = atof(data)/10000;
			}
			keyword = "COMPONENTGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				m_cPCB_Sheet.m_fComponentGridSize = atof(data)/10000;
			}
			m_cPCB_Sheet.m_fComponentGridSizeX = m_cPCB_Sheet.m_fComponentGridSizeY = m_cPCB_Sheet.m_fComponentGridSize;

			keyword = "DOTGRID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	m_cPCB_Sheet.m_bDotGrid = TRUE;
				else m_cPCB_Sheet.m_bDotGrid = FALSE;
			}

			keyword = "DISPLAYUNIT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_cPCB_Sheet.m_nUnit = atoi(data);

			for(int lay=0; lay<16; lay++)
			{
				char layer_num[100];
				itoa(lay+1, layer_num, 10);
				CString str1, str2;

				str1 = "PLANE";
				str2 = "NETNAME=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
				{
					if(data != "(No Net)") m_csPowerPlane[lay] = data;
				}
			}


			for(int lay=0; lay<PCB_LAYER_NUM; lay++)
			{
				char layer_num[100];
				itoa(lay+1, layer_num, 10);
				CString str1, str2;

				str1 = "LAYER";
				str2 = "NAME=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
				{
					m_sPCB_LayerParam[lay].csLayerName = data;
				}

				str2 = "PREV=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	m_sPCB_LayerParam[lay].nLayerPrev = atoi(data) - 1;

				str2 = "NEXT=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	m_sPCB_LayerParam[lay].nLayerNext = atoi(data) - 1;
				//Mech Enable
				str2 = "MECHENABLED=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	
				{
					if(data == "FALSE")	m_sPCB_LayerParam[lay].bLayerMechEnabled = FALSE;
					else m_sPCB_LayerParam[lay].bLayerMechEnabled = TRUE;
				}
				//Sheet Linked
				str2 = "SHEETLINKED=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	
				{
					if(data == "FALSE")	m_sPCB_LayerParam[lay].bLayerSheetLinked = FALSE;
					else m_sPCB_LayerParam[lay].bLayerSheetLinked = TRUE;
				}

				str2 = "COPTHICK=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	m_sPCB_LayerParam[lay].csLayerCopThick = data;
				str2 = "DIELTYPE=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	m_sPCB_LayerParam[lay].csLayerDielType = data;
				str2 = "DIELCONST=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	m_sPCB_LayerParam[lay].csLayerDielConst = data;
				str2 = "DIELHEIGHT=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	m_sPCB_LayerParam[lay].csLayerDielHeight = data;
				str2 = "DIELMATERIAL=";
				keyword = str1 + layer_num + str2;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)	m_sPCB_LayerParam[lay].csLayerDielMaterial = data;
			}

			if(bEDA_ConvertEnglishLayerName == TRUE) 
			{
				Convert_LayerNameToChinese();
			}

			keyword = "TOGGLELAYERS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data.GetLength() == 42) 
				{
					int s = 0;
					for(int i=0; i<15; i++)
					{
						if(data.GetAt(i) == '0') m_sPCB_LayerParam[s].bLayerShow = FALSE;
						else m_sPCB_LayerParam[s].bLayerShow = TRUE;
						s++;
					}
					s+=16;
					for(int i=15; i<26; i++)  
					{
						if(data.GetAt(i) == '0') m_sPCB_LayerParam[s].bLayerShow = FALSE;
						else m_sPCB_LayerParam[s].bLayerShow = TRUE;
						s++;
					}
					s+=12;
					for(int i=26; i<32; i++)  
					{
						if(data.GetAt(i) == '0') m_sPCB_LayerParam[s].bLayerShow = FALSE;
						else m_sPCB_LayerParam[s].bLayerShow = TRUE;
						s++;
					}
					s+=12;
					for(int i=32; i<42; i++)  
					{
						if(data.GetAt(i) == '0') m_sPCB_LayerParam[s].bLayerShow = FALSE;
						else m_sPCB_LayerParam[s].bLayerShow = TRUE;
						s++;
					}
				}
				else if(data.GetLength() == PCB_LAYER_NUM)
				{
					for(int i=0; i<PCB_LAYER_NUM; i++)
					{
						if(data.GetAt(i) == '0') m_sPCB_LayerParam[i].bLayerShow = FALSE;
						else m_sPCB_LayerParam[i].bLayerShow = TRUE;
					}				
				}
			}
			
		}
		else if(data == "EngineeringChangeOrderOptions")
		{
		}
		else if(data == "OutputOptions")
		{
		}
		else if(data == "PrinterOptions")
		{
		}
		else if(data == "GerberOptions")
		{
		}
		else if(data == "AdvancedPlacerOptions")
		{
		}
		else if(data == "DesignRuleCheckerOptions")
		{
		}		
		else if(data == "PinSwapOptions")
		{
		}		
		else if(data == "Class")
		{
		}		
		else if(data == "Net")
		{
			COb_pcbnet* pcbnet = new COb_pcbnet();			

			keyword = "|NAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				pcbnet->m_csNetName = data;
			}
			
			keyword = "VISIBLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pcbnet->m_bVisible = TRUE;
				else pcbnet->m_bVisible = FALSE;
			}

			m_arrayPcbNet.Add(pcbnet);
		}		
		else if(data == "Component")
		{
			COb_pcbcomp* pcbcomp = new COb_pcbcomp();
			pcbcomp->pDocument = this;
			
			keyword = "|X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcbcomp->m_fX = atof(data);
			}
			keyword = "|Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcbcomp->m_fY = atof(data);
			}
			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "TOP") pcbcomp->m_nLayer = TOPLAYER;
				else if(data == "BOTTOM") pcbcomp->m_nLayer = BOTTOMLAYER;
			}
			else pcbcomp->m_nLayer = TOPLAYER;
			//Height
			keyword = "HEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcbcomp->m_fHeight = atof(data);
			}

			keyword = "ROTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_fRotation = atof(data);
			else pcbcomp->m_fRotation = 0;
			
			//文字
			keyword = "|PATTERN=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csPattern = data;
			else pcbcomp->m_csPattern.Empty();		
			keyword = "|SOURCEDESIGNATOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csSourceDesignator= data;
			else pcbcomp->m_csSourceDesignator.Empty();	
			keyword = "|SOURCEUNIQUEID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csSourceUniqueID = data;
			else pcbcomp->m_csSourceUniqueID.Empty();	
			keyword = "|SOURCEHIERARCHICALPATH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csSourceHierarchicalPath = data;
			else pcbcomp->m_csSourceHierarchicalPath.Empty();	
			keyword = "|SOURCEFOOTPRINTLIBRARY=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csSourceFootprintLibrary = data;
			else pcbcomp->m_csSourceFootprintLibrary.Empty();	
			keyword = "|SOURCECOMPONENTLIBRARY=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csSourceComponentLibrary = data;
			else pcbcomp->m_csSourceComponentLibrary.Empty();	
			keyword = "|SOURCELIBREFERENCE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csSourceLibReference = data;
			else pcbcomp->m_csSourceLibReference.Empty();	
			keyword = "|SOURCEDESCRIPTION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csSourceDescription = data;
			else pcbcomp->m_csSourceDescription.Empty();	
			keyword = "|FOOTPRINTDESCRIPTION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csFootprintDescription = data;
			else pcbcomp->m_csFootprintDescription.Empty();	
			keyword = "|DEFAULTPCB3DMODEL=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_csDefaultPcb3dModel = data;
			else pcbcomp->m_csDefaultPcb3dModel.Empty();	

			//Name On
			keyword = "NAMEON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bNameOn = FALSE;
				else pcbcomp->m_bNameOn = TRUE;
			}
			else pcbcomp->m_bNameOn = FALSE;	
			//Comment On
			keyword = "COMMENTON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bCommentOn = FALSE;
				else pcbcomp->m_bCommentOn = TRUE;
			}
			else pcbcomp->m_bCommentOn = FALSE;	
			
			//Name Auto Position
			keyword = "NAMEAUTOPOSITION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nNamePosition = atoi(data);
			else pcbcomp->m_nNamePosition = 0;
			//Comment Auto Position
			keyword = "COMMENTAUTOPOSITION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nCommentPosition = atoi(data);
			else pcbcomp->m_nCommentPosition = 0;	

			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bLocked = FALSE;
				else pcbcomp->m_bLocked = TRUE;
			}
			else pcbcomp->m_bLocked = FALSE;			
			keyword = "PRIMITIVELOCK=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bPrimitiveLock = FALSE;
				else pcbcomp->m_bPrimitiveLock = TRUE;
			}
			else pcbcomp->m_bPrimitiveLock = FALSE;	
			keyword = "LOCKSTRINGS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bLockStrings = FALSE;
				else pcbcomp->m_bLockStrings = TRUE;
			}
			else pcbcomp->m_bLockStrings = FALSE;				
			keyword = "ENABLEPINSWAPPING=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bEnablePinSwapping = FALSE;
				else pcbcomp->m_bEnablePinSwapping = TRUE;
			}
			else pcbcomp->m_bEnablePinSwapping = FALSE;
			keyword = "ENABLEPARTSWAPPING=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bEnablePartSwapping = FALSE;
				else pcbcomp->m_bEnablePartSwapping = TRUE;
			}
			else pcbcomp->m_bEnablePartSwapping = FALSE;

			keyword = "GROUPNUM=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nGroupNum = atoi(data);
			else pcbcomp->m_nGroupNum = 0;
			keyword = "COUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nCount = atoi(data);
			else pcbcomp->m_nCount = 0;
			keyword = "COMPONENTKIND=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nComponentKind = atoi(data);
			else pcbcomp->m_nComponentKind = 0;
			keyword = "COMPONENTKINDVERSION2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nComponentKindVersion2 = atoi(data);
			else pcbcomp->m_nComponentKindVersion2 = 0;
			keyword = "HANNELOFFSET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nChannelOffset = atoi(data);
			else pcbcomp->m_nChannelOffset = -1;

			//IndexForSave
			//keyword = "INDEXFORSAVE=";
			//if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nIndexForSave = atoi(data);
			//else pcbcomp->m_nIndexForSave = -1;			
			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcbcomp->m_nUnionIndex = atoi(data);
			else pcbcomp->m_nUnionIndex = -1;		
			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bPolygonOutline = FALSE;
				else pcbcomp->m_bPolygonOutline = TRUE;
			}
			else pcbcomp->m_bPolygonOutline = FALSE;
			//User Routed
			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcbcomp->m_bUserRouted = FALSE;
				else pcbcomp->m_bUserRouted = TRUE;
			}
			else pcbcomp->m_bUserRouted = FALSE;				

			m_arrayPcbComp.Add(pcbcomp);

		}		
		else if(data == "Polygon")
		{
		}	
		else if(data == "Dimension")
		{
			COb_pcbdimension* pdimension = new COb_pcbdimension();
			pdimension->pDocument = this;

			//m_fXs, m_fYs, m_fXe, m_fYe, m_fHeight, m_fWidth
			keyword = "|X1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fXs = atof(data);
			}
			keyword = "|Y1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fYs = atof(data);
			}
			keyword = "|X2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fXe = atof(data);
			}
			keyword = "|Y2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fYe = atof(data);
			}
			keyword = "HEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fHeight = atof(data);
			}
			keyword = "LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fLineWidth = atof(data);
			}
			//m_fSx, m_fSy, m_fTextWidth, m_fTextHeight
			keyword = "TEXT1X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextSx = atof(data);
			}
			keyword = "TEXT1Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextSy = atof(data);
			}
			keyword = "TEXTHEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextHeight = atof(data);
			}
			keyword = "TEXTWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextLineWidth = atof(data);
			}
			keyword = "TEXT1ANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_fRotation = atof(data);
			else pdimension->m_fRotation = 0;

			//Stroke Font
			keyword = "FONT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				for(int i=0; i<3; i++)
				{
					if( data == csPCB_StrokeFontName[i] )
					{
						pdimension->m_nStrokeFont = i;
						break;
					}
				}
			}
			else pdimension->m_nStrokeFont = 0;

			keyword = "DIMENSIONLAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pdimension->m_nLayer = i;
						break;
					}
				}
			}
			else pdimension->m_nLayer = 0;

			keyword = "STYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Brackets") pdimension->m_nUnitStyle = UNIT_STYLE_BRACKETS;
				else if(data == "Normal") pdimension->m_nUnitStyle = UNIT_STYLE_NORMAL;
				else pdimension->m_nUnitStyle = UNIT_STYLE_NONE;
			}

			keyword = "DIMENSIONKIND=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 	pdimension->m_nDimensionKind = atoi(data);

			//True Type Font
			keyword = "USETTFONTS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pdimension->m_bUseTTFont = TRUE;
				else pdimension->m_bUseTTFont = FALSE;
			}
			else pdimension->m_bUseTTFont = FALSE;

			keyword = "FONTNAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_csTTFontName = data;
			//Bold, Italic, Inverted
			keyword = "BOLD=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pdimension->m_bBold = TRUE;
				else pdimension->m_bBold = FALSE;
			}
			else pdimension->m_bBold = FALSE;
			keyword = "ITALIC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pdimension->m_bItalic = TRUE;
				else pdimension->m_bItalic = FALSE;
			}
			else pdimension->m_bItalic = FALSE;

			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bLocked = FALSE;
				else pdimension->m_bLocked = TRUE;
			}
			//Primitive Lock
			keyword = "PRIMITIVELOCK=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bPrimitiveLock = FALSE;
				else pdimension->m_bPrimitiveLock = TRUE;
			}
	
			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_nUnionIndex = atoi(data);
			else pdimension->m_nUnionIndex = -1;		
			//User Routed
			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bUserRouted = FALSE;
				else pdimension->m_bUserRouted = TRUE;
			}
			else pdimension->m_bUserRouted = FALSE;	
			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bPolygonOutline = FALSE;
				else pdimension->m_bPolygonOutline = TRUE;
			}
			else pdimension->m_bPolygonOutline = FALSE;

			m_arrayPcbDimension.Add(pdimension);
		}
		else if(data == "Coordinate")
		{
			COb_pcbcoordinate* pcoordinate = new COb_pcbcoordinate();
			pcoordinate->pDocument = this;

			//m_fSx, m_fSy;m_fHeight;m_fWidth;m_fRotation;m_fX1, m_fY1, m_fX2, m_fY2;
			keyword = "|X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcoordinate->m_fX = atof(data);
			}
			keyword = "|Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcoordinate->m_fY = atof(data);
			}
			keyword = "SIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcoordinate->m_fSize = atof(data);
			}
			keyword = "LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcoordinate->m_fLineWidth = atof(data);
			}
			keyword = "TEXTHEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcoordinate->m_fTextHeight = atof(data);
			}
			keyword = "TEXTWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pcoordinate->m_fTextWidth = atof(data);
			}
			keyword = "ROTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcoordinate->m_fRotation = atof(data);
			else pcoordinate->m_fRotation = 0;

			//Stroke Font
			keyword = "FONT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				for(int i=0; i<3; i++)
				{
					if( data == csPCB_StrokeFontName[i] )
					{
						pcoordinate->m_nStrokeFont = i;
						break;
					}
				}
			}
			else pcoordinate->m_nStrokeFont = 0;

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pcoordinate->m_nLayer = i;
						break;
					}
				}
			}
			else pcoordinate->m_nLayer = 0;

			keyword = "STYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Brackets") pcoordinate->m_nUnitStyle = UNIT_STYLE_BRACKETS;
				else if(data == "Normal") pcoordinate->m_nUnitStyle = UNIT_STYLE_NORMAL;
				else pcoordinate->m_nUnitStyle = UNIT_STYLE_NONE;
			}
			
			//True Type Font
			keyword = "USETTFONTS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pcoordinate->m_bUseTTFont = TRUE;
				else pcoordinate->m_bUseTTFont = FALSE;
			}
			else pcoordinate->m_bUseTTFont = FALSE;

			keyword = "FONTNAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcoordinate->m_csTTFontName = data;
			//Bold, Italic, Inverted
			keyword = "BOLD=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pcoordinate->m_bBold = TRUE;
				else pcoordinate->m_bBold = FALSE;
			}
			else pcoordinate->m_bBold = FALSE;
			keyword = "ITALIC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pcoordinate->m_bItalic = TRUE;
				else pcoordinate->m_bItalic = FALSE;
			}
			else pcoordinate->m_bItalic = FALSE;


			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcoordinate->m_bLocked = FALSE;
				else pcoordinate->m_bLocked = TRUE;
			}
			//Primitive Lock
			keyword = "PRIMITIVELOCK=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcoordinate->m_bPrimitiveLock = FALSE;
				else pcoordinate->m_bPrimitiveLock = TRUE;
			}
	
			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcoordinate->m_nUnionIndex = atoi(data);
			else pcoordinate->m_nUnionIndex = -1;		
			//User Routed
			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcoordinate->m_bUserRouted = FALSE;
				else pcoordinate->m_bUserRouted = TRUE;
			}
			else pcoordinate->m_bUserRouted = FALSE;	
			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pcoordinate->m_bPolygonOutline = FALSE;
				else pcoordinate->m_bPolygonOutline = TRUE;
			}
			else pcoordinate->m_bPolygonOutline = FALSE;

			m_arrayPcbCoordinate.Add(pcoordinate);
		}
		else if(data == "DXPRule")
		{
		}
		else if(data == "Rule")
		{
		}
		else if(data == "Embedded")
		{
			COb_pcbdimension* pdimension = new COb_pcbdimension();
			pdimension->pDocument = this;

			keyword = "|X1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fXs = atof(data);
			}
			keyword = "|Y1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fYs = atof(data);
			}
			keyword = "|X2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fXe = atof(data);
			}
			keyword = "|Y2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fYe = atof(data);
			}
			
			keyword = "ANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_fRotation = atof(data);
			else pdimension->m_fRotation = 0;

			keyword = "ARROWSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fArrowSize = atof(data);
			}
			keyword = "ARROWLINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fArrowLineWidth = atof(data);
			}
			keyword = "ARROWLENGTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fArrowLength = atof(data);
			}
			keyword = "ARROWPOSITION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Inside") pdimension->m_nArrowPosition = 0;
				else if(data == "Outside") pdimension->m_nArrowPosition = 1;
			}

			keyword = "EXTENSIONOFFSET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fExtensionOffset = atof(data);
			}
			keyword = "EXTENSIONLINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fExtensionLineWidth = atof(data);
			}
			keyword = "EXTENSIONPICKGAP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fExtensionPickGap = atof(data);
			}

			keyword = "TEXTHEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextHeight = atof(data);
			}
			keyword = "TEXTLINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextLineWidth = atof(data);
			}
			keyword = "TEXTPOSITION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Auto") pdimension->m_nTextPosition = TEXTPOSITION_AUTO;
				else if(data == "Center") pdimension->m_nTextPosition = TEXTPOSITION_CENTER;
				else if(data == "Top") pdimension->m_nTextPosition = TEXTPOSITION_TOP;
				else if(data == "Bottom") pdimension->m_nTextPosition = TEXTPOSITION_BOTTOM;
				else if(data == "Right") pdimension->m_nTextPosition = TEXTPOSITION_RIGHT;
				else if(data == "Left") pdimension->m_nTextPosition = TEXTPOSITION_LEFT;
				else if(data == "InsideRight") pdimension->m_nTextPosition = TEXTPOSITION_INSIDERIGHT;
				else if(data == "InsideLeft") pdimension->m_nTextPosition = TEXTPOSITION_INSIDELEFT;
				else if(data == "UniDirectional") pdimension->m_nTextPosition = TEXTPOSITION_UNIDIRECTIONAL;
				else if(data == "Manual") pdimension->m_nTextPosition = TEXTPOSITION_MANUAL;
			}			
			keyword = "TEXTGAP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextGap = atof(data);
			}
			keyword = "TEXTFORMAT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "10") pdimension->m_nTextFormat = 1;				
				else if(data == "10mil") pdimension->m_nTextFormat = 2;		
				else if(data == "10 (mil)") pdimension->m_nTextFormat = 3;	
				else pdimension->m_nTextFormat = 0;							
			}
			keyword = "TEXTDIMENSIONUNIT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Mils") pdimension->m_nTextDimensionUnit = UNIT_MIL;
				else if(data == "Millimeters") pdimension->m_nTextDimensionUnit = UNIT_MM;
				else if(data == "Inches") pdimension->m_nTextDimensionUnit = UNIT_INCH;
				else if(data == "Centimeters") pdimension->m_nTextDimensionUnit = UNIT_CM;
				else if(data == "AutomaticUnit") pdimension->m_nTextDimensionUnit = UNIT_AUTO;
			}
			keyword = "TEXTPRECISION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_nTextPrecision = atoi(data);

			keyword = "TEXTPREFIX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_csPrefix = data;
			keyword = "TEXTSUFFIX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_csSuffix = data;

			keyword = "TEXT1X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextSx = atof(data);
			}
			keyword = "TEXT1Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextSy = atof(data);
			}
			keyword = "TEXTHEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pdimension->m_fTextHeight = atof(data);
			}

			keyword = "TEXT1ANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_fTextRotation = atof(data);
			else pdimension->m_fTextRotation = 0;

			int vertex = 0;
			keyword = "REFERENCES_COUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex = atoi(data);
			else vertex = 0;
			pdimension->m_cRefPoint.SetSize(vertex);
			for(int i=0; i < vertex; i++)
			{
				char s[100];
				itoa(i, s, 10);

				keyword = CString("REFERENCE") + s + CString("POINTX");
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
				{
					data = data.TrimRight("mil");
					pdimension->m_cRefPoint[i].fx = atof(data);
				}

				keyword = CString("REFERENCE") + s + CString("POINTY");
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
				{
					data = data.TrimRight("mil");
					pdimension->m_cRefPoint[i].fy = atof(data);
				}
			}

			//Stroke Font
			keyword = "FONT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				for(int i=0; i<3; i++)
				{
					if( data == csPCB_StrokeFontName[i] )
					{
						pdimension->m_nStrokeFont = i;
						break;
					}
				}
			}
			else pdimension->m_nStrokeFont = 0;

			keyword = "DIMENSIONLAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pdimension->m_nLayer = i;
						break;
					}
				}
			}
			else pdimension->m_nLayer = 0;

			keyword = "STYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "Brackets") pdimension->m_nUnitStyle = UNIT_STYLE_BRACKETS;
				else if(data == "Normal") pdimension->m_nUnitStyle = UNIT_STYLE_NORMAL;
				else pdimension->m_nUnitStyle = UNIT_STYLE_NONE;
			}

			keyword = "DIMENSIONKIND=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 	pdimension->m_nDimensionKind = atoi(data);

			//True Type Font
			keyword = "USETTFONTS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pdimension->m_bUseTTFont = TRUE;
				else pdimension->m_bUseTTFont = FALSE;
			}
			else pdimension->m_bUseTTFont = FALSE;

			keyword = "FONTNAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_csTTFontName = data;
			//Bold, Italic, Inverted
			keyword = "BOLD=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pdimension->m_bBold = TRUE;
				else pdimension->m_bBold = FALSE;
			}
			else pdimension->m_bBold = FALSE;
			keyword = "ITALIC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pdimension->m_bItalic = TRUE;
				else pdimension->m_bItalic = FALSE;
			}
			else pdimension->m_bItalic = FALSE;


			//Selection
			/*keyword = "SELECTION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bSelection = FALSE;
				else pdimension->m_bSelection = TRUE;
			}
			else pdimension->m_bSelection = FALSE;*/

			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bLocked = FALSE;
				else pdimension->m_bLocked = TRUE;
			}
			//Primitive Lock
			keyword = "PRIMITIVELOCK=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bPrimitiveLock = FALSE;
				else pdimension->m_bPrimitiveLock = TRUE;
			}

			//IndexForSave
			//keyword = "INDEXFORSAVE=";
			//if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_nIndexForSave = atoi(data);
			//else pdimension->m_nIndexForSave = -1;			
			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pdimension->m_nUnionIndex = atoi(data);
			else pdimension->m_nUnionIndex = -1;		
			//User Routed
			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bUserRouted = FALSE;
				else pdimension->m_bUserRouted = TRUE;
			}
			else pdimension->m_bUserRouted = FALSE;	
			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pdimension->m_bPolygonOutline = FALSE;
				else pdimension->m_bPolygonOutline = TRUE;
			}
			else pdimension->m_bPolygonOutline = FALSE;

			m_arrayPcbDimension.Add(pdimension);
		}
		else if(data == "Arc")
		{
			COb_pcbarc* parc = new COb_pcbarc();
			parc->pDocument = this;

			keyword = "NET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nNet = atoi(data);

			keyword = "COMPONENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nComponent = atoi(data);

			keyword = "POLYGON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nPolygon = atoi(data);
			else parc->m_nPolygon = -1;

			keyword = "WIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				parc->m_fWidth = atof(data);
			}

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						parc->m_nLayer = i;
						break;
					}
				}
			}
			else parc->m_nLayer = 0;

			keyword = "LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				parc->m_fCx = atof(data);
			}
			keyword = "LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				parc->m_fCy = atof(data);
			}

			keyword = "RADIUS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				parc->m_fRadius = atof(data);
			}

			keyword = "STARTANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_fSangle = atof(data);
			else parc->m_fSangle = 0;				
			keyword = "ENDANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_fEangle = atof(data);
			else parc->m_fEangle = 0;

			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	parc->m_bLocked = FALSE;
				else parc->m_bLocked = TRUE;
			}
			else parc->m_bLocked = FALSE;			
			//Keepout
			keyword = "KEEPOUT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	parc->m_bKeepout = FALSE;
				else parc->m_bKeepout = TRUE;
			}
			else parc->m_bKeepout = FALSE;
			
			//IndexForSave
			//keyword = "INDEXFORSAVE=";
			//if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nIndexForSave = atoi(data);
			//else parc->m_nIndexForSave = -1;			
			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nUnionIndex = atoi(data);
			else parc->m_nUnionIndex = -1;		
			//SubPolyIndex
			keyword = "SUBPOLYINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nSubPolyIndex = atoi(data);
			else parc->m_nSubPolyIndex = -1;
			//User Routed
/*			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	parc->m_bUserRouted = FALSE;
				else parc->m_bUserRouted = TRUE;
			}
			else parc->m_bUserRouted = FALSE;*/
			//TearDrop
			keyword = "TEARDROP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	parc->m_bTearDrop = FALSE;
				else parc->m_bTearDrop = TRUE;
			}
			else parc->m_bTearDrop = FALSE;

			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	parc->m_bPolygonOutline = FALSE;
				else parc->m_bPolygonOutline = TRUE;
			}
			else parc->m_bPolygonOutline = FALSE;

			m_arrayPcbArc.Add(parc);
		}
		else if(data == "Pad")
		{
			COb_pcbpad* ppad = new COb_pcbpad();
			ppad->pDocument = this;

			keyword = "NET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nNet = atoi(data);

			keyword = "COMPONENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nComponent = atoi(data);


			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						ppad->m_nLayer = i;
						break;
					}
				}
			}
			else ppad->m_nLayer = MULTILAYER;

			keyword = "NAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_csName = data;
			else ppad->m_csName.Empty();

			keyword = "|X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCx = atof(data);
			}
			keyword = "|Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCy = atof(data);
			}

			keyword = "ROTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_fRotation = atof(data);
			else ppad->m_fRotation = 0;

			keyword = "PADMODE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nPadMode = atoi(data);
			else ppad->m_nPadMode = PAD_MODE_SIMPLE;
			if(ppad->m_nPadMode == PAD_MODE_FULLSTACK) ppad->m_nPadMode = PAD_MODE_T_M_B; 


			keyword = "|XSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				for(int itemp = 0; itemp < 32; itemp++)
				{
					ppad->m_fSizeX[itemp] = atof(data);
				}
			}
			keyword = "|YSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				for(int itemp = 0; itemp < 32; itemp++)
				{
					ppad->m_fSizeY[itemp] = atof(data);
				}
			}
			keyword = "|SHAPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				int shape;
				for(int itemp=0; itemp<3; itemp++)
				{
					if(data == csPCB_PadShape[itemp]) shape = itemp;
				}
				for(int itemp = 0; itemp < 32; itemp++)
				{
					ppad->m_nShape[itemp] = shape;
				}
			}

			keyword = "|TOPXSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fSizeX[TOPLAYER] = atof(data);
			}
			keyword = "|TOPYSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fSizeY[TOPLAYER] = atof(data);
			}
			keyword = "|BOTXSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fSizeX[BOTTOMLAYER] = atof(data);
			}
			keyword = "|BOTYSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fSizeY[BOTTOMLAYER] = atof(data);
			}
			keyword = "|TOPSHAPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int itemp=0; itemp<3; itemp++)
				{
					if(data == csPCB_PadShape[itemp]) ppad->m_nShape[TOPLAYER] = itemp;
				}
			}
			keyword = "|BOTSHAPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int itemp=0; itemp<3; itemp++)
				{
					if(data == csPCB_PadShape[itemp]) ppad->m_nShape[BOTTOMLAYER] = itemp;
				}
			}			
			
			keyword = "|MIDXSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				for(int itemp = 1; itemp < 31; itemp++)
				{
					ppad->m_fSizeX[itemp] = atof(data);
				}
			}
			keyword = "|MIDYSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				for(int itemp = 1; itemp < 31; itemp++)
				{
					ppad->m_fSizeY[itemp] = atof(data);
				}
			}
			keyword = "|MIDSHAPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				int shape;
				for(int itemp=0; itemp<3; itemp++)
				{
					if(data == csPCB_PadShape[itemp]) shape = itemp;
				}
				for(int itemp = 1; itemp < 31; itemp++)
				{
					ppad->m_nShape[itemp] = shape;
				}

			}


			keyword = "HOLESIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fHoleDia = atof(data);
			}

			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bLocked = FALSE;
				else ppad->m_bLocked = TRUE;
			}
			else ppad->m_bLocked = FALSE;			
						
			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nUnionIndex = atoi(data);
			else ppad->m_nUnionIndex = -1;		
			//User Routed
			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bUserRouted = FALSE;
				else ppad->m_bUserRouted = TRUE;
			}
			else ppad->m_bUserRouted = FALSE;		
			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bPolygonOutline = FALSE;
				else ppad->m_bPolygonOutline = TRUE;
			}
			else ppad->m_bPolygonOutline = FALSE;
			
			//Tenting
			keyword = "TENTING=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bTenting = FALSE;
				else ppad->m_bTenting = TRUE;
			}
			else ppad->m_bTenting = FALSE;
			keyword = "TENTINGTOP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bTentingTop = FALSE;
				else ppad->m_bTentingTop = TRUE;
			}
			else ppad->m_bTentingTop = FALSE;
			keyword = "TENTINGBOTTOM=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bTentingBottom = FALSE;
				else ppad->m_bTentingBottom = TRUE;
			}
			else ppad->m_bTentingBottom = FALSE;
			//Testing
			keyword = "TESTPOINT_TOP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bTestingTop = FALSE;
				else ppad->m_bTestingTop = TRUE;
			}
			else ppad->m_bTestingTop = FALSE;
			keyword = "TESTPOINT_BOTTOM=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ppad->m_bTestingBottom = FALSE;
				else ppad->m_bTestingBottom = TRUE;
			}
			else ppad->m_bTestingBottom = FALSE;

			//?
			keyword = "CPLV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCPLV = atoi(data);
			keyword = "CPL=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCPL = atoi(data);

			keyword = "CPEV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCPEV = atoi(data);
			keyword = "CPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCPE = atof(data);
			}			

			keyword = "CSEV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCSEV = atoi(data);
			keyword = "CSE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCSE = atof(data);
			}

			keyword = "CPCV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCPCV = atoi(data);
			keyword = "CPC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCPC = atof(data);
			}

			keyword = "CCSV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCCSV = atoi(data);
			keyword = "CCS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				int i;
				for(i=0; i<3; i++)
				{
					if(data == csPCB_PlaneStyle[i]) break;
				}
				if(i < 3) ppad->m_nCCS = i;
			}
			keyword = "CENV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCENV = atoi(data);
			keyword = "CEN=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCEN = atoi(data);

			keyword = "CCWV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCCWV = atoi(data);
			keyword = "CCW=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCCW = atof(data);
			}

			keyword = "CAGV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCAGV = atoi(data);
			keyword = "CAG=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCAG = atof(data);
			}

			keyword = "CPRV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppad->m_nCPRV = atoi(data);
			keyword = "CPR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ppad->m_fCPR = atof(data);
			}

			m_arrayPcbPad.Add(ppad);
		}
		else if(data == "Via")
		{
			COb_pcbvia* pvia = new COb_pcbvia();
			pvia->pDocument = this;

			keyword = "NET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nNet = atoi(data);

			keyword = "COMPONENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nComponent = atoi(data);

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pvia->m_nLayer = i;
						break;
					}
				}
			}
			else pvia->m_nLayer = MULTILAYER;

			keyword = "STARTLAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pvia->m_nStartLayer = i;
						break;
					}
				}
			}
			else pvia->m_nStartLayer = TOPLAYER;

			keyword = "ENDLAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pvia->m_nEndLayer = i;
						break;
					}
				}
			}
			else pvia->m_nEndLayer = BOTTOMLAYER;

			keyword = "|X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCx = atof(data);
			}
			keyword = "|Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCy = atof(data);
			}

			keyword = "DIAMETER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fDia = atof(data);
			}

			keyword = "HOLESIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fHoleDia = atof(data);
			}

			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bLocked = FALSE;
				else pvia->m_bLocked = TRUE;
			}
			else pvia->m_bLocked = FALSE;			
						
			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nUnionIndex = atoi(data);
			else pvia->m_nUnionIndex = -1;		
			//User Routed
			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bUserRouted = FALSE;
				else pvia->m_bUserRouted = TRUE;
			}
			else pvia->m_bUserRouted = FALSE;				
			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bPolygonOutline = FALSE;
				else pvia->m_bPolygonOutline = TRUE;
			}
			else pvia->m_bPolygonOutline = FALSE;
			
			//Tenting
			keyword = "TENTING=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bTenting = FALSE;
				else pvia->m_bTenting = TRUE;
			}
			else pvia->m_bTenting = FALSE;
			keyword = "TENTINGTOP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bTentingTop = FALSE;
				else pvia->m_bTentingTop = TRUE;
			}
			else pvia->m_bTentingTop = FALSE;
			keyword = "TENTINGBOTTOM=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bTentingBottom = FALSE;
				else pvia->m_bTentingBottom = TRUE;
			}
			else pvia->m_bTentingBottom = FALSE;
			//Testing
			keyword = "TESTPOINT_TOP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bTestingTop = FALSE;
				else pvia->m_bTestingTop = TRUE;
			}
			else pvia->m_bTestingTop = FALSE;
			keyword = "TESTPOINT_BOTTOM=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pvia->m_bTestingBottom = FALSE;
				else pvia->m_bTestingBottom = TRUE;
			}
			else pvia->m_bTestingBottom = FALSE;
	
			//?
			keyword = "CPLV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCPLV = atoi(data);
			keyword = "CPL=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCPL = atoi(data);

			/*keyword = "CPEV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCPEV = atoi(data);
			keyword = "CPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCPE = atof(data);
			}*/			

			keyword = "CSEV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCSEV = atoi(data);
			keyword = "CSE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCSE = atof(data);
			}

			keyword = "CPCV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCPCV = atoi(data);
			keyword = "CPC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCPC = atof(data);
			}

			keyword = "CCSV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCCSV = atoi(data);
			keyword = "CCS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				int i;
				for(i=0; i<3; i++)
				{
					if(data == csPCB_PlaneStyle[i]) break;
				}
				if(i < 3) pvia->m_nCCS = i;
			}
			keyword = "CENV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCENV = atoi(data);
			keyword = "CEN=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCEN = atoi(data);

			keyword = "CCWV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCCWV = atoi(data);
			keyword = "CCW=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCCW = atof(data);
			}

			keyword = "CAGV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCAGV = atoi(data);
			keyword = "CAG=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCAG = atof(data);
			}

			keyword = "CPRV=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pvia->m_nCPRV = atoi(data);
			keyword = "CPR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pvia->m_fCPR = atof(data);
			}

			m_arrayPcbVia.Add(pvia);

		}
		else if(data == "Track")
		{
			COb_pcbtrack* ptrack = new COb_pcbtrack();
			ptrack->pDocument = this;
			
			keyword = "NET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptrack->m_nNet = atoi(data);

			keyword = "COMPONENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptrack->m_nComponent = atoi(data);

			keyword = "POLYGON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptrack->m_nPolygon = atoi(data);
			else ptrack->m_nPolygon = -1;

			keyword = "WIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptrack->m_fWidth = atof(data);
			}
			else ptrack->m_fWidth = 4;

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						ptrack->m_nLayer = i;
						break;
					}
				}
			}
			else ptrack->m_nLayer = 0;
			
			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptrack->m_bLocked = FALSE;
				else ptrack->m_bLocked = TRUE;
			}
			else ptrack->m_bLocked = FALSE;			
			//Keepout
			keyword = "KEEPOUT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptrack->m_bKeepout = FALSE;
				else ptrack->m_bKeepout = TRUE;
			}
			else ptrack->m_bKeepout = FALSE;

			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptrack->m_nUnionIndex = atoi(data);
			else ptrack->m_nUnionIndex = -1;		
			//SubPolyIndex
			keyword = "SUBPOLYINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptrack->m_nSubPolyIndex = atoi(data);
			else ptrack->m_nSubPolyIndex = -1;
			//User Routed

			//TearDrop
			keyword = "TEARDROP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptrack->m_bTearDrop = FALSE;
				else ptrack->m_bTearDrop = TRUE;
			}
			else ptrack->m_bTearDrop = FALSE;

			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptrack->m_bPolygonOutline = FALSE;
				else ptrack->m_bPolygonOutline = TRUE;
			}
			else ptrack->m_bPolygonOutline = FALSE;

			//X1,Y1,X2,Y2
			keyword = "X1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptrack->m_fX1 = atof(data);
			}
			else ptrack->m_fX1 = 0;
			keyword = "Y1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptrack->m_fY1 = atof(data);
			}
			else ptrack->m_fX2 = 0;
			keyword = "X2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptrack->m_fX2 = atof(data);
			}
			else ptrack->m_fY2 = 0;
			keyword = "Y2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptrack->m_fY2 = atof(data);
			}
			else ptrack->m_fX1 = 0;
					
			m_arrayPcbTrack.Add(ptrack);
		}
		else if(data == "Text")
		{
			COb_pcbtext* ptext = new COb_pcbtext();
			ptext->pDocument = this;

			//m_fSx, m_fSy;m_fHeight;m_fWidth;m_fRotation;m_fX1, m_fY1, m_fX2, m_fY2;
			keyword = "|X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fSx = atof(data);
			}
			keyword = "|Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fSy = atof(data);
			}
			keyword = "X1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fX1 = atof(data);
			}
			keyword = "Y1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fY1 = atof(data);
			}
			keyword = "X2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fX2 = atof(data);
			}
			keyword = "Y2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fY2 = atof(data);
			}
			keyword = "HEIGHT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fHeight = atof(data);
			}
			keyword = "WIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fWidth = atof(data);
			}
			keyword = "ROTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_fRotation = atof(data);
			else ptext->m_fRotation = 0;

			keyword = "TEXT=";
			if(Get_Text_Data(cstring, keyword, data) == TRUE) ptext->m_csText = data;
			else ptext->m_csText.Empty();

			//Stroke Font
			keyword = "FONT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				for(int i=0; i<3; i++)
				{
					if( data == csPCB_StrokeFontName[i] )
					{
						ptext->m_nStrokeFont = i;
						break;
					}
				}
			}
			else ptext->m_nStrokeFont = 0;


			keyword = "COMPONENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nComponent = atoi(data);

			keyword = "DESIGNATOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "True")	ptext->m_bDesignator = TRUE;
				else ptext->m_bDesignator = FALSE;
			}

			keyword = "COMMENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "True")	ptext->m_bComment = TRUE;
				else ptext->m_bComment = FALSE;
			}

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						ptext->m_nLayer = i;
						break;
					}
				}
			}
			else ptext->m_nLayer = 0;
			
			keyword = "MIRROR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	ptext->m_bMirror = TRUE;
				else ptext->m_bMirror = FALSE;
			}
			else ptext->m_bMirror = FALSE;			

			//True Type Font
			keyword = "USETTFONTS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	ptext->m_bUseTTFont = TRUE;
				else ptext->m_bUseTTFont = FALSE;
			}
			else ptext->m_bUseTTFont = FALSE;

			keyword = "FONTNAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csTTFontName = data;
			//Bold, Italic, Inverted
			keyword = "BOLD=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	ptext->m_bBold = TRUE;
				else ptext->m_bBold = FALSE;
			}
			else ptext->m_bBold = FALSE;
			keyword = "ITALIC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	ptext->m_bItalic = TRUE;
				else ptext->m_bItalic = FALSE;
			}
			else ptext->m_bItalic = FALSE;
			keyword = "INVERTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	ptext->m_bInverted = TRUE;
				else ptext->m_bInverted = FALSE;
			}
			else ptext->m_bInverted = FALSE;
			keyword = "INVERTEDTTTEXTBORDER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				ptext->m_fInvertedBorder = atof(data);
			}

			//Selection
			/*keyword = "SELECTION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptext->m_bSelection = FALSE;
				else ptext->m_bSelection = TRUE;
			}
			else ptext->m_bSelection = FALSE;*/

			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptext->m_bLocked = FALSE;
				else ptext->m_bLocked = TRUE;
			}
			else ptext->m_bLocked = FALSE;			

			//IndexForSave
			//keyword = "INDEXFORSAVE=";
			//if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nIndexForSave = atoi(data);
			//else ptext->m_nIndexForSave = -1;			
			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nUnionIndex = atoi(data);
			else ptext->m_nUnionIndex = -1;		
			//User Routed
			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptext->m_bUserRouted = FALSE;
				else ptext->m_bUserRouted = TRUE;
			}
			else ptext->m_bUserRouted = FALSE;	
			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	ptext->m_bPolygonOutline = FALSE;
				else ptext->m_bPolygonOutline = TRUE;
			}
			else ptext->m_bPolygonOutline = FALSE;

			m_arrayPcbText.Add(ptext);
		}
		else if(data == "Fill")
		{
			COb_pcbfill* pfill = new COb_pcbfill();
			pfill->pDocument = this;

			//m_fRotation,m_fX1, m_fY1, m_fX2, m_fY2;
			keyword = "X1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pfill->m_fX1 = atof(data);
			}
			keyword = "Y1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pfill->m_fY1 = atof(data);
			}
			keyword = "X2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pfill->m_fX2 = atof(data);
			}
			keyword = "Y2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				data = data.TrimRight("mil");
				pfill->m_fY2 = atof(data);
			}
			keyword = "ROTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pfill->m_fRotation = atof(data);
			else pfill->m_fRotation = 0;

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pfill->m_nLayer = i;
						break;
					}
				}
			}
			else pfill->m_nLayer = 0;
			
			keyword = "NET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pfill->m_nNet = atoi(data);

			keyword = "COMPONENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pfill->m_nComponent = atoi(data);


			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pfill->m_bLocked = FALSE;
				else pfill->m_bLocked = TRUE;
			}
			else pfill->m_bLocked = FALSE;			

			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pfill->m_nUnionIndex = atoi(data);
			else pfill->m_nUnionIndex = -1;		
			//User Routed
			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pfill->m_bUserRouted = FALSE;
				else pfill->m_bUserRouted = TRUE;
			}
			else pfill->m_bUserRouted = FALSE;	
			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pfill->m_bPolygonOutline = FALSE;
				else pfill->m_bPolygonOutline = TRUE;
			}
			else pfill->m_bPolygonOutline = FALSE;

			m_arrayPcbFill.Add(pfill);
		}
		else if(data == "Region")
		{
			COb_pcbregion* pregion = new COb_pcbregion();
			pregion->pDocument = this;

			int vertex = 0;
			keyword = "MAINCONTOURVERTEXCOUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex = atoi(data);
			else vertex = 0;
			pregion->m_cVertex.SetSize(vertex);
			for(int i=0; i < vertex; i++)
			{
				char s[100];
				itoa(i, s, 10);

				keyword = CString("|VX") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_cVertex[i].fx = atof(data);

				keyword = CString("|VY") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_cVertex[i].fy = atof(data);

				keyword = CString("|CX") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_cVertex[i].cx = atof(data);
				keyword = CString("|CY") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_cVertex[i].cy = atof(data);

				keyword = CString("|SA") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_cVertex[i].sangle = atof(data);

				keyword = CString("|EA") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_cVertex[i].eangle = atof(data);

				keyword = CString("|R") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_cVertex[i].radius = atof(data);
			}

			int nHoleNum;
			keyword = "HOLECOUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) nHoleNum = atoi(data);
			else nHoleNum = 0;

			for(int i=0;i<nHoleNum; i++)
			{
				int holevertexcount = 0;
				char s[100];
				itoa(i, s, 10);

				keyword = CString("HOLE") + s;
				keyword += "VERTEXCOUNT=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) holevertexcount = atoi(data);

				CArray<Fpoint, Fpoint>* p_arrayHole = new CArray<Fpoint, Fpoint>;
				for(int j=0; j<holevertexcount; j++)
				{
					Fpoint fp;
					char ss[100];
					itoa(j, ss, 10);

					keyword = CString("H") + s + "VX" + ss;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) fp.fx = atof(data)/10000;
					keyword = CString("H") + s + "VY" + ss;
					keyword += "=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) fp.fy = atof(data)/10000;

					p_arrayHole->Add(fp);
				}

				pregion->m_arrayHoles.Add(p_arrayHole);
			}

			keyword = "LAYER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				for(int i=0; i<=MULTILAYER; i++)
				{
					if(data == csEDA_PCB_LayerLabel[i])
					{
						pregion->m_nLayer = i;
						break;
					}
				}
			}
			else pregion->m_nLayer = 0;

			keyword = "NET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_nNet = atoi(data);

			keyword = "COMPONENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_nComponent = atoi(data);

			keyword = "POLYGON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_nPolygon = atoi(data);

			//Locked
			keyword = "LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pregion->m_bLocked = FALSE;
				else pregion->m_bLocked = TRUE;
			}
			else pregion->m_bLocked = FALSE;			
			//Polygon Cutout
			keyword = "KIND=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "1")	pregion->m_bPolygonCutout = TRUE;
				else pregion->m_bPolygonCutout = FALSE;
			}
			//Board Cutout
			keyword = "ISBOARDCUTOUT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "TRUE")	pregion->m_bBoardCutout = TRUE;
				else pregion->m_bBoardCutout = FALSE;
			}
			//keepout
			keyword = "KEEPOUT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pregion->m_bKeepout = FALSE;
				else pregion->m_bKeepout = TRUE;
			}

	
			//UnionIndex
			keyword = "UNIONINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_nUnionIndex = atoi(data);
			else pregion->m_nUnionIndex = -1;		
			//SubPolyIndex
			keyword = "SUBPOLYINDEX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pregion->m_nSubPolyIndex = atoi(data);
			else pregion->m_nSubPolyIndex = -1;
			//User Routed
			keyword = "USERROUTED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pregion->m_bUserRouted = FALSE;
				else pregion->m_bUserRouted = TRUE;
			}
			else pregion->m_bUserRouted = FALSE;	
			//Polygon Outline
			keyword = "POLYGONOUTLINE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE)
			{
				if(data == "FALSE")	pregion->m_bPolygonOutline = FALSE;
				else pregion->m_bPolygonOutline = TRUE;
			}
			else pregion->m_bPolygonOutline = FALSE;

			m_arrayPcbRegion.Add(pregion);
		}
	}

	InitLayerShowArray();

	double fmaxx, fmaxy, fminx, fminy;
	Get_Document_Maxy(&fmaxx, &fmaxy, &fminx, &fminy);

	m_cPCB_Sheet.m_cVertex.SetSize(5);
	m_cPCB_Sheet.m_cVertex[0].fx = fminx;
	m_cPCB_Sheet.m_cVertex[0].fy = fminy;
	m_cPCB_Sheet.m_cVertex[1].fx = fminx;
	m_cPCB_Sheet.m_cVertex[1].fy = fmaxy;
	m_cPCB_Sheet.m_cVertex[2].fx = fmaxx;
	m_cPCB_Sheet.m_cVertex[2].fy = fmaxy;
	m_cPCB_Sheet.m_cVertex[3].fx = fmaxx;
	m_cPCB_Sheet.m_cVertex[3].fy = fminy;
	m_cPCB_Sheet.m_cVertex[4].fx = fminx;
	m_cPCB_Sheet.m_cVertex[4].fy = fminy;

}

void CESDDoc::Open_AD6_SCH_Binary_File(CFile* myfile, int nCurrentDiagram)
{
	int filelength;
	filelength = myfile->GetLength();
	if(filelength <  0x1800) return;

	char* pbuffer = (char*)malloc(filelength);
	char* pbufferbak = (char*)malloc(filelength);

	myfile->Seek(512, CFile::begin);
	filelength -= 512;
	if(myfile->Read(pbuffer, filelength) < filelength )
	{
		AfxMessageBox("意外的数据格式错误，文件丢失部分数据，打开文档失败。");
		free(pbuffer);
		free(pbufferbak);
		return;	
	}
	int times = filelength/65536;

	char *temp, *tempbak;
	temp = pbuffer;
	tempbak = pbufferbak;
	int l=0;
	for(int i=0; i<times; i++)
	{
		temp += 512;
		memcpy(tempbak, temp, (65536-512));
		temp += (65536-512);
		tempbak += (65536-512);
		l += (65536-512);
	}
	memcpy(tempbak, temp, (filelength-times*65536));
	l += (filelength-times*65536);


	temp = pbuffer;
	tempbak = pbufferbak;
	tempbak += 0x1400;
	l -= 0x1400;
	memcpy(temp, tempbak, l);
	

	temp = pbuffer;

	int length;
	memcpy(&length, temp, 4);
	temp += 4;

	char str[1000];
	if(length < 999)
	{
		memcpy(str, temp, length);
		temp += length;
		str[length] = '\0';
	}
	CString onelinestr = str;

	if(onelinestr.Find("|HEADER=Protel for Windows - Schematic Capture Binary File Version 5.0") < 0)
	{
		AfxMessageBox("数据格式无法识别，打开文档失败。");
		free(pbuffer);
		free(pbufferbak);
		return;			
	}

	int total_linenum;
	int pos = onelinestr.ReverseFind('=');
	if(pos >= 0)
	{
		pos = onelinestr.GetLength() - pos - 1;				
		onelinestr = onelinestr.Right(pos);
		total_linenum = atoi(onelinestr);  
	}
	else total_linenum = 0;
	

	CStringArray stringarray;
	stringarray.SetSize(total_linenum);

	for(int i=0; i<total_linenum; i++)
	{
		for(int j=0;j<1000;j++)
		{
			if((temp[j] == 'R')&&(temp[j+1] == 'E')&&(temp[j+2] == 'C')&&(temp[j+3] == 'O')&&(temp[j+4] == 'R')&&(temp[j+5] == 'D')&&(temp[j+6] == '='))
			{
				if(temp[j-1] == '|') temp += j-1;
				else temp += j;
				break;
			}
		}
		
		for(int j=10;j<1000;j++)
		{
			if((temp[j] == 'R')&&(temp[j+1] == 'E')&&(temp[j+2] == 'C')&&(temp[j+3] == 'O')&&(temp[j+4] == 'R')&&(temp[j+5] == 'D')&&(temp[j+6] == '='))
			{
				if(temp[j-1] == '|')
				{
					memcpy(str, temp, j-5);
					temp += j-5;
				}
				else
				{
					memcpy(str, temp, j-4);
					temp += j-4;
				}
				break;
			}
		}
		
		onelinestr = str;
		stringarray.SetAt(i, onelinestr);
	}
	
	free(pbuffer);
	free(pbufferbak);

	Open_AD6_SCH_File(total_linenum, stringarray, nCurrentDiagram);

}

void CESDDoc::Open_AD6_SCH_File(int total_linenum, CStringArray& stringarray, int nCurrentDiagram)
{
	CArray <POSITION, POSITION&> posArray;
	posArray.RemoveAll();
	posArray.SetSize(total_linenum);

	for(int linenum=0; linenum < total_linenum; linenum++)
	{
		CString cstring, keyword,data;
		cstring = stringarray.GetAt(linenum);
		if(cstring.IsEmpty() == TRUE) continue;

		keyword = "RECORD=";
		if(Get_KeyWord_Data(cstring, keyword, data) == FALSE) continue;
		if( data == "31")
		{
			keyword = "|FONTIDCOUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) != TRUE) continue;
			int fontnum = atoi(data);
			m_arrayFont[nCurrentDiagram].RemoveAll();
 
			for(int i=1; i<=fontnum; i++)  
			{
				LOGFONT logfont;
				char s[100];  itoa(i, s, 10);

				keyword = CString("|SIZE") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) logfont.lfHeight = atoi(data);
				else logfont.lfHeight = 10;

				keyword = CString("|ROTATION") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) logfont.lfOrientation = atoi(data);
				else logfont.lfOrientation = 0;
				logfont.lfOrientation = Get_Correct_Orientation(logfont.lfOrientation);

				keyword = CString("|FONTNAME") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
				{
					data = data.Left(31);
					strcpy(logfont.lfFaceName,data);
				}
				else strcpy(logfont.lfFaceName, NULL);

				keyword = CString("|UNDERLINE") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) logfont.lfUnderline = TRUE;
				else logfont.lfUnderline = FALSE;

				keyword = CString("|STRIKEOUT") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) logfont.lfStrikeOut = TRUE;
				else logfont.lfStrikeOut = FALSE;

				keyword = CString("|BOLD") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) logfont.lfWeight = FW_BOLD;
				else logfont.lfWeight = FW_NORMAL;

				keyword = CString("|ITALIC") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) logfont.lfItalic = TRUE;
				else logfont.lfItalic = FALSE;

				logfont.lfWidth = 0;
				logfont.lfEscapement = logfont.lfOrientation*10;
				logfont.lfCharSet = DEFAULT_CHARSET; // always
				logfont.lfOutPrecision = OUT_DEFAULT_PRECIS; // always
				logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS; // always
				logfont.lfQuality = PROOF_QUALITY; //always
				logfont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN; //always
				m_arrayFont[nCurrentDiagram].Add(logfont);
			}

			keyword = "|SHEETSTYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_pSCH_Sheet[nCurrentDiagram]->m_nStandardStyle = atoi(data);
			else  m_pSCH_Sheet[nCurrentDiagram]->m_nStandardStyle = 0;

			keyword = "|WORKSPACEORIENTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_pSCH_Sheet[nCurrentDiagram]->m_nOrientation = atoi(data);
			else  m_pSCH_Sheet[nCurrentDiagram]->m_nOrientation = 0;	

			keyword = "|BORDERON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_pSCH_Sheet[nCurrentDiagram]->m_bShowBorder = TRUE;
			else  m_pSCH_Sheet[nCurrentDiagram]->m_bShowBorder = FALSE;
					
			keyword = "|TITLEBLOCKON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_pSCH_Sheet[nCurrentDiagram]->m_bTitleBlock = TRUE;
			else  m_pSCH_Sheet[nCurrentDiagram]->m_bTitleBlock = FALSE;
			keyword = "|DOCUMENTBORDERSTYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_pSCH_Sheet[nCurrentDiagram]->m_nTitleBlockStyle = atoi(data);
			else  m_pSCH_Sheet[nCurrentDiagram]->m_nTitleBlockStyle = 0;
			keyword = "|SHEETNUMBERSPACESIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_pSCH_Sheet[nCurrentDiagram]->m_nSheetNumberSpace = atoi(data);
			else  m_pSCH_Sheet[nCurrentDiagram]->m_nSheetNumberSpace = 4;		

			keyword = "|REFERENCEZONESON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_pSCH_Sheet[nCurrentDiagram]->m_bShowReferenceZones = FALSE; 
			else m_pSCH_Sheet[nCurrentDiagram]->m_bShowReferenceZones = TRUE;	

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_pSCH_Sheet[nCurrentDiagram]->m_nBorderColor = atoi(data);
			else  m_pSCH_Sheet[nCurrentDiagram]->m_nBorderColor = RGB(0,0,0);
			keyword = "|AREACOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_pSCH_Sheet[nCurrentDiagram]->m_nSheetColor = atoi(data);
			else  m_pSCH_Sheet[nCurrentDiagram]->m_nSheetColor = RGB(0,0,0);

			keyword = "|SYSTEMFONT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_pSCH_Sheet[nCurrentDiagram]->m_nFont = atoi(data) - 1;
			else  m_pSCH_Sheet[nCurrentDiagram]->m_nFont = 0;

			keyword = "|SHOWTEMPLATEGRAPHICS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_pSCH_Sheet[nCurrentDiagram]->m_bShowTemplateGraphics = TRUE;
			else  m_pSCH_Sheet[nCurrentDiagram]->m_bShowTemplateGraphics = FALSE;
						
			keyword = "|USECUSTOMSHEET=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_pSCH_Sheet[nCurrentDiagram]->m_bCustom = TRUE;
			else  m_pSCH_Sheet[nCurrentDiagram]->m_bCustom = FALSE;

			int i1,i2;
			keyword = "|CUSTOMX=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else  i1 = 1100;
			keyword = "|CUSTOMX_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else  i2 = 0;
			m_pSCH_Sheet[nCurrentDiagram]->m_fCustomWidth = i1*10.0 + i2/10000.0;
			keyword = "|CUSTOMY=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else  i1 = 800;
			keyword = "|CUSTOMY_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else  i2 = 0;
			m_pSCH_Sheet[nCurrentDiagram]->m_fCustomHeight = i1*10.0 + i2/10000.0;

			keyword = "|CUSTOMXZONES=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_pSCH_Sheet[nCurrentDiagram]->m_nCustomXRegionCount = atoi(data);
			else  m_pSCH_Sheet[nCurrentDiagram]->m_nCustomXRegionCount = 6;
			keyword = "|CUSTOMYZONES=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_pSCH_Sheet[nCurrentDiagram]->m_nCustomYRegionCount = atoi(data);
			else  m_pSCH_Sheet[nCurrentDiagram]->m_nCustomYRegionCount = 4;
			
			keyword = "|CUSTOMMARGINWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else  i1 = 20;
			keyword = "|CUSTOMMARGINWIDTH_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else  i2 = 0;
			m_pSCH_Sheet[nCurrentDiagram]->m_fCustomMargin = i1*10.0 + i2/10000.0;

			m_pSCH_Sheet[nCurrentDiagram]->m_nDiagram = nCurrentDiagram;

			keyword = "|SNAPGRIDON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_bSCH_SnapGridEnable = TRUE;
			else  m_bSCH_SnapGridEnable = FALSE;
			keyword = "|SNAPGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else  i1 = 10;
			keyword = "|SNAPGRIDSIZE_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else  i2 = 0;
			m_fSCH_SnapGrid = i1*10.0 + i2/10000.0;

			keyword = "|VISIBLEGRIDON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_bSCH_VisibleGridShow = TRUE;
			else  m_bSCH_VisibleGridShow = FALSE;
			keyword = "|VISIBLEGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else  i1 = 10;
			keyword = "|VISIBLEGRIDSIZE_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else  i2 = 0;
			m_fSCH_VisibleGrid = i1*10.0 + i2/10000.0;

			keyword = "|HOTSPOTGRIDON=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_bSCH_ElecGridEnable = TRUE;
			else  m_bSCH_ElecGridEnable = FALSE;
			keyword = "|HOTSPOTGRIDSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else  i1 = 10;
			keyword = "|HOTSPOTGRIDSIZE_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else  i2 = 0;
			m_fSCH_ElecGrid = i1*10.0 + i2/10000.0;
			
			keyword = "|DISPLAY_UNIT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) m_nSCH_Unit = atoi(data); //atoi(data)%2;
			else  m_nSCH_Unit = UNIT_MIL;

		}
		else if((data == "26")||(data == "27"))  
		{
			COb_schline* pline = new COb_schline();
			pline->pDocument = this;
			pline->m_nDiagram = nCurrentDiagram;	
					
			if(data == "26") pline->m_nAttrib = LINE_BUS;
			if(data == "27") pline->m_nAttrib = LINE_WIRE;

			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pline->m_nLineWidth = atoi(data);
			else pline->m_nLineWidth = 0;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pline->m_nColor = atoi(data);
			else pline->m_nColor = RGB(0,0,0);

			int vertex = 0;
			keyword = "|LOCATIONCOUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex = atoi(data);
			else vertex = 0;
			pline->m_cVertex.SetSize(vertex);
			for(int i=1; i <= vertex; i++)
			{
				int i1,i2;
				char s[100];
				itoa(i, s, 10);
				//X坐标
				keyword = CString("|X") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				keyword = CString("|X") + s + "_FRAC=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
				else i2 = 0;
				pline->m_cVertex[i-1].fx = i1*10.0 + i2/10000.0;
				//Y坐标
				keyword = CString("|Y") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				keyword = CString("|Y") + s + "_FRAC=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
				else i2 = 0;
				pline->m_cVertex[i-1].fy = i1*10.0 + i2/10000.0;
			}
					
			m_listSchObject[nCurrentDiagram].AddTail(pline);
		}
		else if( data == "6" )
		{
			COb_schpolyline* ppolyline = new COb_schpolyline();
			ppolyline->pDocument = this;
			ppolyline->m_nDiagram = nCurrentDiagram;	

			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			COb_schcomp* pcomp = NULL;

			if(ownerindex > 0)
			{
				POSITION posParent = posArray.GetAt(ownerindex);
				if(posParent != NULL)
				{
					CObject* pobject;
					pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						pcomp = (COb_schcomp*)pobject;
						ppolyline->m_posParent = posParent;

						keyword = "|OWNERPARTID=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolyline->m_nOwnerPartID = atoi(data);
						else ppolyline->m_nOwnerPartID = 0;

						keyword = "|OWNERPARTDISPLAYMODE=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolyline->m_nDisplayMode = atoi(data);
						else ppolyline->m_nDisplayMode = 0;
					}
				}
			}

			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolyline->m_nLineWidth = atoi(data);
			else ppolyline->m_nLineWidth = 0;

			keyword = "|LINESTYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolyline->m_nLineType = atoi(data);
			else ppolyline->m_nLineType = 0;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolyline->m_nColor = atoi(data);
			else ppolyline->m_nColor = RGB(0,0,0);

			keyword = "|STARTLINESHAPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolyline->m_nStartLineShape = atoi(data);
			else ppolyline->m_nStartLineShape = 0;

			keyword = "|ENDLINESHAPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolyline->m_nEndLineShape = atoi(data);
			else ppolyline->m_nEndLineShape = 0;

			keyword = "|LINESHAPESIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolyline->m_nLineShapeSize = atoi(data);
			else ppolyline->m_nLineShapeSize = 0;

			int vertex = 0;
			keyword = "|LOCATIONCOUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex = atoi(data);
			else vertex = 0;
			ppolyline->m_cVertex.SetSize(vertex);
			for(int i=1; i <= vertex; i++)
			{
				int i1,i2;
				char s[100];
				itoa(i, s, 10);

				keyword = CString("|X") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				keyword = CString("|X") + s + "_FRAC=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
				else i2 = 0;
				ppolyline->m_cVertex[i-1].fx = i1*10.0 + i2/10000.0;

				keyword = CString("|Y") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				keyword = CString("|Y") + s + "_FRAC=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
				else i2 = 0;
				ppolyline->m_cVertex[i-1].fy = i1*10.0 + i2/10000.0;
			}
					

			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ppolyline);

			if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
		}
		else if( data == "13" )
		{
			COb_schpolyline* ppolyline = new COb_schpolyline();
			ppolyline->pDocument = this;
			ppolyline->m_nDiagram = nCurrentDiagram;	

			//OWNERINDEX
			keyword = "OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			COb_schcomp* pcomp = NULL;
			if(ownerindex > 0)
			{
				POSITION posParent = posArray.GetAt(ownerindex);
				if(posParent != NULL)
				{
					CObject* pobject;
					pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						pcomp = (COb_schcomp*)pobject;
						ppolyline->m_posParent = posParent;
						keyword = "OWNERPARTID=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolyline->m_nOwnerPartID = atoi(data);
						else ppolyline->m_nOwnerPartID = 0;

						keyword = "OWNERPARTDISPLAYMODE=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolyline->m_nDisplayMode = atoi(data);
						else ppolyline->m_nDisplayMode = 0;
					}
				}
			}

			keyword = "LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolyline->m_nLineWidth = atoi(data);
			else ppolyline->m_nLineWidth = 0;

			keyword = "LINESTYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolyline->m_nLineType = atoi(data);
			else ppolyline->m_nLineType = 0;

			keyword = "COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolyline->m_nColor = atoi(data);
			else ppolyline->m_nColor = RGB(0,0,0);

			ppolyline->m_nStartLineShape = 0;
			ppolyline->m_nEndLineShape = 0;
			ppolyline->m_nLineShapeSize = 0;


			int vertex = 2;
			ppolyline->m_cVertex.SetSize(vertex);
			int i1,i2;

			keyword = "LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ppolyline->m_cVertex[0].fx = i1*10.0 + i2/10000.0;

			keyword = "LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ppolyline->m_cVertex[0].fy = i1*10.0 + i2/10000.0;

			keyword = "CORNER.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "CORNER.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ppolyline->m_cVertex[1].fx = i1*10.0 + i2/10000.0;

			keyword = "CORNER.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "CORNER.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ppolyline->m_cVertex[1].fy = i1*10.0 + i2/10000.0;
					
			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ppolyline);

			if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
		}
		else if( data == "37" ) 
		{
			COb_schline* pline = new COb_schline();
			pline->pDocument = this;
			pline->m_nDiagram = nCurrentDiagram;	
					
			pline->m_nAttrib = LINE_BUS_ENTRY;

			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pline->m_nLineWidth = atoi(data);
			else pline->m_nLineWidth = 0;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pline->m_nColor = atoi(data);
			else pline->m_nColor = RGB(0,0,0);

			pline->m_cVertex.SetSize(2);
			int i1,i2;

			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pline->m_cVertex[0].fx = i1*10.0 + i2/10000.0;

			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pline->m_cVertex[0].fy = i1*10.0 + i2/10000.0;

			keyword = "|CORNER.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|CORNER.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pline->m_cVertex[1].fx = i1*10.0 + i2/10000.0;

			keyword = "|CORNER.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|CORNER.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pline->m_cVertex[1].fy = i1*10.0 + i2/10000.0;
					
			m_listSchObject[nCurrentDiagram].AddTail(pline);
		}
		else if( data == "29" )  
		{
			COb_schjunc* pjunc = new COb_schjunc();
			pjunc->pDocument = this;
			pjunc->m_nDiagram = nCurrentDiagram;	
					
			keyword = "|SIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pjunc->m_nSize = atoi(data);
			else pjunc->m_nSize = 0;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pjunc->m_nColor = atoi(data);
			else pjunc->m_nColor = RGB(0,0,0);

			keyword = "|LOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pjunc->m_bManual = TRUE;
			else pjunc->m_bManual = FALSE;

			int i1,i2;

			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pjunc->m_fCx = i1*10.0 + i2/10000.0;

			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pjunc->m_fCy = i1*10.0 + i2/10000.0;
			
			m_listSchObject[nCurrentDiagram].AddTail(pjunc);
		}				
		else if( data == "12" )  
		{
			COb_scharc* parc = new COb_scharc();
			parc->pDocument = this;
			parc->m_nDiagram = nCurrentDiagram;	

			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			COb_schcomp* pcomp = NULL;
			if(ownerindex > 0)
			{
				POSITION posParent = posArray.GetAt(ownerindex);
				if(posParent != NULL)
				{
					CObject* pobject;
					pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						pcomp = (COb_schcomp*)pobject;
						parc->m_posParent = posParent;
						keyword = "|OWNERPARTID=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nOwnerPartID = atoi(data);
						else parc->m_nOwnerPartID = 0;

						keyword = "|OWNERPARTDISPLAYMODE=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nDisplayMode = atoi(data);
						else parc->m_nDisplayMode = 0;
					}
				}
			}
			
			int i1,i2;

			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			parc->m_fCx = i1*10.0 + i2/10000.0;

			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			parc->m_fCy = i1*10.0 + i2/10000.0;
			
			keyword = "|RADIUS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|RADIUS_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			parc->m_fRadius = i1*10.0 + i2/10000.0;	

			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nWidth = atoi(data);
			else parc->m_nWidth = 0;
					
			keyword = "|STARTANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_fSangle = atof(data);
			else parc->m_fSangle = 0;	

			keyword = "|ENDANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_fEangle = atof(data);
			else parc->m_fEangle = 0;						

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) parc->m_nColor = atoi(data);
			else parc->m_nColor = RGB(0,0,0);

			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(parc);

			if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
		}
		else if( data == "11" )  
		{
			COb_schellipse* pellipse = new COb_schellipse();
			pellipse->pDocument = this;
			pellipse->m_nDiagram = nCurrentDiagram;	
					
			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			COb_schcomp* pcomp = NULL;
			if(ownerindex > 0)
			{
				POSITION posParent = posArray.GetAt(ownerindex);
				if(posParent != NULL)
				{
					CObject* pobject;
					pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						pcomp = (COb_schcomp*)pobject;
						pellipse->m_posParent = posParent;
						keyword = "|OWNERPARTID=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pellipse->m_nOwnerPartID = atoi(data);
						else pellipse->m_nOwnerPartID = 0;

						keyword = "|OWNERPARTDISPLAYMODE=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pellipse->m_nDisplayMode = atoi(data);
						else pellipse->m_nDisplayMode = 0;
					}
				}
			}

			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pellipse->m_nWidth = atoi(data);
			else pellipse->m_nWidth = 0;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pellipse->m_nColor = atoi(data);
			else pellipse->m_nColor = RGB(0,0,0);

			int i1,i2;

			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pellipse->m_fCx = i1*10.0 + i2/10000.0;

			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pellipse->m_fCy = i1*10.0 + i2/10000.0;
			
			keyword = "|RADIUS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|RADIUS_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pellipse->m_fRadius_x = i1*10.0 + i2/10000.0;

			keyword = "|SECONDARYRADIUS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|SECONDARYRADIUS_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pellipse->m_fRadius_y = i1*10.0 + i2/10000.0;

			keyword = "|STARTANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pellipse->m_fSangle = atof(data);
			else pellipse->m_fSangle = 0;				
			keyword = "|ENDANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pellipse->m_fEangle = atof(data);
			else pellipse->m_fEangle = 0;
					

			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pellipse);

			if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
		}
		else if( data == "8" )  //Elliptical Fill
		{
			COb_schellipsefill* pellipsefill = new COb_schellipsefill();
			pellipsefill->pDocument = this;
			pellipsefill->m_nDiagram = nCurrentDiagram;	
					
			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			COb_schcomp* pcomp = NULL;
			if(ownerindex > 0)
			{
				POSITION posParent = posArray.GetAt(ownerindex);
				if(posParent != NULL)
				{
					CObject* pobject;
					pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						pcomp = (COb_schcomp*)pobject;
						pellipsefill->m_posParent = posParent;
						keyword = "|OWNERPARTID=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pellipsefill->m_nOwnerPartID = atoi(data);
						else pellipsefill->m_nOwnerPartID = 0;

						keyword = "|OWNERPARTDISPLAYMODE=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pellipsefill->m_nDisplayMode = atoi(data);
						else pellipsefill->m_nDisplayMode = 0;
					}
				}
			}

			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pellipsefill->m_nBorderWidth = atoi(data);
			else pellipsefill->m_nBorderWidth = 0;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pellipsefill->m_nBorderColor = atoi(data);
			else pellipsefill->m_nBorderColor = RGB(0,0,0);

			keyword = "|AREACOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pellipsefill->m_nFillColor = atoi(data);
			else pellipsefill->m_nFillColor = RGB(0,0,0);

			keyword = "|ISSOLID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pellipsefill->m_bFillSolid = TRUE;
			else pellipsefill->m_bFillSolid = FALSE;

			keyword = "|TRANSPARENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pellipsefill->m_bTransparent = TRUE;
			else pellipsefill->m_bTransparent = FALSE;


			int i1,i2;

			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pellipsefill->m_fCx = i1*10.0 + i2/10000.0;

			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pellipsefill->m_fCy = i1*10.0 + i2/10000.0;
			
			keyword = "|RADIUS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|RADIUS_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pellipsefill->m_fRadius_x = i1*10.0 + i2/10000.0;

			keyword = "|SECONDARYRADIUS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|SECONDARYRADIUS_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pellipsefill->m_fRadius_y = i1*10.0 + i2/10000.0;
			
					
			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pellipsefill);

			if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
		}
		else if( data == "9" )  
		{

			COb_schpie* ppie = new COb_schpie();
			ppie->pDocument = this;
			ppie->m_nDiagram = nCurrentDiagram;	

			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			COb_schcomp* pcomp = NULL;
			if(ownerindex > 0)
			{
				POSITION posParent = posArray.GetAt(ownerindex);
				if(posParent != NULL)
				{
					CObject* pobject;
					pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						pcomp = (COb_schcomp*)pobject;
						ppie->m_posParent = posParent;
						keyword = "|OWNERPARTID=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppie->m_nOwnerPartID = atoi(data);
						else ppie->m_nOwnerPartID = 0;

						keyword = "|OWNERPARTDISPLAYMODE=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppie->m_nDisplayMode = atoi(data);
						else ppie->m_nDisplayMode = 0;
					}
				}
			}

			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppie->m_nBorderWidth = atoi(data);
			else ppie->m_nBorderWidth = 0;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppie->m_nBorderColor = atoi(data);
			else ppie->m_nBorderColor = RGB(0,0,0);

			keyword = "|AREACOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppie->m_nFillColor = atoi(data);
			else ppie->m_nFillColor = RGB(0,0,0);

			keyword = "|ISSOLID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppie->m_bFillSolid = TRUE;
			else ppie->m_bFillSolid = FALSE;

			int i1,i2;

			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ppie->m_fCx = i1*10.0 + i2/10000.0;

			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ppie->m_fCy = i1*10.0 + i2/10000.0;
			

			keyword = "|RADIUS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|RADIUS_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ppie->m_fRadius = i1*10.0 + i2/10000.0;

			keyword = "|STARTANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppie->m_fSangle = atof(data);
			else ppie->m_fSangle = 0;				
			keyword = "|ENDANGLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppie->m_fEangle = atof(data);
			else ppie->m_fEangle = 0;				
					
			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ppie);

			if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
		}
		else if( (data == "4" )||(data == "25"))  
		{
			COb_schtext* ptext = new COb_schtext();
			ptext->pDocument = this;
			ptext->m_nDiagram = nCurrentDiagram;	

			if(data == "4") ptext->m_nID = TEXT_TEXT;
			else if(data == "25") ptext->m_nID = TEXT_NETLABEL;
			ptext->m_csNameComment.Empty();

			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			COb_schcomp* pcomp = NULL;
			if(ownerindex > 0)
			{
				POSITION posParent = posArray.GetAt(ownerindex);
				if(posParent != NULL)
				{
					CObject* pobject;
					pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						pcomp = (COb_schcomp*)pobject;
						ptext->m_posParent = posParent;
						keyword = "|OWNERPARTID=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nOwnerPartID = atoi(data);
						else ptext->m_nOwnerPartID = 0;

						keyword = "|OWNERPARTDISPLAYMODE=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nDisplayMode = atoi(data);
						else ptext->m_nDisplayMode = 0;
					}
				}
			}

			int i1,i2;

			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ptext->m_fSx = i1*10.0 + i2/10000.0;

			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ptext->m_fSy = i1*10.0 + i2/10000.0;

			keyword = "|ORIENTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nOrientation = atoi(data)*90;
			else ptext->m_nOrientation = 0;
			ptext->m_nOrientation = Get_Correct_Orientation(ptext->m_nOrientation);

			keyword = "|JUSTIFICATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			ptext->m_nH_Justification = i1%3;
			ptext->m_nV_Justification = i1/3;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nColor = atoi(data);
			else ptext->m_nColor = 0;		

			keyword = "|FONTID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nFont = atoi(data) - 1;
			else ptext->m_nFont = 0;

			keyword = "|TEXT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csText = data;
			else ptext->m_csText.Empty();

			keyword = "|ISMIRRORED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bMirror = TRUE;
			else ptext->m_bMirror = FALSE;


			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptext);

			if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
		}
		else if( data == "7" )  
		{
			COb_schpolygon* ppolygon = new COb_schpolygon();
			ppolygon->pDocument = this;
			ppolygon->m_nDiagram = nCurrentDiagram;	
	
			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			COb_schcomp* pcomp = NULL;
			if(ownerindex > 0)
			{
				POSITION posParent = posArray.GetAt(ownerindex);
				if(posParent != NULL)
				{
					CObject* pobject;
					pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						pcomp = (COb_schcomp*)pobject;
						ppolygon->m_posParent = posParent;
						keyword = "|OWNERPARTID=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_nOwnerPartID = atoi(data);
						else ppolygon->m_nOwnerPartID = 0;

						keyword = "|OWNERPARTDISPLAYMODE=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_nDisplayMode = atoi(data);
						else ppolygon->m_nDisplayMode = 0;
					}
				}
			}


			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_nBorderWidth = atoi(data);
			else ppolygon->m_nBorderWidth = 0;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_nBorderColor = atoi(data);
			else ppolygon->m_nBorderColor = RGB(0,0,0);

			keyword = "|AREACOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_nFillColor = atoi(data);
			else ppolygon->m_nFillColor = RGB(0,0,0);

			keyword = "|ISSOLID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_bFillSolid = TRUE;
			else ppolygon->m_bFillSolid = FALSE;

			keyword = "|TRANSPARENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppolygon->m_bTransparent = TRUE;
			else ppolygon->m_bTransparent = FALSE;


			int vertex = 0;
			keyword = "|LOCATIONCOUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex = atoi(data);
			else vertex = 0;
			ppolygon->m_cVertex.SetSize(vertex);
			for(int i=1; i <= vertex; i++)
			{
				int i1,i2;
				char s[100];
				itoa(i, s, 10);

				keyword = CString("|X") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				keyword = CString("|X") + s + "_FRAC=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
				else i2 = 0;
				ppolygon->m_cVertex[i-1].fx = i1*10.0 + i2/10000.0;

				keyword = CString("|Y") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				keyword = CString("|Y") + s + "_FRAC=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
				else i2 = 0;
				ppolygon->m_cVertex[i-1].fy = i1*10.0 + i2/10000.0;
			}
					

			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ppolygon);

			if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
		}
		else if( data == "28" ) 
		{
			COb_schtextframe* ptextframe = new COb_schtextframe();
			ptextframe->pDocument = this;
			ptextframe->m_nDiagram = nCurrentDiagram;	

			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			COb_schcomp* pcomp = NULL;
			if(ownerindex > 0)
			{
				POSITION posParent = posArray.GetAt(ownerindex);
				if(posParent != NULL)
				{
					CObject* pobject;
					pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						pcomp = (COb_schcomp*)pobject;
						ptextframe->m_posParent = posParent;
						keyword = "|OWNERPARTID=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptextframe->m_nOwnerPartID = atoi(data);
						else ptextframe->m_nOwnerPartID = 0;

						keyword = "|OWNERPARTDISPLAYMODE=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptextframe->m_nDisplayMode = atoi(data);
						else ptextframe->m_nDisplayMode = 0;
					}
				}
			}

			int i1,i2;

			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ptextframe->m_fX1 = i1*10.0 + i2/10000.0;

			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ptextframe->m_fY1 = i1*10.0 + i2/10000.0;	

			keyword = "|CORNER.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|CORNER.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ptextframe->m_fX2 = i1*10.0 + i2/10000.0;

			keyword = "|CORNER.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|CORNER.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ptextframe->m_fY2 = i1*10.0 + i2/10000.0;	

						

			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptextframe->m_nBorderWidth = atoi(data);
			else ptextframe->m_nBorderWidth = 0;

			keyword = "|FONTID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptextframe->m_nFont = atoi(data) - 1;
			else ptextframe->m_nFont = 0;

			keyword = "|ISSOLID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptextframe->m_bFillSolid = TRUE;
			else ptextframe->m_bFillSolid = FALSE;

			keyword = "|SHOWBORDER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptextframe->m_bDrawBorder = TRUE;
			else ptextframe->m_bDrawBorder = FALSE;

			keyword = "|WORDWRAP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptextframe->m_bWordWrap = TRUE;
			else ptextframe->m_bWordWrap = FALSE;

			keyword = "|CLIPTORECT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptextframe->m_bClipToArea= TRUE;
			else ptextframe->m_bClipToArea = FALSE;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptextframe->m_nBorderColor = atoi(data);
			else ptextframe->m_nBorderColor = RGB(0,0,0);
			keyword = "|AREACOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptextframe->m_nFillColor = atoi(data);
			else ptextframe->m_nFillColor = RGB(0,0,0);
			keyword = "|TEXTCOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptextframe->m_nTextColor = atoi(data);
			else ptextframe->m_nTextColor = RGB(0,0,0);
			//ALIGNMENT
			keyword = "|ALIGNMENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptextframe->m_nAlignment = atoi(data);
			else ptextframe->m_nAlignment = 0;

			//TEXT
			CString  text;
			keyword = "|Text=";
			//if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptextframe->m_csText = data;
			//else ptextframe->m_csText.Empty();
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) text = data;
			else text.Empty();

			ptextframe->m_arrayText.RemoveAll();
			//CString text = m_csText;
			int index = text.Find("~1");
			for(;;)
			{
				if(index < 0)
				{
					ptextframe->m_arrayText.Add(text);
					break;
				}
				else if(index == 0)
				{
					CString t;
					t = text.Left(index);
					ptextframe->m_arrayText.Add(t);
					text = text.Right(text.GetLength() - index - 2);
				}
				else
				{
					if(text.GetAt(index-1) == '~') 
					{
						index = text.Find("~1", index+2);
						continue;
					}
					else
					{
						CString t;
						t = text.Left(index);
						ptextframe->m_arrayText.Add(t);
						text = text.Right(text.GetLength() - index - 2);
					}
				}
				index = text.Find("~1");
			}

			int total = ptextframe->m_arrayText.GetSize();
			for(int i=0; i<total; i++)
			{
				CString str, str1, str2;;
				str = ptextframe->m_arrayText.GetAt(i);
				int index = str.Find("~~");
				for(;;)
				{
					if(index < 0)	break;
					str1 = str.Left(index);
					str2 = str.Right(str.GetLength() - index -1);
					str = str1+str2;
					index = str.Find("~~", index+1);
				}
				ptextframe->m_arrayText.SetAt(i, str);
			}

			m_listSchObject[nCurrentDiagram].AddTail(ptextframe);
		}
		else if( data == "14" )  
		{
			COb_schrect* prect = new COb_schrect();
			prect->pDocument = this;
			prect->m_nDiagram = nCurrentDiagram;	

			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			COb_schcomp* pcomp = NULL;
			if(ownerindex > 0)
			{
				POSITION posParent = posArray.GetAt(ownerindex);
				if(posParent != NULL)
				{
					CObject* pobject;
					pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						pcomp = (COb_schcomp*)pobject;
						prect->m_posParent = posParent;
						keyword = "|OWNERPARTID=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) prect->m_nOwnerPartID = atoi(data);
						else prect->m_nOwnerPartID = 0;

						keyword = "|OWNERPARTDISPLAYMODE=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) prect->m_nDisplayMode = atoi(data);
						else prect->m_nDisplayMode = 0;
					}
				}
			}

			int i1,i2;

			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			prect->m_fX1 = i1*10.0 + i2/10000.0;

			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			prect->m_fY1 = i1*10.0 + i2/10000.0;	

			keyword = "|CORNER.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|CORNER.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			prect->m_fX2 = i1*10.0 + i2/10000.0;

			keyword = "|CORNER.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|CORNER.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			prect->m_fY2 = i1*10.0 + i2/10000.0;	
					

			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) prect->m_nBorderWidth = atoi(data);
			else prect->m_nBorderWidth = 0;

			keyword = "|ISSOLID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) prect->m_bFillSolid = TRUE;
			else prect->m_bFillSolid = FALSE;

			keyword = "|TRANSPARENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) prect->m_bTransparent = TRUE;
			else prect->m_bTransparent = FALSE;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) prect->m_nBorderColor = atoi(data);
			else prect->m_nBorderColor = RGB(0,0,0);
			keyword = "|AREACOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) prect->m_nFillColor = atoi(data);
			else prect->m_nFillColor = RGB(0,0,0);

			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(prect);

			if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
		}
		else if( data == "10" )  // Round Rectangle
		{
			COb_schroundrect* proundrect = new COb_schroundrect();
			proundrect->pDocument = this;
			proundrect->m_nDiagram = nCurrentDiagram;	

			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			COb_schcomp* pcomp = NULL;
			if(ownerindex > 0)
			{
				POSITION posParent = posArray.GetAt(ownerindex);
				if(posParent != NULL)
				{
					CObject* pobject;
					pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						pcomp = (COb_schcomp*)pobject;
						proundrect->m_posParent = posParent;
						keyword = "|OWNERPARTID=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) proundrect->m_nOwnerPartID = atoi(data);
						else proundrect->m_nOwnerPartID = 0;

						keyword = "|OWNERPARTDISPLAYMODE=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) proundrect->m_nDisplayMode = atoi(data);
						else proundrect->m_nDisplayMode = 0;
					}
				}
			}

			int i1,i2;

			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			proundrect->m_fX1 = i1*10.0 + i2/10000.0;

			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			proundrect->m_fY1 = i1*10.0 + i2/10000.0;	

			keyword = "|CORNER.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|CORNER.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			proundrect->m_fX2 = i1*10.0 + i2/10000.0;

			keyword = "|CORNER.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|CORNER.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			proundrect->m_fY2 = i1*10.0 + i2/10000.0;	


			keyword = "|CORNERXRADIUS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|CORNERXRADIUS_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			proundrect->m_fRadius_x = i1*10.0 + i2/10000.0;

			keyword = "|CORNERYRADIUS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|CORNERYRADIUS_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			proundrect->m_fRadius_y = i1*10.0 + i2/10000.0;	
						

			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) proundrect->m_nBorderWidth = atoi(data);
			else proundrect->m_nBorderWidth = 0;

			keyword = "|ISSOLID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) proundrect->m_bFillSolid = TRUE;
			else proundrect->m_bFillSolid = FALSE;


			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) proundrect->m_nBorderColor = atoi(data);
			else proundrect->m_nBorderColor = RGB(0,0,0);
			keyword = "|AREACOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) proundrect->m_nFillColor = atoi(data);
			else proundrect->m_nFillColor = RGB(0,0,0);

			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(proundrect);

			if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
		}
		else if( data == "15" ) 
		{
			COb_schsheetsymbol* psheetsymbol = new COb_schsheetsymbol();
			psheetsymbol->pDocument = this;
			psheetsymbol->m_nDiagram = nCurrentDiagram;	

			int i1,i2;
			//LOCATION.X
			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			psheetsymbol->m_fX = i1*10.0 + i2/10000.0;
			//LOCATION.Y
			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			psheetsymbol->m_fY = i1*10.0 + i2/10000.0;	

			keyword = "|XSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|XSIZE_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			psheetsymbol->m_fXsize = i1*10.0 + i2/10000.0;

			keyword = "|YSIZE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|YSIZE_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			psheetsymbol->m_fYsize = i1*10.0 + i2/10000.0;

			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) psheetsymbol->m_nBorderWidth = atoi(data);
			else psheetsymbol->m_nBorderWidth = 0;

			keyword = "|ISSOLID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) psheetsymbol->m_bFillSolid = TRUE;
			else psheetsymbol->m_bFillSolid = FALSE;

			keyword = "|SHOWHIDDENFIELDS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) psheetsymbol->m_bShowHiddenTextFields = TRUE;
			else psheetsymbol->m_bShowHiddenTextFields = FALSE;


			keyword = "|UNIQUEID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) psheetsymbol->m_csUniqueID = data;
			else psheetsymbol->m_csUniqueID.Empty();

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) psheetsymbol->m_nBorderColor = atoi(data);
			else psheetsymbol->m_nBorderColor = RGB(0,0,0);
			keyword = "|AREACOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) psheetsymbol->m_nFillColor = atoi(data);
			else psheetsymbol->m_nFillColor = RGB(0,0,0);

			POSITION posParent = m_listSchObject[nCurrentDiagram].AddTail(psheetsymbol);
			posArray.SetAt(linenum, posParent);
		}
		else if( data == "16") 
		{
			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			if(ownerindex < 0) continue;
			COb_schsheetsymbol* psheetsymbol;
			POSITION posParent = posArray.GetAt(ownerindex);
			CObject* pobject;
			pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				psheetsymbol = (COb_schsheetsymbol*)pobject;
			}
			else continue;

			COb_schsheetentry* psheetentry = new COb_schsheetentry();
			psheetentry->pDocument = this;
			psheetentry->m_nDiagram = nCurrentDiagram;	

			psheetentry->m_posParent = posParent;

			int i1,i2;

			keyword = "|DISTANCEFROMTOP=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|DISTANCEFROMTOP_FRAC1=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			psheetentry->m_fOffset = i1*100.0 + i2/10000.0; 

			keyword = "|IOTYPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) psheetentry->m_nType = atoi(data);
			else psheetentry->m_nType = 0;

			keyword = "|STYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) psheetentry->m_nStyle = atoi(data);
			else psheetentry->m_nStyle = 0;

			keyword = "|SIDE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) psheetentry->m_nSide = atoi(data);
			else psheetentry->m_nSide = 0;


			keyword = "|NAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) psheetentry->m_csName = data;
			else psheetentry->m_csName.Empty();

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) psheetentry->m_nBorderColor = atoi(data);
			else psheetentry->m_nBorderColor = RGB(0,0,0);
			keyword = "|AREACOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) psheetentry->m_nFillColor = atoi(data);
			else psheetentry->m_nFillColor = RGB(0,0,0);
			keyword = "|TEXTCOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) psheetentry->m_nTextColor = atoi(data);
			else psheetentry->m_nTextColor = RGB(0,0,0);

			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(psheetentry);

			psheetsymbol->m_arrayPos.Add(pos);
		}
		else if((data == "32")||(data == "33"))
		{
			COb_schtext* ptext = new COb_schtext();
			ptext->pDocument = this;
			ptext->m_nDiagram = nCurrentDiagram;	

			if(data == "32") ptext->m_nID = TEXT_SHEETSYMBOLDESIGNATOR;
			else ptext->m_nID = TEXT_SHEETSYMBOLFILENAME;

			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			if(ownerindex < 0) continue;
			COb_schsheetsymbol* psheetsymbol;
			POSITION posParent = posArray.GetAt(ownerindex);
			CObject* pobject;
			pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				psheetsymbol = (COb_schsheetsymbol*)pobject;
			}
			else continue;

			int i1,i2;
			//LOCATION.X
			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ptext->m_fSx = i1*10.0 + i2/10000.0;
			//LOCATION.Y
			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ptext->m_fSy = i1*10.0 + i2/10000.0;	
			//ORIENTATION
			keyword = "|ORIENTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nOrientation = atoi(data)*90;
			else ptext->m_nOrientation = 0;
			ptext->m_nOrientation = Get_Correct_Orientation(ptext->m_nOrientation);
			//COLOR
			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nColor = atoi(data);
			else ptext->m_nColor = 0;
			//FONTID
			keyword = "|FONTID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nFont = atoi(data) - 1;
			else ptext->m_nFont = 0;
			//Hidden
			keyword = "|ISHIDDEN=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bTextVisible = FALSE;
			else ptext->m_bTextVisible = TRUE;
			//TEXT
			keyword = "|TEXT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csText = data;
			else ptext->m_csText.Empty();

			keyword = "|NOTAUTOPOSITION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bAutoPosition = FALSE;
			else ptext->m_bAutoPosition = TRUE;

			keyword = "|TEXTHORZANCHOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nAnChorH = atoi(data);
			else ptext->m_nAnChorH = 0;	

			keyword = "|TEXTVERTANCHOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nAnChorV = atoi(data);
			else ptext->m_nAnChorV = 0;

			ptext->m_posParent = posParent;

			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptext);

			psheetsymbol->m_arrayPos.Add(pos);
		}
		else if( data == "18" )  
		{
			COb_schport* pport = new COb_schport();
			pport->pDocument = this;
			pport->m_nDiagram = nCurrentDiagram;	

			int i1,i2;
			//LOCATION.X
			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pport->m_fX = i1*10.0 + i2/10000.0;
			//LOCATION.Y
			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pport->m_fY = i1*10.0 + i2/10000.0;	

			keyword = "|WIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|WIDTH_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pport->m_fSize = i1*10.0 + i2/10000.0;
						
			//style
			keyword = "|STYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pport->m_nStyle = atoi(data);
			else pport->m_nStyle = 0;
			//IO type
			keyword = "|IOTYPE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pport->m_nType = atoi(data);
			else pport->m_nType = 0;
			//alignment
			keyword = "|ALIGNMENT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pport->m_nAlignment = atoi(data);
			else pport->m_nAlignment = 0;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pport->m_nBorderColor = atoi(data);
			else pport->m_nBorderColor = RGB(0,0,0);
			keyword = "|AREACOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pport->m_nFillColor = atoi(data);
			else pport->m_nFillColor = RGB(0,0,0);
			keyword = "|TEXTCOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pport->m_nTextColor = atoi(data);
			else pport->m_nTextColor = RGB(0,0,0);

			//Name
			keyword = "|NAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pport->m_csName = data;
			else pport->m_csName.Empty();
			//Unique ID
			keyword = "|UNIQUEID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pport->m_csUniqueID = data;
			else pport->m_csUniqueID.Empty();

			m_listSchObject[nCurrentDiagram].AddTail(pport);
		}
		else if( data == "17" )  
		{
			COb_schpower* ppower = new COb_schpower();
			ppower->pDocument = this;
			ppower->m_nDiagram = nCurrentDiagram;	

			int i1,i2;
			//LOCATION.X
			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ppower->m_fX = i1*10.0 + i2/10000.0;
			//LOCATION.Y
			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ppower->m_fY = i1*10.0 + i2/10000.0;	
						
			//style
			keyword = "|STYLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppower->m_nStyle = atoi(data);
			else ppower->m_nStyle = 0;

			keyword = "|SHOWNETNAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "T") ppower->m_bShowNetName = TRUE;
				else  ppower->m_bShowNetName = FALSE;
			}
			else ppower->m_bShowNetName = FALSE;

			keyword = "|ISCROSSSHEETCONNECTOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppower->m_bIsCrossSheetConnector = TRUE;
			else ppower->m_bIsCrossSheetConnector = FALSE;
			//Orientation
			keyword = "|ORIENTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppower->m_nOrientation = atoi(data)*90;
			else ppower->m_nOrientation = 0;
			ppower->m_nOrientation = Get_Correct_Orientation(ppower->m_nOrientation);

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppower->m_nColor = atoi(data);
			else ppower->m_nColor = RGB(0,0,0);

			//Net Name
			keyword = "|TEXT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppower->m_csNetName = data;
			else ppower->m_csNetName.Empty();
		
			m_listSchObject[nCurrentDiagram].AddTail(ppower);
		}
		else if( data == "5"  )
		{
			COb_schbezier* pbezier = new COb_schbezier();
			pbezier->pDocument = this;
			pbezier->m_nDiagram = nCurrentDiagram;	

			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			COb_schcomp* pcomp = NULL;
			if(ownerindex > 0)
			{
				POSITION posParent = posArray.GetAt(ownerindex);
				if(posParent != NULL)
				{
					CObject* pobject;
					pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						pcomp = (COb_schcomp*)pobject;
						pbezier->m_posParent = posParent;
						keyword = "|OWNERPARTID=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pbezier->m_nOwnerPartID = atoi(data);
						else pbezier->m_nOwnerPartID = 0;

						keyword = "|OWNERPARTDISPLAYMODE=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pbezier->m_nDisplayMode = atoi(data);
						else pbezier->m_nDisplayMode = 0;
					}
				}
			}


			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pbezier->m_nWidth = atoi(data);
			else pbezier->m_nWidth = 0;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pbezier->m_nColor = atoi(data);
			else pbezier->m_nColor = RGB(0,0,0);

			int vertex = 0;
			keyword = "|LOCATIONCOUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) vertex = atoi(data);
			else vertex = 0;
			pbezier->m_cVertex.SetSize(vertex);
			for(int i=1; i <= vertex; i++)
			{
				int i1,i2;
				char s[100];
				itoa(i, s, 10);

				keyword = CString("|X") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				keyword = CString("|X") + s + "_FRAC=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
				else i2 = 0;
				pbezier->m_cVertex[i-1].fx = i1*10.0 + i2/10000.0;

				keyword = CString("|Y") + s;
				keyword += "=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				keyword = CString("|Y") + s + "_FRAC=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
				else i2 = 0;
				pbezier->m_cVertex[i-1].fy = i1*10.0 + i2/10000.0;
			}
					

			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pbezier);

			if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
		}
		else if( data == "30" )  
		{
			COb_schgraphic* pgraphic = new COb_schgraphic();
			pgraphic->pDocument = this;
			pgraphic->m_nDiagram = nCurrentDiagram;	

			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			COb_schcomp* pcomp = NULL;
			if(ownerindex > 0)
			{
				POSITION posParent = posArray.GetAt(ownerindex);
				if(posParent != NULL)
				{
					CObject* pobject;
					pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						pcomp = (COb_schcomp*)pobject;
						pgraphic->m_posParent = posParent;
						keyword = "|OWNERPARTID=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pgraphic->m_nOwnerPartID = atoi(data);
						else pgraphic->m_nOwnerPartID = 0;

						keyword = "|OWNERPARTDISPLAYMODE=";
						if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pgraphic->m_nDisplayMode = atoi(data);
						else pgraphic->m_nDisplayMode = 0;
					}
				}
			}

			int i1,i2;
			//LOCATION.X
			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pgraphic->m_fX1 = i1*10.0 + i2/10000.0;
			//LOCATION.Y
			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pgraphic->m_fY1 = i1*10.0 + i2/10000.0;	

			keyword = "|CORNER.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|CORNER.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pgraphic->m_fX2 = i1*10.0 + i2/10000.0;

			keyword = "|CORNER.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|CORNER.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pgraphic->m_fY2 = i1*10.0 + i2/10000.0;	


			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pgraphic->m_nBorderWidth = atoi(data);
			else pgraphic->m_nBorderWidth = 0;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pgraphic->m_nBorderColor = atoi(data);
			else pgraphic->m_nBorderColor = RGB(0,0,0);

			keyword = "|ISSOLID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pgraphic->m_bBorderOn = TRUE;
			else pgraphic->m_bBorderOn = FALSE;

			keyword = "|KEEPASPECT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pgraphic->m_bAspect = TRUE;
			else pgraphic->m_bAspect = FALSE;

			keyword = "|EMBEDIMAGE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pgraphic->m_bEmbedded = TRUE;
			else pgraphic->m_bEmbedded = FALSE;

			keyword = "|FILENAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pgraphic->m_csFileName = data;
			else pgraphic->m_csFileName.Empty();

			pgraphic->LoadBitmap();

			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pgraphic);

			if(pcomp != NULL) pcomp->m_arrayPos.Add(pos);
		}
		else if( data == "1" )  
		{						
			COb_schcomp* pcomp = new COb_schcomp();
			pcomp->pDocument = this;
			pcomp->m_nDiagram = nCurrentDiagram;	

			keyword = "|LIBREFERENCE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_csLibraryReference = data;
			else pcomp->m_csLibraryReference.Empty();		

			keyword = "|COMPONENTDESCRIPTION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_csDescription = data;
			else pcomp->m_csDescription.Empty();			

			keyword = "|PARTCOUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_nPartCount = atoi(data) - 1;  
			else pcomp->m_nPartCount = 1;			

			keyword = "|DISPLAYMODECOUNT=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_nDisplayModeCount = atoi(data);
			else pcomp->m_nDisplayModeCount = 1;			
			//|OWNERPARTID=-1
		
			int i1,i2;
			//LOCATION.X
			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pcomp->m_fX = i1*10.0 + i2/10000.0;
			//LOCATION.Y
			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pcomp->m_fY = i1*10.0 + i2/10000.0;	


			keyword = "|DISPLAYMODE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_nDisplayMode = atoi(data);
			else pcomp->m_nDisplayMode = 0;

			keyword = "|ISMIRRORED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_bMirrored = TRUE;
			else pcomp->m_bMirrored = FALSE;

			keyword = "|ORIENTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_nOrientation = atoi(data)*90;
			else pcomp->m_nOrientation = 0;
			pcomp->m_nOrientation = Get_Correct_Orientation(pcomp->m_nOrientation);

			keyword = "|CURRENTPARTID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_nPart = atoi(data);
			else pcomp->m_nPart = 1;

			keyword = "|SHOWHIDDENPINS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_bShowHidePins = TRUE;
			else pcomp->m_bShowHidePins = FALSE;

			keyword = "|LIBRARYPATH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_csLibraryPath = data;
			else pcomp->m_csLibraryPath.Empty();

			keyword = "|SOURCELIBRARYNAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_csLibraryFileName = data;
			else pcomp->m_csLibraryFileName.Empty();

			keyword = "|TARGETFILENAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_csTargetFileName = data;
			else pcomp->m_csTargetFileName.Empty();

			keyword = "|UNIQUEID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_csUniqueID = data;
			else pcomp->m_csUniqueID.Empty();

			keyword = "|AREACOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_nFillColor = atoi(data);
			else pcomp->m_nFillColor = RGB(0,0,0);
			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_nColor = atoi(data);
			else pcomp->m_nColor = RGB(0,0,0);	
			keyword = "|PINCOLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_nPinColor = atoi(data);
			else pcomp->m_nPinColor = RGB(0,0,0);


			keyword = "|OVERIDECOLORS=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_bColorLocked = TRUE;
			else pcomp->m_bColorLocked = FALSE;

			keyword = "|DESIGNATORLOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_bDesignatorLocked = TRUE;
			else pcomp->m_bDesignatorLocked = FALSE;

			keyword = "|PARTIDLOCKED=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
			{
				if(data == "T") pcomp->m_bPartLocked = TRUE;
				else pcomp->m_bPartLocked = FALSE;
			}
			else pcomp->m_bPartLocked = FALSE;

			keyword = "|PINSMOVEABLE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_bPinsMoveable = TRUE;
			else pcomp->m_bPinsMoveable = FALSE;

			keyword = "|COMPONENTKIND=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_nGraphicalType = atoi(data);
			else pcomp->m_nGraphicalType = 0;		
			keyword = "|COMPONENTKINDVERSION2=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pcomp->m_nGraphicalType = atoi(data);

			POSITION posParent = m_listSchObject[nCurrentDiagram].AddTail(pcomp);
			posArray.SetAt(linenum, posParent);
		}
		else if( data == "2" ) 
		{
			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			if(ownerindex < 0) continue;
			COb_schcomp* pcomp;
			POSITION posParent = posArray.GetAt(ownerindex);
			if(posParent == NULL) continue;

			CObject* pobject;
			pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				pcomp = (COb_schcomp*)pobject;
			}
			else continue;


			COb_schpin* ppin = new COb_schpin();
			ppin->pDocument = this;
			ppin->m_nDiagram = nCurrentDiagram;

			keyword = "|OWNERPARTID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppin->m_nOwnerPartID = atoi(data);
			else ppin->m_nOwnerPartID = 0;

			keyword = "|OWNERPARTDISPLAYMODE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppin->m_nDisplayMode = atoi(data);
			else ppin->m_nDisplayMode = 0;


			int i1,i2;
			//LOCATION.X
			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ppin->m_fX = i1*10.0 + i2/10000.0;
			//LOCATION.Y
			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ppin->m_fY = i1*10.0 + i2/10000.0;	
			//Length
			keyword = "|PINLENGTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|PINLENGTH_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			ppin->m_fPinLength = i1*10.0 + i2/10000.0;	

			keyword = "|PINCONGLOMERATE=";
			int pin_conglomerate;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pin_conglomerate = atoi(data);
			else pin_conglomerate = 0;
			ppin->m_nOrientation = (pin_conglomerate&0x03)*90;
			ppin->m_bHide = ((pin_conglomerate&0x04) == 0? FALSE: TRUE);
			ppin->m_bNameVisible = ((pin_conglomerate&0x08) == 0? FALSE: TRUE);
			ppin->m_bNumberVisible = ((pin_conglomerate&0x10) == 0? FALSE: TRUE);
			ppin->m_bPinLocked = ((pin_conglomerate&0x20) == 0? FALSE: TRUE);

			keyword = "|SYMBOL_INNEREDGE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppin->m_nInsideEdge = atoi(data);
			else ppin->m_nInsideEdge = 0;
			keyword = "|SYMBOL_OUTEREDGE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppin->m_nOutsideEdge = atoi(data);
			else ppin->m_nOutsideEdge = 0;
			keyword = "|SYMBOL_INNER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppin->m_nInside = atoi(data);
			else ppin->m_nInside = 0;
			keyword = "|SYMBOL_OUTER=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppin->m_nOutside = atoi(data);
			else ppin->m_nOutside = 0;

			keyword = "|ELECTRICAL=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppin->m_nElectricalType = atoi(data);
			else ppin->m_nElectricalType = 0;
					
			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppin->m_nColor = atoi(data);
			else ppin->m_nColor = RGB(0,0,0);

			keyword = "|NAME=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppin->m_csName = data;
			else ppin->m_csName.Empty();

			keyword = "|DESIGNATOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppin->m_csNumber = data;
			else ppin->m_csNumber.Empty();

			keyword = "|DESCRIPTION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppin->m_csDescription = data;
			else ppin->m_csDescription.Empty();

			ppin->m_csConnectTo = ppin->m_csName;
			ppin->m_csUniqueID.Empty();
			ppin->m_posParent = posParent;

			for(;;)
			{
				linenum++;
				cstring = stringarray.GetAt(linenum);
				keyword = "RECORD=";
				if(Get_KeyWord_Data(cstring, keyword, data) == FALSE) break;
				if( data == "41")
				{
					keyword = "|NAME=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) 
					{
						if(data == "HiddenNetName")
						{
							keyword = "|TEXT=";
							if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppin->m_csConnectTo = data;
						}
						else if(data == "PinUniqueId")
						{
							keyword = "|TEXT=";
							if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ppin->m_csUniqueID = data;
						}
					}
				}
				else 
				{
					linenum--;
					break;
				}
			}

			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ppin);

			pcomp->m_arrayPos.Add(pos);
		}
		else if( data == "3" ) 
		{
			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			if(ownerindex < 0) continue;
			COb_schcomp* pcomp;
			POSITION posParent = posArray.GetAt(ownerindex);
			if(posParent == NULL) continue;

			CObject* pobject;
			pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				pcomp = (COb_schcomp*)pobject;
			}
			else continue;

			COb_schieee* pieee = new COb_schieee();
			pieee->pDocument = this;
			pieee->m_nDiagram = nCurrentDiagram;

			keyword = "|OWNERPARTID=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pieee->m_nOwnerPartID = atoi(data);
			else pieee->m_nOwnerPartID = 0;

			keyword = "|OWNERPARTDISPLAYMODE=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pieee->m_nDisplayMode = atoi(data);
			else pieee->m_nDisplayMode = 0;

			int i1,i2;
			//LOCATION.X
			keyword = "|LOCATION.X=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.X_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pieee->m_fX = i1*10.0 + i2/10000.0;
			//LOCATION.Y
			keyword = "|LOCATION.Y=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|LOCATION.Y_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pieee->m_fY = i1*10.0 + i2/10000.0;	
			//Length
			keyword = "|SCALEFACTOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
			else i1 = 0;
			keyword = "|SCALEFACTOR_FRAC=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
			else i2 = 0;
			pieee->m_fSize = i1 + i2/10000.0;	

			keyword = "|ORIENTATION=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pieee->m_nOrientation = atoi(data)*90;
			else pieee->m_nOrientation = 0;
			pieee->m_nOrientation = Get_Correct_Orientation(pieee->m_nOrientation);

			keyword = "|LINEWIDTH=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pieee->m_nWidth = atoi(data);
			else pieee->m_nWidth = 0;

			keyword = "|COLOR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pieee->m_nColor = atoi(data);
			else pieee->m_nColor = RGB(0,0,0);

			keyword = "|MIRROR=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pieee->m_bMirrored = TRUE;
			else pieee->m_bMirrored = FALSE;

			keyword = "|SYMBOL=";
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) pieee->m_nIeeeSymbol = atoi(data);
			else pieee->m_nIeeeSymbol = 0;

			pieee->m_posParent = posParent;

			POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(pieee);

			pcomp->m_arrayPos.Add(pos);

		}
		else if( data == "34" )
		{
				//OWNERINDEX
				keyword = "|OWNERINDEX=";
				int ownerindex;
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
				else ownerindex = -1;

				if(ownerindex < 0) continue;
				COb_schcomp* pcomp;
				POSITION posParent = posArray.GetAt(ownerindex);
				if(posParent == NULL) continue;

				CObject* pobject;
				pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
				{
					pcomp = (COb_schcomp*)pobject;
				}
				else continue;
				
				COb_schtext* ptext = new COb_schtext();
				ptext->pDocument = this;
				ptext->m_nDiagram = nCurrentDiagram;	
				//分类
				ptext->m_nID = TEXT_COMPDESIGNATOR;
				ptext->m_csNameComment = "名称(Designator)";
				ptext->m_bNameLocked = TRUE;
					
				int i1,i2;
				//LOCATION.X
				keyword = "|LOCATION.X=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				keyword = "|LOCATION.X_FRAC=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
				else i2 = 0;
				ptext->m_fSx = i1*10.0 + i2/10000.0;
				//LOCATION.Y
				keyword = "|LOCATION.Y=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				keyword = "|LOCATION.Y_FRAC=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
				else i2 = 0;
				ptext->m_fSy = i1*10.0 + i2/10000.0;	
				//ORIENTATION
				keyword = "|ORIENTATION=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nOrientation = atoi(data)*90;
				else ptext->m_nOrientation = 0;
				ptext->m_nOrientation = Get_Correct_Orientation(ptext->m_nOrientation);

				keyword = "|JUSTIFICATION=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				ptext->m_nH_Justification = i1%3;
				ptext->m_nV_Justification = i1/3;

				keyword = "|COLOR=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nColor = atoi(data);
				else ptext->m_nColor = 0;
				//FONTID
				keyword = "|FONTID=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nFont = atoi(data) - 1;
				else ptext->m_nFont = 0;
				//Hidden
				keyword = "|ISHIDDEN=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bTextVisible = FALSE;
				else ptext->m_bTextVisible = TRUE;
				//TEXT
				keyword = "|TEXT=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csText = data;
				else ptext->m_csText.Empty();
				//PARAMTYPE
				keyword = "|PARAMTYPE=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nParamType = atoi(data);
				else ptext->m_nParamType = 0;
				//NameComment
				keyword = "|NAME=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csNameComment = data;
				//Show Name
				keyword = "|SHOWNAME=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bNameVisible = TRUE;
				else ptext->m_bNameVisible = FALSE;

				keyword = "|READONLYSTATE=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				ptext->m_bNameLocked = i1%2;
				ptext->m_bTextLocked = i1/2;
				//Unique ID
				keyword = "|UNIQUEID=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csUniqueID = data;
				else ptext->m_csUniqueID.Empty();
				//allow library synchronize
				keyword = "|NOTALLOWLIBRARYSYNCHRONIZE=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bAllowLibrarySynchronize = FALSE;
				else ptext->m_bAllowLibrarySynchronize = TRUE;
				//allow database synchronize
				keyword = "|NOTALLOWDATABASESYNCHRONIZE=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bAllowDataBaseSynchronize = FALSE;
				else ptext->m_bAllowDataBaseSynchronize = TRUE;

				keyword = "|NOTAUTOPOSITION=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bAutoPosition = FALSE;
				else ptext->m_bAutoPosition = TRUE;
				//Mirror
				keyword = "|ISMIRRORED=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bMirror = TRUE;
				else ptext->m_bMirror = FALSE;

				ptext->m_posParent = posParent;

				POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptext);

				pcomp->m_arrayPos.Add(pos);
	
				linenum++;
				cstring = stringarray.GetAt(linenum);
				keyword = "RECORD=";
				if( (Get_KeyWord_Data(cstring, keyword, data) == TRUE) && ( data == "41") )
				{

					COb_schtext* ptext = new COb_schtext();
					ptext->pDocument = this;
					ptext->m_nDiagram = nCurrentDiagram;	
						
					ptext->m_nID = TEXT_COMPCOMMENT;
					ptext->m_csNameComment = "标称值(Comment)";

					int i1,i2;
					//LOCATION.X
					keyword = "|LOCATION.X=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
					else i1 = 0;
					keyword = "|LOCATION.X_FRAC=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
					else i2 = 0;
					ptext->m_fSx = i1*10.0 + i2/10000.0;
					//LOCATION.Y
					keyword = "|LOCATION.Y=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
					else i1 = 0;
					keyword = "|LOCATION.Y_FRAC=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
					else i2 = 0;
					ptext->m_fSy = i1*10.0 + i2/10000.0;	
					//ORIENTATION
					keyword = "|ORIENTATION=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nOrientation = atoi(data)*90;
					else ptext->m_nOrientation = 0;
					ptext->m_nOrientation = Get_Correct_Orientation(ptext->m_nOrientation);
					//JUSTIFICATION
					keyword = "|JUSTIFICATION=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
					else i1 = 0;
					ptext->m_nH_Justification = i1%3;
					ptext->m_nV_Justification = i1/3;
					//COLOR
					keyword = "|COLOR=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nColor = atoi(data);
					else ptext->m_nColor = 0;
					//FONTID
					keyword = "|FONTID=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nFont = atoi(data) - 1;
					else ptext->m_nFont = 0;
					//Hidden
					keyword = "|ISHIDDEN=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bTextVisible = FALSE;
					else ptext->m_bTextVisible = TRUE;
					//TEXT
					keyword = "|TEXT=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csText = data;
					else ptext->m_csText.Empty();
					//PARAMTYPE
					keyword = "|PARAMTYPE=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nParamType = atoi(data);
					else ptext->m_nParamType = 0;
					//NameComment
					keyword = "|NAME=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csNameComment = data;
					//Show Name
					keyword = "|SHOWNAME=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bNameVisible = TRUE;
					else ptext->m_bNameVisible = FALSE;

					keyword = "|READONLYSTATE=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
					else i1 = 0;
					ptext->m_bNameLocked = i1%2;
					ptext->m_bTextLocked = i1/2;
					//Unique ID
					keyword = "|UNIQUEID=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csUniqueID = data;
					else ptext->m_csUniqueID.Empty();
					//allow library synchronize
					keyword = "|NOTALLOWLIBRARYSYNCHRONIZE=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bAllowLibrarySynchronize = FALSE;
					else ptext->m_bAllowLibrarySynchronize = TRUE;
					//allow database synchronize
					keyword = "|NOTALLOWDATABASESYNCHRONIZE=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bAllowDataBaseSynchronize = FALSE;
					else ptext->m_bAllowDataBaseSynchronize = TRUE;

					keyword = "|NOTAUTOPOSITION=";
					if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bAutoPosition = FALSE;
					else ptext->m_bAutoPosition = TRUE;

					ptext->m_posParent = posParent;
			
					POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptext);

					pcomp->m_arrayPos.Add(pos);
				}
				else linenum--;
		}
		else if( data == "41" ) 
		{
			//OWNERINDEX
			keyword = "|OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = 0;

			if(ownerindex == 0)
			{
				
				continue;
			}

			POSITION posParent = posArray.GetAt(ownerindex);
			if(posParent == NULL) continue;

			CObject* pobject;
			pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);

			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;

				COb_schtext* ptext = new COb_schtext();
				ptext->pDocument = this;
				ptext->m_nDiagram = nCurrentDiagram;	

				//Others
				ptext->m_nID = TEXT_COMPTEXT;

				int i1,i2;
				//LOCATION.X
				keyword = "|LOCATION.X=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				keyword = "|LOCATION.X_FRAC=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
				else i2 = 0;
				ptext->m_fSx = i1*10.0 + i2/10000.0;
				//LOCATION.Y
				keyword = "|LOCATION.Y=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				keyword = "|LOCATION.Y_FRAC=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i2 = atoi(data);
				else i2 = 0;
				ptext->m_fSy = i1*10.0 + i2/10000.0;	
				//ORIENTATION
				keyword = "|ORIENTATION=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nOrientation = atoi(data)*90;
				else ptext->m_nOrientation = 0;
				ptext->m_nOrientation = Get_Correct_Orientation(ptext->m_nOrientation);
				//JUSTIFICATION
				keyword = "|JUSTIFICATION=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				ptext->m_nH_Justification = i1%3;
				ptext->m_nV_Justification = i1/3;
				//COLOR
				keyword = "|COLOR=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nColor = atoi(data);
				else ptext->m_nColor = 0;
				//FONTID
				keyword = "|FONTID=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nFont = atoi(data) - 1;
				else ptext->m_nFont = 0;
				//Hidden
				keyword = "|ISHIDDEN=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bTextVisible = FALSE;
				else ptext->m_bTextVisible = TRUE;
				//TEXT
				keyword = "|TEXT=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csText = data;
				else ptext->m_csText.Empty();
				//PARAMTYPE
				keyword = "|PARAMTYPE=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_nParamType = atoi(data);
				else ptext->m_nParamType = 0;
				//NameComment
				keyword = "|NAME=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csNameComment = data;
				//Show Name
				keyword = "|SHOWNAME=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bNameVisible = TRUE;
				else ptext->m_bNameVisible = FALSE;

				keyword = "|READONLYSTATE=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) i1 = atoi(data);
				else i1 = 0;
				ptext->m_bNameLocked = i1%2;
				ptext->m_bTextLocked = i1/2;

				keyword = "|UNIQUEID=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csUniqueID = data;
				else ptext->m_csUniqueID.Empty();
				//allow library synchronize
				keyword = "|NOTALLOWLIBRARYSYNCHRONIZE=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bAllowLibrarySynchronize = FALSE;
				else ptext->m_bAllowLibrarySynchronize = TRUE;
				//allow database synchronize
				keyword = "|NOTALLOWDATABASESYNCHRONIZE=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bAllowDataBaseSynchronize = FALSE;
				else ptext->m_bAllowDataBaseSynchronize = TRUE;
				//not auto position自动放置
				keyword = "|NOTAUTOPOSITION=";
				if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_bAutoPosition = FALSE;
				else ptext->m_bAutoPosition = TRUE;

				ptext->m_posParent = posParent;
	
				POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptext);

				pcomp->m_arrayPos.Add(pos);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
			}
		}
		else if( data == "44") 
		{
			//OWNERINDEX
			keyword = "OWNERINDEX=";
			int ownerindex;
			if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ownerindex = atoi(data);
			else ownerindex = -1;

			if(ownerindex < 0) continue; 
			POSITION posParent = posArray.GetAt(ownerindex);
			if(posParent == NULL) continue;

			CObject* pobject;
			pobject = m_listSchObject[nCurrentDiagram].GetAt(posParent);

			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;

				for(;;)
				{
					linenum++;
					if(linenum >= total_linenum) break;
					cstring = stringarray.GetAt(linenum);
					keyword = "RECORD=";
					if(Get_KeyWord_Data(cstring, keyword, data) == FALSE) break;
					if( atoi(data) > 40)
					{
						if(data == "45")
						{
							keyword = "|MODELTYPE=";
							if((Get_KeyWord_Data(cstring, keyword, data) == TRUE) && (data == "PCBLIB"))
							{
								keyword = "|ISCURRENT=";
								if((Get_KeyWord_Data(cstring, keyword, data) == TRUE) && (data == "T"))
								{
									COb_schtext* ptext = new COb_schtext();
									ptext->pDocument = this;
									ptext->m_nDiagram = nCurrentDiagram;	

									//Others
									ptext->m_nID = TEXT_COMPPACKAGE;
									//TEXT
									keyword = "|MODELNAME=";
									if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csText = data;
									else ptext->m_csText.Empty();

									ptext->m_bTextVisible = FALSE;
									//NAME
									keyword = "|DESCRIPTION=";
									if(Get_KeyWord_Data(cstring, keyword, data) == TRUE) ptext->m_csNameComment = data;
									else ptext->m_csNameComment.Empty();
									ptext->m_bNameVisible = FALSE;
												
									//LOCATION.X
									ptext->m_fSx = pcomp->m_fX;
									//LOCATION.Y
									ptext->m_fSy = pcomp->m_fY;
									//ORIENTATION
									ptext->m_nOrientation = 0;
									//COLOR
									ptext->m_nColor = 0;
									//FONTID
									ptext->m_nFont = 0;

									ptext->m_bAutoPosition = FALSE;

									ptext->m_posParent = posParent;
						
									POSITION pos = m_listSchObject[nCurrentDiagram].AddTail(ptext);

									pcomp->m_arrayPos.Add(pos);
								}
							}
						}
					}
					else 
					{
						linenum--;
						break;
					}
				}
			}
		}
	}
}