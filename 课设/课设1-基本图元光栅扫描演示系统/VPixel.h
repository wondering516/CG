// VPixel.h: interface for the CVPixel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VPIXEL_H__366C3CF3_AA0D_4495_A3DC_CF19EFFD87B0__INCLUDED_)
#define AFX_VPIXEL_H__366C3CF3_AA0D_4495_A3DC_CF19EFFD87B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "P2.h"

class CVPixel  
{
public:
	CVPixel();
	virtual ~CVPixel();
	void DrawGrid(CDC *);//绘制网格
	void FillVPiexl(CDC *,CP2,CRGB);//填充虚拟像素
public:
	static int VPSize;//虚拟正方形像素边长
	static CRect rect;//绘图区域
	CP2 VPixel[40][30];//虚拟像素数组
};

#endif // !defined(AFX_VPIXEL_H__366C3CF3_AA0D_4495_A3DC_CF19EFFD87B0__INCLUDED_)
