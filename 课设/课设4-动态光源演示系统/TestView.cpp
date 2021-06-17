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
	Play=FALSE;
	RedLight=1,GreenLight=1,BlueLight=1;
	ra=400,rb=300;ga=300,gb=200;ba=500,bb=400;Alpha=0;
	LightNum=3;//��Դ����
	pLight=new CLighting(LightNum);//һά��Դ��̬����
	for(int i=0;i<LightNum;i++)
	{
		pLight->Light[i].L_Diffuse=CRGB(1.0,1.0,1.0);//��Դ����������ɫ
		pLight->Light[i].L_Specular=CRGB(1.0,1.0,1.0);//��Դ��ɫ
		pLight->Light[i].L_C0=1.0;//����˥��ϵ��
		pLight->Light[i].L_C1=0.00000001;//����˥��ϵ��
		pLight->Light[i].L_C2=0.000000001;//����˥��ϵ��
		pLight->Light[i].L_OnOff=TRUE;//������Դ	
	}
	pMaterial=new CMaterial;//һά���ʶ�̬����
	pMaterial->M_Ambient=CRGB(0.5,0.5,0.5);//���ʶԻ�����ķ�����
	pMaterial->M_Diffuse=CRGB(0.5,0.5,0.5);//���ʶ��������ķ�����
	pMaterial->M_Specular=CRGB(0.5,0.5,0.5);//���ʶԾ��淴���ķ�����
	pMaterial->M_Emit=CRGB(0.0,0.0,0.0);//��������ɢ����ɫ
	pMaterial->M_Exp=1.0;//�߹�ָ��
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
	SetTimer(1,50,NULL);
	InitModel();
	SetLight();
	DrawScene(&MemDC);
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//���ڴ�λͼ��������Ļ
	MemDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
	MemDC.DeleteDC();//ɾ��MemDC
	ReleaseDC(pDC);//�ͷ�DC
}

void CTestView::DrawScene(CDC *pDC)//��������
{
	CZBuffer *zbuf=new CZBuffer;//�����ڴ�
	zbuf->InitDeepBuffer(1000,1000,-1000);//��ȳ�ʼ��
	Globe.DrawObject(pDC,pLight,pMaterial,zbuf,GLOBE);//��������
	if(RedLight)
	{
		RedGTran=CP3(ra*cos(Alpha*PI/180),rb*sin(Alpha*PI/180),0);
		Tran_RedG.Translate(RedGTran.x,RedGTran.y,RedGTran.z);
		RedG.DrawObject(pDC,NULL,NULL,zbuf,REDG);//���ƺ�ɫ��Դ
	}
	if(GreenLight)
	{
		GreenGTran=CP3(0,ga*cos(Alpha*PI/180),gb*sin(Alpha*PI/180));
		Tran_GreenG.Translate(GreenGTran.x,GreenGTran.y,GreenGTran.z);
		GreenG.DrawObject(pDC,NULL,NULL,zbuf,GREENG);//������ɫ��Դ
	}
	if(BlueLight)
	{
		BlueGTran=CP3(bb*sin(Alpha*PI/180),0,ba*cos(Alpha*PI/180));
		Tran_BlueG.Translate(BlueGTran.x,BlueGTran.y,BlueGTran.z);
		BlueG.DrawObject(pDC,NULL,NULL,zbuf,BLUEG);//������ɫ��Դ
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
		pLight->Light[i].SetPosition(LightPos[i].x,LightPos[i].y,LightPos[i].z);//��Դλ��
		pLight->Light[i].L_Specular=LightClr[i];//��Դ��ɫ
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

void CTestView::OnTimer(UINT nIDEvent)//��ʱ������ 
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

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//��ֹ����ˢ�� 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}


