// TestView.h : interface of the CTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_)
#define AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Face.h"
#include "Vector.h"
#include "ZBuffer.h"
#include "Lighting.h"
#include "TransForm.h"
#include "TestDoc.h"

#include "ACLine.h"

class CTestView : public CView
{
protected: // create from serialization only
	CTestView();
	DECLARE_DYNCREATE(CTestView)

// Attributes
public:
	CTestDoc* GetDocument();
	void ReadPoint();//���붥������
	void ReadFace();//����������
	void InitParameter();//�任����
	void PerProject(CP3);//͸��ͶӰ
	void DoubleBuffer();//˫����
	void DrawObject(CDC *);//��������
	void ReadImage();//��������
	void AntiColorLine(CDC*,CPi3,CPi3);//��ɫ����������
// Operations
public:
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
	int NumR1,NumR2;//���ֶ����ͻ�����ֶ���
	double R1,R2;//���뾶�ͻ�����뾶
	int RingAfa,RingBeta;//����ȷֽ�
	CP3 *P;//���Ķ���һά����
	CVector *PV;//Բ��ÿ�������ƽ��������
	CFace **F;//��Ķ�ά����
	double R,Theta,Phi,d;//�ӵ����û�����ϵ�е�������
	double k[9];//͸�ӱ任����
	CP3 ViewPoint;//�ӵ�������λ��
	CPi3 ScreenP;//��Ļ����ϵ�Ķ�ά�����
	double afa,beta;//x������ת����,y������ת�½�
	int LightNum;//��Դ����
	CLighting *pLight;//���ջ���
	CMaterial *pMaterial;//�������
	CTransForm tran;//�任����
    CZBuffer *zbuf;//��Ȼ������
	BITMAP bm;//BITMAP�ṹ����
	CString PathName;//����λͼ·��
	BYTE *Image;//λͼ������
	int  Model;//ģ�ͷ���
	int  Switch;//���տ���
	int  Render;//��䷽ʽ
// Generated message map functions
public:
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

#endif // !defined(AFX_TESTVIEW_H__A75FDCFB_621C_4E38_A154_C344803E6372__INCLUDED_)
