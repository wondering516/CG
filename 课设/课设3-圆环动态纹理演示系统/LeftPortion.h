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

class CLeftPortion : public CFormView
{
protected:
	CLeftPortion();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CLeftPortion)

// Form Data
public:
	//{{AFX_DATA(CLeftPortion)
	enum { IDD = IDD_DIALOG1 };
	CStatic	m_ShowTex;
	CStatic	m_CurrentFaces;
	CSliderCtrl	m_NumR2;
	CSliderCtrl	m_NumR1;
	CSliderCtrl	m_R2;
	CSliderCtrl	m_R1;
	int		m_Model;
	int		m_Render;
	int		m_Switch;
	//}}AFX_DATA
// Attributes
public:
// Operations
public:

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
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnFrame();
	afx_msg void OnSurface();
	afx_msg void OnTexture();
	afx_msg void OnOff();
	afx_msg void OnOn();
	afx_msg void OnPhong();
	afx_msg void OnGouraud();
	afx_msg void OnSeltex();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTPORTION_H__50B7BF9C_BA99_4DBD_A0AF_8518BB1F6E16__INCLUDED_)
