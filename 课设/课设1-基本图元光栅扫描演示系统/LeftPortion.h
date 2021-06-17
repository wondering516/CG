#if !defined(AFX_LEFTPORTION_H__50B7BF9C_BA99_4DBD_A0AF_8518BB1F6E16__INCLUDED_)
#define AFX_LEFTPORTION_H__50B7BF9C_BA99_4DBD_A0AF_8518BB1F6E16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeftPortion.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLeftPortion form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "ColorButton.h"

class CLeftPortion : public CFormView
{
protected:
	CLeftPortion();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLeftPortion)

// Form Data
public:
	//{{AFX_DATA(CLeftPortion)
	enum { IDD = IDD_DIALOG1 };
	CColorButton	m_SPColor;
	CColorButton	m_EPColor;
	CColorButton	m_Color8;
	CColorButton	m_Color7;
	CColorButton	m_Color6;
	CColorButton	m_Color5;
	CColorButton	m_Color4;
	CColorButton	m_Color3;
	CColorButton	m_Color2;
	CColorButton	m_Color1;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	CRGB  m_color1;
	CRGB  m_color2;
	CRGB  m_color3;
	CRGB  m_color4;
	CRGB  m_color5;
	CRGB  m_color6;
	CRGB  m_color7;
	CRGB  m_color8;
	CRGB  m_spcolor;
	CRGB  m_epcolor;
	BOOL  m_select;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftPortion)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLeftPortion();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CLeftPortion)
	afx_msg void OnColor2();
	afx_msg void OnColor3();
	afx_msg void OnColor4();
	afx_msg void OnColor5();
	afx_msg void OnColor6();
	afx_msg void OnColor7();
	afx_msg void OnColor8();
	afx_msg void OnSpcolor();
	afx_msg void OnEpcolor();
	afx_msg void OnDoubleclickedSpcolor();
	afx_msg void OnDoubleclickedEpcolor();
	afx_msg void OnColor1();
	afx_msg void OnLine();
	afx_msg void OnEllipse();
	afx_msg void OnAlias();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTPORTION_H__50B7BF9C_BA99_4DBD_A0AF_8518BB1F6E16__INCLUDED_)
