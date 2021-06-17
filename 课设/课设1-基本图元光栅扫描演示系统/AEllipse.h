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
	void DrawAEllipse(CDC*,CP2,CP2);//��Բ�ķ������㷨
	void AEllipsePointAC(CDC*,CP2,double,CRGB);//�ķַ�����ԲAC�Ӻ���
	void AEllipsePointBC(CDC*,CP2,double,CRGB);//�ķַ�����ԲBC�Ӻ���
private:
	CP2  P0;//��Ӿ��ε����Ͻǵ�
	CP2  P1;//��Ӿ��ε����½ǵ�
	CRGB Bc;//����ɫ
};

#endif // !defined(AFX_AELLIPSE_H__D8947556_550F_424E_BCBB_10DC345447B2__INCLUDED_)
