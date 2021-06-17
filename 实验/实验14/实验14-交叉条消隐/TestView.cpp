// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#define ROUND(a) int(a+0.5)
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
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MZBUFFER, OnMzbuffer)
	ON_UPDATE_COMMAND_UI(ID_MZBUFFER, OnUpdateMzbuffer)
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

void CTestView::ReadPointDeep()//深度缓冲点表
{
	a=60;d=100;
	int i;	
	//上条顶点的三维坐标(x,y,z)，颜色为红色
	P[0][0].x=-3*a;P[0][0].y=+2*a;P[0][0].z=+d;
	P[0][1].x=-3*a;P[0][1].y=  +a;P[0][1].z=+d;
	P[0][2].x=+3*a;P[0][2].y=  +a;P[0][2].z=-d;
	P[0][3].x=+3*a;P[0][3].y=+2*a;P[0][3].z=-d;
	for(i=0;i<4;i++)//顶点颜色赋值
	{
		P[0][i].c=CRGB(1.0,0.0,0.0);//顶点颜色赋值
	}
	//下条顶点的三维坐标(x,y,z),颜色为黄色
	P[1][0].x=-3*a;P[1][0].y=  -a;P[1][0].z=-d;
	P[1][1].x=-3*a;P[1][1].y=-2*a;P[1][1].z=-d;
	P[1][2].x=+3*a;P[1][2].y=-2*a;P[1][2].z=+d;
	P[1][3].x=+3*a;P[1][3].y=  -a;P[1][3].z=+d;
	for(i=0;i<4;i++)
	{
		P[1][i].c=CRGB(1.0,1.0,0.0);//顶点颜色赋值
	}
	//左条顶点的三维坐标(x,y,z),颜色为绿色
	P[2][0].x=-2*a;P[2][0].y=+3*a;P[2][0].z=-d;
	P[2][1].x=-2*a;P[2][1].y=-3*a;P[2][1].z=+d;
	P[2][2].x=  -a;P[2][2].y=-3*a;P[2][2].z=+d;
	P[2][3].x=  -a;P[2][3].y=+3*a;P[2][3].z=-d;
	for(i=0;i<4;i++)
	{
		P[2][i].c=CRGB(0.0,1.0,0.0);//顶点颜色赋值
	}	
	//右条顶点的三维坐标(x,y,z)，颜色为蓝色
	P[3][0].x=  +a;P[3][0].y=+3*a;P[3][0].z=+d;
	P[3][1].x=  +a;P[3][1].y=-3*a;P[3][1].z=-d;
	P[3][2].x=+2*a;P[3][2].y=-3*a;P[3][2].z=-d;
	P[3][3].x=+2*a;P[3][3].y=+3*a;P[3][3].z=+d;
	for(i=0;i<4;i++)
	{
		P[3][i].c=CRGB(0.0,0.0,1.0);//顶点颜色赋值
	}
}

void CTestView::ReadPointSort()//深度排序点表
{
	a=60;d=100;
	int i;	
	//上条顶点的三维坐标(x,y,z)，颜色为红色
	P[0][0].x=-3*a;P[0][0].y=+2*a;P[0][0].z=-d;
	P[0][1].x=-3*a;P[0][1].y=  +a;P[0][1].z=-d;
	P[0][2].x=+3*a;P[0][2].y=  +a;P[0][2].z=-d;
	P[0][3].x=+3*a;P[0][3].y=+2*a;P[0][3].z=-d;
	for(i=0;i<4;i++)//顶点颜色赋值
	{
		P[0][i].c=CRGB(1.0,0.0,0.0);//顶点颜色赋值
	}
	//下条顶点的三维坐标(x,y,z),颜色为黄色
	P[1][0].x=-3*a;P[1][0].y=  -a;P[1][0].z=+d;
	P[1][1].x=-3*a;P[1][1].y=-2*a;P[1][1].z=+d;
	P[1][2].x=+3*a;P[1][2].y=-2*a;P[1][2].z=+d;
	P[1][3].x=+3*a;P[1][3].y=  -a;P[1][3].z=+d;
	for(i=0;i<4;i++)
	{
		P[1][i].c=CRGB(1.0,1.0,0.0);//顶点颜色赋值
	}
	//左条顶点的三维坐标(x,y,z),颜色为绿色
	P[2][0].x=-2*a;P[2][0].y=+3*a;P[2][0].z=-2*d;
	P[2][1].x=-2*a;P[2][1].y=-3*a;P[2][1].z=-2*d;
	P[2][2].x=  -a;P[2][2].y=-3*a;P[2][2].z=-2*d;
	P[2][3].x=  -a;P[2][3].y=+3*a;P[2][3].z=-2*d;
	for(i=0;i<4;i++)
	{
		P[2][i].c=CRGB(0.0,1.0,0.0);//顶点颜色赋值
	}	
	//右条顶点的三维坐标(x,y,z)，颜色为蓝色
	P[3][0].x=  +a;P[3][0].y=+3*a;P[3][0].z=+2*d;
	P[3][1].x=  +a;P[3][1].y=-3*a;P[3][1].z=+2*d;
	P[3][2].x=+2*a;P[3][2].y=-3*a;P[3][2].z=+2*d;
	P[3][3].x=+2*a;P[3][3].y=+3*a;P[3][3].z=+2*d;
	for(i=0;i<4;i++)
	{
		P[3][i].c=CRGB(0.0,0.0,1.0);//顶点颜色赋值
	}
}

void CTestView::DoubleBuffer()//设置坐标系
{	
	CRect rect;
	GetClientRect(&rect);
	CDC *pDC=GetDC();
	pDC->SetMapMode(MM_ANISOTROPIC);//自定义坐标系
	pDC->SetWindowExt(rect.Width(),rect.Height());
	pDC->SetViewportExt(rect.Width(),-rect.Height());//x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//屏幕中心为原点
	if(Play)
	{
		ReadPointSort();
		DrawObject1(pDC);//禁用深度缓冲
	}
	else
	{
		ReadPointDeep();
		DrawObject2(pDC);//启用深度缓冲	
	}
}

void CTestView::DrawObject1(CDC *pDC)//禁用深度缓冲绘制交叉条
{
	CPi2 Point[4];
	SortDeep();
	for(int nBar=0;nBar<4;nBar++)
	{	
		for(int nPoint=0;nPoint<4;nPoint++)//顶点循环
		{
			Point[nPoint].x=P[nBar][nPoint].x;
			Point[nPoint].y=ROUND(P[nBar][nPoint].y);
			Point[nPoint].c=P[nBar][nPoint].c;
		}
		CFill *fill=new CFill;//申请内存
		fill->SetPoint(Point,4);//设置顶点		
		fill->CreateBucket();//建立桶表
		fill->CreateEdge();//建立边表
		fill->Gouraud(pDC);//填充四边形
		delete fill;//释放内存
	}
	
}

void CTestView::DrawObject2(CDC *pDC)//启用深度缓冲绘制交叉条
{
	CPi3 Point[4];
	CZBuffer *zbuf=new CZBuffer;//申请内存
	zbuf->InitDeepBuffer(800,800,-1000);//深度初始化
	for(int nBar=0;nBar<4;nBar++)
	{	
		for(int nPoint=0;nPoint<4;nPoint++)//顶点循环
		{
			Point[nPoint].x=P[nBar][nPoint].x;
			Point[nPoint].y=ROUND(P[nBar][nPoint].y);
			Point[nPoint].z=P[nBar][nPoint].z;
			Point[nPoint].c=P[nBar][nPoint].c;
		}
		zbuf->SetPoint(Point,4);//设置顶点			
		zbuf->CreateBucket();//建立桶表
		zbuf->CreateEdge();//建立边表
		zbuf->Gouraud(pDC);//填充四边形	
		zbuf->ClearMemory();//内存清理
	}
	delete zbuf;//释放内存
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//设置背景色为黑色 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;   
	pDC->GetClipBox(&rect);   
	pDC->FillSolidRect(rect,RGB(0,0,0));
	return TRUE;
}

void CTestView::OnMzbuffer()//按钮控制函数 
{
	// TODO: Add your command handler code here
	Play=Play?FALSE:TRUE;
	if(Play)
		ReadPointSort();
	else
		ReadPointDeep();
	Invalidate(FALSE);	
}

void CTestView::OnUpdateMzbuffer(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(Play)
	{
		pCmdUI->SetCheck(TRUE);
		pCmdUI->SetText("启用深度缓冲");
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->SetText("禁用深度缓冲");
	}		
}
void CTestView::SortDeep()//深度排序
{
	CP3 t;
	for(int j=1;j<4;j++)//冒泡算法
		for(int i=1;i<=4-j;i++)
		{
			for(int k=0;k<4;k++)
			{
				if(P[i-1][k].z>P[i][k].z)
				{
					t=P[i-1][k];P[i-1][k]=P[i][k];P[i][k]=t;					
				}
			}
		}		
}