// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "TestDoc.h"
#include "TestView.h"
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
ON_COMMAND(ID_MENU_B, OnMenuB)
ON_WM_MOUSEMOVE()
ON_WM_LBUTTONDOWN()
ON_WM_LBUTTONUP()
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
	m_AbleToLeftBtn=FALSE;
	m_AbleToMove=FALSE;
	m_i=-1;
	//9������ĳ�ʼ��
	P[0]=CPoint(104,330);
	P[1]=CPoint(204,231);
	P[2]=CPoint(286,362);
	P[3]=CPoint(363,145);
	P[4]=CPoint(472,527);
	P[5]=CPoint(548,228);
	P[6]=CPoint(662,40);
	P[7]=CPoint(830,450);
	P[8]=CPoint(930,350);
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
	CRect rect;//����ͻ���
	GetClientRect(&rect);//��ÿͻ����Ĵ�С
	CDC MemDC;//�ڴ��豸������
	CBitmap NewBitmap,*pOldBitmap;//�ڴ��г���ͼ�����ʱλͼ
	MemDC.CreateCompatibleDC(pDC);//��������ĻpDC���ݵ�MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//��������λͼ 
	pOldBitmap=MemDC.SelectObject(&NewBitmap); //������λͼѡ��MemDC 
	MemDC.FillSolidRect(rect,pDC->GetBkColor());//��ԭ���������ͻ����������Ǻ�ɫ	
	CPen NewPen3,*pOldPen3;
	NewPen3.CreatePen(PS_SOLID,3,RGB(0,0,0));//���ƿ��ƶ����
	pOldPen3=MemDC.SelectObject(&NewPen3);//ѡ�뻭��	
	MemDC.MoveTo(P[0]);
	MemDC.Rectangle(P[0].x-2,P[0].y-2,P[0].x+2,P[0].y+2);//���ƿ��ƶ���ζ���
	for(int i=1;i<9;i++)
	{
		MemDC.LineTo(P[i]);
		MemDC.Rectangle(P[i].x-2,P[i].y-2,P[i].x+2,P[i].y+2);
	}
	if(m_i!=-1)
	{
		CString	str;
		str.Format("x=%d,y=%d",P[m_i].x,P[m_i].y);
		MemDC.TextOut(P[m_i].x+10,P[m_i].y+10,str);//������Ƶ��������Ϣ
	}
	B3Curves(P,&MemDC);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY);//���ڴ�λͼ��������Ļ
	MemDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
	MemDC.SelectObject(pOldPen3);//�ָ�����
	NewPen3.DeleteObject();//ɾ������	
	MemDC.DeleteDC();//ɾ��MemDC	
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
void CTestView::B3Curves(CPoint p[],CDC *pDC)//��������B��������
{
	double delt=1.0/10;//����
	CPoint PStart,PEnd;//ÿ��B�������ߵ������յ�
	double F03,F13,F23,F33;//B����������
	PStart.x=long((p[0].x+4.0*p[1].x+p[2].x)/6.0);//t��0�����x����
	PStart.y=long((p[0].y+4.0*p[1].y+p[2].y)/6.0);//t��0�����y����
	pDC->MoveTo(PStart);
	CPen NewPen1(PS_SOLID,1,RGB(0,0,255));//����B��������
	CPen *pOldPen1=pDC->SelectObject(&NewPen1);
	for(int i=1;i<7;i++)//6����������
	{
		for(double t=0;t<=1;t+=delt)
		{
			F03=(-t*t*t+3*t*t-3*t+1)/6;  //����F0,3(t)
			F13=(3*t*t*t-6*t*t+4)/6;     //����F1,3(t)
			F23=(-3*t*t*t+3*t*t+3*t+1)/6;//����F2,3(t)
			F33=t*t*t/6;                 //����B3,3(t)
			PEnd.x=long(p[i-1].x*F03+p[i].x*F13+p[i+1].x*F23+p[i+2].x*F33);
			PEnd.y=long(p[i-1].y*F03+p[i].y*F13+p[i+1].y*F23+p[i+2].y*F33);
			pDC->LineTo(PEnd);
		}
	}
	pDC->SelectObject(pOldPen1);
	NewPen1.DeleteObject();	
}

void CTestView::OnMenuB()//�˵�����
{
	// TODO: Add your command handler code here
	MessageBox("���϶����ƶ���εĶ��㣡","��Ϣ",MB_ICONINFORMATION|MB_OK);		
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point)//����ƶ����� 
{
	// TODO: Add your message handler code here and/or call default
	if(TRUE==m_AbleToMove)
	{
		P[m_i]=point;	
	}
	m_i=-1;
	int i;
	for(i=0;i<9;i++)
	{
		if((point.x-P[i].x)*(point.x-P[i].x)+(point.y-P[i].y)*(point.y-P[i].y)<25)
		{
			m_i=i;
			m_AbleToLeftBtn=TRUE;
			m_Cursor=AfxGetApp()->LoadCursor(IDC_CURSOR1);//��Ϊ���ι��
			SetCursor(m_Cursor);
			break;
		}
	}
	if(10==i)
	{
		m_i=-1;
	}
	Invalidate(FALSE);	
	CView::OnMouseMove(nFlags, point);
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) //���������º���
{
	// TODO: Add your message handler code here and/or call default
	if(TRUE==m_AbleToLeftBtn)
		m_AbleToMove=TRUE;
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnLButtonUp(UINT nFlags, CPoint point) //������������ 
{
	// TODO: Add your message handler code here and/or call default
	m_AbleToLeftBtn=FALSE;
	m_AbleToMove=FALSE;
	m_i=0;
	CView::OnLButtonUp(nFlags, point);
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//��ֹ����ˢ�� 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}
