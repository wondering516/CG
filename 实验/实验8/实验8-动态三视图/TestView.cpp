// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "MainFrm.h"
#include "VView.h"//
#include "HView.h"//
#include "WView.h"//
#include "TestDoc.h"
#include "TestView.h"
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
	ON_WM_KEYDOWN()
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
	Afa=0;
	Beta=0;	
	afa=45;beta=45;    
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
	POld[0].x=-80;POld[0].y=-40;POld[0].z=-40;//A点
	POld[1].x=-80;POld[1].y=-40;POld[1].z= 40;//B点
	POld[2].x= 80;POld[2].y=-40;POld[2].z= 40;//C点
	POld[3].x= 80;POld[3].y=-40;POld[3].z=-40;//D点
	POld[4].x=-80;POld[4].y= 40;POld[4].z= 40;//E点
	POld[5].x= 80;POld[5].y= 40;POld[5].z= 40;//F点
	POld[6].x= 80;POld[6].y= 40;POld[6].z=-40;//G点
	POld[7].x=-80;POld[7].y= 40;POld[7].z=-40;//H点
	POld[8].x=-80;POld[8].y= 80;POld[8].z=  0;//I点
	POld[9].x= 80;POld[9].y= 80;POld[9].z=  0;//J点
}

void CTestView::ReadFace()//面表
{//面的边数、面的顶点编号
	F[0].SetEN(4);F[0].p[0]=1;F[0].p[1]=2;F[0].p[2]=5;F[0].p[3]=4;            //前面BCFE
	F[1].SetEN(4);F[1].p[0]=0;F[1].p[1]=7;F[1].p[2]=6;F[1].p[3]=3;            //后面AHGD
	F[2].SetEN(5);F[2].p[0]=0;F[2].p[1]=1;F[2].p[2]=4;F[2].p[3]=8;F[2].p[4]=7;//左面ABEIH
	F[3].SetEN(5);F[3].p[0]=3;F[3].p[1]=6;F[3].p[2]=9;F[3].p[3]=5;F[3].p[4]=2;//右面DGJFC
	F[4].SetEN(4);F[4].p[0]=4;F[4].p[1]=5;F[4].p[2]=9;F[4].p[3]=8;            //前顶面EFJI
	F[5].SetEN(4);F[5].p[0]=8;F[5].p[1]=9;F[5].p[2]=6;F[5].p[3]=7;            //后顶面IJGH
	F[6].SetEN(4);F[6].p[0]=0;F[6].p[1]=3;F[6].p[2]=2;F[6].p[3]=1;            //底面ADCB
}
void CTestView::Rotate()//绕x轴旋转α角,绕y轴旋转β角
{
	
	for(int nVertex=0;nVertex<10;nVertex++)//多面体有10个顶点
	{
		CP3	PTemp;
		PTemp.x=POld[nVertex].x;
		PTemp.y=POld[nVertex].y*cos(PI*Afa/180)-POld[nVertex].z*sin(PI*Afa/180);
		PTemp.z=POld[nVertex].y*sin(PI*Afa/180)+POld[nVertex].z*cos(PI*Afa/180);
		PNew[nVertex].x=PTemp.z*sin(PI*Beta/180)+PTemp.x*cos(PI*Beta/180);
		PNew[nVertex].y=PTemp.y;
		PNew[nVertex].z=PTemp.z*cos(PI*Beta/180)-PTemp.x*sin(PI*Beta/180);	
	}
}
void CTestView::DoubleBuffer()//双缓冲
{
	CDC* pDC=GetDC();
	CRect rect;//定义客户区
	GetClientRect(&rect);//获得客户区的大小
	CDC MemDC[4];
	CDC* pViewDC[4];
	CMainFrame *pFrame=(CMainFrame*)AfxGetMainWnd();//获得框架指针
	CVView *pVView=(CVView*)pFrame->m_wndSplitter.GetPane(0,0);//获得主视图窗格指针
	CWView *pWView=(CWView*)pFrame->m_wndSplitter.GetPane(0,1);//获得左视图窗格指针
	CHView *pHView=(CHView*)pFrame->m_wndSplitter.GetPane(1,0);//获得俯视图窗格指针
	pViewDC[0]=pDC;//多面体屏幕DC
	pViewDC[1]=pVView->GetDC();//主视图屏幕DC
	pViewDC[2]=pWView->GetDC();//左视屏幕DC
	pViewDC[3]=pHView->GetDC();//俯视图屏幕DC
	CBitmap NewBitmap[4],*pOldBitmap[4];//内存中承载图像的临时位图
    for(int i=0;i<4;i++)//创建内存DC
	{	
		pViewDC[i]->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
		pViewDC[i]->SetWindowExt(rect.Width(),rect.Height());//设置窗口范围
		pViewDC[i]->SetViewportExt(rect.Width(),-rect.Height());//x轴水平向右，y轴铅直向上
		pViewDC[i]->SetViewportOrg(rect.Width()/2,rect.Height()/2);//屏幕中心为原点
		NewBitmap[i].CreateCompatibleBitmap(pViewDC[i],rect.Width(),rect.Height());//创建兼容位图
		MemDC[i].CreateCompatibleDC(pViewDC[i]);
		pOldBitmap[i]=MemDC[i].SelectObject(&NewBitmap[i]);
		MemDC[i].FillSolidRect(rect,pViewDC[i]->GetBkColor());//按原来背景填充客户区，否则是黑色 
		MemDC[i].SetMapMode(MM_ANISOTROPIC);//MemDC自定义坐标系
		MemDC[i].SetWindowExt(rect.Width(),rect.Height());
		MemDC[i].SetViewportExt(rect.Width(),-rect.Height());
		MemDC[i].SetViewportOrg(rect.Width()/2,rect.Height()/2);
	}
	DrawObject(&MemDC[0]);//绘制立体图
	DrawVView(&MemDC[1]); //绘制主视图
	DrawWView(&MemDC[2]); //绘制左视图
	DrawHView(&MemDC[3]); //绘制俯视图
	for(i=0;i<4;i++)
	{
		pViewDC[i]->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC[i],-rect.Width()/2,-rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
		MemDC[i].SelectObject(pOldBitmap[i]);//恢复位图
		NewBitmap[i].DeleteObject();//删除位图
		ReleaseDC(pViewDC[i]);//释放视图DC
	}
	ReleaseDC(pDC);//释放DC
}
void CTestView::Project(CP3 p)//斜等测变换
{
	ScreenP.x=p.x-p.z/sqrt(2);
	ScreenP.y=p.y-p.z/sqrt(2);	
}

void CTestView::DrawObject(CDC *pDC)//绘制多面体表面
{
	pDC->TextOut(-230,130,"立体图");
	for(int nFace=0;nFace<7;nFace++)
	{
		CP3 ViewPoint(1/2.0,1/2.0,sqrt(2)/2.0);//根据投影方向计算视点
		CVector VS(ViewPoint);//面的视矢量
		CVector V01(PNew[F[nFace].p[0]],PNew[F[nFace].p[1]]);//面的一条边矢量
		CVector V12(PNew[F[nFace].p[1]],PNew[F[nFace].p[2]]);//面的另一条边矢量
		CVector VN=V01*V12;//面的法矢量
		if(Dot(VS,VN)>=0)//背面剔除
		{
			CP2 t;	
			CLine *line=new CLine;
			for(int nEdge=0;nEdge<F[nFace].En;nEdge++)//边循环
			{
				Project(PNew[F[nFace].p[nEdge]]);//斜等测投影
				if(nEdge==0)
				{
					line->MoveTo(pDC,ScreenP.x,ScreenP.y);
					t=ScreenP;
				}
				else
				{
					line->LineTo(pDC,ScreenP.x,ScreenP.y);
				}		
			}
			line->LineTo(pDC,t.x,t.y);//闭合多边形
			delete line;
		}
	}
}

void CTestView::DrawTriView(CDC *pDC,CP3 P[])//绘制三视图
{
	for(int nFace=0;nFace<7;nFace++)
	{
		CP3 ScreenP,t;	
		CLine *line=new CLine;
		for(int nEdge=0;nEdge<F[nFace].En;nEdge++)//边循环
		{
			ScreenP=P[F[nFace].p[nEdge]];
			if(nEdge==0)
			{
				line->MoveTo(pDC,ScreenP.x,ScreenP.y);
				t=ScreenP;
			}
			else
			{
				line->LineTo(pDC,ScreenP.x,ScreenP.y);
			}		
		}
		line->LineTo(pDC,t.x,t.y);//闭合多边形
		delete line;
	}
}

void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nChar)
	{
	case VK_UP:
		Afa+=1;//设定步长
		break;
	case VK_DOWN:
		Afa-=1;
		break;
	case VK_LEFT:
		Beta-=1;
		break;
	case VK_RIGHT:
		Beta+=1;
		break;
	default:
		break;			
	}
	Rotate();
	Invalidate(FALSE);		
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTestView::TVMatrix()//主视图变换矩阵
{
	TV[0][0]=1;TV[0][1]=0;TV[0][2]=0;TV[0][3]=0;
	TV[1][0]=0;TV[1][1]=1;TV[1][2]=0;TV[1][3]=0;
	TV[2][0]=0;TV[2][1]=0;TV[2][2]=0;TV[2][3]=0;
	TV[3][0]=0;TV[3][1]=0;TV[3][2]=0;TV[3][3]=1;
}

void CTestView::THMatrix()//俯视图变换矩阵
{
	TH[0][0]=1;TH[0][1]=0; TH[0][2]=0;TH[0][3]=0;
	TH[1][0]=0;TH[1][1]=0; TH[1][2]=0;TH[1][3]=0;
	TH[2][0]=0;TH[2][1]=-1;TH[2][2]=0;TH[2][3]=0;
	TH[3][0]=0;TH[3][1]=0; TH[3][2]=0;TH[3][3]=1;
}

void CTestView::TWMatrix()//侧视图变换矩阵
{
	TW[0][0]=0;TW[0][1]=0;TW[0][2]=0;TW[0][3]=0;
	TW[1][0]=0;TW[1][1]=1;TW[1][2]=0;TW[1][3]=0;
	TW[2][0]=1;TW[2][1]=0;TW[2][2]=0;TW[2][3]=0;
	TW[3][0]=0;TW[3][1]=0;TW[3][2]=0;TW[3][3]=1;
}

void CTestView::DrawVView(CDC *pDC)
{
	pDC->TextOut(-230,130,"主视图");
	MultiMatrix(TV);
	DrawTriView(pDC,PTri);
}

void CTestView::DrawWView(CDC *pDC)
{
	pDC->TextOut(-230,130,"侧视图");
	MultiMatrix(TW);
	DrawTriView(pDC,PTri);
}

void CTestView::DrawHView(CDC *pDC)
{
	pDC->TextOut(-230,130,"俯视图");
	MultiMatrix(TH);
	DrawTriView(pDC,PTri);
}
void CTestView::MultiMatrix(double T[][4])//两个矩阵相乘
{
	for(int i=0;i<10;i++)
	{
		PTri[i].x=PNew[i].x*T[0][0]+PNew[i].y*T[1][0]+PNew[i].z*T[2][0]+PNew[i].w*T[3][0];
		PTri[i].y=PNew[i].x*T[0][1]+PNew[i].y*T[1][1]+PNew[i].z*T[2][1]+PNew[i].w*T[3][1];
		PTri[i].z=PNew[i].x*T[0][2]+PNew[i].y*T[1][2]+PNew[i].z*T[2][2]+PNew[i].w*T[3][2];
		PTri[i].w=PNew[i].x*T[0][3]+PNew[i].y*T[1][3]+PNew[i].z*T[2][3]+PNew[i].w*T[3][3];
	}		
}

void CTestView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(((CMainFrame*)AfxGetMainWnd())->IsPlay)
	{
		Afa+=1;
		Beta+=1;
		Rotate();
		Invalidate(FALSE);;	
	}
	CView::OnTimer(nIDEvent);
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
	TVMatrix();
	THMatrix();
	TWMatrix();
	ReadPoint();
	ReadFace();
	Rotate();
	SetTimer(1,50,NULL);//设置定时器
}
