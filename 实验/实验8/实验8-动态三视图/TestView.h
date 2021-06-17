// TestView.h : interface of the CTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_)
#define AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Face.h"//包含面头文件
#include "Line.h"//包含直线头文件
#include "Vector.h"//包含矢量头文件

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
	void ReadPoint();//读入顶点函数
	void ReadFace();//读入面函数
	void DoubleBuffer();//双缓冲绘图
	void Project(CP3);//斜等侧变换
	void DrawObject(CDC *);//绘制立体表面
	void DrawTriView(CDC *,CP3 []);
	void Rotate();//旋转变换
	void TVMatrix();//主视图变换矩阵
	void THMatrix();//俯视图变换矩阵
	void TWMatrix();//侧视图变换矩阵
	void MultiMatrix(double T[][4]);//矩阵相乘 
	void DrawVView(CDC*pDC );//绘制主视图
	void DrawHView(CDC*pDC);//绘制俯视图
	void DrawWView(CDC*pDC);//绘制侧视图
	double afa,beta;//斜投影角
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
	CP3 POld[10];//旧三维顶点
	CP3 PNew[10];//新三维顶点
	CP3 PTri[10];//三视图顶点
	CFace F[7];//面表
	double TV[4][4];//主视图变换矩阵
	double TH[4][4];//俯视图变换矩阵
	double TW[4][4];//左视图变换矩阵
	double Afa,Beta;//x方向旋转α角,y方向旋转β角
	CP2 ScreenP;//屏幕坐标系的二维坐标点
// Generated message map functions
protected:
	//{{AFX_MSG(CTestView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
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
