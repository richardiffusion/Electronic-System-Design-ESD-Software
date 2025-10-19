void CDesignView::OnUnrouteLocation() 
{
	int i,lmode,flag;
	unsigned ch1;
	unsigned x1,y1;
	char *pointer, *temp;
	char *temptrackbuf, *temppadbuf, *temparcfillbuf, *temppackbuf;
	struct htrack
	{ 
		char* pointer;             } highlighttrack;
	struct hjunc
	{
		char* pointer;             } highlightjunc;
	struct hpack
	{ 
		short int padnum;
		char* pointer;    } highlightpack;
	struct harcfill
	{
		char* pointer;             } highlightarcfill;

	struct Undo_Head head;
	char *undo_p;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(PCBHighlightBufferInit() != 0) return;
	temptrackbuf = Highlight_Trackbuf;
	temppadbuf = Highlight_Padbuf;
	temparcfillbuf = Highlight_ArcFillbuf;
	temppackbuf = Highlight_Packagebuf;
	DrawPCBHighlight(0);

	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(helpstr62);
	flag = 0;
	do 
	{
		ch1=MouseKey();
		if((ch1 == VK_RETURN)||(ch1==PUSHMOUSE))
		{
			for(i=0;i<HL_lnum;i++) Highlight_Trackbuf[i]=0;
			for(i=0;i<HL_pnum;i++) Highlight_Padbuf[i]=0;
			for(i=0;i<HL_afnum;i++) Highlight_ArcFillbuf[i]=0;
			for(i=0;i<HL_packnum;i++) Highlight_Packagebuf[i]=0;
			TempTrackbuf=Highlight_Trackbuf;
			TempPadbuf=Highlight_Padbuf;
			TempArcFillbuf=Highlight_ArcFillbuf;
			TempPackagebuf=Highlight_Packagebuf;
			temptrackbuf = Highlight_Trackbuf;
			temppadbuf = Highlight_Padbuf;
			temparcfillbuf = Highlight_ArcFillbuf;
			temppackbuf = Highlight_Packagebuf;

			x1 = m_nCurrent_X;
			y1 = m_nCurrent_Y;
			lmode = 0;
			if(Select_Line(x1,y1,&pointer) != 0) lmode = 1;
			else if(Select_JP(x1,y1,&pointer) != 0) lmode = 2;
			else if(Select_PackagePad(x1,y1,&pointer,&i)!=0) lmode = 3;
			else if(Select_Arc(x1,y1,&pointer) != 0) lmode = 4;
			else if(Select_Fill(x1,y1,&pointer) != 0) lmode = 5;
			if(lmode==0)
			{
				MessageBeep(0xFFFFFFFF);
				continue;
			}
			if(lmode==1)
			{
				memcpy(&gnline,pointer,16);
				if(gnline.layer>BOTTOMLAYER) 
				{
					MessageBeep(0xFFFFFFFF);
					continue;
				}
				highlighttrack.pointer = pointer;
				memcpy(TempTrackbuf,&highlighttrack,4);
				TempTrackbuf+=4;
				PCBMakeHighlight();
			}
			else if(lmode==2)
			{
				memcpy(&gnjunction,pointer,16);
				if((gnjunction.layer==MULTILAYER)||\
					(gnjunction.layer==TOPLAYER)||\
					(gnjunction.layer==BOTTOMLAYER)) {}
				else 
				{
					MessageBeep(0xFFFFFFFF);
					continue;
				}
				highlightjunc.pointer = pointer;
				memcpy(TempPadbuf,&highlightjunc,4);
				TempPadbuf+=4;
				PCBMakeHighlight();
			}
			else if(lmode==3)
			{
				highlightpack.pointer = pointer;
				highlightpack.padnum = i;
				memcpy(TempPackagebuf,&highlightpack,6);
				TempPackagebuf+=6;
				PCBMakeHighlight();
			}
			else if(lmode==4)
			{
				highlightarcfill.pointer = pointer;
				memcpy(TempArcFillbuf,&highlightarcfill,4);
				TempArcFillbuf += 4;
				PCBMakeHighlight();
			}
			else if(lmode==5)
			{
				highlightarcfill.pointer = pointer;
				memcpy(TempArcFillbuf,&highlightarcfill,4);
				TempArcFillbuf += 4;
				PCBMakeHighlight();
			}

			DrawPCBHighlight(0);
			temptrackbuf = Highlight_Trackbuf;
			pDoc->SetModifiedFlag(TRUE);
			for(;;)
			{
				memcpy(&highlighttrack,temptrackbuf,4);
				if(highlighttrack.pointer == NULL) break;
				else
				{
					pointer = highlighttrack.pointer;
					memcpy(&gnline,pointer,16);
					CloseMouseCursor();
					DrawStructLine(&gnline,BK_Color,&dc);
					OpenMouseCursor();
					gnline.flag = 0xff;
					memcpy(pointer,&gnline,16);
					
					flag = 1;
					Check_Undo_Buffer(10);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_UNDELETE;
					head.ob = UNDO_LINE;
					head.p = pointer;
					memcpy(undo_p,&head,6);
					pDoc->m_pPCB_Undo_Current += 6;
					
				}
				temptrackbuf += 4;
			}
			temppadbuf = Highlight_Padbuf;
			for(;;)
			{
				memcpy(&highlightjunc,temppadbuf,4);
				if(highlightjunc.pointer == NULL) break;
				else
				{
					pointer = highlightjunc.pointer;
					memcpy(&gnjunction,pointer,16);
					CloseMouseCursor();
					DrawStructJP(&gnjunction,BK_Color,0,&dc);
					OpenMouseCursor();
					gnjunction.flag=0xff;
					memcpy(pointer,&gnjunction,16);
					
					flag = 1;
					Check_Undo_Buffer(10);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_UNDELETE;
					head.ob = UNDO_JUNCTION;
					head.p = pointer;
					memcpy(undo_p,&head,6);
					pDoc->m_pPCB_Undo_Current += 6;
					
				}
				temppadbuf += 4;
			}
			temparcfillbuf = Highlight_ArcFillbuf;
			for(;;)
			{
				memcpy(&highlightarcfill,temparcfillbuf,4);
				if(highlightarcfill.pointer == NULL) break;
				else
				{
					pointer = highlightarcfill.pointer;
					memcpy(&gnarc,pointer,16);
					if(gnarc.id==0x60)
					{
						memcpy(&gnarc,pointer,16);
						CloseMouseCursor();
						DrawStructArc(&gnarc,BK_Color,&dc);
						OpenMouseCursor();
						gnarc.flag=0xff;
						memcpy(pointer,&gnarc,16);
						
						flag = 1;
						Check_Undo_Buffer(10);
						undo_p = pDoc->m_pPCB_Undo_Current;
						head.op = UNDO_UNDELETE;
						head.ob = UNDO_ARC;
						head.p = pointer;
						memcpy(undo_p,&head,6);
						pDoc->m_pPCB_Undo_Current += 6;
						
					}
					else if(gnarc.id==0x70)
					{
						memcpy(&gnfill,pointer,16);
						temp = (char*)&gnfill;
						memcpy(temp,pointer,gnfill.total16byte*16);
						CloseMouseCursor();
						DrawStructFillArea(&gnfill,BK_Color,&dc);
						OpenMouseCursor();
						pointer = highlightarcfill.pointer;
						gnfill.flag = 0xff;
						memcpy(pointer,&gnfill,16);
						
						flag = 1;
						Check_Undo_Buffer(10);
						undo_p = pDoc->m_pPCB_Undo_Current;
						head.op = UNDO_UNDELETE;
						head.ob = UNDO_FILL;
						head.p = pointer;
						memcpy(undo_p,&head,6);
						pDoc->m_pPCB_Undo_Current += 6;
						
					}
				}
				temparcfillbuf+=4;
			}
			if(flag != 0)
			{
				
				pDoc->m_nPCB_Undo_Num++;
				if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
				pDoc->m_nPCB_Redo_Num=0;
				pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
				
			}
		}
		else  Command_In_Function(ch1);
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));

	CL_Auto_Pan = OFF;
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	free(Highlight_Trackbuf);
	free(Highlight_Padbuf);
	free(Highlight_ArcFillbuf);
	free(Highlight_Packagebuf);
	Highlight_Trackbuf = NULL;
	Highlight_Padbuf = NULL;
	Highlight_ArcFillbuf = NULL;
	Highlight_Packagebuf = NULL;
	Write_Help(commandend);
}

void CDesignView::OnUnrouteNet() 
{
	int i,m,n,l,flag;
	int ch1;
	char *pointer, *temp, *temp1, *packmem, *p;
	char *temptrackbuf, *temppadbuf, *temparcfillbuf, *temppackbuf;
	struct htrack
	{ 
		char* pointer;             } highlighttrack;
	struct hjunc
	{ 
		char* pointer;             } highlightjunc;
	struct harcfill
	{ 
		char* pointer;             } highlightarcfill;
	struct hpack
	{ 
		short int padnum;
		char* pointer;    } highlightpack;
	char str1[30],str2[10];
	char **nets;
	char str[60];
	struct Undo_Head head;
	char *undo_p;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Number==0) 
	{
		Error( error16 );
		return;
	}
	if((nets = (char**)malloc(4000))==NULL)
	{
		memory_error();
		return;
	}
	temp = pDoc->m_pNet_Netnamebuf;
	for(i=0;i<pDoc->m_nNet_Number;i++)
	{
		nets[i] = temp;
		temp+=strlen(temp)+1;
	}

	CSelectionDlg selection;
	selection.m_TitleText = select_net;
	selection.m_pList = nets;
	selection.m_nNum = pDoc->m_nNet_Number;

	Write_Help("选择网络删除布线.");
	i = selection.DoModal();
	if(i != IDOK) 
	{
		free(nets);
		return ;     
	}
	strcpy(str, selection.m_Selected_Text);
   
	
	temp = pDoc->m_pNet_Netnamebuf;
	for(ch1=0;ch1<pDoc->m_nNet_Number;ch1++)
	{
		nets[ch1]=temp;
		if(strcmp(temp,str)==0) break;
		temp+=strlen(temp)+1;
	}
	free(nets);

	if(PCBHighlightBufferInit() != 0) return;

	temptrackbuf = Highlight_Trackbuf;
	temppadbuf = Highlight_Padbuf;
	temparcfillbuf = Highlight_ArcFillbuf;
	temppackbuf = Highlight_Packagebuf;

	temp1 = pDoc->m_ppNet_Pinspointer[ch1];

	if((packmem = (char*)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}

	pDoc->SetModifiedFlag(TRUE);
	for(;;)
	{
		if(temp1 == pDoc->m_ppNet_Pinspointer[ch1+1]) break;
		memcpy(str,temp1,strlen(temp1)+1);
		temp1+=strlen(temp1)+1;
		temp1+=2;
		n=strlen(str);
		for(m=n;m>=0;m--)
		{
			if(str[m]=='-')
			{
				memcpy(str1,str,m);
				str1[m]='\0';
				memcpy(str2,str+m+1,n-m-1);
				str2[n-m-1]='\0';
				break;
			}
		}
   
		p = pDoc->m_pPackage_Buffer;
		for(i=0; i<pDoc->m_nPackage_Number; i++)
		{
			pointer = p;
			memcpy(&gnpackage,pointer,16);
			if(gnpackage.flag==0)
			{
				temp = p;
				memcpy(temp,pointer,gnpackage.total16byte*16);
				temp = p;
				memcpy(&gnpackage,temp,112);
				if(strcmp(gnpackage.ID,str1)==0)
				{
					temp += 112;
					for(l=0;l<gnpackage.padnum;l++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						if(strcmp((char*)gnjunction.padname,str2)==0)
						{
							temppackbuf=Highlight_Packagebuf;
							highlightpack.pointer = p;
							highlightpack.padnum = l;
							memcpy(TempPackagebuf,&highlightpack,6);
							TempPackagebuf+=6;
							break;
						}
					}
				}
			}
			p += gnpackage.total16byte*16;
		}
	}

	PCBMakeHighlight();

	free(packmem);

	DrawPCBHighlight(0);

	flag = 0;
	temptrackbuf=Highlight_Trackbuf;
	for(;;)
    { 
		memcpy(&highlighttrack,temptrackbuf,4);
		if(highlighttrack.pointer==NULL) break;
		else
		{
			pointer = highlighttrack.pointer;
			memcpy(&gnline,pointer,16);
			CloseMouseCursor();
			DrawStructLine(&gnline,BK_Color,&dc);
			OpenMouseCursor();
			gnline.flag = 0xff;
			memcpy(pointer,&gnline,16);
			
			flag = 1;
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_UNDELETE;
			head.ob = UNDO_LINE;
			head.p = pointer;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			
		}
		temptrackbuf+=4;
	}
	temppadbuf=Highlight_Padbuf;
	for(;;)
    {
		memcpy(&highlightjunc,temppadbuf,4);
		if(highlightjunc.pointer == NULL) break;
		else
		{
			pointer = highlightjunc.pointer;
			memcpy(&gnjunction,pointer,16);
			CloseMouseCursor();
			DrawStructJP(&gnjunction,BK_Color,0,&dc);
			OpenMouseCursor();
			gnjunction.flag=0xff;
			memcpy(pointer,&gnjunction,16);
			
			flag = 1;
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_UNDELETE;
			head.ob = UNDO_JUNCTION;
			head.p = pointer;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			
		}
		temppadbuf+=4;
	}
	temparcfillbuf=Highlight_ArcFillbuf;
	for(;;)
    {
		memcpy(&highlightarcfill,temparcfillbuf,4);
		if(highlightarcfill.pointer == NULL) break;
		else
		{
			pointer = highlightarcfill.pointer;
			memcpy(&gnarc,pointer,16);
			if(gnarc.id==0x60)
			{
				memcpy(&gnarc,pointer,16);
				CloseMouseCursor();
				DrawStructArc(&gnarc,BK_Color,&dc);
				OpenMouseCursor();
				gnarc.flag=0xff;
				memcpy(pointer,&gnarc,16);
				
				flag = 1;
				Check_Undo_Buffer(10);
				undo_p = pDoc->m_pPCB_Undo_Current;
				head.op = UNDO_UNDELETE;
				head.ob = UNDO_ARC;
				head.p = pointer;
				memcpy(undo_p,&head,6);
				pDoc->m_pPCB_Undo_Current += 6;
				
			}
			else if(gnarc.id==0x70)
			{
				memcpy(&gnfill,pointer,16);
				temp = (char*)&gnfill;
				memcpy(temp,pointer,gnfill.total16byte*16);
				CloseMouseCursor();
				DrawStructFillArea(&gnfill,BK_Color,&dc);
				OpenMouseCursor();
				pointer = highlightarcfill.pointer;
				gnfill.flag = 0xff;
				memcpy(pointer,&gnfill,16);
				
				flag = 1;
				Check_Undo_Buffer(10);
				undo_p = pDoc->m_pPCB_Undo_Current;
				head.op = UNDO_UNDELETE;
				head.ob = UNDO_FILL;
				head.p = pointer;
				memcpy(undo_p,&head,6);
				pDoc->m_pPCB_Undo_Current += 6;
				
			}
		}
		temparcfillbuf+=4;
	}
	if(flag != 0)
	{
		
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
		
	}

	free(Highlight_Trackbuf);
	free(Highlight_Padbuf);
	free(Highlight_ArcFillbuf);
	free(Highlight_Packagebuf);
	Highlight_Trackbuf=NULL;
	Highlight_Padbuf=NULL;
	Highlight_ArcFillbuf=NULL;
	Highlight_Packagebuf=NULL;
	Write_Help(commandend);
}

void CDesignView::OnUnrouteAll() 
{
	int i,flag;
	char *p;
	struct Undo_Head head;
	char *undo_p;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	Write_Help("删除设计上所有走线");
	
	i = AfxMessageBox("所有走线将被删除，确认?", MB_YESNO|MB_ICONQUESTION);
	if(i != IDYES)	return;

	pDoc->SetModifiedFlag(TRUE);


	flag = 0;

	
	p = pDoc->m_pLine_Buffer;
	for(i=0; i<pDoc->m_nLine_Number; i++)
	{	
		memcpy(&gnline,p,16);
		if((gnline.attrib==PCB)&&(gnline.flag==0)&&(gnline.layer<=BOTTOMLAYER))
		{
			gnline.flag = 0xff;
			memcpy(p,&gnline,16);
			
			flag = 1;
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_UNDELETE;
			head.ob = UNDO_LINE;
			head.p = p;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			
		}
		p+=16;
	}


	
 	p = pDoc->m_pJunction_Buffer;
	for(i=0; i<pDoc->m_nJunction_Number; i++)
	{	
		memcpy(&gnjunction,p,16);
		if((gnjunction.attrib==PCB)&&(gnjunction.flag==0))
		{
			gnjunction.flag = 0xff;
			memcpy(p,&gnjunction,16);
			
			flag = 1;
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_UNDELETE;
			head.ob = UNDO_JUNCTION;
			head.p = p;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			
		}
		p+=16;
	}


	
 	p = pDoc->m_pArc_Buffer;
	for(i=0; i<pDoc->m_nArc_Number; i++)
	{	
		memcpy(&gnarc,p,16);
		if((gnarc.attrib==PCB)&&(gnarc.flag==0)&&(gnarc.layer<=BOTTOMLAYER))
		{
			gnarc.flag=0xff;
			memcpy(p,&gnarc,16);
			
			flag = 1;
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_UNDELETE;
			head.ob = UNDO_ARC;
			head.p = p;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			
		}
		p+=16;
	}
	
	p = pDoc->m_pFill_Buffer;
	for(i=0; i<pDoc->m_nFill_Number; i++)
	{	
		memcpy(&gnfill,p,16);
		if((gnfill.attrib==PCB)&&(gnfill.flag==0)&&(gnfill.layer<=BOTTOMLAYER))
		{
			gnfill.flag=0xff;
			memcpy(p,&gnfill,16);
			
			flag = 1;
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_UNDELETE;
			head.ob = UNDO_FILL;
			head.p = p;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			
		}
		p += gnfill.total16byte*16;
	}
	
	if(flag != 0)
	{
		
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
		
	}

	Invalidate();
	Write_Help(commandend);
}

void CDesignView::OnPlaceAll() 
{
	unsigned ch1, flag, undoflag;
	unsigned x1,y1;
	char *p, *pointer,*temp,*packagemem, *packagebak;
	struct Package movepackage;
	int i,ii,k,l,pstate,length,invalidflag,statebak;
	int tmax_x,tmin_x,tmax_y,tmin_y;
	char cbak;
	int ratsnestbak, routemodebak;
	unsigned ccx,ccy,ccx0,ccy0,ccdeltax,ccdeltay,ccstate,ccstatebak;
	unsigned optix,optiy,placeflag;
	unsigned long optilength, optilengthbak;
	struct Package *onepackage, *abcpack;
	unsigned px1,px2,py1,py2,tempxy;
	int xs,ys,xe,ye, placementstate, increasenum;
	struct Undo_Head head;
	char *undo_p;

	CClientDC dc(this);
	int drawmode = dc.GetROP2();
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Number==0) 
	{
		Error( error16 );
	    return;
	}

	DrawPCBHighlight(0);
	free(Highlight_Trackbuf);
	free(Highlight_Padbuf);
	free(Highlight_ArcFillbuf);
	free(Highlight_Packagebuf);
	Highlight_Trackbuf = NULL;
	Highlight_Padbuf = NULL;
	Highlight_ArcFillbuf = NULL;
	Highlight_Packagebuf = NULL;

	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(helpstr1);

	flag=0;
	px1 = px2 = 0;
	py1 = py2 = 0;
	do
	{
		ch1 = MouseKey();
		if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
		{
			px1 = m_nCurrent_X;
			py1 = m_nCurrent_Y;
			Get_VPort_XY(&xs,&ys,px1,py1);
			xe = xs;
			ye = ys;
			//CloseMouseCursor();
			Write_Help(helpstr2);
			//OpenMouseCursor();
			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xs,&ys,px1,py1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
				{
					CL_Auto_Pan = OFF;
					DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
					CloseMouseCursor();
					//setwritemode(1);
					//setcolor(Overwrite_Color);
					//rectangle(xs,ys,xe,ye);
					dc.SetROP2(R2_NOTXORPEN);
					rectangle(&dc,xs,ys,xe,ye);
					dc.SetROP2(drawmode);
					OpenMouseCursor();
					return;
				}
				if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{ 
						flag = 1;
						CloseMouseCursor();
						//setwritemode(1);
						//setcolor(Overwrite_Color);
						//rectangle(xs,ys,xe,ye);
						dc.SetROP2(R2_NOTXORPEN);
						rectangle(&dc,xs,ys,xe,ye);
						dc.SetROP2(drawmode);
						OpenMouseCursor();
					}
					else
					{
						if(CL_RedrawFlag==1)
						{
							CL_RedrawFlag=0;
						}
						else
						{
							//setwritemode(1);
							//setcolor(Overwrite_Color);
							//rectangle(xs,ys,xe,ye);      
							dc.SetROP2(R2_NOTXORPEN);
							rectangle(&dc,xs,ys,xe,ye);
							dc.SetROP2(drawmode);
						}
						px2 = m_nCurrent_X;
						py2 = m_nCurrent_Y;
						Get_VPort_XY(&xe,&ye,px2,py2);
						CloseMouseCursor();
						//setwritemode(1);
						//setcolor(Overwrite_Color);
						//rectangle(xs,ys,xe,ye);
						dc.SetROP2(R2_NOTXORPEN);
						rectangle(&dc,xs,ys,xe,ye);
						dc.SetROP2(drawmode);
						OpenMouseCursor();
					}
				}
				if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
				{
					ch1 = VK_ESCAPE;
					CloseMouseCursor();
					//setwritemode(1);
					//setcolor(Overwrite_Color);
					//rectangle(xs,ys,xe,ye);
					dc.SetROP2(R2_NOTXORPEN);
					rectangle(&dc,xs,ys,xe,ye);
					dc.SetROP2(drawmode);
					OpenMouseCursor();
					break;
				}
				else Command_In_Function(ch1);
			}while(1);
		}
		else  Command_In_Function(ch1);
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));

    if((px1==px2)||(py1==py2)) 
	{
		CL_Auto_Pan = OFF;
		DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
		return ;
	}
	if(px1>px2)
	{
		tempxy = px2;
		px2 = px1;
		px1 = tempxy;
	}
	if(py1>py2)
	{
		tempxy = py2;
		py2 = py1;
		py1 = tempxy;
	}

	ccdeltax = CL_Autoplace_Dx;
	ccdeltay = CL_Autoplace_Dy;

	CL_Auto_Pan = OFF;
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);

	pDoc->SetModifiedFlag(TRUE);

	
	undoflag = 0;
	p = pDoc->m_pPackage_Buffer;
	for(i=0; i<pDoc->m_nPackage_Number; i++)
	{
		onepackage = (struct Package*)p;
	    if((onepackage->flag==0)&&(onepackage->u1==0))
		{
			temp = p;
			x1 = 12345 - onepackage->cx; 
			y1 = 12345 - onepackage->cy;
			
			undoflag = 1;
			Check_Undo_Buffer(30);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_MOVE;
			head.ob = UNDO_PACKAGE;
			head.p = p;
			head.dx = x1;
			head.dy = y1;
			head.state1 = (unsigned char)onepackage->angle;
			head.state2 = head.state1;
			memcpy(undo_p,&head,16);
			pDoc->m_pPCB_Undo_Current += 16;
			
			Check_Undo_Buffer(30);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_MOVE;
			head.ob = UNDO_PACKLIMMIT;
			head.p = p;
			head.dx = x1;
			head.dy = y1;
			head.size1 = x1;
			head.size2 = y1;
			memcpy(undo_p,&head,16);
			pDoc->m_pPCB_Undo_Current += 16;
			
			Check_Undo_Buffer(30);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_MOVE;
			head.ob = UNDO_PACKID;
			head.p = p;
			head.dx = x1;
			head.dy = y1;
			head.size1 = onepackage->IDsize;
			head.size2 = onepackage->IDsize;
			head.state1 = onepackage->IDalign;
			head.state2 = onepackage->IDalign;
			memcpy(undo_p,&head,16);
			pDoc->m_pPCB_Undo_Current += 16;
			
			Check_Undo_Buffer(30);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_MOVE;
			head.ob = UNDO_PACKCOMMENT;
			head.p = p;
			head.dx = x1;
			head.dy = y1;
			head.size1 = onepackage->Csize;
			head.size2 = onepackage->Csize;
			head.state1 = onepackage->Calign;
			head.state2 = onepackage->Calign;
			memcpy(undo_p,&head,16);
			pDoc->m_pPCB_Undo_Current += 16;
			
			onepackage->cx = 12345;
			onepackage->cy = 12345;
			onepackage->IDx += x1;
			onepackage->IDy += y1;
			onepackage->Cx += x1;
			onepackage->Cy += y1;
			onepackage->Px += x1;
			onepackage->Py += y1;
			onepackage->x  += x1;
			onepackage->y  += y1;
			temp += 112;
			for(l=0;l<onepackage->padnum;l++)
			{
				memcpy(&gnjunction,temp,16);
				gnjunction.centerx += x1;
				gnjunction.centery += y1;
				memcpy(temp,&gnjunction,16);
				temp+=16;
			}
			for(l=0;l<onepackage->lnum;l++)
			{
				memcpy(&gnline,temp,16);
				gnline.startx += x1;
				gnline.starty += y1;
				gnline.endx += x1;
				gnline.endy += y1;
				memcpy(temp,&gnline,16);
				temp+=16;
			}
			for(l=0;l<onepackage->jnum;l++)
			{
				memcpy(&gnjunction,temp,16);
				gnjunction.centerx += x1;
				gnjunction.centery += y1;
				memcpy(temp,&gnjunction,16);
				temp+=16;
			}
			for(l=0;l<onepackage->snum;l++)
			{
				memcpy(&gnstring,temp,64);
				gnstring.sx += x1;
				gnstring.sy += y1;
				memcpy(temp,&gnstring,64);
				temp+=64;
			}
			for(l=0;l<onepackage->anum;l++)
			{
				memcpy(&gnarc,temp,16);
				gnarc.centerx += x1;
				gnarc.centery += y1;
				memcpy(temp,&gnarc,16);
				temp+=16;
			}
			for(l=0;l<onepackage->fnum;l++)
			{
				memcpy(&gnfill,temp,16);
				memcpy(&gnfill,temp,gnfill.total16byte*16);
				for(k=0;k<gnfill.vertexnum;k++)
				{
					gnfill.pline[k].x += x1;
					gnfill.pline[k].y += y1;
				}
				memcpy(temp,&gnfill,gnfill.total16byte*16);
				temp+=gnfill.total16byte*16;
			}
	    }
		p += onepackage->total16byte*16;
	}
	
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	

	Invalidate();
	SendMessage(WM_PAINT);

	

	if((packagemem = (char *)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}
	if((packagebak = (char *)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}
	

	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(autoplacement);

	if(Ratsnestflag!=0) 
	{
		Show_Ratsnest(&dc);
		routemodebak = Route_Mode;
		Route_Mode = 0;
	}
	else 
	{
		ratsnestbak = Ratsnestflag;
		Ratsnestflag = 1;
		routemodebak = Route_Mode;
		Route_Mode = 0;
	}

	EnableFrame(FALSE);
	undoflag = 0;
	pDoc->Get_Package_Table();
	for(;;)
	{
		if(Get_Package_In_Sequence(&pointer) == -1) break;
		temp = pointer;
		DrawDeletePackage(temp,&dc);
		//CloseMouseCursor();
		memcpy(&movepackage,pointer,16);
		temp = packagemem;
		memcpy(temp,pointer,movepackage.total16byte*16);
		memcpy(packagebak,packagemem,movepackage.total16byte*16);
		temp = packagemem;
		memcpy(&movepackage,temp,112);
		x1 = movepackage.cx;
		y1 = movepackage.cy;
		movepackage.cx = 100;
		movepackage.cy = 100;
		memcpy(pointer,&movepackage,16);
		pDoc->Fresh_Ratsnest_Package(movepackage.ID);
		movepackage.cx = x1;
		movepackage.cy = y1;
		movepackage.flag = 0xcc;  
		memcpy(pointer,&movepackage,16);
		pstate = movepackage.angle;
		x1 = movepackage.cx;
		y1 = movepackage.cy;


	    temp = packagemem;
	    memcpy(&movepackage,temp,112);
		temp += 112;
		for(k=0;k<movepackage.padnum;k++)
		{
			memcpy(&gnjunction,temp,16);
		    temp+=16;
		    if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
			   DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
		}
		for(k=0;k<movepackage.lnum;k++)
		{
			memcpy(&gnline,temp,16);
		    temp+=16;
		    DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,Highlighted_Color,1,&dc);
		}
	    for(k=0;k<movepackage.jnum;k++)
		{
			memcpy(&gnjunction,temp,16);
			temp+=16;
		}
		for(k=0;k<movepackage.snum;k++)
		{
			memcpy(&gnstring,temp,64);
			temp+=64;
		}
		for(k=0;k<movepackage.anum;k++)
		{
			memcpy(&gnarc,temp,16);
			gnarc.centerx=gnarc.centerx-x1;
			gnarc.centery=gnarc.centery-y1;
			memcpy(temp,&gnarc,16);
			temp+=16;
		}
		for(k=0;k<movepackage.fnum;k++)
		{
			memcpy(&gnfill,temp,16);
			temp+=gnfill.total16byte*16;
		}

	    if(Ratsnestflag!=0)
		{
			//Ratsnestflag=3;
		    //strcpy(Ratsneststring,movepackage.ID);
			//Show_Ratsnest();
		}
		
		//CL_Auto_Pan = ON;
		//Mouse_In_Work_Area();
		//DrawStructBut_2(But_move);
		//OpenMouseCursor();
		CL_RedrawFlag = 0;

	    flag = 0;
	    tmax_x = 0; tmin_x = CL_Size_Maxx; tmax_y = 0; tmin_y = CL_Size_Maxy;
	    ccx = ccx0 = ((int)ceil((float)px1/25))*25;
	    ccy = ccy0 = ((int)ceil((float)py1/25))*25;
	    increasenum = optilengthbak = 0;
	    //ccdeltax=ccdeltay=200;
		ccstate = pstate;
		ccstatebak = 0;
		ch1 = 1;
		optilength = 1234567890;
		placeflag = 0;
		placementstate = 0;
	    do
		{
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptplacement, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						ch1 = VK_ESCAPE;
					}
				}
				break;
			}
		    
			if(ch1==VK_RETURN) ch1=VK_ESCAPE;

		    
			else if((ch1==0)||(ch1==1))
			{
			   
				if(ch1==0)
			    {
					if(optilength>Ratsnest_Length)
				    {
						optilength=Ratsnest_Length;
						optix=ccx; optiy=ccy;
						ccstate=pstate;
						placeflag=1;
						increasenum=0;
				    }
					else if(optilengthbak<Ratsnest_Length)
				    {
						increasenum++;
					}
					else increasenum=0;
					optilengthbak=Ratsnest_Length;
				}

				
				if((placementstate==3)||(placeflag==2))
				{
					if(ccstatebak<3)
					{
						ccx=ccx0=((int)ceil((float)px1/25))*25;
						ccy=ccy0=((int)ceil((float)py1/25))*25;
						increasenum=optilengthbak=0;
						ch1=1;
						placementstate=0;
						ccstatebak++;
						ch1=SPACE;
					}
					else
					{
						if(placeflag==0) ch1 = VK_ESCAPE;
						else if(placeflag==1)
						{
							m_nCurrent_X = optix;
							m_nCurrent_Y = optiy;
							pstate = ccstate;
							ch1 = MOUSEMOVE;
							placeflag = 2;
						}
						else ch1 = VK_RETURN;
					}
				}
			    else 
				{
					
					ccx+=ccdeltax;

				    
				    if((placementstate==2)||(increasenum>3))
					{
						ccx=ccx0;
						ccy+=ccdeltay;
						increasenum=0;
					}

				    
				    m_nCurrent_X = ccx;
				    m_nCurrent_Y = ccy;
				    ch1 = MOUSEMOVE;
				 }
			}
			else if(ch1==MOUSEMOVE)
			{
				placementstate=Placement_Valid1(packagemem,movepackage.padnum,movepackage.lnum,px1,py1,px2,py2);
			    if((placementstate==0)&&(Placement_Valid2(packagemem,movepackage.padnum,movepackage.lnum)==0))
				{
					ch1 = 0;
				}
				else ch1 = 1;
			}
			
			else if(ch1==SPACE) ch1=1;

		    if(ch1==VK_ESCAPE)
			{
				
			    temp = packagemem;
			    temp += 112;
			    for(k=0;k<movepackage.padnum;k++)
				{
					memcpy(&gnjunction,temp,16);
					temp+=16;
					if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
					DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
				}
				for(k=0;k<movepackage.lnum;k++)
				{
					memcpy(&gnline,temp,16);
					temp+=16;
					DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
				}
			    if((flag!=0)&&(Ratsnestflag!=0)) Show_Ratsnest(&dc);

			    temp = packagebak;
				memcpy(pointer,temp,movepackage.total16byte*16);
				DrawPackage(pointer,&dc);
				if(Ratsnestflag!=0) 
				{
					Show_Ratsnest(&dc); 
					flag = 1;
				}
				
				goto LIMOVEEND;
			}
			if(ch1==MOUSEMOVE)
			{
				if(CL_RedrawFlag==1)
				{
					CL_RedrawFlag = 0;
					flag=1;
				}
				else  
				{
					//CloseMouseCursor();
					temp = packagemem;
					temp += 112;
					for(k=0;k<movepackage.padnum;k++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
							DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
					}
					for(k=0;k<movepackage.lnum;k++)
					{
						memcpy(&gnline,temp,16);
						temp+=16;
						DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
					}
					//OpenMouseCursor();
				}
				//CloseMouseCursor();
				if((flag!=0)&&(Ratsnestflag!=0))
				{
					Show_Ratsnest(&dc);
					flag=0;
				}
				temp = packagemem;
				temp += 112;
				for(k=0;k<movepackage.padnum;k++)
				{
					memcpy(&gnjunction,temp,16);
					if(((gnjunction.flag%2)^(pstate%2))!=0)
					{
						cbak = gnjunction.Dx;
						gnjunction.Dx = gnjunction.Dy;
						gnjunction.Dy = cbak;
					}
					RefreshPad(&gnjunction,x1,y1,pstate);
					memcpy(temp,&gnjunction,16);
					temp+=16;
					if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
						DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
				}
				statebak=pstate;
				for(k=0;k<movepackage.lnum;k++)
				{
					memcpy(&gnline,temp,16);
					RefreshLine(&gnline,x1,y1,pstate);
					memcpy(temp,&gnline,16);
					temp+=16;
					DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
				}
				
				invalidflag=0;
				temp=packagemem;
				temp+=112;
				for(k=0;k<movepackage.padnum;k++)
				{
					memcpy(&gnjunction,temp,16);
					temp+=16;
					if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy)) {}
					else invalidflag=1;
				}
				if(invalidflag==0)
				{
					temp = packagemem;
					memcpy(&movepackage,temp,16);
					movepackage.flag=0xcc;  /* temp value for dynamic ratsnest */
					memcpy(temp,&movepackage,16);
					movepackage.flag=0;
					memcpy(pointer,temp,movepackage.total16byte*16);
				}
				
				x1 = m_nCurrent_X;
				y1 = m_nCurrent_Y;
				//OpenMouseCursor();
			}
			else if(ch1==0)
			{
				if((flag==0)&&(Ratsnestflag!=0))
				{
					Show_Ratsnest(&dc);
					flag=1;
				}
			}
			else if(ch1==CTR_J)
			{
				//CloseMouseCursor();
				invalidflag=0;
				temp=packagemem;
				temp+=112;
				for(k=0;k<movepackage.padnum;k++)
				{
					memcpy(&gnjunction,temp,16);
					temp+=16;
					if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
						DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
					else invalidflag=1;
				}
				for(k=0;k<movepackage.lnum;k++)
				{
					memcpy(&gnline,temp,16);
					temp+=16;
					if(DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc)==-1)
					invalidflag=1;
				}

				if(invalidflag==1)
				{
					temp=packagemem;
					temp+=112;
					for(k=0;k<movepackage.padnum;k++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
						DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
					}
					for(k=0;k<movepackage.lnum;k++)
					{ 
						memcpy(&gnline,temp,16);
						temp+=16;
						DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
					}
					//OpenMouseCursor();
				}
			}
			else if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
			{
				if((flag!=0)&&(Ratsnestflag!=0)) Show_Ratsnest(&dc);
				temp=packagemem;
				temp+=112;
				for(k=0;k<movepackage.padnum;k++)
				{
					memcpy(&gnjunction,temp,16);  temp+=16;
				    if(tmax_x<(gnjunction.centerx+gnjunction.Dx)) tmax_x=gnjunction.centerx+gnjunction.Dx;
				    if(tmax_y<(gnjunction.centery+gnjunction.Dy)) tmax_y=gnjunction.centery+gnjunction.Dy;
				    if(tmin_x>(gnjunction.centerx-gnjunction.Dx)) tmin_x=gnjunction.centerx-gnjunction.Dx;
				    if(tmin_y>(gnjunction.centery-gnjunction.Dy)) tmin_y=gnjunction.centery-gnjunction.Dy;
				    DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],0,&dc);
				    if(PCB_View_Pins==ON)
					   DrawAnyString(gnjunction.centerx,gnjunction.centery,35,0,(char*)gnjunction.padname,Highlighted_Color,&dc);
				}
				for(k=0;k<movepackage.lnum;k++)
				{
					memcpy(&gnline,temp,16);  temp+=16;
				    if(tmax_x<gnline.startx) tmax_x=gnline.startx;
				    if(tmax_y<gnline.starty) tmax_y=gnline.starty;
				    if(tmin_x>gnline.startx) tmin_x=gnline.startx;
				    if(tmin_y>gnline.starty) tmin_y=gnline.starty;
				    if(tmax_x<gnline.endx) tmax_x=gnline.endx;
				    if(tmax_y<gnline.endy) tmax_y=gnline.endy;
				    if(tmin_x>gnline.endx) tmin_x=gnline.endx;
				    if(tmin_y>gnline.endy) tmin_y=gnline.endy;
				    DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
				}
				for(k=0;k<movepackage.jnum;k++)
				{
					memcpy(&gnjunction,temp,16);
				    temp+=16;
				    DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],0,&dc);
				}
				for(k=0;k<movepackage.snum;k++)
				{
					memcpy(&gnstring,temp,64);
				    RefreshString(&gnstring,m_nCurrent_X,m_nCurrent_Y,pstate);
				    memcpy(temp,&gnstring,64);
				    temp+=64;
				    if(pstate==0) DrawStructString(&gnstring,PCB_LayerColor[gnstring.layer],&dc);
				}
				for(k=0;k<movepackage.anum;k++)
				{
					memcpy(&gnarc,temp,16);
				    gnarc.centerx=gnarc.centerx + m_nCurrent_X;
				    gnarc.centery=gnarc.centery + m_nCurrent_Y;
				    
					RefreshPCBArc(&gnarc,pstate);
					memcpy(temp,&gnarc,16);
					temp+=16;
					DrawStructArc(&gnarc,PCB_LayerColor[gnarc.layer],&dc);
				}
				for(k=0;k<movepackage.fnum;k++)
				{
					memcpy(&gnfill,temp,16);
				    memcpy(&gnfill,temp,gnfill.total16byte);
				    temp+=gnfill.total16byte*16;
				    DrawStructFillArea(&gnfill,PCB_LayerColor[gnfill.layer],&dc);
				}

				//CloseMouseCursor();

				movepackage.cx=x1;
				movepackage.cy=y1;
				movepackage.angle=pstate;

				movepackage.IDx = m_nCurrent_X;
				movepackage.IDy = m_nCurrent_Y + 150;
				movepackage.Px = tmax_x;
				movepackage.Py = tmax_y;
				movepackage.x = tmin_x;
				movepackage.y = tmin_y;
				movepackage.Cx = movepackage.IDx;
				movepackage.Cy = movepackage.IDy-movepackage.Csize;

				if(PCB_Component_ID_Hide==ON)
				{
					length=movepackage.IDstate>>2;
					if((movepackage.IDstate&0x01)==0)
						DrawPCBAnyString(movepackage.IDx,movepackage.IDy,movepackage.IDsize,\
						      movepackage.IDalign,movepackage.IDwidth,movepackage.ID,PCB_LayerColor[length],&dc);
				}
				if(PCB_Component_Comment_Hide==ON)
				{
					length=movepackage.Cstate>>2;
					if((movepackage.Cstate&0x01)==0)
						DrawPCBAnyString(movepackage.Cx,movepackage.Cy,movepackage.Csize,\
						      movepackage.Calign,movepackage.Cwidth,movepackage.C,PCB_LayerColor[length],&dc);
				}

				temp = packagemem;
				memcpy(temp,&movepackage,112);
				temp = packagemem;
				
				undoflag = 1;
				abcpack = (struct Package*)pointer;
				Check_Undo_Buffer(30);
				undo_p = pDoc->m_pPCB_Undo_Current;
				head.op = UNDO_MOVE;
				head.ob = UNDO_PACKAGE;
				head.p = pointer;
				head.dx = x1 - abcpack->cx;
				head.dy = y1 - abcpack->cy;
				head.state1 = (unsigned char)abcpack->angle;
				head.state2 = pstate;
				memcpy(undo_p,&head,16);
				pDoc->m_pPCB_Undo_Current += 16;
				
				Check_Undo_Buffer(30);
				undo_p = pDoc->m_pPCB_Undo_Current;
				head.op = UNDO_MOVE;
				head.ob = UNDO_PACKLIMMIT;
				head.p = pointer;
				head.dx = movepackage.Px - abcpack->Px;
				head.dy = movepackage.Py - abcpack->Py;
				head.size1 = movepackage.x - abcpack->x;
				head.size2 = movepackage.y - abcpack->y;
				memcpy(undo_p,&head,16);
				pDoc->m_pPCB_Undo_Current += 16;
				
				Check_Undo_Buffer(30);
				undo_p = pDoc->m_pPCB_Undo_Current;
				head.op = UNDO_MOVE;
				head.ob = UNDO_PACKID;
				head.p = pointer;
				head.dx = movepackage.IDx - abcpack->IDx;
				head.dy = movepackage.IDy - abcpack->IDy;
				head.size1 = abcpack->IDsize;
				head.size2 = movepackage.IDsize;
				head.state1 = abcpack->IDalign;
				head.state2 = movepackage.IDalign;
				memcpy(undo_p,&head,16);
				pDoc->m_pPCB_Undo_Current += 16;
				
				Check_Undo_Buffer(30);
				undo_p = pDoc->m_pPCB_Undo_Current;
				head.op = UNDO_MOVE;
				head.ob = UNDO_PACKCOMMENT;
				head.p = pointer;
				head.dx = movepackage.Cx - abcpack->Cx;
				head.dy = movepackage.Cy - abcpack->Cy;
				head.size1 = abcpack->Csize;
				head.size2 = movepackage.Csize;
				head.state1 = abcpack->Calign;
				head.state2 = movepackage.Calign;
				memcpy(undo_p,&head,16);
				pDoc->m_pPCB_Undo_Current += 16;
				
				memcpy(pointer,temp,movepackage.total16byte*16);
				
				//OpenMouseCursor();
				break;
			}
			else if(ch1==SPACE)
			{
				if(pstate<3)  pstate++;
				else if(pstate==3) pstate=0;
				else if(pstate==4) pstate=7;
				else if(pstate>4)  pstate--;
			}
			else Command_In_Function(ch1);
		}while(1);
	}

LIMOVEEND:

	
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	
	
	
	if(ratsnestbak==0)
	{
		Show_Ratsnest(&dc);
		Ratsnestflag=0;
		Route_Mode=routemodebak;
	}
	else 
	{
		Show_Ratsnest(&dc);
		Route_Mode = routemodebak;
		Show_Ratsnest(&dc);
	}

	CL_Auto_Pan = OFF;
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	free(packagemem);
	free(packagebak);	
	Write_Help(commandend);
	EnableFrame(TRUE);
}

void CDesignView::OnPlaceOne()
{
	unsigned ch1, flag, undoflag;
	unsigned x1,y1;
	char *pointer,*temp,*packagemem, *packagebak;
	struct Package movepackage, *ratsnestpackage, *abcpack;
	int i,ii,j,pstate,length,invalidflag,statebak;
	int tmax_x,tmin_x,tmax_y,tmin_y;
	char cbak;
	int ratsnestbak, routemodebak;

	unsigned ccx,ccy,ccx0,ccy0,ccdeltax,ccdeltay,ccstate, ccstatebak;
	unsigned optix,optiy,placeflag;
	unsigned long optilength, optilengthbak;
	unsigned px1,px2,py1,py2,tempxy;
	int xs,ys,xe,ye, placementstate,increasenum;
	struct Undo_Head head;
	char *undo_p;
	CClientDC dc(this);
	int drawmode = dc.GetROP2();
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Number==0) 
	{
		Error( error16 );
	    return;
	}

	DrawPCBHighlight(0);
	free(Highlight_Trackbuf);
	free(Highlight_Padbuf);
	free(Highlight_ArcFillbuf);
	free(Highlight_Packagebuf);
	Highlight_Trackbuf = NULL;
	Highlight_Padbuf = NULL;
	Highlight_ArcFillbuf = NULL;
	Highlight_Packagebuf = NULL;

	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(helpstr1);

	flag = 0;
	px1=px2=0;py1=py2=0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
		{
			px1 = m_nCurrent_X;
			py1 = m_nCurrent_Y;
			Get_VPort_XY(&xs,&ys,px1,py1);
			xe=xs;
			ye=ys;
			//CloseMouseCursor();
			Write_Help(helpstr2);
			//OpenMouseCursor();
			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xs,&ys,px1,py1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
				{
					CL_Auto_Pan = OFF;
					DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
					//setwritemode(1);
					//setcolor(Overwrite_Color);
					//rectangle(xs,ys,xe,ye);
					dc.SetROP2(R2_NOTXORPEN);
					rectangle(&dc,xs,ys,xe,ye);
					dc.SetROP2(drawmode);
					return;
				}
				if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{
						flag=1;
						//CloseMouseCursor();
						//setwritemode(1);
						//setcolor(Overwrite_Color);
						//rectangle(xs,ys,xe,ye);
						dc.SetROP2(R2_NOTXORPEN);
						rectangle(&dc,xs,ys,xe,ye);
						dc.SetROP2(drawmode);
						//OpenMouseCursor();
					}
					else
					{
						if(CL_RedrawFlag==1)
						{
							CL_RedrawFlag = 0;
						}
						else 
						{
							//setwritemode(1);
							//setcolor(Overwrite_Color);
							//rectangle(xs,ys,xe,ye);      
							dc.SetROP2(R2_NOTXORPEN);
							rectangle(&dc,xs,ys,xe,ye);
							dc.SetROP2(drawmode);
						}
						px2 = m_nCurrent_X;
						py2 = m_nCurrent_Y;
						Get_VPort_XY(&xe,&ye,px2,py2);
						//CloseMouseCursor();
						//setwritemode(1);
						//setcolor(Overwrite_Color);
						//rectangle(xs,ys,xe,ye);
						dc.SetROP2(R2_NOTXORPEN);
						rectangle(&dc,xs,ys,xe,ye);
						dc.SetROP2(drawmode);
						//OpenMouseCursor();
					}
				}
				if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
				{
					ch1 = VK_ESCAPE;
					//CloseMouseCursor();
					//setwritemode(1);
					//setcolor(Overwrite_Color);
					dc.SetROP2(R2_NOTXORPEN);
					rectangle(&dc,xs,ys,xe,ye);
					dc.SetROP2(drawmode);
					//rectangle(xs,ys,xe,ye);
					//OpenMouseCursor();
					break;
				}
				else Command_In_Function(ch1);
			}while(1);
		}
		else  Command_In_Function(ch1);
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));

	if((px1==px2)||(py1==py2))
	{
		CL_Auto_Pan = OFF;
		DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
		return;
	}
	if(px1>px2)
	{
		tempxy=px2;
		px2=px1;
		px1=tempxy;
	}
	if(py1>py2)
	{
		tempxy=py2;
		py2=py1;
		py1=tempxy;
	}

	CL_Auto_Pan = OFF;
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);

	if((packagemem=(char *)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}
	if((packagebak=(char *)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}
	
	pDoc->SetModifiedFlag(TRUE);

	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(helpstr29);
	tmax_x=0; tmin_x=CL_Size_Maxx; tmax_y=0; tmin_y=CL_Size_Maxy;
	ratsnestbak=Ratsnestflag;
	routemodebak=Route_Mode;
	undoflag = 0;
	do 
	{
		ch1=MouseKey();
		if((ch1==VK_RETURN)||(ch1==PUSHMOUSE))
		{
			x1 = m_nCurrent_X;
			y1 = m_nCurrent_Y;
			if(Select_Package(x1,y1,&pointer) == 0) 
			{
				MessageBeep(0xFFFFFFFF);
				continue;
			}

			EnableFrame(FALSE);
			DrawDeletePackage(pointer,&dc);
			memcpy(&movepackage,pointer,16);
			temp=packagemem;
			memcpy(temp,pointer,movepackage.total16byte*16);
			memcpy(packagebak,packagemem,movepackage.total16byte*16);

			if(Ratsnestflag!=0) Show_Ratsnest(&dc);
			Ratsnestflag=1;
			Route_Mode=0;
			pDoc->Get_Package_Table();
			ratsnestpackage = (struct Package*)packagemem;
			pDoc->Fresh_Ratsnest_Package(ratsnestpackage->ID);

			memcpy(&movepackage,pointer,16);
			movepackage.flag=0xcc;  /* temp value for dynamic ratsnest */
			memcpy(pointer,&movepackage,16);

			pstate=movepackage.angle;
			x1=movepackage.cx;
			y1=movepackage.cy;
			Get_VPort_XY(&i,&j,x1,y1);
			//SetMousePosition(i+4,j+74);
			Restore_LargeCrossXY();
			m_nCurrent_X = x1;
			m_nCurrent_Y = y1;

			temp=packagemem;
			memcpy(&movepackage,temp,112);
			temp+=112;
			for(i=0;i<movepackage.padnum;i++)
			{
				memcpy(&gnjunction,temp,16);
				temp+=16;
				if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
					DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
			}
			for(i=0;i<movepackage.lnum;i++)
			{
				memcpy(&gnline,temp,16);
				temp+=16;
				DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,Highlighted_Color,1,&dc);
			}
			for(i=0;i<movepackage.jnum;i++)
			{
				memcpy(&gnjunction,temp,16);
				temp+=16;
			}
			for(i=0;i<movepackage.snum;i++)
			{
				memcpy(&gnstring,temp,64);
				temp+=64;
			}
			for(i=0;i<movepackage.anum;i++)
			{
				memcpy(&gnarc,temp,16);
				gnarc.centerx=gnarc.centerx-x1;
				gnarc.centery=gnarc.centery-y1;
				memcpy(temp,&gnarc,16);
				temp+=16;
			}
			for(i=0;i<movepackage.fnum;i++)
			{
				memcpy(&gnfill,temp,16);
				temp+=gnfill.total16byte*16;
			}

			if(Ratsnestflag!=0) 
			{
				Ratsnestflag=3;
				strcpy(Ratsneststring,movepackage.ID);
				//Show_Ratsnest();
			}


			CL_Auto_Pan = ON;
			DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
			CL_RedrawFlag = 0;
			Write_Help(autoplacement);
			flag=0;
			ccx=ccx0=((int)ceil((float)px1/25))*25;
			ccy=ccy0=((int)ceil((float)py1/25))*25;
			increasenum=optilengthbak=0;
			ccdeltax = CL_Autoplace_Dx;
			ccdeltay = CL_Autoplace_Dy;
			ccstate=pstate;
			ccstatebak=0;
			ch1=1;
			optilength=1234567890;
			placeflag=0;
			placementstate=0;
			do
			{
				
				for(;;)
				{
					int ch2 = MRetKey();
					if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
					{
						ii = AfxMessageBox(interuptplacement, MB_YESNO|MB_ICONWARNING);
						if( ii == IDYES)
						{
							ch1 = VK_ESCAPE;
						}
					}
					break;
				}
				
				
				if(ch1 == VK_RETURN) ch1 = VK_ESCAPE;

				
				else if((ch1==0)||(ch1==1))
				{
					
					if(ch1==0)
					{
						if(optilength>Ratsnest_Length)
						{
							optilength=Ratsnest_Length;
							optix=ccx; optiy=ccy;
							ccstate=pstate;
							placeflag=1;
							increasenum=0;
						}
						else if(optilengthbak<Ratsnest_Length)
						{
							increasenum++;
						}
						else increasenum=0;
						optilengthbak=Ratsnest_Length;
					}

					
					if((placementstate==3)||(placeflag==2))
					{
						if(ccstatebak<3)
						{
							ccx=ccx0=((int)ceil((float)px1/25))*25;
							ccy=ccy0=((int)ceil((float)py1/25))*25;
							increasenum=optilengthbak=0;
							ch1=1;
							placementstate=0;
							ccstatebak++;
							ch1=SPACE;
						}
						else
						{
							if(placeflag==0) ch1=ESC;
							else if(placeflag==1)
							{
								m_nCurrent_X = optix;
								m_nCurrent_Y = optiy;
								pstate = ccstate;
								ch1 = MOUSEMOVE;
								placeflag = 2;
							}
							else ch1 = VK_RETURN;
						}
					}
					else
					{
						
						ccx+=ccdeltax;

						
						if((placementstate==2)||(increasenum>3))
						{
							ccx=ccx0;
							ccy+=ccdeltay;
							increasenum=0;
						}

						
						m_nCurrent_X = ccx;
						m_nCurrent_Y = ccy;
						ch1=MOUSEMOVE;
					}
				}
				else if(ch1==MOUSEMOVE)
				{
					placementstate=Placement_Valid1(packagemem,movepackage.padnum,movepackage.lnum,px1,py1,px2,py2);
					if((placementstate==0)&&(Placement_Valid2(packagemem,movepackage.padnum,movepackage.lnum)==0))
					{
						ch1=0;
					}
					else ch1=1;
				}
				else if(ch1==SPACE) ch1=1;

				if(ch1==VK_ESCAPE)
				{
				   //CloseMouseCursor();
					temp=packagemem;
					temp+=112;
					for(i=0;i<movepackage.padnum;i++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
							DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
					}
					for(i=0;i<movepackage.lnum;i++)
					{
						memcpy(&gnline,temp,16);
						temp+=16;
						DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
					}
					if((flag!=0)&&(Ratsnestflag!=0)) Show_Ratsnest(&dc);

					temp=packagebak;
				    memcpy(pointer,temp,movepackage.total16byte*16);
					DrawPackage(pointer,&dc);
					if(Ratsnestflag!=0)
					{
						Show_Ratsnest(&dc); 
						flag=1; 
					}
					//OpenMouseCursor();
					goto LIMOVEEND;
				}
				if(ch1==MOUSEMOVE)
				{
					if(CL_RedrawFlag==1)
					{
						CL_RedrawFlag=0;
						flag=1;
					}
				    else 
					{
						//CloseMouseCursor();
					    temp=packagemem;
					    temp+=112;
					    for(i=0;i<movepackage.padnum;i++)
						{
							memcpy(&gnjunction,temp,16);
							temp+=16;
							if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
							DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
						}
						for(i=0;i<movepackage.lnum;i++)
						{
							memcpy(&gnline,temp,16);
							temp+=16;
							DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
						}
						//OpenMouseCursor();
					}
					//CloseMouseCursor();
				    if((flag!=0)&&(Ratsnestflag!=0))
					{ 
						Show_Ratsnest(&dc);
						flag=0;
					}
					
					temp=packagemem;
					temp+=112;
					for(i=0;i<movepackage.padnum;i++)
					{
						memcpy(&gnjunction,temp,16);
						if(((gnjunction.flag%2)^(pstate%2))!=0)
						{
							cbak = gnjunction.Dx;
							gnjunction.Dx = gnjunction.Dy;
							gnjunction.Dy = cbak;
						}
						RefreshPad(&gnjunction,x1,y1,pstate);
						memcpy(temp,&gnjunction,16);
						temp+=16;
						if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
						  DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
					}
					statebak=pstate;
					for(i=0;i<movepackage.lnum;i++)
					{
						memcpy(&gnline,temp,16);
						RefreshLine(&gnline,x1,y1,pstate);
						memcpy(temp,&gnline,16);
						temp+=16;
						DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
					}

					invalidflag=0;
					temp=packagemem;
					temp+=112;
					for(i=0;i<movepackage.padnum;i++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy)) {}
						else invalidflag=1;
					}
					if(invalidflag==0)
					{
					     temp=packagemem;
					     memcpy(&movepackage,temp,16);
					     movepackage.flag=0xcc;  // temp value for dynamic ratsnest 
					     memcpy(temp,&movepackage,16);
					     movepackage.flag=0;
					     memcpy(pointer,temp,movepackage.total16byte*16);
					}
					
					x1 = m_nCurrent_X;
					y1 = m_nCurrent_Y;
					//OpenMouseCursor();
				}
				else if(ch1==0)
				{
					if((flag==0)&&(Ratsnestflag!=0))
					{
						Show_Ratsnest(&dc);
						flag=1;
					}
				}
				else if(ch1==CTR_J)
				{
					//CloseMouseCursor();
					invalidflag=0;
					temp=packagemem;
					temp+=112;
					for(i=0;i<movepackage.padnum;i++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
							DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
						else invalidflag=1;
					}
					for(i=0;i<movepackage.lnum;i++)
					{
						memcpy(&gnline,temp,16);
						temp+=16;
						if(DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc)==-1)
						invalidflag=1;
					}

					if(invalidflag==1)
					{
						temp=packagemem;
						temp+=112;
						for(i=0;i<movepackage.padnum;i++)
						{
							memcpy(&gnjunction,temp,16);
							temp+=16;
							if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
								DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
						}
						for(i=0;i<movepackage.lnum;i++)
						{
							memcpy(&gnline,temp,16);
							temp+=16;
							DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
						}
						//OpenMouseCursor();
					}
				}
				else if((ch1==PUSHMOUSE)||(ch1==ENTER))
				{
					if((flag!=0)&&(Ratsnestflag!=0)) Show_Ratsnest(&dc);
					temp=packagemem;
					temp+=112;
					for(i=0;i<movepackage.padnum;i++)
					{
						memcpy(&gnjunction,temp,16);  temp+=16;
						if(tmax_x<(gnjunction.centerx+gnjunction.Dx)) tmax_x=gnjunction.centerx+gnjunction.Dx;
						if(tmax_y<(gnjunction.centery+gnjunction.Dy)) tmax_y=gnjunction.centery+gnjunction.Dy;
						if(tmin_x>(gnjunction.centerx-gnjunction.Dx)) tmin_x=gnjunction.centerx-gnjunction.Dx;
						if(tmin_y>(gnjunction.centery-gnjunction.Dy)) tmin_y=gnjunction.centery-gnjunction.Dy;
						DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],0,&dc);
						if(PCB_View_Pins==ON)
						DrawAnyString(gnjunction.centerx,gnjunction.centery,35,0,(char*)gnjunction.padname,Highlighted_Color,&dc);
					}
					for(i=0;i<movepackage.lnum;i++)
					{
						memcpy(&gnline,temp,16);  temp+=16;
						if(tmax_x<gnline.startx) tmax_x=gnline.startx;
						if(tmax_y<gnline.starty) tmax_y=gnline.starty;
						if(tmin_x>gnline.startx) tmin_x=gnline.startx;
						if(tmin_y>gnline.starty) tmin_y=gnline.starty;
						if(tmax_x<gnline.endx) tmax_x=gnline.endx;
						if(tmax_y<gnline.endy) tmax_y=gnline.endy;
						if(tmin_x>gnline.endx) tmin_x=gnline.endx;
						if(tmin_y>gnline.endy) tmin_y=gnline.endy;
						DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
					}
					for(i=0;i<movepackage.jnum;i++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],0,&dc);
					}
					for(i=0;i<movepackage.snum;i++)
					{
						memcpy(&gnstring,temp,64);
						//RefreshString(&gnstring,pstate);
						memcpy(temp,&gnstring,64);
						temp+=64;
						if(pstate==0) DrawStructString(&gnstring,PCB_LayerColor[gnstring.layer],&dc);
					}
					for(i=0;i<movepackage.anum;i++)
					{
						memcpy(&gnarc,temp,16);
						gnarc.centerx=gnarc.centerx + m_nCurrent_X;
						gnarc.centery=gnarc.centery + m_nCurrent_Y;
						//gnarc.flag=0;
						RefreshPCBArc(&gnarc,pstate);
						memcpy(temp,&gnarc,16);
						temp+=16;
						DrawStructArc(&gnarc,PCB_LayerColor[gnarc.layer],&dc);
					}
					for(i=0;i<movepackage.fnum;i++)
					{
						memcpy(&gnfill,temp,16);
						memcpy(&gnfill,temp,gnfill.total16byte);
						temp+=gnfill.total16byte*16;
						DrawStructFillArea(&gnfill,PCB_LayerColor[gnfill.layer],&dc);
					}

					//CloseMouseCursor();

					movepackage.cx=x1;
					movepackage.cy=y1;
					movepackage.angle=pstate;

					movepackage.IDx = m_nCurrent_X;
					movepackage.IDy = m_nCurrent_Y+150;
					movepackage.Px=tmax_x;
					movepackage.Py=tmax_y;
					movepackage.x = tmin_x;
					movepackage.y = tmin_y;
					movepackage.Cx=movepackage.IDx;
					movepackage.Cy=movepackage.IDy-movepackage.Csize;

					if(PCB_Component_ID_Hide==ON)
					{
						length=movepackage.IDstate>>2;
						if((movepackage.IDstate&0x01)==0)
							DrawPCBAnyString(movepackage.IDx,movepackage.IDy,movepackage.IDsize,\
						      movepackage.IDalign,movepackage.IDwidth,movepackage.ID,PCB_LayerColor[length],&dc);
					}
					if(PCB_Component_Comment_Hide==ON)
					{
						length=movepackage.Cstate>>2;
						if((movepackage.Cstate&0x01)==0)
							DrawPCBAnyString(movepackage.Cx,movepackage.Cy,movepackage.Csize,\
						      movepackage.Calign,movepackage.Cwidth,movepackage.C,PCB_LayerColor[length],&dc);
					}

					temp = packagemem;
					memcpy(temp,&movepackage,112);
					temp = packagemem;
					
					undoflag = 1;
					abcpack = (struct Package*)pointer;
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_PACKAGE;
					head.p = pointer;
					head.dx = x1 - abcpack->cx;
					head.dy = y1 - abcpack->cy;
					head.state1 = (unsigned char)abcpack->angle;
					head.state2 = pstate;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_PACKLIMMIT;
					head.p = pointer;
					head.dx = movepackage.Px - abcpack->Px;
					head.dy = movepackage.Py - abcpack->Py;
					head.size1 = movepackage.x - abcpack->x;
					head.size2 = movepackage.y - abcpack->y;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_PACKID;
					head.p = pointer;
					head.dx = movepackage.IDx - abcpack->IDx;
					head.dy = movepackage.IDy - abcpack->IDy;
					head.size1 = abcpack->IDsize;
					head.size2 = movepackage.IDsize;
					head.state1 = abcpack->IDalign;
					head.state2 = movepackage.IDalign;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_PACKCOMMENT;
					head.p = pointer;
					head.dx = movepackage.Cx - abcpack->Cx;
					head.dy = movepackage.Cy - abcpack->Cy;
					head.size1 = abcpack->Csize;
					head.size2 = movepackage.Csize;
					head.state1 = abcpack->Calign;
					head.state2 = movepackage.Calign;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					
					memcpy(pointer,temp,movepackage.total16byte*16);
					if(Ratsnestflag!=0) Show_Ratsnest(&dc);
					//OpenMouseCursor();
					goto LIMOVEEND;
				}
				else if(ch1==SPACE)
				{
					if(pstate<3)  pstate++;
					else if(pstate==3) pstate=0;
					else if(pstate==4) pstate=7;
					else if(pstate>4)  pstate--;
				}
				else Command_In_Function(ch1);
			 }while(1);
		}
		else    Command_In_Function(ch1);
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));

LIMOVEEND:
	
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	

	if(ratsnestbak==0)
	{
		Show_Ratsnest(&dc);
		Ratsnestflag=0;
		Route_Mode=routemodebak;
	}
	else 
	{
		Show_Ratsnest(&dc);
		Route_Mode=routemodebak;
		Show_Ratsnest(&dc);
	}
	CL_Auto_Pan = OFF;
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	free(packagemem);
	free(packagebak);	
	Write_Help(commandend);
	EnableFrame(TRUE);
}

void CDesignView::OnPlaceOptimizeAll()
{
	unsigned ch1, flag, interuptflag, undoflag;
	unsigned x1,y1;
	char *p,*pointer,*temp,*packagemem, *packagebak;
	struct Package movepackage;
	int i,ii,l,k,pstate,length,invalidflag,statebak;
	int tmax_x,tmin_x,tmax_y,tmin_y;
	char cbak;
	int ratsnestbak, routemodebak;
	unsigned ccx,ccy,ccx0,ccy0,ccdeltax,ccdeltay;
	unsigned optix,optiy,placeflag;
	unsigned long optilength, optilengthbak;
	unsigned px1,py1;
	int placementstate,increasenum, increasedir;
	struct Package *onepackage, *abcpack;

	struct Undo_Head head;
	char *undo_p;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Number==0) 
	{
		Error( error16 );
	    return;
	}
	
	pDoc->SetModifiedFlag(TRUE);

	DrawPCBHighlight(0);
	free(Highlight_Trackbuf);
	free(Highlight_Padbuf);
	free(Highlight_ArcFillbuf);
	free(Highlight_Packagebuf);
	Highlight_Trackbuf = NULL;
	Highlight_Padbuf = NULL;
	Highlight_ArcFillbuf = NULL;
	Highlight_Packagebuf = NULL;

	if((packagemem=(char *)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}
	if((packagebak=(char *)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}

	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(optimizeplacement);

	if(Ratsnestflag!=0)
    { 
		Show_Ratsnest(&dc);
		routemodebak = Route_Mode;
		Route_Mode = 0;
    }
	else 
	{
		ratsnestbak = Ratsnestflag;
		Ratsnestflag = 1;
		routemodebak = Route_Mode;
		Route_Mode = 0;
	}

	EnableFrame(FALSE);
	undoflag = 0;
	interuptflag = 0;
	p = pDoc->m_pPackage_Buffer;
	for(i=0; i<pDoc->m_nPackage_Number; i++)
	{
		onepackage = (struct Package*)p;
	    if((onepackage->flag==0)&&(onepackage->u1==0))
		{
			pointer = p;
			DrawDeletePackage(pointer,&dc);
			memcpy(&movepackage,pointer,16);
			temp = packagemem;
			memcpy(temp,pointer,movepackage.total16byte*16);
			memcpy(packagebak,packagemem,movepackage.total16byte*16);

		    temp = packagemem;
		    memcpy(&movepackage,temp,112);
		    pDoc->Get_Package_Table();
		    pDoc->Fresh_Ratsnest_Package(movepackage.ID);
		    movepackage.flag = 0xcc;  
		    memcpy(pointer,&movepackage,16);

			pstate = movepackage.angle;
			x1 = movepackage.cx;
			y1 = movepackage.cy;
			Get_VPort_XY(&k,&l,x1,y1);
			Restore_LargeCrossXY();
			m_nCurrent_X = x1;
			m_nCurrent_Y = y1;

			temp = packagemem;
			memcpy(&movepackage,temp,112);
			temp += 112;
			for(k=0;k<movepackage.padnum;k++)
			{
				memcpy(&gnjunction,temp,16);
				temp+=16;
				if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
					DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
			}
			for(k=0;k<movepackage.lnum;k++)
			{
				memcpy(&gnline,temp,16);
				temp+=16;
				DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,Highlighted_Color,1,&dc);
			}
			for(k=0;k<movepackage.jnum;k++)
			{
				memcpy(&gnjunction,temp,16);
				temp+=16;
			}
			for(k=0;k<movepackage.snum;k++)
			{
			     memcpy(&gnstring,temp,64);
			     temp+=64;
			}
			for(k=0;k<movepackage.anum;k++)
			{
			     memcpy(&gnarc,temp,16);
			     gnarc.centerx=gnarc.centerx-x1;
			     gnarc.centery=gnarc.centery-y1;
			     memcpy(temp,&gnarc,16);
			     temp+=16;
			}
			for(k=0;k<movepackage.fnum;k++)
			{
				memcpy(&gnfill,temp,16);
				temp+=gnfill.total16byte*16;
			}

			if(Ratsnestflag!=0) 
			{
				Ratsnestflag = 3;
				strcpy(Ratsneststring,movepackage.ID);
				Show_Ratsnest(&dc);
				Show_Ratsnest(&dc);
			}

			CL_RedrawFlag = 0;
			flag = 0;
			tmax_x=0; tmin_x=CL_Size_Maxx; tmax_y=0; tmin_y=CL_Size_Maxy;
			px1 = movepackage.cx;
			py1 = movepackage.cy;
			ccx = ccx0 = ((int)ceil((float)px1/25))*25;
			ccy = ccy0 = ((int)ceil((float)py1/25))*25;
			increasenum = increasedir = 0;
			ccdeltax = CL_Autoplace_OPDx;
			ccdeltay = CL_Autoplace_OPDy;
			ch1 = 1;
			optilengthbak = optilength = Ratsnest_Length;
			placeflag = 0;
			placementstate = 0;
			do{
				
				for(;;)
				{
					int ch2 = MRetKey();
					if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
					{
						ii = AfxMessageBox(interuptplacement, MB_YESNO|MB_ICONWARNING);
						if( ii == IDYES)
						{
							ch1 = VK_ESCAPE;
							interuptflag = 1;
						}
					}
					break;
				}
				
				if(ch1 == VK_RETURN) ch1=VK_ESCAPE;

			    
			    else if((ch1==0)||(ch1==1))
				{
					
					if(ch1==0)
					{
						if(Ratsnest_Length==0) 
						{
							ch1 = VK_RETURN; 
							continue; 
						}
						if(optilength>Ratsnest_Length)
						{
							optilength=Ratsnest_Length;
							optix=ccx; optiy=ccy;
							placeflag=1;
							increasenum=0;
					    }
						else if(optilengthbak<Ratsnest_Length)
					    {
							increasenum++;
					    }
						else increasenum=0;
						optilengthbak=Ratsnest_Length;
					}

					
					if((placementstate==3)||(placeflag==2)||(increasedir>=4))
				    {
					    if(placeflag==0) ch1 = VK_ESCAPE;
						else if(placeflag==1)
						{
							m_nCurrent_X = optix;
							m_nCurrent_Y = optiy;
							ch1 = MOUSEMOVE;
							placeflag = 2;
						}
						else ch1 = VK_RETURN;
					}
					else
					{
						if((placementstate!=0)||(increasenum>0))
						{
							if(increasedir==0)  
							{
								ccx-=ccdeltax;
								ccy+=ccdeltay;
							}
							else if(increasedir==1) 
							{
								ccy-=ccdeltay;
								ccx-=ccdeltax;
							}
							else if(increasedir==2) 
							{
								ccx+=ccdeltax;
								ccy-=ccdeltay;
							}
							else if(increasedir==3) ccy+=ccdeltay;
							else {}
							increasenum=0;
							increasedir++;
						}
						else
						{
							if(increasedir==0)  ccx+=ccdeltax;
							else if(increasedir==1) ccy+=ccdeltay;
							else if(increasedir==2) ccx-=ccdeltax;
							else if(increasedir==3) ccy-=ccdeltay;
							else {}
						}
						
					    m_nCurrent_X = ccx;
					    m_nCurrent_Y = ccy;
					    ch1=MOUSEMOVE;
					}
				}
			    else if(ch1==MOUSEMOVE)
				{
					placementstate=Placement_Valid1(packagemem,movepackage.padnum,movepackage.lnum,0,0,CL_Size_Maxx,CL_Size_Maxy);
				    if((placementstate==0)&&\
				       ((placementstate=Placement_Valid2(packagemem,movepackage.padnum,movepackage.lnum))==0))
					{
						ch1=0;
					}
					else ch1=1;
				}
				if(ch1==VK_ESCAPE)
				{
					//CloseMouseCursor();
				    temp=packagemem;
				    temp+=112;
				    for(k=0;k<movepackage.padnum;k++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
						DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
					}
				    for(k=0;k<movepackage.lnum;k++)
					{
						memcpy(&gnline,temp,16);
						temp+=16;
						DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
					}
					if((flag!=0)&&(Ratsnestflag!=0)) Show_Ratsnest(&dc);

				    temp = packagebak;
					memcpy(pointer,temp,movepackage.total16byte*16);
					DrawPackage(pointer,&dc);
					if(interuptflag==1) goto LIMOVEEND;
					else break;
				}
				if(ch1==MOUSEMOVE)
				{
					if(CL_RedrawFlag==1)
					{
						CL_RedrawFlag = 0;
						flag = 1;
					}
					else 
					{
						temp = packagemem;
						temp += 112;
						for(k=0;k<movepackage.padnum;k++)
						{
							memcpy(&gnjunction,temp,16);
							temp+=16;
							if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
								DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
						}
						for(k=0;k<movepackage.lnum;k++)
						{
							memcpy(&gnline,temp,16);
							temp+=16;
							DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
						}
					}
					if((flag!=0)&&(Ratsnestflag!=0))
					{
						Show_Ratsnest(&dc);
						flag=0;
					}
					temp=packagemem;
					temp+=112;
					for(k=0;k<movepackage.padnum;k++)
					{
						memcpy(&gnjunction,temp,16);
						if(((gnjunction.flag%2)^(pstate%2))!=0)
						{
							cbak = gnjunction.Dx;
							gnjunction.Dx = gnjunction.Dy;
							gnjunction.Dy = cbak;
						}
						RefreshPad(&gnjunction,x1,y1,pstate);
						memcpy(temp,&gnjunction,16);
						temp+=16;
						if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
							DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
					}
					statebak=pstate;
					for(k=0;k<movepackage.lnum;k++)
					{
						memcpy(&gnline,temp,16);
						RefreshLine(&gnline,x1,y1,pstate);
						memcpy(temp,&gnline,16);
						temp+=16;
						DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
					}

					invalidflag=0;
					temp = packagemem;
					temp += 112;
					for(k=0;k<movepackage.padnum;k++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy)) {}
						else invalidflag=1;
					}
					if(invalidflag==0)
					{
						temp=packagemem;
						memcpy(&movepackage,temp,16);
						movepackage.flag=0xcc;  
						memcpy(temp,&movepackage,16);
						movepackage.flag=0;
						memcpy(pointer,temp,movepackage.total16byte*16);
					}
					x1 = m_nCurrent_X;
					y1 = m_nCurrent_Y;
				}
				else if(ch1==0)
				{
					if((flag==0)&&(Ratsnestflag!=0))
					{
						Show_Ratsnest(&dc);
						flag=1;
					}
				}
				else if(ch1==CTR_J)
				{
					invalidflag=0;
					temp=packagemem;
					temp+=112;
					for(k=0;k<movepackage.padnum;k++)
					{ 
						memcpy(&gnjunction,temp,16);
						temp+=16;
						if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
						DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
						else invalidflag=1;
					}
					for(k=0;k<movepackage.lnum;k++)
					{
						memcpy(&gnline,temp,16);
						temp+=16;
						if(DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc)==-1)
							invalidflag=1;
					}
					if(invalidflag==1)
					{
						temp=packagemem;
						temp+=112;
						for(k=0;k<movepackage.padnum;k++)
						{
							memcpy(&gnjunction,temp,16);
							temp+=16;
							if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
							DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
						}
						for(k=0;k<movepackage.lnum;k++)
						{
							memcpy(&gnline,temp,16);
							temp+=16;
							DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
						}
					}
				}
				else if((ch1==PUSHMOUSE)||(ch1==ENTER))
				{
					if((flag!=0)&&(Ratsnestflag!=0)) Show_Ratsnest(&dc);
					temp=packagemem;
					temp+=112;
					for(k=0;k<movepackage.padnum;k++)
					{
						memcpy(&gnjunction,temp,16);  temp+=16;
						if(tmax_x<(gnjunction.centerx+gnjunction.Dx)) tmax_x=gnjunction.centerx+gnjunction.Dx;
						if(tmax_y<(gnjunction.centery+gnjunction.Dy)) tmax_y=gnjunction.centery+gnjunction.Dy;
						if(tmin_x>(gnjunction.centerx-gnjunction.Dx)) tmin_x=gnjunction.centerx-gnjunction.Dx;
						if(tmin_y>(gnjunction.centery-gnjunction.Dy)) tmin_y=gnjunction.centery-gnjunction.Dy;
						DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],0,&dc);
						if(PCB_View_Pins==ON)
							DrawAnyString(gnjunction.centerx,gnjunction.centery,35,0,(char*)gnjunction.padname,Highlighted_Color,&dc);
					}
					for(k=0;k<movepackage.lnum;k++)
					{
						memcpy(&gnline,temp,16);  temp+=16;
					    if(tmax_x<gnline.startx) tmax_x=gnline.startx;
					    if(tmax_y<gnline.starty) tmax_y=gnline.starty;
					    if(tmin_x>gnline.startx) tmin_x=gnline.startx;
					    if(tmin_y>gnline.starty) tmin_y=gnline.starty;
					    if(tmax_x<gnline.endx) tmax_x=gnline.endx;
					    if(tmax_y<gnline.endy) tmax_y=gnline.endy;
					    if(tmin_x>gnline.endx) tmin_x=gnline.endx;
					    if(tmin_y>gnline.endy) tmin_y=gnline.endy;
					    DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
					}
					for(k=0;k<movepackage.jnum;k++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],0,&dc);
					}
					for(k=0;k<movepackage.snum;k++)
					{
					     memcpy(&gnstring,temp,64);
					     memcpy(temp,&gnstring,64);
					     temp+=64;
					     if(pstate==0) DrawStructString(&gnstring,PCB_LayerColor[gnstring.layer],&dc);
					}
					for(k=0;k<movepackage.anum;k++)
					{
						memcpy(&gnarc,temp,16);
					    gnarc.centerx=gnarc.centerx + m_nCurrent_X;
					    gnarc.centery=gnarc.centery + m_nCurrent_Y;
					    RefreshPCBArc(&gnarc,pstate);
					    memcpy(temp,&gnarc,16);
					    temp+=16;
					    DrawStructArc(&gnarc,PCB_LayerColor[gnarc.layer],&dc);
					}
					for(k=0;k<movepackage.fnum;k++)
					{
						memcpy(&gnfill,temp,16);
					    memcpy(&gnfill,temp,gnfill.total16byte);
					    temp+=gnfill.total16byte*16;
					    DrawStructFillArea(&gnfill,PCB_LayerColor[gnfill.layer],&dc);
					}

					movepackage.cx = x1;
					movepackage.cy = y1;
					movepackage.angle = pstate;

					movepackage.IDx = m_nCurrent_X;
					movepackage.IDy = m_nCurrent_Y+150;
					movepackage.Px = tmax_x;
					movepackage.Py = tmax_y;
					movepackage.x = tmin_x;
					movepackage.y = tmin_y;
					movepackage.Cx = movepackage.IDx;
					movepackage.Cy = movepackage.IDy-movepackage.Csize;

					if(PCB_Component_ID_Hide==ON)
					{
						length=movepackage.IDstate>>2;
						if((movepackage.IDstate&0x01)==0)
							DrawPCBAnyString(movepackage.IDx,movepackage.IDy,movepackage.IDsize,\
							      movepackage.IDalign,movepackage.IDwidth,movepackage.ID,PCB_LayerColor[length],&dc);
					}
					if(PCB_Component_Comment_Hide==ON)
					{
						length=movepackage.Cstate>>2;
						if((movepackage.Cstate&0x01)==0)
							DrawPCBAnyString(movepackage.Cx,movepackage.Cy,movepackage.Csize,\
							      movepackage.Calign,movepackage.Cwidth,movepackage.C,PCB_LayerColor[length],&dc);
					}

					temp = packagemem;
					memcpy(temp,&movepackage,112);
					temp = packagemem;
					
					undoflag = 1;
					abcpack = (struct Package*)pointer;
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_PACKAGE;
					head.p = pointer;
					head.dx = x1 - abcpack->cx;
					head.dy = y1 - abcpack->cy;
					head.state1 = (unsigned char)abcpack->angle;
					head.state2 = pstate;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_PACKLIMMIT;
					head.p = pointer;
					head.dx = movepackage.Px - abcpack->Px;
					head.dy = movepackage.Py - abcpack->Py;
					head.size1 = movepackage.x - abcpack->x;
					head.size2 = movepackage.y - abcpack->y;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_PACKID;
					head.p = pointer;
					head.dx = movepackage.IDx - abcpack->IDx;
					head.dy = movepackage.IDy - abcpack->IDy;
					head.size1 = abcpack->IDsize;
					head.size2 = movepackage.IDsize;
					head.state1 = abcpack->IDalign;
					head.state2 = movepackage.IDalign;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_PACKCOMMENT;
					head.p = pointer;
					head.dx = movepackage.Cx - abcpack->Cx;
					head.dy = movepackage.Cy - abcpack->Cy;
					head.size1 = abcpack->Csize;
					head.size2 = movepackage.Csize;
					head.state1 = abcpack->Calign;
					head.state2 = movepackage.Calign;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					
	
					memcpy(pointer,temp,movepackage.total16byte*16);
					break;
				}
				else if(ch1==SPACE)
				{
					if(pstate<3)  pstate++;
					else if(pstate==3) pstate=0;
					else if(pstate==4) pstate=7;
					else if(pstate>4)  pstate--;
				}
				else Command_In_Function(ch1);
			}while(1);
		}
		p+=onepackage->total16byte*16;
	}



LIMOVEEND:
	
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	

	if(ratsnestbak == 0)
	{
		Show_Ratsnest(&dc);
		Ratsnestflag = 0;
		Route_Mode = routemodebak;
	}
	else 
	{
		Show_Ratsnest(&dc);
		Route_Mode = routemodebak;
		Show_Ratsnest(&dc);
	}
	
	CL_Auto_Pan = OFF;
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	free(packagemem);
	free(packagebak);
	Write_Help(commandend);
	EnableFrame(TRUE);
}

void CDesignView::OnPlaceOptimizeOne() 
{
	unsigned ch1, flag,undoflag;
	unsigned x1,y1;
	char *pointer,*temp,*packagemem, *packagebak;
	struct Package movepackage,*abcpack;
	int i,ii,j,pstate,length,invalidflag,statebak;
	int tmax_x,tmin_x,tmax_y,tmin_y;
	char cbak;
	int ratsnestbak, routemodebak;
	unsigned ccx,ccy,ccx0,ccy0,ccdeltax,ccdeltay;
	unsigned optix,optiy,placeflag;
	unsigned long optilength, optilengthbak;
	unsigned px1,py1;
	int placementstate,increasenum, increasedir;
	struct Undo_Head head;
	char *undo_p;

	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Number == 0)
	{
		Error( error16 );
	    return;
	}
	DrawPCBHighlight(0);
	free(Highlight_Trackbuf);
	free(Highlight_Padbuf);
	free(Highlight_ArcFillbuf);
	free(Highlight_Packagebuf);
	Highlight_Trackbuf = NULL;
	Highlight_Padbuf = NULL;
	Highlight_ArcFillbuf = NULL;
	Highlight_Packagebuf = NULL;

	if((packagemem=(char *)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}
	if((packagebak=(char *)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}
	
	pDoc->SetModifiedFlag(TRUE);	

	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(helpstr29);
	ratsnestbak = Ratsnestflag;
	routemodebak = Route_Mode;
	tmax_x=0; tmin_x=CL_Size_Maxx; tmax_y=0; tmin_y=CL_Size_Maxy;
	undoflag = 0;
	do 
	{  
		ch1 = MouseKey();
		if((ch1==VK_RETURN)||(ch1==PUSHMOUSE))
		{
			x1 = m_nCurrent_X;
			y1 = m_nCurrent_Y;
			if(Select_PickPackage(x1,y1,&pointer) == 0) 
			{
				MessageBeep(0xFFFFFFFF);
				continue;
			}

			EnableFrame(FALSE);

			DrawDeletePackage(pointer,&dc);
			//CloseMouseCursor();
			memcpy(&movepackage,pointer,16);
			temp=packagemem;
			memcpy(temp,pointer,movepackage.total16byte*16);
			memcpy(packagebak,packagemem,movepackage.total16byte*16);

			if(Ratsnestflag!=0) Show_Ratsnest(&dc);
			Ratsnestflag = 1;
			Route_Mode = 0;

			temp = packagemem;
			memcpy(&movepackage,temp,112);
			pDoc->Get_Package_Table();
			pDoc->Fresh_Ratsnest_Package(movepackage.ID);
			movepackage.flag = 0xcc;  
			memcpy(pointer,&movepackage,16);


			pstate = movepackage.angle;
			x1 = movepackage.cx;
			y1 = movepackage.cy;
			Get_VPort_XY(&i,&j,x1,y1);
			//SetMousePosition(i+4,j+74);
			Restore_LargeCrossXY();
			m_nCurrent_X = x1;
			m_nCurrent_Y = y1;

			temp = packagemem;
			memcpy(&movepackage,temp,112);
			temp+=112;
			for(i=0;i<movepackage.padnum;i++)
			{
				memcpy(&gnjunction,temp,16);
				temp+=16;
				if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
					DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
			}
			for(i=0;i<movepackage.lnum;i++)
			{
				memcpy(&gnline,temp,16);
				temp+=16;
				DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,Highlighted_Color,1,&dc);
			}
			for(i=0;i<movepackage.jnum;i++)
			{
				memcpy(&gnjunction,temp,16);
				temp+=16;
			}
			for(i=0;i<movepackage.snum;i++)
			{
				memcpy(&gnstring,temp,64);
				temp+=64;
			}
			for(i=0;i<movepackage.anum;i++)
			{
				memcpy(&gnarc,temp,16);
				gnarc.centerx = gnarc.centerx-x1;
				gnarc.centery = gnarc.centery-y1;
				memcpy(temp,&gnarc,16);
				temp+=16;
			}
			for(i=0;i<movepackage.fnum;i++)
			{
				memcpy(&gnfill,temp,16);
				temp+=gnfill.total16byte*16;
			}

			if(Ratsnestflag!=0) 
			{
				Ratsnestflag=3;
				strcpy(Ratsneststring,movepackage.ID);
				Show_Ratsnest(&dc);
				Show_Ratsnest(&dc);
			}

			CL_RedrawFlag = 0;
			flag=0;
			px1 = movepackage.cx;
			py1 = movepackage.cy;
			ccx = ccx0 = ((int)ceil((float)px1/25))*25;
			ccy = ccy0 = ((int)ceil((float)py1/25))*25;
			increasenum = increasedir = 0;
			ccdeltax = CL_Autoplace_OPDx;
			ccdeltay = CL_Autoplace_OPDy;
			ch1 = 1;
			optilengthbak=optilength=Ratsnest_Length;
			placeflag = 0;
			placementstate=0;
			Write_Help(optimizeplacement);
			do
			{
				
				for(;;)
				{
					int ch2 = MRetKey();
					if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
					{
						ii = AfxMessageBox(interuptplacement, MB_YESNO|MB_ICONWARNING);
						if( ii == IDYES)
						{
							ch1 = VK_ESCAPE;
						}
					}
					break;
				}

				
				if(ch1 == VK_RETURN) ch1 = VK_ESCAPE;

				
				else if((ch1==0)||(ch1==1))
				{
					
					if(ch1==0)
					{
						if(Ratsnest_Length==0) 
						{
							ch1 = VK_RETURN;
							continue; 
						}
						if(optilength>Ratsnest_Length)
						{
							optilength=Ratsnest_Length;
							optix=ccx; optiy=ccy;
							placeflag=1;
							increasenum=0;
						}
						else if(optilengthbak<Ratsnest_Length)
						{
							increasenum++;
						}
						else increasenum=0;
						optilengthbak=Ratsnest_Length;
					}
				
					
					if((placementstate==3)||(placeflag==2)||(increasedir>=4))
					{
						if(placeflag==0) ch1 = VK_ESCAPE;
						else if(placeflag==1)
						{
							m_nCurrent_X = optix;
							m_nCurrent_Y = optiy;
							ch1 = MOUSEMOVE;
							placeflag = 2;
						}
						else ch1 = VK_RETURN;
					}
					else
					{
						if((placementstate!=0)||(increasenum>0))
						{
							if(increasedir==0)  
							{
								ccx-=ccdeltax;
								ccy+=ccdeltay;
							}
							else if(increasedir==1)
							{
								ccy-=ccdeltay;
								ccx-=ccdeltax;
							}
							else if(increasedir==2)
							{
								ccx+=ccdeltax;
								ccy-=ccdeltay;
							}
							else if(increasedir==3) ccy+=ccdeltay;
							else {}
							increasenum=0;
							increasedir++;
						}
						else
						{
							if(increasedir==0)  ccx+=ccdeltax;
							else if(increasedir==1) ccy+=ccdeltay;
							else if(increasedir==2) ccx-=ccdeltax;
							else if(increasedir==3) ccy-=ccdeltay;
							else {}
						}
						
						m_nCurrent_X = ccx;
						m_nCurrent_Y = ccy;
						ch1=MOUSEMOVE;
					}
				}
				else if(ch1==MOUSEMOVE)
				{
					placementstate=Placement_Valid1(packagemem,movepackage.padnum,movepackage.lnum,0,0,CL_Size_Maxx,CL_Size_Maxy);
					if((placementstate==0)&&\
					((placementstate=Placement_Valid2(packagemem,movepackage.padnum,movepackage.lnum))==0))
					{
						ch1=0;
					}
					else ch1=1;
				}
				if(ch1==VK_ESCAPE)
				{
					//CloseMouseCursor();
					temp=packagemem;
					temp+=112;
					for(i=0;i<movepackage.padnum;i++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
							DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
					}
					for(i=0;i<movepackage.lnum;i++)
					{
						memcpy(&gnline,temp,16);
						temp+=16;
						DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
					}
					if((flag!=0)&&(Ratsnestflag!=0)) Show_Ratsnest(&dc);

					temp = packagebak;
					memcpy(pointer,temp,movepackage.total16byte*16);
					DrawPackage(pointer,&dc);
					if(Ratsnestflag!=0) 
					{
						Show_Ratsnest(&dc); 
						flag=1; 
					}
					//OpenMouseCursor();
					goto LIMOVEEND;
				}
				if(ch1==MOUSEMOVE)
				{
					if(CL_RedrawFlag==1)
					{
						CL_RedrawFlag=0;
						flag=1;
					}
					else 
					{
						//CloseMouseCursor();
					    temp=packagemem;
					    temp+=112;
					    for(i=0;i<movepackage.padnum;i++)
						{
							memcpy(&gnjunction,temp,16);
							temp+=16;
							if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
								DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
						}
						for(i=0;i<movepackage.lnum;i++)
						{
							memcpy(&gnline,temp,16);
							temp+=16;
							DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
						}
						//OpenMouseCursor();
					}
				    //CloseMouseCursor();
				    if((flag!=0)&&(Ratsnestflag!=0))
					{
						Show_Ratsnest(&dc);
						flag=0;
					}
					
					temp=packagemem;
					temp+=112;
					for(i=0;i<movepackage.padnum;i++)
					{
						memcpy(&gnjunction,temp,16);
						if(((gnjunction.flag%2)^(pstate%2))!=0)
						{
							cbak = gnjunction.Dx;
							gnjunction.Dx = gnjunction.Dy;
							gnjunction.Dy = cbak;
						}
						RefreshPad(&gnjunction,x1,y1,pstate);
						memcpy(temp,&gnjunction,16);
						temp+=16;
						if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
						  DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
					}
				    statebak=pstate;
				    for(i=0;i<movepackage.lnum;i++)
					{
						memcpy(&gnline,temp,16);
						RefreshLine(&gnline,x1,y1,pstate);
						memcpy(temp,&gnline,16);
						temp+=16;
						DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
					}

					invalidflag=0;
					temp=packagemem;
					temp+=112;
					for(i=0;i<movepackage.padnum;i++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy)) {}
						else invalidflag=1;
					}
					if(invalidflag==0)
					{
						temp = packagemem;
						memcpy(&movepackage,temp,16);
						movepackage.flag=0xcc;  
						memcpy(temp,&movepackage,16);
						movepackage.flag=0;
						memcpy(pointer,temp,movepackage.total16byte*16);
					}
					x1 = m_nCurrent_X;
					y1 = m_nCurrent_Y;
					//OpenMouseCursor();
				}
				else if(ch1==0)
				{
					if((flag==0)&&(Ratsnestflag!=0))
					{
						Show_Ratsnest(&dc);
						flag=1;
					}
				}
				else if(ch1==CTR_J)
				{
					//CloseMouseCursor();
					invalidflag=0;
					temp=packagemem;
					temp+=112;
					for(i=0;i<movepackage.padnum;i++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
							DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
						else invalidflag=1;
					}
					for(i=0;i<movepackage.lnum;i++)
					{
						memcpy(&gnline,temp,16);
						temp+=16;
						if(DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc)==-1)
							invalidflag=1;
					}

					if(invalidflag==1)
					{
						temp=packagemem;
						temp+=112;
						for(i=0;i<movepackage.padnum;i++)
						{
							memcpy(&gnjunction,temp,16);
							temp+=16;
							if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy))
								DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],1,&dc);
						}
						for(i=0;i<movepackage.lnum;i++)
						{
							memcpy(&gnline,temp,16);
							temp+=16;
							DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
						}
						//OpenMouseCursor();
					}
				}
				else if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
				{
					if((flag!=0)&&(Ratsnestflag!=0)) Show_Ratsnest(&dc);
					temp=packagemem;
					temp+=112;
					for(i=0;i<movepackage.padnum;i++)
					{
						memcpy(&gnjunction,temp,16);  temp+=16;
						if(tmax_x<(gnjunction.centerx+gnjunction.Dx)) tmax_x=gnjunction.centerx+gnjunction.Dx;
						if(tmax_y<(gnjunction.centery+gnjunction.Dy)) tmax_y=gnjunction.centery+gnjunction.Dy;
						if(tmin_x>(gnjunction.centerx-gnjunction.Dx)) tmin_x=gnjunction.centerx-gnjunction.Dx;
						if(tmin_y>(gnjunction.centery-gnjunction.Dy)) tmin_y=gnjunction.centery-gnjunction.Dy;
						DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],0,&dc);
						if(PCB_View_Pins==ON)
						DrawAnyString(gnjunction.centerx,gnjunction.centery,35,0,(char*)gnjunction.padname,Highlighted_Color,&dc);
				    }
					for(i=0;i<movepackage.lnum;i++)
					{
						memcpy(&gnline,temp,16);  temp+=16;
						if(tmax_x<gnline.startx) tmax_x=gnline.startx;
						if(tmax_y<gnline.starty) tmax_y=gnline.starty;
						if(tmin_x>gnline.startx) tmin_x=gnline.startx;
						if(tmin_y>gnline.starty) tmin_y=gnline.starty;
						if(tmax_x<gnline.endx) tmax_x=gnline.endx;
						if(tmax_y<gnline.endy) tmax_y=gnline.endy;
						if(tmin_x>gnline.endx) tmin_x=gnline.endx;
						if(tmin_y>gnline.endy) tmin_y=gnline.endy;
						DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
					}
					for(i=0;i<movepackage.jnum;i++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],0,&dc);
					}
					for(i=0;i<movepackage.snum;i++)
					{
						memcpy(&gnstring,temp,64);
						//RefreshString(&gnstring,pstate);
						memcpy(temp,&gnstring,64);
						temp+=64;
						if(pstate==0) DrawStructString(&gnstring,PCB_LayerColor[gnstring.layer],&dc);
					}
					for(i=0;i<movepackage.anum;i++)
					{
						memcpy(&gnarc,temp,16);
						gnarc.centerx=gnarc.centerx + m_nCurrent_X;
						gnarc.centery=gnarc.centery + m_nCurrent_Y;
						//gnarc.flag=0;
						RefreshPCBArc(&gnarc,pstate);
						memcpy(temp,&gnarc,16);
						temp+=16;
						DrawStructArc(&gnarc,PCB_LayerColor[gnarc.layer],&dc);
					}
					for(i=0;i<movepackage.fnum;i++)
					{
						memcpy(&gnfill,temp,16);
						memcpy(&gnfill,temp,gnfill.total16byte);
						temp+=gnfill.total16byte*16;
						DrawStructFillArea(&gnfill,PCB_LayerColor[gnfill.layer],&dc);
					}

					//CloseMouseCursor();

					movepackage.cx=x1;
					movepackage.cy=y1;
					movepackage.angle=pstate;

					movepackage.IDx = m_nCurrent_X;
					movepackage.IDy = m_nCurrent_Y+150;
					movepackage.Px=tmax_x;
					movepackage.Py=tmax_y;
					movepackage.x = tmin_x;
					movepackage.y = tmin_y;
					movepackage.Cx=movepackage.IDx;
					movepackage.Cy=movepackage.IDy-movepackage.Csize;
	
					if(PCB_Component_ID_Hide==ON)
					{
						length=movepackage.IDstate>>2;
						if((movepackage.IDstate&0x01)==0)
							DrawPCBAnyString(movepackage.IDx,movepackage.IDy,movepackage.IDsize,\
						      movepackage.IDalign,movepackage.IDwidth,movepackage.ID,PCB_LayerColor[length],&dc);
					}
					if(PCB_Component_Comment_Hide==ON)
					{
						length=movepackage.Cstate>>2;
						if((movepackage.Cstate&0x01)==0)
							DrawPCBAnyString(movepackage.Cx,movepackage.Cy,movepackage.Csize,\
						      movepackage.Calign,movepackage.Cwidth,movepackage.C,PCB_LayerColor[length],&dc);
					}

					temp = packagemem;
					memcpy(temp,&movepackage,112);
					temp = packagemem;
					
					undoflag = 1;
					abcpack = (struct Package*)pointer;
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_PACKAGE;
					head.p = pointer;
					head.dx = x1 - abcpack->cx;
					head.dy = y1 - abcpack->cy;
					head.state1 = (unsigned char)abcpack->angle;
					head.state2 = pstate;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_PACKLIMMIT;
					head.p = pointer;
					head.dx = movepackage.Px - abcpack->Px;
					head.dy = movepackage.Py - abcpack->Py;
					head.size1 = movepackage.x - abcpack->x;
					head.size2 = movepackage.y - abcpack->y;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_PACKID;
					head.p = pointer;
					head.dx = movepackage.IDx - abcpack->IDx;
					head.dy = movepackage.IDy - abcpack->IDy;
					head.size1 = abcpack->IDsize;
					head.size2 = movepackage.IDsize;
					head.state1 = abcpack->IDalign;
					head.state2 = movepackage.IDalign;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_PACKCOMMENT;
					head.p = pointer;
					head.dx = movepackage.Cx - abcpack->Cx;
					head.dy = movepackage.Cy - abcpack->Cy;
					head.size1 = abcpack->Csize;
					head.size2 = movepackage.Csize;
					head.state1 = abcpack->Calign;
					head.state2 = movepackage.Calign;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					
				    memcpy(pointer,temp,movepackage.total16byte*16);
					if(Ratsnestflag!=0) Show_Ratsnest(&dc);
					//OpenMouseCursor();
					goto LIMOVEEND;
				}
				else if(ch1==SPACE)
				{
					if(pstate<3)  pstate++;
					else if(pstate==3) pstate=0;
					else if(pstate==4) pstate=7;
					else if(pstate>4)  pstate--;
				}
				else Command_In_Function(ch1);
			}while(1);
		}
		else   Command_In_Function(ch1);
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));

LIMOVEEND:
	
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	

	if(ratsnestbak==0)
	{
		Show_Ratsnest(&dc);
		Ratsnestflag=0;
		Route_Mode=routemodebak;
	}
	else 
	{
		Show_Ratsnest(&dc);
		Route_Mode=routemodebak;
		Show_Ratsnest(&dc);
	}
	CL_Auto_Pan = OFF;
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	free(packagemem);
	free(packagebak);
	Write_Help(commandend);
	EnableFrame(TRUE);
}

void CDesignView::OnRankAll() 
{
	int i,k,l,placedesity;
	unsigned placex,placey,deltax,deltay,compmaxx,compmaxy;
	unsigned placebottomy,alreadyx,alreadyy;
	unsigned ch1;
	unsigned x1,y1,x2,y2,tempxy;
	int xs,ys,xe,ye,flag;
	char *p, *pointer,*temp,*packagemem;
	
	CClientDC dc(this);
	int drawmode = dc.GetROP2();
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if((packagemem=(char *)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}

	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(helpstr1);

	flag=0;
	x1=x2=0;y1=y2=0;
	do
	{
		ch1=MouseKey();
		if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
		{
			x1 = m_nCurrent_X;
			y1 = m_nCurrent_Y;
			Get_VPort_XY(&xs,&ys,x1,y1);
			xe=xs;
			ye=ys;
			Write_Help(helpstr2);
			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xs,&ys,x1,y1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC ))
				{
					CL_Auto_Pan = OFF;
					DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
					//setwritemode(1);
					//setcolor(Overwrite_Color);
					//rectangle(xs,ys,xe,ye);
					dc.SetROP2(R2_NOTXORPEN);
					rectangle(&dc,xs,ys,xe,ye);
					dc.SetROP2(drawmode);
					return;
				}
				if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{
						flag=1;
						CloseMouseCursor();
						//setwritemode(1);
						//setcolor(Overwrite_Color);
						//rectangle(xs,ys,xe,ye);
						OpenMouseCursor();
						dc.SetROP2(R2_NOTXORPEN);
						rectangle(&dc,xs,ys,xe,ye);
						dc.SetROP2(drawmode);
					}
					else
					{
						if(CL_RedrawFlag==1)
						{
							CL_RedrawFlag=0;
						}
						else 
						{
							//setwritemode(1);
							//setcolor(Overwrite_Color);
							//rectangle(xs,ys,xe,ye);      
							dc.SetROP2(R2_NOTXORPEN);
							rectangle(&dc,xs,ys,xe,ye);
							dc.SetROP2(drawmode);
						}
						x2 = m_nCurrent_X;
						y2 = m_nCurrent_Y;
						Get_VPort_XY(&xe,&ye,x2,y2);
						CloseMouseCursor();
						//setwritemode(1);
						//setcolor(Overwrite_Color);
						//rectangle(xs,ys,xe,ye);
						dc.SetROP2(R2_NOTXORPEN);
						rectangle(&dc,xs,ys,xe,ye);
						dc.SetROP2(drawmode);
						OpenMouseCursor();
					}
				}
				if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
				{  
					ch1 = VK_ESCAPE;
					CloseMouseCursor();
					//setwritemode(1);
					//setcolor(Overwrite_Color);
					//rectangle(xs,ys,xe,ye);
					dc.SetROP2(R2_NOTXORPEN);
					rectangle(&dc,xs,ys,xe,ye);
					dc.SetROP2(drawmode);
					OpenMouseCursor();
					break;
				}
				else Command_In_Function(ch1);
			}while(1);
		}
		else  Command_In_Function(ch1);
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));

    if((x1==x2)||(y1==y2)) 
	{
		CL_Auto_Pan = OFF;
		DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
		return;
	}
	
	if(x1>x2)
	{ 
		tempxy=x2;
		x2=x1;
		x1=tempxy;
	}
	if(y1>y2)
	{
		tempxy=y2;
		y2=y1;
		y1=tempxy;
	}
	CL_Auto_Pan = OFF;
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	
	pDoc->SetModifiedFlag(TRUE);

	placedesity = CL_Autoplace_Dx;
	
	placey=0;
	for(;;)
    {
		compmaxx = 0;
		compmaxy = 0;
		pointer = NULL;
	
		p = pDoc->m_pPackage_Buffer;
		for(i=0; i<pDoc->m_nPackage_Number; i++)
		{
			memcpy(&gnpackage,p,16);
			if((gnpackage.attrib==PCB)&&(gnpackage.flag==0))
			{
				temp = (char*)&gnpackage;
				memcpy(temp,p, 112);
				if((gnpackage.cx==12345)&&(gnpackage.cy==12345)&&((int)compmaxy<(gnpackage.Py-gnpackage.y)))
				{
					compmaxy=(gnpackage.Py-gnpackage.y);
					compmaxx=(gnpackage.Px-gnpackage.x);
					pointer = p;
				}
			}
			p += gnpackage.total16byte*16;
		}

		if(placey==0)
		{
			placex=x1+placedesity+200;
			placey=y1+compmaxy+placedesity;
			placebottomy=y1;
			alreadyy=placey;
			alreadyx=placex;
		}
		if(pointer == NULL) break;
		else
		{
			if(((alreadyy-placebottomy-placedesity)>(UINT)CL_Size_Maxy)||((alreadyy-placebottomy-placedesity)<compmaxy))
			{
				placex=alreadyx+placedesity;
				if(placex>x2)
				{
					placex=x1+placedesity+200;
					placebottomy=placey;
					placey+=compmaxy+placedesity;
					if(placey>y2) break;
				}
				alreadyy=placey;
				alreadyx=placex;
			}
			memcpy(&gnpackage,pointer,16);
			temp = packagemem;
			memcpy(temp,pointer,gnpackage.total16byte*16);
		    DrawDeletePackage(pointer,&dc);
		    temp = packagemem;
		    memcpy(&gnpackage,temp,112);
		    deltax = placex-gnpackage.cx;
		    if((gnpackage.x+deltax)>(UINT)CL_Size_Maxx) deltax = -gnpackage.x;
		    deltay = alreadyy-gnpackage.cy;
		    gnpackage.cx  += deltax;
		    gnpackage.cy  += deltay;
		    gnpackage.IDx += deltax;
		    gnpackage.IDy += deltay;
		    gnpackage.Cx  += deltax;
		    gnpackage.Cy  += deltay;
		    gnpackage.Px  += deltax;
		    gnpackage.Py  += deltay;
		    gnpackage.x   += deltax;
		    gnpackage.y   += deltay;
		    memcpy(temp,&gnpackage,112);
		    temp+=112;
		    for(l=0;l<gnpackage.padnum;l++)
			{ 
				memcpy(&gnjunction,temp,16);
				gnjunction.centerx += deltax;
				gnjunction.centery += deltay;
				memcpy(temp,&gnjunction,16);
				temp+=16;
			}
		    for(l=0;l<gnpackage.lnum;l++)
			{
				memcpy(&gnline,temp,16);
				gnline.startx += deltax;
				gnline.starty += deltay;
				gnline.endx += deltax;
				gnline.endy += deltay;
				memcpy(temp,&gnline,16);
				temp+=16;
			}
		    for(l=0;l<gnpackage.jnum;l++)
			{  
				memcpy(&gnjunction,temp,16);
				gnjunction.centerx += deltax;
				gnjunction.centery += deltay;
				memcpy(temp,&gnjunction,16);
				temp+=16;
			}
		    for(l=0;l<gnpackage.snum;l++)
			{  
				memcpy(&gnstring,temp,64);
				gnstring.sx += deltax;
				gnstring.sy += deltay;
				memcpy(temp,&gnstring,64);
				temp+=64;
			}
		    for(l=0;l<gnpackage.anum;l++)
			{  
				memcpy(&gnarc,temp,16);
				gnarc.centerx += deltax;
				gnarc.centery += deltay;
				memcpy(temp,&gnarc,16);
				temp+=16;
			}
		    for(l=0;l<gnpackage.fnum;l++)
			{ 
				memcpy(&gnfill,temp,16);
				memcpy(&gnfill,temp,gnfill.total16byte*16);
				for(k=0;k<gnfill.vertexnum;k++)
				{
					gnfill.pline[k].x += deltax;
					gnfill.pline[k].y += deltay;
				}
				memcpy(temp,&gnfill,gnfill.total16byte*16);
				temp+=gnfill.total16byte*16;
			}

		    temp = packagemem;
			memcpy(pointer,temp,gnpackage.total16byte*16);
		    DrawPackage(temp,&dc);
		    alreadyy -= compmaxy + placedesity/2;
		    if((placex+compmaxx)>alreadyx) alreadyx=placex+compmaxx;
		}
    }
    free(packagemem);
	Write_Help(commandend);
}

void CDesignView::OnLoadPackage() 
{
	int i,j;
	int missingcomp,missingpins,extrapins;
	char str_ID[30],str_P[30],str_C[30], string[60];
	char *tempNet_Compbuf;
	char *packagemem,*temp;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Compnum==0) 
	{
		Error( error16 );
		return;
	}
	missingcomp = missingpins = extrapins = 0;
	if(PCBLibnum <= 0) 
	{
		Error(error10);
		return;
	}
 	pDoc->SetModifiedFlag(TRUE);
	
	if((packagemem=(char *)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}
	tempNet_Compbuf = pDoc->m_pNet_Compbuf;
	for(i=0;i<pDoc->m_nNet_Compnum;i++)
	{
		memcpy(str_ID,tempNet_Compbuf,strlen(tempNet_Compbuf)+1);
		tempNet_Compbuf+=strlen(tempNet_Compbuf)+1;
		memcpy(str_P,tempNet_Compbuf,strlen(tempNet_Compbuf)+1);
		tempNet_Compbuf+=strlen(tempNet_Compbuf)+1;
		memcpy(str_C,tempNet_Compbuf,strlen(tempNet_Compbuf)+1);
		tempNet_Compbuf+=strlen(tempNet_Compbuf)+1;
		j = Package_Exist(str_ID);
		if(j==-1) continue;
		j = Get_Package(packagemem,str_P);
		if(j==-1)
		{
			missingcomp++;
			continue;
	    }
		strcpy(gnpackage.ID,str_ID);
		strcpy(gnpackage.P,str_P);
		strcpy(gnpackage.C,str_C);
		temp=packagemem;
		memcpy(temp,&gnpackage,112);
		pDoc->Save_EMMPackage(packagemem,gnpackage.total16byte);
		DrawPackage(packagemem,&dc);
	}
	if(missingcomp==0) AfxMessageBox("已调入所有元件的封装.", MB_OK|MB_ICONINFORMATION);
	if(missingcomp!=0)
	{
		itoa(missingcomp,str_ID,10);
		strcpy(string,"丢失元件数 = ");
		strcat(string,str_ID);
		Error(string);
	}
	free(packagemem);	
}


int CDesignView::Get_Package_In_Sequence(char * * pointer)
{
	char *p, *pp,*temp;
	int i, k, ii;
	struct Package *onepackage, *otherpackage, mainpackage;
	int relationnum, relationnumbak;

	int netnum,m,n;
	int flag1, flag2;
	char *tempNet_Pins;
	char str1[30],strid1[30],strnum1[10];
	unsigned char numstate[256], numstatebak[256];
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();


	strcpy(gnpackage.ID,Null_Str);
	relationnum = relationnumbak = -1;
 	p = pDoc->m_pPackage_Buffer;
	for(i=0; i<pDoc->m_nPackage_Number; i++)
	{
		onepackage = (struct Package*)p;
	    if((onepackage->flag==0)&&(onepackage->cx==12345)&&(onepackage->cy==12345))
		{
			temp = p;
			memcpy(&mainpackage,temp,112);
			if(strcmp(gnpackage.ID,mainpackage.ID)!=0)
			{
				for(k=0;k<256;k++) numstate[k]=0;
				strcpy(gnpackage.ID,mainpackage.ID);
		    }

			relationnum = 0;
			pp = pDoc->m_pPackage_Buffer;
			for(ii=0; ii<pDoc->m_nPackage_Number; ii++)
			{
				otherpackage = (struct Package*)pp;
				if((otherpackage->flag==0)&&((otherpackage->cx!=12345)||(otherpackage->cy!=12345)))
				{
					temp = pp;
					for(netnum=0;netnum<pDoc->m_nNet_Number;netnum++)
					{
						for(m=0;m<256;m++) numstatebak[m]=0;
						flag1=flag2=0;
						tempNet_Pins = pDoc->m_ppNet_Pinspointer[netnum];
						for(;;)
						{
							memcpy(str1,tempNet_Pins,strlen(tempNet_Pins)+1);
							tempNet_Pins+=strlen(tempNet_Pins)+1;
							memcpy(&k,tempNet_Pins,2);
							tempNet_Pins+=2;
							n=strlen(str1);
							for(m=n;m>=0;m--)
							{
								if(str1[m]=='-')
								{
									memcpy(strid1,str1,m);
									strid1[m]='\0';
									memcpy(strnum1,str1+m+1,n-m-1);
									strnum1[n-m-1]='\0';
									break;
								}
							}
							if(strcmp(otherpackage->ID,strid1)==0) flag2=1;
							if(strcmp(mainpackage.ID,strid1)==0)
						    {
								n=(unsigned)atoi(strnum1);
								if(numstate[n]==0)
								{
									flag1++;
									numstatebak[n]=1;
								}
							}
							if(tempNet_Pins==pDoc->m_ppNet_Pinspointer[netnum+1]) break;
						}
						if(flag2!=0) 
						{
							relationnum+=flag1;
							for(m=0;m<256;m++)
							{
								if(numstate[m]!=1) numstate[m]=numstatebak[m];
							}
						}
					}
				}
				pp += otherpackage->total16byte*16;
			}
			if(relationnum>relationnumbak)
			{
				*pointer = p;
				relationnumbak = relationnum;
			}
		}
		p += onepackage->total16byte*16;
	}

   if(relationnumbak>=0)
   {
	   //*pointer = page;
	   //*bakoffset=offset;
	   return 0;
   }
   return -1;
}


int CDesignView::Placement_Valid1(char * packagemem, int movepackage_padnum,\
	  int movepackage_lnum, UINT x_min, UINT y_min, UINT x_max, UINT y_max)
{
	int i;
	int invalidflag;
	char *temp;
	CClientDC dc(this);
	
	CloseMouseCursor();
	invalidflag=0;
	temp=packagemem;
	temp+=112;
	for(i=0;i<movepackage_padnum;i++)
	{
		memcpy(&gnjunction,temp,16);
		temp+=16;
		if((gnjunction.centerx<CL_Size_Maxx)&&(gnjunction.centery<CL_Size_Maxy)&&\
			(gnjunction.centerx>x_min)&&(gnjunction.centerx<x_max)&&\
			(gnjunction.centery>y_min)&&(gnjunction.centery<y_max))
			continue;
		else if((gnjunction.centery>y_max)&&(gnjunction.centery<CL_Size_Maxy))
		{
			invalidflag=3;
			break;
		}
		else if((gnjunction.centery>CL_Size_Maxy)||\
		  (gnjunction.centery<y_min)||\
		  ((gnjunction.centerx>x_max)&&(gnjunction.centerx<CL_Size_Maxx)))
	    {
			invalidflag=2;
			break;
	    }
		else 
		{
			invalidflag=1;
	    }
	}
	if(invalidflag==0)
    {
		for(i=0;i<movepackage_lnum;i++)
		{
			memcpy(&gnline,temp,16);
			temp+=16;
			if(DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc)==-1)
		    {
				invalidflag=1;
		    }
			DrawAnyLine(gnline.startx,gnline.starty,gnline.endx,gnline.endy,SCH_Color[13],1,&dc);
		}
    }
	OpenMouseCursor();
	return invalidflag;
}



int CDesignView::Placement_Valid2(char * packagemem, int movepackage_padnum, \
								  int movepackage_lnum)
{
	int invalidflag;
	int i;
	int delta=0;
	char *temp, *p;
	int tmax_x,tmin_x,tmax_y,tmin_y;
	unsigned pmax_x,pmin_x,pmax_y,pmin_y;
	struct Package *onepackage;
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	invalidflag = 0;
	tmax_x = 0; 
	tmin_x = CL_Size_Maxx; 
	tmax_y = 0; 
	tmin_y = CL_Size_Maxy;
	
	temp = packagemem;
	temp += 112;
	for(i=0;i<movepackage_padnum;i++)
	{
		memcpy(&gnjunction,temp,16);  temp+=16;
		if(tmax_x<(gnjunction.centerx+gnjunction.Dx)) tmax_x=gnjunction.centerx+gnjunction.Dx;
		if(tmax_y<(gnjunction.centery+gnjunction.Dy)) tmax_y=gnjunction.centery+gnjunction.Dy;
		if(tmin_x>(gnjunction.centerx-gnjunction.Dx)) tmin_x=gnjunction.centerx-gnjunction.Dx;
		if(tmin_y>(gnjunction.centery-gnjunction.Dy)) tmin_y=gnjunction.centery-gnjunction.Dy;
	}
	for(i=0;i<movepackage_lnum;i++)
	{
		memcpy(&gnline,temp,16);  temp+=16;
		if(tmax_x<gnline.startx) tmax_x=gnline.startx;
		if(tmax_y<gnline.starty) tmax_y=gnline.starty;
		if(tmin_x>gnline.startx) tmin_x=gnline.startx;
		if(tmin_y>gnline.starty) tmin_y=gnline.starty;
		if(tmax_x<gnline.endx) tmax_x=gnline.endx;
		if(tmax_y<gnline.endy) tmax_y=gnline.endy;
		if(tmin_x>gnline.endx) tmin_x=gnline.endx;
		if(tmin_y>gnline.endy) tmin_y=gnline.endy;
	}

 	p = pDoc->m_pPackage_Buffer;
	for(i=0; i<pDoc->m_nPackage_Number; i++)
	{
		onepackage = (struct Package*)p;
	    if(onepackage->flag==0)
		{
			temp = p;
			pmax_x=onepackage->Px;
			pmax_y=onepackage->Py;
			pmin_x=onepackage->x;
			pmin_y=onepackage->y;
			if(Placement_Connect(tmax_x,tmax_y,tmin_x,tmin_y,pmax_x,pmax_y,pmin_x,pmin_y)==1)
			{
				return 1;
			}
	    }
		p += onepackage->total16byte*16;
	}

  return 0;
}

int CDesignView::Placement_Connect(UINT x_max, UINT y_max, UINT x_min, \
			UINT y_min, UINT xx_max, UINT yy_max, UINT xx_min, UINT yy_min)
{
	int d = 50;
	x_max += d;
	y_max += d;
	x_min -= d;	if(x_min>(UINT)CL_Size_Maxx) x_min = 0;
	y_min -= d;	if(y_min>(UINT)CL_Size_Maxy) y_min = 0;
	if(((x_max>=xx_min)&&(x_max<=xx_max)&&(y_max>=yy_min)&&(y_max<=yy_max))||\
       ((x_min>=xx_min)&&(x_min<=xx_max)&&(y_max>=yy_min)&&(y_max<=yy_max))||\
       ((x_max>=xx_min)&&(x_max<=xx_max)&&(y_min>=yy_min)&&(y_min<=yy_max))||\
       ((x_min>=xx_min)&&(x_min<=xx_max)&&(y_min>=yy_min)&&(y_min<=yy_max))||\
       ((xx_max>=x_min)&&(xx_max<=x_max)&&(yy_max>=y_min)&&(yy_max<=y_max))||\
       ((xx_min>=x_min)&&(xx_min<=x_max)&&(yy_max>=y_min)&&(yy_max<=y_max))||\
       ((xx_max>=x_min)&&(xx_max<=x_max)&&(yy_min>=y_min)&&(yy_min<=y_max))||\
       ((xx_min>=x_min)&&(xx_min<=x_max)&&(yy_min>=y_min)&&(yy_min<=y_max))||\
       ((x_min>=xx_min)&&(x_max<=xx_max)&&(y_min<=yy_min)&&(y_max>=yy_max))||\
       ((xx_min>=x_min)&&(xx_max<=x_max)&&(yy_min<=y_min)&&(yy_max>=y_max)))
       return 1;
	else return 0;
}


int CDesignView::Package_Exist(char * strid)
{
	int i;
	char *p,*pointer,*temp;
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	p = pDoc->m_pPackage_Buffer;
	for(i=0; i<pDoc->m_nPackage_Number; i++)
	{
		pointer = p;
		memcpy(&gnpackage,pointer,16);
		if((gnpackage.attrib==PCB)&&(gnpackage.flag==0))
		{
			temp = (char*)&gnpackage;
			memcpy(temp,pointer,112);
			if(strcmp(gnpackage.ID,strid)==0) return -1;
		}
		p+=gnpackage.total16byte*16;
	}

   return 0;
}

int CDesignView::Get_Package(char * packmem, char * string)
{
	char libstring[200], str[40];
	unsigned length;
	int abc1;
	int i,j,l;
	struct Packagehead packagehead;
	char *pointer,*libmem,**libname,*tempmem,**tempname;
	int flag;
	long int fpoffset;
	FILE *libfp;

	int tmax_x,tmin_x,tmax_y,tmin_y;
	long int ll;
	char *temp;
	char obj[16];
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	
	strupr(string);
	for(i=0;i<PCBLibnum;i++)
	{
		flag = 0;
		
	    pointer = pcb_library[i].position;
	    length = pcb_library[i].length;
	    
		if((libmem=(char *)malloc(length))==NULL)
		{
			memory_error();
			return -1;
		}
		if((libname=(char **)malloc(pcb_library[i].totalnum*4))==NULL)
		{
			memory_error();
			return -1;
		}

		tempmem = libmem;
		tempname = libname;
		memcpy(tempmem,pointer,length);
		tempmem = libmem;
	    l = 0;
	    for(j=0;j<pcb_library[i].totalnum;j++)
		{
			tempname[j] = tempmem+l;
		    abc1 = strlen(tempname[j]);
		    memcpy(str,tempname[j],abc1+1);
			if(strcmp(str,string)==0)
			{
				flag=1;
				break;
			}
		    for(;;)
			{
				if(tempmem[l]==0) 
				{
					l++;
					break;
				}
				else l++;
			}
		}
	    
		if(flag==1)
		{
		     sprintf(libstring,"%s\\package\\%s",System_Path,pcb_library[i].libname);
		     if((libfp=fopen(libstring,"rb"))==NULL)
			 {
				 Error( error8 );
				 free(libmem);
				 free(libname);
				 return -1;
			 }
		     fpoffset=16+(long)j*32;
		     fseek(libfp,fpoffset,0);
		     fread(&packagehead,1,32,libfp);
		     ll=((long)packagehead.offset)&0x0000ffff;
		     ll=ll*16;
		     temp=packmem;
		     temp+=112;
		     gnpackage.padnum=0;
		     gnpackage.lnum=0;
		     gnpackage.jnum=0;
		     gnpackage.snum=0;
		     gnpackage.anum=0;
		     gnpackage.fnum=0;
		     tmax_x=0; tmin_x=CL_Size_Maxx; tmax_y=0; tmin_y=CL_Size_Maxy;
		     for(j=0;j<5;j++)
			 {
				fseek(libfp,ll,0);
				i=0;
				for(;;)
				{
					if(i==packagehead.objectnum) break;
					fread(obj,1,16,libfp);
					if(obj[1]==1)
					{
						if(j==4) 
						{
							gnarc.id=0x60;
							gnarc.centerx = (obj[2]&0x00ff)+((obj[3]&0x00ff)<<8);
							gnarc.centerx+=12345;
							gnarc.centery = (obj[4]&0x00ff)+((obj[5]&0x00ff)<<8);
							gnarc.centery+=12345;
							gnarc.radius  = (obj[6]&0x00ff)+((obj[7]&0x00ff)<<8);
							gnarc.linewidth = obj[9]&0x00ff;
							gnarc.attrib = m_nSystem_State;
							gnarc.layer = (obj[0]&0x00ff)-1;
							gnarc.u1=gnarc.flag=0;
							if((obj[8]&0x00ff)==0xff)
							{
								gnarc.stangle = (obj[10]&0x00ff)+((obj[11]&0x00ff)<<8);
								gnarc.endangle = (obj[12]&0x00ff)+((obj[13]&0x00ff)<<8);
							}
							else 
							{
						       switch(obj[8]) 
							   {
								case 1: gnarc.stangle=0;
										gnarc.endangle=90;
										break;
								case 2: gnarc.stangle=90;
										gnarc.endangle=180;
										break;
								case 3: gnarc.stangle=0;
										gnarc.endangle=180;
										break;
								case 4: gnarc.stangle=180;
										gnarc.endangle=270;
										break;
								case 6: gnarc.stangle=90;
										gnarc.endangle=270;
										break;
								case 7: gnarc.stangle=0;
										gnarc.endangle=270;
										break;
								case 8: gnarc.stangle=270;
										gnarc.endangle=360;
										break;
								case 9: gnarc.stangle=270;
										gnarc.endangle=450;
										break;
								case 11:gnarc.stangle=270;
										gnarc.endangle=540;
										break;
								case 12:gnarc.stangle=180;
										gnarc.endangle=360;
										break;
								case 13:gnarc.stangle=180;
										gnarc.endangle=450;
										break;
								case 14:gnarc.stangle=90;
										gnarc.endangle=360;
										break;
								case 15:gnarc.stangle=0;
										gnarc.endangle=360;
										break;
								default:gnarc.stangle=0;
										gnarc.endangle=0;
										break;
								}
							}
							memcpy(temp,&gnarc,16);
							temp+=16;
							i++;
							gnpackage.anum++;
							continue;
						}
						else 
						{
							i++;
							continue;
						}
					}
					else if(obj[1]==2) 
					{
						i++;
						continue;
					}
					else if(obj[1]==3)
					{ 
						if(j==0) 
						{
							gnjunction.id=0x21;
							gnjunction.centerx=(obj[2]&0x00ff)+((obj[3]&0x00ff)<<8);
							gnjunction.centery=(obj[4]&0x00ff)+((obj[5]&0x00ff)<<8);
							gnjunction.centerx+=12345;
							gnjunction.centery+=12345;
							gnjunction.Dx=obj[6]&0x00ff;
							gnjunction.Dy=obj[8]&0x00ff;
							gnjunction.d=obj[10]&0x00ff;
							gnjunction.attrib = m_nSystem_State;
							gnjunction.layer=obj[0]&0x00ff;
							if(gnjunction.layer==13) gnjunction.layer=16;
							else gnjunction.layer--;
							if(obj[12]==3) gnjunction.padtype=1;
							else if(obj[12]==4) gnjunction.padtype=3;
							else gnjunction.padtype=obj[12]&0x00ff;
							gnjunction.flag=0;
							fread(obj,1,16,libfp);
							gnjunction.padname[0]=obj[3];
							gnjunction.padname[1]=obj[4];
							gnjunction.padname[2]=obj[5];
							gnjunction.padname[3]=0;
							memcpy(temp,&gnjunction,16);
							if(tmax_x<(gnjunction.centerx+gnjunction.Dx)) tmax_x=gnjunction.centerx+gnjunction.Dx;
							if(tmax_y<(gnjunction.centery+gnjunction.Dy)) tmax_y=gnjunction.centery+gnjunction.Dy;
							if(tmin_x>(gnjunction.centerx-gnjunction.Dx)) tmin_x=gnjunction.centerx-gnjunction.Dx;
							if(tmin_y>(gnjunction.centery-gnjunction.Dy)) tmin_y=gnjunction.centery-gnjunction.Dy;
							temp+=16;
							i++;
							gnpackage.padnum++;
							continue;
						}
						else 
						{
							fread(obj,1,16,libfp);
							i++;
							continue;
						}
					}
					else if(obj[1]==5)
					{
						if(j==1)
						{
							gnline.id=0x10;
							gnline.startx=(obj[2]&0x00ff)+((obj[3]&0x00ff)<<8);
							gnline.startx+=12345;
							gnline.starty=(obj[4]&0x00ff)+((obj[5]&0x00ff)<<8);
							gnline.starty+=12345;
							gnline.endx=(obj[6]&0x00ff)+((obj[7]&0x00ff)<<8);
							gnline.endx+=12345;
							gnline.endy=(obj[8]&0x00ff)+((obj[9]&0x00ff)<<8);
							gnline.endy+=12345;
							gnline.linewidth=obj[10]&0x00ff;
							gnline.type=80;
							gnline.attrib=m_nSystem_State;
							gnline.layer=(obj[0]&0x00ff)-1;
							gnline.u1=gnline.u2=gnline.flag=0;
							memcpy(temp,&gnline,16);
							if(tmax_x<gnline.startx) tmax_x=gnline.startx;
							if(tmax_y<gnline.starty) tmax_y=gnline.starty;
							if(tmin_x>gnline.startx) tmin_x=gnline.startx;
							if(tmin_y>gnline.starty) tmin_y=gnline.starty;
							if(tmax_x<gnline.endx) tmax_x=gnline.endx;
							if(tmax_y<gnline.endy) tmax_y=gnline.endy;
							if(tmin_x>gnline.endx) tmin_x=gnline.endx;
							if(tmin_y>gnline.endy) tmin_y=gnline.endy;
							temp+=16;
							i++;
							gnpackage.lnum++;
							continue;
						}
						else
						{
							i++;
							continue;
						}
					}
					else if(obj[1]==6)
					{
						if(j==3) 
						{
							gnstring.id=0x30;
							gnstring.sx=(obj[2]&0x00ff)+((obj[3]&0x00ff)<<8);
							gnstring.sx+=12345;
							gnstring.sy=(obj[4]&0x00ff)+((obj[5]&0x00ff)<<8);
							gnstring.sy+=12345;
							gnstring.height=obj[6]+(obj[7]<<8);
							gnstring.align=obj[10];
							gnstring.layer=(obj[0]&0x00ff)-1;
							gnstring.attrib=m_nSystem_State;
							gnstring.flag=0;
							fread(obj,1,16,libfp);
							strcpy(gnstring.text,&obj[3]);
							memcpy(temp,&gnstring,64);
							temp+=64;
							i++;
							gnpackage.snum++;
							continue;
						}
						else
						{
							fread(obj,1,16,libfp);
							i++;
							continue;
						}
					}
					else if(obj[1]==8)
					{
						if(j==2) 
						{
							gnjunction.id=0x22;
							gnjunction.centerx=(obj[2]&0x00ff)+((obj[3]&0x00ff)<<8);
							gnjunction.centerx+=12345;
							gnjunction.centery=(obj[4]&0x00ff)+((obj[5]&0x00ff)<<8);
							gnjunction.centery+=12345;
							gnjunction.Dx=gnjunction.Dy=obj[6];
							gnjunction.d=obj[7];
							gnjunction.attrib=m_nSystem_State;
							gnjunction.layer=16;
							gnjunction.padtype=1;
							gnjunction.flag=0;
							memcpy(temp,&gnjunction,16);
							temp+=16;
							i++;
							gnpackage.jnum++;
							continue;
						}
						else 
						{
							i++;
							continue;
						}
					}
					else i++;
				}
			}
			gnpackage.id=0x80;
		    gnpackage.cx=12345;
		    gnpackage.cy=12345;
		    gnpackage.angle=0;
		    gnpackage.total16byte = 7 + gnpackage.padnum + gnpackage.lnum + gnpackage.jnum + gnpackage.snum*4 + gnpackage.anum ;
		    gnpackage.type = 0 ;
		    gnpackage.layer = (unsigned char)m_nPCB_Current_Layer;
		    gnpackage.u1 = 0;
		    gnpackage.attrib = PCB;
		    gnpackage.IDwidth=gnpackage.Cwidth=PCB_Stringwidth;
		    gnpackage.flag=0;
		    gnpackage.IDx=12345;
		    gnpackage.IDy=12345+150;
		    gnpackage.IDsize=PCB_Component_ID_Size;
		    gnpackage.IDstate=0+(TOPOVERLAY<<2);
		    gnpackage.Px=tmax_x;
		    gnpackage.Py=tmax_y;
		    gnpackage.x = tmin_x;
		    gnpackage.y = tmin_y;
		    gnpackage.IDalign=0;
		    gnpackage.Calign=0;
		    gnpackage.Pstate=1+(TOPOVERLAY<<2);
		    gnpackage.Cx=gnpackage.IDx;
		    gnpackage.Cy=gnpackage.IDy-PCB_Component_Comment_Size;
		    gnpackage.Csize=PCB_Component_Comment_Size;
		    gnpackage.Cstate=0+(TOPOVERLAY<<2);
		    fclose(libfp);
		    free(libmem);
		    free(libname);
		    return 0;
		}
	    free(libmem);
		free(libname);
	}
	return -1;
}

