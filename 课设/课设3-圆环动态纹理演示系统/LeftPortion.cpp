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
	m_Model = -1;
	m_Render = -1;
	m_Switch = -1;
	//}}AFX_DATA_INIT
}

CLeftPortion::~CLeftPortion()
{
}

void CLeftPortion::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeftPortion)
	DDX_Control(pDX, IDC_SHOWTEX, m_ShowTex);
	DDX_Control(pDX, IDC_CURRENTFACES, m_CurrentFaces);
	DDX_Control(pDX, IDC_NUMR2, m_NumR2);
	DDX_Control(pDX, IDC_NUMR1, m_NumR1);
	DDX_Control(pDX, IDC_R2, m_R2);
	DDX_Control(pDX, IDC_R1, m_R1);
	DDX_Radio(pDX, IDC_FRAME, m_Model);
	DDX_Radio(pDX, IDC_GOURAUD, m_Render);
	DDX_Radio(pDX, IDC_OFF, m_Switch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeftPortion, CFormView)
	//{{AFX_MSG_MAP(CLeftPortion)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_FRAME, OnFrame)
	ON_BN_CLICKED(IDC_SURFACE, OnSurface)
	ON_BN_CLICKED(IDC_TEXTURE, OnTexture)
	ON_BN_CLICKED(IDC_OFF, OnOff)
	ON_BN_CLICKED(IDC_ON, OnOn)
	ON_BN_CLICKED(IDC_PHONG, OnPhong)
	ON_BN_CLICKED(IDC_GOURAUD, OnGouraud)
	ON_BN_CLICKED(IDC_SELTEX, OnSeltex)
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
	//�����󴰸񻬶����ķ�Χ����ʼֵ
	m_R1.SetRange(100,300,TRUE);
	m_R1.SetPos(200);
	m_R1.SetTicFreq(50);
	m_R1.SetPageSize(50);
	m_R2.SetRange(20,120,TRUE);
	m_R2.SetPos(80);
	m_R2.SetTicFreq(20);
	m_R2.SetPageSize(20);
	m_NumR1.SetRange(6,36,TRUE);
	m_NumR1.SetPos(18);
	m_NumR1.SetTicFreq(6);
	m_NumR1.SetPageSize(6);
	m_NumR2.SetRange(6,36,TRUE);
	m_NumR2.SetPos(18);
	m_NumR2.SetTicFreq(6);
	m_NumR2.SetPageSize(6);
	m_Model=0;//ģ�ͷ���Ϊ���߿�ģ�ͣ��˴��뽫һ����ѡ��ť��group����ѡ��
	m_Switch=1;//��Դ���أ���
	m_Render=0;//��������ģʽΪGouraud
	GetDlgItem(IDC_GOURAUD)->EnableWindow(FALSE);//����Gouraud��ɫģ��
	GetDlgItem(IDC_PHONG)->EnableWindow(FALSE);//����Phong��ɫģ��
	GetDlgItem(IDC_SELTEX)->EnableWindow(FALSE);//����ѡ������ť
	CString str("");
	str.Format("��Ƭ������%d",m_NumR1.GetPos()*m_NumR2.GetPos());
    m_CurrentFaces.SetWindowText(str);
 	UpdateData(FALSE);
}

void CLeftPortion::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	UpdateData();
	pDoc->m_R1=m_R1.GetPos();
	pDoc->m_R2=m_R2.GetPos();
	pDoc->m_NumR1=m_NumR1.GetPos();
	pDoc->m_NumR2=m_NumR2.GetPos();
	CString str("");
	int FacesCount=m_NumR1.GetPos()*m_NumR2.GetPos();
	str.Format("��Ƭ������%d",FacesCount);
	m_CurrentFaces.SetWindowText(str);
	UpdateData(FALSE);
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CLeftPortion::OnFrame() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_GOURAUD)->EnableWindow(FALSE);//����Gouraud��ɫģ��
	GetDlgItem(IDC_PHONG)->EnableWindow(FALSE);//����Phong��ɫģ��
	GetDlgItem(IDC_SELTEX)->EnableWindow(FALSE);//����ѡ������ť
	m_Model=0;
   	UpdateData(FALSE);
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->m_Model=m_Model;
}

void CLeftPortion::OnSurface() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_GOURAUD)->EnableWindow(TRUE);//����Gouraud��ɫģ��
	GetDlgItem(IDC_PHONG)->EnableWindow(TRUE);//����Phong��ɫģ��
	GetDlgItem(IDC_SELTEX)->EnableWindow(FALSE);//����ѡ������ť
	m_Model=1;
	UpdateData(FALSE);
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->m_Model=m_Model;
}

void CLeftPortion::OnTexture() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_SELTEX)->EnableWindow(TRUE);//����ѡ������ť
	m_Model=2;//ģ�ͷ���Ϊ������ģ��
	m_Render=1;//��������ģʽΪGouraud
	UpdateData(FALSE);
	GetDlgItem(IDC_GOURAUD)->EnableWindow(FALSE);//����Gouraud��ɫģ��
	GetDlgItem(IDC_PHONG)->EnableWindow(TRUE);//����Phong��ɫģ��
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->m_Model=m_Model;
	pDoc->m_Render=m_Render;	
}

void CLeftPortion::OnOff() 
{
	// TODO: Add your control notification handler code here
	m_Switch=0;
	UpdateData(FALSE);
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->m_Switch=m_Switch;	
}

void CLeftPortion::OnOn() 
{
	// TODO: Add your control notification handler code here
	m_Switch=1;
	UpdateData(FALSE);
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->m_Switch=m_Switch;	
}

void CLeftPortion::OnGouraud() 
{
	// TODO: Add your control notification handler code here
	m_Render=0;
	UpdateData(FALSE);
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->m_Render=m_Render;
}

void CLeftPortion::OnPhong() 
{
	// TODO: Add your control notification handler code here
	m_Render=1;
	UpdateData(FALSE);
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	pDoc->m_Render=m_Render;
}

void CLeftPortion::OnSeltex() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	CFileDialog  file(TRUE);
	CString PicName;
	file.m_ofn.lpstrInitialDir=".\\Data";//Ĭ��Ϊ��Ŀ¼��Data�ļ���
	file.m_ofn.lpstrFilter="λͼ�ļ�(*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0\0";//λͼ����
	if(file.DoModal()==IDOK)
	{
		pDoc->m_PathName=file.GetPathName();
		PicName=file.GetFileName(); 		
	}
	else
		return;
	CDC *pDC=GetDC();
	HBITMAP *hBitmap=(HBITMAP*)LoadImage(::AfxGetInstanceHandle(),PicName.GetBuffer(0),IMAGE_BITMAP,0,0,LR_LOADFROMFILE);//��ü���λͼ�ľ��					            				       
	CDC MemDC;//�ڴ��豸������
	CRect rectPic;//ʾ��λͼ�ľ�������
	m_ShowTex.GetWindowRect(&rectPic);//��ô�С
	if(!MemDC.m_hDC)
		MemDC.CreateCompatibleDC(pDC);//������pDC����ݵ�MemDC
	MemDC.SelectObject(hBitmap);//ѡ��λͼ
	ScreenToClient(&rectPic);//ӳ�䵽��Ļ��������
	pDC->BitBlt(rectPic.left,rectPic.top,rectPic.Width(),rectPic.Height(),&MemDC,0,0,SRCCOPY);//ʾ��λͼ
	MemDC.DeleteDC();
	ReleaseDC(pDC);
}

