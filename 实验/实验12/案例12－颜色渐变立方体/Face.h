// Face.h: interface for the CFace class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FACE_H__82BF93AE_A82E_407D_AF42_6E6608253C1C__INCLUDED_)
#define AFX_FACE_H__82BF93AE_A82E_407D_AF42_6E6608253C1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFace  
{
public:
	CFace();
	virtual ~CFace();
	void SetEN(int);
public:
	int En;//面的边数
	int *p;//面的顶点号
};

#endif // !defined(AFX_FACE_H__82BF93AE_A82E_407D_AF42_6E6608253C1C__INCLUDED_)
