//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Task.cpp: implementation of the CTask class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "Task.h"
#include "gate.h"
#include "wsclass.h"
#include "WinscoreFonts.h"

#include "global_prototypes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define CURRENTSCHEMA 7

#define PAGE_WIDTH    800


static void	GetMinMaxPoint( double dRadius, CLocation* pcPrev, CLocation* pcTurnpoint, CLocation* pcNext, CLocation &cMaxLoc, CLocation &cMinLoc );

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CTask, CTask, VERSIONABLE_SCHEMA| CURRENTSCHEMA)

CTask::CTask()
	{
	Initialize();
	}

CTask::~CTask()
{
}

CTask::CTask(CTask *pcTask)
{
	*this= *pcTask;
}

double CTask::GetDistance(CTurnpointArray& cTurnpointArray, EUnits eUnits)
	{
	//if( m_eType==ePost ) return 0.00;

	double dDist=::GetHandicappedDistance(  m_eType,
											m_cStartGate,
		                                    NULL,
											cTurnpointArray,
											m_cFinishGate,
											1.0,
											m_nTurnpoints,
											m_aiTurnpointIDs,
											NULL,					// dummy value, shoul never be used
											true,
											1,						// dummy value, shoul never be used
											*(cTurnpointArray[0])); // dummy value, shoul never be used

	return ConvertDistance(dDist, SYSTEMUNITS, eUnits);
	}

CString CTask::GetStatusText()
{
 
  return GetStatusText(m_eStatus);
}

CString CTask::GetStatusText(EStatus eStatus)
{
  if( eStatus==eNoContest )
	  return _T("No_Contest_Day");
  else if( eStatus==eOfficial )
	  return _T("Official");
  else if( eStatus==eUnofficial )
	  return _T("Unofficial");
    else if( eStatus==ePreliminary )
	  return _T("Preliminary");
   else if( eStatus==eFinal )
	  return _T("Final");
  return _T(" ");
}




int CTask::GetAssignedTurnpoints(CTurnpointArray& cTurnpointArray, CString acStrings[])
{
//	if( m_eType==ePost  ) return 0;
	int j=0;
	for( int i=0; i<m_nTurnpoints; i++ )
		{
		CTurnpoint* pcTurnpoint=cTurnpointArray.GetAt(m_aiTurnpointIDs[i]-1);
		ASSERT( pcTurnpoint );
		if( !pcTurnpoint ) continue;
		acStrings[j++]=pcTurnpoint->GetNoNameText();
		}
	return m_nTurnpoints;
}

    CString CTask::DateText()
		{
		return m_cDate.Format(_T("%b %d, %y"));     // , %H, %M %S");
		}


    CString CTask::TypeText()
		{
		if( m_eType==eAssigned )
			return _T("Assigned");
		if( m_eType==eTurnArea )
			return _T("Turn Area");
		else if( m_eType==eFAIRacing )
			return "FAI Racing";
		else if( m_eType==eFAIAssignedArea )
			return "FAI Assigned Area";
		else if( m_eType==eModifiedAssigned )
			return _T("Mod. Assigned");
		else
			return _T("");
		}
  
    CString CTask::MinTimeText()
		{
		if( m_eType==eAssigned || m_eType==eFAIRacing )
			return _T("-");
		else
			return m_cPostTime.Format(_T("%H:%M"));
		}
    

      
	CString CTask::GPSStartText(EUnits eUnits)
		{
		CString str;
		if( GPSStartCylinder() )
			str.Format(_T("%2.3lf"), ConvertDistance( m_cStartGate.GetRadius(), SYSTEMUNITS, eUnits ) );
		else
			str=_T("-");
		return str;
		}
    
	CString CTask::GPSFinishText(EUnits eUnits)
		{
		CString str;
		if( GPSFinishCylinder() )
			str.Format(_T("%2.3lf"), ConvertDistance( m_cFinishGate.GetRadius(), SYSTEMUNITS, eUnits ) );
		else
			str=_T("-");
		return str;
		}

CString CTask::TaskOpenText()
	{
	return m_cGateOpenTime.Format(_T("%H:%M"));
	}

CString CTask::DistanceText(CTurnpointArray& cTurnpointArray, EUnits eUnit)
	{
	CString str;
	if( m_eType==eAssigned || m_eType==eModifiedAssigned || IsAreaTask() || m_eType==eFAIRacing )
		if( m_b2ndGateActive )
			{
			double dDist1=::GetHandicappedDistance( m_eType,
													m_cStartGate,
				                                    NULL,
													cTurnpointArray,
													m_cFinishGate,
													1.0,
													m_nTurnpoints,
													m_aiTurnpointIDs,
													NULL,
													true,
													1,						// dummy value, shoul never be used
													*(cTurnpointArray[0])); // dummy value, shoul never be used
			double dDist2=::GetHandicappedDistance( m_eType,
													m_cStartGate2,
				                                    NULL,
													cTurnpointArray,
													m_cFinishGate,
													1.0,
													m_nTurnpoints,
													m_aiTurnpointIDs,
													NULL,
													true,
													1,						// dummy value, shoul never be used
													*(cTurnpointArray[0])); // dummy value, shoul never be used
	
			str.Format(_T("%4.2lf, %4.2lf"), ConvertDistance(dDist1, SYSTEMUNITS, eUnit),  ConvertDistance(dDist2, SYSTEMUNITS, eUnit) );

			}
		else
			str.Format(_T("%4.2lf"), GetDistance(cTurnpointArray,eUnit) );
	else
		str=_T("-");
	return str;
	}


CString CTask::TurnpointText()
{
	CString str, str1;
	if( m_eType==eAssigned || m_eType==eModifiedAssigned || IsAreaTask() || m_eType==eFAIRacing )
		{
		for( int i=0; i<m_nTurnpoints; i++ )
			{
			str1.Format(_T("%d"),m_aiTurnpointIDs[i]);
			str+=str1;
			if( i<m_nTurnpoints-1 ) str+=_T(",");
			}
		}
	else
		str=_T("-");
	return str;
}



int CALLBACK CompareTasks(LPARAM lParam1, LPARAM lParam2, 
    LPARAM iColumn)
{
	int iR=0;
	switch( abs(iColumn)-1 )
		{
	case 0:
		if( ((CTask*)lParam1)->m_cDate <
			((CTask*)lParam2)->m_cDate  ) iR= -1;
		if( ((CTask*)lParam1)->m_cDate >
			((CTask*)lParam2)->m_cDate  ) iR=  1;
		break;
	case 1:		
		if( ((CTask*)lParam1)->m_eClass <
			((CTask*)lParam2)->m_eClass  ) iR= -1;
		if( ((CTask*)lParam1)->m_eClass >
			((CTask*)lParam2)->m_eClass  ) iR=  1;
		break;
	default:
		iR= 0;
		}

	return (iColumn<0)?(-iR):(iR);
}


CString CTask::GetStartCeilingText()
	{
		CString str;
		str.Format(_T("%d"), m_cStartGate.GetHeight() );
		return str;
	}

CString CTask::GetFinishBaseText()
{
		CString str;
		str.Format(_T("%d"), m_cFinishGate.GetBase() );
		return str;

}

BOOL CTask::GPSStartCylinder()
{
	return m_cStartGate.IsGPSCylinder();
}

BOOL CTask::GPSFinishCylinder()
{
	return m_cFinishGate.IsGPSCylinder();

}

void CTask::AddToControl(CTurnpointArray &turnpointArray, CWSView &cWSView, int iGateFlag, CGate *pcStartGate, bool bGatesOnly)
{
		CTurnpoint* pcTurnpoint = NULL;
		CGate cStartGate;

		if( iGateFlag==3 && pcStartGate )
			cStartGate=*pcStartGate;
		else
			cStartGate=( iGateFlag==1 && m_b2ndGateActive)?(m_cStartGate2):(m_cStartGate);
	
		pcTurnpoint =(CTurnpoint*)turnpointArray.GetAt(cStartGate.GetPointID()-1);

		if( pcTurnpoint ) 
			cWSView.InitSite((float)pcTurnpoint->GetLat(), (float)pcTurnpoint->GetLong(), 0);

		if( pcTurnpoint )
			{
			if( cStartGate.GetGateType()==eCylinder)
				{
				cWSView.SetStartGateCylinder(		(float)pcTurnpoint->GetLat(),
													(float)pcTurnpoint->GetLong(), 
													(float)cStartGate.GetRadius(),
													cStartGate.GetHeight(),
													pcTurnpoint->m_iElevation);
				}
			else
				{
				if( cStartGate.IsPerpToCourse() ) 
					{
					TURNPOINTCLASS* p1st    =(TURNPOINTCLASS*)turnpointArray[m_aiTurnpointIDs[0]-1];
					if( pcTurnpoint && p1st )
						{
						CLocation cLoc1(pcTurnpoint->GetLat(), pcTurnpoint->GetLong());
						CLocation cLoc2(p1st->GetLat(), p1st->GetLong());
						cStartGate.SetHeading(cLoc1.CourseTo(cLoc2));
						}
					}
				float fLat1, fLong1, fLat2, fLong2;
				cStartGate.Initialize(  *(CTurnpoint*)turnpointArray.GetAt(cStartGate.GetPointID()-1)  );
				double dCorr=GetLatCorrection(pcTurnpoint->GetLat());
				cStartGate.GetEnds(fLat1, fLong1, fLat2, fLong2,dCorr);
				cWSView.SetStartGateLine( fLat1, fLong1, fLat2, fLong2,cStartGate.GetHeight());
				}
			}

		if(m_b2ndGateActive && iGateFlag==2 )
			{
			pcTurnpoint =(CTurnpoint*)turnpointArray.GetAt(m_cStartGate2.GetPointID()-1);

			if( pcTurnpoint )
				{
				cWSView.Set2ndStartGateCylinder(	(float)pcTurnpoint->GetLat(),
													(float)pcTurnpoint->GetLong(), 
													(float)m_cStartGate2.GetRadius(),
													m_cStartGate2.GetHeight(),
													pcTurnpoint->m_iElevation);
				}
			}


		pcTurnpoint = (CTurnpoint*)turnpointArray.GetAt(m_cFinishGate.GetPointID()-1);
		if( pcTurnpoint )
			{
			if( m_cFinishGate.GetGateType()==eCylinder)
				{
				cWSView.SetFinishGateCylinder(	(float)pcTurnpoint->GetLat(),
												(float)pcTurnpoint->GetLong(), 
												(float)m_cFinishGate.GetRadius(),
												pcTurnpoint->m_iElevation,
												pcTurnpoint->m_iElevation);
				}
			else
				{
				float fLat1, fLong1, fLat2, fLong2;
				m_cFinishGate.Initialize(  *(CTurnpoint*)turnpointArray.GetAt(m_cFinishGate.GetPointID()-1)  );
				m_cFinishGate.GetEnds(fLat1, fLong1, fLat2, fLong2);
				cWSView.SetFinishGateLine( fLat1, fLong1, fLat2, fLong2, m_cFinishGate.GetBase());
				}
			}

		if( m_eType==eAssigned || IsAreaTask() || m_eType==eFAIRacing )
			{
			// Add the task points
			cWSView.AddTaskPoint(cStartGate.GetPointID());
			for( int i=0; i<m_nTurnpoints; i++)
				{
				cWSView.AddTaskPoint(m_aiTurnpointIDs[i]);
				}
			cWSView.AddTaskPoint(m_cFinishGate.GetPointID());
			}
		else if( m_eType==eModifiedAssigned && !bGatesOnly )
			{
			// Add the task points
			cWSView.AddTaskPoint(cStartGate.GetPointID());
			for( int i=0; i<m_nTurnpoints; i++)
				{
				cWSView.AddTaskPoint(m_aiTurnpointIDs[i]);
				}
			}

	if(  IsAreaTask() )
		{
		for( int i=0; i<m_nTurnpoints; i++)
			{
			cWSView.SetTaskPointRadius(i+1, m_afTurnpointRadius[i]);
			}
		}

}

int CTask::GetCurrentSchema()
	{
	return CURRENTSCHEMA;
	}

int	CTask::GetNumLegs()
	{
    if( m_eType==eAssigned || IsAreaTask() || m_eType==eFAIRacing) 
		return m_nTurnpoints+1;
	else if( m_eType==eModifiedAssigned ) 
		return m_nTurnpoints;
	return 0;
	}

int CTask::GetLegStart(int iLeg)
	{
//	if( m_eType==ePost ) 
//		return 0;
//	else 
//		{
		if( iLeg==0 )
			return m_cStartGate.GetPointID();
		else
			return GetTurnpointID(iLeg-1);
//		}

	return 0;
	}

int CTask::GetLegFinish(int iLeg)
	{
//	if( m_eType==ePost ) 
//		return 0;
//	else 
//		{
		if( iLeg==GetNumLegs() )
			return m_cFinishGate.GetPointID();
		else
			return GetTurnpointID(iLeg);
//		}
	return 0;
	}

CTask& CTask::operator =(CTask &cTask)
	{

	m_eStatus=cTask.m_eStatus;
	m_eClass=cTask.m_eClass;
	m_cDate=cTask.m_cDate;
	m_eType=cTask.m_eType;

	m_cGateOpenTime = cTask.m_cGateOpenTime;
	m_cPostTime		= cTask.m_cPostTime;
	
	m_nTurnpoints=cTask.m_nTurnpoints;
	for( int i=0; i<MAXTASKTPS; i++)
		{
		m_aiTurnpointIDs[i]=cTask.m_aiTurnpointIDs[i];
		m_afTurnpointRadius[i]=cTask.m_afTurnpointRadius[i];
		}

	m_cFinishGate		=cTask.m_cFinishGate;
	m_cStartGate		=cTask.m_cStartGate;

	m_b2ndGateActive	=cTask.m_b2ndGateActive;
	m_cStartGate2		=cTask.m_cStartGate2;

	return *this;
	}

bool CTask::CheckTurnAreaDistances(	CTurnpointArray &cTurnpointArray, CString &strError,EUnits eUnits)
	{
	CLocation cPoints[MAXTASKTPS+2];
	double dRadii[MAXTASKTPS+2] = {0.0};
	CString strNames[MAXTASKTPS+2];

	if( m_nTurnpoints==0 ) return false;

	// Calculate from start to first turn
	CTurnpoint* pcStart = cTurnpointArray.GetAt(m_cStartGate.GetPointID()-1);
	CTurnpoint* pcFinish= cTurnpointArray.GetAt(m_cFinishGate.GetPointID()-1);
	if( !pcStart || !pcFinish ) return false;

	cPoints[0]				=*pcStart;
	cPoints[m_nTurnpoints+1]=*pcFinish;

	strNames[0]=pcStart->m_strName;
	strNames[m_nTurnpoints+1]=pcFinish->m_strName;

	if( m_cStartGate.IsGPSCylinder() )
		{
		dRadii[0]=m_cStartGate.GetRadius();
		}
	if( m_cFinishGate.IsGPSCylinder() )
		{
		dRadii[m_nTurnpoints+1]=m_cFinishGate.GetRadius();
		}
	else
		dRadii[m_nTurnpoints+1]=0.0;


	for( int i=0; i<m_nTurnpoints; i++ )
		{
		cPoints[i+1]=*cTurnpointArray.GetAt(m_aiTurnpointIDs[i]-1);
		dRadii[i+1]=m_afTurnpointRadius[i];
		strNames[i+1]=cTurnpointArray.GetAt(m_aiTurnpointIDs[i]-1)->m_strName;
		}


	// Check the legs
	for( int i=0; i<m_nTurnpoints+1; i++ )
		{
		double dGap=cPoints[i].DistanceTo(cPoints[i+1],SYSTEMUNITS);
		dGap-=dRadii[i+1];
		dGap-=dRadii[i];
		double dMiDist=ConvertDistance(fabs(dGap),  SYSTEMUNITS, eUnits);
		if(dGap<0.0)
			strError.Format(" %f %s overlap between ",dMiDist,UnitsText(eUnits));
		else
		    strError.Format(" %f %s gap between ",dMiDist,UnitsText(eUnits));
		strError+=strNames[i];
	    strError+=" and ";
		strError+=strNames[i+1];
		if( IsFAITask() )
			{
			if( dGap<ConvertDistance(1, eKilometers, SYSTEMUNITS) ) return false;
			}
		else
			{
			if( dGap<ConvertDistance(2, eStatute, SYSTEMUNITS) ) 
			  {
			  return false;
			  }
			}
		}

	return true;
	}



void CTask::GetTurnAreaDistances(	CTurnpointArray &cTurnpointArray, 
									double &dMin, 
									double &dNominal, 
									double &dMax)
	{
	dMin	=0;
	dMax	=0;
	dNominal=0;
	CLocation cMaxLoc[MAXTASKTPS+2];
	CLocation cMinLoc[MAXTASKTPS+2];
	CLocation cPoints[MAXTASKTPS+2];

	if( m_nTurnpoints==0 ) return;

	// Calculate from start to first turn
	CTurnpoint* pcStart = cTurnpointArray.GetAt(m_cStartGate.GetPointID()-1);
	CTurnpoint* pcFinish= cTurnpointArray.GetAt(m_cFinishGate.GetPointID()-1);
	if( !pcStart || !pcFinish ) return;

	cPoints[0]				=*pcStart;
	cPoints[m_nTurnpoints+1]=*pcFinish;
	for( int i=0; i<m_nTurnpoints; i++ )
		{
		CLocation *pLoc=cTurnpointArray.GetAt(m_aiTurnpointIDs[i]-1);
		if( !pLoc ) continue;
		cPoints[i+1]=*pLoc;
		}

	for( int i=0; i<m_nTurnpoints+2; i++ )
		{
		cMaxLoc[i]=cPoints[i];
		cMinLoc[i]=cPoints[i];
		}

	// Compute the fix point locations
	for( int i=0; i<m_nTurnpoints; i++ )
		{
		GetMinMaxPoint( m_afTurnpointRadius[i],  &cPoints[i], &cPoints[i+1], &cPoints[i+2], cMaxLoc[i+1], cMinLoc[i+1] );
		}

	// Sum the legs
	double dDist=0;
	for( int i=0; i<m_nTurnpoints+1; i++ )
		{
		dMin+=	cMinLoc[i].DistanceTo(cMinLoc[i+1],SYSTEMUNITS);
		dMax+=	cMaxLoc[i].DistanceTo(cMaxLoc[i+1],SYSTEMUNITS);
		dNominal+=	cPoints[i].DistanceTo(cPoints[i+1],SYSTEMUNITS);
		}

	if( m_cStartGate.IsGPSCylinder() )
		{
		dMin-=m_cStartGate.GetRadius();
		dMax-=m_cStartGate.GetRadius();
		dNominal-=m_cStartGate.GetRadius();
		}
	if( m_cFinishGate.IsGPSCylinder() )
		{
		dMin-=m_cFinishGate.GetRadius();
		dMax-=m_cFinishGate.GetRadius();
		dNominal-=m_cFinishGate.GetRadius();
		}
	}


static void	GetMinMaxPoint( double dRadius, CLocation* pcPrev, CLocation* pcTurnpoint, CLocation* pcNext, CLocation &cMaxLoc, CLocation &cMinLoc )
	{

	double dBearingPrev=pcTurnpoint->CourseTo(*pcPrev);
	double dBearingNext=pcTurnpoint->CourseTo(*pcNext);

	double dAverage=(dBearingPrev+dBearingNext)/2.;

	CLocation cEnd1(pcTurnpoint), cEnd2(pcTurnpoint);
	cEnd1.Move(dRadius, TORAD*(dAverage), GetLatCorrection(pcTurnpoint->GetLat()) );
	cEnd2.Move(dRadius, TORAD*(dAverage+180), GetLatCorrection(pcTurnpoint->GetLat()));

	double dDist1=cEnd1.DistanceTo(pcPrev, SYSTEMUNITS);
	double dDist2=cEnd2.DistanceTo(pcPrev, SYSTEMUNITS);

	if( dDist1>dDist2 )
		{
		cMaxLoc=cEnd1;
		cMinLoc=cEnd2;
		}
	else
		{
		cMaxLoc=cEnd2;
		cMinLoc=cEnd1;
		}
	}

CString CTask::StartGateText(CTurnpointArray& cTurnpointArray )
	{

	CTurnpoint *pcTurnpoint= cTurnpointArray.GetAt(m_cStartGate.GetPointID()-1);
	if( !pcTurnpoint ) return _T("");

	return pcTurnpoint->GetNoNameText();
	}

CString CTask::FinishGateText(CTurnpointArray& cTurnpointArray )
	{

	CTurnpoint *pcTurnpoint= cTurnpointArray.GetAt(m_cFinishGate.GetPointID()-1);
	if( !pcTurnpoint ) return _T("");

	return pcTurnpoint->GetNoNameText();
	}


CString CTask::StartGateBText(CTurnpointArray& cTurnpointArray )
	{
	if( !m_b2ndGateActive ) return _T("");

	CTurnpoint *pcTurnpoint= cTurnpointArray.GetAt(m_cStartGate2.GetPointID()-1);
	if( !pcTurnpoint ) return _T("");

	return pcTurnpoint->GetNoNameText();
	}

CGate& CTask::GetActiveStartGate(CContestant *pcContestant)
{
		BOOL bUseBGate=false;
		if( m_b2ndGateActive )
			{
			if( pcContestant )
				bUseBGate = pcContestant->IsInBGroup();
			}
		return (bUseBGate)?(m_cStartGate2):(m_cStartGate);
}


bool CTask::GetSSAXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode, EUnits eUnits, CTurnpointArray  &cTurnpointArray, bool bDateOutput)
{
    MSXML2::IXMLDOMNodePtr DOMContestantPtr;
	TCHAR  buff[10];

	MSXML2::IXMLDOMNodePtr pIDOMChildNode;
	cMgr.CreateChild(pParentNode, pIDOMChildNode, _T("Task") );

	if( bDateOutput )
		GetXMLDateFull(cMgr, pIDOMChildNode, _T("Date"), m_cDate);


	cMgr.CreateElement( pIDOMChildNode, _T("Status"),  GetStatusText());
	cMgr.CreateElement( pIDOMChildNode, _T("Type"),	TypeText() );

	cMgr.CreateElement( pIDOMChildNode, _T("TaskOpenTime"),	TaskOpenText() );
	cMgr.CreateElement( pIDOMChildNode, _T("MinimumTaskTime"),	MinTimeText());

	MSXML2::IXMLDOMNodePtr pStartGateNode;
	cMgr.CreateChild(pIDOMChildNode, pStartGateNode, _T("StartGate"));
	m_cStartGate.GetSSAXML(cMgr,pStartGateNode, eUnits, &cTurnpointArray,IsFAITask());

	if( IsFAITask() )
		{
		if( m_cStartGate.IsPreStartAltitude() )
			cMgr.CreateElementIntC( pIDOMChildNode, _T("PreStartAltitude"), m_cStartGate.GetPreStartAltitude());
		CString strSpeed;
		strSpeed.Format(_T("%4.1lf"), ConvertDistance(m_cStartGate.GetMaxSpeed(),SYSTEMUNITS, eUnits));
		cMgr.CreateElement ( pIDOMChildNode, _T("MaxGroundSpeed"), strSpeed);
		}

	if( m_b2ndGateActive )
		{
		MSXML2::IXMLDOMNodePtr pStartGateNode;
		cMgr.CreateChild(pIDOMChildNode, pStartGateNode, _T("StartGate2"));
		m_cStartGate.GetSSAXML(cMgr,pStartGateNode, eUnits, &cTurnpointArray,IsFAITask());
		}

	MSXML2::IXMLDOMNodePtr pTurnpoints;
	cMgr.CreateChild(pIDOMChildNode, pTurnpoints, _T("Turnpoints"));

	for(int i=0; i<m_nTurnpoints; i++)
		{
		MSXML2::IXMLDOMNodePtr pTurnpoint;
		cMgr.CreateChild(pTurnpoints, pTurnpoint, _T("Turnpoint"));
		 _itoa_s(m_aiTurnpointIDs[i],buff,10,10);
		cMgr.CreateElement( pTurnpoint, _T("ID"),buff);
		cMgr.CreateElement( pTurnpoint, _T("Name"), cTurnpointArray[m_aiTurnpointIDs[i]-1]->m_strName);

		CString strRadius;
		if( IsAreaTask()  )
   			strRadius.Format(_T("%4.1lf"), ConvertDistance(m_afTurnpointRadius[i],SYSTEMUNITS, eUnits));
		else
			{
			float fTurnpointRadius		= (float)GetWinscoreDouble(INNERRADIUS, ConvertDistance(DEFAULTINNERRADIUS, eStatute, SYSTEMUNITS) );
   			strRadius.Format(_T("%4.1lf"), fTurnpointRadius);
			}

		cMgr.CreateElement( pTurnpoint, _T("Radius"), strRadius );
		}

	MSXML2::IXMLDOMNodePtr pFinishGateNode;
	cMgr.CreateChild( pIDOMChildNode, pFinishGateNode, _T("FinishGate") );

	m_cFinishGate.GetSSAXML(cMgr, pFinishGateNode, eUnits, &cTurnpointArray,IsFAITask() );

	if( m_eType==eAssigned || m_eType==eModifiedAssigned || m_eType==eFAIRacing)
			{
		    cMgr.CreateElement( pIDOMChildNode, _T("Distance"),	DistanceText(cTurnpointArray, eUnits) );
			}
	else if( IsAreaTask() ) 
			{
			double dMin;
			double dNominal; 
			double dMax;
			CString str;
			GetTurnAreaDistances( cTurnpointArray, dMin, dNominal, dMax);

			str.Format(_T("%4.2lf"), ConvertDistance( dMin, SYSTEMUNITS, eUnits) );
		    cMgr.CreateElement( pIDOMChildNode, _T("MinDistance"),	str );
			str.Format(_T("%4.2lf"), ConvertDistance( dNominal, SYSTEMUNITS, eUnits) );
		    cMgr.CreateElement( pIDOMChildNode, _T("Distance"),	str );
			str.Format(_T("%4.2lf"), ConvertDistance( dMax, SYSTEMUNITS, eUnits) );
		    cMgr.CreateElement( pIDOMChildNode, _T("MaxDistance"),	str );
			}

	return true;
	}


bool CTask::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode)
{
    MSXML2::IXMLDOMNodePtr DOMContestantPtr;

	TCHAR buff[10];

    MSXML2::IXMLDOMNodePtr pIDOMChildNode;
	cMgr.CreateChild(pParentNode, pIDOMChildNode, _T("Task") );

	_itoa_s(m_eStatus,buff,10,10);
	cMgr.CreateElement( pIDOMChildNode, _T("Status"), buff);
	_itoa_s(m_eType,buff,10,10);
	cMgr.CreateElement( pIDOMChildNode, _T("Type"),	buff);
	cMgr.CreateElement( pIDOMChildNode, _T("Class"),	LPCSTR(GetClass(m_eClass).GetName()) );
	_itoa_s(m_eClass,buff,10,10);
	cMgr.CreateElement( pIDOMChildNode, _T("ClassID"), buff);
	GetXMLDate( cMgr, pIDOMChildNode,  _T("Date"),	m_cDate);
//	cMgr.CreateElement( pIDOMChildNode, _T("DateID", itoa(m_cDate.GetTime(),buff,10));

	GetXMLTime( cMgr, pIDOMChildNode,  _T("TaskOpenTime"), m_cGateOpenTime, false);
//	cMgr.CreateElement( pIDOMChildNode, "TaskOpen", itoa(m_cGateOpenTime.GetTime(),buff,10));
	_itoa_s((int)m_cPostTime.GetTotalSeconds(),buff,10,10);
	cMgr.CreateElement( pIDOMChildNode, _T("TaskTime"), buff);

	MSXML2::IXMLDOMNodePtr pStartGateNode;
	cMgr.CreateChild( pIDOMChildNode, pStartGateNode, _T("StartGate") );

	m_cStartGate.GetXML(cMgr, pStartGateNode);

	if( m_b2ndGateActive )
		{
		MSXML2::IXMLDOMNodePtr pStartGateNode;
		cMgr.CreateChild(pParentNode, pStartGateNode, _T("StartGate2"));
;
		m_cStartGate.GetXML(cMgr, pStartGateNode);
		}

	MSXML2::IXMLDOMNodePtr pTurnpoints;
	cMgr.CreateChild(pIDOMChildNode, pTurnpoints, _T("Turnpoints"));

	for(int i=0; i<m_nTurnpoints; i++)
		{
		_itoa_s(m_aiTurnpointIDs[i],buff,10,10);
		cMgr.CreateElement( pTurnpoints, _T("ID"), buff);
		cMgr.CreateElementDbl( pTurnpoints, _T("Radius"),m_afTurnpointRadius[i] );
		}

	MSXML2::IXMLDOMNodePtr pFinishGateNode;
	cMgr.CreateChild( pIDOMChildNode, pFinishGateNode, _T("FinishGate"));

	m_cFinishGate.GetXML(cMgr, pFinishGateNode);

	return true;
	}
 
CTask::CTask(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pTask)
		{
		Initialize();

		LoadXML(cMgr, pTask, false);
/*
		GET_XML_INT(cMgr, pTask, _T("Status"), EStatus, m_eStatus, eUnofficial);
		GET_XML_INT(cMgr, pTask, _T("Type"), ETaskType, m_eType, eAssigned);
		GET_XML_INT(cMgr, pTask, _T("ClassID"), EClass, m_eClass, eStandard);

//		GET_XML_INT(pTask, "DateID", int, m_cDate, 0); //Leave this here for legacy.
//		GET_XML_INT(pTask, "TaskOpen", int, m_cGateOpenTime, 0);

		ImportXMLDate( cMgr, pTask, _T("Date"),			m_cDate);
		ImportXMLTime( cMgr, pTask, _T("TaskOpenTime"),	m_cGateOpenTime, 
												m_cDate.GetYear(),
												m_cDate.GetMonth(),
												m_cDate.GetDay()	);
		GET_XML_INT(cMgr, pTask, _T("TaskTime"), int, m_cPostTime, 0);


		MSXML2::IXMLDOMNodePtr pStartGate;
		TESTHR(cMgr.SelectChild(_T("StartGate"),pTask,pStartGate));
		if( pStartGate!=NULL )
			{
			m_cStartGate.FromXML(cMgr, pStartGate);
			}

		MSXML2::IXMLDOMNodePtr pStartGate2;
		TESTHR(cMgr.SelectChild(_T("StartGate2"),pTask,pStartGate2));
		if( pStartGate2!=NULL )
			{
			m_b2ndGateActive=true;
			m_cStartGate2.FromXML(cMgr, pStartGate);
			}


		MSXML2::IXMLDOMNodePtr pFinishGate;
		TESTHR(cMgr.SelectChild(_T("FinishGate"),pTask,pFinishGate));
		if( pFinishGate!=NULL )
			{
			m_cFinishGate.FromXML(cMgr, pFinishGate);
			}


		MSXML2::IXMLDOMNodePtr pTurnpoints;
		TESTHR(cMgr.SelectChild(_T("Turnpoints"),pTask,pTurnpoints));
		if( pTurnpoints!=NULL )
			{
			MSXML2::IXMLDOMNodeListPtr pIDs=cMgr.SelectNodeList(_T("ID"),pTurnpoints );
			if( pIDs!=NULL )
				{
				long length=0;
				TESTHR(pIDs->get_length(&length));
				m_nTurnpoints=length;
				for( int i=0; i<m_nTurnpoints; i++ )
					{
					MSXML2::IXMLDOMNodePtr pID;
					TESTHR(pIDs->get_item(i,&pID));
					BSTR bstr;
					pID->get_text(&bstr);
					CString str=bstr;
					m_aiTurnpointIDs[i]=atoi(str);
					}

				MSXML2::IXMLDOMNodeListPtr pRads=cMgr.SelectNodeList(_T("Radius"),pTurnpoints );
				if( pRads!=NULL )
					{
					for(  int i=0; i<m_nTurnpoints; i++ )
						{
						MSXML2::IXMLDOMNodePtr pRad;
						TESTHR(pRads->get_item(i,&pRad));
						BSTR bstr;
						pRad->get_text(&bstr);
						CString str=bstr;
						m_afTurnpointRadius[i]=(float)atof(str);
						}
					}
				}
			}
			*/
		}


void CTask::Initialize()
	{
	m_eStatus=eNoContest;
	m_eClass=eStandard;
	m_eType=eAssigned;
	m_nTurnpoints=0;
	for( int i=0; i<MAXTASKTPS; i++) m_aiTurnpointIDs[i]=0;

	float fTurnpointRadius		= (float)GetWinscoreDouble(INNERRADIUS, ConvertDistance(DEFAULTINNERRADIUS, eStatute, SYSTEMUNITS) );
	for( int i=0; i<MAXTASKTPS; i++) m_afTurnpointRadius[i]=fTurnpointRadius;

	m_b2ndGateActive=FALSE;
	m_cPostTime		=CTimeSpan(0,0,0,0);
	m_cGateOpenTime	=CTime(0);

	}


CString CTask::GetText(CTurnpointArray &turnpointArray, EUnits eUnits)
	{
	CString strOut;
	CString cLine;
	CString strNEWLINE=_T("\r\n");
	CString strTAB=_T("\t");

    if( IsAreaTask() || m_eType== eAssigned || m_eType== eModifiedAssigned || m_eType== eFAIRacing )
    	{
    	if( m_eType==eAssigned )
    		strOut+=_T("Assigned Task");
    	else if(m_eType== eModifiedAssigned)
    		strOut+=_T("Modified Assigned Task");
    	else if(m_eType== eTurnArea)
    		strOut+=_T("Turn Area Task");
    	else if(m_eType== eFAIRacing)
    		strOut+=_T("FAI Racing Task");
    	else if(m_eType== eFAIAssignedArea)
    		strOut+=_T("FAI Assigned Area Task");

    	if( IsTimedTask() )
    		{
     		cLine=m_cPostTime.Format(_T("\tMinimum Time: %H:%M") );
			strOut+=cLine;  
    		}

		strOut+=strNEWLINE;
		strOut+=strTAB;
    	strOut+=_T("ID");
		strOut+=strTAB;
    	strOut+=_T("Name");
		strOut+=strTAB;
    	strOut+=_T("Distance (");
    	strOut+=UnitsText(eUnits);
    	strOut+=_T(")");
		if( IsAreaTask()  || 
			m_cStartGate.IsGPSCylinder() ||
			m_cFinishGate.IsGPSCylinder()   )
			{
			strOut+=strTAB;
			strOut+=_T("Radius\t Alt(MSL)");
			}
		strOut+=strNEWLINE;
			
    
   		double dDist=0.0;
   		CTurnpoint *pcTurnpoint=NULL, *pcPrevTurnpoint=NULL;
    
		pcTurnpoint=turnpointArray[m_cStartGate.GetPointID()-1];
		OUTPUT_TPT_TEXT( pcTurnpoint, dDist,m_cStartGate.IsGPSCylinder()?(ConvertDistance(m_cStartGate.GetRadius(),SYSTEMUNITS, eUnits)):(0.0), cLine, true, "", strOut );
		strOut+=GetStartCeilingText();
		strOut+=" Max";
		strOut+=strNEWLINE;

   		pcPrevTurnpoint=pcTurnpoint;

		if( m_cStartGate.IsGPSCylinder() )
			dDist-=ConvertDistance(m_cStartGate.GetRadius(), SYSTEMUNITS, eUnits);

   		for( int iTpt=0; iTpt<GetNumTurnpoints(); iTpt++ )
    		{
     		pcPrevTurnpoint=pcTurnpoint;
   			pcTurnpoint=turnpointArray[GetTurnpointID(iTpt)-1];
        	dDist+=pcPrevTurnpoint->DistanceTo( pcTurnpoint, eUnits);
			OUTPUT_TPT_TEXT( pcTurnpoint, dDist,(IsAreaTask())?(ConvertDistance(GetTurnpointRadius(iTpt),SYSTEMUNITS, eUnits)):(0.0), cLine, true, strNEWLINE, strOut );
 			pcPrevTurnpoint=pcTurnpoint;
    		}

   		pcTurnpoint=turnpointArray[m_cFinishGate.GetPointID()-1];
       	dDist+=pcPrevTurnpoint->DistanceTo( pcTurnpoint, eUnits);

		if( m_cFinishGate.IsGPSCylinder() )
			dDist-=ConvertDistance(m_cFinishGate.GetRadius(), SYSTEMUNITS, eUnits);

   		pcTurnpoint=turnpointArray[m_cFinishGate.GetPointID()-1];
  		OUTPUT_TPT_TEXT( pcTurnpoint, dDist, m_cFinishGate.IsGPSCylinder()?(ConvertDistance(m_cFinishGate.GetRadius(),SYSTEMUNITS, eUnits)):(0.0), cLine, true, "", strOut );
		strOut+=GetFinishBaseText();
		strOut+=" Min";
		strOut+=strNEWLINE;

    	if( m_eType== eAssigned || m_eType== eFAIRacing )
    		{
			strOut+=strNEWLINE;
    		cLine.Format( _T("Total Distance: %6.2lf %s"), dDist, UnitsText(eUnits)  );
			strOut+=cLine;
    		}
   		else if(  IsAreaTask() )
    		{
			double dMin, dDist, dMax;
			GetTurnAreaDistances( turnpointArray,  dMin, dDist,  dMax );

			strOut+=strNEWLINE;
      		cLine.Format( _T("Distance (%s)- %6.2lf Min: %6.2lf, Max: %6.2lf"), UnitsText(eUnits),	ConvertDistance(dDist, SYSTEMUNITS, eUnits),
																								ConvertDistance(dMin, SYSTEMUNITS, eUnits),
																								ConvertDistance(dMax, SYSTEMUNITS, eUnits)  );
			strOut+=cLine;
     		}
		}

	return strOut;
	}



bool CTask::Export(CString strFileName)
{

	try
		{
		CXMLMgr cMgr;
		cMgr.Initlize();
		cMgr.CreateXMLFileHeader();
		cMgr.CreateProcInstruction(_T("xml-stylesheet"),_T("type=\"text/xsl\" href=\"ws_scoresheet.xsl\""));
		cMgr.CreateRoot(_T("Winscore"));

		MSXML2::IXMLDOMNodePtr pIDOMChildNode=NULL;
		cMgr.CreateChild( cMgr.GetRoot(), pIDOMChildNode, _T("Tasks"));

		if( !GetXML(cMgr, pIDOMChildNode) ) return false;

		TESTHR(cMgr.Save(strFileName) );
		}
	catch(...)
		{
		return false;
		}
	return true;
}


bool CTask::Import(CString strFileName)
{
		try
			{
			CXMLMgr cMgr;
			cMgr.Initlize();

			if( !cMgr.Load(strFileName) ) return false;

			CString strRoot=cMgr.GetNodeName(cMgr.GetRoot());
			if(strRoot!=_T("Winscore") ) return false;

			MSXML2::IXMLDOMNodePtr pTasks;
			TESTHR(cMgr.SelectChild(_T("Tasks"),cMgr.GetRoot(),pTasks));
			if( pTasks==NULL ) return false;

			MSXML2::IXMLDOMNodeListPtr pTaskList=cMgr.GetChildList(pTasks );
			if( pTaskList==NULL ) return false;

			long length=0;
			TESTHR(pTaskList->get_length(&length));
			if( length!=1 ) return false;

			MSXML2::IXMLDOMNodePtr pTask;
			TESTHR(pTaskList->get_item(0,&pTask));
			if( !pTask ) return false;
			if( LoadXML( cMgr, pTask, true ) ) return true;
			}
		catch (...)
			{
			return false;
			}
		return false;
	}


bool CTask::LoadXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pTask, bool bFromLibrary)
		{
		if( !bFromLibrary )
			{
			GET_XML_INT(cMgr, pTask, _T("Status"), EStatus, m_eStatus, eUnofficial);
			GET_XML_INT(cMgr, pTask, _T("ClassID"), EClass, m_eClass, eStandard);

			ImportXMLDate( cMgr, pTask, _T("Date"),			m_cDate);
			ImportXMLTime( cMgr, pTask, _T("TaskOpenTime"),	m_cGateOpenTime, 
													m_cDate.GetYear(),
													m_cDate.GetMonth(),
													m_cDate.GetDay()	);
			}
		else
			{// Status from library is always preliminary
			m_eStatus=ePreliminary;
			}

		GET_XML_INT(cMgr, pTask, _T("Type"), ETaskType, m_eType, eAssigned);
		GET_XML_INT(cMgr, pTask, _T("TaskTime"), int, m_cPostTime, 0);


		MSXML2::IXMLDOMNodePtr pStartGate;
		TESTHR(cMgr.SelectChild(_T("StartGate"),pTask,pStartGate));
		if( pStartGate!=NULL )
			{
			m_cStartGate.FromXML(cMgr, pStartGate);
			}

		MSXML2::IXMLDOMNodePtr pStartGate2;
		TESTHR(cMgr.SelectChild(_T("StartGate2"),pTask,pStartGate2));
		if( pStartGate2!=NULL )
			{
			m_b2ndGateActive=true;
			m_cStartGate2.FromXML(cMgr, pStartGate);
			}


		MSXML2::IXMLDOMNodePtr pFinishGate;
		TESTHR(cMgr.SelectChild(_T("FinishGate"),pTask,pFinishGate));
		if( pFinishGate!=NULL )
			{
			m_cFinishGate.FromXML(cMgr, pFinishGate);
			}


		MSXML2::IXMLDOMNodePtr pTurnpoints;
		TESTHR(cMgr.SelectChild(_T("Turnpoints"),pTask,pTurnpoints));
		if( pTurnpoints!=NULL )
			{
			MSXML2::IXMLDOMNodeListPtr pIDs=cMgr.SelectNodeList(_T("ID"),pTurnpoints );
			if( pIDs!=NULL )
				{
				long length=0;
				TESTHR(pIDs->get_length(&length));
				m_nTurnpoints=length;
				for( int i=0; i<m_nTurnpoints; i++ )
					{
					MSXML2::IXMLDOMNodePtr pID;
					TESTHR(pIDs->get_item(i,&pID));
					BSTR bstr;
					pID->get_text(&bstr);
					CString str=bstr;
					m_aiTurnpointIDs[i]=atoi(str);
					}

				MSXML2::IXMLDOMNodeListPtr pRads=cMgr.SelectNodeList(_T("Radius"),pTurnpoints );
				if( pRads!=NULL )
					{
					for(  int i=0; i<m_nTurnpoints; i++ )
						{
						MSXML2::IXMLDOMNodePtr pRad;
						TESTHR(pRads->get_item(i,&pRad));
						BSTR bstr;
						pRad->get_text(&bstr);
						CString str=bstr;
						m_afTurnpointRadius[i]=(float)atof(str);
						}
					}
				}
			}
		return true;
}

bool CTask::IsOfficial(void)
{
	return m_eStatus==eOfficial;
}


int CTask::ExportCUP(CString strFileName, CTurnpointArray& cTurnpointArray, EUnits eUnit)
{
	CString strCUPUnits="m";
	//if( eUnit==eStatute )
	//	strCUPUnits="ml";
	//else if( eUnit==eNautical )
	//	strCUPUnits="nm";
	//else if( eUnit==eKilometers)
	//	strCUPUnits="km";

	CTurnpoint *pcTurnpoint;

	if(m_eStatus==eNoContest || m_nTurnpoints==0 ) 
		{
		AfxMessageBox(_T("No task defined for this day."));
		return -1;
		}
	
	CStdioFile  cFile;
	CFileException e;
	if( !cFile.Open( strFileName, CFile::modeCreate | CFile::modeWrite, &e ) )
		{
		AfxMessageBox(_T("Could not open file: ") + strFileName);
		return -1;
		}

	cFile.WriteString( "name,code,country,lat,lon,elev,style,rwdir,rwlen,rwwidth,freq,desc,userdata,pics\n" );

	//Write Start
	pcTurnpoint =(CTurnpoint*)cTurnpointArray.GetAt(m_cStartGate.GetPointID()-1);
	cFile.WriteString( pcTurnpoint->GetCUP() );

	for( int i=0; i<m_nTurnpoints; i++ )
		{
		CTurnpoint* pcTurnpoint=cTurnpointArray.GetAt(m_aiTurnpointIDs[i]-1);
		ASSERT( pcTurnpoint );
		if( !pcTurnpoint ) continue;
		cFile.WriteString( pcTurnpoint->GetCUP() );
		}

	//Write Finish
	pcTurnpoint =(CTurnpoint*)cTurnpointArray.GetAt(m_cFinishGate.GetPointID()-1);
	cFile.WriteString( pcTurnpoint->GetCUP() );


	//  Now output the task
	cFile.WriteString("----Related Tasks----\n");


	cFile.WriteString("\"");
	cFile.WriteString( DateText() );
	cFile.WriteString(" - ");
	cFile.WriteString(GetClass(m_eClass).GetName());
	cFile.WriteString("\",\"???\",\"");

	pcTurnpoint =(CTurnpoint*)cTurnpointArray.GetAt(m_cStartGate.GetPointID()-1);
	cFile.WriteString( pcTurnpoint->m_strName );
	cFile.WriteString("\",");

	for( int i=0; i<m_nTurnpoints; i++ )
		{
		CTurnpoint* pcTurnpoint=cTurnpointArray.GetAt(m_aiTurnpointIDs[i]-1);
		ASSERT( pcTurnpoint );
		if( !pcTurnpoint ) continue;
		cFile.WriteString("\"");
		cFile.WriteString( pcTurnpoint->m_strName );
		cFile.WriteString("\",");
		}

	pcTurnpoint =(CTurnpoint*)cTurnpointArray.GetAt(m_cFinishGate.GetPointID()-1);
	cFile.WriteString("\"");
	cFile.WriteString( pcTurnpoint->m_strName );
	cFile.WriteString("\",\"???\"\n");


	// Now do the options:
//enum	ETaskType {eAssigned=0, eModifiedAssigned=2, eTurnArea=3, eFAIRacing=4, eFAIAssignedArea=5};

	CString strOptions="Options,";

	strOptions+="NoStart="+TaskOpenText();
    strOptions+=":00";

	float fTurnpointRadius		= (float)GetWinscoreDouble(INNERRADIUS, ConvertDistance(DEFAULTINNERRADIUS, eStatute, SYSTEMUNITS) );

	if( m_eType!=eFAIRacing &&  m_eType!=eAssigned )
		{
	    strOptions+=",TaskTime="+MinTimeText();
	    strOptions+=":00";
		}

	CString strOptions2;
	strOptions2.Format(",NearDis=%2.3lf%s,NearAlt=656.0ft",ConvertDistance( fTurnpointRadius, SYSTEMUNITS,eKilometers )*1000.,strCUPUnits);
	strOptions+=strOptions2;


	// Do the observation zones:


	if( m_eType==eFAIAssignedArea || m_eType==eTurnArea )
		{
		strOptions+=",WpDis=False\n";
		cFile.WriteString( strOptions);


		CString strOBZone;
		strOBZone.Format("ObsZone=0,Style=2,SpeedStyle=0,R1=%2.3lf%s,A1=180,R2=0m,A2=0,MaxAlt=%i.0ft", ConvertDistance( m_cStartGate.GetRadius(), SYSTEMUNITS, eKilometers )*1000.,strCUPUnits,m_cStartGate.GetMaxAltitude());
		cFile.WriteString( strOBZone);
		if( !m_cStartGate.IsGPSCylinder() )
			cFile.WriteString(",Line=1");
		cFile.WriteString("\n");
	
		for( int i=0; i<m_nTurnpoints; i++ )
			{
			CString strOBZone;
			strOBZone.Format("ObsZone=%i,Style=1,SpeedStyle=1,R1=%5.3lf%s,A1=180,R2=0m,A2=0,MaxAlt=0.0ft,AAT=1",i+1,ConvertDistance( m_eType==eFAIRacing||m_eType==eAssigned?fTurnpointRadius:m_afTurnpointRadius[i], SYSTEMUNITS,eKilometers )*1000.,strCUPUnits);


			cFile.WriteString( strOBZone);
			cFile.WriteString("\n");
			}

		//Finish
		strOBZone.Format("ObsZone=%i,Style=3,SpeedStyle=2,R1=%2.3lf%s,A1=180,R2=0m,A2=0,MaxAlt=0.0ft", m_nTurnpoints+1, ConvertDistance( m_cFinishGate.GetRadius(), SYSTEMUNITS, eKilometers )*1000.,strCUPUnits);
		cFile.WriteString( strOBZone);
		if( m_cFinishGate.IsGPSCylinder() )
			cFile.WriteString( ",Reduce=1");

		cFile.WriteString("\n");
		}
	else //Racing Tasks
		{
		strOptions+="\n";
		cFile.WriteString( strOptions);

		//ObsZone=0,Style=2,SpeedStyle=0,R1=4828m,A1=180,R2=0m,A2=0,MaxAlt=5000.0ft,Line=1

		CString strOBZone;
		strOBZone.Format("ObsZone=0,Style=2,SpeedStyle=0,R1=%2.3lf%s,A1=180,R2=0m,A2=0,MaxAlt=%i.0ft", ConvertDistance( m_cStartGate.GetRadius(), SYSTEMUNITS, eKilometers )*1000.,strCUPUnits,m_cStartGate.GetMaxAltitude());
		cFile.WriteString( strOBZone);
		if( !m_cStartGate.IsGPSCylinder() )
			cFile.WriteString(",Line=1");
		cFile.WriteString("\n");

		for( int i=0; i<m_nTurnpoints; i++ )
			{
			CString strOBZone;
			strOBZone.Format("ObsZone=%i,Style=5,SpeedStyle=1,R1=%5.3lf%s,A1=180,R2=0m,A2=0,MaxAlt=0.0ft\n",i+1,ConvertDistance( fTurnpointRadius, SYSTEMUNITS,eKilometers )*1000.,strCUPUnits);
			cFile.WriteString( strOBZone);

			//ObsZone=1,Style=5,SpeedStyle=1,R1=483m,A1=180,R2=0m,A2=0,MaxAlt=0.0ft
			}
		//Finish
		strOBZone.Format("ObsZone=%i,Style=3,SpeedStyle=2,R1=%2.3lf%s,A1=180,R2=0m,A2=0,MaxAlt=0.0ft", m_nTurnpoints+1, ConvertDistance( m_cFinishGate.GetRadius(), SYSTEMUNITS, eKilometers )*1000.,strCUPUnits);
		cFile.WriteString( strOBZone);

		if( m_cFinishGate.IsGPSCylinder() )
			cFile.WriteString( ",Reduce=1");
		cFile.WriteString("\n");

		}

	cFile.Close();

	return 0;
}
