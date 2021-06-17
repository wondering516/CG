// Load3DS.h: interface for the CLoad3DS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOAD3DS_H__7DC86AA7_A7A6_4A1B_8B03_B6D2F85353AB__INCLUDED_)
#define AFX_LOAD3DS_H__7DC86AA7_A7A6_4A1B_8B03_B6D2F85353AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
#include <math.h>
#include <vector>
#include "3DS.h"
#include "Vector.h"
#include "Objects.h"
#include "T2.h"
#include "Chunk.h"
#include "Material3d.h" 

using namespace std;

class CLoad3DS  
{
public:
	CLoad3DS();
	virtual ~CLoad3DS();
public:
	BOOL		Load3DSFile(CString FileName);	    // ����3DS�ļ�
	void		CalculateNormal();					// ���㶥�㷨ʸ��
	CChunk  	ReadChunkHead();					// ������ͷ��Ϣ
	void		ReadFileData(long Length);			// ����Length���ȵ��ļ�����
	void		ReadObjectData(long EndPos);		// ����OBJECT_EDIT���е�����
	void		ReadMaterialData(long EndPos);		// ����MATERIAL_EDIT���е�����
	void		ReadString(char String[]);			// ����һ���ַ���
	BYTE		ReadByte();							// ����һ���ֽ�
	WORD		ReadWord();							// ����һ����
	float		ReadFloat();						// ����һ��������
public:
	int NumOfObject;								// 3ds����ĸ���
	int NumOfMaterial;								// 3ds���ʵĸ���
	vector<CMaterial3d> pMaterial;				    // ����3ds����,����һ��tMaterial���͵�ʸ������
	vector<CObjects *>  pObject;			  		// ����3ds����
	CFile cFile;									// �ļ�����
};


#endif // !defined(AFX_LOAD3DS_H__7DC86AA7_A7A6_4A1B_8B03_B6D2F85353AB__INCLUDED_)
