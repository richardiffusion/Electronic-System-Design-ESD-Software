void CESDView::OnEditFindText()
{
	Struct_Find structFind;

	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	CDlg_Edit_FindText dlg;
	dlg.m_csEditText = csEDA_FindText;
	dlg.m_bCheckCase = bEDA_FindCase;
	dlg.m_bCheckJump = bEDA_FindJump;
	dlg.m_bCheckWord = bEDA_FindWord;
	dlg.m_nComboScope = nEDA_FindScope;
	dlg.m_nComboObject = nEDA_FindObject;

	m_arrayFindText.RemoveAll();
	m_indexFindText = 0;

	if(dlg.DoModal() == IDOK)
	{
		bEDA_FindCase = dlg.m_bCheckCase;
		bEDA_FindJump = dlg.m_bCheckJump;
		bEDA_FindWord = dlg.m_bCheckWord;
		nEDA_FindScope = dlg.m_nComboScope;
		nEDA_FindObject = dlg.m_nComboObject;
		csEDA_FindText = dlg.m_csEditText;
		
		for(int diagram = 0; diagram < SCH_SHEET_NUM; diagram++)
		{
			
			if(nEDA_FindScope == 1) diagram = m_nDiagram;
		
			CObject* pobject;
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
					if(ptext->m_nFlag != FLAG_NORMAL)	continue;
					if((nEDA_FindObject == 2)||((nEDA_FindObject == 1)&&(ptext->m_nID != TEXT_NETLABEL))) continue;

					CString str1, str2;
					str1 = ptext->m_csText;
					str2 = dlg.m_csEditText;
					if(bEDA_FindCase == FALSE) { str1.MakeUpper(); str2.MakeUpper(); }
					
					
					if( ((bEDA_FindWord == TRUE)&&(str1 == str2)) || ((bEDA_FindWord == FALSE)&&(str1.Find(str2) >= 0)) )
					{
						structFind.nDiagram = diagram;
						structFind.pos = posbak;
						m_arrayFindText.Add(structFind);
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
				{
					
					COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
					if((psheetsymbol->m_nFlag != FLAG_NORMAL)||(nEDA_FindObject == 1))
					{
						for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);

						continue;
					}

					for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
					{
						POSITION posbak2 = pos;
						pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;
							if((ptext->m_nID != TEXT_SHEETSYMBOLDESIGNATOR)||(ptext->m_nID != TEXT_SHEETSYMBOLFILENAME)) continue;
							
							CString str1, str2;
							str1 = ptext->m_csText;
							str2 = dlg.m_csEditText;
							if(bEDA_FindCase == FALSE) { str1.MakeUpper(); str2.MakeUpper(); }
							
							
							if( ((bEDA_FindWord == TRUE)&&(str1 == str2)) || ((bEDA_FindWord == FALSE)&&(str1.Find(str2) >= 0)) )
							{
								structFind.nDiagram = diagram;
								structFind.pos = posbak2;
								m_arrayFindText.Add(structFind);
							}
						}
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
				{
					
					COb_schport* pport = (COb_schport*)pobject;
					if((pport->m_nFlag != FLAG_NORMAL)||(nEDA_FindObject == 2))	continue;

					CString str1, str2;
					str1 = pport->m_csName;
					str2 = dlg.m_csEditText;
					if(bEDA_FindCase == FALSE) { str1.MakeUpper(); str2.MakeUpper(); }

					
					if( ((bEDA_FindWord == TRUE)&&(str1 == str2)) || ((bEDA_FindWord == FALSE)&&(str1.Find(str2) >= 0)) )
					{
						structFind.nDiagram = diagram;
						structFind.pos = posbak;
						m_arrayFindText.Add(structFind);
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
				{
					
					COb_schpower* ppower = (COb_schpower*)pobject;
					if((ppower->m_nFlag != FLAG_NORMAL)||(nEDA_FindObject == 2)) continue;
					
					CString str1, str2;
					str1 = ppower->m_csNetName;
					str2 = dlg.m_csEditText;
					if(bEDA_FindCase == FALSE) { str1.MakeUpper(); str2.MakeUpper(); }

					
					if( ((bEDA_FindWord == TRUE)&&(str1 == str2)) || ((bEDA_FindWord == FALSE)&&(str1.Find(str2) >= 0)) )
					{
						structFind.nDiagram = diagram;
						structFind.pos = posbak;
						m_arrayFindText.Add(structFind);
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
				{
					
					COb_schcomp* pcomp;
					pcomp = (COb_schcomp*)pobject;
					if((pcomp->m_nFlag != FLAG_NORMAL)||(nEDA_FindObject == 1))
					{
						for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);

						continue;
					}

					for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
					{
						POSITION posbak2 = pos;
						pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;
							
							if((ptext->m_nID != TEXT_COMPDESIGNATOR)&&(ptext->m_nID != TEXT_COMPCOMMENT)) continue;
							if((ptext->m_nID != TEXT_COMPDESIGNATOR)&&(nEDA_FindObject == 2)) continue;
							
							CString str1, str2;
							str1 = ptext->m_csText;
							str2 = dlg.m_csEditText;
							if(bEDA_FindCase == FALSE) { str1.MakeUpper(); str2.MakeUpper(); }
							
							
							if( ((bEDA_FindWord == TRUE)&&(str1 == str2)) || ((bEDA_FindWord == FALSE)&&(str1.Find(str2) >= 0)) )
							{
								structFind.nDiagram = diagram;
								structFind.pos = posbak2;
								m_arrayFindText.Add(structFind);
							}
						}
					}
				}
			}

			if(nEDA_FindScope == 1) break;
		}
		
		if(m_arrayFindText.GetCount() > 0)
		{
			structFind = m_arrayFindText.GetAt(m_indexFindText);
			m_indexFindText++;

			CObject* pobject;
			pobject = pDoc->m_listSchObject[structFind.nDiagram].GetAt(structFind.pos);
			if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
			{
				COb_schtext* ptext = (COb_schtext*)pobject;
				if(bEDA_FindJump == TRUE) Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, ptext->m_nDiagram);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
			{
				COb_schport* pport = (COb_schport*)pobject;
				if(bEDA_FindJump == TRUE) Change_Viewpoint(pport->m_fX, pport->m_fY, pport->m_nDiagram);
			}
			else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
			{
				COb_schpower* ppower = (COb_schpower*)pobject;
				if(bEDA_FindJump == TRUE) Change_Viewpoint(ppower->m_fX, ppower->m_fY, ppower->m_nDiagram);
			}
		}
		else
		{
			CString str;
			str.Format("%s 未找到.", dlg.m_csEditText);
			AfxMessageBox(str);
		}
	}
}

void CESDView::OnEditReplaceText()
{
	Struct_Find structFind;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	CDlg_Edit_ReplaceText dlg;
	dlg.m_csEditText = csEDA_FindText;
	dlg.m_csEditReplaceText = csEDA_ReplaceText;
	dlg.m_bCheckWord = bEDA_FindWord;
	dlg.m_nComboScope = nEDA_FindScope;
	dlg.m_nComboObject = nEDA_FindObject;
	dlg.m_bCheckPrompt = bEDA_FindPrompt;

	m_arrayFindText.RemoveAll();

	if(dlg.DoModal() == IDOK)
	{
		bEDA_FindWord = dlg.m_bCheckWord;
		nEDA_FindScope = dlg.m_nComboScope;
		nEDA_FindObject = dlg.m_nComboObject;
		bEDA_FindPrompt = dlg.m_bCheckPrompt;
		csEDA_FindText = dlg.m_csEditText;
		csEDA_ReplaceText = dlg.m_csEditReplaceText;

		BOOL bCaseSensitive = dlg.m_bCheckCase;

		for(int diagram = 0; diagram < SCH_SHEET_NUM; diagram++)
		{
			
			if(nEDA_FindScope == 0) diagram = m_nDiagram;
		
			CObject* pobject;
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
					if(ptext->m_nFlag != FLAG_NORMAL)	continue;
					if((nEDA_FindObject == 2)||((nEDA_FindObject == 1)&&(ptext->m_nID != TEXT_NETLABEL))) continue;

					CString str1, str2;
					str1 = ptext->m_csText;
					str2 = dlg.m_csEditText;
					if(bCaseSensitive == FALSE) { str1.MakeUpper(); str2.MakeUpper(); }
					
					
					if( ((bEDA_FindWord == TRUE)&&(str1 == str2)) || ((bEDA_FindWord == FALSE)&&(str1.Find(str2) >= 0)) )
					{
						structFind.nDiagram = diagram;
						structFind.pos = posbak;
						m_arrayFindText.Add(structFind);
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
				{
					
					COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
					if((psheetsymbol->m_nFlag != FLAG_NORMAL)||(nEDA_FindObject == 1))
					{
						for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);

						continue;
					}

					for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++)
					{
						POSITION posbak2 = pos;
						pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;
							if((ptext->m_nID != TEXT_SHEETSYMBOLDESIGNATOR)||(ptext->m_nID != TEXT_SHEETSYMBOLFILENAME)) continue;
							
							CString str1, str2;
							str1 = ptext->m_csText;
							str2 = dlg.m_csEditText;
							if(bCaseSensitive == FALSE) { str1.MakeUpper(); str2.MakeUpper(); }
							
							
							if( ((bEDA_FindWord == TRUE)&&(str1 == str2)) || ((bEDA_FindWord == FALSE)&&(str1.Find(str2) >= 0)) )
							{
								structFind.nDiagram = diagram;
								structFind.pos = posbak2;
								m_arrayFindText.Add(structFind);
							}
						}
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
				{
					
					COb_schport* pport = (COb_schport*)pobject;
					if((pport->m_nFlag != FLAG_NORMAL)||(nEDA_FindObject == 2))	continue;

					CString str1, str2;
					str1 = pport->m_csName;
					str2 = dlg.m_csEditText;
					if(bCaseSensitive == FALSE) { str1.MakeUpper(); str2.MakeUpper(); }

					
					if( ((bEDA_FindWord == TRUE)&&(str1 == str2)) || ((bEDA_FindWord == FALSE)&&(str1.Find(str2) >= 0)) )
					{
						structFind.nDiagram = diagram;
						structFind.pos = posbak;
						m_arrayFindText.Add(structFind);
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
				{
					
					COb_schpower* ppower = (COb_schpower*)pobject;
					if((ppower->m_nFlag != FLAG_NORMAL)||(nEDA_FindObject == 2)) continue;
					
					CString str1, str2;
					str1 = ppower->m_csNetName;
					str2 = dlg.m_csEditText;
					if(bCaseSensitive == FALSE) { str1.MakeUpper(); str2.MakeUpper(); }

					
					if( ((bEDA_FindWord == TRUE)&&(str1 == str2)) || ((bEDA_FindWord == FALSE)&&(str1.Find(str2) >= 0)) )
					{
						structFind.nDiagram = diagram;
						structFind.pos = posbak;
						m_arrayFindText.Add(structFind);
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
				{
					
					COb_schcomp* pcomp;
					pcomp = (COb_schcomp*)pobject;
					if((pcomp->m_nFlag != FLAG_NORMAL)||(nEDA_FindObject == 1))
					{
						for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);

						continue;
					}

					for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
					{
						POSITION posbak2 = pos;
						pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;
							
							if((ptext->m_nID != TEXT_COMPDESIGNATOR)&&(ptext->m_nID != TEXT_COMPCOMMENT)) continue;
							if((ptext->m_nID != TEXT_COMPDESIGNATOR)&&(nEDA_FindObject == 2)) continue;
							
							CString str1, str2;
							str1 = ptext->m_csText;
							str2 = dlg.m_csEditText;
							if(bCaseSensitive == FALSE) { str1.MakeUpper(); str2.MakeUpper(); }
							
							
							if( ((bEDA_FindWord == TRUE)&&(str1 == str2)) || ((bEDA_FindWord == FALSE)&&(str1.Find(str2) >= 0)) )
							{
								structFind.nDiagram = diagram;
								structFind.pos = posbak2;
								m_arrayFindText.Add(structFind);
							}
						}
					}
				}
			}

			if(nEDA_FindScope == 0) break;
		}

		m_indexFindText = 0;
		if(m_arrayFindText.GetCount() > 0)
		{
			for(;;)
			{
				if(m_indexFindText >= m_arrayFindText.GetCount()) break;
				structFind = m_arrayFindText.GetAt(m_indexFindText);
				m_indexFindText++;

				CObject* pobject;
				pobject = pDoc->m_listSchObject[structFind.nDiagram].GetAt(structFind.pos);
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
				{
					COb_schtext* ptext = (COb_schtext*)pobject;
					if(bEDA_FindPrompt == TRUE)
					{
						Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, ptext->m_nDiagram);
						CString cstring, worningtext;
						cstring = ptext->m_csText;
						cstring.Replace(dlg.m_csEditText, dlg.m_csEditReplaceText);
						worningtext.Format("%s 替换为 %s, 确认替换?", ptext->m_csText, cstring);
						if(AfxMessageBox(worningtext, MB_YESNO|MB_ICONWARNING) != IDYES) continue;
						
						
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;

						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_TEXT;
						pundo->m_nDiagram = structFind.nDiagram;
						pundo->m_nPos = structFind.pos;
						pundo->schtext.Copy(ptext);

						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
						ptext->m_csText.Replace(dlg.m_csEditText, dlg.m_csEditReplaceText);
					}
					else
					{
						
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;

						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_TEXT;
						pundo->m_nDiagram = structFind.nDiagram;
						pundo->m_nPos = structFind.pos;
						pundo->schtext.Copy(ptext);

						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
						ptext->m_csText.Replace(dlg.m_csEditText, dlg.m_csEditReplaceText);
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
				{
					COb_schport* pport = (COb_schport*)pobject;
					if(bEDA_FindPrompt == TRUE)
					{
						Change_Viewpoint(pport->m_fX, pport->m_fY, pport->m_nDiagram);
						CString cstring, worningtext;
						cstring = pport->m_csName;
						cstring.Replace(dlg.m_csEditText, dlg.m_csEditReplaceText);
						worningtext.Format("%s 替换为 %s, 确认替换?", pport->m_csName, cstring);
						if(AfxMessageBox(worningtext, MB_YESNO|MB_ICONWARNING) != IDYES) continue;
						
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_PORT;
						pundo->m_nDiagram = structFind.nDiagram;
						pundo->m_nPos = structFind.pos;
						pundo->schport.Copy(pport);
							
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
						pport->m_csName.Replace(dlg.m_csEditText, dlg.m_csEditReplaceText);
					}
					else
					{
						
						pport->m_csName.Replace(dlg.m_csEditText, dlg.m_csEditReplaceText);
						
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_PORT;
						pundo->m_nDiagram = structFind.nDiagram;
						pundo->m_nPos = structFind.pos;
						pundo->schport.Copy(pport);
							
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
				{
					COb_schpower* ppower = (COb_schpower*)pobject;
					if(bEDA_FindPrompt == TRUE)
					{
						Change_Viewpoint(ppower->m_fX, ppower->m_fY, ppower->m_nDiagram);
						CString cstring, worningtext;
						cstring = ppower->m_csNetName;
						cstring.Replace(dlg.m_csEditText, dlg.m_csEditReplaceText);
						worningtext.Format("%s 替换为 %s, 确认替换?", ppower->m_csNetName, cstring);
						if(AfxMessageBox(worningtext, MB_YESNO|MB_ICONWARNING) != IDYES) continue;
						
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_POWER;
						pundo->m_nDiagram = structFind.nDiagram;
						pundo->m_nPos = structFind.pos;
						pundo->schpower.Copy(ppower);
							
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
						ppower->m_csNetName.Replace(dlg.m_csEditText, dlg.m_csEditReplaceText);
					}
					else
					{
						
						if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
						{
							pDoc->Clear_Discarded_Undo_Buffer();
						}
						clear_undo_buffer_flag = TRUE;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_POWER;
						pundo->m_nDiagram = structFind.nDiagram;
						pundo->m_nPos = structFind.pos;
						pundo->schpower.Copy(ppower);
							
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
						ppower->m_csNetName.Replace(dlg.m_csEditText, dlg.m_csEditReplaceText);
					}				
				}
			}
		}
		else
		{
			CString str;
			str.Format("%s 未找到.", dlg.m_csEditText);
			AfxMessageBox(str);
		}
		
		
		if(undo_flag == TRUE)
		{
			pDoc->m_nSCH_Undo_Num++;
			if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
		}
	}
}

void CESDView::OnEditFindNext()
{
	Struct_Find structFind;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if(m_indexFindText < m_arrayFindText.GetCount())
	{
		structFind = m_arrayFindText.GetAt(m_indexFindText);
		m_indexFindText++;

		CObject* pobject;
		pobject = pDoc->m_listSchObject[structFind.nDiagram].GetAt(structFind.pos);
		if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext = (COb_schtext*)pobject;
			if(bEDA_FindJump == TRUE) Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, ptext->m_nDiagram);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport = (COb_schport*)pobject;
			if(bEDA_FindJump == TRUE) Change_Viewpoint(pport->m_fX, pport->m_fY, pport->m_nDiagram);
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpower )))
		{
			COb_schpower* ppower = (COb_schpower*)pobject;
			if(bEDA_FindJump == TRUE) Change_Viewpoint(ppower->m_fX, ppower->m_fY, ppower->m_nDiagram);
		}
	}
}

void CESDView::OnUpdateEditFindNext(CCmdUI *pCmdUI)
{
	if(m_indexFindText < m_arrayFindText.GetCount()) pCmdUI->Enable(TRUE);
	else pCmdUI->Enable(FALSE);
}

void CESDView::OnEditAlignLeft()
{
	double x0, y0, deltax, deltay;
	int rotation = 0;
	BOOL mirror = FALSE;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	POSITION posbak;
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

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
			if( pline->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_LINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = pline->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pline->m_cVertex[0].fx;
				y0 = pline->m_cVertex[0].fy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;
				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pline->m_cVertex[k].fx - x0, pline->m_cVertex[k].fy - y0, rotation, mirror);
					pline->m_cVertex[k].fx = fx + x0 + deltax;
					pline->m_cVertex[k].fy = fy + y0 + deltay;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			if(ppolyline->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POLYLINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = ppolyline->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppolyline->m_cVertex[0].fx;
				y0 = ppolyline->m_cVertex[0].fy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;
				for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ppolyline->m_cVertex[k].fx - x0, ppolyline->m_cVertex[k].fy - y0, rotation, mirror);
					ppolyline->m_cVertex[k].fx = fx + x0 + deltax;
					ppolyline->m_cVertex[k].fy = fy + y0 + deltay;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_JUNCTION;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = pjunc->m_fCx;
				pundo->m_fY1 = pjunc->m_fCy;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pjunc->m_fCx;
				y0 = pjunc->m_fCy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pjunc->m_fCx - x0, pjunc->m_fCy - y0, rotation, mirror);
				pjunc->m_fCx = fx + x0 + deltax;
				pjunc->m_fCy = fy + y0 + deltay;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if(ptext->m_bSelection == TRUE)
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
				pundo->m_nPos = posbak;
				pundo->schtext.Copy(ptext);
				pundo->schtext.m_nFlag = FLAG_NORMAL;

				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ptext->m_fSx;
				y0 = ptext->m_fSy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;
				double fx, fy;					
				pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - x0, ptext->m_fSy - y0, rotation, mirror);
				ptext->m_fSx = fx + x0 + deltax;
				ptext->m_fSy = fy + y0 + deltay;

				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ptext->m_nOrientation, rotation, FALSE); //mirror);
				ptext->m_nOrientation = orientation;
				ptext->m_nOrientation = pDoc->Get_Correct_Orientation(ptext->m_nOrientation);
		
				if(mirror == TRUE)
				{
					if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
					else ptext->m_bMirror = TRUE;
					if((ptext->m_nOrientation == 0)||(ptext->m_nOrientation == 180))
					{
						if(ptext->m_nH_Justification == 0) ptext->m_nH_Justification = 2;
						else if(ptext->m_nH_Justification == 2) ptext->m_nH_Justification = 0;
					}
					else
					{
						if(ptext->m_nV_Justification == 0) ptext->m_nV_Justification = 2;
						else if(ptext->m_nV_Justification == 2) ptext->m_nV_Justification = 0;
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if(parc->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ARC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->scharc.Copy(parc);
				pundo->scharc.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = parc->m_fCx;
				y0 = parc->m_fCy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, parc->m_fCx - x0, parc->m_fCy - y0, rotation, mirror);
				parc->m_fCx = fx + x0 + deltax;
				parc->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, parc->m_fSangle, parc->m_fEangle, rotation, mirror);
				parc->m_fSangle = salpha;
				parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
				parc->m_fEangle = ealpha;
				parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if(pellipse->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schellipse.Copy(pellipse);
				pundo->schellipse.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pellipse->m_fCx;
				y0 = pellipse->m_fCy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pellipse->m_fCx - x0, pellipse->m_fCy - y0, rotation, mirror);
				pellipse->m_fCx = fx + x0 + deltax;
				pellipse->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, pellipse->m_fSangle, pellipse->m_fEangle, rotation, mirror);
				pellipse->m_fSangle = salpha;
				pellipse->m_fSangle = pDoc->Get_Correct_Angle(pellipse->m_fSangle);
				pellipse->m_fEangle = ealpha;
				pellipse->m_fEangle = pDoc->Get_Correct_Angle(pellipse->m_fEangle);
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = pellipse->m_fRadius_y;
					pellipse->m_fRadius_y = pellipse->m_fRadius_x;
					pellipse->m_fRadius_x = ftemp;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if(pellipsefill->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schellipsefill.Copy(pellipsefill);
				pundo->schellipsefill.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pellipsefill->m_fCx;
				y0 = pellipsefill->m_fCy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pellipsefill->m_fCx - x0, pellipsefill->m_fCy - y0, rotation, mirror);
				pellipsefill->m_fCx = fx + x0 + deltax;
				pellipsefill->m_fCy = fy + y0 + deltay;
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = pellipsefill->m_fRadius_y;
					pellipsefill->m_fRadius_y = pellipsefill->m_fRadius_x;
					pellipsefill->m_fRadius_x = ftemp;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if(ppie->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_PIE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schpie.Copy(ppie);
				pundo->schpie.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppie->m_fCx;
				y0 = ppie->m_fCy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppie->m_fCx - x0, ppie->m_fCy - y0, rotation, mirror);
				ppie->m_fCx = fx + x0 + deltax;
				ppie->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, ppie->m_fSangle, ppie->m_fEangle, rotation, mirror);
				ppie->m_fSangle = salpha;
				ppie->m_fSangle = pDoc->Get_Correct_Angle(ppie->m_fSangle);
				ppie->m_fEangle = ealpha;
				ppie->m_fEangle = pDoc->Get_Correct_Angle(ppie->m_fEangle);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if(ppolygon->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POLYGON;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = ppolygon->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppolygon->m_cVertex[0].fx;
				y0 = ppolygon->m_cVertex[0].fy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;

				for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ppolygon->m_cVertex[k].fx - x0, ppolygon->m_cVertex[k].fy - y0, rotation, mirror);
					ppolygon->m_cVertex[k].fx = fx + x0 + deltax;
					ppolygon->m_cVertex[k].fy = fy + y0 + deltay;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = ptextframe->m_fX1;
				pundo->m_fY1 = ptextframe->m_fY1;
				pundo->m_fX2 = ptextframe->m_fX2;
				pundo->m_fY2 = ptextframe->m_fY2;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ptextframe->m_fX1;
				y0 = ptextframe->m_fY1;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;

				double fx, fy, dx, dy;
				dx = ptextframe->m_fX2 - ptextframe->m_fX1;
				dy = ptextframe->m_fY2 - ptextframe->m_fY1;
				pDoc->Get_Rotate_XY(&fx, &fy, ptextframe->m_fX1 - x0, ptextframe->m_fY1 - y0, rotation, mirror);
				ptextframe->m_fX1 = fx + x0 + deltax;
				ptextframe->m_fY1 = fy + y0 + deltay;
				ptextframe->m_fX2 = ptextframe->m_fX1 + dx;
				ptextframe->m_fY2 = ptextframe->m_fY1 + dy;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if(prect->m_bSelection == TRUE)
			{

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_RECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = prect->m_fX1;
				pundo->m_fY1 = prect->m_fY1;
				pundo->m_fX2 = prect->m_fX2;
				pundo->m_fY2 = prect->m_fY2;
			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = prect->m_fX1;
				y0 = prect->m_fY1;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, prect->m_fX1 - x0, prect->m_fY1 - y0, rotation, mirror);
				prect->m_fX1 = fx + x0 + deltax;
				prect->m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, prect->m_fX2 - x0, prect->m_fY2 - y0, rotation, mirror);
				prect->m_fX2 = fx + x0 + deltax;
				prect->m_fY2 = fy + y0 + deltay;

				double fminx, fminy, fmaxx, fmaxy;
				fminx = MIN(prect->m_fX1, prect->m_fX2);
				fmaxx = MAX(prect->m_fX1, prect->m_fX2);
				fminy = MIN(prect->m_fY1, prect->m_fY2);
				fmaxy = MAX(prect->m_fY1, prect->m_fY2);
				prect->m_fX1 = fminx;
				prect->m_fY1 = fminy;
				prect->m_fX2 = fmaxx;
				prect->m_fY2 = fmaxy;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if(proundrect->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = proundrect->m_fX1;
				pundo->m_fY1 = proundrect->m_fY1;
				pundo->m_fX2 = proundrect->m_fX2;
				pundo->m_fY2 = proundrect->m_fY2;
				pundo->m_fRx = proundrect->m_fRadius_x;
				pundo->m_fRy = proundrect->m_fRadius_y;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = proundrect->m_fX1;
				y0 = proundrect->m_fY1;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, proundrect->m_fX1 - x0, proundrect->m_fY1 - y0, rotation, mirror);
				proundrect->m_fX1 = fx + x0 + deltax;
				proundrect->m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, proundrect->m_fX2 - x0, proundrect->m_fY2 - y0, rotation, mirror);
				proundrect->m_fX2 = fx + x0 + deltax;
				proundrect->m_fY2 = fy + y0 + deltay;
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = proundrect->m_fRadius_y;
					proundrect->m_fRadius_y = proundrect->m_fRadius_x;
					proundrect->m_fRadius_x = ftemp;
				}

				double fminx, fminy, fmaxx, fmaxy;
				fminx = MIN(proundrect->m_fX1, proundrect->m_fX2);
				fmaxx = MAX(proundrect->m_fX1, proundrect->m_fX2);
				fminy = MIN(proundrect->m_fY1, proundrect->m_fY2);
				fmaxy = MAX(proundrect->m_fY1, proundrect->m_fY2);
				proundrect->m_fX1 = fminx;
				proundrect->m_fY1 = fminy;
				proundrect->m_fX2 = fmaxx;
				proundrect->m_fY2 = fmaxy;
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
			
			double dx, dy;
			if(psheetsymbol->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = psheetsymbol->m_fX;
				pundo->m_fY1 = psheetsymbol->m_fY;
				pundo->m_fX2 = psheetsymbol->m_fXsize;
				pundo->m_fY2 = psheetsymbol->m_fYsize;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = psheetsymbol->m_fX;
				y0 = psheetsymbol->m_fY;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;

				double fx, fy, fxbak, fybak;
				fxbak = psheetsymbol->m_fX;
				fybak = psheetsymbol->m_fY;

				pDoc->Get_Rotate_XY(&fx, &fy, psheetsymbol->m_fX - x0, psheetsymbol->m_fY - y0, rotation, mirror);
				psheetsymbol->m_fX = fx + x0 + deltax;
				psheetsymbol->m_fY = fy + y0 + deltay;

				dx = psheetsymbol->m_fX - fxbak;
				dy = psheetsymbol->m_fY - fybak;

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) 
				{
					posbak = pos;
					pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext;
						ptext = (COb_schtext*)pobject;

						
						pundo = new CSchUndo;
						pundo->m_nOp = UNDO_MOVE;
						pundo->m_nOb = SCH_ELEMENT_TEXT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = posbak;
						pundo->m_fX1 = ptext->m_fSx;
						pundo->m_fY1 = ptext->m_fSy;
						pundo->m_nOrientation = ptext->m_nOrientation;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
						ptext->m_fSx += dx;
						ptext->m_fSy += dy;
					}
				}
			}
			else 
			{
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(pport->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_PORT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schport.Copy(pport);
				pundo->schport.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pport->m_fX;
				y0 = pport->m_fY;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pport->m_fX - x0, pport->m_fY - y0, rotation, mirror);
				pport->m_fX = fx + x0 + deltax;
				pport->m_fY = fy + y0 + deltay;
				if(rotation == 90)
				{
					if(pport->m_nStyle == PORT_STYLE_NH) pport->m_nStyle = PORT_STYLE_NV;
					else if(pport->m_nStyle == PORT_STYLE_NV) pport->m_nStyle = PORT_STYLE_NH;
					else if(pport->m_nStyle == PORT_STYLE_LEFTRIGHT) pport->m_nStyle = PORT_STYLE_TOPBOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOPBOTTOM) pport->m_nStyle = PORT_STYLE_LEFTRIGHT;
					else if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_BOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_LEFT;
				}
				else if(rotation == 180)
				{
					if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_LEFT;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_BOTTOM;
				}
				else if(rotation == 270)
				{
					if(pport->m_nStyle == PORT_STYLE_NH) pport->m_nStyle = PORT_STYLE_NV;
					else if(pport->m_nStyle == PORT_STYLE_NV) pport->m_nStyle = PORT_STYLE_NH;
					else if(pport->m_nStyle == PORT_STYLE_LEFTRIGHT) pport->m_nStyle = PORT_STYLE_TOPBOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOPBOTTOM) pport->m_nStyle = PORT_STYLE_LEFTRIGHT;
					else if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_LEFT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_BOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_RIGHT;
				}

				if(mirror == TRUE)
				{
					if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_LEFT;
				}

				if(pport->m_nStyle < PORT_STYLE_NV)
				{
					pport->m_fX -= pport->m_fSize;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POWER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = ppower->m_fX;
				pundo->m_fY1 = ppower->m_fY;
				pundo->m_nOrientation = ppower->m_nOrientation;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppower->m_fX;
				y0 = ppower->m_fY;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppower->m_fX - x0, ppower->m_fY - y0, rotation, mirror);
				ppower->m_fX = fx + x0 + deltax;
				ppower->m_fY = fy + y0 + deltay;
				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ppower->m_nOrientation, rotation, mirror);
				ppower->m_nOrientation = orientation;
				ppower->m_nOrientation = pDoc->Get_Correct_Orientation(ppower->m_nOrientation);

				//ppower->m_nOrientation += rotation;
				//ppower->m_nOrientation = pDoc->Get_Correct_Orientation(ppower->m_nOrientation);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if(pbezier->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_BEZIER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = pbezier->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pbezier->m_cVertex[0].fx;
				y0 = pbezier->m_cVertex[0].fy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;

				for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pbezier->m_cVertex[k].fx - x0, pbezier->m_cVertex[k].fy - y0, rotation, mirror);
					pbezier->m_cVertex[k].fx = fx + x0 + deltax;
					pbezier->m_cVertex[k].fy = fy + y0 + deltay;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = pgraphic->m_fX1;
				pundo->m_fY1 = pgraphic->m_fY1;
				pundo->m_fX2 = pgraphic->m_fX2;
				pundo->m_fY2 = pgraphic->m_fY2;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pgraphic->m_fX1;
				y0 = pgraphic->m_fY1;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;

				double fx, fy, dx, dy;
				dx = pgraphic->m_fX2 - pgraphic->m_fX1;
				dy = pgraphic->m_fY2 - pgraphic->m_fY1;
				pDoc->Get_Rotate_XY(&fx, &fy, pgraphic->m_fX1 - x0, pgraphic->m_fY1 - y0, rotation, mirror);
				pgraphic->m_fX1 = fx + x0 + deltax;
				pgraphic->m_fY1 = fy + y0 + deltay;
				pgraphic->m_fX2 = pgraphic->m_fX1 + dx;
				pgraphic->m_fY2 = pgraphic->m_fY1 + dy;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_COMPONENT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schcomp.Copy(pcomp);
				pundo->schcomp.m_nFlag = FLAG_NORMAL;
						
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pcomp->m_fX;
				y0 = pcomp->m_fY;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] - x0;
				deltay = 0;

				double fx1, fy1;
				pDoc->Get_Rotate_XY(&fx1, &fy1, pcomp->m_fX - x0, pcomp->m_fY - y0, rotation, mirror);
				pcomp->m_fX = fx1 + x0 + deltax;
				pcomp->m_fY = fy1 + y0 + deltay;
				pcomp->m_nOrientation += rotation;
				pcomp->m_nOrientation = pDoc->Get_Correct_Orientation(pcomp->m_nOrientation);
				if(mirror == TRUE)
				{
					if(pcomp->m_bMirrored == TRUE) pcomp->m_bMirrored = FALSE;
					else pcomp->m_bMirrored = TRUE;
				}
				
				for(int i=0; i<pcomp->m_arrayPos.GetSize(); i++)
				{
					CObject* pobject;
					POSITION subpos;
					subpos = pcomp->m_arrayPos.GetAt(i);
					pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
					{
						COb_schpin* ppin = (COb_schpin*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_PIN;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpin.Copy(ppin);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						

						pDoc->Get_Rotate_XY(&fx1, &fy1, ppin->m_fX - x0, ppin->m_fY - y0, rotation, mirror);
						ppin->m_fX = fx1 + x0 + deltax;
						ppin->m_fY = fy1 + y0 + deltay;

						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, ppin->m_nOrientation, rotation, mirror);
						ppin->m_nOrientation = orientation;
						ppin->m_nOrientation = pDoc->Get_Correct_Orientation(ppin->m_nOrientation);					
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
					{
						COb_schieee* pieee = (COb_schieee*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_IEEE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schieee.Copy(pieee);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pieee->m_fX - x0, pieee->m_fY - y0, rotation, mirror);
						pieee->m_fX = fx1 + x0 + deltax;
						pieee->m_fY = fy1 + y0 + deltay;
						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, pieee->m_nOrientation, rotation, mirror);
						pieee->m_nOrientation = orientation;
						pieee->m_nOrientation = pDoc->Get_Correct_Orientation(pieee->m_nOrientation);
	
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_TEXT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schtext.Copy(ptext);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, ptext->m_fSx - x0, ptext->m_fSy - y0, rotation, mirror);
						ptext->m_fSx = fx1 + x0 + deltax;
						ptext->m_fSy = fy1 + y0 + deltay;
						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, ptext->m_nOrientation, rotation, FALSE); //mirror);
						ptext->m_nOrientation = orientation;
						ptext->m_nOrientation = pDoc->Get_Correct_Orientation(ptext->m_nOrientation);
			
						if(mirror == TRUE)
						{
							if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
							else ptext->m_bMirror = TRUE;
							if((ptext->m_nOrientation == 0)||(ptext->m_nOrientation == 180))
							{
								if(ptext->m_nH_Justification == 0) ptext->m_nH_Justification = 2;
								else if(ptext->m_nH_Justification == 2) ptext->m_nH_Justification = 0;
							}
							else
							{
								if(ptext->m_nV_Justification == 0) ptext->m_nV_Justification = 2;
								else if(ptext->m_nV_Justification == 2) ptext->m_nV_Justification = 0;
							}
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
					{
						COb_schbezier* pbezier = (COb_schbezier*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_BEZIER;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schbezier.Copy(pbezier);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, pbezier->m_cVertex[k].fx-x0, pbezier->m_cVertex[k].fy-y0, rotation, mirror);
							pbezier->m_cVertex[k].fx = fx1 + x0 + deltax;
							pbezier->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
					{
						COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_POLYLINE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpolyline.Copy(ppolyline);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, ppolyline->m_cVertex[k].fx-x0, ppolyline->m_cVertex[k].fy-y0, rotation, mirror);
							ppolyline->m_cVertex[k].fx = fx1 + x0 + deltax;
							ppolyline->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
					{
						COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_POLYGON;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpolygon.Copy(ppolygon);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, ppolygon->m_cVertex[k].fx-x0, ppolygon->m_cVertex[k].fy-y0, rotation, mirror);
							ppolygon->m_cVertex[k].fx = fx1 + x0 + deltax;
							ppolygon->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
					{
						COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schellipsefill.Copy(pellipsefill);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pellipsefill->m_fCx - x0, pellipsefill->m_fCy - y0, rotation, mirror);
						pellipsefill->m_fCx = fx1 + x0 + deltax;
						pellipsefill->m_fCy = fy1 + y0 + deltay;
						if((rotation == 90)||(rotation == 270))
						{
							double ftemp;
							ftemp = pellipsefill->m_fRadius_y;
							pellipsefill->m_fRadius_y = pellipsefill->m_fRadius_x;
							pellipsefill->m_fRadius_x = ftemp;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
					{
						COb_schpie* ppie = (COb_schpie*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_PIE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpie.Copy(ppie);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, ppie->m_fCx - x0, ppie->m_fCy - y0, rotation, mirror);
						ppie->m_fCx = fx1 + x0 + deltax;
						ppie->m_fCy = fy1 + y0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, ppie->m_fSangle, ppie->m_fEangle, rotation, mirror);
						ppie->m_fSangle = salpha;
						ppie->m_fSangle = pDoc->Get_Correct_Angle(ppie->m_fSangle);
						ppie->m_fEangle = ealpha;
						ppie->m_fEangle = pDoc->Get_Correct_Angle(ppie->m_fEangle);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
					{
						COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schroundrect.Copy(proundrect);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX1 - x0, proundrect->m_fY1 - y0, rotation, mirror);
						proundrect->m_fX1 = fx1 + x0 + deltax;
						proundrect->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX2 - x0, proundrect->m_fY2 - y0, rotation, mirror);
						proundrect->m_fX2 = fx1 + x0 + deltax;
						proundrect->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(proundrect->m_fX1, proundrect->m_fX2);
						fmaxx = MAX(proundrect->m_fX1, proundrect->m_fX2);
						fminy = MIN(proundrect->m_fY1, proundrect->m_fY2);
						fmaxy = MAX(proundrect->m_fY1, proundrect->m_fY2);
						proundrect->m_fX1 = fminx;
						proundrect->m_fY1 = fminy;
						proundrect->m_fX2 = fmaxx;
						proundrect->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
					{
						COb_schellipse* pellipse = (COb_schellipse*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schellipse.Copy(pellipse);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pellipse->m_fCx - x0, pellipse->m_fCy - y0, rotation, mirror);
						pellipse->m_fCx = fx1 + x0 + deltax;
						pellipse->m_fCy = fy1 + y0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, pellipse->m_fSangle, pellipse->m_fEangle, rotation, mirror);
						pellipse->m_fSangle = salpha;
						pellipse->m_fSangle = pDoc->Get_Correct_Angle(pellipse->m_fSangle);
						pellipse->m_fEangle = ealpha;
						pellipse->m_fEangle = pDoc->Get_Correct_Angle(pellipse->m_fEangle);
						if((rotation == 90)||(rotation == 270))
						{
							double ftemp;
							ftemp = pellipse->m_fRadius_y;
							pellipse->m_fRadius_y = pellipse->m_fRadius_x;
							pellipse->m_fRadius_x = ftemp;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
					{
						COb_scharc* parc = (COb_scharc*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ARC;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->scharc.Copy(parc);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, parc->m_fCx - x0, parc->m_fCy - y0, rotation, mirror);
						parc->m_fCx = fx1 + x0 + deltax;
						parc->m_fCy = fy1 + y0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, parc->m_fSangle, parc->m_fEangle, rotation, mirror);
						parc->m_fSangle = salpha;
						parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
						parc->m_fEangle = ealpha;
						parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
					{
						COb_schrect* prect = (COb_schrect*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_RECT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schrect.Copy(prect);
										
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX1 - x0, prect->m_fY1 - y0, rotation, mirror);
						prect->m_fX1 = fx1 + x0 + deltax;
						prect->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX2 - x0, prect->m_fY2 - y0, rotation, mirror);
						prect->m_fX2 = fx1 + x0 + deltax;
						prect->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(prect->m_fX1, prect->m_fX2);
						fmaxx = MAX(prect->m_fX1, prect->m_fX2);
						fminy = MIN(prect->m_fY1, prect->m_fY2);
						fmaxy = MAX(prect->m_fY1, prect->m_fY2);
						prect->m_fX1 = fminx;
						prect->m_fY1 = fminy;
						prect->m_fX2 = fmaxx;
						prect->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
					{
						COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schtextframe.Copy(ptextframe);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX1 - x0, ptextframe->m_fY1 - y0, rotation, mirror);
						ptextframe->m_fX1 = fx1 + x0 + deltax;
						ptextframe->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX2 - x0, ptextframe->m_fY2 - y0, rotation, mirror);
						ptextframe->m_fX2 = fx1 + x0 + deltax;
						ptextframe->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(ptextframe->m_fX1, ptextframe->m_fX2);
						fmaxx = MAX(ptextframe->m_fX1, ptextframe->m_fX2);
						fminy = MIN(ptextframe->m_fY1, ptextframe->m_fY2);
						fmaxy = MAX(ptextframe->m_fY1, ptextframe->m_fY2);
						ptextframe->m_fX1 = fminx;
						ptextframe->m_fY1 = fminy;
						ptextframe->m_fX2 = fmaxx;
						ptextframe->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
					{
						COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schgraphic.Copy(pgraphic);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX1 - x0, pgraphic->m_fY1 - y0, rotation, mirror);
						pgraphic->m_fX1 = fx1 + x0 + deltax;
						pgraphic->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX2 - x0, pgraphic->m_fY2 - y0, rotation, mirror);
						pgraphic->m_fX2 = fx1 + x0 + deltax;
						pgraphic->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(pgraphic->m_fX1, pgraphic->m_fX2);
						fmaxx = MAX(pgraphic->m_fX1, pgraphic->m_fX2);
						fminy = MIN(pgraphic->m_fY1, pgraphic->m_fY2);
						fmaxy = MAX(pgraphic->m_fY1, pgraphic->m_fY2);
						pgraphic->m_fX1 = fminx;
						pgraphic->m_fY1 = fminy;
						pgraphic->m_fX2 = fmaxx;
						pgraphic->m_fY2 = fmaxy;
					}
				}
			}
			
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
	}
	
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}	
	
	Invalidate();
}

void CESDView::OnEditAlignTop()
{
	double x0, y0, deltax, deltay;
	int rotation = 0;
	BOOL mirror = FALSE;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  

	POSITION posbak;
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

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
			if( pline->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_LINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = pline->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pline->m_cVertex[0].fx;
				y0 = pline->m_cVertex[0].fy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;
				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pline->m_cVertex[k].fx - x0, pline->m_cVertex[k].fy - y0, rotation, mirror);
					pline->m_cVertex[k].fx = fx + x0 + deltax;
					pline->m_cVertex[k].fy = fy + y0 + deltay;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			if(ppolyline->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POLYLINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = ppolyline->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppolyline->m_cVertex[0].fx;
				y0 = ppolyline->m_cVertex[0].fy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;
				for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ppolyline->m_cVertex[k].fx - x0, ppolyline->m_cVertex[k].fy - y0, rotation, mirror);
					ppolyline->m_cVertex[k].fx = fx + x0 + deltax;
					ppolyline->m_cVertex[k].fy = fy + y0 + deltay;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_JUNCTION;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = pjunc->m_fCx;
				pundo->m_fY1 = pjunc->m_fCy;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pjunc->m_fCx;
				y0 = pjunc->m_fCy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pjunc->m_fCx - x0, pjunc->m_fCy - y0, rotation, mirror);
				pjunc->m_fCx = fx + x0 + deltax;
				pjunc->m_fCy = fy + y0 + deltay;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if(ptext->m_bSelection == TRUE)
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
				pundo->m_nPos = posbak;
				pundo->schtext.Copy(ptext);
				pundo->schtext.m_nFlag = FLAG_NORMAL;

				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ptext->m_fSx;
				y0 = ptext->m_fSy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;
				double fx, fy;					
				pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - x0, ptext->m_fSy - y0, rotation, mirror);
				ptext->m_fSx = fx + x0 + deltax;
				ptext->m_fSy = fy + y0 + deltay;

				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ptext->m_nOrientation, rotation, FALSE); //mirror);
				ptext->m_nOrientation = orientation;
				ptext->m_nOrientation = pDoc->Get_Correct_Orientation(ptext->m_nOrientation);
		
				if(mirror == TRUE)
				{
					if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
					else ptext->m_bMirror = TRUE;
					if((ptext->m_nOrientation == 0)||(ptext->m_nOrientation == 180))
					{
						if(ptext->m_nH_Justification == 0) ptext->m_nH_Justification = 2;
						else if(ptext->m_nH_Justification == 2) ptext->m_nH_Justification = 0;
					}
					else
					{
						if(ptext->m_nV_Justification == 0) ptext->m_nV_Justification = 2;
						else if(ptext->m_nV_Justification == 2) ptext->m_nV_Justification = 0;
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if(parc->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ARC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->scharc.Copy(parc);
				pundo->scharc.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = parc->m_fCx;
				y0 = parc->m_fCy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, parc->m_fCx - x0, parc->m_fCy - y0, rotation, mirror);
				parc->m_fCx = fx + x0 + deltax;
				parc->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, parc->m_fSangle, parc->m_fEangle, rotation, mirror);
				parc->m_fSangle = salpha;
				parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
				parc->m_fEangle = ealpha;
				parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if(pellipse->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schellipse.Copy(pellipse);
				pundo->schellipse.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pellipse->m_fCx;
				y0 = pellipse->m_fCy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pellipse->m_fCx - x0, pellipse->m_fCy - y0, rotation, mirror);
				pellipse->m_fCx = fx + x0 + deltax;
				pellipse->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, pellipse->m_fSangle, pellipse->m_fEangle, rotation, mirror);
				pellipse->m_fSangle = salpha;
				pellipse->m_fSangle = pDoc->Get_Correct_Angle(pellipse->m_fSangle);
				pellipse->m_fEangle = ealpha;
				pellipse->m_fEangle = pDoc->Get_Correct_Angle(pellipse->m_fEangle);
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = pellipse->m_fRadius_y;
					pellipse->m_fRadius_y = pellipse->m_fRadius_x;
					pellipse->m_fRadius_x = ftemp;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if(pellipsefill->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schellipsefill.Copy(pellipsefill);
				pundo->schellipsefill.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pellipsefill->m_fCx;
				y0 = pellipsefill->m_fCy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pellipsefill->m_fCx - x0, pellipsefill->m_fCy - y0, rotation, mirror);
				pellipsefill->m_fCx = fx + x0 + deltax;
				pellipsefill->m_fCy = fy + y0 + deltay;
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = pellipsefill->m_fRadius_y;
					pellipsefill->m_fRadius_y = pellipsefill->m_fRadius_x;
					pellipsefill->m_fRadius_x = ftemp;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if(ppie->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_PIE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schpie.Copy(ppie);
				pundo->schpie.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppie->m_fCx;
				y0 = ppie->m_fCy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppie->m_fCx - x0, ppie->m_fCy - y0, rotation, mirror);
				ppie->m_fCx = fx + x0 + deltax;
				ppie->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, ppie->m_fSangle, ppie->m_fEangle, rotation, mirror);
				ppie->m_fSangle = salpha;
				ppie->m_fSangle = pDoc->Get_Correct_Angle(ppie->m_fSangle);
				ppie->m_fEangle = ealpha;
				ppie->m_fEangle = pDoc->Get_Correct_Angle(ppie->m_fEangle);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if(ppolygon->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POLYGON;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = ppolygon->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppolygon->m_cVertex[0].fx;
				y0 = ppolygon->m_cVertex[0].fy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;

				for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ppolygon->m_cVertex[k].fx - x0, ppolygon->m_cVertex[k].fy - y0, rotation, mirror);
					ppolygon->m_cVertex[k].fx = fx + x0 + deltax;
					ppolygon->m_cVertex[k].fy = fy + y0 + deltay;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = ptextframe->m_fX1;
				pundo->m_fY1 = ptextframe->m_fY1;
				pundo->m_fX2 = ptextframe->m_fX2;
				pundo->m_fY2 = ptextframe->m_fY2;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ptextframe->m_fX2;
				y0 = ptextframe->m_fY2;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;

				double fx, fy, dx, dy;
				dx = ptextframe->m_fX2 - ptextframe->m_fX1;
				dy = ptextframe->m_fY2 - ptextframe->m_fY1;
				pDoc->Get_Rotate_XY(&fx, &fy, ptextframe->m_fX1 - x0, ptextframe->m_fY1 - y0, rotation, mirror);
				ptextframe->m_fX1 = fx + x0 + deltax;
				ptextframe->m_fY1 = fy + y0 + deltay;
				ptextframe->m_fX2 = ptextframe->m_fX1 + dx;
				ptextframe->m_fY2 = ptextframe->m_fY1 + dy;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if(prect->m_bSelection == TRUE)
			{

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_RECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = prect->m_fX1;
				pundo->m_fY1 = prect->m_fY1;
				pundo->m_fX2 = prect->m_fX2;
				pundo->m_fY2 = prect->m_fY2;
			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = prect->m_fX2;
				y0 = prect->m_fY2;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, prect->m_fX1 - x0, prect->m_fY1 - y0, rotation, mirror);
				prect->m_fX1 = fx + x0 + deltax;
				prect->m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, prect->m_fX2 - x0, prect->m_fY2 - y0, rotation, mirror);
				prect->m_fX2 = fx + x0 + deltax;
				prect->m_fY2 = fy + y0 + deltay;

				double fminx, fminy, fmaxx, fmaxy;
				fminx = MIN(prect->m_fX1, prect->m_fX2);
				fmaxx = MAX(prect->m_fX1, prect->m_fX2);
				fminy = MIN(prect->m_fY1, prect->m_fY2);
				fmaxy = MAX(prect->m_fY1, prect->m_fY2);
				prect->m_fX1 = fminx;
				prect->m_fY1 = fminy;
				prect->m_fX2 = fmaxx;
				prect->m_fY2 = fmaxy;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if(proundrect->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = proundrect->m_fX1;
				pundo->m_fY1 = proundrect->m_fY1;
				pundo->m_fX2 = proundrect->m_fX2;
				pundo->m_fY2 = proundrect->m_fY2;
				pundo->m_fRx = proundrect->m_fRadius_x;
				pundo->m_fRy = proundrect->m_fRadius_y;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = proundrect->m_fX2;
				y0 = proundrect->m_fY2;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, proundrect->m_fX1 - x0, proundrect->m_fY1 - y0, rotation, mirror);
				proundrect->m_fX1 = fx + x0 + deltax;
				proundrect->m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, proundrect->m_fX2 - x0, proundrect->m_fY2 - y0, rotation, mirror);
				proundrect->m_fX2 = fx + x0 + deltax;
				proundrect->m_fY2 = fy + y0 + deltay;
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = proundrect->m_fRadius_y;
					proundrect->m_fRadius_y = proundrect->m_fRadius_x;
					proundrect->m_fRadius_x = ftemp;
				}

				double fminx, fminy, fmaxx, fmaxy;
				fminx = MIN(proundrect->m_fX1, proundrect->m_fX2);
				fmaxx = MAX(proundrect->m_fX1, proundrect->m_fX2);
				fminy = MIN(proundrect->m_fY1, proundrect->m_fY2);
				fmaxy = MAX(proundrect->m_fY1, proundrect->m_fY2);
				proundrect->m_fX1 = fminx;
				proundrect->m_fY1 = fminy;
				proundrect->m_fX2 = fmaxx;
				proundrect->m_fY2 = fmaxy;
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
			
			double dx, dy;
			if(psheetsymbol->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = psheetsymbol->m_fX;
				pundo->m_fY1 = psheetsymbol->m_fY;
				pundo->m_fX2 = psheetsymbol->m_fXsize;
				pundo->m_fY2 = psheetsymbol->m_fYsize;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = psheetsymbol->m_fX;
				y0 = psheetsymbol->m_fY;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;

				double fx, fy, fxbak, fybak;
				fxbak = psheetsymbol->m_fX;
				fybak = psheetsymbol->m_fY;

				pDoc->Get_Rotate_XY(&fx, &fy, psheetsymbol->m_fX - x0, psheetsymbol->m_fY - y0, rotation, mirror);
				psheetsymbol->m_fX = fx + x0 + deltax;
				psheetsymbol->m_fY = fy + y0 + deltay;

				dx = psheetsymbol->m_fX - fxbak;
				dy = psheetsymbol->m_fY - fybak;

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) 
				{
					posbak = pos;
					pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext;
						ptext = (COb_schtext*)pobject;

						
						pundo = new CSchUndo;
						pundo->m_nOp = UNDO_MOVE;
						pundo->m_nOb = SCH_ELEMENT_TEXT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = posbak;
						pundo->m_fX1 = ptext->m_fSx;
						pundo->m_fY1 = ptext->m_fSy;
						pundo->m_nOrientation = ptext->m_nOrientation;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
						ptext->m_fSx += dx;
						ptext->m_fSy += dy;
					}
				}
			}
			else 
			{
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(pport->m_bSelection == TRUE)
			{

				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_PORT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schport.Copy(pport);
				pundo->schport.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pport->m_fX;
				y0 = pport->m_fY;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pport->m_fX - x0, pport->m_fY - y0, rotation, mirror);
				pport->m_fX = fx + x0 + deltax;
				pport->m_fY = fy + y0 + deltay;
				if(rotation == 90)
				{
					if(pport->m_nStyle == PORT_STYLE_NH) pport->m_nStyle = PORT_STYLE_NV;
					else if(pport->m_nStyle == PORT_STYLE_NV) pport->m_nStyle = PORT_STYLE_NH;
					else if(pport->m_nStyle == PORT_STYLE_LEFTRIGHT) pport->m_nStyle = PORT_STYLE_TOPBOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOPBOTTOM) pport->m_nStyle = PORT_STYLE_LEFTRIGHT;
					else if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_BOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_LEFT;
				}
				else if(rotation == 180)
				{
					if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_LEFT;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_BOTTOM;
				}
				else if(rotation == 270)
				{
					if(pport->m_nStyle == PORT_STYLE_NH) pport->m_nStyle = PORT_STYLE_NV;
					else if(pport->m_nStyle == PORT_STYLE_NV) pport->m_nStyle = PORT_STYLE_NH;
					else if(pport->m_nStyle == PORT_STYLE_LEFTRIGHT) pport->m_nStyle = PORT_STYLE_TOPBOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOPBOTTOM) pport->m_nStyle = PORT_STYLE_LEFTRIGHT;
					else if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_LEFT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_BOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_RIGHT;
				}

				if(mirror == TRUE)
				{
					if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_LEFT;
				}

				if(pport->m_nStyle < PORT_STYLE_NV)
				{
					pport->m_fX -= pport->m_fSize;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POWER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = ppower->m_fX;
				pundo->m_fY1 = ppower->m_fY;
				pundo->m_nOrientation = ppower->m_nOrientation;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppower->m_fX;
				y0 = ppower->m_fY;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppower->m_fX - x0, ppower->m_fY - y0, rotation, mirror);
				ppower->m_fX = fx + x0 + deltax;
				ppower->m_fY = fy + y0 + deltay;
				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ppower->m_nOrientation, rotation, mirror);
				ppower->m_nOrientation = orientation;
				ppower->m_nOrientation = pDoc->Get_Correct_Orientation(ppower->m_nOrientation);

				//ppower->m_nOrientation += rotation;
				//ppower->m_nOrientation = pDoc->Get_Correct_Orientation(ppower->m_nOrientation);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if(pbezier->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_BEZIER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = pbezier->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pbezier->m_cVertex[0].fx;
				y0 = pbezier->m_cVertex[0].fy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;

				for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pbezier->m_cVertex[k].fx - x0, pbezier->m_cVertex[k].fy - y0, rotation, mirror);
					pbezier->m_cVertex[k].fx = fx + x0 + deltax;
					pbezier->m_cVertex[k].fy = fy + y0 + deltay;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = pgraphic->m_fX1;
				pundo->m_fY1 = pgraphic->m_fY1;
				pundo->m_fX2 = pgraphic->m_fX2;
				pundo->m_fY2 = pgraphic->m_fY2;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pgraphic->m_fX2;
				y0 = pgraphic->m_fY2;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;

				double fx, fy, dx, dy;
				dx = pgraphic->m_fX2 - pgraphic->m_fX1;
				dy = pgraphic->m_fY2 - pgraphic->m_fY1;
				pDoc->Get_Rotate_XY(&fx, &fy, pgraphic->m_fX1 - x0, pgraphic->m_fY1 - y0, rotation, mirror);
				pgraphic->m_fX1 = fx + x0 + deltax;
				pgraphic->m_fY1 = fy + y0 + deltay;
				pgraphic->m_fX2 = pgraphic->m_fX1 + dx;
				pgraphic->m_fY2 = pgraphic->m_fY1 + dy;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_COMPONENT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schcomp.Copy(pcomp);
				pundo->schcomp.m_nFlag = FLAG_NORMAL;
						
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pcomp->m_fX;
				y0 = pcomp->m_fY;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - y0;

				double fx1, fy1;
				pDoc->Get_Rotate_XY(&fx1, &fy1, pcomp->m_fX - x0, pcomp->m_fY - y0, rotation, mirror);
				pcomp->m_fX = fx1 + x0 + deltax;
				pcomp->m_fY = fy1 + y0 + deltay;
				pcomp->m_nOrientation += rotation;
				pcomp->m_nOrientation = pDoc->Get_Correct_Orientation(pcomp->m_nOrientation);
				if(mirror == TRUE)
				{
					if(pcomp->m_bMirrored == TRUE) pcomp->m_bMirrored = FALSE;
					else pcomp->m_bMirrored = TRUE;
				}
				
				for(int i=0; i<pcomp->m_arrayPos.GetSize(); i++)
				{
					CObject* pobject;
					POSITION subpos;
					subpos = pcomp->m_arrayPos.GetAt(i);
					pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
					{
						COb_schpin* ppin = (COb_schpin*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_PIN;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpin.Copy(ppin);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						

						pDoc->Get_Rotate_XY(&fx1, &fy1, ppin->m_fX - x0, ppin->m_fY - y0, rotation, mirror);
						ppin->m_fX = fx1 + x0 + deltax;
						ppin->m_fY = fy1 + y0 + deltay;

						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, ppin->m_nOrientation, rotation, mirror);
						ppin->m_nOrientation = orientation;
						ppin->m_nOrientation = pDoc->Get_Correct_Orientation(ppin->m_nOrientation);					
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
					{
						COb_schieee* pieee = (COb_schieee*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_IEEE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schieee.Copy(pieee);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pieee->m_fX - x0, pieee->m_fY - y0, rotation, mirror);
						pieee->m_fX = fx1 + x0 + deltax;
						pieee->m_fY = fy1 + y0 + deltay;
						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, pieee->m_nOrientation, rotation, mirror);
						pieee->m_nOrientation = orientation;
						pieee->m_nOrientation = pDoc->Get_Correct_Orientation(pieee->m_nOrientation);
	
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_TEXT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schtext.Copy(ptext);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, ptext->m_fSx - x0, ptext->m_fSy - y0, rotation, mirror);
						ptext->m_fSx = fx1 + x0 + deltax;
						ptext->m_fSy = fy1 + y0 + deltay;
						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, ptext->m_nOrientation, rotation, FALSE); //mirror);
						ptext->m_nOrientation = orientation;
						ptext->m_nOrientation = pDoc->Get_Correct_Orientation(ptext->m_nOrientation);
			
						if(mirror == TRUE)
						{
							if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
							else ptext->m_bMirror = TRUE;
							if((ptext->m_nOrientation == 0)||(ptext->m_nOrientation == 180))
							{
								if(ptext->m_nH_Justification == 0) ptext->m_nH_Justification = 2;
								else if(ptext->m_nH_Justification == 2) ptext->m_nH_Justification = 0;
							}
							else
							{
								if(ptext->m_nV_Justification == 0) ptext->m_nV_Justification = 2;
								else if(ptext->m_nV_Justification == 2) ptext->m_nV_Justification = 0;
							}
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
					{
						COb_schbezier* pbezier = (COb_schbezier*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_BEZIER;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schbezier.Copy(pbezier);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, pbezier->m_cVertex[k].fx-x0, pbezier->m_cVertex[k].fy-y0, rotation, mirror);
							pbezier->m_cVertex[k].fx = fx1 + x0 + deltax;
							pbezier->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
					{
						COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_POLYLINE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpolyline.Copy(ppolyline);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, ppolyline->m_cVertex[k].fx-x0, ppolyline->m_cVertex[k].fy-y0, rotation, mirror);
							ppolyline->m_cVertex[k].fx = fx1 + x0 + deltax;
							ppolyline->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
					{
						COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_POLYGON;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpolygon.Copy(ppolygon);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, ppolygon->m_cVertex[k].fx-x0, ppolygon->m_cVertex[k].fy-y0, rotation, mirror);
							ppolygon->m_cVertex[k].fx = fx1 + x0 + deltax;
							ppolygon->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
					{
						COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schellipsefill.Copy(pellipsefill);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pellipsefill->m_fCx - x0, pellipsefill->m_fCy - y0, rotation, mirror);
						pellipsefill->m_fCx = fx1 + x0 + deltax;
						pellipsefill->m_fCy = fy1 + y0 + deltay;
						if((rotation == 90)||(rotation == 270))
						{
							double ftemp;
							ftemp = pellipsefill->m_fRadius_y;
							pellipsefill->m_fRadius_y = pellipsefill->m_fRadius_x;
							pellipsefill->m_fRadius_x = ftemp;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
					{
						COb_schpie* ppie = (COb_schpie*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_PIE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpie.Copy(ppie);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, ppie->m_fCx - x0, ppie->m_fCy - y0, rotation, mirror);
						ppie->m_fCx = fx1 + x0 + deltax;
						ppie->m_fCy = fy1 + y0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, ppie->m_fSangle, ppie->m_fEangle, rotation, mirror);
						ppie->m_fSangle = salpha;
						ppie->m_fSangle = pDoc->Get_Correct_Angle(ppie->m_fSangle);
						ppie->m_fEangle = ealpha;
						ppie->m_fEangle = pDoc->Get_Correct_Angle(ppie->m_fEangle);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
					{
						COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schroundrect.Copy(proundrect);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX1 - x0, proundrect->m_fY1 - y0, rotation, mirror);
						proundrect->m_fX1 = fx1 + x0 + deltax;
						proundrect->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX2 - x0, proundrect->m_fY2 - y0, rotation, mirror);
						proundrect->m_fX2 = fx1 + x0 + deltax;
						proundrect->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(proundrect->m_fX1, proundrect->m_fX2);
						fmaxx = MAX(proundrect->m_fX1, proundrect->m_fX2);
						fminy = MIN(proundrect->m_fY1, proundrect->m_fY2);
						fmaxy = MAX(proundrect->m_fY1, proundrect->m_fY2);
						proundrect->m_fX1 = fminx;
						proundrect->m_fY1 = fminy;
						proundrect->m_fX2 = fmaxx;
						proundrect->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
					{
						COb_schellipse* pellipse = (COb_schellipse*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schellipse.Copy(pellipse);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pellipse->m_fCx - x0, pellipse->m_fCy - y0, rotation, mirror);
						pellipse->m_fCx = fx1 + x0 + deltax;
						pellipse->m_fCy = fy1 + y0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, pellipse->m_fSangle, pellipse->m_fEangle, rotation, mirror);
						pellipse->m_fSangle = salpha;
						pellipse->m_fSangle = pDoc->Get_Correct_Angle(pellipse->m_fSangle);
						pellipse->m_fEangle = ealpha;
						pellipse->m_fEangle = pDoc->Get_Correct_Angle(pellipse->m_fEangle);
						if((rotation == 90)||(rotation == 270))
						{
							double ftemp;
							ftemp = pellipse->m_fRadius_y;
							pellipse->m_fRadius_y = pellipse->m_fRadius_x;
							pellipse->m_fRadius_x = ftemp;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
					{
						COb_scharc* parc = (COb_scharc*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ARC;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->scharc.Copy(parc);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, parc->m_fCx - x0, parc->m_fCy - y0, rotation, mirror);
						parc->m_fCx = fx1 + x0 + deltax;
						parc->m_fCy = fy1 + y0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, parc->m_fSangle, parc->m_fEangle, rotation, mirror);
						parc->m_fSangle = salpha;
						parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
						parc->m_fEangle = ealpha;
						parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
					{
						COb_schrect* prect = (COb_schrect*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_RECT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schrect.Copy(prect);
										
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX1 - x0, prect->m_fY1 - y0, rotation, mirror);
						prect->m_fX1 = fx1 + x0 + deltax;
						prect->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX2 - x0, prect->m_fY2 - y0, rotation, mirror);
						prect->m_fX2 = fx1 + x0 + deltax;
						prect->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(prect->m_fX1, prect->m_fX2);
						fmaxx = MAX(prect->m_fX1, prect->m_fX2);
						fminy = MIN(prect->m_fY1, prect->m_fY2);
						fmaxy = MAX(prect->m_fY1, prect->m_fY2);
						prect->m_fX1 = fminx;
						prect->m_fY1 = fminy;
						prect->m_fX2 = fmaxx;
						prect->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
					{
						COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schtextframe.Copy(ptextframe);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX1 - x0, ptextframe->m_fY1 - y0, rotation, mirror);
						ptextframe->m_fX1 = fx1 + x0 + deltax;
						ptextframe->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX2 - x0, ptextframe->m_fY2 - y0, rotation, mirror);
						ptextframe->m_fX2 = fx1 + x0 + deltax;
						ptextframe->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(ptextframe->m_fX1, ptextframe->m_fX2);
						fmaxx = MAX(ptextframe->m_fX1, ptextframe->m_fX2);
						fminy = MIN(ptextframe->m_fY1, ptextframe->m_fY2);
						fmaxy = MAX(ptextframe->m_fY1, ptextframe->m_fY2);
						ptextframe->m_fX1 = fminx;
						ptextframe->m_fY1 = fminy;
						ptextframe->m_fX2 = fmaxx;
						ptextframe->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
					{
						COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schgraphic.Copy(pgraphic);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX1 - x0, pgraphic->m_fY1 - y0, rotation, mirror);
						pgraphic->m_fX1 = fx1 + x0 + deltax;
						pgraphic->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX2 - x0, pgraphic->m_fY2 - y0, rotation, mirror);
						pgraphic->m_fX2 = fx1 + x0 + deltax;
						pgraphic->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(pgraphic->m_fX1, pgraphic->m_fX2);
						fmaxx = MAX(pgraphic->m_fX1, pgraphic->m_fX2);
						fminy = MIN(pgraphic->m_fY1, pgraphic->m_fY2);
						fmaxy = MAX(pgraphic->m_fY1, pgraphic->m_fY2);
						pgraphic->m_fX1 = fminx;
						pgraphic->m_fY1 = fminy;
						pgraphic->m_fX2 = fmaxx;
						pgraphic->m_fY2 = fmaxy;
					}
				}
			}
			
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
	}
	
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}	
	
	Invalidate();
}

void CESDView::OnEditAlignHplace()
{
	double x0, y0, deltax, deltay;
	int rotation = 0;
	BOOL mirror = FALSE;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	POSITION posbak;
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);
	
	int totalnum = pDoc->m_arraySelection[m_nDiagram].GetCount();
	int currentnum = 0;
	double totalspace =  pDoc->m_fSelection_X2[m_nDiagram] -  pDoc->m_fSelection_X1[m_nDiagram];

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
			if( pline->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_LINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = pline->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pline->m_cVertex[0].fx;
				y0 = pline->m_cVertex[0].fy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pline->m_cVertex[k].fx - x0, pline->m_cVertex[k].fy - y0, rotation, mirror);
					pline->m_cVertex[k].fx = fx + x0 + deltax;
					pline->m_cVertex[k].fy = fy + y0 + deltay;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			if(ppolyline->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POLYLINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = ppolyline->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppolyline->m_cVertex[0].fx;
				y0 = ppolyline->m_cVertex[0].fy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ppolyline->m_cVertex[k].fx - x0, ppolyline->m_cVertex[k].fy - y0, rotation, mirror);
					ppolyline->m_cVertex[k].fx = fx + x0 + deltax;
					ppolyline->m_cVertex[k].fy = fy + y0 + deltay;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_JUNCTION;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = pjunc->m_fCx;
				pundo->m_fY1 = pjunc->m_fCy;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pjunc->m_fCx;
				y0 = pjunc->m_fCy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pjunc->m_fCx - x0, pjunc->m_fCy - y0, rotation, mirror);
				pjunc->m_fCx = fx + x0 + deltax;
				pjunc->m_fCy = fy + y0 + deltay;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if(ptext->m_bSelection == TRUE)
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
				pundo->m_nPos = posbak;
				pundo->schtext.Copy(ptext);
				pundo->schtext.m_nFlag = FLAG_NORMAL;

				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ptext->m_fSx;
				y0 = ptext->m_fSy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				double fx, fy;					
				pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - x0, ptext->m_fSy - y0, rotation, mirror);
				ptext->m_fSx = fx + x0 + deltax;
				ptext->m_fSy = fy + y0 + deltay;

				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ptext->m_nOrientation, rotation, FALSE); //mirror);
				ptext->m_nOrientation = orientation;
				ptext->m_nOrientation = pDoc->Get_Correct_Orientation(ptext->m_nOrientation);
		
				if(mirror == TRUE)
				{
					if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
					else ptext->m_bMirror = TRUE;
					if((ptext->m_nOrientation == 0)||(ptext->m_nOrientation == 180))
					{
						if(ptext->m_nH_Justification == 0) ptext->m_nH_Justification = 2;
						else if(ptext->m_nH_Justification == 2) ptext->m_nH_Justification = 0;
					}
					else
					{
						if(ptext->m_nV_Justification == 0) ptext->m_nV_Justification = 2;
						else if(ptext->m_nV_Justification == 2) ptext->m_nV_Justification = 0;
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if(parc->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ARC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->scharc.Copy(parc);
				pundo->scharc.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = parc->m_fCx;
				y0 = parc->m_fCy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, parc->m_fCx - x0, parc->m_fCy - y0, rotation, mirror);
				parc->m_fCx = fx + x0 + deltax;
				parc->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, parc->m_fSangle, parc->m_fEangle, rotation, mirror);
				parc->m_fSangle = salpha;
				parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
				parc->m_fEangle = ealpha;
				parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if(pellipse->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schellipse.Copy(pellipse);
				pundo->schellipse.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pellipse->m_fCx;
				y0 = pellipse->m_fCy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pellipse->m_fCx - x0, pellipse->m_fCy - y0, rotation, mirror);
				pellipse->m_fCx = fx + x0 + deltax;
				pellipse->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, pellipse->m_fSangle, pellipse->m_fEangle, rotation, mirror);
				pellipse->m_fSangle = salpha;
				pellipse->m_fSangle = pDoc->Get_Correct_Angle(pellipse->m_fSangle);
				pellipse->m_fEangle = ealpha;
				pellipse->m_fEangle = pDoc->Get_Correct_Angle(pellipse->m_fEangle);
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = pellipse->m_fRadius_y;
					pellipse->m_fRadius_y = pellipse->m_fRadius_x;
					pellipse->m_fRadius_x = ftemp;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if(pellipsefill->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schellipsefill.Copy(pellipsefill);
				pundo->schellipsefill.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pellipsefill->m_fCx;
				y0 = pellipsefill->m_fCy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pellipsefill->m_fCx - x0, pellipsefill->m_fCy - y0, rotation, mirror);
				pellipsefill->m_fCx = fx + x0 + deltax;
				pellipsefill->m_fCy = fy + y0 + deltay;
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = pellipsefill->m_fRadius_y;
					pellipsefill->m_fRadius_y = pellipsefill->m_fRadius_x;
					pellipsefill->m_fRadius_x = ftemp;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if(ppie->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_PIE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schpie.Copy(ppie);
				pundo->schpie.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppie->m_fCx;
				y0 = ppie->m_fCy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppie->m_fCx - x0, ppie->m_fCy - y0, rotation, mirror);
				ppie->m_fCx = fx + x0 + deltax;
				ppie->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, ppie->m_fSangle, ppie->m_fEangle, rotation, mirror);
				ppie->m_fSangle = salpha;
				ppie->m_fSangle = pDoc->Get_Correct_Angle(ppie->m_fSangle);
				ppie->m_fEangle = ealpha;
				ppie->m_fEangle = pDoc->Get_Correct_Angle(ppie->m_fEangle);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if(ppolygon->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POLYGON;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = ppolygon->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppolygon->m_cVertex[0].fx;
				y0 = ppolygon->m_cVertex[0].fy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ppolygon->m_cVertex[k].fx - x0, ppolygon->m_cVertex[k].fy - y0, rotation, mirror);
					ppolygon->m_cVertex[k].fx = fx + x0 + deltax;
					ppolygon->m_cVertex[k].fy = fy + y0 + deltay;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = ptextframe->m_fX1;
				pundo->m_fY1 = ptextframe->m_fY1;
				pundo->m_fX2 = ptextframe->m_fX2;
				pundo->m_fY2 = ptextframe->m_fY2;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ptextframe->m_fX1;
				y0 = ptextframe->m_fY1;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				double fx, fy, dx, dy;
				dx = ptextframe->m_fX2 - ptextframe->m_fX1;
				dy = ptextframe->m_fY2 - ptextframe->m_fY1;
				pDoc->Get_Rotate_XY(&fx, &fy, ptextframe->m_fX1 - x0, ptextframe->m_fY1 - y0, rotation, mirror);
				ptextframe->m_fX1 = fx + x0 + deltax;
				ptextframe->m_fY1 = fy + y0 + deltay;
				ptextframe->m_fX2 = ptextframe->m_fX1 + dx;
				ptextframe->m_fY2 = ptextframe->m_fY1 + dy;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if(prect->m_bSelection == TRUE)
			{

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_RECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = prect->m_fX1;
				pundo->m_fY1 = prect->m_fY1;
				pundo->m_fX2 = prect->m_fX2;
				pundo->m_fY2 = prect->m_fY2;
			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = prect->m_fX1;
				y0 = prect->m_fY1;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, prect->m_fX1 - x0, prect->m_fY1 - y0, rotation, mirror);
				prect->m_fX1 = fx + x0 + deltax;
				prect->m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, prect->m_fX2 - x0, prect->m_fY2 - y0, rotation, mirror);
				prect->m_fX2 = fx + x0 + deltax;
				prect->m_fY2 = fy + y0 + deltay;

				double fminx, fminy, fmaxx, fmaxy;
				fminx = MIN(prect->m_fX1, prect->m_fX2);
				fmaxx = MAX(prect->m_fX1, prect->m_fX2);
				fminy = MIN(prect->m_fY1, prect->m_fY2);
				fmaxy = MAX(prect->m_fY1, prect->m_fY2);
				prect->m_fX1 = fminx;
				prect->m_fY1 = fminy;
				prect->m_fX2 = fmaxx;
				prect->m_fY2 = fmaxy;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if(proundrect->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = proundrect->m_fX1;
				pundo->m_fY1 = proundrect->m_fY1;
				pundo->m_fX2 = proundrect->m_fX2;
				pundo->m_fY2 = proundrect->m_fY2;
				pundo->m_fRx = proundrect->m_fRadius_x;
				pundo->m_fRy = proundrect->m_fRadius_y;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = proundrect->m_fX1;
				y0 = proundrect->m_fY1;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, proundrect->m_fX1 - x0, proundrect->m_fY1 - y0, rotation, mirror);
				proundrect->m_fX1 = fx + x0 + deltax;
				proundrect->m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, proundrect->m_fX2 - x0, proundrect->m_fY2 - y0, rotation, mirror);
				proundrect->m_fX2 = fx + x0 + deltax;
				proundrect->m_fY2 = fy + y0 + deltay;
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = proundrect->m_fRadius_y;
					proundrect->m_fRadius_y = proundrect->m_fRadius_x;
					proundrect->m_fRadius_x = ftemp;
				}

				double fminx, fminy, fmaxx, fmaxy;
				fminx = MIN(proundrect->m_fX1, proundrect->m_fX2);
				fmaxx = MAX(proundrect->m_fX1, proundrect->m_fX2);
				fminy = MIN(proundrect->m_fY1, proundrect->m_fY2);
				fmaxy = MAX(proundrect->m_fY1, proundrect->m_fY2);
				proundrect->m_fX1 = fminx;
				proundrect->m_fY1 = fminy;
				proundrect->m_fX2 = fmaxx;
				proundrect->m_fY2 = fmaxy;
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
			
			double dx, dy;
			if(psheetsymbol->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = psheetsymbol->m_fX;
				pundo->m_fY1 = psheetsymbol->m_fY;
				pundo->m_fX2 = psheetsymbol->m_fXsize;
				pundo->m_fY2 = psheetsymbol->m_fYsize;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = psheetsymbol->m_fX;
				y0 = psheetsymbol->m_fY;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				double fx, fy, fxbak, fybak;
				fxbak = psheetsymbol->m_fX;
				fybak = psheetsymbol->m_fY;

				pDoc->Get_Rotate_XY(&fx, &fy, psheetsymbol->m_fX - x0, psheetsymbol->m_fY - y0, rotation, mirror);
				psheetsymbol->m_fX = fx + x0 + deltax;
				psheetsymbol->m_fY = fy + y0 + deltay;

				dx = psheetsymbol->m_fX - fxbak;
				dy = psheetsymbol->m_fY - fybak;

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) 
				{
					posbak = pos;
					pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext;
						ptext = (COb_schtext*)pobject;

						
						pundo = new CSchUndo;
						pundo->m_nOp = UNDO_MOVE;
						pundo->m_nOb = SCH_ELEMENT_TEXT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = posbak;
						pundo->m_fX1 = ptext->m_fSx;
						pundo->m_fY1 = ptext->m_fSy;
						pundo->m_nOrientation = ptext->m_nOrientation;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
						ptext->m_fSx += dx;
						ptext->m_fSy += dy;
					}
				}
			}
			else 
			{
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(pport->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_PORT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schport.Copy(pport);
				pundo->schport.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pport->m_fX;
				y0 = pport->m_fY;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pport->m_fX - x0, pport->m_fY - y0, rotation, mirror);
				pport->m_fX = fx + x0 + deltax;
				pport->m_fY = fy + y0 + deltay;
				if(rotation == 90)
				{
					if(pport->m_nStyle == PORT_STYLE_NH) pport->m_nStyle = PORT_STYLE_NV;
					else if(pport->m_nStyle == PORT_STYLE_NV) pport->m_nStyle = PORT_STYLE_NH;
					else if(pport->m_nStyle == PORT_STYLE_LEFTRIGHT) pport->m_nStyle = PORT_STYLE_TOPBOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOPBOTTOM) pport->m_nStyle = PORT_STYLE_LEFTRIGHT;
					else if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_BOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_LEFT;
				}
				else if(rotation == 180)
				{
					if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_LEFT;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_BOTTOM;
				}
				else if(rotation == 270)
				{
					if(pport->m_nStyle == PORT_STYLE_NH) pport->m_nStyle = PORT_STYLE_NV;
					else if(pport->m_nStyle == PORT_STYLE_NV) pport->m_nStyle = PORT_STYLE_NH;
					else if(pport->m_nStyle == PORT_STYLE_LEFTRIGHT) pport->m_nStyle = PORT_STYLE_TOPBOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOPBOTTOM) pport->m_nStyle = PORT_STYLE_LEFTRIGHT;
					else if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_LEFT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_BOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_RIGHT;
				}

				if(mirror == TRUE)
				{
					if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_LEFT;
				}

				if(pport->m_nStyle < PORT_STYLE_NV)
				{
					pport->m_fX -= pport->m_fSize;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POWER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = ppower->m_fX;
				pundo->m_fY1 = ppower->m_fY;
				pundo->m_nOrientation = ppower->m_nOrientation;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppower->m_fX;
				y0 = ppower->m_fY;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppower->m_fX - x0, ppower->m_fY - y0, rotation, mirror);
				ppower->m_fX = fx + x0 + deltax;
				ppower->m_fY = fy + y0 + deltay;
				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ppower->m_nOrientation, rotation, mirror);
				ppower->m_nOrientation = orientation;
				ppower->m_nOrientation = pDoc->Get_Correct_Orientation(ppower->m_nOrientation);

				//ppower->m_nOrientation += rotation;
				//ppower->m_nOrientation = pDoc->Get_Correct_Orientation(ppower->m_nOrientation);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if(pbezier->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_BEZIER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = pbezier->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pbezier->m_cVertex[0].fx;
				y0 = pbezier->m_cVertex[0].fy;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pbezier->m_cVertex[k].fx - x0, pbezier->m_cVertex[k].fy - y0, rotation, mirror);
					pbezier->m_cVertex[k].fx = fx + x0 + deltax;
					pbezier->m_cVertex[k].fy = fy + y0 + deltay;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = pgraphic->m_fX1;
				pundo->m_fY1 = pgraphic->m_fY1;
				pundo->m_fX2 = pgraphic->m_fX2;
				pundo->m_fY2 = pgraphic->m_fY2;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pgraphic->m_fX1;
				y0 = pgraphic->m_fY1;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				double fx, fy, dx, dy;
				dx = pgraphic->m_fX2 - pgraphic->m_fX1;
				dy = pgraphic->m_fY2 - pgraphic->m_fY1;
				pDoc->Get_Rotate_XY(&fx, &fy, pgraphic->m_fX1 - x0, pgraphic->m_fY1 - y0, rotation, mirror);
				pgraphic->m_fX1 = fx + x0 + deltax;
				pgraphic->m_fY1 = fy + y0 + deltay;
				pgraphic->m_fX2 = pgraphic->m_fX1 + dx;
				pgraphic->m_fY2 = pgraphic->m_fY1 + dy;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_COMPONENT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schcomp.Copy(pcomp);
				pundo->schcomp.m_nFlag = FLAG_NORMAL;
						
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pcomp->m_fX;
				y0 = pcomp->m_fY;
				deltax = pDoc->m_fSelection_X1[m_nDiagram] + totalspace*currentnum/totalnum - x0;
				deltay = 0;
				currentnum++;

				double fx1, fy1;
				pDoc->Get_Rotate_XY(&fx1, &fy1, pcomp->m_fX - x0, pcomp->m_fY - y0, rotation, mirror);
				pcomp->m_fX = fx1 + x0 + deltax;
				pcomp->m_fY = fy1 + y0 + deltay;
				pcomp->m_nOrientation += rotation;
				pcomp->m_nOrientation = pDoc->Get_Correct_Orientation(pcomp->m_nOrientation);
				if(mirror == TRUE)
				{
					if(pcomp->m_bMirrored == TRUE) pcomp->m_bMirrored = FALSE;
					else pcomp->m_bMirrored = TRUE;
				}
				
				for(int i=0; i<pcomp->m_arrayPos.GetSize(); i++)
				{
					CObject* pobject;
					POSITION subpos;
					subpos = pcomp->m_arrayPos.GetAt(i);
					pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
					{
						COb_schpin* ppin = (COb_schpin*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_PIN;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpin.Copy(ppin);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						

						pDoc->Get_Rotate_XY(&fx1, &fy1, ppin->m_fX - x0, ppin->m_fY - y0, rotation, mirror);
						ppin->m_fX = fx1 + x0 + deltax;
						ppin->m_fY = fy1 + y0 + deltay;

						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, ppin->m_nOrientation, rotation, mirror);
						ppin->m_nOrientation = orientation;
						ppin->m_nOrientation = pDoc->Get_Correct_Orientation(ppin->m_nOrientation);					
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
					{
						COb_schieee* pieee = (COb_schieee*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_IEEE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schieee.Copy(pieee);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pieee->m_fX - x0, pieee->m_fY - y0, rotation, mirror);
						pieee->m_fX = fx1 + x0 + deltax;
						pieee->m_fY = fy1 + y0 + deltay;
						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, pieee->m_nOrientation, rotation, mirror);
						pieee->m_nOrientation = orientation;
						pieee->m_nOrientation = pDoc->Get_Correct_Orientation(pieee->m_nOrientation);
	
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_TEXT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schtext.Copy(ptext);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, ptext->m_fSx - x0, ptext->m_fSy - y0, rotation, mirror);
						ptext->m_fSx = fx1 + x0 + deltax;
						ptext->m_fSy = fy1 + y0 + deltay;
						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, ptext->m_nOrientation, rotation, FALSE); //mirror);
						ptext->m_nOrientation = orientation;
						ptext->m_nOrientation = pDoc->Get_Correct_Orientation(ptext->m_nOrientation);
			
						if(mirror == TRUE)
						{
							if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
							else ptext->m_bMirror = TRUE;
							if((ptext->m_nOrientation == 0)||(ptext->m_nOrientation == 180))
							{
								if(ptext->m_nH_Justification == 0) ptext->m_nH_Justification = 2;
								else if(ptext->m_nH_Justification == 2) ptext->m_nH_Justification = 0;
							}
							else
							{
								if(ptext->m_nV_Justification == 0) ptext->m_nV_Justification = 2;
								else if(ptext->m_nV_Justification == 2) ptext->m_nV_Justification = 0;
							}
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
					{
						COb_schbezier* pbezier = (COb_schbezier*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_BEZIER;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schbezier.Copy(pbezier);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, pbezier->m_cVertex[k].fx-x0, pbezier->m_cVertex[k].fy-y0, rotation, mirror);
							pbezier->m_cVertex[k].fx = fx1 + x0 + deltax;
							pbezier->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
					{
						COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_POLYLINE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpolyline.Copy(ppolyline);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, ppolyline->m_cVertex[k].fx-x0, ppolyline->m_cVertex[k].fy-y0, rotation, mirror);
							ppolyline->m_cVertex[k].fx = fx1 + x0 + deltax;
							ppolyline->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
					{
						COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_POLYGON;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpolygon.Copy(ppolygon);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, ppolygon->m_cVertex[k].fx-x0, ppolygon->m_cVertex[k].fy-y0, rotation, mirror);
							ppolygon->m_cVertex[k].fx = fx1 + x0 + deltax;
							ppolygon->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
					{
						COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schellipsefill.Copy(pellipsefill);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pellipsefill->m_fCx - x0, pellipsefill->m_fCy - y0, rotation, mirror);
						pellipsefill->m_fCx = fx1 + x0 + deltax;
						pellipsefill->m_fCy = fy1 + y0 + deltay;
						if((rotation == 90)||(rotation == 270))
						{
							double ftemp;
							ftemp = pellipsefill->m_fRadius_y;
							pellipsefill->m_fRadius_y = pellipsefill->m_fRadius_x;
							pellipsefill->m_fRadius_x = ftemp;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
					{
						COb_schpie* ppie = (COb_schpie*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_PIE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpie.Copy(ppie);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, ppie->m_fCx - x0, ppie->m_fCy - y0, rotation, mirror);
						ppie->m_fCx = fx1 + x0 + deltax;
						ppie->m_fCy = fy1 + y0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, ppie->m_fSangle, ppie->m_fEangle, rotation, mirror);
						ppie->m_fSangle = salpha;
						ppie->m_fSangle = pDoc->Get_Correct_Angle(ppie->m_fSangle);
						ppie->m_fEangle = ealpha;
						ppie->m_fEangle = pDoc->Get_Correct_Angle(ppie->m_fEangle);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
					{
						COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schroundrect.Copy(proundrect);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX1 - x0, proundrect->m_fY1 - y0, rotation, mirror);
						proundrect->m_fX1 = fx1 + x0 + deltax;
						proundrect->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX2 - x0, proundrect->m_fY2 - y0, rotation, mirror);
						proundrect->m_fX2 = fx1 + x0 + deltax;
						proundrect->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(proundrect->m_fX1, proundrect->m_fX2);
						fmaxx = MAX(proundrect->m_fX1, proundrect->m_fX2);
						fminy = MIN(proundrect->m_fY1, proundrect->m_fY2);
						fmaxy = MAX(proundrect->m_fY1, proundrect->m_fY2);
						proundrect->m_fX1 = fminx;
						proundrect->m_fY1 = fminy;
						proundrect->m_fX2 = fmaxx;
						proundrect->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
					{
						COb_schellipse* pellipse = (COb_schellipse*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schellipse.Copy(pellipse);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pellipse->m_fCx - x0, pellipse->m_fCy - y0, rotation, mirror);
						pellipse->m_fCx = fx1 + x0 + deltax;
						pellipse->m_fCy = fy1 + y0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, pellipse->m_fSangle, pellipse->m_fEangle, rotation, mirror);
						pellipse->m_fSangle = salpha;
						pellipse->m_fSangle = pDoc->Get_Correct_Angle(pellipse->m_fSangle);
						pellipse->m_fEangle = ealpha;
						pellipse->m_fEangle = pDoc->Get_Correct_Angle(pellipse->m_fEangle);
						if((rotation == 90)||(rotation == 270))
						{
							double ftemp;
							ftemp = pellipse->m_fRadius_y;
							pellipse->m_fRadius_y = pellipse->m_fRadius_x;
							pellipse->m_fRadius_x = ftemp;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
					{
						COb_scharc* parc = (COb_scharc*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ARC;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->scharc.Copy(parc);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, parc->m_fCx - x0, parc->m_fCy - y0, rotation, mirror);
						parc->m_fCx = fx1 + x0 + deltax;
						parc->m_fCy = fy1 + y0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, parc->m_fSangle, parc->m_fEangle, rotation, mirror);
						parc->m_fSangle = salpha;
						parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
						parc->m_fEangle = ealpha;
						parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
					{
						COb_schrect* prect = (COb_schrect*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_RECT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schrect.Copy(prect);
										
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX1 - x0, prect->m_fY1 - y0, rotation, mirror);
						prect->m_fX1 = fx1 + x0 + deltax;
						prect->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX2 - x0, prect->m_fY2 - y0, rotation, mirror);
						prect->m_fX2 = fx1 + x0 + deltax;
						prect->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(prect->m_fX1, prect->m_fX2);
						fmaxx = MAX(prect->m_fX1, prect->m_fX2);
						fminy = MIN(prect->m_fY1, prect->m_fY2);
						fmaxy = MAX(prect->m_fY1, prect->m_fY2);
						prect->m_fX1 = fminx;
						prect->m_fY1 = fminy;
						prect->m_fX2 = fmaxx;
						prect->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
					{
						COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schtextframe.Copy(ptextframe);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX1 - x0, ptextframe->m_fY1 - y0, rotation, mirror);
						ptextframe->m_fX1 = fx1 + x0 + deltax;
						ptextframe->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX2 - x0, ptextframe->m_fY2 - y0, rotation, mirror);
						ptextframe->m_fX2 = fx1 + x0 + deltax;
						ptextframe->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(ptextframe->m_fX1, ptextframe->m_fX2);
						fmaxx = MAX(ptextframe->m_fX1, ptextframe->m_fX2);
						fminy = MIN(ptextframe->m_fY1, ptextframe->m_fY2);
						fmaxy = MAX(ptextframe->m_fY1, ptextframe->m_fY2);
						ptextframe->m_fX1 = fminx;
						ptextframe->m_fY1 = fminy;
						ptextframe->m_fX2 = fmaxx;
						ptextframe->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
					{
						COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schgraphic.Copy(pgraphic);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX1 - x0, pgraphic->m_fY1 - y0, rotation, mirror);
						pgraphic->m_fX1 = fx1 + x0 + deltax;
						pgraphic->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX2 - x0, pgraphic->m_fY2 - y0, rotation, mirror);
						pgraphic->m_fX2 = fx1 + x0 + deltax;
						pgraphic->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(pgraphic->m_fX1, pgraphic->m_fX2);
						fmaxx = MAX(pgraphic->m_fX1, pgraphic->m_fX2);
						fminy = MIN(pgraphic->m_fY1, pgraphic->m_fY2);
						fmaxy = MAX(pgraphic->m_fY1, pgraphic->m_fY2);
						pgraphic->m_fX1 = fminx;
						pgraphic->m_fY1 = fminy;
						pgraphic->m_fX2 = fmaxx;
						pgraphic->m_fY2 = fmaxy;
					}
				}
			}
			
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
	}
	
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}	
	
	Invalidate();
}

void CESDView::OnEditAlignVplace()
{
	double x0, y0, deltax, deltay;
	int rotation = 0;
	BOOL mirror = FALSE;
	BOOL clear_undo_buffer_flag = FALSE;	
	BOOL undo_flag = FALSE;  
	
	POSITION posbak;
	CObject* pobject;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	int totalnum = pDoc->m_arraySelection[m_nDiagram].GetCount();
	int currentnum = 0;
	double totalspace =  pDoc->m_fSelection_Y2[m_nDiagram] -  pDoc->m_fSelection_Y1[m_nDiagram];

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
			if( pline->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_LINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = pline->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pline->m_cVertex[0].fx;
				y0 = pline->m_cVertex[0].fy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				for(int k=0; k<pline->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pline->m_cVertex[k].fx - x0, pline->m_cVertex[k].fy - y0, rotation, mirror);
					pline->m_cVertex[k].fx = fx + x0 + deltax;
					pline->m_cVertex[k].fy = fy + y0 + deltay;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
		{
			COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
			if((ppolyline->m_nDiagram != m_nDiagram)||(ppolyline->m_nFlag == FLAG_DELETED))	continue;
			if(ppolyline->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POLYLINE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = ppolyline->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppolyline->m_cVertex[0].fx;
				y0 = ppolyline->m_cVertex[0].fy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ppolyline->m_cVertex[k].fx - x0, ppolyline->m_cVertex[k].fy - y0, rotation, mirror);
					ppolyline->m_cVertex[k].fx = fx + x0 + deltax;
					ppolyline->m_cVertex[k].fy = fy + y0 + deltay;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_JUNCTION;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = pjunc->m_fCx;
				pundo->m_fY1 = pjunc->m_fCy;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pjunc->m_fCx;
				y0 = pjunc->m_fCy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pjunc->m_fCx - x0, pjunc->m_fCy - y0, rotation, mirror);
				pjunc->m_fCx = fx + x0 + deltax;
				pjunc->m_fCy = fy + y0 + deltay;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
		{
			COb_schtext* ptext;
			ptext = (COb_schtext*)pobject;
			if((ptext->m_nDiagram != m_nDiagram)||(ptext->m_nFlag == FLAG_DELETED))	continue;
			if(ptext->m_bSelection == TRUE)
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
				pundo->m_nPos = posbak;
				pundo->schtext.Copy(ptext);
				pundo->schtext.m_nFlag = FLAG_NORMAL;

				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ptext->m_fSx;
				y0 = ptext->m_fSy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				double fx, fy;					
				pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - x0, ptext->m_fSy - y0, rotation, mirror);
				ptext->m_fSx = fx + x0 + deltax;
				ptext->m_fSy = fy + y0 + deltay;

				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ptext->m_nOrientation, rotation, FALSE); //mirror);
				ptext->m_nOrientation = orientation;
				ptext->m_nOrientation = pDoc->Get_Correct_Orientation(ptext->m_nOrientation);
		
				if(mirror == TRUE)
				{
					if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
					else ptext->m_bMirror = TRUE;
					if((ptext->m_nOrientation == 0)||(ptext->m_nOrientation == 180))
					{
						if(ptext->m_nH_Justification == 0) ptext->m_nH_Justification = 2;
						else if(ptext->m_nH_Justification == 2) ptext->m_nH_Justification = 0;
					}
					else
					{
						if(ptext->m_nV_Justification == 0) ptext->m_nV_Justification = 2;
						else if(ptext->m_nV_Justification == 2) ptext->m_nV_Justification = 0;
					}
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
		{
			COb_scharc* parc;
			parc = (COb_scharc*)pobject;
			if((parc->m_nDiagram != m_nDiagram)||(parc->m_nFlag == FLAG_DELETED))	continue;
			if(parc->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ARC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->scharc.Copy(parc);
				pundo->scharc.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = parc->m_fCx;
				y0 = parc->m_fCy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, parc->m_fCx - x0, parc->m_fCy - y0, rotation, mirror);
				parc->m_fCx = fx + x0 + deltax;
				parc->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, parc->m_fSangle, parc->m_fEangle, rotation, mirror);
				parc->m_fSangle = salpha;
				parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
				parc->m_fEangle = ealpha;
				parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
		{
			COb_schellipse* pellipse;
			pellipse = (COb_schellipse*)pobject;
			if((pellipse->m_nDiagram != m_nDiagram)||(pellipse->m_nFlag == FLAG_DELETED))	continue;
			if(pellipse->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schellipse.Copy(pellipse);
				pundo->schellipse.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pellipse->m_fCx;
				y0 = pellipse->m_fCy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pellipse->m_fCx - x0, pellipse->m_fCy - y0, rotation, mirror);
				pellipse->m_fCx = fx + x0 + deltax;
				pellipse->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, pellipse->m_fSangle, pellipse->m_fEangle, rotation, mirror);
				pellipse->m_fSangle = salpha;
				pellipse->m_fSangle = pDoc->Get_Correct_Angle(pellipse->m_fSangle);
				pellipse->m_fEangle = ealpha;
				pellipse->m_fEangle = pDoc->Get_Correct_Angle(pellipse->m_fEangle);
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = pellipse->m_fRadius_y;
					pellipse->m_fRadius_y = pellipse->m_fRadius_x;
					pellipse->m_fRadius_x = ftemp;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
		{
			COb_schellipsefill* pellipsefill;
			pellipsefill = (COb_schellipsefill*)pobject;
			if((pellipsefill->m_nDiagram != m_nDiagram)||(pellipsefill->m_nFlag == FLAG_DELETED))	continue;
			if(pellipsefill->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schellipsefill.Copy(pellipsefill);
				pundo->schellipsefill.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pellipsefill->m_fCx;
				y0 = pellipsefill->m_fCy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pellipsefill->m_fCx - x0, pellipsefill->m_fCy - y0, rotation, mirror);
				pellipsefill->m_fCx = fx + x0 + deltax;
				pellipsefill->m_fCy = fy + y0 + deltay;
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = pellipsefill->m_fRadius_y;
					pellipsefill->m_fRadius_y = pellipsefill->m_fRadius_x;
					pellipsefill->m_fRadius_x = ftemp;
				}
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
		{
			COb_schpie* ppie;
			ppie = (COb_schpie*)pobject;
			if((ppie->m_nDiagram != m_nDiagram)||(ppie->m_nFlag == FLAG_DELETED))	continue;
			if(ppie->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_PIE;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schpie.Copy(ppie);
				pundo->schpie.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppie->m_fCx;
				y0 = ppie->m_fCy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppie->m_fCx - x0, ppie->m_fCy - y0, rotation, mirror);
				ppie->m_fCx = fx + x0 + deltax;
				ppie->m_fCy = fy + y0 + deltay;
					
				double salpha, ealpha;
				pDoc->Get_Rotate_Angle(&salpha, &ealpha, ppie->m_fSangle, ppie->m_fEangle, rotation, mirror);
				ppie->m_fSangle = salpha;
				ppie->m_fSangle = pDoc->Get_Correct_Angle(ppie->m_fSangle);
				ppie->m_fEangle = ealpha;
				ppie->m_fEangle = pDoc->Get_Correct_Angle(ppie->m_fEangle);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
		{
			COb_schpolygon* ppolygon;
			ppolygon = (COb_schpolygon*)pobject;
			if((ppolygon->m_nDiagram != m_nDiagram)||(ppolygon->m_nFlag == FLAG_DELETED))	continue;
			if(ppolygon->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POLYGON;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = ppolygon->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppolygon->m_cVertex[0].fx;
				y0 = ppolygon->m_cVertex[0].fy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, ppolygon->m_cVertex[k].fx - x0, ppolygon->m_cVertex[k].fy - y0, rotation, mirror);
					ppolygon->m_cVertex[k].fx = fx + x0 + deltax;
					ppolygon->m_cVertex[k].fy = fy + y0 + deltay;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = ptextframe->m_fX1;
				pundo->m_fY1 = ptextframe->m_fY1;
				pundo->m_fX2 = ptextframe->m_fX2;
				pundo->m_fY2 = ptextframe->m_fY2;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ptextframe->m_fX2;
				y0 = ptextframe->m_fY2;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				double fx, fy, dx, dy;
				dx = ptextframe->m_fX2 - ptextframe->m_fX1;
				dy = ptextframe->m_fY2 - ptextframe->m_fY1;
				pDoc->Get_Rotate_XY(&fx, &fy, ptextframe->m_fX1 - x0, ptextframe->m_fY1 - y0, rotation, mirror);
				ptextframe->m_fX1 = fx + x0 + deltax;
				ptextframe->m_fY1 = fy + y0 + deltay;
				ptextframe->m_fX2 = ptextframe->m_fX1 + dx;
				ptextframe->m_fY2 = ptextframe->m_fY1 + dy;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
		{
			COb_schrect* prect;
			prect = (COb_schrect*)pobject;
			if((prect->m_nDiagram != m_nDiagram)||(prect->m_nFlag == FLAG_DELETED))	continue;
			if(prect->m_bSelection == TRUE)
			{

				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_RECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = prect->m_fX1;
				pundo->m_fY1 = prect->m_fY1;
				pundo->m_fX2 = prect->m_fX2;
				pundo->m_fY2 = prect->m_fY2;
			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = prect->m_fX2;
				y0 = prect->m_fY2;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, prect->m_fX1 - x0, prect->m_fY1 - y0, rotation, mirror);
				prect->m_fX1 = fx + x0 + deltax;
				prect->m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, prect->m_fX2 - x0, prect->m_fY2 - y0, rotation, mirror);
				prect->m_fX2 = fx + x0 + deltax;
				prect->m_fY2 = fy + y0 + deltay;

				double fminx, fminy, fmaxx, fmaxy;
				fminx = MIN(prect->m_fX1, prect->m_fX2);
				fmaxx = MAX(prect->m_fX1, prect->m_fX2);
				fminy = MIN(prect->m_fY1, prect->m_fY2);
				fmaxy = MAX(prect->m_fY1, prect->m_fY2);
				prect->m_fX1 = fminx;
				prect->m_fY1 = fminy;
				prect->m_fX2 = fmaxx;
				prect->m_fY2 = fmaxy;
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
		{
			COb_schroundrect* proundrect;
			proundrect = (COb_schroundrect*)pobject;
			if((proundrect->m_nDiagram != m_nDiagram)||(proundrect->m_nFlag == FLAG_DELETED))	continue;
			if(proundrect->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = proundrect->m_fX1;
				pundo->m_fY1 = proundrect->m_fY1;
				pundo->m_fX2 = proundrect->m_fX2;
				pundo->m_fY2 = proundrect->m_fY2;
				pundo->m_fRx = proundrect->m_fRadius_x;
				pundo->m_fRy = proundrect->m_fRadius_y;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = proundrect->m_fX2;
				y0 = proundrect->m_fY2;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, proundrect->m_fX1 - x0, proundrect->m_fY1 - y0, rotation, mirror);
				proundrect->m_fX1 = fx + x0 + deltax;
				proundrect->m_fY1 = fy + y0 + deltay;
				pDoc->Get_Rotate_XY(&fx, &fy, proundrect->m_fX2 - x0, proundrect->m_fY2 - y0, rotation, mirror);
				proundrect->m_fX2 = fx + x0 + deltax;
				proundrect->m_fY2 = fy + y0 + deltay;
				if((rotation == 90)||(rotation == 270))
				{
					double ftemp;
					ftemp = proundrect->m_fRadius_y;
					proundrect->m_fRadius_y = proundrect->m_fRadius_x;
					proundrect->m_fRadius_x = ftemp;
				}

				double fminx, fminy, fmaxx, fmaxy;
				fminx = MIN(proundrect->m_fX1, proundrect->m_fX2);
				fmaxx = MAX(proundrect->m_fX1, proundrect->m_fX2);
				fminy = MIN(proundrect->m_fY1, proundrect->m_fY2);
				fmaxy = MAX(proundrect->m_fY1, proundrect->m_fY2);
				proundrect->m_fX1 = fminx;
				proundrect->m_fY1 = fminy;
				proundrect->m_fX2 = fmaxx;
				proundrect->m_fY2 = fmaxy;
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
			
			double dx, dy;
			if(psheetsymbol->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_SHEETSYMBOL;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = psheetsymbol->m_fX;
				pundo->m_fY1 = psheetsymbol->m_fY;
				pundo->m_fX2 = psheetsymbol->m_fXsize;
				pundo->m_fY2 = psheetsymbol->m_fYsize;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = psheetsymbol->m_fX;
				y0 = psheetsymbol->m_fY;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				double fx, fy, fxbak, fybak;
				fxbak = psheetsymbol->m_fX;
				fybak = psheetsymbol->m_fY;

				pDoc->Get_Rotate_XY(&fx, &fy, psheetsymbol->m_fX - x0, psheetsymbol->m_fY - y0, rotation, mirror);
				psheetsymbol->m_fX = fx + x0 + deltax;
				psheetsymbol->m_fY = fy + y0 + deltay;

				dx = psheetsymbol->m_fX - fxbak;
				dy = psheetsymbol->m_fY - fybak;

				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) 
				{
					posbak = pos;
					pobject = pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext;
						ptext = (COb_schtext*)pobject;

						
						pundo = new CSchUndo;
						pundo->m_nOp = UNDO_MOVE;
						pundo->m_nOb = SCH_ELEMENT_TEXT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = posbak;
						pundo->m_fX1 = ptext->m_fSx;
						pundo->m_fY1 = ptext->m_fSy;
						pundo->m_nOrientation = ptext->m_nOrientation;
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						undo_flag = TRUE;
						
						ptext->m_fSx += dx;
						ptext->m_fSy += dy;
					}
				}
			}
			else 
			{
				for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schport )))
		{
			COb_schport* pport;
			pport = (COb_schport*)pobject;
			if((pport->m_nDiagram != m_nDiagram)||(pport->m_nFlag == FLAG_DELETED))	continue;
			if(pport->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_PORT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schport.Copy(pport);
				pundo->schport.m_nFlag = FLAG_NORMAL;
				
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pport->m_fX;
				y0 = pport->m_fY;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, pport->m_fX - x0, pport->m_fY - y0, rotation, mirror);
				pport->m_fX = fx + x0 + deltax;
				pport->m_fY = fy + y0 + deltay;
				if(rotation == 90)
				{
					if(pport->m_nStyle == PORT_STYLE_NH) pport->m_nStyle = PORT_STYLE_NV;
					else if(pport->m_nStyle == PORT_STYLE_NV) pport->m_nStyle = PORT_STYLE_NH;
					else if(pport->m_nStyle == PORT_STYLE_LEFTRIGHT) pport->m_nStyle = PORT_STYLE_TOPBOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOPBOTTOM) pport->m_nStyle = PORT_STYLE_LEFTRIGHT;
					else if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_BOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_LEFT;
				}
				else if(rotation == 180)
				{
					if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_LEFT;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_BOTTOM;
				}
				else if(rotation == 270)
				{
					if(pport->m_nStyle == PORT_STYLE_NH) pport->m_nStyle = PORT_STYLE_NV;
					else if(pport->m_nStyle == PORT_STYLE_NV) pport->m_nStyle = PORT_STYLE_NH;
					else if(pport->m_nStyle == PORT_STYLE_LEFTRIGHT) pport->m_nStyle = PORT_STYLE_TOPBOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOPBOTTOM) pport->m_nStyle = PORT_STYLE_LEFTRIGHT;
					else if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_TOP;
					else if(pport->m_nStyle == PORT_STYLE_BOTTOM) pport->m_nStyle = PORT_STYLE_LEFT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_BOTTOM;
					else if(pport->m_nStyle == PORT_STYLE_TOP) pport->m_nStyle = PORT_STYLE_RIGHT;
				}

				if(mirror == TRUE)
				{
					if(pport->m_nStyle == PORT_STYLE_LEFT)  pport->m_nStyle = PORT_STYLE_RIGHT;
					else if(pport->m_nStyle == PORT_STYLE_RIGHT) pport->m_nStyle = PORT_STYLE_LEFT;
				}

				if(pport->m_nStyle < PORT_STYLE_NV)
				{
					pport->m_fX -= pport->m_fSize;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_POWER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = ppower->m_fX;
				pundo->m_fY1 = ppower->m_fY;
				pundo->m_nOrientation = ppower->m_nOrientation;
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = ppower->m_fX;
				y0 = ppower->m_fY;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ppower->m_fX - x0, ppower->m_fY - y0, rotation, mirror);
				ppower->m_fX = fx + x0 + deltax;
				ppower->m_fY = fy + y0 + deltay;
				int orientation;
				pDoc->Get_Rotate_Orientation(&orientation, ppower->m_nOrientation, rotation, mirror);
				ppower->m_nOrientation = orientation;
				ppower->m_nOrientation = pDoc->Get_Correct_Orientation(ppower->m_nOrientation);

				//ppower->m_nOrientation += rotation;
				//ppower->m_nOrientation = pDoc->Get_Correct_Orientation(ppower->m_nOrientation);
			}
		}
		else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
		{
			COb_schbezier* pbezier;
			pbezier = (COb_schbezier*)pobject;
			if((pbezier->m_nDiagram != m_nDiagram)||(pbezier->m_nFlag == FLAG_DELETED))	continue;
			if(pbezier->m_bSelection == TRUE)
			{
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_BEZIER;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_cVertex.RemoveAll();
				for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
				{
					Fpoint fpoint;
					fpoint = pbezier->m_cVertex.GetAt(k);
					pundo->m_cVertex.Add(fpoint);
				}			
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pbezier->m_cVertex[0].fx;
				y0 = pbezier->m_cVertex[0].fy;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
				{
					double fx, fy;
					pDoc->Get_Rotate_XY(&fx, &fy, pbezier->m_cVertex[k].fx - x0, pbezier->m_cVertex[k].fy - y0, rotation, mirror);
					pbezier->m_cVertex[k].fx = fx + x0 + deltax;
					pbezier->m_cVertex[k].fy = fy + y0 + deltay;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_MOVE;
				pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->m_fX1 = pgraphic->m_fX1;
				pundo->m_fY1 = pgraphic->m_fY1;
				pundo->m_fX2 = pgraphic->m_fX2;
				pundo->m_fY2 = pgraphic->m_fY2;
		
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pgraphic->m_fX2;
				y0 = pgraphic->m_fY2;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				double fx, fy, dx, dy;
				dx = pgraphic->m_fX2 - pgraphic->m_fX1;
				dy = pgraphic->m_fY2 - pgraphic->m_fY1;
				pDoc->Get_Rotate_XY(&fx, &fy, pgraphic->m_fX1 - x0, pgraphic->m_fY1 - y0, rotation, mirror);
				pgraphic->m_fX1 = fx + x0 + deltax;
				pgraphic->m_fY1 = fy + y0 + deltay;
				pgraphic->m_fX2 = pgraphic->m_fX1 + dx;
				pgraphic->m_fY2 = pgraphic->m_fY1 + dy;
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
				
				if((pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].IsEmpty() == FALSE)&&(clear_undo_buffer_flag == FALSE))
				{
					pDoc->Clear_Discarded_Undo_Buffer();
				}
				clear_undo_buffer_flag = TRUE;

				CSchUndo *pundo = new CSchUndo;
				pundo->m_nOp = UNDO_CHANGE;
				pundo->m_nOb = SCH_ELEMENT_COMPONENT;
				pundo->m_nDiagram = m_nDiagram;
				pundo->m_nPos = posbak;
				pundo->schcomp.Copy(pcomp);
				pundo->schcomp.m_nFlag = FLAG_NORMAL;
						
				pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
				undo_flag = TRUE;
				
				x0 = pcomp->m_fX;
				y0 = pcomp->m_fY;
				deltax = 0;
				deltay = pDoc->m_fSelection_Y2[m_nDiagram] - totalspace*currentnum/totalnum - y0;
				currentnum++;

				double fx1, fy1;
				pDoc->Get_Rotate_XY(&fx1, &fy1, pcomp->m_fX - x0, pcomp->m_fY - y0, rotation, mirror);
				pcomp->m_fX = fx1 + x0 + deltax;
				pcomp->m_fY = fy1 + y0 + deltay;
				pcomp->m_nOrientation += rotation;
				pcomp->m_nOrientation = pDoc->Get_Correct_Orientation(pcomp->m_nOrientation);
				if(mirror == TRUE)
				{
					if(pcomp->m_bMirrored == TRUE) pcomp->m_bMirrored = FALSE;
					else pcomp->m_bMirrored = TRUE;
				}
				
				for(int i=0; i<pcomp->m_arrayPos.GetSize(); i++)
				{
					CObject* pobject;
					POSITION subpos;
					subpos = pcomp->m_arrayPos.GetAt(i);
					pobject = pDoc->m_listSchObject[m_nDiagram].GetAt(subpos);
					if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpin )))
					{
						COb_schpin* ppin = (COb_schpin*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_PIN;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpin.Copy(ppin);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						

						pDoc->Get_Rotate_XY(&fx1, &fy1, ppin->m_fX - x0, ppin->m_fY - y0, rotation, mirror);
						ppin->m_fX = fx1 + x0 + deltax;
						ppin->m_fY = fy1 + y0 + deltay;

						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, ppin->m_nOrientation, rotation, mirror);
						ppin->m_nOrientation = orientation;
						ppin->m_nOrientation = pDoc->Get_Correct_Orientation(ppin->m_nOrientation);					
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schieee )))
					{
						COb_schieee* pieee = (COb_schieee*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_IEEE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schieee.Copy(pieee);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pieee->m_fX - x0, pieee->m_fY - y0, rotation, mirror);
						pieee->m_fX = fx1 + x0 + deltax;
						pieee->m_fY = fy1 + y0 + deltay;
						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, pieee->m_nOrientation, rotation, mirror);
						pieee->m_nOrientation = orientation;
						pieee->m_nOrientation = pDoc->Get_Correct_Orientation(pieee->m_nOrientation);
	
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
					{
						COb_schtext* ptext = (COb_schtext*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_TEXT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schtext.Copy(ptext);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, ptext->m_fSx - x0, ptext->m_fSy - y0, rotation, mirror);
						ptext->m_fSx = fx1 + x0 + deltax;
						ptext->m_fSy = fy1 + y0 + deltay;
						int orientation;
						pDoc->Get_Rotate_Orientation(&orientation, ptext->m_nOrientation, rotation, FALSE); //mirror);
						ptext->m_nOrientation = orientation;
						ptext->m_nOrientation = pDoc->Get_Correct_Orientation(ptext->m_nOrientation);
			
						if(mirror == TRUE)
						{
							if(ptext->m_bMirror == TRUE) ptext->m_bMirror = FALSE;
							else ptext->m_bMirror = TRUE;
							if((ptext->m_nOrientation == 0)||(ptext->m_nOrientation == 180))
							{
								if(ptext->m_nH_Justification == 0) ptext->m_nH_Justification = 2;
								else if(ptext->m_nH_Justification == 2) ptext->m_nH_Justification = 0;
							}
							else
							{
								if(ptext->m_nV_Justification == 0) ptext->m_nV_Justification = 2;
								else if(ptext->m_nV_Justification == 2) ptext->m_nV_Justification = 0;
							}
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schbezier )))
					{
						COb_schbezier* pbezier = (COb_schbezier*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_BEZIER;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schbezier.Copy(pbezier);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						for(int k=0; k<pbezier->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, pbezier->m_cVertex[k].fx-x0, pbezier->m_cVertex[k].fy-y0, rotation, mirror);
							pbezier->m_cVertex[k].fx = fx1 + x0 + deltax;
							pbezier->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolyline )))
					{
						COb_schpolyline* ppolyline = (COb_schpolyline*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_POLYLINE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpolyline.Copy(ppolyline);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						for(int k=0; k<ppolyline->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, ppolyline->m_cVertex[k].fx-x0, ppolyline->m_cVertex[k].fy-y0, rotation, mirror);
							ppolyline->m_cVertex[k].fx = fx1 + x0 + deltax;
							ppolyline->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpolygon )))
					{
						COb_schpolygon* ppolygon = (COb_schpolygon*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_POLYGON;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpolygon.Copy(ppolygon);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						for(int k=0; k<ppolygon->m_cVertex.GetCount(); k++)
						{
							pDoc->Get_Rotate_XY(&fx1, &fy1, ppolygon->m_cVertex[k].fx-x0, ppolygon->m_cVertex[k].fy-y0, rotation, mirror);
							ppolygon->m_cVertex[k].fx = fx1 + x0 + deltax;
							ppolygon->m_cVertex[k].fy = fy1 + y0 + deltay;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipsefill )))
					{
						COb_schellipsefill* pellipsefill = (COb_schellipsefill*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ELLIPSEFILL;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schellipsefill.Copy(pellipsefill);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pellipsefill->m_fCx - x0, pellipsefill->m_fCy - y0, rotation, mirror);
						pellipsefill->m_fCx = fx1 + x0 + deltax;
						pellipsefill->m_fCy = fy1 + y0 + deltay;
						if((rotation == 90)||(rotation == 270))
						{
							double ftemp;
							ftemp = pellipsefill->m_fRadius_y;
							pellipsefill->m_fRadius_y = pellipsefill->m_fRadius_x;
							pellipsefill->m_fRadius_x = ftemp;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schpie )))
					{
						COb_schpie* ppie = (COb_schpie*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_PIE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schpie.Copy(ppie);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, ppie->m_fCx - x0, ppie->m_fCy - y0, rotation, mirror);
						ppie->m_fCx = fx1 + x0 + deltax;
						ppie->m_fCy = fy1 + y0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, ppie->m_fSangle, ppie->m_fEangle, rotation, mirror);
						ppie->m_fSangle = salpha;
						ppie->m_fSangle = pDoc->Get_Correct_Angle(ppie->m_fSangle);
						ppie->m_fEangle = ealpha;
						ppie->m_fEangle = pDoc->Get_Correct_Angle(ppie->m_fEangle);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schroundrect )))
					{
						COb_schroundrect* proundrect = (COb_schroundrect*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ROUNDRECT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schroundrect.Copy(proundrect);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX1 - x0, proundrect->m_fY1 - y0, rotation, mirror);
						proundrect->m_fX1 = fx1 + x0 + deltax;
						proundrect->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, proundrect->m_fX2 - x0, proundrect->m_fY2 - y0, rotation, mirror);
						proundrect->m_fX2 = fx1 + x0 + deltax;
						proundrect->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(proundrect->m_fX1, proundrect->m_fX2);
						fmaxx = MAX(proundrect->m_fX1, proundrect->m_fX2);
						fminy = MIN(proundrect->m_fY1, proundrect->m_fY2);
						fmaxy = MAX(proundrect->m_fY1, proundrect->m_fY2);
						proundrect->m_fX1 = fminx;
						proundrect->m_fY1 = fminy;
						proundrect->m_fX2 = fmaxx;
						proundrect->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schellipse )))
					{
						COb_schellipse* pellipse = (COb_schellipse*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ELLIPSE;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schellipse.Copy(pellipse);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pellipse->m_fCx - x0, pellipse->m_fCy - y0, rotation, mirror);
						pellipse->m_fCx = fx1 + x0 + deltax;
						pellipse->m_fCy = fy1 + y0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, pellipse->m_fSangle, pellipse->m_fEangle, rotation, mirror);
						pellipse->m_fSangle = salpha;
						pellipse->m_fSangle = pDoc->Get_Correct_Angle(pellipse->m_fSangle);
						pellipse->m_fEangle = ealpha;
						pellipse->m_fEangle = pDoc->Get_Correct_Angle(pellipse->m_fEangle);
						if((rotation == 90)||(rotation == 270))
						{
							double ftemp;
							ftemp = pellipse->m_fRadius_y;
							pellipse->m_fRadius_y = pellipse->m_fRadius_x;
							pellipse->m_fRadius_x = ftemp;
						}
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_scharc )))
					{
						COb_scharc* parc = (COb_scharc*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_ARC;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->scharc.Copy(parc);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, parc->m_fCx - x0, parc->m_fCy - y0, rotation, mirror);
						parc->m_fCx = fx1 + x0 + deltax;
						parc->m_fCy = fy1 + y0 + deltay;
						
						double salpha, ealpha;
						pDoc->Get_Rotate_Angle(&salpha, &ealpha, parc->m_fSangle, parc->m_fEangle, rotation, mirror);
						parc->m_fSangle = salpha;
						parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
						parc->m_fEangle = ealpha;
						parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schrect )))
					{
						COb_schrect* prect = (COb_schrect*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_RECT;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schrect.Copy(prect);
										
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX1 - x0, prect->m_fY1 - y0, rotation, mirror);
						prect->m_fX1 = fx1 + x0 + deltax;
						prect->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, prect->m_fX2 - x0, prect->m_fY2 - y0, rotation, mirror);
						prect->m_fX2 = fx1 + x0 + deltax;
						prect->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(prect->m_fX1, prect->m_fX2);
						fmaxx = MAX(prect->m_fX1, prect->m_fX2);
						fminy = MIN(prect->m_fY1, prect->m_fY2);
						fmaxy = MAX(prect->m_fY1, prect->m_fY2);
						prect->m_fX1 = fminx;
						prect->m_fY1 = fminy;
						prect->m_fX2 = fmaxx;
						prect->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtextframe )))
					{
						COb_schtextframe* ptextframe = (COb_schtextframe*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_TEXTFRAME;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schtextframe.Copy(ptextframe);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX1 - x0, ptextframe->m_fY1 - y0, rotation, mirror);
						ptextframe->m_fX1 = fx1 + x0 + deltax;
						ptextframe->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, ptextframe->m_fX2 - x0, ptextframe->m_fY2 - y0, rotation, mirror);
						ptextframe->m_fX2 = fx1 + x0 + deltax;
						ptextframe->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(ptextframe->m_fX1, ptextframe->m_fX2);
						fmaxx = MAX(ptextframe->m_fX1, ptextframe->m_fX2);
						fminy = MIN(ptextframe->m_fY1, ptextframe->m_fY2);
						fmaxy = MAX(ptextframe->m_fY1, ptextframe->m_fY2);
						ptextframe->m_fX1 = fminx;
						ptextframe->m_fY1 = fminy;
						ptextframe->m_fX2 = fmaxx;
						ptextframe->m_fY2 = fmaxy;
					}
					else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schgraphic )))
					{
						COb_schgraphic* pgraphic = (COb_schgraphic*)pobject;
						
						CSchUndo *pundo = new CSchUndo;
						pundo->m_nOp = UNDO_CHANGE;
						pundo->m_nOb = SCH_ELEMENT_GRAPHIC;
						pundo->m_nDiagram = m_nDiagram;
						pundo->m_nPos = subpos;
						pundo->schgraphic.Copy(pgraphic);
								
						pDoc->m_listSchUndo[pDoc->m_nSCH_Undo_Num].AddTail(pundo);
						
						pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX1 - x0, pgraphic->m_fY1 - y0, rotation, mirror);
						pgraphic->m_fX1 = fx1 + x0 + deltax;
						pgraphic->m_fY1 = fy1 + y0 + deltay;
						pDoc->Get_Rotate_XY(&fx1, &fy1, pgraphic->m_fX2 - x0, pgraphic->m_fY2 - y0, rotation, mirror);
						pgraphic->m_fX2 = fx1 + x0 + deltax;
						pgraphic->m_fY2 = fy1 + y0 + deltay;
						
						double fminx, fminy, fmaxx, fmaxy;
						fminx = MIN(pgraphic->m_fX1, pgraphic->m_fX2);
						fmaxx = MAX(pgraphic->m_fX1, pgraphic->m_fX2);
						fminy = MIN(pgraphic->m_fY1, pgraphic->m_fY2);
						fmaxy = MAX(pgraphic->m_fY1, pgraphic->m_fY2);
						pgraphic->m_fX1 = fminx;
						pgraphic->m_fY1 = fminy;
						pgraphic->m_fX2 = fmaxx;
						pgraphic->m_fY2 = fmaxy;
					}
				}
			}
			
			for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[m_nDiagram].GetNext(pos);
		}
	}
	
	if(undo_flag == TRUE)
	{
		pDoc->m_nSCH_Undo_Num++;
		if(pDoc->m_nSCH_Undo_Num >= SCH_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();
	}	
	
	Invalidate();
}

void CESDView::OnEditJump(UINT nID)
{
	int index = nID - ID_EDIT_JUMP_1;
	if(m_nSystem_State == SCH)
	{
		if(m_sJump[index].fx >=0 ) Change_Viewpoint(m_sJump[index].fx, m_sJump[index].fy, m_sJump[index].nDiagram);
	}
	else
	{
		if(m_sPcbJump[index].fx >=0 ) Change_Viewpoint(m_sPcbJump[index].fx, m_sPcbJump[index].fy,  0);
	}
}

void CESDView::OnEditSetJump(UINT nID)
{
	int index = nID - ID_EDIT_SETJUMP_1;

	unsigned ch1;
	char szMoveText[] = "设置光标跳转位置: <左键><回车>设置, <右键><ESC>取消";

	Write_Help(szMoveText);
	do
	{
       ch1 = MouseKey();
       if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
		   if(m_nSystem_State == SCH)
		   {
			   m_sJump[index].nDiagram = m_nDiagram;
			   m_sJump[index].fx = m_fCurrentSnapX;
			   m_sJump[index].fy = m_fCurrentSnapY;
		   }
		   else
		   {
			   m_sPcbJump[index].nDiagram = 0;
			   m_sPcbJump[index].fx = m_fCurrentSnapX;
			   m_sPcbJump[index].fy = m_fCurrentSnapY;
		   }
		   break;
	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;

}

void CESDView::OnEditJumpOrigin()
{
	m_fScreenX = m_fScreenY = 0;
	m_fCurrentSnapX = m_fScreenX;
	m_fCurrentSnapY = m_fScreenY;
	//Invalidate();	
	//Restore_LargeCrossXY();
	OnViewZoomCenter();
}

void CESDView::OnEditJumpLocation()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CDlg_Edit_JumpLocation dlg;
	if(dlg.DoModal() == IDOK)
	{
		if(m_nSystem_State == SCH)
		{
			m_fCurrentSnapX = pDoc->Convert_String_To_XY(dlg.m_csEditX);
			m_fCurrentSnapY = pDoc->Convert_String_To_XY(dlg.m_csEditY);
		}
		else
		{
			m_fCurrentSnapX = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditX) + pDoc->m_cPCB_Sheet.m_fOriginX;
			m_fCurrentSnapY = pDoc->Convert_PCB_String_To_XY(dlg.m_csEditY) + pDoc->m_cPCB_Sheet.m_fOriginY;
		}
		OnViewZoomCenter();
	}
}

void CESDView::OnEditJumpCurrentOrigin()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	m_fCurrentSnapX = pDoc->m_cPCB_Sheet.m_fOriginX;
	m_fCurrentSnapY = pDoc->m_cPCB_Sheet.m_fOriginY;
	OnViewZoomCenter();
}

void CESDView::OnEditJumpComponent()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	CDlg_Edit_JumpComp dlg;
	if(dlg.DoModal() == IDOK)
	{
		for(int diagram = 0; diagram < SCH_SHEET_NUM; diagram++)
		{
			
			if(dlg.m_nComboScope == 1) diagram = m_nDiagram;
		
			CObject* pobject;
			int num = pDoc->m_listSchObject[diagram].GetCount();
			POSITION pos;
			pos = pDoc->m_listSchObject[diagram].GetHeadPosition();
			for(int i=0; i<num; i++)
			{	
				if(pos == NULL) break;

				pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
				
				if(pobject->IsKindOf( RUNTIME_CLASS( COb_schcomp )))
				{
					
					COb_schcomp* pcomp = (COb_schcomp*)pobject;
					if(pcomp->m_nFlag != FLAG_NORMAL)
					{
						for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);

						continue;
					}

					for(int j=0; j<pcomp->m_arrayPos.GetSize(); j++)
					{
						pobject = pDoc->m_listSchObject[diagram].GetNext(pos);
						if(pobject->IsKindOf( RUNTIME_CLASS( COb_schtext )))
						{
							COb_schtext* ptext = (COb_schtext*)pobject;
							
							if(ptext->m_nID != TEXT_COMPDESIGNATOR) continue;
							CString str1, str2;
							str1 = ptext->m_csText;
							str2 = dlg.m_csEditText;
							str1.MakeUpper(); str2.MakeUpper();
							
							if( str1 == str2)
							{
							 	Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, ptext->m_nDiagram);
								return;
							}
						}
					}
				}
				else if(pobject->IsKindOf( RUNTIME_CLASS( COb_schsheetsymbol )))
				{
					COb_schsheetsymbol* psheetsymbol = (COb_schsheetsymbol*)pobject;
					for(int j=0; j<psheetsymbol->m_arrayPos.GetSize(); j++) pDoc->m_listSchObject[diagram].GetNext(pos);
				}
			}

			if(dlg.m_nComboScope == 1) break;
		}
		
		CString str;
		str.Format("%s 未找到.", dlg.m_csEditText);
		AfxMessageBox(str);
	}
}

void CESDView::OnPcbJumpComp()
{
	CDlg_PcbJumpComp dlg;
	dlg.csTitle = "元件名称";
	if(dlg.DoModal() == IDOK)
	{
		CString csCompID = dlg.m_csCompDesignator;
		if(csCompID.IsEmpty() != TRUE)
		{
			CESDDoc* pDoc = (CESDDoc*)GetDocument();
			for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
			{
				COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
				if(pcomp->m_nFlag != FLAG_NORMAL) continue;

				CString str = pcomp->GetPcbCompDesignator(i);
				if(csCompID.CompareNoCase(str) == 0) Change_Viewpoint(pcomp->m_fX, pcomp->m_fY, 0);
			}
		}
	}
}

void CESDView::OnPcbJumpText()
{
	CDlg_PcbJumpComp dlg;
	dlg.csTitle = "文字";
	if(dlg.DoModal() == IDOK)
	{
		CString csText = dlg.m_csCompDesignator;
		if(csText.IsEmpty() != TRUE)
		{
			CESDDoc* pDoc = (CESDDoc*)GetDocument();
			for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
				if(ptext->m_nFlag != FLAG_NORMAL) continue;

				if(csText.CompareNoCase(ptext->m_csText) == 0) Change_Viewpoint(ptext->m_fSx, ptext->m_fSy, 0);
			}
		}
	}
}