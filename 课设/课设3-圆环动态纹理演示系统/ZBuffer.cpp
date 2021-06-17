// ZBuffer.cpp: implementation of the CZBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "ZBuffer.h"
#define ROUND(a) int(a+0.5)
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CZBuffer::CZBuffer()
{
	P=NULL;
	VN=NULL;
	UV=NULL;
	pHeadE=NULL;
	pCurrentB=NULL;
	pEdge=NULL;
	pCurrentE=NULL;
	pHeadB=NULL;
	ZB=NULL;
}

CZBuffer::~CZBuffer()
{
	for(int i=0;i<Width;i++)
	{
		delete[] ZB[i];
		ZB[i]=NULL;
	}
	if(ZB!=NULL)
	{
		delete ZB;
		ZB=NULL;
	}	
	ClearMemory();
}

void CZBuffer::SetPoint(CPi3 *p,CVector *vn,int m)
{
	P=new CPi3[m];
	VN=new CVector[m];
    for(int i=0;i<m;i++)
	{
		P[i]=p[i];	
		VN[i]=vn[i];
	}
	PNum=m;
}
void CZBuffer::SetPoint(CPi3 *p,int m)
{
	
	if(P!=NULL)
	{
		delete []P;
		P=NULL;
	}
	P=new CPi3[m];
	for(int i=0;i<m;i++)
	{
		P[i]=p[i];	
	}
	PNum=m;	
}
void CZBuffer::SetPoint(CPi3 *p,CVector *vn,CP2 *uv,int m)
{
	P=new CPi3[m];
	VN=new CVector[m];
	UV=new CP2[m];
    for(int i=0;i<m;i++)
	{
		P[i]=p[i];	
		VN[i]=vn[i];
		UV[i]=uv[i];
	}
	PNum=m;
}
void CZBuffer::CreateBucket()//创建桶表
{
	int yMin,yMax;
	yMin=yMax=P[0].y;
	for(int i=0;i<PNum;i++)//查找多边形所覆盖的最小和最大扫描线
	{
		if(P[i].y<yMin)
		{
			yMin=P[i].y;//扫描线的最小值
		}
		if(P[i].y>yMax)
		{
			yMax=P[i].y;//扫描线的最大值
		}
	}
	for(int y=yMin;y<=yMax;y++)
	{
		if(yMin==y)//建立桶头结点
		{
			pHeadB=new CBucket;//建立桶的头结点
			pCurrentB=pHeadB;//pCurrentB为CBucket当前结点指针
			pCurrentB->ScanLine=yMin;
			pCurrentB->pET=NULL;//没有链接边表
			pCurrentB->next=NULL;
		}
		else//其他扫描线
		{
			pCurrentB->next=new CBucket;//建立桶的其他结点
			pCurrentB=pCurrentB->next;
			pCurrentB->ScanLine=y;
			pCurrentB->pET=NULL;
			pCurrentB->next=NULL;
		}
	}
}

void CZBuffer::CreateEdgeforPhong()//创建边表
{
	for(int i=0;i<PNum;i++)
	{
		pCurrentB=pHeadB;
		int j=(i+1)%PNum;//边的第二个顶点，P[i]和P[j]构成边
		if(P[i].y<P[j].y)//边的终点比起点高
		{
			pEdge=new CAET;
			pEdge->x=P[i].x;//计算ET表的值
			pEdge->yMax=P[j].y;
			pEdge->k=(P[j].x-P[i].x)/(P[j].y-P[i].y);//代表1/k
			pEdge->pb=P[i];//绑定顶点和颜色	
			pEdge->pe=P[j];
			pEdge->vb=VN[i];
			pEdge->ve=VN[j];
			pEdge->next=NULL;
			while(pCurrentB->ScanLine!=P[i].y)//在桶内寻找该边的yMin
			{
				pCurrentB=pCurrentB->next;//移到yMin所在的桶结点
			}		
		}
		if(P[j].y<P[i].y)//边的终点比起点低
		{
			pEdge=new CAET;
			pEdge->x=P[j].x;
			pEdge->yMax=P[i].y;
			pEdge->k=(P[i].x-P[j].x)/(P[i].y-P[j].y);
			pEdge->pb=P[i];
			pEdge->pe=P[j];
			pEdge->vb=VN[i];
			pEdge->ve=VN[j];
			pEdge->next=NULL;
			while(pCurrentB->ScanLine!=P[j].y)
			{
				pCurrentB=pCurrentB->next;
			}
		}
		if(int(P[j].y)!=P[i].y)
		{
			pCurrentE=pCurrentB->pET;
			if(pCurrentE==NULL)
			{
				pCurrentE=pEdge;
				pCurrentB->pET=pCurrentE;
			}
			else
			{
				while(pCurrentE->next!=NULL)
				{
					pCurrentE=pCurrentE->next;
				}
				pCurrentE->next=pEdge;
			}
		}
	}
}
void CZBuffer::CreateEdgeforGouraud()
{
	for(int i=0;i<PNum;i++)
	{
		pCurrentB=pHeadB;
		int j=(i+1)%PNum;//边的第二个顶点，P[i]和P[j]构成边
		if(P[i].y<P[j].y)//边的终点比起点高
		{
			pEdge=new CAET;
			pEdge->x=P[i].x;//计算ET表的值
			pEdge->yMax=P[j].y;
			pEdge->k=(P[j].x-P[i].x)/(P[j].y-P[i].y);//代表1/k
			pEdge->pb=P[i];//绑定顶点和颜色
			pEdge->pe=P[j];
			pEdge->next=NULL;
			while(pCurrentB->ScanLine!=P[i].y)//在桶内寻找该边的yMin
			{
				pCurrentB=pCurrentB->next;//移到yMin所在的桶结点
			}		
		}
		if(P[j].y<P[i].y)//边的终点比起点低
		{
			pEdge=new CAET;
			pEdge->x=P[j].x;
			pEdge->yMax=P[i].y;
			pEdge->k=(P[i].x-P[j].x)/(P[i].y-P[j].y);
			pEdge->pb=P[i];
			pEdge->pe=P[j];
			pEdge->next=NULL;
			while(pCurrentB->ScanLine!=P[j].y)
			{
				pCurrentB=pCurrentB->next;
			}
		}
		if(int(P[j].y)!=P[i].y)
		{
			pCurrentE=pCurrentB->pET;
			if(pCurrentE==NULL)
			{
				pCurrentE=pEdge;
				pCurrentB->pET=pCurrentE;
			}
			else
			{
				while(pCurrentE->next!=NULL)
				{
					pCurrentE=pCurrentE->next;
				}
				pCurrentE->next=pEdge;
			}
		}
	}
}
void CZBuffer::CreateEdgeforTexture()
{
	for(int i=0;i<PNum;i++)
	{
		pCurrentB=pHeadB;
		int j=(i+1)%PNum;//边的第二个顶点，P[i]和P[j]构成边
		if(P[i].y<P[j].y)//边的终点比起点高
		{
			pEdge=new CAET;
			pEdge->x=P[i].x;//计算ET表的值
			pEdge->yMax=P[j].y;
			pEdge->k=(P[j].x-P[i].x)/(P[j].y-P[i].y);//代表1/k
			pEdge->pb=P[i];//绑定顶点和颜色
			pEdge->pe=P[j];
			pEdge->vb=VN[i];//绑定顶点法矢量
			pEdge->ve=VN[j];
			pEdge->uvb=UV[i];
			pEdge->uve=UV[j];
			pEdge->next=NULL;
			while(pCurrentB->ScanLine!=P[i].y)//在桶内寻找该边的yMin
			{
				pCurrentB=pCurrentB->next;//移到yMin所在的桶结点
			}		
		}
		if(P[j].y<P[i].y)//边的终点比起点低
		{
			pEdge=new CAET;
			pEdge->x=P[j].x;
			pEdge->yMax=P[i].y;
			pEdge->k=(P[i].x-P[j].x)/(P[i].y-P[j].y);
			pEdge->pb=P[i];
			pEdge->pe=P[j];
			pEdge->vb=VN[i];//绑定顶点法矢量
			pEdge->ve=VN[j];
			pEdge->uvb=UV[i];
			pEdge->uve=UV[j];
			pEdge->next=NULL;
			while(pCurrentB->ScanLine!=P[j].y)
			{
				pCurrentB=pCurrentB->next;
			}
		}
		if(int(P[j].y)!=P[i].y)
		{
			pCurrentE=pCurrentB->pET;
			if(pCurrentE==NULL)
			{
				pCurrentE=pEdge;
				pCurrentB->pET=pCurrentE;
			}
			else
			{
				while(pCurrentE->next!=NULL)
				{
					pCurrentE=pCurrentE->next;
				}
				pCurrentE->next=pEdge;
			}
		}
	}
}
void CZBuffer::Phong(CDC *pDC,CP3 ViewPoint,CLighting *pLight,CMaterial *pMaterial)
{
	double	z=0.0;//当前扫描线的z
	double	zStep=0.0;//当前扫描线随着x增长的z步长
	double	A,B,C,D;//平面方程Ax+By+Cz＋D=0的系数
	CVector V01(P[0],P[1]),V12(P[1],P[2]);
	CVector VN=V01*V12;
	A=VN.x;B=VN.y;C=VN.z;
	D=-A*P[1].x-B*P[1].y-C*P[1].z;
	//计算curDeep;从x=xMin开始计算，此时针对yi
	zStep=-A/C;//计算直线z增量
	CAET *pT1,*pT2;
	pHeadE=NULL;	
	for(pCurrentB=pHeadB;pCurrentB!=NULL;pCurrentB=pCurrentB->next)
	{
		for(pCurrentE=pCurrentB->pET;pCurrentE!=NULL;pCurrentE=pCurrentE->next)
		{
			pEdge=new CAET;
			pEdge->x=pCurrentE->x;
			pEdge->yMax=pCurrentE->yMax;
			pEdge->k=pCurrentE->k;
			pEdge->pb=pCurrentE->pb;
			pEdge->pe=pCurrentE->pe;
			pEdge->vb=pCurrentE->vb;
			pEdge->ve=pCurrentE->ve;
			pEdge->next=NULL;
			AddEt(pEdge);
		}
		EtOrder();	
		pT1=pHeadE;
		if(pT1==NULL)
		{
			return;
		}
		while(pCurrentB->ScanLine>=pT1->yMax)//下闭上开
		{
			CAET * pAETTEmp=pT1;			
			pT1=pT1->next;
			delete pAETTEmp;
			pHeadE=pT1;
			if(pHeadE==NULL)
				return;
		}
		if(pT1->next!=NULL)
		{
			pT2=pT1;
			pT1=pT2->next;
		}
		while(pT1!=NULL)
		{
			if(pCurrentB->ScanLine>=pT1->yMax)//下闭上开
			{
				CAET* pAETTemp =pT1;
				pT2->next=pT1->next;				
				pT1=pT2->next;
				delete pAETTemp;
			}
			else
			{
				pT2=pT1;
				pT1=pT2->next;
			}
		}
		CVector na,nb,nf;//na、nb代表边上任意点的法矢量，nf代表面上任意点的法矢量
		na=Interpolation(pCurrentB->ScanLine,pHeadE->pb.y,pHeadE->pe.y,pHeadE->vb,pHeadE->ve);
		nb=Interpolation(pCurrentB->ScanLine,pHeadE->next->pb.y,pHeadE->next->pe.y,pHeadE->next->vb,pHeadE->next->ve);
		BOOL Flag=FALSE;
		double xb,xe;//扫描线和有效边相交区间的起点和终点坐标
		for(pT1=pHeadE;pT1!=NULL;pT1=pT1->next)
		{
			if(Flag==FALSE)
			{
				xb=pT1->x;
				z=-(xb*A+pCurrentB->ScanLine*B+D)/C;//z=-(Ax+By-D)/C
				Flag=TRUE;
			}
			else
			{
				xe=pT1->x;
				for(double x=xb;x<xe;x++)//左闭右开
				{
					nf=Interpolation(x,xb,xe,na,nb);	
					CRGB c=pLight->Lighting(ViewPoint,CP3(ROUND(x),pCurrentB->ScanLine,z),nf,pMaterial);
					if(z>=ZB[ROUND(x)+Width/2][pCurrentB->ScanLine+Height/2])//如果新采样点的深度大于原采样点的深度
					{
					   ZB[ROUND(x)+Width/2][pCurrentB->ScanLine+Height/2]=z;//xy坐标与数组下标保持一致
					   pDC->SetPixel(ROUND(x),pCurrentB->ScanLine,RGB(c.red*255,c.green*255,c.blue*255));
					}
						z+=zStep;				
				}
				Flag=FALSE;
			}
		}
		for(pT1=pHeadE;pT1!=NULL;pT1=pT1->next)//边的连续性
		{
			pT1->x=pT1->x+pT1->k;
		}		
	}	
}
void CZBuffer::Gouraud(CDC *pDC)
{
	double	CurDeep=0.0;//当前扫描线的深度
	double	DeepStep=0.0;//当前扫描线随着x增长的深度步长
	double	A,B,C,D;//平面方程Ax+By+Cz＋D=0的系数
	CVector V21(P[1],P[2]),V10(P[0],P[1]);
	CVector VN=V21*V10;
	A=VN.x;B=VN.y;C=VN.z;
	D=-A*P[1].x-B*P[1].y-C*P[1].z;
	DeepStep=-A/C;//计算扫描线深度步长增量
	CAET *pT1,*pT2;
	pHeadE=NULL;	
	for(pCurrentB=pHeadB;pCurrentB!=NULL;pCurrentB=pCurrentB->next)
	{
		for(pCurrentE=pCurrentB->pET;pCurrentE!=NULL;pCurrentE=pCurrentE->next)
		{
			pEdge=new CAET;
			pEdge->x=pCurrentE->x;
			pEdge->yMax=pCurrentE->yMax;
			pEdge->k=pCurrentE->k;
			pEdge->pb=pCurrentE->pb;
			pEdge->pe=pCurrentE->pe;
			pEdge->next=NULL;
			AddEt(pEdge);
		}
		EtOrder();	
		pT1=pHeadE;
		if(pT1==NULL)
		{
			return;
		}
		while(pCurrentB->ScanLine>=pT1->yMax)//下闭上开
		{
			CAET * pAETTEmp=pT1;			
			pT1=pT1->next;
			delete pAETTEmp;
			pHeadE=pT1;
			if(pHeadE==NULL)
				return;
		}
		if(pT1->next!=NULL)
		{
			pT2=pT1;
			pT1=pT2->next;
		}
		while(pT1!=NULL)
		{
			if(pCurrentB->ScanLine>=pT1->yMax)//下闭上开
			{
				CAET* pAETTemp =pT1;
				pT2->next=pT1->next;				
				pT1=pT2->next;
				delete pAETTemp;
			}
			else
			{
				pT2=pT1;
				pT1=pT2->next;
			}
		}
		CRGB Ca,Cb,Cf;//Ca、Cb代边上任意点的颜色，Cf代表面上任意点的颜色
		Ca=Interpolation(pCurrentB->ScanLine,pHeadE->pb.y,pHeadE->pe.y,pHeadE->pb.c,pHeadE->pe.c);
		Cb=Interpolation(pCurrentB->ScanLine,pHeadE->next->pb.y,pHeadE->next->pe.y,pHeadE->next->pb.c,pHeadE->next->pe.c);
		BOOL Flag=FALSE;
		double xb,xe;//扫描线和有效边相交区间的起点和终点坐标
		for(pT1=pHeadE;pT1!=NULL;pT1=pT1->next)
		{
			if(Flag==FALSE)
			{
				xb=pT1->x;
				CurDeep=-(xb*A+pCurrentB->ScanLine*B+D)/C;//z=-(Ax+By-D)/C
				Flag=TRUE;
			}
			else
			{
				xe=pT1->x;
				for(double x=xb;x<xe;x++)//左闭右开
				{
					Cf=Interpolation(x,xb,xe,Ca,Cb);
					if(CurDeep>=ZB[ROUND(x)+Width/2][pCurrentB->ScanLine+Height/2])//如果新采样点的深度大于原采样点的深度
					{
						ZB[ROUND(x)+Width/2][pCurrentB->ScanLine+Height/2]=CurDeep;//xy坐标与数组下标保持一致
						pDC->SetPixel(ROUND(x),pCurrentB->ScanLine,RGB(Cf.red*255,Cf.green*255,Cf.blue*255));
					}
					CurDeep+=DeepStep;					
				}
				Flag=FALSE;
			}
		}
		for(pT1=pHeadE;pT1!=NULL;pT1=pT1->next)//边的连续性
		{
			pT1->x=pT1->x+pT1->k;
		}		
	}	
}
void CZBuffer::TextureMap(CDC *pDC,CZBuffer *pZbuf,BYTE *Image,BITMAP *bm)
{
    double	CurDeep=0.0;//当前扫描线的深度
	double	DeepStep=0.0;//当前扫描线随着x增长的深度步长
	double	A,B,C,D;//平面方程Ax+By+Cz＋D=0的系数
	CVector V21(P[1],P[2]),V10(P[0],P[1]);
	CVector VN=V21*V10;
	A=VN.x;B=VN.y;C=VN.z;
	D=-A*P[1].x-B*P[1].y-C*P[1].z;
	//计算curDeep;从x=xMin开始计算，此时针对yi
	DeepStep=-A/C;//计算直线deep增量
	CAET *T1,*T2;
	pHeadE=NULL;	
	for(pCurrentB=pHeadB;pCurrentB!=NULL;pCurrentB=pCurrentB->next)
	{
		for(pCurrentE=pCurrentB->pET;pCurrentE!=NULL;pCurrentE=pCurrentE->next)
		{
			pEdge=new CAET;
			pEdge->x=pCurrentE->x;
			pEdge->yMax=pCurrentE->yMax;
			pEdge->k=pCurrentE->k;
			pEdge->pb=pCurrentE->pb;
			pEdge->pe=pCurrentE->pe;
			pEdge->uvb=pCurrentE->uvb;
			pEdge->uve=pCurrentE->uve;
			pEdge->vb=pCurrentE->vb;
			pEdge->ve=pCurrentE->ve;
			pEdge->next=NULL;
			AddEt(pEdge);
		}
		EtOrder();	
		T1=pHeadE;
		if(T1==NULL)
		{
			return;
		}
		while(pCurrentB->ScanLine>=T1->yMax)//下闭上开
		{
			CAET * pAETTEmp = T1;			
			T1=T1->next;
			delete pAETTEmp;
			pHeadE=T1;
			if(pHeadE==NULL)
				return;
		}
		if(T1->next!=NULL)
		{
			T2=T1;
			T1=T2->next;
		}
		while(T1!=NULL)
		{
			if(pCurrentB->ScanLine>=T1->yMax)//下闭上开
			{
				CAET* pAETTemp = T1;
				T2->next=T1->next;				
				T1=T2->next;
				delete pAETTemp;
			}
			else
			{
				T2=T1;
				T1=T2->next;
			}
		}
		CP2 UVa,UVb,UVf;//UVa和UVb代表边上任一点的纹理，UVf代表面上任一点的纹理
		UVa=Interpolation(pCurrentB->ScanLine,pHeadE->pb.y,pHeadE->pe.y,pHeadE->uvb,pHeadE->uve);
		UVb=Interpolation(pCurrentB->ScanLine,pHeadE->next->pb.y,pHeadE->next->pe.y,pHeadE->next->uvb,pHeadE->next->uve);
		BOOL Flag=FALSE;
		double xb,xe;//扫描线的起点和终点坐标
		for(T1=pHeadE;T1!=NULL;T1=T1->next)
		{
			if(Flag==FALSE)
			{
				xb=T1->x;
				CurDeep=-(xb*A+pCurrentB->ScanLine*B+D)/C;//z=-(Ax+By-D)/C
				Flag=TRUE;
			}
			else
			{
				xe=T1->x;
				for(double x=xb;x<xe;x++)//左闭右开
				{
					if(CurDeep>=(pZbuf->ZB[ROUND(x)+pZbuf->Width/2][pCurrentB->ScanLine+pZbuf->Height/2]) )//如果新的采样点的深度大于原采样点的深度
					{
						pZbuf->ZB[ROUND(x)+pZbuf->Width/2][pCurrentB->ScanLine+pZbuf->Height/2]=CurDeep;//xy坐标与数组下标保持一致，加a
						UVf=Interpolation(x,xb,xe,UVa,UVb);	
						int u=ROUND(UVf.x);//纹理坐标
						int v=ROUND(UVf.y);
						if(u>=bm->bmWidth)	u=bm->bmWidth-1;
						if(v>=bm->bmHeight)	v=bm->bmHeight-1;
						BYTE Red=Image[v*bm->bmWidth*4 + u*4 + 2];
						BYTE Green=Image[v*bm->bmWidth*4 + u*4 + 1];
						BYTE Blue=Image[v*bm->bmWidth*4 + u*4];
						pDC->SetPixel(ROUND(x),pCurrentB->ScanLine,RGB(Red,Green,Blue));
					}
					CurDeep+=DeepStep;
				}
				Flag=FALSE;
			}
		}
		for(T1=pHeadE;T1!=NULL;T1=T1->next)//边的连续性
		{
			T1->x=T1->x+T1->k;
		}		
	}	
}

void CZBuffer::TextureMapWithPhong(CDC *pDC,CZBuffer *pZbuf,BYTE *Image,BITMAP *bm,CP3 ViewPoint,CLighting *pLight)
{
	double	CurDeep=0.0;//当前扫描线的深度
	double	DeepStep=0.0;//当前扫描线随着x增长的深度步长
	double	A,B,C,D;//平面方程Ax+By+Cz＋D=0的系数
	CVector V21(P[1],P[2]),V10(P[0],P[1]);
	CVector VN=V21*V10;
	A=VN.x;B=VN.y;C=VN.z;
	D=-A*P[1].x-B*P[1].y-C*P[1].z;
	//计算curDeep;从x=xMin开始计算，此时针对yi
	DeepStep=-A/C;//计算直线deep增量
	CAET *T1,*T2;
	pHeadE=NULL;	
	for(pCurrentB=pHeadB;pCurrentB!=NULL;pCurrentB=pCurrentB->next)
	{
		for(pCurrentE=pCurrentB->pET;pCurrentE!=NULL;pCurrentE=pCurrentE->next)
		{
			pEdge=new CAET;
			pEdge->x=pCurrentE->x;
			pEdge->yMax=pCurrentE->yMax;
			pEdge->k=pCurrentE->k;
			pEdge->pb=pCurrentE->pb;
			pEdge->pe=pCurrentE->pe;
			pEdge->uvb=pCurrentE->uvb;
			pEdge->uve=pCurrentE->uve;
			pEdge->vb=pCurrentE->vb;
			pEdge->ve=pCurrentE->ve;
			pEdge->next=NULL;
			AddEt(pEdge);
		}
		EtOrder();	
		T1=pHeadE;
		if(T1==NULL)
		{
			return;
		}
		while(pCurrentB->ScanLine>=T1->yMax)//下闭上开
		{
			CAET * pAETTEmp = T1;			
			T1=T1->next;
			delete pAETTEmp;
			pHeadE=T1;
			if(pHeadE==NULL)
				return;
		}
		if(T1->next!=NULL)
		{
			T2=T1;
			T1=T2->next;
		}
		while(T1!=NULL)
		{
			if(pCurrentB->ScanLine>=T1->yMax)//下闭上开
			{
				CAET* pAETTemp = T1;
				T2->next=T1->next;				
				T1=T2->next;
				delete pAETTemp;
			}
			else
			{
				T2=T1;
				T1=T2->next;
			}
		}
		CP2 UVa,UVb,UVf;//UVa和UVb代表边上任一点的纹理，UVf代表面上任一点的纹理
		UVa=Interpolation(pCurrentB->ScanLine,pHeadE->pb.y,pHeadE->pe.y,pHeadE->uvb,pHeadE->uve);
		UVb=Interpolation(pCurrentB->ScanLine,pHeadE->next->pb.y,pHeadE->next->pe.y,pHeadE->next->uvb,pHeadE->next->uve);
		CVector na,nb,nf;//na和nb代表边上任意点的法矢量，nf代表面上任意点的法矢量
		na=Interpolation(pCurrentB->ScanLine,pHeadE->pb.y,pHeadE->pe.y,pHeadE->vb,pHeadE->ve);
		nb=Interpolation(pCurrentB->ScanLine,pHeadE->next->pb.y,pHeadE->next->pe.y,pHeadE->next->vb,pHeadE->next->ve);
		BOOL Flag=FALSE;
		double xb,xe;//扫描线的起点和终点坐标
		for(T1=pHeadE;T1!=NULL;T1=T1->next)
		{
			if(Flag==FALSE)
			{
				xb=T1->x;
				CurDeep=-(xb*A+pCurrentB->ScanLine*B+D)/C;//z=-(Ax+By-D)/C
				Flag=TRUE;
			}
			else
			{
				xe=T1->x;
				CMaterial *pMa=new CMaterial();//用纹理的颜色值作为材质对漫反射光和环境光的反射率
				for(double x=xb;x<xe;x++)//左闭右开
				{
					if(CurDeep>=(pZbuf->ZB[ROUND(x)+pZbuf->Width/2][pCurrentB->ScanLine+pZbuf->Height/2]) )//如果新的采样点的深度大于原采样点的深度
					{
						pZbuf->ZB[ROUND(x)+pZbuf->Width/2][pCurrentB->ScanLine+pZbuf->Height/2]=CurDeep;//xy坐标与数组下标保持一致，加a
						UVf=Interpolation(x,xb,xe,UVa,UVb);
						nf=Interpolation(x,xb,xe,na,nb);
						int u=ROUND(UVf.x);
						int v=ROUND(UVf.y);
						if(u>=bm->bmWidth)	u=bm->bmWidth-1;
						if(v>=bm->bmHeight)	v=bm->bmHeight-1;
						BYTE Red=Image[v*bm->bmWidth*4 + u*4 + 2];
						BYTE Green=Image[v*bm->bmWidth*4 + u*4 + 1];
						BYTE Blue=Image[v*bm->bmWidth*4 + u*4];
						CRGB tRGB(Red/255.0,Green/255.0,Blue/255.0);
						pMa->M_Ambient=tRGB;//材质对环境光的反射率
						pMa->M_Diffuse=tRGB;//材质对漫反射光的反射率
						pMa->M_Specular=tRGB;//材质对镜面反射光的反射率
						pMa->M_Exp=80;
						CRGB clr=pLight->Lighting(ViewPoint,CP3(ROUND(x),pCurrentB->ScanLine,CurDeep),nf,pMa);
						pDC->SetPixel(ROUND(x),pCurrentB->ScanLine,RGB(clr.red*255,clr.green*255,clr.blue*255) );
					}
					CurDeep+=DeepStep;
				}
				delete pMa;
				Flag=FALSE;
			}
		}
		for(T1=pHeadE;T1!=NULL;T1=T1->next)//边的连续性
		{
			T1->x=T1->x+T1->k;
		}		
	}
}
void CZBuffer::AddEt(CAET *pNewEdge)//合并ET表
{
	CAET *pCE;
	pCE=pHeadE;
	if(pCE==NULL)
	{
		pHeadE=pNewEdge;
		pCE=pHeadE;
	}
	else
	{
		while(pCE->next!=NULL)
		{
			pCE=pCE->next;
		}
		pCE->next=pNewEdge;
	}
}
void CZBuffer::EtOrder()//边表的冒泡排序算法
{
	CAET *pT1,*pT2;
	int Count=1;
	pT1=pHeadE;
	if(pT1==NULL)
	{
		return;
	}
	if(pT1->next==NULL)//如果该ET表没有再连ET表
	{
		return;//桶结点只有一条边，不需要排序
	}
	while(pT1->next!=NULL)//统计边结点的个数
	{
		Count++;
		pT1=pT1->next;
	}
	for(int i=1;i<Count;i++)//冒泡排序
	{
		pT1=pHeadE;
		if(pT1->x>pT1->next->x)//按x由小到大排序
		{
			pT2=pT1->next;
			pT1->next=pT1->next->next;
			pT2->next=pT1;
			pHeadE=pT2;
		}
		else
		{
			if(pT1->x==pT1->next->x)
			{
				if(pT1->k>pT1->next->k)//按斜率由小到大排序
				{
					pT2=pT1->next;
					pT1->next=pT1->next->next;
					pT2->next=pT1;
					pHeadE=pT2;
				}
			}
		}
		pT1=pHeadE;
		while(pT1->next->next!=NULL)
		{
			pT2=pT1;
			pT1=pT1->next;
			if(pT1->x>pT1->next->x)//按x由小到大排序
			{
				pT2->next=pT1->next;
				pT1->next=pT1->next->next;
				pT2->next->next=pT1;
				pT1=pT2->next;
			}
			else
			{
				if(pT1->x==pT1->next->x)
				{
					if(pT1->k>pT1->next->k)//按斜率由小到大排序
					{
						pT2->next=pT1->next;
						pT1->next=pT1->next->next;
						pT2->next->next=pT1;
						pT1=pT2->next;
					}
				}
			}
		}
	}
}

CRGB CZBuffer::Interpolation(double t,double t1,double t2,CRGB c1,CRGB c2)//颜色插值
{
	CRGB c;
	c=(t-t2)/(t1-t2)*c1+(t-t1)/(t2-t1)*c2;
	return c;
}
CVector CZBuffer::Interpolation(double t,double t1,double t2,CVector v1,CVector v2)//法矢插值
{
	CVector v;
	v=v1*(t-t2)/(t1-t2)+v2*(t-t1)/(t2-t1);
	return v;
}
CP2 CZBuffer::Interpolation(double t,double t1,double t2,CP2 p1,CP2 p2)//纹理插值
{
	CP2 p;
	p=p1*(t-t2)/(t1-t2)+p2*(t-t1)/(t2-t1);
	return p;
} 
void CZBuffer::InitDeepBuffer(int width,int height,double depth)//初始化深度缓冲
{
	Width=width,Height=height;
	ZB=new double *[Width];
	for(int i=0;i<Width;i++)
		ZB[i]=new double[Height];
	for(i=0;i<Width;i++)//初始化深度缓冲
		for(int j=0;j<Height;j++)
			ZB[i][j]=double(depth);
}

void CZBuffer::ClearMemory()//内存清理
{
	DeleteAETChain(pHeadE);
	CBucket *pBucket=pHeadB;
	while (pBucket !=NULL)//针对每一个桶
	{
		CBucket * pBucketTemp = pBucket->next;
		DeleteAETChain(pBucket->pET);
		delete pBucket;
		pBucket=pBucketTemp;
	}
	pHeadB=NULL;
	pHeadE=NULL;
	//删除当前缓冲区中的面片数据，防止内存泄露
	if(P!=NULL)
	{
		delete []P;
		P=NULL;
	}
	if (VN!=NULL)
	{
		delete []VN;
		VN=NULL;
	}
	if (UV!=NULL)
	{
		delete []UV;
		UV=NULL;
	}
}

void CZBuffer::DeleteAETChain(CAET* pAET)//删除当前桶节点上的边节点
{
	while (pAET!=NULL)
	{
		CAET* pAETTemp=pAET->next;
		delete pAET;
		pAET=pAETTemp;
	}
}