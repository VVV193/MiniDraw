
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
		m_LineArray.Serialize(ar);
	}
	else
	{
		// TODO: add loading code here
		m_LineArray.Serialize(ar);
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


// CMiniDrawDoc commands
IMPLEMENT_SERIAL(CLine, CObject, 1) // Класс, баз.класс, версия

void CLine::Draw(CDC *PDC)
{
	PDC->MoveTo(m_X1, m_Y1);
	PDC->LineTo(m_X2, m_Y2);
}

void CLine::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
		ar << m_X1 << m_Y1 << m_X2 << m_Y2;
	else
		ar >> m_X1 >> m_Y1 >> m_X2 >> m_Y2;
}

CRect CLine::GetDimRect()
{
	return CRect(min(m_X1, m_X2), min(m_Y1, m_Y2),
				 max(m_X1, m_X2) + 1, max(m_Y1, m_Y2) + 1);
}

CLine *CMiniDrawDoc::AddLine(int X1, int Y1, int X2, int Y2)
{
	CLine *PLine = new CLine(X1, Y1, X2, Y2);
	m_LineArray.Add(PLine);
	SetModifiedFlag();
	return PLine;
}

CLine *CMiniDrawDoc::GetLine(int Index)
{
	if (Index < 0 || Index > m_LineArray.GetUpperBound())
		return 0;
	return m_LineArray.GetAt(Index);
}

int CMiniDrawDoc::GetNumLines()
{
	return m_LineArray.GetSize();
}

void CMiniDrawDoc::DeleteContents()
{
	// TODO: Add your specialized code here and/or call the base class
	int Index = m_LineArray.GetSize();
	while (Index--)
		delete m_LineArray.GetAt(Index);
	m_LineArray.RemoveAll();

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
	pCmdUI->Enable(m_LineArray.GetSize());
}

void CMiniDrawDoc::OnEditUndo()
{
	// TODO: Add your command handler code here
	int Index = m_LineArray.GetUpperBound();
	if (Index > -1)
	{
		delete m_LineArray.GetAt(Index);
		m_LineArray.RemoveAt(Index);
	}
	UpdateAllViews(0);
	SetModifiedFlag();
}

void CMiniDrawDoc::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_LineArray.GetSize());
}
