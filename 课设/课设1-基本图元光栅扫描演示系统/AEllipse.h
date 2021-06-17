// AEllipse.h: interface for the CAEllipse class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AELLIPSE_H__D8947556_550F_424E_BCBB_10DC345447B2__INCLUDED_)
#define AFX_AELLIPSE_H__D8947556_550F_424E_BCBB_10DC345447B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "VPixel.h"

class CAEllipse  :public CVPixel
{
public:
	CAEllipse();
	virtual ~CAEllipse();
	void DrawAEllipse(CDC*,CP2,CP2);//椭圆的反走样算法
	void AEllipsePointAC(CDC*,CP2,double,CRGB);//四分法画椭圆AC子函数
	void AEllipsePointBC(CDC*,CP2,double,CRGB);//四分法画椭圆BC子函数
private:
	CP2  P0;//外接矩形的左上角点
	CP2  P1;//外接矩形的右下角点
	CRGB Bc;//背景色
};

#endif // !defined(AFX_AELLIPSE_H__D8947556_550F_424E_BCBB_10DC345447B2__INCLUDED_)
