//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Flight.h: interface for the CFlight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLIGHT_H__6F701641_D984_11D2_B0BD_0080C84B0161__INCLUDED_)
#define AFX_FLIGHT_H__6F701641_D984_11D2_B0BD_0080C84B0161__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "Winscore.h"	
#include "Turnpoint.h"
#include "task.h"	
#include "tasklist.h"	

#define   CONTESTANTLISTCLASS	CContestantList
#define   TASKCLASS				CTask
#define   TURNPOINTCLASS		CTurnpoint
#define   TURNPOINTCLASSARRAY   CTurnpointArray

#include "contestantlist.h"	


#include "IGCFile.h"
#include "Location.h"

#include "gate.h"	
#include "WinscoreDefs.h"	// Added by ClassView
#include <afxtempl.h>
#include "warning.h"


#define FORWARD	   0
#define BACKWARD   1

//#define FLT_ROLLINGFINISH		0x00001
#define FLT_AIRFIELDBONUS		0x00002
#define FLT_SLANDINGPOINTLOCKED	0x00004
#define FLT_SECURITYPASSED		0x00008
#define FLT_SECURITYFAILED		0x00010
#define FLT_ROLLTIMELOCKED		0x00020
#define FLT_STARTTIMELOCKED		0x00040
#define FLT_FINISHTIMELOCKED	0x00080
#define FLT_LANDINGTIMELOCKED	0x00100
#define FLT_DISTANCELOCKED		0x00200
#define FLT_TURNPOINTSLOCKED	0x00400
#define FLT_AIRFIELDBONUSLOCKED	0x00800
#define FLT_SAFETYFINISH		0x01000
#define FLT_OPTTURNPOINTSLOCKED	0x02000
#define FLT_CHECKED				0x04000
#define FLT_MISSING_ENL			0x08000

enum EFlightStatus {	eNotAnalyzed,
						eNoTaskSpecified,
						eDidNotStart,
						eIncompleteTask,
						eCompletedTask,
						eNotEnteredInContest,
						eNoLandingCardSpecified,
						eNoData,
						eBelowFinishCylinder};


class CFlight : public CIGCFile  
{
public:

	CFlight(CString strIGCFileName);
	CFlight();
	CFlight(CFlight *pcFlight);
	virtual ~CFlight();


/////////////////////  Data Setup  /////////////////////////////////

	void	SetStartPoint(	TURNPOINTCLASS *pcStartControlPoint );
	void	SetFinishPoint( TURNPOINTCLASS *pcFinishControlPoint );
	void	SetTaskPoint(	TURNPOINTCLASS *pcTaskPoint );
	void	SetHomePoint(	TURNPOINTCLASS *pcHomePoint );

/////////////////////  Analysis Functions //////////////////////////
public:


	bool Analyze(	TASKCLASS			*pcTask, 
				TURNPOINTCLASSARRAY &turnpointArray,
				CONTESTANTLISTCLASS	*contestantList,
				EUnits				eUnits,
				bool				bSilent=false);


	void	AssignPositionStatus(	TASKCLASS* pcTask, 
									bool bAutoTask, 
									TURNPOINTCLASSARRAY &turnpointArray);

	void CalculateFlightAverages(	int  &iMinAltitude, 
									int  &iMaxAltitude, 
									bool &bNoAltitudeInfo,
									CArray<double,double> &dSpeedArray);

private:

	void	FindStartsAndFinish(CTask *pcTask);
	int		FindAcheivedTurnpoint(int iEvent, int iTpt, int iStartPos, int iDirection=FORWARD, int iTPPos=-1);
	CTime	InterpolateStartPoints( CPosition *pPos, CPosition *pPrevPoss, CLocation &cStartFix );
	CTime	InterpolateFinishPoints( CPosition *pPos, CPosition *pPrevPos );
	CTime   InterpolateStartAltitude(CPosition *pPos, CPosition *pPrevPos, CLocation &cStartFix );
	CTime   InterpolateFinishAltitude(CPosition *pPos, CPosition *pPrevPo  );
	int		InterpolateFinishPressureAlt(CPosition *pPos, CPosition *pPrevPos );

	int		FindEvent( int iEvent, int iStart, int iDirection, BOOL bSet=TRUE );
	int		FindTime(CTime cTime, int iStart, int iDirection);
	int		FindClosestPointToStart(int iStart  );
	int		FindClosestPointToTurnpoint( int iTurnpoint, bool bCheckOnlyAtTurnpoint=false, int iMaxCheck= -1 );
	int		FindPointInStartCyl(int iStart, double dRadius );


	//CPosition*  FindFurthestProgress(int iStart, int iTurnpointID);
	CPosition*  FindFurthestProgress(int iStart, CLocation &cLoc);
	CPosition*  FindFurthestFAIProgress(int iStart, CLocation &cLocTo, CLocation &cLocFrom, double dRadius=1.0 );
	bool		FindAcheivedTurnpoints(TASKCLASS *pcTask);
	void		FindLandingLocation();
	void        FindBetterStart(TASKCLASS *pcTask, TURNPOINTCLASSARRAY &turnpointarray);


/////////////////////  Query results of Analysis ///////////////////
public:

	void CheckStartPenalty(TASKCLASS* pcTask);
	void CheckFAIStartAltitudes(TASKCLASS* pcTask);

	void	InitializeAnalysisParams(EUnits	eUnits=eStatute);
	bool	IsContestant();
	void	SetStatus( EFlightStatus eStatus);

	CString GetLandingTimeText(bool bAddDot=false);
	CString GetRollTimeText(bool bAddDot=false);
	CString GetScoredLandingPointText(bool bAddDot=false);
	CString GetLandingLocationText(bool bAddDot=false);
	CString GetStartTimeText(bool bAddDot=false);
	CString GetFinishTimeText(bool bAddDot=false);
	CString GetTaskOpenTimeText();
	CString AcheivedTurnpointText( bool bUseMemberPoints=TRUE, int nPoints=0, bool bAddDot=false );
	CString GetStatusText();
	
	CSemaphore m_cSem_Analyze, m_cSem_AddToList;

	EFlightStatus GetStatus()	{return m_eStatus;};
	int		GetAcheivedTurnpoint(int i);
	int		GetNumAcheivedTurnpoints();
	void	SetNumAcheivedTurnpoints(int i);
	int		GetNumTaskTurnpoints()	{ return m_nTaskPoints; };
	void	SetTaskOpenTime(CTime &cTaskOpenTime);
	CTime	GetTaskOpenTime();

	void	SetAltitudeControlTime( CTimeSpan &cTimeSpan);

	bool	AllTurnpointsAcheived();
	bool	IsTaskComplete();
	bool	IsFinishTimeValid(){return AllTurnpointsAcheived() && m_bValidFinish && m_cFinishTime>0;};
	bool	IsStartTimeValid() {return m_bValidStart; };
	bool    IsLocked() {return m_bLocked;}

	CTime		GetLandingTime();
	CTime		GetRollTime();
	CTime		GetFinishTime();
	CTime		GetStartTime();
	int			GetLandoutTurnpointID();
	void		SetLandoutTurnpointID(int i) { m_iLandoutAirfield=i; };
	void		SetLandoutTurnpointName(CString str) { m_strLandoutAirfield=str; };

	void		SetLandingTime(CTime &cTime) { m_cLandingTime=cTime; };
	void		SetRollTime(CTime &cTime)	 { m_cRollTime=cTime; };
	void		SetFinishTime(CTime &cTime)  { m_cFinishTime=cTime; };
	void		SetStartTime(CTime &cTime)   { m_cStartTime=cTime; };

	void		SetStartValidity(bool b)   { m_bValidStart=b; };
	void		SetFinishValidity(bool b)   { m_bValidFinish=b; };
	void		SetTurnpointsAchevied(bool b)   { m_bAllTurnpointsAcheived=b; };
	void		SetLock(bool b)   { m_bLocked=b; };

	CTime		LastValidStartTime( );
	int			NumberOfWarnings( );
	int			NumberOfActiveWarnings();
	void		RemoveWarning(int iWarn);
	CWarning*	GetWarning(int iWarn );
	int			GetWarning(EWarningType eWarnType );
	CLocation&	GetLandingLocation();

	int AddToList(CListCtrl & ListCtrl, BOOL fVisible=FALSE, int iItem= -1 );


/////////////////////  Input Data    ///////////////////
public:

	ETaskType	 m_eTaskType;
	CGate		 m_cFinishGate;
	CGate		 m_cStartGate;
	int			 m_nTaskPoints;
	int			 m_nMATTaskPoints;
	void*		 m_pDoc;

private:

	TURNPOINTCLASS*	 m_pcStartControlPoint;
	TURNPOINTCLASS*  m_pcFinishControlPoint; 

	TURNPOINTCLASS	*m_pcHomePoint;
	TURNPOINTCLASS	*m_pcTaskTurnpoints[ALLOCTASKTPS];
	int			 m_aiTaskTurnpointIDs[ALLOCTASKTPS];
	double		 m_adTurnpointRadii[ALLOCTASKTPS];
	
	CTimeSpan	 m_cAltitudeControlTime;
	CTime		 m_cTaskOpenTime;

	int			m_iOptions;

	double		 m_dTurnpointRadius;
	double		 m_dTurnpointOuterRadius;

/////////////////////  Results of Analysis ///////////////////
public:
	CTurnpoint* GetHomePoint();
	CTimeSpan TimeRemainingInLog(int iPoint);
	CFlight(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pFlight,CTaskList &cTaskList);
	void GetXML(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr &pParentNode);

	bool RetardStartTime(int iStartPoint, CTimeSpan &cRetard );
	BOOL CheckOption(int iOption);
	void ClearOption(int iOption);
	void SetOption(int iOption);
	void CheckSecurity();
	bool CheckSecurity2(CString &cResult);

	bool IsAreaTask()		{ return m_eTaskType==eTurnArea			|| m_eTaskType==eFAIAssignedArea; }
	bool IsTimedTask()		{ return m_eTaskType==eTurnArea			|| m_eTaskType==eFAIAssignedArea || m_eTaskType==eModifiedAssigned; }
	bool IsAssignedTask()	{ return m_eTaskType==eAssigned			|| m_eTaskType==eFAIRacing ; }
	bool IsFAITask()		{ return m_eTaskType==eFAIAssignedArea	|| m_eTaskType==eFAIRacing ; }

	BOOL SecurityPassed() { return m_iOptions & FLT_SECURITYPASSED;}
	BOOL SecurityFailed() { return m_iOptions & FLT_SECURITYFAILED;}
	void SetSecurityFailed() 
		{ m_iOptions|= FLT_SECURITYFAILED;
		  if( SecurityPassed() ) m_iOptions^= FLT_SECURITYPASSED;}
	void SetSecurityPassed() 
		{ m_iOptions|= FLT_SECURITYPASSED;
		  if( SecurityFailed() ) m_iOptions^= FLT_SECURITYFAILED; }


	CString GetAirfieldBonusText(bool bAddDot=false);
	int GetFinishGateID();
	int GetStartGateID();
	void SetStartGateID(int  iGate);

	bool CheckAirspaceIncursions(TURNPOINTCLASSARRAY &TURNPOINTCLASSArray);
	int GetTurnAreaFixID( int iTP );
	void SetTurnAreaFixID( int iTP, int iFix );
	CLocation&	GetTurnAreaLocation(int iTP);
	void		SetTurnAreaLocation(int iTP, CLocation &cLoc);

	bool NotAnalyzed();
	CString GetTurnAreaText(TURNPOINTCLASSARRAY *pTURNPOINTCLASSArray);
	void SetTurnpointRadius(int i, double d);
	double GetTurnpointRadius(int i);
	CString GetStartGateText();
	int GetSoughtTurnpoint();
	void SetSoughtTurnpoint(int iSought);
	double ComputeDistance(	TASKCLASS *pcTask, 
							TURNPOINTCLASSARRAY &cturnpointArray);
	double GetDistance(	);
	void   SetDistance(double d) {m_dDistance=d;};
	CString GetDistanceText(bool bAddDot=false);
	void FindOptimumTurnpoints(TASKCLASS *pcTask, TURNPOINTCLASSARRAY &cTurnpointArray);
	bool GetFlightTimeRemaining(int iPos, CTimeSpan &cTimeSpan);
	bool AutoFindTaskPoints(TURNPOINTCLASSARRAY &turnpointArray);
//	void RollingFinish(bool b);
//	bool IsRollingFinish();
	void AirfieldBonus(bool b);
	bool IsAirfieldBonus();
	EUnits m_eUnits;

	CLocation&  GetScoredLandingPointLocation();
	void SetScoredLandingPointLocation(CLocation& cLoc) {m_cScoredLandingPointLocation=cLoc;};
	int  GetStartHeightPenalty( int iStartPoint, int &iControlHeight, int &MSH );
	int GetCandidateStarts( int iMaxReturn, CTime caStartTimes[], CTimeSpan caTOCs[], int iaPenalties[], CTask *pcTask=NULL );

	void LocateFurthestProgess(TASKCLASS *pcTask, CGate &cFinish, TURNPOINTCLASSARRAY &cTurnpointArray);
	int GetTaskPointID(int iTskPt);
	void SetAcheviedTurnpoint(int i, int IID);
	void CheckMotorRun();
	void CheckBFI();
	bool UpdateCID(CONTESTANTLISTCLASS *contestantList);
	void CheckAllTimes();
	CString GetFlightText(TASKCLASS *pcTask=NULL, TURNPOINTCLASSARRAY *pcTurnpointArray=NULL, CString strSeparator="\t");
	void SetLandingLocation( CLocation &cLocation );
	CString GetTaskPointText( int nDisplay );
	CString GetNumWarningsText();
	void FindStartFix();
	void ClearStartWarning();
	bool HasStartWarning();
	bool MotorRunLandoutWarning();
	void	  GetWarningsArray(CStringArray &strArray);
	void	  SetWarningsArray(CStringArray &strArray);
	void	  ResetAllWarnings();
	void	  AddWarning(EWarningType eType, int iPenalty, CString strWarning);

	CString		m_strCID;
	EClass		m_eClass;
	CLocation   m_cStartFix;

	CLocation	m_acTurnAreaTurnpointLocations[ALLOCTASKTPS];
	int			m_aiTurnAreaTurnpointFixIDs[ALLOCTASKTPS+2];

private:

	void CheckSafetyFinish();
	void CheckGaps();

	bool		  m_bAllTurnpointsAcheived;
	bool		  m_bValidFinish;
	bool		  m_bValidStart;
	bool		  m_bLocked;

	int			  m_iStartPenalty;
	CTime		  m_cRollTime;
	CTime		  m_cStartTime;
	CLocation	  m_cScoredLandingPointLocation;
	CTime		  m_cFinishTime;
	CLocation	  m_cFinishLocation;
	CLocation	  m_cLandingLocation;
	CTime		  m_cLandingTime;
	EFlightStatus m_eStatus;
	CTypedPtrArray<CPtrArray,CWarning*>	m_cWarningArray;
	int			  m_nAcheivedTurnpoints;
	int			  m_aiAcheivedTurnpoints[ALLOCTASKTPS];
	int			  m_iSoughtTurnpoint;
	double		  m_dDistance;

	CString		  m_strLandoutAirfield;
	int			  m_iLandoutAirfield;

};

int CALLBACK CompareFlight(LPARAM lParam1, LPARAM lParam2, LPARAM iColumn);

#endif // !defined(AFX_FLIGHT_H__6F701641_D984_11D2_B0BD_0080C84B0161__INCLUDED_)

