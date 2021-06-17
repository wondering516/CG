// Objects.h: interface for the CObjects class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTS_H__69E5D573_8528_4FC7_8D5B_5D4992C3DC99__INCLUDED_)
#define AFX_OBJECTS_H__69E5D573_8528_4FC7_8D5B_5D4992C3DC99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Vector.h"//ʸ����
#include "T2.h"//������
#include "Face.h"//��Ƭ��

class CObjects  //3ds������
{
public:
	CObjects();
	virtual ~CObjects();
public:
	int		 NumOfVertex;   //�������
	int		 NumOfFace;     //��Ƭ����
	int		 NumOfTexVertex;//���������
	char	 ObjName[255];  //��������
	CP3  	 *pVertex;      //��������
	CVector  *pVertNormal;  //��ķ�ʸ��
	CVector  *pFaceNormal;  //��ķ�ʸ��
	CT2      *pTexVertex;   //��������
	CFace	 *pFace;        //����Ϣ
};

#endif // !defined(AFX_OBJECTS_H__69E5D573_8528_4FC7_8D5B_5D4992C3DC99__INCLUDED_)
