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
	double    x;    //��ǰɨ��������Ч�ߵĽ����x����
	int       yMax; //�ߵ����yֵ
	double    k;    //б�ʵĵ���(x������)
	CAET      *next;
};

#endif // !defined(AFX_CAet_H__D7F2123F_A3AE_40D6_81D8_E403C7E50447__INCLUDED_)
