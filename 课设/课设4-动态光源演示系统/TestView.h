// TestView.h : interface of the CTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_)
#define AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Sphere.h"
#include "TransForm.h"

class CTestView : public CView
{
protected: // create from serialization only
	CTestView();
	DECLARE_DYNCREATE(CTestView)

// Attributes
public:
	CTestDoc* GetDocument();

// Operations
public:
	void InitModel();//���ó�ʼλ�úʹ�С
	void DoubleBuffer();//˫����
	void SetLight();//���ù�Դ
	void DrawScene(CDC *);//���Ƴ���	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CSphere Globe,RedG,GreenG,BlueG;
	CTransForm Tran_RedG,Tran_GreenG,Tran_BlueG;//�任����
	CP3 RedGTran,GreenGTran,BlueGTran;
	CMaterial *pMaterial;//�������
	int LightNum;//��Դ����
	CLighting *pLight;//���ջ���
	BOOL Play;//��������
	int ra,rb,ga,gb,ba,bb;//��������Դ�켣��Բ�ĳ��̰���
	int Alpha;//��Բ������
	int RedLight,GreenLight,BlueLight;//��ת��Դ
// Generated message map functions
protected:
	//{{AFX_MSG(CTestView)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TestView.cpp
inline CTestDoc* CTestView::GetDocument()
   { return (CTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_)
