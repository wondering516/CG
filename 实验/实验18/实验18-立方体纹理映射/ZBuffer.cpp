// ZBuffer.cpp: implementation of the CZBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "ZBuffer.h"
#define ROUND(a) int(floor(a+0.5))
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
	pHeadE=NULL;
	pCurrentB=NULL;
	pEdge=NULL;
	pCurrentE=NULL;
	pHeadB=NULL;
	ZB=NULL;
}

CZBuffer::~CZBuffer()
{
	//��ǰС��ĵ���������Ӧ�ڻ����굱ǰ��֮��ʱɾ��
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

void CZBuffer::SetPoint(CPi3 *p,CP2 *tc,int m)
{
	P=new CPi3[m];
	TC=new CP2[m];
    for(int i=0;i<m;i++)
	{
		P[i]=p[i];	
		TC[i]=tc[i];
	}
	PNum=m;
}
void CZBuffer::CreateBucket()//����Ͱ��
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
		if(yMin==y)//����Ͱͷ���
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

void CZBuffer::CreateEdge()//�����߱�
{
	for(int i=0;i<PNum;i++)
	{
		pCurrentB=pHeadB;
		int j=(i+1)%PNum;//�ߵĵڶ������㣬P[i]��P[j]���ɱ�
		if(P[i].y<P[j].y)//�ߵ��յ������
		{
			pEdge=new CAET;
			pEdge->x=P[i].x;//����ET���ֵ
			pEdge->yMax=P[j].y;
			pEdge->k=(P[j].x-P[i].x)/(P[j].y-P[i].y);//����1/k
			pEdge->pb=P[i];//�󶨶������ɫ	
			pEdge->pe=P[j];
			pEdge->tb=TC[i];
			pEdge->te=TC[j];
			pEdge->next=NULL;
			while(pCurrentB->ScanLine!=P[i].y)//��Ͱ��Ѱ�Ҹñߵ�yMin
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
			pEdge->tb=TC[i];
			pEdge->te=TC[j];
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

void CZBuffer::TextureMap(CDC *pDC,COLORREF **Image)//����ӳ��
{
	double	z=0.0;//��ǰɨ���ߵ�z
	double	zStep=0.0;//��ǰɨ��������x������z����
	double	A,B,C,D;//ƽ�淽��Ax+By+Cz��D=0��ϵ��
    CVector V01(P[0],P[1]),V12(P[1],P[2]);
	CVector VN=V01*V12;
	A=VN.x;B=VN.y;C=VN.z;
	D=-A*P[1].x-B*P[1].y-C*P[1].z;
	zStep=-A/C;//����ֱ��z����
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
			pEdge->tb=pCurrentE->tb;
			pEdge->te=pCurrentE->te;
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
		CP2 ta,tb,tf;//ta��tb���������һ�������tf����������һ�������
		ta=Interpolation(pCurrentB->ScanLine,pHeadE->pb.y,pHeadE->pe.y,pHeadE->tb,pHeadE->te);
		tb=Interpolation(pCurrentB->ScanLine,pHeadE->next->pb.y,pHeadE->next->pe.y,pHeadE->next->tb,pHeadE->next->te);
		BOOL Flag=FALSE;
		double xb,xe;//ɨ���ߺ���Ч���ཻ����������յ�����
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
				for(double x=xb;x<xe;x++)//����ҿ�
				{
					tf=Interpolation(x,xb,xe,ta,tb);	
					COLORREF clr=Image[ROUND(tf.y)][ROUND(tf.x)];
					if(z>=ZB[ROUND(x)+Width/2][pCurrentB->ScanLine+Height/2])//����²��������ȴ���ԭ����������
					{
					   ZB[ROUND(x)+Width/2][pCurrentB->ScanLine+Height/2]=z;//xy�����������±걣��һ��
					   pDC->SetPixel(ROUND(x),pCurrentB->ScanLine,clr);
					}
					z+=zStep;				
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

void CZBuffer::AddEt(CAET *pNewEdge)//�ϲ�ET��
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
void CZBuffer::EtOrder()//�߱��ð�������㷨
{
	CAET *pT1,*pT2;
	int Count=1;
	pT1=pHeadE;
	if(pT1==NULL)
	{
		return;
	}
	if(pT1->next==NULL)//�����ET��û������ET��
	{
		return;//Ͱ���ֻ��һ���ߣ�����Ҫ����
	}
	while(pT1->next!=NULL)//ͳ�Ʊ߽��ĸ���
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

CP2 CZBuffer::Interpolation(double t,double t1,double t2,CP2 p1,CP2 p2)//���Բ�ֵ
{
	CP2 p;
	p=(t-t2)/(t1-t2)*p1+(t-t1)/(t2-t1)*p2;
	return p;
}
void CZBuffer::InitDeepBuffer(int width,int height,double depth)//��ʼ����Ȼ���
{
	Width=width,Height=height;
	ZB=new double *[Width];
	for(int i=0;i<Width;i++)
		ZB[i]=new double[Height];
	for(i=0;i<Width;i++)//��ʼ����Ȼ���
		for(int j=0;j<Height;j++)
			ZB[i][j]=double(depth);
}

void CZBuffer::ClearMemory()
{
	DeleteAETChain(pHeadE);
	CBucket *pBucket=pHeadB;
	while (pBucket !=NULL)//���ÿһ��Ͱ
	{
		CBucket * pBucketTemp = pBucket->next;
		DeleteAETChain(pBucket->pET);
		delete pBucket;
		pBucket=pBucketTemp;
	}
	pHeadB=NULL;
	pHeadE=NULL;
	if(P!=NULL)//delete����Ϣ
	{
		delete []P;
		P=NULL;
	}
	if (TC!=NULL)//delete����������Ϣ
	{
		delete []TC;
		TC=NULL;
	}
}

void CZBuffer::DeleteAETChain(CAET* pAET)
{
	while (pAET!=NULL)
	{
		CAET* pAETTemp=pAET->next;
		delete pAET;
		pAET=pAETTemp;
	}
}