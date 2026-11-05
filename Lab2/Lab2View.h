
// Lab2View.h : interface of the CLab2View class
//

#pragma once

#include <vector>

class CLab2View : public CView
{
protected: // create from serialization only
	CLab2View() noexcept;
	DECLARE_DYNCREATE(CLab2View)

// Attributes
public:
	CLab2Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	bool grid = true;
	double ugao1 = 0.0f;
	double ugao2 = 0.0f;
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CLab2View();
	void crtaj_svetli_kaktus(CDC* pDC);
	void crtaj_tamni_kaktus(CDC* pDC);
	void crtaj_saksiju(CDC* pDC);
	void crtaj_rotiran_tamni_kaktus(CDC* pDC,double angle);
	void crtaj_rotiran_deblji_tamni_kaktus(CDC* pDC, double angle);
	void crtaj_krug(CDC* pDC, int x, int y, int r);
	BOOL Scale(CDC* pDC, float scaleX, float scaleY, LPCTSTR multiplication);
	BOOL Rotate(CDC* pDC, float angle, LPCTSTR multiplication);
	BOOL Translate(CDC* pDC, float offsetX, float offsetY, LPCTSTR multiplication);
	void PushTransformacija(CDC* pDC);
	void PopTransformacija(CDC* pDC);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	std::vector<XFORM> transformStack;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in Lab2View.cpp
inline CLab2Doc* CLab2View::GetDocument() const
   { return reinterpret_cast<CLab2Doc*>(m_pDocument); }
#endif

