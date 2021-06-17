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
	R=1000,d=800,Phi=90.0,Theta=0.0;//�ӵ�ʼ��λ����ǰ��	
	afa=0.0;beta=0.0;//Բ����ʼλ�ý�
	LightNum=1;//��Դ����
	pLight=new CLighting(LightNum);//һά��Դ��̬����
    pLight->Light[0].SetPosition(0,0,1000);//���ù�Դλ������
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(1.0,1.0,1.0);//��Դ����������ɫ	
		pLight->Light[i].L_Specular=CRGB(0.828,0.636,0.366);//��Դ����߹���ɫ
		pLight->Light[i].L_C0=1.0;//����˥��ϵ��
		pLight->Light[i].L_C1=0.0000001;//����˥��ϵ��
		pLight->Light[i].L_C2=0.00000001;//����˥��ϵ��
		if(Switch)
			pLight->Light[i].L_OnOff=TRUE;//�رչ�Դ	
		else
			pLight->Light[i].L_OnOff=FALSE;//������Դ
	}
	pMaterial=new CMaterial;//һά���ʶ�̬����
	pMaterial->M_Ambient=CRGB(0.547,0.400,0.075);//���ʶԻ�����ķ�����
	pMaterial->M_Diffuse=CRGB(0.912,0.616,0.116);//���ʶ��������ķ�����
	pMaterial->M_Specular=CRGB(1.0,1.0,1.0);//���ʶԾ��淴���ķ�����
	pMaterial->M_Exp=100.0;//�߹�ָ��
	PathName = ".\\Data\\texture1.bmp";//��ʼ������λͼ·��
}

CTestView::~CTestView()
{
    //�����ڴ�
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
		
		for(int n=0;n<NumR1;n++)//ע�⳷������,���к���,�������෴
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
void CTestView::ReadPoint()//���������
{
	RingAfa=360/NumR1,RingBeta=360/NumR2;//��Ƭ����ΪNumR1��NumR2
	P=new CP3 [NumR1*NumR2];//���㶯̬����
	PV=new CVector[NumR1*NumR2];//���㷨ʸ����̬����
	for(int i=0;i<NumR1;i++)
	{
		double afa1=RingAfa*i*PI/180;
		for(int j=0;j<NumR2;j++)//���㸳ֵ
		{
			double beta1=RingBeta*j*PI/180;
			P[i*NumR2+j].x=R1*sin(afa1)+R2*sin(beta1)*sin(afa1);
			P[i*NumR2+j].y=R2*cos(beta1);
			P[i*NumR2+j].z=R1*cos(afa1)+R2*sin(beta1)*cos(afa1);
		}
	}
	tran.SetMat(P,NumR1*NumR2);//�任�����ʼ��
}

void CTestView::ReadFace()//�������
{
	//��Ķ�ά��̬����
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
			F[i][j].SetEN(4);//��ı���
			F[i][j].p[0]=i*NumR2+j;//������ıߺ�
			F[i][j].p[1]=i*NumR2+tempj;
			F[i][j].p[2]=tempi*NumR2+tempj; 
			F[i][j].p[3]=tempi*NumR2+j;
		}
	}
}

void CTestView::InitParameter()//͸�ӱ任����
{
	k[1]=sin(PI*Theta/180);
	k[2]=sin(PI*Phi/180);
	k[3]=cos(PI*Theta/180);
	k[4]=cos(PI*Phi/180);
	k[5]=k[2]*k[3];
	k[6]=k[2]*k[1];
	k[7]=k[4]*k[3];
	k[8]=k[4]*k[1];
	ViewPoint.x=R*k[6];//�û�����ϵ���ӵ�������
	ViewPoint.y=R*k[4];
	ViewPoint.z=R*k[5];
}

void CTestView::PerProject(CP3 P)//͸�ӱ任
{
	CP3 ViewP;
	ViewP.x=P.x*k[3]-P.z*k[1];
	ViewP.y=-P.x*k[8]+P.y*k[2]-P.z*k[7];
	ViewP.z=-P.x*k[6]-P.y*k[4]-P.z*k[5]+R;//��������ϵ���۲�����ϵ
    ViewP.c=P.c;
	ScreenP.x=d*ViewP.x/ViewP.z;//��Ļ����ϵ��ά����
	ScreenP.y=ROUND(d*ViewP.y/ViewP.z);
    ScreenP.z=d*(R-ViewP.z)/ViewP.z;
	ScreenP.c=ViewP.c;
}

void CTestView::DoubleBuffer()//˫����
{
	CDC* pDC=GetDC();
	CRect rect;//����ͻ���
	GetClientRect(&rect);//��ÿͻ����Ĵ�С
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC�Զ�������ϵ
	pDC->SetWindowExt(rect.Width(),rect.Height());//���ô��ڷ�Χ
	pDC->SetViewportExt(rect.Width(),-rect.Height());//x��ˮƽ���ң�y�ᴹֱ����
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//��Ļ����Ϊԭ��
	CDC MemDC;//�ڴ�DC
	CBitmap NewBitmap,*pOldBitmap;//�ڴ��г���ͼ�����ʱλͼ
	MemDC.CreateCompatibleDC(pDC);//��������ĻpDC���ݵ�MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//��������λͼ 
	pOldBitmap=MemDC.SelectObject(&NewBitmap);//������λͼѡ��MemDC 
	MemDC.FillSolidRect(rect,RGB(0,0,0));//��䱳��ɫ
	MemDC.SetMapMode(MM_ANISOTROPIC);//MemDC�Զ�������ϵ
	MemDC.SetWindowExt(rect.Width(),rect.Height());
	MemDC.SetViewportExt(rect.Width(),-rect.Height());
	MemDC.SetViewportOrg(rect.Width()/2,rect.Height()/2);
	SetTimer(1,50,NULL);
	ReadPoint();
	ReadFace();
	tran.RotateX(afa);//��x����ת
	tran.RotateY(beta);//��y����ת
	DrawObject(&MemDC);
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//���ڴ�λͼ��������Ļ
	MemDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
	MemDC.DeleteDC();//ɾ��MemDC
	ReleaseDC(pDC);//�ͷ�DC
}

void CTestView::DrawObject(CDC *pDC)//��������
{
	zbuf=new CZBuffer;//�����ڴ�
	zbuf->InitDeepBuffer(1000,1000,-1000);//��ȳ�ʼ��
	for(int i=0;i<NumR1;i++)//ѭ������ÿ�������ƽ��ʸ��
	{
		for(int j=0;j<NumR2;j++)
		{
			int beforei=i-1,beforej=j-1;
			int afteri=i+1,afterj=j+1;
			if(i==0) beforei=NumR1-1;
			if(j==0) beforej=NumR2-1;
			if(afteri==NumR1) afteri=0;
			if(afterj==NumR2) afterj=0;
			CVector VN0,VN1,VN2,VN3,VN;//�ĸ���Ƭ�ķ�ʸ��
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
			PV[i*NumR2+j]=VN;//�����ƽ����ʸ��
			P[i*NumR2+j].c=pLight->Lighting(ViewPoint,P[i*NumR2+j],VN,pMaterial);//�������ɫ
		}
	}
	for( i=0;i<NumR1;i++)
	{
		for(int j=0;j<NumR2;j++)
		{
			CPi3 Point4[4];
			CVector Normal4[4];
			CVector VS(P[F[i][j].p[1]],ViewPoint);//�����ʸ��
			F[i][j].SetNormal(P[F[i][j].p[0]],P[F[i][j].p[1]],P[F[i][j].p[2]]);
			if(Dot(VS,F[i][j].Normal)>=0)//�����޳�
			{
				for(int m=0;m<4;m++)
				{
					PerProject(P[F[i][j].p[m]]);
					Point4[m]=ScreenP;
					Normal4[m]=PV[F[i][j].p[m]];
				}
				if(Model==0)//�߿�ģ��
				{
					AntiColorLine(pDC,Point4[0],Point4[1]);
					AntiColorLine(pDC,Point4[1],Point4[2]);
					AntiColorLine(pDC,Point4[2],Point4[3]);
					AntiColorLine(pDC,Point4[3],Point4[0]);
				}
				else if(Model==1)//����ģ��
				{
					if(Render==0)//Gouraud��ǿ��ֵ�㷨���
					{
						zbuf->SetPoint(Point4,4);//��ʼ��
						zbuf->CreateBucket();//����Ͱ��
						zbuf->CreateEdgeforGouraud();//�����߱�
						zbuf->Gouraud(pDC);//����ı���
						zbuf->ClearMemory();
					}
					if (Render==1)//Phong��ʸ��ֵ�㷨���
					{ 
					  zbuf->SetPoint(Point4,Normal4,4);//��ʼ��
					  zbuf->CreateBucket();//����Ͱ��
					  zbuf->CreateEdgeforPhong();//�����߱�
					  zbuf->Phong(pDC,ViewPoint,pLight,pMaterial);//����ı���
						zbuf->ClearMemory();
					}
				}
				else if(Model==2)//����ģ��
				{
					CP2 uv[4];//��������
					double tempi=i+1,tempj=j+1;
					uv[0].x=RingAfa*i/360.0*800;    uv[0].y=RingBeta*j/360.0*320;
					uv[1].x=RingAfa*i/360.0*800;	uv[1].y=(RingBeta*tempj)/360.0*320;
					uv[2].x=RingAfa*tempi/360.0*800;uv[2].y=(RingBeta*tempj)/360.0*320;
					uv[3].x=RingAfa*tempi/360.0*800;uv[3].y=RingBeta*j/360.0*320;
					if(Switch==0)//���տ��ء��ء�
					{
						zbuf->SetPoint(Point4,Normal4,uv,4);//��ʼ��
						zbuf->CreateBucket();//����Ͱ��
						zbuf->CreateEdgeforTexture();//�����߱�
						zbuf->TextureMap(pDC,zbuf,Image,&bm);//��������ı���
						zbuf->ClearMemory();//�����ڴ�
					}
					if(Switch==1)//���տ��ء�����
					{						
					    if(Render==1)//Phong��ʸ��ֵ�㷨���
						{
							zbuf->SetPoint(Point4,Normal4,uv,4);
							zbuf->CreateBucket();//����Ͱ��
							zbuf->CreateEdgeforTexture();//�����߱�
							zbuf->TextureMapWithPhong(pDC,zbuf,Image,&bm,ViewPoint,pLight);//������������ı���
							zbuf->ClearMemory();//�����ڴ�
						}
					}					
				}				
			}
		}
	}
    //������ʱ��������ֹ�ڴ�й¶
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
	if(Switch==0)//���տ��ء��ء�
	{
		for(int i=0;i<LightNum;i++)
		{
			pLight->Light[i].L_OnOff=FALSE;//�رչ�Դ	
		}
	}
	else if(Switch==1)//���տ��ء�����
	{
		for(int i=0;i<LightNum;i++)
		{
			pLight->Light[i].L_OnOff=TRUE;//������Դ	
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

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//��ֹ����ˢ�� 
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
		MessageBox("�ļ�������");
		return;
	}
	HANDLE hBitmap=LoadImage(GetModuleHandle(NULL),PathName,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//���ļ��е���ͼƬ
	if(hBitmap==NULL)
	{
		MessageBox("ͼƬ����ʧ��");
		return;
	}
	CBitmap NewBitmap;
	NewBitmap.Attach((HBITMAP)hBitmap);
	NewBitmap.GetBitmap(&bm);//���CBitmap����Ϣ��Bitmap�ṹ����
	int nbytesize=(bm.bmHeight*bm.bmWidth*bm.bmBitsPixel+7)/8;//���λͼ�����ֽ���
	Image=new BYTE[nbytesize];//����װ��λͼ�Ļ�����
	NewBitmap.GetBitmapBits(nbytesize,(LPVOID)Image);//��λͼ������������
}

