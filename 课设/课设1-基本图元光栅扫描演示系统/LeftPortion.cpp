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
	DDX_Control(pDX, IDC_SPCOLOR, m_SPColor);
	DDX_Control(pDX, IDC_EPCOLOR, m_EPColor);
	DDX_Control(pDX, IDC_COLOR8, m_Color8);
	DDX_Control(pDX, IDC_COLOR7, m_Color7);
	DDX_Control(pDX, IDC_COLOR6, m_Color6);
	DDX_Control(pDX, IDC_COLOR5, m_Color5);
	DDX_Control(pDX, IDC_COLOR4, m_Color4);
	DDX_Control(pDX, IDC_COLOR3, m_Color3);
	DDX_Control(pDX, IDC_COLOR2, m_Color2);
	DDX_Control(pDX, IDC_COLOR1, m_Color1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLeftPortion, CFormView)
	//{{AFX_MSG_MAP(CLeftPortion)
	ON_BN_CLICKED(IDC_COLOR2, OnColor2)
	ON_BN_CLICKED(IDC_COLOR3, OnColor3)
	ON_BN_CLICKED(IDC_COLOR4, OnColor4)
	ON_BN_CLICKED(IDC_COLOR5, OnColor5)
	ON_BN_CLICKED(IDC_COLOR6, OnColor6)
	ON_BN_CLICKED(IDC_COLOR7, OnColor7)
	ON_BN_CLICKED(IDC_COLOR8, OnColor8)
	ON_BN_CLICKED(IDC_SPCOLOR, OnSpcolor)
	ON_BN_CLICKED(IDC_EPCOLOR, OnEpcolor)
	ON_BN_DOUBLECLICKED(IDC_SPCOLOR, OnDoubleclickedSpcolor)
	ON_BN_DOUBLECLICKED(IDC_EPCOLOR, OnDoubleclickedEpcolor)
	ON_BN_CLICKED(IDC_COLOR1, OnColor1)
	ON_BN_CLICKED(IDC_LINE, OnLine)
	ON_BN_CLICKED(IDC_ELLIPSE, OnEllipse)
	ON_BN_CLICKED(IDC_ALIAS, OnAlias)
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
	m_select=TRUE;
	m_color1=CRGB(0.0,0.0,0.0);//黑色
	m_color2=CRGB(1.0,0.0,0.0);//红色
	m_color3=CRGB(1.0,1.0,0.0);//黄色
	m_color4=CRGB(0.0,1.0,0.0);//绿色
	m_color5=CRGB(0.0,1.0,1.0);//青色
	m_color6=CRGB(1.0,0.0,1.0);//品红
	m_color7=CRGB(0.0,0.0,1.0);//蓝色
	m_color8=CRGB(1.0,1.0,1.0);//白色
	m_spcolor=CRGB(0.0,0.0,0.0);//起点颜色
	m_epcolor=CRGB(0.0,0.0,0.0);//终点颜色
	m_Color1.SetBkColor(m_color1);
	m_Color2.SetBkColor(m_color2);
	m_Color3.SetBkColor(m_color3);
	m_Color4.SetBkColor(m_color4);
	m_Color5.SetBkColor(m_color5);
	m_Color6.SetBkColor(m_color6);
	m_Color7.SetBkColor(m_color7);
	m_Color8.SetBkColor(m_color8);
	m_SPColor.SetBkColor(m_spcolor);
	m_EPColor.SetBkColor(m_epcolor);
	UpdateData(FALSE);
}

void CLeftPortion::OnColor1() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	if(m_select==TRUE)
	{
		m_spcolor=m_color1;
		m_SPColor.SetBkColor(m_spcolor);
	}
	else
	{
		m_epcolor=m_color1;
		m_EPColor.SetBkColor(m_epcolor);		
	}
	pDoc->m_SPColor=m_spcolor;
	pDoc->m_EPColor=m_epcolor;
}

void CLeftPortion::OnColor2() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	if(m_select==TRUE)
	{
		m_spcolor=m_color2;
		m_SPColor.SetBkColor(m_spcolor);
	}
	else
	{
		m_epcolor=m_color2;
		m_EPColor.SetBkColor(m_epcolor);		
	}
	pDoc->m_SPColor=m_spcolor;
	pDoc->m_EPColor=m_epcolor;
}

void CLeftPortion::OnColor3() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	if(m_select==TRUE)
	{
		m_spcolor=m_color3;
		m_SPColor.SetBkColor(m_spcolor);
	}
	else
	{
		m_epcolor=m_color3;
		m_EPColor.SetBkColor(m_epcolor);		
	}
	pDoc->m_SPColor=m_spcolor;
	pDoc->m_EPColor=m_epcolor;
}

void CLeftPortion::OnColor4() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	if(m_select==TRUE)
	{
		m_spcolor=m_color4;
		m_SPColor.SetBkColor(m_spcolor);
	}
	else
	{
		m_epcolor=m_color4;
		m_EPColor.SetBkColor(m_epcolor);		
	}
	pDoc->m_SPColor=m_spcolor;
	pDoc->m_EPColor=m_epcolor;
}

void CLeftPortion::OnColor5() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	if(m_select==TRUE)
	{
		m_spcolor=m_color5;
		m_SPColor.SetBkColor(m_spcolor);
	}
	else
	{
		m_epcolor=m_color5;
		m_EPColor.SetBkColor(m_epcolor);		
	}
	pDoc->m_SPColor=m_spcolor;
	pDoc->m_EPColor=m_epcolor;
}

void CLeftPortion::OnColor6() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	if(m_select==TRUE)
	{
		m_spcolor=m_color6;
		m_SPColor.SetBkColor(m_spcolor);
	}
	else
	{
		m_epcolor=m_color6;
		m_EPColor.SetBkColor(m_epcolor);		
	}
	pDoc->m_SPColor=m_spcolor;
	pDoc->m_EPColor=m_epcolor;
}

void CLeftPortion::OnColor7() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	if(m_select==TRUE)
	{
		m_spcolor=m_color7;
		m_SPColor.SetBkColor(m_spcolor);
	}
	else
	{
		m_epcolor=m_color7;
		m_EPColor.SetBkColor(m_epcolor);		
	}
	pDoc->m_SPColor=m_spcolor;
	pDoc->m_EPColor=m_epcolor;
}

void CLeftPortion::OnColor8() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	if(m_select==TRUE)
	{
		m_spcolor=m_color8;
		m_SPColor.SetBkColor(m_spcolor);
	}
	else
	{
		m_epcolor=m_color8;
		m_EPColor.SetBkColor(m_epcolor);		
	}
	pDoc->m_SPColor=m_spcolor;
	pDoc->m_EPColor=m_epcolor;	
}

void CLeftPortion::OnSpcolor() 
{
	// TODO: Add your control notification handler code here
	m_select=TRUE;
}

void CLeftPortion::OnEpcolor() 
{
	// TODO: Add your control notification handler code here
	m_select=FALSE;
}

void CLeftPortion::OnDoubleclickedSpcolor() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	CColorDialog dlg;
 	if(dlg.DoModal()==IDOK)
 	{
 		COLORREF color=dlg.GetColor();
 		m_spcolor=CRGB(GetRValue(color)/255.0,GetGValue(color)/255.0,GetBValue(color)/255.0);
		m_SPColor.SetBkColor(m_spcolor);
    }
	pDoc->m_SPColor=m_spcolor;
}

void CLeftPortion::OnDoubleclickedEpcolor() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	CColorDialog dlg;
 	if(dlg.DoModal()==IDOK)
 	{
 		COLORREF color=dlg.GetColor();
 		m_epcolor=CRGB(GetRValue(color)/255.0,GetGValue(color)/255.0,GetBValue(color)/255.0);
		m_EPColor.SetBkColor(m_epcolor);
    }
	pDoc->m_EPColor=m_epcolor;
}

void CLeftPortion::OnLine() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
 	pDoc->m_nType=1; 
	pDoc->m_SPColor=m_spcolor;
	pDoc->m_EPColor=m_epcolor;
}

void CLeftPortion::OnEllipse() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
 	pDoc->m_nType=2;
	m_epcolor=m_spcolor;
	m_SPColor.SetBkColor(m_spcolor);
	m_EPColor.SetBkColor(m_epcolor);
	pDoc->m_SPColor=m_spcolor;
	pDoc->m_EPColor=m_epcolor;
}

void CLeftPortion::OnAlias() 
{
	// TODO: Add your control notification handler code here
	CTestDoc *pDoc=(CTestDoc*)CFormView::GetDocument();
	CButton *pBtn=(CButton*)GetDlgItem(IDC_ALIAS);
	if(pBtn->GetCheck())
		pDoc->m_Alias=TRUE;
	else
		pDoc->m_Alias=FALSE;	
}
