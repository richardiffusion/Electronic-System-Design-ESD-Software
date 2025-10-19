void CDesignView::OnRouteAll() 
{
	int i;
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	EnableFrame(FALSE);
	for(i=0;i<6;i++) 
	{
		if(Route_Layer[i]==3) break;  //=3说明是单面板布线
	}
	if(i==6)  Route_Board();
	else //if( (i==TOPLAYER)||(i==BOTTOMLAYER) )
	{
		Route_D_Layer = i;
	    Route_Board_D();
	}
	//------------------- UNDO  ------------------------------------------------------------
	if(pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] != pDoc->m_pPCB_Undo_Current)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num = 0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//--------------------------------------------------------------------------------------
	Write_Help(commandend);
	EnableFrame(TRUE);
}


void CDesignView::Route_Board()
{
	int i,ii,j,k,judge,routednum,gridbak,routemodebak,swapbak;
	int flag;
	int ch1;
	int ratsnestnum;
	unsigned tempx,tempy;
	char *route_ratsnest, *temp_ratsnest;
	char *swappad1, *swappad2, *swaptrack1, *swaptrack2;
	char *swappad1bak, *swappad2bak, *swaptrack1bak, *swaptrack2bak;
	struct Route_Rats route,route1;
	char str1[60],str2[100],str[60];
	float f1;
	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if(pDoc->m_nNet_Number==0) 
	{
		Error( error16 );
		return;
	}

	gridbak = CL_Snap_Grid;
	//if(Snap_Grid<5) Snap_Grid=5;
	CL_Snap_Grid = 25;
	routemodebak = Route_Mode;
	Route_Mode = 0;
	swapbak = Route_Switch[9];
	//if(Route_Switch[1]==1)  Route_Switch[9] = 1;

	if((route_ratsnest = (char*)malloc(64000))==NULL)
	{
		memory_error();
		return;
	}
	if(Route_Switch[9]==1)		//交叉搜索
    {
	   swappad1 = (char*)malloc(30000);
	   swappad2 = (char*)malloc(30000);
	   swaptrack1 = (char*)malloc(50000);
	   swaptrack2 = (char*)malloc(50000);
	   if((swappad1==NULL)||(swappad2==NULL)||(swaptrack1==NULL)||(swaptrack2==NULL))
	   {
		   memory_error();
		   return;
	   }
	   swappad1bak = swappad1; swappad2bak = swappad2;
	   swaptrack1bak = swaptrack1; swaptrack2bak = swaptrack2;
    }

	if(Route_Switch[0] == 1)	//处理SMD预布线, 相当于扇出fanout
    {
		CloseMouseCursor();
		Write_Help(helpstr50);
		OpenMouseCursor();
		Ratsnest_Refresh(0);
		if(Route_Switch[1] == 1)
	    {
			ratsnestnum=0;
			pDoc->Fresh_Package_Table(1,Null_Str);
			temp_ratsnest=route_ratsnest;
			pDoc->m_ptemp_Package_Table = pDoc->m_pPackage_Table;
			Ratsnestflag = 1;
			for(ch1=0;ch1<pDoc->m_nNet_Number;ch1++)
		    {
				temp_ratsnest=route_ratsnest+ratsnestnum*13;
				ratsnestnum += pDoc->Get_Onenet_Ratsnest(temp_ratsnest,ch1);
		    }
			temp_ratsnest=route_ratsnest;
			PWR_SMD_Pad(ratsnestnum,temp_ratsnest);
			Ratsnest_Refresh(0);
			Show_Ratsnest(&dc);
		}
    }

	if(PCBHighlightBufferInit() != 0) return;
	Write_Help(helpstr51);
	ratsnestnum=0;

	// 获取所有网络的飞线
	pDoc->Fresh_Package_Table(1,Null_Str);
	temp_ratsnest=route_ratsnest;
	pDoc->m_ptemp_Package_Table = pDoc->m_pPackage_Table;
	Ratsnestflag=1;
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
	if(Route_Switch[1]==1)
	{
		k = Route_D_Layer;
		CloseMouseCursor();
		Write_Help(helpstr50);
		OpenMouseCursor();
		temp_ratsnest=route_ratsnest;
		for(i=0;i<ratsnestnum;i++)
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
			if((abs(route.sy-route.ey)==0)&&(route.slayer==route.elayer)&&\
			(route.slayer!=MULTILAYER)&&(route.length<=(2*ROUTEDELTA)))
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
				Route_D_Layer = route.slayer;
				if(Route_Hor_D(&route)!=0)
				{
					routednum++;
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
				}
			}
			else if((abs(route.sx-route.ex)==0)&&(route.slayer==route.elayer)&&\
		      (route.slayer!=MULTILAYER)&&(route.length<=(2*ROUTEDELTA)))
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
				Route_D_Layer = route.slayer;
				if(Route_Ver_D(&route)!=0)
				{
					routednum++;
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
				}
			}
			temp_ratsnest+=13;
		}

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
			if((abs(route.sy-route.ey)<=ROUTEDELTA)&&(route.slayer==route.elayer)&&\
				(route.slayer!=MULTILAYER)&&(route.length<=(2*ROUTEDELTA)))
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
				Route_D_Layer = route.slayer;
				if(Route_Hor_D(&route)!=0)
				{
					routednum++;
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
				}
			}
			else if((abs(route.sx-route.ex)<=ROUTEDELTA)&&(route.slayer==route.elayer)&&\
		      (route.slayer!=MULTILAYER)&&(route.length<=(2*ROUTEDELTA)))
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
				Route_D_Layer = route.slayer;
				if(Route_Ver_D(&route)!=0)
				{
					routednum++;
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
				}
			}
			temp_ratsnest+=13;
		}
	}

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
				if(Route_Hor(&route)!=0)
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
				if(Route_Ver(&route)!=0)
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

	if(Route_Switch[1]==1)
	{
		temp_ratsnest=route_ratsnest;
		Swap_SMD_Pad(ratsnestnum, temp_ratsnest);
		Route_D_Layer = k;
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
			if(Route_Switch[9]==1)
			{
				for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
				for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
				swappad1bak=swappad1; swappad2bak=swappad2;
				swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
				Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
				Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			}
			Highlight_Twopad(&route);
			flag=0;
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
					if(Route_L(&route)!=0) flag=1;
				}
			}
			if(Route_Switch[9]==1)
			{
				if(flag==0)
				{
					if(Route_Swap_H_V(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; 
				}
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
			if(Route_Switch[9]==1)
			{
				for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
				for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
				swappad1bak=swappad1; swappad2bak=swappad2;
				swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
				Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
				Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			}
			Highlight_Twopad(&route);
			flag=0;
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
					if(Route_Z(&route,0)!=0) flag=1;
				}
			}
			if(Route_Switch[9]==1)
			{
				if(flag==0)
				{
					if(Route_Swap_L(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; 
				}
				//if(flag==0) { if(Route_Swap_Z(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak)!=0) flag=1; }
			}
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
			if(Route_Switch[9]==1)
			{
				for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
				for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
				swappad1bak=swappad1; swappad2bak=swappad2;
				swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
				Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
				Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			}
			Highlight_Twopad(&route);
			flag=0;
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
				if(Route_C(&route)!=0) flag=1;
			}
			if(Route_Switch[9]==1)
			{
				if(flag==0)
				{
					if(Route_Swap_L(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; 
				}
				//if(flag==0) { if(Route_Swap_Z(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak)!=0) flag=1; }
				//if(flag==0) { if(Route_Swap_C(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak)!=0) flag=1; }
			}
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
			if(Route_Switch[9]==1)
			{
				for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
				for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
				swappad1bak=swappad1; swappad2bak=swappad2;
				swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
				Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
				Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			}
			Highlight_Twopad(&route);
			flag=0;
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
				j=Super_C(&route);
				if(j<0) goto REND;
				else if(j>0) flag=1;
			}
			if(Route_Switch[9]==1)
			{
				//if(flag==0) { if(Route_Swap_L(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak)!=0) flag=1; }
				if(flag==0) { if(Route_Swap_Z(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
				if(flag==0) { if(Route_Swap_C(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
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
			if(Route_Switch[9]==1)
			{
				for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
				for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
				swappad1bak=swappad1; swappad2bak=swappad2;
				swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
				Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
				Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			}
			Highlight_Twopad(&route);
			flag=0;
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
				j=Super_Z(&route);
				if(j<0) goto REND;
				else if(j>0) flag=1;
			}
			if(Route_Switch[9]==1)
			{
				if(flag==0) { if(Route_Swap_Z(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
				if(flag==0) { if(Route_Swap_C(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			}
			if((flag==0)&&(Route_Switch[9]==1))
			{
				j=Route_Swap_SuperZC(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length);
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
	sprintf(str,"%s%s%s%s%s%4.1f%s","飞线总数=",str1," 已布线=",str2," 布通率Rate=",f1,"%");
	AfxMessageBox(str, MB_OK|MB_ICONINFORMATION);
	Ratsnestflag=0;
	CL_Snap_Grid = gridbak;
	Route_Mode=routemodebak;
	Route_Switch[9] = swapbak;
	free(route_ratsnest);
	if(Route_Switch[9]==1)
	{
		free(swappad1); free(swappad2); free(swaptrack1); free(swaptrack2);
	}
	Free_PCBHighlightbuf();
	Ratsnest_Refresh(0);
	//EnableFrame(TRUE);
}

void CDesignView::OnRouteNet()
{
	int i;
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	pDoc->SetModifiedFlag(TRUE);

	//EnableFrame(FALSE);
	for(i=0;i<6;i++)
	{
		if(Route_Layer[i]==3) break;
	}
	if(i==6)  Route_Net();
	else //if( (i==TOPLAYER)||(i==BOTTOMLAYER) )
	{
		Route_D_Layer = i;
		Route_Net_D();
	}

	//------------------- UNDO  ------------------------------------------------------------
	if(pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] != pDoc->m_pPCB_Undo_Current)
	{
		pDoc->m_nPCB_Undo_Num++;
		if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
		pDoc->m_nPCB_Redo_Num = 0;
		pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
	}
	//--------------------------------------------------------------------------------------
	Write_Help(commandend);
	EnableFrame(TRUE);
}

void CDesignView::Route_Net(void)
{
	int i,ii,j,k,num,judge,gridbak,routemodebak,flag,swapbak;
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
	for(i=0;i<pDoc->m_nNet_Number;i++)
	{
		nets[i] = temp;
		temp+=strlen(temp)+1;
	}
		
	CSelectionDlg selection;
	selection.m_TitleText = select_net;
	selection.m_pList = nets;
	selection.m_nNum = pDoc->m_nNet_Number;

	Write_Help("选择布线网络");
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
	
	if(Route_Switch[9]==1)
    {
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
    }

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
		/*if(Route_Switch[1] == 1)
	    {
			ratsnestnum=0;
			pDoc->Fresh_Package_Table(2,str);
			temp_ratsnest=route_ratsnest;
			pDoc->m_ptemp_Package_Table = pDoc->m_pPackage_Table;
			Ratsnestflag = 1;
			for(ch1=0;ch1<pDoc->m_nNet_Number;ch1++)
		    {
				temp_ratsnest=route_ratsnest+ratsnestnum*13;
				ratsnestnum += pDoc->Get_Onenet_Ratsnest(temp_ratsnest,ch1);
		    }
			temp_ratsnest=route_ratsnest;
			PWR_SMD_Pad(ratsnestnum,temp_ratsnest);
			Ratsnest_Refresh(0);
	    }*/
	}

	if(PCBHighlightBufferInit() != 0) 
	{
		if(Route_Switch[9]==1)
		{
			free(swappad1); free(swappad2); free(swaptrack1); free(swaptrack2);
		}
		free(packmem);
		free(route_ratsnest);
		Free_PCBHighlightbuf();	
		return;
	}
	CloseMouseCursor();
	Write_Help(helpstr51);
	OpenMouseCursor();

	ratsnestnum=0;
	// 获取一个网络的飞线
	Ratsnestflag = 2;
	strcpy(Ratsneststring,str);
	pDoc->Fresh_Package_Table(Ratsnestflag,str);

	ratsnestnum = pDoc->Get_All_Ratsnest(route_ratsnest);

	num = ratsnestnum;
	if(num<1) goto RNEND;

	EnableFrame(FALSE);

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

	if(Route_Switch[1]==1)
	{
		k = Route_D_Layer;
		CloseMouseCursor();
		Write_Help(helpstr50);
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
			if((abs(route.sy-route.ey)==0)&&(route.slayer==route.elayer)&&\
				(route.slayer!=MULTILAYER)&&(route.length<=(2*ROUTEDELTA)))
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
				Route_D_Layer = route.slayer;
				if(Route_Hor_D(&route)!=0)
				{
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
				}
			}
			else if((abs(route.sx-route.ex)==0)&&(route.slayer==route.elayer)&&\
		      (route.slayer!=MULTILAYER)&&(route.length<=(2*ROUTEDELTA)))
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
				Route_D_Layer = route.slayer;
				if(Route_Ver_D(&route)!=0)
				{
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
				}
			}
			temp_ratsnest+=13;
		}

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
			if((abs(route.sy-route.ey)<=ROUTEDELTA)&&(route.slayer==route.elayer)&&\
			(route.slayer!=MULTILAYER)&&(route.length<=(2*ROUTEDELTA)))
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
				Route_D_Layer = route.slayer;
				if(Route_Hor_D(&route)!=0)
				{
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
				}
			}
			else if((abs(route.sx-route.ex)<=ROUTEDELTA)&&(route.slayer==route.elayer)&&\
		      (route.slayer!=MULTILAYER)&&(route.length<=(2*ROUTEDELTA)))
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
				Route_D_Layer = route.slayer;
				if(Route_Ver_D(&route)!=0)
				{
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
				}
			}
			temp_ratsnest+=13;
		}
	}

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
				if(Route_Hor(&route)!=0)
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
				if(Route_Ver(&route)!=0)
				{
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
				}
			}
			temp_ratsnest+=13;
		}
	}

/*	if(Route_Switch[1]==1)
	{
		temp_ratsnest=route_ratsnest;
		Swap_SMD_Pad(num, temp_ratsnest);
		Route_D_Layer = k;
	}*/

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
			if(Route_Switch[9]==1)
			{
				for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
				for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
				swappad1bak=swappad1; swappad2bak=swappad2;
				swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
				Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
				Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			}
			Highlight_Twopad(&route);
			flag=0;
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
					if(Route_L(&route)!=0) flag=1;
				}
			}
			if(Route_Switch[9]==1)
			{
				if(Route_Swap_H_V(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) { flag=1; }
			}
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
			if(Route_Switch[9]==1)
			{
				for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
				for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
				swappad1bak=swappad1; swappad2bak=swappad2;
				swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
				Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
				Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			}
			Highlight_Twopad(&route);
			flag=0;
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
					if(Route_Z(&route,0)!=0) flag=1;
				}
			}
			if(Route_Switch[9]==1)
			{
				if(flag==0)
				{
					if(Route_Swap_L(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; 
				}
				//if(flag==0) { if(Route_Swap_Z(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak)!=0) flag=1; }
			}
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
			if(Route_Switch[9]==1)
			{
				for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
				for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
				swappad1bak=swappad1; swappad2bak=swappad2;
				swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
				Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
				Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			}
			Highlight_Twopad(&route);
			flag=0;
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
				if(Route_C(&route)!=0) flag=1;
			}
			if(Route_Switch[9]==1)
			{
				if(flag==0)
				{
					if(Route_Swap_L(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; 
				}
			}
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
			if(Route_Switch[9]==1)
			{
				for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
				for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
				swappad1bak=swappad1; swappad2bak=swappad2;
				swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
				Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
				Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			}
			Highlight_Twopad(&route);
			flag=0;
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
				j=Super_C(&route);
				if(j<0) goto RNEND;
				else if(j>0) flag=1;
			}
			if(Route_Switch[9]==1)
			{
				if(flag==0) { if(Route_Swap_Z(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
				if(flag==0) { if(Route_Swap_C(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
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
			if(Route_Switch[9]==1)
			{
				for(j=0;j<30000;j++) {swappad1[j]=swappad2[j]=0;}
				for(j=0;j<50000;j++) {swaptrack1[j]=swaptrack2[j]=0;}
				swappad1bak=swappad1; swappad2bak=swappad2;
				swaptrack1bak=swaptrack1; swaptrack2bak=swaptrack2;
				Highlight_Onepad(route.sx,route.sy,route.slayer,swappad1bak,swaptrack1bak);
				Highlight_Onepad(route.ex,route.ey,route.elayer,swappad2bak,swaptrack2bak);
			}
			Highlight_Twopad(&route);
			flag=0;
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
				j=Super_Z(&route);
				if(j<0) goto RNEND;
				else if(j>0) flag=1;
			}
			if(Route_Switch[9]==1)
			{
				if(flag==0) { if(Route_Swap_Z(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
				if(flag==0) { if(Route_Swap_C(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length)!=0) flag=1; }
			}
			if((flag==0)&&(Route_Switch[9]==1))
			{
				j=Route_Swap_SuperZC(swappad1bak,swaptrack1bak,swappad2bak,swaptrack2bak,route.length);
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
	Ratsnestflag = 0;
	CL_Snap_Grid = gridbak;
	Route_Mode=routemodebak;
	Route_Switch[9] = swapbak;
	if(Route_Switch[9]==1)
	{
		free(swappad1); free(swappad2); free(swaptrack1); free(swaptrack2);
	}
	free(packmem);
	free(route_ratsnest);
	Free_PCBHighlightbuf();	
}

void CDesignView::OnRouteRatslength() 
{
	int i, ratsnestnum;
	char *route_ratsnest, *temp_ratsnest;
	char str[100],str1[10],str2[10];
	int totallength;
	struct Route_Rats route;

	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	if((route_ratsnest = (char*)malloc(130000))==NULL)
	{
		memory_error();
		return;
	}
	ratsnestnum = pDoc->Get_All_Ratsnest(route_ratsnest);

	temp_ratsnest = route_ratsnest;
	totallength = 0;
	for(i=0;i<ratsnestnum;i++)
    {
		memcpy(&route,temp_ratsnest,13);
		temp_ratsnest+=13;
		if(route.state==0)
		{
			totallength += (int)sqrt(((float)route.sx-route.ex)*((float)route.sx-route.ex)+\
					((float)route.sy-route.ey)*((float)route.sy-route.ey));
		}
    }
	free(route_ratsnest);
	itoa(ratsnestnum,str1,10);
	ltoa(totallength,str2,10);
	sprintf(str,"%s%s%s%s%s","飞线总数 = ",str1,"; 飞线总长度 = ",str2,"mil;");
	Write_Help("显示所有飞线的总长度");
	AfxMessageBox(str, MB_OK|MB_ICONINFORMATION);
	Write_Help(commandend);
}

void CDesignView::OnRouteManuel() 
{
	int width; 
	unsigned ch1;
	unsigned x1,y1,x2,y2,x3,y3, tempx, tempy;
	int xs,ys,xe,ye,xe1,ye1,flag,layerbak;
	int drawmode;
	struct Undo_Head head;
	char *undo_p;

	int i;
	unsigned xx1,yy1,xx2,yy2,fflag,ratsflag;
	int xxs,yys,xxe,yye;
	struct Route_Rats route;
	char *route_ratsnest, *temp_ratsnest;
	int route_ratsnum=0;

	CClientDC dc(this);
	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();
	CPen OverwritePen, RatsPen, LinePen, *oldpen;

	pDoc->SetModifiedFlag(TRUE);

	CL_Auto_Pan=ON;
	//Mouse_In_Work_Area();
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	DrawStructButton(ID_EDIT_PLACE_TRACK,TRUE);
	Write_Help(helpstr3);

	width = Current_Linewidth/m_nSScale;
	if(width < 1) width = 1;
	OverwritePen.CreatePen(PS_SOLID,width,Overwrite_Color);
	RatsPen.CreatePen(PS_SOLID,1,Overwrite_Color);

	if(pDoc->m_nNet_Number==0) 
	{
		Error( error16 );
	    return;
	}
	
	if((route_ratsnest = (char*)malloc(130000))==NULL)
	{
		memory_error();
		return;
	}
	if(Ratsnestflag!=0) route_ratsnum = pDoc->Get_All_Ratsnest(route_ratsnest);

	//oldpen = dc.SelectObject(&OverwritePen);
	//dc.SelectObject(oldpen);
	//drawmode = dc.GetROP2();
	//dc.SetROP2(R2_NOTXORPEN);
	//dc.SetROP2(drawmode);

	flag = 0;
	fflag = 0;
	ratsflag = 0;
	CL_RedrawFlag = 0;
	do
	{ 
		ch1 = MouseKey();
		if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
		{
			layerbak = m_nPCB_Current_Layer;
			x1 = m_nCurrent_X;
			y1 = m_nCurrent_Y;
			x2 = x3 = x1; 
			y2 = y3 = y1;
			Get_VPort_XY(&xs,&ys,x1,y1);
			xe = xe1 = xs;
			ye = ye1 = ys;

			Write_Help(helpstr4);
			if(m_nPCB_Current_Layer <= BOTTOMLAYER) ratsflag = 1;
			else ratsflag = 0;

			if(ratsflag != 0)
			{
				temp_ratsnest=route_ratsnest;
				flag=0;
				fflag=0;
				for(i=0;i<route_ratsnum;i++)
				{
					memcpy(&route,temp_ratsnest,13);
					if((abs(route.sx-x1)<25)&&(abs(route.sy-y1)<25)&&(route.state==0))
					{
						xx1 = route.ex;
						yy1 = route.ey;
						xx2 = x3; 
						yy2 = y3;
						Get_VPort_XY(&xxs,&yys,xx1,yy1);
						xxe = xe; yye = ye;
						fflag=1;
						CloseMouseCursor();
						DrawAnyLine(route.sx,route.sy,route.ex,route.ey,Overwrite_Color,1,&dc);
						OpenMouseCursor();
						route.state=1;
						memcpy(temp_ratsnest,&route,13);
						break;
					 }
					else if((abs(route.ex-x1)<25)&&(abs(route.ey-y1)<25)&&(route.state==0))
					{
						xx1=route.sx;
						yy1=route.sy;
						xx2=x3; yy2=y3;
						Get_VPort_XY(&xxs,&yys,xx1,yy1);
						xxe=xe; yye=ye;
						fflag=2;
						CloseMouseCursor();
						DrawAnyLine(route.sx,route.sy,route.ex,route.ey,Overwrite_Color,1,&dc);
						OpenMouseCursor();
						route.state=1;
						memcpy(temp_ratsnest,&route,13);
						break;
					}
					temp_ratsnest+=13;
				}
			}

			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xs,&ys,x1,y1);
				Get_VPort_XY(&xxs,&yys,xx1,yy1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{ 
					Write_Help(helpstr3);

					//setwritemode(1);
					//setcolor(Overwrite_Color);
					drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);
					oldpen = dc.SelectObject(&OverwritePen);
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xe1,ye1);

					if(fflag!=0) 
					{
						dc.SelectObject(&RatsPen);
						dc.MoveTo(xxs,yys);
						dc.LineTo(xxe,yye);
					}

					dc.SelectObject(oldpen);
					dc.SetROP2(drawmode);

					ch1 = MOUSEMOVE;
					break;
				}
				else if(ch1==MOUSEMOVE)
				{
TEMP:
					if(flag==0)
					{
						flag = 1;
						CloseMouseCursor();
						//setwritemode(1);
						//setcolor(Overwrite_Color);
						//line(xs,ys,xe,ye);
						//if(fflag!=0)  line(xxs,yys,xxe,yye);
						drawmode = dc.GetROP2();
						dc.SetROP2(R2_NOTXORPEN);
						oldpen = dc.SelectObject(&OverwritePen);
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ye);
						//line(xs,ys,xe,ye);
						if(fflag!=0) 
						{
							dc.SelectObject(&RatsPen);
							dc.MoveTo(xxs,yys);
							dc.LineTo(xxe,yye);
							//line(xxs,yys,xxe,yye);
						}
						dc.SelectObject(oldpen);
						dc.SetROP2(drawmode);
						OpenMouseCursor();
					}
					else
					{
						if(CL_RedrawFlag==1)
						{ 
							OverwritePen.DeleteObject();
							width = Current_Linewidth/m_nSScale;
							if(width < 1) width = 1;
							OverwritePen.CreatePen(PS_SOLID,width,Overwrite_Color);
							CL_RedrawFlag=0;
						}
						else  
						{
							//setwritemode(1);
							//setcolor(Overwrite_Color);
							drawmode = dc.GetROP2();
							dc.SetROP2(R2_NOTXORPEN);
							oldpen = dc.SelectObject(&OverwritePen);
							dc.MoveTo(xs,ys);
							dc.LineTo(xe,ye);
							dc.LineTo(xe1,ye1);
							if(fflag!=0) 
							{
								dc.SelectObject(&RatsPen);
								dc.MoveTo(xxs,yys);
								dc.LineTo(xxe,yye);
								//line(xxs,yys,xxe,yye);
							}
							dc.SelectObject(oldpen);
							dc.SetROP2(drawmode);
						}
						x3 = x2 = m_nCurrent_X; y3 = y2 = m_nCurrent_Y;
						Get_VPort_XY(&xe,&ye,x2,y2);
						xe1 = xe;  ye1 = ye;
						if(CL_Ortho_Mode==ON)
						{ 
							if(PCB_Line_Angle == 1)
							{
								if((x3 == x1)&&(y3 == y1)) {}
								else if(x3 == x1){	}
								else if(y3 == y1){	}
								else if((x3>x1)&&(y3>y1))
								{  
									//         / -------------- x3,y3
									//        /
									// x1,y1 /

									tempx=x3-x1;  tempy=y3-y1;
									if(tempx == tempy){	}
									else if(tempx > tempy)
									{  
										if(tempx > 10*tempy)
										{
											x2 = x3 - tempy;
											y2 = y1;
										}
										else
										{
											x2 = x1 + tempy;
											y2 = y3;
										}
									}
									else
									{ 
										if(tempy > 10*tempx)
										{
											y2 = y3 - tempx;
											x2 = x1;
										}
										else
										{
											y2 = y1 + tempx;
											x2 = x3;
										}
									}
								}
								else if((x1>x3)&&(y3>y1))
								{
									// x3,y3 -----------------\
									//						   \	
									//                          \ x1,y1
									
									tempx=x1-x3;  tempy=y3-y1;
									if(tempx == tempy){	}
									else if(tempx > tempy)
									{  
										if(tempx > 10*tempy)
										{
											x2 = x3 + tempy;
											y2 = y1;
										}
										else
										{
											x2 = x1 - tempy;
											y2 = y3;
										}
									}
									else
									{ 
										if(tempy > 10*tempx)
										{
											y2 = y3 - tempx;
											x2 = x1;
										}
										else
										{
											y2 = y1 + tempx;
											x2 = x3;
										}
									}
								}
								else if((x3>x1)&&(y1>y3))
								{  
									//  x1, y1 --------------\
									//                        \
									//                         \  x3,y3

									tempx=x3-x1;  tempy=y1-y3;
									if(tempx == tempy){	}
									else if(tempx > tempy)
									{  
										if(tempx > 10*tempy)
										{
											x2 = x3 - tempy;
											y2 = y1;
										}
										else
										{
											x2 = x1 + tempy;
											y2 = y3;
										}
									}
									else
									{ 
										if(tempy > 10*tempx)
										{
											y2 = y3 + tempx;
											x2 = x1;
										}
										else
										{
											y2 = y1 - tempx;
											x2 = x3;
										}
									}
								}
								else if((x1>x3)&&(y1>y3))
								{  
									//         /--------------- x1,y1
									//        /
									// x3,y3 /

									tempx=x1-x3;  tempy=y1-y3;
									if(tempx == tempy){	}
									else if(tempx > tempy)
									{  
										if(tempx > 10*tempy)
										{
											x2 = x3 + tempy;
											y2 = y1;
										}
										else
										{
											x2 = x1 - tempy;
											y2 = y3;
										}
									}
									else
									{ 
										if(tempy > 10*tempx)
										{
											y2 = y3 + tempx;
											x2 = x1;
										}
										else
										{
											y2 = y1 - tempx;
											x2 = x3;
										}
									}
								}
								Get_VPort_XY(&xe,&ye,x2,y2);
								Get_VPort_XY(&xe1,&ye1,x3,y3);
							}
							else
							{
								if((x3>=x1)&&(y3>=y1))
								{
									tempx=x3-x1;  tempy=y3-y1;
									if(tempx>=tempy)
									{
										if(tempx<=(tempy+10))
										{
											x3=x1+tempy;
										}
										else
										{
											y3=y1;
										}
									}
									else
									{
										if(tempy<=(tempx+10))
										{
											y3=y1+tempx;
										}
										else
										{
											x3=x1;
										}
									}
								}
								else if((x1>=x3)&&(y3>=y1))
								{
									tempx=x1-x3;  tempy=y3-y1;
									if(tempx>=tempy)
									{
										if(tempx<=(tempy+10))
										{
											x3=x1-tempy;
										}
										else
										{
											y3=y1;
										}
									}
									else
									{
										if(tempy<=(tempx+10))
										{
											y3=y1+tempx;
										}
										else
										{
											x3=x1;
										}
									}
								}
								else if((x3>=x1)&&(y1>=y3))
								{
									tempx=x3-x1;  tempy=y1-y3;
									if(tempx>=tempy)
									{
										if(tempx<=(tempy+10))
										{
											x3=x1+tempy;
										}
										else
										{
											y3=y1;
										}
									}
									else
									{
										if(tempy<=(tempx+10))
										{
											y3=y1-tempx;
										}
										else
										{
											x3=x1;
										}
									}
								}
								else if((x1>=x3)&&(y1>=y3))
								{
									tempx=x1-x3;  tempy=y1-y3;
									if(tempx>=tempy)
									{
										if(tempx<=(tempy+10))
										{
											x3=x1-tempy;
										}
										else
										{
											y3=y1;
										}
									}
									else
									{
										if(tempy<=(tempx+10))
										{
											y3=y1-tempx;
										}
										else
										{
											x3=x1;
										}
									}
								}
								Get_VPort_XY(&xe1,&ye1,x3,y3);
								x2 = x3; y2 = y3;
								xe = xe1; ye = ye1;
							}
						}

						//setwritemode(1);
						//setcolor(Overwrite_Color);
						xx2=x3;yy2=y3;
						xxe=xe1;yye=ye1;
						drawmode = dc.GetROP2();
						dc.SetROP2(R2_NOTXORPEN);
						oldpen = dc.SelectObject(&OverwritePen);
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ye);
						dc.LineTo(xe1,ye1);
						if(fflag!=0) 
						{
							dc.SelectObject(&RatsPen);
							dc.MoveTo(xxs,yys);
							dc.LineTo(xxe,yye);
							//line(xxs,yys,xxe,yye);
						}
						dc.SelectObject(oldpen);
						dc.SetROP2(drawmode);
					}
				}
				else if((ch1 == PUSHMOUSE)||(ch1 == VK_RETURN))
				{
					if(m_nPCB_Current_Layer>BOTTOMLAYER)
					{
						Error("无法在当前层上布线.");
						continue;
					}
					if((x1==x3)&&(y1==y3)) continue;
					//setwritemode(1);
					//setcolor(Overwrite_Color);
					drawmode = dc.GetROP2();
					dc.SetROP2(R2_NOTXORPEN);
					oldpen = dc.SelectObject(&OverwritePen);
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ye);
					dc.LineTo(xe1,ye1);
					if(fflag!=0) 
					{
						dc.SelectObject(&RatsPen);
						dc.MoveTo(xxs,yys);
						dc.LineTo(xxe,yye);
						//line(xxs,yys,xxe,yye);
					}
					dc.SelectObject(oldpen);
					dc.SetROP2(drawmode);

					// 创建并存储走线带
					gnline.id = 0x10;
					gnline.startx = x1; gnline.starty = y1;
					gnline.endx = x2;   gnline.endy = y2;
					gnline.type = Current_Linetype;
					gnline.layer = (unsigned char)m_nPCB_Current_Layer;
					gnline.linewidth = Current_Linewidth;
					gnline.attrib = PCB;
					gnline.u1 = 0;
					gnline.u2 = 0;
					gnline.flag = 0;
					//------------- Undo -----------------
					Check_Undo_Buffer(20);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_DELETE;
					head.ob = UNDO_LINE;
					head.p = pDoc->m_pLine_Buffer_Temp;
					memcpy(undo_p,&head,6);
					pDoc->m_pPCB_Undo_Current += 6;
					//-------------------------------------
					pDoc->Save_EMMLine(&gnline);
					POINT p;
					int ix,iy;
					Get_VPort_XY(&ix,&iy,m_nCurrent_X,m_nCurrent_Y);
					p.x = ix; p.y = iy;
					DrawMyCursor(&dc,&p);
					DrawStructLine(&gnline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
					DrawMyCursor(&dc,&p);
					// auto place via if layer switched 
					if((PCB_Auto_Via==1)&&(m_nSystem_State==PCB)&&(layerbak != m_nPCB_Current_Layer)&&\
						(layerbak<=BOTTOMLAYER)&&(m_nPCB_Current_Layer<=BOTTOMLAYER)&&(Pad_Exist(x1,y1)!=1))
					{
						gnjunction.id = 0x22;
						gnjunction.centerx = x1; 
						gnjunction.centery = y1;
						gnjunction.Dx = PCB_Current_Via_Size;
						gnjunction.Dy = PCB_Current_Via_Size;
						gnjunction.d  = PCB_Current_Via_HoleSize;
						gnjunction.padtype = 1;
						gnjunction.layer = MULTILAYER;
						gnjunction.attrib = PCB;
						strcpy((char*)gnjunction.padname,Null_Str);
						gnjunction.flag = 0;
						if(Pad_Invalid(&gnjunction)==0)
						{ 
							//---------------- Undo --------------
							undo_p = pDoc->m_pPCB_Undo_Current;
							head.op = UNDO_DELETE;
							head.ob = UNDO_JUNCTION;
							head.p = pDoc->m_pJunction_Buffer_Temp;
							memcpy(undo_p,&head,6);
							pDoc->m_pPCB_Undo_Current += 6;
							//-------------------------------------
							pDoc->Save_EMMJP(&gnjunction);
							DrawStructJP(&gnjunction,PCB_LayerColor[MULTILAYER],0,&dc);
						}
						else
						{ 
						   //Sound();
							int j = AfxMessageBox("与另一个焊盘距离太近，确定?", MB_YESNO|MB_ICONWARNING);
							if( j != IDYES ) {}
							else
							{
								//---------------- Undo --------------
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_JUNCTION;
								head.p = pDoc->m_pJunction_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//-------------------------------------
								pDoc->Save_EMMJP(&gnjunction);
								DrawStructJP(&gnjunction,PCB_LayerColor[MULTILAYER],0,&dc);
							}
						}
					}
					// --------- Undo ----------
					pDoc->m_nPCB_Undo_Num++;
					if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
					pDoc->m_nPCB_Redo_Num=0;
					pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
					// --------------------------------------------------------------------------
					layerbak = m_nPCB_Current_Layer;
					x1 = x2;  y1 = y2;
					x2 = x3;  y2 = y3;
					xs = xe;  ys = ye;
					xe = xe1; ye = ye1;
					flag=0;
					if(fflag==1)
					{
						if((abs(route.ex-x2)<25)&&(abs(route.ey-y2)<25))
						{
							layerbak = m_nPCB_Current_Layer;
							fflag=0;
						}
					}
					else if(fflag==2)
					{
						if((abs(route.sx-x2)<25)&&(abs(route.sy-y2)<25))
						{
							layerbak = m_nPCB_Current_Layer;
							fflag=0;
						}
					}
		
					if(m_nPCB_Current_Layer <= BOTTOMLAYER) ratsflag = 1;
					else ratsflag = 0;
					if((fflag==0)&&(ratsflag != 0))
					{
						temp_ratsnest=route_ratsnest;
						for(i=0;i<route_ratsnum;i++)
						{
							memcpy(&route,temp_ratsnest,13);
							if((abs(route.sx-x1)<25)&&(abs(route.sy-y1)<25)&&(route.state==0))
							{
								xx1=route.ex;
								yy1=route.ey;
								xx2=x3; yy2=y3;
								Get_VPort_XY(&xxs,&yys,xx1,yy1);
								xxe=xe1; yye=ye1;
								fflag=1;
								CloseMouseCursor();
								DrawAnyLine(route.sx,route.sy,route.ex,route.ey,Overwrite_Color,1,&dc);
								route.state = 1;
								memcpy(temp_ratsnest,&route,13);
								layerbak = m_nPCB_Current_Layer;
								break;
							}
							else if((abs(route.ex-x1)<25)&&(abs(route.ey-y1)<25)&&(route.state==0))
							{
								xx1=route.sx;
								yy1=route.sy;
								xx2=x3; yy2=y3;
								Get_VPort_XY(&xxs,&yys,xx1,yy1);
								xxe=xe1; yye=ye1;
								fflag=2;
								DrawAnyLine(route.sx,route.sy,route.ex,route.ey,Overwrite_Color,1,&dc);
								route.state = 1;
								memcpy(temp_ratsnest,&route,13);
								layerbak = m_nPCB_Current_Layer;
								break;
							}
							temp_ratsnest+=13;
						}
					}
					xx2=x3; yy2=y3;
					xxe=xe1; yye=ye1;
					ch1 = MOUSEMOVE;
					goto TEMP;
					//continue;
				}
				else Command_In_Function(ch1);
			}while(1);
		}
		else	Command_In_Function(ch1);
    }while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));
	
	//Mouse_Free();
	CL_Auto_Pan=OFF;
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	DrawStructButton(ID_EDIT_PLACE_TRACK,FALSE);
	Write_Help(commandend);
}

/*void CDesignView::OnRouteManuel() 
{
	int i;
	int layerbak;
	unsigned ch1;
	unsigned x1,y1,x2,y2,tempx,tempy;
	int xs,ys,xe,ye,flag;
	unsigned xx1,yy1,xx2,yy2,fflag;
	int xxs,yys,xxe,yye;
	struct Route_Rats route;
	char *route_ratsnest, *temp_ratsnest;
	int route_ratsnum=0;
	struct Undo_Head head;
	char *undo_p;

	CClientDC dc(this);
	int drawmode = dc.GetROP2();
	CPen OverwritePen, *oldpen;
	OverwritePen.CreatePen(PS_SOLID,1,Overwrite_Color);

	CDesignDoc* pDoc = (CDesignDoc*)GetDocument();

	pDoc->SetModifiedFlag(TRUE);


	if(pDoc->m_nNet_Number==0) 
	{
		Error( error16 );
	    return;
	}

	if((route_ratsnest = (char*)malloc(130000))==NULL)
	{
		memory_error();
		return;
	}
	if(Ratsnestflag!=0) route_ratsnum = pDoc->Get_All_Ratsnest(route_ratsnest);

	CL_Auto_Pan = ON;
	DrawStructButton(ID_VIEW_ZOOM_PAN,TRUE);
	Write_Help(helpstr3);
	flag = 0;
	fflag = 0;
	CL_RedrawFlag = 0;
	do
	{
		ch1 = MouseKey();
		if((ch1==VK_RETURN)||(ch1==PUSHMOUSE))
		{
			layerbak = m_nPCB_Current_Layer;
			x1 = m_nCurrent_X;
			y1 = m_nCurrent_Y;
			x2 = x1;
			y2 = y1;
			Get_VPort_XY(&xs,&ys,x1,y1);
			xe = xs;	
			ye = ys;
			CloseMouseCursor();
			Write_Help(helpstr4);
			OpenMouseCursor();
			temp_ratsnest=route_ratsnest;
			flag=0;
			fflag=0;
			for(i=0;i<route_ratsnum;i++)
			{
				memcpy(&route,temp_ratsnest,13);
				if((abs(route.sx-x1)<25)&&(abs(route.sy-y1)<25)&&(route.state==0))
				{
					xx1 = route.ex;
					yy1 = route.ey;
					xx2 = x2; 
					yy2 = y2;
					Get_VPort_XY(&xxs,&yys,xx1,yy1);
					xxe = xe; yye = ye;
					fflag=1;
					CloseMouseCursor();
					DrawAnyLine(route.sx,route.sy,route.ex,route.ey,Overwrite_Color,1,&dc);
					OpenMouseCursor();
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
					break;
				 }
				else if((abs(route.ex-x1)<25)&&(abs(route.ey-y1)<25)&&(route.state==0))
				{
					xx1=route.sx;
					yy1=route.sy;
					xx2=x2; yy2=y2;
					Get_VPort_XY(&xxs,&yys,xx1,yy1);
					xxe=xe; yye=ye;
					fflag=2;
					CloseMouseCursor();
					DrawAnyLine(route.sx,route.sy,route.ex,route.ey,Overwrite_Color,1,&dc);
					OpenMouseCursor();
					route.state=1;
					memcpy(temp_ratsnest,&route,13);
					break;
				}
				temp_ratsnest+=13;
			}
			do
			{
				ch1 = MouseKey();
				Get_VPort_XY(&xs,&ys,x1,y1);
				Get_VPort_XY(&xxs,&yys,xx1,yy1);
				if((ch1 == VK_ESCAPE)||(ch1 == PUSHMOUSEESC))
				{
					Write_Help(helpstr3);
					CloseMouseCursor();
					//setwritemode(1);
					//setcolor(Overwrite_Color);
					dc.SetROP2(R2_NOTXORPEN);
					oldpen = dc.SelectObject(&OverwritePen);
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ye);
					//line(xs,ys,xe,ye);
					if(fflag!=0) 
					{
						dc.MoveTo(xxs,yys);
						dc.LineTo(xxe,yye);
						//line(xxs,yys,xxe,yye);
					}
					dc.SelectObject(oldpen);
					dc.SetROP2(drawmode);
					OpenMouseCursor();
					ch1=MOUSEMOVE;
					break;
				}
				else if(ch1==MOUSEMOVE)
				{
					if(flag==0)
					{
						flag=1;
						CloseMouseCursor();
						//setwritemode(1);
						//setcolor(Overwrite_Color);
						//line(xs,ys,xe,ye);
						//if(fflag!=0)  line(xxs,yys,xxe,yye);
						dc.SetROP2(R2_NOTXORPEN);
						oldpen = dc.SelectObject(&OverwritePen);
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ye);
						//line(xs,ys,xe,ye);
						if(fflag!=0) 
						{
							dc.MoveTo(xxs,yys);
							dc.LineTo(xxe,yye);
							//line(xxs,yys,xxe,yye);
						}
						dc.SelectObject(oldpen);
						dc.SetROP2(drawmode);
						OpenMouseCursor();
					}
					else
					{
						if(CL_RedrawFlag==1) CL_RedrawFlag=0;
						else 
						{
							CloseMouseCursor();
							//setwritemode(1);
							//setcolor(Overwrite_Color);
							//line(xs,ys,xe,ye);
							//if(fflag!=0) line(xxs,yys,xxe,yye);
							dc.SetROP2(R2_NOTXORPEN);
							oldpen = dc.SelectObject(&OverwritePen);
							dc.MoveTo(xs,ys);
							dc.LineTo(xe,ye);
							//line(xs,ys,xe,ye);
							if(fflag!=0) 
							{
								dc.MoveTo(xxs,yys);
								dc.LineTo(xxe,yye);
								//line(xxs,yys,xxe,yye);
							}
							dc.SelectObject(oldpen);
							dc.SetROP2(drawmode);
							OpenMouseCursor();
						}
						x2 = m_nCurrent_X;
						y2 = m_nCurrent_Y;
						Get_VPort_XY(&xe,&ye,x2,y2);
						if(CL_Ortho_Mode==ON)
						{
							if((x2>=x1)&&(y2>=y1))
							{
								tempx=x2-x1;  tempy=y2-y1;
								if(tempx>=tempy)
								{
									if(tempx<=(tempy+10))
									{
										x2=x1+tempy;
									}
									else
									{
										y2=y1;
									}
								}
								else
								{
									if(tempy<=(tempx+10))
									{
										y2=y1+tempx;
									}
									else
									{
										x2=x1;
									}
								}
							}
							else if((x1>=x2)&&(y2>=y1))
							{
								tempx=x1-x2;  tempy=y2-y1;
								if(tempx>=tempy)
								{
									if(tempx<=(tempy+10))
									{
										x2=x1-tempy;
									}
									else
									{
										y2=y1;
									}
								}
								else
								{
									if(tempy<=(tempx+10))
									{
										y2=y1+tempx;
									}
									else
									{
										x2=x1;
									}
								}
							}
							else if((x2>=x1)&&(y1>=y2))
							{
								tempx=x2-x1;  tempy=y1-y2;
								if(tempx>=tempy)
								{
									if(tempx<=(tempy+10))
									{
										x2=x1+tempy;
									}
									else
									{
										y2=y1;
									}
								}
								else
								{
									if(tempy<=(tempx+10))
									{
										y2=y1-tempx;
									}
									else
									{
										x2=x1;
									}
								}
							}
							else if((x1>=x2)&&(y1>=y2))
							{
								tempx=x1-x2;  tempy=y1-y2;
								if(tempx>=tempy)
								{
									if(tempx<=(tempy+10))
									{
										x2=x1-tempy;
									}
									else
									{
										y2=y1;
									}
								}
								else
								{
									if(tempy<=(tempx+10))
									{
										y2=y1-tempx;
									}
									else
									{
										x2=x1;
									}
								}
							}
							Get_VPort_XY(&xe,&ye,x2,y2);
						}
						xx2=x2;yy2=y2;
						xxe=xe;yye=ye;
						CloseMouseCursor();
						//setwritemode(1);
						//setcolor(Overwrite_Color);
						//line(xs,ys,xe,ye);
						//if(fflag!=0)  line(xxs,yys,xxe,yye);
						dc.SetROP2(R2_NOTXORPEN);
						oldpen = dc.SelectObject(&OverwritePen);
						dc.MoveTo(xs,ys);
						dc.LineTo(xe,ye);
						//line(xs,ys,xe,ye);
						if(fflag!=0) 
						{
							dc.MoveTo(xxs,yys);
							dc.LineTo(xxe,yye);
							//line(xxs,yys,xxe,yye);
						}
						dc.SelectObject(oldpen);
						dc.SetROP2(drawmode);
						OpenMouseCursor();
					}
				}
				else if((ch1==PUSHMOUSE)||(ch1==VK_RETURN))
				{
					if(m_nPCB_Current_Layer>BOTTOMLAYER)
					{
						Error("不能在当前层布线!");
						continue;
					}
					if((x1==x2)&&(y1==y2)) continue;
					if((layerbak != m_nPCB_Current_Layer)&&(PCB_Auto_Via == 1)&&(Pad_Exist(x1,y1)!=1))
					{
						gnjunction.id = 0x22;
						gnjunction.centerx = x1;
						gnjunction.centery = y1;
						gnjunction.Dx = PCB_Current_Via_Size;
						gnjunction.Dy = PCB_Current_Via_Size;
						gnjunction.d  = PCB_Current_Via_HoleSize;
						gnjunction.padtype = 1;
						gnjunction.layer = MULTILAYER;
						gnjunction.attrib = PCB;
						strcpy((char*)gnjunction.padname,Null_Str);
						gnjunction.flag = 0;
						if(Pad_Invalid(&gnjunction)==0)
						{ 
							//---------------- Undo --------------
							undo_p = pDoc->m_pPCB_Undo_Current;
							head.op = UNDO_DELETE;
							head.ob = UNDO_JUNCTION;
							head.p = pDoc->m_pJunction_Buffer_Temp;
							memcpy(undo_p,&head,6);
							pDoc->m_pPCB_Undo_Current += 6;
							//-------------------------------------
							pDoc->Save_EMMJP(&gnjunction);
							DrawStructJP(&gnjunction,PCB_LayerColor[MULTILAYER],0,&dc);
						}
						else
						{ 
						   //Sound();
							int j = MessageBox("焊盘或过孔距离太近! 是否允许?",worningop,MB_YESNO|MB_ICONWARNING);
							if( j != IDYES ) {}
							else
							{
								//---------------- Undo --------------
								undo_p = pDoc->m_pPCB_Undo_Current;
								head.op = UNDO_DELETE;
								head.ob = UNDO_JUNCTION;
								head.p = pDoc->m_pJunction_Buffer_Temp;
								memcpy(undo_p,&head,6);
								pDoc->m_pPCB_Undo_Current += 6;
								//-------------------------------------
								pDoc->Save_EMMJP(&gnjunction);
								DrawStructJP(&gnjunction,PCB_LayerColor[MULTILAYER],0,&dc);
							}
						}
					}
					layerbak = m_nPCB_Current_Layer;
					CloseMouseCursor();
					//setwritemode(1);
					//setcolor(Overwrite_Color);
					//line(xs,ys,xe,ye);
					//if(fflag!=0) line(xxs,yys,xxe,yye);
					dc.SetROP2(R2_NOTXORPEN);
					oldpen = dc.SelectObject(&OverwritePen);
					dc.MoveTo(xs,ys);
					dc.LineTo(xe,ye);
					//line(xs,ys,xe,ye);
					if(fflag!=0) 
					{
						dc.MoveTo(xxs,yys);
						dc.LineTo(xxe,yye);
						//line(xxs,yys,xxe,yye);
					}
					dc.SelectObject(oldpen);
					dc.SetROP2(drawmode);
					gnline.id = 0x10;
					gnline.startx = x1; 
					gnline.starty = y1;
					gnline.endx = x2;
					gnline.endy = y2;
					gnline.type = (unsigned char)Current_Linetype;
					gnline.layer = (unsigned char)m_nPCB_Current_Layer;
					gnline.linewidth = Current_Linewidth;
					gnline.attrib = m_nSystem_State;
					gnline.u1 = 0;
					gnline.u2 = 0;
					gnline.flag = 0;
					//------------- Undo -----------------
					Check_Undo_Buffer(20);
					undo_p = pDoc->m_pPCB_Undo_Current;
					head.op = UNDO_DELETE;
					head.ob = UNDO_LINE;
					head.p = pDoc->m_pLine_Buffer_Temp;
					memcpy(undo_p,&head,6);
					pDoc->m_pPCB_Undo_Current += 6;
					//-------------------------------------
					pDoc->Save_EMMLine(&gnline);
					DrawStructLine(&gnline,PCB_LayerColor[m_nPCB_Current_Layer],&dc);
					OpenMouseCursor();
					// --------- Undo ----------
					pDoc->m_nPCB_Undo_Num++;
					if(pDoc->m_nPCB_Undo_Num >= UNDO_NUM_LIMMIT) Undo_Buffer_Tran();
					pDoc->m_nPCB_Redo_Num=0;
					pDoc->m_pPCB_Undo_Command[pDoc->m_nPCB_Undo_Num] = pDoc->m_pPCB_Undo_Current;
					// --------------------------------------------------------------------------
					x1=x2;y1=y2;
					xs=xe;ys=ye;
					flag=0;
					if(fflag==1)
					{
						if((abs(route.ex-x2)<25)&&(abs(route.ey-y2)<25))
						{
							layerbak = m_nPCB_Current_Layer;
							fflag=0;
						}
					}
					else if(fflag==2)
					{
						if((abs(route.sx-x2)<25)&&(abs(route.sy-y2)<25))
						{
							layerbak = m_nPCB_Current_Layer;
							fflag=0;
						}
					}
					if(fflag==0)
					{
						temp_ratsnest=route_ratsnest;
						for(i=0;i<route_ratsnum;i++)
						{
							memcpy(&route,temp_ratsnest,13);
							if((abs(route.sx-x1)<25)&&(abs(route.sy-y1)<25)&&(route.state==0))
							{
								xx1=route.ex;
								yy1=route.ey;
								xx2=x2; yy2=y2;
								Get_VPort_XY(&xxs,&yys,xx1,yy1);
								xxe=xe; yye=ye;
								fflag=1;
								CloseMouseCursor();
								DrawAnyLine(route.sx,route.sy,route.ex,route.ey,Overwrite_Color,1,&dc);
								route.state = 1;
								memcpy(temp_ratsnest,&route,13);
								layerbak = m_nPCB_Current_Layer;
								break;
							}
							else if((abs(route.ex-x1)<25)&&(abs(route.ey-y1)<25)&&(route.state==0))
							{
								xx1=route.sx;
								yy1=route.sy;
								xx2=x2; yy2=y2;
								Get_VPort_XY(&xxs,&yys,xx1,yy1);
								xxe=xe; yye=ye;
								fflag=2;
								DrawAnyLine(route.sx,route.sy,route.ex,route.ey,Overwrite_Color,1,&dc);
								route.state = 1;
								memcpy(temp_ratsnest,&route,13);
								layerbak = m_nPCB_Current_Layer;
								break;
							}
							temp_ratsnest+=13;
						}
					}
					xx2=x2; yy2=y2;
					xxe=xe; yye=ye;
					continue;
				}
				else Command_In_Function(ch1);
			}while(1);
		}
		else Command_In_Function(ch1);
	}while((ch1 != VK_ESCAPE)&&(ch1 != PUSHMOUSEESC ));

	CL_Auto_Pan = OFF;
	CloseMouseCursor();
	DrawStructButton(ID_VIEW_ZOOM_PAN,FALSE);
	OpenMouseCursor();
	free(route_ratsnest);
	Write_Help(commandend);
}*/

int CDesignView::Route_Swap_H_V(char * padbuf1, char * trackbuf1,\
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

	if((routebuf = (char*)malloc(13000)) == NULL)
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
				//if(route.length <= (length+200))
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
			if(Route_Hor(&route)!=0)
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
			if(Route_Ver(&route)!=0)
			{
				free(routebuf);
				return 1;
			}
		}
	}
	free(routebuf);
	return 0;
}


int CDesignView::Route_Swap_L(char * padbuf1, char * trackbuf1,\
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
				//if(route.length <= (length+200))
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
		if(Route_L(&route)!=0)
		{
			free(routebuf);
			return 1;
		}
	}
	free(routebuf);
	return 0;
}


int CDesignView::Route_Swap_Z(char * padbuf1, char * trackbuf1,\
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
				//if(route.length <= (length+200))
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
		if(Route_Z(&route,0)!=0)
		{
			free(routebuf);
			return 1;
		}
	}
	free(routebuf);
	return 0;
}


int CDesignView::Route_Swap_C(char * padbuf1, char * trackbuf1,\
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
				//if(route.length <= (length+200))
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
		if(Route_C(&route)!=0)
		{
			free(routebuf);
			return 1;
		}
	}
	free(routebuf);
	return 0;
}


int CDesignView::Route_Swap_SuperZC(char * padbuf1, char * trackbuf1, \
									char * padbuf2, char * trackbuf2, int length)
{
	int i,ii,j, routenum, judge;
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
				//if(route.length <= (length+200))
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
		j =Super_C(&route);
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
		j =Super_Z(&route);
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
