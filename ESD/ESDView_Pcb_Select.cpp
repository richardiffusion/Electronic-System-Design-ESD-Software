struct Struct_Pcb_Multi_Selection CESDView::SelectPcbObject(double fx, double fy, int mode)
{
	
	Struct_Pcb_Multi_Selection selected_element;

	CArray<Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&> array_element;
	CDlg_PCBMultiSelection dlg;

	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* ppcbarc = pDoc->m_arrayPcbArc.GetAt(i);
		if((ppcbarc->m_nPolygon == -1)&&(ppcbarc->m_nComponent == -1)&&(ppcbarc->Is_On_Me(fx, fy) == TRUE)&&(ppcbarc->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_ARC;
			selected_element.index = i;
			
			if(mode == THE_FIRST_ONE) return selected_element;
			if(mode == PCB_HIGHLIGHT_SELECTION)
			{
				if((ppcbarc->m_nLayer == MULTILAYER)||(ppcbarc->m_nLayer == m_nPCB_Current_Layer)) return selected_element;
			}
			
			if(array_element.GetCount() > 15) break;
			array_element.Add(selected_element);
			dlg.m_arrayObject.Add(ppcbarc);

			CString cstring;
			cstring = "Ô²»¡ ";
			cstring += pDoc->m_sPCB_LayerParam[ppcbarc->m_nLayer].csLayerName;
			if(cstring.GetLength() < 16) for(int m=cstring.GetLength(); m < 16; m++) cstring += " ";
			cstring += "(" + pDoc->Convert_XY_To_String(ppcbarc->m_fCx  - pDoc->m_cPCB_Sheet.m_fOriginX) + "," + pDoc->Convert_XY_To_String(ppcbarc->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY) + ")";
			dlg.m_arrayString.Add(cstring);
			TRACE("pcbarc\n");
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nPolygon == -1)&&(ptrack->m_nComponent == -1)&&(ptrack->Is_On_Me(fx, fy) == TRUE)&&(ptrack->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_TRACK;
			selected_element.index = i;
			
			if(mode == THE_FIRST_ONE) return selected_element;
			if(mode == PCB_HIGHLIGHT_SELECTION)
			{
				if((ptrack->m_nLayer == MULTILAYER)||(ptrack->m_nLayer == m_nPCB_Current_Layer)) return selected_element;	
			}

			if(array_element.GetCount() > 15) break;
			array_element.Add(selected_element);
			dlg.m_arrayObject.Add(ptrack);

			CString cstring;
			cstring = "Á¬Ïß ";
			cstring += pDoc->m_sPCB_LayerParam[ptrack->m_nLayer].csLayerName;
			if(cstring.GetLength() < 16) for(int m=cstring.GetLength(); m < 16; m++) cstring += " ";
			cstring += "(" + pDoc->Convert_XY_To_String(ptrack->m_fX1 - pDoc->m_cPCB_Sheet.m_fOriginX) + "," + pDoc->Convert_XY_To_String(ptrack->m_fY1 - pDoc->m_cPCB_Sheet.m_fOriginY) + ")";
			dlg.m_arrayString.Add(cstring);
			TRACE("pcbtrack\n");
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);

		if((pvia->m_nComponent == -1)&&(pvia->Is_On_Me(fx, fy) == TRUE)&&(pvia->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_VIA;
			selected_element.index = i;
			
			if(mode == THE_FIRST_ONE) return selected_element;
			if(mode == PCB_HIGHLIGHT_SELECTION) return selected_element;
			
			if(array_element.GetCount() > 15) break;
			array_element.Add(selected_element);
			dlg.m_arrayObject.Add(pvia);

			CString cstring;
			cstring = "¹ý¿× ";
			
			if(cstring.GetLength() < 16) for(int m=cstring.GetLength(); m < 16; m++) cstring += " ";
			cstring += "(" + pDoc->Convert_XY_To_String(pvia->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX) + "," + pDoc->Convert_XY_To_String(pvia->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY) + ")";
			dlg.m_arrayString.Add(cstring);
			TRACE("pcbvia\n");
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ppcbtext = pDoc->m_arrayPcbText.GetAt(i);
		if((ppcbtext->m_nFlag == FLAG_NORMAL)&&(ppcbtext->Is_On_Me(fx, fy, this) == TRUE))
		{
			if(ppcbtext->m_nComponent == -1)  
			{
				selected_element.pcbelement = PCB_ELEMENT_TEXT;
				selected_element.index = i;
				
				if(mode == THE_FIRST_ONE) return selected_element;
				if(mode == PCB_HIGHLIGHT_SELECTION) break;
				
				if(array_element.GetCount() > 15) break;
				array_element.Add(selected_element);
				dlg.m_arrayObject.Add(ppcbtext);

				CString cstring;
				cstring = "ÎÄ×Ö ";
				cstring += pDoc->m_sPCB_LayerParam[ppcbtext->m_nLayer].csLayerName;
				if(cstring.GetLength() < 16) for(int m=cstring.GetLength(); m < 16; m++) cstring += " ";
				cstring += "(" + ppcbtext->m_csText + ")";
				dlg.m_arrayString.Add(cstring);
				TRACE("pcbtext\n");
			}
			else	
			{		
				COb_pcbcomp* pcomp = (COb_pcbcomp*)pDoc->m_arrayPcbComp.GetAt(ppcbtext->m_nComponent);
				if( ((ppcbtext->m_bComment == TRUE)&&(pcomp->m_bCommentOn == TRUE)) || ((ppcbtext->m_bDesignator == TRUE)&&(pcomp->m_bNameOn == TRUE)) )
				{
					selected_element.pcbelement = PCB_ELEMENT_TEXT;
					selected_element.index = i;
					
					if(mode == THE_FIRST_ONE) return selected_element;
					if(mode == PCB_HIGHLIGHT_SELECTION) break;

					if(array_element.GetCount() > 15) break;
					array_element.Add(selected_element);
					dlg.m_arrayObject.Add(ppcbtext);

					CString cstring;
					cstring = "ÎÄ×Ö ";
					cstring += pDoc->m_sPCB_LayerParam[ppcbtext->m_nLayer].csLayerName;
					if(cstring.GetLength() < 16) for(int m=cstring.GetLength(); m < 16; m++) cstring += " ";
					cstring += "(" + ppcbtext->m_csText + ")";
					dlg.m_arrayString.Add(cstring);
					TRACE("pcbtext\n");
				}

			}
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* ppcbfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((ppcbfill->m_nComponent == -1)&&(ppcbfill->Is_On_Me(fx, fy) == TRUE)&&(ppcbfill->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_FILL;
			selected_element.index = i;
			
			if(mode == THE_FIRST_ONE) return selected_element;			
			if(mode == PCB_HIGHLIGHT_SELECTION)
			{
				if((ppcbfill->m_nLayer == MULTILAYER)||(ppcbfill->m_nLayer == m_nPCB_Current_Layer)) return selected_element;	
			}

			if(array_element.GetCount() > 15) break;
			array_element.Add(selected_element);
			dlg.m_arrayObject.Add(ppcbfill);

			CString cstring;
			cstring = "Ìî³ä ";
			cstring += pDoc->m_sPCB_LayerParam[ppcbfill->m_nLayer].csLayerName;
			if(cstring.GetLength() < 16) for(int m=cstring.GetLength(); m < 16; m++) cstring += " ";
			cstring += "(" + pDoc->Convert_XY_To_String(ppcbfill->m_fX1 - pDoc->m_cPCB_Sheet.m_fOriginX) + "," + pDoc->Convert_XY_To_String(ppcbfill->m_fY1 - pDoc->m_cPCB_Sheet.m_fOriginY) + ")";
			dlg.m_arrayString.Add(cstring);
			TRACE("pcbfill\n");
		}
	}

	int padflag = 0;
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->Is_On_Me(fx, fy) == TRUE)&&(ppad->m_nFlag == FLAG_NORMAL))
		{
			if((mode == INCLUDE_CPAD)||(mode == PCB_HIGHLIGHT_SELECTION)||(mode == THE_FIRST_ONE)||( (mode == NOT_INCLUDE_CPAD)&&(ppad->m_nComponent < 0)) ) //( ((mode == NOT_INCLUDE_CPAD)||(mode == THE_FIRST_ONE))&&(ppad->m_nComponent < 0)) )
			{
				selected_element.pcbelement = PCB_ELEMENT_PAD;
				selected_element.index = i;
				
				if(mode == THE_FIRST_ONE) return selected_element;
				if(mode == PCB_HIGHLIGHT_SELECTION)
				{
					if((ppad->m_nLayer == MULTILAYER)||(ppad->m_nLayer == m_nPCB_Current_Layer)) return selected_element;
				}

				if(array_element.GetCount() > 15) break;
				array_element.Add(selected_element);
				dlg.m_arrayObject.Add(ppad);

				CString cstring;
				cstring = "º¸ÅÌ ";
				cstring += pDoc->m_sPCB_LayerParam[ppad->m_nLayer].csLayerName;
				if(cstring.GetLength() < 16) for(int m=cstring.GetLength(); m < 16; m++) cstring += " ";
				cstring += "(" + pDoc->Convert_XY_To_String(ppad->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX) + "," + pDoc->Convert_XY_To_String(ppad->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY) + ")";
				dlg.m_arrayString.Add(cstring);
				padflag = 1;
				TRACE("pcbpcb\n");
			}
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nPolygon == -1)&&(pregion->m_nComponent == -1)&&(pregion->Is_On_Me(fx, fy) == TRUE)&&(pregion->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_REGION;
			selected_element.index = i;
			
			if(mode == THE_FIRST_ONE) return selected_element;
			if(mode == PCB_HIGHLIGHT_SELECTION)
			{
				if((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == m_nPCB_Current_Layer)) return selected_element;	
			}

			if(array_element.GetCount() > 15) break;
			array_element.Add(selected_element);
			dlg.m_arrayObject.Add(pregion);

			CString cstring;
			cstring = "ÇøÓò ";
			cstring += pDoc->m_sPCB_LayerParam[pregion->m_nLayer].csLayerName;
			if(cstring.GetLength() < 16) for(int m=cstring.GetLength(); m < 16; m++) cstring += " ";
			dlg.m_arrayString.Add(cstring);
			TRACE("pcbregion\n");
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbCoordinate.GetCount(); i++)
	{
		COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(i);
		if((pcoordinate->Is_On_Me(fx, fy, this) == TRUE)&&(pcoordinate->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_COORDINATE;
			selected_element.index = i;
			
			if(mode == THE_FIRST_ONE) return selected_element;
			if(mode == PCB_HIGHLIGHT_SELECTION) break;

			if(array_element.GetCount() > 15) break;
			array_element.Add(selected_element);
			dlg.m_arrayObject.Add(pcoordinate);

			CString cstring = "×ø±ê ";
			cstring += pDoc->m_sPCB_LayerParam[pcoordinate->m_nLayer].csLayerName;
			if(cstring.GetLength() < 16) for(int m=cstring.GetLength(); m < 16; m++) cstring += " ";
			dlg.m_arrayString.Add(cstring);
			TRACE("pcoordinate\n");
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(i);
		if((pdimension->Is_On_Me(fx, fy, this) == TRUE)&&(pdimension->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_DIMENSION;
			selected_element.index = i;
			
			if(mode == THE_FIRST_ONE) return selected_element;
			if(mode == PCB_HIGHLIGHT_SELECTION) break;
			
			if(array_element.GetCount() > 15) break;
			array_element.Add(selected_element);
			dlg.m_arrayObject.Add(pdimension);

			CString cstring = "³ß´ç±ê×¢ ";
			cstring += pDoc->m_sPCB_LayerParam[pdimension->m_nLayer].csLayerName;
			if(cstring.GetLength() < 16) for(int m=cstring.GetLength(); m < 16; m++) cstring += " ";
			dlg.m_arrayString.Add(cstring);
			TRACE("pdimension\n");
		}
	}

	if(padflag == 0)	
	{
		for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
			if((ppolygon->Is_On_Me(fx, fy) == TRUE)&&(ppolygon->m_nFlag == FLAG_NORMAL)&&(m_nPCB_Current_Layer == ppolygon->m_nLayer))
			{
				selected_element.pcbelement = PCB_ELEMENT_POLYGON;
				selected_element.index = i;
				
				if(mode == THE_FIRST_ONE) return selected_element;
				/*if(mode == PCB_HIGHLIGHT_SELECTION)
				{
					if((ppolygon->m_nLayer == MULTILAYER)||(ppolygon->m_nLayer == m_nPCB_Current_Layer)) return selected_element;
				}*/

				if(array_element.GetCount() > 15) break;
				array_element.Add(selected_element);
				dlg.m_arrayObject.Add(ppolygon);

				CString cstring;
				if(ppolygon->m_nPolygonType == COPPER_POLYGON) cstring = "¸²Í­ ";
				else if(ppolygon->m_nPolygonType == SPLITPLANE) cstring = "·Ö¸î ";
				cstring += pDoc->m_sPCB_LayerParam[ppolygon->m_nLayer].csLayerName;
				if(cstring.GetLength() < 16) for(int m=cstring.GetLength(); m < 16; m++) cstring += " ";
				dlg.m_arrayString.Add(cstring);
				TRACE("pcbpolygon\n");
			}
		}

		for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
		{
			COb_pcbcomp* pcomp = (COb_pcbcomp*)pDoc->m_arrayPcbComp.GetAt(i);
			if(pcomp->m_nFlag != FLAG_NORMAL) continue;

			
			double fcomp_x1, fcomp_y1, fcomp_x2, fcomp_y2, fcomp_x3, fcomp_y3, fcomp_x4, fcomp_y4;
			pcomp->GetPcbCompRect(&fcomp_x1, &fcomp_y1, &fcomp_x2, &fcomp_y2, &fcomp_x3, &fcomp_y3, &fcomp_x4, &fcomp_y4, i);
			CRgn rgn;
			POINT point[5];
			point[0].x = (int)fcomp_x1;	point[0].y = (int)fcomp_y1;
			point[1].x = (int)fcomp_x2;	point[1].y = (int)fcomp_y2;
			point[2].x = (int)fcomp_x3;	point[2].y = (int)fcomp_y3;
			point[3].x = (int)fcomp_x4;	point[3].y = (int)fcomp_y4;
			point[4].x = (int)fcomp_x1;	point[4].y = (int)fcomp_y1;

			rgn.CreatePolygonRgn(point, 5, ALTERNATE);
			
			if(rgn.PtInRegion((int)fx, (int)fy) != FALSE)
			{
				selected_element.pcbelement = PCB_ELEMENT_COMP;
				selected_element.index = i;
				
				if(mode == THE_FIRST_ONE) return selected_element;
				if(mode == PCB_HIGHLIGHT_SELECTION) break;

				if(array_element.GetCount() > 15) break;
				array_element.Add(selected_element);
				dlg.m_arrayObject.Add(pcomp);

				CString cstring;
				cstring = "Ôª¼þ";
				cstring += "(" + pcomp->GetPcbCompDesignator(i) + ")";
				dlg.m_arrayString.Add(cstring);
				TRACE("pcbcomp\n");
			}
		}
	}

	if(array_element.GetCount() == 0)
	{
		selected_element.pcbelement = 0;
		selected_element.index = -1;
	}
	else if(array_element.GetCount() == 1)
	{
		selected_element.pcbelement = array_element[0].pcbelement;
		selected_element.index = array_element[0].index;
	}
	else
	{
		if(mode == PCB_HIGHLIGHT_SELECTION)
		{
			for(int i=0; i<array_element.GetCount(); i++)
			{
				selected_element.pcbelement = array_element[i].pcbelement;
				selected_element.index = array_element[i].index;
				if((selected_element.pcbelement == PCB_ELEMENT_TRACK)||(selected_element.pcbelement == PCB_ELEMENT_FILL)||(selected_element.pcbelement == PCB_ELEMENT_REGION)) return selected_element;
			}
		}

		
		int x,y;
		Get_VPort_XY(&x, &y, fx, fy);
		CPoint pt;
		pt.x = x; pt.y = y;
		ClientToScreen(&pt);
		x = pt.x; y = pt.y; 
		dlg.m_nx = x;
		dlg.m_ny = y;

		int rtn = dlg.DoModal();
		if( rtn == IDCANCEL)
		{
			selected_element.pcbelement = 0;
			selected_element.index = -1;
		}
		else
		{
			int index = rtn - 100;
			if((index >= 0)&&(index <array_element.GetCount()))
			{
				selected_element.pcbelement = array_element[index].pcbelement;
				selected_element.index = array_element[index].index;
			}
		}
	}

	return selected_element;
}

void CESDView::PcbSelectElement(int element, int index)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if((element <= 0) || (index < 0)) return;

	if(element == PCB_ELEMENT_TRACK)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(index);
		ptrack->m_bSelection = TRUE;
	}
	else if(element == PCB_ELEMENT_ARC)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(index);
		parc->m_bSelection = TRUE;
	}
	else if(element == PCB_ELEMENT_PAD)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(index);
		ppad->m_bSelection = TRUE;
	}	
	else if(element == PCB_ELEMENT_VIA)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(index);
		pvia->m_bSelection = TRUE;
	}		
	else if(element == PCB_ELEMENT_TEXT)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(index);
		ptext->m_bSelection = TRUE;
	}		
	else if(element == PCB_ELEMENT_FILL)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(index);
		pfill->m_bSelection = TRUE;
	}	
	else if(element == PCB_ELEMENT_REGION)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(index);
		pregion->m_bSelection = TRUE;
	}	
	else if(element == PCB_ELEMENT_COMP)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(index);
		pcomp->m_bSelection = TRUE;
	}	
	else if(element == PCB_ELEMENT_POLYGON)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(index);

		ppolygon->m_bSelection = TRUE;
	}	
	else if(element == PCB_ELEMENT_COORDINATE)
	{
		COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(index);
		pcoordinate->m_bSelection = TRUE;
	}	
	else if(element == PCB_ELEMENT_DIMENSION)
	{
		COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(index);
		pdimension->m_bSelection = TRUE;
	}
	else
	{
		return;
	}

	
	pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
}

void CESDView::PcbMouseSelectInsideArea(void)
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

		PcbSelectInsideArea(fminx, fminy, fmaxx, fmaxy);
	}

	dc.SetROP2(drawmode);
	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}

void CESDView::PcbSelectInsideArea(double fx1, double fy1, double fx2, double fy2)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* ppcbarc = pDoc->m_arrayPcbArc.GetAt(i);
		if((ppcbarc->m_nPolygon == -1)&&(ppcbarc->m_nComponent == -1)&&(ppcbarc->m_nFlag == FLAG_NORMAL))
		{
			if(ppcbarc->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppcbarc->m_bSelection = TRUE;
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nPolygon == -1)&&(ptrack->m_nComponent == -1)&&(ptrack->m_nFlag == FLAG_NORMAL))
		{
			if(ptrack->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ptrack->m_bSelection = TRUE;
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);

		if((pvia->m_nComponent == -1)&&(pvia->m_nFlag == FLAG_NORMAL))
		{
			if(pvia->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pvia->m_bSelection = TRUE;
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == -1))
		{
			if(ppad->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppad->m_bSelection = TRUE;
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ppcbtext = pDoc->m_arrayPcbText.GetAt(i);
		if((ppcbtext->m_nFlag == FLAG_NORMAL)&&(ppcbtext->m_nComponent == -1))
		{
			if(ppcbtext->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppcbtext->m_bSelection = TRUE;
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* ppcbfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((ppcbfill->m_nComponent == -1)&&(ppcbfill->m_nFlag == FLAG_NORMAL))
		{
			if(ppcbfill->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppcbfill->m_bSelection = TRUE;
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nPolygon == -1)&&(pregion->m_nComponent == -1)&&(pregion->m_nFlag == FLAG_NORMAL))
		{
			if(pregion->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pregion->m_bSelection = TRUE;
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
		if((ppolygon->m_nFlag == FLAG_NORMAL)&&(m_nPCB_Current_Layer == ppolygon->m_nLayer))
		{
			if(ppolygon->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppolygon->m_bSelection = TRUE;
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = (COb_pcbcomp*)pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag == FLAG_NORMAL)
		{
			if(pcomp->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pcomp->m_bSelection = TRUE;
		}
	}

	pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
}

void CESDView::PcbSelectAll(BOOL Is_Selecting)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nComponent >= 0)||(parc->m_nPolygon != -1)) continue;
			
		parc->m_bSelection = Is_Selecting;
	}
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon != -1)) continue;
			
		ptrack->m_bSelection = Is_Selecting;
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag != FLAG_NORMAL) || ((ptext->m_nComponent >= 0)&&(ptext->m_bComment != TRUE)&&(ptext->m_bDesignator != TRUE)) ) continue;
		
		if(ptext->m_nComponent >= 0)
		{
			if((Is_Selecting == FALSE)&&((ptext->m_bComment == TRUE)||(ptext->m_bDesignator == TRUE))) 	ptext->m_bSelection = Is_Selecting;
			continue;
		}

		ptext->m_bSelection = Is_Selecting;
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)) continue;
			
		pfill->m_bSelection = Is_Selecting;
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag != FLAG_NORMAL) continue;
			
		ppad->m_bSelection = Is_Selecting;
	}		
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nComponent >= 0)) continue;
			
		pvia->m_bSelection = Is_Selecting;
	}	
	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nComponent >= 0)||(pregion->m_nPolygon >= 0)) continue;
			
		pregion->m_bSelection = Is_Selecting;
	}
	for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
		if(ppolygon->m_nFlag != FLAG_NORMAL) continue;

		ppolygon->m_bSelection = Is_Selecting;
	}
	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag != FLAG_NORMAL) continue;
			
		pcomp->m_bSelection = Is_Selecting;
	}	
	for(int i=0; i<pDoc->m_arrayPcbCoordinate.GetCount(); i++)
	{
		COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(i);
		if(pcoordinate->m_nFlag != FLAG_NORMAL) continue;
			
		pcoordinate->m_bSelection = Is_Selecting;
	}	
	for(int i=0; i<pDoc->m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(i);
		if(pdimension->m_nFlag != FLAG_NORMAL) continue;
			
		pdimension->m_bSelection = Is_Selecting;
	}	

	
	pDoc->m_fpPcbSelection = PcbGetSelectionCenter();

}

void CESDView::PcbToggleSelection(void)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nComponent >= 0)||(parc->m_nPolygon >= 0)) continue;
			
		parc->m_bSelection = parc->m_bSelection?FALSE:TRUE;
	}
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;
			
		ptrack->m_bSelection = ptrack->m_bSelection?FALSE:TRUE;
	}
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if((ptext->m_nFlag != FLAG_NORMAL)||(ptext->m_nComponent >= 0)) continue;
			
		ptext->m_bSelection = ptext->m_bSelection?FALSE:TRUE;
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)) continue;
			
		pfill->m_bSelection = pfill->m_bSelection?FALSE:TRUE;
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent >= 0)) continue;
			
		ppad->m_bSelection = ppad->m_bSelection?FALSE:TRUE;
	}		
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nComponent >= 0)) continue;
			
		pvia->m_bSelection = pvia->m_bSelection?FALSE:TRUE;
	}
	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nComponent >= 0)||(pregion->m_nPolygon >= 0)) continue;
			
		pregion->m_bSelection = pregion->m_bSelection?FALSE:TRUE;
	}
	for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
		if(ppolygon->m_nFlag != FLAG_NORMAL) continue;
			
		ppolygon->m_bSelection = ppolygon->m_bSelection?FALSE:TRUE;
	}
	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag != FLAG_NORMAL) continue;
			
		pcomp->m_bSelection = pcomp->m_bSelection?FALSE:TRUE;
	}	
	for(int i=0; i<pDoc->m_arrayPcbCoordinate.GetCount(); i++)
	{
		COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(i);
		if(pcoordinate->m_nFlag != FLAG_NORMAL) continue;
			
		pcoordinate->m_bSelection = pcoordinate->m_bSelection?FALSE:TRUE;
	}	
	for(int i=0; i<pDoc->m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(i);
		if(pdimension->m_nFlag != FLAG_NORMAL) continue;
			
		pdimension->m_bSelection = pdimension->m_bSelection?FALSE:TRUE;
	}	

	
	pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
}


void CESDView::PcbDeSelectInsideArea(double fx1, double fy1, double fx2, double fy2)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* ppcbarc = pDoc->m_arrayPcbArc.GetAt(i);
		if((ppcbarc->m_nPolygon == -1)&&(ppcbarc->m_nComponent == -1)&&(ppcbarc->m_nFlag == FLAG_NORMAL))
		{
			if(ppcbarc->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppcbarc->m_bSelection = FALSE;
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nPolygon == -1)&&(ptrack->m_nComponent == -1)&&(ptrack->m_nFlag == FLAG_NORMAL))
		{
			if(ptrack->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ptrack->m_bSelection = FALSE;
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);

		if((pvia->m_nComponent == -1)&&(pvia->m_nFlag == FLAG_NORMAL))
		{
			if(pvia->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pvia->m_bSelection = FALSE;
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == -1))
		{
			if(ppad->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppad->m_bSelection = FALSE;
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ppcbtext = pDoc->m_arrayPcbText.GetAt(i);
		if((ppcbtext->m_nFlag == FLAG_NORMAL)&&(ppcbtext->m_nComponent == -1))
		{
			if(ppcbtext->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppcbtext->m_bSelection = FALSE;
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* ppcbfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((ppcbfill->m_nComponent == -1)&&(ppcbfill->m_nFlag == FLAG_NORMAL))
		{
			if(ppcbfill->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppcbfill->m_bSelection = FALSE;
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nPolygon == -1)&&(pregion->m_nComponent == -1)&&(pregion->m_nFlag == FLAG_NORMAL))
		{
			if(pregion->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pregion->m_bSelection = FALSE;
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
		if((ppolygon->m_nFlag == FLAG_NORMAL)&&(m_nPCB_Current_Layer == ppolygon->m_nLayer))
		{
			if(ppolygon->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) ppolygon->m_bSelection = FALSE;
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = (COb_pcbcomp*)pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag == FLAG_NORMAL)
		{
			if(pcomp->Is_Inside_Area(fx1, fy1, fx2, fy2) == TRUE) pcomp->m_bSelection = FALSE;
		}
	}
	pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
}


void CESDView::PcbDeSelectOutsideArea(double fx1, double fy1, double fx2, double fy2)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* ppcbarc = pDoc->m_arrayPcbArc.GetAt(i);
		if((ppcbarc->m_nPolygon == -1)&&(ppcbarc->m_nComponent == -1)&&(ppcbarc->m_nFlag == FLAG_NORMAL))
		{
			if(ppcbarc->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) ppcbarc->m_bSelection = FALSE;
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nPolygon == -1)&&(ptrack->m_nComponent == -1)&&(ptrack->m_nFlag == FLAG_NORMAL))
		{
			if(ptrack->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) ptrack->m_bSelection = FALSE;
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);

		if((pvia->m_nComponent == -1)&&(pvia->m_nFlag == FLAG_NORMAL))
		{
			if(pvia->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) pvia->m_bSelection = FALSE;
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == -1))
		{
			if(ppad->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) ppad->m_bSelection = FALSE;
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ppcbtext = pDoc->m_arrayPcbText.GetAt(i);
		if((ppcbtext->m_nFlag == FLAG_NORMAL)&&(ppcbtext->m_nComponent == -1))
		{
			if(ppcbtext->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) ppcbtext->m_bSelection = FALSE;
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* ppcbfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((ppcbfill->m_nComponent == -1)&&(ppcbfill->m_nFlag == FLAG_NORMAL))
		{
			if(ppcbfill->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) ppcbfill->m_bSelection = FALSE;
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nPolygon == -1)&&(pregion->m_nComponent == -1)&&(pregion->m_nFlag == FLAG_NORMAL))
		{
			if(pregion->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) pregion->m_bSelection = FALSE;
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
		if((ppolygon->m_nFlag == FLAG_NORMAL)&&(m_nPCB_Current_Layer == ppolygon->m_nLayer))
		{
			if(ppolygon->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) ppolygon->m_bSelection = FALSE;
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = (COb_pcbcomp*)pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag == FLAG_NORMAL)
		{
			if(pcomp->Is_Inside_Area(fx1, fy1, fx2, fy2) == FALSE) pcomp->m_bSelection = FALSE;
		}
	}

	pDoc->m_fpPcbSelection = PcbGetSelectionCenter();
}

Fpoint CESDView::PcbGetSelectionCenter(void)
{
	double fminx, fminy, fmaxx, fmaxy;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	Struct_Pcb_Multi_Selection selected_element;

	fminx = fminy = 100000;
	fmaxx = fmaxy = -100000;
	pDoc->m_arrayPcbSelection.RemoveAll();

	
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* ppcbarc = pDoc->m_arrayPcbArc.GetAt(i);
		if((ppcbarc->m_nPolygon == -1)&&(ppcbarc->m_nComponent == -1)&&(ppcbarc->m_bSelection == TRUE)&&(ppcbarc->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_ARC;
			selected_element.index = i;
			pDoc->m_arrayPcbSelection.Add(selected_element);

			fmaxx = MAX(fmaxx, ppcbarc->m_fCx + ppcbarc->m_fRadius);
			fmaxy = MAX(fmaxy, ppcbarc->m_fCy + ppcbarc->m_fRadius);
			fminx = MIN(fminx, ppcbarc->m_fCx - ppcbarc->m_fRadius);
			fminy = MIN(fminy, ppcbarc->m_fCy - ppcbarc->m_fRadius);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nPolygon == -1)&&(ptrack->m_nComponent == -1)&&(ptrack->m_bSelection == TRUE)&&(ptrack->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_TRACK;
			selected_element.index = i;
			pDoc->m_arrayPcbSelection.Add(selected_element);			
		
			double fx1, fy1, fx2, fy2;
			fx1 = MIN(ptrack->m_fX1, ptrack->m_fX2);
			fy1 = MIN(ptrack->m_fY1, ptrack->m_fY2);
			fx2 = MAX(ptrack->m_fX1, ptrack->m_fX2);
			fy2 = MAX(ptrack->m_fY1, ptrack->m_fY2);
			fmaxx = MAX(fmaxx, fx2);
			fmaxy = MAX(fmaxy, fy2);
			fminx = MIN(fminx, fx1);
			fminy = MIN(fminy, fy1);			
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if((pvia->m_nComponent == -1)&&(pvia->m_bSelection == TRUE)&&(pvia->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_VIA;
			selected_element.index = i;
			pDoc->m_arrayPcbSelection.Add(selected_element);				
			
			fmaxx = MAX(fmaxx, pvia->m_fCx + pvia->m_fDia/2);
			fmaxy = MAX(fmaxy, pvia->m_fCy + pvia->m_fDia/2);
			fminx = MIN(fminx, pvia->m_fCx - pvia->m_fDia/2);
			fminy = MIN(fminy, pvia->m_fCy - pvia->m_fDia/2);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ppcbtext = pDoc->m_arrayPcbText.GetAt(i);
		if((ppcbtext->m_nFlag == FLAG_NORMAL)&&(ppcbtext->m_bSelection == TRUE))
		{
			selected_element.pcbelement = PCB_ELEMENT_TEXT;
			selected_element.index = i;
			pDoc->m_arrayPcbSelection.Add(selected_element);	
				
			double fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;
			ppcbtext->GetTextCorner(&fx1,&fy1,&fx2,&fy2,&fx3,&fy3,&fx4,&fy4);
			
			fmaxx = MAX(fmaxx, MAX(fx1, MAX(fx2, MAX(fx3, fx4))));
			fmaxy = MAX(fmaxy, MAX(fy1, MAX(fy2, MAX(fy3, fy4))));
			fminx = MIN(fminx, MIN(fx1, MAX(fx2, MAX(fx3, fx4))));
			fminy = MIN(fminy, MIN(fy1, MAX(fy2, MAX(fy3, fy4))));
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* ppcbfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((ppcbfill->m_nComponent == -1)&&(ppcbfill->m_bSelection == TRUE)&&(ppcbfill->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_FILL;
			selected_element.index = i;
			pDoc->m_arrayPcbSelection.Add(selected_element);				
			
			fmaxx = MAX(fmaxx, MAX(ppcbfill->m_fX1, ppcbfill->m_fX2));
			fmaxy = MAX(fmaxy, MAX(ppcbfill->m_fY1, ppcbfill->m_fY2));
			fminx = MIN(fminx, MIN(ppcbfill->m_fX1, ppcbfill->m_fX2));
			fminy = MIN(fminy, MIN(ppcbfill->m_fY1, ppcbfill->m_fY2));
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		//if((ppad->m_bSelection == TRUE)&&(ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == -1))
		if((ppad->m_bSelection == TRUE)&&(ppad->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_PAD;
			selected_element.index = i;
			pDoc->m_arrayPcbSelection.Add(selected_element);				
			
			fmaxx = MAX(fmaxx, ppad->m_fCx + ppad->m_fSizeX[0]);
			fmaxy = MAX(fmaxy, ppad->m_fCy + ppad->m_fSizeY[0]);
			fminx = MIN(fminx, ppad->m_fCx - ppad->m_fSizeX[0]);
			fminy = MIN(fminy, ppad->m_fCy - ppad->m_fSizeY[0]);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nPolygon == -1)&&(pregion->m_nComponent == -1)&&(pregion->m_bSelection == TRUE)&&(pregion->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_REGION;
			selected_element.index = i;
			pDoc->m_arrayPcbSelection.Add(selected_element);

			for(int j=0; j<pregion->m_cVertex.GetCount(); j++)
			{
				fmaxx = MAX(fmaxx, pregion->m_cVertex[j].fx);
				fmaxy = MAX(fmaxy, pregion->m_cVertex[j].fy);
				fminx = MIN(fminx, pregion->m_cVertex[j].fx);
				fminy = MIN(fminy, pregion->m_cVertex[j].fy);
			}
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbCoordinate.GetCount(); i++)
	{
		COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(i);
		if((pcoordinate->m_bSelection == TRUE)&&(pcoordinate->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_COORDINATE;
			selected_element.index = i;
			pDoc->m_arrayPcbSelection.Add(selected_element);			
			
			fmaxx = MAX(fmaxx, (pcoordinate->m_fX + 10*pcoordinate->m_fTextHeight));
			fmaxy = MAX(fmaxy, (pcoordinate->m_fY + 10*pcoordinate->m_fTextHeight));
			fminx = MIN(fminx, (pcoordinate->m_fX - 10*pcoordinate->m_fTextHeight));
			fminy = MIN(fminy, (pcoordinate->m_fY - 10*pcoordinate->m_fTextHeight));
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(i);
		if((pdimension->m_bSelection == TRUE)&&(pdimension->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_DIMENSION;
			selected_element.index = i;
			pDoc->m_arrayPcbSelection.Add(selected_element);

			fmaxx = MAX(fmaxx, (pdimension->m_fTextSx + 10*pdimension->m_fTextHeight));
			fmaxy = MAX(fmaxy, (pdimension->m_fTextSy + 10*pdimension->m_fTextHeight));
			fminx = MIN(fminx, (pdimension->m_fTextSx - 10*pdimension->m_fTextHeight));
			fminy = MIN(fminy, (pdimension->m_fTextSy - 10*pdimension->m_fTextHeight));
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);
		if((ppolygon->m_bSelection == TRUE)&&(ppolygon->m_nFlag == FLAG_NORMAL))
		{
			selected_element.pcbelement = PCB_ELEMENT_POLYGON;
			selected_element.index = i;
			pDoc->m_arrayPcbSelection.Add(selected_element);				
			
			for(int j=0; j<ppolygon->m_cVertex.GetCount(); j++)
			{
				fmaxx = MAX(fmaxx, ppolygon->m_cVertex[j].fx);
				fmaxy = MAX(fmaxy, ppolygon->m_cVertex[j].fy);
				fminx = MIN(fminx, ppolygon->m_cVertex[j].fx);
				fminy = MIN(fminy, ppolygon->m_cVertex[j].fy);
			}
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = (COb_pcbcomp*)pDoc->m_arrayPcbComp.GetAt(i);
		if((pcomp->m_nFlag == FLAG_NORMAL)&&(pcomp->m_bSelection == TRUE))
		{
			selected_element.pcbelement = PCB_ELEMENT_COMP;
			selected_element.index = i;
			pDoc->m_arrayPcbSelection.Add(selected_element);	
			
			
			double fcomp_x1, fcomp_y1, fcomp_x2, fcomp_y2, fcomp_x3, fcomp_y3, fcomp_x4, fcomp_y4;
			pcomp->GetPcbCompRect(&fcomp_x1, &fcomp_y1, &fcomp_x2, &fcomp_y2, &fcomp_x3, &fcomp_y3, &fcomp_x4, &fcomp_y4, i);

			CRect rect;
			rect.left = MIN(MIN(fcomp_x1, fcomp_x2), MIN(fcomp_x3, fcomp_x4));
			rect.right = MAX(MAX(fcomp_x1, fcomp_x2), MAX(fcomp_x3, fcomp_x4));
			rect.bottom = MIN(MIN(fcomp_y1, fcomp_y2), MIN(fcomp_y3, fcomp_y4));
			rect.top = MAX(MAX(fcomp_y1, fcomp_y2), MAX(fcomp_y3, fcomp_y4));

			fmaxx = MAX(fmaxx, rect.right);
			fmaxy = MAX(fmaxy, rect.top);
			fminx = MIN(fminx, rect.left);
			fminy = MIN(fminy, rect.bottom);			
		}
	}
	
	if(fminx == fmaxx) fmaxx += 100;
	if(fminy == fmaxy) fmaxy += 100;

	pDoc->m_fPcbSelection_X1 = fminx;
	pDoc->m_fPcbSelection_Y1 = fminy;
	pDoc->m_fPcbSelection_X2 = fmaxx;
	pDoc->m_fPcbSelection_Y2 = fmaxy;

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

struct Struct_Pcb_Multi_Selection CESDView::PcbFindSelectedObject(double fx, double fy)
{
	Struct_Pcb_Multi_Selection null_element;
	null_element.index = -1;
	null_element.pcbelement = 0;

	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<pDoc->m_arrayPcbSelection.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selected_element;
		selected_element = pDoc->m_arrayPcbSelection.GetAt(i);

		if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
			if(ptrack->Is_On_Me(fx, fy) == TRUE) return selected_element;
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
			if(parc->Is_On_Me(fx, fy) == TRUE) return selected_element;
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
			if(ppad->Is_On_Me(fx, fy) == TRUE)  return selected_element;
		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
			if(pvia->Is_On_Me(fx, fy) == TRUE)  return selected_element;
		}		
		else if(selected_element.pcbelement == PCB_ELEMENT_TEXT)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(selected_element.index);
			if(ptext->Is_On_Me(fx, fy, this) == TRUE)  return selected_element;
		}		
		else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
			if(pfill->Is_On_Me(fx, fy) == TRUE)  return selected_element;
		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
			if(pregion->Is_On_Me(fx, fy) == TRUE)  return selected_element;
		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_COMP)
		{
			COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(selected_element.index);
			if(pcomp->Is_On_Me(fx, fy, selected_element.index) == TRUE)  return selected_element;
		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_POLYGON)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(selected_element.index);
			if(ppolygon->Is_On_Me(fx, fy) == TRUE)  return selected_element;
		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_COORDINATE)
		{
			COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(selected_element.index);
			//if(pcoordinate->Is_On_Me(fx, fy) == TRUE)  return selected_element;
		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_DIMENSION)
		{
			COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(selected_element.index);
			//if(pdimension->Is_On_Me(fx, fy) == TRUE)  return selected_element;
		}
	}

	return null_element;
}

void CESDView::PcbProcessingSelection(BOOL mode)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* ppcbarc = pDoc->m_arrayPcbArc.GetAt(i);
		
		if(mode == FALSE) 
		{
			if(ppcbarc->m_nFlag == FLAG_PROCESSING) ppcbarc->m_nFlag = FLAG_NORMAL;
		}
		
		if((ppcbarc->m_nPolygon == -1)&&(ppcbarc->m_nComponent == -1)&&(ppcbarc->m_nFlag == FLAG_NORMAL)&&(mode == TRUE))
		{
			if(ppcbarc->m_bSelection == TRUE)
			{
				ppcbarc->m_nFlag = FLAG_PROCESSING;
			}
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		
		if(mode == FALSE) 
		{
			if(ptrack->m_nFlag == FLAG_PROCESSING) ptrack->m_nFlag = FLAG_NORMAL;
		}

		if((ptrack->m_nPolygon == -1)&&(ptrack->m_nComponent == -1)&&(ptrack->m_nFlag == FLAG_NORMAL)&&(mode == TRUE))
		{
			if(ptrack->m_bSelection == TRUE)
			{
				ptrack->m_nFlag = FLAG_PROCESSING;
			}
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		
		if(mode == FALSE) 
		{
			if(pvia->m_nFlag == FLAG_PROCESSING) pvia->m_nFlag = FLAG_NORMAL;
		}
		
		if((pvia->m_nComponent == -1)&&(pvia->m_nFlag == FLAG_NORMAL)&&(mode == TRUE))
		{
			if(pvia->m_bSelection == TRUE)
			{
				pvia->m_nFlag = FLAG_PROCESSING;
			}
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ppcbtext = pDoc->m_arrayPcbText.GetAt(i);

		if(mode == FALSE) 
		{
			if(ppcbtext->m_nFlag == FLAG_PROCESSING) ppcbtext->m_nFlag = FLAG_NORMAL;
		}

		if((ppcbtext->m_nFlag == FLAG_NORMAL)&&(ppcbtext->m_nComponent == -1)&&(mode == TRUE))
		{
			if(ppcbtext->m_bSelection == TRUE)
			{
				ppcbtext->m_nFlag = FLAG_PROCESSING;
			}
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* ppcbfill = pDoc->m_arrayPcbFill.GetAt(i);

		if(mode == FALSE) 
		{
			if(ppcbfill->m_nFlag == FLAG_PROCESSING) ppcbfill->m_nFlag = FLAG_NORMAL;
		}

		if((ppcbfill->m_nComponent == -1)&&(ppcbfill->m_nFlag == FLAG_NORMAL)&&(mode == TRUE))
		{
			if(ppcbfill->m_bSelection == TRUE)
			{
				ppcbfill->m_nFlag = FLAG_PROCESSING;
			}
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);

		if(mode == FALSE) 
		{
			if(ppad->m_nFlag == FLAG_PROCESSING) ppad->m_nFlag = FLAG_NORMAL;
		}

		if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == -1)&&(mode == TRUE))
		{
			if(ppad->m_bSelection == TRUE)
			{
				ppad->m_nFlag = FLAG_PROCESSING;
			}
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);

		if(mode == FALSE) 
		{
			if(pregion->m_nFlag == FLAG_PROCESSING) pregion->m_nFlag = FLAG_NORMAL;
		}

		if((pregion->m_nPolygon == -1)&&(pregion->m_nComponent == -1)&&(pregion->m_nFlag == FLAG_NORMAL)&&(mode == TRUE))
		{
			if(pregion->m_bSelection == TRUE)
			{
				pregion->m_nFlag = FLAG_PROCESSING;
			}
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbCoordinate.GetCount(); i++)
	{
		COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(i);

		if(mode == FALSE) 
		{
			if(pcoordinate->m_nFlag == FLAG_PROCESSING) pcoordinate->m_nFlag = FLAG_NORMAL;
		}

		if((pcoordinate->m_nFlag == FLAG_NORMAL)&&(mode == TRUE))
		{
			if(pcoordinate->m_bSelection == TRUE)
			{
				pcoordinate->m_nFlag = FLAG_PROCESSING;
			}
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(i);

		if(mode == FALSE) 
		{
			if(pdimension->m_nFlag == FLAG_PROCESSING) pdimension->m_nFlag = FLAG_NORMAL;
		}

		if((pdimension->m_nFlag == FLAG_NORMAL)&&(mode == TRUE))
		{
			if(pdimension->m_bSelection == TRUE)
			{
				pdimension->m_nFlag = FLAG_PROCESSING;
			}
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(i);

		if(mode == FALSE) 
		{
			if(ppolygon->m_nFlag == FLAG_PROCESSING) ppolygon->m_nFlag = FLAG_NORMAL;
		}

		if((ppolygon->m_nFlag == FLAG_NORMAL)&&(m_nPCB_Current_Layer == ppolygon->m_nLayer)&&(mode == TRUE))
		{
			if(ppolygon->m_bSelection == TRUE)
			{
				ppolygon->m_nFlag = FLAG_PROCESSING;
			}
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = (COb_pcbcomp*)pDoc->m_arrayPcbComp.GetAt(i);

		if(mode == FALSE) 
		{
			if(pcomp->m_nFlag == FLAG_PROCESSING) pcomp->m_nFlag = FLAG_NORMAL;
		}

		if((pcomp->m_nFlag == FLAG_NORMAL)&&(mode == TRUE))
		{
			if(pcomp->m_bSelection == TRUE)
			{
				pcomp->m_nFlag = FLAG_PROCESSING;
				for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
					if((parc->m_nComponent == i)&&(parc->m_nFlag == FLAG_NORMAL)) parc->m_nFlag = FLAG_PROCESSING;
				}
				for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
					if((ptrack->m_nComponent == i)&&(ptrack->m_nFlag == FLAG_NORMAL)) ptrack->m_nFlag = FLAG_PROCESSING;
				}
				for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
					if((ppad->m_nComponent == i)&&(ppad->m_nFlag == FLAG_NORMAL)) ppad->m_nFlag = FLAG_PROCESSING;
				}
				for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
				{
					COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
					if((pvia->m_nComponent == i)&&(pvia->m_nFlag == FLAG_NORMAL)) pvia->m_nFlag = FLAG_PROCESSING;
				}
				for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
				{
					COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
					if((pfill->m_nComponent == i)&&(pfill->m_nFlag == FLAG_NORMAL)) pfill->m_nFlag = FLAG_PROCESSING;
				}
				for(int j=0; j<pDoc->m_arrayPcbText.GetCount(); j++)
				{
					COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(j);
					if((ptext->m_nComponent == i)&&(ptext->m_nFlag == FLAG_NORMAL)) ptext->m_nFlag = FLAG_PROCESSING;
				}
			}
		}
	}

	Invalidate();
}

int CESDView::PcbFindSelectionKeyPoint(double fx, double fy)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<pDoc->m_arrayPcbSelection.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selected_element = pDoc->m_arrayPcbSelection.GetAt(i);

		if((selected_element.pcbelement == 0)||( selected_element.index < 0)) continue;
		if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);

			double fx1, fy1, fx2, fy2;
			fx1 = ptrack->m_fX1 - nEDA_PcbSelection_Offset;
			fy1 = ptrack->m_fY1 - nEDA_PcbSelection_Offset;
			fx2 = ptrack->m_fX1 + nEDA_PcbSelection_Offset;
			fy2 = ptrack->m_fY1 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 1;
			fx1 = ptrack->m_fX2 - nEDA_PcbSelection_Offset;
			fy1 = ptrack->m_fY2 - nEDA_PcbSelection_Offset;
			fx2 = ptrack->m_fX2 + nEDA_PcbSelection_Offset;
			fy2 = ptrack->m_fY2 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 1;

			fx1 = (ptrack->m_fX1 + ptrack->m_fX2)/2 - nEDA_PcbSelection_Offset;
			fy1 = (ptrack->m_fY1 + ptrack->m_fY2)/2 - nEDA_PcbSelection_Offset;
			fx2 = (ptrack->m_fX1 + ptrack->m_fX2)/2 + nEDA_PcbSelection_Offset;
			fy2 = (ptrack->m_fY1 + ptrack->m_fY2)/2 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 2;
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
			
			
			double fx0, fy0, fx1, fy1, fx2, fy2;
			
			fx0 = parc->m_fCx + parc->m_fRadius*cos(parc->m_fEangle*fEDA_pi/180);
			fy0 = parc->m_fCy + parc->m_fRadius*sin(parc->m_fEangle*fEDA_pi/180);

			fx1 = fx0 - nEDA_PcbSelection_Offset;
			fy1 = fy0 - nEDA_PcbSelection_Offset;
			fx2 = fx0 + nEDA_PcbSelection_Offset;
			fy2 = fy0 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 3;
			
			fx0 = parc->m_fCx + parc->m_fRadius*cos(parc->m_fSangle*fEDA_pi/180);
			fy0 = parc->m_fCy + parc->m_fRadius*sin(parc->m_fSangle*fEDA_pi/180);

			fx1 = fx0 - nEDA_PcbSelection_Offset;
			fy1 = fy0 - nEDA_PcbSelection_Offset;
			fx2 = fx0 + nEDA_PcbSelection_Offset;
			fy2 = fy0 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 4;

			
			double alpha;
			if(parc->m_fEangle > parc->m_fSangle) alpha = (parc->m_fSangle + parc->m_fEangle)/2;
			else alpha = (parc->m_fSangle + parc->m_fEangle)/2 + 180;
			fx0 = parc->m_fCx + parc->m_fRadius*cos(alpha*fEDA_pi/180);
			fy0 = parc->m_fCy + parc->m_fRadius*sin(alpha*fEDA_pi/180);

			fx1 = fx0 - nEDA_PcbSelection_Offset;
			fy1 = fy0 - nEDA_PcbSelection_Offset;
			fx2 = fx0 + nEDA_PcbSelection_Offset;
			fy2 = fy0 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 5;
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
			
			
			double fx0, fy0, fx1, fy1, fx2, fy2;
			double fxx1, fyy1, fxx2, fyy2, fxx3, fyy3, fxx4, fyy4;
			pfill->GetFillCorner(&fxx1, &fyy1, &fxx2, &fyy2, &fxx3, &fyy3, &fxx4, &fyy4);
			
			fx1 = fxx1 - nEDA_PcbSelection_Offset;
			fy1 = fyy1 - nEDA_PcbSelection_Offset;
			fx2 = fxx1 + nEDA_PcbSelection_Offset;
			fy2 = fyy1 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 6;
			fx1 = fxx2 - nEDA_PcbSelection_Offset;
			fy1 = fyy2 - nEDA_PcbSelection_Offset;
			fx2 = fxx2 + nEDA_PcbSelection_Offset;
			fy2 = fyy2 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 6;
			fx1 = fxx3 - nEDA_PcbSelection_Offset;
			fy1 = fyy3 - nEDA_PcbSelection_Offset;
			fx2 = fxx3 + nEDA_PcbSelection_Offset;
			fy2 = fyy3 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 6;
			fx1 = fxx4 - nEDA_PcbSelection_Offset;
			fy1 = fyy4 - nEDA_PcbSelection_Offset;
			fx2 = fxx4 + nEDA_PcbSelection_Offset;
			fy2 = fyy4 + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 6;

			double fcx, fcy;
			fcx = (pfill->m_fX1 + pfill->m_fX2)/2;
			fcy = (pfill->m_fY1 + pfill->m_fY2)/2;
			double r = abs(pfill->m_fX2 - pfill->m_fX1)*0.35;
			double rx = fcx + r*cos(pfill->m_fRotation*fEDA_pi/180);
			double ry = fcy + r*sin(pfill->m_fRotation*fEDA_pi/180);

			fx1 = rx - nEDA_PcbSelection_Offset;
			fy1 = ry - nEDA_PcbSelection_Offset;
			fx2 = rx + nEDA_PcbSelection_Offset;
			fy2 = ry + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 7;
		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_TEXT)
		{
			COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(selected_element.index);
			
			
			double fx1, fy1, fx2, fy2;
			double r = abs(ptext->m_fX2 - ptext->m_fX1);
			double rx, ry;
			if(ptext->m_bMirror == FALSE)
			{
				rx = ptext->m_fSx + r*cos(ptext->m_fRotation*fEDA_pi/180);
				ry = ptext->m_fSy + r*sin(ptext->m_fRotation*fEDA_pi/180);
			}
			else
			{
				rx = ptext->m_fSx - r*cos(ptext->m_fRotation*fEDA_pi/180);
				ry = ptext->m_fSy - r*sin(ptext->m_fRotation*fEDA_pi/180);
			}

			fx1 = rx - nEDA_PcbSelection_Offset;
			fy1 = ry - nEDA_PcbSelection_Offset;
			fx2 = rx + nEDA_PcbSelection_Offset;
			fy2 = ry + nEDA_PcbSelection_Offset;
			if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 8;
		}		
		/*else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);

		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);

		}*/	
		else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);

			for(int j=0; j<pregion->m_cVertex.GetCount(); j++)
			{
				Struct_RegionVertex vertex = pregion->m_cVertex.GetAt(j);

				double fx1, fy1, fx2, fy2;
				fx1 = vertex.fx - nEDA_PcbSelection_Offset;
				fy1 = vertex.fy - nEDA_PcbSelection_Offset;
				fx2 = vertex.fx + nEDA_PcbSelection_Offset;
				fy2 = vertex.fy + nEDA_PcbSelection_Offset;
				if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 9;
			}
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_POLYGON)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(selected_element.index);
			if(ppolygon->m_nPolygonType == COPPER_POLYGON)
			{
				for(int j=0; j<ppolygon->m_cVertex.GetCount(); j++)
				{
					Struct_RegionVertex vertex = ppolygon->m_cVertex.GetAt(j);

					double fx1, fy1, fx2, fy2;
					fx1 = vertex.fx - nEDA_PcbSelection_Offset;
					fy1 = vertex.fy - nEDA_PcbSelection_Offset;
					fx2 = vertex.fx + nEDA_PcbSelection_Offset;
					fy2 = vertex.fy + nEDA_PcbSelection_Offset;
					if((fx > fx1)&&(fx < fx2)&&(fy > fy1)&&(fy < fy2)) return 10;
				}
			}
		}	
		/*else if(selected_element.pcbelement == PCB_ELEMENT_COMP)
		{
			COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(selected_element.index);

		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_COORDINATE)
		{
			COb_pcbcoordinate* pcoordinate = pDoc->m_arrayPcbCoordinate.GetAt(selected_element.index);

		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_DIMENSION)
		{
			COb_pcbdimension* pdimension = pDoc->m_arrayPcbDimension.GetAt(selected_element.index);

		}*/
	}

	return -1;
}

