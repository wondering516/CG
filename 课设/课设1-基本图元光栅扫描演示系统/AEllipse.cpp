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
	Bc=CRGB(1.0,1.0,1.0);//Ĭ����Ļ����ɫ
}

CAEllipse::~CAEllipse()
{

}

void CAEllipse::AEllipsePointAC(CDC* pDC,CP2 point,double e,CRGB color)//����ԲAC���Ӻ���
{//����ĸ������е��������ص�
	CP2 Center;
	Center=(P0+P1)/2;//�����Բ�����ĵ�����
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

void CAEllipse::AEllipsePointBC(CDC *pDC,CP2 point,double e,CRGB color)//����ԲBC���Ӻ���
{	
	CP2 Center;
	Center=(P0+P1)/2;//�����Բ�����ĵ�����
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

void CAEllipse::DrawAEllipse(CDC* pDC,CP2 p0,CP2 p1)//������Բ����
{
	P0=p0;
	P1=p1;
	double a,b;//��Բ�ĳ����ᣬ�̰���
	CP2 Center,Axle;//��Բ��Բ�ĺͰ���
	Center=(P0+P1)/2;//��Բ����
	Axle=(P1-P0)/2;//��Բ����
	a=abs(int(Axle.x));//��ó����᳤��
	b=abs(int(Axle.y));//��ö̰��᳤��
	if(a==0 && b==0)
		return;
	double e,h;
	CP2 p,pc;//�������
	CRGB color;
	color=p0.c;
	pc.x=a*a/sqrt(a*a+b*b),pc.y=b*b/sqrt(a*a+b*b);//C�������
	for(p.x=0,p.y=b,h=b;p.x<=pc.x;p.x++)//����AC��
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
	for(p.x=a,p.y=0,h=a;p.y<pc.y;p.y++)//����BC��
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