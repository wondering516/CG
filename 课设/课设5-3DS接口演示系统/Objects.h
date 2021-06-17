// Objects.h: interface for the CObjects class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECTS_H__69E5D573_8528_4FC7_8D5B_5D4992C3DC99__INCLUDED_)
#define AFX_OBJECTS_H__69E5D573_8528_4FC7_8D5B_5D4992C3DC99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Vector.h"//矢量类
#include "T2.h"//纹理类
#include "Face.h"//面片类

class CObjects  //3ds对象类
{
public:
	CObjects();
	virtual ~CObjects();
public:
	int		 NumOfVertex;   //顶点个数
	int		 NumOfFace;     //面片个数
	int		 NumOfTexVertex;//纹理顶点个数
	char	 ObjName[255];  //对象名称
	CP3  	 *pVertex;      //顶点坐标
	CVector  *pVertNormal;  //点的法矢量
	CVector  *pFaceNormal;  //面的法矢量
	CT2      *pTexVertex;   //纹理坐标
	CFace	 *pFace;        //面信息
};

#endif // !defined(AFX_OBJECTS_H__69E5D573_8528_4FC7_8D5B_5D4992C3DC99__INCLUDED_)
