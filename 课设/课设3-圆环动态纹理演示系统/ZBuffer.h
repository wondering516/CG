// ZBuffer.h: interface for the CZBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ZBUFFER_H__ED93E842_7399_49B8_B579_6A52FBA1289D__INCLUDED_)
#define AFX_ZBUFFER_H__ED93E842_7399_49B8_B579_6A52FBA1289D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Bucket.h"//桶头文件
#include "Vector.h"//矢量头文件
#include "Lighting.h"
#include "Material.h"

class CZBuffer  
{
public:
	CZBuffer();
    virtual ~CZBuffer();
	void CreateBucket();//创建桶
	void CreateEdgeforPhong();//为Phong光照创建边表 
	void CreateEdgeforGouraud();//为Gouraud光照创建边表 
	void CreateEdgeforTexture();//为纹理映射创建边表
	void AddEt(CAET *);//合并边表
	void EtOrder();//边表排序
	void Phong(CDC *,CP3,CLighting *,CMaterial *);//Phong填充函数
    void Gouraud(CDC *);//Gouraud填充
	void TextureMap(CDC*,CZBuffer*,BYTE*,BITMAP*);//纹理映射
	void TextureMapWithPhong(CDC*,CZBuffer*,BYTE*,BITMAP*,CP3,CLighting*);//Phong纹理映射 
	void InitDeepBuffer(int,int,double);//初始化深度缓存
    CRGB Interpolation(double,double,double,CRGB,CRGB);//光强线性插值
	CVector Interpolation(double,double,double,CVector,CVector);//法矢线性插值
	CP2  Interpolation(double,double,double,CP2,CP2);//纹理插值
	void SetPoint(CPi3 *,CVector *,int);//Phong光照初始化
    void SetPoint(CPi3 *,int);//Gouraud光照初始化
	void SetPoint(CPi3*,CVector*,CP2*,int);//纹理映射初始化
	void ClearMemory();//清理内存
	void DeleteAETChain(CAET* pAET);//删除边表
protected:
	int PNum;//顶点个数
	CPi3 *P;//顶点数组
	CVector *VN;//顶点的法矢量动态数组
	CP2 *UV;//纹理坐标
	CAET *pHeadE,*pCurrentE,*pEdge;//有效边表结点指针
	CBucket *pCurrentB,*pHeadB;//桶指针
	double **ZB;//深度缓冲区
	int Width,Height;//深度缓冲大小参数
};

#endif // !defined(AFX_ZBUFFER_H__ED93E842_7399_49B8_B579_6A52FBA1289D__INCLUDED_)
