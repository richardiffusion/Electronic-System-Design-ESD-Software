void CDesignView::Show_Ratsnest(CDC* pDC)
{
	int i , ratsnestnum;
	char *route_ratsnest;
	char str[20];
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	Ratsnest_Length = 0;
	
	if(Ratsnestflag == 0) return;
	
	if((route_ratsnest = (char*)malloc(130000))==NULL) 
	{
		memory_error();
		return;
	}

	if(Route_Mode_Change!=0)
    { 
		i = Route_Mode;
		Route_Mode = Old_Route_Mode;
		ratsnestnum = pDoc->Get_All_Ratsnest(route_ratsnest);
		Draw_All_Ratsnest(route_ratsnest,ratsnestnum,pDC);
		Route_Mode_Change = 0;
		Route_Mode = i;
    }
	else 
	{
		ratsnestnum = pDoc->Get_All_Ratsnest(route_ratsnest);
		Draw_All_Ratsnest(route_ratsnest,ratsnestnum,pDC);
	}
	if(Show_Ratsnest_Lenght_Flag!=0)
    { 
		ltoa(Ratsnest_Length,str,10);
		Write_Help(str);
    }
	free(route_ratsnest);
}

UINT CDesignView::Draw_All_Ratsnest(char * route_ratsnest, int ratsnestnum, CDC* pDC)
{
	int i;
	char *temp_ratsnest;
	unsigned long totallength;
	struct Route_Rats route;

	//CClientDC dc(this);

	if(Ratsnestflag == 0) return 0;

	temp_ratsnest = route_ratsnest;
	totallength=0;
	for(i=0;i<ratsnestnum;i++)
    { 
		memcpy(&route,temp_ratsnest,13);
		temp_ratsnest+=13;
		if(route.state==0)
		{
			DrawAnyLine(route.sx,route.sy,route.ex,route.ey,Highlighted_Color,1,pDC);
	       /*totallength+=sqrt(((float)route.sx-route.ex)*((float)route.sx-route.ex)+\
				 ((float)route.sy-route.ey)*((float)route.sy-route.ey));*/
		}
    }
	return totallength;
}

//  Ratsnestflag = 0 hide                
//  Ratsnestflag = 1 show all            
//  Ratsnestflag = 2 show onenet         
//  Ratsnestflag = 3 show net on package 

void CDesignView::OnAutoRatsAll() 
{
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Number <= 0) return;
	if(Ratsnestflag != 0) Show_Ratsnest(&dc);
	Ratsnestflag = 1;
	pDoc->Fresh_Package_Table(1,Null_Str);
	Show_Ratsnest(&dc);
}

void CDesignView::OnAutoRatsHideAll() 
{
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(Ratsnestflag != 0)
    {
		Show_Ratsnest(&dc);
		pDoc->Fresh_Package_Table(4,Null_Str);
		Ratsnestflag = 0;
    }
}

void CDesignView::OnAutoRatsNet() 
{
	int i,j;
	char **nets, *temp;
	char str[60];
	
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Number == 0) 
	{
		Error( error16 );
	    return;
	}
	
	// 定义网络名称
	CTextInput hzin;
	hzin.m_MaxLength = 32;
	hzin.m_Title = gnhelpstr40;
	hzin.m_Text_Input = Null_Str;

	Write_Help("显示某个网络的飞线");
	i = hzin.DoModal();
	if(i == IDCANCEL)
	{
		Write_Help(commandend);
		return;
	}
	strcpy(str,hzin.m_Text_Input);

	if((nets = (char**)malloc(16000))==NULL)
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
		temp += strlen(temp) + 1;
	}
	if(j == 0)
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
	
	if(Ratsnestflag != 0) Show_Ratsnest(&dc);
	Ratsnestflag = 2;
	strcpy(Ratsneststring,str);
	pDoc->Fresh_Package_Table(Ratsnestflag,str);
	Show_Ratsnest(&dc);
	free(nets);
	Write_Help(commandend);
}

void CDesignView::OnAutoRatsHideNet() 
{
	int i,j;
	char **nets, *temp;
	char str[60];

	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Number==0)
	{
		Error( error16 );
	    return;
	}
	strcpy(str,Null_Str);
	// 定义网络名称
	CTextInput hzin;
	hzin.m_MaxLength = 32;
	hzin.m_Title = gnhelpstr40;
	hzin.m_Text_Input = Null_Str;

	Write_Help("隐藏某个网络的飞线");
	i = hzin.DoModal();
	if(i == IDCANCEL) return;
	strcpy(str,hzin.m_Text_Input);


	if((nets = (char**)malloc(16000))==NULL)
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
		temp += strlen(temp) + 1;
	}
	if(j == 0)
    {
		CSelectionDlg selection;
		selection.m_TitleText = select_net;
		selection.m_pList = nets;
		selection.m_nNum = pDoc->m_nNet_Number;
		i = selection.DoModal();
		if(i != IDOK) 
		{
			free(nets);
			return ;     
		}
		strcpy(str, selection.m_Selected_Text);
    }
	
	if(Ratsnestflag != 0) Show_Ratsnest(&dc);
	Ratsnestflag = 5;
	strcpy(Ratsneststring,str);
	pDoc->Fresh_Package_Table(5,str);
	Show_Ratsnest(&dc);
	free(nets);	
}

void CDesignView::OnAutoRatsPackage() 
{
	int i;
	char **id, *temp;
	char str[200];
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Compnum <= 0) return;
	if((id = (char**)malloc(64000))==NULL)
	{
		memory_error();
		return;
	}
	
	temp = pDoc->m_pNet_Compbuf;
	for(i=0;i<pDoc->m_nNet_Compnum;i++)
	{
		id[i] = temp;
		//if((strlen(temp)+1)>maxlen) maxlen=strlen(temp)+1;
		temp+=strlen(temp)+1;
		temp+=strlen(temp)+1;
		temp+=strlen(temp)+1;
	}

	// 定义元件名称
	CTextInput hzin;
	hzin.m_MaxLength = 16;
	hzin.m_Title = gnhelpstr39;
	hzin.m_Text_Input = Null_Str;

	Write_Help("显示某个元件的飞线");
	i = hzin.DoModal();
	if(i == IDCANCEL)
	{
		free(id);
		Write_Help(commandend);
		return;
	}
	strcpy(str,hzin.m_Text_Input);

	//if(ASCII_Input(str,gnhelpstr39,Null_Str,16)==ESC) { free(id); return; }
	for(i=0;i<pDoc->m_nNet_Compnum;i++)
	{
		if(stricmp(str,id[i])==0) break;
	}
	if(i == pDoc->m_nNet_Compnum)
	{
		Error(error7);
		CSelectionDlg selection;
		selection.m_TitleText = select_package;
		selection.m_pList = id;
		selection.m_nNum = pDoc->m_nNet_Compnum;
		i = selection.DoModal();
		if(i != IDOK) 
		{
			free(id);
			Write_Help(commandend);
			return ;     
		}
		strcpy(str, selection.m_Selected_Text);
		//if(SortString(str,select_package,64/maxlen,id,Net_Compnum)==ESC) 
		//{
			//free(id); 
			//return; 
		//}
	}
	
	if(Ratsnestflag!=0) Show_Ratsnest(&dc);
	pDoc->Fresh_Ratsnest_Package(str);
	strcpy(Ratsneststring,str);
	Show_Ratsnest(&dc);
	free(id);
	Write_Help(commandend);
}

void CDesignView::OnAutoRatsHidePackage() 
{
	int i,ch1,m,n;
	short int k;
	char **id, *temp;
	char *tempNet_Pins, *tempNetname;
	char str[200], netname[60];
	char str1[30],strid1[30],strnum1[10];
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Compnum <= 0) return;
	
	if((id = (char**)malloc(64000))==NULL)
	{
		memory_error();
		return;
	}
	
	temp = pDoc->m_pNet_Compbuf;
	for(i=0;i<pDoc->m_nNet_Compnum;i++)
	{
		id[i]=temp;
		temp += strlen(temp) + 1;
		temp += strlen(temp) + 1;
		temp += strlen(temp) + 1;
	}
	// 定义元件名称
	CTextInput hzin;
	hzin.m_MaxLength = 16;
	hzin.m_Title = gnhelpstr39;
	hzin.m_Text_Input = Null_Str;

	Write_Help("隐藏某个元件的飞线");
	i = hzin.DoModal();
	if(i == IDCANCEL)
	{
		free(id);
		Write_Help(commandend);
		return;
	}
	strcpy(str,hzin.m_Text_Input);
	
	//if(ASCII_Input(str,gnhelpstr39,Null_Str,16)==ESC) { free(id); return; }
	for(i=0;i<pDoc->m_nNet_Compnum;i++)
	{
		if(stricmp(str,id[i])==0) break;
	}
	
	if(i == pDoc->m_nNet_Compnum)
	{
		Error(error7);
		CSelectionDlg selection;
		selection.m_TitleText = select_package;
		selection.m_pList = id;
		selection.m_nNum = pDoc->m_nNet_Compnum;
		i = selection.DoModal();
		if(i != IDOK) 
		{
			free(id);
			Write_Help(commandend);
			return ;     
		}
		strcpy(str, selection.m_Selected_Text);
		//if(SortString(str,select_package,64/maxlen,id,Net_Compnum)==ESC) 
		//{
			//free(id); 
			//return; 
		//}
	}
	
	if(Ratsnestflag!=0) Show_Ratsnest(&dc);
	Ratsnestflag = 2;
	tempNetname = pDoc->m_pNet_Netnamebuf;
	for(ch1=0;ch1<pDoc->m_nNet_Number;ch1++)
	{
		memcpy(netname,tempNetname,(strlen(tempNetname)+1));
		tempNetname+=strlen(tempNetname)+1;
		tempNet_Pins = pDoc->m_ppNet_Pinspointer[ch1];
		for(;;)
		{
			if(tempNet_Pins == pDoc->m_ppNet_Pinspointer[ch1+1]) break;
			memcpy(str1,tempNet_Pins,strlen(tempNet_Pins)+1);
			tempNet_Pins+=strlen(tempNet_Pins)+1;
			memcpy(&k,tempNet_Pins,2);
			tempNet_Pins+=2;
			if((k>0)&&(k<10000)) continue;

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
			if(stricmp(strid1,str)==0)
			{
				pDoc->Fresh_Package_Table(5,netname);
				break;
			}
		}
	}

	strcpy(Ratsneststring,str);
	Show_Ratsnest(&dc);
	free(id);
	Write_Help(commandend);
}

void CDesignView::OnAutoRatsRefresh() 
{
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	Ratsnest_Refresh(0);
	Write_Help(commandend);
}

void CDesignView::Ratsnest_Refresh(int state)
{
	int i,num,m,n,l,subnetnum;
	short int k;
	int ch1;
	char *pointer, *temp, *temp1, *temp2, *packmem, *p;
	char *temptrackbuf, *temppadbuf, *temparcfillbuf, *temppackbuf;
	struct htrack
	{ 
		char* pointer;             } highlighttrack;
	struct hpack
	{
		short int padnum;
		char* pointer;    } highlightpack;
	struct Line oneline;
	char str1[30],str2[10];
	char str[60], strbak[60];
	char hidenest[2]={'1','\0'}, shownest[2]={'0','\0'}, midnest[2]={'2','\0'};

	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Number==0) 
	{
		Error( error16 );
		return;
	}

	Write_Help("飞线刷新...");
	
	if(state==0)
	{
		if(PCBHighlightBufferInit() != 0) return;
	}
	else DrawPCBHighlight(0);
	temptrackbuf = Highlight_Trackbuf;
	temppadbuf = Highlight_Padbuf;
	temparcfillbuf = Highlight_ArcFillbuf;
	temppackbuf = Highlight_Packagebuf;

	if((packmem = (char*)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}

	// clear net connection 
	for(ch1=0;ch1<pDoc->m_nNet_Number;ch1++)
    {
		temp1 = pDoc->m_ppNet_Pinspointer[ch1];
		for(;;)
		{
			if(temp1 == pDoc->m_ppNet_Pinspointer[ch1+1]) break;
			temp1+=strlen(temp1)+1;
			k=0;
			memcpy(temp1,&k,2);
			temp1+=2;
		}
    }
	for(ch1=0;ch1<pDoc->m_nNet_Number;ch1++)
	{
	   temp1 = pDoc->m_ppNet_Pinspointer[ch1];
	   subnetnum=0;
	   for(;;)
	   {
			if(temp1 == pDoc->m_ppNet_Pinspointer[ch1+1]) break;
			memcpy(str,temp1,strlen(temp1)+1);
			temp1+=strlen(temp1)+1;
			memcpy(&k,temp1,2);
			if(k!=0)
			{
				temp1+=2;
				continue;
			}
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
					temp = packmem;
					memcpy(temp,pointer,gnpackage.total16byte*16);
					temp = packmem;
					memcpy(&gnpackage,temp,112);
					if(strcmp(gnpackage.ID,str1)==0)
					{
						temp+=112;
						for(l=0;l<gnpackage.padnum;l++)
						{
							memcpy(&gnjunction,temp,16);
							temp+=16;
							if(strcmp((char*)gnjunction.padname,str2)==0)
							{   
								temppackbuf=Highlight_Packagebuf;
								for(;;)
								{  
									memcpy(&highlightpack,temppackbuf,6);
									if((highlightpack.pointer==p)&&(highlightpack.padnum==l)) break;
									if(highlightpack.pointer == NULL)
									{
										highlightpack.pointer = p;
										highlightpack.padnum=l;
										memcpy(TempPackagebuf,&highlightpack,6);
										TempPackagebuf += 6;
										goto TEMPA;
									}
									temppackbuf+=6;
								}
							}
						}
					}
				}
				p += gnpackage.total16byte*16;
			}
TEMPA:
			PCBMakeHighlight();
			DrawPCBHighlight(0);
			if((int)(((long)TempPackagebuf-(long)Highlight_Packagebuf)/6)>1)
			{
				subnetnum++;
				temppackbuf=Highlight_Packagebuf;
				for(;;)
				{
					memcpy(&highlightpack,temppackbuf,6);
					if(highlightpack.pointer == NULL) break;
					else
					{
						pointer = highlightpack.pointer;
						memcpy(&gnpackage,pointer,16);
						num=gnpackage.total16byte;
						temp=packmem;
						memcpy(temp,pointer,gnpackage.total16byte*16);
						temp = packmem;
						memcpy(&gnpackage,temp,112);
						temp+=112;
						strcpy(str1,gnpackage.ID);
						for(l=0;l<gnpackage.padnum;l++)
						{
							memcpy(&gnjunction,temp,16);
							temp+=16;
							if(l==highlightpack.padnum)
							{
								strcpy(str2,(char*)gnjunction.padname);
								sprintf(str,"%s%s%s",str1,"-",str2);
								temp2 = pDoc->m_ppNet_Pinspointer[ch1];
								for(;;)
								{ 
									if(temp2 == pDoc->m_ppNet_Pinspointer[ch1+1]) break;
									memcpy(strbak,temp2,strlen(temp2)+1);
									temp2+=strlen(temp2)+1;
									if(strcmp(str,strbak)==0)
									{
										k=32000;
										memcpy(temp2,&k,2);
									}
									temp2+=2;
								}
								break;
							}
						}
					}
					temppackbuf+=6;
				}
				
				temp2 = pDoc->m_ppNet_Pinspointer[ch1];	
				for(;;)
				{
					if(temp2 == pDoc->m_ppNet_Pinspointer[ch1+1]) break;
					temp2+=strlen(temp2)+1;
					memcpy(&k,temp2,2);
					if(k==32000) 
					{
						temp=temp2;
						memcpy(temp2,&subnetnum,2);
				    }
					temp2+=2;
				}
				k=subnetnum+10000;
				memcpy(temp,&k,2);
			}
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
			for(i=0;i<HL_lnum;i++) Highlight_Trackbuf[i]=0;
			for(i=0;i<HL_pnum;i++) Highlight_Padbuf[i]=0;
			for(i=0;i<HL_afnum;i++) Highlight_ArcFillbuf[i]=0;
			for(i=0;i<HL_packnum;i++) Highlight_Packagebuf[i]=0;
			TempTrackbuf=Highlight_Trackbuf;
			TempPadbuf=Highlight_Padbuf;
			TempArcFillbuf=Highlight_ArcFillbuf;
			TempPackagebuf=Highlight_Packagebuf;
		}
	}

	p = pDoc->m_pLine_Buffer;
	for(i=0; i<pDoc->m_nLine_Number; i++)
	{	
		memcpy(&gnline,p,16);
		if((gnline.attrib==PCB)&&(gnline.flag==0xa0)\
			&&(gnline.layer<=BOTTOMLAYER))
		{
			gnline.flag=0;
			memcpy(p,&gnline,16);
		}
		p+=16;
	}
	
	if(state==0)
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
	
	else Ratsnestflag = 0;
	free(packmem);
	pDoc->Get_Package_Table();
	Write_Help(commandend);

	if(Ratsnestflag == 1)
	{
		pDoc->Fresh_Package_Table(1,Null_Str);
	}
	Invalidate();

}
void CDesignDoc::Fresh_Package_Table(int mode, char * string)
{
	int i,m,n,ch1;
	char *temp,*p;
	char *tempNet_Pins, *tempNetname;
	char str[60],str1[30],strid1[30],strnum1[10];
	struct Package *onepackage;
	struct Junction *onejunction;
	struct tablestruct 
	{
		char* pointer; 
		unsigned char flag; } tablestruct;

	m_ptemp_Package_Table = m_pPackage_Table;
	tempNetname = m_pNet_Netnamebuf;
	if(m_nNet_Number == 0) return;
	
	for(ch1=0;ch1<m_nNet_Number;ch1++)
	{
		// str = 网络名称
		memcpy(str,tempNetname,(strlen(tempNetname)+1));
		tempNetname += strlen(tempNetname) + 1;
		tempNet_Pins = m_ppNet_Pinspointer[ch1];
		for(;;)
		{
			// str1 = 元件名称-引脚编号
			memcpy(str1,tempNet_Pins,strlen(tempNet_Pins)+1);
			tempNet_Pins+=strlen(tempNet_Pins)+1;
			tempNet_Pins+=2;
	
			n=strlen(str1);
			for(m=n;m>=0;m--)
			{
				//分解str1 --->  strid1 = 元件名称  strnum = 引脚编号
				if(str1[m]=='-')
				{
					memcpy(strid1,str1,m);
					strid1[m]='\0';
					memcpy(strnum1,str1+m+1,n-m-1);
					strnum1[n-m-1]='\0';
					break;
				}
			}
			
			//mode = 1 show all 
			//     = 2 show net, string = netname
			//     = 4 hide all
			//     = 5 hide net, string = netname
			if(((mode==2)||(mode==5))&&(strcmp(str,string)!=0))
			{
				m_ptemp_Package_Table+=5;
				if(tempNet_Pins == m_ppNet_Pinspointer[ch1+1]) break;
				continue;
			}

			p = m_pPackage_Buffer;
			for(i=0; i<m_nPackage_Number; i++)
			{
				onepackage = (struct Package*)p;
				if(onepackage->flag==0)
				{
					if((strcmp(onepackage->ID,strid1)==0)&&\
					((onepackage->cx!=12345)||(onepackage->cy!=12345)))
					{
						temp = p;
						temp+=112;
						for(m=0;m<onepackage->padnum;m++)
						{
							onejunction = (struct Junction*)temp;
							if(strcmp((char*)onejunction->padname,strnum1)==0)
							{
								memcpy(&tablestruct,m_ptemp_Package_Table,5);
								tablestruct.pointer = temp;
								if(mode==1) tablestruct.flag = 1;
								else if(mode==2)
								{ 
									if(strcmp(str,string)==0) tablestruct.flag = 1;
								}
								if(mode==4) tablestruct.flag=0;
								else if(mode==5)
								{ 
									if(strcmp(str,string)==0) tablestruct.flag = 0;
								}
								memcpy(m_ptemp_Package_Table,&tablestruct,5);
								m_ptemp_Package_Table+=5;
								goto LIOUT;
							}
							temp+=16;
						}
					}
				}
				p += onepackage->total16byte*16;
			}
			
			tablestruct.pointer = NULL;
			tablestruct.flag = 0;
			memcpy(m_ptemp_Package_Table,&tablestruct,5);
			m_ptemp_Package_Table += 5;
LIOUT:
			if(tempNet_Pins == m_ppNet_Pinspointer[ch1+1]) break;
		}
	}    
}
//	struct tablestruct 
//	{
//		char* pointer; 
//		unsigned char flag; } tablestruct;

void CDesignDoc::Fresh_Ratsnest_Package(char * str)
{
	int ch1,m,n;
	short int k;
	char *tempNet_Pins, *tempNetname;
	char netname[60];
	char str1[30],strid1[30],strnum1[10];

	Ratsnestflag = 2;
	tempNetname = m_pNet_Netnamebuf;
	for(ch1=0;ch1<m_nNet_Number;ch1++)
	{
		memcpy(netname,tempNetname,(strlen(tempNetname)+1));
		tempNetname += strlen(tempNetname)+1;
		tempNet_Pins = m_ppNet_Pinspointer[ch1];
		for(;;)
		{
			if(tempNet_Pins == m_ppNet_Pinspointer[ch1+1]) break;
			memcpy(str1,tempNet_Pins,strlen(tempNet_Pins)+1);
			tempNet_Pins+=strlen(tempNet_Pins)+1;
			memcpy(&k,tempNet_Pins,2);
			tempNet_Pins+=2;
			if((k>0)&&(k<10000)) continue;

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
			if(stricmp(strid1,str)==0)
			{
				Fresh_Package_Table(2,netname);
				break;
			}
		}
	}
}

void CDesignDoc::Get_Package_Table()
{
	int i,m,n,ch1;
	char *temp, *p;
	char *tempNet_Pins;
	char str1[30],strid1[30],strnum1[10];
	struct Package *onepackage;
	struct Junction *onejunction;
	struct tablestruct 
	{
		char* pointer; 
		unsigned char flag; } tablestruct;
	char *package_table;


	package_table = m_pPackage_Table;
	if(m_nNet_Number == 0) return;

	//扫描所有网络
	for(ch1=0; ch1<(int)m_nNet_Number;ch1++)
	{
		tempNet_Pins = m_ppNet_Pinspointer[ch1];
		//每个网络中，有x个Rm-n节点指示
		for(;;)
		{
			//AfxMessageBox(tempNet_Pins);
			memcpy(str1,tempNet_Pins,strlen(tempNet_Pins)+1);
			tempNet_Pins += strlen(tempNet_Pins)+1;
			tempNet_Pins += 2;
			n = strlen(str1);

			//分割Rm-n, strid1=Rm, strnum1=n
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

			p = m_pPackage_Buffer;
			//扫描所有封装, 找到节点Rm-n对应的封装
			for(i=0; i<m_nPackage_Number; i++)
			{
				onepackage = (struct Package*)p;
				if(onepackage->flag==0)
				{
					if(strcmp(onepackage->ID,strid1)==0)
					{
						temp = p;
						temp += 112;
						for(m=0; m<onepackage->padnum; m++)
						{
							onejunction = (struct Junction*)temp;
							if(strcmp((char*)onejunction->padname,strnum1)==0)
							{
								tablestruct.pointer = temp;
								tablestruct.flag = 0;
								memcpy(package_table,&tablestruct,5);
								package_table += 5;
								goto LIOUT;
							}
							temp += 16;
						}
					}
				}
				p += onepackage->total16byte*16;
			}

			tablestruct.pointer = NULL;
			tablestruct.flag = 0;
			memcpy(package_table,&tablestruct,5);
			package_table += 5;
LIOUT:
			if(tempNet_Pins == m_ppNet_Pinspointer[ch1+1]) break;
		}
	}
}

UINT CDesignDoc::Get_All_Ratsnest(char * route_ratsnest)
{
	//int i,j,k,n,m;
	int ch1,ratsnestnum;
	char *temp_ratsnest, *tempNet_Pins;

	ratsnestnum = 0;
	temp_ratsnest = route_ratsnest;
	m_ptemp_Package_Table = m_pPackage_Table;
	for(ch1=0;ch1<m_nNet_Number;ch1++)
	{
		tempNet_Pins = m_ppNet_Pinspointer[ch1];
		temp_ratsnest = route_ratsnest + ratsnestnum*13;
		ratsnestnum += Get_Onenet_Ratsnest(temp_ratsnest,ch1);
	}
	return ratsnestnum;
}

int CDesignDoc::Get_Onenet_Ratsnest(char * ratsnest, int netnum)
{
	int i,j,m,ratsnum,nodenum,flag;
	short int k;
	char *temp,*packagemem;
	char *tempNet_Pins;
	char *routejunc, *tempjunc;
	char str1[30],str2[30];
	unsigned tempx1,tempy1,tempx2,tempy2;
	unsigned char templ1,templ2;
	unsigned long templength;
	int mainnum1,mainnum2,mlength;
	int subnum1,subnum2,slength;
	struct Junction *onejunction;
	struct Route_Rats route;
	struct Node
    { 
		unsigned short cx,cy;
		unsigned char layer,flag,state;  } node, *node1, *node2;
	short int *distancebuf;
	struct tablestruct 
	{
		char* pointer; 
		unsigned char flag; } tablestruct;

	if((packagemem=(char *)malloc(16384))==NULL)
	{
		memory_error();
		return 0;
	}
	if((routejunc=(char *)malloc(10000))==NULL)
	{
		memory_error();
		return 0;
	}
	tempjunc=routejunc;
	ratsnum=0;
	nodenum=0;

	if(Route_Mode == ROUTESEQUENCE)
	{
		tempNet_Pins = m_ppNet_Pinspointer[netnum];
		for(;;)
		{
			memcpy(str1,tempNet_Pins,strlen(tempNet_Pins)+1);
			tempNet_Pins += strlen(tempNet_Pins) + 1;
			memcpy(&k,tempNet_Pins,2);
			memcpy(&tablestruct,m_ptemp_Package_Table,5);
			m_ptemp_Package_Table += 5;
			if((k>0)&&(k<10000))
			{
				tempNet_Pins+=2;
				continue;
			}
			tempNet_Pins+=2;

			if(tempNet_Pins == m_ppNet_Pinspointer[netnum+1]) break;
			tempx1=tempy1=tempx2=tempy2=0;
			if((tablestruct.pointer != NULL)&&(tablestruct.flag==1))
			{
				onejunction = (struct Junction*)tablestruct.pointer;
				tempx1 = onejunction->centerx;
				tempy1 = onejunction->centery;
				templ1 = onejunction->layer;
			}
			memcpy(str2,tempNet_Pins,strlen(tempNet_Pins)+1);
			memcpy(&tablestruct,m_ptemp_Package_Table,5);
			if((tablestruct.pointer != NULL)&&(tablestruct.flag == 1))
			{
				onejunction = (struct Junction*)tablestruct.pointer;
				tempx2 = onejunction->centerx;
				tempy2 = onejunction->centery;
				templ2 = onejunction->layer;
			}

			if((tempx1!=0)&&(tempy1!=0)&&(tempx2!=0)&&(tempy2!=0))
			{
				route.sx=tempx1; route.sy=tempy1; route.slayer=templ1;
				route.ex=tempx2; route.ey=tempy2; route.elayer=templ2;
				templength = (unsigned long)sqrt(((float)tempx1-tempx2)*((float)tempx1-tempx2)\
			       +((float)tempy1-tempy2)*((float)tempy1-tempy2));
				route.length = (unsigned short)templength;
				Ratsnest_Length += templength;
				route.state = 0;
				memcpy(ratsnest,&route,13);
				ratsnest += 13;
				ratsnum++;
			}
		}
	}

	else if(Route_Mode==ROUTESHORTESTTREE)
	{
		tempNet_Pins = m_ppNet_Pinspointer[netnum];
		for(;;)
		{
			if(tempNet_Pins == m_ppNet_Pinspointer[netnum+1]) break;
			memcpy(str1,tempNet_Pins,strlen(tempNet_Pins)+1);
			tempNet_Pins+=strlen(tempNet_Pins)+1;
			memcpy(&k,tempNet_Pins,2);

			memcpy(&tablestruct,m_ptemp_Package_Table,5);
			m_ptemp_Package_Table+=5;

			if((k>0)&&(k<10000))
			{
				tempNet_Pins+=2;
				continue;
			}
			tempNet_Pins += 2;
			tempx1 = tempy1 = 0;
			if((tablestruct.pointer != NULL)&&(tablestruct.flag==1))
			{
				onejunction = (struct Junction*)tablestruct.pointer;
				tempx1 = onejunction->centerx;
				tempy1 = onejunction->centery;
				templ1 = onejunction->layer;
			}
			if((tempx1!=0)&&(tempy1!=0))
			{
				temp = routejunc;
				for(i=0;i<nodenum;i++)
				{
					memcpy(&node,temp,7);
					if((node.cx==tempx1)&&(node.cy==tempy1)&&(node.layer==templ1)) break;
					temp+=7;
				}
				if(i==nodenum)
				{
					node.cx=tempx1; node.cy=tempy1; node.layer=templ1; node.flag=0;
					memcpy(tempjunc,&node,7);
					tempjunc+=7;
					nodenum++;
				}
			}
		}

		if(nodenum<=1)
		{
			free(packagemem);
			free(routejunc);
			return 0;
		}
		if((nodenum >= Part_Net_Number)||((distancebuf = (short int*)malloc((nodenum-1)*(long)nodenum))==NULL))
	    {  
			//Error("too many nodes[节点太多]!");
			ratsnum = 0;
			goto LIEND;
	    }
		temp = routejunc;
		for(i=0;i<nodenum;i++)
		{
			for(j=i+1;j<nodenum;j++)
			{
				node1 = (struct Node*)(temp+i*7);
				node2 = (struct Node*)(temp+j*7);
				distancebuf[(long)i*nodenum-(long)i*(i+1)/2+(j-i-1)] = \
				(short int)sqrt(((float)(node1->cx)-node2->cx)*((float)(node1->cx)-node2->cx)+\
					((float)(node1->cy)-node2->cy)*((float)(node1->cy)-node2->cy));
			}
		}
		flag=0;
		temp=routejunc;
		mainnum1=0;
		mainnum2 = Find_min(mainnum1,distancebuf,temp,nodenum);
		mlength=distancebuf[((long)mainnum1*nodenum-(long)mainnum1*(mainnum1+1)/2)+(mainnum2-mainnum1-1)];

		memcpy(&node,temp+mainnum1*7,7);
		tempx1=node.cx; tempy1=node.cy; templ1=node.layer;
		if(((Ratsnestflag==3)&&(node.state==1))||(Ratsnestflag!=3)) flag=1;
		memcpy(&node,temp+mainnum2*7,7);
		tempx2=node.cx; tempy2=node.cy; templ2=node.layer;
		if(((Ratsnestflag==3)&&(node.state==1))||(Ratsnestflag!=3)) flag=1;

		// put to ratsnest buffer 
		route.sx=tempx1; route.sy=tempy1; route.slayer=templ1;
		route.ex=tempx2; route.ey=tempy2; route.elayer=templ2;
		route.length=mlength;
		Ratsnest_Length += mlength;
		route.state=0;
		if(flag==1)
		{
			memcpy(ratsnest,&route,13);
			ratsnest+=13;
			ratsnum++;
		}

		memcpy(&node,temp+mainnum1*7,7);
		node.flag=1;
		memcpy(temp+mainnum1*7,&node,7);
		memcpy(&node,temp+mainnum2*7,7);
		node.flag=1;
		memcpy(temp+mainnum2*7,&node,7);
		m=0;
		for(;;)
		{
			flag=0;
			mainnum1=0;
			mainnum2=Find_min(mainnum1,distancebuf,temp,nodenum);
			mlength=distancebuf[((long)mainnum1*nodenum-(long)mainnum1*(mainnum1+1)/2)+(mainnum2-mainnum1-1)];
			if(mainnum2==0) break;
			for(i=1;i<nodenum;i++)
			{
				//memcpy(&node,temp+i*7,7);
				node1 = (struct Node*)(temp+i*7);
				//if(node.flag==0) continue;
				if(node1->flag==0) continue;
				subnum1=i;
				subnum2=Find_min(subnum1,distancebuf,temp,nodenum);
				if(subnum2>subnum1)  slength=distancebuf[((long)subnum1*nodenum-(long)subnum1*(subnum1+1)/2)+(subnum2-subnum1-1)];
				else slength=distancebuf[((long)subnum2*nodenum-(long)subnum2*(subnum2+1)/2)+(subnum1-subnum2-1)];
				if(subnum2==0) break;
				if(slength<mlength) 
				{
					mainnum1=subnum1;
				    mainnum2=subnum2;
				    mlength=slength;
				}
			}

			node1 = (struct Node*)(temp+mainnum1*7);
			node2 = (struct Node*)(temp + mainnum2*7);
			if(((Ratsnestflag==3)&&(node.state==1))||(Ratsnestflag!=3)) flag=1;

			route.sx=node1->cx; route.sy=node1->cy; route.slayer=node1->layer;
			route.ex=node2->cx; route.ey=node2->cy; route.elayer=node2->layer;
			route.length=mlength;
			Ratsnest_Length+=mlength;
			route.state=0;
			if(flag==1)
			{
				memcpy(ratsnest,&route,13);
				ratsnest+=13;
				ratsnum++;
			}
			node1 = (struct Node*)(temp + mainnum2*7);
			node1->flag=1;
		}
		free(distancebuf);
	}

LIEND:
	free(packagemem);
	free(routejunc);
	return ratsnum;
}

void CDesignDoc::Fresh_Package_Table(int mode, char * string)
{
	int i,m,n,ch1;
	char *temp,*p;
	char *tempNet_Pins, *tempNetname;
	char str[60],str1[30],strid1[30],strnum1[10];
	struct Package *onepackage;
	struct Junction *onejunction;
	struct tablestruct 
	{
		char* pointer; 
		unsigned char flag; } tablestruct;

	m_ptemp_Package_Table = m_pPackage_Table;
	tempNetname = m_pNet_Netnamebuf;
	if(m_nNet_Number == 0) return;
	for(ch1=0;ch1<m_nNet_Number;ch1++)
	{
		memcpy(str,tempNetname,(strlen(tempNetname)+1));
		tempNetname += strlen(tempNetname) + 1;
		tempNet_Pins = m_ppNet_Pinspointer[ch1];
		for(;;)
		{
			memcpy(str1,tempNet_Pins,strlen(tempNet_Pins)+1);
			tempNet_Pins+=strlen(tempNet_Pins)+1;
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

			if(((mode==2)||(mode==5))&&(strcmp(str,string)!=0))
			{
				m_ptemp_Package_Table+=5;
				if(tempNet_Pins == m_ppNet_Pinspointer[ch1+1]) break;
				continue;
			}

			p = m_pPackage_Buffer;
			for(i=0; i<m_nPackage_Number; i++)
			{
				onepackage = (struct Package*)p;
				if(onepackage->flag==0)
				{
					if((strcmp(onepackage->ID,strid1)==0)&&\
					((onepackage->cx!=12345)||(onepackage->cy!=12345)))
					{
						temp = p;
						temp+=112;
						for(m=0;m<onepackage->padnum;m++)
						{
							onejunction = (struct Junction*)temp;
							if(strcmp((char*)onejunction->padname,strnum1)==0)
							{
								memcpy(&tablestruct,m_ptemp_Package_Table,5);
								tablestruct.pointer = temp;
								if(mode==1) tablestruct.flag = 1;
								else if(mode==2)
								{ 
									if(strcmp(str,string)==0) tablestruct.flag = 1;
								}
								if(mode==4) tablestruct.flag=0;
								else if(mode==5)
								{ 
									if(strcmp(str,string)==0) tablestruct.flag = 0;
								}
								memcpy(m_ptemp_Package_Table,&tablestruct,5);
								m_ptemp_Package_Table+=5;
								goto LIOUT;
							}
							temp+=16;
						}
					}
				}
				p += onepackage->total16byte*16;
			}
			
			tablestruct.pointer = NULL;
			tablestruct.flag = 0;
			memcpy(m_ptemp_Package_Table,&tablestruct,5);
			m_ptemp_Package_Table += 5;
LIOUT:
			if(tempNet_Pins == m_ppNet_Pinspointer[ch1+1]) break;
		}
	}    
}

void CDesignDoc::Fresh_Ratsnest_Package(char * str)
{
	int ch1,m,n;
	short int k;
	char *tempNet_Pins, *tempNetname;
	char netname[60];
	char str1[30],strid1[30],strnum1[10];

	Ratsnestflag = 2;
	tempNetname = m_pNet_Netnamebuf;
	for(ch1=0;ch1<m_nNet_Number;ch1++)
	{
		memcpy(netname,tempNetname,(strlen(tempNetname)+1));
		tempNetname += strlen(tempNetname)+1;
		tempNet_Pins = m_ppNet_Pinspointer[ch1];
		for(;;)
		{
			if(tempNet_Pins == m_ppNet_Pinspointer[ch1+1]) break;
			memcpy(str1,tempNet_Pins,strlen(tempNet_Pins)+1);
			tempNet_Pins+=strlen(tempNet_Pins)+1;
			memcpy(&k,tempNet_Pins,2);
			tempNet_Pins+=2;
			if((k>0)&&(k<10000)) continue;

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
			if(stricmp(strid1,str)==0)
			{
				Fresh_Package_Table(2,netname);
				break;
			}
		}
	}
}