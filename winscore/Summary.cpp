//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// Summary.cpp: implementation of the CSummary class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "winscore.h"
#include "wsclass.h"
#include "Summary.h"
#include "global_prototypes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSummary::CSummary() :

	m_dTaskDistance(0),
	m_dBestSpeed(0),
	m_dBestScoredSpeed(0),
	m_dSlowestSpeed(10000),
	m_dSlowestScoredSpeed(10000),
	m_dBestDistance(0),
	m_dBestHdcapSpeed(0),
	m_dBestScoredHdcapSpeed(0),
	m_dSlowestHdcapSpeed(0),
	m_dSlowestScoredHdcapSpeed(0),
	m_dBestHdcapDist(0),
	m_dDistaceOfFinisherWithBestSpeed(0),
	m_cTOCBest(0),
	m_cTOCBestSpeed(0),
	m_cTOCBestHdcapSpeed(0),
	m_nContestants(0),
	m_nFinishers(0),
	m_nGuestFinishers(0),
	m_nFinTocGT15underMinTime(0),
	m_nGTMinDistance(0),
	m_dBestHdcapDistFinisher(0),
	m_dBestDistanceFinisher(0),
	m_strCIDBestSpeed(_T("")), 
	m_strCIDSlowestSpeed(_T("")), 
	m_strCIDBestDistance(_T("")),
	m_strCIDBestHCSpeed(_T("")), 
	m_strCIDSlowestHCSpeed(_T("")), 
	m_strCIDBestHCDistance(_T("")),
	m_dDt(0.),
	m_cTd(0),
	m_dD1(0.),
	m_dDm(0.),
	m_in1(0),
	m_in2(0),
	m_in3(0),
	m_in4(0),
	m_iN(0),
	m_dHo(0.0),
	m_dDo(0.),
	m_dVo(0.),
	m_cTo(0),
	m_dPm(0.),
	m_dPdm(0.),
	m_dPvm(0.),
	m_dF(1.0),
	m_dFCR(1.0),
	m_dSpo(0.0),
	m_dSpm(0.0)
{

}

void CSummary::Initialize() 
	{
	m_dTaskDistance=0;
	m_dBestSpeed=0;
	m_dBestScoredSpeed=0;
	m_dSlowestSpeed=10000;
	m_dSlowestScoredSpeed=10000;
	m_dBestDistance=0;
	m_dBestHdcapSpeed=0;
	m_dBestScoredHdcapSpeed=0;
	m_dSlowestHdcapSpeed=0;
	m_dSlowestScoredHdcapSpeed=0;
	m_dBestHdcapDist=0;
	m_dDistaceOfFinisherWithBestSpeed=0;
	m_cTOCBest=0;
	m_cTOCBestSpeed=0;
	m_cTOCBestHdcapSpeed=0;
	m_nContestants=0;
	m_nFinishers=0;
	m_nGuestFinishers=0;
	m_nFinTocGT15underMinTime=0;
	m_nGTMinDistance=0;
	m_dBestHdcapDistFinisher=0;
	m_dBestDistanceFinisher=0;
	m_strCIDBestSpeed=_T(""); 
	m_strCIDSlowestSpeed=_T(""); 
	m_strCIDBestDistance=_T("");
	m_strCIDBestHCSpeed=_T(""); 
	m_strCIDSlowestHCSpeed=_T(""); 
	m_strCIDBestHCDistance=_T("");
	m_dDt=0.;
	m_cTd=0;
	m_dD1=0.;
	m_dDm=0.;
	m_in1=0;
	m_in2=0;
	m_in3=0;
	m_in4=0;
	m_iN=0;
	m_dHo=0.0;
	m_dDo=0.;
	m_dVo=0.;
	m_cTo=0;
	m_dPm=0.;
	m_dPdm=0.;
	m_dPvm=0.;
	m_dF=1.0;
	m_dFCR=1.0;
	m_dSpo=0.0;
	m_dSpm=0.0;
	m_strCIDHo=_T("");
	m_strCIDDo=_T("");
	m_strCIDVo=_T("");
}


CSummary::~CSummary()
{

}

void CSummary::SetTaskDistance(double dDist)
	{
	m_dTaskDistance=dDist;
	m_dDt=dDist;
	}

void CSummary::Output(CTask *pcTask, EUnits eUnits, bool bIsHandicapped, CWinscoreListCtrl &cStatus )
{
		CString strSummary;
		CString cUnitsStr=UnitsText(eUnits);

		if( pcTask->IsFAITask() ) 
			{
			if( pcTask->m_eType==eFAIRacing )
				{
    			strSummary.Format(_T("%6.3lf "), ConvertDistance(m_dDt, SYSTEMUNITS, eUnits ) );
    			strSummary+=cUnitsStr;
    			cStatus.AddTwoStringItem(_T("Dt - Task Distance"),strSummary);
				}


    		strSummary.Format(_T("%6.3lf %s"), ConvertDistance(m_dD1, SYSTEMUNITS, eUnits ), cUnitsStr );
    		cStatus.AddTwoStringItem(_T("D1 -  Minimum Distance for 1000 points"),strSummary);

    		strSummary.Format(_T("%6.3lf %s"), ConvertDistance(m_dDm, SYSTEMUNITS, eUnits ), cUnitsStr );
    		cStatus.AddTwoStringItem(_T("Dm -  Minimum Distance to validate the Day"),strSummary);

    		strSummary.Format(_T("%d"),	m_in1 );
    		cStatus.AddTwoStringItem(_T("n1 - Number of competitors who achieve a Handicapped Distance (Dh) of at least Dm"),strSummary);

    		strSummary.Format(_T("%d"),	m_in2 );
    		cStatus.AddTwoStringItem(_T("n2 - Number of finishers exceeding 2/3 of best Handicapped Speed (Vo)."),strSummary);

    		strSummary.Format(_T("%d"),	m_in3 );
    		cStatus.AddTwoStringItem(_T("n3 - Number of finishers, regardless of speed"),strSummary);

    		strSummary.Format(_T("%d"),	m_in4 );
    		cStatus.AddTwoStringItem(_T("n4 - Number of competitors who achieve a Handicapped Distance (Dh) of at least Dm/2"),strSummary);

    		strSummary.Format(_T("%d"),	m_iN );
    		cStatus.AddTwoStringItem(_T("N - Number of competitors having had a competition launch that Day"),strSummary);

    		strSummary.Format(_T("%6.3lf %s  (%s) "), ConvertDistance(m_dDo, SYSTEMUNITS, eUnits ),cUnitsStr,m_strCIDDo );
    		cStatus.AddTwoStringItem(_T("Do -  Highest Handicapped Distance (Dh) of the Day"),strSummary);

    		strSummary.Format(_T("%6.3lf %s/Hr (%s)"), ConvertDistance(m_dVo, SYSTEMUNITS, eUnits ), cUnitsStr, m_strCIDVo );
    		cStatus.AddTwoStringItem(_T("Highest finisher’s Handicapped Speed (Vh) of the Day"),strSummary);

    		strSummary.Format(_T("%6.2lf"),	m_dPm );
    		cStatus.AddTwoStringItem(_T("Pm -  Maximum available Score for the Day, before F and FCR are applied."),strSummary);

    		strSummary.Format(_T("%6.2lf"),	m_dPdm);
    		cStatus.AddTwoStringItem(_T("Pdm -  Maximum available Distance Points for the Day, before F and FCR are applied."),strSummary);

    		strSummary.Format(_T("%6.2lf"),	m_dPvm);
    		cStatus.AddTwoStringItem(_T("Pvm -  Maximum available Speed Points for the Day, before F and FCR are applied."),strSummary);

    		strSummary.Format(_T("%6.3lf "), m_dF );
    		cStatus.AddTwoStringItem(_T("F - Day Factor"),strSummary);

    		strSummary.Format(_T("%6.3lf "), m_dFCR );
    		cStatus.AddTwoStringItem(_T("FCR -  Completion Ratio Factor."),strSummary);

    		strSummary.Format(_T("%6.3lf "), m_dSpo );
    		cStatus.AddTwoStringItem(_T("Spo - Highest Provisional Score (SP) of the Day."),strSummary);

    		strSummary.Format(_T("%6.3lf "), m_dSpm );
    		cStatus.AddTwoStringItem(_T("Spm - Median Provisional Score (SP) of the Day."),strSummary);


    		strSummary.Format(_T("%6.3lf "), min(1., 200./( m_dSpo - m_dSpm)) );
    		cStatus.AddTwoStringItem(_T("min(1,200/( Spo - Spm))"),strSummary);
			if( pcTask->IsTimedTask() )
				{
    			strSummary=m_cTd.Format(_T(" %H:%M:%S ") );
    			cStatus.AddTwoStringItem(_T("Td - Minimum Task Time."),strSummary);
				}

    		strSummary=m_cTo.Format(_T(" %H:%M:%S ") );
    		cStatus.AddTwoStringItem(_T("To - Marking Time (T) of the finisher whose Vh = Vo."),strSummary);

    		double dPercent=0.0;

    		if( m_iN>0 )
    			dPercent= ((double)m_nGTMinDistance/(double)m_iN)*100.;
    
    		CString cPercent;
    		if( dPercent<25. )
    			cPercent.Format(_T("%5.1lf%% - INSUFFICIENT FOR OFFICIAL DAY"), dPercent);
    		else
    			cPercent.Format(_T("%5.1lf%%"), dPercent);
    
    		cStatus.AddTwoStringItem(_T("Num Contestants(N)>Min Distance(Dm)"),cPercent);

			}
		else //SSA Task
			{
    		double dPercent=0.0;
    		if( m_nContestants>0 )
    			dPercent= ((double)m_nGTMinDistance/(double)m_nContestants)*100.;
    
    		CString cPercent;
    		if( dPercent<25. )
    			cPercent.Format(_T("%5.1lf%% - INSUFFICIENT FOR OFFICIAL DAY"), dPercent);
    		else
    			cPercent.Format(_T("%5.1lf%%"), dPercent);
    
    		cStatus.AddTwoStringItem(_T("Contestants>Min Distance"),cPercent);
    
   
    		// List Best Speed
    		if( m_dBestSpeed>0.0) 
    			{
    			strSummary.Format(_T("%s with %6.3lf "), m_strCIDBestSpeed, ConvertDistance(m_dBestSpeed, SYSTEMUNITS, eUnits ) );
    			strSummary+=cUnitsStr+_T("/Hr");
    			cStatus.AddTwoStringItem(_T("Best Speed"),strSummary);
    			}
    
    		// List Best Speed
    		strSummary.Format(_T("%s with %6.3lf "), m_strCIDBestDistance, ConvertDistance(m_dBestDistance, SYSTEMUNITS, eUnits ) );
    		strSummary+=cUnitsStr;
    		cStatus.AddTwoStringItem(_T("Best Distance"),strSummary);
    
    		if( bIsHandicapped )
    			{
    			// List Best HC Speed
    			if( m_dBestHdcapSpeed>0.0) 
    				{
    				strSummary.Format(_T("%s with %6.3lf "), m_strCIDBestHCSpeed, ConvertDistance(m_dBestHdcapSpeed, SYSTEMUNITS, eUnits ) );
    				strSummary+=cUnitsStr+_T("/Hr");
    				cStatus.AddTwoStringItem(_T("Best Hdcap Speed"),strSummary);
    				}
    
    			// List Best HC Dist
    			strSummary.Format(_T("%s with %6.3lf "), m_strCIDBestHCDistance, ConvertDistance(m_dBestHdcapDist, SYSTEMUNITS, eUnits ) );
    			strSummary+=cUnitsStr;
    			cStatus.AddTwoStringItem(_T("Best Hdcap Distance"),strSummary);
    			}
    
    
    		strSummary=m_cTOCBestSpeed.Format(_T(" %H:%M:%S ") );
    		cStatus.AddTwoStringItem(_T("Time On Course (Best Speed)"),strSummary);

    
    		strSummary.Format(_T("%d"),	m_nContestants );
    		cStatus.AddTwoStringItem(_T("Number Of Contestants"),strSummary);
    
    		strSummary.Format(_T("%d"),	m_nFinishers );
    		cStatus.AddTwoStringItem(_T("Number Of Finishers"),strSummary);
       
			if( bIsHandicapped )
				{
    			strSummary=m_cTOCBestHdcapSpeed.Format(_T(" %H:%M:%S ") );
    			cStatus.AddTwoStringItem(_T("Time On Course (Best HdCap Speed)"),strSummary);
				}
			double BESTDIST=.0;
			double dMinTimeHours=(double)pcTask->m_cPostTime.GetTotalSeconds()/(60.*60.);
			if( bIsHandicapped )
				{
				if(m_nFinishers==0 ) 
					BESTDIST=m_dBestHdcapDist;
				else
					BESTDIST=m_dBestHdcapDistFinisher;
				}
			else
				{
				if( m_nFinishers==0 )
					BESTDIST=m_dBestDistance;
				else
					BESTDIST=m_dBestDistanceFinisher;
				}
    		strSummary.Format(_T("%6.3lf "), ConvertDistance(BESTDIST, SYSTEMUNITS, eUnits ) );
    		strSummary+=cUnitsStr;
			cStatus.AddTwoStringItem(_T("BESTDIST"),strSummary);
			}


}


bool CSummary::GetXML(CXMLMgr &cMgr,MSXML2::IXMLDOMNodePtr pIDOMNode, CTask *pcTask, EUnits eUnits, bool bIsHandicapped )
	{

		CString strSummary;
		CString cUnitsStr=UnitsText(eUnits);


		if( pcTask->IsFAITask() ) 
			{
			cMgr.CreateElement(	pIDOMNode, _T("TaskRules"),	"FAI" );
			if( pcTask->m_eType==eFAIRacing )
				{
    			strSummary.Format(_T("%6.3lf "), ConvertDistance(m_dDt, SYSTEMUNITS, eUnits ) );
    			strSummary+=cUnitsStr;
				cMgr.CreateElement(	pIDOMNode, _T("Dt"),	strSummary );
				}

			//if( eUnits==eKilometers )
    			strSummary.Format(_T("%6.3lf %s"), ConvertDistance(m_dD1, SYSTEMUNITS, eUnits ), cUnitsStr );
			//else
//    			strSummary.Format(_T("%6.3lf %s  (%6.2lf %s)"), ConvertDistance(m_dD1, SYSTEMUNITS, eUnits ), cUnitsStr,  ConvertDistance(m_dD1, SYSTEMUNITS, eKilometers ), UnitsText(eKilometers) );
			cMgr.CreateElement(	pIDOMNode, _T("D1"),	strSummary );


//			if( eUnits==eKilometers )
    			  strSummary.Format(_T("%6.3lf %s"), ConvertDistance(m_dDm, SYSTEMUNITS, eUnits ), cUnitsStr );
//			else
//		   		  strSummary.Format(_T("%6.3lf %s  (%6.2lf %s)"), ConvertDistance(m_dDm, SYSTEMUNITS, eUnits ), cUnitsStr,  ConvertDistance(m_dDm, SYSTEMUNITS, eKilometers ), UnitsText(eKilometers) );
			cMgr.CreateElement(	pIDOMNode, _T("Dm"),	strSummary );

    		strSummary.Format(_T("%d"),	m_in1 );
 			cMgr.CreateElement(	pIDOMNode, _T("n1"),	strSummary );

    		strSummary.Format(_T("%d"),	m_in2 );
			cMgr.CreateElement(	pIDOMNode, _T("n2"),	strSummary );

    		strSummary.Format(_T("%d"),	m_in3 );
			cMgr.CreateElement(	pIDOMNode, _T("n3"),	strSummary );

    		strSummary.Format(_T("%d"),	m_in4 );
			cMgr.CreateElement(	pIDOMNode, _T("n4"),	strSummary );

    		strSummary.Format(_T("%d"),	m_iN );
			cMgr.CreateElement(	pIDOMNode, _T("N"),	strSummary );

    		strSummary.Format(_T("%6.3lf %s  (%s) "), ConvertDistance(m_dDo, SYSTEMUNITS, eUnits ),cUnitsStr,m_strCIDDo );
 			cMgr.CreateElement(	pIDOMNode, _T("Do"),	strSummary );

    		strSummary.Format(_T("%6.3lf %s/Hr (%s)"), ConvertDistance(m_dVo, SYSTEMUNITS, eUnits ), cUnitsStr, m_strCIDVo );
			cMgr.CreateElement(	pIDOMNode, _T("Vo"),	strSummary );

    		strSummary.Format(_T("%6.2lf"),	m_dPm );
			cMgr.CreateElement(	pIDOMNode, _T("Pm"),	strSummary );

    		strSummary.Format(_T("%6.2lf"),	m_dPdm);
			cMgr.CreateElement(	pIDOMNode, _T("Pdm"),	strSummary );

    		strSummary.Format(_T("%6.2lf"),	m_dPvm);
			cMgr.CreateElement(	pIDOMNode, _T("Pvm"),	strSummary );

    		strSummary.Format(_T("%6.3lf "), m_dF );
			cMgr.CreateElement(	pIDOMNode, _T("F"),	strSummary );

    		strSummary.Format(_T("%6.3lf "), m_dFCR );
			cMgr.CreateElement(	pIDOMNode, _T("FCR"),	strSummary );

    		strSummary.Format(_T("%6.3lf "), m_dSpo );
			cMgr.CreateElement(	pIDOMNode, _T("Spo"),	strSummary );

    		strSummary.Format(_T("%6.3lf "), m_dSpm );
			cMgr.CreateElement(	pIDOMNode, _T("Spm"),	strSummary );

			if( pcTask->IsTimedTask() )
				{
    			strSummary=m_cTd.Format(_T(" %H:%M:%S ") );
			    cMgr.CreateElement(	pIDOMNode, _T("Td"),	strSummary );
				}

    		strSummary=m_cTo.Format(_T(" %H:%M:%S ") );
			cMgr.CreateElement(	pIDOMNode, _T("To"),	strSummary );

    		double dPercent=0.0;

    		if( m_iN>0 )
    			dPercent= ((double)m_nGTMinDistance/(double)m_iN)*100.;
    
    		CString cPercent;
    		if( dPercent<25. )
    			cPercent.Format(_T("%5.1lf%% - INSUFFICIENT FOR OFFICIAL DAY"), dPercent);
    		else
    			cPercent.Format(_T("%5.1lf%%"), dPercent);
    
   			cMgr.CreateElement(	pIDOMNode, _T("NumContestantsGTMinDist"),	cPercent );
			}
		else //SSA Task
			{
			cMgr.CreateElement(	pIDOMNode, _T("TaskRules"),	"SSA" );
    		double dPercent=0.0;
    		if( m_nContestants>0 )
    			dPercent= ((double)m_nGTMinDistance/(double)m_nContestants)*100.;
    
    		CString cPercent;
    		if( dPercent<25. )
    			cPercent.Format(_T("%5.1lf%% - INSUFFICIENT FOR OFFICIAL DAY"), dPercent);
    		else
    			cPercent.Format(_T("%5.1lf%%"), dPercent);
    
			cMgr.CreateElement(	pIDOMNode, _T("ContestantsGTMinDist"),	cPercent );
   
   
    		// List Best Speed
    		if( m_dBestSpeed>0.0) 
    			{
    			strSummary.Format(_T("%s with %6.3lf "), m_strCIDBestSpeed, ConvertDistance(m_dBestSpeed, SYSTEMUNITS, eUnits ) );
    			strSummary+=cUnitsStr+_T("/Hr");
				cMgr.CreateElement(	pIDOMNode, _T("BestSpeed"),	strSummary );
    			}
    
    		// List Best Speed
    		strSummary.Format(_T("%s with %6.3lf "), m_strCIDBestDistance, ConvertDistance(m_dBestDistance, SYSTEMUNITS, eUnits ) );
    		strSummary+=cUnitsStr;
 			cMgr.CreateElement(	pIDOMNode, _T("BestDist"),	strSummary );

    		if( bIsHandicapped )
    			{
    			// List Best HC Speed
    			if( m_dBestHdcapSpeed>0.0) 
    				{
    				strSummary.Format(_T("%s with %6.3lf "), m_strCIDBestHCSpeed, ConvertDistance(m_dBestHdcapSpeed, SYSTEMUNITS, eUnits ) );
    				strSummary+=cUnitsStr+_T("/Hr");
					cMgr.CreateElement(	pIDOMNode, _T("BestHdcapSpeed"),	strSummary );
    				}
    
    			// List Best HC Dist
    			strSummary.Format(_T("%s with %6.3lf "), m_strCIDBestHCDistance, ConvertDistance(m_dBestHdcapDist, SYSTEMUNITS, eUnits ) );
    			strSummary+=cUnitsStr;
				cMgr.CreateElement(	pIDOMNode, _T("BestHdcapDist"),	strSummary );
    			}
    
    
    		strSummary=m_cTOCBestSpeed.Format(_T(" %H:%M:%S ") );
			cMgr.CreateElement(	pIDOMNode, _T("TOCBestSpeed"),	strSummary );

    
    		strSummary.Format(_T("%d"),	m_nContestants );
			cMgr.CreateElement(	pIDOMNode, _T("NumberOfContestants"),	strSummary );
    
    		strSummary.Format(_T("%d"),	m_nFinishers );
			cMgr.CreateElement(	pIDOMNode, _T("NumberOfFinishers"),	strSummary );
     
    		strSummary.Format(_T("%d"),	m_nFinTocGT15underMinTime );
			cMgr.CreateElement(	pIDOMNode, _T("NumberOfFinishers15"),	strSummary );

    		strSummary.Format(_T("%d"),	m_nGTMinDistance );
			cMgr.CreateElement(	pIDOMNode, _T("NumberGTMinDist"),	strSummary );

			if( bIsHandicapped )
				{
    			strSummary=m_cTOCBestHdcapSpeed.Format(_T(" %H:%M:%S ") );
				cMgr.CreateElement(	pIDOMNode, _T("TOCBestHdCapSpeed"),	strSummary );
				}
			double BESTDIST=0.0;
			double dMinTimeHours=(double)pcTask->m_cPostTime.GetTotalSeconds()/(60.*60.);

			if( bIsHandicapped )
				{
				if(m_nFinishers==0 ) 
					BESTDIST=m_dBestHdcapDist;
				else
					BESTDIST=m_dBestHdcapDistFinisher;
				}
			else
				{
				if( m_nFinishers==0 )
					BESTDIST=m_dBestDistance;
				else
					BESTDIST=m_dBestDistanceFinisher;
				}
    		strSummary.Format(_T("%6.3lf "), ConvertDistance(BESTDIST, SYSTEMUNITS, eUnits ) );
    		strSummary+=cUnitsStr;
			cMgr.CreateElement(	pIDOMNode, _T("BESTDIST"),	strSummary );
			}

	return true;
	}