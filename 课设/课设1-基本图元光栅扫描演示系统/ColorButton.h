#if !defined(AFX_ColorButton_H__5AB0AEA6_A2E5_46F0_829E_FFE70FFC9208__INCLUDED_)
#define AFX_ColorButton_H__5AB0AEA6_A2E5_46F0_829E_FFE70FFC9208__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorButton.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CColorButton window
#include "RGB.h"

class CColorButton : public CButton
{
// Construction
public:
	CColorButton();

// Attributes
public:

// Operations
public:
	void SetBkColor(CRGB);//���ð�ť������ɫ
	void SetText(CString);//���ð�ť�ı�
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorButton();

	// Generated message map functions
protected:
	CString     ButtonText;//��ť����
	CRect       ButtonRect;//��ť�ߴ�
	CRGB        BackColor;//��ť����ɫ
	//{{AFX_MSG(CColorButton)
	// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORBUTTON_H__5AB0AEA6_A2E5_46F0_829E_FFE70FFC9208__INCLUDED_)
