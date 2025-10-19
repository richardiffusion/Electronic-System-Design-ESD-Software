void CESDView::OnAutoAnnotateReset()
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	int nScope = 0; 

	CDlg_AnnotateReset dlg;
	dlg.m_nRadioScope = 0;
	if(dlg.DoModal() != IDOK) return;

	nScope = dlg.m_nRadioScope;

	if((nScope == 0)||(nScope == 1))
	{
		CObject* pobject;
		int num;
		POSITION pos;
		for(int diagram=0; diagram<SCH_SHEET_NUM; diagram++)
		{
			if(nScope == 1) diagram = m_nDiagram;
			num = pDoc->m_listSchObject[diagram].GetCount();
			pos = pDoc->m_listSchObject[diagram].GetHeadPosition();
			for(int i=0; i<num; i++)
			{	
				if(pos == NULL) break;
				POSITION posbak = pos;
				pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
				
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
				{
					COb_schcomp* pcomp;
					pcomp = (COb_schcomp*)pobject;
					if( ((nScope == 1)&&(pcomp->m_nDiagram != diagram)) || (pcomp->m_nFlag == FLAG_DELETED) )
					{
						for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);
						continue;
					}
					
					for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) 
					{
						POSITION posbak = pos;
						pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;
							if((ptext->m_nID == TEXT_COMPDESIGNATOR)&&(ptext->m_bTextLocked != TRUE))
							{
								CString str = ptext->m_csText;
								str.TrimRight("0123456789");
								if(str != ptext->m_csText)
								{
									
									if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
									{
										pDoc->Clear_Discarded_Undo_Buffer();
									}
									clear_undo_buffer_flag = TRUE;
									CSchUndo *pundo = new CSchUndo;
									pundo->m_nOp = UNDO_CHANGE;
									pundo->m_nOb = SCH_ELEMENT_TEXT;
									pundo->m_nDiagram = diagram;
									pundo->m_nPos = posbak;
									pundo->schtext.Copy(ptext);
									pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
									undo_flag = TRUE;
									

									ptext->m_csText.TrimRight("0123456789");
									ptext->m_csText += "?";
								}
							}
						}
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
				{
					COb_schsheetsymbol* psheetsymbol;
					psheetsymbol = (COb_schsheetsymbol*)pobject;
					for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);
				}

			}
		
			if(nScope == 1) break;
		}
	}
	else
	{
		CObject* pobject;
		int num;
		POSITION pos;
		for(int sel=0; sel<pDoc->m_arraySelection[m_nDiagram].GetCount(); sel++)
		{
			pos = pDoc->m_arraySelection[m_nDiagram].GetAt(sel);
			pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp;
				pcomp = (COb_schcomp*)pobject;
				if( pcomp->m_nFlag == FLAG_DELETED) continue;
					
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) 
				{
					pos = pcomp->m_arrayPos.GetAt(j);
					pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						if((ptext->m_nID == TEXT_COMPDESIGNATOR)&&(ptext->m_bTextLocked != TRUE))
						{
							CString str = ptext->m_csText;
							str.TrimRight("0123456789");
							if(str != ptext->m_csText)
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
								pundo->m_nPos = pos;
								pundo->schtext.Copy(ptext);
								pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
								undo_flag = TRUE;
								

								ptext->m_csText.TrimRight("0123456789");
								ptext->m_csText += "?";
							}
						}
					}
				}
			}
		}
	}

	
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}
	
	Invalidate();
}

void CESDView::OnAutoAnnotate()
{
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);
	
	Struct_Annotate acomp, acomp1, acomp2; 

	CArray<Struct_Annotate, Struct_Annotate&> arrayAnnotate;
	CArray<Struct_Annotate, Struct_Annotate&> arrayExist;
	CStringArray arrayRecord;
	CArray<Struct_Annotate, Struct_Annotate&> arrayMultiPart;

	CObject* pobject;
	int num;
	POSITION pos;

	int diagram;
	for(diagram=0; diagram<SCH_SHEET_NUM; diagram++)
	{
		num = pDoc->m_listSchObject[diagram].GetCount();
		pos = pDoc->m_listSchObject[diagram].GetHeadPosition();
		for(int i=0; i<num; i++)
		{	
			if(pos == NULL) break;
			POSITION comppos = pos;
			pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
			
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp;
				pcomp = (COb_schcomp*)pobject;
				if( pcomp->m_nFlag == FLAG_DELETED )
				{
					for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);
					continue;
				}
				
				acomp.diagram = diagram;
				acomp.fx = pcomp->m_fX;
				acomp.fy = pcomp->m_fY;
				acomp.comppos = comppos;
				acomp.partcount = pcomp->m_nPartCount;
				acomp.partnum = pcomp->m_nPart;
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) 
				{
					POSITION posbak = pos;
                    pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						if(ptext->m_nID == TEXT_COMPDESIGNATOR)
						{
							acomp.designator = ptext->m_csText;
							acomp.textpos = posbak;
						}
						else if(ptext->m_nID == TEXT_COMPCOMMENT) acomp.comment = ptext->m_csText;
					}
				}

				if((acomp.textpos == NULL)||(acomp.comppos == NULL)) continue;

				int strl = acomp.designator.GetLength();
				if(acomp.designator.ReverseFind('?') == (strl-1))
				{
					arrayAnnotate.Add(acomp);
				}
				else arrayExist.Add(acomp);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol;
				psheetsymbol = (COb_schsheetsymbol*)pobject;
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);
			}
		}
	}

	CDlg_Annotate dlg;
	dlg.m_nEditStartNum = 1;
	dlg.m_nRadioScope = 0;
	dlg.m_nComboOrder = 0;
	for(int i=0; i<arrayAnnotate.GetCount(); i++)
	{
		acomp = arrayAnnotate.GetAt(i);
		CString str;
		if(acomp.partcount > 1) 
		{
			char s[2] = { 'A', '\0'};
			s[0] += acomp.partnum - 1;
			str = s;
		}
		str = acomp.designator + str;
		dlg.m_arrayStringA.Add(str);
	}
	for(int i=0; i<arrayExist.GetCount(); i++)
	{
		acomp = arrayExist.GetAt(i);
		CString str;
		if(acomp.partcount > 1) 
		{
			char s[2] = { 'A', '\0'};
			s[0] += acomp.partnum - 1;
			str = s;
		}
		str = acomp.designator + str;
		dlg.m_arrayStringE.Add(str);
	}
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_nComboOrder == 0)		
		{
			int judge = 1;
			for(;;)
			{
				if(judge == 1) judge = 0;
				else if(judge == 0) break;
				for(int i=0;i<arrayAnnotate.GetCount()-1;i++)
				{
					acomp1 = arrayAnnotate.GetAt(i);
					acomp2 = arrayAnnotate.GetAt(i+1);

					if((acomp1.diagram > acomp2.diagram)||\
						((acomp1.diagram == acomp2.diagram)&&(acomp1.fx >  acomp2.fx))||\
						((acomp1.diagram == acomp2.diagram)&&(acomp1.fx == acomp2.fx)&&(acomp1.fy < acomp2.fy)))
					{
						judge=1;
						arrayAnnotate.SetAt(i, acomp2);
						arrayAnnotate.SetAt(i+1, acomp1);
					}
				}
			}
		}
		else if(dlg.m_nComboOrder == 1)	
		{
			int judge = 1;
			for(;;)
			{
				if(judge == 1) judge = 0;
				else if(judge == 0) break;
				for(int i=0;i<arrayAnnotate.GetCount()-1;i++)
				{
					acomp1 = arrayAnnotate.GetAt(i);
					acomp2 = arrayAnnotate.GetAt(i+1);

					if((acomp1.diagram > acomp2.diagram)||\
						((acomp1.diagram == acomp2.diagram)&&(acomp1.fx >  acomp2.fx))||\
						((acomp1.diagram == acomp2.diagram)&&(acomp1.fx == acomp2.fx)&&(acomp1.fy > acomp2.fy)))
					{
						judge=1;
						arrayAnnotate.SetAt(i, acomp2);
						arrayAnnotate.SetAt(i+1, acomp1);
					}
				}
			}
		}
		else if(dlg.m_nComboOrder == 2)	
		{
			int judge = 1;
			for(;;)
			{
				if(judge == 1) judge = 0;
				else if(judge == 0) break;
				for(int i=0;i<arrayAnnotate.GetCount()-1;i++)
				{
					acomp1 = arrayAnnotate.GetAt(i);
					acomp2 = arrayAnnotate.GetAt(i+1);

					if((acomp1.diagram > acomp2.diagram)||\
						((acomp1.diagram == acomp2.diagram)&&(acomp1.fy < acomp2.fy))||\
						((acomp1.diagram == acomp2.diagram)&&(acomp1.fy == acomp2.fy)&&(acomp1.fx > acomp2.fx)))
					{
						judge=1;
						arrayAnnotate.SetAt(i, acomp2);
						arrayAnnotate.SetAt(i+1, acomp1);
					}
				}
			}
		}		
		else	
		{
			int judge = 1;
			for(;;)
			{
				if(judge == 1) judge = 0;
				else if(judge == 0) break;
				for(int i=0;i<arrayAnnotate.GetCount()-1;i++)
				{
					acomp1 = arrayAnnotate.GetAt(i);
					acomp2 = arrayAnnotate.GetAt(i+1);

					if((acomp1.diagram > acomp2.diagram)||\
						((acomp1.diagram == acomp2.diagram)&&(acomp1.fy > acomp2.fy))||\
						((acomp1.diagram == acomp2.diagram)&&(acomp1.fy == acomp2.fy)&&(acomp1.fx > acomp2.fx)))
					{
						judge=1;
						arrayAnnotate.SetAt(i, acomp2);
						arrayAnnotate.SetAt(i+1, acomp1);
					}
				}
			}
		}	
			
		
		for(int i=0; i<arrayAnnotate.GetCount(); i++)
		{
			acomp = arrayAnnotate.GetAt(i);
			
			if((dlg.m_nRadioScope == 1)&&(acomp.diagram != m_nDiagram)) continue;
			
			
			acomp.designator.TrimRight("?");

			CString ref, number;
			ref = acomp.designator;
			
			int flag_multi = 0;
			int multi_r;
			if(acomp.partcount > 1)
			{
				for(multi_r=0; multi_r<arrayMultiPart.GetCount(); multi_r++)
				{
					acomp2 = arrayMultiPart.GetAt(multi_r);
					if(acomp.comment == acomp2.comment)
					{
						acomp.partnum = acomp2.partnum;
						acomp.designator = acomp2.designator;
						flag_multi = 1;
						break;
					}
				}
			}

			int flag_old = 0;
			int old_r;
			number.Empty();
			for(old_r = 0; old_r<arrayRecord.GetCount(); old_r++)
			{
				CString str = arrayRecord.GetAt(old_r);
				int j;
				int length = str.GetLength();
				for(j = length-1; j>=0;  j--)
				{
					if(j<0) break;
					char c = str.GetAt(j);
					if(isdigit(c) == 0) break;
				}
				
				CString ref1, number1;
				if(j < 0) ref1.Empty();
				else ref1 = str.Left(j+1);
				number1 = str.Right(length-j-1);
				if(ref == ref1)
				{
					number = number1;
					flag_old = 1;
					break;
				}
			}

			for(;;)
			{
				if(number.IsEmpty() == TRUE)
				{
					char s[1000];
					itoa(dlg.m_nEditStartNum, s, 10);
					number = s;
					if(acomp.partcount > 1) acomp.partnum = 1;
					acomp.designator = ref + number;
				}
				else
				{
					if(acomp.partcount > 1) 
					{
						if((flag_multi == 1)&&(acomp.partnum < acomp.partcount)) 
						{
							acomp.partnum++;
						}
						else
						{
							acomp.partnum = 1;
							char s[1000];
							strcpy(s, number);
							int n = atoi(s);
							itoa(n+1, s, 10);
							number = s;
							acomp.designator = ref + number;
						}
					}
					else
					{
						char s[1000];
						strcpy(s, number);
						int n = atoi(s);
						itoa(n+1, s, 10);
						number = s;
						acomp.designator = ref + number;
					}
				}
				
				
				int k;
				for(k=0; k<arrayExist.GetCount(); k++)
				{
					acomp1 = arrayExist.GetAt(k);
					if((acomp1.designator == acomp.designator)&&(acomp1.partnum == acomp.partnum)) break;
				}

				if(k >= arrayExist.GetCount()) break;
			}

			
			if(acomp.partcount > 1)
			{
				if(flag_multi == 1) arrayMultiPart.SetAt(multi_r, acomp);
				else arrayMultiPart.Add(acomp);
			}
			if(flag_old == 1) arrayRecord.SetAt(old_r, acomp.designator);
			else arrayRecord.Add(acomp.designator);
			COb_schtext* ptext = (COb_schtext*)pDoc->m_listSchObject[acomp.diagram].GetAt(acomp.textpos);
			
			if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
			{
				pDoc->Clear_Discarded_Undo_Buffer();
			}
			clear_undo_buffer_flag = TRUE;
			CSchUndo *pundo = new CSchUndo;
			pundo->m_nOp = UNDO_CHANGE;
			pundo->m_nOb = SCH_ELEMENT_TEXT;
			pundo->m_nDiagram = diagram;
			pundo->m_nPos = acomp.textpos;
			pundo->schtext.Copy(ptext);
			pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
			undo_flag = TRUE;
			
			ptext->m_csText = acomp.designator;

			if(acomp.partcount > 1)
			{
				COb_schcomp* pcomp = (COb_schcomp*)pDoc->m_listSchObject[acomp.diagram].GetAt(acomp.comppos);
				
				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_COMPONENT;
				pundo->m_nDiagram = diagram;
				pundo->m_nPos = acomp.comppos;
				pundo->schcomp.Copy(pcomp);
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				
				pcomp->m_nPart = acomp.partnum;
			}
		}
	}
	
	
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}		

	Invalidate();
}

void CESDView::OnHighlight(UINT nID)
{
	int index = nID - ID_HIGHLIGHT1;


	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	unsigned ch1;
	char szHelpText[] = "移动高亮网络: <左键><回车>选择网络, <右键><ESC>结束命令";
	MoveCursorToCenter();

	Write_Help(szHelpText);
	do
	{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			m_arrayHighlight[index].RemoveAll();
			Struct_Multi_Selection multi_selection = SelectSCHObject(m_fCurrentSnapX, m_fCurrentSnapY, MULTI_SELECTION);
			if( (multi_selection.element == SCH_ELEMENT_PIN)||(multi_selection.element == SCH_ELEMENT_POWER)||\
				(multi_selection.element == SCH_ELEMENT_PORT)||(multi_selection.element == SCH_ELEMENT_SHEETENTRY) )
			{
				Struct_ObjectPosition objpos;
				objpos.pos = multi_selection.pos;
				objpos.diagram = m_nDiagram;
				m_arrayHighlight[index].Add(objpos);
			}
			else if(multi_selection.element == SCH_ELEMENT_LINE)
			{
				COb_schline* pline = (COb_schline*)pDoc->m_listSchObject[m_nDiagram].GetAt(multi_selection.pos);
				if(pline->m_nAttrib == LINE_WIRE)
				{
					Struct_ObjectPosition objpos;
					objpos.pos = multi_selection.pos;
					objpos.diagram = m_nDiagram;
					m_arrayHighlight[index].Add(objpos);
				}
			}
			else if(multi_selection.element == SCH_ELEMENT_TEXT)
			{
				COb_schtext* ptext = (COb_schtext*)pDoc->m_listSchObject[m_nDiagram].GetAt(multi_selection.pos);
				if(ptext->m_nID == TEXT_NETLABEL)
				{
					Struct_ObjectPosition objpos;
					objpos.pos = multi_selection.pos;
					objpos.diagram = m_nDiagram;
					m_arrayHighlight[index].Add(objpos);
				}
			}

			CObject* pobject;
			for(int i=0; i<m_arrayHighlight[index].GetCount(); i++)
			{
				Struct_ObjectPosition objpos;
				objpos = m_arrayHighlight[index].GetAt(i);
				pobject = pDoc->m_listSchObject[objpos.diagram].GetNext(objpos.pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
				{
					COb_schline* pline = (COb_schline*)pobject;
					HighlightWire(pline, index);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
				{
					COb_schjunc* pjunc = (COb_schjunc*)pobject;
					HighlightJunc(pjunc, index);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext = (COb_schtext*)pobject;
					HighlightNetlabel(ptext, index);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
				{
					COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
					HighlightSheetEntry(psheetentry, index);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
				{
					COb_schport* pport = (COb_schport*)pobject;
					HighlightPort(pport, index);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
				{
					COb_schpower* ppower = (COb_schpower*)pobject;
					HighlightPower(ppower, index);
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
				{
					COb_schpin* ppin = (COb_schpin*)pobject;
					HighlightPin(ppin, index);
				}
			}

			Invalidate();

			Write_Help(szHelpText);
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;
}


void CESDView::OnHighlightReset()
{
	for(int i=0; i<6; i++) m_arrayHighlight[i].RemoveAll();
	m_bViewHighlightFlag = FALSE;
	Invalidate();
}

void CESDView::OnUpdateHighlightReset(CCmdUI *pCmdUI)
{
	for(int i=0; i<6; i++)
	{
		if(m_arrayHighlight[i].GetCount() > 0)
		{
			pCmdUI->Enable(TRUE);
			return;
		}
	}
	pCmdUI->Enable(FALSE);
}

void CESDView::OnHighlightList()
{
	int row_num;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CString csColor[6] = { "b6b6ff", "ff9696", "96ff96", "5555ff", "ff0000", "00ff00"};

	CString csListString = "<Border Padding='4' BorderThickness='1' BorderBrush='#767676' Background='#fdfdfd'>\r\n";
	csListString += "<ScrollViewer>";
	csListString += "<StackPanel TextBlock.FontFamily='Times New Roman'>\r\n";
	
	for(int n=0; n<6; n++)
	{
		CStringArray arrayString;
		arrayString.RemoveAll();
		if(m_arrayHighlight[n].GetCount() > 0)
		{
			csListString += "<Border Padding='2, 0, 2, 0' Background='#";
			csListString += csColor[n];
			csListString += "'><TextBlock TextAlignment='Left' FontWeight='Bold'>";
			csListString += "网络:";
			csListString += GetNetName(n);
			csListString += "</TextBlock></Border>\r\n";
			
			csListString += "<Grid><Grid.ColumnDefinitions>";
			
			row_num = 4;
			for(int j=0; j<row_num; j++) csListString += "<ColumnDefinition MinWidth='30'/>";
			csListString += "</Grid.ColumnDefinitions>\r\n";
			
			for(int j=0; j<m_arrayHighlight[n].GetCount(); j++)
			{
				Struct_ObjectPosition objectpos;
				objectpos = m_arrayHighlight[n].GetAt(j);
				CObject* pobject = pDoc->m_listSchObject[objectpos.diagram].GetAt(objectpos.pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin)))
				{
					COb_schpin* ppin = (COb_schpin*)pobject;
					COb_schcomp* pcomp = (COb_schcomp*)pDoc->m_listSchObject[objectpos.diagram].GetAt(ppin->m_posParent);
					CString string = pcomp->GetDesignator() + "-" + ppin->m_csNumber;
					arrayString.Add(string);
				}
			}			
			
			
			int judge = 1;
			for(;;)
			{
				if(judge == 1) judge = 0;
				else if(judge == 0) break;
				for(int i=0;i<arrayString.GetCount()-1;i++)
				{
					CString str1 = arrayString.GetAt(i);
					CString str2 = arrayString.GetAt(i+1);

					if(str1 > str2)
					{
						judge=1;
						arrayString.SetAt(i, str2);
						arrayString.SetAt(i+1, str1);
					}
				}
			}

			int pinnum = arrayString.GetCount();
		
			
			int line_num = pinnum/row_num;
			if((m_arrayHighlight[n].GetCount()%row_num) > 0) line_num++;
			csListString += "<Grid.RowDefinitions>";
			for(int j=0; j<line_num+1; j++) csListString += "<RowDefinition Height='Auto'/>";
			csListString += "</Grid.RowDefinitions>\r\n";
		}

		int pinnum=0;
		for(int i=0; i<arrayString.GetCount(); i++)
		{
			CString string = arrayString.GetAt(i);
			int row = pinnum/row_num;
			int column = pinnum - row*row_num;
			CString csCol, csRow;
			csCol.Format("%d", column);
			csRow.Format("%d", row);

			csListString += "<Border Margin='1' BorderThickness='1' Grid.Column='"; 
			csListString += csCol;
			csListString += "' Grid.Row='";
			csListString += csRow;
			csListString += "'><TextBlock>";
			csListString += string;
			csListString += "</TextBlock></Border>\r\n";
			pinnum++;
		}
		if(m_arrayHighlight[n].GetCount() > 0)
		{
			csListString += "</Grid>\r\n";
			csListString += "<TextBlock></TextBlock>\r\n";
		}
	}
	csListString += "</StackPanel>\r\n";
	csListString += "</ScrollViewer>";
	csListString += "</Border>";

	int size = csListString.GetLength();
	size +=10;
	CDlg_HighlightList dlg;
	dlg.pListBuffer = (char*)malloc(size);

	strcpy(dlg.pListBuffer, csListString);
	dlg.DoModal();

	free(dlg.pListBuffer);
}

void CESDView::OnUpdateHighlightList(CCmdUI *pCmdUI)
{
	int n;
	for(n=0; n<6; n++)
	{
		if(m_arrayHighlight[n].GetCount() > 0) break;
	}

	if(n < 6) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CESDView::OnHighlightView()
{
	if(m_bViewHighlightFlag == FALSE) m_bViewHighlightFlag = TRUE;
	else m_bViewHighlightFlag = FALSE;

	Invalidate();
}

void CESDView::OnUpdateHighlightView(CCmdUI *pCmdUI)
{
	int n;
	for(n=0; n<6; n++)
	{
		if(m_arrayHighlight[n].GetCount() > 0) break;
	}

	if(n < 6) 
	{
		pCmdUI->Enable(TRUE);
		if(m_bViewHighlightFlag == TRUE) pCmdUI->SetCheck(1);
		else pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->Enable(FALSE);
		m_bViewHighlightFlag = FALSE;
	}
}

void CESDView::HighlightWire(COb_schline* phighlightline, int n)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	int num = pDoc->m_listSchObject[phighlightline->m_nDiagram].GetCount();
	POSITION pos;
	pos = pDoc->m_listSchObject[phighlightline->m_nDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{	
		if(pos == NULL) break;
		POSITION posbak = pos;
		pobject = pDoc->m_listSchObject[phighlightline->m_nDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nAttrib != LINE_WIRE)||(pline->m_nDiagram != phighlightline->m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;

			if( ((pline->m_cVertex[0].fx == phighlightline->m_cVertex[0].fx)&&(pline->m_cVertex[0].fy == phighlightline->m_cVertex[0].fy)) ||\
				((pline->m_cVertex[0].fx == phighlightline->m_cVertex[phighlightline->m_cVertex.GetCount()-1].fx)&&(pline->m_cVertex[0].fy == phighlightline->m_cVertex[phighlightline->m_cVertex.GetCount()-1].fy)) ||\
				((pline->m_cVertex[pline->m_cVertex.GetCount()-1].fx == phighlightline->m_cVertex[phighlightline->m_cVertex.GetCount()-1].fx)&&(pline->m_cVertex[pline->m_cVertex.GetCount()-1].fy == phighlightline->m_cVertex[phighlightline->m_cVertex.GetCount()-1].fy)) ||\
				((pline->m_cVertex[pline->m_cVertex.GetCount()-1].fx == phighlightline->m_cVertex[0].fx)&&(pline->m_cVertex[pline->m_cVertex.GetCount()-1].fy == phighlightline->m_cVertex[0].fy)) )
			{
				if(HighlightExist(phighlightline->m_nDiagram, posbak, n) == FALSE)
				{
					Struct_ObjectPosition objpos;
					objpos.pos = posbak;
					objpos.diagram = phighlightline->m_nDiagram;
					m_arrayHighlight[n].Add(objpos);
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
		{
			COb_schjunc* pjunc = (COb_schjunc*)pobject;
			if((pjunc->m_nDiagram != phighlightline->m_nDiagram)||(pjunc->m_nFlag != FLAG_NORMAL))	continue;

			for(int k=0; k<phighlightline->m_cVertex.GetCount(); k++)
			{
				if((pjunc->m_fCx == phighlightline->m_cVertex[k].fx)&&(pjunc->m_fCy == phighlightline->m_cVertex[k].fy)) 
				{
					if(HighlightExist(phighlightline->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = phighlightline->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
				if(k != phighlightline->m_cVertex.GetCount()-1)
				{
					if( ((phighlightline->m_cVertex[k].fx == phighlightline->m_cVertex[k+1].fx)&&(pjunc->m_fCx == phighlightline->m_cVertex[k].fx)&&\
						  (pjunc->m_fCy > MIN(phighlightline->m_cVertex[k].fy, phighlightline->m_cVertex[k+1].fy))&&\
						  (pjunc->m_fCy < MAX(phighlightline->m_cVertex[k].fy, phighlightline->m_cVertex[k+1].fy)) ) ||\
						 ((phighlightline->m_cVertex[k].fy == phighlightline->m_cVertex[k+1].fy)&&(pjunc->m_fCy == phighlightline->m_cVertex[k].fy)&&\
						  (pjunc->m_fCx > MIN(phighlightline->m_cVertex[k].fx, phighlightline->m_cVertex[k+1].fx))&&\
						  (pjunc->m_fCx < MAX(phighlightline->m_cVertex[k].fx, phighlightline->m_cVertex[k+1].fx)) ) )
					{
						if(HighlightExist(phighlightline->m_nDiagram, posbak, n) == FALSE)
						{
							Struct_ObjectPosition objpos;
							objpos.pos = posbak;
							objpos.diagram = phighlightline->m_nDiagram;
							m_arrayHighlight[n].Add(objpos);
						}
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext = (COb_schtext*)pobject;
			if((ptext->m_nID != TEXT_NETLABEL) || (ptext->m_nDiagram != phighlightline->m_nDiagram) || (ptext->m_nFlag != FLAG_NORMAL))	continue;
			
			for(int k=0; k<phighlightline->m_cVertex.GetCount(); k++)
			{
				if((ptext->m_fSx == phighlightline->m_cVertex[k].fx)&&(ptext->m_fSy == phighlightline->m_cVertex[k].fy)) 
				{
					if(HighlightExist(phighlightline->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = phighlightline->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
				if(k != phighlightline->m_cVertex.GetCount()-1)
				{
					if( ((phighlightline->m_cVertex[k].fx == phighlightline->m_cVertex[k+1].fx)&&(ptext->m_fSx == phighlightline->m_cVertex[k].fx)&&\
						 (ptext->m_fSy > MIN(phighlightline->m_cVertex[k].fy, phighlightline->m_cVertex[k+1].fy))&&\
						 (ptext->m_fSy < MAX(phighlightline->m_cVertex[k].fy, phighlightline->m_cVertex[k+1].fy)) ) ||\
						((phighlightline->m_cVertex[k].fy == phighlightline->m_cVertex[k+1].fy)&&(ptext->m_fSy == phighlightline->m_cVertex[k].fy)&&\
						 (ptext->m_fSx > MIN(phighlightline->m_cVertex[k].fx, phighlightline->m_cVertex[k+1].fx))&&\
						 (ptext->m_fSx < MAX(phighlightline->m_cVertex[k].fx, phighlightline->m_cVertex[k+1].fx)) ) )
					{
						if(HighlightExist(phighlightline->m_nDiagram, posbak, n) == FALSE)
						{
							Struct_ObjectPosition objpos;
							objpos.pos = posbak;
							objpos.diagram = phighlightline->m_nDiagram;
							m_arrayHighlight[n].Add(objpos);
						}
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol;
			psheetsymbol = (COb_schsheetsymbol*)pobject;
			if((psheetsymbol->m_nDiagram != phighlightline->m_nDiagram)||(psheetsymbol->m_nFlag != FLAG_NORMAL))
			{
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[phighlightline->m_nDiagram].GetNext(pos);

				continue;
			}

			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
			{
				POSITION subpos = pos;
				pobject = pDoc->m_listSchObject[phighlightline->m_nDiagram].GetNext(pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
				{
					COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
					if(psheetentry->m_nFlag != FLAG_NORMAL)	continue;
					double fx, fy;
					if(psheetentry->m_nSide == ENTRY_SIDE_LEFT)
					{
						fx = psheetsymbol->m_fX;
						fy = psheetsymbol->m_fY - psheetentry->m_fOffset;
					}
					else if(psheetentry->m_nSide == ENTRY_SIDE_TOP)
					{
						fx = psheetsymbol->m_fX + psheetentry->m_fOffset;
						fy = psheetsymbol->m_fY;
					}
					else if(psheetentry->m_nSide == ENTRY_SIDE_RIGHT)
					{
						fx = psheetsymbol->m_fX + psheetsymbol->m_fXsize;
						fy = psheetsymbol->m_fY - psheetentry->m_fOffset;
					}
					else if(psheetentry->m_nSide == ENTRY_SIDE_BOTTOM)
					{
						fx = psheetsymbol->m_fX + psheetentry->m_fOffset;
						fy = psheetsymbol->m_fY - psheetsymbol->m_fYsize;
					}

					for(int k=0; k<phighlightline->m_cVertex.GetCount(); k++)
					{
						if((fx == phighlightline->m_cVertex[k].fx)&&(fy == phighlightline->m_cVertex[k].fy)) 
						{
							if(HighlightExist(phighlightline->m_nDiagram, subpos, n) == FALSE)
							{
								Struct_ObjectPosition objpos;
								objpos.pos = subpos;
								objpos.diagram = phighlightline->m_nDiagram;
								m_arrayHighlight[n].Add(objpos);
							}
						}
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != phighlightline->m_nDiagram)||(pport->m_nFlag != FLAG_NORMAL))	continue;
			double fx,fy;
			if(pport->m_nStyle < PORT_STYLE_NV) 
			{
				fx = pport->m_fX + pport->m_fSize;
				fy = pport->m_fY;
			}
			else
			{
				fx = pport->m_fX;
				fy = pport->m_fY + pport->m_fSize;
			}

			for(int k=0; k<phighlightline->m_cVertex.GetCount(); k++)
			{
				if( ((pport->m_fX == phighlightline->m_cVertex[k].fx)&&(pport->m_fY == phighlightline->m_cVertex[k].fy)) ||\
					((fx == phighlightline->m_cVertex[k].fx)&&(fy == phighlightline->m_cVertex[k].fy)) )
				{
					if(HighlightExist(phighlightline->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = phighlightline->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != phighlightline->m_nDiagram)||(ppower->m_nFlag != FLAG_NORMAL))	continue;
			for(int k=0; k<phighlightline->m_cVertex.GetCount(); k++)
			{
				if((ppower->m_fX == phighlightline->m_cVertex[k].fx)&&(ppower->m_fY == phighlightline->m_cVertex[k].fy)) 
				{
					if(HighlightExist(phighlightline->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = phighlightline->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp;
			pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram == phighlightline->m_nDiagram)&&(pcomp->m_nFlag == FLAG_NORMAL))	
			{
				for(int j=0; j<pcomp->m_arrayPos.GetCount(); j++)
				{
					POSITION subpos = pcomp->m_arrayPos.GetAt(j);
					pobject = pDoc->m_listSchObject[phighlightline->m_nDiagram].GetAt(subpos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
					{
						COb_schpin* ppin = (COb_schpin*)pobject;
						if(ppin->m_nFlag != FLAG_NORMAL) continue;

						
						if(( pcomp->m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  pcomp->m_nPart))
						{
							
							if(( pcomp->m_nDisplayModeCount <= 1)||( pcomp->m_nDisplayMode == ppin->m_nDisplayMode))
							{
								double fx, fy;
								if(ppin->m_nOrientation == 0)
								{
									fx = ppin->m_fX + ppin->m_fPinLength;
									fy = ppin->m_fY;
								}
								else if(ppin->m_nOrientation == 90)
								{
									fx = ppin->m_fX;
									fy = ppin->m_fY + ppin->m_fPinLength;
								}
								else if(ppin->m_nOrientation == 180)
								{
									fx = ppin->m_fX - ppin->m_fPinLength;
									fy = ppin->m_fY;
								}
								else if(ppin->m_nOrientation == 270)
								{
									fx = ppin->m_fX;
									fy = ppin->m_fY - ppin->m_fPinLength;
								}

								for(int k=0; k<phighlightline->m_cVertex.GetCount(); k++)
								{
									if((fx == phighlightline->m_cVertex[k].fx)&&(fy == phighlightline->m_cVertex[k].fy)) 
									{
										if(HighlightExist(phighlightline->m_nDiagram, subpos, n) == FALSE)
										{
											Struct_ObjectPosition objpos;
											objpos.pos = subpos;
											objpos.diagram = phighlightline->m_nDiagram;
											m_arrayHighlight[n].Add(objpos);
										}
									}
								}
							}
						}
					}
				}
			}

			
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[phighlightline->m_nDiagram].GetNext(pos);
		}
	}	
}

void CESDView::HighlightJunc(COb_schjunc* phighlightjunc, int n)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	int num = pDoc->m_listSchObject[phighlightjunc->m_nDiagram].GetCount();
	POSITION pos;
	pos = pDoc->m_listSchObject[phighlightjunc->m_nDiagram].GetHeadPosition();
	for(int i=0; i<num; i++)
	{	
		if(pos == NULL) break;
		POSITION posbak = pos;
		pobject = pDoc->m_listSchObject[phighlightjunc->m_nDiagram].GetNext(pos);
		
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
		{
			COb_schline* pline = (COb_schline*)pobject;
			if((pline->m_nAttrib != LINE_WIRE)||(pline->m_nDiagram != phighlightjunc->m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;

			for(int k=0; k<pline->m_cVertex.GetCount(); k++)
			{
				if((phighlightjunc->m_fCx == pline->m_cVertex[k].fx)&&(phighlightjunc->m_fCy == pline->m_cVertex[k].fy)) 
				{
					if(HighlightExist(pline->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = pline->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
				if(k != pline->m_cVertex.GetCount()-1)
				{
					if( ((pline->m_cVertex[k].fx == pline->m_cVertex[k+1].fx)&&(phighlightjunc->m_fCx == pline->m_cVertex[k].fx)&&\
						  (phighlightjunc->m_fCy > MIN(pline->m_cVertex[k].fy, pline->m_cVertex[k+1].fy))&&\
						  (phighlightjunc->m_fCy < MAX(pline->m_cVertex[k].fy, pline->m_cVertex[k+1].fy)) ) ||\
						 ((pline->m_cVertex[k].fy == pline->m_cVertex[k+1].fy)&&(phighlightjunc->m_fCy == pline->m_cVertex[k].fy)&&\
						  (phighlightjunc->m_fCx > MIN(pline->m_cVertex[k].fx, pline->m_cVertex[k+1].fx))&&\
						  (phighlightjunc->m_fCx < MAX(pline->m_cVertex[k].fx, pline->m_cVertex[k+1].fx)) ) )
					{
						if(HighlightExist(pline->m_nDiagram, posbak, n) == FALSE)
						{
							Struct_ObjectPosition objpos;
							objpos.pos = posbak;
							objpos.diagram = pline->m_nDiagram;
							m_arrayHighlight[n].Add(objpos);
						}
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
		{
			COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
			for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[phighlightjunc->m_nDiagram].GetNext(pos);

		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower = (COb_schpower*)pobject;
			if((ppower->m_nDiagram != phighlightjunc->m_nDiagram)||(ppower->m_nFlag != FLAG_NORMAL))	continue;
			
			if((ppower->m_fX == phighlightjunc->m_fCx)&&(ppower->m_fY == phighlightjunc->m_fCy)) 
			{
				if(HighlightExist(phighlightjunc->m_nDiagram, posbak, n) == FALSE)
				{
					Struct_ObjectPosition objpos;
					objpos.pos = posbak;
					objpos.diagram = phighlightjunc->m_nDiagram;
					m_arrayHighlight[n].Add(objpos);
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
		{
			COb_schcomp* pcomp = (COb_schcomp*)pobject;
			if((pcomp->m_nDiagram == phighlightjunc->m_nDiagram)&&(pcomp->m_nFlag == FLAG_NORMAL))	
			{
				for(int j=0; j<pcomp->m_arrayPos.GetCount(); j++)
				{
					POSITION subpos = pcomp->m_arrayPos.GetAt(j);
					pobject = pDoc->m_listSchObject[phighlightjunc->m_nDiagram].GetAt(subpos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
					{
						COb_schpin* ppin = (COb_schpin*)pobject;
						if(ppin->m_nFlag != FLAG_NORMAL) continue;

						
						if(( pcomp->m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  pcomp->m_nPart))
						{
							
							if(( pcomp->m_nDisplayModeCount <= 1)||( pcomp->m_nDisplayMode == ppin->m_nDisplayMode))
							{
								double fx, fy;
								if(ppin->m_nOrientation == 0)
								{
									fx = ppin->m_fX + ppin->m_fPinLength;
									fy = ppin->m_fY;
								}
								else if(ppin->m_nOrientation == 90)
								{
									fx = ppin->m_fX;
									fy = ppin->m_fY + ppin->m_fPinLength;
								}
								else if(ppin->m_nOrientation == 180)
								{
									fx = ppin->m_fX - ppin->m_fPinLength;
									fy = ppin->m_fY;
								}
								else if(ppin->m_nOrientation == 270)
								{
									fx = ppin->m_fX;
									fy = ppin->m_fY - ppin->m_fPinLength;
								}

								if((fx == phighlightjunc->m_fCx)&&(fy == phighlightjunc->m_fCy)) 
								{
									if(HighlightExist(phighlightjunc->m_nDiagram, subpos, n) == FALSE)
									{
										Struct_ObjectPosition objpos;
										objpos.pos = subpos;
										objpos.diagram = phighlightjunc->m_nDiagram;
										m_arrayHighlight[n].Add(objpos);
									}
								}
							}
						}
					}
				}
			}

			
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[phighlightjunc->m_nDiagram].GetNext(pos);
		}
	}	
}

void CESDView::HighlightNetlabel(COb_schtext* phighlighttext, int n)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int diagram=0; diagram < SCH_SHEET_NUM; diagram++)
	{
		int num = pDoc->m_listSchObject[diagram].GetCount();
		POSITION pos;
		pos = pDoc->m_listSchObject[diagram].GetHeadPosition();
		for(int i=0; i<num; i++)
		{	
			if(pos == NULL) break;
			POSITION posbak = pos;
			pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
			
			if((diagram == phighlighttext->m_nDiagram)&&(pobject->IsKindOf( RUNTIME_CLASS( COb_schline ))))
			{
				COb_schline* pline = (COb_schline*)pobject;
				if((pline->m_nAttrib != LINE_WIRE)||(pline->m_nDiagram != phighlighttext->m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;

				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					if((phighlighttext->m_fSx == pline->m_cVertex[k].fx)&&(phighlighttext->m_fSy == pline->m_cVertex[k].fy)) 
					{
						if(HighlightExist(pline->m_nDiagram, posbak, n) == FALSE)
						{
							Struct_ObjectPosition objpos;
							objpos.pos = posbak;
							objpos.diagram = pline->m_nDiagram;
							m_arrayHighlight[n].Add(objpos);
						}
					}
					if(k != pline->m_cVertex.GetCount()-1)
					{
						if( ((pline->m_cVertex[k].fx == pline->m_cVertex[k+1].fx)&&(phighlighttext->m_fSx == pline->m_cVertex[k].fx)&&\
							(phighlighttext->m_fSy > MIN(pline->m_cVertex[k].fy, pline->m_cVertex[k+1].fy))&&\
							(phighlighttext->m_fSy < MAX(pline->m_cVertex[k].fy, pline->m_cVertex[k+1].fy)) ) ||\
							((pline->m_cVertex[k].fy == pline->m_cVertex[k+1].fy)&&(phighlighttext->m_fSy == pline->m_cVertex[k].fy)&&\
							(phighlighttext->m_fSx > MIN(pline->m_cVertex[k].fx, pline->m_cVertex[k+1].fx))&&\
							(phighlighttext->m_fSx < MAX(pline->m_cVertex[k].fx, pline->m_cVertex[k+1].fx)) ) )
						{
							if(HighlightExist(pline->m_nDiagram, posbak, n) == FALSE)
							{
								Struct_ObjectPosition objpos;
								objpos.pos = posbak;
								objpos.diagram = pline->m_nDiagram;
								m_arrayHighlight[n].Add(objpos);
							}
						}
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				if((ptext->m_nID != TEXT_NETLABEL) || (ptext->m_nFlag != FLAG_NORMAL))	continue;

				if(ptext->m_csText == phighlighttext->m_csText)
				{
					if(HighlightExist(ptext->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = ptext->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol;
				psheetsymbol = (COb_schsheetsymbol*)pobject;
				if(psheetsymbol->m_nFlag != FLAG_NORMAL)
				{
					for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);

					continue;
				}

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
				{
					POSITION subpos = pos;
					pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
					{
						COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
						if(psheetentry->m_nFlag != FLAG_NORMAL)	continue;

						if(psheetentry->m_csName == phighlighttext->m_csText)
						{
							if(HighlightExist(psheetsymbol->m_nDiagram, subpos, n) == FALSE)
							{
								Struct_ObjectPosition objpos;
								objpos.pos = subpos;
								objpos.diagram = psheetsymbol->m_nDiagram;
								m_arrayHighlight[n].Add(objpos);
							}
						}
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport = (COb_schport*)pobject;
				if(pport->m_nFlag != FLAG_NORMAL)	continue;
				
				if(pport->m_csName == phighlighttext->m_csText)
				{
					if(HighlightExist(pport->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = pport->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				if(ppower->m_nFlag != FLAG_NORMAL)	continue;
				
				if(ppower->m_csNetName == phighlighttext->m_csText)
				{
					if(HighlightExist(ppower->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = ppower->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;
				if(pcomp->m_nFlag == FLAG_NORMAL)
				{
					for(int j=0; j<pcomp->m_arrayPos.GetCount(); j++)
					{
						POSITION subpos = pcomp->m_arrayPos.GetAt(j);
						pobject = pDoc->m_listSchObject[diagram].GetAt(subpos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
						{
							COb_schpin* ppin = (COb_schpin*)pobject;
							if(ppin->m_nFlag != FLAG_NORMAL) continue;

							
							if(( pcomp->m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  pcomp->m_nPart))
							{
								
								if(( pcomp->m_nDisplayModeCount <= 1)||( pcomp->m_nDisplayMode == ppin->m_nDisplayMode))
								{
									if((ppin->m_bHide == TRUE)&&((ppin->m_csConnectTo.IsEmpty()?ppin->m_csName:ppin->m_csConnectTo) == phighlighttext->m_csText))
									{
										if(HighlightExist(diagram, subpos, n) == FALSE)
										{
											Struct_ObjectPosition objpos;
											objpos.pos = subpos;
											objpos.diagram = diagram;
											m_arrayHighlight[n].Add(objpos);
										}
									}
								}
							}
						}
					}
				}

				
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);
			}

		}	
	}
}

void CESDView::HighlightPower(COb_schpower* phighlightpower, int n)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int diagram=0; diagram < SCH_SHEET_NUM; diagram++)
	{
		int num = pDoc->m_listSchObject[diagram].GetCount();
		POSITION pos;
		pos = pDoc->m_listSchObject[diagram].GetHeadPosition();
		for(int i=0; i<num; i++)
		{	
			if(pos == NULL) break;
			POSITION posbak = pos;
			pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
			
			if((diagram == phighlightpower->m_nDiagram)&&(pobject->IsKindOf( RUNTIME_CLASS( COb_schline ))))
			{
				COb_schline* pline = (COb_schline*)pobject;
				if((pline->m_nAttrib != LINE_WIRE)||(pline->m_nDiagram != phighlightpower->m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;

				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					if((phighlightpower->m_fX == pline->m_cVertex[k].fx)&&(phighlightpower->m_fY == pline->m_cVertex[k].fy)) 
					{
						if(HighlightExist(pline->m_nDiagram, posbak, n) == FALSE)
						{
							Struct_ObjectPosition objpos;
							objpos.pos = posbak;
							objpos.diagram = pline->m_nDiagram;
							m_arrayHighlight[n].Add(objpos);
						}
					}
				}
			}
			if((diagram == phighlightpower->m_nDiagram)&&(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc ))))
			{
				COb_schjunc* pjunc = (COb_schjunc*)pobject;
				if((pjunc->m_nDiagram != phighlightpower->m_nDiagram)||(pjunc->m_nFlag != FLAG_NORMAL))	continue;

				if((phighlightpower->m_fX == pjunc->m_fCx)&&(phighlightpower->m_fY == pjunc->m_fCy)) 
				{
					if(HighlightExist(pjunc->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = pjunc->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				if((ptext->m_nID != TEXT_NETLABEL) || (ptext->m_nFlag != FLAG_NORMAL))	continue;

				if(ptext->m_csText == phighlightpower->m_csNetName)
				{
					if(HighlightExist(ptext->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = ptext->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol;
				psheetsymbol = (COb_schsheetsymbol*)pobject;
				if(psheetsymbol->m_nFlag != FLAG_NORMAL)
				{
					for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);

					continue;
				}

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
				{
					POSITION subpos = pos;
					pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
					{
						COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
						if(psheetentry->m_nFlag != FLAG_NORMAL)	continue;

						if(psheetentry->m_csName == phighlightpower->m_csNetName)
						{
							if(HighlightExist(psheetsymbol->m_nDiagram, subpos, n) == FALSE)
							{
								Struct_ObjectPosition objpos;
								objpos.pos = subpos;
								objpos.diagram = psheetsymbol->m_nDiagram;
								m_arrayHighlight[n].Add(objpos);
							}
						}
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport = (COb_schport*)pobject;
				if(pport->m_nFlag != FLAG_NORMAL)	continue;
				
				if(pport->m_csName == phighlightpower->m_csNetName)
				{
					if(HighlightExist(pport->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = pport->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				if(ppower->m_nFlag != FLAG_NORMAL)	continue;
				
				if(ppower->m_csNetName == phighlightpower->m_csNetName)
				{
					if(HighlightExist(ppower->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = ppower->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;
				if(pcomp->m_nFlag == FLAG_NORMAL)
				{
					for(int j=0; j<pcomp->m_arrayPos.GetCount(); j++)
					{
						POSITION subpos = pcomp->m_arrayPos.GetAt(j);
						pobject = pDoc->m_listSchObject[diagram].GetAt(subpos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
						{
							COb_schpin* ppin = (COb_schpin*)pobject;
							if(ppin->m_nFlag != FLAG_NORMAL) continue;

							
							if(( pcomp->m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  pcomp->m_nPart))
							{
								
								if(( pcomp->m_nDisplayModeCount <= 1)||( pcomp->m_nDisplayMode == ppin->m_nDisplayMode))
								{
									if(ppin->m_bHide == TRUE)
									{
										if((ppin->m_csConnectTo.IsEmpty()?ppin->m_csName:ppin->m_csConnectTo) == phighlightpower->m_csNetName)
										{
											if(HighlightExist(diagram, subpos, n) == FALSE)
											{
												Struct_ObjectPosition objpos;
												objpos.pos = subpos;
												objpos.diagram = diagram;
												m_arrayHighlight[n].Add(objpos);
											}
										}
									}
									else
									{
										double fx, fy;
										if(ppin->m_nOrientation == 0)
										{
											fx = ppin->m_fX + ppin->m_fPinLength;
											fy = ppin->m_fY;
										}
										else if(ppin->m_nOrientation == 90)
										{
											fx = ppin->m_fX;
											fy = ppin->m_fY + ppin->m_fPinLength;
										}
										else if(ppin->m_nOrientation == 180)
										{
											fx = ppin->m_fX - ppin->m_fPinLength;
											fy = ppin->m_fY;
										}
										else if(ppin->m_nOrientation == 270)
										{
											fx = ppin->m_fX;
											fy = ppin->m_fY - ppin->m_fPinLength;
										}
										if( (pcomp->m_nDiagram == phighlightpower->m_nDiagram)&&(ppin->m_bHide != TRUE)&&\
											(phighlightpower->m_fX == fx)&&(phighlightpower->m_fY == fy))
										{
											if(HighlightExist(diagram, subpos, n) == FALSE)
											{
												Struct_ObjectPosition objpos;
												objpos.pos = subpos;
												objpos.diagram = diagram;
												m_arrayHighlight[n].Add(objpos);
											}
										}
									}
								}
							}
						}
					}
				}

				
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);
			}

		}	
	}
}

void CESDView::HighlightPort(COb_schport* phighlightport,  int n)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
			
	double fx,fy;
	if(phighlightport->m_nStyle < PORT_STYLE_NV) 
	{
		fx = phighlightport->m_fX + phighlightport->m_fSize;
		fy = phighlightport->m_fY;
	}
	else
	{
		fx = phighlightport->m_fX;
		fy = phighlightport->m_fY + phighlightport->m_fSize;
	}

	for(int diagram=0; diagram < SCH_SHEET_NUM; diagram++)
	{
		int num = pDoc->m_listSchObject[diagram].GetCount();
		POSITION pos;
		pos = pDoc->m_listSchObject[diagram].GetHeadPosition();
		for(int i=0; i<num; i++)
		{	
			if(pos == NULL) break;
			POSITION posbak = pos;
			pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
			
			if((diagram == phighlightport->m_nDiagram)&&(pobject->IsKindOf( RUNTIME_CLASS( COb_schline ))))
			{
				COb_schline* pline = (COb_schline*)pobject;
				if((pline->m_nAttrib != LINE_WIRE)||(pline->m_nDiagram != phighlightport->m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;

				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					if( ((phighlightport->m_fX == pline->m_cVertex[k].fx)&&(phighlightport->m_fY == pline->m_cVertex[k].fy)) ||\
						((fx == pline->m_cVertex[k].fx)&&(fy == pline->m_cVertex[k].fy)) )
					{
						if(HighlightExist(pline->m_nDiagram, posbak, n) == FALSE)
						{
							Struct_ObjectPosition objpos;
							objpos.pos = posbak;
							objpos.diagram = pline->m_nDiagram;
							m_arrayHighlight[n].Add(objpos);
						}
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				if((ptext->m_nID != TEXT_NETLABEL) || (ptext->m_nFlag != FLAG_NORMAL))	continue;

				if(ptext->m_csText == phighlightport->m_csName)
				{
					if(HighlightExist(ptext->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = ptext->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol;
				psheetsymbol = (COb_schsheetsymbol*)pobject;
				if(psheetsymbol->m_nFlag != FLAG_NORMAL)
				{
					for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);

					continue;
				}

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
				{
					POSITION subpos = pos;
					pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
					{
						COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
						if(psheetentry->m_nFlag != FLAG_NORMAL)	continue;

						if(psheetentry->m_csName == phighlightport->m_csName)
						{
							if(HighlightExist(psheetsymbol->m_nDiagram, subpos, n) == FALSE)
							{
								Struct_ObjectPosition objpos;
								objpos.pos = subpos;
								objpos.diagram = psheetsymbol->m_nDiagram;
								m_arrayHighlight[n].Add(objpos);
							}
						}
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport = (COb_schport*)pobject;
				if(pport->m_nFlag != FLAG_NORMAL)	continue;
				
				if(pport->m_csName == phighlightport->m_csName)
				{
					if(HighlightExist(pport->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = pport->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				if(ppower->m_nFlag != FLAG_NORMAL)	continue;
				
				if(ppower->m_csNetName == phighlightport->m_csName)
				{
					if(HighlightExist(ppower->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = ppower->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;
				if(pcomp->m_nFlag == FLAG_NORMAL)
				{
					for(int j=0; j<pcomp->m_arrayPos.GetCount(); j++)
					{
						POSITION subpos = pcomp->m_arrayPos.GetAt(j);
						pobject = pDoc->m_listSchObject[diagram].GetAt(subpos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
						{
							COb_schpin* ppin = (COb_schpin*)pobject;
							if(ppin->m_nFlag != FLAG_NORMAL) continue;

							
							if(( pcomp->m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  pcomp->m_nPart))
							{
								
								if(( pcomp->m_nDisplayModeCount <= 1)||( pcomp->m_nDisplayMode == ppin->m_nDisplayMode))
								{
									if((ppin->m_bHide == TRUE)&&((ppin->m_csConnectTo.IsEmpty()?ppin->m_csName:ppin->m_csConnectTo) == phighlightport->m_csName))
									{
										if(HighlightExist(diagram, subpos, n) == FALSE)
										{
											Struct_ObjectPosition objpos;
											objpos.pos = subpos;
											objpos.diagram = diagram;
											m_arrayHighlight[n].Add(objpos);
										}
									}
								}
							}
						}
					}
				}

				
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);
			}

		}	
	}
}

void CESDView::HighlightSheetEntry(COb_schsheetentry* phighlightsheetentry, int n)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pDoc->m_listSchObject[phighlightsheetentry->m_nDiagram].GetAt(phighlightsheetentry->m_posParent);
	double fx, fy;
	if(phighlightsheetentry->m_nSide == ENTRY_SIDE_LEFT)
	{
		fx = psheetsymbol->m_fX;
		fy = psheetsymbol->m_fY - phighlightsheetentry->m_fOffset;
	}
	else if(phighlightsheetentry->m_nSide == ENTRY_SIDE_TOP)
	{
		fx = psheetsymbol->m_fX + phighlightsheetentry->m_fOffset;
		fy = psheetsymbol->m_fY;
	}
	else if(phighlightsheetentry->m_nSide == ENTRY_SIDE_RIGHT)
	{
		fx = psheetsymbol->m_fX + psheetsymbol->m_fXsize;
		fy = psheetsymbol->m_fY - phighlightsheetentry->m_fOffset;
	}
	else if(phighlightsheetentry->m_nSide == ENTRY_SIDE_BOTTOM)
	{
		fx = psheetsymbol->m_fX + phighlightsheetentry->m_fOffset;
		fy = psheetsymbol->m_fY - psheetsymbol->m_fYsize;
	}

	for(int diagram=0; diagram < SCH_SHEET_NUM; diagram++)
	{
		int num = pDoc->m_listSchObject[diagram].GetCount();
		POSITION pos;
		pos = pDoc->m_listSchObject[diagram].GetHeadPosition();
		for(int i=0; i<num; i++)
		{	
			if(pos == NULL) break;
			POSITION posbak = pos;
			pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
			
			if((diagram == phighlightsheetentry->m_nDiagram)&&(pobject->IsKindOf( RUNTIME_CLASS( COb_schline ))))
			{
				COb_schline* pline = (COb_schline*)pobject;
				if((pline->m_nAttrib != LINE_WIRE)||(pline->m_nDiagram != phighlightsheetentry->m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;

				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					if((fx == pline->m_cVertex[k].fx)&&(fy == pline->m_cVertex[k].fy)) 
					{
						if(HighlightExist(pline->m_nDiagram, posbak, n) == FALSE)
						{
							Struct_ObjectPosition objpos;
							objpos.pos = posbak;
							objpos.diagram = pline->m_nDiagram;
							m_arrayHighlight[n].Add(objpos);
						}
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				if((ptext->m_nID != TEXT_NETLABEL) || (ptext->m_nFlag != FLAG_NORMAL))	continue;

				if(ptext->m_csText == phighlightsheetentry->m_csName)
				{
					if(HighlightExist(ptext->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = ptext->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol;
				psheetsymbol = (COb_schsheetsymbol*)pobject;
				if(psheetsymbol->m_nFlag != FLAG_NORMAL)
				{
					for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);

					continue;
				}

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
				{
					POSITION subpos = pos;
					pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
					{
						COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
						if(psheetentry->m_nFlag != FLAG_NORMAL)	continue;

						if(psheetentry->m_csName == phighlightsheetentry->m_csName)
						{
							if(HighlightExist(psheetsymbol->m_nDiagram, subpos, n) == FALSE)
							{
								Struct_ObjectPosition objpos;
								objpos.pos = subpos;
								objpos.diagram = psheetsymbol->m_nDiagram;
								m_arrayHighlight[n].Add(objpos);
							}
						}
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport = (COb_schport*)pobject;
				if(pport->m_nFlag != FLAG_NORMAL)	continue;
				
				if(pport->m_csName == phighlightsheetentry->m_csName)
				{
					if(HighlightExist(pport->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = pport->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				if(ppower->m_nFlag != FLAG_NORMAL)	continue;
				
				if(ppower->m_csNetName == phighlightsheetentry->m_csName)
				{
					if(HighlightExist(ppower->m_nDiagram, posbak, n) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = ppower->m_nDiagram;
						m_arrayHighlight[n].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;
				if(pcomp->m_nFlag == FLAG_NORMAL)
				{
					for(int j=0; j<pcomp->m_arrayPos.GetCount(); j++)
					{
						POSITION subpos = pcomp->m_arrayPos.GetAt(j);
						pobject = pDoc->m_listSchObject[diagram].GetAt(subpos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
						{
							COb_schpin* ppin = (COb_schpin*)pobject;
							if(ppin->m_nFlag != FLAG_NORMAL) continue;

							
							if(( pcomp->m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  pcomp->m_nPart))
							{
								
								if(( pcomp->m_nDisplayModeCount <= 1)||( pcomp->m_nDisplayMode == ppin->m_nDisplayMode))
								{
									if((ppin->m_bHide == TRUE)&&((ppin->m_csConnectTo.IsEmpty()?ppin->m_csName:ppin->m_csConnectTo) == phighlightsheetentry->m_csName))
									{
										if(HighlightExist(diagram, subpos, n) == FALSE)
										{
											Struct_ObjectPosition objpos;
											objpos.pos = subpos;
											objpos.diagram = diagram;
											m_arrayHighlight[n].Add(objpos);
										}
									}
								}
							}
						}
					}
				}

				
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);
			}
		}	
	}
}

void CESDView::HighlightPin(COb_schpin* phighlightpin, int HighlightBufferNo)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(phighlightpin->m_bHide == TRUE)
	{
		for(int diagram=0; diagram < SCH_SHEET_NUM; diagram++)
		{
			int num = pDoc->m_listSchObject[diagram].GetCount();
			POSITION pos;
			pos = pDoc->m_listSchObject[diagram].GetHeadPosition();
			for(int i=0; i<num; i++)
			{	
				if(pos == NULL) break;
				POSITION posbak = pos;
				pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
				
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext = (COb_schtext*)pobject;
					if((ptext->m_nID != TEXT_NETLABEL) || (ptext->m_nFlag != FLAG_NORMAL))	continue;

					if(ptext->m_csText == (phighlightpin->m_csConnectTo.IsEmpty()?phighlightpin->m_csName:phighlightpin->m_csConnectTo))
					{
						if(HighlightExist(ptext->m_nDiagram, posbak, HighlightBufferNo) == FALSE)
						{
							Struct_ObjectPosition objpos;
							objpos.pos = posbak;
							objpos.diagram = ptext->m_nDiagram;
							m_arrayHighlight[HighlightBufferNo].Add(objpos);
						}
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
				{
					COb_schsheetsymbol* psheetsymbol;
					psheetsymbol = (COb_schsheetsymbol*)pobject;
					if(psheetsymbol->m_nFlag != FLAG_NORMAL)
					{
						for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);

						continue;
					}

					for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
					{
						POSITION subpos = pos;
						pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
						{
							COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
							if(psheetentry->m_nFlag != FLAG_NORMAL)	continue;

							if(psheetentry->m_csName == (phighlightpin->m_csConnectTo.IsEmpty()?phighlightpin->m_csName:phighlightpin->m_csConnectTo))
							{
								if(HighlightExist(psheetsymbol->m_nDiagram, subpos, HighlightBufferNo) == FALSE)
								{
									Struct_ObjectPosition objpos;
									objpos.pos = subpos;
									objpos.diagram = psheetsymbol->m_nDiagram;
									m_arrayHighlight[HighlightBufferNo].Add(objpos);
								}
							}
						}
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
				{
					COb_schport* pport = (COb_schport*)pobject;
					if(pport->m_nFlag != FLAG_NORMAL)	continue;
					
					if(pport->m_csName == (phighlightpin->m_csConnectTo.IsEmpty()?phighlightpin->m_csName:phighlightpin->m_csConnectTo))
					{
						if(HighlightExist(pport->m_nDiagram, posbak, HighlightBufferNo) == FALSE)
						{
							Struct_ObjectPosition objpos;
							objpos.pos = posbak;
							objpos.diagram = pport->m_nDiagram;
							m_arrayHighlight[HighlightBufferNo].Add(objpos);
						}
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
				{
					COb_schpower* ppower = (COb_schpower*)pobject;
					if(ppower->m_nFlag != FLAG_NORMAL)	continue;
					
					if(ppower->m_csNetName == (phighlightpin->m_csConnectTo.IsEmpty()?phighlightpin->m_csName:phighlightpin->m_csConnectTo))
					{
						if(HighlightExist(ppower->m_nDiagram, posbak, HighlightBufferNo) == FALSE)
						{
							Struct_ObjectPosition objpos;
							objpos.pos = posbak;
							objpos.diagram = ppower->m_nDiagram;
							m_arrayHighlight[HighlightBufferNo].Add(objpos);
						}
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
				{
					COb_schcomp* pcomp = (COb_schcomp*)pobject;
					if(pcomp->m_nFlag == FLAG_NORMAL)
					{
						for(int j=0; j<pcomp->m_arrayPos.GetCount(); j++)
						{
							POSITION subpos = pcomp->m_arrayPos.GetAt(j);
							pobject = pDoc->m_listSchObject[diagram].GetAt(subpos);
							if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
							{
								COb_schpin* ppin = (COb_schpin*)pobject;
								if(ppin->m_nFlag != FLAG_NORMAL) continue;

								
								if(( pcomp->m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  pcomp->m_nPart))
								{
									
									if(( pcomp->m_nDisplayModeCount <= 1)||( pcomp->m_nDisplayMode == ppin->m_nDisplayMode))
									{
										if( (ppin->m_bHide == TRUE)&&\
											((ppin->m_csConnectTo.IsEmpty()?ppin->m_csName:ppin->m_csConnectTo) == (phighlightpin->m_csConnectTo.IsEmpty()?phighlightpin->m_csName:phighlightpin->m_csConnectTo)))
										{
											if(HighlightExist(diagram, subpos, HighlightBufferNo) == FALSE)
											{
												Struct_ObjectPosition objpos;
												objpos.pos = subpos;
												objpos.diagram = diagram;
												m_arrayHighlight[HighlightBufferNo].Add(objpos);
											}
										}
									}
								}
							}
						}
					}

					
					for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);
				}

			}	
		}
	}
	else
	{
		double fx, fy;
		if(phighlightpin->m_nOrientation == 0)
		{
			fx = phighlightpin->m_fX + phighlightpin->m_fPinLength;
			fy = phighlightpin->m_fY;
		}
		else if(phighlightpin->m_nOrientation == 90)
		{
			fx = phighlightpin->m_fX;
			fy = phighlightpin->m_fY + phighlightpin->m_fPinLength;
		}
		else if(phighlightpin->m_nOrientation == 180)
		{
			fx = phighlightpin->m_fX - phighlightpin->m_fPinLength;
			fy = phighlightpin->m_fY;
		}
		else if(phighlightpin->m_nOrientation == 270)
		{
			fx = phighlightpin->m_fX;
			fy = phighlightpin->m_fY - phighlightpin->m_fPinLength;
		}

		int num = pDoc->m_listSchObject[phighlightpin->m_nDiagram].GetCount();
		POSITION pos;
		pos = pDoc->m_listSchObject[phighlightpin->m_nDiagram].GetHeadPosition();
		for(int i=0; i<num; i++)
		{	
			if(pos == NULL) break;
			POSITION posbak = pos;
			pobject = pDoc->m_listSchObject[phighlightpin->m_nDiagram].GetNext(pos);
			
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				if((pline->m_nAttrib != LINE_WIRE)||(pline->m_nDiagram != phighlightpin->m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL))	continue;
				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					if((fx == pline->m_cVertex[k].fx)&&(fy == pline->m_cVertex[k].fy)) 
					{
						if(HighlightExist(pline->m_nDiagram, posbak, HighlightBufferNo) == FALSE)
						{
							Struct_ObjectPosition objpos;
							objpos.pos = posbak;
							objpos.diagram = pline->m_nDiagram;
							m_arrayHighlight[HighlightBufferNo].Add(objpos);
						}
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc = (COb_schjunc*)pobject;
				if((pjunc->m_nDiagram != phighlightpin->m_nDiagram)||(pjunc->m_nFlag != FLAG_NORMAL))	continue;

				if((fx == pjunc->m_fCx)&&(fy == pjunc->m_fCy)) 
				{
					if(HighlightExist(pjunc->m_nDiagram, posbak, HighlightBufferNo) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = pjunc->m_nDiagram;
						m_arrayHighlight[HighlightBufferNo].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol;
				psheetsymbol = (COb_schsheetsymbol*)pobject;
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[phighlightpin->m_nDiagram].GetNext(pos);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport = (COb_schport*)pobject;
				if((pport->m_nDiagram != phighlightpin->m_nDiagram)||(pport->m_nFlag != FLAG_NORMAL))	continue;
				if((pport->m_fX == fx)&&(pport->m_fY == fy)) 
				{
					if(HighlightExist(pport->m_nDiagram, posbak, HighlightBufferNo) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = pport->m_nDiagram;
						m_arrayHighlight[HighlightBufferNo].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				if((ppower->m_nDiagram != phighlightpin->m_nDiagram)||(ppower->m_nFlag != FLAG_NORMAL))	continue;
				if((ppower->m_fX == fx)&&(ppower->m_fY == fy)) 
				{
					if(HighlightExist(ppower->m_nDiagram, posbak, HighlightBufferNo) == FALSE)
					{
						Struct_ObjectPosition objpos;
						objpos.pos = posbak;
						objpos.diagram = ppower->m_nDiagram;
						m_arrayHighlight[HighlightBufferNo].Add(objpos);
					}
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp;
				pcomp = (COb_schcomp*)pobject;
				if((pcomp->m_nDiagram == phighlightpin->m_nDiagram)&&(pcomp->m_nFlag == FLAG_NORMAL))	
				{
					for(int j=0; j<pcomp->m_arrayPos.GetCount(); j++)
					{
						POSITION subpos = pcomp->m_arrayPos.GetAt(j);
						pobject = pDoc->m_listSchObject[pcomp->m_nDiagram].GetAt(subpos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
						{
							COb_schpin* ppin = (COb_schpin*)pobject;
							if(ppin->m_nFlag != FLAG_NORMAL) continue;

							
							if(( pcomp->m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  pcomp->m_nPart))
							{
								
								if(( pcomp->m_nDisplayModeCount <= 1)||( pcomp->m_nDisplayMode == ppin->m_nDisplayMode))
								{
									double fx1, fy1;
									if(ppin->m_nOrientation == 0)
									{
										fx1 = ppin->m_fX + ppin->m_fPinLength;
										fy1 = ppin->m_fY;
									}
									else if(ppin->m_nOrientation == 90)
									{
										fx1 = ppin->m_fX;
										fy1 = ppin->m_fY + ppin->m_fPinLength;
									}
									else if(ppin->m_nOrientation == 180)
									{
										fx1 = ppin->m_fX - ppin->m_fPinLength;
										fy1 = ppin->m_fY;
									}
									else if(ppin->m_nOrientation == 270)
									{
										fx1 = ppin->m_fX;
										fy1 = ppin->m_fY - ppin->m_fPinLength;
									}

									if((fx == fx1)&&(fy == fy1)) 
									{
										if(HighlightExist(pcomp->m_nDiagram, subpos, HighlightBufferNo) == FALSE)
										{
											Struct_ObjectPosition objpos;
											objpos.pos = subpos;
											objpos.diagram = pcomp->m_nDiagram;
											m_arrayHighlight[HighlightBufferNo].Add(objpos);
										}
									}
								}
							}
						}
					}
				}

				
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[pcomp->m_nDiagram].GetNext(pos);
			}
		}	
	}
}

BOOL CESDView::HighlightExist(int diagram, POSITION pos, int HighlightBufferNo)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<m_arrayHighlight[HighlightBufferNo].GetCount(); i++)
	{
		Struct_ObjectPosition objpos = m_arrayHighlight[HighlightBufferNo].GetAt(i);
		if((diagram == objpos.diagram)&&(pos == objpos.pos)) return TRUE;
	}

	CObject* pobject;
	pobject = pDoc->m_listSchObject[diagram].GetAt(pos);
	if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
	{
		
		COb_schpin* ppin = (COb_schpin*)pobject;
		COb_schcomp* pcomp = (COb_schcomp*)pDoc->m_listSchObject[diagram].GetAt(ppin->m_posParent);

		if((pcomp->m_nPartCount > 1)&&(ppin->m_nOwnerPartID == 0))
		{
			for(int i=0; i<m_arrayHighlight[HighlightBufferNo].GetCount(); i++)
			{
				Struct_ObjectPosition objpos = m_arrayHighlight[HighlightBufferNo].GetAt(i);
				CObject* pobject_1;
				pobject_1 = pDoc->m_listSchObject[objpos.diagram].GetAt(objpos.pos);
				if(pobject_1->IsKindOf( RUNTIME_CLASS( COb_schpin )))
				{		
					COb_schpin* ppin_1 = (COb_schpin*)pobject_1;
					COb_schcomp* pcomp_1 = (COb_schcomp*)pDoc->m_listSchObject[objpos.diagram].GetAt(ppin_1->m_posParent);

					
					if((pcomp_1->m_nPartCount > 1)&&(ppin_1->m_nOwnerPartID == 0))
					{
						if((pcomp->GetDesignator() == pcomp_1->GetDesignator())&&(ppin->m_csNumber == ppin_1->m_csNumber)) return TRUE;
					}
				}
			}
		}
	}

	return FALSE;
}

void CESDView::DrawHighlight(CDC* pDC)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	for(int n=0; n<6; n++)
	{
		CObject* pobject;
		for(int i=0; i<m_arrayHighlight[n].GetCount(); i++)
		{

			Struct_ObjectPosition objpos;
			objpos = m_arrayHighlight[n].GetAt(i);
			pobject = pDoc->m_listSchObject[objpos.diagram].GetAt(objpos.pos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL)) continue;
				
				if(pline->PickMe(this) == TRUE)
				{
					pline->DrawHighlight(pDC, this, m_nHighlightColor[n]);
					if((pline->m_bSelection == TRUE)&&(nEDA_Print_Flag == 0)) pline->DrawSelection(pDC, this);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
			{
				COb_schpin* ppin = (COb_schpin*)pobject;
				POSITION posParent = ppin->m_posParent;
				COb_schcomp* pcomp = (COb_schcomp*)pDoc->m_listSchObject[objpos.diagram].GetAt(posParent);
				if((pcomp->m_nDiagram != m_nDiagram)||(pcomp->m_nFlag != FLAG_NORMAL)||(ppin->m_nFlag != FLAG_NORMAL)) continue;

				
				if(( pcomp->m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  pcomp->m_nPart))
				{
					
					if(( pcomp->m_nDisplayModeCount <= 1)||( pcomp->m_nDisplayMode == ppin->m_nDisplayMode))
					{
						ppin->DrawHighlight(pDC, this,m_nHighlightColor[n]);
					}
				}
			}
		}

		for(int i=0; i<m_arrayHighlight[n].GetCount(); i++)
		{
			Struct_ObjectPosition objpos;
			objpos = m_arrayHighlight[n].GetAt(i);
			pobject = pDoc->m_listSchObject[objpos.diagram].GetAt(objpos.pos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
			{
				COb_schline* pline = (COb_schline*)pobject;
				if((pline->m_nDiagram != m_nDiagram)||(pline->m_nFlag != FLAG_NORMAL)) continue;
				
				if(pline->PickMe(this) == TRUE)
				{
					pline->Draw(pDC, this);
					if((pline->m_bSelection == TRUE)&&(nEDA_Print_Flag == 0)) pline->DrawSelection(pDC, this);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
			{
				COb_schjunc* pjunc = (COb_schjunc*)pobject;
				if((pjunc->m_nDiagram != m_nDiagram)||(pjunc->m_nFlag != FLAG_NORMAL)) continue;
				
				pjunc->Draw(pDC, this);	
				if((pjunc->m_bSelection == TRUE)&&(nEDA_Print_Flag == 0)) pjunc->DrawSelection(pDC, this);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag != FLAG_NORMAL)) continue;
				
				if(ptext->PickMe(pDC, this) == TRUE) 
				{
					ptext->DrawSignal(pDC, this);
					if((ptext->m_bSelection == TRUE)&&(nEDA_Print_Flag == 0)) ptext->DrawSelection(pDC, this);
				}
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
			{
				COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
				if((psheetentry->m_nDiagram != m_nDiagram)||(psheetentry->m_nFlag != FLAG_NORMAL)) continue;

				psheetentry->Draw(pDC, this);
				if((psheetentry->m_bSelection == TRUE)&&(nEDA_Print_Flag == 0)) psheetentry->DrawSelection(pDC, this);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport = (COb_schport*)pobject;
				if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag != FLAG_NORMAL)) continue;

				pport->Draw(pDC, this);
				if((pport->m_bSelection == TRUE)&&(nEDA_Print_Flag == 0)) pport->DrawSelection(pDC, this);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				if((ppower->m_nDiagram != m_nDiagram)||(ppower->m_nFlag != FLAG_NORMAL)) continue;
				
				ppower->Draw(pDC, this);
				if((ppower->m_bSelection == TRUE)&&(nEDA_Print_Flag == 0)) ppower->DrawSelection(pDC, this);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
			{
				COb_schpin* ppin = (COb_schpin*)pobject;
				POSITION posParent = ppin->m_posParent;
				COb_schcomp* pcomp = (COb_schcomp*)pDoc->m_listSchObject[objpos.diagram].GetAt(posParent);
				if((pcomp->m_nDiagram != m_nDiagram)||(pcomp->m_nFlag != FLAG_NORMAL)||(ppin->m_nFlag != FLAG_NORMAL)) continue;

				
				if(( pcomp->m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  pcomp->m_nPart))
				{
					
					if(( pcomp->m_nDisplayModeCount <= 1)||( pcomp->m_nDisplayMode == ppin->m_nDisplayMode))
					{
						ppin->Draw(pDC, this);
					}
				}
			}
		}
	}
}

void CESDView::DrawMask(CDC* pDC)
{
	BITMAP bitmap;
	bitmap.bmType = 0;
	bitmap.bmWidth = m_nViewport_Ex;
	bitmap.bmHeight = m_nViewport_Ey;

	bitmap.bmWidthBytes = m_nViewport_Ex/16;
	if((m_nViewport_Ex%16) != 0) bitmap.bmWidthBytes++;
	bitmap.bmWidthBytes *= 2;

	bitmap.bmPlanes = 1;
	bitmap.bmBitsPixel = 1;

	int size = bitmap.bmWidthBytes*m_nViewport_Ey;
	BYTE* pBuffer = (BYTE*)malloc(size);
	bitmap.bmBits = pBuffer;

	for(int i=0;i<m_nViewport_Ey;i++)
	{
		for(int j=0; j<bitmap.bmWidthBytes; j++)
		{
			if((i%2) == 0)  pBuffer[i*bitmap.bmWidthBytes + j] = 0xaa;
			else pBuffer[i*bitmap.bmWidthBytes + j] = 0x55;
		}
	}
		
	CBitmap cbitmap;
	cbitmap.CreateBitmapIndirect(&bitmap);

	CDC			dcMem;	
	dcMem.CreateCompatibleDC(pDC);

	// Select the bitmap into the memory DC.
	CBitmap* pOldBitmap = dcMem.SelectObject(&cbitmap);

	// Blt the bitmap to screen
	pDC->BitBlt( 0, 0, m_nViewport_Ex, m_nViewport_Ey, &dcMem, 0, 0, SRCPAINT);
		
	// cleanup
	dcMem.SelectObject(pOldBitmap);

	free(pBuffer);
}

CString CESDView::GetNetName(int HighlightBufferNo)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	CStringArray arrayString;
	arrayString.RemoveAll();
	for(int j=0; j<m_arrayHighlight[HighlightBufferNo].GetCount(); j++)
	{
		Struct_ObjectPosition objectpos;
		objectpos = m_arrayHighlight[HighlightBufferNo].GetAt(j);
		CObject* pobject = pDoc->m_listSchObject[objectpos.diagram].GetAt(objectpos.pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin)))
		{
			COb_schpin* ppin = (COb_schpin*)pobject;
			//if(ppin->m_bHide == TRUE) continue;

			COb_schcomp* pcomp = (COb_schcomp*)pDoc->m_listSchObject[objectpos.diagram].GetAt(ppin->m_posParent);
			CString string = pcomp->GetDesignator() + "-" + ppin->m_csNumber;
			arrayString.Add(string);
		}
	}			

	
	int judge = 1;
	for(;;)
	{
		if(judge == 1) judge = 0;
		else if(judge == 0) break;
		for(int i=0;i<arrayString.GetCount()-1;i++)
		{
			CString str1 = arrayString.GetAt(i);
			CString str2 = arrayString.GetAt(i+1);

			if(str1 > str2)
			{
				judge=1;
				arrayString.SetAt(i, str2);
				arrayString.SetAt(i+1, str1);
			}
		}
	}

	CString netname;
	netname.Empty();
	for(int i=0; i<m_arrayHighlight[HighlightBufferNo].GetCount(); i++)
	{
		Struct_ObjectPosition objpos;
		objpos = m_arrayHighlight[HighlightBufferNo].GetAt(i);
		pobject = pDoc->m_listSchObject[objpos.diagram].GetAt(objpos.pos);
			
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext = (COb_schtext*)pobject;
			netname = ptext->m_csText;
			break;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
		{
			COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
			netname = psheetentry->m_csName;
			break;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport = (COb_schport*)pobject;
			netname = pport->m_csName;
			break;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower = (COb_schpower*)pobject;
			netname = ppower->m_csNetName;
			break;
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
		{
			COb_schpin* ppin = (COb_schpin*)pobject;
			if(ppin->m_bHide == TRUE) 
			{
				netname = (ppin->m_csConnectTo.IsEmpty()?ppin->m_csName:ppin->m_csConnectTo);  //ppin->m_csConnectTo;
				break;
			}
		}
	}
	
	if(arrayString.GetCount() >= 2)
	{
		if(netname.IsEmpty() == TRUE) netname = arrayString.GetAt(0) + "_" + arrayString.GetAt(1);
	}
	else if(arrayString.GetCount() == 1) netname = arrayString.GetAt(0);
	else netname = "N/A 没有网络";

	return netname;
}

void CESDView::OnAutoCompile()
{
	
	int delta = 35;
	CObject* pobject;
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

	
	char filestring[MAX_PATH];
	sprintf(filestring,"%s.NET", filepath);
	CFile cfileNet;
	if(cfileNet.Open(filestring,  CFile::modeCreate|CFile::modeWrite) != TRUE) 
	{
		AfxMessageBox("编译错误, 无法创建网络表文件.", MB_ICONSTOP);
		return;
	}
	CArchive arNet(&cfileNet, CArchive::store);

	
	sprintf(filestring,"%s.WIR", filepath);
	CFile cfileWir;
	if(cfileWir.Open(filestring,  CFile::modeCreate|CFile::modeWrite) != TRUE) 
	{
		AfxMessageBox("编译错误, 无法创建连线表文件.", MB_ICONSTOP);
		arNet.Close();  cfileNet.Close();
		return;
	}
	CArchive arWir(&cfileWir, CArchive::store);

	
	sprintf(filestring,"%s.BM1", filepath);
	CFile cfileBm1;
	if(cfileBm1.Open(filestring,  CFile::modeCreate|CFile::modeWrite) != TRUE) 
	{
		arNet.Close();  cfileNet.Close();
		arWir.Close();	cfileWir.Close();
		AfxMessageBox("编译错误, 无法创建元件清单文件.", MB_ICONSTOP);
		return;
	}
	CArchive arBm1(&cfileBm1, CArchive::store);

	
	sprintf(filestring,"%s.BM2", filepath);
	CFile cfileBm2;
	if(cfileBm2.Open(filestring,  CFile::modeCreate|CFile::modeWrite) != TRUE) 
	{
		arNet.Close();  cfileNet.Close();
		arWir.Close();	cfileWir.Close();
		arBm1.Close();	cfileBm1.Close();
		AfxMessageBox("编译错误, 无法创建元件清单文件.", MB_ICONSTOP);
		return;
	}
	CArchive arBm2(&cfileBm2, CArchive::store);

	
	sprintf(filestring,"%s.ERR", filepath);
	CFile cfileErr;
	if(cfileErr.Open(filestring,  CFile::modeCreate|CFile::modeWrite) != TRUE) 
	{
		arNet.Close();  cfileNet.Close();
		arWir.Close();	cfileWir.Close();
		arBm1.Close();	cfileBm1.Close();
		arBm2.Close();	cfileBm2.Close();
		AfxMessageBox("编译错误, 无法创建出错报告文件.", MB_ICONSTOP);
		return;
	}
	CArchive arErr(&cfileErr, CArchive::store);

	Write_Help("检索所有元件......");
	
	CStringArray arrayCompDesignator;	arrayCompDesignator.RemoveAll();
	CStringArray arrayCompComment;		arrayCompComment.RemoveAll();
	CStringArray arrayCompPackage;		arrayCompPackage.RemoveAll();
	CStringArray arrayPackageLibraryName;	arrayPackageLibraryName.RemoveAll();
	for(int diagram=0; diagram<SCH_SHEET_NUM; diagram++)
	{
		int num = pDoc->m_listSchObject[diagram].GetCount();
		POSITION pos;
		pos = pDoc->m_listSchObject[diagram].GetHeadPosition();
		for(int i=0; i<num; i++)
		{	
			if(pos == NULL) break;
			pobject = pDoc->m_listSchObject[diagram].GetNext(pos);

			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;
				if(pcomp->m_nFlag == FLAG_NORMAL)
				{
					
					if(pcomp->m_nPartCount > 1)
					{
						char s[100];
						s[0] = 64 + pcomp->m_nPart;	s[1] = '\0';
						CString cstring;
						
						cstring = pcomp->GetDesignator();
						cstring += s;
						cstring += "_@_%_";
						arrayCompDesignator.Add(cstring);
						
						cstring = pcomp->GetComment();
						arrayCompComment.Add(cstring);
						
						cstring = pcomp->GetPackage();
						arrayCompPackage.Add(cstring);
						
						cstring = pcomp->m_csLibraryFileName;
						arrayPackageLibraryName.Add(cstring);
					}
					else
					{
						CString cstring;
						
						cstring = pcomp->GetDesignator();
						arrayCompDesignator.Add(cstring);
						
						cstring = pcomp->GetComment();
						arrayCompComment.Add(cstring);
						
						cstring = pcomp->GetPackage();
						arrayCompPackage.Add(cstring);
						
						cstring = pcomp->m_csLibraryFileName;
						arrayPackageLibraryName.Add(cstring);
					}
				}

				
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
			}
		}
	}

	
	for(int i=0; i<arrayCompDesignator.GetCount(); i++)
	{
		CString str1, str2;
		str1 = arrayCompDesignator.GetAt(i);
		str1.TrimRight("_@_%_");

		int j;
		for(j=i+1; j<arrayCompDesignator.GetCount(); j++)
		{
			str2 = arrayCompDesignator.GetAt(j);
			str2.TrimRight("_@_%_");
			if(str1.CompareNoCase(str2) == 0) break;
		}
		if(j < arrayCompDesignator.GetCount())
		{ 
			CString errstr;
			errstr = "<错误>";
			errstr += "相同元件名称:";
			errstr += str1 + "," + str2;
			errstr += "\r\n";
			arErr.WriteString(errstr);
		}
	}

	Write_Help("排序......");
	
	int judge = 1;
	for(;;)
	{
		if(judge == 1) judge = 0;
		else if(judge == 0) break;
		for(int i=0;i<arrayCompDesignator.GetCount()-1;i++)
		{
			CString str1 = arrayCompDesignator.GetAt(i);
			CString str2 = arrayCompDesignator.GetAt(i+1);

			if(str1 > str2)
			{
				judge=1;
				arrayCompDesignator.SetAt(i, str2);
				arrayCompDesignator.SetAt(i+1, str1);
				
				
				str1 = arrayCompComment.GetAt(i);
				str2 = arrayCompComment.GetAt(i+1);
				arrayCompComment.SetAt(i, str2);
				arrayCompComment.SetAt(i+1, str1);
				
				str1 = arrayCompPackage.GetAt(i);
				str2 = arrayCompPackage.GetAt(i+1);
				arrayCompPackage.SetAt(i, str2);
				arrayCompPackage.SetAt(i+1, str1);
				
				str1 = arrayPackageLibraryName.GetAt(i);
				str2 = arrayPackageLibraryName.GetAt(i+1);
				arrayPackageLibraryName.SetAt(i, str2);
				arrayPackageLibraryName.SetAt(i+1, str1);
			}
		}
	}

	Write_Help("产生明细表...");
	pDoc->m_arrayCompComment.RemoveAll();
	pDoc->m_arrayCompDesignator.RemoveAll();
	pDoc->m_arrayCompPackage.RemoveAll();
	pDoc->m_arrayPackageLibraryName.RemoveAll();

	CStringArray arrayCompDesignator2;	arrayCompDesignator2.RemoveAll();
	CStringArray arrayCompComment2;		arrayCompComment2.RemoveAll();
	
	char *divideline="----|------------------------------------------------------------------------------------------------------------\r\n";
	arBm1.WriteString("序号|   元件名称(Designator)               封装(Footprint)                    注释(Comment)\r\n");
	arBm1.WriteString(divideline);
	index = 1;  char c_index[100];
	for(int i=0; i<arrayCompDesignator.GetCount(); i++)
	{	
		CString oneline, netstring;
		
		CString cstring = arrayCompDesignator.GetAt(i);
		netstring = "[\r\n";
		arNet.WriteString(netstring);

		if(cstring.Find("_@_%_") >= 0)
		{
			
			cstring = cstring.Left(cstring.Find("_@_%_"));
			char c = cstring.GetAt(cstring.GetLength() - 1);

			cstring = cstring.Left(cstring.GetLength() - 1);
			
			
			pDoc->m_arrayCompDesignator.Add(cstring);
			pDoc->m_arrayCompComment.Add(arrayCompComment.GetAt(i));
			pDoc->m_arrayCompPackage.Add(arrayCompPackage.GetAt(i));
			pDoc->m_arrayPackageLibraryName.Add(arrayPackageLibraryName.GetAt(i));
			
			
			arNet.WriteString(cstring);
			arNet.WriteString("\r\n");
			
			oneline = cstring;
			oneline += "(";

			char s[2]; 
			s[1] = '\0';
			s[0] = c; 
			oneline += s;
			int k = i;
			for(;;)
			{
				k++;
				if(k >= arrayCompDesignator.GetCount()) break;	
				CString str = arrayCompDesignator.GetAt(k);
				if(str.Find("_@_%_") < 0) break;				
				else
				{
					str = str.Left(str.Find("_@_%_"));
					char cc = str.GetAt(str.GetLength() - 1);
					str = str.Left(str.GetLength() - 1);
					if(str.CompareNoCase(cstring) != 0) break;	
					oneline += ",";
					s[0] = cc;
					oneline += s;
				}
			}

			oneline += ")";
			cstring = oneline;			arrayCompDesignator2.Add(cstring);
			if(cstring.GetLength() < delta) for(int i=cstring.GetLength(); i<delta; i++) oneline += " ";
			else oneline += " ";
			
			
			cstring = arrayCompPackage.GetAt(i);

			
			netstring = cstring + "\r\n";
			arNet.WriteString(netstring);

			
			oneline += cstring;
			if(cstring.GetLength() < delta) for(int i=cstring.GetLength(); i<delta; i++) oneline += " ";
			else oneline += " ";
			
			
			cstring = arrayCompComment.GetAt(i);  arrayCompComment2.Add(cstring);

			
			netstring = cstring + "\r\n";
			arNet.WriteString(netstring);

			oneline += cstring;

			
			oneline += "\r\n";

			
			itoa(index, c_index, 10);
			cstring = c_index;
			if(cstring.GetLength() < 8) 
			{
				for(int j=cstring.GetLength(); j<8; j++) 
				{
					if( j == 4) cstring += "|";
					else cstring += " ";
				}
			}
			oneline = cstring + oneline;
			arBm1.WriteString(oneline);
			index++;

			i = k-1;
		}
		else
		{
			pDoc->m_arrayCompDesignator.Add(cstring);
			pDoc->m_arrayCompComment.Add(arrayCompComment.GetAt(i));
			pDoc->m_arrayCompPackage.Add(arrayCompPackage.GetAt(i));
			pDoc->m_arrayPackageLibraryName.Add(arrayPackageLibraryName.GetAt(i));

			
			arNet.WriteString(cstring);
			arNet.WriteString("\r\n");

			oneline = cstring;   arrayCompDesignator2.Add(cstring);
			if(cstring.GetLength() < delta) for(int j=cstring.GetLength(); j<delta; j++) oneline += " ";
			else oneline += " ";
			
			
			cstring = arrayCompPackage.GetAt(i);
			
			
			arNet.WriteString(cstring);
			arNet.WriteString("\r\n");
			
			oneline += cstring;
			if(cstring.GetLength() < delta) for(int j=cstring.GetLength(); j<delta; j++) oneline += " ";
			else oneline += " ";
			
			
			
			cstring = arrayCompComment.GetAt(i);  arrayCompComment2.Add(cstring);

			
			arNet.WriteString(cstring);
			arNet.WriteString("\r\n");

			oneline += cstring;

			
			oneline += "\r\n";
				
			
			itoa(index, c_index, 10);
			cstring = c_index;
			if(cstring.GetLength() < 8) 
			{
				for(int j=cstring.GetLength(); j<8; j++) 
				{
					if( j == 4) cstring += "|";
					else cstring += " ";
				}
			}
			oneline = cstring + oneline;
			arBm1.WriteString(oneline);
			index++;
		}

		netstring = "\r\n\r\n\r\n]\r\n";
		arNet.WriteString(netstring);
	}

	
	
	judge = 1;
	for(;;)
	{
		if(judge == 1) judge = 0;
		else if(judge == 0) break;
		for(int i=0;i<arrayCompComment2.GetCount()-1;i++)
		{
			CString str1 = arrayCompComment2.GetAt(i);
			CString str2 = arrayCompComment2.GetAt(i+1);

			if(str1 > str2)
			{
				judge=1;
				arrayCompComment2.SetAt(i, str2);
				arrayCompComment2.SetAt(i+1, str1);
				
				
				str1 = arrayCompDesignator2.GetAt(i);
				str2 = arrayCompDesignator2.GetAt(i+1);
				arrayCompDesignator2.SetAt(i, str2);
				arrayCompDesignator2.SetAt(i+1, str1);
			}
		}
	}

	Write_Help("产生元件统计列表......");
	
	arBm2.WriteString("序号|   注释(Comment)                      数量(Number)                       名称(Designator)\r\n");
	arBm2.WriteString(divideline);

	CString prevComment = "NA(未知)";
	index = 1;
	for(int i=0; i<arrayCompComment2.GetCount(); i++)
	{
		CString oneline; oneline.Empty();
		CString str = arrayCompComment2.GetAt(i);
		
		
		int num = 1;
		for(int k=i+1; k<arrayCompComment2.GetCount(); k++)
		{
			CString str1 = arrayCompComment2.GetAt(k);
			if(str1.CompareNoCase(str) == 0) num++;
			else break;
		}

		oneline = str;
		if(str.GetLength() < delta) for(int k=str.GetLength(); k<delta; k++) oneline += " ";
		else oneline += " ";
		char snum[100];  itoa(num, snum, 10);
		oneline += snum;
		if(strlen(snum) < delta) for(int k=strlen(snum); k<delta; k++) oneline += " ";
		else oneline += " ";
		oneline += arrayCompDesignator2.GetAt(i);
		oneline += "\r\n";

		
		itoa(index, c_index, 10);
		CString cstring = c_index;
		if(cstring.GetLength() < 8) 
		{
			for(int i=cstring.GetLength(); i<8; i++) 
			{
				if( i == 4) cstring += "|";
				else cstring += " ";
			}
		}
		oneline = cstring + oneline;
		arBm2.WriteString(oneline);
		index++;

		oneline.Empty();
		for(int k=1; k<num; k++)
		{
			for(int j=0; j<delta*2; j++) oneline += " ";
			oneline += arrayCompDesignator2.GetAt(i+k);
			oneline += "\r\n";
			
			
			itoa(index, c_index, 10);
			cstring = c_index;
			if(cstring.GetLength() < 8) 
			{
				for(int i=cstring.GetLength(); i<8; i++) 
				{
					if( i == 4) cstring += "|";
					else cstring += " ";
				}
			}
			oneline = cstring + oneline;			
			arBm2.WriteString(oneline);
			index++;
			oneline.Empty();
		}

		i += num-1;
	}


	
	for(int n=0; n<6; n++) m_arrayHighlight[n].RemoveAll();
	m_bViewHighlightFlag = FALSE;
	Invalidate();
	
	while (!pDoc->m_arraySchNet.IsEmpty())
	{
		delete pDoc->m_arraySchNet.GetAt(0);
        pDoc->m_arraySchNet.RemoveAt(0);
	}
	
	Write_Help("网络编译...");
	CClientDC dc(this);
	for(int diagram=0; diagram<SCH_SHEET_NUM; diagram++)
	{
		int num = pDoc->m_listSchObject[diagram].GetCount();
		POSITION pos;
		pos = pDoc->m_listSchObject[diagram].GetHeadPosition();
		for(int i=0; i<num; i++)
		{	
			if(pos == NULL) break;
			pobject = pDoc->m_listSchObject[diagram].GetNext(pos);

			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
			{
				COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
			{
				COb_schcomp* pcomp = (COb_schcomp*)pobject;
				if(pcomp->m_nFlag == FLAG_NORMAL)
				{
					for(int j=0; j< pcomp->m_arrayPos.GetSize(); j++)
					{
						POSITION subpos =  pcomp->m_arrayPos.GetAt(j);
						pobject = pDoc->m_listSchObject[diagram].GetAt(subpos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
						{
							COb_schpin* ppin = (COb_schpin*)pobject;

							if(ppin->m_nFlag == FLAG_DELETED) continue;
							
							if(( pcomp->m_nPartCount <= 1)||(ppin->m_nOwnerPartID == 0)||(ppin->m_nOwnerPartID ==  pcomp->m_nPart))
							{
								
								if(( pcomp->m_nDisplayModeCount <= 1)||( pcomp->m_nDisplayMode == ppin->m_nDisplayMode))
								{
									Struct_ObjectPosition objpos;
									objpos.diagram = diagram;
									objpos.pos = subpos;

									
									if(HighlightExist(objpos.diagram, objpos.pos, 1) == TRUE) continue;
									m_arrayHighlight[0].Add(objpos);
									for(int k=0; k<m_arrayHighlight[0].GetCount(); k++)
									{
										Struct_ObjectPosition objpos;
										objpos = m_arrayHighlight[0].GetAt(k);
										pobject = pDoc->m_listSchObject[objpos.diagram].GetAt(objpos.pos);
										if(pobject->IsKindOf( RUNTIME_CLASS( COb_schline )))
										{
											COb_schline* pline = (COb_schline*)pobject;
											HighlightWire(pline, 0);
										}
										else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schjunc )))
										{
											COb_schjunc* pjunc = (COb_schjunc*)pobject;
											HighlightJunc(pjunc, 0);
										}
										else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
										{
											COb_schtext* ptext = (COb_schtext*)pobject;
											HighlightNetlabel(ptext, 0);
										}
										else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetentry )))
										{
											COb_schsheetentry* psheetentry = (COb_schsheetentry*)pobject;
											HighlightSheetEntry(psheetentry, 0);
										}
										else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
										{
											COb_schport* pport = (COb_schport*)pobject;
											HighlightPort(pport, 0);
										}
										else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
										{
											COb_schpower* ppower = (COb_schpower*)pobject;
											HighlightPower(ppower, 0);
										}
										else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
										{
											COb_schpin* ppin = (COb_schpin*)pobject;
											HighlightPin(ppin, 0);

										}
									}

									
									CString cstring = GetNetName(0);
									char netname_str[1000];
									strcpy(netname_str, "网络编译:");
									strcat(netname_str, cstring);
									Write_Help(netname_str);
									if(cstring == "N/A 没有网络")
									{
									}
									else if(GetHighlightPinNumber(0) == 1)
									{
										CString oneline;
										oneline = "<警告: 未连接引脚> ";
										oneline += cstring;
										oneline += "\r\n";
										arErr.WriteString(oneline);
									}
									else
									{
										CString oneline;
										oneline = "网络名称: ";
										oneline += cstring;
										oneline += "\r\n";
										arWir.WriteString(oneline);
										arWir.WriteString("--------------------------------------------------------------------------------\r\n");
										int flag = 1;
										oneline.Empty();

										
										CString netstring;
										netstring = "(\r\n";
										arNet.WriteString(netstring);
										netstring = cstring + "\r\n";
										arNet.WriteString(netstring);
										
										COb_schnet* pschnet = new COb_schnet();
										pDoc->m_arraySchNet.Add(pschnet);
										pschnet->m_csNetName = cstring;
										pschnet->m_arrayCompDesignator.RemoveAll();
										pschnet->m_arrayPinNumber.RemoveAll();

										for(int k=0; k<m_arrayHighlight[0].GetCount(); k++)
										{
											Struct_ObjectPosition objpos;
											objpos = m_arrayHighlight[0].GetAt(k);
											pobject = pDoc->m_listSchObject[objpos.diagram].GetAt(objpos.pos);
											if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
											{
												COb_schpin* ppin = (COb_schpin*)pobject;
												COb_schcomp* pcomp = (COb_schcomp*)pDoc->m_listSchObject[objpos.diagram].GetAt(ppin->m_posParent);
												cstring = pcomp->GetDesignator() + "-" + ppin->m_csNumber;

												
												netstring = cstring + "\r\n";
												arNet.WriteString(netstring);
												pschnet->m_arrayCompDesignator.Add(pcomp->GetDesignator());
												pschnet->m_arrayPinNumber.Add(ppin->m_csNumber);

												oneline += cstring;
												if(oneline.GetLength() < flag*20) for(int m=oneline.GetLength(); m < flag*20; m++) oneline += " ";
												else oneline += " ";
												flag++;
												if(flag > 4)
												{
													oneline += "\r\n";
													arWir.WriteString(oneline);
													oneline.Empty();
													flag = 1;
												}
												m_arrayHighlight[1].Add(objpos);
											}
										}
										if(flag > 1) 
										{
											oneline += "\r\n";
											arWir.WriteString(oneline);
										}
										arWir.WriteString("\r\n");
										arWir.WriteString("\r\n");

										
										netstring = ")\r\n";
										arNet.WriteString(netstring);
TRACE1("%s\n",netname_str);										
									}

									
									m_arrayHighlight[0].RemoveAll();
								}
							}
						}
					}
				}

				
				for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);
			}
		}
	}

	m_arrayHighlight[0].RemoveAll();
	m_arrayHighlight[1].RemoveAll();

	arNet.Flush();  arNet.Close();  cfileNet.Close();
	arWir.Flush();	arWir.Close();	cfileWir.Close();
	arBm1.Flush();	arBm1.Close();	cfileBm1.Close();
	arBm2.Flush();	arBm2.Close();	cfileBm2.Close();
	arErr.Flush();	arErr.Close();	cfileErr.Close();

	AfxMessageBox("编译完成");
	Write_Help(szEDA_Help_Ready);
}

int CESDView::GetHighlightPinNumber(int HighlightBufferNo)
{
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	int num = 0;
	for(int j=0; j<m_arrayHighlight[HighlightBufferNo].GetCount(); j++)
	{
		Struct_ObjectPosition objectpos = m_arrayHighlight[HighlightBufferNo].GetAt(j);
		CObject* pobject = pDoc->m_listSchObject[objectpos.diagram].GetAt(objectpos.pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin)))
		{
			COb_schpin* ppin = (COb_schpin*)pobject;
			//if(ppin->m_bHide == TRUE) continue;

			num++;
		}
	}

	return num;
}


void CESDView::OnReportBm1()
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
	sprintf(filestring,"%s.BM1", filepath);
	CFile cfileBm1;
	if(cfileBm1.Open(filestring,  CFile::modeRead) != TRUE) 
	{
		AfxMessageBox("未找到元件清单文件.");
		return;
	}

	CDlg_Report dlg;
	CArchive arBm1(&cfileBm1, CArchive::load);
	for(;;)
	{
		CString cstring;
		if(arBm1.ReadString(cstring) == FALSE) break;

		dlg.m_arrayReport.Add(cstring);
	}

	dlg.m_TitleText = "元件清单BM1";

	dlg.DoModal();

	arBm1.Close();
	cfileBm1.Close();
}

void CESDView::OnReportBm2()
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
	sprintf(filestring,"%s.BM2", filepath);
	CFile cfileBm2;
	if(cfileBm2.Open(filestring,  CFile::modeRead) != TRUE) 
	{
		AfxMessageBox("未找到元件清单文件.");
		return;
	}

	CDlg_Report dlg;
	CArchive arBm2(&cfileBm2, CArchive::load);
	for(;;)
	{
		CString cstring;
		if(arBm2.ReadString(cstring) == FALSE) break;

		dlg.m_arrayReport.Add(cstring);
	}

	dlg.m_TitleText = "元件清单BM2";

	dlg.DoModal();

	arBm2.Close();
	cfileBm2.Close();
}

void CESDView::OnReportErr()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	char filepath[MAX_PATH];
	CString path = pDoc->GetPathName();
	int index = path.ReverseFind('.');
	if(index > 0) path = path.Left(index);

	strcpy(filepath,path);
	if( strcmp(filepath,"") == 0 )
	{
		AfxMessageBox("未找到出错报告文件.");
		return;
	}

	
	char filestring[MAX_PATH];
	sprintf(filestring,"%s.ERR", filepath);
	CFile cfileErr;
	if(cfileErr.Open(filestring,  CFile::modeRead) != TRUE) 
	{
		AfxMessageBox("未找到出错报告文件.");
		return;
	}

	CDlg_Report dlg;
	CArchive arErr(&cfileErr, CArchive::load);
	for(;;)
	{
		CString cstring;
		if(arErr.ReadString(cstring) == FALSE) break;

		dlg.m_arrayReport.Add(cstring);
	}

	dlg.m_TitleText = "出错报告";

	dlg.DoModal();

	arErr.Close();
	cfileErr.Close();
}

void CESDView::OnReportWir()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	char filepath[MAX_PATH];
	CString path = pDoc->GetPathName();
	int index = path.ReverseFind('.');
	if(index > 0) path = path.Left(index);

	strcpy(filepath,path);
	if( strcmp(filepath,"") == 0 )
	{
		AfxMessageBox("未找到连线表文件.");
		return;
	}

	
	char filestring[MAX_PATH];
	sprintf(filestring,"%s.WIR", filepath);
	CFile cfileWir;
	if(cfileWir.Open(filestring,  CFile::modeRead) != TRUE) 
	{
		AfxMessageBox("未找到连线表文件.");
		return;
	}

	CDlg_Report dlg;
	CArchive arWir(&cfileWir, CArchive::load);
	for(;;)
	{
		CString cstring;
		if(arWir.ReadString(cstring) == FALSE) break;

		dlg.m_arrayReport.Add(cstring);
	}

	dlg.m_TitleText = "连线表";

	dlg.DoModal();

	arWir.Close();
	cfileWir.Close();
}
void CESDView::OnReportNet()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	char filepath[MAX_PATH];
	CString path = pDoc->GetPathName();
	int index = path.ReverseFind('.');
	if(index > 0) path = path.Left(index);

	strcpy(filepath,path);
	if( strcmp(filepath,"") == 0 )
	{
		AfxMessageBox("未找到网络表文件.");
		return;
	}

	
	char filestring[MAX_PATH];
	sprintf(filestring,"%s.NET", filepath);
	CFile cfileWir;
	if(cfileWir.Open(filestring,  CFile::modeRead) != TRUE) 
	{
		AfxMessageBox("未找到网络表文件.");
		return;
	}

	CDlg_Report dlg;
	CArchive arWir(&cfileWir, CArchive::load);
	for(;;)
	{
		CString cstring;
		if(arWir.ReadString(cstring) == FALSE) break;

		dlg.m_arrayReport.Add(cstring);
	}

	dlg.m_TitleText = "网络表";

	dlg.DoModal();

	arWir.Close();
	cfileWir.Close();
}
