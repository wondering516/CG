// Line.h: interface for the CLine class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_LINE_H__4F2B9CBA_79F9_412F_8E9F_4A7D0B14C66C__INCLUDED_)
#define AFX_LINE_H__4F2B9CBA_79F9_412F_8E9F_4A7D0B14C66C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VPixel.h"

class CLine  :public CVPixel
{
public:
	CLine();
	virtual ~CLine();
	void MoveTo(CDC *,CP2);//移动到指定位置
	void MoveTo(CDC *,double,double,CRGB);
	void LineTo(CDC *,CP2);//绘制直线，不画终点
	void LineTo(CDC *,double,double,CRGB);
	CRGB Interpolation(double,double,double,CRGB,CRGB);//线性插值
public:
	CP2 P0;//直线起点坐标
	CP2 P1;//直线终点坐标
};

#endif // !defined(AFX_LINE_H__4F2B9CBA_79F9_412F_8E9F_4A7D0B14C66C__INCLUDED_)
