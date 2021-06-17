// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#include "Mainfrm.h"//
#include "math.h"//��ѧ����
#define  PI 3.14159265//Բ����
#define ROUND(a) int(a+0.5)//��������
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
	Model=1;//����
	R=200,d=800,Phi=90.0,Theta=0.0;
	afa=0.0;beta=0.0;
	pMaterial=NULL;pLight=NULL;
	LightNum=1;//��Դ����
	pLight=new CLighting(LightNum);//һά��Դ��̬����
	pLight->Light[0].SetPosition(0,800,800);//���õ�һ����Դλ������
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(1.0,1.0,1.0);//��Դ����������ɫ	
		pLight->Light[i].L_Specular=CRGB(1.0,1.0,1.0);//��Դ����߹���ɫ
		pLight->Light[i].L_C0=1.0;//����˥��ϵ��
		pLight->Light[i].L_C1=0.0000001;//����˥��ϵ��
		pLight->Light[i].L_C2=0.00000001;//����˥��ϵ��
		pLight->Light[i].L_OnOff=TRUE;//������Դ	
	}
	pMaterial=new CMaterial;//һά���ʶ�̬����
	pMaterial->M_Ambient=CRGB(0.3,0.3,0.3);//���ʶԻ�����ķ�����
	pMaterial->M_Diffuse=CRGB(0.85,0.08,0.0);//���ʶ��������ķ�����
	pMaterial->M_Specular=CRGB(0.828,0.8,0.8);//���ʶԾ��淴���ķ�����
	pMaterial->M_Emit=CRGB(0.0,0.0,0.0);//��������ɢ����ɫ
	pMaterial->M_Exp=20.0;//�߹�ָ��
	if(load3ds.Load3DSFile("football.3DS")==FALSE)//��ȡ3ds�ļ�
		exit(1);
}

CTestView::~CTestView()
{	
	if(pLight!=NULL)
	{
		delete[] pLight;
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
	MemDC.SetMapMode(MM_ANISOTROPIC);//MemDC�Զ�������ϵ
	MemDC.SetWindowExt(rect.Width(),rect.Height());
	MemDC.SetViewportExt(rect.Width(),-rect.Height());
	MemDC.SetViewportOrg(rect.Width()/2,rect.Height()/2);
	SetTimer(1,15,NULL);
	DrawObject(&MemDC);
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//���ڴ�λͼ��������Ļ
	MemDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
	MemDC.DeleteDC();//ɾ��MemDC
	ReleaseDC(pDC);//�ͷ�DC
}

void CTestView::InitParameter()
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

void CTestView::DrawObject(CDC *pDC)//��������
{
	CZBuffer *zbuf=new CZBuffer;//�����ڴ�
	zbuf->InitDeepBuffer(800,800,-1000);//��ȳ�ʼ��
	for(int i=0;i<load3ds.NumOfObject;i++)//������12��������κ�20�������������
	{
		CObjects *Obj=load3ds.pObject[i];
		CVector	*pNormal=Obj->pVertNormal;
		CP3	*pVertex=Obj->pVertex;		
		for(int j=0;j<Obj->NumOfFace;j++)//���������ÿһ����������Ƭ
		{ 
			CP3 temp;
			CPi3 Point[3];
			CVector Normal3[3];
			CMaterial3d CurrentMat=load3ds.pMaterial[Obj->pFace[j].MaterialIndex];//��Ĳ�����Ϣ
			for(int k=0;k<3;k++)//������������Ƭ�Ķ���ͷ�ʸ��
			{
				P[k]=pVertex[Obj->pFace[j].p[k]];
				Normal3[k]=pNormal[Obj->pFace[j].p[k]];
			}
			CP3 PT=CP3(P[1].x,P[1].y,P[1].z);
			CVector VS(PT,ViewPoint);//�����ʸ��
			if(Dot(VS,Obj->pFaceNormal[j])>=0)//�����޳�
			{				
				for(int k=0;k<3;k++)//����������
				{
					CRGB MaterialClr(CurrentMat.DifColor[0]/255.0,CurrentMat.DifColor[1]/255.0,CurrentMat.DifColor[2]/255.0);
					pMaterial->SetDiffuse(MaterialClr);
					Point[k].c=pLight->Lighting(ViewPoint,P[k],Normal3[k],pMaterial);//���ù��պ������㶥�����ɫ
					PerProject(P[k]);//͸�ӱ任
					Point[k].x=ScreenP.x;
					Point[k].y=ROUND(ScreenP.y);
					Point[k].z=ScreenP.z;
				}				
				if(Model)//���Ʊ���ģ��
				{					
					zbuf->SetPoint(Point,3);			
					zbuf->CreateBucket();//����Ͱ��
					zbuf->CreateEdge();//�����߱�
					zbuf->Gouraud(pDC);//��ɫ�������
					zbuf->ClearMemory();		
				}
				else//�����߿�ģ��
				{
					CLine *line=new CLine;
					line->MoveTo(pDC,Point[0].x,Point[0].y);
					for(int i=1;i<4;++i)
						line->LineTo(pDC,Point[i%3].x,Point[i%3].y);
					delete line;
				}
			}			
		}
		tran.SetMat(load3ds.pObject[i]->pVertex,load3ds.pObject[i]->NumOfVertex);
		tran.RotateX(afa);//x������ת����
		tran.RotateY(beta); //y������ת����
	}	
	load3ds.CalculateNormal();
	delete zbuf;//�ͷ��ڴ�
}

void CTestView::OnTimer(UINT nIDEvent)//��ʱ������ 
{
	// TODO: Add your message handler code here and/or call default
	CTestDoc* pDoc=GetDocument();
	if(((CMainFrame*)AfxGetMainWnd())->IsPlay)
	{
		Model=pDoc->m_Model;
		afa=5;beta=5;
		Invalidate(FALSE);
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
	for(int i=0;i<load3ds.NumOfObject;i++)
	{
		tran.SetMat(load3ds.pObject[i]->pVertex,load3ds.pObject[i]->NumOfVertex);
		tran.RotateX(100);//��ʼλ��
		tran.RotateY(0);
	}	
	load3ds.CalculateNormal();
	InitParameter();	
}
