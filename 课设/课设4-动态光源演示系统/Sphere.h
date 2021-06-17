// Sphere.h: interface for the CSphere class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPHERE_H__629952EF_5B21_47B6_801E_E6F236B8BFEA__INCLUDED_)
#define AFX_SPHERE_H__629952EF_5B21_47B6_801E_E6F236B8BFEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Face.h"
#include "Lighting.h"
#include "Material.h"
#include "ZBuffer.h"

class CSphere  
{
public:
	CSphere();
	virtual ~CSphere();
	void SetSize(int);
	void ReadPoint();//���붥���
	void ReadFace();//�������
	void InitParameter();//������ʼ��
	void PerProject(CP3);//͸��ͶӰ
	void DrawObject(CDC *,CLighting*,CMaterial*,CZBuffer *,int);//��������	
public:
	int r;//����뾶
	double R,Theta,Phi,d;//�ӵ����û�����ϵ�е�������
	double k[9];//͸�ӱ任����
	CP3 ViewPoint;//�ӵ�������λ��
	CPi3 ScreenP;//��Ļ����ϵ�Ķ�ά�����
	CP3 *P;//��Ķ���һά����
	CFace **F;//��Ķ�ά����
	int N1,N2;//N1Ϊγ������,N2Ϊ��������
};

#endif // !defined(AFX_SPHERE_H__629952EF_5B21_47B6_801E_E6F236B8BFEA__INCLUDED_)
