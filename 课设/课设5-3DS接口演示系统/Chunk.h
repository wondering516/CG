// Chunk.h: interface for the CChunk class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHUNK_H__45F2EA07_4019_434F_97DD_C1FB48456905__INCLUDED_)
#define AFX_CHUNK_H__45F2EA07_4019_434F_97DD_C1FB48456905__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CChunk  //����
{
public:
	CChunk();
	virtual ~CChunk();
public:	
	WORD  ID;    //���ID,ռ2���ֽ�
	UINT Length; //���ݿ��λ�ã�ռ4���ֽ�
};

#endif // !defined(AFX_CHUNK_H__45F2EA07_4019_434F_97DD_C1FB48456905__INCLUDED_)
