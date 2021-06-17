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
	Model=1;//表面
	R=200,d=800,Phi=90.0,Theta=0.0;
	afa=0.0;beta=0.0;
	pMaterial=NULL;pLight=NULL;
	LightNum=1;//光源个数
	pLight=new CLighting(LightNum);//一维光源动态数组
	pLight->Light[0].SetPosition(0,800,800);//设置第一个光源位置坐标
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
	pMaterial->M_Emit=CRGB(0.0,0.0,0.0);//材质自身发散的颜色
	pMaterial->M_Exp=20.0;//高光指数
	if(load3ds.Load3DSFile("football.3DS")==FALSE)//读取3ds文件
		exit(1);
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
	pOldBitmap=MemDC.SelectObject(&NewBitmap);//将兼容位图选入MemDC 
	MemDC.SetMapMode(MM_ANISOTROPIC);//MemDC自定义坐标系
	MemDC.SetWindowExt(rect.Width(),rect.Height());
	MemDC.SetViewportExt(rect.Width(),-rect.Height());
	MemDC.SetViewportOrg(rect.Width()/2,rect.Height()/2);
	SetTimer(1,15,NULL);
	DrawObject(&MemDC);
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	MemDC.DeleteDC();//删除MemDC
	ReleaseDC(pDC);//释放DC
}

void CTestView::InitParameter()
{
	k[1]=sin(PI*Theta/180);
	k[2]=sin(PI*Phi/180);
	k[3]=cos(PI*Theta/180);
	k[4]=cos(PI*Phi/180);
	k[5]=k[2]*k[3];
	k[6]=k[2]*k[1];
	k[7]=k[4]*k[3];
	k[8]=k[4]*k[1];
	ViewPoint.x=R*k[6];//用户坐标系的视点球坐标
	ViewPoint.y=R*k[4];
	ViewPoint.z=R*k[5];
}

void CTestView::PerProject(CP3 P)
{
	CP3 ViewP;
	ViewP.x=P.x*k[3]-P.z*k[1];
	ViewP.y=-P.x*k[8]+P.y*k[2]-P.z*k[7];
	ViewP.z=-P.x*k[6]-P.y*k[4]-P.z*k[5]+R;//世界坐标系到观察坐标系
    ViewP.c=P.c;
	ScreenP.x=d*ViewP.x/ViewP.z;//屏幕坐标系二维坐标
	ScreenP.y=ROUND(d*ViewP.y/ViewP.z);
    ScreenP.z=d*(R-ViewP.z)/ViewP.z;
	ScreenP.c=ViewP.c;
}

void CTestView::DrawObject(CDC *pDC)//绘制物体
{
	CZBuffer *zbuf=new CZBuffer;//申请内存
	zbuf->InitDeepBuffer(800,800,-1000);//深度初始化
	for(int i=0;i<load3ds.NumOfObject;i++)//足球由12个正五边形和20个正六边形组成
	{
		CObjects *Obj=load3ds.pObject[i];
		CVector	*pNormal=Obj->pVertNormal;
		CP3	*pVertex=Obj->pVertex;		
		for(int j=0;j<Obj->NumOfFace;j++)//访问物体的每一个三角形面片
		{ 
			CP3 temp;
			CPi3 Point[3];
			CVector Normal3[3];
			CMaterial3d CurrentMat=load3ds.pMaterial[Obj->pFace[j].MaterialIndex];//面的材质信息
			for(int k=0;k<3;k++)//计算三角形面片的顶点和法矢量
			{
				P[k]=pVertex[Obj->pFace[j].p[k]];
				Normal3[k]=pNormal[Obj->pFace[j].p[k]];
			}
			CP3 PT=CP3(P[1].x,P[1].y,P[1].z);
			CVector VS(PT,ViewPoint);//面的视矢量
			if(Dot(VS,Obj->pFaceNormal[j])>=0)//背面剔除
			{				
				for(int k=0;k<3;k++)//绘制三角形
				{
					CRGB MaterialClr(CurrentMat.DifColor[0]/255.0,CurrentMat.DifColor[1]/255.0,CurrentMat.DifColor[2]/255.0);
					pMaterial->SetDiffuse(MaterialClr);
					Point[k].c=pLight->Lighting(ViewPoint,P[k],Normal3[k],pMaterial);//调用光照函数计算顶点的颜色
					PerProject(P[k]);//透视变换
					Point[k].x=ScreenP.x;
					Point[k].y=ROUND(ScreenP.y);
					Point[k].z=ScreenP.z;
				}				
				if(Model)//绘制表面模型
				{					
					zbuf->SetPoint(Point,3);			
					zbuf->CreateBucket();//建立桶表
					zbuf->CreateEdge();//建立边表
					zbuf->Gouraud(pDC);//颜色渐变填充
					zbuf->ClearMemory();		
				}
				else//绘制线框模型
				{
					CLine *line=new CLine;
					line->MoveTo(pDC,Point[0].x,Point[0].y);
					for(int i=1;i<4;++i)
						line->LineTo(pDC,Point[i%3].x,Point[i%3].y);
					delete line;
				}
			}			
		}
		tran.SetMat(load3ds.pObject[i]->pVertex,load3ds.pObject[i]->NumOfVertex);
		tran.RotateX(afa);//x方向旋转足球
		tran.RotateY(beta); //y方向旋转足球
	}	
	load3ds.CalculateNormal();
	delete zbuf;//释放内存
}

void CTestView::OnTimer(UINT nIDEvent)//定时器函数 
{
	// TODO: Add your message handler code here and/or call default
	CTestDoc* pDoc=GetDocument();
	if(((CMainFrame*)AfxGetMainWnd())->IsPlay)
	{
		Model=pDoc->m_Model;
		afa=5;beta=5;
		Invalidate(FALSE);
	}
	CView::OnTimer(nIDEvent);
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
	for(int i=0;i<load3ds.NumOfObject;i++)
	{
		tran.SetMat(load3ds.pObject[i]->pVertex,load3ds.pObject[i]->NumOfVertex);
		tran.RotateX(100);//初始位置
		tran.RotateY(0);
	}	
	load3ds.CalculateNormal();
	InitParameter();	
}
