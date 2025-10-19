void CDesignView::Route_Board_D()
{
	int i,ii,j,judge,routednum,gridbak,routemodebak;
	int flag;
	int ch1, ratsnestnum;
	unsigned tempx,tempy;
	char *route_ratsnest, *temp_ratsnest;
	char *swappad1, *swappad2, *swaptrack1, *swaptrack2;
	char *swappad1bak, *swappad2bak, *swaptrack1bak, *swaptrack2bak;
	struct Route_Rats route,route1;
	char str1[60],str2[10],str[60];
	float f1;
	
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Number==0) 
	{
		Error( error16 );
		return;
	}

	gridbak = CL_Snap_Grid;
	CL_Snap_Grid = 25;
	routemodebak = Route_Mode;
	Route_Mode = 0;

	if((route_ratsnest = (char*)malloc(64000))==NULL)
	{
		memory_error();
		return;
	}
	swappad1 = (char*)malloc(30000);
	swappad2 = (char*)malloc(30000);
	swaptrack1 = (char*)malloc(50000);
	swaptrack2 = (char*)malloc(50000);
	if((swappad1==NULL)||(swappad2==NULL)||(swaptrack1==NULL)||(swaptrack2==NULL))
	{
		memory_error();
		return;
	}
	swappad1bak=swappad1; swappad2bak=swappad2;
	swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;

	if( PCBHighlightBufferInit() != 0) return;
	if(Route_Switch[0]==1)
	{    
		CloseMouseCursor();
		Write_Help(helpstr50);
		OpenMouseCursor();
		Ratsnest_Refresh(1);
    }
	CloseMouseCursor();
	Write_Help(helpstr51);
	OpenMouseCursor();

	ratsnestnum = 0;
	pDoc->Fresh_Package_Table(1,Null_Str);
	temp_ratsnest = route_ratsnest;
	pDoc->m_ptemp_Package_Table = pDoc->m_pPackage_Table;
	Ratsnestflag = 1;
	for(ch1=0;ch1<pDoc->m_nNet_Number;ch1++)
    {
		temp_ratsnest=route_ratsnest+ratsnestnum*13;
		ratsnestnum += pDoc->Get_Onenet_Ratsnest(temp_ratsnest,ch1);
    }
	temp_ratsnest=route_ratsnest;
	for(i=0;i<ratsnestnum;i++)
	{
		memcpy(&route,temp_ratsnest,13);
		if(route.sx>route.ex)
		{
			tempx=route.sx; tempy=route.sy;
			route.sx=route.ex; route.sy=route.ey;
			route.ex=tempx;  route.ey=tempy;
			tempx=route.slayer;
			route.slayer=route.elayer;
			route.elayer=tempx;
			memcpy(temp_ratsnest,&route,13);
		}
		temp_ratsnest+=13;
	}

	if(ratsnestnum<1)  goto REND;

	judge=1;
	for(;;)
	{
		if(judge==1) judge=0;
	    else if(judge==0) break;
	    temp_ratsnest=route_ratsnest;
	    for(i=0;i<ratsnestnum-1;i++)
		{
			memcpy(&route,temp_ratsnest,13);
			memcpy(&route1,temp_ratsnest+13,13);
			if((route.sx>route1.sx)||\
		      ((route.sx==route1.sx)&&(route.sy>route1.sy)))
			{
				judge=1;
				memcpy(temp_ratsnest,&route1,13);
				memcpy(temp_ratsnest+13,&route,13);
			}
			temp_ratsnest+=13;
		}
	}

	temp_ratsnest=route_ratsnest;
	judge=1;
	for(;;)
	{
		if(judge==1) judge=0;
		else if(judge==0) break;
	    temp_ratsnest=route_ratsnest;
	    for(i=0;i<ratsnestnum-1;i++)
		{
			memcpy(&route,temp_ratsnest,13);
			memcpy(&route1,temp_ratsnest+13,13);
			if(route.length>route1.length)
			{
				judge=1;
				memcpy(temp_ratsnest,&route1,13);
				memcpy(temp_ratsnest+13,&route,13);
			}
			temp_ratsnest+=13;
		}
	}

	routednum=0;
	if(Route_Switch[2]==1)
	{
		CloseMouseCursor();
		Write_Help(helpstr52);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<ratsnestnum;i++)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						goto REND;
					}
				}
				break;
			}

			memcpy(&route,temp_ratsnest,13);
			if(route.state==1)
			{
				temp_ratsnest+=13;
				continue;
			}
			if(abs(route.sy-route.ey)<=ROUTEDELTA)
			{
				Highlight_Twopad(&route);
				if(route.sx>route.ex) 
				{
					tempx=route.sx; tempy=route.sy;
					route.sx=route.ex; route.sy=route.ey;
					route.ex=tempx;  route.ey=tempy;
					tempx=route.slayer;
					route.slayer=route.elayer;
					route.elayer=tempx;
				}
				if(Route_Hor_D(&route)!=0)
				{
					routednum++;
					f1=100*(float)routednum/ratsnestnum;
					sprintf(str1,"%s, %s%4.1f%s",helpstr52,gnhelpstr67,f1,"%");
					Write_Help(str1);
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
				}
			}
			temp_ratsnest+=13;
		}
	}

	if(Route_Switch[3]==1)
	{
		CloseMouseCursor();
		Write_Help(helpstr53);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<ratsnestnum;i++)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						goto REND;
					}
				}
				break;
			}
			memcpy(&route,temp_ratsnest,13);
			if(route.state==1)
			{
				temp_ratsnest+=13;
				continue;
			}
			if(abs(route.sx-route.ex)<=ROUTEDELTA)
			{
				Highlight_Twopad(&route);
				if(route.sy>route.ey) 
				{
					tempx=route.sx; tempy=route.sy;
					route.sx=route.ex; route.sy=route.ey;
					route.ex=tempx;  route.ey=tempy;
					tempx=route.slayer;
					route.slayer=route.elayer;
					route.elayer=tempx;
				}
				if(Route_Ver_D(&route)!=0)
			    {
					routednum++;
					f1=100*(float)routednum/ratsnestnum;
					sprintf(str1,"%s, %s%4.1f%s",helpstr53,gnhelpstr67,f1,"%");
					Write_Help(str1);
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
				}
			}
			temp_ratsnest+=13;
		}
	}

	if(Route_Switch[4]==1)
	{
		CloseMouseCursor();
		Write_Help(helpstr54);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<ratsnestnum;i++)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						goto REND;
					}
				}
				break;
			}
			memcpy(&route,temp_ratsnest,13);
			if(route.state==1)
			{
				temp_ratsnest+=13;
				continue;
			}

			for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
			for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
			swappad1bak=swappad1; swappad2bak=swappad2;
			swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
			Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
			Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			Highlight_Twopad(&route);
			flag=0;
			if(Route_Swap1_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) { flag=1; }
			if(flag==0)
			{
				if(route.sx>route.ex) 
				{
					tempx=route.sx; tempy=route.sy;
					route.sx=route.ex; route.sy=route.ey;
					route.ex=tempx;  route.ey=tempy;
					tempx=route.slayer;
					route.slayer=route.elayer;
					route.elayer=tempx;
				}
				if((abs(route.sx-route.ex)>ROUTEDELTA)&&(abs(route.sy-route.ey)>ROUTEDELTA))
				{
					if(Route_L_D(&route)!=0) flag=1;
				}
			}
			if(flag==0)
			{ 
				if(Route_Swap2_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) 
					flag=1; 
			}
			if(flag!=0)
			{ 
				routednum++;
				f1=100*(float)routednum/ratsnestnum;
				sprintf(str1,"%s, %s%4.1f%s",helpstr54,gnhelpstr67,f1,"%");
				Write_Help(str1);
				route1.state=1;
				memcpy(temp_ratsnest,&route1,13);
			}
			temp_ratsnest+=13;
		}
	}

	if(Route_Switch[5]==1)
	{
		CloseMouseCursor();
		Write_Help(helpstr55);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<ratsnestnum;i++)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						goto REND;
					}
				}
				break;
			}
			
			memcpy(&route,temp_ratsnest,13);
			if(route.state==1)
			{
				temp_ratsnest+=13;
				continue;
			}
			for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
			for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
			swappad1bak=swappad1; swappad2bak=swappad2;
			swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
			Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
			Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			Highlight_Twopad(&route);
			flag=0;
			if(Route_Swap1_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) { flag=1; }
			if(flag==0)
			{
				if(route.sx>route.ex) 
				{
					tempx=route.sx; tempy=route.sy;
					route.sx=route.ex; route.sy=route.ey;
					route.ex=tempx;  route.ey=tempy;
					tempx=route.slayer;
					route.slayer=route.elayer;
					route.elayer=tempx;
				}
				if((abs(route.sx-route.ex)>ROUTEDELTA)&&(abs(route.sy-route.ey)>ROUTEDELTA))
				{
					if(Route_Z_D(&route,0)!=0) flag=1;
				}
			}
			if(flag==0) { if(Route_Swap2_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0) { if(Route_Swap3_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag!=0)
			{
				routednum++;
				f1=100*(float)routednum/ratsnestnum;
				sprintf(str1,"%s, %s%4.1f%s",helpstr55,gnhelpstr67,f1,"%");
				Write_Help(str1);
				route1.state=1;
				memcpy(temp_ratsnest,&route1,13);
			}
			temp_ratsnest+=13;
		}
	}

	if(Route_Switch[6]==1)
	{
	  
		CloseMouseCursor();
		Write_Help(helpstr56);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<ratsnestnum;i++)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						goto REND;
					}
				}
				break;
			}
			memcpy(&route,temp_ratsnest,13);
			if(route.state==1)
			{
				temp_ratsnest+=13;
				continue;
			}
			for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
			for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
			swappad1bak=swappad1; swappad2bak=swappad2;
			swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
			Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
			Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			Highlight_Twopad(&route);
			flag=0;
			if(Route_Swap1_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) { flag=1; }
			if(flag==0) { if(Route_Swap2_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0)
			{
				if(route.sx>route.ex)
				{
					tempx=route.sx; tempy=route.sy;
					route.sx=route.ex; route.sy=route.ey;
					route.ex=tempx;  route.ey=tempy;
					tempx=route.slayer;
					route.slayer=route.elayer;
					route.elayer=tempx;
				}
				if(Route_C_D(&route)!=0) flag=1;
			}
			if(flag==0) { if(Route_Swap3_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0) { if(Route_Swap4_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag!=0)
			{
				routednum++;
				f1=100*(float)routednum/ratsnestnum;
				sprintf(str1,"%s, %s%4.1f%s",helpstr56,gnhelpstr67,f1,"%");
				Write_Help(str1);
				route1.state=1;
				memcpy(temp_ratsnest,&route1,13);
			}
			temp_ratsnest+=13;
		}
	}

	if(Route_Switch[7]==1)
	{
		CloseMouseCursor();
		Write_Help(helpstr57);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<ratsnestnum;i++)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						goto REND;
					}
				}
				break;
			}

			memcpy(&route,temp_ratsnest,13);
			if(route.state==1)
			{
				temp_ratsnest+=13;
				continue;
			}
			for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
			for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
			swappad1bak=swappad1; swappad2bak=swappad2;
			swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
			Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
			Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			Highlight_Twopad(&route);
			flag=0;
			if(Route_Swap1_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) { flag=1; }
			if(flag==0) { if(Route_Swap2_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0) { if(Route_Swap3_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0) { if(Route_Swap4_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0)
			{
				if(route.sx>route.ex) 
				{
					tempx=route.sx; tempy=route.sy;
					route.sx=route.ex; route.sy=route.ey;
					route.ex=tempx;  route.ey=tempy;
					tempx=route.slayer;
					route.slayer=route.elayer;
					route.elayer=tempx;
				}
				j=Super_C_D(&route);
				if(j<0) goto REND;
				else if(j>0) flag=1;
			}
			if(flag!=0)
			{
				routednum++;
				f1=100*(float)routednum/ratsnestnum;
				sprintf(str1,"%s, %s%4.1f%s",helpstr57,gnhelpstr67,f1,"%");
				Write_Help(str1);
				route1.state=1;
				memcpy(temp_ratsnest,&route1,13);
			}
			DrawPCBHighlight(0);
			temp_ratsnest+=13;
		}
	}

	if(Route_Switch[8]==1)
	{
		CloseMouseCursor();
		Write_Help(helpstr57);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<ratsnestnum;i++)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						goto REND;
					}
				}
				break;
			}

			memcpy(&route,temp_ratsnest,13);
			if(route.state==1)
			{
				temp_ratsnest+=13;
				continue;
			}
			for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
			for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
			swappad1bak=swappad1; swappad2bak=swappad2;
			swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
			Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
			Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			Highlight_Twopad(&route);
			flag=0;
			if(Route_Swap1_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) { flag=1; }
			if(flag==0) { if(Route_Swap2_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0) { if(Route_Swap3_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0) { if(Route_Swap4_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0)
			{
				if(route.sx>route.ex)
				{
					tempx=route.sx; tempy=route.sy;
					route.sx=route.ex; route.sy=route.ey;
					route.ex=tempx;  route.ey=tempy;
					tempx=route.slayer;
					route.slayer=route.elayer;
					route.elayer=tempx;
				}
				j=Super_Z_D(&route);
				if(j<0) goto REND;
				else if(j>0) flag=1;
			}
			if(flag==0)
			{
				j=Route_Swap5_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length);
				if(j<0) goto REND;
				else if(j>0) flag=1;
			}
			if(flag!=0)
			{
				routednum++;
				f1=100*(float)routednum/ratsnestnum;
				sprintf(str1,"%s, %s%4.1f%s",helpstr57,gnhelpstr67,f1,"%");
				Write_Help(str1);
				route1.state=1;
				memcpy(temp_ratsnest,&route1,13);
			}
			DrawPCBHighlight(0);
			temp_ratsnest+=13;
	   }
   }

REND:
	j=0;
	temp_ratsnest=route_ratsnest;
	for(i=0;i<ratsnestnum;i++)
	{
		memcpy(&route,temp_ratsnest,13);
		temp_ratsnest+=13;
		if(route.state==1) j++;
	}
	itoa(ratsnestnum,str1,10);
	itoa(j,str2,10);
	if(routednum == ratsnestnum) f1 = 100;
	else f1=100*(float)routednum/ratsnestnum;
	sprintf(str,"%s%s%s%s%s%4.1f%s","Total ratsnests=",str1," Routed=",str2," Rate=",f1,"%");
	AfxMessageBox(str, MB_OK|MB_ICONINFORMATION);
	Ratsnestflag=0;
	CL_Snap_Grid=gridbak;
	Route_Mode=routemodebak;
	free(route_ratsnest);
	free(swappad1); free(swappad2); free(swaptrack1); free(swaptrack2);
	Free_PCBHighlightbuf();
	Ratsnest_Refresh(0);
}

void CDesignView::Route_Net_D()
{
	int i,ii,j,num,judge,gridbak,routemodebak,flag,swapbak;
	unsigned tempx,tempy, ratsnestnum;
	int ch1;
	char *temp, *packmem;
	char *route_ratsnest, *temp_ratsnest;
	char *temptrackbuf, *temppadbuf, *temparcfillbuf, *temppackbuf;
	struct Route_Rats route,route1;
	char **nets;
	char str[60];
	char *swappad1, *swappad2, *swaptrack1, *swaptrack2;
	char *swappad1bak, *swappad2bak, *swaptrack1bak, *swaptrack2bak;

	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Number==0) 
	{
		Error( error16 );
		return;
	}

	if((nets = (char**)malloc(8000)) == NULL) 
	{
		memory_error();
		return;
	}
	temp = pDoc->m_pNet_Netnamebuf;
	//maxlen = 16;
	for(i=0;i<pDoc->m_nNet_Number;i++)
	{
		nets[i] = temp;
		temp+=strlen(temp)+1;
	}
		
	CSelectionDlg selection;
	selection.m_TitleText = select_net;
	selection.m_pList = nets;
	selection.m_nNum = pDoc->m_nNet_Number;

	Write_Help("选择网络布线");
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

	gridbak = CL_Snap_Grid;
	CL_Snap_Grid = 25;
	routemodebak = Route_Mode;
	Route_Mode = 0;
	swapbak = Route_Switch[9];
	
	// ???
	//if(Route_Switch[1]==1)  Route_Switch[9]=1;

	if((route_ratsnest = (char*)malloc(64000)) == NULL)
	{
		memory_error();
		return;
	}
	if((packmem = (char*)malloc(16384))==NULL)
	{
		memory_error();
		return;
	}
//	if(Route_Switch[9]==1)
  //  {
	   swappad1 = (char*)malloc(30000);
	   swappad2 = (char*)malloc(30000);
	   swaptrack1 = (char*)malloc(50000);
	   swaptrack2 = (char*)malloc(50000);
	   if((swappad1==NULL)||(swappad2==NULL)||(swaptrack1==NULL)||(swaptrack2==NULL))
	   {
		   memory_error();
		   return;
	   }
	   swappad1bak=swappad1; swappad2bak=swappad2;
	   swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
    //}

	temptrackbuf = Highlight_Trackbuf;
	temppadbuf = Highlight_Padbuf;
	temparcfillbuf = Highlight_ArcFillbuf;
	temppackbuf = Highlight_Packagebuf;

	if(Route_Switch[0]==1)
	{
		CloseMouseCursor();
		Write_Help(helpstr50);
		OpenMouseCursor();
		Ratsnest_Refresh(0);
	}

	if(PCBHighlightBufferInit() != 0) return;
	CloseMouseCursor();
	Write_Help(helpstr51);
	OpenMouseCursor();

	ratsnestnum=0;
	// 获取一个网络的飞线
	pDoc->Fresh_Package_Table(2,str);
	temp_ratsnest = route_ratsnest;
	pDoc->m_ptemp_Package_Table = pDoc->m_pPackage_Table;
	Ratsnestflag=1;
	for(ch1=0;ch1<pDoc->m_nNet_Number;ch1++)
    {
		temp_ratsnest=route_ratsnest+ratsnestnum*13;
		ratsnestnum += pDoc->Get_Onenet_Ratsnest(temp_ratsnest,ch1);
    }


	num=ratsnestnum;
	if(num<1) goto RNEND;

	temp_ratsnest=route_ratsnest;
	judge=1;
	for(;;)
	{
		if(judge==1) judge=0;
		else if(judge==0) break;
	    temp_ratsnest=route_ratsnest;
	    for(i=0;i<num-1;i++)
		{
			memcpy(&route,temp_ratsnest,13);
			memcpy(&route1,temp_ratsnest+13,13);
			if(route.length>route1.length)
			{
				judge=1;
				memcpy(temp_ratsnest,&route1,13);
				memcpy(temp_ratsnest+13,&route,13);
			}
			temp_ratsnest+=13;
		}
	}
	/*  Draw_All_Ratsnest(route_ratsnest,num);*/

	if(Route_Switch[2]==1)
	{
		CloseMouseCursor();
		Write_Help(helpstr52);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<num;i++)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						goto RNEND;
					}
				}
				break;
			}

			memcpy(&route,temp_ratsnest,13);
			if(route.state==1)
			{
				temp_ratsnest+=13;
				continue;
			}
			if(abs(route.sy-route.ey)<=ROUTEDELTA)
			{
				Highlight_Netpad(&route);
				if(route.sx>route.ex) 
				{
					tempx=route.sx; tempy=route.sy;
					route.sx=route.ex; route.sy=route.ey;
					route.ex=tempx;  route.ey=tempy;
					tempx=route.slayer;
					route.slayer=route.elayer;
					route.elayer=tempx;
				}
				if(Route_Hor_D(&route)!=0)
				{
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
				}
			}
			temp_ratsnest+=13;
		}
	}
	
	if(Route_Switch[3]==1)
	{
		CloseMouseCursor();
		Write_Help(helpstr53);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<num;i++)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						goto RNEND;
					}
				}
				break;
			}

			memcpy(&route,temp_ratsnest,13);
			if(route.state==1)
			{
				temp_ratsnest+=13;
				continue;
			 }
			if(abs(route.sx-route.ex)<=ROUTEDELTA)
			{
				Highlight_Netpad(&route);
				if(route.sy>route.ey) 
				{
					tempx=route.sx; tempy=route.sy;
					route.sx=route.ex; route.sy=route.ey;
					route.ex=tempx;  route.ey=tempy;
					tempx=route.slayer;
					route.slayer=route.elayer;
					route.elayer=tempx;
				}
				if(Route_Ver_D(&route)!=0)
				{
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
				}
			}
			temp_ratsnest+=13;
		}
	}

	if(Route_Switch[4]==1)
	{
		CloseMouseCursor();
		Write_Help(helpstr54);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<num;i++)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						goto RNEND;
					}
				}
				break;
			}
			memcpy(&route,temp_ratsnest,13);
			if(route.state==1) 
			{
				temp_ratsnest+=13;
				continue;
			}
			for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
			for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
			swappad1bak=swappad1; swappad2bak=swappad2;
			swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
			Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
			Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			Highlight_Twopad(&route);
			flag=0;
			if(Route_Swap1_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) { flag=1; }
			if(flag==0)
			{
				if(route.sx>route.ex)
				{
					tempx=route.sx; tempy=route.sy;
					route.sx=route.ex; route.sy=route.ey;
					route.ex=tempx;  route.ey=tempy;
					tempx=route.slayer;
					route.slayer=route.elayer;
					route.elayer=tempx;
				}
				if((abs(route.sx-route.ex)>ROUTEDELTA)&&(abs(route.sy-route.ey)>ROUTEDELTA))
				{
					if(Route_L_D(&route)!=0) flag=1;
				}
			}
			if(flag==0) { if(Route_Swap2_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag!=0)
			{
				route1.state=1;
				memcpy(temp_ratsnest,&route1,13);
			}
			temp_ratsnest+=13;
		}
	}

	if(Route_Switch[5]==1)
	{
		CloseMouseCursor();
		Write_Help(helpstr55);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<num;i++)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						goto RNEND;
					}
				}
				break;
			}

			memcpy(&route,temp_ratsnest,13);
			if(route.state==1)
			{
				temp_ratsnest+=13;
				continue;
			}
			for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
			for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
			swappad1bak=swappad1; swappad2bak=swappad2;
			swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
			Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
			Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			Highlight_Twopad(&route);
			flag=0;
			if(Route_Swap1_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) { flag=1; }
			if(flag==0)
			{
				if(route.sx>route.ex)
				{
					tempx=route.sx; tempy=route.sy;
					route.sx=route.ex; route.sy=route.ey;
					route.ex=tempx;  route.ey=tempy;
					tempx=route.slayer;
					route.slayer=route.elayer;
					route.elayer=tempx;
				}
				if((abs(route.sx-route.ex)>ROUTEDELTA)&&(abs(route.sy-route.ey)>ROUTEDELTA))
				{
					if(Route_Z_D(&route,0)!=0) flag=1;
				}
			}
			if(flag==0) { if(Route_Swap2_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0) { if(Route_Swap3_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag!=0)
			{
				route1.state=1;
				memcpy(temp_ratsnest,&route1,13);
			}
			temp_ratsnest+=13;
		}
	}

	if(Route_Switch[6]==1)
	{
		CloseMouseCursor();
		Write_Help(helpstr56);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<num;i++)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						goto RNEND;
					}
				}
				break;
			}

			memcpy(&route,temp_ratsnest,13);
			if(route.state==1) 
			{
				temp_ratsnest+=13;
				continue;
			}
			for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
			for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
			swappad1bak=swappad1; swappad2bak=swappad2;
			swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
			Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
			Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			Highlight_Twopad(&route);
			flag=0;
			if(Route_Swap1_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) { flag=1; }
			if(flag==0) { if(Route_Swap2_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0)
			{
				if(route.sx>route.ex)
				{
					tempx=route.sx; tempy=route.sy;
					route.sx=route.ex; route.sy=route.ey;
					route.ex=tempx;  route.ey=tempy;
					tempx=route.slayer;
					route.slayer=route.elayer;
					route.elayer=tempx;
				}
				if(Route_C_D(&route)!=0) flag=1;
			}
			if(flag==0)
			{
				if(Route_Swap3_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; 
			}
			if(flag==0) { if(Route_Swap4_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag!=0)
			{
				route1.state=1;
				memcpy(temp_ratsnest,&route1,13);
			}
			temp_ratsnest+=13;
		}
	}

	if(Route_Switch[7]==1)
	{
		CloseMouseCursor();
		Write_Help(helpstr57);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<num;i++)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						goto RNEND;
					}
				}
				break;
			}

			memcpy(&route,temp_ratsnest,13);
			if(route.state==1) 
			{
				temp_ratsnest+=13;
				continue;
			}
			for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
			for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
			swappad1bak=swappad1; swappad2bak=swappad2;
			swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
			Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
			Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			Highlight_Twopad(&route);
			flag=0;
			if(Route_Swap1_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) { flag=1; }
			if(flag==0) { if(Route_Swap2_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0) { if(Route_Swap3_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0) { if(Route_Swap4_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0)
			{
				if(route.sx>route.ex) 
				{
					tempx=route.sx; tempy=route.sy;
					route.sx=route.ex; route.sy=route.ey;
					route.ex=tempx;  route.ey=tempy;
					tempx=route.slayer;
					route.slayer=route.elayer;
					route.elayer=tempx;
				}
				j=Super_C_D(&route);
				if(j<0) goto RNEND;
				else if(j>0) flag=1;
			}
			if(flag!=0)
			{
				route1.state=1;
				memcpy(temp_ratsnest,&route1,13);
			}
			DrawPCBHighlight(0);
			temp_ratsnest+=13;
		}
	}

	if(Route_Switch[8]==1)
	{
		CloseMouseCursor();
		Write_Help(helpstr57);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<num;i++)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						goto RNEND;
					}
				}
				break;
			}

			memcpy(&route,temp_ratsnest,13);
			if(route.state==1)
			{
				temp_ratsnest+=13;
				continue;
			}
			for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
			for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
			swappad1bak=swappad1; swappad2bak=swappad2;
			swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
			Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
			Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			Highlight_Twopad(&route);
			flag=0;
			if(Route_Swap1_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) { flag=1; }
			if(flag==0) { if(Route_Swap2_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0) { if(Route_Swap3_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0) { if(Route_Swap4_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			if(flag==0)
			{
				if(route.sx>route.ex)
				{
					tempx=route.sx; tempy=route.sy;
					route.sx=route.ex; route.sy=route.ey;
					route.ex=tempx;  route.ey=tempy;
					tempx=route.slayer;
					route.slayer=route.elayer;
					route.elayer=tempx;
				}
				j=Super_Z_D(&route);
				if(j<0) goto RNEND;
				else if(j>0) flag=1;
			}
			if(flag==0)
			{
				j=Route_Swap5_D(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length);
				if(j<0) goto RNEND;
				else if(j>0) flag=1;
			}
			if(flag!=0)
			{
				route1.state=1;
				memcpy(temp_ratsnest,&route1,13);
			}
			DrawPCBHighlight(0);
			temp_ratsnest+=13;
		}
	}

RNEND:
	Ratsnestflag=0;
	CL_Snap_Grid=gridbak;
	Route_Mode=routemodebak;
	free(swappad1); free(swappad2); free(swaptrack1); free(swaptrack2);
	free(packmem);
	free(route_ratsnest);
	Free_PCBHighlightbuf();
}


int CDesignView::Route_Swap1_D(char * padbuf1, char * trackbuf1,\
							   char * padbuf2, char * trackbuf2, int length)
{
	int i,routenum, judge;
	char *temppadbuf1, *temppadbuf2;
	unsigned tempx,tempy;
	struct swappad
	{ 
		unsigned short cx, cy;
		unsigned char layer, state; } swappad;
	struct Route_Rats route, route1;
	char *routebuf,*temproutebuf;

	if((routebuf = (char*)malloc(13000))==NULL)
	{
		memory_error();
		return 0;
	}
	temproutebuf=routebuf;

	routenum=0;
	temppadbuf1=padbuf1;
	for(;;)
    {
		memcpy(&swappad,temppadbuf1,6);
		if((swappad.cx==0)&&(swappad.cy==0)) break;
		route.sx=swappad.cx; route.sy=swappad.cy; route.slayer=swappad.layer;
		temppadbuf1+=6;
		temppadbuf2=padbuf2;
		for(;;)
		{
			memcpy(&swappad,temppadbuf2,6);
			if((swappad.cx==0)&&(swappad.cy==0)) break;
			route.ex=swappad.cx; route.ey=swappad.cy; route.elayer=swappad.layer;
			if((abs(route.sy-route.ey)<=ROUTEDELTA)||(abs(route.sx-route.ex)<=ROUTEDELTA))
			{
				if(routenum>=1000) break;
				route.length=SQRT(route.sx,route.sy,route.ex,route.ey);
				//if(route.length <= length)
				//{
					memcpy(temproutebuf,&route,13);
					temproutebuf+=13;
					routenum++;
				//}
			}
			temppadbuf2+=6;
		}
	}

	if(routenum<1)
    {
		free(routebuf);
		return 0;
    }

	judge=1;
	for(;;)
	{
		if(judge==1) judge=0;
	    else if(judge==0) break;
	    temproutebuf=routebuf;
	    for(i=0;i<routenum-1;i++)
		{
			memcpy(&route,temproutebuf,13);
			memcpy(&route1,temproutebuf+13,13);
			if(route.length>route1.length)
			{
				judge=1;
				memcpy(temproutebuf,&route1,13);
				memcpy(temproutebuf+13,&route,13);
			}
			temproutebuf+=13;
		}
	}

	temproutebuf=routebuf;
	for(i=0;i<routenum;i++)
	{
		memcpy(&route,temproutebuf,13);
		temproutebuf+=13;
		if(abs(route.sy-route.ey)<=ROUTEDELTA)
		{
			if(route.sx>route.ex)
			{ 
				tempx=route.sx; tempy=route.sy;
				route.sx=route.ex; route.sy=route.ey;
				route.ex=tempx;  route.ey=tempy;
				tempx=route.slayer;
				route.slayer=route.elayer;
				route.elayer=tempx;
			}
			if(Route_Hor_D(&route)!=0)
			{
				free(routebuf);
				return 1;
			}
		}
		if(abs(route.sx-route.ex)<=ROUTEDELTA)
		{
			if(route.sy>route.ey)
			{
				tempx=route.sx; tempy=route.sy;
				route.sx=route.ex; route.sy=route.ey;
				route.ex=tempx;  route.ey=tempy;
				tempx=route.slayer;
				route.slayer=route.elayer;
				route.elayer=tempx;
			}
			if(Route_Ver_D(&route)!=0)
			{
				free(routebuf);
				return 1;
			}
		}
	}
	free(routebuf);
	return 0;
}

int CDesignView::Route_Swap2_D(char * padbuf1, char * trackbuf1, \
							   char * padbuf2, char * trackbuf2, int length)
{
	int i, routenum, judge;
	char *temppadbuf1, *temppadbuf2;
	unsigned tempx,tempy;
	struct swappad
	{
		unsigned short cx, cy;
		unsigned char layer, state; } swappad;
	struct Route_Rats route, route1;
	char *routebuf,*temproutebuf;

	if((routebuf = (char*)malloc(13000))==NULL) 
	{
		memory_error();
		return 0;
	}
	temproutebuf=routebuf;

	routenum=0;
	temppadbuf1=padbuf1;
	for(;;)
    { 
		memcpy(&swappad,temppadbuf1,6);
		if((swappad.cx==0)&&(swappad.cy==0)) break;
		route.sx=swappad.cx; route.sy=swappad.cy; route.slayer=swappad.layer;
		temppadbuf1+=6;
		temppadbuf2=padbuf2;
		for(;;)
		{
			memcpy(&swappad,temppadbuf2,6);
			if((swappad.cx==0)&&(swappad.cy==0)) break;
			route.ex=swappad.cx; route.ey=swappad.cy; route.elayer=swappad.layer;
			if((abs(route.sy-route.ey)>ROUTEDELTA)&&(abs(route.sx-route.ex)>ROUTEDELTA))
			{  
				if(routenum>=1000) break;
				route.length=SQRT(route.sx,route.sy,route.ex,route.ey);
				//if(route.length <= length)
				//{
					memcpy(temproutebuf,&route,13);
					temproutebuf+=13;
					routenum++;
				//}
			}
			temppadbuf2+=6;
		}
    }

	if(routenum<1)
    {
		free(routebuf);
		return 0;
    }

	judge=1;
	for(;;)
	{
	    if(judge==1) judge=0;
	    else if(judge==0) break;
	    temproutebuf=routebuf;
	    for(i=0;i<routenum-1;i++)
		{
			memcpy(&route,temproutebuf,13);
			memcpy(&route1,temproutebuf+13,13);
			if(route.length>route1.length)
			{
				judge=1;
				memcpy(temproutebuf,&route1,13);
				memcpy(temproutebuf+13,&route,13);
			}
			temproutebuf+=13;
		}
	}

	temproutebuf=routebuf;
	for(i=0;i<routenum;i++)
	{
		memcpy(&route,temproutebuf,13);
		temproutebuf+=13;
		if(route.sx>route.ex)
		{
			tempx=route.sx; tempy=route.sy;
			route.sx=route.ex; route.sy=route.ey;
			route.ex=tempx;  route.ey=tempy;
			tempx=route.slayer;
			route.slayer=route.elayer;
			route.elayer=tempx;
		}
		if(Route_L_D(&route)!=0)
		{
			free(routebuf);
			return 1;
		}
	}
	free(routebuf);
	return 0;
}



int CDesignView::Route_Swap3_D(char * padbuf1, char * trackbuf1,\
							   char * padbuf2, char * trackbuf2, int length)
{
	int i, routenum, judge;
	char *temppadbuf1,  *temppadbuf2;
	unsigned tempx,tempy;
	struct swappad
	{
		unsigned short cx, cy;
		unsigned char layer, state; } swappad;
	struct Route_Rats route, route1;
	char *routebuf,*temproutebuf;

	if((routebuf = (char*)malloc(13000))==NULL)
	{
		memory_error();
		return 0;
	}
	temproutebuf=routebuf;

	routenum=0;
	temppadbuf1=padbuf1;
	for(;;)
    {
		memcpy(&swappad,temppadbuf1,6);
		if((swappad.cx==0)&&(swappad.cy==0)) break;
		route.sx=swappad.cx; route.sy=swappad.cy; route.slayer=swappad.layer;
		temppadbuf1+=6;
		temppadbuf2=padbuf2;
		for(;;)
		{
			memcpy(&swappad,temppadbuf2,6);
			if((swappad.cx==0)&&(swappad.cy==0)) break;
			route.ex=swappad.cx; route.ey=swappad.cy; route.elayer=swappad.layer;
			if((abs(route.sy-route.ey)>ROUTEDELTA)&&(abs(route.sx-route.ex)>ROUTEDELTA))
			{
				if(routenum>=1000) break;
				route.length=SQRT(route.sx,route.sy,route.ex,route.ey);
				//if(route.length <= length)
				//{
					memcpy(temproutebuf,&route,13);
					temproutebuf+=13;
					routenum++;
				//}
			}
			temppadbuf2+=6;
		}
	}

	if(routenum<1)
    {
		free(routebuf);
		return 0;
    }

	judge=1;
	for(;;)
	{
		if(judge==1) judge=0;
		else if(judge==0) break;
	    temproutebuf=routebuf;
	    for(i=0;i<routenum-1;i++)
		{
		   memcpy(&route,temproutebuf,13);
		   memcpy(&route1,temproutebuf+13,13);
		   if(route.length>route1.length)
		   {
				judge=1;
				memcpy(temproutebuf,&route1,13);
				memcpy(temproutebuf+13,&route,13);
		   }
		   temproutebuf+=13;
		 }
	}

	temproutebuf=routebuf;
	for(i=0;i<routenum;i++)
	{
		memcpy(&route,temproutebuf,13);
		temproutebuf+=13;
		if(route.sx>route.ex)
		{
			tempx=route.sx; tempy=route.sy;
			route.sx=route.ex; route.sy=route.ey;
			route.ex=tempx;  route.ey=tempy;
			tempx=route.slayer;
			route.slayer=route.elayer;
			route.elayer=tempx;
		}
		if(Route_Z_D(&route,0)!=0)
		{
			free(routebuf);
			return 1;
		}
	}
	free(routebuf);
	return 0;
}


int CDesignView::Route_Swap4_D(char * padbuf1, char * trackbuf1,\
							   char * padbuf2, char * trackbuf2, int length)
{
	int i, routenum, judge;
	char *temppadbuf1,  *temppadbuf2;
	unsigned tempx,tempy;
	struct swappad
	{ 
		unsigned short cx, cy;
		unsigned char layer, state; } swappad;
	struct Route_Rats route, route1;
	char *routebuf,*temproutebuf;

	if((routebuf = (char*)malloc(13000))==NULL)
	{
		memory_error();
		return 0;
	}
	temproutebuf=routebuf;

	routenum=0;
	temppadbuf1=padbuf1;
	for(;;)
    { 
		memcpy(&swappad,temppadbuf1,6);
		if((swappad.cx==0)&&(swappad.cy==0)) break;
		route.sx=swappad.cx; route.sy=swappad.cy; route.slayer=swappad.layer;
		temppadbuf1+=6;
		temppadbuf2=padbuf2;
		for(;;)
		{
			memcpy(&swappad,temppadbuf2,6);
			if((swappad.cx==0)&&(swappad.cy==0)) break;
			route.ex=swappad.cx; route.ey=swappad.cy; route.elayer=swappad.layer;
			if((abs(route.sy-route.ey)>ROUTEDELTA)&&(abs(route.sx-route.ex)>ROUTEDELTA))
			{
				if(routenum>=1000) break;
				route.length=SQRT(route.sx,route.sy,route.ex,route.ey);
				//if(route.length <= length)
				//{
					memcpy(temproutebuf,&route,13);
					temproutebuf+=13;
					routenum++;
				//}
			}
			temppadbuf2+=6;
		}
	}

	if(routenum<1)
    {
		free(routebuf);
		return 0;
    }

	judge=1;
	for(;;)
	{
		if(judge==1) judge=0;
	    else if(judge==0) break;
	    temproutebuf=routebuf;
	    for(i=0;i<routenum-1;i++)
		{
			memcpy(&route,temproutebuf,13);
			memcpy(&route1,temproutebuf+13,13);
			if(route.length>route1.length)
			{
				judge=1;
				memcpy(temproutebuf,&route1,13);
				memcpy(temproutebuf+13,&route,13);
			}
			temproutebuf+=13;
		}
	}

	temproutebuf=routebuf;
	for(i=0;i<routenum;i++)
	{
		memcpy(&route,temproutebuf,13);
		temproutebuf+=13;
		if(route.sx>route.ex)
		{
			tempx=route.sx; tempy=route.sy;
			route.sx=route.ex; route.sy=route.ey;
			route.ex=tempx;  route.ey=tempy;
			tempx=route.slayer;
			route.slayer=route.elayer;
			route.elayer=tempx;
		}
		if(Route_C_D(&route)!=0)
		{
			free(routebuf);
			return 1;
		}
	}
	free(routebuf);
	return 0;
}

int CDesignView::Route_Swap5_D(char * padbuf1, char * trackbuf1, \
							   char * padbuf2, char * trackbuf2, int length)
{
	int i,ii,j,routenum, judge;
	char *temppadbuf1, *temppadbuf2;
	unsigned tempx,tempy;
	struct swappad
	{
		unsigned  short cx, cy;
		unsigned char layer, state; } swappad;
	struct Route_Rats route, route1;
	char *routebuf,*temproutebuf;

	if((routebuf = (char*)malloc(13000))==NULL)
	{
		memory_error();
		return 0;
	}
	temproutebuf=routebuf;

	routenum=0;
	temppadbuf1=padbuf1;
	for(;;)
    {
		memcpy(&swappad,temppadbuf1,6);
       if((swappad.cx==0)&&(swappad.cy==0)) break;
       route.sx=swappad.cx; route.sy=swappad.cy; route.slayer=swappad.layer;
       temppadbuf1+=6;
       temppadbuf2=padbuf2;
       for(;;)
		{
			memcpy(&swappad,temppadbuf2,6);
			if((swappad.cx==0)&&(swappad.cy==0)) break;
			route.ex=swappad.cx; route.ey=swappad.cy; route.elayer=swappad.layer;
			if((abs(route.sy-route.ey)>ROUTEDELTA)&&(abs(route.sx-route.ex)>ROUTEDELTA))
			{
				if(routenum>=1000) break;
				route.length=SQRT(route.sx,route.sy,route.ex,route.ey);
				//if(route.length <= length)
				//{
					memcpy(temproutebuf,&route,13);
					temproutebuf+=13;
					routenum++;
				//}
			}
			temppadbuf2+=6;
		}
	}

	if(routenum<1)
    {
		free(routebuf);
		return 0;
    }

	judge=1;
	for(;;)
	{
		if(judge==1) judge=0;
	    else if(judge==0) break;
	    temproutebuf=routebuf;
	    for(i=0;i<routenum-1;i++)
		{
			memcpy(&route,temproutebuf,13);
			memcpy(&route1,temproutebuf+13,13);
			if(route.length>route1.length)
			{
				judge=1;
				memcpy(temproutebuf,&route1,13);
				memcpy(temproutebuf+13,&route,13);
			}
			temproutebuf+=13;
		}
	}

	temproutebuf=routebuf;
	for(i=0;i<routenum;i++)
	{
		// interupt route
		for(;;)
		{
			int ch2 = MRetKey();
			if((ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
			{
				ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
				if( ii == IDYES)
				{
					free(routebuf);
					return -1;
				}
			}
			break;
		}
		memcpy(&route,temproutebuf,13);
		temproutebuf+=13;
		if(route.sx>route.ex)
		{
			tempx=route.sx; tempy=route.sy;
			route.sx=route.ex; route.sy=route.ey;
			route.ex=tempx;  route.ey=tempy;
			tempx=route.slayer;
			route.slayer=route.elayer;
			route.elayer=tempx;
		}
		j =Super_C_D(&route);
		if(j<0)
		{
			free(routebuf);
			return -1;
		}
		else if(j>0) 
		{
			free(routebuf);
			return 1;
		}
		if(route.sx>route.ex)
		{
			tempx=route.sx; tempy=route.sy;
			route.sx=route.ex; route.sy=route.ey;
			route.ex=tempx;  route.ey=tempy;
			tempx=route.slayer;
			route.slayer=route.elayer;
			route.elayer=tempx;
		}
		j =Super_Z_D(&route);
		if(j<0)
		{
			free(routebuf);
			return -1;
		}
		else if(j>0) 
		{
			free(routebuf);
			return 1;
		}
	}
	free(routebuf);
	return 0;
}


int CDesignView::Route_Hor_D(struct Route_Rats * route)
{
	int j,n;
	char *temp;
	char linebuf[80];
	struct Undo_Head head;
	char *undo_p;

	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	DrawAnyLine(route->sx,route->sy,route->ex,route->ey,Highlighted_Color,1, &dc);
	temp=linebuf;
	n=Route_Hor_Line_D(route,temp);
	CloseMouseCursor();
	DrawAnyLine(route->sx,route->sy,route->ex,route->ey,Highlighted_Color,1,&dc);
	DrawPCBHighlight(0);
	OpenMouseCursor();
	if(n!=0)
	{
		temp=linebuf;
		for(j=0;j<n;j++)
		{
			memcpy(&gnline,temp,16);
			temp+=16;
			if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
		    pDoc->Save_EMMLine(&gnline);
			CloseMouseCursor();
			DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
			OpenMouseCursor();
		}
    }
	if(n!=0) return 1;
	else return 0;
}


int CDesignView::Route_Ver_D(struct Route_Rats * route)
{
	int j,n;
	char *temp;
	char linebuf[80];
	struct Undo_Head head;
	char *undo_p;

	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	DrawAnyLine(route->sx,route->sy,route->ex,route->ey,Highlighted_Color,1,&dc);
	temp=linebuf;
	n=Route_Ver_Line_D(route,temp);
	CloseMouseCursor();
	DrawAnyLine(route->sx,route->sy,route->ex,route->ey,Highlighted_Color,1,&dc);
	DrawPCBHighlight(0);
	OpenMouseCursor();
	if(n!=0)
	{
		temp=linebuf;
		for(j=0;j<n;j++)
		{
			memcpy(&gnline,temp,16);
			temp+=16;
			if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
			pDoc->Save_EMMLine(&gnline);
			CloseMouseCursor();
			DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
			OpenMouseCursor();
		}
    }
	if(n!=0) return 1;
	else return 0;
}


int CDesignView::Route_L_D(struct Route_Rats * route)
{

	int i,j,n1,n2,n3;
	char *temp, *temp1, *temp2;
	char linebuf1[80];
	char linebuf2[80];
	struct Route_Rats route1,route2;
	struct Junction routevia;
	unsigned tempx,tempy;
	struct Undo_Head head;
	char *undo_p;

	long int delta[5][2] = {0,1, 1,0, 0,-1, -1,0 };
	long int delta1[9][2]={ 1,1, 0,0, 0,1, 1,0, -1,-1, -1,0, -1,1, 0,-1, 1,-1};
   /******************/
   /*   Osx,sy       */
   /*   |            */
   /* 6 2 0    ex,ey */
   /* 5 1 3--------O */
   /* 4 7 8          */
   /******************/
   long int delta2[9][2]={ 1,-1, 0,0, 0,-1, 1,0, -1,1, -1,0, -1,-1, 0,1, 1,1 };
   /*********************/
   /* 4 7 8             */
   /* 5 1 3--------- O  */
   /* 6 2 0       ex,ey */
   /*  |                */
   /*  O sx,sy          */
   /*********************/
   long int delta3[9][2]={-1,-1, 0,0, 0,-1, -1,0, 1,1, 1,0, 1,-1, 0,1, -1,1  };
   /******************/
   /*           8 7 4*/
   /*  O--------3 1 5*/
   /* sx,sy     0 2 6*/
   /*             |  */
   /*      ex,ey  O  */
   /******************/
   long int delta4[9][2]={-1, 1, 0,0, 0,1, -1,0, 1,-1, 1,0, 1,1, 0,-1, -1,-1  };
   /**********************/
   /*          ex,ey O   */
   /*                |   */
   /*  sx,sy       0 2 6 */
   /*  O-----------3 1 5 */
   /*              8 7 4 */
   /**********************/
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	routevia.id=0x22;
   routevia.Dx=routevia.Dy=Route_TW;
   routevia.d=1;
   routevia.padtype=1;
   routevia.attrib=PCB;
   routevia.layer=MULTILAYER;
   routevia.flag=0;

   DrawAnyLine(route->sx,route->sy,route->ex,route->ey,Highlighted_Color,1,&dc);
   /***********************************/
   /* Mode 1                          */
   /*  3                              */
   /*4 0 2-------- O    Osx,sy        */
   /*  1         ex,ey  |             */
   /*  |                1       ex,ey */
   /*  O sx,sy        4 0 2---------O */
   /*                   3             */
   /***********************************/
   tempx=route->sx;
   tempy=route->ey;
   routevia.centerx=tempx;
   routevia.centery=tempy;
   n3=Clearance_Route((struct Line*)&routevia);
   if(n3!=0) goto LMODE2;
   if(tempy>route->sy)
   {
	   route1.sx=route->sx;
		route1.sy=route->sy;
		route1.slayer=route->slayer;
		route1.ex=tempx;
		route1.ey=tempy;
		route1.elayer=MULTILAYER;
   }
   else
   {
	   route1.sx=tempx;
		route1.sy=tempy;
		route1.slayer=MULTILAYER;
		route1.ex=route->sx;
		route1.ey=route->sy;
		route1.elayer=route->slayer;
   }
   n1=Fast_Route_Ver_Line_D(&route1,linebuf1);
   if(n1==0) goto LMODE2;
   route2.sx=tempx;
   route2.sy=tempy;
   route2.slayer=MULTILAYER;
   route2.ex=route->ex;
   route2.ey=route->ey;
   route2.elayer=route->elayer;
   n2=Fast_Route_Hor_Line_D(&route2,linebuf2);
   if((n1!=0)&&(n2!=0)&&(n3==0)) goto LEND;

LMODE2:
   tempx=route->ex;
   tempy=route->sy;
   routevia.centerx=tempx;
   routevia.centery=tempy;
   n3=Clearance_Route((struct Line*)&routevia);
   if(n3!=0) goto LMODE3;
   if(tempy>route->ey)
    {
	   route1.sx=route->ex;
		route1.sy=route->ey;
		route1.slayer=route->elayer;
		route1.ex=tempx;
		route1.ey=tempy;
		route1.elayer=MULTILAYER;
    }
	else
    {
		route1.sx=tempx;
		route1.sy=tempy;
		route1.slayer=MULTILAYER;
		route1.ex=route->ex;
		route1.ey=route->ey;
		route1.elayer=route->elayer;
    }
	n1=Fast_Route_Ver_Line_D(&route1,linebuf1);
   if(n1==0)  goto LMODE3;
   route2.sx=route->sx;
   route2.sy=route->sy;
   route2.slayer=route->slayer;
   route2.ex=tempx;
   route2.ey=tempy;
   route2.elayer=MULTILAYER;
   n2=Fast_Route_Hor_Line_D(&route2,linebuf2);
   if((n1!=0)&&(n2!=0)&&(n3==0)) goto LEND;

LMODE3:
   for(i=0;i<4;i++)
   {
	   tempx=(long)(route->sx)+delta[i][0]*ROUTEDELTA;
	   tempy=(long)(route->ey)+delta[i][1]*ROUTEDELTA;
	   routevia.centerx=tempx;
	   routevia.centery=tempy;
	   n3=Clearance_Route((struct Line*)&routevia);
	   if(n3!=0) continue;
	   if(tempy>route->sy)
	   {
		   route1.sx=route->sx;
			route1.sy=route->sy;
			route1.slayer=route->slayer;
			route1.ex=tempx;
			route1.ey=tempy;
			route1.elayer=MULTILAYER;
	   }
	   else
	   {  
		   route1.sx=tempx;
			route1.sy=tempy;
			route1.slayer=MULTILAYER;
			route1.ex=route->sx;
			route1.ey=route->sy;
			route1.elayer=route->slayer;
	   }
	   n1=Fast_Route_Ver_Line_D(&route1,linebuf1);
	   if(n1==0) continue;
	   route2.sx=tempx;
	   route2.sy=tempy;
	   route2.slayer=MULTILAYER;
	   route2.ex=route->ex;
	   route2.ey=route->ey;
	   route2.elayer=route->elayer;
	   n2=Fast_Route_Hor_Line_D(&route2,linebuf2);
	   if((n1!=0)&&(n2!=0)&&(n3==0)) goto LEND;
    }
   /***********************************/
   /* Mode 2                          */
   /*        ex,ey O    O-----------x */
   /*              |    sx,sy       | */
   /* sx,sy        |                | */
   /*  O---------- x           ex,eyO */
   /***********************************/
	for(i=0;i<4;i++)
	{
		tempx=(long)(route->ex)+delta[i][0]*ROUTEDELTA;
		tempy=(long)(route->sy)+delta[i][1]*ROUTEDELTA;
		routevia.centerx=tempx;
		routevia.centery=tempy;
		n3=Clearance_Route((struct Line*)&routevia);
		if(n3!=0) continue;
		if(tempy>route->ey)
		{
			route1.sx=route->ex;
			route1.sy=route->ey;
			route1.slayer=route->elayer;
			route1.ex=tempx;
			route1.ey=tempy;
			route1.elayer=MULTILAYER;
	    }
		else
	    {
			route1.sx=tempx;
			route1.sy=tempy;
			route1.slayer=MULTILAYER;
			route1.ex=route->ex;
			route1.ey=route->ey;
			route1.elayer=route->elayer;
	    }
	   n1=Fast_Route_Ver_Line_D(&route1,linebuf1);
	   if(n1==0)  continue;
	   route2.sx=route->sx;
	   route2.sy=route->sy;
	   route2.slayer=route->slayer;
	   route2.ex=tempx;
	   route2.ey=tempy;
	   route2.elayer=MULTILAYER;
	   n2=Fast_Route_Hor_Line_D(&route2,linebuf2);
	   if((n1!=0)&&(n2!=0)&&(n3==0)) goto LEND;
	}
	if(route->ey<route->sy)
	{
		/******************/
		/*   Osx,sy       */
		/*   |            */
		/* 6 2 0    ex,ey */
		/* 5 1 3--------O */
		/* 4 7 8          */
		/******************/
		for(i=0;i<9;i++)
		{
			tempx=(long)(route->sx)+delta1[i][0]*ROUTEDELTA;
			tempy=(long)(route->ey)+delta1[i][1]*ROUTEDELTA;
			routevia.centerx=tempx;
			routevia.centery=tempy;
			n3=Clearance_Route((struct Line*)&routevia);
			if(n3!=0) continue;
			route1.sx=tempx;
			route1.sy=tempy;
			route1.slayer=MULTILAYER;
			route1.ex=route->sx;
			route1.ey=route->sy;
			route1.elayer=route->slayer;
			temp1=linebuf1;
			n1=Route_Ver_Line_D(&route1,temp1);
			if(n1==0) 
			{
				if(i==1) break;
		       else if(i==4) { i+=4; continue; }
		       else if(i==5) { i+=1; continue; }
		       else if(i==7) { i+=1; continue; }
		       else  continue;
			}
			route2.sx=tempx;
			route2.sy=tempy;
			route2.slayer=MULTILAYER;
			route2.ex=route->ex;
			route2.ey=route->ey;
			route2.elayer=route->elayer;
			temp2=linebuf2;
			n2=Route_Hor_Line_D(&route2,temp2);
			if(n2==0)
			{
				if(i==1) break;
		       else if(i==4) { i+=4; continue; }
		       else if(i==5) { i+=1; continue; }
		       else if(i==7) { i+=1; continue; }
		       else  continue;
		    }
			if((n1!=0)&&(n2!=0)&&(n3==0)) goto LEND;
		}
	}
    else
	{
		/*********************/
	   /* 4 7 8             */
	   /* 5 1 3--------- O  */
	   /* 6 2 0       ex,ey */
	   /*  |                */
	   /*  O sx,sy          */
	   /*********************/
		for(i=0;i<9;i++)
		{
			tempx=(long)(route->sx)+delta2[i][0]*ROUTEDELTA;
			tempy=(long)(route->ey)+delta2[i][1]*ROUTEDELTA;
			routevia.centerx=tempx;
			routevia.centery=tempy;
			n3=Clearance_Route((struct Line*)&routevia);
			if(n3!=0) continue;
			route1.sx=route->sx;
			route1.sy=route->sy;
			route1.slayer=route->slayer;
			route1.ex=tempx;
			route1.ey=tempy;
			route1.elayer=MULTILAYER;
			temp1=linebuf1;
			n1=Route_Ver_Line_D(&route1,temp1);
			if(n1==0)
			{
				if(i==1) break;
		       else if(i==4) { i+=4; continue; }
		       else if(i==5) { i+=1; continue; }
		       else if(i==7) { i+=1; continue; }
		       else  continue;
		     }
			route2.sx=tempx;
			route2.sy=tempy;
			route2.slayer=MULTILAYER;
			route2.ex=route->ex;
			route2.ey=route->ey;
			route2.elayer=route->elayer;
			temp2=linebuf2;
			n2=Route_Hor_Line_D(&route2,temp2);
			if(n2==0)
			{
				if(i==1) break;
		       else if(i==4) { i+=4; continue; }
		       else if(i==5) { i+=1; continue; }
		       else if(i==7) { i+=1; continue; }
		       else  continue;
		    }
			if((n1!=0)&&(n2!=0)&&(n3==0)) goto LEND;
		}
	}

//LMODE4:
	if(route->ey<route->sy)
	{
		/*******************/
	 /*           8 7 4 */
	 /*  O--------3 1 5 */
	 /* sx,sy     0 2 6 */
	 /*             |   */
	 /*      ex,ey  O   */
	 /*******************/
		for(i=0;i<9;i++)
		{
			tempx=(long)(route->ex)+delta3[i][0]*ROUTEDELTA;
			tempy=(long)(route->sy)+delta3[i][1]*ROUTEDELTA;
			routevia.centerx=tempx;
			routevia.centery=tempy;
			n3=Clearance_Route((struct Line*)&routevia);
			if(n3!=0) continue;
			route1.ex=tempx;
			route1.ey=tempy;
			route1.elayer=MULTILAYER;
			route1.sx=route->ex;
			route1.sy=route->ey;
			route1.slayer=route->elayer;
			temp1=linebuf1;
			n1=Route_Ver_Line_D(&route1,temp1);
			if(n1==0)
			{
				if(i==1) break;
		       else if(i==4) { i+=4; continue; }
		       else if(i==5) { i+=1; continue; }
		       else if(i==7) { i+=1; continue; }
		       else continue;
		     }
			route2.sx=route->sx;
			route2.sy=route->sy;
			route2.slayer=route->slayer;
			route2.ex=tempx;
			route2.ey=tempy;
			route2.elayer=MULTILAYER;
			temp2=linebuf2;
			n2=Route_Hor_Line_D(&route2,temp2);
			if(n2==0)
			{
				if(i==1) break;
		       else if(i==4) { i+=4; continue; }
		       else if(i==5) { i+=1; continue; }
		       else if(i==7) { i+=1; continue; }
		       else  continue;
		     }
			if((n1!=0)&&(n2!=0)&&(n3==0)) goto LEND;
		}
    }
	else
    {
		/**********************/
		/*          ex,ey O   */
		/*                |   */
		/*  sx,sy       0 2 6 */
		/*  O-----------3 1 5 */
		/*              8 7 4 */
		/**********************/
		for(i=0;i<9;i++)
		{
			tempx=(long)(route->ex)+delta4[i][0]*ROUTEDELTA;
			tempy=(long)(route->sy)+delta4[i][1]*ROUTEDELTA;
			routevia.centerx=tempx;
			routevia.centery=tempy;
			n3=Clearance_Route((struct Line*)&routevia);
			if(n3!=0) continue;
			route1.ex=route->ex;
			route1.ey=route->ey;
			route1.elayer=route->elayer;
			route1.sx=tempx;
			route1.sy=tempy;
			route1.slayer=MULTILAYER;
			temp1=linebuf1;
			n1=Route_Ver_Line_D(&route1,temp1);
			if(n1==0)
			{
				if(i==1) break;
		       else if(i==4) { i+=4; continue; }
		       else if(i==5) { i+=1; continue; }
		       else if(i==7) { i+=1; continue; }
		       else  continue;
		     }
			route2.sx=route->sx;
			route2.sy=route->sy;
			route2.slayer=route->slayer;
			route2.ex=tempx;
			route2.ey=tempy;
			route2.elayer=MULTILAYER;
			temp2=linebuf2;
			n2=Route_Hor_Line_D(&route2,temp2);
			if(n2==0)
			{
				if(i==1) break;
		       else if(i==4) { i+=4; continue; }
		       else if(i==5) { i+=1; continue; }
		       else if(i==7) { i+=1; continue; }
		       else  continue;
		     }
			if((n1!=0)&&(n2!=0)&&(n3==0)) goto LEND;
		}
    }
LEND:
	CloseMouseCursor();
	DrawAnyLine(route->sx,route->sy,route->ex,route->ey,Highlighted_Color,1,&dc);
	DrawPCBHighlight(0);
	OpenMouseCursor();
	if((n1!=0)&&(n2!=0)&&(n3==0))
	{
		temp=linebuf1;
		for(j=0;j<n1;j++)
		{
			memcpy(&gnline,temp,16);
			temp+=16;
			if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
			pDoc->Save_EMMLine(&gnline);
			CloseMouseCursor();
			DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
			OpenMouseCursor();
		}
		temp=linebuf2;
		for(j=0;j<n2;j++)
		{
			memcpy(&gnline,temp,16);
			temp+=16;
			if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
			pDoc->Save_EMMLine(&gnline);
			CloseMouseCursor();
			DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
			OpenMouseCursor();
		}
    }
	if((n1!=0)&&(n2!=0)&&(n3==0)) return 1;
	else return 0;
}

int CDesignView::Route_Z_D(struct Route_Rats * route, int mode)
{
	int i,j,n,n1,n2,n3,v1,v2,num,flag;
   char *temp;
   char linebuf1[80];
   char linebuf2[80];
   char linebuf3[80];
   long int delta1[3] = { 0,-1,1}, delta2[3]= {0,1,-1};
   struct Route_Rats route1,route2,route3;
   struct Junction routevia1, routevia2;
   unsigned tempx1,tempy1,tempx2,tempy2;
   unsigned tx1,ty1,tx2,ty2;
	struct Undo_Head head;
	char *undo_p;

	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

   routevia1.id=0x22;
   routevia1.Dx=routevia1.Dy=Route_TW;
   routevia1.d=1;
   routevia1.padtype=1;
   routevia1.attrib=PCB;
   routevia1.layer=MULTILAYER;
   routevia1.flag=0;
   strcpy((char*)routevia1.padname,Null_Str);
   routevia2.id=0x22;
   routevia2.Dx=routevia2.Dy=Route_TW;
   routevia2.d=1;
   routevia2.padtype=1;
   routevia2.attrib=PCB;
   routevia2.layer=MULTILAYER;
   routevia2.flag=0;
   strcpy((char*)routevia2.padname,Null_Str);

   DrawAnyLine(route->sx,route->sy,route->ex,route->ey,Highlighted_Color,1,&dc);
   /****************************/
   /*                          */
   /*   O------- x t1          */
   /* sx,sy      |             */
   /*            |             */
   /*            |             */
   /*            |       ex,ey */
   /*        t2  x ----------O */
   /*                          */
   /****************************/
//ZMODE1:
	if((mode==0)||(mode==1))
	{
		num=ROUTEDELTA/CL_Snap_Grid;
		n = (route->ex-route->sx)/CL_Snap_Grid;
		tx1=route->sx;
		ty1=route->sy;
		tx2=route->ex;
		ty2=route->ey;
		flag=0;
		for(;;)
		{
			for(i=0;i<3;i++)
			{
				/* t1 position */
				tempx1=tx1+CL_Snap_Grid*num;
				tempy1=(long)ty1+delta1[i]*ROUTEDELTA;
				/* via1 at t1 */
				routevia1.centerx=tempx1;
				routevia1.centery=tempy1;
				v1=Clearance_Route((struct Line*)&routevia1);
				if(v1!=0) continue;
				else break;
			}
			if(v1!=0)
			{
				num++;
		       if(num>n) break;
		       continue;
		    }
			/* route1 from START(sx,sy) to t1 */
			route1.sx=tx1;
			route1.sy=ty1;
			route1.slayer=route->slayer;
			route1.ex=tempx1;
			route1.ey=tempy1;
			route1.elayer=MULTILAYER;
			n1=Route_Hor_Line_D(&route1,linebuf1);
			if(n1==0)
			{
				if(i==0) break;
			    else 
				{ 
					num++ ;
					if(num>n) break;
					continue;
			    }
			}
			for(i=0;i<3;i++)
			{
				/* t2 position */
				tempx2=tempx1;
				tempy2=(long)ty2+delta2[i]*ROUTEDELTA;
				/* via2 at t2 */
				routevia2.centerx=tempx2;
				routevia2.centery=tempy2;
				v2=Clearance_Route((struct Line*)&routevia2);
				if(v2!=0)  continue;
				else break;
			}
			if(v2!=0) 
			{
				num++;
		       if(num>n) break;
		       continue;
		     }
			/* route3 from END(ex,ey) to t2 */
			route3.sx=tempx2;
			route3.sy=tempy2;
			route3.slayer=MULTILAYER;
			route3.ex=tx2;
			route3.ey=ty2;
			route3.elayer=route->elayer;
			n3=Route_Hor_Line_D(&route3,linebuf3);
			if(n3==0)
			{
				if(flag==0)
				{
					route3.sx=tx2-2*ROUTEDELTA;
					route3.sy=ty2;
					route3.ex=tx2;
					route3.ey=ty2;
					if(Route_Hor_Line_D(&route3,linebuf3)==0) break;
					flag=1;
				}
		       /*num+=2;*/
				num++;
				if(num>n) break;
				continue;
			}
			/* route2 from t1 to t2 */
			if(tempy2>tempy1)
			{
				route2.sx=tempx1;
				route2.sy=tempy1;
				route2.slayer=MULTILAYER;
				route2.ex=tempx2;
				route2.ey=tempy2;
				route2.elayer=MULTILAYER;
			}
			else
		    {
				route2.sx=tempx2;
				route2.sy=tempy2;
				route2.slayer=MULTILAYER;
				route2.ex=tempx1;
				route2.ey=tempy1;
				route2.elayer=MULTILAYER;
			}
			n2=Route_Ver_Line_D(&route2,linebuf2);
			if(n2==0)
			{
				num++;
		       if(num>n) break;
		       continue;
		    }
			/* route OK! or not */
			if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)) goto ZEND;
		}
	}
   /****************************/
   /*   O  ex,ey               */
   /*   |                      */
   /*   |                      */
   /*   |                 t1   */
   /*   x ---------------- x   */
   /*  t2                  |   */
   /*                      |   */
   /*                      |   */
   /*               sx,sy  O   */
   /****************************/
//ZMODE2:
	if((mode==0)||(mode==2))
	{
		if(route->sy>route->ey) 
		{
			tempx1=route->sx; tempy1=route->sy;
			route->sx=route->ex; route->sy=route->ey;
			route->ex=tempx1;  route->ey=tempy1;
			tempx1=route->slayer;
			route->slayer=route->elayer;
			route->elayer=tempx1;
		}
		num=ROUTEDELTA/CL_Snap_Grid;
		n = (route->ey-route->sy)/CL_Snap_Grid;
		tx1=route->sx;
		ty1=route->sy;
		tx2=route->ex;
		ty2=route->ey;
		flag=0;
		for(;;)
		{
			for(i=0;i<3;i++)
			{
				/* t1 position */
				tempx1=(long)tx1+delta1[i]*ROUTEDELTA;
				tempy1=ty1+CL_Snap_Grid*num;
				/* via1 at t1 */
				routevia1.centerx=tempx1;
				routevia1.centery=tempy1;
				v1=Clearance_Route((struct Line*)&routevia1);
				if(v1!=0) continue;
				else break;
			}
			if(v1!=0) 
			{
				num++;
				if(num>n) break;
				continue;
			}
			/* route1 from START(sx,sy) to t1 */
			route1.sx=tx1;
			route1.sy=ty1;
			route1.slayer=route->slayer;
			route1.ex=tempx1;
			route1.ey=tempy1;
			route1.elayer=MULTILAYER;
			n1=Route_Ver_Line_D(&route1,linebuf1);
			if(n1==0)
			{
				if(i==0) break;
				else
				{
					num++ ;
					if(num>n) break;
					continue;
			    }
			}
			for(i=0;i<3;i++)
			{
				/* t2 position */
				tempx2=(long)tx2-delta2[i]*ROUTEDELTA;
				tempy2=tempy1;
				/* via2 at t2 */
				routevia2.centerx=tempx2;
				routevia2.centery=tempy2;
				v2=Clearance_Route((struct Line*)&routevia2);
				if(v2!=0)  continue;
				else break;
			}
			if(v2!=0) 
			{
				num++;
				if(num>n) break;
				continue;
			}
			/* route3 from END(ex,ey) to t2 */
			route3.sx=tempx2;
			route3.sy=tempy2;
			route3.slayer=MULTILAYER;
			route3.ex=tx2;
			route3.ey=ty2;
			route3.elayer=route->elayer;
			n3=Route_Ver_Line_D(&route3,linebuf3);
			if(n3==0)
			{
				if(flag==0)
				{
					route3.sx=tx2;
					route3.sy=ty2-2*ROUTEDELTA;
					route3.ex=tx2;
					route3.ey=ty2;
					if(Route_Ver_Line_D(&route3,linebuf3)==0) break;
					flag=1;
				}
		       /*num+=2;*/ 
				num++;
		       if(num>n) break;
		       continue;
			}
			/* route2 from t1 to t2 */
			if(tempx2>tempx1)
			{
				route2.sx=tempx1;
				route2.sy=tempy1;
				route2.slayer=MULTILAYER;
				route2.ex=tempx2;
				route2.ey=tempy2;
				route2.elayer=MULTILAYER;
			}
			else
			{
				route2.sx=tempx2;
				route2.sy=tempy2;
				route2.slayer=MULTILAYER;
				route2.ex=tempx1;
				route2.ey=tempy1;
				route2.elayer=MULTILAYER;
			}
			n2=Route_Hor_Line_D(&route2,linebuf2);
			if(n2==0)
			{
				num++;
				if(num>n) break;
				continue;
		    }
			if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)) goto ZEND;
		}
	}
ZEND:
	CloseMouseCursor();
	DrawAnyLine(route->sx,route->sy,route->ex,route->ey,Highlighted_Color,1,&dc);
	DrawPCBHighlight(0);
	OpenMouseCursor();
	if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0))
     {
		temp=linebuf1;
		for(j=0;j<n1;j++)
		{
			memcpy(&gnline,temp,16);
			temp+=16;
			if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
			pDoc->Save_EMMLine(&gnline);
			CloseMouseCursor();
			DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
			OpenMouseCursor();
		}
		temp=linebuf2;
		for(j=0;j<n2;j++)
		{
			memcpy(&gnline,temp,16);
			temp+=16;
			if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
			pDoc->Save_EMMLine(&gnline);
			CloseMouseCursor();
			DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
			OpenMouseCursor();
		}
		temp=linebuf3;
		for(j=0;j<n3;j++)
		{
			memcpy(&gnline,temp,16);
			temp+=16;
			if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
			pDoc->Save_EMMLine(&gnline);
			CloseMouseCursor();
			DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
			OpenMouseCursor();
		}
		return 1;
    }
	else return 0;
}


int CDesignView::Super_Z_D(struct Route_Rats * route)
{

	int ii,j,k,l,m,t1,t2,flag;
	char *temp;
	long int try1,try2;
	struct Route_Rats route1;
	unsigned tx1,ty1,tx2,ty2;
	unsigned tempx,tempy;
	struct Route_Rats temproute1, temproute2;
	struct Junction tempvia1,tempvia2;
	char templinebuf1[80], templinebuf2[80];
	struct Undo_Head head;
	char *undo_p;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	tempvia1.id=0x22;
	tempvia1.Dx=tempvia1.Dy=Route_TW;
	tempvia1.d=1;
   tempvia1.padtype=1;
   tempvia1.attrib=PCB;
   tempvia1.layer=MULTILAYER;
   tempvia1.flag=0;
   strcpy((char*)tempvia1.padname,Null_Str);
   tempvia2.id=0x22;
   tempvia2.Dx=tempvia2.Dy=Route_TW;
   tempvia2.d=1;
   tempvia2.padtype=1;
   tempvia2.attrib=PCB;
   tempvia2.layer=MULTILAYER;
   tempvia2.flag=0;
   strcpy((char*)tempvia2.padname,Null_Str);

   DrawAnyLine(route->sx,route->sy,route->ex,route->ey,Highlighted_Color,1,&dc);
   /****************************/
   /*   O                      */
   /*   |------- x t1          */
   /* sx,sy      |             */
   /*            |             */
   /*            |             */
   /*            |       ex,ey */
   /*        t2  x ----------| */
   /*                        O */
   /****************************/
//SZMODE1:
   if(route->sy>route->ey) flag=0;
   else flag=1;
   tx1=route->sx;
   ty1=route->sy;
   tx2=route->ex;
   ty2=route->ey;

   if(flag==0) try1=-1l;
   else try1=1l;
   k=0;
   for(;;)
   {
	  if(flag==0)
	  {
		  if(try1==0) try1+=3l;
			else try1++;
	  }
	  else
	  {
		  if(try1==0) try1-=3l;
	      else try1--;
	  }
	  ty1=(long)(route->sy)-try1*CL_Snap_Grid;
	  if((ty1>=CL_Size_Maxy)||(ty1<ROUTEDELTA)||\
	     ((ty1<=(route->ey+ROUTEDELTA))&&(flag==0))||\
	     ((ty1>=(route->ey-ROUTEDELTA))&&(flag==1)))
	  {
		  goto SZMODE2;
	  }
	  tempvia1.centerx=tx1;
	  tempvia1.centery=ty1;
	  l=Clearance_Route((struct Line*)&tempvia1);
	  if(l!=0) continue;
	  temproute1.sx=tx1;
	  temproute1.sy=ty1;
	  temproute1.slayer=MULTILAYER;
	  temproute1.ex=route->sx;
	  temproute1.ey=route->sy;
	  temproute1.elayer=route->slayer;
	  if(temproute1.sy>temproute1.ey)
	  {
		  tempx=temproute1.sx; tempy=temproute1.sy;
	      temproute1.sx=temproute1.ex; temproute1.sy=temproute1.ey;
	      temproute1.ex=tempx;  temproute1.ey=tempy;
	      tempx=temproute1.slayer;
	      temproute1.slayer=temproute1.elayer;
	      temproute1.elayer=tempx;
	   }
		if(((temproute1.slayer!=MULTILAYER)||(temproute1.elayer!=MULTILAYER))&&\
	     (temproute1.sx==temproute1.ex)&&(temproute1.sy==temproute1.ey)) continue;
		t1=Route_Ver_Line_D(&temproute1,templinebuf1);
		if(t1==0)
		{
			goto SZMODE2;
	    }
		else
		{
			if(flag==0) try2=-1l;
			else try2=1l;
			for(;;)
			{
				// interupt route
				for(;;)
				{
					int ch2 = MRetKey();
					if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
					{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
						if( ii == IDYES)
						{
							return -1;
						}
					}
					break;
				}

			   if(flag==0)
			   {
				   if(try2==0) try2+=3l;
				   else try2++;
			   }
			   else
			   {
				   if(try2==0) try2-=3l;
				   else try2--;
			   }
			   ty2=(long)(route->ey)+try2*CL_Snap_Grid;
			   if((ty2>=CL_Size_Maxy)||(ty2<ROUTEDELTA)||\
			      ((ty2>=(route->sy-ROUTEDELTA))&&(flag==0))||\
			      ((ty2<=(route->sy+ROUTEDELTA))&&(flag==1)))
			   {
				   break;
			   }
			   tempvia2.centerx=tx2;
			   tempvia2.centery=ty2;
			   m=Clearance_Route((struct Line*)&tempvia2);
			   if(m!=0) continue;
			   temproute2.sx=route->ex;
			   temproute2.sy=route->ey;
			   temproute2.slayer=route->elayer;
			   temproute2.ex=tx2;
			   temproute2.ey=ty2;
			   temproute2.elayer=MULTILAYER;
			   if(temproute2.sy>temproute2.ey)
			   {  tempx=temproute2.sx; tempy=temproute2.sy;
			      temproute2.sx=temproute2.ex; temproute2.sy=temproute2.ey;
			      temproute2.ex=tempx;  temproute2.ey=tempy;
			      tempx=temproute2.slayer;
			      temproute2.slayer=temproute2.elayer;
			      temproute2.elayer=tempx;
			   }
			   if(((temproute2.slayer!=MULTILAYER)||(temproute2.elayer!=MULTILAYER))&&\
			      (temproute2.sx==temproute2.ex)&&(temproute2.sy==temproute2.ey)) continue;
			   t2=Route_Ver_Line_D(&temproute2,templinebuf2);
			   if(t2==0)  break;
			   else 
			   {
				   route1.sx=tx1; route1.sy=ty1; route1.slayer=MULTILAYER;
				   route1.ex=tx2; route1.ey=ty2; route1.elayer=MULTILAYER;
				   if(route1.sx>route1.ex)
				   {
					   tempx=route1.sx; tempy=route1.sy;
						route1.sx=route1.ex; route1.sy=route1.ey;
						route1.ex=tempx;  route1.ey=tempy;
				   }
				   if(Route_Z_D(&route1,1)!=0)
				   {
					   k=1;
					   goto SZEND;
				   }
			   }
			}
		}
	}

   /****************************/
   /* O--  ex,ey               */
   /*   |                      */
   /*   |                      */
   /*   |                 t1   */
   /*   x ---------------- x   */
   /*  t2                  |   */
   /*                      |   */
   /*                      |   */
   /*               sx,sy  --O */
   /****************************/
SZMODE2:
   if(route->sy>route->ey) 
   {
	   tempx=route->sx; tempy=route->sy;
	   route->sx=route->ex; route->sy=route->ey;
	   route->ex=tempx;  route->ey=tempy;
	   tempx=route->slayer;
	   route->slayer=route->elayer;
	   route->elayer=tempx;
   }
   if(route->sx>route->ex) flag=0;
   else flag=1;
   if(flag==0) try1=-1l;
   else try1=1l;
   tx1=route->sx;
   ty1=route->sy;
   tx2=route->ex;
   ty2=route->ey;
   for(;;)
   {
	  if(flag==0)
	   { if(try1==0) try1+=3l;
	     else try1++;
	   }
	  else
	   { if(try1==0) try1-=3l;
	      else try1--;
	   }
	  tx1=(long)(route->sx)-try1*CL_Snap_Grid;
	  if((tx1>=CL_Size_Maxx)||(tx1<ROUTEDELTA)||\
	     ((tx1<=(route->ex+ROUTEDELTA))&&(flag==0))||\
	     ((tx1>=(route->ex-ROUTEDELTA))&&(flag==1)))
	    {   goto SZMODE3;
	    }
	  tempvia1.centerx=tx1;
	  tempvia1.centery=ty1;
	  l=Clearance_Route((struct Line*)&tempvia1);
	  if(l!=0) continue;
	  temproute1.sx=tx1;
	  temproute1.sy=ty1;
	  temproute1.slayer=MULTILAYER;
	  temproute1.ex=route->sx;
	  temproute1.ey=route->sy;
	  temproute1.elayer=route->slayer;
	  if(temproute1.sx>temproute1.ex)
	    { tempx=temproute1.sx; tempy=temproute1.sy;
	      temproute1.sx=temproute1.ex; temproute1.sy=temproute1.ey;
	      temproute1.ex=tempx;  temproute1.ey=tempy;
	      tempx=temproute1.slayer;
	      temproute1.slayer=temproute1.elayer;
	      temproute1.elayer=tempx;
	    }
	  if(((temproute1.slayer!=MULTILAYER)||(temproute1.elayer!=MULTILAYER))&&\
	  (temproute1.sx==temproute1.ex)&&(temproute1.sy==temproute1.ey)) continue;
	  t1=Route_Hor_Line_D(&temproute1,templinebuf1);
	  if(t1==0){    goto SZMODE3;
		   }
	  else {  if(flag==0) try2=-1l;
		  else try2=1l;
		  for(;;){
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						return -1;
					}
				}
				break;
			}

			   if(flag==0)
			    { if(try2==0) try2+=3l;
			      else try2++;
			    }
			   else
			    { if(try2==0) try2-=3l;
			      else try2--;
			    }
			   tx2=(long)(route->ex)+try2*CL_Snap_Grid;
			   if((tx2>=CL_Size_Maxx)||(tx2<50)||\
			      ((tx2>=(route->sx-ROUTEDELTA))&&(flag==0))||\
			      ((tx2<=(route->sx+ROUTEDELTA))&&(flag==1)))
			      {   break;
			      }
			   tempvia2.centerx=tx2;
			   tempvia2.centery=ty2;
			   m=Clearance_Route((struct Line*)&tempvia2);
			   if(m!=0) continue;
			   temproute2.sx=route->ex;
			   temproute2.sy=route->ey;
			   temproute2.slayer=route->elayer;
			   temproute2.ex=tx2;
			   temproute2.ey=ty2;
			   temproute2.elayer=MULTILAYER;
			   if(temproute2.sx>temproute2.ex)
			    { tempx=temproute2.sx; tempy=temproute2.sy;
			      temproute2.sx=temproute2.ex; temproute2.sy=temproute2.ey;
			      temproute2.ex=tempx;  temproute2.ey=tempy;
			      tempx=temproute2.slayer;
			      temproute2.slayer=temproute2.elayer;
			      temproute2.elayer=tempx;
			    }
			   if(((temproute2.slayer!=MULTILAYER)||(temproute2.elayer!=MULTILAYER))&&\
			      (temproute2.sx==temproute2.ex)&&(temproute2.sy==temproute2.ey)) continue;
			   t2=Route_Hor_Line_D(&temproute2,templinebuf2);
			   if(t2==0){  break;
				    }
			   else {  route1.sx=tx1; route1.sy=ty1; route1.slayer=MULTILAYER;
				   route1.ex=tx2; route1.ey=ty2; route1.elayer=MULTILAYER;
				   if(route1.sx>route1.ex)
				     {    tempx=route1.sx; tempy=route1.sy;
					  route1.sx=route1.ex; route1.sy=route1.ey;
					  route1.ex=tempx;  route1.ey=tempy;
				     }
				   if(Route_Z_D(&route1,2)!=0)
				     { k=1;
				       goto SZEND;
				     }
				}
			}

	       }
	}

   /****************************/
   /*                          */
   /*   |------- x t1          */
   /*   O        |             */
   /* sx,sy      |             */
   /*            |             */
   /*            |       ex,ey */
   /*        t2  x ----------| */
   /*                        O */
   /****************************/
SZMODE3:
   if(route->sy>route->ey) flag=0;
   else flag=1;
   tx1=route->sx;
   ty1=route->sy;
   tx2=route->ex;
   ty2=route->ey;

   if(flag==0) try1=-2l; /*1l;*/
   else try1=2l; /*-1l;*/
   k=0;
   for(;;)
	{
	  if(flag==0) try1--;
	  else try1++;
	  ty1=(long)(route->sy)-try1*CL_Snap_Grid;
	  if((ty1>=CL_Size_Maxy)||(ty1<ROUTEDELTA)||\
	     ((ty1>=(route->sy+10*ROUTEDELTA))&&(flag==0))||\
	     ((ty1<=(route->sy-10*ROUTEDELTA))&&(flag==1)))
	      { goto SZMODE4;
	      }
	  tempvia1.centerx=tx1;
	  tempvia1.centery=ty1;
	  l=Clearance_Route((struct Line*)&tempvia1);
	  if(l!=0) continue;
	  temproute1.sx=tx1;
	  temproute1.sy=ty1;
	  temproute1.slayer=MULTILAYER;
	  temproute1.ex=route->sx;
	  temproute1.ey=route->sy;
	  temproute1.elayer=route->slayer;
	  if(temproute1.sy>temproute1.ey)
	   {  tempx=temproute1.sx; tempy=temproute1.sy;
	      temproute1.sx=temproute1.ex; temproute1.sy=temproute1.ey;
	      temproute1.ex=tempx;  temproute1.ey=tempy;
	      tempx=temproute1.slayer;
	      temproute1.slayer=temproute1.elayer;
	      temproute1.elayer=tempx;
	   }
	  if(((temproute1.slayer!=MULTILAYER)||(temproute1.elayer!=MULTILAYER))&&\
	     (temproute1.sx==temproute1.ex)&&(temproute1.sy==temproute1.ey)) continue;
	  t1=Route_Ver_Line_D(&temproute1,templinebuf1);
	  if(t1==0) {   goto SZMODE4;
		    }
	  else {   if(flag==0) try2=-1l;
		   else try2=1l;
		   for(;;)
			{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						return -1;
					}
				}
				break;
			}

			   if(flag==0)
			    { if(try2==0) try2+=3l;
			      else try2++;
			    }
			   else
			    { if(try2==0) try2-=3l;
			      else try2--;
			    }
			   ty2=(long)(route->ey)+try2*CL_Snap_Grid;
			   if((ty2>=CL_Size_Maxy)||(ty2<ROUTEDELTA)||\
			      ((ty2>=(route->sy-ROUTEDELTA))&&(flag==0))||\
			      ((ty2<=(route->sy+ROUTEDELTA))&&(flag==1)))
			      {   break;
			      }
			   tempvia2.centerx=tx2;
			   tempvia2.centery=ty2;
			   m=Clearance_Route((struct Line*)&tempvia2);
			   if(m!=0) continue;
			   temproute2.sx=route->ex;
			   temproute2.sy=route->ey;
			   temproute2.slayer=route->elayer;
			   temproute2.ex=tx2;
			   temproute2.ey=ty2;
			   temproute2.elayer=MULTILAYER;
			   if(temproute2.sy>temproute2.ey)
			   {  tempx=temproute2.sx; tempy=temproute2.sy;
			      temproute2.sx=temproute2.ex; temproute2.sy=temproute2.ey;
			      temproute2.ex=tempx;  temproute2.ey=tempy;
			      tempx=temproute2.slayer;
			      temproute2.slayer=temproute2.elayer;
			      temproute2.elayer=tempx;
			   }
			   if(((temproute2.slayer!=MULTILAYER)||(temproute2.elayer!=MULTILAYER))&&\
			      (temproute2.sx==temproute2.ex)&&(temproute2.sy==temproute2.ey)) continue;
			   t2=Route_Ver_Line_D(&temproute2,templinebuf2);
			   if(t2==0) {  break;
				     }
			   else {  route1.sx=tx1; route1.sy=ty1; route1.slayer=MULTILAYER;
				   route1.ex=tx2; route1.ey=ty2; route1.elayer=MULTILAYER;
				   if(route1.sx>route1.ex)
				    { tempx=route1.sx; tempy=route1.sy;
				      route1.sx=route1.ex; route1.sy=route1.ey;
				      route1.ex=tempx;  route1.ey=tempy;
				    }
				   if(Route_Z_D(&route1,1)!=0)
				     { k=1;
				       goto SZEND;
				     }
				}
		    }
	       }
	}

SZMODE4:
   if(route->sy>route->ey) flag=0;
   else flag=1;
   tx1=route->sx;
   ty1=route->sy;
   tx2=route->ex;
   ty2=route->ey;

   if(flag==0) try1=-1l;
   else try1=1l;
   k=0;
   for(;;)
	{
	  if(flag==0)
	   { if(try1==0) try1+=3l;
	     else try1++;
	   }
	  else
	   { if(try1==0) try1-=3l;
	      else try1--;
	   }
	  ty1=(long)(route->sy)-try1*CL_Snap_Grid;
	  if((ty1>=CL_Size_Maxy)||(ty1<ROUTEDELTA)||\
	     ((ty1<=(route->ey+ROUTEDELTA))&&(flag==0))||\
	     ((ty1>=(route->ey-ROUTEDELTA))&&(flag==1)))
	      { goto SZMODE5;
	      }
	  tempvia1.centerx=tx1;
	  tempvia1.centery=ty1;
	  l=Clearance_Route((struct Line*)&tempvia1);
	  if(l!=0) continue;
	  temproute1.sx=tx1;
	  temproute1.sy=ty1;
	  temproute1.slayer=MULTILAYER;
	  temproute1.ex=route->sx;
	  temproute1.ey=route->sy;
	  temproute1.elayer=route->slayer;
	  if(temproute1.sy>temproute1.ey)
	   {  tempx=temproute1.sx; tempy=temproute1.sy;
	      temproute1.sx=temproute1.ex; temproute1.sy=temproute1.ey;
	      temproute1.ex=tempx;  temproute1.ey=tempy;
	      tempx=temproute1.slayer;
	      temproute1.slayer=temproute1.elayer;
	      temproute1.elayer=tempx;
	   }
	  if(((temproute1.slayer!=MULTILAYER)||(temproute1.elayer!=MULTILAYER))&&\
	     (temproute1.sx==temproute1.ex)&&(temproute1.sy==temproute1.ey)) continue;
	  t1=Route_Ver_Line_D(&temproute1,templinebuf1);
	  if(t1==0) {   goto SZMODE5;
		    }
	  else {   if(flag==0) try2=-2l; /*1l;*/
		   else try2=2l;  /*-1l;*/
		   for(;;)
			{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						return -1;
					}
				}
				break;
			}

			   if(flag==0) try2--;
			   else try2++;
			   ty2=(long)(route->ey)+try2*CL_Snap_Grid;
			   if((ty2>=CL_Size_Maxy)||(ty2<ROUTEDELTA)||\
			      ((ty2<=(route->ey-10*ROUTEDELTA))&&(flag==0))||\
			      ((ty2>=(route->ey+10*ROUTEDELTA))&&(flag==1)))
			      {   break;
			      }
			   tempvia2.centerx=tx2;
			   tempvia2.centery=ty2;
			   m=Clearance_Route((struct Line*)&tempvia2);
			   if(m!=0) continue;
			   temproute2.sx=route->ex;
			   temproute2.sy=route->ey;
			   temproute2.slayer=route->elayer;
			   temproute2.ex=tx2;
			   temproute2.ey=ty2;
			   temproute2.elayer=MULTILAYER;
			   if(temproute2.sy>temproute2.ey)
			   {  tempx=temproute2.sx; tempy=temproute2.sy;
			      temproute2.sx=temproute2.ex; temproute2.sy=temproute2.ey;
			      temproute2.ex=tempx;  temproute2.ey=tempy;
			      tempx=temproute2.slayer;
			      temproute2.slayer=temproute2.elayer;
			      temproute2.elayer=tempx;
			   }
			   if(((temproute2.slayer!=MULTILAYER)||(temproute2.elayer!=MULTILAYER))&&\
			      (temproute2.sx==temproute2.ex)&&(temproute2.sy==temproute2.ey)) continue;
			   t2=Route_Ver_Line_D(&temproute2,templinebuf2);
			   if(t2==0) {  break;
				     }
			   else {  route1.sx=tx1; route1.sy=ty1; route1.slayer=MULTILAYER;
				   route1.ex=tx2; route1.ey=ty2; route1.elayer=MULTILAYER;
				   if(route1.sx>route1.ex)
				    { tempx=route1.sx; tempy=route1.sy;
				      route1.sx=route1.ex; route1.sy=route1.ey;
				      route1.ex=tempx;  route1.ey=tempy;
				    }
				   if(Route_Z_D(&route1,1)!=0)
				     { k=1;
				       goto SZEND;
				     }
				}
		    }
	       }
	}

   /****************************/
   /*   --O  ex,ey             */
   /*   |                      */
   /*   |                      */
   /*   |                 t1   */
   /*   x ---------------- x   */
   /*  t2                  |   */
   /*                      |   */
   /*                      |   */
   /*               sx,sy  --O */
   /****************************/
SZMODE5:
   if(route->sy>route->ey) { tempx=route->sx; tempy=route->sy;
			     route->sx=route->ex; route->sy=route->ey;
			     route->ex=tempx;  route->ey=tempy;
			     tempx=route->slayer;
			     route->slayer=route->elayer;
			     route->elayer=tempx;
			   }
   if(route->sx>route->ex) flag=0;
   else flag=1;
   if(flag==0) try1=-2l;  /*1l;*/
   else try1=2l;  /*-1l;*/
   tx1=route->sx;
   ty1=route->sy;
   tx2=route->ex;
   ty2=route->ey;
   for(;;)
	{
	  if(flag==0) try1--;
	  else try1++;
	  tx1=(long)(route->sx)-try1*CL_Snap_Grid;
	  if((tx1>=CL_Size_Maxx)||(tx1<ROUTEDELTA)||\
	     ((tx1>=(route->sx+10*ROUTEDELTA))&&(flag==0))||\
	     ((tx1<=(route->sx-10*ROUTEDELTA))&&(flag==1)))
	    {   goto SZMODE6;
	    }
	  tempvia1.centerx=tx1;
	  tempvia1.centery=ty1;
	  l=Clearance_Route((struct Line*)&tempvia1);
	  if(l!=0) continue;
	  temproute1.sx=tx1;
	  temproute1.sy=ty1;
	  temproute1.slayer=MULTILAYER;
	  temproute1.ex=route->sx;
	  temproute1.ey=route->sy;
	  temproute1.elayer=route->slayer;
	  if(temproute1.sx>temproute1.ex)
	    { tempx=temproute1.sx; tempy=temproute1.sy;
	      temproute1.sx=temproute1.ex; temproute1.sy=temproute1.ey;
	      temproute1.ex=tempx;  temproute1.ey=tempy;
	      tempx=temproute1.slayer;
	      temproute1.slayer=temproute1.elayer;
	      temproute1.elayer=tempx;
	    }
	  if(((temproute1.slayer!=MULTILAYER)||(temproute1.elayer!=MULTILAYER))&&\
	     (temproute1.sx==temproute1.ex)&&(temproute1.sy==temproute1.ey)) continue;
	  t1=Route_Hor_Line_D(&temproute1,templinebuf1);
	  if(t1==0){    goto SZMODE6;
		   }
	  else {  if(flag==0) try2=-1l;
		  else try2=1l;
		  for(;;)
			{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
					return -1;
					}
				}
				break;
			}

			   if(flag==0)
			    { if(try2==0) try2+=3l;
			      else try2++;
			    }
			   else
			    { if(try2==0) try2-=3l;
			      else try2--;
			    }
			   tx2=(long)(route->ex)+try2*CL_Snap_Grid;
			   if((tx2>=CL_Size_Maxx)||(tx2<50)||\
			      ((tx2>=(route->sx-ROUTEDELTA))&&(flag==0))||\
			      ((tx2<=(route->sx+ROUTEDELTA))&&(flag==1)))
			      {   break;
			      }
			   tempvia2.centerx=tx2;
			   tempvia2.centery=ty2;
			   m=Clearance_Route((struct Line*)&tempvia2);
			   if(m!=0) continue;
			   temproute2.sx=route->ex;
			   temproute2.sy=route->ey;
			   temproute2.slayer=route->elayer;
			   temproute2.ex=tx2;
			   temproute2.ey=ty2;
			   temproute2.elayer=MULTILAYER;
			   if(temproute2.sx>temproute2.ex)
			    { tempx=temproute2.sx; tempy=temproute2.sy;
			      temproute2.sx=temproute2.ex; temproute2.sy=temproute2.ey;
			      temproute2.ex=tempx;  temproute2.ey=tempy;
			      tempx=temproute2.slayer;
			      temproute2.slayer=temproute2.elayer;
			      temproute2.elayer=tempx;
			    }
			   if(((temproute2.slayer!=MULTILAYER)||(temproute2.elayer!=MULTILAYER))&&\
			      (temproute2.sx==temproute2.ex)&&(temproute2.sy==temproute2.ey)) continue;
			   t2=Route_Hor_Line_D(&temproute2,templinebuf2);
			   if(t2==0){  break;
				    }
			   else {  route1.sx=tx1; route1.sy=ty1; route1.slayer=MULTILAYER;
				   route1.ex=tx2; route1.ey=ty2; route1.elayer=MULTILAYER;
				   if(route1.sx>route1.ex)
				     {    tempx=route1.sx; tempy=route1.sy;
					  route1.sx=route1.ex; route1.sy=route1.ey;
					  route1.ex=tempx;  route1.ey=tempy;
				     }
				   if(Route_Z_D(&route1,2)!=0)
				     { k=1;
				       goto SZEND;
				     }
				}
			}

	       }
	}

SZMODE6:
   if(route->sy>route->ey) { tempx=route->sx; tempy=route->sy;
			     route->sx=route->ex; route->sy=route->ey;
			     route->ex=tempx;  route->ey=tempy;
			     tempx=route->slayer;
			     route->slayer=route->elayer;
			     route->elayer=tempx;
			   }
   if(route->sx>route->ex) flag=0;
   else flag=1;
   if(flag==0) try1=-1l;
   else try1=1l;
   tx1=route->sx;
   ty1=route->sy;
   tx2=route->ex;
   ty2=route->ey;
   for(;;)
	{
	  if(flag==0)
	   { if(try1==0) try1+=3l;
	     else try1++;
	   }
	  else
	   { if(try1==0) try1-=3l;
	      else try1--;
	   }
	  tx1=(long)(route->sx)-try1*CL_Snap_Grid;
	  if((tx1>=CL_Size_Maxx)||(tx1<ROUTEDELTA)||\
	     ((tx1<=(route->ex+ROUTEDELTA))&&(flag==0))||\
	     ((tx1>=(route->ex-ROUTEDELTA))&&(flag==1)))
	    {   goto SZMODE7;
	    }
	  tempvia1.centerx=tx1;
	  tempvia1.centery=ty1;
	  l=Clearance_Route((struct Line*)&tempvia1);
	  if(l!=0) continue;
	  temproute1.sx=tx1;
	  temproute1.sy=ty1;
	  temproute1.slayer=MULTILAYER;
	  temproute1.ex=route->sx;
	  temproute1.ey=route->sy;
	  temproute1.elayer=route->slayer;
	  if(temproute1.sx>temproute1.ex)
	    { tempx=temproute1.sx; tempy=temproute1.sy;
	      temproute1.sx=temproute1.ex; temproute1.sy=temproute1.ey;
	      temproute1.ex=tempx;  temproute1.ey=tempy;
	      tempx=temproute1.slayer;
	      temproute1.slayer=temproute1.elayer;
	      temproute1.elayer=tempx;
	    }
	  if(((temproute1.slayer!=MULTILAYER)||(temproute1.elayer!=MULTILAYER))&&\
	     (temproute1.sx==temproute1.ex)&&(temproute1.sy==temproute1.ey)) continue;
	  t1=Route_Hor_Line_D(&temproute1,templinebuf1);
	  if(t1==0){    goto SZMODE7;
		   }
	  else {  if(flag==0) try2=-2l; /*1l;*/
		  else try2=2l; /*-1l;*/
		  for(;;)
			{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						return -1;
					}
				}
				break;
			}

			   if(flag==0) try2--;
			   else try2++;
			   tx2=(long)(route->ex)+try2*CL_Snap_Grid;
			   if((tx2>=CL_Size_Maxx)||(tx2<50)||\
			      ((tx2<=(route->ex-10*ROUTEDELTA))&&(flag==0))||\
			      ((tx2>=(route->ex+10*ROUTEDELTA))&&(flag==1)))
			      {   break;
			      }
			   tempvia2.centerx=tx2;
			   tempvia2.centery=ty2;
			   m=Clearance_Route((struct Line*)&tempvia2);
			   if(m!=0) continue;
			   temproute2.sx=route->ex;
			   temproute2.sy=route->ey;
			   temproute2.slayer=route->elayer;
			   temproute2.ex=tx2;
			   temproute2.ey=ty2;
			   temproute2.elayer=MULTILAYER;
			   if(temproute2.sx>temproute2.ex)
			    { tempx=temproute2.sx; tempy=temproute2.sy;
			      temproute2.sx=temproute2.ex; temproute2.sy=temproute2.ey;
			      temproute2.ex=tempx;  temproute2.ey=tempy;
			      tempx=temproute2.slayer;
			      temproute2.slayer=temproute2.elayer;
			      temproute2.elayer=tempx;
			    }
			   if(((temproute2.slayer!=MULTILAYER)||(temproute2.elayer!=MULTILAYER))&&\
			      (temproute2.sx==temproute2.ex)&&(temproute2.sy==temproute2.ey)) continue;
			   t2=Route_Hor_Line_D(&temproute2,templinebuf2);
			   if(t2==0){  break;
				    }
			   else {  route1.sx=tx1; route1.sy=ty1; route1.slayer=MULTILAYER;
				   route1.ex=tx2; route1.ey=ty2; route1.elayer=MULTILAYER;
				   if(route1.sx>route1.ex)
				     {    tempx=route1.sx; tempy=route1.sy;
					  route1.sx=route1.ex; route1.sy=route1.ey;
					  route1.ex=tempx;  route1.ey=tempy;
				     }
				   if(Route_Z_D(&route1,2)!=0)
				     { k=1;
				       goto SZEND;
				     }
				}
			}

	       }
	}

SZMODE7:
   if(route->sy>route->ey) flag=0;
   else flag=1;
   tx1=route->sx;
   ty1=route->sy;
   tx2=route->ex;
   ty2=route->ey;

   if(flag==0) try1=1l;
   else try1=-1l;
   k=0;
   for(;;)
	{
	  if(flag==0)
	   { if(try1==0) try1-=3l;
	     else try1--;
	   }
	  else
	   { if(try1==0) try1+=3l;
	      else try1++;
	   }
	  ty1=(long)(route->sy)-try1*CL_Snap_Grid;
	  if((ty1>=CL_Size_Maxy)||(ty1<ROUTEDELTA)||\
	     ((ty1>=(route->sy+10*ROUTEDELTA))&&(flag==0))||\
	     ((ty1<=(route->sy-10*ROUTEDELTA))&&(flag==1)))
	      { goto SZMODE8;
	      }
	  tempvia1.centerx=tx1;
	  tempvia1.centery=ty1;
	  l=Clearance_Route((struct Line*)&tempvia1);
	  if(l!=0) continue;
	  temproute1.sx=tx1;
	  temproute1.sy=ty1;
	  temproute1.slayer=MULTILAYER;
	  temproute1.ex=route->sx;
	  temproute1.ey=route->sy;
	  temproute1.elayer=route->slayer;
	  if(temproute1.sy>temproute1.ey)
	     {  tempx=temproute1.sx; tempy=temproute1.sy;
		temproute1.sx=temproute1.ex; temproute1.sy=temproute1.ey;
		temproute1.ex=tempx;  temproute1.ey=tempy;
		tempx=temproute1.slayer;
		temproute1.slayer=temproute1.elayer;
		temproute1.elayer=tempx;
	     }
	  if(((temproute1.slayer!=MULTILAYER)||(temproute1.elayer!=MULTILAYER))&&\
	     (temproute1.sx==temproute1.ex)&&(temproute1.sy==temproute1.ey)) continue;
	  t1=Route_Ver_Line_D(&temproute1,templinebuf1);
	  if(t1==0) {   goto SZMODE8;
		    }
	  else {   if(flag==0) try2=1l;
		   else try2=-1l;
		   for(;;)
			{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						return -1;
					}
				}
				break;
			}

			   if(flag==0)
			    { if(try2==0) try2-=3l;
			      else try2--;
			    }
			   else
			    { if(try2==0) try2+=3l;
			      else try2++;
			    }
			   ty2=(long)(route->ey)+try2*CL_Snap_Grid;
			   if((ty2>=CL_Size_Maxy)||(ty2<ROUTEDELTA)||\
			      ((ty2<=(route->ey-10*ROUTEDELTA))&&(flag==0))||\
			      ((ty2>=(route->ey+10*ROUTEDELTA))&&(flag==1)))
			      {   break;
			      }
			   tempvia2.centerx=tx2;
			   tempvia2.centery=ty2;
			   m=Clearance_Route((struct Line*)&tempvia2);
			   if(m!=0) continue;
			   temproute2.sx=route->ex;
			   temproute2.sy=route->ey;
			   temproute2.slayer=route->elayer;
			   temproute2.ex=tx2;
			   temproute2.ey=ty2;
			   temproute2.elayer=MULTILAYER;
			   if(temproute2.sy>temproute2.ey)
			   {  tempx=temproute2.sx; tempy=temproute2.sy;
			      temproute2.sx=temproute2.ex; temproute2.sy=temproute2.ey;
			      temproute2.ex=tempx;  temproute2.ey=tempy;
			      tempx=temproute2.slayer;
			      temproute2.slayer=temproute2.elayer;
			      temproute2.elayer=tempx;
			   }
			   if(((temproute2.slayer!=MULTILAYER)||(temproute2.elayer!=MULTILAYER))&&\
			      (temproute2.sx==temproute2.ex)&&(temproute2.sy==temproute2.ey)) continue;
			   t2=Route_Ver_Line_D(&temproute2,templinebuf2);
			   if(t2==0) {  break;
				     }
			   else {  route1.sx=tx1; route1.sy=ty1; route1.slayer=MULTILAYER;
				   route1.ex=tx2; route1.ey=ty2; route1.elayer=MULTILAYER;
				   if(route1.sx>route1.ex)
				    { tempx=route1.sx; tempy=route1.sy;
				      route1.sx=route1.ex; route1.sy=route1.ey;
				      route1.ex=tempx;  route1.ey=tempy;
				    }
				   if(Route_Z_D(&route1,1)!=0)
				     { k=1;
				       goto SZEND;
				     }
				}
		    }
	       }
	}

SZMODE8:
   if(route->sy>route->ey) { tempx=route->sx; tempy=route->sy;
			     route->sx=route->ex; route->sy=route->ey;
			     route->ex=tempx;  route->ey=tempy;
			     tempx=route->slayer;
			     route->slayer=route->elayer;
			     route->elayer=tempx;
			   }
   if(route->sx>route->ex) flag=0;
   else flag=1;
   if(flag==0) try1=1l;
   else try1=-1l;
   tx1=route->sx;
   ty1=route->sy;
   tx2=route->ex;
   ty2=route->ey;
   for(;;)
	{
	  if(flag==0)
	   { if(try1==0) try1-=3l;
	     else try1--;
	   }
	  else
	   { if(try1==0) try1+=3l;
	      else try1++;
	   }
	  tx1=(long)(route->sx)-try1*CL_Snap_Grid;
	  if((tx1>=CL_Size_Maxx)||(tx1<ROUTEDELTA)||\
	     ((tx1>=(route->sx+10*ROUTEDELTA))&&(flag==0))||\
	     ((tx1<=(route->sx-10*ROUTEDELTA))&&(flag==1)))
	    {   goto SZEND;
	    }
	  tempvia1.centerx=tx1;
	  tempvia1.centery=ty1;
	  l=Clearance_Route((struct Line*)&tempvia1);
	  if(l!=0) continue;
	  temproute1.sx=tx1;
	  temproute1.sy=ty1;
	  temproute1.slayer=MULTILAYER;
	  temproute1.ex=route->sx;
	  temproute1.ey=route->sy;
	  temproute1.elayer=route->slayer;
	  if(temproute1.sx>temproute1.ex)
	    { tempx=temproute1.sx; tempy=temproute1.sy;
	      temproute1.sx=temproute1.ex; temproute1.sy=temproute1.ey;
	      temproute1.ex=tempx;  temproute1.ey=tempy;
	      tempx=temproute1.slayer;
	      temproute1.slayer=temproute1.elayer;
	      temproute1.elayer=tempx;
	    }
	  if(((temproute1.slayer!=MULTILAYER)||(temproute1.elayer!=MULTILAYER))&&\
	     (temproute1.sx==temproute1.ex)&&(temproute1.sy==temproute1.ey)) continue;
	  t1=Route_Hor_Line_D(&temproute1,templinebuf1);
	  if(t1==0){    goto SZEND;
		   }
	  else {  if(flag==0) try2=1l;
		  else try2=-1l;
		  for(;;)
			{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						return -1;
					}
				}
				break;
			}

			   if(flag==0)
			    { if(try2==0) try2-=3l;
			      else try2--;
			    }
			   else
			    { if(try2==0) try2+=3l;
			      else try2++;
			    }
			   tx2=(long)(route->ex)+try2*CL_Snap_Grid;
			   if((tx2>=CL_Size_Maxx)||(tx2<50)||\
			      ((tx2<=(route->ex-10*ROUTEDELTA))&&(flag==0))||\
			      ((tx2>=(route->ex+10*ROUTEDELTA))&&(flag==1)))
			      {   break;
			      }
			   tempvia2.centerx=tx2;
			   tempvia2.centery=ty2;
			   m=Clearance_Route((struct Line*)&tempvia2);
			   if(m!=0) continue;
			   temproute2.sx=route->ex;
			   temproute2.sy=route->ey;
			   temproute2.slayer=route->elayer;
			   temproute2.ex=tx2;
			   temproute2.ey=ty2;
			   temproute2.elayer=MULTILAYER;
			   if(temproute2.sx>temproute2.ex)
			    { tempx=temproute2.sx; tempy=temproute2.sy;
			      temproute2.sx=temproute2.ex; temproute2.sy=temproute2.ey;
			      temproute2.ex=tempx;  temproute2.ey=tempy;
			      tempx=temproute2.slayer;
			      temproute2.slayer=temproute2.elayer;
			      temproute2.elayer=tempx;
			    }
			   if(((temproute2.slayer!=MULTILAYER)||(temproute2.elayer!=MULTILAYER))&&\
			      (temproute2.sx==temproute2.ex)&&(temproute2.sy==temproute2.ey)) continue;
			   t2=Route_Hor_Line_D(&temproute2,templinebuf2);
			   if(t2==0){  break;
				    }
			   else {  route1.sx=tx1; route1.sy=ty1; route1.slayer=MULTILAYER;
				   route1.ex=tx2; route1.ey=ty2; route1.elayer=MULTILAYER;
				   if(route1.sx>route1.ex)
				     {    tempx=route1.sx; tempy=route1.sy;
					  route1.sx=route1.ex; route1.sy=route1.ey;
					  route1.ex=tempx;  route1.ey=tempy;
				     }
				   if(Route_Z_D(&route1,2)!=0)
				     { k=1;
				       goto SZEND;
				     }
				}
			}

	       }
	}

SZEND:

   CloseMouseCursor();
   DrawAnyLine(route->sx,route->sy,route->ex,route->ey,Highlighted_Color,1,&dc);
   OpenMouseCursor();
   if((l==0)&&(t1!=0)&&(m==0)&&(t2!=0)&&(k!=0))
     {
       if(try1!=0)
	  {    temp=templinebuf1;
	       for(j=0;j<t1;j++)
		 {  memcpy(&gnline,temp,16);
		    temp+=16;
		    if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
		    pDoc->Save_EMMLine(&gnline);
		    CloseMouseCursor();
		    DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
		    OpenMouseCursor();
		 }
	  }
       if(try2!=0)
	  {    temp=templinebuf2;
	       for(j=0;j<t2;j++)
		 {  memcpy(&gnline,temp,16);
		    temp+=16;
		    if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
		    pDoc->Save_EMMLine(&gnline);
		    CloseMouseCursor();
		    DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
		    OpenMouseCursor();
		 }
	  }
       return 1;
    }
   else return 0;
}

int CDesignView::Route_C_D(struct Route_Rats * route)
{
   int i,j,n1,n2,n3,v1,v2,num;
   char *temp;
   char linebuf1[80];
   char linebuf2[80];
   char linebuf3[80];
   long int delta1[3]={0,-1,1}, delta2[3]={0,1,-1};
   struct Route_Rats route1,route2,route3;
   struct Junction routevia1, routevia2;
   unsigned tempx1,tempy1,tempx2,tempy2;
   unsigned tx1,ty1,tx2,ty2;
	struct Undo_Head head;
	char *undo_p;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

   routevia1.id=0x22;
   routevia1.Dx=routevia1.Dy=Route_TW;
   routevia1.d=1;
   routevia1.padtype=1;
   routevia1.attrib=PCB;
   routevia1.layer=MULTILAYER;
   routevia1.flag=0;
   strcpy((char*)routevia1.padname,Null_Str);
   routevia2.id=0x22;
   routevia2.Dx=routevia2.Dy=Route_TW;
   routevia2.d=1;
   routevia2.padtype=1;
   routevia2.attrib=PCB;
   routevia2.layer=MULTILAYER;
   routevia2.flag=0;
   strcpy((char*)routevia2.padname,Null_Str);

   DrawAnyLine(route->sx,route->sy,route->ex,route->ey,Highlighted_Color,1,&dc);
   /************************/
   /*   t1   x--- O        */
   /*        |   sx,sy     */
   /*        |             */
   /*        |             */
   /*        |       ex,ey */
   /*   t2   x-----------O */
   /************************/
   num=ROUTEDELTA/CL_Snap_Grid;
   tx1=route->sx;
   ty1=route->sy;
   tx2=route->ex;
   ty2=route->ey;
   for(;;)
   {
		if(abs(route->sy-route->ey)<=ROUTEDELTA)
	    { 
		   v1=1; n1=0;
			break;
	    }
		for(i=0;i<3;i++)
		{
			// t1 position 
			tempx1=tx1-CL_Snap_Grid*num;
			if((tempx1>=CL_Size_Maxx)||(tempx1<ROUTEDELTA)) goto CMODE2;
			tempy1=ty1+delta1[i]*ROUTEDELTA;
			// via1 at t1 
			routevia1.centerx=tempx1;
			routevia1.centery=tempy1;
			v1=Clearance_Route((struct Line*)&routevia1);
			if(v1!=0) continue;
			else break;
		}
		if(v1!=0)
		{
			num++;
			continue;
		}
		// route1 from START(sx,sy) to t1 
		route1.sx=tempx1;
		route1.sy=tempy1;
		route1.slayer=MULTILAYER;
		route1.ex=tx1;
		route1.ey=ty1;
		route1.elayer=route->slayer;
		n1=Route_Hor_Line_D(&route1,linebuf1);
		if(n1==0) break;
		for(i=0;i<3;i++)
		{
			// t2 position 
			tempx2=tempx1;
			tempy2=ty2+delta2[i]*ROUTEDELTA;
			// via2 at t2 
			routevia2.centerx=tempx2;
			routevia2.centery=tempy2;
			v2=Clearance_Route((struct Line*)&routevia2);
			if(v2!=0) continue;
			else break;
	     }
		if(v2!=0)
		{
			num++;
			continue;
		}
		// route3 from END(ex,ey) to t2 
		route3.sx=tempx2;
		route3.sy=tempy2;
		route3.slayer=MULTILAYER;
		route3.ex=tx2;
		route3.ey=ty2;
		route3.elayer=route->elayer;
		n3=Route_Hor_Line_D(&route3,linebuf3);
		if(n3==0) break;
		// route2 from t1 to t2 
		if(tempy2>tempy1)
	    {
			route2.sx=tempx1;
			route2.sy=tempy1;
			route2.slayer=MULTILAYER;
			route2.ex=tempx2;
			route2.ey=tempy2;
			route2.elayer=MULTILAYER;
	    }
		else
	    {
			route2.sx=tempx2;
			route2.sy=tempy2;
			route2.slayer=MULTILAYER;
			route2.ex=tempx1;
			route2.ey=tempy1;
			route2.elayer=MULTILAYER;
	    }
	   n2=Route_Ver_Line_D(&route2,linebuf2);
	   if(n2==0)
	   {
		   num++;
		   continue;
	   }
	   if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)) goto CEND;
    }

   /************************/
   /*   O--------------x   */
   /*  sx,sy        t1 |   */
   /*                  |   */
   /*                  |   */
   /*         ex,ey    |t2 */
   /*           O------x   */
   /************************/
CMODE2:
	num=ROUTEDELTA/CL_Snap_Grid;
	tx1=route->sx;
	ty1=route->sy;
	tx2=route->ex;
	ty2=route->ey;
	for(;;)
    {  	   
		if(abs(route->sy-route->ey)<=ROUTEDELTA)
	    {
			v1=1; n1=0;
			break;
	    }
		for(i=0;i<3;i++)
		{
			// t1 
			tempx1=tx2+CL_Snap_Grid*num;
			if((tempx1>=CL_Size_Maxx)||(tempx1<ROUTEDELTA)) goto CMODE3;
			tempy1=ty1+delta1[i]*ROUTEDELTA;
			// via1 at t1 
			routevia1.centerx=tempx1;
			routevia1.centery=tempy1;
			v1=Clearance_Route((struct Line*)&routevia1);
			if(v1!=0) continue;
			else break;
		}
		if(v1!=0) 
		{
			num++;
			continue;
		}
		// route1 from START to t1 
		route1.ex=tempx1;
		route1.ey=tempy1;
		route1.elayer=MULTILAYER;
		route1.sx=tx1;
		route1.sy=ty1;
		route1.slayer=route->slayer;
		n1=Route_Hor_Line_D(&route1,linebuf1);
		if(n1==0) break;
		for(i=0;i<3;i++)
		{
			// t2 
		   tempx2=tempx1;
		   tempy2=ty2+delta2[i]*ROUTEDELTA;
		   // via2 
		   routevia2.centerx=tempx2;
		   routevia2.centery=tempy2;
		   v2=Clearance_Route((struct Line*)&routevia2);
		   if(v2!=0) continue;
		   else break;
		}
		if(v2!=0)
		{
			num++;
			continue;
		}
		route3.ex=tempx2;
		route3.ey=tempy2;
		route3.elayer=MULTILAYER;
		route3.sx=tx2;
		route3.sy=ty2;
		route3.slayer=route->elayer;
		n3=Route_Hor_Line_D(&route3,linebuf3);
		if(n3==0) break;
		if(tempy2>tempy1)
		{
			route2.sx=tempx1;
			route2.sy=tempy1;
			route2.slayer=MULTILAYER;
			route2.ex=tempx2;
			route2.ey=tempy2;
			route2.elayer=MULTILAYER;
	    }
		else
	    {   
		   route2.sx=tempx2;
			route2.sy=tempy2;
			route2.slayer=MULTILAYER;
			route2.ex=tempx1;
			route2.ey=tempy1;
			route2.elayer=MULTILAYER;
	    }
		n2=Route_Ver_Line_D(&route2,linebuf2);
		if(n2==0)
		{
			num++;
			continue;
		}
	   if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)) goto CEND;
    }
   /****************************/
   /*   O ex,ey                */
   /*   |                sx,sy */
   /*   |                  O   */
   /*   |                  |   */
   /*   | t2            t1 |   */
   /*   x ---------------- x   */
   /****************************/
CMODE3:
	if(route->sy>route->ey)
	{
		tempx1=route->sx; tempy1=route->sy;
		route->sx=route->ex; route->sy=route->ey;
		route->ex=tempx1;  route->ey=tempy1;
		tempx1=route->slayer;
		route->slayer=route->elayer;
		route->elayer=tempx1;
	}
	num=ROUTEDELTA/CL_Snap_Grid;
	tx1=route->sx;
	ty1=route->sy;
	tx2=route->ex;
	ty2=route->ey;
	for(;;)
    {
		if(abs(route->sx-route->ex)<=ROUTEDELTA)
	    {
			v1=1; n1=0;
			break;
	    }
		for(i=0;i<3;i++)
	    {
			// t1 
			tempx1=tx1+delta1[i]*ROUTEDELTA;
			tempy1=ty1-CL_Snap_Grid*num;
			if((tempy1>=CL_Size_Maxy)||(tempy1<ROUTEDELTA)) goto CMODE4;
			routevia1.centerx=tempx1;
			routevia1.centery=tempy1;
			v1=Clearance_Route((struct Line*)&routevia1);
			if(v1!=0) continue;
			else break;
		}
		if(v1!=0)
		{
			num++;
			continue;
		}
		route1.sx=tempx1;
		route1.sy=tempy1;
		route1.slayer=MULTILAYER;
		route1.ex=tx1;
		route1.ey=ty1;
		route1.elayer=route->slayer;
		n1=Route_Ver_Line_D(&route1,linebuf1);
		if(n1==0) break;
		for(i=0;i<3;i++)
		{
			tempx2=tx2+delta2[i]*ROUTEDELTA;
			tempy2=tempy1;
			routevia2.centerx=tempx2;
			routevia2.centery=tempy2;
			v2=Clearance_Route((struct Line*)&routevia2);
			if(v2!=0) continue;
			else break;
		}
		if(v2!=0) 
		{
			num++;
			continue;
		}
		route3.sx=tempx2;
		route3.sy=tempy2;
		route3.slayer=MULTILAYER;
		route3.ex=tx2;
		route3.ey=ty2;
		route3.elayer=route->elayer;
		n3=Route_Ver_Line_D(&route3,linebuf3);
		if(n3==0) break;
		if(tempx2>tempx1)
	    {
			route2.sx=tempx1;
			route2.sy=tempy1;
			route2.slayer=MULTILAYER;
			route2.ex=tempx2;
			route2.ey=tempy2;
			route2.elayer=MULTILAYER;
	    }
		else
	    {
			route2.sx=tempx2;
			route2.sy=tempy2;
			route2.slayer=MULTILAYER;
			route2.ex=tempx1;
			route2.ey=tempy1;
			route2.elayer=MULTILAYER;
	    }
		n2=Route_Hor_Line_D(&route2,linebuf2);
		if(n2==0)
		{
			num++;
			continue;
		}
		if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)) goto CEND;
    }
   /****************************/
   /*   x----------------- x   */
   /*   |                  |   */
   /*   |                  |   */
   /*   O                  |   */
   /*  ex,ey               |   */
   /*               sx,sy  O   */
   /****************************/
CMODE4:
	num=ROUTEDELTA/CL_Snap_Grid;
	tx1=route->sx;
	ty1=route->sy;
	tx2=route->ex;
	ty2=route->ey;
	for(;;)
    {
		if(abs(route->sx-route->ex)<=ROUTEDELTA)
	    {
			v1=1; n1=0;
			break;
	    }
		for(i=0;i<3;i++)
	    {
			tempx1=tx1+delta1[i]*ROUTEDELTA;
			tempy1=ty2+CL_Snap_Grid*num;
			if((tempy1>=CL_Size_Maxy)||(tempy1<ROUTEDELTA)) goto CEND;
			routevia1.centerx=tempx1;
			routevia1.centery=tempy1;
			v1=Clearance_Route((struct Line*)&routevia1);
			if(v1!=0) continue;
			else break;
		}
		if(v1!=0) 
		{
			num++;
			continue;
		}
		route1.ex=tempx1;
		route1.ey=tempy1;
		route1.elayer=MULTILAYER;
		route1.sx=tx1;
		route1.sy=ty1;
		route1.slayer=route->slayer;
		n1=Route_Ver_Line_D(&route1,linebuf1);
		if(n1==0) break;
		for(i=0;i<3;i++)
		{
			tempx2=tx2+delta2[i]*ROUTEDELTA;
			tempy2=tempy1;
			routevia2.centerx=tempx2;
			routevia2.centery=tempy2;
			v2=Clearance_Route((struct Line*)&routevia2);
			if(v2!=0) continue;
			else break;
		}
		if(v2!=0)
		{
			num++;
			continue;
		}
		route3.ex=tempx2;
		route3.ey=tempy2;
		route3.elayer=MULTILAYER;
		route3.sx=tx2;
		route3.sy=ty2;
		route3.slayer=route->elayer;
		n3=Route_Ver_Line_D(&route3,linebuf3);
		if(n3==0) break;
		if(tempx2>tempx1)
	    {
			route2.sx=tempx1;
			route2.sy=tempy1;
			route2.slayer=MULTILAYER;
			route2.ex=tempx2;
			route2.ey=tempy2;
			route2.elayer=MULTILAYER;
	    }
		else
	    {
			route2.sx=tempx2;
			route2.sy=tempy2;
			route2.slayer=MULTILAYER;
			route2.ex=tempx1;
			route2.ey=tempy1;
			route2.elayer=MULTILAYER;
	    }
		n2=Route_Hor_Line_D(&route2,linebuf2);
		if(n2==0)
		{
			num++;
			continue;
		}
		if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)) goto CEND;
    }
CEND:
	CloseMouseCursor();
	DrawAnyLine(route->sx,route->sy,route->ex,route->ey,Highlighted_Color,1,&dc);
	DrawPCBHighlight(0);
	OpenMouseCursor();
	if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0))
    {
		temp=linebuf1;
		for(j=0;j<n1;j++)
		{
			memcpy(&gnline,temp,16);
			temp+=16;
			if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
			pDoc->Save_EMMLine(&gnline);
			CloseMouseCursor();
			DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
			OpenMouseCursor();
		}
		temp=linebuf2;
		for(j=0;j<n2;j++)
		{
			memcpy(&gnline,temp,16);
			temp+=16;
			if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
			pDoc->Save_EMMLine(&gnline);
			CloseMouseCursor();
			DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
			OpenMouseCursor();
		}
		temp=linebuf3;
		for(j=0;j<n3;j++)
		{
			memcpy(&gnline,temp,16);
			temp+=16;
			if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
			pDoc->Save_EMMLine(&gnline);
			CloseMouseCursor();
			DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
			OpenMouseCursor();
		}
		return 1;
    }
	else return 0;
}


int CDesignView::Super_C_D(struct Route_Rats * route)
{
   int i,ii,j,l,m,n1,n2,n3,v1,v2,t1,t2,num, flag1,flag2;
   char *temp;
   char linebuf1[80];
   char linebuf2[80];
   char linebuf3[80];
   char templinebuf1[80];
   char templinebuf2[80];
   long int try1,try2,delta1[3]={0,-1,1}, delta2[3]={0,1,-1};
   struct Route_Rats route1,route2,route3;
   struct Junction routevia1, routevia2;
   unsigned tempx1,tempy1,tempx2,tempy2;
   unsigned tx1,ty1,tx2,ty2;
   struct Route_Rats temproute1, temproute2;
   struct Junction tempvia1,tempvia2;
	struct Undo_Head head;
	char *undo_p;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

   routevia1.id=0x22;
   routevia1.Dx=routevia1.Dy=Route_TW;
   routevia1.d=1;
   routevia1.padtype=1;
   routevia1.attrib=PCB;
   routevia1.layer=MULTILAYER;
   routevia1.flag=0;
   strcpy((char*)routevia1.padname,Null_Str);
   routevia2.id=0x22;
   routevia2.Dx=routevia2.Dy=Route_TW;
   routevia2.d=1;
   routevia2.padtype=1;
   routevia2.attrib=PCB;
   routevia2.layer=MULTILAYER;
   routevia2.flag=0;
   strcpy((char*)routevia2.padname,Null_Str);

   tempvia1.id=0x22;
   tempvia1.Dx=tempvia1.Dy=Route_TW;
   tempvia1.d=1;
   tempvia1.padtype=1;
   tempvia1.attrib=PCB;
   tempvia1.layer=MULTILAYER;
   tempvia1.flag=0;
   strcpy((char*)tempvia1.padname,Null_Str);
   tempvia2.id=0x22;
   tempvia2.Dx=tempvia2.Dy=Route_TW;
   tempvia2.d=1;
   tempvia2.padtype=1;
   tempvia2.attrib=PCB;
   tempvia2.layer=MULTILAYER;
   tempvia2.flag=0;
   strcpy((char*)tempvia2.padname,Null_Str);

   DrawAnyLine(route->sx,route->sy,route->ex,route->ey,Highlighted_Color,1,&dc);
//SCMODE1:
	if(route->sy >= route->ey)
	{
		//MessageBox("1");
	   /************************/
	   /*   t1   x--- O        */
	   /*        |   sx,sy     */
	   /*        |             */
	   /*        |             */
	   /*        |       ex,ey */
	   /*   t2   x-----------O */
	   /************************/
		num=ROUTEDELTA/CL_Snap_Grid;
		tx1=route->sx;
		ty1=route->sy;
		tx2=route->ex;
		ty2=route->ey;
		try1=try2=1l;
		flag1=flag2=0;
		for(;;)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						return -1;
					}
				}
				break;
		}

		for(i=0;i<3;i++)
		{
			/* t1 position */
			tempx1=tx1-CL_Snap_Grid*num;
			if((tempx1>=CL_Size_Maxx)||(tempx1<ROUTEDELTA)) goto SCMODE2;
			tempy1=ty1+delta1[i]*ROUTEDELTA;
			/* via1 at t1 */
			routevia1.centerx=tempx1;
			routevia1.centery=tempy1;
			v1=Clearance_Route((struct Line*)&routevia1);
			if(v1!=0) continue;
			else break;
		}
		if(v1!=0) 
		{
			num++;
			continue;
		}
		/* route1 from START(sx,sy) to t1 */
		route1.sx=tempx1;
		route1.sy=tempy1;
		route1.slayer=MULTILAYER;
		route1.ex=tx1;
		route1.ey=ty1;
		route1.elayer=route->slayer;
		n1=Route_Hor_Line_D(&route1,linebuf1);
		if(n1==0)
		{
			for(;;)
			{
				if(flag1==0) try1++;
				else try1--;
				ty1=(long)(route->sy)-try1*CL_Snap_Grid;
				if((ty1>=CL_Size_Maxy)||(ty1<50))
				{
					if(flag1==1) goto SCMODE2;
					else 
					{
						try1=-1l;
						flag1=1;
						continue;
					}
				}
				tempvia1.centerx=tx1;
				tempvia1.centery=ty1;
				l=Clearance_Route((struct Line*)&tempvia1);
				if(l!=0) continue;
				if(flag1==0)
				{
					temproute1.sx=tx1;
					  temproute1.sy=ty1;
					  temproute1.slayer=MULTILAYER;
					  temproute1.ex=route->sx;
					  temproute1.ey=route->sy;
					  temproute1.elayer=route->slayer;
				}
				else 
				{
					temproute1.sx=route->sx;
				  temproute1.sy=route->sy;
				  temproute1.slayer=route->slayer;
				  temproute1.ex=tx1;
				  temproute1.ey=ty1;
				  temproute1.elayer=MULTILAYER;
				}
				t1=Route_Ver_Line_D(&temproute1,templinebuf1);
				if(t1==0)
				{
					if(flag1==1) goto SCMODE2;
					else { try1=-1l;
					       flag1=1;
					       continue;
					    }
				}
				else
				{
					num=ROUTEDELTA/CL_Snap_Grid;
					break;
				}
			}
			continue;
		}
		for(i=0;i<3;i++)
	    { 
			/* t2 position */
			tempx2=tempx1;
			tempy2=ty2+delta2[i]*ROUTEDELTA;
			/* via2 at t2 */
			routevia2.centerx=tempx2;
			routevia2.centery=tempy2;
			v2=Clearance_Route((struct Line*)&routevia2);
			if(v2!=0) continue;
			else break;
		}
		if(v2!=0)
		{
			num++;
			continue;
		}
		/* route3 from END(ex,ey) to t2 */
		route3.sx=tempx2;
		route3.sy=tempy2;
		route3.slayer=MULTILAYER;
		route3.ex=tx2;
		route3.ey=ty2;
		route3.elayer=route->elayer;
		n3=Route_Hor_Line_D(&route3,linebuf3);
		if(n3==0)
		{
			for(;;)
			{
				if(flag2==0) try2++;
				else try2--;
				ty2=(long)(route->ey)+try2*CL_Snap_Grid;
				if((ty2>=CL_Size_Maxy)||(ty2<50))
				{
					if(flag2==1) goto SCMODE2;
					else
					{
						try2=-1l;
						flag2=1;
						continue;
					}
				}
				tempvia2.centerx=tx2;
				tempvia2.centery=ty2;
				m=Clearance_Route((struct Line*)&tempvia2);
				if(m!=0) continue;
				if(flag2==0) 
				{
					temproute2.sx=route->ex;
					temproute2.sy=route->ey;
					temproute2.slayer=route->elayer;
					temproute2.ex=tx2;
					temproute2.ey=ty2;
					temproute2.elayer=MULTILAYER;
				}
				else 
				{
					temproute2.sx=tx2;
				   temproute2.sy=ty2;
				   temproute2.slayer=MULTILAYER;
				   temproute2.ex=route->ex;
				   temproute2.ey=route->ey;
				   temproute2.elayer=route->elayer;
				}
				t2=Route_Ver_Line_D(&temproute2,templinebuf2);
				if(t2==0)
				{
					if(flag2==1) goto SCMODE2;
					else 
					{
						try2=-1l;
						flag2=1;
						continue;
					}
				}
				else break;
			}
			continue;
		}
		if(tempy2>tempy1)
		{
			route2.sx=tempx1;
			route2.sy=tempy1;
			route2.slayer=MULTILAYER;
			route2.ex=tempx2;
			route2.ey=tempy2;
			route2.elayer=MULTILAYER;
	    }
		else
	    {
			route2.sx=tempx2;
			route2.sy=tempy2;
			route2.slayer=MULTILAYER;
			route2.ex=tempx1;
			route2.ey=tempy1;
			route2.elayer=MULTILAYER;
		}
		n2=Route_Ver_Line_D(&route2,linebuf2);
		if(n2==0)
		{
			num++;
			continue;
		}
		if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)\
			&&(((try1!=1l)&&(l==0)&&(t1!=0))||(try1==1l))\
			&&(((try2!=1l)&&(m==0)&&(t2!=0))||(try2==1l))) goto SCEND;
		}
	}
	else
	{
		//MessageBox("2");
	   /************************/
	   /*   t1   x----------0  */
	   /*        |       ex,ey */
	   /*        |             */
	   /*        |             */
	   /*        |    sx,sy    */
	   /*   t2   x-------0     */
	   /************************/
num=ROUTEDELTA/CL_Snap_Grid;
		tx1=route->sx;
		ty1=route->sy;
		tx2=route->ex;
		ty2=route->ey;
		try1=try2=1l;
		flag1=flag2=0;
		for(;;)
		{
			// interupt route
			for(;;)
			{
				int ch2 = MRetKey();
				if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
				{
					ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
					if( ii == IDYES)
					{
						return -1;
					}
				}
				break;
		}

		for(i=0;i<3;i++)
		{
			/* t1 position */
			tempx1=tx1-CL_Snap_Grid*num;
			if((tempx1>=CL_Size_Maxx)||(tempx1<ROUTEDELTA)) goto SCMODE2;
			tempy1=ty1+delta1[i]*ROUTEDELTA;
			/* via1 at t1 */
			routevia1.centerx=tempx1;
			routevia1.centery=tempy1;
			v1=Clearance_Route((struct Line*)&routevia1);
			if(v1!=0) continue;
			else break;
		}
		if(v1!=0) 
		{
			num++;
			continue;
		}
		/* route1 from START(sx,sy) to t1 */
		route1.sx=tempx1;
		route1.sy=tempy1;
		route1.slayer=MULTILAYER;
		route1.ex=tx1;
		route1.ey=ty1;
		route1.elayer=route->slayer;
		n1=Route_Hor_Line_D(&route1,linebuf1);
		if(n1==0)
		{
			for(;;)
			{
				if(flag1==0) try1++;
				else try1--;
				ty1=(long)(route->sy)+try1*CL_Snap_Grid;
				if((ty1>=CL_Size_Maxy)||(ty1<50))
				{
					if(flag1==1) goto SCMODE2;
					else 
					{
						try1=-1l;
						flag1=1;
						continue;
					}
				}
				tempvia1.centerx=tx1;
				tempvia1.centery=ty1;
				l=Clearance_Route((struct Line*)&tempvia1);
				if(l!=0) continue;
				if(flag1==0)
				{
					temproute1.ex=tx1;
					  temproute1.ey=ty1;
					  temproute1.elayer=MULTILAYER;
					  temproute1.sx=route->sx;
					  temproute1.sy=route->sy;
					  temproute1.slayer=route->slayer;
				}
				else 
				{
					temproute1.ex=route->sx;
				  temproute1.ey=route->sy;
				  temproute1.elayer=route->slayer;
				  temproute1.sx=tx1;
				  temproute1.sy=ty1;
				  temproute1.slayer=MULTILAYER;
				}
				t1=Route_Ver_Line_D(&temproute1,templinebuf1);
				if(t1==0)
				{
					if(flag1==1) goto SCMODE2;
					else { try1=-1l;
					       flag1=1;
					       continue;
					    }
				}
				else
				{
					num=ROUTEDELTA/CL_Snap_Grid;
					break;
				}
			}
			continue;
		}
		for(i=0;i<3;i++)
	    { 
			/* t2 position */
			tempx2=tempx1;
			tempy2=ty2+delta2[i]*ROUTEDELTA;
			/* via2 at t2 */
			routevia2.centerx=tempx2;
			routevia2.centery=tempy2;
			v2=Clearance_Route((struct Line*)&routevia2);
			if(v2!=0) continue;
			else break;
		}
		if(v2!=0)
		{
			num++;
			continue;
		}
		/* route3 from END(ex,ey) to t2 */
		route3.sx=tempx2;
		route3.sy=tempy2;
		route3.slayer=MULTILAYER;
		route3.ex=tx2;
		route3.ey=ty2;
		route3.elayer=route->elayer;
		n3=Route_Hor_Line_D(&route3,linebuf3);
		if(n3==0)
		{
			for(;;)
			{
				if(flag2==0) try2++;
				else try2--;
				ty2=(long)(route->ey)-try2*CL_Snap_Grid;
				if((ty2>=CL_Size_Maxy)||(ty2<50))
				{
					if(flag2==1) goto SCMODE2;
					else
					{
						try2=-1l;
						flag2=1;
						continue;
					}
				}
				tempvia2.centerx=tx2;
				tempvia2.centery=ty2;
				m=Clearance_Route((struct Line*)&tempvia2);
				if(m!=0) continue;
				if(flag2==0) 
				{
					temproute2.ex=route->ex;
					temproute2.ey=route->ey;
					temproute2.elayer=route->elayer;
					temproute2.sx=tx2;
					temproute2.sy=ty2;
					temproute2.slayer=MULTILAYER;
				}
				else 
				{
					temproute2.ex=tx2;
				   temproute2.ey=ty2;
				   temproute2.elayer=MULTILAYER;
				   temproute2.sx=route->ex;
				   temproute2.sy=route->ey;
				   temproute2.slayer=route->elayer;
				}
				t2=Route_Ver_Line_D(&temproute2,templinebuf2);
				if(t2==0)
				{
					if(flag2==1) goto SCMODE2;
					else 
					{
						try2=-1l;
						flag2=1;
						continue;
					}
				}
				else break;
			}
			continue;
		}
		if(tempy2>tempy1)
		{
			route2.sx=tempx1;
			route2.sy=tempy1;
			route2.slayer=MULTILAYER;
			route2.ex=tempx2;
			route2.ey=tempy2;
			route2.elayer=MULTILAYER;
	    }
		else
	    {
			route2.sx=tempx2;
			route2.sy=tempy2;
			route2.slayer=MULTILAYER;
			route2.ex=tempx1;
			route2.ey=tempy1;
			route2.elayer=MULTILAYER;
		}
		n2=Route_Ver_Line_D(&route2,linebuf2);
		if(n2==0)
		{
			num++;
			continue;
		}
		if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)\
			&&(((try1!=1l)&&(l==0)&&(t1!=0))||(try1==1l))\
			&&(((try2!=1l)&&(m==0)&&(t2!=0))||(try2==1l))) goto SCEND;
		}		
	}

SCMODE2:
	if(route->sy >= route->ey)
	{
		//MessageBox("3");
	   /************************/
	   /*   O--------------x   */
	   /*  sx,sy        t1 |   */
	   /*                  |   */
	   /*                  |   */
	   /*         ex,ey    |t2 */
	   /*           O------x   */
	   /************************/
	   num=ROUTEDELTA/CL_Snap_Grid;
	   tx1=route->sx;
	   ty1=route->sy;
	   tx2=route->ex;
	   ty2=route->ey;
	   try1=try2=1l;
	   flag1=flag2=0;
	   for(;;)
		{
				// interupt route
				for(;;)
				{
					int ch2 = MRetKey();
					if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
					{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
						if( ii == IDYES)
						{
							return -1;
						}
					}
					break;
				}

		  for(i=0;i<3;i++)
			 {     /* t1 */
			   tempx1=tx2+CL_Snap_Grid*num;
			   if((tempx1>=CL_Size_Maxx)||(tempx1<ROUTEDELTA)) goto SCMODE3;
			   tempy1=ty1+delta1[i]*ROUTEDELTA;
			   /* via1 at t1 */
			   routevia1.centerx=tempx1;
			   routevia1.centery=tempy1;
			   v1=Clearance_Route((struct Line*)&routevia1);
			   if(v1!=0) continue;
			   else break;
			  }
		   if(v1!=0) {  num++;
				continue;
				 }
		   /* route1 from START to t1 */
		   route1.ex=tempx1;
		   route1.ey=tempy1;
		   route1.elayer=MULTILAYER;
		   route1.sx=tx1;
		   route1.sy=ty1;
		   route1.slayer=route->slayer;
		   n1=Route_Hor_Line_D(&route1,linebuf1);
		   if(n1==0) { for(;;)
				{ if(flag1==0) try1++;
				  else try1--;
				  ty1=(long)(route->sy)-try1*CL_Snap_Grid;
				  if((ty1>=CL_Size_Maxy)||(ty1<50))
					  { if(flag1==1) goto SCMODE3;
					else { try1=-1l;
						   flag1=1;
						   continue;
						 }
					  }
				  tempvia1.centerx=tx1;
				  tempvia1.centery=ty1;
				  l=Clearance_Route((struct Line*)&tempvia1);
				  if(l!=0) continue;
				  if(flag1==0) {  temproute1.sx=tx1;
						  temproute1.sy=ty1;
						  temproute1.slayer=MULTILAYER;
						  temproute1.ex=route->sx;
						  temproute1.ey=route->sy;
						  temproute1.elayer=route->slayer;
						   }
				  else {  temproute1.sx=route->sx;
					  temproute1.sy=route->sy;
					  temproute1.slayer=route->slayer;
					  temproute1.ex=tx1;
					  temproute1.ey=ty1;
					  temproute1.elayer=MULTILAYER;
					   }
				  t1=Route_Ver_Line_D(&temproute1,templinebuf1);
				  if(t1==0) {   if(flag1==1) goto SCMODE3;
						else { try1=-1l;
							   flag1=1;
							   continue;
							}
						}
				  else { num=ROUTEDELTA/CL_Snap_Grid;
					 break;
					   }
				}
				   continue;
				 }
		   for(i=0;i<3;i++)
			 {     /* t2 */
			   tempx2=tempx1;
			   tempy2=ty2+delta2[i]*ROUTEDELTA;
			   /* via2 */
			   routevia2.centerx=tempx2;
			   routevia2.centery=tempy2;
			   v2=Clearance_Route((struct Line*)&routevia2);
			   if(v2!=0) continue;
			   else break;
			 }
		   if(v2!=0){  num++;
				   continue;
				}
		   route3.ex=tempx2;
		   route3.ey=tempy2;
		   route3.elayer=MULTILAYER;
		   route3.sx=tx2;
		   route3.sy=ty2;
		   route3.slayer=route->elayer;
		   n3=Route_Hor_Line_D(&route3,linebuf3);
		   if(n3==0) { for(;;)
				{  if(flag2==0) try2++;
				   else try2--;
				   ty2=(long)(route->ey)+try2*CL_Snap_Grid;
				   if((ty2>=CL_Size_Maxy)||(ty2<50))
					  { if(flag2==1) goto SCMODE3;
					else { try2=-1l;
						   flag2=1;
						   continue;
						 }
					  }
				   tempvia2.centerx=tx2;
				   tempvia2.centery=ty2;
				   m=Clearance_Route((struct Line*)&tempvia2);
				   if(m!=0) continue;
				   if(flag2==0) {  temproute2.sx=route->ex;
						   temproute2.sy=route->ey;
						   temproute2.slayer=route->elayer;
						   temproute2.ex=tx2;
						   temproute2.ey=ty2;
						   temproute2.elayer=MULTILAYER;
						}
				   else {  temproute2.sx=tx2;
					   temproute2.sy=ty2;
					   temproute2.slayer=MULTILAYER;
					   temproute2.ex=route->ex;
					   temproute2.ey=route->ey;
					   temproute2.elayer=route->elayer;
					}
				   t2=Route_Ver_Line_D(&temproute2,templinebuf2);
				   if(t2==0) {  if(flag2==1) goto SCMODE3;
						else { try2=-1l;
							   flag2=1;
							   continue;
							 }
						 }
				   else break;
				}
				   continue;
				 }
		   if(tempy2>tempy1)
			{   route2.sx=tempx1;
			route2.sy=tempy1;
			route2.slayer=MULTILAYER;
			route2.ex=tempx2;
			route2.ey=tempy2;
			route2.elayer=MULTILAYER;
			}
		   else
			{   route2.sx=tempx2;
			route2.sy=tempy2;
			route2.slayer=MULTILAYER;
			route2.ex=tempx1;
			route2.ey=tempy1;
			route2.elayer=MULTILAYER;
			}
		   n2=Route_Ver_Line_D(&route2,linebuf2);
		   if(n2==0){  num++;
				   continue;
				}
		   if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)\
			 &&(((try1!=1l)&&(l==0)&&(t1!=0))||(try1==1l))\
			 &&(((try2!=1l)&&(m==0)&&(t2!=0))||(try2==1l))) goto SCEND;
		}
	}
	else
	{
		//MessageBox("4");
	   /************************/
	   /*           0------x   */
	   /*         ex,ey t1 |   */
	   /*                  |   */
	   /*                  |   */
	   /*   sx,sy          |t2 */
	   /*    0-------------x   */
	   /************************/
	   num=ROUTEDELTA/CL_Snap_Grid;
	   tx1=route->sx;
	   ty1=route->sy;
	   tx2=route->ex;
	   ty2=route->ey;
	   try1=try2=1l;
	   flag1=flag2=0;
	   for(;;)
		{
				// interupt route
				for(;;)
				{
					int ch2 = MRetKey();
					if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
					{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
						if( ii == IDYES)
						{
							return -1;
						}
					}
					break;
				}

		  for(i=0;i<3;i++)
			 {     /* t1 */
			   tempx1=tx2+CL_Snap_Grid*num;
			   if((tempx1>=CL_Size_Maxx)||(tempx1<ROUTEDELTA)) goto SCMODE3;
			   tempy1=ty1+delta1[i]*ROUTEDELTA;
			   /* via1 at t1 */
			   routevia1.centerx=tempx1;
			   routevia1.centery=tempy1;
			   v1=Clearance_Route((struct Line*)&routevia1);
			   if(v1!=0) continue;
			   else break;
			  }
		   if(v1!=0) {  num++;
				continue;
				 }
		   /* route1 from START to t1 */
		   route1.ex=tempx1;
		   route1.ey=tempy1;
		   route1.elayer=MULTILAYER;
		   route1.sx=tx1;
		   route1.sy=ty1;
		   route1.slayer=route->slayer;
		   n1=Route_Hor_Line_D(&route1,linebuf1);
		   if(n1==0) { for(;;)
				{ if(flag1==0) try1++;
				  else try1--;
				  ty1=(long)(route->sy)+try1*CL_Snap_Grid;
				  if((ty1>=CL_Size_Maxy)||(ty1<50))
					  { if(flag1==1) goto SCMODE3;
					else { try1=-1l;
						   flag1=1;
						   continue;
						 }
					  }
				  tempvia1.centerx=tx1;
				  tempvia1.centery=ty1;
				  l=Clearance_Route((struct Line*)&tempvia1);
				  if(l!=0) continue;
				  if(flag1==0) {  temproute1.ex=tx1;
						  temproute1.ey=ty1;
						  temproute1.elayer=MULTILAYER;
						  temproute1.sx=route->sx;
						  temproute1.sy=route->sy;
						  temproute1.slayer=route->slayer;
						   }
				  else {  temproute1.ex=route->sx;
					  temproute1.ey=route->sy;
					  temproute1.elayer=route->slayer;
					  temproute1.sx=tx1;
					  temproute1.sy=ty1;
					  temproute1.slayer=MULTILAYER;
					   }
				  t1=Route_Ver_Line_D(&temproute1,templinebuf1);
				  if(t1==0) {   if(flag1==1) goto SCMODE3;
						else { try1=-1l;
							   flag1=1;
							   continue;
							}
						}
				  else { num=ROUTEDELTA/CL_Snap_Grid;
					 break;
					   }
				}
				   continue;
				 }
		   for(i=0;i<3;i++)
			 {     /* t2 */
			   tempx2=tempx1;
			   tempy2=ty2+delta2[i]*ROUTEDELTA;
			   /* via2 */
			   routevia2.centerx=tempx2;
			   routevia2.centery=tempy2;
			   v2=Clearance_Route((struct Line*)&routevia2);
			   if(v2!=0) continue;
			   else break;
			 }
		   if(v2!=0){  num++;
				   continue;
				}
		   route3.ex=tempx2;
		   route3.ey=tempy2;
		   route3.elayer=MULTILAYER;
		   route3.sx=tx2;
		   route3.sy=ty2;
		   route3.slayer=route->elayer;
		   n3=Route_Hor_Line_D(&route3,linebuf3);
		   if(n3==0) { for(;;)
				{  if(flag2==0) try2++;
				   else try2--;
				   ty2=(long)(route->ey)-try2*CL_Snap_Grid;
				   if((ty2>=CL_Size_Maxy)||(ty2<50))
					  { if(flag2==1) goto SCMODE3;
					else { try2=-1l;
						   flag2=1;
						   continue;
						 }
					  }
				   tempvia2.centerx=tx2;
				   tempvia2.centery=ty2;
				   m=Clearance_Route((struct Line*)&tempvia2);
				   if(m!=0) continue;
				   if(flag2==0) {  temproute2.ex=route->ex;
						   temproute2.ey=route->ey;
						   temproute2.elayer=route->elayer;
						   temproute2.sx=tx2;
						   temproute2.sy=ty2;
						   temproute2.slayer=MULTILAYER;
						}
				   else {  temproute2.ex=tx2;
					   temproute2.ey=ty2;
					   temproute2.elayer=MULTILAYER;
					   temproute2.sx=route->ex;
					   temproute2.sy=route->ey;
					   temproute2.slayer=route->elayer;
					}
				   t2=Route_Ver_Line_D(&temproute2,templinebuf2);
				   if(t2==0) {  if(flag2==1) goto SCMODE3;
						else { try2=-1l;
							   flag2=1;
							   continue;
							 }
						 }
				   else break;
				}
				   continue;
				 }
		   if(tempy2>tempy1)
			{   route2.sx=tempx1;
			route2.sy=tempy1;
			route2.slayer=MULTILAYER;
			route2.ex=tempx2;
			route2.ey=tempy2;
			route2.elayer=MULTILAYER;
			}
		   else
			{   route2.sx=tempx2;
			route2.sy=tempy2;
			route2.slayer=MULTILAYER;
			route2.ex=tempx1;
			route2.ey=tempy1;
			route2.elayer=MULTILAYER;
			}
		   n2=Route_Ver_Line_D(&route2,linebuf2);
		   if(n2==0){  num++;
				   continue;
				}
		   if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)\
			 &&(((try1!=1l)&&(l==0)&&(t1!=0))||(try1==1l))\
			 &&(((try2!=1l)&&(m==0)&&(t2!=0))||(try2==1l))) goto SCEND;
		}
	}
SCMODE3:
   if(route->sy>route->ey) { tempx1=route->sx; tempy1=route->sy;
			     route->sx=route->ex; route->sy=route->ey;
			     route->ex=tempx1;  route->ey=tempy1;
			     tempx1=route->slayer;
			     route->slayer=route->elayer;
			     route->elayer=tempx1;
			   }
   if(route->sx >= route->ex)
   {
	   //MessageBox("5");
		/****************************/
	   /*   O ex,ey                */
	   /*   |                sx,sy */
	   /*   |                  O   */
	   /*   |                  |   */
	   /*   | t2            t1 |   */
	   /*   x ---------------- x   */
	   /****************************/
	   num=ROUTEDELTA/CL_Snap_Grid;
	   tx1=route->sx;
	   ty1=route->sy;
	   tx2=route->ex;
	   ty2=route->ey;
	   try1=try2=1l;
	   flag1=flag2=0;
	   for(;;)
		{
				// interupt route
				for(;;)
				{
					int ch2 = MRetKey();
					if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
					{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
						if( ii == IDYES)
						{
							return -1;
						}
					}
					break;
				}

		   for(i=0;i<3;i++)
			 {     /* t1 */
			   tempx1=tx1+delta1[i]*ROUTEDELTA;
			   tempy1=ty1-CL_Snap_Grid*num;
			   if((tempy1>=CL_Size_Maxy)||(tempy1<ROUTEDELTA)) goto SCMODE4;
			   routevia1.centerx=tempx1;
			   routevia1.centery=tempy1;
			   v1=Clearance_Route((struct Line*)&routevia1);
			   if(v1!=0) continue;
			   else break;
			 }
		   if(v1!=0) {  num++;
				continue;
				 }
		   route1.sx=tempx1;
		   route1.sy=tempy1;
		   route1.slayer=MULTILAYER;
		   route1.ex=tx1;
		   route1.ey=ty1;
		   route1.elayer=route->slayer;
		   n1=Route_Ver_Line_D(&route1,linebuf1);
		   if(n1==0) { for(;;)
				{ if(flag1==0) try1++;
				  else try1--;
				  tx1=(long)(route->sx)-try1*CL_Snap_Grid;
				  if((tx1>=CL_Size_Maxx)||(tx1<50))
					{   if(flag1==1) goto SCMODE4;
					else { try1=-1l;
						   flag1=1;
						   continue;
						 }
					}
				  tempvia1.centerx=tx1;
				  tempvia1.centery=ty1;
				  l=Clearance_Route((struct Line*)&tempvia1);
				  if(l!=0) continue;
				  if(flag1==0){   temproute1.sx=tx1;
						  temproute1.sy=ty1;
						  temproute1.slayer=MULTILAYER;
						  temproute1.ex=route->sx;
						  temproute1.ey=route->sy;
						  temproute1.elayer=route->slayer;
						  }
				  else {  temproute1.sx=route->sx;
					  temproute1.sy=route->sy;
					  temproute1.slayer=route->slayer;
					  temproute1.ex=tx1;
					  temproute1.ey=ty1;
					  temproute1.elayer=MULTILAYER;
					   }
				  t1=Route_Hor_Line_D(&temproute1,templinebuf1);
				  if(t1==0) {   if(flag1==1) goto SCMODE4;
						else { try1=-1l;
							   flag1=1;
							   continue;
							 }
						}
				  else {  num=ROUTEDELTA/CL_Snap_Grid;
					  break;
					   }
				}
				   continue;
				 }
		   for(i=0;i<3;i++)
			 {     tempx2=tx2+delta2[i]*ROUTEDELTA;
			   tempy2=tempy1;
			   routevia2.centerx=tempx2;
			   routevia2.centery=tempy2;
			   v2=Clearance_Route((struct Line*)&routevia2);
			   if(v2!=0) continue;
			   else break;
			 }
		   if(v2!=0) {  num++;
				continue;
				 }
		   route3.sx=tempx2;
		   route3.sy=tempy2;
		   route3.slayer=MULTILAYER;
		   route3.ex=tx2;
		   route3.ey=ty2;
		   route3.elayer=route->elayer;
		   n3=Route_Ver_Line_D(&route3,linebuf3);
		   if(n3==0)  { for(;;)
				{  if(flag2==0) try2++;
				   else try2--;
				   tx2=(long)(route->ex)+try2*CL_Snap_Grid;
				   if((tx2>=CL_Size_Maxx)||(tx2<50))
					{   if(flag2==1) goto SCMODE4;
					else { try2=-1l;
						   flag2=1;
						   continue;
						 }
					}
				   tempvia2.centerx=tx2;
				   tempvia2.centery=ty2;
				   m=Clearance_Route((struct Line*)&tempvia2);
				   if(m!=0) continue;
				   if(flag2==0) {  temproute2.sx=route->ex;
						   temproute2.sy=route->ey;
						   temproute2.slayer=route->elayer;
						   temproute2.ex=tx2;
						   temproute2.ey=ty2;
						   temproute2.elayer=MULTILAYER;
						}
				   else {  temproute2.sx=tx2;
					   temproute2.sy=ty2;
					   temproute2.slayer=MULTILAYER;
					   temproute2.ex=route->ex;
					   temproute2.ey=route->ey;
					   temproute2.elayer=route->elayer;
					}
				   t2=Route_Hor_Line_D(&temproute2,templinebuf2);
				   if(t2==0) {  if(flag2==1) goto SCMODE4;
						else { try2=-1l;
							   flag2=1;
							   continue;
							 }
						}
				   else break;
				}
				   continue;
				 }
		   if(tempx2>tempx1)
			{   route2.sx=tempx1;
			route2.sy=tempy1;
			route2.slayer=MULTILAYER;
			route2.ex=tempx2;
			route2.ey=tempy2;
			route2.elayer=MULTILAYER;
			}
		   else
			{   route2.sx=tempx2;
			route2.sy=tempy2;
			route2.slayer=MULTILAYER;
			route2.ex=tempx1;
			route2.ey=tempy1;
			route2.elayer=MULTILAYER;
			}
		   n2=Route_Hor_Line_D(&route2,linebuf2);
		   if(n2==0){  num++;
				   continue;
				}
		   if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)\
			 &&(((try1!=1l)&&(l==0)&&(t1!=0))||(try1==1l))\
			 &&(((try2!=1l)&&(m==0)&&(t2!=0))||(try2==1l))) goto SCEND;
		}
	}
   else
   {
	   //MessageBox("6");
		/****************************/
	   /*   O ex,ey                */
	   /*   |                sx,sy */
	   /*   |                  O   */
	   /*   |                  |   */
	   /*   | t2            t1 |   */
	   /*   x ---------------- x   */
	   /****************************/
	   num=ROUTEDELTA/CL_Snap_Grid;
	   tx1=route->sx;
	   ty1=route->sy;
	   tx2=route->ex;
	   ty2=route->ey;
	   try1=try2=1l;
	   flag1=flag2=0;
	   for(;;)
		{
				// interupt route
				for(;;)
				{
					int ch2 = MRetKey();
					if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
					{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
						if( ii == IDYES)
						{
							return -1;
						}
					}
					break;
				}

		   for(i=0;i<3;i++)
			 {     /* t1 */
			   tempx1=tx1+delta1[i]*ROUTEDELTA;
			   tempy1=ty1-CL_Snap_Grid*num;
			   if((tempy1>=CL_Size_Maxy)||(tempy1<ROUTEDELTA)) goto SCMODE4;
			   routevia1.centerx=tempx1;
			   routevia1.centery=tempy1;
			   v1=Clearance_Route((struct Line*)&routevia1);
			   if(v1!=0) continue;
			   else break;
			 }
		   if(v1!=0) {  num++;
				continue;
				 }
		   route1.sx=tempx1;
		   route1.sy=tempy1;
		   route1.slayer=MULTILAYER;
		   route1.ex=tx1;
		   route1.ey=ty1;
		   route1.elayer=route->slayer;
		   n1=Route_Ver_Line_D(&route1,linebuf1);
		   if(n1==0) { for(;;)
				{ if(flag1==0) try1++;
				  else try1--;
				  tx1=(long)(route->sx)+try1*CL_Snap_Grid;
				  if((tx1>=CL_Size_Maxx)||(tx1<50))
					{   if(flag1==1) goto SCMODE4;
					else { try1=-1l;
						   flag1=1;
						   continue;
						 }
					}
				  tempvia1.centerx=tx1;
				  tempvia1.centery=ty1;
				  l=Clearance_Route((struct Line*)&tempvia1);
				  if(l!=0) continue;
				  if(flag1==0){   temproute1.ex=tx1;
						  temproute1.ey=ty1;
						  temproute1.elayer=MULTILAYER;
						  temproute1.sx=route->sx;
						  temproute1.sy=route->sy;
						  temproute1.slayer=route->slayer;
						  }
				  else {  temproute1.ex=route->sx;
					  temproute1.ey=route->sy;
					  temproute1.elayer=route->slayer;
					  temproute1.sx=tx1;
					  temproute1.sy=ty1;
					  temproute1.slayer=MULTILAYER;
					   }
				  t1=Route_Hor_Line_D(&temproute1,templinebuf1);
				  if(t1==0) {   if(flag1==1) goto SCMODE4;
						else { try1=-1l;
							   flag1=1;
							   continue;
							 }
						}
				  else {  num=ROUTEDELTA/CL_Snap_Grid;
					  break;
					   }
				}
				   continue;
				 }
		   for(i=0;i<3;i++)
			 {     tempx2=tx2+delta2[i]*ROUTEDELTA;
			   tempy2=tempy1;
			   routevia2.centerx=tempx2;
			   routevia2.centery=tempy2;
			   v2=Clearance_Route((struct Line*)&routevia2);
			   if(v2!=0) continue;
			   else break;
			 }
		   if(v2!=0) {  num++;
				continue;
				 }
		   route3.sx=tempx2;
		   route3.sy=tempy2;
		   route3.slayer=MULTILAYER;
		   route3.ex=tx2;
		   route3.ey=ty2;
		   route3.elayer=route->elayer;
		   n3=Route_Ver_Line_D(&route3,linebuf3);
		   if(n3==0)  { for(;;)
				{  if(flag2==0) try2++;
				   else try2--;
				   tx2=(long)(route->ex)-try2*CL_Snap_Grid;
				   if((tx2>=CL_Size_Maxx)||(tx2<50))
					{   if(flag2==1) goto SCMODE4;
					else { try2=-1l;
						   flag2=1;
						   continue;
						 }
					}
				   tempvia2.centerx=tx2;
				   tempvia2.centery=ty2;
				   m=Clearance_Route((struct Line*)&tempvia2);
				   if(m!=0) continue;
				   if(flag2==0) {  temproute2.ex=route->ex;
						   temproute2.ey=route->ey;
						   temproute2.elayer=route->elayer;
						   temproute2.sx=tx2;
						   temproute2.sy=ty2;
						   temproute2.slayer=MULTILAYER;
						}
				   else {  temproute2.ex=tx2;
					   temproute2.ey=ty2;
					   temproute2.elayer=MULTILAYER;
					   temproute2.sx=route->ex;
					   temproute2.sy=route->ey;
					   temproute2.slayer=route->elayer;
					}
				   t2=Route_Hor_Line_D(&temproute2,templinebuf2);
				   if(t2==0) {  if(flag2==1) goto SCMODE4;
						else { try2=-1l;
							   flag2=1;
							   continue;
							 }
						}
				   else break;
				}
				   continue;
				 }
		   if(tempx2>tempx1)
			{   route2.sx=tempx1;
			route2.sy=tempy1;
			route2.slayer=MULTILAYER;
			route2.ex=tempx2;
			route2.ey=tempy2;
			route2.elayer=MULTILAYER;
			}
		   else
			{   route2.sx=tempx2;
			route2.sy=tempy2;
			route2.slayer=MULTILAYER;
			route2.ex=tempx1;
			route2.ey=tempy1;
			route2.elayer=MULTILAYER;
			}
		   n2=Route_Hor_Line_D(&route2,linebuf2);
		   if(n2==0){  num++;
				   continue;
				}
		   if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)\
			 &&(((try1!=1l)&&(l==0)&&(t1!=0))||(try1==1l))\
			 &&(((try2!=1l)&&(m==0)&&(t2!=0))||(try2==1l))) goto SCEND;
		}
	}

SCMODE4:
	if(route->sx >= route->ex)
	{
		//MessageBox("7");
		/****************************/
	   /*   x----------------- x   */
	   /*   |                  |   */
	   /*   |                  |   */
	   /*   O                  |   */
	   /*  ex,ey               |   */
	   /*               sx,sy  O   */
	   /****************************/
	   num=ROUTEDELTA/CL_Snap_Grid;
	   tx1=route->sx;
	   ty1=route->sy;
	   tx2=route->ex;
	   ty2=route->ey;
	   try1=try2=1l;
	   flag1=flag2=0;
	   for(;;)
		{
				// interupt route
				for(;;)
				{
					int ch2 = MRetKey();
					if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
					{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
						if( ii == IDYES)
						{
							return -1;
						}
					}
					break;
				}

		for(i=0;i<3;i++)
			 {     tempx1=tx1+delta1[i]*ROUTEDELTA;
			   tempy1=ty2+CL_Snap_Grid*num;
			   if((tempy1>=CL_Size_Maxy)||(tempy1<ROUTEDELTA)) goto SCEND;
			   routevia1.centerx=tempx1;
			   routevia1.centery=tempy1;
			   v1=Clearance_Route((struct Line*)&routevia1);
			   if(v1!=0) continue;
			   else break;
			 }
		   if(v1!=0) {  num++;
				continue;
				 }
		   route1.ex=tempx1;
		   route1.ey=tempy1;
		   route1.elayer=MULTILAYER;
		   route1.sx=tx1;
		   route1.sy=ty1;
		   route1.slayer=route->slayer;
		   n1=Route_Ver_Line_D(&route1,linebuf1);
		   if(n1==0) { for(;;)
				{ if(flag1==0) try1++;
				  else try1--;
				  tx1=(long)(route->sx)-try1*CL_Snap_Grid;
				  if((tx1>=CL_Size_Maxx)||(tx1<50))
					{   if(flag1==1) goto SCEND;
					else { try1=-1l;
						   flag1=1;
						   continue;
						 }
					}
				  tempvia1.centerx=tx1;
				  tempvia1.centery=ty1;
				  l=Clearance_Route((struct Line*)&tempvia1);
				  if(l!=0) continue;
				  if(flag1==0) {  temproute1.sx=tx1;
						  temproute1.sy=ty1;
						  temproute1.slayer=MULTILAYER;
						  temproute1.ex=route->sx;
						  temproute1.ey=route->sy;
						  temproute1.elayer=route->slayer;
						   }
				  else {  temproute1.sx=route->sx;
					  temproute1.sy=route->sy;
					  temproute1.slayer=route->slayer;
					  temproute1.ex=tx1;
					  temproute1.ey=ty1;
					  temproute1.elayer=MULTILAYER;
					}
				  t1=Route_Hor_Line_D(&temproute1,templinebuf1);
				  if(t1==0) {   if(flag1==1) goto SCEND;
						else { try1=-1l;
							   flag1=1;
							   continue;
							 }
					   }
				  else {  num=ROUTEDELTA/CL_Snap_Grid;
					  break;
					   }
				}
				   continue;
				 }
		   for(i=0;i<3;i++)
			 {     tempx2=tx2+delta2[i]*ROUTEDELTA;
			   tempy2=tempy1;
			   routevia2.centerx=tempx2;
			   routevia2.centery=tempy2;
			   v2=Clearance_Route((struct Line*)&routevia2);
			   if(v2!=0) continue;
			   else break;
			 }
		   if(v2!=0) {  num++;
				continue;
				 }
		   route3.ex=tempx2;
		   route3.ey=tempy2;
		   route3.elayer=MULTILAYER;
		   route3.sx=tx2;
		   route3.sy=ty2;
		   route3.slayer=route->elayer;
		   n3=Route_Ver_Line_D(&route3,linebuf3);
		   if(n3==0)  { for(;;)
				{  if(flag2==0) try2++;
				   else try2--;
				   tx2=(long)(route->ex)+try2*CL_Snap_Grid;
				   if((tx2>=CL_Size_Maxx)||(tx2<50))
					{   if(flag2==1) goto SCEND;
					else { try2=-1l;
						   flag2=1;
						   continue;
						 }
					}
				   tempvia2.centerx=tx2;
				   tempvia2.centery=ty2;
				   m=Clearance_Route((struct Line*)&tempvia2);
				   if(m!=0) continue;
				   if(flag2==0) {  temproute2.sx=route->ex;
						   temproute2.sy=route->ey;
						   temproute2.slayer=route->elayer;
						   temproute2.ex=tx2;
						   temproute2.ey=ty2;
						   temproute2.elayer=MULTILAYER;
						}
				   else {  temproute2.sx=tx2;
					   temproute2.sy=ty2;
					   temproute2.slayer=MULTILAYER;
					   temproute2.ex=route->ex;
					   temproute2.ey=route->ey;
					   temproute2.elayer=route->elayer;
					}
				   t2=Route_Hor_Line_D(&temproute2,templinebuf2);
				   if(t2==0) {  if(flag2==1) goto SCEND;
						else { try2=-1l;
							   flag2=1;
							   continue;
							 }
						 }
				   else break;
				}
				   continue;
				 }
		   if(tempx2>tempx1)
			{   route2.sx=tempx1;
			route2.sy=tempy1;
			route2.slayer=MULTILAYER;
			route2.ex=tempx2;
			route2.ey=tempy2;
			route2.elayer=MULTILAYER;
			}
		   else
			{   route2.sx=tempx2;
			route2.sy=tempy2;
			route2.slayer=MULTILAYER;
			route2.ex=tempx1;
			route2.ey=tempy1;
			route2.elayer=MULTILAYER;
			}
		   n2=Route_Hor_Line_D(&route2,linebuf2);
		   if(n2==0){  num++;
				   continue;
				}
		   if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)\
			 &&(((try1!=1l)&&(l==0)&&(t1!=0))||(try1==1l))\
			 &&(((try2!=1l)&&(m==0)&&(t2!=0))||(try2==1l))) goto SCEND;
		}
	}
	else
	{
		//MessageBox("8");
		/****************************/
	   /*   x----------------- x   */
	   /*   |                  |   */
	   /*   |                  |   */
	   /*   O                  |   */
	   /*  ex,ey               |   */
	   /*               sx,sy  O   */
	   /****************************/
	   num=ROUTEDELTA/CL_Snap_Grid;
	   tx1=route->sx;
	   ty1=route->sy;
	   tx2=route->ex;
	   ty2=route->ey;
	   try1=try2=1l;
	   flag1=flag2=0;
	   for(;;)
		{
				// interupt route
				for(;;)
				{
					int ch2 = MRetKey();
					if( (ch2 == VK_ESCAPE)||(ch2 == PUSHMOUSEESC))
					{
						ii = AfxMessageBox(interuptroute, MB_YESNO|MB_ICONWARNING);
						if( ii == IDYES)
						{
							return -1;
						}
					}
					break;
				}

		for(i=0;i<3;i++)
			 {     tempx1=tx1+delta1[i]*ROUTEDELTA;
			   tempy1=ty2+CL_Snap_Grid*num;
			   if((tempy1>=CL_Size_Maxy)||(tempy1<ROUTEDELTA)) goto SCEND;
			   routevia1.centerx=tempx1;
			   routevia1.centery=tempy1;
			   v1=Clearance_Route((struct Line*)&routevia1);
			   if(v1!=0) continue;
			   else break;
			 }
		   if(v1!=0) {  num++;
				continue;
				 }
		   route1.ex=tempx1;
		   route1.ey=tempy1;
		   route1.elayer=MULTILAYER;
		   route1.sx=tx1;
		   route1.sy=ty1;
		   route1.slayer=route->slayer;
		   n1=Route_Ver_Line_D(&route1,linebuf1);
		   if(n1==0) { for(;;)
				{ if(flag1==0) try1++;
				  else try1--;
				  tx1=(long)(route->sx)+try1*CL_Snap_Grid;
				  if((tx1>=CL_Size_Maxx)||(tx1<50))
					{   if(flag1==1) goto SCEND;
					else { try1=-1l;
						   flag1=1;
						   continue;
						 }
					}
				  tempvia1.centerx=tx1;
				  tempvia1.centery=ty1;
				  l=Clearance_Route((struct Line*)&tempvia1);
				  if(l!=0) continue;
				  if(flag1==0) {  temproute1.ex=tx1;
						  temproute1.ey=ty1;
						  temproute1.elayer=MULTILAYER;
						  temproute1.sx=route->sx;
						  temproute1.sy=route->sy;
						  temproute1.slayer=route->slayer;
						   }
				  else {  temproute1.ex=route->sx;
					  temproute1.ey=route->sy;
					  temproute1.elayer=route->slayer;
					  temproute1.sx=tx1;
					  temproute1.sy=ty1;
					  temproute1.slayer=MULTILAYER;
					}
				  t1=Route_Hor_Line_D(&temproute1,templinebuf1);
				  if(t1==0) {   if(flag1==1) goto SCEND;
						else { try1=-1l;
							   flag1=1;
							   continue;
							 }
					   }
				  else {  num=ROUTEDELTA/CL_Snap_Grid;
					  break;
					   }
				}
				   continue;
				 }
		   for(i=0;i<3;i++)
			 {     tempx2=tx2+delta2[i]*ROUTEDELTA;
			   tempy2=tempy1;
			   routevia2.centerx=tempx2;
			   routevia2.centery=tempy2;
			   v2=Clearance_Route((struct Line*)&routevia2);
			   if(v2!=0) continue;
			   else break;
			 }
		   if(v2!=0) {  num++;
				continue;
				 }
		   route3.ex=tempx2;
		   route3.ey=tempy2;
		   route3.elayer=MULTILAYER;
		   route3.sx=tx2;
		   route3.sy=ty2;
		   route3.slayer=route->elayer;
		   n3=Route_Ver_Line_D(&route3,linebuf3);
		   if(n3==0)  { for(;;)
				{  if(flag2==0) try2++;
				   else try2--;
				   tx2=(long)(route->ex)-try2*CL_Snap_Grid;
				   if((tx2>=CL_Size_Maxx)||(tx2<50))
					{   if(flag2==1) goto SCEND;
					else { try2=-1l;
						   flag2=1;
						   continue;
						 }
					}
				   tempvia2.centerx=tx2;
				   tempvia2.centery=ty2;
				   m=Clearance_Route((struct Line*)&tempvia2);
				   if(m!=0) continue;
				   if(flag2==0) {  temproute2.ex=route->ex;
						   temproute2.ey=route->ey;
						   temproute2.elayer=route->elayer;
						   temproute2.sx=tx2;
						   temproute2.sy=ty2;
						   temproute2.slayer=MULTILAYER;
						}
				   else {  temproute2.ex=tx2;
					   temproute2.ey=ty2;
					   temproute2.elayer=MULTILAYER;
					   temproute2.sx=route->ex;
					   temproute2.sy=route->ey;
					   temproute2.slayer=route->elayer;
					}
				   t2=Route_Hor_Line_D(&temproute2,templinebuf2);
				   if(t2==0) {  if(flag2==1) goto SCEND;
						else { try2=-1l;
							   flag2=1;
							   continue;
							 }
						 }
				   else break;
				}
				   continue;
				 }
		   if(tempx2>tempx1)
			{   route2.sx=tempx1;
			route2.sy=tempy1;
			route2.slayer=MULTILAYER;
			route2.ex=tempx2;
			route2.ey=tempy2;
			route2.elayer=MULTILAYER;
			}
		   else
			{   route2.sx=tempx2;
			route2.sy=tempy2;
			route2.slayer=MULTILAYER;
			route2.ex=tempx1;
			route2.ey=tempy1;
			route2.elayer=MULTILAYER;
			}
		   n2=Route_Hor_Line_D(&route2,linebuf2);
		   if(n2==0){  num++;
				   continue;
				}
		   if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)\
			 &&(((try1!=1l)&&(l==0)&&(t1!=0))||(try1==1l))\
			 &&(((try2!=1l)&&(m==0)&&(t2!=0))||(try2==1l))) goto SCEND;
		}
	}

SCEND:
   CloseMouseCursor();
   DrawAnyLine(route->sx,route->sy,route->ex,route->ey,Highlighted_Color,1,&dc);
   OpenMouseCursor();
   if((n1!=0)&&(n2!=0)&&(n3!=0)&&(v1==0)&&(v2==0)\
     &&(((try1!=1l)&&(l==0)&&(t1!=0))||(try1==1l))\
     &&(((try2!=1l)&&(m==0)&&(t2!=0))||(try2==1l)))
     { temp=linebuf1;
       for(j=0;j<n1;j++)
	 {  memcpy(&gnline,temp,16);
	    temp+=16;
	    if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
		//----------- Undo -----------
		Check_Undo_Buffer(10);
		undo_p = pDoc->m_pPCB_Undo_Current;
		head.op = UNDO_DELETE;
		head.ob = UNDO_LINE;
		head.p = pDoc->m_pLine_Buffer_Temp;
		memcpy(undo_p,&head,6);
		pDoc->m_pPCB_Undo_Current += 6;
		//---------------------------------------------------------------------------
	    pDoc->Save_EMMLine(&gnline);
	    CloseMouseCursor();
	    DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
	    OpenMouseCursor();
		//MessageBox("line");
	 }
       temp=linebuf2;
       for(j=0;j<n2;j++)
	 {  memcpy(&gnline,temp,16);
	    temp+=16;
	    if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
	    pDoc->Save_EMMLine(&gnline);
	    CloseMouseCursor();
	    DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
	    OpenMouseCursor();
		//MessageBox("line");
	 }
       temp=linebuf3;
       for(j=0;j<n3;j++)
	 {  memcpy(&gnline,temp,16);
	    temp+=16;
	    if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
	    pDoc->Save_EMMLine(&gnline);
	    CloseMouseCursor();
	    DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
	    OpenMouseCursor();
		//MessageBox("line");
	 }
       if(try1!=1l)
	  {    temp=templinebuf1;
	       for(j=0;j<t1;j++)
		 {  memcpy(&gnline,temp,16);
		    temp+=16;
		    if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
		    pDoc->Save_EMMLine(&gnline);
		    CloseMouseCursor();
		    DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
		    OpenMouseCursor();
			//MessageBox("line");
		 }
	  }
       if(try2!=1l)
	  {    temp=templinebuf2;
	       for(j=0;j<t2;j++)
		 {  memcpy(&gnline,temp,16);
		    temp+=16;
		    if((gnline.startx==gnline.endx)&&(gnline.starty==gnline.endy)) continue;
			//----------- Undo -----------
			Check_Undo_Buffer(10);
			undo_p = pDoc->m_pPCB_Undo_Current;
			head.op = UNDO_DELETE;
			head.ob = UNDO_LINE;
			head.p = pDoc->m_pLine_Buffer_Temp;
			memcpy(undo_p,&head,6);
			pDoc->m_pPCB_Undo_Current += 6;
			//---------------------------------------------------------------------------
		    pDoc->Save_EMMLine(&gnline);
		    CloseMouseCursor();
		    DrawStructLine(&gnline,PCB_LayerColor[gnline.layer],&dc);
		    OpenMouseCursor();
			//MessageBox("line");
		 }
	  }
       return 1;
    }
   else return 0;
}


int CDesignView::Route_Hor_Line_D(struct Route_Rats * route, char * linebuf)
{
	int i,k,l,n,m,linenum,flag,flagbak1,flagbak2;
	int m1,n1,m2,n2;
	char *temp;
	struct Line oneline[5];
	unsigned delta;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	
	DrawAnyLine(route->sx,route->sy,route->ex,route->ey,0,1,&dc);
	flag=1;
	for(i=Route_D_Layer;i<=Route_D_Layer;i++)
    {
		if((pDoc->m_nPCB_Layer_Switch[i]==0)) continue;
		if(((route->slayer!=MULTILAYER)&&(route->slayer!=i))||\
			((route->elayer!=MULTILAYER)&&(route->elayer!=i))) continue;
		oneline[0].id=oneline[1].id=oneline[2].id=oneline[3].id=oneline[4].id=0x10;
		oneline[0].layer=oneline[1].layer=oneline[2].layer=oneline[3].layer=oneline[4].layer=i;
		oneline[0].linewidth=oneline[1].linewidth=oneline[2].linewidth=oneline[3].linewidth=oneline[4].linewidth=Route_TW;
		oneline[0].attrib=oneline[1].attrib=oneline[2].attrib=oneline[3].attrib=oneline[4].attrib=PCB;
		oneline[0].flag=oneline[1].flag=oneline[2].flag=oneline[3].flag=oneline[4].flag=0;
		if(route->sy==route->ey)
		{
			/****************/
			/*  mode 1      */
			/*  O<------>O  */
			/****************/
			linenum=1;
			oneline[0].startx=route->sx;
			oneline[0].starty=route->sy;
			oneline[0].endx=route->ex;
			oneline[0].endy=route->ey;
			flag=Clearance_Route(&oneline[0]);
			if(flag==0) goto ROUTEOK;

//HMODE2:    /************************/
			/* mode 2               */
			/*       ________       */
			/*  O---/        \----O */
			/************************/
			flagbak1=flagbak2=0;
			n=(route->ex)-(route->sx);
			m=n;
			l=2;
			for(;;)
			{
				oneline[0].startx=route->sx;
				oneline[0].starty=route->sy;
				oneline[0].endx=route->sx+m;
				oneline[0].endy=route->sy;
				flag=Clearance_Route(&oneline[0]);
				if(flag==0)
				{
					if(((n/l)<=CL_Snap_Grid/2)||(l==2)) break;
					m=m+n/l;
					l=l*2;
				}
				else 
				{
					flagbak1=flag;
					if((n/l)<=CL_Snap_Grid/4)  break;
					m=m-n/l;
					l=l*2;
				}
			}
			k=m/CL_Snap_Grid;
			m1=k; n1=k/2;
			m=n;
			l=2;
			for(;;)
			{
				oneline[0].startx=route->ex;
				oneline[0].starty=route->ey;
				oneline[0].endx=route->ex-m;
				oneline[0].endy=route->ey;
				flag=Clearance_Route(&oneline[0]);
				if(flag==0)
				{
					if(((n/l)<=CL_Snap_Grid/2)||(l==2)) break;
					m=m+n/l;
					l=l*2;
				}
				else 
				{
					flagbak2=flag;
					if((n/l)<=CL_Snap_Grid/4) break;
					m=m-n/l;
					l=l*2;
				}
			}
			k=m/CL_Snap_Grid;
			m2=k; n2=k/2;
			if(flagbak1==1)  n1=m1;
			if(flagbak2==1)  n2=m2;
			if((flagbak1==2)&&(m1>0))  { m1=m1-1; n1=m1; }
			if((flagbak2==2)&&(m2>0))  { m2=m2-1; n2=m2; }

			delta = CL_Snap_Grid;
			do 
			{
				for(k=n1;k<=m1;k++)
				{
					linenum=1;
					oneline[0].startx=route->sx;
					oneline[0].starty=route->sy;
					oneline[0].endx=route->sx + k*CL_Snap_Grid;
					oneline[0].endy=route->sy;
					linenum=2;
					oneline[1].startx=oneline[0].endx;
					oneline[1].starty=oneline[0].endy;
					oneline[1].endx=oneline[0].endx+delta;   /*ROUTEDELTA;*/
					oneline[1].endy=oneline[0].endy+delta;   /*ROUTEDELTA;*/
					flag=Clearance_Route(&oneline[1]);
					if(flag!=0) continue;
					else
					{
						for(l=n2;l<=m2;l++)
						{
							linenum=3;
							oneline[2].startx=route->ex;
							oneline[2].starty=route->ey;
							oneline[2].endx=route->ex-l*CL_Snap_Grid;
							oneline[2].endy=route->ey;
							linenum=4;
							oneline[3].startx=oneline[2].endx;
							oneline[3].starty=oneline[2].endy;
							oneline[3].endx=oneline[2].endx-delta;   /*-ROUTEDELTA;*/
							oneline[3].endy=oneline[2].endy+delta;   /*ROUTEDELTA;*/
							flag=Clearance_Route(&oneline[3]);
							if(flag!=0) continue;
							else
							{
								linenum=5;
								oneline[4].startx=oneline[1].endx;
								oneline[4].starty=oneline[1].endy;
								oneline[4].endx=oneline[3].endx;
								oneline[4].endy=oneline[3].endy;
								flag=Clearance_Route(&oneline[4]);
								if(flag!=0) continue;
								else goto ROUTEOK;
							}
						}
						break;
					}
				}
				delta+=CL_Snap_Grid;
			}while(delta<=ROUTEDELTA);

//HMODE3:    /*************************/
			/*  mode 3               */
			/*  O----\        /----O */
			/*        --------       */
			/*************************/
			delta=CL_Snap_Grid;
			do
			{
				for(k=n1;k<=m1;k++)
				{
					linenum=1;
					oneline[0].startx=route->sx;
					oneline[0].starty=route->sy;
					oneline[0].endx=route->sx + k*CL_Snap_Grid;
					oneline[0].endy=route->sy;
					linenum=2;
					oneline[1].startx=oneline[0].endx;
					oneline[1].starty=oneline[0].endy;
					oneline[1].endx=oneline[0].endx+delta;   /*ROUTEDELTA;*/
					oneline[1].endy=oneline[0].endy-delta;   /*ROUTEDELTA;*/
					flag=Clearance_Route(&oneline[1]);
					if(flag!=0) continue;
					else
					{
						for(l=n2;l<=m2;l++)
						{
							linenum=3;
							oneline[2].startx=route->ex;
							oneline[2].starty=route->ey;
							oneline[2].endx=route->ex-l*CL_Snap_Grid;
							oneline[2].endy=route->ey;
							linenum=4;
							oneline[3].startx=oneline[2].endx;
							oneline[3].starty=oneline[2].endy;
							oneline[3].endx=oneline[2].endx-delta;   /*ROUTEDELTA;*/
							oneline[3].endy=oneline[2].endy-delta;   /*ROUTEDELTA;*/
							flag=Clearance_Route(&oneline[3]);
							if(flag!=0) continue;
							else
							{
								linenum=5;
								oneline[4].startx=oneline[1].endx;
								oneline[4].starty=oneline[1].endy;
								oneline[4].endx=oneline[3].endx;
								oneline[4].endy=oneline[3].endy;
								flag=Clearance_Route(&oneline[4]);
								if(flag!=0) continue;
								else goto ROUTEOK;
							}
						}
						break;
					}
				}
				delta+=CL_Snap_Grid;
			}while(delta<=ROUTEDELTA);
		}
		else	
		{
			/************************/
			/* mode 4               */
			/*  O____________       */
			/*               \----O */
			/************************/
/*RouteMode4:*/
			n=(route->ex)-(route->sx);
			flagbak1=0;
			flagbak2=0;
			m=n;
			l=2;
			for(;;)
			{
				oneline[0].startx=route->sx;
				oneline[0].starty=route->sy;
				oneline[0].endx=route->sx+m;
				oneline[0].endy=route->sy;
				flag=Clearance_Route(&oneline[0]);
				if(flag==0)
				{
					if(((n/l)<=CL_Snap_Grid/2)||(l==2)) break;
					m=m+n/l;
					l=l*2;
				}
				else 
				{
					flagbak1=flag;
					if((n/l)<=CL_Snap_Grid/4) break;
					m=m-n/l;
					l=l*2;
				}
			}
			k=m/CL_Snap_Grid;
			m1=k;
			m=n;
			l=2;
			for(;;)
			{
				oneline[0].startx=route->ex;
				oneline[0].starty=route->ey;
				oneline[0].endx=route->ex-m;
				oneline[0].endy=route->ey;
				flag=Clearance_Route(&oneline[0]);
				if(flag==0)
				{
					if(((n/l)<=CL_Snap_Grid/2)||(l==2)) break;
					m=m+n/l;
					l=l*2;
				}
				else
				{
					flagbak2=flag;
					if((n/l)<=CL_Snap_Grid/4) break;
					m=m-n/l;
					l=l*2;
				}
			}
			k=m/CL_Snap_Grid;
			m2=k;
			m=n/CL_Snap_Grid;
			flag=1;
			linenum=0;
			if(route->sy>route->ey)
			{
				if((m1+m2+(route->sy-route->ey)/CL_Snap_Grid)>=m)
				{
					if((flagbak1==0)&&(flagbak2==0))  m1=m1/2;
					else if(flagbak1==0) 
					{
						m1=m-m2/2;
						if(m1>m-ROUTEDELTA/CL_Snap_Grid) m1=m-ROUTEDELTA/CL_Snap_Grid;
					}
					else if(flagbak2==0) m1=m1/2;
					else 
					{
						if((m1/2+m2+ROUTEDELTA/CL_Snap_Grid)>=m) m1=m1/2;
						else if((m1-1+m2+ROUTEDELTA/CL_Snap_Grid)>m) m1=m1-1;
					}
					linenum=1;
					oneline[0].startx=route->sx;
					oneline[0].starty=route->sy;
					oneline[0].endx=route->sx+m1*CL_Snap_Grid;
					oneline[0].endy=route->sy;
					linenum=2;
					oneline[1].startx=oneline[0].endx;
					oneline[1].starty=oneline[0].endy;
					oneline[1].endx=oneline[0].endx+(route->sy-route->ey);
					oneline[1].endy=route->ey;
					flag=Clearance_Route(&oneline[1]);
					if(flag==0)
					{
						linenum=3;
						oneline[2].startx=oneline[1].endx;
						oneline[2].starty=oneline[1].endy;
						oneline[2].endx=route->ex;
						oneline[2].endy=route->ey;
						goto ROUTEOK;
					}
				}
			}
		/*************************/
		/*  mode 5               */
		/*                /----O */
		/*  O-------------       */
		/*************************/
/*RouteMode5:*/
			else
			{	
				if((m1+m2+(route->ey-route->sy)/CL_Snap_Grid)>=m)
				{
					if((flagbak1==0)&&(flagbak2==0)) m1=m1/2;
					else if(flagbak1==0)
					{
						m1=m-m2/2;
						if(m1>m-ROUTEDELTA/CL_Snap_Grid) m1=m-ROUTEDELTA/CL_Snap_Grid;
					 }
					else if(flagbak2==0) m1=m1/2;
					else 
					{
						if((m1/2+m2+ROUTEDELTA/CL_Snap_Grid)>=m) m1=m1/2;
						else if((m1-1+m2+ROUTEDELTA/CL_Snap_Grid)>m) m1=m1-1;
					}
					linenum=1;
					oneline[0].startx=route->sx;
					oneline[0].starty=route->sy;
					oneline[0].endx=route->sx+m1*CL_Snap_Grid;
					oneline[0].endy=route->sy;
					linenum=2;
					oneline[1].startx=oneline[0].endx;
					oneline[1].starty=oneline[0].endy;
					oneline[1].endx=oneline[0].endx+(route->ey-route->sy);
					oneline[1].endy=route->ey;
					flag=Clearance_Route(&oneline[1]);
					if(flag==0)
					{
						linenum=3;
						oneline[2].startx=oneline[1].endx;
						oneline[2].starty=oneline[1].endy;
						oneline[2].endx=route->ex;
						oneline[2].endy=route->ey;
						goto ROUTEOK;
					}
				}
			}
		}
		if(flag==0) goto ROUTEOK;
		else continue;
	}

ROUTEOK:
	DrawAnyLine(route->sx,route->sy,route->ex,route->ey,0,1,&dc);
	if(flag==0)
	{
		temp=linebuf;
		for(i=0;i<linenum;i++)
		{
			memcpy(temp,&oneline[i],16);
			temp+=16;
		}
		return linenum;
	}
    else  return 0;
}

int CDesignView::Fast_Route_Hor_Line_D(struct Route_Rats * route, char * linebuf)
{
	int i,k,l,n,m,linenum,flag,flagbak1,flagbak2;
	int m1,m2;
	char *temp;
	struct Line oneline[5];
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	flag=1;
	for(i=Route_D_Layer;i<=Route_D_Layer;i++)
    {
		if(pDoc->m_nPCB_Layer_Switch[i]==0) continue;
		if(((route->slayer!=MULTILAYER)&&(route->slayer!=i))||\
			((route->elayer!=MULTILAYER)&&(route->elayer!=i))) continue;
       /*if((route->slayer==MULTILAYER)&&(route->elayer==MULTILAYER)&&(Route_Layer[i]!=1)) continue;*/
		oneline[0].id=oneline[1].id=oneline[2].id=oneline[3].id=oneline[4].id=0x10;
		oneline[0].layer=oneline[1].layer=oneline[2].layer=oneline[3].layer=oneline[4].layer=i;
		oneline[0].linewidth=oneline[1].linewidth=oneline[2].linewidth=oneline[3].linewidth=oneline[4].linewidth=Route_TW;
		oneline[0].attrib=oneline[1].attrib=oneline[2].attrib=oneline[3].attrib=oneline[4].attrib=PCB;
		oneline[0].flag=oneline[1].flag=oneline[2].flag=oneline[3].flag=oneline[4].flag=0;
		if(route->sy==route->ey)
		{
			/****************/
			/*  mode 1      */
			/*  O<------>O  */
			/****************/
			linenum=1;
			oneline[0].startx=route->sx;
			oneline[0].starty=route->sy;
			oneline[0].endx=route->ex;
			oneline[0].endy=route->ey;
			flag=Clearance_Route(&oneline[0]);
			if(flag==0) goto ROUTEOK;
		}
		else
		{
			/************************/
			/* mode 4               */
			/*  O____________       */
			/*               \----O */
			/************************/
/*RouteMode4:*/
			n=(route->ex)-(route->sx);
			flagbak1=0;
			flagbak2=0;
			m=n;
			l=2;
			for(;;)
			{
				oneline[0].startx=route->sx;
				oneline[0].starty=route->sy;
				oneline[0].endx=route->sx+m;
				oneline[0].endy=route->sy;
				flag=Clearance_Route(&oneline[0]);
				if(flag==0) 
				{
					if(((n/l)<=CL_Snap_Grid/2)||(l==2)) break;
					m=m+n/l;
					l=l*2;
				}
				else 
				{
					flagbak1=flag;
					if((n/l)<=CL_Snap_Grid/4) break;
					m=m-n/l;
					l=l*2;
				}
			}
			k=m/CL_Snap_Grid;
			m1=k;
			m=n;
			l=2;
			for(;;)
			{
				oneline[0].startx=route->ex;
				oneline[0].starty=route->ey;
				oneline[0].endx=route->ex-m;
				oneline[0].endy=route->ey;
				flag=Clearance_Route(&oneline[0]);
				if(flag==0)
				{
					if(((n/l)<=CL_Snap_Grid/2)||(l==2)) break;
					m=m+n/l;
					l=l*2;
				}
				else
				{
					flagbak2=flag;
					if((n/l)<=CL_Snap_Grid/4) break;
					m=m-n/l;
					l=l*2;
				}
			}
			k=m/CL_Snap_Grid;
			m2=k;
			m=n/CL_Snap_Grid;
			flag=1;
			linenum=0;
			if(route->sy>route->ey)
			{
				if((m1+m2+(route->sy-route->ey)/CL_Snap_Grid)>=m)
				{
					if((flagbak1==0)&&(flagbak2==0))  m1=m1/2;
					else if(flagbak1==0)  
					{
						m1=m-m2/2;
						if(m1>m-ROUTEDELTA/CL_Snap_Grid) m1=m-ROUTEDELTA/CL_Snap_Grid;
					}
					else if(flagbak2==0) m1=m1/2;
					else
					{
						if((m1/2+m2+ROUTEDELTA/CL_Snap_Grid)>=m) m1=m1/2;
					}
					linenum=1;
					oneline[0].startx=route->sx;
					oneline[0].starty=route->sy;
					oneline[0].endx=route->sx+m1*CL_Snap_Grid;
					oneline[0].endy=route->sy;
					linenum=2;
					oneline[1].startx=oneline[0].endx;
					oneline[1].starty=oneline[0].endy;
					oneline[1].endx=oneline[0].endx+(route->sy-route->ey);
					oneline[1].endy=route->ey;
					flag=Clearance_Route(&oneline[1]);
					if(flag==0)
					{
						linenum=3;
						oneline[2].startx=oneline[1].endx;
						oneline[2].starty=oneline[1].endy;
						oneline[2].endx=route->ex;
						oneline[2].endy=route->ey;
						goto ROUTEOK;
					}
				}
			}
	   /*************************/
	   /*  mode 5               */
	   /*                /----O */
	   /*  O-------------       */
	   /*************************/
/*RouteMode5:*/
			else
			{
				if((m1+m2+(route->ey-route->sy)/CL_Snap_Grid)>=m)
				{ 
					if((flagbak1==0)&&(flagbak2==0)) m1=m1/2;
					else if(flagbak1==0) 
					{
						m1=m-m2/2;
						if(m1>m-ROUTEDELTA/CL_Snap_Grid) m1=m-ROUTEDELTA/CL_Snap_Grid;
					}
					else if(flagbak2==0) m1=m1/2;
					else 
					{
						if((m1/2+m2+ROUTEDELTA/CL_Snap_Grid)>=m) m1=m1/2;
					}
					linenum=1;
					oneline[0].startx=route->sx;
					oneline[0].starty=route->sy;
					oneline[0].endx=route->sx+m1*CL_Snap_Grid;
					oneline[0].endy=route->sy;
					linenum=2;
					oneline[1].startx=oneline[0].endx;
					oneline[1].starty=oneline[0].endy;
					oneline[1].endx=oneline[0].endx+(route->ey-route->sy);
					oneline[1].endy=route->ey;
					flag=Clearance_Route(&oneline[1]);
					if(flag==0)
					{ 
						linenum=3;
						oneline[2].startx=oneline[1].endx;
						oneline[2].starty=oneline[1].endy;
						oneline[2].endx=route->ex;
						oneline[2].endy=route->ey;
						goto ROUTEOK;
					}
				}
			}
		}
		if(flag==0) goto ROUTEOK;
		else continue;
    }
ROUTEOK:
	if(flag==0)
	{
		temp=linebuf;
		for(i=0;i<linenum;i++)
		{
			memcpy(temp,&oneline[i],16);
			temp+=16;
		}
		return linenum;
	}
    else  return 0;
}


int CDesignView::Route_Ver_Line_D(struct Route_Rats * route, char * linebuf)
{
	int i,k,l,n,m,linenum,flag,flagbak1,flagbak2;
	int m1,n1,m2,n2;
	char *temp;
	struct Line oneline[5];
	unsigned delta;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	
	DrawAnyLine(route->sx,route->sy,route->ex,route->ey,0,1,&dc);
	flag=1;
	for(i=Route_D_Layer;i<=Route_D_Layer;i++)
    {
		if(pDoc->m_nPCB_Layer_Switch[i]==0) continue;
		if(((route->slayer!=MULTILAYER)&&(route->slayer!=i))||\
			((route->elayer!=MULTILAYER)&&(route->elayer!=i))) continue;
       /*if((route->slayer==MULTILAYER)&&(route->elayer==MULTILAYER)&&(Route_Layer[i]!=2)) continue;*/
	    oneline[0].id=oneline[1].id=oneline[2].id=oneline[3].id=oneline[4].id=0x10;
		oneline[0].layer=oneline[1].layer=oneline[2].layer=oneline[3].layer=oneline[4].layer=i;
		oneline[0].linewidth=oneline[1].linewidth=oneline[2].linewidth=oneline[3].linewidth=oneline[4].linewidth=Route_TW;
		oneline[0].attrib=oneline[1].attrib=oneline[2].attrib=oneline[3].attrib=oneline[4].attrib=PCB;
		oneline[0].flag=oneline[1].flag=oneline[2].flag=oneline[3].flag=oneline[4].flag=0;
		if(route->sx==route->ex)
		{
			/***********/
			/*  mode 1 */
			/*  Oex,ey */
			/*  ^      */
			/*  |      */
			/*  |      */
			/*  v      */
			/*  Osx,xy */
			/***********/
			linenum=1;
			oneline[0].startx=route->sx;
			oneline[0].starty=route->sy;
			oneline[0].endx=route->ex;
			oneline[0].endy=route->ey;
			flag=Clearance_Route(&oneline[0]);
			if(flag==0) goto ROUTEOK;
			/**********/
			/* mode 2 */
			/*        */
			/*  Oex,ey*/
			/*  |     */
			/*  \     */
			/*   |    */
			/*   |    */
			/*  /     */
			/*  |     */
			/*  Osx,sy*/
			/**********/
//VMODE2:  
			n=(route->ey)-(route->sy);
			flagbak1=flagbak2=0;
			m=n;
			l=2;
			for(;;)
			{
				oneline[0].startx=route->sx;
				oneline[0].starty=route->sy;
				oneline[0].endx=route->sx;
				oneline[0].endy=route->sy+m;
				flag=Clearance_Route(&oneline[0]);
				if(flag==0)
				{
					if(((n/l)<=CL_Snap_Grid/2)||(l==2)) break;
					m=m+n/l;
					l=l*2;
				}
				else
				{
					flagbak1=flag;
					if((n/l)<=CL_Snap_Grid/4) break;
					m=m-n/l;
					l=l*2;
				}
			}
			k=m/CL_Snap_Grid;
			m1=k; n1=k/2;
			m=n;
			l=2;
			for(;;)
			{
				oneline[0].startx=route->ex;
				oneline[0].starty=route->ey;
				oneline[0].endx=route->ex;
				oneline[0].endy=route->ey-m;
				flag=Clearance_Route(&oneline[0]);
				if(flag==0) 
				{
					if(((int)(n/l)<=CL_Snap_Grid/2)||(l==2)) break;
					m=m+n/l;
					l=l*2;
				}
				else 
				{
					flagbak2=flag;
					if((n/l)<=CL_Snap_Grid/4) break;
					m=m-n/l;
					l=l*2;
				}
			}
			k=m/CL_Snap_Grid;
			m2=k; n2=k/2;
			if(flagbak1==1)  n1=m1;
			if(flagbak2==1)  n2=m2;
			if((flagbak1==2)&&(m1>0)) { m1=m1-1; n1=m1; }
			if((flagbak2==2)&&(m2>0)) { m2=m2-1; n2=m2; }
			/*if(((flagbak1==2)||(flagbak1==1))&&(m1>0))  { n1=m1-1; }
			if(((flagbak2==2)||(flagbak1==1))&&(m2>0))  { n2=m2-1; }*/

			delta=CL_Snap_Grid;
			do
			{
				for(k=n1;k<=m1;k++)
				{
					linenum=1;
					oneline[0].startx=route->sx;
					oneline[0].starty=route->sy;
					oneline[0].endx=route->sx;
					oneline[0].endy=route->sy + k*CL_Snap_Grid;
					linenum=2;
					oneline[1].startx=oneline[0].endx;
					oneline[1].starty=oneline[0].endy;
					oneline[1].endx=oneline[0].endx+delta;   /*ROUTEDELTA;*/
					oneline[1].endy=oneline[0].endy+delta;   /*ROUTEDELTA;*/
					flag=Clearance_Route(&oneline[1]);
					if(flag!=0) continue;
					else 
					{
						for(l=n2;l<=m2;l++)
						{   
							linenum=3;
							oneline[2].startx=route->ex;
							oneline[2].starty=route->ey;
							oneline[2].endx=route->ex;
							oneline[2].endy=route->ey - l*CL_Snap_Grid;
							linenum=4;
							oneline[3].startx=oneline[2].endx;
							oneline[3].starty=oneline[2].endy;
							oneline[3].endx=oneline[2].endx+delta;  /*ROUTEDELTA;*/
							oneline[3].endy=oneline[2].endy-delta;  /*ROUTEDELTA;*/
							flag=Clearance_Route(&oneline[3]);
							if(flag!=0) continue;
							else 
							{
								linenum=5;
								oneline[4].startx=oneline[1].endx;
								oneline[4].starty=oneline[1].endy;
								oneline[4].endx=oneline[3].endx;
								oneline[4].endy=oneline[3].endy;
								flag=Clearance_Route(&oneline[4]);
								if(flag!=0) continue;
								else goto ROUTEOK;
							}
						}
						break;
					}
				}
				delta+=CL_Snap_Grid;
			}while(delta<=ROUTEDELTA);
	   /**********/
	   /* mode 3 */
	   /*        */
	   /*  Oex,ey*/
	   /*  |     */
	   /*  /     */
	   /* |      */
	   /* |      */
	   /*  \     */
	   /*  |     */
	   /*  Osx,sy*/
	   /**********/
//VMODE3:		delta=CL_Snap_Grid;
			do
			{
				for(k=n1;k<=m1;k++)
				{
					linenum=1;
					oneline[0].startx=route->sx;
					oneline[0].starty=route->sy;
					oneline[0].endx=route->sx;
					oneline[0].endy=route->sy + k*CL_Snap_Grid;
					linenum=2;
					oneline[1].startx=oneline[0].endx;
					oneline[1].starty=oneline[0].endy;
					oneline[1].endx=oneline[0].endx-delta;  /*ROUTEDELTA;*/
					oneline[1].endy=oneline[0].endy+delta;  /*ROUTEDELTA;*/
					flag=Clearance_Route(&oneline[1]);
					if(flag!=0) continue;
					else
					{
						for(l=n2;l<=m2;l++)
						{
							linenum=3;
							oneline[2].startx=route->ex;
							oneline[2].starty=route->ey;
							oneline[2].endx=route->ex;
							oneline[2].endy=route->ey - l*CL_Snap_Grid;
							linenum=4;
							oneline[3].startx=oneline[2].endx;
				 oneline[3].starty=oneline[2].endy;
				 oneline[3].endx=oneline[2].endx-delta;  /*ROUTEDELTA;*/
				 oneline[3].endy=oneline[2].endy-delta;  /*ROUTEDELTA;*/
				 flag=Clearance_Route(&oneline[3]);
				 if(flag!=0) continue;
				 else { linenum=5;
					oneline[4].startx=oneline[1].endx;
					oneline[4].starty=oneline[1].endy;
					oneline[4].endx=oneline[3].endx;
					oneline[4].endy=oneline[3].endy;
					flag=Clearance_Route(&oneline[4]);
					if(flag!=0) continue;
					else goto ROUTEOK;
				      }
			     }
			     break;
			}
		}
	      delta+=CL_Snap_Grid;
	    } while(delta<=ROUTEDELTA);
	}
       else
	{
	   /**********/
	   /* mode 4 */
	   /*        */
	   /* Oex,ey */
	   /* |      */
	   /* |      */
	   /* |      */
	   /* |      */
	   /*  \     */
	   /*  |     */
	   /*  Osx,sy*/
	   /**********/
/*RouteMode4:*/
	n=(route->ey)-(route->sy);
	flagbak1=0;
	flagbak2=0;
	m=n;
	l=2;
	for(;;)
		{
		   oneline[0].startx=route->sx;
		   oneline[0].starty=route->sy;
		   oneline[0].endx=route->sx;
		   oneline[0].endy=route->sy+m;
		   flag=Clearance_Route(&oneline[0]);
		   if(flag==0) { if(((n/l)<=CL_Snap_Grid/2)||(l==2)) break;
				 m=m+n/l;
				 l=l*2;
			       }
		   else {  flagbak1=flag;
			   if((n/l)<=CL_Snap_Grid/4) break;
			   m=m-n/l;
			   l=l*2;
			}
		}
	k=m/CL_Snap_Grid;
	m1=k;
	m=n;
	l=2;
	for(;;)
	     {
		 oneline[0].startx=route->ex;
		 oneline[0].starty=route->ey;
		 oneline[0].endx=route->ex;
		 oneline[0].endy=route->ey-m;
		 flag=Clearance_Route(&oneline[0]);
		 if(flag==0) { if(((n/l)<=CL_Snap_Grid/2)||(l==2)) break;
			       m=m+n/l;
			       l=l*2;
			     }
		 else {  flagbak2=flag;
			 if((n/l)<=CL_Snap_Grid/4) break;
			 m=m-n/l;
			 l=l*2;
		      }
	     }
	k=m/CL_Snap_Grid;
	m2=k;
	m=n/CL_Snap_Grid;
	flag=1;
	linenum=0;
	if((route->sx)>(route->ex))
	 {   if((m1+m2+(route->sx-route->ex)/CL_Snap_Grid)>=m)
	       {
		   if((flagbak1==0)&&(flagbak2==0))  m1=m1/2;
		   else if(flagbak1==0)  { m1=m-m2/2;
					  if(m1>m-ROUTEDELTA/CL_Snap_Grid) m1=m-ROUTEDELTA/CL_Snap_Grid;
					 }
		   else if(flagbak2==0) m1=m1/2;
		   else { if((m1/2+m2+ROUTEDELTA/CL_Snap_Grid)>=m) m1=m1/2;
			  else if((m1-1+m2+ROUTEDELTA/CL_Snap_Grid)>m) m1=m1-1;
			}
		   linenum=1;
		   oneline[0].startx=route->sx;
		   oneline[0].starty=route->sy;
		   oneline[0].endx=route->sx;
		   oneline[0].endy=route->sy+m1*CL_Snap_Grid;
		   linenum=2;
		   oneline[1].startx=oneline[0].endx;
		   oneline[1].starty=oneline[0].endy;
		   oneline[1].endx=route->ex;
		   oneline[1].endy=oneline[0].endy+(route->sx-route->ex);
		   flag=Clearance_Route(&oneline[1]);
		   if(flag==0)
		     {   linenum=3;
			 oneline[2].startx=oneline[1].endx;
			 oneline[2].starty=oneline[1].endy;
			 oneline[2].endx=route->ex;
			 oneline[2].endy=route->ey;
			 goto ROUTEOK;
		     }
		}
	  }
	   /**********/
	   /* mode 5 */
	   /*        */
	   /*  Oex,ey*/
	   /*  |     */
	   /*  /     */
	   /* |      */
	   /* |      */
	   /* |      */
	   /* |      */
	   /* Osx,sy */
	   /**********/
/*RouteMode5:*/
	 else
	  { if((m1+m2+(route->ex-route->sx)/CL_Snap_Grid)>=m)
	       {   if((flagbak1==0)&&(flagbak2==0))  m1=m1/2;
		   else if(flagbak1==0) { m1=m-m2/2;
					  if(m1>m-ROUTEDELTA/CL_Snap_Grid) m1=m-ROUTEDELTA/CL_Snap_Grid;
					}
		   else if(flagbak2==0) m1=m1/2;
		   else { if((m1/2+m2+ROUTEDELTA/CL_Snap_Grid)>=m) m1=m1/2;
			  else if((m1-1+m2+ROUTEDELTA/CL_Snap_Grid)>m) m1=m1-1;
			}
		   linenum=1;
		   oneline[0].startx=route->sx;
		   oneline[0].starty=route->sy;
		   oneline[0].endx=route->sx;
		   oneline[0].endy=route->sy+m1*CL_Snap_Grid;
		   linenum=2;
		   oneline[1].startx=oneline[0].endx;
		   oneline[1].starty=oneline[0].endy;
		   oneline[1].endx=route->ex;
		   oneline[1].endy=oneline[0].endy+(route->ex-route->sx);
		   flag=Clearance_Route(&oneline[1]);
		   if(flag==0)
		     {   linenum=3;
			 oneline[2].startx=oneline[1].endx;
			 oneline[2].starty=oneline[1].endy;
			 oneline[2].endx=route->ex;
			 oneline[2].endy=route->ey;
			 goto ROUTEOK;
		     }
		}
	  }
	}
       if(flag==0) goto ROUTEOK;
       else continue;
     }
ROUTEOK:
    DrawAnyLine(route->sx,route->sy,route->ex,route->ey,0,1,&dc);
    if(flag==0) {  temp=linebuf;
		   for(i=0;i<linenum;i++)
		     {  memcpy(temp,&oneline[i],16);
			temp+=16;
		     }
		   return linenum;
		}
    else  return 0;
}


int CDesignView::Fast_Route_Ver_Line_D(struct Route_Rats * route, char * linebuf)
{
	int i,k,l,n,m,linenum,flag,flagbak1,flagbak2;
	int m1,m2;
	char *temp;
	struct Line oneline[5];
	//CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	flag=1;
	for(i=Route_D_Layer;i<=Route_D_Layer;i++)
    {
		if(pDoc->m_nPCB_Layer_Switch[i]==0) continue;
		if(((route->slayer!=MULTILAYER)&&(route->slayer!=i))||\
			((route->elayer!=MULTILAYER)&&(route->elayer!=i))) continue;
       /*if((route->slayer==MULTILAYER)&&(route->elayer==MULTILAYER)&&(Route_Layer[i]!=2)) continue;*/
		oneline[0].id=oneline[1].id=oneline[2].id=oneline[3].id=oneline[4].id=0x10;
		oneline[0].layer=oneline[1].layer=oneline[2].layer=oneline[3].layer=oneline[4].layer=i;
		oneline[0].linewidth=oneline[1].linewidth=oneline[2].linewidth=oneline[3].linewidth=oneline[4].linewidth=Route_TW;
		oneline[0].attrib=oneline[1].attrib=oneline[2].attrib=oneline[3].attrib=oneline[4].attrib=PCB;
		oneline[0].flag=oneline[1].flag=oneline[2].flag=oneline[3].flag=oneline[4].flag=0;
		if(route->sx==route->ex)
		{
			/***********/
			/*  mode 1 */
			/*  Oex,ey */
			/*  ^      */
			/*  |      */
			/*  |      */
			/*  v      */
			/*  Osx,xy */
			/***********/
			linenum=1;
			oneline[0].startx=route->sx;
			oneline[0].starty=route->sy;
			oneline[0].endx=route->ex;
			oneline[0].endy=route->ey;
			flag=Clearance_Route(&oneline[0]);
			if(flag==0) goto ROUTEOK;
		}
		else
		{
			/**********/
			/* mode 4 */
			/*        */
			/* Oex,ey */
			/* |      */
			/* |      */
			/* |      */
			/* |      */
			/*  \     */
			/*  |     */
			/*  Osx,sy*/
			/**********/
/*RouteMode4:*/
			n=(route->ey)-(route->sy);
			flagbak1=0;
			flagbak2=0;
			m=n;
			l=2;
			for(;;)
			{
				oneline[0].startx=route->sx;
				oneline[0].starty=route->sy;
				oneline[0].endx=route->sx;
				oneline[0].endy=route->sy+m;
				flag=Clearance_Route(&oneline[0]);
				if(flag==0)
				{
					if(((n/l)<=CL_Snap_Grid/2)||(l==2)) break;
					m=m+n/l;
					l=l*2;
				}
				else
				{
					flagbak1=flag;
					if((n/l)<=CL_Snap_Grid/4) break;
					m=m-n/l;
					l=l*2;
				}
			}
			k=m/CL_Snap_Grid;
			m1=k;
			m=n;
			l=2;
			for(;;)
			{
				oneline[0].startx=route->ex;
				oneline[0].starty=route->ey;
				oneline[0].endx=route->ex;
				oneline[0].endy=route->ey-m;
				flag=Clearance_Route(&oneline[0]);
				if(flag==0) 
				{
					if(((n/l)<=CL_Snap_Grid/2)||(l==2)) break;
					m=m+n/l;
					l=l*2;
				}
				else 
				{
					flagbak2=flag;
					if((n/l)<=CL_Snap_Grid/4) break;
					m=m-n/l;
					l=l*2;
				}
			}
			k=m/CL_Snap_Grid;
			m2=k;
			m=n/CL_Snap_Grid;
			flag=1;
			linenum=0;
			if((route->sx)>(route->ex))
			{
				if((m1+m2+(route->sx-route->ex)/CL_Snap_Grid)>=m)
				{ 
					if((flagbak1==0)&&(flagbak2==0))   m1=m1/2;
					else if(flagbak1==0) 
					{
						m1=m-m2/2;
						if(m1>m-ROUTEDELTA/CL_Snap_Grid) m1=m-ROUTEDELTA/CL_Snap_Grid;
					}
					else if(flagbak2==0) m1=m1/2;
					else
					{
						if((m1/2+m2+ROUTEDELTA/CL_Snap_Grid)>=m) m1=m1/2;
					}
					linenum=1;
					oneline[0].startx=route->sx;
					oneline[0].starty=route->sy;
					oneline[0].endx=route->sx;
					oneline[0].endy=route->sy+m1*CL_Snap_Grid;
					linenum=2;
					oneline[1].startx=oneline[0].endx;
					oneline[1].starty=oneline[0].endy;
					oneline[1].endx=route->ex;
					oneline[1].endy=oneline[0].endy+(route->sx-route->ex);
					flag=Clearance_Route(&oneline[1]);
					if(flag==0)
					{
						linenum=3;
						oneline[2].startx=oneline[1].endx;
						oneline[2].starty=oneline[1].endy;
						oneline[2].endx=route->ex;
						oneline[2].endy=route->ey;
						goto ROUTEOK;
					}
				}
			}
			/**********/
			/* mode 5 */
			/*        */
			/*  Oex,ey*/
			/*  |     */
			/*  /     */
			/* |      */
			/* |      */
			/* |      */
			/* |      */
			/* Osx,sy */
			/**********/
/*RouteMode5:*/
			else
			{
				if((m1+m2+(route->ex-route->sx)/CL_Snap_Grid)>=m)
				{
					if((flagbak1==0)&&(flagbak2==0))   m1=m1/2;
					else if(flagbak1==0) 
					{
						m1=m-m2/2;
						if(m1>m-ROUTEDELTA/CL_Snap_Grid) m1=m-ROUTEDELTA/CL_Snap_Grid;
					}
					else if(flagbak2==0) m1=m1/2;
					else
					{
						if((m1/2+m2+ROUTEDELTA/CL_Snap_Grid)>=m) m1=m1/2;
					}
					linenum=1;
					oneline[0].startx=route->sx;
					oneline[0].starty=route->sy;
					oneline[0].endx=route->sx;
					oneline[0].endy=route->sy+m1*CL_Snap_Grid;
					linenum=2;
					oneline[1].startx=oneline[0].endx;
					oneline[1].starty=oneline[0].endy;
					oneline[1].endx=route->ex;
					oneline[1].endy=oneline[0].endy+(route->ex-route->sx);
					flag=Clearance_Route(&oneline[1]);
					if(flag==0)
					{
						linenum=3;
						oneline[2].startx=oneline[1].endx;
						oneline[2].starty=oneline[1].endy;
						oneline[2].endx=route->ex;
						oneline[2].endy=route->ey;
						goto ROUTEOK;
					}
				}
			}
		}
		if(flag==0) goto ROUTEOK;
		else continue;
     }

ROUTEOK:
	if(flag==0) 
	{
		temp=linebuf;
		for(i=0;i<linenum;i++)
		{
			memcpy(temp,&oneline[i],16);
			temp+=16;
		}
		return linenum;
	}
    else  return 0;
}

