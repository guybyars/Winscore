//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Winscore.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Winscore.h"
#include "wsversion.h"
#include "afxinet.h"

#include "MainFrm.h"
#include "WinscoreDoc.h"
#include "ListVwEx.h"
#include "WinscoreView.h"
#include "wssingledoctemplate.h"
#include "Splash.h"
#include "Global_Prototypes.h"
//#include "RegistrationDlg.h"
#include "bitmappicture.h"
#include "ModulVer.h"

#include <locale.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CString TryUrl(CString url);
void CheckLaterVersion();

/////////////////////////////////////////////////////////////////////////////
// CWinscoreApp

BEGIN_MESSAGE_MAP(CWinscoreApp, CWinApp)
	//{{AFX_MSG_MAP(CWinscoreApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_HELP_INDEX, OnHelpIndex)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_HELP_WINSCOREMANUAL, OnHelpWinscoremanual)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_HELP_FINDER, CWinApp::OnHelpFinder)
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinscoreApp construction

CWinscoreApp::CWinscoreApp()
{

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWinscoreApp object

CWinscoreApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWinscoreApp initialization

BOOL CWinscoreApp::InitInstance()
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

	CWinAppEx::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(_T("OLE Initialization Failed"));
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);


#ifdef WINSCOREBETA
AfxMessageBox(_T("Winscore Beta version.  Evaluation only, not to be used for actual contest scoring."));
#endif

	HANDLE hMutex = CreateMutex(NULL,FALSE,_T("Winscore"));
	if (GetLastError() == ERROR_ALREADY_EXISTS) 
		{
		if( AfxMessageBox( _T("Winscore is already running, create another instance?"), MB_YESNOCANCEL  )==IDNO ) return FALSE;
		}
	if( hMutex!= NULL ) ReleaseMutex(hMutex);


	// CG: The following block was inserted by 'Status Bar' component.
	{
		//Set up date and time defaults so they're the same as system defaults
		setlocale(LC_ALL, "");
	}

	// CG: The following block was added by the Splash Screen component.
	{
		CCommandLineInfo cmdInfo;

		ParseCommandLine(cmdInfo);

		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	}
	AfxEnableControlContainer();

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Winscore App"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	SetDefaultDayToday();


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CWSSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CWSSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CWinscoreDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CWinscoreView));
	AddDocTemplate(pDocTemplate);

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	CoInitialize(NULL);

	// If no file was specified on the command line, use the last one.
	if( cmdInfo.m_strFileName.GetLength()==0 )
		{
		CFileStatus rStatus;
		// Get the name of the last used file from the registry
		CString strName=GetProfileString( REGSECTION, REGCONTEST ); 
		if( strName.GetLength()>0 && CFile::GetStatus(strName, rStatus) )
			{
			try
				{
				OpenDocumentFile( strName );
				}
			catch (...)
				{
				return TRUE;
				}
			}
		}


	
	CheckLaterVersion();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CBitmapPicture	m_cPicture;
	CString	m_cVersionText;
	CString	m_strName;
	CString	m_strNumber;
	CString	m_strRegistration;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() :  CDialogEx(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	m_cVersionText = _T("Winscore Version: ");
	m_strName = _T("");
	m_strNumber = _T("");
	m_strRegistration = _T("");
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_PICTURE, m_cPicture);
	DDX_Text(pDX, IDC_VERSION_TEXT, m_cVersionText);
	DDX_Text(pDX, IDC_REG_NAME, m_strName);
	DDX_Text(pDX, IDC_REG_NUMBER, m_strNumber);
	DDX_Text(pDX, IDC_REG1, m_strRegistration);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CWinscoreApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CWinscoreApp commands

BOOL CWinscoreApp::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following lines were added by the Splash Screen component.
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}

void CWinscoreApp::OnHelpIndex() 
{
	CWinApp::OnHelpIndex();
	
}


void CWinscoreApp::OnAppExit() 
{
	//  Application exit,

	//RemindAboutScores();

	CWinApp::OnAppExit();

}

int CWinscoreApp::ExitInstance() 
{
	return CWinApp::ExitInstance();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();
	
	m_cPicture.SetBitmap(IDB_SMALLSPLASH);

	m_cVersionText+= WINSCOREVERSION;
	m_cVersionText+= _T("\nRevision Date: ");
	m_cVersionText+= __DATE__;
	
//	CString strRegName=AfxGetApp()->GetProfileString( REGSECTION, REGNAMENAME, _T("") ); 
//	int		iRegNumber=AfxGetApp()->GetProfileInt( REGSECTION, REGNUMBER, 0 ); 

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CWinscoreApp::OnHelpWinscoremanual() 
{
CWaitCursor cWait;

ShellExecute(NULL, _T("open"), _T("Winscore User's Guide.pdf"), NULL, NULL, SW_SHOWNORMAL);
}


void CheckLaterVersion()
	{

	if( AfxGetApp()->GetProfileInt(	REGSECTION, CHECKNEWVERSION,  1 )==0 ) return;

	CModuleVersion cVer;
	cVer.GetFileVersionInfo("winscore.exe");
	CString strVersion=cVer.GetValue("FileVersion");
	if( strVersion.GetLength()<=0 ) return;

	CString strURL="http://";
	strURL+=WINSCOREWEB;
	strURL+="//wsversion.txt";
	CString strLatest=TryUrl(strURL);
	if( strLatest.GetLength()<=0 ) return;

	//Check for valid response.
	if( strLatest.Find("20")!=0 ) return;

	if( strLatest > strVersion ) 
		{
		if(AfxGetApp()->GetProfileInt(REGSECTION, strLatest, 0)==0 ) //Check if we have already nagged.
			{ // We haven't advised, nag now.
			CString strMess;
			strMess.Format("A later version of Winscore, %s, is available from http://www/gfbyars/com/winscore", strLatest);
			AfxMessageBox(strMess, MB_OK | MB_ICONINFORMATION);
			AfxGetApp()->WriteProfileInt(REGSECTION, strLatest,	1 );  //Set the flag so we don't continue to nag about this one.
			}
		}
	}


CString TryUrl(CString url)
{

	CInternetSession session;
	CInternetFile* file = NULL;
	CString line;

	try	{		
		file = (CInternetFile*) session.OpenURL(url);
		}
	catch (CInternetException* pEx) 
		{
		//if anything went wrong, just set file to NULL
		file = NULL;
		pEx->Delete();
		return "";
		}

	if (file) 
		{
		file->ReadString(line);
		}

	// cleanup
	if (file) 
		{ 
		file->Close(); 
		delete file;	
		}
	return line;

}