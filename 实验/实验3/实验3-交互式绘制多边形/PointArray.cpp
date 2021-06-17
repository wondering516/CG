// PointArray.cpp: implementation of the CPointArray class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "PointArray.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPointArray::CPointArray()
{

}

CPointArray::~CPointArray()
{

}

CPointArray::CPointArray(CPoint pt)
{
	this->pt=pt;
}
