void CDesignView::OnAutoHighlightLocation() 
{
	int i,flag,lmode;
	unsigned ch1;
	unsigned x1,y1;
	char *pointer;
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
	

	if(PCBHighlightBufferInit() != 0) return;
	temptrackbuf = Highlight_Trackbuf;
	temppadbuf = Highlight_Padbuf;
	temparcfillbuf = Highlight_ArcFillbuf;
	temppackbuf = Highlight_Packagebuf;
	
	flag=0;
	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	DrawStructButton(ID_AUTO_HIGHLIGHT_LOCATION,TRUE);
	Write_Help(helpstr60);
	do 
	{
		ch1 = MouseKey();
		if((ch1==VK_RETURN)||(ch1==PUSHMOUSE))
		{
			CloseMouseCursor();
			DrawPCBHighlight(0);
			OpenMouseCursor();
			for(i=0;i<HL_lnum;i++) Highlight_Trackbuf[i]=0;
			for(i=0;i<HL_pnum;i++) Highlight_Padbuf[i]=0;
			for(i=0;i<HL_afnum;i++) Highlight_ArcFillbuf[i]=0;
			for(i=0;i<HL_packnum;i++) Highlight_Packagebuf[i]=0;
			TempTrackbuf = Highlight_Trackbuf;
			TempPadbuf = Highlight_Padbuf;
			TempArcFillbuf = Highlight_ArcFillbuf;
			TempPackagebuf = Highlight_Packagebuf;
			temptrackbuf = Highlight_Trackbuf;
			temppadbuf = Highlight_Padbuf;
			temparcfillbuf = Highlight_ArcFillbuf;
			temppackbuf = Highlight_Packagebuf;
			flag = 0;

			x1 = m_nCurrent_X;
			y1 = m_nCurrent_Y;
			lmode = 0;
			if(Select_Line(x1,y1,&pointer) != 0) lmode = 1;
			else if(Select_JP(x1,y1,&pointer) != 0) lmode = 2;
			else if(Select_PackagePad(x1,y1,&pointer,&i) != 0) lmode = 3;
			else if(Select_Arc(x1,y1,&pointer) != 0) lmode = 4;
			else if(Select_Fill(x1,y1,&pointer)!= 0) lmode = 5;
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
				flag = 1;
				highlighttrack.pointer = pointer;
				memcpy(TempTrackbuf,&highlighttrack,4);
				TempTrackbuf += 4;
				CloseMouseCursor();
				PCBMakeHighlight();
				OpenMouseCursor();
			}
			else if(lmode==2)
			{
				memcpy(&gnjunction,pointer,16);
				if((gnjunction.layer==MULTILAYER)||\
					(gnjunction.layer==TOPLAYER)||\
					(gnjunction.layer==BOTTOMLAYER)) flag=1;
				else
				{
					MessageBeep(0xFFFFFFFF);
					continue;
				}
				highlightjunc.pointer = pointer;
				memcpy(TempPadbuf,&highlightjunc,4);
				TempPadbuf += 4;
				CloseMouseCursor();
				PCBMakeHighlight();
				OpenMouseCursor();
			}
			else if(lmode==3)
			{
				flag = 1;
				highlightpack.pointer = pointer;
				highlightpack.padnum = (short int)i;
				memcpy(TempPackagebuf,&highlightpack,6);
				TempPackagebuf += 6;
				CloseMouseCursor();
				PCBMakeHighlight();
				OpenMouseCursor();
			}
			else if(lmode==4)
			{
				flag=1;
				highlightarcfill.pointer = pointer;
				memcpy(TempArcFillbuf,&highlightarcfill,4);
				TempArcFillbuf += 4;
				CloseMouseCursor();
				PCBMakeHighlight();
				OpenMouseCursor();
			}
			else if(lmode==5)
			{
				flag = 1;
				highlightarcfill.pointer = pointer;
				memcpy(TempArcFillbuf,&highlightarcfill,4);
				TempArcFillbuf += 4;
				CloseMouseCursor();
				PCBMakeHighlight();
				OpenMouseCursor();
			}
		}
		else Command_In_Function(ch1);
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));

	//Mouse_Free();
	CL_Auto_Pan = OFF;
	//CloseMouseCursor();
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	DrawStructButton(ID_AUTO_HIGHLIGHT_LOCATION,FALSE);
	//OpenMouseCursor();
	if(flag==0)
    {
		free(Highlight_Trackbuf);
		free(Highlight_Padbuf);
		free(Highlight_ArcFillbuf);
		free(Highlight_Packagebuf);
		Highlight_Trackbuf=NULL;
		Highlight_Padbuf=NULL;
		Highlight_ArcFillbuf=NULL;
		Highlight_Packagebuf=NULL;
    }
	Write_Help(commandend);
}

void CDesignView::OnAutoHighlightNet() 
{
	int i,j,m,n,l;
	int ch1;
	char *pointer, *temp, *packmem, *nettemp, *p;
	char *temptrackbuf, *temppadbuf, *temparcfillbuf, *temppackbuf;
	struct hpack
	{ 
		short int padnum;
		char* pointer;    } highlightpack;
	char str1[30],str2[10];
	char **nets;
	char str[100];
	char netstr[100];
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if( pDoc->m_nNet_Number == 0) 
	{
		Error( error16 );
		return;
	}

	// 定义网络名称
	CTextInput hzin;
	hzin.m_MaxLength = 32;
	hzin.m_Title = gnhelpstr40;
	hzin.m_Text_Input = Null_Str;

	Write_Help(gnhelpstr100);
	i = hzin.DoModal();
	if(i == IDCANCEL) 
	{
		Write_Help(commandend);
		return;
	}
	strcpy(str,hzin.m_Text_Input);

	if((nets = (char**)malloc(4000))==NULL)
	{
		memory_error();
		return;
	}
	temp = pDoc->m_pNet_Netnamebuf;
	
	j = 0;
	for(i=0;i<pDoc->m_nNet_Number;i++)
	{
		nets[i] = temp;
		if(strcmp(temp,str)==0) j = 1;
		temp += strlen(temp)+1;
	}
	if(j==0)
	{
		CSelectionDlg selection;
		selection.m_TitleText = select_net;
		selection.m_pList = nets;
		selection.m_nNum = pDoc->m_nNet_Number;
		i = selection.DoModal();
		if(i != IDOK) 
		{
			free(nets);
			Write_Help(commandend);
			return ;     
		}
		strcpy(str, selection.m_Selected_Text);
	}

	temp = pDoc->m_pNet_Netnamebuf;
	for(ch1=0;ch1<pDoc->m_nNet_Number;ch1++)
    { 
		if(strcmp(temp,str)==0) break;
		temp+=strlen(temp)+1;
    }
	free(nets);
	if(ch1 == pDoc->m_nNet_Number) 
	{
		Write_Help(commandend);
		return;
	}
	

	strcpy(netstr,str);
	if((packmem = (char*)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}

	if(PCBHighlightBufferInit() != 0) return;
	temptrackbuf = Highlight_Trackbuf;
	temppadbuf = Highlight_Padbuf;
	temparcfillbuf = Highlight_ArcFillbuf;
	temppackbuf = Highlight_Packagebuf;

	nettemp = pDoc->m_ppNet_Pinspointer[ch1];
	for(;;)
	{
		if(nettemp == pDoc->m_ppNet_Pinspointer[ch1+1]) break;
		memcpy(str,nettemp,strlen(nettemp)+1);
		nettemp+=strlen(nettemp)+1;
		nettemp+=2;
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

		temp = packmem;
		p = pDoc->m_pPackage_Buffer;
		for(i=0; i<pDoc->m_nPackage_Number; i++)
		{
			pointer = p;
			memcpy(&gnpackage,pointer,16);
			if(gnpackage.flag==0)
			{
				temp = packmem;
				memcpy(temp,pointer,gnpackage.total16byte*16);
				temp = packmem;
				memcpy(&gnpackage,temp,112);
				if(strcmp(gnpackage.ID,str1) == 0)
				{
					temp+=112;
					for(l=0;l<gnpackage.padnum;l++)
					{
						memcpy(&gnjunction,temp,16);
						temp+=16;
						if(strcmp((char*)gnjunction.padname,str2)==0)
						{
							temppackbuf = Highlight_Packagebuf;
							highlightpack.pointer = p;
							highlightpack.padnum = l;
							memcpy(TempPackagebuf,&highlightpack,6);
							TempPackagebuf += 6;
							goto TEMPB;
						}
					}
				}
			}
			p += gnpackage.total16byte*16; 
		}
TEMPB:
		continue;
	}
	free(packmem);
	PCBMakeHighlight();	
	Write_Help(commandend);
}

void CDesignView::OnAutoHighlightReset() 
{
	DrawPCBHighlight(0);
	free(Highlight_Trackbuf);
	free(Highlight_Padbuf);
	free(Highlight_ArcFillbuf);
	free(Highlight_Packagebuf);
	Highlight_Trackbuf   = NULL;
	Highlight_Padbuf     = NULL;
	Highlight_ArcFillbuf = NULL;
	Highlight_Packagebuf = NULL;
}

void CDesignView::OnAutoHighlightList() 
{
	char **pinname;
	char *pinmem,*temppinmem;
	char *packmem,*pointer,*temp;
	char string[30],string1[30],string2[30];
	int k,num;
	char *temppackbuf;
	struct hpack
	{
		short int padnum;
		char* pointer;    } highlightpack;

	if((Highlight_Trackbuf==NULL)||(Highlight_Padbuf==NULL)||\
     (Highlight_ArcFillbuf==NULL)||(Highlight_Packagebuf==NULL))
	{
		Error( error23 );
		return;
	}
	if((pinname = (char**)malloc(4000))==NULL)
	{
		memory_error();
		return;
	}
	if((pinmem = (char*)malloc(22000))==NULL)
	{
		memory_error();
		return;
	}
	temppinmem = pinmem;
	if((packmem = (char*)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}

	temppackbuf = Highlight_Packagebuf;
	num=0;
	for(;;)
	{
		memcpy(&highlightpack,temppackbuf,6);
		temppackbuf += 6;
		if(highlightpack.pointer == NULL) break;
		pointer = highlightpack.pointer;
		memcpy(&gnpackage,pointer,16);
		temp = packmem;
		memcpy(temp,pointer,gnpackage.total16byte*16);
		temp = packmem;
		memcpy(&gnpackage,temp,112);
		strcpy(string1,gnpackage.ID);
		temp += 112;
		for(k=0;k<gnpackage.padnum;k++)
		{
			memcpy(&gnjunction,temp,16);
			temp+=16;
			if(highlightpack.padnum==k) 
			{
				strcpy(string2,(char*)gnjunction.padname);
				break;
		    }
		}
		sprintf(string,"%s-%s",string1,string2);
		memcpy(temppinmem,string,strlen(string)+1);
		pinname[num] = temppinmem;
		temppinmem += strlen(string) + 1;
		num++;
	}
	
	CSelectionDlg selection;
	selection.m_TitleText = "高亮引脚";  //"Highlight pin";
	selection.m_pList = pinname;
	selection.m_nNum = num;

	//Write_Help("Show the list of highlighted pins");
	Write_Help("显示高亮引脚清单");
	selection.DoModal();

	free(pinmem);
	free(packmem);
	free(pinname);
	Write_Help(commandend);
}

void CDesignView::OnAutoNetLoad() 
{
	char *comp;
	int i,n=50, notuse=0;
	FILE *netfile;
	char netfilename[200];
	char s[200];
	char *tempNet_Compbuf,*tempNet_Netnamebuf,*tempNet_Pins;

	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	//Write_Help("Load netlist");
	Write_Help("调入网络表");
	comp = pDoc->m_pNet_Compbuf;
	if(pDoc->m_nNet_Number != 0)
    { 
		//i = AfxMessageBox("The original netlist will be overwritten, please confirm.", MB_YESNO|MB_ICONQUESTION);
		i = AfxMessageBox("原来的网络表将被覆盖，确认?", MB_YESNO|MB_ICONQUESTION);
		if(i != IDYES)	
		{
			Write_Help(commandend);
			return;
		}
	}

	pDoc->SetModifiedFlag(TRUE);
	
	static char BASED_CODE szNetFilter[] = "网络表文件(*.NET) |*.NET|所有文件 (*.*) |*.*||";
	CFileDialog netfiledialog(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,szNetFilter,NULL);
	netfiledialog.m_ofn.lpstrTitle = opennetfile_str;		
	
	if(netfiledialog.DoModal() != IDOK) return;
	
	CString filename = netfiledialog.GetPathName();
	strcpy(netfilename,filename);
	if((netfile = fopen(netfilename,"r"))==NULL) 
	{
		Error(error1);
		Write_Help(commandend);
		return;
	}

	if(Ratsnestflag != 0) 
	{
		Show_Ratsnest(&dc);
		Ratsnestflag = 0;
	}
	
	tempNet_Compbuf = pDoc->m_pNet_Compbuf;
	tempNet_Netnamebuf = pDoc->m_pNet_Netnamebuf;
	tempNet_Pins = pDoc->m_pNet_Pins;
	pDoc->m_nNet_Number = 0;
	pDoc->m_nNet_Compnum = 0;
	for(;;)
    {
		fgets(s,n,netfile);
		i=strlen(s);
		s[i-1]='\0';
		if(feof(netfile)!=0) break;
		if(strcmp(s,"[")==0)
		{
			fgets(s,n,netfile);
			i=strlen(s);
			s[i-1]='\0';
			memcpy(tempNet_Compbuf,s,strlen(s)+1);
			tempNet_Compbuf+=strlen(s)+1;
			fgets(s,n,netfile);
			i=strlen(s);
			s[i-1]='\0';
			memcpy(tempNet_Compbuf,s,strlen(s)+1);
			tempNet_Compbuf+=strlen(s)+1;
			fgets(s,n,netfile);
			i=strlen(s);
			s[i-1]='\0';
			memcpy(tempNet_Compbuf,s,strlen(s)+1);
			tempNet_Compbuf+=strlen(s)+1;
			pDoc->m_nNet_Compnum++;
		}
		else if(strcmp(s,"]")==0) { }
		else if(strcmp(s,"(")==0)
		{
			fgets(s,n,netfile);
			i = strlen(s);
			s[i-1] = '\0';
			memcpy(tempNet_Netnamebuf,s,strlen(s)+1);
			tempNet_Netnamebuf += strlen(s) + 1;
			pDoc->m_ppNet_Pinspointer[pDoc->m_nNet_Number] = tempNet_Pins;
			for(;;)
			{
				fgets(s,n,netfile);
				i=strlen(s);
				s[i-1]='\0';
				if(feof(netfile)!=0) break;
				if(strcmp(s,")")==0)
				{
					pDoc->m_nNet_Number++;
					pDoc->m_ppNet_Pinspointer[pDoc->m_nNet_Number] = tempNet_Pins;
					break;
				}
				memcpy(tempNet_Pins,s,strlen(s)+1);
				tempNet_Pins+=strlen(s)+1;
				memcpy(tempNet_Pins,&notuse,2);
				tempNet_Pins+=2;
			}
		}
	}
	fclose(netfile);
	pDoc->Get_Package_Table();
	Write_Help(commandend);
}

void CDesignView::OnAutoNetClear() 
{
	int i;
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	Write_Help("清除网络表");
	//i = AfxMessageBox("The netlist will be cleared, please confirm.", MB_YESNO|MB_ICONWARNING);
	i = AfxMessageBox("所有网络信息将被清楚，确认?", MB_YESNO|MB_ICONWARNING);
	if(i == IDYES)	
	{
		pDoc->SetModifiedFlag(TRUE);
		pDoc->m_nNet_Number = 0;
	}
	Write_Help(commandend);
}

void CDesignView::OnAutoNetShow() 
{
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	
	if(pDoc->m_nNet_Number == 0) 
	{
		Error( error16 );
	    return;
	}

	CNetShow netshow;
	netshow.net_number = pDoc->m_nNet_Number;
	netshow.net_name_buf = pDoc->m_pNet_Netnamebuf;
	netshow.net_pinspointer = pDoc->m_ppNet_Pinspointer;
	
	//Write_Help("Show the netlist information");
	Write_Help("显示网络信息");
	netshow.DoModal();
	Write_Help(commandend);
}

void CDesignView::OnMakenet()
{
	FILE *netfp;
	int i,m,n,netnum,judge,totalpack;
	int l,ll;
	char *p, *pointer, *temp, *ttemp, *packmem, *ppackmem;
	char *temptrackbuf, *temppadbuf, *temparcfillbuf, *temppackbuf;
	struct htrack
	{ 
		char *pointer;             } highlighttrack;
	struct hpack
	{ 
		short int padnum;
		char* pointer;    } highlightpack;
	char str[100],string[10];
	char *Ncomp,*TempNcomp,*tempncomp;
	struct Package package, ppackage;
	char netname[8], nnn[4]={'N','E','T','\0'};
	char compid1[30],compid2[30],tempbuf1[60],tempbuf2[60];
	struct Line oneline;

	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	static char BASED_CODE szNetFilter[] = "网络表文件(*.NET) |*.NET|所有文件 (*.*) |*.*||";
	CFileDialog makenetdialog(FALSE,"NET",NULL,OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST |OFN_HIDEREADONLY,szNetFilter,NULL);
	makenetdialog.m_ofn.lpstrTitle = "创建网络表文件";

	Write_Help("定义网络表文件名称.");
	if(makenetdialog.DoModal() != IDOK) return;
	CString filename = makenetdialog.GetPathName();
	strcpy(str,filename);
	
	if((netfp = fopen(str,"w"))==NULL) 
	{
		Error(error1);
		Write_Help(commandend);
		return ;
	}

	if(PCBHighlightBufferInit() != 0) return;
	temptrackbuf = Highlight_Trackbuf;
	temppadbuf = Highlight_Padbuf;
	temparcfillbuf = Highlight_ArcFillbuf;
	temppackbuf = Highlight_Packagebuf;

	if((packmem = (char*)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}
	if((ppackmem = (char*)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}
	if((Ncomp = (char*)malloc(24000))==NULL)
 	{
		memory_error();
		return;
	}
	
	for(i=0;i<24000;i++) Ncomp[i]=0;
	TempNcomp = Ncomp;
	tempncomp = Ncomp;

	totalpack=0;
   
	p = pDoc->m_pPackage_Buffer;
	for(i=0; i<pDoc->m_nPackage_Number; i++)
	{
		memcpy(&package,p,16);
		pointer = p;
		if(package.flag==0)
		{
			totalpack++;
			temp=packmem;
			memcpy(temp,pointer,package.total16byte*16);
			temp=packmem;
			memcpy(&package,temp,112);
			memcpy(tempncomp,package.ID,strlen(package.ID)+1);
			tempncomp+=strlen(package.ID)+1;
			memcpy(tempncomp,package.P,strlen(package.P)+1);
			tempncomp+=strlen(package.P)+1;
			memcpy(tempncomp,package.C,strlen(package.C)+1);
			tempncomp+=strlen(package.C)+1;
		}
		p += package.total16byte*16;
	}


	judge=1;
	for(;;)
	{
		if(judge==1) judge=0;
	    else if(judge==0) break;
	    tempncomp=Ncomp;
	    for(i=0;i<totalpack-1;i++)
		{
			memcpy(compid1,tempncomp,strlen(tempncomp)+1);
			l = Strlen_Comp(tempncomp);
			memcpy(compid2,tempncomp+l,strlen(tempncomp+l)+1);
			if(strcmp(compid1,compid2)>0)
			{
				judge=1;
				memcpy(tempbuf1,tempncomp,l);
				ll=Strlen_Comp(tempncomp+l);
				memcpy(tempbuf2,tempncomp+l,ll);
				memcpy(tempncomp,tempbuf2,ll);
				memcpy(tempncomp+ll,tempbuf1,l);
				tempncomp+=ll;
			}
			else   tempncomp+=l;
		}
	}

	tempncomp=Ncomp;
	for(i=0;i<totalpack;i++)
	{
		fputs("[\n",netfp);
		memcpy(package.ID,tempncomp,strlen(tempncomp)+1);
		tempncomp+=strlen(tempncomp)+1;
		memcpy(package.P,tempncomp,strlen(tempncomp)+1);
		tempncomp+=strlen(tempncomp)+1;
		memcpy(package.C,tempncomp,strlen(tempncomp)+1);
		tempncomp+=strlen(tempncomp)+1;
		fputs(package.ID,netfp);
		fputs(C_Retern1,netfp);
		fputs(package.P,netfp);
		fputs(C_Retern1,netfp);
		fputs(package.C,netfp);
		fputs("\n\n\n\n",netfp);
		fputs("]\n",netfp);
	}



	for(i=0;i<24000;i++) Ncomp[i]=0;
	TempNcomp=Ncomp;
	tempncomp=Ncomp;

	netnum=1;
	p = pDoc->m_pPackage_Buffer;
	for(i=0; i<pDoc->m_nPackage_Number; i++)
	{
		pointer = p;
		memcpy(&package,pointer,16);
		if(package.flag==0)
		{  
			temp = packmem;
			memcpy(temp,pointer,package.total16byte*16);
			temp = packmem;
			memcpy(&package,temp,112);
			temp+=112;
			for(l=0;l<package.padnum;l++)
			{
				memcpy(&gnjunction,temp,16);
				temp+=16;
				tempncomp = Ncomp;
				for(;;)
				{
					memcpy(&highlightpack,tempncomp,6);
					if((highlightpack.pointer==p)&&(highlightpack.padnum==l))
					{ 
						m = 1; 
						break; 
					}
					if(highlightpack.pointer == NULL)
					{
						m = 0;
						break; 
					}
					tempncomp+=6;
				}
				if(m==0)
				{
					itoa(netnum,string,10);
					sprintf(netname,"%s%s",nnn,string);
					Write_Help(netname);
					highlightpack.pointer = p;
					highlightpack.padnum = l;
					memcpy(TempPackagebuf,&highlightpack,6);
					TempPackagebuf += 6;
					memcpy(TempNcomp,&highlightpack,6);
					TempNcomp += 6;
					PCBMakeHighlight();
					temppackbuf = Highlight_Packagebuf;
					for(;;)
					{ 
						memcpy(&highlightpack,temppackbuf,6);
						if(highlightpack.pointer == NULL) break;
						else
						{
							memcpy(TempNcomp,&highlightpack,6);
							TempNcomp+=6;
						}
						temppackbuf+=6;
					}
					if((int)(((long)TempPackagebuf-(long)Highlight_Packagebuf)/6)>1)
					{ 
						fputs("(\n",netfp);
						fputs(netname,netfp);
						fputs(C_Retern1,netfp);
						temppackbuf = Highlight_Packagebuf;
						for(;;)
						{
							memcpy(&highlightpack,temppackbuf,6);
							if(highlightpack.pointer == NULL) break;
							else
							{
								pointer = highlightpack.pointer;
								memcpy(&ppackage,pointer,16);
								ttemp = ppackmem;
								memcpy(ttemp,pointer,ppackage.total16byte*16);
								ttemp = ppackmem;
								memcpy(&ppackage,ttemp,112);
								ttemp+=112;
								for(n=0;n<ppackage.padnum;n++)
								{
									memcpy(&gnjunction,ttemp,16);
									ttemp+=16;
									if(n==highlightpack.padnum)
									{
										sprintf(str,"%s%s%s%s",ppackage.ID,"-",gnjunction.padname,C_Retern1);
										fputs(str,netfp);
										break;
									}
								}
							}
							temppackbuf+=6;
						}
						fputs(")\n",netfp);
						netnum++;
					}
					DrawPCBHighlight(0);
					temptrackbuf=Highlight_Trackbuf;
					for(;;)
					{
						memcpy(&highlighttrack,temptrackbuf,4);
						if(highlighttrack.pointer == NULL) break;
						else
						{
							memcpy(&oneline,highlighttrack.pointer,16);
							oneline.flag = 0xa0;
							memcpy(highlighttrack.pointer,&oneline,16);
						}
						temptrackbuf+=4;
					}

					for(n=0;n<HL_lnum;n++) Highlight_Trackbuf[n]=0;
					for(n=0;n<HL_pnum;n++) Highlight_Padbuf[n]=0;
					for(n=0;n<HL_afnum;n++) Highlight_ArcFillbuf[n]=0;
					for(n=0;n<HL_packnum;n++) Highlight_Packagebuf[n]=0;
					TempTrackbuf=Highlight_Trackbuf;
					TempPadbuf=Highlight_Padbuf;
					TempArcFillbuf=Highlight_ArcFillbuf;
					TempPackagebuf=Highlight_Packagebuf;
					temptrackbuf = Highlight_Trackbuf;
					temppadbuf = Highlight_Padbuf;
					temparcfillbuf = Highlight_ArcFillbuf;
					temppackbuf = Highlight_Packagebuf;
				}
			}
		}
		p += package.total16byte*16;
	}

	p = pDoc->m_pLine_Buffer;
	for(i=0; i<pDoc->m_nLine_Number; i++)
	{	
		memcpy(&gnline,p,16);
		if((gnline.attrib==PCB)&&(gnline.flag==0xa0)\
			&&(gnline.layer<=BOTTOMLAYER))
		{
			gnline.flag = 0;
			memcpy(p,&gnline,16);
		}
		p+=16;
	}


   free(packmem);
   free(ppackmem);
   free(Ncomp);
   fclose(netfp);	
   Write_Help(commandend);
}

void CDesignView::OnAutoSetup() 
{
	int i;
	CRouteSetup  routesetup(_T("布线设置"),this,0);
	// 布线模式
	routesetup.m_Setupmode.m_bPreRoute = Route_Switch[0];
	routesetup.m_Setupmode.m_bSMDRoute = Route_Switch[1];
	routesetup.m_Setupmode.m_bHRoute   = Route_Switch[2];
	routesetup.m_Setupmode.m_bVRoute   = Route_Switch[3];
	routesetup.m_Setupmode.m_bLRoute   = Route_Switch[4];
	routesetup.m_Setupmode.m_bZRoute   = Route_Switch[5];
	routesetup.m_Setupmode.m_bCRoute   = Route_Switch[6];
	routesetup.m_Setupmode.m_bM1Route  = Route_Switch[7];
	routesetup.m_Setupmode.m_bM2Route  = Route_Switch[8];
	routesetup.m_Setupmode.m_bXSearch  = Route_Switch[9];
	routesetup.m_Setupmode.m_nTop = Route_Layer[0];
	routesetup.m_Setupmode.m_nMid1 = Route_Layer[1];
	routesetup.m_Setupmode.m_nMid2 = Route_Layer[2];
	routesetup.m_Setupmode.m_nMid3 = Route_Layer[3];
	routesetup.m_Setupmode.m_nMid4 = Route_Layer[4];
	routesetup.m_Setupmode.m_nBottom = Route_Layer[5];
	// 布线参数
	routesetup.m_Setupparam.m_nTrackwidth = Route_TW;
	routesetup.m_Setupparam.m_nViasize = Route_Viasize;
	// 飞线模式
	routesetup.m_SetupRats.m_nRats_Netmode = Dynamic_Ratsnest_Mode;
	routesetup.m_SetupRats.m_nNet_Num = Part_Net_Number;
	routesetup.m_SetupRats.m_nRats_Mode = Route_Mode;
	routesetup.m_SetupRats.m_bRats_Auto = CL_Rats_Auto;
	// DRC参数
	routesetup.m_DRCSetup.m_nT_T = DRC_T_T;
	routesetup.m_DRCSetup.m_nT_P = DRC_T_P;
	routesetup.m_DRCSetup.m_nP_P = DRC_P_P;
	//布局参数
	routesetup.m_AutoplaceSetup.m_nDx = CL_Autoplace_Dx;
	routesetup.m_AutoplaceSetup.m_nDy = CL_Autoplace_Dy;
	routesetup.m_AutoplaceSetup.m_nOPDx = CL_Autoplace_OPDx;
	routesetup.m_AutoplaceSetup.m_nOPDy = CL_Autoplace_OPDy;


	Write_Help("设置布线策略，布线参数，校验参数和飞线模式...");
	i = routesetup.DoModal();
	if(i == IDOK)
	{
		Route_Switch[0] = routesetup.m_Setupmode.m_bPreRoute;
		Route_Switch[1] = routesetup.m_Setupmode.m_bSMDRoute;
		Route_Switch[2] = routesetup.m_Setupmode.m_bHRoute ;
		Route_Switch[3] = routesetup.m_Setupmode.m_bVRoute;
		Route_Switch[4] = routesetup.m_Setupmode.m_bLRoute;
		Route_Switch[5] = routesetup.m_Setupmode.m_bZRoute;
		Route_Switch[6] = routesetup.m_Setupmode.m_bCRoute;
		Route_Switch[7] = routesetup.m_Setupmode.m_bM1Route;
		Route_Switch[8] = routesetup.m_Setupmode.m_bM2Route;
		Route_Switch[9] = routesetup.m_Setupmode.m_bXSearch;
		Route_Layer[0] = routesetup.m_Setupmode.m_nTop;
		Route_Layer[1] = routesetup.m_Setupmode.m_nMid1;
		Route_Layer[2] = routesetup.m_Setupmode.m_nMid2;
		Route_Layer[3] = routesetup.m_Setupmode.m_nMid3;
		Route_Layer[4] = routesetup.m_Setupmode.m_nMid4;
		Route_Layer[5] = routesetup.m_Setupmode.m_nBottom;

		Route_TW = routesetup.m_Setupparam.m_nTrackwidth;
		Route_Viasize = routesetup.m_Setupparam.m_nViasize;
		
		Dynamic_Ratsnest_Mode = routesetup.m_SetupRats.m_nRats_Netmode;
		Part_Net_Number = routesetup.m_SetupRats.m_nNet_Num;
		Route_Mode = routesetup.m_SetupRats.m_nRats_Mode;
		CL_Rats_Auto = routesetup.m_SetupRats.m_bRats_Auto;
	
		DRC_T_T = routesetup.m_DRCSetup.m_nT_T;
		DRC_T_P = routesetup.m_DRCSetup.m_nT_P;
		DRC_P_P = routesetup.m_DRCSetup.m_nP_P;

		CL_Autoplace_Dx = routesetup.m_AutoplaceSetup.m_nDx;
		CL_Autoplace_Dy = routesetup.m_AutoplaceSetup.m_nDy;
		CL_Autoplace_OPDx = routesetup.m_AutoplaceSetup.m_nOPDx;
		CL_Autoplace_OPDy = routesetup.m_AutoplaceSetup.m_nOPDy;
	}
	Write_Help(commandend);
}

int CDesignView::Clearance_Route(struct Line * oneline)
{
	int i,l;
	char *p,*temp;
	struct Line *abcline;
	struct Junction *onejunction, *abcjunc;
	struct Arc *onearc;
	struct Fill *onefill;
	struct Package *abcpackage;
	
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	
	// all track -> routed track 
 	p = pDoc->m_pLine_Buffer;
	for(i=0; i<pDoc->m_nLine_Number; i++)
	{	
		abcline = (struct Line*)p;
		if((abcline->attrib==PCB)&&(abcline->flag==0)\
			&&((abcline->layer<=BOTTOMLAYER)||(abcline->layer==KEEPOUTLAYER)))
		{
			if(abcline->flag == 1) break;
			if((oneline->id==0x10)&&\
				((oneline->layer==abcline->layer)||(abcline->layer==KEEPOUTLAYER)))
			{
				if(Line_Line_Connect(abcline,oneline,DRC_T_T)!=0)
					if(Highlighttrack_Exist(p)==0) return 1;
			}
			else if((oneline->id==0x22)||(oneline->id==0x21))
			{
				abcjunc = (struct Junction*)oneline;
				if(Line_Pad_Connect(abcline,abcjunc,DRC_T_P)!=0)
					if(Highlighttrack_Exist(p)==0) return 1;
			}
		}
		p+=16;
	}

	// all pad/via - routed track 
  	p = pDoc->m_pJunction_Buffer;
	for(i=0; i<pDoc->m_nJunction_Number; i++)
	{	
		abcjunc = (struct Junction*)p;
		if((abcjunc->attrib==PCB)&&(abcjunc->flag==0))
		{
			if(oneline->id==0x10)
			{
				if(Line_Pad_Connect(oneline,abcjunc,DRC_T_P)!=0)
					if(Highlightpad_Exist(p)==0) return 2;
			}
			else if((oneline->id==0x22)||(oneline->id==0x21))
			{
				onejunction = (struct Junction*)oneline;
				if(Pad_Pad_Connect(abcjunc,onejunction,DRC_P_P)!=0)
				{  
					if(Highlightpad_Exist(p)==0) return 1;
				}
			}
		}
		p+=16;
	}

	// all package's pad - routed track 
	p = pDoc->m_pPackage_Buffer;
	for(i=0; i<pDoc->m_nPackage_Number; i++)
	{
		abcpackage = (struct Package*)p;
		if(abcpackage->flag==0)
		{
			temp = p;
			temp+=112;
			for(l=0;l<abcpackage->padnum;l++)
			{
				abcjunc = (struct Junction*)temp;
				temp += 16;
				if(oneline->id==0x10)
				{
					if(Line_Pad_Connect(oneline,abcjunc,DRC_T_P)!=0)
						if(Highlightpackage_Exist(p,l)==0)  return 3;
				}
				else if((oneline->id==0x22)||(oneline->id==0x21))
				{
					onejunction = (struct Junction*)oneline;
					if(Pad_Pad_Connect(abcjunc,onejunction,DRC_P_P)!=0)
					{  
						if(Highlightpackage_Exist(p,l)==0) return 1;	
					}
				}
			}
		}
		p += abcpackage->total16byte*16;
	}

	// all arc-> routed track 
	p = pDoc->m_pArc_Buffer;
	for(i=0; i<pDoc->m_nArc_Number; i++)
	{	
	    onearc = (struct Arc*)p;
	    if((onearc->attrib==PCB)&&(onearc->flag==0)&&\
	       ((onearc->layer<=BOTTOMLAYER)||(onearc->layer==KEEPOUTLAYER)))
		{
			if((oneline->id==0x10)&&\
				((oneline->layer==onearc->layer)||(onearc->layer==KEEPOUTLAYER)))
			{
				if(Line_Arc_Connect(oneline,onearc,DRC_T_T)!=0)
					if(Highlightarcfill_Exist(p)==0) return 1;
			}
			else if((oneline->id==0x22)||(oneline->id==0x21))
			{
				abcjunc = (struct Junction*)oneline;
				if(Pad_Arc_Connect(abcjunc,onearc,DRC_T_P)!=0)
					if(Highlightarcfill_Exist(p)==0) return 1;
			}
		}
		p+=16;
	}

	// all fill -> routed track 
	p = pDoc->m_pFill_Buffer;
	for(i=0; i<pDoc->m_nFill_Number; i++)
	{	
		onefill = (struct Fill*)p;
	    if((onefill->attrib==PCB)&&(onefill->flag==0)&&\
	       ((onefill->layer<=BOTTOMLAYER)||(onefill->layer==KEEPOUTLAYER)))
		{
			if((oneline->id==0x10)&&\
				((oneline->layer==onefill->layer)||(onefill->layer==KEEPOUTLAYER)))
			{
				if(Line_Fill_Connect(oneline,onefill,DRC_T_T)!=0)
					if(Highlightarcfill_Exist(p)==0) return 1;
			}
			else if((oneline->id==0x22)||(oneline->id==0x21))
			{
				abcjunc = (struct Junction*)oneline;
				if(Pad_Fill_Connect(abcjunc,onefill,DRC_T_P)!=0)
					if(Highlightarcfill_Exist(p)==0) return 1;
			}
		}
		p += onefill->total16byte*16;
	}  
   return 0;
}

void CDesignView::OnOptimizeViaminimize() 
{
	int i,ii,totalnum,flag;
	int invalidflag,l,ll,invalidflag1,invalidflag2;
	char *p,*pp, *pointer, str[10], string[30];
	struct Junction *abcjunc, onejunc;
	struct Line *abcline;
	struct Package *abcpackage;
	char *trackbuf1, *trackbuf2, *temptrackbuf1, *temptrackbuf2, *temp;
	unsigned buf1num, buf2num, buf1layer, buf2layer;
	struct swaptrack
	{
		char* pointer;
		struct Line line; } swaptrack, swap;
	struct htrack
	{
		char* pointer;             } highlighttrack;
	char *hstr1 =  "过孔优化...    ";
	struct Undo_Head head;
	char *undo_p;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	if((trackbuf1 = (char*)malloc(10000))==NULL)
	{
		memory_error();
		return;
	}
	if((trackbuf2 = (char*)malloc(10000))==NULL)
	{
		memory_error();
		return;
	}
	temptrackbuf1 = trackbuf1;
	temptrackbuf2 = trackbuf2;
	buf1num = buf2num = 0;
	buf1layer = buf2layer = 100;
	totalnum = 0;
	Write_Help(hstr1);

	invalidflag = 0;
	flag = 0;
	p = pDoc->m_pJunction_Buffer;
	for(i=0; i<pDoc->m_nJunction_Number; i++)
	{	
		abcjunc = (struct Junction*)p;
		pointer = p;
		memcpy(&onejunc,pointer,16);
		if((abcjunc->attrib==PCB)&&(abcjunc->layer==MULTILAYER)&&\
			(abcjunc->flag==0)&&(abcjunc->id==0x22)&&\
			(((abcjunc->padtype&0x38)>>3)==0))
		{
			// search for related tracks 
			temptrackbuf1 = trackbuf1;
			temptrackbuf2 = trackbuf2;
			buf1num = buf2num = 0;
			buf1layer = buf2layer = 100;
			invalidflag = 0;
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				abcline = (struct Line*)pp;
				if((abcline->attrib==PCB)&&(abcline->layer<=BOTTOMLAYER)&&\
					(abcline->flag==0)&&(Line_Pad_Connect(abcline,&onejunc,0)!=0))
				{
					if(abcline->layer==buf1layer)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf1,&swaptrack,20);
						temptrackbuf1+=20;
						buf1num++;
					}
					else if(abcline->layer==buf2layer)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf2,&swaptrack,20);
						temptrackbuf2+=20;
						buf2num++;
					}
					else if(buf1layer==100)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf1,&swaptrack,20);
						temptrackbuf1+=20;
						buf1layer=abcline->layer;
						buf1num++;
					}
					else if(buf2layer==100)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf2,&swaptrack,20);
						temptrackbuf2+=20;
						buf2layer=abcline->layer;
						buf2num++;
					}
					else invalidflag=1;
				}
				pp += 16;
			}
			if((buf1layer==100)||(buf2layer==100)) invalidflag = 1;
			if(invalidflag!=0) goto NEXT_VIA;
			// process track's in buf1 
			invalidflag1 = 0;
			temptrackbuf1 = trackbuf1;
			for(l=0;l<buf1num;l++)
			{
				memcpy(&swaptrack,temptrackbuf1,20);
				memcpy(&gnline,&swaptrack.line,16);
				if(invalidflag1==0)
				{
					pp = pDoc->m_pLine_Buffer;
					for(ii=0; ii<pDoc->m_nLine_Number; ii++)
					{
						abcline = (struct Line*)pp;
						if((abcline->attrib==PCB)&&(abcline->layer==gnline.layer)&&\
							(abcline->flag==0)&&(Line_Line_Connect(abcline,&gnline,0)!=0))
						{
							temp = trackbuf1;
							invalidflag = 1;
							for(ll=0;ll<buf1num;ll++)
							{
								memcpy(&swap,temp,20);
								if(swap.pointer==pp)
								{
									invalidflag = 0;
									break;
								}
								temp += 20;
							}
							if(invalidflag != 0)
							{
								swaptrack.pointer = pp;
								memcpy(&swaptrack.line,abcline,16);
								memcpy(trackbuf1+buf1num*20,&swaptrack,20);
								buf1num++;
							}
						}
						pp+=16;
					}
				}
				// to see if connect free SMD pad or not 
				if(invalidflag1==0)
				{
					pp = pDoc->m_pJunction_Buffer;
					for(ii=0; ii<pDoc->m_nJunction_Number; ii++)
					{	
						abcjunc = (struct Junction*)pp;
						if((abcjunc->attrib==PCB)&&(abcjunc->layer==gnline.layer)&&\
							(abcjunc->flag==0)&&(Line_Pad_Connect(&gnline,abcjunc,0)!=0))
						{
							invalidflag1 = 1;
							break;
						}
						pp+=16;
					}
				}

			   // to see if connect SMD Package's PAD or not 
				if(invalidflag1==0)
				{
					pp = pDoc->m_pPackage_Buffer;
					for(ii=0; ii<pDoc->m_nPackage_Number; ii++)
					{
						abcpackage = (struct Package*)pp;
						if(abcpackage->flag==0)
						{
							temp = pp;
							temp += 112;
							for(ll=0;ll<abcpackage->padnum;ll++)
							{
								abcjunc = (struct Junction*)temp;
								if((abcjunc->layer==gnline.layer)&&\
									(Line_Pad_Connect(&gnline,abcjunc,0)!=0))
								{
									invalidflag1=1;
									break;
								}
								temp+=16;
							}
						}
						pp += abcpackage->total16byte*16;

					}
				}
				else break;
				temptrackbuf1 += 20;
			}
			
			if(invalidflag1!=0) goto NEXT_VIA;
			//  swap tracks layer to check clearance 
			if(PCBHighlightBufferInit() != 0) return;
			memcpy(&swap,trackbuf1,20);
			memcpy(&gnline,&swap.line,16);
			highlighttrack.pointer = swap.pointer;
			memcpy(TempTrackbuf,&highlighttrack,4);
			TempTrackbuf += 4;
			PCBMakeHighlight();
			temptrackbuf1 = trackbuf1;
			for(l=0;l<buf1num;l++)
			{
				memcpy(&swaptrack,temptrackbuf1,20);
				temptrackbuf1+=20;
				swaptrack.line.layer=buf2layer;
				if(Clearance_Route(&swaptrack.line)!=0)
				{
					invalidflag1=1;
					break;
				}
			}
			
			DrawPCBHighlight(0);
			// process via and related tracks 
			if(invalidflag1==0)
			{
				abcjunc = (struct Junction*)p;
				CloseMouseCursor();
				DrawStructJP(abcjunc,BK_Color,0,&dc);
				OpenMouseCursor();
				abcjunc->flag = 0xff;
				//----------- Undo -----------
				flag = 1;
				Check_Undo_Buffer(10);
				undo_p = pDoc->m_pPCB_Undo_Current;
				head.op = UNDO_UNDELETE;
				head.ob = UNDO_JUNCTION;
				head.p = p;
				memcpy(undo_p,&head,6);
				pDoc->m_pPCB_Undo_Current += 6;
				//---------------------------------------------------------------------------
				temptrackbuf1 = trackbuf1;
				totalnum++;
				itoa(totalnum,str,10);
				sprintf(string,"%s%s",hstr1,str);
				Write_Help(string);
				for(l=0;l<buf1num;l++)
				{
					memcpy(&swap,temptrackbuf1,20);
					temptrackbuf1+=20;
					abcline = (struct Line*)swap.pointer;
					//----------- Undo -----------
					flag = 1;
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_CHANGE;
					head.ob = UNDO_LINE;
					head.p = swap.pointer;
					head.dx = abcline->linewidth;
					head.dy = abcline->linewidth;
					head.state1 = abcline->layer;
					head.state2 = buf2layer;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					//---------------------------------------------------------------------------
					abcline->layer = buf2layer;
					CloseMouseCursor();
					DrawStructLine(abcline,PCB_LayerColor[abcline->layer],&dc);
					OpenMouseCursor();
				}
				goto NEXT_VIA;
			}
			
			// process track's in buf2 
			invalidflag2=0;
			temptrackbuf2=trackbuf2;
			for(l=0;l<buf2num;l++)
			{
				memcpy(&swaptrack,temptrackbuf2,20);
				memcpy(&gnline,&swaptrack.line,16);
				if(invalidflag2==0)
				{
					pp = pDoc->m_pLine_Buffer;
					for(ii=0; ii<pDoc->m_nLine_Number; ii++)
					{	
						abcline = (struct Line*)pp;
						if((abcline->attrib==PCB)&&(abcline->layer==gnline.layer)&&\
							(abcline->flag==0)&&(Line_Line_Connect(abcline,&gnline,0)!=0))
						{
							temp = trackbuf2;
							invalidflag = 1;
							for(ll=0;ll<buf2num;ll++)
							{
								memcpy(&swap,temp,20);
								if(swap.pointer == pp)
								{
									invalidflag=0;
									break;
								}
								temp+=20;
							}
							if(invalidflag!=0)
							{
								swaptrack.pointer = pp;
								memcpy(&swaptrack.line,abcline,16);
								memcpy(trackbuf2+buf2num*20,&swaptrack,20);
								buf2num++;
							}
						}
						pp+=16;
					}	
				}

				// to see if connect free SMD pad or not 
				if(invalidflag2==0)
				{
					pp = pDoc->m_pJunction_Buffer;
					for(ii=0; ii<pDoc->m_nJunction_Number; ii++)
					{	
						abcjunc = (struct Junction*)pp;
						if((abcjunc->attrib==PCB)&&(abcjunc->layer==gnline.layer)&&\
							(abcjunc->flag==0)&&(Line_Pad_Connect(&gnline,abcjunc,0)!=0))
						{
							invalidflag2=1;
							break;
						}
						pp+=16;
					}
				}
				
				// to see if connect SMD Package's PAD or not 
				if(invalidflag2==0)
				{
					pp = pDoc->m_pPackage_Buffer;
					for(ii=0; ii<pDoc->m_nPackage_Number; ii++)
					{
						abcpackage = (struct Package*)pp;
						if(abcpackage->flag==0)
						{
							temp = pp;
							temp+=112;
							for(ll=0;ll<abcpackage->padnum;ll++)
							{
								abcjunc = (struct Junction*)temp;
								if((abcjunc->layer==gnline.layer)&&\
									(Line_Pad_Connect(&gnline,abcjunc,0)!=0))
								{
									invalidflag2=1;
									break;
								}
								temp+=16;
							}
						}
						pp += abcpackage->total16byte*16;
					}	
				}
				else break;
				temptrackbuf2 += 20;
			}
			
			if(invalidflag2!=0) goto NEXT_VIA;
			//  swap tracks layer to check clearance 
			if(PCBHighlightBufferInit()!=0) return;
			memcpy(&swap,trackbuf2,20);
			memcpy(&gnline,&swap.line,16);
			highlighttrack.pointer = swap.pointer;
			memcpy(TempTrackbuf,&highlighttrack,4);
			TempTrackbuf+=4;
			PCBMakeHighlight();
			temptrackbuf2=trackbuf2;
			for(l=0;l<buf2num;l++)
			{
				memcpy(&swaptrack,temptrackbuf2,20);
				temptrackbuf2+=20;
				swaptrack.line.layer=buf1layer;
				if(Clearance_Route(&swaptrack.line)!=0)
				{
					invalidflag2=1;
					break;
				}
			}
			
			DrawPCBHighlight(0);
			// process via and related tracks 
			if(invalidflag2==0)
			{
				abcjunc = (struct Junction*)p;
				CloseMouseCursor();
				DrawStructJP(abcjunc,BK_Color,0,&dc);
				OpenMouseCursor();
				abcjunc->flag=0xff;
				//----------- Undo -----------
				flag = 1;
				Check_Undo_Buffer(10);
				undo_p = pDoc->m_pPCB_Undo_Current;
				head.op = UNDO_UNDELETE;
				head.ob = UNDO_JUNCTION;
				head.p = p;
				memcpy(undo_p,&head,6);
				pDoc->m_pPCB_Undo_Current += 6;
				//---------------------------------------------------------------------------
				temptrackbuf2=trackbuf2;
				totalnum++;
				itoa(totalnum,str,10);
				sprintf(string,"%s%s",hstr1,str);
				Write_Help(string);
				for(l=0;l<buf2num;l++)
				{
					memcpy(&swap,temptrackbuf2,20);
					temptrackbuf2+=20;
					abcline = (struct Line*)swap.pointer;
					//----------- Undo -----------
					flag = 1;
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_CHANGE;
					head.ob = UNDO_LINE;
					head.p = swap.pointer;
					head.dx = abcline->linewidth;
					head.dy = abcline->linewidth;
					head.state1 = abcline->layer;
					head.state2 = buf1layer;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					//---------------------------------------------------------------------------
					abcline->layer=buf1layer;
					CloseMouseCursor();
					DrawStructLine(abcline,PCB_LayerColor[abcline->layer],&dc);
					OpenMouseCursor();
				}
				goto NEXT_VIA;
			}
NEXT_VIA:
			ll++;
		}
		p+=16;
	}

	p = pDoc->m_pJunction_Buffer;
	for(i=0; i<pDoc->m_nJunction_Number; i++)
	{	
		abcjunc = (struct Junction*)p;
		pointer = p;
		memcpy(&onejunc,pointer,16);
		if((abcjunc->attrib==PCB)&&(abcjunc->layer==MULTILAYER)&&\
			(abcjunc->flag==0)&&(abcjunc->id==0x22)&&\
			(((abcjunc->padtype&0x38)>>3)==0))
		{
			// search for related tracks 
			temptrackbuf1=trackbuf1;
			temptrackbuf2=trackbuf2;
			buf1num=buf2num=0;
			buf1layer=buf2layer=100;
			invalidflag=0;
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				abcline = (struct Line*)pp;
				if((abcline->attrib==PCB)&&(abcline->layer<=BOTTOMLAYER)&&\
					(abcline->flag==0)&&(Line_Pad_Connect(abcline,&onejunc,0)!=0))
				{
					if(abcline->layer==buf1layer)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf1,&swaptrack,20);
						temptrackbuf1+=20;
						buf1num++;
					}
					else if(abcline->layer==buf2layer)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf2,&swaptrack,20);
						temptrackbuf2+=20;
						buf2num++;
					}
					else if(buf1layer==100)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf1,&swaptrack,20);
						temptrackbuf1+=20;
						buf1layer=abcline->layer;
						buf1num++;
					}
					else if(buf2layer==100)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf2,&swaptrack,20);
						temptrackbuf2+=20;
						buf2layer=abcline->layer;
						buf2num++;
					}
					else invalidflag=1;
				}
				pp+=16;
			}
			
			if((buf1layer==100)||(buf2layer==100))
			{
				abcjunc = (struct Junction*)p;
				CloseMouseCursor();
				DrawStructJP(abcjunc,BK_Color,0,&dc);
				OpenMouseCursor();
				abcjunc->flag = 0xff;
				//----------- Undo -----------
				flag = 1;
				Check_Undo_Buffer(10);
				undo_p = pDoc->m_pPCB_Undo_Current;
				head.op = UNDO_UNDELETE;
				head.ob = UNDO_JUNCTION;
				head.p = p;
				memcpy(undo_p,&head,6);
				pDoc->m_pPCB_Undo_Current += 6;
				//---------------------------------------------------------------------------
				totalnum++;
				itoa(totalnum,str,10);
				sprintf(string,"%s%s",hstr1,str);
				Write_Help(string);
			}
		}
		p+=16;
	}
	//----------- Undo -----------
	if(flag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------
	free(trackbuf1);
	free(trackbuf2);
	Free_PCBHighlightbuf();
	if(Via_Showmode==0)
	{
		itoa(totalnum,str,10);
		sprintf(string,"%s%s",str,"过孔已经移除");
		AfxMessageBox(string, MB_OK|MB_ICONINFORMATION);
	}
	Write_Help(commandend);
}

void CDesignView::OnOptimizeSqueeze()
{
	int i,j,k,ii,num,l,ll,flag;
	unsigned ch1;
	unsigned x1,y1;
	int deltax,deltay;
	long int li;
	char *pp, *temp, *pointer, *ttemp;
	char *movebuf, *newbuf, *tempnewbuf, *tempbuf;
	struct sq
    {
		char* pointer;
		short int type; } sq, *sq1, sq2;
	struct Junction *abcjunc, onejunc;
	struct Line *abcline, oneline;
	struct Package *abcpackage, *onepackage;
	struct String *abcstring;
	struct Arc *abcarc;
	struct Fill *abcfill;
	char *hstr1="推挤...";
	char *hstr2="选择走线或者元件";
	int direcpoly[16] = { -12,2, -24,2, 0,-14, 24,2, 12,2, 12,14, -12,14, -12,2 };
	POINT point[8];
	
	int undoflag;
	struct Undo_Head head;
	char *undo_p;
	
	CClientDC dc(this);
	int drawmode = dc.GetROP2();
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	if((newbuf = (char*)malloc(600000))==NULL)
	{
		memory_error();
		return;
	}


	flag = 0;
SBEGIN:
	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(hstr2);
	//setwritemode(1);
	//setcolor(Overwrite_Color);
	Get_VPort_XY(&j,&k,m_nCurrent_X,m_nCurrent_Y);
	if(flag==0)
    {
		for(i=0;i<8;i++)
		{
			point[i].x = direcpoly[2*i]+j;
			point[i].y = direcpoly[2*i+1]+k;
		}
    }
	else if(flag==1)
    {
		for(i=0;i<8;i++)
		{
			point[i].x = direcpoly[2*i+1] + j;
			point[i].y = direcpoly[2*i] + k;
		}
    }
	else if(flag==2)
    {
		for(i=0;i<8;i++)
		{
			point[i].x = j - direcpoly[2*i];
			point[i].y = k - direcpoly[2*i+1];
		}
    }
	else if(flag==3)
    {
		for(i=0;i<8;i++)
		{
			point[i].x = j - direcpoly[2*i+1];
			point[i].y = k - direcpoly[2*i];
		}
    }
	
	CloseMouseCursor();
	//setwritemode(1);
	//setcolor(Overwrite_Color);
	//drawpoly(8,ppoly);
	dc.SetROP2(R2_NOTXORPEN);
	dc.Polyline( point, 8);
	dc.SetROP2(drawmode);
	OpenMouseCursor();
	CL_RedrawFlag = 0;
	do
    {
		ch1 = MouseKey();
		x1 = m_nCurrent_X;
		y1 = m_nCurrent_Y;

		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			CloseMouseCursor();
			dc.SetROP2(R2_NOTXORPEN);
			dc.Polyline( point, 8);
			dc.SetROP2(drawmode);
			OpenMouseCursor();
			break;
		}
		else if(ch1==MOUSEMOVE)
		{
TEMP:
			if(CL_RedrawFlag==1) { CL_RedrawFlag=0; }
			else 
			{
				CloseMouseCursor();
				dc.SetROP2(R2_NOTXORPEN);
				dc.Polyline( point, 8);
				dc.SetROP2(drawmode);
				OpenMouseCursor();
			}
		
			Get_VPort_XY(&j,&k,m_nCurrent_X,m_nCurrent_Y);
			if(flag==0)
			{
				for(i=0;i<8;i++)
				{
					point[i].x = direcpoly[2*i]+j;
					point[i].y = direcpoly[2*i+1]+k;
				}
			}
			else if(flag==1)
			{
				for(i=0;i<8;i++)
				{
					point[i].x = direcpoly[2*i+1] + j;
					point[i].y = direcpoly[2*i] + k;
				}
			}
			else if(flag==2)
			{
				for(i=0;i<8;i++)
				{
					point[i].x = j - direcpoly[2*i];
					point[i].y = k - direcpoly[2*i+1];
				}
			}
			else if(flag==3)
			{
				for(i=0;i<8;i++)
				{
					point[i].x = j - direcpoly[2*i+1];
					point[i].y = k - direcpoly[2*i];
				}
			}
			CloseMouseCursor();
			dc.SetROP2(R2_NOTXORPEN);
			dc.Polyline( point, 8);
			dc.SetROP2(drawmode);
			OpenMouseCursor();
		}
		else if((ch1==VK_RETURN)||(ch1==PUSHMOUSE))
		{
			if(flag==0)
			{
				deltax=0;
				deltay=CL_Snap_Grid;
				if(deltay>25) deltay=25;
			}
			else if(flag==1)
			{
				deltay=0;
				deltax=-CL_Snap_Grid;
				if(deltax<-25) deltax=-25;
			}
			else if(flag==2)
			{
				deltax=0;
				deltay=-CL_Snap_Grid;
				if(deltay<-25) deltay=-25;
			}
			else if(flag==3)
			{
				deltay=0;
				deltax=CL_Snap_Grid;
				if(deltax>25) deltax=25;
			}

			if(Select_Line(x1,y1,&pointer)!=0)
			{
				sq.type=EMMLINE;
				CloseMouseCursor();
				dc.SetROP2(R2_NOTXORPEN);
				dc.Polyline( point, 8);
				dc.SetROP2(drawmode);
				OpenMouseCursor();
				break;
			}
			else if(Select_PickPackage(x1,y1,&pointer)!=0)
			{
				sq.type=EMMPACKAGE;
				CloseMouseCursor();
				dc.SetROP2(R2_NOTXORPEN);
				dc.Polyline( point, 8);
				dc.SetROP2(drawmode);
				OpenMouseCursor();
				break;
			}
		}
		else if(ch1==SPACE)
		{
			flag++;
			if(flag>=4) flag=0;
			goto TEMP;
		}
		else  Command_In_Function(ch1);
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));

	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
	{
		free(newbuf);
		CL_Auto_Pan = OFF;
		DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
		Write_Help(commandend);
		return;
	}

	undoflag = 0;
	for(li=0;li<600000;li++) newbuf[li] = 0;

	tempnewbuf=newbuf;
	sq.pointer = pointer;
	memcpy(tempnewbuf,&sq,6);
	tempnewbuf+=6;

	CloseMouseCursor();
	Write_Help(hstr1);
	OpenMouseCursor();
	movebuf=newbuf;
	for(;;)
	{
		memcpy(&sq,movebuf,6);
		movebuf+=6;
		if(sq.pointer == NULL) break;
		if((sq.type&0x000f)==EMMLINE)
		{
			temp = sq.pointer;
			memcpy(&oneline,temp,16);
			if(((deltay==0)&&(oneline.starty==oneline.endy))||\
				((deltax==0)&&(oneline.startx==oneline.endx)))  { continue; }
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				abcline = (struct Line*)pp;
				if((abcline->attrib==PCB)&&(abcline->layer==oneline.layer)&&\
					(abcline->flag==0)&&(Line_Line_Connect(abcline,&oneline,0)!=0))
				{
					tempbuf = newbuf;
					for(;;)
					{
						sq1 = (struct sq*)tempbuf;
						if(sq1->pointer == pp)
						{
							if((deltay==0)&&(abcline->starty==abcline->endy))
							{
								if(Is_On_Line(abcline->startx,abcline->starty,&oneline)!=0)
								{
									sq1->type=sq1->type|0x0010;
								}
								else if(Is_On_Line(abcline->endx,abcline->endy,&oneline)!=0)
								{
									sq1->type=sq1->type|0x0020;
								}
							}
							else if((deltax==0)&&(abcline->startx==abcline->endx))
							{
								if(Is_On_Line(abcline->startx,abcline->starty,&oneline)!=0)
								{
									sq1->type=sq1->type|0x0010;
								}
								else if(Is_On_Line(abcline->endx,abcline->endy,&oneline)!=0)
							    {
									sq1->type=sq1->type|0x0020;
							    }
							}
							break;
						}
						else if(sq1->pointer == NULL)
						{
							break;
						}
						tempbuf+=6;
					}
					if(sq1->pointer == NULL)
					{
						sq2.pointer = pp;
						sq2.type = EMMLINE;
						if((deltay==0)&&(abcline->starty==abcline->endy))
						{
							if(Is_On_Line(abcline->startx,abcline->starty,&oneline)!=0)
							{
								sq2.type=sq2.type|0x0010;
							}
							else if(Is_On_Line(abcline->endx,abcline->endy,&oneline)!=0)
							{
								sq2.type=sq2.type|0x0020;
							}
						}
						else if((deltax==0)&&(abcline->startx==abcline->endx))
						{
							if(Is_On_Line(abcline->startx,abcline->starty,&oneline)!=0)
							{
								sq2.type=sq2.type|0x0010;
							}
							else if(Is_On_Line(abcline->endx,abcline->endy,&oneline)!=0)
							{
								sq2.type=sq2.type|0x0020;
							}
						}
						memcpy(tempnewbuf,&sq2,6);
						tempnewbuf+=6;
					}
				}
				pp+=16;
			}

			// to see if connect free SMD pad or not 
			pp = pDoc->m_pJunction_Buffer;
			for(ii=0; ii<pDoc->m_nJunction_Number; ii++)
			{	
				abcjunc = (struct Junction*)pp;
				if((abcjunc->attrib==PCB)&&(abcjunc->flag==0)&&\
					(Line_Pad_Connect(&oneline,abcjunc,0)!=0))
				{
					tempbuf = newbuf;
					for(;;)
					{
						sq1 = (struct sq*)tempbuf;
						if(sq1->pointer == pp)
						{
							break;
						}
						else if(sq1->pointer == NULL)
						{
							break;
						}
						tempbuf+=6;
					}
					if(sq1->pointer == NULL)
					{
						sq2.pointer = pp;
						sq2.type = EMMJP;
						memcpy(tempnewbuf,&sq2,6);
						tempnewbuf += 6;
					}
				}
				pp+=16;
			}

			temp = sq.pointer;
			memcpy(&oneline,temp,16);
			CloseMouseCursor();
			DrawStructLine(&oneline,BK_Color,&dc);
			//----------- Undo -----------
			undoflag = 1;
			Check_Undo_Buffer(30);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_MOVE;
			head.ob = UNDO_LINE;
			head.p = sq.pointer;
			head.dx = deltax;
			head.dy = deltay;
			memcpy(undo_p,&head,16);
			pDoc->m_pPCB_Undo_Current += 16;
			//---------------------------------------------------------------------------
			oneline.startx+=deltax;
			oneline.starty+=deltay;
			oneline.endx+=deltax;
			oneline.endy+=deltay;
			memcpy(temp,&oneline,16);
			DrawStructLine(&oneline,PCB_LayerColor[oneline.layer],&dc);
			OpenMouseCursor();
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				abcline = (struct Line*)pp;
				if((abcline->attrib==PCB)&&(abcline->layer==oneline.layer)&&\
					(abcline->flag==0)&&(Line_Line_Connect(abcline,&oneline,DRC_T_T)!=0))
				{
					tempbuf = newbuf;
					for(;;)
					{
						sq1 = (struct sq*)tempbuf;
						if(sq1->pointer== pp)
						{
							break;
						}
						else if(sq1->pointer == NULL)
						{
							break;
						}
						tempbuf+=6;
					}
					if(sq1->pointer == NULL)
					{
						sq2.pointer = pp;
						sq2.type = EMMLINE;
						if((deltay==0)&&(abcline->starty==abcline->endy))
						{ 
							if(deltax>0)
							{
								if(abcline->startx<abcline->endx)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
							else
							{
								if(abcline->startx>abcline->endx)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
						}
						else if((deltax==0)&&(abcline->startx==abcline->endx))
						{
							if(deltay>0)
							{
								if(abcline->starty<abcline->endy)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
							else
							{
								if(abcline->starty>abcline->endy)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
						}
						memcpy(tempnewbuf,&sq2,6);
						tempnewbuf+=6;
					}
				}
				pp+=16;
			}

			// to see if connect free SMD pad or not 
			pp = pDoc->m_pJunction_Buffer;
			for(ii=0; ii<pDoc->m_nJunction_Number; ii++)
			{	
				abcjunc = (struct Junction*)pp;
				if((abcjunc->attrib==PCB)&&(abcjunc->flag==0)&&\
					(Line_Pad_Connect(&oneline,abcjunc,DRC_T_P)!=0))
				{
					tempbuf=newbuf;
					for(;;)
					{
						sq1 = (struct sq*)tempbuf;
						if(sq1->pointer == pp)
						{
							break;
						}
						else if(sq1->pointer == NULL)
						{
							break;
						}
						tempbuf+=6;
					}
					if(sq1->pointer == NULL)
					{
						sq2.pointer = pp;
						sq2.type = EMMJP;
						memcpy(tempnewbuf,&sq2,6);
						tempnewbuf+=6;
					}
				}
				pp+=16;
			}

			// to see if connect SMD Package's PAD or not 
			pp = pDoc->m_pPackage_Buffer;
			for(ii=0; ii<pDoc->m_nPackage_Number; ii++)
			{
				abcpackage = (struct Package*)pp;
				if(abcpackage->flag==0)
				{
					temp = pp;
					temp+=112;
					for(ll=0;ll<abcpackage->padnum;ll++)
					{
						abcjunc = (struct Junction*)temp;
						if(Line_Pad_Connect(&oneline,abcjunc,DRC_T_P)!=0)
						{
							tempbuf=newbuf;
							for(;;)
							{
								sq1 = (struct sq*)tempbuf;
								if(sq1->pointer == pp)
								{
									break;
								}
								else if(sq1->pointer == NULL)
								{
									break;
								}
								tempbuf+=6;
							}
							if(sq1->pointer == NULL)
							{
								sq2.pointer = pp;
								sq2.type=EMMPACKAGE;
								memcpy(tempnewbuf,&sq2,6);
								tempnewbuf+=6;
							}
							break;
						}
						temp+=16;
					}
				}
				pp += abcpackage->total16byte*16;
			}
       }
	   else if(sq.type==EMMJP)
       {
			temp = sq.pointer;
			memcpy(&onejunc,temp,16);
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				abcline = (struct Line*)pp;
				if((abcline->attrib==PCB)&&(abcline->flag==0)&&\
					(abcline->layer<=BOTTOMLAYER)&&(Line_Pad_Connect(abcline,&onejunc,0)!=0))
				{
					tempbuf=newbuf;
					for(;;)
					{
						sq1 = (struct sq*)tempbuf;
						if(sq1->pointer == pp)
						{
							if((deltay==0)&&(abcline->starty==abcline->endy))
							{ 
								if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
								{
									sq1->type=sq1->type|0x0010;
								}
								if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
								{
									sq1->type=sq1->type|0x0020;
								}
							}
							else if((deltax==0)&&(abcline->startx==abcline->endx))
							{
								if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
								{
									sq1->type=sq1->type|0x0010;
								}
								if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
								{
									sq1->type=sq1->type|0x0020;
								}
							}
							break;
						}
						else if(sq1->pointer == NULL)
						{
							break;
						}
						tempbuf+=6;
					}
					if(sq1->pointer == NULL)
					{
						sq2.pointer = pp;
						sq2.type=EMMLINE;
						if((deltay==0)&&(abcline->starty==abcline->endy))
						{
							if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
							{
								sq2.type=sq2.type|0x0010;
							}
							if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
							{
								sq2.type=sq2.type|0x0020;
							}
						}
						else if((deltax==0)&&(abcline->startx==abcline->endx))
						{
							if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
							{
								sq2.type=sq2.type|0x0010;
							}
							if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
							{
								sq2.type=sq2.type|0x0020;
							}
						}
						memcpy(tempnewbuf,&sq2,6);
						tempnewbuf+=6;
					}
				}
				pp+=16;
			}

			temp = sq.pointer;
			memcpy(&onejunc,temp,16);
			CloseMouseCursor();
			DrawStructJP(&onejunc,BK_Color,0,&dc);
			//----------- Undo -----------
			undoflag = 1;
			Check_Undo_Buffer(30);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_MOVE;
			head.ob = UNDO_JUNCTION;
			head.p = sq.pointer;
			head.dx = deltax;
			head.dy = deltay;
			memcpy(undo_p,&head,16);
			pDoc->m_pPCB_Undo_Current += 16;
			//---------------------------------------------------------------------------
			onejunc.centerx+=deltax;
			onejunc.centery+=deltay;
			memcpy(temp,&onejunc,16);
			DrawStructJP(&onejunc,PCB_LayerColor[onejunc.layer],0,&dc);
			OpenMouseCursor();
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				abcline = (struct Line*)pp;
				if((abcline->attrib==PCB)&&(abcline->flag==0)&&\
					(abcline->layer<=BOTTOMLAYER)&&(Line_Pad_Connect(abcline,&onejunc,DRC_T_P)!=0))
				{
					tempbuf=newbuf;
					for(;;)
					{
						sq1 = (struct sq*)tempbuf;
						if(sq1->pointer == pp)
						{
							break;
						}
						else if(sq1->pointer == NULL)
						{
							break;
						}
						tempbuf+=6;
					}
					if(sq1->pointer == NULL)
					{
						sq2.pointer = pp;
						sq2.type=EMMLINE;
						if((deltay==0)&&(abcline->starty==abcline->endy))
						{
							if(deltax>0)
							{
								if(abcline->startx<abcline->endx)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
							else
							{
								if(abcline->startx>abcline->endx)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
						}
						else if((deltax==0)&&(abcline->startx==abcline->endx))
						{
							if(deltay>0)
							{
								if(abcline->starty<abcline->endy)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
							else
							{
								if(abcline->starty>abcline->endy)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
						}
						memcpy(tempnewbuf,&sq2,6);
						tempnewbuf+=6;
					}
				}
				pp+=16;
			}

			pp = pDoc->m_pJunction_Buffer;
			for(ii=0; ii<pDoc->m_nJunction_Number; ii++)
			{	
				abcjunc = (struct Junction*)pp;
				if((abcjunc->attrib==PCB)&&(abcjunc->flag==0)&&\
					(Pad_Pad_Connect(&onejunc,abcjunc,DRC_P_P)!=0))
				{
					tempbuf=newbuf;
					for(;;)
					{
						sq1 = (struct sq*)tempbuf;
						if(sq1->pointer == pp)
						{
							break;
						}
						else if(sq1->pointer == NULL)
						{
						     break;
						}
						tempbuf+=6;
					}
					if(sq1->pointer == NULL)
					{
						sq2.pointer = pp;
						sq2.type=EMMJP;
						memcpy(tempnewbuf,&sq2,6);
						tempnewbuf+=6;
					}
				}
				pp+=16;
			}	
			pp = pDoc->m_pPackage_Buffer;
			for(ii=0; ii<pDoc->m_nPackage_Number; ii++)
			{
				abcpackage = (struct Package*)pp;
				if(abcpackage->flag==0)
				{
					temp = pp;
					temp += 112;
					for(ll=0;ll<abcpackage->padnum;ll++)
					{
						abcjunc = (struct Junction*)temp;
						if(Pad_Pad_Connect(&onejunc,abcjunc,DRC_P_P)!=0)
						{
							tempbuf=newbuf;
							for(;;)
							{
								sq1 = (struct sq*)tempbuf;
								if(sq1->pointer == pp)
								{
									break;
								}
								else if(sq1->pointer == NULL)
								{
									break;
								}
								tempbuf+=6;
							}
							if(sq1->pointer == NULL)
							{
								sq2.pointer = pp;
								sq2.type=EMMPACKAGE;
								memcpy(tempnewbuf,&sq2,6);
								tempnewbuf+=6;
							}
							break;
						}
						temp+=16;
					}
				}
				pp += abcpackage->total16byte*16;
			}
		}
		else if(sq.type==EMMPACKAGE)
		{
			// search for connected tracks 
			abcpackage = (struct Package*)sq.pointer;
			ttemp = (char*)abcpackage;
			ttemp += 112;
			num = abcpackage->padnum;
			for(l=0;l<num;l++)
			{
				memcpy(&onejunc,ttemp,16);
				pp = pDoc->m_pLine_Buffer;
				for(ii=0; ii<pDoc->m_nLine_Number; ii++)
				{	
					abcline = (struct Line*)pp;
					if((abcline->attrib==PCB)&&(abcline->flag==0)&&\
						(abcline->layer<=BOTTOMLAYER)&&(Line_Pad_Connect(abcline,&onejunc,0)!=0))
					{
						tempbuf=newbuf;
						for(;;)
						{
							sq1 = (struct sq*)tempbuf;
							if(sq1->pointer == pp)
							{
								if((deltay==0)&&(abcline->starty==abcline->endy))
								{
									if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
									{
										sq1->type=sq1->type|0x0010;
									}
									else if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
									{
										sq1->type=sq1->type|0x0020;
									}
								}
								else if((deltax==0)&&(abcline->startx==abcline->endx))
								{
									if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
									{
										sq1->type=sq1->type|0x0010;
									}
									else if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
									{
										sq1->type=sq1->type|0x0020;
									}
								}
								break;
							}
							else if(sq1->pointer == NULL)
							{
								break;
							}
							tempbuf+=6;
						}
						if(sq1->pointer == NULL)
						{
							sq2.pointer = pp;
							sq2.type = EMMLINE;
							if((deltay==0)&&(abcline->starty==abcline->endy))
							{
								if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
								{
									sq2.type=sq2.type|0x0010;
								}
								if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
								{
									sq2.type=sq2.type|0x0020;
							    }
							}
							else if((deltax==0)&&(abcline->startx==abcline->endx))
							{
								if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
								{
									sq2.type=sq2.type|0x0010;
								}
								if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
								{
									sq2.type=sq2.type|0x0020;
								}
							}
							memcpy(tempnewbuf,&sq2,6);
							tempnewbuf+=6;
						}
					}
					pp+=16;
				}
				ttemp+=16;
			}

			// refresh package 
			//----------- Undo -----------
			undoflag = 1;
			abcpackage = (struct Package*)sq.pointer;
			Check_Undo_Buffer(30);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_MOVE;
			head.ob = UNDO_PACKAGE;
			head.p = sq.pointer;
			head.dx = deltax;
			head.dy = deltay;
			head.state1 = (unsigned char)abcpackage->angle;
			head.state2 = (unsigned char)abcpackage->angle;
			memcpy(undo_p,&head,16);
			pDoc->m_pPCB_Undo_Current += 16;
			//----------- Undo -----------
			Check_Undo_Buffer(30);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_MOVE;
			head.ob = UNDO_PACKLIMMIT;
			head.p = sq.pointer;
			head.dx = deltax;
			head.dy = deltay;
			head.size1 = deltax;
			head.size2 = deltay;
			memcpy(undo_p,&head,16);
			pDoc->m_pPCB_Undo_Current += 16;
			//----------- Undo -----------
			DrawDeletePackage(sq.pointer,&dc);
			abcpackage = (struct Package*)sq.pointer;
			abcpackage->cx  += deltax;
			abcpackage->cy  += deltay;
			abcpackage->IDx += deltax;
			abcpackage->IDy += deltay;
			abcpackage->Cx  += deltax;
			abcpackage->Cy  += deltay;
			abcpackage->Px  += deltax;
			abcpackage->Py  += deltay;
			abcpackage->x   += deltax;
			abcpackage->y   += deltay;
			temp = sq.pointer;
			temp+=112;
			for(l=0;l<abcpackage->padnum;l++)
			{
				abcjunc = (struct Junction*)temp;
				abcjunc->centerx+=deltax;
				abcjunc->centery+=deltay;
				temp+=16;
			}
			for(l=0;l<abcpackage->lnum;l++)
			{
				abcline = (struct Line*)temp;
				abcline->startx+=deltax;
				abcline->starty+=deltay;
				abcline->endx+=deltax;
				abcline->endy+=deltay;
				temp+=16;
			}
			for(l=0;l<abcpackage->jnum;l++)
			{
				abcjunc = (struct Junction*)temp;
				abcjunc->centerx+=deltax;
				abcjunc->centery+=deltay;
				temp+=16;
			}
			for(l=0;l<abcpackage->snum;l++)
			{ 
				abcstring = (struct String*)temp;
				abcstring->sx+=deltax;
				abcstring->sy+=deltay;
				temp+=64;
			}
			for(l=0;l<abcpackage->anum;l++)
			{
				abcarc = (struct Arc*)temp;
				abcarc->centerx+=deltax;
				abcarc->centery+=deltay;
				temp+=16;
			}
			for(l=0;l<abcpackage->fnum;l++)
			{
				abcfill = (struct Fill*)temp;
				for(ll=0;ll<abcfill->vertexnum;ll++)
				{
					abcfill->pline[ll].x+=deltax;
					 abcfill->pline[ll].y+=deltay;
				}
				temp+=abcfill->total16byte*16;
			}
			DrawPackage(sq.pointer,&dc);

			// search for DRC tracks/vias/packages 
			abcpackage = (struct Package*)sq.pointer;
			ttemp = (char*)abcpackage;
			ttemp+=112;
			num=abcpackage->padnum;
			for(l=0;l<num;l++)
			{ 
				memcpy(&onejunc,ttemp,16);
	      		pp = pDoc->m_pLine_Buffer;
				for(ii=0; ii<pDoc->m_nLine_Number; ii++)
				{	
					abcline = (struct Line*)pp;
					if((abcline->attrib==PCB)&&(abcline->flag==0)&&\
					(abcline->layer<=BOTTOMLAYER)&&(Line_Pad_Connect(abcline,&onejunc,DRC_T_P)!=0))
					{
						tempbuf=newbuf;
						for(;;)
						{
							sq1 = (struct sq*)tempbuf;
							if(sq1->pointer == pp)
							{
								break;
							}
							else if(sq1->pointer == NULL)
							{
								break;
							}
							tempbuf+=6;
						}
						if(sq1->pointer == NULL)
						{
							sq2.pointer = pp;
							sq2.type=EMMLINE;
							if((deltay==0)&&(abcline->starty==abcline->endy))
							{
								if(deltax>0)
								{
									if(abcline->startx<abcline->endx)  sq2.type=sq2.type|0x0010;
									else sq2.type=sq2.type|0x0020;
								}
								else
								{
									if(abcline->startx>abcline->endx)  sq2.type=sq2.type|0x0010;
									else sq2.type=sq2.type|0x0020;
								}
							}
							else if((deltax==0)&&(abcline->startx==abcline->endx))
							{
								if(deltay>0)
								{
									if(abcline->starty<abcline->endy)  sq2.type=sq2.type|0x0010;
									else sq2.type=sq2.type|0x0020;
								}
								else
								{
									if(abcline->starty>abcline->endy)  sq2.type=sq2.type|0x0010;
									else sq2.type=sq2.type|0x0020;
								}
							}
							memcpy(tempnewbuf,&sq2,6);
							tempnewbuf+=6;
						}
					}
					pp+=16;
				}
				pp = pDoc->m_pJunction_Buffer;
				for(ii=0; ii<pDoc->m_nJunction_Number; ii++)
				{	
					abcjunc = (struct Junction*)pp;
					if((abcjunc->attrib==PCB)&&(abcjunc->flag==0)&&\
						(Pad_Pad_Connect(&onejunc,abcjunc,DRC_P_P)!=0))
					{
						tempbuf=newbuf;
						for(;;)
						{
							sq1 = (struct sq*)tempbuf;
							if(sq1->pointer == pp)
							{
								break;
							}
							else if(sq1->pointer == NULL)
							{
								break;
							}
							tempbuf+=6;
						}
						if(sq1->pointer == NULL)
						{
							sq2.pointer = pp;
							sq2.type = EMMJP;
							memcpy(tempnewbuf,&sq2,6);
							tempnewbuf+=6;
						}
					}
					pp+=16;
				}

				pp = pDoc->m_pPackage_Buffer;
				for(ii=0; ii<pDoc->m_nPackage_Number; ii++)
				{
					onepackage = (struct Package*)pp;
					if(onepackage->flag==0)
					{
						temp = pp;
						temp += 112;
						for(ll=0;ll<onepackage->padnum;ll++)
						{
							abcjunc = (struct Junction*)temp;
							if(Pad_Pad_Connect(&onejunc,abcjunc,DRC_P_P)!=0)
							{
								tempbuf=newbuf;
								for(;;)
								{
									sq1 = (struct sq*)tempbuf;
									if(sq1->pointer == pp)
									{
										break;
									}
									else if(sq1->pointer == NULL)
									{
										break;
									}
									tempbuf+=6;
								}
								if(sq1->pointer == NULL)
								{
									sq2.pointer = pp;
									sq2.type=EMMPACKAGE;
									memcpy(tempnewbuf,&sq2,6);
									tempnewbuf+=6;
								}
								break;
							}
							temp+=16;
						}
					}
					pp += onepackage->total16byte*16;
				}
				ttemp+=16;
			}
		}
	}

	movebuf=newbuf;
	for(;;)
	{
		memcpy(&sq,movebuf,6);
		movebuf+=6;
		if(sq.pointer == NULL) break;
		if((sq.type&0x000f)==EMMLINE)
		{
			temp = sq.pointer;
			memcpy(&oneline,temp,16);
			if(((deltay==0)&&(oneline.starty==oneline.endy))||\
				((deltax==0)&&(oneline.startx==oneline.endx)))
			{
				if((oneline.startx==oneline.endx)&&(oneline.starty==oneline.endy))
				{
					continue;
				}
				if(sq.type==17)
				{
					CloseMouseCursor();
					DrawStructLine(&oneline,BK_Color,&dc);
					//----------- Undo -----------
					undoflag = 1;
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_DRAGLINE;
					head.p = sq.pointer;
					head.dx = deltax;
					head.dy = deltay;
					head.size1 = 0;
					head.size2 = 0;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					//---------------------------------------------------------------------------
					oneline.startx+=deltax;
					oneline.starty+=deltay;
					memcpy(temp,&oneline,16);
					DrawStructLine(&oneline,PCB_LayerColor[oneline.layer],&dc);
					OpenMouseCursor();
				}
				else if(sq.type==33)
				{
					CloseMouseCursor();
					DrawStructLine(&oneline,BK_Color,&dc);
					//----------- Undo -----------
					undoflag = 1;
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_DRAGLINE;
					head.p = sq.pointer;
					head.dx = 0;
					head.dy = 0;
					head.size1 = deltax;
					head.size2 = deltay;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					//---------------------------------------------------------------------------
					oneline.endx+=deltax;
					oneline.endy+=deltay;
					memcpy(temp,&oneline,16);
					DrawStructLine(&oneline,PCB_LayerColor[oneline.layer],&dc);
					OpenMouseCursor();
				}
				else if((sq.type==1)&&\
					(((deltay==0)&&(oneline.starty==oneline.endy))||\
					((deltax==0)&&(oneline.startx==oneline.endx))))
				{
					continue;
				}
				else
				{
					temp = sq.pointer;
					memcpy(&oneline,temp,16);
					CloseMouseCursor();
					DrawStructLine(&oneline,BK_Color,&dc);
					//----------- Undo -----------
					undoflag = 1;
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_LINE;
					head.p = sq.pointer;
					head.dx = deltax;
					head.dy = deltay;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					//---------------------------------------------------------------------------
					oneline.startx+=deltax;
					oneline.starty+=deltay;
					oneline.endx+=deltax;
					oneline.endy+=deltay;
					memcpy(temp,&oneline,16);
					DrawStructLine(&oneline,PCB_LayerColor[oneline.layer],&dc);
					OpenMouseCursor();
				}
			}
		}
	}
	//----------- Undo -----------
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------

	goto SBEGIN;
}

void CDesignView::OnOptimizeDrip() 
{
	unsigned ch1;
	unsigned x1,y1;
	int padnum;
	char *p, *pointer, *temp, *fillbuff;
	int i,j,l,flag;
	struct Package *abcpack;
	char *hstr1 = "选择焊盘";
	struct Junction abcjunc;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	fillbuff = pDoc->m_pFill_Buffer_Temp;
	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(hstr1);
	flag=0;
	do 
	{
		ch1 = MouseKey();
		if((ch1==VK_RETURN)||(ch1==PUSHMOUSE))
		{
			x1 = m_nCurrent_X;
			y1 = m_nCurrent_Y;
			flag=0;
			if(Select_JP(x1,y1,&pointer) != 0) flag=1;
			else if(Select_PackagePad(x1,y1,&pointer,&padnum)!=0) flag=2;
			if(flag==0)
			{
				MessageBeep(0xFFFFFFFF);
				continue;
			}
			else
			{
				if(flag==1)
				{
					memcpy(&gnjunction,pointer,16);
				}
				else if(flag==2) 
				{
					abcpack = (struct Package*)pointer;
					temp = pointer;
					temp+=112;
					for(l=0;l<abcpack->padnum;l++)
					{
						if(l==padnum)
						{
							memcpy(&gnjunction,temp,16);
							break;
						}
						temp+=16;
					}
				}
				generate_drip(gnjunction);
				CloseMouseCursor();
				DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],0,&dc);
				OpenMouseCursor();

				//j = FreeMenu(200,160,32,2,changeallpad);
				j = AfxMessageBox("修改所有相同属性焊盘?", MB_YESNO|MB_ICONQUESTION);
				if( j == IDYES)
				{
					if(flag==1)
					{
						p = pDoc->m_pJunction_Buffer;
						for(i=0; i<pDoc->m_nJunction_Number; i++)
						{	
							memcpy(&abcjunc,p,16);
							if((abcjunc.attrib==PCB)&&(abcjunc.Dx==gnjunction.Dx)&&\
								(abcjunc.Dy==gnjunction.Dy)&&(abcjunc.d==gnjunction.d)&&\
								(abcjunc.padtype==gnjunction.padtype)&&(abcjunc.flag==0)&&\
								((abcjunc.centerx!=gnjunction.centerx)||(abcjunc.centery!=gnjunction.centery)))
							{
								generate_drip(abcjunc);
								CloseMouseCursor();
								DrawStructJP(&abcjunc,PCB_LayerColor[abcjunc.layer],0,&dc);
								OpenMouseCursor();
							}
							p+=16;
						}	
					}
					else if(flag==2)
					{
						p = pDoc->m_pPackage_Buffer;
						for(i=0; i<pDoc->m_nPackage_Number; i++)
						{
							abcpack = (struct Package*)p;
							if(abcpack->flag==0)
							{
								temp = p;
								temp += 112;
								for(l=0;l<abcpack->padnum;l++)
								{
									memcpy(&abcjunc,temp,16);
									if((abcjunc.Dx==gnjunction.Dx)&&(abcjunc.Dy==gnjunction.Dy)&&\
									(abcjunc.d==gnjunction.d)&&(abcjunc.padtype==gnjunction.padtype)&&\
									((abcjunc.centerx!=gnjunction.centerx)||(abcjunc.centery!=gnjunction.centery)))
									{
										generate_drip(abcjunc);
										CloseMouseCursor();
										DrawStructJP(&abcjunc,PCB_LayerColor[abcjunc.layer],0,&dc);
										OpenMouseCursor();
									}
									temp+=16;
								}
							}
							p+=abcpack->total16byte*16;
						}
					}
				}
			}
		}
		else  Command_In_Function(ch1);
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));

	//----------- Undo -----------
	if(fillbuff != pDoc->m_pFill_Buffer_Temp)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------
	CL_Auto_Pan=OFF;
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	Write_Help(commandend);
}


void CDesignView::generate_drip(struct Junction onejunction)
{
	unsigned x1,y1;
	char *p;
	int i;
	struct Line *abcline;
	float alpha;
	unsigned u1;
	float f1=0.87, f2=0.25, f3=0.96, ff=0.7;
	unsigned x2,y2,x3,y3;
	struct Fill onefill;
	struct Undo_Head head;
	char *undo_p;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	onefill.id=0x70;
	onefill.vertexnum=4;
	onefill.fillpattern=0;
	onefill.total16byte=2;
	onefill.u1=onefill.u2=0;
	onefill.type=0;
	onefill.u3=0;
	onefill.attrib=PCB;
	onefill.u4=0;
	onefill.u5=0;
	onefill.flag=0;
	// generate drip 
	// search for track connected 
	if(((onejunction.padtype&0x07)==1)||\
      (((onejunction.padtype&0x07)==3)&&(onejunction.Dx==onejunction.Dy)))
    {
		p = pDoc->m_pLine_Buffer;
		for(i=0; i<pDoc->m_nLine_Number; i++)
		{	
			abcline = (struct Line*)p;
			if( (abcline->attrib==PCB)&&(abcline->flag==0)&&(abcline->layer<=BOTTOMLAYER)&&\
				((abcline->layer==onejunction.layer)||(onejunction.layer==MULTILAYER))&&\
				( (Is_On_JP(abcline->startx,abcline->starty,&onejunction)!=0)||\
				  (Is_On_JP(abcline->endx,abcline->endy,&onejunction)!=0)||\
				  (Is_On_Line(onejunction.centerx,onejunction.centery,abcline)!=0))  )
			{
				if((Is_On_JP(abcline->startx,abcline->starty,&onejunction)==0)||\
					(Is_On_JP(abcline->endx,abcline->endy,&onejunction)==0))
				{

					if(Is_On_JP(abcline->startx,abcline->starty,&onejunction)!=0)
					{
						if(SQRT(abcline->endx,abcline->endy,onejunction.centerx,onejunction.centery)>onejunction.Dx/2)
						{
							if((abcline->starty==abcline->endy)&&(abcline->endx>abcline->startx)) alpha=0;
							else if((abcline->starty==abcline->endy)&&(abcline->endx<abcline->startx)) alpha=180;
							else if((abcline->startx==abcline->endx)&&(abcline->endy>abcline->starty)) alpha=90;
							else if((abcline->startx==abcline->endx)&&(abcline->endy<abcline->starty)) alpha=270;
							else alpha=180+(180/CL_pi)*atan2(((float)abcline->starty-(float)abcline->endy),\
								((float)abcline->startx-(float)abcline->endx));
							if(((int)alpha>338)||((int)alpha<=22))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->endx<u1) x1=abcline->endx;
								else x1=u1;
								if(alpha==0) y1=abcline->starty;
								else y1=abcline->starty+((float)x1-abcline->startx)*((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx+onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>22)&&((int)alpha<=67))
							{ 
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->endx<u1) x1=abcline->endx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery-f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery+f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>67)&&((int)alpha<=112))
							{
								u1=onejunction.centery+onejunction.Dx;
								if(abcline->endy<u1) y1=abcline->endy;
								else y1=u1;
								if(alpha==90) x1=abcline->startx;
								else x1=abcline->startx+((float)y1-abcline->starty)*((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								x2=onejunction.centerx+f1*onejunction.Dx/2;
								y2=onejunction.centery+onejunction.Dx/4;
								x3=onejunction.centerx-f1*onejunction.Dx/2;
								y3=onejunction.centery+onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>112)&&((int)alpha<=157))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->endx>u1) x1=abcline->endx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+f2*onejunction.Dx/2;
								y2=onejunction.centery+f3*onejunction.Dx/2;
								x3=onejunction.centerx-f3*onejunction.Dx/2;
								y3=onejunction.centery-f2*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>157)&&((int)alpha<=202))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->endx>u1) x1=abcline->endx;
								else x1=u1;
								if(alpha==180) y1=abcline->starty;
								else y1=abcline->starty+((float)x1-abcline->startx)*((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx-onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx-onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>202)&&((int)alpha<=247))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->endx>u1) x1=abcline->endx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx-f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx+f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>247)&&((int)alpha<=292))
							{
								u1=onejunction.centery-onejunction.Dx;
								if(abcline->endy>u1) y1=abcline->endy;
								else y1=u1;
								if(alpha==270) x1=abcline->startx;
								else x1=abcline->startx+((float)y1-abcline->starty)*((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								x2=onejunction.centerx-f1*onejunction.Dx/2;
								y2=onejunction.centery-onejunction.Dx/4;
								x3=onejunction.centerx+f1*onejunction.Dx/2;
								y3=onejunction.centery-onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>292)&&((int)alpha<=338))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->endx<u1) x1=abcline->endx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
						}
					}
					else if(Is_On_JP(abcline->endx,abcline->endy,&onejunction)!=0)
					{
						if(SQRT(abcline->startx,abcline->starty,onejunction.centerx,onejunction.centery)>onejunction.Dx/2)
						{
							if((abcline->starty==abcline->endy)&&(abcline->startx>abcline->endx)) alpha=0;
							else if((abcline->starty==abcline->endy)&&(abcline->startx<abcline->endx)) alpha=180;
							else if((abcline->startx==abcline->endx)&&(abcline->starty>abcline->endy)) alpha=90;
							else if((abcline->startx==abcline->endx)&&(abcline->starty<abcline->endy)) alpha=270;
							else alpha=180+(180/CL_pi)*atan2(((float)abcline->endy-(float)abcline->starty),\
								((float)abcline->endx-(float)abcline->startx));
							if(((int)alpha>338)||((int)alpha<=22))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->startx<u1) x1=abcline->startx;
								else x1=u1;
								if(alpha==0) y1=abcline->starty;
								else y1=abcline->starty+((float)x1-abcline->startx)*((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx+onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>22)&&((int)alpha<=67))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->startx<u1) x1=abcline->startx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery-f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery+f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>67)&&((int)alpha<=112))
							{
								u1=onejunction.centery+onejunction.Dx;
								if(abcline->starty<u1) y1=abcline->starty;
								else y1=u1;
								if(alpha==90) x1=abcline->startx;
								else x1=abcline->startx+((float)y1-abcline->starty)*((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								x2=onejunction.centerx+f1*onejunction.Dx/2;
								y2=onejunction.centery+onejunction.Dx/4;
								x3=onejunction.centerx-f1*onejunction.Dx/2;
								y3=onejunction.centery+onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>112)&&((int)alpha<=157))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->startx>u1) x1=abcline->startx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+f2*onejunction.Dx/2;
								y2=onejunction.centery+f3*onejunction.Dx/2;
								x3=onejunction.centerx-f3*onejunction.Dx/2;
								y3=onejunction.centery-f2*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>157)&&((int)alpha<=202))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->startx>u1) x1=abcline->startx;
								else x1=u1;
								if(alpha==180) y1=abcline->starty;
								else y1=abcline->starty+((float)x1-abcline->startx)*((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx-onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx-onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>202)&&((int)alpha<=247))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->startx>u1) x1=abcline->startx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx-f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx+f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>247)&&((int)alpha<=292))
							{
								u1=onejunction.centery-onejunction.Dx;
								if(abcline->starty>u1) y1=abcline->starty;
								else y1=u1;
								if(alpha==270) x1=abcline->startx;
								else x1=abcline->startx+((float)y1-abcline->starty)*((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								x2=onejunction.centerx-f1*onejunction.Dx/2;
								y2=onejunction.centery-onejunction.Dx/4;
								x3=onejunction.centerx+f1*onejunction.Dx/2;
								y3=onejunction.centery-onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>292)&&((int)alpha<=338))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->startx<u1) x1=abcline->startx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
						}
					}
					else
					{
						if(SQRT(abcline->endx,abcline->endy,onejunction.centerx,onejunction.centery)>onejunction.Dx/2)
						{
							if((abcline->starty==abcline->endy)&&(abcline->endx>abcline->startx)) alpha=0;
							else if((abcline->starty==abcline->endy)&&(abcline->endx<abcline->startx)) alpha=180;
							else if((abcline->startx==abcline->endx)&&(abcline->endy>abcline->starty)) alpha=90;
							else if((abcline->startx==abcline->endx)&&(abcline->endy<abcline->starty)) alpha=270;
							else alpha=180+(180/CL_pi)*atan2(((float)abcline->starty-(float)abcline->endy),\
								((float)abcline->startx-(float)abcline->endx));
							if(((int)alpha>338)||((int)alpha<=22))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->endx<u1) x1=abcline->endx;
								else x1=u1;
								//if(alpha==0) y1 = abcline->starty;
								if(alpha==0) y1 = onejunction.centery; 
								//else y1 = abcline->starty+((float)x1-abcline->startx)*((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								else y1 = onejunction.centery+((float)x1-onejunction.centerx)*\
									((float)abcline->endy-onejunction.centery)/((float)abcline->endx-onejunction.centerx);
								x2=onejunction.centerx+onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx+onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>22)&&((int)alpha<=67))
							{ 
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->endx<u1) x1=abcline->endx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=onejunction.centery+((float)x1-onejunction.centerx)*\
									((float)abcline->endy-onejunction.centery)/((float)abcline->endx-onejunction.centerx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery-f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery+f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>67)&&((int)alpha<=112))
							{
								u1=onejunction.centery+onejunction.Dx;
								if(abcline->endy<u1) y1=abcline->endy;
								else y1=u1;
								//if(alpha==90) x1=abcline->startx;
								if(alpha==90) x1=onejunction.centerx;
								//else x1=abcline->startx+((float)y1-abcline->starty)*\
								//  ((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								else x1=onejunction.centerx+((float)y1-onejunction.centery)*\
									((float)abcline->endx-onejunction.centerx)/((float)abcline->endy-onejunction.centery);
								x2=onejunction.centerx+f1*onejunction.Dx/2;
								y2=onejunction.centery+onejunction.Dx/4;
								x3=onejunction.centerx-f1*onejunction.Dx/2;
								y3=onejunction.centery+onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>112)&&((int)alpha<=157))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->endx>u1) x1=abcline->endx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=onejunction.centery+((float)x1-onejunction.centerx)*\
									((float)abcline->endy-onejunction.centery)/((float)abcline->endx-onejunction.centerx);
								x2=onejunction.centerx+f2*onejunction.Dx/2;
								y2=onejunction.centery+f3*onejunction.Dx/2;
								x3=onejunction.centerx-f3*onejunction.Dx/2;
								y3=onejunction.centery-f2*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>157)&&((int)alpha<=202))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->endx>u1) x1=abcline->endx;
								else x1=u1;
								//if(alpha==180) y1=abcline->starty;
								if(alpha==180) y1=onejunction.centery;
								//else y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								else y1=onejunction.centery+((float)x1-onejunction.centerx)*\
									((float)abcline->endy-onejunction.centery)/((float)abcline->endx-onejunction.centerx);
								x2=onejunction.centerx-onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx-onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>202)&&((int)alpha<=247))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->endx>u1) x1=abcline->endx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=onejunction.centery+((float)x1-onejunction.centerx)*\
									((float)abcline->endy-onejunction.centery)/((float)abcline->endx-onejunction.centerx);
								x2=onejunction.centerx-f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx+f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>247)&&((int)alpha<=292))
							{
								u1=onejunction.centery-onejunction.Dx;
								if(abcline->endy>u1) y1=abcline->endy;
								else y1=u1;
								//if(alpha==270) x1=abcline->startx;
								if(alpha==270) x1=onejunction.centerx;
								//else x1=abcline->startx+((float)y1-abcline->starty)*\
								//  ((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								else x1=onejunction.centerx+((float)y1-onejunction.centery)*\
									((float)abcline->endx-onejunction.centerx)/((float)abcline->endy-onejunction.centery);
								x2=onejunction.centerx-f1*onejunction.Dx/2;
								y2=onejunction.centery-onejunction.Dx/4;
								x3=onejunction.centerx+f1*onejunction.Dx/2;
								y3=onejunction.centery-onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>292)&&((int)alpha<=338))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->endx<u1) x1=abcline->endx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=onejunction.centery+((float)x1-onejunction.centerx)*\
									((float)abcline->endy-onejunction.centery)/((float)abcline->endx-onejunction.centerx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
						}
						
						if(SQRT(abcline->startx,abcline->starty,onejunction.centerx,onejunction.centery)>onejunction.Dx/2)
						{
							if((abcline->starty==abcline->endy)&&(abcline->startx>abcline->endx)) alpha=0;
							else if((abcline->starty==abcline->endy)&&(abcline->startx<abcline->endx)) alpha=180;
							else if((abcline->startx==abcline->endx)&&(abcline->starty>abcline->endy)) alpha=90;
							else if((abcline->startx==abcline->endx)&&(abcline->starty<abcline->endy)) alpha=270;
							else alpha=180+(180/CL_pi)*atan2(((float)abcline->endy-(float)abcline->starty),\
								((float)abcline->endx-(float)abcline->startx));
							if(((int)alpha>338)||((int)alpha<=22))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->startx<u1) x1=abcline->startx;
								else x1=u1;
								if(alpha==0) y1=abcline->starty;
								//else y1=abcline->starty+((float)x1-abcline->startx)*((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								else y1=abcline->starty+((float)x1-abcline->startx)*((float)onejunction.centery-abcline->starty)/((float)onejunction.centerx-abcline->startx);
								x2=onejunction.centerx+onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx+onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>22)&&((int)alpha<=67))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->startx<u1) x1=abcline->startx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)onejunction.centery-abcline->starty)/((float)onejunction.centerx-abcline->startx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery-f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery+f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>67)&&((int)alpha<=112))
							{
								u1=onejunction.centery+onejunction.Dx;
								if(abcline->starty<u1) y1=abcline->starty;
								else y1=u1;
								if(alpha==90) x1=abcline->startx;
								//else x1=abcline->startx+((float)y1-abcline->starty)*((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								else x1=abcline->startx+((float)y1-abcline->starty)*((float)onejunction.centerx-abcline->startx)/((float)onejunction.centery-abcline->starty);
								x2=onejunction.centerx+f1*onejunction.Dx/2;
								y2=onejunction.centery+onejunction.Dx/4;
								x3=onejunction.centerx-f1*onejunction.Dx/2;
								y3=onejunction.centery+onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>112)&&((int)alpha<=157))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->startx>u1) x1=abcline->startx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)onejunction.centery-abcline->starty)/((float)onejunction.centerx-abcline->startx);
								x2=onejunction.centerx+f2*onejunction.Dx/2;
								y2=onejunction.centery+f3*onejunction.Dx/2;
								x3=onejunction.centerx-f3*onejunction.Dx/2;
								y3=onejunction.centery-f2*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>157)&&((int)alpha<=202))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->startx>u1) x1=abcline->startx;
								else x1=u1;
								if(alpha==180) y1=abcline->starty;
								//else y1=abcline->starty+((float)x1-abcline->startx)*((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								else y1=abcline->starty+((float)x1-abcline->startx)*((float)onejunction.centery-abcline->starty)/((float)onejunction.centerx-abcline->startx);
								x2=onejunction.centerx-onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx-onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>202)&&((int)alpha<=247))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->startx>u1) x1=abcline->startx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)onejunction.centery-abcline->starty)/((float)onejunction.centerx-abcline->startx);
								x2=onejunction.centerx-f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx+f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>247)&&((int)alpha<=292))
							{
								u1=onejunction.centery-onejunction.Dx;
								if(abcline->starty>u1) y1=abcline->starty;
								else y1=u1;
								if(alpha==270) x1=abcline->startx;
								//else x1=abcline->startx+((float)y1-abcline->starty)*((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								else x1=abcline->startx+((float)y1-abcline->starty)*((float)onejunction.centerx-abcline->startx)/((float)onejunction.centery-abcline->starty);
								x2=onejunction.centerx-f1*onejunction.Dx/2;
								y2=onejunction.centery-onejunction.Dx/4;
								x3=onejunction.centerx+f1*onejunction.Dx/2;
								y3=onejunction.centery-onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>292)&&((int)alpha<=338))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->startx<u1) x1=abcline->startx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)onejunction.centery-abcline->starty)/((float)onejunction.centerx-abcline->startx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
						}
					}
				}
			}
			p+=16;
		}	
    }
}

void CDesignView::OnOptimizeAutofill() 
{
	int i,ii,j,p,delta,trackwidth, optimize, filllimit;
	int flag, undoflag;
	int t_t, t_p, p_p;
	unsigned ch1;
	unsigned x1,y1,x2,y2,tempd;
	int xs,ys,xe,ye;
	struct Junction via;
	struct Line oneline;
	int fd, mode, linenum, linenum2;
	char *p1, *p2, *position, *position2;
	struct Line *abcline1, *abcline2;
	struct Undo_Head head;
	char *undo_p;

	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);
	
	int drawmode = dc.GetROP2();

	if(m_nPCB_Current_Layer>BOTTOMLAYER)
    {
		Error("当前图层无法放置覆铜区.");
		return;
    }

	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(helpstr1);

	flag=0;
	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{ 
			CL_Auto_Pan=OFF;
			DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
			Write_Help(commandend);
			return ;
		}
		if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
		{
			x1 = m_nCurrent_X;
			y1 = m_nCurrent_Y;
			Get_VPort_XY(&xs,&ys,x1,y1);
			xe=xs;
			ye=ys;
			//CloseMouseCursor();
			Write_Help(helpstr2);
			//OpenMouseCursor();
			do
			{
				ch1=MouseKey();
				Get_VPort_XY(&xs,&ys,x1,y1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					//CloseMouseCursor();
					//setwritemode(1);
					//setcolor(Overwrite_Color);
					//rectangle(xs,ys,xe,ye);
					dc.SetROP2(R2_NOTXORPEN);
					rectangle(&dc,xs,ys,xe,ye);
					dc.SetROP2(drawmode);
					CL_Auto_Pan = OFF;
					DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
					//OpenMouseCursor();
					return ;
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
						if(CL_RedrawFlag==1){ CL_RedrawFlag=0;}
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
					//rectangle(xs,ys,xe,ye);
					dc.SetROP2(R2_NOTXORPEN);
					rectangle(&dc,xs,ys,xe,ye);
					dc.SetROP2(drawmode);
					//OpenMouseCursor();
					break;
				}
				else  Command_In_Function(ch1);
			}while(1);
		}
		else  Command_In_Function(ch1);
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));

    if(x1>x2)
	{
		tempd=x2;
		x2=x1;
		x1=tempd;
	}
    if(y1>y2)
	{
		tempd=y2;
		y2=y1;
		y1=tempd;
	}

	CAutoFill autofill;
	autofill.m_nFill_D = Auto_Fill_D;
	autofill.m_nTrack_Width = Auto_Fill_Width;
	autofill.m_nFill_Mode = Auto_Fill_Mode;
	autofill.m_nFill_Grid = Auto_Fill_Grid;
	autofill.m_nOptimize = TRUE;
	autofill.m_nAutofill_Limit = 1000;

	Write_Help("定义填充模式和参数");
    i = autofill.DoModal();
	if(i != IDOK)
	{
		CL_Auto_Pan=OFF;
		DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
		Write_Help(commandend);
		return;
	}
	
	position = pDoc->m_pLine_Buffer_Temp;
	linenum = 0;

	Auto_Fill_D = autofill.m_nFill_D ;
	Auto_Fill_Width = autofill.m_nTrack_Width;
	Auto_Fill_Mode = autofill.m_nFill_Mode;
	Auto_Fill_Grid = autofill.m_nFill_Grid;
	optimize = autofill.m_nOptimize;
	filllimit = autofill.m_nAutofill_Limit;

	p = Auto_Fill_D;
	trackwidth = Auto_Fill_Width;
	mode = Auto_Fill_Mode + 1;
	fd = Auto_Fill_Grid;
    
	t_t = DRC_T_T;
    t_p = DRC_T_P;
    p_p = DRC_P_P;
    DRC_T_T = p;
	DRC_T_P = p;
	DRC_P_P = p;
	if(DRC_T_T < t_t) DRC_T_T = t_t;
	if(DRC_T_P < t_p) DRC_T_P = t_p;
	if(DRC_P_P < p_p) DRC_P_P = p_p;

	if(pDoc->m_nNet_Number!=0) OnAutoHighlightNet();

    via.id = 0x21;
    via.Dx = via.Dy = trackwidth;
    via.d=0;
    via.padtype = 0x81;
    via.attrib = PCB;
    via.layer = (unsigned char)m_nPCB_Current_Layer;
    via.flag = 0;
    strcpy((char*)via.padname,Null_Str);

    oneline.flag = 1;
    oneline.id = 0x10;
    oneline.layer = (unsigned char)m_nPCB_Current_Layer;
    oneline.linewidth = trackwidth;
    oneline.attrib = PCB;
    oneline.type = 0;

    Write_Help("自动覆铜...");
	EnableFrame(FALSE);

	POINT pp;
	int ix,iy;
	Get_VPort_XY(&ix,&iy,m_nCurrent_X,m_nCurrent_Y);
	pp.x = ix; pp.y = iy;
	DrawMyCursor(&dc,&pp);

	undoflag = 0;
    
    if((mode==1)||(mode==3)||(mode==4))
	{
		via.centerx = x1;
	    via.centery = y1;
	    i = Clearance_Route((struct Line*)&via);
	    if(i==0)
		{
			oneline.startx = via.centerx;
			oneline.starty = via.centery;
			flag=0;
		}
	    else flag=1;
	    delta = p;  
	    for(;;)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					j = AfxMessageBox(interuptautofill, MB_YESNO|MB_ICONWARNING);
					if( j == IDYES)
					{
						goto SEEDEND;
					}
				}
				break;
			}
			via.centerx += delta;
			if(via.centerx >x2)
			{
				if(flag==0)
				{
					oneline.endx = x2;
					oneline.endy = via.centery;
					//------------- Undo -----------------
					undoflag = 1;
					Check_Undo_Buffer(20);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_DELETE;
					head.ob = UNDO_LINE;
					head.p = pDoc->m_pLine_Buffer_Temp;
					memcpy(undo_p,&head,6);
					pDoc->m_pPCB_Undo_Current += 6;
					//-------------------------------------
					pDoc->Save_EMMLine(&oneline);
					linenum++;
					CloseMouseCursor();
					DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
					OpenMouseCursor();
				}
				via.centerx = x1;
				if(via.centery==y2) break;
				via.centery += ( trackwidth - 1);
				if(via.centery>y2) via.centery=y2;
				i = Clearance_Route((struct Line*)&via);
				if(i==0)
				{
					oneline.startx = via.centerx;
					oneline.starty = via.centery;
					flag=0;
				}
				else flag=1;
				delta = p; 
				continue;
			}
			i = Clearance_Route((struct Line*)&via);
			if(i==0)
			{
				if(flag==1)
				{
					if(delta==1)
					{
						oneline.startx = via.centerx;
						oneline.starty = via.centery;
						flag=0;
						delta = p; 
					}
					else
					{
						via.centerx -= delta;
						delta = delta/2;
					}
				}
			}
			else
			{
				if(flag==0)
				{
					if(delta==1)
					{
						oneline.endx = via.centerx-1;
						oneline.endy = via.centery;
						//------------- Undo -----------------
						undoflag = 1;
						Check_Undo_Buffer(20);
						undo_p = pDoc->m_pPCB_Undo_Current;
						head.op = UNDO_DELETE;
						head.ob = UNDO_LINE;
						head.p = pDoc->m_pLine_Buffer_Temp;
						memcpy(undo_p,&head,6);
						pDoc->m_pPCB_Undo_Current += 6;
						//-------------------------------------
						pDoc->Save_EMMLine(&oneline);
						linenum++;
						CloseMouseCursor();
						DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
						OpenMouseCursor();
						flag=1;
						delta = p;
						continue;
					}
					else
					{
						via.centerx -=delta;
						delta = delta/2;
						continue;
					}
				}
			}
		}
	}

	position2 = pDoc->m_pLine_Buffer_Temp;
	linenum2 = 0;
    if((mode==2)||(mode==3)||(mode==4))
	{
	    via.centerx=x1;
	    via.centery=y1;
	    i = Clearance_Route((struct Line*)&via);
	    if(i==0)
		{
			oneline.startx = via.centerx;
			oneline.starty = via.centery;
			flag=0;
		}
	    else flag=1;
	    delta = p;
	    for(;;)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					j = AfxMessageBox(interuptautofill, MB_YESNO|MB_ICONWARNING);
					if( j == IDYES)
					{
						 goto SEEDEND;
					}
				}
				break;
			}
			via.centery+=delta;
			if(via.centery >y2)
			{
				if(flag==0)
				{
					oneline.endy = y2;
					oneline.endx = via.centerx;
					//------------- Undo -----------------
					undoflag = 1;
					Check_Undo_Buffer(20);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_DELETE;
					head.ob = UNDO_LINE;
					head.p = pDoc->m_pLine_Buffer_Temp;
					memcpy(undo_p,&head,6);
					pDoc->m_pPCB_Undo_Current += 6;
					//-------------------------------------
					pDoc->Save_EMMLine(&oneline);
					linenum2++;
					CloseMouseCursor();
					DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
					OpenMouseCursor();
				}
				via.centery=y1;
				if(via.centerx==x2) break;
				via.centerx += (trackwidth - 1);
				if(via.centerx>x2) via.centerx=x2;
				i = Clearance_Route((struct Line*)&via);
				if(i==0)
				{
					oneline.startx = via.centerx;
					oneline.starty = via.centery;
					flag=0;
				}
				else flag=1;
				delta = p; 
				continue;
			}
			i=Clearance_Route((struct Line*)&via);
			if(i==0)
			{
				if(flag==1)
				{
					if(delta==1)
					{
						oneline.startx = via.centerx;
						oneline.starty = via.centery;
						flag=0;
						delta = p;
					}
					else
					{
						via.centery -=delta;
						delta = delta/2;
					}
				}
			}
			else
			{
				if(flag==0)
				{
					if(delta==1)
					{
						oneline.endy = via.centery-1;
						oneline.endx = via.centerx;
						//------------- Undo -----------------
						undoflag = 1;
						Check_Undo_Buffer(20);
						undo_p = pDoc->m_pPCB_Undo_Current;
						head.op = UNDO_DELETE;
						head.ob = UNDO_LINE;
						head.p = pDoc->m_pLine_Buffer_Temp;
						memcpy(undo_p,&head,6);
						pDoc->m_pPCB_Undo_Current += 6;
						//-------------------------------------
						pDoc->Save_EMMLine(&oneline);
						linenum2++;
						CloseMouseCursor();
						DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
						OpenMouseCursor();
						flag=1;
						delta = p; 
						continue;
					}
					else
					{
						via.centery -=delta;
						delta = delta/2;
						continue;
					}
				}
			}
		}
	}

SEEDEND:
	if((mode == 1)||(mode == 3)||(mode == 4))
	{
		if((mode != 3)&&(mode != 4))
		{
			p1 = position;
			for(i=0; i<linenum; i++)
			{	
				abcline1 = (struct Line*)p1;
				if((abcline1->attrib == PCB)&&(abcline1->flag == 1))
				{
					if( (i+1) >= linenum) break;
					abcline2 = (struct Line*)(p1 + 16);
					if((abcline2->attrib == PCB)&&(abcline2->flag == 1))
					{
						if(abcline2->starty == abcline1->starty)
						{
							oneline.flag = 2;
							oneline.startx = abcline1->endx;
							oneline.endx = abcline2->startx;
							oneline.starty = abcline1->starty;
							oneline.endy = abcline1->endy;
							if(p > trackwidth) 
							{
								DRC_T_T = p - trackwidth;
								DRC_T_P = p - trackwidth;
								if(DRC_T_T < t_t) DRC_T_T = t_t;
								if(DRC_T_P < t_p) DRC_T_P = t_p;
							}
							j = Clearance_Route(&oneline);
							if(j == 0)
							{
								//------------- Undo -----------------
								undoflag = 1;
								Check_Undo_Buffer(20);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_LINE;
								head.p = pDoc->m_pLine_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//-------------------------------------
								pDoc->Save_EMMLine(&oneline);
								CloseMouseCursor();
								DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
								OpenMouseCursor();
							}
						}
					}
				}
				p1 += 16;
			}
		}
	
		p1 = position; //pDoc->m_pLine_Buffer;
		for(i=0; i<linenum; i++)
		{	
			abcline1 = (struct Line*)p1;
			if((abcline1->attrib == PCB)&&(abcline1->flag == 1))
			{
				flag = 0;
				p2 = p1;
				for(ii = i; ii<linenum; ii++)
				{	
					abcline2 = (struct Line*)p2;
					if((abcline2->attrib == PCB)&&(abcline2->flag == 1))
					{
						if(abcline2->starty == abcline1->starty){}
						else if(abcline2->starty == (abcline1->starty + trackwidth - 1))
						{
							if(abs(abcline2->startx - abcline1->startx) <= p)//trackwidth)
							{
								oneline.flag = 2;
								oneline.startx = abcline1->startx;
								oneline.starty = abcline1->starty;
								oneline.endx = abcline2->startx;
								oneline.endy = abcline2->starty;
								//------------- Undo -----------------
								undoflag = 1;
								Check_Undo_Buffer(20);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_LINE;
								head.p = pDoc->m_pLine_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//-------------------------------------
								pDoc->Save_EMMLine(&oneline);
								CloseMouseCursor();
								DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
								OpenMouseCursor();
							}
							if(abs(abcline2->endx - abcline1->endx) <= p) //trackwidth)
							{
								oneline.flag = 2;
								oneline.startx = abcline1->endx;
								oneline.starty = abcline1->endy;
								oneline.endx = abcline2->endx;
								oneline.endy = abcline2->endy;
								//------------- Undo -----------------
								undoflag = 1;
								Check_Undo_Buffer(20);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_LINE;
								head.p = pDoc->m_pLine_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//-------------------------------------
								pDoc->Save_EMMLine(&oneline);
								CloseMouseCursor();
								DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
								OpenMouseCursor();
							}
						}
						else break;
					}
					p2 += 16;
					if(flag>1) break;
				}		
			}
			p1 += 16;
		}
	}
	
	if((mode == 2)||(mode == 3))
	{
		if((mode != 3)&&(mode != 4))
		{
			p1 = position2;
			for(i=0; i<linenum2; i++)
			{	
				abcline1 = (struct Line*)p1;
				if((abcline1->attrib == PCB)&&(abcline1->flag == 1))
				{
					if( (i+1) >= linenum2) break;
					abcline2 = (struct Line*)(p1 + 16);
					if((abcline2->attrib == PCB)&&(abcline2->flag == 1))
					{
						if(abcline2->startx == abcline1->startx)
						{
							oneline.flag = 2;
							oneline.startx = abcline1->startx;
							oneline.endx = abcline1->endx;
							oneline.starty = abcline1->endy;
							oneline.endy = abcline2->starty;
							if(p > trackwidth) 
							{
								DRC_T_T = p - trackwidth;
								DRC_T_P = p - trackwidth;
								if(DRC_T_T < t_t) DRC_T_T = t_t;
								if(DRC_T_P < t_p) DRC_T_P = t_p;
							}
							j = Clearance_Route(&oneline);
							if(j == 0)
							{
								//------------- Undo -----------------
								undoflag = 1;
								Check_Undo_Buffer(20);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_LINE;
								head.p = pDoc->m_pLine_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//-------------------------------------
								pDoc->Save_EMMLine(&oneline);
								CloseMouseCursor();
								DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
								OpenMouseCursor();
							}
						}
					}
				}
				p1 += 16;
			}
		}
	
		p1 = position2;
		for(i=0; i<linenum2; i++)
		{	
			abcline1 = (struct Line*)p1;
			if((abcline1->attrib == PCB)&&(abcline1->flag == 1))
			{
				flag = 0;
				p2 = p1;
				for(ii = i; ii<linenum2; ii++)
				{	
					abcline2 = (struct Line*)p2;
					if((abcline2->attrib == PCB)&&(abcline2->flag == 1))
					{
						if(abcline2->startx == abcline1->startx){}
						else if(abcline2->startx == (abcline1->startx + trackwidth - 1))
						{
							if(abs(abcline2->starty - abcline1->starty) <= p)//trackwidth)
							{
								oneline.flag = 2;
								oneline.startx = abcline1->startx;
								oneline.starty = abcline1->starty;
								oneline.endx = abcline2->startx;
								oneline.endy = abcline2->starty;
								//------------- Undo -----------------
								undoflag = 1;
								Check_Undo_Buffer(20);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_LINE;
								head.p = pDoc->m_pLine_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//-------------------------------------
								pDoc->Save_EMMLine(&oneline);
								CloseMouseCursor();
								DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
								OpenMouseCursor();
							}
							if(abs(abcline2->endy - abcline1->endy) <= p) //trackwidth)
							{
								oneline.flag = 2;
								oneline.startx = abcline1->endx;
								oneline.starty = abcline1->endy;
								oneline.endx = abcline2->endx;
								oneline.endy = abcline2->endy;
								//------------- Undo -----------------
								undoflag = 1;
								Check_Undo_Buffer(20);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_LINE;
								head.p = pDoc->m_pLine_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//-------------------------------------
								pDoc->Save_EMMLine(&oneline);
								CloseMouseCursor();
								DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
								OpenMouseCursor();
							}
						}
						else break;
					}
					p2 += 16;
					if(flag>1) break;
				}		
			}
			p1 += 16;
		}
	}

	//----------- Undo -----------
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------

	//充填完成

	//处理网格充填
	undoflag = 0;
	if(mode == 4)
	{
		p1 = position;
		j = 0;
		for(i=0; i<linenum; i++)
		{	
			abcline1 = (struct Line*)p1;
			if((abcline1->attrib == PCB)&&(abcline1->flag == 1))
			{
				if(abcline1->starty >= y2) break;
				if(j == 0) j = abcline1->starty;
				else if(j == abcline1->starty) {}
				else if(abcline1->starty < (j + fd*(trackwidth-1)))
				{
					abcline1->flag = 0xff;
					//----------- Undo -----------
					undoflag = 1;
					Check_Undo_Buffer(10);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_UNDELETE;
					head.ob = UNDO_LINE;
					head.p = p1;
					memcpy(undo_p,&head,6);
					pDoc->m_pPCB_Undo_Current += 6;
					//---------------------------------------------------------------------------
				}
				else j = abcline1->starty;
			}
			p1 += 16;
		}
		
		p1 = position2;
		j = 0;
		for(i=0; i<linenum2; i++)
		{	
			abcline1 = (struct Line*)p1;
			if((abcline1->attrib == PCB)&&(abcline1->flag == 1))
			{
				if(abcline1->startx >= x2) break;
				if(j == 0) j = abcline1->startx;
				else if(j == abcline1->startx) {}
				else if(abcline1->startx < (j + fd*(trackwidth-1)))
				{
					abcline1->flag = 0xff;
					//----------- Undo -----------
					undoflag = 1;
					Check_Undo_Buffer(10);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_UNDELETE;
					head.ob = UNDO_LINE;
					head.p = p1;
					memcpy(undo_p,&head,6);
					pDoc->m_pPCB_Undo_Current += 6;
					//---------------------------------------------------------------------------
				}
				else j = abcline1->startx;
			}
			p1 += 16;
		}
		Invalidate();
	}

	//----------- Undo -----------
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------
	Block_Line_Recover();


	DrawPCBHighlight(0);
	Free_PCBHighlightbuf();

	//优化充填，自动去除残线
	position2 = pDoc->m_pLine_Buffer_Temp;
	int totallinenum = (position2 - position)/16;
	//char ccc[100];
	//sprintf(ccc, "%d", totallinenum);
	//AfxMessageBox(ccc);

	char *buf, *temp_p1, *temp_p2, *temptrackbuf;
	struct htrack
	{ 
		char* pointer;             } highlighttrack;
	
	undoflag = 0;
	if((optimize != 0)&&(PCBHighlightBufferInit() == 0)&&((buf = (char*)malloc(5000)) != NULL))
	{
		temp_p1 = position;
		j = 0;
		delta = 0;
		for(;;)
		{
			abcline1 = (struct Line*)temp_p1;
			if((abcline1->flag == 0xff)||(abcline1->flag == 0x01))
			{
				temp_p1 += 16;
				j++;
				if((temp_p1 == position2)||(j == totallinenum))  break;
				continue;
			}

			highlighttrack.pointer = temp_p1;
			memcpy(TempTrackbuf,&highlighttrack,4);
			TempTrackbuf += 4;
			CloseMouseCursor();
			PCBMakeHighlight();
			OpenMouseCursor();

			DrawPCBHighlight(0);
			
			temptrackbuf = Highlight_Trackbuf;
			for(i=0;i<((int)(HL_lnum/4));i++)
			{
				memcpy(&highlighttrack,temptrackbuf,4);
				if(highlighttrack.pointer == NULL) break;

				abcline1 = (struct Line*)highlighttrack.pointer;
				if(abcline1->flag == 0) abcline1->flag = 1;
				temptrackbuf+=4;
			}

			if(i < filllimit)
			{
				if(delta == 0)
				{
					memcpy(buf, Highlight_Trackbuf, i*4);
					ii = i;
				}
				else if(delta > i)
				{
					temp_p2 = Highlight_Trackbuf;
					for(int kk=0; kk<i; kk++)
					{
						memcpy(&highlighttrack, temp_p2, 4);
						if(highlighttrack.pointer >= position)
						{
							abcline2 = (struct Line*)highlighttrack.pointer;
							abcline2->flag = 0xff;
							// undo -------------------------
							undoflag = 1;
							Check_Undo_Buffer(10);
							undo_p = pDoc->m_pPCB_Undo_Current;
							head.op = UNDO_UNDELETE;
							head.ob = UNDO_LINE;
							head.p = highlighttrack.pointer;
							memcpy(undo_p,&head,6);
							pDoc->m_pPCB_Undo_Current += 6;
							// ------------------------------
						}
						temp_p2 += 4;
					}
				}
				else
				{
					// 删除buf中记录的线
					temp_p2 = buf;
					for(int kk=0; kk<ii; kk++)
					{
						memcpy(&highlighttrack, temp_p2, 4);
						if(highlighttrack.pointer >= position)
						{
							abcline2 = (struct Line*)highlighttrack.pointer;
							abcline2->flag = 0xff;
							// undo -------------------------
							undoflag = 1;
							Check_Undo_Buffer(10);
							undo_p = pDoc->m_pPCB_Undo_Current;
							head.op = UNDO_UNDELETE;
							head.ob = UNDO_LINE;
							head.p = highlighttrack.pointer;
							memcpy(undo_p,&head,6);
							pDoc->m_pPCB_Undo_Current += 6;
							// ------------------------------
						}
						temp_p2 += 4;
					}

					// 将当前记录复制到buf
					memcpy(buf, Highlight_Trackbuf, i*4);
					ii = i;
				}
			}
			
			if(delta < i) delta = i;

			for(i=0; i<HL_lnum;i++) Highlight_Trackbuf[i]   = 0;
			for(i=0; i<HL_pnum; i++) Highlight_Padbuf[i]     = 0;
			for(i=0; i<HL_afnum; i++) Highlight_ArcFillbuf[i] = 0;
			for(i=0; i<HL_packnum; i++) Highlight_Packagebuf[i] = 0;

			TempTrackbuf = Highlight_Trackbuf;
			TempPadbuf = Highlight_Padbuf;
			TempArcFillbuf = Highlight_ArcFillbuf;
			TempPackagebuf = Highlight_Packagebuf;
			
			temp_p1 += 16;
			j++;
			if((temp_p1 >= position2)||(j >= totallinenum))  break;
		}
	}
	
	//----------- Undo -----------
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------
	Block_Line_Recover();

	free(buf);
	Free_PCBHighlightbuf();

	DrawMyCursor(&dc,&pp);
	DRC_T_T = t_t;
	DRC_T_P = t_p;
	DRC_P_P = p_p;
	CL_Auto_Pan = OFF;
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	Write_Help(commandend);
	EnableFrame(TRUE);
	Invalidate();
}

void CDesignView::OnOptimizeTrack() 
{
	int i,num, flag;
	int ii;
	char *p, *pp;
	struct Line *abcline, *otherline;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	Write_Help("走线优化...");
	// 删除长度为零的走线带
	num = 0;
	p = pDoc->m_pLine_Buffer;
	for(i=0; i<pDoc->m_nLine_Number; i++)
	{	
		abcline = (struct Line*)p;
		if((abcline->flag==0)&&(abcline->attrib==PCB)&&\
			(abcline->startx==abcline->endx)&&(abcline->starty==abcline->endy))
		{
			abcline->flag=0xff;
			num++;
		}
		p+=16;
	}

	// 删除被覆盖的走线带
	p = pDoc->m_pLine_Buffer;
	for(i=0; i<pDoc->m_nLine_Number; i++)
	{	
		abcline = (struct Line*)p;
		if((abcline->flag==0)&&(abcline->attrib==PCB))
		{
			flag=0;
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				otherline = (struct Line*)pp;
				if((otherline->attrib==abcline->attrib)&&\
					(otherline->flag==0)&&(otherline->layer==abcline->layer)&&\
					(p != pp))
				{
					if((otherline->startx==otherline->endx)&&(abcline->startx==abcline->endx)&&(otherline->endx==abcline->endx)&&\
						(MAX(otherline->starty,otherline->endy)>=MAX(abcline->starty,abcline->endy))&&\
						(MIN(otherline->starty,otherline->endy)<=MIN(abcline->starty,abcline->endy))&&\
						(otherline->linewidth>=abcline->linewidth))
					{
						flag=1;
						num++;
					}
					else if((otherline->starty==otherline->endy)&&(abcline->starty==abcline->endy)&&(otherline->endy==abcline->endy)&&\
						(MAX(otherline->startx,otherline->endx)>=MAX(abcline->startx,abcline->endx))&&\
						(MIN(otherline->startx,otherline->endx)<=MIN(abcline->startx,abcline->endx))&&\
						(otherline->linewidth>=abcline->linewidth))
					{
						flag=1;
						num++;
					}
					if(flag==1) 
					{
						break; 
					}
				}
				pp+=16;
			}	
			abcline = (struct Line*)p;
			if(flag==1)
			{
				flag=0;
				abcline->flag=0xff; 
			}
		}
		p+=16;
	}

	// 连接的走线带合二为一
TEMP:
	flag=0;
	p = pDoc->m_pLine_Buffer;
	for(i=0; i<pDoc->m_nLine_Number; i++)
	{	
		abcline = (struct Line*)p;
		if((abcline->flag==0)&&(abcline->attrib==PCB))
		{
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				otherline = (struct Line*)pp;
				if((otherline->attrib==abcline->attrib)&&(otherline->linewidth==abcline->linewidth)&&\
					(otherline->flag==0)&&(otherline->layer==abcline->layer)&&\
					(p != pp))
				{
					if((otherline->startx==otherline->endx)&&(abcline->startx==abcline->endx)&&(otherline->endx==abcline->endx)&&\
						(MAX(otherline->starty,otherline->endy)>=MAX(abcline->starty,abcline->endy))&&\
						(MIN(otherline->starty,otherline->endy)<=MAX(abcline->starty,abcline->endy))&&\
						(MIN(otherline->starty,otherline->endy)>=MIN(abcline->starty,abcline->endy)))
					{
						otherline->starty = MAX(otherline->starty, otherline->endy);
						otherline->endy   = MIN(abcline->starty, abcline->endy);
						abcline->flag = 0xff;
						num++;
						flag = 1;
						break;
					}
					else if((otherline->starty==otherline->endy)&&(abcline->starty==abcline->endy)&&(otherline->endy==abcline->endy)&&\
						(MAX(otherline->startx,otherline->endx)>=MAX(abcline->startx,abcline->endx))&&\
						(MIN(otherline->startx,otherline->endx)<=MAX(abcline->startx,abcline->endx))&&\
						(MIN(otherline->startx,otherline->endx)>=MIN(abcline->startx,abcline->endx)))
					{
						otherline->startx = MAX(otherline->startx, otherline->endx);
						otherline->endx   = MIN(abcline->startx, abcline->endx);
						abcline->flag = 0xff;
						num++;
						flag = 1;
						break;
					}
				}
				pp+=16;
			}
			/*if(flag == 1)
			{
				i = -1;
				p = pDoc->m_pLine_Buffer;
				continue;
			}*/
		}
		p+=16;
	}
	if(flag == 1)
	{
		goto TEMP;
	}

	// 删除小的皱折走线带

	// 走线带自动导45度角

	char str[20],string[50];
	itoa(num,str,10);
	sprintf(string,"%s%s",str,"走线被移除.");
	AfxMessageBox(string, MB_OK|MB_ICONINFORMATION);
	Write_Help(commandend);
}

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
					//----------- Undo -----------
					flag = 1;
					Check_Undo_Buffer(10);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_UNDELETE;
					head.ob = UNDO_LINE;
					head.p = pointer;
					memcpy(undo_p,&head,6);
					pDoc->m_pPCB_Undo_Current += 6;
					//----------------------------------
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
					//----------- Undo -----------
					flag = 1;
					Check_Undo_Buffer(10);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_UNDELETE;
					head.ob = UNDO_JUNCTION;
					head.p = pointer;
					memcpy(undo_p,&head,6);
					pDoc->m_pPCB_Undo_Current += 6;
					//----------------------------------------
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
						//----------- Undo -----------
						flag = 1;
						Check_Undo_Buffer(10);
						undo_p = pDoc->m_pPCB_Undo_Current;
						head.op = UNDO_UNDELETE;
						head.ob = UNDO_ARC;
						head.p = pointer;
						memcpy(undo_p,&head,6);
						pDoc->m_pPCB_Undo_Current += 6;
						//------------------------------------
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
						//----------- Undo -----------
						flag = 1;
						Check_Undo_Buffer(10);
						undo_p = pDoc->m_pPCB_Undo_Current;
						head.op = UNDO_UNDELETE;
						head.ob = UNDO_FILL;
						head.p = pointer;
						memcpy(undo_p,&head,6);
						pDoc->m_pPCB_Undo_Current += 6;
						//-----------------------------------------
					}
				}
				temparcfillbuf+=4;
			}
			if(flag != 0)
			{
				//----------- Undo -----------
				pDoc->m_nPCB_Undo_Num++;
				if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
				pDoc->m_nPCB_Redo_Num=0;
				pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
				//---------------------------------------------------------------------------
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
   
	//if(SortString(str,select_net,64/maxlen,nets,Net_Number)==ESC) { free(nets); return; }
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
			//----------- Undo -----------
			flag = 1;
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_UNDELETE;
			head.ob = UNDO_LINE;
			head.p = pointer;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//----------------------------------
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
			//----------- Undo -----------
			flag = 1;
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_UNDELETE;
			head.ob = UNDO_JUNCTION;
			head.p = pointer;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//----------------------------------------
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
				//----------- Undo -----------
				flag = 1;
				Check_Undo_Buffer(10);
				undo_p = pDoc->m_pPCB_Undo_Current;
				head.op = UNDO_UNDELETE;
				head.ob = UNDO_ARC;
				head.p = pointer;
				memcpy(undo_p,&head,6);
				pDoc->m_pPCB_Undo_Current += 6;
				//------------------------------------
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
				//----------- Undo -----------
				flag = 1;
				Check_Undo_Buffer(10);
				undo_p = pDoc->m_pPCB_Undo_Current;
				head.op = UNDO_UNDELETE;
				head.ob = UNDO_FILL;
				head.p = pointer;
				memcpy(undo_p,&head,6);
				pDoc->m_pPCB_Undo_Current += 6;
				//-----------------------------------------
			}
		}
		temparcfillbuf+=4;
	}
	if(flag != 0)
	{
		//----------- Undo -----------
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
		//---------------------------------------------------------------------------
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
	//i = AfxMessageBox("All the tracks will be deleted, please confirm.", MB_YESNO|MB_ICONQUESTION);
	i = AfxMessageBox("所有走线将被删除，确认?", MB_YESNO|MB_ICONQUESTION);
	if(i != IDYES)	return;

	pDoc->SetModifiedFlag(TRUE);


	flag = 0;

	// line 
	p = pDoc->m_pLine_Buffer;
	for(i=0; i<pDoc->m_nLine_Number; i++)
	{	
		memcpy(&gnline,p,16);
		if((gnline.attrib==PCB)&&(gnline.flag==0)&&(gnline.layer<=BOTTOMLAYER))
		{
			gnline.flag = 0xff;
			memcpy(p,&gnline,16);
			//----------- Undo -----------
			flag = 1;
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_UNDELETE;
			head.ob = UNDO_LINE;
			head.p = p;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//----------------------------------
		}
		p+=16;
	}


	// junction 
 	p = pDoc->m_pJunction_Buffer;
	for(i=0; i<pDoc->m_nJunction_Number; i++)
	{	
		memcpy(&gnjunction,p,16);
		if((gnjunction.attrib==PCB)&&(gnjunction.flag==0))
		{
			gnjunction.flag = 0xff;
			memcpy(p,&gnjunction,16);
			//----------- Undo -----------
			flag = 1;
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_UNDELETE;
			head.ob = UNDO_JUNCTION;
			head.p = p;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//----------------------------------------
		}
		p+=16;
	}


	// arc
 	p = pDoc->m_pArc_Buffer;
	for(i=0; i<pDoc->m_nArc_Number; i++)
	{	
		memcpy(&gnarc,p,16);
		if((gnarc.attrib==PCB)&&(gnarc.flag==0)&&(gnarc.layer<=BOTTOMLAYER))
		{
			gnarc.flag=0xff;
			memcpy(p,&gnarc,16);
			//----------- Undo -----------
			flag = 1;
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_UNDELETE;
			head.ob = UNDO_ARC;
			head.p = p;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//------------------------------------
		}
		p+=16;
	}
	// fill
	p = pDoc->m_pFill_Buffer;
	for(i=0; i<pDoc->m_nFill_Number; i++)
	{	
		memcpy(&gnfill,p,16);
		if((gnfill.attrib==PCB)&&(gnfill.flag==0)&&(gnfill.layer<=BOTTOMLAYER))
		{
			gnfill.flag=0xff;
			memcpy(p,&gnfill,16);
			//----------- Undo -----------
			flag = 1;
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_UNDELETE;
			head.ob = UNDO_FILL;
			head.p = p;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//-----------------------------------------
		}
		p += gnfill.total16byte*16;
	}
	
	if(flag != 0)
	{
		//----------- Undo -----------
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
		//---------------------------------------------------------------------------
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
	/*itoa(ccdeltax,str,10);
	if(ASCII_Input(str,"desity[输入布局密度]",Null_Str,4)!=ESC)
	{
		ccdeltax = atoi(str);
		if(ccdeltax<50) ccdeltax=50;
		ccdeltay = ccdeltax;
	}*/

	CL_Auto_Pan = OFF;
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);

	pDoc->SetModifiedFlag(TRUE);

	// move "Free to Move" Packages to x=12345; y=12345; 
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
			//----------- Undo -----------
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
			//----------- Undo -----------
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
			//----------- Undo -----------
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
			//----------- Undo -----------
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
			//---------------------------------------------------------------------------
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
	//----------- Undo -----------
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------

	Invalidate();
	SendMessage(WM_PAINT);

	//OnDraw(&dc);
	// Redraw();

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
	//Arc_precbak = CL_Arc_precision;
	//CL_Arc_precision=90;

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
		movepackage.flag = 0xcc;  // temp value for dynamic ratsnest 
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
			// interupt placement 
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
		    // avoid cycle 
			if(ch1==VK_RETURN) ch1=VK_ESCAPE;

		    // judge ratsnest's length 
			else if((ch1==0)||(ch1==1))
			{
			   // compare ratsnest length 
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

				// adjust component's X position 
				// ccx+=ccdeltax;
				// adjust component's Y position 
				//if((placementstate==2)||(increasenum>3))
			     //{  ccx=ccx0;
					//ccy+=ccdeltay;
					//increasenum=0;
					//}

				// Place or Move component 
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
					// adjust component's X position 
					ccx+=ccdeltax;

				    // adjust component's Y position 
				    if((placementstate==2)||(increasenum>3))
					{
						ccx=ccx0;
						ccy+=ccdeltay;
						increasenum=0;
					}

				    // move to X,Y position 
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
			    if((flag!=0)&&(Ratsnestflag!=0)) Show_Ratsnest(&dc);

			    temp = packagebak;
				memcpy(pointer,temp,movepackage.total16byte*16);
				DrawPackage(pointer,&dc);
				if(Ratsnestflag!=0) 
				{
					Show_Ratsnest(&dc); 
					flag = 1;
				}
				//OpenMouseCursor();
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
				    //gnarc.flag=0;
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
				//----------- Undo -----------
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
				//----------- Undo -----------
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
				//----------- Undo -----------
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
				//----------- Undo -----------
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
				//---------------------------------------------------------------------------
				memcpy(pointer,temp,movepackage.total16byte*16);
				//if(Ratsnestflag!=0) Show_Ratsnest();
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

	//----------- Undo -----------
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------
	
	//CL_Arc_precision = Arc_precbak;
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
				// interupt placement 
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
				
				// avoid cycle 
				if(ch1 == VK_RETURN) ch1 = VK_ESCAPE;

				// judge ratsnest's length
				else if((ch1==0)||(ch1==1))
				{
					// compare ratsnest length 
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

					// Place or Move component 
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
						// adjust component's X position 
						ccx+=ccdeltax;

						// adjust component's Y position 
						if((placementstate==2)||(increasenum>3))
						{
							ccx=ccx0;
							ccy+=ccdeltay;
							increasenum=0;
						}

						// move to X,Y position 
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
					//----------- Undo -----------
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
					//----------- Undo -----------
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
					//----------- Undo -----------
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
					//----------- Undo -----------
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
					//---------------------------------------------------------------------------
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
	//----------- Undo -----------
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------

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
		    movepackage.flag = 0xcc;  // temp value for dynamic ratsnest 
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
				// interupt placement 
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
				// avoid cycle 
				if(ch1 == VK_RETURN) ch1=VK_ESCAPE;

			    // judge ratsnest's length 
			    else if((ch1==0)||(ch1==1))
				{
					// compare ratsnest length 
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

					// Place or Move component 
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
						// move to X,Y position 
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
						movepackage.flag=0xcc;  // temp value for dynamic ratsnest 
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
					//----------- Undo -----------
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
					//----------- Undo -----------
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
					//----------- Undo -----------
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
					//----------- Undo -----------
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
					//---------------------------------------------------------------------------
	
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
	//----------- Undo -----------
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------

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
			movepackage.flag = 0xcc;  // temp value for dynamic ratsnest 
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
				// interupt placement 
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

				// avoid cycle 
				if(ch1 == VK_RETURN) ch1 = VK_ESCAPE;

				// judge ratsnest's length 
				else if((ch1==0)||(ch1==1))
				{
					// compare ratsnest length 
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
				
					// Place or Move component 
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
						// move to X,Y position 
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
					//----------- Undo -----------
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
					//----------- Undo -----------
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
					//----------- Undo -----------
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
					//----------- Undo -----------
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
					//---------------------------------------------------------------------------
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
	//----------- Undo -----------
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------

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


// invalidflag = 0 OK!
// invalidflag = 1 placement invalid.
// invalidflag = 2 below bottom yline; switch line.
// invalidflag = 3 above top yline; sould be stoped. 


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
		//logical_page=pcb_library[i].pagenum;
	    //if(map_EMM_pages(EMM_Handle,physical_page,logical_page)==0) Exit(-6,0);
	    //offset=pcb_library[i].offset;
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

void CDesignView::OnOptimizeViaminimize() 
{
	int i,ii,totalnum,flag;
	int invalidflag,l,ll,invalidflag1,invalidflag2;
	char *p,*pp, *pointer, str[10], string[30];
	struct Junction *abcjunc, onejunc;
	struct Line *abcline;
	struct Package *abcpackage;
	char *trackbuf1, *trackbuf2, *temptrackbuf1, *temptrackbuf2, *temp;
	unsigned buf1num, buf2num, buf1layer, buf2layer;
	struct swaptrack
	{
		char* pointer;
		struct Line line; } swaptrack, swap;
	struct htrack
	{
		char* pointer;             } highlighttrack;
	char *hstr1 =  "过孔优化...    ";
	struct Undo_Head head;
	char *undo_p;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	if((trackbuf1 = (char*)malloc(10000))==NULL)
	{
		memory_error();
		return;
	}
	if((trackbuf2 = (char*)malloc(10000))==NULL)
	{
		memory_error();
		return;
	}
	temptrackbuf1 = trackbuf1;
	temptrackbuf2 = trackbuf2;
	buf1num = buf2num = 0;
	buf1layer = buf2layer = 100;
	totalnum = 0;
	Write_Help(hstr1);

	invalidflag = 0;
	flag = 0;
	p = pDoc->m_pJunction_Buffer;
	for(i=0; i<pDoc->m_nJunction_Number; i++)
	{	
		abcjunc = (struct Junction*)p;
		pointer = p;
		memcpy(&onejunc,pointer,16);
		if((abcjunc->attrib==PCB)&&(abcjunc->layer==MULTILAYER)&&\
			(abcjunc->flag==0)&&(abcjunc->id==0x22)&&\
			(((abcjunc->padtype&0x38)>>3)==0))
		{
			// search for related tracks 
			temptrackbuf1 = trackbuf1;
			temptrackbuf2 = trackbuf2;
			buf1num = buf2num = 0;
			buf1layer = buf2layer = 100;
			invalidflag = 0;
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				abcline = (struct Line*)pp;
				if((abcline->attrib==PCB)&&(abcline->layer<=BOTTOMLAYER)&&\
					(abcline->flag==0)&&(Line_Pad_Connect(abcline,&onejunc,0)!=0))
				{
					if(abcline->layer==buf1layer)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf1,&swaptrack,20);
						temptrackbuf1+=20;
						buf1num++;
					}
					else if(abcline->layer==buf2layer)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf2,&swaptrack,20);
						temptrackbuf2+=20;
						buf2num++;
					}
					else if(buf1layer==100)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf1,&swaptrack,20);
						temptrackbuf1+=20;
						buf1layer=abcline->layer;
						buf1num++;
					}
					else if(buf2layer==100)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf2,&swaptrack,20);
						temptrackbuf2+=20;
						buf2layer=abcline->layer;
						buf2num++;
					}
					else invalidflag=1;
				}
				pp += 16;
			}
			if((buf1layer==100)||(buf2layer==100)) invalidflag = 1;
			if(invalidflag!=0) goto NEXT_VIA;
			// process track's in buf1 
			invalidflag1 = 0;
			temptrackbuf1 = trackbuf1;
			for(l=0;l<buf1num;l++)
			{
				memcpy(&swaptrack,temptrackbuf1,20);
				memcpy(&gnline,&swaptrack.line,16);
				if(invalidflag1==0)
				{
					pp = pDoc->m_pLine_Buffer;
					for(ii=0; ii<pDoc->m_nLine_Number; ii++)
					{
						abcline = (struct Line*)pp;
						if((abcline->attrib==PCB)&&(abcline->layer==gnline.layer)&&\
							(abcline->flag==0)&&(Line_Line_Connect(abcline,&gnline,0)!=0))
						{
							temp = trackbuf1;
							invalidflag = 1;
							for(ll=0;ll<buf1num;ll++)
							{
								memcpy(&swap,temp,20);
								if(swap.pointer==pp)
								{
									invalidflag = 0;
									break;
								}
								temp += 20;
							}
							if(invalidflag != 0)
							{
								swaptrack.pointer = pp;
								memcpy(&swaptrack.line,abcline,16);
								memcpy(trackbuf1+buf1num*20,&swaptrack,20);
								buf1num++;
							}
						}
						pp+=16;
					}
				}
				// to see if connect free SMD pad or not 
				if(invalidflag1==0)
				{
					pp = pDoc->m_pJunction_Buffer;
					for(ii=0; ii<pDoc->m_nJunction_Number; ii++)
					{	
						abcjunc = (struct Junction*)pp;
						if((abcjunc->attrib==PCB)&&(abcjunc->layer==gnline.layer)&&\
							(abcjunc->flag==0)&&(Line_Pad_Connect(&gnline,abcjunc,0)!=0))
						{
							invalidflag1 = 1;
							break;
						}
						pp+=16;
					}
				}

			   // to see if connect SMD Package's PAD or not 
				if(invalidflag1==0)
				{
					pp = pDoc->m_pPackage_Buffer;
					for(ii=0; ii<pDoc->m_nPackage_Number; ii++)
					{
						abcpackage = (struct Package*)pp;
						if(abcpackage->flag==0)
						{
							temp = pp;
							temp += 112;
							for(ll=0;ll<abcpackage->padnum;ll++)
							{
								abcjunc = (struct Junction*)temp;
								if((abcjunc->layer==gnline.layer)&&\
									(Line_Pad_Connect(&gnline,abcjunc,0)!=0))
								{
									invalidflag1=1;
									break;
								}
								temp+=16;
							}
						}
						pp += abcpackage->total16byte*16;

					}
				}
				else break;
				temptrackbuf1 += 20;
			}
			
			if(invalidflag1!=0) goto NEXT_VIA;
			//  swap tracks layer to check clearance 
			if(PCBHighlightBufferInit() != 0) return;
			memcpy(&swap,trackbuf1,20);
			memcpy(&gnline,&swap.line,16);
			highlighttrack.pointer = swap.pointer;
			memcpy(TempTrackbuf,&highlighttrack,4);
			TempTrackbuf += 4;
			PCBMakeHighlight();
			temptrackbuf1 = trackbuf1;
			for(l=0;l<buf1num;l++)
			{
				memcpy(&swaptrack,temptrackbuf1,20);
				temptrackbuf1+=20;
				swaptrack.line.layer=buf2layer;
				if(Clearance_Route(&swaptrack.line)!=0)
				{
					invalidflag1=1;
					break;
				}
			}
			
			DrawPCBHighlight(0);
			// process via and related tracks 
			if(invalidflag1==0)
			{
				abcjunc = (struct Junction*)p;
				CloseMouseCursor();
				DrawStructJP(abcjunc,BK_Color,0,&dc);
				OpenMouseCursor();
				abcjunc->flag = 0xff;
				//----------- Undo -----------
				flag = 1;
				Check_Undo_Buffer(10);
				undo_p = pDoc->m_pPCB_Undo_Current;
				head.op = UNDO_UNDELETE;
				head.ob = UNDO_JUNCTION;
				head.p = p;
				memcpy(undo_p,&head,6);
				pDoc->m_pPCB_Undo_Current += 6;
				//---------------------------------------------------------------------------
				temptrackbuf1 = trackbuf1;
				totalnum++;
				itoa(totalnum,str,10);
				sprintf(string,"%s%s",hstr1,str);
				Write_Help(string);
				for(l=0;l<buf1num;l++)
				{
					memcpy(&swap,temptrackbuf1,20);
					temptrackbuf1+=20;
					abcline = (struct Line*)swap.pointer;
					//----------- Undo -----------
					flag = 1;
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_CHANGE;
					head.ob = UNDO_LINE;
					head.p = swap.pointer;
					head.dx = abcline->linewidth;
					head.dy = abcline->linewidth;
					head.state1 = abcline->layer;
					head.state2 = buf2layer;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					//---------------------------------------------------------------------------
					abcline->layer = buf2layer;
					CloseMouseCursor();
					DrawStructLine(abcline,PCB_LayerColor[abcline->layer],&dc);
					OpenMouseCursor();
				}
				goto NEXT_VIA;
			}
			
			// process track's in buf2 
			invalidflag2=0;
			temptrackbuf2=trackbuf2;
			for(l=0;l<buf2num;l++)
			{
				memcpy(&swaptrack,temptrackbuf2,20);
				memcpy(&gnline,&swaptrack.line,16);
				if(invalidflag2==0)
				{
					pp = pDoc->m_pLine_Buffer;
					for(ii=0; ii<pDoc->m_nLine_Number; ii++)
					{	
						abcline = (struct Line*)pp;
						if((abcline->attrib==PCB)&&(abcline->layer==gnline.layer)&&\
							(abcline->flag==0)&&(Line_Line_Connect(abcline,&gnline,0)!=0))
						{
							temp = trackbuf2;
							invalidflag = 1;
							for(ll=0;ll<buf2num;ll++)
							{
								memcpy(&swap,temp,20);
								if(swap.pointer == pp)
								{
									invalidflag=0;
									break;
								}
								temp+=20;
							}
							if(invalidflag!=0)
							{
								swaptrack.pointer = pp;
								memcpy(&swaptrack.line,abcline,16);
								memcpy(trackbuf2+buf2num*20,&swaptrack,20);
								buf2num++;
							}
						}
						pp+=16;
					}	
				}

				// to see if connect free SMD pad or not 
				if(invalidflag2==0)
				{
					pp = pDoc->m_pJunction_Buffer;
					for(ii=0; ii<pDoc->m_nJunction_Number; ii++)
					{	
						abcjunc = (struct Junction*)pp;
						if((abcjunc->attrib==PCB)&&(abcjunc->layer==gnline.layer)&&\
							(abcjunc->flag==0)&&(Line_Pad_Connect(&gnline,abcjunc,0)!=0))
						{
							invalidflag2=1;
							break;
						}
						pp+=16;
					}
				}
				
				// to see if connect SMD Package's PAD or not 
				if(invalidflag2==0)
				{
					pp = pDoc->m_pPackage_Buffer;
					for(ii=0; ii<pDoc->m_nPackage_Number; ii++)
					{
						abcpackage = (struct Package*)pp;
						if(abcpackage->flag==0)
						{
							temp = pp;
							temp+=112;
							for(ll=0;ll<abcpackage->padnum;ll++)
							{
								abcjunc = (struct Junction*)temp;
								if((abcjunc->layer==gnline.layer)&&\
									(Line_Pad_Connect(&gnline,abcjunc,0)!=0))
								{
									invalidflag2=1;
									break;
								}
								temp+=16;
							}
						}
						pp += abcpackage->total16byte*16;
					}	
				}
				else break;
				temptrackbuf2 += 20;
			}
			
			if(invalidflag2!=0) goto NEXT_VIA;
			//  swap tracks layer to check clearance 
			if(PCBHighlightBufferInit()!=0) return;
			memcpy(&swap,trackbuf2,20);
			memcpy(&gnline,&swap.line,16);
			highlighttrack.pointer = swap.pointer;
			memcpy(TempTrackbuf,&highlighttrack,4);
			TempTrackbuf+=4;
			PCBMakeHighlight();
			temptrackbuf2=trackbuf2;
			for(l=0;l<buf2num;l++)
			{
				memcpy(&swaptrack,temptrackbuf2,20);
				temptrackbuf2+=20;
				swaptrack.line.layer=buf1layer;
				if(Clearance_Route(&swaptrack.line)!=0)
				{
					invalidflag2=1;
					break;
				}
			}
			
			DrawPCBHighlight(0);
			// process via and related tracks 
			if(invalidflag2==0)
			{
				abcjunc = (struct Junction*)p;
				CloseMouseCursor();
				DrawStructJP(abcjunc,BK_Color,0,&dc);
				OpenMouseCursor();
				abcjunc->flag=0xff;
				//----------- Undo -----------
				flag = 1;
				Check_Undo_Buffer(10);
				undo_p = pDoc->m_pPCB_Undo_Current;
				head.op = UNDO_UNDELETE;
				head.ob = UNDO_JUNCTION;
				head.p = p;
				memcpy(undo_p,&head,6);
				pDoc->m_pPCB_Undo_Current += 6;
				//---------------------------------------------------------------------------
				temptrackbuf2=trackbuf2;
				totalnum++;
				itoa(totalnum,str,10);
				sprintf(string,"%s%s",hstr1,str);
				Write_Help(string);
				for(l=0;l<buf2num;l++)
				{
					memcpy(&swap,temptrackbuf2,20);
					temptrackbuf2+=20;
					abcline = (struct Line*)swap.pointer;
					//----------- Undo -----------
					flag = 1;
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_CHANGE;
					head.ob = UNDO_LINE;
					head.p = swap.pointer;
					head.dx = abcline->linewidth;
					head.dy = abcline->linewidth;
					head.state1 = abcline->layer;
					head.state2 = buf1layer;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					//---------------------------------------------------------------------------
					abcline->layer=buf1layer;
					CloseMouseCursor();
					DrawStructLine(abcline,PCB_LayerColor[abcline->layer],&dc);
					OpenMouseCursor();
				}
				goto NEXT_VIA;
			}
NEXT_VIA:
			ll++;
		}
		p+=16;
	}

	p = pDoc->m_pJunction_Buffer;
	for(i=0; i<pDoc->m_nJunction_Number; i++)
	{	
		abcjunc = (struct Junction*)p;
		pointer = p;
		memcpy(&onejunc,pointer,16);
		if((abcjunc->attrib==PCB)&&(abcjunc->layer==MULTILAYER)&&\
			(abcjunc->flag==0)&&(abcjunc->id==0x22)&&\
			(((abcjunc->padtype&0x38)>>3)==0))
		{
			// search for related tracks 
			temptrackbuf1=trackbuf1;
			temptrackbuf2=trackbuf2;
			buf1num=buf2num=0;
			buf1layer=buf2layer=100;
			invalidflag=0;
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				abcline = (struct Line*)pp;
				if((abcline->attrib==PCB)&&(abcline->layer<=BOTTOMLAYER)&&\
					(abcline->flag==0)&&(Line_Pad_Connect(abcline,&onejunc,0)!=0))
				{
					if(abcline->layer==buf1layer)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf1,&swaptrack,20);
						temptrackbuf1+=20;
						buf1num++;
					}
					else if(abcline->layer==buf2layer)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf2,&swaptrack,20);
						temptrackbuf2+=20;
						buf2num++;
					}
					else if(buf1layer==100)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf1,&swaptrack,20);
						temptrackbuf1+=20;
						buf1layer=abcline->layer;
						buf1num++;
					}
					else if(buf2layer==100)
					{
						swaptrack.pointer = pp;
						memcpy(&swaptrack.line,abcline,16);
						memcpy(temptrackbuf2,&swaptrack,20);
						temptrackbuf2+=20;
						buf2layer=abcline->layer;
						buf2num++;
					}
					else invalidflag=1;
				}
				pp+=16;
			}
			
			if((buf1layer==100)||(buf2layer==100))
			{
				abcjunc = (struct Junction*)p;
				CloseMouseCursor();
				DrawStructJP(abcjunc,BK_Color,0,&dc);
				OpenMouseCursor();
				abcjunc->flag = 0xff;
				//----------- Undo -----------
				flag = 1;
				Check_Undo_Buffer(10);
				undo_p = pDoc->m_pPCB_Undo_Current;
				head.op = UNDO_UNDELETE;
				head.ob = UNDO_JUNCTION;
				head.p = p;
				memcpy(undo_p,&head,6);
				pDoc->m_pPCB_Undo_Current += 6;
				//---------------------------------------------------------------------------
				totalnum++;
				itoa(totalnum,str,10);
				sprintf(string,"%s%s",hstr1,str);
				Write_Help(string);
			}
		}
		p+=16;
	}
	//----------- Undo -----------
	if(flag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------
	free(trackbuf1);
	free(trackbuf2);
	Free_PCBHighlightbuf();
	if(Via_Showmode==0)
	{
		itoa(totalnum,str,10);
		sprintf(string,"%s%s",str,"过孔已经移除");
		AfxMessageBox(string, MB_OK|MB_ICONINFORMATION);
	}
	Write_Help(commandend);
}

void CDesignView::OnOptimizeSqueeze()
{
	int i,j,k,ii,num,l,ll,flag;
	unsigned ch1;
	unsigned x1,y1;
	int deltax,deltay;
	long int li;
	char *pp, *temp, *pointer, *ttemp;
	char *movebuf, *newbuf, *tempnewbuf, *tempbuf;
	struct sq
    {
		char* pointer;
		short int type; } sq, *sq1, sq2;
	struct Junction *abcjunc, onejunc;
	struct Line *abcline, oneline;
	struct Package *abcpackage, *onepackage;
	struct String *abcstring;
	struct Arc *abcarc;
	struct Fill *abcfill;
	char *hstr1="推挤...";
	char *hstr2="选择走线或者元件";
	int direcpoly[16] = { -12,2, -24,2, 0,-14, 24,2, 12,2, 12,14, -12,14, -12,2 };
	POINT point[8];
	
	int undoflag;
	struct Undo_Head head;
	char *undo_p;
	
	CClientDC dc(this);
	int drawmode = dc.GetROP2();
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	if((newbuf = (char*)malloc(600000))==NULL)
	{
		memory_error();
		return;
	}


	flag = 0;
SBEGIN:
	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(hstr2);
	//setwritemode(1);
	//setcolor(Overwrite_Color);
	Get_VPort_XY(&j,&k,m_nCurrent_X,m_nCurrent_Y);
	if(flag==0)
    {
		for(i=0;i<8;i++)
		{
			point[i].x = direcpoly[2*i]+j;
			point[i].y = direcpoly[2*i+1]+k;
		}
    }
	else if(flag==1)
    {
		for(i=0;i<8;i++)
		{
			point[i].x = direcpoly[2*i+1] + j;
			point[i].y = direcpoly[2*i] + k;
		}
    }
	else if(flag==2)
    {
		for(i=0;i<8;i++)
		{
			point[i].x = j - direcpoly[2*i];
			point[i].y = k - direcpoly[2*i+1];
		}
    }
	else if(flag==3)
    {
		for(i=0;i<8;i++)
		{
			point[i].x = j - direcpoly[2*i+1];
			point[i].y = k - direcpoly[2*i];
		}
    }
	
	CloseMouseCursor();
	//setwritemode(1);
	//setcolor(Overwrite_Color);
	//drawpoly(8,ppoly);
	dc.SetROP2(R2_NOTXORPEN);
	dc.Polyline( point, 8);
	dc.SetROP2(drawmode);
	OpenMouseCursor();
	CL_RedrawFlag = 0;
	do
    {
		ch1 = MouseKey();
		x1 = m_nCurrent_X;
		y1 = m_nCurrent_Y;

		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{
			CloseMouseCursor();
			dc.SetROP2(R2_NOTXORPEN);
			dc.Polyline( point, 8);
			dc.SetROP2(drawmode);
			OpenMouseCursor();
			break;
		}
		else if(ch1==MOUSEMOVE)
		{
TEMP:
			if(CL_RedrawFlag==1) { CL_RedrawFlag=0; }
			else 
			{
				CloseMouseCursor();
				dc.SetROP2(R2_NOTXORPEN);
				dc.Polyline( point, 8);
				dc.SetROP2(drawmode);
				OpenMouseCursor();
			}
		
			Get_VPort_XY(&j,&k,m_nCurrent_X,m_nCurrent_Y);
			if(flag==0)
			{
				for(i=0;i<8;i++)
				{
					point[i].x = direcpoly[2*i]+j;
					point[i].y = direcpoly[2*i+1]+k;
				}
			}
			else if(flag==1)
			{
				for(i=0;i<8;i++)
				{
					point[i].x = direcpoly[2*i+1] + j;
					point[i].y = direcpoly[2*i] + k;
				}
			}
			else if(flag==2)
			{
				for(i=0;i<8;i++)
				{
					point[i].x = j - direcpoly[2*i];
					point[i].y = k - direcpoly[2*i+1];
				}
			}
			else if(flag==3)
			{
				for(i=0;i<8;i++)
				{
					point[i].x = j - direcpoly[2*i+1];
					point[i].y = k - direcpoly[2*i];
				}
			}
			CloseMouseCursor();
			dc.SetROP2(R2_NOTXORPEN);
			dc.Polyline( point, 8);
			dc.SetROP2(drawmode);
			OpenMouseCursor();
		}
		else if((ch1==VK_RETURN)||(ch1==PUSHMOUSE))
		{
			if(flag==0)
			{
				deltax=0;
				deltay=CL_Snap_Grid;
				if(deltay>25) deltay=25;
			}
			else if(flag==1)
			{
				deltay=0;
				deltax=-CL_Snap_Grid;
				if(deltax<-25) deltax=-25;
			}
			else if(flag==2)
			{
				deltax=0;
				deltay=-CL_Snap_Grid;
				if(deltay<-25) deltay=-25;
			}
			else if(flag==3)
			{
				deltay=0;
				deltax=CL_Snap_Grid;
				if(deltax>25) deltax=25;
			}

			if(Select_Line(x1,y1,&pointer)!=0)
			{
				sq.type=EMMLINE;
				CloseMouseCursor();
				dc.SetROP2(R2_NOTXORPEN);
				dc.Polyline( point, 8);
				dc.SetROP2(drawmode);
				OpenMouseCursor();
				break;
			}
			else if(Select_PickPackage(x1,y1,&pointer)!=0)
			{
				sq.type=EMMPACKAGE;
				CloseMouseCursor();
				dc.SetROP2(R2_NOTXORPEN);
				dc.Polyline( point, 8);
				dc.SetROP2(drawmode);
				OpenMouseCursor();
				break;
			}
		}
		else if(ch1==SPACE)
		{
			flag++;
			if(flag>=4) flag=0;
			goto TEMP;
		}
		else  Command_In_Function(ch1);
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));

	if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
	{
		free(newbuf);
		CL_Auto_Pan = OFF;
		DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
		Write_Help(commandend);
		return;
	}

	undoflag = 0;
	for(li=0;li<600000;li++) newbuf[li] = 0;

	tempnewbuf=newbuf;
	sq.pointer = pointer;
	memcpy(tempnewbuf,&sq,6);
	tempnewbuf+=6;

	CloseMouseCursor();
	Write_Help(hstr1);
	OpenMouseCursor();
	movebuf=newbuf;
	for(;;)
	{
		memcpy(&sq,movebuf,6);
		movebuf+=6;
		if(sq.pointer == NULL) break;
		if((sq.type&0x000f)==EMMLINE)
		{
			temp = sq.pointer;
			memcpy(&oneline,temp,16);
			if(((deltay==0)&&(oneline.starty==oneline.endy))||\
				((deltax==0)&&(oneline.startx==oneline.endx)))  { continue; }
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				abcline = (struct Line*)pp;
				if((abcline->attrib==PCB)&&(abcline->layer==oneline.layer)&&\
					(abcline->flag==0)&&(Line_Line_Connect(abcline,&oneline,0)!=0))
				{
					tempbuf = newbuf;
					for(;;)
					{
						sq1 = (struct sq*)tempbuf;
						if(sq1->pointer == pp)
						{
							if((deltay==0)&&(abcline->starty==abcline->endy))
							{
								if(Is_On_Line(abcline->startx,abcline->starty,&oneline)!=0)
								{
									sq1->type=sq1->type|0x0010;
								}
								else if(Is_On_Line(abcline->endx,abcline->endy,&oneline)!=0)
								{
									sq1->type=sq1->type|0x0020;
								}
							}
							else if((deltax==0)&&(abcline->startx==abcline->endx))
							{
								if(Is_On_Line(abcline->startx,abcline->starty,&oneline)!=0)
								{
									sq1->type=sq1->type|0x0010;
								}
								else if(Is_On_Line(abcline->endx,abcline->endy,&oneline)!=0)
							    {
									sq1->type=sq1->type|0x0020;
							    }
							}
							break;
						}
						else if(sq1->pointer == NULL)
						{
							break;
						}
						tempbuf+=6;
					}
					if(sq1->pointer == NULL)
					{
						sq2.pointer = pp;
						sq2.type = EMMLINE;
						if((deltay==0)&&(abcline->starty==abcline->endy))
						{
							if(Is_On_Line(abcline->startx,abcline->starty,&oneline)!=0)
							{
								sq2.type=sq2.type|0x0010;
							}
							else if(Is_On_Line(abcline->endx,abcline->endy,&oneline)!=0)
							{
								sq2.type=sq2.type|0x0020;
							}
						}
						else if((deltax==0)&&(abcline->startx==abcline->endx))
						{
							if(Is_On_Line(abcline->startx,abcline->starty,&oneline)!=0)
							{
								sq2.type=sq2.type|0x0010;
							}
							else if(Is_On_Line(abcline->endx,abcline->endy,&oneline)!=0)
							{
								sq2.type=sq2.type|0x0020;
							}
						}
						memcpy(tempnewbuf,&sq2,6);
						tempnewbuf+=6;
					}
				}
				pp+=16;
			}

			// to see if connect free SMD pad or not 
			pp = pDoc->m_pJunction_Buffer;
			for(ii=0; ii<pDoc->m_nJunction_Number; ii++)
			{	
				abcjunc = (struct Junction*)pp;
				if((abcjunc->attrib==PCB)&&(abcjunc->flag==0)&&\
					(Line_Pad_Connect(&oneline,abcjunc,0)!=0))
				{
					tempbuf = newbuf;
					for(;;)
					{
						sq1 = (struct sq*)tempbuf;
						if(sq1->pointer == pp)
						{
							break;
						}
						else if(sq1->pointer == NULL)
						{
							break;
						}
						tempbuf+=6;
					}
					if(sq1->pointer == NULL)
					{
						sq2.pointer = pp;
						sq2.type = EMMJP;
						memcpy(tempnewbuf,&sq2,6);
						tempnewbuf += 6;
					}
				}
				pp+=16;
			}

			temp = sq.pointer;
			memcpy(&oneline,temp,16);
			CloseMouseCursor();
			DrawStructLine(&oneline,BK_Color,&dc);
			//----------- Undo -----------
			undoflag = 1;
			Check_Undo_Buffer(30);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_MOVE;
			head.ob = UNDO_LINE;
			head.p = sq.pointer;
			head.dx = deltax;
			head.dy = deltay;
			memcpy(undo_p,&head,16);
			pDoc->m_pPCB_Undo_Current += 16;
			//---------------------------------------------------------------------------
			oneline.startx+=deltax;
			oneline.starty+=deltay;
			oneline.endx+=deltax;
			oneline.endy+=deltay;
			memcpy(temp,&oneline,16);
			DrawStructLine(&oneline,PCB_LayerColor[oneline.layer],&dc);
			OpenMouseCursor();
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				abcline = (struct Line*)pp;
				if((abcline->attrib==PCB)&&(abcline->layer==oneline.layer)&&\
					(abcline->flag==0)&&(Line_Line_Connect(abcline,&oneline,DRC_T_T)!=0))
				{
					tempbuf = newbuf;
					for(;;)
					{
						sq1 = (struct sq*)tempbuf;
						if(sq1->pointer== pp)
						{
							break;
						}
						else if(sq1->pointer == NULL)
						{
							break;
						}
						tempbuf+=6;
					}
					if(sq1->pointer == NULL)
					{
						sq2.pointer = pp;
						sq2.type = EMMLINE;
						if((deltay==0)&&(abcline->starty==abcline->endy))
						{ 
							if(deltax>0)
							{
								if(abcline->startx<abcline->endx)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
							else
							{
								if(abcline->startx>abcline->endx)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
						}
						else if((deltax==0)&&(abcline->startx==abcline->endx))
						{
							if(deltay>0)
							{
								if(abcline->starty<abcline->endy)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
							else
							{
								if(abcline->starty>abcline->endy)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
						}
						memcpy(tempnewbuf,&sq2,6);
						tempnewbuf+=6;
					}
				}
				pp+=16;
			}

			// to see if connect free SMD pad or not 
			pp = pDoc->m_pJunction_Buffer;
			for(ii=0; ii<pDoc->m_nJunction_Number; ii++)
			{	
				abcjunc = (struct Junction*)pp;
				if((abcjunc->attrib==PCB)&&(abcjunc->flag==0)&&\
					(Line_Pad_Connect(&oneline,abcjunc,DRC_T_P)!=0))
				{
					tempbuf=newbuf;
					for(;;)
					{
						sq1 = (struct sq*)tempbuf;
						if(sq1->pointer == pp)
						{
							break;
						}
						else if(sq1->pointer == NULL)
						{
							break;
						}
						tempbuf+=6;
					}
					if(sq1->pointer == NULL)
					{
						sq2.pointer = pp;
						sq2.type = EMMJP;
						memcpy(tempnewbuf,&sq2,6);
						tempnewbuf+=6;
					}
				}
				pp+=16;
			}

			// to see if connect SMD Package's PAD or not 
			pp = pDoc->m_pPackage_Buffer;
			for(ii=0; ii<pDoc->m_nPackage_Number; ii++)
			{
				abcpackage = (struct Package*)pp;
				if(abcpackage->flag==0)
				{
					temp = pp;
					temp+=112;
					for(ll=0;ll<abcpackage->padnum;ll++)
					{
						abcjunc = (struct Junction*)temp;
						if(Line_Pad_Connect(&oneline,abcjunc,DRC_T_P)!=0)
						{
							tempbuf=newbuf;
							for(;;)
							{
								sq1 = (struct sq*)tempbuf;
								if(sq1->pointer == pp)
								{
									break;
								}
								else if(sq1->pointer == NULL)
								{
									break;
								}
								tempbuf+=6;
							}
							if(sq1->pointer == NULL)
							{
								sq2.pointer = pp;
								sq2.type=EMMPACKAGE;
								memcpy(tempnewbuf,&sq2,6);
								tempnewbuf+=6;
							}
							break;
						}
						temp+=16;
					}
				}
				pp += abcpackage->total16byte*16;
			}
       }
	   else if(sq.type==EMMJP)
       {
			temp = sq.pointer;
			memcpy(&onejunc,temp,16);
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				abcline = (struct Line*)pp;
				if((abcline->attrib==PCB)&&(abcline->flag==0)&&\
					(abcline->layer<=BOTTOMLAYER)&&(Line_Pad_Connect(abcline,&onejunc,0)!=0))
				{
					tempbuf=newbuf;
					for(;;)
					{
						sq1 = (struct sq*)tempbuf;
						if(sq1->pointer == pp)
						{
							if((deltay==0)&&(abcline->starty==abcline->endy))
							{ 
								if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
								{
									sq1->type=sq1->type|0x0010;
								}
								if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
								{
									sq1->type=sq1->type|0x0020;
								}
							}
							else if((deltax==0)&&(abcline->startx==abcline->endx))
							{
								if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
								{
									sq1->type=sq1->type|0x0010;
								}
								if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
								{
									sq1->type=sq1->type|0x0020;
								}
							}
							break;
						}
						else if(sq1->pointer == NULL)
						{
							break;
						}
						tempbuf+=6;
					}
					if(sq1->pointer == NULL)
					{
						sq2.pointer = pp;
						sq2.type=EMMLINE;
						if((deltay==0)&&(abcline->starty==abcline->endy))
						{
							if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
							{
								sq2.type=sq2.type|0x0010;
							}
							if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
							{
								sq2.type=sq2.type|0x0020;
							}
						}
						else if((deltax==0)&&(abcline->startx==abcline->endx))
						{
							if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
							{
								sq2.type=sq2.type|0x0010;
							}
							if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
							{
								sq2.type=sq2.type|0x0020;
							}
						}
						memcpy(tempnewbuf,&sq2,6);
						tempnewbuf+=6;
					}
				}
				pp+=16;
			}

			temp = sq.pointer;
			memcpy(&onejunc,temp,16);
			CloseMouseCursor();
			DrawStructJP(&onejunc,BK_Color,0,&dc);
			//----------- Undo -----------
			undoflag = 1;
			Check_Undo_Buffer(30);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_MOVE;
			head.ob = UNDO_JUNCTION;
			head.p = sq.pointer;
			head.dx = deltax;
			head.dy = deltay;
			memcpy(undo_p,&head,16);
			pDoc->m_pPCB_Undo_Current += 16;
			//---------------------------------------------------------------------------
			onejunc.centerx+=deltax;
			onejunc.centery+=deltay;
			memcpy(temp,&onejunc,16);
			DrawStructJP(&onejunc,PCB_LayerColor[onejunc.layer],0,&dc);
			OpenMouseCursor();
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				abcline = (struct Line*)pp;
				if((abcline->attrib==PCB)&&(abcline->flag==0)&&\
					(abcline->layer<=BOTTOMLAYER)&&(Line_Pad_Connect(abcline,&onejunc,DRC_T_P)!=0))
				{
					tempbuf=newbuf;
					for(;;)
					{
						sq1 = (struct sq*)tempbuf;
						if(sq1->pointer == pp)
						{
							break;
						}
						else if(sq1->pointer == NULL)
						{
							break;
						}
						tempbuf+=6;
					}
					if(sq1->pointer == NULL)
					{
						sq2.pointer = pp;
						sq2.type=EMMLINE;
						if((deltay==0)&&(abcline->starty==abcline->endy))
						{
							if(deltax>0)
							{
								if(abcline->startx<abcline->endx)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
							else
							{
								if(abcline->startx>abcline->endx)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
						}
						else if((deltax==0)&&(abcline->startx==abcline->endx))
						{
							if(deltay>0)
							{
								if(abcline->starty<abcline->endy)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
							else
							{
								if(abcline->starty>abcline->endy)  sq2.type=sq2.type|0x0010;
								else sq2.type=sq2.type|0x0020;
							}
						}
						memcpy(tempnewbuf,&sq2,6);
						tempnewbuf+=6;
					}
				}
				pp+=16;
			}

			pp = pDoc->m_pJunction_Buffer;
			for(ii=0; ii<pDoc->m_nJunction_Number; ii++)
			{	
				abcjunc = (struct Junction*)pp;
				if((abcjunc->attrib==PCB)&&(abcjunc->flag==0)&&\
					(Pad_Pad_Connect(&onejunc,abcjunc,DRC_P_P)!=0))
				{
					tempbuf=newbuf;
					for(;;)
					{
						sq1 = (struct sq*)tempbuf;
						if(sq1->pointer == pp)
						{
							break;
						}
						else if(sq1->pointer == NULL)
						{
						     break;
						}
						tempbuf+=6;
					}
					if(sq1->pointer == NULL)
					{
						sq2.pointer = pp;
						sq2.type=EMMJP;
						memcpy(tempnewbuf,&sq2,6);
						tempnewbuf+=6;
					}
				}
				pp+=16;
			}	
			pp = pDoc->m_pPackage_Buffer;
			for(ii=0; ii<pDoc->m_nPackage_Number; ii++)
			{
				abcpackage = (struct Package*)pp;
				if(abcpackage->flag==0)
				{
					temp = pp;
					temp += 112;
					for(ll=0;ll<abcpackage->padnum;ll++)
					{
						abcjunc = (struct Junction*)temp;
						if(Pad_Pad_Connect(&onejunc,abcjunc,DRC_P_P)!=0)
						{
							tempbuf=newbuf;
							for(;;)
							{
								sq1 = (struct sq*)tempbuf;
								if(sq1->pointer == pp)
								{
									break;
								}
								else if(sq1->pointer == NULL)
								{
									break;
								}
								tempbuf+=6;
							}
							if(sq1->pointer == NULL)
							{
								sq2.pointer = pp;
								sq2.type=EMMPACKAGE;
								memcpy(tempnewbuf,&sq2,6);
								tempnewbuf+=6;
							}
							break;
						}
						temp+=16;
					}
				}
				pp += abcpackage->total16byte*16;
			}
		}
		else if(sq.type==EMMPACKAGE)
		{
			// search for connected tracks 
			abcpackage = (struct Package*)sq.pointer;
			ttemp = (char*)abcpackage;
			ttemp += 112;
			num = abcpackage->padnum;
			for(l=0;l<num;l++)
			{
				memcpy(&onejunc,ttemp,16);
				pp = pDoc->m_pLine_Buffer;
				for(ii=0; ii<pDoc->m_nLine_Number; ii++)
				{	
					abcline = (struct Line*)pp;
					if((abcline->attrib==PCB)&&(abcline->flag==0)&&\
						(abcline->layer<=BOTTOMLAYER)&&(Line_Pad_Connect(abcline,&onejunc,0)!=0))
					{
						tempbuf=newbuf;
						for(;;)
						{
							sq1 = (struct sq*)tempbuf;
							if(sq1->pointer == pp)
							{
								if((deltay==0)&&(abcline->starty==abcline->endy))
								{
									if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
									{
										sq1->type=sq1->type|0x0010;
									}
									else if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
									{
										sq1->type=sq1->type|0x0020;
									}
								}
								else if((deltax==0)&&(abcline->startx==abcline->endx))
								{
									if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
									{
										sq1->type=sq1->type|0x0010;
									}
									else if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
									{
										sq1->type=sq1->type|0x0020;
									}
								}
								break;
							}
							else if(sq1->pointer == NULL)
							{
								break;
							}
							tempbuf+=6;
						}
						if(sq1->pointer == NULL)
						{
							sq2.pointer = pp;
							sq2.type = EMMLINE;
							if((deltay==0)&&(abcline->starty==abcline->endy))
							{
								if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
								{
									sq2.type=sq2.type|0x0010;
								}
								if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
								{
									sq2.type=sq2.type|0x0020;
							    }
							}
							else if((deltax==0)&&(abcline->startx==abcline->endx))
							{
								if(Is_On_JP(abcline->startx,abcline->starty,&onejunc)!=0)
								{
									sq2.type=sq2.type|0x0010;
								}
								if(Is_On_JP(abcline->endx,abcline->endy,&onejunc)!=0)
								{
									sq2.type=sq2.type|0x0020;
								}
							}
							memcpy(tempnewbuf,&sq2,6);
							tempnewbuf+=6;
						}
					}
					pp+=16;
				}
				ttemp+=16;
			}

			// refresh package 
			//----------- Undo -----------
			undoflag = 1;
			abcpackage = (struct Package*)sq.pointer;
			Check_Undo_Buffer(30);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_MOVE;
			head.ob = UNDO_PACKAGE;
			head.p = sq.pointer;
			head.dx = deltax;
			head.dy = deltay;
			head.state1 = (unsigned char)abcpackage->angle;
			head.state2 = (unsigned char)abcpackage->angle;
			memcpy(undo_p,&head,16);
			pDoc->m_pPCB_Undo_Current += 16;
			//----------- Undo -----------
			Check_Undo_Buffer(30);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_MOVE;
			head.ob = UNDO_PACKLIMMIT;
			head.p = sq.pointer;
			head.dx = deltax;
			head.dy = deltay;
			head.size1 = deltax;
			head.size2 = deltay;
			memcpy(undo_p,&head,16);
			pDoc->m_pPCB_Undo_Current += 16;
			//----------- Undo -----------
			DrawDeletePackage(sq.pointer,&dc);
			abcpackage = (struct Package*)sq.pointer;
			abcpackage->cx  += deltax;
			abcpackage->cy  += deltay;
			abcpackage->IDx += deltax;
			abcpackage->IDy += deltay;
			abcpackage->Cx  += deltax;
			abcpackage->Cy  += deltay;
			abcpackage->Px  += deltax;
			abcpackage->Py  += deltay;
			abcpackage->x   += deltax;
			abcpackage->y   += deltay;
			temp = sq.pointer;
			temp+=112;
			for(l=0;l<abcpackage->padnum;l++)
			{
				abcjunc = (struct Junction*)temp;
				abcjunc->centerx+=deltax;
				abcjunc->centery+=deltay;
				temp+=16;
			}
			for(l=0;l<abcpackage->lnum;l++)
			{
				abcline = (struct Line*)temp;
				abcline->startx+=deltax;
				abcline->starty+=deltay;
				abcline->endx+=deltax;
				abcline->endy+=deltay;
				temp+=16;
			}
			for(l=0;l<abcpackage->jnum;l++)
			{
				abcjunc = (struct Junction*)temp;
				abcjunc->centerx+=deltax;
				abcjunc->centery+=deltay;
				temp+=16;
			}
			for(l=0;l<abcpackage->snum;l++)
			{ 
				abcstring = (struct String*)temp;
				abcstring->sx+=deltax;
				abcstring->sy+=deltay;
				temp+=64;
			}
			for(l=0;l<abcpackage->anum;l++)
			{
				abcarc = (struct Arc*)temp;
				abcarc->centerx+=deltax;
				abcarc->centery+=deltay;
				temp+=16;
			}
			for(l=0;l<abcpackage->fnum;l++)
			{
				abcfill = (struct Fill*)temp;
				for(ll=0;ll<abcfill->vertexnum;ll++)
				{
					abcfill->pline[ll].x+=deltax;
					 abcfill->pline[ll].y+=deltay;
				}
				temp+=abcfill->total16byte*16;
			}
			DrawPackage(sq.pointer,&dc);

			// search for DRC tracks/vias/packages 
			abcpackage = (struct Package*)sq.pointer;
			ttemp = (char*)abcpackage;
			ttemp+=112;
			num=abcpackage->padnum;
			for(l=0;l<num;l++)
			{ 
				memcpy(&onejunc,ttemp,16);
	      		pp = pDoc->m_pLine_Buffer;
				for(ii=0; ii<pDoc->m_nLine_Number; ii++)
				{	
					abcline = (struct Line*)pp;
					if((abcline->attrib==PCB)&&(abcline->flag==0)&&\
					(abcline->layer<=BOTTOMLAYER)&&(Line_Pad_Connect(abcline,&onejunc,DRC_T_P)!=0))
					{
						tempbuf=newbuf;
						for(;;)
						{
							sq1 = (struct sq*)tempbuf;
							if(sq1->pointer == pp)
							{
								break;
							}
							else if(sq1->pointer == NULL)
							{
								break;
							}
							tempbuf+=6;
						}
						if(sq1->pointer == NULL)
						{
							sq2.pointer = pp;
							sq2.type=EMMLINE;
							if((deltay==0)&&(abcline->starty==abcline->endy))
							{
								if(deltax>0)
								{
									if(abcline->startx<abcline->endx)  sq2.type=sq2.type|0x0010;
									else sq2.type=sq2.type|0x0020;
								}
								else
								{
									if(abcline->startx>abcline->endx)  sq2.type=sq2.type|0x0010;
									else sq2.type=sq2.type|0x0020;
								}
							}
							else if((deltax==0)&&(abcline->startx==abcline->endx))
							{
								if(deltay>0)
								{
									if(abcline->starty<abcline->endy)  sq2.type=sq2.type|0x0010;
									else sq2.type=sq2.type|0x0020;
								}
								else
								{
									if(abcline->starty>abcline->endy)  sq2.type=sq2.type|0x0010;
									else sq2.type=sq2.type|0x0020;
								}
							}
							memcpy(tempnewbuf,&sq2,6);
							tempnewbuf+=6;
						}
					}
					pp+=16;
				}
				pp = pDoc->m_pJunction_Buffer;
				for(ii=0; ii<pDoc->m_nJunction_Number; ii++)
				{	
					abcjunc = (struct Junction*)pp;
					if((abcjunc->attrib==PCB)&&(abcjunc->flag==0)&&\
						(Pad_Pad_Connect(&onejunc,abcjunc,DRC_P_P)!=0))
					{
						tempbuf=newbuf;
						for(;;)
						{
							sq1 = (struct sq*)tempbuf;
							if(sq1->pointer == pp)
							{
								break;
							}
							else if(sq1->pointer == NULL)
							{
								break;
							}
							tempbuf+=6;
						}
						if(sq1->pointer == NULL)
						{
							sq2.pointer = pp;
							sq2.type = EMMJP;
							memcpy(tempnewbuf,&sq2,6);
							tempnewbuf+=6;
						}
					}
					pp+=16;
				}

				pp = pDoc->m_pPackage_Buffer;
				for(ii=0; ii<pDoc->m_nPackage_Number; ii++)
				{
					onepackage = (struct Package*)pp;
					if(onepackage->flag==0)
					{
						temp = pp;
						temp += 112;
						for(ll=0;ll<onepackage->padnum;ll++)
						{
							abcjunc = (struct Junction*)temp;
							if(Pad_Pad_Connect(&onejunc,abcjunc,DRC_P_P)!=0)
							{
								tempbuf=newbuf;
								for(;;)
								{
									sq1 = (struct sq*)tempbuf;
									if(sq1->pointer == pp)
									{
										break;
									}
									else if(sq1->pointer == NULL)
									{
										break;
									}
									tempbuf+=6;
								}
								if(sq1->pointer == NULL)
								{
									sq2.pointer = pp;
									sq2.type=EMMPACKAGE;
									memcpy(tempnewbuf,&sq2,6);
									tempnewbuf+=6;
								}
								break;
							}
							temp+=16;
						}
					}
					pp += onepackage->total16byte*16;
				}
				ttemp+=16;
			}
		}
	}

	movebuf=newbuf;
	for(;;)
	{
		memcpy(&sq,movebuf,6);
		movebuf+=6;
		if(sq.pointer == NULL) break;
		if((sq.type&0x000f)==EMMLINE)
		{
			temp = sq.pointer;
			memcpy(&oneline,temp,16);
			if(((deltay==0)&&(oneline.starty==oneline.endy))||\
				((deltax==0)&&(oneline.startx==oneline.endx)))
			{
				if((oneline.startx==oneline.endx)&&(oneline.starty==oneline.endy))
				{
					continue;
				}
				if(sq.type==17)
				{
					CloseMouseCursor();
					DrawStructLine(&oneline,BK_Color,&dc);
					//----------- Undo -----------
					undoflag = 1;
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_DRAGLINE;
					head.p = sq.pointer;
					head.dx = deltax;
					head.dy = deltay;
					head.size1 = 0;
					head.size2 = 0;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					//---------------------------------------------------------------------------
					oneline.startx+=deltax;
					oneline.starty+=deltay;
					memcpy(temp,&oneline,16);
					DrawStructLine(&oneline,PCB_LayerColor[oneline.layer],&dc);
					OpenMouseCursor();
				}
				else if(sq.type==33)
				{
					CloseMouseCursor();
					DrawStructLine(&oneline,BK_Color,&dc);
					//----------- Undo -----------
					undoflag = 1;
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_DRAGLINE;
					head.p = sq.pointer;
					head.dx = 0;
					head.dy = 0;
					head.size1 = deltax;
					head.size2 = deltay;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					//---------------------------------------------------------------------------
					oneline.endx+=deltax;
					oneline.endy+=deltay;
					memcpy(temp,&oneline,16);
					DrawStructLine(&oneline,PCB_LayerColor[oneline.layer],&dc);
					OpenMouseCursor();
				}
				else if((sq.type==1)&&\
					(((deltay==0)&&(oneline.starty==oneline.endy))||\
					((deltax==0)&&(oneline.startx==oneline.endx))))
				{
					continue;
				}
				else
				{
					temp = sq.pointer;
					memcpy(&oneline,temp,16);
					CloseMouseCursor();
					DrawStructLine(&oneline,BK_Color,&dc);
					//----------- Undo -----------
					undoflag = 1;
					Check_Undo_Buffer(30);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_MOVE;
					head.ob = UNDO_LINE;
					head.p = sq.pointer;
					head.dx = deltax;
					head.dy = deltay;
					memcpy(undo_p,&head,16);
					pDoc->m_pPCB_Undo_Current += 16;
					//---------------------------------------------------------------------------
					oneline.startx+=deltax;
					oneline.starty+=deltay;
					oneline.endx+=deltax;
					oneline.endy+=deltay;
					memcpy(temp,&oneline,16);
					DrawStructLine(&oneline,PCB_LayerColor[oneline.layer],&dc);
					OpenMouseCursor();
				}
			}
		}
	}
	//----------- Undo -----------
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------

	goto SBEGIN;
}

void CDesignView::OnOptimizeDrip() 
{
	unsigned ch1;
	unsigned x1,y1;
	int padnum;
	char *p, *pointer, *temp, *fillbuff;
	int i,j,l,flag;
	struct Package *abcpack;
	char *hstr1 = "选择焊盘";
	struct Junction abcjunc;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	fillbuff = pDoc->m_pFill_Buffer_Temp;
	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(hstr1);
	flag=0;
	do 
	{
		ch1 = MouseKey();
		if((ch1==VK_RETURN)||(ch1==PUSHMOUSE))
		{
			x1 = m_nCurrent_X;
			y1 = m_nCurrent_Y;
			flag=0;
			if(Select_JP(x1,y1,&pointer) != 0) flag=1;
			else if(Select_PackagePad(x1,y1,&pointer,&padnum)!=0) flag=2;
			if(flag==0)
			{
				MessageBeep(0xFFFFFFFF);
				continue;
			}
			else
			{
				if(flag==1)
				{
					memcpy(&gnjunction,pointer,16);
				}
				else if(flag==2) 
				{
					abcpack = (struct Package*)pointer;
					temp = pointer;
					temp+=112;
					for(l=0;l<abcpack->padnum;l++)
					{
						if(l==padnum)
						{
							memcpy(&gnjunction,temp,16);
							break;
						}
						temp+=16;
					}
				}
				generate_drip(gnjunction);
				CloseMouseCursor();
				DrawStructJP(&gnjunction,PCB_LayerColor[gnjunction.layer],0,&dc);
				OpenMouseCursor();

				//j = FreeMenu(200,160,32,2,changeallpad);
				j = AfxMessageBox("修改所有相同属性焊盘?", MB_YESNO|MB_ICONQUESTION);
				if( j == IDYES)
				{
					if(flag==1)
					{
						p = pDoc->m_pJunction_Buffer;
						for(i=0; i<pDoc->m_nJunction_Number; i++)
						{	
							memcpy(&abcjunc,p,16);
							if((abcjunc.attrib==PCB)&&(abcjunc.Dx==gnjunction.Dx)&&\
								(abcjunc.Dy==gnjunction.Dy)&&(abcjunc.d==gnjunction.d)&&\
								(abcjunc.padtype==gnjunction.padtype)&&(abcjunc.flag==0)&&\
								((abcjunc.centerx!=gnjunction.centerx)||(abcjunc.centery!=gnjunction.centery)))
							{
								generate_drip(abcjunc);
								CloseMouseCursor();
								DrawStructJP(&abcjunc,PCB_LayerColor[abcjunc.layer],0,&dc);
								OpenMouseCursor();
							}
							p+=16;
						}	
					}
					else if(flag==2)
					{
						p = pDoc->m_pPackage_Buffer;
						for(i=0; i<pDoc->m_nPackage_Number; i++)
						{
							abcpack = (struct Package*)p;
							if(abcpack->flag==0)
							{
								temp = p;
								temp += 112;
								for(l=0;l<abcpack->padnum;l++)
								{
									memcpy(&abcjunc,temp,16);
									if((abcjunc.Dx==gnjunction.Dx)&&(abcjunc.Dy==gnjunction.Dy)&&\
									(abcjunc.d==gnjunction.d)&&(abcjunc.padtype==gnjunction.padtype)&&\
									((abcjunc.centerx!=gnjunction.centerx)||(abcjunc.centery!=gnjunction.centery)))
									{
										generate_drip(abcjunc);
										CloseMouseCursor();
										DrawStructJP(&abcjunc,PCB_LayerColor[abcjunc.layer],0,&dc);
										OpenMouseCursor();
									}
									temp+=16;
								}
							}
							p+=abcpack->total16byte*16;
						}
					}
				}
			}
		}
		else  Command_In_Function(ch1);
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));

	//----------- Undo -----------
	if(fillbuff != pDoc->m_pFill_Buffer_Temp)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------
	CL_Auto_Pan=OFF;
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	Write_Help(commandend);
}


void CDesignView::generate_drip(struct Junction onejunction)
{
	unsigned x1,y1;
	char *p;
	int i;
	struct Line *abcline;
	float alpha;
	unsigned u1;
	float f1=0.87, f2=0.25, f3=0.96, ff=0.7;
	unsigned x2,y2,x3,y3;
	struct Fill onefill;
	struct Undo_Head head;
	char *undo_p;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	onefill.id=0x70;
	onefill.vertexnum=4;
	onefill.fillpattern=0;
	onefill.total16byte=2;
	onefill.u1=onefill.u2=0;
	onefill.type=0;
	onefill.u3=0;
	onefill.attrib=PCB;
	onefill.u4=0;
	onefill.u5=0;
	onefill.flag=0;
	// generate drip 
	// search for track connected 
	if(((onejunction.padtype&0x07)==1)||\
      (((onejunction.padtype&0x07)==3)&&(onejunction.Dx==onejunction.Dy)))
    {
		p = pDoc->m_pLine_Buffer;
		for(i=0; i<pDoc->m_nLine_Number; i++)
		{	
			abcline = (struct Line*)p;
			if( (abcline->attrib==PCB)&&(abcline->flag==0)&&(abcline->layer<=BOTTOMLAYER)&&\
				((abcline->layer==onejunction.layer)||(onejunction.layer==MULTILAYER))&&\
				( (Is_On_JP(abcline->startx,abcline->starty,&onejunction)!=0)||\
				  (Is_On_JP(abcline->endx,abcline->endy,&onejunction)!=0)||\
				  (Is_On_Line(onejunction.centerx,onejunction.centery,abcline)!=0))  )
			{
				if((Is_On_JP(abcline->startx,abcline->starty,&onejunction)==0)||\
					(Is_On_JP(abcline->endx,abcline->endy,&onejunction)==0))
				{

					if(Is_On_JP(abcline->startx,abcline->starty,&onejunction)!=0)
					{
						if(SQRT(abcline->endx,abcline->endy,onejunction.centerx,onejunction.centery)>onejunction.Dx/2)
						{
							if((abcline->starty==abcline->endy)&&(abcline->endx>abcline->startx)) alpha=0;
							else if((abcline->starty==abcline->endy)&&(abcline->endx<abcline->startx)) alpha=180;
							else if((abcline->startx==abcline->endx)&&(abcline->endy>abcline->starty)) alpha=90;
							else if((abcline->startx==abcline->endx)&&(abcline->endy<abcline->starty)) alpha=270;
							else alpha=180+(180/CL_pi)*atan2(((float)abcline->starty-(float)abcline->endy),\
								((float)abcline->startx-(float)abcline->endx));
							if(((int)alpha>338)||((int)alpha<=22))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->endx<u1) x1=abcline->endx;
								else x1=u1;
								if(alpha==0) y1=abcline->starty;
								else y1=abcline->starty+((float)x1-abcline->startx)*((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx+onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>22)&&((int)alpha<=67))
							{ 
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->endx<u1) x1=abcline->endx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery-f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery+f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>67)&&((int)alpha<=112))
							{
								u1=onejunction.centery+onejunction.Dx;
								if(abcline->endy<u1) y1=abcline->endy;
								else y1=u1;
								if(alpha==90) x1=abcline->startx;
								else x1=abcline->startx+((float)y1-abcline->starty)*((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								x2=onejunction.centerx+f1*onejunction.Dx/2;
								y2=onejunction.centery+onejunction.Dx/4;
								x3=onejunction.centerx-f1*onejunction.Dx/2;
								y3=onejunction.centery+onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>112)&&((int)alpha<=157))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->endx>u1) x1=abcline->endx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+f2*onejunction.Dx/2;
								y2=onejunction.centery+f3*onejunction.Dx/2;
								x3=onejunction.centerx-f3*onejunction.Dx/2;
								y3=onejunction.centery-f2*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>157)&&((int)alpha<=202))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->endx>u1) x1=abcline->endx;
								else x1=u1;
								if(alpha==180) y1=abcline->starty;
								else y1=abcline->starty+((float)x1-abcline->startx)*((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx-onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx-onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>202)&&((int)alpha<=247))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->endx>u1) x1=abcline->endx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx-f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx+f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>247)&&((int)alpha<=292))
							{
								u1=onejunction.centery-onejunction.Dx;
								if(abcline->endy>u1) y1=abcline->endy;
								else y1=u1;
								if(alpha==270) x1=abcline->startx;
								else x1=abcline->startx+((float)y1-abcline->starty)*((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								x2=onejunction.centerx-f1*onejunction.Dx/2;
								y2=onejunction.centery-onejunction.Dx/4;
								x3=onejunction.centerx+f1*onejunction.Dx/2;
								y3=onejunction.centery-onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>292)&&((int)alpha<=338))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->endx<u1) x1=abcline->endx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
						}
					}
					else if(Is_On_JP(abcline->endx,abcline->endy,&onejunction)!=0)
					{
						if(SQRT(abcline->startx,abcline->starty,onejunction.centerx,onejunction.centery)>onejunction.Dx/2)
						{
							if((abcline->starty==abcline->endy)&&(abcline->startx>abcline->endx)) alpha=0;
							else if((abcline->starty==abcline->endy)&&(abcline->startx<abcline->endx)) alpha=180;
							else if((abcline->startx==abcline->endx)&&(abcline->starty>abcline->endy)) alpha=90;
							else if((abcline->startx==abcline->endx)&&(abcline->starty<abcline->endy)) alpha=270;
							else alpha=180+(180/CL_pi)*atan2(((float)abcline->endy-(float)abcline->starty),\
								((float)abcline->endx-(float)abcline->startx));
							if(((int)alpha>338)||((int)alpha<=22))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->startx<u1) x1=abcline->startx;
								else x1=u1;
								if(alpha==0) y1=abcline->starty;
								else y1=abcline->starty+((float)x1-abcline->startx)*((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx+onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>22)&&((int)alpha<=67))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->startx<u1) x1=abcline->startx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery-f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery+f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>67)&&((int)alpha<=112))
							{
								u1=onejunction.centery+onejunction.Dx;
								if(abcline->starty<u1) y1=abcline->starty;
								else y1=u1;
								if(alpha==90) x1=abcline->startx;
								else x1=abcline->startx+((float)y1-abcline->starty)*((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								x2=onejunction.centerx+f1*onejunction.Dx/2;
								y2=onejunction.centery+onejunction.Dx/4;
								x3=onejunction.centerx-f1*onejunction.Dx/2;
								y3=onejunction.centery+onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>112)&&((int)alpha<=157))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->startx>u1) x1=abcline->startx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+f2*onejunction.Dx/2;
								y2=onejunction.centery+f3*onejunction.Dx/2;
								x3=onejunction.centerx-f3*onejunction.Dx/2;
								y3=onejunction.centery-f2*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>157)&&((int)alpha<=202))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->startx>u1) x1=abcline->startx;
								else x1=u1;
								if(alpha==180) y1=abcline->starty;
								else y1=abcline->starty+((float)x1-abcline->startx)*((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx-onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx-onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>202)&&((int)alpha<=247))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->startx>u1) x1=abcline->startx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx-f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx+f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>247)&&((int)alpha<=292))
							{
								u1=onejunction.centery-onejunction.Dx;
								if(abcline->starty>u1) y1=abcline->starty;
								else y1=u1;
								if(alpha==270) x1=abcline->startx;
								else x1=abcline->startx+((float)y1-abcline->starty)*((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								x2=onejunction.centerx-f1*onejunction.Dx/2;
								y2=onejunction.centery-onejunction.Dx/4;
								x3=onejunction.centerx+f1*onejunction.Dx/2;
								y3=onejunction.centery-onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>292)&&((int)alpha<=338))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->startx<u1) x1=abcline->startx;
								else x1=u1;
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
						}
					}
					else
					{
						if(SQRT(abcline->endx,abcline->endy,onejunction.centerx,onejunction.centery)>onejunction.Dx/2)
						{
							if((abcline->starty==abcline->endy)&&(abcline->endx>abcline->startx)) alpha=0;
							else if((abcline->starty==abcline->endy)&&(abcline->endx<abcline->startx)) alpha=180;
							else if((abcline->startx==abcline->endx)&&(abcline->endy>abcline->starty)) alpha=90;
							else if((abcline->startx==abcline->endx)&&(abcline->endy<abcline->starty)) alpha=270;
							else alpha=180+(180/CL_pi)*atan2(((float)abcline->starty-(float)abcline->endy),\
								((float)abcline->startx-(float)abcline->endx));
							if(((int)alpha>338)||((int)alpha<=22))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->endx<u1) x1=abcline->endx;
								else x1=u1;
								//if(alpha==0) y1 = abcline->starty;
								if(alpha==0) y1 = onejunction.centery; 
								//else y1 = abcline->starty+((float)x1-abcline->startx)*((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								else y1 = onejunction.centery+((float)x1-onejunction.centerx)*\
									((float)abcline->endy-onejunction.centery)/((float)abcline->endx-onejunction.centerx);
								x2=onejunction.centerx+onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx+onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>22)&&((int)alpha<=67))
							{ 
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->endx<u1) x1=abcline->endx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=onejunction.centery+((float)x1-onejunction.centerx)*\
									((float)abcline->endy-onejunction.centery)/((float)abcline->endx-onejunction.centerx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery-f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery+f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>67)&&((int)alpha<=112))
							{
								u1=onejunction.centery+onejunction.Dx;
								if(abcline->endy<u1) y1=abcline->endy;
								else y1=u1;
								//if(alpha==90) x1=abcline->startx;
								if(alpha==90) x1=onejunction.centerx;
								//else x1=abcline->startx+((float)y1-abcline->starty)*\
								//  ((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								else x1=onejunction.centerx+((float)y1-onejunction.centery)*\
									((float)abcline->endx-onejunction.centerx)/((float)abcline->endy-onejunction.centery);
								x2=onejunction.centerx+f1*onejunction.Dx/2;
								y2=onejunction.centery+onejunction.Dx/4;
								x3=onejunction.centerx-f1*onejunction.Dx/2;
								y3=onejunction.centery+onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>112)&&((int)alpha<=157))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->endx>u1) x1=abcline->endx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=onejunction.centery+((float)x1-onejunction.centerx)*\
									((float)abcline->endy-onejunction.centery)/((float)abcline->endx-onejunction.centerx);
								x2=onejunction.centerx+f2*onejunction.Dx/2;
								y2=onejunction.centery+f3*onejunction.Dx/2;
								x3=onejunction.centerx-f3*onejunction.Dx/2;
								y3=onejunction.centery-f2*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>157)&&((int)alpha<=202))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->endx>u1) x1=abcline->endx;
								else x1=u1;
								//if(alpha==180) y1=abcline->starty;
								if(alpha==180) y1=onejunction.centery;
								//else y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								else y1=onejunction.centery+((float)x1-onejunction.centerx)*\
									((float)abcline->endy-onejunction.centery)/((float)abcline->endx-onejunction.centerx);
								x2=onejunction.centerx-onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx-onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>202)&&((int)alpha<=247))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->endx>u1) x1=abcline->endx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=onejunction.centery+((float)x1-onejunction.centerx)*\
									((float)abcline->endy-onejunction.centery)/((float)abcline->endx-onejunction.centerx);
								x2=onejunction.centerx-f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx+f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>247)&&((int)alpha<=292))
							{
								u1=onejunction.centery-onejunction.Dx;
								if(abcline->endy>u1) y1=abcline->endy;
								else y1=u1;
								//if(alpha==270) x1=abcline->startx;
								if(alpha==270) x1=onejunction.centerx;
								//else x1=abcline->startx+((float)y1-abcline->starty)*\
								//  ((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								else x1=onejunction.centerx+((float)y1-onejunction.centery)*\
									((float)abcline->endx-onejunction.centerx)/((float)abcline->endy-onejunction.centery);
								x2=onejunction.centerx-f1*onejunction.Dx/2;
								y2=onejunction.centery-onejunction.Dx/4;
								x3=onejunction.centerx+f1*onejunction.Dx/2;
								y3=onejunction.centery-onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>292)&&((int)alpha<=338))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->endx<u1) x1=abcline->endx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=onejunction.centery+((float)x1-onejunction.centerx)*\
									((float)abcline->endy-onejunction.centery)/((float)abcline->endx-onejunction.centerx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
						}
						
						if(SQRT(abcline->startx,abcline->starty,onejunction.centerx,onejunction.centery)>onejunction.Dx/2)
						{
							if((abcline->starty==abcline->endy)&&(abcline->startx>abcline->endx)) alpha=0;
							else if((abcline->starty==abcline->endy)&&(abcline->startx<abcline->endx)) alpha=180;
							else if((abcline->startx==abcline->endx)&&(abcline->starty>abcline->endy)) alpha=90;
							else if((abcline->startx==abcline->endx)&&(abcline->starty<abcline->endy)) alpha=270;
							else alpha=180+(180/CL_pi)*atan2(((float)abcline->endy-(float)abcline->starty),\
								((float)abcline->endx-(float)abcline->startx));
							if(((int)alpha>338)||((int)alpha<=22))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->startx<u1) x1=abcline->startx;
								else x1=u1;
								if(alpha==0) y1=abcline->starty;
								//else y1=abcline->starty+((float)x1-abcline->startx)*((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								else y1=abcline->starty+((float)x1-abcline->startx)*((float)onejunction.centery-abcline->starty)/((float)onejunction.centerx-abcline->startx);
								x2=onejunction.centerx+onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx+onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>22)&&((int)alpha<=67))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->startx<u1) x1=abcline->startx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)onejunction.centery-abcline->starty)/((float)onejunction.centerx-abcline->startx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery-f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery+f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>67)&&((int)alpha<=112))
							{
								u1=onejunction.centery+onejunction.Dx;
								if(abcline->starty<u1) y1=abcline->starty;
								else y1=u1;
								if(alpha==90) x1=abcline->startx;
								//else x1=abcline->startx+((float)y1-abcline->starty)*((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								else x1=abcline->startx+((float)y1-abcline->starty)*((float)onejunction.centerx-abcline->startx)/((float)onejunction.centery-abcline->starty);
								x2=onejunction.centerx+f1*onejunction.Dx/2;
								y2=onejunction.centery+onejunction.Dx/4;
								x3=onejunction.centerx-f1*onejunction.Dx/2;
								y3=onejunction.centery+onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>112)&&((int)alpha<=157))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->startx>u1) x1=abcline->startx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)onejunction.centery-abcline->starty)/((float)onejunction.centerx-abcline->startx);
								x2=onejunction.centerx+f2*onejunction.Dx/2;
								y2=onejunction.centery+f3*onejunction.Dx/2;
								x3=onejunction.centerx-f3*onejunction.Dx/2;
								y3=onejunction.centery-f2*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>157)&&((int)alpha<=202))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->startx>u1) x1=abcline->startx;
								else x1=u1;
								if(alpha==180) y1=abcline->starty;
								//else y1=abcline->starty+((float)x1-abcline->startx)*((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								else y1=abcline->starty+((float)x1-abcline->startx)*((float)onejunction.centery-abcline->starty)/((float)onejunction.centerx-abcline->startx);
								x2=onejunction.centerx-onejunction.Dx/4;
								y2=onejunction.centery+f1*onejunction.Dx/2;
								x3=onejunction.centerx-onejunction.Dx/4;
								y3=onejunction.centery-f1*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>202)&&((int)alpha<=247))
							{
								u1=onejunction.centerx-onejunction.Dx;
								if(abcline->startx>u1) x1=abcline->startx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)onejunction.centery-abcline->starty)/((float)onejunction.centerx-abcline->startx);
								x2=onejunction.centerx-f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx+f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>247)&&((int)alpha<=292))
							{
								u1=onejunction.centery-onejunction.Dx;
								if(abcline->starty>u1) y1=abcline->starty;
								else y1=u1;
								if(alpha==270) x1=abcline->startx;
								//else x1=abcline->startx+((float)y1-abcline->starty)*((float)abcline->endx-abcline->startx)/((float)abcline->endy-abcline->starty);
								else x1=abcline->startx+((float)y1-abcline->starty)*((float)onejunction.centerx-abcline->startx)/((float)onejunction.centery-abcline->starty);
								x2=onejunction.centerx-f1*onejunction.Dx/2;
								y2=onejunction.centery-onejunction.Dx/4;
								x3=onejunction.centerx+f1*onejunction.Dx/2;
								y3=onejunction.centery-onejunction.Dx/4;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
							else if(((int)alpha>292)&&((int)alpha<=338))
							{
								u1=onejunction.centerx+onejunction.Dx;
								if(abcline->startx<u1) x1=abcline->startx;
								else x1=u1;
								//y1=abcline->starty+((float)x1-abcline->startx)*\
								//	((float)abcline->endy-abcline->starty)/((float)abcline->endx-abcline->startx);
								y1=abcline->starty+((float)x1-abcline->startx)*\
									((float)onejunction.centery-abcline->starty)/((float)onejunction.centerx-abcline->startx);
								x2=onejunction.centerx+f3*onejunction.Dx/2;
								y2=onejunction.centery+f2*onejunction.Dx/2;
								x3=onejunction.centerx-f2*onejunction.Dx/2;
								y3=onejunction.centery-f3*onejunction.Dx/2;
								onefill.pline[0].x=x1; onefill.pline[0].y=y1;
								onefill.pline[1].x=x2; onefill.pline[1].y=y2;
								onefill.pline[2].x=x3; onefill.pline[2].y=y3;
								onefill.pline[3].x=x1; onefill.pline[3].y=y1;
								onefill.layer=abcline->layer;
								//----------- Undo -----------
								Check_Undo_Buffer(10);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_FILL;
								head.p = pDoc->m_pFill_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//---------------------------------------------------------------------------
								pDoc->Save_EMMFill(&onefill);
								DrawStructFillArea(&onefill,PCB_LayerColor[onefill.layer],&dc);
								//OpenMouseCursor();
							}
						}
					}
				}
			}
			p+=16;
		}	
    }
}

void CDesignView::OnOptimizeAutofill() 
{
	int i,ii,j,p,delta,trackwidth, optimize, filllimit;
	int flag, undoflag;
	int t_t, t_p, p_p;
	unsigned ch1;
	unsigned x1,y1,x2,y2,tempd;
	int xs,ys,xe,ye;
	struct Junction via;
	struct Line oneline;
	int fd, mode, linenum, linenum2;
	char *p1, *p2, *position, *position2;
	struct Line *abcline1, *abcline2;
	struct Undo_Head head;
	char *undo_p;

	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);
	
	int drawmode = dc.GetROP2();

	if(m_nPCB_Current_Layer>BOTTOMLAYER)
    {
		Error("当前图层无法放置覆铜区.");
		return;
    }

	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(helpstr1);

	flag=0;
	do
	{
		ch1 = MouseKey();
		if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
		{ 
			CL_Auto_Pan=OFF;
			DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
			Write_Help(commandend);
			return ;
		}
		if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
		{
			x1 = m_nCurrent_X;
			y1 = m_nCurrent_Y;
			Get_VPort_XY(&xs,&ys,x1,y1);
			xe=xs;
			ye=ys;
			//CloseMouseCursor();
			Write_Help(helpstr2);
			//OpenMouseCursor();
			do
			{
				ch1=MouseKey();
				Get_VPort_XY(&xs,&ys,x1,y1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					//CloseMouseCursor();
					//setwritemode(1);
					//setcolor(Overwrite_Color);
					//rectangle(xs,ys,xe,ye);
					dc.SetROP2(R2_NOTXORPEN);
					rectangle(&dc,xs,ys,xe,ye);
					dc.SetROP2(drawmode);
					CL_Auto_Pan = OFF;
					DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
					//OpenMouseCursor();
					return ;
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
						if(CL_RedrawFlag==1){ CL_RedrawFlag=0;}
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
					//rectangle(xs,ys,xe,ye);
					dc.SetROP2(R2_NOTXORPEN);
					rectangle(&dc,xs,ys,xe,ye);
					dc.SetROP2(drawmode);
					//OpenMouseCursor();
					break;
				}
				else  Command_In_Function(ch1);
			}while(1);
		}
		else  Command_In_Function(ch1);
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));

    if(x1>x2)
	{
		tempd=x2;
		x2=x1;
		x1=tempd;
	}
    if(y1>y2)
	{
		tempd=y2;
		y2=y1;
		y1=tempd;
	}

	CAutoFill autofill;
	autofill.m_nFill_D = Auto_Fill_D;
	autofill.m_nTrack_Width = Auto_Fill_Width;
	autofill.m_nFill_Mode = Auto_Fill_Mode;
	autofill.m_nFill_Grid = Auto_Fill_Grid;
	autofill.m_nOptimize = TRUE;
	autofill.m_nAutofill_Limit = 1000;

	Write_Help("定义填充模式和参数");
    i = autofill.DoModal();
	if(i != IDOK)
	{
		CL_Auto_Pan=OFF;
		DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
		Write_Help(commandend);
		return;
	}
	
	position = pDoc->m_pLine_Buffer_Temp;
	linenum = 0;

	Auto_Fill_D = autofill.m_nFill_D ;
	Auto_Fill_Width = autofill.m_nTrack_Width;
	Auto_Fill_Mode = autofill.m_nFill_Mode;
	Auto_Fill_Grid = autofill.m_nFill_Grid;
	optimize = autofill.m_nOptimize;
	filllimit = autofill.m_nAutofill_Limit;

	p = Auto_Fill_D;
	trackwidth = Auto_Fill_Width;
	mode = Auto_Fill_Mode + 1;
	fd = Auto_Fill_Grid;
    
	t_t = DRC_T_T;
    t_p = DRC_T_P;
    p_p = DRC_P_P;
    DRC_T_T = p;
	DRC_T_P = p;
	DRC_P_P = p;
	if(DRC_T_T < t_t) DRC_T_T = t_t;
	if(DRC_T_P < t_p) DRC_T_P = t_p;
	if(DRC_P_P < p_p) DRC_P_P = p_p;

	if(pDoc->m_nNet_Number!=0) OnAutoHighlightNet();

    via.id = 0x21;
    via.Dx = via.Dy = trackwidth;
    via.d=0;
    via.padtype = 0x81;
    via.attrib = PCB;
    via.layer = (unsigned char)m_nPCB_Current_Layer;
    via.flag = 0;
    strcpy((char*)via.padname,Null_Str);

    oneline.flag = 1;
    oneline.id = 0x10;
    oneline.layer = (unsigned char)m_nPCB_Current_Layer;
    oneline.linewidth = trackwidth;
    oneline.attrib = PCB;
    oneline.type = 0;

    Write_Help("自动覆铜...");
	EnableFrame(FALSE);

	POINT pp;
	int ix,iy;
	Get_VPort_XY(&ix,&iy,m_nCurrent_X,m_nCurrent_Y);
	pp.x = ix; pp.y = iy;
	DrawMyCursor(&dc,&pp);

	undoflag = 0;
    
    if((mode==1)||(mode==3)||(mode==4))
	{
		via.centerx = x1;
	    via.centery = y1;
	    i = Clearance_Route((struct Line*)&via);
	    if(i==0)
		{
			oneline.startx = via.centerx;
			oneline.starty = via.centery;
			flag=0;
		}
	    else flag=1;
	    delta = p;  
	    for(;;)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					j = AfxMessageBox(interuptautofill, MB_YESNO|MB_ICONWARNING);
					if( j == IDYES)
					{
						goto SEEDEND;
					}
				}
				break;
			}
			via.centerx += delta;
			if(via.centerx >x2)
			{
				if(flag==0)
				{
					oneline.endx = x2;
					oneline.endy = via.centery;
					//------------- Undo -----------------
					undoflag = 1;
					Check_Undo_Buffer(20);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_DELETE;
					head.ob = UNDO_LINE;
					head.p = pDoc->m_pLine_Buffer_Temp;
					memcpy(undo_p,&head,6);
					pDoc->m_pPCB_Undo_Current += 6;
					//-------------------------------------
					pDoc->Save_EMMLine(&oneline);
					linenum++;
					CloseMouseCursor();
					DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
					OpenMouseCursor();
				}
				via.centerx = x1;
				if(via.centery==y2) break;
				via.centery += ( trackwidth - 1);
				if(via.centery>y2) via.centery=y2;
				i = Clearance_Route((struct Line*)&via);
				if(i==0)
				{
					oneline.startx = via.centerx;
					oneline.starty = via.centery;
					flag=0;
				}
				else flag=1;
				delta = p; 
				continue;
			}
			i = Clearance_Route((struct Line*)&via);
			if(i==0)
			{
				if(flag==1)
				{
					if(delta==1)
					{
						oneline.startx = via.centerx;
						oneline.starty = via.centery;
						flag=0;
						delta = p; 
					}
					else
					{
						via.centerx -= delta;
						delta = delta/2;
					}
				}
			}
			else
			{
				if(flag==0)
				{
					if(delta==1)
					{
						oneline.endx = via.centerx-1;
						oneline.endy = via.centery;
						//------------- Undo -----------------
						undoflag = 1;
						Check_Undo_Buffer(20);
						undo_p = pDoc->m_pPCB_Undo_Current;
						head.op = UNDO_DELETE;
						head.ob = UNDO_LINE;
						head.p = pDoc->m_pLine_Buffer_Temp;
						memcpy(undo_p,&head,6);
						pDoc->m_pPCB_Undo_Current += 6;
						//-------------------------------------
						pDoc->Save_EMMLine(&oneline);
						linenum++;
						CloseMouseCursor();
						DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
						OpenMouseCursor();
						flag=1;
						delta = p;
						continue;
					}
					else
					{
						via.centerx -=delta;
						delta = delta/2;
						continue;
					}
				}
			}
		}
	}

	position2 = pDoc->m_pLine_Buffer_Temp;
	linenum2 = 0;
    if((mode==2)||(mode==3)||(mode==4))
	{
	    via.centerx=x1;
	    via.centery=y1;
	    i = Clearance_Route((struct Line*)&via);
	    if(i==0)
		{
			oneline.startx = via.centerx;
			oneline.starty = via.centery;
			flag=0;
		}
	    else flag=1;
	    delta = p;
	    for(;;)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					j = AfxMessageBox(interuptautofill, MB_YESNO|MB_ICONWARNING);
					if( j == IDYES)
					{
						 goto SEEDEND;
					}
				}
				break;
			}
			via.centery+=delta;
			if(via.centery >y2)
			{
				if(flag==0)
				{
					oneline.endy = y2;
					oneline.endx = via.centerx;
					//------------- Undo -----------------
					undoflag = 1;
					Check_Undo_Buffer(20);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_DELETE;
					head.ob = UNDO_LINE;
					head.p = pDoc->m_pLine_Buffer_Temp;
					memcpy(undo_p,&head,6);
					pDoc->m_pPCB_Undo_Current += 6;
					//-------------------------------------
					pDoc->Save_EMMLine(&oneline);
					linenum2++;
					CloseMouseCursor();
					DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
					OpenMouseCursor();
				}
				via.centery=y1;
				if(via.centerx==x2) break;
				via.centerx += (trackwidth - 1);
				if(via.centerx>x2) via.centerx=x2;
				i = Clearance_Route((struct Line*)&via);
				if(i==0)
				{
					oneline.startx = via.centerx;
					oneline.starty = via.centery;
					flag=0;
				}
				else flag=1;
				delta = p; 
				continue;
			}
			i=Clearance_Route((struct Line*)&via);
			if(i==0)
			{
				if(flag==1)
				{
					if(delta==1)
					{
						oneline.startx = via.centerx;
						oneline.starty = via.centery;
						flag=0;
						delta = p;
					}
					else
					{
						via.centery -=delta;
						delta = delta/2;
					}
				}
			}
			else
			{
				if(flag==0)
				{
					if(delta==1)
					{
						oneline.endy = via.centery-1;
						oneline.endx = via.centerx;
						//------------- Undo -----------------
						undoflag = 1;
						Check_Undo_Buffer(20);
						undo_p = pDoc->m_pPCB_Undo_Current;
						head.op = UNDO_DELETE;
						head.ob = UNDO_LINE;
						head.p = pDoc->m_pLine_Buffer_Temp;
						memcpy(undo_p,&head,6);
						pDoc->m_pPCB_Undo_Current += 6;
						//-------------------------------------
						pDoc->Save_EMMLine(&oneline);
						linenum2++;
						CloseMouseCursor();
						DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
						OpenMouseCursor();
						flag=1;
						delta = p; 
						continue;
					}
					else
					{
						via.centery -=delta;
						delta = delta/2;
						continue;
					}
				}
			}
		}
	}

SEEDEND:
	if((mode == 1)||(mode == 3)||(mode == 4))
	{
		if((mode != 3)&&(mode != 4))
		{
			p1 = position;
			for(i=0; i<linenum; i++)
			{	
				abcline1 = (struct Line*)p1;
				if((abcline1->attrib == PCB)&&(abcline1->flag == 1))
				{
					if( (i+1) >= linenum) break;
					abcline2 = (struct Line*)(p1 + 16);
					if((abcline2->attrib == PCB)&&(abcline2->flag == 1))
					{
						if(abcline2->starty == abcline1->starty)
						{
							oneline.flag = 2;
							oneline.startx = abcline1->endx;
							oneline.endx = abcline2->startx;
							oneline.starty = abcline1->starty;
							oneline.endy = abcline1->endy;
							if(p > trackwidth) 
							{
								DRC_T_T = p - trackwidth;
								DRC_T_P = p - trackwidth;
								if(DRC_T_T < t_t) DRC_T_T = t_t;
								if(DRC_T_P < t_p) DRC_T_P = t_p;
							}
							j = Clearance_Route(&oneline);
							if(j == 0)
							{
								//------------- Undo -----------------
								undoflag = 1;
								Check_Undo_Buffer(20);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_LINE;
								head.p = pDoc->m_pLine_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//-------------------------------------
								pDoc->Save_EMMLine(&oneline);
								CloseMouseCursor();
								DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
								OpenMouseCursor();
							}
						}
					}
				}
				p1 += 16;
			}
		}
	
		p1 = position; //pDoc->m_pLine_Buffer;
		for(i=0; i<linenum; i++)
		{	
			abcline1 = (struct Line*)p1;
			if((abcline1->attrib == PCB)&&(abcline1->flag == 1))
			{
				flag = 0;
				p2 = p1;
				for(ii = i; ii<linenum; ii++)
				{	
					abcline2 = (struct Line*)p2;
					if((abcline2->attrib == PCB)&&(abcline2->flag == 1))
					{
						if(abcline2->starty == abcline1->starty){}
						else if(abcline2->starty == (abcline1->starty + trackwidth - 1))
						{
							if(abs(abcline2->startx - abcline1->startx) <= p)//trackwidth)
							{
								oneline.flag = 2;
								oneline.startx = abcline1->startx;
								oneline.starty = abcline1->starty;
								oneline.endx = abcline2->startx;
								oneline.endy = abcline2->starty;
								//------------- Undo -----------------
								undoflag = 1;
								Check_Undo_Buffer(20);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_LINE;
								head.p = pDoc->m_pLine_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//-------------------------------------
								pDoc->Save_EMMLine(&oneline);
								CloseMouseCursor();
								DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
								OpenMouseCursor();
							}
							if(abs(abcline2->endx - abcline1->endx) <= p) //trackwidth)
							{
								oneline.flag = 2;
								oneline.startx = abcline1->endx;
								oneline.starty = abcline1->endy;
								oneline.endx = abcline2->endx;
								oneline.endy = abcline2->endy;
								//------------- Undo -----------------
								undoflag = 1;
								Check_Undo_Buffer(20);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_LINE;
								head.p = pDoc->m_pLine_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//-------------------------------------
								pDoc->Save_EMMLine(&oneline);
								CloseMouseCursor();
								DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
								OpenMouseCursor();
							}
						}
						else break;
					}
					p2 += 16;
					if(flag>1) break;
				}		
			}
			p1 += 16;
		}
	}
	
	if((mode == 2)||(mode == 3))
	{
		if((mode != 3)&&(mode != 4))
		{
			p1 = position2;
			for(i=0; i<linenum2; i++)
			{	
				abcline1 = (struct Line*)p1;
				if((abcline1->attrib == PCB)&&(abcline1->flag == 1))
				{
					if( (i+1) >= linenum2) break;
					abcline2 = (struct Line*)(p1 + 16);
					if((abcline2->attrib == PCB)&&(abcline2->flag == 1))
					{
						if(abcline2->startx == abcline1->startx)
						{
							oneline.flag = 2;
							oneline.startx = abcline1->startx;
							oneline.endx = abcline1->endx;
							oneline.starty = abcline1->endy;
							oneline.endy = abcline2->starty;
							if(p > trackwidth) 
							{
								DRC_T_T = p - trackwidth;
								DRC_T_P = p - trackwidth;
								if(DRC_T_T < t_t) DRC_T_T = t_t;
								if(DRC_T_P < t_p) DRC_T_P = t_p;
							}
							j = Clearance_Route(&oneline);
							if(j == 0)
							{
								//------------- Undo -----------------
								undoflag = 1;
								Check_Undo_Buffer(20);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_LINE;
								head.p = pDoc->m_pLine_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//-------------------------------------
								pDoc->Save_EMMLine(&oneline);
								CloseMouseCursor();
								DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
								OpenMouseCursor();
							}
						}
					}
				}
				p1 += 16;
			}
		}
	
		p1 = position2;
		for(i=0; i<linenum2; i++)
		{	
			abcline1 = (struct Line*)p1;
			if((abcline1->attrib == PCB)&&(abcline1->flag == 1))
			{
				flag = 0;
				p2 = p1;
				for(ii = i; ii<linenum2; ii++)
				{	
					abcline2 = (struct Line*)p2;
					if((abcline2->attrib == PCB)&&(abcline2->flag == 1))
					{
						if(abcline2->startx == abcline1->startx){}
						else if(abcline2->startx == (abcline1->startx + trackwidth - 1))
						{
							if(abs(abcline2->starty - abcline1->starty) <= p)//trackwidth)
							{
								oneline.flag = 2;
								oneline.startx = abcline1->startx;
								oneline.starty = abcline1->starty;
								oneline.endx = abcline2->startx;
								oneline.endy = abcline2->starty;
								//------------- Undo -----------------
								undoflag = 1;
								Check_Undo_Buffer(20);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_LINE;
								head.p = pDoc->m_pLine_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//-------------------------------------
								pDoc->Save_EMMLine(&oneline);
								CloseMouseCursor();
								DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
								OpenMouseCursor();
							}
							if(abs(abcline2->endy - abcline1->endy) <= p) //trackwidth)
							{
								oneline.flag = 2;
								oneline.startx = abcline1->endx;
								oneline.starty = abcline1->endy;
								oneline.endx = abcline2->endx;
								oneline.endy = abcline2->endy;
								//------------- Undo -----------------
								undoflag = 1;
								Check_Undo_Buffer(20);
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_LINE;
								head.p = pDoc->m_pLine_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//-------------------------------------
								pDoc->Save_EMMLine(&oneline);
								CloseMouseCursor();
								DrawStructLine(&oneline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
								OpenMouseCursor();
							}
						}
						else break;
					}
					p2 += 16;
					if(flag>1) break;
				}		
			}
			p1 += 16;
		}
	}

	//----------- Undo -----------
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------

	//充填完成

	//处理网格充填
	undoflag = 0;
	if(mode == 4)
	{
		p1 = position;
		j = 0;
		for(i=0; i<linenum; i++)
		{	
			abcline1 = (struct Line*)p1;
			if((abcline1->attrib == PCB)&&(abcline1->flag == 1))
			{
				if(abcline1->starty >= y2) break;
				if(j == 0) j = abcline1->starty;
				else if(j == abcline1->starty) {}
				else if(abcline1->starty < (j + fd*(trackwidth-1)))
				{
					abcline1->flag = 0xff;
					//----------- Undo -----------
					undoflag = 1;
					Check_Undo_Buffer(10);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_UNDELETE;
					head.ob = UNDO_LINE;
					head.p = p1;
					memcpy(undo_p,&head,6);
					pDoc->m_pPCB_Undo_Current += 6;
					//---------------------------------------------------------------------------
				}
				else j = abcline1->starty;
			}
			p1 += 16;
		}
		
		p1 = position2;
		j = 0;
		for(i=0; i<linenum2; i++)
		{	
			abcline1 = (struct Line*)p1;
			if((abcline1->attrib == PCB)&&(abcline1->flag == 1))
			{
				if(abcline1->startx >= x2) break;
				if(j == 0) j = abcline1->startx;
				else if(j == abcline1->startx) {}
				else if(abcline1->startx < (j + fd*(trackwidth-1)))
				{
					abcline1->flag = 0xff;
					//----------- Undo -----------
					undoflag = 1;
					Check_Undo_Buffer(10);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_UNDELETE;
					head.ob = UNDO_LINE;
					head.p = p1;
					memcpy(undo_p,&head,6);
					pDoc->m_pPCB_Undo_Current += 6;
					//---------------------------------------------------------------------------
				}
				else j = abcline1->startx;
			}
			p1 += 16;
		}
		Invalidate();
	}

	//----------- Undo -----------
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------
	Block_Line_Recover();


	DrawPCBHighlight(0);
	Free_PCBHighlightbuf();

	//优化充填，自动去除残线
	position2 = pDoc->m_pLine_Buffer_Temp;
	int totallinenum = (position2 - position)/16;
	//char ccc[100];
	//sprintf(ccc, "%d", totallinenum);
	//AfxMessageBox(ccc);

	char *buf, *temp_p1, *temp_p2, *temptrackbuf;
	struct htrack
	{ 
		char* pointer;             } highlighttrack;
	
	undoflag = 0;
	if((optimize != 0)&&(PCBHighlightBufferInit() == 0)&&((buf = (char*)malloc(5000)) != NULL))
	{
		temp_p1 = position;
		j = 0;
		delta = 0;
		for(;;)
		{
			abcline1 = (struct Line*)temp_p1;
			if((abcline1->flag == 0xff)||(abcline1->flag == 0x01))
			{
				temp_p1 += 16;
				j++;
				if((temp_p1 == position2)||(j == totallinenum))  break;
				continue;
			}

			highlighttrack.pointer = temp_p1;
			memcpy(TempTrackbuf,&highlighttrack,4);
			TempTrackbuf += 4;
			CloseMouseCursor();
			PCBMakeHighlight();
			OpenMouseCursor();

			DrawPCBHighlight(0);
			
			temptrackbuf = Highlight_Trackbuf;
			for(i=0;i<((int)(HL_lnum/4));i++)
			{
				memcpy(&highlighttrack,temptrackbuf,4);
				if(highlighttrack.pointer == NULL) break;

				abcline1 = (struct Line*)highlighttrack.pointer;
				if(abcline1->flag == 0) abcline1->flag = 1;
				temptrackbuf+=4;
			}

			if(i < filllimit)
			{
				if(delta == 0)
				{
					memcpy(buf, Highlight_Trackbuf, i*4);
					ii = i;
				}
				else if(delta > i)
				{
					temp_p2 = Highlight_Trackbuf;
					for(int kk=0; kk<i; kk++)
					{
						memcpy(&highlighttrack, temp_p2, 4);
						if(highlighttrack.pointer >= position)
						{
							abcline2 = (struct Line*)highlighttrack.pointer;
							abcline2->flag = 0xff;
							// undo -------------------------
							undoflag = 1;
							Check_Undo_Buffer(10);
							undo_p = pDoc->m_pPCB_Undo_Current;
							head.op = UNDO_UNDELETE;
							head.ob = UNDO_LINE;
							head.p = highlighttrack.pointer;
							memcpy(undo_p,&head,6);
							pDoc->m_pPCB_Undo_Current += 6;
							// ------------------------------
						}
						temp_p2 += 4;
					}
				}
				else
				{
					// 删除buf中记录的线
					temp_p2 = buf;
					for(int kk=0; kk<ii; kk++)
					{
						memcpy(&highlighttrack, temp_p2, 4);
						if(highlighttrack.pointer >= position)
						{
							abcline2 = (struct Line*)highlighttrack.pointer;
							abcline2->flag = 0xff;
							// undo -------------------------
							undoflag = 1;
							Check_Undo_Buffer(10);
							undo_p = pDoc->m_pPCB_Undo_Current;
							head.op = UNDO_UNDELETE;
							head.ob = UNDO_LINE;
							head.p = highlighttrack.pointer;
							memcpy(undo_p,&head,6);
							pDoc->m_pPCB_Undo_Current += 6;
							// ------------------------------
						}
						temp_p2 += 4;
					}

					// 将当前记录复制到buf
					memcpy(buf, Highlight_Trackbuf, i*4);
					ii = i;
				}
			}
			
			if(delta < i) delta = i;

			for(i=0; i<HL_lnum;i++) Highlight_Trackbuf[i]   = 0;
			for(i=0; i<HL_pnum; i++) Highlight_Padbuf[i]     = 0;
			for(i=0; i<HL_afnum; i++) Highlight_ArcFillbuf[i] = 0;
			for(i=0; i<HL_packnum; i++) Highlight_Packagebuf[i] = 0;

			TempTrackbuf = Highlight_Trackbuf;
			TempPadbuf = Highlight_Padbuf;
			TempArcFillbuf = Highlight_ArcFillbuf;
			TempPackagebuf = Highlight_Packagebuf;
			
			temp_p1 += 16;
			j++;
			if((temp_p1 >= position2)||(j >= totallinenum))  break;
		}
	}
	
	//----------- Undo -----------
	if(undoflag != 0)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num=0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//---------------------------------------------------------------------------
	Block_Line_Recover();

	free(buf);
	Free_PCBHighlightbuf();

	DrawMyCursor(&dc,&pp);
	DRC_T_T = t_t;
	DRC_T_P = t_p;
	DRC_P_P = p_p;
	CL_Auto_Pan = OFF;
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	Write_Help(commandend);
	EnableFrame(TRUE);
	Invalidate();
}

void CDesignView::OnOptimizeTrack() 
{
	int i,num, flag;
	int ii;
	char *p, *pp;
	struct Line *abcline, *otherline;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	Write_Help("走线优化...");
	// 删除长度为零的走线带
	num = 0;
	p = pDoc->m_pLine_Buffer;
	for(i=0; i<pDoc->m_nLine_Number; i++)
	{	
		abcline = (struct Line*)p;
		if((abcline->flag==0)&&(abcline->attrib==PCB)&&\
			(abcline->startx==abcline->endx)&&(abcline->starty==abcline->endy))
		{
			abcline->flag=0xff;
			num++;
		}
		p+=16;
	}

	// 删除被覆盖的走线带
	p = pDoc->m_pLine_Buffer;
	for(i=0; i<pDoc->m_nLine_Number; i++)
	{	
		abcline = (struct Line*)p;
		if((abcline->flag==0)&&(abcline->attrib==PCB))
		{
			flag=0;
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				otherline = (struct Line*)pp;
				if((otherline->attrib==abcline->attrib)&&\
					(otherline->flag==0)&&(otherline->layer==abcline->layer)&&\
					(p != pp))
				{
					if((otherline->startx==otherline->endx)&&(abcline->startx==abcline->endx)&&(otherline->endx==abcline->endx)&&\
						(MAX(otherline->starty,otherline->endy)>=MAX(abcline->starty,abcline->endy))&&\
						(MIN(otherline->starty,otherline->endy)<=MIN(abcline->starty,abcline->endy))&&\
						(otherline->linewidth>=abcline->linewidth))
					{
						flag=1;
						num++;
					}
					else if((otherline->starty==otherline->endy)&&(abcline->starty==abcline->endy)&&(otherline->endy==abcline->endy)&&\
						(MAX(otherline->startx,otherline->endx)>=MAX(abcline->startx,abcline->endx))&&\
						(MIN(otherline->startx,otherline->endx)<=MIN(abcline->startx,abcline->endx))&&\
						(otherline->linewidth>=abcline->linewidth))
					{
						flag=1;
						num++;
					}
					if(flag==1) 
					{
						break; 
					}
				}
				pp+=16;
			}	
			abcline = (struct Line*)p;
			if(flag==1)
			{
				flag=0;
				abcline->flag=0xff; 
			}
		}
		p+=16;
	}

	// 连接的走线带合二为一
TEMP:
	flag=0;
	p = pDoc->m_pLine_Buffer;
	for(i=0; i<pDoc->m_nLine_Number; i++)
	{	
		abcline = (struct Line*)p;
		if((abcline->flag==0)&&(abcline->attrib==PCB))
		{
			pp = pDoc->m_pLine_Buffer;
			for(ii=0; ii<pDoc->m_nLine_Number; ii++)
			{	
				otherline = (struct Line*)pp;
				if((otherline->attrib==abcline->attrib)&&(otherline->linewidth==abcline->linewidth)&&\
					(otherline->flag==0)&&(otherline->layer==abcline->layer)&&\
					(p != pp))
				{
					if((otherline->startx==otherline->endx)&&(abcline->startx==abcline->endx)&&(otherline->endx==abcline->endx)&&\
						(MAX(otherline->starty,otherline->endy)>=MAX(abcline->starty,abcline->endy))&&\
						(MIN(otherline->starty,otherline->endy)<=MAX(abcline->starty,abcline->endy))&&\
						(MIN(otherline->starty,otherline->endy)>=MIN(abcline->starty,abcline->endy)))
					{
						otherline->starty = MAX(otherline->starty, otherline->endy);
						otherline->endy   = MIN(abcline->starty, abcline->endy);
						abcline->flag = 0xff;
						num++;
						flag = 1;
						break;
					}
					else if((otherline->starty==otherline->endy)&&(abcline->starty==abcline->endy)&&(otherline->endy==abcline->endy)&&\
						(MAX(otherline->startx,otherline->endx)>=MAX(abcline->startx,abcline->endx))&&\
						(MIN(otherline->startx,otherline->endx)<=MAX(abcline->startx,abcline->endx))&&\
						(MIN(otherline->startx,otherline->endx)>=MIN(abcline->startx,abcline->endx)))
					{
						otherline->startx = MAX(otherline->startx, otherline->endx);
						otherline->endx   = MIN(abcline->startx, abcline->endx);
						abcline->flag = 0xff;
						num++;
						flag = 1;
						break;
					}
				}
				pp+=16;
			}
			/*if(flag == 1)
			{
				i = -1;
				p = pDoc->m_pLine_Buffer;
				continue;
			}*/
		}
		p+=16;
	}
	if(flag == 1)
	{
		goto TEMP;
	}

	// 删除小的皱折走线带

	// 走线带自动导45度角

	char str[20],string[50];
	itoa(num,str,10);
	sprintf(string,"%s%s",str,"走线被移除.");
	AfxMessageBox(string, MB_OK|MB_ICONINFORMATION);
	Write_Help(commandend);
}
