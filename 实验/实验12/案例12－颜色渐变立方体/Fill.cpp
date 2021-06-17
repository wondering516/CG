// Fill.cpp: implementation of the CFill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Fill.h"
#define ROUND(x) int(x+0.5)//四舍五入宏
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFill::CFill()
{
	PNum=0;
	P=NULL;
	pEdge=NULL;
	pHeadB=NULL;
	pHeadE=NULL;
}

CFill::~CFill()
{
	if(P!=NULL)
	{
		delete[] P;
		P=NULL;
	}
	ClearMemory();
}

void CFill::SetPoint(CPi2 *p,int m)
{
	P=new CPi2[m];//创建一维动态数组
	for(int i=0;i<m;i++)
	{
		P[i]=p[i];	
	}
	PNum=m;
}

void CFill::CreateBucket()//创建桶表
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
		if(yMin==y)//如果是扫描线的最小值
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

void CFill::CreateEdge()//创建边表
{
	for(int i=0;i<PNum;i++)
	{
		pCurrentB=pHeadB;
		int j=(i+1)%PNum;//边的第2个顶点，P[i]和P[j]点对构成边
		if(P[i].y<P[j].y)//边的终点比起点高
		{
			pEdge=new CAET;
			pEdge->x=P[i].x;//计算ET表的值
			pEdge->yMax=P[j].y;
			pEdge->k=(P[j].x-P[i].x)/(P[j].y-P[i].y);//代表1/k
			pEdge->pb=P[i];//绑定顶点和颜色
			pEdge->pe=P[j];
			pEdge->next=NULL;
			while(pCurrentB->ScanLine!=P[i].y)//在桶内寻找当前边的yMin
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
		if(P[i].y!=P[j].y)
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

void CFill::Gouraud(CDC *pDC)//填充多边形
{
	CAET *pT1=NULL,*pT2=NULL;
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
		CRGB Ca,Cb,Cf;//Ca、Cb代表边上任意点的颜色，Cf代表面上任意点的颜色
		Ca=Interpolation(pCurrentB->ScanLine,pHeadE->pb.y,pHeadE->pe.y,pHeadE->pb.c,pHeadE->pe.c);
		Cb=Interpolation(pCurrentB->ScanLine,pHeadE->next->pb.y,pHeadE->next->pe.y,pHeadE->next->pb.c,pHeadE->next->pe.c);
		BOOL Flag=FALSE;
		double xb,xe;//扫描线和有效边相交区间的起点和终点坐标
		for(pT1=pHeadE;pT1!=NULL;pT1=pT1->next)
		{
			if(Flag==FALSE)
			{
				xb=pT1->x;
				Flag=TRUE;
			}
			else
			{
				xe=pT1->x;
				for(double x=xb;x<xe;x++)//左闭右开
				{
					Cf=Interpolation(x,xb,xe,Ca,Cb);
					pDC->SetPixel(ROUND(x),pCurrentB->ScanLine,RGB(Cf.red*255,Cf.green*255,Cf.blue*255));
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

void CFill::AddEt(CAET *pNewEdge)//合并ET表
{
	CAET *pCE=pHeadE;
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
void CFill::EtOrder()//边表的冒泡排序算法
{
	CAET *pT1=NULL,*pT2=NULL;
	int Count=1;
	pT1=pHeadE;
	if(NULL==pT1)
	{
		return;
	}
	if(NULL==pT1->next)//如果该ET表没有再连ET表
	{
		return;//桶结点只有一条边，不需要排序
	}
	while(NULL!=pT1->next)//统计结点的个数
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

CRGB CFill::Interpolation(double t,double t1,double t2,CRGB c1,CRGB c2)//线性插值
{
	CRGB c;
	c=(t-t2)/(t1-t2)*c1+(t-t1)/(t2-t1)*c2;
	return c;
}

void CFill::ClearMemory()//安全删除所有桶和桶上面的边
{
	DeleteAETChain(pHeadE);
	CBucket *pBucket=pHeadB;
	while (pBucket != NULL)// 针对每一个桶
	{
		CBucket * pBucketTemp=pBucket->next;
		DeleteAETChain(pBucket->pET);
		delete pBucket;
		pBucket=pBucketTemp;
	}
	pHeadB=NULL;
	pHeadE=NULL;
}

void CFill::DeleteAETChain(CAET* pAET)
{
	while (pAET!=NULL)
	{
		CAET* pAETTemp=pAET->next;
		delete pAET;
		pAET=pAETTemp;
	}
}
