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
int CVPixel::VPSize=20;//��ʼ����̬����,���ر߳�Ϊ20
CRect CVPixel::rect=CRect(10,14,810,614);//��ͼ����Χ
CVPixel::CVPixel()
{

}

CVPixel::~CVPixel()
{

}

void CVPixel::DrawGrid(CDC *pDC)//�������񣬲���ʼ��VPixel����
{
	VPixel[0][0].x=rect.left+VPSize/2;//���������ص���������
	VPixel[0][0].y=rect.top+VPSize/2;
	for(int i=0;i<40;i++)//ÿ��40����������
		for(int j=0;j<30;j++) //ÿ��30����������
		{
			VPixel[i][j].x=VPixel[0][0].x+VPSize*i;
			VPixel[i][j].y=VPixel[0][0].y+VPSize*j;
            pDC->Rectangle(&CRect(int(VPixel[i][j].x-VPSize/2),
								  int(VPixel[i][j].y-VPSize/2),
				                  int(VPixel[i][j].x+VPSize/2),
				                  int(VPixel[i][j].y+VPSize/2)));
		}
}

void CVPixel::FillVPiexl(CDC* pDC,CP2 point,CRGB color)//�����������
{  	
	int xGridStart=0,yGridStart=0;//�������Ͻ�����
	int xGridEnd=40,yGridEnd=30;//�������½�����
	if((xGridStart<=point.x) && (xGridEnd>point.x) && (yGridStart<=point.y) && (point.y<yGridEnd)) //Խ���ж�
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