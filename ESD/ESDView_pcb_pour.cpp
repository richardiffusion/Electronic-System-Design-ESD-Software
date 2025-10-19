BOOL CESDView::CreatePolygonPour(COb_pcbpolygon* ppolygon, int index)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	CTypedPtrArray <CObArray, COb_pcbregion*> arrayRegion;
	CTypedPtrArray <CObArray, COb_pcbregion*> arrayCutout;

	Write_Help("检索元素...");

	CArray<Fpoint, Fpoint&> arrayVertex;	
	pDoc->m_fArcPrecision = ppolygon->m_fArcResolution;
	
	ppolygon->GetPolygonVertex(arrayVertex);
	CleanVertex(arrayVertex);

	if(arrayVertex.GetCount() < 3) return FALSE;

	CArray<Struct_VertexOutline, Struct_VertexOutline&> arrayVertexOutline;

	for(int i=0; i<arrayVertex.GetCount(); i++)
	{
		Fpoint fp1, fp2;
		fp1 = arrayVertex.GetAt(i);
		if(i == (arrayVertex.GetCount() - 1)) fp2 = arrayVertex.GetAt(0);
		else fp2 = arrayVertex.GetAt(i + 1);

		Struct_VertexOutline vertexoutline;
		vertexoutline.kind = 0;
		vertexoutline.x1 = fp1.fx;
		vertexoutline.y1 = fp1.fy;
		vertexoutline.x2 = fp2.fx;
		vertexoutline.y2 = fp2.fy;
		vertexoutline.cx = vertexoutline.cy = vertexoutline.r = vertexoutline.sangle = vertexoutline.eangle = 0;
		arrayVertexOutline.Add(vertexoutline);
	}

	double fminx = 100000, fminy = 100000, fmaxx = -100000, fmaxy = -100000;
	for(int i=0; i<arrayVertex.GetCount(); i++)
	{
		Fpoint fpoint;
		fpoint = arrayVertex.GetAt(i);
		
		fminx = MIN(fminx, fpoint.fx);
		fminy = MIN(fminy, fpoint.fy);
		fmaxx = MAX(fmaxx, fpoint.fx);
		fmaxy = MAX(fmaxy, fpoint.fy);
	}

	if( ((fmaxx - fminx) > 32000) || ((fmaxy - fminy) > 32000) )
	{
		AfxMessageBox("覆铜区域太大, 超过系统允许边界, 命令终止。");
		return FALSE;
	}

	CArray<Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&> arrayObjects;

	COb_pcbregion aregion;
	for(int i=0; i<ppolygon->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex vertex;
		vertex = ppolygon->m_cVertex.GetAt(i);
		aregion.m_cVertex.Add(vertex);
	}
	aregion.m_nLayer = ppolygon->m_nLayer;
	aregion.m_nNet = ppolygon->m_nNet;
	aregion.pDocument = pDoc;

	double drc = ppolygon->m_fPourGap + ppolygon->m_fTrackWidth/2 + fEDA_pi/2;	
	double fTrackWidth = ppolygon->m_fTrackWidth;
	if((ppolygon->m_nHatchStyle == PCB_HATCH_SOLID)&&(ppolygon->m_bRemoveNecks == TRUE))
	{
		drc = ppolygon->m_fPourGap + ppolygon->m_fTrackWidth/2 + ppolygon->m_fNeckWidthThreshold/2 + fEDA_pi/2;		
		fTrackWidth = ppolygon->m_fTrackWidth + ppolygon->m_fNeckWidthThreshold;
	}

	for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);

		if(ptrack->m_nFlag != FLAG_NORMAL) continue;
		if((ppolygon->m_nPourOverStyle == 1)&&(ptrack->m_nNet == ppolygon->m_nNet)) continue;

		if( ((aregion.m_nLayer == MULTILAYER)||(aregion.m_nLayer == ptrack->m_nLayer)||(ptrack->m_nLayer == MULTILAYER))&&\
			(Line_Region_Connect(ptrack, &aregion, drc) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_TRACK;
			selected_element.index = j;
			arrayObjects.Add(selected_element);
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
		if(ppad->m_nFlag != FLAG_NORMAL) continue;
		if((ppolygon->m_nPourOverStyle == 1)&&(ppad->m_nNet == ppolygon->m_nNet)) continue;
		
		if( ((aregion.m_nLayer == MULTILAYER)||(aregion.m_nLayer == ppad->m_nLayer)||(ppad->m_nLayer == MULTILAYER))&&\
			(Pad_Region_Connect(ppad, &aregion, drc) == TRUE) )
		{			
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_PAD;
			selected_element.index = j;
			arrayObjects.Add(selected_element);
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
		if(pvia->m_nFlag != FLAG_NORMAL) continue;
		if((ppolygon->m_nPourOverStyle == 1)&&(pvia->m_nNet == ppolygon->m_nNet)) continue;

		if((pDoc->IsConnectedToVia(aregion.m_nLayer, pvia) == TRUE)&&(Via_Region_Connect(pvia, &aregion, drc) == TRUE))
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_VIA;
			selected_element.index = j;
			arrayObjects.Add(selected_element);
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
		if(parc->m_nFlag != FLAG_NORMAL) continue;
		if((ppolygon->m_nPourOverStyle == 1)&&(parc->m_nNet == ppolygon->m_nNet)) continue;
			
		if( ((aregion.m_nLayer == MULTILAYER)||(aregion.m_nLayer == parc->m_nLayer)||(parc->m_nLayer == MULTILAYER))&&\
			(Arc_Region_Connect(parc, &aregion, drc) == TRUE) )
		{	
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_ARC;
			selected_element.index = j;
			arrayObjects.Add(selected_element);
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
		if(pfill->m_nFlag != FLAG_NORMAL) continue;
		if((ppolygon->m_nPourOverStyle == 1)&&(pfill->m_nNet == ppolygon->m_nNet)) continue;

		if( ((aregion.m_nLayer == MULTILAYER)||(aregion.m_nLayer == pfill->m_nLayer)||(pfill->m_nLayer == MULTILAYER))&&\
			(Fill_Region_Connect(pfill, &aregion, drc) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_FILL;
			selected_element.index = j;
			arrayObjects.Add(selected_element);
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbText.GetCount(); j++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(j);
		if(ptext->m_nFlag != FLAG_NORMAL) continue;

		if((aregion.m_nLayer == MULTILAYER)||(aregion.m_nLayer == ptext->m_nLayer)||(ptext->m_nLayer == MULTILAYER))
		{
			COb_pcbfill afill;
			afill.pDocument = ptext->pDocument;
			afill.m_fX1 = ptext->m_fX1;
			afill.m_fY1 = ptext->m_fY1;
			afill.m_fX2 = ptext->m_fX2;
			afill.m_fY2 = ptext->m_fY2;
			afill.m_fRotation = ptext->m_fRotation;

			if(Fill_Region_Connect(&afill, &aregion, drc) == TRUE)
			{
				Struct_Pcb_Multi_Selection selected_element;
				selected_element.pcbelement = PCB_ELEMENT_TEXT;
				selected_element.index = j;
				arrayObjects.Add(selected_element);
			}
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);

		if(pregion->m_nFlag != FLAG_NORMAL) continue;
		if((ppolygon->m_nPourOverStyle == 1)&&(pregion->m_nNet == ppolygon->m_nNet)) continue;
		
		if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == aregion.m_nLayer)||(aregion.m_nLayer == MULTILAYER))&&\
			(Region_Region_Connect(&aregion, pregion, drc) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_REGION;
			selected_element.index = j;
			arrayObjects.Add(selected_element);
		}
	}

	int cx = (int)(fmaxx - fminx) + 1;
	int cy = (int)(fmaxy - fminy) + 1;
	
	int dx = cx/8;
	if((cx%8) > 0) dx++;
	if((dx%2) > 0) dx++;

	int buffer_count = dx*cy;

	char* buffer = (char*)malloc(buffer_count);
	for(int i=0; i<buffer_count; i++) buffer[i] = 0;

	CDC memdc;
	memdc.CreateCompatibleDC(NULL);

	CBitmap bmpFace; 
	bmpFace.CreateCompatibleBitmap(&memdc, cx, cy); 
	CBitmap* pOldBmp = NULL;
	pOldBmp = memdc.SelectObject(&bmpFace); 
	//memdc.FillSolidRect(0,0,cx,cy, RGB(255,255,255));
	
	// draw with a thick blue pen
	CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* pOldPen = memdc.SelectObject(&pen);
	
	// and a solid red brush
	CBrush brush(RGB(255, 255, 255));
	CBrush* pOldBrush = memdc.SelectObject(&brush);

	PolygonPourDrawVertex(&memdc, arrayVertex, fminx, fminy);

	// draw with a thick blue pen
	CPen pen1(PS_SOLID, 1, RGB(0, 0, 0));
	memdc.SelectObject(&pen1);
	// and a solid red brush
	CBrush brush1(RGB(0, 0, 0));
	memdc.SelectObject(&brush1);

	for(int i=0; i<arrayObjects.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selected_element;
		selected_element = arrayObjects.GetAt(i);
		if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);


			int x1, y1, x2, y2, width;
			x1 = (int)(ptrack->m_fX1 - fminx);	y1 = (int)(ptrack->m_fY1 - fminy);
			x2 = (int)(ptrack->m_fX2 - fminx);	y2 = (int)(ptrack->m_fY2 - fminy);
			width = (int)(ptrack->m_fWidth + 2*drc);
			
			CPen pen_track(PS_SOLID, width, RGB(0, 0, 0));
			CPen* poldpen = memdc.SelectObject(&pen_track);
			memdc.MoveTo(x1, y1);
			memdc.LineTo(x2, y2);
			memdc.SelectObject(poldpen);
            
			ptrack->GetTrackVertexOutline(arrayVertexOutline, drc);
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);

			int cx, cy, r, width;
			cx = (int)(parc->m_fCx - fminx);	cy = (int)(parc->m_fCy - fminy);
			r = (int)parc->m_fRadius;
			width = (int)(parc->m_fWidth + 2*drc);

			CPen pen_arc(PS_SOLID, width, RGB(0, 0, 0));
			CPen* poldpen = memdc.SelectObject(&pen_arc);
			
			CRect rect;
			CPoint sp, ep;

			rect.left = cx - r;
			rect.right = cx + r;
			rect.top = cy + r;
			rect.bottom = cy - r;


			if((parc->m_fEangle == 0)||(parc->m_fEangle == 360))
			{
				sp.x = cx + r;
				sp.y = cy;
			}
			else if(parc->m_fEangle == 90)
			{
				sp.x = cx;
				sp.y = cy + r;
			}
			else if(parc->m_fEangle == 180)
			{
				sp.x = cx - r;
				sp.y = cy;
			}
			else if(parc->m_fEangle == 270)
			{
				sp.x = cx;
				sp.y = cy - r;
			}
			else
			{
				if((parc->m_fEangle > 0)&&(parc->m_fEangle < 180))
				{
					sp.x = cx + (int)(r/tan(parc->m_fEangle*fEDA_pi/180));
					sp.y = cy + r;
				}
				else
				{
					sp.x = cx - (int)(r/tan(parc->m_fEangle*fEDA_pi/180));
					sp.y = cy - r;
				}
			}

			if((parc->m_fSangle == 0)||(parc->m_fSangle == 360))
			{
				ep.x = cx + r;
				ep.y = cy;
			}
			else if(parc->m_fSangle == 180)
			{
				ep.x = cx - r;
				ep.y = cy;
			}
			else
			{
				if((parc->m_fSangle > 0)&&(parc->m_fSangle < 180))
				{
					ep.x = cx + (int)(r/tan(parc->m_fSangle*fEDA_pi/180));
					ep.y = cy + r;
				}
				else
				{
					ep.x = cx - (int)(r/tan(parc->m_fSangle*fEDA_pi/180));
					ep.y = cy - r;
				}
			}
			memdc.Arc(rect.left, rect.top, rect.right, rect.bottom, sp.x, sp.y, ep.x, ep.y);
			memdc.SelectObject(poldpen);
			
			parc->GetArcVertexOutline(arrayVertexOutline, drc);
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
			int cx, cy, r;
			cx = (int)(pvia->m_fCx - fminx);	cy = (int)(pvia->m_fCy - fminy);
			r = (int)(pvia->m_fDia/2 + drc);

			CPen pen_via(PS_SOLID, 1, RGB(0, 0, 0));
			CPen* poldpen = memdc.SelectObject(&pen_via);
			CBrush brush_via(RGB(0, 0, 0));
			CBrush* poldbrush = memdc.SelectObject(&brush_via);

			CRect rect;
			rect.left = cx - r;
			rect.right = cx + r;
			rect.top = cy + r;
			rect.bottom = cy - r;
			memdc.Ellipse(&rect);
			memdc.SelectObject(poldpen);
			memdc.SelectObject(poldbrush);
			
			pvia->GetViaVertexOutline(arrayVertexOutline, drc);
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);

			double startx, starty;
			Fpoint fp;

			int shape;
			int padsize_x;
			int padsize_y;
			if(ppad->m_nPadMode == PAD_MODE_SIMPLE)
			{
				shape = ppad->m_nShape[TOPLAYER];
				padsize_x = ppad->m_fSizeX[TOPLAYER];
				padsize_y = ppad->m_fSizeY[TOPLAYER];
			}
			else
			{
				if((ppolygon->m_nLayer >= TOPLAYER)&&(ppolygon->m_nLayer <= BOTTOMLAYER)) 
				{
					shape = ppad->m_nShape[ppolygon->m_nLayer];
					padsize_x = ppad->m_fSizeX[ppolygon->m_nLayer];
					padsize_y = ppad->m_fSizeY[ppolygon->m_nLayer];
				}
				else
				{
					shape = ppad->m_nShape[TOPLAYER];
					padsize_x = ppad->m_fSizeX[TOPLAYER];
					padsize_y = ppad->m_fSizeY[TOPLAYER];
				}
			}

			if((padsize_x != 0)||(padsize_y != 0))
			{
				if(shape == PAD_SHAPE_ROUND)
				{
					if(padsize_x == padsize_y)
					{
						int cx, cy, r;
						cx = (int)(ppad->m_fCx - fminx);	cy = (int)(ppad->m_fCy - fminy);
						r = (int)(padsize_x/2 + drc);

						CPen pen_pad(PS_SOLID, 1, RGB(0, 0, 0));
						CPen* poldpen = memdc.SelectObject(&pen_pad);
						CBrush brush_pad(RGB(0, 0, 0));
						CBrush* poldbrush = memdc.SelectObject(&brush_pad);

						CRect rect;
						rect.left = cx - r;
						rect.right = cx + r;
						rect.top = cy + r;
						rect.bottom = cy - r;
						memdc.Ellipse(&rect);
						memdc.SelectObject(poldpen);
						memdc.SelectObject(poldbrush);
						
						Struct_VertexOutline vertexoutline;
						vertexoutline.kind = 1;
						vertexoutline.cx = ppad->m_fCx;
						vertexoutline.cy = ppad->m_fCy;
						vertexoutline.r = padsize_x/2 + drc +1; 
						vertexoutline.sangle = 0;
						vertexoutline.eangle = 360;
						vertexoutline.x1 = vertexoutline.cx + vertexoutline.r;
						vertexoutline.y1 = vertexoutline.cy;
						vertexoutline.x2 = vertexoutline.cx + vertexoutline.r;
						vertexoutline.y2 = vertexoutline.cy;
						arrayVertexOutline.Add(vertexoutline);
					}
					else if(padsize_x > padsize_y)
					{
						COb_pcbtrack atrack;
						double fcx, fcy;
						int x1, y1, x2, y2, width;
						
						
						fcx = ppad->m_fCx - cos(ppad->m_fRotation*fEDA_pi/180)*(padsize_x - padsize_y)/2;
						fcy = ppad->m_fCy - sin(ppad->m_fRotation*fEDA_pi/180)*(padsize_x - padsize_y)/2;
						atrack.m_fX1 = fcx;
						atrack.m_fY1 = fcy;

						x1 = (int)(fcx - fminx);	y1 = (int)(fcy - fminy);

						
						fcx = ppad->m_fCx + cos(ppad->m_fRotation*fEDA_pi/180)*(padsize_x - padsize_y)/2;
						fcy = ppad->m_fCy + sin(ppad->m_fRotation*fEDA_pi/180)*(padsize_x - padsize_y)/2;
						atrack.m_fX2 = fcx;
						atrack.m_fY2 = fcy;
						x2 = (int)(fcx- fminx);	y2 = (int)(fcy - fminy);
						
						
						width = (int)(padsize_y + 2*drc);
						
						CPen pen_track(PS_SOLID, width, RGB(0, 0, 0));
						CPen* poldpen = memdc.SelectObject(&pen_track);
						memdc.MoveTo(x1, y1);
						memdc.LineTo(x2, y2);
						memdc.SelectObject(poldpen);

						atrack.pDocument = pDoc;
						atrack.m_nLayer = ppolygon->m_nLayer;
						atrack.m_fWidth = padsize_y;
						
						atrack.GetTrackVertexOutline(arrayVertexOutline, drc);
					}
					else
					{
						COb_pcbtrack atrack;
						double fcx, fcy;
						int x1, y1, x2, y2, width;

						
						fcx = ppad->m_fCx - sin(ppad->m_fRotation*fEDA_pi/180)*(padsize_y - padsize_x)/2;
						fcy = ppad->m_fCy + cos(ppad->m_fRotation*fEDA_pi/180)*(padsize_y - padsize_x)/2;
						atrack.m_fX1 = fcx;
						atrack.m_fY1 = fcy;
						x1 = (int)(fcx - fminx);	y1 = (int)(fcy - fminy);
						
						
						fcx = ppad->m_fCx + sin(ppad->m_fRotation*fEDA_pi/180)*(padsize_y - padsize_x)/2;
						fcy = ppad->m_fCy - cos(ppad->m_fRotation*fEDA_pi/180)*(padsize_y - padsize_x)/2;
						atrack.m_fX2 = fcx;
						atrack.m_fY2 = fcy;
						x2 = (int)(fcx- fminx);	y2 = (int)(fcy - fminy);

						
						width = (int)(padsize_x + 2*drc);

						CPen pen_track(PS_SOLID, width, RGB(0, 0, 0));
						CPen* poldpen = memdc.SelectObject(&pen_track);
						memdc.MoveTo(x1, y1);
						memdc.LineTo(x2, y2);
						memdc.SelectObject(poldpen);

						atrack.pDocument = pDoc;
						atrack.m_nLayer = ppolygon->m_nLayer;
						atrack.m_fWidth = padsize_x;
						
						atrack.GetTrackVertexOutline(arrayVertexOutline, drc);
					}
				}
				else if(shape == PAD_SHAPE_RECTANGLE)
				{
					COb_pcbfill pcbfill;
					pcbfill.m_nLayer = ppolygon->m_nLayer;
					pcbfill.m_fX1 = ppad->m_fCx - padsize_x/2 - drc;
					pcbfill.m_fY1 = ppad->m_fCy - padsize_y/2 - drc;
					pcbfill.m_fX2 = ppad->m_fCx + padsize_x/2 + drc;
					pcbfill.m_fY2 = ppad->m_fCy + padsize_y/2 + drc;
					pcbfill.m_fRotation = ppad->m_fRotation;
					pcbfill.m_nFlag = ppad->m_nFlag;

					double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
					pcbfill.GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	
					
					CArray<Fpoint, Fpoint&> arrayVertex;
					Fpoint fp;
					fp.fx = fx1;	fp.fy = fy1;	arrayVertex.Add(fp);
					fp.fx = fx3;	fp.fy = fy3;	arrayVertex.Add(fp);
					fp.fx = fx2;	fp.fy = fy2;	arrayVertex.Add(fp);
					fp.fx = fx4;	fp.fy = fy4;	arrayVertex.Add(fp);

					CPen pen_pad(PS_SOLID, 1, RGB(0, 0, 0));
					CPen* poldpen = memdc.SelectObject(&pen_pad);
					CBrush brush_pad(RGB(0, 0, 0));
					CBrush* poldbrush = memdc.SelectObject(&brush_pad);
					PolygonPourDrawVertex(&memdc, arrayVertex, fminx, fminy);
					memdc.SelectObject(poldpen);
					memdc.SelectObject(poldbrush);
					
					
					pcbfill.m_fX1 -= 1;
					pcbfill.m_fY1 -= 1;
					pcbfill.m_fX2 += 1;
					pcbfill.m_fY2 += 1;
					pcbfill.GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	
					arrayVertex.RemoveAll();
					fp.fx = fx1;	fp.fy = fy1;	arrayVertex.Add(fp);
					fp.fx = fx3;	fp.fy = fy3;	arrayVertex.Add(fp);
					fp.fx = fx2;	fp.fy = fy2;	arrayVertex.Add(fp);
					fp.fx = fx4;	fp.fy = fy4;	arrayVertex.Add(fp);
		

					for(int i=0; i<arrayVertex.GetCount(); i++)
					{
						Fpoint fp1, fp2;
						fp1 = arrayVertex.GetAt(i);
						if(i == (arrayVertex.GetCount() - 1)) fp2 = arrayVertex.GetAt(0);
						else fp2 = arrayVertex.GetAt(i + 1);

						Struct_VertexOutline vertexoutline;
						vertexoutline.kind = 0;
						vertexoutline.x1 = fp1.fx;
						vertexoutline.y1 = fp1.fy;
						vertexoutline.x2 = fp2.fx;
						vertexoutline.y2 = fp2.fy;
						vertexoutline.cx = vertexoutline.cy = vertexoutline.r = vertexoutline.sangle = vertexoutline.eangle = 0;
						arrayVertexOutline.Add(vertexoutline);
					}
				}
				else
				{
					Fpoint fpoint[9];
					if( padsize_x >= padsize_y)
					{
						
						fpoint[0].fx = ppad->m_fCx - (padsize_x + 2*drc)/2 + (padsize_y + 2*drc)/4;
						fpoint[0].fy = ppad->m_fCy - (padsize_y + 2*drc)/2;				
						fpoint[1].fx = ppad->m_fCx - (padsize_x + 2*drc)/2;
						fpoint[1].fy = ppad->m_fCy - (padsize_y + 2*drc)/4;
						fpoint[2].fx = ppad->m_fCx - (padsize_x + 2*drc)/2;
						fpoint[2].fy = ppad->m_fCy + (padsize_y + 2*drc)/4;
						fpoint[3].fx = ppad->m_fCx - (padsize_x + 2*drc)/2 + (padsize_y + 2*drc)/4;
						fpoint[3].fy = ppad->m_fCy + (padsize_y + 2*drc)/2;
						fpoint[4].fx = ppad->m_fCx + (padsize_x + 2*drc)/2 - (padsize_y + 2*drc)/4;
						fpoint[4].fy = ppad->m_fCy + (padsize_y + 2*drc)/2;
						fpoint[5].fx = ppad->m_fCx + (padsize_x + 2*drc)/2;
						fpoint[5].fy = ppad->m_fCy + (padsize_y + 2*drc)/4;
						fpoint[6].fx = ppad->m_fCx + (padsize_x + 2*drc)/2;
						fpoint[6].fy = ppad->m_fCy - (padsize_y + 2*drc)/4;
						fpoint[7].fx = ppad->m_fCx + (padsize_x + 2*drc)/2 - (padsize_y + 2*drc)/4;
						fpoint[7].fy = ppad->m_fCy - (padsize_y + 2*drc)/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad->m_fCx;
							fpoint[i].fy -= ppad->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad->m_fRotation == 0)||(ppad->m_fRotation == 180)||(ppad->m_fRotation == 360))
							{
								fpoint[i].fx = ppad->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad->m_fCy + fpoint[i].fy;
							}
							else if((ppad->m_fRotation == 90)||(ppad->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad->m_fCx + fr*cos(alpha + ppad->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad->m_fCy + fr*sin(alpha + ppad->m_fRotation*fEDA_pi/180);
							}
						}
					}
					else
					{
						fpoint[0].fx = ppad->m_fCx - (padsize_x+2*drc)/4;
						fpoint[0].fy = ppad->m_fCy - (padsize_y+2*drc)/2;
						fpoint[1].fx = ppad->m_fCx - (padsize_x+2*drc)/2;
						fpoint[1].fy = ppad->m_fCy - (padsize_y+2*drc)/2 + (padsize_x+2*drc)/4;
						fpoint[2].fx = ppad->m_fCx - (padsize_x+2*drc)/2;
						fpoint[2].fy = ppad->m_fCy + (padsize_y+2*drc)/2 - (padsize_x+2*drc)/4;
						fpoint[3].fx = ppad->m_fCx - (padsize_x+2*drc)/4;
						fpoint[3].fy = ppad->m_fCy + (padsize_y+2*drc)/2;
						fpoint[4].fx = ppad->m_fCx + (padsize_x+2*drc)/4;
						fpoint[4].fy = ppad->m_fCy + (padsize_y+2*drc)/2;
						fpoint[5].fx = ppad->m_fCx + (padsize_x+2*drc)/2;
						fpoint[5].fy = ppad->m_fCy + (padsize_y+2*drc)/2 - (padsize_x+2*drc)/4;
						fpoint[6].fx = ppad->m_fCx + (padsize_x+2*drc)/2;
						fpoint[6].fy = ppad->m_fCy - (padsize_y+2*drc)/2 + (padsize_x+2*drc)/4;
						fpoint[7].fx = ppad->m_fCx + (padsize_x+2*drc)/4;
						fpoint[7].fy = ppad->m_fCy - (padsize_y+2*drc)/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad->m_fCx;
							fpoint[i].fy -= ppad->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad->m_fRotation == 0)||(ppad->m_fRotation == 180)||(ppad->m_fRotation == 360))
							{
								fpoint[i].fx = ppad->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad->m_fCy + fpoint[i].fy;
							}
							else if((ppad->m_fRotation == 90)||(ppad->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad->m_fCx + fr*cos(alpha + ppad->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad->m_fCy + fr*sin(alpha + ppad->m_fRotation*fEDA_pi/180);
							}
						}
					}

					
					CArray<Fpoint, Fpoint&> arrayVertex;
					for(int i=0; i<8; i++)
					{
						arrayVertex.Add(fpoint[i]);
					}
					
					CPen pen_pad(PS_SOLID, 1, RGB(0, 0, 0));
					CPen* poldpen = memdc.SelectObject(&pen_pad);
					CBrush brush_pad(RGB(0, 0, 0));
					CBrush* poldbrush = memdc.SelectObject(&brush_pad);
					PolygonPourDrawVertex(&memdc, arrayVertex, fminx, fminy);
					memdc.SelectObject(poldpen);
					memdc.SelectObject(poldbrush);
					
					
					if( padsize_x >= padsize_y)
					{
						
						fpoint[0].fx = ppad->m_fCx - (padsize_x + 2*drc + 2)/2 + (padsize_y + 2*drc + 2)/4;
						fpoint[0].fy = ppad->m_fCy - (padsize_y + 2*drc + 2)/2;				
						fpoint[1].fx = ppad->m_fCx - (padsize_x + 2*drc + 2)/2;
						fpoint[1].fy = ppad->m_fCy - (padsize_y + 2*drc + 2)/4;
						fpoint[2].fx = ppad->m_fCx - (padsize_x + 2*drc + 2)/2;
						fpoint[2].fy = ppad->m_fCy + (padsize_y + 2*drc + 2)/4;
						fpoint[3].fx = ppad->m_fCx - (padsize_x + 2*drc + 2)/2 + (padsize_y + 2*drc + 2)/4;
						fpoint[3].fy = ppad->m_fCy + (padsize_y + 2*drc + 2)/2;
						fpoint[4].fx = ppad->m_fCx + (padsize_x + 2*drc + 2)/2 - (padsize_y + 2*drc + 2)/4;
						fpoint[4].fy = ppad->m_fCy + (padsize_y + 2*drc + 2)/2;
						fpoint[5].fx = ppad->m_fCx + (padsize_x + 2*drc + 2)/2;
						fpoint[5].fy = ppad->m_fCy + (padsize_y + 2*drc + 2)/4;
						fpoint[6].fx = ppad->m_fCx + (padsize_x + 2*drc + 2)/2;
						fpoint[6].fy = ppad->m_fCy - (padsize_y + 2*drc + 2)/4;
						fpoint[7].fx = ppad->m_fCx + (padsize_x + 2*drc + 2)/2 - (padsize_y + 2*drc + 2)/4;
						fpoint[7].fy = ppad->m_fCy - (padsize_y + 2*drc + 2)/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad->m_fCx;
							fpoint[i].fy -= ppad->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad->m_fRotation == 0)||(ppad->m_fRotation == 180)||(ppad->m_fRotation == 360))
							{
								fpoint[i].fx = ppad->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad->m_fCy + fpoint[i].fy;
							}
							else if((ppad->m_fRotation == 90)||(ppad->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad->m_fCx + fr*cos(alpha + ppad->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad->m_fCy + fr*sin(alpha + ppad->m_fRotation*fEDA_pi/180);
							}
						}
					}
					else
					{
						fpoint[0].fx = ppad->m_fCx - (padsize_x+2*drc + 2)/4;
						fpoint[0].fy = ppad->m_fCy - (padsize_y+2*drc + 2)/2;
						fpoint[1].fx = ppad->m_fCx - (padsize_x+2*drc + 2)/2;
						fpoint[1].fy = ppad->m_fCy - (padsize_y+2*drc + 2)/2 + (padsize_x + 2*drc + 2)/4;
						fpoint[2].fx = ppad->m_fCx - (padsize_x+2*drc + 2)/2;
						fpoint[2].fy = ppad->m_fCy + (padsize_y+2*drc + 2)/2 - (padsize_x + 2*drc + 2)/4;
						fpoint[3].fx = ppad->m_fCx - (padsize_x+2*drc + 2)/4;
						fpoint[3].fy = ppad->m_fCy + (padsize_y+2*drc + 2)/2;
						fpoint[4].fx = ppad->m_fCx + (padsize_x+2*drc + 2)/4;
						fpoint[4].fy = ppad->m_fCy + (padsize_y+2*drc + 2)/2;
						fpoint[5].fx = ppad->m_fCx + (padsize_x+2*drc + 2)/2;
						fpoint[5].fy = ppad->m_fCy + (padsize_y+2*drc + 2)/2 - (padsize_x + 2*drc + 2)/4;
						fpoint[6].fx = ppad->m_fCx + (padsize_x+2*drc + 2)/2;
						fpoint[6].fy = ppad->m_fCy - (padsize_y+2*drc + 2)/2 + (padsize_x + 2*drc + 2)/4;
						fpoint[7].fx = ppad->m_fCx + (padsize_x+2*drc + 2)/4;
						fpoint[7].fy = ppad->m_fCy - (padsize_y+2*drc + 2)/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad->m_fCx;
							fpoint[i].fy -= ppad->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad->m_fRotation == 0)||(ppad->m_fRotation == 180)||(ppad->m_fRotation == 360))
							{
								fpoint[i].fx = ppad->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad->m_fCy + fpoint[i].fy;
							}
							else if((ppad->m_fRotation == 90)||(ppad->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad->m_fCx + fr*cos(alpha + ppad->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad->m_fCy + fr*sin(alpha + ppad->m_fRotation*fEDA_pi/180);
							}
						}
					}
					arrayVertex.RemoveAll();
					for(int i=0; i<8; i++)
					{
						arrayVertex.Add(fpoint[i]);
					}
					

					for(int i=0; i<arrayVertex.GetCount(); i++)
					{
						Fpoint fp1, fp2;
						fp1 = arrayVertex.GetAt(i);
						if(i == (arrayVertex.GetCount() - 1)) fp2 = arrayVertex.GetAt(0);
						else fp2 = arrayVertex.GetAt(i + 1);

						Struct_VertexOutline vertexoutline;
						vertexoutline.kind = 0;
						vertexoutline.x1 = fp1.fx;
						vertexoutline.y1 = fp1.fy;
						vertexoutline.x2 = fp2.fx;
						vertexoutline.y2 = fp2.fy;
						vertexoutline.cx = vertexoutline.cy = vertexoutline.r = vertexoutline.sangle = vertexoutline.eangle = 0;
						arrayVertexOutline.Add(vertexoutline);
					}
				}
			}
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
			arrayVertex.RemoveAll();

			COb_pcbfill afill;
			afill.Copy(pfill);
			afill.pDocument = pDoc;
			afill.m_fX1 -= drc;
			afill.m_fY1 -= drc;
			afill.m_fX2 += drc;
			afill.m_fY2 += drc;
			double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
			afill.GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	
			
			CArray<Fpoint, Fpoint&> arrayVertex;
			Fpoint fp;
			fp.fx = fx1;	fp.fy = fy1;	arrayVertex.Add(fp);
			fp.fx = fx3;	fp.fy = fy3;	arrayVertex.Add(fp);
			fp.fx = fx2;	fp.fy = fy2;	arrayVertex.Add(fp);
			fp.fx = fx4;	fp.fy = fy4;	arrayVertex.Add(fp);

			CPen pen_fill(PS_SOLID, 1, RGB(0, 0, 0));
			CPen* poldpen = memdc.SelectObject(&pen_fill);
			CBrush brush_fill(RGB(0, 0, 0));
			CBrush* poldbrush = memdc.SelectObject(&brush_fill);
			PolygonPourDrawVertex(&memdc, arrayVertex, fminx, fminy);
			memdc.SelectObject(poldpen);
			memdc.SelectObject(poldbrush);
			
			
			afill.m_fX1 -= 1;
			afill.m_fY1 -= 1;
			afill.m_fX2 += 1;
			afill.m_fY2 += 1;
			afill.GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	
			arrayVertex.RemoveAll();
			fp.fx = fx1;	fp.fy = fy1;	arrayVertex.Add(fp);
			fp.fx = fx3;	fp.fy = fy3;	arrayVertex.Add(fp);
			fp.fx = fx2;	fp.fy = fy2;	arrayVertex.Add(fp);
			fp.fx = fx4;	fp.fy = fy4;	arrayVertex.Add(fp);
			

			
			for(int i=0; i<arrayVertex.GetCount(); i++)
			{
				Fpoint fp1, fp2;
				fp1 = arrayVertex.GetAt(i);
				if(i == (arrayVertex.GetCount() - 1)) fp2 = arrayVertex.GetAt(0);
				else fp2 = arrayVertex.GetAt(i + 1);

				Struct_VertexOutline vertexoutline;
				vertexoutline.kind = 0;
				vertexoutline.x1 = fp1.fx;
				vertexoutline.y1 = fp1.fy;
				vertexoutline.x2 = fp2.fx;
				vertexoutline.y2 = fp2.fy;
				vertexoutline.cx = vertexoutline.cy = vertexoutline.r = vertexoutline.sangle = vertexoutline.eangle = 0;
				arrayVertexOutline.Add(vertexoutline);
			}
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_TEXT)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(selected_element.index);
			arrayVertex.RemoveAll();

			COb_pcbfill afill;
			afill.pDocument = pDoc;
			afill.m_fX1 = ptext->m_fX1 - drc;
			afill.m_fY1 = ptext->m_fY1 - drc;
			afill.m_fX2 = ptext->m_fX2 + drc;
			afill.m_fY2 = ptext->m_fY2 + drc;
			afill.m_fRotation = ptext->m_fRotation;

			double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
			afill.GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	
			
			CArray<Fpoint, Fpoint&> arrayVertex;
			Fpoint fp;
			fp.fx = fx1;	fp.fy = fy1;	arrayVertex.Add(fp);
			fp.fx = fx3;	fp.fy = fy3;	arrayVertex.Add(fp);
			fp.fx = fx2;	fp.fy = fy2;	arrayVertex.Add(fp);
			fp.fx = fx4;	fp.fy = fy4;	arrayVertex.Add(fp);

			CPen pen_fill(PS_SOLID, 1, RGB(0, 0, 0));
			CPen* poldpen = memdc.SelectObject(&pen_fill);
			CBrush brush_fill(RGB(0, 0, 0));
			CBrush* poldbrush = memdc.SelectObject(&brush_fill);
			PolygonPourDrawVertex(&memdc, arrayVertex, fminx, fminy);
			memdc.SelectObject(poldpen);
			memdc.SelectObject(poldbrush);

			
			afill.m_fX1 -= 1;
			afill.m_fY1 -= 1;
			afill.m_fX2 += 1;
			afill.m_fY2 += 1;
			afill.GetFillCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);	
			arrayVertex.RemoveAll();
			fp.fx = fx1;	fp.fy = fy1;	arrayVertex.Add(fp);
			fp.fx = fx3;	fp.fy = fy3;	arrayVertex.Add(fp);
			fp.fx = fx2;	fp.fy = fy2;	arrayVertex.Add(fp);
			fp.fx = fx4;	fp.fy = fy4;	arrayVertex.Add(fp);
			

			
			for(int i=0; i<arrayVertex.GetCount(); i++)
			{
				Fpoint fp1, fp2;
				fp1 = arrayVertex.GetAt(i);
				if(i == (arrayVertex.GetCount() - 1)) fp2 = arrayVertex.GetAt(0);
				else fp2 = arrayVertex.GetAt(i + 1);

				Struct_VertexOutline vertexoutline;
				vertexoutline.kind = 0;
				vertexoutline.x1 = fp1.fx;
				vertexoutline.y1 = fp1.fy;
				vertexoutline.x2 = fp2.fx;
				vertexoutline.y2 = fp2.fy;
				vertexoutline.cx = vertexoutline.cy = vertexoutline.r = vertexoutline.sangle = vertexoutline.eangle = 0;
				arrayVertexOutline.Add(vertexoutline);
			}
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
			pregion->GetRegionVertex(arrayVertex);
			CleanVertex(arrayVertex);
			
			CPen pen_fill(PS_SOLID, 1, RGB(0, 0, 0));
			CPen* poldpen = memdc.SelectObject(&pen_fill);
			CBrush brush_fill(RGB(0, 0, 0));
			CBrush* poldbrush = memdc.SelectObject(&brush_fill);

			
			PolygonPourDrawVertex(&memdc, arrayVertex, fminx, fminy);
			
			if((pregion->m_bPolygonCutout == FALSE)&&(pregion->m_bBoardCutout == FALSE))
			{
				
				for(int j=0; j<arrayVertex.GetCount(); j++)
				{
					Fpoint fp1, fp2;
					fp1 = arrayVertex.GetAt(j);
					if(j == (arrayVertex.GetCount() - 1)) fp2 = arrayVertex.GetAt(0);
					else fp2 = arrayVertex.GetAt(j + 1);

					COb_pcbtrack atrack;
					atrack.pDocument = pDoc;
					atrack.m_fX1 = fp1.fx;	atrack.m_fY1 = fp1.fy;
					atrack.m_fX2 = fp2.fx;	atrack.m_fY2 = fp2.fy;
					atrack.m_fWidth = 4;

					
					int x1, y1, x2, y2, width;
					x1 = (int)(atrack.m_fX1 - fminx);	y1 = (int)(atrack.m_fY1 - fminy);
					x2 = (int)(atrack.m_fX2 - fminx);	y2 = (int)(atrack.m_fY2 - fminy);
					width = (int)(atrack.m_fWidth + 2*drc);
					
					CPen pen_track(PS_SOLID, width, RGB(0, 0, 0));
					CPen* poldpen = memdc.SelectObject(&pen_track);
					memdc.MoveTo(x1, y1);
					memdc.LineTo(x2, y2);
					memdc.SelectObject(poldpen);
		            
					atrack.GetTrackVertexOutline(arrayVertexOutline, drc);
				}
			}
			else
			{
				
				for(int i=0; i<arrayVertex.GetCount(); i++)
				{
					Fpoint fp1, fp2;
					fp1 = arrayVertex.GetAt(i);
					if(i == (arrayVertex.GetCount() - 1)) fp2 = arrayVertex.GetAt(0);
					else fp2 = arrayVertex.GetAt(i + 1);

					Struct_VertexOutline vertexoutline;
					vertexoutline.kind = 0;
					vertexoutline.x1 = fp1.fx;
					vertexoutline.y1 = fp1.fy;
					vertexoutline.x2 = fp2.fx;
					vertexoutline.y2 = fp2.fy;
					vertexoutline.cx = vertexoutline.cy = vertexoutline.r = vertexoutline.sangle = vertexoutline.eangle = 0;
					arrayVertexOutline.Add(vertexoutline);
				}
			}

			memdc.SelectObject(poldpen);
			memdc.SelectObject(poldbrush);
		}
	}

	int num = bmpFace.GetBitmapBits(dx*cy, buffer);
	

	Write_Help("覆铜: 预处理数据...");
	
	for(int i=0; i<arrayVertexOutline.GetCount(); i++)
	{
		Struct_VertexOutline vertexoutline = arrayVertexOutline.GetAt(i);
		if(vertexoutline.kind == 0)
		{
			int x1, y1, x2, y2;
			x1 = (int)(vertexoutline.x1 - fminx);	y1 = (int)(vertexoutline.y1 - fminy);
			x2 = (int)(vertexoutline.x2 - fminx);	y2 = (int)(vertexoutline.y2 - fminy);

			/*if(SQRT(vertexoutline.x1, vertexoutline.y1, vertexoutline.x2, vertexoutline.y2) < drc/2)
			{
				arrayVertexOutline.RemoveAt(i);
				i--;
				continue;
			}*/
			
			if(abs(x1-x2) >= abs(y1-y2))
			{
				if(x1 > x2)
				{
					int temp;
					temp = x1;	x1 = x2;	x2 = temp;
					temp = y1;	y1 = y2;	y2 = temp;
				}
				
				int flag = 0;
				for(int x=x1; x<=x2; x++)
				{
					int mx,my;
					mx = x;
					if(y1 == y2) my = y1;
					else my = ((double)y2-y1)*(((double)x-x1)/((double)x2-x1)) + y1;

					if(IsOnSolidPoint(mx, my, buffer, dx, cy) == TRUE)
					{
						flag = 1;
						break;
					}
				}

				if(flag == 0)
				{
					arrayVertexOutline.RemoveAt(i);
					i--;
				}
			}
			else
			{
				if(y1 > y2)
				{
					int temp;
					temp = x1;	x1 = x2;	x2 = temp;
					temp = y1;	y1 = y2;	y2 = temp;
				}
				
				int flag = 0;				
				for(int y=y1; y<=y2; y++)
				{
					int mx,my;
					my = y;
					if(x1 == x2) mx = x1;
					else mx = ((double)x2-x1)*(((double)y-y1)/((double)y2-y1)) + x1;

					if(IsOnSolidPoint(mx, my, buffer, dx, cy) == TRUE)
					{
						flag = 1;
						break;
					}
				}
				
				if(flag == 0)
				{
					arrayVertexOutline.RemoveAt(i);
					i--;
				}
			}
		}
		else
		{
			int iSangle, iEangle;
			iSangle = (int)vertexoutline.sangle;
			iEangle = (int)vertexoutline.eangle;

			if(iEangle < iSangle) iEangle += 360;
			
			int flag = 0;	
			for(int angle=iSangle; angle < iEangle; angle++)
			{
				int x, y;
				x = (int)(vertexoutline.cx + (vertexoutline.r*cos(angle*fEDA_pi/180)) - fminx);
				y = (int)(vertexoutline.cy + (vertexoutline.r*sin(angle*fEDA_pi/180)) - fminy);

				if(IsOnSolidPoint(x, y, buffer, dx, cy) == TRUE)
				{
					flag = 1;
					break;
				}
			}

			if(flag == 0)
			{
				arrayVertexOutline.RemoveAt(i);
				i--;
			}
		}
	}
	
	
	for(int i=0; i<arrayVertexOutline.GetCount(); i++)
	{
		Struct_VertexOutline aaa;
		aaa = arrayVertexOutline.GetAt(i);
		
		if(aaa.kind == 0)
		{
			if((fabs(aaa.x1 - aaa.x2) < fEDA_Prec)&&(fabs(aaa.y1 - aaa.y2) < fEDA_Prec))
			{
				arrayVertexOutline.RemoveAt(i);
				i--;
				continue;
			}
		}
		else
		{
			if(fabs(aaa.sangle - aaa.eangle) < fEDA_AnglePrec)
			{
				arrayVertexOutline.RemoveAt(i);
				i--;
				continue;
			}
		}
		for(int j=i+1; j<arrayVertexOutline.GetCount(); j++)
		{
			Struct_VertexOutline bbb;
			bbb = arrayVertexOutline.GetAt(j);
			
			if(bbb.kind == 0)
			{
				if((fabs(bbb.x1 - bbb.x2) < fEDA_Prec)&&(fabs(bbb.y1 - bbb.y2) < fEDA_Prec))
				{
					arrayVertexOutline.RemoveAt(j);
					j--;
					continue;
				}
			}
			else
			{
				if(fabs(bbb.sangle - bbb.eangle) < fEDA_AnglePrec)
				{
					arrayVertexOutline.RemoveAt(j);
					j--;
					continue;
				}
			}
			if((aaa.kind == 0)&&(bbb.kind == 0))			
			{
				
				if(  (( MIN(aaa.y1, aaa.y2) - MAX(bbb.y1, bbb.y2)) > fEDA_Prec ) ||\
					 (( MIN(bbb.y1, bbb.y2) - MAX(aaa.y1, aaa.y2)) > fEDA_Prec ) ||\
					 (( MIN(aaa.x1, aaa.x2) - MAX(bbb.x1, bbb.x2)) > fEDA_Prec ) ||\
					 (( MIN(bbb.x1, bbb.x2) - MAX(aaa.x1, aaa.x2)) > fEDA_Prec ) ) continue;

				
				if( ((fabs(aaa.x1 - bbb.x1) < fEDA_Prec) && (fabs(aaa.y1 - bbb.y1) < fEDA_Prec) && (fabs(aaa.x2 - bbb.x2) < fEDA_Prec) && (fabs(aaa.y2 - bbb.y2)< fEDA_Prec)) || \
					((fabs(aaa.x1 - bbb.x2) < fEDA_Prec) && (fabs(aaa.y1 - bbb.y2) < fEDA_Prec) && (fabs(aaa.x2 - bbb.x1) < fEDA_Prec) && (fabs(aaa.y2 - bbb.y1)< fEDA_Prec)) )
				{
					arrayVertexOutline.RemoveAt(j);
					j--;
					continue;
				}

				if((fabs(aaa.x1 - aaa.x2) < fEDA_Prec) && (fabs(bbb.x1 - bbb.x2) < fEDA_Prec) && (fabs(aaa.x1 - bbb.x1) < fEDA_Prec))			
				{

					if( ((MAX(aaa.y1, aaa.y2) - MIN(bbb.y1, bbb.y2)) > 0) && ((MAX(bbb.y1, bbb.y2) - MIN(aaa.y1, aaa.y2)) > 0) )	
					{
						Struct_VertexOutline vt;
						vt.kind = 0;
						vt.cx = vt.cy = vt.r = vt.sangle = vt.eangle = 0;
						vt.x1 = vt.x2 = aaa.x1;

						
						double py[4];
						py[0] = aaa.y1;		py[1] = aaa.y2;		py[2] = bbb.y1;		py[3] = bbb.y2;
						int judge = 1;
						for(;;)
						{
							if(judge == 1) judge = 0;
							else if(judge == 0) break;
							for(int k=0; k<3; k++)
							{
								double  f1 = py[k];
								double  f2 = py[k+1];
								if(f1 > f2)
								{
									judge=1;
									py[k] = f2;
									py[k+1] = f1;
								}
							}
						}

						vt.y1 = py[0];
						vt.y2 = py[1];
						arrayVertexOutline.Add(vt);
						vt.y1 = py[1];
						vt.y2 = py[2];
						arrayVertexOutline.Add(vt);
						vt.y1 = py[2];
						vt.y2 = py[3];
						arrayVertexOutline.Add(vt);
					
						arrayVertexOutline.RemoveAt(j);
						arrayVertexOutline.RemoveAt(i);
						i--;
						break;
					}
				}
				else if((fabs(aaa.y1 - aaa.y2) < fEDA_Prec) && (fabs(bbb.y1 - bbb.y2) < fEDA_Prec) && (fabs(aaa.y1 - bbb.y1) < fEDA_Prec))		
				{
					if( ((MAX(aaa.x1, aaa.x2) - MIN(bbb.x1, bbb.x2)) > 0) && ((MAX(bbb.x1, bbb.x2) - MIN(aaa.x1, aaa.x2)) > 0)) 	
					{
						Struct_VertexOutline vt;
						vt.kind = 0;
						vt.cx = vt.cy = vt.r = vt.sangle = vt.eangle = 0;
						vt.y1 = vt.y2 = aaa.y1;

						
						double px[4];
						px[0] = aaa.x1;		px[1] = aaa.x2;		px[2] = bbb.x1;		px[3] = bbb.x2;
						int judge = 1;
						for(;;)
						{
							if(judge == 1) judge = 0;
							else if(judge == 0) break;
							for(int i=0; i<3; i++)
							{
								double  f1 = px[i];
								double  f2 = px[i+1];
								if(f1 > f2)
								{
									judge=1;
									px[i] = f2;
									px[i+1] = f1;
								}
							}
						}

						vt.x1 = px[0];
						vt.x2 = px[1];
						arrayVertexOutline.Add(vt);
						vt.x1 = px[1];
						vt.x2 = px[2];
						arrayVertexOutline.Add(vt);
						vt.x1 = px[2];
						vt.x2 = px[3];
						arrayVertexOutline.Add(vt);
								
						arrayVertexOutline.RemoveAt(j);
						arrayVertexOutline.RemoveAt(i);
						i--;
						break;
					}
				}
				else
				{
					double k1 = (aaa.y2 - aaa.y1)/(aaa.x2 - aaa.x1);
					double k2 = (bbb.y2 - bbb.y1)/(bbb.x2 - bbb.x1);
					if(fabs(k1 - k2) < fEDA_Prec)
					{
						if((fabs((aaa.y2 - aaa.y1)*(aaa.x1 - bbb.x1) - (aaa.y1 - bbb.y1)*(aaa.x2 - aaa.x1)) < fEDA_Prec ) && ((MAX(aaa.y1, aaa.y2) - MIN(bbb.y1, bbb.y2)) > 0) && ((MAX(bbb.y1, bbb.y2) - MIN(aaa.y1, aaa.y2)) > 0) )
						{
							Struct_VertexOutline vt;
							vt.kind = 0;
							vt.cx = vt.cy = vt.r = vt.sangle = vt.eangle = 0;

							
							double px[4], py[4];
							px[0] = aaa.x1;		px[1] = aaa.x2;		px[2] = bbb.x1;		px[3] = bbb.x2;
							py[0] = aaa.y1;		py[1] = aaa.y2;		py[2] = bbb.y1;		py[3] = bbb.y2;
							
							int judge = 1;
							for(;;)
							{
								if(judge == 1) judge = 0;
								else if(judge == 0) break;
								for(int i=0; i<3; i++)
								{
									double  f1 = px[i];
									double  f2 = px[i+1];
									if(f1 > f2)
									{
										judge=1;
										px[i] = f2;
										px[i+1] = f1;
										double ftemp;
										ftemp = py[i];
										py[i] = py[i+1];
										py[i+1] = ftemp;
									}
								}
							}
							
							vt.x1 = px[0];	vt.y1 = py[0];
							vt.x2 = px[1];	vt.y2 = py[1];
							arrayVertexOutline.Add(vt);
							vt.x1 = px[1];	vt.y1 = py[1];
							vt.x2 = px[2];	vt.y2 = py[2];
							arrayVertexOutline.Add(vt);
							vt.x1 = px[2];	vt.y1 = py[2];
							vt.x2 = px[3];	vt.y2 = py[3];
							arrayVertexOutline.Add(vt);

							arrayVertexOutline.RemoveAt(j);
							arrayVertexOutline.RemoveAt(i);
							i--;
							break;
						}
						
					}
				}
			}
			else if((aaa.kind == 1)&&(bbb.kind == 1))		
			{
				
				if((fabs(aaa.cx - bbb.cx) < fEDA_Prec)&&(fabs(aaa.cy - bbb.cy) < fEDA_Prec)&&(fabs(aaa.r - bbb.r) < fEDA_Prec))
				{
					
					if((fabs(aaa.sangle - bbb.sangle) < fEDA_AnglePrec)&&(fabs(aaa.eangle - bbb.eangle) < fEDA_AnglePrec))
					{
						arrayVertexOutline.RemoveAt(j);
						j--;
						continue;
					}

					

					if((aaa.eangle > aaa.sangle)&&(bbb.eangle > bbb.sangle))		
					{
						if((aaa.sangle >= bbb.eangle) || (bbb.sangle >= aaa.eangle)) continue;
					}
					else if( (aaa.eangle > aaa.sangle) || (bbb.eangle > bbb.sangle) )	
					{
						if( (aaa.sangle >= bbb.eangle) && (aaa.eangle <= bbb.sangle) ) continue;
					}

					Struct_VertexOutline vt;
					vt.kind = 1;
					vt.cx = aaa.cx;		vt.cy = aaa.cy;		vt.r = aaa.r;

					
					double angle[4];
					angle[0] = aaa.sangle;	angle[1] = aaa.eangle;	angle[2] = bbb.sangle;	angle[3] = bbb.eangle;
					int judge = 1;
					for(;;)
					{
						if(judge == 1) judge = 0;
						else if(judge == 0) break;
						for(int i=0; i<3; i++)
						{
							double  f1 = angle[i];
							double  f2 = angle[i+1];
							if(f1 > f2)
							{
								judge=1;
								angle[i] = f2;
								angle[i+1] = f1;
							}
						}
					}

					double sa, ea, middle;
					
					sa = angle[0];	ea = angle[1];	
					if(ea > sa) middle = (sa + ea)/2;
					else 
					{
						middle = (sa + ea)/2 + 180;
						if(middle >= 360) middle -= 360;
					}
					if( ((aaa.eangle > aaa.sangle)&&((middle > aaa.sangle)&&(middle < aaa.eangle))) ||\
						((aaa.eangle < aaa.sangle)&&((middle > aaa.sangle)||(middle < aaa.eangle))) ||\
						((bbb.eangle > bbb.sangle)&&((middle > bbb.sangle)&&(middle < bbb.eangle))) ||\
						((bbb.eangle < bbb.sangle)&&((middle > bbb.sangle)||(middle < bbb.eangle)))  )
					{
						vt.sangle = pDoc->Get_Correct_Angle(sa);
						vt.eangle = pDoc->Get_Correct_Angle(ea);
						vt.x1 = vt.cx + vt.r*cos(vt.sangle*fEDA_pi/180);
						vt.y1 = vt.cy + vt.r*sin(vt.sangle*fEDA_pi/180);
						vt.x2 = vt.cx + vt.r*cos(vt.eangle*fEDA_pi/180);
						vt.y2 = vt.cy + vt.r*sin(vt.eangle*fEDA_pi/180);
						arrayVertexOutline.Add(vt);
					}
					
					sa = angle[1];	ea = angle[2];
					if(ea > sa) middle = (sa + ea)/2;
					else 
					{
						middle = (sa + ea)/2 + 180;
						if(middle >= 360) middle -= 360;
					}
					if( ((aaa.eangle > aaa.sangle)&&((middle > aaa.sangle)&&(middle < aaa.eangle))) ||\
						((aaa.eangle < aaa.sangle)&&((middle > aaa.sangle)||(middle < aaa.eangle))) ||\
						((bbb.eangle > bbb.sangle)&&((middle > bbb.sangle)&&(middle < bbb.eangle))) ||\
						((bbb.eangle < bbb.sangle)&&((middle > bbb.sangle)||(middle < bbb.eangle)))  )
					{
						vt.sangle = pDoc->Get_Correct_Angle(sa);
						vt.eangle = pDoc->Get_Correct_Angle(ea);
						vt.x1 = vt.cx + vt.r*cos(vt.sangle*fEDA_pi/180);
						vt.y1 = vt.cy + vt.r*sin(vt.sangle*fEDA_pi/180);
						vt.x2 = vt.cx + vt.r*cos(vt.eangle*fEDA_pi/180);
						vt.y2 = vt.cy + vt.r*sin(vt.eangle*fEDA_pi/180);
						arrayVertexOutline.Add(vt);
					}
					
					sa = angle[2];	ea = angle[3];
					if(ea > sa) middle = (sa + ea)/2;
					else 
					{
						middle = (sa + ea)/2 + 180;
						if(middle >= 360) middle -= 360;
					}
					if( ((aaa.eangle > aaa.sangle)&&((middle > aaa.sangle)&&(middle < aaa.eangle))) ||\
						((aaa.eangle < aaa.sangle)&&((middle > aaa.sangle)||(middle < aaa.eangle))) ||\
						((bbb.eangle > bbb.sangle)&&((middle > bbb.sangle)&&(middle < bbb.eangle))) ||\
						((bbb.eangle < bbb.sangle)&&((middle > bbb.sangle)||(middle < bbb.eangle)))  )
					{
						vt.sangle = pDoc->Get_Correct_Angle(sa);
						vt.eangle = pDoc->Get_Correct_Angle(ea);
						vt.x1 = vt.cx + vt.r*cos(vt.sangle*fEDA_pi/180);
						vt.y1 = vt.cy + vt.r*sin(vt.sangle*fEDA_pi/180);
						vt.x2 = vt.cx + vt.r*cos(vt.eangle*fEDA_pi/180);
						vt.y2 = vt.cy + vt.r*sin(vt.eangle*fEDA_pi/180);
						arrayVertexOutline.Add(vt);
					}
					
					sa = angle[3];	ea = angle[0];
					if(ea > sa) middle = (sa + ea)/2;
					else 
					{
						middle = (sa + ea)/2 + 180;
						if(middle >= 360) middle -= 360;
					}
					if( ((aaa.eangle > aaa.sangle)&&((middle > aaa.sangle)&&(middle < aaa.eangle))) ||\
						((aaa.eangle < aaa.sangle)&&((middle > aaa.sangle)||(middle < aaa.eangle))) ||\
						((bbb.eangle > bbb.sangle)&&((middle > bbb.sangle)&&(middle < bbb.eangle))) ||\
						((bbb.eangle < bbb.sangle)&&((middle > bbb.sangle)||(middle < bbb.eangle)))  )
					{
						vt.sangle = pDoc->Get_Correct_Angle(sa);
						vt.eangle = pDoc->Get_Correct_Angle(ea);
						vt.x1 = vt.cx + vt.r*cos(vt.sangle*fEDA_pi/180);
						vt.y1 = vt.cy + vt.r*sin(vt.sangle*fEDA_pi/180);
						vt.x2 = vt.cx + vt.r*cos(vt.eangle*fEDA_pi/180);
						vt.y2 = vt.cy + vt.r*sin(vt.eangle*fEDA_pi/180);
						arrayVertexOutline.Add(vt);
					}

					arrayVertexOutline.RemoveAt(j);
					arrayVertexOutline.RemoveAt(i);
					i--;
					break;
				}
			}
		}
	}

	
	char strtemp[100] = "覆铜: 区域分割...";
	Write_Help(strtemp);
	int processnum = arrayVertexOutline.GetCount();
	for(int i=0; i<arrayVertexOutline.GetCount(); i++)
	{
		
		if((i%100) == 0)
		{
			char str[100];
			sprintf(str,"%s %d/%d", strtemp, i, arrayVertexOutline.GetCount());
			Write_Help(str);
		}

		Struct_VertexOutline aaa;
		aaa = arrayVertexOutline.GetAt(i);
		
		if(aaa.kind == 0)
		{
			if((fabs(aaa.x1 - aaa.x2) < fEDA_Prec)&&(fabs(aaa.y1 - aaa.y2) < fEDA_Prec))
			{
				arrayVertexOutline.RemoveAt(i);
				i--;
				continue;
			}
		}
		else
		{
			double sangle, eangle;
			sangle = aaa.sangle;
			eangle = aaa.eangle;
			for(;;)
			{
				if(eangle < sangle) eangle += 360;
				else break;
			}
			if(fabs(eangle - sangle) < 3)
			{
				arrayVertexOutline.RemoveAt(i);
				i--;
				continue;
			}
		}
		
		for(int j=i+1; j<arrayVertexOutline.GetCount(); j++)
		{
			Struct_VertexOutline bbb;
			bbb = arrayVertexOutline.GetAt(j);

			
			if(bbb.kind == 0)
			{
				if((fabs(bbb.x1 - bbb.x2) < fEDA_Prec)&&(fabs(bbb.y1 - bbb.y2) < fEDA_Prec))
				{
					arrayVertexOutline.RemoveAt(j);
					j--;
					continue;
				}
			}
			else
			{
				double sangle, eangle;
				sangle = bbb.sangle;
				eangle = bbb.eangle;
				for(;;)
				{
					if(eangle < sangle) eangle += 360;
					else break;
				}
				if(fabs(eangle - sangle) < 3)
				{
					arrayVertexOutline.RemoveAt(j);
					j--;
					continue;
				}
			}

			if((aaa.kind == 0)&&(bbb.kind == 0))			
			{
				double k1, k2;
				double ftempx,ftempy;

				
				if(  ((MIN(aaa.y1, aaa.y2) - MAX(bbb.y1, bbb.y2)) > fEDA_Prec) || ((MIN(bbb.y1, bbb.y2) - MAX(aaa.y1, aaa.y2)) > fEDA_Prec) ||\
					 ((MIN(aaa.x1, aaa.x2) - MAX(bbb.x1, bbb.x2)) > fEDA_Prec) || ((MIN(bbb.x1, bbb.x2) - MAX(aaa.x1, aaa.x2)) > fEDA_Prec) ) continue;

				
				if( ((fabs(aaa.x1 - bbb.x1) < fEDA_Prec)&&(fabs(aaa.y1 - bbb.y1) < fEDA_Prec)) || ((fabs(aaa.x2 - bbb.x1) < fEDA_Prec)&&(fabs(aaa.y2 - bbb.y1) < fEDA_Prec)) ||\
					((fabs(aaa.x1 - bbb.x2) < fEDA_Prec)&&(fabs(aaa.y1 - bbb.y2) < fEDA_Prec)) || ((fabs(aaa.x2 - bbb.x2) < fEDA_Prec)&&(fabs(aaa.y2 - bbb.y2) < fEDA_Prec)) ) continue;

				if((aaa.x1 == aaa.x2)&&(bbb.x1 == bbb.x2))	continue;		
				else if((aaa.y1 == aaa.y2)&&(bbb.y1 == bbb.y2))	continue;	
				else if(aaa.x1 == aaa.x2)			
				{
					k2 = (bbb.y2 - bbb.y1)/(bbb.x2 - bbb.x1);
					ftempx = aaa.x2;
					ftempy = k2*(aaa.x2 - bbb.x2) + bbb.y2;

					if( ((ftempx >= MIN(bbb.x1, bbb.x2))&&(ftempx <= MAX(bbb.x1, bbb.x2))) && \
						((ftempx >= MIN(aaa.x1, aaa.x2))&&(ftempx <= MAX(aaa.x1, aaa.x2))) && \
						((ftempy >= MIN(bbb.y1, bbb.y2))&&(ftempy <= MAX(bbb.y1, bbb.y2))) && \
						((ftempy >= MIN(aaa.y1, aaa.y2))&&(ftempy <= MAX(aaa.y1, aaa.y2))) )
					{					
						
						Struct_VertexOutline vt;
						vt.kind = 0;
						vt.cx = vt.cy = vt.r = vt.sangle = vt.eangle = 0;
						
						int flag = 0;
						
						if( ((ftempx != bbb.x1)||(ftempy != bbb.y1)) && ((ftempx != bbb.x2)||(ftempy != bbb.y2)) )
						{
							vt.x1 = bbb.x1;	vt.y1 = bbb.y1;
							vt.x2 = ftempx;	vt.y2 = ftempy;
							arrayVertexOutline.Add(vt);
							vt.x1 = ftempx;	vt.y1 = ftempy;
							vt.x2 = bbb.x2;	vt.y2 = bbb.y2;
							arrayVertexOutline.Add(vt);

							arrayVertexOutline.RemoveAt(j);
							j--;
							flag = 1;
						}

						
						if( ((ftempx != aaa.x1)||(ftempy != aaa.y1)) && ((ftempx != aaa.x2)||(ftempy != aaa.y2)) )
						{
							vt.x1 = aaa.x1;	vt.y1 = aaa.y1;
							vt.x2 = ftempx;	vt.y2 = ftempy;
							arrayVertexOutline.Add(vt);
							vt.x1 = ftempx;	vt.y1 = ftempy;
							vt.x2 = aaa.x2;	vt.y2 = aaa.y2;
							arrayVertexOutline.Add(vt);
							
							arrayVertexOutline.RemoveAt(i);
							i--;
							flag = 2;
						}

						if(flag == 1) continue;
						else if(flag == 2) break;
					}
				}
				else if(bbb.x1 == bbb.x2)			
				{
					k1 = (aaa.y2 - aaa.y1)/(aaa.x2 - aaa.x1);
					ftempx = bbb.x2;
					ftempy = k1*(bbb.x2 - aaa.x2) + aaa.y2;
					
					if( ((ftempx >= MIN(bbb.x1, bbb.x2))&&(ftempx <= MAX(bbb.x1, bbb.x2))) && \
						((ftempx >= MIN(aaa.x1, aaa.x2))&&(ftempx <= MAX(aaa.x1, aaa.x2))) && \
						((ftempy >= MIN(bbb.y1, bbb.y2))&&(ftempy <= MAX(bbb.y1, bbb.y2))) && \
						((ftempy >= MIN(aaa.y1, aaa.y2))&&(ftempy <= MAX(aaa.y1, aaa.y2))) )
					{	
						
						Struct_VertexOutline vt;
						vt.kind = 0;
						vt.cx = vt.cy = vt.r = vt.sangle = vt.eangle = 0;
												
						int flag = 0;
						
						if( ((ftempx != bbb.x1)||(ftempy != bbb.y1)) && ((ftempx != bbb.x2)||(ftempy != bbb.y2)) )
						{
							vt.x1 = bbb.x1;	vt.y1 = bbb.y1;
							vt.x2 = ftempx;	vt.y2 = ftempy;
							arrayVertexOutline.Add(vt);
							vt.x1 = ftempx;	vt.y1 = ftempy;
							vt.x2 = bbb.x2;	vt.y2 = bbb.y2;
							arrayVertexOutline.Add(vt);
							
							arrayVertexOutline.RemoveAt(j);
							j--;
							flag = 1;
						}

						
						if( ((ftempx != aaa.x1)||(ftempy != aaa.y1)) && ((ftempx != aaa.x2)||(ftempy != aaa.y2)) )
						{
							vt.x1 = aaa.x1;	vt.y1 = aaa.y1;
							vt.x2 = ftempx;	vt.y2 = ftempy;
							arrayVertexOutline.Add(vt);
							vt.x1 = ftempx;	vt.y1 = ftempy;
							vt.x2 = aaa.x2;	vt.y2 = aaa.y2;
							arrayVertexOutline.Add(vt);

							arrayVertexOutline.RemoveAt(i);
							i--;
							flag = 2;
						}

						if(flag == 1) continue;
						else if(flag == 2) break;
					}
				}
				else if(aaa.y1 == aaa.y2)			
				{
					k2 = (bbb.y2 - bbb.y1)/(bbb.x2 - bbb.x1);
					k1 = 0;
					ftempx = (k1*aaa.x2 - k2*bbb.x2 - aaa.y2 + bbb.y2)/(k1-k2);
					ftempy = aaa.y2;
					
					if( ((ftempx >= MIN(bbb.x1, bbb.x2))&&(ftempx <= MAX(bbb.x1, bbb.x2))) && \
						((ftempx >= MIN(aaa.x1, aaa.x2))&&(ftempx <= MAX(aaa.x1, aaa.x2))) && \
						((ftempy >= MIN(bbb.y1, bbb.y2))&&(ftempy <= MAX(bbb.y1, bbb.y2))) && \
						((ftempy >= MIN(aaa.y1, aaa.y2))&&(ftempy <= MAX(aaa.y1, aaa.y2))) )
					{	
						
						Struct_VertexOutline vt;
						vt.kind = 0;
						vt.cx = vt.cy = vt.r = vt.sangle = vt.eangle = 0;
						
						int flag = 0;						
						
						if( ((ftempx != bbb.x1)||(ftempy != bbb.y1)) && ((ftempx != bbb.x2)||(ftempy != bbb.y2)) )
						{
							vt.x1 = bbb.x1;	vt.y1 = bbb.y1;
							vt.x2 = ftempx;	vt.y2 = ftempy;
							arrayVertexOutline.Add(vt);
							vt.x1 = ftempx;	vt.y1 = ftempy;
							vt.x2 = bbb.x2;	vt.y2 = bbb.y2;
							arrayVertexOutline.Add(vt);
							
							arrayVertexOutline.RemoveAt(j);
							j--;
							flag = 1;
						}						
						
						if( ((ftempx != aaa.x1)||(ftempy != aaa.y1)) && ((ftempx != aaa.x2)||(ftempy != aaa.y2)) )
						{
							vt.x1 = aaa.x1;	vt.y1 = aaa.y1;
							vt.x2 = ftempx;	vt.y2 = ftempy;
							arrayVertexOutline.Add(vt);
							vt.x1 = ftempx;	vt.y1 = ftempy;
							vt.x2 = aaa.x2;	vt.y2 = aaa.y2;
							arrayVertexOutline.Add(vt);

							arrayVertexOutline.RemoveAt(i);
							i--;
							flag = 2;
						}
						
						if(flag == 1) continue;
						else if(flag == 2) break;
					}
				}
				else if(bbb.y1 == bbb.y2)			
				{
					k1 = (aaa.y2 - aaa.y1)/(aaa.x2 - aaa.x1);
					k2 = 0;
					ftempx = (k1*aaa.x2 - k2*bbb.x2 - aaa.y2 + bbb.y2)/(k1-k2);
					ftempy = bbb.y2; 
					
					if( ((ftempx >= MIN(bbb.x1, bbb.x2))&&(ftempx <= MAX(bbb.x1, bbb.x2))) && \
						((ftempx >= MIN(aaa.x1, aaa.x2))&&(ftempx <= MAX(aaa.x1, aaa.x2))) && \
						((ftempy >= MIN(bbb.y1, bbb.y2))&&(ftempy <= MAX(bbb.y1, bbb.y2))) && \
						((ftempy >= MIN(aaa.y1, aaa.y2))&&(ftempy <= MAX(aaa.y1, aaa.y2))) )
					{	
						
						Struct_VertexOutline vt;
						vt.kind = 0;
						vt.cx = vt.cy = vt.r = vt.sangle = vt.eangle = 0;
						
						int flag = 0;							
						
						if( ((ftempx != bbb.x1)||(ftempy != bbb.y1)) && ((ftempx != bbb.x2)||(ftempy != bbb.y2)) )
						{
							vt.x1 = bbb.x1;	vt.y1 = bbb.y1;
							vt.x2 = ftempx;	vt.y2 = ftempy;
							arrayVertexOutline.Add(vt);
							vt.x1 = ftempx;	vt.y1 = ftempy;
							vt.x2 = bbb.x2;	vt.y2 = bbb.y2;
							arrayVertexOutline.Add(vt);
							
							arrayVertexOutline.RemoveAt(j);
							j--;
							flag = 1;
						}						
						
						if( ((ftempx != aaa.x1)||(ftempy != aaa.y1)) && ((ftempx != aaa.x2)||(ftempy != aaa.y2)) )
						{
							vt.x1 = aaa.x1;	vt.y1 = aaa.y1;
							vt.x2 = ftempx;	vt.y2 = ftempy;
							arrayVertexOutline.Add(vt);
							vt.x1 = ftempx;	vt.y1 = ftempy;
							vt.x2 = aaa.x2;	vt.y2 = aaa.y2;
							arrayVertexOutline.Add(vt);

							arrayVertexOutline.RemoveAt(i);
							i--;
							flag = 2;
						}
						
						if(flag == 1) continue;
						else if(flag == 2) break;
					}
				}
				else
				{
					k1 = (aaa.y2 - aaa.y1)/(aaa.x2 - aaa.x1);
					k2 = (bbb.y2 - bbb.y1)/(bbb.x2 - bbb.x1);
					if(k1 != k2)
					{
						ftempx = (k1*aaa.x2 - k2*bbb.x2 - aaa.y2 + bbb.y2)/(k1-k2);
						ftempy = (k1*k2*aaa.x2 - k1*k2*bbb.x2 + k1*bbb.y2 - k2*aaa.y2)/(k1-k2);

						if( ((ftempx >= MIN(bbb.x1, bbb.x2))&&(ftempx <= MAX(bbb.x1, bbb.x2))) && \
							((ftempx >= MIN(aaa.x1, aaa.x2))&&(ftempx <= MAX(aaa.x1, aaa.x2))) && \
							((ftempy >= MIN(bbb.y1, bbb.y2))&&(ftempy <= MAX(bbb.y1, bbb.y2))) && \
							((ftempy >= MIN(aaa.y1, aaa.y2))&&(ftempy <= MAX(aaa.y1, aaa.y2))) )
						{	
							
							Struct_VertexOutline vt;
							vt.kind = 0;
							vt.cx = vt.cy = vt.r = vt.sangle = vt.eangle = 0;
													
							int flag = 0;	
							
							if( ((ftempx != bbb.x1)||(ftempy != bbb.y1)) && ((ftempx != bbb.x2)||(ftempy != bbb.y2)) )
							{
								vt.x1 = bbb.x1;	vt.y1 = bbb.y1;
								vt.x2 = ftempx;	vt.y2 = ftempy;
								arrayVertexOutline.Add(vt);
								vt.x1 = ftempx;	vt.y1 = ftempy;
								vt.x2 = bbb.x2;	vt.y2 = bbb.y2;
								arrayVertexOutline.Add(vt);
								
								arrayVertexOutline.RemoveAt(j);
								j--;
								flag = 1;
							}							
							
							if( ((ftempx != aaa.x1)||(ftempy != aaa.y1)) && ((ftempx != aaa.x2)||(ftempy != aaa.y2)) )
							{
								vt.x1 = aaa.x1;	vt.y1 = aaa.y1;
								vt.x2 = ftempx;	vt.y2 = ftempy;
								arrayVertexOutline.Add(vt);
								vt.x1 = ftempx;	vt.y1 = ftempy;
								vt.x2 = aaa.x2;	vt.y2 = aaa.y2;
								arrayVertexOutline.Add(vt);

								arrayVertexOutline.RemoveAt(i);
								i--;
								flag = 2;
							}
							if(flag == 1) continue;
							else if(flag == 2) break;
						}
					}
				}
			}
			if((aaa.kind == 1)&&(bbb.kind == 1))		
			{
				double d = aaa.r + bbb.r;
				
				if( ((aaa.cx > bbb.cx)&&((aaa.cx - bbb.cx) > d)) ||	((aaa.cy > bbb.cy)&&((aaa.cy - bbb.cy) > d)) ||\
					((bbb.cx > aaa.cx)&&((bbb.cx - aaa.cx) > d)) ||	((bbb.cy > aaa.cy)&&((bbb.cy - aaa.cy) > d)) ) continue;
				
				if(SQRT(aaa.cx, aaa.cy, bbb.cx, bbb.cy) > d) continue;

				
				if( ((fabs(aaa.x1 - bbb.x1) < fEDA_Prec) && (fabs(aaa.y1 - bbb.y1) < fEDA_Prec) && (fabs(aaa.x2 - bbb.x2) < fEDA_Prec) && (fabs(aaa.y2 - bbb.y2) < fEDA_Prec)) ||\
					((fabs(aaa.x1 - bbb.x2) < fEDA_Prec) && (fabs(aaa.y1 - bbb.y2) < fEDA_Prec) && (fabs(aaa.x2 - bbb.x1) < fEDA_Prec) && (fabs(aaa.y2 - bbb.y1) < fEDA_Prec)) ) continue;

				
				if( ((aaa.x1 == bbb.x1)&&(aaa.y1 == bbb.y1)) ||	((aaa.x2 == bbb.x1)&&(aaa.y2 == bbb.y1))||\
					((aaa.x1 == bbb.x2)&&(aaa.y1 == bbb.y2)) ||	((aaa.x2 == bbb.x2)&&(aaa.y2 == bbb.y2)) )
				{
					if(fabs(SQRT(aaa.cx, aaa.cy, bbb.cx, bbb.cy) - aaa.r - bbb.r) < 1) continue;

					double ftempx, ftempy;
					if(aaa.cx == bbb.cx)
					{
						if(((aaa.x1 == bbb.x1)&&(aaa.y1 == bbb.y1)) || ((aaa.x1 == bbb.x2)&&(aaa.y1 == bbb.y2)))
						{
							ftempx = 2*aaa.cx - aaa.x1;
							ftempy = aaa.y1;
						}
						else
						{
							ftempx = 2*aaa.cx - aaa.x2;
							ftempy = aaa.y2;
						}
					}
					else if(aaa.cy == bbb.cy)
					{
						if(((aaa.x1 == bbb.x1)&&(aaa.y1 == bbb.y1)) || ((aaa.x1 == bbb.x2)&&(aaa.y1 == bbb.y2)))
						{
							ftempx = aaa.x1;
							ftempy = 2*aaa.cy - aaa.y1;
						}
						else
						{
							ftempx = aaa.x2;
							ftempy = 2*aaa.cy - aaa.y2;
						}
					}
					else
					{
						if(((aaa.x1 == bbb.x1)&&(aaa.y1 == bbb.y1)) || ((aaa.x1 == bbb.x2)&&(aaa.y1 == bbb.y2)))
						{
							double beta = atan2((bbb.cy - aaa.cy), (bbb.cx - aaa.cx))*180/fEDA_pi;
							if(beta < 0) beta += 360;

							double beta1 = aaa.sangle;
							if(beta1 < 0) beta1 += 360;

							
							double alpha1 = 2*beta - beta1;
							if(alpha1 < 0) alpha1 += 360;
							else if(alpha1 > 360) alpha1 -= 360;

							ftempx = aaa.cx + aaa.r*cos(alpha1*fEDA_pi/180);
							ftempy = aaa.cy + aaa.r*sin(alpha1*fEDA_pi/180);
							
						}
						else
						{
							
							double beta = atan2((bbb.cy - aaa.cy), (bbb.cx - aaa.cx))*180/fEDA_pi;
							if(beta < 0) beta += 360;
							
							double beta1 = aaa.eangle;
							if(beta1 < 0) beta1 += 360;

							
							double alpha1 = 2*beta - beta1;
							if(alpha1 < 0) alpha1 += 360;
							else if(alpha1 > 360) alpha1 -= 360;

							ftempx = aaa.cx + aaa.r*cos(alpha1*fEDA_pi/180);
							ftempy = aaa.cy + aaa.r*sin(alpha1*fEDA_pi/180);
							
						}
					}

					double alpha1, alpha2;
					if((IsInArcSweepRange(aaa, ftempx, ftempy, &alpha1) == TRUE) && (IsInArcSweepRange(bbb, ftempx, ftempy, &alpha2) == TRUE))
					{
						Struct_VertexOutline vt;
						
						alpha1 = pDoc->Get_Correct_Angle(alpha1);
						vt.kind = 1;
						vt.cx = aaa.cx; vt.cy = aaa.cy; vt.r = aaa.r;
						
						vt.sangle = aaa.sangle;	vt.x1 = aaa.x1;	vt.y1 = aaa.y1;
						vt.eangle = alpha1;
						vt.x2 = ftempx;
						vt.y2 = ftempy;
						arrayVertexOutline.Add(vt);

						vt.sangle = alpha1;
						vt.x1 = ftempx;
						vt.y1 = ftempy;
						vt.eangle = aaa.eangle;	vt.x2 = aaa.x2;	vt.y2 = aaa.y2;
						arrayVertexOutline.Add(vt);

						alpha2 = pDoc->Get_Correct_Angle(alpha2);
						vt.kind = 1;
						vt.cx = bbb.cx; vt.cy = bbb.cy; vt.r = bbb.r;

						vt.sangle = bbb.sangle;	vt.x1 = bbb.x1;	vt.y1 = bbb.y1;
						vt.eangle = alpha2;
						vt.x2 = ftempx;
						vt.y2 = ftempy;
						arrayVertexOutline.Add(vt);

						vt.sangle = alpha2;
						vt.x1 = ftempx;
						vt.y1 = ftempy;
						vt.eangle = bbb.eangle;	vt.x2 = bbb.x2;	vt.y2 = bbb.y2;
						arrayVertexOutline.Add(vt);

						arrayVertexOutline.RemoveAt(j);
						arrayVertexOutline.RemoveAt(i);
						i--;
						break;
					}
					continue;
				}

				
				if((aaa.cx == bbb.cx)&&(aaa.cy == bbb.cy)) continue;

				

				Struct_VertexOutline vta, vtb;
				vta.r = aaa.r;	vta.cx = aaa.cx;	vta.cy = aaa.cy;	vta.sangle = aaa.sangle;	vta.eangle = aaa.eangle;
				vta.kind = aaa.kind;
				vta.x1 = aaa.x1;	vta.y1 = aaa.y1;	vta.x2 = aaa.x2;	vta.y2 = aaa.y2;
				double delta_angle_a = acos((aaa.r - pDoc->m_fArcPrecision)/aaa.r)*180/fEDA_pi;
				if(delta_angle_a < 1) delta_angle_a = 1;
				else if(delta_angle_a > 5) delta_angle_a = 5;
				
				vtb.r = bbb.r;	vtb.cx = bbb.cx;	vtb.cy = bbb.cy;	vtb.sangle = bbb.sangle;	vtb.eangle = bbb.eangle;
				vtb.kind = bbb.kind;
				vtb.x1 = bbb.x1;	vtb.y1 = bbb.y1;	vtb.x2 = bbb.x2;	vtb.y2 = bbb.y2;				
				double delta_angle_b = acos((bbb.r - pDoc->m_fArcPrecision)/bbb.r)*180/fEDA_pi;
				if(delta_angle_b < 1) delta_angle_b = 1;
				else if(delta_angle_b > 5) delta_angle_b = 5;

				
				delta_angle_a = delta_angle_b = 1;
				for(;;)
				{
					if(vta.eangle < vta.sangle) vta.eangle += 360;
					else break;
				}
				for(;;)
				{
					if(vtb.eangle < vtb.sangle) vtb.eangle += 360;
					else break;
				}
						
				double fangle, fangle_bak; 
				int flag = 0;	
				double fx=0, fy=0, fx_bak, fy_bak;
				fangle = fangle_bak = vtb.sangle;
				for(;;)
				{
					fx_bak = fx;
					fy_bak = fy;
					fx = vtb.cx + vtb.r*cos(fangle*fEDA_pi/180);
					fy = vtb.cy + vtb.r*sin(fangle*fEDA_pi/180);

					if(SQRT(fx, fy, aaa.cx, aaa.cy) == aaa.r)
					{
						double alpha1, alpha2;
						if(IsInArcSweepRange(vta, fx, fy, &alpha1) == TRUE)
						{
							if(vtb.sangle >= fangle)
							{
								bbb.sangle = pDoc->Get_Correct_Angle(fangle);
								bbb.eangle = pDoc->Get_Correct_Angle(vtb.eangle);
								bbb.x1 = fx;
								bbb.y1 = fy;
								bbb.x2 = vtb.x2;
								bbb.y2 = vtb.y2;
								arrayVertexOutline.Add(bbb);
								
								arrayVertexOutline.RemoveAt(j);
							}
							else if(vtb.eangle <= fangle)
							{
								bbb.sangle = pDoc->Get_Correct_Angle(vtb.sangle);
								bbb.eangle = pDoc->Get_Correct_Angle(fangle);
								bbb.x1 = vtb.x1;
								bbb.y1 = vtb.y1;
								bbb.x2 = fx;
								bbb.y2 = fy;
								arrayVertexOutline.Add(bbb);
								
								arrayVertexOutline.RemoveAt(j);
							}
							else
							{
								bbb.sangle = pDoc->Get_Correct_Angle(vtb.sangle);
								bbb.eangle = pDoc->Get_Correct_Angle(fangle);
								bbb.x1 = vtb.x1;
								bbb.y1 = vtb.y1;
								bbb.x2 = fx;
								bbb.y2 = fy;
								arrayVertexOutline.Add(bbb);

								bbb.sangle = pDoc->Get_Correct_Angle(fangle);
								bbb.eangle = pDoc->Get_Correct_Angle(vtb.eangle);
								bbb.x1 = fx;
								bbb.y1 = fy;
								bbb.x2 = vtb.x2;
								bbb.y2 = vtb.y2;
								arrayVertexOutline.Add(bbb);
								
								arrayVertexOutline.RemoveAt(j);
							}

							if(vta.sangle >= alpha1)
							{
								aaa.sangle = pDoc->Get_Correct_Angle(alpha1);
								aaa.eangle =  pDoc->Get_Correct_Angle(vta.eangle);
								aaa.x1 = fx;
								aaa.y1 = fy;
								aaa.x2 = vta.x2;
								aaa.y2 = vta.y2;
								arrayVertexOutline.Add(aaa);
								
								arrayVertexOutline.RemoveAt(i);
							}
							else if(vta.eangle <= alpha1)
							{
								aaa.sangle = pDoc->Get_Correct_Angle(vta.sangle);
								aaa.eangle = pDoc->Get_Correct_Angle(alpha1);
								aaa.x1 = vta.x1;
								aaa.y1 = vta.y1;
								aaa.x2 = fx;
								aaa.y2 = fy;
								arrayVertexOutline.Add(aaa);
								
								arrayVertexOutline.RemoveAt(i);
							}
							else
							{
								aaa.sangle = pDoc->Get_Correct_Angle(vta.sangle);
								aaa.eangle = pDoc->Get_Correct_Angle(alpha1);
								aaa.x1 = vta.x1;
								aaa.y1 = vta.y1;
								aaa.x2 = fx;
								aaa.y2 = fy;
								arrayVertexOutline.Add(aaa);

								aaa.sangle = pDoc->Get_Correct_Angle(alpha1);
								aaa.eangle = pDoc->Get_Correct_Angle(vta.eangle);
								aaa.x1 = fx;
								aaa.y1 = fy;
								aaa.x2 = vta.x2;
								aaa.y2 = vta.y2;
								arrayVertexOutline.Add(aaa);

								arrayVertexOutline.RemoveAt(i);
							}

							i--;
							j = arrayVertexOutline.GetCount();
							break;
						}
						flag = 0;
					}
					else if( SQRT(fx, fy, aaa.cx, aaa.cy) > aaa.r )
					{
						if(flag == 2)	
						{
							
							double alpha1, alpha2;
							if((IsInArcSweepRange(vta, fx, fy, &alpha1) == TRUE)&&(IsInArcSweepRange(vta, fx_bak, fy_bak, &alpha2) == TRUE))
							{
								if(vtb.sangle >= fangle_bak)
								{
									bbb.sangle = pDoc->Get_Correct_Angle(fangle);
									bbb.eangle = pDoc->Get_Correct_Angle(vtb.eangle);
									bbb.x1 = bbb.cx + bbb.r*cos(bbb.sangle*fEDA_pi/180);
									bbb.y1 = bbb.cy + bbb.r*sin(bbb.sangle*fEDA_pi/180);
									bbb.x2 = vtb.x2;
									bbb.y2 = vtb.y2;
									arrayVertexOutline.Add(bbb);
								}
								else if(vtb.eangle <= fangle)
								{
									bbb.sangle = pDoc->Get_Correct_Angle(vtb.sangle);
									bbb.eangle = pDoc->Get_Correct_Angle(fangle_bak);
									bbb.x1 = vtb.x1;
									bbb.y1 = vtb.y1;
									bbb.x2 = bbb.cx + bbb.r*cos(bbb.eangle*fEDA_pi/180);
									bbb.y2 = bbb.cy + bbb.r*sin(bbb.eangle*fEDA_pi/180);
									arrayVertexOutline.Add(bbb);
								}
								else
								{
									bbb.sangle = pDoc->Get_Correct_Angle(vtb.sangle);
									bbb.eangle = pDoc->Get_Correct_Angle(fangle_bak);
									bbb.x1 = vtb.x1;
									bbb.y1 = vtb.y1;
									bbb.x2 = bbb.cx + bbb.r*cos(bbb.eangle*fEDA_pi/180);
									bbb.y2 = bbb.cy + bbb.r*sin(bbb.eangle*fEDA_pi/180);
									arrayVertexOutline.Add(bbb);

									bbb.sangle = pDoc->Get_Correct_Angle(fangle);
									bbb.eangle = pDoc->Get_Correct_Angle(vtb.eangle);
									bbb.x1 = bbb.cx + bbb.r*cos(bbb.sangle*fEDA_pi/180);
									bbb.y1 = bbb.cy + bbb.r*sin(bbb.sangle*fEDA_pi/180);
									bbb.x2 = vtb.x2;
									bbb.y2 = vtb.y2;
									arrayVertexOutline.Add(bbb);
								}

								if(vta.sangle >= alpha2)
								{
									aaa.sangle = pDoc->Get_Correct_Angle(alpha2 + delta_angle_a);
									aaa.eangle =  pDoc->Get_Correct_Angle(vta.eangle);
									aaa.x1 = aaa.cx + aaa.r*cos(aaa.sangle*fEDA_pi/180);
									aaa.y1 = aaa.cy + aaa.r*sin(aaa.sangle*fEDA_pi/180);
									aaa.x2 = vta.x2;
									aaa.y2 = vta.y2;
									arrayVertexOutline.Add(aaa);
								}
								else if(vta.eangle <= (alpha2 + delta_angle_a))
								{
									aaa.sangle = pDoc->Get_Correct_Angle(vta.sangle);
									aaa.eangle = pDoc->Get_Correct_Angle(alpha2);
									aaa.x1 = vta.x1;
									aaa.y1 = vta.y1;
									aaa.x2 = aaa.cx + aaa.r*cos(aaa.eangle*fEDA_pi/180);
									aaa.y2 = aaa.cy + aaa.r*sin(aaa.eangle*fEDA_pi/180);
									arrayVertexOutline.Add(aaa);
								}
								else
								{
									aaa.sangle = pDoc->Get_Correct_Angle(vta.sangle);
									aaa.eangle = pDoc->Get_Correct_Angle(alpha2);
									aaa.x1 = vta.x1;
									aaa.y1 = vta.y1;
									aaa.x2 = aaa.cx + aaa.r*cos(aaa.eangle*fEDA_pi/180);
									aaa.y2 = aaa.cy + aaa.r*sin(aaa.eangle*fEDA_pi/180);
									arrayVertexOutline.Add(aaa);
								
									aaa.sangle = pDoc->Get_Correct_Angle(alpha2 + delta_angle_a);
									aaa.eangle = pDoc->Get_Correct_Angle(vta.eangle);
									aaa.x1 = aaa.cx + aaa.r*cos(aaa.sangle*fEDA_pi/180);
									aaa.y1 = aaa.cy + aaa.r*sin(aaa.sangle*fEDA_pi/180);
									aaa.x2 = vta.x2;
									aaa.y2 = vta.y2;
									arrayVertexOutline.Add(aaa);	
								}
								
								arrayVertexOutline.RemoveAt(j);
								arrayVertexOutline.RemoveAt(i);
								i--;
								j = arrayVertexOutline.GetCount();
								break;
							}
						}
						flag = 1;
					}
					else
					{
						if(flag == 1)	
						{
							
							double alpha1, alpha2;
							if((IsInArcSweepRange(vta, fx, fy, &alpha1) == TRUE)&&(IsInArcSweepRange(vta, fx_bak, fy_bak, &alpha2) == TRUE))
							{
								if(vtb.sangle >= fangle_bak)
								{
									bbb.sangle = pDoc->Get_Correct_Angle(fangle);
									bbb.eangle = pDoc->Get_Correct_Angle(vtb.eangle);
									bbb.x1 = bbb.cx + bbb.r*cos(bbb.sangle*fEDA_pi/180);
									bbb.y1 = bbb.cy + bbb.r*sin(bbb.sangle*fEDA_pi/180);
									bbb.x2 = vtb.x2;
									bbb.y2 = vtb.y2;
									arrayVertexOutline.Add(bbb);
								}
								else if(vtb.eangle <= fangle)
								{
									bbb.sangle = pDoc->Get_Correct_Angle(vtb.sangle);
									bbb.eangle = pDoc->Get_Correct_Angle(fangle_bak);
									bbb.x1 = vtb.x1;
									bbb.y1 = vtb.y1;
									bbb.x2 = bbb.cx + bbb.r*cos(bbb.eangle*fEDA_pi/180);
									bbb.y2 = bbb.cy + bbb.r*sin(bbb.eangle*fEDA_pi/180);
									arrayVertexOutline.Add(bbb);
								}
								else
								{
									bbb.sangle = pDoc->Get_Correct_Angle(vtb.sangle);
									bbb.eangle = pDoc->Get_Correct_Angle(fangle_bak);
									bbb.x1 = vtb.x1;
									bbb.y1 = vtb.y1;
									bbb.x2 = bbb.cx + bbb.r*cos(bbb.eangle*fEDA_pi/180);
									bbb.y2 = bbb.cy + bbb.r*sin(bbb.eangle*fEDA_pi/180);
									arrayVertexOutline.Add(bbb);

									bbb.sangle = pDoc->Get_Correct_Angle(fangle);
									bbb.eangle = pDoc->Get_Correct_Angle(vtb.eangle);
									bbb.x1 = bbb.cx + bbb.r*cos(bbb.sangle*fEDA_pi/180);
									bbb.y1 = bbb.cy + bbb.r*sin(bbb.sangle*fEDA_pi/180);
									bbb.x2 = vtb.x2;
									bbb.y2 = vtb.y2;
									arrayVertexOutline.Add(bbb);
								}
	
								if(vta.sangle >= alpha1)
								{
									aaa.sangle = pDoc->Get_Correct_Angle(alpha1 + delta_angle_a);
									aaa.eangle =  pDoc->Get_Correct_Angle(vta.eangle);
									aaa.x1 = aaa.cx + aaa.r*cos(aaa.sangle*fEDA_pi/180);
									aaa.y1 = aaa.cy + aaa.r*sin(aaa.sangle*fEDA_pi/180);
									aaa.x2 = vta.x2;
									aaa.y2 = vta.y2;
									arrayVertexOutline.Add(aaa);
								}
								else if(vta.eangle <= (alpha1 + delta_angle_a))
								{
									aaa.sangle = pDoc->Get_Correct_Angle(vta.sangle);
									aaa.eangle = pDoc->Get_Correct_Angle(alpha1);
									aaa.x1 = vta.x1;
									aaa.y1 = vta.y1;
									aaa.x2 = aaa.cx + aaa.r*cos(aaa.eangle*fEDA_pi/180);
									aaa.y2 = aaa.cy + aaa.r*sin(aaa.eangle*fEDA_pi/180);
									arrayVertexOutline.Add(aaa);
								}
								else
								{
									aaa.sangle = pDoc->Get_Correct_Angle(vta.sangle);
									aaa.eangle = pDoc->Get_Correct_Angle(alpha1);
									aaa.x1 = vta.x1;
									aaa.y1 = vta.y1;
									aaa.x2 = aaa.cx + aaa.r*cos(aaa.eangle*fEDA_pi/180);
									aaa.y2 = aaa.cy + aaa.r*sin(aaa.eangle*fEDA_pi/180);
									arrayVertexOutline.Add(aaa);
									
									aaa.sangle = pDoc->Get_Correct_Angle(alpha1 + delta_angle_a);
									aaa.eangle = pDoc->Get_Correct_Angle(vta.eangle);
									aaa.x1 = aaa.cx + aaa.r*cos(aaa.sangle*fEDA_pi/180);
									aaa.y1 = aaa.cy + aaa.r*sin(aaa.sangle*fEDA_pi/180);
									aaa.x2 = vta.x2;
									aaa.y2 = vta.y2;
									arrayVertexOutline.Add(aaa);
								}
								
								arrayVertexOutline.RemoveAt(j);
								arrayVertexOutline.RemoveAt(i);
								i--;
								j = arrayVertexOutline.GetCount();
								break;
							}
						}
						flag = 2;
					}
					
					if(fangle == vtb.eangle) break;

					fangle_bak = fangle;
					fangle += delta_angle_b;
					
					if(fangle > vtb.eangle) fangle = vtb.eangle;
				}
			}
			else	
			{
				Struct_VertexOutline vtline, vtarc;
				if(aaa.kind == 0)
				{
					vtline.kind = aaa.kind;		vtline.cx = vtline.cy = vtline.r = vtline.sangle = vtline.eangle = 0;
					vtline.x1 = aaa.x1;		vtline.y1 = aaa.y1;		vtline.x2 = aaa.x2;		vtline.y2 = aaa.y2;

					vtarc.kind = bbb.kind;
					vtarc.cx = bbb.cx;		vtarc.cy = bbb.cy;		vtarc.r = bbb.r;		vtarc.sangle = bbb.sangle;		vtarc.eangle = bbb.eangle;
					vtarc.x1 = bbb.x1;		vtarc.y1 = bbb.y1;		vtarc.x2 = bbb.x2;		vtarc.y2 = bbb.y2;
					for(;;)
					{
						if(vtarc.eangle < vtarc.sangle) vtarc.eangle += 360;
						else break;
					}
				}
				else
				{
					vtline.kind = bbb.kind;		vtline.cx = vtline.cy = vtline.r = vtline.sangle = vtline.eangle = 0;
					vtline.x1 = bbb.x1;		vtline.y1 = bbb.y1;		vtline.x2 = bbb.x2;		vtline.y2 = bbb.y2;

					vtarc.kind = aaa.kind;
					vtarc.cx = aaa.cx;		vtarc.cy = aaa.cy;		vtarc.r = aaa.r;		vtarc.sangle = aaa.sangle;		vtarc.eangle = aaa.eangle;
					vtarc.x1 = aaa.x1;		vtarc.y1 = aaa.y1;		vtarc.x2 = aaa.x2;		vtarc.y2 = aaa.y2;
					for(;;)
					{
						if(vtarc.eangle < vtarc.sangle) vtarc.eangle += 360;
						else break;
					}
				}

				
				double f_minx, f_miny, f_maxx, f_maxy;
				f_minx = MIN(vtline.x1, vtline.x2);				f_miny = MIN(vtline.y1, vtline.y2);
				f_maxx = MAX(vtline.x1, vtline.x2);				f_maxy = MAX(vtline.y1, vtline.y2);
					
				double d = vtarc.r;
				if( ((f_minx > vtarc.cx)&&((f_minx - vtarc.cx) > d)) || ((f_maxx < vtarc.cx)&&((vtarc.cx - f_maxx) > d)) || \
					((f_miny > vtarc.cy)&&((f_miny - vtarc.cy) > d)) || ((f_maxy < vtarc.cy)&&((vtarc.cy - f_maxy) > d)) )	continue;
				
				
				if( ((aaa.x1 == bbb.x1)&&(aaa.y1 == bbb.y1)) || ((aaa.x2 == bbb.x1)&&(aaa.y2 == bbb.y1)) ||\
					((aaa.x1 == bbb.x2)&&(aaa.y1 == bbb.y2)) || ((aaa.x2 == bbb.x2)&&(aaa.y2 == bbb.y2)) ) continue;

						
				double ftempx1, ftempy1, ftempx2, ftempy2, alpha1, alpha2;
				BOOL bAlpha1, bAlpha2, bFtemp1, bFtemp2;
				if(vtline.x1 == vtline.x2)			
				{
					if(fabs(fabs(vtline.x1 - vtarc.cx) - vtarc.r) < 0.01)	
					{
						ftempx1 = vtline.x1;
						ftempy1 = vtarc.cy;
						bAlpha1 = IsInArcSweepRange(vtarc, ftempx1, ftempy1, &alpha1);
						bAlpha2 = FALSE;					
						bFtemp1 = (ftempy1 > MIN(vtline.y1, vtline.y2)) && (ftempy1 < MAX(vtline.y1, vtline.y2));
						bFtemp2 = FALSE;
						if(vtline.y1 > vtline.y2)
						{
							double ftemp = vtline.y1;
							vtline.y1 = vtline.y2;
							vtline.y2 = ftemp;
						}
					}
					else
					{
						ftempx1 = ftempx2 = vtline.x1;
						ftempy1 = vtarc.cy - sqrt(vtarc.r*vtarc.r - (vtline.x1 - vtarc.cx)*(vtline.x1 - vtarc.cx));
						ftempy2 = vtarc.cy + sqrt(vtarc.r*vtarc.r - (vtline.x1 - vtarc.cx)*(vtline.x1 - vtarc.cx));
						bAlpha1 = IsInArcSweepRange(vtarc, ftempx1, ftempy1, &alpha1);
						bAlpha2 = IsInArcSweepRange(vtarc, ftempx2, ftempy2, &alpha2);
						bFtemp1 = (ftempy1 > MIN(vtline.y1, vtline.y2)) && (ftempy1 < MAX(vtline.y1, vtline.y2));
						bFtemp2 = (ftempy2 > MIN(vtline.y1, vtline.y2)) && (ftempy2 < MAX(vtline.y1, vtline.y2));
						if(vtline.y1 > vtline.y2)
						{
							double ftemp = vtline.y1;
							vtline.y1 = vtline.y2;
							vtline.y2 = ftemp;
						}
					}
				}
				else if(vtline.y1 == vtline.y2)			
				{
					if(fabs(fabs(vtline.y1 - vtarc.cy) - vtarc.r) < 0.01)	
					{
						ftempx1 = vtarc.cx;
						ftempy1 = vtline.y1;
						bAlpha1 = IsInArcSweepRange(vtarc, ftempx1, ftempy1, &alpha1);
						bAlpha2 = FALSE;					
						bFtemp1 = (ftempx1 > MIN(vtline.x1, vtline.x2)) && (ftempx1 < MAX(vtline.x1, vtline.x2));
						bFtemp2 = FALSE;
						if(vtline.x1 > vtline.x2)
						{
							double ftemp = vtline.x1;
							vtline.x1 = vtline.x2;
							vtline.x2 = ftemp;
						}
					}
					else
					{
						ftempx1 = vtarc.cx - sqrt(vtarc.r*vtarc.r - (vtline.y1 - vtarc.cy)*(vtline.y1 - vtarc.cy));
						ftempx2 = vtarc.cx + sqrt(vtarc.r*vtarc.r - (vtline.y1 - vtarc.cy)*(vtline.y1 - vtarc.cy));
						ftempy1 = ftempy2 = vtline.y1;
						bAlpha1 = IsInArcSweepRange(vtarc, ftempx1, ftempy1, &alpha1);
						bAlpha2 = IsInArcSweepRange(vtarc, ftempx2, ftempy2, &alpha2);
						bFtemp1 = (ftempx1 > MIN(vtline.x1, vtline.x2)) && (ftempx1 < MAX(vtline.x1, vtline.x2));
						bFtemp2 = (ftempx2 > MIN(vtline.x1, vtline.x2)) && (ftempx2 < MAX(vtline.x1, vtline.x2));
						if(vtline.x1 > vtline.x2)
						{
							double ftemp = vtline.x1;
							vtline.x1 = vtline.x2;
							vtline.x2 = ftemp;
						}
					}
				}
				else
				{
					double k = (vtline.y2 - vtline.y1)/(vtline.x2 - vtline.x1);
					if(fabs((fabs(k*vtarc.cx - vtarc.cy + vtline.y1 - k*vtline.x1)/sqrt(k*k + 1)) - vtarc.r) < 0.1)	
					{
						ftempx1 = ( vtarc.cx + k*vtarc.cy + k*k*vtline.x1 - k*vtline.y1 )/(1+k*k);
						ftempy1 = ( vtline.y1 + k*vtarc.cx + k*k*vtarc.cy - k*vtline.x1 )/(1+k*k);
						bAlpha1 = IsInArcSweepRange(vtarc, ftempx1, ftempy1, &alpha1);
						bAlpha2 = FALSE;					
						bFtemp1 = (ftempx1 > MIN(vtline.x1, vtline.x2)) && (ftempx1 < MAX(vtline.x1, vtline.x2)) && (ftempy1 > MIN(vtline.y1, vtline.y2)) && (ftempy1 < MAX(vtline.y1, vtline.y2));
						bFtemp2 = FALSE;
						if(vtline.x1 > vtline.x2)
						{
							double ftemp = vtline.x1;
							vtline.x1 = vtline.x2;
							vtline.x2 = ftemp;
							ftemp = vtline.y1;
							vtline.y1 = vtline.y2;
							vtline.y2 = ftemp;
						}						
					}
					else
					{
						
						double b = k*vtarc.cx - vtarc.cy + vtline.y1 - k*vtline.x1;
						if(((2*k*b*2*k*b) - 4*(k*k+1)*(b*b-vtarc.r*vtarc.r)) >= 0)	
						{
							ftempx1 = (0 - 2*k*b - sqrt((2*k*b*2*k*b) - 4*(k*k+1)*(b*b-vtarc.r*vtarc.r)))/(2*(k*k+1));
							ftempx2 = (0 - 2*k*b + sqrt((2*k*b*2*k*b) - 4*(k*k+1)*(b*b-vtarc.r*vtarc.r)))/(2*(k*k+1));
							ftempy1 = k*ftempx1 + b;
							ftempy2 = k*ftempx2 + b;
							
							ftempx1 += vtarc.cx;	ftempy1 += vtarc.cy;
							ftempx2 += vtarc.cx;	ftempy2 += vtarc.cy;

							bAlpha1 = IsInArcSweepRange(vtarc, ftempx1, ftempy1, &alpha1);
							bAlpha2 = IsInArcSweepRange(vtarc, ftempx2, ftempy2, &alpha2);
							bFtemp1 = ((ftempx1 > MIN(vtline.x1, vtline.x2)) && (ftempx1 < MAX(vtline.x1, vtline.x2))) && \
									  ((ftempy1 > MIN(vtline.y1, vtline.y2)) && (ftempy1 < MAX(vtline.y1, vtline.y2))) ;
							bFtemp2 = ((ftempx2 > MIN(vtline.x1, vtline.x2)) && (ftempx2 < MAX(vtline.x1, vtline.x2))) && \
									  ((ftempy2 > MIN(vtline.y1, vtline.y2)) && (ftempy2 < MAX(vtline.y1, vtline.y2))) ;
							if(vtline.x1 > vtline.x2)
							{
								double ftemp = vtline.x1;
								vtline.x1 = vtline.x2;
								vtline.x2 = ftemp;
								ftemp = vtline.y1;
								vtline.y1 = vtline.y2;
								vtline.y2 = ftemp;
							}
						}
						else continue;
					}
				}

				if( (bAlpha1 == TRUE) && (bFtemp1 == TRUE) )
				{
					if( (bAlpha2 == TRUE) && (bFtemp2 == TRUE) )	
					{
						
						if(aaa.kind == 0)
						{
							aaa.x1 = vtline.x1;		aaa.y1 = vtline.y1;
							aaa.x2 = ftempx1;		aaa.y2 = ftempy1;
							arrayVertexOutline.Add(aaa);
							
							aaa.x1 = ftempx1;		aaa.y1 = ftempy1;
							aaa.x2 = ftempx2;		aaa.y2 = ftempy2;
							arrayVertexOutline.Add(aaa);

							aaa.x1 = ftempx2;		aaa.y1 = ftempy2;
							aaa.x2 = vtline.x2;		aaa.y2 = vtline.y2;
							arrayVertexOutline.Add(aaa);
							
							if(alpha1 > alpha2)
							{
								double ftemp;
								ftemp = alpha1;
								alpha1 = alpha2;
								alpha2 = ftemp;
							}

							bbb.sangle = pDoc->Get_Correct_Angle(vtarc.sangle);
							bbb.eangle = pDoc->Get_Correct_Angle(alpha1);
							bbb.x1 = vtarc.x1;		bbb.y1 = vtarc.y1;
							bbb.x2 = bbb.cx + bbb.r*cos(bbb.eangle*fEDA_pi/180);
							bbb.y2 = bbb.cy + bbb.r*sin(bbb.eangle*fEDA_pi/180);
							arrayVertexOutline.Add(bbb);
							
							bbb.sangle = pDoc->Get_Correct_Angle(alpha1);
							bbb.eangle = pDoc->Get_Correct_Angle(alpha2);
							bbb.x1 = bbb.cx + bbb.r*cos(bbb.sangle*fEDA_pi/180);	
							bbb.y1 = bbb.cy + bbb.r*sin(bbb.sangle*fEDA_pi/180);
							bbb.x2 = bbb.cx + bbb.r*cos(bbb.eangle*fEDA_pi/180);
							bbb.y2 = bbb.cy + bbb.r*sin(bbb.eangle*fEDA_pi/180);
							arrayVertexOutline.Add(bbb);
								
							bbb.sangle = pDoc->Get_Correct_Angle(alpha2);
							bbb.eangle = pDoc->Get_Correct_Angle(vtarc.eangle);
							bbb.x1 = bbb.cx + bbb.r*cos(bbb.sangle*fEDA_pi/180);	
							bbb.y1 = bbb.cy + bbb.r*sin(bbb.sangle*fEDA_pi/180);
							bbb.x2 = vtarc.x2;		bbb.y2 = vtarc.y2;
							arrayVertexOutline.Add(bbb);

							arrayVertexOutline.RemoveAt(j);
							arrayVertexOutline.RemoveAt(i);
							i--;							
							break;
						}
						else
						{
							bbb.x1 = vtline.x1;		bbb.y1 = vtline.y1;
							bbb.x2 = ftempx1;		bbb.y2 = ftempy1;
							arrayVertexOutline.Add(bbb);

							bbb.x1 = ftempx1;		bbb.y1 = ftempy1;
							bbb.x2 = ftempx2;		bbb.y2 = ftempy2;
							arrayVertexOutline.Add(bbb);

							bbb.x1 = ftempx2;		bbb.y1 = ftempy2;
							bbb.x2 = vtline.x2;		bbb.y2 = vtline.y2;
							arrayVertexOutline.Add(bbb);

							if(alpha1 > alpha2)
							{
								double ftemp;
								ftemp = alpha1;
								alpha1 = alpha2;
								alpha2 = ftemp;
							}

							aaa.sangle = pDoc->Get_Correct_Angle(vtarc.sangle);
							aaa.eangle = pDoc->Get_Correct_Angle(alpha1);
							aaa.x1 = vtarc.x1;		aaa.y1 = vtarc.y1;
							aaa.x2 = aaa.cx + aaa.r*cos(aaa.eangle*fEDA_pi/180);
							aaa.y2 = aaa.cy + aaa.r*sin(aaa.eangle*fEDA_pi/180);
							arrayVertexOutline.Add(aaa);
								
							aaa.sangle = pDoc->Get_Correct_Angle(alpha1);
							aaa.eangle = pDoc->Get_Correct_Angle(alpha2);
							aaa.x1 = aaa.cx + aaa.r*cos(aaa.sangle*fEDA_pi/180);	
							aaa.y1 = aaa.cy + aaa.r*sin(aaa.sangle*fEDA_pi/180);
							aaa.x2 = aaa.cx + aaa.r*cos(aaa.eangle*fEDA_pi/180);
							aaa.y2 = aaa.cy + aaa.r*sin(aaa.eangle*fEDA_pi/180);
							arrayVertexOutline.Add(aaa);
								
							aaa.sangle = pDoc->Get_Correct_Angle(alpha2);
							aaa.eangle = pDoc->Get_Correct_Angle(vtarc.eangle);
							aaa.x1 = aaa.cx + aaa.r*cos(aaa.sangle*fEDA_pi/180);	
							aaa.y1 = aaa.cy + aaa.r*sin(aaa.sangle*fEDA_pi/180);
							aaa.x2 = vtarc.x2;		aaa.y2 = vtarc.y2;
							arrayVertexOutline.Add(aaa);

							arrayVertexOutline.RemoveAt(j);
							arrayVertexOutline.RemoveAt(i);
							i--;							
							break;
						}
					}
					else	
					{
						
						if(aaa.kind == 0)
						{
							aaa.x1 = vtline.x1;		aaa.y1 = vtline.y1;
							aaa.x2 = ftempx1;		aaa.y2 = ftempy1;
							arrayVertexOutline.Add(aaa);
							
							aaa.x1 = ftempx1;		aaa.y1 = ftempy1;
							aaa.x2 = vtline.x2;		aaa.y2 = vtline.y2;
							arrayVertexOutline.Add(aaa);
							
							bbb.sangle = pDoc->Get_Correct_Angle(vtarc.sangle);
							bbb.eangle = pDoc->Get_Correct_Angle(alpha1);
							bbb.x1 = vtarc.x1;		bbb.y1 = vtarc.y1;
							bbb.x2 = bbb.cx + bbb.r*cos(bbb.eangle*fEDA_pi/180);
							bbb.y2 = bbb.cy + bbb.r*sin(bbb.eangle*fEDA_pi/180);
							arrayVertexOutline.Add(bbb);
								
							bbb.sangle = pDoc->Get_Correct_Angle(alpha1);
							bbb.eangle = pDoc->Get_Correct_Angle(vtarc.eangle);
							bbb.x1 = bbb.cx + bbb.r*cos(bbb.sangle*fEDA_pi/180);	
							bbb.y1 = bbb.cy + bbb.r*sin(bbb.sangle*fEDA_pi/180);
							bbb.x2 = vtarc.x2;		bbb.y2 = vtarc.y2;
							arrayVertexOutline.Add(bbb);

							arrayVertexOutline.RemoveAt(j);
							arrayVertexOutline.RemoveAt(i);
							i--;	
							break;
						}
						else
						{
							bbb.x1 = vtline.x1;		bbb.y1 = vtline.y1;
							bbb.x2 = ftempx1;		bbb.y2 = ftempy1;
							arrayVertexOutline.Add(bbb);

							bbb.x1 = ftempx1;		bbb.y1 = ftempy1;
							bbb.x2 = vtline.x2;		bbb.y2 = vtline.y2;
							arrayVertexOutline.Add(bbb);

							aaa.sangle = pDoc->Get_Correct_Angle(vtarc.sangle);
							aaa.eangle = pDoc->Get_Correct_Angle(alpha1);
							aaa.x1 = vtarc.x1;		aaa.y1 = vtarc.y1;
							aaa.x2 = aaa.cx + aaa.r*cos(aaa.eangle*fEDA_pi/180);
							aaa.y2 = aaa.cy + aaa.r*sin(aaa.eangle*fEDA_pi/180);
							arrayVertexOutline.Add(aaa);
								
							aaa.sangle = pDoc->Get_Correct_Angle(alpha1);
							aaa.eangle = pDoc->Get_Correct_Angle(vtarc.eangle);
							aaa.x1 = aaa.cx + aaa.r*cos(aaa.sangle*fEDA_pi/180);	
							aaa.y1 = aaa.cy + aaa.r*sin(aaa.sangle*fEDA_pi/180);
							aaa.x2 = vtarc.x2;		aaa.y2 = vtarc.y2;
							arrayVertexOutline.Add(aaa);

							arrayVertexOutline.RemoveAt(j);
							arrayVertexOutline.RemoveAt(i);
							i--;	
							break;
						}
					}
				}
				else if( (bAlpha2 == TRUE) && (bFtemp2 == TRUE) )	
				{
						
						if(aaa.kind == 0)
						{
							aaa.x1 = vtline.x1;		aaa.y1 = vtline.y1;
							aaa.x2 = ftempx2;		aaa.y2 = ftempy2;
							arrayVertexOutline.Add(aaa);
							
							aaa.x1 = ftempx2;		aaa.y1 = ftempy2;
							aaa.x2 = vtline.x2;		aaa.y2 = vtline.y2;
							arrayVertexOutline.Add(aaa);
							
							bbb.sangle = pDoc->Get_Correct_Angle(vtarc.sangle);
							bbb.eangle = pDoc->Get_Correct_Angle(alpha2);
							bbb.x1 = vtarc.x1;		bbb.y1 = vtarc.y1;
							bbb.x2 = bbb.cx + bbb.r*cos(bbb.eangle*fEDA_pi/180);
							bbb.y2 = bbb.cy + bbb.r*sin(bbb.eangle*fEDA_pi/180);
							arrayVertexOutline.Add(bbb);
															
							bbb.sangle = pDoc->Get_Correct_Angle(alpha2);
							bbb.eangle = pDoc->Get_Correct_Angle(vtarc.eangle);
							bbb.x1 = bbb.cx + bbb.r*cos(bbb.sangle*fEDA_pi/180);	
							bbb.y1 = bbb.cy + bbb.r*sin(bbb.sangle*fEDA_pi/180);
							bbb.x2 = vtarc.x2;		bbb.y2 = vtarc.y2;
							arrayVertexOutline.Add(bbb);

							arrayVertexOutline.RemoveAt(j);
							arrayVertexOutline.RemoveAt(i);
							i--;	
							break;
						}
						else
						{
							bbb.x1 = vtline.x1;		bbb.y1 = vtline.y1;
							bbb.x2 = ftempx2;		bbb.y2 = ftempy2;
							arrayVertexOutline.Add(bbb);
							
							bbb.x1 = ftempx2;		bbb.y1 = ftempy2;
							bbb.x2 = vtline.x2;		bbb.y2 = vtline.y2;
							arrayVertexOutline.Add(bbb);

							aaa.sangle = pDoc->Get_Correct_Angle(vtarc.sangle);
							aaa.eangle = pDoc->Get_Correct_Angle(alpha2);
							aaa.x1 = vtarc.x1;		aaa.y1 = vtarc.y1;
							aaa.x2 = aaa.cx + aaa.r*cos(aaa.eangle*fEDA_pi/180);
							aaa.y2 = aaa.cy + aaa.r*sin(aaa.eangle*fEDA_pi/180);
							arrayVertexOutline.Add(aaa);
								
							aaa.sangle = pDoc->Get_Correct_Angle(alpha2);
							aaa.eangle = pDoc->Get_Correct_Angle(vtarc.eangle);
							aaa.x1 = aaa.cx + aaa.r*cos(aaa.sangle*fEDA_pi/180);	
							aaa.y1 = aaa.cy + aaa.r*sin(aaa.sangle*fEDA_pi/180);
							aaa.x2 = vtarc.x2;		aaa.y2 = vtarc.y2;
							arrayVertexOutline.Add(aaa);

							arrayVertexOutline.RemoveAt(j);
							arrayVertexOutline.RemoveAt(i);
							i--;	
							break;
						}
				}
			}
		}
	}

	
	Write_Help("覆铜: 处理数据...");
	for(int i=0; i<arrayVertexOutline.GetCount(); i++)
	{
		Struct_VertexOutline vertexoutline = arrayVertexOutline.GetAt(i);
		if(vertexoutline.kind == 0)
		{
			int x1, y1, x2, y2;
			x1 = (int)(vertexoutline.x1 - fminx);	y1 = (int)(vertexoutline.y1 - fminy);
			x2 = (int)(vertexoutline.x2 - fminx);	y2 = (int)(vertexoutline.y2 - fminy);

			if(abs(x1-x2) >= abs(y1-y2))
			{
				if(x1 > x2)
				{
					int temp;
					temp = x1;	x1 = x2;	x2 = temp;
					temp = y1;	y1 = y2;	y2 = temp;
				}
				
				int nTrue = 0;
				int nFalse = 0;
				for(int x=x1; x<=x2; x++)
				{
					int mx,my;
					mx = x;
					if(y1 == y2) my = y1;
					else my = ((double)y2-y1)*(((double)x-x1)/((double)x2-x1)) + y1;

					if(IsOnSolidPoint(mx, my, buffer, dx, cy) == FALSE)
					{
						arrayVertexOutline.RemoveAt(i);
						i--;
						break;
					}
				}
			}
			else
			{
				if(y1 > y2)
				{
					int temp;
					temp = x1;	x1 = x2;	x2 = temp;
					temp = y1;	y1 = y2;	y2 = temp;
				}
				
				int nTrue = 0;
				int nFalse = 0;
				for(int y=y1; y<=y2; y++)
				{
					int mx,my;
					my = y;
					if(x1 == x2) mx = x1;
					else mx = ((double)x2-x1)*(((double)y-y1)/((double)y2-y1)) + x1;

					if(IsOnSolidPoint(mx, my, buffer, dx, cy) == FALSE)
					{
						arrayVertexOutline.RemoveAt(i);
						i--;
						break;
					}
				}
			}
		}
		else
		{
			int iSangle, iEangle;
			iSangle = (int)vertexoutline.sangle;
			iEangle = (int)vertexoutline.eangle;

			if(iEangle < iSangle) iEangle += 360;
				
			int nTrue = 0;
			int nFalse = 0;		
			for(int angle=iSangle; angle < iEangle; angle++)
			{
				int x, y;
				x = (int)(vertexoutline.cx + (vertexoutline.r*cos(angle*fEDA_pi/180)) - fminx);
				y = (int)(vertexoutline.cy + (vertexoutline.r*sin(angle*fEDA_pi/180)) - fminy);

				if(IsOnSolidPoint(x, y, buffer, dx, cy) == FALSE)
				{
					arrayVertexOutline.RemoveAt(i);
					i--;
					break;
				}
			}
		}
	}

	Write_Help("覆铜: 布尔运算...");
	CArray<Struct_VertexOutline, Struct_VertexOutline&> arrayTempOutline;
	double fMinDisAcceptable = drc;
	
	double current_fx, current_fy;
	for(;;)
	{
		arrayTempOutline.RemoveAll();

		
		int flag = 0;
		double dis = -100000;
		int tag = -1;
		for(int i=0; i<arrayVertexOutline.GetCount(); i++)
		{
			Struct_VertexOutline vertexoutline = arrayVertexOutline.GetAt(i);
			if(vertexoutline.kind == 0)
			{
				double l = SQRT(vertexoutline.x1, vertexoutline.y1, vertexoutline.x2, vertexoutline.y2);
				if(l > dis)
				{
					tag = i;
					dis = l;
				}
			}
		}
		if(tag >= 0)
		{
			Struct_VertexOutline vertexoutline = arrayVertexOutline.GetAt(tag);
			vertexoutline.flag = 1;	

			arrayTempOutline.Add(vertexoutline);
			current_fx = vertexoutline.x2;
			current_fy = vertexoutline.y2;					

			arrayVertexOutline.RemoveAt(tag);	
			flag = 1;
		}

		if(flag == 0)
		{
			for(int i=0; i<arrayVertexOutline.GetCount(); i++)
			{
				Struct_VertexOutline vertexoutline = arrayVertexOutline.GetAt(i);
				vertexoutline.flag = 1;	
				if(vertexoutline.kind == 1)
				{
					if(fabs(vertexoutline.eangle - vertexoutline.sangle) == 360.0)
					{
						COb_pcbregion aregion;
						aregion.pDocument = pDoc;
						aregion.m_nLayer = ppolygon->m_nLayer;	

						
						Struct_RegionVertex vertex;
						vertex.cx = vertexoutline.cx;
						vertex.cy = vertexoutline.cy;
						vertex.radius = vertexoutline.r;
						vertex.sangle = vertexoutline.sangle;
						vertex.eangle = vertexoutline.eangle;
						vertex.fx =	vertex.cx + (vertex.radius*cos(vertex.sangle*fEDA_pi/180));
						vertex.fy =	vertex.cy + (vertex.radius*sin(vertex.sangle*fEDA_pi/180));
						aregion.m_cVertex.Add(vertex);

						
						aregion.GetRegionVertex(arrayVertex);
						CleanVertex(arrayVertex);
					
						if( arrayVertex.GetCount() >= 3 )
						{
							aregion.m_cVertex.RemoveAll();
							for(int k=0; k<arrayVertex.GetCount(); k++)
							{
								Fpoint fp = arrayVertex.GetAt(k);
								Struct_RegionVertex vertex;
								vertex.radius = vertex.cx = vertex.cy = vertex.sangle = vertex.eangle = 0;
								vertex.fx = fp.fx;
								vertex.fy = fp.fy;
								aregion.m_cVertex.Add(vertex);
							}
							
							if(GetRegionArea(&aregion) > 1500)	
							{
								CArray<Struct_VertexOutline, Struct_VertexOutline&> arrayFillLine;
								GetRegionFillLine(&aregion, &arrayFillLine, 1, 2, PCB_HATCH_V);

								for(int k=0; k<arrayFillLine.GetCount(); k++)
								{
									Struct_VertexOutline vt;
									vt = arrayFillLine.GetAt(k);

									if((vt.y2 - vt.y1) > 5)
									{
										for(int m=1; m<=5; m++)	
										{
											int x, y;
											x = (int)(vt.x1  - fminx);
											y = (int)(vt.y1  - fminy) + m;
											if(y > vt.y2) continue;
											if(IsOnSolidPoint(x, y, buffer, dx, cy) == FALSE)
											{
												aregion.m_bPolygonCutout = TRUE;
												break;
											}
										}
									}
									if(aregion.m_bPolygonCutout == TRUE) break;
								}

								
								COb_pcbregion* pnewregion = new COb_pcbregion();
								pnewregion->Copy(&aregion);
								pnewregion->pDocument = pDoc;
								pnewregion->m_nLayer = ppolygon->m_nLayer;
								pnewregion->m_nPolygon = -1;
								if(pnewregion->m_bPolygonCutout == FALSE) arrayRegion.Add(pnewregion);
								else arrayCutout.Add(pnewregion);
							}
						}

						arrayVertexOutline.RemoveAt(i);	
						flag = 2;
						break;
					}
					else
					{
						arrayTempOutline.Add(vertexoutline);
						current_fx = vertexoutline.x2;
						current_fy = vertexoutline.y2;						

						arrayVertexOutline.RemoveAt(i);	
						flag = 1;
						break;
					}
				}
			}
		}

		if(flag == 0) break;
		if(flag == 2) continue;

		
		BOOL bClosed, bOK;
		do
		{
			
			int pIndex=-1;
			double dmin = 100000;
			Struct_VertexOutline  vt_next;
			for(int i=0; i<arrayVertexOutline.GetCount(); i++)
			{
				Struct_VertexOutline vertexoutline;
				vertexoutline = arrayVertexOutline.GetAt(i);
				if(vertexoutline.kind == 0)
				{
					double ftmaxx = MAX(vertexoutline.x1, vertexoutline.x2);
					double ftminx = MIN(vertexoutline.x1, vertexoutline.x2);
					double ftmaxy = MAX(vertexoutline.y1, vertexoutline.y2);
					double ftminy = MIN(vertexoutline.y1, vertexoutline.y2);
				    
					if( ((current_fx < ftminx)&&((ftminx - current_fx) > fMinDisAcceptable)) || ((current_fx > ftmaxx)&&((current_fx - ftmaxx) > fMinDisAcceptable)) ||\
					    ((current_fy < ftminy)&&((ftminy - current_fy) > fMinDisAcceptable)) || ((current_fy > ftmaxy)&&((current_fy - ftmaxy) > fMinDisAcceptable)) ) continue;
					
					double d;
					d = SQRT(current_fx, current_fy, vertexoutline.x1, vertexoutline.y1);
					if((d < dmin)&&(d < fMinDisAcceptable))
					{
						dmin = d;
						pIndex = i;
						vt_next.kind = vertexoutline.kind;	vt_next.cx = vertexoutline.cx;	vt_next.cy = vertexoutline.cy;	vt_next.r = vertexoutline.r;
						vt_next.x1 = vertexoutline.x1;		vt_next.y1 = vertexoutline.y1;	vt_next.x2 = vertexoutline.x2;	vt_next.y2 = vertexoutline.y2;
						vt_next.sangle = vertexoutline.sangle;	vt_next.eangle = vertexoutline.eangle;
						vt_next.flag = 1;
					}
					d = SQRT(current_fx, current_fy, vertexoutline.x2, vertexoutline.y2);
					if((d < dmin) && (d < fMinDisAcceptable))
					{
						dmin = d;
						pIndex = i;
						vt_next.kind = vertexoutline.kind;	vt_next.cx = vertexoutline.cx;	vt_next.cy = vertexoutline.cy;	vt_next.r = vertexoutline.r;
						vt_next.x1 = vertexoutline.x1;		vt_next.y1 = vertexoutline.y1;	vt_next.x2 = vertexoutline.x2;	vt_next.y2 = vertexoutline.y2;
						vt_next.sangle = vertexoutline.sangle;	vt_next.eangle = vertexoutline.eangle;
						vt_next.flag = 0;
					}
				}
				else
				{
					if( ((current_fx > vertexoutline.cx)&&((current_fx - vertexoutline.cx) > (vertexoutline.r + fMinDisAcceptable))) || \
						((current_fx < vertexoutline.cx)&&((vertexoutline.cx - current_fx) > (vertexoutline.r + fMinDisAcceptable))) || \
						((current_fy > vertexoutline.cy)&&((current_fy - vertexoutline.cy) > (vertexoutline.r + fMinDisAcceptable))) || \
						((current_fy < vertexoutline.cy)&&((vertexoutline.cy - current_fy) > (vertexoutline.r + fMinDisAcceptable))) )	continue;

					double d;
					d = SQRT(current_fx, current_fy, vertexoutline.x1, vertexoutline.y1);
					if((d < dmin)&&(d < fMinDisAcceptable))
					{
						dmin = d;
						pIndex = i;
						vt_next.kind = vertexoutline.kind;	vt_next.cx = vertexoutline.cx;	vt_next.cy = vertexoutline.cy;	vt_next.r = vertexoutline.r;
						vt_next.x1 = vertexoutline.x1;		vt_next.y1 = vertexoutline.y1;	vt_next.x2 = vertexoutline.x2;	vt_next.y2 = vertexoutline.y2;
						vt_next.sangle = vertexoutline.sangle;	vt_next.eangle = vertexoutline.eangle;
						vt_next.flag = 1;
					}
					d = SQRT(current_fx, current_fy, vertexoutline.x2, vertexoutline.y2);
					if((d < dmin) && (d < fMinDisAcceptable))
					{
						dmin = d;
						pIndex = i;
						vt_next.kind = vertexoutline.kind;	vt_next.cx = vertexoutline.cx;	vt_next.cy = vertexoutline.cy;	vt_next.r = vertexoutline.r;
						vt_next.x1 = vertexoutline.x1;		vt_next.y1 = vertexoutline.y1;	vt_next.x2 = vertexoutline.x2;	vt_next.y2 = vertexoutline.y2;
						vt_next.sangle = vertexoutline.sangle;	vt_next.eangle = vertexoutline.eangle;
						vt_next.flag = 0;
					}
				}
			}

			BOOL bNextVertexFound;
			if(pIndex >= 0)
			{
				
				bNextVertexFound = TRUE;
				
				arrayVertexOutline.RemoveAt(pIndex);	
				arrayTempOutline.Add(vt_next);
				
				current_fx = vt_next.flag == 0 ? vt_next.x1:vt_next.x2;
				current_fy = vt_next.flag == 0 ? vt_next.y1:vt_next.y2;
			}
			else
			{
				
				bNextVertexFound = FALSE;
			}

			
			bClosed = FALSE;
			bOK = FALSE;
			if(bNextVertexFound == TRUE)	
			{
				Struct_VertexOutline vt1, vt2;
				vt1 = arrayTempOutline.GetAt(0);
				vt2 = arrayTempOutline.GetAt(arrayTempOutline.GetCount() - 1);
				
				if((vt1.x1 == (vt2.flag == 0 ? vt2.x1:vt2.x2))&&(vt1.y1 == (vt2.flag == 0 ? vt2.y1:vt2.y2)))	
				{
					bClosed = TRUE;
					bOK = TRUE;
				}
				else	
				{
				}
			}
			else	
			{
				if(arrayTempOutline.GetCount() <= 1)	
				{
					bClosed = TRUE;
					bOK = FALSE;
				}
				else	
				{
					Struct_VertexOutline vt1, vt2;
					vt1 = arrayTempOutline.GetAt(0);
					vt2 = arrayTempOutline.GetAt(arrayTempOutline.GetCount() -1);

					double endx, endy;
					if(vt2.flag == 1)
					{
						endx = vt2.x2;	
						endy = vt2.y2;
					}
					else
					{
						endx = vt2.x1;	
						endy = vt2.y1;
					}

					if(SQRT(vt1.x1, vt1.y1, endx, endy) < fMinDisAcceptable)	
					{
						bClosed = TRUE;
						bOK = TRUE;
					}
					else	
					{
						arrayTempOutline.RemoveAt(arrayTempOutline.GetCount() - 1);
						Struct_VertexOutline vt = arrayTempOutline.GetAt(arrayTempOutline.GetCount() - 1);
						if(vt.flag == 1)
						{
							current_fx = vt.x2;
							current_fy = vt.y2;
						}
						else
						{
							current_fx = vt.x1;
							current_fy = vt.y1;	
						}
					}
				}
			}
	
			if((bClosed == TRUE)&&(bOK == TRUE))
			{
				COb_pcbregion aregion;
				aregion.pDocument = pDoc;
				aregion.m_nLayer = ppolygon->m_nLayer;

				for(int k=0; k<arrayTempOutline.GetCount(); k++)
				{
					Struct_VertexOutline vt;
					vt = arrayTempOutline.GetAt(k);
					if(vt.kind == 0)
					{
						Struct_RegionVertex vertex;
						vertex.radius = vertex.cx = vertex.cy = vertex.sangle = vertex.eangle = 0;
						if(vt.flag == 1)
						{
							vertex.fx = vt.x1;
							vertex.fy = vt.y1;
							aregion.m_cVertex.Add(vertex);
							vertex.fx = vt.x2;
							vertex.fy = vt.y2;
							aregion.m_cVertex.Add(vertex);
						}
						else
						{
							vertex.fx = vt.x2;
							vertex.fy = vt.y2;
							aregion.m_cVertex.Add(vertex);
							vertex.fx = vt.x1;
							vertex.fy = vt.y1;
							aregion.m_cVertex.Add(vertex);
						}
					}
					else
					{
						Struct_RegionVertex vertex;
						vertex.radius = vt.r;
						vertex.cx = vt.cx;
						vertex.cy = vt.cy;
						vertex.sangle = vt.sangle;
						vertex.eangle = vt.eangle;
						if(vt.flag == 1)
						{
							vertex.fx = vt.x1;
							vertex.fy = vt.y1;
							aregion.m_cVertex.Add(vertex);
						}
						else
						{
							vertex.fx = vt.x2;
							vertex.fy = vt.y2;
							aregion.m_cVertex.Add(vertex);
						}
					}
				}

				aregion.GetRegionVertex(arrayVertex);
				CleanVertex(arrayVertex);

				if( arrayVertex.GetCount() >= 3 )
				{
					aregion.m_cVertex.RemoveAll();
					for(int k=0; k<arrayVertex.GetCount(); k++)
					{
						Fpoint fp = arrayVertex.GetAt(k);
						Struct_RegionVertex vertex;
						vertex.radius = vertex.cx = vertex.cy = vertex.sangle = vertex.eangle = 0;
						vertex.fx = fp.fx;
						vertex.fy = fp.fy;
						aregion.m_cVertex.Add(vertex);
					}

					if(GetRegionArea(&aregion) > 1500)	
					{
						CArray<Struct_VertexOutline, Struct_VertexOutline&> arrayFillLine;
						
						GetRegionFillLine(&aregion, &arrayFillLine, 1, 2, PCB_HATCH_V);
						for(int k=0; k<arrayFillLine.GetCount(); k++)
						{
							Struct_VertexOutline vt;
							vt = arrayFillLine.GetAt(k);

							if((vt.y2 - vt.y1) > 10)
							{
								int x, y;
								x = (int)(vt.x1  - fminx);
								y = (int)(vt.y1  - fminy) + 8; 
								if(y > vt.y2) continue;

								if(IsOnSolidPoint(x, y, buffer, dx, cy) == FALSE)
								{
									aregion.m_bPolygonCutout = TRUE;
									break;
								}
							}
						}

						COb_pcbregion* pnewregion = new COb_pcbregion();
						pnewregion->Copy(&aregion);
						pnewregion->pDocument = pDoc;
						pnewregion->m_nLayer = ppolygon->m_nLayer;
						pnewregion->m_nPolygon = -1;
						if(pnewregion->m_bPolygonCutout == FALSE) arrayRegion.Add(pnewregion);
						else arrayCutout.Add(pnewregion);
					}
				}
			}

		}while(bClosed != TRUE);
	}

	Write_Help("覆铜: 优化...");
	
	for(int i=0; i<arrayRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = arrayRegion.GetAt(i);
		pregion->area = GetRegionArea(pregion);
	}
	
	
	int judge = 1;
	for(;;)
	{
		if(judge == 1) judge = 0;
		else if(judge == 0) break;
		for(int i=0; i<arrayRegion.GetCount()-1;i++)
		{
			COb_pcbregion* pregion_1 = arrayRegion.GetAt(i);
			COb_pcbregion* pregion_2 = arrayRegion.GetAt(i + 1);
			if(pregion_1->area > pregion_2->area)
			{
				judge = 1;
				arrayRegion.SetAt(i, pregion_2);
				arrayRegion.SetAt(i+1, pregion_1);
			}
		}
	}

	
	int sub_region = 0;
	for(int i=0; i<arrayRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = arrayRegion.GetAt(i);
		COb_pcbregion  aregion_for_check;	
		aregion_for_check.Copy(pregion);

		for(int j=0; j<arrayCutout.GetCount(); j++)
		{
			COb_pcbregion* pcutout = arrayCutout.GetAt(j);
			
			Struct_RegionVertex vertex;
			vertex = pcutout->m_cVertex.GetAt(0);
			if(aregion_for_check.Is_On_Me(vertex.fx, vertex.fy) == TRUE)
			{
				CArray<Fpoint, Fpoint>* p_arrayHole = new CArray<Fpoint, Fpoint>;
				for(int k=0; k<pcutout->m_cVertex.GetCount(); k++)
				{
					Struct_RegionVertex vertex;
					vertex = pcutout->m_cVertex.GetAt(k);
					Fpoint fp;
					fp.fx = vertex.fx;
					fp.fy = vertex.fy;
					p_arrayHole->Add(fp);
				}
				pregion->m_arrayHoles.Add(p_arrayHole);

				delete pcutout;
				arrayCutout.RemoveAt(j);
				j--;
			}
		}

		
		if((ppolygon->m_bRemoveIslandsByArea == TRUE)&&(pregion->m_arrayHoles.GetCount() == 0)&&(pregion->area < ppolygon->m_fAreaThreshold))
		{
			delete pregion;
			arrayRegion.RemoveAt(i);
			i--;
			continue;
		}

		
		int flag = 0;
		if((ppolygon->m_nNet >= 0)&&((pDoc->m_nPolygonConnectStyle == PCB_RELIEF_CONNECT)||(ppolygon->m_bRemoveDead == TRUE)))
		{
			double d;
			if(ppolygon->m_bRemoveNecks == TRUE) d = ppolygon->m_fPourGap + ppolygon->m_fTrackWidth + ppolygon->m_fNeckWidthThreshold + fEDA_pi;
			else d = ppolygon->m_fPourGap + ppolygon->m_fTrackWidth + fEDA_pi;

			for(int j=0; j<arrayObjects.GetCount(); j++)
			{
				Struct_Pcb_Multi_Selection selected_element;
				selected_element = arrayObjects.GetAt(j);

				double fx, fy, fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
				if(selected_element.pcbelement == PCB_ELEMENT_VIA)
				{
					COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
					if(pvia->m_nNet != ppolygon->m_nNet) continue;

					fx = pvia->m_fCx;
					fy = pvia->m_fCy;
					fx1 = pvia->m_fCx - (pvia->m_fDia/2 + d);
					fy1 = pvia->m_fCy;
					fx2 = pvia->m_fCx + (pvia->m_fDia/2 + d);
					fy2 = pvia->m_fCy;
					fx3 = pvia->m_fCx;
					fy3 = pvia->m_fCy - (pvia->m_fDia/2 + d);
					fx4 = pvia->m_fCx;
					fy4 = pvia->m_fCy + (pvia->m_fDia/2 + d);
				}
				else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
					if(ppad->m_nNet != ppolygon->m_nNet) continue;

					fx = ppad->m_fCx;
					fy = ppad->m_fCy;
					double rx, ry;
					if(ppad->m_nPadMode == PAD_MODE_SIMPLE)
					{
						rx = ppad->m_fSizeX[TOPLAYER]/2;
						ry = ppad->m_fSizeY[TOPLAYER]/2;
					}
					else
					{
						if(ppolygon->m_nLayer == TOPLAYER)
						{
							rx = ppad->m_fSizeX[TOPLAYER]/2;
							ry = ppad->m_fSizeY[TOPLAYER]/2;
						}
						else if(ppolygon->m_nLayer == BOTTOMLAYER)
						{
							rx = ppad->m_fSizeX[BOTTOMLAYER]/2;
							ry = ppad->m_fSizeY[BOTTOMLAYER]/2;
						}
						else
						{
							rx = ppad->m_fSizeX[MIDLAYER1]/2;
							ry = ppad->m_fSizeY[MIDLAYER1]/2;
						}
					}
					fx1 = ppad->m_fCx - (rx + d)*cos(ppad->m_fRotation*fEDA_pi/180);
					fy1 = ppad->m_fCy - (rx + d)*sin(ppad->m_fRotation*fEDA_pi/180);
					fx2 = ppad->m_fCx + (rx + d)*cos(ppad->m_fRotation*fEDA_pi/180);
					fy2 = ppad->m_fCy + (rx + d)*sin(ppad->m_fRotation*fEDA_pi/180);
					fx3 = ppad->m_fCx - (ry + d)*sin(ppad->m_fRotation*fEDA_pi/180);
					fy3 = ppad->m_fCy + (ry + d)*cos(ppad->m_fRotation*fEDA_pi/180);
					fx4 = ppad->m_fCx + (ry + d)*sin(ppad->m_fRotation*fEDA_pi/180);
					fy4 = ppad->m_fCy - (ry + d)*cos(ppad->m_fRotation*fEDA_pi/180);
				}
				else continue;

				
				if(pregion->Is_On_Me(fx3, fy3) == TRUE)
				{
					COb_pcbtrack* pnewtrack = new COb_pcbtrack();
					pnewtrack->pDocument = pDoc;
					pnewtrack->m_nLayer = ppolygon->m_nLayer;
					pnewtrack->m_fWidth = pDoc->m_fPolygonConductorWidth;
					pnewtrack->m_fX1 = fx;
					pnewtrack->m_fY1 = fy;
					pnewtrack->m_fX2 = fx3;
					pnewtrack->m_fY2 = fy3;
					pnewtrack->m_nPolygon = index;
					pnewtrack->m_nSubPolyIndex = sub_region;
					int newindex = pDoc->m_arrayPcbTrack.Add(pnewtrack);
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = newindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					

					flag = 1;
				}
				if(pregion->Is_On_Me(fx4, fy4) == TRUE)
				{
					COb_pcbtrack* pnewtrack = new COb_pcbtrack();
					pnewtrack->pDocument = pDoc;
					pnewtrack->m_nLayer = ppolygon->m_nLayer;
					pnewtrack->m_fWidth = pDoc->m_fPolygonConductorWidth;
					pnewtrack->m_fX1 = fx;
					pnewtrack->m_fY1 = fy;
					pnewtrack->m_fX2 = fx4;
					pnewtrack->m_fY2 = fy4;
					pnewtrack->m_nPolygon = index;
					pnewtrack->m_nSubPolyIndex = sub_region;
					int newindex = pDoc->m_arrayPcbTrack.Add(pnewtrack);
					
					CPcbUndo *pundo = new CPcbUndo;
					pundo->m_nOp = UNDO_DELETE;
					pundo->m_nOb = PCB_ELEMENT_TRACK;
					pundo->m_nIndex = newindex;
					pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
					

					flag = 1;
				}				
				if(pDoc->m_nPolygonConductors == 4)
				{
					if(pregion->Is_On_Me(fx1, fy1) == TRUE)
					{
						COb_pcbtrack* pnewtrack = new COb_pcbtrack();
						pnewtrack->pDocument = pDoc;
						pnewtrack->m_nLayer = ppolygon->m_nLayer;
						pnewtrack->m_fWidth = pDoc->m_fPolygonConductorWidth;
						pnewtrack->m_fX1 = fx;
						pnewtrack->m_fY1 = fy;
						pnewtrack->m_fX2 = fx1;
						pnewtrack->m_fY2 = fy1;
						pnewtrack->m_nPolygon = index;
						pnewtrack->m_nSubPolyIndex = sub_region;
						int newindex = pDoc->m_arrayPcbTrack.Add(pnewtrack);
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = newindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
						flag = 1;
					}
					if(pregion->Is_On_Me(fx2, fy2) == TRUE)
					{
						COb_pcbtrack* pnewtrack = new COb_pcbtrack();
						pnewtrack->pDocument = pDoc;
						pnewtrack->m_nLayer = ppolygon->m_nLayer;
						pnewtrack->m_fWidth = pDoc->m_fPolygonConductorWidth;
						pnewtrack->m_fX1 = fx;
						pnewtrack->m_fY1 = fy;
						pnewtrack->m_fX2 = fx2;
						pnewtrack->m_fY2 = fy2;
						pnewtrack->m_nPolygon = index;
						pnewtrack->m_nSubPolyIndex = sub_region;
						int newindex = pDoc->m_arrayPcbTrack.Add(pnewtrack);
						
						CPcbUndo *pundo = new CPcbUndo;
						pundo->m_nOp = UNDO_DELETE;
						pundo->m_nOb = PCB_ELEMENT_TRACK;
						pundo->m_nIndex = newindex;
						pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
						
						flag = 1;
					}	
				}
			}
		}

		
		if((ppolygon->m_bRemoveDead == TRUE)&&(flag == 0)) continue;

		
		for(int j=0; j<pregion->m_cVertex.GetCount(); j++)	
		{
			Struct_RegionVertex vertex1, vertex2;
			vertex1 = pregion->m_cVertex.GetAt(j);
			if(j == (pregion->m_cVertex.GetCount() - 1)) vertex2 = pregion->m_cVertex.GetAt(0);
			else vertex2 = pregion->m_cVertex.GetAt(j+1);

			COb_pcbtrack* pnewtrack = new COb_pcbtrack();
			pnewtrack->pDocument = pDoc;
			pnewtrack->m_nLayer = ppolygon->m_nLayer;
			pnewtrack->m_fWidth = fTrackWidth;
			pnewtrack->m_fX1 = vertex1.fx;
			pnewtrack->m_fY1 = vertex1.fy;
			pnewtrack->m_fX2 = vertex2.fx;
			pnewtrack->m_fY2 = vertex2.fy;
			pnewtrack->m_nPolygon = index;
			pnewtrack->m_bPolygonOutline = TRUE;
			pnewtrack->m_nSubPolyIndex = sub_region;
			int newindex = pDoc->m_arrayPcbTrack.Add(pnewtrack);
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_TRACK;
			pundo->m_nIndex = newindex;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			
		}
		for(int j=0; j<pregion->m_arrayHoles.GetCount(); j++)	
		{
			CArray<Fpoint, Fpoint>* p_arrayHole = pregion->m_arrayHoles.GetAt(j);
			for(int k=0; k<p_arrayHole->GetCount(); k++)
			{
				Fpoint fp1, fp2;
				fp1 = p_arrayHole->GetAt(k);
				if(k == (p_arrayHole->GetCount() - 1)) fp2 = p_arrayHole->GetAt(0);
				else fp2 = p_arrayHole->GetAt(k+1);
					
				COb_pcbtrack* pnewtrack = new COb_pcbtrack();
				pnewtrack->pDocument = pDoc;
				pnewtrack->m_nLayer = ppolygon->m_nLayer;
				pnewtrack->m_fWidth = fTrackWidth;
				pnewtrack->m_fX1 = fp1.fx;
				pnewtrack->m_fY1 = fp1.fy;
				pnewtrack->m_fX2 = fp2.fx;
				pnewtrack->m_fY2 = fp2.fy;
				pnewtrack->m_nPolygon = index;
				pnewtrack->m_bPolygonOutline = TRUE;
				pnewtrack->m_nSubPolyIndex = sub_region;
				int newindex = pDoc->m_arrayPcbTrack.Add(pnewtrack);
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = PCB_ELEMENT_TRACK;
				pundo->m_nIndex = newindex;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
		}

		
		if(ppolygon->m_nHatchStyle == PCB_HATCH_SOLID)
		{
			pregion->m_nPolygon = index;
			pregion->m_nSubPolyIndex = sub_region;
			int newindex = pDoc->m_arrayPcbRegion.Add(pregion);
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_REGION;
			pundo->m_nIndex = newindex;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			
			arrayRegion.RemoveAt(i);
			i--;
		}
		else
		{
			
			CArray<Struct_VertexOutline, Struct_VertexOutline&> arrayFillLine;
			GetRegionFillLine(pregion, &arrayFillLine, fTrackWidth, ppolygon->m_fGridSize, ppolygon->m_nHatchStyle);

			for(int i=0; i<arrayFillLine.GetCount(); i++)
			{
				Struct_VertexOutline vt;
				vt = arrayFillLine.GetAt(i);
				
				COb_pcbtrack* pnewtrack = new COb_pcbtrack();
				pnewtrack->pDocument = pDoc;
				pnewtrack->m_nLayer = ppolygon->m_nLayer;
				pnewtrack->m_fWidth = fTrackWidth;
				pnewtrack->m_fX1 = vt.x1;
				pnewtrack->m_fY1 = vt.y1;
				pnewtrack->m_fX2 = vt.x2;
				pnewtrack->m_fY2 = vt.y2;
				pnewtrack->m_nPolygon = index;
				pnewtrack->m_nSubPolyIndex = sub_region;
				int newindex = pDoc->m_arrayPcbTrack.Add(pnewtrack);
				
				CPcbUndo *pundo = new CPcbUndo;
				pundo->m_nOp = UNDO_DELETE;
				pundo->m_nOb = PCB_ELEMENT_TRACK;
				pundo->m_nIndex = newindex;
				pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
				
			}
		}

		sub_region++;
	}
		
	free(buffer);
	memdc.SelectObject(pOldBrush);
	memdc.SelectObject(pOldPen);
	memdc.SelectObject(pOldBmp); 
	bmpFace.DeleteObject();
	while (!arrayRegion.IsEmpty())
	{
		delete arrayRegion.GetAt(0);
        arrayRegion.RemoveAt(0);
	}
	while (!arrayCutout.IsEmpty())
	{
		delete arrayCutout.GetAt(0);
        arrayCutout.RemoveAt(0);
	}
	
	Write_Help(szEDA_Help_Ready);
	Invalidate();
	return TRUE;
}

void CESDView::PolygonPourDrawVertex(CDC* pDC, CArray<Fpoint, Fpoint&>& arrayVertex, double fminx, double fminy)
{
	int vertex_number = arrayVertex.GetCount();
	POINT* p = (POINT*)malloc(2*vertex_number*sizeof(int));
	for(int j=0; j<vertex_number; j++)
	{
		Fpoint fpoint;
		fpoint = arrayVertex.GetAt(j);
		p[j].x = (int)(fpoint.fx - fminx);
		p[j].y = (int)(fpoint.fy - fminy);
	}

	pDC->Polygon(p, vertex_number);
	free(p);
}

BOOL CESDView::IsOnSolidPoint(int x, int y, char* buffer, int dx, int cy)
{
	int ii = x/8;
	int jj = y;
	int kk = 7 - x%8;

	
	if( (x<0) || (y<0) || ((x/8) > (dx-1)) || (y > (cy-1)) ) return FALSE;
	
	unsigned t = ((buffer[jj*dx + ii]&0x000000ff)>>kk)&0x01;

	if(t > 0) return TRUE;
	else 
	{
		unsigned t_left, t_right, t_top, t_bottom, t_leftleft, t_rightright, t_toptop, t_bottombottom;
						
		
		if(kk == 7)
		{
			if(ii == 0) t_left = 0;
			else t_left = buffer[jj*dx + ii - 1]&0x01;
		}
		else t_left = ((buffer[jj*dx + ii]&0x000000ff)>>(kk+1))&0x01;

		
		if(kk == 0)
		{
			if(ii == (dx-1)) t_right = 0;
			else t_right = buffer[jj*dx + ii + 1]&0x80;
		}
		else t_right = ((buffer[jj*dx + ii]&0x000000ff)>>(kk-1))&0x01;

		
		if(jj == 0) t_top = 0;
		else t_top = ((buffer[(jj-1)*dx + ii]&0x000000ff)>>kk)&0x01;
					
		
		if(jj == (cy-1)) t_bottom = 0;
		else t_bottom = ((buffer[(jj+1)*dx + ii]&0x000000ff)>>kk)&0x01;

		
		if(kk == 6)
		{
			if(ii == 0) t_leftleft = 0;
			else t_leftleft = buffer[jj*dx + ii - 1]&0x01;
		}
		else if(kk == 7)
		{
			if(ii == 0) t_leftleft = 0;
			else t_leftleft = buffer[jj*dx + ii - 1]&0x02;
		}
		else t_leftleft = ((buffer[jj*dx + ii]&0x000000ff)>>(kk+2))&0x01;

						
		
		if(kk == 1)
		{
			if(ii == (dx-1)) t_rightright = 0;
			else t_rightright = buffer[jj*dx + ii + 1]&0x80;
		}
		else if(kk == 0)
		{
			if(ii == (dx-1)) t_rightright = 0;
			else t_rightright = buffer[jj*dx + ii + 1]&0x40;
		}
		else t_rightright = ((buffer[jj*dx + ii]&0x000000ff)>>(kk-2))&0x01;

		
		if((jj == 0)||(jj == 1)) t_toptop = 0;
		else t_toptop = ((buffer[(jj-2)*dx + ii]&0x000000ff)>>kk)&0x01;

		
		if( (jj == (cy-1)) || (jj == (cy-2)) ) t_bottombottom = 0;
		else t_bottombottom = ((buffer[(jj+2)*dx + ii]&0x000000ff)>>kk)&0x01;

		if( (t_left > 0) || (t_right > 0) || (t_top > 0) || (t_bottom > 0) || (t_leftleft > 0) || (t_rightright > 0) || (t_toptop > 0) || (t_bottombottom > 0)) return TRUE;
		else return FALSE;
	}

	return FALSE;
}

BOOL CESDView::IsInArcSweepRange(Struct_VertexOutline vt, double fx, double fy, double *fangle)
{
	double alpha;
	if(fx == vt.cx)
	{
		if(fy > vt.cy) alpha = 90.0f;
		else alpha = 270.0f;
	}
	else if(fy == vt.cy) 
	{
		if(fx > vt.cx) alpha = 0;
		else alpha = 180;
	}
	else
	{
		alpha = 180/fEDA_pi*atan2((fy - vt.cy), (fx - vt.cx));
	}

	if(alpha < 0) alpha += 360;
	*fangle = alpha;

	if(abs(vt.sangle - vt.eangle) == 360) return TRUE;
	else if((alpha > vt.sangle)&&(alpha < vt.eangle))
	{
		return TRUE;
	}
	else if(((alpha + 360) > vt.sangle)&&((alpha + 360) < vt.eangle)) 
	{
		*fangle += 360;
		return TRUE;
	}

	return FALSE;
}

double CESDView::GetRegionArea(COb_pcbregion* pregion)
{
	
	double area = 0;
	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex vertex1, vertex2;
		vertex1 = pregion->m_cVertex.GetAt(i);
		if(i ==(pregion->m_cVertex.GetCount() - 1)) vertex2 =  pregion->m_cVertex.GetAt(0);
		else vertex2 =  pregion->m_cVertex.GetAt(i + 1);

		area += vertex1.fx*vertex2.fy - vertex1.fy*vertex2.fx;
	}

	area = 0.5*fabs(area);
	return area;
}

BOOL CESDView::GetRegionFillLine(COb_pcbregion* pregion, CArray<Struct_VertexOutline, Struct_VertexOutline&>* pFillLine, double fTrackWidth, double fTrackGap, int nHatchStyle)	
{
	double fminx, fminy, fmaxx, fmaxy;
	fminx = fminy = 100000;
	fmaxx = fmaxy = -100000;
	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex vertex;
		vertex = pregion->m_cVertex.GetAt(i);
		if(fminx > vertex.fx) fminx = vertex.fx;
		if(fminy > vertex.fy) fminy = vertex.fy;
		if(fmaxx < vertex.fx) fmaxx = vertex.fx;
		if(fmaxy < vertex.fy) fmaxy = vertex.fy;
	}

	double d = fTrackWidth/2 + fTrackGap + 0.000001324354670f;
	
	if((nHatchStyle == PCB_HATCH_90)||(nHatchStyle == PCB_HATCH_V))
	{
		for(double x = fminx + d; x < fmaxx; x+=d)
		{
			CArray<Fpoint, Fpoint&> arrayPoint;
			for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
			{
				Struct_RegionVertex vertex1, vertex2;
				vertex1 = pregion->m_cVertex.GetAt(i);
				if(i == (pregion->m_cVertex.GetCount() - 1)) vertex2 = pregion->m_cVertex.GetAt(0);
				else vertex2 = pregion->m_cVertex.GetAt(i+1);
				
				if(vertex1.fx == vertex2.fx) continue;
				if((MAX(vertex1.fx, vertex2.fx) < x)||(MIN(vertex1.fx, vertex2.fx) > x)) continue;

				double k = (vertex1.fy - vertex2.fy)/(vertex1.fx - vertex2.fx);
				Fpoint fp;
				fp.fx = x;
				fp.fy = k*(fp.fx - vertex1.fx) + vertex1.fy;
				arrayPoint.Add(fp);
			}
			
			for(int i=0; i<pregion->m_arrayHoles.GetCount(); i++)
			{
				CArray<Fpoint, Fpoint>* p_arrayHole = pregion->m_arrayHoles.GetAt(i);
				for(int j=0; j<p_arrayHole->GetCount(); j++)
				{
					Fpoint fp1, fp2;
					fp1 = p_arrayHole->GetAt(j);
					if(j == (p_arrayHole->GetCount() - 1)) fp2 = p_arrayHole->GetAt(0);
					else fp2 = p_arrayHole->GetAt(j+1);

					if(fp1.fx == fp2.fx) continue;
					if((MAX(fp1.fx, fp2.fx) < x)||(MIN(fp1.fx, fp2.fx) > x)) continue;

					double k = (fp1.fy - fp2.fy)/(fp1.fx - fp2.fx);
					Fpoint fp;
					fp.fx = x;
					fp.fy = k*(fp.fx - fp1.fx) + fp1.fy;
					arrayPoint.Add(fp);
				}
			}

			int judge = 1;
			for(;;)
			{
				if(judge == 1) judge = 0;
				else if(judge == 0) break;
				for(int i=0; i<arrayPoint.GetCount()-1;i++)
				{
					Fpoint fp1 = arrayPoint.GetAt(i);
					Fpoint fp2 = arrayPoint.GetAt(i+1);
					if(fp1.fy > fp2.fy)
					{
						judge = 1;
						arrayPoint.SetAt(i, fp2);
						arrayPoint.SetAt(i+1, fp1);
					}
				}
			}

			if( (arrayPoint.GetCount()%2) == 0)	
			{
				for(int i=0; i<arrayPoint.GetCount()/2; i++)
				{
					Fpoint fp1, fp2;
					fp1 = arrayPoint.GetAt(2*i);
					fp2 = arrayPoint.GetAt(2*i+1);
					
					Struct_VertexOutline vt;
					vt.kind = 0;
					vt.x1 = fp1.fx;
					vt.y1 = fp1.fy;
					vt.x2 = fp2.fx;
					vt.y2 = fp2.fy;

					vt.r = vt.cx = vt.cy = vt.sangle = vt.eangle = 0;
					pFillLine->Add(vt);
				}
			}
		}
	}
	
	if((nHatchStyle == PCB_HATCH_90)||(nHatchStyle == PCB_HATCH_H))
	{
		for(double y = fminy + d; y < fmaxy; y+=d)
		{
			CArray<Fpoint, Fpoint&> arrayPoint;
			for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
			{
				Struct_RegionVertex vertex1, vertex2;
				vertex1 = pregion->m_cVertex.GetAt(i);
				if(i == (pregion->m_cVertex.GetCount() - 1)) vertex2 = pregion->m_cVertex.GetAt(0);
				else vertex2 = pregion->m_cVertex.GetAt(i+1);
				
				if(vertex1.fy == vertex2.fy) continue;
				if((MAX(vertex1.fy, vertex2.fy) < y)||(MIN(vertex1.fy, vertex2.fy) > y)) continue;

				
				double invert_k = (vertex1.fx - vertex2.fx)/(vertex1.fy - vertex2.fy);
				Fpoint fp;
				fp.fx = invert_k*(y - vertex1.fy) + vertex1.fx;
				fp.fy = y; 
				arrayPoint.Add(fp);
			}
			
			for(int i=0; i<pregion->m_arrayHoles.GetCount(); i++)
			{
				CArray<Fpoint, Fpoint>* p_arrayHole = pregion->m_arrayHoles.GetAt(i);
				for(int j=0; j<p_arrayHole->GetCount(); j++)
				{
					Fpoint fp1, fp2;
					fp1 = p_arrayHole->GetAt(j);
					if(j == (p_arrayHole->GetCount() - 1)) fp2 = p_arrayHole->GetAt(0);
					else fp2 = p_arrayHole->GetAt(j+1);

					if(fp1.fy == fp2.fy) continue;
					if((MAX(fp1.fy, fp2.fy) < y)||(MIN(fp1.fy, fp2.fy) > y)) continue;

					double invert_k = (fp1.fx - fp2.fx)/(fp1.fy - fp2.fy);
					Fpoint fp;
					fp.fx = invert_k*(y - fp1.fy) + fp1.fx;
					fp.fy = y;
					arrayPoint.Add(fp);
				}
			}

			int judge = 1;
			for(;;)
			{
				if(judge == 1) judge = 0;
				else if(judge == 0) break;
				for(int i=0; i<arrayPoint.GetCount()-1;i++)
				{
					Fpoint fp1 = arrayPoint.GetAt(i);
					Fpoint fp2 = arrayPoint.GetAt(i+1);
					if(fp1.fx > fp2.fx)
					{
						judge = 1;
						arrayPoint.SetAt(i, fp2);
						arrayPoint.SetAt(i+1, fp1);
					}
				}
			}

			if( (arrayPoint.GetCount()%2) == 0)	
			{
				for(int i=0; i<arrayPoint.GetCount()/2; i++)
				{
					Fpoint fp1, fp2;
					fp1 = arrayPoint.GetAt(2*i);
					fp2 = arrayPoint.GetAt(2*i+1);
					
					Struct_VertexOutline vt;
					vt.kind = 0;
					vt.x1 = fp1.fx;
					vt.y1 = fp1.fy;
					vt.x2 = fp2.fx;
					vt.y2 = fp2.fy;

					vt.r = vt.cx = vt.cy = vt.sangle = vt.eangle = 0;
					pFillLine->Add(vt);
				}
			}
		}
	}
	
	if(nHatchStyle == PCB_HATCH_45)
	{
	}

	return TRUE;
}

void CESDView::CleanVertex(CArray<Fpoint, Fpoint&>& arrayVertex)
{
	
	for(int i=0; i<arrayVertex.GetCount(); i++)
	{
		Fpoint fp1, fp2;
		fp1 = arrayVertex.GetAt(i);
		if(i == (arrayVertex.GetCount() - 1)) fp2 = arrayVertex.GetAt(0);
		else fp2 = arrayVertex.GetAt(i+1);

		if(sqrt((fp1.fx - fp2.fx)*(fp1.fx - fp2.fx) + (fp1.fy - fp2.fy)*(fp1.fy - fp2.fy)) < 1)
		{
			arrayVertex.RemoveAt(i);
			i--;
		}
	}
}

void CESDView::OnPcbToolTear()
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	CDlg_PcbTearDrop dlg;

	if(dlg.DoModal() == IDOK)
	{
		
		if(dlg.m_bCheckAllPad == TRUE)
		{
			for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
				if(ppad->m_nFlag != FLAG_NORMAL) continue;
				
				if(dlg.m_nRadioAction == 0)
				{
					
					if(ppad->TearDropExist(this) == TRUE) continue;
					
					for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
					{
						COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
						if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_bTearDrop == TRUE)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;

						if((ppad->m_nLayer != MULTILAYER)&&(ppad->m_nLayer != ptrack->m_nLayer)) continue;

						if(Line_Pad_Connect(ptrack, ppad, 0) == TRUE)
						{
							double r = MIN(ppad->m_fSizeX[TOPLAYER], ppad->m_fSizeY[TOPLAYER])/2;
							double track_length = SQRT(ptrack->m_fX1, ptrack->m_fY1, ptrack->m_fX2, ptrack->m_fY2);
							if( track_length > r)
							{
								int delta = (int)(track_length/r);
								if(ppad->Is_On_Me(ptrack->m_fX1, ptrack->m_fY1) == TRUE)
								{
									
									if(delta < 2)
									{
										double fx, fy;
										fx = ptrack->m_fX2;
										fy = ptrack->m_fY2;
										if(ppad->Is_On_Me(fx, fy) != TRUE)
										{
											clear_undo_buffer_flag = GreatePadTearDrop(fx, fy, ppad, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
											undo_flag = clear_undo_buffer_flag;
										}
									}
									else 
									{
										for(int k=2; k<=delta; k++)
										{
											double fx, fy;
											fx = ptrack->m_fX1 + (ptrack->m_fX2 - ptrack->m_fX1)*k/delta;
											fy = ptrack->m_fY1 + (ptrack->m_fY2 - ptrack->m_fY1)*k/delta;
											if(ppad->Is_On_Me(fx, fy) != TRUE)
											{
												clear_undo_buffer_flag = GreatePadTearDrop(fx, fy, ppad, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
												undo_flag = clear_undo_buffer_flag;
												break;
											}
										}
									}
								}
								else if(ppad->Is_On_Me(ptrack->m_fX2, ptrack->m_fY2) == TRUE)
								{
									
									if(delta < 2)
									{
										double fx, fy;
										fx = ptrack->m_fX1;
										fy = ptrack->m_fY1;
										if(ppad->Is_On_Me(fx, fy) != TRUE)
										{
											clear_undo_buffer_flag = GreatePadTearDrop(fx, fy, ppad, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
											undo_flag = clear_undo_buffer_flag;
										}
									}
									else
									{
										for(int k=2; k<=delta; k++)
										{
											double fx, fy;
											fx = ptrack->m_fX2 + (ptrack->m_fX1 - ptrack->m_fX2)*k/delta;
											fy = ptrack->m_fY2 + (ptrack->m_fY1 - ptrack->m_fY2)*k/delta;
											if(ppad->Is_On_Me(fx, fy) != TRUE)
											{
												clear_undo_buffer_flag = GreatePadTearDrop(fx, fy, ppad, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
												undo_flag = clear_undo_buffer_flag;
												break;
											}
										}
									}
								}
								else
								{
									

								}
							}
						}
					}
				}
				else
				{
					
					for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
					{
						COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
						if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_bTearDrop == FALSE)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;

						if((ppad->m_nLayer != MULTILAYER)&&(ppad->m_nLayer != ptrack->m_nLayer)) continue;

						if(Line_Pad_Connect(ptrack, ppad, 0) == TRUE)
						{
							
							if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_UNDELETE;
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = j;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							ptrack->m_nFlag = FLAG_DELETED;
						}
					}
					
					for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
					{
						COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
						if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_bTearDrop == FALSE)||(parc->m_nComponent >= 0)||(parc->m_nPolygon >= 0)) continue;

						if((ppad->m_nLayer != MULTILAYER)&&(ppad->m_nLayer != parc->m_nLayer)) continue;

						if(Pad_Arc_Connect(ppad, parc, 0) == TRUE)
						{
							
							if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_UNDELETE;
							pundo->m_nOb = PCB_ELEMENT_ARC;
							pundo->m_nIndex = j;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							parc->m_nFlag = FLAG_DELETED;
						}
					}

				}
			}
		}
		else if(dlg.m_bCheckSelection == TRUE)
		{

			for(int i=0; i<pDoc->m_arrayPcbSelection.GetCount(); i++)
			{
				Struct_Pcb_Multi_Selection selection = pDoc->m_arrayPcbSelection.GetAt(i);

				if(selection.pcbelement != PCB_ELEMENT_PAD) continue;

				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selection.index);
				if(ppad->m_nFlag != FLAG_NORMAL) continue;

				if(dlg.m_nRadioAction == 0)
				{
					
					if(ppad->TearDropExist(this) == TRUE) continue;
					
					for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
					{
						COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
						if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_bTearDrop == TRUE)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;

						if((ppad->m_nLayer != MULTILAYER)&&(ppad->m_nLayer != ptrack->m_nLayer)) continue;

						if(Line_Pad_Connect(ptrack, ppad, 0) == TRUE)
						{
							double r = MIN(ppad->m_fSizeX[TOPLAYER], ppad->m_fSizeY[TOPLAYER])/2;
							double track_length = SQRT(ptrack->m_fX1, ptrack->m_fY1, ptrack->m_fX2, ptrack->m_fY2);
							
							if( track_length > r)
							{
								int delta = (int)(track_length/r);
								if(ppad->Is_On_Me(ptrack->m_fX1, ptrack->m_fY1) == TRUE)
								{
									if(delta < 2)
									{
										double fx, fy;
										fx = ptrack->m_fX2;
										fy = ptrack->m_fY2;
										if(ppad->Is_On_Me(fx, fy) != TRUE)
										{
											clear_undo_buffer_flag = GreatePadTearDrop(fx, fy, ppad, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
											undo_flag = clear_undo_buffer_flag;
										}
									}
									else
									{
										for(int k=2; k<=delta; k++)
										{
											double fx, fy;
											fx = ptrack->m_fX1 + (ptrack->m_fX2 - ptrack->m_fX1)*k/delta;
											fy = ptrack->m_fY1 + (ptrack->m_fY2 - ptrack->m_fY1)*k/delta;
											if(ppad->Is_On_Me(fx, fy) != TRUE)
											{
												clear_undo_buffer_flag = GreatePadTearDrop(fx, fy, ppad, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
												undo_flag = clear_undo_buffer_flag;
												break;
											}
										}
									}
								}
								else if(ppad->Is_On_Me(ptrack->m_fX2, ptrack->m_fY2) == TRUE)
								{
									if(delta < 2)
									{
										double fx, fy;
										fx = ptrack->m_fX1;
										fy = ptrack->m_fY1;
										if(ppad->Is_On_Me(fx, fy) != TRUE)
										{
											clear_undo_buffer_flag = GreatePadTearDrop(fx, fy, ppad, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
											undo_flag = clear_undo_buffer_flag;
										}
									}
									else
									{
										for(int k=2; k<=delta; k++)
										{
											double fx, fy;
											fx = ptrack->m_fX2 + (ptrack->m_fX1 - ptrack->m_fX2)*k/delta;
											fy = ptrack->m_fY2 + (ptrack->m_fY1 - ptrack->m_fY2)*k/delta;
											if(ppad->Is_On_Me(fx, fy) != TRUE)
											{
												clear_undo_buffer_flag = GreatePadTearDrop(fx, fy, ppad, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
												undo_flag = clear_undo_buffer_flag;
												break;
											}
										}
									}
								}
								else
								{
									

								}
							}
						}
					}
				}
				else
				{
					
					for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
					{
						COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
						if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_bTearDrop == FALSE)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;

						if((ppad->m_nLayer != MULTILAYER)&&(ppad->m_nLayer != ptrack->m_nLayer)) continue;

						if(Line_Pad_Connect(ptrack, ppad, 0) == TRUE)
						{
							
							if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_UNDELETE;
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = j;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;

							ptrack->m_nFlag = FLAG_DELETED;
						}
					}
					
					for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
					{
						COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
						if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_bTearDrop == FALSE)||(parc->m_nComponent >= 0)||(parc->m_nPolygon >= 0)) continue;

						if((ppad->m_nLayer != MULTILAYER)&&(ppad->m_nLayer != parc->m_nLayer)) continue;

						if(Pad_Arc_Connect(ppad, parc, 0) == TRUE)
						{
							
							if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_UNDELETE;
							pundo->m_nOb = PCB_ELEMENT_ARC;
							pundo->m_nIndex = j;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							parc->m_nFlag = FLAG_DELETED;
						}
					}

				}
			}
		}

		
		if(dlg.m_bCheckAllVia == TRUE)
		{
			for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
				if(pvia->m_nFlag != FLAG_NORMAL) continue;
				
				if(dlg.m_nRadioAction == 0)
				{
					for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
					{
						COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
						if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_bTearDrop == TRUE)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;

						if((pDoc->IsConnectedToVia(ptrack->m_nLayer, pvia) == TRUE)&&(Line_Via_Connect(ptrack, pvia, 0) == TRUE))
						{
							double r = pvia->m_fDia/2;
							double track_length = SQRT(ptrack->m_fX1, ptrack->m_fY1, ptrack->m_fX2, ptrack->m_fY2);

							if( track_length > r)
							{
								int delta = (int)(track_length/r);
								if(pvia->Is_On_Me(ptrack->m_fX1, ptrack->m_fY1) == TRUE)
								{
									if(delta < 2)
									{
										double fx, fy;
										fx = ptrack->m_fX2;
										fy = ptrack->m_fY2;
										if(pvia->Is_On_Me(fx, fy) != TRUE)
										{
											clear_undo_buffer_flag = GreateViaTearDrop(fx, fy, pvia, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
											undo_flag = clear_undo_buffer_flag;
										}
									}
									else
									{
										for(int k=2; k<=delta; k++)
										{
											double fx, fy;
											fx = ptrack->m_fX1 + (ptrack->m_fX2 - ptrack->m_fX1)*k/delta;
											fy = ptrack->m_fY1 + (ptrack->m_fY2 - ptrack->m_fY1)*k/delta;
											if(pvia->Is_On_Me(fx, fy) != TRUE)
											{
												clear_undo_buffer_flag = GreateViaTearDrop(fx, fy, pvia, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
												undo_flag = clear_undo_buffer_flag;
												break;
											}
										}
									}
								}
								else if(pvia->Is_On_Me(ptrack->m_fX2, ptrack->m_fY2) == TRUE)
								{
									if(delta < 2)
									{
										double fx, fy;
										fx = ptrack->m_fX1;
										fy = ptrack->m_fY1;
										if(pvia->Is_On_Me(fx, fy) != TRUE)
										{
											clear_undo_buffer_flag = GreateViaTearDrop(fx, fy, pvia, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
											undo_flag = clear_undo_buffer_flag;
										}
									}
									else
									{
										for(int k=2; k<=delta; k++)
										{
											double fx, fy;
											fx = ptrack->m_fX2 + (ptrack->m_fX1 - ptrack->m_fX2)*k/delta;
											fy = ptrack->m_fY2 + (ptrack->m_fY1 - ptrack->m_fY2)*k/delta;
											if(pvia->Is_On_Me(fx, fy) != TRUE)
											{
												clear_undo_buffer_flag = GreateViaTearDrop(fx, fy, pvia, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
												undo_flag = clear_undo_buffer_flag;
												break;
											}
										}
									}
								}
								else
								{

								}
							}
						}
					}
				}
				else
				{
					for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
					{
						COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
						if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_bTearDrop == FALSE)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;

						if((pDoc->IsConnectedToVia(ptrack->m_nLayer, pvia) == TRUE)&&(Line_Via_Connect(ptrack, pvia, 0) == TRUE))
						{
							if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_UNDELETE;
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = j;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;

							ptrack->m_nFlag = FLAG_DELETED;
						}
					}
					
					for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
					{
						COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
						if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_bTearDrop == FALSE)||(parc->m_nComponent >= 0)||(parc->m_nPolygon >= 0)) continue;

						if((pDoc->IsConnectedToVia(parc->m_nLayer, pvia) == TRUE)&&(Via_Arc_Connect(pvia, parc, 0) == TRUE))
						{

							if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_UNDELETE;
							pundo->m_nOb = PCB_ELEMENT_ARC;
							pundo->m_nIndex = j;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							parc->m_nFlag = FLAG_DELETED;
						}
					}

				}
			}
		}
		else if(dlg.m_bCheckSelection == TRUE)
		{
			for(int i=0; i<pDoc->m_arrayPcbSelection.GetCount(); i++)
			{
				Struct_Pcb_Multi_Selection selection = pDoc->m_arrayPcbSelection.GetAt(i);
				if(selection.pcbelement != PCB_ELEMENT_VIA) continue;

				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selection.index);
				if(pvia->m_nFlag != FLAG_NORMAL) continue;

				if(dlg.m_nRadioAction == 0)
				{
					
					
					for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
					{
						COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
						if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_bTearDrop == TRUE)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;
						
						if((pDoc->IsConnectedToVia(ptrack->m_nLayer, pvia) == TRUE)&&(Line_Via_Connect(ptrack, pvia, 0) == TRUE))
						{
							double r = pvia->m_fDia/2;
							double track_length = SQRT(ptrack->m_fX1, ptrack->m_fY1, ptrack->m_fX2, ptrack->m_fY2);
							
							if( track_length > r)
							{
								int delta = (int)(track_length/r);
								if(pvia->Is_On_Me(ptrack->m_fX1, ptrack->m_fY1) == TRUE)
								{
									if(delta < 2)
									{
										double fx, fy;
										fx = ptrack->m_fX2;
										fy = ptrack->m_fY2;
										if(pvia->Is_On_Me(fx, fy) != TRUE)
										{
											clear_undo_buffer_flag = GreateViaTearDrop(fx, fy, pvia, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
											undo_flag = clear_undo_buffer_flag;
										}
									}
									else
									{
										for(int k=2; k<=delta; k++)
										{
											double fx, fy;
											fx = ptrack->m_fX1 + (ptrack->m_fX2 - ptrack->m_fX1)*k/delta;
											fy = ptrack->m_fY1 + (ptrack->m_fY2 - ptrack->m_fY1)*k/delta;
											if(pvia->Is_On_Me(fx, fy) != TRUE)
											{
												clear_undo_buffer_flag = GreateViaTearDrop(fx, fy, pvia, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
												undo_flag = clear_undo_buffer_flag;
												break;
											}
										}
									}
								}
								else if(pvia->Is_On_Me(ptrack->m_fX2, ptrack->m_fY2) == TRUE)
								{
									if(delta < 2)
									{
										double fx, fy;
										fx = ptrack->m_fX1;
										fy = ptrack->m_fY1;
										if(pvia->Is_On_Me(fx, fy) != TRUE)
										{
											clear_undo_buffer_flag = GreateViaTearDrop(fx, fy, pvia, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
											undo_flag = clear_undo_buffer_flag;
										}
									}
									else
									{
										for(int k=2; k<=delta; k++)
										{
											double fx, fy;
											fx = ptrack->m_fX2 + (ptrack->m_fX1 - ptrack->m_fX2)*k/delta;
											fy = ptrack->m_fY2 + (ptrack->m_fY1 - ptrack->m_fY2)*k/delta;
											if(pvia->Is_On_Me(fx, fy) != TRUE)
											{
												clear_undo_buffer_flag = GreateViaTearDrop(fx, fy, pvia, ptrack, clear_undo_buffer_flag, dlg.m_nRadioSize, pDoc);
												undo_flag = clear_undo_buffer_flag;
												break;
											}
										}
									}
								}
								else
								{
									

								}
							}
						}
					}
				}
				else
				{
					
					for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
					{
						COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
						if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_bTearDrop == FALSE)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;

						if((pDoc->IsConnectedToVia(ptrack->m_nLayer, pvia) == TRUE)&&(Line_Via_Connect(ptrack, pvia, 0) == TRUE))
						{
							
							if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_UNDELETE;
							pundo->m_nOb = PCB_ELEMENT_TRACK;
							pundo->m_nIndex = j;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							ptrack->m_nFlag = FLAG_DELETED;
						}
					}
					
					for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
					{
						COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
						if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_bTearDrop == FALSE)||(parc->m_nComponent >= 0)||(parc->m_nPolygon >= 0)) continue;

						if((pDoc->IsConnectedToVia(parc->m_nLayer, pvia) == TRUE)&&(Via_Arc_Connect(pvia, parc, 0) == TRUE))
						{
							
							if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
							{
								pDoc->Clear_Discarded_Undo_Buffer();
							}
							clear_undo_buffer_flag = TRUE;
							CPcbUndo *pundo = new CPcbUndo;
							pundo->m_nOp = UNDO_UNDELETE;
							pundo->m_nOb = PCB_ELEMENT_ARC;
							pundo->m_nIndex = j;
							pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
							undo_flag = TRUE;
							

							parc->m_nFlag = FLAG_DELETED;
						}
					}

				}
			}
		}
	}
	
	
	if(undo_flag == TRUE)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
}

BOOL CESDView::GreateViaTearDrop(double fx, double fy, COb_pcbvia* pvia, COb_pcbtrack* ptrack, BOOL clear_undo_buffer_flag, int nRadioSize, CESDDoc* pDoc)
{
	
	CClientDC dc(this);

	double d_angle = 12;  
	double l = SQRT(fx, fy, pvia->m_fCx, pvia->m_fCy);
	double alpha;
	alpha = atan2((pvia->m_fCy - fy), (pvia->m_fCx - fx));
	double fx1, fy1, fx2, fy2;
	fx1 = fx + l*cos(alpha + d_angle*fEDA_pi/180);
	fy1 = fy + l*sin(alpha + d_angle*fEDA_pi/180);
	fx2 = fx + l*cos(alpha - d_angle*fEDA_pi/180);
	fy2 = fy + l*sin(alpha - d_angle*fEDA_pi/180);
	
	if(pvia->Is_On_Me(fx1, fy1) == TRUE)
	{
		
		COb_pcbtrack* pnewtrack = new COb_pcbtrack();
		pnewtrack->pDocument = pDoc;
		pnewtrack->m_nLayer = ptrack->m_nLayer;
		pnewtrack->m_fX1 = fx;
		pnewtrack->m_fY1 = fy;
		pnewtrack->m_fX2 = fx1;
		pnewtrack->m_fY2 = fy1;
		pnewtrack->m_fWidth = ptrack->m_fWidth;
		pnewtrack->m_bTearDrop = TRUE;
		int index = pDoc->m_arrayPcbTrack.Add(pnewtrack);
		
		
		if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_DELETE;
		pundo->m_nOb = PCB_ELEMENT_TRACK;
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		
		pnewtrack->Draw(&dc, this);											}
	
	if(pvia->Is_On_Me(fx2, fy2) == TRUE)
	{
		
		COb_pcbtrack* pnewtrack = new COb_pcbtrack();
		pnewtrack->pDocument = pDoc;
		pnewtrack->m_nLayer = ptrack->m_nLayer;
		pnewtrack->m_fX1 = fx;
		pnewtrack->m_fY1 = fy;
		pnewtrack->m_fX2 = fx2;
		pnewtrack->m_fY2 = fy2;
		pnewtrack->m_fWidth = ptrack->m_fWidth;
		pnewtrack->m_bTearDrop = TRUE;
		int index = pDoc->m_arrayPcbTrack.Add(pnewtrack);
		
		
		if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_DELETE;
		pundo->m_nOb = PCB_ELEMENT_TRACK;
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		
		pnewtrack->Draw(&dc, this);
	}
		
	if(nRadioSize == 0)
	{
		
		double d_angle = 20;  
		double l = SQRT(fx, fy, pvia->m_fCx, pvia->m_fCy);
		double alpha;
		alpha = atan2((pvia->m_fCy - fy), (pvia->m_fCx - fx));
		double fx1, fy1, fx2, fy2;
		fx1 = fx + l*cos(alpha + d_angle*fEDA_pi/180);
		fy1 = fy + l*sin(alpha + d_angle*fEDA_pi/180);
		fx2 = fx + l*cos(alpha - d_angle*fEDA_pi/180);
		fy2 = fy + l*sin(alpha - d_angle*fEDA_pi/180);
			
		if(pvia->Is_On_Me(fx1, fy1) == TRUE)
		{
			
			COb_pcbtrack* pnewtrack = new COb_pcbtrack();
			pnewtrack->pDocument = pDoc;
			pnewtrack->m_nLayer = ptrack->m_nLayer;
			pnewtrack->m_fX1 = fx;
			pnewtrack->m_fY1 = fy;
			pnewtrack->m_fX2 = fx1;
			pnewtrack->m_fY2 = fy1;
			pnewtrack->m_fWidth = ptrack->m_fWidth;
			pnewtrack->m_bTearDrop = TRUE;
			int index = pDoc->m_arrayPcbTrack.Add(pnewtrack);
				
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_TRACK;
			pundo->m_nIndex = index;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			
			pnewtrack->Draw(&dc, this);
		}
			
		if(pvia->Is_On_Me(fx2, fy2) == TRUE)
		{
			
			COb_pcbtrack* pnewtrack = new COb_pcbtrack();
			pnewtrack->pDocument = pDoc;
			pnewtrack->m_nLayer = ptrack->m_nLayer;
			pnewtrack->m_fX1 = fx;
			pnewtrack->m_fY1 = fy;
			pnewtrack->m_fX2 = fx2;
			pnewtrack->m_fY2 = fy2;
			pnewtrack->m_fWidth = ptrack->m_fWidth;
			pnewtrack->m_bTearDrop = TRUE;
			int index = pDoc->m_arrayPcbTrack.Add(pnewtrack);
				
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_TRACK;
			pundo->m_nIndex = index;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			
			pnewtrack->Draw(&dc, this);
		}
	}

	return clear_undo_buffer_flag;
}


BOOL CESDView::GreatePadTearDrop(double fx, double fy, COb_pcbpad* ppad, COb_pcbtrack* ptrack, BOOL clear_undo_buffer_flag, int nRadioSize, CESDDoc* pDoc)
{
	
	CClientDC dc(this);

	double d_angle = 12;  
	double l = SQRT(fx, fy, ppad->m_fCx, ppad->m_fCy);
	double alpha;
	alpha = atan2((ppad->m_fCy - fy), (ppad->m_fCx - fx));
	double fx1, fy1, fx2, fy2;
	fx1 = fx + l*cos(alpha + d_angle*fEDA_pi/180);
	fy1 = fy + l*sin(alpha + d_angle*fEDA_pi/180);
	fx2 = fx + l*cos(alpha - d_angle*fEDA_pi/180);
	fy2 = fy + l*sin(alpha - d_angle*fEDA_pi/180);
	
	if(ppad->Is_On_Me(fx1, fy1) == TRUE)
	{
		
		COb_pcbtrack* pnewtrack = new COb_pcbtrack();
		pnewtrack->pDocument = pDoc;
		pnewtrack->m_nLayer = ptrack->m_nLayer;
		pnewtrack->m_fX1 = fx;
		pnewtrack->m_fY1 = fy;
		pnewtrack->m_fX2 = fx1;
		pnewtrack->m_fY2 = fy1;
		pnewtrack->m_fWidth = ptrack->m_fWidth;
		pnewtrack->m_bTearDrop = TRUE;
		int index = pDoc->m_arrayPcbTrack.Add(pnewtrack);
		
		
		if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_DELETE;
		pundo->m_nOb = PCB_ELEMENT_TRACK;
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		
		pnewtrack->Draw(&dc, this);											}
	
	if(ppad->Is_On_Me(fx2, fy2) == TRUE)
	{
		
		COb_pcbtrack* pnewtrack = new COb_pcbtrack();
		pnewtrack->pDocument = pDoc;
		pnewtrack->m_nLayer = ptrack->m_nLayer;
		pnewtrack->m_fX1 = fx;
		pnewtrack->m_fY1 = fy;
		pnewtrack->m_fX2 = fx2;
		pnewtrack->m_fY2 = fy2;
		pnewtrack->m_fWidth = ptrack->m_fWidth;
		pnewtrack->m_bTearDrop = TRUE;
		int index = pDoc->m_arrayPcbTrack.Add(pnewtrack);
		
		
		if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		clear_undo_buffer_flag = TRUE;
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_DELETE;
		pundo->m_nOb = PCB_ELEMENT_TRACK;
		pundo->m_nIndex = index;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		
		pnewtrack->Draw(&dc, this);
	}
		
	if(nRadioSize == 0)
	{
		
		double d_angle = 20;  
		double l = SQRT(fx, fy, ppad->m_fCx, ppad->m_fCy);
		double alpha;
		alpha = atan2((ppad->m_fCy - fy), (ppad->m_fCx - fx));
		double fx1, fy1, fx2, fy2;
		fx1 = fx + l*cos(alpha + d_angle*fEDA_pi/180);
		fy1 = fy + l*sin(alpha + d_angle*fEDA_pi/180);
		fx2 = fx + l*cos(alpha - d_angle*fEDA_pi/180);
		fy2 = fy + l*sin(alpha - d_angle*fEDA_pi/180);
			
		if(ppad->Is_On_Me(fx1, fy1) == TRUE)
		{
			
			COb_pcbtrack* pnewtrack = new COb_pcbtrack();
			pnewtrack->pDocument = pDoc;
			pnewtrack->m_nLayer = ptrack->m_nLayer;
			pnewtrack->m_fX1 = fx;
			pnewtrack->m_fY1 = fy;
			pnewtrack->m_fX2 = fx1;
			pnewtrack->m_fY2 = fy1;
			pnewtrack->m_fWidth = ptrack->m_fWidth;
			pnewtrack->m_bTearDrop = TRUE;
			int index = pDoc->m_arrayPcbTrack.Add(pnewtrack);
				
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_TRACK;
			pundo->m_nIndex = index;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			
			pnewtrack->Draw(&dc, this);
		}
			
		if(ppad->Is_On_Me(fx2, fy2) == TRUE)
		{
			
			COb_pcbtrack* pnewtrack = new COb_pcbtrack();
			pnewtrack->pDocument = pDoc;
			pnewtrack->m_nLayer = ptrack->m_nLayer;
			pnewtrack->m_fX1 = fx;
			pnewtrack->m_fY1 = fy;
			pnewtrack->m_fX2 = fx2;
			pnewtrack->m_fY2 = fy2;
			pnewtrack->m_fWidth = ptrack->m_fWidth;
			pnewtrack->m_bTearDrop = TRUE;
			int index = pDoc->m_arrayPcbTrack.Add(pnewtrack);
				
			
			if((pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_TRACK;
			pundo->m_nIndex = index;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			
			pnewtrack->Draw(&dc, this);
		}
	}

	return clear_undo_buffer_flag;
}

void CESDView::OnPcbSplitPlane()
{
	int ch1;
	double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4, tempx, tempy;
	int x1,y1,x2,y2,x3,y3,x4,y4,flag;
	double fcx, fcy, fradius, fsx, fsy, fex, fey, fsangle, feangle;
	int cx, cy, sx, sy, ex, ey, radius;
	int index;
	Struct_RegionVertex vertex;
	char szPlaceRegion[]  = "分割内平面: <左键><回车>定义顶点, <右键><ESC>结束命令";
	char szInfo[9][100] = {	"<shift+空格><空格>修改模式: 90度-先水平再垂直",	"<shift+空格><空格>修改模式: 90度-先垂直再水平",\
							"<shift+空格><空格>修改模式: 90度-先圆弧再连线",	"<shift+空格><空格>修改模式: 90度-先连线再圆弧",\
							"<shift+空格><空格>修改模式: 45度-先水平垂直再45度","<shift+空格><空格>修改模式: 45度-先45度再水平垂直",\
							"<shift+空格><空格>修改模式: 45度-先圆弧再连线",	"<shift+空格><空格>修改模式: 45度-先连线再圆弧",\
							"<shift+空格><空格>修改模式: 任意角度"};
	
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	if((m_nPCB_Current_Layer < POWERPLANE1)||(m_nPCB_Current_Layer > POWERPLANE16))
	{
		AfxMessageBox("分割只能定义在内平面层上。", MB_ICONWARNING);
		return;
	}
	
	pDoc->SetModifiedFlag(TRUE);

	MoveCursorToCenter();

	Write_Help(szPlaceRegion);

	int layer = m_nPCB_Current_Layer;
	//COLORREF color = (~nEDA_PCB_LayerColor[m_nPCB_Current_Layer]&0x00ffffff)^nEDA_PCB_BoardAreaColor;
	COLORREF color = nEDA_PCB_LayerColor[m_nPCB_Current_Layer];
	int drawmode = dc.GetROP2();
	dc.SetROP2(R2_NOTXORPEN);

	COb_pcbregion aregion;
	aregion.pDocument = pDoc;
	aregion.m_nLayer = layer;
	aregion.m_bPolygonCutout = TRUE;

	fradius = 0;
	
	flag = 1;
	nEDA_RedrawFlag = OFF;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			fx1 = m_fCurrentSnapX;
			fy1 = m_fCurrentSnapY;
			fx2 = fx3 = fx4 = fx1; 
			fy2 = fy3 = fy4 = fy1;

			vertex.fx = fx1;	vertex.fy = fy1;
			vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
			aregion.m_cVertex.Add(vertex);

			vertex.fx = fx2;	vertex.fy = fy2;
			vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
			aregion.m_cVertex.Add(vertex);

			vertex.fx = fx3;	vertex.fy = fy3;
			vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
			aregion.m_cVertex.Add(vertex);

			vertex.fx = fx4;	vertex.fy = fy4;
			vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
			aregion.m_cVertex.Add(vertex);

			Write_Info(szInfo[nEDA_PCB_PlaceTrackMode]);
			do
			{
				ch1 = MouseKey();
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{ 
					Write_Info(szEDA_Info_Ready);

					CloseMouseCursor();
					aregion.FrameRegion(&dc, this, 1, color);
					OpenMouseCursor();
	
					break;
				}
				else if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{ 
						flag = 1;

					}
					else
					{
						if(nEDA_RedrawFlag == ON)
						{ 
							nEDA_RedrawFlag = OFF;
						}
						else  
						{
							CloseMouseCursor();
							aregion.FrameRegion(&dc, this, 1, color);
							OpenMouseCursor();
						}


						fx4 = fx3 = fx2 = m_fCurrentSnapX; 
						fy4 = fy3 = fy2 = m_fCurrentSnapY;
						fcx = fcy = fradius = fsangle = feangle = 0;

						if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_START)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {} 
							else if(fx4 == fx1){} 
							else if(fy4 == fy1){}
							else
							{
								fy2 = fy3 = fy1;
							}
						}
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_END)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {} 
							else if(fx4 == fx1){} 
							else if(fy4 == fy1){}
							else
							{
								fx2 = fx3 = fx1;
							}
						}
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90ARC_START)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {} 
							else if(fx4 == fx1) {} 
							else if(fy4 == fy1) {}
							else
							{
								double deltax, deltay;
								deltax = fx4 - fx1;
								deltay = fy4 - fy1;
								//fx2 = fx1;	
								//fy2 = fy1;
								if( abs(deltax) > abs(deltay) )
								{
									fradius = abs(deltay);
									if(fradius <= fEDA_AutoArcMaxRadius)
									{
										fx2 = fx1;	
										fy2 = fy1;
										if(fx4 > fx1)
										{
											fcx = fx1 + fradius;
											fcy = fy1;
											if(fy4 > fy1)
											{
												fsangle = 90;	feangle = 180;
												fsx = fcx;		fsy = fy4;
												fex = fx2;		fey = fy2;
											}
											else
											{
												fsangle = 180; feangle = 270;
												fsx = fx2;		fsy = fy2;
												fex = fcx;		fey = fy4;
											}
										}
										else
										{
											fcx = fx1 - fradius;
											fcy = fy1;
											if(fy4 > fy1)
											{
												fsangle = 0; feangle = 90;
												fsx = fx2;  fsy = fy2;
												fex = fcx;  fey = fy4;
											}
											else
											{
												fsangle = 270; feangle = 360;
												fsx = fcx;		fsy = fy4;
												fex = fx2;		fey = fy2;
											}
										}
									}
									else
									{
										fradius = fEDA_AutoArcMaxRadius;
										fx2 = fx1;
										
										if(fx4 > fx1)
										{
											fcx = fx1 + fradius;
											if(fy4 > fy1)
											{
												fcy = fy4 - fradius;
												fy2 = fcy;
												fsangle = 90;	feangle = 180;
												fsx = fcx;		fsy = fy4;
												fex = fx2;		fey = fy2;
											}
											else
											{
												fcy = fy4 + fradius;
												fy2 = fcy;
												fsangle = 180; feangle = 270;
												fsx = fx2;		fsy = fy2;
												fex = fcx;		fey = fy4;
											}
										}
										else
										{
											fcx = fx1 - fradius;
											if(fy4 > fy1)
											{
												fcy = fy4 - fradius;
												fy2 = fcy;
												fsangle = 0; feangle = 90;
												fsx = fx2;  fsy = fy2;
												fex = fcx;  fey = fy4;
											}
											else
											{
												fcy = fy4 + fradius;
												fy2 = fcy;
												fsangle = 270; feangle = 360;
												fsx = fcx;		fsy = fy4;
												fex = fx2;		fey = fy2;
											}
										}

									}
									fx3 = fcx;	
									fy3 = fy4;
								}
								else
								{
									fradius = abs(deltax);
									if(fradius <= fEDA_AutoArcMaxRadius)
									{
										fx2 = fx1;	
										fy2 = fy1;
										if(fy4 > fy1)
										{
											fcx = fx1;
											fcy = fy1 + fradius;
											if(fx4 > fx1)
											{
												fsangle = 270; feangle = 360;
												fsx = fx2;	fsy = fy2;
												fex = fx4;	fey = fcy;
											}
											else
											{
												fsangle = 180; feangle = 270;
												fsx = fx4;	fsy = fcy;
												fex = fx2;	fey = fy2;
											}
										}
										else
										{
											fcx = fx1;
											fcy = fy1 - fradius;
											if(fx4 > fx1)
											{
												fsangle = 0; feangle = 90;
												fsx = fx4;	fsy = fcy;
												fex = fx2;	fey = fy2;
											}
											else
											{
												fsangle = 90; feangle = 180;
												fsx = fx2;	fsy = fy2;
												fex = fx4;	fey = fcy;
											}
										}
									}
									else
									{
										fradius = fEDA_AutoArcMaxRadius;
										fy2 = fy1;
										if(fy4 > fy1)
										{
											fcy = fy1 + fradius;
											if(fx4 > fx1)
											{
												fcx = fx4 - fradius;
												fx2 = fcx;
												fsangle = 270; feangle = 360;
												fsx = fx2;	fsy = fy2;
												fex = fx4;	fey = fcy;
											}
											else
											{
												fcx = fx4 + fradius;
												fx2 = fcx;
												fsangle = 180; feangle = 270;
												fsx = fx4;	fsy = fcy;
												fex = fx2;	fey = fy2;
											}
										}
										else
										{
											fcy = fy1 - fradius;
											if(fx4 > fx1)
											{
												fcx = fx4 - fradius;
												fx2 = fcx;
												fsangle = 0; feangle = 90;
												fsx = fx4;	fsy = fcy;
												fex = fx2;	fey = fy2;
											}
											else
											{
												fcx = fx4 + fradius;
												fx2 = fcx;
												fsangle = 90; feangle = 180;
												fsx = fx2;	fsy = fy2;
												fex = fx4;	fey = fcy;
											}
										}
									}
									fx3 = fx4;	
									fy3 = fcy;
								}
							}
						}
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90ARC_END)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {} 
							else if(fx4 == fx1) {} 
							else if(fy4 == fy1) {}
							else
							{
								double deltax, deltay;
								deltax = fx4 - fx1;
								deltay = fy4 - fy1;
								if( abs(deltax) > abs(deltay) )
								{
									fradius = abs(deltay);
									if(fradius <= fEDA_AutoArcMaxRadius)
									{
										fx3 = fx4;	
										fy3 = fy4;
										if(fx4 > fx1)
										{
											fcx = fx4 - fradius;
											fcy = fy4;
											if(fy4 > fy1)
											{
												fsangle = 270;	feangle = 360;
												fsx = fcx;		fsy = fy1;
												fex = fx3;		fey = fy3;
											}
											else
											{
												fsangle = 0; feangle = 90;
												fsx = fx3;		fsy = fy3;
												fex = fcx;		fey = fy1;
											}
										}
										else
										{
											fcx = fx4 + fradius;
											fcy = fy4;
											if(fy4 > fy1)
											{
												fsangle = 180; feangle = 270;
												fsx = fx3;  fsy = fy3;
												fex = fcx;  fey = fy1;
											}
											else
											{
												fsangle = 90; feangle = 180;
												fsx = fcx;		fsy = fy1;
												fex = fx3;		fey = fy3;
											}
										}
									}
									else
									{
										fradius = fEDA_AutoArcMaxRadius;
										fx3 = fx4;
										if(fx4 > fx1)
										{
											fcx = fx4 - fradius;
											if(fy4 > fy1)
											{
												fcy = fy1 + fradius;
												fy3 = fcy;
												fsangle = 270;	feangle = 360;
												fsx = fcx;		fsy = fy1;
												fex = fx3;		fey = fy3;
											}
											else
											{
												fcy = fy1 - fradius;
												fy3 = fcy;
												fsangle = 0; feangle = 90;
												fsx = fx3;		fsy = fy3;
												fex = fcx;		fey = fy1;
											}
										}
										else
										{
											fcx = fx4 + fradius;
											if(fy4 > fy1)
											{
												fcy = fy1 + fradius;
												fy3 = fcy;
												fsangle = 180; feangle = 270;
												fsx = fx3;  fsy = fy3;
												fex = fcx;  fey = fy1;
											}
											else
											{
												fcy = fy1 - fradius;
												fy3 = fcy;
												fsangle = 90; feangle = 180;
												fsx = fcx;		fsy = fy1;
												fex = fx3;		fey = fy3;
											}
										}
									}
									fx2 = fcx;	
									fy2 = fy1;
								}
								else
								{
									fradius = abs(deltax);
									if(fradius <= fEDA_AutoArcMaxRadius)
									{
										fx3 = fx4;
										fy3 = fy4;
										if(fy4 > fy1)
										{
											fcx = fx4;
											fcy = fy4 - fradius;
											if(fx4 > fx1)
											{
												fsangle = 90; feangle = 180;
												fsx = fx3;	fsy = fy3;
												fex = fx1;	fey = fcy;
											}
											else
											{
												fsangle = 0; feangle = 90;
												fsx = fx1;	fsy = fcy;
												fex = fx3;	fey = fy3;
											}
										}
										else
										{
											fcx = fx4;
											fcy = fy4 + fradius;
											if(fx4 > fx1)
											{
												fsangle = 180; feangle = 270;
												fsx = fx1;	fsy = fcy;
												fex = fx3;	fey = fy3;
											}
											else
											{
												fsangle = 270; feangle = 360;
												fsx = fx3;	fsy = fy3;
												fex = fx1;	fey = fcy;
											}
										}
									}
									else
									{
										fradius = fEDA_AutoArcMaxRadius;
										fy3 = fy4;
										if(fy4 > fy1)
										{
											fcy = fy4 - fradius;
											if(fx4 > fx1)
											{
												fcx = fx1 + fradius;
												fx3 = fcx;
												fsangle = 90; feangle = 180;
												fsx = fx3;	fsy = fy3;
												fex = fx1;	fey = fcy;
											}
											else
											{
												fcx = fx1 - fradius;
												fx3 = fcx;
												fsangle = 0; feangle = 90;
												fsx = fx1;	fsy = fcy;
												fex = fx3;	fey = fy3;
											}
										}
										else
										{
											fcy = fy4 + fradius;
											if(fx4 > fx1)
											{
												fcx = fx1 + fradius;
												fx3 = fcx;
												fsangle = 180; feangle = 270;
												fsx = fx1;	fsy = fcy;
												fex = fx3;	fey = fy3;
											}
											else
											{
												fcx = fx1 - fradius;
												fx3 = fcx;
												fsangle = 270; feangle = 360;
												fsx = fx3;	fsy = fy3;
												fex = fx1;	fey = fcy;
											}
										}
									}
									fx2 = fx1;
									fy2 = fcy;
								}
							}
						}
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_START)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {}
							else if(fx4 == fx1){}
							else if(fy4 == fy1){}
							else if(abs(fx4 - fx1) == abs(fy4 - fy1)) {}
							else if((fx4>fx1)&&(fy4>fy1))  
							{  
								tempx = fx4 - fx1;  tempy = fy4 - fy1;
								if(tempx > tempy)
								{  
										fx2 = fx4 - tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 - tempx;
										fx2 = fx1;
								}
							}
							else if((fx1 > fx4)&&(fy4 > fy1))	
							{  
								tempx = fx1 - fx4;  tempy = fy4 - fy1;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										fx2 = fx4 + tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 - tempx;
										fx2 = fx1;
								}
							}
							else if((fx4 > fx1)&&(fy1 > fy4))	
							{  
								tempx = fx4 - fx1;  tempy = fy1 - fy4;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										fx2 = fx4 - tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 + tempx;
										fx2 = fx1;
								}
							}
							else if((fx1 > fx4)&&(fy1 > fy4))	
							{  
								tempx = fx1 - fx4;  tempy = fy1 - fy4;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										fx2 = fx4 + tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 + tempx;
										fx2 = fx1;
								}
							}
							fx3 = fx2; fy3 = fy2;
						}
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_END)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {}
							else if(fx4 == fx1){}
							else if(fy4 == fy1){}
							else if(abs(fx4-fx1) == abs(fy4-fy1)) {}
							else if((fx4>fx1)&&(fy4>fy1))  
							{  
								tempx=fx4-fx1;  tempy=fy4-fy1;
								if(tempx > tempy)
								{  
										fx2 = fx1 + tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 + tempx;
										fx2 = fx4;
								}
							}
							else if((fx1>fx4)&&(fy4>fy1))	
							{  
								tempx=fx1-fx4;  tempy=fy4-fy1;
								if(tempx > tempy)
								{  
										fx2 = fx1 - tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 + tempx;
										fx2 = fx4;
								}
							}
							else if((fx4>fx1)&&(fy1>fy4))	
							{  
								tempx=fx4-fx1;  tempy=fy1-fy4;
								if(tempx > tempy)
								{  
										fx2 = fx1 + tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 - tempx;
										fx2 = fx4;
								}
							}
							else if((fx1>fx4)&&(fy1>fy4))	
							{  
								tempx=fx1-fx4;  tempy=fy1-fy4;
								if(tempx > tempy)
								{  
										fx2 = fx1 - tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 - tempx;
										fx2 = fx4;
								}
							}
							fx3 = fx2; fy3 = fy2;
						}
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45ARC_START)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {}
							else if(fx4 == fx1){}
							else if(fy4 == fy1){}
							else if(abs(fx4 - fx1) == abs(fy4 - fy1)) {}
							else if((fx4>fx1)&&(fy4>fy1))  
							{  
								tempx = fx4 - fx1;  tempy = fy4 - fy1;
								if(tempx > tempy)
								{  
										fx2 = fx4 - tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 - tempx;
										fx2 = fx1;
								}
							}
							else if((fx1 > fx4)&&(fy4 > fy1))	
							{  
								tempx = fx1 - fx4;  tempy = fy4 - fy1;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										fx2 = fx4 + tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 - tempx;
										fx2 = fx1;
								}
							}
							else if((fx4 > fx1)&&(fy1 > fy4))	
							{  
								tempx = fx4 - fx1;  tempy = fy1 - fy4;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										fx2 = fx4 - tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 + tempx;
										fx2 = fx1;
								}
							}
							else if((fx1 > fx4)&&(fy1 > fy4))	
							{  
								tempx = fx1 - fx4;  tempy = fy1 - fy4;
								if(tempx == tempy){	}
								else if(tempx > tempy)
								{  
										fx2 = fx4 + tempy;
										fy2 = fy1;
								}
								else
								{ 
										fy2 = fy4 + tempx;
										fx2 = fx1;
								}
							}
							fx3 = fx2; fy3 = fy2;
						}
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45ARC_END)
						{
							if((fx4 == fx1)&&(fy4 == fy1)) {}
							else if(fx4 == fx1){}
							else if(fy4 == fy1){}
							else if(abs(fx4-fx1) == abs(fy4-fy1)) {}
							else if((fx4>fx1)&&(fy4>fy1))  
							{  
								tempx=fx4-fx1;  tempy=fy4-fy1;
								if(tempx > tempy)
								{  
										fx2 = fx1 + tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 + tempx;
										fx2 = fx4;
								}
							}
							else if((fx1>fx4)&&(fy4>fy1))	
							{  
								tempx=fx1-fx4;  tempy=fy4-fy1;
								if(tempx > tempy)
								{  
										fx2 = fx1 - tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 + tempx;
										fx2 = fx4;
								}
							}
							else if((fx4>fx1)&&(fy1>fy4))	
							{  
								tempx=fx4-fx1;  tempy=fy1-fy4;
								if(tempx > tempy)
								{  
										fx2 = fx1 + tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 - tempx;
										fx2 = fx4;
								}
							}
							else if((fx1>fx4)&&(fy1>fy4))	
							{  
								tempx=fx1-fx4;  tempy=fy1-fy4;
								if(tempx > tempy)
								{  
										fx2 = fx1 - tempy;
										fy2 = fy4;
								}
								else
								{ 
										fy2 = fy1 - tempx;
										fx2 = fx4;
								}
							}
							fx3 = fx2; fy3 = fy2;
						}
						else
						{

						}
				
						index = aregion.m_cVertex.GetCount();

						
						
						vertex.fx = fx2;	vertex.fy = fy2;
						vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
						aregion.m_cVertex.SetAt(index-3, vertex);
						
						
						vertex.fx = fx2;	vertex.fy = fy2;
						vertex.cx = fcx;
						vertex.cy = fcy;
						vertex.radius = fradius;
						vertex.sangle = fsangle;
						vertex.eangle = feangle;
						aregion.m_cVertex.SetAt(index-2, vertex);

						
						vertex.fx = fx4;	vertex.fy = fy4;
						vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
						aregion.m_cVertex.SetAt(index-1, vertex);

						CloseMouseCursor();
						aregion.FrameRegion(&dc, this, 1, color);
						OpenMouseCursor();
					}
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					//ElecHotPointsControl(&dc);
					if((fx1==fx3)&&(fy1==fy3)) continue;

					CloseMouseCursor();
					aregion.FrameRegion(&dc, this, 1, color);
					OpenMouseCursor();

					
					if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_START) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90_END;
					else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_END) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90_START;
					else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_START) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45_END;
					else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_END) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45_START;
					Write_Info(szInfo[nEDA_PCB_PlaceTrackMode]);
					
					POINT p;
					int ix,iy;
					Get_VPort_XY(&ix,&iy,m_fCurrentSnapX,m_fCurrentSnapY);
					p.x = ix; p.y = iy;
					DrawMyCursor(&dc,&p);

					if(fradius != 0)
					{
						index = aregion.m_cVertex.GetCount();
						if((fx1 != fx2)||(fy1 != fy2))
						{
							vertex.fx = fx2;	vertex.fy = fy2;
							vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
							aregion.m_cVertex.SetAt(index-3, vertex);
						
							vertex.fx = fx2;	vertex.fy = fy2;
							vertex.cx = fcx;
							vertex.cy = fcy;
							vertex.radius = fradius;
							vertex.sangle = fsangle;
							vertex.eangle = feangle;
							aregion.m_cVertex.SetAt(index - 2, vertex);
						
							vertex.fx = fx3;	vertex.fy = fy3;
							vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
							aregion.m_cVertex.SetAt(index - 1, vertex);							
							
							vertex.fx = fx4;	vertex.fy = fy4;
							vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
							aregion.m_cVertex.Add(vertex);
							aregion.m_cVertex.Add(vertex);					
							aregion.m_cVertex.Add(vertex);											
						}
						else
						{
							vertex.fx = fx2;	vertex.fy = fy2;
							vertex.cx = fcx;
							vertex.cy = fcy;
							vertex.radius = fradius;
							vertex.sangle = fsangle;
							vertex.eangle = feangle;
							aregion.m_cVertex.SetAt(index - 3, vertex);
							
							vertex.fx = fx3;	vertex.fy = fy3;
							vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
							aregion.m_cVertex.SetAt(index - 2, vertex);
							
							vertex.fx = fx4;	vertex.fy = fy4;
							vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
							aregion.m_cVertex.Add(vertex);
							aregion.m_cVertex.Add(vertex);
						}
					}
					else
					{
						if((fx1 != fx2)||(fy1 != fy2))
						{
							vertex.fx = fx2;	vertex.fy = fy2;
							vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
							index = aregion.m_cVertex.GetCount();
							aregion.m_cVertex.SetAt(index-3, vertex);
							
							vertex.fx = fx4;	vertex.fy = fy4;
							vertex.cx = vertex.cy = vertex.radius = vertex.eangle = vertex.sangle = 0;
							aregion.m_cVertex.Add(vertex);
						}
					}

					aregion.FrameRegion(&dc, this, 1, color);

					DrawMyCursor(&dc,&p);

					fx1 = fx3;  fy1 = fy3;
					fx2 = fx4;  fy2 = fy4;
					fx3 = fx4 = fx2;
					fy3 = fy4 = fy2;
					fradius = 0;
					continue;
				}
				else if(ch1 == SPACE)
				{
					if((GetKeyState(VK_SHIFT)&0x80) != 0)
					{
						
						if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90ARC_START;
						else if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_90ARC_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90ARC_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45_START;
						//else if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45ARC_START;
						//else if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_45ARC_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45ARC_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_ANYANGLE;
						else if((nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_START)||(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_END)) nEDA_PCB_PlaceTrackMode = PCB_PLACE_ANYANGLE;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_ANYANGLE) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90_START; 
					}
					else 
					{
						
						if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_START) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90_END;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90_END) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90_START;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90ARC_START) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90ARC_END;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_90ARC_END) nEDA_PCB_PlaceTrackMode = PCB_PLACE_90ARC_START;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_START) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45_END;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45_END) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45_START;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45ARC_START) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45ARC_END;
						else if(nEDA_PCB_PlaceTrackMode == PCB_PLACE_45ARC_END) nEDA_PCB_PlaceTrackMode = PCB_PLACE_45ARC_START;
					}
					Write_Info(szInfo[nEDA_PCB_PlaceTrackMode]);
				}
			}while(1);
		}
		else if( ch1 == MOUSEMOVE)
		{
			//if(nEDA_RedrawFlag == ON) nEDA_RedrawFlag = OFF;
			//else  ElecHotPointsControl(&dc);
		}
		else if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
		{
			//ElecHotPointsControl(&dc);
			break;
		}
		else Command_In_Function(ch1);
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));
	
	dc.SetROP2(drawmode);
	
	index = aregion.m_cVertex.GetCount();
	for(int i=index-1; i>1; i--)
	{
		Struct_RegionVertex vertex1, vertex2;
		vertex1 = aregion.m_cVertex.GetAt(i);
		vertex2 = aregion.m_cVertex.GetAt(i-1);
		if( (vertex1.cx == vertex2.cx)&&(vertex1.cy == vertex2.cy)&&(vertex1.eangle = vertex2.eangle)&&(vertex1.fx == vertex2.fx)&&\
			(vertex1.fy == vertex2.fy)&&(vertex1.radius == vertex2.radius)&&(vertex1.sangle == vertex2.sangle))
		{
			aregion.m_cVertex.RemoveAt(i);
		}
	}

	CArray<Fpoint, Fpoint&> arrayVertex;	
	aregion.GetRegionVertex(arrayVertex);
	CleanVertex(arrayVertex);

	if(arrayVertex.GetCount() > 2)
	{
		COb_pcbpolygon* ppolygon = new COb_pcbpolygon();
		ppolygon->pDocument = pDoc;
		ppolygon->m_nPolygonType = SPLITPLANE;
		ppolygon->m_nLayer = layer;
		for(int i=0; i<arrayVertex.GetCount(); i++)
		{
			Fpoint fp;
			fp = arrayVertex.GetAt(i);
			Struct_RegionVertex vertex;
			vertex.fx = fp.fx;
			vertex.fy = fp.fy;
			vertex.cx = vertex.cy = vertex.radius = vertex.sangle = vertex.eangle = 0;
			ppolygon->m_cVertex.Add(vertex);
		}

		int newindex = pDoc->m_arrayPcbPolygon.Add(ppolygon);
		
		if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
		{
			pDoc->Clear_Discarded_Undo_Buffer();
		}
		CPcbUndo *pundo = new CPcbUndo;
		pundo->m_nOp = UNDO_DELETE;
		pundo->m_nOb = PCB_ELEMENT_POLYGON;
		pundo->m_nIndex = newindex;
		pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		
		for(int i=0; i<arrayVertex.GetCount(); i++)
		{
			Fpoint fp1, fp2;
			fp1 = arrayVertex.GetAt(i);
			if(i == (arrayVertex.GetCount() - 1)) fp2 = arrayVertex.GetAt(0);
			else fp2 = arrayVertex.GetAt(i + 1);
			
			COb_pcbtrack* pnewtrack = new COb_pcbtrack();
			pnewtrack->m_nLayer = ppolygon->m_nLayer;
			pnewtrack->pDocument = pDoc;
			pnewtrack->m_nPolygon = -2;
			pnewtrack->m_fWidth = pDoc->m_fRulesClearance + 1;
			pnewtrack->m_fX1 = fp1.fx;
			pnewtrack->m_fY1 = fp1.fy;
			pnewtrack->m_fX2 = fp2.fx;
			pnewtrack->m_fY2 = fp2.fy;

			newindex = pDoc->m_arrayPcbTrack.Add(pnewtrack);
			
			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_TRACK;
			pundo->m_nIndex = newindex;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
			
		}

		
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		
	}

	if(m_bCursorState == TRUE) CloseMouseCursor();
	//释放鼠标锁定区域；
	ClipCursor(NULL);  m_bClipState = FALSE;

	Write_Help(szEDA_Help_Ready);
}
