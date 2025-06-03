//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// RListVw.cpp : implementation of the CWinscoreView class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.


#include "stdafx.h"
#include "Winscore.h"
#include "WinscoreDoc.h"
#include "ListVwEx.h"
#include "WinscoreView.h"
#include "MainFrm.h"
#include "ResizingDialog.h"
#include "AddTurnpointDlg.h"
#include "ContestantDlg.h"
#include "ContestInfoDlg.h"
#include "Task.h"
#include "TaskList.h"
#include "TaskDlg.h"
#include "ExportScoresheetDlg.h"
#include "CalculateDlg.h"
#include "PenalityDlg.h"
#include "GridPositionsDlg.h"
#include "scoreserverdlg.h"
#include "Global_Prototypes.h"
#include "resizingdialog.h"
#include "PlannerDlg.h"
#include "WinscoreFonts.h"
#include "ScoreRecordList.h"
#include "ImportDataDlg.h"
#include <fstream>
#include "DontShowDlg.h"
#include "AutoExport.h"
#include "ContestantsFromSSA.h"
#include "Contestant1Dlg.h"
#include "AutoScoreDlg.h"
#include "warning.h"
#include "progressbar.h"
#include "SelectClassDlg.h"
#include "GliderInfoList.h"
#include "NoLogFoundStatus.h"
#include "autosaveoptionsdlg.h"


#include <atlbase.h>

#include "taskinfodlg.h"
#include "ImportFlightsDlg.h"
#include "FDRSettingsPropSheet.h"
#include "ExportFlightFileDlg.h"
#include "DescriptionDlg.h"

#include "CopyScoresheetdlg.h"
#include "ShowWarningsDlg.h"
#include "EditFlightDlg.h"
#include "FlightAnalysisDlg.h"
#include "FlightDisplayDlg.h"
#include "PrintGridDlg.h"
#include "DetailedScoreInfoDlg.h"
#include "XMLMgr.h"
#include "IGCDirectories.h"


	// base class for CWinscoreView

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static  EClass				ePrintClass=eStandard;
static  CTime				cPrintDate= CTime::GetCurrentTime();
//static 	EScoresheetStatus	ePrintStatus=ePrelimStatus;
static  EPrintSelection		ePrintSelection=eDailyScoresheet;

static int nRecords=0, nRecordsPrinted=0;
static CScoreRecordList *pcScoreRecordList=NULL;
static POSITION posScoreLine[2];
static double dPrevScore=0.0;
static int iPrevStanding=(-1);
static int iStanding=0; 
static void* pItemPreselect=NULL;
static int	nCodes=0;
static CStringArray     cCodeArray;
static CStringArray     cReasonArray;


#define	PAGE_WIDTH  780


/////////////////////////////////////////////////////////////////////////////
// CWinscoreView

IMPLEMENT_DYNCREATE(CWinscoreView, CListViewEx)

BEGIN_MESSAGE_MAP(CWinscoreView, CListViewEx)
	//{{AFX_MSG_MAP(CWinscoreView)
	ON_COMMAND(ID_CONTEST_INFO_CONTESTANTS, OnContestInfoContestants)
	ON_COMMAND(ID_CONTEST_INFO_TURNPOINT, OnContestInfoTurnpoint)
	ON_COMMAND(ID_CONTEST_INFO_SETUP, OnContestInfoSetup)
	ON_COMMAND(ID_VIEW_TURNPOINT_GRID, OnViewTurnpointGrid)
	ON_COMMAND(ID_DAILYINFO_TASK, OnDailyinfoTask)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_COMMAND(ID_DAILYINFO_CALCULATESCORES, OnDailyinfoCalculatescores)
	ON_COMMAND(ID_DAILY_PENALTIES, OnDailyPenalties)
	ON_UPDATE_COMMAND_UI(ID_CONTEST_INFO_TURNPOINT, OnUpdateContestInfoTurnpoint)
	ON_UPDATE_COMMAND_UI(ID_CONTEST_INFO_CONTESTANTS, OnUpdateContestInfoContestants)
	ON_UPDATE_COMMAND_UI(ID_CONTEST_INFO_SETUP, OnUpdateContestInfoSetup)
	ON_UPDATE_COMMAND_UI(ID_DAILY_PENALTIES, OnUpdateDailyPenalties)
	ON_UPDATE_COMMAND_UI(ID_DAILYINFO_CALCULATESCORES, OnUpdateDailyinfoCalculatescores)
	ON_UPDATE_COMMAND_UI(ID_DAILYINFO_LANDINGCARDS, OnUpdateDailyinfoLandingcards)
	ON_UPDATE_COMMAND_UI(ID_DAILYINFO_TASK, OnUpdateDailyinfoTask)
	ON_UPDATE_COMMAND_UI(ID_DAILYINFO_TIMES, OnUpdateDailyinfoTimes)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_PREVIEW, OnUpdateFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_SCORESHEET, OnUpdateFileExportScoresheet)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TURNPOINT_GRID, OnUpdateViewTurnpointGrid)
	ON_COMMAND(ID_FILE_EXPORT_SCORESHEET, OnFileExportScoresheet)
	ON_COMMAND(ID_CONTEST_INFO_TAKEOFF_GRID, OnContestInfoTakeoffGrid)
	ON_UPDATE_COMMAND_UI(ID_CONTEST_INFO_TAKEOFF_GRID, OnUpdateContestInfoTakeoffGrid)
	ON_UPDATE_COMMAND_UI(ID_CONTEST_INFO_INVALID_TPT, OnUpdateContestInfoInvalidTpt)
	ON_COMMAND(ID_FILE_IMPORT_EVENTS, OnFileImportEvents)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT_EVENTS, OnUpdateFileImportEvents)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_FILE_EXPORT_CONTESTANTS, OnFileExportContestants)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_CONTESTANTS, OnUpdateFileExportContestants)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT_DIRECT, OnUpdateFilePrintDirect)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrintDirect)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, OnUpdateFilePrint)
	ON_COMMAND(ID_DAILY_TASKPLANNER, OnDailyTaskplanner)
	ON_UPDATE_COMMAND_UI(ID_DAILY_TASKPLANNER, OnUpdateDailyTaskplanner)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_FILE_IMPORT_FLIGHTS, OnFileImportFlights)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT_FLIGHTS, OnUpdateFileImportFlights)
	ON_COMMAND(ID_FLIGHTLOGS_ANALYZEALL, OnFlightlogsAnalyzeall)
	ON_COMMAND(ID_FLIGHTLOGS_VIEWIGCFILE, OnFlightlogsViewigcfile)
	ON_UPDATE_COMMAND_UI(ID_FLIGHTLOGS_VIEWIGCFILE, OnUpdateFlightlogsViewigcfile)
	ON_COMMAND(ID_FLIGHTLOGS_DELETE, OnFlightlogsDelete)
	ON_UPDATE_COMMAND_UI(ID_FLIGHTLOGS_DELETE, OnUpdateFlightlogsDelete)
	ON_UPDATE_COMMAND_UI(ID_FLIGHTLOGS_ANALYZEALL, OnUpdateFlightlogsAnalyzeall)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_SET_FDR_ID, OnSetFDRID)
	ON_UPDATE_COMMAND_UI(ID_SET_FDR_ID, OnUpdateSetFDRID)
	ON_COMMAND(ID_EXPORT_TO_TASK_LIBRARY, OnExportTaskToLibrary)
	ON_COMMAND(ID_EXPORT_TASK_TO_CUP, OnExportTaskToCUP)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_FLIGHTLOGS_SETTINGS, OnFlightlogsSettings)
	ON_COMMAND(ID_FLIGHTLOGS_VIEWWARNINGS, OnFlightlogsViewwarnings)
	ON_UPDATE_COMMAND_UI(ID_FLIGHTLOGS_VIEWWARNINGS, OnUpdateFlightlogsViewwarnings)
	ON_COMMAND(ID_FLIGHTLOGS_ANALYZEFLIGHT, OnFlightlogsAnalyzeflight)
	ON_UPDATE_COMMAND_UI(ID_FLIGHTLOGS_ANALYZEFLIGHT, OnUpdateFlightlogsAnalyzeflight)
	ON_COMMAND(ID_FLIGHTLOGS_CHECKSECURITY, OnFlightlogsCheckSecurity)
	ON_UPDATE_COMMAND_UI(ID_FLIGHTLOGS_CHECKSECURITY, OnUpdateFlightlogsCheckSecurity)
	ON_COMMAND(ID_FLIGHTLOGS_EDITFLIGHTRESLTS, OnFlightlogsEditflightreslts)
	ON_UPDATE_COMMAND_UI(ID_FLIGHTLOGS_EDITFLIGHTRESLTS, OnUpdateFlightlogsEditflightreslts)
	ON_COMMAND(ID_FLIGHTLOGS_ANALYZENEW, OnFlightlogsAnalyzenew)
	ON_UPDATE_COMMAND_UI(ID_FLIGHTLOGS_ANALYZENEW, OnUpdateFlightlogsAnalyzenew)
	ON_COMMAND(ID_FLIGHTLOGS_VIEWANALYSIS, OnFlightlogsViewanalysis)
	ON_UPDATE_COMMAND_UI(ID_FLIGHTLOGS_VIEWANALYSIS, OnUpdateFlightlogsViewanalysis)
	ON_COMMAND(ID_FLIGHTLOGS_REFRESH, OnFlightlogsRefresh)
	ON_UPDATE_COMMAND_UI(ID_FLIGHTLOGS_REFRESH, OnUpdateFlightlogsRefresh)
	ON_COMMAND(ID_FLIGHTLOGS_DISPLAY, OnFlightlogsDisplay)
	ON_UPDATE_COMMAND_UI(ID_FLIGHTLOGS_DISPLAY, OnUpdateFlightlogsDisplay)
	ON_COMMAND(ID_FILE_EXPORT_WINSCOREFLIGHTFILE, OnFileExportWinscoreflightfile)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_WINSCOREFLIGHTFILE, OnUpdateFileExportWinscoreflightfile)
	ON_COMMAND(ID_HELP_REGISTER, OnHelpRegister)
	ON_UPDATE_COMMAND_UI(ID_HELP_REGISTER, OnUpdateHelpRegister)
	ON_WM_TIMER()
	ON_COMMAND(ID_FLIGHTLOGS_VIEW, OnFlightlogsView)
	ON_UPDATE_COMMAND_UI(ID_FLIGHTLOGS_VIEW, OnUpdateFlightlogsView)
	ON_COMMAND(ID_ANALYZE_INFO, OnAnalyzeInfo)
	ON_UPDATE_COMMAND_UI(ID_ANALYZE_INFO, OnUpdateAnalyzeInfo)
	ON_COMMAND(ID_ANALYZE_SECURITY_MARKASFAILED, OnAnalyzeSecurityMarkasfailed)
	ON_COMMAND(ID_ANALYZE_SECURITY_MARKASPASSED, OnAnalyzeSecurityMarkaspassed)
	ON_WM_DESTROY()
	ON_COMMAND(ID_ANALYZE_DETAILEDINFO, OnAnalyzeDetailedinfo)
	ON_UPDATE_COMMAND_UI(ID_ANALYZE_DETAILEDINFO, OnUpdateAnalyzeDetailedinfo)
	ON_COMMAND(ID_FILE_EXPORT_CP_XML, OnFileExportCpXml)
	ON_COMMAND(ID_FILE_EXPORT_CONTESTANTSINXMLFORMAT, OnFileExportContestantsinxmlformat)
	ON_COMMAND(ID_FILE_IMPORT_CONTROLPOINTSINXMLFORMAT, OnFileImportControlpointsinxmlformat)
	ON_COMMAND(ID_FILE_IMPORT_XML_CONTESTANTS, OnFileImportXmlContestants)
	ON_COMMAND(ID_SCORESHEET_SERVER, OnScoresheetServer)
	ON_COMMAND(ID_FILE_EXPORT_XML_ENTIRECONTESTFILE, OnFileExportXmlEntirecontestfile)
	ON_COMMAND(ID_FILE_EXPORT_AUTOEXPORTSCORESHEETTOXML, OnFileExportAutoexportscoresheettoxml)
	ON_COMMAND(ID_FILE_IMPORT_XML_ENTIRECONTEST, OnFileImportXmlEntirecontest)
	ON_COMMAND(ID_IMPORT_FLASHCARD, OnImportFlashcard)
	ON_UPDATE_COMMAND_UI(ID_IMPORT_FLASHCARD, OnUpdateImportFlashcard)
	ON_COMMAND(ID_SSA_SERVER, OnSsaServer)
	ON_UPDATE_COMMAND_UI(ID_SSA_SERVER, OnUpdateSsaServer)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT_CONTROLPOINTSINXMLFORMAT, OnUpdateFileImportControlpointsinxmlformat)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_CP_XML, OnUpdateFileExportCpXml)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_XML_ENTIRECONTESTFILE, OnUpdateFileExportXmlEntirecontestfile)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT_XML_CONTESTANTS, OnUpdateFileImportXmlContestants)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_CONTESTANTSINXMLFORMAT, OnUpdateFileExportContestantsinxmlformat)
	ON_COMMAND(ID_FILE_IMPORT_CONTROLPOINTSINCAIDATFORMAT, OnFileImportControlpointsincaidatformat)
	ON_UPDATE_COMMAND_UI(ID_FILE_IMPORT_CONTROLPOINTSINCAIDATFORMAT, OnUpdateFileImportControlpointsincaidatformat)
	//}}AFX_MSG_MAP
	// Standard printing commands
//	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID__IMPORTFROMSSA, &CWinscoreView::OnImportfromssa)
ON_UPDATE_COMMAND_UI(ID__IMPORTFROMSSA, &CWinscoreView::OnUpdateImportfromssa)
ON_COMMAND(ID_DELETE_CONTESTANT, &CWinscoreView::OnDeleteContestant)
ON_COMMAND(ID_CONTESTANT_EDIT1, &CWinscoreView::OnEdit1Contestant)
ON_COMMAND(ID__ADDCONTESTANT, &CWinscoreView::OnAddContestant)
ON_COMMAND(ID_IMPORT_HANDICAPLISTFROMSSA, &CWinscoreView::OnImportHandicaplistfromssa)
ON_COMMAND(ID_EXPORT_HANDICAPSUMMARYTOHTML, &CWinscoreView::OnExportHandicapsummarytoHTML)
ON_COMMAND(ID_DAILY_AUTOSCORING, &CWinscoreView::OnDailyAutoscoring)
ON_COMMAND(ID_FLIGHTLOGS_RECHECKLATESTSTART, &CWinscoreView::OnFlightlogsRechecklateststart)
ON_UPDATE_COMMAND_UI(ID_FLIGHTLOGS_RECHECKLATESTSTART, &CWinscoreView::OnUpdateFlightlogsRechecklateststart)
ON_COMMAND(ID_EXPORT_ALLTASKSTOXMLFILE, &CWinscoreView::OnExportAlltaskstoxmlfile)
ON_COMMAND(ID_CHANGECLASS, &CWinscoreView::OnChangeClass)
ON_UPDATE_COMMAND_UI(ID_CHANGECLASS, &CWinscoreView::OnUpdateChangeclass)
ON_COMMAND(ID_NOLOGSTATUS_DIDNOTLAUNCH, &CWinscoreView::OnNologstatusDidnotlaunch)
ON_COMMAND(ID_NOLOGSTATUS_LANDOUTREPORTED, &CWinscoreView::OnNologstatusLandoutreported)
ON_COMMAND(ID_NOLOGSTATUS_NOLOGFOUND, &CWinscoreView::OnNologstatusNologfound)
ON_COMMAND(ID_NOLOGSTATUS_CUSTOM, &CWinscoreView::OnNologstatusCustom)
ON_UPDATE_COMMAND_UI(ID_NOLOGSTATUS_DIDNOTLAUNCH, &CWinscoreView::OnUpdateNologstatus)
ON_UPDATE_COMMAND_UI(ID_NOLOGSTATUS_LANDOUTREPORTED, &CWinscoreView::OnUpdateNologstatus)
ON_UPDATE_COMMAND_UI(ID_NOLOGSTATUS_NOLOGFOUND, &CWinscoreView::OnUpdateNologstatus)
ON_UPDATE_COMMAND_UI(ID_NOLOGSTATUS_CUSTOM, &CWinscoreView::OnUpdateNologstatus)
ON_COMMAND(ID_FILE_CHECKFORNEWVERSIONATSTARTUP, &CWinscoreView::OnFileCheckfornewversionatstartup)
ON_UPDATE_COMMAND_UI(ID_FILE_CHECKFORNEWVERSIONATSTARTUP, &CWinscoreView::OnUpdateFileCheckfornewversionatstartup)
ON_COMMAND(ID_Menu, &CWinscoreView::OnAutomaticSaveOptions)
ON_COMMAND(ID_LOGSTATUS_CHECKEDANDOK, &CWinscoreView::OnLogstatusCheckedandok)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinscoreView construction/destruction

CWinscoreView::CWinscoreView()
	{
	m_eViewType=eContestantView;
	for( int i=0; i<NVIEWS; i++ ) m_iSortedColumn[i]=1;
	m_iSortedColumn[eScoreView]		=2;
	m_iSortedColumn[eContestantView]=1;
	m_pModlessAutoScoreDlg=NULL;
	}

CWinscoreView::~CWinscoreView()
{
}



/////////////////////////////////////////////////////////////////////////////
// CWinscoreView initialization

void CWinscoreView::OnInitialUpdate()
	{
	InitialUpdate();
	}

void CWinscoreView::InitialUpdate()
{
	CListViewEx::OnInitialUpdate();
	CListCtrl& ListCtrl = GetListCtrl();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();

	CWinscoreDoc* pDocument=GetDocument();
	if( pDocument->Valid() )
		{
		pFrame->LoadTreeCtrl(pDocument);
//		On initial update, always show the contestants
		//ViewContestantList();
		}
	else
		{
		pFrame->GreyAll();
		}
	return;
}


/////////////////////////////////////////////////////////////////////////////
// CWinscoreView diagnostics

#ifdef _DEBUG

CWinscoreDoc* CWinscoreView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinscoreDoc)));
	return (CWinscoreDoc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CWinscoreView message handlers

void CWinscoreView::ContestInfoTurnpoint() 
	{
	OnContestInfoTurnpoint();
	}

void CWinscoreView::OnContestInfoTurnpoint() 
	{

	CWinscoreDoc* pDocument=GetDocument();
	CTurnpointArray cTempTurnpointArray;
  
	CAddTurnpointDlg	dlg;

	cTempTurnpointArray = pDocument->m_turnpointArray;

	dlg.SetTurnpointArray(&cTempTurnpointArray);
	dlg.m_eCoordinateFormat=pDocument->m_eCoordinateFormat;
    dlg.m_bTasks=!pDocument->m_taskList.IsEmpty();
	dlg.m_pPreselect=(CTurnpoint*)pItemPreselect;


	if (dlg.DoModal() == IDOK)
		{
		pDocument->m_turnpointArray=cTempTurnpointArray;
		pDocument->SetModifiedFlag();
		ViewTurnpointList(); 
		}	
}

void CWinscoreView::ContestInfoContestants() 
	{
	OnContestInfoContestants();
	}

void CWinscoreView::OnContestInfoContestants() 
{
	// TODO: Add your command handler code here

	CWinscoreDoc* pDocument=GetDocument();
	CContestantList cTempContestantList;
  
	cTempContestantList = pDocument->m_contestantList;


//  Tell the scorer he can now get them online.
//	CDontShowDlg dsdlg("CONTESTANTS_FROM_SSA1","You can now get the contestant information directly from the SSA over the Internet.\nUse the \"Import From SSA...\" button at the bottom of the form.",
//						"Winscore",
//						IDI_INFORMATION);
//	dsdlg.DoModalConditional();

	CContestantDlg	dlg;

	dlg.SetDocument(pDocument);
	dlg.SetContestantList(&cTempContestantList);
	dlg.m_pPreselect=(CContestant*)pItemPreselect;

	if (dlg.DoModal() == IDOK)
		{
		pDocument->m_contestantList = cTempContestantList;
		pDocument->SetModifiedFlag();

		CWinscoreView::ViewContestantList();
		}	
}


void CWinscoreView::ViewContestantList() 
{
	SaveColumnWidths();

	CListCtrl& ListCtrl = GetListCtrl();
	CWinscoreDoc* pDocument=GetDocument();

  	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	pFrame->SetViewCombo(eContestantView);

	m_eViewType=eContestantView;

	DeleteColumnsItems();
	pDocument->m_contestantList.CreateControlColumns( ListCtrl );
	pDocument->m_contestantList.LoadContestantList( ListCtrl );
	ListCtrl.SortItems( CompareContestantName, m_iSortedColumn[m_eViewType] );
	RestoreColumnWidths();

	UpdateStatusLine();

	return;
	
}


void CWinscoreView::ViewTurnpointList() 
{
	SaveColumnWidths();
	CListCtrl& ListCtrl = GetListCtrl();
	CWinscoreDoc* pDocument=GetDocument();

  	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	pFrame->SetViewCombo(eTurnpointView);

	m_eViewType=eTurnpointView;

	DeleteColumnsItems();
	pDocument->m_turnpointArray.CreateControlColumns( ListCtrl,  pDocument->m_eCoordinateFormat);
	pDocument->m_turnpointArray.LoadTurnpointList( ListCtrl, pDocument->m_eCoordinateFormat );
	ListCtrl.SortItems( CompareTurnpoint, m_iSortedColumn[m_eViewType] );

	RestoreColumnWidths();

	UpdateStatusLine();

	return;
}

void CWinscoreView::OnContestInfoSetup() 
{	
	CWinscoreDoc* pDocument=GetDocument();
	if( pDocument->PutUpContestInfoDlg(TRUE) ) 
		{
		pDocument->SetModifiedFlag();
		CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
		pFrame->LoadTreeCtrl(pDocument);
		}
}


CWinscoreView* CWinscoreView::GetWinscoreView()
{
return this;
}

/*
void CWinscoreView::OnViewContestantlist()
{
ViewContestantList();
}
void CWinscoreView::OnViewTurnpointlist()
{
ViewTurnpointList();
}
*/
void CWinscoreView::ViewTurnpointGrid()
{
	SaveColumnWidths();
	CListCtrl& ListCtrl = GetListCtrl();
	CWinscoreDoc* pDocument=GetDocument();

  	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	pFrame->SetViewCombo(eTurnpointGridView);

	m_eViewType=eTurnpointGridView;

	DeleteColumnsItems();
	pDocument->m_turnpointArray.CreateGridControlColumns( ListCtrl );
	pDocument->m_turnpointArray.LoadTurnpointGridList( ListCtrl, pDocument->m_eUnits );
	RestoreColumnWidths();

	UpdateStatusLine();

	return;
}

void CWinscoreView::OnViewTurnpointGrid() 
{
ViewTurnpointGrid();
}

void CWinscoreView::ViewTasks() 
{
	SaveColumnWidths();
	CListCtrl& ListCtrl = GetListCtrl();
	CWinscoreDoc* pDocument=GetDocument();

  	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	pFrame->SetViewCombo(eTaskView);
	EClass eClass = pFrame->GetClassCombo();
	CTime  cDate = pFrame->GetDateCombo();

	m_eViewType=eTaskView;

	DeleteColumnsItems();
	pDocument->m_taskList.CreateControlColumns( ListCtrl );
	pDocument->m_taskList.LoadTaskList( ListCtrl, pDocument->m_eUnits, pDocument->m_turnpointArray, eClass, cDate);
	ListCtrl.SortItems( CompareTasks, m_iSortedColumn[m_eViewType] );
	RestoreColumnWidths();

	UpdateStatusLine();

	return;
}

void CWinscoreView::ViewFlightLogs(CTime cDate, EClass eClass, bool bPreContest)
{
	CSingleLock cLock(&m_cSem_UpdateFlightLogs);
	CIGCDirectories cIGCDirs;

	CTime cViewDate=cDate;
	int		nDays=1;

	SaveColumnWidths();
	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CListCtrl& ListCtrl = GetListCtrl();

	if( bPreContest )
		{
		cViewDate=pDocument->GetPreContestDate();
		nDays=pDocument->GetNumPreContestDays();
		pFrame->SetViewCombo(ePreContestView);
		m_eViewType=ePreContestView;
		}
	else
		{
		cViewDate=cDate;
		nDays=1;
		pFrame->SetViewCombo(eFlightLogView);
		m_eViewType=eFlightLogView;
		}


	// Freshen up the list from the flights
	pDocument->m_FlightList.LoadListFromIGC( cIGCDirs.GetFlightLogPath(0,bPreContest), 
											 cViewDate,
											 nDays,
											 pDocument->m_contestantList );

	DeleteColumnsItems();
	pDocument->m_FlightList.CreateControlColumns( ListCtrl, bPreContest );
	RestoreColumnWidths();
	pDocument->m_FlightList.LoadFlightList(	ListCtrl, cViewDate,  nDays, eClass, pDocument->m_contestantList, pDocument->m_eUnits, abs(m_iSortedColumn[eFlightLogView]), bPreContest );
	ListCtrl.SortItems( CompareFlight, 	m_iSortedColumn[eFlightLogView] );

	UpdateStatusLine();

}

void CWinscoreView::UpdateFlightLogs(CMainFrame* pFrame, CTime cDate, EClass eClass, CFlight *pcFlight)
{
	CSingleLock cLock(&m_cSem_UpdateFlightLogs);
	if( m_eViewType!=eFlightLogView) return ;
	
//  	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CTime cTimeCombo=pFrame->GetDateCombo();
	EClass eClassCombo=pFrame->GetClassCombo();
	if( cDate!=cTimeCombo || eClassCombo!=eClass ) return;

	CWinscoreDoc* pDocument=GetDocument();
	CListCtrl& ListCtrl = GetListCtrl();

	//Find this CID in the list

	// How many CID are in the list for this guy.

	int nInList=0;
	for(  int iItem = 0; iItem< ListCtrl.GetItemCount(); iItem++)
		{
		CString strCID=ListCtrl.GetItemText(iItem,0);
		if( strCID==pcFlight->m_strCID ) nInList++;
		}


	for(  int iItem = 0; iItem< ListCtrl.GetItemCount(); iItem++)
		{
		CString strCID=ListCtrl.GetItemText(iItem,0);
		if( strCID==pcFlight->m_strCID )
			{
			if( nInList==1 ) 
				{
			pcFlight->m_eUnits=pDocument->m_eUnits;

			if( m_eViewType==eFlightLogView )
				pcFlight->AddToList( ListCtrl, TRUE, iItem );
			else
				pcFlight->AddToPreContestList( GetListCtrl(), pDocument->m_contestantList.GetByContestNo(pcFlight->m_strCID), TRUE, iItem );

			RemoveWinscoreEntry(getNoLogKey(pcFlight->m_strCID, cDate));
			ListCtrl.UpdateWindow();
			break;
				}
			else
				{
		         CString strIGC=ListCtrl.GetItemText(iItem,14);
				 if(strIGC== pcFlight->m_strFileName ) 
					{
					pcFlight->m_eUnits=pDocument->m_eUnits;
					if( m_eViewType==eFlightLogView )
						pcFlight->AddToList( ListCtrl, TRUE, iItem );
					else
						pcFlight->AddToPreContestList( GetListCtrl(), pDocument->m_contestantList.GetByContestNo(pcFlight->m_strCID), TRUE, iItem );
					RemoveWinscoreEntry(getNoLogKey(pcFlight->m_strCID, cDate));
					ListCtrl.UpdateWindow();
					break;
					}
				}
			}
		}

//	UpdateStatusLine();

}

void CWinscoreView::OnDailyinfoTask() 
{
	ManageTasks();
}

void CWinscoreView::ManageTasks() 
{
	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CTaskDlg	dlg;

	// Check for save
	if( pDocument->SaveReminder() ) return;

	dlg.m_pcTaskList=&(pDocument->m_taskList);

	dlg.m_pDoc=pDocument;
	pItemPreselect=GetSelectedPtr();
	if( !pItemPreselect ) 
		dlg.m_pPreselect=pDocument->m_taskList.GetByDateClass( pFrame->GetDateCombo(),   pFrame->GetClassCombo() );
	else
		dlg.m_pPreselect=(CTask*)pItemPreselect;


	dlg.DoModal();
		{
		pDocument->SetModifiedFlag();
		RefreshScoreDisplay();	
		pFrame->SetDateCombo(dlg.m_cSelectedDate);
		}

	if( m_eViewType==eTaskView )
		ViewTasks();
	
}

void CWinscoreView::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	CListCtrl& ListCtrl = GetListCtrl();

	if( abs(m_iSortedColumn[m_eViewType])-1==pNMListView->iSubItem )
		{
		m_iSortedColumn[m_eViewType]=(-m_iSortedColumn[m_eViewType]);
		}
	else
		m_iSortedColumn[m_eViewType]=pNMListView->iSubItem+1;

	switch(m_eViewType)
		{
	case eContestantView:
		ListCtrl.SortItems( CompareContestantName, m_iSortedColumn[m_eViewType] );
		return;
	case eTurnpointView:
		ListCtrl.SortItems( CompareTurnpoint, m_iSortedColumn[m_eViewType] );
		return;
//	case eTimesView:
//		ListCtrl.SortItems( CompareEvents, m_iSortedColumn[m_eViewType] );
//		return;
	case eTaskView:
		ListCtrl.SortItems( CompareTasks, m_iSortedColumn[m_eViewType] );
		return;
	case eScoreView:
		ListCtrl.SortItems( CompareScores, m_iSortedColumn[m_eViewType] );
		return;		
	case eFlightLogView:
		ListCtrl.SortItems( CompareFlight, m_iSortedColumn[m_eViewType] );
		return;		
	case ePreContestView:
		ListCtrl.SortItems( ComparePreContestFlight, m_iSortedColumn[m_eViewType] );
		return;	
	}

	*pResult = 0;
}


void CWinscoreView::OnDailyinfoCalculatescores() 
{

	CCalculateDlg	dlg;
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();

	dlg.m_pDoc=GetDocument();

	dlg.DoModal();
	pFrame->SetViewCombo(eScoreView);
	m_eViewType=eScoreView;
	RefreshScoreDisplay();

}

void CWinscoreView::ViewScores(CTime cDate, EClass eClass)
{
	SaveColumnWidths();
	CListCtrl& ListCtrl = GetListCtrl();
	CWinscoreDoc* pDocument=GetDocument();
	CWinscoreListCtrl  cDum;
	CSummary cSummary;

	cDum.m_fReport=FALSE;
	pDocument->CalculateScores( cDate, eClass, cDum, cSummary );

	m_eViewType=eScoreView;

	DeleteColumnsItems();
	pDocument->m_ScoreRecordList.CreateControlColumns( ListCtrl, eClass, pDocument->m_eUnits );
	RestoreColumnWidths();

	if( pDocument->NoContestDay( cDate, eClass ) ) 
		{
		UpdateStatusLine();
		return;
		}

	CTask *pcTask=pDocument->m_taskList.GetByDateClass(cDate, eClass);

	pDocument->m_ScoreRecordList.LoadScoreRecordList(	ListCtrl, 
														cDate,
														eClass,
														pcTask,
														pDocument->m_contestantList,
														pDocument->m_PenalityList,
														pDocument->m_eUnits
														);
	ListCtrl.SortItems( CompareScores, m_iSortedColumn[eScoreView] );	
	UpdateStatusLine();

	pDocument->AutoExportXML(cDate);


}

void CWinscoreView::ShowScores(CTime cDate) 
{		
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	pFrame->SetViewCombo(eScoreView);
	pFrame->SetDateCombo(cDate);

	EClass eClass=pFrame->GetClassCombo();

	ViewScores( cDate, eClass);
}

void CWinscoreView::OnDailyPenalties() 
{
	CPenalityDlg	dlg;
	CPenalityList   cTempPenalityList;
	CWinscoreDoc* pDocument=GetDocument();
	cTempPenalityList=pDocument->m_PenalityList;
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();

	dlg.m_pDoc=pDocument;
	dlg.m_pcPenalityList=&cTempPenalityList;

	pItemPreselect=GetSelectedPtr();

	if( pItemPreselect )
		{
		if( m_eViewType==eContestantView)
			{
			CContestant *pContestant=(CContestant*)pItemPreselect;
			dlg.m_strPreselectCID=pContestant->m_strContestNo;
			}
		else if( m_eViewType==eFlightLogView)
			{
			CFlight *pFlight=(CFlight*)pItemPreselect;
			dlg.m_strPreselectCID=pFlight->m_strCID;
			dlg.m_cPreselectDate= pFrame->GetDateCombo();
			}
		else if( m_eViewType==eScoreView )
			{
			CScoreRecord *pRecord=(CScoreRecord*)pItemPreselect;
			dlg.m_strPreselectCID=pRecord->m_cContestNo;
			dlg.m_cPreselectDate= pFrame->GetDateCombo();
			}
		}

	if (dlg.DoModal() == IDOK)
		{
		pDocument->m_PenalityList=cTempPenalityList;
		pDocument->SetModifiedFlag();
		RefreshScoreDisplay();
		}
}

void CWinscoreView::RefreshScoreDisplay()
	{
	if( m_eViewType==eScoreView )
		{
		CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
		CTime cTime=pFrame->GetDateCombo();
		EClass eClass=pFrame->GetClassCombo();
		ViewScores( cTime, eClass);
		}
	}

void CWinscoreView::OnUpdateContestInfoTurnpoint(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );	
}

void CWinscoreView::OnUpdateContestInfoContestants(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );		
}

void CWinscoreView::OnUpdateContestInfoSetup(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );		
}

void CWinscoreView::OnUpdateDailyPenalties(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );		
}

void CWinscoreView::OnUpdateDailyinfoCalculatescores(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );		
}

void CWinscoreView::OnUpdateDailyinfoLandingcards(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );		
}

void CWinscoreView::OnUpdateDailyinfoTask(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );		
}

void CWinscoreView::OnUpdateDailyinfoTimes(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );		
}

void CWinscoreView::OnUpdateFilePrintPreview(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( IsPrintingAllowed() );
}

void CWinscoreView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );		
}

void CWinscoreView::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );		
}

void CWinscoreView::OnUpdateFileExportScoresheet(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );		
}

void CWinscoreView::OnUpdateViewTurnpointGrid(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );		
}

void CWinscoreView::OnFileExportScoresheet() 
{
	CExportScoresheetDlg	dlg;

	dlg.m_pDoc=GetDocument();
	dlg.DoModal();
	
}
void CWinscoreView::ContestInfoTakeoffGrid()
	{OnContestInfoTakeoffGrid();}

void CWinscoreView::OnContestInfoTakeoffGrid() 
{
	CGridPositionsDlg	dlg;
  	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();

	dlg.m_pDoc=GetDocument();

	if (dlg.DoModal() == IDOK)
		{
		pFrame->SetViewCombo(eTakeoffGridView);
		pFrame->SetClassCombo(dlg.m_eClass);
		ViewTakeoffGrid(dlg.m_eClass);
		GetDocument()->SetModifiedFlag();
		}
	
	UpdateStatusLine();
	
}

void CWinscoreView::OnUpdateContestInfoTakeoffGrid(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );		
}

void CWinscoreView::ViewTakeoffGrid(EClass eClass)
{
	SaveColumnWidths();
	LV_COLUMN lvc;
	int iItem=0,i=0;

	LV_ITEM lvi;
	lvi.iSubItem = 0;
	lvi.mask = 0;

	TCHAR cTempBuf[100];

	CListCtrl& ListCtrl = GetListCtrl();
	CWinscoreDoc* pDocument=GetDocument();

  	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	pFrame->SetViewCombo(eTakeoffGridView);
	m_eViewType=eTakeoffGridView;

	DeleteColumnsItems();

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	int nColumns=pDocument->m_caGridPositions[(int)eClass].GetSize();

	if( nColumns>0 )
		{
		lvc.pszText=_T("Position: ");
		lvc.cx =  100;
		}
	else
		{
		lvc.pszText=_T("Takeoff Grid not yet calculated.");
		lvc.cx =  500;
		}

	lvc.iSubItem = 0;
	lvc.fmt = LVCFMT_LEFT;
	ListCtrl.InsertColumn(0,&lvc);

	for ( i=0; i<nColumns; i++)
		{
		CString cHeaderText;
		cHeaderText.Format(_T(" %d"), i+1);
		lvc.iSubItem = i+1;
		strcpy_s( cTempBuf, cHeaderText);
		lvc.pszText=cTempBuf;
		lvc.cx =  60;
		ListCtrl.InsertColumn(i+1,&lvc);
		}

	if (nColumns==0 ) return;

	CStringArray cTempArray;
		
	cTempArray.Copy( pDocument->m_caGridPositions[(int)eClass] );

	for (  i=0; i<pDocument->m_iNumContestDays; i++)
		{ 
		lvi.iItem = iItem;
		ListCtrl.InsertItem(&lvi);
		CString cTemp;
		cTemp.Format(_T("Day %d"),i+1);
		ListCtrl.SetItemText(iItem,0,cTemp);

		for ( int j=0; j<nColumns; j++)
			{ 
			ListCtrl.SetItemText(iItem,j+1, cTempArray.GetAt(j));
			}
		Shuffle20Percent(cTempArray);

		iItem++;
		}
	UpdateStatusLine();

}
/*
void CWinscoreView::OnContestInfoInvalidTpt() 
{
	CInvalidLegDlg	dlg;
	CInvalidLegList  cTempList;
	CWinscoreDoc*   pDoc=GetDocument();

	cTempList=pDoc->m_InvalidLegList;

	dlg.m_pDoc=pDoc;
	dlg.m_pcInvalidLegList=&cTempList;

	if (dlg.DoModal() == IDOK)
		{
		pDoc->m_InvalidLegList=cTempList;
		ViewInvalidLegList();
		GetDocument()->SetModifiedFlag();
		}	
		
}
*/
void CWinscoreView::OnUpdateContestInfoInvalidTpt(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );	
}


void CWinscoreView::OnPrint(CDC* pDC, CPrintInfo* pnIfo) 
	{
	// TODO: Add your specialized code here and/or call the base class

	
	CWinscoreDoc* pDoc = GetDocument();
	
	if( ePrintSelection==eContestantInfo)
		pDoc->m_contestantList.DrawContestantList(	pDC, 
													pDoc->m_strContestName,
													pDoc->m_strLocation,
													pDoc->m_CContestStartDate, 
													pnIfo->m_nCurPage );
	else if( ePrintSelection==eTakeoffGridPositions)
		{
		DrawTakeoffGrid(pDC, 
						pDoc->m_strContestName,
						pDoc->m_strLocation,
						pDoc->m_CContestStartDate, 
						pnIfo->m_nCurPage);
		}
	else if( ePrintSelection==eTaskSheet)
		{
		CTask *pcTask=(CTask*)GetSelectedPtr();
		if( pcTask==NULL ) return;

		pDoc->DrawTaskInfoSheet(	pDC,
									pcTask,
									pDoc->m_strContestName,
									pDoc->m_strLocation );
		






		}
	else if( ePrintSelection==eTurnpointSummary )
		{
		pDoc->m_turnpointArray.DrawTurnpointList(	pDC, 
													pDoc->m_strContestName,
													pDoc->m_strLocation,
													pDoc->m_CContestStartDate, 
													pnIfo->m_nCurPage,
													pDoc->m_eCoordinateFormat);

		}
	else if( ePrintSelection==eDailyScoresheet )
		{
		int iYpos=5;

		// Arm the reminder ONLY if we are actually printing.
		if( !pnIfo->m_bPreview) ArmReminder();

		if( pDoc->DrawTitle(pDC, 
							cPrintDate,
							ePrintClass, 
							iYpos ) )
			{
			if( pnIfo->m_nCurPage==1 ) 
				{
				pDoc->DrawHeader(pDC, 
							 	 cPrintDate,
								 ePrintClass, 
								 &iYpos);
		//    Initialize the values that keep track of the standings from page
		//	  to page.
				dPrevScore=0.0;
				iPrevStanding=(-1);
				iStanding=0; 
				nRecordsPrinted=0;
				nCodes=0;
				cCodeArray.RemoveAll();
				cReasonArray.RemoveAll();

			// Draw grid, scores and any relevant penality codes
				POSITION pos=posScoreLine[0];
				pDoc->DrawDay(	pDC, 
								ePrintClass, 
								cPrintDate,
								pos,
								pcScoreRecordList,
								nRecordsPrinted,
								iYpos,
								dPrevScore,
								iPrevStanding,
								iStanding,
								nCodes,
								cCodeArray,
								cReasonArray );
				}
			else
				{
			// Draw grid, scores and any relevant penality codes
				double dPrevScoreTemp=dPrevScore;
				int iPrevStandingTemp=iPrevStanding;
				int iStandingTemp=iStanding;
				int nRecordsPrintedTemp=nRecordsPrinted;

				POSITION pos=posScoreLine[1];
				pDoc->DrawDay(	pDC, 
								ePrintClass, 
								cPrintDate,
								pos,
								pcScoreRecordList,
								nRecordsPrintedTemp,
								iYpos,
								dPrevScoreTemp,
								iPrevStandingTemp,
								iStandingTemp,
								nCodes,
								cCodeArray,
								cReasonArray );
				}

			}

		pDoc->DrawScoresheetTrailer(pDC);
		}
	}

void CWinscoreView::OnFilePrint() 
{

	if( !SetupPrintOptions() ) return;

	CView::OnFilePrint();	
}
void CWinscoreView::OnFilePrintPreview() 
{

	if( !SetupPrintOptions() ) return;

	CView::OnFilePrintPreview();

	return;
}


BOOL CWinscoreView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	int nPages=1;
    CWinscoreDoc* pDoc = GetDocument();

	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();

	if( ePrintSelection==eDailyScoresheet )
		{
		int nRecords=pDoc->m_ScoreRecordList.GetCount(pFrame->GetClassCombo(), pFrame->GetDateCombo());
		(nRecords<=NUM_SCORES_PER_PAGE)?(nPages=1):(nPages=2);
		}
	else if( ePrintSelection==eContestantInfo)
		{
		nPages=pDoc->m_contestantList.NumberSummaryPages();
		pDoc->m_contestantList.Sort(m_iSortedColumn[m_eViewType]);
		}
	else if( ePrintSelection==eTurnpointSummary )
		{
		nPages=pDoc->m_turnpointArray.NumberSummaryPages();
		}
	else if( ePrintSelection==eTakeoffGridPositions)
		{
		nPages=m_cGridData.iNumDaysToPrint;
		}
	else if( ePrintSelection==eTaskSheet )
		{
		nPages=1;
		}

	pInfo->SetMaxPage(nPages);  
	BOOL bRet = DoPreparePrinting(pInfo);	// default preparation
	pInfo->m_nNumPreviewPages = 1; 
	return bRet;

}

void CWinscoreView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	if( ePrintSelection==eDailyScoresheet )
		{
		pInfo->m_bContinuePrinting=(nRecordsPrinted<nRecords);
		}

	pDC->SetMapMode( MM_LOENGLISH );
	CListViewEx::OnPrepareDC(pDC, pInfo);
}

void CWinscoreView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	if( ePrintSelection==eDailyScoresheet )
		{
		delete pcScoreRecordList;
		}

	// Clean up any fonts used in the printing process
	DeleteFonts();
	
	CListViewEx::OnEndPrinting(pDC, pInfo);
}


void CWinscoreView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
    CWinscoreDoc* pDoc = GetDocument();

	if( ePrintSelection==eDailyScoresheet )
		{
		// Create the score record list, copy in the stuff for this
		// class and day, then set the position at the head.
		pcScoreRecordList = new CScoreRecordList;
		nRecords=pDoc->m_ScoreRecordList.CopySort(
											cPrintDate, 
											ePrintClass, 
											*pcScoreRecordList,
											eCumulative);

		posScoreLine[0]=pcScoreRecordList->GetHeadPosition();
		posScoreLine[1]=pcScoreRecordList->FindIndex(NUM_SCORES_PER_PAGE);
		}

	CListViewEx::OnBeginPrinting(pDC, pInfo);
}


void CWinscoreView::DrawTakeoffGrid(	CDC*	pDC, 
										CString &cContestName,
										CString &cContestLocation,
										CTime	&cContestStartDate, 
										int		iPage)
	{

	CWinscoreDoc* pDocument=GetDocument();

	CSize	 cTextSize;
	int iYpos=0;
	CString  cTitle=_T("Takeoff Grid Positions"), cText, cPage;
	int iRowSize	=m_cGridData.iPrintNumRows;
	int iLeftTab	=120;
	int iLeftMargin =10;
	int iXpos=iLeftMargin;

	int nTotalOutput=0;
	int nRows=0;
	int nTotalContestants=0;
	bool bAllSameDay=true;
	
	for( int iClass=0; iClass< m_cGridData.iPrintNumClasses; iClass++)
		{
		EClass eClass	=m_cGridData.aiPrintClasses[iClass];
		nTotalContestants+=pDocument->m_caGridPositions[(int)eClass].GetSize();

		for( int j=0; j< m_cGridData.iPrintNumClasses; j++)
			{
			if( m_cGridData.aiPrintDays[iClass]!=m_cGridData.aiPrintDays[j] ) bAllSameDay=false;
			}
		}

	SelectFont( pDC, 30, FW_BOLD, FALSE);

    pDC->SetTextAlign( TA_TOP| TA_CENTER );
    pDC->TextOut( GetPageWidth(pDC)/2, 0, cTitle, cTitle.GetLength() );
	cTextSize=pDC->GetTextExtent(cTitle);
	iYpos-=cTextSize.cy;
	iYpos-=5;

	SelectFont( pDC, 25, FW_BOLD, FALSE);
    pDC->SetTextAlign( TA_TOP| TA_LEFT );

	pDC->TextOut( iLeftMargin, iYpos, cContestName, cContestName.GetLength() );
	cTextSize=pDC->GetTextExtent(cContestName);
	iYpos-=cTextSize.cy;

	pDC->TextOut( iLeftMargin, iYpos, cContestLocation, cContestLocation.GetLength() );
	cTextSize=pDC->GetTextExtent(cContestLocation);
	iYpos-=cTextSize.cy;


	if( pDocument->GetNumClasses()==1 || bAllSameDay)
		{
		EClass eClass	=m_cGridData.aiPrintClasses[0];
		int iContestDay	=m_cGridData.aiPrintDays[0]+iPage-1;

		CString cText;
		cText.Format(_T("Day %d"),iContestDay);
		pDC->TextOut( iLeftMargin, iYpos, cText, cText.GetLength() );
		cTextSize=pDC->GetTextExtent(cText);
		iYpos-=cTextSize.cy;
		}
	else
		{
		iXpos=iLeftMargin;
		for( int iClass=0; iClass< m_cGridData.iPrintNumClasses; iClass++)
			{
			EClass eClass	=m_cGridData.aiPrintClasses[iClass];
			CWSClass &cClass=GetClass(eClass);
			int iContestDay	=m_cGridData.aiPrintDays[iClass]+iPage-1;

			CString cClassText;
			cClassText.Format(_T("%s Day %d"), cClass.GetName(), iContestDay );
			if( iClass<m_cGridData.iPrintNumClasses-1) cClassText+=_T(",");
			pDC->TextOut( iXpos, iYpos, cClassText, cClassText.GetLength() );
			cTextSize=pDC->GetTextExtent(cClassText);
			iXpos+=cTextSize.cx+20;
			}
		iYpos-=cTextSize.cy;
		}

	DRAW_HORIZONTAL( iYpos );
	iYpos-=10;
	iXpos=20;

	if(m_cGridData.bLetterScheme )
		{
		iXpos=iLeftTab;

		CString strAlphabet=_T("ABCDEFGHIJ");
		for( int i=0; i<iRowSize; i++ )
			{

			SelectFont( pDC, 24, FW_BOLD, TRUE ); 
			cText=strAlphabet[i];
			pDC->TextOut( iXpos, iYpos, cText, cText.GetLength() );
			iXpos+=GetPageWidth(pDC)/(iRowSize+1);

			}
		iYpos-=cTextSize.cy;
		}


	for( int iClass=0; iClass< m_cGridData.iPrintNumClasses; iClass++)
		{
		EClass eClass	=m_cGridData.aiPrintClasses[iClass];
		nTotalContestants+=pDocument->m_caGridPositions[(int)eClass].GetSize();
		}

	for( int iClass=0; iClass< m_cGridData.iPrintNumClasses; iClass++)
		{
		EClass eClass	=m_cGridData.aiPrintClasses[iClass];
		CWSClass &cClass=GetClass(eClass);
		int iContestDay	=m_cGridData.aiPrintDays[iClass]+iPage-1;

		// Shuffle the grid to get to the correct day
		CStringArray cTempArray;		
		cTempArray.Copy( pDocument->m_caGridPositions[(int)eClass] );
		for( int i=0; i<iContestDay-1; i++ )
			Shuffle20Percent(cTempArray);

		int nContestants=pDocument->m_caGridPositions[(int)eClass].GetSize();
		if( nContestants==0 ) continue;


		int nProcess=nContestants;
		if( iClass<m_cGridData.iPrintNumClasses-1 ) nProcess+=m_cGridData.iSpaces;

		for( int iContestant=0; 
			iContestant< nProcess;
			iContestant++ )
			{

			if( (nTotalOutput % (iRowSize) )==0  ) 
				{
				iXpos=iLeftTab;
				iYpos-=cTextSize.cy/2;

				if(m_cGridData.bLetterScheme )
					{
					SelectFont( pDC, 18, FW_NORMAL, FALSE); 
					cText.Format(_T("%d)"), nRows+1);
					pDC->TextOut( iLeftMargin, iYpos-5, cText, cText.GetLength() );
					}
				else
					{
					int iStart	=(nRows*iRowSize);
					int iStop	=min( nTotalContestants, iStart+iRowSize-1);
					iStop		+=m_cGridData.iPrintStartNumber;
					iStart		+=m_cGridData.iPrintStartNumber;

					SelectFont( pDC, 15, FW_NORMAL, FALSE); 
					cText.Format(_T(" %d - %d"), iStart, iStop);
					pDC->TextOut( iLeftMargin, iYpos-5, cText, cText.GetLength() );
					}

				nRows++;
				}


			SelectFont( pDC, 24, FW_BOLD, FALSE); 
			if( iContestant<nContestants )
				cText=cTempArray.GetAt(iContestant);
			else
				cText=_T("  ");
			pDC->TextOut( iXpos, iYpos, cText, cText.GetLength() );
			nTotalOutput++;
			cTextSize=pDC->GetTextExtent(cText);
			iXpos+=GetPageWidth(pDC)/(iRowSize+1);

			iYpos-=cTextSize.cy/(iRowSize);
			}

		}

	CTime cToday = CTime::GetCurrentTime();
	cText=cToday.Format(_T("Printed on %A %B %d, %Y - %H:%M by Winscore") );
	pDC->SetTextAlign( TA_TOP| TA_CENTER );
	SelectFont( pDC, 13, FW_NORMAL, FALSE); 

	DRAW_HORIZONTAL( -1030 );
	pDC->TextOut( GetPageWidth(pDC)/2, -1035 , cText, cText.GetLength() );

	return; 
	}


void CWinscoreView::OnFileImportEvents() 
{
	AfxMessageBox(_T("This function is no loger supported."));
}

void CWinscoreView::OnUpdateFileImportEvents(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );	
}

void CWinscoreView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	pItemPreselect=GetSelectedPtr();
	int iItemSelect = GetListCtrl().GetNextItem(-1, LVNI_FOCUSED);


	if( m_eViewType==eContestantView)
		{
		if( iItemSelect<0 ) return;
		if( !pItemPreselect ) return;
		CWinscoreDoc* pDocument=GetDocument();
		CContestant1Dlg dlg;
		dlg.m_pContestant=(CContestant*)pItemPreselect;
		dlg.m_pcWinscoreDoc=pDocument;
		if( dlg.DoModal()==IDOK )
			{
			pDocument->m_contestantList.RemoveByPointer(dlg.m_pContestant);
			pDocument->m_contestantList.AddToList(dlg.m_pNewContestant);
			pDocument->m_contestantList.SetContestantItem(GetListCtrl(), iItemSelect, dlg.m_pNewContestant);
			pDocument->SetModifiedFlag();
			}
		}
	else if( m_eViewType==eTurnpointView)
		OnContestInfoTurnpoint();
	else if( m_eViewType==eTaskView)
		OnDailyinfoTask();
	else if( m_eViewType==eFlightLogView ||m_eViewType==ePreContestView)
		OnDblClickFlightView();
	pItemPreselect= NULL;

	CListViewEx::OnLButtonDblClk(nFlags, point);
}

void CWinscoreView::OnFileExportContestants() 
{
	CWinscoreDoc* pDocument=GetDocument();

	static TCHAR BASED_CODE szFilter[] = _T("Winscore Contestant Files (*.wcf)|*.wcf|All Files (*.*)|*.*||");

	CFileDialog  cFileDlg(FALSE, _T("wcf"), _T("contestants.wcf"), OFN_OVERWRITEPROMPT, szFilter);

	if( cFileDlg.DoModal()==IDOK )
		{
	
		try
			{
			ofstream ofs( cFileDlg.GetPathName() );
			if( !ofs.is_open() ) 
				{
				AfxMessageBox( _T("Could not open contestant file:  ")+cFileDlg.GetPathName(),  MB_ICONSTOP   );
				return;
				}
			ofs<< pDocument->m_contestantList;
			ofs.close();
			}
		catch (...)
			{
			AfxMessageBox( _T("Error writing contestant file."),  MB_ICONSTOP   );
			}
		CString cStatus;
		cStatus.Format( _T("%d Contestants successfully exported."), pDocument->m_contestantList.GetCount()  );
		AfxMessageBox( cStatus,  MB_ICONINFORMATION   );
		pDocument->SetModifiedFlag();

		}
	
}

void CWinscoreView::OnUpdateFileExportContestants(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );
}

bool CWinscoreView::SetupPrintOptions()
{

	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	EViews eView=pFrame->GetViewCombo();


//  Set the default print selection
	if( eView== eContestantView	 )
		{
		ePrintSelection=eContestantInfo;
		}
	else if( eView== eTakeoffGridView )
		{
		ePrintClass=pFrame->GetClassCombo();
		ePrintSelection=eTakeoffGridPositions;
		CPrintGridDlg	dlg( pDocument, &m_cGridData );

		if (dlg.DoModal() != IDOK) return false;

		}
	else if( eView==eTurnpointView || eView==eTurnpointGridView)
		{
		ePrintSelection=eTurnpointSummary;
		}
//	else if( eView==eInvalidLegView )
//		{
//		ePrintSelection=eInvalidTaskLeg;
//		}	
//	else if( eView==eTimesView )
//		{
//		ePrintSelection=eEvents;
//		cPrintDate=pFrame->GetDateCombo();
//		}	
	else if( eView==eTaskView )
		{
		CTaskInfoDlg	dlg;
		dlg.m_strTaskText=pDocument->m_cTaskInfoString;
		if (dlg.DoModal() != IDOK) return false;
		ePrintSelection=eTaskSheet;
		pDocument->m_cTaskInfoString=dlg.m_strTaskText;
		}	
	else
		{
		cPrintDate=pFrame->GetDateCombo();
		ePrintClass=pFrame->GetClassCombo();
		ePrintSelection=eDailyScoresheet;
		}
	return true;
}



void CWinscoreView::OnUpdateFilePrintDirect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(  IsPrintingAllowed()  );	
}

void CWinscoreView::OnFilePrintDirect() 
{
	if( !SetupPrintOptions() ) return;

	CView::OnFilePrint();	
}

void CWinscoreView::OnUpdateFilePrint(CCmdUI* pCmdUI) 
{

	pCmdUI->Enable( IsPrintingAllowed() );	
	
}

void CWinscoreView::OnDailyTaskplanner() 
{
	CWinscoreDoc* pDocument=GetDocument();
	CPlannerDlg	dlg(pDocument);
	dlg.DoModal();

}

void CWinscoreView::OnUpdateDailyTaskplanner(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();

	pCmdUI->Enable( pDocument->Valid() && pDocument->m_turnpointArray.GetValidTpts()>3  );	
	
}

void CWinscoreView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( nChar==DELETE_KEY )
		{
		void* pItemSelect=NULL;
		CWinscoreDoc* pDocument=GetDocument();
		CArray<int,int> cIntArray;
		int iItemSelect;

		int nSelected=GetSelectedItems(cIntArray);

		if( nSelected==1 )
			{
			iItemSelect = GetListCtrl().GetNextItem(-1, LVNI_FOCUSED);
			if( iItemSelect>=0 ) 
				{
				pItemSelect=(void*)GetListCtrl().GetItemData(iItemSelect);
				}
			}
		else if( nSelected==0 )
			return;

		switch(m_eViewType)
			{
			case eTaskView:
				{
				if( nSelected==1 )
					{
					CTask *pcTask=((CTask*)pItemSelect);
					CString str=_T("Ok to delete task for ");
					str+=GetClass(pcTask->m_eClass).GetName();
					str+=_T(" on ");
					str+=pcTask->DateText();
					str+=_T("?");
					if( AfxMessageBox(str, MB_YESNO)==IDNO ) break;  
					pDocument->m_taskList.Remove((CTask*)pItemSelect);
					GetListCtrl().DeleteItem(iItemSelect);
					}
				else
					{
					CString str;
					str.Format(_T("Ok to delete %d tasks?"), nSelected);
					if( AfxMessageBox(str, MB_YESNO)==IDNO ) break; 
					for( int i=nSelected-1; i>=0; i-- )
						{
						CTask* pTask=(CTask*)GetListCtrl().GetItemData(cIntArray[i]);
						if( pTask )
							{
							pDocument->m_taskList.Remove(pTask);
							GetListCtrl().DeleteItem(cIntArray[i]);
							}
						}
					}
				pDocument->SetModifiedFlag();
				break;
				}
			case eContestantView:
				{
				if( nSelected==1 )
					{
					CString str=_T("Ok to delete ");
					str+=((CContestant*)pItemSelect)->m_strContestNo;
					str+=_T("?");
					if( AfxMessageBox(str, MB_YESNO)==IDNO ) break;  
					pDocument->m_contestantList.RemoveByPointer((CContestant*)pItemSelect);
					GetListCtrl().DeleteItem(iItemSelect);
					}
				else
					{
					CString str;
					str.Format(_T("Ok to delete %d contestants?"), nSelected);
					if( AfxMessageBox(str, MB_YESNO)==IDNO ) break; 
					for( int i=nSelected-1; i>=0; i-- )
						{
						CContestant* pContestant=(CContestant*)GetListCtrl().GetItemData(cIntArray[i]);
						if( pContestant )
							{
							pDocument->m_contestantList.RemoveByPointer(pContestant);
							GetListCtrl().DeleteItem(cIntArray[i]);
							}
						}

					}
				pDocument->SetModifiedFlag();
				break;
				}
			case eFlightLogView:
				{
				OnFlightlogsDelete();
				}
			}
		UpdateStatusLine();
		}
	
	CListViewEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

bool CWinscoreView::IsPrintingAllowed()
{
	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	EViews eView=pFrame->GetViewCombo();

	if( !pDocument->Valid() )
		return false;
	else if (eView ==eTurnpointGridView)
		return false;
	else if (eView ==eFlightLogView)
		return false;	
//	else if ( eView ==eLandingCardView )
//		return false;
	else if (eView==eTaskView && GetSelectedPtr()==NULL)
		return false;
	else 
		return true;
}

void* CWinscoreView::GetSelectedPtr()
	{
	int iItemPreselect = GetSelectedItem();
	if( iItemPreselect>=0 ) 
		return (void*)GetListCtrl().GetItemData(iItemPreselect);
	else
		return NULL;

	}

int CWinscoreView::GetSelectedItem()
	{
	return GetListCtrl().GetNextItem(-1, LVNI_FOCUSED);
	}

int CWinscoreView::GetSelectedItems(CArray<int,int> &cIntArray)
	{
	int iItem=GetListCtrl().GetNextItem(-1, LVNI_SELECTED);
	if( iItem>=0 ) 
		cIntArray.Add(iItem);
	while( iItem>=0 )
		{
		iItem=GetListCtrl().GetNextItem( iItem, LVNI_SELECTED);
		if( iItem>=0 ) 
			cIntArray.Add(iItem);
		}
	return cIntArray.GetSize();

	}


void CWinscoreView::OnFileImportFlights() 
	{
	CImportFlightsDlg	dlg;
	dlg.m_pDoc=GetDocument();
	dlg.DoModal();

	OnFlightlogsRefresh(); 

	OnFlightlogsAnalyzenew();
	}

void CWinscoreView::OnUpdateFileImportFlights(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );		
}

void CWinscoreView::OnFlightlogsAnalyzeall() 
{
	SaveColumnWidths();

	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CTime cDate=pFrame->GetDateCombo();
	EClass eClass=pFrame->GetClassCombo();

	if( pFrame->GetViewCombo()!=eFlightLogView )
		{
	//	ViewFlightLogs(cDate, eClass );
		}

	CTask *pcTask=pDocument->m_taskList.GetByDateClass(cDate, eClass);
	if( pcTask && pcTask->IsOfficial() )
		{
		if( AfxMessageBox("The task for this class on this day has been set to Official.  The scores for that day are locked and will not be changed by any analysis you might make.\nOK to continue?", MB_YESNO | MB_ICONINFORMATION, 0)==IDNO ) return;
		}

	HWND hwndParent=GetParent()->m_hWnd;

	CWaitCursor cWait;
	pFrame->SetStatusText(_T("Analyzing"));

	pDocument->m_FlightList.AnalyzeAll(		pFrame,
											hwndParent,
											cDate,
											eClass,
											pDocument->m_turnpointArray,
											pDocument->m_contestantList,
											pDocument->m_taskList,
											FALSE,
											pDocument->m_eUnits);
//	ViewFlightLogs(cDate, eClass);
    OnFlightlogsRefresh(); 
	UpdateStatusLine();
	pDocument->SetModifiedFlag();
}

void CWinscoreView::OnFlightlogsViewigcfile() 
{
	if( !IsFlightSelected() ) return;
	CFlight* pcFlight=(CFlight*)GetSelectedPtr();
	if( pcFlight==NULL ) return;

    STARTUPINFO si;
	GetStartupInfo(&si);
	si.dwFlags|=STARTF_USESHOWWINDOW;
	si.wShowWindow=SW_SHOW;

	PROCESS_INFORMATION processInfo;
	CString strCommand=_T("notepad.exe ")+GetNonRelativePath(pcFlight->m_strFilePath);
	TCHAR Command[1024];
	strncpy_s(Command, LPCSTR(strCommand), 1024) ;

	TCHAR cBuf[1024];
	int iSize=1024;
	GetSystemDirectory( cBuf, iSize);
	strcat_s(cBuf,_T("\\NOTEPAD.EXE")) ;

	CFileFind cFinder;
	if( cFinder.FindFile(cBuf)==0 )
		{
		GetWindowsDirectory( cBuf, iSize);
		strcat_s(cBuf,_T("\\NOTEPAD.EXE")) ;
		}

	// TODO: Add your control notification handler code here
    BOOL result=CreateProcess(	cBuf, Command, 
								NULL, NULL, TRUE, 
								CREATE_DEFAULT_ERROR_MODE, 
								NULL, 
								NULL,  //Current directory
								&si, 
								&processInfo);
	}

void CWinscoreView::OnUpdateFlightlogsViewigcfile(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(  IsFlightSelected()	);
}

BOOL CWinscoreView::IsFlightSelected()
{
	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();

	return ( pDocument->Valid() &&
			 (pFrame->GetViewCombo()==eFlightLogView || pFrame->GetViewCombo()==ePreContestView) && GetSelectedPtr()!=NULL );
}

void CWinscoreView::OnFlightlogsDelete() 
{
	CArray<int,int> cIntArray;
	CWinscoreDoc* pDocument=GetDocument();
	void* pItemSelect=NULL;
	int nSelected=GetSelectedItems(cIntArray);
	if( nSelected==0 ) return;
	
	CFileStatus rStatus;
	if( nSelected==1 )
		{
		int iItemSelect = GetListCtrl().GetNextItem(-1, LVNI_FOCUSED);
		if( iItemSelect>=0 ) 
			{
			pItemSelect=(void*)GetListCtrl().GetItemData(iItemSelect);
			}

		if( !pItemSelect ) return;

		CString strFileName=GetNonRelativePath( ((CFlight*)pItemSelect)->m_strFilePath );

		CString cWarn=_T("Do you want to delete the flight log for ");
		cWarn+=((CFlight*)pItemSelect)->m_strPilot;
		cWarn+=_T("?");
		if( AfxMessageBox(cWarn, MB_YESNO )== IDNO )  return; 
		GetDocument()->m_FlightList.RemoveByPointer(((CFlight*)pItemSelect));
		GetListCtrl().DeleteItem(iItemSelect);

		if( CFile::GetStatus( strFileName, rStatus ) )
			{
			cWarn=_T("Do you want to delete the file: ");
			cWarn+=strFileName;
			cWarn+=_T("?");
			if( AfxMessageBox(cWarn, MB_YESNO )== IDNO )  return; 
			DWORD error=0;
			if( DeleteFile(strFileName)==0 )
				{
				error=GetLastError();
				if( error== ERROR_FILE_NOT_FOUND ) 
					{
					AfxMessageBox("System error, file not found.");
					}
				else if( error==ERROR_ACCESS_DENIED )
					{
					AfxMessageBox("System error, access denied.");
					}
				else 
					{
					AfxMessageBox("System error, could not delete file.");
					}
				}
			}
		}
	else
		{
		CStringArray strArr;
		CString str;

		str.Format(_T("Ok to delete %d flight logs?"), nSelected);
		if( AfxMessageBox(str, MB_YESNO)==IDNO ) return;
		for( int i=nSelected-1; i>=0; i-- )
			{
			CFlight* pFlight=(CFlight*)GetListCtrl().GetItemData(cIntArray[i]);
			if( pFlight )
				{
				if( CFile::GetStatus( GetNonRelativePath(pFlight->m_strFilePath), rStatus ) )
					strArr.Add(GetNonRelativePath(pFlight->m_strFilePath));

				pDocument->m_FlightList.RemoveByPointer(pFlight);
				GetListCtrl().DeleteItem(cIntArray[i]);
				}
			}

		if( strArr.GetSize()>0 )
			{
			str.Format(_T("Ok to delete %d IGC files?"), strArr.GetSize());
			if( AfxMessageBox(str, MB_YESNO| MB_DEFBUTTON2 )==IDYES ) 
				{
				for(int i=0; i<strArr.GetSize(); i++ )
					{
					DeleteFile( strArr[i] );
					}
				}
			}

		}
	pDocument->SetModifiedFlag();
	OnFlightlogsRefresh();
	}


void CWinscoreView::OnUpdateFlightlogsDelete(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(  IsFlightSelected()	);	
}

void CWinscoreView::OnUpdateFlightlogsAnalyzeall(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();

	pCmdUI->Enable(  pDocument->Valid() &&
					 pFrame->GetViewCombo()==eFlightLogView );
}

void CWinscoreView::OnEditCopy() 
{
	CWinscoreDoc* pDocument=GetDocument();
	if( !pDocument->Valid() ) return;
  	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();

	CString strNewLine=_T("\r\n");

    CString str;

	switch(m_eViewType)
		{
		case eTurnpointView:
			{
			str=pDocument->m_turnpointArray.GetText(pDocument->m_eCoordinateFormat);
			break;
			}
		case eTaskView:
			{
			CTask *pcTask=(CTask*)GetSelectedPtr();
			if( !pcTask ) break;
			str+=pcTask->m_cDate.Format( _T("%a. %b %d, %y") );
			str+="\t";
			str+=GetClass(pcTask->m_eClass).GetName();
			str+=strNewLine;
			str+=pcTask->GetText(pDocument->m_turnpointArray, pDocument->m_eUnits);	
			str+=strNewLine;
			break;
			}
		case eFlightLogView:
			{
			str=pDocument->m_FlightList.GetText(pFrame->GetDateCombo());
			break;
			}
		case eContestantView:
			{
			pDocument->m_contestantList.Sort(m_iSortedColumn[m_eViewType]);
			str=pDocument->m_contestantList.GetText();
			break;
			}
		case eTurnpointGridView:
			{
			str=pDocument->m_turnpointArray.MatrixText(pDocument->m_eUnits);
			break;
			}
		case eTakeoffGridView:
			{
			EClass eClass=pFrame->GetClassCombo(); 
			CWSClass &cClass=GetClass(eClass);

			str=pDocument->m_strContestName+strNewLine;
			str+=pDocument->m_strLocation+strNewLine;
			str+=cClass.GetName()+strNewLine;
			str+=_T("Takeoff Grid Positions")+strNewLine;
			str+=strNewLine;
			str+=strNewLine;

			int nColumns=pDocument->m_caGridPositions[(int)eClass].GetSize();

			if( nColumns==0 )
				{
				str=_T("Takeoff Grid not yet calculated.");
				}
			else
				{
				CStringArray cTempArray;
					
				cTempArray.Copy( pDocument->m_caGridPositions[(int)eClass] );

				for (  int i=0; i<pDocument->m_iNumContestDays; i++)
					{ 
					CString cTemp;
					cTemp.Format(_T("Day %d \t"),i+1);
					str+=cTemp;
					for ( int j=0; j<nColumns; j++)
						{ 
						str+=cTempArray.GetAt(j)+_T("\t");
						}
					str+=strNewLine;
					str+=strNewLine;
					Shuffle20Percent(cTempArray);
					}
				}
			break;
			}
		case eScoreView:
			{
			bool bTabs=FALSE;
			CCopyScoresheetDlg  dlg;

			if( dlg.DoModal()!=  IDOK ) return;
			bTabs=dlg.m_fTabCheck?(1):(0);

			str=pDocument->	GetTextHeader(	pFrame->GetDateCombo(),
											dlg.m_fSummaryCheck?(eSummary):(eDay),
											strNewLine );

			if( !dlg.m_fSummaryCheck)
				str+=pDocument->GetDayText(	pFrame->GetClassCombo(), 
											pFrame->GetDateCombo(), 
											eCumulative,
											strNewLine,
											bTabs );
			else
				str+=pDocument->GetCumulativeText(	pFrame->GetClassCombo(),
													strNewLine,
													bTabs );

			str+=pDocument->GetTextTrailer(	strNewLine, 
										    _T(""), 
										    eText);

			break;
			}
		}

	size_t strLength=str.GetLength();
    HGLOBAL hglobalText = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, strLength);
	memcpy((char *)GlobalLock(hglobalText),str,strLength);
    GlobalUnlock(hglobalText);
    
    OpenClipboard();
    EmptyClipboard();
    SetClipboardData(CF_TEXT,hglobalText);
    CloseClipboard();

	return;		
}

void CWinscoreView::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	EViews eView=pFrame->GetViewCombo();

	pCmdUI->Enable(	pDocument->Valid()			&&
					(eView ==eContestantView	||
					 eView ==eTurnpointView		||
					 eView ==eTurnpointGridView	||
					 eView ==eTaskView			||
					 eView ==eTakeoffGridView	||
					 eView ==eFlightLogView		||
					 eView ==eScoreView				) );
		
}

void CWinscoreView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CListViewEx::OnRButtonDown(nFlags, point);
}

void CWinscoreView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	EViews eView=pFrame->GetViewCombo();

	int iMenu=0;
	// Make sure we are in the flight veiw 
	if( eView==eFlightLogView )
		iMenu=IDR_FLIGHTMENU;
	else if( eView==ePreContestView )
		iMenu=IDR_PRECONTESTFLIGHTMENU;
	else if( eView==eScoreView )
		iMenu=IDR_SCORELIST;
	else if( eView==eTaskView )
		iMenu=IDR_COPYMENU;
	else if( eView==eContestantView )
		iMenu=IDR_CONTESTANT1;
	else
		return;

	CMenu menu;
	if (menu.LoadMenu(iMenu))
		{
		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
								   point.x,
								   point.y,
								   AfxGetMainWnd()); // use main window for cmds
        }
}

void CWinscoreView::OnFlightlogsSettings() 
{
	CFDRSettingsPropSheet cdlg(_T("Flight Analysis Settings"), GetDocument());
	cdlg.m_pDoc=GetDocument();
	cdlg.DoModal(); 
	GetDocument()->SetModifiedFlag();
}

void CWinscoreView::OnFlightlogsViewwarnings() 
{
	CWinscoreDoc* pDocument=GetDocument();
	if( !IsFlightSelected() ) return;
	CFlight* pcFlight=(CFlight*)GetSelectedPtr();
	if( pcFlight==NULL ) return;
	
	if( pcFlight->NumberOfWarnings()==0 )
		AfxMessageBox(_T("No warnings for ")+pcFlight->m_strPilot );
	else
		{
		CShowWarningsDlg dlg;
		dlg.m_pcFlight=pcFlight;
		dlg.m_pDoc=GetDocument();
		dlg.m_pView=this;
		dlg.DoModal();
		if( m_eViewType==eFlightLogView )
			pcFlight->AddToList( GetListCtrl(), TRUE, GetSelectedItem() );
		else
			pcFlight->AddToPreContestList( GetListCtrl(), pDocument->m_contestantList.GetByContestNo(pcFlight->m_strCID), TRUE, GetSelectedItem() );
		pDocument->SetModifiedFlag();
		}
}

void CWinscoreView::OnUpdateFlightlogsViewwarnings(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(  IsFlightSelected()	);				
}

void CWinscoreView::OnFlightlogsAnalyzeflight() 
{
	CWaitCursor cWait;
	CArray<int,int> cIntArray;
	double dRatio=0.0;
	CProgressBar cBar;
	CIGCDirectories cIGCDIR;

	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CTime cDate=pFrame->GetDateCombo();
	EClass eClass=pFrame->GetClassCombo();

	int nSelected=GetSelectedItems(cIntArray);
	if( nSelected==0 ) return;

	CTask *pcTask=pDocument->m_taskList.GetByDateClass(cDate, eClass);
	if( pcTask && pcTask->IsOfficial() )
		{
		if( AfxMessageBox("The task for this class on this day has been set to Official.  The scores for that day are locked and will not be changed by any analysis you might make.\nOK to continue?", MB_YESNO | MB_ICONINFORMATION, 0)==IDNO ) return;
		}

    cBar.SetRange(0, nSelected); 

	for( int iSel=0; iSel<nSelected; iSel++ )
		{
		CFlight* pcFlight=(CFlight*)GetListCtrl().GetItemData(cIntArray[iSel]);
		if( !pcFlight ) continue;

		CString cStatus=_T("Analyzing: ")+pcFlight->m_strCID;
		((CMainFrame*)pFrame)->SetStatusText(cStatus);
		cBar.UpdateProgressBar(iSel);

		// Check if the file is here
		CString cIGCFilename;

		if( !cIGCDIR.FindIGCFileName(  pcFlight->m_strFileName, cIGCFilename ) )
			{
			if( nSelected==1 )
				{
				AfxMessageBox(_T("IGC file for this pilot could not be found.\n Cannot analyze flight without it."));
				pcFlight->SetIGCFileMissing();
				if( m_eViewType==eFlightLogView )
					pcFlight->AddToList( GetListCtrl(), TRUE, cIntArray[iSel] );
				else
					pcFlight->AddToPreContestList( GetListCtrl(), pDocument->m_contestantList.GetByContestNo(pcFlight->m_strCID), TRUE, cIntArray[iSel] );
				return;
				}
			else
				continue;
			}

		// Delete the flight and reread it from the igc file
		GetDocument()->m_FlightList.RemoveByPointer(pcFlight);
		pcFlight=GetDocument()->m_FlightList.LoadIGC(cIGCFilename, GetDocument()->m_contestantList );

		if( !pcFlight->ReadFlight() )
			{
			if( nSelected==1 )
				{
				AfxMessageBox(_T("IGC file for this pilot could not be found.\n Cannot analyze flight without it."));
				pcFlight->SetIGCFileMissing();
				if( m_eViewType==eFlightLogView )
					pcFlight->AddToList( GetListCtrl(), TRUE, GetSelectedItem() );
				else
					pcFlight->AddToPreContestList( GetListCtrl(), pDocument->m_contestantList.GetByContestNo(pcFlight->m_strCID), TRUE, GetSelectedItem() );
				return;
				}
			else
				continue;
			}

		pcFlight->m_pDoc=pDocument;
		pcFlight->Analyze(	pDocument->m_taskList.GetByDateClass(cDate, eClass), 
							pDocument->m_turnpointArray,
							&pDocument->m_contestantList,
							pDocument->m_eUnits,
							false,
							m_eViewType==ePreContestView );
		pcFlight->m_eUnits=pDocument->m_eUnits;

		int iWarn=pcFlight->GetWarning(eStartLatest);
		if( iWarn>=0 )
			{
			dRatio= pDocument->GetLogTurninRatio( pcFlight->GetStartTime(),  pcFlight->m_eClass);
			if( dRatio>.80 ) 
				{
				if( Rechecklateststart(pcFlight, GetDocument(), true, false ) )
					{
					pcFlight->RemoveWarning(iWarn);
					}
				}
			}
		pcFlight->FreePositionData();

		if( m_eViewType==eFlightLogView )
			pcFlight->AddToList( GetListCtrl(), TRUE, cIntArray[iSel] );
		else
			pcFlight->AddToPreContestList( GetListCtrl(), pDocument->m_contestantList.GetByContestNo(pcFlight->m_strCID), TRUE, cIntArray[iSel] );
		}
	pDocument->SetModifiedFlag();
}

void CWinscoreView::OnUpdateFlightlogsAnalyzeflight(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(  IsFlightSelected()	);			
}



void CWinscoreView::OnFlightlogsEditflightreslts() 
{
	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CTime cDate=pFrame->GetDateCombo();
	CString strCID;
	CFlight* pcFlight=NULL;

	if( m_eViewType!=eFlightLogView ) return;

	if( !IsFlightSelected() ) return;
	pcFlight=(CFlight*)GetSelectedPtr();
	if( pcFlight==NULL ) return;
	strCID=pcFlight->m_strCID;

	if( pcFlight->GetStatus()==eNotEnteredInContest	 ||
		pcFlight->GetStatus()==eNotAnalyzed			 ||
		pcFlight->GetStatus()==eNoTaskSpecified		   )
		{
		AfxMessageBox(_T("Flight must be successfully analyzed before it can be edited."));
		return;
		}

	CEditFlightDlg dlg;
	dlg.m_pcFlight=new CFlight(pcFlight);
	dlg.m_pDoc=pDocument;

	EClass eClass=pFrame->GetClassCombo();
	dlg.m_pcTask=pDocument->m_taskList.GetByDateClass(cDate, eClass);

	if( dlg.m_pcTask->IsOfficial() )
		{
		if( AfxMessageBox("The task for this contestant has been set to Official.  The scores for that day are locked and will not be changed by any edits you might make.\nOK to continue?", MB_YESNO | MB_ICONINFORMATION, 0)==IDNO ) return;
		}

	dlg.m_pcTurnpointArray=&(GetDocument()->m_turnpointArray);

	if( dlg.DoModal()==IDOK )
		{
		pDocument->m_FlightList.RemoveByPointer(pcFlight);
		pDocument->m_FlightList.AddToList(dlg.m_pcFlight);
		dlg.m_pcFlight->m_eUnits=pDocument->m_eUnits;
		dlg.m_pcFlight->AddToList( GetListCtrl(), TRUE, GetSelectedItem() );
		pDocument->SetModifiedFlag();
		}
	else
		{
		delete dlg.m_pcFlight;
		}

}
  


void CWinscoreView::OnUpdateFlightlogsEditflightreslts(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( false	);			
	if( m_eViewType==eFlightLogView )
		{
		pCmdUI->Enable(  IsFlightSelected()	);			
		}
	else if( m_eViewType==eScoreView )
		{
		pCmdUI->Enable( true );			
		}
}

void CWinscoreView::OnFlightlogsAnalyzenew() 
{
	CWaitCursor cWait;
	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CTime cDate=pFrame->GetDateCombo();
	EClass eClass=pFrame->GetClassCombo();

	CTask *pcTask=pDocument->m_taskList.GetByDateClass(cDate, eClass);
	if( pcTask && pcTask->IsOfficial() )
		{
		if( AfxMessageBox("The task for this contestant has been set to Official.  The scores for that day are locked and will not be changed by any analysis you might make.\nOK to continue?", MB_YESNO | MB_ICONINFORMATION, 0)==IDNO ) return;
		}

	HWND hwndParent=GetParent()->m_hWnd;

	pDocument->m_FlightList.AnalyzeAll(		pFrame,
											hwndParent,
											cDate,
											eClass,
											pDocument->m_turnpointArray,
											pDocument->m_contestantList,
											pDocument->m_taskList,
											TRUE,
											pDocument->m_eUnits);
	pDocument->SetModifiedFlag();
//	OnFlightlogsRefresh();
}

void CWinscoreView::OnUpdateFlightlogsAnalyzenew(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();

	pCmdUI->Enable(  pDocument->Valid() &&
					 pFrame->GetViewCombo()==eFlightLogView );	
}

void CWinscoreView::OnFlightlogsViewanalysis() 
{

	CWinscoreDoc* pDocument=GetDocument();
	if( !IsFlightSelected() ) return;
	CFlight* pcFlight=(CFlight*)GetSelectedPtr();
	if( pcFlight==NULL ) return;

	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CTime cDate=pFrame->GetDateCombo();
	EClass eClass=pFrame->GetClassCombo();

	if( !pcFlight->IsContestant() )
		{
		AfxMessageBox(_T("Can not analyze non-contestants."));
		return;
		}

	CFlight cFlight(pcFlight); 

	CFlightAnalysisDlg dlg(pDocument->m_eUnits, pDocument->m_eUnits==eKilometers?eMeters:eFeet);
	dlg.m_pcFlight=&cFlight;
	dlg.m_pcTask=pDocument->m_taskList.GetByDateClass(cDate, eClass);

	if( !cFlight.ReadFlight() )
		{
		AfxMessageBox(_T("IGC file for this pilot could not be found.\n Cannot view analysis without it."));
		pcFlight->SetIGCFileMissing();
		if( m_eViewType==eFlightLogView )
			pcFlight->AddToList( GetListCtrl(), TRUE, GetSelectedItem() );
		else
			pcFlight->AddToPreContestList( GetListCtrl(), pDocument->m_contestantList.GetByContestNo(pcFlight->m_strCID), TRUE, GetSelectedItem() );
		return;
		}
	cFlight.m_pDoc=pDocument;
	bool bSuccess=cFlight.Analyze(	pDocument->m_taskList.GetByDateClass(cDate, pcFlight->m_eClass), 
										pDocument->m_turnpointArray,
										&pDocument->m_contestantList,
										pDocument->m_eUnits,
										false,
										m_eViewType==ePreContestView);
	if( !bSuccess ) return;

	dlg.DoModal();
	cFlight.FreePositionData();

	pcFlight->AddToList( GetListCtrl(), TRUE, GetSelectedItem() );
	if( m_eViewType==eFlightLogView )
		pcFlight->AddToList( GetListCtrl(), TRUE, GetSelectedItem() );
	else
		pcFlight->AddToPreContestList( GetListCtrl(), pDocument->m_contestantList.GetByContestNo(pcFlight->m_strCID), TRUE, GetSelectedItem() );

	pDocument->SetModifiedFlag();


}

void CWinscoreView::OnUpdateFlightlogsViewanalysis(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(  IsFlightSelected()	);				
}

void CWinscoreView::OnDblClickFlightView()
	{
	CWinscoreDoc* pDocument=GetDocument();
	if( !IsFlightSelected() ) return;
	CFlight* pcFlight=(CFlight*)GetSelectedPtr();
	if( pcFlight==NULL ) return;

	if( pcFlight->NumberOfActiveWarnings() >0 && m_eViewType==eFlightLogView)
		OnFlightlogsViewwarnings();
	else
		OnFlightlogsDisplay();

	}



void CWinscoreView::OnFlightlogsRefresh() 
{


	CWinscoreDoc* pDocument=GetDocument();
	CIGCDirectories cIGCDirs;

	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CTime cDate=pFrame->GetDateCombo();
	EClass eClass=pFrame->GetClassCombo();


	CIGCDirectories cIGCDir;
	CStringArray strArray;
	cIGCDir.FindIGCFiles( cDate, 1,  strArray );

	if( pFrame->GetViewCombo()==eFlightLogView )
		{
		pDocument->m_FlightList.LoadListFromIGC( cIGCDirs.GetFlightLogPath(), cDate, 1, pDocument->m_contestantList );
		ViewFlightLogs(cDate, eClass );
		}
	else if( pFrame->GetViewCombo()==eScoreView )
		{
		RefreshScoreDisplay();
		}
}

void CWinscoreView::OnUpdateFlightlogsRefresh(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();

	pCmdUI->Enable(  pDocument->Valid() &&
					 (pFrame->GetViewCombo()==eFlightLogView || pFrame->GetViewCombo()==eScoreView) );	
}

CString CWinscoreView::GetStatusLine()
	{
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	EViews eView=pFrame->GetViewCombo();
	CString cStatus=_T("Ready");

//  Set the default print selection
	if( eView== eContestantView	 )
		{
		cStatus.Format(_T("%d Contestants"), GetListCtrl().GetItemCount() );
		}
	else if( eView==eTurnpointView )
		{
		cStatus.Format(_T("%d Control Points"), GetListCtrl().GetItemCount() );
		}
	else if( eView==eTaskView )
		{
		cStatus.Format(_T("%d Tasks"), GetListCtrl().GetItemCount() );
		}	
	else if( eView==eFlightLogView || eView==ePreContestView )
		{
		cStatus.Format(_T("%d Flights"), GetListCtrl().GetItemCount() );
		}	
	else if( eView==eScoreView )
		{
		cStatus.Format(_T("%d Score Records"), GetListCtrl().GetItemCount() );
		}

	return cStatus;

	}

void CWinscoreView::UpdateStatusLine(  )
	{
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	pFrame->SetMessageText(GetStatusLine());
	}

void CWinscoreView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	UpdateStatusLine();
	
	CListViewEx::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CWinscoreView::OnFlightlogsDisplay() 
{
	CFlight* pcFlight=NULL;
	CFlightDisplayDlg dlg;

	if( m_eViewType==eFlightLogView || m_eViewType==ePreContestView )
		{
		if( !IsFlightSelected() ) return;
		pcFlight=(CFlight*)GetSelectedPtr();
		}
	else if( m_eViewType==eScoreView )
		{
		CScoreRecord *pcRecord=(CScoreRecord*)GetSelectedPtr();
		if( !pcRecord ) return;
		pcFlight=GetDocument()->m_FlightList.Get(pcRecord->m_cDate,pcRecord->m_cContestNo);
		}
	else
		return;
	
	if( pcFlight==NULL ) return;

	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CTime cDate=pFrame->GetDateCombo();
	pcFlight->m_pDoc=pDocument;

	CFlight cFlight(pcFlight);
	if( !cFlight.ReadFlight() )
		{
		AfxMessageBox(_T("IGC file for this pilot could not be found.\n Cannot display without it."));
		pcFlight->SetIGCFileMissing();
		return;
		}

	try {
		cFlight.m_pDoc=pDocument;
		cFlight.Analyze(	pDocument->m_taskList.GetByDateClass(cDate, pcFlight->m_eClass), 
							pDocument->m_turnpointArray,
							&pDocument->m_contestantList,
							pDocument->m_eUnits,
							false,
							m_eViewType==ePreContestView );

		dlg.m_pcFlight= &cFlight;
		dlg.m_pcTask=pDocument->m_taskList.GetByDateClass(cDate, cFlight.m_eClass);
		dlg.m_pDoc=pDocument;
		dlg.DoModal();
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception while attempting to display flight log.\nSave your data, restart Winscore and try the operation again."));
		}
	
}

void CWinscoreView::OnUpdateFlightlogsDisplay(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( false	);			
	if( m_eViewType==eFlightLogView ||  m_eViewType==ePreContestView )
		{
		pCmdUI->Enable(  IsFlightSelected()	);			
		}
	else if( m_eViewType==eScoreView )
		{
		CScoreRecord *pcRecord=(CScoreRecord*)GetSelectedPtr();
		if( !pcRecord ) return;
		CFlight* pcFlight=GetDocument()->m_FlightList.Get(pcRecord->m_cDate,pcRecord->m_cContestNo);
		if( !pcFlight ) return;

		pCmdUI->Enable( true );			
		}
	
}

void CWinscoreView::OnFileExportWinscoreflightfile() 
{
	CWinscoreDoc* pDocument=GetDocument();
	CExportFlightFileDlg dlg;

	dlg.m_pDoc=pDocument;
	dlg.DoModal();

	return;
}

void CWinscoreView::OnUpdateFileExportWinscoreflightfile(CCmdUI* pCmdUI) 
{
	int nFlights=GetDocument()->m_FlightList.GetCount();
	pCmdUI->Enable( nFlights>0	);		
}

void CWinscoreView::OnHelpRegister() 
{
//	CRegistrationDlg dlg(true);
//	dlg.DoModal();
}

void CWinscoreView::OnUpdateHelpRegister(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(  true );
}


void CWinscoreView::OnTimer(UINT nIDEvent) 
{

//	CListViewEx::OnTimer(nIDEvent);
}

void CWinscoreView::OnFlightlogsView() 
{
	OnFlightlogsRefresh();	
}

void CWinscoreView::OnUpdateFlightlogsView(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable( GetDocument()->Valid() );	
}

void CWinscoreView::OnAnalyzeInfo() 
{
	if( !IsFlightSelected() ) return;
	CFlight* pcFlight=(CFlight*)GetSelectedPtr();
	if( pcFlight==NULL ) return;

	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CTime cDate=pFrame->GetDateCombo();
	EClass eClass=pFrame->GetClassCombo();
	CTask *pcTask=GetDocument()->m_taskList.GetByDateClass(cDate, eClass);

	CDescriptionDlg dlg;
	dlg.m_bNoOKButton=true;

	dlg.m_Text=pcFlight->GetFlightText(pcTask, &GetDocument()->m_turnpointArray, _T("\r\n"));
	dlg.m_cTitle=_T("Flight Info for ")+pcFlight->m_strCID+_T(" : ")+pcFlight->m_strPilot;
	dlg.DoModal();
	CWinscoreDoc* pDocument=GetDocument();
	pDocument->SetModifiedFlag();
	
}

void CWinscoreView::OnUpdateAnalyzeInfo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(  IsFlightSelected()	);
}

void CWinscoreView::OnAnalyzeSecurityMarkasfailed() 
{
	if( !IsFlightSelected() ) return;
	CFlight* pcFlight=(CFlight*)GetSelectedPtr();
	if( pcFlight==NULL ) return;
	
	CWinscoreDoc* pDocument=GetDocument();

	pDocument->LogFailed (pcFlight->m_strFileName);
	pcFlight->SetSecurityFailed(); 
	pcFlight->AddToList( GetListCtrl(), TRUE, GetSelectedItem() );
	pDocument->SetModifiedFlag();
	
}

void CWinscoreView::OnAnalyzeSecurityMarkaspassed() 
{
	if( !IsFlightSelected() ) return;
	CFlight* pcFlight=(CFlight*)GetSelectedPtr();
	if( pcFlight==NULL ) return;
	
	CWinscoreDoc* pDocument=GetDocument();

	pcFlight->SetSecurityPassed(); 
	pDocument->LogPassed (pcFlight->m_strFileName);
	pcFlight->AddToList( GetListCtrl(), TRUE, GetSelectedItem() );
	pDocument->SetModifiedFlag();

}

void CWinscoreView::SaveColumnWidths()
	{
	CString strName=_T("");
	switch( m_eViewType )
		{
		case( eContestantView):
			{
			strName=_T("ContestantColumnWidths");
			break;
			}
		case( eTurnpointView):
			{
			strName=_T("TurnpointColumnWidths");
			break;
			}
		case( eTurnpointGridView):
			{
			strName=_T("TurnpointGridColumnWidths");
			break;
			}
		case( eFlightLogView):
			{
			strName=_T("FlightLogColumnWidths");
			break;
			}
		case( eScoreView):
			{
			strName=_T("ScoreColumnWidths");
			break;
			}
		default:
			return;
		}

	WriteColumns(strName,GetListCtrl());

	}

bool CWinscoreView::RestoreColumnWidths()
	{

	CString strName=_T("");
	switch( m_eViewType )
		{
		case( eContestantView):
			{
			strName=_T("ContestantColumnWidths");
			break;
			}
		case( eTurnpointView):
			{
			strName=_T("TurnpointColumnWidths");
			break;
			}
		case( eTurnpointGridView):
			{
			strName=_T("TurnpointGridColumnWidths");
			break;
			}
		case( eFlightLogView):
			{
			strName=_T("FlightLogColumnWidths");
			break;
			}
		case( eScoreView):
			{
			strName=_T("ScoreColumnWidths");
			break;
			}
		default:
			return false;
		}

	ReadColumns(strName,GetListCtrl());


	return true;
	}

void CWinscoreView::OnDestroy() 
{
	SaveColumnWidths();
	CListViewEx::OnDestroy();
}

void CWinscoreView::OnAnalyzeDetailedinfo() 
{
	if( m_eViewType!=eScoreView ) return;

	CScoreRecord *pcRecord=(CScoreRecord*)GetSelectedPtr();
	if( !pcRecord ) return;

	CDetailedScoreInfoDlg dlg(GetDocument(),pcRecord);
	dlg.DoModal();
	
}

void CWinscoreView::OnUpdateAnalyzeDetailedinfo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CWinscoreView::OnFileExportCpXml() 
{
	CWinscoreDoc* pDocument=GetDocument(); 

	static TCHAR BASED_CODE szFilter[] = _T("XML Files (*.xml)|*.xml|All Files (*.*)|.*||");

	CFileDialog  cFileDlg(FALSE, _T("xml"), _T("controlpoints.xml"), OFN_OVERWRITEPROMPT, szFilter);

	if( cFileDlg.DoModal()==IDOK )
		{
		try
			{
			CXMLMgr cMgr;
			cMgr.Initlize();
			cMgr.CreateXMLFileHeader();
			cMgr.CreateRoot(_T("Winscore"));

			pDocument->m_turnpointArray.GetXML(cMgr, cMgr.GetRoot());

			TESTHR(cMgr.Save(cFileDlg.GetPathName()));

			CString strStatus=cFileDlg.GetPathName();
			strStatus+=_T(" sucessfully exported.");
			AfxMessageBox( strStatus,  MB_ICONINFORMATION      );

			}
		catch (...)
			{
			AfxMessageBox( _T("Error writing xml file."),  MB_ICONSTOP   );
			}
	}

}

void CWinscoreView::OnFileExportContestantsinxmlformat() 
{
	CWinscoreDoc* pDocument=GetDocument();

	static TCHAR BASED_CODE szFilter[] = _T("XML Files (*.xml)|*.xml|All Files (*.*)|*.*||");

	CFileDialog  cFileDlg(FALSE, _T("xml"), _T("contestants.xml"), OFN_OVERWRITEPROMPT, szFilter);

	if( cFileDlg.DoModal()==IDOK )
		{
		try
			{
			CXMLMgr cMgr;
			cMgr.Initlize();
			cMgr.CreateXMLFileHeader();
			cMgr.CreateRoot(_T("Winscore"));

			int iNumExported=pDocument->m_contestantList.GetXML(cMgr, cMgr.GetRoot() );

			TESTHR(cMgr.Save(cFileDlg.GetPathName()));

			CString strStatus;
			strStatus.Format(_T("%d contestants exported to %s"), iNumExported, cFileDlg.GetPathName());
			AfxMessageBox( strStatus,  MB_ICONINFORMATION      );

			}
		catch (...)
			{
			AfxMessageBox( _T("Error writing xml file."),  MB_ICONSTOP   );
			}
	}
	
}

void CWinscoreView::OnFileImportControlpointsinxmlformat() 
{
	CWinscoreDoc* pDocument=GetDocument();

	CString  cFileName=_T("");

	if( pDocument->m_turnpointArray.GetValidTpts()>0 )
		if( AfxMessageBox( _T("Imported points may replace turnpoints already entered."), MB_OKCANCEL )!=IDOK ) return;


	static TCHAR BASED_CODE szFilter[] = _T("XML file (*.xml)|*.xml|All Files (*.*)|*.*||");
	CFileDialog  cFileDlg(TRUE, _T("dat"), cFileName, OFN_HIDEREADONLY, szFilter);

	cFileDlg.m_ofn.lpstrTitle=_T("Import Control Points From File");

	if( cFileDlg.DoModal()==IDOK )
		{
		cFileName=cFileDlg.GetPathName();


		try
			{
			CXMLMgr cMgr;
			cMgr.Initlize();

			TESTHR(cMgr.Load(cFileDlg.GetPathName()));

			CString strRoot=cMgr.GetNodeName(cMgr.GetRoot());
			if(strRoot!=_T("Winscore") )
				{
				AfxMessageBox( _T("Not a valid Winscore XML file."),  MB_ICONSTOP   );
				return;
				}

			MSXML2::IXMLDOMNodePtr pWaypoints;
			TESTHR(cMgr.SelectChild(_T("Waypoints"), cMgr.GetRoot() ,pWaypoints));
			if( pWaypoints==NULL )
				{
				AfxMessageBox( _T("No control points found in XML file."),  MB_ICONSTOP   );
				return;
				}

			int nImported=pDocument->m_turnpointArray.ImportXML(cMgr, cMgr.GetRoot());

			CString strStatus;
			strStatus.Format(_T("%d Control Points sucessfully imported from %s."),nImported,cFileDlg.GetPathName());
			AfxMessageBox( strStatus,  MB_ICONINFORMATION      );
			pDocument->SetModifiedFlag();
			}
		catch (...)
			{
			AfxMessageBox( _T("Error reading xml file."),  MB_ICONSTOP   );
			}

		}	

}

void CWinscoreView::OnFileImportXmlContestants() 
{
	CWinscoreDoc* pDocument=GetDocument();
	int nSkipped=0;

	CString  cFileName=_T("");

	static TCHAR BASED_CODE szFilter[] = _T("XML file (*.xml)|*.xml|All Files (*.*)|*.*||");
	CFileDialog  cFileDlg(TRUE, _T("dat"), cFileName, OFN_HIDEREADONLY, szFilter);

	cFileDlg.m_ofn.lpstrTitle=_T("Import Contestants From XML File");

	if( cFileDlg.DoModal()==IDOK )
		{
		cFileName=cFileDlg.GetPathName();

		try
			{
			CXMLMgr cMgr;
			cMgr.Initlize();

			TESTHR(cMgr.Load(cFileDlg.GetPathName()));

			CString strRoot=cMgr.GetNodeName(cMgr.GetRoot());
			if(strRoot!=_T("Winscore") )
				{
				AfxMessageBox( _T("Not a valid Winscore XML file."),  MB_ICONSTOP   );
				return;
				}

			MSXML2::IXMLDOMNodePtr pContestants;
			TESTHR(cMgr.SelectChild(_T("Contestants"), cMgr.GetRoot(), pContestants));
			if( pContestants==NULL )
				{
				AfxMessageBox( _T("No contestants found in XML file."),  MB_ICONSTOP   );
				return;
				}

			int nImported=pDocument->m_contestantList.ImportXML(cMgr,cMgr.GetRoot(), nSkipped);
			ViewContestantList();

			CString strStatus;
			if( nSkipped==0 ) 
			   strStatus.Format( _T("%d new contestants imported from %s"), nImported, cFileDlg.GetPathName());
			else
			   strStatus.Format( _T("%d new contestants were imported and %d existing contestants were updated from %s"), nImported, nSkipped, cFileDlg.GetPathName());
			AfxMessageBox( strStatus,  MB_ICONINFORMATION      );
			pDocument->SetModifiedFlag();

			}
		catch (...)
			{
			AfxMessageBox( _T("Error reading xml file."),  MB_ICONSTOP   );
			}

		}	

	
}

void CWinscoreView::OnScoresheetServer() 
{
	CScoreServerDlg dlg;
	dlg.m_pDoc=GetDocument();
	dlg.DoModal();
		

}
void CWinscoreView::OnFileExportAutoexportscoresheettoxml()
{
	
	CAutoExport dlg;
	dlg.DoModal();

}


void CWinscoreView::OnFileExportXmlEntirecontestfile() 
{
	CWinscoreDoc* pDocument=GetDocument();

	static TCHAR  BASED_CODE szFilter[] = _T("XML Files (*.xml)|*.xml|All Files (*.*)|.*||");

	CFileDialog  cFileDlg(FALSE, _T("xml"), _T("winscore.xml"), OFN_OVERWRITEPROMPT, szFilter);

	if( cFileDlg.DoModal()==IDOK )
		{
		try
			{
;
			CXMLMgr cMgr;
			cMgr.Initlize();
			cMgr.CreateXMLFileHeader();
			cMgr.CreateRoot(_T("Winscore"));

			CWaitCursor cWait;
			MSXML2::IXMLDOMNodePtr pRoot=  cMgr.GetRoot();
			pDocument->GetXML( cMgr, pRoot);

			TESTHR(cMgr.Save(cFileDlg.GetPathName()) );

			CString strStatus;
			strStatus.Format(_T("%s successfully created."), cFileDlg.GetPathName());
			AfxMessageBox( strStatus,  MB_ICONINFORMATION      );
			}
		catch (...)
			{
			AfxMessageBox( _T("Error writing xml file."),  MB_ICONSTOP   );
			}
	}
}

void CWinscoreView::OnFileImportXmlEntirecontest() 
{
	CWinscoreDoc* pDocument=GetDocument();

	CString  cFileName=_T("winscore.xml");

	if( AfxMessageBox( _T("WARNING - Current contest will be purged."), MB_OKCANCEL )!=IDOK ) return;


	static TCHAR BASED_CODE szFilter[] = _T("XML file (*.xml)|*.xml|All Files (*.*)|.*||");
	CFileDialog  cFileDlg(TRUE, _T("xml"), cFileName, OFN_HIDEREADONLY, szFilter);

	cFileDlg.m_ofn.lpstrTitle=_T("Import Contest From File");

	if( cFileDlg.DoModal()==IDOK )
		{
		cFileName=cFileDlg.GetPathName();

		try
			{
			CXMLMgr cMgr;
			cMgr.Initlize();

			TESTHR(cMgr.Load(cFileDlg.GetPathName()));

			CString strRoot=cMgr.GetNodeName(cMgr.GetRoot());
			if(strRoot!=_T("Winscore") )
				{
				AfxMessageBox( _T("Not a valid Winscore XML file."),  MB_ICONSTOP   );
				return;
				}

			CWaitCursor cWait;
			pDocument->DeleteContents();
			pDocument->ImportXML(cMgr,cMgr.GetRoot());

			ViewContestantList();

			CString strStatus=cFileDlg.GetPathName();
			strStatus+=_T(" sucessfully imported.");
			AfxMessageBox( strStatus,  MB_ICONINFORMATION      );
			pDocument->SetModifiedFlag();
			}
		catch (...)
			{
			AfxMessageBox( _T("Error reading xml file."),  MB_ICONSTOP   );
			}

		}	
	
	
}

void CWinscoreView::OnImportFlashcard() 
{
	CStringArray strFiles, strFilePaths;
	TCHAR buff[512];
	bool bFound=false;
	ZeroMemory(buff, 512);

	CWinscoreDoc* pDocument=GetDocument();
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CTime cDate=pFrame->GetDateCombo();
	EClass eClass=pFrame->GetClassCombo();
	CIGCDirectories cIGCDirs;

	DWORD iret=GetLogicalDriveStrings( 512, buff);
	if( iret>0 )
		{
		int iPtr=0;

		while( buff[iPtr] != NULL )
			{
			if( buff[iPtr]!='A' && buff[iPtr]!='a' &&
				GetDriveType(&buff[iPtr])==DRIVE_REMOVABLE  )
				{
				CString strDrive=&buff[iPtr];

				bFound=CIGCDirectories::FindIGCFilesInDirectory( strDrive, _T("cai"),  cDate, 1, strFilePaths, strFiles, 4 )>0;
				if( bFound ) break;

				bFound=CIGCDirectories::FindIGCFilesInDirectory( strDrive, _T("igc"),  cDate, 1, strFilePaths, strFiles, 4 )>0;
				if( bFound ) break;

				}
			iPtr+=4;
			}

		if( bFound )
			{
			CString strInputFilePath=strFilePaths[0];
			CString strInputFileName=strFiles[0];
			CFileStatus rSourceStatus, rDestStatus;
			CTime cDate=0;

			CIGCFile igcFile(strInputFilePath);
			cDate=CTime( igcFile.m_iYear, igcFile.m_iMonth, igcFile.m_iDay, 0, 0, 0 );
			CString str;
			str.Format(_T("Ok to import %s for %s - %s?"),strInputFilePath, igcFile.m_strCompetitionID, igcFile.m_strPilot );
			if( AfxMessageBox(str, MB_YESNO|MB_ICONQUESTION)==IDNO ) return;  

			if( !igcFile.m_bValid )
				{
				AfxMessageBox(_T("Invalid IGC file, could not import."));
				return;
				}


			CWaitCursor cWait;
			CString	strLogPath	=cIGCDirs.GetFlightLogPath(cDate);

			CString strStatus;
			CString strOutputFileName;
			bool bSuccess=false;
			bool bIGCSecure=false;

			if( CIGCDirectories::ImportIGC( strInputFilePath, strInputFileName, strLogPath, strOutputFileName, strStatus ) )
				{
				bSuccess=true;
				CString strSecureStatus;
				if( CheckIGCSecurity( strInputFilePath,  strSecureStatus ) )
					{
					pDocument->LogPassed(strInputFileName);
					bIGCSecure=true;
					}
				else
					{
					AfxMessageBox(strSecureStatus);
					pDocument->LogFailed(strInputFileName);
					}
				}


			if( bSuccess )
				{
				CFlight* pcFlight=pDocument->m_FlightList.LoadIGC( strOutputFileName, pDocument->m_contestantList );

				pcFlight->ReadFlight();
				pcFlight->m_pDoc=pDocument;
				pcFlight->Analyze(	pDocument->m_taskList.GetByDateClass(cDate, eClass), 
									pDocument->m_turnpointArray,
									&pDocument->m_contestantList,
									pDocument->m_eUnits);
				pcFlight->FreePositionData();
				pDocument->SetModifiedFlag();

				OnFlightlogsRefresh();

				if( m_eViewType!=eFlightLogView )
					ViewFlightLogs( cDate,  eClass);

				if( bIGCSecure )
					strStatus+=_T("\r\nSecurity check OK.");
				else
					strStatus+=_T("\r\nSecurity check FAILED.");
				
				AfxMessageBox( strStatus, MB_ICONINFORMATION );

				}
			else
				AfxMessageBox( strStatus );
			}
		else
			{
			AfxMessageBox(_T("No flights for today found on flash card."));
			}
		}
	pDocument->SetModifiedFlag();

	
}

void CWinscoreView::OnUpdateImportFlashcard(CCmdUI* pCmdUI) 
{
	TCHAR buff[512];
	ZeroMemory(buff, 512);
	DWORD iret=GetLogicalDriveStrings( 512, buff);
	DWORD iDrives=GetLogicalDrives();
	if( iret>0 )
		{
		char cDrv='d'; //Start at D drive, always ignore a,b & c;
		iDrives=iDrives>>3;  //Move the idrives to match the d drive

		while( cDrv!='z' )
			{
			CString strDrive=cDrv;
			strDrive+=_T(":\\");
			if( (iDrives & 1) && GetDriveType(strDrive)==DRIVE_REMOVABLE  )
				{
				pCmdUI->Enable( true );	
				return;
				}
			iDrives=iDrives>>1;
			cDrv++;
			}
		}
		pCmdUI->Enable( FALSE );	

}

void CWinscoreView::OnSsaServer() 
{
	CScoreServerDlg dlg;
	dlg.m_pDoc=GetDocument();
	dlg.DoModal();
}

void CWinscoreView::OnUpdateSsaServer(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );
}

void CWinscoreView::OnUpdateFileImportControlpointsinxmlformat(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );
	
}

void CWinscoreView::OnUpdateFileExportCpXml(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );	
}

void CWinscoreView::OnUpdateFileExportXmlEntirecontestfile(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );	
}

void CWinscoreView::OnUpdateFileImportXmlContestants(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );	
}

void CWinscoreView::OnUpdateFileExportContestantsinxmlformat(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );
	
}

void CWinscoreView::OnUpdateFileImportControlpointsincaidatformat(CCmdUI* pCmdUI) 
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );	
}

void CWinscoreView::OnFileImportControlpointsincaidatformat() 
{
	CWinscoreDoc* pDocument=GetDocument();

	CString  cFileName=_T("");

	if( pDocument->m_turnpointArray.GetValidTpts()>0 )
		if( AfxMessageBox( _T("Imported points may replace turnpoints already entered."), MB_OKCANCEL )!=IDOK ) return;


	static TCHAR BASED_CODE szFilter[] = _T("Cambridge .dat file (*.dat)|*.dat|All Files (*.*)|*.*||");
	CFileDialog  cFileDlg(TRUE, _T("dat"), cFileName, OFN_HIDEREADONLY, szFilter);

	cFileDlg.m_ofn.lpstrTitle=_T("Import Control Points From Cambridge .dat File");

	if( cFileDlg.DoModal()==IDOK )
		{
		cFileName=cFileDlg.GetPathName();


		try
			{
			int nImported=pDocument->m_turnpointArray.ImportTurnpoints(	cFileName, 
																cFileDlg.m_ofn.nFilterIndex);
			CString cError;
			if( nImported==0 ) 
				{
				cError=_T("Could not import control points from specified file.");
				AfxMessageBox(  cError,	MB_OK, 0 );
				}
			else
				{
				CString strStatus;
				strStatus.Format(_T("%d Control Points sucessfully imported from %s."),nImported,cFileDlg.GetPathName());
				AfxMessageBox( strStatus,  MB_ICONINFORMATION      );
				ViewTurnpointList(); 
				}
			pDocument->SetModifiedFlag();
			}
		catch (...)
			{
			AfxMessageBox( _T("Error reading .dat file."),  MB_ICONSTOP   );
			}

		}
	}
void CWinscoreView::OnFlightlogsCheckSecurity()
	{
	if( !IsFlightSelected() ) return;
	CWinscoreDoc* pDocument=GetDocument();
	CFlight* pcFlight=(CFlight*)GetSelectedPtr();
	if( pcFlight==NULL ) return;
	pcFlight->m_strFilePath;

	if( !pcFlight->ReadFlight() )
		{
		AfxMessageBox(_T("IGC file for this pilot could not be found.\n Cannot check security without it."));
		pcFlight->SetIGCFileMissing();
		return;
		}

	CString cWarn;
	if( pcFlight->CheckSecurity2(cWarn) )
		{
		CString cMess;
		cMess.Format(_T("PASSED - Security check passed for %s"),pcFlight->m_strCID);
		AfxMessageBox(cMess,MB_ICONINFORMATION );
		pcFlight->SetSecurityPassed(); 
		}
	else
		{
		CString cMess;
		cMess.Format(_T("FAILED - Security check failed for %s \n %s"),pcFlight->m_strCID,cWarn);
		AfxMessageBox(cMess,MB_ICONSTOP );
		pcFlight->SetSecurityFailed(); 
		}		

	if( m_eViewType==eFlightLogView )
		pcFlight->AddToList( GetListCtrl(), TRUE, GetSelectedItem() );
	else
		{
		pcFlight->AddToPreContestList( GetListCtrl(), pDocument->m_contestantList.GetByContestNo(pcFlight->m_strCID), TRUE, GetSelectedItem() );
		}

	pDocument->SetModifiedFlag();
	}
	
void CWinscoreView::OnUpdateFlightlogsCheckSecurity(CCmdUI* pCmdUI)
	{
	pCmdUI->Enable( false	);			
	if( m_eViewType==eFlightLogView ||  m_eViewType==ePreContestView )
		{
		pCmdUI->Enable(  IsFlightSelected()	);			
		}
	}



void CWinscoreView::OnImportfromssa()
{
	CWinscoreDoc* pDocument=GetDocument();

	CContestantsFromSSA cDlg;
	cDlg.m_pDoc=pDocument;
	if (cDlg.DoModal() == IDOK)
		{
		POSITION pos = cDlg.m_cContestantList.GetHeadPosition();
		while(1)
			{
			CContestant *pcContestant=(CContestant*)cDlg.m_cContestantList.GetNext(pos);
			if( pcContestant ) 
				{
				if( !pDocument->m_contestantList.GetByContestNo(pcContestant->m_strContestNo) ) 
					{
					pDocument->m_contestantList.AddTail(new CContestant(pcContestant));
					}
				}
			if( pos==NULL ) break;
			}

		pDocument->SetModifiedFlag();
		CWinscoreView::ViewContestantList();
	}
}


void CWinscoreView::OnUpdateImportfromssa(CCmdUI *pCmdUI)
{
	CWinscoreDoc* pDocument=GetDocument();
	pCmdUI->Enable(  pDocument->Valid() );
}


void CWinscoreView::OnDeleteContestant()
{
		void* pItemSelect=NULL;
		CWinscoreDoc* pDocument=GetDocument();
		CArray<int,int> cIntArray;
		int iItemSelect;

		int nSelected=GetSelectedItems(cIntArray);
		if( nSelected==1 )
			{
			iItemSelect = GetListCtrl().GetNextItem(-1, LVNI_FOCUSED);
			if( iItemSelect>=0 ) 
				{
				pItemSelect=(void*)GetListCtrl().GetItemData(iItemSelect);
				}
			}
		if( nSelected==1 )
			{
			CString str=_T("Ok to delete ");
			str+=((CContestant*)pItemSelect)->m_strContestNo;
			str+=_T("?");
			if( AfxMessageBox(str, MB_YESNO)==IDNO ) return;  
			pDocument->m_contestantList.RemoveByPointer((CContestant*)pItemSelect);
			GetListCtrl().DeleteItem(iItemSelect);
			}
		else
			{
			CString str;
			str.Format(_T("Ok to delete %d contestants?"), nSelected);
			if( AfxMessageBox(str, MB_YESNO)==IDNO ) return; 
			for( int i=nSelected-1; i>=0; i-- )
				{
				CContestant* pContestant=(CContestant*)GetListCtrl().GetItemData(cIntArray[i]);
				if( pContestant )
					{
					pDocument->m_contestantList.RemoveByPointer(pContestant);
					GetListCtrl().DeleteItem(cIntArray[i]);
					}
				}

			}
		pDocument->SetModifiedFlag();
}


void CWinscoreView::OnEdit1Contestant()
{
	pItemPreselect=GetSelectedPtr();
	int iItemSelect = GetListCtrl().GetNextItem(-1, LVNI_FOCUSED);

	if( m_eViewType==eContestantView)
		{
		if( iItemSelect<0 ) return;
		if( !pItemPreselect ) return;
		CWinscoreDoc* pDocument=GetDocument();
		CContestant1Dlg dlg;
		dlg.m_pContestant=(CContestant*)pItemPreselect;
		dlg.m_pcWinscoreDoc=pDocument;
		if( dlg.DoModal()==IDOK )
			{
			pDocument->m_contestantList.RemoveByPointer(dlg.m_pContestant);
			pDocument->m_contestantList.AddToList(dlg.m_pNewContestant);
			pDocument->m_contestantList.SetContestantItem(GetListCtrl(), iItemSelect, dlg.m_pNewContestant);
			pDocument->SetModifiedFlag();
			}
		}
}


void CWinscoreView::OnAddContestant()
{

	CWinscoreDoc* pDocument=GetDocument();
	CContestant1Dlg dlg;
	dlg.m_pContestant=(CContestant*)pItemPreselect;
	dlg.m_pcWinscoreDoc=pDocument;
	if( dlg.DoModal()==IDOK )
		{
		pDocument->m_contestantList.AddToList(dlg.m_pNewContestant);
		pDocument->SetModifiedFlag();
		ViewContestantList();
		}
}

void CWinscoreView::OnImportHandicaplistfromssa()
	{
	CWaitCursor cWait;
	if( !CheckForHandicapUpdate() )
		{
		AfxMessageBox("Your handicap list is already up to date.", MB_ICONINFORMATION  );
		}
	else
		{
		if( AfxMessageBox( "An updated handicap list is available from ssa.org.\n\nUpdate handicap list now?", MB_ICONQUESTION  | MB_YESNO )==IDYES ) 
			{
			CWinscoreDoc* pDocument=GetDocument();
			UpdateHandicapListFromSSA(pDocument->m_cGIList);
			}
		}
	}


//void XSLT(LPCWSTR strXML, LPCWSTR strXSL, LPCWSTR strHTML);

void CWinscoreView::OnExportHandicapsummarytoHTML()
{
	CWinscoreDoc* pDocument=GetDocument();
	static TCHAR BASED_CODE szFilter[] = _T("HTML Files (*.html)|*.html|All Files (*.*)|.*||");

	CFileDialog  cFileDlg(FALSE, _T("html"), _T("HandicapSummary.html"), OFN_OVERWRITEPROMPT, szFilter);

	if( cFileDlg.DoModal()==IDOK )
		{
		try
			{
			CString strFileName=cFileDlg.GetPathName();

			CXMLMgr cMgr;
			cMgr.Initlize();
			cMgr.CreateXMLFileHeader();
			cMgr.CreateRoot(_T("Winscore"));

			//Add the date/time field
			CTime cToday = CTime::GetCurrentTime();
			CString strDate=cToday.Format(_T("%A %B %d, %Y - %H:%M"));
			cMgr.CreateElement( cMgr.GetRoot(), _T("CreationTime"),  LPCSTR(strDate) );
			cMgr.CreateElement( cMgr.GetRoot(), _T("Title"),	LPCSTR(pDocument->m_strContestName) );
			cMgr.CreateElement( cMgr.GetRoot(), _T("Location"),	LPCSTR(pDocument->m_strLocation) );
   			GetXMLDate(cMgr,  cMgr.GetRoot(), _T("startdate"), pDocument->m_CContestStartDate);

			// Populate the XML
			int iSort=m_iSortedColumn[eContestantView] ;
			pDocument->m_contestantList.Sort(iSort);
			int nExport=pDocument->m_contestantList.GetXML(cMgr, cMgr.GetRoot(),true);
			if( nExport==0 ) 
				{
				AfxMessageBox( _T("No handicapped contestants to export."),  MB_ICONSTOP   );
				return;
				}

			// Read the XSL stylesheet
			CXMLMgr cMgrXSL;
			cMgrXSL.Initlize();
			TESTHR(cMgrXSL.Load("ws_handicapsummary.xsl"));

			CXMLMgr cMgrHTML;
			cMgrHTML.Initlize();

			// Combine the XML and the XSL stylesheet and output the resulting HTML
			HRESULT hr=cMgr.TransformDOM2Obj( cMgrXSL.GetDocumentPtr(), cMgrHTML.GetDocumentPtr() );
			if( hr==S_OK ) 
				{
				hr=cMgrHTML.Save(strFileName);
				if( hr!=S_OK )
					{
					CString strStatus;
					strStatus.Format("Could not write to %s.\nMake sure the file is not in use by another program",cFileDlg.GetPathName());
					AfxMessageBox( strStatus,  MB_ICONSTOP  );
					return;
					}

				// Popup the output in a web browser
				ShellExecute(0, 0, LPCSTR(strFileName), 0, 0 , SW_SHOW );
				}
			else
				{
				AfxMessageBox( "Internal error occured converting XML to HTML",  MB_ICONSTOP  );
				}
			}
		catch (...)
			{
			AfxMessageBox( _T("Error writing html file."),  MB_ICONSTOP   );
			}
		}
}



void CWinscoreView::OnDailyAutoscoring()
{
	CMainFrame *pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CTime cDate		=pFrame->GetDateCombo();
	EClass eClass	=pFrame->GetClassCombo();

	GetDocument()->SaveReminder();

	CTask *pcTask=GetDocument()->m_taskList.GetByDateClass(cDate, eClass);
	if( pcTask && pcTask->IsOfficial() )
		{
		if( AfxMessageBox("The task for this contestant has been set to Official.  The scores for that day are locked and will not be changed by any analysis you might make.\nOK to continue?", MB_YESNO | MB_ICONINFORMATION, 0)==IDNO ) return;
		}


	if(m_pModlessAutoScoreDlg)
		{
		m_pModlessAutoScoreDlg->SetForegroundWindow();
		m_pModlessAutoScoreDlg->ShowWindow(SW_RESTORE);
		return;
		}

	m_pModlessAutoScoreDlg=new CAutoScoreDlg();

	m_pModlessAutoScoreDlg->m_pFrame=pFrame;
	m_pModlessAutoScoreDlg->m_pDocument=GetDocument();
	m_pModlessAutoScoreDlg->m_cDate=cDate;
	m_pModlessAutoScoreDlg->m_eClass=eClass;
	m_pModlessAutoScoreDlg->m_iSSA_ID=m_pModlessAutoScoreDlg->m_pDocument->m_iSSA_ID;
	m_pModlessAutoScoreDlg->m_pView=this;

	bool bRemember=GetWinscoreInt( SSACONTESTREMEMBER,0)?(true):(false);
	if( bRemember )
		{
		m_pModlessAutoScoreDlg->m_strUsername	=GetWinscoreString(  SSACONTESTUSERNAME, "" );
		//dlg.m_strPassword	=GetWinscoreStringDecrypt(  SSACONTESTPASSWORD, "" );
		m_pModlessAutoScoreDlg->m_strPassword	=AfxGetApp()->GetProfileString( REGSECTION, SSACONTESTPASSWORD, "" );
		}


	if( m_pModlessAutoScoreDlg->Create( IDD_AUTOMATIC_SCORING, this ) )
		{
		m_pModlessAutoScoreDlg->ShowWindow(SW_SHOW);
		}

	//dlg.DoModal();
}

void CWinscoreView::OnUpdateFlightlogsRechecklateststart(CCmdUI *pCmdUI)
	{
	pCmdUI->Enable(  IsFlightSelected()	);
	}

void CWinscoreView::OnFlightlogsRechecklateststart()
{
	CWaitCursor cWait;
	CFlight* pcFlight=NULL;
	CWinscoreDoc *pDocument=GetDocument();
	CArray<int,int> cIntArray;
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CTime cDate=pFrame->GetDateCombo();
	EClass eClass=pFrame->GetClassCombo();

	if( m_eViewType!=eFlightLogView ) return;

	int nSelected=GetSelectedItems(cIntArray);
	if( nSelected==0 ) return;

	double dRatio= pDocument->GetLogTurninRatio( cDate, eClass);
	if( dRatio<.80 )
		{
		CString strWarn;
		strWarn.Format(_T("Only %2.0f%% of the logs have been turned in.  You should only use this function if 80%% or more logs have been scored.  Continue anyway?"),dRatio*100);
		int iAnswer=AfxMessageBox( strWarn, MB_YESNOCANCEL  );
		if( iAnswer!=IDYES ) return;
		}

	for( int iSel=0; iSel<nSelected; iSel++ )
		{
		CFlight* pcFlight=(CFlight*)GetListCtrl().GetItemData(cIntArray[iSel]);
		if( !pcFlight ) continue;

		Rechecklateststart( pcFlight, pDocument, nSelected>1, true);

		pcFlight->AddToList( GetListCtrl(), TRUE, cIntArray[iSel] );
		}

	pDocument->SetModifiedFlag();
}


bool Rechecklateststart(CFlight* pcFlight,CWinscoreDoc *pDocument, bool bBatch, bool bNeedAnalysis)
{
	int			nMaxRet=15;
	CTime		caStartTimes[15];
	CTimeSpan	caTOCs[15];
	int			iaPenalties[15];
	double		daSpeeds[15];
	double		daDistances[15];
	int			iaPointDiff[15];
	int			iaScore[15];
	int			iBestScorePtr;
	int			nStarts;
	bool		bAlreadyBest;

	CWaitCursor cWait;

	pcFlight->m_pDoc=pDocument;

//	if( !pcFlight->HasStartWarning() )
//		{
//		if( !bBatch ) AfxMessageBox( _T("There are no start warnings for this flight.")); 
//		return true;
//		}

	CTask *pcTask=pDocument->m_taskList.GetByDateClass( pcFlight->GetRollTime(), pcFlight->m_eClass);
	if( !pcTask ) return false;

	if( bNeedAnalysis )
		{
		if( !pcFlight->ReadFlight() )
			{
			if( !bBatch )
				{
				CString strWarn;
				strWarn.Format("IGC file for %s could not be found.\n Cannot check flight without it.",pcFlight->m_strCID);
				AfxMessageBox(strWarn);
				}
			pcFlight->SetIGCFileMissing();
			return false;
			}

		pcFlight->Analyze(	pcTask, 
							pDocument->m_turnpointArray,
							&pDocument->m_contestantList,
							pDocument->m_eUnits);
		}

	nStarts = pDocument->GetMultipleStartPoints(	pcFlight, 
													pcTask,
													iBestScorePtr,
													bAlreadyBest,
													nMaxRet, 
													caStartTimes, 
													caTOCs, 
													daSpeeds, 
													daDistances, 
													iaPointDiff, 
													iaPenalties, 
													iaScore  );


	if( bAlreadyBest )
		{
		if( iaPenalties[iBestScorePtr]==0 ) 
			{
			if( bBatch ) 
				{
				pcFlight->ClearStartWarning();
				pDocument->SetModifiedFlag();
				}
			else
				{
				if( pcFlight->HasStartWarning() )
					{
					CString strWarn;
					strWarn.Format(_T("Flight for %s is already using the best scoring start.  Clear Warning?"),pcFlight->m_strCID);
					int iAnswer=AfxMessageBox( strWarn, MB_YESNOCANCEL  );
					if( iAnswer==IDYES ) 
						{
						pcFlight->ClearStartWarning();
						pDocument->SetModifiedFlag();
						}
					else if( iAnswer==IDCANCEL || iAnswer==IDNO ) 
						{
						return false;
						}
					}
				else
					{
					CString strWarn;
					strWarn.Format(_T("Flight for %s is already using the best scoring start."),pcFlight->m_strCID);
					AfxMessageBox( strWarn, MB_OK | MB_ICONINFORMATION  );
					}
				}
			}
		else
			{
			if( bBatch ) return true;
			CString strWarn;
			strWarn.Format(_T("Flight for %s is already using the best scoring start, but a %d penalty must be applied."),pcFlight->m_strCID,iaPenalties[iBestScorePtr]);
			AfxMessageBox( strWarn  );
			}
		}
	else
		{
		if( bBatch ) return true;
		CString strWarn;
		if( iBestScorePtr>=0 )
			{
			if( iaPenalties[iBestScorePtr]>0 ) 
			  strWarn.Format("The best scoring start for %s ocurs at %s, but a %d point penalty applies.",pcFlight->m_strCID,caStartTimes[iBestScorePtr].Format(_T("%H:%M:%S")),iaPenalties[iBestScorePtr]);
			else
			  strWarn.Format("The best scoring start for %s ocurs at %s, use the Edit function to select it.",pcFlight->m_strCID,caStartTimes[iBestScorePtr].Format(_T("%H:%M:%S")));
			}
		else
			strWarn.Format("A better scoring start for %s may be available, use the Edit function to select it.",pcFlight->m_strCID);
		AfxMessageBox( strWarn);
		}

	pDocument->SetModifiedFlag();

	return true;
}

void  CWinscoreView::OnExportTaskToCUP()
	{
	if( m_eViewType!=eTaskView ) return;

	CTask *pTask = (CTask*)GetSelectedPtr();
	if(pTask==NULL) return;

	CString strDefaultFileName=pTask->TypeText();
	strDefaultFileName+=".cup";


	CWinscoreDoc *pDocument=GetDocument();

    static TCHAR BASED_CODE szFilter[] = _T("SeeYou CUP File (*.cup)|*.cup|All Files (*.*)|*.*||");

	CFileDialog  cFileDlg(FALSE, _T("cup"), strDefaultFileName, OFN_OVERWRITEPROMPT, szFilter);

	if( cFileDlg.DoModal()==IDOK )
		{
		if( pTask->ExportCUP(cFileDlg.GetPathName(),pDocument->m_turnpointArray,pDocument->m_eUnits )==0 )
			{
			CString cStatus;
		    cStatus.Format( _T("%s CUP file successfully written."), cFileDlg.GetPathName()  );
		    AfxMessageBox( cStatus,  MB_ICONINFORMATION   );
			}

		}
	

	}
void  CWinscoreView::OnExportTaskToLibrary()
	{
	if( m_eViewType!=eTaskView ) return;

	CArray<int,int> cIntArray;
	int nSelected=GetSelectedItems(cIntArray);
	if( nSelected==0 ) return;

	CTaskList cList;
	for( int i=0; i<nSelected; i++ )
		{
		CTask* pTask=(CTask*)GetListCtrl().GetItemData(cIntArray[i]);
		if( pTask )
			{
			cList.AddTail(new CTask(pTask));
			}
		}
	int nExported=cList.ExportToTaskLibrary();
	if( nExported>0 )
		{
		CString cStatus;
		cStatus.Format( _T("%d tasks successfully exported."), nExported  );
		AfxMessageBox( cStatus,  MB_ICONINFORMATION   );
		}
}

void CWinscoreView::OnExportAlltaskstoxmlfile()
{
	CWinscoreDoc *pDocument=GetDocument();
	if( pDocument->m_taskList.GetCount()< 1)
		{
		AfxMessageBox( "No tasks have been defined for this contest.",  MB_ICONEXCLAMATION );
		return;
		}
	int nExported=pDocument->m_taskList.ExportToTaskLibrary();
	if( nExported>0 )
		{
		CString cStatus;
		cStatus.Format( _T("%d tasks successfully exported."), nExported  );
		AfxMessageBox( cStatus,  MB_ICONINFORMATION   );
		}
}


void CWinscoreView::OnChangeClass()
	{
	if( m_eViewType!=eContestantView ) return;

	CWinscoreDoc *pDocument=GetDocument();
	int nChanged=0;
	CArray<int,int> cIntArray;
	int nSelected=GetSelectedItems(cIntArray);
	if( nSelected==0 ) return;

	CSelectClassDlg dlg;
	dlg.m_pDoc=GetDocument();
	if (dlg.DoModal() == IDOK)
		{

		int iRead=pDocument->m_cGIList.GetCount();
		if( iRead==0 ) 
			{
			//Cached copy empty, read from local file.
			CString strHCFile=GetHandicapFileName();
			iRead=pDocument->m_cGIList.ImportXML(strHCFile,true);
			}

		for( int i=0; i<nSelected; i++ )
			{
			CContestant* pContestant=(CContestant*)GetListCtrl().GetItemData(cIntArray[i]);
			if( pContestant )
				{
				if( pContestant->m_eClass==dlg.m_eClass ) continue;
				nChanged++;
				if( !GetClass(pContestant->m_eClass).IsHandicapped() &&
					 GetClass(dlg.m_eClass).IsHandicapped()					)
					{
					// Switching from non handicapped to handicapped...
					pContestant->m_eClass=dlg.m_eClass;
					pContestant->UpdateHandicap(pDocument->m_cGIList);
					}
				else
					pContestant->m_eClass=dlg.m_eClass;
				}
			}
		if( nChanged>0 ) 
			{
			ViewContestantList();
			CString strClass=GetClass(dlg.m_eClass).GetName();
			CString strStatus;
			strStatus.Format("%d contestants changed to %s.",nChanged,strClass);
			AfxMessageBox(strStatus,MB_ICONINFORMATION);
			pDocument->SetModifiedFlag();
			}
		}
	}


void CWinscoreView::OnUpdateChangeclass(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(  GetDocument()->GetNumClasses()>1 );	
}


void CWinscoreView::SetNoLogText(CString strReasonText)
{
	if( m_eViewType!=eFlightLogView ) return;

	CWinscoreDoc *pDocument=GetDocument();
	CArray<int,int> cIntArray;
	CMainFrame* pFrame=(CMainFrame*)CWnd::GetParentFrame();
	CTime cDate=pFrame->GetDateCombo();
	CListCtrl& ListCtrl = GetListCtrl();

	int nSelected=GetSelectedItems(cIntArray);
	if( nSelected==0 ) return;

	for( int i=nSelected-1; i>=0; i-- )
		{
		CFlight* pFlight=(CFlight*)GetListCtrl().GetItemData(cIntArray[i]);
		if( !pFlight )
			{
			if( strReasonText.GetLength()>0 )
				{
				ListCtrl.SetItemText(cIntArray[i],2,strReasonText );
				CString cCID=ListCtrl.GetItemText( cIntArray[i], 0);
				WriteWinscoreString(getNoLogKey(cCID, cDate),strReasonText);
				pDocument->SetModifiedFlag();
				}
			else
				{
				ListCtrl.SetItemText(cIntArray[i],2,"No Log Found" );
				CString cCID=ListCtrl.GetItemText( cIntArray[i], 0);
				RemoveWinscoreEntry(getNoLogKey(cCID, cDate));
				pDocument->SetModifiedFlag();
				}
			}
		}
}


void CWinscoreView::OnNologstatusDidnotlaunch()
{
	SetNoLogText("Did Not Launch");
}


void CWinscoreView::OnNologstatusLandoutreported()
{
	SetNoLogText("Landout Reported");
}


void CWinscoreView::OnNologstatusNologfound()
{
	SetNoLogText("");
}


void CWinscoreView::OnNologstatusCustom()
{
	CNoLogFoundStatus dlg;
	dlg.m_strStatusText=GetWinscoreString("DefaultNoLogFoundText","No Log Found");
	if (dlg.DoModal() == IDOK)
		{
		SetNoLogText(dlg.m_strStatusText);
		WriteWinscoreString("DefaultNoLogFoundText",dlg.m_strStatusText);
		}
}


void CWinscoreView::OnUpdateNologstatus(CCmdUI *pCmdUI)
{
	bool bValid=false;
	CFlight* pcFlight=NULL;
	CArray<int,int> cIntArray;
	int nSelected=GetSelectedItems(cIntArray);
	for( int i=nSelected-1; i>=0; i-- )
		{
		CFlight* pFlight=(CFlight*)GetListCtrl().GetItemData(cIntArray[i]);
		if( !pFlight )
			{
			bValid=true;
			break;
			}
		}
	pCmdUI->Enable( bValid );
}


void CWinscoreView::OnFileCheckfornewversionatstartup()
{
	int iNewVer=AfxGetApp()->GetProfileInt(	REGSECTION, CHECKNEWVERSION,  1 );
	if( iNewVer==1 )
		AfxGetApp()->WriteProfileInt(	REGSECTION, CHECKNEWVERSION,  0 );
	else
		AfxGetApp()->WriteProfileInt(	REGSECTION, CHECKNEWVERSION,  1 );
}


void CWinscoreView::OnUpdateFileCheckfornewversionatstartup(CCmdUI *pCmdUI)
{
	int iNewVer=AfxGetApp()->GetProfileInt(	REGSECTION, CHECKNEWVERSION,  1 );
	pCmdUI->SetCheck(iNewVer);
}


void CWinscoreView::OnAutomaticSaveOptions()
{
	CAutosaveOptionsDlg	dlg;
	dlg.m_iRadioOptions		=AfxGetApp()->GetProfileInt(REGSECTION, AUTOSAVEOPTION,	eRemind );
	dlg.m_iRemindHours		=AfxGetApp()->GetProfileInt(REGSECTION, AUTOSAVEREMINDHOURS,  SAVE_REMIND_HOURS );
	dlg.m_iAutosaveHours	=AfxGetApp()->GetProfileInt(REGSECTION, AUTOSAVESAVEHOURS,	SAVE_AUTOSAVE_HOURS );
	if (dlg.DoModal() == IDOK)
		{
		AfxGetApp()->WriteProfileInt(	REGSECTION, AUTOSAVEOPTION,			dlg.m_iRadioOptions );
		AfxGetApp()->WriteProfileInt(	REGSECTION, AUTOSAVEREMINDHOURS,	dlg.m_iRemindHours	);
		AfxGetApp()->WriteProfileInt(	REGSECTION, AUTOSAVESAVEHOURS,		dlg.m_iAutosaveHours);
		}
}

void CWinscoreView::OnLogstatusCheckedandok()
{
	if( m_eViewType!=eFlightLogView ) return;

	CWinscoreDoc *pDocument=GetDocument();
	CArray<int,int> cIntArray;
	CListCtrl& ListCtrl = GetListCtrl();

	int nSelected=GetSelectedItems(cIntArray);
	if( nSelected==0 ) return;

	for( int i=nSelected-1; i>=0; i-- )
		{
		CFlight* pFlight=(CFlight*)ListCtrl.GetItemData(cIntArray[i]);
		if( pFlight && !pFlight->CheckOption(FLT_CHECKED) )
			{
			pFlight->SetOption(FLT_CHECKED);
			CString cStatus=ListCtrl.GetItemText( cIntArray[i], 2);
			cStatus +=" <ok>";
			ListCtrl.SetItemText(cIntArray[i],2,cStatus );
			pDocument->SetModifiedFlag();
			}
		}
}

void CWinscoreView::OnSetFDRID()
	{
	if( m_eViewType!=eFlightLogView && m_eViewType!=ePreContestView ) return;

	CWinscoreDoc *pDocument=GetDocument();
	CArray<int,int> cIntArray;
	CListCtrl& ListCtrl = GetListCtrl();

	int nSelected=GetSelectedItems(cIntArray);
	if( nSelected==0 ) 
		{
		AfxMessageBox( "No Log Found.",  MB_ICONINFORMATION   );
		}

	int nChanged=0;
	for( int i=nSelected-1; i>=0; i-- )
		{
		CFlight* pFlight=(CFlight*)ListCtrl.GetItemData(cIntArray[i]);
		if( pFlight )
			{
			CString strCID = pFlight->m_strCID;
			CContestant *pContestant = pDocument->m_contestantList.GetByContestNo(strCID);
			if( !pContestant ) continue;
			pContestant->SetFDRID(	pFlight->m_strFDRID, 
									pFlight->m_iENLMax, 
									pFlight->m_iENLMin,
									pFlight->m_iMOPMax,
									pFlight->m_iMOPMin );

			if( m_eViewType==eFlightLogView )
				pFlight->AddToList( GetListCtrl(), TRUE, cIntArray[i] );
			else
				{
				pFlight->AddToPreContestList( GetListCtrl(), pDocument->m_contestantList.GetByContestNo(pFlight->m_strCID), TRUE, cIntArray[i] );
				}

			pDocument->SetModifiedFlag();
			nChanged++;
			}
		}
	if( nChanged>0 )
		{
		CString cStatus;
		cStatus.Format( _T("%d Contestant(s) FDR IDs successfully updated."), nChanged );
		AfxMessageBox( cStatus,  MB_ICONINFORMATION   );
		return;
		}
	if( nSelected==1 ) 
		{
		AfxMessageBox( "No Log Found or Not a Contestant.",  MB_ICONINFORMATION   );
		}
	}

void CWinscoreView::OnUpdateSetFDRID(CCmdUI* pCmdUI)
	{
	pCmdUI->Enable(  IsFlightSelected()	);
	}