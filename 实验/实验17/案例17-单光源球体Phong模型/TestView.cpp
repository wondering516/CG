// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"

#include "TestDoc.h"
#include "TestView.h"
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
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_MENUPlay, OnMENUPlay)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_UPDATE_COMMAND_UI(ID_MENUPlay, OnUpdateMENUPlay)
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
	R=1000,d=800,Phi=90.0,Theta=0.0;	
	afa=0.0;beta=0.0;
	LightNum=1;//��Դ����
	pLight=new CLighting(LightNum);//һά��Դ��̬����
    pLight->Light[0].SetPosition(800,800,800);//���õ�һ����Դλ������
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(1.0,1.0,1.0); //��Դ����������ɫ	
		pLight->Light[i].L_Specular=CRGB(1.0,1.0,1.0);//��Դ����߹���ɫ
		pLight->Light[i].L_C0=1.0;//����˥��ϵ��
		pLight->Light[i].L_C1=0.0000001;//����˥��ϵ��
		pLight->Light[i].L_C2=0.00000001;//����˥��ϵ��
		pLight->Light[i].L_OnOff=TRUE;//��Դ����	
	}
 	pMaterial=new CMaterial;//һά���ʶ�̬����
	pMaterial->M_Ambient=CRGB(0.547,0.08,0.0);//���ʶԻ�����ķ�����
	pMaterial->M_Diffuse=CRGB(0.85,0.08,0.0);//���ʶ��������ķ�����
	pMaterial->M_Specular=CRGB(0.828,0.8,0.8);//���ʶԾ��淴���ķ�����
	pMaterial->M_Emit=CRGB(0.2,0.0,0.0);//��������ɢ����ɫ
	pMaterial->M_Exp=20.0;//�߹�ָ��
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
	if(P!=NULL)
	{
		delete[] P;
		P=NULL;
	}
	for(int n=0;n<N1;n++)//ע�⳷������,���к���,�������෴
	{
		delete[] F[n];
		F[n]=NULL;
	}
	delete[] F;
	F=NULL;
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
	int gafa=4,gbeta=4;//��Ƭ�н�
	N1=180/gafa,N2=360/gbeta;//N1Ϊγ������,N2Ϊ��������
	P=new CP3[(N1-1)*N2+2];//γ�ȷ�����ϱ��������С�N1��1�����㣬��2�������ϱ�������
	double afa1,beta1,r=300;//rΪ��뾶
	P[0].x=0;
	P[0].y=r;
	P[0].z=0;
	//����ѭ�����������ϵĵ�����
	for(int i=0;i<N1-1;i++)
	{
		afa1=(i+1)*gafa*PI/180;
		for(int j=0;j<N2;j++)
		{
			beta1=j*gbeta*PI/180;		
			P[i*N2+j+1].x=r*sin(afa1)*sin(beta1);
			P[i*N2+j+1].y=r*cos(afa1);	
			P[i*N2+j+1].z=r*sin(afa1)*cos(beta1);							
		}
	}
	//�����ϼ�������
	P[(N1-1)*N2+1].x=0;
	P[(N1-1)*N2+1].y=-r;
	P[(N1-1)*N2+1].z=0;	
}

void CTestView::ReadFace()//�������
{
	//���ö�ά��̬����
	F=new CFace *[N1];//������
	for(int n=0;n<N1;n++)
	{
		F[n]=new CFace[N2];//������
	}
	for(int j=0;j<N2;j++)//���챱����������Ƭ
	{
		int tempj=j+1;
		if(tempj==N2) tempj=0;//��Ƭ����β����
		int NorthIndex[3];//������������Ƭ����������
		NorthIndex[0]=0;
		NorthIndex[1]=j+1;
		NorthIndex[2]=tempj+1;
		F[0][j].SetEN(3);
		for(int k=0;k<F[0][j].En;k++)//�������е������
		{
			F[0][j].p[k]=NorthIndex[k];
		}
	}
	for(int i=1;i<N1-1;i++)//���������ı�����Ƭ
	{
		for(int j=0;j<N2;j++)
	    {
			int tempi=i+1;
			int tempj=j+1;
			if(tempj==N2) tempj=0;
			int BodyIndex[4];//�����ı�����Ƭ����������
			BodyIndex[0]=(i-1)*N2+j+1;
			BodyIndex[1]=(tempi-1)*N2+j+1;
			BodyIndex[2]=(tempi-1)*N2+tempj+1;
			BodyIndex[3]=(i-1)*N2+tempj+1;
			F[i][j].SetEN(4);
			for(int k=0;k<F[i][j].En;k++)
			{
				F[i][j].p[k]=BodyIndex[k];
			}			
		}
	}
	for(j=0;j<N2;j++)//�����ϼ���������Ƭ
	{
		int tempj=j+1;
		if(tempj==N2) tempj=0;
		int SouthIndex[3];//�ϼ���������Ƭ����������
		SouthIndex[0]=(N1-2)*N2+j+1;
		SouthIndex[1]=(N1-1)*N2+1;
		SouthIndex[2]=(N1-2)*N2+tempj+1;
		F[N1-1][j].SetEN(3);	
		for(int k=0;k<F[N1-1][j].En;k++)
		{
			F[N1-1][j].p[k]=SouthIndex[k];
		}	
	}
}

void CTestView::InitParameter()
{
	k[1]=sin(PI*Theta/180);//sin(theta)
	k[2]=sin(PI*Phi/180);//sin(phi)
	k[3]=cos(PI*Theta/180);//cos(theta)
	k[4]=cos(PI*Phi/180);//cos(phi)
	k[5]=k[2]*k[3];//sin(phi)*cos(theta)
	k[6]=k[2]*k[1];//sin(phi)*sin(theta)
	k[7]=k[4]*k[3];//cos(phi)*cos(theta)
	k[8]=k[4]*k[1];//cos(phi)*sin(theta)
	ViewPoint.x=R*k[6];//�û�����ϵ���ӵ�������
	ViewPoint.y=R*k[4];
	ViewPoint.z=R*k[5];
}

void CTestView::PerProject(CP3 P)
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
	MemDC.FillSolidRect(rect,RGB(128,0,2));//��䱳��ɫ
	MemDC.SetMapMode(MM_ANISOTROPIC);//MemDC�Զ�������ϵ
	MemDC.SetWindowExt(rect.Width(),rect.Height());
	MemDC.SetViewportExt(rect.Width(),-rect.Height());
	MemDC.SetViewportOrg(rect.Width()/2,rect.Height()/2);
	DrawObject(&MemDC);
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//���ڴ�λͼ��������Ļ
	MemDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
	MemDC.DeleteDC();//ɾ��MemDC
	ReleaseDC(pDC);//�ͷ�DC
}

void CTestView::DrawObject(CDC *pDC)//��������
{
	CZBuffer *zbuf=new CZBuffer;//�����ڴ�
	zbuf->InitDeepBuffer(800,800,-1000);//��ȳ�ʼ��
	CPi3 Point3[3];//�ϱ�����������
	CVector Normal3[3];
	CPi3 Point4[4];//���嶥������
	CVector Normal4[4];
	for(int i=0;i<N1;i++)
	{
		for(int j=0;j<N2;j++)
		{
			CVector VS(P[F[i][j].p[1]],ViewPoint);//�����ʸ��
			F[i][j].SetNormal(P[F[i][j].p[0]],P[F[i][j].p[1]],P[F[i][j].p[2]]);//����С��Ƭ��ʸ��
			if(Dot(VS,F[i][j].Normal)>=0)
			{
				if(F[i][j].En==3)
				{
					for(int m=0;m<F[i][j].En;m++)
					{
						PerProject(P[F[i][j].p[m]]);
						Point3[m]=ScreenP;
						Normal3[m]=CVector(P[F[i][j].p[m]]);
					}	
					zbuf->SetPoint(Point3,Normal3,3);//��ʼ��
					zbuf->CreateBucket();//����Ͱ��
					zbuf->CreateEdge();//�����߱�
					zbuf->Phong(pDC,ViewPoint,pLight,pMaterial);//��ɫ�������������
					zbuf->ClearMemory();
				}
				else
				{
					for(int m=0;m<F[i][j].En;m++)
					{
						PerProject(P[F[i][j].p[m]]);
						Point4[m]=ScreenP;
						Normal4[m]=CVector(P[F[i][j].p[m]]);
					}
					zbuf->SetPoint(Point4,Normal4,4);//��ʼ��
					zbuf->CreateBucket();//����Ͱ��
					zbuf->CreateEdge();//�����߱�
					zbuf->Phong(pDC,ViewPoint,pLight,pMaterial);//��ɫ��������ı���
					zbuf->ClearMemory();
				}
			} 	
		}	
	}	
	delete zbuf;
}
	
void CTestView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if(!Play)
	{
		switch(nChar)
		{
		case VK_UP:
			afa=-5;
			tran.RotateX(afa);
			break;
		case VK_DOWN:
			afa=5;
			tran.RotateX(afa);
			break;
		case VK_LEFT:
			beta=-5;
			tran.RotateY(beta);
			break;
		case VK_RIGHT:
			beta=5;
			tran.RotateY(beta);
			break;
		default:
			break;			
		}
		Invalidate(FALSE);		
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CTestView::OnMENUPlay() 
{
	// TODO: Add your command handler code here
	Play=Play?FALSE:TRUE;
	if (Play)//���ö�ʱ��
		SetTimer(1,15,NULL);	
	else
		KillTimer(1);
}

void CTestView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	afa=5;beta=5;
	tran.RotateX(afa);
	tran.RotateY(beta);
	Invalidate(FALSE);
	CView::OnTimer(nIDEvent);
}


BOOL CTestView::OnEraseBkgnd(CDC* pDC)//��ֹ����ˢ�� 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::OnUpdateMENUPlay(CCmdUI* pCmdUI)//������ť״̬����  
{
	// TODO: Add your command update UI handler code here
	if(Play)
	{
		pCmdUI->SetCheck(TRUE);
		pCmdUI->SetText("ֹͣ");
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->SetText("����");
	}		
}

void CTestView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	ReadPoint();
	ReadFace();
	tran.SetMat(P,(N1-1)*N2+2);
	InitParameter();
}
