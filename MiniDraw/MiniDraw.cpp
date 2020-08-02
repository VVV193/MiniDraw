
// MiniDraw.cpp : Defines the class behaviors for the application.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MiniDraw.h"
#include "MainFrm.h"

#include "MiniDrawDoc.h"
#include "MiniDrawView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMiniDrawApp

BEGIN_MESSAGE_MAP(CMiniDrawApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CMiniDrawApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_TOOLS_CIRCLE, &CMiniDrawApp::OnToolsCircle)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CIRCLE, &CMiniDrawApp::OnUpdateToolsCircle)
	ON_COMMAND(ID_TOOLS_CIRCLEFILL, &CMiniDrawApp::OnToolsCirclefill)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_CIRCLEFILL, &CMiniDrawApp::OnUpdateToolsCirclefill)
	ON_COMMAND(ID_TOOLS_LINE, &CMiniDrawApp::OnToolsLine)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_LINE, &CMiniDrawApp::OnUpdateToolsLine)
	ON_COMMAND(ID_TOOLS_RECTANGLE, &CMiniDrawApp::OnToolsRectangle)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_RECTANGLE, &CMiniDrawApp::OnUpdateToolsRectangle)
	ON_COMMAND(ID_TOOLS_RECTFILL, &CMiniDrawApp::OnToolsRectfill)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_RECTFILL, &CMiniDrawApp::OnUpdateToolsRectfill)
	ON_COMMAND(ID_TOOLS_RECTROUND, &CMiniDrawApp::OnToolsRectround)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_RECTROUND, &CMiniDrawApp::OnUpdateToolsRectround)
	ON_COMMAND(ID_TOOLS_RECTROUNDFILL, &CMiniDrawApp::OnToolsRectroundfill)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_RECTROUNDFILL, &CMiniDrawApp::OnUpdateToolsRectroundfill)
	ON_COMMAND(ID_LINE_DOUBLE, &CMiniDrawApp::OnLineDouble)
	ON_UPDATE_COMMAND_UI(ID_LINE_DOUBLE, &CMiniDrawApp::OnUpdateLineDouble)
	ON_COMMAND(ID_LINE_SINGLE, &CMiniDrawApp::OnLineSingle)
	ON_UPDATE_COMMAND_UI(ID_LINE_SINGLE, &CMiniDrawApp::OnUpdateLineSingle)
	ON_COMMAND(ID_LINE_TRIPLE, &CMiniDrawApp::OnLineTriple)
	ON_UPDATE_COMMAND_UI(ID_LINE_TRIPLE, &CMiniDrawApp::OnUpdateLineTriple)
END_MESSAGE_MAP()


// CMiniDrawApp construction

CMiniDrawApp::CMiniDrawApp() noexcept
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MiniDraw.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CMiniDrawApp object

CMiniDrawApp theApp;


// CMiniDrawApp initialization

BOOL CMiniDrawApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMiniDrawDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMiniDrawView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	EnableShellOpen();
	RegisterShellFileTypes();

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	m_pMainWnd->DragAcceptFiles();
	return TRUE;
}

// CMiniDrawApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CMiniDrawApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CMiniDrawApp message handlers

void CMiniDrawApp::OnLineDouble()
{
	// TODO: Add your command handler code here
	m_CurrentThickness = 2;
}

void CMiniDrawApp::OnUpdateLineDouble(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_CurrentThickness == 2 ? 1 : 0);
}

void CMiniDrawApp::OnLineSingle()
{
	// TODO: Add your command handler code here
	m_CurrentThickness = 1;
}

void CMiniDrawApp::OnUpdateLineSingle(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_CurrentThickness == 1 ? 1 : 0);
}

void CMiniDrawApp::OnLineTriple()
{
	// TODO: Add your command handler code here
	m_CurrentThickness = 3;
}

void CMiniDrawApp::OnUpdateLineTriple(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_CurrentThickness == 3 ? 1 : 0);
}

void CMiniDrawApp::OnToolsCircle()
{
	// TODO: Add your command handler code here
	m_CurrentTool = ID_TOOLS_CIRCLE;
}

void CMiniDrawApp::OnUpdateToolsCircle(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_CurrentTool == ID_TOOLS_CIRCLE ? 1 : 0);
}

void CMiniDrawApp::OnToolsCirclefill()
{
	// TODO: Add your command handler code here
	m_CurrentTool = ID_TOOLS_CIRCLEFILL;
}

void CMiniDrawApp::OnUpdateToolsCirclefill(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_CurrentTool == ID_TOOLS_CIRCLEFILL ? 1 : 0);
}

void CMiniDrawApp::OnToolsLine()
{
	// TODO: Add your command handler code here
	m_CurrentTool = ID_TOOLS_LINE;
}

void CMiniDrawApp::OnUpdateToolsLine(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_CurrentTool == ID_TOOLS_LINE ? 1 : 0);
}

void CMiniDrawApp::OnToolsRectangle()
{
	// TODO: Add your command handler code here
	m_CurrentTool = ID_TOOLS_RECTANGLE;
}

void CMiniDrawApp::OnUpdateToolsRectangle(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_CurrentTool == ID_TOOLS_RECTANGLE ? 1 : 0);
}

void CMiniDrawApp::OnToolsRectfill()
{
	// TODO: Add your command handler code here
	m_CurrentTool = ID_TOOLS_RECTFILL;
}

void CMiniDrawApp::OnUpdateToolsRectfill(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_CurrentTool == ID_TOOLS_RECTFILL ? 1 : 0);
}

void CMiniDrawApp::OnToolsRectround()
{
	// TODO: Add your command handler code here
	m_CurrentTool = ID_TOOLS_RECTROUND;
}

void CMiniDrawApp::OnUpdateToolsRectround(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_CurrentTool == ID_TOOLS_RECTROUND ? 1 : 0);
}

void CMiniDrawApp::OnToolsRectroundfill()
{
	// TODO: Add your command handler code here
	m_CurrentTool = ID_TOOLS_RECTROUNDFILL;
}

void CMiniDrawApp::OnUpdateToolsRectroundfill(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_CurrentTool == ID_TOOLS_RECTROUNDFILL ? 1 : 0);
}

