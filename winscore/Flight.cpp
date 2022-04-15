//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Flight.cpp: implementation of the CFlight class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <atlbase.h>

#include "Global_Prototypes.h"

#include "winscoredoc.h"
#include "sua.h"
#include "suautil.h"
#include "DontShowDlg.h"

#include "position.h"
#include "IGCFile.h"
#include "Flight.h"

#include "averager.h"
#include <math.h>
#include <afxtempl.h>
#include "median.h"
#include "xmlmgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static int	iMSLCorrection	=0;
static int	iMSLFinishCorrection=0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFlight::CFlight()
	{
	m_eStatus=eNotAnalyzed;
	m_eClass=eStandard;
	InitializeAnalysisParams();
	m_pDoc=NULL;
	}

CFlight::CFlight(CString strIGCFileName) : CIGCFile(strIGCFileName)
	{
	m_eStatus=eNotAnalyzed;
	m_eClass=eStandard;
	InitializeAnalysisParams();
	m_pDoc=NULL;
	}

CFlight::~CFlight()
	{
	ResetAllWarnings();
	}

void CFlight::SetTaskOpenTime(CTime &cTaskOpenTime)
	{
	m_cTaskOpenTime=cTaskOpenTime;
	}

void CFlight::SetAltitudeControlTime(CTimeSpan &cTimeSpan)
	{
	m_cAltitudeControlTime=cTimeSpan;
	}

void CFlight::SetStartPoint(TURNPOINTCLASS *pcStartControlPoint)
	{
	if( !pcStartControlPoint ) return;
	m_pcStartControlPoint	=pcStartControlPoint;
	}

void CFlight::SetFinishPoint(TURNPOINTCLASS *pcFinishControlPoint )
	{
	if( !pcFinishControlPoint ) return;
	m_pcFinishControlPoint=pcFinishControlPoint; 
	}


void CFlight::SetTaskPoint( TURNPOINTCLASS *pcTaskPoint )
	{
	if( !pcTaskPoint ) return;

	if( m_nTaskPoints<ALLOCTASKTPS )
		{
		m_pcTaskTurnpoints[m_nTaskPoints]=pcTaskPoint;
		m_aiTaskTurnpointIDs[m_nTaskPoints++]=pcTaskPoint->GetID();
		}
	}

void CFlight::SetHomePoint( TURNPOINTCLASS *pcHomePoint )
	{
	m_pcHomePoint=pcHomePoint;
	}


/////////////////////  Analysis functions ///////////////////////// 

	bool CFlight::Analyze(	TASKCLASS			*pcTask, 
							TURNPOINTCLASSARRAY &turnpointArray,
							CONTESTANTLISTCLASS *pContestantList,
							EUnits				eUnits,
							bool				bSilent	)
	{
	CSingleLock cLock(&m_cSem_Analyze);

	CContestant* pcContestant=NULL;	
	m_eUnits=eUnits;
	m_nAcheivedTurnpoints=0;
	ResetAllWarnings();
	CWSClass &cClass=GetClass(m_eClass);

	// If not locked (reanalysis) initialize stuff.
	if( !IsLocked() )
		{
		InitializeAnalysisParams(eUnits);
		}

	if( pcTask!=NULL ) m_eTaskType=pcTask->m_eType;


	if( GetNumPoints()==0 )
		{
		m_eStatus=eNoData;
		AddWarning(eInformation,0,"There were no data points in the flight log, or the program encountered an error reading the IGC file.");
		return false;
		}

	if( pcTask==NULL )
		{
		m_eStatus=eNoTaskSpecified;
		return false;
		}

	if( pContestantList )
		{
		pcContestant=pContestantList->GetByContestNo(m_strCID);
		if( pcContestant==NULL )
			{
			m_eStatus=eNotEnteredInContest;
			return false;
			}
		m_eClass=pcContestant->m_eClass;
		if( !IsLocked() )
			m_cStartGate=pcTask->GetActiveStartGate(pcContestant);
		}

	//  Initialize Gates

	if( m_cStartGate.IsPerpToCourse() )
			{
			// Make start gate perp. to first course line.
			m_pcStartControlPoint	=turnpointArray[m_cStartGate.GetPointID()-1];
			TURNPOINTCLASS* p1st    =(TURNPOINTCLASS*)turnpointArray[pcTask->GetTurnpointID(0)-1];
			if( m_pcStartControlPoint && p1st )
				{
				CLocation cLoc1(m_pcStartControlPoint->GetLat(), m_pcStartControlPoint->GetLong());
				CLocation cLoc2(p1st->GetLat(), p1st->GetLong());
				m_cStartGate.SetHeading(cLoc1.CourseTo(cLoc2));
				}
			}
	m_cStartGate.Initialize( *(turnpointArray[m_cStartGate.GetPointID()-1]) );
	m_pcStartControlPoint	=turnpointArray[m_cStartGate.GetPointID()-1];

	m_cFinishGate=pcTask->m_cFinishGate;
	m_cFinishGate.Initialize( *(turnpointArray[m_cFinishGate.GetPointID()-1]) );
	m_pcFinishControlPoint	=turnpointArray[m_cFinishGate.GetPointID()-1]; 


	SetTaskOpenTime(pcTask->m_cGateOpenTime);
	m_cStartFix=CLocation(m_pcStartControlPoint->GetLat(),m_pcStartControlPoint->GetLong()) ;

	TURNPOINTCLASS *pcHome=turnpointArray.GetHomePoint(m_pcStartControlPoint);
	if( pcHome==NULL )
		{
		AfxMessageBox(_T("No control point has the \"H\" (home) attribute set.\n Cannot perform an analysis without the home control point defned.") );
		return false;
		}
	SetHomePoint( pcHome );

	if( !CheckOption(FLT_TURNPOINTSLOCKED) )
		{

		if( m_eTaskType==eModifiedAssigned ) 
			m_nMATTaskPoints=pcTask->GetNumTurnpoints();

		m_nTaskPoints=0;
		for( int iTpt=0; iTpt<pcTask->GetNumTurnpoints(); iTpt++ )
			{
			SetTaskPoint( turnpointArray[pcTask->GetTurnpointID(iTpt)-1] );
			if( IsAreaTask() )
				SetTurnpointRadius( iTpt, pcTask->GetTurnpointRadius(iTpt) );
			else
				SetTurnpointRadius( iTpt, m_dTurnpointRadius );
			}
		}
	else
		{
		//  Turnpoints are locked   
		
		m_nMATTaskPoints=m_nTaskPoints;


		// Refresh Pointers for the task list.
		for( int i=0; i<ALLOCTASKTPS; i++ )
			{
			TURNPOINTCLASS* pcTaskPoint = (TURNPOINTCLASS*)turnpointArray[m_aiTaskTurnpointIDs[i]-1];
			m_pcTaskTurnpoints[i]=pcTaskPoint;
			}
	
		}



	// Safety Finish, set finish to be a 5 sm cylinder
	if( CheckOption(FLT_SAFETYFINISH) ) 
		{
		m_cFinishGate.SetGateType(eCylinder);
		m_cFinishGate.SetRadius( 0. );
		}

	try { 
		AssignPositionStatus(pcTask, false, turnpointArray);
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::AssignPositionStatus"));
		throw;
		}

	try {
		FindStartsAndFinish(pcTask);
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::FindStartsAndFinish"));
		throw;
		}

	try {
		if( pcContestant && pcContestant->IsMotorized() ) CheckMotorRun();
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::CheckMotorRun"));
		throw;
		}

	try {
		if( pcContestant ) CheckBFI();
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::CheckBFI"));
		throw;
		}

	try {
		FindAcheivedTurnpoints(pcTask);
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::FindAcheivedTurnpoints"));
		throw;
		}
	
	try {
		FindLandingLocation();
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::FindLandingLocation"));
		throw;
		}

		try {
		FindOptimumTurnpoints(pcTask, turnpointArray);
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::FindOptimumTurnpoints"));
		throw;
		}
	
	try {
		FindBetterStart(pcTask, turnpointArray);
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::FindBetterStart"));
		throw;
		}


	try {
		CheckStartPenalty(pcTask);
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::CheckStartPenalty"));
		throw;
		}

	try {
		CheckGaps();
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::CheckGaps"));
		throw;
		}

	try {
		CheckAllTimes();
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::CheckAllTimes"));
		throw;
		}

	try {
		if( !CheckOption(FLT_SLANDINGPOINTLOCKED) )
			{
			LocateFurthestProgess(pcTask,m_cFinishGate,turnpointArray);
			}
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::LocateFurthestProgess"));
		throw;
		}


	try {
		if( CheckAirspaceIncursions( turnpointArray) )
			{
			// Warn of airspace incursion.
			if(!bSilent )
				{
				CString strDlgReg=_T("SUAWARN_")+this->m_strFileName;
				CString strMessage=_T("Possible airspace violation by: ")+this->m_strCompetitionID+_T(" - ")+this->m_strPilot;
				CDontShowDlg dlg(strDlgReg,strMessage);
				dlg.DoModalConditional();
				}
			}
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::CheckAirspaceIncursions"));
		throw;
		}


	if( IsTaskComplete() )
		m_eStatus=eCompletedTask;
	else
		{
		if( IsStartTimeValid() )
			{
			if( m_eStatus!=eBelowFinishCylinder) m_eStatus=eIncompleteTask;
			}
		else
			m_eStatus=eDidNotStart;
		}


	if( !CheckOption(FLT_AIRFIELDBONUSLOCKED) && cClass.IsAirfieldBonusActive() )
		{
		AirfieldBonus(false);
		if( MotorRunLandoutWarning() )
			{
			// Landout due to motor run, check 1mi and 800ft
			TURNPOINTCLASS *pcLandout=turnpointArray.FindClosestAirport(&m_cLandingLocation, ConvertDistance(1.0, eStatute, SYSTEMUNITS) );
			if( pcLandout )
				{
				int iAltitudeAbove=0;
				// Find motor on altitude
				int iLandingPoint=FindEvent( FAN_LANDED, 0, FORWARD );
				CPosition *pcPos=GetPosition(iLandingPoint);
				if( pcPos )
					{
					iAltitudeAbove=pcPos->m_iCorrectedAltitude-pcLandout->m_iElevation;
					if( iAltitudeAbove>800 )
						{
						m_iLandoutAirfield=pcLandout->m_iID;
						m_strLandoutAirfield=pcLandout->m_strName;
						if( !IsTaskComplete() ) 
							{
							AirfieldBonus(true);
							CString strMess;
							strMess.Format("Airfield bonus applied due to motor run %ift above and within 1 mile of %i - %s",iAltitudeAbove,pcLandout->m_iID,pcLandout->m_strName);
							AddWarning(eInformation,0,strMess );
							}
						}
					}
				}
			}
		else
			{
			// See if he landed close to a turnpoint
			TURNPOINTCLASS *pcLandout=turnpointArray.FindClosestAirport(&m_cLandingLocation, ConvertDistance(AIRBONTOLER, eStatute, SYSTEMUNITS) );
			if( pcLandout )
				{
				m_iLandoutAirfield=pcLandout->m_iID;
				m_strLandoutAirfield=pcLandout->m_strName;
		//		m_cLandingLocation=*pcLandout;
				if( !IsTaskComplete() ) AirfieldBonus(true);
				}
			}
		}

	// Check for below finish.
	if( GetStatus()==eBelowFinishCylinder )
		{
		// Mark as rolling Finish.
		m_cFinishTime=m_cLandingTime;
		m_bValidFinish=false;
		if( cClass.IsAirfieldBonusActive() ) AirfieldBonus(true);
		m_cLandingLocation=m_cFinishLocation;
		AddWarning(eFinish,0,_T("Crossed Below Finish Cylinder."));
		}


	if( !MotorRunLandoutWarning()												&&
		!CheckOption(FLT_LANDINGTIMELOCKED)										&&
		!m_cFinishGate.IsGPSCylinder()											&&
		m_pcFinishControlPoint													&&
		m_pcFinishControlPoint->DistanceTo(m_cLandingLocation, eStatute)<1.		&&
		m_bAllTurnpointsAcheived												&&
		!m_bValidFinish)
		{
		// He acheived all turnpoints and landed close to the finish,
		// Mark as rolling Finish.
		m_cLandingTime=m_cLandingTime+CTimeSpan(120);
		m_cFinishTime=m_cLandingTime;
		m_bValidFinish=true;
		m_eStatus=eCompletedTask;
		AirfieldBonus(false);
		m_cLandingLocation=m_cFinishLocation;
		AddWarning(eFinish,0,_T("Rolling Finish with a 2 minute time penalty."));
		}


	// And the final step is to compute the distance.
	m_dDistance=ComputeDistance(pcTask, turnpointArray);


	// Check to see if the constructive landout was too short
	double dSMTD = cClass.m_dSMTD;
	double dCheckDistance=m_dDistance;
	if( cClass.IsHandicapped() ) 
		{
		dCheckDistance=dCheckDistance*pcContestant->m_fHandicap;
		}

//--Pre 2022 check
//	if( !pcTask->IsFAITask() &&
//		m_nAcheivedTurnpoints<=1 && 
//		dCheckDistance>.001 &&
//		dCheckDistance<dSMTD/2 )
//--

	if( !pcTask->IsFAITask() &&
		m_nAcheivedTurnpoints==0 && 
		dCheckDistance<dSMTD/2 )
		{

		double dDist=m_pcHomePoint->DistanceTo(m_cLandingLocation,eStatute);
		if( dDist<AIRBONTOLER )
			{
			CString strWarn;
			if( cClass.IsHandicapped() ) 
				strWarn.Format(_T("Constructive landout distance %5.2lf (%5.2lf Handicapped) is less than SMTD/2. Distance set to 0.0"),ConvertDistance(m_dDistance,SYSTEMUNITS,eStatute),ConvertDistance(m_dDistance,SYSTEMUNITS,eStatute)*pcContestant->m_fHandicap);
			else
				strWarn.Format(_T("Constructive landout distance %5.2lf is less than SMTD/2. Distance set to 0.0"),ConvertDistance(m_dDistance,SYSTEMUNITS,eStatute));
			AddWarning(eInformation,0,strWarn);
			m_dDistance=0.0;
			}
		else if( MotorRunLandoutWarning() )
			{
			AddWarning(eInformation,0,_T("Constructive (motorized) landout distance is less than SMTD/2 . Distance set to 0.0"));
			m_dDistance=0.0;
			}
		}

	if( !CheckOption(FLT_AIRFIELDBONUSLOCKED) && cClass.IsAirfieldBonusActive() )
		{
		AirfieldBonus(false);
		if( MotorRunLandoutWarning() )
			{
			// Landout due to motor run, check 2mi and 1000ft
			TURNPOINTCLASS *pcLandout=turnpointArray.FindClosestAirport(&m_cLandingLocation, ConvertDistance(1.0, eStatute, SYSTEMUNITS) );
			if( pcLandout )
				{
				int iAltitudeAbove=0;
				// Find motor on altitude
				int iLandingPoint=FindEvent( FAN_LANDED, 0, FORWARD );
				CPosition *pcPos=GetPosition(iLandingPoint);
				if( pcPos )
					{
					iAltitudeAbove=pcPos->m_iCorrectedAltitude-pcLandout->m_iElevation;
					if( iAltitudeAbove>1000 )
						{
						m_iLandoutAirfield=pcLandout->m_iID;
						m_strLandoutAirfield=pcLandout->m_strName;
						if( !IsTaskComplete() ) 
							{
							AirfieldBonus(true);
							CString strMess;
							strMess.Format("Airfield bonus applied due to motor run %ift above and within 1 mile of %i - %s",iAltitudeAbove,pcLandout->m_iID,pcLandout->m_strName);
							AddWarning(eInformation,0,strMess );
							}
						}
					}
				}
			}
		else
			{
		// See if he landed close to a turnpoint
			TURNPOINTCLASS *pcLandout=turnpointArray.FindClosestAirport(&m_cLandingLocation, ConvertDistance(AIRBONTOLER, eStatute, SYSTEMUNITS) );
			if( pcLandout )
				{
				m_iLandoutAirfield=pcLandout->m_iID;
				m_strLandoutAirfield=pcLandout->m_strName;
		//		m_cLandingLocation=*pcLandout;
				if( !IsTaskComplete() )
					{ 
					if( m_dDistance>0.0 ) AirfieldBonus(true);
					}	
				}
			}
		}


	// Make sure the last point is far enough away from the finish.

	if( m_eTaskType==eModifiedAssigned &&
		m_bValidFinish  )
		{
		double dDist=m_pcFinishControlPoint->DistanceTo(m_acTurnAreaTurnpointLocations[m_nTaskPoints-1], eStatute);
		if(m_cFinishGate.IsGPSCylinder() )
			dDist-=ConvertDistance(m_cFinishGate.GetRadius(),SYSTEMUNITS, eStatute);

		if( dDist<2 )
			AddWarning(eInformation,0,_T("Final leg of MAT is less than 2 miles."));
		
		}

	try {
		CheckSafetyFinish();
		} 
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::CheckSafetyFinish"));
		throw;
		}

	try {
		CheckSecurity();
		} 
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in CFlight::CheckSecurity"));
		throw;
		}

	return true;
	}

void	CFlight::AssignPositionStatus(TASKCLASS* pcTask, bool bAutoTask, TURNPOINTCLASSARRAY &turnpointArray)
	{
	bool    bRolled			=false;
	bool    bNoAltitudeInfo	=false;
	bool	bAltWarn		=false;
	int		nPoints			=GetNumPoints();
	int		iHomeElevation	=0;
	double	dSpeed			=0.0;
	double  dAveSpeed	    =0.0;
	double  dSlope			=0.0;
	double  dMaxDistance	=0.0;
	int iMinAltitude		=32000;
	int iMaxAltitude		=0;
	int iPoint				=0;
	int iAveAlt				=0;
	int nAveAlt				=0;
	CPosition*	pcPrevPos	=NULL;
	CPosition*	pcPos		=NULL;
	CArray<double,double>	dSpeedArray;
	CAverager cAveAltDiff(5);
	CMedian	  cMedian;

	iMSLCorrection	=0;
	iMSLFinishCorrection=0;

	bool bPressureZero=true;
	for( int iPoint=0; iPoint<nPoints; iPoint++ )
		{
		pcPos=GetPosition(iPoint);
		if( pcPos==NULL) continue;
		if (pcPos->m_iPressureAltitude>0 ) bPressureZero=false;
		}
	if( bPressureZero )
		{
		CString strPenalty=_T("Pressure altitude is zero for all fixes in the log, using GPS altitude instead.");
		AddWarning(eInformation,0,strPenalty);
		for( int iPoint=0; iPoint<nPoints; iPoint++ )
			{
			pcPos=GetPosition(iPoint);
			if( pcPos==NULL) continue;
			pcPos->m_iPressureAltitude=pcPos->m_iGPSAltitude;
			}
		}	

	CalculateFlightAverages(	iMinAltitude, 
								iMaxAltitude, 
								bNoAltitudeInfo,
								dSpeedArray);

//
//
//  1st Pass, Find and flag the rolling/flying and the landed status 
//
//

	for( int iPoint=0; iPoint<nPoints; iPoint++ )
		{
		pcPrevPos=pcPos;
		pcPos=GetPosition(iPoint);
		if( pcPos==NULL) break;

		// Check change in altitude and speed over the last few points
		// to see if landing occured
		dAveSpeed=GetAveSpeed(dSpeedArray, iPoint,5);
		pcPos->m_dSpeed=dAveSpeed;

		int iGrossAltChange=0;
		if( pcPrevPos )
			{
			cAveAltDiff.SetValue(pcPos->m_iPressureAltitude-pcPrevPos->m_iPressureAltitude);
			iGrossAltChange = abs((int)cAveAltDiff.Average());
			}

		if( iPoint>0 )
			{
			CTimeSpan cInc=pcPos->m_cTime-pcPrevPos->m_cTime;
			int iSec=(int)cInc.GetTotalSeconds();

			pcPos->m_fVSpeed=(float)cAveAltDiff.Average()/(float)iSec;
			}

		if( CheckOption(FLT_ROLLTIMELOCKED) )
			if( pcPos->m_cTime<m_cRollTime ) continue;

		// Check if rolling
		double dMinSpeed=20;
		if( (dAveSpeed>ConvertDistance(dMinSpeed/2., eStatute, SYSTEMUNITS ) && iGrossAltChange>10) ||
			 dAveSpeed>ConvertDistance(dMinSpeed, eStatute, SYSTEMUNITS)					)
			{
			bRolled=true;
			pcPos->AddStatus( FAN_ROLLING );
			}

		if( !bRolled ) continue;

		if( CheckOption(FLT_LANDINGTIMELOCKED)  )
			{
			if( pcPos->m_cTime>m_cLandingTime )
				{
				pcPos->AddStatus( FAN_LANDED );
				pcPos->RemoveStatus(FAN_ROLLING);
				}
			}
		else
			{
			
			//Check if Landed
			if( bNoAltitudeInfo )
				{
				if(	bRolled && 
					dAveSpeed < ConvertDistance( 5., eStatute, SYSTEMUNITS) )
					{
					pcPos->AddStatus( FAN_LANDED );
					}
				}
			else
				{
				if(	bRolled && 
					dAveSpeed < ConvertDistance(dMinSpeed/2., eStatute, SYSTEMUNITS) && 
					iGrossAltChange<10 )
					{
					//  Eventhough we have dropped below the tolerances, this could be an anomalty.
					//  We need to look ahead in the log to see if he really landed.
					//
					if( TimeRemainingInLog(iPoint).GetTotalMinutes()<=5 ) 
						{
						// Less than 5 minutes remaining in the log, safe to land now.
						pcPos->AddStatus( FAN_LANDED );
						}
					else
						{
						//Tricky, Check the next 5 minutes and verify the speed remains below
						// the threshold.

						double dMax=0;
						for( int itest=iPoint+1; itest<GetNumPoints(); itest++ )
							{
							double dCheckSpeed=GetAveSpeed(dSpeedArray, itest,5);
							if( dCheckSpeed>dMax ) dMax=dCheckSpeed;
							CPosition *pcCheck=GetPosition(itest);
							if (!pcCheck ) break;
							CTimeSpan cSpan(pcCheck->m_cTime-pcPos->m_cTime);
							int iMin=(int)cSpan.GetTotalMinutes();
							if( iMin>5 ) break;					
							}

						 // Speed remained low for 5 minutes, landout now.
						  if( dMax<ConvertDistance(dMinSpeed/2., eStatute, SYSTEMUNITS) )
							{
	  						pcPos->AddStatus( FAN_LANDED );
							}
						}


					}
				}
			}
		
		// Check for Gaps greater than 1 minute (make it 61 seconds in case some bozo has
		// a 1 minute sampling rate.
		if( pcPos && pcPrevPos )
			{
			CTimeSpan cSpan=pcPos->m_cTime-pcPrevPos->m_cTime;
			if( cSpan.GetTotalSeconds()> 61 )
				{
				pcPos->AddStatus(FAN_GAP);
				}
			}

		}// end for each position
//
//
//  Now find the takeoff point, back up 2 minutes samples and use those points 
//  to calibrate the altitude.
//
//
//
	int iRollPos=FindEvent( FAN_ROLLING, 0, FORWARD );
    iRollPos=max(iRollPos,0);
	pcPos=GetPosition(iRollPos);

	int iCheckRoll=0;
	if(pcPos ) 
		iCheckRoll=FindTime(pcPos->m_cTime-CTimeSpan(0,0,2,0), iRollPos, BACKWARD);


	for( int iPoint=max(iCheckRoll,0); iPoint<iRollPos; iPoint++ )
		{
		CPosition*	pcPos=GetPosition(iPoint);
		if(pcPos==NULL) break;

		cMedian.AddSample(pcPos->m_iPressureAltitude);
		}

	iAveAlt=cMedian.Mode();

	pcPos		=GetPosition(0);
	if( pcPos ) 
		{
		if( m_pcHomePoint && m_pcHomePoint->GetElevation()!=0 ) 
			iHomeElevation=m_pcHomePoint->GetElevation();
		else
			iHomeElevation=pcPos->m_iPressureAltitude;
		}

	iMSLCorrection=iHomeElevation-iAveAlt;
	iMSLFinishCorrection=iMSLCorrection;
//
//
//  Now find the landing point, advance up 2 minutes samples and use those points 
//  to calibrate the altitude for finish.
//
//
	int iLanding=FindEvent( FAN_LANDED, 0, FORWARD );
    iLanding=max(iLanding,0);
	pcPos=GetPosition(iLanding);

	if( m_pcHomePoint && pcPos )
		{
		double dDistance=pcPos->DistanceTo( &CLocation(m_pcHomePoint->GetLat(),m_pcHomePoint->GetLong()), eStatute);
		if( dDistance<1 )
			{
			int iCheckRoll=nPoints;
			if(pcPos ) 
				iCheckRoll=FindTime(pcPos->m_cTime+CTimeSpan(0,0,2,0), iLanding, FORWARD);
			if( iCheckRoll<0 ) iCheckRoll=nPoints-1;

			CMedian cFinishMedian;

            if( iLanding==iCheckRoll ) 
                {
                // No post landing fixes, just use the landing fix's altitude
                cFinishMedian.AddSample(pcPos->m_iPressureAltitude);
                }
            else
                {
                for( int iPoint=max(iLanding,0); iPoint<iCheckRoll; iPoint++ )

                        {
                        CPosition*    pcPos=GetPosition(iPoint);
                        if(pcPos==NULL) break;
                        cFinishMedian.AddSample(pcPos->m_iPressureAltitude);
                        }
                }

			iAveAlt=cFinishMedian.Mode();

			iMSLFinishCorrection=iHomeElevation-iAveAlt;
			}	
		}

//
//
//  Now compute and save the distances to the start and finish gates.
//
//
	for( int iPoint=0; iPoint<nPoints; iPoint++ )
		{
		pcPos=GetPosition(iPoint);
		if(pcPos==NULL) break;
	
		if( !m_pcStartControlPoint) break;
		double dDistanceFromStart=pcPos->DistanceTo( &CLocation(m_pcStartControlPoint->GetLat(),m_pcStartControlPoint->GetLong()), SYSTEMUNITS);
		pcPos->SetDistanceFromStart(dDistanceFromStart);

		if( !m_pcFinishControlPoint) break;
		double dDistanceFromFinish=pcPos->DistanceTo( &CLocation(m_pcFinishControlPoint->GetLat(),m_pcFinishControlPoint->GetLong()), SYSTEMUNITS);
		pcPos->SetDistanceFromFinish(dDistanceFromFinish);
		}
//
//
//  Now apply the altitude correction to all position points 
//

	for( int iPoint=0; iPoint<nPoints; iPoint++ )
		{
//		pcPrevPos=pcPos;
		pcPos=GetPosition(iPoint);
		if(pcPos==NULL) break;
		pcPos->m_iCorrectedAltitude=pcPos->m_iPressureAltitude+iMSLCorrection;
		}
//
//
//  Check for start/finisn entry/exit.
//
//
	for( int iPoint=0; iPoint<nPoints; iPoint++ )
		{

		pcPrevPos=pcPos;
		pcPos=GetPosition(iPoint);
		if(pcPos==NULL) break;
		
		// Check position in relation to the start gate/cylinder
		if( m_pcStartControlPoint )
			{
			// Check Altitudes
			if( !bNoAltitudeInfo &&
				pcPos->m_iCorrectedAltitude > m_cStartGate.GetHeight() )
				{
				pcPos->AddStatus( FAN_ABOVE_START_CYL );
				}

			// Check Start Cylinders
			if( pcPos->m_dDistanceFromStart<=m_cStartGate.GetRadius() )
				{
				pcPos->AddStatus(FAN_IN_START_CYL);
				}
			}

		// Check position in relation to the finish gate/cylinder
		if( m_pcFinishControlPoint )
			{

			int iBaseAlt=0;

			if( CheckOption(FLT_SAFETYFINISH) ) 
				{
				iBaseAlt=(int)(ConvertDistance(pcPos->m_dDistanceFromFinish,SYSTEMUNITS,eStatute)*200.);
				}
			else
				{
				iBaseAlt=m_cFinishGate.GetBase();
				}

			int iFinishAlt=max(pcPos->m_iPressureAltitude+iMSLCorrection,pcPos->m_iPressureAltitude+iMSLFinishCorrection);
			if(  m_pcFinishControlPoint && 
				!bNoAltitudeInfo &&
			    (iFinishAlt < iBaseAlt ))
				{
				pcPos->AddStatus( FAN_BELOW_FINISH_CYL );
				}


			// Check Finish Cylinder

			if( CheckOption(FLT_SAFETYFINISH) ) 
				{
				if( pcPos->m_dDistanceFromFinish<=ConvertDistance(5.00,eStatute,SYSTEMUNITS) )
					{
					pcPos->AddStatus(FAN_IN_FINISH_CYL);
					}
				}
			else
				{
				if( pcPos->m_dDistanceFromFinish<=m_cFinishGate.GetRadius() )
					{
					pcPos->AddStatus(FAN_IN_FINISH_CYL);
					}
				}
			}

		if( !bAltWarn && pcPos->m_iCorrectedAltitude>17500 )
			{
			CString cWarn;
			cWarn.Format(_T("%s - Altitude exceeded 17500ft."), pcPos->m_cTime.Format(_T("%H:%M:%S")) );
			AddWarning(eAirspaceMinor,25,cWarn);
			bAltWarn=true;
			}

		
		}// end for each position

//
//
//
//  Now flag which turnpoints were visited and
//  calculate a distance to each task point from the current position
//
//
//
	double dToler=m_dTurnpointOuterRadius-m_dTurnpointRadius;
	if( IsAreaTask() )
		dToler=ConvertDistance(1.00,eStatute,SYSTEMUNITS);

	pcPos		=GetPosition(0);
	int			iLastTaskPoint= FindTime( pcTask->m_cGateOpenTime, 0, FORWARD );;
	if( iLastTaskPoint<0 ) iLastTaskPoint=0;
	
	// IN the weird corner case when he is already at the 1st turn when the gate opens, advance the 
	// Check time to when he is in the cylinder

	if( m_cStartGate.GetGateType()==eCylinder )
		{
		for(int iPoint=iLastTaskPoint; iPoint<nPoints; iPoint++ )
				{
				pcPos=GetPosition(iPoint);
				if( pcPos->m_dDistanceFromStart < m_cStartGate.GetRadius() )
					{
					iLastTaskPoint=iPoint;
					break;
					}
				}
		}


	int			iTPID=0;

	for( int itpt=0; itpt<m_nTaskPoints; itpt++)
		{
		pcPos		=GetPosition(iLastTaskPoint);
		bool		bBreakOnExit=false;
		bool		bRolled=false;
		int			iOuterPoint= -1;
		for( int iPoint=iLastTaskPoint+1; iPoint<nPoints; iPoint++ )
			{
			if( !m_pcTaskTurnpoints[itpt] ) continue;

			pcPrevPos=pcPos;
			pcPos=GetPosition(iPoint);
			if(pcPos==NULL) break;

			if( pcPos->CheckStatus(FAN_ROLLING) ) bRolled=true;
			if( !bRolled ) continue;

			// If we locked the start time, don't check any turnpoints before then.
			if( CheckOption(FLT_STARTTIMELOCKED) &&
				pcPos->m_cTime		<m_cStartTime  ) continue;

			// Ignore what happened before the task opened.
			if( pcPos->m_cTime < pcTask->m_cGateOpenTime   ) continue;

			double dTpDist=pcPos->DistanceTo( &CLocation(m_pcTaskTurnpoints[itpt]->GetLat(),m_pcTaskTurnpoints[itpt]->GetLong()), SYSTEMUNITS);

			pcPos->SetDistanceFromControlPoint(itpt, dTpDist);

			if( dTpDist<=(GetTurnpointRadius(itpt)+dToler) )
				{
				// He is inside the outer radius of the assigned turnpoint
				pcPos->AddStatus(FAN_OUTER_TURNPOINT);
				pcPos->SetTurnpoint( m_pcTaskTurnpoints[itpt]->GetID(), itpt );
				iLastTaskPoint=iPoint;
				iOuterPoint=iPoint;
				if( dTpDist<=GetTurnpointRadius(itpt) )
					{
					pcPos->AddStatus(FAN_AT_TURNPOINT);
					iTPID=m_pcTaskTurnpoints[itpt]->GetID();

					// Arm the break on exit for MATS for the last point.  We don't want to
					// get confused with any re-visits of the last turnpoint.
					if( m_eTaskType==eModifiedAssigned )
						bBreakOnExit=true;

					}
				}
			else
				{
									
				if( bBreakOnExit && 
					// This helps prevent not catching a dive in, exit, then reentry
					dTpDist>GetTurnpointRadius(itpt)+(3*dToler) ) 
					{
					break;
					}

				// If he acheived the outer band of the turnpoint, but then proceeded on to 
				// the next turnpoint, then we are certainly done with this turnpoint.  Mark
				// the last task point and break out.

				// Actually, not certainly at all.  He could enter the outer band of tp1, then drift into the outer band of tp2, then push on
				// into the inner band of tp1, then proceed on. 
				double dDistNext=0;
				if(m_pcTaskTurnpoints[itpt+1]!=NULL)
					{
					dDistNext=pcPos->DistanceTo( &CLocation(m_pcTaskTurnpoints[itpt+1]->GetLat(),m_pcTaskTurnpoints[itpt+1]->GetLong()), SYSTEMUNITS);
					}
				if( iOuterPoint>0 &&
					itpt<m_nTaskPoints-1 &&
                    //GetTurnpointRadius(itpt+1)>dDistNext)           // Check if he hit the inner band of the next tp, 
					GetTurnpointRadius(itpt+1)+dToler>dDistNext)  // not simly the outer band.
					{
					iLastTaskPoint=iOuterPoint;
					break;
					}
				}
			}// end for each taskpoint
		}// end for each position

//
//
//  For MAT, see if he cut the task short
//
//
	int iLastCheckedPoint;
	bool	bShort = false;
	if( m_nTaskPoints>1 && m_eTaskType==eModifiedAssigned && !CheckOption(FLT_TURNPOINTSLOCKED) )
		{
		for( int itpt=m_nTaskPoints-1; itpt>=0; itpt--)
			{
			iLastCheckedPoint=FindAcheivedTurnpoint( FAN_OUTER_TURNPOINT, 
													 m_pcTaskTurnpoints[itpt]->GetID(), 
													 GetNumPoints()-1,
													 BACKWARD );
			if( iLastCheckedPoint<0 ) 
				{
				if( m_nTaskPoints>1 ) 
					{
					m_nTaskPoints--;
					bShort=true;
					}
				}
			else
				{
				if( itpt>1 ) 
					{
				    //He overflew this turnpoint on the way home, but lets be sure he got it in sequence.  Check the for the proceeding tp
					int iTest=FindAcheivedTurnpoint( FAN_AT_TURNPOINT, 
													 m_pcTaskTurnpoints[itpt-1]->GetID(), 
													 iLastCheckedPoint,
													 BACKWARD );
					if( iTest<0 ) 
						{
						m_nTaskPoints--;
					    bShort=true;
						}
					}
				}

			}// end for each taskpoint

		}
//
//
// For MAT, try and automatically figure out if any extra turnpoints were acheived.
//
//

		if( !bShort && m_eTaskType==eModifiedAssigned && iLastTaskPoint>=0 && !CheckOption(FLT_TURNPOINTSLOCKED))
			{
			int			nTaskPoints=m_nTaskPoints;
			pcPos		=GetPosition(iLastTaskPoint);
			
			for(int iPoint=iLastTaskPoint+1; iPoint<nPoints; iPoint++ )
				{
				pcPrevPos=pcPos;
				pcPos=GetPosition(iPoint);
				if(pcPos==NULL) break;
				
				if( nTaskPoints>0 && pcPos->m_dDistanceFromFinish<2 ) 
					{
					CTimeSpan cTimeRemaining;
					GetFlightTimeRemaining( iPoint, cTimeRemaining);
					if( cTimeRemaining.GetTotalMinutes()<4 ) 
						break;
					}

				// don't use any turnpoints inside the start cyl until a start is made.
				if( nTaskPoints==0 && pcPos->m_dDistanceFromStart<m_cStartGate.GetRadius()+ConvertDistance(5,eStatute,SYSTEMUNITS)) 
					{
					continue;
					}

				for( int itpt=0; itpt<turnpointArray.GetSize(); itpt++)
					{	    
					TURNPOINTCLASS* pTURNPOINTCLASS = (TURNPOINTCLASS*)turnpointArray[itpt];
					if( !pTURNPOINTCLASS || !(pTURNPOINTCLASS->IsTurnPoint()) ) continue;

				
					CLocation cLoc(pTURNPOINTCLASS->GetLat(), pTURNPOINTCLASS->GetLong() );
					double dTpDist=pcPos->DistanceTo( &cLoc, SYSTEMUNITS);
					
					// This check is to prevent a turnpoint from being used twice.
					// It gives priority to the 1st one.
					int iID=pTURNPOINTCLASS->GetID();
					if( (nTaskPoints>3 && 
						iID==m_aiTaskTurnpointIDs[nTaskPoints-2])) 
						{
						continue; 
						}
					
					if( dTpDist<=m_dTurnpointRadius+(dToler *.75 ))// give  tolerance for auto turnpoints
						{
						pcPos->SetDistanceFromControlPoint(nTaskPoints-1, dTpDist);
						pcPos->AddStatus(FAN_OUTER_TURNPOINT);
						if( dTpDist<=GetTurnpointRadius(nTaskPoints-1) )
							{
							pcPos->AddStatus(FAN_AT_TURNPOINT);
							}
						pcPos->SetTurnpoint( pTURNPOINTCLASS->GetID(), nTaskPoints-1 );
						if( iTPID!= pTURNPOINTCLASS->GetID() )
							{
							iTPID= pTURNPOINTCLASS->GetID();
							nTaskPoints++;
							SetTaskPoint(pTURNPOINTCLASS);
							}
						break; // we have one, don't check for more.
						}			
					}// end for each turnpoint
				}// end for each point


			//  Now for some sanity checks.

			// Make sure the last point is far enough away from the finish.
			if( m_nTaskPoints>0 &&
				m_nTaskPoints>m_nMATTaskPoints && 
				m_pcTaskTurnpoints[m_nTaskPoints-1] )
				{
				double dDist=m_pcFinishControlPoint->DistanceTo(m_pcTaskTurnpoints[m_nTaskPoints-1], eStatute);
				if(m_cFinishGate.IsGPSCylinder() )
					dDist-=ConvertDistance(m_cFinishGate.GetRadius(),SYSTEMUNITS, eStatute);
				if( dDist<ConvertDistance(2,eStatute,SYSTEMUNITS) )
					{
					m_nTaskPoints--;
					}
				}

			// Cull out unhelpful turnpoints with penaltys!!!

			int ntries=0;
			bool bOneRemoved=true;
			while ( bOneRemoved || (m_nTaskPoints>MAXTASKTPS && ntries<10) )
			{
			ntries++;
			bOneRemoved=false;
			for( int itpt=max(m_nMATTaskPoints-1,0); itpt<m_nTaskPoints; itpt++ )
				{

				int iClosestPoint=FindClosestPointToTurnpoint(itpt,true);
				if( iClosestPoint<0 ) continue;

				// This had a valid fix inside, skip;
				CPosition *pcPos=GetPosition(iClosestPoint);
				if( pcPos && 
					m_pcTaskTurnpoints[itpt] &&
					m_pcTaskTurnpoints[itpt-1] )
					{
					double dDist1=0;
					double dDist2=0;
					if( itpt<m_nTaskPoints-1 )
						{
						if( !m_pcTaskTurnpoints[itpt+1] ) continue;
						//This turnoint caused a penalty, see the distance differential
						dDist1=m_pcTaskTurnpoints[itpt-1]->DistanceTo(m_pcTaskTurnpoints[itpt],SYSTEMUNITS);
						dDist1+=m_pcTaskTurnpoints[itpt]->DistanceTo(m_pcTaskTurnpoints[itpt+1],SYSTEMUNITS);
						dDist2=m_pcTaskTurnpoints[itpt-1]->DistanceTo(m_pcTaskTurnpoints[itpt+1],SYSTEMUNITS);
						}
					else
						{
						dDist1=m_pcTaskTurnpoints[itpt-1]->DistanceTo(m_pcTaskTurnpoints[itpt],SYSTEMUNITS);
						dDist1+=m_pcTaskTurnpoints[itpt]->DistanceTo(m_pcFinishControlPoint,SYSTEMUNITS);
						dDist2=m_pcTaskTurnpoints[itpt-1]->DistanceTo(m_pcFinishControlPoint,SYSTEMUNITS);
						}
					bool bRemoveThis=false;

					// Over the limit of max declared turnpoints, be agressive about removing tpts
					bRemoveThis=ConvertDistance(dDist1-dDist2,SYSTEMUNITS, eStatute )<2 && m_nTaskPoints>MAXTASKTPS;


					// Within the limit, remove only if there would be a penalty.
					if( !bRemoveThis)
						bRemoveThis=ConvertDistance(dDist1-dDist2,SYSTEMUNITS, eStatute )<2 && !pcPos->CheckStatus(FAN_AT_TURNPOINT) ;

					if( bRemoveThis )
						{
						// This turnpoint is giving a penalty and isn't helping, remove it.
						int iRemoveID=m_aiTaskTurnpointIDs[itpt];
						for( int i=itpt; i<m_nTaskPoints-1; i++ )
							{
							m_pcTaskTurnpoints[i]=m_pcTaskTurnpoints[i+1];
							m_aiTaskTurnpointIDs[i]=m_aiTaskTurnpointIDs[i+1];
							}
						m_nTaskPoints--;
						bOneRemoved=true;

						// Now fix the fixes
						for(int iPoint=0; iPoint<GetNumPoints(); iPoint++ )
							{
							CPosition *pcPos=GetPosition(iPoint);
							if(pcPos==NULL) continue;
							if( pcPos->GetTurnpoint(itpt)==iRemoveID )
								{
								pcPos->SetTurnpoint(-1,itpt);
								pcPos->RemoveStatus(FAN_OUTER_TURNPOINT);
								}

							for(int i=itpt; i<m_nTaskPoints; i++ )
								{
								if( pcPos->GetTurnpoint(i+1)==m_aiTaskTurnpointIDs[i] )
									{
									pcPos->SetTurnpoint(m_aiTaskTurnpointIDs[i],i);
									}
								}

							}
						continue;
						}

					}
				}
			}
			m_nTaskPoints=min(m_nTaskPoints,MAXTASKTPS);

			}// endif

// For safety, make sure the last point is marked as landed
	pcPos=GetPosition(GetNumPoints()-1);
	if( pcPos!=NULL ) pcPos->AddStatus( FAN_LANDED );





	}// end method

/*
10.8.4 Flight-recorder-controlled start procedure

  (a) Each task shall include a Start Point and a Start Radius which shall be an integral number of miles not less than 5 nor more
  than 20. The Start Point and Start Radius should be chosen so that pilots are likely to be able to find lift prior to starting and to
  return to the home field if they fail to do so.

  (b) Each task shall include a Maximum Start Height (MSH) above the home field. This height shall not be less than 5000' AGL nor
  more than 10000' AGL.

  (c) The Start Point, Start Radius, and MSH define a three-dimensional Start Cylinder.

  (d) A start occurs each time a sailplane exits the Start Cylinder (either through the side or the top); at least one fix must lie within
  the cylinder. The following shall be determined:

	(1) Start Fix - the latest fix within the Start Cylinder.

    (2) Start Time - the interpolated time the sailplane exited the Start Cylinder.

  (e) A pilot may claim a start when no fix is within the Start Cylinder; such a start incurs a penalty. The following shall be
                  determined:

	  (1) Start Fix - the fix closest to the Start Cylinder.

	  (2) Start Time - the time of the Start Fix.

  (f) For each start, the following shall be determined:

      (1) Control Fix - the fix with the greatest altitude during the 5 minutes preceding 
	      the Start Fix.

      (2) Control Height - the difference (in feet) between the altitude of the Control 
	      Fix and the elevation of the home field.

      (3) Control Interval - the time difference (in minutes) between the Control Fix and 
	      the Start Fix.

      (4) Start Distance - the distance (in miles) from the Start Fix to the Start Point.

  (g) If the Control Height exceeds MSH or the Start Distance exceeds the Start Radius, a 
      penalty will apply. The penalty will be a fixed minimum plus the sum of the following:

		(Start Distance - Start Radius) * 400 (but not less than zero)

        (Control Height - MSH) * (6.0 - Control Interval) / 10 (but not less than zero)
*/

void	CFlight::FindStartsAndFinish(CTask *pcTask)
	{
	int iNeedTurnpoint=0;

	bool	bMadeLastTurnpoint	=false;
	bool	bRolled				=false;

//
//
//  First loop and find all the possible start occurences.
//
	CPosition* pcPos			=GetPosition(0);
	CPosition* pcPrevPos		=pcPos;


	if( CheckOption(FLT_STARTTIMELOCKED) )
		{
		for(int iPoint=1; iPoint<GetNumPoints(); iPoint++ )
			{
			pcPrevPos=pcPos;
			pcPos=GetPosition(iPoint);
			if(pcPos==NULL) break;
			if( pcPrevPos->m_cTime	<=m_cStartTime && m_cStartTime < pcPos->m_cTime   )
				{
				pcPos->AddStatus(FAN_START);
				pcPos->AddStatus( FAN_LATEST_START );
				break;
				}
			}
		}

	for(int iPoint=1; iPoint<GetNumPoints(); iPoint++ )
		{

		pcPrevPos=pcPos;
		pcPos=GetPosition(iPoint);
		if(pcPos==NULL) break;

		//  If he didn't roll, don't worry about any other stats
		if( pcPos->CheckStatus(FAN_ROLLING) ) bRolled=true;
		if( !bRolled ) continue;

		// Ignore anthing before task open time
		if( pcTask->m_cGateOpenTime>pcPos->m_cTime ) continue;


		if( pcTask->IsFAITask() )  // Dont consider above the gate as bad
			{
			// Flag a cross the line start
			if(  m_cStartGate.GetGateType()==eLine				&&
				 pcPos->CheckStatus(		FAN_IN_START_CYL))
				{
				if( m_cStartGate.CrossedLine(*pcPrevPos, *pcPos, false ) )
					{
					pcPos->AddStatus(FAN_START);
					}
				}

			if(  m_cStartGate.GetGateType()==eCylinder          &&
				!pcPos->CheckStatus(		FAN_IN_START_CYL)   && 
				 pcPrevPos->CheckStatus(	FAN_IN_START_CYL)    	)
				{
				pcPos->AddStatus(FAN_START);
				}
			}
		else
			{
			// Flag a cross the line start
			if(  m_cStartGate.GetGateType()==eLine				&&
				 pcPos->CheckStatus(		FAN_IN_START_CYL)		&& 
				!pcPos->CheckStatus(FAN_ABOVE_START_CYL) 	)
				{
				if( m_cStartGate.CrossedLine(*pcPrevPos, *pcPos, false ) )
					{
					pcPos->AddStatus(FAN_START);
					}
				}

			if(  m_cStartGate.GetGateType()==eCylinder          &&
				!pcPos->CheckStatus(		FAN_IN_START_CYL)   && 
				 pcPrevPos->CheckStatus(	FAN_IN_START_CYL)   && 
				!pcPrevPos->CheckStatus(	FAN_ABOVE_START_CYL) 	)
				{
				pcPos->AddStatus(FAN_START);
				}
			}

		// Check for an "out the top" start
		if(  m_cStartGate.GetGateType()==eCylinder		 	 &&
             m_cStartGate.ScoreOutTheTop()	  				 &&
			 pcPos->CheckStatus(		FAN_IN_START_CYL)	 && 
			 pcPrevPos->CheckStatus(	FAN_IN_START_CYL)	 && 
			 pcPos->CheckStatus(		FAN_ABOVE_START_CYL) && 
			!pcPrevPos->CheckStatus(	FAN_ABOVE_START_CYL)	)
			{
			pcPos->AddStatus(FAN_START);
			pcPos->AddStatus(FAN_OUT_TOP_START);
			}

		}// end for each position


//
//  Make sure each turnpoint was made in turn.  We especially need to mark
//  the acheivement of the last turnpoint.
//

	int	iLastTurnPoint = 0;
	int	iLandedOutPoint = 0;

	pcPos			=GetPosition(0);
	pcPrevPos		=pcPos;
	for(int iPoint=1; iPoint<GetNumPoints(); iPoint++ )
		{
		pcPrevPos=pcPos;
		pcPos=GetPosition(iPoint);
		if(pcPos==NULL) break;

		if( bMadeLastTurnpoint && pcPos->CheckStatus(FAN_LANDED) )
				{
				// Landed on the way back from the last tpt
				iLandedOutPoint=iPoint;
				break;
				}

		if(	!bMadeLastTurnpoint						&&	
			 pcPos->CheckStatus( FAN_OUTER_TURNPOINT ) && 
			 m_pcTaskTurnpoints[iNeedTurnpoint] &&
			 pcPos->CheckTurnpoint( iNeedTurnpoint, m_pcTaskTurnpoints[iNeedTurnpoint]->GetID()) )
			{
			pcPos->AddStatus(FAN_AT_TASKPOINT);
			iNeedTurnpoint++;
			
			if( iNeedTurnpoint==m_nTaskPoints ) 
				{
				bMadeLastTurnpoint=true;
				iLastTurnPoint=iPoint;
				}
			}
		}// end for each position

//
//  Starting at the last turnpoint in the task, start searching for finishes.
//  Of course, only look for a finish if the last turnpoint was actually acheived.
//
	if( CheckOption(FLT_FINISHTIMELOCKED) )
		{
		pcPos			=GetPosition(0);
		pcPrevPos		=pcPos;
		for(int iPoint=1; iPoint<GetNumPoints(); iPoint++ )
			{
			pcPrevPos=pcPos;
			pcPos=GetPosition(iPoint);
			if(pcPos==NULL) break;
			if( pcPrevPos->m_cTime	<m_cFinishTime &&
				pcPos->m_cTime		>m_cFinishTime  )
				{
				pcPos->AddStatus(FAN_FINISH);
				break;
				}
			}
		}
	else
		{

		if( bMadeLastTurnpoint )
			{
			pcPos			=GetPosition(iLastTurnPoint);
			pcPrevPos		=pcPos;
			CPosition*		pHighestFix=NULL;

			for(int iPoint=iLastTurnPoint; iPoint<GetNumPoints(); iPoint++ )
				{
				pcPrevPos=pcPos;
				pcPos=GetPosition(iPoint);
				if(pcPos==NULL) break;

				// Flag Finish across a line
				if(  m_cFinishGate.GetGateType()==eLine		 &&
					 pcPos->CheckStatus(FAN_IN_FINISH_CYL)   	 )
					{
					if( m_cFinishGate.CrossedLine(*pcPrevPos, *pcPos, true ) )
						{
						if( m_cFinishGate.CrossingTime( *pcPrevPos, *pcPos, m_cFinishTime ) )
							{
							m_bValidFinish=true;
							pcPos->AddStatus(FAN_FINISH);

							int iPressureAlt=m_cFinishGate.CrossingAltitude( *pcPrevPos, *pcPos  );
							int iFinishGroundElevation=GetHomePoint()->GetElevation();
							double dAltitudeS=(iPressureAlt+iMSLCorrection)-iFinishGroundElevation;
							double dAltitudeF=(iPressureAlt+iMSLFinishCorrection)-iFinishGroundElevation;
							double dHeightAboveGround=max(dAltitudeF,dAltitudeS);
							if(dHeightAboveGround<50 && pcPos->m_dSpeed >20. && !IsFAITask()  ) // If < 20 knots he is landing/rolling, don't warn.
								{
								CString strPenalty;
								int iGateBase=iFinishGroundElevation+50;
								int iHeight=int(dHeightAboveGround);
								strPenalty.Format(_T("Finish Altitude %i AGL is less than the Finish Gate Base at 50ft AGL (%i MSL)."), iHeight, iGateBase );
								AddWarning(eFinish,0,strPenalty);
								}
							break;
							}
						}
					}

				// Flag entry into the finish from the side
				if(  m_cFinishGate.GetGateType()==eCylinder			 &&
					 pcPos->CheckStatus(		FAN_IN_FINISH_CYL)   && 
					 !pcPrevPos->CheckStatus(	FAN_IN_FINISH_CYL)  /* && 
					 !pcPos->CheckStatus(		FAN_BELOW_FINISH_CYL)*/	 )
					{
					m_bValidFinish=true;
					m_cFinishTime=InterpolateFinishPoints( pcPos, pcPrevPos );
					pcPos->AddStatus(FAN_FINISH);

					if( pcPos->CheckStatus(	FAN_BELOW_FINISH_CYL) )
					{ 
						int iPressureAlt=InterpolateFinishPressureAlt( pcPos, pcPrevPos );
						int iFinishGateBaseAlt=m_cFinishGate.GetBase();
						double dDelta1=iFinishGateBaseAlt-(iPressureAlt+iMSLCorrection);
						double dDelta2=iFinishGateBaseAlt-(iPressureAlt+iMSLFinishCorrection);
						double dDelta=min(dDelta1,dDelta2);
						if( dDelta>0.0 )// && dDelta<=400.0) 
							{
// 12.1.4.5 Finish penalty (Rule 10.9.3.4): penalty = 5 + (Finish Height Difference) / 5.
							double dpenalty=0.0;
//							if( dDelta<200 )
//								dpenalty = dDelta / 4.;
//							else
//								dpenalty = 50. + (dDelta-200) * 1.75;
                            dpenalty = .3 * dDelta;

							CString strPenalty;
							int iCrossingAlt=iFinishGateBaseAlt-int(dDelta);
							strPenalty.Format(_T("Finish Altitude of %d ft MSL is less than the Minimum Finish Altitude of %d ft MSL."), iCrossingAlt,m_cFinishGate.GetBase() );
							AddWarning(eFinish,(int)Roundoff(dpenalty),strPenalty);
							}
//						else if(dDelta>400.0) 
//							{
//							m_bValidFinish=false;
//							m_cLandingTime=m_cFinishTime;
//							m_cLandingLocation=*pcPos;
//							SetStatus(eBelowFinishCylinder);
//							}
						}
					break;
					}
				if( iLandedOutPoint>0 && iLandedOutPoint==iPoint ) break;
				}
			}
		}
//
//  Find the latest valid start
//  Work backward through all the acheived turnpoints, then
//  back to the start
//
	if( CheckOption(FLT_STARTTIMELOCKED) )
		{
//		Still need to find the start fix
		FindStartFix();
		m_bValidStart=true;
		}
	else
		{
		int iLastCheckedPoint=GetNumPoints()-1;
		for( int itpt=m_nTaskPoints-1; itpt>=0; itpt--)
			{
			if( !m_pcTaskTurnpoints[itpt] ) continue;
			iLastCheckedPoint=FindAcheivedTurnpoint( FAN_OUTER_TURNPOINT, 
													 m_pcTaskTurnpoints[itpt]->GetID(), 
													 iLastCheckedPoint,
													 BACKWARD );
			if( iLastCheckedPoint<0 ) 
				{
				// couldn't find last point, try again from the end
				iLastCheckedPoint=GetNumPoints()-1;
				}

			}// end for each taskpoint


		// Note, if no turnpoints were acheived, no problem, the last point
		// will still be set at the end of the flight, so we will search from
		// there
		int iStartPoint=FindEvent( FAN_START, 
						  		   iLastCheckedPoint,
								   BACKWARD );
		if (iStartPoint<0 ) 
			{
			// But first final sanity check, start at task open time and advance to the first start along

			int iTOPoint = FindTime( pcTask->m_cGateOpenTime, 0, FORWARD );

			iStartPoint=FindEvent(	FAN_START, 
									  			iTOPoint,
												FORWARD );
			}
		if( iStartPoint>0 )
			{
			pcPos	 =GetPosition(iStartPoint);
			pcPrevPos=GetPosition(iStartPoint-1);

			if( m_cStartGate.GetGateType()==eLine )
				{
				if( !m_cStartGate.CrossingTime(*pcPos, *pcPrevPos, m_cStartTime ) )
					{
					// An error occured, used the start time of the prev pos
					m_cStartTime=pcPos->m_cTime;
					ASSERT(0);
					}
				}
			else
				{
				if( pcPos->CheckStatus(	FAN_IN_START_CYL) &&
					pcPrevPos->CheckStatus(	FAN_IN_START_CYL) )
					{
					m_cStartTime=InterpolateStartAltitude(pcPos, pcPrevPos, m_cStartFix );
					}
				else
					{
					m_cStartTime=InterpolateStartPoints(pcPos, pcPrevPos, m_cStartFix );
					}
				}
			pcPos->AddStatus( FAN_LATEST_START );
			m_bValidStart=true;
			if(pcPos->m_cTime<m_cTaskOpenTime)
				AddWarning(eStart,0,_T("Last valid start occurs before the task open time."));

			}
		else
			{
			// Oops, no start, got to find the closest poin

			if( m_cStartGate.GetGateType()==eCylinder )
				{
				AddWarning(eStart,0,_T("No valid fixes inside the start cylinder, used closest point."));

				double dClosest=1.e+30;
				CPosition* pcStartPos=NULL;
				for( int i=iLastCheckedPoint; i>=0; i-- )
					{
					CPosition* pcPos=GetPosition(i);
					if(pcPos->m_cTime<=m_cTaskOpenTime) break;
					if( pcPos->CheckStatus(FAN_ROLLING) && pcPos->m_dDistanceFromStart<dClosest )
						{
						pcStartPos=pcPos;
						dClosest=pcPos->m_dDistanceFromStart;
						}
					}
				if( pcStartPos ) 
					{
					m_cStartTime=pcStartPos->m_cTime;
					m_bValidStart=true;
					pcStartPos->AddStatus( FAN_LATEST_START );
					if(pcPos->m_cTime<=m_cTaskOpenTime)
						AddWarning(eStart,0,_T("Last valid start occurs before the task open time."));
					}
				}
			else
				{
				AddWarning(eStart,0,_T("No valid start line crossing, used roll time."));
				int iRollPos=FindEvent( FAN_ROLLING, 0, FORWARD );
				CPosition* pcStartPos=GetPosition(iRollPos);
				if( pcStartPos ) 
					{
					m_cStartTime=pcStartPos->m_cTime;
					m_bValidStart=true;
					pcStartPos->AddStatus( FAN_LATEST_START );
					}
				}
			}
		}


//
// Now find the roll time.  Find a start time, then scan backward.
//
//
	if( !CheckOption(FLT_ROLLTIMELOCKED) )
		{

		int iStartPos=FindEvent(FAN_START, 0, FORWARD );
		int iRollPos=FindEvent( FAN_ROLLING, iStartPos, BACKWARD, FALSE );
		if( iRollPos>=0 )
			{
			pcPos		=GetPosition(iRollPos);
			if( pcPos ) m_cRollTime	=pcPos->m_cTime;
			}
		else
			// do it the safe way
			{
			iRollPos=FindEvent(FAN_ROLLING, 0, FORWARD );
			if( iRollPos<0 ) iRollPos=0;
			pcPos		=GetPosition(iRollPos);
			if( pcPos ) m_cRollTime	=pcPos->m_cTime;
			}
		}

	}// end method



/////////////////////  Query results of Analysis ///////////////////


CTime CFlight::LastValidStartTime( )
	{
	return m_cStartTime;
	}

int CFlight::NumberOfWarnings( )
	{
	return m_cWarningArray.GetCount();
	}

int CFlight::NumberOfActiveWarnings( )
	{
	if( m_cWarningArray.GetCount()==0 ) return 0;
	int iCount=0;
	for( int i=0; i<m_cWarningArray.GetCount(); i++ )
		{
		if( m_cWarningArray[i]->IsActive() ) iCount++;
		}
	return iCount;
	}

CWarning* CFlight::GetWarning(int iWarn )
	{
	if( m_cWarningArray.GetCount()==0 ) return NULL;
	return m_cWarningArray[iWarn];
	}

int CFlight::GetWarning(EWarningType eWarnType )
	{
	if( m_cWarningArray.GetCount()==0 ) return -1;
	int iCount=0;
	for( int i=0; i<m_cWarningArray.GetCount(); i++ )
		{
		if( m_cWarningArray[i]->IsActive() && m_cWarningArray[i]->GetType()==eWarnType ) return i;
		}
	return -1;
	}

CLocation& CFlight::GetLandingLocation()
	{
	return m_cLandingLocation;
	}


bool CFlight::IsTaskComplete()
	{
	return (m_bValidStart && m_bAllTurnpointsAcheived && m_bValidFinish);
	}


bool CFlight::AllTurnpointsAcheived()
	{
	return m_bAllTurnpointsAcheived;
	}



int CFlight::FindAcheivedTurnpoint( int iEvent, int iTpID, int iStart, int iDirection, int iTPPos )
	{

	if( iDirection==FORWARD )
		{
		for( int iPoint=iStart; iPoint<GetNumPoints(); iPoint++ )
				{
				CPosition* pcPos=GetPosition(iPoint);
				if(pcPos==NULL) break;

				if( iTPPos>=0 )
					{
					if( pcPos->CheckStatus(	iEvent ) && 
						pcPos->CheckTurnpoint( iTPPos, iTpID)    )
						{
						return iPoint;
						}
					}
				else 
					{
					if( pcPos->CheckStatus(	iEvent ) && 
						 pcPos->CheckTurnpoint( iTpID)    )
						{
						return iPoint;
						}
					}

				}// end for each taskpoint
		}
	else
		{
		for( int iPoint=iStart; iPoint>0; iPoint-- )
				{
				CPosition* pcPos=GetPosition(iPoint);
				if(pcPos==NULL) break;

				if( pcPos->CheckStatus(	iEvent ) && 
					pcPos->CheckTurnpoint( iTpID)  )
					{
					return iPoint;
					}

				}// end for each taskpoint
		}

	return -1;
	}

CTime CFlight::InterpolateStartPoints(CPosition *pPos, CPosition *pPrevPos, CLocation &cStartFix )
	{
	CTimeSpan cIncrement=pPos->m_cTime-pPrevPos->m_cTime;
	int iIncrementSec=(int)cIncrement.GetTotalSeconds();
	double dDist=pPos->m_dDistanceFromStart-pPrevPos->m_dDistanceFromStart;
	double dD1=m_cStartGate.GetRadius()-pPrevPos->m_dDistanceFromStart;
	double dPercent=fabs(dD1/dDist);

//  Use the above percent to compute the start fix.
	double dNewLat=pPrevPos->GetLat()+(pPos->GetLat()-pPrevPos->GetLat())*dPercent;
	double dNewLon=pPrevPos->GetLong()+(pPos->GetLong()-pPrevPos->GetLong())*dPercent;
	cStartFix=CLocation(dNewLat,dNewLon);

//  Not compute the start time
	int	   nSeconds=(int)Roundoff((((double)iIncrementSec)*dPercent));
	return pPrevPos->m_cTime+CTimeSpan(nSeconds);
	}

CTime CFlight::InterpolateStartAltitude(CPosition *pPos, CPosition *pPrevPos, CLocation &cStartFix )
	{
	CTimeSpan cIncrement=pPos->m_cTime-pPrevPos->m_cTime;
	int iIncrementSec=(int)cIncrement.GetTotalSeconds();
	double dDist=pPos->m_iCorrectedAltitude-pPrevPos->m_iCorrectedAltitude;
	double dD1=(m_cStartGate.GetHeight());
		   dD1-=pPrevPos->m_iCorrectedAltitude;
	double dPercent=fabs(dD1/dDist);
	dPercent=min(dPercent,1);
	dPercent=max(dPercent,0);

//  Use the above percent to compute the start fix.
	double dNewLat=pPrevPos->GetLat()+(pPos->GetLat()-pPrevPos->GetLat())*dPercent;
	double dNewLon=pPrevPos->GetLong()+(pPos->GetLong()-pPrevPos->GetLong())*dPercent;
	cStartFix=CLocation(dNewLat,dNewLon);

//  Not compute the start time
	int	   nSeconds=(int)Roundoff((((double)iIncrementSec)*dPercent));
	return pPrevPos->m_cTime+CTimeSpan(nSeconds);
	}

CTime CFlight::InterpolateFinishAltitude(CPosition *pPos, CPosition *pPrevPos )
	{
	CTimeSpan cIncrement=pPos->m_cTime-pPrevPos->m_cTime;
	int iIncrementSec=(int)cIncrement.GetTotalSeconds();
	double dDist=pPos->m_iCorrectedAltitude-pPrevPos->m_iCorrectedAltitude;
	double dD1=0.0;
	if( m_pcFinishControlPoint )
		dD1=(m_cFinishGate.GetBase())-pPrevPos->m_iCorrectedAltitude;
	double dPercent=fabs(dD1/dDist);
	dPercent=min(dPercent,1);
	dPercent=max(dPercent,0);
	int	   nSeconds=(int)Roundoff((((double)iIncrementSec)*dPercent));
	return pPrevPos->m_cTime+CTimeSpan(nSeconds);
	}

CTime CFlight::InterpolateFinishPoints(CPosition *pPos, CPosition *pPrevPos )
	{
	CTimeSpan cIncrement=pPos->m_cTime-pPrevPos->m_cTime;
	int iIncrementSec=(int)cIncrement.GetTotalSeconds();
	double dDist=pPrevPos->m_dDistanceFromFinish-pPos->m_dDistanceFromFinish;
	double dD1=pPrevPos->m_dDistanceFromFinish-m_cFinishGate.GetRadius();
	double dPercent=fabs(dD1/dDist);
	int	   nSeconds=(int)Roundoff((((double)iIncrementSec)*dPercent));
	return pPrevPos->m_cTime+CTimeSpan(nSeconds);
	}

int CFlight::InterpolateFinishPressureAlt(CPosition *pPos, CPosition *pPrevPos )
	{
	int iIncrementFeet=pPos->m_iPressureAltitude-pPrevPos->m_iPressureAltitude;
	double dDist=pPrevPos->m_dDistanceFromFinish-pPos->m_dDistanceFromFinish;
	double dD1=pPrevPos->m_dDistanceFromFinish-m_cFinishGate.GetRadius();
	double dPercent=fabs(dD1/dDist);
	int	   nFeet=(int)Roundoff((((double)iIncrementFeet)*dPercent));
	return pPrevPos->m_iPressureAltitude+nFeet;
	}


int CFlight::FindEvent(int iEvent, int iStart, int iDirection, BOOL bSet )
	{
	if( iDirection==FORWARD )
		{
		for( int iPoint=iStart; iPoint<GetNumPoints(); iPoint++ )
				{
				CPosition* pcPos=GetPosition(iPoint);
				if(pcPos==NULL) break;
				if( bSet )
					{
					if( pcPos->CheckStatus(	iEvent ) )
						{
						return iPoint;
						}
					}
				else
					{
					if( !pcPos->CheckStatus( iEvent ) )
						{
						return iPoint;
						}
					}

				}
		}
	else
		{
		for( int iPoint=iStart; iPoint>0; iPoint-- )
				{
				CPosition* pcPos=GetPosition(iPoint);
				if(pcPos==NULL) break;

				if( bSet )
					{
					if( pcPos->CheckStatus(	iEvent ) )
						{
						return iPoint;
						}
					}
				else
					{
					if( !pcPos->CheckStatus( iEvent ) )
						{
						return iPoint;
						}
					}

				}
		}

	return -1;
	}


int CFlight::FindTime(CTime cTime, int iStart, int iDirection)
	{
	if( iDirection==FORWARD )
		{
		for( int iPoint=iStart; iPoint<GetNumPoints(); iPoint++ )
				{
				CPosition* pcPos=GetPosition(iPoint);
				if(pcPos==NULL) break;

				if( pcPos->m_cTime>cTime )
					{
					return iPoint;
					}

				}
		}
	else
		{
		for( int iPoint=iStart; iPoint>0; iPoint-- )
				{
				CPosition* pcPos=GetPosition(iPoint);
				if(pcPos==NULL) break;

				if( pcPos->m_cTime<cTime )
					{
					return iPoint;
					}

				}
		}

	return -1;
	}


void CFlight::FindLandingLocation()
	{
	// If he was below the finish cylinder, we already determined the landing location & time.
	if( m_eStatus==eBelowFinishCylinder) return;

	int iStartPos=FindEvent(FAN_LATEST_START, 0, FORWARD );
	int iLanding=FindEvent(FAN_LANDED, max(iStartPos,0), FORWARD );
	CPosition* pcPos=GetPosition(iLanding);
	if(pcPos)
		{
		m_cLandingLocation=*pcPos;
		m_cLandingTime=pcPos->m_cTime;
		}
	}

CPosition*  CFlight::FindFurthestFAIProgress(int iStart, CLocation &cLocTo, CLocation &cLocFrom, double dRadius )
	{
	//First find the closest, if it is < radius, just use that.
	if( iStart<0 ) iStart=0;
	double dClosest=1.e+30;
	CPosition *pcClosest=NULL;
	for( int iPoint=iStart; iPoint<GetNumPoints(); iPoint++ )
		{
		CPosition* pcPos=GetPosition(iPoint);
		double dDist=pcPos->DistanceTo(cLocTo, SYSTEMUNITS);
		if( dDist < dClosest )
			{
			dClosest=dDist;
			pcClosest=pcPos;
			}
		if( pcPos->m_cTime> m_cLandingTime) break;
		}
	if( dClosest <= dRadius ) return pcClosest;

//Tricky now, we have to go after a moving target, the closest point on the circle circumference.

	if( iStart<0 ) iStart=0;
	dClosest=(-1.e6);
	pcClosest=NULL;
	int iClosestPosPoint=0;
	double dCorr=GetLatCorrection(cLocTo.GetLat());

	for( int iPoint=iStart; iPoint<GetNumPoints(); iPoint++ )
		{
		CPosition* pcPos=GetPosition(iPoint);

		double dCourseTo=cLocTo.CourseTo(*pcPos);
		CLocation cFAISought=cLocTo;
		double dMoveDir = ((double)(dCourseTo))*TORAD;
		cFAISought.Move( dRadius, dMoveDir, dCorr);
		double dDistToRadius=pcPos->DistanceTo(cFAISought, SYSTEMUNITS );
		double dDistFromFromToRadius=cLocFrom.DistanceTo(cFAISought, SYSTEMUNITS);
		double dTotal=dDistFromFromToRadius-dDistToRadius;
		if( dTotal > dClosest )
			{
			dClosest=dTotal;
			pcClosest=pcPos;
			iClosestPosPoint=iPoint;
			}
		if( pcPos->m_cTime> m_cLandingTime) break;
		}
	return pcClosest;
	}

CPosition* CFlight::FindFurthestProgress(int iStart, CLocation &cLoc  )
	{
	if( iStart<0 ) iStart=0;
	double dClosest=1.e+30;
	CPosition *pcClosest=NULL;
	for( int iPoint=iStart; iPoint<GetNumPoints(); iPoint++ )
		{
		CPosition* pcPos=GetPosition(iPoint);
		double dDist=pcPos->DistanceTo(cLoc, SYSTEMUNITS);
		if( dDist < dClosest )
			{
			dClosest=dDist;
			pcClosest=pcPos;
			}
		if( pcPos->m_cTime> m_cLandingTime) break;
		}
	return pcClosest;
	}


bool CFlight::FindAcheivedTurnpoints(TASKCLASS *pcTask)
	{
	m_bAllTurnpointsAcheived=true;

	int iAcheivedPoints[ALLOCTASKTPS+1];
	for( int i=0; i<ALLOCTASKTPS+1; i++) 
		{
		iAcheivedPoints[i]= -1;
//		m_aiAcheivedTurnpoints[i]=0;
		}


	int iLastCheckedPoint=FindEvent(FAN_LATEST_START, 0, FORWARD);
	int iNextPoint=0;
	if( iLastCheckedPoint<0 ) return false;

	for( int itpt=0; itpt<m_nTaskPoints; itpt++)
		{
		if( m_pcTaskTurnpoints[itpt]==NULL ) break;

		iNextPoint=FindAcheivedTurnpoint( FAN_OUTER_TURNPOINT,
										  m_pcTaskTurnpoints[itpt]->GetID(), 
										  iLastCheckedPoint );

		if( iNextPoint> FindEvent( FAN_LANDED, iLastCheckedPoint, FORWARD) ) 
				{
				m_bAllTurnpointsAcheived=false;
				break;
				}

		if( iNextPoint<0 ) 
			{
			m_bAllTurnpointsAcheived=false;
			break;
			}

		iLastCheckedPoint=iNextPoint;
		iAcheivedPoints[itpt]=iNextPoint;
		m_aiAcheivedTurnpoints[m_nAcheivedTurnpoints++]=m_pcTaskTurnpoints[itpt]->GetID();

		}

	CString strPenalty;

	for(int itpt=0; itpt<m_nAcheivedTurnpoints; itpt++)
		{
		if( m_pcTaskTurnpoints[itpt]==NULL ) break;

		int iClosestPoint=FindClosestPointToTurnpoint(itpt,true);
		if( iClosestPoint<0 ) continue;

		CPosition *pcPos=GetPosition(iClosestPoint);
		if( pcPos && !pcPos->CheckStatus(FAN_AT_TURNPOINT) )
			{
			double dMiles=pcPos->GetDistanceFromControlPoint(itpt);
			double dMiss=dMiles-GetTurnpointRadius(itpt);
			double dMissMiles=ConvertDistance(dMiss,SYSTEMUNITS, eStatute);
			int iPenalty=(int)Roundoff(dMissMiles*100);
			iPenalty=max(iPenalty,0);
			iPenalty=min(iPenalty,125);
			if( iPenalty>0 )
				{
				strPenalty.Format(_T("Closest fix was %6.2lf %s from turnpoint: %s."), ConvertDistance(dMiles,SYSTEMUNITS, eStatute), UnitsText(m_eUnits), m_pcTaskTurnpoints[itpt]->GetNoNameText());
				AddWarning(eTurnpoint,iPenalty,strPenalty);
				}
			}
		else if( pcPos==NULL )
			{
			strPenalty.Format(_T("No valid fix within %6.2lf %s of turnpoint: %s."),ConvertDistance(m_dTurnpointRadius, SYSTEMUNITS, m_eUnits), UnitsText(m_eUnits), m_pcTaskTurnpoints[itpt]->GetNoNameText());
			AddWarning(eTurnpoint,0,strPenalty);
			}

		}


	return true;

/*
	m_bAllTurnpointsAcheived=false;

	int iLastCheckedPoint=FindEvent(FAN_LATEST_START, 0, FORWARD);
	int iNextPoint=0;

	for( int itpt=0; itpt<m_nTaskPoints; itpt++)
		{

		iNextPoint=FindAcheivedTurnpoint( FAN_AT_TURNPOINT,
										  m_pcTaskTurnpoints[itpt]->GetID(), 
										  iLastCheckedPoint );

		if( iNextPoint> FindEvent( FAN_LANDED, iLastCheckedPoint, FORWARD) ) 
			return false;

		if( iNextPoint<0 ) 
			{
			iNextPoint=FindAcheivedTurnpoint( FAN_OUTER_TURNPOINT,
											  m_pcTaskTurnpoints[itpt]->GetID(), 
											  iLastCheckedPoint );

			if( iNextPoint> FindEvent( FAN_LANDED, iLastCheckedPoint, FORWARD) ) 
				return false;

			if( iNextPoint<0 ) 	return false;

			iLastCheckedPoint=iNextPoint;
			CString strPenalty;


			int iClosestPoint=FindClosestPointToTurnpoint(itpt,true);
			CPosition *pcPos=GetPosition(iClosestPoint);
			if( pcPos )
				{
				double dMiles=pcPos->GetDistanceFromControlPoint(itpt);
				double dMiss=dMiles-GetTurnpointRadius(itpt);
				double dMissMiles=ConvertDistance(dMiss,SYSTEMUNITS, eStatute);
				int iPenalty=25+(int)Roundoff(dMissMiles*100);
				iPenalty=max(iPenalty,0);
				iPenalty=min(iPenalty,125);
				if( iPenalty>0 )
					{
					strPenalty.Format("Closest fix was %6.2lf %s from turnpoint: %s.  Recommend penalty of %d points", ConvertDistance(dMiles,SYSTEMUNITS, eStatute), UnitsText(m_eUnits), m_pcTaskTurnpoints[itpt]->GetNoNameText(), iPenalty);
					m_strWarnings.Add(strPenalty);
					}
				}
			else
				{
				strPenalty.Format("No valid fix within %6.2lf %s of turnpoint: %s.",ConvertDistance(m_dTurnpointRadius, SYSTEMUNITS, m_eUnits), UnitsText(m_eUnits), m_pcTaskTurnpoints[itpt]->GetNoNameText());
				m_strWarnings.Add(strPenalty);
				}

			}
		else
			iLastCheckedPoint=iNextPoint;

		m_aiAcheivedTurnpoints[m_nAcheivedTurnpoints++]=m_pcTaskTurnpoints[itpt]->GetID();

		}
	m_bAllTurnpointsAcheived=true;
	return true;
*/
	}


int CFlight::FindPointInStartCyl(int iStart, double dRadius )
	{
	double dClosest=1.e+30;
	CPosition *pcClosest=NULL;
	for( int iPoint=iStart; iPoint>=0; iPoint-- )
		{
		CPosition* pcPos=GetPosition(iPoint);
		if( pcPos->m_dDistanceFromStart <= dRadius )
			{
			return iPoint;
			}
		}
	return -1;
	}


int CFlight::FindClosestPointToTurnpoint( int iTurnpoint, bool bCheckOnlyAtTurnpoint, int iMaxCheck)
	{
	double dClosest=1.e+30;
	int iClosest= -1;
	for( int iPoint=0; iPoint<GetNumPoints(); iPoint++ )
		{
		CPosition* pcPos=GetPosition(iPoint);

		if( pcPos && 
			bCheckOnlyAtTurnpoint && 
			!pcPos->CheckStatus(FAN_OUTER_TURNPOINT) ) continue;

		if( !m_pcTaskTurnpoints[iTurnpoint] ) continue;

		if( pcPos->GetTurnpoint(iTurnpoint)!=m_pcTaskTurnpoints[iTurnpoint]->GetID() ) continue;

		double dCheckDist=pcPos->DistanceTo(&CLocation(m_pcTaskTurnpoints[iTurnpoint]->GetLat(),m_pcTaskTurnpoints[iTurnpoint]->GetLong()),SYSTEMUNITS);
		if( dCheckDist < dClosest )
			{
			dClosest = dCheckDist;
			iClosest=iPoint;
			}
		if( iMaxCheck>0 && iPoint>iMaxCheck ) break;
		}
	return iClosest;
	}

int CFlight::FindClosestPointToStart(int iStart  )
	{
	double dClosest=1.e+30;
	int iClosest= -1;
	for( int iPoint=iStart; iPoint>=0; iPoint-- )
		{
		CPosition* pcPos=GetPosition(iPoint);
		if( pcPos->m_dDistanceFromStart < dClosest )
			{
			dClosest=pcPos->m_dDistanceFromStart;
			iClosest=iPoint;
			}
		}
	return iClosest;
	}

void CFlight::CheckFAIStartAltitudes(TASKCLASS* pcTask)
	{
/*
A pre-start altitude (MSL) limit may be imposed and shall be specified at the
briefing. After the start gate is opened and before making a valid start, the
pilot must ensure at least one fix below the specified pre-start altitude limit.
Failure to do so will be penalized.*/

	int iLatestStart=FindEvent(FAN_LATEST_START, 0, FORWARD );
	if( iLatestStart<=0 ) return;

	int iMin=INT_MAX;
	for( int iPoint=0; iPoint<iLatestStart; iPoint++ )
		{
		CPosition* pcPos=GetPosition(iPoint);
		if(pcPos->m_cTime<m_cTaskOpenTime) continue;

		iMin=min(iMin,pcPos->m_iCorrectedAltitude);
		// If one, count em, one point is below the start altitude, then no penalty
		if( pcPos->m_iCorrectedAltitude < m_cStartGate.GetHeight() ) 
			{
			return;
			}
		}
	// If we got this far and there are no points below the start height, add a warning.
	int iRecommendedPenalty = (iMin-m_cStartGate.GetHeight())/4;
	CString strWarn;
	strWarn.Format(_T("FAI 7.4.5b requires at least one fix be below start height of %d after task open and before start.  Pilots min altitude was: %d"), m_cStartGate.GetHeight(),iMin);
	AddWarning(eStart,iRecommendedPenalty,strWarn);

	return;
	}

void CFlight::CheckStartPenalty(TASKCLASS* pcTask)
	{
	if( !m_bValidStart ) return;

	if( IsFAITask() ) 
		{
		// Check the moronic FAI 7.4.5 b rule
		CheckFAIStartAltitudes(pcTask);
		return;
		}



	if( m_cStartGate.GetGateType()==eLine ) return;
	int iPositionControlFix= -1;
	CString strPenalty;

	/*
2003

10.8.5.4 
	A pilot may claim a start when no fix is within the Start Cylinder; such a start 
	incurs a penalty. The following shall be determined:  
    Start Fix - the fix closest to the Start Cylinder. 
    Start Time - the time of the Start Fix. 

   Control Fix - the fix with the greatest altitude during the 2 minutes preceding the Start Fix. 
   Control Height - the difference (in feet) between the altitude of the Control Fix and the elevation of the home field. 
   Control Interval - the time difference (in minutes) between the Control Fix and the Start Fix. 
   Start Distance - the distance (in miles) from the Start Fix to the Start Point. 

10.8.5.6	If the Control Height exceeds MSH or the Start Distance exceeds the Start 
			Radius, a penalty will apply. The penalty will be a fixed minimum plus the 
			sum of the following (neither of which shall be less than zero):  

	Distance penalty = (Start Distance - Start Radius) * 200 
    Height penalty = 25, if (Control Height - MSH) is not greater than 200 ft. 
         otherwise, height penalty = (Control Height - MSH) / 2  
*/


//
// Find "Start Distance " (Minimum Distance to start point) 
//
	int iStartPos=FindEvent( FAN_LATEST_START, 0, FORWARD );
	if( iStartPos<0 ) return;
	double dStartDistance=100.;
	for( int i=iStartPos; i>=0; i-- )
		{
		CPosition *pcPos=GetPosition(i);
		if( pcPos && pcPos->CheckStatus(FAN_ROLLING) )
			dStartDistance=min(pcPos->m_dDistanceFromStart, dStartDistance);
		if( CheckOption(FLT_STARTTIMELOCKED) ) break;  // The scorer locked the start, use it and don't search for any other points.

		if( pcTask->m_cGateOpenTime>pcPos->m_cTime ) break;  // Only look at closest point when gate is open
		}


//
//  Now compute the penalty
//

	m_iStartPenalty=0;
	if( dStartDistance>m_cStartGate.GetRadius() && !CheckOption(FLT_STARTTIMELOCKED) ) 
		{
		double dDistancePenalty	= ConvertDistance(dStartDistance-m_cStartGate.GetRadius(),SYSTEMUNITS, eStatute)*100;
		m_iStartPenalty=(int) Roundoff(dDistancePenalty);
		if( m_iStartPenalty<=0 ) 
			m_iStartPenalty=0;
		else
			{
			strPenalty.Format(_T("Start Distance of %6.2lf miles > Start Radius"), ConvertDistance(dStartDistance, SYSTEMUNITS, eStatute ) );
			AddWarning(eStart,m_iStartPenalty,strPenalty);
			}
		}
		
	int iControlHeight=0;
	int MSH=0;
	int iStartHeightPenalty=GetStartHeightPenalty(iStartPos, iControlHeight, MSH );

	if( iStartHeightPenalty>0 ) 
		{
		// Start penalty, lets see if we can avoid this.

		int			nMaxRet=15;
		CTime		caStartTimes[15];
		CTimeSpan	caTOCs[15];
		int			iaPenalties[15];
		bool		bNewStart=false;
		CPosition	*pPrevStartPos=NULL;
		CTime       cPrevStartTime=GetStartTime();
		pPrevStartPos	 =GetPosition(iStartPos);
		CLocation   cPrevStartFix=m_cStartFix;


		int nStarts=GetCandidateStarts( nMaxRet, caStartTimes, caTOCs, iaPenalties, pcTask);
		int iMin= 10000;
		int iBest=0;
		if( nStarts>0 )
			{
			for( int i=0; i<nStarts; i++ )
				{
				if( iaPenalties[i]<iMin ) 
					{
					iMin=iaPenalties[i];
					iBest=i;
					}
				}
CString strTest;
			if( caStartTimes[iBest]!=cPrevStartTime ) 
				{
				pPrevStartPos->RemoveStatus(FAN_LATEST_START);
				int iNewStartPoint=FindTime(caStartTimes[iBest],iStartPos,BACKWARD);
				CPosition *pNewStartPos		=GetPosition(iNewStartPoint);
				CPosition *pNewNextStartPos =GetPosition(iNewStartPoint+1);
				pNewStartPos->AddStatus(FAN_LATEST_START);

				if( pNewStartPos->CheckStatus(	FAN_IN_START_CYL) &&
					pNewNextStartPos->CheckStatus(	FAN_IN_START_CYL) )
					{
					m_cStartTime=InterpolateStartAltitude(pNewNextStartPos, pNewStartPos, m_cStartFix );
					}
				else
					{
					m_cStartTime=InterpolateStartPoints(pNewNextStartPos, pNewStartPos, m_cStartFix );
					}
				if( !CheckOption(FLT_STARTTIMELOCKED) ) 
					{
					bNewStart=true;
					SetStartTime(caStartTimes[iBest]);
					}
				else
					{
					for( int i=0; i<nStarts; i++ )
						{
						if( caStartTimes[i]==cPrevStartTime ) 
							{
							iBest=i;  //Reset the best so the start penalty below will be correct.
							break;
							}
						}
					}
				}
			}
		else
			{
			iaPenalties[iBest]=iStartHeightPenalty;
			bNewStart=false;
			}
		if( bNewStart && iaPenalties[iBest]==0)
			{
			strPenalty.Format(_T("Latest start time at %s incured a penalty, An earlier start was selected to avoid a start penalty."),cPrevStartTime.Format(_T("%H:%M:%S")));
			AddWarning(eStartLatest,0,strPenalty);
			}
		else
			{
			m_cStartFix=cPrevStartFix;
			SetStartTime(cPrevStartTime);
			iStartHeightPenalty=iaPenalties[iBest];
			m_iStartPenalty+=iStartHeightPenalty;
			strPenalty.Format(_T("Control height of %d is greater than MSH of %d\nNote: A higher score might be obtained by editing the flight and selecting an earlier start."),iControlHeight, MSH);
			AddWarning(eStart,m_iStartPenalty,strPenalty);
			}
		}
	}

int CFlight::GetStartHeightPenalty( int iStartPos, int &iControlHeight, int &MSH )
	{
//Maximum Start Height (MSH)
	MSH=m_cStartGate.GetHeight();


//
// Calculate the "Control Fix" and the "Control Height"
//

	CPosition *pcStartPos=GetPosition(iStartPos);
	int iAltitudeControlFix=FindTime( pcStartPos->m_cTime-m_cAltitudeControlTime, iStartPos, BACKWARD );
	iAltitudeControlFix=max( iAltitudeControlFix, 0 );

	int iControlAltitude=0;
	CPosition* pControlFix=NULL;
	for( int i=iAltitudeControlFix; i<iStartPos; i++ )
		{
		CPosition* pcPos=GetPosition(i);
		if( pcPos->m_iCorrectedAltitude>iControlAltitude )
			{
			iControlAltitude=pcPos->m_iCorrectedAltitude;
			pControlFix=pcPos;
			}
		}
	ASSERT(pControlFix);
	if( pControlFix==NULL ) return 0;
	iControlHeight=pControlFix->m_iCorrectedAltitude;

	//Give a 0 ft tolerance for altitude inaccuracies. (Used to be 15, but no no no!!!!)
	if( (iControlHeight-0)>MSH )
		{
		double dHeightPenalty=max(0,(iControlHeight - MSH ) / 4.);  
		int iPts=(int)(dHeightPenalty);
		return iPts;
		}

	return 0;
	}

int CFlight::GetCandidateStarts( int iMaxReturn, CTime caStartTimes[], CTimeSpan caTOCs[], int iaPenalties[], CTask *pcTask )
	{
	int iControlHeight=0;
	int MSH=0;

	if( iMaxReturn<1 ) return 0;
	if( !IsStartTimeValid() ) return 0;

	int iStartPos=FindEvent( FAN_LATEST_START, 0, FORWARD );
	if( iStartPos<0 ) return 0;

	//Go forward in time to catch any later starts, 30 min should do.
	CTime cBeginTime=GetStartTime()+30*60;
	iStartPos=FindTime(cBeginTime, iStartPos, FORWARD);
	iStartPos=FindEvent( FAN_START, iStartPos, BACKWARD );
	if( iStartPos<0 ) return 0;

	CLocation cOriginalStartFix=m_cStartFix;
	CPosition *pcStartPos=GetPosition(iStartPos);
	CPosition *pcPrevPos=GetPosition(iStartPos-1);
	if( pcPrevPos->CheckStatus(	FAN_IN_START_CYL) &&
		pcStartPos->CheckStatus(	FAN_IN_START_CYL) )
		{
		caStartTimes[0]=InterpolateStartAltitude(pcStartPos, pcPrevPos, m_cStartFix );
		}
	else
		{
		caStartTimes[0]=InterpolateStartPoints(pcStartPos, pcPrevPos, m_cStartFix );
		}

	if( IsFinishTimeValid() ) 
		caTOCs[0]=GetFinishTime()-caStartTimes[0];
	else
		caTOCs[0]=0;
	iaPenalties[0]=GetStartHeightPenalty( iStartPos, iControlHeight,MSH );
	int nRet=1;
	int iPrevious=iStartPos;
	while( nRet<iMaxReturn )
		{
		iPrevious=FindEvent( FAN_START, iPrevious-1, BACKWARD );
		if( iPrevious<=0 ) break;

		CPosition *pcStartPos=GetPosition(iPrevious);
		if( !pcStartPos ) break;

		CPosition *pcPrevPos=GetPosition(iPrevious-1);
		if( !pcPrevPos ) break;

		CTimeSpan cHowFarBack=GetStartTime()-pcStartPos->m_cTime;
		if( cHowFarBack.GetTotalMinutes()>60 ) break;


		if( pcTask )
			{
			CTimeSpan cTimeFromTaskOpen=pcStartPos->m_cTime-pcTask->m_cGateOpenTime;
			if( cTimeFromTaskOpen.GetTotalSeconds()<=0 ) break;
			}

//	    Do the bloody interpolation...

		if( pcPrevPos->CheckStatus(	FAN_IN_START_CYL) &&
			pcStartPos->CheckStatus(	FAN_IN_START_CYL) )
			{
			caStartTimes[nRet]=InterpolateStartAltitude(pcStartPos, pcPrevPos, m_cStartFix );
			}
		else
			{
			caStartTimes[nRet]=InterpolateStartPoints(pcStartPos, pcPrevPos, m_cStartFix );
			}

		if( IsFinishTimeValid() ) 
			caTOCs[nRet]=GetFinishTime()-pcStartPos->m_cTime;
		else
			caTOCs[nRet]=0;

		iaPenalties[nRet]=GetStartHeightPenalty( iPrevious, iControlHeight,MSH );

		nRet++;
		}
	m_cStartFix=cOriginalStartFix; //The interpolation can change the start fix, restore it.
	return nRet;
	}

CTime CFlight::GetStartTime()
	{
	return m_cStartTime;
	}

CTime CFlight::GetFinishTime()
	{
	return m_cFinishTime;
	}

int CFlight::GetNumAcheivedTurnpoints()
	{
	return  m_nAcheivedTurnpoints;
	}

void CFlight::SetNumAcheivedTurnpoints(int i)
	{
	m_nAcheivedTurnpoints=i;
	}

int CFlight::GetAcheivedTurnpoint(int i)
	{
	return m_aiAcheivedTurnpoints[i];
	}

int  CFlight::AddToList(CListCtrl & ListCtrl, BOOL fVisible, int iItem )
	{
	CSingleLock cLock(&m_cSem_AddToList);
	int i=0;
	LV_ITEM lvi;
	memset(&lvi,0,sizeof(LV_ITEM));

	lvi.iSubItem = 0;
	lvi.mask = LVIF_PARAM;

	lvi.lParam = (LPARAM)this;
	if( iItem<0 ) 
		iItem=ListCtrl.InsertItem(&lvi);
	else
		ListCtrl.SetItemData( iItem, (LPARAM)this );
	ASSERT(iItem>=0);
	ListCtrl.SetItemText(iItem,i++,m_strCID );
	ListCtrl.SetItemText(iItem,i++,m_strPilot );
	ListCtrl.SetItemText(iItem,i++, GetStatusText() );
	ListCtrl.SetItemText(iItem,i++, GetDistanceText(true) );

	ListCtrl.SetItemText(iItem,i++, GetNumWarningsText() );

	ListCtrl.SetItemText(iItem,i++,GetRollTimeText(true));
	ListCtrl.SetItemText(iItem,i++,GetStartTimeText(true));
	ListCtrl.SetItemText(iItem,i++,GetFinishTimeText(true));
	CString strLTime=GetLandingTimeText(true);
	if( MotorRunLandoutWarning() ) strLTime+=" M";
	ListCtrl.SetItemText(iItem,i++,strLTime);
	ListCtrl.SetItemText(iItem,i++,GetStartGateText() );
	ListCtrl.SetItemText(iItem,i++,AcheivedTurnpointText(true,0,true) );

	ListCtrl.SetItemText(iItem,i++,GetLandingLocationText(true) );
	ListCtrl.SetItemText(iItem,i++,GetAirfieldBonusText(true) );
	ListCtrl.SetItemText(iItem,i++,GetScoredLandingPointText(true) );
	ListCtrl.SetItemText(iItem,i++,GetIGCFileNameText() );

	if( SecurityPassed() ) 
		ListCtrl.SetItemText(iItem,i++,_T("Passed") );
	else if( SecurityFailed() ) 
		ListCtrl.SetItemText(iItem,i++,_T("FAILED") );
	else
		ListCtrl.SetItemText(iItem,i++,_T("") );

	if( fVisible) ListCtrl.EnsureVisible( iItem, FALSE);

	return iItem;
	}

CString CFlight::GetStatusText()
{
	CString strOut;
	if( m_bLocked ) 
		{
		strOut=_T("**");
		}
	else
		{
		if( CheckOption(FLT_CHECKED ) ) strOut=_T(" <ok>");
		}

	if( m_eStatus==eNotAnalyzed )
		return _T("Not Analyzed")+strOut;
	else if( m_eStatus==eNoTaskSpecified )
		return _T("No Task Defined")+strOut;
	else if( m_eStatus==eDidNotStart )
		return _T("Did Not Start")+strOut;
	else if( m_eStatus==eIncompleteTask )
		return _T("Incomplete")+strOut;
	else if( m_eStatus==eCompletedTask )
		return _T("Completed Task")+strOut;
	else if( m_eStatus==eNotEnteredInContest )
		return _T("Not a Contestant")+strOut;
	else if( m_eStatus==eNoData )
		return _T("No Data in Flight Log, ")+strOut;
	else if( m_eStatus==eBelowFinishCylinder )
		return _T("Incomplete - Below Finish Height")+strOut;
	return _T("Not Analyzed");

}

CString CFlight::AcheivedTurnpointText( bool bUseMemberPoints, int nPoints, bool bAddDot )
{
	int nDisplay=0;
	if( bUseMemberPoints )
		nDisplay=m_nAcheivedTurnpoints;
	else
		nDisplay=nPoints;

	if( nDisplay==0 ) return "";

	CString strOut;
	for( int i=0; i<nDisplay; i++ )
		{
		CString cTemp;
		cTemp.Format(_T("%d"), m_aiAcheivedTurnpoints[i]);
		strOut+=cTemp;
		if( i!=m_nAcheivedTurnpoints-1 ) strOut+=_T(",");
		}

	if( m_eTaskType==eModifiedAssigned && !IsFinishTimeValid() ) 
		{
		CString cTemp;
		cTemp.Format(_T(" >%d"), GetSoughtTurnpoint() );
		strOut+=cTemp;
		}

	if( CheckOption(FLT_TURNPOINTSLOCKED ) && bAddDot ) strOut+=" *";
	return strOut;
}

CString CFlight::GetFinishTimeText(bool bAddDot)
{
	CString strOut;
	if( IsTaskComplete() && m_bValidFinish )
		{
		strOut=GetFinishTime().Format(_T("%H:%M:%S"));
		if( CheckOption(FLT_FINISHTIMELOCKED) && bAddDot ) strOut+=" *";
		return strOut;
		}
	else
		return _T("");
}

CString CFlight::GetStartTimeText(bool bAddDot)
	{
	CString strOut;
	if( m_bValidStart)
		{
		strOut=m_cStartTime.Format(_T("%H:%M:%S"));
		if( CheckOption(FLT_STARTTIMELOCKED) && bAddDot ) strOut+=" *";
		return strOut;
		}
	else
		return _T("");
	}
CString CFlight::GetTaskOpenTimeText()
	{
	return m_cTaskOpenTime.Format(_T("%H : %M : %S"));
	}

CString CFlight::GetLandingLocationText(bool bAddDot)
	{
	if( IsTaskComplete() || !IsContestant() || m_eStatus==eNotAnalyzed  )
		return _T("");
	else
		{
		if( m_iLandoutAirfield<0 )
			{
			return m_cLandingLocation.GetFormattedInfo();
			}
		else
			{
			CString strOut;
			strOut.Format(_T("%d - %s"), m_iLandoutAirfield,m_strLandoutAirfield);
			return strOut;
			}
		}
	}

CString CFlight::GetScoredLandingPointText(bool bAddDot)
	{
	if( IsTaskComplete() || !IsContestant() || m_eStatus==eNotAnalyzed )
		return _T("");
	else
		{
		CString strOut=m_cScoredLandingPointLocation.GetFormattedInfo();
		if( CheckOption(FLT_SLANDINGPOINTLOCKED) && bAddDot ) strOut+=" *";
		return strOut;
		}

	}


CString CFlight::GetRollTimeText(bool bAddDot)
	{
	CString strOut;
	if( m_cRollTime.GetYear()<1990 ) 
		return _T("");
	else
		{
		strOut=m_cRollTime.Format(_T("%H:%M:%S"));
		if( CheckOption(FLT_ROLLTIMELOCKED) && bAddDot ) strOut+=" *";
		return strOut;
		}
	}

//The comparison function must return a negative value if the 
//first item should precede the second, a positive value if 
//the first item should follow the second, or zero if the two 
//items are equivalent

int CALLBACK CompareFlight(LPARAM lParam1, LPARAM lParam2, LPARAM iColumn)
{
	if( lParam1==NULL && lParam2!=NULL ) 
		return 1;

	if( lParam1!=NULL && lParam2==NULL ) 
		return -1;

	if( lParam1==NULL && lParam2==NULL ) 
		return 0;

	int iFact=1;
	if( iColumn<0 ) 
		{
		iFact=-1;
		}
	switch( abs(iColumn)-1 )
		{

	case 0: // Contest NO
		return iFact*strcmp( ((CFlight*)lParam1)->m_strCID,
			  				 ((CFlight*)lParam2)->m_strCID  );
	case 1: // Pilot Name
		return iFact*((CFlight*)lParam1)->m_strPilot.CompareNoCase(((CFlight*)lParam2)->m_strPilot);
	
	case 2:  // Status
		return iFact*strcmp(  ((CFlight*)lParam1)->GetStatusText(),
							  ((CFlight*)lParam2)->GetStatusText() );
	case 3:  // Distance
		if( ((CFlight*)lParam1)->GetDistance() == ((CFlight*)lParam2)->GetDistance())
			return 0;
		else if( ((CFlight*)lParam1)->GetDistance() > ((CFlight*)lParam2)->GetDistance())
			return iFact;
		else if( ((CFlight*)lParam1)->GetDistance() < ((CFlight*)lParam2)->GetDistance())
			return -iFact;
	case 4:  // Warnings
		return iFact*strcmp(  ((CFlight*)lParam1)->GetNumWarningsText(),
							  ((CFlight*)lParam2)->GetNumWarningsText() );
	case 5: // Roll time
		return iFact*strcmp(  ((CFlight*)lParam1)->GetRollTimeText(),
							  ((CFlight*)lParam2)->GetRollTimeText() );
	case 6: // Start time
		return iFact*strcmp(  ((CFlight*)lParam1)->GetStartTimeText(),
							  ((CFlight*)lParam2)->GetStartTimeText() );
	case 7: //  Finish time
		return iFact*strcmp(  ((CFlight*)lParam1)->GetFinishTimeText(),
							  ((CFlight*)lParam2)->GetFinishTimeText() );
	case 8: //  Landing time
		return iFact*strcmp(  ((CFlight*)lParam1)->GetLandingTimeText(),
							  ((CFlight*)lParam2)->GetLandingTimeText() );
	case 9: //  Start Point ID
		{
		if( ((CFlight*)lParam1)->m_cStartGate.GetPointID() == ((CFlight*)lParam2)->m_cStartGate.GetPointID())
			return 0;
		else if( ((CFlight*)lParam1)->m_cStartGate.GetPointID() > ((CFlight*)lParam2)->m_cStartGate.GetPointID())
			return iFact;
		else if( ((CFlight*)lParam1)->m_cStartGate.GetPointID() < ((CFlight*)lParam2)->m_cStartGate.GetPointID())
			return -iFact;
		}
	case 10: //  Acheived Turnpoints
		return iFact*strcmp(  ((CFlight*)lParam1)->AcheivedTurnpointText(),
							  ((CFlight*)lParam2)->AcheivedTurnpointText() );
	case 11: //  Landing Location
		return iFact*strcmp(  ((CFlight*)lParam1)->GetLandingLocationText(),
							  ((CFlight*)lParam2)->GetLandingLocationText() );
	case 12: //  Landing Location
		return iFact*strcmp(  ((CFlight*)lParam1)->GetAirfieldBonusText(),
							  ((CFlight*)lParam2)->GetAirfieldBonusText() );
	case 13: //  Landing Location
		return iFact*strcmp(  ((CFlight*)lParam1)->GetScoredLandingPointText(),
							  ((CFlight*)lParam2)->GetScoredLandingPointText() );
	case 14: // igc filename
		return iFact*((CFlight*)lParam1)->GetIGCFileNameText().CompareNoCase(((CFlight*)lParam2)->GetIGCFileNameText());

	case 15: // Flight Log Security
		if( ((CFlight*)lParam1)->SecurityPassed() == ((CFlight*)lParam2)->SecurityPassed() )
		   return 0;
		else if( ((CFlight*)lParam1)->SecurityPassed() && !((CFlight*)lParam2)->SecurityPassed() )
		   return 1;
		else if( !((CFlight*)lParam1)->SecurityPassed() && ((CFlight*)lParam2)->SecurityPassed() )
		   return -1;
	}
	return 0;
}


CString CFlight::GetLandingTimeText(bool bAddDot)
	{
	CString strOut;
	if( !IsContestant() || m_eStatus==eNotAnalyzed )
		return _T("");
	else
		{
		strOut=m_cLandingTime.Format(_T("%H:%M:%S"));
		if( CheckOption(FLT_LANDINGTIMELOCKED) && bAddDot ) strOut+=" *";
		return strOut;
		}
	}

void CFlight::SetStatus(EFlightStatus eStatus)
{
m_eStatus=eStatus;
}

bool CFlight::IsContestant()
{
	return m_eStatus!=eNotEnteredInContest;
}

void CFlight::InitializeAnalysisParams(EUnits	eUnits)
{
	m_eUnits=eUnits;
	m_cStartTime=CTime(0);
	m_cRollTime=CTime(0);
	m_cFinishTime=CTime(0);
	m_cLandingTime=CTime(0);

	m_bValidFinish=false;
	m_bValidStart =false;
	m_bAllTurnpointsAcheived=false;
	m_bLocked=false;

	m_pcStartControlPoint=NULL;
	m_pcFinishControlPoint=NULL; 
	m_pcHomePoint=NULL;

	m_nAcheivedTurnpoints=0;
	for( int i=0; i<ALLOCTASKTPS; i++) 
		{
		m_aiAcheivedTurnpoints[i]=0;
		m_aiTaskTurnpointIDs[i]=0;
		m_aiTurnAreaTurnpointFixIDs[i]=0;
		}
		
	m_iStartPenalty=0;

	m_cStartGate.SetRadius(ConvertDistance(		5.0, eStatute, SYSTEMUNITS));
	m_cFinishGate.SetRadius(ConvertDistance(	2.0, eStatute, SYSTEMUNITS));

	m_eTaskType=eAssigned;

	m_dTurnpointRadius		= GetWinscoreDouble(INNERRADIUS, ConvertDistance(DEFAULTINNERRADIUS, eStatute, SYSTEMUNITS) );
	m_dTurnpointOuterRadius	= GetWinscoreDouble(OUTERRADIUS, ConvertDistance(DEFAULTOUTERRADIUS, eStatute, SYSTEMUNITS) );

	m_nTaskPoints=0;
	m_nMATTaskPoints=0;
	for( int i=0; i<ALLOCTASKTPS; i++) 
		{
		m_pcTaskTurnpoints[i]=NULL;
		m_adTurnpointRadii[i]=m_dTurnpointRadius;
		}

	// 2 Minutes
	m_cAltitudeControlTime=CTimeSpan(60*2);

	m_iLandoutAirfield= -1;
	m_strLandoutAirfield=_T("");
	m_iSoughtTurnpoint= -1;

	m_bValidFinish=false;
	m_bValidStart=false;
	m_iStartPenalty=0;
	m_iOptions=0;
	ResetAllWarnings();
	m_dDistance=0.0;


}

CString CFlight::GetNumWarningsText()
{
	if( NumberOfWarnings()==0 )
		return _T("");
	else if(NumberOfActiveWarnings()==0 )
		return _T("0");
	else
		{
		CString strOut;
		strOut.Format(_T("%d"),NumberOfActiveWarnings());
		return strOut;
		}
}

void CFlight::CheckGaps()
{
	int iPoint=max(FindTime( m_cStartTime,  0, FORWARD ),0);
	int iLdgPoint=FindTime( m_cLandingTime,  iPoint, FORWARD );
	if( IsFinishTimeValid() )
		iLdgPoint=FindTime( m_cFinishTime,  iPoint, FORWARD );

	while (1)
		{
		iPoint=FindEvent( FAN_GAP,  iPoint, FORWARD );
		if( iPoint<0 ) break;
		if( iPoint>iLdgPoint ) break;

		CPosition* pcPos=GetPosition(iPoint);
		CPosition* pcPrevPos=GetPosition(iPoint-1);
		if( !pcPos || !pcPrevPos ) break;

		CTimeSpan cSpan=pcPos->m_cTime-pcPrevPos->m_cTime;
		CString cWarn;
		cWarn.Format(_T("%s - A gap in the trace of %s was detected."), pcPrevPos->m_cTime.Format(_T("%H:%M:%S")), cSpan.Format(_T("%H:%M:%S")));
		AddWarning(eInformation,0,cWarn);
		iPoint++;
		}


}

CTime CFlight::GetRollTime()
	{
	return m_cRollTime;
	}

CTime CFlight::GetLandingTime()
	{
	return m_cLandingTime;
	}

CTime CFlight::GetTaskOpenTime()
	{
	return m_cTaskOpenTime;
	}

int CFlight::GetLandoutTurnpointID()
	{
	return m_iLandoutAirfield;
	}

CString CFlight::GetTaskPointText(int nDisplay)
	{
	if( nDisplay==0 ) return _T("");

	CString strOut;
	for( int i=0; i<nDisplay; i++ )
		{
		CString cTemp;
		cTemp.Format(_T("%d"), m_aiTaskTurnpointIDs[i]);
		strOut+=cTemp;
		if( i!=nDisplay-1 ) strOut+=_T(",");
		}
	return strOut;
	}

void CFlight::SetLandingLocation(CLocation &cLocation)
	{
	m_cLandingLocation=cLocation;
	}

CString CFlight::GetFlightText(TASKCLASS *pcTask, TURNPOINTCLASSARRAY *pTURNPOINTCLASSArray, CString strSeparator)
{
	CString strOut;
	strOut+=_T("Contest ID: ")+m_strCID;
	strOut+=strSeparator;
	strOut+=_T("Pilot: ")+m_strPilot;
	strOut+=strSeparator;
	strOut+=_T("Status: ")+GetStatusText();
	strOut+=_T("\r\n");
	if( m_eStatus==eNotAnalyzed ) return strOut;

	strOut+=_T("Roll Time: ")+GetRollTimeText();
	strOut+=strSeparator;
	strOut+=_T("Start Time: ")+GetStartTimeText();
	strOut+=strSeparator;
	strOut+=_T("Finish Time: ")+GetFinishTimeText();
	strOut+=strSeparator;
	strOut+=_T("Landing Time: ")+GetLandingTimeText();
	strOut+=strSeparator;
	strOut+=_T("Turnpoints: ")+AcheivedTurnpointText();
	strOut+=strSeparator;
	strOut+=_T("Landed At: ")+GetLandingLocationText();
	strOut+=strSeparator;
	strOut+=_T("Distance: ")+GetDistanceText();
	strOut+=strSeparator;

	if( pTURNPOINTCLASSArray!=NULL )
		{
		strOut+=GetTurnAreaText(pTURNPOINTCLASSArray);
		}

	for( int i=0; i<NumberOfWarnings(); i++ )
		{
		CWarning *pWarn=GetWarning(i);
		if( pWarn ) strOut+=pWarn->GetText();;
		strOut+=_T("\r\n");
		}

	return strOut;

}

void CFlight::CheckAllTimes()
{
	if( m_cRollTime  > m_cStartTime && IsStartTimeValid() )
		AddWarning(eInformation,0,_T("Roll time was calculated as later than the start time."));

	if(  m_cStartTime>m_cFinishTime && IsStartTimeValid() && IsFinishTimeValid() )
		AddWarning(eInformation,0,_T("Start time was calculated as later than the finish time."));

	if( !CheckOption(FLT_SAFETYFINISH ) )
		{
		if(  m_cFinishTime>m_cLandingTime && IsFinishTimeValid() )
			AddWarning(eInformation,0,"Finish time was calculated as later than the landing time.");
		}

}

bool CFlight::UpdateCID(CONTESTANTLISTCLASS *pcontestantList)
{
	BOOL bFound=true;
	CContestant *pcContestant=pcontestantList->GetByContestNo(m_strCompetitionID);
	if( pcContestant )
		m_strCID=m_strCompetitionID;
	else
		{
		pcContestant=pcontestantList->GetByContestNo(m_strGliderID);
		if( pcContestant )
			m_strCID=m_strGliderID;
		else
			{
			pcContestant=pcontestantList->GetByPilotName(m_strPilot);
			if( pcContestant )
				m_strCID=pcContestant->m_strContestNo;
			else
				{
				if( m_strCompetitionID.GetLength()>0 )
					m_strCID=m_strCompetitionID;
				else
					{
					m_strCID=m_strGliderID;
					bFound=FALSE;
					}
				}
			}
		}
	m_strCID.MakeUpper();
	return (bFound)?(true):(false);
	}



void CFlight::CheckBFI()
{
	int nBFIFixes=0;
	for( int iPos=0; iPos<GetNumPoints(); iPos++ )
		{
		CPosition* pcPos=GetPosition(iPos);
		if( pcPos->m_bBFI ) nBFIFixes++;
		}

	if (nBFIFixes==0) return;

	CString strErr;
	strErr.Format("BFI Artificial Horizon was ON for %i fixes.", nBFIFixes);
	AddWarning(eInformation,0,strErr);
}


void CFlight::CheckMotorRun()
{
	CPosition* pcPrevPos=NULL;
	CTimeSpan cTotalMotorONTime(0);
	CTimeSpan cLongestMotorONTime(0);
	CTimeSpan cMotorONTime(0);
	CPosition* pcLongestMotorOn=NULL;
	CPosition* pcMotorOn=NULL;
	CMedian    cENLNoise, cMOPNoise, cAverager;
	int		   iMax=0;
	int		   iDiff=0;

// First, check for any signal at all
	bool bSignal=false;
	for( int iPos=0; iPos<GetNumPoints(); iPos++ )
		{
		CPosition* pcPos=GetPosition(iPos);
		if( pcPos->m_iEngineNoiseLevel>0 || pcPos->m_iMOPLevel>0 )
			{
			bSignal=true;
			break;
			}
		}
	if( !bSignal ) 
		{
		AddWarning(eInformation,0,"No engine noise signal was detected, check motor run manually." );
		return;
		}
	
// Find the start and end points of interest
	int iStartPos=FindEvent(FAN_LATEST_START, 0, FORWARD );
	if( iStartPos<0 ) iStartPos=0;

	int iFinishPos=FindEvent(FAN_FINISH, GetNumPoints()-1, BACKWARD );
	if( iFinishPos<0 ) 
			{
			iFinishPos=FindEvent(FAN_LANDED, GetNumPoints()-1, BACKWARD );
	if( iFinishPos<0 ) iFinishPos=GetNumPoints();
			}

	// Now smooth the data with a rolling average
	int nAve=5;
	for( int iPos=iStartPos+nAve/2; iPos<iFinishPos-nAve/2; iPos++ )
		{
		int iSumENL=0;
		int iSumMOP=0;
		for( int iAve=-nAve/2; iAve<=nAve/2; iAve++ )
			{
			CPosition* pcPos=GetPosition(iPos+iAve);
			iSumENL+=pcPos->m_iEngineNoiseLevel;
			iSumMOP+=pcPos->m_iMOPLevel;
			}
		iSumENL/=nAve;
		iSumMOP/=nAve;
		CPosition* pcPos=GetPosition(iPos);
		pcPos->m_iEngineNoiseLevel=iSumENL;
		pcPos->m_iMOPLevel=iSumMOP;
		}


	// Find max noise levels for ENL and MOP
	for( int iPos=0; iPos<GetNumPoints(); iPos++ )
		{
		CPosition* pcPos=GetPosition(iPos);
		cENLNoise.AddSample(pcPos->m_iEngineNoiseLevel);
		cMOPNoise.AddSample(pcPos->m_iMOPLevel);
		}
	int iENLRange=cENLNoise.GetMax()-cENLNoise.GetMin();
	int iMOPRange=cMOPNoise.GetMax()-cMOPNoise.GetMin();
	
	if( iENLRange<5 && iMOPRange<5 ) return;  // Not enough amplitude, no run.


#if 0
	bool bUseENL=(iENLRange>0);
	if( bUseENL )
		{
		iMax	=cENLNoise.GetMax();
		// Normalize to 150 for baro display.
		for( int iPos=0; iPos<GetNumPoints(); iPos++ )
			{
			CPosition* pcPos=GetPosition(iPos);
			double dPercent=double(pcPos->m_iEngineNoiseLevel)/double(iMax);
				pcPos->m_iEngineNoiseLevel=int(dPercent*150.);
			}
		}
	else
		{
		iMax	=cMOPNoise.GetMax();
		// Normalize to 150 for baro display.
		for( int iPos=0; iPos<GetNumPoints(); iPos++ )
			{
			CPosition* pcPos=GetPosition(iPos);
			double dPercent=double(pcPos->m_iMOPLevel)/double(iMax);
				pcPos->m_iMOPLevel=int(dPercent*150.);
				pcPos->m_iEngineNoiseLevel=int(dPercent*150.);  //Baro display only works off enl.
				}
		}

#else

	//Experimental code to consider both MOP and ENL.

	iMax	=cENLNoise.GetMax();
	// Normalize to 150 for baro display.
	for( int iPos=0; iPos<GetNumPoints(); iPos++ )
		{
		CPosition* pcPos=GetPosition(iPos);
		double dPercent=double(pcPos->m_iEngineNoiseLevel)/double(iMax);
		pcPos->m_iEngineNoiseLevel=int(dPercent*150.);
		}

	iMax	=cMOPNoise.GetMax();
	// Normalize to 150 for baro display.
	for( int iPos=0; iPos<GetNumPoints(); iPos++ )
		{
		CPosition* pcPos=GetPosition(iPos);
		double dPercent=iMax>0?double(pcPos->m_iMOPLevel)/double(iMax):0.0;
		pcPos->m_iMOPLevel=int(dPercent*150.);
		}
		
	CMedian cAveragerMOP;
	//Find Average
	for( int iPos=iStartPos; iPos<iFinishPos; iPos++  )
		{
		CPosition* pcPos=GetPosition(iPos);
		cAveragerMOP.AddSample(pcPos->m_iMOPLevel);
		}
	double dAve2=cAveragerMOP.dAverage();


	if( dAve2<140. && iMOPRange>0 ) // Check valid MOP, if OK, use max MOP ENL
		{
		// test by using max eln and mop
		for( int iPos=0; iPos<GetNumPoints(); iPos++ )
			{
			CPosition* pcPos=GetPosition(iPos);
			pcPos->m_iEngineNoiseLevel=max(pcPos->m_iEngineNoiseLevel,pcPos->m_iMOPLevel);
			}
		}

#endif

	// Check if most of the points are off scale, this indicates a sensor failure.
	int nMax=0;
	for( int iPos=0; iPos<GetNumPoints(); iPos++ )
		{
		CPosition* pcPos=GetPosition(iPos);
		if( pcPos->m_iEngineNoiseLevel>140 ) nMax++;
		}
	double dBogus=double(nMax)/(double)GetNumPoints();
	if( dBogus>.85 )
		{
		AddWarning(eInformation,0,"Engine noise sensor failure, check engine run manually.");
		return;
		}


	//Find Standard Deviation, only in range of interest.
	for( int iPos=iStartPos; iPos<iFinishPos; iPos++ )
		{
		CPosition* pcPos=GetPosition(iPos);
		cAverager.AddSample(pcPos->m_iEngineNoiseLevel);
		}
	double dStdDev=cAverager.StdDeviation();
	double dAve=cAverager.dAverage();

	if( dStdDev<1.0 ) return;

	// Now set the Engine ON status based on average + 2*std deviation and flying <100kts
	for( int iPos=iStartPos; iPos<iFinishPos; iPos++ )
		{
		CPosition* pcPos=GetPosition(iPos);
		int iVal=pcPos->m_iEngineNoiseLevel;
		if( iVal<20 ) continue;  //Since we normalized to 150, make sure we have sufficent signal.
		if( double(iVal)>((1.0*dStdDev)+dAve) && pcPos->m_dSpeed<100. && pcPos->m_dSpeed>25. )
			{
			pcPos->AddStatus( FAN_MOTOR_ON);
			}
		}

	// Search and smooth out spikes.
	pcPrevPos=NULL;
	int nMAXON=30;
	int nON=0;
	bool bSearching=false;
	CPosition *pcMotorON[30];
	
	for( int iPos=0; iPos<GetNumPoints(); iPos++ )
		{
		CPosition* pcPos=GetPosition(iPos);
		if( pcPrevPos && pcPos )
			{

			if( !pcPrevPos->CheckStatus(FAN_MOTOR_ON) &&
			    pcPos->CheckStatus(FAN_MOTOR_ON) 	)
				{
				//Motor came on, start looking for an OFF
				bSearching=true;
				pcMotorON[nON++]=pcPos;
				}
			else if ( pcPrevPos->CheckStatus(FAN_MOTOR_ON) &&
			          pcPos->CheckStatus(FAN_MOTOR_ON) 	   &&
					  bSearching )
				{
				// Continued motor run, accumulate.
				if( nON<nMAXON )
					pcMotorON[nON++]=pcPos;
				else
					{
					bSearching=false;
					nON=0;
					}
				}
			else if ( pcPrevPos->CheckStatus(FAN_MOTOR_ON) &&
			          !pcPos->CheckStatus(FAN_MOTOR_ON) 	)
				{
				//Motor turned off, check for spike
				if( bSearching )
					{
					if( nON>0 )
						{
						bSearching=false;
						for( int iON=0; iON<nON; iON++)
							pcMotorON[iON]->RemoveStatus(FAN_MOTOR_ON);
						nON=0;
						}
					}
				}

			}
		pcPrevPos=pcPos;
		}

	// Search and fix dropouts
	int nOFF=0;
	bSearching=false;
	pcPrevPos=NULL;

	for( int iPos=0; iPos<GetNumPoints(); iPos++ )
		{
		CPosition* pcPos=GetPosition(iPos);
		if( pcPrevPos && pcPos )
			{

			if( pcPrevPos->CheckStatus(FAN_MOTOR_ON) &&
			    !pcPos->CheckStatus(FAN_MOTOR_ON) 	)
				{
				//Motor off, start looking for an ON
				bSearching=true;
				pcMotorON[nOFF++]=pcPos;
				}
			else if ( !pcPrevPos->CheckStatus(FAN_MOTOR_ON) &&
			          !pcPos->CheckStatus(FAN_MOTOR_ON) 	   &&
					  bSearching )
				{
				// Continued motor off, accumulate.
				if( nOFF<nMAXON )
					pcMotorON[nOFF++]=pcPos;
				else
					{
					bSearching=false;
					nOFF=0;
					}
				}
			else if ( !pcPrevPos->CheckStatus(FAN_MOTOR_ON) &&
			          pcPos->CheckStatus(FAN_MOTOR_ON) 	)
				{
				//Motor turned on, check for dropout
				if( bSearching )
					{
					if( nOFF>0 )
						{
						bSearching=false;
						for( int iOFF=0; iOFF<nOFF; iOFF++)
							pcMotorON[iOFF]->AddStatus(FAN_MOTOR_ON);
						nOFF=0;
						}
					}
				}

			}
		pcPrevPos=pcPos;
		}

	// Now find the 1st run, not necessciarily the longest

	pcPrevPos=NULL;
	for( int iPos=iStartPos; iPos<GetNumPoints(); iPos++ )
		{
		CPosition* pcPos=GetPosition(iPos);
		if( pcPrevPos && pcPos )
			{
			// Sum the total
			if( pcPos->CheckStatus(FAN_MOTOR_ON) &&
				pcPrevPos->CheckStatus(FAN_MOTOR_ON) 	)
				{
				cTotalMotorONTime+=pcPos->m_cTime-pcPrevPos->m_cTime;
				}

			//Sum the longest range
			if( !pcPrevPos->CheckStatus(FAN_MOTOR_ON) &&
			    pcPos->CheckStatus(FAN_MOTOR_ON) 	)
				{
				//Motor came on, start counting
				cMotorONTime=pcPos->m_cTime-pcPrevPos->m_cTime;
				pcMotorOn=pcPos;
				}
			else if ( pcPrevPos->CheckStatus(FAN_MOTOR_ON) &&
			          pcPos->CheckStatus(FAN_MOTOR_ON) 	)
				{
				//Continued motor run, accumulate
				cMotorONTime+=pcPos->m_cTime-pcPrevPos->m_cTime;
				}
			else if ( (pcPrevPos->CheckStatus(FAN_MOTOR_ON) &&
			           !pcPos->CheckStatus(FAN_MOTOR_ON)) || iPos==iFinishPos 	)
				{
				//Motor turned off or crossed finish with motor running.
				// If >45 sec, go ahead and use it.
				if( cMotorONTime>45 )
					{
					cLongestMotorONTime=cMotorONTime;
					pcLongestMotorOn=pcMotorOn;
					break;
					}
				if( cMotorONTime>cLongestMotorONTime )
					{
					cLongestMotorONTime=cMotorONTime;
					pcLongestMotorOn=pcMotorOn;
					}
				}

			}
		if( iPos==iFinishPos ) break;
		pcPrevPos=pcPos;
		}

	if( pcLongestMotorOn )
		{
		// Find average altitude during this motor run
		int iMotorOn=FindTime(pcLongestMotorOn->m_cTime, 1, FORWARD);
		int iMotorOff=FindTime(pcLongestMotorOn->m_cTime+cMotorONTime.GetTotalSeconds(), iMotorOn, FORWARD);
		CMedian cAveAlt;
		for( int iPos=iMotorOn; iPos<iMotorOff; iPos++ )
			{
			CPosition* pcPos=GetPosition(iPos);
			cAveAlt.AddSample(pcPos->m_iCorrectedAltitude);
			}
		int iAveAltitude=cAveAlt.Average();
		iDiff=iAveAltitude-pcLongestMotorOn->m_iCorrectedAltitude; //Difference between motor on and ave motor run +means climbs
		}


	if( cLongestMotorONTime.GetTotalSeconds()>45 &&
		!CheckOption(FLT_SLANDINGPOINTLOCKED)	 &&
		!CheckOption(FLT_LANDINGTIMELOCKED)		 &&
		!CheckOption(FLT_FINISHTIMELOCKED)		 &&
		!CheckOption(FLT_TURNPOINTSLOCKED)		 &&
		pcLongestMotorOn!=NULL                   &&
		iDiff>-20 )
		{
		pcLongestMotorOn->AddStatus(FAN_LANDED);
		m_cLandingLocation=*pcLongestMotorOn;
		m_cLandingTime=pcLongestMotorOn->m_cTime;
        m_bValidFinish=false;
		AddWarning(eMotorRunLandout,0,m_cLandingTime.Format(_T("Landout occured at %H:%M:%S due to motor run")) );
		// Remove any finish penalty warnings.
		int iWarn=GetWarning(eFinish);
		if( iWarn>=0 )
			{
			RemoveWarning(iWarn);	
			}
		}


}

void CFlight::SetAcheviedTurnpoint(int i, int iID)
	{
	m_aiAcheivedTurnpoints[i]=iID;
	}

int CFlight::GetTaskPointID(int iTskPt)
{
	return m_aiTaskTurnpointIDs[iTskPt];
}



void CFlight::LocateFurthestProgess(TASKCLASS			*pcTask, CGate &cFinishGate,TURNPOINTCLASSARRAY &cTurnpointArray)
	{
	CPosition* pcFurthest=NULL;
	int iStartPos=0;
	m_cScoredLandingPointLocation=m_cLandingLocation;

	if( IsTaskComplete() || m_nTaskPoints==0 )
		{
		return;
		}

	if( m_nAcheivedTurnpoints==0 )
		{
		int iClose=FindClosestPointToTurnpoint(0);
		iStartPos=FindEvent(FAN_START, iClose, BACKWARD );
		}
	else
		iStartPos=FindEvent(FAN_LATEST_START, 0, FORWARD );


	int iLastTpPnt=iStartPos;
	for( int i=0; i< m_nAcheivedTurnpoints; i++ )
		{
		int iNext=FindAcheivedTurnpoint( IsAreaTask()?FAN_TURN_AREA_TURNPOINT:FAN_AT_TURNPOINT, 
										  m_aiTaskTurnpointIDs[i], 
										  iLastTpPnt, 
										  FORWARD );
		if( iNext<0 )
			iNext=FindAcheivedTurnpoint( FAN_OUTER_TURNPOINT, 
											  m_aiTaskTurnpointIDs[i], 
											  iLastTpPnt, 
											  FORWARD );
		iLastTpPnt=iNext;

		if( iLastTpPnt<0 ) 
			iLastTpPnt=iStartPos;

		}

	if( iLastTpPnt<0 ) 
		iLastTpPnt=iStartPos;

	if( m_eTaskType==eModifiedAssigned &&
		m_nAcheivedTurnpoints<m_nMATTaskPoints &&
		m_nAcheivedTurnpoints>0 ) 
		{
		//  Ok, this logic is to determine if the pilot on a MAT task 
		//  went further to the next turnpoint in his task, or went home.
		CPosition* pcFurthestFinish=NULL;
		CPosition* pcFurthestNext=NULL;
		double dDistNext=0;
		double dDistFini=0;

		// Get distance to finish
		double dMin=1.e+30;
		iLastTpPnt=max(iLastTpPnt,0);
		for( int i=iLastTpPnt; i<GetNumPoints(); i++)
			{
			CPosition *pcPos=GetPosition(i);
			if( pcPos==NULL ) continue;
			if( pcPos->m_dDistanceFromFinish< dMin )
				{
				dMin=pcPos->m_dDistanceFromFinish;
				pcFurthestFinish=pcPos;
				}
			if( pcPos->m_cTime> m_cLandingTime ) break;
			}

		//Get distance to next
		if( m_pcTaskTurnpoints[m_nAcheivedTurnpoints] ) 
			pcFurthestNext=FindFurthestProgress( iLastTpPnt, CLocation(m_pcTaskTurnpoints[m_nAcheivedTurnpoints]->GetLat(),m_pcTaskTurnpoints[m_nAcheivedTurnpoints]->GetLong()));

		if( pcFurthestNext )
			dDistNext= CLocation(m_pcTaskTurnpoints[m_nAcheivedTurnpoints-1]->GetLat(),m_pcTaskTurnpoints[m_nAcheivedTurnpoints-1]->GetLong()).DistanceTo(&CLocation(pcFurthestNext->GetLat(),pcFurthestNext->GetLong()),SYSTEMUNITS);
		if( pcFurthestFinish )
			dDistFini= CLocation(m_pcTaskTurnpoints[m_nAcheivedTurnpoints-1]->GetLat(),m_pcTaskTurnpoints[m_nAcheivedTurnpoints-1]->GetLong()).DistanceTo(&CLocation(pcFurthestFinish->GetLat(),pcFurthestFinish->GetLong()),SYSTEMUNITS);

		if( dDistNext>dDistFini )
			{
			pcFurthest=pcFurthestNext;
			}
		else
			{
			pcFurthest=pcFurthestFinish;
			}

		}
	else if( m_nTaskPoints==m_nAcheivedTurnpoints && GetSoughtTurnpoint()==GetFinishGateID() )
		{
		//  Got all the turnpoints, find progress toward finish
		iLastTpPnt=max(iLastTpPnt,0);
		int iFinishPointID = cFinishGate.GetPointID();
		if( cFinishGate.IsGPSCylinder() &&
			pcTask->m_eType==eFAIAssignedArea &&
			m_pcTaskTurnpoints[m_nAcheivedTurnpoints-1]  )
			{
			CLocation cFinishLoc=cTurnpointArray.GetAt(cFinishGate.GetPointID()-1);
			//Use the weird FAI distance computation which progress is the max distance, not best progress toward
			pcFurthest=FindFurthestFAIProgress( iLastTpPnt, 
												cFinishLoc,
											    CLocation(m_pcTaskTurnpoints[m_nAcheivedTurnpoints-1]->GetLat(),m_pcTaskTurnpoints[m_nAcheivedTurnpoints-1]->GetLong()),
												cFinishGate.GetRadius());
			}
		else
			{
			double dMin=1.e+30;
			for( int i=iLastTpPnt; i<GetNumPoints(); i++)
				{
				CPosition *pcPos=GetPosition(i);
				if( pcPos==NULL ) continue;
				if( pcPos->m_dDistanceFromFinish< dMin )
					{
					dMin=pcPos->m_dDistanceFromFinish;
					pcFurthest=pcPos;
					}
				if( pcPos->m_cTime> m_cLandingTime ) break;
				}
			}
		}
	else
		{
		// Find furthest progress toward next turnpoint,
		if( pcTask->m_eType==eFAIAssignedArea &&
			m_pcTaskTurnpoints[m_nAcheivedTurnpoints] &&
			m_nAcheivedTurnpoints>0 &&
			m_pcTaskTurnpoints[m_nAcheivedTurnpoints-1] )
			{
			//Use the weird FAI distance computation which progress is the max distance, not best progress toward
			pcFurthest=FindFurthestFAIProgress( iLastTpPnt,  
											    CLocation(m_pcTaskTurnpoints[m_nAcheivedTurnpoints]->GetLat(),m_pcTaskTurnpoints[m_nAcheivedTurnpoints]->GetLong()), 
												CLocation(m_pcTaskTurnpoints[m_nAcheivedTurnpoints-1]->GetLat(),m_pcTaskTurnpoints[m_nAcheivedTurnpoints-1]->GetLong()),
												m_adTurnpointRadii[m_nAcheivedTurnpoints]);
			}
		else if (pcTask->m_eType==eFAIAssignedArea &&
				m_pcTaskTurnpoints[0] &&
				m_nAcheivedTurnpoints==0  && 
				m_pcStartControlPoint )
			{
			//Landed out on the way to the 1st turnpoint, Use the weird FAI distance computation which progress is the max distance, not best progress toward
			pcFurthest=FindFurthestFAIProgress( iLastTpPnt,  
												CLocation(m_pcTaskTurnpoints[0]->GetLat(),m_pcTaskTurnpoints[0]->GetLong()),
												CLocation(m_pcStartControlPoint->GetLat(),m_pcStartControlPoint->GetLong()),
												m_adTurnpointRadii[0]);
			}
		else
			{
			if( m_pcTaskTurnpoints[m_nAcheivedTurnpoints] ) 
				pcFurthest=FindFurthestProgress( iLastTpPnt,  CLocation(m_pcTaskTurnpoints[m_nAcheivedTurnpoints]->GetLat(),m_pcTaskTurnpoints[m_nAcheivedTurnpoints]->GetLong()) );
			}
		}

	if( pcFurthest ) 
		{
		pcFurthest->AddStatus(FAN_FURTHEST_PROGRESS);
		m_cScoredLandingPointLocation= *pcFurthest;
		}
	}

CLocation& CFlight::GetScoredLandingPointLocation()
	{
	return m_cScoredLandingPointLocation;
	}
/*
bool CFlight::IsRollingFinish()
{
	return m_iOptions & FLT_ROLLINGFINISH;
}

void CFlight::RollingFinish(bool b)
	{
	if( b )
		m_iOptions|=FLT_ROLLINGFINISH;
	else
		if( IsRollingFinish() ) m_iOptions^=FLT_ROLLINGFINISH;
	}
*/
bool CFlight::IsAirfieldBonus()
{
	return (m_iOptions & FLT_AIRFIELDBONUS)?true:false;
}

void CFlight::AirfieldBonus(bool b)
	{
	if( m_iOptions &  FLT_AIRFIELDBONUSLOCKED) return;
	if( b )
		m_iOptions|=FLT_AIRFIELDBONUS;
	else
		if( IsAirfieldBonus() ) m_iOptions^=FLT_AIRFIELDBONUS;
	}

bool CFlight::GetFlightTimeRemaining(int iPos, CTimeSpan &cTimeSpan)
	{

	CPosition* pcStartPos	=GetPosition(iPos);
	if( !pcStartPos ) return false;

	CPosition* pcPos	=NULL;

	for( int iPoint=iPos; iPoint<GetNumPoints(); iPoint++ )
		{
		pcPos=GetPosition(iPoint);
		if( pcPos->CheckStatus( FAN_LANDED ) ) break;
		}

	if( !pcPos ) return false;
	cTimeSpan=CTimeSpan(pcPos->m_cTime-pcStartPos->m_cTime);

	return true;
	}

#define COPYLOCATION( pt1, pt2 )\
	{\
	pt1.SetLat(pt2->GetLat() );\
	pt1.SetLong(pt2->GetLong() );\
	}\

void CFlight::FindOptimumTurnpoints(TASKCLASS *pcTask, TURNPOINTCLASSARRAY &TURNPOINTCLASSARRAY )
	{
    if( CheckOption(FLT_OPTTURNPOINTSLOCKED) ) return;

	if( m_nAcheivedTurnpoints==0 ) return;
	int iLastCheckedPoint=0;

	CPosition *pcBestPosition[ALLOCTASKTPS+2];
	for( int i=0; i<ALLOCTASKTPS+2; i++ ) 
		{
		pcBestPosition[i]=NULL;
		m_aiTurnAreaTurnpointFixIDs[i]=0;
		}

	CLocation cPoints[ALLOCTASKTPS+2];

	// Calculate from start to first turn
	COPYLOCATION(	cPoints[0], 
					TURNPOINTCLASSARRAY.GetAt(m_cStartGate.GetPointID()-1) );

	for( int i=0; i<GetNumTaskTurnpoints(); i++ )
		COPYLOCATION( cPoints[i+1], TURNPOINTCLASSARRAY.GetAt( GetTaskPointID(i)-1) );

	COPYLOCATION(	cPoints[GetNumTaskTurnpoints()+1], 
					TURNPOINTCLASSARRAY.GetAt(m_cFinishGate.GetPointID()-1) );

	if( pcTask->m_eType!=eFAIRacing )
    // Find optimum
  	  {
		double dTotal=0.0;
		int iLatestStart=FindEvent(FAN_LATEST_START, 0, FORWARD );
		int nMaxLoop=0;
		bool bMoved=false;
		do 
		{
			dTotal=0.0;
			bMoved=false;
			iLastCheckedPoint=max(iLatestStart,0);
		
			int iCheckFor=FAN_AT_TURNPOINT;
			for( int itpt=0; itpt<m_nAcheivedTurnpoints; itpt++ )
				{
				double dMaxDist=0.0;

	//		Check to see if he made the turnpoint, or did he only make the buffer area.  If he only made the
	//		buffer area, then check all points.  If he made the turnpoint, only check those points, not the buffer.

				if( FindAcheivedTurnpoint( FAN_AT_TURNPOINT, 
										   m_aiAcheivedTurnpoints[itpt], 
										   iLastCheckedPoint, 
										   FORWARD,
										   itpt )>0 )
					iCheckFor=FAN_AT_TURNPOINT;
				else
					iCheckFor=FAN_OUTER_TURNPOINT;


				CPosition* pcBestPos=NULL;
				int iBestPos= -1;
				for( int iPoint=iLastCheckedPoint; iPoint<GetNumPoints(); iPoint++ )
					{
					CPosition* pcPos=GetPosition(iPoint);
					if(pcPos==NULL) break;

					if( pcPos->m_cTime>m_cLandingTime ) 
						break;

					if( pcPos->CheckStatus(iCheckFor) &&
						pcPos->CheckTurnpoint( itpt, m_aiAcheivedTurnpoints[itpt])  )
						{
						//Update Distances
						double dDist=0.;
						dDist+=pcPos->DistanceTo(&cPoints[itpt],	SYSTEMUNITS);
						dDist+=pcPos->DistanceTo(&cPoints[itpt+2],	SYSTEMUNITS);
						if( dDist>dMaxDist )
							{
							pcBestPos=pcPos;
							dMaxDist=dDist;
							iBestPos=iPoint;
							}
						iLastCheckedPoint=iPoint;
						}

					// We have at least one point in the desired turn area and 
					// if we hit positions in the next turnpoint, stop checking
					// Also, if we have a valid finish, also stop
					if( iBestPos>0 &&
						(pcPos->CheckTurnpoint( itpt+1, m_aiAcheivedTurnpoints[itpt+1]) ||
						(IsFinishTimeValid() && pcPos->m_cTime>m_cFinishTime)))
						{
						iLastCheckedPoint=iPoint;
						break;
						}

					}
				dTotal+=dMaxDist;
				if( pcBestPos && pcBestPosition[itpt]!=pcBestPos ) 
					{
					bMoved=true;
					COPYLOCATION( cPoints[itpt+1], pcBestPos);
					pcBestPosition[itpt]=pcBestPos;
					m_aiTurnAreaTurnpointFixIDs[itpt]=iBestPos;
					}
				}
			if( nMaxLoop++ >10 ) break;
			} while (bMoved);
		}

	for( int itpt=0; itpt<m_nAcheivedTurnpoints; itpt++ )
		{
		if( pcTask->m_eType!=eFAIRacing && pcBestPosition[itpt] ) 
			{
			pcBestPosition[itpt]->AddStatus( FAN_TURN_AREA_TURNPOINT );
			}
		m_acTurnAreaTurnpointLocations[itpt].SetLat(cPoints[itpt+1].GetLat());
		m_acTurnAreaTurnpointLocations[itpt].SetLong(cPoints[itpt+1].GetLong());
		}
	}



	double CFlight::GetDistance() 
		{return m_dDistance;}


double CFlight::ComputeDistance(TASKCLASS *pcTask, TURNPOINTCLASSARRAY &turnpointarray )
	{

	double dDist=0;

	TURNPOINTCLASS *pSought=NULL;
	int iSought=GetSoughtTurnpoint();

	if( m_eTaskType==eModifiedAssigned && 
		!m_bValidFinish && 
		m_iSoughtTurnpoint <0 &&
		iSought!=m_cFinishGate.GetPointID() )
		{
		// For MATS, check to see if he would have gotten a better distance by going
		// directly to the finish instead of the next MAT point.

		pSought=turnpointarray[iSought-1];
		if( !pSought ) 
			pSought = turnpointarray[m_cFinishGate.GetPointID()-1];

		double dDistSought= ::GetDistance(	m_eTaskType,
											m_cStartGate,
											&m_cStartFix,
											turnpointarray,
											m_cFinishGate,
											m_nAcheivedTurnpoints,
											m_acTurnAreaTurnpointLocations,
											m_adTurnpointRadii,
											m_bValidFinish,
											*pSought,
											m_cScoredLandingPointLocation);

		pSought = turnpointarray[m_cFinishGate.GetPointID()-1];
		double dDistFinish= ::GetDistance(	m_eTaskType,
			                                m_cStartGate,
											&m_cStartFix,
											turnpointarray,
											m_cFinishGate,
											m_nAcheivedTurnpoints,
											m_acTurnAreaTurnpointLocations,
											m_adTurnpointRadii,
											m_bValidFinish,
											*pSought,
											m_cScoredLandingPointLocation);
		if( dDistFinish > dDistSought )
			{
			dDist=dDistFinish;
			SetSoughtTurnpoint(m_cFinishGate.GetPointID());
			}
		else
			dDist=dDistSought;

		}
	else
		{
		pSought=turnpointarray[iSought-1];
		if( !pSought ) 
			pSought = turnpointarray[m_cFinishGate.GetPointID()-1];

		dDist= ::GetDistance(	m_eTaskType,
								m_cStartGate,
								&m_cStartFix,
								turnpointarray,
								m_cFinishGate,
								m_nAcheivedTurnpoints,
								m_acTurnAreaTurnpointLocations,
								m_adTurnpointRadii,
								m_bValidFinish,
								*pSought,
								m_cScoredLandingPointLocation);
		}

	return max(dDist,0.0);
	}

int CFlight::GetSoughtTurnpoint()
{

	if( m_iSoughtTurnpoint!= -1 ) return m_iSoughtTurnpoint;

	if( !IsTaskComplete() )
		{
		if( m_eTaskType!=eModifiedAssigned )
			{
			if( m_nAcheivedTurnpoints==m_nTaskPoints )
				return m_cFinishGate.GetPointID();
			else
			return 	m_aiTaskTurnpointIDs[m_nAcheivedTurnpoints];
			}

		if( m_eTaskType==eModifiedAssigned && m_nAcheivedTurnpoints<m_nMATTaskPoints )
			{
			return 	m_aiTaskTurnpointIDs[m_nAcheivedTurnpoints];
			}

		if( m_nAcheivedTurnpoints==m_nTaskPoints )
			{
			return m_cFinishGate.GetPointID();
			}
		else
			return 	m_aiTaskTurnpointIDs[m_nAcheivedTurnpoints];
		}

	return m_cFinishGate.GetPointID();

}
void CFlight::SetSoughtTurnpoint(int iSought)
	{
	m_iSoughtTurnpoint=iSought;
	};



CString CFlight::GetStartGateText()
{
	if( m_cStartGate.GetPointID()<=0 ) return _T("");

	CString str;
	str.Format(_T("%d"),m_cStartGate.GetPointID() );

	return str;
}

double CFlight::GetTurnpointRadius(int i)
{
	return m_adTurnpointRadii[i];
}

void CFlight::SetTurnpointRadius(int i, double d)
	{
	m_adTurnpointRadii[i]=d;
	}

CString CFlight::GetTurnAreaText(TURNPOINTCLASSARRAY *pTURNPOINTCLASSArray)
	{
	CString strOut;
		strOut+=_T("\r\n");
		strOut+=_T("Maximum flight fixes:");
		strOut+=_T("\r\n");

		void *pcPoints[ALLOCTASKTPS+2];
		CTurnpoint *pcSoughtPoint=NULL;
		for( int j=0; j<ALLOCTASKTPS+2; j++ ) pcPoints[j]=NULL;

		// Calculate from start to first turn
		if( m_cStartGate.IsFirstLegAtExit() )
		   pcPoints[0]						  = &m_cStartFix;
		else
		   pcPoints[0]						  = pTURNPOINTCLASSArray->GetAt(m_cStartGate.GetPointID()-1);


		if( m_bValidFinish )
			pcPoints[m_nAcheivedTurnpoints+1] = pTURNPOINTCLASSArray->GetAt(m_cFinishGate.GetPointID()-1);
		else
			{
			int iSoughtTurnpoint=GetSoughtTurnpoint();
			pcPoints[m_nAcheivedTurnpoints+1]=&m_cScoredLandingPointLocation;
			pcSoughtPoint= pTURNPOINTCLASSArray->GetAt(iSoughtTurnpoint-1);
			}

		for( int i=0; i<m_nAcheivedTurnpoints; i++ )
			pcPoints[i+1]=&m_acTurnAreaTurnpointLocations[i];


		for( int i=0; i<m_nAcheivedTurnpoints+1; i++)
			{
			double dDist=0;
			if( i==0 && m_cStartGate.IsGPSCylinder() && !m_cStartGate.IsFirstLegAtExit() )
				{
				dDist-=m_cStartGate.GetRadius();
				}

			if( i==m_nAcheivedTurnpoints && m_bValidFinish && m_cFinishGate.IsGPSCylinder() )
				{
				dDist-=m_cFinishGate.GetRadius();
				}
			CString cText;
			if( !pcPoints[i] || !pcPoints[i+1] ) continue;

			if( m_bValidFinish || i<m_nAcheivedTurnpoints || pcSoughtPoint==NULL )
				{
			    dDist+=((CLocation*)pcPoints[i])->DistanceTo((CLocation*)(pcPoints[i+1]),	SYSTEMUNITS);
				}
			else 
				{
//			    Need to compute distance from (here to next tpt) then (prev turnpoint to sought tpt), then subtract.
				double dDistLeg=((CLocation*)pcPoints[i])->DistanceTo(pcSoughtPoint,	SYSTEMUNITS);
			    double dDist1=((CLocation*)pcPoints[i+1])->DistanceTo(pcSoughtPoint,	SYSTEMUNITS);
				dDist=dDistLeg-dDist1;
				}

//----------------------------------------------
// Check removed for 2018 season, restored in 2020

			if( i==0 && m_cStartGate.IsGPSCylinder() && m_cStartGate.IsFirstLegAtExit() && pTURNPOINTCLASSArray->GetAt(m_cStartGate.GetPointID()-1) ) 
				{
		//		Check to make sure the 1st leg is not greater than the distance from the center of the
		//		start cylinder to the 1st turnpoint
			    double dCheckDistance=pTURNPOINTCLASSArray->GetAt(m_cStartGate.GetPointID()-1)->DistanceTo((CLocation*)(pcPoints[i+1]), SYSTEMUNITS);
				dDist=min(dCheckDistance,dDist);
				}
//----------------------------------------------

			cText.Format(_T("  Distance for Leg %d : %5.2lf %s,    from    %s    to    %s"),i+1, ConvertDistance(dDist, SYSTEMUNITS, m_eUnits), UnitsText(m_eUnits), ((CLocation*)pcPoints[i])->GetFormattedInfo(), ((CLocation*)pcPoints[i+1])->GetFormattedInfo() );
			strOut+=cText;
			strOut+=_T("\r\n");
			}
	return strOut;
	}


void CFlight::CalculateFlightAverages(	int  &iMinAltitude, 
										int  &iMaxAltitude, 
										bool &bNoAltitudeInfo,
										CArray<double,double> &dSpeedArray)
	{
	double		dLastSpeed	=0.0;
	double		dSpeed		=0.0;
	CPosition	*pcPos		=NULL;
	CPosition	*pcPrevPos	=NULL;
	int			nPoints		=GetNumPoints();
	int iSec=0;
	CMedian       cNoise;

	iMinAltitude=32000;
	iMaxAltitude=0;

	for( int iPoint=0; iPoint<nPoints; iPoint++ )
		{
		pcPrevPos=pcPos;
		pcPos=GetPosition(iPoint);
		if(pcPos==NULL) break;
		pcPos->m_iStatus=0;

		// Compute speeds and store in array
		if( iPoint>0 )
			{
			CTimeSpan cInc=pcPos->m_cTime-pcPrevPos->m_cTime;
			double dDist=pcPos->DistanceTo( pcPrevPos, SYSTEMUNITS );
			iSec=(int)cInc.GetTotalSeconds();
			if( iSec>0 )
				{
				dSpeed=dDist/iSec;
				dSpeed*=3600;
				}

			// Now filter the speed by limiting the acceleration to 4 knots per second.
			double dAccel=4*iSec;
			dAccel=min(40.,dAccel);  // but keep it reasonable between fixes.

			if ( dSpeed>dLastSpeed )
				{
				if( dSpeed-dLastSpeed > dAccel )
					dSpeed=dLastSpeed + dAccel;
				}
			else
				{
				if( dLastSpeed-dSpeed > dAccel )
					dSpeed=dLastSpeed - dAccel;
				}
			iMaxAltitude=max( iMaxAltitude, pcPos->m_iPressureAltitude );
			iMinAltitude=min( iMinAltitude, pcPos->m_iPressureAltitude );
			}
		else
			{
			iMaxAltitude=pcPos->m_iPressureAltitude;
			iMinAltitude=pcPos->m_iPressureAltitude;
			}

		dSpeedArray.Add( dSpeed );
		dLastSpeed=dSpeed;
		}
	bNoAltitudeInfo = iMaxAltitude==iMinAltitude;
	}

bool CFlight::NotAnalyzed()
	{
	return m_eStatus==eNotAnalyzed;
	}

int CFlight::GetTurnAreaFixID(int iTP)
	{
	return m_aiTurnAreaTurnpointFixIDs[iTP];
	}

void CFlight::SetTurnAreaFixID(int iTP, int iFix)
	{
	if( iTP<ALLOCTASKTPS )
		m_aiTurnAreaTurnpointFixIDs[iTP]=iFix;
	}

CLocation& CFlight::GetTurnAreaLocation(int iTP)
	{
	return m_acTurnAreaTurnpointLocations[iTP];
	}


bool CFlight::CheckAirspaceIncursions(TURNPOINTCLASSARRAY &TURNPOINTCLASSArray)
{
	BOOL bSUAProcessing = GetWinscoreInt(  SUADISPLAY, DEFAULT_SUADISPLAY );
	if( !bSUAProcessing ) return false;

	CPosition	*pcPos		=NULL;
	CPosition	*pcPrevPos	=NULL;
	int			nPoints		=GetNumPoints();
	CSUASubSection *pSub	=NULL;
	CSUASubSection *pPrevSub=NULL;
	CLocation	cLocJunk, cLoc;
	int nTotalIncursions=0;

	try {
		if( LoadSUA( TURNPOINTCLASSArray )==0 ) return false;
		}
	catch(...)
		{
		AfxMessageBox(_T("Unhandled exception in LoadSUA"));
		throw;
		}

	for( int iSection=0; iSection<GetSUAArray()->GetSize(); iSection++ )
		{
		CSUASection* pSUASection=GetSUAArray()->GetAt(iSection);

		int nIncursions=0;
		CLocation cPrevLocation;
		for( int iPoint=0; iPoint<nPoints; iPoint++ )
			{
			pcPrevPos=pcPos;
			pcPos=GetPosition(iPoint);
			if(iPoint==0 || pcPos==NULL || pcPrevPos==NULL ) continue;

			if( !pcPos->CheckStatus(FAN_ROLLING) ) continue;

			pPrevSub=NULL;
			for ( int iSub=0; iSub<pSUASection->m_aSubSections.GetSize(); iSub++ )
				{
				 pPrevSub=pSub;
				 pSub=(CSUASubSection*)pSUASection->m_aSubSections[iSub];
				 if( !pSub ) continue;


				if( /* pSub->m_iTop <pcPos->m_iCorrectedAltitude || */ //Allow violation even if above
					pSub->m_iBase>pcPos->m_iCorrectedAltitude	) continue; 

				if( pSub->m_eType==CSUASubSection::eCIRCLE )
					{
					if( pSub->m_cCenter.DistanceTo(pcPos, SYSTEMUNITS )<pSub->m_dRadius )
						nIncursions++;
					}
				else if( pSub->m_eType==CSUASubSection::ePOINT && iSub>0 )
					{
					if( CLocation::GetCrossingLocation( pSub->m_cLocation, cPrevLocation, *pcPos, *pcPrevPos, cLocJunk) )
						nIncursions++;
					}
				else if( (pSub->m_eType==CSUASubSection::eCLOCKWISE		||
						  pSub->m_eType==CSUASubSection::eANTICLOCKWISE) &&
						  pSub->m_cCenter.DistanceTo(pcPos, SYSTEMUNITS)<(pSub->m_dRadius+.5) ) //Make sure we are close before going through this nasty check.
					{
					double dOriginalBearing	=pSub->m_cCenter.CourseTo(cPrevLocation);
					double dFinalBearing	=pSub->m_cCenter.CourseTo(pSub->m_cLocation);
					double dBearing			=dOriginalBearing;

					if ( pSub->m_eType==CSUASubSection::eCLOCKWISE )
						{
						if( dFinalBearing<dOriginalBearing ) dFinalBearing+=360;
						}
					else
						{
						if( dFinalBearing>dOriginalBearing ) dOriginalBearing+=360;
						}

					while( (pSub->m_eType==CSUASubSection::eCLOCKWISE)?(dBearing<dFinalBearing):(dBearing>dFinalBearing) )
						{
						cLoc=pSub->m_cCenter;
						cLoc.Move( pSub->m_dRadius, dBearing*TORAD, GetLatCorrection(pSub->m_cCenter.GetLat()));

						
						if( CLocation::GetCrossingLocation(cPrevLocation, cLoc, *pcPos, *pcPrevPos, cLocJunk) )
							nIncursions++;

						cPrevLocation=cLoc;
						(pSub->m_eType==CSUASubSection::eCLOCKWISE)?(dBearing+=5):(dBearing-=5);
						}

					if( CLocation::GetCrossingLocation(cPrevLocation, cLoc, *pcPos, *pcPrevPos, cLocJunk) )
							nIncursions++;

					}
				cPrevLocation=pSub->m_cLocation;
				}
			}
		if( nIncursions>0 )
			{
			AddWarning(eAirspaceMajor,1000,_T("Airspace incursion at : ")+pSUASection->m_strTitle );
			}
		nTotalIncursions+=nIncursions;
		}
	return nTotalIncursions>0;
	}

CFlight::CFlight(CFlight *pcFlight):
	CIGCFile(pcFlight)
	{
	InitializeAnalysisParams();
	m_strCID				=pcFlight->m_strCID;
	m_eClass				=pcFlight->m_eClass;
	m_bAllTurnpointsAcheived=pcFlight->m_bAllTurnpointsAcheived;
	m_bValidFinish			=pcFlight->m_bValidFinish;
	m_bValidStart			=pcFlight->m_bValidStart;
	m_bLocked				=pcFlight->m_bLocked;
	m_iStartPenalty			=pcFlight->m_iStartPenalty;
	m_iOptions				=pcFlight->m_iOptions;
	m_cRollTime				=pcFlight->m_cRollTime;
	m_cStartTime			=pcFlight->m_cStartTime;
	m_cFinishTime			=pcFlight->m_cFinishTime;
	m_cLandingTime			=pcFlight->m_cLandingTime;
	m_eStatus				=pcFlight->m_eStatus;
	m_eTaskType				=pcFlight->m_eTaskType;
	m_strLandoutAirfield	=pcFlight->m_strLandoutAirfield;
	m_iLandoutAirfield		=pcFlight->m_iLandoutAirfield;
	m_cStartGate			=pcFlight->m_cStartGate;
	m_cFinishGate			=pcFlight->m_cFinishGate;
	m_dDistance				=pcFlight->m_dDistance;
	m_eUnits				=pcFlight->m_eUnits;
	m_iSoughtTurnpoint		=pcFlight->m_iSoughtTurnpoint;
	m_nMATTaskPoints		=pcFlight->m_nMATTaskPoints;

	m_nAcheivedTurnpoints	=pcFlight->m_nAcheivedTurnpoints;
	for( int i=0; i<ALLOCTASKTPS; i++ )
			m_aiAcheivedTurnpoints[i]	=pcFlight->m_aiAcheivedTurnpoints[i];


	m_nTaskPoints	=pcFlight->m_nTaskPoints;
	for( int i=0; i<ALLOCTASKTPS; i++ )
		{
		m_aiTaskTurnpointIDs[i]				=pcFlight->m_aiTaskTurnpointIDs[i];
		m_pcTaskTurnpoints[i]				=pcFlight->m_pcTaskTurnpoints[i];
		m_aiTurnAreaTurnpointFixIDs[i]		=pcFlight->m_aiTurnAreaTurnpointFixIDs[i];
		m_acTurnAreaTurnpointLocations[i]	=pcFlight->m_acTurnAreaTurnpointLocations[i];
		}

	for( int i=0; i<pcFlight->m_cWarningArray.GetCount(); i++ )
		{
		CString strText=pcFlight->m_cWarningArray[i]->GetText();
		m_cWarningArray.Add(new CWarning(strText));
		}

	m_cFinishLocation			  =pcFlight->m_cFinishLocation;
	m_cLandingLocation			  =pcFlight->m_cLandingLocation;
	m_cStartFix					  =pcFlight->m_cStartFix;
	m_cScoredLandingPointLocation =pcFlight->m_cScoredLandingPointLocation;
	m_pDoc=NULL;
	}


void CFlight::SetStartGateID(int iGateID)
	{
	m_cStartGate.SetPointID(iGateID);
	}

int CFlight::GetStartGateID()
{
	return 	m_cStartGate.GetPointID();
}

int CFlight::GetFinishGateID()
{
	return 	m_cFinishGate.GetPointID();
}

CString CFlight::GetDistanceText(bool bAddDot)
{
	if( m_eStatus==eNotAnalyzed )
		return _T("");
	else if( m_eStatus==eNoTaskSpecified )
		return _T("");
	else if( m_eStatus==eDidNotStart )
		return _T("");
	else if( m_eStatus==eNotEnteredInContest )
		return _T("");

	CString cText;
	cText.Format(_T("%5.2lf %s"), ConvertDistance(m_dDistance, SYSTEMUNITS, m_eUnits), UnitsText(m_eUnits) );
	if( CheckOption(FLT_DISTANCELOCKED) && bAddDot ) cText+=" *";		
	return cText;

}

CString CFlight::GetAirfieldBonusText(bool bAddDot)
{
	if( m_eStatus==eNotAnalyzed )
		return _T("");
	else if( m_eStatus==eNoTaskSpecified )
		return _T("");
	else if( m_eStatus==eDidNotStart )
		return _T("");
	else if( m_eStatus==eNotEnteredInContest )
		return _T("");

	if(IsAirfieldBonus() )
		{
		CString strOut= _T("Yes");
		if( CheckOption( FLT_AIRFIELDBONUSLOCKED ) && bAddDot ) strOut+=" *";
		return strOut;
		}
	else
		return _T("");

}

void CFlight::CheckSecurity()
	{
	if( m_pDoc && ((CWinscoreDoc*)m_pDoc)->PassedSecurity(m_strFileName) )
		SetOption(FLT_SECURITYPASSED);
	else
		{
		CString strDLL;
	    HMODULE mod = NULL;

		//First try the later version of the file, the 2 version.  If there, use it.
		strDLL.Format(_T("igc-%s2.dll"),m_strManufacturer);
	    mod = LoadLibrary(strDLL);
		if( mod == NULL )
			{
		    strDLL.Format(_T("igc-%s.dll"),m_strManufacturer);
			mod = LoadLibrary(strDLL);
			}

	    mod = LoadLibrary(strDLL);
		if( mod!= NULL )
			{
			FreeLibrary(mod);
			CString strReason;
			if( ValidateIGCThread( strDLL, GetNonRelativePath(m_strFilePath), strReason ) )
				{
				if( m_pDoc ) ((CWinscoreDoc*)m_pDoc)->LogPassed(m_strFileName);
				SetOption(FLT_SECURITYPASSED);
				}
			else
				{
				CString strWarn;
				strWarn.Format(_T("SECURITY FAILURE - %s"),strReason );
				AddWarning(eSecurity,0,strWarn);
				if( m_pDoc ) ((CWinscoreDoc*)m_pDoc)->LogFailed(m_strFileName);
				SetOption(FLT_SECURITYFAILED);
				}
			}
		else
			{
			CString strWarn;
			strWarn.Format(_T("SECURITY NOT CHECKED - No security dll found for this manufacturer."));
			AddWarning(eSecurity,0,strWarn);
			if( m_pDoc ) ((CWinscoreDoc*)m_pDoc)->LogFailed(m_strFileName);
			SetOption(FLT_SECURITYFAILED);
			}
		}
	}

bool CFlight::CheckSecurity2(CString &cResult)
	{

		CString strDLL;
		HMODULE mod=NULL;
		//First try the later version of the file, the 2 version.  If there, use it.
		strDLL.Format(_T("igc-%s2.dll"),m_strManufacturer);
	    mod = LoadLibrary(strDLL);
		if( mod == NULL )
			{
		strDLL.Format(_T("igc-%s.dll"),m_strManufacturer);
			mod = LoadLibrary(strDLL);
			}
		if( mod!= NULL )
			{
			FreeLibrary(mod);
			CString strReason;
			if( ValidateIGCThread( strDLL, GetNonRelativePath(m_strFilePath), strReason ) )
				{
				return true;
				}
			else
				{
				return false;
				}
			}
		else
			{
			cResult.Format(_T("SECURITY NOT CHECKED - No security dll found for this manufacturer.\n\n%s not found"),strDLL);
			return false;
			}
	return false;
	}


BOOL CFlight::CheckOption(int iOption)
	{
	return m_iOptions & iOption;
	}

void CFlight::SetOption(int iOption)
	{
	m_iOptions|=iOption;
	}

void CFlight::ClearOption(int iOption)
	{
	if( m_iOptions|=iOption ) m_iOptions^=iOption;
	}

void CFlight::SetTurnAreaLocation(int iLoc, CLocation &cLoc)

	{
	if( iLoc<ALLOCTASKTPS )
		m_acTurnAreaTurnpointLocations[iLoc]=cLoc;
	}

bool CFlight::RetardStartTime(int iStartPoint, CTimeSpan &cRetard )
	{
	if( CheckOption(FLT_STARTTIMELOCKED) || iStartPoint<=0 ) return false;

	CPosition *pcStartPos=GetPosition(iStartPoint);
	if( !pcStartPos ) return false;

	// Make sure THIS start is not an out the top (ott) start
//	if( pcStartPos->CheckStatus(FAN_OUT_TOP_START) ) return false;

	int iPreviousOTTStartPos=FindEvent( FAN_OUT_TOP_START, iStartPoint, BACKWARD );
	if( iPreviousOTTStartPos<=0 ) return false;

	CPosition *pcOTTStartPos=GetPosition(iPreviousOTTStartPos);
	if( !pcOTTStartPos ) return false;

	CPosition *pcOTTStartPrevPos=GetPosition(iPreviousOTTStartPos-1);
	if( !pcOTTStartPrevPos ) return false;

	// Only check back 10 minutes
	if( (pcStartPos->m_cTime-pcOTTStartPos->m_cTime) > 10*60 ) return false;
	cRetard=pcStartPos->m_cTime-pcOTTStartPos->m_cTime;

	// OK, now retard the start
	m_cStartTime=InterpolateStartAltitude( pcOTTStartPos, pcOTTStartPrevPos, m_cStartFix);

	pcOTTStartPos->AddStatus( FAN_LATEST_START );
	pcStartPos->RemoveStatus( FAN_LATEST_START );

	return true;
	}

void CFlight::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode)
	{
	if( m_strCID.GetLength()==0			|| 
		m_eStatus==eNotEnteredInContest ||
		!IsClassValid(m_eClass)				) return;

	MSXML2::IXMLDOMNodePtr pFlightNode=NULL;
	cMgr.CreateChild( pParentNode, pFlightNode, _T("Flight") );

	CIGCFile::GetXML(cMgr, pFlightNode);

	cMgr.CreateElement	( pFlightNode, _T("CID"), LPCSTR(m_strCID) );
	cMgr.CreateElementInt	( pFlightNode, _T("ClassID"), (WORD)m_eClass );
	cMgr.CreateElementIntC( pFlightNode, _T("AllTurnpointsAcheived"), (WORD)m_bAllTurnpointsAcheived);
	cMgr.CreateElementIntC( pFlightNode, _T("ValidFinish"), (WORD)m_bValidFinish);
	cMgr.CreateElementIntC( pFlightNode, _T("ValidStart"), (WORD)m_bValidStart);
	cMgr.CreateElementIntC( pFlightNode, _T("Locked"), (WORD)m_bLocked);
	cMgr.CreateElementIntC( pFlightNode, _T("StartPenalty"), m_iStartPenalty);
	cMgr.CreateElementIntC( pFlightNode, _T("Options"), m_iOptions);

	cMgr.CreateElementInt ( pFlightNode, _T("StartGateID"),m_cStartGate.GetPointID());

	GetXMLTime  ( cMgr, pFlightNode, _T("RollTime"), m_cRollTime, false) ;
	GetXMLTime  ( cMgr, pFlightNode, _T("StartTime"), m_cStartTime, false);
	GetXMLTime  ( cMgr, pFlightNode, _T("FinishTime"), m_cFinishTime, false);
	GetXMLTime  ( cMgr, pFlightNode, _T("LandingTime"), m_cLandingTime, false);
	cMgr.CreateElementIntC( pFlightNode, _T("Status"), (WORD)m_eStatus);
	cMgr.CreateElement ( pFlightNode, _T("LandingAirfieldName"), LPCSTR(m_strLandoutAirfield) );

	if( m_iLandoutAirfield>=0 )
		cMgr.CreateElementIntC( pFlightNode, _T("LandingAirfieldID"), m_iLandoutAirfield);

	cMgr.CreateElementIntC( pFlightNode, _T("NumAcheivedTurnpoints"), m_nAcheivedTurnpoints );


	if( m_nAcheivedTurnpoints>0 )
		{
		MSXML2::IXMLDOMNodePtr pChildNode;
		cMgr.CreateChild( pFlightNode, pChildNode, _T("AcheivedTurnpoints") );
		for( int i=0; i<m_nAcheivedTurnpoints; i++ )
			cMgr.CreateElementIntC( pChildNode, _T("ID"),  m_aiAcheivedTurnpoints[i] );
		}

	cMgr.CreateElementIntC( pFlightNode, _T("NumTaskPoints"), m_nTaskPoints);
	if( m_nTaskPoints>0 )
		{
		MSXML2::IXMLDOMNodePtr pChildNode;
		cMgr.CreateChild( pFlightNode, pChildNode, _T("TaskTurnpoints"));
		for( int i=0; i<MAXTASKTPS; i++ )
			cMgr.CreateElementIntC( pChildNode, _T("ID"),  m_aiTaskTurnpointIDs[i]);
		}

	if( NumberOfWarnings()>0 )
		{
		MSXML2::IXMLDOMNodePtr pChildNode;
		cMgr.CreateChild( pFlightNode, pChildNode, _T("Warnings"));
		for( int i=0; i<NumberOfWarnings(); i++ )
			{
			CWarning *pcWarn=GetWarning(i);
			if( !pcWarn ) continue;
			CString strWarning=pcWarn->GetText();
			cMgr.CreateElement ( pChildNode, _T("Warning"), LPCSTR(strWarning) );
			}
		}

	MSXML2::IXMLDOMNodePtr pChildNode;
	cMgr.CreateChild( pFlightNode, pChildNode, _T("StartFix"));
	m_cStartFix.GetXML( cMgr, pChildNode);

	if( !m_cLandingLocation.IsNull() )
		{
		MSXML2::IXMLDOMNodePtr pChildNode;
		cMgr.CreateChild( pFlightNode, pChildNode, _T("LandingLocation"));
		m_cLandingLocation.GetXML( cMgr, pChildNode);
		}
	if( !m_cLandingLocation.IsNull() )
		{
		MSXML2::IXMLDOMNodePtr pChildNode;
		cMgr.CreateChild( pFlightNode, pChildNode, _T("ScoredLandingLocation"));
		m_cScoredLandingPointLocation.GetXML(cMgr,pChildNode);
		}

	cMgr.CreateElementIntC( pFlightNode, _T("NumMATTaskPoints"),	m_nMATTaskPoints);

	if( m_iSoughtTurnpoint>0 ) 
		cMgr.CreateElementInt	( pFlightNode, _T("SoughtTurnpoint"),	m_iSoughtTurnpoint);

	cMgr.CreateElementDblC	( pFlightNode, _T("Distance"),			m_dDistance);

	if( m_nTaskPoints>0 )
		{
		MSXML2::IXMLDOMNodePtr pChildNode;
		cMgr.CreateChild( pFlightNode, pChildNode, _T("TATaskPoints"));
		for( int i=0; i<m_nTaskPoints; i++ )
			{
			MSXML2::IXMLDOMNodePtr pChildNode2;
			cMgr.CreateChild(  pChildNode, pChildNode2, _T("TATaskPoint"));
			cMgr.CreateElementInt	( pChildNode2, _T("FixID"), m_aiTurnAreaTurnpointFixIDs[i]);
			m_acTurnAreaTurnpointLocations[i].GetXML(cMgr, pChildNode2);
			}	
		}
	}

CFlight::CFlight(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pFlightNode, CTaskList &cTaskList)
	{
	InitializeAnalysisParams();

	CIGCFile::ImportXML(cMgr, pFlightNode);

	GET_XML_STR	( cMgr, pFlightNode, _T("CID"), (m_strCID) );
	ASSERT(m_strCID.GetLength()>0);

	GET_XML_INT	( cMgr, pFlightNode, _T("ClassID"),		EClass, m_eClass, eStandard );
	ASSERT(IsClassValid(m_eClass));

	CTask* pcTask=cTaskList.GetByDateClass(CTime(m_iYear, m_iMonth, m_iDay, 0, 0, 0 ), m_eClass);
//	ASSERT(pcTask!=NULL);
	if( pcTask )
		{
		m_cStartGate=pcTask->m_cStartGate;
		m_cFinishGate=pcTask->m_cFinishGate;
		m_eTaskType=pcTask->m_eType;
		}
	int iGateID;
	GET_XML_INT ( cMgr, pFlightNode, _T("StartGateID"),	int, iGateID, 0);
//	ASSERT(iGateID>0 );
	if( iGateID!=0 ) m_cStartGate.SetPointID(iGateID);

	GET_XML_BOOL( cMgr, pFlightNode, _T("AllTurnpointsAcheived"), m_bAllTurnpointsAcheived, false);
	GET_XML_BOOL( cMgr, pFlightNode, _T("ValidFinish"),	m_bValidFinish, false);
	GET_XML_BOOL( cMgr, pFlightNode, _T("ValidStart"),	m_bValidStart,	false);
	GET_XML_BOOL( cMgr, pFlightNode, _T("Locked"),		m_bLocked,		false);
	GET_XML_INT ( cMgr, pFlightNode, _T("StartPenalty"),	int, m_iStartPenalty,	0);
	GET_XML_INT ( cMgr, pFlightNode, _T("Options"),		int, m_iOptions,		0);

	ImportXMLTime	( cMgr, pFlightNode, _T("RollTime"), m_cRollTime, m_iYear, m_iMonth, m_iDay) ;
	ImportXMLTime	( cMgr, pFlightNode, _T("StartTime"), m_cStartTime, m_iYear, m_iMonth, m_iDay);
	ImportXMLTime	( cMgr, pFlightNode, _T("FinishTime"), m_cFinishTime, m_iYear, m_iMonth, m_iDay);
	ImportXMLTime	( cMgr, pFlightNode, _T("LandingTime"), m_cLandingTime, m_iYear, m_iMonth, m_iDay);
	GET_XML_INT		( cMgr, pFlightNode, _T("Status"), EFlightStatus, m_eStatus, eNotAnalyzed);
	GET_XML_STR		( cMgr, pFlightNode, _T("LandingAirfieldName"), m_strLandoutAirfield );

	GET_XML_INT ( cMgr, pFlightNode, _T("LandingAirfieldID"), int, m_iLandoutAirfield, -1);
	GET_XML_INT ( cMgr, pFlightNode, _T("NumAcheivedTurnpoints"), int, m_nAcheivedTurnpoints,0 );
	if( m_nAcheivedTurnpoints>0 )
		{
		MSXML2::IXMLDOMNodePtr pTurnpoints;
		TESTHR(cMgr.SelectChild(_T("AcheivedTurnpoints"), pFlightNode, pTurnpoints));
		if( pTurnpoints!=NULL ) 
			{
			MSXML2::IXMLDOMNodeListPtr pTurnpointsList=cMgr.GetChildList(pTurnpoints);
			if( pTurnpointsList!=NULL ) 
				{
				long length=0;
				TESTHR(pTurnpointsList->get_length(&length));
				for( int i=0; i<length; i++ )
					{
					MSXML2::IXMLDOMNodePtr pTurnpoint;
					TESTHR(pTurnpointsList->get_item(i,&pTurnpoint));
					BSTR bstr;
					TESTHR(pTurnpoint->get_text(&bstr));
					CString str=bstr;
					str.GetLength()>0? m_aiAcheivedTurnpoints[i]=(int)atoi(str):m_aiAcheivedTurnpoints[i]=0;
					}
				}
			}
		}

	GET_XML_INT ( cMgr, pFlightNode, _T("NumTaskPoints"), int, m_nTaskPoints,0 );
	if( m_nTaskPoints>0 )
		{
		MSXML2::IXMLDOMNodePtr pTurnpoints;
		TESTHR(cMgr.SelectChild(_T("TaskTurnpoints"), pFlightNode, pTurnpoints));
		if( pTurnpoints!=NULL ) 
			{
			MSXML2::IXMLDOMNodeListPtr pTurnpointsList=cMgr.SelectNodeList(_T("ID"),pTurnpoints);
			if( pTurnpointsList!=NULL ) 
				{
				long length=0;
				TESTHR(pTurnpointsList->get_length(&length));
				for( int i=0; i<length; i++ )
					{
					MSXML2::IXMLDOMNodePtr pTurnpoint;
					TESTHR(pTurnpointsList->get_item(i,&pTurnpoint));
					BSTR bstr;
					TESTHR(pTurnpoint->get_text(&bstr));
					CString str=bstr;
					str.GetLength()>0? m_aiTaskTurnpointIDs[i]=(int)atoi(str):m_aiTaskTurnpointIDs[i]=0;
					}
				}
			}
		}

	MSXML2::IXMLDOMNodePtr pWarnings;
	TESTHR(cMgr.SelectChild(_T("Warnings"),pFlightNode,pWarnings));
	if( pWarnings!=NULL ) 
		{
		MSXML2::IXMLDOMNodeListPtr pWarningsList=cMgr.SelectNodeList(_T("Warning"), pWarnings);
		if( pWarningsList!=NULL ) 
			{
			long length=0;
			TESTHR(pWarningsList->get_length(&length));
			for( int i=0; i<length; i++ )
				{
				MSXML2::IXMLDOMNodePtr pWarning;
				TESTHR(pWarningsList->get_item(i,&pWarning));
				BSTR bstr;
				TESTHR(pWarning->get_text(&bstr));
				CString strWarn=bstr;
				m_cWarningArray.Add(new CWarning(strWarn));
				}
			}

		}

	m_cStartFix.ImportXML(cMgr, pFlightNode,_T("StartFix"));

	m_cLandingLocation.ImportXML(cMgr, pFlightNode,_T("LandingLocation"));
	m_cScoredLandingPointLocation.ImportXML(cMgr, pFlightNode,_T("ScoredLandingLocation"));

	GET_XML_INT ( cMgr, pFlightNode, _T("NumMATTaskPoints"),int,	m_nMATTaskPoints,0);
	GET_XML_INT	( cMgr, pFlightNode, _T("SoughtTurnpoint"), int,	m_iSoughtTurnpoint, -1);
	GET_XML_DBL	( cMgr, pFlightNode, _T("Distance"),		  double,	m_dDistance, 0);

	if( m_nTaskPoints>0 )
		{
		MSXML2::IXMLDOMNodePtr pTurnpoints;
		TESTHR(cMgr.SelectChild(_T("TATaskPoints"),pFlightNode,pTurnpoints));
		if( pTurnpoints!=NULL ) 
			{
			MSXML2::IXMLDOMNodeListPtr pTurnpointsList=cMgr.SelectNodeList(_T("TATaskPoint"),pTurnpoints );
			if( pTurnpointsList!=NULL ) 
				{
				long length=0;
				TESTHR(pTurnpointsList->get_length(&length));
				for( int i=0; i<length; i++ )
					{
					MSXML2::IXMLDOMNodePtr pTurnpoint;
					TESTHR(pTurnpointsList->get_item(i,&pTurnpoint));
					GET_XML_INT	( cMgr, pTurnpoint, _T("FixID"), int, m_aiTurnAreaTurnpointFixIDs[i], 0);
					m_acTurnAreaTurnpointLocations[i].FromXML(cMgr,pTurnpoint);
					}
				}
			}
		}
	}

CTimeSpan CFlight::TimeRemainingInLog(int iPoint)
	{
	CPosition*	pcPos=GetPosition(iPoint);
	CPosition*	pcEndPos=GetPosition(GetNumPoints()-1);

	if( !pcPos || !pcEndPos ) return 0;

	return CTimeSpan(pcEndPos->m_cTime - pcPos->m_cTime );
	}


void CFlight::CheckSafetyFinish()
	{
	if( !CheckOption(FLT_SAFETYFINISH ) ) return;


	int iFinishPoint=FindEvent( FAN_FINISH, 
			 		  		    GetNumPoints()-1,
								BACKWARD );
	if( iFinishPoint<=0 ) 
		{
		AddWarning(eFinish,0,_T("INVALID SAFETY FINISH - Could not find fix in safety finish area.  Contestant probably below the 200ft per mile rule.") );
		m_cFinishTime=m_cLandingTime;
		return;
		}
	
	if( CheckOption(FLT_FINISHTIMELOCKED) ) return;
		

	CPosition*	pcFinPos=GetPosition(iFinishPoint);
	if( !pcFinPos ) return;

	double d=pcFinPos->DistanceTo(m_pcFinishControlPoint,eStatute);
	int iSec=(int)(200*(d/5.0));

	m_cFinishTime=pcFinPos->m_cTime+CTimeSpan(0,0,0,iSec);

	}

void CFlight::FindStartFix()
	{

	if( !m_cStartGate.IsFirstLegAtExit() ) return;
/*
OK, here is the procedure.  Since the scorer can edit the start time, lets find
the startfix based upon the start time.

  1) Scan the log and find the fixes before and after the start time.
  2) Interpolate a lat and long for the startfix

*/
	CPosition*	pcPrevPos	=NULL;
	CPosition*	pcPos		=NULL;
	int			nPoints		=GetNumPoints();
	int iPoint;

	for( iPoint=0; iPoint<nPoints; iPoint++ )
		{
		pcPrevPos=pcPos;
		pcPos=GetPosition(iPoint);
		if( pcPrevPos &&
			pcPos	  &&
			pcPrevPos->m_cTime<=m_cStartTime  &&
			pcPos->m_cTime    >=m_cStartTime	   ) break;
		}
	
    CTimeSpan cDelta=pcPos->m_cTime-pcPrevPos->m_cTime;
	double dPercent=((float)CTimeSpan(m_cStartTime-pcPrevPos->m_cTime).GetTotalSeconds())/(float)cDelta.GetTotalSeconds();

	double dNewLat=pcPrevPos->GetLat()+(pcPos->GetLat()-pcPrevPos->GetLat())*dPercent;
	double dNewLon=pcPrevPos->GetLong()+(pcPos->GetLong()-pcPrevPos->GetLong())*dPercent;
	m_cStartFix=CLocation(dNewLat,dNewLon);

	}

CTurnpoint* CFlight::GetHomePoint()
{
return m_pcHomePoint;
}


void CFlight::ClearStartWarning()
	{
	for( int iWarn=0; iWarn<NumberOfWarnings(); iWarn++ )
		{
		if( m_cWarningArray[iWarn]->GetType()==eStartLatest )
			{
			m_cWarningArray[iWarn]->Clear();
			break;
			}
		}
	}

bool CFlight::HasStartWarning()
	{
	for( int iWarn=0; iWarn<NumberOfWarnings(); iWarn++ )
		{
		if( m_cWarningArray[iWarn]->GetType()==eStartLatest				) return true; 
		if( m_cWarningArray[iWarn]->GetType()==eStart					) return true; 
		if( m_cWarningArray[iWarn]->GetText().Find("start penalty")>0	) return true; 
		}
	return false;
	}

bool CFlight::MotorRunLandoutWarning()
	{
	for( int iWarn=0; iWarn<NumberOfWarnings(); iWarn++ )
		{
		if( m_cWarningArray[iWarn]->GetType()==eMotorRunLandout	) return true; 
		}
	return false;
	}


void CFlight::ResetAllWarnings()
	{
	for( int iWarn=0; iWarn<NumberOfWarnings(); iWarn++ )
		{
		delete m_cWarningArray[iWarn];
		}
	m_cWarningArray.RemoveAll();
	}
 
void CFlight::RemoveWarning(int iWarn)
	{
	if( iWarn<0 ) return;
	if( m_cWarningArray.GetCount()==0 ) return;
	delete m_cWarningArray[iWarn];
	m_cWarningArray.RemoveAt(iWarn);
	}

void CFlight::AddWarning(enum EWarningType eType,int iPenalty, CString strWarn)
	{
	m_cWarningArray.Add(new CWarning(eType,iPenalty,strWarn));
	}

void CFlight::GetWarningsArray(CStringArray &strArray)
	{
	for( int iWarn=0; iWarn<NumberOfWarnings(); iWarn++ )
		{
		strArray.Add(m_cWarningArray[iWarn]->GetText());
		}
	}

void CFlight::SetWarningsArray(CStringArray &strArray)
	{
	for( int iWarn=0; iWarn<strArray.GetCount(); iWarn++ )
		{
		m_cWarningArray.Add(new CWarning(strArray[iWarn]));
		}
	}



void CFlight::FindBetterStart(TASKCLASS *pcTask, TURNPOINTCLASSARRAY &turnpointarray )
	{
	if( CheckOption(FLT_STARTTIMELOCKED) ) return;

	// If this is a FAI task and a start line, just take the latest start always.
	if( pcTask->IsFAITask() && !pcTask->m_cStartGate.IsGPSCylinder() ) return;

	int iControlHeight=0;
	int MSH=0;
	CTimeSpan cSTOC;
	m_cScoredLandingPointLocation=m_cLandingLocation;

	int iLatestStart=FindEvent(FAN_LATEST_START, 0, FORWARD );
	if( iLatestStart<0 ) return;

	int iCandidateStart=iLatestStart;

	// If there is a start penalty already, don't look for a better start here.
	int iStartHeightPenalty=GetStartHeightPenalty(iLatestStart, iControlHeight, MSH );
	if( iStartHeightPenalty>0 ) return;

	CPosition *pcLatestStartPos=GetPosition(iLatestStart);	
	if( !pcLatestStartPos ) return;

	// If the latest start is before the task open time, don't look for a better start.
	if( pcTask->m_cGateOpenTime>pcLatestStartPos->m_cTime ) return;
	CTimeSpan cMINTIME=pcTask->m_cPostTime;

	while( 1) 
		{
		double dSpeedNew=0.0;
		double dSpeedOriginal=0.0;

		double dDistanceOriginal=ComputeDistance(pcTask, turnpointarray);
		cSTOC=(m_cFinishTime-m_cStartTime);
		if( (pcTask->IsTimedTask()) && (cSTOC < cMINTIME)  )
			{
			//STOC = MINTIME - (MINTIME - TOC) / 10
			cSTOC=cMINTIME.GetTotalSeconds() -int(((double)cMINTIME.GetTotalSeconds()-(double)cSTOC.GetTotalSeconds() )/10.);
			}

		int nSeconds=int(cSTOC.GetTotalSeconds());
		if( nSeconds>0 ) dSpeedOriginal=dDistanceOriginal/double(nSeconds);

		iCandidateStart=FindEvent(FAN_START, iCandidateStart-1, BACKWARD );
		if( iCandidateStart<0 ) break;

		// If the new candidate start has a penalty, try the next start.
		iStartHeightPenalty=GetStartHeightPenalty(iCandidateStart, iControlHeight, MSH );
		if( iStartHeightPenalty>0 ) 
			{
			continue;
			}

		CPosition *pcCandidateStart=GetPosition(iCandidateStart);	
		if( !pcCandidateStart ) break; 

		// If the candidate start is before the task open time we are done, exit.
		if( pcTask->m_cGateOpenTime>pcCandidateStart->m_cTime ) break;

		// Now subvert the flight and see if the speed is better.
		CTime		cStartTimeOriginal=m_cStartTime;
		CLocation	cStartFixOriginal=m_cStartFix;

		m_cStartTime=pcCandidateStart->m_cTime;
		m_cStartFix=*pcCandidateStart;

		double dDistanceNew=ComputeDistance(pcTask, turnpointarray);
		cSTOC=(m_cFinishTime-m_cStartTime);
		if( (pcTask->IsTimedTask() ) && (cSTOC < cMINTIME)  )
			{
			//STOC = MINTIME - (MINTIME - TOC) / 10
			cSTOC=cMINTIME.GetTotalSeconds() -int(((double)cMINTIME.GetTotalSeconds()-(double)cSTOC.GetTotalSeconds() )/10.);
			}

		nSeconds=int(cSTOC.GetTotalSeconds());
		if( nSeconds>0 ) dSpeedNew=dDistanceNew/double(nSeconds);

		if(  (IsTaskComplete() && (dSpeedNew>dSpeedOriginal)) ||
			(!IsTaskComplete() && (dDistanceNew>dDistanceOriginal)) )
			{
			// Get interpolated start time and position
			CPosition *pcPrevStartPrev=GetPosition(iCandidateStart-1);	
			if( pcCandidateStart->CheckStatus(	FAN_IN_START_CYL) &&
				pcPrevStartPrev->CheckStatus(	FAN_IN_START_CYL) )
				{
				m_cStartTime=InterpolateStartAltitude(pcCandidateStart, pcPrevStartPrev, m_cStartFix );
				}
			else
				{
				m_cStartTime=InterpolateStartPoints(pcCandidateStart, pcPrevStartPrev, m_cStartFix );
				}
			pcLatestStartPos->RemoveStatus(FAN_LATEST_START);
			pcCandidateStart->AddStatus( FAN_LATEST_START );
			}
		else
			{
			// No better, reset
			m_cStartTime=cStartTimeOriginal;
			m_cStartFix=cStartFixOriginal;
			}
		}
	}