
// MiniDrawView.h : interface of the CMiniDrawView class
//

#pragma once

const int DRAWWIDTH = 4000;  // ширина рисунка
const int DRAWHEIGHT = 6000;  // высота рисунка

class CMiniDrawView : public CScrollView
{
protected:
	CString m_ClassName;
	int m_Dragging;
	HCURSOR m_HArrow;
	HCURSOR m_HCross; // Дескриптор указателя мыши
	CPen m_PenDotted;
	CPoint m_PointOld;
	CPoint m_PointOrigin;

protected: // create from serialization only
	CMiniDrawView() noexcept;
	DECLARE_DYNCREATE(CMiniDrawView)

// Attributes
public:
	CMiniDrawDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CMiniDrawView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
};

#ifndef _DEBUG  // debug version in MiniDrawView.cpp
inline CMiniDrawDoc* CMiniDrawView::GetDocument() const
   { return reinterpret_cast<CMiniDrawDoc*>(m_pDocument); }
#endif

