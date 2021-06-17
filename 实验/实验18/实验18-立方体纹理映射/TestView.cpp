// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#define ROUND(a) int(a+0.5)//四舍五入
#define PI 3.1415926//圆周率
#include "cmath"//数学头文件

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
	ON_WM_RBUTTONDBLCLK()
	ON_COMMAND(ID_MENUPlay, OnMENUPlay)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_MENUPlay, OnUpdateMENUPlay)
	ON_WM_KEYDOWN()
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
	Play=FALSE;
	R=800;d=1000;Phi=90;Theta=0;	
	afa=0.0;beta=0.0;
}

CTestView::~CTestView()
{

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
	double a=250;//立方体边长
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
	F[0].SetEN(4);F[0].p[0]=4;F[0].p[1]=5;F[0].p[2]=6;F[0].p[3]=7;//前面顶点索引
	F[0].t[0]=CP2(0,511);F[0].t[1]=CP2(511,511);F[0].t[2]=CP2(511,0);F[0].t[3]=CP2(0,0);//前面纹理坐标
	F[1].SetEN(4);F[1].p[0]=0;F[1].p[1]=3;F[1].p[2]=2;F[1].p[3]=1;//后面顶点索引
	F[1].t[0]=CP2(511,511);F[1].t[1]=CP2(511,0);F[1].t[2]=CP2(0,0);F[1].t[3]=CP2(0,511);//后面纹理坐标
	F[2].SetEN(4);F[2].p[0]=0;F[2].p[1]=4;F[2].p[2]=7;F[2].p[3]=3;//左面顶点索引
	F[2].t[0]=CP2(0,511);F[2].t[1]=CP2(511,511);F[2].t[2]=CP2(511,0);F[2].t[3]=CP2(0,0);//左面纹理坐标
	F[3].SetEN(4);F[3].p[0]=1;F[3].p[1]=2;F[3].p[2]=6;F[3].p[3]=5;//右面顶点索引
	F[3].t[0]=CP2(511,511);F[3].t[1]=CP2(511,0);F[3].t[2]=CP2(0,0);F[3].t[3]=CP2(0,511);//右面纹理坐标
	F[4].SetEN(4);F[4].p[0]=2;F[4].p[1]=3;F[4].p[2]=7;F[4].p[3]=6;//顶面顶点索引
	F[4].t[0]=CP2(511,0);F[4].t[1]=CP2(0,0);F[4].t[2]=CP2(0,511);F[4].t[3]=CP2(511,511);//顶面纹理坐标
	F[5].SetEN(4);F[5].p[0]=0;F[5].p[1]=1;F[5].p[2]=5;F[5].p[3]=4;//底面顶点索引
	F[5].t[0]=CP2(0,511);F[5].t[1]=CP2(511,511);F[5].t[2]=CP2(511,0);F[5].t[3]=CP2(0,0);//底面纹理坐标
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
	ViewP.x=P.x*k[3]-P.z*k[1];
	ViewP.y=-P.x*k[8]+P.y*k[2]-P.z*k[7];
	ViewP.z=-P.x*k[6]-P.y*k[4]-P.z*k[5]+R;//世界坐标系到观察坐标系
    ViewP.c=P.c;
	ScreenP.x=d*ViewP.x/ViewP.z;//屏幕坐标系二维坐标
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
	CPi3 Point[4];//面的顶点坐标
	CP2  Texture[4];//面的纹理坐标
	CZBuffer *zbuf=new CZBuffer;
	zbuf->InitDeepBuffer(800,800,-1000);
	for(int nFace=0;nFace<6;nFace++)
	{	
		CVector VS(P[F[nFace].p[1]],ViewPoint);//面的视矢量
		CVector V01(P[F[nFace].p[0]],P[F[nFace].p[1]]);//面的一条边矢量
		CVector V12(P[F[nFace].p[1]],P[F[nFace].p[2]]);//面的另一条边矢量		
		CVector VN=V01*V12;//面的法矢量
		if(Dot(VS,VN)>=0)//背面剔除
		{
			for(int nEdge=0;nEdge<F[nFace].En;nEdge++)//边循环
			{
				PerProject(P[F[nFace].p[nEdge]]);
				Point[nEdge]=ScreenP;
				Texture[nEdge]=F[nFace].t[nEdge];
			}
			ReadImage(nFace);
			zbuf->SetPoint(Point,Texture,4);//初始化（绑定顶点和各个顶点的纹理坐标点）
			zbuf->CreateBucket();//创建桶表
			zbuf->CreateEdge();//创建边表
			zbuf->TextureMap(pDC,Image);//纹理映射
			zbuf->ClearMemory();
			ClearImaMem();
		}
	}	
	delete zbuf;
}

void CTestView::ReadImage(int nface)//读入BMP图片
{
	BYTE Texture[]={IDB_FRONT,IDB_BACK,IDB_LEFT,IDB_RIGHT,IDB_TOP,IDB_BOTTOM};
	CBitmap NewBitmap;
	NewBitmap.LoadBitmap(Texture[nface]);//调入位图资源
	NewBitmap.GetBitmap(&bmp);//获得CBitmap的信息到Bitmap结构体中
	int nbytesize=(bmp.bmWidth*bmp.bmHeight*bmp.bmBitsPixel+7)/8;//获得位图的总字节数
	im=new BYTE[nbytesize];//开辟装载位图的缓冲区
	NewBitmap.GetBitmapBits(nbytesize,(LPVOID)im);//将位图拷贝到缓冲区
	Image=new COLORREF*[bmp.bmHeight];//建立二维颜色数组
	for(int n1=0;n1<bmp.bmHeight;n1++)
	{
		Image[n1]=new COLORREF[bmp.bmWidth];		
	}
	for(n1=bmp.bmHeight-1;n1>=0;n1--)//位图高度
	{
		for(int n2=0;n2<=bmp.bmWidth-1;n2++)//位图宽度
		{
			int pos=n1*bmp.bmWidthBytes+4*n2;//位置
			Image[n1][n2]=RGB(im[pos+2],im[pos+1],im[pos]);	
		
		}
	}
	delete []im;	
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point)//鼠标左键函数
{
	// TODO: Add your message handler code here and/or call default
	R+=100;
	Invalidate(FALSE);
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnRButtonDblClk(UINT nFlags, CPoint point)//鼠标右键函数 
{
	// TODO: Add your message handler code here and/or call default
	R-=100;
	Invalidate(FALSE);
	CView::OnRButtonDblClk(nFlags, point);
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

void CTestView::OnMENUPlay()//动画按钮函数 
{
	// TODO: Add your command handler code here
	Play=Play?FALSE:TRUE;
	if (Play)//设置定时器
		SetTimer(1,15,NULL);	
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

void CTestView::OnUpdateMENUPlay(CCmdUI* pCmdUI) //动画按钮状态函数 
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
		pCmdUI->SetText("开始");
	}
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC) 
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
	tran.SetMat(P,8);	
	InitParameter();
}

void CTestView::ClearImaMem()
{
	for(int n=0;n<bmp.bmHeight;n++)//注意撤销次序,先列后行,与设置相反
	{
		delete[] Image[n];
		Image[n]=NULL;
	}
	delete[] Image;
	Image=NULL;
}
