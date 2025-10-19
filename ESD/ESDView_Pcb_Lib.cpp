void CESDView::OnPcbLibBrowse()
{
	CDlg_PcbLib_Browse dlg;
	dlg.m_bMatchMore = bEDA_PcbMatchMore;
	dlg.DoModal();

	bEDA_PcbMatchMore = dlg.m_bMatchMore;
}

void CESDView::OnPcbLibAdd()
{
	unsigned ch1;
	char szHelp[] = "封装库添加: <左键><回车>选择封装对象, <右键><ESC>结束命令";
	
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	Struct_Pcb_Multi_Selection multi_selection;
	multi_selection.pcbelement = -1;
	multi_selection.index = -1;
	Write_Help(szHelp);
	do
	{
       ch1 = MouseKey();
       if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
	   {
			multi_selection = SelectPcbObject(m_fCurrentX, m_fCurrentY, NOT_INCLUDE_CPAD);
			if(multi_selection.pcbelement == PCB_ELEMENT_COMP) ch1 = VK_ESCAPE;

	   }
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);
	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;	

	if( (multi_selection.pcbelement <= 0) || (multi_selection.index < 0) )  return;

	COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(multi_selection.index);

	CDlg_PcbLib_Add dlg;
	dlg.m_csEditName = pcomp->GetPcbCompComment(multi_selection.index);
	dlg.m_csEditDescription = pcomp->m_csFootprintDescription;
	if(dlg.DoModal() == IDOK)
	{
		if(dlg.m_nRadioLib == 0)
		{

			if(dlg.m_csEditName.IsEmpty() == TRUE)
			{
				AfxMessageBox("无法添加, 未定义元件名称.");
				return;
			}

			
			TRACE2("新库%s添加新元件%s\n",dlg.m_csLibName, dlg.m_csEditName);
			CString csLibraryName = dlg.m_csLibName;
			if(csLibraryName.Find(".lib") < 0) csLibraryName += ".lib";


			char libstring[MAX_PATH];
			sprintf(libstring,"%s\\pcblib\\element.tmp",csEDA_System_Path);
				
			CFile cfileTemp;
			if(cfileTemp.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE){AfxMessageBox("无法打开文件."); return;}
			CArchive temp_ar(&cfileTemp, CArchive::store);
			
			int arcnum=0, tracknum=0, fillnum=0, padnum=0, vianum=0, textnum=0, regionnum=0;
			for(int k=0; k<pDoc->m_arrayPcbArc.GetCount(); k++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(k);
				if((parc->m_nFlag == FLAG_NORMAL)&&(parc->m_nComponent == multi_selection.index)) arcnum++;
			}
			for(int k=0; k<pDoc->m_arrayPcbTrack.GetCount(); k++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(k);
				if((ptrack->m_nFlag == FLAG_NORMAL)&&(ptrack->m_nComponent == multi_selection.index)) tracknum++;
			}
			for(int k=0; k<pDoc->m_arrayPcbFill.GetCount(); k++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(k);
				if((pfill->m_nFlag == FLAG_NORMAL)&&(pfill->m_nComponent == multi_selection.index)) fillnum++;
			}
			for(int k=0; k<pDoc->m_arrayPcbPad.GetCount(); k++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(k);
				if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == multi_selection.index)) padnum++;
			}
			for(int k=0; k<pDoc->m_arrayPcbVia.GetCount(); k++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(k);
				if((pvia->m_nFlag == FLAG_NORMAL)&&(pvia->m_nComponent == multi_selection.index)) vianum++;
			}
			for(int k=0; k<pDoc->m_arrayPcbText.GetCount(); k++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(k);
				if((ptext->m_nFlag == FLAG_NORMAL)&&(ptext->m_nComponent == multi_selection.index)&&(ptext->m_bComment == FALSE)&&(ptext->m_bDesignator == FALSE)) textnum++;
			}

			int rotation = 0;

			double fx0, fy0, fx1, fy1, deltax, deltay;
			fx0 = pcomp->m_fX;
			fy0 = pcomp->m_fY;
			fx1 = fy1 = 0;
			deltax = fx1 - fx0;
			deltay = fy1 - fy0;
			BOOL mirror = FALSE; 
			int element_id;
			
			temp_ar << arcnum;
			for(int k=0; k<pDoc->m_arrayPcbArc.GetCount(); k++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(k);
				if((parc->m_nFlag == FLAG_NORMAL)&&(parc->m_nComponent == multi_selection.index))
				{
					COb_pcbarc* p_arc = new COb_pcbarc();
					p_arc->Copy(parc);
					
					p_arc->m_fCx -= pcomp->m_fX;
					p_arc->m_fCy -= pcomp->m_fY;

					p_arc->Serialize(temp_ar);
					delete p_arc;
				}
			}
			temp_ar << tracknum;
			for(int k=0; k<pDoc->m_arrayPcbTrack.GetCount(); k++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(k);
				if((ptrack->m_nFlag == FLAG_NORMAL)&&(ptrack->m_nComponent == multi_selection.index))
				{
					COb_pcbtrack* p_track = new COb_pcbtrack();
					p_track->Copy(ptrack);
					
					p_track->m_fX1 -= pcomp->m_fX;
					p_track->m_fY1 -= pcomp->m_fY;
					p_track->m_fX2 -= pcomp->m_fX;
					p_track->m_fY2 -= pcomp->m_fY;

					p_track->Serialize(temp_ar);
					delete p_track;
				}
			}
			temp_ar << fillnum;
			for(int k=0; k<pDoc->m_arrayPcbFill.GetCount(); k++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(k);
				if((pfill->m_nFlag == FLAG_NORMAL)&&(pfill->m_nComponent == multi_selection.index))
				{
					COb_pcbfill* p_fill = new COb_pcbfill();
					p_fill->Copy(pfill);
					
					p_fill->m_fX1 -= pcomp->m_fX;
					p_fill->m_fY1 -= pcomp->m_fY;
					p_fill->m_fX2 -= pcomp->m_fX;
					p_fill->m_fY2 -= pcomp->m_fY;

					p_fill->Serialize(temp_ar);
					delete p_fill;
				}
			}
			temp_ar << padnum;
			for(int k=0; k<pDoc->m_arrayPcbPad.GetCount(); k++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(k);
				if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == multi_selection.index))
				{
					COb_pcbpad* p_pad = new COb_pcbpad();
					p_pad->Copy(ppad);
					
					p_pad->m_fCx -= pcomp->m_fX;
					p_pad->m_fCy -= pcomp->m_fY;

					p_pad->Serialize(temp_ar);
					delete p_pad;
				}
			}
			temp_ar << vianum;
			for(int k=0; k<pDoc->m_arrayPcbVia.GetCount(); k++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(k);
				if((pvia->m_nFlag == FLAG_NORMAL)&&(pvia->m_nComponent == multi_selection.index))
				{
					COb_pcbvia* p_via = new COb_pcbvia();
					p_via->Copy(pvia);
					
					p_via->m_fCx -= pcomp->m_fX;
					p_via->m_fCy -= pcomp->m_fY;

					p_via->Serialize(temp_ar);
					delete p_via;
				}
			}
			temp_ar << textnum;
			for(int k=0; k<pDoc->m_arrayPcbText.GetCount(); k++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(k);
				if((ptext->m_nFlag == FLAG_NORMAL)&&(ptext->m_nComponent == multi_selection.index)&&(ptext->m_bComment == FALSE)&&(ptext->m_bDesignator == FALSE))
				{
					COb_pcbtext* p_text = new COb_pcbtext();
					p_text->Copy(ptext);
					
					p_text->m_fSx -= pcomp->m_fX;
					p_text->m_fSy -= pcomp->m_fY;
					p_text->m_fX1 -= pcomp->m_fX;
					p_text->m_fY1 -= pcomp->m_fY;
					p_text->m_fX2 -= pcomp->m_fX;
					p_text->m_fY2 -= pcomp->m_fY;

					p_text->Serialize(temp_ar);
					delete p_text;
				}
			}
			temp_ar << regionnum;

			temp_ar.Flush();
			temp_ar.Close();
			cfileTemp.Close();

			sprintf(libstring,"%s\\pcblib\\element.tmp",csEDA_System_Path);
			if(cfileTemp.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); return;}
			int elementfile_length = cfileTemp.GetLength();

			sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, csLibraryName);
			CFile cfileUserLibrary;
			if(cfileUserLibrary.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE)
			{
				AfxMessageBox("无法打开文件."); 
				cfileTemp.Close(); 
				return;
			}

			CArchive ar(&cfileUserLibrary, CArchive::store);
			CString headformat = PCB_Library_Head;
			ar << headformat;
			int totalnum = 1;
			ar << totalnum;
			ar.Flush();
			ar.Close();

			Struct_Comp_Head comp_head;
			CString tstr = dlg.m_csEditName;
			tstr.Left(99);
			strcpy(comp_head.name, tstr);
			tstr = dlg.m_csEditDescription;
			tstr.Left(210);
			strcpy(comp_head.description, tstr);
			comp_head.length = elementfile_length;
			comp_head.offset = 32 + 320;

			cfileUserLibrary.Write(&comp_head, 320);

			char buffer[1000];
			for(;;)
			{
				if(elementfile_length > 1000) 
				{
					cfileTemp.Read(buffer, 1000);
					cfileUserLibrary.Write(buffer, 1000);
					elementfile_length -= 1000;
				}
				else
				{
					cfileTemp.Read(buffer, elementfile_length);
					cfileUserLibrary.Write(buffer, elementfile_length);
					break;
				}
			}

			cfileTemp.Close();
			cfileUserLibrary.Close();

			arrayEDA_PcbUserLibraryName.Add(csLibraryName);
			arrayEDA_PcbLibraryCompList.RemoveAll();

			sprintf(libstring,"%s\\pcblib\\element.tmp",csEDA_System_Path);
			DeleteFile(libstring);
		}
		else
		{
			TRACE2("旧库%s添加新元件%s\n",dlg.m_csLibName, dlg.m_csEditName);
			if(dlg.m_csEditName.IsEmpty() == TRUE)
			{
				AfxMessageBox("无法添加, 未定义元件名称.");
				return;
			}
			CString csLibraryName = dlg.m_csLibName;
			if(csLibraryName.Find(".lib") < 0) csLibraryName += ".lib";

			char libstring[MAX_PATH];
			sprintf(libstring,"%s\\pcblib\\element.tmp",csEDA_System_Path);
				
			CFile cfileTemp;
			if(cfileTemp.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE){AfxMessageBox("无法打开文件."); return;}
			CArchive temp_ar(&cfileTemp, CArchive::store);
			
			
			int arcnum=0, tracknum=0, fillnum=0, padnum=0, vianum=0, textnum=0, regionnum=0;
			for(int k=0; k<pDoc->m_arrayPcbArc.GetCount(); k++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(k);
				if((parc->m_nFlag == FLAG_NORMAL)&&(parc->m_nComponent == multi_selection.index)) arcnum++;
			}
			for(int k=0; k<pDoc->m_arrayPcbTrack.GetCount(); k++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(k);
				if((ptrack->m_nFlag == FLAG_NORMAL)&&(ptrack->m_nComponent == multi_selection.index)) tracknum++;
			}
			for(int k=0; k<pDoc->m_arrayPcbFill.GetCount(); k++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(k);
				if((pfill->m_nFlag == FLAG_NORMAL)&&(pfill->m_nComponent == multi_selection.index)) fillnum++;
			}
			for(int k=0; k<pDoc->m_arrayPcbPad.GetCount(); k++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(k);
				if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == multi_selection.index)) padnum++;
			}
			for(int k=0; k<pDoc->m_arrayPcbVia.GetCount(); k++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(k);
				if((pvia->m_nFlag == FLAG_NORMAL)&&(pvia->m_nComponent == multi_selection.index)) vianum++;
			}
			for(int k=0; k<pDoc->m_arrayPcbText.GetCount(); k++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(k);
				if((ptext->m_nFlag == FLAG_NORMAL)&&(ptext->m_nComponent == multi_selection.index)&&(ptext->m_bComment == FALSE)&&(ptext->m_bDesignator == FALSE)) textnum++;
			}

			
			int rotation = 0;

			double fx0, fy0, fx1, fy1, deltax, deltay;
			fx0 = pcomp->m_fX;
			fy0 = pcomp->m_fY;
			fx1 = fy1 = 0;
			deltax = fx1 - fx0;
			deltay = fy1 - fy0;
			BOOL mirror = FALSE; 
			int element_id;
			
			temp_ar << arcnum;
			for(int k=0; k<pDoc->m_arrayPcbArc.GetCount(); k++)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(k);
				if((parc->m_nFlag == FLAG_NORMAL)&&(parc->m_nComponent == multi_selection.index))
				{
					COb_pcbarc* p_arc = new COb_pcbarc();
					p_arc->Copy(parc);
					
					p_arc->m_fCx -= pcomp->m_fX;
					p_arc->m_fCy -= pcomp->m_fY;

					p_arc->Serialize(temp_ar);
					delete p_arc;
				}
			}
			temp_ar << tracknum;
			for(int k=0; k<pDoc->m_arrayPcbTrack.GetCount(); k++)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(k);
				if((ptrack->m_nFlag == FLAG_NORMAL)&&(ptrack->m_nComponent == multi_selection.index))
				{
					COb_pcbtrack* p_track = new COb_pcbtrack();
					p_track->Copy(ptrack);
					
					p_track->m_fX1 -= pcomp->m_fX;
					p_track->m_fY1 -= pcomp->m_fY;
					p_track->m_fX2 -= pcomp->m_fX;
					p_track->m_fY2 -= pcomp->m_fY;

					p_track->Serialize(temp_ar);
					delete p_track;
				}
			}
			temp_ar << fillnum;
			for(int k=0; k<pDoc->m_arrayPcbFill.GetCount(); k++)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(k);
				if((pfill->m_nFlag == FLAG_NORMAL)&&(pfill->m_nComponent == multi_selection.index))
				{
					COb_pcbfill* p_fill = new COb_pcbfill();
					p_fill->Copy(pfill);
					
					p_fill->m_fX1 -= pcomp->m_fX;
					p_fill->m_fY1 -= pcomp->m_fY;
					p_fill->m_fX2 -= pcomp->m_fX;
					p_fill->m_fY2 -= pcomp->m_fY;

					p_fill->Serialize(temp_ar);
					delete p_fill;
				}
			}
			temp_ar << padnum;
			for(int k=0; k<pDoc->m_arrayPcbPad.GetCount(); k++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(k);
				if((ppad->m_nFlag == FLAG_NORMAL)&&(ppad->m_nComponent == multi_selection.index))
				{
					COb_pcbpad* p_pad = new COb_pcbpad();
					p_pad->Copy(ppad);
					
					p_pad->m_fCx -= pcomp->m_fX;
					p_pad->m_fCy -= pcomp->m_fY;

					p_pad->Serialize(temp_ar);
					delete p_pad;
				}
			}
			temp_ar << vianum;
			for(int k=0; k<pDoc->m_arrayPcbVia.GetCount(); k++)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(k);
				if((pvia->m_nFlag == FLAG_NORMAL)&&(pvia->m_nComponent == multi_selection.index))
				{
					COb_pcbvia* p_via = new COb_pcbvia();
					p_via->Copy(pvia);
					
					p_via->m_fCx -= pcomp->m_fX;
					p_via->m_fCy -= pcomp->m_fY;

					p_via->Serialize(temp_ar);
					delete p_via;
				}
			}
			temp_ar << textnum;
			for(int k=0; k<pDoc->m_arrayPcbText.GetCount(); k++)
			{
				COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(k);
				if((ptext->m_nFlag == FLAG_NORMAL)&&(ptext->m_nComponent == multi_selection.index)&&(ptext->m_bComment == FALSE)&&(ptext->m_bDesignator == FALSE))
				{
					COb_pcbtext* p_text = new COb_pcbtext();
					p_text->Copy(ptext);
					
					p_text->m_fSx -= pcomp->m_fX;
					p_text->m_fSy -= pcomp->m_fY;
					p_text->m_fX1 -= pcomp->m_fX;
					p_text->m_fY1 -= pcomp->m_fY;
					p_text->m_fX2 -= pcomp->m_fX;
					p_text->m_fY2 -= pcomp->m_fY;

					p_text->Serialize(temp_ar);
					delete p_text;
				}
			}
			temp_ar << regionnum;

			temp_ar.Flush();
			temp_ar.Close();
			cfileTemp.Close();

								
			sprintf(libstring,"%s\\pcblib\\element.tmp",csEDA_System_Path);
			CFile cfileTemp1;
			if(cfileTemp1.Open(libstring, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); return;}
			int elementfile_length = cfileTemp1.GetLength();
					
			
			sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, csLibraryName);
			char libstringbak[MAX_PATH];
			sprintf(libstringbak,"%s\\pcblib\\%stmp",csEDA_System_Path, csLibraryName);
			if(MoveFile(libstring, libstringbak) != TRUE) {AfxMessageBox("无法打开文件."); cfileTemp1.Close(); return; }

			CFile cfileTemp2;
			if(cfileTemp2.Open(libstringbak, CFile::modeRead) != TRUE){AfxMessageBox("无法打开文件."); cfileTemp1.Close(); return;}
			int libfile_length = cfileTemp2.GetLength();
			Struct_Library_Head libhead;
			cfileTemp2.Read(&libhead, 32);
			
			
			sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, csLibraryName);
			CFile cfileUserLibrary;
			if(cfileUserLibrary.Open(libstring, CFile::modeCreate|CFile::modeWrite) != TRUE)
			{
				AfxMessageBox("无法打开文件."); 
				cfileTemp1.Close();		cfileTemp2.Close();
				return;
			}

			CArchive ar(&cfileUserLibrary, CArchive::store);
			CString headformat = PCB_Library_Head;
			ar << headformat;
			int totalnum = libhead.totalcompnum + 1;
			ar << totalnum;
			ar.Flush();
			ar.Close();
					
			for(int i=0; i<libhead.totalcompnum; i++)
			{
				Struct_Comp_Head comp_head;
				cfileTemp2.Read(&comp_head, 320);
					
				comp_head.offset += 320;
				cfileUserLibrary.Write(&comp_head, 320);
			}

			Struct_Comp_Head comp_head;					
			CString tstr = dlg.m_csEditName;
			tstr.Left(99);
			strcpy(comp_head.name, tstr);
			tstr = dlg.m_csEditDescription;
			tstr.Left(210);
			strcpy(comp_head.description, tstr);
			comp_head.length = elementfile_length;
			comp_head.offset = libfile_length + 320;

			cfileUserLibrary.Write(&comp_head, 320);
			
			cfileTemp2.Seek(32 + 320*libhead.totalcompnum, CFile::begin);
			libfile_length -= 32 + 320*libhead.totalcompnum;

			char buffer[1000];
			for(;;)
			{
				if(libfile_length > 1000) 
				{
					cfileTemp2.Read(buffer, 1000);
					cfileUserLibrary.Write(buffer, 1000);
					libfile_length -= 1000;
				}
				else
				{
					cfileTemp2.Read(buffer, libfile_length);
					cfileUserLibrary.Write(buffer, libfile_length);
					break;
				}
			}

			for(;;)
			{
				if(elementfile_length > 1000) 
				{
					cfileTemp1.Read(buffer, 1000);
					cfileUserLibrary.Write(buffer, 1000);
					elementfile_length -= 1000;
				}
				else
				{
					cfileTemp1.Read(buffer, elementfile_length);
					cfileUserLibrary.Write(buffer, elementfile_length);
					break;
				}
			}
			cfileTemp1.Close();
			cfileTemp2.Close();
			cfileUserLibrary.Close();

			arrayEDA_PcbLibraryCompList.RemoveAll();

			
			sprintf(libstring,"%s\\pcblib\\element.tmp",csEDA_System_Path);
			DeleteFile(libstring);
			DeleteFile(libstringbak);

		}
	}
}

void CESDView::OnPcbLibDelete()
{
	CDlg_SchLib_SourceComp dlg;
	if(dlg.DoModal() == IDOK)
	{
		char s[1000];
		if(dlg.m_complist.liboffset < 0) return;

		CString userlibrary = arrayEDA_PcbUserLibraryName.GetAt(dlg.m_complist.liboffset);
		sprintf(s, "元件 %s 将从库 %s 中删除, 相关数据无法恢复, 确定?", dlg.m_complist.compname, userlibrary);
		if(AfxMessageBox(s, MB_YESNO|MB_ICONWARNING) != IDYES) return;
		
		
		char libstring[MAX_PATH], libbak[MAX_PATH];
		sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, userlibrary);
		sprintf(libbak,"%s\\pcblib\\%sbak",csEDA_System_Path, userlibrary);

		if(MoveFile(libstring, libbak) != TRUE) 
		{
			AfxMessageBox("操作无效, 无法移动库文件, 检查文件是否被其他程序锁定."); 
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

		
		int index;
		int deleted_length;
		CArray<int , int> arrayLength;
		
		int flag = 0;
		for(int i = 0; i < totalnum; i++)
		{
			Struct_Comp_Head comphead;
			cfileSource.Read(&comphead, 320);
			
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
	
		unsigned char* pbuffer = (unsigned char*)malloc(3000000); 

		
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
			arrayEDA_PcbUserLibraryName.RemoveAt(dlg.m_complist.liboffset);
			arrayEDA_PcbLibraryCompList.RemoveAll();
		}
	}
}

void CESDView::OnPcbLibEdit()
{
	char libstring[MAX_PATH], libbak[MAX_PATH], libtemp[MAX_PATH];
	int old_length, new_length;

	CDlg_SchLib_SourceComp dlg;
	if(dlg.DoModal() == IDOK)
	{
		
		if(dlg.m_complist.liboffset < 0) return;

		CString userlibrary = arrayEDA_PcbUserLibraryName.GetAt(dlg.m_complist.liboffset);
		sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, userlibrary);	
		
		CFile cfileSource;
		if(cfileSource.Open(libstring, CFile::modeReadWrite) != TRUE)
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
		for(int i = 0; i < totalnum; i++)
		{
			int fileseek = cfileSource.Seek(0, CFile::current);
			cfileSource.Read(&comphead, 320);
			if(strcmp(dlg.m_complist.compname, comphead.name) == 0)
			{
				
				CDlg_PcbLib_Attrib dlg_attrib;
				dlg_attrib.m_csEditName = comphead.name;
				dlg_attrib.m_csEditDescription = comphead.description;
				if(dlg_attrib.DoModal() == IDOK) 
				{
					dlg_attrib.m_csEditName.Left(99);
					strcpy(comphead.name, dlg_attrib.m_csEditName);
					dlg_attrib.m_csEditDescription.Left(210);
					strcpy(comphead.description, dlg_attrib.m_csEditDescription);
					cfileSource.Seek(fileseek, CFile::begin);

					cfileSource.Write(&comphead, 320);
				}
				break;
			}
		}		
		cfileSource.Close();
	}
}

void CESDView::OnPcbLibList()
{
	CDlg_SchLib_List dlg;
	if(dlg.DoModal() == IDOK)
	{
		CFileDialog importdialog(FALSE,"","", OFN_ENABLESIZING|OFN_OVERWRITEPROMPT,"文本文件(*.txt)|*.txt|所有文件 (*.*) |*.*||",NULL);  
		char filename[MAX_PATH];
		if(importdialog.DoModal() == IDOK) 
		{
			
			sprintf(filename,"%s\\pcblib\\%s",csEDA_System_Path, dlg.m_csLibraryName);	
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

			Struct_Library_Head libhead;
			cfileSource.Read(&libhead, 32);
			int totalnum = libhead.totalcompnum;	
			
			CString onelinestr;
			onelinestr = "封装库清单: ";
			onelinestr += dlg.m_csLibraryName;
			onelinestr += "\r\n";
			dest_ar.WriteString(onelinestr);

			onelinestr.Empty();
			onelinestr = "\r\n";
			onelinestr.Empty();
			onelinestr = "序号  名称                      注释\r\n";
			dest_ar.WriteString(onelinestr);

			onelinestr.Empty();
			onelinestr = "----------------------------------------------------------------------------------------------------------------------------------------\r\n";
			dest_ar.WriteString(onelinestr);
			
			Struct_Comp_Head comphead;
			for(int i = 0; i < totalnum; i++)
			{
				cfileSource.Seek(32 + 320*i, CFile::begin);
				cfileSource.Read(&comphead, 320);

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

				onelinestr += comphead.description;
				onelinestr += "\r\n";

				dest_ar.WriteString(onelinestr);
			}

			dest_ar.Flush();
			dest_ar.Close();
			cfileDest.Close();

			cfileSource.Close();

		}
	}
}