// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "math.h"//调用数学头文件
#include "MainFrm.h"//包含CMainFrame头文件，用于状态栏显示鼠标坐标位置
#include "TestDoc.h"
#include "TestView.h"
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
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_MBEZIER, OnMbezier)
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
	m_flag=FALSE;
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
void CTestView::OnMbezier() 
{
	// TODO: Add your command handler code here
	RedrawWindow();
	MessageBox("左键绘制控制多边形，右键绘制曲线","提示",MB_OK);
	pt=new CPoint [N_MAX_POINT];
	m_flag=TRUE;//可以使用鼠标绘制控制点
	CtrlPNum=0;//控制点初始个数为0
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point)//获得屏幕控制点坐标 
{
	// TODO: Add your message handler code here and/or call default	
	if(m_flag)//m_flag为鼠标绘图标志
	{
		pt[CtrlPNum]=point;
		if(CtrlPNum<N_MAX_POINT)
		{
			CtrlPNum++;
		}
		else
		{
			m_flag=FALSE;
		}
		DrawCtrPolygon();
	}	
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::DrawBezier()//绘制Bezier曲线	
{
	CDC *pDC=GetDC();
	double delt=1.0/50000;//步长
	int n=CtrlPNum-1;
	CPoint p;
	long px[N_MAX_POINT],py[N_MAX_POINT];
	for(int k=0;k<=n;k++)
	{
		px[k]=pt[k].x;
		py[k]=pt[k].y;
	}
    for(double t=0;t<=1;t+=delt)
	{
		p.x=DeCasteliau(t,px);//计算x方向
		p.y=DeCasteliau(t,py);//计算y方向
        pDC->SetPixel(p.x,p.y,RGB(0,0,255));
	}
	ReleaseDC(pDC);
}

long CTestView::DeCasteliau(double t,long *p)//德卡斯特里奥函数
{
	double P[N_MAX_POINT][N_MAX_POINT];
	int n=CtrlPNum-1;
	for(int k=0;k<=n;k++)
	{
		P[0][k]=p[k];
	}
	for(int r=1;r<=n;r++)
	{
		for(int i=0;i<=n-r;i++)
		{ 
			P[r][i]=(1-t)*P[r-1][i]+t*P[r-1][i+1];
		} 
    }
    return(long(P[n][0]));
}

void CTestView::DrawCtrPolygon()//绘制控制多边形
{
	CDC *pDC=GetDC();	
	CPen NewPen,*pOldPen;
	NewPen.CreatePen(PS_SOLID,3,RGB(0,0,0));
	pOldPen=pDC->SelectObject(&NewPen);
	CBrush NewBrush,*pOldBrush;
	NewBrush.CreateSolidBrush(RGB(0,0,0));
	pOldBrush=pDC->SelectObject(&NewBrush);
	for(int i=0;i<CtrlPNum;i++)
	{
		if(0==i)
		{
			pDC->MoveTo(pt[i].x,pt[i].y);
			pDC->Rectangle(pt[i].x-2,pt[i].y-2,pt[i].x+2,pt[i].y+2);
		}
		else
		{
			pDC->LineTo(pt[i].x,pt[i].y);
			pDC->Rectangle(pt[i].x-2,pt[i].y-2,pt[i].x+2,pt[i].y+2);
		}
	}
	pDC->SelectObject(pOldBrush);
	NewBrush.DeleteObject();
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
	ReleaseDC(pDC);
}

void CTestView::OnRButtonDown(UINT nFlags, CPoint point)//调用Bezier函数 
{
	// TODO: Add your message handler code here and/or call default
	m_flag=FALSE;//结束鼠标绘图
	DrawBezier();//绘制Bezier曲线
	CView::OnRButtonDown(nFlags, point);
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point)//显示鼠标位置函数 
{
	// TODO: Add your message handler code here and/or call default
	CString strx,stry;//状态栏显示鼠标位置
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;//要求包含MainFrm.h头文件
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;//需要将m_wndStatusBar属性修改为公有
	if(pStatus)
	{
		strx.Format("x=%d",point.x);
		stry.Format("y=%d",point.y);
		CClientDC dc(this);
		CSize sizex=dc.GetTextExtent(strx);
		CSize sizey=dc.GetTextExtent(stry);
		pStatus->SetPaneInfo(1,ID_INDICATOR_X,SBPS_NORMAL,sizex.cx);//改变状态栏风格
		pStatus->SetPaneText(1,strx);
		pStatus->SetPaneInfo(2,ID_INDICATOR_Y,SBPS_NORMAL,sizey.cx);//改变状态栏风格
		pStatus->SetPaneText(2,stry);	
	}
	CView::OnMouseMove(nFlags, point);
}

