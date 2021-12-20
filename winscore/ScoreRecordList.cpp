//
// Copyright© Guy Ford Byars. 1997 - 2001. All rights reserved.
 //
// ScoreRecordList.cpp: implementation of the CScoreRecordList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Winscore.h"
#include "ScoreRecordList.h"
#include "Global_Prototypes.h"
#include "median.h"
#include <Atlbase.h>
#include <vector>
#include <algorithm>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static ESortBy eStaticSortBy;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CScoreRecordList::CScoreRecordList()
{

}

CScoreRecordList::~CScoreRecordList()
{
	if( IsEmpty() !=0  )  return;
	Purge();
	return;
}




CScoreRecord* CScoreRecordList::GetNext(POSITION& pos, CTime cDate, EClass eClass)
		{
		if( IsEmpty() ) return NULL;

		while( pos!=NULL)
			{
			CScoreRecord* pcScoreRecord=(CScoreRecord*)CObList::GetNext(pos);
			if( DatesEqual(pcScoreRecord->m_cDate,cDate)		&&
				pcScoreRecord->m_eClass	==eClass		) 
				{
				return pcScoreRecord;
				}
			}
		return NULL;
		}

void CScoreRecordList::Purge(EClass eClass, CTime cDate)
	{
	if( !IsEmpty() ) 
		{
		POSITION thispos, pos = GetHeadPosition();
		while( pos!=NULL)
			{
			thispos=pos;
			CScoreRecord* pcScoreRecord=(CScoreRecord*)CObList::GetNext(pos);
			if( DatesEqual(pcScoreRecord->m_cDate,cDate)	&&
				pcScoreRecord->m_eClass	==eClass	) 
				{
				delete pcScoreRecord;
				CObList::RemoveAt( thispos );
				}
			}
		}
	}



CScoreRecord* CScoreRecordList::GetNext(POSITION& pos, CString cContestNo)
		{
		if( IsEmpty() ) return NULL;

		while( pos!=NULL)
			{
			CScoreRecord* pcScoreRecord=(CScoreRecord*)CObList::GetNext(pos);
			if( pcScoreRecord->m_cContestNo	==cContestNo		) 
				{
				return pcScoreRecord;
				}
			}
		return NULL;
		}

CScoreRecord* CScoreRecordList::GetNext(POSITION& pos, CTime cDate, CString cContestNo)
		{
		if( IsEmpty() ) return NULL;

		while( pos!=NULL)
			{
			CScoreRecord* pcScoreRecord=(CScoreRecord*)CObList::GetNext(pos);
			if( DatesEqual(pcScoreRecord->m_cDate, cDate)		&&
				pcScoreRecord->m_cContestNo	==cContestNo		) 
				{
				return pcScoreRecord;
				}
			}
		return NULL;
		}

CScoreRecord* CScoreRecordList::GetNext(POSITION& pos, CTime cDate)
		{
		if( IsEmpty() ) return NULL;

		while( pos!=NULL)
			{
			CScoreRecord* pcScoreRecord=(CScoreRecord*)CObList::GetNext(pos);
			if( DatesEqual(pcScoreRecord->m_cDate,cDate) )
				{
				return pcScoreRecord;
				}
			}
		return NULL;
		}


void CScoreRecordList::StoreData(
					CString		cContestNo,
					CTime		cDate,
					EClass		eClass,
					bool		bGuest,
					double		dHandicap,
					BOOL		fContestant,
					BOOL		fFinisher,
					CTime		cStartTime,
					CTime		cFinishTime,
					CTimeSpan	cSTI,
					double		dDistance,
					double 		dSpeed,
					double 		dScoredSpeed,
					double 		dHdcapDist,
					double 		dHdcapSpeed,
					double 		dScoredHdcapSpeed,
					int			nAcheivedTurnpoints,
					int			uPenalityMask
					)
	{

	CScoreRecord* pcNewScoreRecord = new CScoreRecord(	
					cContestNo,
					cDate,
					eClass,
					bGuest,
					dHandicap,
					fContestant,
					fFinisher,
					cStartTime,
					cFinishTime,
					cSTI,
					dDistance,
					dSpeed,
					dScoredSpeed,
					dHdcapDist,
					dHdcapSpeed,
					dScoredHdcapSpeed,
					nAcheivedTurnpoints,
					uPenalityMask);


	//  If a ScoreRecord with this date is there already, remove it
	if( !IsEmpty() ) 
		{
		POSITION thispos, pos = GetHeadPosition();
		while( pos!=NULL)
			{
			thispos=pos;
			CScoreRecord* pcScoreRecord=(CScoreRecord*)CObList::GetNext(pos);
			if( DatesEqual(pcScoreRecord->m_cDate,cDate)		&&
				pcScoreRecord->m_cContestNo	==cContestNo	) 
				{
				delete pcScoreRecord;
				SetAt( thispos, (CScoreRecord*)pcNewScoreRecord );
				return;
				}
			}
		}
//  If we got here, it must be a new one

	AddTail( (CScoreRecord*)pcNewScoreRecord );
	}



#define NUM_COLUMNS 13

static _TCHAR *_gszColumnLabel[NUM_COLUMNS] =
{
_T("Points"), _T("Cum. Points"), _T("Contestant"), _T("ID"), _T("Name"), 
_T("Status"), _T("Distance "), _T("Speed "), 
_T("Time On Course"), _T("Start Time"), _T("Finish Time"),  
 _T("Penalty Pnts"), _T("Remarks") };
static int _gnColumnWidth[NUM_COLUMNS] = 
{
	60, 70, 70, 50, 100, 90, 80, 80, 90, 80, 100, 70, 750
};

#define NUM_SPORTS_COLUMNS 15

static _TCHAR *_gszSportsColumnLabel[NUM_SPORTS_COLUMNS] =
{
_T("Points"),			_T("Cum. Points"),	_T("Contestant"),  _T("ID"),				_T("Name"), 
_T("Status"),			_T("Distance "),	_T("Dist. (Handicapped)"),_T("Speed "),
_T("Speed (Handicapped) "),	_T("Time On Course"), _T("Start Time"),		_T("Finish Time"),  
_T("Penality Pnts"), _T("Remarks")};

static int _gnSportsColumnWidth[NUM_SPORTS_COLUMNS] = 
{
	60, 70, 70, 50, 100, 90, 80, 80, 80, 80, 90, 80, 100, 70, 750

};

void CScoreRecordList::CreateControlColumns(CListCtrl& ListCtrl, EClass eClass, EUnits eUnits)
{
	LV_COLUMN lvc;
	TCHAR buff[100];
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt =LVCFMT_LEFT;

	if( GetClass(eClass).IsHandicapped()  )
		{
		for(  int i = 0; i<NUM_SPORTS_COLUMNS; i++)
			{
			ASSERT(i<MAXCOLUMNS);
			strcpy_s(buff,100,_gszSportsColumnLabel[i]);
			if( i==6 || i==7)
				{
				strcat_s(buff,100, _T("(") );
				strcat_s(buff,100, UnitsText(eUnits) );
				strcat_s(buff,100, _T(")") );
				}
			else if( i==8 || i==9 )
				strcat_s(buff,100, UnitsSpeedText(eUnits) );
			lvc.iSubItem = i;
			lvc.pszText = buff;
			lvc.cx =  _gnSportsColumnWidth[i];
			ListCtrl.InsertColumn(i,&lvc);
			}
		}
	else
		{
		for(  int i = 0; i<NUM_COLUMNS; i++)
			{
			ASSERT(i<MAXCOLUMNS);
			strcpy_s(buff,100,_gszColumnLabel[i]);
			if( i==6 )
				{
				strcat_s(buff, 100, _T("(") );
				strcat_s(buff, 100, UnitsText(eUnits) );
				strcat_s(buff, 100, _T(")") );
				}
			else if( i==7 )
				strcat_s(buff,100, UnitsSpeedText(eUnits) );
			lvc.iSubItem = i;
			lvc.pszText = buff;
			lvc.cx =  _gnColumnWidth[i];
			ListCtrl.InsertColumn(i,&lvc);
			}
		}
	}



int CALLBACK CompareScores(LPARAM lParam1, LPARAM lParam2, 
    LPARAM iColumn)
{

	int iR= -strcmp(  ((CScoreRecord*)lParam1)->m_acColumnText[abs(iColumn)-1],
					  ((CScoreRecord*)lParam2)->m_acColumnText[abs(iColumn)-1]  );
	if( iR==0 ) // in a tie, give priority to day score
		int iR=  strcmp(  ((CScoreRecord*)lParam1)->m_acColumnText[0],
						  ((CScoreRecord*)lParam2)->m_acColumnText[0]  );


	// Always give decending sort for points
	if( abs(iColumn)-1 <=1 ) return iR;

	// Reverse sort for certain columns
	if( abs(iColumn)-1 ==3  ||
		abs(iColumn)-1 ==4  ||
		abs(iColumn)-1 ==5		) iR= -iR;

	return ( iColumn<0 )?(-iR):(iR);

}

void CScoreRecordList::Purge(bool bKeep)
	{
	if( IsEmpty() !=0  )  return;
	if( !bKeep )
		{
		POSITION pos = GetHeadPosition();
		while(pos!=NULL)
			{
			CScoreRecord* pcScoreRecord=(CScoreRecord*)CObList::GetNext(pos);
			if( pcScoreRecord ) delete pcScoreRecord;
			}
		}
	RemoveAll();
	}


int CScoreRecordList::CopyByDate(CTime &cDate, CScoreRecordList & cInputScoreRecordList)
{
	// Records are reused to destination list

	CScoreRecord *pcScoreRecord;
	int nRec=0;
	if( cInputScoreRecordList.IsEmpty() !=0  )  return nRec;

	POSITION thispos, pos = cInputScoreRecordList.GetHeadPosition();

	while(pos)
		{
		thispos=pos;
		pcScoreRecord=(CScoreRecord*)cInputScoreRecordList.GetNext(pos,cDate);
		if( pcScoreRecord ) 
			{
			AddTail(pcScoreRecord);
			nRec++;
			}
		}
	return nRec;
}



CScoreRecordList& CScoreRecordList::operator <<(CScoreRecordList & cInputScoreRecordList)
{
	// Records are transfered to destination list and purged from 
	// source list.  The score records themselves, are reused.

	CScoreRecord *pcScoreRecord;

	if( cInputScoreRecordList.IsEmpty() !=0  )  return *this;

	POSITION thispos, pos = cInputScoreRecordList.GetHeadPosition();

	while(pos)
		{
		thispos=pos;
		pcScoreRecord=(CScoreRecord*)cInputScoreRecordList.CObList::GetNext(pos);
		if( pcScoreRecord ) 
			{
			AddScore(pcScoreRecord);
			cInputScoreRecordList.RemoveAt(thispos);
			}
		}

	return *this;
}

int CScoreRecordList::CopySort(CTime cDate, 
							   EClass eClass, 
							   CScoreRecordList& cOutputScoreRecordList, 
							   ESortBy eSortBy)
{
	int nRec=0;
	bool bGuestsAtBottom=(GetWinscoreInt( GUESTPOSITION, 0 )==0)?(false):(true);

	CScoreRecord  **ppcScoreRecords;

	if( IsEmpty() !=0  )  return 0;

	int nRecords=CObList::GetCount();
	int i;
	ppcScoreRecords=new CScoreRecord*[nRecords];

	POSITION pos = GetHeadPosition();
	while(pos)
		{
		CScoreRecord* pcScoreRecord=GetNext(pos, cDate, eClass);
		if( pcScoreRecord==NULL ) continue;
		ppcScoreRecords[nRec++]=new CScoreRecord(pcScoreRecord);
		}

	if( nRec>0 )
		{
		eStaticSortBy=eDaily;

		qsort( (void*)ppcScoreRecords, nRec, sizeof(CScoreRecord*), compare_records );
		int iPrevPos=1;
		for( i=0; i<nRec; i++ ) 
			{
			if( i>0 && ppcScoreRecords[i]->m_dPoints==ppcScoreRecords[i-1]->m_dPoints)
				ppcScoreRecords[i]->m_iDayRank=iPrevPos;
			else
				ppcScoreRecords[i]->m_iDayRank=i+1;
			iPrevPos=ppcScoreRecords[i]->m_iDayRank;
			}

		eStaticSortBy=eCumulative;
		qsort( (void*)ppcScoreRecords, nRec, sizeof(CScoreRecord*), compare_records );
		for( i=0; i<nRec; i++ ) ppcScoreRecords[i]->m_iCumRank=i+1;

		eStaticSortBy=eSortBy;
		qsort( (void*)ppcScoreRecords, nRec, sizeof(CScoreRecord*), compare_records );

		int iStanding=0, iPrevStanding=(-1);
		double dPrevScore=(-1);
		for( i=0; i<nRec; i++ ) 
			{
			ppcScoreRecords[i]->m_iCumRank=i+1;

			if( !ppcScoreRecords[i]->m_bGuest )
				{
				iStanding++;
				if( i==0 ) 
					{
					dPrevScore=ppcScoreRecords[i]->m_dCumPoints;
					iPrevStanding=iStanding;
					}
				if( ppcScoreRecords[i]->m_dCumPoints!=dPrevScore )
					{
					ppcScoreRecords[i]->m_cCumRankText.Format(_T("%2d"), iStanding);
					dPrevScore=ppcScoreRecords[i]->m_dCumPoints;
					iPrevStanding=iStanding;
					}
				else
				  ppcScoreRecords[i]->m_cCumRankText.Format(_T("%2d"), iPrevStanding);

				
				}
			else
				ppcScoreRecords[i]->m_cCumRankText=_T(" G");
			}

		if( !bGuestsAtBottom )
			for( i=0; i<nRec; i++ ) cOutputScoreRecordList.AddTail(ppcScoreRecords[i]);
		else
			{
			for( i=0; i<nRec; i++ ) 
				{
				if( !ppcScoreRecords[i]->m_bGuest		)
					cOutputScoreRecordList.AddTail(ppcScoreRecords[i]);
				}
			for( i=0; i<nRec; i++ ) 
				{
				if( ppcScoreRecords[i]->m_bGuest		)
					cOutputScoreRecordList.AddTail(ppcScoreRecords[i]);
				}
			}
		}

	delete ppcScoreRecords;

	return nRec;
}

int compare_records( const void *arg1, const void *arg2 )
{
   if( eStaticSortBy==eDaily )
		{
		if( (*(CScoreRecord**)arg1)->m_dPoints<(*(CScoreRecord**)arg2)->m_dPoints ) return  1;
		if( (*(CScoreRecord**)arg1)->m_dPoints>(*(CScoreRecord**)arg2)->m_dPoints ) return -1;
		}
   else
		{
		if( (*(CScoreRecord**)arg1)->m_dCumPoints<(*(CScoreRecord**)arg2)->m_dCumPoints ) return  1;
		if( (*(CScoreRecord**)arg1)->m_dCumPoints>(*(CScoreRecord**)arg2)->m_dCumPoints ) return -1;
		}
    return 0;
}


void CScoreRecordList::AddScore( CScoreRecord* pcNewScoreRecord )
	{
//  This function adds a pointer to the list.  No new record is
//  allocated here.

	//  If a ScoreRecord with this date is there already, remove it
	if( !IsEmpty() ) 
		{
		POSITION thispos, pos = GetHeadPosition();
		while( pos!=NULL)
			{
			thispos=pos;
			CScoreRecord* pcScoreRecord=(CScoreRecord*)CObList::GetNext(pos);
			if( pcScoreRecord->m_cDate		==pcNewScoreRecord->m_cDate		&&
				pcScoreRecord->m_cContestNo	==pcNewScoreRecord->m_cContestNo	) 
				{
				delete pcScoreRecord;
				SetAt( thispos, (CScoreRecord*)pcNewScoreRecord );
				return;
				}
			}
		}
//  If we got here, it must be a new one

	AddTail( (CScoreRecord*)pcNewScoreRecord );
	}


CScoreRecord* CScoreRecordList::Get(CString & cContestNo, CTime & cDate)
	{
	if( IsEmpty() ) return NULL;

	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		CScoreRecord* pcScoreRecord=(CScoreRecord*)CObList::GetNext(pos);
		if( DatesEqual(pcScoreRecord->m_cDate,cDate)		&&
			pcScoreRecord->m_cContestNo	==cContestNo	) 
			{
			return pcScoreRecord;
			}
		}
	return NULL;
	}

CScoreRecord* CScoreRecordList::Get(CString & cContestNo )
	{
	if( IsEmpty() ) return NULL;

	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		CScoreRecord* pcScoreRecord=(CScoreRecord*)CObList::GetNext(pos);
		if(	pcScoreRecord->m_cContestNo	==cContestNo	) 
			{
			return pcScoreRecord;
			}
		}
	return NULL;
	}


int CScoreRecordList::GetCount(EClass eClass, CTime cDate)
	{
	int iCount=0;
	if( IsEmpty() ) return 0;

	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		CScoreRecord* pcScoreRecord=(CScoreRecord*)CObList::GetNext(pos);
		if( DatesEqual(pcScoreRecord->m_cDate,cDate)		&&
			pcScoreRecord->m_eClass		==eClass	) 
			{
			iCount++;
			}
		}
	return iCount;
	}




void CScoreRecordList::CheckMinimumTaskDistance(CWSClass &cClass, CTime &cDate)
{
///////////  Check for min task distance
		double dScoredDistance=0.0;
    	POSITION pos = GetHeadPosition();
    	while(pos!=NULL)
    		{
    		CScoreRecord* pcScoreRecord=GetNext(	pos, 
    												cDate, 
    												cClass.GetType() );
    		if( !pcScoreRecord ) continue;
   
    		if( cClass.IsHandicapped() )
    			{
    			dScoredDistance=pcScoreRecord->m_dHandicappedDistance;
    			}
    		else
    			{
    			dScoredDistance=pcScoreRecord->m_dDistance;
    			}
    
    //	  --Check for Minimum Task Distance
    		if( dScoredDistance<cClass.GetMinTaskDistance(SYSTEMUNITS) )
    			{
    			if( dScoredDistance>0.0 )
    				{
    				pcScoreRecord->m_uPenalityMask |=WSP_MINDIST;
    				}
    			}
        
    		}// End for each score record
}








void CScoreRecordList::DNC(CString cContestNo, 
						   EClass eClass, 
						   CTime cDate, 
						   bool  bGuest,
						   int uMask)
	{
	CScoreRecord* pcNewScoreRecord = new CScoreRecord(	cContestNo, 
														eClass, 
														cDate,
														bGuest );

	pcNewScoreRecord->m_uPenalityMask=uMask;

	//  If a ScoreRecord with this date is there already, remove it
	if( !IsEmpty() ) 
		{
		POSITION thispos, pos = GetHeadPosition();
		while( pos!=NULL)
			{
			thispos=pos;
			CScoreRecord* pcScoreRecord=(CScoreRecord*)CObList::GetNext(pos);
			if( DatesEqual(pcScoreRecord->m_cDate,cDate)		&&
				pcScoreRecord->m_cContestNo	==cContestNo	) 
				{
				delete pcScoreRecord;
				SetAt( thispos, (CScoreRecord*)pcNewScoreRecord );
				return;
				}
			}
		}
//  If we got here, it must be a new one

	AddTail( (CScoreRecord*)pcNewScoreRecord );
	}


void CScoreRecordList::GetSummary(	CTime		cDate,
									EClass		eClass,
									CTask		*pcTask,
									double		dMinDistance,
									CSummary	&cSummary	
									)
	{

		if( pcTask->IsFAITask() ) 
			GetFAISummary(	cDate,
						eClass,
						pcTask,
						dMinDistance,
						cSummary );
		else
			GetSSASummary(	cDate,
							eClass,
							pcTask,
							dMinDistance,
							cSummary );
	}


void CScoreRecordList::GetFAISummary(	CTime		cDate,
										EClass		eClass,
										CTask		*pcTask,
										double		dMinDistance,
										CSummary	&cSummary	
										)
	{

	double d1,dToSeconds,dToHours,d2,d3;

	cSummary.m_cTd=pcTask->GetTaskTime();

	cSummary.m_dD1=GetClass(eClass).m_dD1;
	cSummary.m_dDm=GetClass(eClass).m_dDm;

	if( IsEmpty() ) return;

	cSummary.m_dHo=10.;

	// First get the *o values, but skip guests

	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		CScoreRecord* pcScoreRecord=GetNext(pos, cDate, eClass);
	    if( pcScoreRecord==NULL ) break;

		if( pcScoreRecord->m_bGuest ) continue;

		double dSpeed	=GetClass(eClass).IsHandicapped()?(pcScoreRecord->m_dHandicappedSpeed)   :(pcScoreRecord->m_dSpeed);
		double dDistance=GetClass(eClass).IsHandicapped()?(pcScoreRecord->m_dHandicappedDistance):(pcScoreRecord->m_dDistance);


		// Ho - Lowest Handicap (H) of all competitors
		if( pcScoreRecord->m_dHandicap>cSummary.m_dHo )
			{
			cSummary.m_dHo=pcScoreRecord->m_dHandicap;
			cSummary.m_strCIDHo=pcScoreRecord->m_cContestNo;
			}

		// Do - Highest Handicapped Distance (Dh) of the Day
		if( dDistance> cSummary.m_dDo )
			{
			cSummary.m_dDo=dDistance;
			cSummary.m_strCIDDo=pcScoreRecord->m_cContestNo;
			}

		if( dSpeed > cSummary.m_dVo )
			{
		    // Vo - Highest Handicapped Distance (Dh) of the Day
			cSummary.m_dVo = dSpeed;

			// To = Marking Time (T) of the finisher whose Vh = Vo.
			cSummary.m_cTo=pcScoreRecord->m_cSTOC;
			cSummary.m_strCIDVo=pcScoreRecord->m_cContestNo;
			}

		  if( dDistance>cSummary.m_dDm)  cSummary.m_nGTMinDistance++;

		}


	pos = GetHeadPosition();
	while( pos!=NULL)
		{
		CScoreRecord* pcScoreRecord=GetNext(pos, cDate, eClass);
	    if( pcScoreRecord==NULL ) break;

		// Skip guests in all tallies
		if( pcScoreRecord->m_bGuest ) continue;

		double dSpeed	=GetClass(eClass).IsHandicapped()?(pcScoreRecord->m_dHandicappedSpeed)   :(pcScoreRecord->m_dSpeed);
		double dDistance=GetClass(eClass).IsHandicapped()?(pcScoreRecord->m_dHandicappedDistance):(pcScoreRecord->m_dDistance);

		 // n1 - Number of competitors who achieve a Handicapped Distance (Dh) of at least Dm
		if( pcScoreRecord->m_dDistance >= cSummary.m_dDm  ) cSummary.m_in1++;

		 // n2 - Number of finishers exceeding 2/3 of best Handicapped Speed (Vo)
		if( pcScoreRecord->m_fFinisher && dSpeed >= cSummary.m_dVo*(2./3.) ) cSummary.m_in2++;

		 // n3 - Number of finishers, regardless of speed
		if( pcScoreRecord->m_fFinisher ) cSummary.m_in3++;

		// n4 -Number of competitors who achieve a Handicapped Distance (Dh) of at least Dm/2
		if( dDistance >= cSummary.m_dDm/2.  ) cSummary.m_in4++;

		 // N Number of competitors having had a competition launch that Day
		if( pcScoreRecord->m_fContestant ) cSummary.m_iN++;
		}					

/*
8.4.1 Racing Task
a. Day Parameters:
	Pm	= the least of: 1000 or: 1250 x (Do/D1) - 250 or: (400 x To) – 200
	F	= the lesser of 1 and (1.25 x n1 / N)
	FCR = the lesser of 1 and (1.2 x (n2/n1) + 0.6)
	Pvm = 2/3 (n2 / N) x Pm
	Pdm = Pm – Pvm

8.4.2 Assigned Area Task
a. Day Parameters:
	Pm	= the least of: 1000 or: 1250 x (Do/D1) - 250 or: (400 x To) – 200
	F	= the least of 1 and (1.25 x n1 / N)
	FCR = the lesser of 1 and (1.2 x (n2/n1) + 0.6)
	Pvm = 2/3 (n2 / N) x Pm
	Pdm = Pm – Pvm
*/

	d1=(1250. * (cSummary.m_dDo/cSummary.m_dD1))-250.;
	dToSeconds=(double)cSummary.m_cTo.GetTotalSeconds();
	dToHours=dToSeconds/(3600.);
	d2=(400. * dToHours) - 200.;
	d3=min(1000.,d1);
	if( cSummary.m_cTo==0 ) 
		cSummary.m_dPm=d3;
	else
		cSummary.m_dPm=min(d2,d3);

	cSummary.m_dPm=max(0.0,cSummary.m_dPm);

	// Day Factor
	// 	F	= the lesser of 1 and (1.25 x n1 / N)
	cSummary.m_dF	=cSummary.m_dPm/1000.;

	// Completion Ratio Factor Day
	// FCR = the lesser of 1 and (1.2 x (n2/n1) + 0.6)
	if( cSummary.m_in1>0 ) 
		cSummary.m_dFCR	=min(1.0, (1.2 * (double)((double)cSummary.m_in3 / (double)cSummary.m_in1) +0.6) );

	// 	Pvm = 2/3 (n2 / N) x Pm
	if( cSummary.m_iN>0 ) 
		cSummary.m_dPvm	=( (2./3.)* ((double)cSummary.m_in2 / (double)cSummary.m_iN)*(double )cSummary.m_dPm );
	
	//	Pdm = Pm – Pvm
	if( cSummary.m_iN>0 ) 
  		cSummary.m_dPdm	=(int)Roundoff( (2./3.)* ((double)cSummary.m_in2 / (double)cSummary.m_iN)*(double )cSummary.m_dPm ) ;

	//	Pdm = Pm – Pvm
	cSummary.m_dPdm= cSummary.m_dPm-cSummary.m_dPvm;

	}


void CScoreRecordList::GetSSASummary(	CTime		cDate,
										EClass		eClass,
										CTask		*pcTask,
										double		dMinDistance,
										CSummary	&cSummary	
										)
	{

	int			nMaxNumAssignedTurnpointsByFinishers=0;
	double		dBestSpeedGuest=0.0;
	double		dBestHCSpeedGuest=0.0;
	CString		strCIDBestSpeedGuest=_T("");
	CString		strCIDBestHCSpeedGuest=_T("");
	CTimeSpan   cTOCBestSpeedGuest;
	CTimeSpan   cTOCBestHCSpeedGuest;
	CTimeSpan	cTOCBestGuest;

	BOOL fFirst=TRUE;
	BOOL fFirstGuest=TRUE;

	if( IsEmpty() ) return;

	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		CScoreRecord* pcScoreRecord=GetNext(pos, cDate, eClass);
	    if( pcScoreRecord==NULL ) break;

		if( pcScoreRecord->m_bGuest    ) 
			{
			// Guest Pilot
			if( pcScoreRecord->m_fFinisher ) 
				{
				cSummary.m_nGuestFinishers++;
				CTimeSpan cTOC= pcScoreRecord->m_cFinishTime-pcScoreRecord->m_cStartTime;
				if( fFirstGuest )
					{
					cTOCBestGuest=cTOC;
					fFirstGuest=FALSE;
					strCIDBestSpeedGuest=pcScoreRecord->m_cContestNo;
					strCIDBestHCSpeedGuest=pcScoreRecord->m_cContestNo;
					}
				else
					{
					if( cTOC < cTOCBestGuest ) cTOCBestGuest=cTOC;
					}

				CTimeSpan cTimeUnder=pcTask->m_cPostTime-cTOC;
				if( pcTask->m_eType!=eAssigned && pcTask->m_eType!=eFAIRacing && cTimeUnder.GetTotalSeconds()>900 ) 
					{
    				pcScoreRecord->m_uPenalityMask |=WSP_MINTIME15;
					}

				}

			if( pcScoreRecord->m_dScoredSpeed>dBestSpeedGuest ) 
				{
				dBestSpeedGuest=pcScoreRecord->m_dScoredSpeed;
				cTOCBestSpeedGuest= pcScoreRecord->m_cFinishTime-pcScoreRecord->m_cStartTime;
				strCIDBestSpeedGuest=pcScoreRecord->m_cContestNo;
				}
			if( pcScoreRecord->m_dScoredHandicappedSpeed>dBestHCSpeedGuest ) 
				{
				dBestHCSpeedGuest=pcScoreRecord->m_dScoredHandicappedSpeed;
				cTOCBestHCSpeedGuest=pcScoreRecord->m_cFinishTime-pcScoreRecord->m_cStartTime;
				strCIDBestHCSpeedGuest=pcScoreRecord->m_cContestNo;
				}
			}
		else //  Non-Guest
			{

			if( pcScoreRecord->m_dDistance>cSummary.m_dBestDistance ) 
				{
				cSummary.m_dBestDistance		=pcScoreRecord->m_dDistance;
				cSummary.m_strCIDBestDistance	=pcScoreRecord->m_cContestNo;
				}
			if( pcScoreRecord->m_dHandicappedDistance>cSummary.m_dBestHdcapDist ) 
				{
				cSummary.m_dBestHdcapDist		=pcScoreRecord->m_dHandicappedDistance;
				cSummary.m_strCIDBestHCDistance	=pcScoreRecord->m_cContestNo;
				}

			if( pcScoreRecord->m_fFinisher ) 
				{
				(cSummary.m_nFinishers)++;
				CTimeSpan cTOC= pcScoreRecord->m_cFinishTime-pcScoreRecord->m_cStartTime;
				if( fFirst )
					{
					fFirst=FALSE;
					cSummary.m_cTOCBest=cTOC;
					cSummary.m_strCIDBestSpeed		=pcScoreRecord->m_cContestNo;
					cSummary.m_strCIDBestHCSpeed	=pcScoreRecord->m_cContestNo;
					cSummary.m_strCIDSlowestSpeed	=pcScoreRecord->m_cContestNo;
					cSummary.m_strCIDSlowestHCSpeed	=pcScoreRecord->m_cContestNo;
					cSummary.m_dDistaceOfFinisherWithBestSpeed	=pcScoreRecord->m_dDistance;
					}
				else
					{
					if( cTOC < cSummary.m_cTOCBest ) cSummary.m_cTOCBest=cTOC;
					}

				// best scored speeds.
				if( pcScoreRecord->m_dScoredSpeed>cSummary.m_dBestScoredSpeed ) 
					{
					cSummary.m_dBestScoredSpeed	=pcScoreRecord->m_dScoredSpeed;
					cSummary.m_cTOCBestSpeed	=pcScoreRecord->m_cFinishTime-pcScoreRecord->m_cStartTime;
					cSummary.m_strCIDBestSpeed	=pcScoreRecord->m_cContestNo;
					cSummary.m_dDistaceOfFinisherWithBestSpeed	=pcScoreRecord->m_dDistance;
					}
				if( pcScoreRecord->m_dScoredHandicappedSpeed>cSummary.m_dBestScoredHdcapSpeed ) 
					{
					cSummary.m_dBestScoredHdcapSpeed=pcScoreRecord->m_dScoredHandicappedSpeed;
					cSummary.m_cTOCBestHdcapSpeed	=pcScoreRecord->m_cFinishTime-pcScoreRecord->m_cStartTime;
					cSummary.m_strCIDBestHCSpeed	=pcScoreRecord->m_cContestNo;
					}
				// Best Distance for finishers
				cSummary.m_dBestDistanceFinisher	=max(cSummary.m_dBestDistanceFinisher,  pcScoreRecord->m_dDistance);
				cSummary.m_dBestHdcapDistFinisher	=max(cSummary.m_dBestHdcapDistFinisher, pcScoreRecord->m_dHandicappedDistance);

				// Best non scored speeds.
				if( pcScoreRecord->m_dSpeed>cSummary.m_dBestSpeed ) 
					{
					cSummary.m_dBestSpeed		=pcScoreRecord->m_dSpeed;
					}
				if( pcScoreRecord->m_dHandicappedSpeed>cSummary.m_dBestHdcapSpeed ) 
					{
					cSummary.m_dBestHdcapSpeed		=pcScoreRecord->m_dHandicappedSpeed;
					}

				// Slowest scored speeds
				if( pcScoreRecord->m_dScoredSpeed<cSummary.m_dSlowestScoredSpeed ) 
					{
					cSummary.m_dSlowestScoredSpeed	=pcScoreRecord->m_dScoredSpeed;
					cSummary.m_strCIDSlowestSpeed	=pcScoreRecord->m_cContestNo;
					}

				if( pcScoreRecord->m_dScoredHandicappedSpeed<cSummary.m_dSlowestScoredHdcapSpeed ) 
					{
					cSummary.m_dSlowestScoredHdcapSpeed	=pcScoreRecord->m_dScoredHandicappedSpeed;
					cSummary.m_strCIDSlowestHCSpeed	=pcScoreRecord->m_cContestNo;
					}

				// Slowest Non scored speeds
				if( pcScoreRecord->m_dSpeed<cSummary.m_dSlowestSpeed ) 
					{
					cSummary.m_dSlowestSpeed		=pcScoreRecord->m_dScoredSpeed;
					}

				if( pcScoreRecord->m_dHandicappedSpeed<cSummary.m_dSlowestHdcapSpeed ) 
					{
					cSummary.m_dSlowestHdcapSpeed	=pcScoreRecord->m_dHandicappedSpeed;
					}

				CTimeSpan cTimeUnder=pcTask->m_cPostTime-cTOC;
				int iTimeUnder=(int)cTimeUnder.GetTotalSeconds();
				if( pcTask->m_eType!=eAssigned && pcTask->m_eType!=eFAIRacing && iTimeUnder>900 ) 
					{
    				pcScoreRecord->m_uPenalityMask |=WSP_MINTIME15;
					cSummary.m_nFinTocGT15underMinTime++;
					}
				}


			if( GetClass(eClass).IsHandicapped()  )
				{
				if( pcScoreRecord->m_dHandicappedDistance>dMinDistance ) 
					{
					cSummary.m_nGTMinDistance++;
					}
				}
			else
				{
				if( pcScoreRecord->m_dDistance>dMinDistance ) 
					{
					cSummary.m_nGTMinDistance++;
					}
				}

			if( pcScoreRecord->m_dDistance>0.0 ) cSummary.m_nContestants++;

			}

		}



	// Trap Special case where the guests were the only finishers.

    if( cSummary.m_nFinishers==0 && cSummary.m_nGuestFinishers>0 )
	{
            // Guests were the only finishers.  Set the best speed& such for Them.

            cSummary.m_dBestSpeed             =dBestSpeedGuest;
            cSummary.m_cTOCBestSpeed   =cTOCBestSpeedGuest;
            cSummary.m_strCIDBestSpeed =strCIDBestSpeedGuest;

            cSummary.m_dBestHdcapSpeed=       dBestHCSpeedGuest;
            cSummary.m_dBestScoredHdcapSpeed=dBestHCSpeedGuest;
            cSummary.m_cTOCBestHdcapSpeed=    cTOCBestHCSpeedGuest;
            cSummary.m_strCIDBestHCSpeed=     cSummary.m_strCIDBestHCSpeed;

            cSummary.m_cTOCBest=cTOCBestGuest;
            }             
							

	}



void CScoreRecordList::LoadScoreRecordList(	CListCtrl&	ListCtrl, 
											CTime		cDate, 
											EClass		eClass, 
											CTask		*pcTask, 
											CContestantList& cContestantList,
											CPenalityList& cPenalityList,
											EUnits		eUnits)
{
	CScoreRecord* pcScoreRecord;

	LV_ITEM lvi;
	int i=0;

	lvi.iSubItem = 0;
	lvi.mask = LVIF_PARAM;

	ListCtrl.DeleteAllItems( );

	//  Load up the listbox
	int iItem=0;
	POSITION pos = GetHeadPosition();
	while( pos!=NULL)
		{
		pcScoreRecord=GetNext(pos,cDate, eClass);
		if( pcScoreRecord )
			{
			CContestant* pcContestant=cContestantList.GetByContestNo(pcScoreRecord->m_cContestNo);
			ASSERT(pcContestant);
			if( pcContestant ) 
				{
				lvi.iItem = iItem;
				lvi.lParam = (LPARAM)pcScoreRecord;
				ListCtrl.InsertItem(&lvi);
				i=0;
				pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->PointsText();
				pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->CumPointsText();
				pcScoreRecord->m_acColumnText[i++]=(pcScoreRecord->m_fContestant)?((pcContestant->IsGuest())?(pcContestant->CitizenText()):(_T("Yes"))):(_T("No"));
				pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->m_cContestNo;
				pcScoreRecord->m_acColumnText[i++]=pcContestant->m_strLastName;
				pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->StatusText();
				if( !GetClass(eClass).IsHandicapped() )
					{
					pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->DistanceText(eUnits);
					pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->SpeedText(eUnits);
					}
				else
					{
					pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->DistanceText(eUnits);
					pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->HdcapDistanceText(eUnits);
					pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->SpeedText(eUnits);
					pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->HdcapSpeedText(eUnits);
					}
				pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->TOCText();
				pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->StartTimeText();
				pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->FinishTimeText();
				pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->AppliedPenalityText(cPenalityList);
				pcScoreRecord->m_acColumnText[i++]=pcScoreRecord->PenalityText()+cPenalityList.DayText( pcScoreRecord->m_cContestNo, cDate );

				for( int j=0; j<i; j++ ) ListCtrl.SetItemText(iItem,j,pcScoreRecord->m_acColumnText[j] );
				iItem++;
				}
			}
		}
	}

void CScoreRecordList::ApplyPenalties(CTime &cDate, EClass eClass, CPenalityList &cPenalityList, double dMaxDayPoints)
{

    	POSITION pos = GetHeadPosition();
    	while(pos!=NULL)
    		{
    		CScoreRecord* pcScoreRecord=GetNext(pos, cDate, eClass);
    		if( pcScoreRecord==NULL ) break;
    
    		if( pcScoreRecord->m_dDistance<=0 ) 
    			{
    			pcScoreRecord->m_dPoints=0; 
    			continue;
    			}
    
    		//  Apply penality percents
    		CPenality* pcPenality=cPenalityList.Get(	pcScoreRecord->m_cContestNo, 
    													pcScoreRecord->m_cDate, 
    													eDayPercent);
    //		double dPrePenalityPoints=pcScoreRecord->m_dPoints;
    		if( pcPenality )
    			{
    //			pcScoreRecord->m_dPoints=pcPenality->ApplyDailyPenality(pcScoreRecord->m_dPoints, dMaxDayPoints);
	   			pcScoreRecord->m_dAppliedPenalityPoints=pcPenality->CalculateDailyPenality(dMaxDayPoints);
                pcScoreRecord->m_dAppliedPenalityPoints=min(pcScoreRecord->m_dAppliedPenalityPoints,pcScoreRecord->m_dPoints);
    			}
    
    		//  Apply penality points
			bool bFirst=true;
			while(1)
				{
    			pcPenality=cPenalityList.GetNext(	pcScoreRecord->m_cContestNo, 
    												pcScoreRecord->m_cDate, 
    												eDayPoint,
													bFirst);
    			if( !pcPenality ) break;
				bFirst=false;

    			pcScoreRecord->m_dAppliedPenalityPoints+=pcPenality->CalculateDailyPenality(dMaxDayPoints);
				}

    		pcScoreRecord->m_dPoints-=pcScoreRecord->m_dAppliedPenalityPoints;
    		pcScoreRecord->m_dPoints=max( 0.0, pcScoreRecord->m_dPoints);
    
    
    		}// End for each score record
}



void CScoreRecordList::CalculateSpeeds(	CTask*		pcTask, 
    									CTime		cDate, 
    									CWSClass   &cClass,
										int			nMaxAcheivedAssignedTurnpoints,
    									CWinscoreListCtrl& cStatus,
										CTurnpointArray &turnpointArray)
	{
   	double dScoredDistance=0.0;
    	CString cError;
    	CTimeSpan  cFlightTime;

    	POSITION pos = GetHeadPosition();
    	while(pos!=NULL)
    		{
    		CScoreRecord* pcScoreRecord=GetNext(	pos, 
    												cDate, 
    												cClass.GetType());
    		if( !pcScoreRecord ) continue;
    
    		if( pcScoreRecord->m_uPenalityMask & WSP_NOSTART  ||
    			pcScoreRecord->m_uPenalityMask & WSP_NOLAUNCH   ) continue;
    
    		if( cClass.IsHandicapped()  )
    			{
    			dScoredDistance=pcScoreRecord->m_dHandicappedDistance;
    			}
    		else
    			{
    			dScoredDistance=pcScoreRecord->m_dDistance;
    			}
     
    
    		if( pcScoreRecord->m_fContestant && pcScoreRecord->m_fFinisher )
    			{
    			cFlightTime=pcScoreRecord->m_cFinishTime-pcScoreRecord->m_cStartTime;
    			int iSeconds=(int)cFlightTime.GetTotalSeconds();
    			double dHours=((double)iSeconds)/(60.*60.);
  				pcScoreRecord->m_cSTOC=cFlightTime;
  
    			if(  pcTask->m_eType==eAssigned || pcTask->m_eType==eFAIRacing )
    				{
    				if( cClass.IsHandicapped() )
    					{
    					GetSpeed(	pcScoreRecord->m_dHandicappedDistance, 
    								cFlightTime, 
    								&pcScoreRecord->m_dHandicappedSpeed);
    					GetSpeed(	pcScoreRecord->m_dDistance, 
    								cFlightTime, 
    								&pcScoreRecord->m_dSpeed);

						pcScoreRecord->m_dScoredHandicappedSpeed=pcScoreRecord->m_dHandicappedSpeed;

    					}
    				else
    					{
    					GetSpeed(dScoredDistance,cFlightTime, &pcScoreRecord->m_dSpeed);
    					}
					pcScoreRecord->m_cSTOC=cFlightTime;
					pcScoreRecord->m_dScoredSpeed=pcScoreRecord->m_dSpeed;
					pcScoreRecord->m_dScoredHandicappedSpeed=pcScoreRecord->m_dScoredHandicappedSpeed;
    				}
    			else if( pcTask->IsTimedTask() )
    				{
    				if( cClass.IsHandicapped() )
    					{
/*
2017 Rules Change
11.6.6 Scored Time on Course:
For finishers whose TOC is not less than MINTIME: STOC = TOC
For finishers whose TOC is less than MINTIME: STOC = MINTIME - (MINTIME - TOC) / 10
*/
    					if( pcTask->m_cPostTime<=cFlightTime )
    						{
    						GetSpeed(	pcScoreRecord->m_dHandicappedDistance, 
    									cFlightTime, 
    									&pcScoreRecord->m_dHandicappedSpeed);
    						GetSpeed(	pcScoreRecord->m_dDistance, 
    									cFlightTime, 
    									&pcScoreRecord->m_dSpeed);
							pcScoreRecord->m_cSTOC=cFlightTime;
    						}
    					else
							{ // Under time
							CTimeSpan MINTIME = pcTask->m_cPostTime;
							CTimeSpan TOC = cFlightTime;
							int iSeconds = 0;
							if( pcTask->IsFAITask() )
							  iSeconds = (int)(MINTIME.GetTotalSeconds()); 
							else
							  iSeconds = (int)(MINTIME.GetTotalSeconds() - (MINTIME.GetTotalSeconds() - TOC.GetTotalSeconds())/10 ); 
							CTimeSpan STOC(0,0,0,iSeconds);
							pcScoreRecord->m_cSTOC=STOC;

   							pcScoreRecord->m_uPenalityMask |=WSP_MINTIME;
    						GetSpeed(	pcScoreRecord->m_dDistance, 
    									STOC, 
    									&pcScoreRecord->m_dSpeed);
    						GetSpeed(	pcScoreRecord->m_dHandicappedDistance, 
    									STOC, 
    									&pcScoreRecord->m_dHandicappedSpeed);
							}
    					}
    				else //class is not handicapped
    					{
    					if( pcTask->m_cPostTime<=cFlightTime )
							{
    						GetSpeed(	pcScoreRecord->m_dDistance, 
    									cFlightTime, 
    									&pcScoreRecord->m_dSpeed);
    						GetSpeed(	pcScoreRecord->m_dDistance, 
    									cFlightTime+CTimeSpan(0,0,15,0),
    									&pcScoreRecord->m_dScoredSpeed);
							pcScoreRecord->m_cSTOC=cFlightTime;

							}
    					else
							{ // Under Time
							CTimeSpan MINTIME = pcTask->m_cPostTime;
							CTimeSpan TOC = cFlightTime;
							int iSeconds = 0;
							if( pcTask->IsFAITask() )
							   iSeconds = (int)(MINTIME.GetTotalSeconds()); 
							else
							   iSeconds = (int)(MINTIME.GetTotalSeconds() - ((MINTIME.GetTotalSeconds() - TOC.GetTotalSeconds())/10 )); 

							CTimeSpan STOC(0,0,0,iSeconds);
							pcScoreRecord->m_cSTOC=STOC;

    						pcScoreRecord->m_uPenalityMask |=WSP_MINTIME;

    						GetSpeed(	pcScoreRecord->m_dDistance, 
    									STOC, 
    									&pcScoreRecord->m_dSpeed);
							pcScoreRecord->m_cSTOC=STOC;
    						}
    					}
    
    				} // end if task type=assigned/post
    
    			}// End if finisher
    

			// Force the scored speed to be = actual speed because the stupid
			// 15 minute rule was eliminated in 2004
			pcScoreRecord->m_dScoredHandicappedSpeed=pcScoreRecord->m_dHandicappedSpeed;
			pcScoreRecord->m_dScoredSpeed=pcScoreRecord->m_dSpeed;

    		}// End for each score record

	}

void CScoreRecordList::CalculatePoints(	CTime		cDate,  
    									CWSClass	&cClass,
    									CTask		*pcTask,
										CSummary	&cSummary )
    
    {
    
   	POSITION pos = GetHeadPosition();
   	while(pos!=NULL)
   		{
   		CScoreRecord* pcScoreRecord=GetNext(pos, cDate, cClass.GetType() );
   		if( pcScoreRecord==NULL ) break;
    
		cClass.GetPoints( pcTask, pcScoreRecord, cSummary);
    
   		}// End for each score record
    
// for FAI tasks, we need an extra step to convert Sp (provisional scores) to actual scores.

	   if( pcTask->IsFAITask() )
		   {
		   cSummary.m_dSpo =GetMaxDayPoints(cDate,  cClass.GetType());
		   cSummary.m_dSpm =GetMedianDayPoints(cDate,  cClass.GetType());
	       double dDenom = ( cSummary.m_dSpo-cSummary.m_dSpm);
		   if( dDenom<1.e-10 ) return;

   		   POSITION pos = GetHeadPosition();
   		   while(pos!=NULL)
   				{
   				CScoreRecord* pcScoreRecord=GetNext(pos, cDate, cClass.GetType() );
   				if( pcScoreRecord==NULL ) break;
    
	    		double dS = pcScoreRecord->m_dPoints * min(1.0, 200./dDenom);
				pcScoreRecord->m_dPoints=dS;

				// Now add the Bonus
				if( pcScoreRecord->m_uPenalityMask & WSP_AIRFIELDBONUS )
					{
					pcScoreRecord->m_dPoints+=cClass.m_iAirfieldBonusPoints;
					}
    			}
	      }
    	return;
    	}

double CScoreRecordList::GetMaxDayPoints(CTime &cDate, EClass eClass)
	{
	double dMaxDayPoints=0;
   	POSITION pos = GetHeadPosition();
   	while(pos!=NULL)
   		{
   		CScoreRecord* pcScoreRecord=GetNext(pos, cDate, eClass );
   		if( pcScoreRecord==NULL ) break;
    
       	if( !pcScoreRecord->m_bGuest    ) 
    		{
    		dMaxDayPoints=max( pcScoreRecord->m_dPoints, dMaxDayPoints);
    		}

   		}// End for each score record
    
    return dMaxDayPoints;
	}

double CScoreRecordList::GetMedianDayPoints(CTime &cDate, EClass eClass)
	{
    std::vector<double> scores;
   	POSITION pos = GetHeadPosition();
   	while(pos!=NULL)
   		{
   		CScoreRecord* pcScoreRecord=GetNext(pos, cDate, eClass );
   		if( pcScoreRecord==NULL ) break;
    
       	if( pcScoreRecord->m_bGuest ) continue;
		if( pcScoreRecord->m_dPoints==0.0 ) continue;
    	
		scores.push_back(max(0.0,pcScoreRecord->m_dPoints));

   		}// End for each score record
    
	if( scores.size()==0 ) 
		return 0.0;
    else
	  {
		std::sort(scores.begin(), scores.end());
		if (scores.size() % 2 == 0)
		{
		  return (scores[scores.size() / 2 - 1] + scores[scores.size() / 2]) / 2;
		}
		else 
		{
		  return scores[scores.size() / 2];
		}
	  }
	}


int CScoreRecordList::CalculateNumAssignedTurnpointsAcheived( CTask*	 pcTask,
   															  CTime		 &cDate, 
    														  CWSClass   &cClass)
	{  	

	if( pcTask->m_eType!=eModifiedAssigned ) return 0;

	int nMaxNumAssignedTurnpointsByFinishers=0;
	POSITION pos = GetHeadPosition();
    while(pos!=NULL)
    	{
    	CScoreRecord* pcScoreRecord=GetNext(	pos, 
    											cDate, 
    											cClass.GetType());
    	if( !pcScoreRecord ) continue;

    	if( !pcScoreRecord->m_fFinisher ) continue;

		pcScoreRecord->m_nAcheivedTurnpoints= pcTask->GetNumTurnpoints() ;
		
		// skip guests
		if( pcScoreRecord->m_bGuest) continue;

		nMaxNumAssignedTurnpointsByFinishers=max(nMaxNumAssignedTurnpointsByFinishers, pcScoreRecord->m_nAcheivedTurnpoints );
		}
	return nMaxNumAssignedTurnpointsByFinishers;
	}

void CScoreRecordList::GetXML(CXMLMgr &cMgr, EClass eClass, CTime cDate, MSXML2::IXMLDOMNodePtr &pParentNode)
	{

	MSXML2::IXMLDOMNodePtr pIDOMChildNode;
	cMgr.CreateChild(pParentNode,pIDOMChildNode,_T("ScoreRecords"));

	if( !IsEmpty() ) 
		{
		POSITION thispos, pos = GetHeadPosition();
		while( pos!=NULL)
			{
			thispos=pos;
			CScoreRecord* pcScoreRecord=(CScoreRecord*)CObList::GetNext(pos);
			if( DatesEqual(pcScoreRecord->m_cDate,cDate)	&&
				pcScoreRecord->m_eClass	==eClass	) 
				{
				pcScoreRecord->GetXML(cMgr, pIDOMChildNode);
				}
			}
		}
	}

void CScoreRecordList::GetXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParentNode)
	{

	MSXML2::IXMLDOMNodePtr pIDOMChildNode;
	cMgr.CreateChild(pParentNode,pIDOMChildNode,_T("ScoreRecords"));

	if( !IsEmpty() ) 
		{
		POSITION thispos, pos = GetHeadPosition();
		while( pos!=NULL)
			{
			thispos=pos;
			CScoreRecord* pcScoreRecord=(CScoreRecord*)CObList::GetNext(pos);
			pcScoreRecord->GetXML(cMgr,pIDOMChildNode);
			}
		}
	}

int CScoreRecordList::ImportXML(CXMLMgr &cMgr, MSXML2::IXMLDOMNodePtr &pParent)
	{
	int nImported=0;

	MSXML2::IXMLDOMNodePtr pRecords;
	TESTHR(cMgr.SelectChild(_T("ScoreRecords"),pParent,pRecords));
	if( pRecords==NULL ) return 0;


	MSXML2::IXMLDOMNodeListPtr pRecordList=cMgr.SelectNodeList(_T("ScoreRecord"), pRecords );
	if( pRecordList==NULL ) return 0;

	long length=0;
	TESTHR(pRecordList->get_length(&length));
	if( length==0 ) return 0;

	for( int i=0; i<length; i++ )
		{
		MSXML2::IXMLDOMNodePtr pRecord;
		TESTHR(pRecordList->get_item(i,&pRecord));
		CScoreRecord* pcRecord=NULL;
		try
			{
			pcRecord=new CScoreRecord( cMgr, pRecord );
			}
		catch(...)
			{
			ASSERT(FALSE);
			if(pcRecord) delete pcRecord;
			continue;
			}

		AddScore(pcRecord);

		nImported++;
		}

	return nImported;
	}


