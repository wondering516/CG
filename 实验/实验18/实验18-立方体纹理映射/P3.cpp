// P3.cpp: implementation of the CP3 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "P3.h"
#include "math.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CP3::CP3()
{
	this->z=0.0;
}

CP3::~CP3()
{

}
CP3::CP3(double x0,double y0,double z0):CP2(x0,y0)
{
	this->z=z0;
}

CP3 operator +(CP3 &p1,CP3 &p2)//和
{	
	CP3 p;
	p.x=p1.x+p2.x;
	p.y=p1.y+p2.y;
	p.z=p1.z+p2.z;
	return p;
}

CP3 operator -(CP3 &p1,CP3 &p2)//差
{
	CP3 p;
	p.x=p1.x-p2.x;
	p.y=p1.y-p2.y;
	p.z=p1.z-p2.z;
	return p;
}

CP3 operator *(CP3 &p,double k)//点和常量的积
{	
	return CP3(p.x*k,p.y*k,p.z*k);
}

CP3 operator *(double k,CP3 &p)//点和常量的积
{	
	return CP3(p.x*k,p.y*k,p.z*k);
}

CP3 operator /(CP3 &p,double k)//数除
{
	if(fabs(k)<1e-6)
		k=1.0;
	CP3 point;
	point.x=p.x/k;
	point.y=p.y/k;
	point.z=p.z/k;
	return point;
}

CP3 operator+=(CP3 &p1,CP3 &p2)
{
	p1.x=p1.x+p2.x;
	p1.y=p1.y+p2.y;
	p1.z=p1.z+p2.z;
	return p1;
}

CP3 operator-=(CP3 &p1,CP3 &p2)
{
	p1.x=p1.x-p2.x;
	p1.y=p1.y-p2.y;
	p1.z=p1.z-p2.z;
	return p1;
}

CP3 operator*=(CP3 &p1,double k)
{
	p1.x=p1.x*k;
	p1.y=p1.y*k;
	p1.z=p1.z*k;
	return p1;
}

CP3 operator/=(CP3 &p1,double k)
{
	if(fabs(k)<1e-6)
		k=1.0;
	p1.x=p1.x/k;
	p1.y=p1.y/k;
	p1.z=p1.z/k;
	return p1;
}

double CP3::Mold()//长度
{
	double product=sqrt(this->x*this->x+this->y*this->y+this->z*this->z);
	return product;
}

