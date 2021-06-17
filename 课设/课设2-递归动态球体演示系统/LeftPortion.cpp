// LeftPortion.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "LeftPortion.h"
#include "TestDoc.h"//
#include "math.h"//
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLeftPortion

IMPLEMENT_DYNCREATE(CLeftPortion, CFormView)

CLeftPortion::CLeftPortion()
	: CFormView(CLeftPortion::IDD)
{
	//{{AFX_DATA_INIT(CLeftPortion)
	m_model = -1;
	m_switch = -1;
	//}}AFX_DATA_INIT
}

CLeftPortion::~CLeftPortion()
{
}

void CLeftPortion::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeftPortion)
	DDX_Control(pDX, IDC_SLIDER2, m_facecount);
	DDX_Control(pDX, IDC_CURFACE, m_curface);
	DDX_Control(pDX, IDC_SLIDER8, m_rotateZ);
	DDX_Control(pDX, IDC_SLIDER7, m_rotateY);
	DDX_Control(pDX, IDC_SLIDER6, m_rotateX);
	DDX_Control(pDX, IDC_SLIDER5, m_translateZ);
	DDX_Control(pDX, IDC_SLIDER4, m_translateY);
	DDX_Control(pDX, IDC_SLIDER3, m_translateX);
	DDX_Control(pDX, IDC_SLIDER1, m_radius);
	DDX_Radio(pDX, IDC_FRAME, m_model);
	DDX_Radio(pDX, IDC_OFF, m_switch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeftPortion, CFormView)
	//{{AFX_MSG_MAP(CLeftPortion)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_OFF, OnOff)
	ON_BN_CLICKED(IDC_ON, OnOn)
	ON_BN_CLICKED(IDC_FRAME, OnFrame)
	ON_BN_CLICKED(IDC_SURFACE, OnSurface)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftPortion diagnostics

#ifdef _DEBUG
void CLeftPortion::AssertValid() const
{
	CFormView::AssertValid();
}

void CLeftPortion::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftPortion message handlers

void CLeftPortion::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	//设置左窗格滑动条的范围及初始值
	m_radius.SetRange(50,200,TRUE);
	m_radius.SetPos(150);//球体半径
	m_facecount.SetRange(0,6,TRUE);
	m_facecount.SetPos(4);//面片级数
	m_model=0;//线框消隐
	m_switch=0;//光照开关
	m_translateX.SetRange(0,20,TRUE);
	m_translateX.SetPos(5);//x方向平移
	m_translateY.SetRange(0,20,TRUE);
	m_translateY.SetPos(5);//y方向平移
	m_translateZ.SetRange(0,20,TRUE);	
	m_translateZ.SetPos(5);//z方向平移
	m_rotateX.SetRange(0,20,TRUE);
	m_rotateX.SetPos(3);//x方向旋转
	m_rotateY.SetRange(0,20,TRUE);
	m_rotateY.SetPos(3);//y方向旋转
	m_rotateZ.SetRange(0,20,TRUE);	
	m_rotateZ.SetPos(3);//z方向旋转
	CString str("");
 	str.Format("当前球体面片数为：%d",(int(8*pow(4,m_facecount.GetPos()))));
 	m_curface.SetWindowText(str);
 	UpdateData(FALSE);
}

void CLeftPortion::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
 	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
 	UpdateData();
	pDoc->m_Radius=m_radius.GetPos();
 	pDoc->m_FaceCount=m_facecount.GetPos();
	pDoc->m_TranslateX=m_translateX.GetPos();
 	pDoc->m_TranslateY=m_translateY.GetPos();
	pDoc->m_TranslateZ=m_translateZ.GetPos();
	pDoc->m_RotateX=m_rotateX.GetPos();
	pDoc->m_RotateY=m_rotateY.GetPos();
	pDoc->m_RotateZ=m_rotateZ.GetPos();
 	CString str("");
 	int iFacesCount=int(8*pow((double)4,(double)m_facecount.GetPos()));
 	str.Format("当前球体面片数为：%d",iFacesCount);
 	m_curface.SetWindowText(str);
 	UpdateData(FALSE);
 	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CLeftPortion::OnOff() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->m_Switch=0;
	Invalidate(FALSE);
}

void CLeftPortion::OnOn() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->m_Switch=1;
	Invalidate(FALSE);
}

void CLeftPortion::OnFrame() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
   	pDoc->m_Model=0;
 	Invalidate(FALSE);
}

void CLeftPortion::OnSurface() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
   	pDoc->m_Model=1;
 	Invalidate(FALSE);
}
