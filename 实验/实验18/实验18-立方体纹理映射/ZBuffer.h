// ZBuffer.h: interface for the CZBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZBUFFER_H__ED93E842_7399_49B8_B579_6A52FBA1289D__INCLUDED_)
#define AFX_ZBUFFER_H__ED93E842_7399_49B8_B579_6A52FBA1289D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Pi3.h"//��ά��ͷ�ļ�
#include "Bucket.h"//Ͱͷ�ļ�
#include "Vector.h"//ʸ��ͷ�ļ�
#include "P2.h"

class CZBuffer  
{
public:
	CZBuffer();
    virtual ~CZBuffer();
	void CreateBucket();//����Ͱ
	void CreateEdge();//�߱�
	void AddEt(CAET *);//�ϲ�ET��
	void EtOrder();//ET������
	void TextureMap(CDC *,COLORREF **);//����ӳ��
	void InitDeepBuffer(int,int,double);//��ʼ����Ȼ���
    CP2 Interpolation(double,double,double,CP2,CP2);//���Բ�ֵ
	void SetPoint(CPi3 *,CP2 *,int);
	void ClearMemory();//�����ڴ�
	void DeleteAETChain(CAET* pAET);//ɾ���߱�
protected:
	int PNum;//�������
	CPi3 *P;//��������
	CP2 *TC;//����ķ�ʸ����̬����
	CAET *pHeadE,*pCurrentE,*pEdge;//��Ч�߱���ָ��
	CBucket *pCurrentB,*pHeadB;
	double **ZB;
	int Width,Height;//��Ȼ����С����
};

#endif // !defined(AFX_ZBUFFER_H__ED93E842_7399_49B8_B579_6A52FBA1289D__INCLUDED_)
