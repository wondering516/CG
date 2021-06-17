// AEllipse.cpp: implementation of the CAEllipse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "test.h"
#include "AEllipse.h"
#include "math.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAEllipse::CAEllipse()
{
	Bc=CRGB(1.0,1.0,1.0);//默认屏幕背景色
}

CAEllipse::~CAEllipse()
{

}

void CAEllipse::AEllipsePointAC(CDC* pDC,CP2 point,double e,CRGB color)//画椭圆AC段子函数
{//填充四个象限中的虚拟像素点
	CP2 Center;
	Center=(P0+P1)/2;//获得椭圆的中心点坐标
	CRGB c1,c2;			
	c1=e*(Bc-color)+color;
	c2=(1.0-e)*(Bc-color)+color;
	FillVPiexl(pDC,CP2(point.x+Center.x,point.y+Center.y),c1);//(x,y)
	FillVPiexl(pDC,CP2(point.x+Center.x,(point.y-1)+Center.y),c2);
	FillVPiexl(pDC,CP2(point.x+Center.x,-point.y+Center.y),c1);//(x,-y)
	FillVPiexl(pDC,CP2(point.x+Center.x,-(point.y-1)+Center.y),c2);
	FillVPiexl(pDC,CP2(-point.x+Center.x,-point.y+Center.y),c1);//(-x,-y)
	FillVPiexl(pDC,CP2(-point.x+Center.x,-(point.y-1)+Center.y),c2);
	FillVPiexl(pDC,CP2(-point.x+Center.x,point.y+Center.y),c1);//(-x,y)
	FillVPiexl(pDC,CP2(-point.x+Center.x,(point.y-1)+Center.y),c2);	
}

void CAEllipse::AEllipsePointBC(CDC *pDC,CP2 point,double e,CRGB color)//画椭圆BC段子函数
{	
	CP2 Center;
	Center=(P0+P1)/2;//获得椭圆的中心点坐标
	CRGB c1,c2;			
	c1=e*(Bc-color)+color;
	c2=(1.0-e)*(Bc-color)+color;
	FillVPiexl(pDC,CP2(point.x+Center.x,point.y+Center.y),c1);//(x,y)
	FillVPiexl(pDC,CP2((point.x-1)+Center.x,point.y+Center.y),c2);    
	FillVPiexl(pDC,CP2(point.x+Center.x,-point.y+Center.y),c1);//(x,-y)
	FillVPiexl(pDC,CP2((point.x-1)+Center.x,-point.y+Center.y),c2);
	FillVPiexl(pDC,CP2(-point.x+Center.x,-point.y+Center.y),c1);//(-x,-y)
	FillVPiexl(pDC,CP2(-(point.x-1)+Center.x,-point.y+Center.y),c2);
	FillVPiexl(pDC,CP2(-point.x+Center.x,point.y+Center.y),c1);//(-x,y)
	FillVPiexl(pDC,CP2(-(point.x-1)+Center.x,point.y+Center.y),c2);	
}

void CAEllipse::DrawAEllipse(CDC* pDC,CP2 p0,CP2 p1)//绘制椭圆函数
{
	P0=p0;
	P1=p1;
	double a,b;//椭圆的长半轴，短半轴
	CP2 Center,Axle;//椭圆的圆心和半轴
	Center=(P0+P1)/2;//椭圆中心
	Axle=(P1-P0)/2;//椭圆半轴
	a=abs(int(Axle.x));//获得长半轴长度
	b=abs(int(Axle.y));//获得短半轴长度
	if(a==0 && b==0)
		return;
	double e,h;
	CP2 p,pc;//点的坐标
	CRGB color;
	color=p0.c;
	pc.x=a*a/sqrt(a*a+b*b),pc.y=b*b/sqrt(a*a+b*b);//C点的坐标
	for(p.x=0,p.y=b,h=b;p.x<=pc.x;p.x++)//绘制AC段
	{
		e=h-sqrt(b*b-b*b*(p.x+1)*(p.x+1)/(a*a));
		if(e>=1)
		{
			h--;
			e--;
			p.y--;
		}	
		AEllipsePointAC(pDC,p,e,color);
	}
	for(p.x=a,p.y=0,h=a;p.y<pc.y;p.y++)//绘制BC段
	{
		e=h-sqrt(a*a-a*a*(p.y+1)*(p.y+1)/(b*b));
		if(e>=1)
		{
			h--;
			e--;
			p.x--;
		}	
		AEllipsePointBC(pDC,p,e,color);
	}
}