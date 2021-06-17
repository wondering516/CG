// TestView.cpp : implementation of the CTestView class
//

#include "stdafx.h"
#include "Test.h"
#include "Line.h"
#include "TestDoc.h"//����CLine��ͷ�ļ�
#include "TestView.h"
#include "MainFrm.h"//����CMainFrameͷ�ļ�������״̬����ʾ�������λ��
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
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_MDRAW, OnMdraw)
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
	m_Flag=FALSE;
	m_LBDown=FALSE;
	m_Attach=FALSE;
	m_MState=FALSE;
	m_IsInsert=FALSE; 
	m_Arrow=TRUE;
}

CTestView::~CTestView()
{	
	int nCount=m_ptrarray.GetSize();
	for (int i=0;i<nCount;i++)
	{
		CPointArray *p=(CPointArray*)m_ptrarray.GetAt(i);
		delete p;//�ͷ�ָ����ָ��Ķ��ڴ�
	}
	m_ptrarray.RemoveAll();//�Ƴ�����Ԫ��
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
void CTestView::DoubleBuffer()//˫����
{
	CRect rect;//����ͻ���
	GetClientRect(&rect);//��ÿͻ����Ĵ�С
	CDC* pDC=GetDC();
	CDC MemDC;//�ڴ��豸������
	CBitmap NewBitmap,*pOldBitmap;//�ڴ��г���ͼ�����ʱλͼ
	MemDC.CreateCompatibleDC(pDC);//��������ĻpDC���ݵ�MemDC 
	NewBitmap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());//��������λͼ 
	pOldBitmap=MemDC.SelectObject(&NewBitmap); //������λͼѡ��MemDC 
	MemDC.FillSolidRect(rect,pDC->GetBkColor());//��ԭ���������ͻ����������Ǻ�ɫ 
	DrawObject(&MemDC);
	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&MemDC,0,0,SRCCOPY);//���ڴ�λͼ��������Ļ
	MemDC.SelectObject(pOldBitmap);//�ָ�λͼ
	NewBitmap.DeleteObject();//ɾ��λͼ
	MemDC.DeleteDC();//ɾ��MemDC
	ReleaseDC(pDC);//�ͷ�DC
}

void CTestView::DrawObject(CDC *pDC)//���ƶ����
{
	int index=m_ptrarray.GetSize();
	CLine *line=new CLine;
	if(index)
	{		
		line->MoveTo(pDC,((CPointArray*)m_ptrarray.GetAt(0))->pt);
		for(int i=1;i<index;i++)
		{
			line->LineTo(pDC,((CPointArray*)m_ptrarray.GetAt(i))->pt);		
		}
		if(FALSE==m_Flag)//�߶αպϣ����ͼ��
		{
			FillPolygon(pDC);	
		}
	}
	delete line;
}

void CTestView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(TRUE==m_Flag)//��ͼ״̬
	{
		m_LBDown=TRUE; 
		CPointArray *pPointArray=new CPointArray(point);
	    m_ptrarray.Add(pPointArray);//����¶���
	}
	m_IsInsert=FALSE;
	CView::OnLButtonDown(nFlags, point);
}

void CTestView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_Arrow)
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	else
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	CString strx,stry;//״̬����ʾ���λ��
	CMainFrame *pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;//Ҫ�����MainFrm.hͷ�ļ�
	CStatusBar *pStatus=&pFrame->m_wndStatusBar;//��Ҫ��m_wndStatusBar�����޸�Ϊ����
	if(pStatus)
	{
		strx.Format("x=%d",point.x);
		stry.Format("y=%d",point.y);
		CDC *pDC=GetDC();
		CSize sizex=pDC->GetTextExtent(strx);
		CSize sizey=pDC->GetTextExtent(stry);
		pStatus->SetPaneInfo(1,ID_INDICATOR_X,SBPS_NORMAL,sizex.cx);//�ı�״̬�����
		pStatus->SetPaneText(1,strx);
		pStatus->SetPaneInfo(2,ID_INDICATOR_Y,SBPS_NORMAL,sizey.cx);//�ı�״̬�����
		pStatus->SetPaneText(2,stry);
		ReleaseDC(pDC);
	}
	int index=m_ptrarray.GetSize()-1;
	if(m_LBDown)
	{
		if(!m_IsInsert)//����ǵ�һ���ƶ���������µĶ���
		{
			CPointArray *pPointArray=new CPointArray(point);
			m_ptrarray.Add(pPointArray);
			m_IsInsert=TRUE;
		}
		else//�޸��ϴβ���Ķ�������
		{  			
			((CPointArray *)m_ptrarray.GetAt(index))->pt=point;			
		}		
	}
	if(m_LBDown)
	{
		if(MK_SHIFT==nFlags)//Լ��:���԰�����Shift��
		{
			CPoint* pt1=&(((CPointArray *)m_ptrarray.GetAt(index))->pt);
			CPoint* pt2=&(((CPointArray *)m_ptrarray.GetAt(index-1))->pt);
			if(abs(pt1->x-pt2->x)>=abs(pt1->y-pt2->y))
			{
				pt1->y=pt2->y;//x����Ĵ���
			}
			else
			{
				pt1->x=pt2->x;//y����Ĵ���
			}
		}
	}
	if(index>3)
	{
		CPoint pt=((CPointArray*)m_ptrarray.GetAt(0))->pt;
		if((abs(point.x-pt.x)<=5) && (abs(point.y-pt.y)<=5))//������:�߳�Ϊ10��������
		{
			((CPointArray *)m_ptrarray.GetAt(index))->pt=pt;//�޸�����
			m_Arrow=TRUE;
			m_LBDown=FALSE;
			m_MState=TRUE;
			m_Flag=FALSE;
		}
	}
	Invalidate(FALSE);
	CView::OnMouseMove(nFlags, point);
}

BOOL CTestView::OnEraseBkgnd(CDC* pDC)//��ֹ����ˢ�� 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}

void CTestView::OnMdraw() 
{
	// TODO: Add your command handler code here
	m_Arrow=FALSE;//����ж�
	m_Flag=TRUE;//��ͼ״̬
	m_MState=FALSE;//Shift������
	for (int i = 0; i<m_ptrarray.GetSize(); i++)
	{
		CPointArray* p = (CPointArray*)m_ptrarray.GetAt(i);
		delete p;
	}
	m_ptrarray.RemoveAll();//�������
	if(FALSE==m_Attach)
	{
		MessageBox("��ʹ���������Ļ�ϻ��ƶ����","��ʾ", MB_ICONINFORMATION);
		m_Attach=TRUE;	
	}
	Invalidate(FALSE);	
}

void CTestView::FillPolygon(CDC *pDC) 
{
	// TODO: Add your command handler code here
    int size=m_ptrarray.GetSize();
	CPoint *p=new CPoint[size];//�����ڴ�ռ�
	for(int i=0;i<size;i++)//�������ݵ�һ����̬����
	{
		p[i]=((CPointArray *)m_ptrarray.GetAt(i))->pt;
	}
	CFill *fill=new CFill;//��̬�����ڴ�
	fill->SetPoint(p,size);//���ö���ζ�������
	fill->CreateBucket();//����Ͱ��
	fill->CreateEdge();//�����߱�
	fill->FillPolygon(pDC);//�������
	delete fill;//�ͷ��ڴ�
	delete []p;	
}
