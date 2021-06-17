// Fill.cpp: implementation of the CFill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Fill.h"
#define ROUND(x) int(x+0.5)//���������
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
	P=new CPi2[m];//����һά��̬����
	for(int i=0;i<m;i++)
	{
		P[i]=p[i];	
	}
	PNum=m;
}

void CFill::CreateBucket()//����Ͱ��
{
	int yMin,yMax;
	yMin=yMax=P[0].y;
	for(int i=0;i<PNum;i++)//���Ҷ���������ǵ���С�����ɨ����
	{
		if(P[i].y<yMin)
		{
			yMin=P[i].y;//ɨ���ߵ���Сֵ
		}
		if(P[i].y>yMax)
		{
			yMax=P[i].y;//ɨ���ߵ����ֵ
		}
	}
	for(int y=yMin;y<=yMax;y++)
	{
		if(yMin==y)//�����ɨ���ߵ���Сֵ
		{
			pHeadB=new CBucket;//����Ͱ��ͷ���
			pCurrentB=pHeadB;//pCurrentBΪCBucket��ǰ���ָ��
			pCurrentB->ScanLine=yMin;
			pCurrentB->pET=NULL;//û�����ӱ߱�
			pCurrentB->next=NULL;
		}
		else//����ɨ����
		{
			pCurrentB->next=new CBucket;//����Ͱ���������
			pCurrentB=pCurrentB->next;
			pCurrentB->ScanLine=y;
			pCurrentB->pET=NULL;
			pCurrentB->next=NULL;
		}
	}
}

void CFill::CreateEdge()//�����߱�
{
	for(int i=0;i<PNum;i++)
	{
		pCurrentB=pHeadB;
		int j=(i+1)%PNum;//�ߵĵ�2�����㣬P[i]��P[j]��Թ��ɱ�
		if(P[i].y<P[j].y)//�ߵ��յ������
		{
			pEdge=new CAET;
			pEdge->x=P[i].x;//����ET���ֵ
			pEdge->yMax=P[j].y;
			pEdge->k=(P[j].x-P[i].x)/(P[j].y-P[i].y);//����1/k
			pEdge->pb=P[i];//�󶨶������ɫ
			pEdge->pe=P[j];
			pEdge->next=NULL;
			while(pCurrentB->ScanLine!=P[i].y)//��Ͱ��Ѱ�ҵ�ǰ�ߵ�yMin
			{
				pCurrentB=pCurrentB->next;//�Ƶ�yMin���ڵ�Ͱ���
			}		
		}
		if(P[j].y<P[i].y)//�ߵ��յ������
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

void CFill::Gouraud(CDC *pDC)//�������
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
		while(pCurrentB->ScanLine>=pT1->yMax)//�±��Ͽ�
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
			if(pCurrentB->ScanLine>=pT1->yMax)//�±��Ͽ�
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
		CRGB Ca,Cb,Cf;//Ca��Cb���������������ɫ��Cf����������������ɫ
		Ca=Interpolation(pCurrentB->ScanLine,pHeadE->pb.y,pHeadE->pe.y,pHeadE->pb.c,pHeadE->pe.c);
		Cb=Interpolation(pCurrentB->ScanLine,pHeadE->next->pb.y,pHeadE->next->pe.y,pHeadE->next->pb.c,pHeadE->next->pe.c);
		BOOL Flag=FALSE;
		double xb,xe;//ɨ���ߺ���Ч���ཻ����������յ�����
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
				for(double x=xb;x<xe;x++)//����ҿ�
				{
					Cf=Interpolation(x,xb,xe,Ca,Cb);
					pDC->SetPixel(ROUND(x),pCurrentB->ScanLine,RGB(Cf.red*255,Cf.green*255,Cf.blue*255));
				}
				Flag=FALSE;
			}
		}
		for(pT1=pHeadE;pT1!=NULL;pT1=pT1->next)//�ߵ�������
		{
			pT1->x=pT1->x+pT1->k;
		}		
	}
}

void CFill::AddEt(CAET *pNewEdge)//�ϲ�ET��
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
void CFill::EtOrder()//�߱��ð�������㷨
{
	CAET *pT1=NULL,*pT2=NULL;
	int Count=1;
	pT1=pHeadE;
	if(NULL==pT1)
	{
		return;
	}
	if(NULL==pT1->next)//�����ET��û������ET��
	{
		return;//Ͱ���ֻ��һ���ߣ�����Ҫ����
	}
	while(NULL!=pT1->next)//ͳ�ƽ��ĸ���
	{
		Count++;
		pT1=pT1->next;
	}
	for(int i=1;i<Count;i++)//ð������
	{
		pT1=pHeadE;
		if(pT1->x>pT1->next->x)//��x��С��������
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
				if(pT1->k>pT1->next->k)//��б����С��������
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
			if(pT1->x>pT1->next->x)//��x��С��������
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
					if(pT1->k>pT1->next->k)//��б����С��������
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

CRGB CFill::Interpolation(double t,double t1,double t2,CRGB c1,CRGB c2)//���Բ�ֵ
{
	CRGB c;
	c=(t-t2)/(t1-t2)*c1+(t-t1)/(t2-t1)*c2;
	return c;
}

void CFill::ClearMemory()//��ȫɾ������Ͱ��Ͱ����ı�
{
	DeleteAETChain(pHeadE);
	CBucket *pBucket=pHeadB;
	while (pBucket != NULL)// ���ÿһ��Ͱ
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
