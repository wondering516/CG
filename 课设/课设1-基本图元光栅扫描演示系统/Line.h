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
	void MoveTo(CDC *,CP2);//�ƶ���ָ��λ��
	void MoveTo(CDC *,double,double,CRGB);
	void LineTo(CDC *,CP2);//����ֱ�ߣ������յ�
	void LineTo(CDC *,double,double,CRGB);
	CRGB Interpolation(double,double,double,CRGB,CRGB);//���Բ�ֵ
public:
	CP2 P0;//ֱ���������
	CP2 P1;//ֱ���յ�����
};

#endif // !defined(AFX_LINE_H__4F2B9CBA_79F9_412F_8E9F_4A7D0B14C66C__INCLUDED_)
