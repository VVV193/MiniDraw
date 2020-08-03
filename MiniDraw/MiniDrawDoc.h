
// MiniDrawDoc.h : interface of the CMiniDrawDoc class
//


#pragma once
class CFigure : public CObject
{
protected:
	COLORREF m_Color;
	DWORD m_X1, m_Y1, m_X2, m_Y2;

	CFigure() {}
	DECLARE_SERIAL(CFigure)

public:
	virtual void Draw(CDC *PDC) {}
	CRect GetDimRect();
	virtual void Serialize(CArchive& ar);
};

class CLine : public CFigure
{
protected:
	DWORD m_Thickness;
	CLine() {}
	DECLARE_SERIAL(CLine)

public:
	CLine(int X1, int Y1, int X2, int Y2, COLORREF Color, int Thickness);
	virtual void Draw(CDC *PDC);
	virtual void Serialize(CArchive& ar);
};

class CRectangle : public CFigure
{
protected:
	DWORD m_Thickness;

	CRectangle() {}
	DECLARE_SERIAL(CRectangle)

public:
	CRectangle(int X1, int Y1, int X2, int Y2, COLORREF Color, int Thickness);
	virtual void Draw(CDC *PDC);
	virtual void Serialize(CArchive& ar);
};

class CRectFill : public CFigure
{
protected:
	CRectFill() {}
	DECLARE_SERIAL(CRectFill)

public:
	CRectFill(int X1, int Y1, int X2, int Y2, COLORREF Color);
	virtual void Draw(CDC *PDC);
};

class CRectRound : public CFigure
{
protected:
	DWORD m_Thickness;

	CRectRound() {}
	DECLARE_SERIAL(CRectRound)

public:
	CRectRound(int X1, int Y1, int X2, int Y2, COLORREF Color, int Thickness);
	virtual void Draw(CDC *PDC);
	virtual void Serialize(CArchive& ar);
};

class CRectRoundFill : public CFigure
{
protected:
	CRectRoundFill() {}
	DECLARE_SERIAL(CRectRoundFill)

public:
	CRectRoundFill(int X1, int Y1, int X2, int Y2, COLORREF Color);
	virtual void Draw(CDC *PDC);
};

class CCircle : public CFigure
{
protected:
	DWORD m_Thickness;

	CCircle() {}
	DECLARE_SERIAL(CCircle)

public:
	CCircle(int X1, int Y1, int X2, int Y2, COLORREF Color, int Thickness);
	virtual void Draw(CDC *PDC);
	virtual void Serialize(CArchive& ar);
};

class CCircleFill : public CFigure
{
protected:
	CCircleFill() {}
	DECLARE_SERIAL(CCircleFill)

public:
	CCircleFill(int X1, int Y1, int X2, int Y2, COLORREF Color);
	virtual void Draw(CDC *PDC);
};

class CMiniDrawDoc : public CDocument
{
protected:
	CTypedPtrArray<CObArray, CFigure*> m_FigArray;

public:
	void AddFigure(CFigure *PFigure);
	CFigure *GetFigure(int Index);
	int GetNumFigs();

protected: // create from serialization only
	CMiniDrawDoc() noexcept;
	DECLARE_DYNCREATE(CMiniDrawDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CMiniDrawDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual void DeleteContents();
	afx_msg void OnEditClearAll();
	afx_msg void OnUpdateEditClearAll(CCmdUI *pCmdUI);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
};
