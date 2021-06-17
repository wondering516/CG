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
	Radius=150;FaceCount=4;
	Model=0;//�߿�
	Switch=0;//�޹���
	TranslateX=0;TranslateY=0;TranslateZ=0;
	DirectionX=1;DirectionY=1;DirectionZ=1;
	RotateX=0;RotateY=0;RotateZ=0;
	pMaterial=NULL;pLight=NULL;
	LightNum=1;//��Դ����
	pLight=new CLighting(LightNum);//һά��Դ��̬����	
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
	pMaterial->M_Emit=CRGB(0.2,0.0,0.0);//��������ɢ����ɫ
	pMaterial->M_Exp=20.0;//�߹�ָ��
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

void CTestView::DoubleBuffer()
{
	CDC* pDC=GetDC();
	CRect Rect;//����ͻ���
	GetClientRect(&Rect);//��ÿͻ����Ĵ�С
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC�Զ�������ϵ
	pDC->SetWindowExt(Rect.Width(),Rect.Height());//���ô��ڷ�Χ
	pDC->SetViewportExt(Rect.Width(),-Rect.Height());//x��ˮƽ���ң�y�ᴹֱ����
	pDC->SetViewportOrg(Rect.Width()/2,Rect.Height()/2);//��Ļ����Ϊԭ��
	CDC MemDC;//�ڴ�DC
	CBitmap NewBitmap,*pOldBitmap;//�ڴ��г���ͼ�����ʱλͼ
	MemDC.CreateCompatibleDC(pDC);//��������ĻpDC���ݵ�MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC,Rect.Width(),Rect.Height());//��������λͼ 
	pOldBitmap=MemDC.SelectObject(&NewBitmap); //������λͼѡ��MemDC 
	MemDC.SetMapMode(MM_ANISOTROPIC);//MemDC�Զ�������ϵ
	MemDC.SetWindowExt(Rect.Width(),Rect.Height());
	MemDC.SetViewportExt(Rect.Width(),-Rect.Height());
	MemDC.SetViewportOrg(Rect.Width()/2,Rect.Height()/2);
	SetTimer(1,50,NULL);
	ReadPoint();//������
	ReadFace(); //�������		
	tran.Translate(TranslateX,TranslateY,TranslateZ);//ƽ�Ʊ任
	tran.RotateX(RotateX,CP3(TranslateX,TranslateY,TranslateZ));//��X����ת�任
	tran.RotateY(RotateY,CP3(TranslateX,TranslateY,TranslateZ));//��Y����ת�任
	tran.RotateZ(RotateZ,CP3(TranslateX,TranslateY,TranslateZ));//��Z����ת�任
	BorderCheck();
	DrawObject(&MemDC);	
	pDC->BitBlt(-Rect.Width()/2,-Rect.Height()/2,Rect.Width(),Rect.Height(),&MemDC,-Rect.Width()/2,-Rect.Height()/2,SRCCOPY);//���ڴ�λͼ��������Ļ
	MemDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
	MemDC.DeleteDC();//ɾ��MemDC
	ReleaseDC(pDC);//�ͷ�DC
}

void CTestView::ReadPoint()//���붥������
{
	P[0].x=0;      P[0].y=Radius; P[0].z=0;      P[0].w=1;
	P[1].x=0;      P[1].y=0;      P[1].z= Radius;P[1].w=1;
	P[2].x=Radius; P[2].y=0;      P[2].z=0;      P[2].w=1;
	P[3].x=0;      P[3].y=0;      P[3].z=-Radius;P[3].w=1;
	P[4].x=-Radius;P[4].y=0;      P[4].z=0;      P[4].w=1; 
	P[5].x=0;      P[5].y=-Radius;P[5].z=0;      P[5].w=1; 
	P[6].x=0;      P[6].y=0;      P[6].z=0;      P[6].w=1;//��������
	tran.SetMat(P,7);
}

void CTestView::ReadFace()//���
{
	//��һ��Ϊÿ����ı���,������Ϊ��Ķ�����
	F[0].SetEN(3);F[0].p[0]=0;F[0].p[1]=1;F[0].p[2]=2;
	F[1].SetEN(3);F[1].p[0]=0;F[1].p[1]=2;F[1].p[2]=3;
	F[2].SetEN(3);F[2].p[0]=0;F[2].p[1]=3;F[2].p[2]=4;
	F[3].SetEN(3);F[3].p[0]=0;F[3].p[1]=4;F[3].p[2]=1;
	F[4].SetEN(3);F[4].p[0]=5;F[4].p[1]=2;F[4].p[2]=1;
	F[5].SetEN(3);F[5].p[0]=5;F[5].p[1]=3;F[5].p[2]=2;
	F[6].SetEN(3);F[6].p[0]=5;F[6].p[1]=4;F[6].p[2]=3;
	F[7].SetEN(3);F[7].p[0]=5;F[7].p[1]=1;F[7].p[2]=4;
}

void CTestView::DrawObject(CDC *pDC)//��������
{
	CP3 Point[3],t;
	for(int nFace=0;nFace<8;nFace++)//��ѭ��
	{		
		for(int nEdge=0;nEdge<F[nFace].En;nEdge++)//��ѭ��
		{
			Point[nEdge]=P[F[nFace].p[nEdge]];		
		}
		SubDivide(pDC,Point[0],Point[1],Point[2],FaceCount);
	}	
}

void CTestView::SubDivide(CDC *pDC,CP3 p0, CP3 p1, CP3 p2,int n)//�ݹ麯��
{
	if(n==0)
	{
		DrawTriangle(pDC,p0,p1,p2);
		return;
	}
	else
	{
		CP3 p01,p12,p20;
		p01=(p0+p1)/2;
		p12=(p1+p2)/2;
		p20=(p2+p0)/2;	
		Normalize(p01);//��չģ��
		Normalize(p12);
		Normalize(p20);
		SubDivide(pDC,p0,p01,p20,n-1);//�ݹ����
		SubDivide(pDC,p1,p12,p01,n-1);
		SubDivide(pDC,p2,p20,p12,n-1);
		SubDivide(pDC,p01,p12,p20,n-1);
	}
}

void CTestView::Normalize(CP3 &p)//ģ����׼������
{
	p-=P[6];
	double mag=p.Mold();//ģ��
	if(mag==0)
		return;
	p/=mag;
	p*=Radius;
	p+=P[6];
}

void CTestView::DrawTriangle(CDC *pDC,CP3 p0, CP3 p1, CP3 p2)//���������κ���
{
	CP3 p[3];
	p[0]=p0;p[1]=p1;p[2]=p2;
	CP3 ViewPoint(P[6].x,P[6].y,P[6].z+1000);//������ʸ��������ȡ��λʸ��
	CVector VS(ViewPoint);//�����ʸ��
	CVector V01(p0,p1);//���һ����ʸ��
	CVector V12(p1,p2);//�����һ����ʸ��
	CVector VN=V01*V12;//��ķ�ʸ��	
	if(Dot(VS,VN)>=0 )
	{
		if(Model==0)//�߿�ģ��
		{
			if(Switch==0)//��Դ���ء��ء�
			{
				for(int i=0;i<LightNum;i++)
				{
					pLight->Light[i].L_OnOff=FALSE;//��Դ�ر�	
				}	
				CLine *line=new CLine;
				line->MoveTo(pDC,p0.x,p0.y);
				line->LineTo(pDC,p1.x,p1.y);
				line->LineTo(pDC,p2.x,p2.y);
				line->LineTo(pDC,p0.x,p0.y);
				delete line;
			}
			else//��Դ���ء�����
			{
				for(int i=0;i<LightNum;i++)
				{
					pLight->Light[i].L_OnOff=TRUE;//������Դ	
				}	
				pLight->Light[0].L_Position.x=P[6].x+1000;//���ù�Դλ�����Ͻ�
				pLight->Light[0].L_Position.y=P[6].y+1000;
				pLight->Light[0].L_Position.z=P[6].z+1000;
				CP2 Point[3];
				for(i=0;i<3;i++)
				{
					p[i].x-=P[6].x;p[i].y-=P[6].y;p[i].z-=P[6].z;
					CVector PNormal(p[i]);//���λ��ʸ��������õ���������ƽ����ʸ��
					p[i].x+=P[6].x;p[i].y+=P[6].y;p[i].z+=P[6].z;
					Point[i].x=p[i].x;
					Point[i].y=ROUND(p[i].y);
					Point[i].c=pLight->Lighting(ViewPoint,p[i],PNormal,pMaterial);//���ù��պ���
				}
				AntiColorLine(pDC,Point[0],Point[1]);
				AntiColorLine(pDC,Point[1],Point[2]);
				AntiColorLine(pDC,Point[2],Point[0]);			
			}
		}	
		else//����ģ��
		{
			if(Switch==0)//��Դ���ء��ء�
			{
				for(int i=0;i<LightNum;i++)
				{
					pLight->Light[i].L_OnOff=FALSE;//�رչ�Դ	
				}			
			}
			else//��Դ���ء�����
			{
				for(int i=0;i<LightNum;i++)
				{
					pLight->Light[i].L_OnOff=TRUE;//������Դ	
				}
			}			
			pLight->Light[0].L_Position.x=P[6].x+1000;//���ù�Դλ�����Ͻ�
			pLight->Light[0].L_Position.y=P[6].y+1000;
			pLight->Light[0].L_Position.z=P[6].z+1000;
			CPi2 Point[3];
			for(int i=0;i<3;i++)
			{
				p[i].x-=P[6].x;p[i].y-=P[6].y;p[i].z-=P[6].z;
				CVector PNormal(p[i]);//���λ��ʸ��������õ���������ƽ����ʸ��
				p[i].x+=P[6].x;p[i].y+=P[6].y;p[i].z+=P[6].z;
				Point[i].x=p[i].x;
				Point[i].y=ROUND(p[i].y);
				Point[i].c=pLight->Lighting(ViewPoint,p[i],PNormal,pMaterial);//���ù��պ���
			}
			CFill *fill=new CFill;//��������
			fill->SetPoint(Point,3);//��ʼ��CFill�����
			fill->CreateBucket();//����Ͱ��
			fill->CreateEdge();//�����߱�
			fill->Gouraud(pDC);//��ɫ�������������
			delete fill;
		}
	}
}

void CTestView::OnTimer(UINT nIDEvent)//��ʱ������ 
{
	// TODO: Add your message handler code here and/or call default
	CTestDoc* pDoc=GetDocument();
	if(((CMainFrame*)AfxGetMainWnd())->IsPlay)
	{
		Radius=pDoc->m_Radius;
		FaceCount=pDoc->m_FaceCount;
		Model=pDoc->m_Model;
		Switch=pDoc->m_Switch;
		TranslateX+=pDoc->m_TranslateX*DirectionX;
		TranslateY+=pDoc->m_TranslateY*DirectionY;
		TranslateZ+=pDoc->m_TranslateZ*DirectionZ;
		RotateX+=pDoc->m_RotateX;
		RotateY+=pDoc->m_RotateY;
		RotateZ+=pDoc->m_RotateZ;
		Invalidate(FALSE);
	}
	CView::OnTimer(nIDEvent);
}

void CTestView::BorderCheck()//�߽���ײ���
{
	CRect rect;
	GetClientRect(&rect);
	int nWidth=rect.Width()/2;
	int nHeight=rect.Height()/2;
	if(fabs(P[6].x)+Radius>nWidth)
	{
		DirectionX*=-1;
		TranslateX+=fabs(fabs(P[6].x)+Radius-nWidth)*DirectionX;//�ж�����ˮƽԽ��

	}
	if(fabs(P[6].y)+Radius>nHeight)
	{
		DirectionY*=-1;
		TranslateY+=fabs(fabs(P[6].y)+Radius-nHeight)*DirectionY;//�ж����崹ֱԽ��
	}	
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//��ֹ����ˢ�� 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::AntiColorLine(CDC* pDC,CP2 p0,CP2 p1)//������ɫ���䷴����ֱ��
{
	CACLine *aline=new CACLine;
	aline->MoveTo(pDC,p0);
	aline->LineTo(pDC,p1);
	delete aline;
}
