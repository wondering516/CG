// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "Mainfrm.h"
#include "TestDoc.h"
#include "TestView.h"
#include "LeftPortion.h"
#include "math.h"
#define  PI 3.1415926
#define  ROUND(x)  int(x+0.5)
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
	P=NULL;PV=NULL;F=NULL;Image=NULL;
	R1=200;R2=80;NumR1=18;NumR2=18;Switch=1;Model=0;Render=0;
	R=1000,d=800,Phi=90.0,Theta=0.0;//视点始终位于正前方	
	afa=0.0;beta=0.0;//圆环起始位置角
	LightNum=1;//光源个数
	pLight=new CLighting(LightNum);//一维光源动态数组
    pLight->Light[0].SetPosition(0,0,1000);//设置光源位置坐标
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(1.0,1.0,1.0);//光源的漫反射颜色	
		pLight->Light[i].L_Specular=CRGB(0.828,0.636,0.366);//光源镜面高光颜色
		pLight->Light[i].L_C0=1.0;//常数衰减系数
		pLight->Light[i].L_C1=0.0000001;//线性衰减系数
		pLight->Light[i].L_C2=0.00000001;//二次衰减系数
		if(Switch)
			pLight->Light[i].L_OnOff=TRUE;//关闭光源	
		else
			pLight->Light[i].L_OnOff=FALSE;//开启光源
	}
	pMaterial=new CMaterial;//一维材质动态数组
	pMaterial->M_Ambient=CRGB(0.547,0.400,0.075);//材质对环境光的反射率
	pMaterial->M_Diffuse=CRGB(0.912,0.616,0.116);//材质对漫反射光的反射率
	pMaterial->M_Specular=CRGB(1.0,1.0,1.0);//材质对镜面反射光的反射率
	pMaterial->M_Exp=100.0;//高光指数
	PathName = ".\\Data\\texture1.bmp";//初始化纹理位图路径
}

CTestView::~CTestView()
{
    //清理内存
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
	if(P!=NULL)
	{
		delete[]P;
		P=NULL;
	}
	if (PV!=NULL)
	{
		delete[]PV;
		PV=NULL;
	}
	if (F!=NULL)
	{
		
		for(int n=0;n<NumR1;n++)//注意撤销次序,先列后行,与设置相反
		{
			delete[] F[n];
			F[n]=NULL;
		}
		delete[] F;
		F=NULL;
	}
	if (Image!=NULL)
	{
		delete []Image;
		Image=NULL;
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
void CTestView::ReadPoint()//读入点坐标
{
	RingAfa=360/NumR1,RingBeta=360/NumR2;//面片数量为NumR1×NumR2
	P=new CP3 [NumR1*NumR2];//顶点动态数组
	PV=new CVector[NumR1*NumR2];//顶点法矢量动态数组
	for(int i=0;i<NumR1;i++)
	{
		double afa1=RingAfa*i*PI/180;
		for(int j=0;j<NumR2;j++)//顶点赋值
		{
			double beta1=RingBeta*j*PI/180;
			P[i*NumR2+j].x=R1*sin(afa1)+R2*sin(beta1)*sin(afa1);
			P[i*NumR2+j].y=R2*cos(beta1);
			P[i*NumR2+j].z=R1*cos(afa1)+R2*sin(beta1)*cos(afa1);
		}
	}
	tran.SetMat(P,NumR1*NumR2);//变换矩阵初始化
}

void CTestView::ReadFace()//读入面表
{
	//面的二维动态数组
	F=new CFace *[NumR1];
	for(int n=0;n<NumR1;n++)
	{
		F[n]=new CFace[NumR2];
	}
	for(int i=0;i<NumR1;i++)
	{
		for(int j=0;j<NumR2;j++)
		{
			int tempi=i+1;
			int tempj=j+1;
			if(tempj==NumR2) tempj=0;
			if(tempi==NumR1) tempi=0;
			F[i][j].SetEN(4);//面的边数
			F[i][j].p[0]=i*NumR2+j;//建立面的边号
			F[i][j].p[1]=i*NumR2+tempj;
			F[i][j].p[2]=tempi*NumR2+tempj; 
			F[i][j].p[3]=tempi*NumR2+j;
		}
	}
}

void CTestView::InitParameter()//透视变换参数
{
	k[1]=sin(PI*Theta/180);
	k[2]=sin(PI*Phi/180);
	k[3]=cos(PI*Theta/180);
	k[4]=cos(PI*Phi/180);
	k[5]=k[2]*k[3];
	k[6]=k[2]*k[1];
	k[7]=k[4]*k[3];
	k[8]=k[4]*k[1];
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
	pOldBitmap=MemDC.SelectObject(&NewBitmap);//将兼容位图选入MemDC 
	MemDC.FillSolidRect(rect,RGB(0,0,0));//填充背景色
	MemDC.SetMapMode(MM_ANISOTROPIC);//MemDC自定义坐标系
	MemDC.SetWindowExt(rect.Width(),rect.Height());
	MemDC.SetViewportExt(rect.Width(),-rect.Height());
	MemDC.SetViewportOrg(rect.Width()/2,rect.Height()/2);
	SetTimer(1,50,NULL);
	ReadPoint();
	ReadFace();
	tran.RotateX(afa);//绕x轴旋转
	tran.RotateY(beta);//绕y轴旋转
	DrawObject(&MemDC);
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//将内存位图拷贝到屏幕
	MemDC.SelectObject(pOldBitmap);//恢复位图
	NewBitmap.DeleteObject();//删除位图
	MemDC.DeleteDC();//删除MemDC
	ReleaseDC(pDC);//释放DC
}

void CTestView::DrawObject(CDC *pDC)//绘制物体
{
	zbuf=new CZBuffer;//申请内存
	zbuf->InitDeepBuffer(1000,1000,-1000);//深度初始化
	for(int i=0;i<NumR1;i++)//循环计算每个顶点的平均矢量
	{
		for(int j=0;j<NumR2;j++)
		{
			int beforei=i-1,beforej=j-1;
			int afteri=i+1,afterj=j+1;
			if(i==0) beforei=NumR1-1;
			if(j==0) beforej=NumR2-1;
			if(afteri==NumR1) afteri=0;
			if(afterj==NumR2) afterj=0;
			CVector VN0,VN1,VN2,VN3,VN;//四个面片的法矢量
			CVector edge00(P[beforei*NumR2+beforej],P[beforei*NumR2+j]);
			CVector edge01(P[beforei*NumR2+j],P[i*NumR2+j]);
			VN0=edge00*edge01;
			CVector edge10(P[beforei*NumR2+j],P[beforei*NumR2+afterj]);
			CVector edge11(P[beforei*NumR2+afterj],P[i*NumR2+afterj]);
			VN1=edge10*edge11;
			CVector edge20(P[i*NumR2+j],P[i*NumR2+afterj]);
			CVector edge21(P[i*NumR2+afterj],P[afteri*NumR2+afterj]);
			VN2=edge20*edge21;
			CVector edge30(P[i*NumR2+beforej],P[i*NumR2+j]);
			CVector edge31(P[i*NumR2+j],P[afteri*NumR2+j]);
			VN3=edge30*edge31;
			VN=VN0+VN1+VN2+VN3;
			PV[i*NumR2+j]=VN;//顶点的平均法矢量
			P[i*NumR2+j].c=pLight->Lighting(ViewPoint,P[i*NumR2+j],VN,pMaterial);//顶点的颜色
		}
	}
	for( i=0;i<NumR1;i++)
	{
		for(int j=0;j<NumR2;j++)
		{
			CPi3 Point4[4];
			CVector Normal4[4];
			CVector VS(P[F[i][j].p[1]],ViewPoint);//面的视矢量
			F[i][j].SetNormal(P[F[i][j].p[0]],P[F[i][j].p[1]],P[F[i][j].p[2]]);
			if(Dot(VS,F[i][j].Normal)>=0)//背面剔除
			{
				for(int m=0;m<4;m++)
				{
					PerProject(P[F[i][j].p[m]]);
					Point4[m]=ScreenP;
					Normal4[m]=PV[F[i][j].p[m]];
				}
				if(Model==0)//线框模型
				{
					AntiColorLine(pDC,Point4[0],Point4[1]);
					AntiColorLine(pDC,Point4[1],Point4[2]);
					AntiColorLine(pDC,Point4[2],Point4[3]);
					AntiColorLine(pDC,Point4[3],Point4[0]);
				}
				else if(Model==1)//表面模型
				{
					if(Render==0)//Gouraud光强插值算法填充
					{
						zbuf->SetPoint(Point4,4);//初始化
						zbuf->CreateBucket();//创建桶表
						zbuf->CreateEdgeforGouraud();//创建边表
						zbuf->Gouraud(pDC);//填充四边形
						zbuf->ClearMemory();
					}
					if (Render==1)//Phong法矢插值算法填充
					{ 
					  zbuf->SetPoint(Point4,Normal4,4);//初始化
					  zbuf->CreateBucket();//创建桶表
					  zbuf->CreateEdgeforPhong();//创建边表
					  zbuf->Phong(pDC,ViewPoint,pLight,pMaterial);//填充四边形
						zbuf->ClearMemory();
					}
				}
				else if(Model==2)//纹理模型
				{
					CP2 uv[4];//纹理坐标
					double tempi=i+1,tempj=j+1;
					uv[0].x=RingAfa*i/360.0*800;    uv[0].y=RingBeta*j/360.0*320;
					uv[1].x=RingAfa*i/360.0*800;	uv[1].y=(RingBeta*tempj)/360.0*320;
					uv[2].x=RingAfa*tempi/360.0*800;uv[2].y=(RingBeta*tempj)/360.0*320;
					uv[3].x=RingAfa*tempi/360.0*800;uv[3].y=RingBeta*j/360.0*320;
					if(Switch==0)//光照开关“关”
					{
						zbuf->SetPoint(Point4,Normal4,uv,4);//初始化
						zbuf->CreateBucket();//创建桶表
						zbuf->CreateEdgeforTexture();//创建边表
						zbuf->TextureMap(pDC,zbuf,Image,&bm);//纹理填充四边形
						zbuf->ClearMemory();//清理内存
					}
					if(Switch==1)//光照开关“开”
					{						
					    if(Render==1)//Phong法矢插值算法填充
						{
							zbuf->SetPoint(Point4,Normal4,uv,4);
							zbuf->CreateBucket();//创建桶表
							zbuf->CreateEdgeforTexture();//创建边表
							zbuf->TextureMapWithPhong(pDC,zbuf,Image,&bm,ViewPoint,pLight);//光照纹理填充四边形
							zbuf->ClearMemory();//清理内存
						}
					}					
				}				
			}
		}
	}
    //清理临时变量，防止内存泄露
	if(zbuf!=NULL)
	{
		delete zbuf;
		zbuf=NULL;
	}
	if (P!=NULL)
    {
		delete []P;
		P=NULL;
    }
	if (PV!=NULL)
	{
		delete []PV;
		PV=NULL;
	}
	if (F!=NULL)
	{
		for (int k=0;k<NumR1;k++)
		{
			delete [] F[k];
			F[k]=NULL;
		}
		delete [] F;
		F=NULL;
	}
}

void CTestView::AntiColorLine(CDC* pDC, CPi3 p0, CPi3 p1)
{
	CACLine aline;
	aline.MoveTo(pDC,p0.x,p0.y,p0.c);
	aline.LineTo(pDC,p1.x,p1.y,p1.c);
}	

void CTestView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CTestDoc* pDoc=GetDocument();
	if(((CMainFrame *)AfxGetMainWnd())->IsPlay)
	{
		afa=(afa<360.0)?afa+5.0:0;		
		Switch=pDoc->m_Switch;		
		R1=pDoc->m_R1;
		R2=pDoc->m_R2;
		NumR1=pDoc->m_NumR1;
		NumR2=pDoc->m_NumR2;
		Model=pDoc->m_Model;
		Render=pDoc->m_Render;
		Invalidate(FALSE);
	}
	if(Switch==0)//光照开关“关”
	{
		for(int i=0;i<LightNum;i++)
		{
			pLight->Light[i].L_OnOff=FALSE;//关闭光源	
		}
	}
	else if(Switch==1)//光照开关“开”
	{
		for(int i=0;i<LightNum;i++)
		{
			pLight->Light[i].L_OnOff=TRUE;//开启光源	
		}
	}
	if(pDoc->m_PathName !="" && pDoc->m_PathName!=PathName)
	{
		PathName=pDoc->m_PathName;
		if(Image!=NULL)
		{
			delete []Image;
			Image=NULL;
		}
		ReadImage();
	}
	CView::OnTimer(nIDEvent);
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//禁止背景刷新 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}
void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadImage();
	InitParameter();
}

void CTestView::ReadImage()
{
	if(PathName==" ")
	{
		MessageBox("文件不存在");
		return;
	}
	HANDLE hBitmap=LoadImage(GetModuleHandle(NULL),PathName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//从文件中导入图片
	if(hBitmap==NULL)
	{
		MessageBox("图片加载失败");
		return;
	}
	CBitmap NewBitmap;
	NewBitmap.Attach((HBITMAP)hBitmap);
	NewBitmap.GetBitmap(&bm);//获得CBitmap的信息到Bitmap结构体中
	int nbytesize=(bm.bmHeight*bm.bmWidth*bm.bmBitsPixel+7)/8;//获得位图的总字节数
	Image=new BYTE[nbytesize];//开辟装载位图的缓冲区
	NewBitmap.GetBitmapBits(nbytesize,(LPVOID)Image);//将位图拷贝到缓冲区
}

