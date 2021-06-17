// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#include "math.h"//��ѧͷ�ļ�
#define PI 3.1415926//PI�ĺ궨��
#define ROUND(a) int(a+0.5)//��������
#include "InputDlg.h"//�����Ի���ͷ�ļ�
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
	ON_COMMAND(IDM_DIAMOND, OnDiamond)
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
	p=NULL;
}

CTestView::~CTestView()
{
	if(p!=NULL)
	{
		delete[] p;//�ͷ�һά���������ڴ�ռ�
		p=NULL;
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
void CTestView::Diamond()//���ʯ����
{	
    CDC *pDC=GetDC();//����豸������
	CRect rect;//�������
	GetClientRect(&rect);//��þ��οͻ�����С
	pDC->SetMapMode(MM_ANISOTROPIC);//�Զ�������ϵ
	pDC->SetWindowExt(rect.Width(),rect.Height());//���ô��ڷ�Χ
	pDC->SetViewportExt(rect.Width(),-rect.Height());//x��ˮƽ���ң�y��Ǧֱ����
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//��Ļ����Ϊ����ϵԭ��
	CPen NewPen,*pOldPen;//���廭��
	NewPen.CreatePen(PS_SOLID,1,RGB(0,0,255));//������ɫ����
	pOldPen=pDC->SelectObject(&NewPen);//����ɫ����ѡ���豸������
	double thta;//thtaΪԲ�ĵȷֽ�
	thta=2*PI/n;
	for(int i=0;i<n;i++)//����ȷֵ�����
	{
		p[i].x=r*cos(i*thta);
		p[i].y=r*sin(i*thta);
	}
	for(i=0;i<=n-2;i++)//���θ����ӵȷֵ�
	{
		for(int j=i+1;j<=n-1;j++)
		{
			pDC->MoveTo(ROUND(p[i].x),ROUND(p[i].y));
			pDC->LineTo(ROUND(p[j].x),ROUND(p[j].y));
		}
	}
	pDC->SelectObject(pOldPen);//�ָ��豸������ԭ����
	NewPen.DeleteObject();//ɾ���ѳ�����״̬���»��� 
	ReleaseDC(pDC);//�ͷ�pDC
}

void CTestView::OnDiamond() 
{
	// TODO: Add your command handler code here
	CInputDlg dlg;
	if(IDOK==dlg.DoModal())//���öԻ���ģ�飬�ж��Ƿ񵥻�OK��ť
	{
		n=dlg.m_n;//nΪ�ȷֵ����
		r=dlg.m_r;//rΪԲ�İ뾶
	}
	else
	{
		return;
	}
	RedrawWindow();//�ػ洰��
	p=new CP2[n];//������̬һά�����ڴ�ռ�,n���������ͱ��� 
	Diamond();	
}
