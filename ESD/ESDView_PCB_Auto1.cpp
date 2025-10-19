void CESDView::OnPcbToolLoadComponent()
{
	CArray<int, int> missingcomp;
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	if((pDoc->m_arraySchNet.GetCount() <= 0)||(pDoc->m_arrayCompDesignator.GetCount() <= 0))
	{
		AfxMessageBox("没有找到网络表, 请先完成电路图设计并且编译创建网络表。", MB_OK|MB_ICONSTOP);
		return;
	}

 	pDoc->SetModifiedFlag(TRUE);
	
	for(int i=0; i<pDoc->m_arrayCompDesignator.GetCount(); i++)
	{
		CString csID = pDoc->m_arrayCompDesignator.GetAt(i);

		char str[1000];		strcpy(str, "调入封装:");		strcat(str, csID);
		Write_Help(str);

		if(Package_Exist(csID) == TRUE) continue;

		if(Load_Package(i) == FALSE) missingcomp.Add(i);

	}

	if(missingcomp.GetCount() > 0 )
	{
		CDlg_Missing_Component dlg;
		char str[100];
		itoa(missingcomp.GetCount(), str, 10);
		dlg.csNum = str;
		for(int i=0; i<missingcomp.GetCount(); i++)
		{
			int index = missingcomp.GetAt(i);
			CString cstring;
			cstring = pDoc->m_arrayCompDesignator.GetAt(index);
			dlg.arrayCompID.Add(cstring);
			cstring = pDoc->m_arrayCompComment.GetAt(index);
			dlg.arrayCompC.Add(cstring);
			cstring = pDoc->m_arrayCompPackage.GetAt(index);
			dlg.arrayCompP.Add(cstring);
			cstring = pDoc->m_arrayPackageLibraryName.GetAt(index);
			dlg.arrayCompLib.Add(cstring);
		}

		dlg.DoModal();
	}
	else AfxMessageBox("调入完成, 未发现丢失封装.");

	Apply_Schnet_To_Pcb();

	Write_Help(szEDA_Help_Ready);

	Invalidate();
}

BOOL CESDView::Package_Exist(CString &csDesignator)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
		if(pcomp->m_nFlag == FLAG_DELETED) continue;
		
		if(pcomp->GetPcbCompDesignator(i) == csDesignator) return TRUE;
	}
	
	return FALSE;
}

BOOL CESDView::Load_Package(int nPartNo)
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	CString csDesignator = pDoc->m_arrayCompDesignator.GetAt(nPartNo);
	CString csComment = pDoc->m_arrayCompComment.GetAt(nPartNo);
	CString csPackage = pDoc->m_arrayCompPackage.GetAt(nPartNo);
	CString csPackageLibraryName = pDoc->m_arrayPackageLibraryName.GetAt(nPartNo);
	CString csDescription;

	Struct_CompList complist;

	complist.liboffset = -1;
	if(csPackageLibraryName.IsEmpty() != TRUE)
	{
		int pcblib_number;
		for(pcblib_number=0; pcblib_number<PCB_SYSTEM_LIB_NUMBER; pcblib_number++)
		{
			if(csPackageLibraryName.CompareNoCase(pcb_library_name[pcblib_number]) == 0) break;
		}
		
		if(pcblib_number < PCB_SYSTEM_LIB_NUMBER)
		{
			for(int i=0; i<arrayEDA_PcbLibraryCompName[pcblib_number].GetCount(); i++)
			{
				complist = arrayEDA_PcbLibraryCompName[pcblib_number].GetAt(i);
				if(csPackage.CompareNoCase(complist.compname) == 0) break;
			}
		}
	}

	if(complist.liboffset < 0)
	{
		for(int i=0; i<PCB_SYSTEM_LIB_NUMBER; i++)
		{
			for(int j=0; j<arrayEDA_PcbLibraryCompName[i].GetCount(); j++)
			{
				Struct_CompList tempcomplist;
				tempcomplist = arrayEDA_PcbLibraryCompName[i].GetAt(j);
				if(csPackage.CompareNoCase(tempcomplist.compname) == 0)
				{
					complist.basiclibrary = tempcomplist.basiclibrary;
					strcpy(complist.compname,tempcomplist.compname);
					complist.length = tempcomplist.length;
					complist.liboffset = tempcomplist.liboffset;
					complist.offset = tempcomplist.offset;
					break;
				}
			}
			if(complist.liboffset >= 0) break;
		}
	}

	if(complist.liboffset < 0)
	{
		for(int i=0; i<PCB_SYSTEM_LIB_NUMBER; i++)
		{
			for(int j=0; j<arrayEDA_PcbLibraryCompName[i].GetCount(); j++)
			{
				Struct_CompList tempcomplist;
				tempcomplist = arrayEDA_PcbLibraryCompName[i].GetAt(j);
				if(csPackage.CompareNoCase(tempcomplist.compname) == 0)
				{
					complist.basiclibrary = tempcomplist.basiclibrary;
					strcpy(complist.compname,tempcomplist.compname);
					complist.length = tempcomplist.length;
					complist.liboffset = tempcomplist.liboffset;
					complist.offset = tempcomplist.offset;
					break;
				}
			}
			if(complist.liboffset >= 0) break;
		}
		for(int i=0; i<arrayEDA_PcbUserLibraryName.GetCount(); i++)
		{
			CString userlibrary = arrayEDA_PcbUserLibraryName.GetAt(i);
			int lib_index = i;

			
			char libstring[MAX_PATH];
			sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, userlibrary);
			
			CFile cfileUserLibrary;
			if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE)  continue;

			CArchive ar(&cfileUserLibrary, CArchive::load);

			
			CString headformat;
			ar >> headformat;
			if(headformat != PCB_Library_Head)	{	ar.Close();	cfileUserLibrary.Close(); continue;	}

			int totalnum;
			ar >> totalnum;
			char comp_name[100];
			char comp_description[212];
			for(int j = 0; j < totalnum; j++)
			{
				ar.Read(comp_name, 100);
				ar.Read(comp_description, 212);
				int offset, length;
				ar >> offset;
				ar >> length;
				CString cstring = comp_name;
				if(csPackage.CompareNoCase(cstring) == 0)
				{
					complist.basiclibrary = FALSE;
					strcpy(complist.compname,cstring);
					complist.length = 0;
					complist.liboffset = lib_index;
					complist.offset = j;
					break;
				}
			}
			ar.Close();
			cfileUserLibrary.Close();
		}
	}

	if(complist.liboffset < 0)  return FALSE;


	int offset, length;
	unsigned char* pbuffer;

	if(complist.basiclibrary == TRUE)  
	{
		
		offset = complist.liboffset + complist.offset;
		length = complist.length;
		
		
		cfileEDA_PcbLibrary.Seek(complist.liboffset, CFile::begin);
		Struct_Library_Head libhead;
		cfileEDA_PcbLibrary.Read(&libhead, 32);
		for(int i=0; i<libhead.totalcompnum; i++)
		{
			cfileEDA_PcbLibrary.Seek(complist.liboffset + 32 + 320*i, CFile::begin);
			Struct_Comp_Head comp_head;
			cfileEDA_PcbLibrary.Read(&comp_head, 320);
			CString cstring = comp_head.name;
			if(strcmp(comp_head.name, complist.compname) == 0)
			{
				csDescription = comp_head.description;
				break;
			}
		}

		pbuffer = (unsigned char*)malloc(length + 100);

		cfileEDA_PcbLibrary.Seek(offset, CFile::begin);
		cfileEDA_PcbLibrary.Read(pbuffer, length);
	}
	else 
	{
		
		CString userlibrary, compname;
		userlibrary = arrayEDA_PcbUserLibraryName.GetAt(complist.liboffset);
		
		char libstring[MAX_PATH];
		sprintf(libstring,"%s\\pcblib\\%s",csEDA_System_Path, userlibrary);

		CFile cfileUserLibrary;
		if(cfileUserLibrary.Open(libstring, CFile::modeRead) != TRUE)  return FALSE;
		CArchive ar(&cfileUserLibrary, CArchive::load);
		
		CString headformat;
		ar >> headformat;
		if(headformat != PCB_Library_Head)	{	ar.Close();	cfileUserLibrary.Close();	return FALSE;	}
		int totalnum;
		ar >> totalnum;
		ar.Close();

		
		cfileUserLibrary.Seek(32 + 320*complist.offset, CFile::begin);

		Struct_Comp_Head comp_head;
		cfileUserLibrary.Read(&comp_head, 320);
		csDescription = comp_head.description;
		offset = comp_head.offset;
		length = comp_head.length;

		pbuffer = (unsigned char*)malloc(length + 100); 

		cfileUserLibrary.Seek(offset, CFile::begin);
		cfileUserLibrary.Read(pbuffer, length);

		cfileUserLibrary.Close();
	}

	CMemFile memfile(pbuffer, length);
	CArchive ar(&memfile,CArchive::load);
	
		
	
	COb_pcbcomp* pcomp = new COb_pcbcomp();
	pcomp->pDocument = pDoc;
	pcomp->m_fX = 0;
	pcomp->m_fY = 0;
	pcomp->m_fRotation = 0;
	pcomp->m_fHeight = 50;

	pcomp->m_nLayer = TOPLAYER;

	pcomp->m_bNameOn = cEDA_pcbcomp.m_bNameOn;
	//pcomp->m_nNamePosition = cEDA_pcbcomp.m_nNamePosition;
	pcomp->m_nNamePosition = LEFTTOP;
	pcomp->m_bCommentOn = cEDA_pcbcomp.m_bCommentOn;
	//pcomp->m_nCommentPosition = cEDA_pcbcomp.m_nCommentPosition;
	pcomp->m_nCommentPosition = LEFTTOP;

	pcomp->m_csPattern = complist.compname;
	pcomp->m_csFootprintDescription = csDescription;

	
	int index = pDoc->m_arrayPcbComp.Add(pcomp);

	int arcnum, tracknum, fillnum, padnum, vianum, textnum, regionnum;

	ar >> arcnum;
	for(int i=0; i<arcnum; i++)
	{
		COb_pcbarc* parc = new COb_pcbarc();
		parc->Serialize(ar);
		parc->pDocument = pDoc;
		parc->m_nComponent = index;
		pDoc->m_arrayPcbArc.Add(parc);
	}
	ar >> tracknum;
	for(int i=0; i<tracknum; i++)
	{
		COb_pcbtrack* ptrack = new COb_pcbtrack();
		ptrack->Serialize(ar);
		ptrack->pDocument = pDoc;
		ptrack->m_nComponent = index;
		pDoc->m_arrayPcbTrack.Add(ptrack);
	}
	ar >> fillnum;
	for(int i=0; i<fillnum; i++)
	{
		COb_pcbfill* pfill = new COb_pcbfill();
		pfill->Serialize(ar);
		pfill->pDocument = pDoc;
		pfill->m_nComponent = index;
		pDoc->m_arrayPcbFill.Add(pfill);
	}
	ar >> padnum;
	for(int i=0; i<padnum; i++)
	{
		COb_pcbpad* ppad = new COb_pcbpad();
		ppad->Serialize(ar);
		ppad->pDocument = pDoc;
		ppad->m_nComponent = index;
		pDoc->m_arrayPcbPad.Add(ppad);
	}
	ar >> vianum;
	for(int i=0; i<vianum; i++)
	{
		COb_pcbvia* pvia = new COb_pcbvia();
		pvia->Serialize(ar);
		pvia->pDocument = pDoc;
		pvia->m_nComponent = index;
		pDoc->m_arrayPcbVia.Add(pvia);
	}
	ar >> textnum;
	for(int i=0; i<textnum; i++)
	{
		COb_pcbtext* ptext = new COb_pcbtext();
		ptext->Serialize(ar);
		ptext->pDocument = pDoc;
		ptext->m_nComponent = index;
		pDoc->m_arrayPcbText.Add(ptext);
	}
	ar >> regionnum;
	for(int i=0; i<regionnum; i++)
	{
		COb_pcbregion* pregion = new COb_pcbregion();
		pregion->Serialize(ar);
		pregion->pDocument = pDoc;
		pregion->m_nComponent = index;
		pDoc->m_arrayPcbRegion.Add(pregion);
	}

	ar.Close();
	memfile.Close();
	free(pbuffer);

	COb_pcbtext* ptext = new COb_pcbtext();
	ptext->pDocument = pDoc;
	ptext->m_nComponent = index;
	ptext->m_nLayer = TOPOVERLAY;
	ptext->m_fSx = pcomp->m_fX;
	ptext->m_fSy = pcomp->m_fY + 100;
	ptext->m_bComment = FALSE;
	ptext->m_bDesignator = TRUE;

	ptext->m_fRotation = cEDA_pcbcomp_designator.m_fRotation;
	ptext->m_csText = csDesignator;
	ptext->m_fHeight = cEDA_pcbcomp_designator.m_fHeight;
	ptext->m_fWidth = cEDA_pcbcomp_designator.m_fWidth;
	ptext->m_bMirror = cEDA_pcbcomp_designator.m_bMirror;
	ptext->m_nStrokeFont = cEDA_pcbcomp_designator.m_nStrokeFont;
	ptext->m_bUseTTFont = cEDA_pcbcomp_designator.m_bUseTTFont;
	ptext->m_bBold = cEDA_pcbcomp_designator.m_bBold;
	ptext->m_bItalic = cEDA_pcbcomp_designator.m_bItalic;
	ptext->m_bInverted = cEDA_pcbcomp_designator.m_bInverted;
	ptext->m_csTTFontName = cEDA_pcbcomp_designator.m_csTTFontName;
	ptext->m_fInvertedBorder = cEDA_pcbcomp_designator.m_fInvertedBorder;

	pDoc->m_arrayPcbText.Add(ptext);

	ptext = new COb_pcbtext();
	ptext->pDocument = pDoc;
	ptext->m_nComponent = index;
	ptext->m_nLayer = TOPOVERLAY;
	ptext->m_fSx = pcomp->m_fX;
	ptext->m_fSy = pcomp->m_fY;
	ptext->m_bComment = TRUE;
	ptext->m_bDesignator = FALSE;

	ptext->m_fRotation = cEDA_pcbcomp_comment.m_fRotation;
	ptext->m_csText = csComment;
	ptext->m_fHeight = cEDA_pcbcomp_comment.m_fHeight;
	ptext->m_fWidth = cEDA_pcbcomp_comment.m_fWidth;
	ptext->m_bMirror = cEDA_pcbcomp_comment.m_bMirror;
	ptext->m_nStrokeFont = cEDA_pcbcomp_comment.m_nStrokeFont;
	ptext->m_bUseTTFont = cEDA_pcbcomp_comment.m_bUseTTFont;
	ptext->m_bBold = cEDA_pcbcomp_comment.m_bBold;
	ptext->m_bItalic = cEDA_pcbcomp_comment.m_bItalic;
	ptext->m_bInverted = cEDA_pcbcomp_comment.m_bInverted;
	ptext->m_csTTFontName = cEDA_pcbcomp_comment.m_csTTFontName;
	ptext->m_fInvertedBorder = cEDA_pcbcomp_comment.m_fInvertedBorder;

	pDoc->m_arrayPcbText.Add(ptext);
		
	pcomp->m_fX = fEDA_LoadPackage_X;
	pcomp->m_fY = fEDA_LoadPackage_Y;

	double fx0 = 0;
	double fy0 = 0;
	double fx1 = fEDA_LoadPackage_X;
	double fy1 = fEDA_LoadPackage_Y;
	double deltax = fx1 - fx0;
	double deltay = fy1 - fy0;

	if(pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].IsEmpty() == FALSE)
	{
		pDoc->Clear_Discarded_Undo_Buffer();
	}
	CPcbUndo *pundo = new CPcbUndo;
	pundo->m_nOp = UNDO_DELETE;
	pundo->m_nOb = PCB_ELEMENT_COMP;
	pundo->m_nIndex = index;
	pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

	for(int i=0; i<pDoc->m_arrayPcbArc.GetCount(); i++)
	{
		COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(i);
		if(parc->m_nComponent == index)
		{
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, parc->m_fCx - fx0, parc->m_fCy - fy0, 0,  FALSE);
			parc->m_fCx = fx + fx0 + deltax;
			parc->m_fCy = fy + fy0 + deltay;

			parc->m_fSangle = pDoc->Get_Correct_Angle(parc->m_fSangle);
			parc->m_fEangle = pDoc->Get_Correct_Angle(parc->m_fEangle);

			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_ARC;
			pundo->m_nIndex = i;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbTrack.GetCount(); i++)
	{
		COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(i);
		if(ptrack->m_nComponent == index)
		{
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, ptrack->m_fX1 - fx0, ptrack->m_fY1 - fy0, 0, FALSE);
			ptrack->m_fX1 = fx + fx0 + deltax;
			ptrack->m_fY1 = fy + fy0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, ptrack->m_fX2 - fx0, ptrack->m_fY2 - fy0, 0, FALSE);
			ptrack->m_fX2 = fx + fx0 + deltax;
			ptrack->m_fY2 = fy + fy0 + deltay;	

			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_TRACK;
			pundo->m_nIndex = i;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);
		}
	}
	for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
	{
		COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
		if(ppad->m_nComponent == index)
		{
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, ppad->m_fCx - fx0, ppad->m_fCy - fy0, 0, FALSE);
			ppad->m_fCx = fx + fx0 + deltax;
			ppad->m_fCy = fy + fy0 + deltay;
								
			ppad->m_fRotation = pDoc->Get_Correct_Angle(ppad->m_fRotation);

			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_PAD;
			pundo->m_nIndex = i;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

		}
	}
	for(int i=0; i<pDoc->m_arrayPcbFill.GetCount(); i++)
	{
		COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(i);
		if(pfill->m_nComponent == index)
		{
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, pfill->m_fX1 - fx0, pfill->m_fY1 - fy0, 0, FALSE);
			pfill->m_fX1 = fx + fx0 + deltax;
			pfill->m_fY1 = fy + fy0 + deltay;
			pDoc->Get_Rotate_XY(&fx, &fy, pfill->m_fX2 - fx0, pfill->m_fY2 - fy0, 0, FALSE);
			pfill->m_fX2 = fx + fx0 + deltax;
			pfill->m_fY2 = fy + fy0 + deltay;

			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_FILL;
			pundo->m_nIndex = i;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

		}
	}
	for(int i=0; i<pDoc->m_arrayPcbVia.GetCount(); i++)
	{
		COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(i);
		if(pvia->m_nComponent == index)
		{
			double fx, fy;
			pDoc->Get_Rotate_XY(&fx, &fy, pvia->m_fCx - fx0, pvia->m_fCy - fy0, 0, FALSE);
			pvia->m_fCx = fx + fx0 + deltax;
			pvia->m_fCy = fy + fy0 + deltay;

			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_VIA;
			pundo->m_nIndex = i;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

		}
	}
	
	double fcomp_x1, fcomp_y1, fcomp_x2, fcomp_y2, fcomp_x3, fcomp_y3, fcomp_x4, fcomp_y4;
	pcomp->GetPcbCompRect(&fcomp_x1, &fcomp_y1, &fcomp_x2, &fcomp_y2, &fcomp_x3, &fcomp_y3, &fcomp_x4, &fcomp_y4, index);
	CRect rect;
	rect.left = MIN(MIN(fcomp_x1, fcomp_x2), MIN(fcomp_x3, fcomp_x4));
	rect.right = MAX(MAX(fcomp_x1, fcomp_x2), MAX(fcomp_x3, fcomp_x4));
	rect.bottom = MIN(MIN(fcomp_y1, fcomp_y2), MIN(fcomp_y3, fcomp_y4));
	rect.top = MAX(MAX(fcomp_y1, fcomp_y2), MAX(fcomp_y3, fcomp_y4));

	for(int i=0; i<pDoc->m_arrayPcbText.GetCount(); i++)
	{
		COb_pcbtext* ptext = pDoc->m_arrayPcbText.GetAt(i);
		if(ptext->m_nComponent == index)
		{
			if(ptext->m_bDesignator == TRUE)
			{
				ptext->m_fSx = rect.left;
				ptext->m_fSy = rect.top + 150;

			}
			else if(ptext->m_bComment == TRUE)
			{
				ptext->m_fSx = rect.left;
				ptext->m_fSy = rect.top + 50;
			}
			else 
			{
				double fx, fy;
				pDoc->Get_Rotate_XY(&fx, &fy, ptext->m_fSx - fx0, ptext->m_fSy - fy0, 0, FALSE);
				ptext->m_fSx = fx + fx0 + deltax;
				ptext->m_fSy = fy + fy0 + deltay;

				ptext->m_fRotation = pDoc->Get_Correct_Angle(ptext->m_fRotation);
			}

			ptext->UpdateStringBorder(this, TRUE);

			CPcbUndo *pundo = new CPcbUndo;
			pundo->m_nOp = UNDO_DELETE;
			pundo->m_nOb = PCB_ELEMENT_TEXT;
			pundo->m_nIndex = i;
			pDoc->m_listPcbUndo[pDoc->m_nPCB_Undo_Num].AddTail(pundo);

		}
	}

	pDoc->m_nPCB_Undo_Num++;
	if(pDoc->m_nPCB_Undo_Num >= PCB_UNDO_NUMBER) pDoc->Undo_Buffer_Transfer();

	return TRUE;
}

void CESDView::OnPcbToolPlacementManual()
{
	CClientDC dc(this);
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	
	CArray<int, int> arraySequence;
	CArray<int, int> arrayArea;
	
	for(int i=0; i<pDoc->m_arrayPcbComp.GetCount(); i++)
	{
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(i);
		if((pcomp->m_nFlag == FLAG_NORMAL)&&(pcomp->m_fX == fEDA_LoadPackage_X)&&(pcomp->m_fY == fEDA_LoadPackage_Y))
		{
			arraySequence.Add(i);
			double fcomp_x1, fcomp_y1, fcomp_x2, fcomp_y2, fcomp_x3, fcomp_y3, fcomp_x4, fcomp_y4;
			pcomp->GetPcbCompRect(&fcomp_x1, &fcomp_y1, &fcomp_x2, &fcomp_y2, &fcomp_x3, &fcomp_y3, &fcomp_x4, &fcomp_y4, i);
			CRect rect;
			rect.left = MIN(MIN(fcomp_x1, fcomp_x2), MIN(fcomp_x3, fcomp_x4));
			rect.right = MAX(MAX(fcomp_x1, fcomp_x2), MAX(fcomp_x3, fcomp_x4));
			rect.bottom = MIN(MIN(fcomp_y1, fcomp_y2), MIN(fcomp_y3, fcomp_y4));
			rect.top = MAX(MAX(fcomp_y1, fcomp_y2), MAX(fcomp_y3, fcomp_y4));
			int area = abs(rect.right - rect.left)*abs(rect.top - rect.bottom);
			arrayArea.Add(area);
		}		
	}

	
	int judge = 1;
	for(;;)
	{
		if(judge == 1) judge = 0;
		else if(judge == 0) break;
		for(int i=0; i<arrayArea.GetCount()-1; i++)
		{
			int area1 = arrayArea.GetAt(i);
			int area2 = arrayArea.GetAt(i+1);

			if(area2 > area1)
			{
				
				judge=1;
				arrayArea.SetAt(i, area2);
				arrayArea.SetAt(i+1, area1);

				int index1 = arraySequence.GetAt(i);
				int index2 = arraySequence.GetAt(i+1);
				arraySequence.SetAt(i, index2);
				arraySequence.SetAt(i+1, index1);
			}
		}
	}

	bEDA_LoadPackageByName = FALSE;
	for(;;)
	{
		if(arraySequence.GetCount() <= 0)
		{
			AfxMessageBox("没找到需要布局的元件, 布局完成。");
			break;
		}
		
		int index = arraySequence.GetAt(0);
		COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(index);
		char str[100];
		strcpy(str, "当前元件:");
		strcat(str, pcomp->GetPcbCompDesignator(index).GetBuffer(50));
		strcat(str,"  按TAB键选择元件");
		Write_Info(str);

		if(pcomp->MovePlacePackage(&dc, this, index) == FALSE)
		{
			if(bEDA_LoadPackageByName == FALSE) break;
			else
			{
				bEDA_LoadPackageByName = FALSE;
				CDlg_PCB_LoadPackageByName dlg;
				dlg.m_csComboName = pcomp->GetPcbCompDesignator(index);
				for(int j=0; j<arraySequence.GetCount(); j++)
				{
					int subindex = arraySequence.GetAt(j);
					COb_pcbcomp* psubcomp = pDoc->m_arrayPcbComp.GetAt(subindex);
					dlg.arrayCompName.Add(psubcomp->GetPcbCompDesignator(subindex));
				}
				if(dlg.DoModal() == IDOK)
				{
					for(int j=0; j<arraySequence.GetCount(); j++)
					{
						int subindex = arraySequence.GetAt(j);
						COb_pcbcomp* psubcomp = pDoc->m_arrayPcbComp.GetAt(subindex);
						if(psubcomp->GetPcbCompDesignator(subindex).CompareNoCase(dlg.m_csComboName) == 0)
						{
							TRACE3("%s %s %d\n", psubcomp->GetPcbCompDesignator(subindex), dlg.m_csComboName, j);
							arraySequence.RemoveAt(j);
							arraySequence.InsertAt(0, subindex);
							break;
						}
					}
				}
				else break;
			}
		}
		else
		{
			arraySequence.RemoveAt(0);
		}
	}

	Refresh_Ratsnest();
}

void CESDView::Apply_Schnet_To_Pcb()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();

	
	while (!pDoc->m_arrayPcbNet.IsEmpty())
	{
		delete pDoc->m_arrayPcbNet.GetAt(0);
		pDoc->m_arrayPcbNet.RemoveAt(0);
	}

	
	for(int nNet=0; nNet<pDoc->m_arraySchNet.GetCount(); nNet++)
	{
		COb_schnet* pschnet = pDoc->m_arraySchNet.GetAt(nNet);
		
		char str[1000];		strcpy(str, "应用SCH网络:");		strcat(str, pschnet->m_csNetName);
		Write_Help(str);

		COb_pcbnet* ppcbnet = new COb_pcbnet();
		pDoc->m_arrayPcbNet.Add(ppcbnet);
		ppcbnet->pDocument = pDoc;
		ppcbnet->m_csNetName = pschnet->m_csNetName;

		for(int j=0; j<pschnet->m_arrayCompDesignator.GetCount(); j++)
		{
			CString csDesignator = pschnet->m_arrayCompDesignator.GetAt(j);
			CString csPinNumber = pschnet->m_arrayPinNumber.GetAt(j);

			for(int k=0; k<pDoc->m_arrayPcbPad.GetCount(); k++)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(k);
				if((ppad->m_nFlag != FLAG_NORMAL)||(ppad->m_nComponent < 0)) continue;

				COb_pcbcomp* pcomp = pDoc->m_arrayPcbComp.GetAt(ppad->m_nComponent);
				if((csPinNumber.CompareNoCase(ppad->m_csName) == 0)&&(csDesignator.CompareNoCase(pcomp->GetPcbCompDesignator(ppad->m_nComponent)) == 0))
				{
					ppad->m_nNet = nNet;
				}
			}
		}
	}
}


void CESDView::OnPcbToolRatsHideAll()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;

		pnet->m_bVisible = FALSE;
	}

	Invalidate();
}

void CESDView::OnPcbToolRatsHideComp()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	unsigned ch1;
	char szMoveText[] = "隐藏元件网络飞线: <左键><回车>选择元件, <右键><ESC>结束命令";
	MoveCursorToCenter();

	Write_Help(szMoveText);
	do{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			Struct_Pcb_Multi_Selection selected_element = SelectPcbObject(m_fCurrentX, m_fCurrentY, NOT_INCLUDE_CPAD);
			if(selected_element.pcbelement == PCB_ELEMENT_COMP)
			{
				for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
					if(ppad->m_nComponent == selected_element.index)
					{
						if(ppad->m_nNet >= 0)
						{
							COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(ppad->m_nNet);
							pnet->m_bVisible = FALSE;
							Refresh_Net_Ratsnest(ppad->m_nNet);
						}
					}
				}

				break;
			}
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;

	Invalidate();
}

void CESDView::OnPcbToolRatsHideNet()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);
	unsigned ch1;
	char szMoveText[] = "隐藏网络飞线: <左键><回车>选择网络, <右键><ESC>结束命令";
	MoveCursorToCenter();

	Write_Help(szMoveText);
	do{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			Struct_Pcb_Multi_Selection selected_element = SelectPcbObject(m_fCurrentX, m_fCurrentY, INCLUDE_CPAD);
			if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
				if(ptrack->m_nNet >= 0)
				{
					COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(ptrack->m_nNet);
					DrawPcbConnections(&dc);
					pnet->m_bVisible = FALSE;
					DrawPcbConnections(&dc);
				}
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
				if(parc->m_nNet >= 0)
				{
					COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(parc->m_nNet);
					DrawPcbConnections(&dc);
					pnet->m_bVisible = FALSE;
					DrawPcbConnections(&dc);
				}				
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
				if(ppad->m_nNet >= 0)
				{
					COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(ppad->m_nNet);
					DrawPcbConnections(&dc);
					pnet->m_bVisible = FALSE;
					DrawPcbConnections(&dc);
				}					
			}	
			else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
				if(pvia->m_nNet >= 0)
				{
					COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(pvia->m_nNet);
					DrawPcbConnections(&dc);
					pnet->m_bVisible = FALSE;
					DrawPcbConnections(&dc);
				}					
			}		
			else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
				if(pfill->m_nNet >= 0)
				{
					COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(pfill->m_nNet);
					DrawPcbConnections(&dc);
					pnet->m_bVisible = FALSE;
					DrawPcbConnections(&dc);
				}					
			}	
			else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
			{
				COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
				if(pregion->m_nNet >= 0)
				{
					COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(pregion->m_nNet);
					DrawPcbConnections(&dc);
					pnet->m_bVisible = FALSE;
					DrawPcbConnections(&dc);
				}					
			}	
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;

	Invalidate();
}

void CESDView::OnPcbToolRatsShowAll()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	
	for(int i=0; i<pDoc->m_arrayPcbNet.GetCount(); i++)
	{
		COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(i);
		if(pnet->m_nFlag != FLAG_NORMAL) continue;

		pnet->m_bVisible = TRUE;
	}

	Refresh_Ratsnest();
	Invalidate();
}

void CESDView::OnPcbToolRatsShowComp()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);

	unsigned ch1;
	char szMoveText[] = "显示元件网络飞线: <左键><回车>选择元件, <右键><ESC>结束命令";
	MoveCursorToCenter();

	Write_Help(szMoveText);
	do{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			Struct_Pcb_Multi_Selection selected_element = SelectPcbObject(m_fCurrentX, m_fCurrentY, NOT_INCLUDE_CPAD);
			if(selected_element.pcbelement == PCB_ELEMENT_COMP)
			{
				for(int i=0; i<pDoc->m_arrayPcbPad.GetCount(); i++)
				{
					COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(i);
					if(ppad->m_nComponent == selected_element.index)
					{
						if(ppad->m_nNet >= 0)
						{
							COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(ppad->m_nNet);
							if(pnet->m_nFlag == FLAG_NORMAL)
							{
								pnet->m_bVisible = TRUE;
								Refresh_Net_Ratsnest(ppad->m_nNet);
							}
						}
					}
				}
				break;
			}
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;

	Invalidate();
}

void CESDView::OnPcbToolRatsShowNet()
{
	CESDDoc* pDoc = (CESDDoc*)GetDocument();
	CClientDC dc(this);
	unsigned ch1;
	char szMoveText[] = "显示网络飞线: <左键><回车>选择网络, <右键><ESC>结束命令";
	MoveCursorToCenter();

	Write_Help(szMoveText);
	do{
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1== VK_RETURN))
		{
			Struct_Pcb_Multi_Selection selected_element = SelectPcbObject(m_fCurrentX, m_fCurrentY, INCLUDE_CPAD);
			if(selected_element.pcbelement == PCB_ELEMENT_TRACK)
			{
				COb_pcbtrack* ptrack = pDoc->m_arrayPcbTrack.GetAt(selected_element.index);
				if(ptrack->m_nNet >= 0)
				{
					COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(ptrack->m_nNet);
					pnet->m_bVisible = TRUE;
					Refresh_Net_Ratsnest(ptrack->m_nNet);
					DrawPcbConnections(&dc);
				}
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_ARC)
			{
				COb_pcbarc* parc = pDoc->m_arrayPcbArc.GetAt(selected_element.index);
				if(parc->m_nNet >= 0)
				{
					COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(parc->m_nNet);
					pnet->m_bVisible = TRUE;
					Refresh_Net_Ratsnest(parc->m_nNet);
					DrawPcbConnections(&dc);
				}				
			}
			else if(selected_element.pcbelement == PCB_ELEMENT_PAD)
			{
				COb_pcbpad* ppad = pDoc->m_arrayPcbPad.GetAt(selected_element.index);
				if(ppad->m_nNet >= 0)
				{
					COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(ppad->m_nNet);
					pnet->m_bVisible = TRUE;
					Refresh_Net_Ratsnest(ppad->m_nNet);
					DrawPcbConnections(&dc);
				}					
			}	
			else if(selected_element.pcbelement == PCB_ELEMENT_VIA)
			{
				COb_pcbvia* pvia = pDoc->m_arrayPcbVia.GetAt(selected_element.index);
				if(pvia->m_nNet >= 0)
				{
					COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(pvia->m_nNet);
					pnet->m_bVisible = TRUE;
					Refresh_Net_Ratsnest(pvia->m_nNet);
					DrawPcbConnections(&dc);
				}					
			}		
			else if(selected_element.pcbelement == PCB_ELEMENT_FILL)
			{
				COb_pcbfill* pfill = pDoc->m_arrayPcbFill.GetAt(selected_element.index);
				if(pfill->m_nNet >= 0)
				{
					COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(pfill->m_nNet);
					pnet->m_bVisible = TRUE;
					Refresh_Net_Ratsnest(pfill->m_nNet);
					DrawPcbConnections(&dc);
				}					
			}	
			else if(selected_element.pcbelement == PCB_ELEMENT_REGION)
			{
				COb_pcbregion* pregion = pDoc->m_arrayPcbRegion.GetAt(selected_element.index);
				if(pregion->m_nNet >= 0)
				{
					COb_pcbnet* pnet = pDoc->m_arrayPcbNet.GetAt(pregion->m_nNet);
					pnet->m_bVisible = TRUE;
					Refresh_Net_Ratsnest(pregion->m_nNet);
					DrawPcbConnections(&dc);
				}					
			}	
		}
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC));

	Write_Help(szEDA_Help_Ready);

	if(m_bCursorState == TRUE) CloseMouseCursor();
	ClipCursor(NULL);  m_bClipState = FALSE;

	Invalidate();
}
