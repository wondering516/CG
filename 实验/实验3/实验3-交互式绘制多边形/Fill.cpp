// Fill.cpp: implementation of the CFill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Fill.h"

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

void CFill::SetPoint(CPoint p[],int m)//动态创建多边形顶点数组
{
	P=new CPoint[m];
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
		if(yMin==y)//建立桶头结点
		{
			pHeadB=new CBucket;//pHeadB为CBucket的头结点
			pCurrentB=pHeadB;//CurrentB为CBucket当前结点
			pCurrentB->ScanLine=yMin;
			pCurrentB->pET=NULL;//没有链接边表
			pCurrentB->next=NULL;
		}
		else//建立桶的其它结点
		{
			pCurrentB->next=new CBucket;
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
		int j=(i+1)%PNum;//边的第二个顶点，P[i]和P[j]构成边
		if(P[i].y<P[j].y)//边的起点比终点低
		{
			pEdge=new CAET;
			pEdge->x=P[i].x;//计算ET表的值
			pEdge->yMax=P[j].y;
			pEdge->k=(double)(P[j].x-P[i].x)/((double)(P[j].y-P[i].y));//代表1/k
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
			pEdge->k=(double)(P[i].x-P[j].x)/((double)(P[i].y-P[j].y));
			pEdge->next=NULL;
			while(pCurrentB->ScanLine!=P[j].y)
			{
				pCurrentB=pCurrentB->next;
			}
		}
		if((P[j].y)!=P[i].y)
		{
			pCurrentE=pCurrentB->pET;
			if(pCurrentE==NULL)
			{
				pCurrentE=pEdge;
				pCurrentB->pET=pCurrentE;
			}
			else
			{
				while(NULL!=pCurrentE->next)
				{
					pCurrentE=pCurrentE->next;
				}
				pCurrentE->next=pEdge;
			}
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

void CFill::FillPolygon(CDC *pDC)//填充多边形
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
		BOOL In=FALSE;//设置一个BOOL变量In，初始值为假
		int xb,xe;//扫描线的起点和终点
		for(pT1=pHeadE;pT1!=NULL;pT1=pT1->next)//填充扫描线和多边形相交的区间
		{
			if(FALSE==In)
			{
				xb=(int)pT1->x;
				In=TRUE;//每访问一个结点,把In值取反一次
			}
			else//如果In值为真，则填充从当前结点的x值开始到下一结点的x值结束的区间
			{
				xe=(int)pT1->x;
				for(int x=xb;x<=xe;x++)
					pDC->SetPixel(x,pCurrentB->ScanLine,RGB(0,0,255));//蓝色填充
				In=FALSE;
			}		
		}
		for(pT1=pHeadE;pT1!=NULL;pT1=pT1->next)//边连贯性
		{
			pT1->x=pT1->x+pT1->k;//x=x+1/k				 
		}
	}
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
