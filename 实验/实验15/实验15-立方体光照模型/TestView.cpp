// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#define ROUND(a) int(a+0.5)//四舍五入
#define PI 3.1415926//圆周率
#include "math.h"//数学头文件
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
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MPLAY, OnMplay)
	ON_UPDATE_COMMAND_UI(ID_MPLAY, OnUpdateMplay)
	ON_COMMAND(ID_MLRED, OnMlred)
	ON_COMMAND(ID_MMEMERALD, OnMmemerald)
	ON_COMMAND(ID_MMRUBY, OnMmruby)
	ON_COMMAND(ID_MLGREEN, OnMlgreen)
	ON_COMMAND(ID_MLBLUE, OnMlblue)
	ON_COMMAND(ID_MMSAPPHIRE, OnMmsapphire)
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
	Play=FALSE;//动画按钮状态
	R=800.0,d=1000,Phi=90.0,Theta=0.0;//视点位置球坐标正前方	
	LightNum=1;//光源数量
	pLight=new CLighting(LightNum);//光源动态数组
	pLight->Light[0].SetPosition(0,0,1000);//光源位置坐标
	//设置光源参数
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(0.8,0.8,0.8);//光源的漫反射颜色	
		pLight->Light[i].L_Specular=CRGB(0.508,0.508,0.508);//光源镜面高光颜色
		pLight->Light[i].L_C0=1.0;//常数衰减系数
		pLight->Light[i].L_C1=0.0000001;//线性衰减系数
		pLight->Light[i].L_C2=0.00000001;//二次衰减系数
		pLight->Light[i].L_OnOff=TRUE;//光源开启
	}	
	//设置材质参数
	pMaterial=new CMaterial;//材质指针
	pMaterial->M_Ambient=CRGB(0.192,0.192,0.192);//材质对环境光的反射率
	pMaterial->M_Diffuse=CRGB(0.508,0.508,0.508);//材质对漫反射光的反射率
	pMaterial->M_Specular=CRGB(1.0,1.0,1.0);//材质对镜面反射光的反射率
	pMaterial->M_Emit=CRGB(0.2,0.2,0.2);//材质自身发散的颜色
	pMaterial->M_Exp=20.0;//高光指数
}

CTestView::~CTestView()
{
	if(pLight!=NULL)
	{
		delete [] pLight;
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

void CTestView::ReadPoint()//点表
{
	double a=200;//立方体边长
	//顶点的三维坐标(x,y,z)
	P[0].x=-a/2;P[0].y=-a/2;P[0].z=-a/2;
	P[1].x=+a/2;P[1].y=-a/2;P[1].z=-a/2;
	P[2].x=+a/2;P[2].y=+a/2;P[2].z=-a/2;
	P[3].x=-a/2;P[3].y=+a/2;P[3].z=-a/2;
	P[4].x=-a/2;P[4].y=-a/2;P[4].z=+a/2;
	P[5].x=+a/2;P[5].y=-a/2;P[5].z=+a/2;
	P[6].x=+a/2;P[6].y=+a/2;P[6].z=+a/2;
	P[7].x=-a/2;P[7].y=+a/2;P[7].z=+a/2;
}
void CTestView::ReadFace()//面表
{
	//面的边数、面的顶点编号
	F[0].SetEN(4);F[0].p[0]=4;F[0].p[1]=5;F[0].p[2]=6;F[0].p[3]=7;//前面
	F[1].SetEN(4);F[1].p[0]=0;F[1].p[1]=3;F[1].p[2]=2;F[1].p[3]=1;//后面
	F[2].SetEN(4);F[2].p[0]=0;F[2].p[1]=4;F[2].p[2]=7;F[2].p[3]=3;//左面
	F[3].SetEN(4);F[3].p[0]=1;F[3].p[1]=2;F[3].p[2]=6;F[3].p[3]=5;//右面
	F[4].SetEN(4);F[4].p[0]=2;F[4].p[1]=3;F[4].p[2]=7;F[4].p[3]=6;//顶面
	F[5].SetEN(4);F[5].p[0]=0;F[5].p[1]=1;F[5].p[2]=5;F[5].p[3]=4;//底面
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
	ViewP.c=P.c;
    ScreenP.x=d*ViewP.x/ViewP.z;//屏幕坐标系三维坐标
	ScreenP.y=ROUND(d*ViewP.y/ViewP.z);
	ScreenP.z=d*(R-ViewP.z)/ViewP.z;
	ScreenP.c=ViewP.c;
}

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

void CTestView::DrawObject(CDC* pDC)//绘制立方体
{
	CZBuffer *zbuf=new CZBuffer;//申请内存
	zbuf->InitDeepBuffer(1000,1000,-1000);//深度初始化
	for(int nFace=0;nFace<6;nFace++)
	{
		CVector VS(ViewPoint);//面的视矢量
		CPi3  Point[4];
		CVector V01(P[F[nFace].p[0]],P[F[nFace].p[1]]);//面的一个边矢量
		CVector V12(P[F[nFace].p[1]],P[F[nFace].p[2]]);//面的另一个边矢量	
		CVector VN=V01*V12;//面的法矢量	
		if(Dot(VS,VN)>0.0)//背面剔除
		{
			for(int nEdge=0;nEdge<F[nFace].En;nEdge++)
			{
				PerProject(P[F[nFace].p[nEdge]]);//透视变换
				Point[nEdge]=ScreenP;
				Point[nEdge].c=pLight->Lighting(ViewPoint,P[F[nFace].p[nEdge]],VN,pMaterial);
			}
			zbuf->SetPoint(Point,4);//设置顶点			
			zbuf->CreateBucket();//建立桶表
			zbuf->CreateEdge();//建立边表
			zbuf->Gouraud(pDC);//填充四边形	
			zbuf->ClearMemory();//内存清理			
		}
	}
	delete zbuf;
}
void CTestView::OnLButtonDown(UINT nFlags, CPoint point)//鼠标左键函数
{
	// TODO: Add your message handler code here and/or call default
	R=R+100;
	InitParameter();
	Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnRButtonDown(UINT nFlags, CPoint point)//鼠标右键函数 
{
	// TODO: Add your message handler code here and/or call default
	R=R-100;
	InitParameter();
	Invalidate(FALSE);
	CView::OnRButtonDown(nFlags, point);
}

void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(!Play)
	{
		switch(nChar)
		{
		case VK_UP:
			afa=-5;
			tran.RotateX(afa);
			break;
		case VK_DOWN:
		    afa=5;
			tran.RotateX(afa);
			break;
		case VK_LEFT:
		    beta=-5;
			tran.RotateY(beta);
			break;
		case VK_RIGHT:
		    beta=5;
			tran.RotateY(beta);
			break;
		default:
			break;
		}
		Invalidate(FALSE);
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTestView::OnMplay()//动画按钮函数 
{
	// TODO: Add your command handler code here
	Play=Play?FALSE:TRUE;
	if (Play)//设置定时器
		SetTimer(1,150,NULL);	
	else
		KillTimer(1);
}

void CTestView::OnTimer(UINT nIDEvent)//动画时间函数 
{
	// TODO: Add your message handler code here and/or call default
	afa=5;beta=5;
	tran.RotateX(afa);
	tran.RotateY(beta);
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}

void CTestView::OnUpdateMplay(CCmdUI* pCmdUI)//动画按钮状态函数 
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

void CTestView::OnMlred()//设置红色光源 
{
	// TODO: Add your command handler code here
	pLight->Light[0].SetSpecular(CRGB(1.0,0.0,0.0));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLRED,MF_CHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLGREEN,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLBLUE,MF_UNCHECKED);
	Invalidate(FALSE);	
}

void CTestView::OnMlgreen()//设置绿色光源  
{
	// TODO: Add your command handler code here
	pLight->Light[0].SetSpecular(CRGB(0.0,1.0,0.0));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLRED,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLGREEN,MF_CHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLBLUE,MF_UNCHECKED);
	Invalidate(FALSE);	
}

void CTestView::OnMlblue()//设置蓝色光源  
{
	// TODO: Add your command handler code here
	pLight->Light[0].SetSpecular(CRGB(0.0,0.0,1.0));
 	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLRED,MF_UNCHECKED);
 	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLGREEN,MF_UNCHECKED);
 	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MLBLUE,MF_CHECKED);
  	Invalidate(FALSE);
}

void CTestView::OnMmruby()//设置红宝石材质 
{
	// TODO: Add your command handler code here
	pMaterial->SetDiffuse(CRGB(1.0,0.0,0.0));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMSAPPHIRE,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMEMERALD,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMRUBY,MF_CHECKED);
	Invalidate(FALSE);	
}

void CTestView::OnMmemerald()//设置绿宝石材质 
{
	// TODO: Add your command handler code here
	pMaterial->SetDiffuse(CRGB(0.0,1.0,0.0));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMSAPPHIRE,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMEMERALD,MF_CHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMRUBY,MF_UNCHECKED);
	Invalidate(FALSE);	
}

void CTestView::OnMmsapphire()//设置蓝宝石材质 
{
	// TODO: Add your command handler code here
	pMaterial->SetDiffuse(CRGB(0.0,0.0,1.0));
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMSAPPHIRE,MF_CHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMEMERALD,MF_UNCHECKED);
	AfxGetMainWnd()->GetMenu()->CheckMenuItem(ID_MMRUBY,MF_UNCHECKED);
	Invalidate(FALSE);
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadPoint();
	ReadFace();	
	tran.SetMat(P,8);
	InitParameter();
}

