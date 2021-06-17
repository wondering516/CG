// Lighting.cpp: implementation of the CLighting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Lighting.h"
#include "cmath"
#define  PI  3.14159265
#define  MIN(a,b)  ((a<b)?(a):(b))
#define  MAX(a,b)  ((a>b)?(a):(b))
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLighting::CLighting()
{
	LightNum=1;
	Light=new CLight[LightNum];
	Ambient=CRGB(0.3,0.3,0.3);//������㶨����
}

CLighting::~CLighting()
{
	if(Light)
	{
		delete []Light;
		Light=NULL;
	}
}
void CLighting::SetLightNumber(int lnum)
{
	if(Light)
	{
		delete []Light;
	}
	LightNum=lnum;
	Light=new CLight[lnum];
}

CLighting::CLighting(int lnum)
{
	LightNum=lnum;
	Light=new CLight[lnum];
	Ambient=CRGB(0.3,0.3,0.3);
}

CRGB CLighting::Lighting(CP3 ViewPoint,CP3 Point,CVector Normal,CMaterial *pMaterial)
{	
	CRGB LastC=pMaterial->M_Emit;//��������ɢɫΪ��ʼֵ	
	for(int i=0;i<LightNum;i++)//���Թ�Դ
	{	
		if(Light[i].L_OnOff)
		{		
			CRGB InitC;
			InitC.red=0,InitC.green=0,InitC.blue=0;
			CVector VL(Point,Light[i].L_Position);//ָ���Դ��ʸ��
			double d=VL.Mold();//�⴫���ľ��룬����ʸ��VL��ģ
			VL=VL.Unit();//��ʸ����λ��
			CVector VN=Normal;
			VN=VN.Unit();//��ʸ����λ��			
			double CosTheta=Dot(VL,VN);
			if(CosTheta>=0.0)//���߿������䵽����
			{
				//��1���������������
				InitC.red+=Light[i].L_Diffuse.red*pMaterial->M_Diffuse.red*CosTheta;
				InitC.green+=Light[i].L_Diffuse.green*pMaterial->M_Diffuse.green*CosTheta;
				InitC.blue+=Light[i].L_Diffuse.blue*pMaterial->M_Diffuse.blue*CosTheta;
				//��2�������뾵�淴���
				CVector VS(Point,ViewPoint);//VS��ʸ��
				VS=VS.Unit();
				CVector VH=(VL+VS)/(VL+VS).Mold();//ƽ��ʸ��	
				double nRV=pow(Dot(VH,VN),pMaterial->M_Exp);
				InitC.red+=Light[i].L_Specular.red*pMaterial->M_Specular.red*nRV;
				InitC.green+=Light[i].L_Specular.green*pMaterial->M_Specular.green*nRV;
				InitC.blue+=Light[i].L_Specular.blue*pMaterial->M_Specular.blue*nRV;	
			}
			//��3������ǿ˥��
			double c0=Light[i].L_C0;
			double c1=Light[i].L_C1;
			double c2=Light[i].L_C2;			
			double f=(1.0/(c0+c1*d+c2*d*d));//����˥������
			f=MIN(1.0,f);		
			LastC+=InitC*f;		
		}
		else
		{
			LastC=Point.c;
		}		
	}
	//��4�������뻷����
	LastC+=Ambient*pMaterial->M_Ambient;
	//��5������ɫ��һ��[0,1]����
	LastC.Normalize();		
	//��6�������������㶥��Ĺ�ǿ��ɫ
	return LastC;
}