// Sphere.cpp: implementation of the CSphere class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Sphere.h"
#include "cmath"
#define  PI 3.1415926
#define  ROUND(x)  int(x+0.5)
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSphere::CSphere()
{
	P = NULL;
	r=0;
	F = NULL;
}

CSphere::~CSphere()
{
	if(P!=NULL)
	{
		delete[] P;
		P=NULL;
	}
	for(int n=0;n<N1;n++)//ע�⳷������,���к���,�������෴
	{
		if (F[n] != NULL)
		{
			delete[] F[n];
		}
	}
	delete[] F;
	F=NULL;
}

void CSphere::SetSize(int r)
{
   this->r=r;
   R=1000,d=800,Phi=90.0,Theta=0.0;
   ReadPoint();
   ReadFace();
   InitParameter();
}

void CSphere::ReadPoint()//���������
{
	int gafa=10,gbeta=10;//��Ƭ�н�
	N1=180/gafa,N2=360/gbeta;//N1Ϊγ������,N2Ϊ��������
	if (P==NULL)
	{
		P=new CP3[(N1-1)*N2+2];//γ�ȷ�����ϱ��������С�N1��1�����㣬��2�������ϱ�������
	}
	double afa1,beta1;
	P[0].x=0;
	P[0].y=r;
	P[0].z=0;
	//����ѭ�����������ϵĵ�����
	for(int i=0;i<N1-1;i++)
	{
		afa1=(i+1)*gafa*PI/180;
		for(int j=0;j<N2;j++)
		{
			beta1=j*gbeta*PI/180;		
			P[i*N2+j+1].x=r*sin(afa1)*sin(beta1);
			P[i*N2+j+1].y=r*cos(afa1);	
			P[i*N2+j+1].z=r*sin(afa1)*cos(beta1);							
		}
	}
	//�����ϼ�������
	P[(N1-1)*N2+1].x=0;
	P[(N1-1)*N2+1].y=-r;
	P[(N1-1)*N2+1].z=0;	
}

void CSphere::ReadFace()//�������
{
	//���ö�ά��̬����
	if (F==NULL)
	{
		F=new CFace*[N1];//������
		memset(F, 0, sizeof(CFace*)*N1);
	}
	for(int n=0;n<N1;n++)
	{
		if (F[n]==NULL)
		{
			F[n]=new CFace[N2];//������
		}
	}
	for(int j=0;j<N2;j++)//���챱����������Ƭ
	{
		int tempj=j+1;
		if(tempj==N2) tempj=0;//��Ƭ����β����
		int NorthIndex[3];//������������Ƭ����������
		NorthIndex[0]=0;
		NorthIndex[1]=j+1;
		NorthIndex[2]=tempj+1;
		F[0][j].SetEN(3);
		for(int k=0;k<F[0][j].En;k++)//�������е������
		{
			F[0][j].p[k]=NorthIndex[k];
		}
	}
	for(int i=1;i<N1-1;i++)//���������ı�����Ƭ
	{
		for(int j=0;j<N2;j++)
	    {
			int tempi=i+1;
			int tempj=j+1;
			if(tempj==N2) tempj=0;
			int BodyIndex[4];//�����ı�����Ƭ����������
			BodyIndex[0]=(i-1)*N2+j+1;
			BodyIndex[1]=(tempi-1)*N2+j+1;
			BodyIndex[2]=(tempi-1)*N2+tempj+1;
			BodyIndex[3]=(i-1)*N2+tempj+1;
			F[i][j].SetEN(4);
			for(int k=0;k<F[i][j].En;k++)
			{
				F[i][j].p[k]=BodyIndex[k];
			}			
		}
	}
	for(j=0;j<N2;j++)//�����ϼ���������Ƭ
	{
		int tempj=j+1;
		if(tempj==N2) tempj=0;
		int SouthIndex[3];//�ϼ���������Ƭ����������
		SouthIndex[0]=(N1-2)*N2+j+1;
		SouthIndex[1]=(N1-1)*N2+1;
		SouthIndex[2]=(N1-2)*N2+tempj+1;
		F[N1-1][j].SetEN(3);	
		for(int k=0;k<F[N1-1][j].En;k++)
		{
			F[N1-1][j].p[k]=SouthIndex[k];
		}	
	}
}

void CSphere::InitParameter()
{
	k[1]=sin(PI*Theta/180);
	k[2]=sin(PI*Phi/180);
	k[3]=cos(PI*Theta/180);
	k[4]=cos(PI*Phi/180);
	k[5]=k[2]*k[3];
	k[6]=k[2]*k[1];
	k[7]=k[4]*k[3];
	k[8]=k[4]*k[1];
	ViewPoint.x=R*k[6];//�û�����ϵ���ӵ�������
	ViewPoint.y=R*k[4];
	ViewPoint.z=R*k[5];
}

void CSphere::PerProject(CP3 P)
{
	CP3 ViewP;
	ViewP.x=P.x*k[3]-P.z*k[1];//�۲�����ϵ��ά����
	ViewP.y=-P.x*k[8]+P.y*k[2]-P.z*k[7];
	ViewP.z=-P.x*k[6]-P.y*k[4]-P.z*k[5]+R;
	ViewP.c=P.c;
    ScreenP.x=d*ViewP.x/ViewP.z;//��Ļ����ϵ��ά����
	ScreenP.y=ROUND(d*ViewP.y/ViewP.z);
	ScreenP.z=d*(R-ViewP.z)/ViewP.z;
	ScreenP.c=ViewP.c;
}

void CSphere::DrawObject(CDC* pDC,CLighting *pLight,CMaterial *pMaterial,CZBuffer *zbuf,int type)//��������
{
	for(int n=0;n<(N1-1)*N2+2;n++)//�������е�
	{
		CVector PNormal(P[n]);
		switch(type)
		{
		case REDG:
			P[n].c=CRGB(1.0,0.0,0.0);
			break;
		case GREENG:
			P[n].c=CRGB(0.0,1.0,0.0);
			break;
		case BLUEG:
			P[n].c=CRGB(0.0,0.0,1.0);
			break;
		default:
			P[n].c=pLight->Lighting(ViewPoint,P[n],PNormal,pMaterial);//���ù��պ������㶥����ɫ
		}
	}
	CPi3 Point3[3];//�ϱ�����������
	CPi3 Point4[4];//���嶥������	
	for(int i=0;i<N1;i++)
	{
		for(int j=0;j<N2;j++)
		{
			CVector VS(P[F[i][j].p[1]],ViewPoint);//�����ʸ��
			F[i][j].SetNormal(P[F[i][j].p[0]],P[F[i][j].p[1]],P[F[i][j].p[2]]);
			if(Dot(VS,F[i][j].Normal)>=0)//�����޳�
			{				
				if(F[i][j].En==3)//��������Ƭ
				{
					for(int m=0;m<F[i][j].En;m++)
					{
						PerProject(P[F[i][j].p[m]]);
						Point3[m]=ScreenP;
					}
					zbuf->SetPoint(Point3,3);//���ö���			
					zbuf->CreateBucket();//����Ͱ��
					zbuf->CreateEdge();//�����߱�
					zbuf->Gouraud(pDC);//���������	
					zbuf->ClearMemory();//�ڴ�����					
				}
				else//�ı�����Ƭ
				{
					for(int m=0;m<F[i][j].En;m++)
					{
						PerProject(P[F[i][j].p[m]]);
						Point4[m]=ScreenP;						
					}
					zbuf->SetPoint(Point4,4);//���ö���			
					zbuf->CreateBucket();//����Ͱ��
					zbuf->CreateEdge();//�����߱�
					zbuf->Gouraud(pDC);//����ı���	
					zbuf->ClearMemory();//�ڴ�����
				}
			} 	
		}	
	}
}