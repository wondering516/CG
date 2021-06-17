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

void CEllipse::EllipsePoint(CDC* pDC,CP2 point,CRGB color)//�ķַ�����Բ
{//����ĸ������е��������ص�
	CP2 Center;
	Center=(P0+P1)/2;//�����Բ�����ĵ�����	
	FillVPiexl(pDC,CP2( point.x+Center.x, point.y+Center.y),color);// x, y
	FillVPiexl(pDC,CP2(-point.x+Center.x, point.y+Center.y),color);//-x, y
	FillVPiexl(pDC,CP2( point.x+Center.x,-point.y+Center.y),color);// x,-y
	FillVPiexl(pDC,CP2(-point.x+Center.x,-point.y+Center.y),color);//-x,-y
}
void CEllipse::DrawEllipse(CDC* pDC,CP2 p0,CP2 p1)//������Բ����
{
	P0=p0;
	P1=p1;
	double a,b;//��Բ�ĳ����ᣬ�̰���
	CP2 Center,Axle;//��Բ��Բ��
	double d1,d2; 
	CRGB color;
	color=p0.c;
	Center=(P0+P1)/2;//��Բ����
	Axle=(P1-P0)/2;//��Բ����
	a=abs(int(Axle.x));//��ó����᳤��
	b=abs(int(Axle.y));//��ö̰��᳤��
	CP2 p(0,b);	
	d1=b*b+a*a*(-b+0.25);
	EllipsePoint(pDC,p,color);//���Ƶ�һ�����ص�
	while(b*b*(p.x+1)<a*a*(p.y-0.5))//��ԲAC���ε��б�ʽ
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
		EllipsePoint(pDC,p,color);//������ص�
	}
	d2=b*b*(p.x+0.5)*(p.x+0.5)+a*a*(p.y-1)*(p.y-1)-a*a*b*b;//��ԲBC���ε��б�ʽ
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



