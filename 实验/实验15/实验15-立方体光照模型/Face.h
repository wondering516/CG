// Face.h: interface for the CFace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACE_H__82BF93AE_A82E_407D_AF42_6E6608253C1C__INCLUDED_)
#define AFX_FACE_H__82BF93AE_A82E_407D_AF42_6E6608253C1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Vector.h"

class CFace  
{
public:
	CFace();
	virtual ~CFace();
	void SetEN(int);
	void SetNormal(CP3,CP3,CP3);//����С�淨ʸ��
public:
	int En;//��ı���
	int *p;//��Ķ����
	CVector Normal;//С��ķ�ʸ��
};

#endif // !defined(AFX_FACE_H__82BF93AE_A82E_407D_AF42_6E6608253C1C__INCLUDED_)
