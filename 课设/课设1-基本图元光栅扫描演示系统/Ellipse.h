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
	void DrawEllipse(CDC*,CP2,CP2);//��Բ��Bresenham�㷨
	void EllipsePoint(CDC*,CP2,CRGB);
private:
	CP2 P0;//��Ӿ��ε����Ͻǵ�
	CP2 P1;//��Ӿ��ε����½ǵ�
};

#endif // !defined(AFX_ELLIPSE_H__C5A218B2_AC04_40E0_A0DD_C91839408CBE__INCLUDED_)
