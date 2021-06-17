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
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
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
	this->vpsize=CVPixel::VPSize;
	this->rect=CVPixel::rect;
	nType=0;
	LBdown=FALSE;
	IsAnti=FALSE;
	PStart=CP2(-1.0,-1.0,CRGB(0.0,0.0,0.0));
	PEnd=CP2(-1.0,-1.0,CRGB(0.0,0.0,0.0));
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
	CRect Rect;//����ͻ���
	GetClientRect(&Rect);//��ÿͻ����Ĵ�С
	CDC MemDC;//�ڴ�DC
	CBitmap NewBitmap,*pOldBitmap;//�ڴ��г���ͼ�����ʱλͼ
	MemDC.CreateCompatibleDC(pDC);//��������ĻpDC���ݵ�MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC,Rect.Width(),Rect.Height());//��������λͼ 
	pOldBitmap=MemDC.SelectObject(&NewBitmap); //������λͼѡ��MemDC
	MemDC.FillSolidRect(Rect,pDC->GetBkColor());//��ԭ���������ͻ����������Ǻ�ɫ
	DrawObject(&MemDC);
	pDC->BitBlt(0,0,Rect.Width(),Rect.Height(),&MemDC,0,0,SRCCOPY);//���ڴ�λͼ��������Ļ
	MemDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
	MemDC.DeleteDC();//ɾ��MemDC
	ReleaseDC(pDC);//�ͷ�DC
}

void CTestView::DrawGrid(CDC* pDC)//��������
{
	VirtualPixel.DrawGrid(pDC);
}

void CTestView::DrawObject(CDC* pDC)//����ͼ�κ���
{
	CTestDoc* pDoc=GetDocument();
	nType=pDoc->m_nType;
	IsAnti=pDoc->m_Alias;
	PStart.c=pDoc->m_SPColor;
	PEnd.c=pDoc->m_EPColor;
	switch(nType)
	{
	case 0:
		DrawGrid(pDC);//��������
		break;
	case 1:
		DrawLine(pDC);
		break;
	case 2:
		DrawEllipse(pDC);
		break;
	}	
}

void CTestView::DrawLine(CDC* pDC)//����ֱ��
{
	if(IsAnti)//������ֱ��
	{
		aline.DrawGrid(pDC);
		aline.MoveTo(pDC,PStart);
		aline.LineTo(pDC,PEnd);
	}
	else//����ֱ��
	{
		line.DrawGrid(pDC);
		line.MoveTo(pDC,PStart);
		line.LineTo(pDC,PEnd);
	}
}

void CTestView::DrawEllipse(CDC* pDC) //������Բ
{
	if(IsAnti)//��������Բ
	{
		aellipse.DrawGrid(pDC);
		aellipse.DrawAEllipse(pDC,PStart,PEnd);
	}
	else//������Բ
	{
		ellipse.DrawGrid(pDC);
		ellipse.DrawEllipse(pDC,PStart,PEnd);
	}
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	LBdown=TRUE;	
	PStart.x=(point.x-rect.left)/vpsize;
	PStart.y=(point.y-rect.top)/vpsize;
	PEnd=PStart;	
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(LBdown==TRUE)
	{
		LBdown=FALSE;
	}
	CView::OnLButtonUp(nFlags, point);
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString strx,stry;///״̬��Ϣ
    CMainFrame *pFrame=(CMainFrame*)AfxGetMainWnd();//��ô���ָ��
	CStatusBar *pstatus=&pFrame->m_wndStatusBar;//���״̬����ָ��
	if( point.x >= rect.left && point.x < rect.right && point.y >=rect.top && point.y<rect.bottom )	
	{
		if(pstatus)
		{	
			strx.Format("x=%d",(point.x-rect.left)/vpsize);
			stry.Format("y=%d",(point.y-rect.top)/vpsize);
			CClientDC dc(this);
			CSize sizex=dc.GetTextExtent(strx);
			CSize sizey=dc.GetTextExtent(stry);
			pstatus->SetPaneInfo(1,ID_INDICATOR_X,SBPS_NORMAL,sizex.cx);//��̬�ı�x������ʾ����Ŀ��
			pstatus->SetPaneText(1,strx);
			pstatus->SetPaneInfo(2,ID_INDICATOR_Y,SBPS_NORMAL,sizey.cx);//��̬�ı�y������ʾ����Ŀ��
			pstatus->SetPaneText(2,stry);				
		}
		if(LBdown==TRUE && nType!=0)//���������²����ڻ�ͼ״̬
		{
			PEnd.x=(point.x-rect.left)/vpsize;//�������������ֹ��
			PEnd.y=(point.y-rect.top)/vpsize;
			double dx=(PEnd.x-PStart.x);//ƫ��ʸ��
			double dy=(PEnd.y-PStart.y);
			if(nType==1)
			{
				if(GetKeyState(VK_SHIFT) & 0x8000)//����ֱ��ʱ������SHIFT��
				{
					if(fabs(dx)>=fabs(dy))
					{
						PEnd.y=PStart.y;//x����Ĵ���
					}
					else
					{
						PEnd.x=PStart.x;//y����Ĵ���
					}
				}
			}
			if(nType==2)
			{
				if(GetKeyState(VK_SHIFT) & 0x8000)//������Բʱ���Ƿ���SHIFT��
				{
					if(fabs(dx)<fabs(dy))//��dy����Ϊ������|dx|ΪԲ��ֱ��
					{
						PEnd.y=PStart.y+sign(dy)*fabs(dx);//sign(dy)������������fabs(dx)Ϊ����
					}
					else
					{
						PEnd.x=PStart.x+sign(dx)*fabs(dy);
					}
				}
			}
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));//ʮ�ֹ��	
		}
	}
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));//����ϵͳ���	
	Invalidate(FALSE);//�ػ棬������������
	CView::OnMouseMove(nFlags, point);	
}

int CTestView::sign(double n)//���ź���
{
	if(n<0)
		return -1;
	if(n>0)
		return 1;
	else 
		return 0;
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//��ֹ����ˢ�� 
{
	// TODO: Add your message handler code here and/or call default
	return  TRUE;
}

