// Face.h: interface for the CFace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACE_H__288D3AA0_BB35_4E92_9AE4_4939AE57DEC9__INCLUDED_)
#define AFX_FACE_H__288D3AA0_BB35_4E92_9AE4_4939AE57DEC9__INCLUDED_

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
	void SetNormal(CP3,CP3,CP3);//设置法矢量
public:
	int En;//边数
	int *p;//顶点索引号动态数组
	CP2 *t;//纹理索引号动态数组
	CVector Normal;//法矢量
};

#endif // !defined(AFX_FACE_H__288D3AA0_BB35_4E92_9AE4_4939AE57DEC9__INCLUDED_)
