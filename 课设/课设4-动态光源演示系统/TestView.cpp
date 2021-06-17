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
	Play=FALSE;
	RedLight=1,GreenLight=1,BlueLight=1;
	ra=400,rb=300;ga=300,gb=200;ba=500,bb=400;Alpha=0;
	LightNum=3;//光源个数
	pLight=new CLighting(LightNum);//一维光源动态数组
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(1.0,1.0,1.0);//光源的漫反射颜色
		pLight->Light[i].L_Specular=CRGB(1.0,1.0,1.0);//光源颜色
		pLight->Light[i].L_C0=1.0;//常数衰减系数
		pLight->Light[i].L_C1=0.00000001;//线性衰减系数
		pLight->Light[i].L_C2=0.000000001;//二次衰减系数
		pLight->Light[i].L_OnOff=TRUE;//开启光源	
	}
	pMaterial=new CMaterial;//一维材质动态数组
	pMaterial->M_Ambient=CRGB(0.5,0.5,0.5);//材质对环境光的反射率
	pMaterial->M_Diffuse=CRGB(0.5,0.5,0.5);//材质对漫反射光的反射率
	pMaterial->M_Specular=CRGB(0.5,0.5,0.5);//材质对镜面反射光的反射率
	pMaterial->M_Emit=CRGB(0.0,0.0,0.0);//材质自身发散的颜色
	pMaterial->M_Exp=1.0;//高光指数
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
	SetTimer(1,50,NULL);
	InitModel();
	SetLight();
	DrawScene(&MemDC);
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	MemDC.DeleteDC();//删除MemDC
	ReleaseDC(pDC);//释放DC
}

void CTestView::DrawScene(CDC *pDC)//绘制球体
{
	CZBuffer *zbuf=new CZBuffer;//申请内存
	zbuf->InitDeepBuffer(1000,1000,-1000);//深度初始化
	Globe.DrawObject(pDC,pLight,pMaterial,zbuf,GLOBE);//绘制球体
	if(RedLight)
	{
		RedGTran=CP3(ra*cos(Alpha*PI/180),rb*sin(Alpha*PI/180),0);
		Tran_RedG.Translate(RedGTran.x,RedGTran.y,RedGTran.z);
		RedG.DrawObject(pDC,NULL,NULL,zbuf,REDG);//绘制红色光源
	}
	if(GreenLight)
	{
		GreenGTran=CP3(0,ga*cos(Alpha*PI/180),gb*sin(Alpha*PI/180));
		Tran_GreenG.Translate(GreenGTran.x,GreenGTran.y,GreenGTran.z);
		GreenG.DrawObject(pDC,NULL,NULL,zbuf,GREENG);//绘制绿色光源
	}
	if(BlueLight)
	{
		BlueGTran=CP3(bb*sin(Alpha*PI/180),0,ba*cos(Alpha*PI/180));
		Tran_BlueG.Translate(BlueGTran.x,BlueGTran.y,BlueGTran.z);
		BlueG.DrawObject(pDC,NULL,NULL,zbuf,BLUEG);//绘制蓝色光源
	}
	delete zbuf;
}

void CTestView::InitModel()
{
	Globe.SetSize(Globe_Radius);
	RedG.SetSize(RedG_Radius);
	Tran_RedG.SetMat(RedG.P,(RedG.N1-1)*RedG.N2+2);
	GreenG.SetSize(GreenG_Radius);
	Tran_GreenG.SetMat(GreenG.P,(GreenG.N1-1)*GreenG.N2+2);
	BlueG.SetSize(BlueG_Radius);
	Tran_BlueG.SetMat(BlueG.P,(BlueG.N1-1)*BlueG.N2+2);
}

void CTestView::SetLight() 
{
	// TODO: Add your command handler code here
	CRGB LightClr[3];
	LightClr[0]=CRGB(1.0,0.0,0.0);
	LightClr[1]=CRGB(0.0,1.0,0.0);
	LightClr[2]=CRGB(0.0,0.0,1.0);
	CP3 LightPos[3];
	LightPos[0]=CP3(ra*cos(Alpha*PI/180),rb*sin(Alpha*PI/180),0);
	LightPos[1]=CP3(0,ga*cos(Alpha*PI/180),gb*sin(Alpha*PI/180));
	LightPos[2]=CP3(bb*sin(Alpha*PI/180),0,ba*cos(Alpha*PI/180));
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].SetPosition(LightPos[i].x,LightPos[i].y,LightPos[i].z);//光源位置
		pLight->Light[i].L_Specular=LightClr[i];//光源颜色
	}
	if(RedLight)
		pLight->Light[0].L_OnOff=TRUE;
	else
		pLight->Light[0].L_OnOff=FALSE;	
	if(GreenLight)
		pLight->Light[1].L_OnOff=TRUE;
	else
		pLight->Light[1].L_OnOff=FALSE;	
	if(BlueLight)
		pLight->Light[2].L_OnOff=TRUE;
	else
		pLight->Light[2].L_OnOff=FALSE;	
}

void CTestView::OnTimer(UINT nIDEvent)//定时器函数 
{
	// TODO: Add your message handler code here and/or call default
	CTestDoc* pDoc=GetDocument();
	if(((CMainFrame*)AfxGetMainWnd())->IsPlay)
	{
		RedLight=pDoc->m_RedLight;
		GreenLight=pDoc->m_GreenLight;
		BlueLight=pDoc->m_BlueLight;
		Alpha+=5;
		Invalidate(FALSE);
	}
	CView::OnTimer(nIDEvent);
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//禁止背景刷新 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}


