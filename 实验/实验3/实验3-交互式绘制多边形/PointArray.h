// PointArray.h: interface for the CPointArray class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINTARRAY_H__0606E0A6_3F1F_4F75_B6FC_F2987767B586__INCLUDED_)
#define AFX_POINTARRAY_H__0606E0A6_3F1F_4F75_B6FC_F2987767B586__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPointArray: public CObject 
{
public:
	CPointArray();
	CPointArray(CPoint);
	virtual ~CPointArray();
public:
	CPoint pt;
};

#endif // !defined(AFX_POINTARRAY_H__0606E0A6_3F1F_4F75_B6FC_F2987767B586__INCLUDED_)
