// ACLine.cpp: implementation of the CACLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "ACLine.h"
#define  ROUND(x)  int(x+0.5)
#include "math.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CACLine::CACLine()
{ 
	Bc=CRGB(1.0,1.0,1.0);//默认屏幕背景色
}

CACLine::CACLine(CRGB bkcolor)//设置背景色
{
	Bc=bkcolor;
}

CACLine::~CACLine()
{
	
}

void CACLine::MoveTo(CDC *pDC,CP2 p0)
{	
	P0=p0;
}

void CACLine::MoveTo(CDC *pDC,double x,double y,CRGB c)//重载函数
{	
	MoveTo(pDC,CP2(x,y,c));
}

void CACLine::LineTo(CDC *pDC,CP2 p1)
{
	P1=p1;
	CP2 p,t;
	CRGB c0,c1;
	if(fabs(P0.x-P1.x)==0)//绘制垂线
	{
		if(P0.y>P1.y)//交换顶点,使得起始点低于终点顶点
		{
			t=P0;P0=P1;P1=t;
		}
		for(p=P0;p.y<P1.y;p.y++)
		{
			p.c=Interpolation(p.y,P0.y,P1.y,P0.c,P1.c);
			FillVPiexl(pDC,CP2(ROUND(p.x),ROUND(p.y)),p.c);
		}
	}	
	else
	{
		double k,e;
		k=(P1.y-P0.y)/(P1.x-P0.x);
		if(k>1.0)//绘制k＞1
		{
			if(P0.y>P1.y)
			{
				t=P0;P0=P1;P1=t;
			}	  		
			for(p=P0,e=1/k;p.y<P1.y;p.y++)
			{
				c0=(Bc-p.c)*e+p.c;
				c1=(Bc-p.c)*(1-e)+p.c;
				FillVPiexl(pDC,CP2(p.x,p.y),c0);
				FillVPiexl(pDC,CP2(p.x+1,p.y),c1);
				e=e+1/k;
                if(e>=1.0)
				{
					p.x++;
					e--;
				}				     
			}
		}
		if(0.0<=k && k<=1.0)//绘制0≤k≤1
		{
			if(P0.x>P1.x)
			{
				t=P0;P0=P1;P1=t;
			}	  
			for(p=P0,e=k;p.x<P1.x;p.x++)
			{
				p.c=Interpolation(p.x,P0.x,P1.x,P0.c,P1.c);
				c0=(Bc-p.c)*e+p.c;
				c1=(Bc-p.c)*(1-e)+p.c;
				FillVPiexl(pDC,CP2(p.x,p.y),c0);
				FillVPiexl(pDC,CP2(p.x,p.y+1),c1);					
				e=e+k;
                if(e>=1.0)
				{
					p.y++;
					e--;
				}				     
			}
		}		
		if(k>=-1.0 && k<0.0)//绘制-1≤k＜0
		{
			if(P0.x>P1.x)
			{
				t=P0;P0=P1;P1=t;
			}	 
			for(p=P0,e=-k;p.x<P1.x;p.x++)
			{
				p.c=Interpolation(p.x,P0.x,P1.x,P0.c,P1.c);
				c0=(Bc-p.c)*e+p.c;
				c1=(Bc-p.c)*(1-e)+p.c;
				FillVPiexl(pDC,CP2(p.x,p.y),c0);
				FillVPiexl(pDC,CP2(p.x,p.y-1),c1);
				e=e-k;
                if(e>=1.0)
				{
					p.y--;
					e--;
				}				
			}
		}
		if(k<-1.0)//绘制k＜-1 
		{
			if(P0.y<P1.y)
			{
				t=P0;P0=P1;P1=t;
			}	  
			for(p=P0,e=-1/k;p.y>P1.y;p.y--)
			{
				p.c=Interpolation(p.y,P0.y,P1.y,P0.c,P1.c);
				c0=(Bc-p.c)*e+p.c;
				c1=(Bc-p.c)*(1-e)+p.c;
				FillVPiexl(pDC,CP2(p.x,p.y),c0);
				FillVPiexl(pDC,CP2(p.x+1,p.y),c1);
				e=e-1/k;
                if(e>=1.0)
				{
					p.x++;
					e--;
				}				   
			}
		}
	}
	P0=p1;
}

void CACLine::LineTo(CDC *pDC,double x,double y,CRGB c)//重载函数
{
	 LineTo(pDC,CP2(x,y,c));
}

CRGB CACLine::Interpolation(double t,double t0,double t1,CRGB c0,CRGB c1)//线性插值
{
	return (t-t1)/(t0-t1)*c0+(t-t0)/(t1-t0)*c1;
}


