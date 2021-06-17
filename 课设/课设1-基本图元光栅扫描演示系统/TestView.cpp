// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "Line.h"//直线类
#include "TestDoc.h"
#include "TestView.h"
#include "Mainfrm.h"//主框架头文件
#define ROUND(a) int(a+0.5)//四舍五入
#include "math.h"//数学头文件
#define  PI 3.14159265//圆周率
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
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
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
	this->vpsize=CVPixel::VPSize;
	this->rect=CVPixel::rect;
	nType=0;
	LBdown=FALSE;
	IsAnti=FALSE;
	PStart=CP2(-1.0,-1.0,CRGB(0.0,0.0,0.0));
	PEnd=CP2(-1.0,-1.0,CRGB(0.0,0.0,0.0));
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
	CRect Rect;//定义客户区
	GetClientRect(&Rect);//获得客户区的大小
	CDC MemDC;//内存DC
	CBitmap NewBitmap,*pOldBitmap;//内存中承载图像的临时位图
	MemDC.CreateCompatibleDC(pDC);//建立与屏幕pDC兼容的MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC,Rect.Width(),Rect.Height());//创建兼容位图 
	pOldBitmap=MemDC.SelectObject(&NewBitmap); //将兼容位图选入MemDC
	MemDC.FillSolidRect(Rect,pDC->GetBkColor());//按原来背景填充客户区，否则是黑色
	DrawObject(&MemDC);
	pDC->BitBlt(0,0,Rect.Width(),Rect.Height(),&MemDC,0,0,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	MemDC.DeleteDC();//删除MemDC
	ReleaseDC(pDC);//释放DC
}

void CTestView::DrawGrid(CDC* pDC)//绘制网格
{
	VirtualPixel.DrawGrid(pDC);
}

void CTestView::DrawObject(CDC* pDC)//绘制图形函数
{
	CTestDoc* pDoc=GetDocument();
	nType=pDoc->m_nType;
	IsAnti=pDoc->m_Alias;
	PStart.c=pDoc->m_SPColor;
	PEnd.c=pDoc->m_EPColor;
	switch(nType)
	{
	case 0:
		DrawGrid(pDC);//绘制网格
		break;
	case 1:
		DrawLine(pDC);
		break;
	case 2:
		DrawEllipse(pDC);
		break;
	}	
}

void CTestView::DrawLine(CDC* pDC)//绘制直线
{
	if(IsAnti)//反走样直线
	{
		aline.DrawGrid(pDC);
		aline.MoveTo(pDC,PStart);
		aline.LineTo(pDC,PEnd);
	}
	else//走样直线
	{
		line.DrawGrid(pDC);
		line.MoveTo(pDC,PStart);
		line.LineTo(pDC,PEnd);
	}
}

void CTestView::DrawEllipse(CDC* pDC) //绘制椭圆
{
	if(IsAnti)//反走样椭圆
	{
		aellipse.DrawGrid(pDC);
		aellipse.DrawAEllipse(pDC,PStart,PEnd);
	}
	else//走样椭圆
	{
		ellipse.DrawGrid(pDC);
		ellipse.DrawEllipse(pDC,PStart,PEnd);
	}
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	LBdown=TRUE;	
	PStart.x=(point.x-rect.left)/vpsize;
	PStart.y=(point.y-rect.top)/vpsize;
	PEnd=PStart;	
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(LBdown==TRUE)
	{
		LBdown=FALSE;
	}
	CView::OnLButtonUp(nFlags, point);
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString strx,stry;///状态信息
    CMainFrame *pFrame=(CMainFrame*)AfxGetMainWnd();//获得窗口指针
	CStatusBar *pstatus=&pFrame->m_wndStatusBar;//获得状态栏的指针
	if( point.x >= rect.left && point.x < rect.right && point.y >=rect.top && point.y<rect.bottom )	
	{
		if(pstatus)
		{	
			strx.Format("x=%d",(point.x-rect.left)/vpsize);
			stry.Format("y=%d",(point.y-rect.top)/vpsize);
			CClientDC dc(this);
			CSize sizex=dc.GetTextExtent(strx);
			CSize sizey=dc.GetTextExtent(stry);
			pstatus->SetPaneInfo(1,ID_INDICATOR_X,SBPS_NORMAL,sizex.cx);//动态改变x坐标显示窗格的宽度
			pstatus->SetPaneText(1,strx);
			pstatus->SetPaneInfo(2,ID_INDICATOR_Y,SBPS_NORMAL,sizey.cx);//动态改变y坐标显示窗格的宽度
			pstatus->SetPaneText(2,stry);				
		}
		if(LBdown==TRUE && nType!=0)//鼠标左键按下并处于绘图状态
		{
			PEnd.x=(point.x-rect.left)/vpsize;//正常情况计算终止点
			PEnd.y=(point.y-rect.top)/vpsize;
			double dx=(PEnd.x-PStart.x);//偏移矢量
			double dy=(PEnd.y-PStart.y);
			if(nType==1)
			{
				if(GetKeyState(VK_SHIFT) & 0x8000)//绘制直线时，按下SHIFT键
				{
					if(fabs(dx)>=fabs(dy))
					{
						PEnd.y=PStart.y;//x方向的垂线
					}
					else
					{
						PEnd.x=PStart.x;//y方向的垂线
					}
				}
			}
			if(nType==2)
			{
				if(GetKeyState(VK_SHIFT) & 0x8000)//绘制椭圆时，是否按下SHIFT键
				{
					if(fabs(dx)<fabs(dy))//向dy方向为增长，|dx|为圆的直径
					{
						PEnd.y=PStart.y+sign(dy)*fabs(dx);//sign(dy)计算增长方向，fabs(dx)为长度
					}
					else
					{
						PEnd.x=PStart.x+sign(dx)*fabs(dy);
					}
				}
			}
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));//十字光标	
		}
	}
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));//加载系统光标	
	Invalidate(FALSE);//重绘，但不擦除背景
	CView::OnMouseMove(nFlags, point);	
}

int CTestView::sign(double n)//符号函数
{
	if(n<0)
		return -1;
	if(n>0)
		return 1;
	else 
		return 0;
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//禁止背景刷新 
{
	// TODO: Add your message handler code here and/or call default
	return  TRUE;
}

