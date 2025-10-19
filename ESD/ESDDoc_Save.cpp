void CESDDoc::Save_ESD(CArchive& ar)
{
	ar.WriteString(csESDFileHead_V1_2);
	ar.WriteString("\n");

	for(int nCurrentDiagram=0; nCurrentDiagram<SCH_SHEET_NUM; nCurrentDiagram++)
	{
		ar << (int)m_arrayFont[nCurrentDiagram].GetCount();
		for(int fontNo=0; fontNo<m_arrayFont[nCurrentDiagram].GetCount(); fontNo++)
		{
			LOGFONT logfont = m_arrayFont[nCurrentDiagram].GetAt(fontNo);
			ar << logfont.lfHeight;
			ar << logfont.lfOrientation;
			ar << logfont.lfUnderline;
			ar << logfont.lfItalic;
			ar << logfont.lfWeight;
			ar << logfont.lfStrikeOut;
			CString cstring;
			cstring = logfont.lfFaceName;
			ar << cstring;
		}
	}

	for(int nCurrentDiagram=0; nCurrentDiagram<SCH_SHEET_NUM; nCurrentDiagram++)
	{
		ar << m_fSCH_ScreenX[nCurrentDiagram];			//Screen X
		ar << m_fSCH_ScreenY[nCurrentDiagram];			//Screen Y
		ar << m_fSCH_Scale[nCurrentDiagram];			//Scale
		ar << m_bSCH_SheetShow[nCurrentDiagram];		//Sheet Show
		ar << m_csSCH_SheetName[nCurrentDiagram];		//Sheet Name
		m_pSCH_Sheet[nCurrentDiagram]->Serialize(ar);	//Sheet parameters
	}

	ar << m_nSCH_Unit;
	ar << m_bSCH_SnapGridEnable;
	ar << m_fSCH_SnapGrid;
	ar << m_bSCH_VisibleGridShow;
	ar << m_fSCH_VisibleGrid;
	ar << m_bSCH_ElecGridEnable;
	ar << m_fSCH_ElecGrid;


	CObject* pobject;
	for(int nCurrentDiagram=0; nCurrentDiagram<SCH_SHEET_NUM; nCurrentDiagram++)
	{
		//int nCurrentDiagram=0;
		int num = Get_SCH_Elements_Param(0, nCurrentDiagram, 1);
		ar << num;
		POSITION pos;
		pos = m_listSchObject[nCurrentDiagram].GetHeadPosition();
		for(int i=0; i<m_listSchObject[nCurrentDiagram].GetCount(); i++)
		{	
			if(pos == NULL) break;
			pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
			
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				if((pline->m_nDiagram != nCurrentDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_LINE;
				ar << elementNo;
				pline->Serialize(ar);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
			{
				COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
				if((ppolyline->m_nDiagram != nCurrentDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_POLYLINE;
				ar << elementNo;
				ppolyline->Serialize(ar);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
			{
				COb_schpolygon* ppolygon;
				ppolygon = (COb_schpolygon*)pobject;
				if((ppolygon->m_nDiagram != nCurrentDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_POLYGON;
				ar << elementNo;
				ppolygon->Serialize(ar);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc;
				pjunc = (COb_schjunc*)pobject;
				if((pjunc->m_nDiagram != nCurrentDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_JUNCTION;
				ar << elementNo;
				pjunc->Serialize(ar);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
			{
				COb_schellipsefill* pellipsefill;
				pellipsefill = (COb_schellipsefill*)pobject;
				if((pellipsefill->m_nDiagram != nCurrentDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_ELLIPSEFILL;
				ar << elementNo;
				pellipsefill->Serialize(ar);					
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
			{
				COb_schellipse* pellipse;
				pellipse = (COb_schellipse*)pobject;
				if((pellipse->m_nDiagram != nCurrentDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_ELLIPSE;
				ar << elementNo;
				pellipse->Serialize(ar);					
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
			{
				COb_scharc* parc;
				parc = (COb_scharc*)pobject;
				if((parc->m_nDiagram != nCurrentDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_ARC;
				ar << elementNo;
				parc->Serialize(ar);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
			{
				COb_schpie* ppie;
				ppie = (COb_schpie*)pobject;
				if((ppie->m_nDiagram != nCurrentDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_PIE;
				ar << elementNo;
				ppie->Serialize(ar);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
			{
				COb_schrect* prect;
				prect = (COb_schrect*)pobject;
				if((prect->m_nDiagram != nCurrentDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_RECT;
				ar << elementNo;
				prect->Serialize(ar);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
			{
				COb_schroundrect* proundrect;
				proundrect = (COb_schroundrect*)pobject;
				if((proundrect->m_nDiagram != nCurrentDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_ROUNDRECT;
				ar << elementNo;
				proundrect->Serialize(ar);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport;
				pport = (COb_schport*)pobject;
				if((pport->m_nDiagram != nCurrentDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_PORT;
				ar << elementNo;
				pport->Serialize(ar);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower;
				ppower = (COb_schpower*)pobject;
				if((ppower->m_nDiagram != nCurrentDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_POWER;
				ar << elementNo;
				ppower->Serialize(ar);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
			{
				COb_schbezier* pbezier;
				pbezier = (COb_schbezier*)pobject;
				if((pbezier->m_nDiagram != nCurrentDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_BEZIER;
				ar << elementNo;
				pbezier->Serialize(ar);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
			{
				COb_schgraphic* pgraphic;
				pgraphic = (COb_schgraphic*)pobject;
				if((pgraphic->m_nDiagram != nCurrentDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_GRAPHIC;
				ar << elementNo;
				pgraphic->Serialize(ar);
			}

			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext;
				ptext = (COb_schtext*)pobject;
				if((ptext->m_nDiagram != nCurrentDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_TEXT;
				ar << elementNo;
				ptext->Serialize(ar);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
			{
				COb_schtextframe* ptextframe;
				ptextframe = (COb_schtextframe*)pobject;
				if((ptextframe->m_nDiagram != nCurrentDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
				
				int elementNo = SCH_ELEMENT_TEXTFRAME;
				ar << elementNo;
				ptextframe->Serialize(ar);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol;
				psheetsymbol = (COb_schsheetsymbol*)pobject;
				if((psheetsymbol->m_nDiagram != nCurrentDiagram)||(psheetsymbol->m_nFlag == FLAG_DELETED))
				{
					for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) m_listSchObject[nCurrentDiagram].GetNext(pos);
					continue;
				}
				
				int elementNo = SCH_ELEMENT_SHEETSYMBOL;
				ar << elementNo;
				psheetsymbol->Serialize(ar);

				POSITION posbak = pos;
				int subelementNum = 0;
				for(int k=0; k<psheetsymbol->m_arrayPos.GetSize(); k++)
				{
					pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
					{
						COb_schsheetentry* psheetentry;
						psheetentry = (COb_schsheetentry*)pobject;
						if((psheetentry->m_nDiagram != nCurrentDiagram)||(psheetentry->m_nFlag == FLAG_DELETED))	continue;

						subelementNum++;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						subelementNum++;
					}
				}

				pos = posbak;
				ar << subelementNum;
				for(int k=0; k<psheetsymbol->m_arrayPos.GetSize(); k++)
				{
					pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
					{
						COb_schsheetentry* psheetentry;
						psheetentry = (COb_schsheetentry*)pobject;
						if((psheetentry->m_nDiagram != nCurrentDiagram)||(psheetentry->m_nFlag == FLAG_DELETED))	continue;

						int elementNo = SCH_ELEMENT_SHEETENTRY;
						ar << elementNo;
						psheetentry->Serialize(ar);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext;
						ptext = (COb_schtext*)pobject;

						int elementNo = SCH_ELEMENT_TEXT;
						ar << elementNo;
						ptext->Serialize(ar);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp;
				pcomp = (COb_schcomp*)pobject;
				if((pcomp->m_nDiagram != nCurrentDiagram)||(pcomp->m_nFlag == FLAG_DELETED))
				{
					for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) m_listSchObject[nCurrentDiagram].GetNext(pos);
					continue;
				}

				int elementNo = SCH_ELEMENT_COMPONENT;
				ar << elementNo;
				pcomp->Serialize(ar);

				int subelementNum = pcomp->m_arrayPos.GetCount();
				ar << subelementNum;

				for(int k=0; k<pcomp->m_arrayPos.GetCount(); k++)
				{
					pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
					{
						COb_schpin* ppin = (COb_schpin*)pobject;
						int elementNo = SCH_ELEMENT_PIN;
						ar << elementNo;
						ppin->Serialize(ar);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
					{
						COb_schieee* pieee = (COb_schieee*)pobject;
						int elementNo = SCH_ELEMENT_IEEE;
						ar << elementNo;
						pieee->Serialize(ar);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
					{
						COb_scharc* parc = (COb_scharc*)pobject;
						int elementNo = SCH_ELEMENT_ARC;
						ar << elementNo;
						parc->Serialize(ar);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
					{
						COb_schellipse* pellipse = (COb_schellipse*)pobject;
						int elementNo = SCH_ELEMENT_ELLIPSE;
						ar << elementNo;
						pellipse->Serialize(ar);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
					{
						COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
						int elementNo = SCH_ELEMENT_ELLIPSEFILL;
						ar << elementNo;
						pellipsefill->Serialize(ar);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
					{
						COb_schpie* ppie = (COb_schpie*)pobject;
						int elementNo = SCH_ELEMENT_PIE;
						ar << elementNo;
						ppie->Serialize(ar);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
					{
						COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
						int elementNo = SCH_ELEMENT_POLYLINE;
						ar << elementNo;
						ppolyline->Serialize(ar);	
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
					{
						COb_schrect* prect = (COb_schrect*)pobject;
						int elementNo = SCH_ELEMENT_RECT;
						ar << elementNo;
						prect->Serialize(ar);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
					{
						COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
						int elementNo = SCH_ELEMENT_ROUNDRECT;
						ar << elementNo;
						proundrect->Serialize(ar);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
					{
						COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
						int elementNo = SCH_ELEMENT_POLYGON;
						ar << elementNo;
						ppolygon->Serialize(ar);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
					{
						COb_schbezier* pbezier = (COb_schbezier*)pobject;
						int elementNo = SCH_ELEMENT_BEZIER;
						ar << elementNo;
						pbezier->Serialize(ar);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						int elementNo = SCH_ELEMENT_TEXT;
						ar << elementNo;
						ptext->Serialize(ar);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
					{
						COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
						int elementNo = SCH_ELEMENT_TEXTFRAME;
						ar << elementNo;
						ptextframe->Serialize(ar);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
					{
						COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
						int elementNo = SCH_ELEMENT_GRAPHIC;
						ar << elementNo;
						pgraphic->Serialize(ar);
					}
				}
			}
		}
	}

	
	int num = m_arrayCompDesignator.GetCount();
	ar << num;
	for(int i=0; i<num; i++)
	{
		CString cstring;
		cstring = m_arrayCompDesignator.GetAt(i);
		ar << cstring;
		cstring = m_arrayCompComment.GetAt(i);
		ar << cstring;
		cstring = m_arrayCompPackage.GetAt(i);
		ar << cstring;
		cstring = m_arrayPackageLibraryName.GetAt(i);
		ar << cstring;
	}

	num = m_arraySchNet.GetCount();
	ar << num;
	for(int i=0; i<num; i++) 
	{
		COb_schnet* pschnet = m_arraySchNet.GetAt(i);
		pschnet->Serialize(ar);
	}

	for(int i=0; i<PCB_LAYER_NUM; i++)
	{
		ar << m_sPCB_LayerParam[i].csLayerName;
		ar << m_sPCB_LayerParam[i].nLayerPrev;
		ar << m_sPCB_LayerParam[i].nLayerNext;
		ar << m_sPCB_LayerParam[i].bLayerMechEnabled;
		ar << m_sPCB_LayerParam[i].bLayerSheetLinked;
		ar << m_sPCB_LayerParam[i].csLayerCopThick;
		ar << m_sPCB_LayerParam[i].csLayerDielType;
		ar << m_sPCB_LayerParam[i].csLayerDielConst;
		ar << m_sPCB_LayerParam[i].csLayerDielHeight;
		ar << m_sPCB_LayerParam[i].csLayerDielMaterial;
		
		ar << m_sPCB_LayerParam[i].bLayerShow;
	}

	m_cPCB_Sheet.Serialize(ar);

	CArray<POINT, POINT> arrayNet;
	CArray<POINT, POINT> arrayComp;
	CArray<POINT, POINT> arrayPolygon;
	int newindex;

	num = 0;
	for(int i=0; i<m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag == FLAG_NORMAL) num++;
	}
	ar << num;
	newindex = 0;
	for(int i=0; i<m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag == FLAG_NORMAL)
		{
			POINT point;
			point.x = i;
			point.y = newindex;
			arrayNet.Add(point);
			pnet->Serialize(ar);
			newindex++;
		}
	}

	num = 0;
	for(int i=0; i<m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag == FLAG_NORMAL) num++;
	}
	ar << num;
	newindex = 0;
	for(int i=0; i<m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag == FLAG_NORMAL)
		{
			POINT point;
			point.x = i;
			point.y = newindex;
			arrayComp.Add(point);
			pcomp->Serialize(ar);
			newindex++;
		}
	}
	
	num = 0;
	for(int i=0; i<m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = m_arrayPcbPolygon.GetAt(i);
		if(ppolygon->m_nFlag == FLAG_NORMAL) num++;
	}
	
	ar << num;
	newindex = 0;
	for(int i=0; i<m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = m_arrayPcbPolygon.GetAt(i);
		if(ppolygon->m_nFlag == FLAG_NORMAL) 
		{
			POINT point;
			point.x = i;
			point.y = newindex;
			arrayPolygon.Add(point);

			int netbak = ppolygon->m_nNet;
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == ppolygon->m_nNet) 
				{
					ppolygon->m_nNet = point.y;
					break;
				}
			}

			ppolygon->Serialize(ar);

			ppolygon->m_nNet = netbak;	
			
			newindex++;
		}
	}

	num = m_arrayPcbConnection.GetCount();
	ar << num;
	for(int i=0; i<num; i++)
	{
		COb_pcbconnection* pconnection = m_arrayPcbConnection.GetAt(i);
			
		int netbak = pconnection->m_nNet;
		for(int j=0; j<arrayNet.GetCount(); j++)
		{
			POINT point;
			point = arrayNet.GetAt(j);
			if(point.x == pconnection->m_nNet) 
			{
				pconnection->m_nNet = point.y;
				break;
			}
		}
		pconnection->Serialize(ar);

		pconnection->m_nNet = netbak;
	}

	num = 0;
	for(int i=0; i<m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = m_arrayPcbTrack.GetAt(i);
		if(ptrack->m_nFlag == FLAG_NORMAL) num++;
	}
	ar << num;
	for(int i=0; i<m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = m_arrayPcbTrack.GetAt(i);
		if(ptrack->m_nFlag == FLAG_NORMAL)
		{
			int netbak = ptrack->m_nNet;
			int compbak = ptrack->m_nComponent;
			int polybak = ptrack->m_nPolygon;
			
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == ptrack->m_nNet) 
				{
					ptrack->m_nNet = point.y;
					break;
				}
			}
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == ptrack->m_nComponent) 
				{
					ptrack->m_nComponent = point.y;
					break;
				}
			}
			for(int j=0; j<arrayPolygon.GetCount(); j++)
			{
				POINT point;
				point = arrayPolygon.GetAt(j);
				if(point.x == ptrack->m_nPolygon) 
				{
					ptrack->m_nPolygon = point.y;
					break;
				}
			}
			ptrack->Serialize(ar);
			
			ptrack->m_nNet = netbak;
			ptrack->m_nComponent = compbak;
			ptrack->m_nPolygon = polybak;
		}
	}

	num = 0;
	for(int i=0; i<m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = m_arrayPcbArc.GetAt(i);
		if(parc->m_nFlag == FLAG_NORMAL) num++;
	}
	ar << num;
	for(int i=0; i<m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = m_arrayPcbArc.GetAt(i);
		if(parc->m_nFlag == FLAG_NORMAL)
		{
			int netbak = parc->m_nNet;
			int compbak = parc->m_nComponent;
			int polybak = parc->m_nPolygon;
			
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == parc->m_nNet) 
				{
					parc->m_nNet = point.y;
					break;
				}
			}
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == parc->m_nComponent) 
				{
					parc->m_nComponent = point.y;
					break;
				}
			}
			for(int j=0; j<arrayPolygon.GetCount(); j++)
			{
				POINT point;
				point = arrayPolygon.GetAt(j);
				if(point.x == parc->m_nPolygon) 
				{
					parc->m_nPolygon = point.y;
					break;
				}
			}
			parc->Serialize(ar);

			parc->m_nNet = netbak;
			parc->m_nComponent = compbak;
			parc->m_nPolygon = polybak;
		}
	}

	num = 0;
	for(int i=0; i<m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = m_arrayPcbFill.GetAt(i);
		if(pfill->m_nFlag == FLAG_NORMAL) num++;
	}
	ar << num;
	for(int i=0; i<m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = m_arrayPcbFill.GetAt(i);
		if(pfill->m_nFlag == FLAG_NORMAL) 
		{
			int netbak = pfill->m_nNet;
			int compbak = pfill->m_nComponent;
			
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == pfill->m_nNet) 
				{
					pfill->m_nNet = point.y;
					break;
				}
			}
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == pfill->m_nComponent) 
				{
					pfill->m_nComponent = point.y;
					break;
				}
			}

			pfill->Serialize(ar);

			pfill->m_nNet = netbak;
			pfill->m_nComponent = compbak;
		}
	}

	num = 0;
	for(int i=0; i<m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = m_arrayPcbText.GetAt(i);
		if(ptext->m_nFlag == FLAG_NORMAL) num++;
	}
	ar << num;
	for(int i=0; i<m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = m_arrayPcbText.GetAt(i);
		if(ptext->m_nFlag == FLAG_NORMAL)
		{
			int compbak = ptext->m_nComponent;
			
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == ptext->m_nComponent) 
				{
					ptext->m_nComponent = point.y;
					break;
				}
			}
			ptext->Serialize(ar);

			ptext->m_nComponent = compbak;
		}
	}

	num = 0;
	for(int i=0; i<m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag == FLAG_NORMAL) num++;
	}
	ar << num;
	for(int i=0; i<m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag == FLAG_NORMAL)
		{
			int netbak = ppad->m_nNet;
			int compbak = ppad->m_nComponent;
			
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == ppad->m_nNet) 
				{
					ppad->m_nNet = point.y;
					break;
				}
			}
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == ppad->m_nComponent) 
				{
					ppad->m_nComponent = point.y;
					break;
				}
			}

			ppad->Serialize(ar);
			ppad->m_nNet = netbak;
			ppad->m_nComponent = compbak;
		}
	}

	num = 0;
	for(int i=0; i<m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag == FLAG_NORMAL) num++;
	}
	ar << num;
	for(int i=0; i<m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag == FLAG_NORMAL) 
		{
			int netbak = pvia->m_nNet;
			int compbak = pvia->m_nComponent;
			
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == pvia->m_nNet) 
				{
					pvia->m_nNet = point.y;
					break;
				}
			}
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == pvia->m_nComponent) 
				{
					pvia->m_nComponent = point.y;
					break;
				}
			}
			pvia->Serialize(ar);

			pvia->m_nNet = netbak;
			pvia->m_nComponent = compbak;
		}
	}

	num = 0;
	for(int i=0; i<m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = m_arrayPcbRegion.GetAt(i);
		if(pregion->m_nFlag == FLAG_NORMAL) num++;
	}
	ar << num;
	for(int i=0; i<m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = m_arrayPcbRegion.GetAt(i);
		if(pregion->m_nFlag == FLAG_NORMAL)
		{
			int netbak = pregion->m_nNet;
			int compbak = pregion->m_nComponent;
			int polybak = pregion->m_nPolygon;
			
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == pregion->m_nNet) 
				{
					pregion->m_nNet = point.y;
					break;
				}
			}
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == pregion->m_nComponent) 
				{
					pregion->m_nComponent = point.y;
					break;
				}
			}
			for(int j=0; j<arrayPolygon.GetCount(); j++)
			{
				POINT point;
				point = arrayPolygon.GetAt(j);
				if(point.x == pregion->m_nPolygon) 
				{
					pregion->m_nPolygon = point.y;
					break;
				}
			}
			pregion->Serialize(ar);

			pregion->m_nNet = netbak;
			pregion->m_nComponent = compbak;
			pregion->m_nPolygon = polybak;
		}
	}

	num = 0;
	for(int i=0; i<m_arrayPcbCoordinate.GetCount(); i++)
	{
		COb_pcbcoordinate* pcoordinate = m_arrayPcbCoordinate.GetAt(i);
		if(pcoordinate->m_nFlag == FLAG_NORMAL) num++;
	}
	ar << num;
	for(int i=0; i<m_arrayPcbCoordinate.GetCount(); i++)
	{
		COb_pcbcoordinate* pcoordinate = m_arrayPcbCoordinate.GetAt(i);
		if(pcoordinate->m_nFlag == FLAG_NORMAL) pcoordinate->Serialize(ar);
	}

	num = 0;
	for(int i=0; i<m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = m_arrayPcbDimension.GetAt(i);
		if(pdimension->m_nFlag == FLAG_NORMAL) num++;
	}
	ar << num;
	for(int i=0; i<m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = m_arrayPcbDimension.GetAt(i);
		if(pdimension->m_nFlag == FLAG_NORMAL) pdimension->Serialize(ar);
	}
}

void CESDDoc::Save_AD6_SCH_ASCII_File(CArchive& ar, int nCurrentDiagram)
{
	CString onelinestr;
	CString csLineEnd = "\r\n";
	CString keyword;
	char str[100];
	int nLineNo;
	int i1, i2;

	int total_linenum = Get_SCH_Elements_Param(0, nCurrentDiagram, 2) + 1;

	CString csHead = "|HEADER=Protel for Windows - Schematic Capture Ascii File Version 5.0";
	itoa(total_linenum, str, 10);
	CString csWeight = "|WEIGHT=";
	csWeight += str;
	onelinestr = csHead + csWeight + csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	nLineNo = 0;

	keyword = "|RECORD=31";
	onelinestr += keyword;

	keyword = "|FONTIDCOUNT=";
	itoa(m_arrayFont[nCurrentDiagram].GetCount(), str, 10);
	keyword += str;
	onelinestr += keyword;

	for(int i=1; i<=m_arrayFont[nCurrentDiagram].GetCount(); i++)  
	{
		LOGFONT logfont;
		logfont = m_arrayFont[nCurrentDiagram].GetAt(i-1);
		char fontNo[100];
		itoa(i, fontNo, 10);

		keyword = CString("|SIZE") + fontNo;
		keyword += "=";
		itoa(logfont.lfHeight, str, 10);
		keyword += str;
		onelinestr += keyword;

		if(logfont.lfOrientation != 0)
		{
			keyword = CString("|ROTATION") + fontNo;
			keyword += "=";
			itoa(logfont.lfOrientation, str, 10);
			keyword += str;
			onelinestr += keyword;
		}

		if(logfont.lfUnderline == TRUE)
		{
			keyword = CString("|UNDERLINE") + fontNo;
			keyword += "=T";
			onelinestr += keyword;
		}

		if( logfont.lfItalic == TRUE )
		{
			keyword = CString("|ITALIC") + fontNo;
			keyword += "=T";
			onelinestr += keyword;
		}

		if(logfont.lfWeight == FW_BOLD)
		{
			keyword = CString("|BOLD") + fontNo;
			keyword += "=T";
			onelinestr += keyword;
		}

		if(logfont.lfStrikeOut == TRUE)
		{
			keyword = CString("|STRIKEOUT") + fontNo;
			keyword += "=T";
			onelinestr += keyword;
		}

		keyword = CString("|FONTNAME") + fontNo;
		keyword += "=";
		keyword += logfont.lfFaceName;
		onelinestr += keyword;
	}

	onelinestr += "|USEMBCS=T|ISBOC=T";

	if(m_bSCH_ElecGridEnable == TRUE)
	{
		onelinestr += "|HOTSPOTGRIDON=T";
		onelinestr += "|HOTSPOTGRIDON=T";
	}
	i1 = (int)(m_fSCH_ElecGrid/10);
	i2 = m_fSCH_ElecGrid*10000;
	i2 = i2 - i1*100000;
	itoa(i1, str, 10);
	keyword = "|HOTSPOTGRIDSIZE=";
	keyword += str;
	onelinestr += keyword;
	if(i2 != 0)
	{
		itoa(i2, str, 10);
		keyword = "|HOTSPOTGRIDSIZE_FRAC=";
		keyword += str;
		onelinestr += keyword;
	}

	if(m_pSCH_Sheet[nCurrentDiagram]->m_nStandardStyle != 0)
	{
		keyword = "|SHEETSTYLE=";
		itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nStandardStyle, str, 10);
		keyword += str;
		onelinestr += keyword;
	}

	keyword = "|SYSTEMFONT=";
	itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nFont + 1, str, 10);
	keyword += str;
	onelinestr += keyword;

	if(m_pSCH_Sheet[nCurrentDiagram]->m_nTitleBlockStyle != 0)
	{
		keyword = "|DOCUMENTBORDERSTYLE=";
		itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nTitleBlockStyle, str, 10);
		keyword += str;
		onelinestr += keyword;
	}

	if(m_pSCH_Sheet[nCurrentDiagram]->m_nOrientation != 0)
	{
		keyword = "|WORKSPACEORIENTATION=";
		itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nOrientation, str, 10);
		keyword += str;
		onelinestr += keyword;
	}

	if(m_pSCH_Sheet[nCurrentDiagram]->m_bShowBorder == TRUE)
	{
		keyword = "|BORDERON=T";
		onelinestr += keyword;
	}

	if(m_pSCH_Sheet[nCurrentDiagram]->m_bTitleBlock == TRUE)
	{
		keyword = "|TITLEBLOCKON=T";
		onelinestr += keyword;
	}

	keyword = "|SHEETNUMBERSPACESIZE=";
	itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nSheetNumberSpace, str, 10);
	keyword += str;
	onelinestr += keyword;

	if(m_pSCH_Sheet[nCurrentDiagram]->m_nBorderColor != 0)
	{
		keyword = "|COLOR=";
		itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nBorderColor, str, 10);
		keyword += str;
		onelinestr += keyword;
	}
	if(m_pSCH_Sheet[nCurrentDiagram]->m_nSheetColor != 0)
	{
		keyword = "|AREACOLOR=";
		itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nSheetColor, str, 10);
		keyword += str;
		onelinestr += keyword;
	}

	if(m_bSCH_SnapGridEnable == TRUE)
	{
		onelinestr += "|SNAPGRIDON=T";
	}
	i1 = (int)(m_fSCH_SnapGrid/10);
	i2 = m_fSCH_SnapGrid*10000;
	i2 = i2 - i1*100000;
	itoa(i1, str, 10);
	keyword = "|SNAPGRIDSIZE=";
	keyword += str;
	onelinestr += keyword;
	if(i2 != 0)
	{
		itoa(i2, str, 10);
		keyword = "|SNAPGRIDSIZE_FRAC=";
		keyword += str;
		onelinestr += keyword;
	}

	if(m_bSCH_VisibleGridShow == TRUE)
	{
		onelinestr += "|VISIBLEGRIDON=T";
	}
	i1 = (int)(m_fSCH_VisibleGrid/10);
	i2 = m_fSCH_VisibleGrid*10000;
	i2 = i2 - i1*100000;
	itoa(i1, str, 10);
	keyword = "|VISIBLEGRIDSIZE=";
	keyword += str;
	onelinestr += keyword;
	if(i2 != 0)
	{
		itoa(i2, str, 10);
		keyword = "|VISIBLEGRIDSIZE_FRAC=";
		keyword += str;
		onelinestr += keyword;
	}

	i1 = (int)(m_pSCH_Sheet[nCurrentDiagram]->m_fCustomWidth/10);
	i2 = m_pSCH_Sheet[nCurrentDiagram]->m_fCustomWidth*10000;
	i2 = i2 - i1*100000;
	itoa(i1, str, 10);
	keyword = "|CUSTOMX=";
	keyword += str;
	onelinestr += keyword;
	if(i2 != 0)
	{
		itoa(i2, str, 10);
		keyword = "|CUSTOMX_FRAC=";
		keyword += str;
		onelinestr += keyword;
	}

	i1 = (int)(m_pSCH_Sheet[nCurrentDiagram]->m_fCustomHeight/10);
	i2 = m_pSCH_Sheet[nCurrentDiagram]->m_fCustomHeight*10000;
	i2 = i2 - i1*100000;
	itoa(i1, str, 10);
	keyword = "|CUSTOMY=";
	keyword += str;
	onelinestr += keyword;
	if(i2 != 0)
	{
		itoa(i2, str, 10);
		keyword = "|CUSTOMY_FRAC=";
		keyword += str;
		onelinestr += keyword;
	}

	if(m_pSCH_Sheet[nCurrentDiagram]->m_bCustom == TRUE)
	{
		onelinestr += "|USECUSTOMSHEET=T";
	}

	if(m_pSCH_Sheet[nCurrentDiagram]->m_bShowReferenceZones == FALSE)
	{
		onelinestr += "|REFERENCEZONESON=T";
	}

	keyword = "|CUSTOMXZONES=";
	itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nCustomXRegionCount, str, 10);
	keyword += str;
	onelinestr += keyword;

	keyword = "|CUSTOMYZONES=";
	itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nCustomYRegionCount, str, 10);
	keyword += str;
	onelinestr += keyword;

	i1 = (int)(m_pSCH_Sheet[nCurrentDiagram]->m_fCustomMargin/10);
	i2 = m_pSCH_Sheet[nCurrentDiagram]->m_fCustomMargin*10000;
	i2 = i2 - i1*100000;
	itoa(i1, str, 10);
	keyword = "|CUSTOMMARGINWIDTH=";
	keyword += str;
	onelinestr += keyword;
	if(i2 != 0)
	{
		itoa(i2, str, 10);
		keyword = "|CUSTOMMARGINWIDTH_FRAC=";
		keyword += str;
		onelinestr += keyword;
	}
	keyword = "|DISPLAY_UNIT=";
	itoa(m_nSCH_Unit, str, 10);
	keyword += str;
	onelinestr += keyword;

	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	nLineNo++;
	onelinestr.Empty();
	
	
	CObject* pobject;

	int num = m_listSchObject[nCurrentDiagram].GetCount();
	POSITION pos;
	pos = m_listSchObject[nCurrentDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{
		if(pos == NULL) break;
		pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != nCurrentDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
			
			if((pline->m_nAttrib == LINE_WIRE)||(pline->m_nAttrib == LINE_BUS))
			{
				if(pline->m_nAttrib == LINE_WIRE) onelinestr = "|RECORD=27";
				else onelinestr = "|RECORD=26";

				onelinestr += "|OWNERPARTID=-1";

				if(pline->m_nLineWidth > 0)
				{
					keyword = "|LINEWIDTH=";
					itoa(pline->m_nLineWidth, str, 10);
					keyword += str;
					onelinestr += keyword;
				}

				if(pline->m_nColor > 0)
				{
					keyword = "|COLOR=";
					itoa(pline->m_nColor, str, 10);
					keyword += str;
					onelinestr += keyword;
				}

				keyword = "|LOCATIONCOUNT=";
				itoa(pline->m_cVertex.GetCount(), str, 10);
				keyword += str;
				onelinestr += keyword;

				for(int i=1; i <= pline->m_cVertex.GetCount(); i++)
				{
					itoa(i, str, 10);
					keyword = CString("|X") + str + "=";
					i1 = (int)(pline->m_cVertex[i-1].fx/10);
					i2 = pline->m_cVertex[i-1].fx*10000;
					i2 = i2 - i1*100000;
					itoa(i1, str, 10);
					keyword += str;
					onelinestr += keyword;
					if(i2 != 0)
					{
						itoa(i, str, 10);
						keyword = CString("|X") + str + "_FRAC=";
						itoa(i2, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					itoa(i, str, 10);
					keyword = CString("|Y") + str + "=";
					i1 = (int)(pline->m_cVertex[i-1].fy/10);
					i2 = pline->m_cVertex[i-1].fy*10000;
					i2 = i2 - i1*100000;
					itoa(i1, str, 10);
					keyword += str;
					onelinestr += keyword;
					if(i2 != 0)
					{
						itoa(i, str, 10);
						keyword = CString("|Y") + str + "_FRAC=";
						itoa(i2, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
				}

				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				nLineNo++;
				onelinestr.Empty();
			}
			else if(pline->m_nAttrib == LINE_BUS_ENTRY)
			{
				onelinestr = "|RECORD=37";
				onelinestr += "|OWNERPARTID=-1";

				i1 = (int)(pline->m_cVertex[0].fx/10);
				i2 = pline->m_cVertex[0].fx*10000;
				i2 = i2 - i1*100000;
				itoa(i1, str, 10);
				keyword = "|LOCATION.X=";
				keyword += str;
				onelinestr += keyword;
				if(i2 != 0)
				{
					itoa(i2, str, 10);
					keyword = "|LOCATION.X_FRAC=";
					keyword += str;
					onelinestr += keyword;
				}				

				i1 = (int)(pline->m_cVertex[0].fy/10);
				i2 = pline->m_cVertex[0].fy*10000;
				i2 = i2 - i1*100000;
				itoa(i1, str, 10);
				keyword = "|LOCATION.Y=";
				keyword += str;
				onelinestr += keyword;
				if(i2 != 0)
				{
					itoa(i2, str, 10);
					keyword = "|LOCATION.Y_FRAC=";
					keyword += str;
					onelinestr += keyword;
				}				

				i1 = (int)(pline->m_cVertex[1].fx/10);
				i2 = pline->m_cVertex[1].fx*10000;
				i2 = i2 - i1*100000;
				itoa(i1, str, 10);
				keyword = "|CORNER.X=";
				keyword += str;
				onelinestr += keyword;
				if(i2 != 0)
				{
					itoa(i2, str, 10);
					keyword = "|CORNER.X_FRAC=";
					keyword += str;
					onelinestr += keyword;
				}				

				i1 = (int)(pline->m_cVertex[1].fy/10);
				i2 = pline->m_cVertex[1].fy*10000;
				i2 = i2 - i1*100000;
				itoa(i1, str, 10);
				keyword = "|CORNER.Y=";
				keyword += str;
				onelinestr += keyword;
				if(i2 != 0)
				{
					itoa(i2, str, 10);
					keyword = "|CORNER.Y_FRAC=";
					keyword += str;
					onelinestr += keyword;
				}

				if(pline->m_nLineWidth > 0)
				{
					keyword = "|LINEWIDTH=";
					itoa(pline->m_nLineWidth, str, 10);
					keyword += str;
					onelinestr += keyword;
				}	

				if(pline->m_nColor > 0)
				{
					keyword = "|COLOR=";
					itoa(pline->m_nColor, str, 10);
					keyword += str;
					onelinestr += keyword;
				}				

				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				nLineNo++;
				onelinestr.Empty();
			}

		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != nCurrentDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;

			onelinestr = "|RECORD=6";
			onelinestr += "|OWNERPARTID=-1";

			if(ppolyline->m_nLineWidth > 0)
			{
				keyword = "|LINEWIDTH=";
				itoa(ppolyline->m_nLineWidth, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ppolyline->m_nLineType > 0)
			{
				keyword = "|LINESTYLE=";
				itoa(ppolyline->m_nLineType, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ppolyline->m_nStartLineShape > 0)
			{
				keyword = "|STARTLINESHAPE=";
				itoa(ppolyline->m_nStartLineShape, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ppolyline->m_nEndLineShape > 0)
			{
				keyword = "|ENDLINESHAPE=";
				itoa(ppolyline->m_nEndLineShape, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ppolyline->m_nLineShapeSize > 0)
			{
				keyword = "|LINESHAPESIZE=";
				itoa(ppolyline->m_nLineShapeSize, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ppolyline->m_nColor > 0)
			{
				keyword = "|COLOR=";
				itoa(ppolyline->m_nColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}			

			keyword = "|LOCATIONCOUNT=";
			itoa(ppolyline->m_cVertex.GetCount(), str, 10);
			keyword += str;
			onelinestr += keyword;

			for(int i=1; i <= ppolyline->m_cVertex.GetCount(); i++)
			{
				itoa(i, str, 10);
				keyword = CString("|X") + str + "=";
				i1 = (int)(ppolyline->m_cVertex[i-1].fx/10);
				i2 = ppolyline->m_cVertex[i-1].fx*10000;
				i2 = i2 - i1*100000;
				itoa(i1, str, 10);
				keyword += str;
				onelinestr += keyword;
				if(i2 != 0)
				{
					itoa(i, str, 10);
					keyword = CString("|X") + str + "_FRAC=";
					itoa(i2, str, 10);
					keyword += str;
					onelinestr += keyword;
				}

				itoa(i, str, 10);
				keyword = CString("|Y") + str + "=";
				i1 = (int)(ppolyline->m_cVertex[i-1].fy/10);
				i2 = ppolyline->m_cVertex[i-1].fy*10000;
				i2 = i2 - i1*100000;
				itoa(i1, str, 10);
				keyword += str;
				onelinestr += keyword;
				if(i2 != 0)
				{
					itoa(i, str, 10);
					keyword = CString("|Y") + str + "_FRAC=";
					itoa(i2, str, 10);
					keyword += str;
					onelinestr += keyword;
				}
			}

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			nLineNo++;
			onelinestr.Empty();
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != nCurrentDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = "|RECORD=29";
			onelinestr += "|OWNERPARTID=-1";

			i1 = (int)(pjunc->m_fCx/10);
			i2 = pjunc->m_fCx*10000;
			i2 = i2 - i1*100000;
			itoa(i1, str, 10);
			keyword = "|LOCATION.X=";
			keyword += str;
			onelinestr += keyword;
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(pjunc->m_fCy/10);
			i2 = pjunc->m_fCy*10000;
			i2 = i2 - i1*100000;
			itoa(i1, str, 10);
			keyword = "|LOCATION.Y=";
			keyword += str;
			onelinestr += keyword;
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			if(pjunc->m_nSize > 0)
			{
				keyword = "|SIZE=";
				itoa(pjunc->m_nSize, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(pjunc->m_nColor > 0)
			{
				keyword = "|COLOR=";
				itoa(pjunc->m_nColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(pjunc->m_bManual == TRUE) onelinestr += "|LOCKED=T";
			
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			nLineNo++;
			onelinestr.Empty();
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != nCurrentDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			
			if((ptext->m_nID == TEXT_TEXT)||(ptext->m_nID == TEXT_NETLABEL)||(ptext->m_nID == TEXT_SHEETTEXT))
			{
				if(ptext->m_nID == TEXT_TEXT) onelinestr = "|RECORD=4";
				else if(ptext->m_nID == TEXT_NETLABEL) onelinestr = "|RECORD=25";
				else onelinestr = "|RECORD=41";

				onelinestr += "|OWNERPARTID=-1";

				i1 = (int)(ptext->m_fSx/10);
				i2 = ptext->m_fSx*10000;
				i2 = i2 - i1*100000;
				if(i1 != 0)
				{
					itoa(i1, str, 10);
					keyword = "|LOCATION.X=";
					keyword += str;
					onelinestr += keyword;
				}
				if(i2 != 0)
				{
					itoa(i2, str, 10);
					keyword = "|LOCATION.X_FRAC=";
					keyword += str;
					onelinestr += keyword;
				}				

				i1 = (int)(ptext->m_fSy/10);
				i2 = ptext->m_fSy*10000;
				i2 = i2 - i1*100000;
				if(i1 != 0)
				{
					itoa(i1, str, 10);
					keyword = "|LOCATION.Y=";
					keyword += str;
					onelinestr += keyword;
				}
				if(i2 != 0)
				{
					itoa(i2, str, 10);
					keyword = "|LOCATION.Y_FRAC=";
					keyword += str;
					onelinestr += keyword;
				}

				if((ptext->m_nOrientation/90) > 0)
				{
					keyword = "|ORIENTATION=";
					itoa(ptext->m_nOrientation/90, str, 10);
					keyword += str;
					onelinestr += keyword;
				}

				if( (ptext->m_nH_Justification>0)||(ptext->m_nV_Justification>0))
				{
					keyword = "|JUSTIFICATION=";
					itoa((ptext->m_nH_Justification + ptext->m_nV_Justification*3), str, 10);
					keyword += str;
					onelinestr += keyword;
				}

				if(ptext->m_nColor > 0)
				{
					keyword = "|COLOR=";
					itoa(ptext->m_nColor, str, 10);
					keyword += str;
					onelinestr += keyword;
				}

				keyword = "|FONTID=";
				itoa(ptext->m_nFont+1, str, 10);
				keyword += str;
				onelinestr += keyword;

				if(ptext->m_bTextVisible == FALSE) onelinestr += "|ISHIDDEN=T";

				keyword = "|TEXT=";
				keyword += ptext->m_csText;
				onelinestr += keyword;

				if(ptext->m_bTextVisible == FALSE) onelinestr += "|ISHIDDEN=T";

				if(ptext->m_nParamType > 0)
				{
					keyword = "|PARAMTYPE=";
					itoa(ptext->m_nParamType, str, 10);
					keyword += str;
					onelinestr += keyword;
				}

				if(ptext->m_csNameComment.IsEmpty() == FALSE)
				{
					keyword = "|NAME=";
					keyword += ptext->m_csNameComment;
					onelinestr += keyword;
				}	

				if(ptext->m_bNameVisible == TRUE) onelinestr += "|SHOWNAME=T";

				if( (ptext->m_bNameLocked > 0) || (ptext->m_bTextLocked > 0) )
				{
					keyword = "|READONLYSTATE=";
					itoa((ptext->m_bNameLocked + ptext->m_bTextLocked*2), str, 10);
					keyword += str;
					onelinestr += keyword;	
				}

				if(ptext->m_csUniqueID.IsEmpty() == FALSE)
				{
					keyword = "|UNIQUEID=";
					keyword += ptext->m_csUniqueID;
					onelinestr += keyword;
				}

				if(ptext->m_bAllowLibrarySynchronize == FALSE) onelinestr += "|NOTALLOWLIBRARYSYNCHRONIZE=T";

				if(ptext->m_bAllowDataBaseSynchronize == FALSE) onelinestr += "|NOTALLOWDATABASESYNCHRONIZE=T";

				if(ptext->m_bAutoPosition == FALSE) onelinestr += "|NOTAUTOPOSITION=T";

				if(ptext->m_bMirror == TRUE) onelinestr += "|ISMIRRORED=T";

				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				nLineNo++;
				onelinestr.Empty();
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != nCurrentDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = "|RECORD=12";
			onelinestr += "|OWNERPARTID=-1";

			i1 = (int)(parc->m_fCx/10);
			i2 = parc->m_fCx*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(parc->m_fCy/10);
			i2 = parc->m_fCy*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			i1 = (int)(parc->m_fRadius/10);
			i2 = parc->m_fRadius*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|RADIUS=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|RADIUS_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}	

			if(parc->m_nWidth > 0)
			{
				keyword = "|LINEWIDTH=";
				itoa(parc->m_nWidth, str, 10);
				keyword += str;
				onelinestr += keyword;
			}
			
			if(parc->m_fSangle != 0)
			{
				keyword = "|STARTANGLE=";
				sprintf(str, "%.13f", parc->m_fSangle);
				keyword += str;
				if(keyword.Find('.') > 0)
				{
					keyword.TrimRight('0');
					keyword.TrimRight('.');
				}
				onelinestr += keyword;
			}

			if(parc->m_fEangle != 0)
			{
				keyword = "|ENDANGLE=";
				sprintf(str, "%.13f", parc->m_fEangle);
				keyword += str;
				if(keyword.Find('.') > 0)
				{
					keyword.TrimRight('0');
					keyword.TrimRight('.');
				}
				onelinestr += keyword;
			}

			if(parc->m_nColor > 0)
			{
				keyword = "|COLOR=";
				itoa(parc->m_nColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			nLineNo++;
			onelinestr.Empty();
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != nCurrentDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = "|RECORD=11";
			onelinestr += "|OWNERPARTID=-1";

			i1 = (int)(pellipse->m_fCx/10);
			i2 = pellipse->m_fCx*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(pellipse->m_fCy/10);
			i2 = pellipse->m_fCy*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			i1 = (int)(pellipse->m_fRadius_x/10);
			i2 = pellipse->m_fRadius_x*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|RADIUS=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|RADIUS_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}	

			i1 = (int)(pellipse->m_fRadius_y/10);
			i2 = pellipse->m_fRadius_y*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|SECONDARYRADIUS=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|SECONDARYRADIUS_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}	

			if(pellipse->m_nWidth > 0)
			{
				keyword = "|LINEWIDTH=";
				itoa(pellipse->m_nWidth, str, 10);
				keyword += str;
				onelinestr += keyword;
			}
			
			if(pellipse->m_fSangle != 0)
			{
				keyword = "|STARTANGLE=";
				sprintf(str, "%.13f", pellipse->m_fSangle);
				keyword += str;
				if(keyword.Find('.') > 0)
				{
					keyword.TrimRight('0');
					keyword.TrimRight('.');
				}
				onelinestr += keyword;
			}

			if(pellipse->m_fEangle != 0)
			{
				keyword = "|ENDANGLE=";
				sprintf(str, "%.13f", pellipse->m_fEangle);
				keyword += str;
				if(keyword.Find('.') > 0)
				{
					keyword.TrimRight('0');
					keyword.TrimRight('.');
				}
				onelinestr += keyword;
			}

			if(pellipse->m_nColor > 0)
			{
				keyword = "|COLOR=";
				itoa(pellipse->m_nColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			nLineNo++;
			onelinestr.Empty();
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != nCurrentDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = "|RECORD=8";
			onelinestr += "|OWNERPARTID=-1";

			i1 = (int)(pellipsefill->m_fCx/10);
			i2 = pellipsefill->m_fCx*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(pellipsefill->m_fCy/10);
			i2 = pellipsefill->m_fCy*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			i1 = (int)(pellipsefill->m_fRadius_x/10);
			i2 = pellipsefill->m_fRadius_x*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|RADIUS=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|RADIUS_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}	

			i1 = (int)(pellipsefill->m_fRadius_y/10);
			i2 = pellipsefill->m_fRadius_y*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|SECONDARYRADIUS=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|SECONDARYRADIUS_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}	

			if(pellipsefill->m_nBorderWidth > 0)
			{
				keyword = "|LINEWIDTH=";
				itoa(pellipsefill->m_nBorderWidth, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(pellipsefill->m_nBorderColor > 0)
			{
				keyword = "|COLOR=";
				itoa(pellipsefill->m_nBorderColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(pellipsefill->m_nFillColor > 0)
			{
				keyword = "|AREACOLOR=";
				itoa(pellipsefill->m_nFillColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(pellipsefill->m_bFillSolid == TRUE) onelinestr += "|ISSOLID=T";

			if( pellipsefill->m_bTransparent == TRUE) onelinestr += "|TRANSPARENT=T";
			
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			nLineNo++;
			onelinestr.Empty();
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != nCurrentDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;

			onelinestr = "|RECORD=9";
			onelinestr += "|OWNERPARTID=-1";

			i1 = (int)(ppie->m_fCx/10);
			i2 = ppie->m_fCx*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(ppie->m_fCy/10);
			i2 = ppie->m_fCy*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			i1 = (int)(ppie->m_fRadius/10);
			i2 = ppie->m_fRadius*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|RADIUS=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|RADIUS_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			if(ppie->m_nBorderWidth > 0)
			{
				keyword = "|LINEWIDTH=";
				itoa(ppie->m_nBorderWidth, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ppie->m_fSangle != 0)
			{
				keyword = "|STARTANGLE=";
				sprintf(str, "%.13f", ppie->m_fSangle);
				keyword += str;
				if(keyword.Find('.') > 0)
				{
					keyword.TrimRight('0');
					keyword.TrimRight('.');
				}
				onelinestr += keyword;
			}

			if(ppie->m_fEangle != 0)
			{
				keyword = "|ENDANGLE=";
				sprintf(str, "%.13f", ppie->m_fEangle);
				keyword += str;
				if(keyword.Find('.') > 0)
				{
					keyword.TrimRight('0');
					keyword.TrimRight('.');
				}
				onelinestr += keyword;
			}

			if(ppie->m_nBorderColor > 0)
			{
				keyword = "|COLOR=";
				itoa(ppie->m_nBorderColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ppie->m_nFillColor > 0)
			{
				keyword = "|AREACOLOR=";
				itoa(ppie->m_nFillColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ppie->m_bFillSolid == TRUE) onelinestr += "|ISSOLID=T";

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			nLineNo++;
			onelinestr.Empty();
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != nCurrentDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = "|RECORD=7";
			onelinestr += "|OWNERPARTID=-1";

			if(ppolygon->m_nBorderWidth > 0)
			{
				keyword = "|LINEWIDTH=";
				itoa(ppolygon->m_nBorderWidth, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ppolygon->m_nBorderColor > 0)
			{
				keyword = "|COLOR=";
				itoa(ppolygon->m_nBorderColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ppolygon->m_nFillColor > 0)
			{
				keyword = "|AREACOLOR=";
				itoa(ppolygon->m_nFillColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ppolygon->m_bFillSolid == TRUE) onelinestr += "|ISSOLID=T";

			if( ppolygon->m_bTransparent == TRUE) onelinestr += "|TRANSPARENT=T";

			keyword = "|LOCATIONCOUNT=";
			itoa(ppolygon->m_cVertex.GetCount(), str, 10);
			keyword += str;
			onelinestr += keyword;

			for(int i=1; i <= ppolygon->m_cVertex.GetCount(); i++)
			{
				itoa(i, str, 10);
				keyword = CString("|X") + str + "=";
				i1 = (int)(ppolygon->m_cVertex[i-1].fx/10);
				i2 = ppolygon->m_cVertex[i-1].fx*10000;
				i2 = i2 - i1*100000;
				itoa(i1, str, 10);
				keyword += str;
				onelinestr += keyword;
				if(i2 != 0)
				{
					itoa(i, str, 10);
					keyword = CString("|X") + str + "_FRAC=";
					itoa(i2, str, 10);
					keyword += str;
					onelinestr += keyword;
				}

				itoa(i, str, 10);
				keyword = CString("|Y") + str + "=";
				i1 = (int)(ppolygon->m_cVertex[i-1].fy/10);
				i2 = ppolygon->m_cVertex[i-1].fy*10000;
				i2 = i2 - i1*100000;
				itoa(i1, str, 10);
				keyword += str;
				onelinestr += keyword;
				if(i2 != 0)
				{
					itoa(i, str, 10);
					keyword = CString("|Y") + str + "_FRAC=";
					itoa(i2, str, 10);
					keyword += str;
					onelinestr += keyword;
				}
			}
			
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			nLineNo++;
			onelinestr.Empty();
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != nCurrentDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = "|RECORD=14";
			onelinestr += "|OWNERPARTID=-1";

			i1 = (int)(prect->m_fX1/10);
			i2 = prect->m_fX1*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(prect->m_fY1/10);
			i2 = prect->m_fY1*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			i1 = (int)(prect->m_fX2/10);
			i2 = prect->m_fX2*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|CORNER.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|CORNER.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(prect->m_fY2/10);
			i2 = prect->m_fY2*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|CORNER.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|CORNER.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			if(prect->m_nBorderWidth > 0)
			{
				keyword = "|LINEWIDTH=";
				itoa(prect->m_nBorderWidth, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(prect->m_nBorderColor > 0)
			{
				keyword = "|COLOR=";
				itoa(prect->m_nBorderColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(prect->m_nFillColor > 0)
			{
				keyword = "|AREACOLOR=";
				itoa(prect->m_nFillColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(prect->m_bFillSolid == TRUE) onelinestr += "|ISSOLID=T";

			if(prect->m_bTransparent == TRUE) onelinestr += "|TRANSPARENT=T";
			
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			nLineNo++;
			onelinestr.Empty();
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != nCurrentDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = "|RECORD=10";
			onelinestr += "|OWNERPARTID=-1";

			i1 = (int)(proundrect->m_fX1/10);
			i2 = proundrect->m_fX1*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				
			i1 = (int)(proundrect->m_fY1/10);
			i2 = proundrect->m_fY1*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}
			i1 = (int)(proundrect->m_fX2/10);
			i2 = proundrect->m_fX2*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|CORNER.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|CORNER.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(proundrect->m_fY2/10);
			i2 = proundrect->m_fY2*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|CORNER.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|CORNER.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			i1 = (int)(proundrect->m_fRadius_x/10);
			i2 = proundrect->m_fRadius_x*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|CORNERXRADIUS=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|CORNERXRADIUS_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}	

			i1 = (int)(proundrect->m_fRadius_y/10);
			i2 = proundrect->m_fRadius_y*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|CORNERYRADIUS=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|CORNERYRADIUS_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			if(proundrect->m_nBorderWidth > 0)
			{
				keyword = "|LINEWIDTH=";
				itoa(proundrect->m_nBorderWidth, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(proundrect->m_nBorderColor > 0)
			{
				keyword = "|COLOR=";
				itoa(proundrect->m_nBorderColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(proundrect->m_nFillColor > 0)
			{
				keyword = "|AREACOLOR=";
				itoa(proundrect->m_nFillColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(proundrect->m_bFillSolid == TRUE) onelinestr += "|ISSOLID=T";

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			nLineNo++;
			onelinestr.Empty();
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != nCurrentDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = "RECORD=28";
			onelinestr += "|OWNERPARTID=-1";

			i1 = (int)(ptextframe->m_fX1/10);
			i2 = ptextframe->m_fX1*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(ptextframe->m_fY1/10);
			i2 = ptextframe->m_fY1*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			i1 = (int)(ptextframe->m_fX2/10);
			i2 = ptextframe->m_fX2*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|CORNER.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|CORNER.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(ptextframe->m_fY2/10);
			i2 = ptextframe->m_fY2*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|CORNER.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|CORNER.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			if(ptextframe->m_nBorderWidth > 0)
			{
				keyword = "|LINEWIDTH=";
				itoa(ptextframe->m_nBorderWidth, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ptextframe->m_nBorderColor > 0)
			{
				keyword = "|COLOR=";
				itoa(ptextframe->m_nBorderColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ptextframe->m_nFillColor > 0)
			{
				keyword = "|AREACOLOR=";
				itoa(ptextframe->m_nFillColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ptextframe->m_nTextColor > 0)
			{
				keyword = "|TEXTCOLOR=";
				itoa(ptextframe->m_nTextColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			keyword = "|FONTID=";
			itoa(ptextframe->m_nFont+1, str, 10);
			keyword += str;
			onelinestr += keyword;

			if(ptextframe->m_bFillSolid == TRUE) onelinestr += "|ISSOLID=T";

			if(ptextframe->m_bDrawBorder == TRUE) onelinestr += "|SHOWBORDER=T";

			if(ptextframe->m_nAlignment > 0)
			{
				keyword = "|ALIGNMENT=";
				itoa(ptextframe->m_nAlignment, str, 10);
				keyword += str;
				onelinestr += keyword;
			}
			
			if(ptextframe->m_bWordWrap == TRUE) onelinestr += "|WORDWRAP=T";

			if(ptextframe->m_bClipToArea == TRUE) onelinestr += "|CLIPTORECT=T";

			CString  text;
			keyword = "|Text=";
			for(int i=0; i<ptextframe->m_arrayText.GetCount(); i++)
			{
				keyword += ptextframe->m_arrayText.GetAt(i);
				if(i != (ptextframe->m_arrayText.GetCount()-1)) keyword += "~1";
			}
			onelinestr += keyword;

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			nLineNo++;
			onelinestr.Empty();
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != nCurrentDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = "|RECORD=18";
			onelinestr += "|OWNERPARTID=-1";

			if(pport->m_nStyle > 0)
			{
				keyword = "|STYLE=";
				itoa(pport->m_nStyle, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(pport->m_nType > 0)
			{
				keyword = "|IOTYPE=";
				itoa(pport->m_nType, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(pport->m_nAlignment > 0)
			{
				keyword = "|ALIGNMENT=";
				itoa(pport->m_nAlignment, str, 10);
				keyword += str;
				onelinestr += keyword;
			}
			
			i1 = (int)(pport->m_fSize/10);
			i2 = pport->m_fSize*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|WIDTH=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|WIDTH_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			i1 = (int)(pport->m_fX/10);
			i2 = pport->m_fX*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(pport->m_fY/10);
			i2 = pport->m_fY*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			if(pport->m_nBorderColor > 0)
			{
				keyword = "|COLOR=";
				itoa(pport->m_nBorderColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(pport->m_nFillColor > 0)
			{
				keyword = "|AREACOLOR=";
				itoa(pport->m_nFillColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(pport->m_nTextColor > 0)
			{
				keyword = "|TEXTCOLOR=";
				itoa(pport->m_nTextColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			keyword = "|NAME=";
			keyword += pport->m_csName;
			onelinestr += keyword;
			
			keyword = "|UNIQUEID=";
			keyword += pport->m_csUniqueID;
			onelinestr += keyword;

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			nLineNo++;
			onelinestr.Empty();
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != nCurrentDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = "|RECORD=17";
			onelinestr += "|OWNERPARTID=-1";
			
			if(ppower->m_nStyle > 0)
			{
				keyword = "|STYLE=";
				itoa(ppower->m_nStyle, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ppower->m_bShowNetName == TRUE) onelinestr += "|SHOWNETNAME=T";
			else onelinestr += "|SHOWNETNAME=F";

			i1 = (int)(ppower->m_fX/10);
			i2 = ppower->m_fX*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(ppower->m_fY/10);
			i2 = ppower->m_fY*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			if((ppower->m_nOrientation/90) > 0)
			{
				keyword = "|ORIENTATION=";
				itoa(ppower->m_nOrientation/90, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(ppower->m_nColor > 0)
			{
				keyword = "|COLOR=";
				itoa(ppower->m_nColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}					

			keyword = "|TEXT=";
			keyword += ppower->m_csNetName;
			onelinestr += keyword;

			if(ppower->m_bIsCrossSheetConnector == TRUE) onelinestr += "|ISCROSSSHEETCONNECTOR=T";

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			nLineNo++;
			onelinestr.Empty();
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != nCurrentDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = "|RECORD=5";
			onelinestr += "|OWNERPARTID=-1";

			if(pbezier->m_nWidth > 0)
			{
				keyword = "|LINEWIDTH=";
				itoa(pbezier->m_nWidth, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(pbezier->m_nColor > 0)
			{
				keyword = "|COLOR=";
				itoa(pbezier->m_nColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			keyword = "|LOCATIONCOUNT=";
			itoa(pbezier->m_cVertex.GetCount(), str, 10);
			keyword += str;
			onelinestr += keyword;

			for(int i=1; i <= pbezier->m_cVertex.GetCount(); i++)
			{
				itoa(i, str, 10);
				keyword = CString("|X") + str + "=";
				i1 = (int)(pbezier->m_cVertex[i-1].fx/10);
				i2 = pbezier->m_cVertex[i-1].fx*10000;
				i2 = i2 - i1*100000;
				itoa(i1, str, 10);
				keyword += str;
				onelinestr += keyword;
				if(i2 != 0)
				{
					itoa(i, str, 10);
					keyword = CString("|X") + str + "_FRAC=";
					itoa(i2, str, 10);
					keyword += str;
					onelinestr += keyword;
				}

				itoa(i, str, 10);
				keyword = CString("|Y") + str + "=";
				i1 = (int)(pbezier->m_cVertex[i-1].fy/10);
				i2 = pbezier->m_cVertex[i-1].fy*10000;
				i2 = i2 - i1*100000;
				itoa(i1, str, 10);
				keyword += str;
				onelinestr += keyword;
				if(i2 != 0)
				{
					itoa(i, str, 10);
					keyword = CString("|Y") + str + "_FRAC=";
					itoa(i2, str, 10);
					keyword += str;
					onelinestr += keyword;
				}
			}

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			nLineNo++;
			onelinestr.Empty();
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != nCurrentDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = "|RECORD=30";
			onelinestr += "|OWNERPARTID=-1";

			i1 = (int)(pgraphic->m_fX1/10);
			i2 = pgraphic->m_fX1*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(pgraphic->m_fY1/10);
			i2 = pgraphic->m_fY1*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			i1 = (int)(pgraphic->m_fX2/10);
			i2 = pgraphic->m_fX2*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|CORNER.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|CORNER.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(pgraphic->m_fY2/10);
			i2 = pgraphic->m_fY2*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|CORNER.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|CORNER.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			if(pgraphic->m_nBorderWidth > 0)
			{
				keyword = "|LINEWIDTH=";
				itoa(pgraphic->m_nBorderWidth, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(pgraphic->m_nBorderColor > 0)
			{
				keyword = "|COLOR=";
				itoa(pgraphic->m_nBorderColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(pgraphic->m_bBorderOn == TRUE) onelinestr += "|ISSOLID=T";

			if(pgraphic->m_bAspect == TRUE) onelinestr += "|KEEPASPECT=T";

			if(pgraphic->m_bEmbedded == TRUE) onelinestr += "|EMBEDIMAGE=";

			keyword = "|FILENAME=";
			keyword += pgraphic->m_csFileName;
			onelinestr += keyword;

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			nLineNo++;
			onelinestr.Empty();
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			int delta = 0;
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;
			if((psheetsymbol->m_nDiagram != nCurrentDiagram)||(psheetsymbol->m_nFlag == FLAG_DELETED))
			{
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) m_listSchObject[nCurrentDiagram].GetNext(pos);
				continue;
			}
			
			onelinestr = "|RECORD=15";
			onelinestr += "|OWNERPARTID=-1";
			
			i1 = (int)(psheetsymbol->m_fX/10);
			i2 = psheetsymbol->m_fX*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(psheetsymbol->m_fY/10);
			i2 = psheetsymbol->m_fY*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			i1 = (int)(psheetsymbol->m_fXsize/10);
			i2 = psheetsymbol->m_fXsize*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|XSIZE=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|XSIZE_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(psheetsymbol->m_fYsize/10);
			i2 = psheetsymbol->m_fYsize*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|YSIZE=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|YSIZE_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}

			if(psheetsymbol->m_nBorderWidth > 0)
			{
				keyword = "|LINEWIDTH=";
				itoa(psheetsymbol->m_nBorderWidth, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(psheetsymbol->m_nBorderColor > 0)
			{
				keyword = "|COLOR=";
				itoa(psheetsymbol->m_nBorderColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(psheetsymbol->m_nFillColor > 0)
			{
				keyword = "|AREACOLOR=";
				itoa(psheetsymbol->m_nFillColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(psheetsymbol->m_bFillSolid == TRUE) onelinestr += "|ISSOLID=T";

			if(psheetsymbol->m_bShowHiddenTextFields == TRUE) onelinestr += "|SHOWHIDDENFIELDS=T";

			keyword = "|UNIQUEID=";
			keyword += psheetsymbol->m_csUniqueID;
			onelinestr += keyword;
			
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			delta++;
			onelinestr.Empty();

			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
			{
				pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
				{
					COb_schsheetentry* psheetentry;
					psheetentry = (COb_schsheetentry*)pobject;
					
					onelinestr = "|RECORD=16";
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;
					//Owner Part ID
					onelinestr += "|OWNERPARTID=-1";


					//Entry IOType
					if(psheetentry->m_nType > 0)
					{
						keyword = "|IOTYPE=";
						itoa(psheetentry->m_nType, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					//Entry Style
					if(psheetentry->m_nStyle > 0)
					{
						keyword = "|STYLE=";
						itoa(psheetentry->m_nStyle, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					//Entry Side
					if(psheetentry->m_nSide > 0)
					{
						keyword = "|SIDE=";
						itoa(psheetentry->m_nSide, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(psheetentry->m_fOffset/100); 
					i2 = psheetentry->m_fOffset*10000;
					i2 = i2 - i1*1000000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|DISTANCEFROMTOP=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|DISTANCEFROMTOP_FRAC1=";
						keyword += str;
						onelinestr += keyword;
					}

					if(psheetentry->m_nBorderColor > 0)
					{
						keyword = "|COLOR=";
						itoa(psheetentry->m_nBorderColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(psheetentry->m_nFillColor > 0)
					{
						keyword = "|AREACOLOR=";
						itoa(psheetentry->m_nFillColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(psheetentry->m_nTextColor > 0)
					{
						keyword = "|TEXTCOLOR=";
						itoa(psheetentry->m_nTextColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					keyword = "|NAME=";
					keyword += psheetentry->m_csName;
					onelinestr += keyword;

					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext;
					ptext = (COb_schtext*)pobject;
					
					if(ptext->m_nID == TEXT_SHEETSYMBOLDESIGNATOR) onelinestr = "|RECORD=32";
					else if(ptext->m_nID == TEXT_SHEETSYMBOLFILENAME) onelinestr = "|RECORD=33";
					else onelinestr = "|RECORD=41";

					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;

					onelinestr += "|OWNERPARTID=-1";

					i1 = (int)(ptext->m_fSx/10);
					i2 = ptext->m_fSx*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(ptext->m_fSy/10);
					i2 = ptext->m_fSy*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					if((ptext->m_nOrientation/90) > 0)
					{
						keyword = "|ORIENTATION=";
						itoa(ptext->m_nOrientation/90, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if( (ptext->m_nH_Justification>0)||(ptext->m_nV_Justification>0))
					{
						keyword = "|JUSTIFICATION=";
						itoa((ptext->m_nH_Justification + ptext->m_nV_Justification*3), str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ptext->m_nColor > 0)
					{
						keyword = "|COLOR=";
						itoa(ptext->m_nColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					keyword = "|FONTID=";
					itoa(ptext->m_nFont+1, str, 10);
					keyword += str;
					onelinestr += keyword;

					if(ptext->m_bTextVisible == FALSE) onelinestr += "|ISHIDDEN=T";

					keyword = "|TEXT=";
					keyword += ptext->m_csText;
					onelinestr += keyword;

					if(ptext->m_nParamType > 0)
					{
						keyword = "|PARAMTYPE=";
						itoa(ptext->m_nParamType, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ptext->m_csNameComment.IsEmpty() == FALSE)
					{
						keyword = "|NAME=";
						keyword += ptext->m_csNameComment;
						onelinestr += keyword;
					}	

					if(ptext->m_bNameVisible == TRUE) onelinestr += "|SHOWNAME=T";

					if( (ptext->m_bNameLocked > 0) || (ptext->m_bTextLocked > 0) )
					{
						keyword = "|READONLYSTATE=";
						itoa((ptext->m_bNameLocked + ptext->m_bTextLocked*2), str, 10);
						keyword += str;
						onelinestr += keyword;	
					}

					if(ptext->m_csUniqueID.IsEmpty() == FALSE)
					{
						keyword = "|UNIQUEID=";
						keyword += ptext->m_csUniqueID;
						onelinestr += keyword;
					}

					if(ptext->m_bAllowLibrarySynchronize == FALSE) onelinestr += "|NOTALLOWLIBRARYSYNCHRONIZE=T";

					if(ptext->m_bAllowDataBaseSynchronize == FALSE) onelinestr += "|NOTALLOWDATABASESYNCHRONIZE=T";

					if(ptext->m_bAutoPosition == FALSE) onelinestr += "|NOTAUTOPOSITION=T";

					if(ptext->m_nAnChorH > 0)
					{
						keyword = "|TEXTHORZANCHOR=";
						itoa(ptext->m_nAnChorH, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ptext->m_nAnChorV > 0)
					{
						keyword = "|TEXTVERTANCHOR=";
						itoa(ptext->m_nAnChorV, str, 10);
						keyword += str;
						onelinestr += keyword;					
					}

					if(ptext->m_bMirror == TRUE) onelinestr += "|ISMIRRORED=T";

					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();
				}
			}
			
			nLineNo += delta;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			int delta = 0;
			int nPinLineNo = 0;
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram != nCurrentDiagram)||(pcomp->m_nFlag == FLAG_DELETED))
			{
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) m_listSchObject[nCurrentDiagram].GetNext(pos);

				continue;
			}

			onelinestr = "|RECORD=1";

			if(pcomp->m_csLibraryReference.IsEmpty() == FALSE)
			{
				keyword = "|LIBREFERENCE=";
				keyword += pcomp->m_csLibraryReference;
				onelinestr += keyword;
			}
			
			if(pcomp->m_csDescription.IsEmpty() == FALSE)
			{
				keyword = "|COMPONENTDESCRIPTION=";
				keyword += pcomp->m_csDescription;
				onelinestr += keyword;
			}

			keyword = "|PARTCOUNT=";
			itoa((pcomp->m_nPartCount + 1), str, 10);
			keyword += str;
			onelinestr += keyword;	

			keyword = "|DISPLAYMODECOUNT=";
			itoa(pcomp->m_nDisplayModeCount, str, 10);
			keyword += str;
			onelinestr += keyword;

			onelinestr += "|OWNERPARTID=-1";

			i1 = (int)(pcomp->m_fX/10);
			i2 = pcomp->m_fX*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.X=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.X_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}				

			i1 = (int)(pcomp->m_fY/10);
			i2 =pcomp->m_fY*10000;
			i2 = i2 - i1*100000;
			if(i1 != 0)
			{
				itoa(i1, str, 10);
				keyword = "|LOCATION.Y=";
				keyword += str;
				onelinestr += keyword;
			}
			if(i2 != 0)
			{
				itoa(i2, str, 10);
				keyword = "|LOCATION.Y_FRAC=";
				keyword += str;
				onelinestr += keyword;
			}
			
			if(pcomp->m_nDisplayMode > 1)
			{
				keyword = "|DISPLAYMODE=";
				itoa(pcomp->m_nDisplayMode, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(pcomp->m_bMirrored == TRUE) onelinestr += "|ISMIRRORED=T";

			if( (pcomp->m_nOrientation/90) > 0)
			{
				keyword = "|ORIENTATION=";
				itoa((pcomp->m_nOrientation/90), str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			keyword = "|CURRENTPARTID=";
			itoa(pcomp->m_nPart, str, 10);
			keyword += str;
			onelinestr += keyword;

			if(pcomp->m_bShowHidePins == TRUE) onelinestr += "|SHOWHIDDENPINS=T";
			
			if(pcomp->m_csLibraryPath.IsEmpty() == FALSE)
			{
				keyword = "|LIBRARYPATH=";
				keyword += pcomp->m_csLibraryPath;
				onelinestr += keyword;
			}

			if(pcomp->m_csLibraryFileName.IsEmpty() == FALSE)
			{
				keyword = "|SOURCELIBRARYNAME=";
				keyword += pcomp->m_csLibraryFileName;
				onelinestr += keyword;
			}

			keyword = "|TARGETFILENAME=";
			keyword += pcomp->m_csTargetFileName;
			onelinestr += keyword;

			keyword = "|UNIQUEID=";
			keyword += pcomp->m_csUniqueID;
			onelinestr += keyword;

			if(pcomp->m_nFillColor > 0)
			{
				keyword = "|AREACOLOR=";
				itoa(pcomp->m_nFillColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}		
			if(pcomp->m_nColor > 0)
			{
				keyword = "|COLOR=";
				itoa(pcomp->m_nColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}
			if(pcomp->m_nPinColor > 0)
			{
				keyword = "|PINCOLOR=";
				itoa(pcomp->m_nPinColor, str, 10);
				keyword += str;
				onelinestr += keyword;
			}

			if(pcomp->m_bColorLocked == TRUE) onelinestr += "|OVERIDECOLORS=T";

			if(pcomp->m_bDesignatorLocked == TRUE) onelinestr += "|DESIGNATORLOCKED=T";

			if(pcomp->m_bPartLocked == TRUE) onelinestr += "|PARTIDLOCKED=T";
			else onelinestr += "|PARTIDLOCKED=F";

			if(pcomp->m_bPinsMoveable == TRUE) onelinestr += "|PINSMOVEABLE=T";

			if(pcomp->m_nGraphicalType > 4)
			{
				keyword = "|COMPONENTKINDVERSION2=";
				itoa(pcomp->m_nGraphicalType, str, 10);
				keyword += str;
				onelinestr += keyword;
			}
			else if(pcomp->m_nGraphicalType > 0)
			{
				keyword = "|COMPONENTKIND=";
				itoa(pcomp->m_nGraphicalType, str, 10);
				keyword += str;
				onelinestr += keyword;
			}
				
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			delta++;
			onelinestr.Empty();

			POSITION posbak = pos;
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
			{
				pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
				{
					COb_schpin* ppin = (COb_schpin*)pobject;

					onelinestr = "|RECORD=2";
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;
					
					if(ppin->m_nOwnerPartID != 0)
					{
						keyword = "|OWNERPARTID=";
						itoa(ppin->m_nOwnerPartID, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ppin->m_nDisplayMode > 0)
					{
						keyword = "|OWNERPARTDISPLAYMODE=";
						itoa(ppin->m_nDisplayMode, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					
					if(ppin->m_nInsideEdge > 0)
					{
						keyword = "|SYMBOL_INNEREDGE=";
						itoa(ppin->m_nInsideEdge, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					if(ppin->m_nOutsideEdge > 0)
					{
						keyword = "|SYMBOL_OUTEREDGE=";
						itoa(ppin->m_nOutsideEdge, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					if(ppin->m_nInside > 0)
					{
						keyword = "|SYMBOL_INNER=";
						itoa(ppin->m_nInside, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					if(ppin->m_nOutside > 0)
					{
						keyword = "|SYMBOL_OUTER=";
						itoa(ppin->m_nOutside, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					
					//Description
					if(ppin->m_csDescription.IsEmpty() != TRUE)
					{
						keyword = "|DESCRIPTION=";
						keyword += ppin->m_csDescription;
						onelinestr += keyword;
					}

					onelinestr += "|FORMALTYPE=1";
					
					if(ppin->m_nElectricalType > 0)
					{
						keyword = "|ELECTRICAL=";
						itoa(ppin->m_nElectricalType, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					
					int pin_conglomerate;
					pin_conglomerate = ppin->m_nOrientation/90;
					if(ppin->m_bHide == TRUE) pin_conglomerate += 0x04;
					if(ppin->m_bNameVisible == TRUE) pin_conglomerate += 0x08;
					if(ppin->m_bNumberVisible == TRUE) pin_conglomerate += 0x10;
					if(ppin->m_bPinLocked == TRUE) pin_conglomerate += 0x20;
					if(pin_conglomerate > 0)
					{
						keyword = "|PINCONGLOMERATE=";
						itoa(pin_conglomerate, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(ppin->m_fPinLength/10);
					i2 = ppin->m_fPinLength*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|PINLENGTH=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|PINLENGTH_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}	

					i1 = (int)(ppin->m_fX/10);
					i2 = ppin->m_fX*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(ppin->m_fY/10);
					i2 =ppin->m_fY*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					if(ppin->m_nColor > 0)
					{
						keyword = "|COLOR=";
						itoa(ppin->m_nColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					keyword = "|NAME=";
					keyword += ppin->m_csName;
					onelinestr += keyword;

					keyword = "|DESIGNATOR=";
					keyword += ppin->m_csNumber;
					onelinestr += keyword;
			
					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();

					nPinLineNo = nLineNo + delta - 1;

					if(ppin->m_csConnectTo.IsEmpty() == FALSE)
					{
						onelinestr = "|RECORD=41";
					
						//OwnerIndex
						keyword = "|OWNERINDEX=";
						itoa(nPinLineNo, str, 10);
						keyword += str;
						onelinestr += keyword;

						keyword = "|OWNERPARTID=-1|COLOR=8388608|FONTID=1|TEXT=";
						keyword += ppin->m_csConnectTo;
						keyword += "|NAME=HiddenNetName";
						onelinestr += keyword;

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						delta++;
						onelinestr.Empty();
					}

					onelinestr = "|RECORD=41";
					
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nPinLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;

					//Owner Part ID
					onelinestr += "|OWNERPARTID=-1";

					i1 = (int)(ppin->m_fX/10);
					i2 = ppin->m_fX*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(ppin->m_fY/10);
					i2 =ppin->m_fY*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}
					
					onelinestr += "|COLOR=8388608|FONTID=1|ISHIDDEN=T";
					
					if(ppin->m_csUniqueID.IsEmpty() == FALSE)
					{
						keyword = "|TEXT=";
						keyword += ppin->m_csUniqueID;
						onelinestr += keyword;
					}

					onelinestr += "|ISHIDDEN=T|NAME=PinUniqueId";
					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
				{
					COb_schieee* pieee = (COb_schieee*)pobject;
					
					onelinestr = "|RECORD=3";
					
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;

					//not accesssible
					onelinestr += "|ISNOTACCESIBLE=T";

					if(pieee->m_nOwnerPartID > 0)
					{
						keyword = "|OWNERPARTID=";
						itoa(pieee->m_nOwnerPartID, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(pieee->m_nDisplayMode > 0)
					{
						keyword = "|OWNERPARTDISPLAYMODE=";
						itoa(pieee->m_nDisplayMode, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					
					if(pieee->m_nIeeeSymbol > 0)
					{
						keyword = "|SYMBOL=";
						itoa(pieee->m_nIeeeSymbol, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(pieee->m_fX/10);
					i2 = pieee->m_fX*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(pieee->m_fY/10);
					i2 =pieee->m_fY*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(pieee->m_fSize);
					i2 = pieee->m_fSize*10000;
					i2 = i2 - i1*10000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|SCALEFACTOR=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|SCALEFACTOR_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					if((pieee->m_nOrientation/90) > 0)
					{
						keyword = "|ORIENTATION=";
						itoa((pieee->m_nOrientation/90), str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(pieee->m_nWidth > 0)
					{
						keyword = "|LINEWIDTH=";
						itoa(pieee->m_nWidth, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(pieee->m_nColor > 0)
					{
						keyword = "|COLOR=";
						itoa(pieee->m_nColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(pieee->m_bMirrored == TRUE) onelinestr += "|MIRROR=T";

					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
				{
					COb_scharc* parc = (COb_scharc*)pobject;

					onelinestr = "|RECORD=12";
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;
					//not accesssible
					onelinestr += "|ISNOTACCESIBLE=T";

					if(parc->m_nOwnerPartID > 0)
					{
						keyword = "|OWNERPARTID=";
						itoa(parc->m_nOwnerPartID, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(parc->m_nDisplayMode > 0)
					{
						keyword = "|OWNERPARTDISPLAYMODE=";
						itoa(parc->m_nDisplayMode, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(parc->m_fCx/10);
					i2 = parc->m_fCx*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(parc->m_fCy/10);
					i2 = parc->m_fCy*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(parc->m_fRadius/10);
					i2 = parc->m_fRadius*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|RADIUS=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|RADIUS_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}	

					if(parc->m_nWidth > 0)
					{
						keyword = "|LINEWIDTH=";
						itoa(parc->m_nWidth, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					
					if(parc->m_fSangle != 0)
					{
						keyword = "|STARTANGLE=";
						sprintf(str, "%.13f", parc->m_fSangle);
						keyword += str;
						if(keyword.Find('.') > 0)
						{
							keyword.TrimRight('0');
							keyword.TrimRight('.');
						}
						onelinestr += keyword;
					}

					if(parc->m_fEangle != 0)
					{
						keyword = "|ENDANGLE=";
						sprintf(str, "%.13f", parc->m_fEangle);
						keyword += str;
						if(keyword.Find('.') > 0)
						{
							keyword.TrimRight('0');
							keyword.TrimRight('.');
						}
						onelinestr += keyword;
					}
					if(parc->m_nColor > 0)
					{
						keyword = "|COLOR=";
						itoa(parc->m_nColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
				{
					COb_schellipse* pellipse = (COb_schellipse*)pobject;

					onelinestr = "|RECORD=11";
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;
					//not accesssible
					onelinestr += "|ISNOTACCESIBLE=T";
					if(pellipse->m_nOwnerPartID > 0)
					{
						keyword = "|OWNERPARTID=";
						itoa(pellipse->m_nOwnerPartID, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(pellipse->m_nDisplayMode > 0)
					{
						keyword = "|OWNERPARTDISPLAYMODE=";
						itoa(pellipse->m_nDisplayMode, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(pellipse->m_fCx/10);
					i2 = pellipse->m_fCx*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(pellipse->m_fCy/10);
					i2 = pellipse->m_fCy*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(pellipse->m_fRadius_x/10);
					i2 = pellipse->m_fRadius_x*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|RADIUS=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|RADIUS_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}	

					i1 = (int)(pellipse->m_fRadius_y/10);
					i2 = pellipse->m_fRadius_y*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|SECONDARYRADIUS=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|SECONDARYRADIUS_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}	

					if(pellipse->m_nWidth > 0)
					{
						keyword = "|LINEWIDTH=";
						itoa(pellipse->m_nWidth, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					
					if(pellipse->m_fSangle != 0)
					{
						keyword = "|STARTANGLE=";
						sprintf(str, "%.13f", pellipse->m_fSangle);
						keyword += str;
						if(keyword.Find('.') > 0)
						{
							keyword.TrimRight('0');
							keyword.TrimRight('.');
						}
						onelinestr += keyword;
					}

					if(pellipse->m_fEangle != 0)
					{
						keyword = "|ENDANGLE=";
						sprintf(str, "%.13f", pellipse->m_fEangle);
						keyword += str;
						if(keyword.Find('.') > 0)
						{
							keyword.TrimRight('0');
							keyword.TrimRight('.');
						}
						onelinestr += keyword;
					}

					if(pellipse->m_nColor > 0)
					{
						keyword = "|COLOR=";
						itoa(pellipse->m_nColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();

				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
				{
					COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;

					onelinestr = "|RECORD=8";
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;
					//not accesssible
					onelinestr += "|ISNOTACCESIBLE=T";

					if(pellipsefill->m_nOwnerPartID > 0)
					{
						keyword = "|OWNERPARTID=";
						itoa(pellipsefill->m_nOwnerPartID, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(pellipsefill->m_nDisplayMode > 0)
					{
						keyword = "|OWNERPARTDISPLAYMODE=";
						itoa(pellipsefill->m_nDisplayMode, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(pellipsefill->m_fCx/10);
					i2 = pellipsefill->m_fCx*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(pellipsefill->m_fCy/10);
					i2 = pellipsefill->m_fCy*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(pellipsefill->m_fRadius_x/10);
					i2 = pellipsefill->m_fRadius_x*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|RADIUS=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|RADIUS_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}	

					i1 = (int)(pellipsefill->m_fRadius_y/10);
					i2 = pellipsefill->m_fRadius_y*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|SECONDARYRADIUS=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|SECONDARYRADIUS_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}	

					if(pellipsefill->m_nBorderWidth > 0)
					{
						keyword = "|LINEWIDTH=";
						itoa(pellipsefill->m_nBorderWidth, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(pellipsefill->m_nBorderColor > 0)
					{
						keyword = "|COLOR=";
						itoa(pellipsefill->m_nBorderColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(pellipsefill->m_nFillColor > 0)
					{
						keyword = "|AREACOLOR=";
						itoa(pellipsefill->m_nFillColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(pellipsefill->m_bFillSolid == TRUE) onelinestr += "|ISSOLID=T";

					if( pellipsefill->m_bTransparent == TRUE) onelinestr += "|TRANSPARENT=T";
					
					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
				{
					COb_schpie* ppie = (COb_schpie*)pobject;

					onelinestr = "|RECORD=9";
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;
					//not accesssible
					onelinestr += "|ISNOTACCESIBLE=T";

					if(ppie->m_nOwnerPartID > 0)
					{
						keyword = "|OWNERPARTID=";
						itoa(ppie->m_nOwnerPartID, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ppie->m_nDisplayMode > 0)
					{
						keyword = "|OWNERPARTDISPLAYMODE=";
						itoa(ppie->m_nDisplayMode, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(ppie->m_fCx/10);
					i2 = ppie->m_fCx*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(ppie->m_fCy/10);
					i2 = ppie->m_fCy*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(ppie->m_fRadius/10);
					i2 = ppie->m_fRadius*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|RADIUS=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|RADIUS_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					if(ppie->m_nBorderWidth > 0)
					{
						keyword = "|LINEWIDTH=";
						itoa(ppie->m_nBorderWidth, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ppie->m_fSangle != 0)
					{
						keyword = "|STARTANGLE=";
						sprintf(str, "%.13f", ppie->m_fSangle);
						keyword += str;
						if(keyword.Find('.') > 0)
						{
							keyword.TrimRight('0');
							keyword.TrimRight('.');
						}
						onelinestr += keyword;
					}

					if(ppie->m_fEangle != 0)
					{
						keyword = "|ENDANGLE=";
						sprintf(str, "%.13f", ppie->m_fEangle);
						keyword += str;
						if(keyword.Find('.') > 0)
						{
							keyword.TrimRight('0');
							keyword.TrimRight('.');
						}
						onelinestr += keyword;
					}

					if(ppie->m_nBorderColor > 0)
					{
						keyword = "|COLOR=";
						itoa(ppie->m_nBorderColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ppie->m_nFillColor > 0)
					{
						keyword = "|AREACOLOR=";
						itoa(ppie->m_nFillColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ppie->m_bFillSolid == TRUE) onelinestr += "|ISSOLID=T";

					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
				{
					COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;

					onelinestr = "|RECORD=6";
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;
					//not accesssible
					onelinestr += "|ISNOTACCESIBLE=T";
						keyword = "|OWNERPARTID=";
						itoa(ppolyline->m_nOwnerPartID, str, 10);
						keyword += str;
						onelinestr += keyword;

					if(ppolyline->m_nDisplayMode > 0)
					{
						keyword = "|OWNERPARTDISPLAYMODE=";
						itoa(ppolyline->m_nDisplayMode, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ppolyline->m_nLineWidth > 0)
					{
						keyword = "|LINEWIDTH=";
						itoa(ppolyline->m_nLineWidth, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ppolyline->m_nLineType > 0)
					{
						keyword = "|LINESTYLE=";
						itoa(ppolyline->m_nLineType, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ppolyline->m_nStartLineShape > 0)
					{
						keyword = "|STARTLINESHAPE=";
						itoa(ppolyline->m_nStartLineShape, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ppolyline->m_nEndLineShape > 0)
					{
						keyword = "|ENDLINESHAPE=";
						itoa(ppolyline->m_nEndLineShape, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					//line shape size
					if(ppolyline->m_nLineShapeSize > 0)
					{
						keyword = "|LINESHAPESIZE=";
						itoa(ppolyline->m_nLineShapeSize, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					keyword = "|COLOR=";
					itoa(ppolyline->m_nColor, str, 10);
					keyword += str;
					onelinestr += keyword;


					keyword = "|LOCATIONCOUNT=";
					itoa(ppolyline->m_cVertex.GetCount(), str, 10);
					keyword += str;
					onelinestr += keyword;

					for(int i=1; i <= ppolyline->m_cVertex.GetCount(); i++)
					{
						itoa(i, str, 10);
						keyword = CString("|X") + str + "=";
						i1 = (int)(ppolyline->m_cVertex[i-1].fx/10);
						i2 = ppolyline->m_cVertex[i-1].fx*10000;
						i2 = i2 - i1*100000;
						itoa(i1, str, 10);
						keyword += str;
						onelinestr += keyword;
						if(i2 != 0)
						{
							itoa(i, str, 10);
							keyword = CString("|X") + str + "_FRAC=";
							itoa(i2, str, 10);
							keyword += str;
							onelinestr += keyword;
						}

						itoa(i, str, 10);
						keyword = CString("|Y") + str + "=";
						i1 = (int)(ppolyline->m_cVertex[i-1].fy/10);
						i2 = ppolyline->m_cVertex[i-1].fy*10000;
						i2 = i2 - i1*100000;
						itoa(i1, str, 10);
						keyword += str;
						onelinestr += keyword;
						if(i2 != 0)
						{
							itoa(i, str, 10);
							keyword = CString("|Y") + str + "_FRAC=";
							itoa(i2, str, 10);
							keyword += str;
							onelinestr += keyword;
						}
					}

					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();

				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
				{
					COb_schrect* prect = (COb_schrect*)pobject;

					onelinestr = "|RECORD=14";
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;
					//not accesssible
					onelinestr += "|ISNOTACCESIBLE=T";

					if(prect->m_nOwnerPartID > 0)
					{
						keyword = "|OWNERPARTID=";
						itoa(prect->m_nOwnerPartID, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(prect->m_nDisplayMode > 0)
					{
						keyword = "|OWNERPARTDISPLAYMODE=";
						itoa(prect->m_nDisplayMode, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(prect->m_fX1/10);
					i2 = prect->m_fX1*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(prect->m_fY1/10);
					i2 = prect->m_fY1*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(prect->m_fX2/10);
					i2 = prect->m_fX2*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|CORNER.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|CORNER.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(prect->m_fY2/10);
					i2 = prect->m_fY2*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|CORNER.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|CORNER.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					if(prect->m_nBorderWidth > 0)
					{
						keyword = "|LINEWIDTH=";
						itoa(prect->m_nBorderWidth, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(prect->m_nBorderColor > 0)
					{
						keyword = "|COLOR=";
						itoa(prect->m_nBorderColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(prect->m_nFillColor > 0)
					{
						keyword = "|AREACOLOR=";
						itoa(prect->m_nFillColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(prect->m_bFillSolid == TRUE) onelinestr += "|ISSOLID=T";

					if(prect->m_bTransparent == TRUE) onelinestr += "|TRANSPARENT=T";
					
					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
				{
					COb_schroundrect* proundrect = (COb_schroundrect*)pobject;

					onelinestr = "|RECORD=10";
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;
					//not accesssible
					onelinestr += "|ISNOTACCESIBLE=T";

					if(proundrect->m_nOwnerPartID > 0)
					{
						keyword = "|OWNERPARTID=";
						itoa(proundrect->m_nOwnerPartID, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(proundrect->m_nDisplayMode > 0)
					{
						keyword = "|OWNERPARTDISPLAYMODE=";
						itoa(proundrect->m_nDisplayMode, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(proundrect->m_fX1/10);
					i2 = proundrect->m_fX1*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(proundrect->m_fY1/10);
					i2 = proundrect->m_fY1*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(proundrect->m_fX2/10);
					i2 = proundrect->m_fX2*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|CORNER.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|CORNER.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(proundrect->m_fY2/10);
					i2 = proundrect->m_fY2*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|CORNER.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|CORNER.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(proundrect->m_fRadius_x/10);
					i2 = proundrect->m_fRadius_x*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|CORNERXRADIUS=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|CORNERXRADIUS_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}	

					i1 = (int)(proundrect->m_fRadius_y/10);
					i2 = proundrect->m_fRadius_y*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|CORNERYRADIUS=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|CORNERYRADIUS_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					if(proundrect->m_nBorderWidth > 0)
					{
						keyword = "|LINEWIDTH=";
						itoa(proundrect->m_nBorderWidth, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(proundrect->m_nBorderColor > 0)
					{
						keyword = "|COLOR=";
						itoa(proundrect->m_nBorderColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(proundrect->m_nFillColor > 0)
					{
						keyword = "|AREACOLOR=";
						itoa(proundrect->m_nFillColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(proundrect->m_bFillSolid == TRUE) onelinestr += "|ISSOLID=T";

					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
				{
					COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;

					onelinestr = "|RECORD=7";
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;
					//not accesssible
					onelinestr += "|ISNOTACCESIBLE=T";

					if(ppolygon->m_nOwnerPartID > 0)
					{
						keyword = "|OWNERPARTID=";
						itoa(ppolygon->m_nOwnerPartID, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ppolygon->m_nDisplayMode > 0)
					{
						keyword = "|OWNERPARTDISPLAYMODE=";
						itoa(ppolygon->m_nDisplayMode, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ppolygon->m_nBorderWidth > 0)
					{
						keyword = "|LINEWIDTH=";
						itoa(ppolygon->m_nBorderWidth, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ppolygon->m_nBorderColor > 0)
					{
						keyword = "|COLOR=";
						itoa(ppolygon->m_nBorderColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ppolygon->m_nFillColor > 0)
					{
						keyword = "|AREACOLOR=";
						itoa(ppolygon->m_nFillColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ppolygon->m_bFillSolid == TRUE) onelinestr += "|ISSOLID=T";

					if( ppolygon->m_bTransparent == TRUE) onelinestr += "|TRANSPARENT=T";

					keyword = "|LOCATIONCOUNT=";
					itoa(ppolygon->m_cVertex.GetCount(), str, 10);
					keyword += str;
					onelinestr += keyword;

					for(int i=1; i <= ppolygon->m_cVertex.GetCount(); i++)
					{
						itoa(i, str, 10);
						keyword = CString("|X") + str + "=";
						i1 = (int)(ppolygon->m_cVertex[i-1].fx/10);
						i2 = ppolygon->m_cVertex[i-1].fx*10000;
						i2 = i2 - i1*100000;
						itoa(i1, str, 10);
						keyword += str;
						onelinestr += keyword;
						if(i2 != 0)
						{
							itoa(i, str, 10);
							keyword = CString("|X") + str + "_FRAC=";
							itoa(i2, str, 10);
							keyword += str;
							onelinestr += keyword;
						}

						itoa(i, str, 10);
						keyword = CString("|Y") + str + "=";
						i1 = (int)(ppolygon->m_cVertex[i-1].fy/10);
						i2 = ppolygon->m_cVertex[i-1].fy*10000;
						i2 = i2 - i1*100000;
						itoa(i1, str, 10);
						keyword += str;
						onelinestr += keyword;
						if(i2 != 0)
						{
							itoa(i, str, 10);
							keyword = CString("|Y") + str + "_FRAC=";
							itoa(i2, str, 10);
							keyword += str;
							onelinestr += keyword;
						}
					}
					
					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
				{
					COb_schbezier* pbezier = (COb_schbezier*)pobject;

					onelinestr = "|RECORD=5";
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;
					//not accesssible
					onelinestr += "|ISNOTACCESIBLE=T";
					if(pbezier->m_nOwnerPartID > 0)
					{
						keyword = "|OWNERPARTID=";
						itoa(pbezier->m_nOwnerPartID, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					if(pbezier->m_nDisplayMode > 0)
					{
						keyword = "|OWNERPARTDISPLAYMODE=";
						itoa(pbezier->m_nDisplayMode, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(pbezier->m_nWidth > 0)
					{
						keyword = "|LINEWIDTH=";
						itoa(pbezier->m_nWidth, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(pbezier->m_nColor > 0)
					{
						keyword = "|COLOR=";
						itoa(pbezier->m_nColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					keyword = "|LOCATIONCOUNT=";
					itoa(pbezier->m_cVertex.GetCount(), str, 10);
					keyword += str;
					onelinestr += keyword;

					for(int i=1; i <= pbezier->m_cVertex.GetCount(); i++)
					{
						itoa(i, str, 10);
						keyword = CString("|X") + str + "=";
						i1 = (int)(pbezier->m_cVertex[i-1].fx/10);
						i2 = pbezier->m_cVertex[i-1].fx*10000;
						i2 = i2 - i1*100000;
						itoa(i1, str, 10);
						keyword += str;
						onelinestr += keyword;
						if(i2 != 0)
						{
							itoa(i, str, 10);
							keyword = CString("|X") + str + "_FRAC=";
							itoa(i2, str, 10);
							keyword += str;
							onelinestr += keyword;
						}

						itoa(i, str, 10);
						keyword = CString("|Y") + str + "=";
						i1 = (int)(pbezier->m_cVertex[i-1].fy/10);
						i2 = pbezier->m_cVertex[i-1].fy*10000;
						i2 = i2 - i1*100000;
						itoa(i1, str, 10);
						keyword += str;
						onelinestr += keyword;
						if(i2 != 0)
						{
							itoa(i, str, 10);
							keyword = CString("|Y") + str + "_FRAC=";
							itoa(i2, str, 10);
							keyword += str;
							onelinestr += keyword;
						}
					}

					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
				{
					COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;

					onelinestr = "RECORD=28";
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;
					//not accesssible
					onelinestr += "|ISNOTACCESIBLE=T";

					if(ptextframe->m_nOwnerPartID > 0)
					{
						keyword = "|OWNERPARTID=";
						itoa(ptextframe->m_nOwnerPartID, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ptextframe->m_nDisplayMode > 0)
					{
						keyword = "|OWNERPARTDISPLAYMODE=";
						itoa(ptextframe->m_nDisplayMode, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(ptextframe->m_fX1/10);
					i2 = ptextframe->m_fX1*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(ptextframe->m_fY1/10);
					i2 = ptextframe->m_fY1*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(ptextframe->m_fX2/10);
					i2 = ptextframe->m_fX2*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|CORNER.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|CORNER.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(ptextframe->m_fY2/10);
					i2 = ptextframe->m_fY2*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|CORNER.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|CORNER.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					if(ptextframe->m_nBorderWidth > 0)
					{
						keyword = "|LINEWIDTH=";
						itoa(ptextframe->m_nBorderWidth, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ptextframe->m_nBorderColor > 0)
					{
						keyword = "|COLOR=";
						itoa(ptextframe->m_nBorderColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ptextframe->m_nFillColor > 0)
					{
						keyword = "|AREACOLOR=";
						itoa(ptextframe->m_nFillColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ptextframe->m_nTextColor > 0)
					{
						keyword = "|TEXTCOLOR=";
						itoa(ptextframe->m_nTextColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					//FONTID
					keyword = "|FONTID=";
					itoa(ptextframe->m_nFont+1, str, 10);
					keyword += str;
					onelinestr += keyword;

					if(ptextframe->m_bFillSolid == TRUE) onelinestr += "|ISSOLID=T";

					if(ptextframe->m_bDrawBorder == TRUE) onelinestr += "|SHOWBORDER=T";

					if(ptextframe->m_nAlignment > 0)
					{
						keyword = "|ALIGNMENT=";
						itoa(ptextframe->m_nAlignment, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					
					if(ptextframe->m_bWordWrap == TRUE) onelinestr += "|WORDWRAP=T";

					if(ptextframe->m_bClipToArea == TRUE) onelinestr += "|CLIPTORECT=T";

					CString  text;
					keyword = "|Text=";
					for(int i=0; i<ptextframe->m_arrayText.GetCount(); i++)
					{
						keyword += ptextframe->m_arrayText.GetAt(i);
						if(i != (ptextframe->m_arrayText.GetCount()-1)) keyword += "~1";
					}
					onelinestr += keyword;

					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();

				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
				{
					COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;

					onelinestr = "|RECORD=30";
					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;
					//not accesssible
					onelinestr += "|ISNOTACCESIBLE=T";
					if(pgraphic->m_nOwnerPartID > 0)
					{
						keyword = "|OWNERPARTID=";
						itoa(pgraphic->m_nOwnerPartID, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					if(pgraphic->m_nDisplayMode > 0)
					{
						keyword = "|OWNERPARTDISPLAYMODE=";
						itoa(pgraphic->m_nDisplayMode, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(pgraphic->m_fX1/10);
					i2 = pgraphic->m_fX1*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(pgraphic->m_fY1/10);
					i2 = pgraphic->m_fY1*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					i1 = (int)(pgraphic->m_fX2/10);
					i2 = pgraphic->m_fX2*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|CORNER.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|CORNER.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(pgraphic->m_fY2/10);
					i2 = pgraphic->m_fY2*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|CORNER.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|CORNER.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					if(pgraphic->m_nBorderWidth > 0)
					{
						keyword = "|LINEWIDTH=";
						itoa(pgraphic->m_nBorderWidth, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(pgraphic->m_nBorderColor > 0)
					{
						keyword = "|COLOR=";
						itoa(pgraphic->m_nBorderColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(pgraphic->m_bBorderOn == TRUE) onelinestr += "|ISSOLID=T";

					if(pgraphic->m_bAspect == TRUE) onelinestr += "|KEEPASPECT=T";

					if(pgraphic->m_bEmbedded == TRUE) onelinestr += "|EMBEDIMAGE=";

					keyword = "|FILENAME=";
					keyword += pgraphic->m_csFileName;
					onelinestr += keyword;

					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();

				}
			}
			pos = posbak;
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
			{
				pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext = (COb_schtext*)pobject;
					if(ptext->m_nID == TEXT_TEXT) onelinestr = "|RECORD=4";
					else if(ptext->m_nID == TEXT_COMPDESIGNATOR) onelinestr = "|RECORD=34";
					else if(ptext->m_nID == TEXT_COMPPACKAGE)
					{
						int ntemp = nLineNo + delta;
						
						keyword = "|RECORD=44|OWNERINDEX=";

						//OwnerIndex
						itoa(nLineNo, str, 10);
						keyword += str;
						onelinestr += keyword;

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						delta++;
						onelinestr.Empty();

						keyword = "|RECORD=45|OWNERINDEX=";
						//OwnerIndex
						itoa(ntemp, str, 10);
						keyword += str;
						onelinestr += keyword;

						onelinestr += "|DESCRIPTION=";
						onelinestr += ptext->m_csNameComment;

						onelinestr += "|MODELNAME=";
						onelinestr += ptext->m_csText;

						onelinestr += "|MODELTYPE=PCBLIB|DATAFILECOUNT=1|MODELDATAFILEENTITY0=";
						onelinestr += ptext->m_csText;

						onelinestr += "|MODELDATAFILEKIND0=PCBLib|ISCURRENT=T|DATALINKSLOCKED=T|INTEGRATEDMODEL=T";

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						delta++;
						onelinestr.Empty();

						keyword = "|RECORD=46|OWNERINDEX=";
						//OwnerIndex
						itoa(ntemp+1, str, 10);
						keyword += str;
						onelinestr += keyword;
						
						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						delta++;
						onelinestr.Empty();

						keyword = "|RECORD=48|OWNERINDEX=";
						//OwnerIndex
						itoa(ntemp+1, str, 10);
						keyword += str;
						onelinestr += keyword;

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						delta++;
						onelinestr.Empty();

						continue;
					}
					else onelinestr = "|RECORD=41";

					//OwnerIndex
					keyword = "|OWNERINDEX=";
					itoa(nLineNo, str, 10);
					keyword += str;
					onelinestr += keyword;

					if(ptext->m_nID == TEXT_TEXT)
					{
						//not accesssible
						onelinestr += "|ISNOTACCESIBLE=T";
						if(ptext->m_nOwnerPartID > 0)
						{
							keyword = "|OWNERPARTID=";
							itoa(ptext->m_nOwnerPartID, str, 10);
							keyword += str;
							onelinestr += keyword;
						}

						if(ptext->m_nDisplayMode > 0)
						{
							keyword = "|OWNERPARTDISPLAYMODE=";
							itoa(ptext->m_nDisplayMode, str, 10);
							keyword += str;
							onelinestr += keyword;
						}
					}
					else onelinestr += "|OWNERPARTID=-1";

					i1 = (int)(ptext->m_fSx/10);
					i2 = ptext->m_fSx*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.X=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.X_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}				

					i1 = (int)(ptext->m_fSy/10);
					i2 = ptext->m_fSy*10000;
					i2 = i2 - i1*100000;
					if(i1 != 0)
					{
						itoa(i1, str, 10);
						keyword = "|LOCATION.Y=";
						keyword += str;
						onelinestr += keyword;
					}
					if(i2 != 0)
					{
						itoa(i2, str, 10);
						keyword = "|LOCATION.Y_FRAC=";
						keyword += str;
						onelinestr += keyword;
					}

					if((ptext->m_nOrientation/90) > 0)
					{
						keyword = "|ORIENTATION=";
						itoa(ptext->m_nOrientation/90, str, 10);
						keyword += str;
						onelinestr += keyword;
					}
					//JUSTIFICATION
					if( (ptext->m_nH_Justification>0)||(ptext->m_nV_Justification>0))
					{
						keyword = "|JUSTIFICATION=";
						itoa((ptext->m_nH_Justification + ptext->m_nV_Justification*3), str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ptext->m_nColor > 0)
					{
						keyword = "|COLOR=";
						itoa(ptext->m_nColor, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					keyword = "|FONTID=";
					itoa(ptext->m_nFont+1, str, 10);
					keyword += str;
					onelinestr += keyword;

					if(ptext->m_bTextVisible == FALSE) onelinestr += "|ISHIDDEN=T";

					keyword = "|TEXT=";
					keyword += ptext->m_csText;
					onelinestr += keyword;

					if(ptext->m_bTextVisible == FALSE) onelinestr += "|ISHIDDEN=T";

					if(ptext->m_nParamType > 0)
					{
						keyword = "|PARAMTYPE=";
						itoa(ptext->m_nParamType, str, 10);
						keyword += str;
						onelinestr += keyword;
					}

					if(ptext->m_csNameComment.IsEmpty() == FALSE)
					{
						keyword = "|NAME=";
						keyword += ptext->m_csNameComment;
						onelinestr += keyword;
					}	
					//Show Name
					if(ptext->m_bNameVisible == TRUE) onelinestr += "|SHOWNAME=T";

					if( (ptext->m_bNameLocked > 0) || (ptext->m_bTextLocked > 0) )
					{
						keyword = "|READONLYSTATE=";
						itoa((ptext->m_bNameLocked + ptext->m_bTextLocked*2), str, 10);
						keyword += str;
						onelinestr += keyword;	
					}

					if(ptext->m_csUniqueID.IsEmpty() == FALSE)
					{
						keyword = "|UNIQUEID=";
						keyword += ptext->m_csUniqueID;
						onelinestr += keyword;
					}
					//allow library synchronize
					if(ptext->m_bAllowLibrarySynchronize == FALSE) onelinestr += "|NOTALLOWLIBRARYSYNCHRONIZE=T";
					//allow database synchronize
					if(ptext->m_bAllowDataBaseSynchronize == FALSE) onelinestr += "|NOTALLOWDATABASESYNCHRONIZE=T";

					if(ptext->m_bAutoPosition == FALSE) onelinestr += "|NOTAUTOPOSITION=T";
					//Mirror
					if(ptext->m_bMirror == TRUE) onelinestr += "|ISMIRRORED=T";

					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					delta++;
					onelinestr.Empty();
				}
			}
			nLineNo += delta;
		}
	}
	
	onelinestr = "|HEADER=Icon storage";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
}

void CESDDoc::Save_99_SCH_ASCII_File(CArchive& ar, int nCurrentDiagram)
{
	CString onelinestr;
	CString tspace = "  "; 
	CString csLineEnd = "\r\n";
	char str[100];
	CObject* pobject;

	onelinestr = "Protel for Windows - Schematic Capture Ascii File Version 1.2 - 2.0";

	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	onelinestr = "0";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	onelinestr = "[Font_Table]";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	itoa(m_arrayFont[nCurrentDiagram].GetCount(), str, 10);
	onelinestr = tspace;
	onelinestr += str;
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	for(int i=1; i<=m_arrayFont[nCurrentDiagram].GetCount(); i++)  
	{
		onelinestr = tspace;

		LOGFONT logfont;
		logfont = m_arrayFont[nCurrentDiagram].GetAt(i-1);

		itoa(logfont.lfHeight, str, 10);
		onelinestr += str;
		onelinestr += tspace;

		itoa(logfont.lfOrientation, str, 10);
		onelinestr += str;
		onelinestr += tspace;

		if(logfont.lfUnderline == TRUE) onelinestr += "1";
		else onelinestr += "0";
		onelinestr += tspace;

		if( logfont.lfItalic == TRUE ) onelinestr += "1";
		else onelinestr += "0";
		onelinestr += tspace;

		if(logfont.lfWeight == FW_BOLD) onelinestr += "1";
		else onelinestr += "0";
		onelinestr += tspace;

		if(logfont.lfStrikeOut == TRUE) onelinestr += "1";
		else onelinestr += "0";
		onelinestr += tspace;

		onelinestr += logfont.lfFaceName;
		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();
	}

	onelinestr = "EndFont";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	onelinestr = "Library";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	onelinestr = tspace;
	itoa(Get_SCH_Elements_Param(SCH_ELEMENT_COMPONENT, nCurrentDiagram, 0), str, 10);
	onelinestr += str;
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	POSITION pos;
	pos = m_listSchObject[nCurrentDiagram].GetHeadPosition();
	for(int i=0; i<m_listSchObject[nCurrentDiagram].GetCount(); i++)
	{
		if(pos == NULL) break;
		pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram != nCurrentDiagram)||(pcomp->m_nFlag == FLAG_DELETED))
			{
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) m_listSchObject[nCurrentDiagram].GetNext(pos);
				continue;
			}
			
			onelinestr = tspace;
			onelinestr += "Component";
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();

			//Number of parts
			onelinestr = tspace; onelinestr += tspace; 
			itoa(pcomp->m_nPartCount, str, 10); onelinestr += str;
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();
			//Number of shared
			onelinestr = tspace; onelinestr += tspace; onelinestr += "1";
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();
			//library name
			onelinestr = tspace; onelinestr += tspace;	onelinestr += pcomp->m_csLibraryFileName;
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();
			//Description
			onelinestr = tspace; onelinestr += tspace;	onelinestr += pcomp->m_csDescription;
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();
			//footprint1
			onelinestr = tspace; onelinestr += tspace;	onelinestr += pcomp->GetPackage();
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();
			//footprint2-4
			for(int j=0; j<3; j++)
			{
				onelinestr = tspace;
				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				onelinestr.Empty();
			}
			//library part field 1 - 8
			for(int j=0; j<8; j++)
			{
				onelinestr = tspace;
				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				onelinestr.Empty();
			}
			//default designator field
			CString cst = pcomp->GetDesignator();
			cst = cst.GetAt(0); cst += "?";
			onelinestr = tspace; onelinestr += tspace;	onelinestr += cst;
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();			
			//sheet part file name
			onelinestr = tspace; 
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();
			//list of all the shared components
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += pcomp->m_csLibraryReference;
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();

			POSITION posbak;
			posbak = pos;
			for(int k=1; k<=pcomp->m_nPartCount; k++)
			{
				onelinestr = tspace; onelinestr += tspace;	onelinestr += "Part";
				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				onelinestr.Empty();

				onelinestr = tspace; onelinestr += tspace; onelinestr += tspace;
				CSize csize = pcomp->GetCompXYSize();
				itoa(csize.cx, str, 10);
				onelinestr += str; onelinestr += tspace;
				itoa(csize.cy, str, 10);
				onelinestr += str; onelinestr += tspace;
				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				onelinestr.Empty();

				pos = posbak;
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
				{
					pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
					{
						COb_schpin* ppin = (COb_schpin*)pobject;
						if((ppin->m_nOwnerPartID != 0)&&(ppin->m_nOwnerPartID != k)) continue;

						onelinestr = tspace; onelinestr += tspace; onelinestr += tspace;
						onelinestr += "Pin"; onelinestr += tspace;

						//1 = dot
						if(ppin->m_nOutsideEdge == 1) onelinestr += "1";
						else  onelinestr += "0";
						onelinestr += tspace;
						//1 = clock
						if(ppin->m_nInsideEdge == 3) onelinestr += "1";
						else  onelinestr += "0";
						onelinestr += tspace;
						//electrical type
						itoa(ppin->m_nElectricalType, str, 10);
						onelinestr += str; onelinestr += tspace;
						//1 = hidden
						if(ppin->m_bHide == TRUE) onelinestr += "1";
						else  onelinestr += "0";
						onelinestr += tspace;
						//1 = show name
						if(ppin->m_bNameVisible == TRUE) onelinestr += "1";
						else  onelinestr += "0";
						onelinestr += tspace;
						//1 = show number
						if(ppin->m_bNumberVisible == TRUE) onelinestr += "1";
						else  onelinestr += "0";
						onelinestr += tspace;
						//Pin length
						itoa(ppin->m_fPinLength/10, str, 10);
						onelinestr += str; onelinestr += tspace;
					
						double f_oldx, f_oldy, fx, fy;
						f_oldx = ppin->m_fX - pcomp->m_fX;
						f_oldy = ppin->m_fY - pcomp->m_fY;

						int orientation;
						if(pcomp->m_bMirrored == TRUE) orientation = pcomp->m_nOrientation;
						else
						{
							if(pcomp->m_nOrientation == 0) orientation = 0;
							else orientation = 360 - pcomp->m_nOrientation;
						}
						Get_Rotate_XY(&fx, &fy, f_oldx, f_oldy, orientation, pcomp->m_bMirrored);
						//x
						itoa(fx/10, str, 10);
						onelinestr += str; onelinestr += tspace;
						//y
						itoa(fy/10, str, 10);
						onelinestr += str; onelinestr += tspace;
						//orientation
						int new_o;
						Get_Rotate_Orientation(&new_o, ppin->m_nOrientation, orientation, pcomp->m_bMirrored);
						itoa(new_o/90, str, 10);
						onelinestr += str; onelinestr += tspace;
						//color
						itoa(ppin->m_nColor, str, 10);
						onelinestr += str; onelinestr += tspace;
						//pin name
						onelinestr += "'";
						onelinestr += ppin->m_csName;
						onelinestr += "'";
						onelinestr += tspace;
						//pin number
						onelinestr += "'";
						onelinestr += ppin->m_csNumber;
						onelinestr += "'";
						onelinestr += tspace;

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
					{
						COb_schieee* pieee = (COb_schieee*)pobject;

						if((pieee->m_nOwnerPartID != 0)&&(pieee->m_nOwnerPartID != k)) continue;


						onelinestr = tspace; onelinestr += tspace; onelinestr += tspace;
						onelinestr += "Symbol"; onelinestr += tspace;
						
						itoa(pieee->m_nIeeeSymbol, str, 10);
						onelinestr += str; onelinestr += tspace;

						double f_oldx, f_oldy, fx, fy;
						f_oldx = pieee->m_fX - pcomp->m_fX;
						f_oldy = pieee->m_fY - pcomp->m_fY;

						int orientation;
						if(pcomp->m_bMirrored == TRUE) orientation = pcomp->m_nOrientation;
						else
						{
							if(pcomp->m_nOrientation == 0) orientation = 0;
							else orientation = 360 - pcomp->m_nOrientation;
						}
						Get_Rotate_XY(&fx, &fy, f_oldx, f_oldy, orientation, pcomp->m_bMirrored);
						//x
						itoa(fx/10, str, 10);
						onelinestr += str; onelinestr += tspace;
						//y
						itoa(fy/10, str, 10);
						onelinestr += str; onelinestr += tspace;
						//≥ﬂ¥Á
						itoa(pieee->m_fSize, str, 10);
						onelinestr += str; onelinestr += tspace;
						//orientation
						int new_o;
						Get_Rotate_Orientation(&new_o, pieee->m_nOrientation, orientation, pcomp->m_bMirrored);
						itoa(new_o/90, str, 10);
						onelinestr += str; onelinestr += tspace;
						// linewidth
						itoa(pieee->m_nWidth, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//Color
						itoa(pieee->m_nColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//selection
						onelinestr += "0";  onelinestr += tspace;

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
					{
						COb_scharc* parc = (COb_scharc*)pobject;
						if((parc->m_nOwnerPartID != 0)&&(parc->m_nOwnerPartID != k)) continue;

						// Arc
						onelinestr = tspace; onelinestr += tspace; onelinestr += tspace;
						onelinestr += "Arc"; onelinestr += tspace;

						double f_oldx, f_oldy, fx, fy;
						f_oldx = parc->m_fCx - pcomp->m_fX;
						f_oldy = parc->m_fCy - pcomp->m_fY;

						int orientation;
						if(pcomp->m_bMirrored == TRUE) orientation = pcomp->m_nOrientation;
						else
						{
							if(pcomp->m_nOrientation == 0) orientation = 0;
							else orientation = 360 - pcomp->m_nOrientation;
						}
						Get_Rotate_XY(&fx, &fy, f_oldx, f_oldy, orientation, pcomp->m_bMirrored);

						itoa(fx/10, str, 10);
						onelinestr += str;  onelinestr += tspace;

						itoa(fy/10, str, 10);
						onelinestr += str;  onelinestr += tspace;

						itoa(parc->m_fRadius/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// linewidth
						itoa(parc->m_nWidth, str, 10);
						onelinestr += str;  onelinestr += tspace;

						double sangle, eangle, sa, ea;
						sa = parc->m_fSangle;
						ea = parc->m_fEangle;
											
						Get_Rotate_Angle(&sangle, &eangle, sa, ea, orientation, pcomp->m_bMirrored);
						//start angle
						sprintf(str,"%.3f",sangle);
						onelinestr += str;  onelinestr += tspace;
						//end angle
						sprintf(str,"%.3f", eangle);
						onelinestr += str;  onelinestr += tspace;			
						//Color
						itoa(parc->m_nColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//selection
						onelinestr += "0";  onelinestr += tspace;
						
						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();

					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
					{
						COb_schellipse* pellipse = (COb_schellipse*)pobject;
						
						if((pellipse->m_nOwnerPartID != 0)&&(pellipse->m_nOwnerPartID != k)) continue;

						onelinestr = tspace; onelinestr += tspace; onelinestr += tspace;
						onelinestr += "EllipticalArc"; onelinestr += tspace;
						
						double f_oldx, f_oldy, fx, fy;
						f_oldx = pellipse->m_fCx - pcomp->m_fX;
						f_oldy = pellipse->m_fCy - pcomp->m_fY;

						int orientation;
						if(pcomp->m_bMirrored == TRUE) orientation = pcomp->m_nOrientation;
						else
						{
							if(pcomp->m_nOrientation == 0) orientation = 0;
							else orientation = 360 - pcomp->m_nOrientation;
						}
						Get_Rotate_XY(&fx, &fy, f_oldx, f_oldy, orientation, pcomp->m_bMirrored);

						itoa(fx/10, str, 10);
						onelinestr += str;  onelinestr += tspace;

						itoa(fy/10, str, 10);
						onelinestr += str;  onelinestr += tspace;

						if((pcomp->m_nOrientation == 90) || (pcomp->m_nOrientation == 270))
						{
							double dtemp = pellipse->m_fRadius_x;
							pellipse->m_fRadius_x = pellipse->m_fRadius_y;
							pellipse->m_fRadius_y = dtemp;
						}

						itoa(pellipse->m_fRadius_x/10, str, 10);
						onelinestr += str;  onelinestr += tspace;

						itoa(pellipse->m_fRadius_y/10, str, 10);
						onelinestr += str;  onelinestr += tspace;

						itoa(pellipse->m_nWidth, str, 10);
						onelinestr += str;  onelinestr += tspace;

						double sangle, eangle, sa, ea;
						sa = pellipse->m_fSangle;
						ea = pellipse->m_fEangle;
											
						Get_Rotate_Angle(&sangle, &eangle, sa, ea, orientation, pcomp->m_bMirrored);
						//start angle
						sprintf(str,"%.3f", sangle);
						onelinestr += str;  onelinestr += tspace;
						//end angle
						sprintf(str,"%.3f", eangle);
						onelinestr += str;  onelinestr += tspace;			
						//Color
						itoa(pellipse->m_nColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//selection
						onelinestr += "0";  onelinestr += tspace;
						
						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();	

					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
					{
						COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;

						if((pellipsefill->m_nOwnerPartID != 0)&&(pellipsefill->m_nOwnerPartID != k)) continue;

						// Ellipse Arc
						onelinestr = tspace; onelinestr += tspace;onelinestr += tspace;
						onelinestr += "Ellipse"; onelinestr += tspace;

						double f_oldx, f_oldy, fx, fy;
						f_oldx = pellipsefill->m_fCx - pcomp->m_fX;
						f_oldy = pellipsefill->m_fCy - pcomp->m_fY;

						int orientation;
						if(pcomp->m_bMirrored == TRUE) orientation = pcomp->m_nOrientation;
						else
						{
							if(pcomp->m_nOrientation == 0) orientation = 0;
							else orientation = 360 - pcomp->m_nOrientation;
						}
						Get_Rotate_XY(&fx, &fy, f_oldx, f_oldy, orientation, pcomp->m_bMirrored);

						itoa(fx/10, str, 10);
						onelinestr += str;  onelinestr += tspace;

						itoa(fy/10, str, 10);
						onelinestr += str;  onelinestr += tspace;

						if((pcomp->m_nOrientation == 90) || (pcomp->m_nOrientation == 270))
						{
							double dtemp = pellipsefill->m_fRadius_x;
							pellipsefill->m_fRadius_x = pellipsefill->m_fRadius_y;
							pellipsefill->m_fRadius_y = dtemp;
						}

						itoa(pellipsefill->m_fRadius_x/10, str, 10);
						onelinestr += str;  onelinestr += tspace;

						itoa(pellipsefill->m_fRadius_y/10, str, 10);
						onelinestr += str;  onelinestr += tspace;

						// linewidth
						itoa(pellipsefill->m_nBorderWidth, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// Border Color
						itoa(pellipsefill->m_nBorderColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// Fill Color
						itoa(pellipsefill->m_nFillColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// 1 = solid
						if(pellipsefill->m_bFillSolid == TRUE) onelinestr += "1";
						else onelinestr += "0";
						onelinestr += tspace;
						//selection
						onelinestr += "0";  onelinestr += tspace;
						
						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();					
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
					{
						COb_schpie* ppie = (COb_schpie*)pobject;

						if((ppie->m_nOwnerPartID != 0)&&(ppie->m_nOwnerPartID != k)) continue;

						// Pie
						onelinestr = tspace; onelinestr += tspace;
						onelinestr += "Pie"; onelinestr += tspace;

						double f_oldx, f_oldy, fx, fy;
						f_oldx = ppie->m_fCx - pcomp->m_fX;
						f_oldy = ppie->m_fCy - pcomp->m_fY;

						int orientation;
						if(pcomp->m_bMirrored == TRUE) orientation = pcomp->m_nOrientation;
						else
						{
							if(pcomp->m_nOrientation == 0) orientation = 0;
							else orientation = 360 - pcomp->m_nOrientation;
						}
						Get_Rotate_XY(&fx, &fy, f_oldx, f_oldy, orientation, pcomp->m_bMirrored);

						itoa(fx/10, str, 10);
						onelinestr += str;  onelinestr += tspace;

						itoa(fy/10, str, 10);
						onelinestr += str;  onelinestr += tspace;

						itoa(ppie->m_fRadius/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// linewidth
						itoa(ppie->m_nBorderWidth, str, 10);
						onelinestr += str;  onelinestr += tspace;
						
						double sangle, eangle, sa, ea;
						sa = ppie->m_fSangle;
						ea = ppie->m_fEangle;
											
						Get_Rotate_Angle(&sangle, &eangle, sa, ea, orientation, pcomp->m_bMirrored);
						//start angle
						sprintf(str,"%.3f", sangle);
						onelinestr += str;  onelinestr += tspace;
						//end angle
						sprintf(str,"%.3f", eangle);
						onelinestr += str;  onelinestr += tspace;			
						// Border Color
						itoa(ppie->m_nBorderColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//Fill color
						itoa(ppie->m_nFillColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// 1 = solid
						if(ppie->m_bFillSolid == TRUE) onelinestr += "1";
						else onelinestr += "0";
						onelinestr += tspace;
						//selection
						onelinestr += "0";  onelinestr += tspace;
						
						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();	
				
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
					{
						COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;

						if((ppolyline->m_nOwnerPartID != 0)&&(ppolyline->m_nOwnerPartID != k)) continue;
						
						// Polyline
						onelinestr = tspace; onelinestr += tspace; onelinestr += tspace;
						onelinestr += "Polyline"; onelinestr += tspace;
						// linewidth
						itoa(ppolyline->m_nLineWidth, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// style
						itoa(ppolyline->m_nLineType, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// Color
						itoa(ppolyline->m_nColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//selection
						onelinestr += "0";  onelinestr += tspace;
						//number of vertices
						itoa(ppolyline->m_cVertex.GetCount(), str, 10);
						onelinestr += str;  onelinestr += tspace;
						//x vertex 1,  y vertex 1, ......
						for(int j=0; j<ppolyline->m_cVertex.GetCount(); j++)
						{
							double f_oldx, f_oldy, fx, fy;
							f_oldx = ppolyline->m_cVertex[j].fx - pcomp->m_fX;
							f_oldy = ppolyline->m_cVertex[j].fy - pcomp->m_fY;

							int orientation;
							if(pcomp->m_bMirrored == TRUE) orientation = pcomp->m_nOrientation;
							else
							{
								if(pcomp->m_nOrientation == 0) orientation = 0;
								else orientation = 360 - pcomp->m_nOrientation;
							}
							Get_Rotate_XY(&fx, &fy, f_oldx, f_oldy, orientation, pcomp->m_bMirrored);

							itoa(fx/10, str, 10);
							onelinestr += str;  onelinestr += tspace;
							itoa(fy/10, str, 10);
							onelinestr += str;  onelinestr += tspace;
						}

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();	
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
					{
						COb_schrect* prect = (COb_schrect*)pobject;

						if((prect->m_nOwnerPartID != 0)&&(prect->m_nOwnerPartID != k)) continue;

						// Rect
						onelinestr = tspace; onelinestr += tspace; onelinestr += tspace;
						onelinestr += "Rectangle"; onelinestr += tspace;

						double f_oldx, f_oldy, fx, fy;
						f_oldx = prect->m_fX1 - pcomp->m_fX;
						f_oldy = prect->m_fY1 - pcomp->m_fY;

						int orientation;
						if(pcomp->m_bMirrored == TRUE) orientation = pcomp->m_nOrientation;
						else
						{
							if(pcomp->m_nOrientation == 0) orientation = 0;
							else orientation = 360 - pcomp->m_nOrientation;
						}
						Get_Rotate_XY(&fx, &fy, f_oldx, f_oldy, orientation, pcomp->m_bMirrored);
						//x1
						itoa(fx/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//y1
						itoa(fy/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						f_oldx = prect->m_fX2 - pcomp->m_fX;
						f_oldy = prect->m_fY2 - pcomp->m_fY;
						Get_Rotate_XY(&fx, &fy, f_oldx, f_oldy, orientation, pcomp->m_bMirrored);
						//x2
						itoa(fx/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//y2
						itoa(fy/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// linewidth
						itoa(prect->m_nBorderWidth, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// Color
						itoa(prect->m_nBorderColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// fill Color
						itoa(prect->m_nFillColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//selection
						onelinestr += "0";  onelinestr += tspace;
						// 1 = solid
						if(prect->m_bFillSolid == TRUE) onelinestr += "1";
						else onelinestr += "0";
						onelinestr += tspace;

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();	

					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
					{
						COb_schroundrect* proundrect = (COb_schroundrect*)pobject;

						if((proundrect->m_nOwnerPartID != 0)&&(proundrect->m_nOwnerPartID != k)) continue;

						onelinestr = tspace; onelinestr += tspace;
						onelinestr += "RoundRectangle"; onelinestr += tspace;

						double f_oldx, f_oldy, fx, fy;
						f_oldx = proundrect->m_fX1 - pcomp->m_fX;
						f_oldy = proundrect->m_fY1 - pcomp->m_fY;

						int orientation;
						if(pcomp->m_bMirrored == TRUE) orientation = pcomp->m_nOrientation;
						else
						{
							if(pcomp->m_nOrientation == 0) orientation = 0;
							else orientation = 360 - pcomp->m_nOrientation;
						}
						Get_Rotate_XY(&fx, &fy, f_oldx, f_oldy, orientation, pcomp->m_bMirrored);
						//x1
						itoa(fx/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//y1
						itoa(fy/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						f_oldx = proundrect->m_fX2 - pcomp->m_fX;
						f_oldy = proundrect->m_fY2 - pcomp->m_fY;
						Get_Rotate_XY(&fx, &fy, f_oldx, f_oldy, orientation, pcomp->m_bMirrored);
						//x2
						itoa(fx/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//y2
						itoa(fy/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						if((pcomp->m_nOrientation == 90) || (pcomp->m_nOrientation == 270))
						{
							double dtemp = proundrect->m_fRadius_x;
							proundrect->m_fRadius_x = proundrect->m_fRadius_y;
							proundrect->m_fRadius_y = dtemp;
						}
						//x radius
						itoa(proundrect->m_fRadius_x/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//y radius
						itoa(proundrect->m_fRadius_y/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// linewidth
						itoa(proundrect->m_nBorderWidth, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// Color
						itoa(proundrect->m_nBorderColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// fill Color
						itoa(proundrect->m_nFillColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//selection
						onelinestr += "0";  onelinestr += tspace;
						// 1 = solid
						if(proundrect->m_bFillSolid == TRUE) onelinestr += "1";
						else onelinestr += "0";
						onelinestr += tspace;

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();	

					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
					{
						COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;

						if((ppolygon->m_nOwnerPartID != 0)&&(ppolygon->m_nOwnerPartID != k)) continue;

						// Polygon
						onelinestr = tspace; onelinestr += tspace; onelinestr += tspace;
						onelinestr += "Polygon"; onelinestr += tspace;
						// linewidth
						itoa(ppolygon->m_nBorderWidth, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// Color
						itoa(ppolygon->m_nBorderColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// fill Color
						itoa(ppolygon->m_nFillColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// 1 = solid
						if(ppolygon->m_bFillSolid == TRUE) onelinestr += "1";
						else onelinestr += "0";
						onelinestr += tspace;	
						//selection
						onelinestr += "0";  onelinestr += tspace;
						//number of vertices
						itoa(ppolygon->m_cVertex.GetCount(), str, 10);
						onelinestr += str;  onelinestr += tspace;
						//x vertex 1,  y vertex 1, ......
						for(int j=0; j<ppolygon->m_cVertex.GetCount(); j++)
						{
							double f_oldx, f_oldy, fx, fy;
							f_oldx = ppolygon->m_cVertex[j].fx - pcomp->m_fX;
							f_oldy = ppolygon->m_cVertex[j].fy - pcomp->m_fY;

							int orientation;
							if(pcomp->m_bMirrored == TRUE) orientation = pcomp->m_nOrientation;
							else
							{
								if(pcomp->m_nOrientation == 0) orientation = 0;
								else orientation = 360 - pcomp->m_nOrientation;
							}
							Get_Rotate_XY(&fx, &fy, f_oldx, f_oldy, orientation, pcomp->m_bMirrored);

							itoa(fx/10, str, 10);
							onelinestr += str;  onelinestr += tspace;
							itoa(fy/10, str, 10);
							onelinestr += str;  onelinestr += tspace;
						}

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();				
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
					{
						COb_schbezier* pbezier = (COb_schbezier*)pobject;
			
						if((pbezier->m_nOwnerPartID != 0)&&(pbezier->m_nOwnerPartID != k)) continue;

						// Bezier
						onelinestr = tspace; onelinestr += tspace;
						onelinestr += "Bezier"; onelinestr += tspace;
						// linewidth
						itoa(pbezier->m_nWidth, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// Color
						itoa(pbezier->m_nColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//selection
						onelinestr += "0";  onelinestr += tspace;
						//number of vertices
						itoa(pbezier->m_cVertex.GetCount(), str, 10);
						onelinestr += str;  onelinestr += tspace;
						//x vertex 1,  y vertex 1, ......
						for(int j=0; j<pbezier->m_cVertex.GetCount(); j++)
						{
							double f_oldx, f_oldy, fx, fy;
							f_oldx = pbezier->m_cVertex[j].fx - pcomp->m_fX;
							f_oldy = pbezier->m_cVertex[j].fy - pcomp->m_fY;

							int orientation;
							if(pcomp->m_bMirrored == TRUE) orientation = pcomp->m_nOrientation;
							else
							{
								if(pcomp->m_nOrientation == 0) orientation = 0;
								else orientation = 360 - pcomp->m_nOrientation;
							}
							Get_Rotate_XY(&fx, &fy, f_oldx, f_oldy, orientation, pcomp->m_bMirrored);
							itoa(fx/10, str, 10);
							onelinestr += str;  onelinestr += tspace;
							itoa(fy/10, str, 10);
							onelinestr += str;  onelinestr += tspace;
						}

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();	
				
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;

						if(ptext->m_nID != TEXT_TEXT) continue;
						if((ptext->m_nOwnerPartID != 0)&&(ptext->m_nOwnerPartID != k)) continue;
				
						// Label
						onelinestr = tspace; onelinestr += tspace;
						onelinestr += "Label"; onelinestr += tspace;

						double f_oldx, f_oldy, fx, fy;
						f_oldx = ptext->m_fSx - pcomp->m_fX;
						f_oldy = ptext->m_fSy - pcomp->m_fY;

						int orientation;
						if(pcomp->m_bMirrored == TRUE) orientation = pcomp->m_nOrientation;
						else
						{
							if(pcomp->m_nOrientation == 0) orientation = 0;
							else orientation = 360 - pcomp->m_nOrientation;
						}
						Get_Rotate_XY(&fx, &fy, f_oldx, f_oldy, orientation, pcomp->m_bMirrored);

						//x
						itoa(fx/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//y
						itoa(fy/10, str, 10);
						onelinestr += str;  onelinestr += tspace;

						//orientation
						int ttt;
						if( pcomp->m_bMirrored == TRUE)
						{
							if(ptext->m_nOrientation == 0) ttt = 180;
							else if(ptext->m_nOrientation == 180) ttt = 0;
							else ttt = ptext->m_nOrientation;
						}
						else ttt = ptext->m_nOrientation;
						int new_o;
						Get_Rotate_Orientation(&new_o, ttt, orientation, pcomp->m_bMirrored);
						itoa(new_o/90, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// Color
						itoa(ptext->m_nColor, str, 10);
						onelinestr += str;  onelinestr += tspace;				
						//Font ID
						itoa(ptext->m_nFont+1, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//selection
						onelinestr += "0";  onelinestr += tspace;
						//Text
						onelinestr += "'";
						onelinestr += ptext->m_csText;
						onelinestr += "'";

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();	
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
					{
						COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
				
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
					{
						COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
				
					}
				}
				onelinestr = tspace; onelinestr += tspace;	onelinestr += "EndNormalPart";
				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				onelinestr.Empty();
	  			
				onelinestr = tspace; onelinestr += tspace;	onelinestr += "EndDeMorganPart";
				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				onelinestr.Empty();  

  				onelinestr = tspace; onelinestr += tspace;	onelinestr += "EndIEEEPart";
				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				onelinestr.Empty();  
			}

			onelinestr = tspace;
			onelinestr += "EndComponent";
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();
		}
	}

	onelinestr = "EndLibrary";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	onelinestr = tspace;
	onelinestr += "Organization";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	for(int j=0; j<8; j++)
	{
		onelinestr = tspace;
		onelinestr += "*";
		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();
	}
	onelinestr = "  0  0";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	
	onelinestr = tspace;
	onelinestr += "End";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	onelinestr = "Future"; onelinestr += tspace;
	if(m_bSCH_ElecGridEnable == TRUE) onelinestr += "1";
	else onelinestr += "0";	onelinestr += tspace;
	itoa((int)m_fSCH_ElecGrid, str, 10);
	onelinestr += str; 	onelinestr += tspace;
	itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nStandardStyle, str, 10);
	onelinestr += str; 
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	onelinestr = "EndFuture 1";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	onelinestr = "Sheet";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	
	onelinestr = tspace; 
	//standard/ansi
	itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nTitleBlockStyle, str, 10);
	onelinestr += str; onelinestr += tspace;
	//sheet style 0=a4,...
	itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nStandardStyle, str, 10);
	onelinestr += str; onelinestr += tspace;
	//orientation landscape/portrait
	itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nOrientation, str, 10);
	onelinestr += str; onelinestr += tspace;
	//1 = show border/0 = hide
	if(m_pSCH_Sheet[nCurrentDiagram]->m_bShowBorder == TRUE) onelinestr += "1";
	else onelinestr += "0";
	onelinestr += tspace;
	//1= show title block
	if(m_pSCH_Sheet[nCurrentDiagram]->m_bTitleBlock == TRUE) onelinestr += "1";
	else onelinestr += "0";
	onelinestr += tspace;

	//sheet border/title/text color
	itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nBorderColor, str, 10);
	onelinestr += str;	onelinestr += tspace;
	//sheet background color
	itoa(m_pSCH_Sheet[nCurrentDiagram]->m_nSheetColor, str, 10);
	onelinestr += str;	onelinestr += tspace;
	//1= snap grid on
	if(m_bSCH_SnapGridEnable == TRUE)  onelinestr += "1";
	else onelinestr += "0"; onelinestr += tspace;
	//snap grid size
	itoa((int)m_fSCH_SnapGrid, str, 10);
	onelinestr += str;	onelinestr += tspace;
	//1=visible grid on
	if(m_bSCH_VisibleGridShow == TRUE)  onelinestr += "1";
	else onelinestr += "0"; onelinestr += tspace;
	//visiblesnap grid size
	itoa((int)m_fSCH_VisibleGrid, str, 10);
	onelinestr += str;	onelinestr += tspace;
	//x custom size
	itoa(m_pSCH_Sheet[nCurrentDiagram]->m_fCustomWidth/10, str, 10);
	onelinestr += str;	onelinestr += tspace;
	//y custom size
	itoa(m_pSCH_Sheet[nCurrentDiagram]->m_fCustomHeight/10, str, 10);
	onelinestr += str;	onelinestr += tspace;
	//1 = use custom
	if(m_pSCH_Sheet[nCurrentDiagram]->m_bCustom == TRUE)  onelinestr += "1";
	else  onelinestr += "0"; onelinestr += tspace;
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	
	int nCompLibraryID = 0;
	pos = m_listSchObject[nCurrentDiagram].GetHeadPosition();
	for(int i=0; i<m_listSchObject[nCurrentDiagram].GetCount(); i++)
	{	
		if(pos == NULL) break;
		pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nDiagram != nCurrentDiagram)||(pline->m_nFlag == FLAG_DELETED))	continue;
			
			if( pline->m_nAttrib == LINE_WIRE)
			{
				onelinestr = tspace; onelinestr += tspace;
				onelinestr += "Wire"; onelinestr += tspace;
				// linewidth
				itoa(pline->m_nLineWidth, str, 10);
				onelinestr += str;  onelinestr += tspace;
				// Color
				itoa(pline->m_nColor, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//selection
				onelinestr += "0";  onelinestr += tspace;
				//number of vertices
				itoa(pline->m_cVertex.GetCount(), str, 10);
				onelinestr += str;  onelinestr += tspace;
				//x vertex 1,  y vertex 1, ......
				for(int j=0; j<pline->m_cVertex.GetCount(); j++)
				{
					itoa(pline->m_cVertex[j].fx/10, str, 10);
					onelinestr += str;  onelinestr += tspace;
					itoa(pline->m_cVertex[j].fy/10, str, 10);
					onelinestr += str;  onelinestr += tspace;
				}
				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				onelinestr.Empty();
			}
			else if( pline->m_nAttrib == LINE_BUS)
			{
				// Bus
				onelinestr = tspace; onelinestr += tspace;
				onelinestr += "Bus"; onelinestr += tspace;
				// linewidth
				itoa(pline->m_nLineWidth, str, 10);
				onelinestr += str;  onelinestr += tspace;
				// Color
				itoa(pline->m_nColor, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//selection
				onelinestr += "0";  onelinestr += tspace;
				//number of vertices
				itoa(pline->m_cVertex.GetCount(), str, 10);
				onelinestr += str;  onelinestr += tspace;
				//x vertex 1,  y vertex 1, ......
				for(int j=0; j<pline->m_cVertex.GetCount(); j++)
				{
					itoa(pline->m_cVertex[j].fx/10, str, 10);
					onelinestr += str;  onelinestr += tspace;
					itoa(pline->m_cVertex[j].fy/10, str, 10);
					onelinestr += str;  onelinestr += tspace;
				}
				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				onelinestr.Empty();
			}
			else if(pline->m_nAttrib == LINE_BUS_ENTRY)
			{
				// BusEntry
				onelinestr = tspace; onelinestr += tspace;
				onelinestr += "BusEntry"; onelinestr += tspace;
				//x1
				itoa(pline->m_cVertex[0].fx/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//y1
				itoa(pline->m_cVertex[0].fy/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//x2
				itoa(pline->m_cVertex[1].fx/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//y2
				itoa(pline->m_cVertex[1].fy/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
				// linewidth
				itoa(pline->m_nLineWidth, str, 10);
				onelinestr += str;  onelinestr += tspace;
				// Color
				itoa(pline->m_nColor, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//selection
				onelinestr += "0";  onelinestr += tspace;

				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				onelinestr.Empty();				
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != nCurrentDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			
			// Polyline
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "Polyline"; onelinestr += tspace;
			// linewidth
			itoa(ppolyline->m_nLineWidth, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// style
			itoa(ppolyline->m_nLineType, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// Color
			itoa(ppolyline->m_nColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//selection
			onelinestr += "0";  onelinestr += tspace;
			//number of vertices
			itoa(ppolyline->m_cVertex.GetCount(), str, 10);
			onelinestr += str;  onelinestr += tspace;
			//x vertex 1,  y vertex 1, ......
			for(int j=0; j<ppolyline->m_cVertex.GetCount(); j++)
			{
				itoa(ppolyline->m_cVertex[j].fx/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
				itoa(ppolyline->m_cVertex[j].fy/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
			}

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc;
			pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != nCurrentDiagram)||(pjunc->m_nFlag == FLAG_DELETED))	continue;
				
			// Junction
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "Junction"; onelinestr += tspace;
			//x
			itoa(pjunc->m_fCx/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y
			itoa(pjunc->m_fCy/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//size
			itoa(pjunc->m_nSize, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// Color
			itoa(pjunc->m_nColor, str, 10);
			onelinestr += str;  onelinestr += tspace;				
			//selection
			onelinestr += "0";  onelinestr += tspace;

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();				
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != nCurrentDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;

			else if(ptext->m_nID == TEXT_TEXT)
			{
				// Label
				onelinestr = tspace; onelinestr += tspace;
				onelinestr += "Label"; onelinestr += tspace;
				//x
				itoa(ptext->m_fSx/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//y
				itoa(ptext->m_fSy/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//orientation
				itoa(ptext->m_nOrientation/90, str, 10);
				onelinestr += str;  onelinestr += tspace;
				// Color
				itoa(ptext->m_nColor, str, 10);
				onelinestr += str;  onelinestr += tspace;				
				//Font ID
				itoa(ptext->m_nFont+1, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//selection
				onelinestr += "0";  onelinestr += tspace;
				//Text
				onelinestr += "'";
				onelinestr += ptext->m_csText;
				onelinestr += "'";

				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				onelinestr.Empty();	
			}
			else if(ptext->m_nID == TEXT_NETLABEL)
			{
				// NetLabel
				onelinestr = tspace; onelinestr += tspace;
				onelinestr += "NetLabel"; onelinestr += tspace;
				//x
				itoa(ptext->m_fSx/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//y
				itoa(ptext->m_fSy/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//orientation
				itoa(ptext->m_nOrientation/90, str, 10);
				onelinestr += str;  onelinestr += tspace;
				// Color
				itoa(ptext->m_nColor, str, 10);
				onelinestr += str;  onelinestr += tspace;				
				//Font ID
				itoa(ptext->m_nFont+1, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//selection
				onelinestr += "0";  onelinestr += tspace;
				//Text
				onelinestr += "'";
				onelinestr += ptext->m_csText;
				onelinestr += "'";

				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				onelinestr.Empty();
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != nCurrentDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
	
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "Arc"; onelinestr += tspace;

			itoa(parc->m_fCx/10, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(parc->m_fCy/10, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(parc->m_fRadius/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// linewidth
			itoa(parc->m_nWidth, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//start angle
			sprintf(str,"%.3f", parc->m_fSangle);
			onelinestr += str;  onelinestr += tspace;
			//end angle
			sprintf(str,"%.3f", parc->m_fEangle);
			onelinestr += str;  onelinestr += tspace;			
			//Color
			itoa(parc->m_nColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//selection
			onelinestr += "0";  onelinestr += tspace;
			
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != nCurrentDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "EllipticalArc"; onelinestr += tspace;

			itoa(pellipse->m_fCx/10, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(pellipse->m_fCy/10, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(pellipse->m_fRadius_x/10, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(pellipse->m_fRadius_y/10, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(pellipse->m_nWidth, str, 10);
			onelinestr += str;  onelinestr += tspace;

			sprintf(str,"%.3f", pellipse->m_fSangle);
			onelinestr += str;  onelinestr += tspace;

			sprintf(str,"%.3f", pellipse->m_fEangle);
			onelinestr += str;  onelinestr += tspace;			

			itoa(pellipse->m_nColor, str, 10);
			onelinestr += str;  onelinestr += tspace;

			onelinestr += "0";  onelinestr += tspace;
			
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != nCurrentDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "Ellipse"; onelinestr += tspace;

			itoa(pellipsefill->m_fCx/10, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(pellipsefill->m_fCy/10, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(pellipsefill->m_fRadius_x/10, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(pellipsefill->m_fRadius_y/10, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(pellipsefill->m_nBorderWidth, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(pellipsefill->m_nBorderColor, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(pellipsefill->m_nFillColor, str, 10);
			onelinestr += str;  onelinestr += tspace;

			if(pellipsefill->m_bFillSolid == TRUE) onelinestr += "1";
			else onelinestr += "0";
			onelinestr += tspace;

			onelinestr += "0";  onelinestr += tspace;
			
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != nCurrentDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "Pie"; onelinestr += tspace;

			itoa(ppie->m_fCx/10, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(ppie->m_fCy/10, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(ppie->m_fRadius/10, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(ppie->m_nBorderWidth, str, 10);
			onelinestr += str;  onelinestr += tspace;

			sprintf(str,"%.3f", ppie->m_fSangle);
			onelinestr += str;  onelinestr += tspace;

			sprintf(str,"%.3f", ppie->m_fEangle);
			onelinestr += str;  onelinestr += tspace;			

			itoa(ppie->m_nBorderColor, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(ppie->m_nFillColor, str, 10);
			onelinestr += str;  onelinestr += tspace;

			if(ppie->m_bFillSolid == TRUE) onelinestr += "1";
			else onelinestr += "0";
			onelinestr += tspace;

			onelinestr += "0";  onelinestr += tspace;
			
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != nCurrentDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "Polygon"; onelinestr += tspace;

			itoa(ppolygon->m_nBorderWidth, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(ppolygon->m_nBorderColor, str, 10);
			onelinestr += str;  onelinestr += tspace;

			itoa(ppolygon->m_nFillColor, str, 10);
			onelinestr += str;  onelinestr += tspace;

			if(ppolygon->m_bFillSolid == TRUE) onelinestr += "1";
			else onelinestr += "0";
			onelinestr += tspace;			

			onelinestr += "0";  onelinestr += tspace;

			itoa(ppolygon->m_cVertex.GetCount(), str, 10);
			onelinestr += str;  onelinestr += tspace;

			for(int j=0; j<ppolygon->m_cVertex.GetCount(); j++)
			{
				itoa(ppolygon->m_cVertex[j].fx/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
				itoa(ppolygon->m_cVertex[j].fy/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
			}

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();				

		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
		{
			COb_schtextframe* ptextframe;
			ptextframe = (COb_schtextframe*)pobject;
			if((ptextframe->m_nDiagram != nCurrentDiagram)||(ptextframe->m_nFlag == FLAG_DELETED))	continue;

			// TextFrame
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "TextFrame"; onelinestr += tspace;
			//x1
			itoa(ptextframe->m_fX1/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y1
			itoa(ptextframe->m_fY1/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//x2
			itoa(ptextframe->m_fX2/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y2
			itoa(ptextframe->m_fY2/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// linewidth
			itoa(ptextframe->m_nBorderWidth, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// Color
			itoa(ptextframe->m_nBorderColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// fill Color
			itoa(ptextframe->m_nFillColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// text Color
			itoa(ptextframe->m_nTextColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//Font ID
			itoa(ptextframe->m_nFont+1, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// 1 = solid
			if(ptextframe->m_bFillSolid == TRUE) onelinestr += "1";
			else onelinestr += "0";
			onelinestr += tspace;
			// 1= show border
			if(ptextframe->m_bDrawBorder == TRUE) onelinestr += "1";
			else onelinestr += "0";
			onelinestr += tspace;
			//alignment
			itoa(ptextframe->m_nAlignment, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//1 = word wrap
			if(ptextframe->m_bWordWrap == TRUE) onelinestr += "1";
			else onelinestr += "0";
			onelinestr += tspace;
			//1 = clip to bounding
			if(ptextframe->m_bClipToArea == TRUE) onelinestr += "1";
			else onelinestr += "0";
			onelinestr += tspace;
			//selection
			onelinestr += "0";  onelinestr += tspace;
			
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();				
			
			//Text Begin
			onelinestr = "Begin";
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	
			for(int j=0; j<ptextframe->m_arrayText.GetCount(); j++)
			{
				char special_str[4] = {0x0c, 0x0d, 0x0a, 0};
				onelinestr = ptextframe->m_arrayText.GetAt(j);
				if(j == (ptextframe->m_arrayText.GetCount()-1)) onelinestr += special_str;
				else onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				onelinestr.Empty();	
			}
			
			//Text End
			onelinestr = "End";
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();			
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != nCurrentDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;

			// Rect
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "Rectangle"; onelinestr += tspace;
			//x1
			itoa(prect->m_fX1/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y1
			itoa(prect->m_fY1/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//x2
			itoa(prect->m_fX2/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y2
			itoa(prect->m_fY2/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// linewidth
			itoa(prect->m_nBorderWidth, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// Color
			itoa(prect->m_nBorderColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// fill Color
			itoa(prect->m_nFillColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//selection
			onelinestr += "0";  onelinestr += tspace;
			// 1 = solid
			if(prect->m_bFillSolid == TRUE) onelinestr += "1";
			else onelinestr += "0";
			onelinestr += tspace;

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != nCurrentDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
				
			// RoundRect
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "RoundRectangle"; onelinestr += tspace;
			//x1
			itoa(proundrect->m_fX1/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y1
			itoa(proundrect->m_fY1/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//x2
			itoa(proundrect->m_fX2/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y2
			itoa(proundrect->m_fY2/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//x radius
			itoa(proundrect->m_fRadius_x/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y radius
			itoa(proundrect->m_fRadius_y/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// linewidth
			itoa(proundrect->m_nBorderWidth, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// Color
			itoa(proundrect->m_nBorderColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// fill Color
			itoa(proundrect->m_nFillColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//selection
			onelinestr += "0";  onelinestr += tspace;
			// 1 = solid
			if(proundrect->m_bFillSolid == TRUE) onelinestr += "1";
			else onelinestr += "0";
			onelinestr += tspace;

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();				
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;
			if((psheetsymbol->m_nDiagram != nCurrentDiagram)||(psheetsymbol->m_nFlag == FLAG_DELETED))
			{
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) m_listSchObject[nCurrentDiagram].GetNext(pos);
				continue;
			}

			// SheetSymbol
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "SheetSymbol"; onelinestr += tspace;
			//x
			itoa(psheetsymbol->m_fX/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y
			itoa(psheetsymbol->m_fY/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//x size
			itoa(psheetsymbol->m_fXsize/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y size
			itoa(psheetsymbol->m_fYsize/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// border width
			itoa(psheetsymbol->m_nBorderWidth, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// border Color
			itoa(psheetsymbol->m_nBorderColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// fill Color
			itoa(psheetsymbol->m_nFillColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//selection
			onelinestr += "0";  onelinestr += tspace;
			// 1 = solid
			if(psheetsymbol->m_bFillSolid == TRUE) onelinestr += "1";
			else onelinestr += "0";
			onelinestr += tspace;

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	
			
			POSITION posbak = pos;
			pos = posbak;
			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
			{
				pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext;
					ptext = (COb_schtext*)pobject;
					if(ptext->m_nID == TEXT_SHEETSYMBOLFILENAME)
					{
						// Filename
						onelinestr = tspace; onelinestr += tspace; onelinestr += tspace;
						onelinestr += "SheetFileName"; onelinestr += tspace;
						//x
						itoa(ptext->m_fSx/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//y
						itoa(ptext->m_fSy/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//orientation
						itoa(ptext->m_nOrientation/90, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// Color
						itoa(ptext->m_nColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//Font ID
						itoa(ptext->m_nFont+1, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//selection
						onelinestr += "0";  onelinestr += tspace;
						//1 == hidden
						if(ptext->m_bTextVisible == FALSE) onelinestr += "1";
						else onelinestr += "0";
						onelinestr += tspace;
						//Text
						onelinestr += "'";
						onelinestr += ptext->m_csText;
						onelinestr += "'";

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();		
						break;
					}
				}
			}
			pos = posbak;
			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
			{
				pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext;
					ptext = (COb_schtext*)pobject;
					if(ptext->m_nID == TEXT_SHEETSYMBOLDESIGNATOR)
					{
						// Sheet name
						onelinestr = tspace; onelinestr += tspace;onelinestr += tspace;
						onelinestr += "SheetName"; onelinestr += tspace;
						//x
						itoa(ptext->m_fSx/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//y
						itoa(ptext->m_fSy/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//orientation
						itoa(ptext->m_nOrientation/90, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// Color
						itoa(ptext->m_nColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//Font ID
						itoa(ptext->m_nFont+1, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//selection
						onelinestr += "0";  onelinestr += tspace;
						//1 == hidden
						if(ptext->m_bTextVisible == FALSE) onelinestr += "1";
						else onelinestr += "0";
						onelinestr += tspace;
						//Text
						onelinestr += "'";
						onelinestr += ptext->m_csText;
						onelinestr += "'";

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();		
						break;
					}					

				}
			}
			pos = posbak;
			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
			{
				pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
				{
					COb_schsheetentry* psheetentry;
					psheetentry = (COb_schsheetentry*)pobject;

					//SheetNet
					onelinestr = tspace; onelinestr += tspace;onelinestr += tspace;
					onelinestr += "SheetNet"; onelinestr += tspace;
					//input/output type
					itoa(psheetentry->m_nType, str, 10);
					onelinestr += str;  onelinestr += tspace;
					//arrow type
					itoa(psheetentry->m_nStyle, str, 10);
					onelinestr += str;  onelinestr += tspace;
					//position
					itoa(psheetentry->m_nSide, str, 10);
					onelinestr += str;  onelinestr += tspace;
					// Offset
					itoa(psheetentry->m_fOffset/100, str, 10);
					onelinestr += str;  onelinestr += tspace;
					// Border Color
					itoa(psheetentry->m_nBorderColor, str, 10);
					onelinestr += str;  onelinestr += tspace;
					// Fill Color
					itoa(psheetentry->m_nFillColor, str, 10);
					onelinestr += str;  onelinestr += tspace;
					// Text Color
					itoa(psheetentry->m_nTextColor, str, 10);
					onelinestr += str;  onelinestr += tspace;
					//selection
					onelinestr += "0";  onelinestr += tspace;
					//Text
					onelinestr += "'";
					onelinestr += psheetentry->m_csName;
					onelinestr += "'";

					onelinestr += csLineEnd;
					ar.WriteString(onelinestr);
					onelinestr.Empty();		
				}
			}

			// End of SheetSymbol
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "EndSheetSymbol"; onelinestr += tspace;
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != nCurrentDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			
			// Port
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "Port"; onelinestr += tspace;
			// arrow style
			itoa(pport->m_nStyle, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// input/output type
			itoa(pport->m_nType, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// alignment
			itoa(pport->m_nAlignment, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// width
			itoa(pport->m_fSize/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//x
			itoa(pport->m_fX/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y
			itoa(pport->m_fY/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// Color
			itoa(pport->m_nBorderColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// Fill Color
			itoa(pport->m_nFillColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// Text Color
			itoa(pport->m_nTextColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//selection
			onelinestr += "0";  onelinestr += tspace;
			//Text
			onelinestr += "'";
			onelinestr += pport->m_csName;
			onelinestr += "'";
			
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower;
			ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != nCurrentDiagram)||(ppower->m_nFlag == FLAG_DELETED))	continue;
			
			// PowerObject
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "PowerObject"; onelinestr += tspace;
			// style
			itoa(ppower->m_nStyle, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//x
			itoa(ppower->m_fX/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y
			itoa(ppower->m_fY/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//orientation
			itoa(ppower->m_nOrientation/90, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// Color
			itoa(ppower->m_nColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//selection
			onelinestr += "0";  onelinestr += tspace;
			//Text
			onelinestr += "'";
			onelinestr += ppower->m_csNetName;
			onelinestr += "'";
			
			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != nCurrentDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			
			// Bezier
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "Bezier"; onelinestr += tspace;
			// linewidth
			itoa(pbezier->m_nWidth, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// Color
			itoa(pbezier->m_nColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//selection
			onelinestr += "0";  onelinestr += tspace;
			//number of vertices
			itoa(pbezier->m_cVertex.GetCount(), str, 10);
			onelinestr += str;  onelinestr += tspace;
			//x vertex 1,  y vertex 1, ......
			for(int j=0; j<pbezier->m_cVertex.GetCount(); j++)
			{
				itoa(pbezier->m_cVertex[j].fx/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
				itoa(pbezier->m_cVertex[j].fy/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
			}

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
		{
			COb_schgraphic* pgraphic;
			pgraphic = (COb_schgraphic*)pobject;
			if((pgraphic->m_nDiagram != nCurrentDiagram)||(pgraphic->m_nFlag == FLAG_DELETED))	continue;

			// Image
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "Image"; onelinestr += tspace;
			//x1
			itoa(pgraphic->m_fX1/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y1
			itoa(pgraphic->m_fY1/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//x2
			itoa(pgraphic->m_fX2/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y2
			itoa(pgraphic->m_fY2/10, str, 10);
			onelinestr += str;  onelinestr += tspace;

			// linewidth
			itoa(pgraphic->m_nBorderWidth, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// Color
			itoa(pgraphic->m_nBorderColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//selection
			onelinestr += "0";  onelinestr += tspace;
			//solid
			onelinestr += "0";  onelinestr += tspace;
			//1=keey aspect ratio
			if(pgraphic->m_bAspect == TRUE) onelinestr += "1";
			else onelinestr += "0";
			onelinestr += tspace;
			//filename
			onelinestr += "'";
			onelinestr += pgraphic->m_csFileName;
			onelinestr += "'";

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram != nCurrentDiagram)||(pcomp->m_nFlag == FLAG_DELETED))
			{
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) m_listSchObject[nCurrentDiagram].GetNext(pos);
				continue;
			}
			
			//====== Part
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "Part"; onelinestr += tspace;
			//x
			itoa(pcomp->m_fX/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y
			itoa(pcomp->m_fY/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//display mode
			itoa(pcomp->m_nDisplayMode, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//1 == mirrered
			if(pcomp->m_bMirrored == TRUE) onelinestr += "1";
			else onelinestr += "0";
			onelinestr += tspace;
			//orientation
			itoa(pcomp->m_nOrientation/90, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//selection
			onelinestr += "0";  onelinestr += tspace;
			// Part ID
			itoa(pcomp->m_nPart, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//Comp Library ID
			itoa(nCompLibraryID, str, 10);
			onelinestr += str;  onelinestr += tspace;
			nCompLibraryID++;
			//show hide field
			onelinestr += "0";  onelinestr += tspace;
			//1 = show hidden pins
			if(pcomp->m_bShowHidePins == TRUE)  onelinestr += "1";
			else onelinestr += "0";
			onelinestr += tspace;
			//library reference
			onelinestr += "'";
			onelinestr += pcomp->m_csLibraryReference;
			onelinestr += "'";
			onelinestr += tspace;
			//footprint
			onelinestr += "'";
			onelinestr += pcomp->GetPackage();
			onelinestr += "'";

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	

			POSITION posbak = pos;
			pos = posbak;
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
			{
				pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext;
					ptext = (COb_schtext*)pobject;
					if(ptext->m_nID == TEXT_COMPDESIGNATOR)
					{
						//====== Designator
						onelinestr = tspace; onelinestr += tspace;
						onelinestr += "Designator"; onelinestr += tspace;
						//x
						itoa(ptext->m_fSx/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//y
						itoa(ptext->m_fSy/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//orientation
						itoa(ptext->m_nOrientation/90, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// color
						itoa(ptext->m_nColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// Font ID
						itoa(ptext->m_nFont + 1, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//selection
						onelinestr += "0";  onelinestr += tspace;
						//1 = hidden
						if(ptext->m_bTextVisible == FALSE)  onelinestr += "1";
						else onelinestr += "0";
						onelinestr += tspace;
						//Text
						onelinestr += "'";
						onelinestr += ptext->m_csText;
						onelinestr += "'";

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();		
						break;
					}
				}
			}
			pos = posbak;
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
			{
				pobject = m_listSchObject[nCurrentDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext;
					ptext = (COb_schtext*)pobject;
					if(ptext->m_nID == TEXT_COMPCOMMENT)
					{
						//====== Designator
						onelinestr = tspace; onelinestr += tspace;
						onelinestr += "PartType"; onelinestr += tspace;
						//x
						itoa(ptext->m_fSx/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//y
						itoa(ptext->m_fSy/10, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//orientation
						itoa(ptext->m_nOrientation/90, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// color
						itoa(ptext->m_nColor, str, 10);
						onelinestr += str;  onelinestr += tspace;
						// Font ID
						itoa(ptext->m_nFont + 1, str, 10);
						onelinestr += str;  onelinestr += tspace;
						//selection
						onelinestr += "0";  onelinestr += tspace;
						//1 = hidden
						if(ptext->m_bTextVisible == FALSE)  onelinestr += "1";
						else onelinestr += "0";
						onelinestr += tspace;
						//Text
						onelinestr += "'";
						onelinestr += ptext->m_csText;
						onelinestr += "'";

						onelinestr += csLineEnd;
						ar.WriteString(onelinestr);
						onelinestr.Empty();		
						break;
					}				
				}
			}

			for(int j=0; j<8; j++)
			{
				//====== PartDescription
				onelinestr = tspace; onelinestr += tspace;
				onelinestr += "PartDescription"; onelinestr += tspace;
				//x
				itoa(pcomp->m_fX/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//y
				itoa(pcomp->m_fY/10, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//orientation
				itoa(0, str, 10);
				onelinestr += str;  onelinestr += tspace;
				// color
				itoa(pcomp->m_nColor, str, 10);
				onelinestr += str;  onelinestr += tspace;
				// Font ID
				itoa(1, str, 10);
				onelinestr += str;  onelinestr += tspace;
				//selection
				onelinestr += "0";  onelinestr += tspace;
				//1 = hidden
				onelinestr += "1";
				onelinestr += tspace;
				//Text
				onelinestr += "''";

				onelinestr += csLineEnd;
				ar.WriteString(onelinestr);
				onelinestr.Empty();	
			}
			
			//SheetFartFileName
			onelinestr = tspace; onelinestr += tspace;
			onelinestr += "SheetPartFileName"; onelinestr += tspace;
			//x
			itoa(pcomp->m_fX/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//y
			itoa(pcomp->m_fY/10, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//orientation
			itoa(0, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// color
			itoa(pcomp->m_nColor, str, 10);
			onelinestr += str;  onelinestr += tspace;
			// Font ID
			itoa(1, str, 10);
			onelinestr += str;  onelinestr += tspace;
			//selection
			onelinestr += "0";  onelinestr += tspace;
			//1 = hidden
			onelinestr += "1";
			onelinestr += tspace;
			//Text
			onelinestr += "''";

			onelinestr += csLineEnd;
			ar.WriteString(onelinestr);
			onelinestr.Empty();	

			pos = posbak;
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) m_listSchObject[nCurrentDiagram].GetNext(pos);
		}
	}


	onelinestr = "EndSheet";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
}

void CESDDoc::Save_AD6_PCB_ASCII_File(CArchive& ar)
{
	//AfxMessageBox("Save: AD6 PCB");
	CString onelinestr;
	CString csLineEnd = "\r\n";
	CString csLineSoftEnd = "\r";
	CString keyword;
	char str[1000];

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	onelinestr += "|SELECTION=FALSE|LAYER=UNKNOWN|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0";
	onelinestr += "|FILENAME=|KIND=Protel_Advanced_PCB|VERSION=5.00|DATE=|TIME=";
	

	keyword = "|ORIGINX=";
	mil_to_str(m_cPCB_Sheet.m_fOriginX, str, WITH_MIL, 4);
	keyword += str;
	onelinestr += keyword;

	keyword = "|ORIGINY=";
	mil_to_str(m_cPCB_Sheet.m_fOriginY, str, WITH_MIL, 4);
	keyword += str;
	onelinestr += keyword;
	
	keyword = "|BIGVISIBLEGRIDSIZE=";
	mil_to_str(m_cPCB_Sheet.m_fBigVisibleGridSize*10000, str, WITHOUT_MIL, 3);
	keyword += str;
	onelinestr += keyword;

	keyword = "|VISIBLEGRIDSIZE=";
	mil_to_str(m_cPCB_Sheet.m_fVisibleGridSize*10000, str, WITHOUT_MIL, 3);
	keyword += str;
	onelinestr += keyword;

	keyword = "|ELECTRICALGRIDRANGE=";
	mil_to_str(m_cPCB_Sheet.m_fElecGridRange, str, WITH_MIL, 4);
	keyword += str;
	onelinestr += keyword;

	keyword = "|ELECTRICALGRIDENABLED=";
	if(m_cPCB_Sheet.m_bElecGridEnable == TRUE) keyword += "TRUE";
	else keyword += "FALSE";
	onelinestr += keyword;

	keyword = "|SNAPGRIDSIZE=";
	mil_to_str(m_cPCB_Sheet.m_fSnapGridSize*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;

	keyword = "|SNAPGRIDSIZEX=";
	mil_to_str(m_cPCB_Sheet.m_fSnapGridSizeX*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;
	
	keyword = "|SNAPGRIDSIZEY=";
	mil_to_str(m_cPCB_Sheet.m_fSnapGridSizeY*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;
	
	keyword = "|TRACKGRIDSIZE=";
	mil_to_str(m_cPCB_Sheet.m_fTrackGridSize*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;
	
	keyword = "|VIAGRIDSIZE=";
	mil_to_str(m_cPCB_Sheet.m_fViaGridSize*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;

	keyword = "|COMPONENTGRIDSIZE=";
	mil_to_str(m_cPCB_Sheet.m_fComponentGridSize*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;

	keyword = "|COMPONENTGRIDSIZEX=";
	mil_to_str(m_cPCB_Sheet.m_fComponentGridSizeX*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;

	keyword = "|COMPONENTGRIDSIZEY=";
	mil_to_str(m_cPCB_Sheet.m_fComponentGridSizeY*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;


	keyword = "|DOTGRID=";
	if(m_cPCB_Sheet.m_bDotGrid == TRUE) keyword += "TRUE";
	else keyword += "FALSE";
	onelinestr += keyword;
	
	keyword = "|DISPLAYUNIT=";
	itoa(m_cPCB_Sheet.m_nUnit, str, 10);
	keyword += str;
	onelinestr += keyword;

	keyword = "|DESIGNATORDISPLAYMODE=";
	itoa(m_cPCB_Sheet.m_nDesignatorDisplayMode, str, 10);
	keyword += str;
	onelinestr += keyword;

	onelinestr += "|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|PRIMITIVELOCK=TRUE|POLYGONTYPE=Polygon|POUROVER=FALSE|REMOVEDEAD=FALSE|GRIDSIZE=10mil|TRACKWIDTH=10mil|HATCHSTYLE=None|USEOCTAGONS=FALSE|MINPRIMLENGTH=3mil";
	for(int i=0; i<m_cPCB_Sheet.m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex vertex;
		vertex = m_cPCB_Sheet.m_cVertex.GetAt(i);
		char numstr[100];
		itoa(i, numstr, 10);  strcat(numstr, "=");

		keyword = "|KIND";	keyword += numstr;
		if(vertex.radius != 0) keyword += "1";
		else keyword += "0";
		onelinestr += keyword;

		keyword = "|VX";	keyword += numstr;
		mil_to_str(vertex.fx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|VY";	keyword += numstr;
		mil_to_str(vertex.fy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|CX";	keyword += numstr;
		mil_to_str(vertex.cx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|CY";	keyword += numstr;
		mil_to_str(vertex.cy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|SA";	keyword += numstr;	keyword += " ";
		sprintf(str, "%.14E", vertex.sangle);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|EA";	keyword += numstr;	keyword += " ";
		sprintf(str, "%.14E", vertex.eangle);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|R";	keyword += numstr;
		mil_to_str(vertex.radius, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
	}

	onelinestr += "|SHELVED=FALSE|RESTORELAYER=TOP|RESTORENET=|REMOVEISLANDSBYAREA=TRUE|REMOVENECKS=TRUE|AREATHRESHOLD=250000000000.000000|ARCRESOLUTION=0.5mil|NECKWIDTHTHRESHOLD=5mil|POUROVERSTYLE=0|NAME=|POURINDEX=-1|IGNOREVIOLATIONS=FALSE";

	keyword = "|SHEETX=";
	mil_to_str(m_cPCB_Sheet.m_fSheetX, str, WITH_MIL, 4);
	keyword += str;
	onelinestr += keyword;
	
	keyword = "|SHEETY=";
	mil_to_str(m_cPCB_Sheet.m_fSheetY, str, WITH_MIL, 4);
	keyword += str;
	onelinestr += keyword;
	
	keyword = "|SHEETWIDTH=";
	mil_to_str(m_cPCB_Sheet.m_fSheetWidth, str, WITH_MIL, 4);
	keyword += str;
	onelinestr += keyword;
	
	keyword = "|SHEETHEIGHT=";
	mil_to_str(m_cPCB_Sheet.m_fSheetHeight, str, WITH_MIL, 4);
	keyword += str;
	onelinestr += keyword;
	
	keyword = "|SHOWSHEET=";
	if(m_cPCB_Sheet.m_bShowSheet == TRUE) keyword += "TRUE";
	else keyword += "FALSE";
	onelinestr += keyword;
	
	keyword = "|LOCKSHEET=";
	if(m_cPCB_Sheet.m_bLockSheet == TRUE) keyword += "TRUE";
	else keyword += "FALSE";
	onelinestr += keyword;
	
	for(int i=0; i<POWER_LAYER_NUM; i++)
	{
		char layernum[100];
		itoa(i+1, layernum, 10);
		keyword = "|PLANE";
		keyword += layernum;
		keyword += "NETNAME=";
		if(m_csPowerPlane[i].IsEmpty() == TRUE) keyword += "(No Net)";
		else if(m_csPowerPlane[i] == "ŒﬁÕ¯¬Á") keyword += "(No Net)";
		else if(m_csPowerPlane[i] == "∂‡Õ¯¬Á") keyword += "(Multiple Nets)";
		else keyword += m_csPowerPlane[i];
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	onelinestr += "|TOPTYPE=3|TOPCONST=3.500|TOPHEIGHT=0.4mil|TOPMATERIAL=Solder Resist";
	onelinestr += "|BOTTOMTYPE=3|BOTTOMCONST=3.500|BOTTOMHEIGHT=0.4mil|BOTTOMMATERIAL=Solder Resist";
	onelinestr += "|LAYERSTACKSTYLE=0|SHOWTOPDIELECTRIC=FALSE|SHOWBOTTOMDIELECTRIC=FALSE";

	for(int i=1; i<=5; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=6; i<=10; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=11; i<=15; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=16; i<=20; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=21; i<=25; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=26; i<=30; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=31; i<=35; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=36; i<=40; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=41; i<=45; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=46; i<=50; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=51; i<=55; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=56; i<=60; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=61; i<=65; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=66; i<=70; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=71; i<=75; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=76; i<=80; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=81; i<=82; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	for(int i=0; i<POWER_LAYER_NUM; i++)
	{
		char layernum[100];
		itoa(i+1, layernum, 10);

		keyword = "|PLANE";
		keyword += layernum;
		keyword += "PULLBACK=";
		mil_to_str(m_fPlanePullBack[i], str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
	}

	onelinestr += "|LAYERPAIR0LOW=TOP|LAYERPAIR0HIGH=BOTTOM|LAYERPAIR0DRILLGUIDE=FALSE|LAYERPAIR0DRILLDRAWING=FALSE";

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;

	keyword = "|TOGGLELAYERS=";
	for(int i=0; i<PCB_LAYER_NUM; i++)
	{
		if(m_sPCB_LayerParam[i].bLayerShow == TRUE) keyword += "1";
		else keyword += "0";
	}
	onelinestr += keyword;

	for(int i=0; i<10; i++)
	{
		char num[100];
		itoa(i+1, num, 10);
		
		keyword = "|PLACEMARKERX";
		keyword += num;
		keyword += "=";
		mil_to_str(m_fPCB_Marker[i].fx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|PLACEMARKERY";
		keyword += num;
		keyword += "=";
		mil_to_str(m_fPCB_Marker[i].fy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
	}

	for(int i=0; i<8; i++)
	{
		char num[100];
		itoa(i+1, num, 10);
		
		keyword = "|SELECTIONMEMORYLOCK";
		keyword += num;
		keyword += "=";
		if(m_bSelectionMemoryLock[i] == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
	}

	onelinestr += "|AUTOMATICSPLITPLANES=TRUE";
	onelinestr += "|SURFACEMICROSTRIP_I=(87/SQRT(Er+1.41))*LN(5.98*TraceToPlaneDistance/(0.8*TraceWidth + TraceHeight))";
	onelinestr += "|SURFACEMICROSTRIP_W=((5.98*TraceToPlaneDistance/EXP(CharacteristicImpedance*SQRT(Er+1.41)/87))- TraceHeight)*1.25";
	onelinestr += "|SYMMETRICSTRIPLINE_I=(60/SQRT(Er))*LN((1.9*PlaneToPlaneDistance)/(0.8*TraceWidth + TraceHeight))";
	onelinestr += "|SYMMETRICSTRIPLINE_W=((1.9*PlaneToPlaneDistance/EXP(CharacteristicImpedance/(60/SQRT(Er))))- TraceHeight)*1.25";

	onelinestr += "|ELECTRICALGRIDSNAPTOBO=FALSE|ELECTRICALGRIDUSEALLLAYERS=FALSE";
	
	onelinestr += "|ROUTINGDIRECTIONTOP LAYER=Automatic";
	for(int i=0; i<30; i++)
	{
		char num[100];
		itoa(i+1, num, 10);
		keyword = "|ROUTINGDIRECTIONMID LAYER ";
		keyword += num;
		keyword += "=Automatic";
		onelinestr += keyword;
	}
	onelinestr += "|ROUTINGDIRECTIONBOTTOM LAYER=Automatic";
	
	onelinestr += "|TOPLAYER_MRLASTWIDTH=10mil";
	for(int i=0; i<30; i++)
	{
		char num[100];
		itoa(i+1, num, 10);
		keyword = "|MIDLAYER";
		keyword += num;
		keyword += "_MRLASTWIDTH=10mil";
		onelinestr += keyword;
	}
	onelinestr += "|BOTTOMLAYER_MRLASTWIDTH=10mil";

	onelinestr += "|MRLASTVIASIZE=50mil|MRLASTVIAHOLE=28mil";
	onelinestr += "|LASTTARGETLENGTH=99999mil|SHOWDEFAULTSETS=TRUE";
		
	onelinestr += "|LAYERSETSCOUNT=5|LAYERSET1NAME=&All Layers|LAYERSET1LAYERS=TopLayer,BottomLayer,TopOverlay,BottomOverlay,TopPaste,BottomPaste,TopSolder,BottomSolder,DrillGuide,KeepOutLayer,Mechanical1,DrillDrawing,MultiLayer";
	onelinestr += "|LAYERSET1ACTIVELAYER=1|LAYERSET1ISCURRENT=FALSE|LAYERSET1ISLOCKED=TRUE|LAYERSET1FLIPBOARD=FALSE";
	onelinestr += "|LAYERSET2NAME=&Signal Layers|LAYERSET2LAYERS=TopLayer,BottomLayer,MultiLayer|LAYERSET2ACTIVELAYER=1|LAYERSET2ISCURRENT=FALSE|LAYERSET2ISLOCKED=TRUE|LAYERSET2FLIPBOARD=FALSE";
	onelinestr += "|LAYERSET3NAME=&Plane Layers|LAYERSET3LAYERS=|LAYERSET3ACTIVELAYER=0|LAYERSET3ISCURRENT=FALSE|LAYERSET3ISLOCKED=TRUE|LAYERSET3FLIPBOARD=FALSE";
	onelinestr += "|LAYERSET4NAME=&NonSignal Layers|LAYERSET4LAYERS=TopOverlay,BottomOverlay,TopPaste,BottomPaste,TopSolder,BottomSolder,DrillGuide,KeepOutLayer,DrillDrawing,MultiLayer|LAYERSET4ACTIVELAYER=33";
	onelinestr += "|LAYERSET4ISCURRENT=FALSE|LAYERSET4ISLOCKED=TRUE|LAYERSET4FLIPBOARD=FALSE";
	onelinestr += "|LAYERSET5NAME=&Mechanical Layers|LAYERSET5LAYERS=Mechanical1|LAYERSET5ACTIVELAYER=57|LAYERSET5ISCURRENT=FALSE|LAYERSET5ISLOCKED=TRUE|LAYERSET5FLIPBOARD=FALSE";

	onelinestr += "|BOARDINSIGHTVIEWCONFIGURATIONNAME=";

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	onelinestr = "|RECORD=Board|VISIBLEGRIDMULTFACTOR=0.000|BIGVISIBLEGRIDMULTFACTOR=0.000|ELECTRICALGRIDMULTFACT=0.000";

	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	onelinestr += "|RECORD=EngineeringChangeOrderOptions|ECOISACTIVE=FALSE|ECOFILENAME=PCB.ECO";
	//Ω·Œ≤
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	onelinestr += "|RECORD=OutputOptions|DRILLGUIDEHOLESIZE=30mil|DRILLDRAWSYMBOLSIZE=50mil|DRILLSYMBOLKIND=0|MULTILAYERONPADMASTER=TRUE|TOPLAYERONPADMASTER=TRUE|BOTTOMLAYERONPADMASTER=TRUE";
	onelinestr += "|INCLUDEVIASINSOLDERMASK=TRUE|INCLUDEUNCONNECTEDPADS=TRUE";
	onelinestr += "|INCLUDEMECH1WITHALLPLOTS=FALSE|INCLUDEMECH2WITHALLPLOTS=FALSE|INCLUDEMECH3WITHALLPLOTS=FALSE|INCLUDEMECH4WITHALLPLOTS=FALSE";
	onelinestr += "|INCLUDEMECH5WITHALLPLOTS=FALSE|INCLUDEMECH6WITHALLPLOTS=FALSE|INCLUDEMECH7WITHALLPLOTS=FALSE|INCLUDEMECH8WITHALLPLOTS=FALSE";
	onelinestr += "|INCLUDEMECH9WITHALLPLOTS=FALSE|INCLUDEMECH10WITHALLPLOTS=FALSE|INCLUDEMECH11WITHALLPLOTS=FALSE|INCLUDEMECH12WITHALLPLOTS=FALSE";
	onelinestr += "|INCLUDEMECH13WITHALLPLOTS=FALSE|INCLUDEMECH14WITHALLPLOTS=FALSE|INCLUDEMECH15WITHALLPLOTS=FALSE|INCLUDEMECH16WITHALLPLOTS=FALSE";

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	
	onelinestr += "|RECORD=OutputOptions|PLOTLAYERS=0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	onelinestr += "|FLIPLAYERS=0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	

	onelinestr += "|RECORD=PrinterOptions|DEVICE=|DRIVER=|OUTPUT=|SHOWHOLES=FALSE|SCALETOFITPAGE=FALSE|USEPRINTERFONTS=FALSE|USESOFTWAREARCS=FALSE|BATCHTYPE=0|COMPOSITETYPE=0|CBORDERSIZE=1000mil|SCALE=0.0001mil|XCORRECT=0.0001mil|YCORRECT=0.0001mil";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PLOTPADNETS=TRUE|PLOTPADNUMBERS=TRUE|PLOTTERSCALE=0.0001mil|PLOTTERXCORRECT=0.0001mil|PLOTTERYCORRECT=0.0001mil|PLOTTERXOFFSET=0mil|PLOTTERYOFFSET=0mil|PLOTTERSHOWHOLES=FALSE|PLOTTERUSESOFTWAREARCS=FALSE";
	onelinestr += "|PLOTTERWAITBETWEENSHEETS=FALSE|PLOTTEROUTPUTPORT=0|PLOTTERLANGUAGE=0";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|SPD1=3|THK1=13mil|SPD2=3|THK2=13mil|SPD3=3|THK3=13mil|SPD4=3|THK4=13mil|SPD5=3|THK5=13mil|SPD6=3|THK6=13mil|SPD7=3|THK7=13mil|SPD8=3|THK8=13mil";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PLOTMODE=000000000000000000000|DRIVERTYPE=0";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PLOTLAYERS=1111111111111111111111111111111111111111111111111111111111111111111111111111111111";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PP1=1|PP2=1|PP3=1|PP4=1|PP5=1|PP6=1|PP7=1|PP8=1|PP9=1|PP10=1|PP11=1|PP12=1|PP13=1|PP14=1|PP15=1|PP16=1|PP17=1|PP18=1|PP19=1|PP20=1|PP21=1|PP22=1|PP23=1|PP24=1|PP25=1|PP26=1|PP27=1|PP28=1";
	onelinestr += "|PP29=1|PP30=1|PP31=1|PP32=1|PP33=1|PP34=1|PP35=1|PP36=1|PP37=1|PP38=1|PP39=1|PP40=1|PP41=1|PP42=1|PP43=1|PP44=1|PP45=1|PP46=1|PP47=1|PP48=1|PP49=1|PP50=1|PP51=1|PP52=1|PP53=1|PP54=1|PP55=1|PP56=1|PP57=1|PP58=1";
	onelinestr += "|PP59=1|PP60=1|PP61=1|PP62=1|PP63=1|PP64=1|PP65=1|PP66=1|PP67=1|PP68=1|PP69=1|PP70=1|PP71=1|PP72=1|PP73=1|PP74=1|PP75=1|PP76=1|PP77=1|PP78=1|PP79=1|PP80=1|PP81=1|PP82=1";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM1=2105376|PM2=197379|PM3=394758|PM4=592137|PM5=789516|PM6=986895|PM7=1184274|PM8=1381653|PM9=1579032|PM10=1776411";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM11=1973790|PM12=2171169|PM13=2368548|PM14=2565927|PM15=2763306|PM16=2960685|PM17=3158064|PM18=3355443|PM19=3552822|PM20=3750201";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM21=3947580|PM22=4144959|PM23=4342338|PM24=4539717|PM25=4737096|PM26=4934475|PM27=5131854|PM28=5329233|PM29=5526612|PM30=5723991";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM31=5921370|PM32=4210752|PM33=12632256|PM34=6513507|PM35=6710886|PM36=6908265|PM37=7105644|PM38=7303023|PM39=7500402|PM40=7697781";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM41=7895160|PM42=8092539|PM43=8289918|PM44=8487297|PM45=8684676|PM46=8882055|PM47=9079434|PM48=9276813|PM49=9474192|PM50=9671571";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM51=9868950|PM52=10066329|PM53=10263708|PM54=10461087|PM55=10658466|PM56=10855845|PM57=11053224|PM58=11250603|PM59=11447982|PM60=11645361";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM61=11842740|PM62=12040119|PM63=12237498|PM64=12434877|PM65=12632256|PM66=12829635|PM67=13027014|PM68=13224393|PM69=13421772|PM70=13619151";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM71=13816530|PM72=14013909|PM73=14211288|PM74=0|PM75=14606046|PM76=16777215|PM77=15000804|PM78=15198183|PM79=15395562|PM80=15592941";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM81=15790320|PM82=15987699";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC1=255|PC2=36540|PC3=16440176|PC4=6736896|PC5=16737945|PC6=16776960|PC7=8388736|PC8=16711935|PC9=32896|PC10=65535";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC11=8421504|PC12=16777215|PC13=8388736|PC14=8421376|PC15=12632256|PC16=128|PC17=32768|PC18=65280|PC19=8388608|PC20=16776960";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC21=8388736|PC22=16711935|PC23=32896|PC24=65535|PC25=8421504|PC26=16777215|PC27=8388736|PC28=8421376|PC29=12632256|PC30=128";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC31=32768|PC32=16711680|PC33=65535|PC34=32896|PC35=8421504|PC36=128|PC37=8388736|PC38=16711935|PC39=32768|PC40=128";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC41=8388736|PC42=8421376|PC43=32768|PC44=128|PC45=8388736|PC46=8421376|PC47=32768|PC48=128|PC49=8388736|PC50=8421376";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC51=32768|PC52=128|PC53=8388736|PC54=8421376|PC55=128|PC56=16711935|PC57=16711935|PC58=8388736|PC59=32768|PC60=32896";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC61=16711935|PC62=8388736|PC63=32768|PC64=32896|PC65=16711935|PC66=8388736|PC67=32768|PC68=32896|PC69=16711935|PC70=8388736";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC71=32768|PC72=0|PC73=2752767|PC74=12632256|PC75=7709086|PC76=0|PC77=65280|PC78=16777215|PC79=6049101|PC80=9473425";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC81=9474304|PC82=25217";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	

	onelinestr += "|RECORD=GerberOptions|SORTOUTPUT=FALSE|USESOFTWAREARCS=TRUE|CENTERPHOTOPLOTS=TRUE|EMBEDAPERTURES=FALSE|PANELIZE=FALSE|G54=FALSE|PLUSTOL=0.005mil|MINUSTOL=0.005mil|FILMSIZEX=20000mil";
	onelinestr += "|FILMSIZEY=16000mil|BORDERSIZE=1000mil|APTTABLE=|MAXAPERSIZE=250mil|RELIEFSHAPESALLOWED=TRUE|PADSFLASHONLY=TRUE|GERBERUNITS=3|GERBERDECS=1|FLASHALLFILLS=FALSE";

	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	

	onelinestr += "|RECORD=AdvancedPlacerOptions|PLACELARGECLEAR=50mil|PLACESMALLCLEAR=20mil|PLACEUSEROTATION=TRUE|PLACEUSELAYERSWAP=FALSE|PLACEBYPASSNET1=|PLACEBYPASSNET2=|PLACEUSEADVANCEDPLACE=TRUE|PLACEUSEGROUPING=TRUE";

	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	

	onelinestr += "|RECORD=DesignRuleCheckerOptions|DOMAKEDRCFILE=TRUE|DOMAKEDRCERRORLIST=TRUE|DOSUBNETDETAILS=TRUE|REPORTFILENAME=|EXTERNALNETLISTFILENAME=|CHECKEXTERNALNETLIST=FALSE|MAXVIOLATIONCOUNT=500";
	onelinestr += "|REPORTDRILLEDSMTPADS=TRUE|REPORTINVALIDMULTILAYERPADS=TRUE|RULESETTOCHECK=0,1,2,3,4,5,15,16,18,19,21,22,23,42,45,46,47,50|ONLINERULESETTOCHECK=0,1,2,3,4,5,15,17,18,19,21,22,23,24,25,42,43,44,45,46,47,48,50|INTERNALPLANEWARNINGS=TRUE|VERIFYSHORTINGCOPPER=TRUE";

	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	
	onelinestr += "|RECORD=PinSwapOptions|QUIET=FALSE|APPROXIMATEPINPOSITIONS=FALSE|ALLOWPARTIALLYROUTEDCONNECTIONS=TRUE|VIAPENALTYSTATE=TRUE|CROSSOVERRATIO=50|VIAPENALTYVALUE=0|IGNORENETS=|IGNORENETCLASSES=";
	onelinestr += "|IGNORECOMPONENTS=|IGNOREDIFFERENTIALPAIRS=|HEURISTICNAME=|HEURISTICONOFFSTATE=|HEURISTICWEIGHTVALUE=";

	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	

	for(int i=0; i<m_arrayPcbClass.GetCount(); i++)
	{
		COb_pcbclass* pclass = (COb_pcbclass*)m_arrayPcbClass.GetAt(i);

		keyword = "|RECORD=Class";
		onelinestr += keyword;

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0";
		onelinestr += keyword;

		keyword = "|NAME=";
		keyword += pclass->m_csName;
		onelinestr += keyword;

		keyword = "|KIND=";
		itoa(pclass->m_nKind, str, 10);
		keyword += str;
		onelinestr += keyword;
			
		keyword = "|SUPERCLASS=";
		if(pclass->m_bSuperClass == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;	

		for(int j=0; j<pclass->m_arrayMember.GetCount(); j++)
		{
			char numstr[100];
			itoa(j, numstr, 10);  strcat(numstr, "=");

			keyword = "|M";	keyword += numstr;
			keyword += pclass->m_arrayMember.GetAt(j);
			onelinestr += keyword;
		}

		onelinestr += "|SELECTED=FALSE";

		//Ω·Œ≤
		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	CArray<POINT, POINT> arrayNet;
	CArray<POINT, POINT> arrayComp;
	CArray<POINT, POINT> arrayPolygon;
	int newindex;

	newindex = 0;
	for(int i=0; i<m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = (COb_pcbnet*)m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;
	
		POINT point;
		point.x = i;
		point.y = newindex;
		arrayNet.Add(point);
		
		keyword = "|RECORD=Net";
		onelinestr += keyword;

		keyword = "|ID=";
		itoa(newindex, str, 10);
		keyword += str;
		onelinestr += keyword;
		newindex++;

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|PRIMITIVELOCK=FALSE";
		onelinestr += keyword;

		keyword = "|NAME=";
		keyword += pnet->m_csNetName;
		onelinestr += keyword;

		keyword = "|VISIBLE=";
		if(pnet->m_bVisible == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;	
		
		keyword = "|COLOR=";
		itoa(pnet->m_nColor, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|LOOPREMOVAL=TRUE";
		onelinestr += keyword;
		
		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	newindex = 0;
	for(int i=0; i<m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = (COb_pcbcomp*)m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag != FLAG_NORMAL) continue;
		
		POINT point;
		point.x = i;
		point.y = newindex;
		arrayComp.Add(point);
		
		keyword = "|RECORD=Component";
		onelinestr += keyword;

		keyword = "|ID=";
		itoa(newindex, str, 10);
		keyword += str;
		onelinestr += keyword;
		newindex++;

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[pcomp->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(pcomp->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(pcomp->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|USERROUTED=";
		if(pcomp->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|PRIMITIVELOCK=";
		if(pcomp->m_bPrimitiveLock == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|X=";
		mil_to_str(pcomp->m_fX, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|Y=";
		mil_to_str(pcomp->m_fY, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|PATTERN=";
		keyword += pcomp->m_csPattern;
		onelinestr += keyword;
		
		keyword = "|NAMEON=";
		if(pcomp->m_bNameOn == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|COMMENTON=";
		if(pcomp->m_bCommentOn == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|GROUPNUM=";
		itoa(pcomp->m_nGroupNum, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|COUNT=";
		itoa(pcomp->m_nCount, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|ROTATION=";
		sprintf(str, "%.14E", pcomp->m_fRotation);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|HEIGHT=";
		mil_to_str(pcomp->m_fHeight, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|NAMEAUTOPOSITION=";
		itoa(pcomp->m_nNamePosition, str, 10);
		keyword += str;
		onelinestr += keyword;			
		
		keyword = "|COMMENTAUTOPOSITION=";
		itoa(pcomp->m_nCommentPosition, str, 10);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|UNIONINDEX=";
		itoa(pcomp->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		if(pcomp->m_bLockStrings == TRUE) onelinestr += "|LOCKSTRINGS=TRUE";
		if(pcomp->m_bEnablePinSwapping == TRUE) onelinestr += "|ENABLEPINSWAPPING=TRUE";
		if(pcomp->m_bEnablePartSwapping == TRUE) onelinestr += "|ENABLEPARTSWAPPING=TRUE";

		if(pcomp->m_csSourceDesignator.IsEmpty() == FALSE) onelinestr += "|SOURCEDESIGNATOR=" + pcomp->m_csSourceDesignator;
		if(pcomp->m_csSourceFootprintLibrary.IsEmpty() == FALSE) onelinestr += "|SOURCEFOOTPRINTLIBRARY=" + pcomp->m_csSourceFootprintLibrary;
		if(pcomp->m_csFootprintDescription.IsEmpty() == FALSE) onelinestr += "|FOOTPRINTDESCRIPTION=" + pcomp->m_csFootprintDescription;
		if(pcomp->m_csSourceComponentLibrary.IsEmpty() == FALSE) onelinestr += "|SOURCECOMPONENTLIBRARY=" + pcomp->m_csSourceComponentLibrary;
		if(pcomp->m_csSourceDescription.IsEmpty() ==  FALSE) onelinestr += "|SOURCEDESCRIPTION=" + pcomp->m_csSourceDescription;

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	int dimension_num = -1;
	for(int i=0; i<m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = (COb_pcbdimension*)m_arrayPcbDimension.GetAt(i);
		if(pdimension->m_nFlag != FLAG_NORMAL) continue;
		if(pdimension->m_nDimensionKind != 8) continue;
		dimension_num++;
	
		keyword = "|RECORD=Dimension";
		onelinestr += keyword;

		keyword = "|INDEXFORSAVE=";
		itoa(dimension_num, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[pdimension->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(pdimension->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(pdimension->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|USERROUTED=";
		if(pdimension->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|UNIONINDEX=";
		itoa(pdimension->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|PRIMITIVELOCK=";
		if(pdimension->m_bPrimitiveLock == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|DIMENSIONLAYER=";
		keyword += csEDA_PCB_LayerLabel[pdimension->m_nLayer];
		onelinestr += keyword;

		keyword = "|DIMENSIONLOCKED=";
		if(pdimension->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		onelinestr += "|OBJECTID=13";
		
		keyword = "|DIMENSIONKIND=";
		itoa(pdimension->m_nDimensionKind, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|DRCERROR=FALSE";

		keyword = "|VINDEXFORSAVE=";
		itoa(dimension_num, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LX=";
		mil_to_str(pdimension->m_fLx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|LY=";
		mil_to_str(pdimension->m_fLy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|HX=";
		mil_to_str(pdimension->m_fHx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|HY=";
		mil_to_str(pdimension->m_fHy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|X1=";
		mil_to_str(pdimension->m_fXs, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|Y1=";
		mil_to_str(pdimension->m_fYs, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|X2=";
		mil_to_str(pdimension->m_fXe, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|Y2=";
		mil_to_str(pdimension->m_fYe, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTX=";
		mil_to_str(pdimension->m_fTextSx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;		
		keyword = "|TEXTY=";
		mil_to_str(pdimension->m_fTextSy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;					
		keyword = "|HEIGHT=";
		mil_to_str(pdimension->m_fHeight, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;					
		keyword = "|LINEWIDTH=";
		mil_to_str(pdimension->m_fLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTHEIGHT=";
		mil_to_str(pdimension->m_fTextHeight, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|TEXTWIDTH=";
		mil_to_str(pdimension->m_fTextLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|FONT=";
		keyword += csPCB_StrokeFontName[pdimension->m_nStrokeFont];
		onelinestr += keyword;
		
		keyword = "|STYLE=";
		if(pdimension->m_nUnitStyle == UNIT_STYLE_NORMAL) keyword += "Normal";
		else if(pdimension->m_nUnitStyle == UNIT_STYLE_BRACKETS) keyword += "Brackets";
		else keyword += "None";
		onelinestr += keyword;

		keyword = "|TEXTLINEWIDTH=";
		mil_to_str(pdimension->m_fTextLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|TEXTPOSITION=Auto";

		keyword = "|TEXTGAP=";
		mil_to_str(pdimension->m_fTextGap, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTFORMAT=";
		if(pdimension->m_nTextFormat == 2) keyword += "10mil";
		else if(pdimension->m_nTextFormat == 3) keyword += "10 (mil)";
		else keyword += "10";
		onelinestr += keyword;

		keyword = "|TEXTDIMENSIONUNIT=";
		if(pdimension->m_nTextDimensionUnit == UNIT_MIL) keyword += "Mils";
		else if(pdimension->m_nTextDimensionUnit == UNIT_MM) keyword += "Millimeters";
		else if(pdimension->m_nTextDimensionUnit == UNIT_INCH) keyword += "Inches";
		else if(pdimension->m_nTextDimensionUnit == UNIT_CM) keyword += "Centimeters";
		else keyword += "AutomaticUnit";
		onelinestr += keyword;
		
		keyword = "|TEXTPRECISION=";
		itoa(pdimension->m_nTextPrecision, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTPREFIX=";
		keyword += pdimension->m_csPrefix;
		onelinestr += keyword;
		
		keyword = "|TEXTSUFFIX=";
		keyword += pdimension->m_csSuffix;
		onelinestr += keyword;		
		
		keyword = "|ARROWSIZE=";
		mil_to_str(pdimension->m_fArrowSize, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;		

		keyword = "|ARROWLINEWIDTH=";
		mil_to_str(pdimension->m_fArrowLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|ARROWLENGTH=";
		mil_to_str(pdimension->m_fArrowLength, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|ARROWPOSITION=";
		if(pdimension->m_nArrowPosition == 0) keyword += "Inside";
		else keyword += "Outside";
		onelinestr += keyword;	

		keyword = "|EXTENSIONOFFSET=";
		mil_to_str(pdimension->m_fExtensionOffset, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	
		
		keyword = "|EXTENSIONLINEWIDTH=";
		mil_to_str(pdimension->m_fExtensionLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|EXTENSIONPICKGAP=";
		mil_to_str(pdimension->m_fExtensionPickGap, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|REFERENCES_COUNT=";
		itoa(pdimension->m_cRefPoint.GetCount(), str, 10);
		keyword += str;
		onelinestr += keyword;

		for(int j=0; j<pdimension->m_cRefPoint.GetCount(); j++)
		{
			Fpoint fpoint = pdimension->m_cRefPoint.GetAt(j);
			char numstr[100];
			itoa(j, numstr, 10);
			keyword = "|REFERENCE"; keyword += numstr; keyword += "PRIM=-1";
			keyword += "|REFERENCE"; keyword += numstr; keyword += "OBJECTID=-1";
			keyword += "|REFERENCE"; keyword += numstr; keyword += "OBJECTSTRING=NoObject";
			keyword += "|REFERENCE"; keyword += numstr; keyword += "POINTX=";
			mil_to_str(fpoint.fx, str, WITH_MIL, 4);
			keyword += str;
			keyword += "|REFERENCE"; keyword += numstr; keyword += "POINTY=";
			mil_to_str(fpoint.fy, str, WITH_MIL, 4);
			keyword += str;
			keyword += "|REFERENCE"; keyword += numstr; keyword += "ANCHOR=-1";
			onelinestr += keyword;
		}

		keyword = "|TEXT1X=";
		mil_to_str(pdimension->m_fTextSx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	
		
		keyword = "|TEXT1Y=";
		mil_to_str(pdimension->m_fTextSy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	
		
		keyword = "|TEXT1ANGLE=";
		sprintf(str, "%.14E", pdimension->m_fTextRotation);
		keyword += str;
		onelinestr += keyword;	

		onelinestr += "|TEXT1MIRROR=FALSE";

		keyword = "|USETTFONTS=";
		if(pdimension->m_bUseTTFont == FALSE) keyword += "FALSE";
		else keyword += "TRUE";
		onelinestr += keyword;	
		
		keyword = "|BOLD=";
		if(pdimension->m_bBold == FALSE) keyword += "FALSE";
		else keyword += "TRUE";
		onelinestr += keyword;	

		keyword = "|ITALIC=";
		if(pdimension->m_bItalic == FALSE) keyword += "FALSE";
		else keyword += "TRUE";
		onelinestr += keyword;	

		keyword = "|FONTNAME=";
		keyword += pdimension->m_csTTFontName;
		onelinestr += keyword;	
		
		//Ω·Œ≤
		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}
    
	for(int i=0; i<m_arrayPcbCoordinate.GetCount(); i++)
	{
		COb_pcbcoordinate* pcoordinate = (COb_pcbcoordinate*)m_arrayPcbCoordinate.GetAt(i);
		if(pcoordinate->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Coordinate";
		onelinestr += keyword;

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[pcoordinate->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(pcoordinate->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(pcoordinate->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|USERROUTED=";
		if(pcoordinate->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|UNIONINDEX=";
		itoa(pcoordinate->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|PRIMITIVELOCK=";
		if(pcoordinate->m_bPrimitiveLock == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|X=";
		mil_to_str(pcoordinate->m_fX, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y=";
		mil_to_str(pcoordinate->m_fY, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SIZE=";
		mil_to_str(pcoordinate->m_fSize, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LINEWIDTH=";
		mil_to_str(pcoordinate->m_fLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTHEIGHT=";
		mil_to_str(pcoordinate->m_fTextHeight, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTWIDTH=";
		mil_to_str(pcoordinate->m_fTextWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|FONT=";
		keyword += csPCB_StrokeFontName[pcoordinate->m_nStrokeFont];
		onelinestr += keyword;

		if(pcoordinate->m_nUnitStyle != 0)
		{
			keyword = "|STYLE=";
			if(pcoordinate->m_nUnitStyle == UNIT_STYLE_NORMAL) keyword += "Normal";
			else if(pcoordinate->m_nUnitStyle == UNIT_STYLE_BRACKETS) keyword += "Brackets";
			onelinestr += keyword;
		}
		
		keyword = "|ROTATION=";
		sprintf(str, "%.14E", pcoordinate->m_fRotation);
        keyword += str;
		onelinestr += keyword;
		
		keyword = "|USETTFONTS=";
		if(pcoordinate->m_bUseTTFont == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|BOLD=";
		if(pcoordinate->m_bBold == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;		
		
		keyword = "|ITALIC=";
		if(pcoordinate->m_bItalic == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;			
		
		keyword = "|FONTNAME=";
		keyword += pcoordinate->m_csTTFontName;
		onelinestr += keyword;			

		//Ω·Œ≤
		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	int nPolygon = 0;
	newindex = 0;
	for(int i=0; i<m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = (COb_pcbpolygon*)m_arrayPcbPolygon.GetAt(i);
		if(ppolygon->m_nFlag != FLAG_NORMAL) continue;

		POINT point;
		point.x = i;
		point.y = newindex;
		arrayPolygon.Add(point);
		
		keyword = "|RECORD=Polygon";
		onelinestr += keyword;

		keyword = "|ID=";
		itoa(newindex, str, 10);
		keyword += str;
		onelinestr += keyword;
		newindex++;

		int netbak = ppolygon->m_nNet;
		for(int j=0; j<arrayNet.GetCount(); j++)
		{
			POINT point;
			point = arrayNet.GetAt(j);
			if(point.x == ppolygon->m_nNet) 
			{
				ppolygon->m_nNet = point.y;
				break;
			}
		}

		keyword = "|NET=";
		itoa(ppolygon->m_nNet, str, 10);
		keyword += str;
		onelinestr += keyword;
        
		ppolygon->m_nNet = netbak;	

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[ppolygon->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(ppolygon->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(ppolygon->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|USERROUTED=";
		if(ppolygon->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|UNIONINDEX=";
		itoa(ppolygon->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|PRIMITIVELOCK=";
		if(ppolygon->m_bPrimitiveLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONTYPE=";
		if(ppolygon->m_nPolygonType == COPPER_POLYGON) keyword += "Polygon";
		else if(ppolygon->m_nPolygonType == SPLITPLANE) keyword += "Split Plane";
		onelinestr += keyword;

		keyword = "|POUROVER=";
		if(ppolygon->m_bPourOver == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|REMOVEDEAD=";
		if(ppolygon->m_bRemoveDead == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|GRIDSIZE=";
		mil_to_str(ppolygon->m_fGridSize, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TRACKWIDTH=";
		mil_to_str(ppolygon->m_fTrackWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|HATCHSTYLE=";
		if(ppolygon->m_nHatchStyle == PCB_HATCH_SOLID) keyword += "Solid";
		else if(ppolygon->m_nHatchStyle == PCB_HATCH_90) keyword += "90Degree";
		else if(ppolygon->m_nHatchStyle == PCB_HATCH_45) keyword += "45Degree";
		else if(ppolygon->m_nHatchStyle == PCB_HATCH_H) keyword += "Horizontal";
		else if(ppolygon->m_nHatchStyle == PCB_HATCH_V) keyword += "Vertical";
		else if(ppolygon->m_nHatchStyle == PCB_HATCH_NONE) keyword += "None";
		onelinestr += keyword;

		keyword = "|USEOCTAGONS=";
		if(ppolygon->m_bUseOctagons == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|MINPRIMLENGTH=";
		mil_to_str(ppolygon->m_fMinPrimLength, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
	
		for(int i=0; i<ppolygon->m_cVertex.GetCount(); i++)
		{
			Struct_RegionVertex vertex;
			vertex = ppolygon->m_cVertex.GetAt(i);
			char numstr[100];
			itoa(i, numstr, 10);  strcat(numstr, "=");

			keyword = "|KIND";	keyword += numstr;
			if(vertex.radius != 0) keyword += "1";
			else keyword += "0";
			onelinestr += keyword;

			keyword = "|VX";	keyword += numstr;
			mil_to_str(vertex.fx, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|VY";	keyword += numstr;
			mil_to_str(vertex.fy, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|CX";	keyword += numstr;
			mil_to_str(vertex.cx, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|CY";	keyword += numstr;
			mil_to_str(vertex.cy, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|SA";	keyword += numstr;	keyword += " ";
			sprintf(str, "%.14E", vertex.sangle);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|EA";	keyword += numstr;	keyword += " ";
			sprintf(str, "%.14E", vertex.eangle);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|R";	keyword += numstr;
			mil_to_str(vertex.radius, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
		}

		keyword = "|SHELVED=";
		if(ppolygon->m_bShelved == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		onelinestr += "|RESTORELAYER=TOP|RESTORENET=";

		keyword = "|REMOVEISLANDSBYAREA=";
		if(ppolygon->m_bRemoveIslandsByArea == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|REMOVENECKS=";
		if(ppolygon->m_bRemoveNecks == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
			
		keyword = "|AREATHRESHOLD=";
		mil_to_str(ppolygon->m_fAreaThreshold*100000000, str, WITHOUT_MIL, 6);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|ARCRESOLUTION=";
		mil_to_str(ppolygon->m_fArcResolution, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|NECKWIDTHTHRESHOLD=";
		mil_to_str(ppolygon->m_fNeckWidthThreshold, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|POUROVERSTYLE=";
		itoa(ppolygon->m_nPourOverStyle, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|NAME=";
		CString cstring = ppolygon->m_csName;
		for(int i=0; i<ppolygon->m_csName.GetLength(); i++)
		{
			unsigned char c = cstring.GetAt(0);
			itoa((unsigned int)c, str, 10);
			keyword += str;
			if(i !=	ppolygon->m_csName.GetLength()-1) keyword += ",";
			cstring = cstring.Right(cstring.GetLength() -1);
		}
		onelinestr += keyword;
		
		keyword = "|POURINDEX=";
		if(ppolygon->m_nPolygonType == COPPER_POLYGON)
		{
			itoa(nPolygon + 1000, str, 10);
		}
		else strcpy(str, "-1");
		keyword += str;
		onelinestr += keyword;

		keyword = "|IGNOREVIOLATIONS";
		if(ppolygon->m_bIgnoreViolations == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}
    
	for(int i=0; i<m_arrayPcbConnection.GetCount(); i++)
	{
		COb_pcbconnection* pconnection = (COb_pcbconnection*)m_arrayPcbConnection.GetAt(i);

		keyword = "|RECORD=Connection";
		onelinestr += keyword;

		int netbak = pconnection->m_nNet;
		for(int j=0; j<arrayNet.GetCount(); j++)
		{
			POINT point;
			point = arrayNet.GetAt(j);
			if(point.x == pconnection->m_nNet) 
			{
				pconnection->m_nNet = point.y;
				break;
			}
		}

		keyword = "|NET=";
		itoa(pconnection->m_nNet, str, 10);
		keyword += str;
		onelinestr += keyword;

		pconnection->m_nNet = netbak;	

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE|LAYER=MULTILAYER|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0";
		onelinestr += keyword;
		
		keyword = "|X1=";
		mil_to_str(pconnection->m_fX1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y1=";
		mil_to_str(pconnection->m_fY1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;		
		
		keyword = "|X2=";
		mil_to_str(pconnection->m_fX2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y2=";
		mil_to_str(pconnection->m_fY2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;			
		
		keyword = "|LAYER1=";
		keyword += csEDA_PCB_LayerLabel[pconnection->m_nLayer1];
		onelinestr += keyword;
		
		keyword = "|LAYER2=";
		keyword += csEDA_PCB_LayerLabel[pconnection->m_nLayer2];
		onelinestr += keyword;

		//Ω·Œ≤
		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=0|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=ShortCircuit|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=ShortCircuit|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=HKPCITJB|DEFINEDBYLOGICALDOCUMENT=FALSE|ALLOWED=FALSE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=0|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=ShortCircuit|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=ShortCircuit|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|ALLOWED=FALSE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=1|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=UnRoutedNet|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=UnRoutedNet|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=KTCTOSHV|DEFINEDBYLOGICALDOCUMENT=FALSE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=1|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=UnRoutedNet|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=UnRoutedNet|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=2|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=Clearance|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=Clearance|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=PTEAWJYR|DEFINEDBYLOGICALDOCUMENT=FALSE|GAP=10mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=2|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=Clearance|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Clearance|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|GAP=10mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=3|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=PlaneConnect|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=PlaneConnect|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=VUCJOKLA|DEFINEDBYLOGICALDOCUMENT=FALSE|PLANECONNECTSTYLE=Relief|RELIEFEXPANSION=20mil|RELIEFENTRIES=4|RELIEFCONDUCTORWIDTH=10mil|RELIEFAIRGAP=10mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=3|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=PlaneConnect|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=PlaneConnect|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|PLANECONNECTSTYLE=Relief|RELIEFEXPANSION=20mil|RELIEFENTRIES=4|RELIEFCONDUCTORWIDTH=10mil|RELIEFAIRGAP=10mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=4|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=PlaneClearance|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=PlaneClearance|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=IHSYRHBV|DEFINEDBYLOGICALDOCUMENT=FALSE|CLEARANCE=20mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=4|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=PlaneClearance|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=PlaneClearance|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|CLEARANCE=20mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=5|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=PolygonConnect|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=PolygonConnect|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=UFUGCYCM|DEFINEDBYLOGICALDOCUMENT=FALSE|CONNECTSTYLE=Relief|RELIEFCONDUCTORWIDTH=10mil|RELIEFENTRIES=4|POLYGONRELIEFANGLE=90 Angle";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=5|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=PolygonConnect|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=PolygonConnect|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|CONNECTSTYLE=Relief|RELIEFCONDUCTORWIDTH=10mil|RELIEFENTRIES=4|POLYGONRELIEFANGLE=90 Angle";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=6|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=Width|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=Width|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=YNESDDCY|DEFINEDBYLOGICALDOCUMENT=FALSE|MAXLIMIT=10mil|MINLIMIT=10mil|PREFEREDWIDTH=10mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=6|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=Width|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Width|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|MAXLIMIT=10mil|MINLIMIT=10mil|PREFEREDWIDTH=10mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=7|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=RoutingPriority|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=RoutingPriority|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=SWPYJATW|DEFINEDBYLOGICALDOCUMENT=FALSE|ROUTINGPRIORITY=0";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=7|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=RoutingPriority|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=RoutingPriority|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|PRIORITY=0";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=8|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=RoutingLayers|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=RoutingLayers|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=GXULASHX|DEFINEDBYLOGICALDOCUMENT=FALSE|TOP LAYER_V5=TRUE|MID LAYER 1_V5=TRUE|MID LAYER 2_V5=TRUE|MID LAYER 3_V5=TRUE|MID LAYER 4_V5=TRUE|MID LAYER 5_V5=TRUE|MID LAYER 6_V5=TRUE|MID LAYER 7_V5=TRUE|MID LAYER 8_V5=TRUE|MID LAYER 9_V5=TRUE|MID LAYER 10_V5=TRUE|MID LAYER 11_V5=TRUE|MID LAYER 12_V5=TRUE|MID LAYER 13_V5=TRUE|MID LAYER 14_V5=TRUE|MID LAYER 15_V5=TRUE|MID LAYER 16_V5=TRUE|MID LAYER 17_V5=TRUE|MID LAYER 18_V5=TRUE|MID LAYER 19_V5=TRUE|MID LAYER 20_V5=TRUE|MID LAYER 21_V5=TRUE|MID LAYER 22_V5=TRUE|MID LAYER 23_V5=TRUE|MID LAYER 24_V5=TRUE|MID LAYER 25_V5=TRUE|MID LAYER 26_V5=TRUE|MID LAYER 27_V5=TRUE|MID LAYER 28_V5=TRUE|MID LAYER 29_V5=TRUE|MID LAYER 30_V5=TRUE|BOTTOM LAYER_V5=TRUE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=8|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=RoutingLayers|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=RoutingLayers|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|TOP LAYER=Automatic|MID LAYER 1=Automatic|MID LAYER 2=Automatic|MID LAYER 3=Automatic|MID LAYER 4=Automatic|MID LAYER 5=Automatic|MID LAYER 6=Automatic|MID LAYER 7=Automatic|MID LAYER 8=Automatic|MID LAYER 9=Automatic|MID LAYER 10=Automatic|MID LAYER 11=Automatic|MID LAYER 12=Automatic|MID LAYER 13=Automatic|MID LAYER 14=Automatic|MID LAYER 15=Automatic|MID LAYER 16=Automatic|MID LAYER 17=Automatic|MID LAYER 18=Automatic|MID LAYER 19=Automatic|MID LAYER 20=Automatic|MID LAYER 21=Automatic|MID LAYER 22=Automatic|MID LAYER 23=Automatic|MID LAYER 24=Automatic|MID LAYER 25=Automatic|MID LAYER 26=Automatic|MID LAYER 27=Automatic|MID LAYER 28=Automatic|MID LAYER 29=Automatic|MID LAYER 30=Automatic|BOTTOM LAYER=Automatic";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=9|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=RoutingCorners|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=RoutingCorners|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=SJVPAJRS|DEFINEDBYLOGICALDOCUMENT=FALSE|CORNERSTYLE=45-Degree|MINSETBACK=100mil|MAXSETBACK=100mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=9|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=RoutingCorners|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=RoutingCorners|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|CORNERSTYLE=45-Degree|MINSETBACK=100mil|MAXSETBACK=100mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=10|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=RoutingVias|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=RoutingVias|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=CHQVHKRT|DEFINEDBYLOGICALDOCUMENT=FALSE|HOLEWIDTH=28mil|WIDTH=50mil|VIASTYLE=Through Hole|MINHOLEWIDTH=28mil|MINWIDTH=50mil|MAXHOLEWIDTH=28mil|MAXWIDTH=50mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=10|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=RoutingVias|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=RoutingVias|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|HOLEWIDTH=28mil|WIDTH=50mil|VIASTYLE=Through Hole|MINHOLEWIDTH=28mil|MINWIDTH=50mil|MAXHOLEWIDTH=28mil|MAXWIDTH=50mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=11|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=PasteMaskExpansion|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=PasteMaskExpansion|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=IIFQMSAF|DEFINEDBYLOGICALDOCUMENT=FALSE|EXPANSION=0mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=11|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=PasteMaskExpansion|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=PasteMaskExpansion|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|EXPANSION=0mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=12|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=SolderMaskExpansion|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=SolderMaskExpansion|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=CEPWRLMW|DEFINEDBYLOGICALDOCUMENT=FALSE|EXPANSION=4mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=12|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=SolderMaskExpansion|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=SolderMaskExpansion|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|EXPANSION=4mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=13|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=RoutingTopology|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=RoutingTopology|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=GUVVXYCH|DEFINEDBYLOGICALDOCUMENT=FALSE|TOPOLOGY=Shortest";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=13|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=RoutingTopology|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=RoutingTopology|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|TOPOLOGY=Shortest";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=14|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=TestPointUsage|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=TestPointUsage|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=IHPVLTXS|DEFINEDBYLOGICALDOCUMENT=FALSE|VALID=0|ALLOWMULTIPLE=FALSE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=14|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=TestPointUsage|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=TestPointUsage|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|VALID=0|ALLOWMULTIPLE=FALSE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=15|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=Testpoint|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=Testpoint|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=UWOHUQOL|DEFINEDBYLOGICALDOCUMENT=FALSE|SIDE=15|TESTPOINTUNDERCOMPONENT=TRUE|MINSIZE=40mil|MAXSIZE=100mil|PREFEREDSIZE=60mil|MINHOLESIZE=0mil|MAXHOLESIZE=40mil|PREFEREDHOLESIZE=32mil|TESTPOINTGRID=1mil|STYLE=1023|ORDER=0-112345678";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=15|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=Testpoint|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Testpoint|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|SIDE=15|TESTPOINTUNDERCOMPONENT=TRUE|MINSIZE=40mil|MAXSIZE=100mil|PREFEREDSIZE=60mil|MINHOLESIZE=0mil|MAXHOLESIZE=40mil|PREFEREDHOLESIZE=32mil|TESTPOINTGRID=1mil|STYLE=1023|ORDER=0-112345678";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=16|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=ComponentClearance|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=ComponentClearance|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=YDVHXXUW|DEFINEDBYLOGICALDOCUMENT=FALSE|GAP=10mil|COLLISIONCHECKMODE=3|VERTICALGAP=10mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=16|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=ComponentClearance|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=ComponentClearance|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|GAP=10mil|COLLISIONCHECKMODE=3";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=17|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=Height|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=Height|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=YAAEQLQW|DEFINEDBYLOGICALDOCUMENT=FALSE|MINHEIGHT=0mil|MAXHEIGHT=1000mil|PREFHEIGHT=500mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=17|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=Height|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Height|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=18|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=LayerPairs|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=LayerPairs|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=CDYWLAMA|DEFINEDBYLOGICALDOCUMENT=FALSE|ENFORCE=TRUE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=18|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=LayerPairs|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=LayerPairs|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|ENFORCE=TRUE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=19|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=HoleSize|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=HoleSize|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=SHUWQOCD|DEFINEDBYLOGICALDOCUMENT=FALSE|ABSOLUTEVALUES=TRUE|MAXLIMIT=100mil|MINLIMIT=1mil|MAXPERCENT=80.000|MINPERCENT=20.000";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=19|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=HoleSize|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=HoleSize|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|ABSOLUTEVALUES=TRUE|MAXLIMIT=100mil|MINLIMIT=1mil|MAXPERCENT=80.000|MINPERCENT=20.000";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=20|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=Fanout_Default|ENABLED=TRUE|PRIORITY=5|COMMENT=Fanout_Default (Default Rule)|UNIQUEID=FKGNUSDQ|DEFINEDBYLOGICALDOCUMENT=FALSE|BGADIR=Out|BGAVIAMODE=Centered|FANOUTSTYLE=Auto|FANOUTDIRECTION=Alternating|VIAGRID=1mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=20|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Fanout_Default|ENABLED=TRUE|COMMENTLENGTH=30|COMMENT=Fanout_Default (Default Rule)";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=21|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=(CompPinCount < 5)|SCOPE2EXPRESSION=All|NAME=Fanout_Small|ENABLED=TRUE|PRIORITY=4|COMMENT=Fanout_Small (Default Rule)|UNIQUEID=CGXYKESG|DEFINEDBYLOGICALDOCUMENT=FALSE|BGADIR=Out|BGAVIAMODE=Centered|FANOUTSTYLE=Auto|FANOUTDIRECTION=OutThenIn|VIAGRID=1mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=21|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Fanout_Small|ENABLED=FALSE|COMMENTLENGTH=84|COMMENT=Protel DXP Scope 1 Expression Was: (CompPinCount < 5)   Fanout_Small (Default Rule)";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=22|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=IsSOIC|SCOPE2EXPRESSION=All|NAME=Fanout_SOIC|ENABLED=TRUE|PRIORITY=3|COMMENT=Fanout_SOIC (Default Rule)|UNIQUEID=YVRABBWH|DEFINEDBYLOGICALDOCUMENT=FALSE|BGADIR=Out|BGAVIAMODE=Centered|FANOUTSTYLE=Auto|FANOUTDIRECTION=Alternating|VIAGRID=1mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=22|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Fanout_SOIC|ENABLED=FALSE|COMMENTLENGTH=71|COMMENT=Protel DXP Scope 1 Expression Was: IsSOIC   Fanout_SOIC (Default Rule)";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=23|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=IsLCC|SCOPE2EXPRESSION=All|NAME=Fanout_LCC|ENABLED=TRUE|PRIORITY=2|COMMENT=Fanout_LCC (Default Rule)|UNIQUEID=KBOWPUJQ|DEFINEDBYLOGICALDOCUMENT=FALSE|BGADIR=Out|BGAVIAMODE=Centered|FANOUTSTYLE=Auto|FANOUTDIRECTION=Alternating|VIAGRID=1mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=23|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Fanout_LCC|ENABLED=FALSE|COMMENTLENGTH=69|COMMENT=Protel DXP Scope 1 Expression Was: IsLCC   Fanout_LCC (Default Rule)";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=24|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=IsBGA|SCOPE2EXPRESSION=All|NAME=Fanout_BGA|ENABLED=TRUE|PRIORITY=1|COMMENT=Fanout_BGA (Default Rule)|UNIQUEID=BXKFDOAH|DEFINEDBYLOGICALDOCUMENT=FALSE|BGADIR=Out|BGAVIAMODE=Centered|FANOUTSTYLE=Auto|FANOUTDIRECTION=Alternating|VIAGRID=1mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=24|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Fanout_BGA|ENABLED=FALSE|COMMENTLENGTH=69|COMMENT=Protel DXP Scope 1 Expression Was: IsBGA   Fanout_BGA (Default Rule)";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=25|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=DiffPairsRouting|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=DiffPairsRouting|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=KVRBRADC|DEFINEDBYLOGICALDOCUMENT=FALSE|MAXLIMIT=10mil|MINLIMIT=10mil|MOSTFREQGAP=10mil|MAXUNCOUPLEDLENGTH=500mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=25|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=DiffPairsRouting|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=DiffPairsRouting|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	

	for(int i=0; i<m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = (COb_pcbdimension*)m_arrayPcbDimension.GetAt(i);
		if(pdimension->m_nFlag != FLAG_NORMAL) continue;
		if(pdimension->m_nDimensionKind != 1) continue;

		keyword = "|RECORD=Embedded";
		onelinestr += keyword;

		keyword = "|INDEXFORSAVE=0";

		onelinestr += keyword;

		onelinestr += "|SELECTION=FALSE|LAYER=MULTILAYER|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|NAME=";
		onelinestr += "|SELECTION=FALSE";

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[pdimension->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(pdimension->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(pdimension->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|USERROUTED=";
		if(pdimension->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|UNIONINDEX=";
		itoa(pdimension->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|PRIMITIVELOCK=";
		if(pdimension->m_bPrimitiveLock == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|DIMENSIONLAYER=";
		keyword += csEDA_PCB_LayerLabel[pdimension->m_nLayer];
		onelinestr += keyword;

		keyword = "|DIMENSIONLOCKED=";
		if(pdimension->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		onelinestr += "|OBJECTID=13";
		
		keyword = "|DIMENSIONKIND=";
		itoa(pdimension->m_nDimensionKind, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|DRCERROR=FALSE";

		keyword = "|VINDEXFORSAVE=0";

		onelinestr += keyword;

		keyword = "|LX=";
		mil_to_str(pdimension->m_fLx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|LY=";
		mil_to_str(pdimension->m_fLy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|HX=";
		mil_to_str(pdimension->m_fHx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|HY=";
		mil_to_str(pdimension->m_fHy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|X1=";
		mil_to_str(pdimension->m_fXs, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|Y1=";
		mil_to_str(pdimension->m_fYs, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|X2=";
		mil_to_str(pdimension->m_fXe, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|Y2=";
		mil_to_str(pdimension->m_fYe, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTX=";
		mil_to_str(pdimension->m_fTextSx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;		
		keyword = "|TEXTY=";
		mil_to_str(pdimension->m_fTextSy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;					
		keyword = "|HEIGHT=";
		mil_to_str(pdimension->m_fHeight, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;					
		keyword = "|LINEWIDTH=";
		mil_to_str(pdimension->m_fLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTHEIGHT=";
		mil_to_str(pdimension->m_fTextHeight, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|TEXTWIDTH=";
		mil_to_str(pdimension->m_fTextLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|FONT=";
		keyword += csPCB_StrokeFontName[pdimension->m_nStrokeFont];
		onelinestr += keyword;
		
		keyword = "|STYLE=";
		if(pdimension->m_nUnitStyle == UNIT_STYLE_NORMAL) keyword += "Normal";
		else if(pdimension->m_nUnitStyle == UNIT_STYLE_BRACKETS) keyword += "Brackets";
		else keyword += "None";
		onelinestr += keyword;

		keyword = "|TEXTLINEWIDTH=";
		mil_to_str(pdimension->m_fTextLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|TEXTPOSITION=Auto";

		keyword = "|TEXTGAP=";
		mil_to_str(pdimension->m_fTextGap, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTFORMAT=";
		if(pdimension->m_nTextFormat == 2) keyword += "10mil";
		else if(pdimension->m_nTextFormat == 3) keyword += "10 (mil)";
		else keyword += "10";
		onelinestr += keyword;

		keyword = "|TEXTDIMENSIONUNIT=";
		if(pdimension->m_nTextDimensionUnit == UNIT_MIL) keyword += "Mils";
		else if(pdimension->m_nTextDimensionUnit == UNIT_MM) keyword += "Millimeters";
		else if(pdimension->m_nTextDimensionUnit == UNIT_INCH) keyword += "Inches";
		else if(pdimension->m_nTextDimensionUnit == UNIT_CM) keyword += "Centimeters";
		else keyword += "AutomaticUnit";
		onelinestr += keyword;
		
		keyword = "|TEXTPRECISION=";
		itoa(pdimension->m_nTextPrecision, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTPREFIX=";
		keyword += pdimension->m_csPrefix;
		onelinestr += keyword;
		
		keyword = "|TEXTSUFFIX=";
		keyword += pdimension->m_csSuffix;
		onelinestr += keyword;		
		
		keyword = "|ARROWSIZE=";
		mil_to_str(pdimension->m_fArrowSize, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;		

		keyword = "|ARROWLINEWIDTH=";
		mil_to_str(pdimension->m_fArrowLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|ARROWLENGTH=";
		mil_to_str(pdimension->m_fArrowLength, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|ARROWPOSITION=";
		if(pdimension->m_nArrowPosition == 0) keyword += "Inside";
		else keyword += "Outside";
		onelinestr += keyword;	

		keyword = "|EXTENSIONOFFSET=";
		mil_to_str(pdimension->m_fExtensionOffset, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	
		
		keyword = "|EXTENSIONLINEWIDTH=";
		mil_to_str(pdimension->m_fExtensionLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|EXTENSIONPICKGAP=";
		mil_to_str(pdimension->m_fExtensionPickGap, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|REFERENCES_COUNT=";
		itoa(pdimension->m_cRefPoint.GetCount(), str, 10);
		keyword += str;
		onelinestr += keyword;

		for(int j=0; j<pdimension->m_cRefPoint.GetCount(); j++)
		{
			Fpoint fpoint = pdimension->m_cRefPoint.GetAt(j);
			char numstr[100];
			itoa(j, numstr, 10);
			keyword = "|REFERENCE"; keyword += numstr; keyword += "PRIM=-1";
			keyword += "|REFERENCE"; keyword += numstr; keyword += "OBJECTID=-1";
			keyword += "|REFERENCE"; keyword += numstr; keyword += "OBJECTSTRING=NoObject";
			keyword += "|REFERENCE"; keyword += numstr; keyword += "POINTX=";
			mil_to_str(fpoint.fx, str, WITH_MIL, 4);
			keyword += str;
			keyword += "|REFERENCE"; keyword += numstr; keyword += "POINTY=";
			mil_to_str(fpoint.fy, str, WITH_MIL, 4);
			keyword += str;
			keyword += "|REFERENCE"; keyword += numstr; keyword += "ANCHOR=-1";
			onelinestr += keyword;
		}

		keyword = "|TEXT1X=";
		mil_to_str(pdimension->m_fTextSx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	
		
		keyword = "|TEXT1Y=";
		mil_to_str(pdimension->m_fTextSy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	
		
		keyword = "|TEXT1ANGLE=";
		sprintf(str, "%.14E", pdimension->m_fTextRotation);
		keyword += str;
		onelinestr += keyword;	

		onelinestr += "|TEXT1MIRROR=FALSE";

		keyword = "|USETTFONTS=";
		if(pdimension->m_bUseTTFont == FALSE) keyword += "FALSE";
		else keyword += "TRUE";
		onelinestr += keyword;	
		
		keyword = "|BOLD=";
		if(pdimension->m_bBold == FALSE) keyword += "FALSE";
		else keyword += "TRUE";
		onelinestr += keyword;	

		keyword = "|ITALIC=";
		if(pdimension->m_bItalic == FALSE) keyword += "FALSE";
		else keyword += "TRUE";
		onelinestr += keyword;	

		keyword = "|FONTNAME=";
		keyword += pdimension->m_csTTFontName;
		onelinestr += keyword;	
		
		keyword = "|ANGLE=";
		sprintf(str, "%.14E", pdimension->m_fRotation);
		keyword += str;
		onelinestr += keyword;	

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	for(int i=0; i<m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = (COb_pcbarc*)m_arrayPcbArc.GetAt(i);
		if(parc->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Arc";
		onelinestr += keyword;

		if(parc->m_nComponent != -1)
		{
			int compbak = parc->m_nComponent;
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == parc->m_nComponent) 
				{
					parc->m_nComponent = point.y;
					break;
				}
			}

			keyword = "|COMPONENT=";
			itoa(parc->m_nComponent, str, 10);
			keyword += str;
			onelinestr += keyword;

			parc->m_nComponent = compbak;	
		}
		
		if(parc->m_nPolygon != -1)
		{
			int polybak = parc->m_nPolygon;
			for(int j=0; j<arrayPolygon.GetCount(); j++)
			{
				POINT point;
				point = arrayPolygon.GetAt(j);
				if(point.x == parc->m_nPolygon) 
				{
					parc->m_nPolygon = point.y;
					break;
				}
			}

			keyword = "|POLYGON=";
			itoa(parc->m_nPolygon, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			parc->m_nPolygon = polybak;	
		}
		
		if(parc->m_nNet != -1)
		{
			int netbak = parc->m_nNet;
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == parc->m_nNet) 
				{
					parc->m_nNet = point.y;
					break;
				}
			}

			keyword = "|NET=";
			itoa(parc->m_nNet, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			parc->m_nNet = netbak;	
		}

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[parc->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(parc->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(parc->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|TEARDROP=";
		if(parc->m_bTearDrop == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;			
		
		keyword = "|USERROUTED=FALSE";
		onelinestr += keyword;	

		keyword = "|KEEPOUT=";
		if(parc->m_bKeepout == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;	

		keyword = "|UNIONINDEX=";
		itoa(parc->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LOCATION.X=";
		mil_to_str(parc->m_fCx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|LOCATION.Y=";
		mil_to_str(parc->m_fCy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|RADIUS=";
		mil_to_str(parc->m_fRadius, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|STARTANGLE= ";
		sprintf(str, "%.14E", parc->m_fSangle);
		keyword += str;
		onelinestr += keyword;

		keyword = "|ENDANGLE= ";
		sprintf(str, "%.14E", parc->m_fEangle);
		keyword += str;
		onelinestr += keyword;

		keyword = "|WIDTH=";
		mil_to_str(parc->m_fWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SUBPOLYINDEX=";
		itoa(parc->m_nSubPolyIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	for(int i=0; i<m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = (COb_pcbpad*)m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Pad";
		onelinestr += keyword;

		if(ppad->m_nComponent != -1)
		{
			int compbak = ppad->m_nComponent;
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == ppad->m_nComponent) 
				{
					ppad->m_nComponent = point.y;
					break;
				}
			}

			keyword = "|COMPONENT=";
			itoa(ppad->m_nComponent, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			ppad->m_nComponent = compbak;	
		}
		
		if(ppad->m_nNet != -1)
		{
			int netbak = ppad->m_nNet;
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == ppad->m_nNet) 
				{
					ppad->m_nNet = point.y;
					break;
				}
			}

			keyword = "|NET=";
			itoa(ppad->m_nNet, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			ppad->m_nNet = netbak;
		}

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[ppad->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(ppad->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(ppad->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|USERROUTED=";
		if(ppad->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		if(ppad->m_bTenting == TRUE) onelinestr += "|TENTING=TRUE";

		keyword = "|UNIONINDEX=";
		itoa(ppad->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|NAME=";
		keyword += ppad->m_csName;
		onelinestr += keyword;

		keyword = "|X=";
		mil_to_str(ppad->m_fCx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y=";
		mil_to_str(ppad->m_fCy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
			
		if( (ppad->m_fSizeX[TOPLAYER] == ppad->m_fSizeX[BOTTOMLAYER])&&(ppad->m_fSizeX[TOPLAYER] == ppad->m_fSizeX[MIDLAYER1])&&\
			(ppad->m_fSizeY[TOPLAYER] == ppad->m_fSizeY[BOTTOMLAYER])&&(ppad->m_fSizeY[TOPLAYER] == ppad->m_fSizeY[MIDLAYER1]))
		{
			keyword = "|XSIZE=";
			mil_to_str(ppad->m_fSizeX[0], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|YSIZE=";
			mil_to_str(ppad->m_fSizeY[0], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
		}
		else
		{
			keyword = "|TOPXSIZE=";
			mil_to_str(ppad->m_fSizeX[TOPLAYER], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			keyword = "|MIDXSIZE=";
			mil_to_str(ppad->m_fSizeX[MIDLAYER1], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			keyword = "|BOTXSIZE=";
			mil_to_str(ppad->m_fSizeX[BOTTOMLAYER], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|TOPYSIZE=";
			mil_to_str(ppad->m_fSizeY[TOPLAYER], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			keyword = "|MIDYSIZE=";
			mil_to_str(ppad->m_fSizeY[MIDLAYER1], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			keyword = "|BOTYSIZE=";
			mil_to_str(ppad->m_fSizeY[BOTTOMLAYER], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
		}

		if((ppad->m_nShape[TOPLAYER] == ppad->m_nShape[BOTTOMLAYER])&&(ppad->m_nShape[TOPLAYER] == ppad->m_nShape[MIDLAYER1]))
		{
			keyword = "|SHAPE=";
			keyword += csPCB_PadShape[ppad->m_nShape[TOPLAYER]];
			onelinestr += keyword;
		}
		else
		{
			keyword = "|TOPSHAPE=";
			keyword += csPCB_PadShape[ppad->m_nShape[TOPLAYER]];
			onelinestr += keyword;
			keyword = "|MIDSHAPE=";
			keyword += csPCB_PadShape[ppad->m_nShape[MIDLAYER1]];
			onelinestr += keyword;
			keyword = "|BOTSHAPE=";
			keyword += csPCB_PadShape[ppad->m_nShape[BOTTOMLAYER]];
			onelinestr += keyword;
		}

		keyword = "|HOLESIZE=";
		mil_to_str(ppad->m_fHoleDia, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|ROTATION=";
		sprintf(str, "%.14E", ppad->m_fRotation);
		keyword += str;
		onelinestr += keyword;

		keyword = "|PLATED=";
		if(ppad->m_bPlated == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|DAISYCHAIN=";
		keyword += csPCB_PadElecTpye[ppad->m_nDaisychain];
		onelinestr += keyword;

		keyword = "|CCSV=";
		itoa(ppad->m_nCCSV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPLV=";
		itoa(ppad->m_nCPLV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CCWV=";
		itoa(ppad->m_nCCWV, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|CENV=";
		itoa(ppad->m_nCENV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CAGV=";
		itoa(ppad->m_nCAGV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPEV=";
		itoa(ppad->m_nCPEV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CSEV=";
		itoa(ppad->m_nCSEV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPCV=";
		itoa(ppad->m_nCPCV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPRV=";
		itoa(ppad->m_nCPRV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CCW=";
		mil_to_str(ppad->m_fCCW, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|CEN=";
		itoa(ppad->m_nCEN, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CAG=";
		mil_to_str(ppad->m_fCAG, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPE=";
		mil_to_str(ppad->m_fCPE, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CSE=";
		mil_to_str(ppad->m_fCSE, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPC=";
		mil_to_str(ppad->m_fCPC, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPR=";
		mil_to_str(ppad->m_fCPR, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|PADMODE=";
		itoa(ppad->m_nPadMode, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|SWAPID_PAD=|SWAPID_GATE=|&|0|SWAPPEDPADNAME=|GATEID=0";
		
		if(ppad->m_bTentingTop == TRUE) onelinestr += "|TENTINGTOP=TRUE";
		if(ppad->m_bTentingBottom == TRUE) onelinestr += "|TENTINGBOTTOM=TRUE";			
			
		onelinestr += "|OVERRIDEWITHV6_6SHAPES=FALSE|DRILLTYPE=0";

		keyword = "|HOLETYPE=";
		itoa(ppad->m_nHoleType, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|HOLEWIDTH=";
		mil_to_str(ppad->m_fHoleWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|HOLEROTATION=";
		sprintf(str, "%.14E", ppad->m_fHoleRotation);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|PADXOFFSET0=0mil|PADYOFFSET0=0mil|PADXOFFSET1=0mil|PADYOFFSET1=0mil|PADXOFFSET2=0mil|PADYOFFSET2=0mil|PADXOFFSET3=0mil|PADYOFFSET3=0mil|PADXOFFSET4=0mil";
		onelinestr += "|PADYOFFSET4=0mil|PADXOFFSET5=0mil|PADYOFFSET5=0mil|PADXOFFSET6=0mil|PADYOFFSET6=0mil|PADXOFFSET7=0mil|PADYOFFSET7=0mil|PADXOFFSET8=0mil|PADYOFFSET8=0mil";
		onelinestr += "|PADXOFFSET9=0mil|PADYOFFSET9=0mil|PADXOFFSET10=0mil|PADYOFFSET10=0mil|PADXOFFSET11=0mil|PADYOFFSET11=0mil|PADXOFFSET12=0mil|PADYOFFSET12=0mil|PADXOFFSET13=0mil";
		onelinestr += "|PADYOFFSET13=0mil|PADXOFFSET14=0mil|PADYOFFSET14=0mil|PADXOFFSET15=0mil|PADYOFFSET15=0mil|PADXOFFSET16=0mil|PADYOFFSET16=0mil|PADXOFFSET17=0mil|PADYOFFSET17=0mil";
		onelinestr += "|PADXOFFSET18=0mil|PADYOFFSET18=0mil|PADXOFFSET19=0mil|PADYOFFSET19=0mil|PADXOFFSET20=0mil|PADYOFFSET20=0mil|PADXOFFSET21=0mil|PADYOFFSET21=0mil|PADXOFFSET22=0mil";
		onelinestr += "|PADYOFFSET22=0mil|PADXOFFSET23=0mil|PADYOFFSET23=0mil|PADXOFFSET24=0mil|PADYOFFSET24=0mil|PADXOFFSET25=0mil|PADYOFFSET25=0mil|PADXOFFSET26=0mil|PADYOFFSET26=0mil";
		onelinestr += "|PADXOFFSET27=0mil|PADYOFFSET27=0mil|PADXOFFSET28=0mil|PADYOFFSET28=0mil|PADXOFFSET29=0mil|PADYOFFSET29=0mil|PADXOFFSET30=0mil|PADYOFFSET30=0mil|PADXOFFSET31=0mil|PADYOFFSET31=0mil|PADJUMPERID=0";

		//Ω·Œ≤
		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	for(int i=0; i<m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = (COb_pcbvia*)m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Via";
		onelinestr += keyword;

		if(pvia->m_nComponent != -1)
		{
			int compbak = pvia->m_nComponent;
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == pvia->m_nComponent) 
				{
					pvia->m_nComponent = point.y;
					break;
				}
			}

			keyword = "|COMPONENT=";
			itoa(pvia->m_nComponent, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			pvia->m_nComponent = compbak;	
		}
		
		if(pvia->m_nNet != -1)
		{
			int netbak = pvia->m_nNet;
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == pvia->m_nNet) 
				{
					pvia->m_nNet = point.y;
					break;
				}
			}

			keyword = "|NET=";
			itoa(pvia->m_nNet, str, 10);
			keyword += str;
			onelinestr += keyword;

			pvia->m_nNet = netbak;
		}

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[MULTILAYER]; //[pvia->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(pvia->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(pvia->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|USERROUTED=";
		if(pvia->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		if(pvia->m_bTenting == TRUE) onelinestr += "|TENTING=TRUE";

		keyword = "|UNIONINDEX=";
		itoa(pvia->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|X=";
		mil_to_str(pvia->m_fCx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y=";
		mil_to_str(pvia->m_fCy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|DIAMETER=";
		mil_to_str(pvia->m_fDia, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|HOLESIZE=";
		mil_to_str(pvia->m_fHoleDia, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|STARTLAYER=";
		keyword += csEDA_PCB_LayerLabel[pvia->m_nStartLayer];
		onelinestr += keyword;

		keyword = "|ENDLAYER=";
		keyword += csEDA_PCB_LayerLabel[pvia->m_nEndLayer];
		onelinestr += keyword;

		keyword = "|CCSV=";
		itoa(pvia->m_nCCSV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPLV=";
		itoa(pvia->m_nCPLV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CCWV=";
		itoa(pvia->m_nCCWV, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|CENV=";
		itoa(pvia->m_nCENV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CAGV=";
		itoa(pvia->m_nCAGV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPEV=";
		itoa(pvia->m_nCAGV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CSEV=";
		itoa(pvia->m_nCSEV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPCV=";
		itoa(pvia->m_nCPCV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPRV=";
		itoa(pvia->m_nCPRV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CCW=";
		mil_to_str(pvia->m_fCCW, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|CEN=";
		itoa(pvia->m_nCEN, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CAG=";
		mil_to_str(pvia->m_fCAG, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CSE=";
		mil_to_str(pvia->m_fCSE, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPC=";
		mil_to_str(pvia->m_fCPC, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPR=";
		mil_to_str(pvia->m_fCPR, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		if(pvia->m_bTentingTop == TRUE) onelinestr += "|TENTINGTOP=TRUE";
		if(pvia->m_bTentingBottom == TRUE) onelinestr += "|TENTINGBOTTOM=TRUE";

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}


	for(int i=0; i<m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = (COb_pcbtrack*)m_arrayPcbTrack.GetAt(i);
		if(ptrack->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Track";
		onelinestr += keyword;

		if(ptrack->m_nComponent != -1)
		{
			int compbak = ptrack->m_nComponent;
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == ptrack->m_nComponent) 
				{
					ptrack->m_nComponent = point.y;
					break;
				}
			}

			keyword = "|COMPONENT=";
			itoa(ptrack->m_nComponent, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			ptrack->m_nComponent = compbak;	
		}
		
		if(ptrack->m_nPolygon != -1)
		{
			int polybak = ptrack->m_nPolygon;
			for(int j=0; j<arrayPolygon.GetCount(); j++)
			{
				POINT point;
				point = arrayPolygon.GetAt(j);
				if(point.x == ptrack->m_nPolygon) 
				{
					ptrack->m_nPolygon = point.y;
					break;
				}
			}

			keyword = "|POLYGON=";
			itoa(ptrack->m_nPolygon, str, 10);
			keyword += str;
			onelinestr += keyword;

			ptrack->m_nPolygon = polybak;	
		}
		
		if(ptrack->m_nNet != -1)
		{
			int netbak = ptrack->m_nNet;
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == ptrack->m_nNet) 
				{
					ptrack->m_nNet = point.y;
					break;
				}
			}

			keyword = "|NET=";
			itoa(ptrack->m_nNet, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			ptrack->m_nNet = netbak;
		}

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[ptrack->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(ptrack->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(ptrack->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|TEARDROP=";
		if(ptrack->m_bTearDrop == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;			
		
		keyword = "|USERROUTED=FALSE";
		onelinestr += keyword;	

		keyword = "|KEEPOUT=";
		if(ptrack->m_bKeepout == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;			

		keyword = "|UNIONINDEX=";
		itoa(ptrack->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|X1=";
		mil_to_str(ptrack->m_fX1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y1=";
		mil_to_str(ptrack->m_fY1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|X2=";
		mil_to_str(ptrack->m_fX2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y2=";
		mil_to_str(ptrack->m_fY2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;		
		
		keyword = "|WIDTH=";
		mil_to_str(ptrack->m_fWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SUBPOLYINDEX=";
		itoa(ptrack->m_nSubPolyIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	for(int i=0; i<m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = (COb_pcbtext*)m_arrayPcbText.GetAt(i);
		if(ptext->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Text";
		onelinestr += keyword;

		if(ptext->m_nComponent != -1)
		{
			int compbak = ptext->m_nComponent;
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == ptext->m_nComponent) 
				{
					ptext->m_nComponent = point.y;
					break;
				}
			}

			keyword = "|COMPONENT=";
			itoa(ptext->m_nComponent, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			ptext->m_nComponent = compbak;	
		}
		
		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[ptext->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(ptext->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(ptext->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|USERROUTED=";
		if(ptext->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|UNIONINDEX=";
		itoa(ptext->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|X1=";
		mil_to_str(ptext->m_fX1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y1=";
		mil_to_str(ptext->m_fY1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|X2=";
		mil_to_str(ptext->m_fX2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y2=";
		mil_to_str(ptext->m_fY2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|ROTATION= ";
		sprintf(str, "%.14E", ptext->m_fRotation);
		keyword += str;
		onelinestr += keyword;

		keyword = "|X=";
		mil_to_str(ptext->m_fSx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y=";
		mil_to_str(ptext->m_fSy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|HEIGHT=";
		mil_to_str(ptext->m_fHeight, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|FONT=";
		keyword += csPCB_StrokeFontName[ptext->m_nStrokeFont];
		onelinestr += keyword;
		
		keyword = "|ROTATION= ";
		sprintf(str, "%.14E", ptext->m_fRotation);
		keyword += str;
		onelinestr += keyword;

		keyword = "|MIRROR=";
		if(ptext->m_bMirror == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|TEXT=";
		keyword += ptext->m_csText;
		onelinestr += keyword;
		
		keyword = "|WIDTH=";
		mil_to_str(ptext->m_fWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|USETTFONTS=";
		if(ptext->m_bUseTTFont == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;		
		
		keyword = "|BOLD=";
		if(ptext->m_bBold == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;				
		
		keyword = "|ITALIC=";
		if(ptext->m_bItalic == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;				
		
		keyword = "|INVERTED=";
		if(ptext->m_bInverted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;	

		keyword = "|FONTNAME=";
		keyword += ptext->m_csTTFontName;
		onelinestr += keyword;
		
	
		keyword = "|INVERTEDTTTEXTBORDER=";
		mil_to_str(ptext->m_fInvertedBorder, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

	
		if(ptext->m_bDesignator == TRUE) onelinestr += "|DESIGNATOR=True";

		if(ptext->m_bComment == TRUE) onelinestr += "|COMMENT=True";
		
		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	for(int i=0; i<m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = (COb_pcbfill*)m_arrayPcbFill.GetAt(i);
        if(pfill->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Fill";
		onelinestr += keyword;

		if(pfill->m_nComponent != -1)
		{
			int compbak = pfill->m_nComponent;
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == pfill->m_nComponent) 
				{
					pfill->m_nComponent = point.y;
					break;
				}
			}

			keyword = "|COMPONENT=";
			itoa(pfill->m_nComponent, str, 10);
			keyword += str;
			onelinestr += keyword;

			pfill->m_nComponent = compbak;	
		}
		
		if(pfill->m_nNet != -1)
		{
			int netbak = pfill->m_nNet;
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == pfill->m_nNet) 
				{
					pfill->m_nNet = point.y;
					break;
				}
			}

			keyword = "|NET=";
			itoa(pfill->m_nNet, str, 10);
			keyword += str;
			onelinestr += keyword;

			pfill->m_nNet = netbak;
		}

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[pfill->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(pfill->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(pfill->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|USERROUTED=";
		if(pfill->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|UNIONINDEX=";
		itoa(pfill->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;
	
		keyword = "|X1=";
		mil_to_str(pfill->m_fX1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
			
		keyword = "|Y1=";
		mil_to_str(pfill->m_fY1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
			
		keyword = "|X2=";
		mil_to_str(pfill->m_fX2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
			
		keyword = "|Y2=";
		mil_to_str(pfill->m_fY2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|ROTATION= ";
		sprintf(str, "%.14E", pfill->m_fRotation);
		keyword += str;
		onelinestr += keyword;

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}


	for(int i=0; i<m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = (COb_pcbregion*)m_arrayPcbRegion.GetAt(i);
		if(pregion->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Region";
		onelinestr += keyword;

		if(pregion->m_nComponent != -1)
		{
			int compbak = pregion->m_nComponent;
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == pregion->m_nComponent) 
				{
					pregion->m_nComponent = point.y;
					break;
				}
			}

			keyword = "|COMPONENT=";
			itoa(pregion->m_nComponent, str, 10);
			keyword += str;
			onelinestr += keyword;

			pregion->m_nComponent = compbak;	
		}
		
		if(pregion->m_nPolygon != -1)
		{
			int polybak = pregion->m_nPolygon;
			for(int j=0; j<arrayPolygon.GetCount(); j++)
			{
				POINT point;
				point = arrayPolygon.GetAt(j);
				if(point.x == pregion->m_nPolygon) 
				{
					pregion->m_nPolygon = point.y;
					break;
				}
			}

			keyword = "|POLYGON=";
			itoa(pregion->m_nPolygon, str, 10);
			keyword += str;
			onelinestr += keyword;

			pregion->m_nPolygon = polybak;	
		}
		
		if(pregion->m_nNet != -1)
		{
			int netbak = pregion->m_nNet;
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == pregion->m_nNet) 
				{
					pregion->m_nNet = point.y;
					break;
				}
			}

			keyword = "|NET=";
			itoa(pregion->m_nNet, str, 10);
			keyword += str;
			onelinestr += keyword;

			pregion->m_nNet = netbak;
		}

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[pregion->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(pregion->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(pregion->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|USERROUTED=";
		if(pregion->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|UNIONINDEX=";
		itoa(pregion->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|NAME=";

		keyword = "|KIND=";
		if(pregion->m_bPolygonCutout == TRUE) keyword += "1";
		else keyword += "0";
		onelinestr += keyword;
		
		keyword = "|SUBPOLYINDEX=";
		itoa(pregion->m_nSubPolyIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|ARCRESOLUTION=0.5mil|ISSHAPEBASED=TRUE";

		keyword = "|MAINCONTOURVERTEXCOUNT=";
		itoa(pregion->m_cVertex.GetCount(), str, 10);
		keyword += str;
		onelinestr += keyword;
		
		for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
		{
			Struct_RegionVertex vertex;
			vertex = pregion->m_cVertex.GetAt(i);
			char numstr[100];
			itoa(i, numstr, 10);  strcat(numstr, "=");

			keyword = "|KIND";	keyword += numstr;
			if(vertex.radius != 0) keyword += "1";
			else keyword += "0";
			onelinestr += keyword;

			keyword = "|VX";	keyword += numstr;
			mil_to_str(vertex.fx, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|VY";	keyword += numstr;
			mil_to_str(vertex.fy, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|CX";	keyword += numstr;
			mil_to_str(vertex.cx, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|CY";	keyword += numstr;
			mil_to_str(vertex.cy, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|SA";	keyword += numstr;	keyword += " ";
			sprintf(str, "%.14E", vertex.sangle);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|EA";	keyword += numstr;	keyword += " ";
			sprintf(str, "%.14E", vertex.eangle);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|R";	keyword += numstr;
			mil_to_str(vertex.radius, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
		}
		
		//|HOLECOUNT=0
		keyword = "|HOLECOUNT=";
		itoa(pregion->m_arrayHoles.GetCount(), str, 10);
		keyword += str;
		onelinestr += keyword;

		for(int i=0; i<pregion->m_arrayHoles.GetCount(); i++)
		{
			CArray<Fpoint, Fpoint>* p_arrayHole = pregion->m_arrayHoles.GetAt(i);
			char numstr[100];
			itoa(i, numstr, 10);

			keyword = "|HOLE";		keyword += numstr;
			keyword += "VERTEXCOUNT=";
			itoa(p_arrayHole->GetCount(), str, 10);
			keyword += str;
			onelinestr += keyword;

			for(int j=0; j<p_arrayHole->GetCount(); j++)
			{
				Fpoint fp = p_arrayHole->GetAt(j);

				char sub_numstr[100];
				itoa(j, sub_numstr, 10);  strcat(sub_numstr, "=");				

				keyword = "|H";		keyword += numstr;
				keyword += "VX";	keyword += sub_numstr;
				sprintf(str, "%.6f", fp.fx*10000);
				keyword += str;
				onelinestr += keyword;
				
				keyword = "|H";		keyword += numstr;
				keyword += "VY";	keyword += sub_numstr;
				sprintf(str, "%.6f", fp.fy*10000);
				keyword += str;
				onelinestr += keyword;
			}
		}

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}
}

void CESDDoc::Save_99_PCB_ASCII_File(CArchive& ar)
{
	CString onelinestr;
	CString csLineEnd = "\r\n";
	CString csLineSoftEnd = "\r";
	CString keyword;
	char str[1000];

	keyword = "|RECORD=Board";
	onelinestr += keyword;

	onelinestr += "|FILENAME=|KIND=Protel_Advanced_PCB|VERSION=3.00|DATE=|TIME=";
	
	keyword = "|ORIGINX=";
	mil_to_str(m_cPCB_Sheet.m_fOriginX, str, WITH_MIL, 4);
	keyword += str;
	onelinestr += keyword;

	keyword = "|ORIGINY=";
	mil_to_str(m_cPCB_Sheet.m_fOriginY, str, WITH_MIL, 4);
	keyword += str;
	onelinestr += keyword;
	
	keyword = "|BIGVISIBLEGRIDSIZE=";
	mil_to_str(m_cPCB_Sheet.m_fBigVisibleGridSize*10000, str, WITHOUT_MIL, 3);
	keyword += str;
	onelinestr += keyword;

	keyword = "|VISIBLEGRIDSIZE=";
	mil_to_str(m_cPCB_Sheet.m_fVisibleGridSize*10000, str, WITHOUT_MIL, 3);
	keyword += str;
	onelinestr += keyword;

	keyword = "|ELECTRICALGRIDRANGE=";
	mil_to_str(m_cPCB_Sheet.m_fElecGridRange, str, WITH_MIL, 4);
	keyword += str;
	onelinestr += keyword;

	keyword = "|ELECTRICALGRIDENABLED=";
	if(m_cPCB_Sheet.m_bElecGridEnable == TRUE) keyword += "TRUE";
	else keyword += "FALSE";
	onelinestr += keyword;

	keyword = "|SNAPGRIDSIZE=";
	mil_to_str(m_cPCB_Sheet.m_fSnapGridSize*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;

	keyword = "|SNAPGRIDSIZEX=";
	mil_to_str(m_cPCB_Sheet.m_fSnapGridSizeX*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;
	
	keyword = "|SNAPGRIDSIZEY=";
	mil_to_str(m_cPCB_Sheet.m_fSnapGridSizeY*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;
	
	keyword = "|TRACKGRIDSIZE=";
	mil_to_str(m_cPCB_Sheet.m_fTrackGridSize*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;
	
	keyword = "|VIAGRIDSIZE=";
	mil_to_str(m_cPCB_Sheet.m_fViaGridSize*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;

	keyword = "|COMPONENTGRIDSIZE=";
	mil_to_str(m_cPCB_Sheet.m_fComponentGridSize*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;

	keyword = "|COMPONENTGRIDSIZEX=";
	mil_to_str(m_cPCB_Sheet.m_fComponentGridSizeX*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;

	keyword = "|COMPONENTGRIDSIZEY=";
	mil_to_str(m_cPCB_Sheet.m_fComponentGridSizeY*10000, str, WITHOUT_MIL, 6);
	keyword += str;
	onelinestr += keyword;

	keyword = "|DOTGRID=";
	if(m_cPCB_Sheet.m_bDotGrid == TRUE) keyword += "TRUE";
	else keyword += "FALSE";
	onelinestr += keyword;
	
	keyword = "|DISPLAYUNIT=";
	itoa(m_cPCB_Sheet.m_nUnit, str, 10);
	keyword += str;
	onelinestr += keyword;

	for(int i=0; i<POWER_LAYER_NUM; i++)
	{
		char layernum[100];
		itoa(i+1, layernum, 10);
		keyword = "|PLANE";
		keyword += layernum;
		keyword += "NETNAME=";
		if(m_csPowerPlane[i].IsEmpty() == TRUE) keyword += "(No Net)";
		else if(m_csPowerPlane[i] == "ŒﬁÕ¯¬Á") keyword += "(No Net)";
		else if(m_csPowerPlane[i] == "∂‡Õ¯¬Á") keyword += "(Multiple Nets)";
		else keyword += m_csPowerPlane[i];
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	onelinestr += "|TOPTYPE=3|TOPCONST=3.500|TOPHEIGHT=0.4mil|TOPMATERIAL=Solder Resist";
	onelinestr += "|BOTTOMTYPE=3|BOTTOMCONST=3.500|BOTTOMHEIGHT=0.4mil|BOTTOMMATERIAL=Solder Resist";
	onelinestr += "|LAYERSTACKSTYLE=0|SHOWTOPDIELECTRIC=FALSE|SHOWBOTTOMDIELECTRIC=FALSE";

	for(int i=1; i<=5; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=6; i<=10; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=11; i<=15; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=16; i<=20; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=21; i<=25; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=26; i<=30; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=31; i<=35; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=36; i<=40; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=41; i<=45; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=46; i<=50; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=51; i<=55; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=56; i<=60; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=61; i<=65; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=66; i<=70; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=71; i<=75; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=76; i<=80; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;
	for(int i=81; i<=82; i++)
	{
		char layernum[100];
		itoa(i, layernum, 10);

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NAME=";
		keyword += m_sPCB_LayerParam[i-1].csLayerName;
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "PREV=";
		itoa(m_sPCB_LayerParam[i-1].nLayerPrev + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "NEXT=";
		itoa(m_sPCB_LayerParam[i-1].nLayerNext + 1, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "MECHENABLED=";
		if(m_sPCB_LayerParam[i-1].bLayerMechEnabled == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "COPTHICK=1.4mil";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELTYPE=0";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELCONST=4.800";	
		onelinestr += keyword;

		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELHEIGHT=12.6mil";	
		onelinestr += keyword;
		
		keyword = "|LAYER";
		keyword += layernum;
		keyword += "DIELMATERIAL=FR-4";	
		onelinestr += keyword;
	}

	onelinestr += "|LAYERPAIR0LOW=TOP|LAYERPAIR0HIGH=BOTTOM|LAYERPAIR0DRILLGUIDE=FALSE|LAYERPAIR0DRILLDRAWING=FALSE";

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	keyword = "|RECORD=Board";
	onelinestr += keyword;

	keyword = "|TOGGLELAYERS=";
	for(int i=0; i<PCB_LAYER_NUM; i++)
	{
		if(m_sPCB_LayerParam[i].bLayerShow == TRUE) keyword += "1";
		else keyword += "0";
	}
	onelinestr += keyword;

	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	onelinestr += "|RECORD=EngineeringChangeOrderOptions|ECOISACTIVE=FALSE|ECOFILENAME=PCB.ECO";

	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();

	onelinestr += "|RECORD=OutputOptions|DRILLGUIDEHOLESIZE=30mil|DRILLDRAWSYMBOLSIZE=50mil|DRILLSYMBOLKIND=0|MULTILAYERONPADMASTER=TRUE|TOPLAYERONPADMASTER=TRUE|BOTTOMLAYERONPADMASTER=TRUE";
	onelinestr += "|INCLUDEVIASINSOLDERMASK=TRUE|INCLUDEUNCONNECTEDPADS=TRUE";
	onelinestr += "|INCLUDEMECH1WITHALLPLOTS=FALSE|INCLUDEMECH2WITHALLPLOTS=FALSE|INCLUDEMECH3WITHALLPLOTS=FALSE|INCLUDEMECH4WITHALLPLOTS=FALSE";
	onelinestr += "|INCLUDEMECH5WITHALLPLOTS=FALSE|INCLUDEMECH6WITHALLPLOTS=FALSE|INCLUDEMECH7WITHALLPLOTS=FALSE|INCLUDEMECH8WITHALLPLOTS=FALSE";
	onelinestr += "|INCLUDEMECH9WITHALLPLOTS=FALSE|INCLUDEMECH10WITHALLPLOTS=FALSE|INCLUDEMECH11WITHALLPLOTS=FALSE|INCLUDEMECH12WITHALLPLOTS=FALSE";
	onelinestr += "|INCLUDEMECH13WITHALLPLOTS=FALSE|INCLUDEMECH14WITHALLPLOTS=FALSE|INCLUDEMECH15WITHALLPLOTS=FALSE|INCLUDEMECH16WITHALLPLOTS=FALSE";

	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	
	onelinestr += "|RECORD=OutputOptions|PLOTLAYERS=0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
	onelinestr += "|FLIPLAYERS=0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";

	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	

	onelinestr += "|RECORD=PrinterOptions|DEVICE=|DRIVER=|OUTPUT=|SHOWHOLES=FALSE|SCALETOFITPAGE=FALSE|USEPRINTERFONTS=FALSE|USESOFTWAREARCS=FALSE|BATCHTYPE=0|COMPOSITETYPE=0|CBORDERSIZE=1000mil|SCALE=0.0001mil|XCORRECT=0.0001mil|YCORRECT=0.0001mil";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PLOTPADNETS=TRUE|PLOTPADNUMBERS=TRUE|PLOTTERSCALE=0.0001mil|PLOTTERXCORRECT=0.0001mil|PLOTTERYCORRECT=0.0001mil|PLOTTERXOFFSET=0mil|PLOTTERYOFFSET=0mil|PLOTTERSHOWHOLES=FALSE|PLOTTERUSESOFTWAREARCS=FALSE";
	onelinestr += "|PLOTTERWAITBETWEENSHEETS=FALSE|PLOTTEROUTPUTPORT=0|PLOTTERLANGUAGE=0";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|SPD1=3|THK1=13mil|SPD2=3|THK2=13mil|SPD3=3|THK3=13mil|SPD4=3|THK4=13mil|SPD5=3|THK5=13mil|SPD6=3|THK6=13mil|SPD7=3|THK7=13mil|SPD8=3|THK8=13mil";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PLOTMODE=000000000000000000000|DRIVERTYPE=0";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PLOTLAYERS=1111111111111111111111111111111111111111111111111111111111111111111111111111111111";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PP1=1|PP2=1|PP3=1|PP4=1|PP5=1|PP6=1|PP7=1|PP8=1|PP9=1|PP10=1|PP11=1|PP12=1|PP13=1|PP14=1|PP15=1|PP16=1|PP17=1|PP18=1|PP19=1|PP20=1|PP21=1|PP22=1|PP23=1|PP24=1|PP25=1|PP26=1|PP27=1|PP28=1";
	onelinestr += "|PP29=1|PP30=1|PP31=1|PP32=1|PP33=1|PP34=1|PP35=1|PP36=1|PP37=1|PP38=1|PP39=1|PP40=1|PP41=1|PP42=1|PP43=1|PP44=1|PP45=1|PP46=1|PP47=1|PP48=1|PP49=1|PP50=1|PP51=1|PP52=1|PP53=1|PP54=1|PP55=1|PP56=1|PP57=1|PP58=1";
	onelinestr += "|PP59=1|PP60=1|PP61=1|PP62=1|PP63=1|PP64=1|PP65=1|PP66=1|PP67=1|PP68=1|PP69=1|PP70=1|PP71=1|PP72=1|PP73=1|PP74=1|PP75=1|PP76=1|PP77=1|PP78=1|PP79=1|PP80=1|PP81=1|PP82=1";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM1=2105376|PM2=197379|PM3=394758|PM4=592137|PM5=789516|PM6=986895|PM7=1184274|PM8=1381653|PM9=1579032|PM10=1776411";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM11=1973790|PM12=2171169|PM13=2368548|PM14=2565927|PM15=2763306|PM16=2960685|PM17=3158064|PM18=3355443|PM19=3552822|PM20=3750201";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM21=3947580|PM22=4144959|PM23=4342338|PM24=4539717|PM25=4737096|PM26=4934475|PM27=5131854|PM28=5329233|PM29=5526612|PM30=5723991";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM31=5921370|PM32=4210752|PM33=12632256|PM34=6513507|PM35=6710886|PM36=6908265|PM37=7105644|PM38=7303023|PM39=7500402|PM40=7697781";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM41=7895160|PM42=8092539|PM43=8289918|PM44=8487297|PM45=8684676|PM46=8882055|PM47=9079434|PM48=9276813|PM49=9474192|PM50=9671571";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM51=9868950|PM52=10066329|PM53=10263708|PM54=10461087|PM55=10658466|PM56=10855845|PM57=11053224|PM58=11250603|PM59=11447982|PM60=11645361";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM61=11842740|PM62=12040119|PM63=12237498|PM64=12434877|PM65=12632256|PM66=12829635|PM67=13027014|PM68=13224393|PM69=13421772|PM70=13619151";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM71=13816530|PM72=14013909|PM73=14211288|PM74=0|PM75=14606046|PM76=16777215|PM77=15000804|PM78=15198183|PM79=15395562|PM80=15592941";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PM81=15790320|PM82=15987699";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC1=255|PC2=36540|PC3=16440176|PC4=6736896|PC5=16737945|PC6=16776960|PC7=8388736|PC8=16711935|PC9=32896|PC10=65535";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC11=8421504|PC12=16777215|PC13=8388736|PC14=8421376|PC15=12632256|PC16=128|PC17=32768|PC18=65280|PC19=8388608|PC20=16776960";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC21=8388736|PC22=16711935|PC23=32896|PC24=65535|PC25=8421504|PC26=16777215|PC27=8388736|PC28=8421376|PC29=12632256|PC30=128";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC31=32768|PC32=16711680|PC33=65535|PC34=32896|PC35=8421504|PC36=128|PC37=8388736|PC38=16711935|PC39=32768|PC40=128";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC41=8388736|PC42=8421376|PC43=32768|PC44=128|PC45=8388736|PC46=8421376|PC47=32768|PC48=128|PC49=8388736|PC50=8421376";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC51=32768|PC52=128|PC53=8388736|PC54=8421376|PC55=128|PC56=16711935|PC57=16711935|PC58=8388736|PC59=32768|PC60=32896";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC61=16711935|PC62=8388736|PC63=32768|PC64=32896|PC65=16711935|PC66=8388736|PC67=32768|PC68=32896|PC69=16711935|PC70=8388736";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC71=32768|PC72=0|PC73=2752767|PC74=12632256|PC75=7709086|PC76=0|PC77=65280|PC78=16777215|PC79=6049101|PC80=9473425";
	onelinestr += csLineSoftEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();
	onelinestr += "|RECORD=PrinterOptions|PC81=9474304|PC82=25217";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	

	onelinestr += "|RECORD=GerberOptions|SORTOUTPUT=FALSE|USESOFTWAREARCS=TRUE|CENTERPHOTOPLOTS=TRUE|EMBEDAPERTURES=FALSE|PANELIZE=FALSE|G54=FALSE|PLUSTOL=0.005mil|MINUSTOL=0.005mil|FILMSIZEX=20000mil";
	onelinestr += "|FILMSIZEY=16000mil|BORDERSIZE=1000mil|APTTABLE=|MAXAPERSIZE=250mil|RELIEFSHAPESALLOWED=TRUE|PADSFLASHONLY=TRUE|GERBERUNITS=3|GERBERDECS=1|FLASHALLFILLS=FALSE";

	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	

	onelinestr += "|RECORD=AdvancedPlacerOptions|PLACELARGECLEAR=50mil|PLACESMALLCLEAR=20mil|PLACEUSEROTATION=TRUE|PLACEUSELAYERSWAP=FALSE|PLACEBYPASSNET1=|PLACEBYPASSNET2=|PLACEUSEADVANCEDPLACE=TRUE|PLACEUSEGROUPING=TRUE";

	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	

	onelinestr += "|RECORD=DesignRuleCheckerOptions|DOMAKEDRCFILE=TRUE|DOMAKEDRCERRORLIST=TRUE|DOSUBNETDETAILS=TRUE|REPORTFILENAME=|EXTERNALNETLISTFILENAME=|CHECKEXTERNALNETLIST=FALSE|MAXVIOLATIONCOUNT=500";
	onelinestr += "|REPORTDRILLEDSMTPADS=TRUE|REPORTINVALIDMULTILAYERPADS=TRUE|RULESETTOCHECK=0,1,2,3,4,5,15,16,18,19,21,22,23,42,45,46,47,50|ONLINERULESETTOCHECK=0,1,2,3,4,5,15,17,18,19,21,22,23,24,25,42,43,44,45,46,47,48,50|INTERNALPLANEWARNINGS=TRUE|VERIFYSHORTINGCOPPER=TRUE";

	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	

	for(int i=0; i<m_arrayPcbClass.GetCount(); i++)
	{
		COb_pcbclass* pclass = (COb_pcbclass*)m_arrayPcbClass.GetAt(i);

		keyword = "|RECORD=Class";
		onelinestr += keyword;

		keyword = "|NAME=";
		keyword += pclass->m_csName;
		onelinestr += keyword;

		keyword = "|KIND=";
		itoa(pclass->m_nKind, str, 10);
		keyword += str;
		onelinestr += keyword;
			
		keyword = "|SUPERCLASS=";
		if(pclass->m_bSuperClass == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;	

		for(int j=0; j<pclass->m_arrayMember.GetCount(); j++)
		{
			char numstr[100];
			itoa(j, numstr, 10);  strcat(numstr, "=");

			keyword = "|M";	keyword += numstr;
			keyword += pclass->m_arrayMember.GetAt(j);
			onelinestr += keyword;
		}

		onelinestr += "|SELECTED=FALSE";

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	CArray<POINT, POINT> arrayNet;
	CArray<POINT, POINT> arrayComp;
	CArray<POINT, POINT> arrayPolygon;
	int newindex;

	newindex = 0;
	for(int i=0; i<m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = (COb_pcbnet*)m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;
			
		POINT point;
		point.x = i;
		point.y = newindex;
		arrayNet.Add(point);

		keyword = "|RECORD=Net";
		onelinestr += keyword;

		keyword = "|ID=";
		itoa(newindex, str, 10);
		keyword += str;
		onelinestr += keyword;
		newindex++;

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|NAME=";
		keyword += pnet->m_csNetName;
		onelinestr += keyword;

		keyword = "|VISIBLE=";
		if(pnet->m_bVisible == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;	
		
		keyword = "|COLOR=";
		itoa(pnet->m_nColor, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	newindex = 0;
	for(int i=0; i<m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = (COb_pcbcomp*)m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag != FLAG_NORMAL) continue;
		
		POINT point;
		point.x = i;
		point.y = newindex;
		arrayComp.Add(point);

		keyword = "|RECORD=Component";
		onelinestr += keyword;

		keyword = "|ID=";
		itoa(newindex, str, 10);
		keyword += str;
		onelinestr += keyword;
		newindex++;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[pcomp->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(pcomp->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(pcomp->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|USERROUTED=";
		if(pcomp->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|PRIMITIVELOCK=";
		if(pcomp->m_bPrimitiveLock == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|X=";
		mil_to_str(pcomp->m_fX, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|Y=";
		mil_to_str(pcomp->m_fY, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|PATTERN=";
		keyword += pcomp->m_csPattern;
		onelinestr += keyword;
		
		keyword = "|NAMEON=";
		if(pcomp->m_bNameOn == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|COMMENTON=";
		if(pcomp->m_bCommentOn == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|GROUPNUM=";
		itoa(pcomp->m_nGroupNum, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|COUNT=";
		itoa(pcomp->m_nCount, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|ROTATION=";
		sprintf(str, "%.14E", pcomp->m_fRotation);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|HEIGHT=";
		mil_to_str(pcomp->m_fHeight, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|NAMEAUTOPOSITION=";
		itoa(pcomp->m_nNamePosition, str, 10);
		keyword += str;
		onelinestr += keyword;			
		
		keyword = "|COMMENTAUTOPOSITION=";
		itoa(pcomp->m_nCommentPosition, str, 10);
		keyword += str;
		onelinestr += keyword;			

		keyword = "|UNIONINDEX=";
		itoa(pcomp->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	int dimension_num = -1;
	for(int i=0; i<m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = (COb_pcbdimension*)m_arrayPcbDimension.GetAt(i);
		if(pdimension->m_nFlag != FLAG_NORMAL) continue;
		if(pdimension->m_nDimensionKind != 8) continue;
		dimension_num++;

		keyword = "|RECORD=Dimension";
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[pdimension->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(pdimension->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(pdimension->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|USERROUTED=";
		if(pdimension->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|UNIONINDEX=";
		itoa(pdimension->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|PRIMITIVELOCK=";
		if(pdimension->m_bPrimitiveLock == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|DIMENSIONLAYER=";
		keyword += csEDA_PCB_LayerLabel[pdimension->m_nLayer];
		onelinestr += keyword;

		keyword = "|DIMENSIONLOCKED=";
		if(pdimension->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		onelinestr += "|OBJECTID=13";
		
		keyword = "|DIMENSIONKIND=";
		itoa(pdimension->m_nDimensionKind, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|DRCERROR=FALSE";

		keyword = "|VINDEXFORSAVE=";
		itoa(dimension_num, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LX=";
		mil_to_str(pdimension->m_fLx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|LY=";
		mil_to_str(pdimension->m_fLy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|HX=";
		mil_to_str(pdimension->m_fHx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|HY=";
		mil_to_str(pdimension->m_fHy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|X1=";
		mil_to_str(pdimension->m_fXs, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|Y1=";
		mil_to_str(pdimension->m_fYs, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|X2=";
		mil_to_str(pdimension->m_fXe, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|Y2=";
		mil_to_str(pdimension->m_fYe, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTX=";
		mil_to_str(pdimension->m_fTextSx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;		
		keyword = "|TEXTY=";
		mil_to_str(pdimension->m_fTextSy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;					
		keyword = "|HEIGHT=";
		mil_to_str(pdimension->m_fHeight, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;					
		keyword = "|LINEWIDTH=";
		mil_to_str(pdimension->m_fLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTHEIGHT=";
		mil_to_str(pdimension->m_fTextHeight, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|TEXTWIDTH=";
		mil_to_str(pdimension->m_fTextLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|FONT=";
		keyword += csPCB_StrokeFontName[pdimension->m_nStrokeFont];
		onelinestr += keyword;
		
		keyword = "|STYLE=";
		if(pdimension->m_nUnitStyle == UNIT_STYLE_NORMAL) keyword += "Normal";
		else if(pdimension->m_nUnitStyle == UNIT_STYLE_BRACKETS) keyword += "Brackets";
		else keyword += "None";
		onelinestr += keyword;

		keyword = "|TEXTLINEWIDTH=";
		mil_to_str(pdimension->m_fTextLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|TEXTPOSITION=Auto";

		keyword = "|TEXTGAP=";
		mil_to_str(pdimension->m_fTextGap, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTFORMAT=";
		if(pdimension->m_nTextFormat == 2) keyword += "10mil";
		else if(pdimension->m_nTextFormat == 3) keyword += "10 (mil)";
		else keyword += "10";
		onelinestr += keyword;

		keyword = "|TEXTDIMENSIONUNIT=";
		if(pdimension->m_nTextDimensionUnit == UNIT_MIL) keyword += "Mils";
		else if(pdimension->m_nTextDimensionUnit == UNIT_MM) keyword += "Millimeters";
		else if(pdimension->m_nTextDimensionUnit == UNIT_INCH) keyword += "Inches";
		else if(pdimension->m_nTextDimensionUnit == UNIT_CM) keyword += "Centimeters";
		else keyword += "AutomaticUnit";
		onelinestr += keyword;
		
		keyword = "|TEXTPRECISION=";
		itoa(pdimension->m_nTextPrecision, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTPREFIX=";
		keyword += pdimension->m_csPrefix;
		onelinestr += keyword;
		
		keyword = "|TEXTSUFFIX=";
		keyword += pdimension->m_csSuffix;
		onelinestr += keyword;		
		
		keyword = "|ARROWSIZE=";
		mil_to_str(pdimension->m_fArrowSize, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;		

		keyword = "|ARROWLINEWIDTH=";
		mil_to_str(pdimension->m_fArrowLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|ARROWLENGTH=";
		mil_to_str(pdimension->m_fArrowLength, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|ARROWPOSITION=";
		if(pdimension->m_nArrowPosition == 0) keyword += "Inside";
		else keyword += "Outside";
		onelinestr += keyword;	

		keyword = "|EXTENSIONOFFSET=";
		mil_to_str(pdimension->m_fExtensionOffset, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	
		
		keyword = "|EXTENSIONLINEWIDTH=";
		mil_to_str(pdimension->m_fExtensionLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|EXTENSIONPICKGAP=";
		mil_to_str(pdimension->m_fExtensionPickGap, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|REFERENCES_COUNT=";
		itoa(pdimension->m_cRefPoint.GetCount(), str, 10);
		keyword += str;
		onelinestr += keyword;

		for(int j=0; j<pdimension->m_cRefPoint.GetCount(); j++)
		{
			Fpoint fpoint = pdimension->m_cRefPoint.GetAt(j);
			char numstr[100];
			itoa(j, numstr, 10);
			keyword = "|REFERENCE"; keyword += numstr; keyword += "PRIM=-1";
			keyword += "|REFERENCE"; keyword += numstr; keyword += "OBJECTID=-1";
			keyword += "|REFERENCE"; keyword += numstr; keyword += "OBJECTSTRING=NoObject";
			keyword += "|REFERENCE"; keyword += numstr; keyword += "POINTX=";
			mil_to_str(fpoint.fx, str, WITH_MIL, 4);
			keyword += str;
			keyword += "|REFERENCE"; keyword += numstr; keyword += "POINTY=";
			mil_to_str(fpoint.fy, str, WITH_MIL, 4);
			keyword += str;
			keyword += "|REFERENCE"; keyword += numstr; keyword += "ANCHOR=-1";
			onelinestr += keyword;
		}

		keyword = "|TEXT1X=";
		mil_to_str(pdimension->m_fTextSx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	
		
		keyword = "|TEXT1Y=";
		mil_to_str(pdimension->m_fTextSy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	
		
		keyword = "|TEXT1ANGLE=";
		sprintf(str, "%.14E", pdimension->m_fTextRotation);
		keyword += str;
		onelinestr += keyword;	

		onelinestr += "|TEXT1MIRROR=FALSE";

		keyword = "|USETTFONTS=";
		if(pdimension->m_bUseTTFont == FALSE) keyword += "FALSE";
		else keyword += "TRUE";
		onelinestr += keyword;	
		
		keyword = "|BOLD=";
		if(pdimension->m_bBold == FALSE) keyword += "FALSE";
		else keyword += "TRUE";
		onelinestr += keyword;	

		keyword = "|ITALIC=";
		if(pdimension->m_bItalic == FALSE) keyword += "FALSE";
		else keyword += "TRUE";
		onelinestr += keyword;	

		keyword = "|FONTNAME=";
		keyword += pdimension->m_csTTFontName;
		onelinestr += keyword;	
		
		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}
    
	for(int i=0; i<m_arrayPcbCoordinate.GetCount(); i++)
	{
		COb_pcbcoordinate* pcoordinate = (COb_pcbcoordinate*)m_arrayPcbCoordinate.GetAt(i);
		if(pcoordinate->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Coordinate";
		onelinestr += keyword;

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[pcoordinate->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(pcoordinate->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(pcoordinate->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|USERROUTED=";
		if(pcoordinate->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|UNIONINDEX=";
		itoa(pcoordinate->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|PRIMITIVELOCK=";
		if(pcoordinate->m_bPrimitiveLock == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|X=";
		mil_to_str(pcoordinate->m_fX, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y=";
		mil_to_str(pcoordinate->m_fY, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SIZE=";
		mil_to_str(pcoordinate->m_fSize, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LINEWIDTH=";
		mil_to_str(pcoordinate->m_fLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTHEIGHT=";
		mil_to_str(pcoordinate->m_fTextHeight, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTWIDTH=";
		mil_to_str(pcoordinate->m_fTextWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|FONT=";
		keyword += csPCB_StrokeFontName[pcoordinate->m_nStrokeFont];
		onelinestr += keyword;

		if(pcoordinate->m_nUnitStyle != 0)
		{
			keyword = "|STYLE=";
			if(pcoordinate->m_nUnitStyle == UNIT_STYLE_NORMAL) keyword += "Normal";
			else if(pcoordinate->m_nUnitStyle == UNIT_STYLE_BRACKETS) keyword += "Brackets";
			onelinestr += keyword;
		}
		
		keyword = "|ROTATION=";
		sprintf(str, "%.14E", pcoordinate->m_fRotation);
        keyword += str;
		onelinestr += keyword;
		
		keyword = "|USETTFONTS=";
		if(pcoordinate->m_bUseTTFont == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|BOLD=";
		if(pcoordinate->m_bBold == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;		
		
		keyword = "|ITALIC=";
		if(pcoordinate->m_bItalic == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;			
		
		keyword = "|FONTNAME=";
		keyword += pcoordinate->m_csTTFontName;
		onelinestr += keyword;			

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	int nPolygon = 0;
	newindex = 0;
	for(int i=0; i<m_arrayPcbPolygon.GetCount(); i++)
	{
		COb_pcbpolygon* ppolygon = (COb_pcbpolygon*)m_arrayPcbPolygon.GetAt(i);
		if(ppolygon->m_nFlag != FLAG_NORMAL) continue;

		POINT point;
		point.x = i;
		point.y = newindex;
		arrayComp.Add(point);

		keyword = "|RECORD=Polygon";
		onelinestr += keyword;

		keyword = "|ID=";
		itoa(newindex, str, 10);
		keyword += str;
		onelinestr += keyword;
		newindex++;
	
		int netbak = ppolygon->m_nNet;
		for(int j=0; j<arrayNet.GetCount(); j++)
		{
			POINT point;
			point = arrayNet.GetAt(j);
			if(point.x == ppolygon->m_nNet) 
			{
				ppolygon->m_nNet = point.y;
				break;
			}
		}

		keyword = "|NET=";
		itoa(ppolygon->m_nNet, str, 10);
		keyword += str;
		onelinestr += keyword;
        
		ppolygon->m_nNet = netbak;

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[ppolygon->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(ppolygon->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(ppolygon->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|USERROUTED=";
		if(ppolygon->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|UNIONINDEX=";
		itoa(ppolygon->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|PRIMITIVELOCK=";
		if(ppolygon->m_bPrimitiveLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONTYPE=";
		if(ppolygon->m_nPolygonType == COPPER_POLYGON) keyword += "Polygon";
		else if(ppolygon->m_nPolygonType == SPLITPLANE) keyword += "Split Plane";
		onelinestr += keyword;

		keyword = "|POUROVER=";
		if(ppolygon->m_bPourOver == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|REMOVEDEAD=";
		if(ppolygon->m_bRemoveDead == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|GRIDSIZE=";
		mil_to_str(ppolygon->m_fGridSize, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TRACKWIDTH=";
		mil_to_str(ppolygon->m_fTrackWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|HATCHSTYLE=";
		if(ppolygon->m_nHatchStyle == PCB_HATCH_SOLID) keyword += "Solid";
		else if(ppolygon->m_nHatchStyle == PCB_HATCH_90) keyword += "90Degree";
		else if(ppolygon->m_nHatchStyle == PCB_HATCH_45) keyword += "45Degree";
		else if(ppolygon->m_nHatchStyle == PCB_HATCH_H) keyword += "Horizontal";
		else if(ppolygon->m_nHatchStyle == PCB_HATCH_V) keyword += "Vertical";
		else if(ppolygon->m_nHatchStyle == PCB_HATCH_NONE) keyword += "None";
		onelinestr += keyword;

		keyword = "|USEOCTAGONS=";
		if(ppolygon->m_bUseOctagons == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|MINPRIMLENGTH=";
		mil_to_str(ppolygon->m_fMinPrimLength, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
	
		for(int i=0; i<ppolygon->m_cVertex.GetCount(); i++)
		{
			Struct_RegionVertex vertex;
			vertex = ppolygon->m_cVertex.GetAt(i);
			char numstr[100];
			itoa(i, numstr, 10);  strcat(numstr, "=");

			keyword = "|KIND";	keyword += numstr;
			if(vertex.radius != 0) keyword += "1";
			else keyword += "0";
			onelinestr += keyword;

			keyword = "|VX";	keyword += numstr;
			mil_to_str(vertex.fx, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|VY";	keyword += numstr;
			mil_to_str(vertex.fy, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|CX";	keyword += numstr;
			mil_to_str(vertex.cx, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|CY";	keyword += numstr;
			mil_to_str(vertex.cy, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|SA";	keyword += numstr;	keyword += " ";
			sprintf(str, "%.14E", vertex.sangle);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|EA";	keyword += numstr;	keyword += " ";
			sprintf(str, "%.14E", vertex.eangle);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|R";	keyword += numstr;
			mil_to_str(vertex.radius, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
		}

		keyword = "|SHELVED=";
		if(ppolygon->m_bShelved == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		onelinestr += "|RESTORELAYER=TOP|RESTORENET=";

		keyword = "|REMOVEISLANDSBYAREA=";
		if(ppolygon->m_bRemoveIslandsByArea == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|REMOVENECKS=";
		if(ppolygon->m_bRemoveNecks == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
			
		keyword = "|AREATHRESHOLD=";
		mil_to_str(ppolygon->m_fAreaThreshold*100000000, str, WITHOUT_MIL, 6);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|ARCRESOLUTION=";
		mil_to_str(ppolygon->m_fArcResolution, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|NECKWIDTHTHRESHOLD=";
		mil_to_str(ppolygon->m_fNeckWidthThreshold, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|POUROVERSTYLE=";
		itoa(ppolygon->m_nPourOverStyle, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|NAME=";
		CString cstring = ppolygon->m_csName;
		for(int i=0; i<ppolygon->m_csName.GetLength(); i++)
		{
			unsigned char c = cstring.GetAt(0);
			itoa((unsigned int)c, str, 10);
			keyword += str;
			if(i !=	ppolygon->m_csName.GetLength()-1) keyword += ",";
			cstring = cstring.Right(cstring.GetLength() -1);
		}
		onelinestr += keyword;
		
		keyword = "|POURINDEX=";
		if(ppolygon->m_nPolygonType == COPPER_POLYGON)
		{
			itoa(nPolygon + 1000, str, 10);
		}
		else strcpy(str, "-1");
		keyword += str;
		onelinestr += keyword;

		keyword = "|IGNOREVIOLATIONS";
		if(ppolygon->m_bIgnoreViolations == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}
    
	for(int i=0; i<m_arrayPcbConnection.GetCount(); i++)
	{
		COb_pcbconnection* pconnection = (COb_pcbconnection*)m_arrayPcbConnection.GetAt(i);

		keyword = "|RECORD=Connection";
		onelinestr += keyword;

		int netbak = pconnection->m_nNet;
		for(int j=0; j<arrayNet.GetCount(); j++)
		{
			POINT point;
			point = arrayNet.GetAt(j);
			if(point.x == pconnection->m_nNet) 
			{
				pconnection->m_nNet = point.y;
				break;
			}
		}

		keyword = "|NET=";
		itoa(pconnection->m_nNet, str, 10);
		keyword += str;
		onelinestr += keyword;

		pconnection->m_nNet = netbak;	

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE|LAYER=MULTILAYER|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0";
		onelinestr += keyword;
		
		keyword = "|X1=";
		mil_to_str(pconnection->m_fX1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y1=";
		mil_to_str(pconnection->m_fY1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;		
		
		keyword = "|X2=";
		mil_to_str(pconnection->m_fX2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y2=";
		mil_to_str(pconnection->m_fY2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;			
		
		keyword = "|LAYER1=";
		keyword += csEDA_PCB_LayerLabel[pconnection->m_nLayer1];
		onelinestr += keyword;
		
		keyword = "|LAYER2=";
		keyword += csEDA_PCB_LayerLabel[pconnection->m_nLayer2];
		onelinestr += keyword;

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=0|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=ShortCircuit|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=ShortCircuit|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=HKPCITJB|DEFINEDBYLOGICALDOCUMENT=FALSE|ALLOWED=FALSE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=0|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=ShortCircuit|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=ShortCircuit|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|ALLOWED=FALSE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=1|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=UnRoutedNet|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=UnRoutedNet|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=KTCTOSHV|DEFINEDBYLOGICALDOCUMENT=FALSE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=1|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=UnRoutedNet|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=UnRoutedNet|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=2|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=Clearance|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=Clearance|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=PTEAWJYR|DEFINEDBYLOGICALDOCUMENT=FALSE|GAP=10mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=2|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=Clearance|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Clearance|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|GAP=10mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=3|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=PlaneConnect|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=PlaneConnect|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=VUCJOKLA|DEFINEDBYLOGICALDOCUMENT=FALSE|PLANECONNECTSTYLE=Relief|RELIEFEXPANSION=20mil|RELIEFENTRIES=4|RELIEFCONDUCTORWIDTH=10mil|RELIEFAIRGAP=10mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=3|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=PlaneConnect|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=PlaneConnect|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|PLANECONNECTSTYLE=Relief|RELIEFEXPANSION=20mil|RELIEFENTRIES=4|RELIEFCONDUCTORWIDTH=10mil|RELIEFAIRGAP=10mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=4|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=PlaneClearance|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=PlaneClearance|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=IHSYRHBV|DEFINEDBYLOGICALDOCUMENT=FALSE|CLEARANCE=20mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=4|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=PlaneClearance|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=PlaneClearance|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|CLEARANCE=20mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=5|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=PolygonConnect|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=PolygonConnect|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=UFUGCYCM|DEFINEDBYLOGICALDOCUMENT=FALSE|CONNECTSTYLE=Relief|RELIEFCONDUCTORWIDTH=10mil|RELIEFENTRIES=4|POLYGONRELIEFANGLE=90 Angle";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=5|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=PolygonConnect|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=PolygonConnect|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|CONNECTSTYLE=Relief|RELIEFCONDUCTORWIDTH=10mil|RELIEFENTRIES=4|POLYGONRELIEFANGLE=90 Angle";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=6|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=Width|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=Width|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=YNESDDCY|DEFINEDBYLOGICALDOCUMENT=FALSE|MAXLIMIT=10mil|MINLIMIT=10mil|PREFEREDWIDTH=10mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=6|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=Width|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Width|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|MAXLIMIT=10mil|MINLIMIT=10mil|PREFEREDWIDTH=10mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=7|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=RoutingPriority|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=RoutingPriority|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=SWPYJATW|DEFINEDBYLOGICALDOCUMENT=FALSE|ROUTINGPRIORITY=0";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=7|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=RoutingPriority|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=RoutingPriority|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|PRIORITY=0";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=8|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=RoutingLayers|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=RoutingLayers|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=GXULASHX|DEFINEDBYLOGICALDOCUMENT=FALSE|TOP LAYER_V5=TRUE|MID LAYER 1_V5=TRUE|MID LAYER 2_V5=TRUE|MID LAYER 3_V5=TRUE|MID LAYER 4_V5=TRUE|MID LAYER 5_V5=TRUE|MID LAYER 6_V5=TRUE|MID LAYER 7_V5=TRUE|MID LAYER 8_V5=TRUE|MID LAYER 9_V5=TRUE|MID LAYER 10_V5=TRUE|MID LAYER 11_V5=TRUE|MID LAYER 12_V5=TRUE|MID LAYER 13_V5=TRUE|MID LAYER 14_V5=TRUE|MID LAYER 15_V5=TRUE|MID LAYER 16_V5=TRUE|MID LAYER 17_V5=TRUE|MID LAYER 18_V5=TRUE|MID LAYER 19_V5=TRUE|MID LAYER 20_V5=TRUE|MID LAYER 21_V5=TRUE|MID LAYER 22_V5=TRUE|MID LAYER 23_V5=TRUE|MID LAYER 24_V5=TRUE|MID LAYER 25_V5=TRUE|MID LAYER 26_V5=TRUE|MID LAYER 27_V5=TRUE|MID LAYER 28_V5=TRUE|MID LAYER 29_V5=TRUE|MID LAYER 30_V5=TRUE|BOTTOM LAYER_V5=TRUE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=8|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=RoutingLayers|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=RoutingLayers|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|TOP LAYER=Automatic|MID LAYER 1=Automatic|MID LAYER 2=Automatic|MID LAYER 3=Automatic|MID LAYER 4=Automatic|MID LAYER 5=Automatic|MID LAYER 6=Automatic|MID LAYER 7=Automatic|MID LAYER 8=Automatic|MID LAYER 9=Automatic|MID LAYER 10=Automatic|MID LAYER 11=Automatic|MID LAYER 12=Automatic|MID LAYER 13=Automatic|MID LAYER 14=Automatic|MID LAYER 15=Automatic|MID LAYER 16=Automatic|MID LAYER 17=Automatic|MID LAYER 18=Automatic|MID LAYER 19=Automatic|MID LAYER 20=Automatic|MID LAYER 21=Automatic|MID LAYER 22=Automatic|MID LAYER 23=Automatic|MID LAYER 24=Automatic|MID LAYER 25=Automatic|MID LAYER 26=Automatic|MID LAYER 27=Automatic|MID LAYER 28=Automatic|MID LAYER 29=Automatic|MID LAYER 30=Automatic|BOTTOM LAYER=Automatic";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=9|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=RoutingCorners|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=RoutingCorners|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=SJVPAJRS|DEFINEDBYLOGICALDOCUMENT=FALSE|CORNERSTYLE=45-Degree|MINSETBACK=100mil|MAXSETBACK=100mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=9|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=RoutingCorners|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=RoutingCorners|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|CORNERSTYLE=45-Degree|MINSETBACK=100mil|MAXSETBACK=100mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=10|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=RoutingVias|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=RoutingVias|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=CHQVHKRT|DEFINEDBYLOGICALDOCUMENT=FALSE|HOLEWIDTH=28mil|WIDTH=50mil|VIASTYLE=Through Hole|MINHOLEWIDTH=28mil|MINWIDTH=50mil|MAXHOLEWIDTH=28mil|MAXWIDTH=50mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=10|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=RoutingVias|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=RoutingVias|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|HOLEWIDTH=28mil|WIDTH=50mil|VIASTYLE=Through Hole|MINHOLEWIDTH=28mil|MINWIDTH=50mil|MAXHOLEWIDTH=28mil|MAXWIDTH=50mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=11|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=PasteMaskExpansion|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=PasteMaskExpansion|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=IIFQMSAF|DEFINEDBYLOGICALDOCUMENT=FALSE|EXPANSION=0mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=11|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=PasteMaskExpansion|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=PasteMaskExpansion|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|EXPANSION=0mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=12|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=SolderMaskExpansion|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=SolderMaskExpansion|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=CEPWRLMW|DEFINEDBYLOGICALDOCUMENT=FALSE|EXPANSION=4mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=12|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=SolderMaskExpansion|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=SolderMaskExpansion|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|EXPANSION=4mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=13|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=RoutingTopology|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=RoutingTopology|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=GUVVXYCH|DEFINEDBYLOGICALDOCUMENT=FALSE|TOPOLOGY=Shortest";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=13|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=RoutingTopology|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=RoutingTopology|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|TOPOLOGY=Shortest";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=14|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=TestPointUsage|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=TestPointUsage|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=IHPVLTXS|DEFINEDBYLOGICALDOCUMENT=FALSE|VALID=0|ALLOWMULTIPLE=FALSE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=14|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=TestPointUsage|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=TestPointUsage|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|VALID=0|ALLOWMULTIPLE=FALSE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=15|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=Testpoint|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=Testpoint|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=UWOHUQOL|DEFINEDBYLOGICALDOCUMENT=FALSE|SIDE=15|TESTPOINTUNDERCOMPONENT=TRUE|MINSIZE=40mil|MAXSIZE=100mil|PREFEREDSIZE=60mil|MINHOLESIZE=0mil|MAXHOLESIZE=40mil|PREFEREDHOLESIZE=32mil|TESTPOINTGRID=1mil|STYLE=1023|ORDER=0-112345678";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=15|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=Testpoint|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Testpoint|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|SIDE=15|TESTPOINTUNDERCOMPONENT=TRUE|MINSIZE=40mil|MAXSIZE=100mil|PREFEREDSIZE=60mil|MINHOLESIZE=0mil|MAXHOLESIZE=40mil|PREFEREDHOLESIZE=32mil|TESTPOINTGRID=1mil|STYLE=1023|ORDER=0-112345678";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=16|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=ComponentClearance|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=ComponentClearance|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=YDVHXXUW|DEFINEDBYLOGICALDOCUMENT=FALSE|GAP=10mil|COLLISIONCHECKMODE=3|VERTICALGAP=10mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=16|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=ComponentClearance|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=ComponentClearance|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|GAP=10mil|COLLISIONCHECKMODE=3";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=17|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=Height|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=Height|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=YAAEQLQW|DEFINEDBYLOGICALDOCUMENT=FALSE|MINHEIGHT=0mil|MAXHEIGHT=1000mil|PREFHEIGHT=500mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=17|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=Height|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Height|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=18|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=LayerPairs|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=LayerPairs|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=CDYWLAMA|DEFINEDBYLOGICALDOCUMENT=FALSE|ENFORCE=TRUE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=18|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=LayerPairs|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=LayerPairs|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|ENFORCE=TRUE";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=19|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=HoleSize|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=HoleSize|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=SHUWQOCD|DEFINEDBYLOGICALDOCUMENT=FALSE|ABSOLUTEVALUES=TRUE|MAXLIMIT=100mil|MINLIMIT=1mil|MAXPERCENT=80.000|MINPERCENT=20.000";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=19|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=HoleSize|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=HoleSize|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=|ABSOLUTEVALUES=TRUE|MAXLIMIT=100mil|MINLIMIT=1mil|MAXPERCENT=80.000|MINPERCENT=20.000";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=20|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=Fanout_Default|ENABLED=TRUE|PRIORITY=5|COMMENT=Fanout_Default (Default Rule)|UNIQUEID=FKGNUSDQ|DEFINEDBYLOGICALDOCUMENT=FALSE|BGADIR=Out|BGAVIAMODE=Centered|FANOUTSTYLE=Auto|FANOUTDIRECTION=Alternating|VIAGRID=1mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=20|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Fanout_Default|ENABLED=TRUE|COMMENTLENGTH=30|COMMENT=Fanout_Default (Default Rule)";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=21|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=(CompPinCount < 5)|SCOPE2EXPRESSION=All|NAME=Fanout_Small|ENABLED=TRUE|PRIORITY=4|COMMENT=Fanout_Small (Default Rule)|UNIQUEID=CGXYKESG|DEFINEDBYLOGICALDOCUMENT=FALSE|BGADIR=Out|BGAVIAMODE=Centered|FANOUTSTYLE=Auto|FANOUTDIRECTION=OutThenIn|VIAGRID=1mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=21|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Fanout_Small|ENABLED=FALSE|COMMENTLENGTH=84|COMMENT=Protel DXP Scope 1 Expression Was: (CompPinCount < 5)   Fanout_Small (Default Rule)";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=22|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=IsSOIC|SCOPE2EXPRESSION=All|NAME=Fanout_SOIC|ENABLED=TRUE|PRIORITY=3|COMMENT=Fanout_SOIC (Default Rule)|UNIQUEID=YVRABBWH|DEFINEDBYLOGICALDOCUMENT=FALSE|BGADIR=Out|BGAVIAMODE=Centered|FANOUTSTYLE=Auto|FANOUTDIRECTION=Alternating|VIAGRID=1mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=22|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Fanout_SOIC|ENABLED=FALSE|COMMENTLENGTH=71|COMMENT=Protel DXP Scope 1 Expression Was: IsSOIC   Fanout_SOIC (Default Rule)";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=23|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=IsLCC|SCOPE2EXPRESSION=All|NAME=Fanout_LCC|ENABLED=TRUE|PRIORITY=2|COMMENT=Fanout_LCC (Default Rule)|UNIQUEID=KBOWPUJQ|DEFINEDBYLOGICALDOCUMENT=FALSE|BGADIR=Out|BGAVIAMODE=Centered|FANOUTSTYLE=Auto|FANOUTDIRECTION=Alternating|VIAGRID=1mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=23|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Fanout_LCC|ENABLED=FALSE|COMMENTLENGTH=69|COMMENT=Protel DXP Scope 1 Expression Was: IsLCC   Fanout_LCC (Default Rule)";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=24|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=IsBGA|SCOPE2EXPRESSION=All|NAME=Fanout_BGA|ENABLED=TRUE|PRIORITY=1|COMMENT=Fanout_BGA (Default Rule)|UNIQUEID=BXKFDOAH|DEFINEDBYLOGICALDOCUMENT=FALSE|BGADIR=Out|BGAVIAMODE=Centered|FANOUTSTYLE=Auto|FANOUTDIRECTION=Alternating|VIAGRID=1mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=24|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=FanoutControl|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=Fanout_BGA|ENABLED=FALSE|COMMENTLENGTH=69|COMMENT=Protel DXP Scope 1 Expression Was: IsBGA   Fanout_BGA (Default Rule)";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=DXPRule|INDEXFORSAVE=25|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|RULEKIND=DiffPairsRouting|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1EXPRESSION=All|SCOPE2EXPRESSION=All|NAME=DiffPairsRouting|ENABLED=TRUE|PRIORITY=1|COMMENT=|UNIQUEID=KVRBRADC|DEFINEDBYLOGICALDOCUMENT=FALSE|MAXLIMIT=10mil|MINLIMIT=10mil|MOSTFREQGAP=10mil|MAXUNCOUPLEDLENGTH=500mil";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	
	onelinestr += "|RECORD=Rule|INDEXFORSAVE=25|SELECTION=FALSE|LAYER=TOP|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|RULEKIND=DiffPairsRouting|NETSCOPE=DifferentNets|LAYERKIND=SameLayer|SCOPE1COUNT=1|SCOPE1_0_KIND=Board|SCOPE1_0_VALUE=Board|SCOPE2COUNT=1|SCOPE2_0_KIND=Board|SCOPE2_0_VALUE=Board|NAME=DiffPairsRouting|ENABLED=TRUE|COMMENTLENGTH=0|COMMENT=";
	onelinestr += csLineEnd;
	ar.WriteString(onelinestr);
	onelinestr.Empty();	

	for(int i=0; i<m_arrayPcbDimension.GetCount(); i++)
	{
		COb_pcbdimension* pdimension = (COb_pcbdimension*)m_arrayPcbDimension.GetAt(i);
		if(pdimension->m_nFlag != FLAG_NORMAL) continue;
		if(pdimension->m_nDimensionKind != 1) continue;

		keyword = "|RECORD=Embedded";
		onelinestr += keyword;

		keyword = "|INDEXFORSAVE=0";

		onelinestr += keyword;

		onelinestr += "|SELECTION=FALSE|LAYER=MULTILAYER|LOCKED=FALSE|POLYGONOUTLINE=FALSE|USERROUTED=TRUE|UNIONINDEX=0|NAME=";
		onelinestr += "|SELECTION=FALSE";

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[pdimension->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(pdimension->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(pdimension->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|USERROUTED=";
		if(pdimension->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|UNIONINDEX=";
		itoa(pdimension->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|PRIMITIVELOCK=";
		if(pdimension->m_bPrimitiveLock == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|DIMENSIONLAYER=";
		keyword += csEDA_PCB_LayerLabel[pdimension->m_nLayer];
		onelinestr += keyword;

		keyword = "|DIMENSIONLOCKED=";
		if(pdimension->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		onelinestr += "|OBJECTID=13";
		
		keyword = "|DIMENSIONKIND=";
		itoa(pdimension->m_nDimensionKind, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|DRCERROR=FALSE";

		keyword = "|VINDEXFORSAVE=0";
		onelinestr += keyword;

		keyword = "|LX=";
		mil_to_str(pdimension->m_fLx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|LY=";
		mil_to_str(pdimension->m_fLy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|HX=";
		mil_to_str(pdimension->m_fHx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|HY=";
		mil_to_str(pdimension->m_fHy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|X1=";
		mil_to_str(pdimension->m_fXs, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|Y1=";
		mil_to_str(pdimension->m_fYs, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|X2=";
		mil_to_str(pdimension->m_fXe, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		keyword = "|Y2=";
		mil_to_str(pdimension->m_fYe, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTX=";
		mil_to_str(pdimension->m_fTextSx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;		
		keyword = "|TEXTY=";
		mil_to_str(pdimension->m_fTextSy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;					
		keyword = "|HEIGHT=";
		mil_to_str(pdimension->m_fHeight, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;					
		keyword = "|LINEWIDTH=";
		mil_to_str(pdimension->m_fLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTHEIGHT=";
		mil_to_str(pdimension->m_fTextHeight, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|TEXTWIDTH=";
		mil_to_str(pdimension->m_fTextLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|FONT=";
		keyword += csPCB_StrokeFontName[pdimension->m_nStrokeFont];
		onelinestr += keyword;
		
		keyword = "|STYLE=";
		if(pdimension->m_nUnitStyle == UNIT_STYLE_NORMAL) keyword += "Normal";
		else if(pdimension->m_nUnitStyle == UNIT_STYLE_BRACKETS) keyword += "Brackets";
		else keyword += "None";
		onelinestr += keyword;

		keyword = "|TEXTLINEWIDTH=";
		mil_to_str(pdimension->m_fTextLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|TEXTPOSITION=Auto";

		keyword = "|TEXTGAP=";
		mil_to_str(pdimension->m_fTextGap, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTFORMAT=";
		if(pdimension->m_nTextFormat == 2) keyword += "10mil";
		else if(pdimension->m_nTextFormat == 3) keyword += "10 (mil)";
		else keyword += "10";
		onelinestr += keyword;

		keyword = "|TEXTDIMENSIONUNIT=";
		if(pdimension->m_nTextDimensionUnit == UNIT_MIL) keyword += "Mils";
		else if(pdimension->m_nTextDimensionUnit == UNIT_MM) keyword += "Millimeters";
		else if(pdimension->m_nTextDimensionUnit == UNIT_INCH) keyword += "Inches";
		else if(pdimension->m_nTextDimensionUnit == UNIT_CM) keyword += "Centimeters";
		else keyword += "AutomaticUnit";
		onelinestr += keyword;
		
		keyword = "|TEXTPRECISION=";
		itoa(pdimension->m_nTextPrecision, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|TEXTPREFIX=";
		keyword += pdimension->m_csPrefix;
		onelinestr += keyword;
		
		keyword = "|TEXTSUFFIX=";
		keyword += pdimension->m_csSuffix;
		onelinestr += keyword;		
		
		keyword = "|ARROWSIZE=";
		mil_to_str(pdimension->m_fArrowSize, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;		

		keyword = "|ARROWLINEWIDTH=";
		mil_to_str(pdimension->m_fArrowLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|ARROWLENGTH=";
		mil_to_str(pdimension->m_fArrowLength, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|ARROWPOSITION=";
		if(pdimension->m_nArrowPosition == 0) keyword += "Inside";
		else keyword += "Outside";
		onelinestr += keyword;	

		keyword = "|EXTENSIONOFFSET=";
		mil_to_str(pdimension->m_fExtensionOffset, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	
		
		keyword = "|EXTENSIONLINEWIDTH=";
		mil_to_str(pdimension->m_fExtensionLineWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|EXTENSIONPICKGAP=";
		mil_to_str(pdimension->m_fExtensionPickGap, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	

		keyword = "|REFERENCES_COUNT=";
		itoa(pdimension->m_cRefPoint.GetCount(), str, 10);
		keyword += str;
		onelinestr += keyword;

		for(int j=0; j<pdimension->m_cRefPoint.GetCount(); j++)
		{
			Fpoint fpoint = pdimension->m_cRefPoint.GetAt(j);
			char numstr[100];
			itoa(j, numstr, 10);
			keyword = "|REFERENCE"; keyword += numstr; keyword += "PRIM=-1";
			keyword += "|REFERENCE"; keyword += numstr; keyword += "OBJECTID=-1";
			keyword += "|REFERENCE"; keyword += numstr; keyword += "OBJECTSTRING=NoObject";
			keyword += "|REFERENCE"; keyword += numstr; keyword += "POINTX=";
			mil_to_str(fpoint.fx, str, WITH_MIL, 4);
			keyword += str;
			keyword += "|REFERENCE"; keyword += numstr; keyword += "POINTY=";
			mil_to_str(fpoint.fy, str, WITH_MIL, 4);
			keyword += str;
			keyword += "|REFERENCE"; keyword += numstr; keyword += "ANCHOR=-1";
			onelinestr += keyword;
		}

		keyword = "|TEXT1X=";
		mil_to_str(pdimension->m_fTextSx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	
		
		keyword = "|TEXT1Y=";
		mil_to_str(pdimension->m_fTextSy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;	
		
		keyword = "|TEXT1ANGLE=";
		sprintf(str, "%.14E", pdimension->m_fTextRotation);
		keyword += str;
		onelinestr += keyword;	

		onelinestr += "|TEXT1MIRROR=FALSE";

		keyword = "|USETTFONTS=";
		if(pdimension->m_bUseTTFont == FALSE) keyword += "FALSE";
		else keyword += "TRUE";
		onelinestr += keyword;	
		
		keyword = "|BOLD=";
		if(pdimension->m_bBold == FALSE) keyword += "FALSE";
		else keyword += "TRUE";
		onelinestr += keyword;	

		keyword = "|ITALIC=";
		if(pdimension->m_bItalic == FALSE) keyword += "FALSE";
		else keyword += "TRUE";
		onelinestr += keyword;	

		keyword = "|FONTNAME=";
		keyword += pdimension->m_csTTFontName;
		onelinestr += keyword;	
		
		keyword = "|ANGLE=";
		sprintf(str, "%.14E", pdimension->m_fRotation);
		keyword += str;
		onelinestr += keyword;	

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}


	for(int i=0; i<m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = (COb_pcbarc*)m_arrayPcbArc.GetAt(i);
		if(parc->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Arc";
		onelinestr += keyword;

		if(parc->m_nComponent != -1)
		{
			//∏¸–¬m_nComponent
			int compbak = parc->m_nComponent;
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == parc->m_nComponent) 
				{
					parc->m_nComponent = point.y;
					break;
				}
			}

			keyword = "|COMPONENT=";
			itoa(parc->m_nComponent, str, 10);
			keyword += str;
			onelinestr += keyword;

			parc->m_nComponent = compbak;	
		}
		
		if(parc->m_nPolygon != -1)
		{
			int polybak = parc->m_nPolygon;
			for(int j=0; j<arrayPolygon.GetCount(); j++)
			{
				POINT point;
				point = arrayPolygon.GetAt(j);
				if(point.x == parc->m_nPolygon) 
				{
					parc->m_nPolygon = point.y;
					break;
				}
			}

			keyword = "|POLYGON=";
			itoa(parc->m_nPolygon, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			parc->m_nPolygon = polybak;	
		}
		
		if(parc->m_nNet != -1)
		{
			int netbak = parc->m_nNet;
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == parc->m_nNet) 
				{
					parc->m_nNet = point.y;
					break;
				}
			}

			keyword = "|NET=";
			itoa(parc->m_nNet, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			parc->m_nNet = netbak;	
		}

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[parc->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(parc->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(parc->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|TEARDROP=";
		if(parc->m_bTearDrop == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;			
		
		keyword = "|USERROUTED=FALSE";
		onelinestr += keyword;	

		keyword = "|KEEPOUT=";
		if(parc->m_bKeepout == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;	

		keyword = "|UNIONINDEX=";
		itoa(parc->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|LOCATION.X=";
		mil_to_str(parc->m_fCx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|LOCATION.Y=";
		mil_to_str(parc->m_fCy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|RADIUS=";
		mil_to_str(parc->m_fRadius, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|STARTANGLE= ";
		sprintf(str, "%.14E", parc->m_fSangle);
		keyword += str;
		onelinestr += keyword;

		keyword = "|ENDANGLE= ";
		sprintf(str, "%.14E", parc->m_fEangle);
		keyword += str;
		onelinestr += keyword;

		keyword = "|WIDTH=";
		mil_to_str(parc->m_fWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SUBPOLYINDEX=";
		itoa(parc->m_nSubPolyIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	for(int i=0; i<m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = (COb_pcbpad*)m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Pad";
		onelinestr += keyword;

		if(ppad->m_nComponent != -1)
		{
			int compbak = ppad->m_nComponent;
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == ppad->m_nComponent) 
				{
					ppad->m_nComponent = point.y;
					break;
				}
			}

			keyword = "|COMPONENT=";
			itoa(ppad->m_nComponent, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			ppad->m_nComponent = compbak;	
		}
		
		if(ppad->m_nNet != -1)
		{
			int netbak = ppad->m_nNet;
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == ppad->m_nNet) 
				{
					ppad->m_nNet = point.y;
					break;
				}
			}

			keyword = "|NET=";
			itoa(ppad->m_nNet, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			ppad->m_nNet = netbak;
		}

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[ppad->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(ppad->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(ppad->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|USERROUTED=";
		if(ppad->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		if(ppad->m_bTenting == TRUE) onelinestr += "|TENTING=TRUE";

		keyword = "|UNIONINDEX=";
		itoa(ppad->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|NAME=";
		keyword += ppad->m_csName;
		onelinestr += keyword;

		keyword = "|X=";
		mil_to_str(ppad->m_fCx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y=";
		mil_to_str(ppad->m_fCy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
			
		if( (ppad->m_fSizeX[TOPLAYER] == ppad->m_fSizeX[BOTTOMLAYER])&&(ppad->m_fSizeX[TOPLAYER] == ppad->m_fSizeX[MIDLAYER1])&&\
			(ppad->m_fSizeY[TOPLAYER] == ppad->m_fSizeY[BOTTOMLAYER])&&(ppad->m_fSizeY[TOPLAYER] == ppad->m_fSizeY[MIDLAYER1]))
		{
			keyword = "|XSIZE=";
			mil_to_str(ppad->m_fSizeX[0], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|YSIZE=";
			mil_to_str(ppad->m_fSizeY[0], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
		}
		else
		{
			keyword = "|TOPXSIZE=";
			mil_to_str(ppad->m_fSizeX[TOPLAYER], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			keyword = "|MIDXSIZE=";
			mil_to_str(ppad->m_fSizeX[MIDLAYER1], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			keyword = "|BOTXSIZE=";
			mil_to_str(ppad->m_fSizeX[BOTTOMLAYER], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|TOPYSIZE=";
			mil_to_str(ppad->m_fSizeY[TOPLAYER], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			keyword = "|MIDYSIZE=";
			mil_to_str(ppad->m_fSizeY[MIDLAYER1], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			keyword = "|BOTYSIZE=";
			mil_to_str(ppad->m_fSizeY[BOTTOMLAYER], str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
		}

		if((ppad->m_nShape[TOPLAYER] == ppad->m_nShape[BOTTOMLAYER])&&(ppad->m_nShape[TOPLAYER] == ppad->m_nShape[MIDLAYER1]))
		{
			keyword = "|SHAPE=";
			keyword += csPCB_PadShape[ppad->m_nShape[TOPLAYER]];
			onelinestr += keyword;
		}
		else
		{
			keyword = "|TOPSHAPE=";
			keyword += csPCB_PadShape[ppad->m_nShape[TOPLAYER]];
			onelinestr += keyword;
			keyword = "|MIDSHAPE=";
			keyword += csPCB_PadShape[ppad->m_nShape[MIDLAYER1]];
			onelinestr += keyword;
			keyword = "|BOTSHAPE=";
			keyword += csPCB_PadShape[ppad->m_nShape[BOTTOMLAYER]];
			onelinestr += keyword;
		}

		keyword = "|HOLESIZE=";
		mil_to_str(ppad->m_fHoleDia, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|ROTATION=";
		sprintf(str, "%.14E", ppad->m_fRotation);
		keyword += str;
		onelinestr += keyword;

		keyword = "|PLATED=";
		if(ppad->m_bPlated == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|DAISYCHAIN=";
		keyword += csPCB_PadElecTpye[ppad->m_nDaisychain];
		onelinestr += keyword;

		keyword = "|CCSV=";
		itoa(ppad->m_nCCSV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPLV=";
		itoa(ppad->m_nCPLV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CCWV=";
		itoa(ppad->m_nCCWV, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|CENV=";
		itoa(ppad->m_nCENV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CAGV=";
		itoa(ppad->m_nCAGV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPEV=";
		itoa(ppad->m_nCPEV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CSEV=";
		itoa(ppad->m_nCSEV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPCV=";
		itoa(ppad->m_nCPCV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPRV=";
		itoa(ppad->m_nCPRV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CCW=";
		mil_to_str(ppad->m_fCCW, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|CEN=";
		itoa(ppad->m_nCEN, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CAG=";
		mil_to_str(ppad->m_fCAG, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPE=";
		mil_to_str(ppad->m_fCPE, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CSE=";
		mil_to_str(ppad->m_fCSE, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPC=";
		mil_to_str(ppad->m_fCPC, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPR=";
		mil_to_str(ppad->m_fCPR, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|PADMODE=";
		itoa(ppad->m_nPadMode, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|SWAPID_PAD=|SWAPID_GATE=|&|0|SWAPPEDPADNAME=|GATEID=0";
		
		if(ppad->m_bTentingTop == TRUE) onelinestr += "|TENTINGTOP=TRUE";
		if(ppad->m_bTentingBottom == TRUE) onelinestr += "|TENTINGBOTTOM=TRUE";			
			
		onelinestr += "|OVERRIDEWITHV6_6SHAPES=FALSE|DRILLTYPE=0";

		keyword = "|HOLETYPE=";
		itoa(ppad->m_nHoleType, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|HOLEWIDTH=";
		mil_to_str(ppad->m_fHoleWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|HOLEROTATION=";
		sprintf(str, "%.14E", ppad->m_fHoleRotation);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|PADXOFFSET0=0mil|PADYOFFSET0=0mil|PADXOFFSET1=0mil|PADYOFFSET1=0mil|PADXOFFSET2=0mil|PADYOFFSET2=0mil|PADXOFFSET3=0mil|PADYOFFSET3=0mil|PADXOFFSET4=0mil";
		onelinestr += "|PADYOFFSET4=0mil|PADXOFFSET5=0mil|PADYOFFSET5=0mil|PADXOFFSET6=0mil|PADYOFFSET6=0mil|PADXOFFSET7=0mil|PADYOFFSET7=0mil|PADXOFFSET8=0mil|PADYOFFSET8=0mil";
		onelinestr += "|PADXOFFSET9=0mil|PADYOFFSET9=0mil|PADXOFFSET10=0mil|PADYOFFSET10=0mil|PADXOFFSET11=0mil|PADYOFFSET11=0mil|PADXOFFSET12=0mil|PADYOFFSET12=0mil|PADXOFFSET13=0mil";
		onelinestr += "|PADYOFFSET13=0mil|PADXOFFSET14=0mil|PADYOFFSET14=0mil|PADXOFFSET15=0mil|PADYOFFSET15=0mil|PADXOFFSET16=0mil|PADYOFFSET16=0mil|PADXOFFSET17=0mil|PADYOFFSET17=0mil";
		onelinestr += "|PADXOFFSET18=0mil|PADYOFFSET18=0mil|PADXOFFSET19=0mil|PADYOFFSET19=0mil|PADXOFFSET20=0mil|PADYOFFSET20=0mil|PADXOFFSET21=0mil|PADYOFFSET21=0mil|PADXOFFSET22=0mil";
		onelinestr += "|PADYOFFSET22=0mil|PADXOFFSET23=0mil|PADYOFFSET23=0mil|PADXOFFSET24=0mil|PADYOFFSET24=0mil|PADXOFFSET25=0mil|PADYOFFSET25=0mil|PADXOFFSET26=0mil|PADYOFFSET26=0mil";
		onelinestr += "|PADXOFFSET27=0mil|PADYOFFSET27=0mil|PADXOFFSET28=0mil|PADYOFFSET28=0mil|PADXOFFSET29=0mil|PADYOFFSET29=0mil|PADXOFFSET30=0mil|PADYOFFSET30=0mil|PADXOFFSET31=0mil|PADYOFFSET31=0mil|PADJUMPERID=0";

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	for(int i=0; i<m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = (COb_pcbvia*)m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Via";
		onelinestr += keyword;

		if(pvia->m_nComponent != -1)
		{
			int compbak = pvia->m_nComponent;
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == pvia->m_nComponent) 
				{
					pvia->m_nComponent = point.y;
					break;
				}
			}

			keyword = "|COMPONENT=";
			itoa(pvia->m_nComponent, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			pvia->m_nComponent = compbak;	
		}
		
		if(pvia->m_nNet != -1)
		{
			int netbak = pvia->m_nNet;
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == pvia->m_nNet) 
				{
					pvia->m_nNet = point.y;
					break;
				}
			}

			keyword = "|NET=";
			itoa(pvia->m_nNet, str, 10);
			keyword += str;
			onelinestr += keyword;

			pvia->m_nNet = netbak;
		}

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[pvia->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(pvia->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(pvia->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|USERROUTED=";
		if(pvia->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		if(pvia->m_bTenting == TRUE) onelinestr += "|TENTING=TRUE";

		keyword = "|UNIONINDEX=";
		itoa(pvia->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|X=";
		mil_to_str(pvia->m_fCx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y=";
		mil_to_str(pvia->m_fCy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|DIAMETER=";
		mil_to_str(pvia->m_fDia, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|HOLESIZE=";
		mil_to_str(pvia->m_fHoleDia, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|STARTLAYER=";
		keyword += csEDA_PCB_LayerLabel[pvia->m_nStartLayer];
		onelinestr += keyword;

		keyword = "|ENDLAYER=";
		keyword += csEDA_PCB_LayerLabel[pvia->m_nEndLayer];
		onelinestr += keyword;

		keyword = "|CCSV=";
		itoa(pvia->m_nCCSV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPLV=";
		itoa(pvia->m_nCPLV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CCWV=";
		itoa(pvia->m_nCCWV, str, 10);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|CENV=";
		itoa(pvia->m_nCENV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CAGV=";
		itoa(pvia->m_nCAGV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPEV=";
		itoa(pvia->m_nCAGV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CSEV=";
		itoa(pvia->m_nCSEV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPCV=";
		itoa(pvia->m_nCPCV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPRV=";
		itoa(pvia->m_nCPRV, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CCW=";
		mil_to_str(pvia->m_fCCW, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|CEN=";
		itoa(pvia->m_nCEN, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CAG=";
		mil_to_str(pvia->m_fCAG, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CSE=";
		mil_to_str(pvia->m_fCSE, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPC=";
		mil_to_str(pvia->m_fCPC, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|CPR=";
		mil_to_str(pvia->m_fCPR, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		if(pvia->m_bTentingTop == TRUE) onelinestr += "|TENTINGTOP=TRUE";
		if(pvia->m_bTentingBottom == TRUE) onelinestr += "|TENTINGBOTTOM=TRUE";

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	
	for(int i=0; i<m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = (COb_pcbtrack*)m_arrayPcbTrack.GetAt(i);
		if(ptrack->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Track";
		onelinestr += keyword;

		if(ptrack->m_nComponent != -1)
		{
			int compbak = ptrack->m_nComponent;
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == ptrack->m_nComponent) 
				{
					ptrack->m_nComponent = point.y;
					break;
				}
			}

			keyword = "|COMPONENT=";
			itoa(ptrack->m_nComponent, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			ptrack->m_nComponent = compbak;	
		}
		
		if(ptrack->m_nPolygon != -1)
		{
			int polybak = ptrack->m_nPolygon;
			for(int j=0; j<arrayPolygon.GetCount(); j++)
			{
				POINT point;
				point = arrayPolygon.GetAt(j);
				if(point.x == ptrack->m_nPolygon) 
				{
					ptrack->m_nPolygon = point.y;
					break;
				}
			}

			keyword = "|POLYGON=";
			itoa(ptrack->m_nPolygon, str, 10);
			keyword += str;
			onelinestr += keyword;

			ptrack->m_nPolygon = polybak;	
		}
		
		if(ptrack->m_nNet != -1)
		{
			int netbak = ptrack->m_nNet;
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == ptrack->m_nNet) 
				{
					ptrack->m_nNet = point.y;
					break;
				}
			}

			keyword = "|NET=";
			itoa(ptrack->m_nNet, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			ptrack->m_nNet = netbak;
		}

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[ptrack->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(ptrack->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(ptrack->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|TEARDROP=";
		if(ptrack->m_bTearDrop == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;	

        keyword = "|USERROUTED=FALSE";
		onelinestr += keyword;	

		keyword = "|KEEPOUT=";
		if(ptrack->m_bKeepout == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;	

		keyword = "|UNIONINDEX=";
		itoa(ptrack->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|X1=";
		mil_to_str(ptrack->m_fX1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y1=";
		mil_to_str(ptrack->m_fY1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|X2=";
		mil_to_str(ptrack->m_fX2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y2=";
		mil_to_str(ptrack->m_fY2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;		
		
		keyword = "|WIDTH=";
		mil_to_str(ptrack->m_fWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SUBPOLYINDEX=";
		itoa(ptrack->m_nSubPolyIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}


	for(int i=0; i<m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = (COb_pcbtext*)m_arrayPcbText.GetAt(i);
		if(ptext->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Text";
		onelinestr += keyword;

		if(ptext->m_nComponent != -1)
		{
			int compbak = ptext->m_nComponent;
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == ptext->m_nComponent) 
				{
					ptext->m_nComponent = point.y;
					break;
				}
			}

			keyword = "|COMPONENT=";
			itoa(ptext->m_nComponent, str, 10);
			keyword += str;
			onelinestr += keyword;
			
			ptext->m_nComponent = compbak;	
		}
		
		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[ptext->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(ptext->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(ptext->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|USERROUTED=";
		if(ptext->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|UNIONINDEX=";
		itoa(ptext->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|X1=";
		mil_to_str(ptext->m_fX1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y1=";
		mil_to_str(ptext->m_fY1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|X2=";
		mil_to_str(ptext->m_fX2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y2=";
		mil_to_str(ptext->m_fY2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|ROTATION= ";
		sprintf(str, "%.14E", ptext->m_fRotation);
		keyword += str;
		onelinestr += keyword;

		keyword = "|X=";
		mil_to_str(ptext->m_fSx, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|Y=";
		mil_to_str(ptext->m_fSy, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|HEIGHT=";
		mil_to_str(ptext->m_fHeight, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		keyword = "|FONT=";
		keyword += csPCB_StrokeFontName[ptext->m_nStrokeFont];
		onelinestr += keyword;
		
		keyword = "|ROTATION= ";
		sprintf(str, "%.14E", ptext->m_fRotation);
		keyword += str;
		onelinestr += keyword;

		keyword = "|MIRROR=";
		if(ptext->m_bMirror == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|TEXT=";
		keyword += ptext->m_csText;
		onelinestr += keyword;
		
		keyword = "|WIDTH=";
		mil_to_str(ptext->m_fWidth, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|USETTFONTS=";
		if(ptext->m_bUseTTFont == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;		
		
		keyword = "|BOLD=";
		if(ptext->m_bBold == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;				
		
		keyword = "|ITALIC=";
		if(ptext->m_bItalic == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;				
		
		keyword = "|INVERTED=";
		if(ptext->m_bInverted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;	

		keyword = "|FONTNAME=";
		keyword += ptext->m_csTTFontName;
		onelinestr += keyword;
		
		keyword = "|INVERTEDTTTEXTBORDER=";
		mil_to_str(ptext->m_fInvertedBorder, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;

		if(ptext->m_bDesignator == TRUE) onelinestr += "|DESIGNATOR=True";

		if(ptext->m_bComment == TRUE) onelinestr += "|COMMENT=True";
		
		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	for(int i=0; i<m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = (COb_pcbfill*)m_arrayPcbFill.GetAt(i);
		if(pfill->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Fill";
		onelinestr += keyword;

		if(pfill->m_nComponent != -1)
		{
			int compbak = pfill->m_nComponent;
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == pfill->m_nComponent) 
				{
					pfill->m_nComponent = point.y;
					break;
				}
			}

			keyword = "|COMPONENT=";
			itoa(pfill->m_nComponent, str, 10);
			keyword += str;
			onelinestr += keyword;

			pfill->m_nComponent = compbak;	
		}
		
		if(pfill->m_nNet != -1)
		{
			int netbak = pfill->m_nNet;
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == pfill->m_nNet) 
				{
					pfill->m_nNet = point.y;
					break;
				}
			}

			keyword = "|NET=";
			itoa(pfill->m_nNet, str, 10);
			keyword += str;
			onelinestr += keyword;

			pfill->m_nNet = netbak;
		}

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[pfill->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(pfill->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(pfill->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|USERROUTED=";
		if(pfill->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|UNIONINDEX=";
		itoa(pfill->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;
	
		keyword = "|X1=";
		mil_to_str(pfill->m_fX1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
			
		keyword = "|Y1=";
		mil_to_str(pfill->m_fY1, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
			
		keyword = "|X2=";
		mil_to_str(pfill->m_fX2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
			
		keyword = "|Y2=";
		mil_to_str(pfill->m_fY2, str, WITH_MIL, 4);
		keyword += str;
		onelinestr += keyword;
		
		keyword = "|ROTATION= ";
		sprintf(str, "%.14E", pfill->m_fRotation);
		keyword += str;
		onelinestr += keyword;

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}

	for(int i=0; i<m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = (COb_pcbregion*)m_arrayPcbRegion.GetAt(i);
		if(pregion->m_nFlag != FLAG_NORMAL) continue;

		keyword = "|RECORD=Region";
		onelinestr += keyword;

		if(pregion->m_nComponent != -1)
		{
			int compbak = pregion->m_nComponent;
			for(int j=0; j<arrayComp.GetCount(); j++)
			{
				POINT point;
				point = arrayComp.GetAt(j);
				if(point.x == pregion->m_nComponent) 
				{
					pregion->m_nComponent = point.y;
					break;
				}
			}

			keyword = "|COMPONENT=";
			itoa(pregion->m_nComponent, str, 10);
			keyword += str;
			onelinestr += keyword;

			pregion->m_nComponent = compbak;	
		}
		
		if(pregion->m_nPolygon != -1)
		{
			int polybak = pregion->m_nPolygon;
			for(int j=0; j<arrayPolygon.GetCount(); j++)
			{
				POINT point;
				point = arrayPolygon.GetAt(j);
				if(point.x == pregion->m_nPolygon) 
				{
					pregion->m_nPolygon = point.y;
					break;
				}
			}

			keyword = "|POLYGON=";
			itoa(pregion->m_nPolygon, str, 10);
			keyword += str;
			onelinestr += keyword;

			pregion->m_nPolygon = polybak;	
		}
		
		if(pregion->m_nNet != -1)
		{
			int netbak = pregion->m_nNet;
			for(int j=0; j<arrayNet.GetCount(); j++)
			{
				POINT point;
				point = arrayNet.GetAt(j);
				if(point.x == pregion->m_nNet) 
				{
					pregion->m_nNet = point.y;
					break;
				}
			}

			keyword = "|NET=";
			itoa(pregion->m_nNet, str, 10);
			keyword += str;
			onelinestr += keyword;

			pregion->m_nNet = netbak;
		}

		keyword = "|INDEXFORSAVE=";
		itoa(i, str, 10);
		keyword += str;
		onelinestr += keyword;

		keyword = "|SELECTION=FALSE";
		onelinestr += keyword;

		keyword = "|LAYER=";
		keyword += csEDA_PCB_LayerLabel[pregion->m_nLayer];
		onelinestr += keyword;
		
		keyword = "|LOCKED=";
		if(pregion->m_bLocked == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|POLYGONOUTLINE=";
		if(pregion->m_bPolygonOutline == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;
		
		keyword = "|USERROUTED=";
		if(pregion->m_bUserRouted == TRUE) keyword += "TRUE";
		else keyword += "FALSE";
		onelinestr += keyword;

		keyword = "|UNIONINDEX=";
		itoa(pregion->m_nUnionIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|NAME=";

		keyword = "|KIND=";
		if(pregion->m_bPolygonCutout == TRUE) keyword += "1";
		else keyword += "0";
		onelinestr += keyword;
		
		keyword = "|SUBPOLYINDEX=";
		itoa(pregion->m_nSubPolyIndex, str, 10);
		keyword += str;
		onelinestr += keyword;

		onelinestr += "|ARCRESOLUTION=0.5mil|ISSHAPEBASED=TRUE";

		keyword = "|MAINCONTOURVERTEXCOUNT=";
		itoa(pregion->m_cVertex.GetCount(), str, 10);
		keyword += str;
		onelinestr += keyword;
		
		for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
		{
			Struct_RegionVertex vertex;
			vertex = pregion->m_cVertex.GetAt(i);
			char numstr[100];
			itoa(i, numstr, 10);  strcat(numstr, "=");

			keyword = "|KIND";	keyword += numstr;
			if(vertex.radius != 0) keyword += "1";
			else keyword += "0";
			onelinestr += keyword;

			keyword = "|VX";	keyword += numstr;
			mil_to_str(vertex.fx, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|VY";	keyword += numstr;
			mil_to_str(vertex.fy, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|CX";	keyword += numstr;
			mil_to_str(vertex.cx, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|CY";	keyword += numstr;
			mil_to_str(vertex.cy, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|SA";	keyword += numstr;	keyword += " ";
			sprintf(str, "%.14E", vertex.sangle);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|EA";	keyword += numstr;	keyword += " ";
			sprintf(str, "%.14E", vertex.eangle);
			keyword += str;
			onelinestr += keyword;
			
			keyword = "|R";	keyword += numstr;
			mil_to_str(vertex.radius, str, WITH_MIL, 4);
			keyword += str;
			onelinestr += keyword;
		}
		
		//|HOLECOUNT=0
		keyword = "|HOLECOUNT=";
		itoa(pregion->m_arrayHoles.GetCount(), str, 10);
		keyword += str;
		onelinestr += keyword;

		for(int i=0; i<pregion->m_arrayHoles.GetCount(); i++)
		{
			CArray<Fpoint, Fpoint>* p_arrayHole = pregion->m_arrayHoles.GetAt(i);
			char numstr[100];
			itoa(i, numstr, 10);

			keyword = "|HOLE";		keyword += numstr;
			keyword += "VERTEXCOUNT=";
			itoa(p_arrayHole->GetCount(), str, 10);
			keyword += str;
			onelinestr += keyword;

			for(int j=0; j<p_arrayHole->GetCount(); j++)
			{
				Fpoint fp = p_arrayHole->GetAt(j);

				char sub_numstr[100];
				itoa(j, sub_numstr, 10);  strcat(sub_numstr, "=");				

				keyword = "|H";		keyword += numstr;
				keyword += "VX";	keyword += sub_numstr;
				sprintf(str, "%.6f", fp.fx*10000);
				keyword += str;
				onelinestr += keyword;
				
				keyword = "|H";		keyword += numstr;
				keyword += "VY";	keyword += sub_numstr;
				sprintf(str, "%.6f", fp.fy*10000);
				keyword += str;
				onelinestr += keyword;
			}
		}

		onelinestr += csLineEnd;
		ar.WriteString(onelinestr);
		onelinestr.Empty();	
	}
}

