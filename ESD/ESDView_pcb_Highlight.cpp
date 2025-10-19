void CESDView::OnPcbToolHighlightLocation()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);
	unsigned ch1;
	char szMoveText[] = "ÏÔÊ¾ÍøÂç¸ßÁÁ: <×ó¼ü><»Ø³µ>Ñ¡ÔñÍøÂç, <ÓÒ¼ü><ESC>½áÊøÃüÁî";
	MoveCursorToCenter();

	Write_Help(szMoveText);
	do{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			Struct_Pcb_Multi_Selection selected_element = SelectPcbObject(m_fCurrentX, m_fCurrentY, PCB_HIGHLIGHT_SELECTION);

			
			DrawPcbHighlight(&dc, 0);
			m_arrayPcbHighlight.RemoveAll();
			if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
				if(pDoc->IsOnSignalLayer(ptrack->m_nLayer) == TRUE)
				{
					ptrack->m_nFlag = FLAG_PROCESSING;
					m_arrayPcbHighlight.Add(selected_element);
					PCBMakeHighlight(TRUE);
					PcbProcessingSelection(FALSE);
				}
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
				if(pDoc->IsOnSignalLayer(parc->m_nLayer) == TRUE)
				{
					parc->m_nFlag = FLAG_PROCESSING;
					m_arrayPcbHighlight.Add(selected_element);
					PCBMakeHighlight(TRUE);
					PcbProcessingSelection(FALSE);
				}			
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
				if(pDoc->IsOnSignalLayer(ppad->m_nLayer) == TRUE)
				{
					ppad->m_nFlag = FLAG_PROCESSING;
					m_arrayPcbHighlight.Add(selected_element);
					PCBMakeHighlight(TRUE);
					PcbProcessingSelection(FALSE);
				}				
			}	
			else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
				pvia->m_nFlag = FLAG_PROCESSING;
				m_arrayPcbHighlight.Add(selected_element);
				PCBMakeHighlight(TRUE);
				PcbProcessingSelection(FALSE);
			}		
			else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
				if(pDoc->IsOnSignalLayer(pfill->m_nLayer) == TRUE)
				{
					pfill->m_nFlag = FLAG_PROCESSING;
					m_arrayPcbHighlight.Add(selected_element);
					PCBMakeHighlight(TRUE);
					PcbProcessingSelection(FALSE);
				}
			}	
			else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
			{
				COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
				if(pDoc->IsOnSignalLayer(pregion->m_nLayer) == TRUE)
				{
					pregion->m_nFlag = FLAG_PROCESSING;
					m_arrayPcbHighlight.Add(selected_element);
					PCBMakeHighlight(TRUE);
					PcbProcessingSelection(FALSE);
				}				
			}	
			//else if(selected_element.pcbelement == PCB_ELEMENT_POLYGON)
			//{
			//	COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(selected_element.index);
			//	if(pDoc->IsOnSignalLayer(ppolygon->m_nLayer) == TRUE)
			//	{
			//		m_arrayPcbHighlight.Add(selected_element);
			//		PCBMakeHighlight();
			//	}				
			//}
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;

	Invalidate();
}

void CESDView::OnPcbToolHighlightNet()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	if(pDoc->m_arrayPcbNet.GetCount() <= 0) 
	{
		AfxMessageBox("Î´ÕÒµ½PCBÍøÂçÐÅÏ¢, ÃüÁîÖÕÖ¹.");
		return;
	}

	CDlg_PcbNetName dlg;

	for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;

		dlg.arrayString.Add(pnet->m_csNetName);
	}

	if(dlg.DoModal() == IDOK)
	{
		for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
		{
			COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(i);
			if(pnet->m_nFlag != FLAG_NORMAL) continue;

			int iNet = i;
			if(dlg.m_csComboNet == pnet->m_csNetName)
			{
				
				DrawPcbHighlight(&dc, 0);
				m_arrayPcbHighlight.RemoveAll();

				Struct_Pcb_Multi_Selection selected_element;
				for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
				{
					COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
					if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nNet != iNet)||(ptrack->m_nComponent >= 0)||(ptrack->m_nPolygon >= 0)) continue;

					if(pDoc->IsOnSignalLayer(ptrack->m_nLayer) == TRUE)
					{
						selected_element.pcbelement = PCB_ELEMENT_TRACK;
						selected_element.index = j;
						ptrack->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(selected_element);
					}
				}
				for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
				{
					COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
					if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nNet != iNet)||(parc->m_nComponent >= 0)||(parc->m_nPolygon >= 0)) continue;
					
					if(pDoc->IsOnSignalLayer(parc->m_nLayer) == TRUE)
					{
						selected_element.pcbelement = PCB_ELEMENT_ARC;
						selected_element.index = j;
						parc->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(selected_element);
					}
				}
				for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
				{
					COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
					if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nNet != iNet)||(pfill->m_nComponent >= 0)) continue;

					if(pDoc->IsOnSignalLayer(pfill->m_nLayer) == TRUE)
					{
						selected_element.pcbelement = PCB_ELEMENT_FILL;
						selected_element.index = j;
						pfill->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(selected_element);
					}					
				}
				for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
					if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nNet != iNet)) continue;
					if(pDoc->IsOnSignalLayer(ppad->m_nLayer) == TRUE)
					{
						selected_element.pcbelement = PCB_ELEMENT_PAD;
						selected_element.index = j;
						ppad->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(selected_element);
					}					
				}
				for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
				{
					COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
					if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nNet != iNet)||(pvia->m_nComponent >= 0)) continue;
						
					selected_element.pcbelement = PCB_ELEMENT_VIA;
					selected_element.index = j;					
					pvia->m_nFlag = FLAG_PROCESSING;
					m_arrayPcbHighlight.Add(selected_element);
				}
				for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
				{
					COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
					if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nNet != iNet)||(pregion->m_nComponent >= 0)||(pregion->m_nPolygon >= 0)) continue;
					if(pDoc->IsOnSignalLayer(pregion->m_nLayer) == TRUE)
					{
						selected_element.pcbelement = PCB_ELEMENT_REGION;
						selected_element.index = j;		
						pregion->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(selected_element);
					}					
				}
				PCBMakeHighlight(TRUE);
				PcbProcessingSelection(FALSE);

				Invalidate();
				break;
			}
		}
	}
}

void CESDView::OnPcbToolHighlightPins()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	CStringArray arrayCompID;
	CStringArray arrayPinNumber;
	CString csNet;
	csNet.Empty();
	
	for(int i=0; i<m_arrayPcbHighlight.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selected_element = m_arrayPcbHighlight.GetAt(i);
		if(selected_element.pcbelement == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
			if(ppad->m_nFlag == FLAG_NORMAL)
			{
				if(ppad->m_nComponent >= 0)
				{
					COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
					CString cstring = pcomp->GetPcbCompDesignator(ppad->m_nComponent);
					arrayPinNumber.Add(ppad->m_csName);
					arrayCompID.Add(cstring);
					if((ppad->m_nNet >= 0)&&(csNet.IsEmpty() == TRUE))
					{
						COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(ppad->m_nNet);
						csNet = "ÍøÂç: " + pnet->m_csNetName;
					}
				}
			}
		}
	}

	if(arrayPinNumber.GetCount() <= 0) AfxMessageBox("¸ßÁÁÍøÂçÖÐÎ´ÕÒµ½Ôª¼þÒý½Å.");
	else
	{
        CDlg_PcbHighlightPins dlg;
		dlg.csNet = csNet;
		for(int i=0; i<arrayCompID.GetCount(); i++)
		{
			CString cstring;
			cstring = arrayCompID.GetAt(i);
			dlg.compID.Add(cstring);
			cstring = arrayPinNumber.GetAt(i);
			dlg.pinNum.Add(cstring);
		}

		dlg.DoModal();
	}
}

void CESDView::OnUpdatePcbToolHighlightPins(CCmdUI *pCmdUI)
{
	if(m_arrayPcbHighlight.GetCount() > 0) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CESDView::OnPcbToolHighlightClear()
{
	m_arrayPcbHighlight.RemoveAll();
	m_bPcbViewHighlightFlag = FALSE;
	Invalidate();
}

void CESDView::OnUpdatePcbToolHighlightClear(CCmdUI *pCmdUI)
{
	if(m_arrayPcbHighlight.GetCount() > 0) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CESDView::OnPcbViewHighlight()
{
	if(m_bPcbViewHighlightFlag == FALSE) m_bPcbViewHighlightFlag = TRUE;
	else m_bPcbViewHighlightFlag = FALSE;

	Invalidate();
}

void CESDView::OnUpdatePcbViewHighlight(CCmdUI *pCmdUI)
{
	if(m_arrayPcbHighlight.GetCount() > 0)
	{
		pCmdUI->Enable(TRUE);
		if(m_bPcbViewHighlightFlag == TRUE) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->Enable(FALSE);
		m_bPcbViewHighlightFlag = FALSE;
	}
}

void CESDView::DrawPcbHighlight(CDC* pDC, int mode)
{
	
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<m_arrayPcbHighlight.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selected_element = m_arrayPcbHighlight.GetAt(i);
		if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
			if(ptrack->m_nFlag == FLAG_NORMAL)
			{
				if(mode == 0) ptrack->Draw(pDC, this);
				else if(mode == 1) ptrack->DrawHighlighted(pDC, this, CONNECTIONS);
			}
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
			if(parc->m_nFlag == FLAG_NORMAL)
			{
				if(mode == 0) parc->Draw(pDC, this);
				else if(mode == 1) parc->DrawHighlighted(pDC, this, CONNECTIONS);
			}				
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
			if(pfill->m_nFlag == FLAG_NORMAL)
			{
				if(mode == 0) pfill->Draw(pDC, this);
				else if(mode == 1) pfill->DrawHighlighted(pDC, this, CONNECTIONS);
			}					
		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
			if(pregion->m_nFlag == FLAG_NORMAL)
			{
				if(mode == 0) pregion->Draw(pDC, this);
				else if(mode == 1) pregion->DrawHighlighted(pDC, this, CONNECTIONS);
			}					
		}	
	}

	for(int i=0; i<m_arrayPcbHighlight.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selected_element = m_arrayPcbHighlight.GetAt(i);
		if(selected_element.pcbelement == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
			if(ppad->m_nFlag == FLAG_NORMAL)
			{
				if(mode == 0)
				{
					ppad->Draw(pDC, this, DRAW_NORMAL);
					//if(ppad->m_nLayer != MULTILAYER) ppad->DrawSingleLayerPad(pDC, this, 0);
					//else ppad->DrawMultiLayerPad(pDC, this, 0);
				}
				else if(mode == 1) ppad->DrawHighlighted(pDC, this, CONNECTIONS);
			}
		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
			if(pvia->m_nFlag == FLAG_NORMAL)
			{
				if(mode == 0) pvia->Draw(pDC, this, 0);
				else if(mode == 1) pvia->DrawHighlighted(pDC, this, CONNECTIONS);
			}
		}		
	}
}

void CESDView::PCBMakeHighlight(BOOL bShowHighlight)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	for(int i=0; i<m_arrayPcbHighlight.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selected_element = m_arrayPcbHighlight.GetAt(i);
		if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
			PCBHighlight_Line_Search(ptrack, bShowHighlight);

			if(bEDA_PcbPolygonHighlight == TRUE)
			{
				CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&> arrayPolygonHighlight;
				PCBHighlightPolygon_Line_Search(ptrack, arrayPolygonHighlight, bShowHighlight);
				int num = arrayPolygonHighlight.GetCount();
				if(num > 0)
				{
					for(int j=i+1; j<m_arrayPcbHighlight.GetCount(); j++)
					{
						selected_element = m_arrayPcbHighlight.GetAt(j);
						arrayPolygonHighlight.Add(selected_element);
					}
					for(int j=i+1; j<m_arrayPcbHighlight.GetCount(); j++)
					{
						m_arrayPcbHighlight.RemoveAt(j);
						j--;
					}
					for(int j=0; j<arrayPolygonHighlight.GetCount(); j++)
					{
						selected_element = arrayPolygonHighlight.GetAt(j);
						m_arrayPcbHighlight.Add(selected_element);
					}
					i += num;
				}
			}
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
			PCBHighlight_Arc_Search(parc, bShowHighlight);
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
			PCBHighlight_Pad_Search(ppad, bShowHighlight);

			if(bEDA_PcbPolygonHighlight == TRUE)
			{
				CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&> arrayPolygonHighlight;
				PCBHighlightPolygon_Pad_Search(ppad, arrayPolygonHighlight, bShowHighlight);
				int num = arrayPolygonHighlight.GetCount();
				if(num > 0)
				{
					for(int j=i+1; j<m_arrayPcbHighlight.GetCount(); j++)
					{
						selected_element = m_arrayPcbHighlight.GetAt(j);
						arrayPolygonHighlight.Add(selected_element);
					}
					for(int j=i+1; j<m_arrayPcbHighlight.GetCount(); j++)
					{
						m_arrayPcbHighlight.RemoveAt(j);
						j--;
					}
					for(int j=0; j<arrayPolygonHighlight.GetCount(); j++)
					{
						selected_element = arrayPolygonHighlight.GetAt(j);
						m_arrayPcbHighlight.Add(selected_element);
					}
					i += num;
				}
			}
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
			PCBHighlight_Via_Search(pvia, bShowHighlight);		

			if(bEDA_PcbPolygonHighlight == TRUE)
			{
				CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&> arrayPolygonHighlight;
				PCBHighlightPolygon_Via_Search(pvia, arrayPolygonHighlight, bShowHighlight);
				int num = arrayPolygonHighlight.GetCount();
				if(num > 0)
				{
					for(int j=i+1; j<m_arrayPcbHighlight.GetCount(); j++)
					{
						selected_element = m_arrayPcbHighlight.GetAt(j);
						arrayPolygonHighlight.Add(selected_element);
					}
					for(int j=i+1; j<m_arrayPcbHighlight.GetCount(); j++)
					{
						m_arrayPcbHighlight.RemoveAt(j);
						j--;
					}
					for(int j=0; j<arrayPolygonHighlight.GetCount(); j++)
					{
						selected_element = arrayPolygonHighlight.GetAt(j);
						m_arrayPcbHighlight.Add(selected_element);
					}
					i += num;
				}
			}
		}		
		else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
			PCBHighlight_Fill_Search(pfill, bShowHighlight);	
		}	
		else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
			PCBHighlight_Region_Search(pregion, bShowHighlight);	
		}
	}

	int nNet = -1;
	for(int i=0; i<m_arrayPcbHighlight.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selected_element = m_arrayPcbHighlight.GetAt(i);
		if(selected_element.pcbelement == PCB_ELEMENT_PAD)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
			if(ppad->m_nComponent >= 0)	
			{
				nNet = ppad->m_nNet;
				break;
			}
		}
	}
	if(nNet >= 0)
	{
		for(int i=0; i<m_arrayPcbHighlight.GetCount(); i++)
		{
			Struct_Pcb_Multi_Selection selected_element = m_arrayPcbHighlight.GetAt(i);
			if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
				if((ptrack->m_nComponent != -1)||(ptrack->m_nPolygon != -1)) continue;

				ptrack->m_nNet = nNet;
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
				if((parc->m_nComponent != -1)||(parc->m_nPolygon != -1)) continue;

				parc->m_nNet = nNet;
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
				if(pvia->m_nComponent != -1) continue;

				pvia->m_nNet = nNet;
			}		
			else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
				if(ppad->m_nComponent != -1) continue;

				ppad->m_nNet = nNet;
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
				if(pfill->m_nComponent != -1) continue;

				pfill->m_nNet = nNet;
			}	
			else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
			{
				COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
				if((pregion->m_nComponent != -1)||(pregion->m_nPolygon != -1)) continue;

				pregion->m_nNet = nNet;	
			}
		}
	}

}

void CESDView::PCBHighlight_Line_Search(COb_pcbtrack* phighlight_track, BOOL bShowHighlight)
{
	Struct_Pcb_Multi_Selection selected_element;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	if(bShowHighlight == TRUE) phighlight_track->DrawHighlighted(&dc, this, CONNECTIONS);

	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(ptrack->m_nLayer) != TRUE)) continue;

		if( ((ptrack->m_nLayer == phighlight_track->m_nLayer)||(ptrack->m_nLayer == MULTILAYER)||(phighlight_track->m_nLayer == MULTILAYER)) &&\
			(Line_Line_Connect(ptrack, phighlight_track, 0) == TRUE) )
		{
			selected_element.pcbelement = PCB_ELEMENT_TRACK;
			selected_element.index = i;

			ptrack->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag != FLAG_NORMAL) continue;
		
		if(((ppad->m_nLayer == MULTILAYER)||(ppad->m_nLayer == phighlight_track->m_nLayer)||(phighlight_track->m_nLayer == MULTILAYER))&&(Line_Pad_Connect(phighlight_track, ppad, 0) == TRUE))
		{
			if(ppad->m_nComponent >= 0)
			{
				COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
				if((pcomp->m_fX == fEDA_LoadPackage_X)&&(pcomp->m_fY == fEDA_LoadPackage_Y)) continue;
			}
			selected_element.pcbelement = PCB_ELEMENT_PAD;
			selected_element.index = i;

			ppad->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag != FLAG_NORMAL) continue;
		
		if((pDoc->IsConnectedToVia(phighlight_track->m_nLayer, pvia) == TRUE)&&(Line_Via_Connect(phighlight_track, pvia, 0) == TRUE))
		{
			selected_element.pcbelement = PCB_ELEMENT_VIA;
			selected_element.index = i;

			pvia->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(parc->m_nLayer) != TRUE)) continue;
	
		if( ((parc->m_nLayer == phighlight_track->m_nLayer)||(parc->m_nLayer == MULTILAYER)||(phighlight_track->m_nLayer == MULTILAYER))&&\
			(Line_Arc_Connect(phighlight_track, parc, 0) == TRUE) )
		{
			selected_element.pcbelement = PCB_ELEMENT_ARC;
			selected_element.index = i;

			parc->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}
	
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pDoc->IsOnSignalLayer(pfill->m_nLayer) != TRUE)) continue;
		
		if( ((pfill->m_nLayer == phighlight_track->m_nLayer)||(pfill->m_nLayer == MULTILAYER)||(phighlight_track->m_nLayer == MULTILAYER))&&\
			(Line_Fill_Connect(phighlight_track, pfill, 0) == TRUE) )
		{
			selected_element.pcbelement = PCB_ELEMENT_FILL;
			selected_element.index = i;

			pfill->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(pregion->m_nLayer) != TRUE)) continue;

		if( ((pregion->m_nLayer == phighlight_track->m_nLayer)||(pregion->m_nLayer == MULTILAYER)||(phighlight_track->m_nLayer == MULTILAYER))&&\
			(Line_Region_Connect(phighlight_track, pregion, 0) == TRUE) )
		{
			selected_element.pcbelement = PCB_ELEMENT_REGION;
			selected_element.index = i;

			pregion->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}
}

void CESDView::PCBHighlight_PolygonElement_Search(CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&>& arrayPolygonHighlight, BOOL bShowHighlight)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	for(int i=0; i<arrayPolygonHighlight.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selected_element = arrayPolygonHighlight.GetAt(i);
		if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
			if(ptrack->m_bPolygonOutline == TRUE) PCBHighlight_Line_Search(ptrack, bShowHighlight);
			else
			{
				for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
					if(ppad->m_nFlag != FLAG_NORMAL) continue;
					
					if(((ppad->m_nLayer == MULTILAYER)||(ppad->m_nLayer == ptrack->m_nLayer)||(ptrack->m_nLayer == MULTILAYER))&&(Line_Pad_Connect(ptrack, ppad, 0) == TRUE))
					{
						selected_element.pcbelement = PCB_ELEMENT_PAD;
						selected_element.index = j;

						ppad->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(selected_element);
					}
				}
				
				for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
				{
					COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
					if(pvia->m_nFlag != FLAG_NORMAL) continue;
					
					if((pDoc->IsConnectedToVia(ptrack->m_nLayer, pvia) == TRUE)&&(Line_Via_Connect(ptrack, pvia, 0) == TRUE))
					{
						selected_element.pcbelement = PCB_ELEMENT_VIA;
						selected_element.index = j;

						pvia->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(selected_element);
					}
				}
			}
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
			if(parc->m_bPolygonOutline == TRUE)	PCBHighlight_Arc_Search(parc, bShowHighlight);
			else
			{
				for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
					if(ppad->m_nFlag != FLAG_NORMAL) continue;
					
					if( ((ppad->m_nLayer == parc->m_nLayer)||(ppad->m_nLayer == MULTILAYER)||(parc->m_nLayer == MULTILAYER))&& \
						(Pad_Arc_Connect(ppad, parc, 0) == TRUE))
					{
						Struct_Pcb_Multi_Selection selected_element;
						selected_element.pcbelement = PCB_ELEMENT_PAD;
						selected_element.index = j;

						ppad->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(selected_element);
					}
				}

				for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
				{
					COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
					if(pvia->m_nFlag != FLAG_NORMAL) continue;
					
					if((pDoc->IsConnectedToVia(parc->m_nLayer, pvia) == TRUE)&&(Via_Arc_Connect(pvia, parc,0) == TRUE))
					{
						Struct_Pcb_Multi_Selection selected_element;
						selected_element.pcbelement = PCB_ELEMENT_VIA;
						selected_element.index = j;

						pvia->m_nFlag = FLAG_PROCESSING;
						m_arrayPcbHighlight.Add(selected_element);
					}
				}
			}
		}
		else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
			PCBHighlight_Region_Search(pregion, bShowHighlight);
		}
	}
}

void CESDView::PCBHighlight_Pad_Search(COb_pcbpad* phighlight_pad, BOOL bShowHighlight)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	if(bShowHighlight == TRUE) phighlight_pad->DrawHighlighted(&dc, this, CONNECTIONS);

	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(ptrack->m_nLayer) != TRUE)) continue;
		
		if(((phighlight_pad->m_nLayer == ptrack->m_nLayer)||(phighlight_pad->m_nLayer == MULTILAYER)||(ptrack->m_nLayer == MULTILAYER))&&(Line_Pad_Connect(ptrack, phighlight_pad, 0) == TRUE))
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_TRACK;
			selected_element.index = i;

			ptrack->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag != FLAG_NORMAL) continue;
		
		if(((ppad->m_nLayer == phighlight_pad->m_nLayer)||(ppad->m_nLayer == MULTILAYER)||(phighlight_pad->m_nLayer == MULTILAYER))&&(Pad_Pad_Connect(phighlight_pad, ppad, 0) == TRUE))
		{
			if(ppad->m_nComponent >= 0)
			{
				COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
				if((pcomp->m_fX == fEDA_LoadPackage_X)&&(pcomp->m_fY == fEDA_LoadPackage_Y)) continue;
			}

			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_PAD;
			selected_element.index = i;
			
			ppad->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag != FLAG_NORMAL) continue;
		
		if((pDoc->IsConnectedToVia(phighlight_pad->m_nLayer, pvia) == TRUE)&&(Pad_Via_Connect(phighlight_pad, pvia, 0) == TRUE))
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_VIA;
			selected_element.index = i;
			
			pvia->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(parc->m_nLayer) != TRUE)) continue;

		if(((phighlight_pad->m_nLayer == parc->m_nLayer)||(phighlight_pad->m_nLayer == MULTILAYER)||(parc->m_nLayer == MULTILAYER))&&(Pad_Arc_Connect(phighlight_pad, parc, 0) == TRUE))
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_ARC;
			selected_element.index = i;

			parc->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pDoc->IsOnSignalLayer(pfill->m_nLayer) != TRUE)) continue;
		
		if(((phighlight_pad->m_nLayer == pfill->m_nLayer)||(phighlight_pad->m_nLayer == MULTILAYER)||(pfill->m_nLayer == MULTILAYER))&&\
			(Pad_Fill_Connect(phighlight_pad, pfill, 0) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_FILL;
			selected_element.index = i;

			pfill->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(pregion->m_nLayer) != TRUE)) continue;

		if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == phighlight_pad->m_nLayer)||(phighlight_pad->m_nLayer == MULTILAYER))&&\
			(Pad_Region_Connect(phighlight_pad, pregion, 0) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_REGION;
			selected_element.index = i;

			pregion->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}
}

void CESDView::PCBHighlight_Via_Search(COb_pcbvia* phighlight_via, BOOL bShowHighlight)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	if(bShowHighlight == TRUE) phighlight_via->DrawHighlighted(&dc, this, CONNECTIONS);

	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(ptrack->m_nLayer) != TRUE)) continue;
		
		if((pDoc->IsConnectedToVia(ptrack->m_nLayer, phighlight_via) == TRUE)&&(Line_Via_Connect(ptrack, phighlight_via, 0) == TRUE))
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_TRACK;
			selected_element.index = i;

			ptrack->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(parc->m_nLayer) != TRUE)) continue;
		
		if((pDoc->IsConnectedToVia(parc->m_nLayer, phighlight_via) == TRUE)&&(Via_Arc_Connect(phighlight_via, parc, 0) == TRUE))
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_ARC;
			selected_element.index = i;

			parc->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag != FLAG_NORMAL) continue;
		
		if((pDoc->IsConnectedToVia(ppad->m_nLayer, phighlight_via) == TRUE)&&(Pad_Via_Connect(ppad, phighlight_via, 0) == TRUE))
		{
			if(ppad->m_nComponent >= 0)
			{
				COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
				if((pcomp->m_fX == fEDA_LoadPackage_X)&&(pcomp->m_fY == fEDA_LoadPackage_Y)) continue;
			}
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_PAD;
			selected_element.index = i;
			
			ppad->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag != FLAG_NORMAL) continue;
		
		if((pDoc->IsConnected_ViaToVia(pvia, phighlight_via) == TRUE)&&(Via_Via_Connect(pvia, phighlight_via, 0) == TRUE))
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_VIA;
			selected_element.index = i;
			
			pvia->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pDoc->IsOnSignalLayer(pfill->m_nLayer) != TRUE)) continue;
		
		if((pDoc->IsConnectedToVia(pfill->m_nLayer, phighlight_via) == TRUE)&&(Via_Fill_Connect(phighlight_via, pfill, 0) == TRUE))
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_FILL;
			selected_element.index = i;
			
			pfill->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(pregion->m_nLayer) != TRUE)) continue;

		if((pDoc->IsConnectedToVia(pregion->m_nLayer, phighlight_via) == TRUE)&&(Via_Region_Connect(phighlight_via, pregion, 0) == TRUE))
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_REGION;
			selected_element.index = i;

			pregion->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}
}

void CESDView::PCBHighlight_Arc_Search(COb_pcbarc* phighlight_arc, BOOL bShowHighlight)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	if(bShowHighlight == TRUE) phighlight_arc->DrawHighlighted(&dc, this, CONNECTIONS);
	

	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(ptrack->m_nLayer) != TRUE)) continue;

		if( ((ptrack->m_nLayer == MULTILAYER)||(ptrack->m_nLayer == phighlight_arc->m_nLayer)||(phighlight_arc->m_nLayer == MULTILAYER)) &&\
			(Line_Arc_Connect(ptrack, phighlight_arc, 0) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_TRACK;
			selected_element.index = i;

			ptrack->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(parc->m_nLayer) != TRUE)) continue;

		if( ((parc->m_nLayer == MULTILAYER)||(parc->m_nLayer == phighlight_arc->m_nLayer)||(phighlight_arc->m_nLayer == MULTILAYER)) &&\
			(Arc_Arc_Connect(parc, phighlight_arc, 0) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_ARC;
			selected_element.index = i;

			parc->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag != FLAG_NORMAL) continue;
		
		if( ((ppad->m_nLayer == phighlight_arc->m_nLayer)||(ppad->m_nLayer == MULTILAYER)||(phighlight_arc->m_nLayer == MULTILAYER))&& \
			(Pad_Arc_Connect(ppad, phighlight_arc, 0) == TRUE))
		{
			if(ppad->m_nComponent >= 0)
			{
				COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
				if((pcomp->m_fX == fEDA_LoadPackage_X)&&(pcomp->m_fY == fEDA_LoadPackage_Y)) continue;
			}

			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_PAD;
			selected_element.index = i;

			ppad->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag != FLAG_NORMAL) continue;
		
		if((pDoc->IsConnectedToVia(phighlight_arc->m_nLayer, pvia) == TRUE)&&(Via_Arc_Connect(pvia, phighlight_arc,0) == TRUE))
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_VIA;
			selected_element.index = i;

			pvia->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pDoc->IsOnSignalLayer(pfill->m_nLayer) != TRUE)) continue;
		
		if( ((pfill->m_nLayer == MULTILAYER)||(pfill->m_nLayer == phighlight_arc->m_nLayer)||(phighlight_arc->m_nLayer == MULTILAYER))&&\
			(Arc_Fill_Connect(phighlight_arc, pfill, 0) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_FILL;
			selected_element.index = i;

			pfill->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(pregion->m_nLayer) != TRUE)) continue;

		if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == phighlight_arc->m_nLayer)||(phighlight_arc->m_nLayer == MULTILAYER))&&\
			(Arc_Region_Connect(phighlight_arc, pregion, 0) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_REGION;
			selected_element.index = i;

			pregion->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}
}

void CESDView::PCBHighlight_Fill_Search(COb_pcbfill* phighlight_fill, BOOL bShowHighlight)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	if(bShowHighlight == TRUE) phighlight_fill->DrawHighlighted(&dc, this, CONNECTIONS);

	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(ptrack->m_nLayer) != TRUE)) continue;

		if( ((ptrack->m_nLayer == MULTILAYER)||(ptrack->m_nLayer == phighlight_fill->m_nLayer)||(phighlight_fill->m_nLayer == MULTILAYER)) &&\
			(Line_Fill_Connect(ptrack, phighlight_fill, 0) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_TRACK;
			selected_element.index = i;

			ptrack->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(parc->m_nLayer) != TRUE)) continue;

		if( ((parc->m_nLayer == MULTILAYER)||(parc->m_nLayer == phighlight_fill->m_nLayer)||(phighlight_fill->m_nLayer == MULTILAYER)) &&\
			(Arc_Fill_Connect(parc, phighlight_fill, 0) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_ARC;
			selected_element.index = i;

			parc->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag != FLAG_NORMAL) continue;
		
		if( ((ppad->m_nLayer == phighlight_fill->m_nLayer)||(ppad->m_nLayer == MULTILAYER)||(phighlight_fill->m_nLayer == MULTILAYER))&& \
			(Pad_Fill_Connect(ppad, phighlight_fill, 0) == TRUE))
		{
			if(ppad->m_nComponent >= 0)
			{
				COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
				if((pcomp->m_fX == fEDA_LoadPackage_X)&&(pcomp->m_fY == fEDA_LoadPackage_Y)) continue;
			}

			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_PAD;
			selected_element.index = i;

			ppad->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag != FLAG_NORMAL) continue;
		
		if((pDoc->IsConnectedToVia(phighlight_fill->m_nLayer, pvia) == TRUE)&&(Via_Fill_Connect(pvia, phighlight_fill,0) == TRUE))
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_VIA;
			selected_element.index = i;

			pvia->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pDoc->IsOnSignalLayer(pfill->m_nLayer) != TRUE)) continue;
		
		if( ((pfill->m_nLayer == MULTILAYER)||(pfill->m_nLayer == phighlight_fill->m_nLayer)||(phighlight_fill->m_nLayer == MULTILAYER))&&\
			(Fill_Fill_Connect(phighlight_fill, pfill, 0) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_FILL;
			selected_element.index = i;

			pfill->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(pregion->m_nLayer) != TRUE)) continue;

		if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == phighlight_fill->m_nLayer)||(phighlight_fill->m_nLayer == MULTILAYER))&&\
			(Fill_Region_Connect(phighlight_fill, pregion, 0) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_REGION;
			selected_element.index = i;

			pregion->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}
}

void CESDView::PCBHighlight_Region_Search(COb_pcbregion* phighlight_region, BOOL bShowHighlight)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	if(bShowHighlight == TRUE) phighlight_region->DrawHighlighted(&dc, this, CONNECTIONS);
	

	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(ptrack->m_nLayer) != TRUE)) continue;

		if( ((ptrack->m_nLayer == MULTILAYER)||(ptrack->m_nLayer == phighlight_region->m_nLayer)||(phighlight_region->m_nLayer == MULTILAYER)) &&\
			(Line_Region_Connect(ptrack, phighlight_region, 0) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_TRACK;
			selected_element.index = i;

			ptrack->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(parc->m_nLayer) != TRUE)) continue;

		if( ((parc->m_nLayer == MULTILAYER)||(parc->m_nLayer == phighlight_region->m_nLayer)||(phighlight_region->m_nLayer == MULTILAYER)) &&\
			(Arc_Region_Connect(parc, phighlight_region, 0) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_ARC;
			selected_element.index = i;

			parc->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}


	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad->m_nFlag != FLAG_NORMAL) continue;
		
		if( ((ppad->m_nLayer == phighlight_region->m_nLayer)||(ppad->m_nLayer == MULTILAYER)||(phighlight_region->m_nLayer == MULTILAYER))&& \
			(Pad_Region_Connect(ppad, phighlight_region, 0) == TRUE))
		{
			if(ppad->m_nComponent >= 0)
			{
				COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
				if((pcomp->m_fX == fEDA_LoadPackage_X)&&(pcomp->m_fY == fEDA_LoadPackage_Y)) continue;
			}

			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_PAD;
			selected_element.index = i;

			ppad->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if(pvia->m_nFlag != FLAG_NORMAL) continue;
		
		if((pDoc->IsConnectedToVia(phighlight_region->m_nLayer, pvia) == TRUE)&&(Via_Region_Connect(pvia, phighlight_region,0) == TRUE))
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_VIA;
			selected_element.index = i;

			pvia->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pDoc->IsOnSignalLayer(pfill->m_nLayer) != TRUE)) continue;
		
		if( ((pfill->m_nLayer == MULTILAYER)||(pfill->m_nLayer == phighlight_region->m_nLayer)||(phighlight_region->m_nLayer == MULTILAYER))&&\
			(Fill_Region_Connect(pfill, phighlight_region, 0) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_FILL;
			selected_element.index = i;

			pfill->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}

	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(i);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon != -1)||(pDoc->IsOnSignalLayer(pregion->m_nLayer) != TRUE)) continue;

		if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == phighlight_region->m_nLayer)||(phighlight_region->m_nLayer == MULTILAYER))&&\
			(Region_Region_Connect(phighlight_region, pregion, 0) == TRUE) )
		{
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_REGION;
			selected_element.index = i;

			pregion->m_nFlag = FLAG_PROCESSING;
			m_arrayPcbHighlight.Add(selected_element);
		}
	}
}

void CESDView::PCBHighlightPolygon_Line_Search(COb_pcbtrack* phighlight_track, CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&>& arrayPolygonHighlight, BOOL bShowHighlight)
{
	Struct_Pcb_Multi_Selection selected_element;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);


	for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
	{
		COb_pcbregion* ppolygon_region = pDoc->m_arrayPcbRegion.GetAt(j);
		if((ppolygon_region->m_nFlag != FLAG_NORMAL)||(ppolygon_region->m_nPolygon < 0)) continue;

		if( ((ppolygon_region->m_nLayer == phighlight_track->m_nLayer)||(ppolygon_region->m_nLayer == MULTILAYER)||(phighlight_track->m_nLayer == MULTILAYER))&&\
			(Line_Region_Connect(phighlight_track, ppolygon_region, 0) == TRUE) )
		{
			selected_element.pcbelement = PCB_ELEMENT_REGION;
			selected_element.index = j;

			ppolygon_region->m_nFlag = FLAG_PROCESSING;
			arrayPolygonHighlight.Add(selected_element);

			for(int k=0; k<pDoc->m_arrayPcbTrack.GetCount(); k++)
			{
				COb_pcbtrack* psub_track = pDoc->m_arrayPcbTrack.GetAt(k);
				if((psub_track->m_nFlag == FLAG_NORMAL)&&(psub_track->m_nPolygon == ppolygon_region->m_nPolygon)&&(psub_track->m_nSubPolyIndex == ppolygon_region->m_nSubPolyIndex))
				{
					selected_element.pcbelement = PCB_ELEMENT_TRACK;
					selected_element.index = k;

					psub_track->m_nFlag = FLAG_PROCESSING;
					if(psub_track->m_bPolygonOutline == FALSE)	arrayPolygonHighlight.Add(selected_element);
				}
			}
			for(int k=0; k<pDoc->m_arrayPcbArc.GetCount(); k++)
			{
				COb_pcbarc* psub_arc = pDoc->m_arrayPcbArc.GetAt(k);
				if((psub_arc->m_nFlag == FLAG_NORMAL)&&(psub_arc->m_nPolygon == ppolygon_region->m_nPolygon)&&(psub_arc->m_nSubPolyIndex == ppolygon_region->m_nSubPolyIndex))
				{
					selected_element.pcbelement = PCB_ELEMENT_ARC;
					selected_element.index = k;

					psub_arc->m_nFlag = FLAG_PROCESSING;
					if(psub_arc->m_bPolygonOutline == FALSE)	arrayPolygonHighlight.Add(selected_element);
				}
			}
		}
	}


	for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
	{
		COb_pcbtrack* ppolygon_track = pDoc->m_arrayPcbTrack.GetAt(j);
		if((ppolygon_track->m_nFlag != FLAG_NORMAL)||(ppolygon_track->m_nPolygon < 0)||(ppolygon_track->m_bPolygonOutline == FALSE)) continue;
					
		if( ((phighlight_track->m_nLayer == ppolygon_track->m_nLayer)||(phighlight_track->m_nLayer == MULTILAYER)||(ppolygon_track->m_nLayer == MULTILAYER)) &&\
			(Line_Line_Connect(phighlight_track, ppolygon_track, 0) == TRUE) )
		{
			selected_element.pcbelement = PCB_ELEMENT_TRACK;
			selected_element.index = j;

			ppolygon_track->m_nFlag = FLAG_PROCESSING;
			arrayPolygonHighlight.Add(selected_element);

			for(int k=0; k<pDoc->m_arrayPcbTrack.GetCount(); k++)
			{
				COb_pcbtrack* psub_track = pDoc->m_arrayPcbTrack.GetAt(k);
				if((psub_track->m_nFlag == FLAG_NORMAL)&&(psub_track->m_nPolygon == ppolygon_track->m_nPolygon)&&(psub_track->m_nSubPolyIndex == ppolygon_track->m_nSubPolyIndex))
				{
					selected_element.pcbelement = PCB_ELEMENT_TRACK;
					selected_element.index = k;

					psub_track->m_nFlag = FLAG_PROCESSING;
					arrayPolygonHighlight.Add(selected_element);
				}
			}
			for(int k=0; k<pDoc->m_arrayPcbArc.GetCount(); k++)
			{
				COb_pcbarc* psub_arc = pDoc->m_arrayPcbArc.GetAt(k);
				if((psub_arc->m_nFlag == FLAG_NORMAL)&&(psub_arc->m_nPolygon == ppolygon_track->m_nPolygon)&&(psub_arc->m_nSubPolyIndex == ppolygon_track->m_nSubPolyIndex))
				{
					selected_element.pcbelement = PCB_ELEMENT_ARC;
					selected_element.index = k;

					psub_arc->m_nFlag = FLAG_PROCESSING;
					arrayPolygonHighlight.Add(selected_element);
				}
			}
		}
	}
	
	for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
	{
		COb_pcbarc* ppolygon_arc = pDoc->m_arrayPcbArc.GetAt(j);
		if((ppolygon_arc->m_nFlag != FLAG_NORMAL)||(ppolygon_arc->m_nPolygon < 0)||(ppolygon_arc->m_bPolygonOutline == FALSE)) continue;
					
		if( ((phighlight_track->m_nLayer == ppolygon_arc->m_nLayer)||(phighlight_track->m_nLayer == MULTILAYER)||(ppolygon_arc->m_nLayer == MULTILAYER)) &&\
			(Line_Arc_Connect(phighlight_track, ppolygon_arc, 0) == TRUE) )
		{
			selected_element.pcbelement = PCB_ELEMENT_ARC;
			selected_element.index = j;

			ppolygon_arc->m_nFlag = FLAG_PROCESSING;
			arrayPolygonHighlight.Add(selected_element);

			for(int k=0; k<pDoc->m_arrayPcbTrack.GetCount(); k++)
			{
				COb_pcbtrack* psub_track = pDoc->m_arrayPcbTrack.GetAt(k);
				if((psub_track->m_nFlag == FLAG_NORMAL)&&(psub_track->m_nPolygon == ppolygon_arc->m_nPolygon)&&(psub_track->m_nSubPolyIndex == ppolygon_arc->m_nSubPolyIndex))
				{
					selected_element.pcbelement = PCB_ELEMENT_TRACK;
					selected_element.index = k;

					psub_track->m_nFlag = FLAG_PROCESSING;
					arrayPolygonHighlight.Add(selected_element);
				}
			}
			for(int k=0; k<pDoc->m_arrayPcbArc.GetCount(); k++)
			{
				COb_pcbarc* psub_arc = pDoc->m_arrayPcbArc.GetAt(k);
				if((psub_arc->m_nFlag == FLAG_NORMAL)&&(psub_arc->m_nPolygon == ppolygon_arc->m_nPolygon)&&(psub_arc->m_nSubPolyIndex == ppolygon_arc->m_nSubPolyIndex))
				{
					selected_element.pcbelement = PCB_ELEMENT_ARC;
					selected_element.index = k;

					psub_arc->m_nFlag = FLAG_PROCESSING;
					arrayPolygonHighlight.Add(selected_element);
				}
			}
		}
	}

	PCBHighlight_PolygonElement_Search(arrayPolygonHighlight, bShowHighlight);
}

void CESDView::PCBHighlightPolygon_Pad_Search(COb_pcbpad* phighlight_pad, CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&>& arrayPolygonHighlight, BOOL bShowHighlight)
{
	Struct_Pcb_Multi_Selection selected_element;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
	{
		COb_pcbregion* ppolygon_region = pDoc->m_arrayPcbRegion.GetAt(j);
		if((ppolygon_region->m_nFlag != FLAG_NORMAL)||(ppolygon_region->m_nPolygon < 0)) continue;

		if( ((ppolygon_region->m_nLayer == MULTILAYER)||(ppolygon_region->m_nLayer == phighlight_pad->m_nLayer)||(phighlight_pad->m_nLayer == MULTILAYER))&&\
			(Pad_Region_Connect(phighlight_pad, ppolygon_region, 0) == TRUE) )
		{
			selected_element.pcbelement = PCB_ELEMENT_REGION;
			selected_element.index = j;

			ppolygon_region->m_nFlag = FLAG_PROCESSING;
			arrayPolygonHighlight.Add(selected_element);

			for(int k=0; k<pDoc->m_arrayPcbTrack.GetCount(); k++)
			{
				COb_pcbtrack* psub_track = pDoc->m_arrayPcbTrack.GetAt(k);
				if((psub_track->m_nFlag == FLAG_NORMAL)&&(psub_track->m_nPolygon == ppolygon_region->m_nPolygon)&&(psub_track->m_nSubPolyIndex == ppolygon_region->m_nSubPolyIndex))
				{
					selected_element.pcbelement = PCB_ELEMENT_TRACK;
					selected_element.index = k;

					psub_track->m_nFlag = FLAG_PROCESSING;
					if(psub_track->m_bPolygonOutline == FALSE)	arrayPolygonHighlight.Add(selected_element);
				}
			}
			for(int k=0; k<pDoc->m_arrayPcbArc.GetCount(); k++)
			{
				COb_pcbarc* psub_arc = pDoc->m_arrayPcbArc.GetAt(k);
				if((psub_arc->m_nFlag == FLAG_NORMAL)&&(psub_arc->m_nPolygon == ppolygon_region->m_nPolygon)&&(psub_arc->m_nSubPolyIndex == ppolygon_region->m_nSubPolyIndex))
				{
					selected_element.pcbelement = PCB_ELEMENT_ARC;
					selected_element.index = k;

					psub_arc->m_nFlag = FLAG_PROCESSING;
					if(psub_arc->m_bPolygonOutline == FALSE)	arrayPolygonHighlight.Add(selected_element);
				}
			}
		}
	}

	for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
	{
		COb_pcbtrack* ppolygon_track = pDoc->m_arrayPcbTrack.GetAt(j);
		if((ppolygon_track->m_nFlag != FLAG_NORMAL)||(ppolygon_track->m_nPolygon < 0)||(ppolygon_track->m_bPolygonOutline == TRUE)) continue;
				
		if(((phighlight_pad->m_nLayer == MULTILAYER)||(phighlight_pad->m_nLayer == ppolygon_track->m_nLayer)||(ppolygon_track->m_nLayer == MULTILAYER))&&(Line_Pad_Connect(ppolygon_track, phighlight_pad, 0) == TRUE))
		{
			selected_element.pcbelement = PCB_ELEMENT_TRACK;
			selected_element.index = j;

			ppolygon_track->m_nFlag = FLAG_PROCESSING;
			arrayPolygonHighlight.Add(selected_element);

			for(int k=0; k<pDoc->m_arrayPcbTrack.GetCount(); k++)
			{
				COb_pcbtrack* psub_track = pDoc->m_arrayPcbTrack.GetAt(k);
				if((psub_track->m_nFlag == FLAG_NORMAL)&&(psub_track->m_nPolygon == ppolygon_track->m_nPolygon)&&(psub_track->m_nSubPolyIndex == ppolygon_track->m_nSubPolyIndex))
				{
					selected_element.pcbelement = PCB_ELEMENT_TRACK;
					selected_element.index = k;

					psub_track->m_nFlag = FLAG_PROCESSING;
					arrayPolygonHighlight.Add(selected_element);
				}
			}
			for(int k=0; k<pDoc->m_arrayPcbArc.GetCount(); k++)
			{
				COb_pcbarc* psub_arc = pDoc->m_arrayPcbArc.GetAt(k);
				if((psub_arc->m_nFlag == FLAG_NORMAL)&&(psub_arc->m_nPolygon == ppolygon_track->m_nPolygon)&&(psub_arc->m_nSubPolyIndex == ppolygon_track->m_nSubPolyIndex))
				{
					selected_element.pcbelement = PCB_ELEMENT_ARC;
					selected_element.index = k;

					psub_arc->m_nFlag = FLAG_PROCESSING;
					arrayPolygonHighlight.Add(selected_element);
				}
			}
		}
	}
	
	PCBHighlight_PolygonElement_Search(arrayPolygonHighlight, bShowHighlight);
}


void CESDView::PCBHighlightPolygon_Via_Search(COb_pcbvia* phighlight_via, CArray <Struct_Pcb_Multi_Selection, Struct_Pcb_Multi_Selection&>& arrayPolygonHighlight, BOOL bShowHighlight)
{
	Struct_Pcb_Multi_Selection selected_element;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
	{
		COb_pcbregion* ppolygon_region = pDoc->m_arrayPcbRegion.GetAt(j);
		if((ppolygon_region->m_nFlag != FLAG_NORMAL)||(ppolygon_region->m_nPolygon < 0)) continue;

		if((pDoc->IsConnectedToVia(ppolygon_region->m_nLayer, phighlight_via) == TRUE)&&(Via_Region_Connect(phighlight_via, ppolygon_region, 0) == TRUE))
		{
			selected_element.pcbelement = PCB_ELEMENT_REGION;
			selected_element.index = j;

			ppolygon_region->m_nFlag = FLAG_PROCESSING;
			arrayPolygonHighlight.Add(selected_element);

			for(int k=0; k<pDoc->m_arrayPcbTrack.GetCount(); k++)
			{
				COb_pcbtrack* psub_track = pDoc->m_arrayPcbTrack.GetAt(k);
				if((psub_track->m_nFlag == FLAG_NORMAL)&&(psub_track->m_nPolygon == ppolygon_region->m_nPolygon)&&(psub_track->m_nSubPolyIndex == ppolygon_region->m_nSubPolyIndex))
				{
					selected_element.pcbelement = PCB_ELEMENT_TRACK;
					selected_element.index = k;

					psub_track->m_nFlag = FLAG_PROCESSING;
					if(psub_track->m_bPolygonOutline == FALSE)	arrayPolygonHighlight.Add(selected_element);
				}
			}
			for(int k=0; k<pDoc->m_arrayPcbArc.GetCount(); k++)
			{
				COb_pcbarc* psub_arc = pDoc->m_arrayPcbArc.GetAt(k);
				if((psub_arc->m_nFlag == FLAG_NORMAL)&&(psub_arc->m_nPolygon == ppolygon_region->m_nPolygon)&&(psub_arc->m_nSubPolyIndex == ppolygon_region->m_nSubPolyIndex))
				{
					selected_element.pcbelement = PCB_ELEMENT_ARC;
					selected_element.index = k;

					psub_arc->m_nFlag = FLAG_PROCESSING;
					if(psub_arc->m_bPolygonOutline == FALSE)	arrayPolygonHighlight.Add(selected_element);
				}
			}
		}
	}

	for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
	{
		COb_pcbtrack* ppolygon_track = pDoc->m_arrayPcbTrack.GetAt(j);
		if((ppolygon_track->m_nFlag != FLAG_NORMAL)||(ppolygon_track->m_nPolygon < 0)||(ppolygon_track->m_bPolygonOutline == TRUE)) continue;
				
		if((pDoc->IsConnectedToVia(ppolygon_track->m_nLayer, phighlight_via) == TRUE)&&(Line_Via_Connect(ppolygon_track, phighlight_via, 0) == TRUE))
		{
			selected_element.pcbelement = PCB_ELEMENT_TRACK;
			selected_element.index = j;

			ppolygon_track->m_nFlag = FLAG_PROCESSING;
			arrayPolygonHighlight.Add(selected_element);

			for(int k=0; k<pDoc->m_arrayPcbTrack.GetCount(); k++)
			{
				COb_pcbtrack* psub_track = pDoc->m_arrayPcbTrack.GetAt(k);
				if((psub_track->m_nFlag == FLAG_NORMAL)&&(psub_track->m_nPolygon == ppolygon_track->m_nPolygon)&&(psub_track->m_nSubPolyIndex == ppolygon_track->m_nSubPolyIndex))
				{
					selected_element.pcbelement = PCB_ELEMENT_TRACK;
					selected_element.index = k;

					psub_track->m_nFlag = FLAG_PROCESSING;
					arrayPolygonHighlight.Add(selected_element);
				}
			}
			for(int k=0; k<pDoc->m_arrayPcbArc.GetCount(); k++)
			{
				COb_pcbarc* psub_arc = pDoc->m_arrayPcbArc.GetAt(k);
				if((psub_arc->m_nFlag == FLAG_NORMAL)&&(psub_arc->m_nPolygon == ppolygon_track->m_nPolygon)&&(psub_arc->m_nSubPolyIndex == ppolygon_track->m_nSubPolyIndex))
				{
					selected_element.pcbelement = PCB_ELEMENT_ARC;
					selected_element.index = k;

					psub_arc->m_nFlag = FLAG_PROCESSING;
					arrayPolygonHighlight.Add(selected_element);
				}
			}
		}
	}
	
	PCBHighlight_PolygonElement_Search(arrayPolygonHighlight, bShowHighlight);
}


BOOL CESDView::Line_Line_Connect(COb_pcbtrack* ptrack1, COb_pcbtrack* ptrack2, double drc)
{
	double d = ptrack1->m_fWidth/2 + ptrack2->m_fWidth/2 + drc;

	if((MIN(ptrack1->m_fX1, ptrack1->m_fX2) - MAX(ptrack2->m_fX1, ptrack2->m_fX2)) > d) return CONNECT_NO;
	if((MIN(ptrack2->m_fX1, ptrack2->m_fX2) - MAX(ptrack1->m_fX1, ptrack1->m_fX2)) > d) return CONNECT_NO;
	if((MIN(ptrack1->m_fY1, ptrack1->m_fY2) - MAX(ptrack2->m_fY1, ptrack2->m_fY2)) > d) return CONNECT_NO;
	if((MIN(ptrack2->m_fY1, ptrack2->m_fY2) - MAX(ptrack1->m_fY1, ptrack1->m_fY2)) > d) return CONNECT_NO;
	
	return Dist_Line_Line(ptrack1->m_fX1,ptrack1->m_fY1, ptrack1->m_fX2, ptrack1->m_fY2,   ptrack2->m_fX1, ptrack2->m_fY1, ptrack2->m_fX2, ptrack2->m_fY2,  d);
}

BOOL CESDView::Line_Pad_Connect(COb_pcbtrack* ptrack, COb_pcbpad* ppad, double drc)
{
	double dist, d;
	double deltax, deltay, temp;

	if(ppad->m_nPadMode == PAD_MODE_SIMPLE)
	{
		dist = MAX(ppad->m_fSizeX[TOPLAYER], ppad->m_fSizeY[TOPLAYER]);
	}
	else 
	{
		dist = MAX(ppad->m_fSizeX[TOPLAYER], ppad->m_fSizeY[TOPLAYER]);
		dist = MAX(dist, ppad->m_fSizeX[BOTTOMLAYER]);
		dist = MAX(dist, ppad->m_fSizeY[BOTTOMLAYER]);
		dist = MAX(dist, ppad->m_fSizeX[MIDLAYER1]);
		dist = MAX(dist, ppad->m_fSizeY[MIDLAYER1]);
	}

	d = (ptrack->m_fWidth)/2 + dist + drc;


	if((MIN(ptrack->m_fX1, ptrack->m_fX2) - ppad->m_fCx) > d) return CONNECT_NO;
	if((ppad->m_fCx - MAX(ptrack->m_fX1, ptrack->m_fX2)) > d) return CONNECT_NO;
	if((MIN(ptrack->m_fY1, ptrack->m_fY2) - ppad->m_fCy) > d) return CONNECT_NO;
	if((ppad->m_fCy - MAX(ptrack->m_fY1, ptrack->m_fY2)) > d) return CONNECT_NO;


	double sx,sy,ex,ey,cx,cy;
	   
	sx = ptrack->m_fX1;  ex = ptrack->m_fX2;
	sy = ptrack->m_fY1;  ey = ptrack->m_fY2;
	cx = ppad->m_fCx; cy = ppad->m_fCy;

	if((ppad->m_nLayer == MULTILAYER)||(ppad->m_nLayer == ptrack->m_nLayer)||(ptrack->m_nLayer == MULTILAYER))
	{
		int layer;
		if(ppad->m_nPadMode == PAD_MODE_SIMPLE) layer = TOPLAYER;
		else
		{
			if(ptrack->m_nLayer == TOPLAYER) layer = TOPLAYER;
			else if(ptrack->m_nLayer == BOTTOMLAYER) layer = BOTTOMLAYER;
			else layer = MIDLAYER1;
		}

		if(ppad->m_nShape[layer] == PAD_SHAPE_ROUND)
		{
			if(ppad->m_fSizeX[layer] == ppad->m_fSizeY[layer])
			{
				d = ptrack->m_fWidth/2 + ppad->m_fSizeX[layer]/2 + drc;
				return Dist_P_Line(cx,cy,sx,sy,ex,ey,d);
			}
			else if(ppad->m_fSizeX[layer] > ppad->m_fSizeY[layer])
			{
				double fr, cx1, cy1, cx2, cy2;
					
				fr = ppad->m_fSizeY[layer];

				cx1 = ppad->m_fCx - cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;
				cy1 = ppad->m_fCy - sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;

				cx2 = ppad->m_fCx + cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;
				cy2 = ppad->m_fCy + sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;

				d = ptrack->m_fWidth/2 + fr/2 + drc;
				return Dist_Line_Line(cx1, cy1, cx2, cy2, sx, sy, ex, ey, d);
			}
			else
			{
				double fr, cx1, cy1, cx2, cy2;
				
				fr = ppad->m_fSizeX[layer];

				cx1 = ppad->m_fCx - sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
				cy1 = ppad->m_fCy + cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;

				cx2 = ppad->m_fCx + sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
				cy2 = ppad->m_fCy - cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
					
				d = ptrack->m_fWidth/2 + fr/2 + drc;
				return Dist_Line_Line(cx1, cy1, cx2, cy2, sx, sy, ex, ey, d);								
			}
		}
		else if(ppad->m_nShape[layer] == PAD_SHAPE_RECTANGLE)
		{
			d = ptrack->m_fWidth/2 + drc;
			COb_pcbfill afill;
			afill.m_fX1 = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
			afill.m_fY1 = ppad->m_fCy - ppad->m_fSizeY[layer]/2;
			afill.m_fX2 = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
			afill.m_fY2 = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
			afill.m_fRotation = ppad->m_fRotation;

			double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
			afill.GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);

			BOOL b = Dist_Line_Line(fx1, fy1, fx3, fy3, sx, sy, ex, ey, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_Line_Line(fx3, fy3, fx2, fy2, sx, sy, ex, ey, d);
			if(b == TRUE) return CONNECT_YES;	
			b = Dist_Line_Line(fx2, fy2, fx4, fy4, sx, sy, ex, ey, d);
			if(b == TRUE) return CONNECT_YES;					
			b = Dist_Line_Line(fx4, fy4, fx1, fy1, sx, sy, ex, ey, d);
			if(b == TRUE) return CONNECT_YES;

			return CONNECT_NO;
		}
		else
		{
			d = ptrack->m_fWidth/2 + drc;
			if( ppad->m_fSizeX[layer] >= ppad->m_fSizeY[layer])
			{
				Fpoint fpoint[9];

				fpoint[0].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2 + ppad->m_fSizeY[layer]/4;
				fpoint[0].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;				
				fpoint[1].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
				fpoint[1].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/4;
				fpoint[2].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
				fpoint[2].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/4;
				fpoint[3].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2 + ppad->m_fSizeY[layer]/4;
				fpoint[3].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
				fpoint[4].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2 - ppad->m_fSizeY[layer]/4;
				fpoint[4].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
				fpoint[5].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
				fpoint[5].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/4;
				fpoint[6].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
				fpoint[6].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/4;
				fpoint[7].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2 - ppad->m_fSizeY[layer]/4;
				fpoint[7].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;

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

				fpoint[8].fx = fpoint[0].fx;
				fpoint[8].fy = fpoint[0].fy;
				for(int i=0; i<8; i++)
				{
					BOOL b = Dist_Line_Line(fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, sx, sy, ex, ey, d);
					if(b == TRUE) return CONNECT_YES;
				}

				return CONNECT_NO;
			}
			else
			{
				Fpoint fpoint[9];
				fpoint[0].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/4;
				fpoint[0].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;
				fpoint[1].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
				fpoint[1].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2 + ppad->m_fSizeX[layer]/4;
				fpoint[2].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
				fpoint[2].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2 - ppad->m_fSizeX[layer]/4;
				fpoint[3].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/4;
				fpoint[3].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
				fpoint[4].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/4;
				fpoint[4].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
				fpoint[5].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
				fpoint[5].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2 - ppad->m_fSizeX[layer]/4;
				fpoint[6].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
				fpoint[6].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2 + ppad->m_fSizeX[layer]/4;
				fpoint[7].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/4;
				fpoint[7].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;

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

				fpoint[8].fx = fpoint[0].fx;
				fpoint[8].fy = fpoint[0].fy;
				for(int i=0; i<8; i++)
				{
					BOOL b = Dist_Line_Line(fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, sx, sy, ex, ey, d);
					if(b == TRUE) return CONNECT_YES;
				}
					
				return CONNECT_NO;
			}
		}
	}
	
	return CONNECT_NO;
}

BOOL CESDView::Line_Via_Connect(COb_pcbtrack* ptrack, COb_pcbvia* pvia, double drc)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	double dist, d;
	double deltax, deltay, temp;
	   
	d = (ptrack->m_fWidth)/2 + pvia->m_fDia/2 + drc;
	if((MIN(ptrack->m_fX1, ptrack->m_fX2) - pvia->m_fCx) > d) return CONNECT_NO;
	if((pvia->m_fCx - MAX(ptrack->m_fX1, ptrack->m_fX2)) > d) return CONNECT_NO;
	if((MIN(ptrack->m_fY1, ptrack->m_fY2) - pvia->m_fCy) > d) return CONNECT_NO;
	if((pvia->m_fCy - MAX(ptrack->m_fY1, ptrack->m_fY2)) > d) return CONNECT_NO;

	double sx,sy,ex,ey,cx,cy;
	   
	sx = ptrack->m_fX1;  ex = ptrack->m_fX2;
	sy = ptrack->m_fY1;  ey = ptrack->m_fY2;
	cx = pvia->m_fCx; cy = pvia->m_fCy;
	if(pDoc->IsConnectedToVia(ptrack->m_nLayer, pvia) == TRUE) return Dist_P_Line(cx,cy,sx,sy,ex,ey,d);
	
	return CONNECT_NO;
}

BOOL CESDView::Line_Arc_Connect(COb_pcbtrack* ptrack, COb_pcbarc* parc, double drc)
{
	double d = ptrack->m_fWidth/2 + parc->m_fWidth/2 + parc->m_fRadius + drc;
	if((MIN(ptrack->m_fX1, ptrack->m_fX2) - parc->m_fCx) > d) return CONNECT_NO;
	if((parc->m_fCx - MAX(ptrack->m_fX1, ptrack->m_fX2)) > d) return CONNECT_NO;
	if((MIN(ptrack->m_fY1, ptrack->m_fY2) - parc->m_fCy) > d) return CONNECT_NO;
	if((parc->m_fCy - MAX(ptrack->m_fY1, ptrack->m_fY2)) > d) return CONNECT_NO;

	d = ptrack->m_fWidth/2 + parc->m_fWidth/2 + drc;
	return Dist_Line_Arc(ptrack->m_fX1, ptrack->m_fY1, ptrack->m_fX2, ptrack->m_fY2, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
}

BOOL CESDView::Line_Fill_Connect(COb_pcbtrack* ptrack, COb_pcbfill* pfill, double drc)
{
	double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
	pfill->GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);

	double fminx1, fminy1, fmaxx1, fmaxy1;
	fminx1 = MIN(fx1, fx2);
	fminx1 = MIN(fminx1, fx3);
	fminx1 = MIN(fminx1, fx4);
	fminy1 = MIN(fy1, fy2);
	fminy1 = MIN(fminy1, fy3);
	fminy1 = MIN(fminy1, fy4);
	fmaxx1 = MAX(fx1, fx2);
	fmaxx1 = MAX(fmaxx1, fx3);
	fmaxx1 = MAX(fmaxx1, fx4);
	fmaxy1 = MAX(fy1, fy2);
	fmaxy1 = MAX(fmaxy1, fy3);
	fmaxy1 = MAX(fmaxy1, fy4);
	double fminx2, fminy2, fmaxx2, fmaxy2;
	fminx2 = MIN(ptrack->m_fX1, ptrack->m_fX2);
	fminy2 = MIN(ptrack->m_fY1, ptrack->m_fY2);
	fmaxx2 = MAX(ptrack->m_fX1, ptrack->m_fX2);
	fmaxy2 = MAX(ptrack->m_fY1, ptrack->m_fY2);
	
	double d = ptrack->m_fWidth/2 + drc;
	if((fminx1 > fmaxx2)&&((fminx1 - fmaxx2) > d)) return CONNECT_NO;
	if((fmaxx1 < fminx2)&&((fminx2 - fmaxx1) > d)) return CONNECT_NO;
	if((fminy1 > fmaxy2)&&((fminy1 - fmaxy2) > d)) return CONNECT_NO;
	if((fmaxy1 < fminy2)&&((fminy2 - fmaxy1) > d)) return CONNECT_NO;

	BOOL b = Dist_Line_Line(fx1, fy1, fx3, fy3, ptrack->m_fX1, ptrack->m_fY1, ptrack->m_fX2, ptrack->m_fY2, d);
	if(b == TRUE) return CONNECT_YES;
	b = Dist_Line_Line(fx3, fy3, fx2, fy2, ptrack->m_fX1, ptrack->m_fY1, ptrack->m_fX2, ptrack->m_fY2, d);
	if(b == TRUE) return CONNECT_YES;
	b = Dist_Line_Line(fx2, fy2, fx4, fy4, ptrack->m_fX1, ptrack->m_fY1, ptrack->m_fX2, ptrack->m_fY2, d);
	if(b == TRUE) return CONNECT_YES;
	b = Dist_Line_Line(fx4, fy4, fx1, fy1, ptrack->m_fX1, ptrack->m_fY1, ptrack->m_fX2, ptrack->m_fY2, d);
	if(b == TRUE) return CONNECT_YES;

	return CONNECT_NO;
}

BOOL CESDView::Line_Region_Connect(COb_pcbtrack* ptrack, COb_pcbregion* pregion, double drc)
{
	double fminx1=100000, fminy1=100000, fmaxx1=-100000, fmaxy1=-100000;
	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex vertex;
		vertex = pregion->m_cVertex.GetAt(i);
		fminx1 = MIN(fminx1, vertex.fx);
		fmaxx1 = MAX(fmaxx1, vertex.fx);
		fminy1 = MIN(fminy1, vertex.fy);
		fmaxy1 = MAX(fmaxy1, vertex.fy);
	}

	double fminx2, fminy2, fmaxx2, fmaxy2;
	fminx2 = MIN(ptrack->m_fX1, ptrack->m_fX2);
	fminy2 = MIN(ptrack->m_fY1, ptrack->m_fY2);
	fmaxx2 = MAX(ptrack->m_fX1, ptrack->m_fX2);
	fmaxy2 = MAX(ptrack->m_fY1, ptrack->m_fY2);

	double d = ptrack->m_fWidth/2 + drc;
	if((fminx1 > fmaxx2)&&((fminx1 - fmaxx2) > d)) return CONNECT_NO;
	if((fmaxx1 < fminx2)&&((fminx2 - fmaxx1) > d)) return CONNECT_NO;
	if((fminy1 > fmaxy2)&&((fminy1 - fmaxy2) > d)) return CONNECT_NO;
	if((fmaxy1 < fminy2)&&((fminy2 - fmaxy1) > d)) return CONNECT_NO;

	if((pregion->Is_On_Me(ptrack->m_fX1, ptrack->m_fY1) == TRUE)||(pregion->Is_On_Me(ptrack->m_fX2, ptrack->m_fY2) == TRUE)) return CONNECT_YES;

	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex1, vertex2;
		vertex1 = pregion->m_cVertex.GetAt(i);
		if(i < (pregion->m_cVertex.GetCount()-1)) vertex2 = pregion->m_cVertex.GetAt(i+1);
		else vertex2 = pregion->m_cVertex.GetAt(0);

		BOOL b = Dist_Line_Line(ptrack->m_fX1, ptrack->m_fY1, ptrack->m_fX2, ptrack->m_fY2, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
		if(b == TRUE) return CONNECT_YES;
	}
	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex;
		vertex = pregion->m_cVertex.GetAt(i);

		if(vertex.radius != 0)
		{
			BOOL b = Dist_Line_Arc(ptrack->m_fX1, ptrack->m_fY1, ptrack->m_fX2, ptrack->m_fY2, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
			if(b == TRUE) return CONNECT_YES;
		}
	}

	return CONNECT_NO;
}

BOOL CESDView::Pad_Pad_Connect(COb_pcbpad* ppad1, COb_pcbpad* ppad2, double drc)
{
	double dist1, dist2, d;
	double deltax, deltay, temp;
	   
	CESDDoc* pDoc = (CESDDoc*)ppad1->pDocument;

	if((ppad1->m_nNet == ppad2->m_nNet)&&(ppad1->m_nLayer == MULTILAYER)&&(ppad2->m_nLayer ==  MULTILAYER))
	{
		int layer = TOPLAYER;
		for(int i=0; i<PCB_LAYER_NUM; i++)
		{
			if((layer >= POWERPLANE1)&&(layer <= POWERPLANE16))
			{
				if((ppad1->ConnectToInternalPlane(layer) == TRUE)&&(ppad2->ConnectToInternalPlane(layer) == TRUE)) return CONNECT_YES;

			}
			if(layer == BOTTOMLAYER) break;
			layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
		}
	}

	if(ppad1->m_nPadMode == PAD_MODE_SIMPLE)
	{
		dist1 = MAX(ppad1->m_fSizeX[TOPLAYER], ppad1->m_fSizeY[TOPLAYER]);
	}
	else 
	{
		dist1 = MAX(ppad1->m_fSizeX[TOPLAYER], ppad1->m_fSizeY[TOPLAYER]);
		dist1 = MAX(dist1, ppad1->m_fSizeX[BOTTOMLAYER]);
		dist1 = MAX(dist1, ppad1->m_fSizeY[BOTTOMLAYER]);
		dist1 = MAX(dist1, ppad1->m_fSizeX[MIDLAYER1]);
		dist1 = MAX(dist1, ppad1->m_fSizeY[MIDLAYER1]);
	}
	
	if(ppad2->m_nPadMode == PAD_MODE_SIMPLE)
	{
		dist2 = MAX(ppad2->m_fSizeX[TOPLAYER], ppad2->m_fSizeY[TOPLAYER]);
	}
	else 
	{
		dist2 = MAX(ppad2->m_fSizeX[TOPLAYER], ppad2->m_fSizeY[TOPLAYER]);
		dist2 = MAX(dist2, ppad2->m_fSizeX[BOTTOMLAYER]);
		dist2 = MAX(dist2, ppad2->m_fSizeY[BOTTOMLAYER]);
		dist2 = MAX(dist2, ppad2->m_fSizeX[MIDLAYER1]);
		dist2 = MAX(dist2, ppad2->m_fSizeY[MIDLAYER1]);
	}

	d = dist1 + dist2 + drc;

	if((ppad1->m_fCx - ppad2->m_fCx) > d) return CONNECT_NO;
	if((ppad2->m_fCx - ppad1->m_fCx) > d) return CONNECT_NO;
	if((ppad1->m_fCy - ppad2->m_fCy) > d) return CONNECT_NO;
	if((ppad2->m_fCy - ppad1->m_fCy) > d) return CONNECT_NO;

	int cycle = 0;
	int layerbuf[3] = {TOPLAYER, MIDLAYER1, BOTTOMLAYER};
	int layer;
	if((ppad1->m_nPadMode == PAD_MODE_SIMPLE)&&(ppad2->m_nPadMode == PAD_MODE_SIMPLE)) cycle = 1;
	else cycle = 3;

	for(int i=0; i<cycle; i++)
	{
		layer = layerbuf[i];
		if(ppad1->m_nShape[layer] == PAD_SHAPE_ROUND)
		{
			if(ppad1->m_fSizeX[layer] == ppad1->m_fSizeY[layer])	
			{
				if(ppad2->m_nShape[layer] == PAD_SHAPE_ROUND)
				{
					if(ppad2->m_fSizeX[layer] == ppad2->m_fSizeY[layer])	
					{
						d = ppad1->m_fSizeX[layer]/2 + ppad2->m_fSizeX[layer]/2 + drc;
						BOOL b = Dist_P_P(ppad1->m_fCx, ppad1->m_fCy, ppad2->m_fCx, ppad2->m_fCy, d);
						if(b == TRUE) return CONNECT_YES;
					}
					else if(ppad2->m_fSizeX[layer] > ppad2->m_fSizeY[layer])	
					{
						double fr, cx1, cy1, cx2, cy2;
							
						fr = ppad2->m_fSizeY[layer];

						cx1 = ppad2->m_fCx - cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						cy1 = ppad2->m_fCy - sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;

						cx2 = ppad2->m_fCx + cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						cy2 = ppad2->m_fCy + sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;

						d = ppad1->m_fSizeX[layer]/2 + fr/2 + drc;
						BOOL b = Dist_P_Line(ppad1->m_fCx, ppad1->m_fCy, cx1, cy1, cx2, cy2, d);
						if(b == TRUE) return CONNECT_YES;
					}
					else	
					{
						double fr, cx1, cy1, cx2, cy2;
						
						
						fr = ppad2->m_fSizeX[layer];

						
						cx1 = ppad2->m_fCx - sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
						cy1 = ppad2->m_fCy + cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;

						
						cx2 = ppad2->m_fCx + sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
						cy2 = ppad2->m_fCy - cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
							
						d = ppad1->m_fSizeX[layer]/2 + fr/2 + drc;
						BOOL b = Dist_P_Line(ppad1->m_fCx, ppad1->m_fCy, cx1, cy1, cx2, cy2, d);
						if(b == TRUE) return CONNECT_YES;
					}
				}
				else if(ppad2->m_nShape[layer] == PAD_SHAPE_RECTANGLE)	
				{
					d = ppad1->m_fSizeX[layer]/2 + drc;
					COb_pcbfill afill;
					afill.m_fX1 = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
					afill.m_fY1 = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;
					afill.m_fX2 = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
					afill.m_fY2 = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
					afill.m_fRotation = ppad2->m_fRotation;

					double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
					afill.GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);

					BOOL b = Dist_P_Line(ppad1->m_fCx, ppad1->m_fCy, fx1, fy1, fx3, fy3, d);
					if(b == TRUE) return CONNECT_YES;
					b = Dist_P_Line(ppad1->m_fCx, ppad1->m_fCy, fx3, fy3, fx2, fy2, d);
					if(b == TRUE) return CONNECT_YES;	
					b = Dist_P_Line(ppad1->m_fCx, ppad1->m_fCy, fx2, fy2, fx4, fy4, d);
					if(b == TRUE) return CONNECT_YES;					
					b = Dist_P_Line(ppad1->m_fCx, ppad1->m_fCy, fx4, fy4, fx1, fy1, d);
					if(b == TRUE) return CONNECT_YES;
				}
				else	
				{
					d = ppad1->m_fSizeX[layer]/2 + drc;
					if( ppad2->m_fSizeX[layer] >= ppad2->m_fSizeY[layer])
					{
						Fpoint fpoint[9];
						
						fpoint[0].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2 + ppad2->m_fSizeY[layer]/4;
						fpoint[0].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;				
						fpoint[1].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[1].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/4;
						fpoint[2].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[2].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/4;
						fpoint[3].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2 + ppad2->m_fSizeY[layer]/4;
						fpoint[3].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[4].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2 - ppad2->m_fSizeY[layer]/4;
						fpoint[4].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[5].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[5].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/4;
						fpoint[6].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[6].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/4;
						fpoint[7].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2 - ppad2->m_fSizeY[layer]/4;
						fpoint[7].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad2->m_fCx;
							fpoint[i].fy -= ppad2->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad2->m_fRotation == 0)||(ppad2->m_fRotation == 180)||(ppad2->m_fRotation == 360))
							{
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad2->m_fCy + fpoint[i].fy;
							}
							else if((ppad2->m_fRotation == 90)||(ppad2->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad2->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad2->m_fCx + fr*cos(alpha + ppad2->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad2->m_fCy + fr*sin(alpha + ppad2->m_fRotation*fEDA_pi/180);
							}
						}

						fpoint[8].fx = fpoint[0].fx;
						fpoint[8].fy = fpoint[0].fy;
						for(int i=0; i<8; i++)
						{
							BOOL b = Dist_P_Line(ppad1->m_fCx, ppad1->m_fCy, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
						}
					}
					else
					{
						Fpoint fpoint[9];
						fpoint[0].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/4;
						fpoint[0].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;
						fpoint[1].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[1].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2 + ppad2->m_fSizeX[layer]/4;
						fpoint[2].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[2].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2 - ppad2->m_fSizeX[layer]/4;
						fpoint[3].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/4;
						fpoint[3].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[4].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/4;
						fpoint[4].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[5].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[5].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2 - ppad2->m_fSizeX[layer]/4;
						fpoint[6].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[6].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2 + ppad2->m_fSizeX[layer]/4;
						fpoint[7].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/4;
						fpoint[7].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad2->m_fCx;
							fpoint[i].fy -= ppad2->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad2->m_fRotation == 0)||(ppad2->m_fRotation == 180)||(ppad2->m_fRotation == 360))
							{
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad2->m_fCy + fpoint[i].fy;
							}
							else if((ppad2->m_fRotation == 90)||(ppad2->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad2->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad2->m_fCx + fr*cos(alpha + ppad2->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad2->m_fCy + fr*sin(alpha + ppad2->m_fRotation*fEDA_pi/180);
							}
						}

						fpoint[8].fx = fpoint[0].fx;
						fpoint[8].fy = fpoint[0].fy;
						for(int i=0; i<8; i++)
						{
							BOOL b = Dist_P_Line(ppad1->m_fCx, ppad1->m_fCy, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
						}
					}
				}
			}
			else if(ppad1->m_fSizeX[layer] > ppad1->m_fSizeY[layer])	
			{
				double a_fr, a_cx1, a_cy1, a_cx2, a_cy2;
					
				
				a_fr = ppad1->m_fSizeY[layer];

				
				a_cx1 = ppad1->m_fCx - cos(ppad1->m_fRotation*fEDA_pi/180)*(ppad1->m_fSizeX[layer] - ppad1->m_fSizeY[layer])/2;
				a_cy1 = ppad1->m_fCy - sin(ppad1->m_fRotation*fEDA_pi/180)*(ppad1->m_fSizeX[layer] - ppad1->m_fSizeY[layer])/2;

				
				a_cx2 = ppad1->m_fCx + cos(ppad1->m_fRotation*fEDA_pi/180)*(ppad1->m_fSizeX[layer] - ppad1->m_fSizeY[layer])/2;
				a_cy2 = ppad1->m_fCy + sin(ppad1->m_fRotation*fEDA_pi/180)*(ppad1->m_fSizeX[layer] - ppad1->m_fSizeY[layer])/2;

				if(ppad2->m_nShape[layer] == PAD_SHAPE_ROUND)
				{
					if(ppad2->m_fSizeX[layer] == ppad2->m_fSizeY[layer])	
					{
						d = a_fr/2 + ppad2->m_fSizeX[layer]/2 + drc;
						BOOL b = Dist_P_Line(ppad2->m_fCx, ppad2->m_fCy, a_cx1, a_cy1, a_cx2, a_cy2, d);
						if(b == TRUE) return CONNECT_YES;
					}
					else if(ppad2->m_fSizeX[layer] > ppad2->m_fSizeY[layer])	
					{
						double fr, cx1, cy1, cx2, cy2;
							
						
						fr = ppad2->m_fSizeY[layer];

						
						cx1 = ppad2->m_fCx - cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						cy1 = ppad2->m_fCy - sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;

						
						cx2 = ppad2->m_fCx + cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						cy2 = ppad2->m_fCy + sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;

						d = a_fr/2 + fr/2 + drc;
						BOOL b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, cx1, cy1, cx2, cy2, d);
						if(b == TRUE) return CONNECT_YES;
					}
					else	
					{
						double fr, cx1, cy1, cx2, cy2;
						
						
						fr = ppad2->m_fSizeX[layer];

						
						cx1 = ppad2->m_fCx - sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
						cy1 = ppad2->m_fCy + cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;

						
						cx2 = ppad2->m_fCx + sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
						cy2 = ppad2->m_fCy - cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
							
						d = a_fr/2 + fr/2 + drc;
						BOOL b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, cx1, cy1, cx2, cy2, d);
						if(b == TRUE) return CONNECT_YES;
					}
				}
				else if(ppad2->m_nShape[layer] == PAD_SHAPE_RECTANGLE)	
				{
					d = a_fr/2 + drc;
					COb_pcbfill afill;
					afill.m_fX1 = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
					afill.m_fY1 = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;
					afill.m_fX2 = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
					afill.m_fY2 = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
					afill.m_fRotation = ppad2->m_fRotation;

					double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
					afill.GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);

					BOOL b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, fx1, fy1, fx3, fy3, d);
					if(b == TRUE) return CONNECT_YES;
					b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, fx3, fy3, fx2, fy2, d);
					if(b == TRUE) return CONNECT_YES;	
					b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, fx2, fy2, fx4, fy4, d);
					if(b == TRUE) return CONNECT_YES;					
					b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, fx4, fy4, fx1, fy1, d);
					if(b == TRUE) return CONNECT_YES;
				}
				else	
				{
					d = a_fr/2 + drc;
					if( ppad2->m_fSizeX[layer] >= ppad2->m_fSizeY[layer])
					{
						Fpoint fpoint[9];
						
						fpoint[0].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2 + ppad2->m_fSizeY[layer]/4;
						fpoint[0].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;				
						fpoint[1].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[1].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/4;
						fpoint[2].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[2].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/4;
						fpoint[3].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2 + ppad2->m_fSizeY[layer]/4;
						fpoint[3].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[4].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2 - ppad2->m_fSizeY[layer]/4;
						fpoint[4].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[5].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[5].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/4;
						fpoint[6].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[6].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/4;
						fpoint[7].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2 - ppad2->m_fSizeY[layer]/4;
						fpoint[7].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad2->m_fCx;
							fpoint[i].fy -= ppad2->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad2->m_fRotation == 0)||(ppad2->m_fRotation == 180)||(ppad2->m_fRotation == 360))
							{
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad2->m_fCy + fpoint[i].fy;
							}
							else if((ppad2->m_fRotation == 90)||(ppad2->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad2->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad2->m_fCx + fr*cos(alpha + ppad2->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad2->m_fCy + fr*sin(alpha + ppad2->m_fRotation*fEDA_pi/180);
							}
						}

						fpoint[8].fx = fpoint[0].fx;
						fpoint[8].fy = fpoint[0].fy;
						for(int i=0; i<8; i++)
						{
							BOOL b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
						}
					}
					else
					{
						Fpoint fpoint[9];
						fpoint[0].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/4;
						fpoint[0].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;
						fpoint[1].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[1].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2 + ppad2->m_fSizeX[layer]/4;
						fpoint[2].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[2].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2 - ppad2->m_fSizeX[layer]/4;
						fpoint[3].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/4;
						fpoint[3].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[4].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/4;
						fpoint[4].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[5].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[5].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2 - ppad2->m_fSizeX[layer]/4;
						fpoint[6].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[6].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2 + ppad2->m_fSizeX[layer]/4;
						fpoint[7].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/4;
						fpoint[7].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad2->m_fCx;
							fpoint[i].fy -= ppad2->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad2->m_fRotation == 0)||(ppad2->m_fRotation == 180)||(ppad2->m_fRotation == 360))
							{
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad2->m_fCy + fpoint[i].fy;
							}
							else if((ppad2->m_fRotation == 90)||(ppad2->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad2->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad2->m_fCx + fr*cos(alpha + ppad2->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad2->m_fCy + fr*sin(alpha + ppad2->m_fRotation*fEDA_pi/180);
							}
						}

						fpoint[8].fx = fpoint[0].fx;
						fpoint[8].fy = fpoint[0].fy;
						for(int i=0; i<8; i++)
						{
							BOOL b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
						}
					}
				}
			}
			else	
			{
				double a_fr, a_cx1, a_cy1, a_cx2, a_cy2;
				
				
				a_fr = ppad1->m_fSizeX[layer];

				
				a_cx1 = ppad1->m_fCx - sin(ppad1->m_fRotation*fEDA_pi/180)*(ppad1->m_fSizeY[layer] - ppad1->m_fSizeX[layer])/2;
				a_cy1 = ppad1->m_fCy + cos(ppad1->m_fRotation*fEDA_pi/180)*(ppad1->m_fSizeY[layer] - ppad1->m_fSizeX[layer])/2;

				
				a_cx2 = ppad1->m_fCx + sin(ppad1->m_fRotation*fEDA_pi/180)*(ppad1->m_fSizeY[layer] - ppad1->m_fSizeX[layer])/2;
				a_cy2 = ppad1->m_fCy - cos(ppad1->m_fRotation*fEDA_pi/180)*(ppad1->m_fSizeY[layer] - ppad1->m_fSizeX[layer])/2;
				
				if(ppad2->m_nShape[layer] == PAD_SHAPE_ROUND)
				{
					if(ppad2->m_fSizeX[layer] == ppad2->m_fSizeY[layer])	
					{
						d = a_fr/2 + ppad2->m_fSizeX[layer]/2 + drc;
						BOOL b = Dist_P_Line(ppad2->m_fCx, ppad2->m_fCy, a_cx1, a_cy1, a_cx2, a_cy2, d);
						if(b == TRUE) return CONNECT_YES;
					}
					else if(ppad2->m_fSizeX[layer] > ppad2->m_fSizeY[layer])	
					{
						double fr, cx1, cy1, cx2, cy2;
							
						
						fr = ppad2->m_fSizeY[layer];

						
						cx1 = ppad2->m_fCx - cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						cy1 = ppad2->m_fCy - sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;

						
						cx2 = ppad2->m_fCx + cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						cy2 = ppad2->m_fCy + sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;

						d = a_fr/2 + fr/2 + drc;
						BOOL b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, cx1, cy1, cx2, cy2, d);
						if(b == TRUE) return CONNECT_YES;
					}
					else	
					{
						double fr, cx1, cy1, cx2, cy2;
						
						
						fr = ppad2->m_fSizeX[layer];

						
						cx1 = ppad2->m_fCx - sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
						cy1 = ppad2->m_fCy + cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;

						
						cx2 = ppad2->m_fCx + sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
						cy2 = ppad2->m_fCy - cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
							
						d = a_fr/2 + fr/2 + drc;
						BOOL b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, cx1, cy1, cx2, cy2, d);
						if(b == TRUE) return CONNECT_YES;
					}
				}
				else if(ppad2->m_nShape[layer] == PAD_SHAPE_RECTANGLE)	
				{
					d = a_fr/2 + drc;
					COb_pcbfill afill;
					afill.m_fX1 = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
					afill.m_fY1 = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;
					afill.m_fX2 = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
					afill.m_fY2 = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
					afill.m_fRotation = ppad2->m_fRotation;

					double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
					afill.GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);

					BOOL b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, fx1, fy1, fx3, fy3, d);
					if(b == TRUE) return CONNECT_YES;
					b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, fx3, fy3, fx2, fy2, d);
					if(b == TRUE) return CONNECT_YES;	
					b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, fx2, fy2, fx4, fy4, d);
					if(b == TRUE) return CONNECT_YES;					
					b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, fx4, fy4, fx1, fy1, d);
					if(b == TRUE) return CONNECT_YES;
				}
				else	
				{
					d = a_fr/2 + drc;
					if( ppad2->m_fSizeX[layer] >= ppad2->m_fSizeY[layer])
					{
						Fpoint fpoint[9];
						
						fpoint[0].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2 + ppad2->m_fSizeY[layer]/4;
						fpoint[0].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;				
						fpoint[1].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[1].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/4;
						fpoint[2].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[2].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/4;
						fpoint[3].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2 + ppad2->m_fSizeY[layer]/4;
						fpoint[3].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[4].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2 - ppad2->m_fSizeY[layer]/4;
						fpoint[4].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[5].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[5].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/4;
						fpoint[6].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[6].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/4;
						fpoint[7].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2 - ppad2->m_fSizeY[layer]/4;
						fpoint[7].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad2->m_fCx;
							fpoint[i].fy -= ppad2->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad2->m_fRotation == 0)||(ppad2->m_fRotation == 180)||(ppad2->m_fRotation == 360))
							{
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad2->m_fCy + fpoint[i].fy;
							}
							else if((ppad2->m_fRotation == 90)||(ppad2->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad2->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad2->m_fCx + fr*cos(alpha + ppad2->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad2->m_fCy + fr*sin(alpha + ppad2->m_fRotation*fEDA_pi/180);
							}
						}

						fpoint[8].fx = fpoint[0].fx;
						fpoint[8].fy = fpoint[0].fy;
						for(int i=0; i<8; i++)
						{
							BOOL b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
						}
					}
					else
					{
						Fpoint fpoint[9];
						fpoint[0].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/4;
						fpoint[0].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;
						fpoint[1].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[1].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2 + ppad2->m_fSizeX[layer]/4;
						fpoint[2].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[2].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2 - ppad2->m_fSizeX[layer]/4;
						fpoint[3].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/4;
						fpoint[3].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[4].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/4;
						fpoint[4].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[5].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[5].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2 - ppad2->m_fSizeX[layer]/4;
						fpoint[6].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[6].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2 + ppad2->m_fSizeX[layer]/4;
						fpoint[7].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/4;
						fpoint[7].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad2->m_fCx;
							fpoint[i].fy -= ppad2->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad2->m_fRotation == 0)||(ppad2->m_fRotation == 180)||(ppad2->m_fRotation == 360))
							{
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad2->m_fCy + fpoint[i].fy;
							}
							else if((ppad2->m_fRotation == 90)||(ppad2->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad2->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad2->m_fCx + fr*cos(alpha + ppad2->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad2->m_fCy + fr*sin(alpha + ppad2->m_fRotation*fEDA_pi/180);
							}
						}

						fpoint[8].fx = fpoint[0].fx;
						fpoint[8].fy = fpoint[0].fy;
						for(int i=0; i<8; i++)
						{
							BOOL b = Dist_Line_Line(a_cx1, a_cy1, a_cx2, a_cy2, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
						}
					}
				}					
			}
		}
		else if(ppad1->m_nShape[layer] == PAD_SHAPE_RECTANGLE)
		{
			COb_pcbfill afill;
			afill.m_fX1 = ppad1->m_fCx - ppad1->m_fSizeX[layer]/2;
			afill.m_fY1 = ppad1->m_fCy - ppad1->m_fSizeY[layer]/2;
			afill.m_fX2 = ppad1->m_fCx + ppad1->m_fSizeX[layer]/2;
			afill.m_fY2 = ppad1->m_fCy + ppad1->m_fSizeY[layer]/2;
			afill.m_fRotation = ppad1->m_fRotation;

			double a_fx1, a_fy1, a_fx2, a_fy2, a_fx3, a_fy3, a_fx4, a_fy4;
			afill.GetFillCorner(&a_fx1, &a_fy1, &a_fx2, &a_fy2, &a_fx3, &a_fy3, &a_fx4, &a_fy4);
				
				if(ppad2->m_nShape[layer] == PAD_SHAPE_ROUND)
				{
					if(ppad2->m_fSizeX[layer] == ppad2->m_fSizeY[layer])	
					{
						d = ppad2->m_fSizeX[layer]/2 + drc;
						BOOL b = Dist_P_Line(ppad2->m_fCx, ppad2->m_fCy, a_fx1, a_fy1, a_fx3, a_fy3, d);
						if(b == TRUE) return CONNECT_YES;
						b = Dist_P_Line(ppad2->m_fCx, ppad2->m_fCy, a_fx3, a_fy3, a_fx2, a_fy2, d);
						if(b == TRUE) return CONNECT_YES;	
						b = Dist_P_Line(ppad2->m_fCx, ppad2->m_fCy, a_fx2, a_fy2, a_fx4, a_fy4, d);
						if(b == TRUE) return CONNECT_YES;					
						b = Dist_P_Line(ppad2->m_fCx, ppad2->m_fCy, a_fx4, a_fy4, a_fx1, a_fy1, d);
						if(b == TRUE) return CONNECT_YES;
					}
					else if(ppad2->m_fSizeX[layer] > ppad2->m_fSizeY[layer])	
					{
						double fr, cx1, cy1, cx2, cy2;
							
						
						fr = ppad2->m_fSizeY[layer];

						
						cx1 = ppad2->m_fCx - cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						cy1 = ppad2->m_fCy - sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;

						
						cx2 = ppad2->m_fCx + cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						cy2 = ppad2->m_fCy + sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						
						d = fr/2 + drc;
						BOOL b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, cx1, cy1, cx2, cy2, d);
						if(b == TRUE) return CONNECT_YES;
						b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, cx1, cy1, cx2, cy2, d);
						if(b == TRUE) return CONNECT_YES;	
						b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, cx1, cy1, cx2, cy2, d);
						if(b == TRUE) return CONNECT_YES;					
						b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, cx1, cy1, cx2, cy2, d);
						if(b == TRUE) return CONNECT_YES;
					}
					else	
					{
						double fr, cx1, cy1, cx2, cy2;
						
						
						fr = ppad2->m_fSizeX[layer];

						
						cx1 = ppad2->m_fCx - sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
						cy1 = ppad2->m_fCy + cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;

						
						cx2 = ppad2->m_fCx + sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
						cy2 = ppad2->m_fCy - cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
							
						d = fr/2 + drc;
						BOOL b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, cx1, cy1, cx2, cy2, d);
						if(b == TRUE) return CONNECT_YES;
						b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, cx1, cy1, cx2, cy2, d);
						if(b == TRUE) return CONNECT_YES;	
						b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, cx1, cy1, cx2, cy2, d);
						if(b == TRUE) return CONNECT_YES;					
						b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, cx1, cy1, cx2, cy2, d);
						if(b == TRUE) return CONNECT_YES;
					}
				}
				else if(ppad2->m_nShape[layer] == PAD_SHAPE_RECTANGLE)	
				{
					afill.m_fX1 = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
					afill.m_fY1 = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;
					afill.m_fX2 = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
					afill.m_fY2 = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
					afill.m_fRotation = ppad2->m_fRotation;

					double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
					afill.GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);

					d = drc;
					BOOL b;
					b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, fx1, fy1, fx3, fy3, d);
					if(b == TRUE) return CONNECT_YES;
					b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, fx1, fy1, fx3, fy3, d);
					if(b == TRUE) return CONNECT_YES;	
					b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, fx1, fy1, fx3, fy3, d);
					if(b == TRUE) return CONNECT_YES;					
					b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, fx1, fy1, fx3, fy3, d);
					if(b == TRUE) return CONNECT_YES;
					
					b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, fx3, fy3, fx2, fy2, d);
					if(b == TRUE) return CONNECT_YES;
					b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, fx3, fy3, fx2, fy2, d);
					if(b == TRUE) return CONNECT_YES;	
					b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, fx3, fy3, fx2, fy2, d);
					if(b == TRUE) return CONNECT_YES;					
					b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, fx3, fy3, fx2, fy2, d);
					if(b == TRUE) return CONNECT_YES;

					b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, fx2, fy2, fx4, fy4, d);
					if(b == TRUE) return CONNECT_YES;
					b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, fx2, fy2, fx4, fy4, d);
					if(b == TRUE) return CONNECT_YES;	
					b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, fx2, fy2, fx4, fy4, d);
					if(b == TRUE) return CONNECT_YES;					
					b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, fx2, fy2, fx4, fy4, d);
					if(b == TRUE) return CONNECT_YES;

					b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, fx4, fy4, fx1, fy1, d);
					if(b == TRUE) return CONNECT_YES;
					b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, fx4, fy4, fx1, fy1, d);
					if(b == TRUE) return CONNECT_YES;	
					b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, fx4, fy4, fx1, fy1, d);
					if(b == TRUE) return CONNECT_YES;					
					b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, fx4, fy4, fx1, fy1, d);
					if(b == TRUE) return CONNECT_YES;

				}
				else	
				{
					d = drc;
					if( ppad2->m_fSizeX[layer] >= ppad2->m_fSizeY[layer])
					{
						Fpoint fpoint[9];
						
						fpoint[0].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2 + ppad2->m_fSizeY[layer]/4;
						fpoint[0].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;				
						fpoint[1].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[1].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/4;
						fpoint[2].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[2].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/4;
						fpoint[3].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2 + ppad2->m_fSizeY[layer]/4;
						fpoint[3].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[4].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2 - ppad2->m_fSizeY[layer]/4;
						fpoint[4].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[5].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[5].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/4;
						fpoint[6].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[6].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/4;
						fpoint[7].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2 - ppad2->m_fSizeY[layer]/4;
						fpoint[7].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad2->m_fCx;
							fpoint[i].fy -= ppad2->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad2->m_fRotation == 0)||(ppad2->m_fRotation == 180)||(ppad2->m_fRotation == 360))
							{
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad2->m_fCy + fpoint[i].fy;
							}
							else if((ppad2->m_fRotation == 90)||(ppad2->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad2->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad2->m_fCx + fr*cos(alpha + ppad2->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad2->m_fCy + fr*sin(alpha + ppad2->m_fRotation*fEDA_pi/180);
							}
						}

						fpoint[8].fx = fpoint[0].fx;
						fpoint[8].fy = fpoint[0].fy;
						for(int i=0; i<8; i++)
						{
							BOOL b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
							b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
							b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
							b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
						}

					}
					else
					{
						Fpoint fpoint[9];
						fpoint[0].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/4;
						fpoint[0].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;
						fpoint[1].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[1].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2 + ppad2->m_fSizeX[layer]/4;
						fpoint[2].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[2].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2 - ppad2->m_fSizeX[layer]/4;
						fpoint[3].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/4;
						fpoint[3].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[4].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/4;
						fpoint[4].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[5].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[5].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2 - ppad2->m_fSizeX[layer]/4;
						fpoint[6].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[6].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2 + ppad2->m_fSizeX[layer]/4;
						fpoint[7].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/4;
						fpoint[7].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad2->m_fCx;
							fpoint[i].fy -= ppad2->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad2->m_fRotation == 0)||(ppad2->m_fRotation == 180)||(ppad2->m_fRotation == 360))
							{
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad2->m_fCy + fpoint[i].fy;
							}
							else if((ppad2->m_fRotation == 90)||(ppad2->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad2->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad2->m_fCx + fr*cos(alpha + ppad2->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad2->m_fCy + fr*sin(alpha + ppad2->m_fRotation*fEDA_pi/180);
							}
						}

						fpoint[8].fx = fpoint[0].fx;
						fpoint[8].fy = fpoint[0].fy;
						for(int i=0; i<8; i++)
						{
							BOOL b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
							b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
							b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
							b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
						}
					}
				}
		}
		else
		{
			if( ppad1->m_fSizeX[layer] >= ppad1->m_fSizeY[layer])
			{
				Fpoint a_fpoint[9];
				
				a_fpoint[0].fx = ppad1->m_fCx - ppad1->m_fSizeX[layer]/2 + ppad1->m_fSizeY[layer]/4;
				a_fpoint[0].fy = ppad1->m_fCy - ppad1->m_fSizeY[layer]/2;				
				a_fpoint[1].fx = ppad1->m_fCx - ppad1->m_fSizeX[layer]/2;
				a_fpoint[1].fy = ppad1->m_fCy - ppad1->m_fSizeY[layer]/4;
				a_fpoint[2].fx = ppad1->m_fCx - ppad1->m_fSizeX[layer]/2;
				a_fpoint[2].fy = ppad1->m_fCy + ppad1->m_fSizeY[layer]/4;
				a_fpoint[3].fx = ppad1->m_fCx - ppad1->m_fSizeX[layer]/2 + ppad1->m_fSizeY[layer]/4;
				a_fpoint[3].fy = ppad1->m_fCy + ppad1->m_fSizeY[layer]/2;
				a_fpoint[4].fx = ppad1->m_fCx + ppad1->m_fSizeX[layer]/2 - ppad1->m_fSizeY[layer]/4;
				a_fpoint[4].fy = ppad1->m_fCy + ppad1->m_fSizeY[layer]/2;
				a_fpoint[5].fx = ppad1->m_fCx + ppad1->m_fSizeX[layer]/2;
				a_fpoint[5].fy = ppad1->m_fCy + ppad1->m_fSizeY[layer]/4;
				a_fpoint[6].fx = ppad1->m_fCx + ppad1->m_fSizeX[layer]/2;
				a_fpoint[6].fy = ppad1->m_fCy - ppad1->m_fSizeY[layer]/4;
				a_fpoint[7].fx = ppad1->m_fCx + ppad1->m_fSizeX[layer]/2 - ppad1->m_fSizeY[layer]/4;
				a_fpoint[7].fy = ppad1->m_fCy - ppad1->m_fSizeY[layer]/2;

				
				for(int i=0; i<8; i++)
				{
					a_fpoint[i].fx -= ppad1->m_fCx;
					a_fpoint[i].fy -= ppad1->m_fCy;
				}

				double alpha, fr;
				for(int i=0; i<8; i++)
				{
					if((ppad1->m_fRotation == 0)||(ppad1->m_fRotation == 180)||(ppad1->m_fRotation == 360))
					{
						a_fpoint[i].fx = ppad1->m_fCx + a_fpoint[i].fx;
						a_fpoint[i].fy = ppad1->m_fCy + a_fpoint[i].fy;
					}
					else if((ppad1->m_fRotation == 90)||(ppad1->m_fRotation == 270))
					{
						double temp;
						temp = a_fpoint[i].fx;
						a_fpoint[i].fx = ppad1->m_fCx + a_fpoint[i].fy;
						a_fpoint[i].fy = ppad1->m_fCy + temp;
					}
					else
					{
						alpha = atan2(a_fpoint[i].fy, a_fpoint[i].fx);
						fr = sqrt(a_fpoint[i].fy*a_fpoint[i].fy + a_fpoint[i].fx*a_fpoint[i].fx);
						a_fpoint[i].fx = ppad1->m_fCx + fr*cos(alpha + ppad1->m_fRotation*fEDA_pi/180);
						a_fpoint[i].fy = ppad1->m_fCy + fr*sin(alpha + ppad1->m_fRotation*fEDA_pi/180);
					}
				}

				a_fpoint[8].fx = a_fpoint[0].fx;
				a_fpoint[8].fy = a_fpoint[0].fy;

				if(ppad2->m_nShape[layer] == PAD_SHAPE_ROUND)
				{
					if(ppad2->m_fSizeX[layer] == ppad2->m_fSizeY[layer])	
					{
						d = ppad2->m_fSizeX[layer]/2 + drc;
						for(int i=0; i<8; i++)
						{
							BOOL b = Dist_P_Line(ppad2->m_fCx, ppad2->m_fCy, a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
						}
					}
					else if(ppad2->m_fSizeX[layer] > ppad2->m_fSizeY[layer])	
					{
						double fr, cx1, cy1, cx2, cy2;
							
						
						fr = ppad2->m_fSizeY[layer];

						
						cx1 = ppad2->m_fCx - cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						cy1 = ppad2->m_fCy - sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;

						
						cx2 = ppad2->m_fCx + cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						cy2 = ppad2->m_fCy + sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						
						d = fr/2 + drc;
						for(int i=0; i<8; i++)
						{
							BOOL b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, cx1, cy1, cx2, cy2, d);
							if(b == TRUE) return CONNECT_YES;
						}
					}
					else	
					{
						double fr, cx1, cy1, cx2, cy2;
						
						
						fr = ppad2->m_fSizeX[layer];

						
						cx1 = ppad2->m_fCx - sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
						cy1 = ppad2->m_fCy + cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;

						
						cx2 = ppad2->m_fCx + sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
						cy2 = ppad2->m_fCy - cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
							
						d = fr/2 + drc;
						for(int i=0; i<8; i++)
						{
							BOOL b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, cx1, cy1, cx2, cy2, d);
							if(b == TRUE) return CONNECT_YES;
						}						
					}
				}
				else if(ppad2->m_nShape[layer] == PAD_SHAPE_RECTANGLE)	
				{
					COb_pcbfill afill;
					afill.m_fX1 = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
					afill.m_fY1 = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;
					afill.m_fX2 = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
					afill.m_fY2 = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
					afill.m_fRotation = ppad2->m_fRotation;

					double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
					afill.GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);

					d = drc;
					for(int i=0; i<8; i++)
					{
						BOOL b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, fx1, fy1, fx3, fy3, d);
						if(b == TRUE) return CONNECT_YES;
						b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, fx3, fy3, fx2, fy2, d);
						if(b == TRUE) return CONNECT_YES;
						b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, fx2, fy2, fx4, fy4, d);
						if(b == TRUE) return CONNECT_YES;
						b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, fx4, fy4, fx1, fy1, d);
						if(b == TRUE) return CONNECT_YES;
					}	
				}
				else	
				{
					d = drc;
					if( ppad2->m_fSizeX[layer] >= ppad2->m_fSizeY[layer])
					{
						Fpoint fpoint[9];
						
						fpoint[0].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2 + ppad2->m_fSizeY[layer]/4;
						fpoint[0].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;				
						fpoint[1].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[1].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/4;
						fpoint[2].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[2].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/4;
						fpoint[3].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2 + ppad2->m_fSizeY[layer]/4;
						fpoint[3].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[4].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2 - ppad2->m_fSizeY[layer]/4;
						fpoint[4].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[5].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[5].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/4;
						fpoint[6].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[6].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/4;
						fpoint[7].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2 - ppad2->m_fSizeY[layer]/4;
						fpoint[7].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad2->m_fCx;
							fpoint[i].fy -= ppad2->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad2->m_fRotation == 0)||(ppad2->m_fRotation == 180)||(ppad2->m_fRotation == 360))
							{
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad2->m_fCy + fpoint[i].fy;
							}
							else if((ppad2->m_fRotation == 90)||(ppad2->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad2->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad2->m_fCx + fr*cos(alpha + ppad2->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad2->m_fCy + fr*sin(alpha + ppad2->m_fRotation*fEDA_pi/180);
							}
						}

						fpoint[8].fx = fpoint[0].fx;
						fpoint[8].fy = fpoint[0].fy;
						for(int i=0; i<8; i++)
						{
							for(int j=0; j<8; j++)
							{
								BOOL b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, fpoint[j].fx, fpoint[j].fy, fpoint[j+1].fx, fpoint[j+1].fy, d);
								if(b == TRUE) return CONNECT_YES;
							}
						}	
					}
					else
					{
						Fpoint fpoint[9];
						fpoint[0].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/4;
						fpoint[0].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;
						fpoint[1].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[1].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2 + ppad2->m_fSizeX[layer]/4;
						fpoint[2].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[2].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2 - ppad2->m_fSizeX[layer]/4;
						fpoint[3].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/4;
						fpoint[3].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[4].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/4;
						fpoint[4].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[5].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[5].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2 - ppad2->m_fSizeX[layer]/4;
						fpoint[6].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[6].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2 + ppad2->m_fSizeX[layer]/4;
						fpoint[7].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/4;
						fpoint[7].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad2->m_fCx;
							fpoint[i].fy -= ppad2->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad2->m_fRotation == 0)||(ppad2->m_fRotation == 180)||(ppad2->m_fRotation == 360))
							{
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad2->m_fCy + fpoint[i].fy;
							}
							else if((ppad2->m_fRotation == 90)||(ppad2->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad2->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad2->m_fCx + fr*cos(alpha + ppad2->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad2->m_fCy + fr*sin(alpha + ppad2->m_fRotation*fEDA_pi/180);
							}
						}

						fpoint[8].fx = fpoint[0].fx;
						fpoint[8].fy = fpoint[0].fy;
						for(int i=0; i<8; i++)
						{
							for(int j=0; j<8; j++)
							{
								BOOL b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, fpoint[j].fx, fpoint[j].fy, fpoint[j+1].fx, fpoint[j+1].fy, d);
								if(b == TRUE) return CONNECT_YES;
							}
						}	
					}
				}
			}
			else
			{
				Fpoint a_fpoint[9];
				a_fpoint[0].fx = ppad1->m_fCx - ppad1->m_fSizeX[layer]/4;
				a_fpoint[0].fy = ppad1->m_fCy - ppad1->m_fSizeY[layer]/2;
				a_fpoint[1].fx = ppad1->m_fCx - ppad1->m_fSizeX[layer]/2;
				a_fpoint[1].fy = ppad1->m_fCy - ppad1->m_fSizeY[layer]/2 + ppad1->m_fSizeX[layer]/4;
				a_fpoint[2].fx = ppad1->m_fCx - ppad1->m_fSizeX[layer]/2;
				a_fpoint[2].fy = ppad1->m_fCy + ppad1->m_fSizeY[layer]/2 - ppad1->m_fSizeX[layer]/4;
				a_fpoint[3].fx = ppad1->m_fCx - ppad1->m_fSizeX[layer]/4;
				a_fpoint[3].fy = ppad1->m_fCy + ppad1->m_fSizeY[layer]/2;
				a_fpoint[4].fx = ppad1->m_fCx + ppad1->m_fSizeX[layer]/4;
				a_fpoint[4].fy = ppad1->m_fCy + ppad1->m_fSizeY[layer]/2;
				a_fpoint[5].fx = ppad1->m_fCx + ppad1->m_fSizeX[layer]/2;
				a_fpoint[5].fy = ppad1->m_fCy + ppad1->m_fSizeY[layer]/2 - ppad1->m_fSizeX[layer]/4;
				a_fpoint[6].fx = ppad1->m_fCx + ppad1->m_fSizeX[layer]/2;
				a_fpoint[6].fy = ppad1->m_fCy - ppad1->m_fSizeY[layer]/2 + ppad1->m_fSizeX[layer]/4;
				a_fpoint[7].fx = ppad1->m_fCx + ppad1->m_fSizeX[layer]/4;
				a_fpoint[7].fy = ppad1->m_fCy - ppad1->m_fSizeY[layer]/2;

				
				for(int i=0; i<8; i++)
				{
					a_fpoint[i].fx -= ppad1->m_fCx;
					a_fpoint[i].fy -= ppad1->m_fCy;
				}

				double alpha, fr;
				for(int i=0; i<8; i++)
				{
					if((ppad1->m_fRotation == 0)||(ppad1->m_fRotation == 180)||(ppad1->m_fRotation == 360))
					{
						a_fpoint[i].fx = ppad1->m_fCx + a_fpoint[i].fx;
						a_fpoint[i].fy = ppad1->m_fCy + a_fpoint[i].fy;
					}
					else if((ppad1->m_fRotation == 90)||(ppad1->m_fRotation == 270))
					{
						double temp;
						temp = a_fpoint[i].fx;
						a_fpoint[i].fx = ppad1->m_fCx + a_fpoint[i].fy;
						a_fpoint[i].fy = ppad1->m_fCy + temp;
					}
					else
					{
						alpha = atan2(a_fpoint[i].fy, a_fpoint[i].fx);
						fr = sqrt(a_fpoint[i].fy*a_fpoint[i].fy + a_fpoint[i].fx*a_fpoint[i].fx);
						a_fpoint[i].fx = ppad1->m_fCx + fr*cos(alpha + ppad1->m_fRotation*fEDA_pi/180);
						a_fpoint[i].fy = ppad1->m_fCy + fr*sin(alpha + ppad1->m_fRotation*fEDA_pi/180);
					}
				}

				a_fpoint[8].fx = a_fpoint[0].fx;
				a_fpoint[8].fy = a_fpoint[0].fy;
				if(ppad2->m_nShape[layer] == PAD_SHAPE_ROUND)
				{
					if(ppad2->m_fSizeX[layer] == ppad2->m_fSizeY[layer])	
					{
						d = ppad2->m_fSizeX[layer]/2 + drc;
						for(int i=0; i<8; i++)
						{
							BOOL b = Dist_P_Line(ppad2->m_fCx, ppad2->m_fCy, a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, d);
							if(b == TRUE) return CONNECT_YES;
						}
					}
					else if(ppad2->m_fSizeX[layer] > ppad2->m_fSizeY[layer])	
					{
						double fr, cx1, cy1, cx2, cy2;
							
						
						fr = ppad2->m_fSizeY[layer];

						
						cx1 = ppad2->m_fCx - cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						cy1 = ppad2->m_fCy - sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;

						
						cx2 = ppad2->m_fCx + cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						cy2 = ppad2->m_fCy + sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeX[layer] - ppad2->m_fSizeY[layer])/2;
						
						d = fr/2 + drc;
						for(int i=0; i<8; i++)
						{
							BOOL b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, cx1, cy1, cx2, cy2, d);
							if(b == TRUE) return CONNECT_YES;
						}
					}
					else	
					{
						double fr, cx1, cy1, cx2, cy2;
						
						
						fr = ppad2->m_fSizeX[layer];

						
						cx1 = ppad2->m_fCx - sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
						cy1 = ppad2->m_fCy + cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;

						
						cx2 = ppad2->m_fCx + sin(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
						cy2 = ppad2->m_fCy - cos(ppad2->m_fRotation*fEDA_pi/180)*(ppad2->m_fSizeY[layer] - ppad2->m_fSizeX[layer])/2;
							
						d = fr/2 + drc;
						for(int i=0; i<8; i++)
						{
							BOOL b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, cx1, cy1, cx2, cy2, d);
							if(b == TRUE) return CONNECT_YES;
						}						
					}
				}
				else if(ppad2->m_nShape[layer] == PAD_SHAPE_RECTANGLE)	
				{
					COb_pcbfill afill;
					afill.m_fX1 = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
					afill.m_fY1 = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;
					afill.m_fX2 = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
					afill.m_fY2 = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
					afill.m_fRotation = ppad2->m_fRotation;

					double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
					afill.GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);

					d = drc;
					for(int i=0; i<8; i++)
					{
						BOOL b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, fx1, fy1, fx3, fy3, d);
						if(b == TRUE) return CONNECT_YES;
						b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, fx3, fy3, fx2, fy2, d);
						if(b == TRUE) return CONNECT_YES;
						b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, fx2, fy2, fx4, fy4, d);
						if(b == TRUE) return CONNECT_YES;
						b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, fx4, fy4, fx1, fy1, d);
						if(b == TRUE) return CONNECT_YES;
					}	
				}
				else	
				{
					d = drc;
					if( ppad2->m_fSizeX[layer] >= ppad2->m_fSizeY[layer])
					{
						Fpoint fpoint[9];
						
						fpoint[0].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2 + ppad2->m_fSizeY[layer]/4;
						fpoint[0].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;				
						fpoint[1].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[1].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/4;
						fpoint[2].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[2].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/4;
						fpoint[3].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2 + ppad2->m_fSizeY[layer]/4;
						fpoint[3].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[4].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2 - ppad2->m_fSizeY[layer]/4;
						fpoint[4].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[5].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[5].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/4;
						fpoint[6].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[6].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/4;
						fpoint[7].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2 - ppad2->m_fSizeY[layer]/4;
						fpoint[7].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad2->m_fCx;
							fpoint[i].fy -= ppad2->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad2->m_fRotation == 0)||(ppad2->m_fRotation == 180)||(ppad2->m_fRotation == 360))
							{
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad2->m_fCy + fpoint[i].fy;
							}
							else if((ppad2->m_fRotation == 90)||(ppad2->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad2->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad2->m_fCx + fr*cos(alpha + ppad2->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad2->m_fCy + fr*sin(alpha + ppad2->m_fRotation*fEDA_pi/180);
							}
						}

						fpoint[8].fx = fpoint[0].fx;
						fpoint[8].fy = fpoint[0].fy;
						for(int i=0; i<8; i++)
						{
							for(int j=0; j<8; j++)
							{
								BOOL b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, fpoint[j].fx, fpoint[j].fy, fpoint[j+1].fx, fpoint[j+1].fy, d);
								if(b == TRUE) return CONNECT_YES;
							}
						}	
					}
					else
					{
						Fpoint fpoint[9];
						fpoint[0].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/4;
						fpoint[0].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;
						fpoint[1].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[1].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2 + ppad2->m_fSizeX[layer]/4;
						fpoint[2].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/2;
						fpoint[2].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2 - ppad2->m_fSizeX[layer]/4;
						fpoint[3].fx = ppad2->m_fCx - ppad2->m_fSizeX[layer]/4;
						fpoint[3].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[4].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/4;
						fpoint[4].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2;
						fpoint[5].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[5].fy = ppad2->m_fCy + ppad2->m_fSizeY[layer]/2 - ppad2->m_fSizeX[layer]/4;
						fpoint[6].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/2;
						fpoint[6].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2 + ppad2->m_fSizeX[layer]/4;
						fpoint[7].fx = ppad2->m_fCx + ppad2->m_fSizeX[layer]/4;
						fpoint[7].fy = ppad2->m_fCy - ppad2->m_fSizeY[layer]/2;

						
						for(int i=0; i<8; i++)
						{
							fpoint[i].fx -= ppad2->m_fCx;
							fpoint[i].fy -= ppad2->m_fCy;
						}

						double alpha, fr;
						for(int i=0; i<8; i++)
						{
							if((ppad2->m_fRotation == 0)||(ppad2->m_fRotation == 180)||(ppad2->m_fRotation == 360))
							{
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fx;
								fpoint[i].fy = ppad2->m_fCy + fpoint[i].fy;
							}
							else if((ppad2->m_fRotation == 90)||(ppad2->m_fRotation == 270))
							{
								double temp;
								temp = fpoint[i].fx;
								fpoint[i].fx = ppad2->m_fCx + fpoint[i].fy;
								fpoint[i].fy = ppad2->m_fCy + temp;
							}
							else
							{
								alpha = atan2(fpoint[i].fy, fpoint[i].fx);
								fr = sqrt(fpoint[i].fy*fpoint[i].fy+fpoint[i].fx*fpoint[i].fx);
								fpoint[i].fx = ppad2->m_fCx + fr*cos(alpha + ppad2->m_fRotation*fEDA_pi/180);
								fpoint[i].fy = ppad2->m_fCy + fr*sin(alpha + ppad2->m_fRotation*fEDA_pi/180);
							}
						}

						fpoint[8].fx = fpoint[0].fx;
						fpoint[8].fy = fpoint[0].fy;
						for(int i=0; i<8; i++)
						{
							for(int j=0; j<8; j++)
							{
								BOOL b = Dist_Line_Line(a_fpoint[i].fx, a_fpoint[i].fy, a_fpoint[i+1].fx, a_fpoint[i+1].fy, fpoint[j].fx, fpoint[j].fy, fpoint[j+1].fx, fpoint[j+1].fy, d);
								if(b == TRUE) return CONNECT_YES;
							}
						}	
					}
				}
			}
		}
	}
	
	return CONNECT_NO;
}

BOOL CESDView::Pad_Via_Connect(COb_pcbpad* ppad, COb_pcbvia* pvia, double drc)
{
	double dist, d;
	double deltax, deltay, temp;

	CESDDoc* pDoc = (CESDDoc*)ppad->pDocument;
	
	if((ppad->m_nLayer == MULTILAYER)&&(ppad->m_nNet == pvia->m_nNet))
	{
		int layer = TOPLAYER;
		for(int i=0; i<PCB_LAYER_NUM; i++)
		{
			if((layer >= POWERPLANE1)&&(layer <= POWERPLANE16))
			{
				if((ppad->ConnectToInternalPlane(layer) == TRUE)&&(pvia->ConnectToInternalPlane(layer) == TRUE)) return CONNECT_YES;

			}
			if(layer == BOTTOMLAYER) break;
			layer = pDoc->m_sPCB_LayerParam[layer].nLayerNext;
		}
	}
	   
	
	if(ppad->m_nPadMode == PAD_MODE_SIMPLE)
	{
		dist = MAX(ppad->m_fSizeX[TOPLAYER], ppad->m_fSizeY[TOPLAYER]);
	}
	else 
	{
		dist = MAX(ppad->m_fSizeX[TOPLAYER], ppad->m_fSizeY[TOPLAYER]);
		dist = MAX(dist, ppad->m_fSizeX[BOTTOMLAYER]);
		dist = MAX(dist, ppad->m_fSizeY[BOTTOMLAYER]);
		dist = MAX(dist, ppad->m_fSizeX[MIDLAYER1]);
		dist = MAX(dist, ppad->m_fSizeY[MIDLAYER1]);
	}
	
	d = dist + pvia->m_fDia/2 + drc;
	if((ppad->m_fCx - pvia->m_fCx) > d) return CONNECT_NO;
	if((pvia->m_fCx - ppad->m_fCx) > d) return CONNECT_NO;
	if((ppad->m_fCy - pvia->m_fCy) > d) return CONNECT_NO;
	if((pvia->m_fCy - ppad->m_fCy) > d) return CONNECT_NO;

	int cycle = 0;
	int layerbuf[3] = {TOPLAYER, MIDLAYER1, BOTTOMLAYER};
	int layer;
	if(ppad->m_nPadMode == PAD_MODE_SIMPLE) cycle = 1;
	else cycle = 3;

	for(int i=0; i<cycle; i++)
	{
		layer = layerbuf[i];
		if(ppad->m_nShape[layer] == PAD_SHAPE_ROUND)
		{
			if(ppad->m_fSizeX[layer] == ppad->m_fSizeY[layer])
			{
				d = ppad->m_fSizeX[layer]/2 + pvia->m_fDia/2 + drc;
				BOOL b = Dist_P_P(ppad->m_fCx, ppad->m_fCy, pvia->m_fCx, pvia->m_fCy, d);
				if(b == TRUE) return CONNECT_YES;
			}
			else if(ppad->m_fSizeX[layer] > ppad->m_fSizeY[layer])
			{
				double fr, cx1, cy1, cx2, cy2;
					
				
				fr = ppad->m_fSizeY[layer];

				
				cx1 = ppad->m_fCx - cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;
				cy1 = ppad->m_fCy - sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;

				
				cx2 = ppad->m_fCx + cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;
				cy2 = ppad->m_fCy + sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;

				d = fr/2 +  pvia->m_fDia/2 + drc;
				BOOL b = Dist_P_Line(pvia->m_fCx, pvia->m_fCy, cx1, cy1, cx2, cy2, d);
				if(b == TRUE) return CONNECT_YES;

			}
			else
			{
				double fr, cx1, cy1, cx2, cy2;
				
				
				fr = ppad->m_fSizeX[layer];

				
				cx1 = ppad->m_fCx - sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
				cy1 = ppad->m_fCy + cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;

				
				cx2 = ppad->m_fCx + sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
				cy2 = ppad->m_fCy - cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
				
				d = fr/2 +  pvia->m_fDia/2 + drc;
				BOOL b = Dist_P_Line(pvia->m_fCx, pvia->m_fCy, cx1, cy1, cx2, cy2, d);
				if(b == TRUE) return CONNECT_YES;
			}
		}
		else if(ppad->m_nShape[layer] == PAD_SHAPE_RECTANGLE)
		{
			COb_pcbfill afill;
			afill.m_fX1 = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
			afill.m_fY1 = ppad->m_fCy - ppad->m_fSizeY[layer]/2;
			afill.m_fX2 = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
			afill.m_fY2 = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
			afill.m_fRotation = ppad->m_fRotation;

			double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
			afill.GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);
					
			d = pvia->m_fDia/2 + drc;
			BOOL b;
			b = Dist_P_Line(pvia->m_fCx, pvia->m_fCy, fx1, fy1, fx3, fy3, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_P_Line(pvia->m_fCx, pvia->m_fCy, fx3, fy3, fx2, fy2, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_P_Line(pvia->m_fCx, pvia->m_fCy, fx2, fy2, fx4, fy4, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_P_Line(pvia->m_fCx, pvia->m_fCy, fx4, fy4, fx1, fy1, d);
			if(b == TRUE) return CONNECT_YES;
		}
		else
		{
			if( ppad->m_fSizeX[layer] >= ppad->m_fSizeY[layer])
			{
				Fpoint fpoint[9];
				
				fpoint[0].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2 + ppad->m_fSizeY[layer]/4;
				fpoint[0].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;				
				fpoint[1].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
				fpoint[1].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/4;
				fpoint[2].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
				fpoint[2].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/4;
				fpoint[3].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2 + ppad->m_fSizeY[layer]/4;
				fpoint[3].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
				fpoint[4].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2 - ppad->m_fSizeY[layer]/4;
				fpoint[4].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
				fpoint[5].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
				fpoint[5].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/4;
				fpoint[6].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
				fpoint[6].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/4;
				fpoint[7].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2 - ppad->m_fSizeY[layer]/4;
				fpoint[7].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;

				
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
						fr = sqrt(fpoint[i].fy*fpoint[i].fy + fpoint[i].fx*fpoint[i].fx);
						fpoint[i].fx = ppad->m_fCx + fr*cos(alpha + ppad->m_fRotation*fEDA_pi/180);
						fpoint[i].fy = ppad->m_fCy + fr*sin(alpha + ppad->m_fRotation*fEDA_pi/180);
					}
				}

				fpoint[8].fx = fpoint[0].fx;
				fpoint[8].fy = fpoint[0].fy;

				d = pvia->m_fDia/2 + drc;
				for(int i=0; i<8; i++)
				{
					BOOL b = Dist_P_Line(pvia->m_fCx, pvia->m_fCy, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
					if(b == TRUE) return CONNECT_YES;
				}
			}
			else
			{
				Fpoint fpoint[9];
				fpoint[0].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/4;
				fpoint[0].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;
				fpoint[1].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
				fpoint[1].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2 + ppad->m_fSizeX[layer]/4;
				fpoint[2].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
				fpoint[2].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2 - ppad->m_fSizeX[layer]/4;
				fpoint[3].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/4;
				fpoint[3].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
				fpoint[4].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/4;
				fpoint[4].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
				fpoint[5].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
				fpoint[5].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2 - ppad->m_fSizeX[layer]/4;
				fpoint[6].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
				fpoint[6].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2 + ppad->m_fSizeX[layer]/4;
				fpoint[7].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/4;
				fpoint[7].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;

				
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
						fr = sqrt(fpoint[i].fy*fpoint[i].fy + fpoint[i].fx*fpoint[i].fx);
						fpoint[i].fx = ppad->m_fCx + fr*cos(alpha + ppad->m_fRotation*fEDA_pi/180);
						fpoint[i].fy = ppad->m_fCy + fr*sin(alpha + ppad->m_fRotation*fEDA_pi/180);
					}
				}

				fpoint[8].fx = fpoint[0].fx;
				fpoint[8].fy = fpoint[0].fy;
				
				d = pvia->m_fDia/2 + drc;
				for(int i=0; i<8; i++)
				{
					BOOL b = Dist_P_Line(pvia->m_fCx, pvia->m_fCy, fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, d);
					if(b == TRUE) return CONNECT_YES;
				}
			}
		}
	}
	
	return CONNECT_NO;
}

BOOL CESDView::Pad_Arc_Connect(COb_pcbpad* ppad, COb_pcbarc* parc, double drc)
{
	double dist, d;
	double deltax, deltay, temp;
	   
	
	if(ppad->m_nPadMode == PAD_MODE_SIMPLE)
	{
		dist = MAX(ppad->m_fSizeX[TOPLAYER], ppad->m_fSizeY[TOPLAYER]);
	}
	else 
	{
		dist = MAX(ppad->m_fSizeX[TOPLAYER], ppad->m_fSizeY[TOPLAYER]);
		dist = MAX(dist, ppad->m_fSizeX[BOTTOMLAYER]);
		dist = MAX(dist, ppad->m_fSizeY[BOTTOMLAYER]);
		dist = MAX(dist, ppad->m_fSizeX[MIDLAYER1]);
		dist = MAX(dist, ppad->m_fSizeY[MIDLAYER1]);
	}
	
	d = dist + parc->m_fRadius + parc->m_fWidth/2 + drc;
	if((ppad->m_fCx - parc->m_fCx) > d) return CONNECT_NO;
	if((parc->m_fCx - ppad->m_fCx) > d) return CONNECT_NO;
	if((ppad->m_fCy - parc->m_fCy) > d) return CONNECT_NO;
	if((parc->m_fCy - ppad->m_fCy) > d) return CONNECT_NO;

	int layer;
	if(ppad->m_nPadMode == PAD_MODE_SIMPLE) layer = TOPLAYER;
	else
	{
		if(parc->m_nLayer == TOPLAYER) layer = TOPLAYER;
		else if(parc->m_nLayer == BOTTOMLAYER) layer = BOTTOMLAYER;
		else layer = MIDLAYER1;
	}


	if(ppad->m_nShape[layer] == PAD_SHAPE_ROUND)
	{
		if(ppad->m_fSizeX[layer] == ppad->m_fSizeY[layer])
		{
			d = ppad->m_fSizeX[layer]/2 + parc->m_fWidth/2 + drc;
			BOOL b = Dist_P_Arc(ppad->m_fCx, ppad->m_fCy, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
			if(b == TRUE) return CONNECT_YES;
		}
		else if(ppad->m_fSizeX[layer] > ppad->m_fSizeY[layer])
		{
			double fr, cx1, cy1, cx2, cy2;
					
			
			fr = ppad->m_fSizeY[layer];

			
			cx1 = ppad->m_fCx - cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;
			cy1 = ppad->m_fCy - sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;

			
			cx2 = ppad->m_fCx + cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;
			cy2 = ppad->m_fCy + sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;

			d = fr/2 + parc->m_fWidth/2 + drc;
			BOOL b = Dist_Line_Arc(cx1, cy1, cx2, cy2, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
			if(b == TRUE) return CONNECT_YES;
		}
		else
		{
			double fr, cx1, cy1, cx2, cy2;
			
			
			fr = ppad->m_fSizeX[layer];

			
			cx1 = ppad->m_fCx - sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
			cy1 = ppad->m_fCy + cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;

			
			cx2 = ppad->m_fCx + sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
			cy2 = ppad->m_fCy - cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
				
			d = fr/2 + parc->m_fWidth/2 + drc;
			BOOL b = Dist_Line_Arc(cx1, cy1, cx2, cy2, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
			if(b == TRUE) return CONNECT_YES;
		}
	}
	else if(ppad->m_nShape[layer] == PAD_SHAPE_RECTANGLE)
	{
		COb_pcbfill afill;
		afill.m_fX1 = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
		afill.m_fY1 = ppad->m_fCy - ppad->m_fSizeY[layer]/2;
		afill.m_fX2 = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
		afill.m_fY2 = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
		afill.m_fRotation = ppad->m_fRotation;

		double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
		afill.GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);
					
		d = parc->m_fWidth/2 + drc;
		BOOL b;
		b = Dist_Line_Arc(fx1, fy1, fx3, fy3, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
		if(b == TRUE) return CONNECT_YES;
		b = Dist_Line_Arc(fx3, fy3, fx2, fy2, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
		if(b == TRUE) return CONNECT_YES;
		b = Dist_Line_Arc(fx2, fy2, fx4, fy4, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
		if(b == TRUE) return CONNECT_YES;
		b = Dist_Line_Arc(fx4, fy4, fx1, fy1, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
		if(b == TRUE) return CONNECT_YES;
	}
	else
	{
		if( ppad->m_fSizeX[layer] >= ppad->m_fSizeY[layer])
		{
			Fpoint fpoint[9];
			
			fpoint[0].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2 + ppad->m_fSizeY[layer]/4;
			fpoint[0].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;				
			fpoint[1].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
			fpoint[1].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/4;
			fpoint[2].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
			fpoint[2].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/4;
			fpoint[3].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2 + ppad->m_fSizeY[layer]/4;
			fpoint[3].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
			fpoint[4].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2 - ppad->m_fSizeY[layer]/4;
			fpoint[4].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
			fpoint[5].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
			fpoint[5].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/4;
			fpoint[6].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
			fpoint[6].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/4;
			fpoint[7].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2 - ppad->m_fSizeY[layer]/4;
			fpoint[7].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;

			
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
					fr = sqrt(fpoint[i].fy*fpoint[i].fy + fpoint[i].fx*fpoint[i].fx);
					fpoint[i].fx = ppad->m_fCx + fr*cos(alpha + ppad->m_fRotation*fEDA_pi/180);
					fpoint[i].fy = ppad->m_fCy + fr*sin(alpha + ppad->m_fRotation*fEDA_pi/180);
				}
			}

			fpoint[8].fx = fpoint[0].fx;
			fpoint[8].fy = fpoint[0].fy;

			d = parc->m_fWidth/2 + drc;
			for(int i=0; i<8; i++)
			{
				BOOL b = Dist_Line_Arc(fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
				if(b == TRUE) return CONNECT_YES;
			}
		}
		else
		{
			Fpoint fpoint[9];
			fpoint[0].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/4;
			fpoint[0].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;
			fpoint[1].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
			fpoint[1].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2 + ppad->m_fSizeX[layer]/4;
			fpoint[2].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
			fpoint[2].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2 - ppad->m_fSizeX[layer]/4;
			fpoint[3].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/4;
			fpoint[3].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
			fpoint[4].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/4;
			fpoint[4].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
			fpoint[5].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
			fpoint[5].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2 - ppad->m_fSizeX[layer]/4;
			fpoint[6].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
			fpoint[6].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2 + ppad->m_fSizeX[layer]/4;
			fpoint[7].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/4;
			fpoint[7].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;

			
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
					fr = sqrt(fpoint[i].fy*fpoint[i].fy + fpoint[i].fx*fpoint[i].fx);
					fpoint[i].fx = ppad->m_fCx + fr*cos(alpha + ppad->m_fRotation*fEDA_pi/180);
					fpoint[i].fy = ppad->m_fCy + fr*sin(alpha + ppad->m_fRotation*fEDA_pi/180);
				}
			}

			fpoint[8].fx = fpoint[0].fx;
			fpoint[8].fy = fpoint[0].fy;
				
			d = parc->m_fWidth/2 + drc;
			for(int i=0; i<8; i++)
			{
				BOOL b = Dist_Line_Arc(fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
				if(b == TRUE) return CONNECT_YES;
			}
		}
	}
	
	return CONNECT_NO;
}

BOOL CESDView::Pad_Fill_Connect(COb_pcbpad* ppad, COb_pcbfill* pfill, double drc)
{
	double dist, d;
	double deltax, deltay, temp;
	double fcx, fcy;
	   
	
	fcx = ppad->m_fCx; fcy = ppad->m_fCy;
	if(ppad->m_nPadMode == PAD_MODE_SIMPLE)
	{
		dist = MAX(ppad->m_fSizeX[TOPLAYER], ppad->m_fSizeY[TOPLAYER]);
	}
	else 
	{
		dist = MAX(ppad->m_fSizeX[TOPLAYER], ppad->m_fSizeY[TOPLAYER]);
		dist = MAX(dist, ppad->m_fSizeX[BOTTOMLAYER]);
		dist = MAX(dist, ppad->m_fSizeY[BOTTOMLAYER]);
		dist = MAX(dist, ppad->m_fSizeX[MIDLAYER1]);
		dist = MAX(dist, ppad->m_fSizeY[MIDLAYER1]);
	}
	
	double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
	pfill->GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);
	
	double fminx, fminy, fmaxx, fmaxy;
	fminx = MIN(fx1, fx2);
	fminx = MIN(fminx, fx3);
	fminx = MIN(fminx, fx4);
	fminy = MIN(fy1, fy2);
	fminy = MIN(fminy, fy3);
	fminy = MIN(fminy, fy4);
	fmaxx = MAX(fx1, fx2);
	fmaxx = MAX(fmaxx, fx3);
	fmaxx = MAX(fmaxx, fx4);
	fmaxy = MAX(fy1, fy2);
	fmaxy = MAX(fmaxy, fy3);
	fmaxy = MAX(fmaxy, fy4);
	
	d = dist + drc;

	if((fminx > fcx)&&((fminx - fcx) > d)) return CONNECT_NO;
	if((fmaxx < fcx)&&((fcx - fmaxx) > d)) return CONNECT_NO;
	if((fminy > fcy)&&((fminy - fcy) > d)) return CONNECT_NO;
	if((fmaxy < fcy)&&((fcy - fmaxy) > d)) return CONNECT_NO;

	CRgn rgn;
	POINT point[5];
	point[0].x = (int)fx1;	point[0].y = (int)fy1;
	point[1].x = (int)fx3;	point[1].y = (int)fy3;
	point[2].x = (int)fx2;	point[2].y = (int)fy2;
	point[3].x = (int)fx4;	point[3].y = (int)fy4;
	point[4].x = (int)fx1;	point[4].y = (int)fy1;
	
	rgn.CreatePolygonRgn(point, 5, ALTERNATE);
	POINT point_xy;
	point_xy.x = fcx; point_xy.y = fcy;
	
	if(rgn.PtInRegion(point_xy) == TRUE) return CONNECT_YES;


	int layer;
	if(ppad->m_nPadMode == PAD_MODE_SIMPLE) layer = TOPLAYER;
	else
	{
		if(pfill->m_nLayer == TOPLAYER) layer = TOPLAYER;
		else if(pfill->m_nLayer == BOTTOMLAYER) layer = BOTTOMLAYER;
		else layer = MIDLAYER1;
	}


	if(ppad->m_nShape[layer] == PAD_SHAPE_ROUND)
	{
		if(ppad->m_fSizeX[layer] == ppad->m_fSizeY[layer])
		{
			d = ppad->m_fSizeX[layer]/2 + drc;
			BOOL b; 
			b = Dist_P_Line(fcx, fcy, fx1, fy1, fx3, fy3, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_P_Line(fcx, fcy, fx3, fy3, fx2, fy2, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_P_Line(fcx, fcy, fx2, fy2, fx4, fy4, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_P_Line(fcx, fcy, fx4, fy4, fx1, fy1, d);
			if(b == TRUE) return CONNECT_YES;
		}
		else if(ppad->m_fSizeX[layer] > ppad->m_fSizeY[layer])
		{
			double fr, cx1, cy1, cx2, cy2;
					
			
			fr = ppad->m_fSizeY[layer];

			
			cx1 = ppad->m_fCx - cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;
			cy1 = ppad->m_fCy - sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;

			
			cx2 = ppad->m_fCx + cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;
			cy2 = ppad->m_fCy + sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;

			d = fr/2 + drc;
			BOOL b;
			b = Dist_Line_Line(fx1, fy1, fx3, fy3, cx1, cy1, cx2, cy2, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_Line_Line(fx3, fy3, fx2, fy2, cx1, cy1, cx2, cy2, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_Line_Line(fx2, fy2, fx4, fy4, cx1, cy1, cx2, cy2, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_Line_Line(fx4, fy4, fx1, fy1, cx1, cy1, cx2, cy2, d);
			if(b == TRUE) return CONNECT_YES;
		}
		else
		{
			double fr, cx1, cy1, cx2, cy2;
			
			
			fr = ppad->m_fSizeX[layer];

			
			cx1 = ppad->m_fCx - sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
			cy1 = ppad->m_fCy + cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;

			
			cx2 = ppad->m_fCx + sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
			cy2 = ppad->m_fCy - cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
				
			d = fr/2 + drc;
			BOOL b;
			b = Dist_Line_Line(fx1, fy1, fx3, fy3, cx1, cy1, cx2, cy2, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_Line_Line(fx3, fy3, fx2, fy2, cx1, cy1, cx2, cy2, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_Line_Line(fx2, fy2, fx4, fy4, cx1, cy1, cx2, cy2, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_Line_Line(fx4, fy4, fx1, fy1, cx1, cy1, cx2, cy2, d);
			if(b == TRUE) return CONNECT_YES;
		}
	}
	else if(ppad->m_nShape[layer] == PAD_SHAPE_RECTANGLE)
	{
		COb_pcbfill afill;
		afill.m_fX1 = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
		afill.m_fY1 = ppad->m_fCy - ppad->m_fSizeY[layer]/2;
		afill.m_fX2 = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
		afill.m_fY2 = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
		afill.m_fRotation = ppad->m_fRotation;

		double a_fx1, a_fy1, a_fx2, a_fy2, a_fx3, a_fy3, a_fx4, a_fy4;
		afill.GetFillCorner(&a_fx1, &a_fy1, &a_fx2, &a_fy2, &a_fx3, &a_fy3, &a_fx4, &a_fy4);
					
		d = drc;
		BOOL b;
		b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, fx1, fy1, fx3, fy3, d);
		if(b == TRUE) return CONNECT_YES;
		b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, fx1, fy1, fx3, fy3, d);
		if(b == TRUE) return CONNECT_YES;	
		b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, fx1, fy1, fx3, fy3, d);
		if(b == TRUE) return CONNECT_YES;					
		b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, fx1, fy1, fx3, fy3, d);
		if(b == TRUE) return CONNECT_YES;
					
		b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, fx3, fy3, fx2, fy2, d);
		if(b == TRUE) return CONNECT_YES;
		b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, fx3, fy3, fx2, fy2, d);
		if(b == TRUE) return CONNECT_YES;	
		b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, fx3, fy3, fx2, fy2, d);
		if(b == TRUE) return CONNECT_YES;					
		b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, fx3, fy3, fx2, fy2, d);
		if(b == TRUE) return CONNECT_YES;

		b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, fx2, fy2, fx4, fy4, d);
		if(b == TRUE) return CONNECT_YES;
		b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, fx2, fy2, fx4, fy4, d);
		if(b == TRUE) return CONNECT_YES;	
		b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, fx2, fy2, fx4, fy4, d);
		if(b == TRUE) return CONNECT_YES;					
		b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, fx2, fy2, fx4, fy4, d);
		if(b == TRUE) return CONNECT_YES;

		b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, fx4, fy4, fx1, fy1, d);
		if(b == TRUE) return CONNECT_YES;
		b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, fx4, fy4, fx1, fy1, d);
		if(b == TRUE) return CONNECT_YES;	
		b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, fx4, fy4, fx1, fy1, d);
		if(b == TRUE) return CONNECT_YES;					
		b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, fx4, fy4, fx1, fy1, d);
		if(b == TRUE) return CONNECT_YES;
	}
	else
	{
		if( ppad->m_fSizeX[layer] >= ppad->m_fSizeY[layer])
		{
			Fpoint fpoint[9];
			
			fpoint[0].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2 + ppad->m_fSizeY[layer]/4;
			fpoint[0].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;				
			fpoint[1].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
			fpoint[1].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/4;
			fpoint[2].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
			fpoint[2].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/4;
			fpoint[3].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2 + ppad->m_fSizeY[layer]/4;
			fpoint[3].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
			fpoint[4].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2 - ppad->m_fSizeY[layer]/4;
			fpoint[4].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
			fpoint[5].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
			fpoint[5].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/4;
			fpoint[6].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
			fpoint[6].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/4;
			fpoint[7].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2 - ppad->m_fSizeY[layer]/4;
			fpoint[7].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;

			
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
					fr = sqrt(fpoint[i].fy*fpoint[i].fy + fpoint[i].fx*fpoint[i].fx);
					fpoint[i].fx = ppad->m_fCx + fr*cos(alpha + ppad->m_fRotation*fEDA_pi/180);
					fpoint[i].fy = ppad->m_fCy + fr*sin(alpha + ppad->m_fRotation*fEDA_pi/180);
				}
			}

			fpoint[8].fx = fpoint[0].fx;
			fpoint[8].fy = fpoint[0].fy;

			d = drc;
			for(int i=0; i<8; i++)
			{
				BOOL b;
				b = Dist_Line_Line(fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, fx1, fy1, fx3, fy3, d);
				if(b == TRUE) return CONNECT_YES;
				b = Dist_Line_Line(fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, fx3, fy3, fx2, fy2, d);
				if(b == TRUE) return CONNECT_YES;
				b = Dist_Line_Line(fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, fx2, fy2, fx4, fy4, d);
				if(b == TRUE) return CONNECT_YES;
				b = Dist_Line_Line(fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, fx4, fy4, fx1, fy1, d);
				if(b == TRUE) return CONNECT_YES;
			}
		}
		else
		{
			Fpoint fpoint[9];
			fpoint[0].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/4;
			fpoint[0].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;
			fpoint[1].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
			fpoint[1].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2 + ppad->m_fSizeX[layer]/4;
			fpoint[2].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
			fpoint[2].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2 - ppad->m_fSizeX[layer]/4;
			fpoint[3].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/4;
			fpoint[3].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
			fpoint[4].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/4;
			fpoint[4].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
			fpoint[5].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
			fpoint[5].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2 - ppad->m_fSizeX[layer]/4;
			fpoint[6].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
			fpoint[6].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2 + ppad->m_fSizeX[layer]/4;
			fpoint[7].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/4;
			fpoint[7].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;

			
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
					fr = sqrt(fpoint[i].fy*fpoint[i].fy + fpoint[i].fx*fpoint[i].fx);
					fpoint[i].fx = ppad->m_fCx + fr*cos(alpha + ppad->m_fRotation*fEDA_pi/180);
					fpoint[i].fy = ppad->m_fCy + fr*sin(alpha + ppad->m_fRotation*fEDA_pi/180);
				}
			}

			fpoint[8].fx = fpoint[0].fx;
			fpoint[8].fy = fpoint[0].fy;
				
			d = drc;
			for(int i=0; i<8; i++)
			{
				BOOL b;
				b = Dist_Line_Line(fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, fx1, fy1, fx3, fy3, d);
				if(b == TRUE) return CONNECT_YES;
				b = Dist_Line_Line(fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, fx3, fy3, fx2, fy2, d);
				if(b == TRUE) return CONNECT_YES;
				b = Dist_Line_Line(fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, fx2, fy2, fx4, fy4, d);
				if(b == TRUE) return CONNECT_YES;
				b = Dist_Line_Line(fpoint[i].fx, fpoint[i].fy, fpoint[i+1].fx, fpoint[i+1].fy, fx4, fy4, fx1, fy1, d);
				if(b == TRUE) return CONNECT_YES;
			}
		}
	}
	
	return CONNECT_NO;
}

BOOL CESDView::Pad_Region_Connect(COb_pcbpad* ppad, COb_pcbregion* pregion, double drc)
{
	double dist, d;
	double fcx, fcy;
	   
	
	fcx = ppad->m_fCx; fcy = ppad->m_fCy;
	if(ppad->m_nPadMode == PAD_MODE_SIMPLE)
	{
		dist = MAX(ppad->m_fSizeX[TOPLAYER], ppad->m_fSizeY[TOPLAYER]);
	}
	else 
	{
		dist = MAX(ppad->m_fSizeX[TOPLAYER], ppad->m_fSizeY[TOPLAYER]);
		dist = MAX(dist, ppad->m_fSizeX[BOTTOMLAYER]);
		dist = MAX(dist, ppad->m_fSizeY[BOTTOMLAYER]);
		dist = MAX(dist, ppad->m_fSizeX[MIDLAYER1]);
		dist = MAX(dist, ppad->m_fSizeY[MIDLAYER1]);
	}

	double fminx, fminy, fmaxx, fmaxy;
	fminx = 100000; fminy = 100000;
	fmaxx = -100000; fmaxy = -100000;
	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex;
		vertex = pregion->m_cVertex.GetAt(i);
		fminx = MIN(fminx, vertex.fx);
		fminy = MIN(fminy, vertex.fy);
		fmaxx = MAX(fmaxx, vertex.fx);
		fmaxy = MAX(fmaxy, vertex.fy);
	}
	
	d = dist + drc;

	if((fminx > fcx)&&((fminx - fcx) > d)) return CONNECT_NO;
	if((fmaxx < fcx)&&((fcx - fmaxx) > d)) return CONNECT_NO;
	if((fminy > fcy)&&((fminy - fcy) > d)) return CONNECT_NO;
	if((fmaxy < fcy)&&((fcy - fmaxy) > d)) return CONNECT_NO;

	if(pregion->Is_On_Me(fcx, fcy) == TRUE) return CONNECT_YES;

	int layer;
	if(ppad->m_nPadMode == PAD_MODE_SIMPLE) layer = TOPLAYER;
	else
	{
		if(pregion->m_nLayer == TOPLAYER) layer = TOPLAYER;
		else if(pregion->m_nLayer == BOTTOMLAYER) layer = BOTTOMLAYER;
		else layer = MIDLAYER1;
	}

	if(ppad->m_nShape[layer] == PAD_SHAPE_ROUND)
	{
		if(ppad->m_fSizeX[layer] == ppad->m_fSizeY[layer])
		{
			d = ppad->m_fSizeX[layer]/2 + drc;
			for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
			{
				Struct_RegionVertex  vertex1, vertex2;
				vertex1 = pregion->m_cVertex.GetAt(i);
				if(i < (pregion->m_cVertex.GetCount()-1)) vertex2 = pregion->m_cVertex.GetAt(i+1);
				else vertex2 = pregion->m_cVertex.GetAt(0);

				BOOL b = Dist_P_Line(fcx, fcy, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
				if(b == TRUE) return CONNECT_YES;
			}
			for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
			{
				Struct_RegionVertex  vertex;
				vertex = pregion->m_cVertex.GetAt(i);

				if(vertex.radius != 0)
				{
					BOOL b = Dist_P_Arc(fcx, fcy, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
					if(b == TRUE) return CONNECT_YES;
				}
			}
		}
		else if(ppad->m_fSizeX[layer] > ppad->m_fSizeY[layer])
		{
			double fr, cx1, cy1, cx2, cy2;
					
			
			fr = ppad->m_fSizeY[layer];

			
			cx1 = ppad->m_fCx - cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;
			cy1 = ppad->m_fCy - sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;

			
			cx2 = ppad->m_fCx + cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;
			cy2 = ppad->m_fCy + sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeX[layer] - ppad->m_fSizeY[layer])/2;

			d = fr/2 + drc;
			for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
			{
				Struct_RegionVertex  vertex1, vertex2;
				vertex1 = pregion->m_cVertex.GetAt(i);
				if(i < (pregion->m_cVertex.GetCount()-1)) vertex2 = pregion->m_cVertex.GetAt(i+1);
				else vertex2 = pregion->m_cVertex.GetAt(0);

				BOOL b = Dist_Line_Line(cx1, cy1, cx2, cy2, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
				if(b == TRUE) return CONNECT_YES;
			}
			for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
			{
				Struct_RegionVertex  vertex;
				vertex = pregion->m_cVertex.GetAt(i);

				if(vertex.radius != 0)
				{
					BOOL b = Dist_Line_Arc(cx1, cy1, cx2, cy2, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
					if(b == TRUE) return CONNECT_YES;
				}
			}

		}
		else
		{
			double fr, cx1, cy1, cx2, cy2;
				
			
			fr = ppad->m_fSizeX[layer];

			
			cx1 = ppad->m_fCx - sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
			cy1 = ppad->m_fCy + cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;

			
			cx2 = ppad->m_fCx + sin(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
			cy2 = ppad->m_fCy - cos(ppad->m_fRotation*fEDA_pi/180)*(ppad->m_fSizeY[layer] - ppad->m_fSizeX[layer])/2;
				
			d = fr/2 + drc;
			for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
			{
				Struct_RegionVertex  vertex1, vertex2;
				vertex1 = pregion->m_cVertex.GetAt(i);
				if(i < (pregion->m_cVertex.GetCount()-1)) vertex2 = pregion->m_cVertex.GetAt(i+1);
				else vertex2 = pregion->m_cVertex.GetAt(0);

				BOOL b = Dist_Line_Line(cx1, cy1, cx2, cy2, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
				if(b == TRUE) return CONNECT_YES;
			}
			for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
			{
				Struct_RegionVertex  vertex;
				vertex = pregion->m_cVertex.GetAt(i);

				if(vertex.radius != 0)
				{
					BOOL b = Dist_Line_Arc(cx1, cy1, cx2, cy2, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
					if(b == TRUE) return CONNECT_YES;
				}
			}
		}
	}
	else if(ppad->m_nShape[layer] == PAD_SHAPE_RECTANGLE)
	{
		COb_pcbfill afill;
		afill.m_fX1 = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
		afill.m_fY1 = ppad->m_fCy - ppad->m_fSizeY[layer]/2;
		afill.m_fX2 = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
		afill.m_fY2 = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
		afill.m_fRotation = ppad->m_fRotation;

		double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
		afill.GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);
					
		d = drc;
		for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
		{
			Struct_RegionVertex  vertex1, vertex2;
			vertex1 = pregion->m_cVertex.GetAt(i);
			if(i < (pregion->m_cVertex.GetCount()-1)) vertex2 = pregion->m_cVertex.GetAt(i+1);
			else vertex2 = pregion->m_cVertex.GetAt(0);

			BOOL b;
			b = Dist_Line_Line(fx1, fy1, fx3, fy3, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_Line_Line(fx3, fy3, fx2, fy2, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_Line_Line(fx2, fy2, fx4, fy4, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_Line_Line(fx4, fy4, fx1, fy1, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
			if(b == TRUE) return CONNECT_YES;
		}
		for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
		{
			Struct_RegionVertex  vertex;
			vertex = pregion->m_cVertex.GetAt(i);

			if(vertex.radius != 0)
			{
				BOOL b;
				b = Dist_Line_Arc(fx1, fy1, fx3, fy3, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
				if(b == TRUE) return CONNECT_YES;
				b = Dist_Line_Arc(fx3, fy3, fx2, fy2, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
				if(b == TRUE) return CONNECT_YES;
				b = Dist_Line_Arc(fx2, fy2, fx4, fy4, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
				if(b == TRUE) return CONNECT_YES;
				b = Dist_Line_Arc(fx4, fy4, fx1, fy1, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
				if(b == TRUE) return CONNECT_YES;
			}
		}
	}
	else
	{
		if( ppad->m_fSizeX[layer] >= ppad->m_fSizeY[layer])
		{
			Fpoint fpoint[9];
			
			fpoint[0].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2 + ppad->m_fSizeY[layer]/4;
			fpoint[0].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;				
			fpoint[1].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
			fpoint[1].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/4;
			fpoint[2].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
			fpoint[2].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/4;
			fpoint[3].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2 + ppad->m_fSizeY[layer]/4;
			fpoint[3].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
			fpoint[4].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2 - ppad->m_fSizeY[layer]/4;
			fpoint[4].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
			fpoint[5].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
			fpoint[5].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/4;
			fpoint[6].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
			fpoint[6].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/4;
			fpoint[7].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2 - ppad->m_fSizeY[layer]/4;
			fpoint[7].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;

			
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
					fr = sqrt(fpoint[i].fy*fpoint[i].fy + fpoint[i].fx*fpoint[i].fx);
					fpoint[i].fx = ppad->m_fCx + fr*cos(alpha + ppad->m_fRotation*fEDA_pi/180);
					fpoint[i].fy = ppad->m_fCy + fr*sin(alpha + ppad->m_fRotation*fEDA_pi/180);
				}
			}

			fpoint[8].fx = fpoint[0].fx;
			fpoint[8].fy = fpoint[0].fy;

			d = drc;
			for(int j=0; j<8; j++)
			{
				for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
				{
					Struct_RegionVertex  vertex1, vertex2;
					vertex1 = pregion->m_cVertex.GetAt(i);
					if(i < (pregion->m_cVertex.GetCount()-1)) vertex2 = pregion->m_cVertex.GetAt(i+1);
					else vertex2 = pregion->m_cVertex.GetAt(0);

					BOOL b;
					b = Dist_Line_Line(fpoint[j].fx, fpoint[j].fy, fpoint[j+1].fx, fpoint[j+1].fy, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
					if(b == TRUE) return CONNECT_YES;
				}
				for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
				{
					Struct_RegionVertex  vertex;
					vertex = pregion->m_cVertex.GetAt(i);

					if(vertex.radius != 0)
					{
						BOOL b;
						b = Dist_Line_Arc(fpoint[j].fx, fpoint[j].fy, fpoint[j+1].fx, fpoint[j+1].fy, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
						if(b == TRUE) return CONNECT_YES;

					}
				}
			}
		}
		else
		{
			Fpoint fpoint[9];
			fpoint[0].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/4;
			fpoint[0].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;
			fpoint[1].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
			fpoint[1].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2 + ppad->m_fSizeX[layer]/4;
			fpoint[2].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/2;
			fpoint[2].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2 - ppad->m_fSizeX[layer]/4;
			fpoint[3].fx = ppad->m_fCx - ppad->m_fSizeX[layer]/4;
			fpoint[3].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
			fpoint[4].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/4;
			fpoint[4].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2;
			fpoint[5].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
			fpoint[5].fy = ppad->m_fCy + ppad->m_fSizeY[layer]/2 - ppad->m_fSizeX[layer]/4;
			fpoint[6].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/2;
			fpoint[6].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2 + ppad->m_fSizeX[layer]/4;
			fpoint[7].fx = ppad->m_fCx + ppad->m_fSizeX[layer]/4;
			fpoint[7].fy = ppad->m_fCy - ppad->m_fSizeY[layer]/2;

			
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
					fr = sqrt(fpoint[i].fy*fpoint[i].fy + fpoint[i].fx*fpoint[i].fx);
					fpoint[i].fx = ppad->m_fCx + fr*cos(alpha + ppad->m_fRotation*fEDA_pi/180);
					fpoint[i].fy = ppad->m_fCy + fr*sin(alpha + ppad->m_fRotation*fEDA_pi/180);
				}
			}

			fpoint[8].fx = fpoint[0].fx;
			fpoint[8].fy = fpoint[0].fy;
				
			d = drc;
			for(int j=0; j<8; j++)
			{
				for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
				{
					Struct_RegionVertex  vertex1, vertex2;
					vertex1 = pregion->m_cVertex.GetAt(i);
					if(i < (pregion->m_cVertex.GetCount()-1)) vertex2 = pregion->m_cVertex.GetAt(i+1);
					else vertex2 = pregion->m_cVertex.GetAt(0);

					BOOL b;
					b = Dist_Line_Line(fpoint[j].fx, fpoint[j].fy, fpoint[j+1].fx, fpoint[j+1].fy, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
					if(b == TRUE) return CONNECT_YES;
				}
				for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
				{
					Struct_RegionVertex  vertex;
					vertex = pregion->m_cVertex.GetAt(i);

					if(vertex.radius != 0)
					{
						BOOL b;
						b = Dist_Line_Arc(fpoint[j].fx, fpoint[j].fy, fpoint[j+1].fx, fpoint[j+1].fy, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
						if(b == TRUE) return CONNECT_YES;

					}
				}
			}
		}
	}
	
	return CONNECT_NO;
}

BOOL CESDView::Via_Via_Connect(COb_pcbvia* pvia1, COb_pcbvia* pvia2, double drc)
{
	double d = pvia1->m_fDia/2 + pvia2->m_fDia/2 + drc;

	if((pvia1->m_fCx - pvia2->m_fCx) > d) return CONNECT_NO;
	if((pvia2->m_fCx - pvia1->m_fCx) > d) return CONNECT_NO;
	if((pvia1->m_fCy - pvia2->m_fCy) > d) return CONNECT_NO;
	if((pvia2->m_fCy - pvia1->m_fCy) > d) return CONNECT_NO;

	return Dist_P_P(pvia1->m_fCx, pvia1->m_fCy, pvia2->m_fCx, pvia2->m_fCy, d);
}

BOOL CESDView::Via_Arc_Connect(COb_pcbvia* pvia, COb_pcbarc* parc, double drc)
{
	double d = pvia->m_fDia/2 + parc->m_fWidth/2 + parc->m_fRadius + drc;

	if((pvia->m_fCx - parc->m_fCx) > d) return CONNECT_NO;
	if((parc->m_fCx - pvia->m_fCx) > d) return CONNECT_NO;
	if((pvia->m_fCy - parc->m_fCy) > d) return CONNECT_NO;
	if((parc->m_fCy - pvia->m_fCy) > d) return CONNECT_NO;
	
	d = pvia->m_fDia/2 + parc->m_fWidth/2 + drc;
	return Dist_P_Arc(pvia->m_fCx, pvia->m_fCy, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
}

BOOL CESDView::Via_Fill_Connect(COb_pcbvia* pvia, COb_pcbfill* pfill, double drc)
{
	double d;
	double fcx, fcy;

	fcx = pvia->m_fCx; fcy = pvia->m_fCy;
	
	double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
	pfill->GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);
	
	double fminx, fminy, fmaxx, fmaxy;
	fminx = MIN(fx1, fx2);
	fminx = MIN(fminx, fx3);
	fminx = MIN(fminx, fx4);
	fminy = MIN(fy1, fy2);
	fminy = MIN(fminy, fy3);
	fminy = MIN(fminy, fy4);
	fmaxx = MAX(fx1, fx2);
	fmaxx = MAX(fmaxx, fx3);
	fmaxx = MAX(fmaxx, fx4);
	fmaxy = MAX(fy1, fy2);
	fmaxy = MAX(fmaxy, fy3);
	fmaxy = MAX(fmaxy, fy4);
	
	d = pvia->m_fDia/2 + drc;

	if((fminx > fcx)&&((fminx - fcx) > d)) return CONNECT_NO;
	if((fmaxx < fcx)&&((fcx - fmaxx) > d)) return CONNECT_NO;
	if((fminy > fcy)&&((fminy - fcy) > d)) return CONNECT_NO;
	if((fmaxy < fcy)&&((fcy - fmaxy) > d)) return CONNECT_NO;

	CRgn rgn;
	POINT point[5];
	point[0].x = (int)fx1;	point[0].y = (int)fy1;
	point[1].x = (int)fx3;	point[1].y = (int)fy3;
	point[2].x = (int)fx2;	point[2].y = (int)fy2;
	point[3].x = (int)fx4;	point[3].y = (int)fy4;
	point[4].x = (int)fx1;	point[4].y = (int)fy1;
	
	rgn.CreatePolygonRgn(point, 5, ALTERNATE);
	POINT point_xy;
	point_xy.x = (int)fcx; point_xy.y = (int)fcy;
	
	if(rgn.PtInRegion(point_xy) == TRUE) return CONNECT_YES;

	BOOL b;
	b = Dist_P_Line(fcx, fcy, fx1, fy1, fx3, fy3, d);
	if(b == TRUE) return CONNECT_YES;
	b = Dist_P_Line(fcx, fcy, fx3, fy3, fx2, fy2, d);
	if(b == TRUE) return CONNECT_YES;
	b = Dist_P_Line(fcx, fcy, fx2, fy2, fx4, fy4, d);
	if(b == TRUE) return CONNECT_YES;
	b = Dist_P_Line(fcx, fcy, fx4, fy4, fx1, fy1, d);
	if(b == TRUE) return CONNECT_YES;

	return CONNECT_NO;
}

BOOL CESDView::Via_Region_Connect(COb_pcbvia* pvia, COb_pcbregion* pregion, double drc)
{
	double d;
	double fcx, fcy;
	   
	fcx = pvia->m_fCx; fcy = pvia->m_fCy;

	double fminx, fminy, fmaxx, fmaxy;
	fminx = 100000; fminy = 100000;
	fmaxx = -100000; fmaxy = -100000;
	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex;
		vertex = pregion->m_cVertex.GetAt(i);
		fminx = MIN(fminx, vertex.fx);
		fminy = MIN(fminy, vertex.fy);
		fmaxx = MAX(fmaxx, vertex.fx);
		fmaxy = MAX(fmaxy, vertex.fy);
	}
	
	d = pvia->m_fDia/2 + drc;

	if((fminx > fcx)&&((fminx - fcx) > d)) return CONNECT_NO;
	if((fmaxx < fcx)&&((fcx - fmaxx) > d)) return CONNECT_NO;
	if((fminy > fcy)&&((fminy - fcy) > d)) return CONNECT_NO;
	if((fmaxy < fcy)&&((fcy - fmaxy) > d)) return CONNECT_NO;

	if(pregion->Is_On_Me(fcx, fcy) == TRUE) return CONNECT_YES;
	
	d = pvia->m_fDia/2 + drc;
	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex1, vertex2;
		vertex1 = pregion->m_cVertex.GetAt(i);
		if(i < (pregion->m_cVertex.GetCount()-1)) vertex2 = pregion->m_cVertex.GetAt(i+1);
		else vertex2 = pregion->m_cVertex.GetAt(0);

		BOOL b = Dist_P_Line(pvia->m_fCx, pvia->m_fCy, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
		if(b == TRUE) return CONNECT_YES;
	}
	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex;
		vertex = pregion->m_cVertex.GetAt(i);

		if(vertex.radius != 0)
		{
			BOOL b = Dist_P_Arc(pvia->m_fCx, pvia->m_fCy, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
			if(b == TRUE) return CONNECT_YES;
		}
	}
	return CONNECT_NO;
}

BOOL CESDView::Arc_Arc_Connect(COb_pcbarc* parc1, COb_pcbarc* parc2, double drc)
{
	double d = parc1->m_fWidth/2 + parc1->m_fRadius + parc2->m_fWidth/2 + parc2->m_fRadius + drc;
	if((parc1->m_fCx - parc2->m_fCx) > d) return CONNECT_NO;
	if((parc2->m_fCx - parc1->m_fCx) > d) return CONNECT_NO;
	if((parc1->m_fCy - parc2->m_fCy) > d) return CONNECT_NO;
	if((parc2->m_fCy - parc1->m_fCy) > d) return CONNECT_NO;

	double dis = sqrt((parc1->m_fCx - parc2->m_fCx)*(parc1->m_fCx - parc2->m_fCx) + (parc1->m_fCy - parc2->m_fCy)*(parc1->m_fCy - parc2->m_fCy));
	if(dis > d) return CONNECT_NO;

	d = parc1->m_fWidth/2 + parc2->m_fWidth/2 + drc;
	if(parc1->m_fRadius < parc2->m_fRadius)
	{
		if((parc1->m_fSangle == parc1->m_fEangle)||(abs(parc1->m_fSangle - parc1->m_fEangle) == 360))
		{
			double fx1, fy1, fx2, fy2;
			double r = parc1->m_fRadius;
			for(int i=0; i<=360; i++)
			{
				if(i == 0)
				{
					fx2 = parc1->m_fCx + r;
					fy2 = parc1->m_fCy;
				}
				else
				{
					fx1 = fx2;
					fy1 = fy2;
					
					fx2 = parc1->m_fCx + r*cos(i*fEDA_pi/180);
					fy2 = parc1->m_fCy + r*sin(i*fEDA_pi/180);

					BOOL b = Dist_Line_Arc(fx1, fy1, fx2, fy2, parc2->m_fCx, parc2->m_fCy, parc2->m_fRadius, parc2->m_fSangle, parc2->m_fEangle, d);
					if(b == TRUE) return CONNECT_YES;

				}
			}
		}
		else
		{
			double fx1, fy1, fx2, fy2;
			double r = parc1->m_fRadius;
			
			int sangle, eangle;
			sangle = parc1->m_fSangle;
			eangle = parc1->m_fEangle;
			for(;;)
			{
				if(eangle < sangle) eangle += 360;
				else break;
			}
			for(int i=sangle; i<=eangle; i++)
			{
				if(i == sangle)
				{
					fx2 = parc1->m_fCx + r*cos(i*fEDA_pi/180);
					fy2 = parc1->m_fCy + r*sin(i*fEDA_pi/180);
				}
				else
				{
					fx1 = fx2;
					fy1 = fy2;
					
					fx2 = parc1->m_fCx + r*cos(i*fEDA_pi/180);
					fy2 = parc1->m_fCy + r*sin(i*fEDA_pi/180);

					BOOL b = Dist_Line_Arc(fx1, fy1, fx2, fy2, parc2->m_fCx, parc2->m_fCy, parc2->m_fRadius, parc2->m_fSangle, parc2->m_fEangle, d);
					if(b == TRUE) return CONNECT_YES;
				}
			}
		}
	}
	else
	{
		if((parc2->m_fSangle == parc2->m_fEangle)||(abs(parc2->m_fSangle - parc2->m_fEangle) == 360))
		{
			double fx1, fy1, fx2, fy2;
			double r = parc2->m_fRadius;
			for(int i=0; i<=360; i++)
			{
				if(i == 0)
				{
					fx2 = parc2->m_fCx + r;
					fy2 = parc2->m_fCy;
				}
				else
				{
					fx1 = fx2;
					fy1 = fy2;
					
					fx2 = parc2->m_fCx + r*cos(i*fEDA_pi/180);
					fy2 = parc2->m_fCy + r*sin(i*fEDA_pi/180);

					BOOL b = Dist_Line_Arc(fx1, fy1, fx2, fy2, parc1->m_fCx, parc1->m_fCy, parc1->m_fRadius, parc1->m_fSangle, parc1->m_fEangle, d);
					if(b == TRUE) return CONNECT_YES;

				}
			}
		}
		else
		{
			double fx1, fy1, fx2, fy2;
			double r = parc2->m_fRadius;
			
			int sangle, eangle;
			sangle = parc2->m_fSangle;
			eangle = parc2->m_fEangle;
			for(;;)
			{
				if(eangle < sangle) eangle += 360;
				else break;
			}
			for(int i=sangle; i<=eangle; i++)
			{
				if(i == sangle)
				{
					fx2 = parc2->m_fCx + r*cos(i*fEDA_pi/180);
					fy2 = parc2->m_fCy + r*sin(i*fEDA_pi/180);
				}
				else
				{
					fx1 = fx2;
					fy1 = fy2;
					
					fx2 = parc2->m_fCx + r*cos(i*fEDA_pi/180);
					fy2 = parc2->m_fCy + r*sin(i*fEDA_pi/180);

					BOOL b = Dist_Line_Arc(fx1, fy1, fx2, fy2, parc1->m_fCx, parc1->m_fCy, parc1->m_fRadius, parc1->m_fSangle, parc1->m_fEangle, d);
					if(b == TRUE) return CONNECT_YES;
				}
			}
		}
	}

	return CONNECT_NO;
}

BOOL CESDView::Arc_Fill_Connect(COb_pcbarc* parc, COb_pcbfill* pfill, double drc)
{
	double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
	pfill->GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);

	double fminx, fminy, fmaxx, fmaxy;
	fminx = MIN(fx1, fx2);
	fminx = MIN(fminx, fx3);
	fminx = MIN(fminx, fx4);
	fminy = MIN(fy1, fy2);
	fminy = MIN(fminy, fy3);
	fminy = MIN(fminy, fy4);
	fmaxx = MAX(fx1, fx2);
	fmaxx = MAX(fmaxx, fx3);
	fmaxx = MAX(fmaxx, fx4);
	fmaxy = MAX(fy1, fy2);
	fmaxy = MAX(fmaxy, fy3);
	fmaxy = MAX(fmaxy, fy4);
	
	double fcx = parc->m_fCx;
	double fcy = parc->m_fCy;

	double d = parc->m_fWidth/2 + parc->m_fRadius + drc;

	if((fminx > fcx)&&((fminx - fcx) > d)) return CONNECT_NO;
	if((fmaxx < fcx)&&((fcx - fmaxx) > d)) return CONNECT_NO;
	if((fminy > fcy)&&((fminy - fcy) > d)) return CONNECT_NO;
	if((fmaxy < fcy)&&((fcy - fmaxy) > d)) return CONNECT_NO;

	d = parc->m_fWidth/2 + drc;
	BOOL b;
	b = Dist_Line_Arc(fx1, fy1, fx3, fy3, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
	if(b == TRUE) return CONNECT_YES;
	b = Dist_Line_Arc(fx3, fy3, fx2, fy2, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
	if(b == TRUE) return CONNECT_YES;
	b = Dist_Line_Arc(fx2, fy2, fx4, fy4, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
	if(b == TRUE) return CONNECT_YES;
	b = Dist_Line_Arc(fx4, fy4, fx1, fy1, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
	if(b == TRUE) return CONNECT_YES;

	return CONNECT_NO;
}

BOOL CESDView::Arc_Region_Connect(COb_pcbarc* parc, COb_pcbregion* pregion, double drc)
{
	double d;
	double fcx, fcy;
	   
	fcx = parc->m_fCx; fcy = parc->m_fCy;

	double fminx, fminy, fmaxx, fmaxy;
	fminx = 100000; fminy = 100000;
	fmaxx = -100000; fmaxy = -100000;
	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex;
		vertex = pregion->m_cVertex.GetAt(i);
		fminx = MIN(fminx, vertex.fx);
		fminy = MIN(fminy, vertex.fy);
		fmaxx = MAX(fmaxx, vertex.fx);
		fmaxy = MAX(fmaxy, vertex.fy);
	}
	
	d = parc->m_fWidth/2 + parc->m_fRadius + drc;

	if((fminx > fcx)&&((fminx - fcx) > d)) return CONNECT_NO;
	if((fmaxx < fcx)&&((fcx - fmaxx) > d)) return CONNECT_NO;
	if((fminy > fcy)&&((fminy - fcy) > d)) return CONNECT_NO;
	if((fmaxy < fcy)&&((fcy - fmaxy) > d)) return CONNECT_NO;

	double sx = fcx + parc->m_fRadius*cos(parc->m_fSangle*fEDA_pi/180);
	double sy = fcy + parc->m_fRadius*sin(parc->m_fSangle*fEDA_pi/180);	
	if(pregion->Is_On_Me(sx, sy) == TRUE) return CONNECT_YES;
	double ex = fcx + parc->m_fRadius*cos(parc->m_fEangle*fEDA_pi/180);
	double ey = fcy + parc->m_fRadius*sin(parc->m_fEangle*fEDA_pi/180);	
	if(pregion->Is_On_Me(ex, ey) == TRUE) return CONNECT_YES;

	d = parc->m_fWidth/2 + drc;
	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex1, vertex2;
		vertex1 = pregion->m_cVertex.GetAt(i);
		if(i < (pregion->m_cVertex.GetCount()-1)) vertex2 = pregion->m_cVertex.GetAt(i+1);
		else vertex2 = pregion->m_cVertex.GetAt(0);

		BOOL b = Dist_Line_Arc(vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, parc->m_fCx, parc->m_fCy, parc->m_fRadius, parc->m_fSangle, parc->m_fEangle, d);
		if(b == TRUE) return CONNECT_YES;
	}
	/*for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex;
		vertex = pregion->m_cVertex.GetAt(i);

		if(vertex.radius != 0)
		{
			BOOL b = Dist_Arc_Arc(ptrack->m_fX1, ptrack->m_fY1, ptrack->m_fX2, ptrack->m_fY2, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
			if(b == TRUE) return CONNECT_YES;
		}
	}*/
	return CONNECT_NO;
}

BOOL CESDView::Fill_Fill_Connect(COb_pcbfill* pfill1, COb_pcbfill* pfill2, double drc)
{
	double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
	pfill1->GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);

	double a_fx1, a_fy1, a_fx2, a_fy2, a_fx3, a_fy3, a_fx4, a_fy4;
	pfill2->GetFillCorner(&a_fx1, &a_fy1, &a_fx2, &a_fy2, &a_fx3, &a_fy3, &a_fx4, &a_fy4);

	double fminx1, fminy1, fmaxx1, fmaxy1;
	fminx1 = MIN(fx1, fx2);
	fminx1 = MIN(fminx1, fx3);
	fminx1 = MIN(fminx1, fx4);
	fminy1 = MIN(fy1, fy2);
	fminy1 = MIN(fminy1, fy3);
	fminy1 = MIN(fminy1, fy4);
	fmaxx1 = MAX(fx1, fx2);
	fmaxx1 = MAX(fmaxx1, fx3);
	fmaxx1 = MAX(fmaxx1, fx4);
	fmaxy1 = MAX(fy1, fy2);
	fmaxy1 = MAX(fmaxy1, fy3);
	fmaxy1 = MAX(fmaxy1, fy4);
	double fminx2, fminy2, fmaxx2, fmaxy2;
	fminx2 = MIN(a_fx1, a_fx2);
	fminx2 = MIN(fminx2, a_fx3);
	fminx2 = MIN(fminx2, a_fx4);
	fminy2 = MIN(a_fy1, a_fy2);
	fminy2 = MIN(fminy2, a_fy3);
	fminy2 = MIN(fminy2, a_fy4);
	fmaxx2 = MAX(a_fx1, a_fx2);
	fmaxx2 = MAX(fmaxx2, a_fx3);
	fmaxx2 = MAX(fmaxx2, a_fx4);
	fmaxy2 = MAX(a_fy1, a_fy2);
	fmaxy2 = MAX(fmaxy2, a_fy3);
	fmaxy2 = MAX(fmaxy2, a_fy4);

	double d = drc;	
	if((fminx1 > fmaxx2)&&((fminx1 - fmaxx2) > d)) return CONNECT_NO;
	if((fmaxx1 < fminx2)&&((fminx2 - fmaxx1) > d)) return CONNECT_NO;
	if((fminy1 > fmaxy2)&&((fminy1 - fmaxy2) > d)) return CONNECT_NO;
	if((fmaxy1 < fminy2)&&((fminy2 - fmaxy1) > d)) return CONNECT_NO;

	BOOL b;
	b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, fx1, fy1, fx3, fy3, d);
	if(b == TRUE) return CONNECT_YES;
	b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, fx1, fy1, fx3, fy3, d);
	if(b == TRUE) return CONNECT_YES;	
	b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, fx1, fy1, fx3, fy3, d);
	if(b == TRUE) return CONNECT_YES;					
	b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, fx1, fy1, fx3, fy3, d);
	if(b == TRUE) return CONNECT_YES;
					
	b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, fx3, fy3, fx2, fy2, d);
	if(b == TRUE) return CONNECT_YES;
	b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, fx3, fy3, fx2, fy2, d);
	if(b == TRUE) return CONNECT_YES;	
	b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, fx3, fy3, fx2, fy2, d);
	if(b == TRUE) return CONNECT_YES;					
	b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, fx3, fy3, fx2, fy2, d);
	if(b == TRUE) return CONNECT_YES;

	b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, fx2, fy2, fx4, fy4, d);
	if(b == TRUE) return CONNECT_YES;
	b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, fx2, fy2, fx4, fy4, d);
	if(b == TRUE) return CONNECT_YES;	
	b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, fx2, fy2, fx4, fy4, d);
	if(b == TRUE) return CONNECT_YES;					
	b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, fx2, fy2, fx4, fy4, d);
	if(b == TRUE) return CONNECT_YES;

	b = Dist_Line_Line(a_fx1, a_fy1, a_fx3, a_fy3, fx4, fy4, fx1, fy1, d);
	if(b == TRUE) return CONNECT_YES;
	b = Dist_Line_Line(a_fx3, a_fy3, a_fx2, a_fy2, fx4, fy4, fx1, fy1, d);
	if(b == TRUE) return CONNECT_YES;	
	b = Dist_Line_Line(a_fx2, a_fy2, a_fx4, a_fy4, fx4, fy4, fx1, fy1, d);
	if(b == TRUE) return CONNECT_YES;					
	b = Dist_Line_Line(a_fx4, a_fy4, a_fx1, a_fy1, fx4, fy4, fx1, fy1, d);
	if(b == TRUE) return CONNECT_YES;

	return CONNECT_NO;
}

BOOL CESDView::Fill_Region_Connect(COb_pcbfill* pfill, COb_pcbregion* pregion, double drc)
{
	double fminx1, fminy1, fmaxx1, fmaxy1;
	fminx1 = 100000; fminy1 = 100000;
	fmaxx1 = -100000; fmaxy1 = -100000;
	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex;
		vertex = pregion->m_cVertex.GetAt(i);
		fminx1 = MIN(fminx1, vertex.fx);
		fminy1 = MIN(fminy1, vertex.fy);
		fmaxx1 = MAX(fmaxx1, vertex.fx);
		fmaxy1 = MAX(fmaxy1, vertex.fy);
	}
	
	double fx1, fy1, fx2, fy2, fx3, fy3, fx4, fy4;
	pfill->GetFillCorner(&fx1, &fy1, &fx2, &fy2, &fx3, &fy3, &fx4, &fy4);
	
	double fminx2, fminy2, fmaxx2, fmaxy2;
	fminx2 = MIN(fx1, fx2);
	fminx2 = MIN(fminx2, fx3);
	fminx2 = MIN(fminx2, fx4);
	fminy2 = MIN(fy1, fy2);
	fminy2 = MIN(fminy2, fy3);
	fminy2 = MIN(fminy2, fy4);
	fmaxx2 = MAX(fx1, fx2);
	fmaxx2 = MAX(fmaxx2, fx3);
	fmaxx2 = MAX(fmaxx2, fx4);
	fmaxy2 = MAX(fy1, fy2);
	fmaxy2 = MAX(fmaxy2, fy3);
	fmaxy2 = MAX(fmaxy2, fy4);

	double d = drc;	
	if((fminx1 > fmaxx2)&&((fminx1 - fmaxx2) > d)) return CONNECT_NO;
	if((fmaxx1 < fminx2)&&((fminx2 - fmaxx1) > d)) return CONNECT_NO;
	if((fminy1 > fmaxy2)&&((fminy1 - fmaxy2) > d)) return CONNECT_NO;
	if((fmaxy1 < fminy2)&&((fminy2 - fmaxy1) > d)) return CONNECT_NO;

	if(pregion->Is_On_Me(fx1, fy1) == TRUE) return CONNECT_YES;
	if(pregion->Is_On_Me(fx2, fy2) == TRUE) return CONNECT_YES;
	if(pregion->Is_On_Me(fx3, fy3) == TRUE) return CONNECT_YES;
	if(pregion->Is_On_Me(fx4, fy4) == TRUE) return CONNECT_YES;

	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex1, vertex2;
		vertex1 = pregion->m_cVertex.GetAt(i);
		if(i < (pregion->m_cVertex.GetCount()-1)) vertex2 = pregion->m_cVertex.GetAt(i+1);
		else vertex2 = pregion->m_cVertex.GetAt(0);

		BOOL b;
		b = Dist_Line_Line(fx1, fy1, fx3, fy3, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
		if(b == TRUE) return CONNECT_YES;
		b = Dist_Line_Line(fx3, fy3, fx2, fy2, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
		if(b == TRUE) return CONNECT_YES;
		b = Dist_Line_Line(fx2, fy2, fx4, fy4, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
		if(b == TRUE) return CONNECT_YES;
		b = Dist_Line_Line(fx4, fy4, fx1, fy1, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
		if(b == TRUE) return CONNECT_YES;
	}
	for(int i=0; i<pregion->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex;
		vertex = pregion->m_cVertex.GetAt(i);

		if(vertex.radius != 0)
		{
			BOOL b;
			b = Dist_Line_Arc(fx1, fy1, fx3, fy3, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_Line_Arc(fx3, fy3, fx2, fy2, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_Line_Arc(fx2, fy2, fx4, fy4, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
			if(b == TRUE) return CONNECT_YES;
			b = Dist_Line_Arc(fx4, fy4, fx1, fy1, vertex.cx, vertex.cy, vertex.radius, vertex.sangle, vertex.eangle, d);
			if(b == TRUE) return CONNECT_YES;
		}
	}

	return CONNECT_NO;
}

BOOL CESDView::Region_Region_Connect(COb_pcbregion* pregion1, COb_pcbregion* pregion2, double drc)
{
	double fminx1, fminy1, fmaxx1, fmaxy1;
	fminx1 = 100000; fminy1 = 100000;
	fmaxx1 = -100000; fmaxy1 = -100000;
	for(int i=0; i<pregion1->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex;
		vertex = pregion1->m_cVertex.GetAt(i);
		fminx1 = MIN(fminx1, vertex.fx);
		fminy1 = MIN(fminy1, vertex.fy);
		fmaxx1 = MAX(fmaxx1, vertex.fx);
		fmaxy1 = MAX(fmaxy1, vertex.fy);
	}
	
	double fminx2, fminy2, fmaxx2, fmaxy2;
	fminx2 = 100000; fminy2 = 100000;
	fmaxx2 = -100000; fmaxy2 = -100000;
	for(int i=0; i<pregion2->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex;
		vertex = pregion2->m_cVertex.GetAt(i);
		fminx2 = MIN(fminx2, vertex.fx);
		fminy2 = MIN(fminy2, vertex.fy);
		fmaxx2 = MAX(fmaxx2, vertex.fx);
		fmaxy2 = MAX(fmaxy2, vertex.fy);
	}
	
	double d = drc;	
	if((fminx1 > fmaxx2)&&((fminx1 - fmaxx2) > d)) return CONNECT_NO;
	if((fmaxx1 < fminx2)&&((fminx2 - fmaxx1) > d)) return CONNECT_NO;
	if((fminy1 > fmaxy2)&&((fminy1 - fmaxy2) > d)) return CONNECT_NO;
	if((fmaxy1 < fminy2)&&((fminy2 - fmaxy1) > d)) return CONNECT_NO;
	
	for(int i=0; i<pregion1->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex;
		vertex = pregion1->m_cVertex.GetAt(i);
		if(pregion2->Is_On_Me(vertex.fx, vertex.fy) == TRUE) return CONNECT_YES;
	}
	
	for(int i=0; i<pregion2->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  vertex;
		vertex = pregion2->m_cVertex.GetAt(i);
		if(pregion1->Is_On_Me(vertex.fx, vertex.fy) == TRUE) return CONNECT_YES;
	}
	
	for(int i=0; i<pregion1->m_cVertex.GetCount(); i++)
	{
		Struct_RegionVertex  a_vertex1, a_vertex2;
		a_vertex1 = pregion1->m_cVertex.GetAt(i);
		if(i < (pregion1->m_cVertex.GetCount()-1)) a_vertex2 = pregion1->m_cVertex.GetAt(i+1);
		else a_vertex2 = pregion1->m_cVertex.GetAt(0);

		for(int j=0; j<pregion2->m_cVertex.GetCount(); j++)
		{
			Struct_RegionVertex  vertex1, vertex2;
			vertex1 = pregion2->m_cVertex.GetAt(j);
			if(j < (pregion2->m_cVertex.GetCount()-1)) vertex2 = pregion2->m_cVertex.GetAt(j+1);
			else vertex2 = pregion2->m_cVertex.GetAt(0);

			BOOL b = Dist_Line_Line(a_vertex1.fx, a_vertex1.fy, a_vertex2.fx, a_vertex2.fy, vertex1.fx, vertex1.fy, vertex2.fx, vertex2.fy, d);
			if(b == TRUE) return CONNECT_YES;
		}
	}

	return CONNECT_NO;
}

BOOL CESDView::Dist_P_P(double fx1, double fy1, double fx2, double fy2, double d)
{ 
	double ftemp;
	
	if((fx1 - fx2) > d) return CONNECT_NO;
	if((fx2 - fx1) > d) return CONNECT_NO;
	if((fy1 - fy2) > d) return CONNECT_NO;
	if((fy2 - fy1) > d) return CONNECT_NO;

	ftemp = sqrt((fx1 - fx2)*(fx1 - fx2) + (fy1 - fy2)*(fy1 - fy2));
	if(ftemp <= d) return CONNECT_YES;
	else return CONNECT_NO;
}

BOOL CESDView::Dist_P_Line(double fx1, double fy1, double fxs, double fys, double fxe, double fye, double d)
{ 
	double ftemp;
	double m,f;
	double ftempx, ftempy;

  	if((fxe == fxs)&&(fye == fys)) return Dist_P_P(fx1, fy1, fxs, fys, d);
	
	if((MIN(fxs, fxe) - fx1) > d) return CONNECT_NO;
	if((fx1 - MAX(fxs, fxe)) > d) return CONNECT_NO;
	if((MIN(fys, fye) - fy1) > d) return CONNECT_NO;
	if((fy1 - MAX(fys, fye)) > d) return CONNECT_NO;

	
	if(fxe == fxs)	
	{
		if(((fy1 >= fys)&&(fy1 <= fye))||((fy1 >= fye)&&(fy1 <= fys)))
		{
			if(fxs >= fx1) ftemp = fxs - fx1;
			else ftemp = fx1 - fxs;
			if(ftemp <= d) return CONNECT_YES;
			else return CONNECT_NO;
		}
		else
		{ 
			if(fys > fye)
			{
				if(fy1 > fys)
				{
					if((abs(fx1 - fxs) > d)||(abs(fy1 - fys) > d)) return CONNECT_NO;
					ftemp = sqrt((fx1 - fxs)*(fx1 - fxs) + (fy1 - fys)*(fy1 - fys));
				}
				else
				{
					if((abs(fx1 - fxe) > d)||(abs(fy1 - fye) > d)) return CONNECT_NO;
					ftemp = sqrt((fx1 - fxe)*(fx1 - fxe) + (fy1 - fye)*(fy1 - fye));
				}
			}
			else
			{
				if(fy1 < fys)
				{
					if((abs(fx1 - fxs) > d)||(abs(fy1 - fys) > d)) return CONNECT_NO;
					ftemp = sqrt((fx1 - fxs)*(fx1 - fxs) + (fy1 - fys)*(fy1 - fys));
				}
				else
				{
					if((abs(fx1 - fxe) > d)||(abs(fy1 - fye) > d)) return CONNECT_NO;
					ftemp = sqrt((fx1 - fxe)*(fx1 - fxe)+(fy1 - fye)*(fy1 - fye));
				}
			}
			if(ftemp <= d) return CONNECT_YES;
			else return CONNECT_NO;
		}
	}
	else if(fye == fys)		
	{
		if(((fx1 >= fxs)&&(fx1 <= fxe))||((fx1 >= fxe)&&(fx1 <= fxs)))
		{
			if(fys >= fy1) ftemp = fys - fy1;
			else ftemp = fy1 - fys;
			if(ftemp<=d) return CONNECT_YES;
			else return CONNECT_NO;
		}
		else
		{
			if(fxs >= fxe)
			{ 
				if(fx1 > fxs)
				{
					if((abs(fx1 - fxs) > d)||(abs(fy1 - fys) > d)) return CONNECT_NO;
					ftemp = sqrt((fx1 - fxs)*(fx1 - fxs) + (fy1 - fys)*(fy1 - fys));
				}
				else
				{
					if((abs(fx1 - fxe) > d)||(abs(fy1 - fye) > d)) return CONNECT_NO;
					ftemp = sqrt((fx1 - fxe)*(fx1 - fxe) + (fy1 - fye)*(fy1 - fye));
				}
			}
			else
			{
				if(fx1 < fxs)
				{
					if((abs(fx1 - fxs) > d)||(abs(fy1 - fys) > d)) return CONNECT_NO;
					ftemp = sqrt((fx1 - fxs)*(fx1 - fxs) + (fy1 - fys)*(fy1 - fys));
				}
				else
				{
					if((abs(fx1 - fxe) > d)||(abs(fy1 - fye) > d)) return CONNECT_NO;
					ftemp = sqrt((fx1 - fxe)*(fx1 - fxe) + (fy1 - fye)*(fy1 - fye));
				}
			}
			if(ftemp <= d) return CONNECT_YES;
			else return CONNECT_NO;
		}
	}
	else
	{
		m = (fye - fys)/(fxe - fxs);
		f = (m*m*fxs + fx1 - m*fys + m*fy1)/(m*m + 1);
		ftempx = f;
		ftempy = m*(f - fxs) + fys;
		if( ((ftempx >= fxs)&&(ftempx <= fxe)&&(ftempy >= fys)&&(ftempy <= fye)) || ((ftempx >= fxs)&&(ftempx <= fxe)&&(ftempy >= fye)&&(ftempy <= fys))||\
			((ftempx >= fxe)&&(ftempx <= fxs)&&(ftempy >= fys)&&(ftempy <= fye)) || ((ftempx >= fxe)&&(ftempx <= fxs)&&(ftempy >= fye)&&(ftempy <= fys)) )
		{
			if((abs(fx1 - ftempx) > d) || (abs(fy1 - ftempy) > d)) return CONNECT_NO;
			ftemp = sqrt((fx1 - ftempx)*(fx1 - ftempx) + (fy1 - ftempy)*(fy1 - ftempy));
			if(ftemp <= d) return CONNECT_YES;
			else return CONNECT_NO;
		}
		else
		{
			ftemp=sqrt((fx1 - fxs)*(fx1 - fxs) + (fy1 - fys)*(fy1 - fys));
			if(ftemp <= d) return CONNECT_YES;
			ftemp = sqrt((fx1 - fxe)*(fx1 - fxe) + (fy1 - fye)*(fy1 - fye));
			if(ftemp <= d) return CONNECT_YES;
			else return CONNECT_NO;
		}
	}
}

BOOL CESDView::Dist_Line_Line(double aaa_fx1, double aaa_fy1, double aaa_fx2, double aaa_fy2, double bbb_fx1, double bbb_fy1, double bbb_fx2, double bbb_fy2, double d)
{  
	double ftemp;
	double k1, k2;
	double ftempx,ftempy;

	if(((aaa_fx1 == bbb_fx1)&&(aaa_fy1 == bbb_fy1))||((aaa_fx2 == bbb_fx1)&&(aaa_fy2 == bbb_fy1))||((aaa_fx1 == bbb_fx2)&&(aaa_fy1 == bbb_fy2))||((aaa_fx2 == bbb_fx2)&&(aaa_fy2 == bbb_fy2))) return CONNECT_YES;


	if((aaa_fx1 == aaa_fx2)&&(bbb_fx1 == bbb_fx2))	
	{
		if((((aaa_fy1 >= bbb_fy1)&&(aaa_fy1 <= bbb_fy2))||((aaa_fy1 >= bbb_fy2)&&(aaa_fy1 <= bbb_fy1)))||(((aaa_fy2 >= bbb_fy1)&&(aaa_fy2 <= bbb_fy2))||((aaa_fy2 >= bbb_fy2)&&(aaa_fy2 <= bbb_fy1))))
		{  
			if(aaa_fx1 >= bbb_fx1) ftemp = aaa_fx1 - bbb_fx1;
			else ftemp = bbb_fx1 - aaa_fx1;
		}
		else if((((bbb_fy1 >= aaa_fy1)&&(bbb_fy1 <= aaa_fy2))||((bbb_fy1 >= aaa_fy2)&&(bbb_fy1 <= aaa_fy1)))||(((bbb_fy2 >= aaa_fy1)&&(bbb_fy2 <= aaa_fy2))||((bbb_fy2 >= aaa_fy2)&&(bbb_fy2 <= aaa_fy1))))
		{
			if(bbb_fx1 >= aaa_fx1) ftemp = bbb_fx1 - aaa_fx1;
			else ftemp = aaa_fx1 - bbb_fx1;
		}
		else
		{
			if(aaa_fy1 >= bbb_fy1)
			{ 
				if((aaa_fy1 >= aaa_fy2)&&(bbb_fy1 >= bbb_fy2))
				{ 
					if((abs(aaa_fx2 - bbb_fx1) > d)||(abs(aaa_fy2 - bbb_fy1) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx2 - bbb_fx1)*(aaa_fx2 - bbb_fx1) + (aaa_fy2 - bbb_fy1)*(aaa_fy2 - bbb_fy1));
				}
				else if((aaa_fy1 >= aaa_fy2)&&(bbb_fy2 >= bbb_fy1))
				{
					if((abs(aaa_fx2 - bbb_fx2) > d)||(abs(aaa_fy2 - bbb_fy2) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx2 - bbb_fx2)*(aaa_fx2 - bbb_fx2)+(aaa_fy2 - bbb_fy2)*(aaa_fy2 - bbb_fy2));
				}
				else if((aaa_fy2 >= aaa_fy1)&&(bbb_fy1 >= bbb_fy2))
				{
					if((abs(aaa_fx1 - bbb_fx1) > d)||(abs(aaa_fy1 - bbb_fy1) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx1 - bbb_fx1)*(aaa_fx1 - bbb_fx1) + (aaa_fy1 - bbb_fy1)*(aaa_fy1 - bbb_fy1));
				}
				else
				{
					if((abs(aaa_fx1 - bbb_fx2) > d)||(abs(aaa_fy1 - bbb_fy2) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx1 - bbb_fx2)*(aaa_fx1 - bbb_fx2) + (aaa_fy1 - bbb_fy2)*(aaa_fy1 - bbb_fy2));
				}
			}
			else
			{
				if((bbb_fy2 >= bbb_fy1)&&(aaa_fy2 >= aaa_fy1))
				{
					if((abs(aaa_fx2 - bbb_fx1) > d)||(abs(aaa_fy2 - bbb_fy1) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx2 - bbb_fx1)*(aaa_fx2 - bbb_fx1) + (aaa_fy2 - bbb_fy1)*(aaa_fy2 - bbb_fy1));
				}
				else if((bbb_fy1 >= bbb_fy2)&&(aaa_fy2 >= aaa_fy1))
				{
					if((abs(aaa_fx2 - bbb_fx2) > d)||(abs(aaa_fy2 - bbb_fy2) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx2 - bbb_fx2)*(aaa_fx2 - bbb_fx2) + (aaa_fy2 - bbb_fy2)*(aaa_fy2 - bbb_fy2));
				}
				else if((bbb_fy2 >= bbb_fy1)&&(aaa_fy1 >= aaa_fy2))
				{
					if((abs(aaa_fx1 - bbb_fx1) > d)||(abs(aaa_fy1 - bbb_fy1) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx1 - bbb_fx1)*(aaa_fx1 - bbb_fx1) + (aaa_fy1 - bbb_fy1)*(aaa_fy1 - bbb_fy1));
				}
				else
				{
					if((abs(aaa_fx1 - bbb_fx2) > d)||(abs(aaa_fy1 - bbb_fy2) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx1 - bbb_fx2)*(aaa_fx1 - bbb_fx2) + (aaa_fy1 - bbb_fy2)*(aaa_fy1 - bbb_fy2));
				}
			}
		}
		if(ftemp <= d) return CONNECT_YES;
		else return CONNECT_NO;
	}
	
	else if((aaa_fy1 == aaa_fy2)&&(bbb_fy1 == bbb_fy2))
	{
		if( (((aaa_fx1 >= bbb_fx1)&&(aaa_fx1 <= bbb_fx2)) || ((aaa_fx1 >= bbb_fx2)&&(aaa_fx1 <= bbb_fx1))) || (((aaa_fx2 >= bbb_fx1)&&(aaa_fx2 <= bbb_fx2)) || ((aaa_fx2 >= bbb_fx2)&&(aaa_fx2 <= bbb_fx1))) )
		{
			if(aaa_fy1 >= bbb_fy1) ftemp = aaa_fy1 - bbb_fy1;
			else ftemp = bbb_fy1 - aaa_fy1;
		}
		else if( (((bbb_fx1 >= aaa_fx1)&&(bbb_fx1 <= aaa_fx2)) || ((bbb_fx1 >= aaa_fx2)&&(bbb_fx1 <= aaa_fx1))) || (((bbb_fx2 >= aaa_fx1)&&(bbb_fx2 <= aaa_fx2)) || ((bbb_fx2 >= aaa_fx2)&&(bbb_fx2 <= aaa_fx1))) )
		{
			if(bbb_fy1 >= aaa_fy1) ftemp = bbb_fy1 - aaa_fy1;
			else ftemp = aaa_fy1 - bbb_fy1;
		}
		else
		{
			if(aaa_fx1 >= bbb_fx1)
			{
				if((aaa_fx1 >= aaa_fx2)&&(bbb_fx1 >= bbb_fx2))
				{
					if((abs(aaa_fx2 - bbb_fx1) > d)||(abs(aaa_fy2 - bbb_fy1) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx2 - bbb_fx1)*(aaa_fx2 - bbb_fx1)+(aaa_fy2 - bbb_fy1)*(aaa_fy2 - bbb_fy1));
				}
				else if((aaa_fx1 >= aaa_fx2)&&(bbb_fx2 >= bbb_fx1))
				{
					if((abs(aaa_fx2 - bbb_fx2) > d)||(abs(aaa_fy2 - bbb_fy2) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx2 - bbb_fx2)*(aaa_fx2 - bbb_fx2) + (aaa_fy2 - bbb_fy2)*(aaa_fy2 - bbb_fy2));
				}
				else if((aaa_fx2 >= aaa_fx1)&&(bbb_fx1 >= bbb_fx2))
				{
					if((abs(aaa_fx1 - bbb_fx1) > d)||(abs(aaa_fy1 - bbb_fy1) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx1 - bbb_fx1)*(aaa_fx1 - bbb_fx1) + (aaa_fy1 - bbb_fy1)*(aaa_fy1 - bbb_fy1));
				}
				else
				{
					if((abs(aaa_fx1 - bbb_fx2) > d)||(abs(aaa_fy1 - bbb_fy2) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx1 - bbb_fx2)*(aaa_fx1 - bbb_fx2) + (aaa_fy1 - bbb_fy2)*(aaa_fy1 - bbb_fy2));
				}
			}
			else
			{
				if((bbb_fx2 >= bbb_fx1)&&(aaa_fx2 >= aaa_fx1))
				{
					if((abs(aaa_fx2 - bbb_fx1) > d)||(abs(aaa_fy2 - bbb_fy1) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx2 - bbb_fx1)*(aaa_fx2 - bbb_fx1) + (aaa_fy2 - bbb_fy1)*(aaa_fy2 - bbb_fy1));
				}
				else if((bbb_fx1 >= bbb_fx2)&&(aaa_fx2 >= aaa_fx1))
				{
					if((abs(aaa_fx2 - bbb_fx2) > d)||(abs(aaa_fy2 - bbb_fy2) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx2 - bbb_fx2)*(aaa_fx2 - bbb_fx2) + (aaa_fy2 - bbb_fy2)*(aaa_fy2 - bbb_fy2));
				}
				else if((bbb_fx2 >= bbb_fx1)&&(aaa_fx1 >= aaa_fx2))
				{
					if((abs(aaa_fx1 - bbb_fx1) > d)||(abs(aaa_fy1 - bbb_fy1) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx1 - bbb_fx1)*(aaa_fx1 - bbb_fx1) + (aaa_fy1 - bbb_fy1)*(aaa_fy1 - bbb_fy1));
				}
				else
				{
					if((abs(aaa_fx1 - bbb_fx2) > d)||(abs(aaa_fy1 - bbb_fy2) > d)) return CONNECT_NO;
					ftemp = sqrt((aaa_fx1 - bbb_fx2)*(aaa_fx1 - bbb_fx2) + (aaa_fy1 - bbb_fy2)*(aaa_fy1 - bbb_fy2));
				}
			}
		}
		if(ftemp<=d) return CONNECT_YES;
		else return CONNECT_NO;
	}
	else if(aaa_fx1 == aaa_fx2)
	{
		k2 = (bbb_fy2 - bbb_fy1)/(bbb_fx2 - bbb_fx1);
		ftempx = aaa_fx2;
		ftempy = k2*(aaa_fx2 - bbb_fx2) + bbb_fy2;

		if( (ftempy <= MAX(aaa_fy2, aaa_fy1))&&(ftempy >= MIN(aaa_fy2, aaa_fy1))&&\
			(ftempx <= MAX(bbb_fx2, bbb_fx1))&&(ftempx >= MIN(bbb_fx2, bbb_fx1))&&\
			(ftempy <= MAX(bbb_fy2, bbb_fy1))&&(ftempy >= MIN(bbb_fy2, bbb_fy1)))	return CONNECT_YES;
	}

	else if(bbb_fx1 == bbb_fx2)
	{
		k1 = (aaa_fy2 - aaa_fy1)/(aaa_fx2 - aaa_fx1);
		ftempx = bbb_fx2;
		ftempy = k1*(bbb_fx2 - aaa_fx2) + aaa_fy2;

		if( (ftempy <= MAX(bbb_fy2, bbb_fy1))&&(ftempy >= MIN(bbb_fy2, bbb_fy1))&&\
			(ftempx <= MAX(aaa_fx2, aaa_fx1))&&(ftempx >= MIN(aaa_fx2, aaa_fx1))&&\
			(ftempy <= MAX(aaa_fy2, aaa_fy1))&&(ftempy >= MIN(aaa_fy2, aaa_fy1)))    return CONNECT_YES;
    }
	else if(aaa_fy1 == aaa_fy2)
	{
		k2 = (bbb_fy2 - bbb_fy1)/(bbb_fx2 - bbb_fx1);
		ftempx = (k2*bbb_fx2 + aaa_fy2 - bbb_fy2)/k2;
		ftempy = aaa_fy2;

		if( (ftempx <= MAX(bbb_fx1, bbb_fx2))&&(ftempx >= MIN(bbb_fx1, bbb_fx2))&&\
			(ftempy <= MAX(bbb_fy2, bbb_fy1))&&(ftempy >= MIN(bbb_fy2, bbb_fy1))&&\
			(ftempx <= MAX(aaa_fx2, aaa_fx1))&&(ftempx >= MIN(aaa_fx2, aaa_fx1)) )	return CONNECT_YES;
	}
	else if(bbb_fy1 == bbb_fy2)
	{
		k1 = (aaa_fy2 - aaa_fy1)/(aaa_fx2 - aaa_fx1);
		ftempx = (k1*aaa_fx2 - aaa_fy2 + bbb_fy2)/k1;
		ftempy = bbb_fy2; 

		if( (ftempx <= MAX(bbb_fx1, bbb_fx2))&&(ftempx >= MIN(bbb_fx1, bbb_fx2))&&\
			(ftempx <= MAX(aaa_fx2, aaa_fx1))&&(ftempx >= MIN(aaa_fx2, aaa_fx1))&&\
			(ftempy <= MAX(aaa_fy2, aaa_fy1))&&(ftempy >= MIN(aaa_fy2, aaa_fy1)) )	return CONNECT_YES;
	}
	else
	{
		k1 = (aaa_fy2 - aaa_fy1)/(aaa_fx2 - aaa_fx1);
		k2 = (bbb_fy2 - bbb_fy1)/(bbb_fx2 - bbb_fx1);
		if(k1 != k2)
		{
			ftempx = (k1*aaa_fx2 - k2*bbb_fx2 - aaa_fy2 + bbb_fy2)/(k1-k2);
			ftempy = (k1*k2*aaa_fx2 - k1*k2*bbb_fx2 + k1*bbb_fy2 - k2*aaa_fy2)/(k1-k2);

			if( (ftempx <= MAX(bbb_fx1, bbb_fx2))&&(ftempx >= MIN(bbb_fx1, bbb_fx2))&&\
				(ftempy <= MAX(bbb_fy2, bbb_fy1))&&(ftempy >= MIN(bbb_fy2, bbb_fy1))&&\
				(ftempx <= MAX(aaa_fx2, aaa_fx1))&&(ftempx >= MIN(aaa_fx2, aaa_fx1))&&\
				(ftempy <= MAX(aaa_fy2, aaa_fy1))&&(ftempy >= MIN(aaa_fy2, aaa_fy1)) )  return CONNECT_YES;
		}
	}

	BOOL b = Dist_P_Line(aaa_fx1, aaa_fy1, bbb_fx1, bbb_fy1, bbb_fx2, bbb_fy2, d);
	if(b == CONNECT_YES) return CONNECT_YES;

	b = Dist_P_Line(aaa_fx2, aaa_fy2, bbb_fx1, bbb_fy1, bbb_fx2, bbb_fy2, d);
	if(b == CONNECT_YES) return CONNECT_YES;

	b = Dist_P_Line(bbb_fx1, bbb_fy1, aaa_fx1, aaa_fy1, aaa_fx2, aaa_fy2, d);
	if(b == CONNECT_YES) return CONNECT_YES;

	b = Dist_P_Line(bbb_fx2, bbb_fy2, aaa_fx1, aaa_fy1, aaa_fx2, aaa_fy2, d);
	if(b == CONNECT_YES) return CONNECT_YES;

	return CONNECT_NO;
}

BOOL CESDView::Dist_P_Arc(double fx, double fy, double fcx, double fcy, double r, double sangle, double eangle, double d)
{ 
	double amaxx = fcx + r;
	double aminx = fcx - r;
	double amaxy = fcy + r;
	double aminy = fcy - r;
	if((fx > amaxx)&&((fx - amaxx) > d)) return CONNECT_NO;
	if((fx < aminx)&&((aminx - fx) > d)) return CONNECT_NO;
	if((fy > amaxy)&&((fy - amaxy) > d)) return CONNECT_NO;
	if((fy < aminy)&&((aminy - fy) > d)) return CONNECT_NO;
	
	if((sangle == eangle)||(abs(sangle - eangle) == 360))
	{
		double dis = abs(r - sqrt((fx - fcx)*(fx - fcx) + (fy - fcy)*(fy - fcy)));
		if(dis > d) return CONNECT_NO;
		else return CONNECT_YES;
	}
	else
	{
		double alpha;
		if(fx == fcx)
		{
			if(fy > fcy) alpha = 90.0f;
			else alpha = 270.0f;
		}
		else if(fy == fcy) 
		{
			if(fx > fcx) alpha = 0;
			else alpha = 180;
		}
		else
		{
			alpha = 180/fEDA_pi*atan((fy - fcy)/(fx - fcx));
			if(fx < fcx) alpha += 180;
		}
		if(alpha < 0) alpha += 360;

		if(IsOnArc(alpha, sangle, eangle) == TRUE)
		{
			double dis = abs(r - sqrt((fx - fcx)*(fx - fcx) + (fy - fcy)*(fy - fcy)));
			if(dis <=  d) return CONNECT_YES;
		}

		double sx = fcx + r*cos(sangle*fEDA_pi/180);
		double sy = fcy + r*sin(sangle*fEDA_pi/180);
		double dis = sqrt((fx - sx)*(fx - sx) + (fy - sy)*(fy - sy));
		if(dis <= d) return CONNECT_YES;

		double ex = fcx + r*cos(eangle*fEDA_pi/180);
		double ey = fcy + r*sin(eangle*fEDA_pi/180);
		dis = sqrt((fx - ex)*(fx - ex) + (fy - ey)*(fy - ey));
		if(dis <= d) return CONNECT_YES;
	}

	return  CONNECT_NO;
}

BOOL CESDView::Dist_Line_Arc(double fx1, double fy1, double fx2, double fy2, double fcx, double fcy, double r, double sangle, double eangle, double d)
{ 
	double lmaxx = MAX(fx1, fx2);
	double lminx = MIN(fx1, fx2);
	double lmaxy = MAX(fy1, fy2);
	double lminy = MIN(fy1, fy2);
	double amaxx = fcx + r;
	double aminx = fcx - r;
	double amaxy = fcy + r;
	double aminy = fcy - r;
	if((lminx - amaxx) > d) return CONNECT_NO;
	if((aminx - lmaxx) > d) return CONNECT_NO;
	if((lminy - amaxy) > d) return CONNECT_NO;
	if((aminy - lmaxy) > d) return CONNECT_NO;

	if(Dist_P_Line(fcx, fcy, fx1, fy1, fx2, fy2, d+r) == CONNECT_NO) return CONNECT_NO;

	if(fx1 == fx2)
	{
		double fx, fy, alpha;
		
		fx = fx1;
		fy = fcy + sqrt(r*r - (fx1 - fcx)*(fx1 - fcx));

		if(fx == fcx)
		{
			if(fy > fcy) alpha = 90.0f;
			else alpha = 270.0f;
		}
		else if(fy == fcy) 
		{
			if(fx > fcx) alpha = 0;
			else alpha = 180;
		}
		else
		{
			alpha = 180/fEDA_pi*atan((fy - fcy)/(fx - fcx));
			if(fx < fcx) alpha += 180;
		}
		if(alpha < 0) alpha += 360;

		if((IsOnArc(alpha, sangle, eangle) == TRUE)&&(fx <= lmaxx)&&(fx >= lminx)&&(fy <= lmaxy)&&(fy >= lminy)) return CONNECT_YES;

		
		fx = fx1;
		fy = fcy - sqrt(r*r - (fx1 - fcx)*(fx1 - fcx));

		if(fx == fcx)
		{
			if(fy > fcy) alpha = 90.0f;
			else alpha = 270.0f;
		}
		else if(fy == fcy) 
		{
			if(fx > fcx) alpha = 0;
			else alpha = 180;
		}
		else
		{
			alpha = 180/fEDA_pi*atan((fy - fcy)/(fx - fcx));
			if(fx < fcx) alpha += 180;
		}
		if(alpha < 0) alpha += 360;		
		
		if((IsOnArc(alpha, sangle, eangle) == TRUE)&&(fx <= lmaxx)&&(fx >= lminx)&&(fy <= lmaxy)&&(fy >= lminy)) return CONNECT_YES;
				
		
		fx = fcx + r*cos(sangle*fEDA_pi/180);
		fy = fcy + r*sin(sangle*fEDA_pi/180);
		BOOL b = Dist_P_Line(fx, fy, fx1, fy1, fx2, fy2, d);
		if(b == TRUE) return CONNECT_YES;

		
		fx = fcx + r*cos(eangle*fEDA_pi/180);
		fy = fcy + r*sin(eangle*fEDA_pi/180);
		b = Dist_P_Line(fx, fy, fx1, fy1, fx2, fy2, d);
		if(b == TRUE) return CONNECT_YES;

		
		b = Dist_P_Arc(fx1, fy1, fcx, fcy, r, sangle, eangle, d);
		if(b == TRUE) return CONNECT_YES;

		
		b = Dist_P_Arc(fx2, fy2, fcx, fcy, r, sangle, eangle, d);
		if(b == TRUE) return CONNECT_YES;

		
		fx = fx1;
		fy = fcy;
		double pd = fabs(fx - fcx);
		
		if(fx == fcx)
		{
			if(fy > fcy) alpha = 90.0f;
			else alpha = 270.0f;
		}
		else if(fy == fcy) 
		{
			if(fx > fcx) alpha = 0;
			else alpha = 180;
		}
		else
		{
			alpha = 180/fEDA_pi*atan((fy - fcy)/(fx - fcx));
			if(fx < fcx) alpha += 180;
		}
		if(alpha < 0) alpha += 360;		
		
		if((IsOnArc(alpha, sangle, eangle) == TRUE)&&(fx <= lmaxx)&&(fx >= lminx)&&(fy <= lmaxy)&&(fy >= lminy)&&(fabs(pd-r)<d)) return CONNECT_YES;
	}
	else if(fy1 == fy2)
	{
		double fx, fy, alpha;
		
		fx = fcx + sqrt(r*r - (fy1 - fcy)*(fy1 - fcy));
		fy = fy1;

		if(fx == fcx)
		{
			if(fy > fcy) alpha = 90.0f;
			else alpha = 270.0f;
		}
		else if(fy == fcy) 
		{
			if(fx > fcx) alpha = 0;
			else alpha = 180;
		}
		else
		{
			alpha = 180/fEDA_pi*atan((fy - fcy)/(fx - fcx));
			if(fx < fcx) alpha += 180;
		}
		if(alpha < 0) alpha += 360;

		if((IsOnArc(alpha, sangle, eangle) == TRUE)&&(fx <= lmaxx)&&(fx >= lminx)&&(fy <= lmaxy)&&(fy >= lminy)) return CONNECT_YES;
		
		
		fx = fcx - sqrt(r*r - (fy1 - fcy)*(fy1 - fcy));
		fy = fy1;
	
		if(fx == fcx)
		{
			if(fy > fcy) alpha = 90.0f;
			else alpha = 270.0f;
		}
		else if(fy == fcy) 
		{
			if(fx > fcx) alpha = 0;
			else alpha = 180;
		}
		else
		{
			alpha = 180/fEDA_pi*atan((fy - fcy)/(fx - fcx));
			if(fx < fcx) alpha += 180;
		}
		if(alpha < 0) alpha += 360;		
		
		if((IsOnArc(alpha, sangle, eangle) == TRUE)&&(fx <= lmaxx)&&(fx >= lminx)&&(fy <= lmaxy)&&(fy >= lminy)) return CONNECT_YES;
			
		
		fx = fcx + r*cos(sangle*fEDA_pi/180);
		fy = fcy + r*sin(sangle*fEDA_pi/180);
		BOOL b = Dist_P_Line(fx, fy, fx1, fy1, fx2, fy2, d);
		if(b == TRUE) return CONNECT_YES;

		
		fx = fcx + r*cos(eangle*fEDA_pi/180);
		fy = fcy + r*sin(eangle*fEDA_pi/180);
		b = Dist_P_Line(fx, fy, fx1, fy1, fx2, fy2, d);
		if(b == TRUE) return CONNECT_YES;

		
		b = Dist_P_Arc(fx1, fy1, fcx, fcy, r, sangle, eangle, d);
		if(b == TRUE) return CONNECT_YES;

		
		b = Dist_P_Arc(fx2, fy2, fcx, fcy, r, sangle, eangle, d);
		if(b == TRUE) return CONNECT_YES;

		
		fx = fcx;
		fy = fy1;
		double pd = fabs(fy - fcy);
		
		if(fx == fcx)
		{
			if(fy > fcy) alpha = 90.0f;
			else alpha = 270.0f;
		}
		else if(fy == fcy) 
		{
			if(fx > fcx) alpha = 0;
			else alpha = 180;
		}
		else
		{
			alpha = 180/fEDA_pi*atan((fy - fcy)/(fx - fcx));
			if(fx < fcx) alpha += 180;
		}
		if(alpha < 0) alpha += 360;		
		
		if((IsOnArc(alpha, sangle, eangle) == TRUE)&&(fx <= lmaxx)&&(fx >= lminx)&&(fy <= lmaxy)&&(fy >= lminy)&&(fabs(pd-r)<d)) return CONNECT_YES;
	}
	else
	{
		double a = (fy2 - fy1)/(fx2 - fx1);
		double b = ((fcx - fx1)*(fy2 - fy1) - (fcy - fy1)*(fx2 - fx1))/(fx2 - fx1);
		double A = a*a + 1;
		double B = 2*a*b;
		double C = b*b - r*r;

		double fx, fy, alpha;
		
		fx = (sqrt(B*B - 4*A*C) - B)/(2*A);
		fy = a*fx + b;
		fx += fcx;
		fy += fcy;

		if(fx == fcx)
		{
			if(fy > fcy) alpha = 90.0f;
			else alpha = 270.0f;
		}
		else if(fy == fcy) 
		{
			if(fx > fcx) alpha = 0;
			else alpha = 180;
		}
		else
		{
			alpha = 180/fEDA_pi*atan((fy - fcy)/(fx - fcx));
			if(fx < fcx) alpha += 180;
		}
		if(alpha < 0) alpha += 360;

		if((IsOnArc(alpha, sangle, eangle) == TRUE)&&(fx <= lmaxx)&&(fx >= lminx)&&(fy <= lmaxy)&&(fy >= lminy)) return CONNECT_YES;
		
		
		fx = (0 - (sqrt(B*B - 4*A*C) + B))/(2*A);
		fy = a*fx + b;
		fx += fcx;
		fy += fcy;

		if(fx == fcx)
		{
			if(fy > fcy) alpha = 90.0f;
			else alpha = 270.0f;
		}
		else if(fy == fcy) 
		{
			if(fx > fcx) alpha = 0;
			else alpha = 180;
		}
		else
		{
			alpha = 180/fEDA_pi*atan((fy - fcy)/(fx - fcx));
			if(fx < fcx) alpha += 180;
		}
		if(alpha < 0) alpha += 360;

		if((IsOnArc(alpha, sangle, eangle) == TRUE)&&(fx <= lmaxx)&&(fx >= lminx)&&(fy <= lmaxy)&&(fy >= lminy)) return CONNECT_YES;
		
		
		fx = fcx + r*cos(sangle*fEDA_pi/180);
		fy = fcy + r*sin(sangle*fEDA_pi/180);
		
		BOOL rtn = Dist_P_Line(fx, fy, fx1, fy1, fx2, fy2, d);
		if(rtn == TRUE) return CONNECT_YES;

		
		fx = fcx + r*cos(eangle*fEDA_pi/180);
		fy = fcy + r*sin(eangle*fEDA_pi/180);
		rtn = Dist_P_Line(fx, fy, fx1, fy1, fx2, fy2, d);
		if(rtn == TRUE) return CONNECT_YES;

		
		rtn = Dist_P_Arc(fx1, fy1, fcx, fcy, r, sangle, eangle, d);
		if(rtn == TRUE) return CONNECT_YES;

		
		rtn = Dist_P_Arc(fx2, fy2, fcx, fcy, r, sangle, eangle, d);
		if(rtn == TRUE) return CONNECT_YES;

		
		fx = (fcx + a*fcy + a*a*fx1 - a*fy1)/A;
		fy = (fy1 + a*fcx + a*a*fcy - a*fx1)/A;
		double pd = SQRT(fx, fy, fcx, fcy);
		
		if(fx == fcx)
		{
			if(fy > fcy) alpha = 90.0f;
			else alpha = 270.0f;
		}
		else if(fy == fcy) 
		{
			if(fx > fcx) alpha = 0;
			else alpha = 180;
		}
		else
		{
			alpha = 180/fEDA_pi*atan((fy - fcy)/(fx - fcx));
			if(fx < fcx) alpha += 180;
		}
		if(alpha < 0) alpha += 360;		
		
		if((IsOnArc(alpha, sangle, eangle) == TRUE)&&(fx <= lmaxx)&&(fx >= lminx)&&(fy <= lmaxy)&&(fy >= lminy)&&(fabs(pd-r)<d)) return CONNECT_YES;
	}

	return  CONNECT_NO;
}


BOOL CESDView::IsOnArc(double alpha, double sangle, double eangle)
{
	if((sangle == eangle)||(abs(sangle - eangle) == 360)) return TRUE;

	if(eangle > sangle)
	{
		if((alpha > sangle)&&(alpha < eangle)) return TRUE;
	}
	else
	{
		if((alpha > sangle)||(alpha < eangle)) return TRUE;
	}

	return FALSE;
}
