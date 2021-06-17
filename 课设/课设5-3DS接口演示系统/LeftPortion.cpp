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
	//}}AFX_DATA_INIT
}

CLeftPortion::~CLeftPortion()
{
}

void CLeftPortion::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeftPortion)
	DDX_Radio(pDX, IDC_FRAME, m_model);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeftPortion, CFormView)
	//{{AFX_MSG_MAP(CLeftPortion)
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
	m_model=1;//表面模型
	UpdateData(FALSE);
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
