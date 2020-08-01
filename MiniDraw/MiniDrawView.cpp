
// MiniDrawView.cpp : implementation of the CMiniDrawView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MiniDraw.h"
#endif

#include "MiniDrawDoc.h"
#include "MiniDrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMiniDrawView

IMPLEMENT_DYNCREATE(CMiniDrawView, CScrollView)

BEGIN_MESSAGE_MAP(CMiniDrawView, CScrollView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMiniDrawView construction/destruction

CMiniDrawView::CMiniDrawView() noexcept
{
	// TODO: add construction code here
	m_Dragging = 0;
	m_HArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_HCross = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
}

CMiniDrawView::~CMiniDrawView()
{
}

BOOL CMiniDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	m_ClassName = AfxRegisterWndClass
	(CS_HREDRAW | CS_VREDRAW,				// стили окна
		0,                                      // без курсора
		(HBRUSH)::GetStockObject(WHITE_BRUSH),	// белый фон 
		0);                                     // без значка
	cs.lpszClass = m_ClassName;

	return CScrollView::PreCreateWindow(cs);
}

// CMiniDrawView drawing

void CMiniDrawView::OnDraw(CDC* pDC)
{
	CMiniDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here

	CSize ScrollSize = GetTotalSize(); // Получим размер
	pDC->MoveTo(ScrollSize.cx, 0);
	pDC->LineTo(ScrollSize.cx, ScrollSize.cy);	// Рисуем
	pDC->LineTo(0, ScrollSize.cy);				// границу

	int Index = pDoc->GetNumLines();
	while (Index--)
		pDoc->GetLine(Index)->Draw(pDC);

}


// CMiniDrawView diagnostics

#ifdef _DEBUG
void CMiniDrawView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMiniDrawView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMiniDrawDoc* CMiniDrawView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMiniDrawDoc)));
	return (CMiniDrawDoc*)m_pDocument;
}
#endif //_DEBUG


// CMiniDrawView message handlers


void CMiniDrawView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CClientDC ClientDC(this);
	OnPrepareDC(&ClientDC);
	ClientDC.DPtoLP(&point);

	// Находимся ли внутри области окна представления	
	CSize ScrollSize = GetTotalSize();
	CRect ScrollRect(0, 0, ScrollSize.cx, ScrollSize.cy);
	if (!ScrollRect.PtInRect(point)) // TRUE, если внутри
		return;

	// Сохранение позиции, "захват" мыши, установка флага
	m_PointOrigin = point; 	// Начало линии
	m_PointOld = point; 	// Используют другие обработчики
	SetCapture(); 			// "Захват" сообщений от мыши окном
	m_Dragging = 1; 		// Идет рисование

	RECT Rect;
	GetClientRect(&Rect); 	// Координаты окна представления
	ClientToScreen(&Rect);	// Преобразуем в экранные (от верхнего левого угла)
	::ClipCursor(&Rect);	// Огр.перемещ.курс.пределами окна

	// Ограничение перемещений указателя мыши
	ClientDC.LPtoDP(&ScrollRect);
	CRect ViewRect; // Координаты окна представления
	GetClientRect(&ViewRect);
	CRect IntRect;  // Координаты пересечения области рисунка 
	IntRect.IntersectRect(&ScrollRect, &ViewRect);
	ClientToScreen(&IntRect); // В координаты экрана
	::ClipCursor(&IntRect);   // Ограничить перемещение

	CScrollView::OnLButtonDown(nFlags, point);
}


void CMiniDrawView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CClientDC ClientDC(this);
	OnPrepareDC(&ClientDC);
	ClientDC.DPtoLP(&point);
	CSize ScrollSize = GetTotalSize();
	CRect ScrollRect(0, 0, ScrollSize.cx, ScrollSize.cy);
	if (ScrollRect.PtInRect(point))
		::SetCursor(m_HCross); // Установка типа курсора
	else
		::SetCursor(m_HArrow); // Установка типа курсора
	if (m_Dragging)
	{
		CClientDC ClientDC(this);
		OnPrepareDC(&ClientDC);
		ClientDC.DPtoLP(&point);
		ClientDC.SetROP2(R2_NOT);
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(m_PointOld); 	// Стирание линии
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(point); 	// Рисование новой линии
		m_PointOld = point;
	}
	CScrollView::OnMouseMove(nFlags, point);
}


void CMiniDrawView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_Dragging)
	{
		m_Dragging = 0;
		::ReleaseCapture(); // Отменить захват сообщений мыши
		::ClipCursor(NULL); // Курсор двигается по всему экрану
		CClientDC ClientDC(this);
		OnPrepareDC(&ClientDC);
		ClientDC.SetROP2(R2_NOT);
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(m_PointOld);
		ClientDC.SetROP2(R2_COPYPEN);
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(point);
		CMiniDrawDoc* PDoc = GetDocument();
		PDoc->AddLine(m_PointOrigin.x, m_PointOrigin.y, point.x, point.y);	// Запомнить линию
		PDoc->UpdateAllViews(this);
	}
	CScrollView::OnLButtonUp(nFlags, point);
}


void CMiniDrawView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	SIZE Size = { 640, 480 };
	SetScrollSizes(MM_TEXT, Size);
}
