// Material3d.h: interface for the CMaterial3d class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATERIAL3D_H__3E6AB0A0_372A_4111_AFF3_4F2997D60718__INCLUDED_)
#define AFX_MATERIAL3D_H__3E6AB0A0_372A_4111_AFF3_4F2997D60718__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMaterial3d  //3ds材质类
{
public:
	CMaterial3d();
	virtual ~CMaterial3d();
public:
	char	MaterialName[255];//材质的名称
	char	TextureName[255]; //纹理的名称
	BYTE	DifColor[3];	  //材质漫反射颜色
	UINT	TextureData;	  //存放纹理的数据
	bool	HasTexture;		  //该材质是否包含纹理
};

#endif // !defined(AFX_MATERIAL3D_H__3E6AB0A0_372A_4111_AFF3_4F2997D60718__INCLUDED_)
