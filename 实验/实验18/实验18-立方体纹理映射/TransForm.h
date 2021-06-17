// TransForm.h: interface for the CTransForm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSFORM_H__C4FA86B2_0AF5_409A_94EB_B02A1EE13E05__INCLUDED_)
#define AFX_TRANSFORM_H__C4FA86B2_0AF5_409A_94EB_B02A1EE13E05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "P3.h"

class CTransForm  
{
public:
	CTransForm();
	virtual ~CTransForm();
	void SetMat(CP3 *p,int n);
	void Identity();
	CP3* Translate(double Tx,double Ty,double Tz);//平移变换矩阵
	CP3* Scale(double s);//整体比例变换矩阵
	CP3* Scale(double sx,double sy,double sz);//比例变换矩阵
	CP3* RotateX(double beta);//旋转变换矩阵
	CP3* RotateY(double beta);//旋转变换矩阵
	CP3* RotateZ(double beta);//旋转变换矩阵
	CP3* ReflectX();//X轴反射变换矩阵
	CP3* ReflectY();//Y轴反射变换矩阵
	CP3* ReflectZ();//Z轴反射变换矩阵
	CP3* ReflectXOY();//XOY面反射变换矩阵
	CP3* ReflectYOZ();//YOZ面反射变换矩阵
	CP3* ReflectZOX();//ZOX面反射变换矩阵
	CP3* ShearX(double d,double g);//X方向错切变换矩阵
	CP3* ShearY(double b,double h);//Y方向错切变换矩阵
	CP3* ShearZ(double c,double f);//Z方向错切变换矩阵
	void MultiMatrix();//矩阵相乘
public:
	double T[4][4];
	CP3 *POld;
	int num;
};

#endif // !defined(AFX_TRANSFORM_H__C4FA86B2_0AF5_409A_94EB_B02A1EE13E05__INCLUDED_)
