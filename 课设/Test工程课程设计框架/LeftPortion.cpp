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
	//}}AFX_DATA_INIT
}

CLeftPortion::~CLeftPortion()
{
}

void CLeftPortion::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLeftPortion)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeftPortion, CFormView)
	//{{AFX_MSG_MAP(CLeftPortion)
	ON_WM_HSCROLL()
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
	
 	UpdateData(FALSE);
}

void CLeftPortion::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
 	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
 	UpdateData();
	
 	UpdateData(FALSE);
 	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}




