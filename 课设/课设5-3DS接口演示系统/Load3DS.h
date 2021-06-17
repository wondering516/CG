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
	BOOL		Load3DSFile(CString FileName);	    // 载入3DS文件
	void		CalculateNormal();					// 计算顶点法矢量
	CChunk  	ReadChunkHead();					// 读出块头信息
	void		ReadFileData(long Length);			// 读出Length长度的文件数据
	void		ReadObjectData(long EndPos);		// 读出OBJECT_EDIT块中的数据
	void		ReadMaterialData(long EndPos);		// 读出MATERIAL_EDIT块中的数据
	void		ReadString(char String[]);			// 读出一个字符串
	BYTE		ReadByte();							// 读出一个字节
	WORD		ReadWord();							// 读出一个字
	float		ReadFloat();						// 读出一个浮点数
public:
	int NumOfObject;								// 3ds对象的个数
	int NumOfMaterial;								// 3ds材质的个数
	vector<CMaterial3d> pMaterial;				    // 保存3ds材质,定义一个tMaterial类型的矢量容器
	vector<CObjects *>  pObject;			  		// 保存3ds对象
	CFile cFile;									// 文件对象
};


#endif // !defined(AFX_LOAD3DS_H__7DC86AA7_A7A6_4A1B_8B03_B6D2F85353AB__INCLUDED_)
