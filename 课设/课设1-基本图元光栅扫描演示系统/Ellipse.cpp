// Ellipse.cpp: implementation of the CEllipse class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Ellipse.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEllipse::CEllipse()
{

}

CEllipse::~CEllipse()
{

}

void CEllipse::EllipsePoint(CDC* pDC,CP2 point,CRGB color)//四分法画椭圆
{//填充四个象限中的虚拟像素点
	CP2 Center;
	Center=(P0+P1)/2;//获得椭圆的中心点坐标	
	FillVPiexl(pDC,CP2( point.x+Center.x, point.y+Center.y),color);// x, y
	FillVPiexl(pDC,CP2(-point.x+Center.x, point.y+Center.y),color);//-x, y
	FillVPiexl(pDC,CP2( point.x+Center.x,-point.y+Center.y),color);// x,-y
	FillVPiexl(pDC,CP2(-point.x+Center.x,-point.y+Center.y),color);//-x,-y
}
void CEllipse::DrawEllipse(CDC* pDC,CP2 p0,CP2 p1)//绘制椭圆函数
{
	P0=p0;
	P1=p1;
	double a,b;//椭圆的长半轴，短半轴
	CP2 Center,Axle;//椭圆的圆心
	double d1,d2; 
	CRGB color;
	color=p0.c;
	Center=(P0+P1)/2;//椭圆中心
	Axle=(P1-P0)/2;//椭圆半轴
	a=abs(int(Axle.x));//获得长半轴长度
	b=abs(int(Axle.y));//获得短半轴长度
	CP2 p(0,b);	
	d1=b*b+a*a*(-b+0.25);
	EllipsePoint(pDC,p,color);//绘制第一个像素点
	while(b*b*(p.x+1)<a*a*(p.y-0.5))//椭圆AC弧段的判别式
	{
		if(d1<0)
		{
			d1+=b*b*(2*p.x+3);
		}
		else
		{
			d1+=b*b*(2*p.x+3)+a*a*(-2*p.y+2);
			p.y--;
		}
		p.x++;
		EllipsePoint(pDC,p,color);//填充像素点
	}
	d2=b*b*(p.x+0.5)*(p.x+0.5)+a*a*(p.y-1)*(p.y-1)-a*a*b*b;//椭圆BC弧段的判别式
	while(p.y>0)
	{
		if (d2<0)
		{
			d2+=b*b*(2*p.x+2)+a*a*(-2*p.y+3);
			p.x++;
		}
		else
		{
			d2+=a*a*(-2*p.y+3);			
		}
		p.y--;
		EllipsePoint(pDC,p,color);
	}
}



