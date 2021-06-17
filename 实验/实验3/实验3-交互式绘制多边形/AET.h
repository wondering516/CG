// Aet.h: interface for the CAet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAET_H__D7F2123F_A3AE_40D6_81D8_E403C7E50447__INCLUDED_)
#define AFX_CAET_H__D7F2123F_A3AE_40D6_81D8_E403C7E50447__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAET  
{
public:
	CAET();
	virtual ~CAET();
public:
	double    x;    //当前扫描线与有效边的交点的x坐标
	int       yMax; //边的最大y值
	double    k;    //斜率的倒数(x的增量)
	CAET      *next;
};

#endif // !defined(AFX_CAet_H__D7F2123F_A3AE_40D6_81D8_E403C7E50447__INCLUDED_)
