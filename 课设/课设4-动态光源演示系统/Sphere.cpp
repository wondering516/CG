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
	for(int n=0;n<N1;n++)//注意撤销次序,先列后行,与设置相反
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

void CSphere::ReadPoint()//读入点坐标
{
	int gafa=10,gbeta=10;//面片夹角
	N1=180/gafa,N2=360/gbeta;//N1为纬度区间,N2为经度区间
	if (P==NULL)
	{
		P=new CP3[(N1-1)*N2+2];//纬度方向除南北极点外有“N1－1”个点，“2”代表南北极两点
	}
	double afa1,beta1;
	P[0].x=0;
	P[0].y=r;
	P[0].z=0;
	//按行循环计算球体上的点坐标
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
	//计算南极点坐标
	P[(N1-1)*N2+1].x=0;
	P[(N1-1)*N2+1].y=-r;
	P[(N1-1)*N2+1].z=0;	
}

void CSphere::ReadFace()//读入面表
{
	//设置二维动态数组
	if (F==NULL)
	{
		F=new CFace*[N1];//设置行
		memset(F, 0, sizeof(CFace*)*N1);
	}
	for(int n=0;n<N1;n++)
	{
		if (F[n]==NULL)
		{
			F[n]=new CFace[N2];//设置列
		}
	}
	for(int j=0;j<N2;j++)//构造北极三角形面片
	{
		int tempj=j+1;
		if(tempj==N2) tempj=0;//面片的首尾连接
		int NorthIndex[3];//北极三角形面片索引号数组
		NorthIndex[0]=0;
		NorthIndex[1]=j+1;
		NorthIndex[2]=tempj+1;
		F[0][j].SetEN(3);
		for(int k=0;k<F[0][j].En;k++)//传入面中点的索引
		{
			F[0][j].p[k]=NorthIndex[k];
		}
	}
	for(int i=1;i<N1-1;i++)//构造球体四边形面片
	{
		for(int j=0;j<N2;j++)
	    {
			int tempi=i+1;
			int tempj=j+1;
			if(tempj==N2) tempj=0;
			int BodyIndex[4];//球体四边形面片索引号数组
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
	for(j=0;j<N2;j++)//构造南极三角形面片
	{
		int tempj=j+1;
		if(tempj==N2) tempj=0;
		int SouthIndex[3];//南极三角形面片索引号数组
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
	ViewPoint.x=R*k[6];//用户坐标系的视点球坐标
	ViewPoint.y=R*k[4];
	ViewPoint.z=R*k[5];
}

void CSphere::PerProject(CP3 P)
{
	CP3 ViewP;
	ViewP.x=P.x*k[3]-P.z*k[1];//观察坐标系三维坐标
	ViewP.y=-P.x*k[8]+P.y*k[2]-P.z*k[7];
	ViewP.z=-P.x*k[6]-P.y*k[4]-P.z*k[5]+R;
	ViewP.c=P.c;
    ScreenP.x=d*ViewP.x/ViewP.z;//屏幕坐标系三维坐标
	ScreenP.y=ROUND(d*ViewP.y/ViewP.z);
	ScreenP.z=d*(R-ViewP.z)/ViewP.z;
	ScreenP.c=ViewP.c;
}

void CSphere::DrawObject(CDC* pDC,CLighting *pLight,CMaterial *pMaterial,CZBuffer *zbuf,int type)//绘制球体
{
	for(int n=0;n<(N1-1)*N2+2;n++)//遍历所有点
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
			P[n].c=pLight->Lighting(ViewPoint,P[n],PNormal,pMaterial);//调用光照函数计算顶点颜色
		}
	}
	CPi3 Point3[3];//南北极顶点数组
	CPi3 Point4[4];//球体顶点数组	
	for(int i=0;i<N1;i++)
	{
		for(int j=0;j<N2;j++)
		{
			CVector VS(P[F[i][j].p[1]],ViewPoint);//面的视矢量
			F[i][j].SetNormal(P[F[i][j].p[0]],P[F[i][j].p[1]],P[F[i][j].p[2]]);
			if(Dot(VS,F[i][j].Normal)>=0)//背面剔除
			{				
				if(F[i][j].En==3)//三角形面片
				{
					for(int m=0;m<F[i][j].En;m++)
					{
						PerProject(P[F[i][j].p[m]]);
						Point3[m]=ScreenP;
					}
					zbuf->SetPoint(Point3,3);//设置顶点			
					zbuf->CreateBucket();//建立桶表
					zbuf->CreateEdge();//建立边表
					zbuf->Gouraud(pDC);//填充三角形	
					zbuf->ClearMemory();//内存清理					
				}
				else//四边形面片
				{
					for(int m=0;m<F[i][j].En;m++)
					{
						PerProject(P[F[i][j].p[m]]);
						Point4[m]=ScreenP;						
					}
					zbuf->SetPoint(Point4,4);//设置顶点			
					zbuf->CreateBucket();//建立桶表
					zbuf->CreateEdge();//建立边表
					zbuf->Gouraud(pDC);//填充四边形	
					zbuf->ClearMemory();//内存清理
				}
			} 	
		}	
	}
}