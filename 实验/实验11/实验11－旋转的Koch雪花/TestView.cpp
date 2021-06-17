// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"

#include "TestDoc.h"
#include "TestView.h"
#include "math.h"//��ѧͷ�ļ�
#define  PI 3.1415926//PI�ĺ궨��
#define ROUND(x) int(x+0.5)//��������
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
	ON_COMMAND(ID_MDRAW, OnMdraw)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_MDRAW, OnUpdateMdraw)
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
	RotateAngle=0;
	Play=FALSE;
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

void CTestView::DoubleBuffer()//˫�����ͼ
{
	CDC* pDC=GetDC();
	CRect rect;//����ͻ���
	GetClientRect(&rect);//��ÿͻ����Ĵ�С
	pDC->SetMapMode(MM_ANISOTROPIC);//pDC�Զ�������ϵ
	pDC->SetWindowExt(rect.Width(),rect.Height());//���ô��ڷ�Χ
	pDC->SetViewportExt(rect.Width(),-rect.Height());//x��ˮƽ���ң�y�ᴹֱ����
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//��Ļ����Ϊԭ��
	CBitmap NewBitmap,*pOldBitmap;//�ڴ��г���ͼ�����ʱλͼ
	MemDC.CreateCompatibleDC(pDC);//��������ĻpDC���ݵ�MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//��������λͼ 
	pOldBitmap=MemDC.SelectObject(&NewBitmap); //������λͼѡ��MemDC 
	//MemDC.FillSolidRect(rect,pDC->GetBkColor());//��ԭ���������ͻ����������Ǻ�ɫ 
	MemDC.SetMapMode(MM_ANISOTROPIC);//MemDC�Զ�������ϵ
	MemDC.SetWindowExt(rect.Width(),rect.Height());
	MemDC.SetViewportExt(rect.Width(),-rect.Height());
	MemDC.SetViewportOrg(rect.Width()/2,rect.Height()/2);
	CPen PenBlue,*pOldPen;
	PenBlue.CreatePen(PS_SOLID,1,RGB(0,0,255));
	pOldPen=MemDC.SelectObject(&PenBlue);
	MemDC.MoveTo(-10,0);//����ѩ������
	MemDC.LineTo(10,0);
	MemDC.MoveTo(0,-10);
	MemDC.LineTo(0,10);
	MemDC.SelectObject(pOldPen);
	PenBlue.DeleteObject();
	CPen PenWhite;
	PenWhite.CreatePen(PS_SOLID,1,RGB(255,255,255));
	pOldPen=MemDC.SelectObject(&PenWhite);
	n=4;//�ݹ����
	theta=PI/3;//Koch�Ƕ�
	r=200;//�����������Բ�뾶
	//�������ζ�������
	x0=r*sin(RotateAngle*PI/180);
	y0=r*cos(RotateAngle*PI/180);
	x1=r*sin((RotateAngle+120)*PI/180);
	y1=r*cos((RotateAngle+120)*PI/180);
	x2=r*sin((RotateAngle+240)*PI/180);
	y2=r*cos((RotateAngle+240)*PI/180);
	//��������Koch����
	Fractal(x0,y0,x1,y1);//�����ұ�
	Fractal(x1,y1,x2,y2);//���Ƶױ�
	Fractal(x2,y2,x0,y0);//�������
	MemDC.SelectObject(pOldPen);
	PenWhite.DeleteObject();
	pDC->BitBlt(-rect.Width()/2,-rect.Height()/2,rect.Width(),rect.Height(),&MemDC,-rect.Width()/2,-rect.Height()/2,SRCCOPY);//���ڴ�λͼ��������Ļ
	MemDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
	MemDC.DeleteDC();//ɾ��MemDC
	ReleaseDC(pDC);//�ͷ�DC
}

void CTestView::Fractal(double bx,double by,double ex,double ey)//����һ��Koch����
{
    L=sqrt((ex-bx)*(ex-bx)+(ey-by)*(ey-by));//�߶γ���
	d=L/pow(2*(1+cos(theta)),n);//������ÿһ�γ���
	angle=atan((ey-by)/(ex-bx));//������ʼ�Ƕ�
	if(ex<bx)
		angle=angle+PI;//����ױ߷������
	ax=bx,ay=by;
    Koch(angle,n);
}
void CTestView::Koch(double alpha,int n)//�ƺͺ���
{
	if(n==0)
    {
		bx=ax+d*cos(alpha);
		by=ay+d*sin(alpha);
		MemDC.MoveTo(ROUND(ax),ROUND(ay));
		MemDC.LineTo(ROUND(bx),ROUND(by));
		ax=bx;ay=by;
		return;
	}
	Koch(alpha,n-1);
    alpha+=theta;
    Koch(alpha,n-1);
    alpha-=2*theta;
    Koch(alpha,n-1);
    alpha+=theta;
    Koch(alpha,n-1);
}

void CTestView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	RotateAngle+=10;//�趨����
	Invalidate(FALSE);	
	CView::OnTimer(nIDEvent);
}

void CTestView::OnMdraw() 
{
	// TODO: Add your command handler code here
	Play=Play?FALSE:TRUE;
	if (Play)//���ö�ʱ��
		SetTimer(1,50,NULL);	
	else
		KillTimer(1);	
}

void CTestView::OnUpdateMdraw(CCmdUI* pCmdUI) 
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
		pCmdUI->SetText("��ʼ");
	}
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//��ֹ�������� 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}
