// VPixel.cpp: implementation of the CVPixel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Test.h"
#include "VPixel.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int CVPixel::VPSize=20;//初始化静态变量,像素边长为20
CRect CVPixel::rect=CRect(10,14,810,614);//绘图区域范围
CVPixel::CVPixel()
{

}

CVPixel::~CVPixel()
{

}

void CVPixel::DrawGrid(CDC *pDC)//绘制网格，并初始化VPixel数组
{
	VPixel[0][0].x=rect.left+VPSize/2;//正方形像素的中心坐标
	VPixel[0][0].y=rect.top+VPSize/2;
	for(int i=0;i<40;i++)//每行40个虚拟像素
		for(int j=0;j<30;j++) //每列30个虚拟像素
		{
			VPixel[i][j].x=VPixel[0][0].x+VPSize*i;
			VPixel[i][j].y=VPixel[0][0].y+VPSize*j;
            pDC->Rectangle(&CRect(int(VPixel[i][j].x-VPSize/2),
								  int(VPixel[i][j].y-VPSize/2),
				                  int(VPixel[i][j].x+VPSize/2),
				                  int(VPixel[i][j].y+VPSize/2)));
		}
}

void CVPixel::FillVPiexl(CDC* pDC,CP2 point,CRGB color)//填充虚拟像素
{  	
	int xGridStart=0,yGridStart=0;//网格左上角坐标
	int xGridEnd=40,yGridEnd=30;//网格右下角坐标
	if((xGridStart<=point.x) && (xGridEnd>point.x) && (yGridStart<=point.y) && (point.y<yGridEnd)) //越界判断
	{
		int i=int(point.x),j=int(point.y); 
		CBrush NewBrush,*pOldBrush;
		NewBrush.CreateSolidBrush(RGB(color.red*255,color.green*255,color.blue*255));
		pOldBrush=pDC->SelectObject(&NewBrush);
		pDC->Rectangle(&CRect(int(VPixel[i][j].x-VPSize/2),
			                  int(VPixel[i][j].y-VPSize/2),
			                  int(VPixel[i][j].x+VPSize/2),
			                  int(VPixel[i][j].y+VPSize/2)));
		pDC->SelectObject(pOldBrush);
		NewBrush.DeleteObject();	
	}
}