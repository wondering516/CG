// P2.h: interface for the CP2 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_P2_H__E525809F_8CB8_41E2_ABF7_3909432BB73D__INCLUDED_)
#define AFX_P2_H__E525809F_8CB8_41E2_ABF7_3909432BB73D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "RGB.h"

class CP2//定义二维点类
{
public:
	CP2();
	CP2(double,double);
	CP2(double,double,CRGB);
	virtual ~CP2();
	friend CP2 operator +(CP2 &,CP2 &);
	friend CP2 operator -(CP2 &,CP2 &);
	friend CP2 operator *(CP2 &,double);
	friend CP2 operator *(double,CP2 &);
	friend CP2 operator /(CP2 &,double);
	friend CP2 operator+=(CP2 &,CP2 &);
	friend CP2 operator-=(CP2 &,CP2 &);
	friend CP2 operator*=(CP2 &,double);
	friend CP2 operator/=(CP2 &,double);
public:
	double x;
	double y;
	CRGB   c;
};

#endif // !defined(AFX_P2_H__E525809F_8CB8_41E2_ABF7_3909432BB73D__INCLUDED_)
