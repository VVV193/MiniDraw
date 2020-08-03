
// MiniDrawDoc.cpp : implementation of the CMiniDrawDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MiniDraw.h"
#endif

#include "MiniDrawDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMiniDrawDoc

IMPLEMENT_DYNCREATE(CMiniDrawDoc, CDocument)

BEGIN_MESSAGE_MAP(CMiniDrawDoc, CDocument)
	ON_COMMAND(ID_EDIT_CLEAR_ALL, &CMiniDrawDoc::OnEditClearAll)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL, &CMiniDrawDoc::OnUpdateEditClearAll)
	ON_COMMAND(ID_EDIT_UNDO, &CMiniDrawDoc::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &CMiniDrawDoc::OnUpdateEditUndo)
END_MESSAGE_MAP()


// CMiniDrawDoc construction/destruction

CMiniDrawDoc::CMiniDrawDoc() noexcept
{
	// TODO: add one-time construction code here

}

CMiniDrawDoc::~CMiniDrawDoc()
{
}

BOOL CMiniDrawDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMiniDrawDoc serialization

void CMiniDrawDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		m_FigArray.Serialize(ar);
	}
	else
	{
		// TODO: add loading code here
		m_FigArray.Serialize(ar);
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CMiniDrawDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CMiniDrawDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CMiniDrawDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMiniDrawDoc diagnostics

#ifdef _DEBUG
void CMiniDrawDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMiniDrawDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// implementation of figure classes: 

IMPLEMENT_SERIAL(CFigure, CObject, 2)

CRect CFigure::GetDimRect()
{
	return CRect
	(min(m_X1, m_X2),
		min(m_Y1, m_Y2),
		max(m_X1, m_X2) + 1,
		max(m_Y1, m_Y2) + 1);
}

void CFigure::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
		ar << m_X1 << m_Y1 << m_X2 << m_Y2 << m_Color;
	else
		ar >> m_X1 >> m_Y1 >> m_X2 >> m_Y2 >> m_Color;
}

IMPLEMENT_SERIAL(CLine, CFigure, 2)

CLine::CLine(int X1, int Y1, int X2, int Y2, COLORREF Color, int Thickness)
{
	m_X1 = X1;
	m_Y1 = Y1;
	m_X2 = X2;
	m_Y2 = Y2;
	m_Color = Color;
	m_Thickness = Thickness;
}

void CLine::Serialize(CArchive& ar)
{
	CFigure::Serialize(ar);
	if (ar.IsStoring())
		ar << m_Thickness;
	else
		ar >> m_Thickness;
}

void CLine::Draw(CDC *PDC)
{
	CPen Pen, *POldPen;

	// select pen/brush:
	Pen.CreatePen(PS_SOLID, m_Thickness, m_Color);
	POldPen = PDC->SelectObject(&Pen);

	// draw figure:
	PDC->MoveTo(m_X1, m_Y1);
	PDC->LineTo(m_X2, m_Y2);

	// remove pen/brush: 
	PDC->SelectObject(POldPen);
}

IMPLEMENT_SERIAL(CRectangle, CFigure, 2)

CRectangle::CRectangle(int X1, int Y1, int X2, int Y2,
	COLORREF Color, int Thickness)
{
	m_X1 = X1;
	m_Y1 = Y1;
	m_X2 = X2;
	m_Y2 = Y2;
	m_Color = Color;
	m_Thickness = Thickness;
}

void CRectangle::Serialize(CArchive& ar)
{
	CFigure::Serialize(ar);
	if (ar.IsStoring())
		ar << m_Thickness;
	else
		ar >> m_Thickness;
}

void CRectangle::Draw(CDC *PDC)
{
	CPen Pen, *POldPen;

	// select pen/brush:
	Pen.CreatePen(PS_INSIDEFRAME, m_Thickness, m_Color);
	POldPen = PDC->SelectObject(&Pen);
	PDC->SelectStockObject(NULL_BRUSH);

	// draw figure:
	PDC->Rectangle(m_X1, m_Y1, m_X2, m_Y2);

	// remove pen/brush:  
	PDC->SelectObject(POldPen);
}

IMPLEMENT_SERIAL(CRectFill, CFigure, 2)

CRectFill::CRectFill(int X1, int Y1, int X2, int Y2, COLORREF Color)
{
	m_X1 = min(X1, X2);
	m_Y1 = min(Y1, Y2);
	m_X2 = max(X1, X2);
	m_Y2 = max(Y1, Y2);
	m_Color = Color;
}

void CRectFill::Draw(CDC *PDC)
{
	CBrush Brush, *POldBrush;
	CPen Pen, *POldPen;

	// select pen/brush:
	Pen.CreatePen(PS_INSIDEFRAME, 1, m_Color);
	POldPen = PDC->SelectObject(&Pen);
	Brush.CreateSolidBrush(m_Color);
	POldBrush = PDC->SelectObject(&Brush);

	// draw figure:
	PDC->Rectangle(m_X1, m_Y1, m_X2, m_Y2);

	// remove pen/brush:  
	PDC->SelectObject(POldPen);
	PDC->SelectObject(POldBrush);
}

IMPLEMENT_SERIAL(CRectRound, CFigure, 2)

CRectRound::CRectRound(int X1, int Y1, int X2, int Y2,
	COLORREF Color, int Thickness)
{
	m_X1 = min(X1, X2);
	m_Y1 = min(Y1, Y2);
	m_X2 = max(X1, X2);
	m_Y2 = max(Y1, Y2);
	m_Color = Color;
	m_Thickness = Thickness;
}

void CRectRound::Serialize(CArchive& ar)
{
	CFigure::Serialize(ar);
	if (ar.IsStoring())
		ar << m_Thickness;
	else
		ar >> m_Thickness;
}

void CRectRound::Draw(CDC *PDC)
{
	CPen Pen, *POldPen;

	// select pen/brush:
	Pen.CreatePen(PS_INSIDEFRAME, m_Thickness, m_Color);
	POldPen = PDC->SelectObject(&Pen);
	PDC->SelectStockObject(NULL_BRUSH);

	// draw figure:
	int SizeRound = (m_X2 - m_X1 + m_Y2 - m_Y1) / 6;
	PDC->RoundRect(m_X1, m_Y1, m_X2, m_Y2, SizeRound, SizeRound);

	// remove pen/brush:  
	PDC->SelectObject(POldPen);
}

IMPLEMENT_SERIAL(CRectRoundFill, CFigure, 2)

CRectRoundFill::CRectRoundFill(int X1, int Y1, int X2, int Y2, COLORREF Color)
{
	m_X1 = min(X1, X2);
	m_Y1 = min(Y1, Y2);
	m_X2 = max(X1, X2);
	m_Y2 = max(Y1, Y2);
	m_Color = Color;
}

void CRectRoundFill::Draw(CDC *PDC)
{
	CBrush Brush, *POldBrush;
	CPen Pen, *POldPen;

	// select pen/brush:
	Pen.CreatePen(PS_INSIDEFRAME, 1, m_Color);
	POldPen = PDC->SelectObject(&Pen);
	Brush.CreateSolidBrush(m_Color);
	POldBrush = PDC->SelectObject(&Brush);

	// draw figure:
	int SizeRound = (m_X2 - m_X1 + m_Y2 - m_Y1) / 6;
	PDC->RoundRect(m_X1, m_Y1, m_X2, m_Y2, SizeRound, SizeRound);

	// remove pen/brush:  
	PDC->SelectObject(POldPen);
	PDC->SelectObject(POldBrush);
}

IMPLEMENT_SERIAL(CCircle, CFigure, 2)

CCircle::CCircle(int X1, int Y1, int X2, int Y2,
	COLORREF Color, int Thickness)
{
	m_X1 = min(X1, X2);
	m_Y1 = min(Y1, Y2);
	m_X2 = max(X1, X2);
	m_Y2 = max(Y1, Y2);
	m_Color = Color;
	m_Thickness = Thickness;
}

void CCircle::Serialize(CArchive& ar)
{
	CFigure::Serialize(ar);
	if (ar.IsStoring())
		ar << m_Thickness;
	else
		ar >> m_Thickness;
}

void CCircle::Draw(CDC *PDC)
{
	CPen Pen, *POldPen;

	// select pen/brush:
	Pen.CreatePen(PS_INSIDEFRAME, m_Thickness, m_Color);
	POldPen = PDC->SelectObject(&Pen);
	PDC->SelectStockObject(NULL_BRUSH);

	// draw figure:
	PDC->Ellipse(m_X1, m_Y1, m_X2, m_Y2);

	// remove pen/brush:  
	PDC->SelectObject(POldPen);
}

IMPLEMENT_SERIAL(CCircleFill, CFigure, 2)

CCircleFill::CCircleFill(int X1, int Y1, int X2, int Y2, COLORREF Color)
{
	m_X1 = min(X1, X2);
	m_Y1 = min(Y1, Y2);
	m_X2 = max(X1, X2);
	m_Y2 = max(Y1, Y2);
	m_Color = Color;
}

void CCircleFill::Draw(CDC *PDC)
{
	CBrush Brush, *POldBrush;
	CPen Pen, *POldPen;

	// select pen/brush:
	Pen.CreatePen(PS_INSIDEFRAME, 1, m_Color);
	POldPen = PDC->SelectObject(&Pen);
	Brush.CreateSolidBrush(m_Color);
	POldBrush = PDC->SelectObject(&Brush);

	// draw figure:
	PDC->Ellipse(m_X1, m_Y1, m_X2, m_Y2);

	// remove pen/brush:  
	PDC->SelectObject(POldPen);
	PDC->SelectObject(POldBrush);
}
void CMiniDrawDoc::AddFigure(CFigure *PFigure)
{
	m_FigArray.Add(PFigure);
	SetModifiedFlag();
}

CFigure *CMiniDrawDoc::GetFigure(int Index)
{
	if (Index < 0 || Index > m_FigArray.GetUpperBound())
		return 0;
	return (CFigure *)m_FigArray.GetAt(Index);
}

int CMiniDrawDoc::GetNumFigs()
{
	return m_FigArray.GetSize();
}

// CMiniDrawDoc commands

void CMiniDrawDoc::DeleteContents()
{
	// TODO: Add your specialized code here and/or call the base class

	int Index = m_FigArray.GetSize();
	while (Index--)
		delete m_FigArray.GetAt(Index);
	m_FigArray.RemoveAll();

	CDocument::DeleteContents();

	CDocument::DeleteContents();
}


void CMiniDrawDoc::OnEditClearAll()
{
	// TODO: Add your command handler code here
	DeleteContents();
	UpdateAllViews(0); // Удалить содержимое окна представления
	SetModifiedFlag();
}


void CMiniDrawDoc::OnUpdateEditClearAll(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_FigArray.GetSize());
}


void CMiniDrawDoc::OnEditUndo()
{
	// TODO: Add your command handler code here
	int Index = m_FigArray.GetUpperBound();
	if (Index > -1)
	{
		delete m_FigArray.GetAt(Index);
		m_FigArray.RemoveAt(Index);
	}
	UpdateAllViews(0);
	SetModifiedFlag();
}

void CMiniDrawDoc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_FigArray.GetSize());
}