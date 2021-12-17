//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Winscore.h : main header file for the WINSCORE application
//

#if !defined(AFX_WINSCORE_H__BAD124DA_4095_11D1_ACCF_000000000000__INCLUDED_)
#define AFX_WINSCORE_H__BAD124DA_4095_11D1_ACCF_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "winscoredefs.h"
#include "afxwinappex.h"

#define  MAX_CONTESTANTS	125
#define  MAX_TURNPOINTS		999
#define  DB_SCHEMA_ID		1
	
#define NUM_CONTESTANTS_PER_PAGE  28
#define NUM_SCORES_PER_PAGE		  40
#define NUM_EVENTS_PER_PAGE		  38

#define DRAW_VERTICAL( X, Y1, Y2)  pDC->MoveTo(X,Y1); pDC->LineTo(X,Y2);
#define DRAW_HORIZONTAL( Y )  \
{\
pDC->MoveTo(0,Y); \
pDC->LineTo(GetPageWidth(pDC),Y);\
}\

#define	TEXT_OUT( TEXT, X, Y) {CString cTEXT=TEXT; pDC->TextOut( X+5, Y-2, cTEXT, cTEXT.GetLength() );}


#define DELETE_KEY   46
#define  ASCII251	_T("\xFB\r\n")

// Registry settings
#define REGSECTION		_T("Settings")
#define REGCONTEST		_T("LastContest")
#define REGSECTION		_T("Settings")
#define TASKPATH		_T("TaskLibraryPath")
#define LOGPATH			_T("FlightLogPath")
#define LOGPATHNUMFOLDERS _T("NumFolders")
#define DATEDLOGPATHS   _T("LogFolderByDate")
#define COMPORT			_T("ComPort")
#define GUESTPOSITION	_T("GuestsAtBottom")
#define ALLOWTWOGATES	_T("AllowTwoStartGates")
#define EXPORTPATH		_T("ExportPath")
#define IMPORTINDEX		_T("ImportFilterIndex")
#define IMPORTPATH		_T("ImportPath")
#define DIALOGDISPLAY	_T("DialogDisplay")
#define CHECKNEWVERSION _T("CheckForNewVersion")

#define FTPCONDENSE		_T("FTPCondense2")
#define FTPCONDENSESEC	_T("FTPCondenseSec2")
#define FTPTRANSFERTOP	_T("FTPTransferTop")
#define SSACONTESTUSERNAME _T("SSAUsername")
#define SSACONTESTPASSWORD _T("SSAPassword")
#define SSACONTESTREMEMBER _T("SSARemember")

#define EMAILUSERNAME	_T("EmailUsername")
#define EMAILPASSWORD	_T("EmailPassword")
#define EMAILSERVER		_T("EmailServer")
#define EMAILMAILBOX	_T("EmailMailbox")
#define EMAILPORTID		_T("EmailPortID")
#define EMAILSSL		_T("EmailUseSSL")

#define AUTOSAVEOPTION	_T("AutosaveOption")
#define AUTOSAVEREMINDHOURS _T("AutosaveRemindHours")
#define AUTOSAVESAVEHOURS   _T("AutosaveSaveHours")


#define INNERRADIUS		_T("TurnpointInnerRadius")
#define OUTERRADIUS		_T("TurnpointOuterRadius")
//#define REGNAMENAME		_T("RegisteredName")
//#define REGNUMBER		_T("RegisteredNumber")

#define DEFAULTINNERRADIUS		1.0
#define DEFAULTOUTERRADIUS		2.0


#define SUADISPLAY		_T("SUADisplay")
#define SUA_IGNORE		_T("SUAIgnore")
#define SUADISPLAYFILE	_T("SUADisplayFile")
#define DEFAULT_SUADISPLAYFILE	_T("conus.sua")
#define DEFAULT_SUADISPLAY		1
#define DEFAULT_SUA_IGNORE		1

#define LOGTIMEOPTION		_T("LogTimeOption")
#define LOGTIMEUSERVALUE	_T("LogTimeUserValue")

#define AUTOEXPORTENABLEDFLAG	_T("AutoExportEnabledFlag")
#define AUTOEXPORTPATH			_T("AutoExportPath")

#define CONTROLBARVISIBLE       _T("ControlBarVisible")

#define AUTOSCORE_DURATION		_T("AutoScoreDuration")
#define AUTOSCORE_EMAIL			_T("AutoScoreEmail")
#define AUTOSCORE_MEDIA			_T("AutoScoreMedia")
#define AUTOSCORE_ANALYSIS		_T("AutoScoreAnalysis")
#define AUTOSCORE_SCORES		_T("AutoScoreScores")
#define AUTOSCORE_AUTOSAVE		_T("AutoScoreAutoSave")



enum EFieldID {	eUnknown,
				eDate,
				eClass,
				eTaskDefinition,
				eTaskControlPointList,
				eTaskStartSpecification,
				eTaskFinishSpecification,
				eCompetitionID,
				eClaimedTask,
				eTakeoffTime,
				eStartTime,
				eFinishTime,
				eLandingInformation,
				eScoredLandingInformation,
				eScoreAdjustment,
				eTaskOpenTime,
				eCommentField		};

enum EPenality {eDayPoint=0, eDayPercent=1, eContestPenalty=2, eComment=3};

enum ESortBy{eDaily, eCumulative};
enum EScoresheetType {eDay, eSummary};

enum EPrintSelection {	eDailyScoresheet=0,  
						eContestantInfo=1, 
						eTurnpointSummary=2,
						eTakeoffGridPositions=3,
						eInvalidTaskLeg=4,
						eEvents=5,
						eTaskSheet=6 };

enum EExportType {eText=2, eXML=3};

#define  NVIEWS		7

enum EViews {	eContestantView			=0,
				eTurnpointView			=1,
				eTurnpointGridView		=2,
				eTakeoffGridView		=3,
				eTaskView				=4,
				eFlightLogView			=5,
				eScoreView				=6 };

static _TCHAR *_gszViewNames[NVIEWS] =
{
	_T("Contestants"), 
	_T("Control Points"), 
	_T("Distance Grid"),
	_T("Takeoff Grid"),
	_T("Tasks"),
	_T("Flight Logs"),
	_T("Scores")
 };

#define m_to_ft  3.281

/////////////////////////////////////////////////////////////////////////////
// CWinscoreApp:
// See Winscore.cpp for the implementation of this class
//

class CWinscoreApp : public CWinAppEx
{
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CWinscoreApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinscoreApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWinscoreApp)
	afx_msg void OnAppAbout();
	afx_msg void OnHelpIndex();
	afx_msg void OnAppExit();
	afx_msg void OnHelpWinscoremanual();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:




};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINSCORE_H__BAD124DA_4095_11D1_ACCF_000000000000__INCLUDED_)


