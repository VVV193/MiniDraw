
// MiniDrawDoc.h : interface of the CMiniDrawDoc class
//


#pragma once
class CLine : public CObject
{
protected:
	int m_X1, m_Y1, m_X2, m_Y2;
public:
	CLine(int X1, int Y1, int X2, int Y2)
	{
		m_X1 = X1;
		m_Y1 = Y1;
		m_X2 = X2;
		m_Y2 = Y2;
	}
	void Draw(CDC *PDC);
};

class CMiniDrawDoc : public CDocument
{
protected:
	CTypedPtrArray <CObArray, CLine*> m_LineArray;
public:
	void AddLine(int X1, int Y1, int X2, int Y2);
	CLine *GetLine(int Index);
	int GetNumLines();

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
