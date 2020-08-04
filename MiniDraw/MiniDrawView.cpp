
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
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CMiniDrawView construction/destruction

CMiniDrawView::CMiniDrawView() noexcept
{
	// TODO: add construction code here
	m_Dragging = 0;
	m_HArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_HCross = AfxGetApp()->LoadStandardCursor(IDC_CROSS);
	m_PenDotted.CreatePen(PS_DOT, 1, RGB(0, 0, 0));
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
	pDC->LineTo(ScrollSize.cx, ScrollSize.cy); // Рисуем
	pDC->LineTo(0, ScrollSize.cy);	 // границу
	CRect ClipRect;
	CRect DimRect;
	CRect IntRect;
	CFigure *PFigure;

	pDC->GetClipBox(&ClipRect); // Получить недейств. область

	int NumFigs = pDoc->GetNumFigs();
	for (int Index = 0; Index < NumFigs; ++Index)
	{
		PFigure = pDoc->GetFigure(Index);
		DimRect = PFigure->GetDimRect();
		if (IntRect.IntersectRect(DimRect, ClipRect))
			PFigure->Draw(pDC);
	}
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

	if (!m_Dragging)
	{
		CSize ScrollSize = GetTotalSize();
		CRect ScrollRect(0, 0, ScrollSize.cx, ScrollSize.cy);
		if (ScrollRect.PtInRect(point))
			::SetCursor(m_HCross);
		else
			::SetCursor(m_HArrow);
		return;
	}

	ClientDC.SetROP2(R2_NOT);
	ClientDC.SelectObject(&m_PenDotted);
	ClientDC.SetBkMode(TRANSPARENT);
	ClientDC.SelectStockObject(NULL_BRUSH);

	switch (((CMiniDrawApp *)AfxGetApp())->m_CurrentTool)
	{
	case ID_TOOLS_LINE:
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(m_PointOld);
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(point);
		break;

	case ID_TOOLS_RECTANGLE:
	case ID_TOOLS_RECTFILL:
		ClientDC.Rectangle(m_PointOrigin.x, m_PointOrigin.y,
			m_PointOld.x, m_PointOld.y);
		ClientDC.Rectangle(m_PointOrigin.x, m_PointOrigin.y,
			point.x, point.y);
		break;

	case ID_TOOLS_RECTROUND:
	case ID_TOOLS_RECTROUNDFILL:
	{
		int SizeRound = (abs(m_PointOld.x - m_PointOrigin.x) +
			abs(m_PointOld.y - m_PointOrigin.y)) / 6;
		ClientDC.RoundRect(m_PointOrigin.x, m_PointOrigin.y,
			m_PointOld.x, m_PointOld.y,
			SizeRound, SizeRound);
		SizeRound = (abs(point.x - m_PointOrigin.x) +
			abs(point.y - m_PointOrigin.y)) / 6;
		ClientDC.RoundRect(m_PointOrigin.x, m_PointOrigin.y,
			point.x, point.y,
			SizeRound, SizeRound);
		break;
	}

	case ID_TOOLS_CIRCLE:
	case ID_TOOLS_CIRCLEFILL:
		ClientDC.Ellipse(m_PointOrigin.x, m_PointOrigin.y,
			m_PointOld.x, m_PointOld.y);
		ClientDC.Ellipse(m_PointOrigin.x, m_PointOrigin.y,
			point.x, point.y);
		break;
	}

	m_PointOld = point;

	CScrollView::OnMouseMove(nFlags, point);
}


void CMiniDrawView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (!m_Dragging)
		return;

	m_Dragging = 0;
	::ReleaseCapture();
	::ClipCursor(NULL);

	CClientDC ClientDC(this);
	OnPrepareDC(&ClientDC);
	ClientDC.DPtoLP(&point);
	ClientDC.SetROP2(R2_NOT);
	ClientDC.SelectObject(&m_PenDotted);
	ClientDC.SetBkMode(TRANSPARENT);
	ClientDC.SelectStockObject(NULL_BRUSH);

	CMiniDrawApp *PApp = (CMiniDrawApp *)AfxGetApp();
	CFigure *PFigure = NULL;

	switch (PApp->m_CurrentTool)
	{
	case ID_TOOLS_LINE:
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(m_PointOld);
		PFigure = new CLine
		(m_PointOrigin.x, m_PointOrigin.y,
			point.x, point.y,
			PApp->m_CurrentColor,
			PApp->m_CurrentThickness);
		break;

	case ID_TOOLS_RECTANGLE:
		ClientDC.Rectangle(m_PointOrigin.x, m_PointOrigin.y,
			m_PointOld.x, m_PointOld.y);
		PFigure = new CRectangle
		(m_PointOrigin.x, m_PointOrigin.y,
			point.x, point.y,
			PApp->m_CurrentColor,
			PApp->m_CurrentThickness);
		break;

	case ID_TOOLS_RECTFILL:
		ClientDC.Rectangle(m_PointOrigin.x, m_PointOrigin.y,
			m_PointOld.x, m_PointOld.y);
		PFigure = new CRectFill
		(m_PointOrigin.x, m_PointOrigin.y,
			point.x, point.y,
			PApp->m_CurrentColor);
		break;

	case ID_TOOLS_RECTROUND:
	{
		int SizeRound = (abs(m_PointOld.x - m_PointOrigin.x) +
			abs(m_PointOld.y - m_PointOrigin.y)) / 6;
		ClientDC.RoundRect(m_PointOrigin.x, m_PointOrigin.y,
			m_PointOld.x, m_PointOld.y,
			SizeRound, SizeRound);
		PFigure = new CRectRound
		(m_PointOrigin.x, m_PointOrigin.y,
			point.x, point.y,
			PApp->m_CurrentColor,
			PApp->m_CurrentThickness);
		break;
	}

	case ID_TOOLS_RECTROUNDFILL:
	{
		int SizeRound = (abs(m_PointOld.x - m_PointOrigin.x) +
			abs(m_PointOld.y - m_PointOrigin.y)) / 6;
		ClientDC.RoundRect(m_PointOrigin.x, m_PointOrigin.y,
			m_PointOld.x, m_PointOld.y,
			SizeRound, SizeRound);
		PFigure = new CRectRoundFill
		(m_PointOrigin.x, m_PointOrigin.y,
			point.x, point.y,
			PApp->m_CurrentColor);
		break;
	}

	case ID_TOOLS_CIRCLE:
		ClientDC.Ellipse(m_PointOrigin.x, m_PointOrigin.y,
			m_PointOld.x, m_PointOld.y);
		PFigure = new CCircle
		(m_PointOrigin.x, m_PointOrigin.y,
			point.x, point.y,
			PApp->m_CurrentColor,
			PApp->m_CurrentThickness);
		break;

	case ID_TOOLS_CIRCLEFILL:
		ClientDC.Ellipse(m_PointOrigin.x, m_PointOrigin.y,
			m_PointOld.x, m_PointOld.y);
		PFigure = new CCircleFill
		(m_PointOrigin.x, m_PointOrigin.y,
			point.x, point.y,
			PApp->m_CurrentColor);
		break;
	}

	ClientDC.SetROP2(R2_COPYPEN);
	PFigure->Draw(&ClientDC);

	CMiniDrawDoc* PDoc = GetDocument();
	PDoc->AddFigure(PFigure);

	PDoc->UpdateAllViews(this, 0, PFigure);

	CScrollView::OnLButtonUp(nFlags, point);
}


void CMiniDrawView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	// TODO: Add your specialized code here and/or call the base class
	SIZE Size = { DRAWWIDTH, DRAWHEIGHT };
	SetScrollSizes(MM_TEXT, Size);
}


void CMiniDrawView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pHint != 0)
	{
		CRect InvalidRect = ((CLine *)pHint)->GetDimRect();
		CClientDC ClientDC(this);
		OnPrepareDC(&ClientDC);// Скорректировать для текущей
							// позиции прокрутки
		ClientDC.LPtoDP(&InvalidRect); // Лог.коорд.->физич.
		InvalidateRect(&InvalidRect);
	}
	else
		CScrollView::OnUpdate(pSender, lHint, pHint);
}


BOOL CMiniDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{

	// TODO:  call DoPreparePrinting to invoke the Print dialog box

	// УДАЛЕНО: return CScrollView::OnPreparePrinting(pInfo);
	return DoPreparePrinting(pInfo);
}
