
// MiniDraw.h : main header file for the MiniDraw application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMiniDrawApp:
// See MiniDraw.cpp for the implementation of this class
//

class CMiniDrawApp : public CWinApp
{
public:
	CMiniDrawApp() noexcept;

public:
	int m_CurrentThickness;
	UINT m_CurrentTool;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnToolsCircle();
	afx_msg void OnUpdateToolsCircle(CCmdUI *pCmdUI);
	afx_msg void OnToolsCirclefill();
	afx_msg void OnUpdateToolsCirclefill(CCmdUI *pCmdUI);
	afx_msg void OnToolsLine();
	afx_msg void OnUpdateToolsLine(CCmdUI *pCmdUI);
	afx_msg void OnToolsRectangle();
	afx_msg void OnUpdateToolsRectangle(CCmdUI *pCmdUI);
	afx_msg void OnToolsRectfill();
	afx_msg void OnUpdateToolsRectfill(CCmdUI *pCmdUI);
	afx_msg void OnToolsRectround();
	afx_msg void OnUpdateToolsRectround(CCmdUI *pCmdUI);
	afx_msg void OnToolsRectroundfill();
	afx_msg void OnUpdateToolsRectroundfill(CCmdUI *pCmdUI);
	afx_msg void OnLineDouble();
	afx_msg void OnUpdateLineDouble(CCmdUI *pCmdUI);
	afx_msg void OnLineSingle();
	afx_msg void OnUpdateLineSingle(CCmdUI *pCmdUI);
	afx_msg void OnLineTriple();
	afx_msg void OnUpdateLineTriple(CCmdUI *pCmdUI);
};

extern CMiniDrawApp theApp;
