// Fill.h: interface for the CFill class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILL_H__98E7027C_F967_4C96_98D8_A2B2A54E5B7F__INCLUDED_)
#define AFX_FILL_H__98E7027C_F967_4C96_98D8_A2B2A54E5B7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Bucket.h"//桶头文件

class CFill  
{
public:
	CFill();
	virtual ~CFill();
	void SetPoint(CPoint p[],int);     //设定多边形顶点
	void CreateBucket();               //创建桶
	void CreateEdge();                 //边表
	void AddEt(CAET *);                //合并ET表
	void EtOrder();                    //ET表排序
	void FillPolygon(CDC *);           //填充
	void ClearMemory();//清理内存
	void DeleteAETChain(CAET* pAET);//删除边表
private:
	int     PNum;                      //顶点个数
	CPoint  *P;                        //多边形顶点数组
	CAET    *pHeadE,*pCurrentE,*pEdge; //有效边表结点指针
	CBucket *pHeadB,*pCurrentB;        //桶表结点指针
};

#endif // !defined(AFX_FILL_H__98E7027C_F967_4C96_98D8_A2B2A54E5B7F__INCLUDED_)
