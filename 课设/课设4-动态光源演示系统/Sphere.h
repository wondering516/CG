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
	void ReadPoint();//读入顶点表
	void ReadFace();//读入面表
	void InitParameter();//参数初始化
	void PerProject(CP3);//透视投影
	void DrawObject(CDC *,CLighting*,CMaterial*,CZBuffer *,int);//绘制球体	
public:
	int r;//球体半径
	double R,Theta,Phi,d;//视点在用户坐标系中的球坐标
	double k[9];//透视变换参数
	CP3 ViewPoint;//视点球坐标位置
	CPi3 ScreenP;//屏幕坐标系的二维坐标点
	CP3 *P;//球的顶点一维数组
	CFace **F;//面的二维数组
	int N1,N2;//N1为纬度区间,N2为经度区间
};

#endif // !defined(AFX_SPHERE_H__629952EF_5B21_47B6_801E_E6F236B8BFEA__INCLUDED_)
