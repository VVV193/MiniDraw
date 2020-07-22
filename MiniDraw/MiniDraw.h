
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


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMiniDrawApp theApp;
