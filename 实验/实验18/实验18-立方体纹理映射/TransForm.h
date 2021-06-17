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
	CP3* Translate(double Tx,double Ty,double Tz);//ƽ�Ʊ任����
	CP3* Scale(double s);//��������任����
	CP3* Scale(double sx,double sy,double sz);//�����任����
	CP3* RotateX(double beta);//��ת�任����
	CP3* RotateY(double beta);//��ת�任����
	CP3* RotateZ(double beta);//��ת�任����
	CP3* ReflectX();//X�ᷴ��任����
	CP3* ReflectY();//Y�ᷴ��任����
	CP3* ReflectZ();//Z�ᷴ��任����
	CP3* ReflectXOY();//XOY�淴��任����
	CP3* ReflectYOZ();//YOZ�淴��任����
	CP3* ReflectZOX();//ZOX�淴��任����
	CP3* ShearX(double d,double g);//X������б任����
	CP3* ShearY(double b,double h);//Y������б任����
	CP3* ShearZ(double c,double f);//Z������б任����
	void MultiMatrix();//�������
public:
	double T[4][4];
	CP3 *POld;
	int num;
};

#endif // !defined(AFX_TRANSFORM_H__C4FA86B2_0AF5_409A_94EB_B02A1EE13E05__INCLUDED_)
