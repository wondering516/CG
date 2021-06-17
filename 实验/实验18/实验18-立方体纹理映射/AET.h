// AET.h: interface for the CAET class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAET_H__D7F2123F_A3AE_40D6_81D8_E403C7E50447__INCLUDED_)
#define AFX_CAET_H__D7F2123F_A3AE_40D6_81D8_E403C7E50447__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Vector.h"
#include "P2.h"
class CAET  
{
public:
	CAET();
	virtual ~CAET();
public:
	double x;//��ǰx
	int    yMax;//�ߵ����yֵ
	double k;//б�ʵĵ���(x������)
	CPi3    pb;//�������
	CPi3    pe;//�յ�����
	CP2     tb;//��������
	CP2     te;
	CAET   *next;
};

#endif // !defined(AFX_CAET_H__D7F2123F_A3AE_40D6_81D8_E403C7E50447__INCLUDED_)
