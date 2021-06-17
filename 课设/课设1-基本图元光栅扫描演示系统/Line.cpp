// Line.cpp: implementation of the CLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Line.h"
#include "math.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLine::CLine()
{ 

}

CLine::~CLine()
{
	
}

void CLine::MoveTo(CDC *pDC,CP2 p0)//起点
{	
	P0=p0;
}

void CLine::MoveTo(CDC *pDC,double x,double y,CRGB c)//重载函数
{	
	MoveTo(pDC,CP2(x,y,c));
}

void CLine::LineTo(CDC *pDC,CP2 p1)//终点
{//Bresenham算法绘制任意斜率直线
	P1=p1;
	CP2 p,t;
	if(fabs(P0.x-P1.x)<1e-6)//绘制垂线
	{
		if(P0.y>P1.y)//交换顶点,使得起始点低于终点
		{
			t=P0;P0=P1;P1=t;
		}
		for(p=P0;p.y<P1.y;p.y++)
		{	
			p.c=Interpolation(p.y,P0.y,P1.y,P0.c,P1.c);
			FillVPiexl(pDC,CP2(p.x,p.y),p.c);
		}
	}
	else
	{
		double k,d;
		k=(P1.y-P0.y)/(P1.x-P0.x);
		if(k>1)//绘制k>1
		{
			if(P0.y>P1.y)
			{
				t=P0;P0=P1;P1=t;
			}
			d=1-0.5*k;
			for(p=P0;p.y<P1.y;p.y++)
			{
				p.c=Interpolation(p.y,P0.y,P1.y,P0.c,P1.c);
			    FillVPiexl(pDC,CP2(p.x,p.y),p.c);
                if(d>=0)
				{
					p.x++;
					d+=1-k;
				}
				else 
                    d+=1;       
			}
		}
		if(0<=k && k<=1)//绘制0≤k≤1
		{
			if(P0.x>P1.x)
			{
				t=P0;P0=P1;P1=t;
			}
			d=0.5-k; 
			for(p=P0;p.x<P1.x;p.x++)
			{
				p.c=Interpolation(p.x,P0.x,P1.x,P0.c,P1.c);
				FillVPiexl(pDC,CP2(p.x,p.y),p.c);
                if(d<0)
				{
					p.y++;
					d+=1-k;
				}
				else 
					d-=k;		
			}
		}		
		if(k>=-1 && k<0)//绘制-1≤k＜0
		{
			if(P0.x>P1.x)
			{
				t=P0;P0=P1;P1=t;
			}
			d=-0.5-k;
            for(p=P0;p.x<P1.x;p.x++)
			{
				p.c=Interpolation(p.x,P0.x,P1.x,P0.c,P1.c);
				FillVPiexl(pDC,CP2(p.x,p.y),p.c);
                if(d>0)
				{
					p.y--;
					d-=1+k;
				}
				else 
					d-=k;		
			}
		}
		if(k<-1)//绘制k<-1 
		{
			if(P0.y<P1.y)
			{
				t=P0;P0=P1;P1=t;
			}
			d=-1-0.5*k;
			for(p=P0;p.y>P1.y;p.y--)
			{
				p.c=Interpolation(p.y,P0.y,P1.y,P0.c,P1.c);
				FillVPiexl(pDC,CP2(p.x,p.y),p.c);
                if(d<0)
				{
					p.x++;
					d-=1+k;
				}
				else 
					d-=1;           
			}
		}
	}
	P0=P1;
}

void CLine::LineTo(CDC *pDC,double x,double y,CRGB c)//重载函数
{
	 LineTo(pDC,CP2(x,y,c));
}

CRGB CLine::Interpolation(double t,double t0,double t1,CRGB c0,CRGB c1)//线性插值
{
	return (t-t1)/(t0-t1)*c0+(t-t0)/(t1-t0)*c1;
}
