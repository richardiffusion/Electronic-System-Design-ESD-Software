#include "stdafx.h"
#include "stdlib.h"
#include "string.h"
#include "resource.h"
#include "sysparam.h"
#include "systemext.h"
#include "function.h"

#include "stdio.h"
#include "string.h"
#include "math.h"
#include "stdlib.h"
#include "dos.h"

void ff_to_str(int x, char *string)
{
    int ndig=3;
    int dec,sign;
    char xx[10],*str;
    double fx;

	fx = (float)x/100000;

    str = _fcvt(fx,ndig,&dec,&sign);
		 if(dec==-3){ xx[0]='0';	xx[1]='.';	  xx[2]='0';	xx[3]='0';	  xx[4]='0';    xx[5]='\0';}
    else if(dec==-2){ xx[0]='0';	xx[1]='.';	  xx[2]='0';	xx[3]='0';	  xx[4]=str[0]; xx[5]='\0';}
    else if(dec==-1){ xx[0]='0';	xx[1]='.';	  xx[2]='0';	xx[3]=str[0]; xx[4]=str[1]; xx[5]='\0';}
    else if(dec==0) { xx[0]='0';	xx[1]='.';	  xx[2]=str[0];	xx[3]=str[1]; xx[4]=str[2]; xx[5]='\0';}
    else if(dec==1) { xx[0]=str[0]; xx[1]='.';	  xx[2]=str[1]; xx[3]=str[2]; xx[4]=str[3]; xx[5]='\0';}
    else if(dec==2) { xx[0]=str[0]; xx[1]=str[1]; xx[2]='.';    xx[3]=str[2]; xx[4]=str[3]; xx[5]=str[4]; xx[6]='\0';}
    else if(dec==3) { xx[0]=str[0]; xx[1]=str[1]; xx[2]=str[2]; xx[3]='.';    xx[4]=str[3]; xx[5]=str[4]; xx[6]=str[5]; xx[7]='\0'; }
    else if(dec==4) { xx[0]=str[0]; xx[1]=str[1]; xx[2]=str[2]; xx[3]=str[3]; xx[4]='.';    xx[5]=str[4]; xx[6]=str[5]; xx[7]=str[6];  xx[8]='\0'; }
    strcpy(string,xx);
}

void mil_to_str(int x, char *string)
{
    int ndig=2;
    int dec,sign;
    char xx[10],*str;
    double fx;

	fx = (float)x/2540;
    str = _fcvt(fx,ndig,&dec,&sign);
         if(dec==-2){ xx[0]='0';	xx[1]='.';	  xx[2]='0';	xx[3]='0';	  xx[4]='\0';}
    else if(dec==-1){ xx[0]='0';	xx[1]='.';	  xx[2]='0';	xx[3]=str[0]; xx[4]='\0';}
    else if(dec==0) { xx[0]='0';	xx[1]='.';	  xx[2]=str[0];	xx[3]=str[1]; xx[4]='\0';}
    else if(dec==1) { xx[0]=str[0]; xx[1]='.';	  xx[2]=str[1]; xx[3]=str[2]; xx[4]='\0';}
    else if(dec==2) { xx[0]=str[0]; xx[1]=str[1]; xx[2]='.';    xx[3]=str[2]; xx[4]=str[3]; xx[5]='\0';}
    else if(dec==3) { xx[0]=str[0]; xx[1]=str[1]; xx[2]=str[2]; xx[3]='.';    xx[4]=str[3]; xx[5]=str[4]; xx[6]='\0'; }
    else if(dec==4) { xx[0]=str[0]; xx[1]=str[1]; xx[2]=str[2]; xx[3]=str[3]; xx[4]='.';    xx[5]=str[4]; xx[6]=str[5]; xx[7]='\0'; }
	else if(dec==5) { xx[0]=str[0]; xx[1]=str[1]; xx[2]=str[2]; xx[3]=str[3]; xx[4]=str[4]; xx[5]='.';    xx[6]=str[5]; xx[7]=str[6]; xx[8]='\0';}
    strcpy(string,xx);
}


int Divide(char *string, char s123[10][40])
{ 
	int i=0,j=0,k=0,flag;
	flag=1;
	for(;;)
	{
		if(string[i]=='\0') 
		{
			s123[j][k]=0; 
			j++; 
			break; 
		}
		if((string[i]==0x0d)||(string[i]==0x0a))
		{
			i++;
			continue;
		}
		if((string[i]==0x20)||(string[i] == 0x09))
		{
			if(flag==0) 
			{
				flag=1;
				s123[j][k]=0;
				j++;
				if(j >= 10) break;
				k=0;
			}
			else if(flag==1)
			{
			}
			i++;
			continue;
		}
		else
		{
			flag = 0;
			s123[j][k] = string[i];
			i++; 
			k++;
		}
	}
	return j;
}

/*int Divide_l(char *string, char s123[20][200])
{ 
	int i=0,j=0,k=0,flag;
	flag=1;
	for(;;)
	{
		if(string[i]=='\0') 
		{
			s123[j][k]=0; 
			j++; 
			break; 
		}
		if((string[i]==0x0d)||(string[i]==0x0a))
		{
			i++;
			continue;
		}
		if((string[i]==0x20)||(string[i] == 0x09))
		{
			if(flag==0) 
			{
				flag=1;
				s123[j][k]=0;
				j++;
				if(j >= 20) break;
				k=0;
			}
			else if(flag==1)
			{
			}
			i++;
			continue;
		}
		else
		{
			flag = 0;
			s123[j][k] = string[i];
			i++; 
			k++;
		}
	}
	return j;
}

int Divide_long(char *string, char s123[200][40])
{ 
	int i=0,j=0,k=0,flag, sw;
	flag=1;
	sw = 0;
	for(;;)
	{
		if(string[i]=='\0') 
		{
			s123[j][k]=0; 
			j++; 
			break; 
		}
		if((string[i]==0x0d)||(string[i]==0x0a))
		{
			i++;
			continue;
		}
		if(((string[i]==0x20)||(string[i] == 0x09))&&(sw == 0))
		{
			if(flag==0) 
			{
				flag=1;
				s123[j][k]=0;
				j++;
				if(j >= 200) break;
				k=0;
			}
			else if(flag==1)
			{
			}
			i++;
			continue;
		}
		else
		{
			if(string[i] == 0x27) sw = (sw+1)&0x01;
			flag = 0;
			s123[j][k] = string[i];
			i++; 
			k++;
		}
	}
	return j;
}

int Divide_Recode(char *string, char s123[100][100])
{ 
	int i=0,j=0,k=0,flag;
	flag=1;
	for(;;)
	{
		if(string[i]=='\0') 
		{
			s123[j][k]=0; 
			j++; 
			break; 
		}
		if((string[i]==0x0d)||(string[i]==0x0a))
		{
			i++;
			continue;
		}
		if(string[i] == 0x7C)
		{
			if(flag==0) 
			{
				flag=1;
				s123[j][k]=0;
				j++;
				if(j >= 100) break;
				k=0;
			}
			else if(flag==1)
			{
			}
			i++;
			continue;
		}
		else
		{
			flag = 0;
			s123[j][k] = string[i];
			i++; 
			k++;
		}
	}
	return j;
}

int Divide_Track(char *string, char *s1, char *s2)
{ 
	int i,j,k,flag;

	j = 0;
	k = 0;
	flag = 0;
	for(i=0; i<strlen(string); i++)
	{
		if((string[i] == '=')&&(flag == 0)) flag = 1;
		else
		{
			if(flag == 0) 
			{
				s1[j] = string[i];
				j++;
			}
			else
			{
				s2[k] = string[i];
				k++;
			}
		}
	}
	s1[j] = '\0';
	s2[k] = '\0';
	return 0;
}*/

int partstrcmp( char *libstr, char *compstr)
{
	int llib,lcomp,k;
	llib=strlen(libstr);
	lcomp=strlen(compstr);
	k=0;
	for(;;)
	{
		if((llib-k)<lcomp) return 1;
		else if(memcmp((libstr+k),compstr,lcomp)==0)  return 0;
		else k++;
	}
}

Stringinvalid(int x,int y,int h,int num, int alpha)
{
	int xx,yy;

		 if((alpha ==  0)||(alpha == 152)) xx = x + h*num/2;
	else if((alpha == 90)||(alpha == 242)) xx = x - h;
	else if((alpha ==166)||(alpha == 270)) xx = x + h;
	else if((alpha == 76)||(alpha == 180)) xx = x - h*num/2;
	if(nEDA_System_State==SCH)
    {
		     if((alpha == 0)||(alpha == 76)) yy = y - h;
		else if((alpha==152)||(alpha ==180)) yy = y + h;
		else if((alpha ==90)||(alpha ==166)) yy = y - h*num/2;
		else if((alpha==242)||(alpha ==270)) yy = y + h*num/2;
		if((xx > nEDA_Size_Maxx)||(yy > nEDA_Size_Maxy)) return 1;
		else return 0;
    }
	else
    {
		     if((alpha == 0)||(alpha == 76)) yy = y + h;
		else if((alpha==152)||(alpha ==180)) yy = y - h;
		else if((alpha ==90)||(alpha ==166)) yy = y + h*num/2;
		else if((alpha==242)||(alpha ==270)) yy = y - h*num/2;
		if((xx > nEDA_Size_Maxx)||(yy > nEDA_Size_Maxy)) return 1;
		else return 0;
    }
}

/*Dist_P_P(unsigned x1,unsigned y1,unsigned x2,unsigned y2,int d)
{ 
	unsigned temp;
  if( abs((int)(x1-x2))>d) return CONNECT_NO;
  if( abs((int)(y1-y2))>d) return CONNECT_NO;
  temp = sqrt(((float)x1-x2)*((float)x1-x2)+((float)y1-y2)*((float)y1-y2));
  if(temp<=d) return CONNECT_YES;
  else return CONNECT_NO;
}

Dist_P_Line(unsigned x1,unsigned y1,unsigned xs,unsigned ys,unsigned xe,unsigned ye,int d)
{ unsigned temp;
  float m,f;
  unsigned tempx,tempy;
  unsigned tmaxx,tminx,tmaxy,tminy;

  if((xe==xs)&&(ye==ys)) return Dist_P_P(x1,y1,xs,ys,d);

  tmaxx=MAX(xs,xe);
  tminx=MIN(xs,xe);
  tmaxy=MAX(ys,ye);
  tminy=MIN(ys,ye);
  if((x1<tminx)&&(abs((int)(x1-tminx))>d)) return CONNECT_NO;
  if((x1>tmaxx)&&(abs((int)(x1-tmaxx))>d)) return CONNECT_NO;
  if((y1<tminy)&&(abs((int)(y1-tminy))>d)) return CONNECT_NO;
  if((y1>tmaxy)&&(abs((int)(y1-tmaxy))>d)) return CONNECT_NO;

  if(xe==xs)
   {  if(((y1>=ys)&&(y1<=ye))||((y1>=ye)&&(y1<=ys)))
	{ if(xs>=x1) temp=xs-x1;
	  else temp=x1-xs;
	  if(temp<=d) return CONNECT_YES;
	  else return CONNECT_NO;
	}
      else {  if(ys>ye)
		{ if(y1>ys)
		   { if((abs((int)(x1-xs))>d)||(abs((int)(y1-ys))>d)) return CONNECT_NO;
		     temp = sqrt(((float)x1-xs)*((float)x1-xs)+((float)y1-ys)*((float)y1-ys));
		   }
		  else
		   { if((abs((int)(x1-xe))>d)||(abs((int)(y1-ye))>d)) return CONNECT_NO;
		     temp = sqrt(((float)x1-xe)*((float)x1-xe)+((float)y1-ye)*((float)y1-ye));
		   }
		}
	      else
		{ if(y1<ys)
		   { if((abs((int)(x1-xs))>d)||(abs((int)(y1-ys))>d)) return CONNECT_NO;
		     temp = sqrt(((float)x1-xs)*((float)x1-xs)+((float)y1-ys)*((float)y1-ys));
		   }
		  else
		   { if((abs((int)(x1-xe))>d)||(abs((int)(y1-ye))>d)) return CONNECT_NO;
		     temp = sqrt(((float)x1-xe)*((float)x1-xe)+((float)y1-ye)*((float)y1-ye));
		   }
		}
	      if(temp<=d) return CONNECT_YES;
	      else return CONNECT_NO;
	   }
   }
  else if(ye==ys)
   {  if(((x1>=xs)&&(x1<=xe))||((x1>=xe)&&(x1<=xs)))
	{ if(ys>=y1) temp=ys-y1;
	  else temp=y1-ys;
	  if(temp<=d) return CONNECT_YES;
	  else return CONNECT_NO;
	}
      else {  if(xs>=xe)
		{ if(x1>xs)
		   { if((abs((int)(x1-xs))>d)||(abs((int)(y1-ys))>d)) return CONNECT_NO;
		     temp = sqrt(((float)x1-xs)*((float)x1-xs)+((float)y1-ys)*((float)y1-ys));
		   }
		  else
		   { if((abs((int)(x1-xe))>d)||(abs((int)(y1-ye))>d)) return CONNECT_NO;
		     temp = sqrt(((float)x1-xe)*((float)x1-xe)+((float)y1-ye)*((float)y1-ye));
		   }
		}
	      else
		{ if(x1<xs)
		   { if((abs((int)(x1-xs))>d)||(abs((int)(y1-ys))>d)) return CONNECT_NO;
		     temp = sqrt(((float)x1-xs)*((float)x1-xs)+((float)y1-ys)*((float)y1-ys));
		   }
		  else
		   { if((abs((int)(x1-xe))>d)||(abs((int)(y1-ye))>d)) return CONNECT_NO;
		     temp = sqrt(((float)x1-xe)*((float)x1-xe)+((float)y1-ye)*((float)y1-ye));
		   }
		}
	      if(temp<=d) return CONNECT_YES;
	      else return CONNECT_NO;
	   }
   }
  else
   {   m = ((float)ye-ys)/((float)xe-xs);
       f=(m*m*xs+x1-m*ys+m*y1)/(m*m+1);
       tempx=f;
       tempy=m*(f-xs)+ys;
       if(((tempx>=xs)&&(tempx<=xe)&&(tempy>=ys)&&(tempy<=ye))||\
	  ((tempx>=xs)&&(tempx<=xe)&&(tempy>=ye)&&(tempy<=ys))||\
	  ((tempx>=xe)&&(tempx<=xs)&&(tempy>=ys)&&(tempy<=ye))||\
	  ((tempx>=xe)&&(tempx<=xs)&&(tempy>=ye)&&(tempy<=ys)))
	     { if((abs((int)(x1-tempx))>d)||(abs((int)(y1-tempy))>d)) return CONNECT_NO;
	       temp=sqrt(((float)x1-tempx)*((float)x1-tempx)+((float)y1-tempy)*((float)y1-tempy));
	       if(temp<=d) return CONNECT_YES;
	       else return CONNECT_NO;
	     }
      else
       {
	  temp=sqrt(((float)x1-xs)*((float)x1-xs)+((float)y1-ys)*((float)y1-ys));
	  if(temp<=d) return CONNECT_YES;
	  temp=sqrt(((float)x1-xe)*((float)x1-xe)+((float)y1-ye)*((float)y1-ye));
	  if(temp<=d) return CONNECT_YES;
	  else return CONNECT_NO;
       }
   }
}

Dist_Line_Line(unsigned x1s,unsigned y1s,unsigned x1e,unsigned y1e,\
	       unsigned x2s,unsigned y2s,unsigned x2e,unsigned y2e,int d)
{  short unsigned temp;
   short unsigned tempd[8];
   float k1,k2;
   short unsigned tempx,tempy;
   //int i;


   if(((x1s==x2s)&&(y1s==y2s))||((x1e==x2s)&&(y1e==y2s))||\
      ((x1s==x2e)&&(y1s==y2e))||((x1e==x2e)&&(y1e==y2e))   )
       {  return CONNECT_YES;
       }
   if((x1s==x1e)&&(x2s==x2e))
    {
      if((((y1s>=y2s)&&(y1s<=y2e))||((y1s>=y2e)&&(y1s<=y2s)))||
	 (((y1e>=y2s)&&(y1e<=y2e))||((y1e>=y2e)&&(y1e<=y2s))))
	   {  if(x1s>=x2s) temp=x1s-x2s;
	      else temp=x2s-x1s;
	   }
      else if((((y2s>=y1s)&&(y2s<=y1e))||((y2s>=y1e)&&(y2s<=y1s)))||
	      (((y2e>=y1s)&&(y2e<=y1e))||((y2e>=y1e)&&(y2e<=y1s))))
	   {  if(x2s>=x1s) temp=x2s-x1s;
	      else temp=x1s-x2s;
	   }
      else { if(y1s>=y2s)
	      {  if((y1s>=y1e)&&(y2s>=y2e))
		   { if((abs((int)(x1e-x2s))>d)||(abs((int)(y1e-y2s))>d)) return CONNECT_NO;
		     temp=sqrt(((float)x1e-x2s)*((float)x1e-x2s)+((float)y1e-y2s)*((float)y1e-y2s));
		   }
		 else if((y1s>=y1e)&&(y2e>=y2s))
		   { if((abs((int)(x1e-x2e))>d)||(abs((int)(y1e-y2e))>d)) return CONNECT_NO;
		     temp=sqrt(((float)x1e-x2e)*((float)x1e-x2e)+((float)y1e-y2e)*((float)y1e-y2e));
		   }
		 else if((y1e>=y1s)&&(y2s>=y2e))
		   { if((abs((int)(x1s-x2s))>d)||(abs((int)(y1s-y2s))>d)) return CONNECT_NO;
		     temp=sqrt(((float)x1s-x2s)*((float)x1s-x2s)+((float)y1s-y2s)*((float)y1s-y2s));
		   }
		 else
		   { if((abs((int)(x1s-x2e))>d)||(abs((int)(y1s-y2e))>d)) return CONNECT_NO;
		     temp=sqrt(((float)x1s-x2e)*((float)x1s-x2e)+((float)y1s-y2e)*((float)y1s-y2e));
		   }
	      }
	     else
	      {  if((y2e>=y2s)&&(y1e>=y1s))
		   { if((abs((int)(x1e-x2s))>d)||(abs((int)(y1e-y2s))>d)) return CONNECT_NO;
		     temp=sqrt(((float)x1e-x2s)*((float)x1e-x2s)+((float)y1e-y2s)*((float)y1e-y2s));
		   }
		 else if((y2s>=y2e)&&(y1e>=y1s))
		   {  if((abs((int)(x1e-x2e))>d)||(abs((int)(y1e-y2e))>d)) return CONNECT_NO;
		     temp=sqrt(((float)x1e-x2e)*((float)x1e-x2e)+((float)y1e-y2e)*((float)y1e-y2e));
		   }
		 else if((y2e>=y2s)&&(y1s>=y1e))
		   { if((abs((int)(x1s-x2s))>d)||(abs((int)(y1s-y2s))>d)) return CONNECT_NO;
		     temp=sqrt(((float)x1s-x2s)*((float)x1s-x2s)+((float)y1s-y2s)*((float)y1s-y2s));
		   }
		 else
		   { if((abs((int)(x1s-x2e))>d)||(abs((int)(y1s-y2e))>d)) return CONNECT_NO;
		     temp=sqrt(((float)x1s-x2e)*((float)x1s-x2e)+((float)y1s-y2e)*((float)y1s-y2e));
		   }
	      }
	   }
      if(temp<=d) return CONNECT_YES;
      else return CONNECT_NO;
    }

   else if((y1s==y1e)&&(y2s==y2e))
    {
      if((((x1s>=x2s)&&(x1s<=x2e))||((x1s>=x2e)&&(x1s<=x2s)))||
	 (((x1e>=x2s)&&(x1e<=x2e))||((x1e>=x2e)&&(x1e<=x2s))))
	   {  if(y1s>=y2s) temp=y1s-y2s;
	      else temp=y2s-y1s;
	   }
      else if((((x2s>=x1s)&&(x2s<=x1e))||((x2s>=x1e)&&(x2s<=x1s)))||
	      (((x2e>=x1s)&&(x2e<=x1e))||((x2e>=x1e)&&(x2e<=x1s))))
	   {  if(y2s>=y1s) temp=y2s-y1s;
	      else temp=y1s-y2s;
	   }
      else { if(x1s>=x2s)
	      {  if((x1s>=x1e)&&(x2s>=x2e))
		  { if((abs((int)(x1e-x2s))>d)||(abs((int)(y1e-y2s))>d)) return CONNECT_NO;
		    temp=sqrt(((float)x1e-x2s)*((float)x1e-x2s)+((float)y1e-y2s)*((float)y1e-y2s));
		  }
		 else if((x1s>=x1e)&&(x2e>=x2s))
		  { if((abs((int)(x1e-x2e))>d)||(abs((int)(y1e-y2e))>d)) return CONNECT_NO;
		    temp=sqrt(((float)x1e-x2e)*((float)x1e-x2e)+((float)y1e-y2e)*((float)y1e-y2e));
		  }
		 else if((x1e>=x1s)&&(x2s>=x2e))
		  { if((abs((int)(x1s-x2s))>d)||(abs((int)(y1s-y2s))>d)) return CONNECT_NO;
		    temp=sqrt(((float)x1s-x2s)*((float)x1s-x2s)+((float)y1s-y2s)*((float)y1s-y2s));
		  }
		 else
		  { if((abs((int)(x1s-x2e))>d)||(abs((int)(y1s-y2e))>d)) return CONNECT_NO;
		    temp=sqrt(((float)x1s-x2e)*((float)x1s-x2e)+((float)y1s-y2e)*((float)y1s-y2e));
		  }
	      }
	     else
	      {  if((x2e>=x2s)&&(x1e>=x1s))
		  { if((abs((int)(x1e-x2s))>d)||(abs((int)(y1e-y2s))>d)) return CONNECT_NO;
		    temp=sqrt(((float)x1e-x2s)*((float)x1e-x2s)+((float)y1e-y2s)*((float)y1e-y2s));
		  }
		 else if((x2s>=x2e)&&(x1e>=x1s))
		  { if((abs((int)(x1e-x2e))>d)||(abs((int)(y1e-y2e))>d)) return CONNECT_NO;
		    temp=sqrt(((float)x1e-x2e)*((float)x1e-x2e)+((float)y1e-y2e)*((float)y1e-y2e));
		  }
		 else if((x2e>=x2s)&&(x1s>=x1e))
		  { if((abs((int)(x1s-x2s))>d)||(abs((int)(y1s-y2s))>d)) return CONNECT_NO;
		    temp=sqrt(((float)x1s-x2s)*((float)x1s-x2s)+((float)y1s-y2s)*((float)y1s-y2s));
		  }
		 else
		  { if((abs((int)(x1s-x2e))>d)||(abs((int)(y1s-y2e))>d)) return CONNECT_NO;
		    temp=sqrt(((float)x1s-x2e)*((float)x1s-x2e)+((float)y1s-y2e)*((float)y1s-y2e));
		  }
	      }
	   }
      if(temp<=d) return CONNECT_YES;
      else return CONNECT_NO;
    }

   else if(x1s==x1e)
    {
	k2=((float)y2e-y2s)/((float)x2e-x2s);
	tempx = x1e;
	tempy = k2*((float)x1e-x2e)+y2e;
	tempd[0] = MAX(y1e,y1s);
	tempd[1] = MIN(y1e,y1s);
	tempd[4] = MAX(x2e,x2s);
	tempd[5] = MIN(x2e,x2s);
	tempd[6] = MAX(y2e,y2s);
	tempd[7] = MIN(y2e,y2s);
	if((tempy<=tempd[0])&&(tempy>=tempd[1])&&\
	   (tempx<=tempd[4])&&(tempx>=tempd[5])&&\
	   (tempy<=tempd[6])&&(tempy>=tempd[7]))
	       return CONNECT_YES;
    }

   else if(x2s==x2e)
    {
	k1=((float)y1e-y1s)/((float)x1e-x1s);
	tempx = x2e;
	tempy = k1*((float)x2e-x1e)+y1e;
	tempd[0] = MAX(y2e,y2s);
	tempd[1] = MIN(y2e,y2s);
	tempd[4] = MAX(x1e,x1s);
	tempd[5] = MIN(x1e,x1s);
	tempd[6] = MAX(y1e,y1s);
	tempd[7] = MIN(y1e,y1s);
	if((tempy<=tempd[0])&&(tempy>=tempd[1])&&\
	   (tempx<=tempd[4])&&(tempx>=tempd[5])&&\
	   (tempy<=tempd[6])&&(tempy>=tempd[7]))
	       return CONNECT_YES;
    }
   else if(y1s==y1e)
    {
	k2=((float)y2e-y2s)/((float)x2e-x2s);
	k1 = 0;
	tempx = (k1*x1e-k2*x2e-(float)y1e+y2e)/(k1-k2);
	tempy = y1e;
	     tempd[0] = MAX(x2s,x2e);
	     tempd[1] = MIN(x2s,x2e);
	     tempd[2] = MAX(y2e,y2s);
	     tempd[3] = MIN(y2e,y2s);
	     tempd[4] = MAX(x1e,x1s);
	     tempd[5] = MIN(x1e,x1s);
	     tempd[6] = MAX(y1e,y1s);
	     tempd[7] = MIN(y1e,y1s);
	     if((tempx<=tempd[0])&&(tempx>=tempd[1])&&\
		(tempy<=tempd[2])&&(tempy>=tempd[3])&&\
		(tempx<=tempd[4])&&(tempx>=tempd[5])&&\
		(tempy<=tempd[6])&&(tempy>=tempd[7]))
		  return CONNECT_YES;
    }

   else if(y2s==y2e)
    {
	k1=((float)y1e-y1s)/((float)x1e-x1s);
	k2 = 0;
	tempx = (k1*x1e-k2*x2e-(float)y1e+y2e)/(k1-k2);
	tempy = y2e; 
	     tempd[0] = MAX(x2s,x2e);
	     tempd[1] = MIN(x2s,x2e);
	     tempd[2] = MAX(y2e,y2s);
	     tempd[3] = MIN(y2e,y2s);
	     tempd[4] = MAX(x1e,x1s);
	     tempd[5] = MIN(x1e,x1s);
	     tempd[6] = MAX(y1e,y1s);
	     tempd[7] = MIN(y1e,y1s);
	     if((tempx<=tempd[0])&&(tempx>=tempd[1])&&\
		(tempy<=tempd[2])&&(tempy>=tempd[3])&&\
		(tempx<=tempd[4])&&(tempx>=tempd[5])&&\
		(tempy<=tempd[6])&&(tempy>=tempd[7]))
		  return CONNECT_YES;
    }

   else
    {
	k1 = ((float)y1e-y1s)/((float)x1e-x1s);
	k2 = ((float)y2e-y2s)/((float)x2e-x2s);
	if(((long)(k1*10000))!=((long)(k2*10000)))
	  {  tempx = (k1*x1e-k2*x2e-(float)y1e+y2e)/(k1-k2);
	     tempy = (k1*k2*x1e-k1*k2*x2e+k1*y2e-k2*y1e)/(k1-k2);
	     tempd[0] = MAX(x2s,x2e);
	     tempd[1] = MIN(x2s,x2e);
	     tempd[2] = MAX(y2e,y2s);
	     tempd[3] = MIN(y2e,y2s);
	     tempd[4] = MAX(x1e,x1s);
	     tempd[5] = MIN(x1e,x1s);
	     tempd[6] = MAX(y1e,y1s);
	     tempd[7] = MIN(y1e,y1s);

	     if((tempx<=tempd[0])&&(tempx>=tempd[1])&&\
		(tempy<=tempd[2])&&(tempy>=tempd[3])&&\
		(tempx<=tempd[4])&&(tempx>=tempd[5])&&\
		(tempy<=tempd[6])&&(tempy>=tempd[7]))
		  return CONNECT_YES;
	  }
    }

   temp = Dist_P_Line(x1s,y1s,x2s,y2s,x2e,y2e,d);
   if(temp==CONNECT_YES) return CONNECT_YES;

   temp = Dist_P_Line(x1e,y1e,x2s,y2s,x2e,y2e,d);
   if(temp==CONNECT_YES) return CONNECT_YES;

   temp = Dist_P_Line(x2s,y2s,x1s,y1s,x1e,y1e,d);
   if(temp==CONNECT_YES) return CONNECT_YES;

   temp = Dist_P_Line(x2e,y2e,x1s,y1s,x1e,y1e,d);
   if(temp==CONNECT_YES) return CONNECT_YES;

   return CONNECT_NO;
}

int Line_Line_Connect(struct Line *oneline1,struct Line *oneline2, int drc)
{  
	//unsigned dist, d;
	//unsigned temp0,temp1,temp2,temp3;
	//unsigned x1s,y1s,x1e,y1e,x2s,y2s,x2e,y2e;
	int dist, d;
	int temp0,temp1,temp2,temp3;
	int x1s,y1s,x1e,y1e,x2s,y2s,x2e,y2e;

	d = oneline1->linewidth/2 + oneline2->linewidth/2 + drc;

	x1s=oneline1->startx;
	x1e=oneline1->endx;
	x2s=oneline2->startx;
	x2e=oneline2->endx;
	if(x1s>=x1e) { temp0=x1s; temp1=x1e; }
	else { temp0=x1e; temp1=x1s; }
	if(x2s>=x2e) { temp2=x2s; temp3=x2e; }
	else { temp2=x2e; temp3=x2s; }
	if((temp1>temp2)&&((temp1-temp2)>d)) return CONNECT_NO;
	if((temp3>temp0)&&((temp3-temp0)>d)) return CONNECT_NO;

	y1s=oneline1->starty;
	y1e=oneline1->endy;
	y2s=oneline2->starty;
	y2e=oneline2->endy;
	if(y1s>=y1e) { temp0=y1s; temp1=y1e; }
	else { temp0=y1e; temp1=y1s; }
	if(y2s>=y2e) { temp2=y2s; temp3=y2e; }
	else { temp2=y2e; temp3=y2s; }
	if((temp1>temp2)&&((temp1-temp2)>d)) return CONNECT_NO;
	if((temp3>temp0)&&((temp3-temp0)>d)) return CONNECT_NO;

	dist = Dist_Line_Line(x1s,y1s,x1e,y1e,x2s,y2s,x2e,y2e,d);
	return dist;
}

int Line_Pad_Connect(struct Line *oneline,struct Junction *jp, int drc)
{
   //unsigned dist,d;
   //unsigned deltax,deltay,temp;
   //unsigned sx,sy,ex,ey,cx,cy;
   int dist,d;
   int deltax,deltay,temp;
   int sx,sy,ex,ey,cx,cy;
   sx=oneline->startx; ex=oneline->endx;
   sy=oneline->starty; ey=oneline->endy;
   cx=jp->centerx; cy=jp->centery;
   d=(oneline->linewidth)/2+MAX((jp->Dx)/2,(jp->Dy)/2) + drc;
   if(sx>=ex) temp=ex;
   else temp=sx;
   if((temp>cx)&&((temp-cx)>d)) return CONNECT_NO;
   if(sx>=ex) temp=sx;
   else temp=ex;
   if((temp<cx)&&((cx-temp)>d)) return CONNECT_NO;
   if(sy>=ey) temp=ey;
   else temp=sy;
   if((temp>cy)&&((temp-cy)>d)) return CONNECT_NO;
   if(sy>=ey) temp=sy;
   else temp=ey;
   if((temp<cy)&&((cy-temp)>d)) return CONNECT_NO;

   if((jp->layer==MULTILAYER)||(jp->layer==oneline->layer)||(jp->id==0x22))
    {  if((((jp->padtype)&0x07)==1)||((((jp->padtype)&0x07)==3)&&(jp->Dx==jp->Dy)))
	 {  d=oneline->linewidth/2+jp->Dx/2 + drc - 1;
	    dist=(unsigned)Dist_P_Line( cx,cy,sx,sy,ex,ey,d);
	    return dist;
	 }
       else if(((jp->padtype)&0x07)==2)
	 {  deltax=(jp->Dx)/2;
	    deltay=(jp->Dy)/2;
	    d=oneline->linewidth/2 + drc - 1;
	    if(deltax>=deltay)
	      {
		  temp = (unsigned)Dist_Line_Line(cx+deltax,cy+deltay,cx+deltax,cy-deltay,\
						  sx,sy,ex,ey,d);
		  if(temp==1) return CONNECT_YES;

		  temp = (unsigned)Dist_Line_Line(cx-deltax,cy-deltay,cx-deltax,cy+deltay,\
						  sx,sy,ex,ey,d);
		  if(temp==1) return CONNECT_YES;

		  temp = (unsigned)Dist_Line_Line(cx-deltax,cy-deltay,cx+deltax,cy-deltay,\
						  sx,sy,ex,ey,d);
		  if(temp==1) return CONNECT_YES;

		  temp = (unsigned)Dist_Line_Line(cx-deltax,cy+deltay,cx+deltax,cy+deltay,\
						  sx,sy,ex,ey,d);
		  if(temp==1) return CONNECT_YES;
	      }
	    else
	      {
		 temp = (unsigned)Dist_Line_Line(cx-deltax,cy-deltay,cx+deltax,cy-deltay,\
						 sx,sy,ex,ey,d);
		 if(temp==1) return CONNECT_YES;

		 temp = (unsigned)Dist_Line_Line(cx-deltax,cy+deltay,cx+deltax,cy+deltay,\
						 sx,sy,ex,ey,d);
		 if(temp==1) return CONNECT_YES;

		 temp = (unsigned)Dist_Line_Line(cx+deltax,cy+deltay,cx+deltax,cy-deltay,\
						 sx,sy,ex,ey,d);
		 if(temp==1) return CONNECT_YES;
		 temp = (unsigned)Dist_Line_Line(cx-deltax,cy-deltay,cx-deltax,cy+deltay,\
						 sx,sy,ex,ey,d);
		 if(temp==1) return CONNECT_YES;
	      }
	    return CONNECT_NO;
	 }
       else if(((jp->padtype)&0x07)==3)
	 {
	    if(jp->Dx>=jp->Dy)
	       { deltax=(jp->Dx-jp->Dy)/2;
		 deltay=(jp->Dy)/2;
		 d=deltay+oneline->linewidth/2 + drc -1;
		 dist=(unsigned)Dist_Line_Line(cx+deltax,cy,cx-deltax,cy,\
					       sx,sy,ex,ey,d);
		 return dist;
	       }
	    else
	       { deltax=(jp->Dx)/2;
		 deltay=(jp->Dy-jp->Dx)/2;
		 d=deltax+oneline->linewidth/2 + drc -1;
		 dist=(unsigned)Dist_Line_Line(cx,cy+deltay,cx,cy-deltay,\
					       sx,sy,ex,ey,d);
		 return dist;
	       }
	 }
       else return CONNECT_NO;
    }
   else if(((jp->layer==BOTTOMLAYER)||(jp->layer==TOPLAYER))&&(jp->d!=0))
    {
	    d=oneline->linewidth/2 + drc -1;
	    dist=(unsigned)Dist_P_Line( cx,cy,sx,sy,ex,ey,d);
	    return dist;
    }

   else return CONNECT_NO;
}

int Pad_Pad_Connect(struct Junction *jp1,struct Junction *jp2,int drc)
{
   unsigned dist,d;
   unsigned deltax,deltay,temp;
   unsigned sx,sy,cx,cy;
   unsigned deltaxx,deltayy;
   unsigned x1,y1,x2,y2, xx1,yy1,xx2,yy2;

   sx=jp1->centerx; sy=jp1->centery;
   cx=jp2->centerx; cy=jp2->centery;

   d = MAX((jp1->Dx)/2,(jp1->Dy)/2) + MAX((jp2->Dx)/2,(jp2->Dy)/2) + drc;
   if((sx>cx)&&((sx-cx)>d)) return CONNECT_NO;
   if((sx<cx)&&((cx-sx)>d)) return CONNECT_NO;
   if((sy>cy)&&((sy-cy)>d)) return CONNECT_NO;
   if((sy<cy)&&((cy-sy)>d)) return CONNECT_NO;

	if((jp1->layer==MULTILAYER)||(jp2->layer==MULTILAYER)||
      (jp1->layer==jp2->layer)||(jp1->id==0x22)||(jp2->id==0x22))
    {
		if(((((jp1->padtype)&0x07)==1)||((((jp1->padtype)&0x07)==3)&&(jp1->Dx==jp1->Dy)))&&\
			((((jp2->padtype)&0x07)==1)||((((jp2->padtype)&0x07)==3)&&(jp2->Dx==jp2->Dy))))
		{
			d=(jp1->Dx/2)+(jp2->Dx/2)+drc;
			dist=(unsigned)Dist_P_P(jp1->centerx,jp1->centery,jp2->centerx,jp2->centery,d);
			return dist;
		}
		else if(((((jp1->padtype)&0x07)==1)||((((jp1->padtype)&0x07)==3)&&(jp1->Dx==jp1->Dy)))&&\
	       (((jp2->padtype)&0x07)==2))
		{
			deltax=(jp2->Dx)/2;
			deltay=(jp2->Dy)/2;
			if( (jp1->centerx > (jp2->centerx-deltax))&&(jp1->centerx < (jp2->centerx+deltax))&&\
				(jp1->centery > (jp2->centery-deltay))&&(jp1->centery < (jp2->centery+deltay)) )
			{
				return CONNECT_YES;
			}
			else if((abs(jp2->centerx-jp1->centerx) < jp1->Dx/2)&&(abs(jp2->centery-jp1->centery) < jp1->Dy/2))
			{
				return CONNECT_YES;
			}

			d=(jp1->Dx)/2 + drc;
			temp = (unsigned)Dist_P_Line(jp1->centerx,jp1->centery,\
				       jp2->centerx+deltax,jp2->centery+deltay,jp2->centerx+deltax,jp2->centery-deltay,d);
			if(temp==1) return CONNECT_YES;

			temp = (unsigned)Dist_P_Line(jp1->centerx,jp1->centery,\
				       jp2->centerx-deltax,jp2->centery-deltay,jp2->centerx-deltax,jp2->centery+deltay,d);
			if(temp==1) return CONNECT_YES;

			temp = (unsigned)Dist_P_Line(jp1->centerx,jp1->centery,\
				       jp2->centerx-deltax,jp2->centery-deltay,jp2->centerx+deltax,jp2->centery-deltay,d);
			if(temp==1) return CONNECT_YES;

			temp = (unsigned)Dist_P_Line(jp1->centerx,jp1->centery,\
				       jp2->centerx-deltax,jp2->centery+deltay,jp2->centerx+deltax,jp2->centery+deltay,d);
			if(temp==1) return CONNECT_YES;

			return CONNECT_NO;
		}
		else if((((jp1->padtype)&0x07)==2)&&\
	       ((((jp2->padtype)&0x07)==1))||((((jp2->padtype)&0x07)==3)&&(jp2->Dx==jp2->Dy)))
		{
			deltax=(jp1->Dx)/2;
			deltay=(jp1->Dy)/2;
			if( (jp2->centerx > (jp1->centerx-deltax))&&(jp2->centerx < (jp1->centerx+deltax))&&\
				(jp2->centery > (jp1->centery-deltay))&&(jp2->centery < (jp1->centery+deltay)) )
			{
				return CONNECT_YES;
			}
			else if((abs(jp1->centerx-jp2->centerx) < jp2->Dx/2)&&(abs(jp1->centery-jp2->centery) < jp2->Dy/2))
			{
				return CONNECT_YES;
			}
			d=(jp2->Dx)/2 + drc;
			temp = (unsigned)Dist_P_Line(jp2->centerx,jp2->centery,\
				       jp1->centerx+deltax,jp1->centery+deltay,jp1->centerx+deltax,jp1->centery-deltay,d);
			if(temp==1) return CONNECT_YES;

			temp = (unsigned)Dist_P_Line(jp2->centerx,jp2->centery,\
				       jp1->centerx-deltax,jp1->centery-deltay,jp1->centerx-deltax,jp1->centery+deltay,d);
			if(temp==1) return CONNECT_YES;

			temp = (unsigned)Dist_P_Line(jp2->centerx,jp2->centery,\
				       jp1->centerx-deltax,jp1->centery-deltay,jp1->centerx+deltax,jp1->centery-deltay,d);
			if(temp==1) return CONNECT_YES;

			temp = (unsigned)Dist_P_Line(jp2->centerx,jp2->centery,\
				       jp1->centerx-deltax,jp1->centery+deltay,jp1->centerx+deltax,jp1->centery+deltay,d);
			if(temp==1) return CONNECT_YES;

			return CONNECT_NO;
		}
		else if(((((jp1->padtype)&0x07)==1)||((((jp1->padtype)&0x07)==3)&&(jp1->Dx==jp1->Dy)))&&\
	       (((jp2->padtype)&0x07)==3))
		{
			if(jp2->Dx>=jp2->Dy)
			{
				deltax=(jp2->Dx-jp2->Dy)/2;
				deltay=(jp2->Dy)/2;
				d=deltay+(jp1->Dx)/2 + drc;
				dist=(unsigned)Dist_P_Line(jp1->centerx,jp1->centery,
					    jp2->centerx+deltax,jp2->centery,jp2->centerx-deltax,jp2->centery,d);
				return dist;
			}
			else
			{
				deltax=(jp2->Dx)/2;
				deltay=(jp2->Dy-jp2->Dx)/2;
				d=deltax+(jp1->Dx)/2 + drc;
				dist=(unsigned)Dist_P_Line(jp1->centerx,jp1->centery,
					    jp2->centerx,jp2->centery+deltay,jp2->centerx,jp2->centery-deltay,d);
				return dist;
			}
		}
		else if((((jp1->padtype)&0x07)==3)&&\
	       ((((jp2->padtype)&0x07)==1))||((((jp2->padtype)&0x07)==3)&&(jp2->Dx==jp2->Dy)))
		{
			if(jp1->Dx>=jp1->Dy)
			{
				deltax=(jp1->Dx-jp1->Dy)/2;
				deltay=(jp1->Dy)/2;
				d=deltay+(jp2->Dx)/2 + drc;
				dist=(unsigned)Dist_P_Line(jp2->centerx,jp2->centery,
					    jp1->centerx+deltax,jp1->centery,jp1->centerx-deltax,jp1->centery,d);
				return dist;
			}
			else
			{
				deltax=(jp1->Dx)/2;
				deltay=(jp1->Dy-jp1->Dx)/2;
				d=deltax+(jp2->Dx)/2 + drc;
				dist=(unsigned)Dist_P_Line(jp2->centerx,jp2->centery,
					    jp1->centerx,jp1->centery+deltay,jp1->centerx,jp1->centery-deltay,d);
				return dist;
			}
		}
		else if((((jp1->padtype)&0x07)==2)&&(((jp2->padtype)&0x07)==2))
		{
			deltax=(jp1->Dx)/2;
			deltay=(jp1->Dy)/2;
			deltaxx=(jp2->Dx)/2;
			deltayy=(jp2->Dy)/2;
			if( (jp1->centerx > (jp2->centerx-deltaxx))&&(jp1->centerx < (jp2->centerx+deltaxx))&&\
				(jp1->centery > (jp2->centery-deltayy))&&(jp1->centery < (jp2->centery+deltayy)) )
			{
				return CONNECT_YES;
			}
			if( (jp2->centerx > (jp1->centerx-deltax))&&(jp2->centerx < (jp1->centerx+deltax))&&\
				(jp2->centery > (jp1->centery-deltay))&&(jp2->centery < (jp1->centery+deltay)) )
			{
				return CONNECT_YES;
			}
			d=drc;
			// 1 
			temp = (unsigned)Dist_Line_Line(jp2->centerx+deltaxx,jp2->centery+deltayy,jp2->centerx+deltaxx,jp2->centery-deltayy,\
					    jp1->centerx+deltax,jp1->centery+deltay,jp1->centerx+deltax,jp1->centery-deltay,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(jp2->centerx+deltaxx,jp2->centery+deltayy,jp2->centerx+deltaxx,jp2->centery-deltayy,\
					    jp1->centerx-deltax,jp1->centery-deltay,jp1->centerx-deltax,jp1->centery+deltay,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(jp2->centerx+deltaxx,jp2->centery+deltayy,jp2->centerx+deltaxx,jp2->centery-deltayy,\
					    jp1->centerx-deltax,jp1->centery-deltay,jp1->centerx+deltax,jp1->centery-deltay,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(jp2->centerx+deltaxx,jp2->centery+deltayy,jp2->centerx+deltaxx,jp2->centery-deltayy,\
					    jp1->centerx-deltax,jp1->centery+deltay,jp1->centerx+deltax,jp1->centery+deltay,d);
			if(temp==1) return CONNECT_YES;

			// 2 
			temp = (unsigned)Dist_Line_Line(jp2->centerx-deltaxx,jp2->centery-deltayy,jp2->centerx-deltaxx,jp2->centery+deltayy,\
					    jp1->centerx+deltax,jp1->centery+deltay,jp1->centerx+deltax,jp1->centery-deltay,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(jp2->centerx-deltaxx,jp2->centery-deltayy,jp2->centerx-deltaxx,jp2->centery+deltayy,\
					    jp1->centerx-deltax,jp1->centery-deltay,jp1->centerx-deltax,jp1->centery+deltay,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(jp2->centerx-deltaxx,jp2->centery-deltayy,jp2->centerx-deltaxx,jp2->centery+deltayy,\
					    jp1->centerx-deltax,jp1->centery-deltay,jp1->centerx+deltax,jp1->centery-deltay,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(jp2->centerx-deltaxx,jp2->centery-deltayy,jp2->centerx-deltaxx,jp2->centery+deltayy,\
					    jp1->centerx-deltax,jp1->centery+deltay,jp1->centerx+deltax,jp1->centery+deltay,d);
			if(temp==1) return CONNECT_YES;

			// 3 
			temp = (unsigned)Dist_Line_Line(jp2->centerx-deltaxx,jp2->centery-deltayy,jp2->centerx+deltaxx,jp2->centery-deltayy,\
					    jp1->centerx+deltax,jp1->centery+deltay,jp1->centerx+deltax,jp1->centery-deltay,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(jp2->centerx-deltaxx,jp2->centery-deltayy,jp2->centerx+deltaxx,jp2->centery-deltayy,\
					    jp1->centerx-deltax,jp1->centery-deltay,jp1->centerx-deltax,jp1->centery+deltay,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(jp2->centerx-deltaxx,jp2->centery-deltayy,jp2->centerx+deltaxx,jp2->centery-deltayy,\
					    jp1->centerx-deltax,jp1->centery-deltay,jp1->centerx+deltax,jp1->centery-deltay,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(jp2->centerx-deltaxx,jp2->centery-deltayy,jp2->centerx+deltaxx,jp2->centery-deltayy,\
					    jp1->centerx-deltax,jp1->centery+deltay,jp1->centerx+deltax,jp1->centery+deltay,d);
			if(temp==1) return CONNECT_YES;

			// 4 
			temp = (unsigned)Dist_Line_Line(jp2->centerx-deltaxx,jp2->centery+deltayy,jp2->centerx+deltaxx,jp2->centery+deltayy,\
						jp1->centerx+deltax,jp1->centery+deltay,jp1->centerx+deltax,jp1->centery-deltay,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(jp2->centerx-deltaxx,jp2->centery+deltayy,jp2->centerx+deltaxx,jp2->centery+deltayy,\
					    jp1->centerx-deltax,jp1->centery-deltay,jp1->centerx-deltax,jp1->centery+deltay,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(jp2->centerx-deltaxx,jp2->centery+deltayy,jp2->centerx+deltaxx,jp2->centery+deltayy,\
					    jp1->centerx-deltax,jp1->centery-deltay,jp1->centerx+deltax,jp1->centery-deltay,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(jp2->centerx-deltaxx,jp2->centery+deltayy,jp2->centerx+deltaxx,jp2->centery+deltayy,\
					    jp1->centerx-deltax,jp1->centery+deltay,jp1->centerx+deltax,jp1->centery+deltay,d);
			if(temp==1) return CONNECT_YES;

			return CONNECT_NO;
		}
		else if((((jp1->padtype)&0x07)==3)&&(((jp2->padtype)&0x07)==3))
		{
			d=drc;
			if(jp1->Dx>jp1->Dy)
			{
				deltax=(jp1->Dx-jp1->Dy)/2;
				deltay=(jp1->Dy)/2;
				x1=jp1->centerx+deltax;
				y1=jp1->centery;
				x2=jp1->centerx-deltax;
				y2=jp1->centery;
				d+=deltay;
			}
			else
			{
				deltax=(jp1->Dx)/2;
				deltay=(jp1->Dy-jp1->Dx)/2;
				x1=jp1->centerx;
				y1=jp1->centery+deltay;
				x2=jp1->centerx;
				y2=jp1->centery-deltay;
				d+=deltax;
			}
			if(jp2->Dx>jp2->Dy)
			{
				deltaxx=(jp2->Dx-jp2->Dy)/2;
				deltayy=(jp2->Dy)/2;
				xx1=jp2->centerx+deltaxx;
				yy1=jp2->centery;
				xx2=jp2->centerx-deltaxx;
				yy2=jp2->centery;
				d+=deltayy;
			}
			else
			{
				deltaxx=(jp2->Dx)/2;
				deltayy=(jp2->Dy-jp2->Dx)/2;
				xx1=jp2->centerx;
				yy1=jp2->centery+deltayy;
				xx2=jp2->centerx;
				yy2=jp2->centery-deltayy;
				d+=deltaxx;
			}
			dist=(unsigned)Dist_Line_Line(x1,y1,x2,y2,xx1,yy1,xx2,yy2,d);
			return dist;
		}
		else if((((jp1->padtype)&0x07)==2)&&(((jp2->padtype)&0x07)==3))
		{
		    d=drc;
			if(jp2->Dx>jp2->Dy)
			{
				deltax=(jp2->Dx-jp2->Dy)/2;
				deltay=(jp2->Dy)/2;
				x1=jp2->centerx+deltax;
				y1=jp2->centery;
				x2=jp2->centerx-deltax;
				y2=jp2->centery;
				d+=deltay;
			}
			else
			{
				deltax=(jp2->Dx)/2;
				deltay=(jp2->Dy-jp2->Dx)/2;
				x1=jp2->centerx;
				y1=jp2->centery+deltay;
				x2=jp2->centerx;
				y2=jp2->centery-deltay;
				d+=deltax;
			}
			deltaxx=(jp1->Dx)/2;
			deltayy=(jp1->Dy)/2;
			temp = (unsigned)Dist_Line_Line(x1,y1,x2,y2,jp1->centerx+deltaxx,jp1->centery+deltayy,\
							jp1->centerx+deltaxx,jp1->centery-deltayy,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(x1,y1,x2,y2,jp1->centerx-deltaxx,jp1->centery-deltayy,\
							jp1->centerx-deltaxx,jp1->centery+deltayy,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(x1,y1,x2,y2,jp1->centerx-deltaxx,jp1->centery-deltayy,\
							jp1->centerx+deltaxx,jp1->centery-deltayy,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(x1,y1,x2,y2,jp1->centerx-deltaxx,jp1->centery+deltayy,\
							jp1->centerx+deltaxx,jp1->centery+deltayy,d);
			if(temp==1) return CONNECT_YES;
			return CONNECT_NO;
		}
		else if((((jp1->padtype)&0x07)==3)&&(((jp2->padtype)&0x07)==2))
		{
			d=drc;
			if(jp1->Dx>jp1->Dy)
			{
				deltax=(jp1->Dx-jp1->Dy)/2;
				deltay=(jp1->Dy)/2;
				x1=jp1->centerx+deltax;
				y1=jp1->centery;
				x2=jp1->centerx-deltax;
				y2=jp1->centery;
				d+=deltay;
			}
			else
			{
				deltax=(jp1->Dx)/2;
				deltay=(jp1->Dy-jp1->Dx)/2;
				x1=jp1->centerx;
				y1=jp1->centery+deltay;
				x2=jp1->centerx;
				y2=jp1->centery-deltay;
				d+=deltax;
			}
			deltaxx=(jp2->Dx)/2;
			deltayy=(jp2->Dy)/2;
			temp = (unsigned)Dist_Line_Line(x1,y1,x2,y2,jp2->centerx+deltaxx,jp2->centery+deltayy,\
							jp2->centerx+deltaxx,jp2->centery-deltayy,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(x1,y1,x2,y2,jp2->centerx-deltaxx,jp2->centery-deltayy,\
							jp2->centerx-deltaxx,jp2->centery+deltayy,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(x1,y1,x2,y2,jp2->centerx-deltaxx,jp2->centery-deltayy,\
							jp2->centerx+deltaxx,jp2->centery-deltayy,d);
			if(temp==1) return CONNECT_YES;
			temp = (unsigned)Dist_Line_Line(x1,y1,x2,y2,jp2->centerx-deltaxx,jp2->centery+deltayy,\
							jp2->centerx+deltaxx,jp2->centery+deltayy,d);
			if(temp==1) return CONNECT_YES;
			return CONNECT_NO;

		}
		else return CONNECT_NO;
    }
	else if(((jp1->layer==BOTTOMLAYER)&&(jp2->layer==TOPLAYER)||\
	    (jp2->layer==BOTTOMLAYER)&&(jp1->layer==TOPLAYER))&&\
	    ((jp1->d!=0)||(jp2->d!=0)))
    {
		d=jp1->Dx/2 + drc;
		dist=(unsigned)Dist_P_P( sx,sy,cx,cy,d);
		if(dist==CONNECT_YES) return dist;
		d=jp2->Dx/2 + drc;
		dist=(unsigned)Dist_P_P( sx,sy,cx,cy,d);
		return dist;
    }
	else return CONNECT_NO;
}

int Line_Arc_Connect(struct Line *oneline, struct Arc *onearc,int drc)
{  
	int i,d,dist;
	unsigned x1,y1,x2,y2;
	unsigned xc,yc;
	int stangle,endangle,r;
	unsigned xa,ya,xb,yb;
	int sangle,eangle;
	unsigned linemaxx,lineminx,linemaxy,lineminy,arcmaxx,arcminx,arcmaxy,arcminy;

	d = oneline->linewidth/2 + onearc->linewidth/2 + drc;
	linemaxx=MAX(oneline->startx,oneline->endx);
	lineminx=MIN(oneline->startx,oneline->endx);
	linemaxy=MAX(oneline->starty,oneline->endy);
	lineminy=MIN(oneline->starty,oneline->endy);
	arcmaxx=onearc->centerx+onearc->radius;
	arcminx=onearc->centerx-onearc->radius;
	arcmaxy=onearc->centery+onearc->radius;
	arcminy=onearc->centery-onearc->radius;
	if((lineminx>arcmaxx)&&((lineminx-arcmaxx)>d)) return CONNECT_NO;
	if((arcminx>linemaxx)&&((arcminx-linemaxx)>d)) return CONNECT_NO;
	if((lineminy>arcmaxy)&&((lineminy-arcmaxy)>d)) return CONNECT_NO;
	if((arcminy>linemaxy)&&((arcminy-linemaxy)>d)) return CONNECT_NO;

	x1=oneline->startx;
	y1=oneline->starty;
	x2=oneline->endx;
	y2=oneline->endy;
	xc=onearc->centerx;
	yc=onearc->centery;
	r=onearc->radius;
	stangle=onearc->stangle;
	endangle=onearc->endangle;

	//nEDA_Arc_precision=2;
	if(stangle==endangle) return CONNECT_NO;
	else if(endangle==stangle+360)
	{
		for(i=-180;i<180;i+=nEDA_Arc_precision)
		{
			sangle=i;eangle=i+nEDA_Arc_precision;
			xa=xc+r*cos(sangle*nEDA_pi/180);
			ya=yc+r*sin(sangle*nEDA_pi/180);
			xb=xc+r*cos(eangle*nEDA_pi/180);
			yb=yc+r*sin(eangle*nEDA_pi/180);
			dist=Dist_Line_Line(x1,y1,x2,y2,xa,ya,xb,yb,d);
			if(dist==1) return CONNECT_YES;
		}
	}
	else 
	{
		if(stangle<0)stangle+=360;
		if(endangle<0) endangle+=360;
		if(endangle<stangle) endangle+=360;
		for(i=stangle;i<endangle;i+=nEDA_Arc_precision)
		{
			sangle=i;eangle=i+nEDA_Arc_precision;
			xa=xc+r*cos(sangle*nEDA_pi/180);
			ya=yc+r*sin(sangle*nEDA_pi/180);
			xb=xc+r*cos(eangle*nEDA_pi/180);
			yb=yc+r*sin(eangle*nEDA_pi/180);
			dist=Dist_Line_Line(x1,y1,x2,y2,xa,ya,xb,yb,d);
			if(dist==1) return CONNECT_YES;
		}
	}
   
	//nEDA_Arc_precision=5;
	return CONNECT_NO;
}

int Line_Fill_Connect(struct Line *oneline,struct Fill *onefill, int drc)
{ 
	int i;
	unsigned x1,y1,x2,y2;
	unsigned xs,ys,xe,ye;
	unsigned temp0,temp1,temp2,temp3;
	int d,dist;

	x1=oneline->startx;  x2=oneline->endx;
	y1=oneline->starty;  y2=oneline->endy;
	d = oneline->linewidth/2 + drc;
	for(i=0;i<onefill->vertexnum-1;i++)
	{
		xs=onefill->pline[i].x;
		ys=onefill->pline[i].y;
		xe=onefill->pline[i+1].x;
		ye=onefill->pline[i+1].y;
		//DrawAnyLine(xs,ys,xe,ye,7,0);
		if(xs>=xe) { temp0=xs; temp1=xe; }
		else { temp0=xe; temp1=xs; }
		if(x1>=x2) { temp2=x1; temp3=x2; }
		else { temp2=x2; temp3=x1; }
		if((temp1>temp2)&&((temp1-temp2)>d))  continue;
		if((temp3>temp0)&&((temp3-temp0)>d))  continue;

		if(ys>=ye) { temp0=ys; temp1=ye; }
		else { temp0=ye; temp1=ys; }
		if(y1>=y2) { temp2=y1; temp3=y2; }
		else { temp2=y2; temp3=y1; }
		if((temp1>temp2)&&((temp1-temp2)>d))  continue;
		if((temp3>temp0)&&((temp3-temp0)>d))  continue;

		dist = (unsigned)Dist_Line_Line(x1,y1,x2,y2,xs,ys,xe,ye,d);
		if(dist==1) return CONNECT_YES;
	}
	return CONNECT_NO;
}

int Pad_Arc_Connect(struct Junction *jp, struct Arc *onearc, int drc)
{  
	int i,d,dist;
	unsigned xc,yc;
	int stangle,endangle,r;
	unsigned xa,ya,xb,yb;
	int sangle,eangle;
	unsigned cx,cy,arcmaxx,arcminx,arcmaxy,arcminy;


	if((jp->layer==MULTILAYER)||(jp->layer==onearc->layer)||(jp->id==0x22))
    {
		d=jp->Dx/2+onearc->linewidth/2+drc;
		cx=jp->centerx; cy=jp->centery;
		arcmaxx=onearc->centerx+onearc->radius;
		arcminx=onearc->centerx-onearc->radius;
		arcmaxy=onearc->centery+onearc->radius;
		arcminy=onearc->centery-onearc->radius;
		if((cx>arcmaxx)&&((cx-arcmaxx)>d)) return CONNECT_NO;
		if((arcminx>cx)&&((arcminx-cx)>d)) return CONNECT_NO;
		if((cy>arcmaxy)&&((cy-arcmaxy)>d)) return CONNECT_NO;
		if((arcminy>cy)&&((arcminy-cy)>d)) return CONNECT_NO;

		xc=onearc->centerx;
		yc=onearc->centery;
		r=onearc->radius;
		stangle=onearc->stangle;
		endangle=onearc->endangle;

		//nEDA_Arc_precision=2;
		if(stangle==endangle) return CONNECT_NO;
		else if(endangle==stangle+360)
		{
			for(i=-180;i<180;i+=nEDA_Arc_precision)
			{
				sangle=i;eangle=i+nEDA_Arc_precision;
				xa=xc+r*cos(sangle*nEDA_pi/180);
				ya=yc+r*sin(sangle*nEDA_pi/180);
				xb=xc+r*cos(eangle*nEDA_pi/180);
				yb=yc+r*sin(eangle*nEDA_pi/180);
				dist=Dist_P_Line(cx,cy,xa,ya,xb,yb,d);
				if(dist==1) return CONNECT_YES;
			}
		}
		else 
		{
			if(stangle<0)stangle+=360;
			if(endangle<0) endangle+=360;
			if(endangle<stangle) endangle+=360;
			for(i=stangle;i<endangle;i+=nEDA_Arc_precision)
			{
				sangle=i;eangle=i+nEDA_Arc_precision;
				xa=xc+r*cos(sangle*nEDA_pi/180);
				ya=yc+r*sin(sangle*nEDA_pi/180);
				xb=xc+r*cos(eangle*nEDA_pi/180);
				yb=yc+r*sin(eangle*nEDA_pi/180);
				dist=Dist_P_Line(cx,cy,xa,ya,xb,yb,d);
				if(dist==1) return CONNECT_YES;
			}
		}
    }
   
	//nEDA_Arc_precision=5;
	return CONNECT_NO;
}

int Pad_Fill_Connect(struct Junction *onejunc, struct Fill *onefill, int drc)
{
	float temp;
	int i,j,k,judge,s,flag;
	unsigned cx,cy,d;
	long x1,y1,x2,y2;
	unsigned xx,yy,crosspoint[50];
	unsigned maxx, maxy, minx, miny;
	struct Junction jp1;

	if((onejunc->layer==MULTILAYER)||\
      (onejunc->layer==onefill->layer)||(onejunc->id==0x22))
    {
		minx = nEDA_Size_Maxx; miny = nEDA_Size_Maxy;
		maxx = 0; maxy = 0;
		for(i=0;i<onefill->vertexnum-1;i++)
		{
			if( onefill->pline[i].x > maxx) maxx=onefill->pline[i].x;
			if( onefill->pline[i].y > maxy) maxy=onefill->pline[i].y;
			if( onefill->pline[i].x < minx) minx=onefill->pline[i].x;
			if( onefill->pline[i].y < miny) miny=onefill->pline[i].y;
		}

		if(onejunc->Dx>=onejunc->Dy) d = onejunc->Dx/2 + drc;
		else d = onejunc->Dy/2 + drc;
		cx = onejunc->centerx; cy = onejunc->centery;
		if((cx>maxx)&&((cx-maxx)>d)) return CONNECT_NO;
		if((cy>maxy)&&((cy-maxy)>d)) return CONNECT_NO;
		if((cx<minx)&&((minx-cx)>d)) return CONNECT_NO;
		if((cy<miny)&&((miny-cy)>d)) return CONNECT_NO;

		flag=0;
		if((cx>maxx)||(cx<minx)) flag++;
		if((cy>maxy)||(cy<miny)) flag+=2;

		if((flag==0)||(flag==1))
		{
			for(i=0;i<50;i++) crosspoint[i]=0;
			s=0;j=0;
			yy=cy;
			for(i=0;i<onefill->vertexnum-1;i+=1)
			{
				x1=onefill->pline[i].x;
				y1=onefill->pline[i].y;
				x2=onefill->pline[i+1].x;
				y2=onefill->pline[i+1].y;
				//if((y1==y2)&&(yy==y2))
				//{
				//	crosspoint[s] = x1;
				//	s++;
				//}
				if(((y1>=yy)&&(y2<yy))||((y1<=yy)&&(y2>yy)))
				{
					temp = (float)(x2-x1)/(y2-y1);
					xx = temp*((float)yy-y1) + x1;
					crosspoint[s]=xx;
					s++;
				}
			}
			s=s/2; s=s*2;
			judge=1;
			for(;;)
			{
				if(judge==1) judge=0;
				else if(judge==0) break;
				for(i=0;i<s;i++)
				{
					if( crosspoint[i]<crosspoint[i+1])
					{
						judge=1;
						k=crosspoint[i]; crosspoint[i]=crosspoint[i+1];
						crosspoint[i+1]=k;
					}
				}
			}

			for(i=0;i<s;i++)
			{
				if(crosspoint[i]<=cx) break;
			}

			if((i%2)!=0) return CONNECT_YES;
			d = onejunc->Dx/2 + drc;
			if(i==0)
			{
				j = Dist_P_P(cx,cy,crosspoint[0],cy,d);
				if(j==CONNECT_YES) return j;
			}
			else if(i==s)
			{
				j = Dist_P_P(cx,cy,crosspoint[i-1],cy,d);
				if(j==CONNECT_YES) return j;
			}
			else
			{
				j = Dist_P_P(cx,cy,crosspoint[i],cy,d);
				if(j==CONNECT_YES) return j;
				j = Dist_P_P(cx,cy,crosspoint[i-1],cy,d);
				if(j==CONNECT_YES) return j;
			}
		}
		else if(flag==2)
		{
			for(i=0;i<50;i++) crosspoint[i]=0;
			s=0;j=0;
			xx=cx;
			for(i=0;i<onefill->vertexnum-1;i+=1)
			{
				x1=onefill->pline[i].x;
				y1=onefill->pline[i].y;
				x2=onefill->pline[i+1].x;
				y2=onefill->pline[i+1].y;
				//if((x1==x2)&&(xx==x2))
				//{
				//	crosspoint[s]=y1;
				//	s++;
				//}
				if(((x1>=xx)&&(x2<xx))||((x1<=xx)&&(x2>xx)))
				{
					temp=(float)((float)xx-x1)/(x2-x1);
					yy=(UINT)(temp*((float)y2-y1) + (float)y1);
					crosspoint[s]=yy;
					s++;
				}
			}

			s=s/2; s=s*2;
			judge=1;
			for(;;)
			{
				if(judge==1) judge=0;
				else if(judge==0) break;
				for(i=0;i<s;i++)
				{
					if( crosspoint[i]<crosspoint[i+1])
					{
						judge=1;
						k=crosspoint[i]; crosspoint[i]=crosspoint[i+1];
						crosspoint[i+1]=k;
					}
				}
			}
			for(i=0;i<s;i++)
			{
				if(crosspoint[i]<=cy) break;
			}
			
			if((i%2)!=0) return CONNECT_YES;
			d = onejunc->Dy/2 + drc;
			if(i==0)
			{
				j = Dist_P_P(cx,cy,cx,crosspoint[i],d);
				if(j==CONNECT_YES) return j;
			}
			else if(i==s)
			{
				j = Dist_P_P(cx,cy,cx,crosspoint[i-1],d);
				if(j==CONNECT_YES) return j;
			}
			else
			{
				j = Dist_P_P(cx,cy,cx,crosspoint[i],d);
				if(j==CONNECT_YES) return j;
				j = Dist_P_P(cx,cy,cx,crosspoint[i-1],d);
				if(j==CONNECT_YES) return j;
			}
		}
		for(i=0;i<onefill->vertexnum-1;i++)
		{
			x1 = onefill->pline[i].x;
			y1 = onefill->pline[i].y;
			jp1.centerx = x1; jp1.centery = y1; 
			jp1.Dx = 1;  jp1.Dy = 1; 
			jp1.layer = MULTILAYER;  jp1.padtype = 1;
			j = Pad_Pad_Connect(&jp1,  onejunc, drc);
			//j = Dist_P_P(cx,cy,x1,y1,d);
			if(j==CONNECT_YES) return j;
		}
    }
	return CONNECT_NO;
}

int Fill_Fill_Connect(struct Fill *fill1,struct Fill *fill2, int drc)
{ 
	int i;
	int ii;
	unsigned x1,y1,x2,y2;
	unsigned xs,ys,xe,ye;
	unsigned temp0,temp1,temp2,temp3;
	int d,dist;

	d = drc;
	for(ii=0;ii<fill1->vertexnum-1;ii++)
	{
		x1=fill1->pline[ii].x;  x2=fill1->pline[ii+1].x;
		y1=fill1->pline[ii].y;  y2=fill1->pline[ii+1].y;

		for(i=0;i<fill2->vertexnum-1;i++)
		{
			xs=fill2->pline[i].x;
			ys=fill2->pline[i].y;
			xe=fill2->pline[i+1].x;
			ye=fill2->pline[i+1].y;
			
			if(xs>=xe) { temp0=xs; temp1=xe; }
			else { temp0=xe; temp1=xs; }
			if(x1>=x2) { temp2=x1; temp3=x2; }
			else { temp2=x2; temp3=x1; }
			if((temp1>temp2)&&((temp1-temp2)>(UINT)d))  continue;;
			if((temp3>temp0)&&((temp3-temp0)>(UINT)d))  continue;;

			if(ys>=ye) { temp0=ys; temp1=ye; }
			else { temp0=ye; temp1=ys; }
			if(y1>=y2) { temp2=y1; temp3=y2; }
			else { temp2=y2; temp3=y1; }
			if((temp1>temp2)&&((temp1-temp2)>(UINT)d))  continue;;
			if((temp3>temp0)&&((temp3-temp0)>(UINT)d))  continue;;

			dist = (unsigned)Dist_Line_Line(x1,y1,x2,y2,xs,ys,xe,ye,d);
			if(dist==1) return CONNECT_YES;
		}
	}
	return CONNECT_NO;
}

int Find_min(int main1, short int *distbuf, char *juncbuf, int num)
{ 
	int i;
	int main2;
	struct Node
    {  unsigned short cx,cy;
       unsigned char layer,flag,state;  } *node;
	unsigned long minl,length;
	char *temp;

	temp = juncbuf;
	minl = nEDA_Size_Maxx;
	main2 = 0;
	for(i=0; i<num; i++)
	{
		node = (struct Node*)temp;
		temp += 7;
		if((node->flag==1)||(i==main1)) continue;
		if(i>main1)  length=distbuf[((long)main1*num-(long)main1*(main1+1)/2)+(i-main1-1)];
		else length=distbuf[((long)i*num-(long)i*(i+1)/2)+(main1-i-1)];
		if(length<minl)
		{
			minl=length;
			main2=i;
		}
	}
	return main2;
}

*/


