void CESDView::OnPcbToolDrc()
{
	int nErrorNum;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	char filepath[MAX_PATH];
	CString path = pDoc->GetPathName();
	int index = path.ReverseFind('.');
	if(index > 0) path = path.Left(index);

	strcpy(filepath,path);
	if( strcmp(filepath,"") == 0 )
	{
		AfxMessageBox("当前文档没有定义文件名称, 请先保存文件.");
		return;
	}
	
	if(((pDoc->m_arrayCompDesignator.GetCount() <= 0)||(pDoc->m_arraySchNet.GetCount() <= 0))&&(pDoc->m_arrayPcbNet.GetCount() <= 0))
	{	
		AfxMessageBox("电路图设计和印制板设计都没有找到网络信息, 校验终止。");
		return;
	}

	if((pDoc->m_arrayCompDesignator.GetCount() <= 0)||(pDoc->m_arraySchNet.GetCount() <= 0))
	{
		if(AfxMessageBox("未找到SCH网络, 是否以PCB网络作为校验基准?", MB_YESNO|MB_ICONWARNING) != IDYES) return;
	}

	char filestring[MAX_PATH];
	sprintf(filestring,"%s.DRC", filepath);
	CFile cfileDRC;
	if(cfileDRC.Open(filestring,  CFile::modeCreate|CFile::modeWrite) != TRUE) 
	{
		AfxMessageBox("校验失败, 无法创建错误报告文件.", MB_ICONSTOP);
		return;
	}
	CArchive arDRC(&cfileDRC, CArchive::store);

	if((pDoc->m_arrayCompDesignator.GetCount() > 0)&&(pDoc->m_arraySchNet.GetCount() > 0))
	{
		nErrorNum = 0;
		arDRC.WriteString("※电路图元件和印制板元件的一致性检查 -----------------------------------------\r\n");
		Write_Help("检查元件...");
		for(int i=0; i<pDoc->m_arrayCompDesignator.GetCount(); i++)		
		{
			CString csSchCompID = pDoc->m_arrayCompDesignator.GetAt(i);
			Write_Info(csSchCompID.GetBuffer());
			
			int flag = 0;
			for(int j=0; j<pDoc->m_arrayPcbComp.GetCount(); j++)
			{
				COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(j);
				if(pcomp->m_nFlag != FLAG_NORMAL) continue;

				CString csPcbCompID = pcomp->GetPcbCompDesignator(j);
				if(csSchCompID ==csPcbCompID)
				{
					flag = 1;
					break;
				}
			}

			if(flag == 0)
			{
				nErrorNum++;
				CString cstring = "PCB丢失元件: ";
				cstring += csSchCompID;
				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)		
		{
			COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
			if(pcomp->m_nFlag != FLAG_NORMAL) continue;

			CString csPcbCompID = pcomp->GetPcbCompDesignator(i);
			Write_Info(csPcbCompID.GetBuffer());

			int flag = 0;
			for(int j=0; j<pDoc->m_arrayCompDesignator.GetCount(); j++)
			{
				CString csSchCompID = pDoc->m_arrayCompDesignator.GetAt(j);
				if(csSchCompID ==csPcbCompID)
				{
					flag = 1;
					break;
				}
			}

			if(flag == 0)
			{
				nErrorNum++;
				CString cstring = "PCB多余元件: ";
				cstring += csPcbCompID;
				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}

		if(nErrorNum == 0) arDRC.WriteString("\r\n未发现任何错误\r\n\r\n");
		else 
		{
			char str[100];
			itoa(nErrorNum, str, 10);
			CString cstring = "\r\n(";
			cstring += str;
			cstring += "个错误)\r\n\r\n";
			arDRC.WriteString(cstring);
		}
		
		nErrorNum = 0;
		arDRC.WriteString("※电路图元件引脚和印制板元件引脚的一致性检查 ---------------------------------\r\n");
		Write_Help("检查元件引脚...");
		for(int i=0; i<pDoc->m_arrayCompDesignator.GetCount(); i++)
		{
			CString csSchCompID = pDoc->m_arrayCompDesignator.GetAt(i);
			Write_Info(csSchCompID.GetBuffer());

			COb_schcomp* pschcomp = NULL;
			for(int diagram=0; diagram<SCH_SHEET_NUM; diagram++)
			{
				int num = pDoc->m_listSchObject[diagram].GetCount();
				POSITION pos;
				pos = pDoc->m_listSchObject[diagram].GetHeadPosition();
				for(int j=0; j<num; j++)
				{	
					if(pos == NULL) break;
					CObject* pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
					{
						COb_schcomp* pcomp = (COb_schcomp*)pobject;
						if((pcomp->GetDesignator() == csSchCompID)&&(pcomp->m_nFlag == FLAG_NORMAL))
						{
							pschcomp = (COb_schcomp*)pobject;
							break;
						}
					}
				}
			}

			COb_pcbcomp* ppcbcomp = NULL;
			int pcbcompIndex = -1;
			for(int j=0; j<pDoc->m_arrayPcbComp.GetCount(); j++)
			{
				COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(j);
				if(pcomp->m_nFlag != FLAG_NORMAL) continue;

				CString csPcbCompID = pcomp->GetPcbCompDesignator(j);
				if(csSchCompID ==csPcbCompID)
				{
					ppcbcomp = pcomp;
					pcbcompIndex = j;
					break;
				}
			}

			if((pschcomp != NULL)&&(ppcbcomp != NULL))
			{
				for(int j=0; j<pschcomp->m_arrayPos.GetCount(); j++)		
				{
					POSITION pos = pschcomp->m_arrayPos.GetAt(j);
					CObject* pobject = pDoc->m_listSchObject[pschcomp->m_nDiagram].GetAt(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin)))
					{
						COb_schpin* ppin = (COb_schpin*)pobject;
						int flag = 0;
						for(int k=0; k<pDoc->m_arrayPcbPad.GetCount(); k++)
						{
							COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(k);
							if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == pcbcompIndex)&&(ppad->m_csName == ppin->m_csNumber))
							{
								flag = 1;
								break;
							}
						}

						if(flag == 0)
						{
							nErrorNum++;
							CString cstring = "PCB元件 ";
							cstring += csSchCompID;
							cstring += " 丢失引脚: ";
							cstring += ppin->m_csNumber;
							cstring += "\r\n";
							arDRC.WriteString(cstring);
						}
					}
				}

				for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)		
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
					if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent != pcbcompIndex)) continue;

					int flag = 0;
					for(int k=0; k<pschcomp->m_arrayPos.GetCount(); k++)
					{
						POSITION pos = pschcomp->m_arrayPos.GetAt(k);
						CObject* pobject = pDoc->m_listSchObject[pschcomp->m_nDiagram].GetAt(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin)))
						{
							COb_schpin* ppin = (COb_schpin*)pobject;
							if(ppad->m_csName == ppin->m_csNumber)
							{
								flag = 1;
								break;
							}
						}
					}

					if(flag == 0)
					{
						nErrorNum++;
						CString cstring = "PCB元件 ";
						cstring += csSchCompID;
						cstring += " 多余引脚: ";
						cstring += ppad->m_csName;
						cstring += "\r\n";
						arDRC.WriteString(cstring);
					}
				}
			}
		}
		
		if(nErrorNum == 0) arDRC.WriteString("\r\n未发现任何错误\r\n\r\n");
		else 
		{
			char str[100];
			itoa(nErrorNum, str, 10);
			CString cstring = "\r\n(";
			cstring += str;
			cstring += "个错误)\r\n\r\n";
			arDRC.WriteString(cstring);
		}

		nErrorNum = 0;
		arDRC.WriteString("※电路图网络和印制板网络的一致性检查 -----------------------------------------\r\n");
		Write_Help("检查网络...");
		for(int i=0; i<pDoc->m_arraySchNet.GetCount(); i++)
		{
			COb_schnet* pschnet = pDoc->m_arraySchNet.GetAt(i);
			Write_Info(pschnet->m_csNetName.GetBuffer());
			
			int nNet = -1;
			for(int j=0; j<pDoc->m_arrayPcbNet.GetCount(); j++)
			{
				COb_pcbnet* ppcbnet = pDoc->m_arrayPcbNet.GetAt(j);
				if(ppcbnet->m_nFlag != FLAG_NORMAL) continue;

				if(ppcbnet->m_csNetName == pschnet->m_csNetName)
				{
					nNet = j;
					break;
				}
			}
			
			if(nNet == -1) 
			{
				nErrorNum++;
				CString cstring = "PCB丢失网络: ";
				cstring += pschnet->m_csNetName;
				cstring += "\r\n";
				arDRC.WriteString(cstring);
				continue;
			}

			for(int j=0; j<pschnet->m_arrayCompDesignator.GetCount(); j++)
			{
				CString csCompID = pschnet->m_arrayCompDesignator.GetAt(j);
				CString csPinNumber = pschnet->m_arrayPinNumber.GetAt(j);
				int flag = 0;
				for(int k=0; k<pDoc->m_arrayPcbPad.GetCount(); k++)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(k);
					if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent < 0)||(ppad->m_nNet != nNet)) continue;

					COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
					if((pcomp->GetPcbCompDesignator(ppad->m_nComponent) == csCompID)&&(ppad->m_csName == csPinNumber))
					{
						flag = 1;
						break;
					}
				}

				if(flag == 0)
				{
					nErrorNum++;
					CString cstring = "网络 ";
					cstring += pschnet->m_csNetName;
					cstring += " 不一致, PCB丢失节点: ";
					cstring += csCompID + "-" + csPinNumber;
					cstring += "\r\n";
					arDRC.WriteString(cstring);
				}
			}

			for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
				if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent < 0)||(ppad->m_nNet != nNet)) continue;

				COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
				CString csPcbCompID = pcomp->GetPcbCompDesignator(ppad->m_nComponent);
				int flag = 0;
				for(int k=0; k<pschnet->m_arrayCompDesignator.GetCount(); k++)
				{
					CString csCompID = pschnet->m_arrayCompDesignator.GetAt(k);
					CString csPinNumber = pschnet->m_arrayPinNumber.GetAt(k);
					if((csPcbCompID == csCompID)&&(ppad->m_csName == csPinNumber))
					{
						flag = 1;
						break;
					}
				}

				if(flag == 0)
				{
					nErrorNum++;
					CString cstring = "网络 ";
					cstring += pschnet->m_csNetName;
					cstring += " 不一致, PCB多余节点: ";
					cstring += csPcbCompID + "-" + ppad->m_csName;
					cstring += "\r\n";
					arDRC.WriteString(cstring);
				}
			}
		}

		if(nErrorNum == 0) arDRC.WriteString("\r\n未发现任何错误\r\n\r\n");
		else 
		{
			char str[100];
			itoa(nErrorNum, str, 10);
			CString cstring = "\r\n(";
			cstring += str;
			cstring += "个错误)\r\n\r\n";
			arDRC.WriteString(cstring);
		}
	}
		
	
	nErrorNum = 0;
	arDRC.WriteString("※印制板网络连接检查 ---------------------------------------------------------\r\n");
	Write_Help("检查连接...");
	for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;
		Write_Info(pnet->m_csNetName.GetBuffer());
			
		
		CClientDC dc(this);
		DrawPcbHighlight(&dc, 0);
		m_arrayPcbHighlight.RemoveAll();

		int sub_net_num = 0;
		int nNet = i;
		for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
			if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent < 0)||(ppad->m_nNet != nNet)) continue;

			ppad->m_nFlag = FLAG_PROCESSING;
			Struct_Pcb_Multi_Selection selected_element;
			selected_element.pcbelement = PCB_ELEMENT_PAD;
			selected_element.index = j;
			m_arrayPcbHighlight.Add(selected_element);
			PCBMakeHighlight(TRUE);

			
			for(int k=0; k<m_arrayPcbHighlight.GetCount(); k++)
			{
				Struct_Pcb_Multi_Selection selected_element = m_arrayPcbHighlight.GetAt(k);
				if(selected_element.pcbelement == PCB_ELEMENT_PAD)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
					if((ppad->m_nNet != nNet)&&(ppad->m_nComponent >= 0))
					{
						COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
						CString csCompID = pcomp->GetPcbCompDesignator(ppad->m_nComponent);
						
						nErrorNum++;
						CString cstring = "网络";
						cstring += pnet->m_csNetName;
                        cstring += "错误连接: ";
						cstring += csCompID + "-" + ppad->m_csName;
						cstring += "\r\n";
						arDRC.WriteString(cstring);
					}
				}
			}			
			
			
			sub_net_num++;
			int k;
			for(k=0; k<pDoc->m_arrayPcbPad.GetCount(); k++)
			{
				COb_pcbpad* punconnected_pad = pDoc->m_arrayPcbPad.GetAt(k);
				if((punconnected_pad->m_nFlag != FLAG_NORMAL)||(punconnected_pad->m_nComponent < 0)||(punconnected_pad->m_nNet != nNet)) continue;

				nErrorNum++;
				CString cstring = "网络";
				cstring += pnet->m_csNetName;
				cstring += "断开(子网络";
				char str[100];
				itoa(sub_net_num, str, 10);
				cstring += str;
				cstring += "): ";
				for(int l=0; l<m_arrayPcbHighlight.GetCount(); l++)
				{
					Struct_Pcb_Multi_Selection selected_element = m_arrayPcbHighlight.GetAt(l);
					if(selected_element.pcbelement == PCB_ELEMENT_PAD)
					{
						COb_pcbpad* psubpad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
						if((psubpad->m_nNet == nNet)&&(psubpad->m_nComponent >= 0))
						{
							COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(psubpad->m_nComponent);
							CString csCompID = pcomp->GetPcbCompDesignator(psubpad->m_nComponent);
							
							cstring += csCompID + "-" + psubpad->m_csName;
							cstring += " ";
						}
					}
				}

				cstring += "\r\n";
				arDRC.WriteString(cstring);

				
				DrawPcbHighlight(&dc, 0);
				m_arrayPcbHighlight.RemoveAll();
				break;
			}
			
			if((k >= pDoc->m_arrayPcbPad.GetCount())&&( sub_net_num > 1))
			{
				nErrorNum++;
				CString cstring = "网络";
				cstring += pnet->m_csNetName;
				cstring += "断开(子网络";
				char str[100];
				itoa(sub_net_num, str, 10);
				cstring += str;
				cstring += "): ";
				for(int l=0; l<m_arrayPcbHighlight.GetCount(); l++)
				{
					Struct_Pcb_Multi_Selection selected_element = m_arrayPcbHighlight.GetAt(l);
					if(selected_element.pcbelement == PCB_ELEMENT_PAD)
					{
						COb_pcbpad* psubpad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
						if((psubpad->m_nNet == nNet)&&(psubpad->m_nComponent >= 0))
						{
							COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(psubpad->m_nComponent);
							CString csCompID = pcomp->GetPcbCompDesignator(psubpad->m_nComponent);
							
							cstring += csCompID + "-" + psubpad->m_csName;
							cstring += " ";
						}
					}
				}

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}

		PcbProcessingSelection(FALSE);
	}
	
	if(nErrorNum == 0) arDRC.WriteString("\r\n未发现任何错误\r\n\r\n");
	else 
	{
		char str[100];
		itoa(nErrorNum, str, 10);
		CString cstring = "\r\n(";
		cstring += str;
		cstring += "个错误)\r\n\r\n";
		arDRC.WriteString(cstring);
	}

	nErrorNum = 0;
	arDRC.WriteString("※间距检查 -------------------------------------------------------------------\r\n");
	Write_Help("检查间距...");
	double drc = pDoc->m_fRulesClearance;
	CString xstr, ystr;
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack_origin = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack_origin->m_nFlag != FLAG_NORMAL)||(ptrack_origin->m_nPolygon >= 0)||(ptrack_origin->m_nComponent >= 0)) continue;
		char strtemp[100];
		sprintf(strtemp, "连线%d", i);
		Write_Info(strtemp);

		for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
			if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nPolygon >= 0)||(ptrack->m_nComponent >= 0)||(ptrack->m_nNet == ptrack_origin->m_nNet)) continue;

			if( ((ptrack->m_nLayer == MULTILAYER)||(ptrack->m_nLayer == ptrack_origin->m_nLayer)||(ptrack_origin->m_nLayer == MULTILAYER)) &&\
			    (Line_Line_Connect(ptrack_origin, ptrack, drc) == TRUE) )
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[ptrack_origin->m_nLayer].csLayerName + ")连线";
				xstr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fX1 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fY1 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";
				xstr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fX2 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fY2 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[ptrack->m_nLayer].csLayerName + ")连线";
				xstr = pDoc->Convert_XY_To_String(ceil(ptrack->m_fX1 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ptrack->m_fY1 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";
				xstr = pDoc->Convert_XY_To_String(ceil(ptrack->m_fX2 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ptrack->m_fY2 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
			if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon >= 0)||(parc->m_nComponent >= 0)||(parc->m_nNet == ptrack_origin->m_nNet)) continue;

			if( ((parc->m_nLayer == MULTILAYER)||(parc->m_nLayer == ptrack_origin->m_nLayer)||(ptrack_origin->m_nLayer == MULTILAYER)) &&\
			    (Line_Arc_Connect(ptrack_origin, parc, drc) == TRUE) )
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[ptrack_origin->m_nLayer].csLayerName + ")连线";
				xstr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fX1 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fY1 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";
				xstr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fX2 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fY2 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";

				cstring += "(" + pDoc->m_sPCB_LayerParam[parc->m_nLayer].csLayerName + ")圆弧";
				xstr = pDoc->Convert_XY_To_String(ceil(parc->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(parc->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
			if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nNet == ptrack_origin->m_nNet)) continue;

			if( ((ppad->m_nLayer == ptrack_origin->m_nLayer)||(ppad->m_nLayer == MULTILAYER)||(ptrack_origin->m_nLayer == MULTILAYER))&&\
			    (Line_Pad_Connect(ptrack_origin, ppad, drc) == TRUE) )
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[ptrack_origin->m_nLayer].csLayerName + ")连线";
				xstr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fX1 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fY1 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";
				xstr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fX2 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fY2 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";
				
				cstring += "(" + pDoc->m_sPCB_LayerParam[ppad->m_nLayer].csLayerName + ")焊盘";
				xstr = pDoc->Convert_XY_To_String(ceil(ppad->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ppad->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
			if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nNet == ptrack_origin->m_nNet)) continue;

			if((pDoc->IsConnectedToVia(ptrack_origin->m_nLayer, pvia) == TRUE)&&(Line_Via_Connect(ptrack_origin, pvia, drc) == TRUE))
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[ptrack_origin->m_nLayer].csLayerName + ")连线";
				xstr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fX1 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fY1 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";
				xstr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fX2 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fY2 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 过孔";
				xstr = pDoc->Convert_XY_To_String(ceil(pvia->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(pvia->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
			if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)||(pfill->m_nNet == ptrack_origin->m_nNet)) continue;

			if( ((pfill->m_nLayer == MULTILAYER)||(pfill->m_nLayer == ptrack_origin->m_nLayer)||(ptrack_origin->m_nLayer == MULTILAYER))&&\
				(Line_Fill_Connect(ptrack_origin, pfill, drc) == TRUE) )
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[ptrack_origin->m_nLayer].csLayerName + ")连线";
				xstr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fX1 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fY1 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";
				xstr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fX2 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fY2 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";


				cstring += " 与 ";
					
				cstring += "(" + pDoc->m_sPCB_LayerParam[pfill->m_nLayer].csLayerName + ")矩形";			
				xstr = pDoc->Convert_XY_To_String(ceil((pfill->m_fX1 + pfill->m_fX2)/2 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil((pfill->m_fY1 + pfill->m_fY2)/2 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon >= 0)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == ptrack_origin->m_nNet)) continue;

			if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == ptrack_origin->m_nLayer)||(ptrack_origin->m_nLayer == MULTILAYER))&&\
				(Line_Region_Connect(ptrack_origin, pregion, drc) == TRUE) )
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[ptrack_origin->m_nLayer].csLayerName + ")连线";
				xstr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fX1 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fY1 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";
				xstr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fX2 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ptrack_origin->m_fY2 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[pregion->m_nLayer].csLayerName + ")区域";	

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}

		ptrack_origin->m_nFlag = FLAG_PROCESSING;
	}

	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad_origin = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad_origin->m_nFlag != FLAG_NORMAL) continue;
		char strtemp[100];
		sprintf(strtemp, "焊盘%d", i);
		Write_Info(strtemp);

		for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
			if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon >= 0)||(parc->m_nComponent >= 0)||(parc->m_nNet == ppad_origin->m_nNet)) continue;
		
			if( ((ppad_origin->m_nLayer == parc->m_nLayer)||(ppad_origin->m_nLayer == MULTILAYER)||(parc->m_nLayer == MULTILAYER))&&\
				(Pad_Arc_Connect(ppad_origin, parc, drc) == TRUE))
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[ppad_origin->m_nLayer].csLayerName + ")焊盘";
				xstr = pDoc->Convert_XY_To_String(ceil(ppad_origin->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ppad_origin->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";

				cstring += "(" + pDoc->m_sPCB_LayerParam[parc->m_nLayer].csLayerName + ")圆弧";
				xstr = pDoc->Convert_XY_To_String(ceil(parc->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(parc->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
			if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nNet == ppad_origin->m_nNet)) continue;
		
			if( ((ppad->m_nLayer == ppad_origin->m_nLayer)||(ppad->m_nLayer == MULTILAYER)||(ppad_origin->m_nLayer == MULTILAYER))&&\
				( Pad_Pad_Connect(ppad_origin, ppad, drc) == TRUE))
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[ppad_origin->m_nLayer].csLayerName + ")焊盘";
				xstr = pDoc->Convert_XY_To_String(ceil(ppad_origin->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ppad_origin->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";
				
				cstring += "(" + pDoc->m_sPCB_LayerParam[ppad->m_nLayer].csLayerName + ")焊盘";
				xstr = pDoc->Convert_XY_To_String(ceil(ppad->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ppad->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
			if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nNet == ppad_origin->m_nNet)) continue;

			if((pDoc->IsConnectedToVia(ppad_origin->m_nLayer, pvia) == TRUE)&&(Pad_Via_Connect(ppad_origin, pvia, drc) == TRUE))
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[ppad_origin->m_nLayer].csLayerName + ")焊盘";
				xstr = pDoc->Convert_XY_To_String(ceil(ppad_origin->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ppad_origin->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 过孔";
				xstr = pDoc->Convert_XY_To_String(ceil(pvia->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(pvia->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
			if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)||(pfill->m_nNet == ppad_origin->m_nNet)) continue;
		
			if( ((ppad_origin->m_nLayer == pfill->m_nLayer)||(ppad_origin->m_nLayer == MULTILAYER)||(pfill->m_nLayer == MULTILAYER))&&\
				(Pad_Fill_Connect(ppad_origin, pfill, drc) == TRUE) )
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[ppad_origin->m_nLayer].csLayerName + ")焊盘";
				xstr = pDoc->Convert_XY_To_String(ceil(ppad_origin->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ppad_origin->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";
					
				cstring += "(" + pDoc->m_sPCB_LayerParam[pfill->m_nLayer].csLayerName + ")矩形";			
				xstr = pDoc->Convert_XY_To_String(ceil((pfill->m_fX1 + pfill->m_fX2)/2 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil((pfill->m_fY1 + pfill->m_fY2)/2 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon >= 0)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == ppad_origin->m_nNet)) continue;
		
			if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == ppad_origin->m_nLayer)||(ppad_origin->m_nLayer == MULTILAYER))&&\
				(Pad_Region_Connect(ppad_origin, pregion, drc) == TRUE) )
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[ppad_origin->m_nLayer].csLayerName + ")焊盘";
				xstr = pDoc->Convert_XY_To_String(ceil(ppad_origin->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(ppad_origin->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[pregion->m_nLayer].csLayerName + ")区域";	

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}

		ppad_origin->m_nFlag = FLAG_PROCESSING;
	}
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia_origin = pDoc->m_arrayPcbVia.GetAt(i);
		if(pvia_origin->m_nFlag != FLAG_NORMAL) continue;
		char strtemp[100];
		sprintf(strtemp, "过孔%d", i);
		Write_Info(strtemp);

		for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
			if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon >= 0)||(parc->m_nComponent >= 0)||(parc->m_nNet == pvia_origin->m_nNet)) continue;
		
			if((pDoc->IsConnectedToVia(parc->m_nLayer, pvia_origin) == TRUE)&&(Via_Arc_Connect(pvia_origin, parc, drc) == TRUE))
			{
				nErrorNum++;
				CString cstring = "间距错误: 过孔";
				xstr = pDoc->Convert_XY_To_String(ceil(pvia_origin->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(pvia_origin->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";

				cstring += "(" + pDoc->m_sPCB_LayerParam[parc->m_nLayer].csLayerName + ")圆弧";
				xstr = pDoc->Convert_XY_To_String(ceil(parc->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(parc->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
			if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nNet == pvia_origin->m_nNet)) continue;

			if((pDoc->IsConnected_ViaToVia(pvia_origin, pvia) == TRUE)&&(Via_Via_Connect(pvia_origin, pvia, drc) == TRUE))
			{
				nErrorNum++;
				CString cstring = "间距错误: 过孔";
				xstr = pDoc->Convert_XY_To_String(ceil(pvia_origin->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(pvia_origin->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 过孔";
				xstr = pDoc->Convert_XY_To_String(ceil(pvia->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(pvia->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
			if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)||(pfill->m_nNet == pvia_origin->m_nNet)) continue;
		
			if((pDoc->IsConnectedToVia(pfill->m_nLayer, pvia_origin) == TRUE)&&(Via_Fill_Connect(pvia_origin, pfill, drc) == TRUE))
			{
				nErrorNum++;
				CString cstring = "间距错误: 过孔";
				xstr = pDoc->Convert_XY_To_String(ceil(pvia_origin->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(pvia_origin->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";
					
				cstring += "(" + pDoc->m_sPCB_LayerParam[pfill->m_nLayer].csLayerName + ")矩形";			
				xstr = pDoc->Convert_XY_To_String(ceil((pfill->m_fX1 + pfill->m_fX2)/2 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil((pfill->m_fY1 + pfill->m_fY2)/2 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon >= 0)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == pvia_origin->m_nNet)) continue;
		
			if((pDoc->IsConnectedToVia(pregion->m_nLayer, pvia_origin) == TRUE)&&(Via_Region_Connect(pvia_origin, pregion, drc) == TRUE))
			{
				nErrorNum++;
				CString cstring = "间距错误: 过孔";
				xstr = pDoc->Convert_XY_To_String(ceil(pvia_origin->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(pvia_origin->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[pregion->m_nLayer].csLayerName + ")区域";	

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}

		pvia_origin->m_nFlag = FLAG_PROCESSING;
	}
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc_origin = pDoc->m_arrayPcbArc.GetAt(i);
		if(parc_origin->m_nFlag != FLAG_NORMAL) continue;
		char strtemp[100];
		sprintf(strtemp, "圆弧%d", i);
		Write_Info(strtemp);

		for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
			if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon >= 0)||(parc->m_nComponent >= 0)||(parc->m_nNet == parc_origin->m_nNet)) continue;
		
			if( ((parc->m_nLayer == MULTILAYER)||(parc->m_nLayer == parc_origin->m_nLayer)||(parc_origin->m_nLayer == MULTILAYER)) &&\
				(Arc_Arc_Connect(parc_origin, parc, drc) == TRUE) )
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[parc_origin->m_nLayer].csLayerName + ")圆弧";
				xstr = pDoc->Convert_XY_To_String(ceil(parc_origin->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(parc_origin->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";

				cstring += "(" + pDoc->m_sPCB_LayerParam[parc->m_nLayer].csLayerName + ")圆弧";
				xstr = pDoc->Convert_XY_To_String(ceil(parc->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(parc->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
			if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)||(pfill->m_nNet == parc_origin->m_nNet)) continue;
		
			if( ((pfill->m_nLayer == MULTILAYER)||(pfill->m_nLayer == parc_origin->m_nLayer)||(parc_origin->m_nLayer == MULTILAYER))&&\
				(Arc_Fill_Connect(parc_origin, pfill, drc) == TRUE) )
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[parc_origin->m_nLayer].csLayerName + ")圆弧";
				xstr = pDoc->Convert_XY_To_String(ceil(parc_origin->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(parc_origin->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";
					
				cstring += "(" + pDoc->m_sPCB_LayerParam[pfill->m_nLayer].csLayerName + ")矩形";			
				xstr = pDoc->Convert_XY_To_String(ceil((pfill->m_fX1 + pfill->m_fX2)/2 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil((pfill->m_fY1 + pfill->m_fY2)/2 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon >= 0)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == parc_origin->m_nNet)) continue;
		
			if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == parc_origin->m_nLayer)||(parc_origin->m_nLayer == MULTILAYER))&&\
				(Arc_Region_Connect(parc_origin, pregion, drc) == TRUE) )
			{		
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[parc_origin->m_nLayer].csLayerName + ")圆弧";
				xstr = pDoc->Convert_XY_To_String(ceil(parc_origin->m_fCx - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil(parc_origin->m_fCy - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[pregion->m_nLayer].csLayerName + ")区域";	

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}

		parc_origin->m_nFlag = FLAG_PROCESSING;
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill_origin = pDoc->m_arrayPcbFill.GetAt(i);
		if(pfill_origin->m_nFlag != FLAG_NORMAL) continue;
		char strtemp[100];
		sprintf(strtemp, "矩形%d", i);
		Write_Info(strtemp);

		for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
			if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)||(pfill->m_nNet == pfill_origin->m_nNet)) continue;

			if( ((pfill->m_nLayer == MULTILAYER)||(pfill->m_nLayer == pfill_origin->m_nLayer)||(pfill_origin->m_nLayer == MULTILAYER))&&\
				(Fill_Fill_Connect(pfill_origin, pfill, drc) == TRUE) )
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[pfill_origin->m_nLayer].csLayerName + ")矩形";			
				xstr = pDoc->Convert_XY_To_String(ceil((pfill_origin->m_fX1 + pfill_origin->m_fX2)/2 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil((pfill_origin->m_fY1 + pfill_origin->m_fY2)/2 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";
					
				cstring += "(" + pDoc->m_sPCB_LayerParam[pfill->m_nLayer].csLayerName + ")矩形";			
				xstr = pDoc->Convert_XY_To_String(ceil((pfill->m_fX1 + pfill->m_fX2)/2 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil((pfill->m_fY1 + pfill->m_fY2)/2 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon >= 0)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == pfill_origin->m_nNet)) continue;
		
			if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == pfill_origin->m_nLayer)||(pfill_origin->m_nLayer == MULTILAYER))&&\
				(Fill_Region_Connect(pfill_origin, pregion, drc) == TRUE) )
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[pfill_origin->m_nLayer].csLayerName + ")矩形";			
				xstr = pDoc->Convert_XY_To_String(ceil((pfill_origin->m_fX1 + pfill_origin->m_fX2)/2 - pDoc->m_cPCB_Sheet.m_fOriginX));
				ystr = pDoc->Convert_XY_To_String(ceil((pfill_origin->m_fY1 + pfill_origin->m_fY2)/2 - pDoc->m_cPCB_Sheet.m_fOriginY));
				cstring += "(" + xstr + "," + ystr + ")";

				cstring += " 与 ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[pregion->m_nLayer].csLayerName + ")区域";	

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}

		pfill_origin->m_nFlag = FLAG_PROCESSING;
	}
	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion_origin = pDoc->m_arrayPcbRegion.GetAt(i);
		if(pregion_origin->m_nFlag != FLAG_NORMAL) continue;
		char strtemp[100];
		sprintf(strtemp, "区域%d", i);
		Write_Info(strtemp);

		for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon >= 0)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == pregion_origin->m_nNet)) continue;
		
			if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == pregion_origin->m_nLayer)||(pregion_origin->m_nLayer == MULTILAYER))&&\
				(Region_Region_Connect(pregion_origin, pregion, drc) == TRUE) )
			{
				nErrorNum++;
				CString cstring = "间距错误: ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[pregion_origin->m_nLayer].csLayerName + ")区域";

				cstring += " 与 ";
				cstring += "(" + pDoc->m_sPCB_LayerParam[pregion->m_nLayer].csLayerName + ")区域";

				cstring += "\r\n";
				arDRC.WriteString(cstring);
			}
		}

		pregion_origin->m_nFlag = FLAG_PROCESSING;
	}
	PcbProcessingSelection(FALSE);
	if(nErrorNum == 0) arDRC.WriteString("\r\n未发现任何错误\r\n\r\n");
	else 
	{
		char str[100];
		itoa(nErrorNum, str, 10);
		CString cstring = "\r\n(";
		cstring += str;
		cstring += "个错误)\r\n\r\n";
		arDRC.WriteString(cstring);
	}

	arDRC.Flush();
	arDRC.Close();
	cfileDRC.Close();
	OnPcbReportDrc();

	
	m_arrayPcbHighlight.RemoveAll();
}

void CESDView::OnPcbReportDrc()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	char filepath[MAX_PATH];
	CString path = pDoc->GetPathName();
	int index = path.ReverseFind('.');
	if(index > 0) path = path.Left(index);

	strcpy(filepath,path);
	if( strcmp(filepath,"") == 0 )
	{
		AfxMessageBox("未找到元件清单文件.");
		return;
	}

	
	char filestring[MAX_PATH];
	sprintf(filestring,"%s.DRC", filepath);
	CFile cfileDRC;
	if(cfileDRC.Open(filestring,  CFile::modeRead) != TRUE) 
	{
		AfxMessageBox("未找到元件清单文件.");
		return;
	}

	CDlg_Report dlg;
	CArchive arDRC(&cfileDRC, CArchive::load);
	for(;;)
	{
		CString cstring;
		if(arDRC.ReadString(cstring) == FALSE) break;

		dlg.m_arrayReport.Add(cstring);
	}

	dlg.m_TitleText = "DRC出错报告";

	dlg.DoModal();

	arDRC.Close();
	cfileDRC.Close();
}

void CESDView::OnPcbToolClearDrcerror()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	pDoc->m_arrayPcbDrcErrors.RemoveAll();
	Invalidate();
}

BOOL CESDView::ExistInDrcErrors(int pcbelement, int index)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	for(int i=0; i<pDoc->m_arrayPcbDrcErrors.GetCount(); i++)
	{
		Struct_Pcb_Multi_Selection selected_element = pDoc->m_arrayPcbDrcErrors.GetAt(i);
		if((selected_element.pcbelement == pcbelement)&&(selected_element.index == index)) return TRUE;
	}

	return FALSE;
}

void CESDView::FindDrcErrors()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	pDoc->m_arrayPcbDrcErrors.RemoveAll();

	Write_Help("查找间距错误");
	double drc = pDoc->m_fRulesClearance;
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack_origin = pDoc->m_arrayPcbTrack.GetAt(i);
		if((ptrack_origin->m_nFlag != FLAG_NORMAL)||(ptrack_origin->m_nPolygon >= 0)||(ptrack_origin->m_nComponent >= 0)) continue;
		char strtemp[100];
		sprintf(strtemp, "连线%d", i);
		Write_Info(strtemp);

		for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
		{
			COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
			if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nPolygon >= 0)||(ptrack->m_nComponent >= 0)||(ptrack->m_nNet == ptrack_origin->m_nNet)) continue;

			if( ((ptrack->m_nLayer == MULTILAYER)||(ptrack->m_nLayer == ptrack_origin->m_nLayer)||(ptrack_origin->m_nLayer == MULTILAYER)) &&\
			    (Line_Line_Connect(ptrack_origin, ptrack, drc) == TRUE) )
			{
				if(ExistInDrcErrors(PCB_ELEMENT_TRACK, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_TRACK;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_TRACK, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_TRACK;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
			if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon >= 0)||(parc->m_nComponent >= 0)||(parc->m_nNet == ptrack_origin->m_nNet)) continue;

			if( ((parc->m_nLayer == MULTILAYER)||(parc->m_nLayer == ptrack_origin->m_nLayer)||(ptrack_origin->m_nLayer == MULTILAYER)) &&\
			    (Line_Arc_Connect(ptrack_origin, parc, drc) == TRUE) )
			{
				if(ExistInDrcErrors(PCB_ELEMENT_TRACK, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_TRACK;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_ARC, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_ARC;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
			if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nNet == ptrack_origin->m_nNet)) continue;

			if( ((ppad->m_nLayer == ptrack_origin->m_nLayer)||(ppad->m_nLayer == MULTILAYER)||(ptrack_origin->m_nLayer == MULTILAYER))&&\
			    (Line_Pad_Connect(ptrack_origin, ppad, drc) == TRUE) )
			{
				if(ExistInDrcErrors(PCB_ELEMENT_TRACK, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_TRACK;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_PAD, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_PAD;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
			if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nNet == ptrack_origin->m_nNet)) continue;

			if((pDoc->IsConnectedToVia(ptrack_origin->m_nLayer, pvia) == TRUE)&&(Line_Via_Connect(ptrack_origin, pvia, drc) == TRUE))
			{
				if(ExistInDrcErrors(PCB_ELEMENT_TRACK, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_TRACK;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_VIA, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_VIA;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
			if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)||(pfill->m_nNet == ptrack_origin->m_nNet)) continue;

			if( ((pfill->m_nLayer == MULTILAYER)||(pfill->m_nLayer == ptrack_origin->m_nLayer)||(ptrack_origin->m_nLayer == MULTILAYER))&&\
				(Line_Fill_Connect(ptrack_origin, pfill, drc) == TRUE) )
			{
				if(ExistInDrcErrors(PCB_ELEMENT_TRACK, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_TRACK;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_FILL, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_FILL;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon >= 0)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == ptrack_origin->m_nNet)) continue;

			if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == ptrack_origin->m_nLayer)||(ptrack_origin->m_nLayer == MULTILAYER))&&\
				(Line_Region_Connect(ptrack_origin, pregion, drc) == TRUE) )
			{
				if(ExistInDrcErrors(PCB_ELEMENT_TRACK, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_TRACK;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_REGION, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_REGION;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}

		ptrack_origin->m_nFlag = FLAG_PROCESSING;
	}

	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad_origin = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad_origin->m_nFlag != FLAG_NORMAL) continue;
		char strtemp[100];
		sprintf(strtemp, "焊盘%d", i);
		Write_Info(strtemp);

		for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
			if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon >= 0)||(parc->m_nComponent >= 0)||(parc->m_nNet == ppad_origin->m_nNet)) continue;
		
			if( ((ppad_origin->m_nLayer == parc->m_nLayer)||(ppad_origin->m_nLayer == MULTILAYER)||(parc->m_nLayer == MULTILAYER))&&\
				(Pad_Arc_Connect(ppad_origin, parc, drc) == TRUE))
			{
				if(ExistInDrcErrors(PCB_ELEMENT_PAD, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_PAD;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_ARC, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_ARC;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
		{
			COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
			if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nNet == ppad_origin->m_nNet)) continue;
		
			if( ((ppad->m_nLayer == ppad_origin->m_nLayer)||(ppad->m_nLayer == MULTILAYER)||(ppad_origin->m_nLayer == MULTILAYER))&&\
				( Pad_Pad_Connect(ppad_origin, ppad, drc) == TRUE))
			{
				if(ExistInDrcErrors(PCB_ELEMENT_PAD, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_PAD;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_PAD, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_PAD;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
			if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nNet == ppad_origin->m_nNet)) continue;

			if((pDoc->IsConnectedToVia(ppad_origin->m_nLayer, pvia) == TRUE)&&(Pad_Via_Connect(ppad_origin, pvia, drc) == TRUE))
			{
				if(ExistInDrcErrors(PCB_ELEMENT_PAD, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_PAD;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_VIA, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_VIA;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
			if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)||(pfill->m_nNet == ppad_origin->m_nNet)) continue;
		
			if( ((ppad_origin->m_nLayer == pfill->m_nLayer)||(ppad_origin->m_nLayer == MULTILAYER)||(pfill->m_nLayer == MULTILAYER))&&\
				(Pad_Fill_Connect(ppad_origin, pfill, drc) == TRUE) )
			{
				if(ExistInDrcErrors(PCB_ELEMENT_PAD, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_PAD;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_FILL, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_FILL;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon >= 0)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == ppad_origin->m_nNet)) continue;
		
			if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == ppad_origin->m_nLayer)||(ppad_origin->m_nLayer == MULTILAYER))&&\
				(Pad_Region_Connect(ppad_origin, pregion, drc) == TRUE) )
			{
				if(ExistInDrcErrors(PCB_ELEMENT_PAD, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_PAD;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_REGION, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_REGION;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}

		ppad_origin->m_nFlag = FLAG_PROCESSING;
	}
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia_origin = pDoc->m_arrayPcbVia.GetAt(i);
		if(pvia_origin->m_nFlag != FLAG_NORMAL) continue;
		char strtemp[100];
		sprintf(strtemp, "过孔%d", i);
		Write_Info(strtemp);

		for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
			if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon >= 0)||(parc->m_nComponent >= 0)||(parc->m_nNet == pvia_origin->m_nNet)) continue;
		
			if((pDoc->IsConnectedToVia(parc->m_nLayer, pvia_origin) == TRUE)&&(Via_Arc_Connect(pvia_origin, parc, drc) == TRUE))
			{
				if(ExistInDrcErrors(PCB_ELEMENT_VIA, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_VIA;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_ARC, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_ARC;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
		{
			COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
			if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nNet == pvia_origin->m_nNet)) continue;

			if((pDoc->IsConnected_ViaToVia(pvia_origin, pvia) == TRUE)&&(Via_Via_Connect(pvia_origin, pvia, drc) == TRUE))
			{
				if(ExistInDrcErrors(PCB_ELEMENT_VIA, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_VIA;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_VIA, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_VIA;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
			if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)||(pfill->m_nNet == pvia_origin->m_nNet)) continue;
		
			if((pDoc->IsConnectedToVia(pfill->m_nLayer, pvia_origin) == TRUE)&&(Via_Fill_Connect(pvia_origin, pfill, drc) == TRUE))
			{
				if(ExistInDrcErrors(PCB_ELEMENT_VIA, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_VIA;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_FILL, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_FILL;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon >= 0)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == pvia_origin->m_nNet)) continue;
		
			if((pDoc->IsConnectedToVia(pregion->m_nLayer, pvia_origin) == TRUE)&&(Via_Region_Connect(pvia_origin, pregion, drc) == TRUE))
			{
				if(ExistInDrcErrors(PCB_ELEMENT_VIA, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_VIA;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_REGION, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_REGION;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}

		pvia_origin->m_nFlag = FLAG_PROCESSING;
	}
	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc_origin = pDoc->m_arrayPcbArc.GetAt(i);
		if(parc_origin->m_nFlag != FLAG_NORMAL) continue;
		char strtemp[100];
		sprintf(strtemp, "圆弧%d", i);
		Write_Info(strtemp);

		for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
		{
			COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
			if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nPolygon >= 0)||(parc->m_nComponent >= 0)||(parc->m_nNet == parc_origin->m_nNet)) continue;
		
			if( ((parc->m_nLayer == MULTILAYER)||(parc->m_nLayer == parc_origin->m_nLayer)||(parc_origin->m_nLayer == MULTILAYER)) &&\
				(Arc_Arc_Connect(parc_origin, parc, drc) == TRUE) )
			{
				if(ExistInDrcErrors(PCB_ELEMENT_ARC, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_ARC;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_ARC, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_ARC;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
			if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)||(pfill->m_nNet == parc_origin->m_nNet)) continue;
		
			if( ((pfill->m_nLayer == MULTILAYER)||(pfill->m_nLayer == parc_origin->m_nLayer)||(parc_origin->m_nLayer == MULTILAYER))&&\
				(Arc_Fill_Connect(parc_origin, pfill, drc) == TRUE) )
			{
				if(ExistInDrcErrors(PCB_ELEMENT_ARC, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_ARC;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_FILL, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_FILL;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon >= 0)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == parc_origin->m_nNet)) continue;
		
			if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == parc_origin->m_nLayer)||(parc_origin->m_nLayer == MULTILAYER))&&\
				(Arc_Region_Connect(parc_origin, pregion, drc) == TRUE) )
			{		
				if(ExistInDrcErrors(PCB_ELEMENT_ARC, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_ARC;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_REGION, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_REGION;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}

		parc_origin->m_nFlag = FLAG_PROCESSING;
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill_origin = pDoc->m_arrayPcbFill.GetAt(i);
		if(pfill_origin->m_nFlag != FLAG_NORMAL) continue;
		char strtemp[100];
		sprintf(strtemp, "矩形%d", i);
		Write_Info(strtemp);

		for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
		{
			COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
			if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nComponent >= 0)||(pfill->m_nNet == pfill_origin->m_nNet)) continue;

			if( ((pfill->m_nLayer == MULTILAYER)||(pfill->m_nLayer == pfill_origin->m_nLayer)||(pfill_origin->m_nLayer == MULTILAYER))&&\
				(Fill_Fill_Connect(pfill_origin, pfill, drc) == TRUE) )
			{
				if(ExistInDrcErrors(PCB_ELEMENT_FILL, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_FILL;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_FILL, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_FILL;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}
		for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon >= 0)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == pfill_origin->m_nNet)) continue;
		
			if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == pfill_origin->m_nLayer)||(pfill_origin->m_nLayer == MULTILAYER))&&\
				(Fill_Region_Connect(pfill_origin, pregion, drc) == TRUE) )
			{
				if(ExistInDrcErrors(PCB_ELEMENT_FILL, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_FILL;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_REGION, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_REGION;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}

		pfill_origin->m_nFlag = FLAG_PROCESSING;
	}
	for(int i=0; i<pDoc->m_arrayPcbRegion.GetCount(); i++)
	{
		COb_pcbregion* pregion_origin = pDoc->m_arrayPcbRegion.GetAt(i);
		if(pregion_origin->m_nFlag != FLAG_NORMAL) continue;
		char strtemp[100];
		sprintf(strtemp, "区域%d", i);
		Write_Info(strtemp);

		for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
		{
			COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
			if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nPolygon >= 0)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == pregion_origin->m_nNet)) continue;
		
			if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == pregion_origin->m_nLayer)||(pregion_origin->m_nLayer == MULTILAYER))&&\
				(Region_Region_Connect(pregion_origin, pregion, drc) == TRUE) )
			{
				if(ExistInDrcErrors(PCB_ELEMENT_REGION, i) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_REGION;
					selected_element.index = i;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
				if(ExistInDrcErrors(PCB_ELEMENT_REGION, j) == FALSE) 
				{
					Struct_Pcb_Multi_Selection selected_element;
					selected_element.pcbelement = PCB_ELEMENT_REGION;
					selected_element.index = j;
					pDoc->m_arrayPcbDrcErrors.Add(selected_element);
				}
			}
		}

		pregion_origin->m_nFlag = FLAG_PROCESSING;
	}

	PcbProcessingSelection(FALSE);
}


BOOL CESDView::FindTrackOnLineDrcErrors(COb_pcbtrack* ptrack_origin)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	double drc = pDoc->m_fRulesClearance;

	for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nNet == ptrack_origin->m_nNet)) continue;
		if(ptrack->m_nPolygon >= 0)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(ptrack->m_nPolygon);
			if(ppolygon->m_bIgnoreViolations == TRUE) continue;
		}

		if( ((ptrack->m_nLayer == MULTILAYER)||(ptrack->m_nLayer == ptrack_origin->m_nLayer)||(ptrack_origin->m_nLayer == MULTILAYER)) &&\
		    (Line_Line_Connect(ptrack_origin, ptrack, drc) == TRUE) )
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nNet == ptrack_origin->m_nNet)) continue;
		if(parc->m_nPolygon >= 0)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(parc->m_nPolygon);
			if(ppolygon->m_bIgnoreViolations == TRUE) continue;
		}
		if( ((parc->m_nLayer == MULTILAYER)||(parc->m_nLayer == ptrack_origin->m_nLayer)||(ptrack_origin->m_nLayer == MULTILAYER)) &&\
		    (Line_Arc_Connect(ptrack_origin, parc, drc) == TRUE) )
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
		if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nNet == ptrack_origin->m_nNet)) continue;

		if( ((ppad->m_nLayer == ptrack_origin->m_nLayer)||(ppad->m_nLayer == MULTILAYER)||(ptrack_origin->m_nLayer == MULTILAYER))&&\
		    (Line_Pad_Connect(ptrack_origin, ppad, drc) == TRUE) )
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
		if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nNet == ptrack_origin->m_nNet)) continue;

		if((pDoc->IsConnectedToVia(ptrack_origin->m_nLayer, pvia) == TRUE)&&(Line_Via_Connect(ptrack_origin, pvia, drc) == TRUE))
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nNet == ptrack_origin->m_nNet)) continue;

		if( ((pfill->m_nLayer == MULTILAYER)||(pfill->m_nLayer == ptrack_origin->m_nLayer)||(ptrack_origin->m_nLayer == MULTILAYER))&&\
			(Line_Fill_Connect(ptrack_origin, pfill, drc) == TRUE) )
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == ptrack_origin->m_nNet)) continue;
		if(pregion->m_nPolygon >= 0)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(pregion->m_nPolygon);
			if(ppolygon->m_bIgnoreViolations == TRUE) continue;
		}

		if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == ptrack_origin->m_nLayer)||(ptrack_origin->m_nLayer == MULTILAYER))&&\
			(Line_Region_Connect(ptrack_origin, pregion, drc) == TRUE) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CESDView::FindArcOnLineDrcErrors(COb_pcbarc* parc_origin)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	double drc = pDoc->m_fRulesClearance;
	for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nNet == parc_origin->m_nNet)) continue;
		if(ptrack->m_nPolygon >= 0)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(ptrack->m_nPolygon);
			if(ppolygon->m_bIgnoreViolations == TRUE) continue;
		}

		if( ((ptrack->m_nLayer == MULTILAYER)||(ptrack->m_nLayer == parc_origin->m_nLayer)||(parc_origin->m_nLayer == MULTILAYER)) &&\
		    (Line_Arc_Connect(ptrack, parc_origin, drc) == TRUE) )
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
		if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nNet == parc_origin->m_nNet)) continue;

		if( ((ppad->m_nLayer == parc_origin->m_nLayer)||(ppad->m_nLayer == MULTILAYER)||(parc_origin->m_nLayer == MULTILAYER))&&\
		    (Pad_Arc_Connect(ppad, parc_origin, drc) == TRUE) )
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
		if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nNet == parc_origin->m_nNet)) continue;

		if((pDoc->IsConnectedToVia(parc_origin->m_nLayer, pvia) == TRUE)&&(Via_Arc_Connect(pvia, parc_origin, drc) == TRUE))
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nNet == parc_origin->m_nNet)) continue;
		if(parc->m_nPolygon >= 0)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(parc->m_nPolygon);
			if(ppolygon->m_bIgnoreViolations == TRUE) continue;
		}
		if( ((parc->m_nLayer == MULTILAYER)||(parc->m_nLayer == parc_origin->m_nLayer)||(parc_origin->m_nLayer == MULTILAYER)) &&\
		    (Arc_Arc_Connect(parc_origin, parc, drc) == TRUE) )
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nNet == parc_origin->m_nNet)) continue;

		if( ((pfill->m_nLayer == MULTILAYER)||(pfill->m_nLayer == parc_origin->m_nLayer)||(parc_origin->m_nLayer == MULTILAYER))&&\
			(Arc_Fill_Connect(parc_origin, pfill, drc) == TRUE) )
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == parc_origin->m_nNet)) continue;
		if(pregion->m_nPolygon >= 0)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(pregion->m_nPolygon);
			if(ppolygon->m_bIgnoreViolations == TRUE) continue;
		}

		if( ((pregion->m_nLayer == MULTILAYER)||(pregion->m_nLayer == parc_origin->m_nLayer)||(parc_origin->m_nLayer == MULTILAYER))&&\
			(Arc_Region_Connect(parc_origin, pregion, drc) == TRUE) )
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CESDView::FindViaOnLineDrcErrors(COb_pcbvia* pvia_origin)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	double drc = pDoc->m_fRulesClearance;

	for(int j=0; j<pDoc->m_arrayPcbTrack.GetCount(); j++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(j);
		if((ptrack->m_nFlag != FLAG_NORMAL)||(ptrack->m_nNet == pvia_origin->m_nNet)) continue;
		if(ptrack->m_nPolygon >= 0)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(ptrack->m_nPolygon);
			if(ppolygon->m_bIgnoreViolations == TRUE) continue;
		}
		if((pDoc->IsConnectedToVia(ptrack->m_nLayer, pvia_origin) == TRUE)&&(Line_Via_Connect(ptrack, pvia_origin, drc) == TRUE))
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbPad.GetCount(); j++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(j);
		if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nNet == pvia_origin->m_nNet)) continue;
			
		if((pDoc->IsConnectedToVia(ppad->m_nLayer, pvia_origin) == TRUE)&&(Pad_Via_Connect(ppad, pvia_origin, drc) == TRUE))
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbArc.GetCount(); j++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(j);
		if((parc->m_nFlag != FLAG_NORMAL)||(parc->m_nNet == pvia_origin->m_nNet)) continue;
		if(parc->m_nPolygon >= 0)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(parc->m_nPolygon);
			if(ppolygon->m_bIgnoreViolations == TRUE) continue;
		}
		if((pDoc->IsConnectedToVia(parc->m_nLayer, pvia_origin) == TRUE)&&(Via_Arc_Connect(pvia_origin, parc, drc) == TRUE))
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbVia.GetCount(); j++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(j);
		if((pvia->m_nFlag != FLAG_NORMAL)||(pvia->m_nNet == pvia_origin->m_nNet)) continue;

		if((pDoc->IsConnected_ViaToVia(pvia_origin, pvia) == TRUE)&&(Via_Via_Connect(pvia_origin, pvia, drc) == TRUE))
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbFill.GetCount(); j++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(j);
		if((pfill->m_nFlag != FLAG_NORMAL)||(pfill->m_nNet == pvia_origin->m_nNet)) continue;

		if((pDoc->IsConnectedToVia(pfill->m_nLayer, pvia_origin) == TRUE)&&(Via_Fill_Connect(pvia_origin, pfill, drc) == TRUE))
		{
			return TRUE;
		}
	}
	for(int j=0; j<pDoc->m_arrayPcbRegion.GetCount(); j++)
	{
		COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(j);
		if((pregion->m_nFlag != FLAG_NORMAL)||(pregion->m_nComponent >= 0)||(pregion->m_nNet == pvia_origin->m_nNet)) continue;
		if(pregion->m_nPolygon >= 0)
		{
			COb_pcbpolygon* ppolygon = pDoc->m_arrayPcbPolygon.GetAt(pregion->m_nPolygon);
			if(ppolygon->m_bIgnoreViolations == TRUE) continue;
		}

        if((pDoc->IsConnectedToVia(pregion->m_nLayer, pvia_origin) == TRUE)&&(Via_Region_Connect(pvia_origin, pregion, drc) == TRUE))
		{
			return TRUE;
		}
	}

	return FALSE;
}