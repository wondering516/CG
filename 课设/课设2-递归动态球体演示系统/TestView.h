// TestView.h : interface of the CTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_)
#define AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Face.h"//������
#include "Vector.h"//ʸ����
#include "Line.h"//ֱ����
#include "Fill.h"//�����
#include "Lighting.h"//������
#include "Material.h"//��Դ��
#include "ACLine.h"//ֱ����
#include "TransForm.h"//�任��

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
	void ReadPoint();//������
	void ReadFace();//�������
	void DrawObject(CDC *);//��������
	void SubDivide(CDC *pDC,CP3,CP3,CP3,int);//�ݹ麯��
	void DrawTriangle(CDC *pDC,CP3,CP3,CP3);//���������κ���
	void Normalize(CP3 &);//ʸ����չ����
	void DoubleBuffer();//˫����
	void BorderCheck();//�߽���ײ���
	void AntiColorLine(CDC*,CP2,CP2);//��ɫ����������
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
	CP3    P[7];//�任��
	CFace  F[8];//���
	int    Model;//�߿�����
	int    Switch;//���տ���
	int	   DirectionX,DirectionY,DirectionZ;//λ�Ʒ������
	double TranslateX,TranslateY,TranslateZ;//ƽ�Ʊ任�������
	double RotateX,RotateY,RotateZ;//��ת�任�������
	int    FaceCount;//�ݹ����
	double Radius;//����뾶
	int	 LightNum;//��Դ����
	CLighting *pLight;//���ջ���
	CMaterial *pMaterial;//�������
	CACLine aline;//��������ɫ����ֱ�������
	CTransForm tran;//�任�����
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
