// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#include "Mainfrm.h"//
#include "math.h"//数学函数
#define  PI 3.14159265//圆周率
#define ROUND(a) int(a+0.5)//四舍五入
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
	ON_WM_TIMER()
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
	Radius=150;FaceCount=4;
	Model=0;//线框
	Switch=0;//无光照
	TranslateX=0;TranslateY=0;TranslateZ=0;
	DirectionX=1;DirectionY=1;DirectionZ=1;
	RotateX=0;RotateY=0;RotateZ=0;
	pMaterial=NULL;pLight=NULL;
	LightNum=1;//光源个数
	pLight=new CLighting(LightNum);//一维光源动态数组	
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(1.0,1.0,1.0);//光源的漫反射颜色	
		pLight->Light[i].L_Specular=CRGB(1.0,1.0,1.0);//光源镜面高光颜色
		pLight->Light[i].L_C0=1.0;//常数衰减系数
		pLight->Light[i].L_C1=0.0000001;//线性衰减系数
		pLight->Light[i].L_C2=0.00000001;//二次衰减系数
		pLight->Light[i].L_OnOff=TRUE;//开启光源	
	}
	pMaterial=new CMaterial;//一维材质动态数组
	pMaterial->M_Ambient=CRGB(0.3,0.3,0.3);//材质对环境光的反射率
	pMaterial->M_Diffuse=CRGB(0.85,0.08,0.0);//材质对漫反射光的反射率
	pMaterial->M_Specular=CRGB(0.828,0.8,0.8);//材质对镜面反射光的反射率
	pMaterial->M_Emit=CRGB(0.2,0.0,0.0);//材质自身发散的颜色
	pMaterial->M_Exp=20.0;//高光指数
}

CTestView::~CTestView()
{	
	if(pLight!=NULL)
	{
		delete[] pLight;
		pLight=NULL;
	}
	if(pMaterial!=NULL)
	{
		delete pMaterial;
		pMaterial=NULL;
	}	
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
	CTestDoc* pDoc=GetDocument();
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

void CTestView::DoubleBuffer()
{
	CDC* pDC=GetDC();
	CRect Rect;//定义客户区
	GetClientRect(&Rect);//获得客户区的大小
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(Rect.Width(),Rect.Height());//设置窗口范围
	pDC->SetViewportExt(Rect.Width(),-Rect.Height());//x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(Rect.Width()/2,Rect.Height()/2);//屏幕中心为原点
	CDC MemDC;//内存DC
	CBitmap NewBitmap,*pOldBitmap;//内存中承载图像的临时位图
	MemDC.CreateCompatibleDC(pDC);//建立与屏幕pDC兼容的MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC,Rect.Width(),Rect.Height());//创建兼容位图 
	pOldBitmap=MemDC.SelectObject(&NewBitmap); //将兼容位图选入MemDC 
	MemDC.SetMapMode(MM_ANISOTROPIC);//MemDC自定义坐标系
	MemDC.SetWindowExt(Rect.Width(),Rect.Height());
	MemDC.SetViewportExt(Rect.Width(),-Rect.Height());
	MemDC.SetViewportOrg(Rect.Width()/2,Rect.Height()/2);
	SetTimer(1,50,NULL);
	ReadPoint();//读入点表
	ReadFace(); //读入面表		
	tran.Translate(TranslateX,TranslateY,TranslateZ);//平移变换
	tran.RotateX(RotateX,CP3(TranslateX,TranslateY,TranslateZ));//绕X轴旋转变换
	tran.RotateY(RotateY,CP3(TranslateX,TranslateY,TranslateZ));//绕Y轴旋转变换
	tran.RotateZ(RotateZ,CP3(TranslateX,TranslateY,TranslateZ));//绕Z轴旋转变换
	BorderCheck();
	DrawObject(&MemDC);	
	pDC->BitBlt(-Rect.Width()/2,-Rect.Height()/2,Rect.Width(),Rect.Height(),&MemDC,-Rect.Width()/2,-Rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	MemDC.DeleteDC();//删除MemDC
	ReleaseDC(pDC);//释放DC
}

void CTestView::ReadPoint()//读入顶点坐标
{
	P[0].x=0;      P[0].y=Radius; P[0].z=0;      P[0].w=1;
	P[1].x=0;      P[1].y=0;      P[1].z= Radius;P[1].w=1;
	P[2].x=Radius; P[2].y=0;      P[2].z=0;      P[2].w=1;
	P[3].x=0;      P[3].y=0;      P[3].z=-Radius;P[3].w=1;
	P[4].x=-Radius;P[4].y=0;      P[4].z=0;      P[4].w=1; 
	P[5].x=0;      P[5].y=-Radius;P[5].z=0;      P[5].w=1; 
	P[6].x=0;      P[6].y=0;      P[6].z=0;      P[6].w=1;//球体中心
	tran.SetMat(P,7);
}

void CTestView::ReadFace()//面表
{
	//第一列为每个面的边数,其余列为面的顶点编号
	F[0].SetEN(3);F[0].p[0]=0;F[0].p[1]=1;F[0].p[2]=2;
	F[1].SetEN(3);F[1].p[0]=0;F[1].p[1]=2;F[1].p[2]=3;
	F[2].SetEN(3);F[2].p[0]=0;F[2].p[1]=3;F[2].p[2]=4;
	F[3].SetEN(3);F[3].p[0]=0;F[3].p[1]=4;F[3].p[2]=1;
	F[4].SetEN(3);F[4].p[0]=5;F[4].p[1]=2;F[4].p[2]=1;
	F[5].SetEN(3);F[5].p[0]=5;F[5].p[1]=3;F[5].p[2]=2;
	F[6].SetEN(3);F[6].p[0]=5;F[6].p[1]=4;F[6].p[2]=3;
	F[7].SetEN(3);F[7].p[0]=5;F[7].p[1]=1;F[7].p[2]=4;
}

void CTestView::DrawObject(CDC *pDC)//绘制物体
{
	CP3 Point[3],t;
	for(int nFace=0;nFace<8;nFace++)//面循环
	{		
		for(int nEdge=0;nEdge<F[nFace].En;nEdge++)//边循环
		{
			Point[nEdge]=P[F[nFace].p[nEdge]];		
		}
		SubDivide(pDC,Point[0],Point[1],Point[2],FaceCount);
	}	
}

void CTestView::SubDivide(CDC *pDC,CP3 p0, CP3 p1, CP3 p2,int n)//递归函数
{
	if(n==0)
	{
		DrawTriangle(pDC,p0,p1,p2);
		return;
	}
	else
	{
		CP3 p01,p12,p20;
		p01=(p0+p1)/2;
		p12=(p1+p2)/2;
		p20=(p2+p0)/2;	
		Normalize(p01);//扩展模长
		Normalize(p12);
		Normalize(p20);
		SubDivide(pDC,p0,p01,p20,n-1);//递归调用
		SubDivide(pDC,p1,p12,p01,n-1);
		SubDivide(pDC,p2,p20,p12,n-1);
		SubDivide(pDC,p01,p12,p20,n-1);
	}
}

void CTestView::Normalize(CP3 &p)//模长标准化函数
{
	p-=P[6];
	double mag=p.Mold();//模长
	if(mag==0)
		return;
	p/=mag;
	p*=Radius;
	p+=P[6];
}

void CTestView::DrawTriangle(CDC *pDC,CP3 p0, CP3 p1, CP3 p2)//绘制三角形函数
{
	CP3 p[3];
	p[0]=p0;p[1]=p1;p[2]=p2;
	CP3 ViewPoint(P[6].x,P[6].y,P[6].z+1000);//计算视矢量，不能取单位矢量
	CVector VS(ViewPoint);//面的视矢量
	CVector V01(p0,p1);//面的一个边矢量
	CVector V12(p1,p2);//面的另一个边矢量
	CVector VN=V01*V12;//面的法矢量	
	if(Dot(VS,VN)>=0 )
	{
		if(Model==0)//线框模型
		{
			if(Switch==0)//光源开关“关”
			{
				for(int i=0;i<LightNum;i++)
				{
					pLight->Light[i].L_OnOff=FALSE;//光源关闭	
				}	
				CLine *line=new CLine;
				line->MoveTo(pDC,p0.x,p0.y);
				line->LineTo(pDC,p1.x,p1.y);
				line->LineTo(pDC,p2.x,p2.y);
				line->LineTo(pDC,p0.x,p0.y);
				delete line;
			}
			else//光源开关“开”
			{
				for(int i=0;i<LightNum;i++)
				{
					pLight->Light[i].L_OnOff=TRUE;//开启光源	
				}	
				pLight->Light[0].L_Position.x=P[6].x+1000;//设置光源位于右上角
				pLight->Light[0].L_Position.y=P[6].y+1000;
				pLight->Light[0].L_Position.z=P[6].z+1000;
				CP2 Point[3];
				for(i=0;i<3;i++)
				{
					p[i].x-=P[6].x;p[i].y-=P[6].y;p[i].z-=P[6].z;
					CVector PNormal(p[i]);//点的位置矢量代表共享该点的所有面的平均法矢量
					p[i].x+=P[6].x;p[i].y+=P[6].y;p[i].z+=P[6].z;
					Point[i].x=p[i].x;
					Point[i].y=ROUND(p[i].y);
					Point[i].c=pLight->Lighting(ViewPoint,p[i],PNormal,pMaterial);//调用光照函数
				}
				AntiColorLine(pDC,Point[0],Point[1]);
				AntiColorLine(pDC,Point[1],Point[2]);
				AntiColorLine(pDC,Point[2],Point[0]);			
			}
		}	
		else//表面模型
		{
			if(Switch==0)//光源开关“关”
			{
				for(int i=0;i<LightNum;i++)
				{
					pLight->Light[i].L_OnOff=FALSE;//关闭光源	
				}			
			}
			else//光源开关“开”
			{
				for(int i=0;i<LightNum;i++)
				{
					pLight->Light[i].L_OnOff=TRUE;//开启光源	
				}
			}			
			pLight->Light[0].L_Position.x=P[6].x+1000;//设置光源位于右上角
			pLight->Light[0].L_Position.y=P[6].y+1000;
			pLight->Light[0].L_Position.z=P[6].z+1000;
			CPi2 Point[3];
			for(int i=0;i<3;i++)
			{
				p[i].x-=P[6].x;p[i].y-=P[6].y;p[i].z-=P[6].z;
				CVector PNormal(p[i]);//点的位置矢量代表共享该点的所有面的平均法矢量
				p[i].x+=P[6].x;p[i].y+=P[6].y;p[i].z+=P[6].z;
				Point[i].x=p[i].x;
				Point[i].y=ROUND(p[i].y);
				Point[i].c=pLight->Lighting(ViewPoint,p[i],PNormal,pMaterial);//调用光照函数
			}
			CFill *fill=new CFill;//填充类对象
			fill->SetPoint(Point,3);//初始化CFill类对象
			fill->CreateBucket();//建立桶表
			fill->CreateEdge();//建立边表
			fill->Gouraud(pDC);//颜色渐变填充三角形
			delete fill;
		}
	}
}

void CTestView::OnTimer(UINT nIDEvent)//定时器函数 
{
	// TODO: Add your message handler code here and/or call default
	CTestDoc* pDoc=GetDocument();
	if(((CMainFrame*)AfxGetMainWnd())->IsPlay)
	{
		Radius=pDoc->m_Radius;
		FaceCount=pDoc->m_FaceCount;
		Model=pDoc->m_Model;
		Switch=pDoc->m_Switch;
		TranslateX+=pDoc->m_TranslateX*DirectionX;
		TranslateY+=pDoc->m_TranslateY*DirectionY;
		TranslateZ+=pDoc->m_TranslateZ*DirectionZ;
		RotateX+=pDoc->m_RotateX;
		RotateY+=pDoc->m_RotateY;
		RotateZ+=pDoc->m_RotateZ;
		Invalidate(FALSE);
	}
	CView::OnTimer(nIDEvent);
}

void CTestView::BorderCheck()//边界碰撞检测
{
	CRect rect;
	GetClientRect(&rect);
	int nWidth=rect.Width()/2;
	int nHeight=rect.Height()/2;
	if(fabs(P[6].x)+Radius>nWidth)
	{
		DirectionX*=-1;
		TranslateX+=fabs(fabs(P[6].x)+Radius-nWidth)*DirectionX;//判断球体水平越界

	}
	if(fabs(P[6].y)+Radius>nHeight)
	{
		DirectionY*=-1;
		TranslateY+=fabs(fabs(P[6].y)+Radius-nHeight)*DirectionY;//判断球体垂直越界
	}	
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//禁止背景刷新 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::AntiColorLine(CDC* pDC,CP2 p0,CP2 p1)//绘制颜色渐变反走样直线
{
	CACLine *aline=new CACLine;
	aline->MoveTo(pDC,p0);
	aline->LineTo(pDC,p1);
	delete aline;
}
