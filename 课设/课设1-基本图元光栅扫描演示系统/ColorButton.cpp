// ColorButton.cpp : implementation file
//

#include "stdafx.h"
#include "Test.h"
#include "ColorButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorButton

CColorButton::CColorButton()
{
}

CColorButton::~CColorButton()
{
}


BEGIN_MESSAGE_MAP(CColorButton, CButton)
	//{{AFX_MSG_MAP(CColorButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorButton message handlers

void CColorButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ModifyStyle(0,BS_OWNERDRAW); //���ð�ť����Ϊowner-draw
	CButton::PreSubclassWindow();
}

void CColorButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC *pDC=CDC::FromHandle(lpDrawItemStruct->hDC);//���DRAWITEMSTRUCT�ṹ����豸������
    GetWindowText(ButtonText);//��ȡ��ť�ı�
	ButtonRect=lpDrawItemStruct->rcItem;//��ȡ��ť�ߴ�
    int nSavedDC=pDC->SaveDC();//�����豸������
	CPen NewPen,*pOldPen;
	NewPen.CreatePen(PS_SOLID,3,RGB(255,255,255));
	pOldPen=pDC->SelectObject(&NewPen); 
    CBrush NewBrush,*pOldBrush;
    NewBrush.CreateSolidBrush(RGB(BackColor.red*255,BackColor.green*255,BackColor.blue*255));//����ɫ��ˢ
	pOldBrush=pDC->SelectObject(&NewBrush);   
    pDC->RoundRect(&ButtonRect,CPoint(5,5));//��Բ�Ǿ��� 
	pDC->SetTextColor(RGB(255-BackColor.red*255,255-BackColor.green*255,255-BackColor.blue*255));//���ð�ť�ı���ɫ
	pDC->SetBkMode(TRANSPARENT);//���ñ���͸��ģʽ
	pDC->DrawText(ButtonText,&ButtonRect,DT_SINGLELINE|DT_CENTER|DT_VCENTER|DT_END_ELLIPSIS);//���ư�ť�ı�
	pDC->SelectObject(pOldBrush);
	NewBrush.DeleteObject();
	pDC->SelectObject(pOldPen);
	NewPen.DeleteObject();
	if(GetFocus()==this)//���ƾ��εĽ���
	{
		CRect Rect;
		Rect.SetRect(ButtonRect.left+3,ButtonRect.top+2,ButtonRect.right-3,ButtonRect.bottom-2);//���γߴ�
		pDC->DrawFocusRect(&Rect);//���ƽ���
	}
    pDC->RestoreDC(nSavedDC);//�ָ��豸������
}

void CColorButton::SetBkColor(CRGB color)//���ñ�����ɫ
{
    BackColor=color;
    Invalidate();
} 

void CColorButton::SetText(CString str)//���ð�ť�ı�
{
    ButtonText=_T("");
    SetWindowText(str);
} 
