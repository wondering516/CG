// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "Line.h"
#include "TestDoc.h"//包含CLine类头文件
#include "TestView.h"
#include "MainFrm.h"//包含CMainFrame头文件，用于状态栏显示鼠标坐标位置
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
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MDRAW, OnMdraw)
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
	m_Flag=FALSE;
	m_LBDown=FALSE;
	m_Attach=FALSE;
	m_MState=FALSE;
	m_IsInsert=FALSE; 
	m_Arrow=TRUE;
}

CTestView::~CTestView()
{	
	int nCount=m_ptrarray.GetSize();
	for (int i=0;i<nCount;i++)
	{
		CPointArray *p=(CPointArray*)m_ptrarray.GetAt(i);
		delete p;//释放指针所指向的堆内存
	}
	m_ptrarray.RemoveAll();//移除所有元素
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
void CTestView::DoubleBuffer()//双缓冲
{
	CRect rect;//定义客户区
	GetClientRect(&rect);//获得客户区的大小
	CDC* pDC=GetDC();
	CDC MemDC;//内存设备上下文
	CBitmap NewBitmap,*pOldBitmap;//内存中承载图像的临时位图
	MemDC.CreateCompatibleDC(pDC);//建立与屏幕pDC兼容的MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//创建兼容位图 
	pOldBitmap=MemDC.SelectObject(&NewBitmap); //将兼容位图选入MemDC 
	MemDC.FillSolidRect(rect,pDC->GetBkColor());//按原来背景填充客户区，否则是黑色 
	DrawObject(&MemDC);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	MemDC.DeleteDC();//删除MemDC
	ReleaseDC(pDC);//释放DC
}

void CTestView::DrawObject(CDC *pDC)//绘制多边形
{
	int index=m_ptrarray.GetSize();
	CLine *line=new CLine;
	if(index)
	{		
		line->MoveTo(pDC,((CPointArray*)m_ptrarray.GetAt(0))->pt);
		for(int i=1;i<index;i++)
		{
			line->LineTo(pDC,((CPointArray*)m_ptrarray.GetAt(i))->pt);		
		}
		if(FALSE==m_Flag)//线段闭合，填充图形
		{
			FillPolygon(pDC);	
		}
	}
	delete line;
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(TRUE==m_Flag)//绘图状态
	{
		m_LBDown=TRUE; 
		CPointArray *pPointArray=new CPointArray(point);
	    m_ptrarray.Add(pPointArray);//添加新顶点
	}
	m_IsInsert=FALSE;
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_Arrow)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	CString strx,stry;//状态栏显示鼠标位置
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;//要求包含MainFrm.h头文件
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;//需要将m_wndStatusBar属性修改为公有
	if(pStatus)
	{
		strx.Format("x=%d",point.x);
		stry.Format("y=%d",point.y);
		CDC *pDC=GetDC();
		CSize sizex=pDC->GetTextExtent(strx);
		CSize sizey=pDC->GetTextExtent(stry);
		pStatus->SetPaneInfo(1,ID_INDICATOR_X,SBPS_NORMAL,sizex.cx);//改变状态栏风格
		pStatus->SetPaneText(1,strx);
		pStatus->SetPaneInfo(2,ID_INDICATOR_Y,SBPS_NORMAL,sizey.cx);//改变状态栏风格
		pStatus->SetPaneText(2,stry);
		ReleaseDC(pDC);
	}
	int index=m_ptrarray.GetSize()-1;
	if(m_LBDown)
	{
		if(!m_IsInsert)//如果是第一次移动，则插入新的顶点
		{
			CPointArray *pPointArray=new CPointArray(point);
			m_ptrarray.Add(pPointArray);
			m_IsInsert=TRUE;
		}
		else//修改上次插入的顶点数据
		{  			
			((CPointArray *)m_ptrarray.GetAt(index))->pt=point;			
		}		
	}
	if(m_LBDown)
	{
		if(MK_SHIFT==nFlags)//约束:测试按下了Shift键
		{
			CPoint* pt1=&(((CPointArray *)m_ptrarray.GetAt(index))->pt);
			CPoint* pt2=&(((CPointArray *)m_ptrarray.GetAt(index-1))->pt);
			if(abs(pt1->x-pt2->x)>=abs(pt1->y-pt2->y))
			{
				pt1->y=pt2->y;//x方向的垂线
			}
			else
			{
				pt1->x=pt2->x;//y方向的垂线
			}
		}
	}
	if(index>3)
	{
		CPoint pt=((CPointArray*)m_ptrarray.GetAt(0))->pt;
		if((abs(point.x-pt.x)<=5) && (abs(point.y-pt.y)<=5))//引力域:边长为10的正方形
		{
			((CPointArray *)m_ptrarray.GetAt(index))->pt=pt;//修改数据
			m_Arrow=TRUE;
			m_LBDown=FALSE;
			m_MState=TRUE;
			m_Flag=FALSE;
		}
	}
	Invalidate(FALSE);
	CView::OnMouseMove(nFlags, point);
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//禁止背景刷新 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::OnMdraw() 
{
	// TODO: Add your command handler code here
	m_Arrow=FALSE;//光标判断
	m_Flag=TRUE;//绘图状态
	m_MState=FALSE;//Shift键按下
	for (int i = 0; i<m_ptrarray.GetSize(); i++)
	{
		CPointArray* p = (CPointArray*)m_ptrarray.GetAt(i);
		delete p;
	}
	m_ptrarray.RemoveAll();//清空数组
	if(FALSE==m_Attach)
	{
		MessageBox("请使用鼠标在屏幕上绘制多边形","提示", MB_ICONINFORMATION);
		m_Attach=TRUE;	
	}
	Invalidate(FALSE);	
}

void CTestView::FillPolygon(CDC *pDC) 
{
	// TODO: Add your command handler code here
    int size=m_ptrarray.GetSize();
	CPoint *p=new CPoint[size];//分配内存空间
	for(int i=0;i<size;i++)//拷贝数据到一个静态数组
	{
		p[i]=((CPointArray *)m_ptrarray.GetAt(i))->pt;
	}
	CFill *fill=new CFill;//动态分配内存
	fill->SetPoint(p,size);//设置多边形顶点数组
	fill->CreateBucket();//建立桶表
	fill->CreateEdge();//建立边表
	fill->FillPolygon(pDC);//填充多边形
	delete fill;//释放内存
	delete []p;	
}
