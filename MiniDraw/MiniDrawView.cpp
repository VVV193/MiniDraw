
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

IMPLEMENT_DYNCREATE(CMiniDrawView, CView)

BEGIN_MESSAGE_MAP(CMiniDrawView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CMiniDrawView construction/destruction

CMiniDrawView::CMiniDrawView() noexcept
{
	// TODO: add construction code here
	m_Dragging = 0;
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
		(CS_HREDRAW | CS_VREDRAW,				// ����� ����
		0,                                      // ��� �������
		(HBRUSH)::GetStockObject(WHITE_BRUSH),	// ����� ��� 
		0);                                     // ��� ������
	cs.lpszClass = m_ClassName;

	return CView::PreCreateWindow(cs);
}

// CMiniDrawView drawing

void CMiniDrawView::OnDraw(CDC* /*pDC*/)
{
	CMiniDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMiniDrawView diagnostics

#ifdef _DEBUG
void CMiniDrawView::AssertValid() const
{
	CView::AssertValid();
}

void CMiniDrawView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
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
	m_PointOrigin = point; 	// ������ �����
	m_PointOld = point; 	// ���������� ������ �����������
	SetCapture(); 			// "������" ��������� �� ���� �����
	m_Dragging = 1; 		// ���� ���������

	RECT Rect;
	GetClientRect(&Rect); 	// ���������� ���� �������������
	ClientToScreen(&Rect);	// ����������� � �������� (�� �������� ������ ����)
	::ClipCursor(&Rect);	// ���.�������.����.��������� ����

	CView::OnLButtonDown(nFlags, point);
}


void CMiniDrawView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	::SetCursor(m_HCross); // ��������� ���� �������
	if (m_Dragging)
	{
		CClientDC ClientDC(this);
		ClientDC.SetROP2(R2_NOT);
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(m_PointOld); 	// �������� �����
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(point); 	// ��������� ����� �����
		m_PointOld = point;
	}
	CView::OnMouseMove(nFlags, point);
}


void CMiniDrawView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_Dragging)
	{
		m_Dragging = 0;
		::ReleaseCapture(); // �������� ������ ��������� ����
		::ClipCursor(NULL); // ������ ��������� �� ����� ������
		CClientDC ClientDC(this);
		ClientDC.SetROP2(R2_NOT);
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(m_PointOld);
		ClientDC.SetROP2(R2_COPYPEN);
		ClientDC.MoveTo(m_PointOrigin);
		ClientDC.LineTo(point);
	}
	CView::OnLButtonUp(nFlags, point);
}
