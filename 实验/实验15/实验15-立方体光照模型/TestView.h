// TestView.h : interface of the CTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_)
#define AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Vector.h"
#include "Face.h"
#include "Lighting.h"
#include "Material.h"
#include "ZBuffer.h"
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
	void ReadPoint();//���붥���
	void ReadFace();//�������
	void InitParameter();//������ʼ��
	void PerProject(CP3);//͸��ͶӰ
	void DoubleBuffer();//˫����
	void DrawObject(CDC *);//����������		
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
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
	double R,Theta,Phi;//�ӵ����û�����ϵ�е�������
	double d;//�Ӿ�
	CP3 P[8];//�����嶥��
	CFace F[6];//���
	double afa,beta;//x������ת����,y������ת�½�
	double k[9];//���㳣��
	CP3 ViewPoint;//�ӵ�������
	CPi3 ScreenP;//��Ļ����ϵ����ά�����
	BOOL Play;//��������
	CLighting *pLight;//��Դ����
	CMaterial *pMaterial;//�������
	int LightNum;//��Դ����
	CTransForm tran;//�任����
	// Generated message map functions
protected:
	//{{AFX_MSG(CTestView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMplay();
	afx_msg void OnUpdateMplay(CCmdUI* pCmdUI);
	afx_msg void OnMlred();
	afx_msg void OnMmemerald();
	afx_msg void OnMmruby();
	afx_msg void OnMlgreen();
	afx_msg void OnMlblue();
	afx_msg void OnMmsapphire();
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

#endif // !defined(AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_)
