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
	ON_BN_CLICKED(IDC_BLUELIGHT, OnBluelight)
	ON_BN_CLICKED(IDC_GREENLIGHT, OnGreenlight)
	ON_BN_CLICKED(IDC_REDLIGHT, OnRedlight)
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
	UINT nCheckIDs[3]={IDC_REDLIGHT,IDC_GREENLIGHT,IDC_BLUELIGHT};
	CButton* pBtn;
	for (int i=0;i<3;i++)
	{
		pBtn=(CButton*)GetDlgItem(nCheckIDs[i]);
		pBtn->SetCheck(1);
	}	
	UpdateData(FALSE);
}

void CLeftPortion::OnRedlight() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	CButton* pBtn=(CButton*)GetDlgItem(IDC_REDLIGHT);
	if(pBtn->GetCheck())
		pDoc->m_RedLight=1;	
	else
		pDoc->m_RedLight=0;	
	Invalidate(FALSE);
}

void CLeftPortion::OnGreenlight() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	CButton* pBtn=(CButton*)GetDlgItem(IDC_GREENLIGHT);
	if(pBtn->GetCheck())
		pDoc->m_GreenLight=1;	
	else
		pDoc->m_GreenLight=0;	
	Invalidate(FALSE);
}

void CLeftPortion::OnBluelight() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	CButton* pBtn=(CButton*)GetDlgItem(IDC_BLUELIGHT);
	if(pBtn->GetCheck())
		pDoc->m_BlueLight=1;	
	else
		pDoc->m_BlueLight=0;	
	Invalidate(FALSE);
}




