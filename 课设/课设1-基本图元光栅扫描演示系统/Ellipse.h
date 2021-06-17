// Ellipse.h: interface for the CEllipse class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_ELLIPSE_H__C5A218B2_AC04_40E0_A0DD_C91839408CBE__INCLUDED_)
#define AFX_ELLIPSE_H__C5A218B2_AC04_40E0_A0DD_C91839408CBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VPixel.h"

class CEllipse  :public CVPixel
{
public:
	CEllipse();
	virtual ~CEllipse();
	void DrawEllipse(CDC*,CP2,CP2);//椭圆的Bresenham算法
	void EllipsePoint(CDC*,CP2,CRGB);
private:
	CP2 P0;//外接矩形的左上角点
	CP2 P1;//外接矩形的右下角点
};

#endif // !defined(AFX_ELLIPSE_H__C5A218B2_AC04_40E0_A0DD_C91839408CBE__INCLUDED_)
