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
	void DrawGrid(CDC *);//��������
	void FillVPiexl(CDC *,CP2,CRGB);//�����������
public:
	static int VPSize;//�������������ر߳�
	static CRect rect;//��ͼ����
	CP2 VPixel[40][30];//������������
};

#endif // !defined(AFX_VPIXEL_H__366C3CF3_AA0D_4495_A3DC_CF19EFFD87B0__INCLUDED_)
