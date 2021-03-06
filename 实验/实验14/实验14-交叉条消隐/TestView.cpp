// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
#define ROUND(a) int(a+0.5)
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
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MZBUFFER, OnMzbuffer)
	ON_UPDATE_COMMAND_UI(ID_MZBUFFER, OnUpdateMzbuffer)
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

void CTestView::ReadPointDeep()//????????????
{
	a=60;d=100;
	int i;	
	//??????????????????(x,y,z)????????????
	P[0][0].x=-3*a;P[0][0].y=+2*a;P[0][0].z=+d;
	P[0][1].x=-3*a;P[0][1].y=  +a;P[0][1].z=+d;
	P[0][2].x=+3*a;P[0][2].y=  +a;P[0][2].z=-d;
	P[0][3].x=+3*a;P[0][3].y=+2*a;P[0][3].z=-d;
	for(i=0;i<4;i++)//????????????
	{
		P[0][i].c=CRGB(1.0,0.0,0.0);//????????????
	}
	//??????????????????(x,y,z),??????????
	P[1][0].x=-3*a;P[1][0].y=  -a;P[1][0].z=-d;
	P[1][1].x=-3*a;P[1][1].y=-2*a;P[1][1].z=-d;
	P[1][2].x=+3*a;P[1][2].y=-2*a;P[1][2].z=+d;
	P[1][3].x=+3*a;P[1][3].y=  -a;P[1][3].z=+d;
	for(i=0;i<4;i++)
	{
		P[1][i].c=CRGB(1.0,1.0,0.0);//????????????
	}
	//??????????????????(x,y,z),??????????
	P[2][0].x=-2*a;P[2][0].y=+3*a;P[2][0].z=-d;
	P[2][1].x=-2*a;P[2][1].y=-3*a;P[2][1].z=+d;
	P[2][2].x=  -a;P[2][2].y=-3*a;P[2][2].z=+d;
	P[2][3].x=  -a;P[2][3].y=+3*a;P[2][3].z=-d;
	for(i=0;i<4;i++)
	{
		P[2][i].c=CRGB(0.0,1.0,0.0);//????????????
	}	
	//??????????????????(x,y,z)????????????
	P[3][0].x=  +a;P[3][0].y=+3*a;P[3][0].z=+d;
	P[3][1].x=  +a;P[3][1].y=-3*a;P[3][1].z=-d;
	P[3][2].x=+2*a;P[3][2].y=-3*a;P[3][2].z=-d;
	P[3][3].x=+2*a;P[3][3].y=+3*a;P[3][3].z=+d;
	for(i=0;i<4;i++)
	{
		P[3][i].c=CRGB(0.0,0.0,1.0);//????????????
	}
}

void CTestView::ReadPointSort()//????????????
{
	a=60;d=100;
	int i;	
	//??????????????????(x,y,z)????????????
	P[0][0].x=-3*a;P[0][0].y=+2*a;P[0][0].z=-d;
	P[0][1].x=-3*a;P[0][1].y=  +a;P[0][1].z=-d;
	P[0][2].x=+3*a;P[0][2].y=  +a;P[0][2].z=-d;
	P[0][3].x=+3*a;P[0][3].y=+2*a;P[0][3].z=-d;
	for(i=0;i<4;i++)//????????????
	{
		P[0][i].c=CRGB(1.0,0.0,0.0);//????????????
	}
	//??????????????????(x,y,z),??????????
	P[1][0].x=-3*a;P[1][0].y=  -a;P[1][0].z=+d;
	P[1][1].x=-3*a;P[1][1].y=-2*a;P[1][1].z=+d;
	P[1][2].x=+3*a;P[1][2].y=-2*a;P[1][2].z=+d;
	P[1][3].x=+3*a;P[1][3].y=  -a;P[1][3].z=+d;
	for(i=0;i<4;i++)
	{
		P[1][i].c=CRGB(1.0,1.0,0.0);//????????????
	}
	//??????????????????(x,y,z),??????????
	P[2][0].x=-2*a;P[2][0].y=+3*a;P[2][0].z=-2*d;
	P[2][1].x=-2*a;P[2][1].y=-3*a;P[2][1].z=-2*d;
	P[2][2].x=  -a;P[2][2].y=-3*a;P[2][2].z=-2*d;
	P[2][3].x=  -a;P[2][3].y=+3*a;P[2][3].z=-2*d;
	for(i=0;i<4;i++)
	{
		P[2][i].c=CRGB(0.0,1.0,0.0);//????????????
	}	
	//??????????????????(x,y,z)????????????
	P[3][0].x=  +a;P[3][0].y=+3*a;P[3][0].z=+2*d;
	P[3][1].x=  +a;P[3][1].y=-3*a;P[3][1].z=+2*d;
	P[3][2].x=+2*a;P[3][2].y=-3*a;P[3][2].z=+2*d;
	P[3][3].x=+2*a;P[3][3].y=+3*a;P[3][3].z=+2*d;
	for(i=0;i<4;i++)
	{
		P[3][i].c=CRGB(0.0,0.0,1.0);//????????????
	}
}

void CTestView::DoubleBuffer()//??????????
{	
	CRect rect;
	GetClientRect(&rect);
	CDC *pDC=GetDC();
	pDC->SetMapMode(MM_ANISOTROPIC);//????????????
	pDC->SetWindowExt(rect.Width(),rect.Height());
	pDC->SetViewportExt(rect.Width(),-rect.Height());//x????????????y??????????
	pDC->SetViewportOrg(rect.Width()/2,rect.Height()/2);//??????????????
	if(Play)
	{
		ReadPointSort();
		DrawObject1(pDC);//????????????
	}
	else
	{
		ReadPointDeep();
		DrawObject2(pDC);//????????????	
	}
}

void CTestView::DrawObject1(CDC *pDC)//??????????????????????
{
	CPi2 Point[4];
	SortDeep();
	for(int nBar=0;nBar<4;nBar++)
	{	
		for(int nPoint=0;nPoint<4;nPoint++)//????????
		{
			Point[nPoint].x=P[nBar][nPoint].x;
			Point[nPoint].y=ROUND(P[nBar][nPoint].y);
			Point[nPoint].c=P[nBar][nPoint].c;
		}
		CFill *fill=new CFill;//????????
		fill->SetPoint(Point,4);//????????		
		fill->CreateBucket();//????????
		fill->CreateEdge();//????????
		fill->Gouraud(pDC);//??????????
		delete fill;//????????
	}
	
}

void CTestView::DrawObject2(CDC *pDC)//??????????????????????
{
	CPi3 Point[4];
	CZBuffer *zbuf=new CZBuffer;//????????
	zbuf->InitDeepBuffer(800,800,-1000);//??????????
	for(int nBar=0;nBar<4;nBar++)
	{	
		for(int nPoint=0;nPoint<4;nPoint++)//????????
		{
			Point[nPoint].x=P[nBar][nPoint].x;
			Point[nPoint].y=ROUND(P[nBar][nPoint].y);
			Point[nPoint].z=P[nBar][nPoint].z;
			Point[nPoint].c=P[nBar][nPoint].c;
		}
		zbuf->SetPoint(Point,4);//????????			
		zbuf->CreateBucket();//????????
		zbuf->CreateEdge();//????????
		zbuf->Gouraud(pDC);//??????????	
		zbuf->ClearMemory();//????????
	}
	delete zbuf;//????????
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//???????????????? 
{
	// TODO: Add your message handler code here and/or call default
	CRect rect;   
	pDC->GetClipBox(&rect);   
	pDC->FillSolidRect(rect,RGB(0,0,0));
	return TRUE;
}

void CTestView::OnMzbuffer()//???????????? 
{
	// TODO: Add your command handler code here
	Play=Play?FALSE:TRUE;
	if(Play)
		ReadPointSort();
	else
		ReadPointDeep();
	Invalidate(FALSE);	
}

void CTestView::OnUpdateMzbuffer(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(Play)
	{
		pCmdUI->SetCheck(TRUE);
		pCmdUI->SetText("????????????");
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
		pCmdUI->SetText("????????????");
	}		
}
void CTestView::SortDeep()//????????
{
	CP3 t;
	for(int j=1;j<4;j++)//????????
		for(int i=1;i<=4-j;i++)
		{
			for(int k=0;k<4;k++)
			{
				if(P[i-1][k].z>P[i][k].z)
				{
					t=P[i-1][k];P[i-1][k]=P[i][k];P[i][k]=t;					
				}
			}
		}		
}