// TransForm.cpp: implementation of the CTransForm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "TransForm.h"
#include "math.h"
#define PI 3.14159
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransForm::CTransForm()
{

}

CTransForm::~CTransForm()
{

}

void CTransForm::SetMat(CP3 *p,int n)
{
	POld=p;
	num=n;
}

void CTransForm::Identity()//��λ����
{
	T[0][0]=1.0;T[0][1]=0.0;T[0][2]=0.0;T[0][3]=0.0;
	T[1][0]=0.0;T[1][1]=1.0;T[1][2]=0.0;T[1][3]=0.0;
	T[2][0]=0.0;T[2][1]=0.0;T[2][2]=1.0;T[2][3]=0.0;
	T[3][0]=0.0;T[3][1]=0.0;T[3][2]=0.0;T[3][3]=1.0;
}

CP3* CTransForm::Translate(double tx,double ty,double tz)//ƽ�Ʊ任����
{
	Identity();
	T[3][0]=tx; 
	T[3][1]=ty; 
	T[3][2]=tz; 
	MultiMatrix();
	return POld;
}

CP3* CTransForm::Scale(double s)//��������任����
{
	Identity();
	T[4][4]=s; 	
	MultiMatrix();
	return POld;
}

CP3* CTransForm::Scale(double sx,double sy,double sz)//�����任����
{
	Identity();
	T[0][0]=sx; 
	T[1][1]=sy; 
	T[2][2]=sz; 
	MultiMatrix();
	return POld;
}
CP3* CTransForm::RotateX(double beta)//��X����ת�任����
{
	Identity();
	double rad=beta*PI/180;
	T[1][1]=cos(rad); T[1][2]=sin(rad);
	T[2][1]=-sin(rad);T[2][2]=cos(rad);
	MultiMatrix();
	return POld;
}

CP3* CTransForm::RotateY(double beta)//��Y����ת�任����
{
	Identity();
	double rad=beta*PI/180;
	T[0][0]=cos(rad);T[0][2]=-sin(rad);
	T[2][0]=sin(rad);T[2][2]=cos(rad);
	MultiMatrix();
	return POld;
}

CP3* CTransForm::RotateZ(double beta)//��Z����ת�任����
{
	Identity();
	double rad=beta*PI/180;
	T[0][0]=cos(rad); T[0][1]=sin(rad);
	T[1][0]=-sin(rad);T[1][1]=cos(rad);
	MultiMatrix();
	return POld;
}
CP3 *CTransForm::ReflectX()//X�ᷴ��任����
{
	Identity();
	T[1][1]=-1;
	T[2][2]=-1;
	MultiMatrix();
	return POld;
}
CP3 *CTransForm::ReflectY()//Y�ᷴ��任����
{
	Identity();
	T[0][0]=-1;
	T[2][2]=-1;
	MultiMatrix();
	return POld;
}
CP3 *CTransForm::ReflectZ()//Z�ᷴ��任����
{
	Identity();
	T[0][0]=-1;
	T[1][1]=-1;
	MultiMatrix();
	return POld;
}
CP3 *CTransForm::ReflectXOY()//XOY�淴��任����
{
	Identity();
	T[2][2]=-1;
	MultiMatrix();
	return POld;
}
CP3 *CTransForm::ReflectYOZ()//YOZ�淴��任����
{
	Identity();
	T[0][0]=-1;
	MultiMatrix();
	return POld;
}
CP3 *CTransForm::ReflectZOX()//ZOX�淴��任����
{
	Identity();
	T[1][1]=-1;
	MultiMatrix();
	return POld;
}

CP3 *CTransForm::ShearX(double d,double g)//X������б任����
{
	Identity();
	T[1][0]=d;
	T[2][0]=g;
	MultiMatrix();
	return POld;
}

CP3 *CTransForm::ShearY(double b,double h)//Y������б任����
{
	Identity();
	T[0][1]=b;
	T[2][1]=h;
	MultiMatrix();
	return POld;
}

CP3 *CTransForm::ShearZ(double c,double f)//Z������б任����
{
	Identity();
	T[0][2]=c;
	T[1][2]=f;
	MultiMatrix();
	return POld;
}
void CTransForm::MultiMatrix()//�������
{
	CP3 *PNew=new CP3[num];	
	for(int i=0;i<num;i++)
	{
		PNew[i]=POld[i];
	}
	for(int j=0;j<num;j++)
	{
		POld[j].x=PNew[j].x*T[0][0]+PNew[j].y*T[1][0]+PNew[j].z*T[2][0]+PNew[j].w*T[3][0];
		POld[j].y=PNew[j].x*T[0][1]+PNew[j].y*T[1][1]+PNew[j].z*T[2][1]+PNew[j].w*T[3][1];
		POld[j].z=PNew[j].x*T[0][2]+PNew[j].y*T[1][2]+PNew[j].z*T[2][2]+PNew[j].w*T[3][2];
		POld[j].w=PNew[j].x*T[0][3]+PNew[j].y*T[1][3]+PNew[j].z*T[2][3]+PNew[j].w*T[3][3];
	}
	delete []PNew;
}
