// Fill.h: interface for the CFill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILL_H__98E7027C_F967_4C96_98D8_A2B2A54E5B7F__INCLUDED_)
#define AFX_FILL_H__98E7027C_F967_4C96_98D8_A2B2A54E5B7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Bucket.h"//Ͱͷ�ļ�

class CFill  
{
public:
	CFill();
	virtual ~CFill();
	void SetPoint(CPoint p[],int);     //�趨����ζ���
	void CreateBucket();               //����Ͱ
	void CreateEdge();                 //�߱�
	void AddEt(CAET *);                //�ϲ�ET��
	void EtOrder();                    //ET������
	void FillPolygon(CDC *);           //���
	void ClearMemory();//�����ڴ�
	void DeleteAETChain(CAET* pAET);//ɾ���߱�
private:
	int     PNum;                      //�������
	CPoint  *P;                        //����ζ�������
	CAET    *pHeadE,*pCurrentE,*pEdge; //��Ч�߱���ָ��
	CBucket *pHeadB,*pCurrentB;        //Ͱ����ָ��
};

#endif // !defined(AFX_FILL_H__98E7027C_F967_4C96_98D8_A2B2A54E5B7F__INCLUDED_)
