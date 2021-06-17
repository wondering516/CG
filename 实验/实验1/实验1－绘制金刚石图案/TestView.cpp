// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#include "math.h"//数学头文件
#define PI 3.1415926//PI的宏定义
#define ROUND(a) int(a+0.5)//四舍五入
#include "InputDlg.h"//包含对话框头文件
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
	ON_COMMAND(IDM_DIAMOND, OnDiamond)
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
	p=NULL;
}

CTestView::~CTestView()
{
	if(p!=NULL)
	{
		delete[] p;//释放一维数组数组内存空间
		p=NULL;
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
void CTestView::Diamond()//金刚石函数
{	
    CDC *pDC=GetDC();//获得设备上下文
	CRect rect;//定义矩形
	GetClientRect(&rect);//获得矩形客户区大小
	pDC->SetMapMode(MM_ANISOTROPIC);//自定义坐标系
	pDC->SetWindowExt(rect.Width(),rect.Height());//设置窗口范围
	pDC->SetViewportExt(rect.Width(),-rect.Height());//x轴水平向右，y轴铅直向上
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//屏幕中心为坐标系原点
	CPen NewPen,*pOldPen;//定义画笔
	NewPen.CreatePen(PS_SOLID,1,RGB(0,0,255));//创建蓝色画笔
	pOldPen=pDC->SelectObject(&NewPen);//将蓝色画笔选入设备上下文
	double thta;//thta为圆的等分角
	thta=2*PI/n;
	for(int i=0;i<n;i++)//计算等分点坐标
	{
		p[i].x=r*cos(i*thta);
		p[i].y=r*sin(i*thta);
	}
	for(i=0;i<=n-2;i++)//依次各连接等分点
	{
		for(int j=i+1;j<=n-1;j++)
		{
			pDC->MoveTo(ROUND(p[i].x),ROUND(p[i].y));
			pDC->LineTo(ROUND(p[j].x),ROUND(p[j].y));
		}
	}
	pDC->SelectObject(pOldPen);//恢复设备上下文原画笔
	NewPen.DeleteObject();//删除已成自由状态的新画笔 
	ReleaseDC(pDC);//释放pDC
}

void CTestView::OnDiamond() 
{
	// TODO: Add your command handler code here
	CInputDlg dlg;
	if(IDOK==dlg.DoModal())//调用对话框模块，判断是否单击OK按钮
	{
		n=dlg.m_n;//n为等分点个数
		r=dlg.m_r;//r为圆的半径
	}
	else
	{
		return;
	}
	RedrawWindow();//重绘窗口
	p=new CP2[n];//创建动态一维数组内存空间,n必须是整型变量 
	Diamond();	
}
