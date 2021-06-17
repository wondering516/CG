// Face.cpp: implementation of the CFace class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "Face.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFace::CFace()
{
	p=NULL;
}

CFace::~CFace()
{
	if(p!=NULL)
	{
		delete[] p;
		p=NULL;
	}
}

void CFace::SetEN(int en)
{
	En=en;
	p=new int[En];
}