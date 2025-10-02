//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// WSClass.cpp: implementation of the CWSClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "winscore.h"
#include "global_prototypes.h"
#include "scorerecord.h"
#include "WSClass.h"
#include <atlbase.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static	CWSClass*	pcClasses[NUMCLASSES];


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL( CWSClass, CWSClass, VERSIONABLE_SCHEMA|2)

void CWSClass::AllocateClasses()
	{
	for (int i = 0; i < NUMCLASSES; i++)
		{
		pcClasses[i] = new CWSClass();
		}
	}

void CWSClass::FreeClasses()
{
	for (int i = 0; i < NUMCLASSES; i++)
	{
		delete pcClasses[i];
		pcClasses[i] = NULL;
	}
}


CWSClass::CWSClass()
	{
	m_strName="Uninitialized";
	m_iOptions=0;
	SetHandicapped(false);
	SetUseShortTaskFactor(true);
	SetUseScoredCompletionRatio(true);
	SetActive(false);
	SetUsingWDSA(false);

	m_iAirfieldBonusPoints	=25;

	m_dMSP_M_ASGNED	=1000;
	m_dMSP_B_ASGNED	=400;
	m_dMDP_M_ASGNED	= -.25;
	m_dMDP_B_ASGNED =.65;

	m_dMSP_M_POST	=800;	
	m_dMSP_B_POST	=400;
	m_dMDP_M_POST	= -.25;
	m_dMDP_B_POST	=.65;

	m_dNoFinisherPoints_ASGNED	=400;
	m_dNoFinisherPoints_POST	=400;

	m_dD1=ConvertDistance(250, eKilometers, SYSTEMUNITS);
	m_dDm=ConvertDistance(100., eKilometers, SYSTEMUNITS);

	m_cSMTT=CTimeSpan(0,2,0,0);
	m_dSMTD=ConvertDistance(50.0, eStatute, SYSTEMUNITS);

	}


//CWSClass::CWSClass(CString strName, EContest eContest, EClass eClass)
//{

//}

CWSClass::~CWSClass()
{

}


void CWSClass::Initialize(EContest eContest, EClass eClass)
{
	m_eContest	=eContest;
	m_eClass	=eClass;
	SetActive(false);

	if( eClass==eStandard )
		m_strName="Standard";
	else if( eClass== e15Meter)
		m_strName="15 Meter";
	else if( eClass== eSports)
		m_strName="Sports";
	else if( eClass== eClub)
		m_strName="Club";
	else if( eClass== eOpen	)
		m_strName="Open";
	else if( eClass== e18Meter )
		m_strName="18 Meter";
	else if( eClass== eFAIHandicapped	)
		m_strName="FAI Handicapped";
	else if( eClass== eUser1)
		m_strName="User Defined 1";
	else if( eClass== eUser2)
		m_strName="User Defined 2";
	else if( eClass== eUser3)
		m_strName="User Defined 3";


	if( eClass==	eStandard		||
		eClass==	e15Meter		||
		eClass==	eOpen			||
		eClass==	e18Meter		||
		eClass==	eFAIHandicapped ||
		eClass==	eUser1			||
		eClass==	eUser2			||
		eClass==	eUser3				)
		{

		if( eClass==eStandard||eClass==eFAIHandicapped)
		   SetHandicapped(true);
		else
		   SetHandicapped(false);

		SetUseShortTaskFactor(true);
		SetUseScoredCompletionRatio(true);

		m_iAirfieldBonusPoints	=25;

		if( eContest==eNational )
			{
			m_dMSP_M_ASGNED	=660;
			m_dMSP_B_ASGNED	=600;
			m_dMDP_M_ASGNED	= -.2;
			m_dMDP_B_ASGNED =.8;
			m_dMSP_M_POST	=500;	
			m_dMSP_B_POST	=600;
			m_dMDP_M_POST	= -.2;
			m_dMDP_B_POST	=.8;
			}
		else //Regional
			{
			m_dMSP_M_ASGNED	=660;
			m_dMSP_B_ASGNED	=600;
			m_dMDP_M_ASGNED	= -.2;
			m_dMDP_B_ASGNED =.8;
			m_dMSP_M_POST	=500;	
			m_dMSP_B_POST	=600;
			m_dMDP_M_POST	= -.2;
			m_dMDP_B_POST	=.8;
			}

		m_dNoFinisherPoints_ASGNED	=400;
		m_dNoFinisherPoints_POST	=400;

		if( eContest==eNational )
			{
			m_cSMTT=CTimeSpan(0,3,0,0);
			if( eClass==eStandard )
				m_dSMTD=ConvertDistance(50.0, eStatute, SYSTEMUNITS);
			else
				m_dSMTD=ConvertDistance(60.0, eStatute, SYSTEMUNITS);
			}
		else
			{
			m_cSMTT=CTimeSpan(0,2,0,0);
			m_dSMTD=ConvertDistance((eClass==eStandard||eClass==eFAIHandicapped)?40.0:50.0, eStatute, SYSTEMUNITS);
			}
		}
	else if( eClass==eSports || eClass==eClub )
		{
		SetHandicapped(true);
		SetUseShortTaskFactor(true);
		SetUseScoredCompletionRatio(true);

		m_iAirfieldBonusPoints	=25;

		if( eContest==eNational )
			{
			m_dMSP_M_ASGNED	=660;
			m_dMSP_B_ASGNED	=600;
			m_dMDP_M_ASGNED	= -.2;
			m_dMDP_B_ASGNED =.8;

			m_dMSP_M_POST	=500;	
			m_dMSP_B_POST	=600;
			m_dMDP_M_POST	= -.2;
			m_dMDP_B_POST	=.8;
			}
		else
			{

			m_dMSP_M_ASGNED	=660;
			m_dMSP_B_ASGNED	=600;
			m_dMDP_M_ASGNED	= -.2;
			m_dMDP_B_ASGNED =.8;

			m_dMSP_M_POST	=500;	
			m_dMSP_B_POST	=600;
			m_dMDP_M_POST	= -.2;
			m_dMDP_B_POST	=.8;

			}			

		m_dNoFinisherPoints_ASGNED	=400;
		m_dNoFinisherPoints_POST	=400;


		if( eContest==eNational )
			{
			m_cSMTT=CTimeSpan(0,3,0,0);
			m_dSMTD=ConvertDistance(50.0, eStatute, SYSTEMUNITS);
			}
		else
			{
			if( eClass==eSports )
				m_cSMTT=CTimeSpan(0,1,30,0);
			else
				m_cSMTT=CTimeSpan(0,2,0,0);
			m_dSMTD=ConvertDistance(40.0, eStatute, SYSTEMUNITS);
			}
		}

							// 2018 FAI Rules
	//		m_dD1;	// Minimum Distance for 1000 points, depending on the class:
						/* Class								D1
						13.5 Metre Club							250 km
						Standard, 15 Metre, 20 Metre Multi-seat 300 km
						18 Metre, Open							350 km */

	//		m_dDm;	//Minimum Distance to validate the Day, depending on the class:
						/* Class								Dm
						13.5 Metre, Club						100 km
						Standard, 15 Metre, 20 Metre Multi-seat 120 km
						18 Metre, Open							140 km

11.4.1.1 ‡ Minimum distance for a Championship Day with a maximum score of 1000 points (D1):
● For Club class: 155 mi
● For Standard, 15-Meter, and 20-Meter classes: 186 mi
● For 18-Meter and Open classes: 217 mi
11.4.1.2 ‡ Minimum distance to validate a Championship Day (Dm):
● For Club class: 62 mi
● For Standard, 15-Meter, and 20-Meter classes: 75 mi
● For 18-Meter and Open classes: 87 mi
						*/

	if( eClass==eStandard || eClass== e15Meter)
		{
		m_dD1=ConvertDistance(186., eStatute, SYSTEMUNITS);
		m_dDm=ConvertDistance(75., eStatute, SYSTEMUNITS);
		}
	else if( eClass==e18Meter || eClass== eOpen)
		{
		m_dD1=ConvertDistance(217., eStatute, SYSTEMUNITS);
		m_dDm=ConvertDistance(87., eStatute, SYSTEMUNITS);
		}
	else 
		{
		m_dD1=ConvertDistance(155., eStatute, SYSTEMUNITS);
		m_dDm=ConvertDistance(62., eStatute, SYSTEMUNITS);
		}
}

CWSClass& CWSClass::operator =(CWSClass &cClass)
{

	m_strName			= cClass.m_strName;
	SetHandicapped( cClass.IsHandicapped());
	SetUseShortTaskFactor( cClass.IsUseShortTaskFactor());
	SetUseScoredCompletionRatio(cClass.IsUseScoredCompletionRatio());
	SetActive( cClass.IsActive());
	m_iAirfieldBonusPoints	= cClass.m_iAirfieldBonusPoints;
	SetUsingWDSA( cClass.IsUsingWDSA());
	m_dNoFinisherPoints_ASGNED = cClass.m_dNoFinisherPoints_ASGNED;
	m_dMDP_M_ASGNED		= cClass.m_dMDP_M_ASGNED;
	m_dMDP_B_ASGNED		= cClass.m_dMDP_B_ASGNED;
	m_dMSP_M_ASGNED		= cClass.m_dMSP_M_ASGNED;
	m_dMSP_B_ASGNED		= cClass.m_dMSP_B_ASGNED;
	m_dNoFinisherPoints_POST = cClass.m_dNoFinisherPoints_POST;
	m_dMDP_M_POST		= cClass.m_dMDP_M_POST;
	m_dMDP_B_POST		= cClass.m_dMDP_B_POST;
	m_dMSP_M_POST		= cClass.m_dMSP_M_POST;
	m_dMSP_B_POST		= cClass.m_dMSP_B_POST;
	m_dSMTD				= cClass.m_dSMTD;
	m_cSMTT				= cClass.m_cSMTT;
	m_eContest			= cClass.m_eContest;
	m_eClass			= cClass.m_eClass;
	m_dDm				= cClass.m_dDm;
	m_dD1				= cClass.m_dD1;
	return *this;
}


void CWSClass::SetMinTaskTime(double dHours)
	{
	m_cSMTT=CTimeSpan(0, 0, (int)(dHours*60.), 0 );
	}



///////////////////////	  class globals ////////////////////////////////


CWSClass& GetClass(int i)
	{
	return *pcClasses[i];
	}


CWSClass& GetClass(EClass eClass, bool bActiveOnly)
	{
	int iSel=0;
	if( bActiveOnly )
		{
		for( int i=0; i<NUMCLASSES; i++ )
			{
			if( pcClasses[i]->GetType()==eClass &&
				pcClasses[i]->IsActive()			) 
				{
				iSel=i;
				break;
				};
			}
		}
	else
		{
		for( int i=0; i<NUMCLASSES; i++ )
			{
			if( pcClasses[i]->GetType()==eClass 	) 
				{
				iSel=i;
				break;
				};
			}
		}

	return *pcClasses[iSel];
	}

CWSClass& GetClass(EContest eContest, EClass eClass)
	{
	int iSel=0;
	for( int i=0; i<NUMCLASSES; i++ )
		{
		if( pcClasses[i]->GetType()	==eClass &&
			pcClasses[i]->m_eContest	==eContest ) 
			{
			iSel=i;
			break;
			}
		}	
	return *pcClasses[iSel];
	}

int GetClassPos(EContest eContest, EClass eClass)
	{
	int iSel=0;
	for( int i=0; i<NUMCLASSES; i++ )
		{
		if( pcClasses[i]->GetType()	==eClass &&
			pcClasses[i]->m_eContest	==eContest ) 
			{
			iSel=i;
			break;
			}
		}	
	return iSel;
	}

bool IsClassValid( EClass eClass )
	{
	return	eClass==eStandard	||
			eClass==e15Meter	||
			eClass==eSports		||
			eClass==eOpen		||
			eClass==eClub		||
			eClass==e18Meter	||
			eClass==eFAIHandicapped		||
			eClass==eUser1		||
			eClass==eUser2		||
			eClass==eUser3;
	}


////////////////////////////////////////////////////////////////////////
//
//	Classes excluded from the viewer
//
//
//

#ifndef WSVIEWER


double CWSClass::GetMaximumSpeedPoints(ETaskType eTaskType, CSummary &cSummary)
	{
    double dCR=GetCompletionRatio(eTaskType,cSummary);
	if ( eTaskType== eFAIRacing )  //TODO
		return 1000.;
	else if( eTaskType==eAssigned )
		return min( (m_dMSP_B_ASGNED +m_dMSP_M_ASGNED*dCR), 1000.);
	else
		return min( (m_dMSP_B_POST +m_dMSP_M_POST*dCR), 1000.);
	}


CTimeSpan CWSClass::GetMinTaskTime()
	{
	return m_cSMTT;
	}

double CWSClass::GetMinTaskDistance(EUnits eUnits)
	{
    return ConvertDistance(m_dSMTD, SYSTEMUNITS, eUnits);
	}


double CWSClass::GetContestantFactor(CSummary &cSummary)
{
	if( !IsUseShortTaskFactor() ) return 1.0; 

	double dCF = ((double)cSummary.m_nContestants)/3.0;

	return min(dCF, 1.0);
}

double CWSClass::GetShortTaskFactor(CSummary &cSummary)
{
	if( !IsUseShortTaskFactor() || cSummary.m_nFinishers==0 ) return 1.0; 

 	if( IsHandicapped() )
		return min( ((double)cSummary.m_cTOCBestHdcapSpeed.GetTotalSeconds()/(double)GetMinTaskTime().GetTotalSeconds()), 1.00 );
    else	
		return min( ((double)cSummary.m_cTOCBestSpeed.GetTotalSeconds()/(double)GetMinTaskTime().GetTotalSeconds()), 1.00 );
}



void CWSClass::GetPoints( CTask *pcTask, CObject *pObject,  CSummary &cSummary)
{
	CScoreRecord *pcScoreRecord=(CScoreRecord*)pObject;
	double	dShortTaskFactor=GetShortTaskFactor( cSummary);
	double	dContestantFactor=GetContestantFactor( cSummary);
	double	dMaxSpeedPoints	=dShortTaskFactor*dContestantFactor*GetMaximumSpeedPoints(	 pcTask->m_eType, cSummary);
	double	dMaxDistPoints	=dShortTaskFactor*dContestantFactor*GetMaximumDistancePoints(	 pcTask->m_eType, cSummary);
	double BESTDIST=0.0;
	double dPv, dPd;

    if( pcTask->m_eType== eFAIRacing || pcTask->m_eType== eFAIAssignedArea  ) 
		{
		/* Competitor's Score:
		*/

		double dSpeed	=IsHandicapped()?(pcScoreRecord->m_dHandicappedSpeed)	:(pcScoreRecord->m_dSpeed);
		double dDistance=IsHandicapped()?(pcScoreRecord->m_dHandicappedDistance):(pcScoreRecord->m_dDistance);

		if( pcScoreRecord->m_fFinisher )
			{

//(i) For any finisher:
//	Pv = 1000 x (Vh/Vo)
//	Pd = 750 x (Dh/Do)

			dPv = 1000.* dSpeed/cSummary.m_dVo;
			dPd = 750. * dDistance/cSummary.m_dDo;

			}
		else
			{
			// Non-Finisher
			dPv = 0.;
			if( cSummary.m_dDo==0.0 )
				dPd=0.0;
			else
				dPd = 750. * (dDistance / cSummary.m_dDo);
			}

		// SP0  - Highest Provisional Score (SP) of the Day
		// SPm  - Median Provisional Score (SP) of the Day, excluding competitors with SP = 0. Note: this is not the Mean Provisional Score.
		// Day - If the Day is not a Championship Day (see 8.2.1) then all
		//Sp = F x FCR x (Pv + Pd)

		//This is actually the provisional score Sp, need to compute S later when all the Sp's are in.
		pcScoreRecord->m_dPoints = cSummary.m_dF * cSummary.m_dFCR * max(dPv,dPd);

		}

	else if( pcTask->m_eType==eAssigned )
		{
		if( pcScoreRecord->m_fFinisher )
			{// Finished Assigned Task

				{
				double dPoints =0;
				double dPoints1=0;
				double dPoints2=0;
				double dPoints3=0;

				if( IsHandicapped() )
					{
   					dPoints1=	dMaxSpeedPoints * 
    							(pcScoreRecord->m_dHandicappedSpeed/cSummary.m_dBestHdcapSpeed);
  					dPoints2=	dMaxDistPoints + 30 +
								(dMaxSpeedPoints*.2*
    							((pcScoreRecord->m_dHandicappedSpeed/cSummary.m_dBestHdcapSpeed)-0.4));
  					dPoints3=	dMaxDistPoints + 30.;
					dPoints=max(dPoints1,dPoints);
					dPoints=max(dPoints2,dPoints);
					dPoints=max(dPoints3,dPoints);
					}
				else
					{
   					dPoints1=	dMaxSpeedPoints * 
    							(pcScoreRecord->m_dSpeed/cSummary.m_dBestSpeed);
  					dPoints2=	dMaxDistPoints + 30 +
								(dMaxSpeedPoints*.2*
    							((pcScoreRecord->m_dSpeed/cSummary.m_dBestSpeed)-0.4));
  					dPoints3=	dMaxDistPoints + 30.;
					dPoints=max(dPoints1,dPoints);
					dPoints=max(dPoints2,dPoints);
					dPoints=max(dPoints3,dPoints);
					}

				pcScoreRecord->m_dPoints=	max( dPoints, (m_iAirfieldBonusPoints+dMaxDistPoints) );
				}

			}
		else //non-finishers
			{

			if( cSummary.m_nFinishers==0 )
				{
				// No Finishers at all
				if( IsHandicapped() )
    				pcScoreRecord->m_dPoints= dMaxDistPoints * (pcScoreRecord->m_dHandicappedDistance/cSummary.m_dBestHdcapDist);
				else
    				pcScoreRecord->m_dPoints= dMaxDistPoints  * (pcScoreRecord->m_dDistance/cSummary.m_dTaskDistance);
				}
			else
				{
				// Points for non finisher
				if( IsHandicapped() )
    				pcScoreRecord->m_dPoints=	dMaxDistPoints * 
    											pcScoreRecord->m_dHandicappedDistance/cSummary.m_dBestHdcapDist;
				else
    				pcScoreRecord->m_dPoints=	dMaxDistPoints * 
    											(pcScoreRecord->m_dDistance/cSummary.m_dTaskDistance);
				}
			}
		}
   	else if( pcTask->m_eType==eTurnArea || pcTask->m_eType==eModifiedAssigned )
		{

		if( pcScoreRecord->m_fFinisher )
			{// Finished Post or modified Task

			double dPoints =0;
			double dPoints1=0;
			double dPoints2=0;
			double dPoints3=0;

			if( IsHandicapped() )
				{
   				dPoints1=	dMaxSpeedPoints * 
    						(pcScoreRecord->m_dScoredHandicappedSpeed/cSummary.m_dBestScoredHdcapSpeed);
  				dPoints2=	dMaxDistPoints *
							(pcScoreRecord->m_dHandicappedDistance/cSummary.m_dBestHdcapDist)+30;
				dPoints2= min(dPoints2,(dMaxDistPoints +30.));		
				dPoints = max(dPoints2,dPoints1);
				}
			else
				{
   				dPoints1=	dMaxSpeedPoints * 
    						(pcScoreRecord->m_dScoredSpeed/cSummary.m_dBestScoredSpeed);
  				dPoints2=	dMaxDistPoints *
							(pcScoreRecord->m_dDistance/cSummary.m_dBestDistance)+30;
				dPoints2= min(dPoints2,(dMaxDistPoints +30.));		
				dPoints = max(dPoints2,dPoints1);
				}

			pcScoreRecord->m_dPoints=dPoints;

			}
		else 
			{
			if( cSummary.m_nFinishers==0 )
				{
				// No Finishers at all in post task

				if( IsHandicapped() )
    				pcScoreRecord->m_dPoints= dMaxDistPoints * (pcScoreRecord->m_dHandicappedDistance/cSummary.m_dBestHdcapDist);
				else
    				pcScoreRecord->m_dPoints= dMaxDistPoints  * (pcScoreRecord->m_dDistance/cSummary.m_dBestDistance);

				}
			else
				{
				// Points for non finisher of MAT & TAT task

    			double dMinTimeHours=(double)pcTask->m_cPostTime.GetTotalSeconds()/(60.*60.);
				
				if( IsHandicapped() )
					{
					BESTDIST=max(cSummary.m_dBestHdcapDistFinisher,cSummary.m_dBestHdcapSpeed*dMinTimeHours);
    				pcScoreRecord->m_dPoints=	dMaxDistPoints * 
    											pcScoreRecord->m_dHandicappedDistance/BESTDIST;
					}
				else
					{
					BESTDIST=max(cSummary.m_dBestDistanceFinisher,cSummary.m_dBestSpeed*dMinTimeHours);
    				pcScoreRecord->m_dPoints=	dMaxDistPoints * 
    											pcScoreRecord->m_dDistance/BESTDIST;
					}
				pcScoreRecord->m_dPoints=	min( pcScoreRecord->m_dPoints, dMaxDistPoints );

				}
			}
		}

		if( !pcTask->IsFAITask() ) 
			{
			// Don't add the FAI airport Bonus here because this is the provisional point score.  Do this later.
			if( pcScoreRecord->m_uPenalityMask & WSP_AIRFIELDBONUS )
				{
   				pcScoreRecord->m_dPoints+=m_iAirfieldBonusPoints;    
				}

			//	Now as a "final calculation step" roundoff the points
			// But not now for FAI, because this is the provisional point score.  Do this later.
			pcScoreRecord->m_dPoints=Roundoff(pcScoreRecord->m_dPoints);
			}	
  
	return;
	}
/*
double CWSClass::GetNoFinisherPoints(ETaskType eTaskType)
{
	if( eTaskType==eAssigned )
		return m_dNoFinisherPoints_ASGNED;
	else
		return m_dNoFinisherPoints_POST;
}
*/

double CWSClass::GetCompletionRatio(ETaskType eTaskType, CSummary &cSummary)
	{
	
	if( cSummary.m_nContestants==0 ) return 1.0;

	if( eTaskType==eFAIRacing ) return 1.0;

	int nFinishers=0;

	if( cSummary.m_nFinishers==0 && cSummary.m_nGuestFinishers>0 )
		nFinishers=cSummary.m_nGuestFinishers;
	else
		nFinishers=cSummary.m_nFinishers;

	if( IsUseScoredCompletionRatio() && eTaskType!=eAssigned )
		return ((double)(nFinishers)-(.75*(double)cSummary.m_nFinTocGT15underMinTime))/(double)cSummary.m_nContestants;
	else
		return (double)(nFinishers)/(double)cSummary.m_nContestants;
	}


double CWSClass::GetMaximumDistancePoints(ETaskType eTaskType, CSummary &cSummary)
	{
    if( eTaskType==eFAIRacing )
		return 1000.;
	else if( eTaskType==eAssigned )
		return min( GetMaximumSpeedPoints(eTaskType,cSummary)*(m_dMDP_B_ASGNED +m_dMDP_M_ASGNED*GetCompletionRatio(eTaskType,cSummary)), 1000.);
	else
		return min( GetMaximumSpeedPoints(eTaskType,cSummary)*(m_dMDP_B_POST +m_dMDP_M_POST*GetCompletionRatio(eTaskType,cSummary)), 1000.);
	}


#endif


CString		CWSClass::GetName(bool bWithClass)
	{
	if( bWithClass ) 
		{
		if( m_strName.Find(_T(" Class"))>0 ) 
			return m_strName; 
		else
			return m_strName+_T(" Class");
		}
	else// No class
		{
		if( m_strName.Find(_T(" Class"))<=0 ) 
			return m_strName; 
		else
			{
			CString strName=m_strName;
			strName.Replace(_T(" Class"),_T(""));
			return strName;
			}
		}
	}


	void		CWSClass::SetHandicapped(bool b)  
		{
		if( b )
			m_iOptions|=WSC_HANDICAPPED;
		else
			if( IsHandicapped() ) m_iOptions^=WSC_HANDICAPPED;
		}

	void		CWSClass::SetActive(bool b)
		{
		if( b )
			m_iOptions|=WSC_ACTIVE;
		else
			if( IsActive() ) m_iOptions^=WSC_ACTIVE;
		}

	void		CWSClass::SetUseShortTaskFactor(bool b) 
		{
		if( b )
			m_iOptions|=WSC_SHORTTASKFACTOR;
		else
			if( IsUseShortTaskFactor() ) m_iOptions^=WSC_SHORTTASKFACTOR;
		}

	void		CWSClass::SetUseScoredCompletionRatio(bool b)
		{
		if( b )
			m_iOptions|=WSC_COMPLETIONRATIO;
		else
			if( IsUseScoredCompletionRatio() ) m_iOptions^=WSC_COMPLETIONRATIO;
		}

	void		CWSClass::SetUsingWDSA(bool b)
		{
		if( b )
			m_iOptions|=WSC_USINGWDSA;
		else
			if( IsUsingWDSA() ) m_iOptions^=WSC_USINGWDSA;
		}


void CWSClass::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode)
	{
	MSXML2::IXMLDOMNodePtr pIDOMChildNode=NULL;
	cMgr.CreateChild( pParentNode, pIDOMChildNode, _T("Class"));

	cMgr.CreateElement( pIDOMChildNode, _T("Name"),			LPCSTR(m_strName) );
	cMgr.CreateElementInt( pIDOMChildNode, _T("ClassID"),			(WORD)m_eClass);
	cMgr.CreateElementInt( pIDOMChildNode, _T("Handicapped"),		(WORD)IsHandicapped());
	cMgr.CreateElementInt( pIDOMChildNode, _T("UseShortTask"),	(WORD)IsUseShortTaskFactor());
	cMgr.CreateElementInt( pIDOMChildNode, _T("UseComplRatio"),	(WORD)IsUseScoredCompletionRatio());
	cMgr.CreateElementInt( pIDOMChildNode, _T("UseingWDSA"),		(WORD)IsUsingWDSA());
	cMgr.CreateElementInt( pIDOMChildNode, _T("AirfieldBonus"),	m_iAirfieldBonusPoints);
	cMgr.CreateElementInt( pIDOMChildNode, _T("NoFinisherPoints_ASGNED"), (int)m_dNoFinisherPoints_ASGNED);
	cMgr.CreateElementDbl( pIDOMChildNode, _T("MDP_M_ASGNED"),	m_dMDP_M_ASGNED);
	cMgr.CreateElementDbl( pIDOMChildNode, _T("MDP_B_ASGNED"),	m_dMDP_B_ASGNED);
	cMgr.CreateElementInt( pIDOMChildNode, _T("MSP_M_ASGNED"),	(int)m_dMSP_M_ASGNED);
	cMgr.CreateElementInt( pIDOMChildNode, _T("MSP_B_ASGNED"),	(int)m_dMSP_B_ASGNED);
	cMgr.CreateElementInt( pIDOMChildNode, _T("NoFinisherPoints_POST"), (int)m_dNoFinisherPoints_POST);
	cMgr.CreateElementDbl( pIDOMChildNode, _T("MDP_M_POST"),		m_dMDP_M_POST);
	cMgr.CreateElementDbl( pIDOMChildNode, _T("MDP_B_POST"),		m_dMDP_B_POST);
	cMgr.CreateElementInt( pIDOMChildNode, _T("MSP_M_POST"),		(int)m_dMSP_M_POST);
	cMgr.CreateElementInt( pIDOMChildNode, _T("MSP_B_POST"),		(int)m_dMSP_B_POST);
	cMgr.CreateElementDbl( pIDOMChildNode, _T("SMTD"),			m_dSMTD);
	cMgr.CreateElementInt( pIDOMChildNode, _T("SMTT"),			(int)m_cSMTT.GetTotalSeconds());
	cMgr.CreateElementInt( pIDOMChildNode, _T("ContestType"),		(WORD)m_eContest);
	cMgr.CreateElementDbl( pIDOMChildNode, _T("D1"),			m_dD1);
	cMgr.CreateElementDbl( pIDOMChildNode, _T("Dm"),			m_dDm);
	}

void CWSClass::ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pIDOMChildNode)
	{
	bool b;
	if( pIDOMChildNode==NULL ) return;

	GET_XML_INT( cMgr, pIDOMChildNode, _T("ContestType"), EContest,	m_eContest, eRegional);
	GET_XML_INT( cMgr, pIDOMChildNode, _T("ClassID"),		EClass,		m_eClass,	eStandard);

	Initialize( m_eContest, m_eClass);

	GET_XML_STR ( cMgr, pIDOMChildNode, _T("Name"),	m_strName );
	GET_XML_BOOL( cMgr, pIDOMChildNode, _T("Handicapped"), b, IsHandicapped());   SetHandicapped(b);
	GET_XML_BOOL( cMgr, pIDOMChildNode, _T("UseShortTask"), b, IsUseShortTaskFactor() ); SetUseShortTaskFactor(b);
	GET_XML_BOOL( cMgr, pIDOMChildNode, _T("UseComplRatio"),  b, IsUseScoredCompletionRatio()); SetUseScoredCompletionRatio(b);
	GET_XML_BOOL( cMgr, pIDOMChildNode, _T("UseingWDSA"), b, IsUsingWDSA()); SetUsingWDSA(b);
	GET_XML_INT ( cMgr, pIDOMChildNode, _T("AirfieldBonus"), int, m_iAirfieldBonusPoints, m_iAirfieldBonusPoints);
	GET_XML_DBL ( cMgr, pIDOMChildNode, _T("NoFinisherPoints_ASGNED"), double, m_dNoFinisherPoints_ASGNED, m_dNoFinisherPoints_ASGNED);
	GET_XML_DBL ( cMgr, pIDOMChildNode, _T("MDP_M_ASGNED"),  double, m_dMDP_M_ASGNED, m_dMDP_M_ASGNED);
	GET_XML_DBL ( cMgr, pIDOMChildNode, _T("MDP_B_ASGNED"),  double, m_dMDP_B_ASGNED, m_dMDP_B_ASGNED);
	GET_XML_DBL ( cMgr, pIDOMChildNode, _T("MSP_M_ASGNED"),  double, m_dMSP_M_ASGNED, m_dMSP_M_ASGNED);
	GET_XML_DBL ( cMgr, pIDOMChildNode, _T("MSP_B_ASGNED"),  double, m_dMSP_B_ASGNED, m_dMSP_B_ASGNED);
	GET_XML_DBL ( cMgr, pIDOMChildNode, _T("NoFinisherPoints_POST"),  double, m_dNoFinisherPoints_POST, m_dNoFinisherPoints_POST);
	GET_XML_DBL ( cMgr, pIDOMChildNode, _T("MDP_M_POST"),  double, m_dMDP_M_POST, m_dMDP_M_POST);
	GET_XML_DBL ( cMgr, pIDOMChildNode, _T("MDP_B_POST"),  double, m_dMDP_B_POST, m_dMDP_B_POST);
	GET_XML_DBL ( cMgr, pIDOMChildNode, _T("MSP_M_POST"),  double, m_dMSP_M_POST, m_dMSP_M_POST);
	GET_XML_DBL ( cMgr, pIDOMChildNode, _T("MSP_B_POST"),  double, m_dMSP_B_POST, m_dMSP_B_POST);
	GET_XML_DBL ( cMgr, pIDOMChildNode, _T("SMTD"),		 double, m_dSMTD, m_dSMTD);
	GET_XML_INT ( cMgr, pIDOMChildNode, _T("SMTT"),		CTimeSpan, m_cSMTT, m_cSMTT);
	GET_XML_DBL ( cMgr, pIDOMChildNode, _T("D1"),  double, m_dD1, m_dD1);
	GET_XML_DBL ( cMgr, pIDOMChildNode, _T("Dm"),  double, m_dDm, m_dDm);
	}

