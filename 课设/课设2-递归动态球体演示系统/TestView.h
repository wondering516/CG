// TestView.h : interface of the CTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_)
#define AFX_TESTVIEW_H__7B09737C_A6B6_4402_B2B8_087738721CF6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Face.h"//表面类
#include "Vector.h"//矢量类
#include "Line.h"//直线类
#include "Fill.h"//填充类
#include "Lighting.h"//光照类
#include "Material.h"//光源类
#include "ACLine.h"//直线类
#include "TransForm.h"//变换类

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
	void ReadPoint();//读入点表
	void ReadFace();//读入面表
	void DrawObject(CDC *);//绘制物体
	void SubDivide(CDC *pDC,CP3,CP3,CP3,int);//递归函数
	void DrawTriangle(CDC *pDC,CP3,CP3,CP3);//绘制三角形函数
	void Normalize(CP3 &);//矢量扩展函数
	void DoubleBuffer();//双缓冲
	void BorderCheck();//边界碰撞检测
	void AntiColorLine(CDC*,CP2,CP2);//彩色线条反走样
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
	CP3    P[7];//变换点
	CFace  F[8];//面表
	int    Model;//线框消隐
	int    Switch;//光照开关
	int	   DirectionX,DirectionY,DirectionZ;//位移方向控制
	double TranslateX,TranslateY,TranslateZ;//平移变换矩阵参数
	double RotateX,RotateY,RotateZ;//旋转变换矩阵参数
	int    FaceCount;//递归深度
	double Radius;//球体半径
	int	 LightNum;//光源数量
	CLighting *pLight;//光照环境
	CMaterial *pMaterial;//物体材质
	CACLine aline;//反走样颜色渐变直线类对象
	CTransForm tran;//变换类对象
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
