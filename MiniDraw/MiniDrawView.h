
// MiniDrawView.h : interface of the CMiniDrawView class
//

#pragma once


class CMiniDrawView : public CScrollView
{
protected:
	CString m_ClassName;
	int m_Dragging;
	HCURSOR m_HCross; // Дескриптор указателя мыши
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
};

#ifndef _DEBUG  // debug version in MiniDrawView.cpp
inline CMiniDrawDoc* CMiniDrawView::GetDocument() const
   { return reinterpret_cast<CMiniDrawDoc*>(m_pDocument); }
#endif

