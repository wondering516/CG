// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"

#include "TestDoc.h"
#include "TestView.h"

#include "cmath"
#define  PI 3.1415926
#define  ROUND(x)  int(x+0.5)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestView

IMPLEMENT_DYNCREATE(CTestView, CView)

BEGIN_MESSAGE_MAP(CTestView, CView)
	//{{AFX_MSG_MAP(CTestView)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_MENUPlay, OnMENUPlay)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_MENUPlay, OnUpdateMENUPlay)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestView construction/destruction

CTestView::CTestView()
{
	// TODO: add construction code here
	P=NULL;F=NULL;
	Play=FALSE;
	R=1000,d=800,Phi=90.0,Theta=0.0;	
}

CTestView::~CTestView()
{	
	if(P!=NULL)
	{
		delete[] P;
		P=NULL;
	}
	for(int n=0;n<N1;n++)//注意撤销次序,先列后行,与设置相反
	{
		delete[] F[n];
		F[n]=NULL;
	}
	delete[] F;
	F=NULL;
}

BOOL CTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTestView drawing

void CTestView::OnDraw(CDC* pDC)
{
	CTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	DoubleBuffer();	
}

/////////////////////////////////////////////////////////////////////////////
// CTestView printing

BOOL CTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTestView diagnostics

#ifdef _DEBUG
void CTestView::AssertValid() const
{
	CView::AssertValid();
}

void CTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestDoc* CTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestDoc)));
	return (CTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTestView message handlers
void CTestView::DoubleBuffer()//双缓冲
{
	CDC* pDC=GetDC();
	CRect rect;//定义客户区
	GetClientRect(&rect);//获得客户区的大小
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(rect.Width(),rect.Height());//设置窗口范围
	pDC->SetViewportExt(rect.Width(),-rect.Height());//x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//屏幕中心为原点
	CDC MemDC;//内存DC
	CBitmap NewBitmap,*pOldBitmap;//内存中承载图像的临时位图
	MemDC.CreateCompatibleDC(pDC);//建立与屏幕pDC兼容的MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//创建兼容位图 
	pOldBitmap=MemDC.SelectObject(&NewBitmap); //将兼容位图选入MemDC 
	MemDC.FillSolidRect(rect,pDC->GetBkColor());//按原来背景填充客户区，否则是黑色
	MemDC.SetMapMode(MM_ANISOTROPIC);//MemDC自定义坐标系
	MemDC.SetWindowExt(rect.Width(),rect.Height());
	MemDC.SetViewportExt(rect.Width(),-rect.Height());
	MemDC.SetViewportOrg(rect.Width()/2,rect.Height()/2);
	DrawObject(&MemDC);
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	MemDC.DeleteDC();//删除MemDC
	ReleaseDC(pDC);//释放DC
}

void CTestView::ReadPoint()//读入点坐标
{
	int gafa=4,gbeta=4;//面片夹角
	N1=180/gafa,N2=360/gbeta;//N1为纬度区域,N2为经度区域
	P=new CP3[(N1-1)*N2+2];//P为球的顶点
	//纬度方向除南北极点外有"N1－1"个点，"2"代表南北极两个点
	double gafa1,gbeta1,r=300;//r为球体半径
	//计算北极点坐标
	P[0].x=0,P[0].y=r,P[0].z=0;
	//按行循环计算球体上的点坐标
	for(int i=0;i<N1-1;i++)
	{
		gafa1=(i+1)*gafa*PI/180;
		for(int j=0;j<N2;j++)
	    {
			gbeta1=j*gbeta*PI/180;		
			P[i*N2+j+1].x=r*sin(gafa1)*sin(gbeta1);
			P[i*N2+j+1].y=r*cos(gafa1);	
			P[i*N2+j+1].z=r*sin(gafa1)*cos(gbeta1);
		}
	}
	//计算南极点坐标
	P[(N1-1)*N2+1].x=0,P[(N1-1)*N2+1].y=-r,P[(N1-1)*N2+1].z=0;	
}

void CTestView::ReadFace()//读入面表
{
	//设置二维动态数组
	F=new CFace *[N1];//设置行
	for(int n=0;n<N1;n++)
	{
		F[n]=new CFace[N2];//设置列
	}
	for(int j=0;j<N2;j++)//构造北极三角形面片
	{
		int tempj=j+1;
		if(tempj==N2) tempj=0;//面片的首尾连接
		int NorthIndex[3];//北极三角形面片索引号数组
		NorthIndex[0]=0;
		NorthIndex[1]=j+1;
		NorthIndex[2]=tempj+1;
		F[0][j].SetEN(3);
		for(int k=0;k<F[0][j].En;k++)
		{
			F[0][j].p[k]=NorthIndex[k];
		}
		F[0][j].SetNormal(P[NorthIndex[0]],P[NorthIndex[1]],P[NorthIndex[2]]);//计算小面片法矢量
	}
	for(int i=1;i<N1-1;i++)//构造球体四边形面片
	{
		for(int j=0;j<N2;j++)
	    {
			int tempi=i+1;
			int tempj=j+1;
			if(tempj==N2) tempj=0;
			int BodyIndex[4];//球体四边形面片索引号数组
			BodyIndex[0]=(i-1)*N2+j+1;
			BodyIndex[1]=(tempi-1)*N2+j+1;
			BodyIndex[2]=(tempi-1)*N2+tempj+1;
			BodyIndex[3]=(i-1)*N2+tempj+1;
			F[i][j].SetEN(4);
			for(int k=0;k<F[i][j].En;k++)
			{
				F[i][j].p[k]=BodyIndex[k];
			}
			F[i][j].SetNormal(P[BodyIndex[0]],P[BodyIndex[1]],P[BodyIndex[2]]);//计算小面片法矢量
		}
	}
	for(j=0;j<N2;j++)//构造南极三角形面片
	{
		int tempj=j+1;
		if(tempj==N2) tempj=0;
		int SouthIndex[3];//南极三角形面片索引号数组
		SouthIndex[0]=(N1-2)*N2+j+1;
		SouthIndex[1]=(N1-1)*N2+1;
		SouthIndex[2]=(N1-2)*N2+tempj+1;
		F[N1-1][j].SetEN(3);	
		for(int k=0;k<F[N1-1][j].En;k++)
		{
			F[N1-1][j].p[k]=SouthIndex[k];
		}
		F[N1-1][j].SetNormal(P[SouthIndex[0]],P[SouthIndex[1]],P[SouthIndex[2]]);//计算小面片法矢量
	}
}

void CTestView::InitParameter()//透视变换参数初始化
{
	k[1]=sin(PI*Theta/180);//sin(theta)
	k[2]=sin(PI*Phi/180);//sin(phi)
	k[3]=cos(PI*Theta/180);//cos(theta)
	k[4]=cos(PI*Phi/180);//cos(phi)
	k[5]=k[2]*k[3];//sin(phi)*cos(theta)
	k[6]=k[2]*k[1];//sin(phi)*sin(theta)
	k[7]=k[4]*k[3];//cos(phi)*cos(theta)
	k[8]=k[4]*k[1];//cos(phi)*sin(theta)
	ViewPoint.x=R*k[6];//用户坐标系的视点球坐标
	ViewPoint.y=R*k[4];
	ViewPoint.z=R*k[5];
}

void CTestView::PerProject(CP3 P)//透视变换
{
	CP3 ViewP;
	ViewP.x=P.x*k[3]-P.z*k[1];//观察坐标系三维坐标
	ViewP.y=-P.x*k[8]+P.y*k[2]-P.z*k[7];
	ViewP.z=-P.x*k[6]-P.y*k[4]-P.z*k[5]+R;
	ScreenP.x=d*ViewP.x/ViewP.z;//屏幕坐标系二维坐标
	ScreenP.y=d*ViewP.y/ViewP.z;
}

void CTestView::DrawObject(CDC *pDC)//绘制球体
{
	CLine *line=new CLine;
    CP2 Point3[3],t3;//南北极顶点数组
	CP2 Point4[4],t4;//球体顶点数组
	for(int i=0;i<N1;i++)
	{
		for(int j=0;j<N2;j++)
		{
			CVector VS(P[F[i][j].p[1]],ViewPoint);//面的视矢量
			if(Dot(VS,F[i][j].Normal)>=0)//背面剔除
			{
				if(F[i][j].En==3)//三角形面片
				{
					for(int m=0;m<F[i][j].En;m++)
					{
						PerProject(P[F[i][j].p[m]]);
						Point3[m]=ScreenP;						
					}			
					for(int n=0;n<3;n++)
					{
						if(n==0)
						{
							line->MoveTo(pDC,Point3[n]);
							t3=Point3[n];
						}
						else
						{
							line->LineTo(pDC,Point3[n]);
						}		
					}
					line->LineTo(pDC,t3);//闭合多边形
				}
				else//四边形面片
				{
					for(int m=0;m<F[i][j].En;m++)
					{
						PerProject(P[F[i][j].p[m]]);
						Point4[m]=ScreenP;						
					}
					
					for(int n=0;n<4;n++)
					{
						if(n==0)
						{
							line->MoveTo(pDC,Point4[n]);
							t4=Point4[n];
						}
						else
						{
							line->LineTo(pDC,Point4[n]);
						}		
					}
					line->LineTo(pDC,t4);//闭合多边形
				}
			} 	
		}	
	}
	delete line;
}

void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(!Play)
	{
		switch(nChar)
		{
		case VK_UP:
			Phi+=5;//设定步长
			break;
		case VK_DOWN:
			Phi-=5;
			break;
		case VK_LEFT:
			Theta+=5;
			break;
		case VK_RIGHT:
		    Theta-=5;
			break;
		default:
			break;			
		}
		InitParameter();
		Invalidate(FALSE);	
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTestView::OnMENUPlay() 
{
	// TODO: Add your command handler code here
	Play=Play?FALSE:TRUE;
	if(Play)//设置定时器
		SetTimer(1,150,NULL);	
	else
		KillTimer(1);
}

void CTestView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	Phi-=5;//设定步长
	Theta-=5;
	InitParameter();
	Invalidate(FALSE);	
	CView::OnTimer(nIDEvent);
}

void CTestView::OnUpdateMENUPlay(CCmdUI* pCmdUI)//动画按钮状态函数  
{
	// TODO: Add your command update UI handler code here
	if(Play)
	{
		pCmdUI->SetCheck(TRUE);
		pCmdUI->SetText("停止");
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->SetText("播放");
	}		
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//禁止背景刷新 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadPoint();
	ReadFace();
	InitParameter();
}
