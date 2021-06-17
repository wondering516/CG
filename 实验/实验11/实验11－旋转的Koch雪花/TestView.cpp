// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"

#include "TestDoc.h"
#include "TestView.h"
#include "math.h"//数学头文件
#define  PI 3.1415926//PI的宏定义
#define ROUND(x) int(x+0.5)//四舍五入
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
	ON_COMMAND(ID_MDRAW, OnMdraw)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_MDRAW, OnUpdateMdraw)
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
	RotateAngle=0;
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

void CTestView::DoubleBuffer()//双缓冲绘图
{
	CDC* pDC=GetDC();
	CRect rect;//定义客户区
	GetClientRect(&rect);//获得客户区的大小
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC自定义坐标系
	pDC->SetWindowExt(rect.Width(),rect.Height());//设置窗口范围
	pDC->SetViewportExt(rect.Width(),-rect.Height());//x轴水平向右，y轴垂直向上
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//屏幕中心为原点
	CBitmap NewBitmap,*pOldBitmap;//内存中承载图像的临时位图
	MemDC.CreateCompatibleDC(pDC);//建立与屏幕pDC兼容的MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//创建兼容位图 
	pOldBitmap=MemDC.SelectObject(&NewBitmap); //将兼容位图选入MemDC 
	//MemDC.FillSolidRect(rect,pDC->GetBkColor());//按原来背景填充客户区，否则是黑色 
	MemDC.SetMapMode(MM_ANISOTROPIC);//MemDC自定义坐标系
	MemDC.SetWindowExt(rect.Width(),rect.Height());
	MemDC.SetViewportExt(rect.Width(),-rect.Height());
	MemDC.SetViewportOrg(rect.Width()/2,rect.Height()/2);
	CPen PenBlue,*pOldPen;
	PenBlue.CreatePen(PS_SOLID,1,RGB(0,0,255));
	pOldPen=MemDC.SelectObject(&PenBlue);
	MemDC.MoveTo(-10,0);//绘制雪花中心
	MemDC.LineTo(10,0);
	MemDC.MoveTo(0,-10);
	MemDC.LineTo(0,10);
	MemDC.SelectObject(pOldPen);
	PenBlue.DeleteObject();
	CPen PenWhite;
	PenWhite.CreatePen(PS_SOLID,1,RGB(255,255,255));
	pOldPen=MemDC.SelectObject(&PenWhite);
	n=4;//递归深度
	theta=PI/3;//Koch角度
	r=200;//正三角形外接圆半径
	//正三角形顶点坐标
	x0=r*sin(RotateAngle*PI/180);
	y0=r*cos(RotateAngle*PI/180);
	x1=r*sin((RotateAngle+120)*PI/180);
	y1=r*cos((RotateAngle+120)*PI/180);
	x2=r*sin((RotateAngle+240)*PI/180);
	y2=r*cos((RotateAngle+240)*PI/180);
	//绘制三段Koch曲线
	Fractal(x0,y0,x1,y1);//绘制右边
	Fractal(x1,y1,x2,y2);//绘制底边
	Fractal(x2,y2,x0,y0);//绘制左边
	MemDC.SelectObject(pOldPen);
	PenWhite.DeleteObject();
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	MemDC.DeleteDC();//删除MemDC
	ReleaseDC(pDC);//释放DC
}

void CTestView::Fractal(double bx,double by,double ex,double ey)//绘制一段Koch曲线
{
    L=sqrt((ex-bx)*(ex-bx)+(ey-by)*(ey-by));//线段长度
	d=L/pow(2*(1+cos(theta)),n);//曲线中每一段长度
	angle=atan((ey-by)/(ex-bx));//曲线起始角度
	if(ex<bx)
		angle=angle+PI;//处理底边反向绘制
	ax=bx,ay=by;
    Koch(angle,n);
}
void CTestView::Koch(double alpha,int n)//科和函数
{
	if(n==0)
    {
		bx=ax+d*cos(alpha);
		by=ay+d*sin(alpha);
		MemDC.MoveTo(ROUND(ax),ROUND(ay));
		MemDC.LineTo(ROUND(bx),ROUND(by));
		ax=bx;ay=by;
		return;
	}
	Koch(alpha,n-1);
    alpha+=theta;
    Koch(alpha,n-1);
    alpha-=2*theta;
    Koch(alpha,n-1);
    alpha+=theta;
    Koch(alpha,n-1);
}

void CTestView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	RotateAngle+=10;//设定步长
	Invalidate(FALSE);	
	CView::OnTimer(nIDEvent);
}

void CTestView::OnMdraw() 
{
	// TODO: Add your command handler code here
	Play=Play?FALSE:TRUE;
	if (Play)//设置定时器
		SetTimer(1,50,NULL);	
	else
		KillTimer(1);	
}

void CTestView::OnUpdateMdraw(CCmdUI* pCmdUI) 
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

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//禁止背景擦除 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}
