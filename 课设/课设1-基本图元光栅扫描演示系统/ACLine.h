// ACLine.h: interface for the CACLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACLINE_H__3CF9AC7A_4E39_4A1C_99D0_95B37C755002__INCLUDED_)
#define AFX_ACLINE_H__3CF9AC7A_4E39_4A1C_99D0_95B37C755002__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VPixel.h"

class CACLine :public CVPixel 
{
public:
	CACLine();
	CACLine(CRGB);
	virtual ~CACLine();
	void MoveTo(CDC *,CP2);//移动到指定位置
	void MoveTo(CDC *,double,double,CRGB);
	void LineTo(CDC *,CP2);//绘制直线，不含终点
	void LineTo(CDC *,double,double,CRGB);
	CRGB Interpolation(double,double,double,CRGB,CRGB);//线性插值
public:
	CP2 P0;//起点
	CP2 P1;//终点
	CRGB Bc;//背景色
};

#endif // !defined(AFX_ACLINE_H__3CF9AC7A_4E39_4A1C_99D0_95B37C755002__INCLUDED_)
