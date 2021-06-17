// ZBuffer.h: interface for the CZBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZBUFFER_H__ED93E842_7399_49B8_B579_6A52FBA1289D__INCLUDED_)
#define AFX_ZBUFFER_H__ED93E842_7399_49B8_B579_6A52FBA1289D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Bucket.h"//Ͱͷ�ļ�
#include "Vector.h"//ʸ��ͷ�ļ�
#include "Lighting.h"
#include "Material.h"

class CZBuffer  
{
public:
	CZBuffer();
    virtual ~CZBuffer();
	void CreateBucket();//����Ͱ
	void CreateEdgeforPhong();//ΪPhong���մ����߱� 
	void CreateEdgeforGouraud();//ΪGouraud���մ����߱� 
	void CreateEdgeforTexture();//Ϊ����ӳ�䴴���߱�
	void AddEt(CAET *);//�ϲ��߱�
	void EtOrder();//�߱�����
	void Phong(CDC *,CP3,CLighting *,CMaterial *);//Phong��亯��
    void Gouraud(CDC *);//Gouraud���
	void TextureMap(CDC*,CZBuffer*,BYTE*,BITMAP*);//����ӳ��
	void TextureMapWithPhong(CDC*,CZBuffer*,BYTE*,BITMAP*,CP3,CLighting*);//Phong����ӳ�� 
	void InitDeepBuffer(int,int,double);//��ʼ����Ȼ���
    CRGB Interpolation(double,double,double,CRGB,CRGB);//��ǿ���Բ�ֵ
	CVector Interpolation(double,double,double,CVector,CVector);//��ʸ���Բ�ֵ
	CP2  Interpolation(double,double,double,CP2,CP2);//�����ֵ
	void SetPoint(CPi3 *,CVector *,int);//Phong���ճ�ʼ��
    void SetPoint(CPi3 *,int);//Gouraud���ճ�ʼ��
	void SetPoint(CPi3*,CVector*,CP2*,int);//����ӳ���ʼ��
	void ClearMemory();//�����ڴ�
	void DeleteAETChain(CAET* pAET);//ɾ���߱�
protected:
	int PNum;//�������
	CPi3 *P;//��������
	CVector *VN;//����ķ�ʸ����̬����
	CP2 *UV;//��������
	CAET *pHeadE,*pCurrentE,*pEdge;//��Ч�߱���ָ��
	CBucket *pCurrentB,*pHeadB;//Ͱָ��
	double **ZB;//��Ȼ�����
	int Width,Height;//��Ȼ����С����
};

#endif // !defined(AFX_ZBUFFER_H__ED93E842_7399_49B8_B579_6A52FBA1289D__INCLUDED_)
