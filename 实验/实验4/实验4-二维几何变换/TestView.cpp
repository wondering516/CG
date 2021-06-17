// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "Line.h"//ֱ����
#include "TestDoc.h"
#include "TestView.h"
#include "Mainfrm.h"//�����ͷ�ļ�
#define ROUND(a) int(a+0.5)//��������
#include "math.h"//��ѧͷ�ļ�
#define  PI 3.14159265//Բ����
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
	degree=0;
	translateX=0;
	translateY=0;
	rotate=0;
	directionX=1;
	directionY=1;
	scale=0;	
	R=100;
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
	CTestDoc* pDoc=GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
	SetTimer(1,50,NULL);//���ö�ʱ��
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
	pOldBitmap=MemDC.SelectObject(&NewBitmap); //������λͼѡ��MemDC 
	MemDC.SetMapMode(MM_ANISOTROPIC);//MemDC�Զ�������ϵ
	MemDC.SetWindowExt(rect.Width(),rect.Height());
	MemDC.SetViewportExt(rect.Width(),-rect.Height());
	MemDC.SetViewportOrg(rect.Width()/2,rect.Height()/2);
	ReadPoint();//����ͼ�ζ�������
	tran.Translate(translateX,translateY);//ƽ�Ʊ任
	tran.Rotate(rotate,CP2(translateX,translateY));//�������������ת�任
	tran.Scale(scale,scale,CP2(translateX,translateY));//����������ı����任
	DrawObject(&MemDC);
	BorderCheck();	
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//���ڴ�λͼ��������Ļ
	MemDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
	MemDC.DeleteDC();//ɾ��MemDC
	ReleaseDC(pDC);//�ͷ�DC
	if (P!=NULL)
	{
		delete []P;
		P=NULL;
	}
}
void CTestView::ReadPoint()
{
	double Dtheta=2*PI/degree;
	P=new CP2[degree+1];
	for(int i=0;i<degree;i++)
	{
		P[i].x=R*cos(i*Dtheta);
		P[i].y=R*sin(i*Dtheta);
	}
	P[degree].x=0;P[degree].y=0;//ͼ�����ĵ�
	tran.SetMat(P,degree+1);
	
}
void CTestView::DrawObject(CDC *pDC)//����ͼ��
{
	CLine *line=new CLine;
	for(int i=0;i<degree;i++)
	{
		line->MoveTo(pDC,ROUND(P[degree].x),ROUND(P[degree].y));
        line->LineTo(pDC,ROUND(P[i].x),ROUND(P[i].y));
		line->LineTo(pDC,ROUND(P[(i+1)%degree].x),ROUND(P[(i+1)%degree].y));
	}
	delete line;
}
void CTestView::OnTimer(UINT nIDEvent)//��ʱ������ 
{
	// TODO: Add your message handler code here and/or call default
	CTestDoc* pDoc=GetDocument();
	if(((CMainFrame*)AfxGetMainWnd())->IsPlay)
	{
		degree=pDoc->m_degree;
		translateX+=pDoc->m_translateX*directionX;
		translateY+=pDoc->m_translateY*directionY;
		rotate+=pDoc->m_rotate;	
		scale=pDoc->m_scale;		
		DoubleBuffer();
	}
	CView::OnTimer(nIDEvent);
}
void CTestView::BorderCheck()//�߽���
{
	double TempR=R*scale;
	CRect rect;//����ͻ���
	GetClientRect(&rect);//��ÿͻ����Ĵ�С
	int nWidth=rect.Width()/2;
	int nHeight=rect.Height()/2;
	if(fabs(P[degree].x)+TempR>nWidth)
	{
		directionX*=-1;
		translateX+=fabs(fabs(P[degree].x)+TempR-nWidth)*directionX;//�ж�����ˮƽԽ��
	}
	if(fabs(P[degree].y)+TempR>nHeight)
	{
		directionY*=-1;
		translateY+=fabs(fabs(P[degree].y)+TempR-nHeight)*directionY;//�ж����崹ֱԽ��
	}
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//��ֹ����ˢ�� 
{
	// TODO: Add your message handler code here and/or call default
	return  TRUE;
}


